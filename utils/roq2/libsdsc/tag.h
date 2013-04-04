/**
 **	$Header: /sdsc/dev/vis/misc/libsdsc/v3.0/libsdsc/src/include/RCS/tag.h,v 1.6 1995/06/29 00:11:14 bduggan Exp $
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
 **	tag.h		-  Tag table package include file
 **
 **  PROJECT
 **	libsdsc		-  SDSC standard function library
 **
 **  DESCRIPTION
 **	tag.h contains the structs, typedefs, externs, defines, and macros
 **	needed to use the Tag table subpackage of the SDSC library.
 **
 **  PUBLIC CONTENTS
 **			d =defined constant
 **			f =function
 **			m =defined macro
 **			t =typedef/struct/union
 **			v =variable
 **			? =other
 **
 **	__TAGH__	d  file inclusion flag
 **
 **	TagErrNo	v  error number
 **	TagNErr		v  number of error messages
 **	TagErrList	v  error messages
 **	TAGE...		d  error codes
 **
 **	TAGTABLENULL	d  NULL Tag Table
 **	TAGENTRYNULL	d  NULL Tag Entry
 **
 **	Tag...		f  Generic functions
 **	TagTable...	f  Tag table functions
 **	TagEntry...	f  Tag entry functions
 **
 **	TagTable	t  Tag Table
 **	TagEntry	t  Tag Table Entry
 **	TAGMAXLEN	d  Maximum tag length
 **
 **  PRIVATE CONTENTS
 **	none
 **
 **  HISTORY
 **	$Log: tag.h,v $
 **	Revision 1.6  1995/06/29  00:11:14  bduggan
 **	updated copyright
 **
 **	Revision 1.5  1994/10/03  16:37:52  nadeau
 **	Updated to ANSI C and C++ compatibility.
 **	Minimized use of custom SDSC types (e.g., uchar vs. unsigned char)
 **	Updated comments.
 **	Updated indenting on some code.
 **	Updated copyright message.
 **
 **	Revision 1.4  93/03/09  10:44:19  nadeau
 **	Corrected non-ANSI-ism of an #endif comment.
 **	
 **	Revision 1.3  92/09/02  15:24:05  vle
 **	Updated copyright notice.
 **	
 **	Revision 1.2  92/06/24  17:38:54  nadeau
 **	Added declaration for TagEntryDup().
 **	
 **	Revision 1.1  91/01/09  16:55:05  nadeau
 **	Initial revision
 **	
 **/


#ifndef __TAGH__
#define __TAGH__

#ifndef __SDSCH__
#include "sdsc.h"
#endif /* __SDSCH__ */





/*
 *  GLOBAL VARIABLE
 *	TagErrNo	-  error number
 *	TagNErr		-  number of error messages
 *	TagErrList	-  error messages
 *
 *  DESCRIPTION
 *	On an error, the tag package routines return -1, or NULL, and set
 *	TagErrNo to an error code.  The programmer may call TagPError
 *	to print the associated error message to stderr, or may do the
 *	message lookup in TagErrList directly.
 */

extern int   TagErrNo;			/* Current error number		*/
extern int   TagNErr;			/* Number of error messages	*/
extern char *TagErrList[];		/* List of error messages	*/





/*
 *  CONSTANTS
 *	TAGE...		-  error codes
 *
 *  DESCRIPTION
 *	TagErrNo may be set to these error codes as a result of an error in
 *	calling one of the tag package routines.
 */

#define TAGESYS			0	/* System error			*/
#define TAGEMALLOC		1	/* Cannot allocate		*/

#define TAGENULLTAGTABLE	2	/* NULL 'tagTable' argument	*/
#define TAGENULLTAGENTRY	3	/* NULL 'tagEntry' argument	*/
#define TAGENULLTAG		4	/* NULL 'tag' argument		*/
#define TAGEBADNTHENTRY		5	/* Bad 'nthEntry' argument	*/
#define TAGEBADNTHOCCUR		6	/* Bad 'nthOccur' argument	*/
#define TAGEUNKVALUETYPE	7	/* Bad 'valueType' argument	*/
#define TAGEUNKTAG		8	/* Unknown tag			*/
#define TAGENOTINTABLE		9	/* Entry isn't in a tag table	*/
			/*	10-2^32	   Reserved			*/





/*
 *  CONSTANTS
 *	TAGTABLENULL	-  NULL Tag Table
 *	TAGENTRYNULL	-  NULL Tag Entry
 *
 *  DESCRIPTION
 *	These values are each used when a NULL (empty) Tag Table, or Tag
 *	Entry is to be indicated.  These values may be returned as error
 *	condition values from functions that normally return Tag Tables,
 *	or Tag Entries.
 */

#define TAGTABLENULL	((TagTable *)0)
#define TAGENTRYNULL	((TagEntry *)0)




/*
 *  CONSTANT
 *	TAGMAXLEN		-  Maximum tag length
 *
 *  DESCRIPTION
 *	TAGMAXLEN is the maximum number of bytes allowed in a tag name in
 *	the tag table.  Tags longer than TAGMAXLEN will be silently
 *	tuncated when entered and compared.
 */

#define TAGMAXLEN	127





/*
 *  TYPEDEF & STRUCTURE
 *	TagTable		-  The top level tag table description
 *	TagHashEntry		-  An entry in a tag table
 *	TagEntry		-  An actual value in the tag table
 *
 *  DESCRIPTION
 *			*** PRIVATE DATA STRUCTURES ***
 *		Tag tables and tag entries should only be accessed
 *		via the provided routine set.  These data structures
 *		are subject to change without notice.
 *
 *	A tag table is a threaded hash table arranged so that it can be
 *	accessed "Directly" via hashing, or "Linearly" by following the
 *	threads.  An 'TagTable' structure describes such a table, giving
 *	its length, pointers to the start and end of the thread, and
 *	a pointer to a dynamically allocated hash table.
 *
 *	Each entry in the hash table is described by an 'TagHashEntry'
 *	structure.  This structure gives the tag string, a count of the
 *	number of tag-value pairs occurring in the table that use the tag,
 *	and a pointer off to the first such occurrence.  The strucutre also
 *	contains a pointer to the next in the hash table collision chain.
 *
 *	Each entry in the linear list, and the list of occurrences for a
 *	hashed tag, is an 'TagEntry' structure.  This structure keeps a
 *	copy of the tag (annoying, but unavoidable), an entry counter
 *	(which it is in the linear list), an occurrence counter (which it
 *	is in the occurrence list), a pointer to the hashed table entry,
 *	a pointer to the next in the linear order list, and a pointer to
 *	the next in the occurrence list.  The structure also holds the
 *	size of the value, in bytes, and the value itself.
 */

typedef struct TagTable		TagTable;
typedef struct TagHashEntry	TagHashEntry;
typedef struct TagEntry		TagEntry;


struct TagTable
{
	int           table_numEntries;	/* Number of entries in table	*/

	TagEntry     *table_linearFirst;/* First entry in linear list	*/
	TagEntry     *table_linearLast;	/* Last entry in linear list	*/

	int           table_tableLength;/* Table length			*/
	TagHashEntry *table_tableHash;	/* Hash table			*/
};


struct TagHashEntry
{
	char          hash_tag[TAGMAXLEN+1];/* Tag name			*/
	TagHashEntry *hash_collideNext;	/* Next in collision list	*/

	int           hash_numOccur;	/* Number of occurrences	*/
	TagEntry     *hash_occur;	/* List of occurrences of tag	*/
};

struct TagEntry
{
	char          entry_tag[TAGMAXLEN+1];/* Tag name		*/

	int           entry_nthEntry;	/* Entry counter		*/
	TagHashEntry *entry_hashEntry;	/* Parent tag table entry	*/
	TagEntry     *entry_linearNext;	/* Next in linear list		*/

	int           entry_nthOccur;	/* Occurrence counter		*/
	TagEntry     *entry_occurNext;	/* Next in occurrence list	*/

	int           entry_valueSize;	/* # of bytes in the value	*/
	int           entry_valueType;	/* Type for the value		*/
	unsigned char entry_value[2];	/* Rest of struct is value	*/
	/* ... dynamically allocated to contain the rest of value ...	*/
};





/*
 *  FUNCTIONS
 *	Tag...		-  Generic functions
 *	TagTag...	-  Tag Table functions
 *	TagEntry...	-  Tag Entry functions
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __STDC__
extern void		TagPError( char * );
extern char *		TagQError( void );

extern TagTable *	TagTableAlloc( void );
extern void		TagTableFree( TagTable * );

extern TagEntry *	TagEntryAlloc( char *, int, void * );
extern void		TagEntryFree( TagEntry * );
extern TagEntry *	TagEntryDup( TagEntry * );

extern int		TagTableAppend( TagTable *, TagEntry * );
extern int		TagTableInsert( TagTable *, int, TagEntry * );
extern int		TagTableDelete( TagTable *, int );
extern int		TagTableReplace( TagTable *, int, TagEntry * );
extern int		TagTableCopy( TagTable *, TagTable *, int, int );

extern int		TagTableQNEntry( TagTable *, char * );
extern TagEntry *	TagTableQLinear( TagTable *, int );
extern TagEntry *	TagTableQDirect( TagTable *, char *, int );

extern char *		TagEntryQTag( TagEntry * );
extern int		TagEntryQNthOccur( TagEntry * );
extern int		TagEntryQNthEntry( TagEntry * );
extern int		TagEntryQValueType( TagEntry * );
extern int		TagEntryQValue( TagEntry *, void * );
#else
extern void		TagPError( );
extern char *		TagQError( );

extern TagTable *	TagTableAlloc( );
extern void		TagTableFree( );

extern TagEntry *	TagEntryAlloc( );
extern void		TagEntryFree( );
extern TagEntry *	TagEntryDup( );

extern int		TagTableAppend( );
extern int		TagTableInsert( );
extern int		TagTableDelete( );
extern int		TagTableReplace( );
extern int		TagTableCopy( );

extern int		TagTableQNEntry( );
extern TagEntry *	TagTableQLinear( );
extern TagEntry *	TagTableQDirect( );

extern char *		TagEntryQTag( );
extern int		TagEntryQNthOccur( );
extern int		TagEntryQNthEntry( );
extern int		TagEntryQValueType( );
extern int		TagEntryQValue( );
#endif

#ifdef __cplusplus
}
#endif


#endif /* __TAGH__ */
