// snd_local.h -- private sound definations

#ifndef SND_LOCAL_H
#define SND_LOCAL_H

#include "../game/q_shared.h"
#include "../qcommon/qcommon.h"
#include "snd_public.h"
#include "../mp3code/mp3struct.h"

#define	PAINTBUFFER_SIZE		4096					// this is in samples

#define SND_CHUNK_SIZE			1024					// samples
#define SND_CHUNK_SIZE_FLOAT	(SND_CHUNK_SIZE/2)		// floats
#define SND_CHUNK_SIZE_BYTE		(SND_CHUNK_SIZE*2)		// bytes
#define	SND_CACHE_SIZE			6144					// this is in chunks


#define DEFAULT_SOUND_NAME		"*defaultsound"

typedef struct {
	int			left;	// the final values will be clamped to +/- 0x00ffff00 and shifted down
	int			right;
} portable_samplepair_t;


typedef	struct sndBuffer_s {
	short					sndChunk[SND_CHUNK_SIZE];
	struct sndBuffer_s		*next;
//    int						size;
} sndBuffer;

// keep this table in-sync with the table "sSoundCompressionMethodStrings" (snd_dma.cpp)	-ste
//
typedef enum
{
	ct_16 = 0,		// 16-bit uncompressed samples (the default)
	ct_MP3,			
	//
	ct_NUMBEROF		// used only for array sizing

} SoundCompressionMethod_t;


typedef struct sfx_s {
	short			*pSoundData;
	qboolean		bDefaultSound;			// couldn't be loaded, so use buzz
	qboolean		bInMemory;				// not in Memory, set qtrue when loaded, and qfalse when its buffers are freed up because of being old, so can be reloaded
	SoundCompressionMethod_t eSoundCompressionMethod;	
	MP3STREAM		*pMP3StreamHeader;		// NULL ptr unless this sfx_t is an MP3. Use Z_Malloc and Z_Free
	int 			iSoundLengthInSamples;	// length in samples, always kept as 16bit now so this is #shorts (watch for stereo later for music?)
	char 			sSoundName[MAX_QPATH];
	int				iLastTimeUsed;
	float			fVolRange;				// used to set the highest volume this sample has at load time - used for lipsynching
	int				iLastLevelUsedOn;		// used for cacheing purposes
	struct sfx_s	*next;					// only used because of hash table when registering
} sfx_t;


typedef struct {
	int			channels;
	int			samples;				// mono samples in buffer
	int			submission_chunk;		// don't mix less than this #
	int			samplebits;
	int			speed;
	byte		*buffer;
} dma_t;

#define START_SAMPLE_IMMEDIATE	0x7fffffff

typedef struct loopSound_s {
	vec3_t		origin;
	vec3_t		velocity;
	sfx_t		*sfx;
	int			mergeFrame;
	qboolean	active;
	qboolean	kill;
	qboolean	doppler;
	float		dopplerScale;
	float		oldDopplerScale;
	int			framenum;

} loopSound_t;

typedef struct
{
	int			allocTime;
	int			startSample;	// START_SAMPLE_IMMEDIATE = set immediately on next mix
	int			entnum;			// to allow overriding a specific sound
	int			entchannel;		// to allow overriding a specific sound
	int			leftvol;		// 0-255 volume after spatialization
	int			rightvol;		// 0-255 volume after spatialization
	int			master_vol;		// 0-255 volume before spatialization
	float		dopplerScale;
	float		oldDopplerScale;
	vec3_t		origin;			// only use if fixed_origin is set
	qboolean	fixed_origin;	// use origin instead of fetching entnum's origin
	sfx_t		*thesfx;		// sfx structure
	qboolean	doppler;
	//
	// new stuff for MP3 decoding...
	//
	MP3STREAM	MP3StreamHeader;
	byte		MP3SlidingDecodeBuffer[50000/*12000*/];	// typical back-request = -3072, so roughly double is 6000 (safety), then doubled again so the 6K pos is in the middle of the buffer)
	int			iMP3SlidingDecodeWritePos;
	int			iMP3SlidingDecodeWindowPos;
} channel_t;


#define	WAV_FORMAT_PCM		1
#define WAV_FORMAT_ADPCM	2	// not actually implemented, but is the value that you get in a WAV header
#define WAV_FORMAT_MP3		3	// not actually used this way, but just ensures we don't match one of the legit formats



typedef struct {
	int			format;
	int			rate;
	int			width;
	int			channels;
	int			samples;
	int			dataofs;		// chunk starts this many bytes from file start
} wavinfo_t;


/*
====================================================================

  SYSTEM SPECIFIC FUNCTIONS

====================================================================
*/

// initializes cycling through a DMA buffer and returns information on it
qboolean SNDDMA_Init(void);

// gets the current DMA position
int		SNDDMA_GetDMAPos(void);

// shutdown the DMA xfer.
void	SNDDMA_Shutdown(void);

void	SNDDMA_BeginPainting (void);

void	SNDDMA_Submit(void);

//====================================================================

#define	MAX_CHANNELS			96

extern	channel_t   s_channels[MAX_CHANNELS];
extern	channel_t   loop_channels[MAX_CHANNELS];
extern	int		numLoopChannels;

extern	int		s_paintedtime;
extern	int		s_rawend;
extern	vec3_t	listener_forward;
extern	vec3_t	listener_right;
extern	vec3_t	listener_up;
extern	dma_t	dma;

#define	MAX_RAW_SAMPLES	16384
extern	portable_samplepair_t	s_rawsamples[MAX_RAW_SAMPLES];

extern cvar_t	*s_volume;
extern cvar_t	*s_nosound;
extern cvar_t	*s_khz;
extern cvar_t	*s_show;
extern cvar_t	*s_mixahead;

extern cvar_t	*s_testsound;
extern cvar_t	*s_separation;

qboolean S_LoadSound( sfx_t *sfx );

void S_PaintChannels(int endtime);

portable_samplepair_t *S_GetRawSamplePointer();

// spatializes a channel
void S_Spatialize(channel_t *ch);

void S_FreeAllSFXMem(void);


#define	NXStream byte


//////////////// sound code changes...
//
byte	*SND_malloc(int iSize, sfx_t *sfx);
void	 SND_setup();
int		 SND_FreeOldestSound();
void	 SND_TouchSFX(sfx_t *sfx);
void	S_DisplayFreeMemory(void);
void	S_memoryLoad(sfx_t *sfx);
//
////////////////


extern short *sfxScratchBuffer;
extern sfx_t *sfxScratchPointer;
extern int	   sfxScratchIndex;

#endif	// #ifndef SND_LOCAL_H
