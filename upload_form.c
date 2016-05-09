/**
FILE NAME : upload_form.c
USE : display a form gets an image from users
INCLUDE : options.h
COMPILED NAME : upload_form.cgi
AUTHOR AND DATE : Gen, MAR 2006
**/

#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ "upload_form.c"


#include "options.h"

int main()
{
    printf( "Content-type: text/html\n" );
    printf( "\n" );

    printf( "<html><head><title>Upload Image</title><meta http-equiv=\"cache-control\" content=\"no-cache\"/></head><body>" );

    printf( "<form method=post action=upload.cgi enctype=multipart/form-data>" );
    printf( "<input type=file name=image>" );
    printf( "<input type=submit value=submit>" );

    printf( "</form>" );
    printf( "<br><a href=board.cgi>Skip</a></body></html>" );

    return 0;
}

