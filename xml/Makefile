PREFIX=yy
YACC = ${HOME}/software/bin/bison
LEX  = flex
YFLAGS  = -dv -S yaxx-0.11-bison-2.1.c 
CFLAGS  = -g -O
LFLAGS  =
BIN = .
CC = gcc

SRC = xml.y xml.l main.c
OBJ = xml.tab.o lex.yy.o main.o

all: test-yaxx.xml

%-yaxx.xml:%.xml $(BIN)/xmlc
	$(BIN)/xmlc < $*.xml
	mv yaxx.xml $*-yaxx.xml

$(BIN)/xmlc : $(OBJ)
	${CC} $(CFLAGS) $(OBJ) -lfl -o $@

%.tab.c: %.y
	export BISON_SIMPLE=../bison/src/bison.simple.yaxx
	${YACC} ${YFLAGS} $< 

lex.yy.c: xml.l
	${LEX} $< 

lex.yy.o: lex.yy.c
	${CC} -c ${CFLAGS} $< -o $@

lex.yy.o: xml.tab.h
xml.tab.h: xml.tab.c

clean   :
	rm -f xml.tab.h xml.tab.c xml.output lex.yy.c *.o

realclean:
	${MAKE} clean
	rm -f $(BIN)/xmlc* *yaxx.xml yaxx.dtd
