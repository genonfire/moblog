/**
FILE NAME : write.c
USE : write to a file from data that user sent
INCLUDE : lib.c
COMPILED NAME : write.cgi
AUTHOR AND DATE : Gen, OCT 2005
**/

#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ "write.c"


#include "lib.c"

int main()
{
    char *ContentBuffer;
    char *NameBuffer;
    char *PasswordBuffer;
    char *SubjectBuffer;
    char *MessageBuffer;
    int n, i;
    MESSAGE writeBuffer;

#ifdef __UA_CHECK__
    if( !CheckUAProfile() ) {
        DisplayError( _UAPROFILEISNOTALLOWED );
        exit( 0 );
    }
#endif

    ContentBuffer = GetContent( &n );

    MessageBuffer = GetMessage( ContentBuffer );
    SubjectBuffer = GetSubject( ContentBuffer );
#ifdef __PASSWORD__
    PasswordBuffer = GetPassword( ContentBuffer );
#endif
    NameBuffer = GetName( ContentBuffer );

#ifdef __CHECK_NULL_NAMESUBJECTMESSAGE__
    if( !strlen( NameBuffer ) || !strlen( SubjectBuffer ) || !strlen( MessageBuffer ) ) {
        DisplayError( _NULLFORNAMESUBJECTMESSAGE );
        exit( 0 );
    }
#endif

    writeBuffer.refno = GetNewRefNumber() + 1;
    strcpy( writeBuffer.name, NameBuffer );
    strcpy( writeBuffer.subject, SubjectBuffer );
    strcpy( writeBuffer.password, PasswordBuffer );

    //strcpy( writeBuffer.message, MessageBuffer );
    Convert_String( writeBuffer.message, MessageBuffer );

#ifdef __DATE_FOR_LIST__
    GetOSDate( (char*)writeBuffer.osdate );
#endif
#ifdef __TIMEANDDATE_FOR_ARTICLE__
    GetUnixDate( (char*)writeBuffer.unixdate );
#endif

#ifdef __SAVE_IP_ADDRESS__
    GetIPAddress( (char*)writeBuffer.ip );
#endif
#ifdef __SAVE_UAPROFILE__
    GetUAProfile( (char*)writeBuffer.ua );
#endif
#ifdef __DELETE_NOT_SELECTABLE__
    SetValid( &writeBuffer.valid );
#endif
#ifdef __REPLY__
    SetReplyNo( &writeBuffer.replyno );
#endif

    if( !WritetoFile( &writeBuffer ) ) {
        DisplayError( _FAILTOWRITEDB );
        ReleaseContent( ContentBuffer );
        exit( 0 );
    } else
        SetNewRefNumber( writeBuffer.refno );

    ReleaseContent( ContentBuffer );

    memset( gs_infomessage, 0x00, _MESSAGE_MAX_STRING );
    strcpy( gs_infomessage, "Writing completed successfully" );

#ifdef __IMAGE_UPLOAD__
    DisplayMessageAndRedirect( (char*)gs_infomessage, (char*)gs_urlupload );
#else
    DisplayMessageAndRedirect( (char*)gs_infomessage, (char*)gs_urlwritesuccess );
#endif

    return 0;
}

