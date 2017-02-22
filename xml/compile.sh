#!/bin/tcsh
make clean
setenv BISON_SIMPLE ../bison/src/bison.simple.yaxx
make
./xml < test.xml
sed -e 's/yaxx.dtd/xml.dtd/' yaxx.xml > test-yaxx.xml
mv -f yaxx.dtd xml.dtd
rm -f yaxx.xml
ln -sf ../yaxx/yaxx.xsl . 
xsltproc test-yaxx.xml
