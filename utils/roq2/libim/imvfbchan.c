/**

 **	$Header: /roq/libim/imvfbchan.c 1     11/02/99 4:38p Zaphod $

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



#define HEADER	"    $Header: /roq/libim/imvfbchan.c 1     11/02/99 4:38p Zaphod $"



/**

 **  FILE

 **	imvfbchan.c	-  Image Library VFB channel interchanging

 **

 **  PROJECT

 **	libim	-  SDSC image manipulation library

 **

 **  DESCRIPTION

 **	These functions permute / copy channels between VFBs.

 **

 **  PUBLIC CONTENTS

 **			d =defined constant

 **			f =function

 **			m =defined macro

 **			t =typedef/struct/union

 **			v =variable

 **			? =other

 **

 **     ImVfbCopyChannel     	f       copy one channel of one vfb to

 **                                     another channel of a another vfb

 **	ImVfbProcessMapRequests f	Permute the channels of a vfb

 **

 **  PRIVATE CONTENTS

 **	none

 **

 **  HISTORY

 **	$Log: /roq/libim/imvfbchan.c $
 * 
 * 1     11/02/99 4:38p Zaphod

 * Revision 1.6  1995/09/28  05:11:40  bduggan

 * fixed -inmap, -outmap bug

 * (default is to copy rgb channels

 * now when the alpha is copied.)

 * /

 *

 * Revision 1.5  1995/06/30  22:11:26  bduggan

 * removed strings.h

 *

 * Revision 1.4  1995/06/29  00:28:04  bduggan

 * updated copyright year

 *

 * Revision 1.3  1995/06/16  08:51:54  bduggan

 * added some casts.  took out some unused vars

 *

 * Revision 1.2  1995/05/17  23:49:12  bduggan

 * Added copying for a few more channels (PROT, Z, DATA)

 *

 * Revision 1.1  1995/02/16  21:43:00  bduggan

 * Initial revision

 *

 **/



/**

 **  CODE CREDITS

 **     Custom development, Brian Duggan, San Diego Supercomputer Center, 1995

 **/



#include "iminternal.h"

#include <string.h>



#ifdef __STDC__

static int imGetOutChannel(char* expression);

static int imGetInChannel(char* expression);

static ImVfb* imAllocateNewVfb(TagTable* flagsTable, ImVfb* inVfb);

static int imCheckChannelMapEntries(TagTable* flagsTable, ImVfb* inVfb);

static int imGiveInfoAboutChannel(TagTable* flagsTable, char* chanName);

#else

static int imGetOutChannel();

static int imGetInChannel();

static ImVfb* imAllocateNewVfb();

static int imCheckChannelMapEntries();

static int imGiveInfoAboutChannel();

#endif



/*

 *  MACRO 

 *	ImVfbQChannel (value, vfb, vfbptr, channel)

 *

 * DESCRIPTION

 *	Query a channel of the vfb into value

 *

 */



#define ImVfbQChannel(value, vfb, vfbptr, channel)			\
switch (channel & IMVFBOTHERMASK)					\
{									\
	case IMVFBGREY :  /* same as INDEX8 */				\
		value = ImVfbQGrey (vfb, vfbptr);			\
		break;							\
	case IMVFBRED  :						\
		value = ImVfbQRed  (vfb, vfbptr);			\
		break;							\
	case IMVFBGREEN :						\
		value = ImVfbQGreen(vfb, vfbptr);			\
		break;							\
	case IMVFBBLUE  :						\
		value = ImVfbQBlue (vfb, vfbptr);			\
		break;							\
	case IMVFBALPHA :						\
		value = ImVfbQAlpha (vfb, vfbptr);			\
		break;							\
	case IMVFBWPROT :						\
		value = ImVfbQWProt (vfb, vfbptr);			\
		break;							\
	case IMVFBZ :							\
		value = ImVfbQZ(vfb, vfbptr );				\
		break;							\
	case IMVFBFDATA :						\
		/* value = ImVfbQFData(vfb, vfbptr ); */		\
		break;							\
	case IMVFBIDATA :						\
		value = ImVfbQIData(vfb, vfbptr );			\
		break;							\
	default :							\
		if (channel & IMVFBINDEX8)				\
			value = ImVfbQGrey (vfb, vfbptr);		\
		else if (channel & IMVFBINDEX16)			\
			value = ImVfbQIndex16 (vfb, vfbptr);            \
		else							\
			value = 0;					\
		break;							\
}





/*

 *  MACRO 

 *	ImVfbSChannel (value, vfb, vfbptr, channel)

 *

 * DESCRIPTION

 *      Put value into this channel of the vfb

 *

 */



#define ImVfbSChannel(value, vfb, vfbptr, channel)			\
switch (channel & IMVFBOTHERMASK)					\
{									\
	case IMVFBRED  :						\
		ImVfbSRed  (vfb, vfbptr, value);			\
		break;							\
	case IMVFBGREEN :						\
		ImVfbSGreen(vfb, vfbptr, value);			\
		break;							\
	case IMVFBBLUE  :						\
		ImVfbSBlue (vfb, vfbptr, value);			\
		break;							\
	case IMVFBALPHA :						\
		ImVfbSAlpha (vfb, vfbptr, value);			\
		break;							\
	case IMVFBWPROT :						\
		ImVfbSWProt (vfb, vfbptr, value);			\
		break;							\
	case IMVFBZ :							\
		ImVfbSZ(vfb, vfbptr, value);				\
		break;							\
	case IMVFBFDATA :						\
		/* ImVfbSFData(vfb, vfbptr, value); */			\
		break;							\
	case IMVFBIDATA :						\
		ImVfbSIData(vfb, vfbptr, value);			\
		break;							\
	default :							\
		if (channel & IMVFBGREY)				\
			ImVfbSGrey (vfb, vfbptr, value);		\
		else							\
		if (channel & IMVFBINDEX16)				\
			ImVfbSIndex16 (vfb, vfbptr, value);		\
		break;							\
}

/*

 *  FUNCTION

 *    ImVfbCopyChannel

 *

 *  DESCRIPTION

 *    Copy a specified channel from one vfb to

 *   a specified channel of another vfb

 *

 *   The channels which can be specifed are as follows:

 *	IMVFBRED	red

 *	IMVFBBLUE	blue

 *	IMVFBGREEN	green

 *	IMVFBALPHA	alpha

 *	IMVFBINDEX8	index8 / gray

 *      IMVFBINDEX16    index16

 *	NULL		none

 *

 *    Copying "none" to a channel is equivalent to filling the outgoing

 *  channel with zeros. 

 *

 *    (It does not make the channel dissappear from the vfb.  If this is

 *  desired, then the -outnoalpha option, for instance, should be used.)

 *

 */

ImVfb *  /* Returns the destination vfb */

#ifdef __STDC__

ImVfbCopyChannel( ImVfb* inVfb, int inFieldMask, ImVfb* outVfb, int outFieldMask)

#else

ImVfbCopyChannel( inVfb, inFieldMask, outVfb, outFieldMask)

ImVfb* inVfb;

int inFieldMask;

ImVfb* outVfb;

int outFieldMask;

#endif

{

  int width, height;	       /* Dimensions of incoming vfb        */

  int w, h;                    /* Loop indices                      */

  ImVfbPtr inVfbPtr;           /* pointer into a vfb                */

  ImVfbPtr outVfbPtr;          /* pointer into a vfb                */

  unsigned int chanData;       /* temporary variable for data       */

  unsigned int tempMask;       /* holds masks during error checking */

  int inOK, outOK;             /* flags indicating that masks are ok*/

  int inFields, outFields;     /* Fields in the vfbs                */

  char message[300];           /* Buffer for messages               */



  if (inVfb==IMVFBNULL)

  {

	ImErrNo = IMENOVFB;

	return IMVFBNULL;

  }



  /* Get width and height of incoming vfb */

  width  = ImVfbQWidth (inVfb);

  height = ImVfbQHeight (inVfb);



  /* Allocate a new Vfb if necessary */

  if (outVfb==IMVFBNEW)

  {

	int allocFields;



	/* Make it the same type as the incoming vfb */

	allocFields = ImVfbQFields (inVfb) & IMVFBIMAGEMASK;



	outVfb = ImVfbAlloc(width, height, allocFields | outFieldMask);

	if (outVfb == IMVFBNULL)

	{

		ImErrNo = IMEMALLOC;

		return IMVFBNULL;

	}



  }



  /* Discern the fields that are present */

  inFields  = ImVfbQFields(inVfb);

  outFields = ImVfbQFields(outVfb);

  if (inFields & IMVFBRGB)

  {

	inFields |= IMVFBRED;

	inFields |= IMVFBGREEN;

	inFields |= IMVFBBLUE;

  }

  if (outFields & IMVFBRGB)

  {

	outFields |= IMVFBRED;

	outFields |= IMVFBGREEN;

	outFields |= IMVFBBLUE;

  }



  /* Confirm that inVfb has the specified field 

   * (if it's not NULL)

   */

  if ((inFieldMask!=NULL) && !(inFields & inFieldMask))

  {

	sprintf(message,"The incoming vfb does not have the requested channel");

	ImErrorFatal( message, IMVFBNULL, IMEBADCHANNEL );



  }



  /* 

   * Confirm that outVfb has the specified field 

   */

  if (!(outFields & outFieldMask))

  {

	sprintf(message,"The requested channel is not supported by the outgoing vfb");

	ImErrorFatal( message, IMVFBNULL, IMEFIELD );



  }



  /* Confirm that the vfbs are the same size */

  if ( width!=ImVfbQWidth(outVfb) || height!=ImVfbQHeight(outVfb))

  {

	ImErrNo = IMEDIFFSIZE;

	return IMVFBNULL;

  }



  /* Verify that the field masks are okay  */

  /* i.e. only one field can be copied     */

  inOK  = (inFieldMask==NULL);  /* Okay to copy from "none" */

  outOK = 0;                    /* Can't copy to "none"     */

  for (tempMask = (((unsigned int)1)<<31); (tempMask!=0) ; tempMask >>= 1)

  {

	if (tempMask==inFieldMask)	

		inOK  = 1;

	if (tempMask==outFieldMask)

		outOK = 1;

  }

  if (!inOK || !outOK)

  {

	ImErrNo = IMEFIELD;

	return IMVFBNULL;

  }



  /* Copy the data (finally)! */

  /* Note that a 16-bit value will automatically be truncated by the cast

     in the ImVfbSChannel macro, if index16 is copied to another channel */

  inVfbPtr  = ImVfbQPtr(inVfb,  0, 0);

  outVfbPtr = ImVfbQPtr(outVfb, 0, 0);

  for (w=0;w<width;w++)

  {

	for (h=0;h<height;h++)

	{

		ImVfbQChannel(chanData, inVfb,  inVfbPtr, inFieldMask);

		ImVfbSChannel(chanData, outVfb, outVfbPtr, outFieldMask);

		ImVfbSInc(inVfb,  inVfbPtr);

		ImVfbSInc(outVfb, outVfbPtr);

	}

  }

  return outVfb;

}





/*

 * FUNCTION

 *    ImVfbProcessMapRequests

 *

 * DESCRIPTION

 *    Go through a flags table, looking for map requests.

 *    Check for constistency of the requests, then

 *    alter each vfb in the tagtable in accordance with

 *    the requests.

 */



int	/* returns status */

#ifdef __STDC__

ImVfbProcessMapRequests( TagTable* flagsTable, TagTable* tagTable)

#else

ImVfbProcessMapRequests( flagsTable,tagTable)

TagTable* flagsTable;

TagTable* tagTable;

#endif

{

	int numRequests;	/* How many map requests are there? */

	TagEntry* tmpEntry;     /* Tag table entry                  */

	TagEntry* vfbEntry;     /* Tag table entry                  */

	char* thisRequest; 	/* The string in the flags table    */

	unsigned int inChannel; /* requested mapping from inChannel */

	unsigned int outChannel;/*                    to outChannel */

	ImVfb* inVfb;           /* VFB originally in tagTable       */

	ImVfb* outVfb;          /* new VFB replacing inVfb          */

	int i;                  /* Loop index                       */

	int vfbIndex;           /* Loop index                       */

	int numVfbs;            /* how many vfbs we've got          */

	char message[200];      /* buffer for messages              */





	numVfbs = TagTableQNEntry( tagTable, "image vfb" );



	for (vfbIndex = 0; vfbIndex < numVfbs; vfbIndex++)

	{

		/* 

		 * Get vfb from tagtable 

		 */

		vfbEntry = TagTableQDirect( tagTable, "image vfb", vfbIndex );

		TagEntryQValue( vfbEntry, &inVfb );

		

		/* 

		 * Check for consistent requests and add default requests

		 * to the flags table

		 */

		if (imCheckChannelMapEntries(flagsTable, inVfb)==-1)

			return -1;

		/*

		 * Allocate outVfb based on requests in the flagsTable,

		 * and inVfb's type.

		 */

		 outVfb = imAllocateNewVfb(flagsTable, inVfb);



		numRequests = TagTableQNEntry (flagsTable,"channel map request");



		for (i=0; i<numRequests; i++)

		{

			tmpEntry = TagTableQDirect( flagsTable, "channel map request", i );

			TagEntryQValue( tmpEntry, &thisRequest );

			inChannel  = imGetInChannel(thisRequest);

			outChannel = imGetOutChannel(thisRequest);

			

			/* Copy this channel! */

			outVfb = ImVfbCopyChannel( inVfb, inChannel, outVfb, outChannel);

			if (outVfb==IMVFBNULL)

			{

				sprintf(message,"Error copying channel");

				ImErrorFatal( message, -1, ImErrNo );

			}

		}



		/* Replace the vfb in the tag table with our new one */

		TagTableReplace( tagTable, TagEntryQNthEntry( vfbEntry),

			TagEntryAlloc( "image vfb", POINTER, &outVfb));

		ImVfbFree (inVfb);

	}



	return ( 1);

}







/*

 * FUNCTION

 *   imGetInChannel

 *

 * DESCRIPTION

 *   reads from a string such as "red=blue" and

 * returns the mask corresponding to the second 

 * word (i.e. IMVFBBLUE )

 */



static int

#ifdef __STDC__

imGetInChannel(char* expression)

#else

imGetInChannel(expression)

char* expression;

#endif

{

 char* chanName;	/* name of the channel */

 char message[200];	/* message buffer      */



 chanName = strchr( expression, '=');

 if (chanName==NULL)

 {

	sprintf(message,"Bad channel specification: '%s'",expression);

	ImErrorFatal(message, -1, IMEBADCHANNEL);

 }

 else

	chanName+=1; /* Move to the right of the '=' */



 if (strcmp(chanName,"red"  )==0)	return IMVFBRED;

 if (strcmp(chanName,"green")==0)	return IMVFBGREEN;

 if (strcmp(chanName,"blue" )==0)	return IMVFBBLUE;

 if (strcmp(chanName,"alpha")==0)	return IMVFBALPHA;

 if (strcmp(chanName,"grey" )==0)	return IMVFBGREY;

 if (strcmp(chanName,"gray" )==0)	return IMVFBGREY;

 if (strcmp(chanName,"index")==0)	return IMVFBINDEX8;

 if (strcmp(chanName,"index16")==0)	return IMVFBINDEX16;

 if (strcmp(chanName,"none" )==0)   	return 0;



 sprintf(message,"Bad channel specification: '%s'",expression);

 ImErrorFatal(message, -1, IMEBADCHANNEL);

}







/*

 * FUNCTION

 *   imGetOutChannel

 *

 * DESCRIPTION

 *   reads from a string such as "red=blue" and

 * returns the mask corresponding to the first 

 * word (i.e. IMVFBRED )

 */



static int

#ifdef __STDC__

imGetOutChannel(char* expression)

#else

imGetOutChannel(expression)

char* expression;

#endif

{

 char chanName[20];	/* name of the channel */

 char* ptr;             /* temp ptr to a char* */

 char message[200];     /* msg buffer          */



 strcpy(chanName,expression);



 /*

  * Turn the '=' into a '\0'

  */

 ptr = strchr( chanName, '=');

 if (ptr==NULL)

 { 

	 sprintf(message,"Bad channel specification: '%s'",expression);

	 ImErrorFatal(message, -1, IMEBADCHANNEL);

 }

 else *ptr='\0';



 if (strcmp(chanName,"red")==0)		return IMVFBRED;

 if (strcmp(chanName,"green")==0)	return IMVFBGREEN;

 if (strcmp(chanName,"blue")==0)	return IMVFBBLUE;

 if (strcmp(chanName,"alpha")==0)	return IMVFBALPHA;

 if (strcmp(chanName,"gray")==0)	return IMVFBGREY;

 if (strcmp(chanName,"grey")==0)	return IMVFBGREY;

 if (strcmp(chanName,"index")==0)	return IMVFBINDEX8;

 if (strcmp(chanName,"index16")==0)	return IMVFBINDEX16;

 if (strcmp(chanName,"none")==0)   	return 0;



 sprintf(message,"Bad channel specification: '%s'",expression);

 ImErrorFatal(message, -1, IMEBADCHANNEL);

}



/*

 * FUNCTION

 *   numOfMappingsToChannel

 *

 * DESCRIPTION

 *   Searches the flags table, and reports

 * the number of channel map requests of

 * the form "channel=xxx" where channel corresponds

 * to the parameter,

 *

 */

static int /* Returns number of mappings to channel */

#ifdef __STDC__

numOfMappingsToChannel(TagTable* flagsTable, int chanMask)

#else

numOfMappingsToChannel(flagsTable, chanMask)

TagTable* flagsTable;

int chanMask;

#endif

{

 int numEntries;	/* Total number of channel requests */

 char *thisEntry;       /* channel request                  */

 int i;                 /* loop index                       */

 int numOfMappings;     /* number of mappings (return value)*/

 TagEntry* tmpEntry;    /* flags table entry                */



 numEntries =  TagTableQNEntry(flagsTable, "channel map request");



 numOfMappings = 0;

 for (i=0;i<numEntries;i++)

 {

	tmpEntry = TagTableQDirect(flagsTable, "channel map request", i);

	TagEntryQValue( tmpEntry, &thisEntry);

	if (imGetOutChannel(thisEntry)==chanMask)

		numOfMappings++;

 }



 return numOfMappings;

}





/*

 * FUNCTION

 *   imCheckChannelMapEntries

 *

 * DESCRIPTION

 *    This function does two things with channel map requests

 *  in a flagsTable:

 *      1. Ensures that every channel in an outgoing vfb has

 *         at least one channel mapped to it.

 *      2. Ensures that no channel in an outgoing vfb has more

 *         than one channel mapped to it.

 *  In case 1, default channels are added to the flags table.

 *  In case 2, an error is produced.

 */



static int  /* Returns status.  >0 means okay, <=0 means not okay. */

#ifdef __STDC__

imCheckChannelMapEntries(TagTable* flagsTable, ImVfb* inVfb)

#else

imCheckChannelMapEntries(flagsTable, inVfb)

TagTable * flagsTable;

ImVfb* inVfb;

#endif

{

	int fieldsToTest; 	/* fields we are checking for          */

	int numMappings;        /* number of mappings for this channel */

	char* newString;        /* new string to add to the table      */

	int inFields;           /* fields in inVfb                     */



	/* Discern the fields that are present */

	inFields = ImVfbQFields(inVfb);

	if (inFields & IMVFBRGB)

	{

		inFields |= IMVFBRED;

		inFields |= IMVFBGREEN;

		inFields |= IMVFBBLUE;

	}



	fieldsToTest = inFields;



	/*

	 * For each channel, ensure that there is precisely

	 * ONE channel mapped to it.

	 *

	 * If there are more than one, report an error and quit.

	 * If there are none

	 *       if this field exists in the inVfb, add channel=channel

	 *       if this field doesn't exist in inVfb, add channel=none

	 */



	/* 

	 * Red

	 */

	if (fieldsToTest & IMVFBRED)

	{

		numMappings = numOfMappingsToChannel(flagsTable, IMVFBRED);

		if (numMappings>1)

		{

			ImErrorFatal("Too many mappings to red", -1, IMEBADCHANNELS);

		}

		if (numMappings==0)

		{ 	/* Add a mapping to red to the flags table */

			if (inFields & IMVFBRED)

			{

				/* Add "red=red" to the flagsTable */

				ImMalloc(newString, char *, 10 * sizeof(char));

				strcpy(newString, "red=red");

			}

			else

			{  	/* Add "red=none" to the flagsTable */

				ImMalloc(newString, char *, 10 * sizeof(char));

				strcpy(newString, "red=none");

			}

			TagTableAppend(flagsTable,

				TagEntryAlloc("channel map request",POINTER, &newString));

		}

		/* Give an imInfo message */

		imGiveInfoAboutChannel(flagsTable,"red");

	}



	/*

	 * Green

	 */

	if (fieldsToTest & IMVFBGREEN)

	{

		numMappings = numOfMappingsToChannel(flagsTable, IMVFBGREEN);

		if (numMappings>1)

		{

			ImErrorFatal("Too many mappings to green", -1, IMEBADCHANNELS);

		}

		if (numMappings==0)

		{ 	/* Add a mapping to red to the flags table */

			if (inFields & IMVFBGREEN)

			{

				/* Add "green=green" to the flagsTable */

				ImMalloc(newString, char *, 15 * sizeof(char));

				strcpy(newString, "green=green");

			}

			else

			{  	/* Add "green=none" to the flagsTable */

				ImMalloc(newString, char *, 15 * sizeof(char));

				strcpy(newString, "green=none");

			}

			TagTableAppend(flagsTable,

				TagEntryAlloc("channel map request",POINTER, &newString));

		}

		/* Give an imInfo message */

		imGiveInfoAboutChannel(flagsTable,"green");

	}

	/*

	 * Blue

	 */

	if (fieldsToTest & IMVFBBLUE)

	{

		numMappings = numOfMappingsToChannel(flagsTable, IMVFBBLUE);

		if (numMappings>1)

		{

			ImErrorFatal("Too many mappings to blue", -1, IMEBADCHANNELS);

		}

		if (numMappings==0)

		{ 	/* Add a mapping to red to the flags table */

			if (inFields & IMVFBBLUE)

			{

				/* Add "blue=blue" to the flagsTable */

				ImMalloc(newString, char *, 15 * sizeof(char));

				strcpy(newString, "blue=blue");

			}

			else

			{  	/* Add "blue=none" to the flagsTable */

				ImMalloc(newString, char *, 15 * sizeof(char));

				strcpy(newString, "blue=none");

			}

			TagTableAppend(flagsTable,

				TagEntryAlloc("channel map request",POINTER, &newString));

		}

		/* Give an imInfo message */

		imGiveInfoAboutChannel(flagsTable,"blue");

	}

	/*

	 * Grey/Index8 

	 */

	if (fieldsToTest & IMVFBGREY)

	{

		numMappings = numOfMappingsToChannel(flagsTable, IMVFBGREY);

		if (numMappings>1)

		{

			ImErrorFatal("Too many mappings to grey", -1, IMEBADCHANNELS);

		}

		if (numMappings==0)

		{ 	/* Add a mapping to grey to the flags table */

			if (inFields & IMVFBGREY)

			{

				/* Add "grey=grey" to the flagsTable */

				ImMalloc(newString, char *, 10 * sizeof(char));

				strcpy(newString, "grey=grey");

			}

			else

			{  	/* Add "grey=none" to the flagsTable */

				ImMalloc(newString, char *, 10 * sizeof(char));

				strcpy(newString, "grey=none");

			}

			TagTableAppend(flagsTable,

				TagEntryAlloc("channel map request",POINTER, &newString));

		}

		/* Give an imInfo message */

		imGiveInfoAboutChannel(flagsTable,"grey");

	}

 	/*

	 * Alpha       

	 */

	if (fieldsToTest & IMVFBALPHA)

	{

		numMappings = numOfMappingsToChannel(flagsTable, IMVFBALPHA);

		if (numMappings>1)

		{

			ImErrorFatal("Too many mappings to alpha", -1, IMEBADCHANNELS);

		}

		if (numMappings==0)

		{ 	/* Add a mapping to alpha to the flags table */

			if (inFields & IMVFBALPHA)

			{

				/* Add "alpha=alpha" to the flagsTable */

				ImMalloc(newString, char *, 10 * sizeof(char));

				strcpy(newString, "alpha=alpha");

			}

			else

			{  	/* Add "alpha=none" to the flagsTable */

				ImMalloc(newString, char *, 10 * sizeof(char));

				strcpy(newString, "alpha=none");

			}

			TagTableAppend(flagsTable,

				TagEntryAlloc("channel map request",POINTER, &newString));

		}

		/* Give an imInfo message */

		imGiveInfoAboutChannel(flagsTable,"alpha");

	}





	/* 

	 * Index16

	 */

	if (fieldsToTest & IMVFBINDEX16)

	{

		numMappings = numOfMappingsToChannel(flagsTable, IMVFBINDEX16);

		if (numMappings>1)

		{

			ImErrorFatal("Too many mappings to index16", -1, IMEBADCHANNELS);

		}

		if (numMappings==0)

		{ 	/* Add a mapping to red to the flags table */

			if (inFields & IMVFBINDEX16)

			{

				/* Add "index16=index16" to the flagsTable */

				ImMalloc(newString, char *, 20 * sizeof(char));

				strcpy(newString, "index16=index16");

			}

			else

			{  	/* Add "index16=none" to the flagsTable */

				ImMalloc(newString, char *, 20 * sizeof(char));

				strcpy(newString, "index16=none");

			}

			TagTableAppend(flagsTable,

				TagEntryAlloc("channel map request",POINTER, &newString));

		}

		/* Give an imInfo message */

		imGiveInfoAboutChannel(flagsTable,"index16");

	}



	return (1);

}





/*

 * FUNCTION

 *    imGiveInfoAboutChannel

 *

 * DESCRIPTION

 *    Spit out an ImInfo message describing what

 * is being mapped to this channel, according to

 * the flags table

 *

 * e.g. passing "red" will search the flags

 * table for something like red=blue, then

 * print "red = blue"

 */



static int /* returns status */

#ifdef __STDC__

imGiveInfoAboutChannel(TagTable* flagsTable, char* chanName)

#else

imGiveInfoAboutChannel(flagsTable,chanName)

TagTable* flagsTable;

char* chanName;

#endif

{

 TagEntry* tmpEntry;            /* temporary tag table entry   */

 char*     request;             /* Request from entry          */

 int numChannelEntries;		/* # of 'channel map request's */

 int i;				/* Loop index                  */

 char tmpRequest[50];           /* Holds the request           */

 char* ptr;                     /* temp char* pointer          */

 char message[300];             /* Buffer for messages         */



 numChannelEntries = TagTableQNEntry(flagsTable, "channel map request");



 for (i=0; i<numChannelEntries; i++)

 {

 	tmpEntry = TagTableQDirect( flagsTable, "channel map request", i);

	TagEntryQValue(tmpEntry, &request);



	/*

	 * Turn the '=' into a '\0' so we can look at the name to the 

	 * left of the equals sign

	 */

	strcpy(tmpRequest,request);

	ptr = strchr( tmpRequest, '=');

	if (ptr==NULL)

	{ 

		sprintf(message,"Bad channel specification: '%s'",request);

		ImErrorFatal(message, -1, IMEBADCHANNEL);

	}

	else *ptr='\0';



	/*

	 * Is this the right channel?

	 */

	 if (strcmp(tmpRequest,chanName)==0)

	 { /* yes */

		sprintf(message,"%s = %s", chanName, ptr+1);

		ImInfo("Channel Mapping", message); 

	 }

 }

 return 1;

}





/*

 * FUNCTION

 * 	imAllocateNewVfb

 *

 * DESCRIPTION

 *   	Use the requests in the flagsTable and in the

 *  fields/type of inVfb to make a type for outVfb. 

 *  Then allocate outVfb.

 */



static ImVfb* /* returns new vfb */

#ifdef __STDC__

imAllocateNewVfb(TagTable* flagsTable, ImVfb* inVfb)

#else

imAllocateNewVfb(flagsTable, inVfb)

TagTable* flagsTable;

ImVfb* inVfb;

#endif

{

 int inFields;	     /* fields in inVfb              */

 int outFields;      /* fields we're going to create */

 int numRequests;    /* number of map requests       */

 int i;              /* index                        */

 TagEntry* tmpEntry; /* tag table entry              */

 char* chanString;   /* string in the flags table    */

 ImVfb* outVfb;      /* to be created.               */



 inFields = ImVfbQFields (inVfb);



 /* For each channel request,

  *  add the fields to outFields.

  */ 

  outFields = 0;

  numRequests = TagTableQNEntry(flagsTable, "channel map request");

  for (i=0;i<numRequests;i++)

  {

 	tmpEntry = TagTableQDirect( flagsTable, "channel map request", i);

	TagEntryQValue(tmpEntry, &chanString);

	outFields |= imGetOutChannel(chanString);

  }

  /*

   * Okay, so we have all the fields, but what

   * TYPE is it?

   */

  if ( (outFields & IMVFBRED) ||

       (outFields & IMVFBBLUE) ||

       (outFields & IMVFBGREEN)) 

  { /* It's rgb. */

	outFields |= IMVFBRGB;

  }

  else if ( (outFields & IMVFBINDEX8) || (outFields & IMVFBINDEX16))

  { 

	/* It's indexed. Do nothing */

  }

  else

  { /* We have no idea what it is.  */

    /* Don't give it a type.        */

  }

  outVfb = ImVfbAlloc(ImVfbQWidth(inVfb), ImVfbQHeight(inVfb), outFields);



 return outVfb;

}







