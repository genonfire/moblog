/**
FILE NAME : reply_form.c
USE : get a comment from the user
INCLUDE : lib.c
COMPILED NAME : reply_form.cgi
AUTHOR AND DATE : Gen, OCT 2005
**/

#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ "reply_form.c"


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

    printf( "<html><head><title>Reply</title><meta http-equiv=\"cache-control\" content=\"no-cache\"/></head><body>" );
    printf( "<form name=cgi method=POST action=reply.cgi?%d>", msgno );
    printf( "Name<br>" );
    printf( "<input type=text name=gencodeurname maxlength=%d><br>", _NAME_MAX_STRING );
    printf( "Comment<br>" );
    printf( "<textarea name=gencodeurmessage rows=%d></textarea><br>", _MESSAGE_TEXTAREA_ROWS_ );
    printf( "<input type=submit value=submit>" );
    printf( "<input type=reset value=clear><br></form>" );
    printf( "</body></html>" );

    return 0;
}

