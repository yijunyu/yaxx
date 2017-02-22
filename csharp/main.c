/***
 *** C# parser/scanner
 *** Copyright 2002 James Power, NUI Maynooth, Ireland <james.power@may.ie>
 *** This version: 19 Feb 2002
 ***
 *** This program is free software; you can redistribute it and/or modify
 *** it under the terms of the GNU General Public License as published by
 *** the Free Software Foundation; either version 2 of the License, or
 *** (at your option) any later version.
 ***
 *** This program is distributed in the hope that it will be useful,
 *** but WITHOUT ANY WARRANTY; without even the implied warranty of
 *** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *** GNU General Public License for more details.
 ***
 *** You should have received a copy of the GNU General Public License
 *** along with this program; if not, write to the Free Software
 *** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ***/

#include<stdio.h>

extern FILE *yyin, *yyout;
extern int yyparse();

int main(int argc, char *argv[])
{
  yyin = stdin;
  yyout = stdout;
 
  /* Open the input & output files */
  if (argc >= 2)  {
    yyin = fopen(argv[1],"r");
    if (yyin == NULL)   {
      fprintf(stderr,"ERROR - cannot open input file\n");
      return 1;
    }
    if (argc >= 3)  {
      yyout = fopen(argv[2],"w");
    }
    if (yyout == NULL)  {
      fprintf(stderr,"ERROR - cannot open output file\n");
      return 1;
    }
  }

  return yyparse();

}


