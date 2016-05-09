/**
FILE NAME : reset_form.c
USE : display a form gets an password from users
INCLUDE : options.h
COMPILED NAME : reset_form.cgi
AUTHOR AND DATE : Gen, MAR 2006
**/

#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ "reset_form.c"


#include "options.h"

int main( int argc, char *argv[] )
{
#ifdef __FULL_RESET__
    printf( "Content-type: text/html\n" );
    printf( "\n" );

    printf( "<html><head><title>Reset DB</title><meta http-equiv=\"cache-control\" content=\"no-cache\"/></head><body>" );
    printf( "<form name=cgi method=POST action=reset.cgi>");
    printf( "<center><font color=red>*** WARNING ***</font></center><br>" );
    printf( "This is gonna delete all of your Database, if you want to continue input the master password..<br>" );
    printf( "<input type=password name=gencodeurnumber maxlength=%d>", _PASSWORD_MAX_STRING );
    printf( "<input type=submit value=submit>" );
    printf( "<input type=reset value=clear><br></form>" );
    printf( "</body></html>" );

#else
    DisplayError( _NOTSUPPORTFULLRESET );
#endif

    return 0;
}

