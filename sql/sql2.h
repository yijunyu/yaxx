
/* A Bison parser, made by GNU Bison 2.5.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NAME = 258,
     STRING = 259,
     INTNUM = 260,
     APPROXNUM = 261,
     OR = 262,
     AND = 263,
     NOT = 264,
     COMPARISON = 265,
     UMINUS = 266,
     ALL = 267,
     AMMSC = 268,
     ANY = 269,
     AS = 270,
     ASC = 271,
     AUTHORIZATION = 272,
     BETWEEN = 273,
     BY = 274,
     CHARACTER = 275,
     CHECK = 276,
     CLOSE = 277,
     COMMIT = 278,
     CONTINUE = 279,
     CREATE = 280,
     CURRENT = 281,
     CURSOR = 282,
     DECIMAL = 283,
     DECLARE = 284,
     DEFAULT = 285,
     DELETE = 286,
     DESC = 287,
     DISTINCT = 288,
     DOUBLE = 289,
     ESCAPE = 290,
     EXISTS = 291,
     FETCH = 292,
     FLOAT = 293,
     FOR = 294,
     FOREIGN = 295,
     FOUND = 296,
     FROM = 297,
     GOTO = 298,
     GRANT = 299,
     GROUP = 300,
     HAVING = 301,
     IN = 302,
     INDICATOR = 303,
     INSERT = 304,
     INTEGER = 305,
     INTO = 306,
     IS = 307,
     KEY = 308,
     LANGUAGE = 309,
     LIKE = 310,
     NULLX = 311,
     NUMERIC = 312,
     OF = 313,
     ON = 314,
     OPEN = 315,
     OPTION = 316,
     ORDER = 317,
     PARAMETER = 318,
     PRECISION = 319,
     PRIMARY = 320,
     PRIVILEGES = 321,
     PROCEDURE = 322,
     PUBLIC = 323,
     REAL = 324,
     REFERENCES = 325,
     ROLLBACK = 326,
     SCHEMA = 327,
     SELECT = 328,
     SET = 329,
     SMALLINT = 330,
     SOME = 331,
     SQLCODE = 332,
     SQLERROR = 333,
     TABLE = 334,
     TO = 335,
     UNION = 336,
     UNIQUE = 337,
     UPDATE = 338,
     USER = 339,
     VALUES = 340,
     VIEW = 341,
     WHENEVER = 342,
     WHERE = 343,
     WITH = 344,
     WORK = 345
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1979 of yaxx-0.13-bison-2.5.c  */
#line 3 "sql2.y"

	int intval;
	double floatval;
	char *strval;
	int subtok;



/* Line 1979 of yaxx-0.13-bison-2.5.c  */
#line 151 "sql2.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


