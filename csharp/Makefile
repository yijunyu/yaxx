 ###
 ### C# parser/scanner
 ### Copyright 2002 James Power, NUI Maynooth, Ireland <james.power@may.ie>
 ### This version: 19 Feb 2002
 ###
 ### This program is free software; you can redistribute it and/or modify
 ### it under the terms of the GNU General Public License as published by
 ### the Free Software Foundation; either version 2 of the License, or
 ### (at your option) any later version.
 ###
 ### This program is distributed in the hope that it will be useful,
 ### but WITHOUT ANY WARRANTY; without even the implied warranty of
 ### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ### GNU General Public License for more details.
 ###
 ### You should have received a copy of the GNU General Public License
 ### along with this program; if not, write to the Free Software
 ### Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  0$(X)1-1307  USA
 ###

YACC = bison$(X)
YFLAGS  = -dv -S yaxx-${Y}-bison-$(X).c

csharp: lex.yy.o csharp.tab.o main.o
	gcc -o csharp $^

csharp.tab.h: csharp.y
	${YACC} ${YFLAGS} csharp.y

csharp.tab.c: csharp.y
	${YACC} ${YFLAGS} csharp.y

lex.yy.c: csharp-lex.l csharp.tab.h
	flex  csharp-lex.l


clean:
	rm -f *.o *~
	rm -f lex.yy.c csharp.tab.c csharp.tab.h csharp.output
	rm -f csharp
	rm -f yaxx.xml yaxx.dtd
