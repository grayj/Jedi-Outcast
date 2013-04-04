#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#if !defined(GHOUL2_SHARED_H_INC)
#define GHOUL2_SHARED_H_INC

/*
Ghoul2 Insert Start
*/
#pragma warning (push, 3)	//go back down to 3 for the stl include
#include <vector>
#include <map>
#pragma warning (pop)
using namespace std;
/*
Ghoul2 Insert End
*/

//===================================================================
//
//   G H O U L  I I  D E F I N E S
//
// we save the whole surfaceInfo_t struct
struct surfaceInfo_t 
{
	int			offFlags;		// what the flags are for this model
	int			surface;		// index into array held inside the model definition of pointers to the actual surface data loaded in - used by both client and game
	float		genBarycentricJ;	// point 0 barycentric coors
	float		genBarycentricI;	// point 1 barycentric coors - point 2 is 1 - point0 - point1
	int			genPolySurfaceIndex; // used to point back to the original surface and poly if this is a generated surface
	int			genLod;			// used to determine original lod of original surface and poly hit location
	
surfaceInfo_t():
	offFlags(0),
	surface(0),
	genBarycentricJ(0),
	genBarycentricI(0),
	genPolySurfaceIndex(0),
	genLod(0)
	{}

};



#define MDXABONEDEF				// used in the mdxformat.h file to stop redefinitions of the bone struct.

// we save the whole structure here.
struct  boneInfo_t
{
	int			boneNumber;		// what bone are we overriding?
	mdxaBone_t	matrix;			// details of bone angle overrides - some are pre-done on the server, some in ghoul2
	int			flags;			// flags for override
	int			startFrame;		// start frame for animation
	int			endFrame;		// end frame for animation NOTE anim actually ends on endFrame+1
	int			startTime;		// time we started this animation
	int			pauseTime;		// time we paused this animation - 0 if not paused
	float		animSpeed;		// speed at which this anim runs. 1.0f means full speed of animation incoming - ie if anim is 20hrtz, we run at 20hrts. If 5hrts, we run at 5 hrts
	float		blendFrame;		// frame PLUS LERP value to blend from
	int			blendLerpFrame;	// frame to lerp the blend frame with.
	int			blendTime;		// Duration time for blending - used to calc amount each frame of new anim is blended with last frame of the last anim
	int			blendStart;		// Time when blending starts - not necessarily the same as startTime since we might start half way through an anim
	int			boneBlendTime;	// time for duration of bone angle blend with normal animation
	int			boneBlendStart;	// time bone angle blend with normal animation began
	int			lastTime;		// this does not go across the network
	mdxaBone_t	newMatrix;		// This is the lerped matrix that Ghoul2 uses on the client side - does not go across the network

boneInfo_t():
	boneNumber(-1),
	flags(0),
	startFrame(0),
	endFrame(0),
	startTime(0),
	pauseTime(0),
	animSpeed(0),
	blendFrame(0),
	blendLerpFrame(0),
	blendTime(0),
	blendStart(0),
	boneBlendTime(0),
	boneBlendStart(0),
	lastTime(0)
	{
		matrix.matrix[0][0] = matrix.matrix[0][1] = matrix.matrix[0][2] = matrix.matrix[0][3] = 
		matrix.matrix[1][0] = matrix.matrix[1][1] = matrix.matrix[1][2] = matrix.matrix[1][3] = 
		matrix.matrix[2][0] = matrix.matrix[2][1] = matrix.matrix[2][2] = matrix.matrix[2][3] = 0.0f;
	}

};
//we save from top to boltUsed here. Don't bother saving the position, it gets rebuilt every frame anyway
struct boltInfo_t{
	int			boneNumber;		// bone number bolt attaches to	
	int			surfaceNumber;	// surface number bolt attaches to 
	int			surfaceType;	// if we attach to a surface, this tells us if it is an original surface or a generated one - doesn't go across the network
	int			boltUsed;		// nor does this
	mdxaBone_t	position;		// this does not go across the network
	boltInfo_t():
	boneNumber(-1),
	surfaceNumber(-1),
	surfaceType(0),
	boltUsed(0)
	{}
};

#ifdef _SOF2
typedef enum
{
	PGORE_NONE,
	PGORE_ARMOR,
	PGORE_BULLETSMALL,
	PGORE_BULLETMED,
	PGORE_BULLETBIG,
	PGORE_HEGRENADE,
	PGORE_COUNT
} goreEnum_t;

struct goreEnumShader_t
{
	goreEnum_t		shaderEnum;
	char			shaderName[MAX_QPATH];
};

struct SSkinGoreData
{
	vec3_t			angles;
	vec3_t			position;
	int				currentTime;
	int				entNum;
	vec3_t			rayDirection;	// in world space
	vec3_t			hitLocation;	// in world space
	vec3_t			scale;
	float			SSize;			// size of splotch in the S texture direction in world units
	float			TSize;			// size of splotch in the T texture direction in world units
	float			theta;			// angle to rotate the splotch

//	qhandle_t		shader;			// handle to shader for gore, this better be rendered after the shader of the underlying surface					
									// this shader should also have "clamp" mode, not tiled.
	goreEnum_t		shaderEnum;		// enum that'll get switched over to the shader's actual handle
};
#endif // _SOF2

#define MAX_GHOUL_COUNT_BITS 8 // bits required to send across the MAX_G2_MODELS inside of the networking - this is the only restriction on ghoul models possible per entity

typedef vector <surfaceInfo_t> surfaceInfo_v;
typedef vector <boneInfo_t> boneInfo_v;
typedef vector <boltInfo_t> boltInfo_v;
typedef vector <pair<int,mdxaBone_t> > mdxaBone_v;

// defines for stuff to go into the mflags
#define		GHOUL2_NOCOLLIDE 0x001
#define		GHOUL2_NORENDER	 0x002
#define		GHOUL2_NOMODEL	 0x004
#define		GHOUL2_NEWORIGIN 0x008


// NOTE order in here matters. We save out from mModelindex to mFlags, but not the STL vectors that are at the top or the bottom.
class CGhoul2Info
{
public:
	surfaceInfo_v 	mSlist;
	boltInfo_v		mBltlist;
	boneInfo_v		mBlist;
// save from here
	int				mModelindex;
	qhandle_t		mCustomShader;
	qhandle_t		mCustomSkin;
	int				mModelBoltLink;
	int				mSurfaceRoot;
	int				mCreationID;
	int				mLodBias;
	int				mNewOrigin;	// this contains the bolt index of the new origin for this model
#ifdef _SOF2
	int				mGoreSetTag;
#endif // _SOF2
	qhandle_t		mModel;		// this and the next entries do NOT go across the network. They are for gameside access ONLY
	char			mFileName[MAX_QPATH];
	int				mAnimFrameDefault;
	int				mSkelFrameNum;
	int				mMeshFrameNum;
	int				mFlags;	// used for determining whether to do full collision detection against this object
// to here
	int				*mTransformedVertsArray;	// used to create an array of pointers to transformed verts per surface for collision detection
	mdxaBone_v		mTempBoneList;
	int				mSkin;

	CGhoul2Info():
	mModelindex(-1),
	mCustomShader(0),
	mCustomSkin(0),
	mModelBoltLink(0),
	mModel(0),
	mSurfaceRoot(0),
	mAnimFrameDefault(0),
	mSkelFrameNum(-1),
	mMeshFrameNum(-1),
	mCreationID(0),
	mFlags(0),
	mTransformedVertsArray(0),
	mLodBias(0),
	mSkin(0),
	mNewOrigin(-1)
	{
		mFileName[0] = 0;
	}
}; 

typedef vector <CGhoul2Info> CGhoul2Info_v;

// collision detection stuff
#define G2_FRONTFACE 1
#define	G2_BACKFACE	 0


class CCollisionRecord
{
public:
	float		mDistance;
	int			mEntityNum;
	int			mModelIndex;
	int			mPolyIndex;
	int			mSurfaceIndex;
	vec3_t		mCollisionPosition;
	vec3_t		mCollisionNormal;
	int			mFlags;
	int			mMaterial;
	int			mLocation;
	float		mBarycentricI; // two barycentic coodinates for the hit point
	float		mBarycentricJ; // K = 1-I-J

	CCollisionRecord():
	mEntityNum(-1),
	mDistance(100000)
	{}
};

// calling defines for the trace function
enum EG2_Collision
{
	G2_NOCOLLIDE,
	G2_COLLIDE,
	G2_RETURNONHIT
};

//typedef map < float, CCollisionRecord > CCollisionRecord_m;

enum EWraithInstFlags
{
	WF_BASEMODEL =					(1<<0),
	WF_CLIENTONLY =					(1<<1),
	WF_SERVERONLY =					(1<<2),
	WF_NPC =						(1<<3),
};


//====================================================================

#endif // GHOUL2_SHARED_H_INC
