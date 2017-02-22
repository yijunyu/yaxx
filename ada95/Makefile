PREFIX=ada
YACC = ${HOME}/software/bin/bison
YFLAGS  = -dv -S yaxx-0.12-bison-2.1.c
LEX  = flex
#YFLAGS  = -dv 
XFLAGS =
CFLAGS  = -g ${XFLAGS} -O
LFLAGS  =
BIN = .
CC = gcc

SRC = grammar9x.y lexer9x.l fft_single.adb
OBJ = grammar9x.tab.o lexer9x.yy.o 

all: fft_single.xml

%.xml:%.adb $(BIN)/ada95
	$(BIN)/ada95 < $*.adb
	mv ${PREFIX}yaxx.xml fft_single.xml

$(BIN)/ada95 : $(OBJ)
	${CC} $(CFLAGS) $(OBJ) -o ada95

%.tab.c: %.y
	${YACC} ${YFLAGS} $< 

lexer9x.yy.c: lexer9x.l
	${LEX} $< 
	mv lex.yy.c lexer9x.yy.c

lexer9x.yy.o: lexer9x.yy.c
	${CC} -c ${CFLAGS} $< -o $@

lexer9x.yy.o: grammar9x.tab.h
grammar9x.tab.h: grammar9x.tab.c

clean   :
	rm -f grammar9x.tab.h grammar9x.tab.c grammar9x.output lexer9x.yy.c *.o

realclean:
	${MAKE} clean
	rm -f ada95* *.xml ${PREFIX}yaxx.dtd
