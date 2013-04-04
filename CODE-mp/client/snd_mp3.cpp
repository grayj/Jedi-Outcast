// Filename:-	cl_mp3.cpp
//
// (The interface module between all the MP3 stuff and Trek)	-Ste.
//
// Note that this SOF2 versions is a lot messier than EF1's version, because of having to kludge stuff around
//	this annoying link-list sound-memory system, so if you ever want to add it to another project, use EF1 version.
//
// this include must remain at the top of every CPP file
#include "client.h"
#include "snd_local.h"
//#include "../qcommon/memory.h"


#include "snd_mp3.h"				// only included directly by a few snd_xxxx.cpp files plus this one
#include "../mp3code/mp3struct.h"	// keep this rather awful file secret from the rest of the program
#include "../mp3code/copyright.h"



// maybe I'm re-inventing the wheel, here, but I can't see any functions that already do this, so...
//
char *Filename_WithoutPath(const char *psFilename)
{
	static char sString[MAX_QPATH];	// !!
	const char *p = strrchr(psFilename,'\\');

  	if (!p++)
		p=psFilename;

	strcpy(sString,p);

	return sString;

}

// returns (eg) "\dir\name" for "\dir\name.bmp"
//
char *Filename_WithoutExt(const char *psFilename)
{
	static char sString[MAX_QPATH];	// !

	strcpy(sString,psFilename);

	char *p = strrchr(sString,'.');		
	char *p2= strrchr(sString,'\\');

	// special check, make sure the first suffix we found from the end wasn't just a directory suffix (eg on a path'd filename with no extension anyway)
	//
	if (p && (p2==0 || (p2 && p>p2)))
		*p=0;	

	return sString;

}



int iFilesFound;
int iFilesUpdated;
int iErrors;
qboolean qbForceRescan;
qboolean qbForceStereo;

void R_CheckMP3s( const char *psDir )
{
//	Com_Printf(va("Scanning Dir: %s\n",psDir));
	Com_Printf(".");	// stops useful info scrolling off screen

	char	**sysFiles, **dirFiles;
	int		numSysFiles, i, numdirs;

	dirFiles = FS_ListFiles( psDir, "/", &numdirs);
	if (numdirs > 2)
	{
		for (i=2;i<numdirs;i++)
		{
			char	sDirName[MAX_QPATH];
			sprintf(sDirName, "%s\\%s", psDir, dirFiles[i]);
			R_CheckMP3s(sDirName);
		}
	}

	sysFiles = FS_ListFiles( psDir, ".mp3", &numSysFiles );
	for(i=0; i<numSysFiles; i++)
	{
		char	sFilename[MAX_QPATH];
		sprintf(sFilename,"%s\\%s", psDir, sysFiles[i]);		
			
		Com_Printf("%sFound file: %s",!i?"\n":"",sFilename);

		iFilesFound++;

		// read it in...
		//
		byte *pbData = NULL;
		int iSize = FS_ReadFile( sFilename, (void **)&pbData);

		if (pbData)
		{
			id3v1_1* pTAG;

			// do NOT check 'qbForceRescan' here as an opt, because we need to actually fill in 'pTAG' if there is one...
			//
			qboolean qbTagNeedsUpdating = (/* qbForceRescan || */ !MP3_ReadSpecialTagInfo(pbData, iSize, &pTAG))?qtrue:qfalse;

			if (pTAG == NULL || qbTagNeedsUpdating || qbForceRescan)
			{
				Com_Printf(" ( Updating )\n");

				// I need to scan this file to get the volume...
				//
				// For EF1 I used a temp sfx_t struct, but I can't do that now with this new alloc scheme,
				//	I have to ask for it legally, so I'll keep re-using one, and restoring it's name after use.
				//	(slightly dodgy, but works ok if no-one else changes stuff)
				//
				//sfx_t SFX = {0};
				extern sfx_t *S_FindName( const char *name );
				//
				static sfx_t *pSFX = NULL;
				const char sReservedSFXEntrynameForMP3[] = "reserved_for_mp3";	// ( strlen() < MAX_QPATH )

				if (pSFX == NULL)	// once only
				{
					pSFX = S_FindName(sReservedSFXEntrynameForMP3);	// always returns, else ERR_FATAL					
				}

				if (MP3_IsValid(sFilename,pbData, iSize, qbForceStereo))
				{
					wavinfo_t info;

					int iRawPCMDataSize = MP3_GetUnpackedSize(sFilename, pbData, iSize, qtrue, qbForceStereo);

					if (iRawPCMDataSize)	// should always be true, unless file is fucked, in which case, stop this conversion process
					{
						float fMaxVol = 128;	// any old default
						int iActualUnpackedSize = iRawPCMDataSize;	// default, override later if not doing music

						if (!qbForceStereo)	// no point for stereo files, which are for music and therefore no lip-sync
						{
							byte *pbUnpackBuffer = (byte *) Z_Malloc ( iRawPCMDataSize+10, TAG_TEMP_WORKSPACE );	// won't return if fails

							iActualUnpackedSize = MP3_UnpackRawPCM( sFilename, pbData, iSize, pbUnpackBuffer );
							if (iActualUnpackedSize != iRawPCMDataSize)
							{
								Com_Error(ERR_DROP, "******* Whoah! MP3 %s unpacked to %d bytes, but size calc said %d!\n",sFilename,iActualUnpackedSize,iRawPCMDataSize);
							}
						
							// fake up a WAV structure so I can use the other post-load sound code such as volume calc for lip-synching
							//
							MP3_FakeUpWAVInfo( sFilename, pbData, iSize, iActualUnpackedSize,
												// these params are all references...
												info.format, info.rate, info.width, info.channels, info.samples, info.dataofs
												);

							extern void S_LoadSound_Finalize(wavinfo_t	*info, sfx_t *sfx, byte *data);

							S_LoadSound_Finalize(&info, pSFX, pbUnpackBuffer);	// all this just for lipsynch. Oh well.

							fMaxVol = pSFX->fVolRange;

							// free sfx->data...
							//
							{
								// Hunk_FreeTempMemory( SFX.data );	// this will have been allocated inside S_LoadSound_Finalise()
								//
								// I want a big thankyou from the Mac guys for providing this define... :-) -ste
								//
								// #ifndef INT_MIN
								// #define INT_MIN     (-2147483647 - 1) /* minimum (signed) int value */
								// #endif
								//
								pSFX->iLastTimeUsed = INT_MIN;		// force this to be oldest sound file, therefore disposable...
								pSFX->bInMemory = qtrue;
								SND_FreeOldestSound();		// ... and do the disposal

								// now set our temp SFX struct back to default name so nothing else accidentally uses it...
								//
								strcpy(pSFX->sSoundName, sReservedSFXEntrynameForMP3);
								pSFX->bDefaultSound = qfalse;								
							}

							// other stuff...
							//
							Z_Free(pbUnpackBuffer);
						}

						// well, time to update the file now...
						//
						fileHandle_t f = FS_FOpenFileWrite( sFilename );
						if (f)
						{
							// write the file back out, but omitting the tag if there was one...
							//
							int iWritten = FS_Write(pbData, iSize-(pTAG?sizeof(*pTAG):0), f);

							if (iWritten)
							{
								// make up a new tag if we didn't find one in the original file...
								//
								id3v1_1 TAG;
								if (!pTAG)
								{
									pTAG = &TAG;
									memset(&TAG,0,sizeof(TAG));
									strncpy(pTAG->id,"TAG",3);
								}

								strncpy(pTAG->title,	Filename_WithoutPath(Filename_WithoutExt(sFilename)), sizeof(pTAG->title));
								strncpy(pTAG->artist,	"Raven Software",						sizeof(pTAG->artist)	);
								strncpy(pTAG->year,		"2001",									sizeof(pTAG->year)		);
								strncpy(pTAG->comment,	va("%s %g",sKEY_MAXVOL,fMaxVol),		sizeof(pTAG->comment)	);
								strncpy(pTAG->album,	va("%s %d",sKEY_UNCOMP,iActualUnpackedSize),sizeof(pTAG->album)	);
								
								if (FS_Write( pTAG, sizeof(*pTAG), f ))	// NZ = success
								{
									iFilesUpdated++;
								}
								else
								{
									Com_Printf("*********** Failed write to file!\n");
									iErrors++;
								}
							}
							else
							{
								Com_Printf("*********** Failed write to file!\n");
								iErrors++;
							}
							FS_FCloseFile( f );
						}
						else
						{
							Com_Printf("*********** Failed to re-open for write!\n");
							iErrors++;
						}					
					}
					else
					{
						Com_Error(ERR_DROP, "******* This MP3 should be deleted: %s\n",sFilename);
					}
				}
				else
				{
					Com_Printf("*********** File was not a valid MP3!\n");
					iErrors++;
				}
			}
			else
			{
				Com_Printf(" ( OK )\n");
			}

			FS_FreeFile( pbData );
		}
	}
	FS_FreeFileList( sysFiles );
	FS_FreeFileList( dirFiles );
}


// this console-function is for development purposes, and makes sure that sound/*.mp3 /s have tags in them
//	specifying stuff like their max volume (and uncompressed size) etc...
//
void S_MP3_CalcVols_f( void )
{
	char sStartDir[MAX_QPATH] = {"sound"};
	const char sUsage[] = "Usage: mp3_calcvols [-rescan] [startdir (default='sound')]\n";

	if (Cmd_Argc()>4)	// 3 optional arguments
	{
		Com_Printf(sUsage);
		return;
	}

	S_StopAllSounds();


	qbForceRescan = qfalse;
	qbForceStereo = qfalse;
	iFilesFound		= 0;
	iFilesUpdated	= 0;
	iErrors			= 0;

	for (int i=1; i<Cmd_Argc(); i++)
	{
		if (Cmd_Argv(i)[0] == '-')
		{
			if (!Q_stricmp(Cmd_Argv(i),"-rescan"))
			{
				qbForceRescan = qtrue;
			}
			else
			if (!Q_stricmp(Cmd_Argv(i),"-stereo"))
			{
				qbForceStereo = qtrue;
			}
			else
			{
				// unknown switch...
				//
				Com_Printf(sUsage);
				return;
			}
			continue;
		}
		strcpy(sStartDir,Cmd_Argv(i));
	}

	Com_Printf(va("Starting Scan for Updates in Dir: %s\n",sStartDir));
	R_CheckMP3s( sStartDir );

	Com_Printf("\n%d files found/scanned, %d files updated      ( %d errors total)\n",iFilesFound,iFilesUpdated,iErrors);
}




// expects data already loaded, filename arg is for error printing only
//
// returns success/fail
//
qboolean MP3_IsValid( const char *psLocalFilename, void *pvData, int iDataLen, qboolean bStereoDesired /* = qfalse */)
{
	char *psError = C_MP3_IsValid(pvData, iDataLen, bStereoDesired);

	if (psError)
	{
		Com_Printf(va(S_COLOR_RED"%s\n(File: %s)\n",psError, psLocalFilename));
	}

	return psError ? qfalse : qtrue;
}



// expects data already loaded, filename arg is for error printing only
//
// returns unpacked length, or 0 for errors (which will be printed internally)
//
int MP3_GetUnpackedSize( const char *psLocalFilename, void *pvData, int iDataLen, qboolean qbIgnoreID3Tag /* = qfalse */
						, qboolean bStereoDesired /* = qfalse */
						)
{
	int	iUnpackedSize = 0;	

	if (qbIgnoreID3Tag || !MP3_ReadSpecialTagInfo((byte *)pvData, iDataLen, NULL, &iUnpackedSize))
	{	
		char *psError = C_MP3_GetUnpackedSize( pvData, iDataLen, &iUnpackedSize, bStereoDesired);

		if (psError)
		{
			Com_Printf(va(S_COLOR_RED"%s\n(File: %s)\n",psError, psLocalFilename));
			return 0;
		}
	}	

	return iUnpackedSize;
}



// expects data already loaded, filename arg is for error printing only
//
// returns byte count of unpacked data (effectively a success/fail bool)
//
int MP3_UnpackRawPCM( const char *psLocalFilename, void *pvData, int iDataLen, byte *pbUnpackBuffer, qboolean bStereoDesired /* = qfalse */)
{
	int iUnpackedSize;
	char *psError = C_MP3_UnpackRawPCM( pvData, iDataLen, &iUnpackedSize, pbUnpackBuffer, bStereoDesired);

	if (psError)
	{
		Com_Printf(va(S_COLOR_RED"%s\n(File: %s)\n",psError, psLocalFilename));
		return 0;
	}

	return iUnpackedSize;
}


// psLocalFilename is just for error reporting (if any)...
//
qboolean MP3Stream_InitPlayingTimeFields( LP_MP3STREAM lpMP3Stream, const char *psLocalFilename, void *pvData, int iDataLen, qboolean bStereoDesired /* = qfalse */)
{
	qboolean bRetval = qfalse;

	int iRate, iWidth, iChannels;

	char *psError = C_MP3_GetHeaderData(pvData, iDataLen, &iRate, &iWidth, &iChannels, bStereoDesired );
	if (psError)
	{
		Com_Printf(va(S_COLOR_RED"MP3Stream_InitPlayingTimeFields(): %s\n(File: %s)\n",psError, psLocalFilename));
	}
	else
	{
		int iUnpackLength = MP3_GetUnpackedSize( psLocalFilename, pvData, iDataLen, qfalse,	// qboolean qbIgnoreID3Tag 
													bStereoDesired);
		if (iUnpackLength)
		{
			lpMP3Stream->iTimeQuery_UnpackedLength	= iUnpackLength;
			lpMP3Stream->iTimeQuery_SampleRate		= iRate;
			lpMP3Stream->iTimeQuery_Channels		= iChannels;
			lpMP3Stream->iTimeQuery_Width			= iWidth;

			bRetval = qtrue;
		}
	}

	return bRetval;
}

float MP3Stream_GetPlayingTimeInSeconds( LP_MP3STREAM lpMP3Stream )
{
	if (lpMP3Stream->iTimeQuery_UnpackedLength)	// fields initialised?
		return (float)((((double)lpMP3Stream->iTimeQuery_UnpackedLength / (double)lpMP3Stream->iTimeQuery_SampleRate) / (double)lpMP3Stream->iTimeQuery_Channels) / (double)lpMP3Stream->iTimeQuery_Width);

	return 0.0f;
}

float MP3Stream_GetRemainingTimeInSeconds( LP_MP3STREAM lpMP3Stream )
{
	if (lpMP3Stream->iTimeQuery_UnpackedLength)	// fields initialised?
		return (float)(((((double)(lpMP3Stream->iTimeQuery_UnpackedLength - (lpMP3Stream->iBytesDecodedTotal * (lpMP3Stream->iTimeQuery_SampleRate / dma.speed)))) / (double)lpMP3Stream->iTimeQuery_SampleRate) / (double)lpMP3Stream->iTimeQuery_Channels) / (double)lpMP3Stream->iTimeQuery_Width);

	return 0.0f;
}

// ( m_lengthInSeconds = ((iUnpackedDataLength / iRate) / iChannels) / iWidth; )


// expects data already loaded, filename arg is for error printing only
//
qboolean MP3_FakeUpWAVInfo( const char *psLocalFilename, void *pvData, int iDataLen, int iUnpackedDataLength, 
						   int &format, int &rate, int &width, int &channels, int &samples, int &dataofs,
						   qboolean bStereoDesired /* = qfalse */
						   )
{
	// some things can be done instantly...
	//
	format = 1;		// 1 for MS format
	dataofs= 0;		// will be 0 for me (since there's no header in the unpacked data)

	// some things need to be read...  (though the whole stereo flag thing is crap)
	//
	char *psError = C_MP3_GetHeaderData(pvData, iDataLen, &rate, &width, &channels, bStereoDesired );
	if (psError)
	{
		Com_Printf(va(S_COLOR_RED"%s\n(File: %s)\n",psError, psLocalFilename));
	}

	// and some stuff needs calculating...
	//
	samples	= iUnpackedDataLength / width;		

	return psError ? qfalse : qtrue;
}



const char sKEY_MAXVOL[]="#MAXVOL";	// formerly #defines
const char sKEY_UNCOMP[]="#UNCOMP";	//    "        "

// returns qtrue for success...
//
qboolean MP3_ReadSpecialTagInfo(byte *pbLoadedFile, int iLoadedFileLen,		// (in)
								id3v1_1** ppTAG,							// (out), can be NULL
								int *piUncompressedSize, float *pfMaxVol	// (out), can be NULL
								)
{
	qboolean qbError = qfalse;

	id3v1_1* pTAG = (id3v1_1*) ((pbLoadedFile+iLoadedFileLen)-sizeof(id3v1_1));	// sizeof = 128	

	if (!strncmp(pTAG->id, "TAG", 3))
	{
		// TAG found...
		//
		
		// read MAXVOL key...
		//
		if (strncmp(pTAG->comment, sKEY_MAXVOL,	strlen(sKEY_MAXVOL)))
		{
			qbError = qtrue;
		}
		else
		{
			if ( pfMaxVol)
			{
				*pfMaxVol = atof(pTAG->comment + strlen(sKEY_MAXVOL));
			}
		}

		//
		// read UNCOMP key...
		//
		if (strncmp(pTAG->album, sKEY_UNCOMP, strlen(sKEY_UNCOMP)))
		{
			qbError = qtrue;
		}
		else
		{
			if ( piUncompressedSize)
			{
				*piUncompressedSize = atoi(pTAG->album + strlen(sKEY_UNCOMP));
			}
		}
	}
	else
	{
		pTAG = NULL;
	}

	if (ppTAG)
	{
		*ppTAG = pTAG;
	}

	return (pTAG && !qbError) ? qtrue: qfalse;
}



#define FUZZY_AMOUNT (5*1024)	// so it has to be significantly over, not just break even, because of
								// the xtra CPU time versus memory saving

cvar_t* cv_MP3overhead = NULL;
void MP3_InitCvars(void)
{
	cv_MP3overhead = Cvar_Get("s_mp3overhead", va("%d", sizeof(MP3STREAM) + FUZZY_AMOUNT), CVAR_ARCHIVE );
}


// a file has been loaded in memory, see if we want to keep it as MP3, else as normal WAV...
//
// return = qtrue if keeping as MP3
//
// (note: the reason I pass in the unpacked size rather than working it out here is simply because I already have it)
//
qboolean MP3Stream_InitFromFile( sfx_t* sfx, byte *pbSrcData, int iSrcDatalen, const char *psSrcDataFilename, 
									int iMP3UnPackedSize, qboolean bStereoDesired /* = qfalse */
								)
{
	// first, make a decision based on size here as to whether or not it's worth it because of MP3 buffer space
	//	making small files much bigger (and therefore best left as WAV)...
	//
	
	if (cv_MP3overhead &&
			(
			//iSrcDatalen + sizeof(MP3STREAM) + FUZZY_AMOUNT < iMP3UnPackedSize
			iSrcDatalen + cv_MP3overhead->value < iMP3UnPackedSize
			)
		)
	{
		// ok, let's keep it as MP3 then...
		//
		float fMaxVol = 128;	// seems to be a reasonable typical default for maxvol (for lip synch). Naturally there's no #define I can use instead...

		MP3_ReadSpecialTagInfo(pbSrcData, iSrcDatalen, NULL, NULL, &fMaxVol );	// try and read a read maxvol from MP3 header
	
		// fill in some sfx_t fields...
		//
//		Q_strncpyz( sfx->name, psSrcDataFilename, sizeof(sfx->name) );					
		sfx->eSoundCompressionMethod = ct_MP3;
		sfx->fVolRange = fMaxVol;
		//sfx->width  = 2;
		sfx->iSoundLengthInSamples = ((iMP3UnPackedSize / 2/*sfx->width*/) / (44100 / dma.speed)) / (bStereoDesired?2:1);
		//
		// alloc mem for data and store it (raw MP3 in this case)...
		//
		sfx->pSoundData = (short *) SND_malloc( iSrcDatalen, sfx );
		memcpy( sfx->pSoundData, pbSrcData, iSrcDatalen );
		
		// now init the low-level MP3 stuff...
		//
		MP3STREAM SFX_MP3Stream = {0};	// important to init to all zeroes!
		char *psError = C_MP3Stream_DecodeInit( &SFX_MP3Stream, /*sfx->data*/ /*sfx->soundData*/ pbSrcData, iSrcDatalen,
												dma.speed,//(s_khz->value == 44)?44100:(s_khz->value == 22)?22050:11025,
												2/*sfx->width*/ * 8,
												bStereoDesired
												);
		SFX_MP3Stream.pbSourceData = (byte *) sfx->pSoundData;
		if (psError)
		{
			// This should never happen, since any errors or problems with the MP3 file would have stopped us getting
			//	to this whole function, but just in case...
			//
			Com_Printf(va(S_COLOR_YELLOW"File \"%s\": %s\n",psSrcDataFilename,psError));

			// This will leave iSrcDatalen bytes on the hunk stack (since you can't dealloc that), but MP3 files are
			//	usually small, and like I say, it should never happen.
			//
			// Strictly speaking, I should do a Z_Malloc above, then I could do a Z_Free if failed, else do a Hunk_Alloc
			//	to copy the Z_Malloc data into, then Z_Free, but for something that shouldn't happen it seemed bad to
			//	penalise the rest of the game with extra alloc demands.
			//
			return qfalse;	
		}

		// success ( ...on a plate).
		//
		// make a copy of the filled-in stream struct and attach to the sfx_t struct...
		//
				sfx->pMP3StreamHeader = (MP3STREAM *) Z_Malloc( sizeof(MP3STREAM), TAG_SND_MP3STREAMHDR );
		memcpy(	sfx->pMP3StreamHeader, &SFX_MP3Stream,		    sizeof(MP3STREAM) );
		//
		return qtrue;
	}

	return qfalse;
}



// decode one packet of MP3 data only (typical output size is 2304, or 2304*2 for stereo, so input size is less
//
// return is decoded byte count, else 0 for finished
//
int MP3Stream_Decode( LP_MP3STREAM lpMP3Stream, qboolean bDoingMusic )
{
	lpMP3Stream->iCopyOffset = 0;

	if (0)//!bDoingMusic)
	{
		// SOF2: need to make a local buffer up so we can decode the piece we want from a contiguous bitstream rather than
		//	this linklist junk...
		//
		// since MP3 packets are generally 416 or 417 bytes in length it seems reasonable to just find which linked-chunk
		//	the current read offset lies within then grab the next one as well (since they're 2048 bytes) and make one
		//	buffer with just the two concat'd together. Shouldn't be much of a processor hit.
		//
		sndBuffer *pChunk = (sndBuffer *) lpMP3Stream->pbSourceData;	
		//
		// may as well make this static to avoid cut down on stack-validation run-time...
		//
		static byte	byRawBuffer[SND_CHUNK_SIZE_BYTE*2];	// *2 for byte->short	// easily enough to decode one frame of MP3 data, most are 416 or 417 bytes
			
		// fast-forward to the correct chunk...
		//		
		int iBytesToSkipPast = lpMP3Stream->iSourceReadIndex;

		while (iBytesToSkipPast >= SND_CHUNK_SIZE_BYTE) 
		{
			pChunk = pChunk->next;			
			if (!pChunk) 
			{
				// err.... reading off the end of the data stream guys...
				//
				// pChunk = (sndBuffer *) lpMP3Stream->pbSourceData;	// restart
				return 0;	// ... 0 bytes decoded, so will just stop caller-decoder all nice and legal as EOS
			}
			iBytesToSkipPast -= SND_CHUNK_SIZE_BYTE;
		}
		// ok, pChunk is now the 2k or so chunk we're in the middle of...
		//
		int iChunk1BytesToCopy = SND_CHUNK_SIZE_BYTE - iBytesToSkipPast;
		memcpy(byRawBuffer,((byte *)pChunk->sndChunk) + iBytesToSkipPast, iChunk1BytesToCopy);
		//
		// concat next chunk on to this as well...
		//
		pChunk = pChunk->next;
		if (pChunk)
		{
			memcpy(byRawBuffer + iChunk1BytesToCopy, pChunk->sndChunk,	SND_CHUNK_SIZE_BYTE);
		}
		else
		{
			memset(byRawBuffer + iChunk1BytesToCopy, 0,					SND_CHUNK_SIZE_BYTE);
		}


		// now we need to backup some struct fields, fake 'em, do the lo-level call, then restore 'em...
		//
		byte *pbSourceData_Old	= lpMP3Stream->pbSourceData;
		int iSourceReadIndex_Old= lpMP3Stream->iSourceReadIndex;

		lpMP3Stream->pbSourceData	= &byRawBuffer[0];
		lpMP3Stream->iSourceReadIndex= 0;	// since this is zero, not the buffer offset within a chunk, we can play tricks further down when restoring

		unsigned int uiBytesDecoded = C_MP3Stream_Decode( lpMP3Stream );

		lpMP3Stream->iSourceReadIndex += iSourceReadIndex_Old;	// note '+=' rather than '=', to take account of movement.
		lpMP3Stream->pbSourceData	   = pbSourceData_Old;

		return uiBytesDecoded;
	}
	else
	{
		// SOF2 music, or EF1 anything...
		//
		return C_MP3Stream_Decode( lpMP3Stream );
	}
}

// returns qtrue for all ok
//
qboolean MP3Stream_Rewind( channel_t *ch )
{		
	ch->iMP3SlidingDecodeWritePos = 0;
	ch->iMP3SlidingDecodeWindowPos= 0;

/*
	char *psError = C_MP3Stream_Rewind( &ch->MP3StreamHeader );

	if (psError)
	{
		Com_Printf(S_COLOR_YELLOW"%s\n",psError);
		return qfalse;
	}

	return qtrue;
*/

	// speed opt, since I know I already have the right data setup here...
	//
	memcpy(&ch->MP3StreamHeader, ch->thesfx->pMP3StreamHeader, sizeof(ch->MP3StreamHeader));
	return qtrue;
}


// returns qtrue while still playing normally, else qfalse for either finished or request-offset-error
//
qboolean MP3Stream_GetSamples( channel_t *ch, int startingSampleNum, int count, short *buf, qboolean bStereo )
{
	qboolean qbStreamStillGoing = qtrue;

	const int iQuarterOfSlidingBuffer		=  sizeof(ch->MP3SlidingDecodeBuffer)/4;
	const int iThreeQuartersOfSlidingBuffer	= (sizeof(ch->MP3SlidingDecodeBuffer)*3)/4;

//	Com_Printf("startingSampleNum %d\n",startingSampleNum);

	count *= 2/* <- = SOF2; ch->sfx->width*/;	// count arg was for words, so double it for bytes;

	// convert sample number into a byte offset... (make new variable for clarity?)
	//
	startingSampleNum *= 2 /* <- = SOF2; ch->sfx->width*/ * (bStereo?2:1);

	if ( startingSampleNum < ch->iMP3SlidingDecodeWindowPos)
	{
		// what?!?!?!   smegging time travel needed or something?, forget it
		memset(buf,0,count);
		return qfalse;
	}

//	OutputDebugString(va("\nRequest: startingSampleNum %d, count %d\n",startingSampleNum,count));
//	OutputDebugString(va("WindowPos %d, WindowWritePos %d\n",ch->iMP3SlidingDecodeWindowPos,ch->iMP3SlidingDecodeWritePos));

//	qboolean _bDecoded = qfalse;

	while (!
		(
			(startingSampleNum			>= ch->iMP3SlidingDecodeWindowPos)
			&&
			(startingSampleNum + count	<  ch->iMP3SlidingDecodeWindowPos + ch->iMP3SlidingDecodeWritePos)
			)
			)
	{
//		if (!_bDecoded)
//		{
//			Com_Printf(S_COLOR_YELLOW"Decode needed!\n");
//		}
//		_bDecoded = qtrue;
//		OutputDebugString("Scrolling...");

		int _iBytesDecoded = MP3Stream_Decode( (LP_MP3STREAM) &ch->MP3StreamHeader, bStereo );	// stereo only for music, so this is safe
//		OutputDebugString(va("%d bytes decoded\n",_iBytesDecoded));
		if (_iBytesDecoded == 0)
		{
			// no more source data left so clear the remainder of the buffer...
			//
			memset(ch->MP3SlidingDecodeBuffer + ch->iMP3SlidingDecodeWritePos, 0, sizeof(ch->MP3SlidingDecodeBuffer)-ch->iMP3SlidingDecodeWritePos);
//			OutputDebugString("Finished\n");
			qbStreamStillGoing = qfalse;
			break;
		}
		else
		{
			memcpy(ch->MP3SlidingDecodeBuffer + ch->iMP3SlidingDecodeWritePos,ch->MP3StreamHeader.bDecodeBuffer,_iBytesDecoded);

			ch->iMP3SlidingDecodeWritePos += _iBytesDecoded;

			// if reached 3/4 of buffer pos, backscroll the decode window by one quarter...
			//
			if (ch->iMP3SlidingDecodeWritePos > iThreeQuartersOfSlidingBuffer)
			{
				memmove(ch->MP3SlidingDecodeBuffer, ((byte *)ch->MP3SlidingDecodeBuffer + iQuarterOfSlidingBuffer), iThreeQuartersOfSlidingBuffer);
				ch->iMP3SlidingDecodeWritePos -= iQuarterOfSlidingBuffer;
				ch->iMP3SlidingDecodeWindowPos+= iQuarterOfSlidingBuffer;
			}
		}
//		OutputDebugString(va("WindowPos %d, WindowWritePos %d\n",ch->iMP3SlidingDecodeWindowPos,ch->iMP3SlidingDecodeWritePos));
	}

//	if (!_bDecoded)
//	{
//		Com_Printf(S_COLOR_YELLOW"No decode needed\n");
//	}

	assert(startingSampleNum >= ch->iMP3SlidingDecodeWindowPos);
	memcpy( buf, ch->MP3SlidingDecodeBuffer + (startingSampleNum-ch->iMP3SlidingDecodeWindowPos), count);	

//	OutputDebugString("OK\n\n");

	return qbStreamStillGoing;
}


///////////// eof /////////////

