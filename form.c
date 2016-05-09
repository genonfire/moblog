/**
FILE NAME : form.c
USE : display a form gets user input
INCLUDE : options.h
COMPILED NAME : form.cgi
AUTHOR AND DATE : Gen, OCT 2005
**/

#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ "form.c"


#include "options.h"

int main()
{
    printf( "Content-type: text/html\n" );
    printf( "\n" );

    printf( "<html><head><title>Write</title><meta http-equiv=\"cache-control\" content=\"no-cache\"/></head><body>" );
    printf( "<form name=cgi method=POST action=write.cgi>" );
    printf( "Name <input type=text name=gencodeurname maxlength=%d><br>", _NAME_MAX_STRING );
#ifdef __PASSWORD__
    printf( "Password <input type=password name=gencodeurpasswd maxlength=%d><br>", _PASSWORD_MAX_STRING );
#endif
    printf( "Subject <input type=text name=gencodeursubject maxlength=%d><br>", _SUBJECT_MAX_STRING );
    printf( "Message<br>" );
    printf( "<textarea name=gencodeurmessage rows=%d></textarea><br>", _MESSAGE_TEXTAREA_ROWS_ );
#if 0 //def __IMAGE_UPLOAD__
    printf( "<input type=file name=binary  size=20  maxlength=255><br>" );
#endif
    printf( "<input type=submit value=submit> " );
    printf( "<input type=reset value=clear><br></form>" );
    printf( "</body></html>" );

    return 0;
}

