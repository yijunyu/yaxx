#!/bin/tcsh
make clean
setenv BISON_SIMPLE ../bison/src/bison.simple.yaxx
make
./sql1 test.sql1
sed -e 's/yaxx.dtd/sql1.dtd/' yaxx.xml > test.sql1.xml
mv -f yaxx.dtd sql1.dtd
./sql2 test.sql2
sed -e 's/yaxx.dtd/sql2.dtd/' yaxx.xml > test.sql2.xml
mv -f yaxx.dtd sql2.dtd
ln -s ../yaxx/yaxx.xsl .
xsltproc test.sql1.xml
xsltproc test.sql2.xml
