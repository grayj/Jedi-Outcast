/**

 **	$Header: /sdsc/dev/vis/misc/libsdsc/v3.0/libsdsc/src/include/RCS/arg.h,v 1.10 1995/06/29 00:13:11 bduggan Exp $

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



/**

 **  FILE

 **	arg.h		-  Argument Parsing Includes

 **

 **  PROJECT

 **	libSDSC		-  SDSC standard function library

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	__ARGH__	d  file inclusion flag

 **

 **	ArgCommand	t  Information about a command/tool

 **

 **	ArgOption	t  Information about one possible option

 **	ARGF...		d  values for arg_flags in ArgOption

 **	ARGT...		d  values for arg_types in ArgOption and ArgValue

 **	ARGV...		d  values for arg_maxvalues in ArgOption

 **	ARGKMAXLEN	d  maximum length of a keyword

 **

 **	ArgEquiv	t  Equivalent keywords

 **

 **	ArgValue	t  Argument value

 **	ARGR...		d  values for arg_rform in ArgValue

 **	arg_...		d  shortcuts for referencing ArgValue fields

 **

 **	ArgErrNo	v  error number

 **	ArgNErr		v  number of error messages

 **	ArgErrList	v  error messages

 **	ARGE...		d  error codes

 **

 **	Arg...		f  function type declarations

 **

 **  PRIVATE CONTENTS

 **	none

 **

 **  HISTORY

 **	$Log: arg.h,v $

 **	Revision 1.10  1995/06/29  00:13:11  bduggan

 **	changed copyright

 **

 **	Revision 1.9  1995/02/21  23:42:24  bduggan

 **	Added comment about new %end option in help string

 **

 **	Revision 1.8  94/10/03  16:35:57  nadeau

 **	Updated to ANSI C and C++ compatibility.

 **	Minimized use of custom SDSC types (e.g., uchar vs. unsigned char)

 **	Updated comments.

 **	Updated indenting on some code.

 **	Updated copyright message.

 **	

 **	Revision 1.7  93/07/21  19:06:43  allans

 **	*** empty log message ***

 **	

 **	Revision 1.6  92/09/02  15:08:11  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.5  91/09/17  19:28:27  nadeau

 **	Added arg_fullusage field to ArgCommand structure.

 **	

 **	Revision 1.4  91/09/01  17:13:44  nadeau

 **	Changed ArgCommand structure to support additional fields for

 **	better help messages, version messages, and registration and

 **	feedback forms.

 **	

 **	Revision 1.3  91/08/25  13:51:57  nadeau

 **	Added declaration for ArgQOccurOpt.

 **	

 **	Revision 1.2  91/01/09  16:52:58  nadeau

 **	Cleaned up function declarations.  Added ArgQError().

 **	

 **	Revision 1.1  90/06/22  12:16:02  nadeau

 **	Initial revision

 **	

 **/



#ifndef __ARGH__

#define __ARGH__











/*

 *  TYPEDEF & STRUCTURE

 *	ArgCommand	-  Information about a command/tool

 *

 *  DESCRIPTION

 *	The ArgCommand structure provides overall information about a

 *	command (tool) using the argument parsing package.  Information

 *	includes:

 *

 *	arg_name	The name the command knows itself by.  This may

 *			differ from the name a site administrator has

 *			installed the command as, which would show up

 *			as argv[0].

 *

 *	arg_major, arg_minor, arg_subminor

 *			The version numbers of the command.  In a version

 *			number like 2.4.1, the major version number is 2,

 *			the minor number 4, and the subminor number 1.

 *

 *	arg_help1, arg_help2

 *			The first and second parts of the -help text.

 *			These are each typically to be multi-line help texts.

 *			If a NULL pointer, not printed.

 *

 *	arg_fullhelp1, arg_fullhelp2

 *			The first and second parts of the -fullhelp text.

 *			These are each typically to be multi-line help texts.

 *			If a NULL pointer, arg_help1 or arg_help2 used instead.

 *

 *	arg_usage

 *			The text for the help usage message printed in the form:

 *				Usage is:  command arg_usage

 *			If a NULL pointer, usage text is generated automatically

 *			from the option list.

 *

 *	arg_fullusage

 *			The text for the fullhelp usage msssage printed in the

 *			form:

 *				Usage is:  command arg_usage

 *			If a NULL pointer, usage text is generated automatically

 *			from the option list.

 *

 *	arg_flags

 *			Parsing control flags.

 *

 *	arg_version

 *			Version name text displayed for -version.  If a NULL

 *			pointer, nothing displayed.

 *

 *	arg_copyright

 *			Copyright message text displayed for -version, -help,

 *			and -fullhelp.  If a NULL, nothing displayed.

 *

 *	arg_register, arg_feedback

 *			The user registration and feedback forms displayed

 *			for the -register and -feedback arguments.  If a NULL

 *			pointer no forms are printed.

 */



typedef struct ArgCommand

{

	char *arg_name;		/* Command name				*/



	int   arg_major;	/* Major version number			*/

	int   arg_minor;	/* Minor version number			*/

	int   arg_subminor;	/* Subminor version number		*/



	char *arg_help1;	/* First part of -help information	*/

	char *arg_help2;	/* Second part of -help information	*/



	char *arg_fullhelp1;	/* First part of -fullhelp information	*/

	char *arg_fullhelp2;	/* Second part of -fullhelp information	*/



	int   arg_flags;	/* Parsing flags			*/

	char *arg_usage;	/* -help usage text			*/

	char *arg_fullusage;	/* -fullhelp usage text			*/

	char *arg_version;	/* Version name				*/

	char *arg_copyright;	/* Copyright message			*/

	char *arg_register;	/* Registration form			*/

	char *arg_feedback;	/* Feedback form			*/

} ArgCommand;











/*

 *  DEFINES

 *	ARGF*		-  flags for arg_flags in ArgCommand

 *

 *  DESCRIPTION

 *	arg_flags in the ArgCommand structure contains the bitwise OR of zero

 *	or more ARG... flags to indicate how options are to be parsed:

 *

 * 	ARGFHELPONERROR		-help text should be displayed on any

 *				command-line parsing error.  The default is

 *				to only print out the error.

 *

 *	ARGFFULLHELPONERROR	-fullhelp text should be displayed on any

 *				command-line parsing error.  The default is

 *				to only print out the error.

 */



#define ARGFHELPONERROR		0x1

#define ARGFFULLHELPONERROR	0x2

		/*	0x4-0xFFFFFFFF	Reserved			*/











/*

 *  TYPEDEF & STRUCTURE

 *	ArgOption	-  Information about one possible option

 *

 *  DESCRIPTION

 *	The ArgOption structure provides information on one possible option

 *	that the parsing code should look for in a user's input command line.

 *	An option consists of a keyword and zero or more values.  The

 *	fields of the ArgOption structure indicate what to look for for this

 *	option:

 *

 *	arg_keyword	The name of the keyword.  All options have keywords.

 *

 *	arg_valuenames	A help text string that gives the name(s) of the values

 *			that follow the keyword, if any.  If there are no

 *			values, the string should be NULL.

 *

 *	arg_help	A help text (40-character maximum) that says what

 *			the option is.  If '%end' appears in this text,

 *           		then the text before '%end' will be considered

 * 			the standard one-line help text, and the text

 * 			after %end will be displayed if help is requested

 *			for this specific option.

 *

 *	arg_flags	Flags indicating how the option should be handled.

 *

 *	arg_minvalues, arg_maxvalues

 *			The minimum and maximum number of values that

 *			parsing code should expect following the keyword.

 *			max must be greater than min.  min can be 0.

 *

 *	arg_type	The basic type (int, float, string) for the values.

 */



typedef struct ArgOption

{

	char *arg_keyword;	/* Keyword name				*/

	char *arg_valuenames;	/* Value names				*/

	char *arg_help;		/* Half-line help text for the option	*/

	int   arg_flags;	/* Parsing flags			*/

	int   arg_minvalues;	/* Minimum number of values		*/

	int   arg_maxvalues;	/* Maximum number of values		*/

	int   arg_type;		/* Type for the values			*/

} ArgOption;











/*

 *  DEFINES

 *	ARGF...		-  values for arg_flags in ArgOption

 *	ARGT...		-  values for arg_types in ArgOption and ArgValue

 *	ARGV...		-  values for arg_maxvalues in ArgOption

 *	ARGKMAXLEN	-  maximum length of a keyword

 *

 *  DESCRIPTION

 *	arg_flags in the ArgOption structure contains the bitwise OR of zero

 *	or more ARG... flags to indicate how the option is to be parsed:

 *

 *	ARGFNONE	No flags set.  Do default actions

 *

 *	ARGFHIDDEN	The option should not be listed in automatically

 *			printed -help text and -fullhelp.  Typically used for

 *			-debug options.

 *

 *	ARGFFULLHELP	Only display the option in -fullhelp listings.  If

 *			not given, display option in -help and -fullhelp.

 *

 *	ARGFMULTIPLE	The option may be given more than once on the

 *			same command line.

 *

 *	ARGFREQUIRED	The option is required.  Issue an error and help

 *			text if the option doesn't appear on the command line.

 *

 *	ARGFIMPKEYWORD	Allow keyword to be implied.  Typically used for

 *			filename arguments.

 *

 *	arg_type indicates the basic type for values following a keyword.

 *

 *	ARGTNONE	No type.  Used when option has no values.

 *

 *	ARGTINT		Integer.  Hex, Octal, Binary, Decimal, whatever.

 *

 *	ARGTFLOAT	Floating point.  With or without exponent.

 *

 *	ARGTSTRING	String.  Space-separated set of characters.  If user

 *			used double quotes, then the entire quoted string,

 *			minus the quotes.

 *

 *	As a special flag, ARGTRANGE may be bitwise OR'ed with ARGTINT or

 *	ARGTFLOAT (but not ARGSTRING) to indicate that a single value may

 *	be a range of numbers, such as 1-4.

 *

 *	arg_maxvalues is the maximum number of values that may follow the

 *	keyword in an option.  If there is no maximum, such as an

 *	arbitrarily long list of file names, then arg_maxvalues may be

 *	set to ARGVNOMAX.

 *

 *	Due to internal space requirements, the maximum length of a keyword

 *	is ARGKMAXLEN.

 */



#define ARGFNONE	0

#define ARGFHIDDEN	0x1

#define ARGFMULTIPLE	0x2

#define ARGFREQUIRED	0x4

#define ARGFIMPKEYWORD	0x8

#define ARGFFULLHELP	0x10

		/*	0x20-0xFFFFFFFF	Reserved			*/



#define ARGTNONE	0

#define ARGTINT		0x1

#define ARGTFLOAT	0x2

#define ARGTSTRING	0x4

		/*	0x8-0x800	Reserved			*/

#define ARGTRANGE	0x1000

		/*	0x2000-0xFFFFFFFF Reserved			*/



#define ARGVNOMAX	-1

		/*	-2 - -2^31	Reserved			*/

		/*	positive values indicate number of values	*/



#define ARGKMAXLEN	100











/*

 *  TYPEDEF & STRUCTURE

 *	ArgEquiv	-  Equivalent keywords

 *

 *  DESCRIPTION

 *	The ArgEquiv structure gives the keyword of an option previously

 *	described by an ArgOption structure, and an alternative, or

 *	equivalent name for the keyword.  For instance, an option with

 *	a keyword -background might have an equivalent keyword of -bg.

 *

 *		** Remember that all keywords and equivalent keywords

 *		** automatically allow all unique abbreviations to be

 *		** entered by the user.  For instance, if the option

 *		** is -name, with an equivalent of -title, then all of

 *		** the following may be entered by the user:

 *		**

 *		**	-name

 *		**	-nam

 *		**	-na

 *		**	-n

 *		**	-title

 *		**	-titl

 *		**	-tit

 *		**	-ti

 *		**	-t

 */



typedef struct ArgEquiv

{

	char *arg_keyword;	/* Base keyword				*/

	char *arg_equivkeyword;	/* Equivalent keyword			*/

} ArgEquiv;











/*

 *  TYPEDEF & STRUCTURE

 *	ArgValue	-  Argument value

 *

 *  DESCRIPTION

 *	The ArgValue structure is returned by ArgQValue() and describes a

 *	single value associated with an occurance of an option on the

 *	command line.  The term 'value' is somewhat overloaded to include

 *	any of the following:

 *

 *		A single integer.

 *		A single flaot.

 *		A single string (of arbitrary number of characters).

 *		A range of integers.

 *		A range of floats.

 *

 *	arg_type	The base type of the value.  Same as arg_type in

 *			the ArgOption structure.

 *

 *	arg_rform	The format of a range, if arg_type has ARGFRANGE set.

 *

 *	arg_v1		The first value for a range, or the only set of values

 *			if ARGFRANGE is not set.

 *

 *	arg_v2		The second value for a range.  Unused for non-range

 *			values.

 *

 *	arg_v1 and arg_v2 are each unions with integer, float, and string

 *	(arg_v1 only) sub-fields.  Only one of these fields will contain

 *	a valid value (obviously).

 */



typedef struct ArgValue

{

	int arg_type;		/* Value type				*/

	int arg_rform;		/* Range format, if any			*/

	union

	{

		long arg_i1;	/* First integer			*/

		double arg_f1;	/* First float				*/

		char *arg_s1;	/* First (and only) string		*/

		int arg_reserved1[4];	/* Reserved for future use	*/

	} arg_v1;

	union

	{

		long arg_i2;	/* Second integer			*/

		double arg_f2;	/* Second float				*/

		int arg_reserved2[4];	/* Reserved for future use	*/

	} arg_v2;

	int arg_reserved[4];	/* Reserved for future use		*/

} ArgValue;











/*

 *  DEFINES

 *	ARGR...		-  values for arg_rform in ArgValue

 *	arg_...		-  shortcuts for referencing ArgValue fields

 *

 *  DESCRIPTION

 *	arg_rform indicates the form of a range that the user entered as

 *	one value for an option.

 *

 *	ARGRCLOSED	n-m	Numbers n through m are in the range.

 *

 *	ARGROPEN	n-	Numbers n through the end are in the range.

 *				Just what the 'end' is is up to the command.

 *

 *	ARGRSINGLE	n	Just number n is in the range.

 *

 *	For options that are not ranges, it is inconvenient to worry about

 *	arg_v1 vs. arg_v2.  It is also annoying to have to mess with the

 *	union field's name.  A variety of define'd constant short-cuts to

 *	relevant fields of the ArgValue structure are available for use.

 */



#define ARGRCLOSED	0

#define ARGROPEN	1

#define ARGRSINGLE	2

		/*	3-2^32	Reserved				*/



#define arg_i		arg_v1.arg_i1

#define arg_f		arg_v1.arg_f1

#define arg_s		arg_v1.arg_s1

#define arg_ir1		arg_v1.arg_i1

#define arg_ir2		arg_v2.arg_i2

#define arg_fr1		arg_v1.arg_f1

#define arg_fr2		arg_v2.arg_f2











/*

 *  GLOBAL VARIABLE

 *	ArgErrNo	-  error number

 *	ArgNErr		-  number of error messages

 *	ArgErrList	-  error messages

 *

 *  DESCRIPTION

 *	On an error, the argument parsing package routines return -1 and set

 *	ArgErrno to an error code.  The programmer may call ArgPError

 *	to print the associated error message to stderr, or may do the

 *	message lookup in ArgErrList themselves.

 */



extern int   ArgErrNo;			/* Current error number		*/

extern int   ArgNErr;			/* Number of error messages	*/

extern char *ArgErrList[];		/* List of error messages	*/











/*

 *  CONSTANTS

 *	ARGE...		-  error codes

 *

 *  DESCRIPTION

 *	ArgErrNo may be set to these error codes as a result of an error in

 *	calling one of the arg parsing package routines.

 */



#define ARGESYS		0		/* System error			*/

#define ARGEUNKKEYWORD	1		/* Unknown keyword		*/

#define ARGENOPT	2		/* Bad option number		*/

#define ARGENOCCUR	3		/* Bad occurance number		*/

#define ARGENVALUE	4		/* Bad value number		*/

#define ARGENOVALUE	5		/* Option doesn't have values	*/

#define ARGEREG		6		/* Cannot open registration file*/

#define ARGEFBK		7		/* Cannot open feedback file	*/

		/*	8-2^32		   Reserved			*/











/*

 *  FUNCTIONS

 *	Arg...		-  function type declarations

 *

 *  DESCRIPTION

 *	The return types of the argument parsing package functions are

 *	declared.

 */



#ifdef __cplusplus

extern "C" {

#endif



#ifdef __STDC__

extern void		ArgPError( char * );

extern char *		ArgQError( void );

extern int		ArgHelp( void );

extern int		ArgFullHelp( void );

extern int		ArgFeedback( void );

extern int		ArgRegister( void );

extern int		ArgVersion( void );

extern int		ArgParse( int, char *[ ], ArgCommand *, int, ArgOption *, int, ArgEquiv * );

extern int		ArgQNOpt( void );

extern int		ArgQOccurOpt( char *, int );

extern char *		ArgQOpt( int, int * );

extern int		ArgQNOccur( char * );

extern int		ArgQNValue( char *, int );

extern ArgValue *	ArgQValue( char *, int, int );

#else

extern void		ArgPError( );

extern char *		ArgQError( );

extern int		ArgHelp( );

extern int		ArgFullHelp( );

extern int		ArgFeedback( );

extern int		ArgRegister( );

extern int		ArgVersion( );

extern int		ArgParse( );

extern int		ArgQNOpt( );

extern int		ArgQOccurOpt( );

extern char *		ArgQOpt( );

extern int		ArgQNOccur( );

extern int		ArgQNValue( );

extern ArgValue *	ArgQValue( );

#endif



#ifdef __cplusplus

}

#endif





#endif /* __ARGH__ */

