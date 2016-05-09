/**
FILE NAME : delete.c
USE : delete an article
INCLUDE : lib.c
COMPILED NAME : delete.cgi
AUTHOR AND DATE : Gen, OCT 2005
**/

#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ "delete.c"


#include "lib.c"

int main( int argc, char *argv[] )
{
    char *ContentBuffer;
    char *PasswordBuffer;
    int n;
    int msgno;
    int maxno;
    int errno;

    ContentBuffer = GetContent( &n );
    PasswordBuffer = GetPasswordToDelete( ContentBuffer );

    strcpy( (char*)gs_password, PasswordBuffer );

    msgno = atoi( argv[ 1 ] );
    maxno = GetNewRefNumber();

    if( !msgno || msgno > maxno ) {
        DisplayError( _NOMOREARTICLES );
        ReleaseContent( ContentBuffer );
        exit( 0 );
    }

    errno = DeleteArticle( msgno );

    if( errno == _PASSWORDMISSMATCH ) {
        DisplayError( _PASSWORDMISSMATCH );
        memset( (char*)gs_backurl, 0x00, _URL_MAX_STRING );
        sprintf( (char*)gs_backurl, "view.cgi?%d", msgno );
        ReleaseContent( ContentBuffer );
        exit( 0 );
    }
    else if( errno == _NOPASSWORD ) {
        DisplayError( _NOPASSWORD );
        memset( (char*)gs_backurl, 0x00, _URL_MAX_STRING );
        sprintf( (char*)gs_backurl, "view.cgi?%d", msgno );
        ReleaseContent( ContentBuffer );
        exit( 0 );
    }
    else if( !errno ) {
        DisplayError( _FAILTODELETEDB );
        memset( (char*)gs_backurl, 0x00, _URL_MAX_STRING );
        sprintf( (char*)gs_backurl, "view.cgi?%d", msgno );
        ReleaseContent( ContentBuffer );
        exit( 0 );
    }
    else {
        memset( gs_infomessage, 0x00, _MESSAGE_MAX_STRING );
        strcpy( gs_infomessage, "Deleting completed successfully" );

        DisplayMessageAndRedirect( (char*)gs_infomessage, (char*)gs_urldeletesuccess );
    }

    ReleaseContent( ContentBuffer );

    return 0;
}

