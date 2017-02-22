%{
#include "xmlinc.h"
#include "xml.tab.h"
%}

%x tag
%x com

%%

"<?xml"[^\n>]*"?>"      { return (SXML); }
"<"[a-zA-Z]+            { BEGIN tag; return (STAG); }
"</"[a-zA-Z]+           { BEGIN tag; return (CTAG); }
"</>"               { return (ZTAG); }
"<!--"              { BEGIN com; cmt=1; }

<tag>">"            { BEGIN 0; return (ETAG); }
<tag>"/>"           { BEGIN 0; return (XTAG); }
<tag>[ \t]*         { }
<tag>"\n"           { linecount++; }
<tag>[a-zA-Z]+          { return (IDNT); }
<tag>\"[^\"\n]*\"       { return (STRG); }
<tag>.              { return (yytext[0]); }

<com>"-->"          { cmt--; if (cmt <= 0) BEGIN 0; }
<com>"<!--"         { cmt++; }
<com>"\n"           { linecount++; }
<com>.              { }

[ \t]+              { return (SPACES); }
"\n"                { linecount++; return (SPACES); }
[^< \t\n]+          { return (TEXT);}
.               { return (TEXT);}
%%
void yyerror(char * s)
{
   fprintf(stderr, s);
}
