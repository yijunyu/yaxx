PREFIX=fortran
YACC = bison2.1
LEX  = flex
YFLAGS  = -dv -S yaxx-0.12-bison-2.1.c
CFLAGS  = -I. -g -O
LFLAGS  =
BIN = .
CC = g++

SRC = fortran.y main.c lex.c
OBJ = fortran.tab.o main.o lex.o

all: ex.xml

%.xml:%.f $(BIN)/fortran77
	$(BIN)/fortran77 $*.f
	mv ${PREFIX}yaxx.xml $*.xml
	
$(BIN)/fortran77 : $(OBJ)
	${CC} $(CFLAGS) $(OBJ) -o fortran77

%.tab.c: %.y
	${YACC} ${YFLAGS} $< 

fortran.tab.h: fortran.tab.c

clean:
	rm -f fortran.tab.h fortran.tab.c fortran.output *.o

realclean: clean
	${MAKE} clean
	rm -f fortran77* *.xml yaxx.dtd
