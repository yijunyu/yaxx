#####################################################################
#
INPUT_FILE=		manual.xml
OTHER_INPUT=		readme.xml changelog.xml requirements.xml
#
#####################################################################

ONE_HTML_DIR=		one-html
SEP_HTML_DIR=		sep-html
POSTSCRIPT_DIR=		postscript
ONE_HTML_OUT=		${ONE_HTML_DIR}/${INPUT_FILE:.xml=.html}

SEP_HTML_OUT=		${SEP_HTML_DIR}/book1.htm

POSTSCRIPT_OUT=		${POSTSCRIPT_DIR}/${INPUT_FILE:.xml=.ps}
PDF_OUT=		${POSTSCRIPT_DIR}/${INPUT_FILE:.xml=.pdf}

PREFIX?=		/usr
JADE=			${PREFIX}/bin/openjade

#CATALOGS+=		-c ${PREFIX}/share/sgml/docbook/dsssl/modular/catalog
CATALOGS+=		-c ${PREFIX}/share/sgml/docbook/sgml-dtd-4.1/catalog
CATALOGS+=		-c ${PREFIX}/share/sgml/sgml-iso-entities-8879.1986/catalog
CATALOGS+=		-c ${PREFIX}/share/sgml/openjade-1.3/catalog

JADE_FLAGS=		${CATALOGS}

DSL_HTML=		${PREFIX}/share/sgml/docbook/dsssl-stylesheets/html/docbook.dsl
DSL_PRINT=		${PREFIX}/share/sgml/docbook/dsssl-stylesheets/print/docbook.dsl
HTMLFLAGS=		
PRINTFLAGS=		
TEXCMDS=

TIDY=			${PREFIX}/bin/tidy -i -m -raw -preserve -f /dev/null
TEX=			${PREFIX}/bin/tex
DVIPS=			${PREFIX}/bin/dvips
PDFTEX=			${PREFIX}/bin/pdftex

all: ${ONE_HTML_OUT} # ${SEP_HTML_OUT} ${POSTSCRIPT_OUT} ${PDF_OUT}
html: ${ONE_HTML_OUT} ${SEP_HTML_OUT}
ps: ${POSTSCRIPT_OUT}
pdf: ${PDF_OUT}

${ONE_HTML_OUT}: ${INPUT_FILE} ${OTHER_INPUT}
	mkdir -p ${ONE_HTML_DIR}
	cp ${INPUT_FILE} ${OTHER_INPUT} ${ONE_HTML_DIR}
	(\
		cd ${ONE_HTML_DIR}; \
		${JADE} ${JADE_FLAGS} -V nochunks ${HTMLFLAGS} -d ${DSL_HTML} -t sgml ${INPUT_FILE} > ${INPUT_FILE:.xml=.html}; \
		rm ${INPUT_FILE} ${OTHER_INPUT}; \
	)
	-${TIDY} ${ONE_HTML_OUT}
	if test -r ${FREEBSDDOC}/share/misc/docbook.css -a ! -r docbook.css ; then \
		cp ${FREEBSDDOC}/share/misc/docbook.css ${ONE_HTML_DIR}; \
	fi
	if test -r docbook.css ; then \
		cp docbook.css ${ONE_HTML_DIR}; \
	fi

${SEP_HTML_OUT}: ${INPUT_FILE} ${OTHER_INPUT}
	mkdir -p ${SEP_HTML_DIR}
	cp ${INPUT_FILE} ${OTHER_INPUT} ${SEP_HTML_DIR}
	(\
		cd ${SEP_HTML_DIR}; \
		${JADE} ${JADE_FLAGS} ${HTMLFLAGS} -d ${DSL_HTML} -t sgml ${INPUT_FILE}; \
		rm ${INPUT_FILE} ${OTHER_INPUT}; \
	)
	for file in `ls ${SEP_HTML_DIR}/*.htm?`; do \
		${TIDY} $$file || echo -n; \
	done
	if test -r ${FREEBSDDOC}/share/misc/docbook.css -a ! -r docbook.css ; then \
		cp ${FREEBSDDOC}/share/misc/docbook.css ${SEP_HTML_DIR}; \
	fi
	if test -r docbook.css ; then \
		cp docbook.css ${SEP_HTML_DIR}; \
	fi

${POSTSCRIPT_OUT}: ${INPUT_FILE} ${OTHER_INPUT}
	mkdir -p ${POSTSCRIPT_DIR}
	cp ${INPUT_FILE} ${OTHER_INPUT} ${POSTSCRIPT_DIR}
	(\
		cd ${POSTSCRIPT_DIR}; \
		${JADE} ${JADE_FLAGS} -Vtex-backend ${PRINTFLAGS} -d ${DSL_PRINT} -t tex ${INPUT_FILE}; \
		rm ${INPUT_FILE} ${OTHER_INPUT}; \
		${TEX} "&jadetex" '${TEXCMDS} \nonstopmode\input{${INPUT_FILE:.xml=.tex}}' ${INPUT_FILE:.xml=.tex}; \
		${TEX} "&jadetex" '${TEXCMDS} \nonstopmode\input{${INPUT_FILE:.xml=.tex}}' ${INPUT_FILE:.xml=.tex}; \
		${TEX} "&jadetex" '${TEXCMDS} \nonstopmode\input{${INPUT_FILE:.xml=.tex}}' ${INPUT_FILE:.xml=.tex}; \
		${DVIPS} -o ${INPUT_FILE:.xml=.ps} ${INPUT_FILE:.xml=.dvi}; \
	)

${PDF_OUT}: ${POSTSCRIPT_OUT} ${INPUT_FILE} ${OTHER_INPUT}
	(\
		cd ${POSTSCRIPT_DIR}; \
		${PDFTEX} "&pdfjadetex" '${TEXCMDS} \nonstopmode\input{${INPUT_FILE:.xml=.tex}}' ${INPUT_FILE:.xml=.tex}; \
		${PDFTEX} "&pdfjadetex" '${TEXCMDS} \nonstopmode\input{${INPUT_FILE:.xml=.tex}}' ${INPUT_FILE:.xml=.tex}; \
		${PDFTEX} "&pdfjadetex" '${TEXCMDS} \nonstopmode\input{${INPUT_FILE:.xml=.tex}}' ${INPUT_FILE:.xml=.tex}; \
	)

clean:
	ln -f ${ONE_HTML_DIR}/manual.html ../README.html
	rm -rf ${ONE_HTML_DIR} ${SEP_HTML_DIR} ${POSTSCRIPT_DIR}

realclean: clean
	rm one-html/manual.html
