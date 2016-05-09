/**
FILE NAME : move_form.c
USE : display a form gets user input to move into another list
INCLUDE : options.h
COMPILED NAME : move_form.cgi
AUTHOR AND DATE : Gen, OCT 2005
**/

#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ "move_form.c"


#include "options.h"

int main()
{
    printf( "Content-type: text/html\n" );
    printf( "\n" );

    printf( "<html><head><title>Move</title><meta http-equiv=\"cache-control\" content=\"no-cache\"/></head><body>" );
    printf( "<form name=cgi method=POST action=move.cgi>" );
    printf( "Go to list (in figures)<br>" );
    printf( "<input type=text name=gencodeurnumber maxlength=10>" );
    printf( "<input type=submit value=submit>" );
    printf( "<input type=reset value=clear><br></form>" );
    printf( "</body></html>" );

    return 0;
}

