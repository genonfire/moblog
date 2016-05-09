/**
FILE NAME : board.c
USE : display the list of contents for wapboard by default
INCLUDE : lib.c
COMPILED NAME : board.cgi
AUTHOR AND DATE : Gen, OCT 2005
**/

#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ "board.c"

#include "lib.c"

int main( int argc, char *argv[] )
{
    int page;
    int maxpage;
    int list; // returned by GetNewRefNumber() and might be modified by GetListNumber()
    int totallist; // returned by GetNewRefNumber()
    int listsize; // number of messages will be displayed on current list
    bool firstpage = TRUE; // a flag indicates the first page itself
    bool lastpage = FALSE; // a flag indicates the last page itself

#ifdef __UA_CHECK__
    if( !CheckUAProfile() ) {
        DisplayError( _UAPROFILEISNOTALLOWED );
        exit( 0 );
    }
#endif

    if( argc > 3 || _LIST_SIZE == 0 ) {
        DisplayError( _DEFAULT );
        exit( 0 );
    }
    else if( argc == 1 || !atoi( argv[ 1 ] ) )
        page = 1;
    else
        page = atoi( argv[ 1 ] );

    list = GetNewRefNumber();

    totallist = list;
    maxpage = GetMaxPage( list, page );
    listsize = GetListNumber( &list, page );
    firstpage = SetFirstPage( page, maxpage );
    lastpage = SetLastPage( page, maxpage );

    DisplayList( list, totallist, page, maxpage, listsize, firstpage, lastpage );

    return 0;
}

