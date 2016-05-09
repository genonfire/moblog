/**
FILE NAME : view.c
USE : view an article
INCLUDE : lib.c
COMPILED NAME : view.cgi
AUTHOR AND DATE : Gen, OCT 2005
**/

#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ "view.c"


#include "lib.c"

int main( int argc, char *argv[] )
{
    int msgno;
    int maxno;

#ifdef __UA_CHECK__
    if( !CheckUAProfile() ) {
        DisplayError( _UAPROFILEISNOTALLOWED );
        exit( 0 );
    }
#endif

     if( argc > 3 || _LIST_SIZE == 0 ) {
        DisplayError( _DEFAULT );
        exit( 0 );
    }
    else if( argc == 1 || !atoi( argv[ 1 ] ) )
        msgno = 1;
    else
        msgno = atoi( argv[ 1 ] );

    maxno = GetNewRefNumber();

    if( !msgno || msgno > maxno ) {
        DisplayError( _NOMOREARTICLES );
        exit( 0 );
    } else
        DisplayArticle( msgno );

    return 0;
}

