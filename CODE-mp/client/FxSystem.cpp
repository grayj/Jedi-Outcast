#include "client.h"

#if !defined(FX_SCHEDULER_H_INC)
	#include "FxScheduler.h"
#endif

#if !defined(CG_CAMERA_H_INC)
//	#include "cg_camera.h"
#endif

cvar_t	fx_debug;
cvar_t	fx_freeze;

#define DEFAULT_EXPLOSION_RADIUS	512

// Stuff for the FxHelper
//------------------------------------------------------
SFxHelper::SFxHelper(void) :
	mTime(0),
	mOldTime(0),
	mFrameTime(0),
	mTimeFrozen(false)
{
}
 
void SFxHelper::ReInit(void)
{
	mTime = 0;
	mOldTime = 0;
	mFrameTime = 0;
	mTimeFrozen = false;
}

//------------------------------------------------------
void SFxHelper::Print( const char *msg, ... )
{
	va_list		argptr;
	char		text[1024];

	va_start( argptr, msg );
	vsprintf( text, msg, argptr );
	va_end( argptr );

	Com_DPrintf( text );
}

//------------------------------------------------------
void SFxHelper::AdjustTime_Pos( int time, vec3_t refdef_vieworg, vec3_t refdef_viewaxis[3] )
{
	if ( fx_freeze.integer )
	{
		mFrameTime = 0;
	}
	else
	{
		VectorCopy( refdef_vieworg, refdef.vieworg );
		VectorCopy( refdef_viewaxis[0], refdef.viewaxis[0] );
		VectorCopy( refdef_viewaxis[1], refdef.viewaxis[1] );
		VectorCopy( refdef_viewaxis[2], refdef.viewaxis[2] );
		mOldTime = mTime;
		mTime = time;
		mFrameTime = mTime - mOldTime;
	}
}

//------------------------------------------------------
void SFxHelper::CameraShake( vec3_t origin, float intensity, int radius, int time )
{
	vec3_t	dir;
	float	dist, intensityScale;
	float	realIntensity;

	VectorSubtract( refdef.vieworg, origin, dir );
	dist = VectorNormalize( dir );

	// Apparently the SoF2 camera shake function takes a time, but not a radius.
	//	when someone feels like fixing this, we can be a bit more flexible
	radius = DEFAULT_EXPLOSION_RADIUS;

	//Use the dir to add kick to the explosion
	if ( dist > radius )
		return;

	intensityScale = 1 - ( dist / (float) radius );
	realIntensity = intensity * intensityScale;

//rjr	theClientCamera.Shake( realIntensity, time );
}
