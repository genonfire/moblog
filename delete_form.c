/**
FILE NAME : delete_form.c
USE : get password and pass to delete an article
INCLUDE : lib.c
COMPILED NAME : delete_form.cgi
AUTHOR AND DATE : Gen, OCT 2005
**/

#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ "delete_form.c"


#include "lib.c"

int main( int argc, char *argv[] )
{
    int msgno;
    int maxno;

    msgno = atoi( argv[ 1 ] );
    maxno = GetNewRefNumber();

    if( !msgno || msgno > maxno ) {
        DisplayError( _NOMOREARTICLES );
        exit( 0 );
    }

    printf( "Content-type: text/html\n" );
    printf( "\n" );

    printf( "<html><head><title>Delete</title><meta http-equiv=\"cache-control\" content=\"no-cache\"/></head><body>" );
    printf( "<form name=cgi method=POST action=delete.cgi?%d>", msgno );
    printf( "Input password<br>" );
    printf( "<input type=password name=gencodeurnumber maxlength=%d>", _PASSWORD_MAX_STRING );
    printf( "<input type=submit value=submit>" );
    printf( "<input type=reset value=clear><br></form>" );
    printf( "</body></html>" );

    return 0;
}

