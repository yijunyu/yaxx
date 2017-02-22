#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "format.h"
#include "format.rhf.c"
#include "format.bib.c"

struct {
  char *name ;
  Format *f ;
  char **template ;
} formats[] = {
  { "bib", &bibformat, bibtemplate },
  { "refer", &bibformat, bibtemplate },
  { "rhf", &rhfformat, rhftemplate },
} ;

Format *theformat ;
char  **thetemplate ;
int         ntemplate ;
char  **extratemplate ;

#define MAXSTORE 40

int nf, firstfield ;

struct store {
  char *name ;
  char *contents ;
  int len ;
} fields[ MAXSTORE ] ;

char *thekey ;
char *thetype ;

void outputformat( char *format, int nt, char **tv )
{
  int i ;
  char c = ' ' ; 

  ntemplate = nt ;
  extratemplate = tv ;
  firstfield = nf ;
  for( i=0 ; i<sizeof( formats ) / sizeof( formats[0] ) ; i++ ) {
    if( strcasecmp( formats[i].name, format ) == 0 ) {
      theformat   = formats[i].f ;
      thetemplate = formats[i].template ;
      return ;
    }
  }
  fprintf(stderr, "Undefined format %s, choose from", format ) ;
  for( i=0 ; i<sizeof( formats ) / sizeof( formats[0] ) ; i++ ) {
    fprintf( stderr, "%c\"%s\"", c, formats[i].name ) ;
    c = ',' ;
  }
  fprintf( stderr, "\n" ) ;
  exit( 1 ) ;
}

void reffield( char *name, char *contents )
{
  if( nf >= MAXSTORE ) {
    fprintf(stderr, "Ridiculous number of fields; key=%s\n", thekey ) ;
  } else {
    fields[ nf ].name = strdup( name ) ;
    fields[ nf ].len = strlen( name ) ;
    fields[ nf ].contents = strdup( contents ) ;
    nf++ ;
  }
}

void refkey( char *type, char *key )
{
  nf = firstfield ;
  reffield( "key", thekey = strdup( key ) ) ;
  reffield( "type", thetype = strdup( type ) ) ;
}

void disposefields( void )
{
  int i ;

  for( i=firstfield ; i<nf ; i++ ) {
    free( fields[ i ].name ) ;
    free( fields[ i ].contents ) ;
  }
}

char *getvalue( char *field, int len )
{
  int i ;

  for( i=0 ; i<nf ; i++ ) {
    if( len == fields[ i ].len &&
	strncasecmp( field, fields[ i ].name, fields[ i ].len ) == 0 ) {
      return fields[ i ].contents ;
    }
  }
  return NULL ;
}

static char *output = NULL ;
static int outputlen = 0 ;
static int outputindex = 0 ;
static int newline = 1 ;

void appendchar( char c )
{
  if( outputlen <= outputindex + 80 ) {
    if( outputlen == 0 ) {
      outputlen = 80 ;
      output = malloc( outputlen ) ;
    } else {
      outputlen = outputlen * 2 ;
      output = realloc( output, outputlen ) ;
    }
  }
  if( c == '\n' ) {
    if( newline ) {
      strcpy( &output[ outputindex ], theformat->emptylines ) ;
      outputindex += strlen( theformat->emptylines ) ;
      return ;
    }
    newline = 1 ;
  } else {
    newline = 0 ;
  }
  output[ outputindex++ ] = c ;
}

void appendstring( char *s, int len )
{
  while( len-- > 0 ) {
    appendchar( *s++ ) ;
  }
}

void printtemplate( char *s, int matched ) ;

void andsplit( char *contents, char *s )
{
  int save = outputindex ;
  char c[100] ;

  while( *contents ) {
    while( *contents != 0 ) {
      while( !isspace( *contents ) && *contents != '\0' ) {
	appendchar( *contents ) ;
	contents++ ;
      }
      if( sscanf( contents, " and%[ \t\n]", &c ) == 1 ) {
	while( isspace( *contents ) ) contents++ ;
	while( !isspace( *contents ) ) contents++ ;
	while( isspace( *contents ) ) contents++ ;
	break ;
      }
      while( isspace( *contents ) && *contents != '\0' ) {
	appendchar( *contents ) ;
	contents++ ;
      }
    }
    printtemplate( s, 1 ) ;
    outputindex = save ;
  }
}

void commasplit( char *contents, char *s )
{
  int save = outputindex ;

  while( *contents ) {
    while( (*contents == ',' || isspace( *contents )) && *contents != '\0' ) {
      contents++ ;
    }
    if( *contents == '\0' ) {
      break ;
    }
    while( *contents != ',' && *contents != '\0' ) {
      appendchar( *contents ) ;
      contents++ ;
    }
    printtemplate( s, 1 ) ;
    outputindex = save ;
  }
}

void printtemplate( char *s, int matched )
{
  char *keyword, *contents ;
  int nothingtomatch = 1 ;

  while( *s ) {
    if( *s == '%' ) {
      ++s ;
      if( *s == '%' ) {
	appendchar( *s ) ;
	s++ ;
	continue ;
      } 
      nothingtomatch = 0 ;
      keyword = s ;
      while( isalpha( *s ) && *s != '_' ) {
	s++ ;
      }
      contents = getvalue( keyword, s-keyword ) ;
      if( *s == '_' ) {
	keyword = ++s ;
	while( isalpha( *s ) && *s != '_' ) {
	  s++ ;
	}
	if( contents != NULL ) {
	  if( s-keyword == 10 && strncmp( keyword, "techreport", 10 ) == 0 ) {
	    if( strcasecmp( thetype, "@techreport" ) != 0 ) {
	      contents = NULL ;
	    }
	  } else if( s-keyword == 12 && strncmp( keyword, "notechreport", 12 ) == 0 ) {
	    if( strcasecmp( thetype, "@techreport" ) == 0 ) {
	      contents = NULL ;
	    }
	  } else if( s-keyword == 10 && strncmp( keyword, "commasplit", 10 ) == 0){
	    commasplit( contents, s ) ;
	    return ;
	  } else if( s-keyword == 8 && strncmp( keyword, "andsplit", 8 ) == 0 ){
	    andsplit( contents, s ) ;
	    return ;
	  } else {
	    fprintf( stderr, "Unknown modifier %.*s\n", s-keyword, keyword ) ;
	    exit( 1 ) ;
	  }
	}
      }
      if( contents != NULL ) {
	appendstring( contents, strlen( contents ) ) ;
	matched = 1 ;
      } else {
	while( outputindex != 1 && !isspace( output[ outputindex-1 ] ) ) {
	  outputindex-- ;
	}
	while( outputindex != 1 && isspace( output[ outputindex-1 ] ) ) {
	  outputindex-- ;
	}
	while( *s && !isspace( *s ) ) {
	  s++ ;
	}
      }
    } else {
      appendchar( *s ) ;
      ++s ;
    }
  }
  if( matched || nothingtomatch ) {
    output[ outputindex ] = '\0' ;
    printf( "%s", output ) ;
  }
}

printtemplateinit( char *s )
{
  outputindex = 0 ;
  printtemplate( s, 0 ) ;
}

void endref( void )
{
  int i ;

  newline = 1 ;
  printf( "%s", theformat->recordstart ) ;
  for( i=0 ; thetemplate[i] != NULL ; i++ ) {
    printtemplateinit( thetemplate[i] ) ;
  }
  for( i=0 ; i < ntemplate ; i++ ) {
    printtemplateinit( extratemplate[i] ) ;
  }
  printf( "%s", theformat->recordend ) ;
  disposefields() ;
}
