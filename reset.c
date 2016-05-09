/**
FILE NAME : reset.c
USE : reset DBs
INCLUDE : lib.c
COMPILED NAME : reset.cgi
AUTHOR AND DATE : Gen, MAR 2006
**/

#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ "reset.c"


#include "lib.c"

int main( int argc, char *argv[] )
{
#ifdef __FULL_RESET__
    char *ContentBuffer;
    char *PasswordBuffer;
    int n;
    int errno;

    ContentBuffer = GetContent( &n );
    PasswordBuffer = GetPasswordToDelete( ContentBuffer );

    strcpy( (char*)gs_password, PasswordBuffer );

    if( !strcmp( (char*)gs_masterpassword, (char*)gs_password ) ) {
        if( FullReset() ) {
            memset( gs_infomessage, 0x00, _MESSAGE_MAX_STRING );
            strcpy( gs_infomessage, "Full Reset Completed.." );
            DisplayMessageAndRedirect( (char*)gs_infomessage, (char*)gs_urldeletesuccess );
        } else
            DisplayError( _FULLRESETERROR );

    } else
        DisplayError( _PASSWORDMISSMATCH );

    ReleaseContent( ContentBuffer );

#else
    DisplayError( _NOTSUPPORTFULLRESET );
#endif

    return 0;
}

