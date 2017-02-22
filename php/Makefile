PREFIX=yy
YACC = bison2.1
LEX  = flex
YFLAGS  = -dv -S yaxx-0.12-bison-2.1.c
XFLAGS = -DYY_ACTION_ONLY
CFLAGS  = -g ${XFLAGS} -O
LFLAGS  =
BIN = .
CC = gcc

SRC = php_parser.y php_lexer.l 
OBJ = php_parser.tab.o lex.yy.o

all: order.xml

%.xml:%.php $(BIN)/php
	$(BIN)/php < $*.php
	mv yaxx.xml $*.xml

$(BIN)/php : $(OBJ)
	${CC} $(CFLAGS) $(OBJ) -o $@

%.tab.c: %.y
	${YACC} ${YFLAGS} $< 

%.yy.c: php_lexer.l
	${LEX} $< 

lex.yy.o: lex.yy.c
	${CC} -c ${CFLAGS} $< -o $@

lex.yy.o: php_parser.tab.h
php_parser.tab.h: php_parser.tab.c

clean   :
	rm -f php_parser.tab.h php_parser.tab.c php_parser.output lex.yy.c *.o order.xml yaxx.dtd

realclean:
	${MAKE} clean
	rm -f php ${PREFIX}xx.xml ${PREFIX}xx.dtd
