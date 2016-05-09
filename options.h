/**
FILE NAME : options.h
USE : some definitions as options
INCLUDE : NONE
COMPILED NAME : NONE
AUTHOR AND DATE : Gen, OCT 2005
**/

/** compile options **********************************************************/
//#define __TRACE__
#define __CHECK_NULL_NAMESUBJECTMESSAGE__
#define __DELETE_NOT_SELECTABLE__
#ifdef __DELETE_NOT_SELECTABLE__
#define __PASSWORD__
#ifdef __PASSWORD__
#define __MASTER_PASSWORD__
//#define __PASSWORD_ENCRYPTION__
#endif
#endif
//#define __UA_CHECK__
#define __DATE_FOR_LIST__
#define __TIMEANDDATE_FOR_ARTICLE__
#define __IMAGE_UPLOAD__
#define __SAVE_IP_ADDRESS__
#define __SAVE_UAPROFILE__
#define __REPLY__
//#define __SEARCH__
#ifdef __SEARCH__
#define __SEARCH_NAME__
#define __SEARH_SUBJECT__
#define __SEARCH_MESSAGE__
//#define __SEARCH_COMMENT__
#endif
//#define __EDIT_MESSAGE__
#define __CREDITS__
#if defined( __CREDITS__ ) && defined( __MASTER_PASSWORD__ )
#define __FULL_RESET__
#endif


/** PRE-Required definitions ***************************************************/
#ifdef NULL
#undef NULL
#endif
#define NULL '\0'

#ifdef bool
#undef bool
#endif
#define bool unsigned char

#ifdef TRUE
#undef TRUE
#endif
#define TRUE 1

#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0


/** some definitions ************************************************************/
#define _NAME "="
#define _NAMEEND '&'
#ifdef __PASSWORD__
#define _PASSWD "&gencodeurpasswd="
#define _PASSWDEND 17 // strlen( "&gencodeurpasswd=" )
#endif
#define _SUBJECT "&gencodeursubject="
#define _SUBJECTEND 18 // strlen( "&gencodeursubject=" )
#define _MESSAGE "&gencodeurmessage="
#define _MESSAGEEND 18 // strlen( "&gencodeurmessage=" )
#ifdef __IMAGE_UPLOAD__
#define _BASEURL "http://gencode3.com/cgi-bin/moblog/"
//#define _BASEURL "http://localhost/execcgi/moblog/"
#define _IMAGESUPPORTED "image/jpeg"
#endif

#define _TRACE_MAX_STRING 400
#define _TRACE_MAX_FILE 20
#define _TRACE_MAX_FUNCTION 32
#define _FUNCTION_MAX_STRING 100
#ifdef __DATE_FOR_LIST__
#define _OSDATE_MAX_STRING 12 // max string of date string for list
#endif
#ifdef __TIMEANDDATE_FOR_ARTICLE__
#define _UNIXDATE_MAX_STRING 28 // max string of date string for article
#endif
#define _NAME_MAX_STRING 16 // max string for name field
#define _SUBJECT_MAX_STRING 16 // max string for subject field
#ifdef __PASSWORD__
#define _PASSWORD_MAX_STRING 16 // max string for password field
#ifdef __PASSWORD_ENCRYPTION__
#define _ENCRYPT_MAX_STRING (_PASSWORD_MAX_STRING+1)*2
#endif
#endif
#define _INPUT_MESSAGE_MAX_STRING 1000 // max string for message field
#define _MESSAGE_TEXTAREA_ROWS_ 3 // rows of message field that is a type of textarea
#ifdef __REPLY__
#define _COMMENT_MAX_STRING 200 // reply msg max string
#endif
#ifdef __SAVE_IP_ADDRESS__
#define _IP_MAX_STRING 16 // max string of ip address
#endif
#ifdef __SAVE_UAPROFILE__
#define _UA_MAX_STRING 30 // max string of uaprofile
#endif
#define _FILENAME_MAX_STRING 11 // length of file name for db + 1(NULL)
#define _DIRLENGTH 3 // strlen( "db/" );
#define _MESSAGE_MAX_STRING 100 // message displayed when done
#define _URL_MAX_STRING 100 // url to redirect
#define _TIME_TO_REDIRECT 1 // time (sec.) to redirect after completing job
#define _URL_MAX_MOVE 20 // url to move
#define _NUMBER_MAX_REPLY 100 // maximum number of replies

typedef enum {
    _DB, // "db/"
#ifdef __REPLY__
    _REPLY, // "rp/"
#endif
    _LOG, // "lg/"
#ifdef __IMAGE_UPLOAD__
    _IMAGE, // "up/"
#endif
    _DIRSIZE // total number of directory
} DIRNAME;

/** some definitions for board.c *************************************************/
#define _LIST_SIZE 5 //

/** some definitions for view.c **************************************************/
//#define _CALLBYLIST 100
//#define _CALLBYARTICLE 200

/** Error Code ***************************************************************/
#ifdef __TRACE__
#define _TRACE 999 // trace
#endif
#define _DEFAULT 0 // blocking
#define _NULLFORNAMESUBJECTMESSAGE 1 // either name, subject or message field is null
#define _UAPROFILEISNOTALLOWED 2 // handset is not allowed to use this service
#define _FAILTOOPENFILEGLOBAL 3 // fail to open file global.dat that contains a total number of messages
#define _FAILTOOPENFILEDB 4 // fail to open an each database file named type of '0000000001'
#define _FAILTOWRITEDB 5 // fail to write database
#define _NOMOREARTICLES 6 // current index of page is out of a page to be.
#define _MESSAGEWASDELETED 7 // the message you selected was already deleted
#define _FAILTODELETEDB 8 // fail to delete an article
#define _NOPASSWORD 9 // there's no password, so this message can not be deleted
#define _PASSWORDMISSMATCH 10 // password missmatch
#define _NULLFORNAMEANDCOMMENT 11 // either name or comment field s null
#define _FAILTOSAVEIMAGE 12 // fail to save uploaded image
#define _IMAGENOTSUPPORTED 13 // the image uploaded is not supported
#define _NOTSUPPORTFULLRESET 14 // when compiled without __FULL_RESET__ option
#define _FULLRESETERROR 15 // fail to reset DBs


/** Strings *******************************************************************/
const char gs_errormsg[][100] = { "unexpected error", // _DEFAULT
    "You should fill out Name, Subject and Message Fields.", // _NULLFORNAMEORCOMMENT
    "Sorry, your handset is not recognized. you can not use this service.", // _UAPROFILEISNOTALLOWED
    "Can not open a file(global.dat), try again later.", // _FAILTOOPENFILEGLOBAL
    "Can not open a database file, try again later.", // _FAILTOOPENFILEDB
    "An error occured while trying to write your message, try again later", // _FAILTOWRITEDB
    "No more articles", // _NOMOREARTICLES
    "The Article you selected has been deleted already.", // _MESSAGEASDELETED
    "An error occured while trying to delete an article, try again later", // _FAILTODELETEDB
    "There is no password for this article, so this message can not be deleted except administrator", // _NOPASSWORD
    "Password missmatch, try again", // _PASSWORDMISSMATCH
    "You should fill out Name and Message Fields.", // _NULLFORNAMEANDCOMMENT
    "An error occured while trying to save the image uploaded", // _FAILTOSAVEIMAGE
    "Your image is not supported", // _IMAGENOTUPPORTED
    "Full Reset is not supported", // _NOTSUPPORTFULLREsET
    "An error occured while full reset, try again later", // _FULLRESETERROR
};

/** Info messages **************************************************************/

#ifdef __TRACE__
char gs_trace[ _TRACE_MAX_STRING ]; // string for trace
#endif
char gv_istrace = FALSE;
int gv_line;
char gv_file[ _TRACE_MAX_FILE ];
char gv_function[ _TRACE_MAX_FUNCTION ];
#ifdef __PASSWORD__
const char gs_nullpassword[] = "NULL"; // NULL password
char gs_password[ _PASSWORD_MAX_STRING + 1 ]; // used for deleting an article
#ifdef __MASTER_PASSWORD__
const char gs_masterpassword[ _PASSWORD_MAX_STRING + 1 ] = "genrulestheworld";
#endif
#ifdef __PASSWORD_ENCRYPTION__
char gv_passwordencrypt[ _ENCRYPT_MAX_STRING ];
char komma= 0x08;
const char publickey[] = "TheFollowingTakesPlaceBetween11PMand12AM.EventsOccurInRealTime.";
#endif
#endif
char gs_function[ _FUNCTION_MAX_STRING ]; // string for function name
char gs_infomessage[ _MESSAGE_MAX_STRING ]; // string for displaying a message after done
char gs_gotourl[ _URL_MAX_STRING ]; // url to redirect
const char gs_defaultdb[ _DIRSIZE ][ _FILENAME_MAX_STRING + _DIRLENGTH ] = {"db/0000000000", "rp/0000000000", "lg/0000000000", "up/0000000000"}; // directory name for database
char gs_backurl[ _URL_MAX_STRING ] = "board.cgi"; // URL to be back after displaying an error message
const char gs_urlwritesuccess[] = "board.cgi"; // url to redirect after saving successfully
#ifdef __IMAGE_UPLOAD__
const char gs_urlupload[] = "upload_form.cgi"; // url to redirect to upload form after saving documents
const char gs_urluploadsuccess[] = "board.cgi"; // url to redirect after saving an image successfully
#endif
const char gs_urldeletesuccess[] = "board.cgi"; // url to redirect after deletng successfully
#ifdef __REPLY__
char gs_urlreplysuccess[ _URL_MAX_MOVE ]; // url to redirect after replying successfully
#endif
char gs_urlmoveinto[ _URL_MAX_MOVE ]; // url to move from user input
const char gs_title[] = "GELOG Mobile"; // title for lists
#ifdef __CREDITS__
const char gs_credits[] = "GENCODE III"; // credits for a list
#endif


/** static variables ************************************************************/
#ifdef __UA_CHECK__
const char gv_ualist[][_UA_MAX_STRING] = { "Opera/8.50", "SAMSUNG-SGH", "FLY", "PSP" };
#endif


/** gloval type of variables *****************************************************/
typedef struct st_MESSAGE {
    int refno; // reference number
#ifdef __DATE_FOR_LIST__
    char osdate[ _OSDATE_MAX_STRING ]; // date format (Nov 17 2005)
#endif
#ifdef __TIMEANDDATE_FOR_ARTICLE__
    char unixdate[ _UNIXDATE_MAX_STRING ]; // time and date format (Thu Nov 17 16:57:44 2005)
#endif
#ifdef __DELETE_NOT_SELECTABLE__
    bool valid; // if FALSE, it was deleted
#endif
    char name[ _NAME_MAX_STRING + 1 ]; // name
    char subject[ _SUBJECT_MAX_STRING + 1 ]; // subject
#ifdef __PASSWORD__
#ifdef __PASSWORD_ENCRYPTION__
    char password[ _ENCRYPT_MAX_STRING + 1 ];
#else
    char password[ _PASSWORD_MAX_STRING + 1 ]; // password
#endif
#endif
    //int messagelength; // strlen( message+1 );
    char message[ _INPUT_MESSAGE_MAX_STRING ]; // message
#ifdef __IMAGE_UPLOAD__
    bool file;
#endif
#ifdef __SAVE_IP_ADDRESS__
    char ip[ _IP_MAX_STRING ]; // IP address
#endif
#ifdef __SAVE_UAPROFILE__
    char ua[ _UA_MAX_STRING ]; // UA Profile
#endif
#ifdef __REPLY__
    int replyno; // total number of replay written
#endif
} MESSAGE;

#ifdef __REPLY__
typedef struct st_REPLY {
    int msgno;
    int replyno;
    char name[ _NAME_MAX_STRING];
#ifdef __DATE_FOR_LIST__
    char osdate[ _OSDATE_MAX_STRING ];
#endif
    char comment[ _COMMENT_MAX_STRING ];
    bool valid;
} REPLY;
#endif

