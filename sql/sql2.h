/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
/* Tokens.  */
#define NAME 258
#define STRING 259
#define INTNUM 260
#define APPROXNUM 261
#define OR 262
#define AND 263
#define NOT 264
#define COMPARISON 265
#define UMINUS 266
#define ALL 267
#define AMMSC 268
#define ANY 269
#define AS 270
#define ASC 271
#define AUTHORIZATION 272
#define BETWEEN 273
#define BY 274
#define CHARACTER 275
#define CHECK 276
#define CLOSE 277
#define COMMIT 278
#define CONTINUE 279
#define CREATE 280
#define CURRENT 281
#define CURSOR 282
#define DECIMAL 283
#define DECLARE 284
#define DEFAULT 285
#define DELETE 286
#define DESC 287
#define DISTINCT 288
#define DOUBLE 289
#define ESCAPE 290
#define EXISTS 291
#define FETCH 292
#define FLOAT 293
#define FOR 294
#define FOREIGN 295
#define FOUND 296
#define FROM 297
#define GOTO 298
#define GRANT 299
#define GROUP 300
#define HAVING 301
#define IN 302
#define INDICATOR 303
#define INSERT 304
#define INTEGER 305
#define INTO 306
#define IS 307
#define KEY 308
#define LANGUAGE 309
#define LIKE 310
#define NULLX 311
#define NUMERIC 312
#define OF 313
#define ON 314
#define OPEN 315
#define OPTION 316
#define ORDER 317
#define PARAMETER 318
#define PRECISION 319
#define PRIMARY 320
#define PRIVILEGES 321
#define PROCEDURE 322
#define PUBLIC 323
#define REAL 324
#define REFERENCES 325
#define ROLLBACK 326
#define SCHEMA 327
#define SELECT 328
#define SET 329
#define SMALLINT 330
#define SOME 331
#define SQLCODE 332
#define SQLERROR 333
#define TABLE 334
#define TO 335
#define UNION 336
#define UNIQUE 337
#define UPDATE 338
#define USER 339
#define VALUES 340
#define VIEW 341
#define WHENEVER 342
#define WHERE 343
#define WITH 344
#define WORK 345




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 3 "sql2.y"
{
	int intval;
	double floatval;
	char *strval;
	int subtok;
}
/* Line 1799 of yacc.c.  */
#line 236 "sql2.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

