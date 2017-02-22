#!/bin/tcsh
make clean
setenv BISON_SIMPLE ../bison/src/bison.simple.yaxx
make
./java11 < HelloWorld.java
sed -e 's/yaxx.dtd/java11.dtd/' yaxx.xml > HelloWorld.xml
mv -f yaxx.dtd java11.dtd
rm -f yaxx.xml
ln -sf ../yaxx/yaxx.xsl .
xsltproc HelloWorld.xml
