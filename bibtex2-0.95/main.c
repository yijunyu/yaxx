#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int flatten = 1 ;
static char *argv0 ;

usage( char*s, char *t )
{
  fprintf( stderr, s, t ) ;
  fprintf( stderr, "Usage: %s [-f] [-l language] {-T template} [file]\n", argv0 ) ;
  fprintf( stderr, "Useful -T are: '%%%%AV ftp://.../%%ftp' -T '%%OC orgc' etc.\n" ) ;
  exit( 1 ) ;
}

main( int argc, char *argv[] )
{
  int i ;
  char *s, *language = "bib" ;
  char **templatelist = malloc(1) ;
  int ntemplate = 0 ;

  argv0 = argv[0] ;
  for( i=1 ; i<argc ; i++ ) {
    if( argv[i][0] == '-' ) {
      switch( argv[i][1] ) {
	case 'f' :
	  flatten = !flatten ;
	  break ;
	case 'l' :
	  language = argv[i][2] == '\0' ? argv[++i] : argv[i]+2 ;
	  break ;
	case 'T' :
	  templatelist = realloc( templatelist, sizeof( char *)*(ntemplate+1)) ;
	  s = argv[i][2] == '\0' ? argv[++i] : argv[i]+2 ;
	  templatelist[ ntemplate ] = malloc( strlen( s ) + 2 ) ;
	  strcpy( templatelist[ ntemplate ], s ) ;
	  strcat( templatelist[ ntemplate ], "\n" ) ;
	  ntemplate++ ;
	  break ;
	default :
	  usage( "Unknown flag %s\n", argv[i] ) ;
      }
    } else {
      break ;
    }
  }
  outputformat( language, ntemplate, templatelist ) ;
  if( i != argc ) {
    if( i != argc-1 ) {
      usage( "Too many files %s ...\n", argv[i] ) ;
    }
    if( freopen( argv[i], "r", stdin ) == NULL ) {
      usage( "Cannot open file %s\n", argv[i] ) ;
    }
  }
  yyparse() ;
}
