/**
FILE NAME : upload.c
USE : write to a file from image that user sent
INCLUDE : lib.c
COMPILED NAME : upload.cgi
AUTHOR AND DATE : Gen, MAR 2006
**/

#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ "upload.c"


#include "lib.c"

int main()
{
    char *ContentBuffer;
    int n;
    int refno;

    ContentBuffer = GetContent( &n );
    refno = GetNewRefNumber();

    if( !SaveImage( ContentBuffer, refno, n ) ) {
        DisplayError( _FAILTOSAVEIMAGE );
        ReleaseContent( ContentBuffer );
        exit( 0 );
    }

    ReleaseContent( ContentBuffer );

    memset( gs_infomessage, 0x00, _MESSAGE_MAX_STRING );
    strcpy( gs_infomessage, "Saving completed successfully" );

    DisplayMessageAndRedirect( (char*)gs_infomessage, (char*)gs_urluploadsuccess );

    return 0;
}

