/* BVA, 26.09.97: prototypes for functions defined by yacc. */



#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
extern int yylineno;
extern int yyleng;

int  fortranlex(void);
int  fortranparse(void);
void yyerror(char*);
void yymark (void);
void yywhere(void);
int  yywrap (void);
void parser_init(void);

#ifdef __cplusplus
}
#endif
