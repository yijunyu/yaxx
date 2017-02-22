%token SPACES
%token SXML
%token STAG
%token ETAG
%token CTAG
%token XTAG
%token ZTAG
%token TEXT
%token IDNT
%token STRG

%{
#include "xmlinc.h"
extern int yydebug;
extern char *yytext;
extern void xmlevent(int event, char * text);

void signalevent(int event)
{
   xmlevent(event,yytext);
}

%}

%start module

%%
module  : opspacs sxml opspacs element opspacs
    ;
opspacs :
    | opspacs SPACES
    ;
sxml    : SXML    { signalevent(EVXMLHDR); }
        ;
stag    : STAG    { signalevent(EVSTOTAG); }
        ;
etag    : ETAG    { signalevent(EVESDTAG); }
        ;
ctag    : CTAG    { signalevent(EVCSTTAG); }
        ;
xtag    : XTAG    { signalevent(EVENDTAG); }
        ;
ztag    : ZTAG    { signalevent(EVGCLTAG); }
        ;
idnt    : IDNT    { signalevent(EVOPTIDT); }
        ;
strg    : STRG    { signalevent(EVOPTSTR); }
        ;
blocks  :
    | block blocks
    ;
block   : SPACES  { signalevent(EVBLSPAC); }
    | TEXT    { signalevent(EVBLTEXT); }
    | element
    ;
element : stag options etag blocks ctag etag
    | stag options xtag 
    | stag options etag blocks ztag
        | error etag   { signalevent(EVSERROR); }
    ;
options :
    | options option
    ;
option  : idnt '=' strg
    ;
