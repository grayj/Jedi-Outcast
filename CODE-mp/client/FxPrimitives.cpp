#include "client.h"

#if !defined(G2_H_INC)
	#include "../ghoul2/g2_local.h"
#endif

#if !defined(FX_SCHEDULER_H_INC)
	#include "FxScheduler.h"
#endif

#include <set>

void FX_AddPrimitive( CEffect **pEffect, CCloud *effectCloud, int killTime );

// Helper function
//-------------------------
void ClampVec( vec3_t dat, byte *res )
{
	int r;

	// clamp all vec values, then multiply the normalized values by 255 to maximize the result
	for ( int i = 0; i < 3; i++ )
	{
		r = dat[i] * 255.0f;

		if ( r < 0 )
		{
			r = 0;
		}
		else if ( r > 255 )
		{
			r = 0xff;
		}

		res[i] = (unsigned char)r;
	}	
}

//--------------------------
//
// Base Effect Class
//
//--------------------------
CEffect::CEffect() :
	mNext(0)
{
	memset( &mRefEnt, 0, sizeof( mRefEnt ));

	mBoltInterfaceValid = false;
}

//----------------------------
CEffect::~CEffect()
{
}

//----------------------------
void CEffect::Die()
{
}

//----------------------------
bool CEffect::Cull()
{
	return false;
}

//----------------------------
void CEffect::Draw()
{
}

//----------------------------
bool CEffect::Update()
{
	// Game pausing can cause dumb time things to happen, so kill the effect in this instance
	if ( mTimeStart > theFxHelper.mTime )
	{
		return false;
	}
	
	return true;
}




CCloud::CCloud() :
	mNumPending(0)
{
	mBoltInterfaceValid = false;
	//mRefEnt.reType = RT_ENT_CHAIN;
}

extern set<CCloud *> OutstandClouds;

CCloud::~CCloud()
{
	OutstandClouds.erase(this);
	Die();
}

void CCloud::Die()
{
	CEffect	*current = mNext;
	CEffect	*next;

	while(current)
	{
		next = current->GetNext();

		current->Die();
		delete current;

		current = next;
	}

	mNext = 0;
}

bool CCloud::Update()
{
	CEffect	*current = mNext;
	CEffect	*next, *previous;
	bool	willDie;
	CGhoul2Info_v *g2Handle;
	mdxaBone_t boltMatrix;
	int entNum;
	vec3_t	objOrg, objAng, objScale, org;

	if (mBoltInterfaceValid)
	{
		entNum = mBoltInterface.GetEntNum();
		//Com_Printf("VALID BOLT INTERFACE ON UPDATED: %i\n", mBoltInterface.GetEntNum());
		
//		vec3_t getOrigin1;
//		mBoltInterface.GetOrigin(getOrigin1);
//		current->SetOrigin1(getOrigin1);
		

		g2Handle = mBoltInterface.GetG2Handle();

		if (!g2Handle || !G2API_HaveWeGhoul2Models(*((CGhoul2Info_v *)g2Handle)))
		{
			return false;
		}

		//First update the origin of the bolt object
		//VM_Call( cgvm, CG_GET_ORIGIN, mBoltInterface.GetEntNum(), objOrg );



		TCGBoltPos	*data = (TCGBoltPos *)cl.mSharedMemory;

		data->mEntityNum = entNum;
		//VM_Call( cgvm, CG_GET_BOLT_POS, entNum, objOrg, objAng);
		VectorCopy(data->mPoint, objOrg);
		VectorCopy(data->mPoint, objAng);

		mBoltInterface.GetScale(objScale);

		if (0)
		{
			gG2_GBMNoReconstruct = qtrue;
		}
		G2API_GetBoltMatrix(*((CGhoul2Info_v *)g2Handle), mBoltInterface.GetModelNum(), mBoltInterface.GetBoltNum(), &boltMatrix, objAng, objOrg, theFxHelper.mTime, NULL, objScale);

		G2API_GiveMeVectorFromMatrix(&boltMatrix, ORIGIN, org);

		ForceOrigin1(org);
	}

	if (!mNext && mNumPending <= 0)
	{
		return false;
	}

	previous = 0;
	while(current)
	{
		next = current->GetNext();

		willDie = false;
		if ( theFxHelper.mTime > current->GetKillTime())
		{ 
			// Clean up old effects, calling any death effects as needed
			// this flag just has to be cleared otherwise death effects might not happen correctly
			current->ClearFlags(FX_KILL_ON_IMPACT); 
			willDie = true;
		}

		if (mBoltInterfaceValid)
		{
			current->ForceOrigin1(org);
		}

		if (!willDie && !current->Update())
		{
			willDie = true;
		}

		if (willDie)
		{
			current->Die();
			delete current;

			if (previous)
			{
				previous->SetNext(next);
			}
			else
			{
				mNext = next;
			}
		}
		else
		{
			previous = current;
		}
		current = next;
	}

	if (!mNext && mNumPending <= 0)
	{
		return false;
	}

	return true;
}

bool CCloud::Cull()
{
	CEffect	*current = mNext;

	while(current)
	{
		if (!current->Cull())
		{
			return false;
		}

		current = current->GetNext();
	}

	return true;
}

void CCloud::Draw()
{
	CEffect		*current = mNext;
	refEntity_t	tempRef;

	VectorCopy(current->GetRefEnt().origin, mRefEnt.origin);
	
	// We must clear out the full entity, otherwise the mini won't stomp everything in the refent.
	memset(&tempRef, 0, sizeof(refEntity_t));

	memcpy(&tempRef, &mRefEnt, sizeof(mRefEnt));
	tempRef.customShader = -1;
	tempRef.uRefEnt.uMini.miniStart = -1;
	tempRef.uRefEnt.uMini.miniCount = 0;

	while(current)
	{
		if (current->GetRefEnt().customShader != tempRef.customShader)
		{
			tempRef.customShader = current->GetRefEnt().customShader;
			theFxHelper.AddFxToScene( &tempRef );
		}

		current->Draw();
		current = current->GetNext();
	}

	// clear it so that no other rogue mini ents get attached to the last parent
	theFxHelper.AddFxToScene((miniRefEntity_t *)0);
}

void CCloud::AddEffect(CEffect *which)
{
	which->SetNext(mNext);
	mNext = which;
}



//--------------------------
//
// Derived Particle Class
//
//--------------------------
CParticle::CParticle()
{
	mRefEnt.reType = RT_SPRITE;
}

//----------------------------
CParticle::~CParticle()
{
}

//----------------------------
void CParticle::Die()
{
	if ( mFlags & FX_DEATH_RUNS_FX && !(mFlags & FX_KILL_ON_IMPACT) )
	{
		vec3_t	norm;

		// Man, this just seems so, like, uncool and stuff...
		VectorSet( norm, crandom(), crandom(), crandom());
		VectorNormalize( norm );

		theFxScheduler.PlayEffect( mDeathFxID, mOrigin1, norm );
	}
}

//----------------------------
bool CParticle::Cull()
{
	vec3_t	dir;

	// Get the direction to the view
	VectorSubtract( mOrigin1, theFxHelper.refdef.vieworg, dir );

	// Check if it's behind the viewer
	if ( (DotProduct( theFxHelper.refdef.viewaxis[0], dir )) < 0 )
	{
		return true;
	}

	float len = VectorLengthSquared( dir );

	// Can't be too close
	if ( len < 12 * 12 )
	{
		return true;
	}

	return false;
}

//----------------------------
void CParticle::Draw()
{
	if ( mFlags & FX_DEPTH_HACK )
	{
		// Not sure if first person needs to be set, but it can't hurt?
		mRefEnt.renderfx |= RF_DEPTHHACK;
	}

	// Add our refEntity to the scene
	VectorCopy( mOrigin1, mRefEnt.origin );

	theFxHelper.AddFxToScene(&mRefEnt);
}

//----------------------------
// Update
//----------------------------
bool CParticle::Update()
{
	// Game pausing can cause dumb time things to happen, so kill the effect in this instance
	if ( mTimeStart > theFxHelper.mTime )
	{
		return false;
	}

#ifndef EFFECTSED
#ifndef CHC // SoF2 only
	if ( mFlags & FX_RELATIVE )
	{
		if (!mObj || !mObj->IsValid() )
		{
			// the thing we are bolted to is no longer valid, so we may as well just die.
			return false;
		}

		vec3_t	org;
		//vec3_t 	realVel, realAccel;
		vec3_t	currentOffset, initialOffset;
		vec3_t	offsetOffset;
	
		vec3_t	objOrg, objAng, objScale;

		CGhoul2Info_v *g2Handle;
		mdxaBone_t boltMatrix;
		int entNum = mObj->GetEntNum();

		g2Handle = mObj->GetG2Handle();

		if (!g2Handle || !G2API_HaveWeGhoul2Models(*((CGhoul2Info_v *)g2Handle)))
		{
			return false;
		}

		//First update the origin of the bolt object
		//VM_Call( cgvm, CG_GET_ORIGIN, mObj->GetEntNum(), objOrg );
		TCGBoltPos	*data = (TCGBoltPos *)cl.mSharedMemory;

		data->mEntityNum = entNum;
		//VM_Call( cgvm, CG_GET_BOLT_POS, entNum, objOrg, objAng);
		VectorCopy(data->mPoint, objOrg);
		VectorCopy(data->mPoint, objAng);

		mObj->SetOrigin(objOrg);
		mObj->SetForward(objAng);

		mObj->GetScale(objScale);

		G2API_GetBoltMatrix(*((CGhoul2Info_v *)g2Handle), mObj->GetModelNum(), mObj->GetBoltNum(), &boltMatrix, objAng, objOrg, theFxHelper.mTime, /*MODELLIST*/NULL, objScale);

		G2API_GiveMeVectorFromMatrix(&boltMatrix, ORIGIN, org);

		//Get the offset from the bolt point right now, and compare it to the offset in the initial position, then add onto the current position so that our offset is equal
		//FIXME: Take current offset caused by random bouncing/velocity into account somehow?

		VectorSubtract(org, mOrigin1, currentOffset);
		mObj->GetInitialOffset(initialOffset);

		VectorSubtract(currentOffset, initialOffset, offsetOffset);

		VectorAdd(mOrigin1, offsetOffset, mOrigin1);

		// calc the real velocity and accel vectors
		// FIXME: if you want right and up movement in addition to the forward movement, you'll have to convert dir into a set of perp. axes and do some extra work
	//	VectorScale( dir, mVel[0], realVel );
	//	VectorScale( dir, mAccel[0], realAccel );

		// Get our real velocity at the current time, taking into account the effects of acceleartion.  NOTE: not sure if this is even 100% correct math-wise
	//	VectorMA( realVel, (theFxHelper.mTime - mTimeStart) * 0.001f, realAccel, realVel );

		// Now move us to where we should be at the given time
	//	VectorMA( org, (theFxHelper.mTime - mTimeStart) * 0.001f, realVel, mOrigin1 );
	}
	else
#endif
#endif
	if ( UpdateOrigin() == false )
	{
		// we are marked for death
		return false;
	}

	UpdateSize();
	UpdateRGB();
	UpdateAlpha();
	UpdateRotation();

	return true;
}

//----------------------------
// Update Origin
//----------------------------
bool CParticle::UpdateOrigin()
{
	vec3_t	new_origin;
	float	ftime, time2;
	int		i;

	UpdateVelocity();

	// Calc the time differences
	ftime = theFxHelper.mFrameTime * 0.001f;
	time2 = ftime * ftime * 0.5f;

	// Predict the new position
	for ( i = 0 ; i < 3 ; i++ ) 
	{
		new_origin[i] = mOrigin1[i] + ftime * mVel[i] + time2 * mVel[i];
	}

	// Only perform physics if this object is tagged to do so
	if ( mFlags & FX_APPLY_PHYSICS )
	{
		bool solid;

		if ( mFlags & FX_EXPENSIVE_PHYSICS )
		{
			solid = true; // by setting this to true, we force a real trace to happen
		}
		else
		{
			TCGPointContents	*data = (TCGPointContents *)cl.mSharedMemory;

			VectorCopy(new_origin, data->mPoint);
			data->mPassEntityNum = ENTITYNUM_WORLD;

			// if this returns solid, we need to do a trace
			//solid = !!(VM_Call( cgvm, CG_POINT_CONTENTS, new_origin, ENTITYNUM_WORLD ) & CONTENTS_SOLID); 
			solid = !!(VM_Call( cgvm, CG_POINT_CONTENTS ) & CONTENTS_SOLID); 
		}

		if ( solid )
		{
			trace_t	trace;
			float	dot;

			if ( mFlags & FX_USE_BBOX )
			{
				theFxHelper.Trace( trace, mOrigin1, mMin, mMax, new_origin, -1, CONTENTS_SOLID );
			}
			else
			{
				theFxHelper.Trace( trace, mOrigin1, NULL, NULL, new_origin, -1, CONTENTS_SOLID );
			}

			// Hit something
			if ( trace.fraction < 1.0f )//&& !trace.startsolid && !trace.allsolid )
			{
				if ( mFlags & FX_IMPACT_RUNS_FX && !(trace.surfaceFlags & SURF_NOIMPACT ))
				{
					theFxScheduler.PlayEffect( mImpactFxID, trace.endpos, trace.plane.normal );
				}

				if ( mFlags & FX_KILL_ON_IMPACT	)
				{
					// time to die
					return false;
				}

				VectorMA( mVel, ftime * trace.fraction, mAccel, mVel );

				dot = DotProduct( mVel, trace.plane.normal );

				VectorMA( mVel, -2 * dot, trace.plane.normal, mVel );

				VectorScale( mVel, mElasticity, mVel );

				// If the velocity is too low, make it stop moving, rotating, and turn off physics to avoid
				//	doing expensive operations when they aren't needed
				if ( trace.plane.normal[2] > 0 && mVel[2] < 4 )
				{
					VectorClear( mVel );
					VectorClear( mAccel );

					mFlags &= ~(FX_APPLY_PHYSICS|FX_IMPACT_RUNS_FX);
				}

				// Set the origin to the exact impact point
				VectorCopy( trace.endpos, mOrigin1 );
				return true;
			}
		}
	}

	// No physics were done to this object, move it
	VectorCopy( new_origin, mOrigin1 );

	return true;
}

//----------------------------
// Update Velocity
//----------------------------
void CParticle::UpdateVelocity()
{
	VectorMA( mVel, theFxHelper.mFrameTime * 0.001f, mAccel, mVel );
}

//----------------------------
// Update Size
//----------------------------
void CParticle::UpdateSize()
{
	// completely biased towards start if it doesn't get overridden
	float	perc1 = 1.0f, perc2 = 1.0f; 

	if ( (mFlags & FX_SIZE_LINEAR) )
	{ 
		// calculate element biasing
		perc1 = 1.0f - (float)(theFxHelper.mTime - mTimeStart) 
						/ (float)(mTimeEnd - mTimeStart);
	}

	// We can combine FX_LINEAR with _either_ FX_NONLINEAR, FX_WAVE, or FX_CLAMP
	if (( mFlags & FX_SIZE_PARM_MASK ) == FX_SIZE_NONLINEAR )
	{
		if ( theFxHelper.mTime > mSizeParm )
		{ 
			// get percent done, using parm as the start of the non-linear fade
			perc2 = 1.0f - (float)(theFxHelper.mTime - mSizeParm) 
							/ (float)(mTimeEnd - mSizeParm);
		}

		if ( mFlags & FX_SIZE_LINEAR )
		{
			// do an even blend
			perc1 = perc1 * 0.5f + perc2 * 0.5f;
		}
		else
		{ 
			// just copy it over...sigh
			perc1 = perc2;
		}
	}
	else if (( mFlags & FX_SIZE_PARM_MASK ) == FX_SIZE_WAVE )
	{ 
		// wave gen, with parm being the frequency multiplier
		perc1 = perc1 * (float)cos( (theFxHelper.mTime - mTimeStart) * mSizeParm );
	}
	else if (( mFlags & FX_SIZE_PARM_MASK ) == FX_SIZE_CLAMP )
	{
		if ( theFxHelper.mTime < mSizeParm )
		{ 
			// get percent done, using parm as the start of the non-linear fade
			perc2 = (float)(mSizeParm - theFxHelper.mTime) 
							/ (float)(mSizeParm - mTimeStart);
		}
		else
		{
			perc2 = 0.0f; // make it full size??
		}

		if ( (mFlags & FX_SIZE_LINEAR) )
		{ 
			// do an even blend
			perc1 = perc1 * 0.5f + perc2 * 0.5f;
		}
		else
		{ 
			// just copy it over...sigh
			perc1 = perc2;
		}
	}

	// If needed, RAND can coexist with linear and either non-linear or wave.
	if (( mFlags & FX_SIZE_RAND ))
	{ 
		// Random simply modulates the existing value
		perc1 = random() * perc1;
	}

	mRefEnt.radius = (mSizeStart * perc1) + (mSizeEnd * (1.0f - perc1));
}

//----------------------------
// Update RGB
//----------------------------
void CParticle::UpdateRGB()
{
	// completely biased towards start if it doesn't get overridden
	float	perc1 = 1.0f, perc2 = 1.0f; 
	vec3_t	res;

	if ( (mFlags & FX_RGB_LINEAR) )
	{ 
		// calculate element biasing
		perc1 = 1.0f - (float)( theFxHelper.mTime - mTimeStart ) 
						/ (float)( mTimeEnd - mTimeStart );
	}

	// We can combine FX_LINEAR with _either_ FX_NONLINEAR, FX_WAVE, or FX_CLAMP
	if (( mFlags & FX_RGB_PARM_MASK ) == FX_RGB_NONLINEAR )
	{
		if ( theFxHelper.mTime > mRGBParm )
		{ 
			// get percent done, using parm as the start of the non-linear fade
			perc2 = 1.0f - (float)( theFxHelper.mTime - mRGBParm ) 
							/ (float)( mTimeEnd - mRGBParm );
		}

		if ( (mFlags & FX_RGB_LINEAR) )
		{ 
			// do an even blend
			perc1 = perc1 * 0.5f + perc2 * 0.5f;
		}
		else
		{ 
			// just copy it over...sigh
			perc1 = perc2;
		}
	}
	else if (( mFlags & FX_RGB_PARM_MASK ) == FX_RGB_WAVE )
	{ 
		// wave gen, with parm being the frequency multiplier
		perc1 = perc1 * (float)cos(( theFxHelper.mTime - mTimeStart ) * mRGBParm );
	}
	else if (( mFlags & FX_RGB_PARM_MASK ) == FX_RGB_CLAMP )
	{
		if ( theFxHelper.mTime < mRGBParm )
		{ 
			// get percent done, using parm as the start of the non-linear fade
			perc2 = (float)(mRGBParm - theFxHelper.mTime) 
							/ (float)(mRGBParm - mTimeStart);
		}
		else
		{
			perc2 = 0.0f; // make it full size??
		}

		if (( mFlags & FX_RGB_LINEAR ))
		{ 
			// do an even blend
			perc1 = perc1 * 0.5f + perc2 * 0.5f;
		}
		else
		{ 
			// just copy it over...sigh
			perc1 = perc2;
		}
	}

	// If needed, RAND can coexist with linear and either non-linear or wave.
	if (( mFlags & FX_RGB_RAND ))
	{ 
		// Random simply modulates the existing value
		perc1 = random() * perc1;
	}

	// Now get the correct color 
	VectorScale( mRGBStart, perc1, res );
	VectorMA( res, (1.0f - perc1), mRGBEnd, mRefEnt.oldorigin ); // angles is a temp storage, will get clamped to a byte in the UpdateAlpha section
}

//----------------------------
// Update Alpha
//----------------------------
void CParticle::UpdateAlpha()
{
	// completely biased towards start if it doesn't get overridden
	float	perc1 = 1.0f, perc2 = 1.0f;

	if ( mFlags & FX_ALPHA_LINEAR )
	{ 
		// calculate element biasing
		perc1 = 1.0f - (float)(theFxHelper.mTime - mTimeStart) 
						/ (float)(mTimeEnd - mTimeStart);
	}

	// We can combine FX_LINEAR with _either_ FX_NONLINEAR, FX_WAVE, or FX_CLAMP
	if (( mFlags & FX_ALPHA_PARM_MASK ) == FX_ALPHA_NONLINEAR )
	{
		if ( theFxHelper.mTime > mAlphaParm )
		{ 
			// get percent done, using parm as the start of the non-linear fade
			perc2 = 1.0f - (float)(theFxHelper.mTime - mAlphaParm) 
							/ (float)(mTimeEnd - mAlphaParm);
		}

		if ( mFlags & FX_ALPHA_LINEAR )
		{ 
			// do an even blend
			perc1 = perc1 * 0.5f + perc2 * 0.5f;
		}
		else
		{ 
			// just copy it over...sigh
			perc1 = perc2;
		}
	}
	else if (( mFlags & FX_ALPHA_PARM_MASK ) == FX_ALPHA_WAVE )
	{ 
		// wave gen, with parm being the frequency multiplier
		perc1 = perc1 * (float)cos( (theFxHelper.mTime - mTimeStart) * mAlphaParm );
	}
	else if (( mFlags & FX_ALPHA_PARM_MASK ) == FX_ALPHA_CLAMP )
	{
		if ( theFxHelper.mTime < mAlphaParm )
		{ 
			// get percent done, using parm as the start of the non-linear fade
			perc2 = (float)(mAlphaParm - theFxHelper.mTime) 
							/ (float)(mAlphaParm - mTimeStart);
		}
		else
		{
			perc2 = 0.0f; // make it full size??
		}

		if ( mFlags & FX_ALPHA_LINEAR )
		{ 
			// do an even blend
			perc1 = perc1 * 0.5f + perc2 * 0.5f;
		}
		else
		{ 
			// just copy it over...sigh
			perc1 = perc2;
		}
	}

	perc1 = (mAlphaStart * perc1) + (mAlphaEnd * (1.0f - perc1));

	// We should be in the right range, but clamp to ensure
	if ( perc1 < 0.0f )
	{
		perc1 = 0.0f;
	}
	else if ( perc1 > 1.0f )
	{
		perc1 = 1.0f;
	}

	// If needed, RAND can coexist with linear and either non-linear or wave.
	if ( (mFlags & FX_ALPHA_RAND) )
	{ 
		// Random simply modulates the existing value
		perc1 = random() * perc1;
	}

	if ( mFlags & FX_USE_ALPHA )
	{
		// should use this when using art that has an alpha channel
		 ClampVec( mRefEnt.oldorigin, (byte*)(&mRefEnt.shaderRGBA) );
		 mRefEnt.shaderRGBA[3] = (byte)(perc1 * 0xff);
	}
	else
	{
		// Modulate the rgb fields by the alpha value to do the fade, works fine for additive blending
		VectorScale( mRefEnt.oldorigin, perc1, mRefEnt.oldorigin );
		ClampVec( mRefEnt.oldorigin, (byte*)(&mRefEnt.shaderRGBA) );
	}
}

//--------------------------
void CParticle::UpdateRotation()
{
	mRefEnt.rotation += theFxHelper.mFrameTime * 0.01f * mRotationDelta;
}


//--------------------------------
//
// Derived Oriented Particle Class
//
//--------------------------------
COrientedParticle::COrientedParticle()
{
	mRefEnt.reType = RT_ORIENTED_QUAD;
}

//----------------------------
COrientedParticle::~COrientedParticle()
{
}

//----------------------------
bool COrientedParticle::Cull()
{
	vec3_t	dir;

	// Get the direction to the view
	VectorSubtract( mOrigin1, theFxHelper.refdef.vieworg, dir );

	// Check if it's behind the viewer
	if ( (DotProduct( theFxHelper.refdef.viewaxis[0], dir )) < 0 )
	{
		return true;
	}

	float len = VectorLengthSquared( dir );

	// Can't be too close
	if ( len < 16 * 16 )
	{
		return true;
	}

	return false;
}

//----------------------------
void COrientedParticle::Draw()
{
	if ( mFlags & FX_DEPTH_HACK )
	{
		// Not sure if first person needs to be set
		mRefEnt.renderfx |= RF_DEPTHHACK;
	}

	// Add our refEntity to the scene
	VectorCopy( mOrigin1, mRefEnt.origin );
	VectorCopy( mNormal, mRefEnt.axis[0] );

	theFxHelper.AddFxToScene( &mRefEnt );
}

//----------------------------
// Update
//----------------------------
bool COrientedParticle::Update()
{
	// Game pausing can cause dumb time things to happen, so kill the effect in this instance
	if ( mTimeStart > theFxHelper.mTime )
	{
		return false;
	}
		
	if ( UpdateOrigin() == false )
	{
		// we are marked for death
		return false;
	}

	UpdateSize();
	UpdateRGB();
	UpdateAlpha();
	UpdateRotation();

	return true;
}


//----------------------------
//
// Derived Line Class
//
//----------------------------
CLine::CLine()
{
	mRefEnt.reType = RT_LINE;
}

//----------------------------
CLine::~CLine()
{
}

//----------------------------
void CLine::Die()
{
}

//----------------------------
bool CLine::Cull()
{
	return false;
}

//----------------------------
void CLine::Draw()
{
	if ( mFlags & FX_DEPTH_HACK )
	{
		// Not sure if first person needs to be set, but it can't hurt?
		mRefEnt.renderfx |= RF_DEPTHHACK;
	}

	VectorCopy( mOrigin1, mRefEnt.origin );
	VectorCopy( mOrigin2, mRefEnt.oldorigin );

	theFxHelper.AddFxToScene(&mRefEnt);
}

//----------------------------
bool CLine::Update()
{
	// Game pausing can cause dumb time things to happen, so kill the effect in this instance
	if ( mTimeStart > theFxHelper.mTime )
	{
		return false;
	}
		
	UpdateSize();
	UpdateRGB();
	UpdateAlpha();

	return true;
}


//----------------------------
//
// Derived Electricity Class
//
//----------------------------
CElectricity::CElectricity()
{
	mRefEnt.reType = RT_ELECTRICITY;
}

//----------------------------
CElectricity::~CElectricity()
{
}

//----------------------------
void CElectricity::Die()
{
}

//----------------------------
bool CElectricity::Cull()
{
	return false;
}

//----------------------------
void CElectricity::Initialize()
{
	mRefEnt.frame = random() * 1265536;
	mRefEnt.axis[0][2] = theFxHelper.mTime + (mTimeEnd - mTimeStart); // endtime

	if ( mFlags & FX_DEPTH_HACK )
	{
		mRefEnt.renderfx |= RF_DEPTHHACK;
	}

	if ( mFlags & FX_BRANCH )
	{
		mRefEnt.renderfx |= RF_FORKED;	
	}

	if ( mFlags & FX_TAPER )
	{
		mRefEnt.renderfx |= RF_TAPERED;
	}

	if ( mFlags & FX_GROW )
	{
		mRefEnt.renderfx |= RF_GROW;
	}
}

//----------------------------
void CElectricity::Draw()
{
	VectorCopy( mOrigin1, mRefEnt.origin );
	VectorCopy( mOrigin2, mRefEnt.oldorigin );
	mRefEnt.axis[0][0] = mChaos;
	mRefEnt.axis[0][1] = mTimeEnd - mTimeStart;

	theFxHelper.AddFxToScene( &mRefEnt );
}

//----------------------------
bool CElectricity::Update()
{
	// Game pausing can cause dumb time things to happen, so kill the effect in this instance
	if ( mTimeStart > theFxHelper.mTime )
	{
		return false;
	}
		
	UpdateSize();
	UpdateRGB();
	UpdateAlpha();

	return true;
}

//----------------------------
//
// Derived Tail Class
//
//----------------------------
CTail::CTail()
{
	mRefEnt.reType = RT_LINE;
}

//----------------------------
CTail::~CTail()
{
}

//----------------------------
bool CTail::Cull()
{
	return false;
}

//----------------------------
void CTail::Draw()
{
	if ( mFlags & FX_DEPTH_HACK )
	{
		// Not sure if first person needs to be set
		mRefEnt.renderfx |= RF_DEPTHHACK;
	}

	VectorCopy( mOrigin1, mRefEnt.origin );

	theFxHelper.AddFxToScene(&mRefEnt);
}

//----------------------------
bool CTail::Update()
{
	// Game pausing can cause dumb time things to happen, so kill the effect in this instance
	if ( mTimeStart > theFxHelper.mTime )
	{
		return false;
	}
		
	VectorCopy( mOrigin1, mOldOrigin );

	if ( UpdateOrigin() == false )
	{
		// we are marked for death
		return false;
	}

	UpdateSize();
	UpdateLength();
	UpdateRGB();
	UpdateAlpha();

	CalcNewEndpoint();

	return true;
}

//----------------------------
void CTail::UpdateLength()
{
	// completely biased towards start if it doesn't get overridden
	float	perc1 = 1.0f, perc2 = 1.0f; 

	if ( mFlags & FX_LENGTH_LINEAR )
	{ 
		// calculate element biasing
		perc1 = 1.0f - (float)(theFxHelper.mTime - mTimeStart) 
						/ (float)(mTimeEnd - mTimeStart);
	}

	// We can combine FX_LINEAR with _either_ FX_NONLINEAR or FX_WAVE
	if (( mFlags & FX_LENGTH_PARM_MASK ) == FX_LENGTH_NONLINEAR )
	{
		if ( theFxHelper.mTime > mLengthParm )
		{ 
			// get percent done, using parm as the start of the non-linear fade
			perc2 = 1.0f - (float)(theFxHelper.mTime - mLengthParm) 
							/ (float)(mTimeEnd - mLengthParm);
		}

		if ( mFlags & FX_LENGTH_LINEAR )
		{ 
			// do an even blend
			perc1 = perc1 * 0.5f + perc2 * 0.5f;
		}
		else
		{ 
			// just copy it over...sigh
			perc1 = perc2;
		}
	}
	else if (( mFlags & FX_LENGTH_PARM_MASK ) == FX_LENGTH_WAVE )
	{ 
		// wave gen, with parm being the frequency multiplier
		perc1 = perc1 * (float)cos( (theFxHelper.mTime - mTimeStart) * mLengthParm );
	}
	else if (( mFlags & FX_LENGTH_PARM_MASK ) == FX_LENGTH_CLAMP )
	{
		if ( theFxHelper.mTime < mLengthParm )
		{ 
			// get percent done, using parm as the start of the non-linear fade
			perc2 = (float)(mLengthParm - theFxHelper.mTime) 
							/ (float)(mLengthParm - mTimeStart);
		}
		else
		{
			perc2 = 0.0f; // make it full size??
		}

		if ( mFlags & FX_LENGTH_LINEAR )
		{ 
			// do an even blend
			perc1 = perc1 * 0.5f + perc2 * 0.5f;
		}
		else
		{ 
			// just copy it over...sigh
			perc1 = perc2;
		}
	}

	// If needed, RAND can coexist with linear and either non-linear or wave.
	if ( mFlags & FX_LENGTH_RAND )
	{ 
		// Random simply modulates the existing value
		perc1 = random() * perc1;
	}

	mLength = (mLengthStart * perc1) + (mLengthEnd * (1.0f - perc1));
}


//----------------------------
void CTail::CalcNewEndpoint()
{
	vec3_t temp;

	// FIXME:  Hmmm, this looks dumb when physics are on and a bounce happens
	VectorSubtract( mOldOrigin, mOrigin1, temp );

	// I wish we didn't have to do a VectorNormalize every frame.....
	VectorNormalize( temp );

	VectorMA( mOrigin1, mLength, temp, mRefEnt.oldorigin );
}


//----------------------------
//
// Derived Cylinder Class
//
//----------------------------
CCylinder::CCylinder()
{
	mRefEnt.reType = RT_CYLINDER;
}

//----------------------------
CCylinder::~CCylinder()
{
}

//----------------------------
bool CCylinder::Cull()
{
	return false;
}

//----------------------------
void CCylinder::Draw()
{
	if ( mFlags & FX_DEPTH_HACK )
	{
		// Not sure if first person needs to be set, but it can't hurt?
		mRefEnt.renderfx |= RF_DEPTHHACK;
	}

	VectorCopy( mOrigin1, mRefEnt.origin );
	VectorMA( mOrigin1, mLength, mRefEnt.axis[0], mRefEnt.oldorigin );

	theFxHelper.AddFxToScene(&mRefEnt);
}

//----------------------------
// Update Size2
//----------------------------
void CCylinder::UpdateSize2()
{
	// completely biased towards start if it doesn't get overridden
	float	perc1 = 1.0f, perc2 = 1.0f; 

	if ( mFlags & FX_SIZE2_LINEAR )
	{ 
		// calculate element biasing
		perc1 = 1.0f - (float)(theFxHelper.mTime - mTimeStart) 
						/ (float)(mTimeEnd - mTimeStart);
	}

	// We can combine FX_LINEAR with _either_ FX_NONLINEAR or FX_WAVE
	if (( mFlags & FX_SIZE2_PARM_MASK ) == FX_SIZE2_NONLINEAR )
	{
		if ( theFxHelper.mTime > mSize2Parm )
		{ 
			// get percent done, using parm as the start of the non-linear fade
			perc2 = 1.0f - (float)(theFxHelper.mTime - mSize2Parm) 
							/ (float)(mTimeEnd - mSize2Parm);
		}

		if ( (mFlags & FX_SIZE2_LINEAR) )
		{ 
			// do an even blend
			perc1 = perc1 * 0.5f + perc2 * 0.5f;
		}
		else
		{ 
			// just copy it over...sigh
			perc1 = perc2;
		}
	}
	else if (( mFlags & FX_SIZE2_PARM_MASK ) == FX_SIZE2_WAVE )
	{ 
		// wave gen, with parm being the frequency multiplier
		perc1 = perc1 * (float)cos( (theFxHelper.mTime - mTimeStart) * mSize2Parm );
	}
	else if (( mFlags & FX_SIZE2_PARM_MASK ) == FX_SIZE2_CLAMP )
	{
		if ( theFxHelper.mTime < mSize2Parm )
		{ 
			// get percent done, using parm as the start of the non-linear fade
			perc2 = (float)(mSize2Parm - theFxHelper.mTime) 
							/ (float)(mSize2Parm - mTimeStart);
		}
		else
		{
			perc2 = 0.0f; // make it full size??
		}

		if ( mFlags & FX_SIZE2_LINEAR )
		{ 
			// do an even blend
			perc1 = perc1 * 0.5f + perc2 * 0.5f;
		}
		else
		{ 
			// just copy it over...sigh
			perc1 = perc2;
		}
	}

	// If needed, RAND can coexist with linear and either non-linear or wave.
	if ( mFlags & FX_SIZE2_RAND )
	{ 
		// Random simply modulates the existing value
		perc1 = random() * perc1;
	}

	mRefEnt.rotation = (mSize2Start * perc1) + (mSize2End * (1.0f - perc1));
}

//----------------------------
bool CCylinder::Update()
{
	// Game pausing can cause dumb time things to happen, so kill the effect in this instance
	if ( mTimeStart > theFxHelper.mTime )
	{
		return false;
	}
		
	UpdateSize();
	UpdateSize2();
	UpdateLength();
	UpdateRGB();
	UpdateAlpha();

	return true;
}


//----------------------------
//
// Derived Emitter Class
//
//----------------------------
CEmitter::CEmitter()
{
	// There may or may not be a model, but if there isn't one, 
	//	we just won't bother adding the refEnt in our Draw func
	mRefEnt.reType = RT_MODEL;
}

//----------------------------
CEmitter::~CEmitter()
{
}

//----------------------------
bool CEmitter::Cull()
{
	return false;
}

//----------------------------
// Draw
//----------------------------
void CEmitter::Draw()
{
	// Emitters don't draw themselves, but they may need to add an attached model
	if ( mFlags & FX_ATTACHED_MODEL )
	{
		mRefEnt.nonNormalizedAxes = qtrue;

		VectorCopy( mOrigin1, mRefEnt.origin );

		// ensure that we are sized
		for ( int i = 0; i < 3; i++ )
		{
			VectorScale( mRefEnt.axis[i], mRefEnt.radius, mRefEnt.axis[i] );
		}

		theFxHelper.AddFxToScene(&mRefEnt);
	}

	// If we are emitting effects, we had better be careful because just calling it every cgame frame could
	//	either choke up the effects system on a fast machine, or look really nasty on a low end one.
	if ( mFlags & FX_EMIT_FX )
	{
		vec3_t	org, v;
		float	ftime, time2, 
				step;
		int		i, t, dif;

#define TRAIL_RATE		12 // we "think" at about a 60hz rate

		// Pick a target step distance and square it
		step = mDensity + crandom() * mVariance;
		step *= step;

		dif = 0;

		for ( t = mOldTime; t <= theFxHelper.mTime; t += TRAIL_RATE ) 
		{
			dif += TRAIL_RATE;

			// ?Not sure if it's better to update this before or after updating the origin
			VectorMA( mOldVelocity, dif * 0.001f, mAccel, v );

			// Calc the time differences
			ftime = dif * 0.001f;
			time2 = ftime * ftime * 0.5f;

			// Predict the new position
			for ( i = 0 ; i < 3 ; i++ ) 
			{
				org[i] = mOldOrigin[i] + ftime * v[i] + time2 * v[i];
			}
	
			// Is it time to draw an effect?
			if ( DistanceSquared( org, mOldOrigin ) >= step )
			{ 
				// Pick a new target step distance and square it
				step = mDensity + crandom() * mVariance;
				step *= step;

				// We met the step criteria so, we should add in the effect
				theFxScheduler.PlayEffect( mEmitterFxID, org, mRefEnt.axis );

				VectorCopy( org, mOldOrigin );
				VectorCopy( v, mOldVelocity );
				dif = 0;
				mOldTime = t;
			}
		}
	}
}

//----------------------------
bool CEmitter::Update()
{
	// Game pausing can cause dumb time things to happen, so kill the effect in this instance
	if ( mTimeStart > theFxHelper.mTime )
	{
		return false;
	}
		
	// Use this to track if we've stopped moving
	VectorCopy( mOrigin1, mOldOrigin );
	VectorCopy( mVel, mOldVelocity );

	if ( UpdateOrigin() == false )
	{
		// we are marked for death
		return false;
	}

	// If the thing is no longer moving, kill the angle delta, but don't do it too quickly or it will
	//	look very artificial.  Don't do it too slowly or it will look like there is no friction.
	if ( VectorCompare( mOldOrigin, mOrigin1 ))
	{
		VectorScale( mAngleDelta, 0.7f, mAngleDelta );
	}

	UpdateAngles();
	UpdateSize();
//	UpdateRGB();	// had wanted to do something slick whereby an emitted effect could somehow inherit these
//	UpdateAlpha();	// values, but it's not a priority right now.

	return true;
}

//----------------------------
void CEmitter::UpdateAngles()
{
	VectorMA( mAngles, theFxHelper.mFrameTime * 0.01f, mAngleDelta, mAngles ); // was 0.001f, but then you really have to jack up the delta to even notice anything
	AnglesToAxis( mAngles, mRefEnt.axis );
}


//--------------------------
//
// Derived Light Class
//
//--------------------------

CLight::CLight()
{
}

//----------------------------
CLight::~CLight()
{
}

//----------------------------
bool CLight::Cull()
{
	return false;
}

//----------------------------
void CLight::Draw()
{
	theFxHelper.AddLightToScene( mOrigin1, mRefEnt.radius, 
				mRefEnt.oldorigin[0], mRefEnt.oldorigin[1], mRefEnt.oldorigin[2] );
}

//----------------------------
// Update
//----------------------------
bool CLight::Update()
{
	// Game pausing can cause dumb time things to happen, so kill the effect in this instance
	if ( mTimeStart > theFxHelper.mTime )
	{
		return false;
	}
		
	UpdateSize();
	UpdateRGB();
	return true;
}

//----------------------------
// Update Size
//----------------------------
void CLight::UpdateSize()
{
	// completely biased towards start if it doesn't get overridden
	float	perc1 = 1.0f, perc2 = 1.0f; 

	if ( mFlags & FX_SIZE_LINEAR )
	{ 
		// calculate element biasing
		perc1 = 1.0f - (float)(theFxHelper.mTime - mTimeStart) 
						/ (float)(mTimeEnd - mTimeStart);
	}

	// We can combine FX_LINEAR with _either_ FX_NONLINEAR or FX_WAVE
	if (( mFlags & FX_SIZE_PARM_MASK ) == FX_SIZE_NONLINEAR )
	{
		if ( theFxHelper.mTime > mSizeParm )
		{ 
			// get percent done, using parm as the start of the non-linear fade
			perc2 = 1.0f - (float)(theFxHelper.mTime - mSizeParm) 
							/ (float)(mTimeEnd - mSizeParm);
		}

		if ( (mFlags & FX_SIZE_LINEAR) )
		{ 
			// do an even blend
			perc1 = perc1 * 0.5f + perc2 * 0.5f;
		}
		else
		{ 
			// just copy it over...sigh
			perc1 = perc2;
		}
	}
	else if (( mFlags & FX_SIZE_PARM_MASK ) == FX_SIZE_WAVE )
	{ 
		// wave gen, with parm being the frequency multiplier
		perc1 = perc1 * (float)cos( (theFxHelper.mTime - mTimeStart) * mSizeParm );
	}
	else if (( mFlags & FX_SIZE_PARM_MASK ) == FX_SIZE_CLAMP )
	{
		if ( theFxHelper.mTime < mSizeParm )
		{ 
			// get percent done, using parm as the start of the non-linear fade
			perc2 = (float)(mSizeParm - theFxHelper.mTime) 
							/ (float)(mSizeParm - mTimeStart);
		}
		else
		{
			perc2 = 0.0f; // make it full size??
		}

		if ( mFlags & FX_SIZE_LINEAR )
		{ 
			// do an even blend
			perc1 = perc1 * 0.5f + perc2 * 0.5f;
		}
		else
		{ 
			// just copy it over...sigh
			perc1 = perc2;
		}
	}

	// If needed, RAND can coexist with linear and either non-linear or wave.
	if ( mFlags & FX_SIZE_RAND )
	{ 
		// Random simply modulates the existing value
		perc1 = random() * perc1;
	}

	mRefEnt.radius = (mSizeStart * perc1) + (mSizeEnd * (1.0f - perc1));
}

//----------------------------
// Update RGB
//----------------------------
void CLight::UpdateRGB()
{
	// completely biased towards start if it doesn't get overridden
	float	perc1 = 1.0f, perc2 = 1.0f; 
	vec3_t	res;

	if ( mFlags & FX_RGB_LINEAR )
	{ 
		// calculate element biasing
		perc1 = 1.0f - (float)( theFxHelper.mTime - mTimeStart ) 
						/ (float)( mTimeEnd - mTimeStart );
	}

	// We can combine FX_LINEAR with _either_ FX_NONLINEAR or FX_WAVE
	if (( mFlags & FX_RGB_PARM_MASK ) == FX_RGB_NONLINEAR )
	{
		if ( theFxHelper.mTime > mRGBParm )
		{ 
			// get percent done, using parm as the start of the non-linear fade
			perc2 = 1.0f - (float)( theFxHelper.mTime - mRGBParm ) 
							/ (float)( mTimeEnd - mRGBParm );
		}

		if ( mFlags & FX_RGB_LINEAR )
		{ 
			// do an even blend
			perc1 = perc1 * 0.5f + perc2 * 0.5f;
		}
		else
		{ 
			// just copy it over...sigh
			perc1 = perc2;
		}
	}
	else if (( mFlags & FX_RGB_PARM_MASK ) == FX_RGB_WAVE )
	{ 
		// wave gen, with parm being the frequency multiplier
		perc1 = perc1 * (float)cos(( theFxHelper.mTime - mTimeStart ) * mRGBParm );
	}
	else if (( mFlags & FX_RGB_PARM_MASK ) == FX_RGB_CLAMP )
	{
		if ( theFxHelper.mTime < mRGBParm )
		{ 
			// get percent done, using parm as the start of the non-linear fade
			perc2 = (float)(mRGBParm - theFxHelper.mTime) 
							/ (float)(mRGBParm - mTimeStart);
		}
		else
		{
			perc2 = 0.0f; // make it full size??
		}

		if ( mFlags & FX_RGB_LINEAR )
		{ 
			// do an even blend
			perc1 = perc1 * 0.5f + perc2 * 0.5f;
		}
		else
		{ 
			// just copy it over...sigh
			perc1 = perc2;
		}
	}

	// If needed, RAND can coexist with linear and either non-linear or wave.
	if ( mFlags & FX_RGB_RAND )
	{ 
		// Random simply modulates the existing value
		perc1 = random() * perc1;
	}

	// Now get the correct color 
	VectorScale( mRGBStart, perc1, res );

	mRefEnt.oldorigin[0] = res[0] + ( 1.0f - perc1 ) * mRGBEnd[0];
	mRefEnt.oldorigin[1] = res[1] + ( 1.0f - perc1 ) * mRGBEnd[1];
	mRefEnt.oldorigin[2] = res[2] + ( 1.0f - perc1 ) * mRGBEnd[2];
}

//#ifdef CHC
//--------------------------
//
// Derived Trail Class
//
//--------------------------
bool CTrail::Cull()
{
	return false;
}

#define NEW_MUZZLE	0
#define NEW_TIP		1
#define OLD_TIP		2
#define OLD_MUZZLE	3

//----------------------------
void CTrail::Draw()
{
	polyVert_t	verts[3];
//	vec3_t		color;

	// build the first tri out of the new muzzle...new tip...old muzzle
	VectorCopy( mVerts[NEW_MUZZLE].origin, verts[0].xyz );
	VectorCopy( mVerts[NEW_TIP].origin, verts[1].xyz );
	VectorCopy( mVerts[OLD_MUZZLE].origin, verts[2].xyz );

//	VectorScale( mVerts[NEW_MUZZLE].curRGB, mVerts[NEW_MUZZLE].curAlpha, color );
	verts[0].modulate[0] = mVerts[NEW_MUZZLE].rgb[0];
	verts[0].modulate[1] = mVerts[NEW_MUZZLE].rgb[1];
	verts[0].modulate[2] = mVerts[NEW_MUZZLE].rgb[2];
	verts[0].modulate[3] = mVerts[NEW_MUZZLE].alpha;

//	VectorScale( mVerts[NEW_TIP].curRGB, mVerts[NEW_TIP].curAlpha, color );
	verts[1].modulate[0] = mVerts[NEW_TIP].rgb[0];
	verts[1].modulate[1] = mVerts[NEW_TIP].rgb[1];
	verts[1].modulate[2] = mVerts[NEW_TIP].rgb[2];
	verts[1].modulate[3] = mVerts[NEW_TIP].alpha;

//	VectorScale( mVerts[OLD_MUZZLE].curRGB, mVerts[OLD_MUZZLE].curAlpha, color );
	verts[2].modulate[0] = mVerts[OLD_MUZZLE].rgb[0];
	verts[2].modulate[1] = mVerts[OLD_MUZZLE].rgb[1];
	verts[2].modulate[2] = mVerts[OLD_MUZZLE].rgb[2];
	verts[2].modulate[3] = mVerts[OLD_MUZZLE].alpha;

	verts[0].st[0] = mVerts[NEW_MUZZLE].curST[0];
	verts[0].st[1] = mVerts[NEW_MUZZLE].curST[1];
	verts[1].st[0] = mVerts[NEW_TIP].curST[0];
	verts[1].st[1] = mVerts[NEW_TIP].curST[1];
	verts[2].st[0] = mVerts[OLD_MUZZLE].curST[0];
	verts[2].st[1] = mVerts[OLD_MUZZLE].curST[1];

	// Add this tri
	theFxHelper.AddPolyToScene( mShader, 3, verts );

	// build the second tri out of the old muzzle...old tip...new tip
	VectorCopy( mVerts[OLD_MUZZLE].origin, verts[0].xyz );
	VectorCopy( mVerts[OLD_TIP].origin, verts[1].xyz );
	VectorCopy( mVerts[NEW_TIP].origin, verts[2].xyz );

//	VectorScale( mVerts[OLD_MUZZLE].curRGB, mVerts[OLD_MUZZLE].curAlpha, color );
	verts[0].modulate[0] = mVerts[OLD_MUZZLE].rgb[0];
	verts[0].modulate[1] = mVerts[OLD_MUZZLE].rgb[1];
	verts[0].modulate[2] = mVerts[OLD_MUZZLE].rgb[2];
	verts[0].modulate[3] = mVerts[OLD_MUZZLE].alpha;

//	VectorScale( mVerts[OLD_TIP].curRGB, mVerts[OLD_TIP].curAlpha, color );
	verts[1].modulate[0] = mVerts[OLD_TIP].rgb[0];
	verts[1].modulate[1] = mVerts[OLD_TIP].rgb[1];
	verts[1].modulate[2] = mVerts[OLD_TIP].rgb[2];
	verts[0].modulate[3] = mVerts[OLD_TIP].alpha;

//	VectorScale( mVerts[NEW_TIP].curRGB, mVerts[NEW_TIP].curAlpha, color );
	verts[2].modulate[0] = mVerts[NEW_TIP].rgb[0];
	verts[2].modulate[1] = mVerts[NEW_TIP].rgb[1];
	verts[2].modulate[2] = mVerts[NEW_TIP].rgb[2];
	verts[0].modulate[3] = mVerts[NEW_TIP].alpha;

	verts[0].st[0] = mVerts[OLD_MUZZLE].curST[0];
	verts[0].st[1] = mVerts[OLD_MUZZLE].curST[1];
	verts[1].st[0] = mVerts[OLD_TIP].curST[0];
	verts[1].st[1] = mVerts[OLD_TIP].curST[1];
	verts[2].st[0] = mVerts[NEW_TIP].curST[0];
	verts[2].st[1] = mVerts[NEW_TIP].curST[1];

	// Add this tri
	theFxHelper.AddPolyToScene( mShader, 3, verts );
}

//----------------------------
// Update
//----------------------------
bool CTrail::Update()
{
	// Game pausing can cause dumb time things to happen, so kill the effect in this instance
	if ( mTimeStart > theFxHelper.mTime )
	{
		return false;
	}
		
	float perc = (float)(mTimeEnd - theFxHelper.mTime) / (float)(mTimeEnd - mTimeStart);

	for ( int t = 0; t < 4; t++ )
	{
		/*
		mVerts[t].curAlpha = mVerts[t].alpha * perc + mVerts[t].destAlpha * ( 1.0f - perc );
		if ( mVerts[t].curAlpha < 0.0f )
		{
			mVerts[t].curAlpha = 0.0f;
		}

		VectorScale( mVerts[t].rgb, perc, mVerts[t].curRGB );
		VectorMA( mVerts[t].curRGB, ( 1.0f - perc ), mVerts[t].destrgb, mVerts[t].curRGB );
		*/
		mVerts[t].curST[0] = mVerts[t].ST[0] * perc + mVerts[t].destST[0] * ( 1.0f - perc );
		if ( mVerts[t].curST[0] > 1.0f )
		{
			mVerts[t].curST[0] = 1.0f;
		}
		mVerts[t].curST[1] = mVerts[t].ST[1] * perc + mVerts[t].destST[1] * ( 1.0f - perc );
	}

	return true;
}
//#endif // CHC

//--------------------------
//
// Derived Poly Class
//
//--------------------------

CPoly::CPoly()
{
}

//----------------------------
CPoly::~CPoly()
{
}

//----------------------------
bool CPoly::Cull()
{
	vec3_t	dir;

	// Get the direction to the view
	VectorSubtract( mOrigin1, theFxHelper.refdef.vieworg, dir );

	// Check if it's behind the viewer
	if ( (DotProduct( theFxHelper.refdef.viewaxis[0], dir )) < 0 )
	{
		return true;
	}

	float len = VectorLengthSquared( dir );

	// Can't be too close
	if ( len < 24 * 24 )
	{
		return true;
	}

	return false;
}

//----------------------------
void CPoly::Draw()
{
	polyVert_t	verts[MAX_CPOLY_VERTS];

	for ( int i = 0; i < mCount; i++ )
	{
		// Add our midpoint and vert offset to get the actual vertex
		VectorAdd( mOrigin1, mOrg[i], verts[i].xyz );

		// Assign the same color to each vert
		verts[i].modulate[0] = mRefEnt.shaderRGBA[0];
		verts[i].modulate[1] = mRefEnt.shaderRGBA[1];
		verts[i].modulate[2] = mRefEnt.shaderRGBA[2];
		verts[i].modulate[3] = mRefEnt.shaderRGBA[3];

		// Copy the ST coords
		Vector2Copy( mST[i], verts[i].st );
	}

	// Add this poly
	theFxHelper.AddPolyToScene( mRefEnt.customShader, mCount, verts );
}

//----------------------------
void CPoly::CalcRotateMatrix()
{
	float	cosX, cosZ;
	float	sinX, sinZ;
	float	rad;

	// rotate around Z
	rad = DEG2RAD( mRotDelta[YAW] * theFxHelper.mFrameTime * 0.01f );
	cosZ = cos( rad );
	sinZ = sin( rad );
	// rotate around X
	rad = DEG2RAD( mRotDelta[PITCH] * theFxHelper.mFrameTime * 0.01f );
	cosX = cos( rad );
	sinX = sin( rad );

/*Pitch - aroundx  Yaw - around z
1 0  0			 c -s 0
0 c -s			 s  c 0 
0 s  c			 0  0 1
*/
	mRot[0][0] = cosZ;
	mRot[1][0] = -sinZ;
	mRot[2][0] = 0;
	mRot[0][1] = cosX * sinZ;
	mRot[1][1] = cosX * cosZ;
	mRot[2][1] = -sinX;
	mRot[0][2] = sinX * sinZ;
	mRot[1][2] = sinX * cosZ;
	mRot[2][2] = cosX;
/*
// ROLL is not supported unless anyone complains, if it needs to be added, use this format
Roll

 c 0 s
 0 1 0 
-s 0 c
*/
	mLastFrameTime = theFxHelper.mFrameTime;
}

//--------------------------------
void CPoly::Rotate()
{
	vec3_t	temp[MAX_CPOLY_VERTS];
	float	dif = fabs( mLastFrameTime - theFxHelper.mFrameTime );
	
	if ( dif > 0.1f * mLastFrameTime )
	{
		CalcRotateMatrix();
	}

	// Multiply our rotation matrix by each of the offset verts to get their new position
	for ( int i = 0; i < mCount; i++ )
	{
		VectorRotate( mOrg[i], mRot, temp[i] );
		VectorCopy( temp[i], mOrg[i] );
	}
}

//----------------------------
// Update
//----------------------------
bool CPoly::Update()
{
	// Game pausing can cause dumb time things to happen, so kill the effect in this instance
	if ( mTimeStart > theFxHelper.mTime )
	{
		return false;
	}
		
	UpdateRGB();
	UpdateAlpha();

	// If our timestamp hasn't exired yet, we won't even consider doing any kind of motion
	if ( theFxHelper.mTime > mTimeStamp )
	{
		vec3_t mOldOrigin;

		VectorCopy( mOrigin1, mOldOrigin );

		if ( UpdateOrigin() == false )
		{
			// we are marked for death
			return false;
		}

		// Only rotate whilst moving
		if ( !VectorCompare( mOldOrigin, mOrigin1 ))
		{
			Rotate();
		}
	}

	return true;
}

//----------------------------
void CPoly::PolyInit()
{
	if ( mCount < 3 )
	{
		return;
	}

	int		i;
	vec3_t	org={0,0,0};

	// Find our midpoint
	for ( i = 0; i < mCount; i++ )
	{
		VectorAdd( org, mOrg[i], org );
	}

	VectorScale( org, (float)(1.0f / mCount), org );

	// now store our midpoint for physics purposes
	VectorCopy( org, mOrigin1 );

	// Now we process the passed in points and make it so that they aren't actually the point...
	//	rather, they are the offset from mOrigin1.
	for ( i = 0; i < mCount; i++ )
	{
		VectorSubtract( mOrg[i], mOrigin1, mOrg[i] );
	}

	CalcRotateMatrix();
}

/*
-------------------------
CBezier

Bezier curve line
-------------------------
*/
bool CBezier::Cull( void )
{
	vec3_t	dir;

	VectorSubtract( mOrigin1, theFxHelper.refdef.vieworg, dir );

	//Check if it's in front of the viewer
	if ( (DotProduct( theFxHelper.refdef.viewaxis[0], dir )) >= 0 )
	{
		return false;	//don't cull
	}

	VectorSubtract( mOrigin2, theFxHelper.refdef.vieworg, dir );

	//Check if it's in front of the viewer
	if ( (DotProduct( theFxHelper.refdef.viewaxis[0], dir )) >= 0 )
	{
		return false;
	}

	VectorSubtract( mControl1, theFxHelper.refdef.vieworg, dir );

	//Check if it's in front of the viewer
	if ( (DotProduct( theFxHelper.refdef.viewaxis[0], dir )) >= 0 )
	{
		return false;
	}

	return true; //all points behind viewer
}

//----------------------------
bool CBezier::Update( void )
{
	float	ftime, time2;

	ftime = theFxHelper.mFrameTime * 0.001f;
	time2 = ftime * ftime * 0.5f;

	for ( int i = 0; i < 3; i++ ) 
	{
		mControl1[i] = mControl1[i] + ftime * mControl1Vel[i] + time2 * mControl1Vel[i];
		mControl2[i] = mControl2[i] + ftime * mControl2Vel[i] + time2 * mControl2Vel[i];
	}

	UpdateSize();
	UpdateRGB();
	UpdateAlpha();

	return true;
}

//----------------------------
inline void CBezier::DrawSegment( vec3_t start, vec3_t end, float texcoord1, float texcoord2 )
{
	vec3_t			lineDir, cross, viewDir;
	static vec3_t	lastEnd[2];
	polyVert_t		verts[4];
	float			scale;

	VectorSubtract( end, start, lineDir );
	VectorSubtract( end, theFxHelper.refdef.vieworg, viewDir );
	CrossProduct( lineDir, viewDir, cross );
	VectorNormalize( cross );

	scale = mRefEnt.radius * 0.5f;

	//Construct the oriented quad
	if ( mInit )
	{
		VectorCopy( lastEnd[0], verts[0].xyz );
		VectorCopy( lastEnd[1], verts[1].xyz );
	}
	else
	{
		VectorMA( start, -scale, cross, verts[0].xyz );
		VectorMA( start, scale, cross, verts[1].xyz );
	}

	verts[0].st[0] = 0.0f;
	verts[0].st[1] = texcoord1;

	verts[0].modulate[0] = mRefEnt.shaderRGBA[0] * ( 1.0f - texcoord1 );
	verts[0].modulate[1] = mRefEnt.shaderRGBA[1] * ( 1.0f - texcoord1 );
	verts[0].modulate[2] = mRefEnt.shaderRGBA[2] * ( 1.0f - texcoord1 );
	verts[0].modulate[3] = mRefEnt.shaderRGBA[3];

	verts[1].st[0] = 1.0f;
	verts[1].st[1] = texcoord1;
	verts[1].modulate[0] = mRefEnt.shaderRGBA[0] * ( 1.0f - texcoord1 );
	verts[1].modulate[1] = mRefEnt.shaderRGBA[1] * ( 1.0f - texcoord1 );
	verts[1].modulate[2] = mRefEnt.shaderRGBA[2] * ( 1.0f - texcoord1 );
	verts[1].modulate[3] = mRefEnt.shaderRGBA[3];

	if ( texcoord1 == 0.0f )
	{
		verts[0].modulate[0] = 0;
		verts[0].modulate[1] = 0;
		verts[0].modulate[2] = 0;
		verts[0].modulate[3] = 0;
		verts[1].modulate[0] = 0;
		verts[1].modulate[1] = 0;
		verts[1].modulate[2] = 0;
		verts[1].modulate[3] = 0;
	}

	VectorMA( end, scale, cross, verts[2].xyz );
	verts[2].st[0] = 1.0f;
	verts[2].st[1] = texcoord2;
	verts[2].modulate[0] = mRefEnt.shaderRGBA[0] * ( 1.0f - texcoord2 );
	verts[2].modulate[1] = mRefEnt.shaderRGBA[1] * ( 1.0f - texcoord2 );
	verts[2].modulate[2] = mRefEnt.shaderRGBA[2] * ( 1.0f - texcoord2 );
	verts[2].modulate[3] = mRefEnt.shaderRGBA[3];

	VectorMA( end, -scale, cross, verts[3].xyz );
	verts[3].st[0] = 0.0f;
	verts[3].st[1] = texcoord2;
	verts[3].modulate[0] = mRefEnt.shaderRGBA[0] * ( 1.0f - texcoord2 );
	verts[3].modulate[1] = mRefEnt.shaderRGBA[1] * ( 1.0f - texcoord2 );
	verts[3].modulate[2] = mRefEnt.shaderRGBA[2] * ( 1.0f - texcoord2 );
	verts[3].modulate[3] = mRefEnt.shaderRGBA[3];

	re.AddPolyToScene( mRefEnt.customShader, 4, verts, 1 );

	VectorCopy( verts[2].xyz, lastEnd[1] );
	VectorCopy( verts[3].xyz, lastEnd[0] );

	mInit = true;
}

const	float	BEZIER_RESOLUTION	= 16.0f;

//----------------------------
void CBezier::Draw( void )	
{
	vec3_t	pos, old_pos;
    float	mu, mum1;
	float	incr = 1.0f / BEZIER_RESOLUTION, tex = 1.0f, tc1, tc2;
	int		i;

	VectorCopy( mOrigin1, old_pos );

	mInit = false;	//Signify a new batch for vert gluing

	// Calculate the texture coords so the texture can stretch along the whole bezier
//	if ( mFlags & FXF_WRAP )
//	{
//		tex = m_stScale / 1.0f;
//	}

	float mum13, mu3, group1, group2;

	tc1 = 0.0f;

	for ( mu = incr; mu <= 1.0f; mu += incr )
	{
		//Four point curve
		mum1	= 1 - mu;
		mum13	= mum1 * mum1 * mum1;
		mu3		= mu * mu * mu;
		group1	= 3 * mu * mum1 * mum1;
		group2	= 3 * mu * mu *mum1;

		for ( i = 0; i < 3; i++ )
		{
			pos[i] = mum13 * mOrigin1[i] + group1 * mControl1[i] + group2 * mControl2[i] + mu3 * mOrigin2[i];
		}

//		if ( m_flags & FXF_WRAP ) 
//		{
			tc2 = mu * tex;
//		}
//		else
//		{
//			// Texture will get mapped onto each segement
//			tc1 = 0.0f;
//			tc2 = 1.0f;
//		}

		//Draw it
		DrawSegment( old_pos, pos, tc1, tc2 );

		VectorCopy( pos, old_pos );
		tc1 = tc2;
	}
}

/*
-------------------------
CFlash

Full screen flash
-------------------------
*/

//----------------------------
bool CFlash::Update( void )
{
	UpdateRGB();

	return true;
}

//----------------------------
void CFlash::Init( void )
{
	vec3_t	dif;
	float	mod = 1.0f, dis;

	VectorSubtract( mOrigin1, theFxHelper.refdef.vieworg, dif );
	dis = VectorNormalize( dif );

	mod = DotProduct( dif, theFxHelper.refdef.viewaxis[0] );

	if ( dis > 600 || ( mod < 0.5f && dis > 100 ))
	{
		mod = 0.0f;
	}
	else if ( mod < 0.5f && dis <= 100 )
	{
		mod += 1.1f;
	}

	mod *= (1.0f - ((dis * dis) / (600.0f * 600.0f)));

	VectorScale( mRGBStart, mod, mRGBStart );
	VectorScale( mRGBEnd, mod, mRGBEnd );
}

//----------------------------
void CFlash::Draw( void )	
{
	mRefEnt.reType = RT_SPRITE;

	for ( int i = 0; i < 3; i++ )
	{
		if ( mRefEnt.oldorigin[i] > 1.0f )
		{
			mRefEnt.oldorigin[i] = 1.0f;
		}
		else if ( mRefEnt.oldorigin[i] < 0.0f )
		{
			mRefEnt.oldorigin[i] = 0.0f;
		}
	}
	mRefEnt.shaderRGBA[0] = mRefEnt.oldorigin[0] * 255; 
	mRefEnt.shaderRGBA[1] = mRefEnt.oldorigin[1] * 255; 
	mRefEnt.shaderRGBA[2] = mRefEnt.oldorigin[2] * 255; 
	mRefEnt.shaderRGBA[3] = 255;

	VectorCopy( theFxHelper.refdef.vieworg, mRefEnt.origin );
	VectorMA( mRefEnt.origin, 8, theFxHelper.refdef.viewaxis[0], mRefEnt.origin );
	mRefEnt.radius = 7.0f;

	theFxHelper.AddFxToScene( &mRefEnt );
}

void FX_FeedTrail(effectTrailArgStruct_t *a)
{
	CTrail *fx = new CTrail;
	int i = 0;

	while (i < 4)
	{
		VectorCopy(a->mVerts[i].origin, fx->mVerts[i].origin);
		VectorCopy(a->mVerts[i].rgb, fx->mVerts[i].rgb);
		VectorCopy(a->mVerts[i].destrgb, fx->mVerts[i].destrgb);
		VectorCopy(a->mVerts[i].curRGB, fx->mVerts[i].curRGB);
		fx->mVerts[i].alpha = a->mVerts[i].alpha;
		fx->mVerts[i].destAlpha = a->mVerts[i].destAlpha;
		fx->mVerts[i].curAlpha = a->mVerts[i].curAlpha;
		fx->mVerts[i].ST[0] = a->mVerts[i].ST[0];
		fx->mVerts[i].ST[1] = a->mVerts[i].ST[1];
		fx->mVerts[i].destST[0] = a->mVerts[i].destST[0];
		fx->mVerts[i].destST[1] = a->mVerts[i].destST[1];
		fx->mVerts[i].curST[0] = a->mVerts[i].curST[0];
		fx->mVerts[i].curST[1] = a->mVerts[i].curST[1];
		i++;
	}

	fx->SetFlags(a->mSetFlags);

	fx->mShader = a->mShader;

	FX_AddPrimitive((CEffect **)&fx, NULL, a->mKillTime);
}
