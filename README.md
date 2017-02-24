YAXX - Yet Another XML eXtension to Bison

= Source files = 

* Makefile -- the makefile
* bison/data/yacc-$VER.c -- the source file taken from bison-$VER/data/yacc.c
* bison/data/yaxx-$Y-bison-$VER.c -- the file version $Y patched on yacc-$VER.c
* yaxx/yaxx.xsl -- XSL transformation for YAXX parsing tree

= Example Grammars =

* ada95/ -- Ada 95
* ansic/ -- ANSI C, see Yu et al. ICSM'05 paper
* bibtex2-0.95/ -- BibTeX, see Yu et al. Literature Programming paper
* csharp/ -- C#, see Akiki et al. ICSE'14, TSE'16, TOCHI'17 papers
* fortran/ -- Fortran 77, see Yu et al. ICPP'00, VL'00, JVLC'01, ICPP'04 papers
* gcc-3.4.0/ -- GCC, see Yu et al. FASE'05 and ICSM'05 papers
* java/ -- Java, see Yu et al. ASE'11 paper and ICSE'12 paper, Butler et al. WCRE'09, ECOOP'11, ICSME'15 paper
* nfr/ -- Non-functional Requirements, see Giorgini et al ER'02 and Mylopoulos et al. TSE'91 papers
* php/ -- PHP, see Yu et al. RE'04, RE'05, RE'07 papers and Candillon et al.'s APDT tool: https://code.google.com/archive/p/apdt/
* sql/ -- SQL, see Zhang et al. VLDB'04, Lu et al. WIRI'05 paper
* telos/ -- Telos, a knowledge representation language underlying ConceptBase, see http://conceptbase.sourceforge.net
* xml/ -- XML, see Yu et al. SPE'03, Lapouchnian et al. CASCON'06, BPM'07, Yu et al. ICWS'08 papers

== Documentation =

* docs/Makefile -- makefile
* docs/Doxyfile -- doxygen input
* docs/changelog.xml -- docbook chapter
* docs/manual.xml -- docbook chapter
* docs/readme.xml -- docbook chapter
* docs/requirements.xml -- docbook chapter

-- Yijun Yu
