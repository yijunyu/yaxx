YACC = bison$(X)
LEX  = flex
YFLAGS  = -dv -S yaxx-${Y}-bison-$(X).c
CFLAGS  = -g -O -DENTITYREPLACE
LFLAGS  =
#BIN = ../../bin
BIN = .
#NFR = ../../nfr-0.1
NFR = .
CC = g++
#OME = ../../ome-3.10/projects/nfr
OME = .

SRC = telos.y telos.l main.c
OBJ = telos.tab.o lex.yy.o main.o 

all: ${NFR}/optimization.dat ${NFR}/complexity.dat 

${NFR}/%.dat: ${OME}/%.tel $(BIN)/telosc
	$(BIN)/telosc <  $< > $@
	cat $@

%.xml:  ${OME}/%.tel $(BIN)/telosc
	$(BIN)/telosc < $<
	mv yaxx.xml $@

$(BIN)/telosc : $(OBJ)
	${CC} $(CFLAGS) $(OBJ) -o $@

%.tab.c: %.y
	${YACC} ${YFLAGS} $< 

lex.yy.c: telos.l
	${LEX} $< 

lex.yy.o: lex.yy.c
	${CC} -c ${CFLAGS} $< -o $@

lex.yy.o: telos.tab.h
telos.tab.h: telos.tab.c

clean   :
	rm -f telos.tab.h telos.tab.c telos.output lex.yy.c *.o

realclean:
	${MAKE} clean
	rm -f telosc* yaxx.dtd optimization.xml telosyaxx.dtd telosyaxx.xml
