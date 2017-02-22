%left COMMA
%union{char *xml;}
%token <xml> IDENTIFIER 
%token FOR AT LEFT_BRACE RIGHT_BRACE IMPLIES COMMA SEMICOLON COMMENT
%token TRUE FALSE AND OR MAKE HELP HURT BREAK LEFT_PAR RIGHT_PAR
%type <xml> advice advices when soft_goal by side_effects_clause
%type <xml> boolop op topic nfr_type topics side_effects pointcuts
%{
#include <stdio.h>
#include <stdarg.h>
static char *xml = NULL;
#define MAXARGS 31
char *xml_concat(char *xml, ...)
{
	char *xml1 = (char *) malloc(10000);
	int len = 0;
	va_list ap;
	strcpy(xml1, "");
	va_start(ap, xml);
	while (xml!=NULL) {
		if (strlen(xml)>0) {
			len += strlen(xml);
			strcat(xml1, xml);
		}
		// free(xml);
		xml = va_arg(ap, char*);
	} 
	va_end(ap);
	return xml1;
}
char *va_xml_concat(char *xml, va_list ap)
{
	char *xml1 = (char *) malloc(10000);
	int len = 0;
	while (xml!=NULL) {
		if (strlen(xml)>0) {
			len += strlen(xml);
			strcat(xml1, xml);
		}
		// free(xml);
		xml = va_arg(ap, char*);
	} 
	return xml1;
}
char *xml_tag(char *tag, char *arg, ... )
{
#ifdef AST
	va_list ap;
	char *xml1, *xml2;
	int len = 0;
	va_start(ap, arg);
	xml1 = va_xml_concat(arg, ap);
	va_end(ap);
	len += strlen(xml1);
	len += 2*strlen(tag)+8;
	xml2 = (char *) malloc(len);
	strcpy(xml2, "<");
	strcat(xml2, tag);
	strcat(xml2, ">");
	strcat(xml2, xml1);
	strcat(xml2, "</");
	strcat(xml2, tag);
	strcat(xml2, ">\n");
	// free(xml1);
	return xml2;
#else
	return NULL;
#endif
}
char *xml_init(char *str)
{
	return (char *) strdup(str);
}


%}
%%
start: advices { 
#ifdef AST
	printf("%s\n", $1); 
#endif
	free($1); };
advice: when IMPLIES soft_goal by side_effects_clause 
	{ $$ = xml_tag("Advice", $1, $3, $4, $5, NULL); 
	  free($1); free($3); free($4); free($5);
	};
advice: soft_goal by side_effects_clause 
	{ $$ = xml_tag("Advice", $1, $2, $3, NULL); 
	  free($1); free($2); free($3);
	}
by: 	  
	  {$$ = xml_init(""); } 
        | LEFT_BRACE RIGHT_BRACE
	  { $$ = xml_tag("By", NULL); }
        | LEFT_BRACE advices RIGHT_BRACE
	  { $$ = xml_tag("By", $2, NULL); }
side_effects_clause: 
	  {$$ = xml_init(""); } 
	| IMPLIES side_effects 
	  { $$ = xml_tag("SideEffect", $2, NULL); 
   	  }
when: TRUE 
	  {$$ = xml_tag("When", (char*)strdup("True"), NULL); } 
     | FALSE 
	  {$$ = xml_tag("When", (char*)strdup("False"), NULL); } 
     | IDENTIFIER LEFT_PAR RIGHT_PAR
	  {$$ = xml_tag("When", $1, NULL); } 
     | when boolop when
	  {$$ = xml_tag("When", $1, $2, $3, NULL); 
		free($1); free($2); free($3); 
          } 
	;
boolop: AND 
	  {$$ = xml_tag("op", (char*)strdup("and"), NULL); } 
     | OR 
	  {$$ = xml_tag("op", (char*)strdup("or"), NULL); };
soft_goal: nfr_type 
	  {$$ = xml_tag("SoftGoal", $1, NULL); } 
	| nfr_type FOR topics
	  {$$ = xml_tag("SoftGoal", $1, $3, NULL); 
	  };
nfr_type: IDENTIFIER 
	  { $$ = xml_tag("Type", $1, NULL); } ;
topic: IDENTIFIER 
	  {$$ = xml_tag("Topic", $1, NULL); }
	| IDENTIFIER AT pointcuts
	  {$$ = xml_tag("Topic", $1, xml_tag("At", $3, NULL), NULL); 
	  }
	;
topics: topic 
	  {$$ = $1; } 
	  | topic SEMICOLON topics
	  {$$ = xml_concat($1, $3, NULL); } ;
pointcuts: topic 
	  {$$ = $1; } 
	  | topic COMMA pointcuts
	  {$$ = xml_concat($1, $3, NULL); };
advices: advice 
	  {$$ = $1; } 
	  | advice boolop advices 
	  {$$ = xml_concat($1, $2, $3, NULL); };
side_effects: 
	op soft_goal 
  	{$$ = xml_concat($1, $2, NULL); }
	| op soft_goal COMMA side_effects
  	{$$ = xml_concat($1, $2, $4, NULL); };
op:  MAKE { $$ =  xml_tag("op", (char*)strdup("++"), NULL); }
   | HELP { $$ =  xml_tag("op", (char*)strdup("+"), NULL); }
   | HURT { $$ =  xml_tag("op", (char*)strdup("-"), NULL); }
   | BREAK{ $$ =  xml_tag("op", (char*)strdup("--"), NULL); };
%%
int yyerror (char *s) { 
  printf("\n%s\n", s);
}

int debug = 0;

int main(int argc, char **argv) {
	int yyreturn;
	if (argc>1) {
		if (argv[1][0]=='-' && argv[1][1]=='d') {
			debug = 1;
		}
		if (argc>debug + 1)
			stdin = fopen(argv[debug + 1], "r");
	} 
#ifdef AST
	printf("<?xml version=\"1.0\"?>\n");
	printf("<?xml-stylesheet type=\"text/xsl\" href=\"nfr.xsl\"?>\n");
	printf("<!DOCTYPE Advice SYSTEM \"nfr.dtd\">\n");
	printf("<NFR xmlns:yaxx=\"urn:YAcc-Xml-eXtension\">\n");
#endif
	yyreturn = yyparse();
#ifdef AST
	printf("</NFR>\n");
#endif
	return yyreturn;
}
