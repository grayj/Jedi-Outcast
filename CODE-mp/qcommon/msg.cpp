#include "../game/q_shared.h"
#include "qcommon.h"
#ifdef _DONETPROFILE_
#include "INetProfile.h"
#endif

//#define _NEWHUFFTABLE_		// Build "c:\\netchan.bin"
//#define _USINGNEWHUFFTABLE_		// Build a new frequency table to cut and paste.

static huffman_t		msgHuff;

static qboolean			msgInit = qfalse;
static FILE				*fp=0;

int pcount[256];

/*
==============================================================================

			MESSAGE IO FUNCTIONS

Handles byte ordering and avoids alignment errors
==============================================================================
*/

#ifndef FINAL_BUILD
	int gLastBitIndex = 0;
#endif

int oldsize = 0;

void MSG_initHuffman();

void MSG_Init( msg_t *buf, byte *data, int length ) {
	if (!msgInit) {
		MSG_initHuffman();
	}
	Com_Memset (buf, 0, sizeof(*buf));
	buf->data = data;
	buf->maxsize = length;
}

void MSG_InitOOB( msg_t *buf, byte *data, int length ) {
	if (!msgInit) {
		MSG_initHuffman();
	}
	Com_Memset (buf, 0, sizeof(*buf));
	buf->data = data;
	buf->maxsize = length;
	buf->oob = qtrue;
}

void MSG_Clear( msg_t *buf ) {
	buf->cursize = 0;
	buf->overflowed = qfalse;
	buf->bit = 0;					//<- in bits
}


void MSG_Bitstream( msg_t *buf ) {
	buf->oob = qfalse;
}

void MSG_BeginReading( msg_t *msg ) {
	msg->readcount = 0;
	msg->bit = 0;
	msg->oob = qfalse;
}

void MSG_BeginReadingOOB( msg_t *msg ) {
	msg->readcount = 0;
	msg->bit = 0;
	msg->oob = qtrue;
}


/*
=============================================================================

bit functions
  
=============================================================================
*/

int	overflows;

// negative bit values include signs
void MSG_WriteBits( msg_t *msg, int value, int bits ) {
	int	i;

	oldsize += bits;

	// this isn't an exact overflow check, but close enough
	if ( msg->maxsize - msg->cursize < 4 ) {
		msg->overflowed = qtrue;
		return;
	}

	if ( bits == 0 || bits < -31 || bits > 32 ) {
		Com_Error( ERR_DROP, "MSG_WriteBits: bad bits %i", bits );
	}

	// check for overflows
	if ( bits != 32 ) {
		if ( bits > 0 ) {
			if ( value > ( ( 1 << bits ) - 1 ) || value < 0 ) {
				overflows++;
#ifndef FINAL_BUILD
//				Com_Printf ("MSG_WriteBits: overflow writing %d in %d bits [index %i]\n", value, bits, gLastBitIndex);
#endif
			}
		} else {
			int	r;

			r = 1 << (bits-1);

			if ( value >  r - 1 || value < -r ) {
				overflows++;
#ifndef FINAL_BUILD
//				Com_Printf ("MSG_WriteBits: overflow writing %d in %d bits [index %i]\n", value, bits, gLastBitIndex);
#endif
			}
		}
	}
	if ( bits < 0 ) {
		bits = -bits;
	}
	if (msg->oob) {
		if (bits==8) {
			msg->data[msg->cursize] = value;
			msg->cursize += 1;
			msg->bit += 8;
		} else if (bits==16) {
			unsigned short *sp = (unsigned short *)&msg->data[msg->cursize];
			*sp = LittleShort(value);
			msg->cursize += 2;
			msg->bit += 16;
		} else if (bits==32) {
			unsigned int *ip = (unsigned int *)&msg->data[msg->cursize];
			*ip = LittleLong(value);
			msg->cursize += 4;
			msg->bit += 8;
		} else {
			Com_Error(ERR_DROP, "can't read %d bits\n", bits);
		}
	} else {
		value &= (0xffffffff>>(32-bits));
		if (bits&7) {
			int nbits;
			nbits = bits&7;
			for(i=0;i<nbits;i++) {
				Huff_putBit((value&1), msg->data, &msg->bit);
				value = (value>>1);
			}
			bits = bits - nbits;
		}
		if (bits) {
			for(i=0;i<bits;i+=8) {
#ifdef _NEWHUFFTABLE_
				fwrite(&value, 1, 1, fp);
#endif // _NEWHUFFTABLE_
				Huff_offsetTransmit (&msgHuff.compressor, (value&0xff), msg->data, &msg->bit);
				value = (value>>8);
			}
		}
		msg->cursize = (msg->bit>>3)+1;
	}
}

int MSG_ReadBits( msg_t *msg, int bits ) {
	int			value;
	int			get;
	qboolean	sgn;
	int			i, nbits;
	value = 0;

	if ( bits < 0 ) {
		bits = -bits;
		sgn = qtrue;
	} else {
		sgn = qfalse;
	}

	if (msg->oob) {
		if (bits==8) {
			value = msg->data[msg->readcount];
			msg->readcount += 1;
			msg->bit += 8;
		} else if (bits==16) {
			unsigned short *sp = (unsigned short *)&msg->data[msg->readcount];
			value = LittleShort(*sp);
			msg->readcount += 2;
			msg->bit += 16;
		} else if (bits==32) {
			unsigned int *ip = (unsigned int *)&msg->data[msg->readcount];
			value = LittleLong(*ip);
			msg->readcount += 4;
			msg->bit += 32;
		} else {
			Com_Error(ERR_DROP, "can't read %d bits\n", bits);
		}
	} else {
		nbits = 0;
		if (bits&7) {
			nbits = bits&7;
			for(i=0;i<nbits;i++) {
				value |= (Huff_getBit(msg->data, &msg->bit)<<i);
			}
			bits = bits - nbits;
		}
		if (bits) {
			for(i=0;i<bits;i+=8) {
				Huff_offsetReceive (msgHuff.decompressor.tree, &get, msg->data, &msg->bit);
#ifdef _NEWHUFFTABLE_
				fwrite(&get, 1, 1, fp);
#endif // _NEWHUFFTABLE_
				value |= (get<<(i+nbits));
			}
		}
		msg->readcount = (msg->bit>>3)+1;
	}
	if ( sgn ) {
		if ( value & ( 1 << ( bits - 1 ) ) ) {
			value |= -1 ^ ( ( 1 << bits ) - 1 );
		}
	}

	return value;
}



//================================================================================

//
// writing functions
//

void MSG_WriteChar( msg_t *sb, int c ) {
#ifdef PARANOID
	if (c < -128 || c > 127)
		Com_Error (ERR_FATAL, "MSG_WriteChar: range error");
#endif

	MSG_WriteBits( sb, c, 8 );
}

void MSG_WriteByte( msg_t *sb, int c ) {
#ifdef PARANOID
	if (c < 0 || c > 255)
		Com_Error (ERR_FATAL, "MSG_WriteByte: range error");
#endif

	MSG_WriteBits( sb, c, 8 );
}

void MSG_WriteData( msg_t *buf, const void *data, int length ) {
	int i;
	for(i=0;i<length;i++) {
		MSG_WriteByte(buf, ((byte *)data)[i]);
	}
}

void MSG_WriteShort( msg_t *sb, int c ) {
#ifdef PARANOID
	if (c < ((short)0x8000) || c > (short)0x7fff)
		Com_Error (ERR_FATAL, "MSG_WriteShort: range error");
#endif

	MSG_WriteBits( sb, c, 16 );
}

void MSG_WriteLong( msg_t *sb, int c ) {
	MSG_WriteBits( sb, c, 32 );
}

void MSG_WriteFloat( msg_t *sb, float f ) {
	union {
		float	f;
		int	l;
	} dat;
	
	dat.f = f;
	MSG_WriteBits( sb, dat.l, 32 );
}

void MSG_WriteString( msg_t *sb, const char *s ) {
	if ( !s ) {
		MSG_WriteData (sb, "", 1);
	} else {
		int		l,i;
		char	string[MAX_STRING_CHARS];

		l = strlen( s );
		if ( l >= MAX_STRING_CHARS ) {
			Com_Printf( "MSG_WriteString: MAX_STRING_CHARS" );
			MSG_WriteData (sb, "", 1);
			return;
		}
		Q_strncpyz( string, s, sizeof( string ) );

		// get rid of 0xff chars, because old clients don't like them
		for ( i = 0 ; i < l ; i++ ) {
			if ( ((byte *)string)[i] > 127 ) {
				string[i] = '.';
			}
		}

		MSG_WriteData (sb, string, l+1);
	}
}

void MSG_WriteBigString( msg_t *sb, const char *s ) {
	if ( !s ) {
		MSG_WriteData (sb, "", 1);
	} else {
		int		l,i;
		char	string[BIG_INFO_STRING];

		l = strlen( s );
		if ( l >= BIG_INFO_STRING ) {
			Com_Printf( "MSG_WriteString: BIG_INFO_STRING" );
			MSG_WriteData (sb, "", 1);
			return;
		}
		Q_strncpyz( string, s, sizeof( string ) );

		// get rid of 0xff chars, because old clients don't like them
		for ( i = 0 ; i < l ; i++ ) {
			if ( ((byte *)string)[i] > 127 ) {
				string[i] = '.';
			}
		}

		MSG_WriteData (sb, string, l+1);
	}
}

void MSG_WriteAngle( msg_t *sb, float f ) {
	MSG_WriteByte (sb, (int)(f*256/360) & 255);
}

void MSG_WriteAngle16( msg_t *sb, float f ) {
	MSG_WriteShort (sb, ANGLE2SHORT(f));
}


//============================================================

//
// reading functions
//

// returns -1 if no more characters are available
int MSG_ReadChar (msg_t *msg ) {
	int	c;
	
	c = (signed char)MSG_ReadBits( msg, 8 );
	if ( msg->readcount > msg->cursize ) {
		c = -1;
	}	
	
	return c;
}

int MSG_ReadByte( msg_t *msg ) {
	int	c;
	
	c = (unsigned char)MSG_ReadBits( msg, 8 );
	if ( msg->readcount > msg->cursize ) {
		c = -1;
	}	
	return c;
}

int MSG_ReadShort( msg_t *msg ) {
	int	c;
	
	c = (short)MSG_ReadBits( msg, 16 );
	if ( msg->readcount > msg->cursize ) {
		c = -1;
	}	

	return c;
}

int MSG_ReadLong( msg_t *msg ) {
	int	c;
	
	c = MSG_ReadBits( msg, 32 );
	if ( msg->readcount > msg->cursize ) {
		c = -1;
	}	
	
	return c;
}

float MSG_ReadFloat( msg_t *msg ) {
	union {
		byte	b[4];
		float	f;
		int	l;
	} dat;
	
	dat.l = MSG_ReadBits( msg, 32 );
	if ( msg->readcount > msg->cursize ) {
		dat.f = -1;
	}	
	
	return dat.f;	
}

char *MSG_ReadString( msg_t *msg ) {
	static char	string[MAX_STRING_CHARS];
	int		l,c;
	
	l = 0;
	do {
		c = MSG_ReadByte(msg);		// use ReadByte so -1 is out of bounds
		if ( c == -1 || c == 0 ) {
			break;
		}
		// translate all fmt spec to avoid crash bugs
		if ( c == '%' ) {
			c = '.';
		}
		// don't allow higher ascii values
		if ( c > 127 ) {
			c = '.';
		}

		string[l] = c;
		l++;
	} while (l <= sizeof(string)-1);
	
	// some bonus protection, shouldn't occur cause server doesn't write such things
	if (l <= sizeof(string)-1)
	{
		string[l] = 0;
	}
	else
	{
		string[sizeof(string)-1] = 0;
	}
	
	return string;
}

char *MSG_ReadBigString( msg_t *msg ) {
	static char	string[BIG_INFO_STRING];
	int		l,c;
	
	l = 0;
	do {
		c = MSG_ReadByte(msg);		// use ReadByte so -1 is out of bounds
		if ( c == -1 || c == 0 ) {
			break;
		}
		// translate all fmt spec to avoid crash bugs
		if ( c == '%' ) {
			c = '.';
		}

		string[l] = c;
		l++;
	} while (l < sizeof(string)-1);
	
	string[l] = 0;
	
	return string;
}

char *MSG_ReadStringLine( msg_t *msg ) {
	static char	string[MAX_STRING_CHARS];
	int		l,c;

	l = 0;
	do {
		c = MSG_ReadByte(msg);		// use ReadByte so -1 is out of bounds
		if (c == -1 || c == 0 || c == '\n') {
			break;
		}
		// translate all fmt spec to avoid crash bugs
		if ( c == '%' ) {
			c = '.';
		}
		string[l] = c;
		l++;
	} while (l < sizeof(string)-1);
	
	string[l] = 0;
	
	return string;
}

float MSG_ReadAngle16( msg_t *msg ) {
	return SHORT2ANGLE(MSG_ReadShort(msg));
}

void MSG_ReadData( msg_t *msg, void *data, int len ) {
	int		i;

	for (i=0 ; i<len ; i++) {
		((byte *)data)[i] = MSG_ReadByte (msg);
	}
}


/*
=============================================================================

delta functions
  
=============================================================================
*/

extern cvar_t *cl_shownet;

#define	LOG(x) if( cl_shownet->integer == 4 ) { Com_Printf("%s ", x ); };

void MSG_WriteDelta( msg_t *msg, int oldV, int newV, int bits ) {
	if ( oldV == newV ) {
		MSG_WriteBits( msg, 0, 1 );
		return;
	}
	MSG_WriteBits( msg, 1, 1 );
	MSG_WriteBits( msg, newV, bits );
}

int	MSG_ReadDelta( msg_t *msg, int oldV, int bits ) {
	if ( MSG_ReadBits( msg, 1 ) ) {
		return MSG_ReadBits( msg, bits );
	}
	return oldV;
}

void MSG_WriteDeltaFloat( msg_t *msg, float oldV, float newV ) {
	if ( oldV == newV ) {
		MSG_WriteBits( msg, 0, 1 );
		return;
	}
	MSG_WriteBits( msg, 1, 1 );
	MSG_WriteBits( msg, *(int *)&newV, 32 );
}

float MSG_ReadDeltaFloat( msg_t *msg, float oldV ) {
	if ( MSG_ReadBits( msg, 1 ) ) {
		float	newV;

		*(int *)&newV = MSG_ReadBits( msg, 32 );
		return newV;
	}
	return oldV;
}

/*
=============================================================================

delta functions with keys
  
=============================================================================
*/

int kbitmask[32] = {
	0x00000001, 0x00000003, 0x00000007, 0x0000000F,
	0x0000001F,	0x0000003F,	0x0000007F,	0x000000FF,
	0x000001FF,	0x000003FF,	0x000007FF,	0x00000FFF,
	0x00001FFF,	0x00003FFF,	0x00007FFF,	0x0000FFFF,
	0x0001FFFF,	0x0003FFFF,	0x0007FFFF,	0x000FFFFF,
	0x001FFFFf,	0x003FFFFF,	0x007FFFFF,	0x00FFFFFF,
	0x01FFFFFF,	0x03FFFFFF,	0x07FFFFFF,	0x0FFFFFFF,
	0x1FFFFFFF,	0x3FFFFFFF,	0x7FFFFFFF,	0xFFFFFFFF,
};

void MSG_WriteDeltaKey( msg_t *msg, int key, int oldV, int newV, int bits ) 
{
	if ( oldV == newV ) 
	{
		MSG_WriteBits( msg, 0, 1 );
		return;
	}
	MSG_WriteBits( msg, 1, 1 );
	MSG_WriteBits( msg, (newV ^ key) & ((1 << bits) - 1), bits );
}

int	MSG_ReadDeltaKey( msg_t *msg, int key, int oldV, int bits ) {
	if ( MSG_ReadBits( msg, 1 ) ) {
		return MSG_ReadBits( msg, bits ) ^ (key & kbitmask[bits]);
	}
	return oldV;
}

void MSG_WriteDeltaKeyFloat( msg_t *msg, int key, float oldV, float newV ) {
	if ( oldV == newV ) {
		MSG_WriteBits( msg, 0, 1 );
		return;
	}
	MSG_WriteBits( msg, 1, 1 );
	MSG_WriteBits( msg, (*(int *)&newV) ^ key, 32 );
}

float MSG_ReadDeltaKeyFloat( msg_t *msg, int key, float oldV ) {
	if ( MSG_ReadBits( msg, 1 ) ) {
		float	newV;

		*(int *)&newV = MSG_ReadBits( msg, 32 ) ^ key;
		return newV;
	}
	return oldV;
}


/*
============================================================================

usercmd_t communication

============================================================================
*/

// ms is allways sent, the others are optional
#define	CM_ANGLE1 	(1<<0)
#define	CM_ANGLE2 	(1<<1)
#define	CM_ANGLE3 	(1<<2)
#define	CM_FORWARD	(1<<3)
#define	CM_SIDE		(1<<4)
#define	CM_UP		(1<<5)
#define	CM_BUTTONS	(1<<6)
#define CM_WEAPON	(1<<7)
//rww - these are new
#define CM_FORCE	(1<<8)
#define CM_INVEN	(1<<9)

/*
=====================
MSG_WriteDeltaUsercmd
=====================
*/
void MSG_WriteDeltaUsercmd( msg_t *msg, usercmd_t *from, usercmd_t *to ) {
	if ( to->serverTime - from->serverTime < 256 ) {
		MSG_WriteBits( msg, 1, 1 );
		MSG_WriteBits( msg, to->serverTime - from->serverTime, 8 );
	} else {
		MSG_WriteBits( msg, 0, 1 );
		MSG_WriteBits( msg, to->serverTime, 32 );
	}
	MSG_WriteDelta( msg, from->angles[0], to->angles[0], 16 );
	MSG_WriteDelta( msg, from->angles[1], to->angles[1], 16 );
	MSG_WriteDelta( msg, from->angles[2], to->angles[2], 16 );
	MSG_WriteDelta( msg, from->forwardmove, to->forwardmove, 8 );
	MSG_WriteDelta( msg, from->rightmove, to->rightmove, 8 );
	MSG_WriteDelta( msg, from->upmove, to->upmove, 8 );
	MSG_WriteDelta( msg, from->buttons, to->buttons, 16 );
	MSG_WriteDelta( msg, from->weapon, to->weapon, 8 );

	MSG_WriteDelta( msg, from->forcesel, to->forcesel, 8 );
	MSG_WriteDelta( msg, from->invensel, to->invensel, 8 );

	MSG_WriteDelta( msg, from->generic_cmd, to->generic_cmd, 8 );
}


/*
=====================
MSG_ReadDeltaUsercmd
=====================
*/
void MSG_ReadDeltaUsercmd( msg_t *msg, usercmd_t *from, usercmd_t *to ) {
	if ( MSG_ReadBits( msg, 1 ) ) {
		to->serverTime = from->serverTime + MSG_ReadBits( msg, 8 );
	} else {
		to->serverTime = MSG_ReadBits( msg, 32 );
	}
	to->angles[0] = MSG_ReadDelta( msg, from->angles[0], 16);
	to->angles[1] = MSG_ReadDelta( msg, from->angles[1], 16);
	to->angles[2] = MSG_ReadDelta( msg, from->angles[2], 16);
	to->forwardmove = MSG_ReadDelta( msg, from->forwardmove, 8);
	to->rightmove = MSG_ReadDelta( msg, from->rightmove, 8);
	to->upmove = MSG_ReadDelta( msg, from->upmove, 8);
	to->buttons = MSG_ReadDelta( msg, from->buttons, 16);
	to->weapon = MSG_ReadDelta( msg, from->weapon, 8);

	to->forcesel = MSG_ReadDelta( msg, from->forcesel, 8);
	to->invensel = MSG_ReadDelta( msg, from->invensel, 8);

	to->generic_cmd = MSG_ReadDelta( msg, from->generic_cmd, 8);
}

/*
=====================
MSG_WriteDeltaUsercmd
=====================
*/
void MSG_WriteDeltaUsercmdKey( msg_t *msg, int key, usercmd_t *from, usercmd_t *to ) {
	if ( to->serverTime - from->serverTime < 256 ) {
		MSG_WriteBits( msg, 1, 1 );
		MSG_WriteBits( msg, to->serverTime - from->serverTime, 8 );
	} else {
		MSG_WriteBits( msg, 0, 1 );
		MSG_WriteBits( msg, to->serverTime, 32 );
	}
	if (from->angles[0] == to->angles[0] &&
		from->angles[1] == to->angles[1] &&
		from->angles[2] == to->angles[2] &&
		from->forwardmove == to->forwardmove &&
		from->rightmove == to->rightmove &&
		from->upmove == to->upmove &&
		from->buttons == to->buttons &&
		from->weapon == to->weapon &&
		from->forcesel == to->forcesel &&
		from->invensel == to->invensel &&
		from->generic_cmd == to->generic_cmd) {
			MSG_WriteBits( msg, 0, 1 );				// no change
			oldsize += 7;
			return;
	}
	key ^= to->serverTime;
	MSG_WriteBits( msg, 1, 1 );
	MSG_WriteDeltaKey( msg, key, from->angles[0], to->angles[0], 16 );
	MSG_WriteDeltaKey( msg, key, from->angles[1], to->angles[1], 16 );
	MSG_WriteDeltaKey( msg, key, from->angles[2], to->angles[2], 16 );
	MSG_WriteDeltaKey( msg, key, from->forwardmove, to->forwardmove, 8 );
	MSG_WriteDeltaKey( msg, key, from->rightmove, to->rightmove, 8 );
	MSG_WriteDeltaKey( msg, key, from->upmove, to->upmove, 8 );
	MSG_WriteDeltaKey( msg, key, from->buttons, to->buttons, 16 );
	MSG_WriteDeltaKey( msg, key, from->weapon, to->weapon, 8 );

	MSG_WriteDeltaKey( msg, key, from->forcesel, to->forcesel, 8 );
	MSG_WriteDeltaKey( msg, key, from->invensel, to->invensel, 8 );

	MSG_WriteDeltaKey( msg, key, from->generic_cmd, to->generic_cmd, 8 );
}


/*
=====================
MSG_ReadDeltaUsercmd
=====================
*/
void MSG_ReadDeltaUsercmdKey( msg_t *msg, int key, usercmd_t *from, usercmd_t *to ) {
	if ( MSG_ReadBits( msg, 1 ) ) {
		to->serverTime = from->serverTime + MSG_ReadBits( msg, 8 );
	} else {
		to->serverTime = MSG_ReadBits( msg, 32 );
	}
	if ( MSG_ReadBits( msg, 1 ) ) {
		key ^= to->serverTime;
		to->angles[0] = MSG_ReadDeltaKey( msg, key, from->angles[0], 16);
		to->angles[1] = MSG_ReadDeltaKey( msg, key, from->angles[1], 16);
		to->angles[2] = MSG_ReadDeltaKey( msg, key, from->angles[2], 16);
		to->forwardmove = MSG_ReadDeltaKey( msg, key, from->forwardmove, 8);
		to->rightmove = MSG_ReadDeltaKey( msg, key, from->rightmove, 8);
		to->upmove = MSG_ReadDeltaKey( msg, key, from->upmove, 8);
		to->buttons = MSG_ReadDeltaKey( msg, key, from->buttons, 16);
		to->weapon = MSG_ReadDeltaKey( msg, key, from->weapon, 8);

		to->forcesel = MSG_ReadDeltaKey( msg, key, from->forcesel, 8);
		to->invensel = MSG_ReadDeltaKey( msg, key, from->invensel, 8);

		to->generic_cmd = MSG_ReadDeltaKey( msg, key, from->generic_cmd, 8);
	} else {
		to->angles[0] = from->angles[0];
		to->angles[1] = from->angles[1];
		to->angles[2] = from->angles[2];
		to->forwardmove = from->forwardmove;
		to->rightmove = from->rightmove;
		to->upmove = from->upmove;
		to->buttons = from->buttons;
		to->weapon = from->weapon;

		to->forcesel = from->forcesel;
		to->invensel = from->invensel;

		to->generic_cmd = from->generic_cmd;
	}
}

/*
=============================================================================

entityState_t communication
  
=============================================================================
*/

/*
=================
MSG_ReportChangeVectors_f

Prints out a table from the current statistics for copying to code
=================
*/
void MSG_ReportChangeVectors_f( void ) {
	int i;
	for(i=0;i<256;i++) {
		if (pcount[i]) {
			Com_Printf("%d used %d\n", i, pcount[i]);
		}
	}
}

typedef struct {
	char	*name;
	int		offset;
	int		bits;		// 0 = float
} netField_t;

// using the stringizing operator to save typing...
#define	NETF(x) #x,(int)&((entityState_t*)0)->x

netField_t	entityStateFields[] = 
{
{ NETF(pos.trTime), 32 },
{ NETF(pos.trBase[0]), 0 },
{ NETF(pos.trBase[1]), 0 },
{ NETF(pos.trDelta[0]), 0 },
{ NETF(pos.trDelta[1]), 0 },
{ NETF(pos.trBase[2]), 0 },
{ NETF(apos.trBase[1]), 0 },
{ NETF(pos.trDelta[2]), 0 },
{ NETF(apos.trBase[0]), 0 },
{ NETF(event), 10 },			// There is a maximum of 256 events (8 bits transmission, 2 high bits for uniqueness)
{ NETF(angles2[1]), 0 },
{ NETF(eType), 8 },
{ NETF(torsoAnim), 16 },		// Maximum number of animation sequences is 2048.  Top bit is reserved for the togglebit
{ NETF(forceFrame), 16 }, //if you have over 65536 frames, then this will explode. Of course if you have that many things then lots of things will probably explode.
{ NETF(eventParm), 8 },
{ NETF(legsAnim), 16 },			// Maximum number of animation sequences is 2048.  Top bit is reserved for the togglebit
{ NETF(groundEntityNum), GENTITYNUM_BITS },
{ NETF(pos.trType), 8 },
{ NETF(eFlags), 32 },
{ NETF(bolt1), 8 },
{ NETF(bolt2), GENTITYNUM_BITS },
{ NETF(trickedentindex), 16 }, //See note in PSF
{ NETF(trickedentindex2), 16 },
{ NETF(trickedentindex3), 16 },
{ NETF(trickedentindex4), 16 },
{ NETF(speed), 0 },
{ NETF(fireflag), 2 },
{ NETF(genericenemyindex), 32 }, //Do not change to GENTITYNUM_BITS, used as a time offset for seeker
{ NETF(activeForcePass), 6 },
{ NETF(emplacedOwner), 32 }, //As above, also used as a time value (for electricity render time)
{ NETF(otherEntityNum), GENTITYNUM_BITS },
{ NETF(weapon), 8 },
{ NETF(clientNum), 8 },
{ NETF(angles[1]), 0 },
{ NETF(pos.trDuration), 32 },
{ NETF(apos.trType), 8 },
{ NETF(origin[0]), 0 },
{ NETF(origin[1]), 0 },
{ NETF(origin[2]), 0 },
{ NETF(solid), 24 },
{ NETF(owner), GENTITYNUM_BITS },
{ NETF(teamowner), 8 },
{ NETF(shouldtarget), 1 },
{ NETF(powerups), 16 },
{ NETF(modelGhoul2), 4 },
{ NETF(g2radius), 8 },
{ NETF(modelindex), -8 },
{ NETF(otherEntityNum2), GENTITYNUM_BITS },
{ NETF(loopSound), 8 },
{ NETF(generic1), 8 },
{ NETF(origin2[2]), 0 },
{ NETF(origin2[0]), 0 },
{ NETF(origin2[1]), 0 },
{ NETF(modelindex2), 8 },
{ NETF(angles[0]), 0 },
{ NETF(time), 32 },
{ NETF(apos.trTime), 32 },
{ NETF(apos.trDuration), 32 },
{ NETF(apos.trBase[2]), 0 },
{ NETF(apos.trDelta[0]), 0 },
{ NETF(apos.trDelta[1]), 0 },
{ NETF(apos.trDelta[2]), 0 },
{ NETF(time2), 32 },
{ NETF(angles[2]), 0 },
{ NETF(angles2[0]), 0 },
{ NETF(angles2[2]), 0 },
{ NETF(constantLight), 32 },
{ NETF(frame), 16 },
{ NETF(saberInFlight), 1 },
{ NETF(saberEntityNum), GENTITYNUM_BITS },
{ NETF(saberMove), 8 },
{ NETF(forcePowersActive), 32 },
{ NETF(isJediMaster), 1 }
};

// if (int)f == f and (int)f + ( 1<<(FLOAT_INT_BITS-1) ) < ( 1 << FLOAT_INT_BITS )
// the float will be sent with FLOAT_INT_BITS, otherwise all 32 bits will be sent
#define	FLOAT_INT_BITS	13
#define	FLOAT_INT_BIAS	(1<<(FLOAT_INT_BITS-1))

/*
==================
MSG_WriteDeltaEntity

Writes part of a packetentities message, including the entity number.
Can delta from either a baseline or a previous packet_entity
If to is NULL, a remove entity update will be sent
If force is not set, then nothing at all will be generated if the entity is
identical, under the assumption that the in-order delta code will catch it.
==================
*/
void MSG_WriteDeltaEntity( msg_t *msg, struct entityState_s *from, struct entityState_s *to, 
						   qboolean force ) {
	int			i, lc;
	int			numFields;
	netField_t	*field;
	int			trunc;
	float		fullFloat;
	int			*fromF, *toF;

	numFields = sizeof(entityStateFields)/sizeof(entityStateFields[0]);

	// all fields should be 32 bits to avoid any compiler packing issues
	// the "number" field is not part of the field list
	// if this assert fails, someone added a field to the entityState_t
	// struct without updating the message fields
	assert( numFields + 1 == sizeof( *from )/4 );

	// a NULL to is a delta remove message
	if ( to == NULL ) {
		if ( from == NULL ) {
			return;
		}
		MSG_WriteBits( msg, from->number, GENTITYNUM_BITS );
		MSG_WriteBits( msg, 1, 1 );
		return;
	}

	if ( to->number < 0 || to->number >= MAX_GENTITIES ) {
		Com_Error (ERR_FATAL, "MSG_WriteDeltaEntity: Bad entity number: %i", to->number );
	}

	lc = 0;
	// build the change vector as bytes so it is endien independent
	for ( i = 0, field = entityStateFields ; i < numFields ; i++, field++ ) {
		fromF = (int *)( (byte *)from + field->offset );
		toF = (int *)( (byte *)to + field->offset );
		if ( *fromF != *toF ) {
			lc = i+1;
		}
	}

	if ( lc == 0 ) {
		// nothing at all changed
		if ( !force ) {
			return;		// nothing at all
		}
		// write two bits for no change
		MSG_WriteBits( msg, to->number, GENTITYNUM_BITS );
		MSG_WriteBits( msg, 0, 1 );		// not removed
		MSG_WriteBits( msg, 0, 1 );		// no delta
		return;
	}

	MSG_WriteBits( msg, to->number, GENTITYNUM_BITS );
	MSG_WriteBits( msg, 0, 1 );			// not removed
	MSG_WriteBits( msg, 1, 1 );			// we have a delta

	MSG_WriteByte( msg, lc );	// # of changes

	oldsize += numFields;

	for ( i = 0, field = entityStateFields ; i < lc ; i++, field++ ) {
		fromF = (int *)( (byte *)from + field->offset );
		toF = (int *)( (byte *)to + field->offset );

		if ( *fromF == *toF ) {
			MSG_WriteBits( msg, 0, 1 );	// no change
			continue;
		}

		MSG_WriteBits( msg, 1, 1 );	// changed

		if ( field->bits == 0 ) {
			// float
			fullFloat = *(float *)toF;
			trunc = (int)fullFloat;

			if (fullFloat == 0.0f) {
					MSG_WriteBits( msg, 0, 1 );
					oldsize += FLOAT_INT_BITS;
			} else {
				MSG_WriteBits( msg, 1, 1 );
				if ( trunc == fullFloat && trunc + FLOAT_INT_BIAS >= 0 && 
					trunc + FLOAT_INT_BIAS < ( 1 << FLOAT_INT_BITS ) ) {
					// send as small integer
					MSG_WriteBits( msg, 0, 1 );
					MSG_WriteBits( msg, trunc + FLOAT_INT_BIAS, FLOAT_INT_BITS );
				} else {
					// send as full floating point value
					MSG_WriteBits( msg, 1, 1 );
					MSG_WriteBits( msg, *toF, 32 );
				}
			}
		} else {
			if (*toF == 0) {
				MSG_WriteBits( msg, 0, 1 );
			} else {
				MSG_WriteBits( msg, 1, 1 );
				// integer
				MSG_WriteBits( msg, *toF, field->bits );
			}
		}
	}
}

/*
==================
MSG_ReadDeltaEntity

The entity number has already been read from the message, which
is how the from state is identified.

If the delta removes the entity, entityState_t->number will be set to MAX_GENTITIES-1

Can go from either a baseline or a previous packet_entity
==================
*/
extern	cvar_t	*cl_shownet;

void MSG_ReadDeltaEntity( msg_t *msg, entityState_t *from, entityState_t *to, 
						 int number) {
	int			i, lc;
	int			numFields;
	netField_t	*field;
	int			*fromF, *toF;
	int			print;
	int			trunc;
	int			startBit, endBit;

	if ( number < 0 || number >= MAX_GENTITIES) {
		Com_Error( ERR_DROP, "Bad delta entity number: %i", number );
	}

	if ( msg->bit == 0 ) {
		startBit = msg->readcount * 8 - GENTITYNUM_BITS;
	} else {
		startBit = ( msg->readcount - 1 ) * 8 + msg->bit - GENTITYNUM_BITS;
	}

	// check for a remove
	if ( MSG_ReadBits( msg, 1 ) == 1 ) {
		Com_Memset( to, 0, sizeof( *to ) );	
		to->number = MAX_GENTITIES - 1;
		if ( cl_shownet->integer >= 2 || cl_shownet->integer == -1 ) {
			Com_Printf( "%3i: #%-3i remove\n", msg->readcount, number );
		}
		return;
	}

	// check for no delta
	if ( MSG_ReadBits( msg, 1 ) == 0 ) {
		*to = *from;
		to->number = number;
		return;
	}

	numFields = sizeof(entityStateFields)/sizeof(entityStateFields[0]);
	lc = MSG_ReadByte(msg);

	// shownet 2/3 will interleave with other printed info, -1 will
	// just print the delta records`
	if ( cl_shownet->integer >= 2 || cl_shownet->integer == -1 ) {
		print = 1;
		Com_Printf( "%3i: #%-3i ", msg->readcount, to->number );
	} else {
		print = 0;
	}

	to->number = number;

#ifdef _DONETPROFILE_
	int startBytes,endBytes;
#endif

	for ( i = 0, field = entityStateFields ; i < lc ; i++, field++ ) {
		fromF = (int *)( (byte *)from + field->offset );
		toF = (int *)( (byte *)to + field->offset );

#ifdef _DONETPROFILE_
		startBytes=msg->readcount;
#endif
		if ( ! MSG_ReadBits( msg, 1 ) ) {
			// no change
			*toF = *fromF;
		} else {
			if ( field->bits == 0 ) {
				// float
				if ( MSG_ReadBits( msg, 1 ) == 0 ) {
						*(float *)toF = 0.0f; 
				} else {
					if ( MSG_ReadBits( msg, 1 ) == 0 ) {
						// integral float
						trunc = MSG_ReadBits( msg, FLOAT_INT_BITS );
						// bias to allow equal parts positive and negative
						trunc -= FLOAT_INT_BIAS;
						*(float *)toF = trunc; 
						if ( print ) {
							Com_Printf( "%s:%i ", field->name, trunc );
						}
					} else {
						// full floating point value
						*toF = MSG_ReadBits( msg, 32 );
						if ( print ) {
							Com_Printf( "%s:%f ", field->name, *(float *)toF );
						}
					}
				}
			} else {
				if ( MSG_ReadBits( msg, 1 ) == 0 ) {
					*toF = 0;
				} else {
					// integer
					*toF = MSG_ReadBits( msg, field->bits );
					if ( print ) {
						Com_Printf( "%s:%i ", field->name, *toF );
					}
				}
			}
//			pcount[i]++;
		}
#ifdef _DONETPROFILE_
		endBytes=msg->readcount;
		ClReadProf().AddField(field->name,endBytes-startBytes);
#endif
	}
	for ( i = lc, field = &entityStateFields[lc] ; i < numFields ; i++, field++ ) {
		fromF = (int *)( (byte *)from + field->offset );
		toF = (int *)( (byte *)to + field->offset );
		// no change
		*toF = *fromF;
	}

	if ( print ) {
		if ( msg->bit == 0 ) {
			endBit = msg->readcount * 8 - GENTITYNUM_BITS;
		} else {
			endBit = ( msg->readcount - 1 ) * 8 + msg->bit - GENTITYNUM_BITS;
		}
		Com_Printf( " (%i bits)\n", endBit - startBit  );
	}
}

/*
============================================================================

plyer_state_t communication

============================================================================
*/

// using the stringizing operator to save typing...
#define	PSF(x) #x,(int)&((playerState_t*)0)->x

netField_t	playerStateFields[] = 
{
{ PSF(commandTime), 32 },				
{ PSF(origin[0]), 0 },
{ PSF(origin[1]), 0 },
{ PSF(bobCycle), 8 },
{ PSF(velocity[0]), 0 },
{ PSF(velocity[1]), 0 },
{ PSF(viewangles[1]), 0 },
{ PSF(viewangles[0]), 0 },
{ PSF(weaponTime), -16 },
{ PSF(weaponChargeTime), 32 }, //? really need 32 bits??
{ PSF(weaponChargeSubtractTime), 32 }, //? really need 32 bits??
{ PSF(origin[2]), 0 },
{ PSF(velocity[2]), 0 },
{ PSF(pm_time), -16 },
{ PSF(eventSequence), 16 },
{ PSF(torsoAnim), 16 },			// Maximum number of animation sequences is 2048.  Top bit is reserved for the togglebit
{ PSF(torsoTimer), 16 },
{ PSF(legsAnim), 16 },			// Maximum number of animation sequences is 2048.  Top bit is reserved for the togglebit
{ PSF(legsTimer), 16 },
{ PSF(movementDir), 4 },
{ PSF(events[0]), 10 },			// There is a maximum of 256 events (8 bits transmission, 2 high bits for uniqueness)
{ PSF(events[1]), 10 },			// There is a maximum of 256 events (8 bits transmission, 2 high bits for uniqueness)
{ PSF(pm_flags), 16 },
{ PSF(groundEntityNum), GENTITYNUM_BITS },
{ PSF(weaponstate), 4 },
{ PSF(eFlags), 32 },
{ PSF(externalEvent), 10 },
{ PSF(gravity), 16 },
{ PSF(speed), 16 },
{ PSF(basespeed), 16 },
{ PSF(delta_angles[1]), 16 },
{ PSF(externalEventParm), 8 },
{ PSF(viewheight), -8 },
{ PSF(damageEvent), 8 },
{ PSF(damageYaw), 8 },
{ PSF(damagePitch), 8 },
{ PSF(damageCount), 8 },
{ PSF(damageType), 2 },
{ PSF(generic1), 8 },
{ PSF(pm_type), 8 },					
{ PSF(delta_angles[0]), 16 },
{ PSF(delta_angles[2]), 16 },
{ PSF(eventParms[0]), -16 },
{ PSF(eventParms[1]), 8 },
{ PSF(clientNum), 8 },
{ PSF(weapon), 5 },
{ PSF(viewangles[2]), 0 },
{ PSF(jumppad_ent), 10 },
{ PSF(loopSound), 16 },

// NOTENOTE Are all of these necessary?
{ PSF(zoomMode), 2 },
{ PSF(zoomTime), 32 },
{ PSF(zoomLocked), 1 },
{ PSF(zoomFov), 8 },

{ PSF(fd.forcePowersActive), 32 },
{ PSF(fd.forceMindtrickTargetIndex), 16 }, //NOTE: Not just an index, used as a (1 << val) bitflag for up to 16 clients
{ PSF(fd.forceMindtrickTargetIndex2), 16 }, //NOTE: Not just an index, used as a (1 << val) bitflag for up to 16 clients
{ PSF(fd.forceMindtrickTargetIndex3), 16 }, //NOTE: Not just an index, used as a (1 << val) bitflag for up to 16 clients
{ PSF(fd.forceMindtrickTargetIndex4), 16 }, //NOTE: Not just an index, used as a (1 << val) bitflag for up to 16 clients
{ PSF(fd.forceJumpZStart), 0 },
{ PSF(fd.forcePowerSelected), 8 },
{ PSF(fd.forcePowersKnown), 32 },
{ PSF(fd.forcePower), 8 },
{ PSF(fd.forceSide), 2 }, //so we know if we should apply greyed out shaders to dark/light force enlightenment
{ PSF(fd.sentryDeployed), 1 },
{ PSF(fd.forcePowerLevel[FP_LEVITATION]), 2 }, //unfortunately we need this for fall damage calculation (client needs to know the distance for the fall noise)
{ PSF(fd.forcePowerLevel[FP_SEE]), 2 }, //needed for knowing when to display players through walls
{ PSF(genericEnemyIndex), 32 }, //NOTE: This isn't just an index all the time, it's often used as a time value, and thus needs 32 bits
{ PSF(activeForcePass), 6 },
{ PSF(hasDetPackPlanted), 1 },
{ PSF(emplacedIndex), GENTITYNUM_BITS },
{ PSF(fd.forceRageRecoveryTime), 32 },
{ PSF(rocketLockIndex), 8 }, //should never exceed MAX_CLIENTS
{ PSF(rocketLockTime), 32 },
{ PSF(rocketTargetTime), 32 },
{ PSF(holocronBits), 32 },
{ PSF(isJediMaster), 1 },
{ PSF(fallingToDeath), 32 },
{ PSF(electrifyTime), 32 },

{ PSF(fd.forcePowerDebounce[FP_LEVITATION]), 32 },

{ PSF(saberMove), 32 }, //This value sometimes exceeds the max LS_ value and gets set to a crazy amount, so it needs 32 bits
{ PSF(saberActive), 1 },
{ PSF(saberInFlight), 1 },
{ PSF(saberBlocked), 8 },
{ PSF(saberEntityNum), GENTITYNUM_BITS }, //Also used for channel tracker storage, but should never exceed entity number
{ PSF(saberCanThrow), 1 },
{ PSF(forceHandExtend), 8 },
{ PSF(forceDodgeAnim), 16 },
{ PSF(fd.saberAnimLevel), 2 },
{ PSF(fd.saberDrawAnimLevel), 2 },
{ PSF(saberAttackChainCount), 4 },
{ PSF(saberHolstered), 1 },
{ PSF(usingATST), 1 },
{ PSF(atstAltFire), 1 },

{ PSF(duelIndex), GENTITYNUM_BITS },
{ PSF(duelTime), 32 },
{ PSF(duelInProgress), 1 },

{ PSF(saberLockTime), 32 },
{ PSF(saberLockEnemy), GENTITYNUM_BITS },
{ PSF(saberLockFrame), 16 },
{ PSF(saberLockAdvance), 1 },

{ PSF(inAirAnim), 1 }, //just transmit it for the sake of knowing right when on the client to play a land anim, it's only 1 bit
{ PSF(dualBlade), 1 },

{ PSF(lastHitLoc[2]), 0 },
{ PSF(lastHitLoc[0]), 0 },
{ PSF(lastHitLoc[1]), 0 } //currently only used so client knows to orient disruptor disintegration.. seems a bit much for just that though.
};

/*
=============
MSG_WriteDeltaPlayerstate

=============
*/
void MSG_WriteDeltaPlayerstate( msg_t *msg, struct playerState_s *from, struct playerState_s *to ) {
	int				i;
	playerState_t	dummy;
	int				statsbits;
	int				persistantbits;
	int				ammobits;
	int				powerupbits;
	int				numFields;
	int				c;
	netField_t		*field;
	int				*fromF, *toF;
	float			fullFloat;
	int				trunc, lc;

	if (!from) {
		from = &dummy;
		Com_Memset (&dummy, 0, sizeof(dummy));
	}

	c = msg->cursize;

	numFields = sizeof( playerStateFields ) / sizeof( playerStateFields[0] );

	lc = 0;
	for ( i = 0, field = playerStateFields ; i < numFields ; i++, field++ ) {
		fromF = (int *)( (byte *)from + field->offset );
		toF = (int *)( (byte *)to + field->offset );
		if ( *fromF != *toF ) {
			lc = i+1;
		}
	}

	MSG_WriteByte( msg, lc );	// # of changes

#ifndef FINAL_BUILD
	gLastBitIndex = lc;
#endif

	oldsize += numFields - lc;

	for ( i = 0, field = playerStateFields ; i < lc ; i++, field++ ) {
		fromF = (int *)( (byte *)from + field->offset );
		toF = (int *)( (byte *)to + field->offset );

		if ( *fromF == *toF ) {
			MSG_WriteBits( msg, 0, 1 );	// no change
			continue;
		}

		MSG_WriteBits( msg, 1, 1 );	// changed
//		pcount[i]++;

		if ( field->bits == 0 ) {
			// float
			fullFloat = *(float *)toF;
			trunc = (int)fullFloat;

			if ( trunc == fullFloat && trunc + FLOAT_INT_BIAS >= 0 && 
				trunc + FLOAT_INT_BIAS < ( 1 << FLOAT_INT_BITS ) ) {
				// send as small integer
				MSG_WriteBits( msg, 0, 1 );
				MSG_WriteBits( msg, trunc + FLOAT_INT_BIAS, FLOAT_INT_BITS );
			} else {
				// send as full floating point value
				MSG_WriteBits( msg, 1, 1 );
				MSG_WriteBits( msg, *toF, 32 );
			}
		} else {
			// integer
			MSG_WriteBits( msg, *toF, field->bits );
		}
	}
	c = msg->cursize - c;


	//
	// send the arrays
	//
	statsbits = 0;
	for (i=0 ; i<16 ; i++) {
		if (to->stats[i] != from->stats[i]) {
			statsbits |= 1<<i;
		}
	}
	persistantbits = 0;
	for (i=0 ; i<16 ; i++) {
		if (to->persistant[i] != from->persistant[i]) {
			persistantbits |= 1<<i;
		}
	}
	ammobits = 0;
	for (i=0 ; i<16 ; i++) {
		if (to->ammo[i] != from->ammo[i]) {
			ammobits |= 1<<i;
		}
	}
	powerupbits = 0;
	for (i=0 ; i<16 ; i++) {
		if (to->powerups[i] != from->powerups[i]) {
			powerupbits |= 1<<i;
		}
	}

	if (!statsbits && !persistantbits && !ammobits && !powerupbits) {
		MSG_WriteBits( msg, 0, 1 );	// no change
		oldsize += 4;
		return;
	}
	MSG_WriteBits( msg, 1, 1 );	// changed

	if ( statsbits ) {
		MSG_WriteBits( msg, 1, 1 );	// changed
		MSG_WriteShort( msg, statsbits );
		for (i=0 ; i<16 ; i++)
			if (statsbits & (1<<i) )
				MSG_WriteShort (msg, to->stats[i]);
	} else {
		MSG_WriteBits( msg, 0, 1 );	// no change
	}


	if ( persistantbits ) {
		MSG_WriteBits( msg, 1, 1 );	// changed
		MSG_WriteShort( msg, persistantbits );
		for (i=0 ; i<16 ; i++)
			if (persistantbits & (1<<i) )
				MSG_WriteShort (msg, to->persistant[i]);
	} else {
		MSG_WriteBits( msg, 0, 1 );	// no change
	}


	if ( ammobits ) {
		MSG_WriteBits( msg, 1, 1 );	// changed
		MSG_WriteShort( msg, ammobits );
		for (i=0 ; i<16 ; i++)
			if (ammobits & (1<<i) )
				MSG_WriteShort (msg, to->ammo[i]);
	} else {
		MSG_WriteBits( msg, 0, 1 );	// no change
	}


	if ( powerupbits ) {
		MSG_WriteBits( msg, 1, 1 );	// changed
		MSG_WriteShort( msg, powerupbits );
		for (i=0 ; i<16 ; i++)
			if (powerupbits & (1<<i) )
				MSG_WriteLong( msg, to->powerups[i] );
	} else {
		MSG_WriteBits( msg, 0, 1 );	// no change
	}
}


/*
===================
MSG_ReadDeltaPlayerstate
===================
*/
void MSG_ReadDeltaPlayerstate (msg_t *msg, playerState_t *from, playerState_t *to ) {
	int			i, lc;
	int			bits;
	netField_t	*field;
	int			numFields;
	int			startBit, endBit;
	int			print;
	int			*fromF, *toF;
	int			trunc;
	playerState_t	dummy;

	if ( !from ) {
		from = &dummy;
		Com_Memset( &dummy, 0, sizeof( dummy ) );
	}
	*to = *from;

	if ( msg->bit == 0 ) {
		startBit = msg->readcount * 8 - GENTITYNUM_BITS;
	} else {
		startBit = ( msg->readcount - 1 ) * 8 + msg->bit - GENTITYNUM_BITS;
	}

	// shownet 2/3 will interleave with other printed info, -2 will
	// just print the delta records
	if ( cl_shownet->integer >= 2 || cl_shownet->integer == -2 ) {
		print = 1;
		Com_Printf( "%3i: playerstate ", msg->readcount );
	} else {
		print = 0;
	}

	numFields = sizeof( playerStateFields ) / sizeof( playerStateFields[0] );
	lc = MSG_ReadByte(msg);

#ifdef _DONETPROFILE_
	int startBytes,endBytes;
#endif

	for ( i = 0, field = playerStateFields ; i < lc ; i++, field++ ) {
		fromF = (int *)( (byte *)from + field->offset );
		toF = (int *)( (byte *)to + field->offset );

#ifdef _DONETPROFILE_
		startBytes=msg->readcount;
#endif
		if ( ! MSG_ReadBits( msg, 1 ) ) {
			// no change
			*toF = *fromF;
		} else {
			if ( field->bits == 0 ) {
				// float
				if ( MSG_ReadBits( msg, 1 ) == 0 ) {
					// integral float
					trunc = MSG_ReadBits( msg, FLOAT_INT_BITS );
					// bias to allow equal parts positive and negative
					trunc -= FLOAT_INT_BIAS;
					*(float *)toF = trunc; 
					if ( print ) {
						Com_Printf( "%s:%i ", field->name, trunc );
					}
				} else {
					// full floating point value
					*toF = MSG_ReadBits( msg, 32 );
					if ( print ) {
						Com_Printf( "%s:%f ", field->name, *(float *)toF );
					}
				}
			} else {
				// integer
				*toF = MSG_ReadBits( msg, field->bits );
				if ( print ) {
					Com_Printf( "%s:%i ", field->name, *toF );
				}
			}
		}
#ifdef _DONETPROFILE_
		endBytes=msg->readcount;
		ClReadProf().AddField(field->name,endBytes-startBytes);
#endif
	}
	for ( i=lc,field = &playerStateFields[lc];i<numFields; i++, field++) {
		fromF = (int *)( (byte *)from + field->offset );
		toF = (int *)( (byte *)to + field->offset );
		// no change
		*toF = *fromF;
	}

	// read the arrays
#ifdef _DONETPROFILE_
		startBytes=msg->readcount;
#endif
	if (MSG_ReadBits( msg, 1 ) ) {
		// parse stats
		if ( MSG_ReadBits( msg, 1 ) ) {
			LOG("PS_STATS");
			bits = MSG_ReadShort (msg);
			for (i=0 ; i<16 ; i++) {
				if (bits & (1<<i) ) {
					to->stats[i] = MSG_ReadShort(msg);
				}
			}
		}
#ifdef _DONETPROFILE_
		endBytes=msg->readcount;
		ClReadProf().AddField("PS_STATS",endBytes-startBytes);
#endif

		// parse persistant stats
#ifdef _DONETPROFILE_
		startBytes=msg->readcount;
#endif
		if ( MSG_ReadBits( msg, 1 ) ) {
			LOG("PS_PERSISTANT");
			bits = MSG_ReadShort (msg);
			for (i=0 ; i<16 ; i++) {
				if (bits & (1<<i) ) {
					to->persistant[i] = MSG_ReadShort(msg);
				}
			}
		}
#ifdef _DONETPROFILE_
		endBytes=msg->readcount;
		ClReadProf().AddField("PS_PERSISTANT",endBytes-startBytes);
#endif

		// parse ammo
#ifdef _DONETPROFILE_
		startBytes=msg->readcount;
#endif
		if ( MSG_ReadBits( msg, 1 ) ) {
			LOG("PS_AMMO");
			bits = MSG_ReadShort (msg);
			for (i=0 ; i<16 ; i++) {
				if (bits & (1<<i) ) {
					to->ammo[i] = MSG_ReadShort(msg);
				}
			}
		}
#ifdef _DONETPROFILE_
		endBytes=msg->readcount;
		ClReadProf().AddField("PS_AMMO",endBytes-startBytes);
#endif

		// parse powerups
#ifdef _DONETPROFILE_
		startBytes=msg->readcount;
#endif
		if ( MSG_ReadBits( msg, 1 ) ) {
			LOG("PS_POWERUPS");
			bits = MSG_ReadShort (msg);
			for (i=0 ; i<16 ; i++) {
				if (bits & (1<<i) ) {
					to->powerups[i] = MSG_ReadLong(msg);
				}
			}
		}
	}
#ifdef _DONETPROFILE_
		endBytes=msg->readcount;
		ClReadProf().AddField("PS_POWERUPS",endBytes-startBytes);
#endif

	if ( print ) {
		if ( msg->bit == 0 ) {
			endBit = msg->readcount * 8 - GENTITYNUM_BITS;
		} else {
			endBit = ( msg->readcount - 1 ) * 8 + msg->bit - GENTITYNUM_BITS;
		}
		Com_Printf( " (%i bits)\n", endBit - startBit  );
	}
}

/*
// New data gathered to tune Q3 to JK2MP. Takes longer to crunch and gain was minimal.
int msg_hData[256] = 
{
	3163878,		// 0
	473992,			// 1
	564019,			// 2
	136497,			// 3
	129559,			// 4
	283019,			// 5
	75812,			// 6
	179836,			// 7
	85958,			// 8
	168542,			// 9
	78898,			// 10
	82007,			// 11
	48613,			// 12
	138741,			// 13
	35482,			// 14
	47433,			// 15
	65214,			// 16
	51636,			// 17
	63741,			// 18
	52823,			// 19
	42464,			// 20
	44495,			// 21
	45347,			// 22
	40260,			// 23
	59168,			// 24
	44990,			// 25
	52957,			// 26
	42700,			// 27
	42414,			// 28
	36451,			// 29
	45653,			// 30
	44667,			// 31
	125336,			// 32
	38435,			// 33
	53658,			// 34
	42621,			// 35
	40932,			// 36
	33409,			// 37
	35470,			// 38
	40769,			// 39
	33813,			// 40
	32480,			// 41
	33664,			// 42
	32303,			// 43
	32394,			// 44
	34822,			// 45
	37724,			// 46
	48016,			// 47
	94212,			// 48
	53774,			// 49
	54522,			// 50
	44044,			// 51
	42800,			// 52
	47597,			// 53
	29742,			// 54
	30237,			// 55
	34291,			// 56
	106496,			// 57
	20963,			// 58
	19342,			// 59
	20603,			// 60
	19568,			// 61
	23013,			// 62
	23939,			// 63
	44995,			// 64
	37128,			// 65
	44264,			// 66
	46636,			// 67
	56400,			// 68
	32746,			// 69
	23458,			// 70
	29702,			// 71
	25305,			// 72
	20159,			// 73
	19645,			// 74
	20593,			// 75
	21729,			// 76
	19362,			// 77
	24760,			// 78
	22788,			// 79
	25085,			// 80
	21074,			// 81
	97271,			// 82
	22048,			// 83
	24131,			// 84
	19287,			// 85
	20296,			// 86
	20131,			// 87
	86477,			// 88
	25352,			// 89
	20872,			// 90
	21382,			// 91
	38744,			// 92
	137256,			// 93
	26025,			// 94
	22243,			// 95
	23974,			// 96
	43305,			// 97
	28191,			// 98
	34638,			// 99
	37613,			// 100
	46003,			// 101
	31415,			// 102
	25746,			// 103
	28338,			// 104
	34689,			// 105
	24948,			// 106
	27110,			// 107
	39950,			// 108
	32793,			// 109
	42639,			// 110
	47883,			// 111
	37439,			// 112
	23875,			// 113
	36092,			// 114
	46471,			// 115
	37392,			// 116
	33063,			// 117
	29604,			// 118
	42140,			// 119
	61745,			// 120
	45618,			// 121
	51779,			// 122
	49684,			// 123
	57644,			// 124
	65021,			// 125
	67318,			// 126
	88197,			// 127
	258378,			// 128
	76806,			// 129
	72430,			// 130
	64936,			// 131
	62196,			// 132
	56461,			// 133
	166474,			// 134
	70036,			// 135
	40735,			// 136
	29598,			// 137
	26966,			// 138
	26093,			// 139
	25853,			// 140
	26065,			// 141
	26176,			// 142
	26777,			// 143
	26684,			// 144
	23880,			// 145
	22932,			// 146
	24566,			// 147
	24305,			// 148
	26399,			// 149
	23487,			// 150
	24485,			// 151
	25956,			// 152
	26065,			// 153
	26151,			// 154
	23111,			// 155
	23900,			// 156
	22128,			// 157
	24096,			// 158
	20863,			// 159
	24298,			// 160
	22572,			// 161
	22364,			// 162
	20813,			// 163
	21414,			// 164
	21570,			// 165
	20799,			// 166
	20971,			// 167
	22485,			// 168
	20397,			// 169
	88096,			// 170
	17802,			// 171
	20091,			// 172
	84250,			// 173
	21953,			// 174
	21406,			// 175
	23401,			// 176
	19546,			// 177
	19180,			// 178
	18843,			// 179
	20673,			// 180
	19918,			// 181
	20640,			// 182
	20326,			// 183
	21174,			// 184
	21736,			// 185
	22511,			// 186
	20290,			// 187
	23303,			// 188
	19800,			// 189
	25465,			// 190
	22801,			// 191
	28831,			// 192
	26663,			// 193
	36485,			// 194
	45768,			// 195
	49795,			// 196
	36026,			// 197
	24119,			// 198
	18543,			// 199
	19261,			// 200
	17137,			// 201
	19435,			// 202
	23672,			// 203
	22988,			// 204
	18107,			// 205
	18734,			// 206
	19847,			// 207
	101897,			// 208
	18405,			// 209
	21260,			// 210
	17818,			// 211
	18971,			// 212
	19317,			// 213
	19112,			// 214
	19395,			// 215
	20688,			// 216
	18438,			// 217
	18945,			// 218
	29309,			// 219
	19666,			// 220
	18735,			// 221
	87691,			// 222
	18478,			// 223
	22634,			// 224
	20984,			// 225
	20079,			// 226
	18624,			// 227
	20045,			// 228
	18369,			// 229
	19014,			// 230
	83179,			// 231
	20899,			// 232
	17854,			// 233
	19332,			// 234
	17875,			// 235
	28647,			// 236
	17465,			// 237
	20277,			// 238
	18994,			// 239
	22192,			// 240
	17443,			// 241
	20243,			// 242
	28174,			// 243
	134871,			// 244
	17753,			// 245
	18924,			// 246
	18281,			// 247
	18937,			// 248
	17419,			// 249
	20679,			// 250
	17865,			// 251
	17984,			// 252
	58615,			// 253
	35506,			// 254
	123499,			// 255
};
*/

// Q3 TA freq. table.
int msg_hData[256] = {
250315,			// 0
41193,			// 1
6292,			// 2
7106,			// 3
3730,			// 4
3750,			// 5
6110,			// 6
23283,			// 7
33317,			// 8
6950,			// 9
7838,			// 10
9714,			// 11
9257,			// 12
17259,			// 13
3949,			// 14
1778,			// 15
8288,			// 16
1604,			// 17
1590,			// 18
1663,			// 19
1100,			// 20
1213,			// 21
1238,			// 22
1134,			// 23
1749,			// 24
1059,			// 25
1246,			// 26
1149,			// 27
1273,			// 28
4486,			// 29
2805,			// 30
3472,			// 31
21819,			// 32
1159,			// 33
1670,			// 34
1066,			// 35
1043,			// 36
1012,			// 37
1053,			// 38
1070,			// 39
1726,			// 40
888,			// 41
1180,			// 42
850,			// 43
960,			// 44
780,			// 45
1752,			// 46
3296,			// 47
10630,			// 48
4514,			// 49
5881,			// 50
2685,			// 51
4650,			// 52
3837,			// 53
2093,			// 54
1867,			// 55
2584,			// 56
1949,			// 57
1972,			// 58
940,			// 59
1134,			// 60
1788,			// 61
1670,			// 62
1206,			// 63
5719,			// 64
6128,			// 65
7222,			// 66
6654,			// 67
3710,			// 68
3795,			// 69
1492,			// 70
1524,			// 71
2215,			// 72
1140,			// 73
1355,			// 74
971,			// 75
2180,			// 76
1248,			// 77
1328,			// 78
1195,			// 79
1770,			// 80
1078,			// 81
1264,			// 82
1266,			// 83
1168,			// 84
965,			// 85
1155,			// 86
1186,			// 87
1347,			// 88
1228,			// 89
1529,			// 90
1600,			// 91
2617,			// 92
2048,			// 93
2546,			// 94
3275,			// 95
2410,			// 96
3585,			// 97
2504,			// 98
2800,			// 99
2675,			// 100
6146,			// 101
3663,			// 102
2840,			// 103
14253,			// 104
3164,			// 105
2221,			// 106
1687,			// 107
3208,			// 108
2739,			// 109
3512,			// 110
4796,			// 111
4091,			// 112
3515,			// 113
5288,			// 114
4016,			// 115
7937,			// 116
6031,			// 117
5360,			// 118
3924,			// 119
4892,			// 120
3743,			// 121
4566,			// 122
4807,			// 123
5852,			// 124
6400,			// 125
6225,			// 126
8291,			// 127
23243,			// 128
7838,			// 129
7073,			// 130
8935,			// 131
5437,			// 132
4483,			// 133
3641,			// 134
5256,			// 135
5312,			// 136
5328,			// 137
5370,			// 138
3492,			// 139
2458,			// 140
1694,			// 141
1821,			// 142
2121,			// 143
1916,			// 144
1149,			// 145
1516,			// 146
1367,			// 147
1236,			// 148
1029,			// 149
1258,			// 150
1104,			// 151
1245,			// 152
1006,			// 153
1149,			// 154
1025,			// 155
1241,			// 156
952,			// 157
1287,			// 158
997,			// 159
1713,			// 160
1009,			// 161
1187,			// 162
879,			// 163
1099,			// 164
929,			// 165
1078,			// 166
951,			// 167
1656,			// 168
930,			// 169
1153,			// 170
1030,			// 171
1262,			// 172
1062,			// 173
1214,			// 174
1060,			// 175
1621,			// 176
930,			// 177
1106,			// 178
912,			// 179
1034,			// 180
892,			// 181
1158,			// 182
990,			// 183
1175,			// 184
850,			// 185
1121,			// 186
903,			// 187
1087,			// 188
920,			// 189
1144,			// 190
1056,			// 191
3462,			// 192
2240,			// 193
4397,			// 194
12136,			// 195
7758,			// 196
1345,			// 197
1307,			// 198
3278,			// 199
1950,			// 200
886,			// 201
1023,			// 202
1112,			// 203
1077,			// 204
1042,			// 205
1061,			// 206
1071,			// 207
1484,			// 208
1001,			// 209
1096,			// 210
915,			// 211
1052,			// 212
995,			// 213
1070,			// 214
876,			// 215
1111,			// 216
851,			// 217
1059,			// 218
805,			// 219
1112,			// 220
923,			// 221
1103,			// 222
817,			// 223
1899,			// 224
1872,			// 225
976,			// 226
841,			// 227
1127,			// 228
956,			// 229
1159,			// 230
950,			// 231
7791,			// 232
954,			// 233
1289,			// 234
933,			// 235
1127,			// 236
3207,			// 237
1020,			// 238
927,			// 239
1355,			// 240
768,			// 241
1040,			// 242
745,			// 243
952,			// 244
805,			// 245
1073,			// 246
740,			// 247
1013,			// 248
805,			// 249
1008,			// 250
796,			// 251
996,			// 252
1057,			// 253
11457,			// 254
13504,			// 255
};

#ifndef _USINGNEWHUFFTABLE_

void MSG_initHuffman() {
	int i,j;

#ifdef _NEWHUFFTABLE_
	fp=fopen("c:\\netchan.bin", "a");
#endif // _NEWHUFFTABLE_
	
	msgInit = qtrue;
	Huff_Init(&msgHuff);
	for(i=0;i<256;i++) {
		for (j=0;j<msg_hData[i];j++) {
			Huff_addRef(&msgHuff.compressor,	(byte)i);			// Do update
			Huff_addRef(&msgHuff.decompressor,	(byte)i);			// Do update
		}
	}
}

#else

void MSG_initHuffman() {

	byte	*data;
	int		size, i, ch;
	int		array[256];

	msgInit = qtrue;

	Huff_Init(&msgHuff);
	// load it in
	size = FS_ReadFile( "netchan\\netchan.bin", (void **)&data );

	for(i=0;i<256;i++) {
		array[i] = 0;
	}
	for(i=0;i<size;i++) {
		ch = data[i];
		Huff_addRef(&msgHuff.compressor,	ch);			// Do update
		Huff_addRef(&msgHuff.decompressor,	ch);			// Do update
		array[ch]++;
	}
	Com_Printf("msg_hData {\n");
	for(i=0;i<256;i++) {
		if (array[i] == 0) {
			Huff_addRef(&msgHuff.compressor,	i);			// Do update
			Huff_addRef(&msgHuff.decompressor,	i);			// Do update
		}
		Com_Printf("%d,			// %d\n", array[i], i);
	}
	Com_Printf("};\n");
	FS_FreeFile( data );
	Cbuf_AddText( "condump dump.txt\n" );
}

#endif _USINGNEWHUFFTABLE_

void MSG_shutdownHuffman()
{
#ifdef _NEWHUFFTABLE_
	if(fp)
	{
		fclose(fp);
	}
#endif // _NEWHUFFTABLE_
}

//===========================================================================
