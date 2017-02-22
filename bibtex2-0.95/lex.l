%{
int lineno = 1 ;
%}
%%
\\[\"'`\^~=\.uvHtcdb]\{\\.\}	{ yylval = stryychar(yytext[4]);return STRING ;}
\\[\"'`\^~=\.uvHtcdb]\{[^\}]\}	{ yylval = stryychar(yytext[3]);return STRING ;}
\\[\"'`\^~=\.uvHtcdb][^\{]	{ yylval = stryychar(yytext[2]);return STRING ;}
\\\"				{ yylval = stryytext(); return STRING ; }
\\				{ yylval = stryytext(); return STRING ; }
~				{ yylval = stryychar(' ') ; return STRING ; }
@[^\\\"	 \n\(\),=\{\}#~]+	{
				  if( strcasecmp( yytext, "@string" ) == 0) {
				    return DEFINESTRING ;
				  } else {
				    yylval = stryytext();
				    return ATSTRING ;
				  }
				}
[^\\\" 	\n\(\),=\{\}#~]+	{ yylval = stryytext();return STRING ; }
[ 	]*			{ yylval = stryytext();return WHITE; }
\n				{ yylval = stryytext();lineno++ ;return WHITE; }
\"				{ yylval = stryytext();return QUOTE ; }
\(				{ yylval = stryytext();return LBRACKET ; }
\)				{ yylval = stryytext();return RBRACKET ; }
\{				{ yylval = stryytext();return LCBRACKET ; }
\}				{ yylval = stryytext();return RCBRACKET ; }
=				{ yylval = stryytext();return EQUAL ; }
#				{ yylval = stryytext();return HASHSIGN ; }
\,				{ yylval = stryytext();return COMMA ; }
%%
