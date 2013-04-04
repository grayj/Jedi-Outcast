/**
 **	$Header: /roq/libsdsc/arg.c 1     11/02/99 4:38p Zaphod $
 **	Copyright (c) 1989-1995  San Diego Supercomputer Center (SDSC)
 **		a division of General Atomics, San Diego, California, USA
 **
 **	Users and possessors of this source code are hereby granted a
 **	nonexclusive, royalty-free copyright and design patent license to
 **	use this code in individual software.  License is not granted for
 **	commercial resale, in whole or in part, without prior written
 **	permission from SDSC.  This source is provided "AS IS" without express
 **	or implied warranty of any kind.
 **
 **	For further information contact:
 **		E-Mail:		info@sds.sdsc.edu
 **
 **		Surface Mail:	Information Center
 **				San Diego Supercomputer Center
 **				P.O. Box 85608
 **				San Diego, CA  92138-5608
 **				(619) 534-5000
 **/

#define HEADER	"    $Header: /roq/libsdsc/arg.c 1     11/02/99 4:38p Zaphod $"

/**
 **  FILE
 **	arg.c		-  Argument Parsing Package
 **
 **  PROJECT
 **	libsdsc		-  SDSC Utility Library
 **
 **  DESCRIPTION
 **	This file contains source for the argument parsing package.
 **	These functions simplify the parsing of complex command line
 **	argument groupings and partially enforce the SDSC tool conventions.
 **
 **  PUBLIC CONTENTS
 **			d =defined constant
 **			f =function
 **			m =defined macro
 **			t =typedef/struct/union
 **			v =variable
 **			? =other
 **
 **	ArgErrNo	v  error number
 **	ArgNNrr		v  number of error messages
 **	ArgErrList	v  error messages
 **	ArgPError	f  Print error message
 **	ArgQError	f  Query error message
 **
 **	ArgHelp		f  print a short help message
 **	ArgFullHelp	f  print a full-length help message
 **	ArgFeedback	f  dump a feedback from to a file
 **	ArgRegister	f  dump a user-registration form to a file
 **	ArgVersion	f  print version number info to stderr
 **
 **	ArgParse	f  Parse command line arguments
 **
 **	ArgQNOpt	f  Query # of Options on Command-Line
 **	ArgQOpt		f  Query Option on Command-Line
 **
 **	ArgQNOccur	f  query # of occurrences of an option
 **	ArgQNValue	f  query # of values for an occurrence of an option
 **	ArgQOccurOpt	f  query which command-line option this occurrence is
 **	ArgQValue	f  query value for an occurrence of an option
 **
 **  PRIVATE CONTENTS
 **
 **	argValue	t  info on one value of an occurrence of an option
 **	argOccur	t  info on one occurrence of an option
 **	argInfo		t  info on an option and all its occurrences
 **	argKeyword	t  info on a keyword in the hash table
 **
 **	ARGNSTANDARD		d  # of standard options
 **	argStandard		v  standard options
 **	argStandardRegister	v  standard -register option
 **	argStandardFeedback	v  standard -feedback option
 **
 **	argKeywordTable		v  Keyword hash table
 **	argKeywordTableLength	v  Length of the hash table
 **
 **	argHelpOption		t  alphabetical option list entry
 **	argHelpCommandName	v  name of the command (argv[0])
 **	argHelpCommand		v  command struct for help
 **	argHelpNOptionList	v  # of options in list
 **	argHelpOptionList	v  options list for help
 **
 **	argOptionOrder		v  Options in command-line order
 **	argNOpt			v  Number of options on command-line
 **
 **	argPrintForm	f  print a form to a file
 **	argHash		f  hash a keyword to get its hash table index
 **	argFind		f  find a keyword's entry in the hash table
 **	argAdd		f  add an entry to the hash table
 **	argGetValue	f  parse a value out of an argument
 **	argSortCompare	f  Comparison function for qsort
 **
 **	argCacheKeyword	v  keyword of last option queried
 **	argCacheInfo	v  info for last option queried
 **	argCacheNOccur	v  # of last occurrence queried
 **	argCacheOccur	v  occurrence for last occurrence queried
 **	argCacheNValue	v  # of last value queried
 **	argCacheValue	v  value for last value queried
 **
 **  HISTORY
 **	$Log: /roq/libsdsc/arg.c $
* 
* 1     11/02/99 4:38p Zaphod
 **	Revision 1.18  1995/06/29  00:17:39  bduggan
 **	updated copyright
 **
 **	Revision 1.17  1995/06/29  00:15:29  bduggan
 **	added prototype for argPrintForm, added include file for sun
 **
 **	Revision 1.16  1995/05/18  18:41:30  bduggan
 **	Fixed bug w/ gcc on sun's (Can't change char*'s in
 **	protected memory)
 **
 **	Revision 1.15  1995/04/21  17:55:23  bduggan
 **	Added command-specific help capability.
 **	Removed protoypes for standard ansi functions.
 **	Added include files.
 **
 **	Revision 1.15  1995/04/21  17:55:23  bduggan
 **	Added command-specific help capability.
 **	Removed protoypes for standard ansi functions.
 **	Added include files.
 **
 **	Revision 1.14  94/10/03  16:09:41  nadeau
 **	Updated to ANSI C and C++ compatibility.
 **	Removed all use of register keyword.
 **	Minimized use of custom SDSC types (e.g., uchar vs. unsigned char)
 **	Added forward declarations.
 **	Added misc. casts to passify SGI and DEC compilers.
 **	Updated comments.
 **	Updated indenting on some code.
 **	Updated copyright message.
 **	
 **	Revision 1.13  93/07/26  09:24:46  allans
 **	fixed bug in ArgParse initialization loop.  Now all NEXT pointers
 **	are set to NIL.
 **	
 **	Revision 1.12  92/09/10  16:05:51  vle
 **	Added extern declaration of argPrintForm() to make SGI
 **	compiler happy.
 **	
 **	Revision 1.11  92/09/02  13:54:41  vle
 **	Updated copyright notice.
 **	
 **	Revision 1.10  91/10/03  13:11:55  nadeau
 **	Comment updates.
 **	
 **	Revision 1.9  91/09/17  20:04:12  nadeau
 **	Added support for arg_fullusage field of ArgCommand struct.
 **	
 **	Revision 1.8  91/09/17  19:32:46  nadeau
 **	Fixed a few minor bugs relating to splitting off -register
 **	and -feedback into their own standard option tables and
 **	adding -fullhelp.  Added hash table debug print routine.
 **	Added code to copy the help text to a tmp buffer before
 **	modifying it prior to printing.  This was necessary for
 **	the NeXT, which defaults to putting initialized strings
 **	into read-only segments.  When we tried to modify the help
 **	text, we died.
 **	
 **	Revision 1.7  91/09/01  17:12:08  nadeau
 **	Changed help support.  Added support for -fullhelp.  Changed
 **	ArgHelp to display user's usage line and copyright message.`
 **	Added ArgFullHelp.  Changed ArgVersion to display user's
 **	version and copyright messages.  Changed ArgRegister and
 **	ArgFeedback to use the user's form strings.
 **	
 **	Revision 1.6  91/08/25  13:56:38  nadeau
 **	Added support for ArgQOccurOpt.
 **	
 **	Revision 1.5  91/03/11  12:58:56  nadeau
 **	Updated feedback and registration forms.
 **	
 **	Revision 1.4  91/01/11  11:34:16  nadeau
 **	Added #ifndef's around INDEX and RINDEX macros.
 **	
 **	Revision 1.3  91/01/09  16:34:23  nadeau
 **	Added ArgQError() and enhanced handling of multiple occurrence
 **	implied keywords.
 **	
 **	Revision 1.2  90/05/16  12:19:09  nadeau
 **	Removed bogus extra check on keywords before adding them.  It caused
 **	later equivs that were the same as earlier abreviations of equivs to
 **	bomb out with a duplicate keyword message when it should have just
 **	removed the extra colliding earlier abreviations.
 ** 
 **	Revision 1.1  90/05/16  11:53:54  nadeau
 **	Initial revision
 ** 
 **/
/*LINTLIBRARY*/

#include "sdsccopyright.h"
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#ifdef sun
#include <floatingpoint.h> /* for strtod() */
#endif
#include "sdsc.h"


#ifdef __STDC__
static int      argPrintForm( char *, char *, char *, int );
#else
static int	argPrintForm( );
#endif





#ifndef NULL
#define NULL	0
#endif

#ifndef MAXINT
#define MAXINT	(((unsigned long)(~((unsigned long)0)))>>1)
#endif

#ifndef W_OK
#define W_OK	0x02
#define F_OK	0x00
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef TRUE
#define TRUE	1
#endif




/*
 * FUNCTION PROTOTYPES
 */

#ifdef __STDC__

static int      argPrintForm( char *, char *, char *, int );
static void 	argPrintWithNames(char *, char *);

#else

static int	argPrintForm( );
static void 	argPrintWithNames();

#endif





/*
 *  GLOBAL VARIABLE
 *	ArgErrNo	-  error number
 *	ArgNErr		-  number of error messages
 *	ArgErrList	-  error messages
 *
 *  DESCRIPTION
 *	On an error, the argument parsing package routines return -1 and set
 *	ArgErrNo to an error code.  The programmer may call ArgPError
 *	to print the associated error message to stderr, or may do the
 *	message lookup in ArgErrList themselves.
 */
int ArgErrNo = -1;		/* Arg package error number		*/
char *ArgErrList[] =		/* Error message list			*/
{
	/* ARGESYS */		"System error:  see errno",
	/* ARGEUNKKEYWORD */	"Unknown keyword",
	/* ARGENOPT */		"Bad option number",
	/* ARGENOCCUR */	"Bad occurrence number",
	/* ARGENVALUE */	"Bad value number",
	/* ARGENOVALUE */	"Option has no values",
	/* ARGEREG */		"Cannot create registration form file",
	/* ARGEFBK */		"Cannot create feedback form file",
};
int ArgNErr = 8;		/* Number of error messages		*/





/*
 *  TYPEDEF & STRUCTURE
 *	argValue	-  info on one value of an occurrence of an option
 *	argOccur	-  info on one occurrence of an option
 *	argInfo		-  info on an option and all its occurrences
 *	argKeyword	-  info on a keyword in the hash table
 *
 *  DESCRIPTION
 *	From top down, the keyword hash table is constructed of argKeyword
 *	structs, each one containing the keyword hashed, and a pointer to
 *	an argInfo struct for it.  Hash collisions are handled by constructing
 *	a linked list of argKeyword structs hanging off the initial argKeyword
 *	struct in the table.
 *
 *	An argInfo struct contains a pointer to the caller's original
 *	ArgOption description, a count of the number of occurrences of
 *	the option on the parsed command line (initially zero), and a pointer
 *	to a linked list of such occurrences (initially NULL).
 *
 *	An argOccur struct contains info on a single occurrence of an option
 *	and its values on the command line.  It contains a count of the
 *	number of values and a linked list of that many argValue structs.
 *	argOccur struct's are also linked together into a linked list in the
 *	order in which options occur on the command-line.  arg_next points to
 *	the next in this list, while arg_option is a back-pointer to the
 *	ArgOption struct for the option.
 *
 *	An argValue struct is just an ArgValue struct with a linked list pointer
 *	to point to the next value in the list.
 */

typedef struct argValue
{
	ArgValue           arg_value;	/* Value information		*/
	struct argValue   *arg_next;	/* Next in value list		*/
} argValue;

typedef struct argOccur
{
	int                arg_nvalue;	/* Number of values in occurrence*/
	argValue          *arg_value;	/* List of values for option	*/
	struct argOccur   *arg_next;	/* Next in occurrence list	*/
	struct ArgOption  *arg_option;	/* Option owning this data	*/
	struct argOccur   *arg_clnext;	/* Next in command line order	*/
	int		   arg_noption;	/* Option # in command line order*/
} argOccur;

typedef struct argInfo
{
	ArgOption         *arg_option;	/* Option for argument		*/
	int                arg_noccur;	/* Number of occurrences of option*/
	argOccur          *arg_occur;	/* List of occurrences		*/
} argInfo;

typedef struct argKeyword
{
	char               arg_keyword[ARGKMAXLEN];/* Keyword name	*/
	argInfo           *arg_info;	/* Information on option	*/
	struct argKeyword *arg_next;	/* Next in collision list	*/
} argKeyword;





/*
 *  GLOBAL
 *	argStandard		-  standard options
 *	argStandardRegister	-  standard -register option
 *	argStandardFeedback	-  standard -feedback option
 *
 *  DESCRIPTION
 *	argStandard provides information on the standard options -help,
 *	-feedback, -register, and -version.  The hash table is primed with
 *	these options before parsing the command-line.
 */

#define ARGNSTANDARD	3
static ArgOption argStandard[ARGNSTANDARD] =
{
	{ "help",	NULL,	"Give help about specific options",
	  ARGFNONE,	0, 0,	ARGTNONE },
	{ "fullhelp",	NULL,	"Display a full-length help message",
	  ARGFNONE,	0, 0,	ARGTNONE },
	{ "version",	NULL,	"Display the command's version numbers",
	  ARGFFULLHELP,	0, 0,	ARGTNONE }
};

static ArgOption argStandardRegister[1] =
{
	{ "register",	NULL,	"Generate a user registration form%end\
   Create a software user registration form in the file %command.reg.0",
	  ARGFFULLHELP,	0, 0,	ARGTNONE },
};

static ArgOption argStandardFeedback[1] =
{
	{ "feedback",	NULL,	"Generate a feedback form (bug report)%end\
  Create a software feedback (bug  report) form in the file %command.fbk.0",
	  ARGFFULLHELP,	0, 0,	ARGTNONE },
};
#define ARGNSTANDARDOPT	(ARGNSTANDARD + 2)





/*
 *  GLOBALS
 *	argKeywordTable		-  Keyword hash table
 *	argKeywordTableLength	-  Length of the hash table
 *
 *  DESCRIPTION
 *	The keyword hash table is the primary data structure for referencing
 *	parsed command-line information.  All keywords, equivalent keywords,
 *	and their truncation abbreviations are entered into the hash table.
 *	Keywords, equivalent keywords and abbreviations that all refer to
 *	the same option, all point to the same argInfo structure.
 *
 *	The size of the hash table is determined and the space dynamically
 *	allocated by ArgParse().
 */
static int argKeywordTableLength   = 0;/* Size of hash table		*/
static argKeyword *argKeywordTable = NULL;/* Hash table of keywords	*/

/*
 * MACROS
 *   EndOfLineString
 *   EndOfLineStringLength
 *
 * DESCRIPTION
 *   This symbol in the help string seperates the one line help string
 * from the detailed help string.  i.e. The detailed help string is only
 * printed when -help -option is specified.  The one line help string
 * is printed when the options are listed.
 * 
 *   EndOfLineStringLength is the length of the complete string.
 */

#define EndOfLineString		"%end"
#define EndOfLineStringLength   4





/*
 *  TYPEDEF & STRUCTURE
 *	argHelpOption		-  alphabetical option list entry
 *
 *  DESCRIPTION
 *	For the printing of the option list information in a help message,
 *	ArgParse() creates alphabetically sorted lists of the option
 *	structures.  Each entry in the list is an argHelpOption struct.
 *	Each struct gives the keyword or equivalent keyword and a pointer
 *	to the option information.  Note that abbreviated keywords and
 *	equivalent keywords are not entered in this list.
 */
typedef struct argHelpOption
{
	char      *arg_keyword;
	ArgOption *arg_option;
} argHelpOption;

#ifdef __STDC__
static int	argSortCompare( argHelpOption *, argHelpOption * );
static char*    argPrintOneLineHelp( argHelpOption *);
#else
static int	argSortCompare( );
static char*    argPrintOneLineHelp( );
#endif





/*
 *  GLOBALS
 *	argHelpCommandName	-  name of the command (argv[0])
 *	argHelpCommand		-  command struct for -help and -fullhelp
 *	argHelpNOptionList	-  # of options in argHelpOptionList
 *	argHelpOptionList	-  options list for -help
 *	argFullHelpNOptionList	-  # of options in argFullHelpOptionList
 *	argFullHelpOptionList	-  options list for -fullhelp
 *
 *  DESCRIPTION
 *	These globals provide information needed by ArgHelp() in printing
 *	a help message.
 *
 *	argHelpCommandName is the name of the command, as invoked.  This
 *	is simply argv[0].
 *
 *	argHelpCommand is a pointer to the user's ArgCommand struct.
 *
 *	ArgHelpNOptionList and ArgFullHelpNOptionList are the lengths of the
 *	alphabetical option lists, and ArgHelpOptionList and ArgFullHelpOption-
 *	List are pointers to the heads of these lists.  The alphabetical
 *	option lists are used in printing the list of options in help messages.
 */

static char           *argHelpCommandName;	/* argv[0]		*/
static ArgCommand     *argHelpCommand;		/* Command info		*/
static int            argHelpNOptionList;	/* Length of list	*/
static argHelpOption *argHelpOptionList;	/* Sorted option list	*/
static int            argFullHelpNOptionList;	/* Length of list	*/
static argHelpOption *argFullHelpOptionList;	/* Sorted option list	*/





/*
 *  GLOBALS
 *	argOptionOrder		-  Options in command-line order
 *	argNOpt			-  Number of options on command-line
 *
 *  DESCRIPTION
 *	During the parsing of the command-line, the options are linked
 *	in to a command-line order list of occurrences and the number of
 *	options counted.
 */
static int       argNOpt = 0;			/* # of options		*/
static argOccur *argOptionOrder = NULL;		/* Command-line order list*/





/*
 *  FUNCTION
 *	ArgPError	-  Print error message
 *
 *  DESCRIPTION
 *	The error text associated with the current ArgErrNo is printed
 *	to stderr, preceded by the given leader string.
 */

void				/* Returns nothing			*/
#ifdef __STDC__
ArgPError( char *s )
#else
ArgPError( s )
	char *s;		/* Leader string			*/
#endif
{
	if ( ArgErrNo == ARGESYS )
		perror( s );
	else if ( ArgErrNo < 0 || ArgErrNo >= ArgNErr )
		(void)fprintf( stderr, "Unknown error\n" );
	else if ( s && *s )
		(void)fprintf( stderr, "%s: %s\n", s, ArgErrList[ArgErrNo] );
	else
		(void)fprintf( stderr, "%s\n", ArgErrList[ArgErrNo] );
}





/*
 *  FUNCTION
 *	ArgQError	-  Query error message
 *
 *  DESCRIPTION
 *	The error text associated with the current ArgErrNo is returned.
 */

extern int errno;			/* System call error code	*/
extern int sys_nerr;			/* # of system call error codes	*/
extern char *sys_errlist[];		/* Error code message strings	*/

char *					/* Returns error text		*/
#ifdef __STDC__
ArgQError( void )
#else
ArgQError( )
#endif
{
	if ( ArgErrNo == ARGESYS )
	{
		if ( errno < 0 || errno >= sys_nerr )
			return ( "Unknown error" );
		return ( sys_errlist[errno] );
	}
	if ( ArgErrNo < 0 || ArgErrNo >= ArgNErr )
		return ( "Unknown error" );
	return ( ArgErrList[ArgErrNo] );
}





/*
 *  FUNCTION
 *	argHash		-  hash a keyword to get its hash table index
 *
 *  DESCRIPTION
 *	The keyword's ASCII characters are added together and the result
 *	modulo the hash table size returned as the hash table index.
 */

static int				/* Returns hash index		*/
#ifdef __STDC__
argHash( char *keyword )
#else
argHash( keyword )
	char *keyword;			/* Keyword to hash		*/
#endif
{
	int i = 0;			/* Hash index			*/

	while ( *keyword )
		i += *keyword++;
	return ( i % argKeywordTableLength );
}





#ifdef DEBUG
/*
 *  FUNCTION
 *	argPrint	-  print the hash table for debugging
 *
 *  DESCRIPTION
 *	The hash table is walked and each entry, and its collision chain
 *	printed to stderr.
 */

static void				/* Returns nothing		*/
#ifdef __STDC__
argPrint( void )
#else
argPrint( )
#endif
{
	int i = 0;			/* Hash table index		*/
	argKeyword *ak;			/* Hash table pointer		*/

	(void)fprintf( stderr, "Hash table dump (%d entries):\n", argKeywordTableLength );
	for ( i = 0; i < argKeywordTableLength; i++ )
	{
		(void)fprintf( stderr, "[%2d] ", i );
		ak = &argKeywordTable[i];
		if ( ak->arg_info == NULL )
		{
			(void)fprintf( stderr, "empty\n" );
			continue;
		}
		(void)fprintf( stderr, "%s (%s)\n", ak->arg_keyword,
			ak->arg_info->arg_option->arg_keyword );
		for ( ak = ak->arg_next; ak; ak = ak->arg_next )
			(void)fprintf( stderr, "     %s (%s)\n", ak->arg_keyword,
				ak->arg_info->arg_option->arg_keyword );
	}
}
#endif /* DEBUG */





/*
 *  FUNCTION
 *	argFind		-  find a keyword's entry in the hash table
 *
 *  DESCRIPTION
 *	The keyword is hashed and looked up in the hash table.  The table's
 *	first entry, and its collision list, are searched until an entry
 *	is found that matches the keyword.  A pointer to the entry found,
 *	or NULL if none is found, is returned.
 */

static argInfo * 			/* Returns table entry		*/
#ifdef __STDC__
argFind( char *keyword )
#else
argFind( keyword )
	char *keyword;			/* Keyword to find in table	*/
#endif
{
	argKeyword *ak;			/* Keyword entry found		*/

	ak = &argKeywordTable[argHash( keyword )];
	while ( ak && strcmp( ak->arg_keyword, keyword ) != 0 )
		ak = ak->arg_next;
	if ( !ak )
		return ( NULL );
	return ( ak->arg_info );
}





/*
 *  FUNCTION
 *	argAdd		-  add an entry to the hash table
 *
 *  DESCRIPTION
 *	The keyword, and all its truncation abbreviations, are added to
 *	the hash table.  For each one of these, the name is hashed and
 *	looked up in the hash table.  The table entry, and each entry in
 *	its collision list is searched to see if the name is already in
 *	the table.  If it is, then either we have an error (if both
 *	that in the table, and the one we are adding are not abbreviations),
 *	or we have two keywords with a common set of abbreviations.
 *	In the later case we search for those abbreviations and remove them.
 */

static int				/* Returns status		*/
#ifdef __STDC__
argAdd( char *keyword, argInfo *info )
#else
argAdd( keyword, info )
	char *keyword;			/* Keyword to add to hash table	*/
	argInfo *info;			/* Options occurrence info	*/
#endif
{
	char *s;			/* String pointer		*/
	argKeyword *ak;			/* Hash table entry		*/
	char str[ARGKMAXLEN];		/* Temporary keyword holder	*/
	argKeyword *aktmp;		/* Temp hash table entry	*/
	argKeyword *akfirst;		/* First Hash table entry	*/
	int  len;			/* Length of temp keyword holder*/
	argInfo *oldinfo;		/* Old option's info		*/

	/*
	 *  For every abbreviation of the keyword, add the option and its
	 *  info structure to the hash table.
	 */
	s = str;
	(void)strcpy( s, keyword );
	for ( len = strlen( s ); *s; s[--len] = '\0' )
	{
		akfirst = ak = &argKeywordTable[argHash( s )];
		if ( ak->arg_info == NULL )
		{
			/*
			 *  Use this entry for the new keyword and option info.
			 */
			(void)strcpy( ak->arg_keyword, s );
			ak->arg_info = info;
			continue;
		}

		/*
		 *  Hash table entry already in use.  For this entry,
		 *  and each entry in the collision chain, check to
		 *  see if we have the same keyword.
		 */
		do
		{
			if ( strcmp( ak->arg_keyword, s ) != 0 )
				continue;		/* Not the same	*/

			/*
			 *  Keyword's are the same.  One of four situations
			 *  has occurred:
			 *
			 *  1.  Keyword in table is unabbreviated, and our
			 *      new keyword is unabbreviated:
			 *		The programmer goofed.  Issue an
			 *		error and exit.
			 *
			 *  2.  Keyword in table is unabbreviated, and our
			 *      new keyword is abbreviated:
			 *		We can't use this abbreviation, or
			 *		any further ones for our new keyword.
			 *		Remove all abbreviations of table
			 *		keyword.
			 *
			 *  3.  Keyword in table is abbreviated, and our
			 *      new keyword is unabbreviated:
			 *		We can't use any of our new keyword's
			 *		abbreviations and all shorter
			 *	        abbreviations of the table keyword must
			 *		be removed from the table.
			 *
			 *  4.  Keyword in table is abbreviated, and our new
			 *      keyword is abbreviated:
			 *		This and all further abbreviations of
			 *		our new keyword and the table keyword
			 *		are non-unique and must be removed
			 *		from the table.
			 */
			oldinfo = ak->arg_info;
			if ( strcmp( oldinfo->arg_option->arg_keyword, ak->arg_keyword ) == 0 )
			{
				/*
				 *  The keyword in the table already is not
				 *  an abbreviation.
				 */
				if ( strcmp( s, keyword ) == 0 )
				{
					/*
					 *  Neither is this an abbreviation.
					 *  So, we have two keywords that are
					 *  identical.  Programmer error.
					 */
					return ( -2 );
				}

				/*
				 *  All abbreviations of the keyword already
				 *  in the table will collide with those of
				 *  the new keyword.  Fall through and delete
				 *  those entries from the table.
				 */
				s[--len] = '\0';
			}
			else
			{
				/*
				 *  The keyword in the table already is
				 *  an abbreviation.
				 */
				if ( strcmp( s, keyword ) == 0 )
				{
					/*
					 *  Our new keyword isn't yet an
					 *  abbreviation.  Take over the
					 *  table entry that used to be for
					 *  some other keyword's abbreviation.
					 */
					ak->arg_info   = info;
					s[--len] = '\0';
				}

				/*
				 *  All abbreviations of the keyword already
				 *  in the table will collide with those of
				 *  the new keyword.  Fall through and delete
				 *  those entries from the table.
				 */
			}

			/*
			 *  Remove non-unique abbreviations.
			 */
			for ( ; *s ; s[--len] = '\0' )
			{
				ak = &argKeywordTable[argHash( s )];
				if ( ak->arg_info == NULL )
					continue;	/* Nothing there */

				if ( ak->arg_info == oldinfo &&
					strcmp( ak->arg_keyword, s ) == 0 )
				{
					/*
					 *  Remove entry from table and move
					 *  1st collision list entry up.
					 */
					if ( ak->arg_next == NULL )
					{
						ak->arg_info = NULL;
						continue;
					}
					aktmp = ak->arg_next;
					(void)strcpy( ak->arg_keyword, aktmp->arg_keyword );
					ak->arg_info = aktmp->arg_info;
					ak->arg_next = aktmp->arg_next;
					free( aktmp );
					/* fall thru to check collisions too */
				}

				/*
				 *  Walk the collision list and unlink the
				 *  abbreviation's entry (if found).
				 */
				if ( ak->arg_next == NULL )
					continue;
				for ( aktmp = ak, ak = ak->arg_next; ak;
					aktmp = ak, ak = ak->arg_next )
				{
					if ( ak->arg_info != oldinfo )
						continue;
					if ( strcmp( ak->arg_keyword, s ) != 0 )
						continue;
					aktmp->arg_next = ak->arg_next;
					free( ak );
					break;
				}
			}
			return ( 0 );
		} while ( (ak = ak->arg_next) );

		/*
		 *  None of the collision entries were for this keyword.
		 *  Add us to the front of the collision chain.
		 */
		if ( (ak = (argKeyword *)malloc( (unsigned int )sizeof( argKeyword ))) == NULL )
			return ( -1 );
		(void)strcpy( ak->arg_keyword, s );
		ak->arg_info      = info;
		ak->arg_next      = akfirst->arg_next;
		akfirst->arg_next = ak;
	}
	return ( 0 );
}





/*
 *  FUNCTION
 *	ArgHelp		-  print a -help message
 *	ArgFullHelp	-  print a -fullhelp message
 *
 *  DESCRIPTION
 *	A help message containing a usage line, the first part of the help
 *	text, a list of options, and the second part of the help text is
 *	printed to stderr.
 */

int					/* Returns status		*/
#ifdef __STDC__
ArgHelp( void )
#else
ArgHelp( )
#endif
{
	char line[81];			/* Output line buffer		*/
	char tmp[81];			/* Temporary line buffer	*/
	char *help1tmp;			/* Temporary help string holder	*/
	char *help2tmp;			/* Temporary help string holder	*/
	int i, j;			/* Counter			*/
	int len;			/* Current line length		*/
	argHelpOption *ah;		/* Help option list pointer	*/
	int nOpt;                       /* Number of options            */
	int specificHelp = 0;           /* Flag: Are we doing help for specific options? */
	char *optionName; 		/* Name of one option           */
	int nOccur;  			/* number of occurences of an option */
	char message[80];               /* Brief message                */
	char* tmpStr;			/* temporary char *		*/


	/* Give help for specific options, if any are given */

	if ( (nOpt = ArgQNOpt()) > 1)
		specificHelp = 1;

	/*
	 *  Print a usage line of the form:
	 *
	 *	Usage:  name options...
	 *
	 *  'name' is argv[0].
	 *
	 *  'options' is either the arg_usage text given in the caller's
	 *  ArgCommand structure, or it is generated automatically as a
	 *  list of the option keywords and their valuenames strings with
	 *  square brackets around optional options and implied keywords.
	 *  Hidden and FullHelp options are omitted.
	 */
	if ( argHelpCommand->arg_usage )
		(void)fprintf( stderr, "Usage :  %s %s\n", argHelpCommandName,
			argHelpCommand->arg_usage );
	else
	{
		(void)sprintf( line, "Usage :  %s ", argHelpCommandName );
		len = strlen( line );
		for ( i=0, ah=argHelpOptionList; i<argHelpNOptionList; i++, ah++ )
		{
			if ( (ah->arg_option->arg_flags & ARGFHIDDEN) ||
				(ah->arg_option->arg_flags & ARGFFULLHELP) )
				continue;
			if ( ah->arg_option->arg_valuenames &&
			     ah->arg_option->arg_valuenames[0] != '\0' )
			{
				if ( ah->arg_option->arg_flags & ARGFREQUIRED )
				{
					if ( ah->arg_option->arg_flags & ARGFIMPKEYWORD)
						(void)sprintf( tmp, "[-%s] %s ",
							ah->arg_keyword,
							ah->arg_option->arg_valuenames);
					else
						(void)sprintf( tmp, "-%s %s ",
							ah->arg_keyword,
							ah->arg_option->arg_valuenames);
				}
				else
				{
					if ( ah->arg_option->arg_flags & ARGFIMPKEYWORD)
						(void)sprintf( tmp, "[[-%s] %s] ",
							ah->arg_keyword,
							ah->arg_option->arg_valuenames);
					else
						(void)sprintf( tmp, "[-%s %s] ",
							ah->arg_keyword,
							ah->arg_option->arg_valuenames);
				}
			}
			else
			{
				if ( ah->arg_option->arg_flags & ARGFREQUIRED )
				{
					if ( ah->arg_option->arg_flags & ARGFIMPKEYWORD)
						(void)sprintf( tmp, "[-%s] ",
							ah->arg_keyword );
					else
						(void)sprintf( tmp, "-%s ",
							ah->arg_keyword );
				}
				else
					(void)sprintf( tmp, "[-%s] ", ah->arg_keyword );
			}

			if ( strlen( tmp ) + len > 80 )
			{
				(void)fprintf( stderr, "%s\n", line );
				(void)sprintf( line, "           " );
				len = strlen( line );
			}
			(void)strcat( line, tmp );
			len += strlen( tmp );
		}
		(void)fprintf( stderr, "%s\n", line );
	}

	/*
	 *  Print a copyright message, if any.
	 */
	if ( argHelpCommand->arg_copyright  && specificHelp==0)
		(void)fprintf( stderr, "\n%s\n\n", argHelpCommand->arg_copyright );


	/*
	 *  Make a temporary private copy of the help text before we scan,
	 *  potentially modify, and print it.
	 *
	 *  This may seem like a dumb, inefficient, and unneccesary thing
	 *  to do, and it is.  However, it really is necessary for portability.
	 *  A command's help text is typically assigned to the ArgCommand
	 *  structure by a compile-time initialization.  On some hosts (such
	 *  as the NeXT and its MACH Gnu C compiler) such compile-time
	 *  initialized data is placed into a write-protected data segment
	 *  that cannot be changed at run-time without causing a memory error.
	 *  Since we do need to make minor changes at run-time, we are forced
	 *  to make a private copy in an unprotected segment and change the
	 *  copy instead of the original.
	 */
	if ( argHelpCommand->arg_help1 && specificHelp==0)
	{
		if ( (help1tmp = (char *)malloc( (unsigned int )(strlen( argHelpCommand->arg_help1 ) + 1) )) == NULL )
		{
			(void)fprintf( stderr, "%s:  Out of memory in argument parsing!\n",
				argHelpCommandName );
			exit( 1 );
			/*NOTREACHED*/
		}
		(void)strcpy( help1tmp, argHelpCommand->arg_help1 );
	}
	else
		help1tmp = NULL;
	if ( argHelpCommand->arg_help2 && specificHelp==0)
	{
		if ( (help2tmp = (char *)malloc( (unsigned int)(strlen( argHelpCommand->arg_help2 ) + 1) )) == NULL )
		{
			(void)fprintf( stderr, "%s:  Out of memory in argument parsing!\n",
				argHelpCommandName );
			exit( 1 );
			/*NOTREACHED*/
		}
		(void)strcpy( help2tmp, argHelpCommand->arg_help2 );
	}
	else
		help2tmp = NULL;

	/*
	 * If there are options beyond the help option, then
	 * for each option, print the help text accompanying 
	 * this option (if any).
	 *
	 * If there are no options beyond the help option, then
	 * print the command help text.
	 */
	 if ( specificHelp==1 )
	 {
		/* Print help for each command that was given. */
		for (i=1; i < nOpt; i++)
		{
			/*
			 * Copy the help string into a buffer (for the same
			 * reasons given above).  Then search for the
			 * string EndOfLineString in the line.  If it's there
			 * then give specific help about this option.
			 * If it's not there, explain that there is no
			 * specific help for this option.
			 *
			 * You may be wondering why there is simply not
			 * another element of the structure containing
			 * extra help for an option.  
			 *
			 * There should be.  However, this package was
			 * not designed with this function in mind, so
			 * to preserve backwards compatibility, we're
			 * doing it this way.
			 */

			/* Get the name of option number i */
			optionName = ArgQOpt (i, &nOccur);

			for ( j=0, ah=argFullHelpOptionList; j<argFullHelpNOptionList; j++, ah++ )
			{
				if (strcmp(ah->arg_keyword, optionName)==0)
					break;
			}

			if (j<argFullHelpNOptionList)
			{
				/*
				 * Get the string occuring after EndOfLineString and print out
				 * the stuff before EndOfLineString.
				 *
				 * Since this string will be modified, we need to make a copy
				 * of it.  (See note above.)
				 */
				fprintf(stderr,"\n");
				tmpStr = argPrintOneLineHelp(ah);
				if (tmpStr)
				{
					if (( (help1tmp = (char *)malloc( strlen(tmpStr) + 1) )) == NULL )
					{
						(void)fprintf( stderr, "%s:  Out of memory in argument parsing!\n",
							argHelpCommandName );
						exit( 1 );
					}
					strcpy( help1tmp, tmpStr);
				}
				else
					help1tmp = NULL;
				fprintf(stderr,"\n");
			}
			else
			{
				/*
				 * This option has no help at all whatsoever associated with it 
				 * i.e. it is a hidden option or something.
				 */
				help1tmp = NULL;
				fprintf(stderr,"    -%s\n\n",optionName);
			}

			/* Print out a friendly line for each attribute of this option. */
			if ( ah->arg_option->arg_flags & ARGFREQUIRED)
			{
				/*
				 * I've worded this "A value for --- must be present"
				 * to distinquish it from something with an implied
				 * keyword.  Note that I haven't checked to see if
				 * this argument really does take a value, but it's
				 * impossible to have an implied keyword unless you
				 * take a value. 
				 */
				sprintf(message,
				"        A value for the -%%option argument must be given to use %%command.");
				argPrintWithNames(message,optionName);
			}
			if ( ah->arg_option->arg_flags & ARGFIMPKEYWORD)
			{
				sprintf(message,
				"        The flag '-%%option' may be omitted when giving a value for -%%option.");
				argPrintWithNames(message,optionName);
			}
			if ( ah->arg_option->arg_flags & ARGFMULTIPLE)
			{
				sprintf(message,
				"        The -%%option option may appear more than once.");
				argPrintWithNames(message,optionName);
			}
			if (help1tmp==NULL)
			{
				/* Print a return if we printed any of the above messages */
				if (ah->arg_option->arg_flags & (ARGFMULTIPLE | ARGFIMPKEYWORD | ARGFREQUIRED))
					fprintf(stderr,"\n");
				fprintf(stderr,"There is no specific help for this option.\n");
				sprintf(message,"Type %%command -fullhelp for general help about all of the options.\n\n");
				argPrintWithNames(message,optionName);
			}
			else
			{
				/* Print help for option */
				/* Print a return if we printed any of the above messages */
				if (ah->arg_option->arg_flags & (ARGFMULTIPLE | ARGFIMPKEYWORD | ARGFREQUIRED))
					fprintf(stderr,"\n");
				argPrintWithNames(help1tmp,optionName);
				fprintf(stderr,"\n");
			}
		}

	 }
	 else  /* Print the command help text */
	 {

		/* Print to stderr, substituting for %command */
		argPrintWithNames(help1tmp,NULL);

		/*
		 *  Print the list of options.
		 */
		for ( i=0, ah=argHelpOptionList; i<argHelpNOptionList; i++, ah++ )
		{
			if ( (ah->arg_option->arg_flags & ARGFHIDDEN) ||
				ah->arg_option->arg_flags & ARGFFULLHELP )
				continue;
			(void) argPrintOneLineHelp(ah);
		}

		/* Print to stderr, substituting for %command */
		argPrintWithNames(help2tmp,NULL);
	}

	/*
	 * Free up some memory
	 */
	if (help1tmp)
		free(help1tmp);
	if (help2tmp)
		free(help2tmp);
	return ( 0 );
}

/*
 * FUNCTION
 *    argPrintWithNames
 *
 * DESCRIPTION
 *    print a string to stderr, substituting
 * argHelpCommandName for %command and
 * the second parameter for %option.
 *
 * Passing a null for the second parameter will
 * overlook '%option's in the string.
 */

static void 	/* Returns nothing */
#ifdef __STDC__
argPrintWithNames(char* s1, char* option)
#else
argPrintWithNames(s1, option)
char* s1;
char* option;
#endif
{
	char *basename;			/* Basename of tool		*/
	char* s;

	s = strrchr( argHelpCommandName, '/' );
	if ( s == NULL )
		basename = argHelpCommandName;
	else
		basename = s + 1;

	/*
	 *  Print the text.  Scan for '%command' and
	 *  replace it with basename. Scan for '%option'
	 *  and replace it with option.
	 */

	if ( s1 != NULL && *s1 != '\0' )
	{
		while ( (s = strchr( s1, '%' )) != NULL )
		{
			*s = '\0';
			(void)fprintf( stderr, "%s", s1 );
			*s = '%';
			if ( strncmp( "%command", s, 8 ) == 0 )
			{
				(void)fprintf( stderr, "%s", basename );
				s1 = s + 8;
			}
			else
			if ( option!=NULL && strncmp( "%option", s, 7 ) == 0)
			{
				(void)fprintf( stderr, "%s", option );
				s1 = s + 7;
			}
			else
			{
				(void)fprintf( stderr, "%%" );
				s1 = s + 1;
			}
		}
		(void)fprintf( stderr, "%s\n", s1 );
	}
}

int					/* Returns status		*/
#ifdef __STDC__
ArgFullHelp( void )
#else
ArgFullHelp( )
#endif
{
	char line[81];			/* Output line buffer		*/
	char tmp[81];			/* Temporary line buffer	*/
	char *basename;			/* Basename of tool		*/
	char *s;			/* String pointer		*/
	char *s1;			/* Start of string		*/
	char *help1tmp;			/* Temporary help string holder	*/
	char *help2tmp;			/* Temporary help string holder	*/
	int i;				/* Counter			*/
	int len;			/* Current line length		*/
	argHelpOption *ah;		/* Help option list pointer	*/


	/*
	 *  Print a usage line of the form:
	 *
	 *	Usage :  name options...
	 *
	 *  'name' is argv[0].
	 *
	 *  'options' is either the arg_fullusage or arg_usage texts
	 *  given in the caller's ArgCommand structure, or it is generated
	 *  automatically as a list of the option keywords and their
	 *  valuenames strings with square brackets around optional options
	 *  and implied keywords.  Hidden options are omitted.
	 */
	if ( argHelpCommand->arg_fullusage )
		(void)fprintf( stderr, "Usage :  %s %s\n", argHelpCommandName,
			argHelpCommand->arg_fullusage );
	else if ( argHelpCommand->arg_usage )
		(void)fprintf( stderr, "Usage :  %s %s\n", argHelpCommandName,
			argHelpCommand->arg_usage );
	else
	{
		(void)sprintf( line, "Usage :  %s ", argHelpCommandName );
		len = strlen( line );
		for ( i=0, ah=argFullHelpOptionList; i<argFullHelpNOptionList; i++, ah++ )
		{
			if ( ah->arg_option->arg_flags & ARGFHIDDEN )
				continue;
			if ( ah->arg_option->arg_valuenames &&
			     ah->arg_option->arg_valuenames[0] != '\0' )
			{
				if ( ah->arg_option->arg_flags & ARGFREQUIRED )
				{
					if ( ah->arg_option->arg_flags & ARGFIMPKEYWORD)
						(void)sprintf( tmp, "[-%s] %s ",
							ah->arg_keyword,
							ah->arg_option->arg_valuenames);
					else
						(void)sprintf( tmp, "-%s %s ",
							ah->arg_keyword,
							ah->arg_option->arg_valuenames);
				}
				else
				{
					if ( ah->arg_option->arg_flags & ARGFIMPKEYWORD)
						(void)sprintf( tmp, "[[-%s] %s] ",
							ah->arg_keyword,
							ah->arg_option->arg_valuenames);
					else
						(void)sprintf( tmp, "[-%s %s] ",
							ah->arg_keyword,
							ah->arg_option->arg_valuenames);
				}
			}
			else
			{
				if ( ah->arg_option->arg_flags & ARGFREQUIRED )
				{
					if ( ah->arg_option->arg_flags & ARGFIMPKEYWORD)
						(void)sprintf( tmp, "[-%s] ",
							ah->arg_keyword );
					else
						(void)sprintf( tmp, "-%s ",
							ah->arg_keyword );
				}
				else
					(void)sprintf( tmp, "[-%s] ", ah->arg_keyword );
			}

			if ( strlen( tmp ) + len > 80 )
			{
				(void)fprintf( stderr, "%s\n", line );
				(void)sprintf( line, "           " );
				len = strlen( line );
			}
			(void)strcat( line, tmp );
			len += strlen( tmp );
		}
		(void)fprintf( stderr, "%s\n", line );
	}


	/*
	 *  Print an copyright message, if any.
	 */
	if ( argHelpCommand->arg_copyright )
		(void)fprintf( stderr, "\n%s\n\n", argHelpCommand->arg_copyright );


	/*
	 *  Make a temporary private copy of the help text before we scan,
	 *  potentially modify, and print it.
	 *
	 *  This may seem like a dumb, inefficient, and unneccesary thing
	 *  to do, and it is.  However, it really is necessary for portability.
	 *  A command's help text is typically assigned to the ArgCommand
	 *  structure by a compile-time initialization.  On some hosts (such
	 *  as the NeXT and its MACH Gnu C compiler) such compile-time
	 *  initialized data is placed into a write-protected data segment
	 *  that cannot be changed at run-time without causing a memory error.
	 *  Since we do need to make minor changes at run-time, we are forced
	 *  to make a private copy in an unprotected segment and change the
	 *  copy instead of the original.
	 */
	s1 = argHelpCommand->arg_fullhelp1;
	if ( !s1 )
		s1 = argHelpCommand->arg_help1;
	if ( s1 )
	{
		if ( (help1tmp = (char *)malloc( (unsigned int)(strlen( s1 ) + 1) )) == NULL )
		{
			(void)fprintf( stderr, "%s:  Out of memory in argument parsing!\n",
				argHelpCommandName );
			exit( 1 );
			/*NOTREACHED*/
		}
		(void)strcpy( help1tmp, s1 );
	}
	else
		help1tmp = NULL;
	s1 = argHelpCommand->arg_fullhelp2;
	if ( !s1 )
		s1 = argHelpCommand->arg_help2;
	if ( s1 )
	{
		if ( (help2tmp = (char *)malloc( (unsigned int)(strlen( s1 ) + 1) )) == NULL )
		{
			(void)fprintf( stderr, "%s:  Out of memory in argument parsing!\n",
				argHelpCommandName );
			exit( 1 );
			/*NOTREACHED*/
		}
		(void)strcpy( help2tmp, s1 );
	}
	else
		help2tmp = NULL;


	/*
	 *  Print the first part of the help text.  Scan for '%command' and
	 *  replace it with argv[0], and "%com" and replace it with
	 *  argHelpCommand->arg_name.
	 */
	s = strrchr( argHelpCommandName, '/' );
	if ( s == NULL )
		basename = argHelpCommandName;
	else
		basename = s + 1;
	s1 = help1tmp;
	if ( s1 != NULL && *s1 != '\0' )
	{
		while ( (s = strchr( s1, '%' )) != NULL )
		{
			*s = '\0';
			(void)fprintf( stderr, "%s", s1 );
			*s = '%';
			if ( strncmp( "%command", s, 8 ) == 0 )
			{
				(void)fprintf( stderr, "%s", basename );
				s1 = s + 8;
			}
			else
			{
				(void)fprintf( stderr, "%%" );
				s1 = s + 1;
			}
		}
		(void)fprintf( stderr, "%s\n", s1 );
	}


	/*
	 *  Print the list of options.
	 */
	for ( i=0, ah=argFullHelpOptionList; i<argFullHelpNOptionList; i++, ah++ )
	{
		if ( !( ah->arg_option->arg_flags & ARGFHIDDEN ))
			(void) argPrintOneLineHelp(ah);
	}


	/*
	 *  Print the second part of the help text.  Scan for '%command' and
	 *  replace it with argv[0].
	 */
	s1 = help2tmp;
	if ( s1 != NULL && *s1 != '\0' )
	{
		while ( (s = strchr( s1, '%' )) != NULL )
		{
			*s = '\0';
			(void)fprintf( stderr, "%s", s1 );
			*s = '%';
			if ( strncmp( "%command", s, 8 ) == 0 )
			{
				(void)fprintf( stderr, "%s", basename );
				s1 = s + 8;
			}
			else
			{
				(void)fprintf( stderr, "%%" );
				s1 = s + 1;
			}
		}
		(void)fprintf( stderr, "%s\n", s1 );
	}

	return ( 0 );
}





/*
 *  FUNCTION
 *	ArgFeedback	-  dump a feedback from to a file
 *	ArgRegister	-  dump a user-registration form to a file
 *	argPrintForm	-  print a form to a file
 *
 *  DESCRIPTION
 *	ArgFeedback calls argPrintForm to print out a user feedback form.
 *	ArgRegister calls argPrintForm to print out a user registration form.
 *
 *	argPrint form tries to create an output file in the current directory.
 *	The initial file name attempted is
 *			toolname.xxx.0
 *	where xxx is 'fbk' for feedback forms, and 'reg' for registration
 *	forms.
 *
 *	If the file already exists, the '0' is incremented 1 and tried again
 *	until a file can be created.
 *
 * 	The form is automatically filled in with information on
 *	this tool.  A message is written to stderr to indicate that the
 *	form has been written out.
 */

int					/* Returns status		*/
#ifdef __STDC__
ArgRegister( void )
#else
ArgRegister( )
#endif
{
	return ( argPrintForm( "reg", "User Registration Form",
		argHelpCommand->arg_register, ARGEREG ) );
}

int					/* Returns status		*/
#ifdef __STDC__
ArgFeedback( void )
#else
ArgFeedback( )
#endif
{
	return ( argPrintForm( "fbk", "User Feedback Form",
		argHelpCommand->arg_feedback, ARGEFBK ) );
}

#define MAXTRIES	20

static int				/* Returns status		*/
#ifdef __STDC__
argPrintForm( char *extension, char *name, char *form, int errorcode )
#else
argPrintForm( extension, name, form, errorcode )
	char *extension;		/* File name extension to use	*/
	char *name;			/* Form name			*/
	char *form;			/* Form content			*/
	int   errorcode;		/* What to return on error	*/
#endif
{
	FILE *fp;			/* Form file			*/
	char filename[1024];		/* Name of form file		*/
	int i;				/* Counter			*/
	time_t clock;			/* Clock time			*/


	/*
	 *  Check access permissions and create the form file.
	 */
	for ( fp = NULL, i = 0; i <= MAXTRIES && fp==NULL ; i++ )
	{
		(void)sprintf( filename, "%s.%s.%d", argHelpCommand->arg_name,
			extension, i );
		if ( (fp = fopen( filename, "rb" )) != NULL )
		{
			/* File exists. */
			fclose ( fp );
			fp = NULL;
		}
		else
		{
			fp = fopen( filename, "wb" );
			if (fp==NULL)
			{
				ArgErrNo = ARGESYS;
				return -1;
			}
		}
	}
	if ( fp == NULL )
	{
		ArgErrNo = errorcode;
		return ( -1 );
	}


	/*
	 *  Output a form.
	 */
	(void)fprintf( fp, "\n" );
	(void)fprintf( fp, "				%s\n", name );
	(void)fprintf( fp, "\n" );
	(void)fprintf( fp, "	Product:	%s\n", argHelpCommandName );
	(void)fprintf( fp, "	Version:	%d.%d.%d\n",
		argHelpCommand->arg_major, argHelpCommand->arg_minor,
		argHelpCommand->arg_subminor );
	clock = time( (time_t *)0 );
	(void)fprintf( fp, "	Date:		%s", ctime( &clock ) );
	(void)fprintf( fp, "\n" );
	(void)fprintf( fp, form );

	(void)fclose( fp );

	(void)fprintf( stderr, "A %s has been written to:  %s\n", name, filename );
	(void)fprintf( stderr, "Instructions for filling out the form are included.\n" );
	return ( 0 );
}





/*
 *  FUNCTION
 *	ArgVersion	-  print version number info to stderr
 *
 *  DESCRIPTION
 *	An SDSC copyright message and information on the version number of
 *	the current tool is written to stderr.
 */

int					/* Returns status		*/
#ifdef __STDC__
ArgVersion( void )
#else
ArgVersion( )
#endif
{
	if ( argHelpCommand->arg_version )
		(void)fprintf( stderr, "%s\n", argHelpCommand->arg_version );
	if ( argHelpCommand->arg_copyright )
		(void)fprintf( stderr, "%s\n", argHelpCommand->arg_copyright );
	(void)fprintf( stderr, "Invoked as:  %s\n", argHelpCommandName );
	(void)fprintf( stderr, "Written as:  %s\n", argHelpCommand->arg_name );
	(void)fprintf( stderr, "Version:     %d.%d.%d\n", argHelpCommand->arg_major,
		argHelpCommand->arg_minor, argHelpCommand->arg_subminor );
	return ( 0 );
}





/*
 *  FUNCTION
 *	argGetValue	-  parse a value out of an argument
 *
 *  DESCRIPTION
 *	Given the argument string and a description of an option, a value
 *	is extracted.  Values may be integers, floats or strings, or
 *	a range of integers or floats.  Integers and floats may be positive
 *	or negative.  Integers may be in any base.  Floats may or may not
 *	start with a numeric (as in .5 vs. 0.5) and may have an exponent.
 *
 *	A pointer to a new value struct is returned.
 */

static argValue *			/* Returns new value struct	*/
#ifdef __STDC__
argGetValue( char *arg, char *key, ArgOption *op )
#else
argGetValue( arg, key, op )
	char *arg;			/* Argument to make into a value*/
	char *key;			/* Keyword from command line	*/
	ArgOption *op;			/* Option data			*/
#endif
{
	argValue *val;			/* New value struct		*/
	char *s;			/* String pointer		*/
	long l, l2;			/* New long values		*/
	double d, d2;			/* New double values		*/

	if ( (val = (argValue *)malloc( (unsigned int)sizeof( argValue ) )) == NULL )
	{
		(void)fprintf( stderr, "%s:  Out of memory in argument parsing!\n",
			argHelpCommandName );
		exit( 1 );
		/*NOTREACHED*/
	}
	val->arg_value.arg_type = op->arg_type;
	val->arg_next = NULL;

	switch ( op->arg_type )
	{
	case ARGTINT:
		if ( *arg == '-' && !isdigit( arg[1] ) )
		{
			free( val );
			return ( NULL );/* End of value list.		*/
		}
		if ( !isdigit( *arg ) && *arg != '+' && *arg != '-' )
		{
			free( val );
			return ( NULL );/* End of value list.		*/
		}
		l = strtol( arg, &s, 0 );
		if ( *s != '\0' )
		{
			(void)fprintf( stderr, "%s:  Extra characters following integer value %ld for '%s' option\n",
				argHelpCommandName, l, key );
			if ( argHelpCommand->arg_flags & ARGFHELPONERROR )
				(void)ArgHelp( );
			if ( argHelpCommand->arg_flags & ARGFFULLHELPONERROR )
				(void)ArgFullHelp( );
			exit( 1 );
			/*NOTREACHED*/
		}
		val->arg_value.arg_i = l;
		return ( val );

	case (ARGTINT|ARGTRANGE):
		if ( *arg == '-' && !isdigit( arg[1] ) )
		{
			free( val );
			return ( NULL );/* End of value list.		*/
		}
		if ( !isdigit( *arg ) && *arg != '+' && *arg != '-' )
		{
			free( val );
			return ( NULL );/* End of value list.		*/
		}
		l = strtol( arg, &s, 0 );
		if ( *s == '\0' )
		{
			/* Single integer in range.			*/
			val->arg_value.arg_rform = ARGRSINGLE;
			val->arg_value.arg_ir1   = l;
			return ( val );
		}
		if ( *s != '-' )
		{
			(void)fprintf( stderr, "%s:  Extra characters following integer value %ld for '%s' option\n",
				argHelpCommandName, l, key );
			if ( argHelpCommand->arg_flags & ARGFHELPONERROR )
				(void)ArgHelp( );
			if ( argHelpCommand->arg_flags & ARGFFULLHELPONERROR )
				(void)ArgFullHelp( );
			exit( 1 );
			/*NOTREACHED*/
		}
		s++;
		if ( *s == '\0' )
		{
			/* Open ended integer range.			*/
			val->arg_value.arg_rform = ARGROPEN;
			val->arg_value.arg_ir1   = l;
			return ( val );
		}
		l2 = strtol( s, &s, 0 );
		if ( *s != '\0' )
		{
			(void)fprintf( stderr, "%s:  Extra characters following integer value %ld for '%s' option\n",
				argHelpCommandName, l2, key );
			if ( argHelpCommand->arg_flags & ARGFHELPONERROR )
				(void)ArgHelp( );
			if ( argHelpCommand->arg_flags & ARGFFULLHELPONERROR )
				(void)ArgFullHelp( );
			exit( 1 );
			/*NOTREACHED*/
		}
		val->arg_value.arg_rform = ARGRCLOSED;
		val->arg_value.arg_ir1   = l;
		val->arg_value.arg_ir2   = l2;
		return ( val );

	case ARGTFLOAT:
		if ( *arg == '-' &&
			((arg[1] != '.' && !isdigit( arg[1] )) ||
			 (arg[1] == '.' && !isdigit( arg[2] ))) )
		{
			free( val );
			return ( NULL );/* End of value list.		*/
		}
		if ( !isdigit( *arg ) && *arg != '+' && *arg != '-' && *arg != '.' )
		{
			free( val );
			return ( NULL );/* End of value list.		*/
		}
		d = strtod( arg, &s );
		if ( *s != '\0' )
		{
			(void)fprintf( stderr, "%s:  Extra characters following float value %f for '%s' option\n",
				argHelpCommandName, d, key );
			if ( argHelpCommand->arg_flags & ARGFHELPONERROR )
				(void)ArgHelp( );
			if ( argHelpCommand->arg_flags & ARGFFULLHELPONERROR )
				(void)ArgFullHelp( );
			exit( 1 );
			/*NOTREACHED*/
		}
		val->arg_value.arg_f = d;
		return ( val );

	case (ARGTFLOAT|ARGTRANGE):
		if ( *arg == '-' &&
			((arg[1] != '.' && !isdigit( arg[1] )) ||
			 (arg[1] == '.' && !isdigit( arg[2] ))) )
		{
			free( val );
			return ( NULL );/* End of value list.		*/
		}
		if ( !isdigit( *arg ) && *arg != '+' && *arg != '-' && *arg != '.' )
		{
			free( val );
			return ( NULL );/* End of value list.		*/
		}
		d = strtod( arg, &s );
		if ( *s == '\0' )
		{
			/* Single float in range.			*/
			val->arg_value.arg_rform = ARGRSINGLE;
			val->arg_value.arg_fr1   = d;
			return ( val );
		}
		if ( *s != '-' )
		{
			(void)fprintf( stderr, "%s:  Extra characters following float value %f for '%s' option\n",
				argHelpCommandName, d, key );
			if ( argHelpCommand->arg_flags & ARGFHELPONERROR )
				(void)ArgHelp( );
			if ( argHelpCommand->arg_flags & ARGFFULLHELPONERROR )
				(void)ArgFullHelp( );
			exit( 1 );
			/*NOTREACHED*/
		}
		s++;
		if ( *s == '\0' )
		{
			/* Open ended float range.			*/
			val->arg_value.arg_rform = ARGROPEN;
			val->arg_value.arg_fr1   = d;
			return ( val );
		}
		d2 = strtod( s, &s );
		if ( *s != '\0' )
		{
			(void)fprintf( stderr, "%s:  Extra characters following float value %f for '%s' option\n",
				argHelpCommandName, d2, key );
			if ( argHelpCommand->arg_flags & ARGFHELPONERROR )
				(void)ArgHelp( );
			if ( argHelpCommand->arg_flags & ARGFFULLHELPONERROR )
				(void)ArgFullHelp( );
			exit( 1 );
			/*NOTREACHED*/
		}
		val->arg_value.arg_rform = ARGRCLOSED;
		val->arg_value.arg_fr1   = d;
		val->arg_value.arg_fr2   = d2;
		return ( val );

	case ARGTSTRING:
		if ( *arg == '-' && arg[1] != '\0' && !isdigit( arg[1] ))
		{
			free( val );
			return ( NULL );/* End of value list.		*/
		}
		val->arg_value.arg_s = arg;
		return ( val );
	}
	return( NULL );
	/*NOTREACHED*/
}





/*
 *  FUNCTION
 *	argSortCompare	-  Comparison function for qsort
 *
 *  DESCRIPTION
 *	Called by qsort(), this function compares two entries in the
 *	help option list and returns an integer that is less than, equal
 *	to, or greater than zero based on whether the first item's keyword
 *	is less than, equal to, or greater than the second item's keyword.
 */

static int				/* Returns comparision result	*/
#ifdef __STDC__
argSortCompare( argHelpOption *one, argHelpOption *two )
#else
argSortCompare( one, two )
	argHelpOption *one;		/* First option to compare	*/
	argHelpOption *two;		/* Second option to compare	*/
#endif
{
	return ( strcmp( one->arg_keyword, two->arg_keyword ) );
}





/*
 *  FUNCTION
 *	ArgParse	-  Parse command line arguments
 *
 *  DESCRIPTION
 *	The command, options, and equivalent keyword information is all
 *	checked for legality.  While processing the options and equivs,
 *	both are added into the keyword hash table and the help option list.
 *
 *	The command-line is then parsed.  Each argument is checked to see if
 *	it is a keyword or a naked value.  Keywords are looked up in the hash
 *	table.  The appropriate number of arguments following the keyword are
 *	extracted and converted to values.  An occurrence struct and zero or
 *	more value structs are allocated, initialized, and linked into things.
 *	For naked values not preceded by keywords, the option list is searched
 *	for the next optional-keyword option that doesn't have any values yet.
 *	The appropriate number of arguments are then extracted and so on.
 */

int					/* Returns number of options	*/
#ifdef __STDC__
ArgParse( int argc, char *argv[ ], ArgCommand *cmnd, int noptions,
	ArgOption *options, int nequiv, ArgEquiv *equiv )
#else
ArgParse( argc, argv, cmnd, noptions, options, nequiv, equiv )
	int         argc;		/* Argument count		*/
	char       *argv[] ;		/* Argument vector		*/
	ArgCommand *cmnd;		/* Command information		*/
	int         noptions;		/* Number of options		*/
	ArgOption  *options;		/* Option information		*/
	int         nequiv;		/* Number of equivalent keywords*/
	ArgEquiv   *equiv;		/* Equivalent keywords		*/
#endif
{
	argInfo    *info;		/* Argument information		*/
	ArgOption  *op;			/* Current option		*/
	ArgEquiv   *eq;			/* Current equivalent item	*/
	argKeyword *ak;			/* Hash table pointer		*/
	argHelpOption *ah;		/* Help option list pointer	*/
	argHelpOption *afh;		/* Full Help option list pointer*/
	argValue   *val;		/* New value info		*/
	argValue   *va;			/* Value list pointer		*/
	argOccur   *occur;		/* Occurance of an option	*/
	argOccur   *oc;			/* Occurance list pointer	*/
	argOccur   *cmndline;		/* Current occurrence on cmnd line*/
	int         i;			/* Counter			*/
	long        maximum;		/* Maximum number of values	*/
	char       *s;			/* String pointer		*/
	char       *key;		/* Current parsed keyword	*/
	int         impMultiple = FALSE;/* Implied + Mult kywd given?	*/
	int         nImp = 0;		/* # of implied kywd options	*/
	int         doHelp = 0;         /* Signifies that we're doing help*/


	/*
	 *  Allocate space for the hash table.  To reduce the number of
	 *  collisions, the hash table should be larger than the number of
	 *  options and equivalent keywords.  Furthermore, each keyword
	 *  will be added to the hash table multiple times, once per unique
	 *  abbreviation.  Computing the size of the table is pretty much
	 *  arbitrary:
	 *
	 *	(number of options + number of equivalent keywords) *
	 *	the average length of a keyword (8 characters?) *
	 *	an expansion factor to reduce collisions
	 */
	argKeywordTableLength = (noptions + nequiv + ARGNSTANDARD) * 8 * 1;

	if ( (argKeywordTable = (argKeyword *)malloc( (unsigned int)(argKeywordTableLength * sizeof( argKeyword )) )) == NULL )
	{
		(void)fprintf( stderr, "ArgOption[-]:  Too many options and equivalent keywords.  Not enough memory!\n" );
		exit( 1 );
		/*NOTREACHED*/
	}

	ak = &argKeywordTable[argKeywordTableLength-1];
	do
	{
		ak->arg_info = NULL;
		ak->arg_next = NULL;
	} while ( --ak != argKeywordTable );

	ak->arg_info = NULL;
	ak->arg_next = NULL;


	/*
	 *  Allocate space for the alphabetically sorted lists of options
	 *  and their equivalent keywords.  These lists are used when printing
	 *  out the -help and -fullhelp texts.
	 */
	if ( (argHelpOptionList = (argHelpOption *)malloc( (unsigned int)((noptions + nequiv + ARGNSTANDARDOPT) * sizeof( argHelpOption ))) ) == NULL )
	{
		(void)fprintf( stderr, "ArgOption[-]:  Too many options and equivalent keywords.  Not enough memory!\n" );
		exit( 1 );
		/*NOTREACHED*/
	}
	if ( (argFullHelpOptionList = (argHelpOption *)malloc( (unsigned int)((noptions + nequiv + ARGNSTANDARDOPT) * sizeof( argHelpOption ))) ) == NULL )
	{
		(void)fprintf( stderr, "ArgOption[-]:  Too many options and equivalent keywords.  Not enough memory!\n" );
		exit( 1 );
		/*NOTREACHED*/
	}
	for ( ah = &argHelpOptionList[noptions + nequiv + ARGNSTANDARDOPT - 1]; ah != argHelpOptionList; ah-- )
	{
		ah->arg_keyword = NULL;
		ah->arg_option  = NULL;
	}
	ah->arg_keyword = NULL;
	ah->arg_option  = NULL;
	for ( afh = &argFullHelpOptionList[noptions + nequiv + ARGNSTANDARDOPT - 1]; afh != argFullHelpOptionList; afh-- )
	{
		afh->arg_keyword = NULL;
		afh->arg_option  = NULL;
	}
	afh->arg_keyword = NULL;
	afh->arg_option  = NULL;


	/*
	 *  Check the legality of everything in the cmnd structure.
	 */
	if ( cmnd == NULL )
	{
		(void)fprintf( stderr, "ArgCommand:  NULL ArgCommand struct pointer?\n" );
		exit( 1 );
		/*NOTREACHED*/
	}
	if ( cmnd->arg_name == NULL || cmnd->arg_name[0] == '\0' )
	{
		(void)fprintf( stderr, "ArgCommand:  arg_name == NULL or empty?\n" );
		exit( 1 );
		/*NOTREACHED*/
	}
	argHelpCommandName = argv[0];
	argHelpCommand     = cmnd;


	/*
	 *  Prime the keyword hash table with the standard options.
	 */
	for ( i = 0, op = argStandard; i < ARGNSTANDARD; i++, op++ )
	{
		if ( (info = (argInfo *)malloc( (unsigned int)sizeof( argInfo ) )) == NULL )
		{
			(void)fprintf( stderr, "ArgOption[%d]:  Too many options and equivalent keywords.  Not enough memory!\n", i );
			exit( 1 );
			/*NOTREACHED*/
		}
		info->arg_option = op;
		info->arg_noccur = 0;
		info->arg_occur  = NULL;
		if ( !(op->arg_flags & ARGFFULLHELP) )
		{
			ah->arg_keyword = op->arg_keyword;
			ah->arg_option  = op;
			ah++;
		}
		afh->arg_keyword = op->arg_keyword;
		afh->arg_option  = op;
		afh++;
		argAdd( op->arg_keyword, info );
	}
	if ( argHelpCommand->arg_register != NULL && *argHelpCommand->arg_register != '\0' )
	{
		if ( (info = (argInfo *)malloc( (unsigned int)sizeof( argInfo ) )) == NULL )
		{
			(void)fprintf( stderr, "ArgOption[%d]:  Too many options and equivalent keywords.  Not enough memory!\n", i );
			exit( 1 );
			/*NOTREACHED*/
		}
		op = argStandardRegister;
		info->arg_option = op;
		info->arg_noccur = 0;
		info->arg_occur  = NULL;
		if ( !(op->arg_flags & ARGFFULLHELP) )
		{
			ah->arg_keyword = op->arg_keyword;
			ah->arg_option  = op;
			ah++;
		}
		afh->arg_keyword = op->arg_keyword;
		afh->arg_option  = op;
		afh++;
		argAdd( op->arg_keyword, info );
	}
	if ( argHelpCommand->arg_feedback != NULL && *argHelpCommand->arg_feedback != '\0' )
	{
		if ( (info = (argInfo *)malloc( (unsigned int)sizeof( argInfo ) )) == NULL )
		{
			(void)fprintf( stderr, "ArgOption[%d]:  Too many options and equivalent keywords.  Not enough memory!\n", i );
			exit( 1 );
			/*NOTREACHED*/
		}
		op = argStandardFeedback;
		info->arg_option = op;
		info->arg_noccur = 0;
		info->arg_occur  = NULL;
		if ( !(op->arg_flags & ARGFFULLHELP) )
		{
			ah->arg_keyword = op->arg_keyword;
			ah->arg_option  = op;
			ah++;
		}
		afh->arg_keyword = op->arg_keyword;
		afh->arg_option  = op;
		afh++;
		argAdd( op->arg_keyword, info );
	}


	/*
	 *  Check the legality of everything in the options list and add
	 *  the options into the hash table.
	 */
	for ( i = 0, op = options; i < noptions; i++, op++ )
	{
		if ( op->arg_keyword == NULL || op->arg_keyword[0] == '\0' )
		{
			(void)fprintf( stderr, "ArgOption[%d]:  arg_keyword == NULL or empty?\n", i );
			exit( 1 );
			/*NOTREACHED*/
		}
		s = op->arg_keyword;
		if ( isdigit( *s ) )
		{
			(void)fprintf( stderr, "ArgOption[%d]:  arg_keyword is illegal?\n", i );
			exit( 1 );
			/*NOTREACHED*/
		}
		do
		{
			if ( iscntrl( *s ) )
			{
				(void)fprintf( stderr, "ArgOption[%d]:  arg_keyword is illegal?\n", i );
				exit( 1 );
				/*NOTREACHED*/
			}
		} while ( *++s );

		if ( op->arg_minvalues == ARGVNOMAX )
		{
			(void)fprintf( stderr, "ArgOption[%d]:  arg_minvalues == ARGVNOMAX?\n", i );
			exit( 1 );
			/*NOTREACHED*/
		}
		if ( op->arg_minvalues < 0 )
		{
			(void)fprintf( stderr, "ArgOption[%d]:  arg_minvalues < 0?\n", i );
			exit( 1 );
			/*NOTREACHED*/
		}
		if ( op->arg_maxvalues < 0 && op->arg_maxvalues != ARGVNOMAX )
		{
			(void)fprintf( stderr, "ArgOption[%d]:  arg_maxvalues < 0?\n", i );
			exit( 1 );
			/*NOTREACHED*/
		}
		if ( op->arg_minvalues > op->arg_maxvalues && op->arg_maxvalues != ARGVNOMAX )
		{
			(void)fprintf( stderr, "ArgOption[%d]:  arg_minvalues > arg_maxvalues?\n", i );
			exit( 1 );
			/*NOTREACHED*/
		}
		if ( op->arg_type == ARGTNONE && op->arg_minvalues != 0 )
		{
			(void)fprintf( stderr, "ArgOption[%d]:  arg_type == ARGTNONE, yet arg_minvalues > 0?\n", i );
			exit( 1 );
			/*NOTREACHED*/
		}
		if ( op->arg_type == (ARGTRANGE|ARGTSTRING) )
		{
			(void)fprintf( stderr, "ArgOption[%d]:  arg_type == (ARGTRANGE|ARGTSTRING)?\n", i );
			exit( 1 );
			/*NOTREACHED*/
		}
		if ( (op->arg_flags & (ARGFMULTIPLE|ARGFIMPKEYWORD)) ==
			(ARGFMULTIPLE|ARGFIMPKEYWORD) )
		{
			if ( nImp != 0 )
			{
				(void)fprintf( stderr, "ArgOption[%d]:  can only have one implied keyword option if\n", i );
				(void)fprintf( stderr, "    that one is allowed multiple times.\n" );
				exit( 1 );
				/*NOTREACHED*/
			}
			++nImp;
			impMultiple = TRUE;
		}
		else if ( (op->arg_flags & ARGFIMPKEYWORD) == ARGFIMPKEYWORD )
		{
			if ( impMultiple == TRUE )
			{
				(void)fprintf( stderr, "ArgOption[%d]:  can only have one implied keyword option if\n", i );
				(void)fprintf( stderr, "    that one is allowed multiple times.\n" );
				exit( 1 );
				/*NOTREACHED*/
			}
			nImp++;
		}


		/*
		 *  Allocate an info structure for the option
		 */
		if ( (info = (argInfo *)malloc( (unsigned int)sizeof( argInfo ) )) == NULL )
		{
			(void)fprintf( stderr, "ArgOption[%d]:  Too many options and equivalent keywords.  Not enough memory!\n", i );
			exit( 1 );
			/*NOTREACHED*/
		}
		info->arg_option = op;
		info->arg_noccur = 0;
		info->arg_occur  = NULL;
		switch ( argAdd( op->arg_keyword, info ) )
		{
		case -2:
			(void)fprintf( stderr, "ArgOption[%d]:  duplicate use of keyword %s\n", i, op->arg_keyword );
			exit( 1 );
			/*NOTREACHED*/
		case -1:
			(void)fprintf( stderr, "ArgOption[%d]:  Too many options and equivalent keywords.  Not enough memory!\n", i );
			exit( 1 );
			/*NOTREACHED*/
		}

		if ( !(op->arg_flags & ARGFFULLHELP) )
		{
			ah->arg_keyword = op->arg_keyword;
			ah->arg_option  = op;
			ah++;
		}
		afh->arg_keyword = op->arg_keyword;
		afh->arg_option  = op;
		afh++;
	}


	/*
	 *  Check the legality of everything in the equiv list and add
	 *  the equiv keywords into the hash table.
	 */
	for ( i = 0, eq = equiv; i < nequiv; i++, eq++ )
	{
		if ( (info = argFind( eq->arg_keyword )) == NULL )
		{
			(void)fprintf( stderr, "ArgEquiv[%d]:  arg_keyword %s not found in option list\n", i, eq->arg_keyword );
			exit( 1 );
			/*NOTREACHED*/
		}
		switch ( argAdd( eq->arg_equivkeyword, info ) )
		{
		case -2:
			(void)fprintf( stderr, "ArgEquiv[%d]:  duplicate use of keyword %s\n", i, eq->arg_equivkeyword );
			exit( 1 );
			/*NOTREACHED*/
		case -1:
			(void)fprintf( stderr, "ArgEquiv[%d]:  Too many options and equivalent keywords.  Not enough memory!\n", i );
			exit( 1 );
			/*NOTREACHED*/
		}

		if ( !(info->arg_option->arg_flags & ARGFFULLHELP) )
		{
			ah->arg_keyword = eq->arg_equivkeyword;
			ah->arg_option  = info->arg_option;
			ah++;
		}
		afh->arg_keyword = eq->arg_equivkeyword;
		afh->arg_option  = info->arg_option;
		afh++;
	}



	/*
	 *  Sort the option lists for use in printing the help text.
	 */
	argHelpNOptionList = ah - argHelpOptionList;
	argFullHelpNOptionList = afh - argFullHelpOptionList;
#ifdef __STDC__
	qsort( (char *)argHelpOptionList, argHelpNOptionList,
		sizeof( argHelpOption ), (int(*)(const void *, const void *)) argSortCompare );
	qsort( (char *)argFullHelpOptionList, argFullHelpNOptionList,
		sizeof( argHelpOption ), (int(*)(const void *, const void *))argSortCompare );
#else
	qsort( (char *)argHelpOptionList, argHelpNOptionList,
		sizeof( argHelpOption ), (int(*)( )) argSortCompare );
	qsort( (char *)argFullHelpOptionList, argFullHelpNOptionList,
		sizeof( argHelpOption ), (int(*)( ))argSortCompare );
#endif
	cmndline = NULL;



	/*
	 *  Parse it!
	 *
	 *  Note that if we are giving out help, then we
	 *  don't require any of the values for the options
	 *  to be given.  Also, we don't want to assume
	 *  anything about implied keywords.
	 */
	argNOpt  = 0;
	--argc;
	++argv;
	while ( argc )
	{
		if ( argv[0][0] == '-' && argv[0][1] != '\0' &&
			!isdigit(argv[0][1]) )
		{
			/* A keyword.					*/
			if ( (info = argFind( &argv[0][1] )) == NULL )
			{
				/* Not found!  Error.			*/
				(void)fprintf( stderr, "%s:  Unknown option '%s'\n",
					argHelpCommandName, argv[0] );
				if ( argHelpCommand->arg_flags & ARGFHELPONERROR )
					(void)ArgHelp( );
				else if ( argHelpCommand->arg_flags & ARGFFULLHELPONERROR )
					(void)ArgFullHelp( );
				else
					(void)fprintf( stderr, "%s:  Type '%s -help' for a list of options.\n",
						argHelpCommandName, argHelpCommandName );
				exit( 1 );
				/*NOTREACHED*/
			}

			/*
			 *  Allocate a new option occurrence structure,
			 *  initialize it and link it to the end of the list
			 *  of occurrences of this option.
			 */
			if ( (occur = (argOccur *)malloc( (unsigned int)sizeof( argOccur ))) == NULL )
			{
				(void)fprintf( stderr, "%s:  Out of memory in argument parsing!\n",
					argHelpCommandName );
				exit( 1 );
				/*NOTREACHED*/
			}
			key = *argv;
			--argc;
			++argv;
			occur->arg_nvalue = 0;
			occur->arg_value  = NULL;
			occur->arg_option = op = info->arg_option;
			occur->arg_next   = NULL;
			occur->arg_clnext = NULL;
			occur->arg_noption= argNOpt++;
			if ( cmndline == NULL )
				argOptionOrder = occur;
			else
				cmndline->arg_clnext = occur;
			cmndline = occur;
			info->arg_noccur++;
			if ( info->arg_occur == NULL )
				info->arg_occur = occur;
			else if ( op->arg_flags & ARGFMULTIPLE )
			{
				for ( oc = info->arg_occur; oc->arg_next; oc = oc->arg_next )
					;
				oc->arg_next = occur;
			}
			else
			{
				(void)fprintf( stderr, "%s:  Option '%s' may only be given once.\n",
					argHelpCommandName, key );
				if ( argHelpCommand->arg_flags & ARGFHELPONERROR )
					(void)ArgHelp( );
				else if ( argHelpCommand->arg_flags & ARGFFULLHELPONERROR )
					(void)ArgFullHelp( );
				else
					(void)fprintf( stderr, "%s:  Type '%s -help' for a list of options.\n",
						argHelpCommandName, argHelpCommandName );
				exit( 1 );
				/*NOTREACHED*/
			}


			if ( op->arg_minvalues == 0 && op->arg_maxvalues == 0 )
			{
				/* No values for this keyword.		*/

				/* Check for standard options.		*/
				if ( strcmp( op->arg_keyword, "help" ) == 0 )
				{
					doHelp = 1;
				}
				if ( strcmp( op->arg_keyword, "fullhelp" ) == 0)
				{
					(void)ArgFullHelp( );
					exit( 0 );
					/*NOTREACHED*/
				}
				if ( strcmp( op->arg_keyword, "feedback" ) ==0)
				{
					if (doHelp==0)
					{
						if ( ArgFeedback( ) == -1 )
							ArgPError( argHelpCommandName );
						exit( 0 );
						/*NOTREACHED*/
					}
				}
				if ( strcmp( op->arg_keyword, "register" ) ==0)
				{
					if (doHelp==0)
					{
						if ( ArgRegister( ) == -1 )
							ArgPError( argHelpCommandName );
						exit( 0 );
						/*NOTREACHED*/
					}
				}
				if ( strcmp( op->arg_keyword, "version" ) == 0)
				{
					if (doHelp==0)
					{
						(void)ArgVersion( );
						exit( 0 );
						/*NOTREACHED*/
					}
				}
				continue;
			}


			/*
			 *  Get the values.
			 */
			if ( op->arg_maxvalues == ARGVNOMAX )
				maximum = MAXINT;
			else
				maximum = op->arg_maxvalues;

			for ( i = 0, va = NULL; i < maximum && argc; i++ )
			{
				if ( (val = argGetValue( argv[0], key, op )) == NULL)
					break;

				/* Add value to value list.	*/
				occur->arg_nvalue++;
				if ( va == NULL )
					occur->arg_value = val;
				else
					va->arg_next = val;
				va = val;
				--argc;
				++argv;
			}
			if ( i < op->arg_minvalues && doHelp==0 )
			{
				(void)fprintf( stderr, "%s:  Too few values for option '%s'; at least %d %s required.\n",
					argHelpCommandName, key,
					op->arg_minvalues,
					op->arg_minvalues==1?"value":"values" );
				if ( argHelpCommand->arg_flags & ARGFHELPONERROR )
					(void)ArgHelp( );
				else if ( argHelpCommand->arg_flags & ARGFFULLHELPONERROR )
					(void)ArgFullHelp( );
				else
					(void)fprintf( stderr, "%s:  Type '%s -help' for a list of options.\n",
						argHelpCommandName, argHelpCommandName );
				exit( 1 );
				/*NOTREACHED*/
			}
			continue;
		}


		/*
		 *  It isn't a keyword.  Determine if there are any outstanding
		 *  options that have implied keywords and no values.
		 *  Alternatively, if there is only one implied keyword option,
		 *  and it can occur multiple times, use it.
		 */
		if (doHelp==0)  /* Don't use implied keywords when giving out help */
		{
			for ( i = 0, op = options; i < noptions; i++, op++ )
			{
				if ( (op->arg_flags & (ARGFIMPKEYWORD|ARGFMULTIPLE)) ==
					(ARGFIMPKEYWORD|ARGFMULTIPLE) )
				{
					info = argFind( op->arg_keyword );
					break;
				}
				if ( (op->arg_flags & ARGFIMPKEYWORD) &&
					((info = argFind( op->arg_keyword))->arg_noccur == 0 ) )
					break;
			}
			if ( i == noptions )
			{
				/*
				 *  No options with optional keywords are still
				 *  awaiting values.  This argument is bogus.
				 */
				(void)fprintf( stderr, "%s:  Unknown option '%s'\n",
					argHelpCommandName, argv[0] );
				if ( argHelpCommand->arg_flags & ARGFHELPONERROR )
					(void)ArgHelp( );
				else if ( argHelpCommand->arg_flags & ARGFFULLHELPONERROR )
					(void)ArgFullHelp( );
				else
					(void)fprintf( stderr, "%s:  Type '%s -help' for a list of options.\n",
						argHelpCommandName, argHelpCommandName );
				exit( 1 );
				/*NOTREACHED*/
			}
		}  /* End of if doHelp... */
		else
		{  /* Doing help.  No implied keywords, so give an error.
		    * Note that they could have typed -help command instead of
		    * -help -command.  Tell them what they've done.
		    */
			(void)fprintf( stderr, "%s:  Unrecognized word '%s'\n", argHelpCommandName, argv[0] );
			(void)fprintf( stderr, "     Type '%s -help -option' to receive help for a specific option\n",
				argHelpCommandName);
			(void)fprintf( stderr, "     Type '%s -help' for a list of options.\n",argHelpCommandName);
			exit ( 1 );
			/*NOTREACHED*/
		}


		/*
		 *  Allocate a new option occurrence structure,
		 *  initialize it and link it to the end of the list
		 *  of occurrences of this option.
		 */
		if ( (occur = (argOccur *)malloc( (unsigned int)sizeof( argOccur ))) == NULL )
		{
			(void)fprintf( stderr, "%s:  Out of memory in argument parsing!\n",
				argHelpCommandName );
			exit( 1 );
			/*NOTREACHED*/
		}
		occur->arg_nvalue = 0;
		occur->arg_value  = NULL;
		occur->arg_option = op;
		occur->arg_next   = NULL;
		occur->arg_clnext = NULL;
		occur->arg_noption= argNOpt++;
		if ( cmndline == NULL )
			argOptionOrder = occur;
		else
			cmndline->arg_clnext = occur;
		cmndline = occur;
		info->arg_noccur++;

		if ( info->arg_occur == NULL )
			info->arg_occur = occur;
		else
		{
			/* Multiple occurrence option.			*/
			for ( oc = info->arg_occur; oc->arg_next; oc = oc->arg_next )
				;
			oc->arg_next = occur;
		}


		/*
		 *  Get the values.
		 */
		if ( op->arg_maxvalues == ARGVNOMAX )
			maximum = MAXINT;
		else
			maximum = op->arg_maxvalues;

		for ( i = 0, va = NULL; i < maximum && argc; i++ )
		{
			if ( (val = argGetValue( argv[0], key, op )) == NULL)
				break;

			/* Add value to value list.			*/
			occur->arg_nvalue++;
			if ( va == NULL )
				occur->arg_value = val;
			else
				va->arg_next = val;
			va = val;
			--argc;
			++argv;
		}
		if ( i < op->arg_minvalues )
		{
			(void)fprintf( stderr, "%s:  Too few values for implied option '-%s'; at least %d %s required.\n",
				argHelpCommandName, op->arg_keyword,
				op->arg_minvalues,
				op->arg_minvalues==1?"value":"values" );
			if ( argHelpCommand->arg_flags & ARGFHELPONERROR )
				(void)ArgHelp( );
			else if ( argHelpCommand->arg_flags & ARGFFULLHELPONERROR )
				(void)ArgFullHelp( );
			else
				(void)fprintf( stderr, "%s:  Type '%s -help' for a list of options.\n",
					argHelpCommandName, argHelpCommandName );
			exit( 1 );
			/*NOTREACHED*/
		}
	}

	if (doHelp==1)
	{
		(void)ArgHelp( );
		exit( 0 );
		/* NOTREACHED */
	}

	/*
	 *  Check that all required options were given.
	 */
	for ( i = 0, op = options; i < noptions; i++, op++ )
	{
		if ( (op->arg_flags & ARGFREQUIRED) &&
			((info = argFind( op->arg_keyword ))->arg_noccur == 0 ))
		{
			(void)fprintf( stderr, "%s:  The '-%s' argument must be given.\n",
				argHelpCommandName, op->arg_keyword );
			if ( argHelpCommand->arg_flags & ARGFHELPONERROR )
				(void)ArgHelp( );
			else if ( argHelpCommand->arg_flags & ARGFFULLHELPONERROR )
				(void)ArgFullHelp( );
			else
				(void)fprintf( stderr, "%s:  Type '%s -help' for a list of options.\n",
					argHelpCommandName, argHelpCommandName );
			exit( 1 );
			/*NOTREACHED*/
		}
	}

	return ( argNOpt );
}





/*
 *  GLOBALS
 *	argCacheKeyword	-  keyword of last option queried
 *	argCacheInfo	-  info for last option queried
 *	argCacheNOccur	-  # of last occurrence queried
 *	argCacheOccur	-  occurrence for last occurrence queried
 *	argCacheNValue	-  # of last value queried
 *	argCacheValue	-  value for last value queried
 *
 *  DESCRIPTION
 *	These globals represent a cache of the information for the last
 *	query made.  In typical use of the ArgQ...() functions, a caller
 *	queries the number of occurrences of an option, then proceeds to
 *	query each value for each occurrence before moving on to the next
 *	option.  To reduce the overhead of continually having to hash
 *	the keyword, look it up in the table, follow a collision chain,
 *	following an occurrence chain, and then follow a value chain,
 *	the last used values are saved and used if the option being queried
 *	is the same as last time.
 */

static char     *argCacheKeyword = NULL;/* Cache keyword looked up	*/
static argInfo  *argCacheInfo    = NULL;/* Info for last keyword	*/
static int       argCacheNOccur  = 0;	/* # of occurrence for last keyword*/
static argOccur *argCacheOccur   = NULL;/* Occurance for last keyword	*/
static int       argCacheNValue  = 0;	/* # of last value for last keyword*/
static argValue *argCacheValue   = NULL;/* Value for last keyword	*/





/*
 *  FUNCTION
 *	ArgQNOpt	-  Query # of Options on Command-Line
 *	ArgQOpt		-  Query Option on Command-Line
 *
 *  DESCRIPTION
 *	ArgQNOpt() returns the number of options (keyword &values, and
 *	no-keyword & values) found on the command-line.  This count was
 *	made during parsing in ArgParse().
 *
 *	ArgQOpt() walks the option list to find the requested option and
 *	returns its keyword and the number of occurrences of that option
 *	type on the command-line.
 */

int					/* Returns # of options		*/
#ifdef __STDC__
ArgQNOpt( void )
#else
ArgQNOpt( )
#endif
{
	return ( argNOpt );
}

char *					/* Returns keyword for option	*/
#ifdef __STDC__
ArgQOpt( int nopt, int *noccur )
#else
ArgQOpt( nopt, noccur )
	int  nopt;			/* Which option			*/
	int *noccur;			/* Number of occurrences of it	*/
#endif
{
	int       i;			/* Counter			*/
	argOccur *oc;			/* Occurance list pointer	*/
	argOccur *oc2;			/* Occurance list pointer	*/

	if ( nopt < 0 || nopt > argNOpt )
	{
		ArgErrNo = ARGENOPT;
		return ( NULL );
	}
	for ( i=0, oc = argOptionOrder; i < nopt; i++, oc = oc->arg_clnext )
		;
	argCacheKeyword = oc->arg_option->arg_keyword;
	argCacheInfo    = argFind( argCacheKeyword );
	argCacheOccur   = oc;
	argCacheNValue  = 0;
	argCacheValue   = oc->arg_value;	/* Might be NULL	*/
	for ( i=0, oc2 = argCacheInfo->arg_occur; oc2 != oc; i++, oc2 = oc2->arg_next )
		;
	argCacheNOccur = *noccur = i;
	return ( argCacheKeyword );
}





/*
 *  FUNCTION
 *	ArgQNOccur	-  query # of occurrences of an option
 *	ArgQNValue	-  query # of values for an occurrence of an option
 *	ArgQOccurOpt	-  query which command-line option this occurrence is
 *	ArgQValue	-  query value for an occurrence of an option
 *
 *  DESCRIPTION
 *	These functions all find the keyword in the table, get its info,
 *	and return information on that option or an occurrence of that
 *	option.  Because these three functions will be used over and over,
 *	and usually in an order that refers to the same option several times
 *	in a row, all of the info, occurrence, and value pointer and count
 *	information is cached in several globals.  This avoids the cost of
 *	hashing, walking collision lists, and so on for every reference to the
 *	same option, occurrence, and value.  It also complicates the routines
 *	a bit.
 *
 *	ArgQNOccur() looks up the keyword, initializes the cached items and
 *	returns the number of occurrences of that option on the command-line.
 *
 *	ArgQNValue() looks up the keyword, looks up the requested occurrence
 *	of the keyword, initializes the cached items (or uses them) and returns
 *	the number of values given for that occurrence of that option on the
 *	command-line.
 *
 *	ArgQOCcurOpt() looks up the keyword and requested occurrence,
 *	initializes the cached items (or uses them) and returns the command-line
 *	option number for the option's occurrence.
 *
 *	ArgValue() looks up the keyword, looks up the requested occurrence
 *	of the keyword, looks up the requested value for the occurrence,
 *	initilaizes the cached items (or uses them) and returns a pointer to
 *	the value structure for that value of that occurrence of that option
 *	on the command-line.
 */

int					/* Returns # of occurrences	*/
#ifdef __STDC__
ArgQNOccur( char *keyword )
#else
ArgQNOccur( keyword )
	char *keyword;			/* Keyword of option to query	*/
#endif
{
	if ( argCacheKeyword && strcmp( argCacheKeyword, keyword ) == 0 )
		return ( argCacheInfo->arg_noccur );

	if ( (argCacheInfo = argFind( keyword )) == NULL )
	{
		ArgErrNo = ARGEUNKKEYWORD;
		argCacheKeyword = NULL;
		return ( -1 );
	}
	argCacheKeyword = argCacheInfo->arg_option->arg_keyword;
	argCacheNOccur  = 0;
	argCacheOccur   = argCacheInfo->arg_occur;
	argCacheNValue  = 0;
	if ( argCacheOccur == NULL )
		argCacheValue = NULL;
	else
		argCacheValue = argCacheOccur->arg_value;/* Might be NULL*/
	return ( argCacheInfo->arg_noccur );
}

int					/* Returns # of values		*/
#ifdef __STDC__
ArgQNValue( char *keyword, int noccur )
#else
ArgQNValue( keyword, noccur )
	char *keyword;			/* Keyword to query		*/
	int   noccur;			/* Which occurrence		*/
#endif
{
	argOccur *oc;			/* Occurance list pointer	*/
	int       i;			/* Counter			*/

	if ( !argCacheKeyword || strcmp( argCacheKeyword, keyword ) != 0 )
	{
		if ( (argCacheInfo = argFind( keyword )) == NULL )
		{
			ArgErrNo = ARGEUNKKEYWORD;
			argCacheKeyword = NULL;
			return ( -1 );
		}
		argCacheKeyword = argCacheInfo->arg_option->arg_keyword;
		argCacheNOccur  = 0;
		argCacheOccur   = argCacheInfo->arg_occur;
		argCacheNValue  = 0;
		if ( argCacheOccur == NULL )
			argCacheValue = NULL;
		else
			argCacheValue = argCacheOccur->arg_value;	/* Might be NULL*/
	}
	if ( noccur < 0 || noccur >= argCacheInfo->arg_noccur )
	{
		ArgErrNo = ARGENOCCUR;
		return ( -1 );
	}
	if ( argCacheNOccur != noccur )
	{
		if ( argCacheNOccur < noccur )
			for ( i=argCacheNOccur, oc = argCacheOccur; i<noccur; i++, oc = oc->arg_next)
				;
		else
			for ( i=0, oc = argCacheInfo->arg_occur; i<noccur; i++, oc = oc->arg_next)
				;
		argCacheNOccur = noccur;
		argCacheOccur  = oc;
		argCacheNValue = 0;
		argCacheValue  = oc->arg_value;	/* Might be NULL	*/
	}
	if ( argCacheValue == NULL )
	{
		ArgErrNo = ARGENOVALUE;
		return ( -1 );
	}
	return ( argCacheOccur->arg_nvalue );
}

int					/* Returns option number	*/
#ifdef __STDC__
ArgQOccurOpt( char *keyword, int noccur )
#else
ArgQOccurOpt( keyword, noccur )
	char     *keyword;		/* Keyword to query		*/
	int       noccur;		/* Which occurrence		*/
#endif
{
	argOccur *oc;			/* Occurance list pointer	*/
	int       i;			/* Counter			*/

	if ( !argCacheKeyword || strcmp( argCacheKeyword, keyword ) != 0 )
	{
		if ( (argCacheInfo = argFind( keyword )) == NULL )
		{
			ArgErrNo = ARGEUNKKEYWORD;
			argCacheKeyword = NULL;
			return ( -1 );
		}
		argCacheKeyword = argCacheInfo->arg_option->arg_keyword;
		argCacheNOccur  = 0;
		argCacheOccur   = argCacheInfo->arg_occur;
		argCacheNValue  = 0;
		if ( argCacheOccur == NULL )
			argCacheValue = NULL;
		else
			argCacheValue = argCacheOccur->arg_value;/*Might NUL*/
	}
	if ( noccur < 0 || noccur >= argCacheInfo->arg_noccur )
	{
		ArgErrNo = ARGENOCCUR;
		return ( -1 );
	}
	if ( argCacheNOccur != noccur )
	{
		if ( argCacheNOccur < noccur )
			for ( i=argCacheNOccur, oc = argCacheOccur; i<noccur; i++, oc = oc->arg_next)
				;
		else
			for ( i=0, oc = argCacheInfo->arg_occur; i<noccur; i++, oc = oc->arg_next)
				;
		argCacheNOccur = noccur;
		argCacheOccur  = oc;
		argCacheNValue = 0;
		argCacheValue  = oc->arg_value;	/* Might be NULL	*/
	}
	return ( argCacheOccur->arg_noption );
}

ArgValue *				/* Returns value		*/
#ifdef __STDC__
ArgQValue( char *keyword, int noccur, int nvalue )
#else
ArgQValue( keyword, noccur, nvalue )
	char     *keyword;		/* Keyword to query		*/
	int       noccur;		/* Which occurrence		*/
	int       nvalue;		/* Which value			*/
#endif
{
	argOccur *oc;			/* Occurance list pointer	*/
	argValue *val;			/* Value list pointer		*/
	int       i;			/* Counter			*/

	if ( !argCacheKeyword || strcmp( argCacheKeyword, keyword ) != 0 )
	{
		if ( (argCacheInfo = argFind( keyword )) == NULL )
		{
			ArgErrNo = ARGEUNKKEYWORD;
			argCacheKeyword = NULL;
			return ( NULL );
		}
		argCacheKeyword = argCacheInfo->arg_option->arg_keyword;
		argCacheNOccur  = 0;
		argCacheOccur   = argCacheInfo->arg_occur;
		argCacheNValue  = 0;
		if ( argCacheOccur == NULL )
			argCacheValue = NULL;
		else
			argCacheValue = argCacheOccur->arg_value;	/* Might be NULL*/
	}
	if ( noccur < 0 || noccur >= argCacheInfo->arg_noccur )
	{
		ArgErrNo = ARGENOCCUR;
		return ( NULL );
	}
	if ( argCacheNOccur != noccur )
	{
		if ( argCacheNOccur < noccur )
			for ( i=argCacheNOccur, oc = argCacheOccur; i<noccur; i++, oc = oc->arg_next)
				;
		else
			for ( i=0, oc = argCacheInfo->arg_occur; i<noccur; i++, oc = oc->arg_next)
				;
		argCacheNOccur = noccur;
		argCacheOccur  = oc;
		argCacheNValue = 0;
		argCacheValue  = oc->arg_value;	/* Might be NULL	*/
	}
	if ( argCacheValue == NULL )
	{
		ArgErrNo = ARGENOVALUE;
		return ( NULL );
	}
	if ( nvalue < 0 || nvalue >= argCacheOccur->arg_nvalue )
	{
		ArgErrNo = ARGENVALUE;
		return ( NULL );
	}
	if ( argCacheNValue < nvalue )
		for ( i=argCacheNValue, val=argCacheValue; i<nvalue; i++,val=val->arg_next )
			;
	else
		for ( i=0, val=argCacheOccur->arg_value; i<nvalue; i++,val=val->arg_next )
			;
	argCacheNValue = nvalue;
	argCacheValue  = val;
	return ( &val->arg_value );
}

/*
 * FUNCTION
 *     argPrintOneLineHelp
 *
 * DESCRIPTION
 *     Print a line to the screen of the form
 *             -option value      description
 *     Return the string occuring after EndOfLineString in
 *     the option description.
 */

static char *   /* Returns extra help */
#ifdef __STDC__
argPrintOneLineHelp( argHelpOption *ah)
#else
argPrintOneLineHelp( ah)
argHelpOption *ah;
#endif
{
	char line[81];			/* Output line buffer		*/
	char tmp[81];			/* Temporary line buffer	*/
	int len;			/* Current line length		*/
	char* extra;                    /* stuff after '%extra'         */
	int iExtra;                     /* index of %extra in string    */
	char preExtra[81];              /* Description (stuff before EndOfLineString) */

	(void)strcpy( line, "    " );
	len = 4;

	if ( ah->arg_option->arg_valuenames &&
	     ah->arg_option->arg_valuenames[0] != '\0' )
		(void)sprintf( tmp, "    -%s %s", ah->arg_keyword,
			ah->arg_option->arg_valuenames);
	else
		(void)sprintf( tmp, "    -%s", ah->arg_keyword );

	if (ah->arg_option->arg_help==NULL)
	{	/* No help */
		fprintf(stderr,"%-40s\n",tmp);
		return NULL;
	}
	/* Search for EndOfLineString */
	extra = strchr(ah->arg_option->arg_help, (int) '%');
	while (extra!=NULL && strncmp(extra,EndOfLineString,EndOfLineStringLength)!=0)
	{
		extra = strchr(extra+1, (int) '%');
	}

	/*
	 * Store the line we're going to print in preExtra 
	 * (i.e. this is the part of the string that's before 
	 * EndOfLineString)
	 */
	if (extra==NULL)
	{
		strcpy (preExtra, ah->arg_option->arg_help);
	}
	else
	{
		iExtra = (int) (extra - ah->arg_option->arg_help);
		strncpy(preExtra, ah->arg_option->arg_help, iExtra);
		preExtra[iExtra] = '\0';
		extra += EndOfLineStringLength;   /* Move to the right of EndOfLineString */
	}


	if ( strlen( tmp ) + len > 40 )
		(void)fprintf( stderr, "%s\n%-40s%s\n", tmp, " ", preExtra );
	else
		(void)fprintf( stderr, "%-40s%s\n", tmp, preExtra);

	return ( extra );
}

