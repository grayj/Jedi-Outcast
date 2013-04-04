#ifdef EFFECTSED
	#include "../PseudoGameEngine.h"
#else
#endif

#ifndef FX_SYSTEM_H_INC
#define FX_SYSTEM_H_INC

#ifdef CHC
	#define irand	Q_irand
	#define flrand	Q_flrand
#endif //CHC

extern cvar_t	fx_debug;
extern cvar_t	fx_freeze;

inline void Vector2Clear(vec2_t a)
{
	a[0] = 0.0f;
	a[1] = 0.0f;
}

inline void Vector2Set(vec2_t a,float b,float c)
{
	a[0] = b;
	a[1] = c;
}

inline void Vector2Copy(vec2_t src,vec2_t dst)
{
	dst[0] = src[0];
	dst[1] = src[1];
}

inline void Vector2MA(vec2_t src, float m, vec2_t v, vec2_t dst)
{
	dst[0] = src[0] + (m*v[0]);
	dst[1] = src[1] + (m*v[1]);
}

inline void Vector2Scale(vec2_t src,float b,vec2_t dst)
{
	dst[0] = src[0] * b;
	dst[1] = src[1] * b;
}

#ifndef EFFECTSED // The editor won't need to see this
//extern void	CG_CalcEntityLerpPositions( centity_t * );	
#endif

class SFxHelper
{
public:
	int			mTime;
	int			mOldTime;
	int			mFrameTime;
	bool		mTimeFrozen;
	refdef_t	refdef;

#ifdef _DEBUG
	int			mMainRefs;
	int			mMiniRefs;
#endif

public:
	SFxHelper(void);

	inline	int	GetTime(void) { return mTime; }
	inline	int	GetFrameTime(void) { return mFrameTime; }

	void	ReInit(void);
	void	AdjustTime_Pos( int time, vec3_t refdef_vieworg, vec3_t refdef_viewaxis[3] );

	// These functions are wrapped and used by the fx system in case it makes things a bit more portable
	void	Print( const char *msg, ... );

	// File handling
	inline	int		OpenFile( const char *path, fileHandle_t *fh, int mode )
	{
		return FS_FOpenFileByMode( path, fh, FS_READ );
	}
	inline	int		ReadFile( void *data, int len, fileHandle_t fh )
	{
		FS_Read2( data, len, fh );
		return 1;
	}
	inline	void	CloseFile( fileHandle_t fh )
	{
		FS_FCloseFile( fh );
	}

	// Sound
	inline	void	PlaySound( vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfxHandle )
	{
		S_StartSound( origin, ENTITYNUM_NONE, CHAN_AUTO, sfxHandle );
	}
	inline	int		RegisterSound( const char *sound )
	{
		return S_RegisterSound( sound );
	}

	// Physics/collision
	inline	void	Trace( trace_t &tr, vec3_t start, vec3_t min, vec3_t max, vec3_t end, int skipEntNum, int flags )
	{
		//VM_Call( cgvm, CG_TRACE, tr, start, min, max, end, skipEntNum, flags );
		TCGTrace		*td = (TCGTrace *)cl.mSharedMemory;

		if ( !min )
			min = vec3_origin;

		if ( !max )
			max = vec3_origin;

		memset(td, sizeof(*td), 0);
		VectorCopy(start, td->mStart);
		VectorCopy(min, td->mMins);
		VectorCopy(max, td->mMaxs);
		VectorCopy(end, td->mEnd);
		td->mSkipNumber = skipEntNum;
		td->mMask = flags;

		VM_Call( cgvm, CG_TRACE );

		tr = td->mResult;
	}

	inline	void	AddFxToScene( refEntity_t *ent )
	{
#ifdef _DEBUG
		mMainRefs++;
#endif
		re.AddRefEntityToScene( ent );
	}
	inline	void	AddFxToScene( miniRefEntity_t *ent )
	{
#ifdef _DEBUG
		mMiniRefs++;
#endif
		re.AddMiniRefEntityToScene( ent );
	}
	inline	void	AddLightToScene( vec3_t org, float radius, float red, float green, float blue )
	{
		re.AddLightToScene(	org, radius, red, green, blue );
	}

	inline	int		RegisterShader( const char *shader )
	{
		return re.RegisterShader( shader );
	}
	inline	int		RegisterModel( const char *model )
	{
		return re.RegisterModel( model );
	}

	inline	void	AddPolyToScene( int shader, int count, polyVert_t *verts )
	{
		re.AddPolyToScene( shader, count, verts, 1 );
	}

	void	CameraShake( vec3_t origin, float intensity, int radius, int time );
};

extern SFxHelper	theFxHelper;

#endif // FX_SYSTEM_H_INC
