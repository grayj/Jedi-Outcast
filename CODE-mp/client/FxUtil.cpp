#include "client.h"
//#include "..\smartheap\smrtheap.h"
#if !defined(FX_SCHEDULER_H_INC)
	#include "FxScheduler.h"
#endif

#ifdef EFFECTSED
	#include "../EffectsCoreInterface.h"
#endif

#include <set>

set<CCloud *> OutstandClouds;

vec3_t	WHITE = {1.0f, 1.0f, 1.0f};

struct SEffectList
{
	CEffect *mEffect;
};

#define PI		3.14159f

SEffectList		effectList[MAX_EFFECTS];
SEffectList		*nextValidEffect;
SFxHelper		theFxHelper;

int				activeFx = 0;
int				drawnFx;
qboolean		fxInitialized = qfalse;	

//-------------------------
// FX_Free
//
// Frees all FX
//-------------------------
bool FX_Free( void )
{
	theFxScheduler.Clean();

	for ( int i = 0; i < MAX_EFFECTS; i++ )
	{
		if ( effectList[i].mEffect )
		{
			delete effectList[i].mEffect;
		}

		effectList[i].mEffect = 0;
	}

	activeFx = 0;

	return true;
}

//-------------------------
// FX_Stop
//
// Frees all active FX but leaves the templates
//-------------------------
void FX_Stop( void )
{
	theFxScheduler.Clean(false);

	for ( int i = 0; i < MAX_EFFECTS; i++ )
	{
		if ( effectList[i].mEffect )
		{
			delete effectList[i].mEffect;
		}

		effectList[i].mEffect = 0;
	}

	activeFx = 0;
}

//-------------------------
// FX_Init
//
// Preps system for use
//-------------------------
int	FX_Init( void )
{ 
	if ( fxInitialized == qfalse )
	{
		fxInitialized = qtrue;

		for ( int i = 0; i < MAX_EFFECTS; i++ )
		{
			effectList[i].mEffect = 0;
		}
	}

	FX_Free();

	nextValidEffect = &effectList[0];
	theFxHelper.ReInit();

	return true;
}


//-------------------------
// FX_FreeMember
//-------------------------
void FX_FreeMember( SEffectList *obj )
{
	obj->mEffect->Die();
	delete obj->mEffect;
	obj->mEffect = 0;

	// May as well mark this to be used next
	nextValidEffect = obj;

	activeFx--;
}


//-------------------------
// FX_GetValidEffect
//
// Finds an unused effect slot
//
// Note - in the editor, this function may return NULL, indicating that all
// effects are being stopped.
//-------------------------
static SEffectList *FX_GetValidEffect()
{
	if ( nextValidEffect->mEffect == 0 )
	{
		return nextValidEffect;
	}

	int			i;
	SEffectList	*ef;

	// Blah..plow through the list till we find something that is currently untainted
	for ( i = 0, ef = effectList; i < MAX_EFFECTS; i++, ef++ )
	{
		if ( ef->mEffect == 0 )
		{
			return ef;
		}
	}

	// report the error.

#ifdef EFFECTSED
	// After we generate three of these messages in rapid succession, kill the effect.
	static DWORD s_dwLastErrorTicks = 0;
	static int s_iErrors = 0;

	if (theFxScheduler.IsStopScheduled())
	{
		return NULL;
	}
	else if ((GetTickCount() - s_dwLastErrorTicks) < 50)
	{
		if (++s_iErrors >= 3)
		{
			theFxScheduler.ScheduleStop();
			s_iErrors = 0;
			return NULL;
		}
	}
	else
	{
		s_iErrors = 0;
	}
#endif

	theFxHelper.Print( "FX system out of effects\n" );

#ifdef EFFECTSED
	s_dwLastErrorTicks = GetTickCount();
#endif

	// Hmmm.. just trashing the first effect in the list is a poor approach
	FX_FreeMember( &effectList[0] );

	// Recursive call
	return FX_GetValidEffect();
}


//-------------------------
// FX_ActiveFx
//
// Returns whether these are any active or scheduled effects
//-------------------------
bool FX_ActiveFx(void)
{
	return ((activeFx > 0) || (theFxScheduler.NumScheduledFx() > 0));
}


//-------------------------
// FX_Add
//
// Adds all fx to the view
//-------------------------
void FX_Add( void )
{
	int			i;
	SEffectList	*ef;

	drawnFx = 0;

#ifdef _DEBUG
	theFxHelper.mMainRefs = theFxHelper.mMiniRefs = 0;
#endif

	// Blah....plow through the whole dang list.
	for ( i = 0, ef = effectList; i < MAX_EFFECTS; i++, ef++ )
	{
		if ( ef->mEffect != 0 )
		{ 
			// Effect is active
			if ( theFxHelper.mTime > ef->mEffect->GetKillTime() )
			{ 
				// Clean up old effects, calling any death effects as needed
				// this flag just has to be cleared otherwise death effects might not happen correctly
				ef->mEffect->ClearFlags( FX_KILL_ON_IMPACT ); 
				FX_FreeMember( ef );
			}
			else
			{
				if ( theFxHelper.mFrameTime > 0 )
				{ 
					// time and the fx system aren't paused
					if ( ef->mEffect->Update() == false )
					{
						// We've been marked for death
						FX_FreeMember( ef );
						continue;
					}
				}
		
				if ( ef->mEffect->Cull() == false )
				{ 
					drawnFx++;

					// presumably visible so draw the effect
					ef->mEffect->Draw();
				}
			}
		}
	}

#ifdef _DEBUG
/*	if (theFxHelper.mMainRefs || theFxHelper.mMiniRefs)
	{
		cgi.OPrintf("Main Refs: %d    Mini Refs: %d\n", theFxHelper.mMainRefs, theFxHelper.mMiniRefs);
	}*/
#endif

	if ( fx_debug.integer )
	{
		theFxHelper.Print( "Active    FX: %i\n", activeFx );
		theFxHelper.Print( "Drawn     FX: %i\n", drawnFx );
		theFxHelper.Print( "Scheduled FX: %i\n", theFxScheduler.NumScheduledFx() );
	}

#ifdef EFFECTSED
	SetEffectCounters(activeFx, theFxScheduler.NumScheduledFx(), drawnFx);
#endif
}


//-------------------------
// FX_AddPrimitive
//
// Note - in the editor, this function may change *pEffect to NULL, indicating that
// all effects are being stopped.
//-------------------------
void FX_AddPrimitive( CEffect **pEffect, CCloud *effectCloud, int killTime )
{
	if (!effectCloud)
	{
		SEffectList *item = FX_GetValidEffect();
#ifdef EFFECTSED
		if (item == NULL)
		{
			// This means we are killing all effects.  This should only happen in the editor.
			delete *pEffect;
			*pEffect = NULL;
			return;
		}
#endif
		item->mEffect = *pEffect;
	}
	else
	{
/*		dbgMemCheckAll();
		DBGMEM_PTR_INFO dbg1;
		if(dbgMemPtrInfo((void *)effectCloud,&dbg1))
		{
			assert(dbg1.isInUse);
		}
		DBGMEM_PTR_INFO dbg2;
		if(dbgMemPtrInfo((void *)*pEffect,&dbg2))
		{
			assert(dbg2.isInUse);
		}
*/
		if (effectCloud&&OutstandClouds.find(effectCloud)!=OutstandClouds.end())
		{
			effectCloud->AddEffect(*pEffect);
		}
//		dbgMemCheckAll();
	}
	(*pEffect)->SetKillTime(theFxHelper.mTime + killTime);
	activeFx++;

	// Stash these in the primitive so it has easy access to the vals
	(*pEffect)->SetTimeStart( theFxHelper.mTime );
	(*pEffect)->SetTimeEnd( theFxHelper.mTime + killTime );
}

CCloud	*FX_AddCloud(void)
{
	CCloud		*cloud;

	cloud = new CCloud;
	OutstandClouds.insert(cloud);
	FX_AddPrimitive((CEffect **)&cloud, 0, 99999);

	return cloud;
}


//-------------------------
//  FX_AddParticle
//-------------------------
CParticle *FX_AddParticle( CCloud *effectCloud, vec3_t org, vec3_t vel, vec3_t accel, float size1, float size2, float sizeParm, 
							float alpha1, float alpha2, float alphaParm, 
							vec3_t sRGB, vec3_t eRGB, float rgbParm,
							float rotation, float rotationDelta,
							vec3_t min, vec3_t max, float elasticity,
							int deathID, int impactID,
							int killTime, qhandle_t shader, int flags = 0 )
{
	if ( theFxHelper.mFrameTime < 1 )
	{ // disallow adding effects when the system is paused
		return 0;
	}

	CParticle *fx = new CParticle;

	if ( fx )
	{
		fx->SetOrigin1( org );
		fx->SetVel( vel );
		fx->SetAccel( accel );

		// RGB----------------
		fx->SetRGBStart( sRGB );
		fx->SetRGBEnd( eRGB );

		if (( flags & FX_RGB_PARM_MASK ) == FX_RGB_WAVE )
		{
			fx->SetRGBParm( rgbParm * PI * 0.001f );
		}
		else if ( flags & FX_RGB_PARM_MASK )
		{
			// rgbParm should be a value from 0-100..
			fx->SetRGBParm( rgbParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Alpha----------------
		fx->SetAlphaStart( alpha1 );
		fx->SetAlphaEnd( alpha2 );

		if (( flags & FX_ALPHA_PARM_MASK ) == FX_ALPHA_WAVE )
		{
			fx->SetAlphaParm( alphaParm * PI * 0.001f );
		}
		else if ( flags & FX_ALPHA_PARM_MASK )
		{
			fx->SetAlphaParm( alphaParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Size----------------
		fx->SetSizeStart( size1 );
		fx->SetSizeEnd( size2 );

		if (( flags & FX_SIZE_PARM_MASK ) == FX_SIZE_WAVE )
		{
			fx->SetSizeParm( sizeParm * PI * 0.001f );
		}
		else if ( flags & FX_SIZE_PARM_MASK )
		{
			fx->SetSizeParm( sizeParm * 0.01f * killTime + theFxHelper.mTime );
		}

		fx->SetFlags( flags );
		fx->SetShader( shader );
		fx->SetRotation( rotation );
		fx->SetRotationDelta( rotationDelta );
		fx->SetElasticity( elasticity );
		fx->SetMin( min );
		fx->SetMax( max );
		fx->SetDeathFxID( deathID );
		fx->SetImpactFxID( impactID );

		FX_AddPrimitive( (CEffect**)&fx, effectCloud, killTime );
		// in the editor, fx may now be NULL
	}

	return fx;
}

#ifndef EFFECTSED
#ifndef CHC // Sof2 only
//-------------------------
//  FX_AddParticle
//-------------------------
CParticle *FX_AddParticle( CCloud *effectCloud, CFxBoltInterface *obj, vec3_t vel, vec3_t accel,
							float size1, float size2, float sizeParm,
							float alpha1, float alpha2, float alphaParm,
							vec3_t rgb1, vec3_t rgb2, float rgbParm,
							float rotation, float rotationDelta,
							int killTime, qhandle_t shader, int flags = 0, bool objCopy = false )
{
	if ( theFxHelper.mFrameTime < 1 )
	{ // disallow adding effects when the system is paused
		return 0;
	}

	CParticle *fx = new CParticle;

	if ( fx )
	{
		vec3_t initOrg;

		obj->GetInitialPartOrg(initOrg);

		fx->SetOrigin1( initOrg );
		fx->SetVel( vel );
		fx->SetAccel( accel );

		// RGB----------------
		fx->SetRGBStart( rgb1 );
		fx->SetRGBEnd( rgb2 );

		if (( flags & FX_RGB_PARM_MASK ) == FX_RGB_WAVE )
		{
			fx->SetRGBParm( rgbParm * PI * 0.001f );
		}
		else if ( flags & FX_RGB_PARM_MASK )
		{
			// rgbParm should be a value from 0-100..
			fx->SetRGBParm( rgbParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Alpha----------------
		fx->SetAlphaStart( alpha1 );
		fx->SetAlphaEnd( alpha2 );

		if (( flags & FX_ALPHA_PARM_MASK ) == FX_ALPHA_WAVE )
		{
			fx->SetAlphaParm( alphaParm * PI * 0.001f );
		}
		else if ( flags & FX_ALPHA_PARM_MASK )
		{
			fx->SetAlphaParm( alphaParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Size----------------
		fx->SetSizeStart( size1 );
		fx->SetSizeEnd( size2 );

		if (( flags & FX_SIZE_PARM_MASK ) == FX_SIZE_WAVE )
		{
			fx->SetSizeParm( sizeParm * PI * 0.001f );
		}
		else if ( flags & FX_SIZE_PARM_MASK )
		{
			fx->SetSizeParm( sizeParm * 0.01f * killTime + theFxHelper.mTime );
		}

		fx->SetFlags( flags );
		fx->SetShader( shader );
		fx->SetRotation( rotation );
		fx->SetRotationDelta( rotationDelta );
		fx->SetElasticity( 0.0f );
		fx->SetMin( NULL );
		fx->SetMax( NULL );
		fx->SetObj( obj );

		if (objCopy)
		{
			fx->SetObjNew( obj );
		}

		FX_AddPrimitive( (CEffect**)&fx, effectCloud, killTime );
	}

	return fx;
}
#endif
#endif

//-------------------------
//  FX_AddLine
//-------------------------
CLine *FX_AddLine( CCloud *effectCloud, vec3_t start, vec3_t end, float size1, float size2, float sizeParm,
									float alpha1, float alpha2, float alphaParm,
									vec3_t sRGB, vec3_t eRGB, float rgbParm,
									int killTime, qhandle_t shader, int flags = 0 )
{
	if ( theFxHelper.mFrameTime < 1 )
	{ // disallow adding new effects when the system is paused
		return 0;
	}

	CLine *fx = new CLine;

	if ( fx )
	{
		fx->SetOrigin1( start );
		fx->SetOrigin2( end );

		// RGB----------------
		fx->SetRGBStart( sRGB );
		fx->SetRGBEnd( eRGB );

		if (( flags & FX_RGB_PARM_MASK ) == FX_RGB_WAVE )
		{
			fx->SetRGBParm( rgbParm * PI * 0.001f );
		}
		else if ( flags & FX_RGB_PARM_MASK )
		{
			// rgbParm should be a value from 0-100..
			fx->SetRGBParm( rgbParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Alpha----------------
		fx->SetAlphaStart( alpha1 );
		fx->SetAlphaEnd( alpha2 );

		if (( flags & FX_ALPHA_PARM_MASK ) == FX_ALPHA_WAVE )
		{
			fx->SetAlphaParm( alphaParm * PI * 0.001f );
		}
		else if ( flags & FX_ALPHA_PARM_MASK )
		{
			fx->SetAlphaParm( alphaParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Size----------------
		fx->SetSizeStart( size1 );
		fx->SetSizeEnd( size2 );

		if (( flags & FX_SIZE_PARM_MASK ) == FX_SIZE_WAVE )
		{
			fx->SetSizeParm( sizeParm * PI * 0.001f );
		}
		else if ( flags & FX_SIZE_PARM_MASK )
		{
			fx->SetSizeParm( sizeParm * 0.01f * killTime + theFxHelper.mTime );
		}

		fx->SetShader( shader );
		fx->SetFlags( flags );

		fx->SetSTScale( 1.0f, 1.0f );

		FX_AddPrimitive( (CEffect**)&fx, effectCloud, killTime );
		// in the editor, fx may now be NULL
	}

	return fx;
}


//-------------------------
//  FX_AddElectricity
//-------------------------
CElectricity *FX_AddElectricity( CCloud *effectCloud, vec3_t start, vec3_t end, float size1, float size2, float sizeParm,
									float alpha1, float alpha2, float alphaParm,
									vec3_t sRGB, vec3_t eRGB, float rgbParm,
									float chaos, int killTime, qhandle_t shader, int flags = 0 )
{
	if ( theFxHelper.mFrameTime < 1 )
	{ // disallow adding new effects when the system is paused
		return 0;
	}

	CElectricity *fx = new CElectricity;

	if ( fx )
	{
		fx->SetOrigin1( start );
		fx->SetOrigin2( end );

		// RGB----------------
		fx->SetRGBStart( sRGB );
		fx->SetRGBEnd( eRGB );

		if (( flags & FX_RGB_PARM_MASK ) == FX_RGB_WAVE )
		{
			fx->SetRGBParm( rgbParm * PI * 0.001f );
		}
		else if ( flags & FX_RGB_PARM_MASK )
		{
			// rgbParm should be a value from 0-100..
			fx->SetRGBParm( rgbParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Alpha----------------
		fx->SetAlphaStart( alpha1 );
		fx->SetAlphaEnd( alpha2 );

		if (( flags & FX_ALPHA_PARM_MASK ) == FX_ALPHA_WAVE )
		{
			fx->SetAlphaParm( alphaParm * PI * 0.001f );
		}
		else if ( flags & FX_ALPHA_PARM_MASK )
		{
			fx->SetAlphaParm( alphaParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Size----------------
		fx->SetSizeStart( size1 );
		fx->SetSizeEnd( size2 );

		if (( flags & FX_SIZE_PARM_MASK ) == FX_SIZE_WAVE )
		{
			fx->SetSizeParm( sizeParm * PI * 0.001f );
		}
		else if ( flags & FX_SIZE_PARM_MASK )
		{
			fx->SetSizeParm( sizeParm * 0.01f * killTime + theFxHelper.mTime );
		}

		fx->SetShader( shader );
		fx->SetFlags( flags );
		fx->SetChaos( chaos );

		fx->SetSTScale( 1.0f, 1.0f );

		FX_AddPrimitive( (CEffect**)&fx, effectCloud, killTime );
		// in the editor, fx may now be NULL
		if ( fx )
		{
			fx->Initialize(); 
		}
	}

	return fx;
}


//-------------------------
//  FX_AddTail
//-------------------------
CTail *FX_AddTail( CCloud *effectCloud, vec3_t org, vec3_t vel, vec3_t accel, 
							float size1, float size2, float sizeParm, 
							float length1, float length2, float lengthParm,
							float alpha1, float alpha2, float alphaParm,
							vec3_t sRGB, vec3_t eRGB, float rgbParm,
							vec3_t min, vec3_t max, float elasticity, 
							int deathID, int impactID,
							int killTime, qhandle_t shader, int flags = 0 )
{
	if ( theFxHelper.mFrameTime < 1 )
	{ // disallow adding effects when the system is paused
		return 0;
	}

	CTail *fx = new CTail;

	if ( fx )
	{
		fx->SetOrigin1( org );
		fx->SetVel( vel );
		fx->SetAccel( accel );

		// RGB----------------
		fx->SetRGBStart( sRGB );
		fx->SetRGBEnd( eRGB );

		if (( flags & FX_RGB_PARM_MASK ) == FX_RGB_WAVE )
		{
			fx->SetRGBParm( rgbParm * PI * 0.001f );
		}
		else if ( flags & FX_RGB_PARM_MASK )
		{
			// rgbParm should be a value from 0-100..
			fx->SetRGBParm( rgbParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Alpha----------------
		fx->SetAlphaStart( alpha1 );
		fx->SetAlphaEnd( alpha2 );

		if (( flags & FX_ALPHA_PARM_MASK ) == FX_ALPHA_WAVE )
		{
			fx->SetAlphaParm( alphaParm * PI * 0.001f );
		}
		else if ( flags & FX_ALPHA_PARM_MASK )
		{
			fx->SetAlphaParm( alphaParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Size----------------
		fx->SetSizeStart( size1 );
		fx->SetSizeEnd( size2 );

		if (( flags & FX_SIZE_PARM_MASK ) == FX_SIZE_WAVE )
		{
			fx->SetSizeParm( sizeParm * PI * 0.001f );
		}
		else if ( flags & FX_SIZE_PARM_MASK )
		{
			fx->SetSizeParm( sizeParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Length----------------
		fx->SetLengthStart( length1 );
		fx->SetLengthEnd( length2 );

		if (( flags & FX_LENGTH_PARM_MASK ) == FX_LENGTH_WAVE )
		{
			fx->SetLengthParm( lengthParm * PI * 0.001f );
		}
		else if ( flags & FX_LENGTH_PARM_MASK )
		{
			fx->SetLengthParm( lengthParm * 0.01f * killTime + theFxHelper.mTime );
		}

		fx->SetFlags( flags );
		fx->SetShader( shader );
		fx->SetElasticity( elasticity );
		fx->SetMin( min );
		fx->SetMax( max );
		fx->SetSTScale( 1.0f, 1.0f );
		fx->SetDeathFxID( deathID );
		fx->SetImpactFxID( impactID );

		FX_AddPrimitive( (CEffect**)&fx, effectCloud, killTime );
		// in the editor, fx may now be NULL
	}

	return fx;
}


//-------------------------
//  FX_AddCylinder
//-------------------------
CCylinder *FX_AddCylinder( CCloud *effectCloud, vec3_t start, vec3_t normal, 
							float size1s, float size1e, float size1Parm,
							float size2s, float size2e, float size2Parm,
							float length1, float length2, float lengthParm,
							float alpha1, float alpha2, float alphaParm,
							vec3_t rgb1, vec3_t rgb2, float rgbParm,
							int killTime, qhandle_t shader, int flags )
{
	if ( theFxHelper.mFrameTime < 1 )
	{ // disallow adding new effects when the system is paused
		return 0;
	}

	CCylinder *fx = new CCylinder;

	if ( fx )
	{
		fx->SetOrigin1( start );
		fx->SetNormal( normal );

		// RGB----------------
		fx->SetRGBStart( rgb1 );
		fx->SetRGBEnd( rgb2 );

		if (( flags & FX_RGB_PARM_MASK ) == FX_RGB_WAVE )
		{
			fx->SetRGBParm( rgbParm * PI * 0.001f );
		}
		else if ( flags & FX_RGB_PARM_MASK )
		{
			// rgbParm should be a value from 0-100..
			fx->SetRGBParm( rgbParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Size1----------------
		fx->SetSizeStart( size1s );
		fx->SetSizeEnd( size1e );

		if (( flags & FX_SIZE_PARM_MASK ) == FX_SIZE_WAVE )
		{
			fx->SetSizeParm( size1Parm * PI * 0.001f );
		}
		else if ( flags & FX_SIZE_PARM_MASK )
		{
			fx->SetSizeParm( size1Parm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Size2----------------
		fx->SetSize2Start( size2s );
		fx->SetSize2End( size2e );

		if (( flags & FX_SIZE2_PARM_MASK ) == FX_SIZE2_WAVE )
		{
			fx->SetSize2Parm( size2Parm * PI * 0.001f );
		}
		else if ( flags & FX_SIZE2_PARM_MASK )
		{
			fx->SetSize2Parm( size2Parm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Length1---------------
		fx->SetLengthStart( length1 );
		fx->SetLengthEnd( length2 );

		if (( flags & FX_LENGTH_PARM_MASK ) == FX_LENGTH_WAVE )
		{
			fx->SetLengthParm( lengthParm * PI * 0.001f );
		}
		else if ( flags & FX_LENGTH_PARM_MASK )
		{
			fx->SetLengthParm( lengthParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Alpha----------------
		fx->SetAlphaStart( alpha1 );
		fx->SetAlphaEnd( alpha2 );

		if (( flags & FX_ALPHA_PARM_MASK ) == FX_ALPHA_WAVE )
		{
			fx->SetAlphaParm( alphaParm * PI * 0.001f );
		}
		else if ( flags & FX_ALPHA_PARM_MASK )
		{
			fx->SetAlphaParm( alphaParm * 0.01f * killTime + theFxHelper.mTime );
		}

		fx->SetShader( shader );
		fx->SetFlags( flags );

		FX_AddPrimitive( (CEffect**)&fx, effectCloud, killTime );
	}

	return fx;
}

//-------------------------
//  FX_AddEmitter
//-------------------------
CEmitter *FX_AddEmitter( CCloud *effectCloud, vec3_t org, vec3_t vel, vec3_t accel, 
								float size1, float size2, float sizeParm,
								float alpha1, float alpha2, float alphaParm,
								vec3_t rgb1, vec3_t rgb2, float rgbParm,
								vec3_t angs, vec3_t deltaAngs,
								vec3_t min, vec3_t max, float elasticity, 
								int deathID, int impactID, int emitterID,
								float density, float variance,
								int killTime, qhandle_t model, int flags = 0 )
{
	if ( theFxHelper.mFrameTime < 1 )
	{ // disallow adding effects when the system is paused
		return 0;
	}

	CEmitter *fx = new CEmitter;

	if ( fx )
	{
		fx->SetOrigin1( org );
		fx->SetVel( vel );
		fx->SetAccel( accel );

		// RGB----------------
		fx->SetRGBStart( rgb1 );
		fx->SetRGBEnd( rgb2 );

		if (( flags & FX_RGB_PARM_MASK ) == FX_RGB_WAVE )
		{
			fx->SetRGBParm( rgbParm * PI * 0.001f );
		}
		else if ( flags & FX_RGB_PARM_MASK )
		{
			// rgbParm should be a value from 0-100..
			fx->SetRGBParm( rgbParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Size----------------
		fx->SetSizeStart( size1 );
		fx->SetSizeEnd( size2 );

		if (( flags & FX_SIZE_PARM_MASK ) == FX_SIZE_WAVE )
		{
			fx->SetSizeParm( sizeParm * PI * 0.001f );
		}
		else if ( flags & FX_SIZE_PARM_MASK )
		{
			fx->SetSizeParm( sizeParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Alpha----------------
		fx->SetAlphaStart( alpha1 );
		fx->SetAlphaEnd( alpha2 );

		if (( flags & FX_ALPHA_PARM_MASK ) == FX_ALPHA_WAVE )
		{
			fx->SetAlphaParm( alphaParm * PI * 0.001f );
		}
		else if ( flags & FX_ALPHA_PARM_MASK )
		{
			fx->SetAlphaParm( alphaParm * 0.01f * killTime + theFxHelper.mTime );
		}

		fx->SetAngles( angs );
		fx->SetAngleDelta( deltaAngs );
		fx->SetFlags( flags );
		fx->SetModel( model );
		fx->SetElasticity( elasticity );
		fx->SetMin( min );
		fx->SetMax( max );
		fx->SetDeathFxID( deathID );
		fx->SetImpactFxID( impactID );
		fx->SetEmitterFxID( emitterID );
		fx->SetDensity( density );
		fx->SetVariance( variance );
		fx->SetOldTime( theFxHelper.mTime );

		fx->SetLastOrg( org );
		fx->SetLastVel( vel );

		FX_AddPrimitive( (CEffect**)&fx, 0, killTime );
		// in the editor, fx may now be NULL
	}

	return fx;
}

//-------------------------
//  FX_AddLight
//-------------------------
CLight *FX_AddLight( CCloud *effectCloud, vec3_t org, float size1, float size2, float sizeParm,
							vec3_t rgb1, vec3_t rgb2, float rgbParm,
							int killTime, int flags = 0 )
{
	if ( theFxHelper.mFrameTime < 1 )
	{ // disallow adding effects when the system is paused
		return 0;
	}

	CLight *fx = new CLight;

	if ( fx )
	{
		fx->SetOrigin1( org );

		// RGB----------------
		fx->SetRGBStart( rgb1 );
		fx->SetRGBEnd( rgb2 );

		if (( flags & FX_RGB_PARM_MASK ) == FX_RGB_WAVE )
		{
			fx->SetRGBParm( rgbParm * PI * 0.001f );
		}
		else if ( flags & FX_RGB_PARM_MASK )
		{
			// rgbParm should be a value from 0-100..
			fx->SetRGBParm( rgbParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Size----------------
		fx->SetSizeStart( size1 );
		fx->SetSizeEnd( size2 );

		if (( flags & FX_SIZE_PARM_MASK ) == FX_SIZE_WAVE )
		{
			fx->SetSizeParm( sizeParm * PI * 0.001f );
		}
		else if ( flags & FX_SIZE_PARM_MASK )
		{
			fx->SetSizeParm( sizeParm * 0.01f * killTime + theFxHelper.mTime );
		}

		fx->SetFlags( flags );

		FX_AddPrimitive( (CEffect**)&fx, 0, killTime );
		// in the editor, fx may now be NULL
	}

	return fx;

}


//-------------------------
//  FX_AddOrientedParticle
//-------------------------
COrientedParticle *FX_AddOrientedParticle( CCloud *effectCloud, vec3_t org, vec3_t norm, vec3_t vel, vec3_t accel,
						float size1, float size2, float sizeParm,
						float alpha1, float alpha2, float alphaParm,
						vec3_t rgb1, vec3_t rgb2, float rgbParm,
						float rotation, float rotationDelta,
						vec3_t min, vec3_t max, float bounce,
						int deathID, int impactID,
						int killTime, qhandle_t shader, int flags = 0 )
{
	if ( theFxHelper.mFrameTime < 1 )
	{ // disallow adding effects when the system is paused
		return 0;
	}

	COrientedParticle *fx = new COrientedParticle;

	if ( fx )
	{
		fx->SetOrigin1( org );
		fx->SetNormal( norm );
		fx->SetVel( vel );
		fx->SetAccel( accel );

		// RGB----------------
		fx->SetRGBStart( rgb1 );
		fx->SetRGBEnd( rgb2 );

		if (( flags & FX_RGB_PARM_MASK ) == FX_RGB_WAVE )
		{
			fx->SetRGBParm( rgbParm * PI * 0.001f );
		}
		else if ( flags & FX_RGB_PARM_MASK )
		{
			// rgbParm should be a value from 0-100..
			fx->SetRGBParm( rgbParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Alpha----------------
		fx->SetAlphaStart( alpha1 );
		fx->SetAlphaEnd( alpha2 );

		if (( flags & FX_ALPHA_PARM_MASK ) == FX_ALPHA_WAVE )
		{
			fx->SetAlphaParm( alphaParm * PI * 0.001f );
		}
		else if ( flags & FX_ALPHA_PARM_MASK )
		{
			fx->SetAlphaParm( alphaParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Size----------------
		fx->SetSizeStart( size1 );
		fx->SetSizeEnd( size2 );

		if (( flags & FX_SIZE_PARM_MASK ) == FX_SIZE_WAVE )
		{
			fx->SetSizeParm( sizeParm * PI * 0.001f );
		}
		else if ( flags & FX_SIZE_PARM_MASK )
		{
			fx->SetSizeParm( sizeParm * 0.01f * killTime + theFxHelper.mTime );
		}

		fx->SetFlags( flags );
		fx->SetShader( shader );
		fx->SetRotation( rotation );
		fx->SetRotationDelta( rotationDelta );
		fx->SetElasticity( bounce );
		fx->SetMin( min );
		fx->SetMax( max );
		fx->SetDeathFxID( deathID );
		fx->SetImpactFxID( impactID );

		FX_AddPrimitive( (CEffect**)&fx, effectCloud, killTime );
		// in the editor, fx may now be NULL
	}

	return fx;
}


//-------------------------
//  FX_AddPoly
//-------------------------
CPoly *FX_AddPoly( CCloud *effectCloud, vec3_t *verts, vec2_t *st, int numVerts, 
							vec3_t vel, vec3_t accel,
							float alpha1, float alpha2, float alphaParm,
							vec3_t rgb1, vec3_t rgb2, float rgbParm,
							vec3_t rotationDelta, float bounce, int motionDelay,
							int killTime, qhandle_t shader, int flags )
{
	if ( theFxHelper.mFrameTime < 1 || !verts )
	{ // disallow adding effects when the system is paused or the user doesn't pass in a vert array
		return 0;
	}

	CPoly *fx = new CPoly;

	if ( fx )
	{
		// Do a cheesy copy of the verts and texture coords into our own structure
		for ( int i = 0; i < numVerts; i++ )
		{
			VectorCopy( verts[i], fx->mOrg[i] );
			Vector2Copy( st[i], fx->mST[i] );
		}

		fx->SetVel( vel );
		fx->SetAccel( accel );

		// RGB----------------
		fx->SetRGBStart( rgb1 );
		fx->SetRGBEnd( rgb2 );

		if (( flags & FX_RGB_PARM_MASK ) == FX_RGB_WAVE )
		{
			fx->SetRGBParm( rgbParm * PI * 0.001f );
		}
		else if ( flags & FX_RGB_PARM_MASK )
		{
			// rgbParm should be a value from 0-100..
			fx->SetRGBParm( rgbParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Alpha----------------
		fx->SetAlphaStart( alpha1 );
		fx->SetAlphaEnd( alpha2 );

		if (( flags & FX_ALPHA_PARM_MASK ) == FX_ALPHA_WAVE )
		{
			fx->SetAlphaParm( alphaParm * PI * 0.001f );
		}
		else if ( flags & FX_ALPHA_PARM_MASK )
		{
			fx->SetAlphaParm( alphaParm * 0.01f * killTime + theFxHelper.mTime );
		}

		fx->SetFlags( flags );
		fx->SetShader( shader );
		fx->SetRot( rotationDelta );
		fx->SetElasticity( bounce );
		fx->SetMotionTimeStamp( motionDelay );
		fx->SetNumVerts( numVerts );

		// Now that we've set our data up, let's process it into a useful format
		fx->PolyInit();

		FX_AddPrimitive( (CEffect**)&fx, effectCloud, killTime );
		// in the editor, fx may now be NULL
	}

	return fx;
}

//-------------------------
//  FX_AddBezier
//-------------------------
CBezier *FX_AddBezier( vec3_t start, vec3_t end, 
								vec3_t control1, vec3_t control1Vel,
								vec3_t control2, vec3_t control2Vel,
								float size1, float size2, float sizeParm,
								float alpha1, float alpha2, float alphaParm,
								vec3_t sRGB, vec3_t eRGB, float rgbParm,
								int killTime, qhandle_t shader, int flags )
{
	if ( theFxHelper.mFrameTime < 1 )
	{ // disallow adding new effects when the system is paused
		return 0;
	}

	CBezier *fx = new CBezier;

	if ( fx )
	{
		fx->SetOrigin1( start );
		fx->SetOrigin2( end );

		fx->SetControlPoints( control1, control2 );
		fx->SetControlVel( control1Vel, control2Vel );

		// RGB----------------
		fx->SetRGBStart( sRGB );
		fx->SetRGBEnd( eRGB );

		if (( flags & FX_RGB_PARM_MASK ) == FX_RGB_WAVE )
		{
			fx->SetRGBParm( rgbParm * PI * 0.001f );
		}
		else if ( flags & FX_RGB_PARM_MASK )
		{
			// rgbParm should be a value from 0-100..
			fx->SetRGBParm( rgbParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Alpha----------------
		fx->SetAlphaStart( alpha1 );
		fx->SetAlphaEnd( alpha2 );

		if (( flags & FX_ALPHA_PARM_MASK ) == FX_ALPHA_WAVE )
		{
			fx->SetAlphaParm( alphaParm * PI * 0.001f );
		}
		else if ( flags & FX_ALPHA_PARM_MASK )
		{
			fx->SetAlphaParm( alphaParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Size----------------
		fx->SetSizeStart( size1 );
		fx->SetSizeEnd( size2 );

		if (( flags & FX_SIZE_PARM_MASK ) == FX_SIZE_WAVE )
		{
			fx->SetSizeParm( sizeParm * PI * 0.001f );
		}
		else if ( flags & FX_SIZE_PARM_MASK )
		{
			fx->SetSizeParm( sizeParm * 0.01f * killTime + theFxHelper.mTime );
		}

		fx->SetShader( shader );
		fx->SetFlags( flags );

		fx->SetSTScale( 1.0f, 1.0f );

		FX_AddPrimitive( (CEffect**)&fx, NULL, killTime );
	}

	return fx;
}

//-------------------------
//  FX_AddFlash
//-------------------------
CFlash *FX_AddFlash( CCloud *effectCloud, vec3_t origin, vec3_t sRGB, vec3_t eRGB, float rgbParm,
						int killTime, qhandle_t shader, int flags = 0 )
{
	if ( theFxHelper.mFrameTime < 1 )
	{ // disallow adding new effects when the system is paused
		return 0;
	}

	CFlash *fx = new CFlash;

	if ( fx )
	{
		fx->SetOrigin1( origin );

		// RGB----------------
		fx->SetRGBStart( sRGB );
		fx->SetRGBEnd( eRGB );

		if (( flags & FX_RGB_PARM_MASK ) == FX_RGB_WAVE )
		{
			fx->SetRGBParm( rgbParm * PI * 0.001f );
		}
		else if ( flags & FX_RGB_PARM_MASK )
		{
			// rgbParm should be a value from 0-100..
			fx->SetRGBParm( rgbParm * 0.01f * killTime + theFxHelper.mTime );
		}

/*		// Alpha----------------
		fx->SetAlphaStart( alpha1 );
		fx->SetAlphaEnd( alpha2 );

		if (( flags & FX_ALPHA_PARM_MASK ) == FX_ALPHA_WAVE )
		{
			fx->SetAlphaParm( alphaParm * PI * 0.001f );
		}
		else if ( flags & FX_ALPHA_PARM_MASK )
		{
			fx->SetAlphaParm( alphaParm * 0.01f * killTime + theFxHelper.mTime );
		}

		// Size----------------
		fx->SetSizeStart( size1 );
		fx->SetSizeEnd( size2 );

		if (( flags & FX_SIZE_PARM_MASK ) == FX_SIZE_WAVE )
		{
			fx->SetSizeParm( sizeParm * PI * 0.001f );
		}
		else if ( flags & FX_SIZE_PARM_MASK )
		{
			fx->SetSizeParm( sizeParm * 0.01f * killTime + theFxHelper.mTime );
		}
*/
		fx->SetShader( shader );
		fx->SetFlags( flags );

//		fx->SetSTScale( 1.0f, 1.0f );
		
		fx->Init();

		FX_AddPrimitive( (CEffect**)&fx, effectCloud, killTime );
	}

	return fx;
}


#ifdef CHC
//-------------------------------------------------------
// Functions for limited backward compatibility with EF.
//	These calls can be used for simple programmatic 
//	effects, temp effects or debug graphics.
// Note that this is not an all-inclusive list of
//	fx add functions from EF, nor are the calls guaranteed
//	to produce the exact same result.
//-------------------------------------------------------

//---------------------------------------------------
void FX_AddSprite( CCloud *effectCloud, vec3_t origin, vec3_t vel, vec3_t accel, 
							float scale, float dscale, 
							float sAlpha, float eAlpha, 
							float rotation, float bounce,  
							int life, qhandle_t shader, int flags )
{
	FX_AddParticle( origin, vel, accel, scale, scale, 0, 
							sAlpha, eAlpha, FX_ALPHA_LINEAR,
							WHITE, WHITE, 0,
							rotation, 0,
							vec3_origin, vec3_origin, bounce,
							0, 0,
							life, shader, flags );
}

//---------------------------------------------------
void FX_AddSprite( CCloud *effectCloud, vec3_t origin, vec3_t vel, vec3_t accel, 
							float scale, float dscale, 
							float sAlpha, float eAlpha, 
							vec3_t sRGB, vec3_t eRGB, 
							float rotation, float bounce, 
							int life, qhandle_t shader, int flags )
{
	FX_AddParticle( origin, vel, accel, scale, scale, 0, 
							sAlpha, eAlpha, FX_ALPHA_LINEAR,
							sRGB, eRGB, 0,
							rotation, 0,
							vec3_origin, vec3_origin, bounce,
							0, 0,
							life, shader, 0 );
}

//---------------------------------------------------
void FX_AddLine( CCloud *effectCloud, vec3_t start, vec3_t end, float stScale, 
							float width, float dwidth, 
							float sAlpha, float eAlpha, 
							int life, qhandle_t shader, int flags )
{
	FX_AddLine( start, end, width, width, 0,
							sAlpha, eAlpha, FX_ALPHA_LINEAR,
							WHITE, WHITE, 0,
							life, shader, 0 );
}

//---------------------------------------------------
void FX_AddLine( vec3_t start, vec3_t end, float stScale, 
							float width, float dwidth, 
							float sAlpha, float eAlpha, 
							vec3_t sRGB, vec3_t eRGB, 
							int life, qhandle_t shader, int flags )
{
	FX_AddLine( start, end, width, width, 0,
							sAlpha, eAlpha, FX_ALPHA_LINEAR,
							sRGB, eRGB, 0,
							life, shader, 0 );
}
#endif // CHC