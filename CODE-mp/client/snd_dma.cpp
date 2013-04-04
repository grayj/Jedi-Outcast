
/*****************************************************************************
 * name:		snd_dma.c
 *
 * desc:		main control for any streaming sound output device
 *
 * $Archive: /MissionPack/code/client/snd_dma.c $
 * $Author: Mrelusive $ 
 * $Revision: 117 $
 * $Modtime: 6/06/01 2:35p $
 * $Date: 6/06/01 7:11p $
 *
 *****************************************************************************/

#include "snd_local.h"
#include "snd_mp3.h"

#include "client.h"

void S_Play_f(void);
void S_SoundList_f(void);
static void S_Music_f(void);

void S_Update_();
void S_StopAllSounds(void);
static void S_UpdateBackgroundTrack( void );
extern int RE_RegisterMedia_GetLevel(void);


//////////////////////////
//
// vars for bgrnd music track...
//
const int iMP3MusicStream_DiskBytesToRead = 10000;//4096;
const int iMP3MusicStream_DiskBufferSize = iMP3MusicStream_DiskBytesToRead * 2;//10;

typedef struct
{	
	int/*qboolean*/	bIsMP3;	// I wanted this kept as qboolean, but {0} init won't work with a typedef'd enum
	//
	// MP3 specific...
	//
	sfx_t		sfxMP3_Bgrnd;
	MP3STREAM	streamMP3_Bgrnd;	// this one is pointed at by the sfx_t's ptr, and is NOT the one the decoder uses every cycle
	channel_t	chMP3_Bgrnd;		// ... but the one in this struct IS.	
	//
	// MP3 disk streamer stuff... (if music is non-dynamic)
	//
	byte		byMP3MusicStream_DiskBuffer[iMP3MusicStream_DiskBufferSize];
	int			iMP3MusicStream_DiskReadPos;
	int			iMP3MusicStream_DiskWindowPos;
	//
	// MP3 disk-load stuff (for use during dynamic music, which is mem-resident)
	//
	byte		*pLoadedData;	// Z_Malloc, Z_Free
	int			iLoadedDataLen;	
	char		sLoadedDataName[MAX_QPATH];
	//
	// remaining dynamic fields...
	//
	int			iXFadeVolumeSeekTime;
	int			iXFadeVolumeSeekTo;	// when changing this, set the above timer to Sys_Milliseconds(). 
									//	Note that this should be thought of more as an up/down bool rather than as a 
									//	number now, in other words set it only to 0 or 255. I'll probably change this
									//	to actually be a bool later.
	int			iXFadeVolume;		// 0 = silent, 255 = max mixer vol, though still modulated via overall music_volume 
	qboolean	bActive;			
	//
	// Generic...
	//
	fileHandle_t s_backgroundFile;	// valid handle, else -1 if an MP3 (so that NZ compares still work)
	wavinfo_t	s_backgroundInfo;
	int			s_backgroundSamples;	
} MusicInfo_t;

// this now no longer supports dynamic music for MP codebase...
//
typedef enum 
{
	eBGRNDTRACK_SLOW = 0,	// for normal walking around
//	eBGRNDTRACK_FAST,		// for when stuff gets exciting
//	eBGRNDTRACK_FADE,		// the xfade channel
//	//
	eBGRNDTRACK_NUMBEROF
} MusicState_e;

#define fDYNAMIC_XFADE_SECONDS (1.0f)

static MusicInfo_t	tMusic_Info[eBGRNDTRACK_NUMBEROF]	= {0};
static MusicState_e	eMusic_State						= eBGRNDTRACK_SLOW;
static char			sMusic_BackgroundLoop[MAX_QPATH]	= {0};
//
//////////////////////////



// =======================================================================
// Internal sound data & structures
// =======================================================================

// only begin attenuating sound volumes when outside the FULLVOLUME range
#define		SOUND_FULLVOLUME	80

#define		SOUND_ATTENUATE		0.0008f

channel_t   s_channels[MAX_CHANNELS];
channel_t   loop_channels[MAX_CHANNELS];
int			numLoopChannels;

static int	s_soundStarted;
qboolean	s_soundMuted;

dma_t		dma;

static int			listener_number;
static vec3_t		listener_origin;
static vec3_t		listener_axis[3];

int			s_soundtime;		// sample PAIRS
int   		s_paintedtime; 		// sample PAIRS

// MAX_SFX may be larger than MAX_SOUNDS because
// of custom player sounds
#define		MAX_SFX			4096
sfx_t		s_knownSfx[MAX_SFX];
int			s_numSfx = 0;

#define		LOOP_HASH		128
static	sfx_t		*sfxHash[LOOP_HASH];

cvar_t		*s_volume;
cvar_t		*s_testsound;
cvar_t		*s_khz;
cvar_t		*s_show;
cvar_t		*s_mixahead;
cvar_t		*s_mixPreStep;
cvar_t		*s_musicVolume;
cvar_t		*s_musicMult;
cvar_t		*s_separation;
cvar_t		*s_doppler;
cvar_t		*s_CPUType;
cvar_t		*s_language;


static loopSound_t		loopSounds[MAX_GENTITIES];
static	channel_t		*freelist = NULL;

int						s_rawend;
portable_samplepair_t	s_rawsamples[MAX_RAW_SAMPLES];



// instead of clearing a whole channel_t struct, we're going to skip the MP3SlidingDecodeBuffer[] buffer in the middle...
//
#ifndef offsetof
#include <stddef.h>
#endif
static inline void Channel_Clear(channel_t *ch)
{
	// memset (ch, 0, sizeof(*ch));

	memset(ch,0,offsetof(channel_t,MP3SlidingDecodeBuffer));

	byte *const p = (byte *)ch + offsetof(channel_t,MP3SlidingDecodeBuffer) + sizeof(ch->MP3SlidingDecodeBuffer);

	memset(p,0,(sizeof(*ch) - offsetof(channel_t,MP3SlidingDecodeBuffer)) - sizeof(ch->MP3SlidingDecodeBuffer));
}



// ====================================================================
// User-setable variables
// ====================================================================

void S_SoundInfo_f(void) {	
	Com_Printf("----- Sound Info -----\n" );
	if (!s_soundStarted) {
		Com_Printf ("sound system not started\n");
	} else {
		if ( s_soundMuted ) {
			Com_Printf ("sound system is muted\n");
		}

		Com_Printf("%5d stereo\n", dma.channels - 1);
		Com_Printf("%5d samples\n", dma.samples);
		Com_Printf("%5d samplebits\n", dma.samplebits);
		Com_Printf("%5d submission_chunk\n", dma.submission_chunk);
		Com_Printf("%5d speed\n", dma.speed);
		Com_Printf("0x%x dma buffer\n", dma.buffer);
		if ( tMusic_Info[eBGRNDTRACK_SLOW].s_backgroundFile ) {
			Com_Printf("Background file: %s\n", sMusic_BackgroundLoop );
		} else {
			Com_Printf("No background file.\n" );
		}

	}
	S_DisplayFreeMemory();
	Com_Printf("----------------------\n" );
}



/*
================
S_Init
================
*/
void S_Init( void ) {
	cvar_t	*cv;
	qboolean	r;

	Com_Printf("\n------- sound initialization -------\n");

	s_volume = Cvar_Get ("s_volume", "0.8", CVAR_ARCHIVE);
	s_musicVolume = Cvar_Get ("s_musicvolume", "0.5", CVAR_ARCHIVE);

	//rww - multiply s_musicVolume by this value. Set in cgame when necessary.
	s_musicMult = Cvar_Get ("s_musicMult", "1", 0);

	s_separation = Cvar_Get ("s_separation", "0.5", CVAR_ARCHIVE);
	s_doppler = Cvar_Get ("s_doppler", "1", CVAR_ARCHIVE);
	s_khz = Cvar_Get ("s_khz", "22", CVAR_ARCHIVE);
	s_mixahead = Cvar_Get ("s_mixahead", "0.2", CVAR_ARCHIVE);

	s_mixPreStep = Cvar_Get ("s_mixPreStep", "0.05", CVAR_ARCHIVE);
	s_show = Cvar_Get ("s_show", "0", CVAR_CHEAT);
	s_testsound = Cvar_Get ("s_testsound", "0", CVAR_CHEAT);

	s_language = Cvar_Get("s_language","english",CVAR_ARCHIVE | CVAR_NORESTART);

	MP3_InitCvars();

	s_CPUType = Cvar_Get("sys_cpuid","",0);

// dontcha just love ID's defines sometimes?...
//
#if !( (defined __linux__ || __FreeBSD__ ) && (defined __i386__) )
#if	!id386
#else
	extern unsigned int uiMMXAvailable;
	uiMMXAvailable = !!(s_CPUType->integer >= CPUID_INTEL_MMX);
#endif
#endif


	cv = Cvar_Get ("s_initsound", "1", 0);
	if ( !cv->integer ) {
		Com_Printf ("not initializing.\n");
		Com_Printf("------------------------------------\n");
		return;
	}

	Cmd_AddCommand("play", S_Play_f);
	Cmd_AddCommand("music", S_Music_f);
	Cmd_AddCommand("soundlist", S_SoundList_f);
	Cmd_AddCommand("soundinfo", S_SoundInfo_f);
	Cmd_AddCommand("soundstop", S_StopAllSounds);

	r = SNDDMA_Init();
	Com_Printf("------------------------------------\n");

	if ( r ) {
		s_soundStarted = 1;
		s_soundMuted = (qboolean)1;
//		s_numSfx = 0;

		s_soundtime = 0;
		s_paintedtime = 0;

		S_StopAllSounds ();

		S_SoundInfo_f();
	}

}


void S_ChannelFree(channel_t *v) {
	v->thesfx = NULL;
	*(channel_t **)v = freelist;
	freelist = (channel_t*)v;
}

channel_t*	S_ChannelMalloc() {
	channel_t *v;
	if (freelist == NULL) {
		return NULL;
	}
	v = freelist;
	freelist = *(channel_t **)freelist;
	v->allocTime = Com_Milliseconds();
	return v;
}

void S_ChannelSetup() {
	channel_t *p, *q;

	// clear all the sounds so they don't
	Com_Memset( s_channels, 0, sizeof( s_channels ) );

	p = s_channels;;
	q = p + MAX_CHANNELS;
	while (--q > p) {
		*(channel_t **)q = q-1;
	}
	
	*(channel_t **)q = NULL;
	freelist = p + MAX_CHANNELS - 1;
	Com_DPrintf("Channel memory manager started\n");
}

// =======================================================================
// Shutdown sound engine
// =======================================================================

void S_Shutdown( void ) {
	if ( !s_soundStarted ) {
		return;
	}

	SNDDMA_Shutdown();

	s_soundStarted = 0;

    Cmd_RemoveCommand("play");
	Cmd_RemoveCommand("music");
	Cmd_RemoveCommand("stopsound");
	Cmd_RemoveCommand("soundlist");
	Cmd_RemoveCommand("soundinfo");
}


// =======================================================================
// Load a sound
// =======================================================================

/*
================
return a hash value for the sfx name
================
*/
static long S_HashSFXName(const char *name) {
	int		i;
	long	hash;
	char	letter;

	hash = 0;
	i = 0;
	while (name[i] != '\0') {
		letter = tolower(name[i]);
		if (letter =='.') break;				// don't include extension
		if (letter =='\\') letter = '/';		// damn path names
		hash+=(long)(letter)*(i+119);
		i++;
	}
	hash &= (LOOP_HASH-1);
	return hash;
}


/*
==================
S_FindName

Will allocate a new sfx if it isn't found
==================
*/
sfx_t *S_FindName( const char *name ) {
	int		i;
	int		hash;

	sfx_t	*sfx;

	if (!name) {
		Com_Error (ERR_FATAL, "S_FindName: NULL\n");
	}
	if (!name[0]) {
		Com_Error (ERR_FATAL, "S_FindName: empty name\n");
	}

	if (strlen(name) >= MAX_QPATH) {
		Com_Error (ERR_FATAL, "Sound name too long: %s", name);
	}

	char sSoundNameNoExt[MAX_QPATH];
	COM_StripExtension(name,sSoundNameNoExt);

	hash = S_HashSFXName(sSoundNameNoExt);

	sfx = sfxHash[hash];
	// see if already loaded
	while (sfx) {
		if (!Q_stricmp(sfx->sSoundName, sSoundNameNoExt) ) {
			return sfx;
		}
		sfx = sfx->next;
	}
/*
	// find a free sfx
	for (i=0 ; i < s_numSfx ; i++) {
		if (!s_knownSfx[i].soundName[0]) {
			break;
		}
	}
*/
	i = s_numSfx;	//we don't clear the soundName after failed loads any more, so it'll always be the last entry
		
	if (s_numSfx == MAX_SFX) 
	{
		// ok, no sfx's free, but are there any with defaultSound set? (which the registering ent will never
		//	see because he gets zero returned if it's default...)
		//
		for (i=0 ; i < s_numSfx ; i++) {
			if (s_knownSfx[i].bDefaultSound) {
				break;
			}
		}

		if (i==s_numSfx)
		{	  
			// genuinely out of handles...

			// if we ever reach this, let me know and I'll either boost the array or put in a map-used-on
			//	reference to enable sfx_t recycling. TA codebase relies on being able to have structs for every sound
			//	used anywhere, ever, all at once (though audio bit-buffer gets recycled). SOF1 used about 1900 distinct
			//	events, so current MAX_SFX limit should do, or only need a small boost...	-ste
			//

			Com_Error (ERR_FATAL, "S_FindName: out of sfx_t");
		}
	}
	else
	{
		s_numSfx++;
	}
	
	sfx = &s_knownSfx[i];
	memset (sfx, 0, sizeof(*sfx));
	Q_strncpyz(sfx->sSoundName, sSoundNameNoExt, sizeof(sfx->sSoundName));

	sfx->next = sfxHash[hash];
	sfxHash[hash] = sfx;

	return sfx;
}

/*
=================
S_DefaultSound
=================
*/
void S_DefaultSound( sfx_t *sfx ) {
	
	int		i;

	sfx->iSoundLengthInSamples	= 512;								// #samples, ie shorts
	sfx->pSoundData				= (short *)	SND_malloc(512*2, sfx);	// ... so *2 for alloc bytes	
	sfx->bInMemory				= qtrue;
	
	for ( i=0 ; i < sfx->iSoundLengthInSamples ; i++ ) 
	{
		sfx->pSoundData[i] = i;
	}
}


/*
===================
S_DisableSounds

Disables sounds until the next S_BeginRegistration.
This is called when the hunk is cleared and the sounds
are no longer valid.
===================
*/
void S_DisableSounds( void ) {
	S_StopAllSounds();
	s_soundMuted = qtrue;
}

/*
=====================
S_BeginRegistration

=====================
*/
void S_BeginRegistration( void ) 
{
	s_soundMuted = qfalse;		// we can play again

	if (s_numSfx == 0) {
		SND_setup();

		s_numSfx = 0;
		Com_Memset( s_knownSfx, 0, sizeof( s_knownSfx ) );
		Com_Memset(sfxHash, 0, sizeof(sfx_t *)*LOOP_HASH);

		S_RegisterSound(DEFAULT_SOUND_NAME);
		S_DefaultSound( &s_knownSfx[0] );
	}
}


/*
==================
S_RegisterSound

Creates a default buzz sound if the file can't be loaded
==================
*/
sfxHandle_t	S_RegisterSound( const char *name) 
{
	sfx_t	*sfx;

	if (!s_soundStarted) {
		return 0;
	}

	if (!name) 
	{
		Com_Error (ERR_FATAL, "S_RegisterSound: NULL name\n");
	}
	if (!name[0]) 
	{
		Com_Error (ERR_FATAL, "S_RegisterSound: empty name\n");
	}

	if ( strlen( name ) >= MAX_QPATH ) {		
		Com_Error (ERR_FATAL, "S_RegisterSound: Sound name exceeds MAX_QPATH - %s\n", name );
		return 0;
	}

	sfx = S_FindName( name );

	SND_TouchSFX(sfx);

	if ( sfx->bDefaultSound )
		return 0;

	if ( sfx->pSoundData ) {		
		return sfx - s_knownSfx;
	}

	sfx->bInMemory = qfalse;

	S_memoryLoad(sfx);

	if ( sfx->bDefaultSound )	{
		// Suppress error for inline sounds
		if(Q_stricmp(sfx->sSoundName, DEFAULT_SOUND_NAME)){
			Com_DPrintf( S_COLOR_YELLOW "WARNING: could not find %s - using default\n", sfx->sSoundName );
		}
		return 0;
	}

	return sfx - s_knownSfx;
}

void S_memoryLoad(sfx_t	*sfx) 
{
	// load the sound file...
	//
	if ( !S_LoadSound( sfx ) ) 
	{
//		Com_Printf( S_COLOR_YELLOW "WARNING: couldn't load sound: %s\n", sfx->sSoundName );
		sfx->bDefaultSound = qtrue;
	}
	sfx->bInMemory = qtrue;
}


//=============================================================================

/*
=================
S_SpatializeOrigin

Used for spatializing s_channels
=================
*/
void S_SpatializeOrigin (vec3_t origin, int master_vol, int *left_vol, int *right_vol)
{
    vec_t		dot;
    vec_t		dist;
    vec_t		lscale, rscale, scale;
    vec3_t		source_vec;
    vec3_t		vec;

	float dist_mult = SOUND_ATTENUATE;	// the more you divide it down, the further away the sound can be heard
	
	// calculate stereo seperation and distance attenuation
	VectorSubtract(origin, listener_origin, source_vec);

	dist = VectorNormalize(source_vec);
	dist -= SOUND_FULLVOLUME;
	if (dist < 0)
		dist = 0;			// close enough to be at full volume
	dist *= dist_mult;		// different attenuation levels
	
	VectorRotate( source_vec, listener_axis, vec );

	dot = -vec[1];

	if (dma.channels == 1)
	{ // no attenuation = no spatialization
		rscale = 1.0;
		lscale = 1.0;
	}
	else
	{
		rscale = 0.5 * (1.0 + dot);
		lscale = 0.5 * (1.0 - dot);
		//rscale = s_separation->value + ( 1.0 - s_separation->value ) * dot;
		//lscale = s_separation->value - ( 1.0 - s_separation->value ) * dot;
		if ( rscale < 0 ) {
			rscale = 0;
		}
		if ( lscale < 0 ) {
			lscale = 0;
		}
	}

	// add in distance effect
	scale = (1.0 - dist) * rscale;
	*right_vol = (master_vol * scale);
	if (*right_vol < 0)
		*right_vol = 0;

	scale = (1.0 - dist) * lscale;
	*left_vol = (master_vol * scale);
	if (*left_vol < 0)
		*left_vol = 0;
}

static qboolean S_CheckChannelStomp( int chan1, int chan2 )
{
	if ( chan1 == chan2 )
	{
		return qtrue;
	}

	// Hmmmm. without CHAN_VOICE_ATTEN this logic just dups the above, so for now...
	//
//	if ( ( chan1 == CHAN_VOICE /* || chan1 == CHAN_VOICE_ATTEN*/ ) && ( chan2 == CHAN_VOICE /*|| chan2 == CHAN_VOICE_ATTEN*/ ) )
//	{
//		return qtrue;
//	}

	return qfalse;
}



channel_t *S_PickChannel(int entnum, int entchannel)
{
    int			ch_idx;
	channel_t	*ch, *ch_firstToDie;
	qboolean	foundChan = qfalse;

	if ( entchannel<0 ) {
		Com_Error (ERR_DROP, "S_PickChannel: entchannel<0");
	}

	// Check for replacement sound, or find the best one to replace

    ch_firstToDie = &s_channels[0];

	for ( int pass = 0; (pass < ((entchannel == CHAN_AUTO)?1:2)) && !foundChan; pass++ )
	{
		for (ch_idx = 0,
			ch = &s_channels[ch_idx]; ch_idx < MAX_CHANNELS ; ch_idx++, ch++ ) 
		{
			if ( entchannel == CHAN_AUTO || pass > 0 )
			{//if we're on the second pass, or we're doing a CHAN_AUTO then just find the first open chan
				if ( !ch->thesfx )
				{//grab the first open channel
					ch_firstToDie = ch;
					foundChan = qtrue;
					break;
				}
			}
			else if ( ch->entnum == entnum 
					 //&& (/*entchannel != CHAN_AMBIENT*/1 || pass)	// don't override Ambient sounds unless 2nd pass (ie all channels in use)
					 && S_CheckChannelStomp( ch->entchannel, entchannel ) 
					 ) 
			{
				// always override sound from same entity
				if ( s_show->integer == 1 && ch->thesfx ) {
					Com_Printf( S_COLOR_YELLOW"...overrides %s\n", ch->thesfx->sSoundName );
					ch->thesfx = 0;	//just to clear the next error msg
				}
				ch_firstToDie = ch;
				foundChan = qtrue;
				break;
			}

			// don't let anything else override local player sounds
			if ( ch->entnum == listener_number 	&& entnum != listener_number && ch->thesfx) {
				continue;
			}

			// Ignore this, loopSounds are different array under TA codebase...
			//
			//			// don't override loop sounds  
			//			if ( ch->loopSound ) {			
			//				continue;
			//			}

			if ( ch->startSample < ch_firstToDie->startSample ) {
				ch_firstToDie = ch;
			}
		}
	}

	if ( s_show->integer == 1 && ch_firstToDie->thesfx ) {
		Com_Printf( S_COLOR_RED"***kicking %s\n", ch_firstToDie->thesfx->sSoundName );
	}

	Channel_Clear(ch_firstToDie);	// memset (ch_firstToDie, 0, sizeof(*ch_firstToDie));
	
    return ch_firstToDie;
}


/*
====================
S_MuteSound

Gets the specified ent/channel and mutes any sound currently playing on it
====================
*/
void S_MuteSound(int entityNum, int entchannel)
{
	channel_t	*ch;

	if (entchannel < 1)
	{
		return;
	}

	ch = S_PickChannel( entityNum, entchannel );
	
	if (!ch)
	{
		return;
	}

	ch->master_vol = 0; //just kill the volume and leave the rest alone, as to not actually interrupt anything expecting the sound to go through
	ch->leftvol = 0;
	ch->rightvol = 0;
}


// =======================================================================
// Start a sound effect
// =======================================================================

/*
====================
S_StartSound

Validates the parms and ques the sound up
if pos is NULL, the sound will be dynamically sourced from the entity
Entchannel 0 will never override a playing sound
====================
*/
void S_StartSound(vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfxHandle ) {
	channel_t	*ch;
	sfx_t		*sfx;
//	int			i, oldest, chosen;

	if ( !s_soundStarted || s_soundMuted ) {
		return;
	}

	if ( !origin && ( entityNum < 0 || entityNum > MAX_GENTITIES ) ) {
		Com_Error( ERR_DROP, "S_StartSound: bad entitynum %i", entityNum );
	}

	if ( sfxHandle < 0 || sfxHandle >= s_numSfx ) {
		Com_Error( ERR_DROP, "S_StartSound: handle %i out of range", sfxHandle );
	}

	sfx = &s_knownSfx[ sfxHandle ];
	if (sfx->bInMemory == qfalse) {
		S_memoryLoad(sfx);
	}
	SND_TouchSFX(sfx);

	if ( s_show->integer == 1 ) {
		Com_Printf( "%i : %s\n", s_paintedtime, sfx->sSoundName );
	}

//	Com_Printf("playing %s\n", sfx->soundName);

// pick a channel to play on
//---------------------------------	
	ch = S_PickChannel(entityNum, entchannel);
	ch->allocTime = sfx->iLastTimeUsed;
/*	ch = S_ChannelMalloc();	// entityNum, entchannel);
	if (!ch) {
		ch = s_channels;

		oldest = sfx->lastTimeUsed;
		chosen = -1;
		for ( i = 0 ; i < MAX_CHANNELS ; i++, ch++ ) {
			if (ch->entnum != listener_number && ch->entnum == entityNum && ch->allocTime<oldest && ch->entchannel != CHAN_ANNOUNCER) {
				oldest = ch->allocTime;
				chosen = i;
			}
		}
		if (chosen == -1) {
			ch = s_channels;
			for ( i = 0 ; i < MAX_CHANNELS ; i++, ch++ ) {
				if (ch->entnum != listener_number && ch->allocTime<oldest && ch->entchannel != CHAN_ANNOUNCER) {
					oldest = ch->allocTime;
					chosen = i;
				}
			}
			if (chosen == -1) {				
				Com_Printf("S_StartSound(): dropping sound \"%s\"\n",sfx->soundName);
				return;
			}
		}
		ch = &s_channels[chosen];
		ch->allocTime = sfx->lastTimeUsed;
	}
*/
//---------------------------------

	if (origin) {
		VectorCopy (origin, ch->origin);
		ch->fixed_origin = qtrue;
	} else {
		ch->fixed_origin = qfalse;
	}

	ch->master_vol = 240;
	ch->entnum = entityNum;
	ch->thesfx = sfx;
	ch->startSample = START_SAMPLE_IMMEDIATE;
	ch->entchannel = entchannel;
	ch->leftvol = ch->master_vol;		// these will get calced at next spatialize
	ch->rightvol = ch->master_vol;		// unless the game isn't running
	ch->doppler = qfalse;

	// EF1 also had this, do we want it?
/*	
	if (entchannel < CHAN_AMBIENT && entityNum == listener_number) {	//only do it for body sounds not local sounds
		ch->master_vol = SOUND_MAXVOL * SOUND_FMAXVOL;	//this won't be attenuated so let it scale down
	}
	(SOUND_FMAXVOL = 0.75f)
	(SOUND_MAXVOL = 255)
*/
//	if ( entchannel == CHAN_VOICE )//|| entchannel == CHAN_VOICE_ATTEN ) 
//	{
//		s_entityWavVol[ ch->entnum ] = -1;	//we've started the sound but it's silent for now
//	}

	if (sfx->pMP3StreamHeader)	// -ste
	{
		memcpy(&ch->MP3StreamHeader,sfx->pMP3StreamHeader,	sizeof(ch->MP3StreamHeader));
		ch->iMP3SlidingDecodeWritePos = 0;
		ch->iMP3SlidingDecodeWindowPos= 0;
	}
	else
	{
		memset(&ch->MP3StreamHeader,0,						sizeof(ch->MP3StreamHeader));
	}

}


/*
==================
S_StartLocalSound
==================
*/
void S_StartLocalSound( sfxHandle_t sfxHandle, int channelNum ) {
	if ( !s_soundStarted || s_soundMuted ) {
		return;
	}

	if ( sfxHandle < 0 || sfxHandle >= s_numSfx ) {
		Com_Printf( S_COLOR_YELLOW, "S_StartLocalSound: handle %i out of range\n", sfxHandle );
		return;
	}

	S_StartSound (NULL, listener_number, channelNum, sfxHandle );
}


/*
==================
S_StartLocalLoopingSound
==================
*/
void S_StartLocalLoopingSound( sfxHandle_t sfxHandle) {
	vec3_t nullVec = {0,0,0};

	if ( !s_soundStarted || s_soundMuted ) {
		return;
	}

	if ( sfxHandle < 0 || sfxHandle >= s_numSfx ) {
		Com_Error( ERR_DROP, "S_StartLocalLoopingSound: handle %i out of range", sfxHandle );
	}

	S_AddLoopingSound( listener_number, nullVec, nullVec, sfxHandle);//, CHAN_AUTO );
}


/*
==================
S_ClearSoundBuffer

If we are about to perform file access, clear the buffer
so sound doesn't stutter.
==================
*/
void S_ClearSoundBuffer( void ) {
	int		clear;
		
	if (!s_soundStarted)
		return;

	// stop looping sounds
	Com_Memset(loopSounds, 0, MAX_GENTITIES*sizeof(loopSound_t));
	Com_Memset(loop_channels, 0, MAX_CHANNELS*sizeof(channel_t));
	numLoopChannels = 0;

	S_ChannelSetup();

	s_rawend = 0;

	if (dma.samplebits == 8)
		clear = 0x80;
	else
		clear = 0;

	SNDDMA_BeginPainting ();
	if (dma.buffer)
		Com_Memset(dma.buffer, clear, dma.samples * dma.samplebits/8);
	SNDDMA_Submit ();
}


/*
==================
S_StopAllSounds
==================
*/
void S_StopAllSounds(void) {
	if ( !s_soundStarted ) {
		return;
	}

	// stop the background music
	S_StopBackgroundTrack();

	S_ClearSoundBuffer ();
}

/*
==============================================================

continuous looping sounds are added each frame

==============================================================
*/

void S_StopLoopingSound(int entityNum) {
	loopSounds[entityNum].active = qfalse;
//	loopSounds[entityNum].sfx = 0;
	loopSounds[entityNum].kill = qfalse;
}

/*
==================
S_ClearLoopingSounds

==================
*/
void S_ClearLoopingSounds( qboolean killall ) {
	int i;
	for ( i = 0 ; i < MAX_GENTITIES ; i++) {
		if (killall || loopSounds[i].kill == qtrue || (loopSounds[i].sfx && loopSounds[i].sfx->iSoundLengthInSamples == 0)) {
			loopSounds[i].kill = qfalse;
			S_StopLoopingSound(i);
		}
	}
	numLoopChannels = 0;
}

/*
==================
S_AddLoopingSound

Called during entity generation for a frame
Include velocity in case I get around to doing doppler...
==================
*/
void S_AddLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfxHandle) {
	sfx_t *sfx;

	if ( !s_soundStarted || s_soundMuted ) {
		return;
	}

	if ( sfxHandle < 0 || sfxHandle >= s_numSfx ) {
		Com_Error( ERR_DROP, "S_AddLoopingSound: handle %i out of range", sfxHandle );
	}

	sfx = &s_knownSfx[ sfxHandle ];
	if (sfx->bInMemory == qfalse) {
		S_memoryLoad(sfx);
	}
	SND_TouchSFX(sfx);

	if ( !sfx->iSoundLengthInSamples ) {
		Com_Error( ERR_DROP, "%s has length 0", sfx->sSoundName );
	}

	VectorCopy( origin, loopSounds[entityNum].origin );
	VectorCopy( velocity, loopSounds[entityNum].velocity );
	loopSounds[entityNum].active = qtrue;
	loopSounds[entityNum].kill = qtrue;
	loopSounds[entityNum].doppler = qfalse;
	loopSounds[entityNum].oldDopplerScale = 1.0;
	loopSounds[entityNum].dopplerScale = 1.0;
	loopSounds[entityNum].sfx = sfx;
/*
	if (VectorLengthSquared(velocity)>0.0) {
		vec3_t	out;
		float	lena, lenb;

		loopSounds[entityNum].doppler = qtrue;
		lena = DistanceSquared(loopSounds[listener_number].origin, loopSounds[entityNum].origin);
		VectorAdd(loopSounds[entityNum].origin, loopSounds[entityNum].velocity, out);
		lenb = DistanceSquared(loopSounds[listener_number].origin, out);
		if ((loopSounds[entityNum].framenum+1) != cls.framecount) {
			loopSounds[entityNum].oldDopplerScale = 1.0;
		} else {
			loopSounds[entityNum].oldDopplerScale = loopSounds[entityNum].dopplerScale;
		}
		loopSounds[entityNum].dopplerScale = lenb/(lena*100);
		if (loopSounds[entityNum].dopplerScale<0.5) {
			loopSounds[entityNum].dopplerScale = 0.5;
		}
	}
*/
	loopSounds[entityNum].framenum = cls.framecount;	
}


/*
==================
S_AddLoopingSound

Called during entity generation for a frame
Include velocity in case I get around to doing doppler...
==================
*/
void S_AddRealLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfxHandle ) {
	sfx_t *sfx;

	if ( !s_soundStarted || s_soundMuted ) {
		return;
	}

	if ( sfxHandle < 0 || sfxHandle >= s_numSfx ) {
		Com_Printf( S_COLOR_YELLOW, "S_AddRealLoopingSound: handle %i out of range\n", sfxHandle );
		return;
	}

	sfx = &s_knownSfx[ sfxHandle ];

	if (sfx->bInMemory == qfalse) {
		S_memoryLoad(sfx);
	}
	SND_TouchSFX(sfx);

	if ( !sfx->iSoundLengthInSamples ) {
		Com_Error( ERR_DROP, "%s has length 0", sfx->sSoundName );
	}
	VectorCopy( origin, loopSounds[entityNum].origin );
	VectorCopy( velocity, loopSounds[entityNum].velocity );
	loopSounds[entityNum].sfx = sfx;
	loopSounds[entityNum].active = qtrue;
	loopSounds[entityNum].kill = qfalse;
	loopSounds[entityNum].doppler = qfalse;
}


// returns qtrue if ok to continue, else qfalse if all channels filled up this frame...
//
static qboolean LoopSound_ChannelInit(loopSound_t *pLoopSound, int iLeftVol, int iRightVol)
{
	// allocate a channel
	//
	channel_t *ch = &loop_channels[numLoopChannels];
	
	if (iLeftVol > 255) {
		iLeftVol = 255;
	}
	if (iRightVol > 255) {
		iRightVol = 255;
	}
	
	ch->master_vol		= 255;
	ch->leftvol			= iLeftVol;
	ch->rightvol		= iRightVol;
	ch->thesfx			= pLoopSound->sfx;
	ch->doppler			= pLoopSound->doppler;
	ch->dopplerScale	= pLoopSound->dopplerScale;
	ch->oldDopplerScale = pLoopSound->oldDopplerScale;

	// you cannot use MP3 files here because they offer only streaming access, not random
	//
	if (pLoopSound->sfx->pMP3StreamHeader)
	{
		Com_Error( ERR_DROP, "LoopSound_ChannelInit(): Cannot use streamed MP3 files here for random access (%s)\n",pLoopSound->sfx->sSoundName );
	}
	else
	{
		memset( &ch->MP3StreamHeader, 0, sizeof(ch->MP3StreamHeader) );
	}

	numLoopChannels++;
	if (numLoopChannels == MAX_CHANNELS) {
		return qfalse;
	}

	return qtrue;
}

// returns qfalse if sound would be inaudible, else qtrue for go ahead and play it this frame...
//
static qboolean LoopSound_SetupVolume(loopSound_t *pLoopSound, int time, int &iLeftTotal, int &iRightTotal)
{
	if (pLoopSound->kill) 
	{
		S_SpatializeOrigin( pLoopSound->origin, 240, &iLeftTotal, &iRightTotal);	// 3d
	} 
	else 
	{
		S_SpatializeOrigin( pLoopSound->origin, 180/*90*/,  &iLeftTotal, &iRightTotal);	// sphere
	}

	pLoopSound->sfx->iLastTimeUsed = time;

	if (iLeftTotal == 0 && iRightTotal == 0)
		return qfalse;	// not audible

	return qtrue;
}



/*
==================
S_AddLoopSounds

Spatialize all of the looping sounds.
All sounds are on the same cycle, so any duplicates can just
sum up the channel multipliers.
==================
*/
void S_AddLoopSounds (void) {
	int			i, time;
	int			left_total, right_total;
	loopSound_t	*loop;
	static int	loopFrame;

	numLoopChannels = 0;

	time = Com_Milliseconds();
	loopFrame++;

	// now do the standard ones...
	//	
	for ( i = 0 ; i < MAX_GENTITIES ; i++) 
	{
		loop = &loopSounds[i];
		
		if ( !loop->active	
			// this next test is pointless, since ->mergeFrame is never set - remove field? -ste.
			// || loop->mergeFrame == loopFrame 	// already merged into an earlier sound
			)
		{
			continue;	
		}

		if (LoopSound_SetupVolume(loop, time, left_total, right_total))
		{
			if (!LoopSound_ChannelInit(loop, left_total, right_total))
				return;	// all looping channels occupied
		}
	}
}

//=============================================================================

/*
=================
S_ByteSwapRawSamples

If raw data has been loaded in little endien binary form, this must be done.
If raw data was calculated, as with ADPCM, this should not be called.
=================
*/
void S_ByteSwapRawSamples( int samples, int width, int s_channels, const byte *data ) {
	int		i;

	if ( width != 2 ) {
		return;
	}
	if ( LittleShort( 256 ) == 256 ) {
		return;
	}

	if ( s_channels == 2 ) {
		samples <<= 1;
	}
	for ( i = 0 ; i < samples ; i++ ) {
		((short *)data)[i] = LittleShort( ((short *)data)[i] );
	}
}

portable_samplepair_t *S_GetRawSamplePointer() {
	return s_rawsamples;
}

/*
============
S_RawSamples

Music streaming
============
*/
void S_RawSamples( int samples, int rate, int width, int s_channels, const byte *data, float volume ) {
	int		i;
	int		src, dst;
	float	scale;
	int		intVolume;

	if ( !s_soundStarted || s_soundMuted ) {
		return;
	}

	intVolume = 256 * volume;

	if ( s_rawend < s_soundtime ) {
		Com_DPrintf( "S_RawSamples: resetting minimum: %i < %i\n", s_rawend, s_soundtime );
		s_rawend = s_soundtime;
	}

	scale = (float)rate / dma.speed;

//Com_Printf ("%i < %i < %i\n", s_soundtime, s_paintedtime, s_rawend);
	if (s_channels == 2 && width == 2)
	{
		if (scale == 1.0)
		{	// optimized case
			for (i=0 ; i<samples ; i++)
			{
				dst = s_rawend&(MAX_RAW_SAMPLES-1);
				s_rawend++;
				s_rawsamples[dst].left = ((short *)data)[i*2] * intVolume;
				s_rawsamples[dst].right = ((short *)data)[i*2+1] * intVolume;
			}
		}
		else
		{
			for (i=0 ; ; i++)
			{
				src = i*scale;
				if (src >= samples)
					break;
				dst = s_rawend&(MAX_RAW_SAMPLES-1);
				s_rawend++;
				s_rawsamples[dst].left = ((short *)data)[src*2] * intVolume;
				s_rawsamples[dst].right = ((short *)data)[src*2+1] * intVolume;
			}
		}
	}
	else if (s_channels == 1 && width == 2)
	{
		for (i=0 ; ; i++)
		{
			src = i*scale;
			if (src >= samples)
				break;
			dst = s_rawend&(MAX_RAW_SAMPLES-1);
			s_rawend++;
			s_rawsamples[dst].left = ((short *)data)[src] * intVolume;
			s_rawsamples[dst].right = ((short *)data)[src] * intVolume;
		}
	}
	else if (s_channels == 2 && width == 1)
	{
		intVolume *= 256;

		for (i=0 ; ; i++)
		{
			src = i*scale;
			if (src >= samples)
				break;
			dst = s_rawend&(MAX_RAW_SAMPLES-1);
			s_rawend++;
			s_rawsamples[dst].left = ((char *)data)[src*2] * intVolume;
			s_rawsamples[dst].right = ((char *)data)[src*2+1] * intVolume;
		}
	}
	else if (s_channels == 1 && width == 1)
	{
		intVolume *= 256;

		for (i=0 ; ; i++)
		{
			src = i*scale;
			if (src >= samples)
				break;
			dst = s_rawend&(MAX_RAW_SAMPLES-1);
			s_rawend++;
			s_rawsamples[dst].left = (((byte *)data)[src]-128) * intVolume;
			s_rawsamples[dst].right = (((byte *)data)[src]-128) * intVolume;
		}
	}

	if ( s_rawend > s_soundtime + MAX_RAW_SAMPLES ) {
		Com_DPrintf( "S_RawSamples: overflowed %i > %i\n", s_rawend, s_soundtime );
	}
}

//=============================================================================

/*
=====================
S_UpdateEntityPosition

let the sound system know where an entity currently is
======================
*/
void S_UpdateEntityPosition( int entityNum, const vec3_t origin ) {
	if ( entityNum < 0 || entityNum > MAX_GENTITIES ) {
		Com_Error( ERR_DROP, "S_UpdateEntityPosition: bad entitynum %i", entityNum );
	}
	VectorCopy( origin, loopSounds[entityNum].origin );
}


/*
============
S_Respatialize

Change the volumes of all the playing sounds for changes in their positions
============
*/
void S_Respatialize( int entityNum, const vec3_t head, vec3_t axis[3], int inwater ) {
	int			i;
	channel_t	*ch;
	vec3_t		origin;

	if ( !s_soundStarted || s_soundMuted ) {
		return;
	}

	listener_number = entityNum;
	VectorCopy(head, listener_origin);
	VectorCopy(axis[0], listener_axis[0]);
	VectorCopy(axis[1], listener_axis[1]);
	VectorCopy(axis[2], listener_axis[2]);

	// update spatialization for dynamic sounds	
	ch = s_channels;
	for ( i = 0 ; i < MAX_CHANNELS ; i++, ch++ ) {
		if ( !ch->thesfx ) {
			continue;
		}
		// anything coming from the view entity will always be full volume
		if (ch->entnum == listener_number) {
			ch->leftvol = ch->master_vol;
			ch->rightvol = ch->master_vol;
		} else {
			if (ch->fixed_origin) {
				VectorCopy( ch->origin, origin );
			} else {
				VectorCopy( loopSounds[ ch->entnum ].origin, origin );
			}

			S_SpatializeOrigin (origin, ch->master_vol, &ch->leftvol, &ch->rightvol);
		}
	}

	// add loopsounds
	S_AddLoopSounds ();
}


/*
========================
S_ScanChannelStarts

Returns qtrue if any new sounds were started since the last mix
========================
*/
qboolean S_ScanChannelStarts( void ) {
	channel_t		*ch;
	int				i;
	qboolean		newSamples;

	newSamples = qfalse;
	ch = s_channels;

	for (i=0; i<MAX_CHANNELS ; i++, ch++) {
		if ( !ch->thesfx ) {
			continue;
		}
		// if this channel was just started this frame,
		// set the sample count to it begins mixing
		// into the very first sample
		if ( ch->startSample == START_SAMPLE_IMMEDIATE ) {
			ch->startSample = s_paintedtime;
			newSamples = qtrue;
			continue;
		}

		// if it is completely finished by now, clear it
		if ( ch->startSample + (ch->thesfx->iSoundLengthInSamples) <= s_paintedtime ) {
			S_ChannelFree(ch);
		}
	}

	return newSamples;
}

/*
============
S_Update

Called once each time through the main loop
============
*/
void S_Update( void ) {
	int			i;
	int			total;
	channel_t	*ch;

	if ( !s_soundStarted || s_soundMuted ) {
		Com_DPrintf ("not started or muted\n");
		return;
	}

	//
	// debugging output
	//
	if ( s_show->integer == 2 ) {
		total = 0;
		ch = s_channels;
		for (i=0 ; i<MAX_CHANNELS; i++, ch++) {
			if (ch->thesfx && (ch->leftvol || ch->rightvol) ) {
				Com_Printf ("%f %f %s\n", ch->leftvol, ch->rightvol, ch->thesfx->sSoundName);
				total++;
			}
		}
		
		Com_Printf ("----(%i)---- painted: %i\n", total, s_paintedtime);
	}

	// add raw data from streamed samples
	S_UpdateBackgroundTrack();

	// mix some sound
	S_Update_();
}

void S_GetSoundtime(void)
{
	int		samplepos;
	static	int		buffers;
	static	int		oldsamplepos;
	int		fullsamples;
	
	fullsamples = dma.samples / dma.channels;

	// it is possible to miscount buffers if it has wrapped twice between
	// calls to S_Update.  Oh well.
	samplepos = SNDDMA_GetDMAPos();
	if (samplepos < oldsamplepos)
	{
		buffers++;					// buffer wrapped
		
		if (s_paintedtime > 0x40000000)
		{	// time to chop things off to avoid 32 bit limits
			buffers = 0;
			s_paintedtime = fullsamples;
			S_StopAllSounds ();
		}
	}
	oldsamplepos = samplepos;

	s_soundtime = buffers*fullsamples + samplepos/dma.channels;

#if 0
// check to make sure that we haven't overshot
	if (s_paintedtime < s_soundtime)
	{
		Com_DPrintf ("S_Update_ : overflow\n");
		s_paintedtime = s_soundtime;
	}
#endif

	if ( dma.submission_chunk < 256 ) {
		s_paintedtime = s_soundtime + s_mixPreStep->value * dma.speed;
	} else {
		s_paintedtime = s_soundtime + dma.submission_chunk;
	}
}


void S_Update_(void) {
	unsigned        endtime;
	int				samps;
	static			float	lastTime = 0.0f;
	float			ma, op;
	float			thisTime, sane;
	static			int ot = -1;

	if ( !s_soundStarted || s_soundMuted ) {
		return;
	}

	thisTime = Com_Milliseconds();

	// Updates s_soundtime
	S_GetSoundtime();

	if (s_soundtime == ot) {
		return;
	}
	ot = s_soundtime;

	// clear any sound effects that end before the current time,
	// and start any new sounds
	S_ScanChannelStarts();

	sane = thisTime - lastTime;
	if (sane<11) {
		sane = 11;			// 85hz
	}

	ma = s_mixahead->value * dma.speed;
	op = s_mixPreStep->value + sane*dma.speed*0.01;

	if (op < ma) {
		ma = op;
	}

	// mix ahead of current position
	endtime = s_soundtime + ma;

	// mix to an even submission block size
	endtime = (endtime + dma.submission_chunk-1)
		& ~(dma.submission_chunk-1);

	// never mix more than the complete buffer
	samps = dma.samples >> (dma.channels-1);
	if (endtime - s_soundtime > samps)
		endtime = s_soundtime + samps;



	SNDDMA_BeginPainting ();

	S_PaintChannels (endtime);

	SNDDMA_Submit ();

	lastTime = thisTime;
}

/*
===============================================================================

console functions

===============================================================================
*/

void S_Play_f( void ) {
	int 		i;
	sfxHandle_t	h;
	char		name[256];
	
	i = 1;
	while ( i<Cmd_Argc() ) {
		if ( !Q_strrchr(Cmd_Argv(i), '.') ) {
			Com_sprintf( name, sizeof(name), "%s.wav", Cmd_Argv(1) );
		} else {
			Q_strncpyz( name, Cmd_Argv(i), sizeof(name) );
		}
		h = S_RegisterSound( name );
		if( h ) {
			S_StartLocalSound( h, CHAN_LOCAL_SOUND );
		}
		i++;
	}
}

static void S_Music_f( void ) {
	int		c;

	c = Cmd_Argc();

	if ( c == 2 ) {
		S_StartBackgroundTrack( Cmd_Argv(1), Cmd_Argv(1), qfalse );
	} else if ( c == 3 ) {
		S_StartBackgroundTrack( Cmd_Argv(1), Cmd_Argv(2), qfalse );		
	} else {
		Com_Printf ("music <musicfile> [loopfile]\n");
		return;
	}
}


// this table needs to be in-sync with the typedef'd enum "SoundCompressionMethod_t"...	-ste
//
static const char *sSoundCompressionMethodStrings[ct_NUMBEROF] = 
{
	"16b",	// ct_16
	"mp3"	// ct_MP3
};
void S_SoundList_f( void ) {
	int		i;
	sfx_t	*sfx;
	int		size, total;

	total = 0;

	Com_Printf("\n");
	Com_Printf("                    InMemory?\n");
	Com_Printf("                    |\n");
	Com_Printf("                    |  LevelLastUsedOn\n");
	Com_Printf("                    |  |\n");
	Com_Printf("                    |  |\n");
	Com_Printf(" Slot   Bytes Type  |  |   Name\n");
//	Com_Printf(" Slot   Bytes Type  InMem?   Name\n");

	for (sfx=s_knownSfx, i=0 ; i<s_numSfx ; i++, sfx++) 
	{
		size = sfx->iSoundLengthInSamples;
		total += sfx->bInMemory ? size : 0;
		Com_Printf("%5d %7i [%s] %s %2d %s\n", i, size, sSoundCompressionMethodStrings[sfx->eSoundCompressionMethod], sfx->bInMemory?"y":"n", sfx->iLastLevelUsedOn, sfx->sSoundName );
	}
	Com_Printf ("Total resident samples: %i  ( not mem usage, see 'meminfo' ).\n", total);
	Com_Printf ("%d out of %d sfx_t slots used\n", s_numSfx, MAX_SFX);
	S_DisplayFreeMemory();
}


/*
===============================================================================

background music functions

===============================================================================
*/

int	FGetLittleLong( fileHandle_t f ) {
	int		v;

	FS_Read( &v, sizeof(v), f );

	return LittleLong( v);
}

int	FGetLittleShort( fileHandle_t f ) {
	short	v;

	FS_Read( &v, sizeof(v), f );

	return LittleShort( v);
}

// returns the length of the data in the chunk, or 0 if not found
int S_FindWavChunk( fileHandle_t f, char *chunk ) {
	char	name[5];
	int		len;
	int		r;

	name[4] = 0;
	len = 0;
	r = FS_Read( name, 4, f );
	if ( r != 4 ) {
		return 0;
	}
	len = FGetLittleLong( f );
	if ( len < 0 || len > 0xfffffff ) {
		len = 0;
		return 0;
	}
	len = (len + 1 ) & ~1;		// pad to word boundary
//	s_nextWavChunk += len + 8;

	if ( strcmp( name, chunk ) ) {
		return 0;
	}

	return len;
}

// fixme: need to move this into qcommon sometime?, but too much stuff altered by other people and I won't be able
//	to compile again for ages if I check that out...
//
// DO NOT replace this with a call to FS_FileExists, that's for checking about writing out, and doesn't work for this.
//
qboolean S_FileExists( const char *psFilename )
{
	fileHandle_t fhTemp;

	FS_FOpenFileRead (psFilename, &fhTemp, qtrue);	// qtrue so I can fclose the handle without closing a PAK
	if (!fhTemp) 
		return qfalse;
	
	FS_FCloseFile(fhTemp);
	return qtrue;
}

// some stuff for streaming MP3 files from disk (not pleasant, but nothing about MP3 is, other than compression ratios...)
//
static void MP3MusicStream_Reset(MusicInfo_t *pMusicInfo)
{
	pMusicInfo->iMP3MusicStream_DiskReadPos		= 0;
	pMusicInfo->iMP3MusicStream_DiskWindowPos	= 0;
}

//
// return is where the decoder should read from...
//
static byte *MP3MusicStream_ReadFromDisk(MusicInfo_t *pMusicInfo, int iReadOffset, int iReadBytesNeeded)
{
	if (iReadOffset < pMusicInfo->iMP3MusicStream_DiskWindowPos)
	{
		assert(0);											// should never happen
		return pMusicInfo->byMP3MusicStream_DiskBuffer;		// ...but return something safe anyway
	}

	while (iReadOffset + iReadBytesNeeded > pMusicInfo->iMP3MusicStream_DiskReadPos)
	{
		int iBytesRead = FS_Read( pMusicInfo->byMP3MusicStream_DiskBuffer + (pMusicInfo->iMP3MusicStream_DiskReadPos - pMusicInfo->iMP3MusicStream_DiskWindowPos), iMP3MusicStream_DiskBytesToRead, pMusicInfo->s_backgroundFile );

		pMusicInfo->iMP3MusicStream_DiskReadPos += iBytesRead;

		if (iBytesRead != iMP3MusicStream_DiskBytesToRead)	// quietly ignore any requests to read past file end
		{
			break;		// we need to do this because the disk read code can't know how much source data we need to
						//	read for a given number of requested output bytes, so we'll always be asking for too many
		}
	}

	// if reached halfway point in buffer (approx 20k), backscroll it...
	//
	if (pMusicInfo->iMP3MusicStream_DiskReadPos - pMusicInfo->iMP3MusicStream_DiskWindowPos > iMP3MusicStream_DiskBufferSize/2)
	{
		int iMoveSrcOffset = iReadOffset - pMusicInfo->iMP3MusicStream_DiskWindowPos;
		int iMoveCount     = (pMusicInfo->iMP3MusicStream_DiskReadPos - pMusicInfo->iMP3MusicStream_DiskWindowPos ) - iMoveSrcOffset;
		memmove( &pMusicInfo->byMP3MusicStream_DiskBuffer, &pMusicInfo->byMP3MusicStream_DiskBuffer[iMoveSrcOffset], iMoveCount);
		pMusicInfo->iMP3MusicStream_DiskWindowPos += iMoveSrcOffset;
	}

	return pMusicInfo->byMP3MusicStream_DiskBuffer + (iReadOffset - pMusicInfo->iMP3MusicStream_DiskWindowPos);
}


// does NOT set s_rawend!...
//
static void S_StopBackgroundTrack_Actual( MusicInfo_t *pMusicInfo ) 
{
	if ( pMusicInfo->s_backgroundFile ) 
	{
		if ( pMusicInfo->s_backgroundFile != -1)
		{
			Sys_EndStreamedFile( pMusicInfo->s_backgroundFile );
			FS_FCloseFile( pMusicInfo->s_backgroundFile );
		}
		pMusicInfo->s_backgroundFile = 0;	
	}
}

static void FreeMusic( MusicInfo_t *pMusicInfo )
{
	if (pMusicInfo->pLoadedData)
	{
		Z_Free(pMusicInfo->pLoadedData);
		pMusicInfo->pLoadedData		= NULL;
		pMusicInfo->iLoadedDataLen	= 0;
		pMusicInfo->sLoadedDataName[0]= '\0';
	}
}

// called only by snd_restart
//
void S_UnCacheDynamicMusic( void )
{
	FreeMusic( &tMusic_Info[eBGRNDTRACK_SLOW] );
}

static void S_StartBackgroundTrack_Actual( MusicInfo_t *pMusicInfo, const char *intro, const char *loop )
{
	int		len;
	char	dump[16];
	char	name[MAX_QPATH];

	Q_strncpyz( sMusic_BackgroundLoop, loop, sizeof( sMusic_BackgroundLoop ));	

	Q_strncpyz( name, intro, sizeof( name ) - 4 );	// this seems to be so that if the filename hasn't got an extension
													//	but doesn't have the room to append on either then you'll just
													//	get the "soft" fopen() error, rather than the ERR_DROP you'd get
													//	if COM_DefaultExtension didn't have room to add it on.
	COM_DefaultExtension( name, sizeof( name ), ".wav" );

	// close the background track, but DON'T reset s_rawend (or remaining music bits that haven't been output yet will be cut off)
	//
#if 0
/*	if ( pMusicInfo->s_backgroundFile ) {
		Sys_EndStreamedFile( pMusicInfo->s_backgroundFile );
		FS_FCloseFile( pMusicInfo->s_backgroundFile );
		pMusicInfo->s_backgroundFile = 0;
	}
*/
#else
	S_StopBackgroundTrack_Actual( pMusicInfo );
#endif

	pMusicInfo->bIsMP3 = qfalse;

	if ( !intro[0] ) {
		return;
	}


	// new bit, if file requested is not same any loaded one (if prev was in-mem), ditch it...
	//
	if (Q_stricmp(name, pMusicInfo->sLoadedDataName))
	{
		FreeMusic( pMusicInfo );
	}

	if (!Q_stricmpn(name+(strlen(name)-4),".mp3",4))
	{
		int iMP3Filelen = FS_FOpenFileRead( name, &pMusicInfo->s_backgroundFile, qtrue );
		if (!pMusicInfo->s_backgroundFile)
		{
			Com_Printf( S_COLOR_RED"Couldn't open music file %s\n", name );
			return;
		}

		MP3MusicStream_Reset( pMusicInfo );

		byte *pbMP3DataSegment	= NULL;
		int iInitialMP3ReadSize = 8192;		// fairly arbitrary, whatever size this is then the decoder is allowed to
											// scan up to halfway of it to find floating headers, so don't make it 
											// too small. 8k works fine.

		pbMP3DataSegment = MP3MusicStream_ReadFromDisk(pMusicInfo, 0, iInitialMP3ReadSize);

		if (MP3_IsValid(name, pbMP3DataSegment, iInitialMP3ReadSize, qtrue /*bStereoDesired*/))
		{
			// init stream struct...
			//
			memset(&pMusicInfo->streamMP3_Bgrnd,0,sizeof(pMusicInfo->streamMP3_Bgrnd));
			char *psError = C_MP3Stream_DecodeInit( &pMusicInfo->streamMP3_Bgrnd, pbMP3DataSegment, iMP3Filelen,
													dma.speed,
													16,		// sfx->width * 8,
													qtrue	// bStereoDesired
													);


			if (psError == NULL)
			{
				// init sfx struct & setup the few fields I actually need...
				//
				memset(	   &pMusicInfo->sfxMP3_Bgrnd,0,sizeof(pMusicInfo->sfxMP3_Bgrnd));
				//			pMusicInfo->sfxMP3_Bgrnd.width					= 2;			// read by MP3_GetSamples()
							pMusicInfo->sfxMP3_Bgrnd.iSoundLengthInSamples	= 0x7FFFFFFF;	// max possible +ve int, since music finishes when decoder stops
							pMusicInfo->sfxMP3_Bgrnd.pMP3StreamHeader		= &pMusicInfo->streamMP3_Bgrnd;
				Q_strncpyz( pMusicInfo->sfxMP3_Bgrnd.sSoundName, name, sizeof(pMusicInfo->sfxMP3_Bgrnd.sSoundName) );

				pMusicInfo->s_backgroundInfo.format		= WAV_FORMAT_MP3;	// not actually used this way, but just ensures we don't match one of the legit formats
				pMusicInfo->s_backgroundInfo.channels	= 2;		// always, for our MP3s when used for music (else 1 for FX)
				pMusicInfo->s_backgroundInfo.rate		= dma.speed;
				pMusicInfo->s_backgroundInfo.width		= 2;		// always, for our MP3s
				pMusicInfo->s_backgroundInfo.samples	= pMusicInfo->sfxMP3_Bgrnd.iSoundLengthInSamples;
				pMusicInfo->s_backgroundSamples			= pMusicInfo->sfxMP3_Bgrnd.iSoundLengthInSamples;

				memset(&pMusicInfo->chMP3_Bgrnd,0,sizeof(pMusicInfo->chMP3_Bgrnd));
						pMusicInfo->chMP3_Bgrnd.thesfx = &pMusicInfo->sfxMP3_Bgrnd;
				memcpy(&pMusicInfo->chMP3_Bgrnd.MP3StreamHeader, pMusicInfo->sfxMP3_Bgrnd.pMP3StreamHeader, sizeof(*pMusicInfo->sfxMP3_Bgrnd.pMP3StreamHeader));				

				pMusicInfo->bIsMP3 = qtrue;
			}
			else
			{
				Com_Printf(S_COLOR_RED"Error streaming file %s: %s\n", name, psError);
				FS_FCloseFile( pMusicInfo->s_backgroundFile );
				pMusicInfo->s_backgroundFile = 0;
			}
		}
		else
		{
			// MP3_IsValid() will already have printed any errors via Com_Printf at this point...
			//			
			FS_FCloseFile( pMusicInfo->s_backgroundFile );
			pMusicInfo->s_backgroundFile = 0;
		}
		
		return;
	}
	else	// not an mp3 file
	{
		//
		// open up a wav file and get all the info
		//
		FS_FOpenFileRead( name, &pMusicInfo->s_backgroundFile, qtrue );
		if ( !pMusicInfo->s_backgroundFile ) {
			Com_Printf( S_COLOR_YELLOW "WARNING: couldn't open music file %s\n", name );
			return;
		}

		// skip the riff wav header

		FS_Read(dump, 12, pMusicInfo->s_backgroundFile);

		if ( !S_FindWavChunk( pMusicInfo->s_backgroundFile, "fmt " ) ) {
			Com_Printf( S_COLOR_YELLOW "WARNING: No fmt chunk in %s\n", name );
			FS_FCloseFile( pMusicInfo->s_backgroundFile );
			pMusicInfo->s_backgroundFile = 0;
			return;
		}

		// save name for soundinfo
		pMusicInfo->s_backgroundInfo.format = FGetLittleShort( pMusicInfo->s_backgroundFile );
		pMusicInfo->s_backgroundInfo.channels = FGetLittleShort( pMusicInfo->s_backgroundFile );
		pMusicInfo->s_backgroundInfo.rate = FGetLittleLong( pMusicInfo->s_backgroundFile );
		FGetLittleLong(  pMusicInfo->s_backgroundFile );
		FGetLittleShort(  pMusicInfo->s_backgroundFile );
		pMusicInfo->s_backgroundInfo.width = FGetLittleShort( pMusicInfo->s_backgroundFile ) / 8;

		if ( pMusicInfo->s_backgroundInfo.format != WAV_FORMAT_PCM ) {
			FS_FCloseFile( pMusicInfo->s_backgroundFile );
			pMusicInfo->s_backgroundFile = 0;
			Com_Printf(S_COLOR_YELLOW "WARNING: Not a microsoft PCM format wav: %s\n", name);
			return;
		}

		if ( pMusicInfo->s_backgroundInfo.channels != 2 || pMusicInfo->s_backgroundInfo.rate != 22050 ) {
			Com_Printf(S_COLOR_YELLOW "WARNING: music file %s is not 22k stereo\n", name );
		}

		if ( ( len = S_FindWavChunk( pMusicInfo->s_backgroundFile, "data" ) ) == 0 ) {
			FS_FCloseFile( pMusicInfo->s_backgroundFile );
			pMusicInfo->s_backgroundFile = 0;
			Com_Printf(S_COLOR_YELLOW "WARNING: No data chunk in %s\n", name);
			return;
		}

		pMusicInfo->s_backgroundInfo.samples = len / (pMusicInfo->s_backgroundInfo.width * pMusicInfo->s_backgroundInfo.channels);

		pMusicInfo->s_backgroundSamples = pMusicInfo->s_backgroundInfo.samples;

		//
		// start the background streaming
		//
		Sys_BeginStreamedFile( pMusicInfo->s_backgroundFile, 0x10000 );
	}
}



static char gsIntroMusic[MAX_QPATH]={0};
static char gsLoopMusic [MAX_QPATH]={0};

void S_RestartMusic( void ) 
{
	if (s_soundStarted && !s_soundMuted )
	{
		if (gsIntroMusic[0] || gsLoopMusic[0])
		{				
			S_StartBackgroundTrack( gsIntroMusic, gsLoopMusic, qfalse );	// ( default music start will set the state to SLOW )			
		}
	}
}

// Basic logic here is to see if the intro file specified actually exists, and if so, then it's not dynamic music,
//	but if it doesn't, and it DOES exist by strcat()ing "_fast" and "_slow" (.mp3) onto 2 copies of it, then it's dynamic.
//
// In either case, open it if it exits, and just set some vars that'll inhibit dynamicness if needed.
//
void S_StartBackgroundTrack( const char *intro, const char *loop, qboolean bReturnWithoutStarting )
{
	if ( !intro ) {
		intro = "";
	}
	if ( !loop || !loop[0] ) {
		loop = intro;
	}

	Q_strncpyz(gsIntroMusic,intro, sizeof(gsIntroMusic));
	Q_strncpyz(gsLoopMusic, loop,  sizeof(gsLoopMusic));

	// do NOT start music from cgame now, the functions Com_TouchMemory() and RegisterMedia_LevelLoadEnd()
	//	can take some time to execute (after music start), and can stutter it. So now, RegisterMedia_LevelLoadEnd()
	//	will call S_RestartMusic now that the strings are set...
	//
	if ( bReturnWithoutStarting )
		return;	

	char sName[MAX_QPATH];
	Q_strncpyz(sName,intro,sizeof(sName));
	COM_DefaultExtension( sName, sizeof( sName ), ".mp3" );

	// conceptually we always play the 'intro'[/sName] track, intro-to-loop transition is handled in UpdateBackGroundTrack().
	//
	if (S_FileExists( sName ))
	{		
		Com_DPrintf("S_StartBackgroundTrack: Found/using music track '%s'\n", sName);
		S_StartBackgroundTrack_Actual( &tMusic_Info[eBGRNDTRACK_SLOW], sName, loop );
	}
	else
	{
		Com_Printf( S_COLOR_RED "ERROR: Unable to find music file:\n( %s )\n", sName );
		S_StopBackgroundTrack();
	}	
}

void S_StopBackgroundTrack( void )
{
	S_StopBackgroundTrack_Actual( &tMusic_Info[eBGRNDTRACK_SLOW] );

	s_rawend = 0;
}



// qboolean return is true only if we're changing from a streamed intro to a dynamic loop...
//
static qboolean S_UpdateBackgroundTrack_Actual( MusicInfo_t *pMusicInfo ) 
{
	int		bufferSamples;
	int		fileSamples;
	byte	raw[30000];		// just enough to fit in a mac stack frame  (note that MP3 doesn't use full size of it)
	int		fileBytes;
	int		r;

	float fMasterVol = (s_musicVolume->value*s_musicMult->value);

	static	float	musicVolume = 0.25f;

// this is to work around an obscure issue to do with sliding decoder windows and amounts being requested, since the
//	original MP3 stream-decoder wrapper was designed to work with audio-paintbuffer sized pieces... Basically 30000
//	is far too big for the window decoder to handle in one request because of the time-travel issue associated with
//	normal sfx buffer painting, and allowing sufficient sliding room, even though the music file never goes back in time.
//
#define SIZEOF_RAW_BUFFER_FOR_MP3 4096
#define RAWSIZE (pMusicInfo->bIsMP3?SIZEOF_RAW_BUFFER_FOR_MP3:sizeof(raw))

	if ( !pMusicInfo->s_backgroundFile ) {
		return qfalse;
	}	

	musicVolume = (musicVolume + fMasterVol)/2.0f;

	// don't bother playing anything if musicvolume is 0
	if ( musicVolume <= 0 ) {
		return qfalse;
	}

	// see how many samples should be copied into the raw buffer
	if ( s_rawend < s_soundtime ) {
		s_rawend = s_soundtime;
	}

	while ( s_rawend < s_soundtime + MAX_RAW_SAMPLES ) 
	{
		bufferSamples = MAX_RAW_SAMPLES - (s_rawend - s_soundtime);

		// decide how much data needs to be read from the file
		fileSamples = bufferSamples * pMusicInfo->s_backgroundInfo.rate / dma.speed;

		// don't try and read past the end of the file
		if ( fileSamples > pMusicInfo->s_backgroundSamples ) {
			fileSamples = pMusicInfo->s_backgroundSamples;
		}

		// our max buffer size
		fileBytes = fileSamples * (pMusicInfo->s_backgroundInfo.width * pMusicInfo->s_backgroundInfo.channels);
		if (fileBytes > RAWSIZE ) {
			fileBytes = RAWSIZE;
			fileSamples = fileBytes / (pMusicInfo->s_backgroundInfo.width * pMusicInfo->s_backgroundInfo.channels);
		}

		qboolean qbForceFinish = qfalse;
		if (pMusicInfo->bIsMP3)
		{
			int iStartingSampleNum = pMusicInfo->chMP3_Bgrnd.thesfx->iSoundLengthInSamples - pMusicInfo->s_backgroundSamples;	// but this IS relevant
			// Com_Printf(S_COLOR_YELLOW "Requesting MP3 samples: sample %d\n",iStartingSampleNum);


			if (pMusicInfo->s_backgroundFile == -1)
			{
				// in-mem...
				//
				qbForceFinish = (MP3Stream_GetSamples( &pMusicInfo->chMP3_Bgrnd, iStartingSampleNum, fileBytes/2, (short*) raw, qtrue ))?qfalse:qtrue;				

				//Com_Printf(S_COLOR_YELLOW "Music time remaining: %f seconds\n", MP3Stream_GetRemainingTimeInSeconds( &pMusicInfo->chMP3_Bgrnd.MP3StreamHeader ));				
			}
			else
			{
				// streaming an MP3 file instead... (note that the 'fileBytes' request size isn't that relevant for MP3s, 
				//										since code here can't know how much the MP3 needs to decompress)
				//
				byte *pbScrolledStreamData = MP3MusicStream_ReadFromDisk(pMusicInfo, pMusicInfo->chMP3_Bgrnd.MP3StreamHeader.iSourceReadIndex, fileBytes);

				pMusicInfo->chMP3_Bgrnd.MP3StreamHeader.pbSourceData = pbScrolledStreamData - pMusicInfo->chMP3_Bgrnd.MP3StreamHeader.iSourceReadIndex;

				qbForceFinish = (MP3Stream_GetSamples( &pMusicInfo->chMP3_Bgrnd, iStartingSampleNum, fileBytes/2, (short*) raw, qtrue ))?qfalse:qtrue;
			}
		}
		else
		{
			// streaming a WAV off disk...
			//
			r = Sys_StreamedRead( raw, 1, fileBytes, pMusicInfo->s_backgroundFile );
			if ( r != fileBytes ) {
				Com_Printf(S_COLOR_RED"StreamedRead failure on music track\n");
				S_StopBackgroundTrack();
				return qfalse;
			}

			// byte swap if needed (do NOT do for MP3 decoder, that has an internal big/little endian handler)
			//
			S_ByteSwapRawSamples( fileSamples, pMusicInfo->s_backgroundInfo.width, pMusicInfo->s_backgroundInfo.channels, raw );
		}

		// add to raw buffer
		S_RawSamples(	fileSamples, pMusicInfo->s_backgroundInfo.rate, 
						pMusicInfo->s_backgroundInfo.width, pMusicInfo->s_backgroundInfo.channels, raw, musicVolume						
					);

		pMusicInfo->s_backgroundSamples -= fileSamples;
		if ( !pMusicInfo->s_backgroundSamples || qbForceFinish ) 
		{
			// loop the music, or play the next piece if we were on the intro...
			//
			//	(but not for dynamic, that can only be used for loop music)
			//
			// for non-dynamic music we need to check if "sMusic_BackgroundLoop" is an actual filename,
			//	or if it's a dynamic music specifier (which can't literally exist), in which case it should set
			//	a return flag then exit...
			//
			char sTestName[MAX_QPATH*2];// *2 so COM_DefaultExtension doesn't do an ERR_DROP if there was no space
										//	for an extension, since this is a "soft" test				
			Q_strncpyz( sTestName, sMusic_BackgroundLoop, sizeof(sTestName));
			COM_DefaultExtension(sTestName, sizeof(sTestName), ".wav");

			if (S_FileExists( sTestName ))
			{
				S_StartBackgroundTrack_Actual( pMusicInfo, sMusic_BackgroundLoop, sMusic_BackgroundLoop );
			}
			else
			{
				// proposed file doesn't exist, but this may be a dynamic track we're wanting to loop, 
				//	so exit with a special flag...
				//
				return qtrue;
			}

			if ( !pMusicInfo->s_backgroundFile ) 
			{
				return qfalse;		// loop failed to restart
			}
		}
	}

#undef SIZEOF_RAW_BUFFER_FOR_MP3
#undef RAWSIZE

	return qfalse;
}


static void S_UpdateBackgroundTrack( void )
{
	qboolean bNewTrackDesired = S_UpdateBackgroundTrack_Actual(&tMusic_Info[eBGRNDTRACK_SLOW]);

	if (bNewTrackDesired)
	{
		S_StartBackgroundTrack( sMusic_BackgroundLoop, sMusic_BackgroundLoop, qfalse );
	}
}


// currently passing in sfx as a param in case I want to do something with it later.
//
byte *SND_malloc(int iSize, sfx_t *sfx) 
{
	byte *pData = (byte *) Z_Malloc(iSize, TAG_SND_RAWDATA);	// don't bother asking for zeroed mem
	return pData;
}

cvar_t *s_soundpoolmegs = NULL;

// called once-only in EXE lifetime...
//
void SND_setup() 
{		 
	s_soundpoolmegs = Cvar_Get("s_soundpoolmegs", "25", CVAR_ARCHIVE);
	if (Sys_LowPhysicalMemory() )
	{
		Cvar_Set("s_soundpoolmegs", "0");
	}

	Com_Printf("Sound memory manager started\n");
}

// ask how much mem an sfx has allocated...
//
static int SND_MemUsed(sfx_t *sfx)
{
	int iSize = 0;
	if (sfx->pSoundData){
		iSize += Z_Size(sfx->pSoundData);
	}

	if (sfx->pMP3StreamHeader) {
		iSize += Z_Size(sfx->pMP3StreamHeader);
	}

	return iSize;
}

// free any allocated sfx mem...
//
// now returns # bytes freed to help with z_malloc()-fail recovery
//
static int SND_FreeSFXMem(sfx_t *sfx)
{
	int iBytesFreed = 0;

	if (						sfx->pSoundData) {
		iBytesFreed +=	Z_Size(	sfx->pSoundData);
						Z_Free(	sfx->pSoundData );
								sfx->pSoundData = NULL;
	}

	sfx->bInMemory = qfalse;	

	if (						sfx->pMP3StreamHeader) {
		iBytesFreed +=	Z_Size(	sfx->pMP3StreamHeader);
						Z_Free(	sfx->pMP3StreamHeader );
								sfx->pMP3StreamHeader = NULL;
	}

	return iBytesFreed;
}



void S_DisplayFreeMemory() 
{
//	Com_Printf("%.2fMB total sound bytes, %.2fMB used this map\n",
//				(float)iSNDBytes_Total		/1024.0f/1024.0f, 
//				(float)iSNDBytes_ThisMap	/1024.0f/1024.0f
//				);
	int iSoundDataSize = Z_MemSize ( TAG_SND_RAWDATA ) + Z_MemSize( TAG_SND_MP3STREAMHDR );
	int iMusicDataSize = Z_MemSize ( TAG_SND_DYNAMICMUSIC );

	if (iSoundDataSize || iMusicDataSize)
	{
		Com_Printf("\n%.2fMB audio data:  ( %.2fMB WAV/MP3 ) + ( %.2fMB Music )\n",
					((float)(iSoundDataSize+iMusicDataSize))/1024.0f/1024.0f,
										((float)(iSoundDataSize))/1024.0f/1024.0f,
																((float)(iMusicDataSize))/1024.0f/1024.0f
					);

		// now count up amount used on this level...
		//
		iSoundDataSize = 0;
		for (int i=1; i<s_numSfx; i++)
		{
			sfx_t *sfx = &s_knownSfx[i];

			if (sfx->iLastLevelUsedOn == RE_RegisterMedia_GetLevel()){
				iSoundDataSize += SND_MemUsed(sfx);
			}
		}

		Com_Printf("%.2fMB in sfx_t alloc data (WAV/MP3) loaded this level\n",(float)iSoundDataSize/1024.0f/1024.0f);
	}
}

void SND_TouchSFX(sfx_t *sfx)
{
	sfx->iLastTimeUsed		= Com_Milliseconds();	// ditch this field sometime?
	sfx->iLastLevelUsedOn	= RE_RegisterMedia_GetLevel();
}


// currently this is only called during snd_shutdown or snd_restart
//
void S_FreeAllSFXMem(void)
{
	for (int i=1 ; i < s_numSfx ; i++)	// start @ 1 to skip freeing default sound
	{
		SND_FreeSFXMem(&s_knownSfx[i]);
	}
}

// returns number of bytes freed up...
//
int SND_FreeOldestSound() 
{	
	int iBytesFreed = 0;
	sfx_t *sfx;

	int	iOldest = Com_Milliseconds();
	int	iUsed	= 0;

	// start on 1 so we never dump the default sound...
	//
	for (int i=1 ; i < s_numSfx ; i++) 
	{
		sfx = &s_knownSfx[i];

		if (!sfx->bDefaultSound && sfx->bInMemory && sfx->iLastTimeUsed < iOldest)
		{
			if (sfx->pSoundData)
			{
				iUsed = i;
				iOldest = sfx->iLastTimeUsed;
			}
			else
			{
				assert(0);	// shouldn't need this, but I'm sure I saw one once then had bInMemory but !pSoundData.... ?!?!?
				sfx->bInMemory = qfalse;
			}
		}
	}

	if (iUsed)
	{
		sfx = &s_knownSfx[ iUsed ];
	
		Com_DPrintf("SND_FreeOldestSound: freeing sound %s\n", sfx->sSoundName);
	
		iBytesFreed = SND_FreeSFXMem(sfx);
	}

	return iBytesFreed;
}


// just before we drop into a level, ensure the audio pool is under whatever the maximum
//	pool size is (but not by dropping out sounds used by the current level)...
//
// returns qtrue if at least one sound was dropped out, so z_malloc-fail recovery code knows if anything changed
//
extern qboolean gbInsideLoadSound;
qboolean SND_RegisterAudio_LevelLoadEnd(qboolean bDeleteEverythingNotUsedThisLevel /* 99% qfalse */)
{
	qboolean bAtLeastOneSoundDropped = qfalse;

	Com_DPrintf( "SND_RegisterAudio_LevelLoadEnd():\n");

	if (gbInsideLoadSound)
	{
		Com_DPrintf( "(Inside S_LoadSound (z_malloc recovery?), exiting...\n");
	}
	else
	{
		int iLoadedAudioBytes	 = Z_MemSize ( TAG_SND_RAWDATA ) + Z_MemSize( TAG_SND_MP3STREAMHDR );
		const int iMaxAudioBytes = s_soundpoolmegs->integer * 1024 * 1024;

		for (int i=1; i<s_numSfx && ( iLoadedAudioBytes > iMaxAudioBytes || bDeleteEverythingNotUsedThisLevel) ; i++) // i=1 so we never page out default sound
		{
			sfx_t *sfx = &s_knownSfx[i];

			if (sfx->bInMemory)
			{
				qboolean bDeleteThis = qfalse;

				if (bDeleteEverythingNotUsedThisLevel)
				{
					bDeleteThis = (sfx->iLastLevelUsedOn != RE_RegisterMedia_GetLevel() ) ? qtrue : qfalse;
				}
				else
				{
					bDeleteThis = (sfx->iLastLevelUsedOn < RE_RegisterMedia_GetLevel() ) ? qtrue : qfalse;
				}

				if (bDeleteThis)
				{
					Com_DPrintf( "Dumping sfx_t \"%s\"\n",sfx->sSoundName);			

					if (SND_FreeSFXMem(sfx))
					{
						bAtLeastOneSoundDropped = qtrue;
					}

					iLoadedAudioBytes = Z_MemSize ( TAG_SND_RAWDATA ) + Z_MemSize( TAG_SND_MP3STREAMHDR );					
				}
			}
		}
	}

	Com_DPrintf( "SND_RegisterAudio_LevelLoadEnd(): Ok\n");	

	return bAtLeastOneSoundDropped;
}

