#!/bin/tcsh
make clean
setenv BISON_SIMPLE ../bison/src/bison.simple.yaxx
make
./ansic < main.c
sed -e 's/yaxx.dtd/ansic.dtd/' yaxx.xml > main.xml
mv -f yaxx.dtd ansic.dtd
rm -f yaxx.xml
ln -sf ../yaxx/yaxx.xsl . 
xsltproc main.xml
