%{
#include <stdio.h>
#include "nfr.tab.h"
extern int debug;
%}
%%
"/*"  { comment(); 
      }
"for" { return FOR; }
"at" { return AT; }
"{" { return LEFT_BRACE; }
"}" { return RIGHT_BRACE; }
"(" { return LEFT_PAR; }
")" { return RIGHT_PAR; }
"=>" { return IMPLIES; }
"++" { return MAKE; }
"+" { return HELP; }
"-" { return HURT; }
"--" { return BREAK; }
"and" { return AND; }
"or" { return OR; }
"," { return COMMA; }
";" { return SEMICOLON; }
"true" { return TRUE; }
"false" { return FALSE; }
[a-zA-Z_][0-9a-zA-Z_-]* { 
	if (debug)
		printf("Identifier: %s\n", (char *) strdup(yytext));
	yylval.xml = (char *) strdup(yytext);
	return IDENTIFIER; 
}
\"[^\"]*\" { 
	if (debug)
		printf("Identifier: %s\n", (char *) strdup(yytext));
	yylval.xml = (char *) strndup(yytext+1, strlen(yytext)-2);
	return IDENTIFIER; 
}
[ \t\v\n\f]     { }
.           { }
%%
int yywrap() { return(1); }

comment()
{
    char c, c1;
#ifdef AST
    printf("<COMMENT>");
#endif
loop:
    while ((c = input()) != '*' && c != 0)
#ifdef AST
        putchar(c)
#endif
	;
    if ((c1 = input()) != '/' && c != 0)
    {
        unput(c1);
        goto loop;
    }
#ifdef AST
    printf("</COMMENT>\n");
#endif
}
