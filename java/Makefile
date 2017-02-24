PREFIX=java
VER=2.3
YACC = bison$(VER)
LEX  = flex
YFLAGS  = -dv -S yaxx-0.13-bison-$(VER).c
CFLAGS  = -g -O
LFLAGS  =
BIN = .
CC = gcc

SRC = java11.y java.l main.c
OBJ = java11.tab.o lex.yy.o main.o

all: HelloWorld.xml

%.xml:%.java $(BIN)/javac
	$(BIN)/javac < $*.java
	mv ${PREFIX}yaxx.xml $*.xml
	
$(BIN)/javac : $(OBJ)
	${CC} $(CFLAGS) $(OBJ) -lfl -o $@

%.tab.c: %.y
	${YACC} ${YFLAGS} $< 

lex.yy.c: java.l
	${LEX} $< 

lex.yy.o: lex.yy.c
	${CC} -c ${CFLAGS} $< -o $@

lex.yy.o: java11.tab.h
java11.tab.h: java11.tab.c

clean   :
	rm -f java11.tab.h java11.tab.c java11.output lex.yy.c *.o

realclean:
	${MAKE} clean
	rm -f $(BIN)/javac* *.xml ${PREFIX}yaxx.dtd
