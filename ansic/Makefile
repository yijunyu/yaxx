PREFIX=yy
VER=2.2
YACC = bison$(VER)
LEX  = flex
YFLAGS  = -dv -S yaxx-0.13-bison-$(VER).c
XFLAGS =
CFLAGS  = -g ${XFLAGS} -O
LFLAGS  =
BIN = .
CC = gcc

SRC = gram.y scan.l main.c
OBJ = gram.tab.o lex.yy.o main.o 

all: main.xml

%.xml:%.c $(BIN)/ansic
	$(BIN)/ansic < $*.c
	mv yaxx.xml $*.xml

$(BIN)/ansic : $(OBJ)
	${CC} $(CFLAGS) $(OBJ) -o ansic

%.tab.c: %.y
	${YACC} ${YFLAGS} $< 

lex.yy.c: scan.l
	${LEX} $< 

lex.yy.o: lex.yy.c
	${CC} -c ${CFLAGS} $< -o $@

lex.yy.o: gram.tab.h
gram.tab.h: gram.tab.c

clean   :
	rm -f gram.tab.h gram.tab.c gram.output lex.yy.c *.o

realclean:
	${MAKE} clean
	rm -f ansic* ${PREFIX}xx.xml ${PREFIX}xx.dtd
