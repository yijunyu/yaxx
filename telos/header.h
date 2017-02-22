#ifndef HEADER_H 
#define HEADER_H
#include <stdio.h>
#include <iostream> 
#include <sstream> 
#include <map>
#include <string>
using namespace std;
int yylex();
int yyerror(char *);

extern char * yytext;
extern int count_element;
extern int count_link;

typedef map<string, string> TUPLES;
typedef TUPLES *TUPLES_PTR;

typedef struct {
  char oid[260];
  TUPLES* attributes;
} OBJECT, *OBJECT_PTR;

extern map<string, OBJECT*> objects;

typedef struct {
 char name[260];
 char value[260];
} PAIR, *PAIR_PTR;

#endif
