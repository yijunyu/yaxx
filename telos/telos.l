%{
#include <stdio.h>
#include "header.h"
#include "telos.tab.h"
#define ECHO
%}

%option prefix="telos" 
%option outfile="lex.yy.c"
%option noyywrap
D           [0-9]
L           [a-zA-Z_\$\.0-9]
E           [Ee][+-]?{D}+

%%
IN	{return IN;}
ISA	{return ISA;}
WITH	{return WITH;}
END	{return END;}
Token 	{ return TOKEN;}
SimpleClass {return SIMPLECLASS;}
MetaClass     {return METACLASS;}
MetaMetaClass {return METAMETACLASS;}
[-+]?{D}+	{ return INTEGER; }
[-+]?{D}+{E}	{ return REAL; }
[-+]?{D}*"."{D}+({E})?	{ return REAL; }
[-+]?{D}+"."{D}*({E})?	{ return REAL; }
[a-zA-z]{L}*	{ return LABEL; }
["].*["]	{ return LABEL; }
,	{return ',';}
:	{return ':';}
\.	{ return '.';}
'$'	{return '$';}
-	{return '-';}
[ \t\v\f]*	{}
.	{}

%%
