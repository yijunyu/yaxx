const char rcsid[] = "$Id: main.c,v 1.6 2004/06/08 03:16:38 yijunsf Exp $";
#include <assert.h>
#include <string.h>
#include "lex.h"	/* initkey() */
#include "yacc.h"	/* yyparse() */
// #include <iostream.h>
#include <iostream>

int main(int argc, char** argv)
{ 
    char infstr[50];
    if (argc<2)  {
	    std::cout << "Input file name = ";
	    std::cin >> infstr;
    } else {
        strcpy(infstr, argv[1]);
    }
    initkey();
    inilex(infstr);
	fortranparse();
    return 0;
}
