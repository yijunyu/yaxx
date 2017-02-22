/* File: fortran.y					*/
/* Functionality: Fortran 77 grammar written in Yacc. 	*/

/* BUG: this grammar does not accept comments at the end of a program */
%name-prefix="fortran" /* according to Sebastien Fricker */

%token SEOS 1
%token SCOMMENT 2
%token SLABEL 3
%token SUNKNOWN 4
%token SHOLLERITH 5
%token SICON 6
%token SRCON 7
%token SDCON 8
%token SBITCON 9
%token SOCTCON 10
%token SHEXCON 11
%token STRUE 12
%token SFALSE 13
%token SNAME 14
%token SNAMEEQ 15
%token SFIELD 16
%token SSCALE 17
%token SINCLUDE 18
%token SLET 19
%token SASSIGN 20
%token SAUTOMATIC 21
%token SBACKSPACE 22
%token SBLOCK 23
%token SCALL 24
%token SCHARACTER 25
%token SCLOSE 26
%token SCOMMON 27
%token SCOMPLEX 28
%token SCONTINUE 29
%token SDATA 30
%token SDCOMPLEX 31
%token SDIMENSION 32
%token SDO 33
%token SDOUBLE 34
%token SELSE 35
%token SELSEIF 36
%token SEND 37
%token SENDFILE 38
%token SENDIF 39
%token SENTRY 40
%token SEQUIV 41
%token SEXTERNAL 42
%token SFORMAT 43
%token SFUNCTION 44
%token SGOTO 45
%token SASGOTO 46
%token SCOMPGOTO 47
%token SARITHIF 48
%token SLOGIF 49
%token SIMPLICIT 50
%token SINQUIRE 51
%token SINTEGER 52
%token SINTRINSIC 53
%token SLOGICAL 54
%token SNAMELIST 55
%token SOPEN 56
%token SPARAM 57
%token SPAUSE 58
%token SPRINT 59
%token SPROGRAM 60
%token SPUNCH 61
%token SREAD 62
%token SREAL 63
%token SRETURN 64
%token SREWIND 65
%token SSAVE 66
%token SSTATIC 67
%token SSTOP 68
%token SSUBROUTINE 69
%token STHEN 70
%token STO 71
%token SUNDEFINED 72
%token SWRITE 73
%token SLPAR 74
%token SRPAR 75
%token SEQUALS 76
%token SCOLON 77
%token SCOMMA 78
%token SCURRENCY 79
%token SPLUS 80
%token SMINUS 81
%token SSTAR 82
%token SSLASH 83
%token SPOWER 84
%token SCONCAT 85
%token SAND 86
%token SOR 87
%token SNEQV 88
%token SEQV 89
%token SNOT 90
%token SEQ 91
%token SLT 92
%token SGT 93
%token SLE 94
%token SGE 95
%token SNE 96
%token SENDDO 97
%token SWHILE 98
%token SSLASHD 99
%token SUNROLL 100
%token SDOALL 101
%token SDOSINGLE 102
%token SGRANULARITY 103
%token SNOCLUS      104
%token SCLUS        105
%token SSTATE       106
%token SOPER        107
%token SIFSINGLE    108
%token SREPEAT      109
%token SUNTIL       110
%token SEXIT        111
%token SLOCAL       112
%token SNODEP       113
%token SPERCENT     114
%token SDOISV       115
%token SDOSSV       116
%token SENDDOSSV    117

%{

#include <assert.h>     	/* assert() */
#include <stdio.h>
#include <string.h>
#include "yacc.h"
#include "lex.h"	        /* token */
#define DEBUG_PARSER 0
bool keep_comment_lines;

%}

%union {
    int      ival;  
    char     strtype[60];
    char     chrtype;  
    OP_type  op_type; 
    void   *set_type;
    void   *symtype; 
}

/* Precedence and associativity of operators */

%left     SCOMMA
%nonassoc SCOLON
%right    SEQUALS
%left     SEQV SNEQV
%left     SOR
%left     SAND
%left     SNOT
%nonassoc SLT SGT SLE SGE SEQ SNE
%left     SCONCAT
%left     SPLUS SMINUS SPERCENT /* SPERCENT added by BVA 19.05.96 */
%left     SSTAR SSLASH
%right    SPOWER

%start executable_program
%type <ival>    thislabel
%type <ival>    SLABEL
/* for XML syntax tree */
%type <strtype> SHOLLERITH
%type <strtype> SICON
%type <strtype> SRCON
%type <strtype> SDCON
%type <strtype> SBITCON
%type <strtype> SOCTCON
%type <strtype> SHEXCON
%type <strtype> STRUE
%type <strtype> SFALSE
/*---------------------*/
%type <ival>    end_spec
%type <ival>	let
%type <ival>    entryname
%type <ival>    progname
%type <op_type> addop relop
%type <void>    subprogram executable_program
%type <chrtype> letter
%type <set_type> letgroups letgroup
%type <strtype> SNAME filename
//%type <dd_type> clus state
%type <symtype> other_outlist_elem inelem progarglist assignlabel
%type <symtype> type typespec typename dcl stop
%type <symtype> lengspec nameeq
%type <symtype> in_ioctl in_data in_dcl
%type <symtype> datavar datavarlist funarglist funargs statement
%type <symtype> dospec dospecw
%type <symtype> callarglist arglist args exprlist
%type <symtype> inlist outlist outlist_elem
%type <symtype> out2 substring
%type <symtype> name arg call var
%type <symtype> lhs expr uexpr opt_expr fexpr unpar_fexpr
%type <symtype> ubound simple value callarg
%type <symtype> complex_const simple_const simple_const_int bit_const
%type <symtype> common_statement comhead
%type <symtype> equivlist implicit implist imptype iffable
%type <symtype> subprogram_stmnts
%type <symtype> subroutine_statement program_statement
%type <symtype> external intrinsic equivalence equivset
%type <symtype> data  datalist vallist val savelist saveitem
%type <symtype> paramlist paramitem dims dimlist dim
%type <symtype> labellist label program_fragment opt_dim opt_dims
%type <symtype> namelist namelistentry namelistlist
%type <symtype> spec exec progargs progarg comblock comblocks var_list
%type <symtype> logif goto io io1 iofmove ioctl iofctl read infmt write
%type <symtype> print fmkeyword
%type <symtype> ctlkeyword ctllist ioclause statements end_statement ss_do
%type <symtype> comment terminated_statement
%type <symtype> var_list_comma var_list_with_opt_comma
%type <symtype> star, power

%%

executable_program:
        subprogram
        | executable_program subprogram
        | program_fragment
        {
        }
        ;
subprogram:
        subprogram_stmnts
        {
        }
        ;
subprogram_stmnts:
        statements end_statement
        {
        }
        ;
program_statement:
        SPROGRAM new_proc progname
        {
        }
        | SPROGRAM new_proc progname progarglist
        {
        }
        ;
subroutine_statement:
        SBLOCK new_proc progname
        {
        }
        | SSUBROUTINE new_proc entryname arglist
        {
        }
        | SFUNCTION new_proc entryname arglist
        {
        }
        | type SFUNCTION new_proc entryname arglist
        {
        }
        ;
program_fragment:
        statements
        {
        }
        ;
statements:
        terminated_statement
        | statements terminated_statement
        {
        }
        ;
terminated_statement:
        statement SEOS {
    }
        ;
end_statement:
        thislabel SEND end_spec SEOS
        {
        }
        ;
statement:
        thislabel spec
        {
        }
        | thislabel exec
        {
        }
        | thislabel SINCLUDE filename
        {
        }
        | thislabel SUNKNOWN
        {
        }
        | comment
        {
        }
        | error
        {
        }
        | thislabel program_statement
        {
        }
        | thislabel subroutine_statement
        {
        }
        ;
comment:
        SCOMMENT
        {
        }
        ;
thislabel:
        SLABEL
        {
        }
        ;
new_proc:
        ;
entryname:
        name
        {
        }
        ;
name:
        SNAME
        {
        }
        ;
progname:
        {
        }
        | entryname
        {
        }
        ;
progarglist:
        SLPAR SRPAR
        {
        }
        | SLPAR progargs SRPAR
        {
        }
        ;
progargs:
        progarg
        | progargs SCOMMA progarg
        {
        }
        ;
progarg:
        SNAME
        {
        }
        | SNAME SEQUALS SNAME
        {
        }
        ;
arglist:
        {
        }
        | SLPAR SRPAR
        {
        }
        | SLPAR args SRPAR
        {
        }
        ;
args:
        arg
        {
        }
        | args SCOMMA arg
        {
        }
        ;
arg:
        name
        | SSTAR
        {
        }
        ;
filename:
        SHOLLERITH
        {
        }
        ;
spec: dcl
        | common_statement
        | external
        | intrinsic
        | equivalence
        | data
        | implicit
        | namelist
        | SSAVE
        {
        }
        | SSAVE savelist
        {
        }
        | SFORMAT
        {
        }
        | SOPEN
        {
        }
        | SPARAM in_dcl SLPAR paramlist SRPAR
        {
        }
        ;
dcl: type opt_comma name in_dcl new_dcl dims lengspec
        {
        }
        | dcl SCOMMA name dims lengspec
        {
        }
        | dcl SSLASHD datainit vallist SSLASHD
        {
        }
        ;
new_dcl:
         ;
type:
         typespec lengspec
         {
         }
         ;
typespec:
         typename
         ;
typename:
        SINTEGER {
   }
        | SREAL {
      }
        | SCOMPLEX {
   }
        | SDOUBLE {
    }
        | SDCOMPLEX {
  }
        | SLOGICAL {
   }
        | SCHARACTER {
      }
        | SUNDEFINED {
 }
        | SDIMENSION {
}
        | SAUTOMATIC {
}
        | SSTATIC {
}
        ;
lengspec:
        {
        }
        | SSTAR intonlyon simple_const_int intonlyoff
        {
        }
        | SSTAR intonlyon SLPAR expr SRPAR intonlyoff
        {
        }
        | SSTAR intonlyon SLPAR SSTAR SRPAR intonlyoff
        {
        }
        ;
common_statement:
        SCOMMON in_dcl var_list_comma var
        {
        }
        | SCOMMON in_dcl comblocks
        {
        }
        ;
var_list_comma:
        {
        }
        | var_list_comma var SCOMMA
        {
        }
        ;
var_list_with_opt_comma:
        var_list_comma
        | var_list_comma var
        {
        }
        ;
comblocks:
        comblock
        | comblocks comblock
        {
        }
        ;
comblock:
        comhead var_list_with_opt_comma
        {
        }
        ;
comhead:
        SCONCAT
        {
        }
        | SSLASH name SSLASH
        {
        }
        ;
var_list:
        {
        }
        | var
        {
        }
        | var_list SCOMMA var
        {
        }
        ;
external:
        SEXTERNAL in_dcl name
        {
        }
        | external SCOMMA name
        {
        }
        ;
intrinsic:
        SINTRINSIC in_dcl name
        {
        }
        | intrinsic SCOMMA name
        {
        }
        ;
equivalence:
        SEQUIV in_dcl equivset
        {
        }
        | equivalence SCOMMA equivset
        {
        }
        ;
equivset:
        SLPAR equivlist SRPAR
        {
        }
        ;
equivlist:
        lhs
        {
        }
        | equivlist SCOMMA lhs
        {
        }
        ;
data:
        SDATA in_data datalist
        {
        }
        | data opt_comma datalist
        {
        }
        ;
in_data:
        {
        }
        ;
datalist:
        datainit datavarlist SSLASH datapop vallist SSLASH
        {
        }
        ;
datainit:
        ;
datapop:
        ;
vallist:
        val
        {
        }
        | vallist SCOMMA val
        {
        }
        ;
val: value
        | simple SSTAR value
        {
        }
        ;
value: simple
        | addop simple
        {
        }
        | complex_const
        ;
savelist:
        saveitem
        | savelist SCOMMA saveitem
        {
        }
        ;
saveitem:
        name
        | comhead
        ;
paramlist:
        paramitem
        | paramlist SCOMMA paramitem
        {
        }
        ;
paramitem:
        name SEQUALS expr
        {
        }
        ;
var: name dims
        {
        }
        ;
datavar:
        lhs
        | SLPAR datavarlist SCOMMA dospec SRPAR
        {
        }
        ;
datavarlist:
        datavar
        {
        }
        | datavarlist SCOMMA datavar
        {
        }
        ;
dims:
        {
        }
        | SLPAR dimlist SRPAR
        {
        }
        ;
dimlist:
        dim
        {
        }
        | dimlist SCOMMA dim
        {
        }
        ;
dim:
        ubound
        {
        }
        | expr SCOLON ubound
        {
        }
        ;
ubound:
        SSTAR
        {
        }
        | expr
        ;
labellist:
        label
        {
        }
        | labellist SCOMMA label
        {
        }
        ;
label:
        SICON
        {
        }
        ;
implicit:
        SIMPLICIT in_dcl implist
        {
        }
        | implicit SCOMMA implist
        {
        }
        ;
implist:
        imptype SLPAR letgroups SRPAR
        {
        }
        | imptype
        ;
imptype:
        type
        {
        }
        ;
letgroups:
        letgroup
        {
        }
        | letgroups SCOMMA letgroup
        {
        }
        ;
letgroup:
        letter
        {
        }
        | letter SMINUS letter
        {
        }
        ;
letter:
        SNAME
        {
        }
        ;
namelist:
        SNAMELIST
        {
        }
        | namelist namelistentry
        {
        }
        ;
namelistentry:
        SSLASH name SSLASH namelistlist
        {
        }
        ;
namelistlist:
        name
        {
        }
        | namelistlist SCOMMA name
        {
        }
        ;
in_dcl:
        {
        }
        ;
funarglist:
        {
        }
        | funargs
        {
        }
        ;
funargs:
        expr
        | funargs SCOMMA expr
        {
        }
        ;
expr:
        uexpr
        | SLPAR expr SRPAR
        {
        }
        | complex_const
        ;
uexpr:
        lhs
        {
        }
        | simple_const
        {
        }
        | expr addop expr %prec SPLUS
        {
        }
        | expr SSTAR expr
        {
        }
        | expr SSLASH expr
        {
        }
        | expr SPOWER expr
        {
        }
        | addop expr %prec SSTAR
        {
        }
        | expr relop expr %prec SEQ
        {
        }
        | expr SEQV expr
        {
        }
        | expr SNEQV expr
        {
        }
        | expr SOR expr
        {
        }
        | expr SAND expr
        {
        }
        | SNOT expr
        {
        }
        | expr SCONCAT expr
        {
        }
        ;
addop:
        SPLUS {
}
        | SMINUS {
}
        | SPERCENT {
}
        ;
relop:
        SEQ {
}
        | SGT {
}
        | SLT {
}
        | SGE {
}
        | SLE {
}
        | SNE {
}
        ;
lhs:
        name
        | name substring
        {
        }
        | name SLPAR funarglist SRPAR
        {
        }
        | name SLPAR funarglist SRPAR substring
        {
        }
        ;
substring:
        SLPAR opt_dims SRPAR
        {
        }
        ;
opt_dim:
        opt_expr SCOLON opt_expr
        {
        }
        ;
opt_dims:
        opt_dim
        {
        }
        | opt_dims SCOMMA opt_dim
        {
        }
        ;
opt_expr:
        {
        }
        | expr
        {
        }
        ;
simple:
        name
        {
        }
        | simple_const
        ;
simple_const:
        STRUE
        {
        }
        | SFALSE
        {
        }
        | SHOLLERITH
        {
        }
        | simple_const_int
        | SRCON
        {
        }
        | SDCON
        {
        }
        | bit_const
        {
        }
        ;
simple_const_int:
        SICON
        {
        }
        ;
complex_const:
        SLPAR uexpr SCOMMA uexpr SRPAR
        {
        }
        ;
bit_const:
        SHEXCON
        {
        }
        | SOCTCON
        {
        }
        | SBITCON
        {
        }
        ;
fexpr:
        unpar_fexpr
        | SLPAR fexpr SRPAR
        {
        }
        ;
unpar_fexpr:
        lhs
        | fexpr SEQUALS fexpr
        {
        }
        | simple_const
        | fexpr addop fexpr %prec SPLUS
        {
        }
        | fexpr SSTAR fexpr
        {
        }
        | fexpr SSLASH fexpr
        {
        }
        | fexpr SPOWER fexpr
        {
        }
        | addop fexpr %prec SSTAR
        {
        }
        | fexpr SCONCAT fexpr
        {
        }
        ;
ss_do:
        SDO end_spec opt_comma dospecw SEOS
        {
        }
        | SDOALL end_spec opt_comma dospecw SEOS
        {
        }
        | SDO SWHILE SLPAR expr SRPAR SEOS
        {
        }
        | SREPEAT SEOS
        {
        }
        ;
exec:
        iffable
        | SDO end_spec intonlyon label intonlyoff opt_comma dospecw
        {
        }
        | SDOALL end_spec intonlyon label intonlyoff opt_comma dospecw
        {
        }
        | ss_do statements thislabel SENDDO
        {
        }
        | ss_do statements thislabel SUNTIL SLPAR expr SRPAR
        {
        }
        | logif iffable
        {
        }
        | logif STHEN
        {
        }
        | SELSEIF end_spec SLPAR expr SRPAR STHEN
        {
        }
        | SELSE end_spec
        {
        }
        | SENDIF end_spec
        {
        }
        | SUNROLL end_spec expr
        {
        }
        | SNODEP var_list
        {
        }
        | SDOALL
        {
        }
        | SDOISV
        {
        }
        | SDOSINGLE
        {
        }
        | SLOCAL var_list
        {
        }
        | SIFSINGLE
        {
        }
        | SENTRY entryname arglist
        {
        }
        ;
logif:
        SLOGIF end_spec SLPAR expr SRPAR
        {
        }
        ;
dospec:
        name SEQUALS exprlist
        {
        }
        ;
dospecw:
        dospec
        ;
iffable:
        let lhs SEQUALS expr
        {
        }
        | SASSIGN end_spec assignlabel STO name
        {
        }
        | SCONTINUE end_spec
        {
        }
        | goto
        | io
        | SARITHIF end_spec SLPAR expr SRPAR label SCOMMA label SCOMMA label
        {
        }
        | call
        {
        }
        | call SLPAR SRPAR
        {
        }
        | call SLPAR callarglist SRPAR
        {
        }
        | SRETURN end_spec opt_expr
        {
        }
        | stop end_spec opt_expr
        {
        }
        ;
assignlabel:
        SICON
        {
        }
        ;
let:
        SLET
        {
        }
        ;
goto:
        SGOTO end_spec label
        {
        }
        | SASGOTO end_spec name
        {
        }
        | SASGOTO end_spec name opt_comma SLPAR labellist SRPAR
        {
        }
        | SCOMPGOTO end_spec SLPAR labellist SRPAR opt_comma expr
        {
        }
        ;
opt_comma:
        | SCOMMA
        ;
call:
        SCALL end_spec name
        {
        }
        ;
callarglist:
        callarg
        | callarglist SCOMMA callarg
        {
        }
        ;
callarg:
        expr
        | SSTAR label
        {
        }
        ;
stop:
        SPAUSE {
}
        | SSTOP {
}
        | SEXIT {
}
        ;
exprlist:
        expr
        | exprlist SCOMMA expr
        {
        }
        ;
end_spec:
        {
        }
        ;
intonlyon:
        ;
intonlyoff:
        ;
io:
        io1
        ;
io1:
        iofmove ioctl
        {
        }
        | iofmove unpar_fexpr
        {
        }
        | iofmove SSTAR
        {
        }
        | iofmove SPOWER
        {
        }
        | iofctl ioctl
        {
        }
        | read ioctl
        {
        }
        | read infmt
        {
        }
        | read ioctl inlist
        {
        }
        | read ioctl SCOMMA inlist
        {
        }
        | write ioctl
        {
        }
        | write ioctl outlist
        {
        }
        | print
        {
        }
        | print SCOMMA outlist
        {
        }
        ;
iofmove:
        fmkeyword end_spec in_ioctl
        {
        }
        ;
fmkeyword:
        SBACKSPACE {
 }
        | SREWIND {
}
        | SENDFILE {
}
        ;
iofctl:
        ctlkeyword end_spec in_ioctl
        {
        }
        ;
ctlkeyword:
        SINQUIRE
        {
        }
        | SCLOSE
        {
        }
        ;
infmt:
        unpar_fexpr
        {
        }
        | star
        ;
ioctl:
        SLPAR ctllist SRPAR
        {
        }
        ;
ctllist:
        ioclause
        {
        }
        | ctllist SCOMMA ioclause
        {
        }
        ;
ioclause:
        fexpr
        {
        }
        | star
        | power
        | nameeq expr
        {
        }
        | nameeq star
        {
        }
        | nameeq power
        {
        }
        ;
nameeq:
        SNAMEEQ
        {
        }
        ;
read:
        SREAD end_spec in_ioctl
        {
        }
        ;
write:
        SWRITE end_spec in_ioctl
        {
        }
        ;
print:
        SPRINT end_spec fexpr in_ioctl
        {
        }
        | SPRINT end_spec star in_ioctl
        {
        }
        ;
inlist:
        inelem
        {
        }
        | inlist SCOMMA inelem
        {
        }
        ;
inelem:
        lhs
        {
        }
        | SLPAR inlist SCOMMA dospec SRPAR
        {
        }
        ;
outlist_elem:
        uexpr
        | other_outlist_elem
        ;
outlist:
        outlist_elem
        | out2
        ;
out2:
        uexpr SCOMMA uexpr
        {
}
        | uexpr SCOMMA other_outlist_elem
        {
}
        | other_outlist_elem SCOMMA uexpr
        {
}
        | other_outlist_elem SCOMMA other_outlist_elem
        {
}
        | out2 SCOMMA uexpr
        {
}
        | out2 SCOMMA other_outlist_elem
        {
}
        ;
other_outlist_elem:
        complex_const
        | SLPAR expr SRPAR
        {
        }
        | SLPAR uexpr SCOMMA dospec SRPAR
        {
        }
        | SLPAR other_outlist_elem SCOMMA dospec SRPAR
        {
        }
        | SLPAR out2 SCOMMA dospec SRPAR
        {
        }
        ;
in_ioctl:
        {
        }
        ;
star:
        SSTAR
        {
        }
        ;
power:
        SPOWER
        {
        }
        ;

%%

// #include <iostream.h>
#include <iostream>
void yyerror(char* s)
{
    std::cout << "yacc: " << s << " in '";
}

char *get_token_value(const char *type, YYSTYPE yylval)
{
    if (type==NULL) return "";
    if (strcmp(type, "")==0) return "";
    if (strcmp(type, "ival")==0) {
        char *buf = new char[100];
        sprintf(buf, "%d", yylval.ival);
        return buf;
    } else if (strcmp(type, "strtype")==0) {
        return yylval.strtype;
    } else if (strcmp(type, "chrtype")==0) {
        char *str = new char[2];
        str[0] = yylval.chrtype;
        str[1] = 0;
        return str;
    } 
    return "";
}
