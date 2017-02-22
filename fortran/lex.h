#ifndef LEX_H
#define LEX_H
#include <stdio.h> /* FILE */

#define LEX_QUOTE 2
#define LOCAL static
#define Table_size 256
#define NO 0
#define YES 1
#define P1_STMTBUFSIZE 1400
typedef FILE *FILEP;
typedef int  flag;
typedef char field;    /* actually need only 4 bits */
typedef int  ftnint;    /* was "long int" before */

typedef enum {
    O_UNDEFINED, O_CON, O_SEQU, O_NEQV,
    O_EQV,	 O_OR,  O_AND,  O_NOT,
    O_GE,	 O_GT,  O_NE,   O_EQ,
    O_LE,	 O_LT,  O_ADD,  O_MIN,
    O_MUT,	 O_DIV, O_EE,   O_PERCENT
} OP_type;


#ifdef __cplusplus
extern "C" {
#endif
    extern char sbuf[13900];
    extern char lex_token[];
    extern char* lastch;
    extern int  lex_toklen;
    extern int  label_no;
#ifdef __cplusplus
}
#endif


/* -------------------------------------------------------- */
/* File: lex.h                                              */
/* Date: Mon Apr 28 11:17:16 DFT 1997                       */
/* This file has been generated automatically by genproto.  */
/* Do NOT edit below the separator.                         */

#ifdef __cplusplus
#undef DEFARG
#define DEFARG(e) =e
extern "C" {
#else
#define DEFARG(e)
#endif

int inilex(const char *const name);
int quitlex(void);
void flline(void);
char *lexline(int *n);
void doinclude(const char *const name);
int yylex(void);
void initkey(void);
void unclassifiable(void);

extern int   lineno;
extern int   startcolno; // KB 29.06.2001 the start column of the current token
                         // white space is not counted
extern int   endcolno;   // KB 29.06.2001 the end columnd number of the current 

#ifdef __cplusplus
}
#endif

#undef DEFARG
#define DEFARG(e)
#endif
