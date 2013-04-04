#if !defined(G2_H_INC)
	#include "../ghoul2/ghoul2_shared.h"
#endif

#if !defined(FX_SYSTEM_H_INC)
	#include "FxSystem.h"
#endif

#ifndef FX_PRIMITIVES_H_INC
#define FX_PRIMITIVES_H_INC

#ifdef CHC
	#define MAX_EFFECTS			1024
#else // anything else
	#define MAX_EFFECTS			1800
#endif

// Generic group flags, used by parser, then get converted to the appropriate specific flags
#define FX_PARM_MASK		0xC	// use this to mask off any transition types that use a parm
#define FX_GENERIC_MASK		0xF
#define FX_LINEAR			0x1
#define FX_RAND				0x2
#define FX_NONLINEAR		0x4
#define FX_WAVE				0x8
#define FX_CLAMP			0xC

// Group flags
#define FX_ALPHA_SHIFT		0
#define FX_ALPHA_PARM_MASK	0x0000000C
#define	FX_ALPHA_LINEAR		0x00000001
#define FX_ALPHA_RAND		0x00000002
#define	FX_ALPHA_NONLINEAR	0x00000004
#define FX_ALPHA_WAVE		0x00000008
#define FX_ALPHA_CLAMP		0x0000000C

#define FX_RGB_SHIFT		4
#define FX_RGB_PARM_MASK	0x000000C0
#define	FX_RGB_LINEAR		0x00000010
#define FX_RGB_RAND			0x00000020
#define	FX_RGB_NONLINEAR	0x00000040
#define FX_RGB_WAVE			0x00000080
#define FX_RGB_CLAMP		0x000000C0

#define FX_SIZE_SHIFT		8
#define FX_SIZE_PARM_MASK	0x00000C00
#define	FX_SIZE_LINEAR		0x00000100
#define FX_SIZE_RAND		0x00000200
#define	FX_SIZE_NONLINEAR	0x00000400
#define FX_SIZE_WAVE		0x00000800
#define FX_SIZE_CLAMP		0x00000C00

#define FX_LENGTH_SHIFT		12
#define FX_LENGTH_PARM_MASK	0x0000C000
#define	FX_LENGTH_LINEAR	0x00001000
#define FX_LENGTH_RAND		0x00002000
#define	FX_LENGTH_NONLINEAR	0x00004000
#define FX_LENGTH_WAVE		0x00008000
#define FX_LENGTH_CLAMP		0x0000C000

#define FX_SIZE2_SHIFT		16
#define FX_SIZE2_PARM_MASK	0x000C0000
#define	FX_SIZE2_LINEAR		0x00010000
#define FX_SIZE2_RAND		0x00020000
#define	FX_SIZE2_NONLINEAR	0x00040000
#define FX_SIZE2_WAVE		0x00080000
#define FX_SIZE2_CLAMP		0x000C0000

// Feature flags
#define	FX_DEPTH_HACK		0x00100000
#define	FX_RELATIVE			0x00200000
#define	FX_SET_SHADER_TIME	0x00400000		// when the shader time gets set in-game, animating textures start animating at the first frame
#define FX_EXPENSIVE_PHYSICS 0x00800000

#define FX_ATTACHED_MODEL	0x01000000

#define FX_APPLY_PHYSICS	0x02000000
#define FX_USE_BBOX			0x04000000		// can make physics more accurate at the expense of speed

#define FX_USE_ALPHA		0x08000000		// the FX system actually uses RGB to do fades, but this will override that
											//	and cause it to fill in the alpha.

#define FX_EMIT_FX			0x10000000		// emitters technically don't have to emit stuff, but when they do
											//	this flag needs to be set
#define FX_DEATH_RUNS_FX	0x20000000		// Normal death triggers effect, but not kill_on_impact
#define FX_KILL_ON_IMPACT	0x40000000		// works just like it says, but only when physics are on.
#define FX_IMPACT_RUNS_FX	0x80000000		// an effect can call another effect when it hits something.

// Lightning flags, duplicates of existing flags, but lightning doesn't use those flags in that context...and nothing will ever use these in this context..so we are safe.
#define FX_TAPER			0x01000000		// tapers as it moves towards its endpoint
#define FX_BRANCH			0x02000000		// enables lightning branching
#define FX_GROW				0x04000000		// lightning grows from start point to end point over the course of its life

class CFxBoltInterface
{
private:
	bool				mIsValid;	
	vec3_t				mOrg;
	vec3_t				mFwd;
	vec3_t				mScale;
	CGhoul2Info_v		*mG2Inst;
	int					mModelNum;
	int					mBoltNum;
	int					mEntNum;

	vec3_t				mInitialPartOrg;
	vec3_t				mInitialOffset;

public:
	
	CFxBoltInterface()	{ mIsValid = true; };
	virtual ~CFxBoltInterface()	{};

	void GetOrigin( vec3_t org ) { VectorCopy(mOrg, org); };	// the fx system will call this to get the new origin
	void GetForward( vec3_t dir ) { VectorCopy(mFwd, dir); };	// and this to get the new forward.  Getting the whole axis is not necessary		
	void GetScale( vec3_t scale ) { VectorCopy(mScale, scale); };	// and this to get the new forward.  Getting the whole axis is not necessary		

	void GetInitialPartOrg( vec3_t org ) { VectorCopy(mInitialPartOrg, org); };
	void GetInitialOffset( vec3_t org ) { VectorCopy(mInitialOffset, org); };

	CGhoul2Info_v *GetG2Handle( void ) { return mG2Inst; }
	int GetModelNum( void ) { return mModelNum; }
	int GetBoltNum( void ) { return mBoltNum; }
	int GetEntNum( void ) { return mEntNum; }

	void SetOrigin( vec3_t org ) { VectorCopy(org, mOrg); };	// the fx system will call this to get the new origin
	void SetForward( vec3_t dir ) { VectorCopy(dir, mFwd); };	// and this to get the new forward.  Getting the whole axis is not necessary		
	void SetScale( vec3_t scale ) { VectorCopy(scale, mScale); };	// and this to get the new forward.  Getting the whole axis is not necessary		

	void SetInitialPartOrg( vec3_t org ) { VectorCopy(org, mInitialPartOrg); };
	void SetInitialOffset( vec3_t org ) { VectorCopy(org, mInitialOffset); };

	void SetG2Handle( CGhoul2Info_v *g2Inst ) { mG2Inst = g2Inst; }
	void SetModelNum( int modelNum ) { mModelNum = modelNum; }
	void SetBoltNum( int boltNum ) { mBoltNum = boltNum; }
	void SetEntNum( int entNum ) { mEntNum = entNum; }

	bool IsValid( void )	{ return mIsValid;	}
	void Invalidate( void )	{ mIsValid = false;	}	// If the thing we are bolted to is no longer valid, clue up the fx system so it doesn't try to do dumb things
	void Validate() { mIsValid = true; }
};

//------------------------------
class CEffect
{
protected:

	vec3_t		mOrigin1;

	int			mTimeStart;
	int			mTimeEnd;

	int			mFlags;

	// Size of our object, useful for things that have physics
	vec3_t		mMin;
	vec3_t		mMax;

	int			mImpactFxID;		// if we have an impact event, we may have to call an effect
	int			mDeathFxID;			// if we have a death event, we may have to call an effect

	int			mKillTime;

	CFxBoltInterface mBoltInterface;
	bool			mBoltInterfaceValid;

	miniRefEntity_t		mRefEnt;
	CEffect				*mNext;

public:

	CEffect();
	virtual ~CEffect();

	virtual void Die();

	virtual bool Update();
	virtual bool Cull();
	virtual void Draw();

	inline	miniRefEntity_t	&GetRefEnt(void) { return mRefEnt; } 

	inline void		SetNext(CEffect *Next) { mNext = Next; }
	inline CEffect	*GetNext(void) { return mNext; }
	inline void		SetKillTime(int KillTime) { mKillTime = KillTime; }
	inline int		GetKillTime(void) { return mKillTime; }
	inline int		GetFlags(void) { return mFlags; }

	inline void SetSTScale(float s,float t)	{ mRefEnt.shaderTexCoord[0]=s;mRefEnt.shaderTexCoord[1]=t;}

	inline void SetMin( vec3_t min )		{ if(min){VectorCopy(min,mMin);}else{VectorClear(mMin);}			}
	inline void SetMax( vec3_t max )		{ if(max){VectorCopy(max,mMax);}else{VectorClear(mMax);}			}
	inline void SetFlags( int flags )		{ mFlags = flags;				}
	inline void AddFlags( int flags )		{ mFlags |= flags;				}
	inline void ClearFlags( int flags )		{ mFlags &= ~flags;				}
	inline void SetOrigin1( vec3_t org )	{ if(org){VectorCopy(org,mOrigin1);}else{VectorClear(mOrigin1);}	}
	inline void SetTimeStart( int time )	{ mTimeStart = time; if (mFlags&FX_SET_SHADER_TIME) { mRefEnt.shaderTime = time * 0.001f; }}
	inline void	SetTimeEnd( int time )		{ mTimeEnd = time;				}
	inline void SetImpactFxID( int id )		{ mImpactFxID = id;				}
	inline void SetDeathFxID( int id )		{ mDeathFxID = id;				}
	inline bool IsBoltInterfaceValid()		{ return mBoltInterfaceValid; }
	inline CFxBoltInterface *GetBIPointer()	{ return &mBoltInterface; }
	inline void GetOrigin1( vec3_t put_to )	{ VectorCopy(mOrigin1, put_to); }
	inline void ForceOrigin1( vec3_t shove_in ) { VectorCopy(shove_in, mOrigin1); }

	inline void SetBoltInterface(CFxBoltInterface *obj)
	{
		vec3_t store;

		mBoltInterface.SetEntNum(obj->GetEntNum());
		mBoltInterface.SetBoltNum(obj->GetBoltNum());
		mBoltInterface.SetModelNum(obj->GetModelNum());
		mBoltInterface.SetG2Handle(obj->GetG2Handle());

		obj->GetForward(store);
		mBoltInterface.SetForward(store);
		obj->GetOrigin(store);
		mBoltInterface.SetOrigin(store);
		obj->GetScale(store);
		mBoltInterface.SetScale(store);
		obj->GetInitialOffset(store);
		mBoltInterface.SetInitialOffset(store);
		obj->GetInitialPartOrg(store);
		mBoltInterface.SetInitialPartOrg(store);

		mBoltInterfaceValid = true;
	}
};

class CCloud : public CEffect
{
private:
	int			mNumPending;

public:
	CCloud();
	virtual ~CCloud();

			void IncreasePending(void) { mNumPending++; }
			void DecreasePending(void) { mNumPending--; }
	virtual void Die();

	virtual bool Update();
	virtual bool Cull();
	virtual void Draw();

	void	AddEffect(CEffect *which);
};

//#ifdef CHC
//---------------------------------------------------
// This class is kind of an exception to the "rule".
//	For now it exists only for allowing an easy way
//	to get the saber slash trails rendered.
//---------------------------------------------------
class CTrail : public CEffect
{
// This is such a specific case thing, just grant public access to the goods.
public:

	typedef struct
	{
		vec3_t	origin;

		// very specifc case, we can modulate the color and the alpha
		vec3_t	rgb;
		vec3_t	destrgb;
		vec3_t	curRGB;

		float	alpha;
		float	destAlpha;
		float	curAlpha;

		// this is a very specific case thing...allow interpolating the st coords so we can map the texture
		//	properly as this segement progresses through it's life
		float	ST[2];
		float	destST[2];
		float	curST[2];

	} TVert;

	TVert		mVerts[4];
	qhandle_t	mShader;


	CTrail()			{};
	virtual ~CTrail()	{};

	virtual bool Update();
	virtual bool Cull();
	virtual void Draw();
};
//#endif // CHC

//------------------------------
class CLight : public CEffect
{
protected:

	float		mSizeStart;
	float		mSizeEnd;
	float		mSizeParm;

	vec3_t		mRGBStart;
	vec3_t		mRGBEnd;
	float		mRGBParm;


	void		UpdateSize();
	void		UpdateRGB();

public:

	CLight();
	virtual ~CLight();

	virtual bool Update();
	virtual bool Cull();
	virtual void Draw();

	inline void SetSizeStart( float sz )	{ mSizeStart = sz;			}
	inline void SetSizeEnd( float sz )		{ mSizeEnd = sz;			}
	inline void SetSizeParm( float parm )	{ mSizeParm = parm;			}

	inline void SetRGBStart( vec3_t rgb )	{ if(rgb){VectorCopy(rgb,mRGBStart);}else{VectorClear(mRGBStart);}	}
	inline void SetRGBEnd( vec3_t rgb )		{ if(rgb){VectorCopy(rgb,mRGBEnd);}else{VectorClear(mRGBEnd);}		}
	inline void SetRGBParm( float parm )	{ mRGBParm = parm;			}
};

//------------------------------
class CFlash : public CLight
{
public:

	CFlash() {}
	virtual ~CFlash() {}

	virtual bool Cull() { return false; }
	virtual bool Update();
	virtual void Draw();

	inline void SetShader( qhandle_t sh )	{ mRefEnt.customShader = sh;				}
	void		Init( void );
};


//------------------------------
class CParticle : public CEffect
{
protected:

	vec3_t		mVel;
	vec3_t		mAccel;

	float		mSizeStart;
	float		mSizeEnd;
	float		mSizeParm;

	vec3_t		mRGBStart;
	vec3_t		mRGBEnd;
	float		mRGBParm;
	
	float		mAlphaStart;
	float		mAlphaEnd;
	float		mAlphaParm;

	float		mRotationDelta;
	float		mElasticity;


	bool		UpdateOrigin();
	void		UpdateVelocity();
	void		UpdateSize();
	void		UpdateRGB();
	void		UpdateAlpha();
	void		UpdateRotation();

#ifndef EFFECTSED
#ifndef CHC
	CFxBoltInterface	*mObj;
	CFxBoltInterface	storedObj;
#endif
#endif

public:

	CParticle();
	virtual ~CParticle();

	virtual void Die();

	virtual bool Update();
	virtual bool Cull();
	virtual void Draw();

	inline void SetShader( qhandle_t sh )	{ mRefEnt.customShader = sh;				}

	inline void SetVel( vec3_t vel )		{ if(vel){VectorCopy(vel,mVel);}else{VectorClear(mVel);}	}
	inline void SetAccel( vec3_t ac )		{ if(ac){VectorCopy(ac,mAccel);}else{VectorClear(mAccel);}	}

	inline void SetSizeStart( float sz )	{ mSizeStart = sz;			}
	inline void SetSizeEnd( float sz )		{ mSizeEnd = sz;			}
	inline void SetSizeParm( float parm )	{ mSizeParm = parm;			}

	inline void SetRGBStart( vec3_t rgb )	{ if(rgb){VectorCopy(rgb,mRGBStart);}else{VectorClear(mRGBStart);}	}
	inline void SetRGBEnd( vec3_t rgb )		{ if(rgb){VectorCopy(rgb,mRGBEnd);}else{VectorClear(mRGBEnd);}		}
	inline void SetRGBParm( float parm )	{ mRGBParm = parm;			}

	inline void SetAlphaStart( float al )	{ mAlphaStart = al;			}
	inline void SetAlphaEnd( float al )		{ mAlphaEnd = al;			}
	inline void SetAlphaParm( float parm )	{ mAlphaParm = parm;		}

	inline void SetRotation( float rot )		{ mRefEnt.rotation = rot;	}
	inline void SetRotationDelta( float rot )	{ mRotationDelta = rot;		}
	inline void SetElasticity( float el )		{ mElasticity = el;			}

#ifndef EFFECTSED
#ifndef CHC
	inline void SetObj( CFxBoltInterface *obj )	{ mObj = obj;				}

	inline void SetObjNew( CFxBoltInterface *obj )
	{ //This is necessary when we want to copy the contents of a temporary variable and use them in a scheduled effect
		vec3_t store;

		mObj = &storedObj;

		mObj->SetEntNum(obj->GetEntNum());
		mObj->SetBoltNum(obj->GetBoltNum());
		mObj->SetModelNum(obj->GetModelNum());
		mObj->SetG2Handle(obj->GetG2Handle());

		obj->GetForward(store);
		mObj->SetForward(store);
		obj->GetOrigin(store);
		mObj->SetOrigin(store);
		obj->GetScale(store);
		mObj->SetScale(store);
		obj->GetInitialOffset(store);
		mObj->SetInitialOffset(store);
		obj->GetInitialPartOrg(store);
		mObj->SetInitialPartOrg(store);
	}
#endif
#endif
};


//------------------------------
class CLine : public CParticle
{
protected:

	vec3_t	mOrigin2;

public:

	CLine();
	virtual ~CLine();

	virtual void Die();

	virtual bool Update();
	virtual bool Cull();
	virtual void Draw();

	inline void SetOrigin2( vec3_t org2 )	{ VectorCopy( org2, mOrigin2 ); }
};

//------------------------------
class CBezier : public CLine
{
protected:

	vec3_t	mControl1;
	vec3_t	mControl1Vel;

	vec3_t	mControl2;
	vec3_t	mControl2Vel;

	bool	mInit;

public:

	CBezier(){ mInit = false; }
	virtual ~CBezier() {}
	virtual void Die() {}

	virtual bool Update();
	virtual bool Cull();
	virtual void Draw();
	
	void DrawSegment( vec3_t start, vec3_t end, float texcoord1, float texcoord2 );

	inline void SetControlPoints( vec3_t ctrl1, vec3_t ctrl2 )	{ VectorCopy( ctrl1, mControl1 ); VectorCopy( ctrl2, mControl2 ); }
	inline void SetControlVel( vec3_t ctrl1v, vec3_t ctrl2v )	{ VectorCopy( ctrl1v, mControl1Vel ); VectorCopy( ctrl2v, mControl2Vel ); }
};

//------------------------------
class CElectricity : public CLine
{
protected:

	float	mChaos;

public:

	CElectricity();
	virtual ~CElectricity();

	virtual void Die();

	virtual bool Update();
	virtual bool Cull();
	virtual void Draw();

	void Initialize();

	inline void SetChaos( float chaos )		{ mChaos = chaos; }
};


// Oriented quad
//------------------------------
class COrientedParticle : public CParticle
{
protected:

	vec3_t	mNormal;

public:

	COrientedParticle();
	virtual ~COrientedParticle();

	virtual bool Update();
	virtual bool Cull();
	virtual void Draw();

	inline void SetNormal( vec3_t norm )	{ VectorCopy( norm, mNormal );	}
};

//------------------------------
class CTail : public CParticle
{
protected:

	vec3_t	mOldOrigin;

	float	mLengthStart;
	float	mLengthEnd;
	float	mLengthParm;

	float	mLength;

	void	UpdateLength();
	void	CalcNewEndpoint();

public:

	CTail();
	virtual ~CTail();

	virtual bool Update();
	virtual bool Cull();
	virtual void Draw();

	inline void SetLengthStart( float len )	{ mLengthStart = len;	}
	inline void SetLengthEnd( float len )	{ mLengthEnd = len;	}
	inline void SetLengthParm( float len )	{ mLengthParm = len;	}
};


//------------------------------
class CCylinder : public CTail
{
protected:

	float		mSize2Start;
	float		mSize2End;
	float		mSize2Parm;

	void	UpdateSize2();

public:

	CCylinder();
	virtual ~CCylinder();

	virtual bool Update();
	virtual bool Cull();
	virtual void Draw();


	inline void SetSize2Start( float sz )	{ mSize2Start = sz;			}
	inline void SetSize2End( float sz )		{ mSize2End = sz;			}
	inline void SetSize2Parm( float parm )	{ mSize2Parm = parm;		}

	inline void SetNormal( vec3_t norm )	{ VectorCopy( norm, mRefEnt.axis[0] ); }
};


//------------------------------
// Emitters are derived from particles because, although they don't draw, any effect called
//	from them can borrow an initial or ending value from the emitters current alpha, rgb, etc..
class CEmitter : public CParticle
{
protected:

	vec3_t		mOldOrigin;		// we use these to do some nice
	vec3_t		mLastOrigin;	//	tricks...
	vec3_t		mOldVelocity;	//	
	int			mOldTime;

	vec3_t		mAngles;		// for a rotating thing, using a delta  
	vec3_t		mAngleDelta;	//	as opposed to an end angle is probably much easier

	int			mEmitterFxID;	// if we have emitter fx, this is our id

	float		mDensity;		// controls how often emitter chucks an effect
	float		mVariance;		// density sloppiness

	void		UpdateAngles();

public:

	CEmitter();
	virtual ~CEmitter();

	virtual bool Update();
	virtual bool Cull();
	virtual void Draw();

	inline void SetModel( qhandle_t model )	{ mRefEnt.hModel = model;	}
	inline void SetAngles( vec3_t ang )		{ if(ang){VectorCopy(ang,mAngles);}else{VectorClear(mAngles);}				}
	inline void SetAngleDelta( vec3_t ang )	{ if(ang){VectorCopy(ang,mAngleDelta);}else{VectorClear(mAngleDelta);}		}
	inline void SetEmitterFxID( int id )	{ mEmitterFxID = id;		}
	inline void SetDensity( float density )	{ mDensity = density;		}
	inline void SetVariance( float var )	{ mVariance = var;			}
	inline void SetOldTime( int time )		{ mOldTime = time;			}
	inline void SetLastOrg( vec3_t org )	{ if(org){VectorCopy(org,mLastOrigin);}else{VectorClear(mLastOrigin);}		}
	inline void SetLastVel( vec3_t vel )	{ if(vel){VectorCopy(vel,mOldVelocity);}else{VectorClear(mOldVelocity);}	}

};

// We're getting pretty low level here, not the kind of thing to abuse considering how much overhead this
//	adds to a SINGLE triangle or quad....
// The editor doesn't need to see or do anything with this
//------------------------------
#define MAX_CPOLY_VERTS	5

class CPoly : public CParticle
{
protected:

	int		mCount;
	vec3_t	mRotDelta;
	int		mTimeStamp;

public:

	vec3_t	mOrg[MAX_CPOLY_VERTS];
	vec2_t	mST[MAX_CPOLY_VERTS];

	float	mRot[3][3];
	int		mLastFrameTime;


	CPoly();
	virtual ~CPoly();

	virtual bool Update();
	virtual bool Cull();
	virtual void Draw();

	void PolyInit();
	void CalcRotateMatrix();
	void Rotate();

	inline void SetNumVerts( int c )					{ mCount = c;			}
	inline void SetRot( vec3_t r )						{ if(r){VectorCopy(r,mRotDelta);}else{VectorClear(mRotDelta);}}
	inline void SetMotionTimeStamp( int t )				{ mTimeStamp = theFxHelper.GetTime() + t; }
	inline int	GetMotionTimeStamp()					{ return mTimeStamp; }
};


#endif //FX_PRIMITIVES_H_INC