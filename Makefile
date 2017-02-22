#BISON = ${HOME}/bison-1.875
BISON = ../bison-2.1

SUBDIRS=$(patsubst %Makefile,%,$(wildcard */Makefile))

all clean realclean: $(SUBDIRS)
	for d in $(SUBDIRS); do (cd ./$$d ; $(MAKE) $@ ); done

setup:
	cp bison/data/yaxx-0.12-bison-2.1.c /usr/local/share/bison/data
	cd $(BISON); ./configure --program-suffix=2.1
