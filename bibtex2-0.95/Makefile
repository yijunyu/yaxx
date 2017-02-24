BINDIR = .
VER=2.3
CC=gcc
YACC=bison$(VER)
YFLAGS  = -dv -S yaxx-0.13-bison-$(VER).c
LEX=flex -f
CFLAGS=-O

SOURCES = yacc.y lex.l main.c format.c
OTHER = format.h format.bib.c format.rhf.c abbreviations.bib
OBJECTS = yacc.o format.o main.o
BINARY = $(BINDIR)/bibtex2
VERSION=0.95


.y.c:
	$(YACC) ${YFLAGS} yacc.y
	mv yacc.tab.c yacc.c

.l.c:
	$(LEX) lex.l
	mv lex.yy.c lex.c

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BINARY) $(OBJECTS) -ll

distribution:
	mkdir bibtex2-$(VERSION)
	ln $(SOURCES) $(OTHER) Makefile README bibtex2-$(VERSION)
	tar cf - bibtex2-$(VERSION) | gzip > bibtex2-$(VERSION).tar.gz
	cp bibtex2-$(VERSION).tar.gz $$HOME/public_html/sources/
	/bin/rm -rf bibtex2-$(VERSION)

yacc.o: yacc.c lex.c

clean:
	rm -f yacc.tab.h yacc.tab.c yacc.output lex.yy.c *.o

realclean:
	${MAKE} clean
	rm -f bibtex2* ${PREFIX}xx.xml ${PREFIX}xx.dtd
