/**
FILE NAME : reply.c
USE : write to a file from data that user append
INCLUDE : lib.c
COMPILED NAME : reply.cgi
AUTHOR AND DATE : Gen, OCT 2005
**/

#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ "reply.c"


#include "lib.c"

int main( int argc, char *argv[] )
{
    char *ContentBuffer;
    char *NameBuffer;
    char *MessageBuffer;
    int n;
    int msgno;
    int maxno;
    REPLY replybuffer;

#ifdef __UA_CHECK__
    if( !CheckUAProfile() ) {
        DisplayError( _UAPROFILEISNOTALLOWED );
        exit( 0 );
    }
#endif

    msgno = atoi( argv[ 1 ] );
    maxno = GetNewRefNumber();

    if( !msgno || msgno > maxno ) {
        DisplayError( _NOMOREARTICLES );
        exit( 0 );
    }

    ContentBuffer = GetContent( &n );
    MessageBuffer = GetMessage( ContentBuffer );
    NameBuffer = GetName( ContentBuffer );

    if( !strlen( NameBuffer ) || !strlen( MessageBuffer ) ) {
        DisplayError( _NULLFORNAMEANDCOMMENT );
        exit( 0 );
    }

    strcpy( (char*)replybuffer.name, NameBuffer );
    Convert_String( (char*)replybuffer.comment, MessageBuffer );
    replybuffer.msgno = msgno;
    replybuffer.valid = TRUE;
#ifdef __DATE_FOR_LIST__
    GetOSDate( (char*)replybuffer.osdate );
#endif
    replybuffer.replyno = GetReplyno( replybuffer.msgno );

    if( !WriteReplytoFile( &replybuffer ) ) {
        DisplayError( _FAILTOWRITEDB );
        ReleaseContent( ContentBuffer );
        exit( 0 );
    } else {
        memset( gs_infomessage, 0x00, _MESSAGE_MAX_STRING );
        memset( gs_urlreplysuccess, 0x00, _URL_MAX_MOVE );
        strcpy( gs_infomessage, "Replying completed successfully" );
        sprintf( (char*)gs_urlreplysuccess, "%s%d", "view.cgi?", msgno );

        DisplayMessageAndRedirect( (char*)gs_infomessage, (char*)gs_urlreplysuccess );
    }

    ReleaseContent( ContentBuffer );

    return 0;
}

