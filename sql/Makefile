PREFIX=yy
YACC = bison$(X)
LEX  = flex
YFLAGS  = -dv -S yaxx-${Y}-bison-$(X).c 
CFLAGS  = -g -O
LFLAGS  =
BIN = .
CC = gcc

SRC = sql1.y sql2.y scn1.l scn2.l sqltext.c
OBJ1 = sql1.tab.o scn1.o
OBJ2 = sql2.tab.o scn2.o sqltext.o 

all: sql1.xml sql2.xml

%.xml:test.% $(BIN)/%
	$(BIN)/$* < test.$*
	mv yaxx.xml $*.xml

$(BIN)/sql1 : $(OBJ1)
	${CC} $(CFLAGS) $(OBJ1) -o $(BIN)/sql1 -lfl

$(BIN)/sql2 : $(OBJ2)
	${CC} $(CFLAGS) $(OBJ2) -o $(BIN)/sql2 -lfl

%.tab.c: %.y
	${YACC} ${YFLAGS} $< 
	mv $*.tab.h $*.h

scn1.c: scn1.l
	${LEX} $< 
	mv lex.yy.c scn1.c

scn1.o: scn1.c
	${CC} -c ${CFLAGS} $< -o $@

scn2.c: scn2.l
	${LEX} $< 
	mv lex.yy.c scn2.c

scn2.o: scn2.c
	${CC} -c ${CFLAGS} $< -o $@

scn1.o: sql1.h
sql1.tab.h: sql1.tab.c
scn2.o: sql2.h
sql2.tab.h: sql2.tab.c

clean:
	rm -f sql*.tab.h sql*.tab.c sql*.output scn*.c *.o

realclean:
	${MAKE} clean
	rm -f ${BIN}/sql1.exe ${BIN}/sql2.exe *.xml yaxx.dtd
