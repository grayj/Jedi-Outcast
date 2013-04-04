/**

 **	$Header: /roq/libsdsc/tag.c 1     11/02/99 4:39p Zaphod $

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



#define HEADER	"    $Header: /roq/libsdsc/tag.c 1     11/02/99 4:39p Zaphod $"



/**

 **  FILE

 **	tag.c		-  Tag table package

 **

 **  PROJECT

 **	libsdsc		-  SDSC Utility Library

 **

 **  DESCRIPTION

 **	This file contains source for the tag-value pair table functions.

 **	These functions build up and handle queries to a table of tag-value

 **	pairs, commonly used to hold information read in from an image file.

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **	TagErrNo	v  error number

 **	TagNNrr		v  number of error messages

 **	TagErrList	v  error messages

 **	TagPError	f  Print error message

 **	TagQError	f  Query error message

 **

 **	TagTableAlloc	f  allocate a new tag table

 **	TagTableFree	f  deallocate a tag table

 **	TagEntryAlloc	f  allocate a new tag table entry

 **	TagEntryFree	f  deallocate a tag table entry

 **	TagEntryDup	f  duplicate a tag table entry

 **

 **	TagTableAppend	f  append an entry to the tag table

 **	TagTableInsert	f  insert an entry into the tag table

 **	TagTableDelete	f  delete an entry from the tag table

 **	TagTableReplace	f  replace an entry in the tag table

 **	TagTableCopy	f  copy a range from a tag table

 **

 **	TagTableQNEntry	f  query the tag table's number of entries

 **	TagTableQLinear	f  query the tag table's linear list

 **	TagTableQDirect	f  query the tag hash table directly

 **

 **	TagEntryQTag		f  query the tag of an entry

 **	TagEntryQNthOccur	f  query the occurrence # of an entry

 **	TagEntryQNthEntry	f  query the entry # of an entry

 **	TagEntryQValueType	f  query the type of the value of an entry

 **	TagEntryQValue		f  query the value of an entry

 **

 **	TagTablePrint	f  print debug dump of tag table to stderr

 **	TagEntryPrint	f  print debug dump of tag entry to stderr

 **

 **  PRIVATE CONTENTS

 **	tagCacheTable	v  last tag table used

 **	tagCacheTag	v  last tag used

 **	tagCacheHash	v  last hash table entry used

 **	tagCacheOccur	v  last occurrence list entry used

 **

 **	tagHash		f  hash a tag to get its hash table index

 **	tagFind		f  find a tag's entry in the hash table

 **	tagAdd		f  add tag entry to hash table

 **

 **  HISTORY

 **	$Log: /roq/libsdsc/tag.c $
 * 
 * 1     11/02/99 4:39p Zaphod

 **	Revision 1.12  1995/06/30  21:53:40  bduggan

 **	fixed some out-of-range array references

 **

 **	Revision 1.11  1995/06/29  00:17:39  bduggan

 **	updated copyright

 **

 **	Revision 1.10  1995/04/21  17:58:10  bduggan

 **	Changed bcopy to memcpy.

 **	Replaced prototypes for standard functions with

 **	include files.

 **

 **	Revision 1.9  1994/11/01  21:31:46  bduggan

 **	Changed TagTableReplace.  If an entry is replacing another

 **	entry with the same tag, then the NthOccur field will now

 **	remain unchanged.

 **

 **	Revision 1.8  94/10/03  16:11:20  nadeau

 **	Updated to ANSI C and C++ compatibility.

 **	Removed all use of register keyword.

 **	Minimized use of custom SDSC types (e.g., uchar vs. unsigned char)

 **	Added forward declarations.

 **	Added misc. casts to passify SGI and DEC compilers.

 **	Updated comments.

 **	Updated indenting on some code.

 **	Updated copyright message.

 **	

 **	Revision 1.7  92/09/02  13:33:04  vle

 **	Updated copyright notice.

 **	

 **	Revision 1.6  92/06/24  17:39:08  nadeau

 **	Added TagEntryDup().

 **	

 **	Revision 1.5  91/10/03  13:11:40  nadeau

 **	Comment updates.

 **	

 **	Revision 1.4  91/01/09  16:31:38  nadeau

 **	Removed leading 'im' and 'Im' from routine and variable names.

 **	

 **	Revision 1.3  90/06/22  11:06:27  nadeau

 **	Added replace and copy functions.

 **	

 **	Revision 1.2  90/05/11  09:58:08  nadeau

 **	Major restructuring.  Changed terminology to 'tag table' instead of

 **	'tag list'.  Changed lots of routine names.  Changed internal data

 **	structures to have cleaner access.  Changed value query calls to deal

 **	with 'tag entries' (a tag, its value, and its position in the table).

 **	Added tag entry allocation and query calls.

 **	

 **	Revision 1.1  90/03/06  17:32:33  nadeau

 **	Initial revision

 **	

 **/

/*LINTLIBRARY*/



#include "sdsccopyright.h"

#include <stdio.h>

#include <ctype.h>

#include <string.h>

#include <malloc.h>

#include <stdlib.h>

#include <errno.h>

#include "sdsc.h"



#ifndef NULL

#define NULL	0

#endif





#define SIZEOF_TAGENTRY(valueSize)	(sizeof(TagEntry)-2+(valueSize))











/*

 *  GLOBAL VARIABLES

 *	tagCacheTable	-  last tag table used

 *	tagCacheTag	-  last tag used

 *	tagCacheHash	-  last hash table entry used

 *	tagCacheOccur	-  last occurrence list entry used

 *

 *  DESCRIPTION

 *	These globals represent a cache of the information for the last

 *	query made.  In typical use of the TagTableQ...() functions, a caller

 *	queries the number of occurrences of a tag, then proceeds to

 *	query each occurrence for the tag before moving on to the next

 *	tag.  To reduce the overhead of continually having to hash

 *	the tag, look it up in the table, follow a collision chain,

 *	and then follow an occurrence chain, the last used values are saved

 *	and used if the tag pair being queried is the same as last time.

 */



static TagTable     *tagCacheTable = NULL;	/* Last tag table used	*/

static char          tagCacheTag[TAGMAXLEN];	/* Last tag used	*/

static TagHashEntry *tagCacheHash  = NULL;	/* Last hash entry used	*/

static TagEntry     *tagCacheOccur = NULL;	/* Value for last tag	*/











/*

 *  FUNCTION DECLARATIONS

 */

#ifdef __STDC__



static int	tagHash( TagTable *, char * );

static TagHashEntry    *tagFind( TagTable *, char * );

static int	tagAdd( TagTable *, TagEntry * );



#else



static int      tagHash( );

static TagHashEntry    *tagFind( );

static int      tagAdd( );

#endif











/*

 *  GLOBAL VARIABLE

 *	TagErrNo	-  error number

 *	TagNNrr		-  number of error messages

 *	TagErrList	-  error messages

 *

 *  DESCRIPTION

 *	On an error, the tag table routines return -1 and set

 *	TagErrNo to an error code.  The programmer may call TagPError

 *	to print the associated error message to stderr, or may do the

 *	message lookup in TagErrList themselves.

 */

int TagErrNo = -1;		/* Tag package error number		*/

char *TagErrList[ ] =		/* Error message list			*/

{

	/*  0 IMESYS */		"System call error;  see errno",

	/*  1 IMEMALLOC */	"Cannot allocate host memory",



	/*  2 IMENULLTAGTABLE */"NULL tag table",

	/*  3 IMENULLTAGENTRY */"NULL tag entry",

	/*  4 IMENULLTAG */	"NULL or empty tag string",

	/*  5 IMEBADDNTHENTRY */"Nth entry count too large or less than zero",

	/*  6 IMEBADNTHOCCUR */	"Nth occurrence count too large or less than zero",

	/*  7 IMEUNKVALUETYPE */"Unknown value type",

	/*  8 IMEUNKTAG */	"Unknown tag;  tag not found in table",

	/*  9 IMENOTINTABLE */	"Tag entry isn't in a tag table yet",

};

int TagNErr = 10;		/* Number of error messages		*/











/*

 *  FUNCTION

 *	TagPError	-  Print error message

 *

 *  DESCRIPTION

 *	The error text associated with the current TagErrNo is printed

 *	to stderr, preceded by the given leader string.

 */



void				/* Returns nothing			*/

#ifdef __STDC__

TagPError( char *s )

#else

TagPError( s )

	char *s;		/* Leader string			*/

#endif

{

	if ( TagErrNo == TAGESYS )

		perror( s );

	else if ( TagErrNo < 0 || TagErrNo >= TagNErr )

		(void)fprintf( stderr, "Unknown error\n" );

	else if ( s && *s )

		(void)fprintf( stderr, "%s: %s\n", s, TagErrList[TagErrNo] );

	else

		(void)fprintf( stderr, "%s\n", TagErrList[TagErrNo] );

}











/*

 *  FUNCTION

 *	TagQError	-  Query error message

 *

 *  DESCRIPTION

 *	The error text associated with the current TagErrNo is returned.

 */



extern int errno;			/* System call error code	*/

extern int sys_nerr;			/* # of system call error codes	*/

extern char *sys_errlist[];		/* Error code message strings	*/



char *					/* Returns error text		*/

#ifdef __STDC__

TagQError( void )

#else

TagQError( )

#endif

{

	if ( TagErrNo == TAGESYS )

	{

		if ( errno < 0 || errno >= sys_nerr )

			return ( "Unknown error" );

		return ( sys_errlist[errno] );

	}

	if ( TagErrNo < 0 || TagErrNo >= TagNErr )

		return ( "Unknown error" );

	return ( TagErrList[TagErrNo] );

}











/*

 *  FUNCTION

 *	TagTableAlloc	-  allocate a new tag table

 *	TagTableFree	-  deallocate a tag table

 *

 *  DESCRIPTION

 *	TagTableAlloc() allocates a new TagTable struct and its hash table.

 *

 *	TagTableFree() walks the tag table and its collision lists, freeing

 *	each TagHashEntry and all its TagEntry's.  It then frees the

 *	hash table and the TagTable struct itself.

 *

 *	TAGTABLELENGTH is the default (only?) size for the tag table hash

 *	table.  This size is selected under the assumption that there are

 *	usually not a large number of different types of tags being delt

 *	with.  Instead, there are more often multiple values for each tag.

 *	Also, to keep the collision tables short, the hash table is

 *	intentionally overly large.

 */



#define TAGTABLELENGTH	30



TagTable *				/* Returns new tag table	*/

#ifdef __STDC__

TagTableAlloc( void )

#else

TagTableAlloc( )

#endif

{

	TagHashEntry *pHash;		/* Hash table pointer		*/

	TagTable     *newTable;		/* New tag table		*/



	if ( (newTable = (TagTable *)malloc( (unsigned int)sizeof( TagTable ))) == NULL )

	{

		TagErrNo = TAGEMALLOC;

		return ( TAGTABLENULL );

	}



	newTable->table_tableLength = TAGTABLELENGTH;

	if ( (newTable->table_tableHash = (TagHashEntry *)malloc(

		(unsigned int)(sizeof( TagHashEntry ) * TAGTABLELENGTH) )) == NULL )

	{

		free( (char *)newTable );

		TagErrNo = TAGEMALLOC;

		return ( TAGTABLENULL );

	}



	newTable->table_numEntries  = 0;

	newTable->table_linearFirst = NULL;

	newTable->table_linearLast  = NULL;



	pHash = &newTable->table_tableHash[newTable->table_tableLength - 1];

	do

	{

		pHash->hash_tag[0]      = '\0';

		pHash->hash_collideNext = NULL;

		pHash->hash_numOccur    = 0;

		pHash->hash_occur       = NULL;

	} while ( pHash-- != newTable->table_tableHash );



	return ( (TagTable *)newTable );

}



void					/* Returns nothing		*/

#ifdef __STDC__

TagTableFree( TagTable *tagTable )

#else

TagTableFree( tagTable )

	TagTable *tagTable;		/* Tag table to deallocate	*/

#endif

{

	int           i;		/* Counter			*/

	TagHashEntry *pHash;		/* Hash table pointer		*/

	TagEntry     *pOccur;		/* Occurrence list pointer	*/

	TagEntry     *pOccurPrev;	/* Previous in occurrence list	*/



	if ( tagTable == TAGTABLENULL )

		return;





	/* Walk the table.						*/

	for ( i = 0; i < tagTable->table_tableLength; i++ )

	{

		/* Walk the collision list				*/

		for ( pHash = &tagTable->table_tableHash[i]; pHash;

			pHash = pHash->hash_collideNext )

		{

			/* Walk the occurrence list			*/

			for ( pOccur = pHash->hash_occur; pOccur; )

			{

				pOccurPrev = pOccur;

				pOccur     = pOccur->entry_occurNext;

				free( (char *)pOccurPrev );

			}

		}

	}



	if ( tagCacheTable == tagTable )

		tagCacheTable = NULL;



	/* Free the hash table itself and the TagTable structure	*/

	free( (char *)tagTable->table_tableHash );

	free( (char *)tagTable );

}











/*

 *  FUNCTION

 *	TagEntryAlloc	-  allocate a new tag table entry

 *	TagEntryFree	-  deallocate a tag table entry

 *	TagEntryDup	-  duplicate a tag table entry

 *

 *  DESCRIPTION

 *	TagEntryAlloc() mallocs a new tag entry struct sufficiently large

 *	for the value bytes, then copies everything into it.

 *

 *	TagEntryFree() simply deallocates the entry's memory.  We assume

 *	(dangerous!!) that the caller is not freeing an entry already in a

 *	tag table.

 */



TagEntry *				/* Returns tag entry		*/

#ifdef __STDC__

TagEntryAlloc( char *tag, int valueType, void *pValue )

#else

TagEntryAlloc( tag, valueType, pValue )

	char           *tag;		/* New entry's tag		*/

	int	        valueType;	/* Type of value		*/

	unsigned char  *pValue;		/* Value pointer		*/

#endif

{

#ifdef __STDC__

	void	       *pVal;		/* Entry value pointer		*/

#else

	unsigned char  *pVal;		/* Entry value pointer		*/

#endif

	int             valueSize;	/* Number of bytes in value	*/

	TagEntry       *newEntry;	/* New tag entry structure	*/

	BinMachineInfo *binInfo;	/* Sizes of things from bin pkg	*/





	if ( tag == NULL || *tag == '\0' )

	{

		TagErrNo = TAGENULLTAG;

		return ( TAGENTRYNULL );

	}





	binInfo   = BinQMachine( );

	valueSize = binInfo->bin_typeSize[ valueType ];

	if ( valueSize == 0 )

	{

		TagErrNo = TAGEUNKVALUETYPE;

		return ( TAGENTRYNULL );

	}



	if ( (newEntry = (TagEntry *)malloc( (unsigned int)(SIZEOF_TAGENTRY(

		valueSize )) )) == NULL )

	{

		TagErrNo = TAGEMALLOC;

		return ( TAGENTRYNULL );

	}



	(void)strncpy( newEntry->entry_tag, tag, TAGMAXLEN );

	newEntry->entry_tag[TAGMAXLEN] = '\0';

	newEntry->entry_nthEntry   = -1;

	newEntry->entry_hashEntry  = NULL;

	newEntry->entry_linearNext = NULL;

	newEntry->entry_nthOccur   = -1;

	newEntry->entry_occurNext  = NULL;

	newEntry->entry_valueSize  = valueSize;

	newEntry->entry_valueType  = valueType;



	pVal = newEntry->entry_value;

	memcpy( (void*)pVal, (void*)pValue, valueSize);



	return ( (TagEntry *)newEntry );

}



void					/* Returns nothing		*/

#ifdef __STDC__

TagEntryFree( TagEntry *tagEntry )

#else

TagEntryFree( tagEntry )

	TagEntry *tagEntry;		/* Tag entry to deallocate	*/

#endif

{

	if ( tagEntry == TAGENTRYNULL )

		return;



	free( (char *)tagEntry );

}



TagEntry *				/* Returns tag entry		*/

#ifdef __STDC__

TagEntryDup( TagEntry *tagEntry )

#else

TagEntryDup( tagEntry )

	TagEntry       *tagEntry;	/* Tag entry structure to dup	*/

#endif

{

#ifdef __STDC__

	void  	       *pVal;		/* Entry value pointer		*/

	void 	       *pValue;		/* Entry value pointer		*/

#else

	unsigned char  *pVal;		/* Entry value pointer		*/

	unsigned char  *pValue;		/* Entry value pointer		*/

#endif

	int             valueSize;	/* Number of bytes in value	*/

	TagEntry       *newEntry;	/* New tag entry structure	*/





	if ( tagEntry == TAGENTRYNULL )

	{

		TagErrNo = TAGENULLTAGENTRY;

		return ( TAGENTRYNULL );

	}





	valueSize = tagEntry->entry_valueSize;

	pValue    = tagEntry->entry_value;

	if ( (newEntry = (TagEntry *)malloc( (unsigned int)(SIZEOF_TAGENTRY(

		valueSize )) )) == NULL )

	{

		TagErrNo = TAGEMALLOC;

		return ( TAGENTRYNULL );

	}



	(void)strncpy( newEntry->entry_tag, tagEntry->entry_tag, TAGMAXLEN );

	newEntry->entry_tag[TAGMAXLEN] = '\0';

	newEntry->entry_nthEntry   = -1;

	newEntry->entry_hashEntry  = NULL;

	newEntry->entry_linearNext = NULL;

	newEntry->entry_nthOccur   = -1;

	newEntry->entry_occurNext  = NULL;

	newEntry->entry_valueSize  = valueSize;

	newEntry->entry_valueType  = tagEntry->entry_valueType;



	pVal = newEntry->entry_value;

	memcpy( (void*)pVal, (void*)pValue, valueSize);



	return ( (TagEntry *)newEntry );

}











/*

 *  FUNCTION

 *	tagHash	-  hash a tag to get its hash table index

 *

 *  DESCRIPTION

 *	The tag's ASCII characters are added together and the result

 *	modulo the hash table size returned as the hash table index.

 *

 *	Tag names are essentially random, so there is little hash function

 *	optimization we could do here.

 */



static int				/* Returns hash index		*/

#ifdef __STDC__

tagHash( TagTable *tagTable, char *tag )

#else

tagHash( tagTable, tag )

	TagTable    *tagTable;		/* Tag table to hash in to	*/

	char        *tag;		/* Tag to hash			*/

#endif

{

	int          i = 0;		/* Hash index			*/



	while ( *tag )

		i += *tag++;

	return ( i % tagTable->table_tableLength );

}











/*

 *  FUNCTION

 *	tagFind	-  find a tag's entry in the hash table

 *

 *  DESCRIPTION

 *	The tag is hashed and looked up in the hash table.  The table's

 *	first entry, and its collision list, are searched until an entry

 *	is found that matches the tag.  A pointer to the entry found,

 *	or NULL if none is found, is returned.

 */



static TagHashEntry *		/* Returns table entry		*/

#ifdef __STDC__

tagFind( TagTable *tagTable, char *tag )

#else

tagFind( tagTable, tag )

	TagTable *tagTable;		/* Tag table to search		*/

	char       *tag;		/* Tag to find in table		*/

#endif

{

	TagHashEntry *pHash;		/* Tag entry found		*/



	pHash = &tagTable->table_tableHash[tagHash( tagTable, tag )];



	while ( pHash && strncmp( pHash->hash_tag, tag, TAGMAXLEN ) != 0 )

		pHash = pHash->hash_collideNext;

	return ( pHash );		/* Could be NULL		*/

}











/*

 *  FUNCTION

 *	tagAdd	-  add tag entry to hash table

 *

 *  DESCRIPTION

 *	The table is searched for the new tag.  If the tag is found to already

 *	be in the table, the new entry is just added to the end of its

 *	occurrence list.

 *

 *	Otherwise the tag is rehashed to get the top level hash table entry

 *	that we'd like to use for the new tag.  If that entry is already in

 *	use (by another tag, of course), then we have a collision.  A new

 *	table entry is allocated and linked to the start of the collision

 *	list.  In either case we initialize the table entry and make it point

 *	to the new tag entry as its first, and only, occurrence.

 */



static int				/* Returns status		*/

#ifdef __STDC__

tagAdd( TagTable *tagTable, TagEntry *tagEntry )

#else

tagAdd( tagTable, tagEntry )

	TagTable     *tagTable;		/* Tag table to append to	*/

	TagEntry     *tagEntry;		/* Entry to append		*/

#endif

{

	TagHashEntry *pHash;		/* Hash table entry		*/

	TagHashEntry *pHashFirst;	/* First Hash table entry	*/

	TagEntry     *pOccur;		/* Occurrence list pointer	*/





	/*

	 *  Hash the tag and check the entry in the hash table.

	 */

	if ( (pHash = tagFind( tagTable, tagEntry->entry_tag )) != NULL )

	{

		/*

		 *  Tag is in the hash table and has 1 or more occurrences.

		 *  Add the tagEntry to the end of the occurrence list.

		 */



		for ( pOccur = pHash->hash_occur; pOccur->entry_occurNext;

			pOccur = pOccur->entry_occurNext )

			;



		pOccur->entry_occurNext  = tagEntry;

		tagEntry->entry_nthOccur = pOccur->entry_nthOccur + 1;

		pHash->hash_numOccur++;

	}

	else

	{

		/*

		 *  Tag isn't in the hash table yet.  Find a place for it.

		 */

		pHash = &tagTable->table_tableHash[tagHash( tagTable,

			tagEntry->entry_tag )];



		if ( pHash->hash_numOccur != 0 )

		{

			/*

			 *  Hashed table entry is occupied.  Add a new table

			 *  entry to the front of the collision chain (if any).

			 */

			pHashFirst = pHash;

			if ( (pHash = (TagHashEntry *)malloc( (unsigned int)

				sizeof( TagHashEntry ))) == NULL )

			{

				TagErrNo = TAGEMALLOC;

				return ( -1 );

			}

			pHash->hash_collideNext = pHashFirst->hash_collideNext;

			pHashFirst->hash_collideNext = pHash;

		}



		(void)strncpy( pHash->hash_tag, tagEntry->entry_tag, TAGMAXLEN );

		pHash->hash_tag[TAGMAXLEN] = '\0';

		pHash->hash_numOccur      = 1;

		pHash->hash_occur         = tagEntry;

		tagEntry->entry_nthOccur  = 0;

	}



	tagEntry->entry_occurNext = NULL;

	tagEntry->entry_hashEntry = pHash;

	return ( 0 );

}











/*

 *  FUNCTION

 *	TagTableAppend	-  append an entry to the tag table

 *

 *  DESCRIPTION

 *	The entry is added to the hash table and appended to the linear

 *	entry list.

 */



int					/* Returns number of table entries*/

#ifdef __STDC__

TagTableAppend( TagTable *tagTable, TagEntry *tagEntry )

#else

TagTableAppend( tagTable, tagEntry )

	TagTable	     *tagTable;	/* Tag table to append to	*/

	TagEntry	     *tagEntry;	/* Entry to append		*/

#endif

{

	if ( tagTable == TAGTABLENULL )

	{

		TagErrNo = TAGENULLTAGTABLE;

		return ( -1 );

	}



	if ( tagEntry == TAGENTRYNULL )

	{

		TagErrNo = TAGENULLTAGENTRY;

		return ( -1 );

	}



	if ( tagAdd( tagTable, tagEntry ) == -1 )

		return ( -1 );		/* TagErrNo already set		*/



	if ( tagTable->table_linearFirst == NULL )

	{

		/* Add the entry as the first (and only) entry in the list.*/

		tagTable->table_linearFirst = tagEntry;

	}

	else

	{

		/* Add the entry as the new last entry in the list.	*/

		tagTable->table_linearLast->entry_linearNext = tagEntry;

	}



	tagTable->table_linearLast = tagEntry;

	tagEntry->entry_linearNext = NULL;

	tagEntry->entry_nthEntry   = tagTable->table_numEntries++;



	return ( tagTable->table_numEntries );

}











/*

 *  FUNCTION

 *	TagTableInsert	-  insert an entry into the tag table

 *

 *  DESCRIPTION

 *	The entry is added to the hash table and then inserted into the

 *	collision list.

 */



int					/* Returns number of table entries*/

#ifdef __STDC__

TagTableInsert( TagTable *tagTable, int afterNthEntry, TagEntry *tagEntry )

#else

TagTableInsert( tagTable, afterNthEntry, tagEntry )

	TagTable   *tagTable;		/* Tag table to insert into	*/

	int         afterNthEntry;	/* Where to insert after	*/

	TagEntry   *tagEntry;		/* Entry to insert		*/

#endif

{

	TagEntry *pLinear;		/* Linear list pointer		*/

	int       i;			/* Counter			*/





	if ( tagTable == TAGTABLENULL )

	{

		TagErrNo = TAGENULLTAGTABLE;

		return ( -1 );

	}



	if ( afterNthEntry < -1 || afterNthEntry >= tagTable->table_numEntries )

	{

		TagErrNo = TAGEBADNTHENTRY;

		return ( -1 );

	}



	if ( tagEntry == TAGENTRYNULL )

	{

		TagErrNo = TAGENULLTAGENTRY;

		return ( -1 );

	}



	if ( tagAdd( tagTable, tagEntry ) == -1 )

		return ( -1 );		/* TagErrNo already set		*/



	if ( tagTable->table_linearFirst == NULL )

	{

		/* Insert as the only entry in the list.		*/

		tagTable->table_linearFirst = tagEntry;

		tagTable->table_linearLast  = tagEntry;

		tagEntry->entry_linearNext  = NULL;

	}

	else if ( afterNthEntry == -1 )

	{

		/* Insert before the first element in the table.	*/

		tagEntry->entry_linearNext  = tagTable->table_linearFirst;

		tagTable->table_linearFirst = tagEntry;

	}

	else if ( afterNthEntry == tagTable->table_numEntries )

	{

		/* Insert at end.					*/

		tagTable->table_linearLast->entry_linearNext = tagEntry;

		tagTable->table_linearLast = tagEntry;

		tagEntry->entry_linearNext  = NULL;

	}

	else

	{

		/* Insert somewhere in the middle.			*/

		for ( pLinear = tagTable->table_linearFirst, i = 0;

			i != afterNthEntry; i++ )

			pLinear = pLinear->entry_linearNext;

		tagEntry->entry_linearNext = pLinear->entry_linearNext;

		pLinear->entry_linearNext  = tagEntry;

	}



	tagEntry->entry_nthEntry = afterNthEntry + 1;

	tagTable->table_numEntries++;





	/*

	 *  Walk the table from the insert point on down, adding one to

	 *  the tag number in each entry.  Keeping such a number avoids

	 *  having to walk the table on each TagTableQDirect() call (the most

	 *  used call in this set).

	 */

	for ( pLinear = tagEntry->entry_linearNext; pLinear;

		pLinear = pLinear->entry_linearNext )

		pLinear->entry_nthEntry++;



	return ( tagTable->table_numEntries );

}











/*

 *  FUNCTION

 *	TagTableDelete	-  delete an entry from the tag table

 *

 *  DESCRIPTION

 *	The linear list is first scanned to find, and unlink, the entry to

 *	delete.  All entries following it on the list have their entry numbers

 *	decremented by 1.

 *

 *	The hash table is then searched to find the top level hash table

 *	entry that uses the entry we wish to delete.  If this is the only

 *	occurrence for that hash table entry, then the entry is removed.

 *

 *	Otherwise the occurrence list is searched and the entry unlinked from

 *	it.  All entries following it on the occurrence list have their

 *	occurrence numbers decremented by 1.

 */



int					/* Returns number of table entries*/

#ifdef __STDC__

TagTableDelete( TagTable *tagTable, int nthEntry )

#else

TagTableDelete( tagTable, nthEntry )

	TagTable   *tagTable;		/* Tag table to delete from	*/

	int         nthEntry;		/* Entry to delete		*/

#endif

{

	TagHashEntry  *pHash;		/* Hash table entry		*/

	TagEntry      *pLinear;		/* Linear list pointer		*/

	TagEntry      *pLinearPrev;	/* Previous in linear list	*/

	TagEntry      *pLinearTmp;	/* Tmp linear list pointer	*/

	TagEntry      *pOccur;		/* Occurrence list pointer	*/

	TagEntry      *pOccurPrev;	/* Previous in occur list	*/

	TagHashEntry  *pHashFirst;	/* First hash table entry	*/

	TagHashEntry  *pHashPrev;	/* Previous in collision list	*/

	int            i;		/* Counter			*/



	if ( tagTable == TAGTABLENULL )

	{

		TagErrNo = TAGENULLTAGTABLE;

		return ( -1 );

	}





	if ( nthEntry <= -1 || nthEntry >= tagTable->table_numEntries )

	{

		TagErrNo = TAGEBADNTHENTRY;

		return ( -1 );

	}





	/*

	 *  Walk the linear list until we find the entry we want to blow away.

	 */

	if ( nthEntry == 0 )

	{

		/*

		 *  Delete the 1st entry on the linear list.

		 */

		pLinear = tagTable->table_linearFirst;

		tagTable->table_linearFirst = pLinear->entry_linearNext;

		if ( tagTable->table_linearLast == pLinear )

			tagTable->table_linearLast = NULL;

	}

	else

	{

		for ( pLinear = tagTable->table_linearFirst, i = 0;

			i != nthEntry; i++ )

		{

			pLinearPrev = pLinear;

			pLinear     = pLinear->entry_linearNext;

		}

		pLinearPrev->entry_linearNext = pLinear->entry_linearNext;

		if ( tagTable->table_linearLast == pLinear )

			tagTable->table_linearLast = pLinearPrev;

	}



	tagTable->table_numEntries--;



	/*

	 *  Walk the rest of the linear list and reduce by one each of their

	 *  entry numbers.

	 */

	for ( pLinearTmp = pLinear->entry_linearNext; pLinearTmp;

		pLinearTmp = pLinearTmp->entry_linearNext )

		pLinearTmp->entry_nthEntry--;





	/*

	 *  Remove the entry from the occurrence list of the hash entry.

	 */

	pHash      = pLinear->entry_hashEntry;

	pHashFirst = &tagTable->table_tableHash[tagHash( tagTable,

		pHash->hash_tag )];



	if ( pHash->hash_occur == pLinear )

	{

		/*

		 *  Entry to delete from the occurrence list is the

		 *  first entry in that list.  Easy.

		 */

		pHash->hash_occur = pLinear->entry_occurNext;

	}

	else

	{

		/*

		 *  Entry to delete is somewhere in the middle of the occurrence

		 *  list.  Walk the list.

		 */



		for ( pOccurPrev = NULL, pOccur = pHash->hash_occur;

			pOccur != pLinear; pOccur = pOccur->entry_occurNext )

			pOccurPrev = pOccur;



		pOccurPrev->entry_occurNext = pLinear->entry_occurNext;

	}

	pHash->hash_numOccur--;



	/*

	 *  Walk the rest of the occurrence list and reduce by one each of

	 *  their occurrence numbers.

	 */

	for ( pOccur = pLinear->entry_occurNext; pOccur;

		pOccur = pOccur->entry_occurNext )

		pOccur->entry_nthOccur--;





	/*

	 *  Clean up.

	 */

	if ( pHash->hash_numOccur == 0 )

	{

		/*

		 *  Deleting this entry brought the occurrence count to 0

		 *  for this hash table entry.  Remove it from the hash table

		 *  or the collision list.

		 */

		if ( pHash == pHashFirst )

		{

			/*

			 *  This entry is in the actual hash table, not in

			 *  a collision chain.  If there is a collision chain,

			 *  move the first collision entry up into the hash

			 *  table proper.  Otherwise just null out this hash

			 *  table entry.

			 */

			if ( pHash->hash_collideNext == NULL )

			{

				pHash->hash_tag[0] = '\0';

				pHash->hash_occur  = NULL;

			}

			else

			{

				pHash = pHash->hash_collideNext;

				(void)strncpy( pHashFirst->hash_tag, pHash->hash_tag, TAGMAXLEN );

				pHashFirst->hash_tag[TAGMAXLEN] = '\0';

				pHashFirst->hash_collideNext = pHash->hash_collideNext;

				pHashFirst->hash_numOccur= pHash->hash_numOccur;

				pHashFirst->hash_occur   = pHash->hash_occur;



				for ( pOccur = pHashFirst->hash_occur; pOccur;

					pOccur = pOccur->entry_occurNext )

					pOccur->entry_hashEntry = pHashFirst;

				free( (char *)pHash );

			}

		}

		else

		{

			/*

			 *  This entry is in the collision list.  Walk the

			 *  list and remove it.

			 */

			for ( pHashPrev = pHashFirst;

				pHashPrev->hash_collideNext != pHash;

				pHashPrev = pHashPrev->hash_collideNext )

				;

			pHashPrev->hash_collideNext = pHash->hash_collideNext;

			free( (char *)pHash );

		}

	}



	if ( tagTable == tagCacheTable )

		tagCacheTable = NULL;



	free( (char *)pLinear );



	return ( tagTable->table_numEntries );

}











/*

 *  FUNCTION

 *	TagTableReplace	-  replace an entry in the tag table

 *

 *  DESCRIPTION

 *	The linear list is first scanned to find the entry to be replaced.

 *	The old entry is unlinked and the new entry put in its place.

 *

 *	The hash table is then searched to find the top level hash table

 *	entry that uses the entry we wish to delete.  If this is the only

 *	occurrence for that hash table entry, then the entry is removed.

 *

 *	Otherwise the occurrence list is searched and the entry unlinked from

 *	it.  All entries following it on the occurrence list have their

 *	occurrence numbers decremented by 1.

 *

 *	The new entry is then added back into the table.

 */



int					/* Returns number of table entries*/

#ifdef __STDC__

TagTableReplace( TagTable *tagTable, int nthEntry, TagEntry *tagEntry )

#else

TagTableReplace( tagTable, nthEntry, tagEntry )

	TagTable   *tagTable;		/* Tag table to delete from	*/

	int         nthEntry;		/* Entry to delete		*/

	TagEntry   *tagEntry;		/* New entry			*/

#endif

{

	TagHashEntry  *pHash;		/* Hash table entry		*/

	TagEntry      *pLinear;		/* Linear list pointer		*/

	TagEntry      *pLinearPrev;	/* Previous in linear list	*/

	TagEntry      *pOccur;		/* Occurrence list pointer	*/

	TagEntry      *pOccurPrev;	/* Previous in occur list	*/

	TagHashEntry  *pHashFirst;	/* First hash table entry	*/

	TagHashEntry  *pHashPrev;	/* Previous in collision list	*/

	int            i;		/* Counter			*/



	if ( tagTable == TAGTABLENULL )

	{

		TagErrNo = TAGENULLTAGTABLE;

		return ( -1 );

	}





	if ( nthEntry <= -1 || nthEntry >= tagTable->table_numEntries )

	{

		TagErrNo = TAGEBADNTHENTRY;

		return ( -1 );

	}



	if ( tagEntry == TAGENTRYNULL )

	{

		TagErrNo = TAGENULLTAGENTRY;

		return ( -1 );

	}







	/*

	 *  Walk the linear list until we find the entry we want to replace.

	 */

	if ( nthEntry == 0 )

	{

		/*

		 *  Replace the 1st entry on the linear list.

		 */

		pLinear = tagTable->table_linearFirst;

		tagTable->table_linearFirst = tagEntry;

		tagEntry->entry_linearNext  = pLinear->entry_linearNext;

		if ( tagTable->table_linearLast == pLinear )

			tagTable->table_linearLast = tagEntry;

	}

	else

	{

		for ( pLinear = tagTable->table_linearFirst, i = 0;

			i != nthEntry; i++ )

		{

			pLinearPrev = pLinear;

			pLinear     = pLinear->entry_linearNext;

		}

		pLinearPrev->entry_linearNext = tagEntry;

		tagEntry->entry_linearNext    = pLinear->entry_linearNext;

		if ( tagTable->table_linearLast == pLinear )

			tagTable->table_linearLast = tagEntry;

	}

	tagEntry->entry_nthEntry = pLinear->entry_nthEntry;



	/*

	 * Check to see if we need to put this entry into a new occurence list, 

	 * or if we can leave it in it's current occurence list.

	 */



	if ( strcmp( pLinear->entry_tag, tagEntry->entry_tag) == 0)

	{ 	/* 

		 * Replace the entry in the occurence list 

		 */

		pHash      = pLinear->entry_hashEntry;

	

		if ( pHash->hash_occur == pLinear )

		{

			/*

			 *  Entry to replace in the occurrence list is the

			 *  first entry in that list.  

			 */

			pHash->hash_occur = tagEntry;

			tagEntry->entry_occurNext = pLinear->entry_occurNext;

		}

		else

		{

			/*

			 *  Entry to replace is somewhere in the middle of the occurrence

			 *  list.  Walk the list.

			 */

			for ( pOccurPrev = NULL, pOccur = pHash->hash_occur;

				pOccur != pLinear; pOccur = pOccur->entry_occurNext )

				pOccurPrev = pOccur;

			pOccurPrev->entry_occurNext = tagEntry;

			tagEntry->entry_occurNext = pLinear->entry_occurNext;

		}

		tagEntry->entry_nthOccur = pLinear->entry_nthOccur;

		tagEntry->entry_hashEntry = pLinear->entry_hashEntry;

	}

	else

	{ 	/* 

		 *  Move the entry to a new occurence list.

		 *

		 *  First, remove the entry from the occurrence list of the hash entry.

		 */



		pHash      = pLinear->entry_hashEntry;

		pHashFirst = &tagTable->table_tableHash[tagHash( tagTable,

			pHash->hash_tag )];



		if ( pHash->hash_occur == pLinear )

		{

			/*

			 *  Entry to delete from the occurrence list is the

			 *  first entry in that list.  Easy.

			 */



			pHash->hash_occur = pLinear->entry_occurNext;

		}

		else

		{

			/*

			 *  Entry to delete is somewhere in the middle of the occurrence

			 *  list.  Walk the list.

			 */



			for ( pOccurPrev = NULL, pOccur = pHash->hash_occur;

				pOccur != pLinear; pOccur = pOccur->entry_occurNext )

				pOccurPrev = pOccur;



			pOccurPrev->entry_occurNext = pLinear->entry_occurNext;

		}

		pHash->hash_numOccur--;



		/*

		 *  Walk the rest of the occurrence list and reduce by one each of

		 *  their occurrence numbers.

		 */

		for ( pOccur = pLinear->entry_occurNext; pOccur;

			pOccur = pOccur->entry_occurNext )

			pOccur->entry_nthOccur--;





		/*

		 *  Clean up.

		 */

		if ( pHash->hash_numOccur == 0 )

		{

			/*

			 *  Deleting this entry brought the occurrence count to 0

			 *  for this hash table entry.  Remove it from the hash table

			 *  or the collision list.

			 */

			if ( pHash == pHashFirst )

			{

				/*

				 *  This entry is in the actual hash table, not in

				 *  a collision chain.  If there is a collision chain,

				 *  move the first collision entry up into the hash

				 *  table proper.  Otherwise just null out this hash

				 *  table entry.

				 */

				if ( pHash->hash_collideNext == NULL )

				{

					pHash->hash_tag[0] = '\0';

					pHash->hash_occur  = NULL;

				}

				else

				{

					pHash = pHash->hash_collideNext;

					(void)strncpy( pHashFirst->hash_tag, pHash->hash_tag, TAGMAXLEN );

					pHashFirst->hash_tag[TAGMAXLEN] = '\0';

					pHashFirst->hash_collideNext = pHash->hash_collideNext;

					pHashFirst->hash_numOccur= pHash->hash_numOccur;

					pHashFirst->hash_occur   = pHash->hash_occur;



					for ( pOccur = pHashFirst->hash_occur; pOccur;

						pOccur = pOccur->entry_occurNext )

						pOccur->entry_hashEntry = pHashFirst;

					free( (char *)pHash );

				}

			}

			else

			{

				/*

				 *  This entry is in the collision list.  Walk the

				 *  list and remove it.

				 */

				for ( pHashPrev = pHashFirst;

					pHashPrev->hash_collideNext != pHash;

					pHashPrev = pHashPrev->hash_collideNext )

					;

				pHashPrev->hash_collideNext = pHash->hash_collideNext;

				free( (char *)pHash );

			}

		}





		/*

		 *  Hash the entry and added it to the end of another occurrence list.

		 */

		if ( tagAdd( tagTable, tagEntry ) == -1 )

			return ( -1 );		/* TagErrNo already set		*/

	}





	if ( tagTable == tagCacheTable )

		tagCacheTable = NULL;



	free( (char *)pLinear );



	return ( tagTable->table_numEntries );

}











/*

 *  FUNCTION

 *	TagTableCopy	-  copy a range from a tag table

 *

 *  DESCRIPTION

 *	The source table is walked and each entry in the range duplicated

 *	and added to the destination table.

 */



int					/* Returns number of table entries*/

#ifdef __STDC__

TagTableCopy( TagTable *srcTable, TagTable *dstTable,

	int firstEntry, int lastEntry )

#else

TagTableCopy( srcTable, dstTable, firstEntry, lastEntry )

	TagTable *srcTable;		/* Table to copy		*/

	TagTable *dstTable;		/* Where to copy to		*/

	int firstEntry;			/* 1st entry to copy		*/

	int lastEntry;			/* last entry to copy		*/

#endif

{

	int i;				/* Entry counter		*/

	TagEntry *srcEntry;		/* Table entry			*/

	TagEntry *dstEntry;		/* New table entry		*/



	if ( srcTable == TAGTABLENULL || dstTable == TAGTABLENULL )

	{

		TagErrNo = TAGENULLTAGTABLE;

		return ( -1 );

	}

	if ( firstEntry <= -1 || firstEntry >= srcTable->table_numEntries )

	{

		TagErrNo = TAGEBADNTHENTRY;

		return ( -1 );

	}

	if ( lastEntry <= -1 || lastEntry >= srcTable->table_numEntries )

	{

		TagErrNo = TAGEBADNTHENTRY;

		return ( -1 );

	}

	if ( firstEntry > lastEntry )

	{

		i          = firstEntry;

		firstEntry = lastEntry;

		lastEntry  = i;

	}





	/*

	 *  Skip to the first entry to copy.

	 */

	for ( srcEntry = srcTable->table_linearFirst, i = 0;

		i != firstEntry; i++, srcEntry = srcEntry->entry_linearNext )

		;





	/*

	 *  Copy all entries until the last one to copy.

	 */

	for ( ; i <= lastEntry; i++, srcEntry = srcEntry->entry_linearNext )

	{

		dstEntry = (TagEntry *)TagEntryAlloc( srcEntry->entry_tag,

			srcEntry->entry_valueType, srcEntry->entry_value );

		if ( TagTableAppend( dstTable, dstEntry ) == -1 )

			return ( -1 );		/* TagErrNo already set	*/

	}



	return ( lastEntry - firstEntry + 1 );

}











/*

 *  FUNCTION

 *	TagTableQNEntry	-  query the tag table's number of entries

 *

 *  DESCRIPTION

 *	If the tag is NULL, the table's total number of entries is returned.

 *

 *	Otherwise we lookup the tag in the hash table, and return the number

 *	of occurrences of that tag.

 *

 *	The cached globals are checked and updated to reduce the number of

 *	times we have to look up the same tag and occurrence of that tag.

 *	tagFind() is an expensive call.

 */



int					/* Returns number of table entries*/

#ifdef __STDC__

TagTableQNEntry( TagTable *tagTable, char *tag )

#else

TagTableQNEntry( tagTable, tag )

	TagTable *tagTable;		/* Table to query		*/

	char       *tag;		/* Possible tag			*/

#endif

{

	TagHashEntry *pHash;		/* Hash table entry		*/



	if ( tagTable == TAGTABLENULL )

	{

		TagErrNo = TAGENULLTAGTABLE;

		return ( -1 );

	}



	if ( tag == NULL || *tag == '\0' )

		return ( tagTable->table_numEntries );



	if ( tagCacheTable == tagTable && tagCacheHash != NULL &&

		strncmp( tagCacheTag, tag, TAGMAXLEN ) == 0 )

		return ( tagCacheHash->hash_numOccur );



	if ( (pHash = tagFind( tagTable, tag )) == NULL )

		return ( 0 );		/* No entries with this tag	*/



	tagCacheTable = tagTable;

	(void)strncpy( tagCacheTag, tag, TAGMAXLEN );

	tagCacheTag[TAGMAXLEN-1] = '\0';

	tagCacheHash     = pHash;

	tagCacheOccur    = pHash->hash_occur;



	return ( pHash->hash_numOccur );

}











/*

 *  FUNCTION

 *	TagTableQLinear	-  query the tag table's linear list

 *

 *  DESCRIPTION

 *	The linear list is walked until we get to the nthEntry.  The cache

 *	is updated and the entry is returned.

 */



TagEntry *				/* Returns entry found		*/

#ifdef __STDC__

TagTableQLinear( TagTable *tagTable, int nthEntry )

#else

TagTableQLinear( tagTable, nthEntry )

	TagTable *tagTable;		/* Table to query		*/

	int       nthEntry;		/* Which entry			*/

#endif

{

	TagEntry *pLinear;		/* Linear list pointer		*/

	int       i;			/* Counter			*/



	if ( tagTable == TAGTABLENULL )

	{

		TagErrNo = TAGENULLTAGTABLE;

		return ( TAGENTRYNULL );

	}



	if ( nthEntry < 0 || nthEntry >= tagTable->table_numEntries )

	{

		TagErrNo = TAGEBADNTHENTRY;

		return ( TAGENTRYNULL );

	}



	if ( tagCacheTable == tagTable && tagCacheHash != NULL &&

		tagCacheOccur->entry_nthEntry <= nthEntry )

	{

		/*  Walk starting at last cached entry.			*/

		for ( pLinear = tagCacheOccur, i = pLinear->entry_nthEntry;

			i < nthEntry; i++, pLinear = pLinear->entry_linearNext )

			;

	}

	else

	{

		/*  Walk starting at first in linear list.		*/

		for ( i = 0, pLinear = tagTable->table_linearFirst;

			i < nthEntry; i++, pLinear = pLinear->entry_linearNext )

			;

	}



	tagCacheTable  = tagTable;

	tagCacheOccur  = pLinear;

	tagCacheHash   = pLinear->entry_hashEntry;

	(void)strncpy( tagCacheTag, tagCacheHash->hash_tag, TAGMAXLEN );

	tagCacheTag[TAGMAXLEN-1] = '\0';



	return ( (TagEntry *)pLinear );

}











/*

 *  FUNCTION

 *	TagTableQDirect	-  query the tag hash table directly

 *

 *  DESCRIPTION

 *	The tag is looked up in the hash table and its occurrence list

 *	walked to find the occurrence requested.

 *

 *	The cache is used and updated in order to minimize the hash table

 *	lookups and list walking done.

 */



TagEntry *				/* Returns entry found		*/

#ifdef __STDC__

TagTableQDirect( TagTable *tagTable, char *tag, int nthOccur )

#else

TagTableQDirect( tagTable, tag, nthOccur )

	TagTable     *tagTable;		/* Table to query		*/

	char         *tag;		/* Tag to look up		*/

	int           nthOccur;		/* Which occurrence		*/

#endif

{

	TagEntry     *pOccur;		/* Occurrence list pointer	*/

	TagHashEntry *pHash;		/* Hash table entry		*/





	if ( tagTable == TAGTABLENULL )

	{

		TagErrNo = TAGENULLTAGTABLE;

		return ( TAGENTRYNULL );

	}



	if ( tag == NULL || *tag == '\0' )

	{

		TagErrNo = TAGENULLTAG;

		return ( TAGENTRYNULL );

	}



	if ( tagCacheTable == tagTable && tagCacheHash != NULL &&

		strncmp( tagCacheTag, tag, TAGMAXLEN ) == 0 )

		pHash = tagCacheHash;

	else

	{

		if ( (pHash = tagFind( tagTable, tag )) == NULL )

		{

			TagErrNo = TAGEUNKTAG;

			return ( TAGENTRYNULL );

		}

		tagCacheTable = tagTable;

		(void)strncpy( tagCacheTag, tag, TAGMAXLEN );

		tagCacheTag[TAGMAXLEN-1] = '\0';

		tagCacheHash     = pHash;

		tagCacheOccur    = pHash->hash_occur;

	}



	if ( nthOccur < 0 || nthOccur >= pHash->hash_numOccur )

	{

		TagErrNo = TAGEBADNTHOCCUR;

		return ( TAGENTRYNULL );

	}



	if ( tagCacheOccur->entry_nthOccur <= nthOccur )

	{

		/* Walk occurrence list from cached one on.		*/

		for ( pOccur = tagCacheOccur; pOccur->entry_nthOccur != nthOccur;

			pOccur = pOccur->entry_occurNext )

			;

	}

	else

	{

		for ( pOccur = pHash->hash_occur;

			pOccur->entry_nthOccur != nthOccur;

			pOccur = pOccur->entry_occurNext )

			;

	}

	tagCacheOccur = pOccur;



	return ( pOccur );

}











/*

 *  FUNCTION

 *	TagEntryQTag		-  query the tag of an entry

 *	TagEntryQNthOccur	-  query the occurrence # of an entry

 *	TagEntryQNthEntry	-  query the entry # of an entry

 *	TagEntryQValueType	-  query the type of the value of an entry

 *	TagEntryQValue		-  query the value of an entry

 *

 *  DESCRIPTION

 *	Each of these calls returns the appropriate field from the tagEntry

 *	structure.

 *

 *	TagEntryQValue() is a bit more complex.  It copies each of the

 *	bytes in the value into consequetive bytes pointed to by the incomming

 *	value pointer.  It is assumed (hoped) that the caller has given us a

 *	pointer to something of the appropriate size.

 */



char *					/* Returns tag string		*/

#ifdef __STDC__

TagEntryQTag( TagEntry *tagEntry )

#else

TagEntryQTag( tagEntry )

	TagEntry *tagEntry;		/* Entry to query		*/

#endif

{

	if ( tagEntry == TAGENTRYNULL )

	{

		TagErrNo = TAGENULLTAGENTRY;

		return ( NULL );

	}



	return ( tagEntry->entry_tag );

}



int					/* Returns occurrence number	*/

#ifdef __STDC__

TagEntryQNthOccur( TagEntry *tagEntry )

#else

TagEntryQNthOccur( tagEntry )

	TagEntry *tagEntry;		/* Entry to query		*/

#endif

{

	if ( tagEntry == TAGENTRYNULL )

	{

		TagErrNo = TAGENULLTAGENTRY;

		return ( -1 );

	}

	if ( tagEntry->entry_nthOccur == -1 )

	{

		TagErrNo = TAGENOTINTABLE;

		return ( -1 );

	}



	return ( tagEntry->entry_nthOccur );

}



int					/* Returns entry number		*/

#ifdef __STDC__

TagEntryQNthEntry( TagEntry *tagEntry )

#else

TagEntryQNthEntry( tagEntry )

	TagEntry *tagEntry;		/* Entry to query		*/

#endif

{

	if ( tagEntry == TAGENTRYNULL )

	{

		TagErrNo = TAGENULLTAGENTRY;

		return ( -1 );

	}

	if ( tagEntry->entry_nthEntry == -1 )

	{

		TagErrNo = TAGENOTINTABLE;

		return ( -1 );

	}



	return ( tagEntry->entry_nthEntry );

}



int					/* Returns value's type		*/

#ifdef __STDC__

TagEntryQValueType( TagEntry *tagEntry )

#else

TagEntryQValueType( tagEntry )

	TagEntry *tagEntry;		/* Entry to query		*/

#endif

{

	if ( tagEntry == TAGENTRYNULL )

	{

		TagErrNo = TAGENULLTAGENTRY;

		return ( -1 );

	}



	return ( tagEntry->entry_valueType );

}



int					/* Returns status		*/

#ifdef __STDC__

TagEntryQValue( TagEntry *tagEntry, void *pValue )

#else

TagEntryQValue( tagEntry, pValue )

	TagEntry *tagEntry;		/* Entry to query		*/

	unsigned char *pValue;		/* Returns value		*/

#endif

{

	int valueSize;			/* Number of bytes to copy	*/

#ifdef __STDC__

	void          *pVal;		/* Pointer to actual value	*/

#else

	unsigned char *pVal;		/* Pointer to actual value	*/

#endif



	if ( tagEntry == TAGENTRYNULL )

	{

		TagErrNo = TAGENULLTAGENTRY;

		return ( -1 );

	}



	valueSize = tagEntry->entry_valueSize;

	pVal = tagEntry->entry_value;

	memcpy( (void*)pValue, (void*)pVal, valueSize);

	return ( 0 );

}









/*

 *  FUNCTION

 *	TagEntryPrint	-  print a debug dump of a tag entry to stderr

 *	TagTablePrint	-  print a debug dump of a tag table to stderr

 *

 *  DESCRIPTION

 *	Both of these routines are intended for use during the debugging

 *	of applications using tag tables.  The routines print the user-

 *	accessible contents of these data structures to stderr.  Internal

 *	pointers and such are not printed.

 */



int					/* Returns status		*/

#ifdef __STDC__

TagEntryPrint( TagEntry *tagEntry )

#else

TagEntryPrint( tagEntry )

	TagEntry *tagEntry;		/* Entry to print		*/

#endif

{

	int i;				/* Counter			*/



	if ( tagEntry == TAGENTRYNULL )

	{

		TagErrNo = TAGENULLTAGENTRY;

		return ( -1 );

	}



	(void)fprintf( stderr, "  tag        = '%s'\n", tagEntry->entry_tag );

	(void)fprintf( stderr, "  nth entry  = %d\n", tagEntry->entry_nthEntry );

	(void)fprintf( stderr, "  nth occur  = %d\n", tagEntry->entry_nthOccur );

	(void)fprintf( stderr, "  value type = " );

	switch ( tagEntry->entry_valueType )

	{

	case POINTER:	(void)fprintf( stderr, "POINTER\n" ); break;

	case CHAR:	(void)fprintf( stderr, "CHAR\n" ); break;

	case UCHAR:	(void)fprintf( stderr, "UCHAR\n" ); break;

	case SHORT:	(void)fprintf( stderr, "SHORT\n" ); break;

	case LONG:	(void)fprintf( stderr, "LONG\n" ); break;

	case INT:	(void)fprintf( stderr, "INT\n" ); break;

	case INT8:	(void)fprintf( stderr, "INT8\n" ); break;

	case INT16:	(void)fprintf( stderr, "INT16\n" ); break;

	case INT32:	(void)fprintf( stderr, "INT32\n" ); break;

	case USHORT:	(void)fprintf( stderr, "USHORT\n" ); break;

	case ULONG:	(void)fprintf( stderr, "ULONG\n" ); break;

	case UINT:	(void)fprintf( stderr, "UINT\n" ); break;

	case UINT8:	(void)fprintf( stderr, "UINT8\n" ); break;

	case UINT16:	(void)fprintf( stderr, "UINT16\n" ); break;

	case UINT32:	(void)fprintf( stderr, "UINT32\n" ); break;

	case BOOLEAN:	(void)fprintf( stderr, "BOOLEAN\n" ); break;

	case FLOAT:	(void)fprintf( stderr, "FLOAT\n" ); break;

	case DOUBLE:	(void)fprintf( stderr, "DOUBLE\n" ); break;

	case INT64:	(void)fprintf( stderr, "INT64\n" ); break;

	case UINT64:	(void)fprintf( stderr, "UINT64\n" ); break;

	default:	(void)fprintf( stderr, "unknown!\n" ); break;

	}



	(void)fprintf( stderr, "  value =      " );

	for ( i = 0; i < tagEntry->entry_valueSize; i++ )

		(void)fprintf( stderr, "0x%016lx ", (unsigned long)tagEntry->entry_value[i] );

	(void)fprintf( stderr, "\n" );



	return ( 0 );

}



int					/* Returns status		*/

#ifdef __STDC__

TagTablePrint( TagTable *tagTable )

#else

TagTablePrint( tagTable )

	TagTable *tagTable;		/* Table to print		*/

#endif

{

	int i;				/* Counter			*/

	TagEntry *pLinear;		/* Linear list pointer		*/



	if ( tagTable == TAGTABLENULL )

	{

		TagErrNo = TAGENULLTAGTABLE;

		return ( -1 );

	}



	for ( i = 0, pLinear = tagTable->table_linearFirst;

		pLinear != TAGENTRYNULL;

		i++, pLinear = pLinear->entry_linearNext )

	{

		(void)fprintf( stderr, "%d:\n", i );

		(void)TagEntryPrint( pLinear );

	}



	return ( 0 );

}



#ifdef DEBUG

int					/* Returns status		*/

#ifdef __STDC__

TagEntryDebug( TagEntry *tagEntry )

#else

TagEntryDebug( tagEntry )

	TagEntry *tagEntry;		/* Entry to print		*/

#endif

{

	int i;				/* Counter			*/



	if ( tagEntry == TAGENTRYNULL )

	{

		TagErrNo = TAGENULLTAGENTRY;

		return ( -1 );

	}



	(void)fprintf( stderr, "  tag        = '%s'\n", tagEntry->entry_tag );

	(void)fprintf( stderr, "  nth entry  = %d\n", tagEntry->entry_nthEntry );

	(void)fprintf( stderr, "  hash entry = 0x%08x\n", tagEntry->entry_hashEntry);

	(void)fprintf( stderr, "  linear next= 0x%08x\n",tagEntry->entry_linearNext);

	(void)fprintf( stderr, "  nth occur  = %d\n", tagEntry->entry_nthOccur );

	(void)fprintf( stderr, "  occur next = 0x%08x\n",tagEntry->entry_occurNext);

	(void)fprintf( stderr, "  value size = %d\n", tagEntry->entry_valueSize );

	(void)fprintf( stderr, "  value type = " );

	switch ( tagEntry->entry_valueType )

	{

	case POINTER:	(void)fprintf( stderr, "POINTER\n" ); break;

	case CHAR:	(void)fprintf( stderr, "CHAR\n" ); break;

	case UCHAR:	(void)fprintf( stderr, "UCHAR\n" ); break;

	case SHORT:	(void)fprintf( stderr, "SHORT\n" ); break;

	case LONG:	(void)fprintf( stderr, "LONG\n" ); break;

	case INT:	(void)fprintf( stderr, "INT\n" ); break;

	case INT8:	(void)fprintf( stderr, "INT8\n" ); break;

	case INT16:	(void)fprintf( stderr, "INT16\n" ); break;

	case INT32:	(void)fprintf( stderr, "INT32\n" ); break;

	case USHORT:	(void)fprintf( stderr, "USHORT\n" ); break;

	case ULONG:	(void)fprintf( stderr, "ULONG\n" ); break;

	case UINT:	(void)fprintf( stderr, "UINT\n" ); break;

	case UINT8:	(void)fprintf( stderr, "UINT8\n" ); break;

	case UINT16:	(void)fprintf( stderr, "UINT16\n" ); break;

	case UINT32:	(void)fprintf( stderr, "UINT32\n" ); break;

	case BOOLEAN:	(void)fprintf( stderr, "BOOLEAN\n" ); break;

	case FLOAT:	(void)fprintf( stderr, "FLOAT\n" ); break;

	case DOUBLE:	(void)fprintf( stderr, "DOUBLE\n" ); break;

	case INT64:	(void)fprintf( stderr, "INT64\n" ); break;

	case UINT64:	(void)fprintf( stderr, "UINT64\n" ); break;

	default:	(void)fprintf( stderr, "unknown!\n" ); break;

	}



	(void)fprintf( stderr, "  value =      " );

	for ( i = 0; i < tagEntry->entry_valueSize; i++ )

		(void)fprintf( stderr, "0x%08x ", tagEntry->entry_value[i] );

	(void)fprintf( stderr, "\n" );



	return ( 0 );

}



int					/* Returns status		*/

#ifdef __STDC__

TagTableDebug( TagTable *tagTable )

#else

TagTableDebug( tagTable )

	TagTable *tagTable;		/* Table to print		*/

#endif

{

	int i;				/* Counter			*/

	TagEntry *pLinear;		/* Linear list pointer		*/



	if ( tagTable == TAGTABLENULL )

	{

		TagErrNo = TAGENULLTAGTABLE;

		return ( -1 );

	}



	(void)fprintf( stderr, "Table:\n" );

	(void)fprintf( stderr, "  num entries  = %d\n",

		tagTable->table_numEntries );

	(void)fprintf( stderr, "  linear first = 0x%08x\n",

		tagTable->table_linearFirst );

	(void)fprintf( stderr, "  linear last  = 0x%08x\n",

		tagTable->table_linearLast );

	(void)fprintf( stderr, "  table length = %d\n",

		tagTable->table_tableLength );

	(void)fprintf( stderr, "  hash table   = 0x%08x\n",

		tagTable->table_tableHash );



	(void)fprintf( stderr, "Hash Table:\n" );

	for ( i = 0; i < tagTable->table_tableLength; i++ )

	{

		(void)fprintf( stderr, "%d:  0x%08x\n",

			i, &tagTable->table_tableHash[i] );

		(void)fprintf( stderr, "  hash_tag     = '%s'\n",

			tagTable->table_tableHash[i].hash_tag );

		(void)fprintf( stderr, "  hash collide = 0x%08x\n",

			tagTable->table_tableHash[i].hash_collideNext );

		(void)fprintf( stderr, "  num occur    = %d\n",

			tagTable->table_tableHash[i].hash_numOccur );

		(void)fprintf( stderr, "  hash occur   = %d\n",

			tagTable->table_tableHash[i].hash_occur );

	}



	(void)fprintf( stderr, "Linear List:\n" );

	for ( i = 0, pLinear = tagTable->table_linearFirst;

		pLinear != TAGENTRYNULL;

		i++, pLinear = pLinear->entry_linearNext )

	{

		(void)fprintf( stderr, "%d:  0x%08x\n", i, pLinear );

		TagEntryDebug( pLinear );

	}



	return ( 0 );

}

#endif

