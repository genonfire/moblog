/**
FILE NAME : lib.c
USE : libary that contains functions will be used by source files
INCLUDE : lib.h
COMPILED NAME : NONE
AUTHOR AND DATE : Gen, OCT 2005
**/

#ifdef __FILENAME__
#undef __FILENAME__
#endif
#define __FILENAME__ "lib.c"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "options.h"

#define DisplayError(a) {memset((char*)gv_file, 0x00, _TRACE_MAX_FILE); strcpy((char*)gv_file, __FILENAME__); gv_line=__LINE__; memset((char*)gv_function, 0x00, _TRACE_MAX_FUNCTION); strcpy((char*)gv_function, __FUNCTION__); DEBUG_DisplayError(a, TRUE);}

/***********************************
*** common functions that decode URL ***
***********************************/
char x2c( char *x )
{
    char digit;

    digit = ( x[0] >= 'A' ? ( ( x[0] & 0xdf ) - 'A' )+10 : ( x[0] - '0' ) );
    digit *= 16;
    digit += ( x[1] >= 'A' ? ( ( x[1] & 0xdf ) - 'A' )+10 : ( x[1] - '0' ) );

    return( digit );
}

void DecodeENCODEDURL( char *encodedURL )
{
    register int x,y;

    for( x=0, y=0; encodedURL[y]; ++x, ++y ) {
        if( ( encodedURL[x] = encodedURL[y] ) == '%' ) {
            encodedURL[x] = x2c( &encodedURL[y+1] );
            y+=2;
        }
    }

    encodedURL[x] = NULL;
}

void PlusToSpace( char *str )
{
    while( *str ) {
        if( *str == '+' )
            *str = ' ';
        str++;
    }
}

void SpaceToPlus( char *str )
{
    while( *str ) {
        if( *str == ' ' )
            *str = '+';
        str++;
    }
}

void DecodeURL( char *buf )
{
    char* a = buf;

    PlusToSpace( buf );
    buf = a;

    DecodeENCODEDURL( buf );
}
/********************************
***** end of decoding functions ******
********************************/

/* functions to display an error message that is currently occured */
void DisplayErrorMessage( int errno, char *errstr, bool print )
{
    if( print ) {
        printf( "Content-type: text/html\n" );
        printf( "\n" );

        printf( "<html><head><title>Error (%d)</title><meta http-equiv=\"cache-control\" content=\"no-cache\"/></head>", errno );
        printf( "<body>" );
    }
#ifdef __TRACE__
    if( errno == _TRACE )
        printf( "<br><font color=red>%s  ***DEBUG***</font><br>", errstr);
    else
#endif
    {
        printf( "<br>%s (%s)<br>", errstr, gs_function );
        printf( "<br><a href=%s>Back</a>", gs_backurl );
    }

    if( print )
        printf("</body></html>");

}

void DEBUG_DisplayError( int errno, bool print )
{
    char *errstr;

#ifdef __TRACE__
    if( errno == _TRACE )
        errstr = (char*)gs_trace;
    else
#endif
    {
        errstr = (char*)gs_errormsg[ errno ];
        memset( gs_function, 0x00, _FUNCTION_MAX_STRING );
        sprintf( (char *)gs_function, "%s : %d %s()", gv_file, gv_line, gv_function );
    }

    DisplayErrorMessage( errno, errstr, print );
}

#ifdef __TRACE__
void TRACE( const char *format,... )
{
    va_list argptr;

    gv_istrace = TRUE;
    memset( gs_trace, 0x00, _TRACE_MAX_STRING );

    va_start( argptr, format);
    vsprintf( gs_trace, format, argptr );
    va_end( argptr );

    if( strstr( (char*)gs_trace, "\n" ) )
        DEBUG_DisplayError( _TRACE, TRUE );
    else
        DEBUG_DisplayError( _TRACE, FALSE );
}
#endif

/* return a pointer of contents */
char *GetContent( int *n )
{
    char *ContentBuffer;
    int i;

    *n = atoi( getenv( "CONTENT_LENGTH" ) );
    ContentBuffer = (char *)malloc( *n+1 );
    for ( i=0; i<*n; i++ )
        ContentBuffer[ i ] = getchar();

    ContentBuffer[ i ] = 0;

    return( ContentBuffer );
}

/* return a pointer of the name for contents  */
char *GetName( char *ContentBuffer )
{
    char *NameBuffer;
    char *NameBuffer_backup;

    NameBuffer = strstr( ContentBuffer, _NAME );
    NameBuffer_backup = ++NameBuffer;

    while( (*NameBuffer++) != _NAMEEND )
        ;

    *(NameBuffer - 1) = NULL;

    DecodeURL( NameBuffer_backup );
    //SpaceToPlus( NameBuffer_backup );

    return( NameBuffer_backup );
}

#ifdef __PASSWORD_ENCRYPTION__
void Encrypt( char *dest, const char *src )
{
    int i, j;
    int n = strlen( src );

    memset( dest, 0x00, _ENCRYPT_MAX_STRING );

    dest[ 0 ] = n;

    for( i=0, j=1; i<n; i++, j++ ) {
        dest[ j++ ] = src[ i ] ^ publickey[ i ];
        dest[ j ] = komma;
    }

    dest[ 0 ] = j;
}

void Decrypt( char *dest, const char *src )
{
    int i, j;
    int n = src[ 0 ];

    memset( dest, 0x00, _PASSWORD_MAX_STRING );

    for( i=0, j=1; j<n; i++, j++ ) {
        dest[ i ] = src[ j ] ^ publickey[ i ];

        if( src[ j+1 ] == komma )
            ++j;
    }
}
#endif

#ifdef __PASSWORD__
/* return a pointer of the password for contents */
char *GetPassword( char *ContentBuffer )
{
    char *PasswordBuffer;
    char *PasswordBuffer_backup;

    PasswordBuffer = strstr( ContentBuffer, _PASSWD );
    PasswordBuffer += _PASSWDEND;
    PasswordBuffer_backup = PasswordBuffer;

    while( (*PasswordBuffer++) != _NAMEEND )
        ;

    *(PasswordBuffer - 1) = NULL;

    if( !*PasswordBuffer_backup ) {
        PasswordBuffer_backup = (char *)gs_nullpassword;
    } else
        DecodeURL( PasswordBuffer_backup );

    return( PasswordBuffer_backup );
}
#endif

/* return a pointer of the subject for contents */
char *GetSubject( char *ContentBuffer )
{
    char *SubjectBuffer;
    char *SubjectBuffer_backup;

    SubjectBuffer = strstr( ContentBuffer, _SUBJECT );
    SubjectBuffer += _SUBJECTEND;
    SubjectBuffer_backup = SubjectBuffer;

    while( (*SubjectBuffer++) != _NAMEEND )
        ;

    *(SubjectBuffer - 1) = NULL;

    DecodeURL( SubjectBuffer_backup );
    //SpaceToPlus( SubjectBuffer_backup );

    return( SubjectBuffer_backup );
}

/* return a pointer of the message for contents */
char *GetMessage( char *ContentBuffer )
{
    char *MessageBuffer;

    MessageBuffer = strstr( ContentBuffer, _MESSAGE );
    MessageBuffer += _MESSAGEEND;

    DecodeURL( MessageBuffer );
    //SpaceToPlus( MessageBuffer );

    return( MessageBuffer );
}

int GetListNumberForMove( char *ContentBuffer )
{
    char *ListBuffer;
    int listno;

    ListBuffer = strstr( ContentBuffer, _NAME );
    listno = atoi( ++ListBuffer );

    return( listno );
}

char *GetPasswordToDelete( char *ContentBuffer )
{
    char *passwordbuffer;

    passwordbuffer = strstr( ContentBuffer, _NAME );

    return( ++passwordbuffer );
}

void ReleaseContent( void *buf )
{
    if( buf ) {
        free( buf );
        buf = NULL;
    }
}

/* function that converts an integer to a string */
void iToa( int iint, char *cstring, int radix, int length )
{
    char *tmpBuffer;
    int count = 0;
    int i = 0;
    int remainder = 0;

    if( !iint ) {
        cstring[ 0 ] = '0';
        cstring[ 1 ] = NULL;
    }
    else {
        tmpBuffer = (char*)calloc( length, sizeof( char ) );

        while( iint ) {
            remainder = iint % radix;
            iint /= radix;

            switch( remainder ) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                    tmpBuffer[ count++ ] = remainder + 48;
                    break;
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                    tmpBuffer[ count++ ] = remainder + 65 - 10;
                    break;
                default:
                    DisplayError( _DEFAULT );
                    exit( 0 );
            }
        }

        for( i=0; i<count; i++ )
            cstring[ i ] = tmpBuffer[ count - 1 - i ];
        cstring[ count ] = NULL;

        ReleaseContent( tmpBuffer );
    }
}

#ifdef __UA_CHECK__
/* a function checking validityof UAProfile */
bool CheckUAProfile( void )
{
    char *uaprofile;
    int i;

    uaprofile = getenv( "HTTP_USER_AGENT" );

    for( i=0; i<( sizeof( gv_ualist ) / sizeof( *gv_ualist ) ); i++ ) {
        if( strstr( uaprofile, (char*)gv_ualist[i] ) )
            return TRUE;
    }

    return FALSE;
}
/* end of checking UAProfile */
#endif

void DisplayMessageAndRedirect( char *message, char *url )
{
    printf( "Content-type: text/html\n" );
    printf( "\n" );

    printf( "<html><head><title>Loading...</title><meta http-equiv=\"cache-control\" content=\"no-cache\"/>" );

    if( strlen( url ) && !gv_istrace )
        printf( "<meta http-equiv=\"REFRESH\" content=\"%d;URL=%s\">", _TIME_TO_REDIRECT, url );
    else
        gv_istrace = FALSE;

    printf( "</head><body>" );

    if( strlen( message ) )
        printf( "%s<br>", message );

    printf("</body></html>");
}

int GetNewRefNumber( void )
{
    int fileno;
    FILE *fp;

    if( !( fp = fopen( "global.dat", "r" ) ) ) {
        DisplayError( _FAILTOOPENFILEGLOBAL );
        exit( 0 );
    }

    fscanf( fp, "%d", &fileno );
    fclose( fp );

    return( fileno );
}

void SetNewRefNumber( int refno )
{
    FILE *fp;

    if( !( fp = fopen( "global.dat", "r+" ) ) ) {
        DisplayError( _FAILTOOPENFILEGLOBAL );
        exit( 0 );
    }

    fprintf( fp, "%d", refno );
    fclose( fp );
}

#ifdef __DATE_FOR_LIST__
void GetOSDate( char *osdate )
{
    time_t tmptime;
    struct tm *today;
    char tmpbuffer[ _OSDATE_MAX_STRING ];

    time( &tmptime );
    today = localtime( &tmptime );
    strftime( tmpbuffer, _OSDATE_MAX_STRING, "%b %d %Y", today );

    strcpy( osdate, tmpbuffer );

    SpaceToPlus( osdate );
}
#endif

#ifdef __TIMEANDDATE_FOR_ARTICLE__
void GetUnixDate( char *unixdate )
{
    time_t tmptime;
    char *tmpbuffer;

    time( &tmptime );
    tmpbuffer = ctime( &tmptime );

    strcpy( unixdate, tmpbuffer );

    SpaceToPlus( unixdate );
}
#endif

#ifdef __SAVE_IP_ADDRESS__
void GetIPAddress( char *ip )
{
    char *tmpIP;

    tmpIP = getenv( "REMOTE_ADDR" );
    strcpy( ip, tmpIP );
}
#endif

#ifdef __SAVE_UAPROFILE__
void GetUAProfile( char *ua )
{
    char *tmpUA;
    int i;

    tmpUA = getenv( "HTTP_USER_AGENT" );

    for( i=0; i<strlen( tmpUA ); i++ ) {
        if( tmpUA[ i ] == ' ' || tmpUA[ i ] == NULL )
            break;
    }

    if( i >= _UA_MAX_STRING )
        i = _UA_MAX_STRING - 1;

    strncpy( ua, tmpUA, i );
    ua[ i ] = NULL;
}
#endif

#ifdef __DELETE_NOT_SELECTABLE__
void SetValid( bool *valid )
{
    *valid = 1;
}
#endif

#ifdef __REPLY__
void SetReplyNo( int *replyno )
{
    *replyno = 0;
}
#endif

void GetFileName( int refno, char *filename )
{
    char buffer[ _FILENAME_MAX_STRING ];
    int i;

    memset( (char*)buffer, 0x00, _FILENAME_MAX_STRING );
    iToa( refno, (char*)buffer, 10, _FILENAME_MAX_STRING ); // argv[ 3 ] = radix, argv[ 4 ] = max length

    for( i=0; i<strlen( buffer ); i++ )
        filename[ _FILENAME_MAX_STRING + _DIRLENGTH - 1 - strlen( buffer ) + i ] = buffer[ i ];
}

/* Convert CR(0x0D), LF(0x0A) to <br> */
void Convert_String( char *to, const char *from )
{
    int i =0, j =0;

    while( from[ i ] && j < _INPUT_MESSAGE_MAX_STRING ) {
        if( from[ i ] == 0x0D ) {
            if( from[ ++i ] == 0x0A ) {
                to[ j ] = '<';
                to[ ++j ] = 'b';
                to[ ++j ] = 'r';
                to[ ++j ] = '>';
            }
            else
                to[ j ] = ' ';
        } else
            to[ j ] = from[ i ];

        i++;
        j++;
    }

    to[ j ] = NULL;
}

void Set_DIRName( char *dirname, int dirtype )
{
    int i;

    if( dirtype < 0 || dirtype >= _DIRSIZE ) {
        DisplayError( _DEFAULT );
        exit( 0 );
    }

    memset( dirname, 0x00, _FILENAME_MAX_STRING + _DIRLENGTH );

    for( i=0; i<_FILENAME_MAX_STRING + _DIRLENGTH; i++ )
        dirname[ i ] = gs_defaultdb[ dirtype ][ i ];
}

#ifdef __IMAGE_UPLOAD__
bool ParseMultipartImage( char *ContentBuffer, int *n )
{
    char *ptr;
    char *ptr2;
    char search1[] = _IMAGESUPPORTED;
    int search_length;
    int sub, acom;

    search_length =  strlen( _IMAGESUPPORTED ) + 4; // 4 = 0x0D, 0X0A, 0x0D, 0x0A
    ptr2 = ContentBuffer;

    if( (ptr = strstr( ContentBuffer, (char *)search1 )) == NULL )
        return FALSE;

    ContentBuffer = &ptr[ search_length ];

    acom = *n;
    sub = ContentBuffer - ptr2;
    acom -= sub;
    *n = acom;

    return TRUE;
}

bool SaveImage( char *ContentBuffer, int refno, int n )
{
    char filename[ _FILENAME_MAX_STRING + _DIRLENGTH ];
    int i;
    int n2;
    FILE *fp;

    n2 = n;

    if( !ParseMultipartImage( ContentBuffer, &n ) ) {
        DisplayError( _IMAGENOTSUPPORTED );
        exit( 0 );
    }

    n2 -= n;

    Set_DIRName( (char*)filename, _IMAGE );
    GetFileName( refno, filename );

    if( !(fp = fopen( strcat( filename, ".jpg" ), "w" ) ) )
        return FALSE;

    for( i=0; i<n; i++ )
        fprintf( fp, "%c", ContentBuffer[ i + n2 ] );

    return TRUE;
}
#endif

bool WritetoFile( MESSAGE *writeBuffer )
{
    char filename[ _FILENAME_MAX_STRING + _DIRLENGTH ];
    FILE *fp;

#if defined(  __DELETE_NOT_SELECTABLE__ ) || defined( __REPLY__ )
    Set_DIRName( (char*)filename, _LOG );
    GetFileName( writeBuffer->refno, filename );

    if( !( fp = fopen( filename, "w" ) ) ) {
        DisplayError( _FAILTOOPENFILEDB );
        return FALSE;
    }
#ifdef __DELETE_NOT_SELECTABLE__
    fprintf( fp, "%d\n", writeBuffer->valid ); // valid
#endif
#ifdef __REPLY__
    fprintf( fp, "%d\n", writeBuffer->replyno ); // replyno
#endif
    fclose( fp );
#endif // end of defined(  __DELETE_NOT_SELECTABLE__ ) || defined( __REPLY__ )

    Set_DIRName( (char*)filename, _DB );
    GetFileName( writeBuffer->refno, filename );

    if( !( fp = fopen( filename, "w" ) ) ) {
        DisplayError( _FAILTOOPENFILEDB );
        return FALSE;
    }

    fprintf( fp, "%d\n", writeBuffer->refno ); // refno
#ifdef __DATE_FOR_LIST__
    fprintf( fp, "%s\n", writeBuffer->osdate ); // osdate
#endif
#ifdef __TIMEANDDATE_FOR_ARTICLE__
    fprintf( fp, "%s", writeBuffer->unixdate ); // unixdate ( no \n )
#endif
    SpaceToPlus( writeBuffer->name );
    fprintf( fp, "%s\n", writeBuffer->name ); // name
    SpaceToPlus( writeBuffer->subject );
    fprintf( fp, "%s\n", writeBuffer->subject ); // subject
#ifdef __PASSWORD__
    SpaceToPlus( writeBuffer->password );
#ifdef __PASSWORD_ENCRYPTION__
    {
        int i;
        Encrypt( (char *)gv_passwordencrypt, (char *)writeBuffer->password );

        for( i=0; i<gv_passwordencrypt[ 0 ]; i++ )
            fprintf( fp, "%c", gv_passwordencrypt[ i ] );
        fprintf( fp, "%c\n", NULL );
    }
#else
    fprintf( fp, "%s\n", writeBuffer->password ); // password
#endif
#endif
    SpaceToPlus( writeBuffer->message );
    fprintf( fp, "%s\n", writeBuffer->message ); // message
#ifdef __SAVE_IP_ADDRESS__
    fprintf( fp, "%s\n", writeBuffer->ip ); // ip
#endif
#ifdef __SAVE_UAPROFILE__
    fprintf( fp, "%s\n", writeBuffer->ua ); // uaprofile
#endif

    fclose( fp );

    return( TRUE );
}

bool WriteLOG( int msgno, bool valid, int replyno )
{
    char filename[ _FILENAME_MAX_STRING + _DIRLENGTH ];
    FILE *fp;

#if defined(  __DELETE_NOT_SELECTABLE__ ) || defined( __REPLY__ )
    Set_DIRName( (char*)filename, _LOG );
    GetFileName( msgno, filename );

    if( !( fp = fopen( filename, "w" ) ) ) {
        DisplayError( _FAILTOOPENFILEDB );
        exit( 0 );
    }
#ifdef __DELETE_NOT_SELECTABLE__
    fprintf( fp, "%d\n", valid ); // valid
#endif
#ifdef __REPLY__
    fprintf( fp, "%d\n", replyno ); // replyno
#endif
    fclose( fp );
#endif // end of defined(  __DELETE_NOT_SELECTABLE__ ) || defined( __REPLY__ )

    return TRUE;
}

#ifdef __REPLY__
int GetReplyno( int msgno )
{
    char filename[ _FILENAME_MAX_STRING + _DIRLENGTH ];
    int finput;
    FILE *fp;

    Set_DIRName( (char*)filename, _LOG );
    GetFileName( msgno, filename );

   if( !( fp = fopen( filename, "r" ) ) ) {
        DisplayError( _FAILTOOPENFILEDB );
        return FALSE;
    }
#ifdef __DELETE_NOT_SELECTABLE__
    fscanf( fp, "%d", &finput ); // valid
    if( !finput ) {
        DisplayError( _MESSAGEWASDELETED );
        fclose( fp );
        return FALSE;
    }
#endif
    fscanf( fp, "%d", &finput );

    fclose( fp );

    return finput;
}

void GetReply( int msgno, int replyno, REPLY *replybuffer )
{
    int i;
    char filename[ _FILENAME_MAX_STRING + _DIRLENGTH ];
    FILE *fp;

    Set_DIRName( (char*)filename, _REPLY );
    GetFileName( msgno, filename );

    if( !( fp = fopen( filename, "r" ) ) ) {
        DisplayError( _FAILTOOPENFILEDB );
        exit( 0 );
    }

    for( i=0; i<replyno; i++ ) {
        fscanf( fp, "%s", replybuffer[i].name ); // name
        PlusToSpace( (char*)replybuffer[i].name );
#ifdef __DATE_FOR_LIST__
        fscanf( fp, "%s", replybuffer[i].osdate ); // osdate
        PlusToSpace( (char*)replybuffer[i].osdate );
#endif
        fscanf( fp, "%s", replybuffer[i].comment ); // comment
        PlusToSpace( (char*)replybuffer[i].comment );
    }

    fclose( fp );
}

bool WriteReplytoFile( REPLY *replybuffer )
{
    char filename[ _FILENAME_MAX_STRING + _DIRLENGTH ];
    FILE *fp;

/*** Writing Reply ***/
    Set_DIRName( (char*)filename, _REPLY );
    GetFileName( replybuffer->msgno, filename );

    if( !( fp = fopen( filename, "a+" ) ) ) {
        DisplayError( _FAILTOOPENFILEDB );
        exit( 0 );
    }

    SpaceToPlus( replybuffer->name );
    fprintf( fp, "%s\n", replybuffer->name ); // name
#ifdef __DATE_FOR_LIST__
    fprintf( fp, "%s\n", replybuffer->osdate ); // osdate
#endif
    SpaceToPlus( replybuffer->comment );
    fprintf( fp, "%s\n", replybuffer->comment ); // name

    fclose( fp );

/*** Writing LOG ***/
    Set_DIRName( (char*)filename, _LOG );
    GetFileName( replybuffer->msgno, filename );

    if( !( fp = fopen( filename, "w" ) ) ) {
        DisplayError( _FAILTOOPENFILEDB );
        exit( 0 );
    }
#ifdef __DELETE_NOT_SELECTABLE__
    fprintf( fp, "%d\n", replybuffer->valid ); // valid
#endif
    fprintf( fp, "%d\n", ++(replybuffer->replyno) ); // replyno

    fclose( fp );

    return TRUE;
}
#endif

int GetMaxPage( int list, int page )
{
    int maxpage;

    if( list % _LIST_SIZE )
        maxpage = list / _LIST_SIZE + 1;
    else
        maxpage = list / _LIST_SIZE;

    if( page > maxpage && maxpage ) {
        DisplayError( _NOMOREARTICLES );
        exit( 0 );
    }

    return( maxpage );
}

int GetListNumber( int *list, int page )
{
    int listsize;

    if( page * _LIST_SIZE <= *list ) {
        *list -= ( (page - 1) * _LIST_SIZE );
        listsize = _LIST_SIZE;
    }
    else if( page * _LIST_SIZE > *list ) {
        listsize = *list - ( (page - 1) * _LIST_SIZE );
        *list -= ( (page - 1) * _LIST_SIZE );
    }

    return( listsize );
}

int GetListNumberForBackFromArticle( int msgno, int maxmsgno )
{
    int list;

    list = 1 + ( maxmsgno - msgno ) / _LIST_SIZE;

    return list;
}

bool SetFirstPage( int page, int maxpage )
{
    if( page <= maxpage || !maxpage) {

        if( page == 1 )
            return( TRUE );
        else
            return( FALSE );
    }
    else {
        DisplayError( _NOMOREARTICLES );
        exit( 0 );
    }
}

bool SetLastPage( int page, int maxpage )
{
    if( page == maxpage )
        return( TRUE );
    else if( page < maxpage || !maxpage )
        return( FALSE );
    else {
        DisplayError( _NOMOREARTICLES );
        exit( 0 );
    }
}

void MakeList( int list, int listsize, MESSAGE *readbuf )
{
    int i, j;
    char filename[ _FILENAME_MAX_STRING + _DIRLENGTH ];
    FILE *fp;
#ifdef __REPLY__
    char replyno[ _NUMBER_MAX_REPLY ];
#endif

    for( i=0, j=0; j<listsize; i++, j++ ) {
#if defined( __DELETE_NOT_SELECTABLE__ ) || defined( __REPLY__ )
        Set_DIRName( (char*)filename, _LOG );
        GetFileName( (list - i), filename );

       if( !( fp = fopen( filename, "r" ) ) ) {
            ReleaseContent( readbuf );
            DisplayError( _FAILTOOPENFILEDB );
            exit( 0 );
        }
#ifdef __DELETE_NOT_SELECTABLE__
        fscanf( fp, "%d", &(readbuf[ j ].valid) ); // valid
#endif
#ifdef __REPLY__
        fscanf( fp, "%d", &(readbuf[ j ].replyno) ); // replyno
#endif
        fclose( fp );
#endif // end of defined( __DELETE_NOT_SELECTABLE__ ) || defined( __REPLY__ )

        Set_DIRName( (char*)filename, _DB );
        GetFileName( (list - i), filename );

       if( !( fp = fopen( filename, "r" ) ) ) {
            ReleaseContent( readbuf );
            DisplayError( _FAILTOOPENFILEDB );
            exit( 0 );
        }

        fscanf( fp, "%d", &(readbuf[ j ].refno) ); // refno
#ifdef __DATE_FOR_LIST__
        fscanf( fp, "%s", readbuf[ j ].osdate ); // osdate ( Oct 25 2005 )
        PlusToSpace( (char *)readbuf[ j ].osdate );
#endif
#ifdef __TIMEANDDATE_FOR_ARTICLE__
        fscanf( fp, "%s", readbuf[ j ].unixdate ); // unixdate ( Tue Oct 25 07:56:30 2005 )
        PlusToSpace( (char *)readbuf[ j ].unixdate );
#endif
        fscanf( fp, "%s", readbuf[ j ].name ); // name
        PlusToSpace( (char *)readbuf[ j ].name );

        fscanf( fp, "%s", readbuf[ j ].subject ); // subject
        PlusToSpace( (char *)readbuf[ j ].subject );
#ifdef __PASSWORD__
        fscanf( fp, "%s", readbuf[ j ].password ); // password
        PlusToSpace( (char *)readbuf[ j ].password );
#endif
        fscanf( fp, "%s", readbuf[ j ].message ); // message
        PlusToSpace( (char *)readbuf[ j ].message );

#ifdef __SAVE_IP_ADDRESS__
        fscanf( fp, "%s", readbuf[ j ].ip ); // ip
#endif
#ifdef __SAVE_UAPROFILE__
        fscanf( fp, "%s", readbuf[ j ].ua ); // uaprofile
#endif

        fclose( fp );
    }
}

void MakeArticle( int list, MESSAGE *readbuf )
{
    int i, j;
    char filename[ _FILENAME_MAX_STRING + _DIRLENGTH ];
    FILE *fp;

#if defined( __DELETE_NOT_SELECTABLE__ ) || defined( __REPLY__ )
    Set_DIRName( (char*)filename, _LOG );
    GetFileName( list, filename );

   if( !( fp = fopen( filename, "r" ) ) ) {
        ReleaseContent( readbuf );
        DisplayError( _FAILTOOPENFILEDB );
        exit( 0 );
    }
#ifdef __DELETE_NOT_SELECTABLE__
    fscanf( fp, "%d", &(readbuf->valid) ); // valid
    if( !readbuf->valid ) {
        DisplayError( _MESSAGEWASDELETED );
        ReleaseContent( readbuf );
        fclose( fp );
        exit( 0 );
    }
#endif
#ifdef __REPLY__
        fscanf( fp, "%d", &(readbuf->replyno) );
#endif
    fclose( fp );
#endif // end of defined( __DELETE_NOT_SELECTABLE__ ) || defined( __REPLY__ )

#ifdef __IMAGE_UPLOAD__
        Set_DIRName( (char*)filename, _IMAGE );
        GetFileName( list, filename );

        if( !(fp = fopen( strcat( filename, ".jpg" ), "r" ) ) )
            readbuf->file = FALSE;
        else {
            readbuf->file = TRUE;
            fclose( fp );
        }
#endif

    Set_DIRName( (char*)filename, _DB );
    GetFileName( list, filename );

    if( !( fp = fopen( filename, "r" ) ) ) {
        ReleaseContent( readbuf );
        DisplayError( _FAILTOOPENFILEDB );
        exit( 0 );
    }

    fscanf( fp, "%d", &(readbuf->refno) ); // refno
#ifdef __DATE_FOR_LIST__
    fscanf( fp, "%s", readbuf->osdate ); // osdate ( Oct 25 2005 )
    PlusToSpace( (char *)readbuf->osdate );
#endif
#ifdef __TIMEANDDATE_FOR_ARTICLE__
    fscanf( fp, "%s", readbuf->unixdate ); // unixdate ( Tue Oct 25 07:56:30 2005 )
    PlusToSpace( (char *)readbuf->unixdate );
#endif
    fscanf( fp, "%s", readbuf->name ); // name
    PlusToSpace( (char *)readbuf->name );

    fscanf( fp, "%s", readbuf->subject ); // subject
    PlusToSpace( (char *)readbuf->subject );
#ifdef __PASSWORD__
#ifdef __PASSWORD_ENCRYPTION__
    {
        int i, n;
        fscanf( fp, "%d", &n );

        memset( gv_passwordencrypt, NULL, _ENCRYPT_MAX_STRING );
        gv_passwordencrypt[ 0 ] = n;

        for( i=0; i<n; i++ )
            fscanf( fp, "%c", (char *)gv_passwordencrypt );

        Decrypt( (char *)readbuf->password, (char *)gv_passwordencrypt );
    }
#else
    fscanf( fp, "%s", readbuf->password ); // password
#endif
    if( !strcmp( readbuf->password, gs_nullpassword ) )
        strcpy( (char*)readbuf->password, "" );
    else
        PlusToSpace( (char *)readbuf->password );
#endif
    fscanf( fp, "%s", readbuf->message ); // message
    PlusToSpace( (char *)readbuf->message );
#ifdef __SAVE_IP_ADDRESS__
    fscanf( fp, "%s", readbuf->ip ); // ip
#endif
#ifdef __SAVE_UAPROFILE__
    fscanf( fp, "%s", readbuf->ua ); // uaprofile
#endif

    fclose( fp );
}

void DisplayList( int list, int totallist, int page, int maxpage, int listsize, bool firstpage, bool lastpage )
{
    int i, j;
    MESSAGE *readbuf;

    readbuf = (MESSAGE *)malloc( listsize * sizeof( MESSAGE ) );

    MakeList( list, listsize, readbuf );

    printf( "Content-type: text/html\n" );
    printf( "\n" );
    printf( "<html><head><meta http-equiv=\"cache-control\" content=\"no-cache\"/>" );
    printf( "<title>%s</title></head><body>", gs_title );
    printf( "<center><font color=gray>%d/%d (total %d)</font></center>", page, maxpage, totallist );
    printf( "<hr width=90%c>", 37 ); // 37 means '%'

    for( i=0; i<listsize; i++ ) {
        printf( "<p mode=nowrap>%d. ", readbuf[ i ].refno );
#ifdef __DELETE_NOT_SELECTABLE__
        if( readbuf[ i ].valid )
#endif
        {
            printf( "<a href=view.cgi?%d>", readbuf[ i ].refno );
        }
        printf( "%s", readbuf[ i ].subject );
#ifdef __REPLY__
        printf( " [%d]", readbuf[ i ].replyno );
#endif
#ifdef __DELETE_NOT_SELECTABLE__
        if( readbuf[ i ].valid )
#endif
        {
            printf( "</a>" );
        }
#ifdef __DATE_FOR_LIST__
        printf( " (%s)</p>", readbuf[ i ].osdate );
#else
        printf( "</p>" );
#endif
    }

    printf( "<br><p align=center>" );
    if( page > 1 )
        printf( "<a href=board.cgi?%d><< prev</a>", page-1 );
    else
        printf( "<< prev" );
    if( page >= maxpage )
        printf( "&nbsp;&nbsp;next >>" );
    else
        printf( "&nbsp;&nbsp;<a href=board.cgi?%d>next >></a>", page+1 );
    printf( "</p><hr width=90%c>", 37 ); // 37 means %

    printf( "<i><a href=form.cgi>Write a new..</a><br>" );
#ifdef __SEARCH__
    printf( "<a href=search_form.cgi>Search</a><br>" );
#endif
    printf( "<a href=move_form.cgi>Move into another..</a></i><br>" );
#ifdef __CREDITS__
#ifdef __FULL_RESET__
    printf( "<br><p align=right><a href=reset_form.cgi><i>%s</i></a> &nbsp; </p>", gs_credits );
#else
    printf( "<br><p align=right><i>%s &nbsp;</i> </p>", gs_credits );
#endif
#endif
    printf( "</body></html>" );

    ReleaseContent( readbuf );
}

void DisplayArticle( int msgno )
{
#ifdef __IMAGE_UPLOAD__
    char filename[ _FILENAME_MAX_STRING + _DIRLENGTH ];
#endif
    int maxmsgno;
    int listno;
    MESSAGE *readbuf;

    readbuf = (MESSAGE *)malloc( 1 * sizeof( MESSAGE ) );

    MakeArticle( msgno, readbuf );

    maxmsgno = GetNewRefNumber();
    listno = GetListNumberForBackFromArticle( msgno, maxmsgno );

    printf( "Content-type: text/html\n" );
    printf( "\n" );
    printf( "<html><head><meta http-equiv=\"cache-control\" content=\"no-cache\"/>" );
    printf( "<title>%d. %s</title></head><body>", readbuf->refno, readbuf->subject );
    printf( "<b>%s</b>", readbuf->name );
#ifdef __TIMEANDDATE_FOR_ARTICLE__
    printf( " (<i>%s</i>)", readbuf->unixdate );
#endif
    printf( "<br>" );
    printf( "<hr width=90%c>", 37 ); // 37 means %
#ifdef __IMAGE_UPLOAD__
    if( readbuf->file ) {
        Set_DIRName( (char*)filename, _IMAGE );
        GetFileName( msgno, filename );
        printf( "<center><a href=%s%s.jpg><img src=%s%s.jpg></a></center>", _BASEURL, filename, _BASEURL, filename );
    } else
        printf( "<center><img src=%sresources/noimage.jpg></center>", _BASEURL );
#endif
    printf( "<br>%s<br><p align=right><br>", readbuf->message );
#ifdef __SAVE_IP_ADDRESS__
    printf( "IP: %s &nbsp;<br>", readbuf->ip );
#endif
#ifdef __SAVE_UAPROFILE__
    printf( "UA: %s &nbsp;", readbuf->ua );
#endif
    printf( "</p>" );

#ifdef __REPLY__
    if( readbuf->replyno > 0 ) {

        int i;
        REPLY *replybuf;

        replybuf = (REPLY *)malloc( readbuf->replyno * sizeof( REPLY ) );

        GetReply( msgno, GetReplyno( msgno ), replybuf );

        for( i = readbuf->replyno - 1; i>=0; i-- ) {
            printf( "<font size=2><b>%s</b>", replybuf[ i ].name );
#ifdef __DATE_FOR_LIST__
            printf( "&nbsp;(<i>%s</i>)", replybuf[ i ].osdate );
#endif
            printf( "<br>&nbsp;%s</font><br>", replybuf[ i ].comment );
        }

        ReleaseContent( replybuf );
    }
#endif
    printf( "<br><p align=center>" );
    if( msgno > 1 )
        printf( "<a href=view.cgi?%d><< prev</a>", msgno-1 );
    else
        printf( "<< prev" );
    if( msgno >= maxmsgno )
        printf( "&nbsp;&nbsp;next >>" );
    else
        printf( "&nbsp;&nbsp;<a href=view.cgi?%d>next >></a>", msgno+1 );
    printf( "</p><hr width=90%c>", 37 ); // 37 means %
    printf( "<a href=board.cgi?%d>Back to List</a><br>", listno );
#ifdef __REPLY__
    printf( "<a href=reply_form.cgi?%d>Reply</a><br>", readbuf->refno );
#endif
#ifdef __DELETE_NOT_SELECTABLE__
    printf( "<a href=delete_form.cgi?%d>Delete this..</a><br>", readbuf->refno );
#endif
    printf( "</body></html>" );

    ReleaseContent( readbuf );
}

#ifdef __DELETE_NOT_SELECTABLE__
int DeleteArticle( int msgno )
{
    MESSAGE *readbuf;

    readbuf = (MESSAGE *)malloc( 1 * sizeof( MESSAGE ) );

    MakeArticle( msgno, readbuf );

#ifdef __MASTER_PASSWORD__
    if( !strcmp( (char*)gs_masterpassword, (char*)gs_password ) ) {
        readbuf->valid = FALSE;
        WriteLOG( msgno, readbuf->valid, readbuf->replyno );
        ReleaseContent( readbuf );
        return TRUE;
    }
#endif
    if( !strcmp( (char*)readbuf->password, "" ) ) {
        ReleaseContent( readbuf );
        return _NOPASSWORD;
    }
    else if( strcmp( (char*)readbuf->password, (char*)gs_password ) ) {
        ReleaseContent( readbuf );
        return _PASSWORDMISSMATCH;
    }
    else if( !strcmp( (char*)readbuf->password, (char*)gs_password ) ) {
        readbuf->valid = FALSE;
        WriteLOG( msgno, readbuf->valid, readbuf->replyno );
        ReleaseContent( readbuf );
        return TRUE;
    } else {
        ReleaseContent( readbuf );
        return FALSE;
    }
}
#endif

#ifdef __FULL_RESET__
void ResetGlobalData()
{
    FILE *fp;

    fp = fopen( "global.dat", "w" );

    fprintf( fp, "%d", 0 );

    fclose( fp );
}

bool FullReset()
{
    system( "rm db/*" );
    system( "rm lg/*" );
#ifdef __REPLY__
    system( "rm rp/*" );
#endif
#ifdef __IMAGE_UPLOAD__
    system( "rm up/*" );
#endif
    ResetGlobalData();

    return TRUE;
}
#endif

