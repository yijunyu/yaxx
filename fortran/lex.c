/* File: lib/lex.c, created by AT&T (f2c authors)	 */
/* Recognize Fortran tokens.				 */
/* 1992-1996, ZFB, several changes			 */
/* 25.06.96, BVA, moved definition of LEX_QUOTE to lex.h */


#include <assert.h>
#include <ctype.h>
// #include <iostream.h> // cout
#include <iostream> // cout
#ifndef WIN32
// #include <stream.h>   // form()
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "fortran.tab.h"


#define DEBUG_LEX 0


#define MAXINCLUDES 10
#define MAXTOKENLEN 10000
#define BLANK   ' '
#define SEOF    0
/* max continuation lines for ! processing */
#define CONTMAX 100
#define USC (unsigned char const*)
#define isalnum_(x) anum_buf[x]
#define isalpha_(x) (anum_buf[x] == 1)


/* card types */
enum { STEOF=1, STINITIAL=2, STCONTINUE=3, STCOMMENT=4 };

/* lex states */
enum { NEWSTMT=1, FIRSTTOKEN=2, OTHERTOKEN=3, RETEOS=4 };

/* struct Inclfile   holds the state information for each include file */
struct Inclfile {
    struct Inclfile *inclnext;
    FILEP            inclfp;
    char const*      inclname;
    int              incllno;
    char const*      incllinp;
    int              incllen;
    int              inclcode;
    ftnint           inclstno;
};

LOCAL struct Inclfile* inclp = NULL;
struct Keylist {
    char const* keyname;
    int         keyval;
    char        notinf66;
};
struct Punctlist {
    char punchar;
    int  punval;
};
struct Dotlist {
    char const* dotname;
    int         dotval;
};



static int  popinclude(void);
static int  getcds(void);
static int  getcd(char *b, int nocont);
static void crunch(void);
static void analyz(void);
static int  getkwd(void);
static int  hexcheck(int key);
static int  gettok(void);

static FILE* infile;
int          lex_toklen;
int   lineno;
int   startcolno; // KB 29.06.2001 the start column of the current token
                  // white space is not counted
int   endcolno;   // KB 29.06.2001 the end columnd number of the current 
                  // token. White space is not counted as a column.
static char const* infname;
static int   needkwd;
static int   inioctl;
static int   inside_comment_directive = 0;

/* BVA, 25.03.98 */
static char  escapes[Table_size] = {
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0,    0,    0,    0,
    ' ',  '!',  '"',  '#',  '$',  '%',  '&', '\'',
    '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
    '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',
    '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
    '@', '\a', '\b',  'C',  'D',  'E', '\f',  'G',
    'H',  'I',  'J',  'K',  'L',  'M', '\n',  'O',
    'P',  'Q', '\r',  'S', '\t',  'U', '\v',  'W',
    'X',  'Y',  'Z',  '[', '\\',  ']',  '^',  '_',
    '`', '\a', '\b',  'c',  'd',  'e', '\f',  'g',
    'h',  'i',  'j',  'k',  'l',  'm', '\n',  'o',
    'p',  'q', '\r',  's', '\t',  'u', '\v',  'w',
    'x',  'y',  'z',  '{',  '|',  '}',  '~',    0, 
};
/* static added by BVA, 01.10.97 */
static int  new_dcl;
char        lex_token[MAXTOKENLEN];
int         label_no;
LOCAL int   stkey;    /*  Type of the current statement (DO, END, IF, etc) */
static int  needwkey;
ftnint      yystno;
static flag intonly;
LOCAL long int stno;
LOCAL long int nxtstno;    /* Statement label */
LOCAL int   parlev;    /* Parentheses level */
LOCAL int   parseen;
LOCAL int   expcom;
LOCAL int   expeql;
LOCAL char const* nextch;
char*       lastch;
LOCAL char* nextcd = NULL;
LOCAL char* endcd;
LOCAL long  prevlin;
LOCAL long  thislin;
LOCAL int   code;        /* Card type; INITIAL, CONTINUE or EOF */
LOCAL int   lexstate = NEWSTMT;

/* Main buffer for Fortran source input.  The number */
/* comes from lines of at most 66 characters, with at */
/* most 20 continuation cards (or something); this is */
/* part of the defn of the standard */
char sbuf[13900];
LOCAL char *send = sbuf + 20*66;
/* Current number of include files */
LOCAL int nincl = 0;
LOCAL long firstline;
static char* linestart[CONTMAX];
LOCAL int ncont;
LOCAL char comment_start[Table_size];


static char anum_buf[Table_size];


LOCAL struct Keylist const* keystart[26];
LOCAL struct Keylist const* keyend[26];
LOCAL char* stbuf[3];


/* KEYWORD AND SPECIAL CHARACTER TABLES
*/

static struct Punctlist puncts[ ] = 
{
    { '(', SLPAR     },
    { ')', SRPAR     },
    { '=', SEQUALS   },
    { ',', SCOMMA    },
    { '+', SPLUS     },
    { '-', SMINUS    },
    { '*', SSTAR     },
    { '/', SSLASH    },
    { '$', SCURRENCY },
    { ':', SCOLON    },
    { '<', SLT       },
    { '>', SGT       },
    { '%', SPERCENT  },
    { 0,   0         },
};


LOCAL struct Dotlist const dots[] =
{
    { "and.",   SAND   },
    { "or.",    SOR    },
    { "not.",   SNOT   },
    { "true.",  STRUE  },
    { "false.", SFALSE },
    { "eq.",    SEQ    },
    { "ne.",    SNE    },
    { "lt.",    SLT    },
    { "le.",    SLE    },
    { "gt.",    SGT    },
    { "ge.",    SGE    },
    { "neqv.",  SNEQV  },
    { "eqv.",   SEQV   },
    { 0,        0      },
};


LOCAL struct Keylist const keys[] = {
    { "assign",          SASSIGN         },
    { "automatic",       SAUTOMATIC, YES },
    { "backspace",       SBACKSPACE      },
    { "blockdata",       SBLOCK          },
    { "call",            SCALL           },
    { "character",       SCHARACTER, YES },
    { "close",           SCLOSE,     YES },
    { "common",          SCOMMON         },
    { "complex",         SCOMPLEX        },
    { "continue",        SCONTINUE       },
    { "data",            SDATA           },
    { "dimension",       SDIMENSION      },
    { "doisv",           SDOISV          },
    { "dossv",           SDOSSV          },
    { "enddossv",        SENDDOSSV       },
    { "dosingle",        SDOSINGLE       },
    { "doubleprecision", SDOUBLE         },
    { "doublecomplex",   SDCOMPLEX,  YES },
    { "elseif",          SELSEIF,    YES },
    { "else",            SELSE,      YES },
    { "endfile",         SENDFILE        },
    { "endif",           SENDIF,     YES },
    { "enddo",           SENDDO,     YES },
    { "end",             SEND            },
    { "entry",           SENTRY,     YES },
    { "equivalence",     SEQUIV          },
    { "exit",            SEXIT           },
    { "external",        SEXTERNAL       },
    { "format",          SFORMAT         },
    { "function",        SFUNCTION       }, 
    { "granularity",     SGRANULARITY    }, 
    { "goto",            SGOTO           },
    { "ifsingle",        SIFSINGLE       },
    { "implicit",        SIMPLICIT,  YES },
    { "include",         SINCLUDE,   YES },
    { "inquire",         SINQUIRE,   YES },
    { "intrinsic",       SINTRINSIC, YES },
    { "integer",         SINTEGER        },
    { "local",           SLOCAL          },
    { "logical",         SLOGICAL        },
    { "namelist",        SNAMELIST,  YES },
    { "nodep",           SNODEP          },
    { "none",            SUNDEFINED, YES },
    { "open",            SOPEN,      YES },
    { "pardoall",        SDOALL          },
    { "parameter",       SPARAM,     YES },
    { "pause",           SPAUSE          },
    { "print",           SPRINT          },
    { "program",         SPROGRAM,   YES },
    { "punch",           SPUNCH,     YES },
    { "read",            SREAD           },
    { "real",            SREAL           },      
    { "repeat",          SREPEAT         },
    { "return",          SRETURN         },
    { "rewind",          SREWIND         },
    { "save",            SSAVE,      YES },
    { "static",          SSTATIC,    YES },
    { "stop",            SSTOP           },
    { "subroutine",      SSUBROUTINE     },
    { "then",            STHEN,      YES },
    { "undefined",       SUNDEFINED, YES },
    { "unroll",          SUNROLL         },   
    { "until",           SUNTIL          },
    { "while",           SWHILE,     YES },
    { "write",           SWRITE          },  
    { "noclustering",    SNOCLUS         },
    { "clustering",      SCLUS           },
    { "statement",       SSTATE          },
    { "operation",       SOPER           },
    { 0,                 0               },
};


/* copyn -- return a new copy of the input */
/* Fortran-string (s) of length (n) */

static char *copyn(int n, const char *const s)
{
    assert(s); /* BVA, 02.09.97 */
    char* const p = new char[n];
    assert(p);
    return (n ? (char *)memcpy(p, s, n) : 0);
}



static int hextoi(char cc)
{
    switch (cc) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return(cc - '0');
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
	return (cc - 'a' + 10);
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
        return (cc - 'A' + 10);
    default:
        printf("lex: number conversion error for character '%c'\n", cc);
        assert(0);
    }
    return 0;
}


static int letter(char cc)
{
    if ((cc <= 'z') && (cc >= 'a'))
        return(cc - 'a');
    if ((cc <= 'Z') && (cc >= 'A'))
        return(cc - 'A');
    return 0;
}



int inilex(char const* const name)
{
    stbuf[0] = new char[3 * P1_STMTBUFSIZE];
    assert(stbuf[0]);
    stbuf[1] = stbuf[0] + P1_STMTBUFSIZE;
    stbuf[2] = stbuf[1] + P1_STMTBUFSIZE;
    nincl = 0;
    inclp = NULL;
    doinclude(name);
    lexstate = NEWSTMT;
    return NO;
}


int quitlex(void)
{
    delete[] stbuf[0];
    return NO;
}


void doinclude(char const* const name)
{
    FILEP fp;
    struct Inclfile *t;

    if (inclp) {
        inclp->incllno = thislin;
        inclp->inclcode = code;
        inclp->inclstno = nxtstno;
        if (nextcd)
            inclp->incllinp = copyn(inclp->incllen = endcd-nextcd, nextcd);
        else
            inclp->incllinp = 0;
    }
    nextcd = NULL;

    if (++nincl >= MAXINCLUDES)
        printf("lex: includes nested too deep\n");
    if (name[0] == 0)
        fp = stdin;
    else
        fp = fopen(name, "r");
    if (fp) {
        t = inclp;
        inclp = new Inclfile;
        inclp->inclnext = t;
        prevlin = thislin = 0;
        infname = inclp->inclname = name;
        infile = inclp->inclfp = fp;
    } 
    else {
        printf("lex: Cannot open file %s\n", name);
        exit(10);
    }
}




LOCAL int popinclude(void)
{
    struct Inclfile* t;
    char const*      p;
    int              k;

    if (infile != stdin)
        fclose(infile);    /* Close the input file */

    --nincl;
    t = inclp->inclnext;

    delete inclp;

    inclp = t;
    if (inclp == NULL) {
        infname = 0;
        return(NO);
    }

    infile = inclp->inclfp;
    infname = inclp->inclname;
    prevlin = thislin = inclp->incllno;
    code = inclp->inclcode;
    stno = nxtstno = inclp->inclstno;
    if (inclp->incllinp) {
        endcd = nextcd = sbuf;
        k = inclp->incllen;
        p = inclp->incllinp;
        while (--k >= 0)
            *endcd++ = *p++;
        delete[] (char*)inclp->incllinp;
    }
    else
        nextcd = NULL;
    return(YES);
}

static char fortrantext_buf[1000];
char *fortrantext;

extern "C" {
int fortranlex(void)
{
    static int    tokno;
    int  retval;
    int lencol = 0;

    switch (lexstate) {
    case NEWSTMT: /* need a new statement */
        retval = getcds();
        if (retval == STEOF) {
            retval = SEOF;
            break;
        }
        if (retval == STCOMMENT) {
            strcpy(lex_token, nextcd);
            retval = SCOMMENT;
            lexstate = RETEOS;
            nextcd = NULL;
            break;
        }
        crunch();
        tokno      = 0;
        lexstate   = FIRSTTOKEN;
        yystno     = stno;
        stno       = nxtstno;
        label_no   = yystno;
        lex_toklen = 0;
        retval     = SLABEL;
        break;

first:
    case FIRSTTOKEN:    /* first step on a statement */
        analyz();
        lexstate = OTHERTOKEN;
        tokno = 1;
        retval = stkey;
        if (retval == SFORMAT || retval == SOPEN) {
            strncpy(lex_token, nextch, lastch - nextch + 1);
            lex_token[lastch - nextch + 1] = 0;
            nextch = lastch + 1;
        }
        break;

    case OTHERTOKEN:    /* return next token */
        if (nextch > lastch)
            goto reteos;
        ++tokno;
        if ((stkey == SLOGIF || stkey == SELSEIF)
	    && parlev == 0 && tokno > 3)
            goto first;
        if (stkey == SGRANULARITY || stkey == SNOCLUS
	    || stkey == SCLUS || stkey == SIMPLICIT)
            goto first;
        if (stkey == SASSIGN && tokno == 3 && nextch < lastch
	    && nextch[0] == 't' && nextch[1] == 'o')
        {
            nextch += 2;
            retval = STO;
            break;
        }
        retval = gettok();
        break;

reteos:
    case RETEOS:
        lexstate = NEWSTMT;
        retval = SEOS;
        break;
    default:
        printf("lex: impossible state %d\n", lexstate);
	assert(0); // BVA, 25.03.99
        break;
    }

    static int previous_lineno=-1;
    static int previous_colno=-1;

    if (retval!=STCOMMENT) {
      endcolno = nextch-sbuf;
      if (lineno==previous_lineno)
        startcolno = previous_colno;
      else
        startcolno = 0;
        previous_lineno=lineno;
        previous_colno=endcolno;
    } else {
      previous_lineno=lineno;
      startcolno=0;
      endcolno=0;
    }
#if DEBUG_LEX
    {
	    std::cout<<"on line "<<lineno<<" column "
	  <<startcolno<<" to column "<<endcolno
	  <<": found token "<<retval<<endl;
    }
#endif
    if (retval == SLABEL) {
        if (label_no!=0) {
            char buf[20], buf2[7];
            sprintf(buf, "%d", label_no);
            strcpy(fortrantext_buf, "\n ");
            strcat(fortrantext_buf, buf);
            strcpy(buf2, "     ");
            strncat(fortrantext_buf, buf2, 5-strlen(buf));
        } else
            sprintf(fortrantext_buf, "\n      \0");
        fortrantext = fortrantext_buf; 
    } else {
        lencol  = endcolno - startcolno;
        strncpy(fortrantext_buf, sbuf + startcolno, lencol);
        fortrantext_buf[lencol] = 0; 
        fortrantext = fortrantext_buf; 
    }
    return retval;
}
}


/* Get Cards. */
/* Returns STEOF or STINITIAL, never STCONTINUE.  Any continuation */
/* cards get merged into one long card (hence the size of the buffer */
/* named   sbuf)   */

LOCAL int getcds(void)
{
    register char *p, *q;

top:
    if (nextcd == NULL) {
	nextcd = sbuf;
        code = getcd(nextcd, 1);
        stno = nxtstno;
        prevlin = thislin;
    }
    if (code == STEOF)
        if (popinclude())
            goto top;
        else
            return(STEOF);

    if (code == STCONTINUE) {
        lineno = thislin;
        nextcd = NULL;
        goto top;
    }
    if (code == STCOMMENT)
        return(STCOMMENT);

    /* Get rid of unused space at the head of the buffer */

    if (nextcd > sbuf) {
        q = nextcd;
        p = sbuf;
        while (q < endcd)
            *p++ = *q++;
        endcd = p;
    }

    /* Be aware that the input (i.e. the string at the address nextcd) is */
    /* NOT NULL-terminated. */

    /* This loop merges all continuations into one long statement, AND puts */
    /* the next card to be read at the end of the buffer (i.e. it stores the */
    /* look-ahead card when there's room) */

    ncont = 0;
    do {
        nextcd = endcd;
        if (ncont < CONTMAX)
            linestart[ncont++] = nextcd;
    } while (nextcd + 66 <= send
	     && (code = getcd(nextcd, 0)) == STCONTINUE);

    nextch = sbuf;
    lastch = nextcd - 1;

    /* Handle buffer overflow by zeroing the 'next' pointer (nextcd) so that
   the top of this function will initialize it next time it is called */

    if (nextcd >= send)
        nextcd = NULL;
    lineno  = prevlin;
    prevlin = thislin;
#if DEBUG_LEX
    printf("getcds() returns STINITIAL -- line %d (%.*s)\n",
	   lineno, (nextcd ? nextcd : send) - sbuf, sbuf);
#endif
    return(STINITIAL);
}




/* getcd - Get next input card */
/* This function reads the next input card from global file pointer infile. */
/* It assumes that b points to currently empty storage somewhere in sbuf. */

LOCAL int getcd(char *b, int nocont)
{
    /* BVA, 16.04.98, made p, bend, a[], aend unsigned */
    register int  c;
    register char *p;
    register char *bend;
    int      speclin;  /* Special line - true when the line is allowed
			  to have more than 66 characters (e.g. the
			  "&" shorthand for continuation, use of a "\t"
			  to skip part of the label columns) */
    static char a[6]; /* Statement label buffer */
    static char *const aend = a + sizeof(a);

    char *atend;
    int   amp;


top:
    endcd   = b;
    bend    = b + 66;
    amp     = NO;
    speclin = NO;
    atend   = aend;

    // Read first character.
    c = getc(infile);

    if (c == '&') {
	/* Handle the continuation shorthand of "&" in the first column, */
	/* which stands for "     x" */
        a[0]    = c;
        a[1]    = 0;
        a[5]    = 'x';
        amp     = YES;
	speclin = YES;
        bend    = send;
        p       = aend;
    }
    /* Handle the Comment cards (a 'C', 'c', '*', or '!' in */
    /* the first column). */
    /* Changed '[c & 0xfff]' into '[c & 0xff]', BVA, 10.06.96. */
    else if (comment_start[c & 0xff]) {
	if (feof(infile))
	    return STEOF;
	// BVA: consider C$$ statements as comments too. 24.03.99.
        if ((c = getc(infile)) != '$'
	    || (c = getc(infile), ungetc(c, infile), c == '$'))
	{
            if (!feof(infile) && (c != '\n'))
                *endcd++ = c;
            while (!feof (infile)
		    && c != '\n'
		    && (c = getc(infile)) != '\n')
	    {
                /* Handle obscure end of file conditions on many machines */
                if (feof(infile) && (c == (unsigned char)EOF || c == EOF))
                    break;
                *endcd++ = c;
            }
            if (feof(infile))
                if (c != '\n')
                    ungetc(c, infile);
            ++thislin;
            *endcd++ = 0;
            return STCOMMENT;
        }
	else {
	    /* C$ STATEMENT   */
	    memset(a, BLANK, sizeof(a));
	}
    }
    else if (c != EOF) {
        /* Load buffer a with the statement label */
        /* a tab in columns 1-6 skips to column 7 */
        ungetc(c, infile);
        for (p = a;
	     p < aend && (c = getc(infile)) != '\n' && c != EOF;
	      )
	{
            if (c == '\t') {
                /* The tab character translates into blank characters in */
		/* the statement label */
		atend = p;
                while (p < aend)
                    *p++ = BLANK;
                speclin = YES;
                bend = send;
            }
            else {
                *p++ = c;
	    }
	}
    }

    /* By now we've read either a continuation character or the statement */
    /* label field */

    if (c == EOF)
        return(STEOF);

    /* The next 'if' block handles lines that have fewer than 7 characters */

    if (c == '\n') {
        while (p < aend)
            *p++ = BLANK;

        /* Blank out the buffer on lines which are not longer */
	/* than 66 characters */

        if (!speclin)
            while (endcd < bend)
                *endcd++ = BLANK;
    }
    else {
	/* read body of line */
        while (endcd < bend
	       && (c = getc(infile)) != '\n'
	       && c != EOF)
            *endcd++ = c;

        /* ZFB, 01.12.95, accept non-terminated last line. */
        if (c == EOF) {
	    printf("lex: last line of input is missing a newline !\n");
            c = '\n';
	}

        /* Drop any extra characters on the input card; this usually means
	those after column 72 */

        if (c != '\n') {
            while ((c = getc(infile)) != '\n')
                if (c == EOF)
                    return(STEOF);
        }

        if (!speclin)
            while (endcd < bend)
                *endcd++ = BLANK;
    }

    /* The flow of control usually gets to this line (unless an earlier */
    /* RETURN has been taken) */

    ++thislin;

    /* Fortran 77 specifies that a 0 in column 6 */
    /* does not signify continuation */

    if (!isspace((unsigned char)a[5]) && a[5] != '0') {
        if (!amp)
            for (p = a; p < aend; )
                if (*p++ == '!' && p != aend)
                    goto initcheck;
        if (nocont) {
            lineno = thislin;
            printf("lex: illegal continuation card (starts \"%.6s\")\n", a);
        }
	else if (!amp && strncmp(a, "     ", 5)) {
            lineno = thislin;
            printf("lex: labeled continuation line (starts \"%.6s\")\n", a);
        }
        return STCONTINUE;
    }

initcheck:
    for (p = a; p < atend; ++p) {
        if (!isspace((unsigned char)*p)) {
            if (*p++ != '!')
                goto initline;
            goto process_comment;
        }
    }
    for (p = b; p < endcd; ++p) {
        if (!isspace((unsigned char)*p)) {
            if (*p++ != '!')
                goto initline;
            goto process_comment;
        }
    }

    /* Skip over blank cards by reading the next one right away */

    goto top;
    
process_comment:
    /* BVA, 21.04.98 */
    if (*p != '$') {
	*endcd++ = 0;
	memmove(b, p, endcd-p);
	return STCOMMENT;
    }
    else {
	p++;
	memmove(b, p, endcd-p);
	endcd -= p - b;
	inside_comment_directive = 1;
#if DEBUG_LEX
	/* BVA, 21.04.98 */
	printf("lex: found !$ directive %.*s\n", endcd-b, b);
#endif
    }

initline:

    /* Set nxtstno equal to the integer value of the statement label */

    nxtstno = 0;
    bend = a + 5;
    for (p = a; p < bend; ++p) {
        if (!isspace((unsigned char)*p)) {
            if (isdigit(*p))
                nxtstno = 10 * nxtstno + (*p - '0');
            else if (*p == '!') {
                endcd = b;
                break;
            }
            else {
                lineno = thislin;
                printf("lex: nondigit in statement label field \"%.5s\"\n", a);
                nxtstno = 0;
                break;
            }
	}
    }
    firstline = thislin;
    return STINITIAL;
}

#undef getc


/* crunch -- deletes all space characters, folds the backslash chars and
   Hollerith strings, quotes the Fortran strings */

LOCAL void crunch(void)
{
    /* BVA, 16.04.98: added 'unsigned' */
    register char *i, *j, *j0, *j1, *prvstr;
    int k, ten, nh, nh0, quote;

    /* i is the next input character to be looked at
       j is the next output character */

    new_dcl = needwkey = parlev = parseen = 0;
    expcom = 0;    /* exposed ','s */
    expeql = 0;    /* exposed equal signs */
    j = sbuf;
    prvstr = sbuf;
    k = 0;
    for (i = sbuf ; i <= lastch ; ++i) {
        if (isspace((unsigned char)*i))
            continue;
        if (*i == '!') {
            while (i >= linestart[k])
                if (++k >= CONTMAX)
                    printf("lex: too many continuations\n");
            j0 = linestart[k];
            i = j0 - 1;
            continue;
        }

        /* Keep everything in a quoted string */

        if (*i == '\'' ||  *i == '"') {
            int len = 0;

            quote = *i;
            *j = LEX_QUOTE; /* special marker */
            for ( ; ; ) {
                if (++i > lastch) {
                    printf("lex: unbalanced quotes; closing quote supplied\n");
                    if (j >= lastch)
                        j = lastch - 1;
                    break;
                }
                if (*i == quote) {
                    if (i < lastch && i[1] == quote) 
                        ++i;
                    else 
                        break;
		}
                else if (*i == '\\' && i < lastch) {
                    ++i;
                    *i = escapes[*USC i];
		}
                if (len + 2 < MAXTOKENLEN)
                    *++j = *i;
                else if (len + 2 == MAXTOKENLEN) {
                    printf("lex: string too long, truncating to %d chars\n",
			   MAXTOKENLEN - 2);
		    assert(0);
		}
                len++;
            } /* for (;;) */

            j[1] = LEX_QUOTE;
            j += 2;
            prvstr = j;
        }
	/* test for Hollerith strings */
	else if ((*i == 'h' || *i == 'H') && j > prvstr)
	{
            j0 = j - 1;
            if (!isdigit(*j0)) 
                goto copychar;
            nh = *j0 - '0';
            ten = 10;
            j1 = prvstr;
            if (j1 + 4 < j)
                j1 = j - 4;
            for ( ; ; ) {
                if (j0-- <= j1)
                    goto copychar;
                if (!isdigit(*j0))
                    break;
                nh += ten * (*j0 - '0');
                ten *= 10;
            }
            /* a hollerith must be preceded by a punctuation mark. */
	    /* '*' is possible only as repetition factor in a data statement */
	    /* not, in particular, in character*2h */
#if DEBUG_LEX
	    std::cout << "lex: found sbuf[0] == '" << sbuf[0] << sbuf[1]
	         << "' / sequence " << *j0 << nh << "H --> "
		 << (((*j0 != '*' || !(sbuf[0] == 'd' && sbuf[1] == 'a'))
		      && *j0 != '/' && *j0 != '(' && *j0 != ')' && *j0 != ','
		      && *j0 != '=' && *j0 != '.') ? "copychar" : "Hollerith")
		 << "\n";
#endif
            if ((*j0 != '*' || !(sbuf[0] == 'd' && sbuf[1] == 'a'))
		&& *j0 != '/'
                && *j0 != '('
                && *j0 != ')'
		&& *j0 != ','
		&& *j0 != '='
		&& *j0 != '.')
                goto copychar;
            nh0 = nh;
            if (i + nh > lastch || nh + 2 > MAXTOKENLEN) {
                printf("lex: %dH too big\n", nh);
                nh = lastch - i;
                if (nh > MAXTOKENLEN - 2)
                    nh = MAXTOKENLEN - 2;
                nh0 = -1;
            }
            j0[1] = LEX_QUOTE; /* special marker */
            j = j0 + 1;
            while (nh-- > 0) {
                if (++i > lastch) {
hol_overflow:
                    if (nh0 >= 0)
                        printf("lex: escapes make %dH too big\n", nh0);
                    break;
                }
                if (*i == '\\' )  {
                    if (++i > lastch)
                        goto hol_overflow;
                    *i = escapes[*USC i];
                }
                *++j = *i;
            }
            j[1] = LEX_QUOTE;
            j += 2;
            prvstr = j;
        } else {
            if (*i == '(') 
                parseen = ++parlev;
            else if (*i == ')') 
                --parlev;
            else if (parlev == 0)
                if (*i == '=') 
                    expeql = 1;
                else if (*i == ',') 
                    expcom = 1;
copychar:   
	    /*not a string or space -- copy, shifting case if necessary */
            if (isupper(*i))
                *j++ = tolower(*i);
            else    
                *j++ = *i;
        }
    }
    lastch = j - 1;
    nextch = sbuf;
}


LOCAL void analyz(void)
{
    char const* i;

    if (parlev != 0) {
        printf("lex: unbalanced parentheses, statement skipped\n");
        stkey = SUNKNOWN;
        lastch = sbuf - 1; /* prevent double error msg */
        return;
    }
    if (nextch + 2 <= lastch
	&& nextch[0] == 'i'
	&& nextch[1] == 'f'
	&& nextch[2] == '(')
    {
        /* assignment or if statement -- look at character */
	/* after balancing parentheses. */
        parlev = 1;
        for (i = nextch + 3 ; i <= lastch; ++i)
            if (*i == LEX_QUOTE) {
                while (*++i != LEX_QUOTE)
                    ;
            }
            else if (*i == '(')
                ++parlev;
            else if (*i == ')') {
                if (--parlev == 0)
                    break;
            }
        if (i >= lastch)
            stkey = SLOGIF;
        else if (i[1] == '=')
            stkey = SLET;
        else if (isdigit(i[1]))
            stkey = SARITHIF;
        else    
            stkey = SLOGIF;
        if (stkey != SLET)
            nextch += 2;
    }
    else if (expeql) /* may be an assignment */ {
        if (expcom && nextch < lastch
	    && nextch[0] == 'd' && nextch[1] == 'o')
	{
            stkey = SDO;
            nextch += 2;
            if (nextch[0] == 'a' && nextch[1] == 'l' && nextch[2] == 'l') {
                stkey = SDOALL;
                nextch += 3;
            }
        }
	else    
            stkey = SLET;
    }
    else if (parseen && nextch + 7 < lastch
         && nextch[2] != 'u' /* screen out "double..." early */
         && nextch[0] == 'd' && nextch[1] == 'o'
         && ((nextch[2] >= '0' && nextch[2] <= '9')
         || nextch[2] == ','
         || nextch[2] == 'w')) {
        stkey = SDO;
        nextch += 2;
        needwkey = 1;
        if (nextch[0] == 'a' && nextch[1] == 'l' && nextch[2] == 'l') {
            stkey = SDOALL;
            nextch += 3;
        }
    } /* otherwise search for keyword */
    else     {
        stkey = getkwd();
        if (stkey == SGOTO && lastch >= nextch)
            if (nextch[0] == '(')
                stkey = SCOMPGOTO;
            else if (isalpha_(*USC nextch))
                stkey = SASGOTO;
    }
    parlev = 0;
}



LOCAL int getkwd(void)
{
    char const* i;
    char const* j;
    struct Keylist const* pk;
    struct Keylist const* pend;
    int    k;

    if (!isalpha_(*USC nextch))
        return(SUNKNOWN);
    k = letter(nextch[0]);
    if ((pk = keystart[k])) {
        for (pend = keyend[k] ; pk <= pend ; ++pk) {
            i = pk->keyname;
            j = nextch;
            while (*++i == *++j && *i != 0)
                ;
            if (*i == 0 && j <= lastch + 1) {
                nextch = j;
                return(pk->keyval);
            }
        }
    }
    return(SUNKNOWN);
}


void initkey(void)
{
    struct Keylist const* p;
    char const* s;
    int i, j;

    for (i = 0; i < 26; ++i)
        keystart[i] = NULL;

    for (p = keys; p->keyname; ++p) {
        j = letter(p->keyname[0]);
        if (keystart[j] == NULL)
            keystart[j] = p;
        keyend[j] = p;
    }
    comment_start['c'] = 1;
    comment_start['C'] = 1;
    comment_start['*'] = 1;
    comment_start['!'] = 1;
    s = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
    while ((i = *s++))
        anum_buf[i] = 1;
    s = "0123456789";
    while ((i = *s++))
        anum_buf[i] = 2;
}


LOCAL int hexcheck(int key)
{
    register int    radix;
    register char *p;
    char const*kind;

    switch (key) {
    case 'z':
    case 'Z':
    case 'x':
    case 'X':
        radix = 16;
        key = SHEXCON;
        kind = "hexadecimal";
        break;
    case 'o':
    case 'O':
        radix = 8;
        key = SOCTCON;
        kind = "octal";
        break;
    case 'b':
    case 'B':
        radix = 2;
        key = SBITCON;
        kind = "binary";
        break;
    default:
        printf("lex: bad bit identifier\n");
        return(SNAME);
    }
    for (p = lex_token; *p; p++)
        if (hextoi(*p) >= radix) {
            printf("lex: invalid %s character\n", kind);
            break;
        }
    return key;
}


/* gettok -- moves the right amount of text from   nextch   into the   token
   buffer.   token   initially contains garbage (leftovers from the prev token)
 */

LOCAL int gettok(void)
{
    int    havdot, havexp, havdbl;
    int    radix, val;
    struct Punctlist const* pp;
    struct Dotlist const* pd;
    register int ch;

    char const* i;
    char const* j;
    char const* n1;
    char* p;

    ch = *USC nextch;
    if (ch == LEX_QUOTE) {
        ++nextch;
        p = lex_token;
        while (*nextch != LEX_QUOTE)
            *p++ = *nextch++;
        lex_toklen = p - lex_token;
        *p = 0;
        /* allow octal, binary, hex constants of the form 'abc'x (etc.) */
        if (++nextch <= lastch && isalpha_(val = *USC nextch)) {
            ++nextch;
            return hexcheck(val);
        }
        return (SHOLLERITH);
    }

    if (needkwd) {
        needkwd = 0;
        return(getkwd());
    }

    for (pp = puncts; pp->punchar; ++pp) {
        if (ch == pp->punchar) {
            val = pp->punval;
            if (++nextch <= lastch) {
                switch (ch) {
                case '/':
                    if (*nextch == '/') {
                        nextch++;
                        val = SCONCAT;
                    }
		    else if (new_dcl && parlev == 0)
                        val = SSLASHD;
                    return val;
                case '*':
                    if (*nextch == '*') {
                        nextch++;
                        return SPOWER;
                    }
                    break;
                case '<':
                    if (*nextch == '=') {
                        nextch++;
                        val = SLE;
                    }
                    if (*nextch == '>') {
                        nextch++;
                        val = SNE;
                    }
                    goto extchk;
                case '=':
                    if (*nextch == '=') {
                        nextch++;
                        val = SEQ;
                        goto extchk;
                    }
                    break;
                case '>':
                    if (*nextch == '=') {
                        nextch++;
                        val = SGE;
                    }
extchk:
                    return val;
                }
	    }
            else if (ch == '/' && new_dcl && parlev == 0)
                return SSLASHD;
            switch (val) {
            case SLPAR:
                ++parlev;
                break;
            case SRPAR:
                --parlev;
            }
            return(val);
        }
    }
    if (ch == '.') {
        if (nextch >= lastch) 
            goto badchar;
        else if (isdigit(nextch[1])) 
            goto numconst;
        else {
            for (pd = dots ; (j = pd->dotname); ++pd) {
                for (i = nextch + 1 ; i <= lastch ; ++i)
                    if (*i != *j) 
                        break;
                    else if (*i != '.') 
                        ++j;
                    else     {
                        nextch = i + 1;
                        return(pd->dotval);
                    }
            }
            goto badchar;
        }
    }
    /* Next block-if added by BVA (30.04.97) to process typeless */
    /* constants with a leading type specifier. */
    if (strchr("bBoOxXzZ", ch) && nextch[1] == LEX_QUOTE) {
	int tctype = ch;
	int tokentype;
#if 0
	fprintf(stderr, "Found typeless constant with radix %c.\n", ch);
#endif
	nextch++;
	assert(*nextch == LEX_QUOTE);
	nextch++;
	for (p = lex_token; nextch <= lastch && *nextch != LEX_QUOTE; )
	    *p++ = *nextch++;
	*p = 0;
	if (nextch <= lastch && *nextch == LEX_QUOTE)
	    nextch++;
	tokentype = hexcheck(tctype);
#if 0
	fprintf(stderr, "Typeless constant -> token type %d\n", tokentype);
#endif
	return tokentype;
    }
    if (isalpha_(ch)) {
        p = lex_token;
        *p++ = *nextch++;
        while (nextch <= lastch) {
            if (isalnum_(*USC nextch))
                *p++ = *nextch++;
            else 
                break;
	}
        lex_toklen = p - lex_token;
        *p = 0;
        if (needwkey) {
            needwkey = 0;
            if (lex_toklen == 5
                 && nextch <= lastch && *nextch == '(' /*)*/
                 && !strcmp(lex_token, "while"))
                return(SWHILE);
        }
        if (inioctl && nextch <= lastch && *nextch == '=') {
            ++nextch;
            return(SNAMEEQ);
        }
        if (lex_toklen > 8
	    && strncmp(lex_token, "function", 8) == 0
	    && isalpha_(*USC (lex_token + 8))
	    && nextch < lastch && nextch[0] == '('
	    && (nextch[1] == ')' || isalpha_(*USC (nextch + 1))))
	{
            nextch -= (lex_toklen - 8);
            return(SFUNCTION);
        }

        if (lex_toklen > 50) {
            char    buff[100];
            sprintf(buff, lex_toklen >= 60
                 ? "name %.56s... too long, truncated to %.*s"
                 : "name %s too long, truncated to %.*s",
                lex_token, 50, lex_token);
            printf("lex: error %s\n", buff);
            lex_toklen = 50;
            lex_token[50] = 0;
        }
        if (lex_toklen == 1 && *nextch == LEX_QUOTE) {
            val = lex_token[0];
            ++nextch;
            for (p = lex_token; *nextch != LEX_QUOTE; )
                *p++ = *nextch++;
            ++nextch;
            lex_toklen = p - lex_token;
            *p = 0;
            return hexcheck(val);
        }
        return(SNAME);
    }

    if (isdigit(ch)) {

        /* Check for NAG's special hex constant */

        if (nextch[1] == '#' || (nextch[2] == '#' && isdigit(nextch[1]))) {
            radix = atoi(nextch);
            if (radix != 2 && radix != 8 && radix != 16) {
                printf("lex: invalid base %d [%.5s...] for constant,"
		       " defaulting to hex\n",
		       radix, nextch);
                radix = 16;
            } /* if */
            if (*++nextch != '#')
                nextch++;
            if (++nextch > lastch)
                goto badchar;
            for (p = lex_token; hextoi(*nextch) < radix; ) {
                *p++ = *nextch++;
                if (nextch > lastch)
                    break;
            }
            lex_toklen = p - lex_token;
            *p = 0;
	    fprintf(stderr, "lex: read typeless constant.\n");
            return (radix == 16)
		   ? SHEXCON
		   : ((radix == 8)
		      ?	SOCTCON
		      : SBITCON);
        }
    }
    else
        goto badchar;
numconst:
    havdot = NO;
    havexp = NO;
    havdbl = NO;
    for (n1 = nextch; nextch <= lastch; ++nextch) {
        if (*nextch == '.') {
            if (havdot) 
                break;
            else if (nextch + 2 <= lastch
		     && isalpha_(*USC (nextch + 1))
		     && isalpha_(*USC (nextch + 2)))
                break;
            else
                havdot = YES;
	}
        else if (!intonly && (*nextch == 'd' || *nextch == 'e')) {
            char const* const p = nextch;
            havexp = YES;
            if (*nextch == 'd')
                havdbl = YES;
            if (nextch < lastch)
                if (nextch[1] == '+' || nextch[1] == '-')
                    ++nextch;
            if (!isdigit(*++nextch)) {
                nextch = p;
                havdbl = havexp = NO;
                break;
            }
            for (++nextch;
                nextch <= lastch && isdigit(*USC nextch);
                ++nextch)
                ;
            break;
        }
	else if (!isdigit(*USC nextch))
            break;
    }
    p = lex_token;
    i = n1;
    while (i < nextch)
        *p++ = *i++;
    lex_toklen = p - lex_token;
    *p = 0;
    if (havdbl) 
        return(SDCON);
    if (havdot || havexp) 
        return(SRCON);

    // BVA, 15.04.99: removed ZFB's erroneous attempt to recognize Hollerith
    // strings here.
    return SICON;

badchar:
    sbuf[0] = *nextch++;
    return(SUNKNOWN);
}
