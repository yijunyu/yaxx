X=2.5
Y=0.13
BISON=bison-${X}

SUBDIRS=$(patsubst %Makefile,%,$(wildcard */Makefile))

all clean realclean: $(SUBDIRS)
	for d in $(SUBDIRS); do (cd ./$$d ; $(MAKE) X=${X} Y=${Y} $@ ); done
	rm -f README.html

setup: ${BISON}
	cp bison/data/yaxx-${Y}-bison-${X}.c /usr/local/share/bison
	cd $(BISON) && ./configure --program-suffix=${X} && make && make install

${BISON}:
	if [ ' -d ${BISON} ' ]; then wget https://ftp.gnu.org/gnu/bison/${BISON}.tar.bz2; tar xvfj ${BISON}.tar.bz2; fi
