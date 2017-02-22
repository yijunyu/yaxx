%{
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef char *charstar ;
#define YYSTYPE charstar

extern int flatten ;
void endref( void ) ;
void reffield( char *name, char *contents ) ;
void refkey( char *type, char *key ) ;
char *emptystring( void ) ;
char *stryytext( void ) ;
char *stryychar( char alternative ) ;
char *scat( char *s, char *t ) ;
void define (char *a, char *b ) ;
char *lookup (char *a) ;

%}
%token DEFINESTRING
%token ATSTRING
%token STRING
%token QUOTE
%token LBRACKET
%token RBRACKET
%token LCBRACKET
%token RCBRACKET
%token EQUAL
%token COMMA
%token HASHSIGN
%token WHITE

%start file

%%

file	:	w entries

entries :	/* empty */
	|	entry entries
	;

w	:	/* empty */
	|	WHITE w
	;

entry 	: 	ATSTRING w LBRACKET  key COMMA { refkey( $1, $4 ) ; }
			 w fields RBRACKET  w  { endref() ; }
	|	ATSTRING w LCBRACKET key COMMA { refkey( $1, $4 ) ; }
			 w fields RCBRACKET w  { endref() ; }
	|	DEFINESTRING w LCBRACKET key EQUAL w body RCBRACKET w
					       { define( $4, $7 ) ; }
	|	DEFINESTRING w LBRACKET key EQUAL w body RBRACKET w
					       { define( $4, $7 ) ; }
	|	error 
	;

key	:	w STRING w			{ $$ = $2 ; }
	|	w ATSTRING w			{ $$ = $2 ; }
	;

fields	:	field
	|	field fields
	;

field	:	STRING w EQUAL w body optcomma { reffield( $1, $5 ) ; }
	;

optcomma:	COMMA w optcomma
	|	
	;

body	:	string				{ $$ = $1 ; }
	|	string HASHSIGN w body		{ $$ = scat( $1, $4 ) ; }
	;

string 	:	STRING w 			{ $$ = lookup($1) ; }
	|	ATSTRING w 			{ $$ = $1 ; }
	|	QUOTE noquote QUOTE w 		{ $$ = $2 ; }
	|	LCBRACKET balbrack RCBRACKET w 	{ $$ = $2 ; }
	;

noquote	:	/* Empty */			{ $$ = emptystring() ; }
	|	noqsym noquote			{ $$ = scat( $1, $2 ) ; }
	;

noqsym 	: WHITE | ATSTRING | STRING | LBRACKET | RBRACKET | EQUAL | COMMA | HASHSIGN
	| LCBRACKET				{ $$ = flatten ? emptystring() : $1 ; }
	| RCBRACKET				{ $$ = flatten ? emptystring() : $1 ; }

	;

balbrack:	/* Empty */			{ $$ = emptystring() ; }
	|	LCBRACKET balbrack RCBRACKET balbrack
						{ if( flatten ) {
						    $$ = scat( $2, $4 ) ;
						  } else {
						    $$ = scat( scat( scat( strdup("{"), $2 ), strdup("}") ), $4 ) ;
						  }
						}
	|	bbsym balbrack			{ $$ = scat( $1, $2 ) ; }
	;

bbsym 	: WHITE | ATSTRING | STRING | LBRACKET | RBRACKET | QUOTE | EQUAL | COMMA | HASHSIGN
	;

%%

#include "lex.c"

yyerror( char *s )
{
  fprintf( stderr, "%d : %s\n", lineno, s ) ;
}

char *scat( char *s, char *t )
{
  char *z = malloc( strlen( s ) + strlen( t ) + 1 ) ;
  strcpy( z, s ) ;
  strcat( z, t ) ;
  free( s ) ;
  free( t ) ;
  return z;
}

char *stryytext( void )
{
  return strdup( yytext ) ;
}

char *stryychar( char alternative )
{
  if( flatten ) {
    char s[2] ;
    s[0] = alternative ;
    s[1] = '\0' ;
    return strdup( s ) ;
  } else {
    return strdup( yytext ) ;
  }
}

char *emptystring( void )
{
  return strdup( "" ) ;
}

typedef struct strings {
  struct strings *left, *right ;
  char *identifier ;
  char *contents ;
} tree ;

static tree *root = NULL ;

insert( tree **root, char *id, char *contents ) {
  if( *root == NULL ) {
    *root = (tree *) malloc( sizeof( tree ) ) ;
    (*root)->left = NULL ;
    (*root)->right = NULL ;
    (*root)->identifier = id ;
    (*root)->contents = contents ;
  } else if( strcasecmp( id, (*root)->identifier) < 0 ) {
    insert( &(*root)->left, id, contents ) ;
  } else {
    insert( &(*root)->right, id, contents ) ;
  }
}

void define( char *id, char *contents )
{
  insert( &root, id, contents ) ;
}

char *lookup( char *id )
{
  tree *s ;
  int comp ;

  s = root ;
  while( s != NULL && (comp = strcasecmp( id, s->identifier ) ) != 0 ) {
    if( comp < 0 ) {
      s = s->left ;
    } else {
      s = s->right ;
    }
  }
  return s == NULL ? id : s->contents ;
}
