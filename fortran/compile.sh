#!/bin/tcsh
make clean
setenv BISON_SIMPLE ../bison/src/bison.simple.yaxx
make
./fortran77 ex.f
sed -e 's/yaxx.dtd/fortran77.dtd/' yaxx.xml > ex.xml
mv -f yaxx.dtd fortran77.dtd
rm -f yaxx.xml
ln -sf ../yaxx/yaxx.xsl .
xsltproc ex.xml
