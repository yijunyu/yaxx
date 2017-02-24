BISON=bison$(X)
PREFIX=yy
NFR = $(wildcard *.nfr)
all : $(NFR:.nfr=.xml)
%.xml : %.nfr nfr
	./nfr $< 
	mv yaxx.xml $@
JUNK += *.xml ${PREFIX}yaxx.dtd

nfr : nfr.tab.c lex.yy.c
	gcc -g -o $@ $^ 
JUNK += nfr.exe nfr

YFLAGS  = -dv -S yaxx-${Y}-bison-$(X).c
nfr.tab.c: nfr.y
	bison$(VER) ${YFLAGS} $<
JUNK += nfr.tab.c nfr.tab.h
JUNK += nfr.output

lex.yy.c : nfr.l
	flex $<
JUNK += lex.yy.c

clean::
	rm -f ${JUNK}

realclean:: clean
	rm -rf yaxx.dtd nfr.dSYM
