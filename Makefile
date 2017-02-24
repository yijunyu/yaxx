X=2.3
Y=0.13
BISON = ../bison-${X}

SUBDIRS=$(patsubst %Makefile,%,$(wildcard */Makefile))

all clean realclean: $(SUBDIRS)
	for d in $(SUBDIRS); do (cd ./$$d ; $(MAKE) $@ ); done

setup:
	cp bison/data/yaxx-${Y}-bison-${X}.c /usr/local/share/bison/data
	cd $(BISON); ./configure --program-suffix=${X}
