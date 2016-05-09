/**
FILE NAME : move.c
USE : move into a list which user wants
INCLUDE : lib.c
COMPILED NAME : move.cgi
AUTHOR AND DATE : Gen, OCT 2005
**/

#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ "move.c"


#include "lib.c"

int main()
{
    char *ContentBuffer;
    int n;
    int listno;
    int maxpage;

#ifdef __UA_CHECK__
    if( !CheckUAProfile() ) {
        DisplayError( _UAPROFILEISNOTALLOWED );
        exit( 0 );
    }
#endif

    ContentBuffer = GetContent( &n );
    listno = GetListNumberForMove( ContentBuffer );
    maxpage = GetNewRefNumber();

    maxpage = GetMaxPage( maxpage, 0 );

    memset( gs_infomessage, 0x00, _MESSAGE_MAX_STRING );
    memset( gs_urlmoveinto, 0x00, _URL_MAX_MOVE );

    if( listno > maxpage ) {
        sprintf( gs_infomessage, "List number is up to %d, try again", maxpage );
        sprintf( gs_urlmoveinto, "move_form.cgi" );
    } else {
        sprintf( gs_infomessage, "Moving to list no.%d", listno );
        sprintf( gs_urlmoveinto, "board.cgi?%d", listno );
    }

    DisplayMessageAndRedirect( (char*)gs_infomessage, (char*)gs_urlmoveinto );

    ReleaseContent( ContentBuffer );

    return 0;
}

