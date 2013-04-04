// leave this as first line for PCH reasons...
//

 
#include "../client/client.h"	//FIXME!! EVIL - just include the definitions needed 


						  
#if !defined(TR_LOCAL_H)
	#include "tr_local.h"
#endif

#include "MatComp.h"
#if !defined(_QCOMMON_H_)
	#include "../qcommon/qcommon.h"
#endif
#if !defined(G2_H_INC)
	#include "../ghoul2/G2.h"
#endif
#include "../ghoul2/G2_local.h"
#include "MatComp.h"

#pragma warning (disable: 4512)	//default assignment operator could not be gened
#include "../qcommon/disablewarnings.h"

#define	LL(x) x=LittleLong(x)

extern cvar_t	*r_Ghoul2AnimSmooth;
extern cvar_t	*r_Ghoul2UnSqashAfterSmooth;

// I hate doing this, but this is the simplest way to get this into the routines it needs to be
mdxaBone_t		worldMatrix;
mdxaBone_t		worldMatrixInv;

class CConstructBoneList
{
public:
	int				surfaceNum;
	int				*boneUsedList;
	surfaceInfo_v	&rootSList;
	model_t			*currentModel;
	boneInfo_v		&boneList; 

	CConstructBoneList(
	int				initsurfaceNum,
	int				*initboneUsedList,
	surfaceInfo_v	&initrootSList,
	model_t			*initcurrentModel,
	boneInfo_v		&initboneList):

	surfaceNum(initsurfaceNum),
	boneUsedList(initboneUsedList),
	rootSList(initrootSList),
	currentModel(initcurrentModel),
	boneList(initboneList) { }

};

// optimised structures, so we aren't passing all these variables down the transform recursion pipe with each call.
class CTransformBone
{
public:
	int				newFrame;
	int				currentFrame;
	int				parent;
	int				child;
	int				frameSize;
	mdxaHeader_t	*header;
	float			backlerp;
	int				*usedBoneList;
	boneInfo_v		&rootBoneList;
	mdxaBone_v		&bonePtr;
	boltInfo_v		&boltList;
	mdxaBone_t		&rootMatrix;
	bool			rootBone;
	int				incomingTime;
	float			blendFrame;
	int				blendOldFrame;
	bool			blendMode;
	float			blendLerp;
	int				boltNum;
	vec3_t			newModelOrigin;

	CTransformBone(
	int				initnewFrame,
	int				initcurrentFrame,
	int				initparent,
	int				initchild,
	int				initframeSize,
	mdxaHeader_t	*initheader,
	float			initbacklerp,
	int				*initusedBoneList,
	boneInfo_v		&initrootBoneList,
	mdxaBone_v		&initbonePtr,
	boltInfo_v		&initboltList,
	mdxaBone_t		&initrootMatrix,
	bool			initrootBone,
	int				initincomingTime,
	float			initblendFrame,
	int				initblendOldFrame,
	bool			initblendMode,
	float			initblendLerp,
	int				initboltNum):

	newFrame(initnewFrame),
	currentFrame(initcurrentFrame),
	parent(initparent),
	child(initchild),
	frameSize(initframeSize),
	header(initheader),
	backlerp(initbacklerp),
	usedBoneList(initusedBoneList),
	rootBoneList(initrootBoneList),
	bonePtr(initbonePtr),
	boltList(initboltList),
	rootMatrix(initrootMatrix),
	rootBone(initrootBone),
	incomingTime(initincomingTime),
	blendFrame(initblendFrame),
	blendOldFrame(initblendOldFrame),
	blendMode(initblendMode),
	blendLerp(initblendLerp),
	boltNum(initboltNum)
	{
		VectorClear(newModelOrigin);
	}
};

class CRenderSurface
{
public:
	int				surfaceNum;
	surfaceInfo_v	&rootSList;
	shader_t		*cust_shader;
	int				fogNum;
	qboolean		personalModel;
	mdxaBone_v		&bonePtr;
	int				renderfx;
	skin_t			*skin;
	model_t			*currentModel;
	int				lod;
	boltInfo_v		&boltList;

	CRenderSurface(
	int				initsurfaceNum,
	surfaceInfo_v	&initrootSList,
	shader_t		*initcust_shader,
	int				initfogNum,
	qboolean		initpersonalModel,
	mdxaBone_v		&initbonePtr,
	int				initrenderfx,
	skin_t			*initskin,
	model_t			*initcurrentModel,
	int				initlod,
	boltInfo_v		&initboltList):

	surfaceNum(initsurfaceNum),
	rootSList(initrootSList),
	cust_shader(initcust_shader),
	fogNum(initfogNum),
	personalModel(initpersonalModel),
	bonePtr(initbonePtr),
	renderfx(initrenderfx),
	skin(initskin),
	currentModel(initcurrentModel),
	lod(initlod),
	boltList(initboltList)
	{}
};

/*

All bones should be an identity orientation to display the mesh exactly
as it is specified.

For all other frames, the bones represent the transformation from the 
orientation of the bone in the base frame to the orientation in this
frame.

*/


/*
=============
R_ACullModel
=============
*/
static int R_GCullModel( trRefEntity_t *ent ) {

	// scale the radius if need be
	float largestScale = ent->e.modelScale[0];

	if (ent->e.modelScale[1] > largestScale)
	{
		largestScale = ent->e.modelScale[1];
	}
	if (ent->e.modelScale[2] > largestScale)
	{
		largestScale = ent->e.modelScale[2];
	}
	if (!largestScale)
	{
		largestScale = 1;
	}

	// cull bounding sphere 
  	switch ( R_CullLocalPointAndRadius( NULL,  ent->e.radius * largestScale) )
  	{
  	case CULL_OUT:
  		tr.pc.c_sphere_cull_md3_out++;
  		return CULL_OUT;

	case CULL_IN:
		tr.pc.c_sphere_cull_md3_in++;
		return CULL_IN;

	case CULL_CLIP:
		tr.pc.c_sphere_cull_md3_clip++;
		return CULL_IN;
 	}
	return CULL_IN;
}


/*
=================
R_AComputeFogNum

=================
*/
static int R_GComputeFogNum( trRefEntity_t *ent ) {

	int				i, j;
	fog_t			*fog;

	if ( tr.refdef.rdflags & RDF_NOWORLDMODEL ) {
		return 0;
	}

	for ( i = 1 ; i < tr.world->numfogs ; i++ ) {
		fog = &tr.world->fogs[i];
		for ( j = 0 ; j < 3 ; j++ ) {
			if ( ent->e.origin[j] - ent->e.radius >= fog->bounds[1][j] ) {
				break;
			}
			if ( ent->e.origin[j] + ent->e.radius <= fog->bounds[0][j] ) {
				break;
			}
		}
		if ( j == 3 ) {
			return i;
		}
	}

	return 0;
}

// work out lod for this entity.
static int G2_ComputeLOD( trRefEntity_t *ent, const model_t *currentModel, int lodBias ) 
{
	float flod, lodscale;
	float projectedRadius;
	int lod;

	if ( currentModel->numLods < 2 )
	{	// model has only 1 LOD level, skip computations and bias
		return(0);
	}

	if ( r_lodbias->integer > lodBias ) 
	{
		lodBias = r_lodbias->integer;
	}

	// scale the radius if need be
	float largestScale = ent->e.modelScale[0];

	if (ent->e.modelScale[1] > largestScale)
	{
		largestScale = ent->e.modelScale[1];
	}
	if (ent->e.modelScale[2] > largestScale)
	{
		largestScale = ent->e.modelScale[2];
	}
	if (!largestScale)
	{
		largestScale = 1;
	}

	if ( ( projectedRadius = ProjectRadius( 0.75*largestScale*ent->e.radius, ent->e.origin ) ) != 0 )	//we reduce the radius to make the LOD match other model types which use the actual bound box size
 	{
 		lodscale = (r_lodscale->value+r_autolodscalevalue->value);
 		if ( lodscale > 20 ) 
		{
			lodscale = 20;	 
		}
		else if ( lodscale < 0 )
		{
			lodscale = 0;
		}
 		flod = 1.0f - projectedRadius * lodscale;
 	}
 	else
 	{
 		// object intersects near view plane, e.g. view weapon
 		flod = 0;
 	}
#ifdef DEDICATED
#define myftol(x) ((int)(x))
#endif
 	flod *= currentModel->numLods;
 	lod = myftol( flod );

 	if ( lod < 0 )
 	{
 		lod = 0;
 	}
 	else if ( lod >= currentModel->numLods )
 	{
 		lod = currentModel->numLods - 1;
 	}


	lod += lodBias;
	
	if ( lod >= currentModel->numLods )
		lod = currentModel->numLods - 1;
	if ( lod < 0 )
		lod = 0;

	return lod;
}

//======================================================================
//
// Bone Manipulation code


void G2_CreateQuaterion(mdxaBone_t *mat, vec4_t quat)
{
	// this is revised for the 3x4 matrix we use in G2.
    float t = 1 + mat->matrix[0][0] + mat->matrix[1][1] + mat->matrix[2][2];
	float s;

    //If the trace of the matrix is greater than zero, then
    //perform an "instant" calculation.
    //Important note wrt. rouning errors:
    //Test if ( T > 0.00000001 ) to avoid large distortions!
	if (t > 0.00000001)
	{
      s = sqrt(t) * 2;
      quat[0] = ( mat->matrix[1][2] - mat->matrix[2][1] ) / s;
      quat[1] = ( mat->matrix[2][0] - mat->matrix[0][2] ) / s;
      quat[2] = ( mat->matrix[0][1] - mat->matrix[1][0] ) / s;
      quat[3] = 0.25 * s;
	}
	else
	{
		//If the trace of the matrix is equal to zero then identify
		//which major diagonal element has the greatest value.

		//Depending on this, calculate the following:

		if ( mat->matrix[0][0] > mat->matrix[1][1] && mat->matrix[0][0] > mat->matrix[2][2] )  {	// Column 0: 
			s  = sqrt( 1.0 + mat->matrix[0][0] - mat->matrix[1][1] - mat->matrix[2][2])* 2;
			quat[0] = 0.25 * s;
			quat[1] = (mat->matrix[0][1] + mat->matrix[1][0] ) / s;
			quat[2] = (mat->matrix[2][0] + mat->matrix[0][2] ) / s;
			quat[3] = (mat->matrix[1][2] - mat->matrix[2][1] ) / s;
		
		} else if ( mat->matrix[1][1] > mat->matrix[2][2] ) {			// Column 1: 
			s  = sqrt( 1.0 + mat->matrix[1][1] - mat->matrix[0][0] - mat->matrix[2][2] ) * 2;
			quat[0] = (mat->matrix[0][1] + mat->matrix[1][0] ) / s;
			quat[1] = 0.25 * s;
			quat[2] = (mat->matrix[1][2] + mat->matrix[2][1] ) / s;
			quat[3] = (mat->matrix[2][0] - mat->matrix[0][2] ) / s;

		} else {						// Column 2:
			s  = sqrt( 1.0 + mat->matrix[2][2] - mat->matrix[0][0] - mat->matrix[1][1] ) * 2;
			quat[0] = (mat->matrix[2][0]+ mat->matrix[0][2] ) / s;
			quat[1] = (mat->matrix[1][2] + mat->matrix[2][1] ) / s;
			quat[2] = 0.25 * s;
			quat[3] = (mat->matrix[0][1] - mat->matrix[1][0] ) / s;
		}
	}
}

void G2_CreateMatrixFromQuaterion(mdxaBone_t *mat, vec4_t quat)
{
	
    float xx      = quat[0] * quat[0];
    float xy      = quat[0] * quat[1];
    float xz      = quat[0] * quat[2];
    float xw      = quat[0] * quat[3];

    float yy      = quat[1] * quat[1];
    float yz      = quat[1] * quat[2];
    float yw      = quat[1] * quat[3];

    float zz      = quat[2] * quat[2];
    float zw      = quat[2] * quat[3];

    mat->matrix[0][0]  = 1 - 2 * ( yy + zz );
    mat->matrix[1][0]  =     2 * ( xy - zw );
    mat->matrix[2][0]  =     2 * ( xz + yw );

    mat->matrix[0][1]  =     2 * ( xy + zw );
    mat->matrix[1][1]  = 1 - 2 * ( xx + zz );
    mat->matrix[2][1]  =     2 * ( yz - xw );

    mat->matrix[0][2]  =     2 * ( xz - yw );
    mat->matrix[1][2]  =     2 * ( yz + xw );
    mat->matrix[2][2]  = 1 - 2 * ( xx + yy );

    mat->matrix[0][3]  = mat->matrix[1][3] = mat->matrix[2][3] = 0;
}

// nasty little matrix multiply going on here..
void Multiply_3x4Matrix(mdxaBone_t *out, mdxaBone_t *in2, mdxaBone_t *in) 
{
	// first row of out                                                                                      
	out->matrix[0][0] = (in2->matrix[0][0] * in->matrix[0][0]) + (in2->matrix[0][1] * in->matrix[1][0]) + (in2->matrix[0][2] * in->matrix[2][0]);
	out->matrix[0][1] = (in2->matrix[0][0] * in->matrix[0][1]) + (in2->matrix[0][1] * in->matrix[1][1]) + (in2->matrix[0][2] * in->matrix[2][1]);
	out->matrix[0][2] = (in2->matrix[0][0] * in->matrix[0][2]) + (in2->matrix[0][1] * in->matrix[1][2]) + (in2->matrix[0][2] * in->matrix[2][2]);
	out->matrix[0][3] = (in2->matrix[0][0] * in->matrix[0][3]) + (in2->matrix[0][1] * in->matrix[1][3]) + (in2->matrix[0][2] * in->matrix[2][3]) + in2->matrix[0][3];
	// second row of outf out                                                                                     
	out->matrix[1][0] = (in2->matrix[1][0] * in->matrix[0][0]) + (in2->matrix[1][1] * in->matrix[1][0]) + (in2->matrix[1][2] * in->matrix[2][0]);
	out->matrix[1][1] = (in2->matrix[1][0] * in->matrix[0][1]) + (in2->matrix[1][1] * in->matrix[1][1]) + (in2->matrix[1][2] * in->matrix[2][1]);
	out->matrix[1][2] = (in2->matrix[1][0] * in->matrix[0][2]) + (in2->matrix[1][1] * in->matrix[1][2]) + (in2->matrix[1][2] * in->matrix[2][2]);
	out->matrix[1][3] = (in2->matrix[1][0] * in->matrix[0][3]) + (in2->matrix[1][1] * in->matrix[1][3]) + (in2->matrix[1][2] * in->matrix[2][3]) + in2->matrix[1][3];
	// third row of out  out                                                                                      
	out->matrix[2][0] = (in2->matrix[2][0] * in->matrix[0][0]) + (in2->matrix[2][1] * in->matrix[1][0]) + (in2->matrix[2][2] * in->matrix[2][0]);
	out->matrix[2][1] = (in2->matrix[2][0] * in->matrix[0][1]) + (in2->matrix[2][1] * in->matrix[1][1]) + (in2->matrix[2][2] * in->matrix[2][1]);
	out->matrix[2][2] = (in2->matrix[2][0] * in->matrix[0][2]) + (in2->matrix[2][1] * in->matrix[1][2]) + (in2->matrix[2][2] * in->matrix[2][2]);
	out->matrix[2][3] = (in2->matrix[2][0] * in->matrix[0][3]) + (in2->matrix[2][1] * in->matrix[1][3]) + (in2->matrix[2][2] * in->matrix[2][3]) + in2->matrix[2][3]; 
}


static int G2_GetBonePoolIndex(	const mdxaHeader_t *pMDXAHeader, int iFrame, int iBone)
{
	const int iOffsetToIndex = (iFrame * pMDXAHeader->numBones * 3) + (iBone * 3);

	mdxaIndex_t *pIndex = (mdxaIndex_t *) ((byte*) pMDXAHeader + pMDXAHeader->ofsFrames + iOffsetToIndex);

	return pIndex->iIndex & 0x00FFFFFF;	// this will cause problems for big-endian machines... ;-)
}


/*static inline*/ void UnCompressBone(float mat[3][4], int iBoneIndex, const mdxaHeader_t *pMDXAHeader, int iFrame)
{
	mdxaCompQuatBone_t *pCompBonePool = (mdxaCompQuatBone_t *) ((byte *)pMDXAHeader + pMDXAHeader->ofsCompBonePool);
	MC_UnCompressQuat(mat, pCompBonePool[ G2_GetBonePoolIndex( pMDXAHeader, iFrame, iBoneIndex ) ].Comp);
}

#define DEBUG_G2_TIMING (0)

// transform each individual bone's information - making sure to use any override information provided, both for angles and for animations, as
// well as multiplying each bone's matrix by it's parents matrix 
void G2_TransformBone (CTransformBone &TB)
{
	mdxaBone_t		tbone[6];
	mdxaSkel_t		*skel;
	mdxaSkelOffsets_t *offsets;
	boneInfo_v		&boneList = TB.rootBoneList;
	int				i, j, boneListIndex;
	int				angleOverride = 0;

#if DEBUG_G2_TIMING
	bool printTiming=false;
#endif

	// decide here if we should go down this path? - is this bone used? -If not, return from this function. Due the hierarchial nature of the bones
	// any bone below this one in the tree shouldn't be used either.
	if (!TB.usedBoneList[TB.child])
	{
		return;
	}

	// should this bone be overridden by a bone in the bone list?
	boneListIndex = G2_Find_Bone_In_List(boneList, TB.child);
	if (boneListIndex != -1)
	{
		// we found a bone in the list - we need to override something here.
		
		// do we override the rotational angles?
		if ((boneList[boneListIndex].flags) & (BONE_ANGLES_TOTAL))
		{
			angleOverride = (boneList[boneListIndex].flags) & (BONE_ANGLES_TOTAL);
		}

		if (boneList[boneListIndex].flags & BONE_ANIM_BLEND)
		{
			float blendTime = TB.incomingTime - boneList[boneListIndex].blendStart;
			// only set up the blend anim if we actually have some blend time left on this bone anim - otherwise we might corrupt some blend higher up the hiearchy
			if (blendTime>=0.0f&&blendTime < boneList[boneListIndex].blendTime)
			{
				TB.blendFrame = boneList[boneListIndex].blendFrame;
				TB.blendOldFrame = boneList[boneListIndex].blendLerpFrame;
				TB.blendLerp = (blendTime / boneList[boneListIndex].blendTime);
				TB.blendMode = true;
			}
			else
			{
				TB.blendMode = false;
			}
		}
		else if ((boneList[boneListIndex].flags) & (BONE_ANIM_OVERRIDE_LOOP | BONE_ANIM_OVERRIDE))
		{
			TB.blendMode = false;
		}

		if ((boneList[boneListIndex].flags) & (BONE_ANIM_OVERRIDE_LOOP | BONE_ANIM_OVERRIDE))
		{
			float	animSpeed = boneList[boneListIndex].animSpeed;
			float	time;
			if (boneList[boneListIndex].pauseTime)
			{
				time = (boneList[boneListIndex].pauseTime - boneList[boneListIndex].startTime) / 50.0f;
			}
			else
			{			
				time = (TB.incomingTime - boneList[boneListIndex].startTime) / 50.0f;
			}
			if (time<0)
			{
				time=0;
			}
			float	newFrame_g = boneList[boneListIndex].startFrame + (time * animSpeed);

			float	endFrame = (float)boneList[boneListIndex].endFrame ;

			int		animSize = endFrame - boneList[boneListIndex].startFrame;
			// we are supposed to be animating right?
			if (animSize)
			{
				// did we run off the end?
				if (((animSpeed > 0.0f) && (newFrame_g > endFrame - 1)) || 
					((animSpeed < 0.0f) && (newFrame_g < endFrame + 1)))
				{
					// yep - decide what to do
					if (boneList[boneListIndex].flags & BONE_ANIM_OVERRIDE_LOOP)
					{
						// get our new animation frame back within the bounds of the animation set
						if (animSpeed < 0.0f)
						{
							if ((newFrame_g < endFrame+1) && (newFrame_g >= endFrame))
							{
								// now figure out what we are lerping between
								// delta is the fraction between this frame and the next, since the new anim is always at a .0f;
								TB.backlerp = (float(endFrame+1)-newFrame_g);
								// frames are easy to calculate
								TB.currentFrame = endFrame;
								TB.newFrame = boneList[boneListIndex].startFrame;
							}
							else
							{
								if (newFrame_g <= endFrame+1)
								{
									newFrame_g=endFrame+fmod(newFrame_g-endFrame,animSize)-animSize;
								}
								// now figure out what we are lerping between
								// delta is the fraction between this frame and the next, since the new anim is always at a .0f;
								TB.backlerp = (ceil(newFrame_g)-newFrame_g);
								// frames are easy to calculate
								TB.currentFrame = ceil(newFrame_g);
								// should we be creating a virtual frame?
								if (newFrame_g <= endFrame + 1)
								{

									TB.newFrame = boneList[boneListIndex].startFrame;
								}
								else
								{
									TB.newFrame = TB.currentFrame - 1;
								}
							}
						}
						else
						{
							// should we be creating a virtual frame?
							if ((newFrame_g > endFrame - 1) && (newFrame_g < endFrame))
							{
								// now figure out what we are lerping between
								// delta is the fraction between this frame and the next, since the new anim is always at a .0f;
								TB.backlerp = (newFrame_g - (int)newFrame_g);
								// frames are easy to calculate
								TB.currentFrame = (int)newFrame_g;
								TB.newFrame = boneList[boneListIndex].startFrame;
							}
							else
							{
								if (newFrame_g >= endFrame)
								{
									newFrame_g=endFrame+fmod(newFrame_g-endFrame,animSize)-animSize;
								}
								// now figure out what we are lerping between
								// delta is the fraction between this frame and the next, since the new anim is always at a .0f;
								TB.backlerp = (newFrame_g - (int)newFrame_g);
								// frames are easy to calculate
								TB.currentFrame = (int)newFrame_g;
								// should we be creating a virtual frame?
								if (newFrame_g >= endFrame - 1)
								{
									TB.newFrame = boneList[boneListIndex].startFrame;
								}
								else
								{
									TB.newFrame = TB.currentFrame + 1;
								}
							}
						}
						// sanity check
						assert ((TB.newFrame < endFrame) && (TB.newFrame >= boneList[boneListIndex].startFrame) || (animSize < 10)); 
					}
					else
					{
						if ((boneList[boneListIndex].flags & BONE_ANIM_OVERRIDE_FREEZE) == BONE_ANIM_OVERRIDE_FREEZE)
						{
							// if we are supposed to reset the default anim, then do so
							if (animSpeed > 0.0f)
							{
								TB.currentFrame = boneList[boneListIndex].endFrame - 1;
							}
							else
							{
								TB.currentFrame = boneList[boneListIndex].endFrame + 1;
							}

							TB.newFrame = TB.currentFrame;
							TB.backlerp = 0;
						}

						// nope, just stop processing this bone. And do nothing	- let the bone take the parents anim info
			   		}
				}
				else //
				{
					if (animSpeed> 0.0)
					{

						TB.currentFrame = (int)newFrame_g;

						// figure out the difference between the two frames	- we have to decide what frame and what percentage of that
						// frame we want to display
						TB.backlerp = (newFrame_g - TB.currentFrame);

						TB.newFrame = TB.currentFrame + 1;
						if (TB.newFrame >= (int)endFrame)
						{
							// we only want to lerp with the first frame of the anim if we are looping 
							if (boneList[boneListIndex].flags & BONE_ANIM_OVERRIDE_LOOP)
							{
					  			TB.newFrame = boneList[boneListIndex].startFrame;
							}
							// if we intend to end this anim or freeze after this, then just keep on the last frame
							else
							{
								TB.newFrame = boneList[boneListIndex].endFrame;
							}
						}
					}
					else
					{
						TB.backlerp = (ceil(newFrame_g)-newFrame_g);
						TB.currentFrame = ceil(newFrame_g);
						if (TB.currentFrame>boneList[boneListIndex].startFrame)
						{
							TB.currentFrame=boneList[boneListIndex].startFrame;
							TB.newFrame = TB.currentFrame;
							TB.backlerp=0.0f;
						}
						else
						{
							TB.newFrame=TB.currentFrame-1;
							// are we now on the end frame?
							if (TB.newFrame < endFrame+1)
							{
								if (boneList[boneListIndex].flags & BONE_ANIM_OVERRIDE_LOOP)
								{
						  			TB.newFrame = boneList[boneListIndex].startFrame;
								}
								else
								{
									TB.newFrame = boneList[boneListIndex].endFrame+1;
								}
							}
						}
					}
				}
			}
			else
			{
				if (animSpeed<0.0)
				{
					TB.currentFrame = boneList[boneListIndex].endFrame+1;
				}
				else
				{
					TB.currentFrame = boneList[boneListIndex].endFrame-1;
				}
				if (TB.currentFrame<0)
				{
					TB.currentFrame=0;
				}
				TB.newFrame = TB.currentFrame;
				TB.backlerp = 0;

			}
#if DEBUG_G2_TIMING
			printTiming=true;
#endif
		}	
	}

	//If either of these things happen the code below will choke to death miserably. Or so I gather.
	//Seems to only happen when the server lags badly and cuts off updates to the client state, perhaps
	//it doesn't like extremely long intervals between updating the animation state?
	if (TB.currentFrame < 0 || TB.newFrame < 0)
	{
		return;
	}	
	else if (TB.blendMode && (TB.blendFrame < 0 || TB.blendOldFrame < 0))
	{
		return;
	}
#if DEBUG_G2_TIMING
	if (printTiming&&boneListIndex==2)
	{
		char mess[1000];
		if (TB.blendMode)
		{
			sprintf(mess,"frb[%2d] %5d   %5d %5d %5d %5d  %f %f\n",boneListIndex,TB.incomingTime,(int)TB.newFrame,(int)TB.currentFrame,(int)TB.blendFrame,(int)TB.blendOldFrame,TB.backlerp,TB.blendLerp);
		}
		else
		{
			sprintf(mess,"fra[%2d] %5d   %5d %5d              %f\n",boneListIndex,TB.incomingTime,TB.newFrame,TB.currentFrame,TB.backlerp);
		}
		OutputDebugString(mess);
		const boneInfo_t &bone=boneList[boneListIndex];
		if (bone.flags&BONE_ANIM_BLEND)
		{
			sprintf(mess,"                                                                    bfb[%2d] %5d  %5d  (%5d-%5d) %4.2f %4x   bt(%5d-%5d) %7.2f %5d\n",
				boneListIndex,
				TB.incomingTime,
				bone.startTime,
				bone.startFrame,
				bone.endFrame,
				bone.animSpeed,
				bone.flags,
				bone.blendStart,
				bone.blendStart+bone.blendTime,
				bone.blendFrame,
				bone.blendLerpFrame
				);
		}
		else
		{
			sprintf(mess,"                                                                    bfa[%2d] %5d  %5d  (%5d-%5d) %4.2f %4x\n",
				boneListIndex,
				TB.incomingTime,
				bone.startTime,
				bone.startFrame,
				bone.endFrame,
				bone.animSpeed,
				bone.flags
				);
		}
		OutputDebugString(mess);
	}
#endif

//	const mdxaCompBone_t	*compBonePointer = (mdxaCompBone_t *)((byte *)TB.header + TB.header->ofsCompBonePool);

	// are we blending with another frame of anim?
	if (TB.blendMode)
	{
		const float backlerp = TB.blendFrame - (int)TB.blendFrame;
		const float frontlerp = 1.0 - backlerp;

		// figure out where the location of the blended animation data is
//	 	const mdxaFrame_t	*bFrame =	(mdxaFrame_t *)((byte *)TB.header + TB.header->ofsFrames + (int)TB.blendFrame * TB.frameSize );
//		const mdxaFrame_t	*boldFrame =(mdxaFrame_t *)((byte *)TB.header + TB.header->ofsFrames + TB.blendOldFrame * TB.frameSize );

//  		MC_UnCompress(tbone[3].matrix,compBonePointer[bFrame->boneIndexes[TB.child]].Comp);
//  		MC_UnCompress(tbone[4].matrix,compBonePointer[boldFrame->boneIndexes[TB.child]].Comp);
			UnCompressBone(tbone[3].matrix,TB.child, TB.header, TB.blendFrame);
			UnCompressBone(tbone[4].matrix,TB.child, TB.header, TB.blendOldFrame);
				
		for ( j = 0 ; j < 12 ; j++ ) 
		{
  			((float *)&tbone[5])[j] = (backlerp * ((float *)&tbone[3])[j])
				+ (frontlerp * ((float *)&tbone[4])[j]);
		}
	}

	// figure out where the location of the bone animation data is
	assert (TB.header->numFrames > TB.currentFrame);//validate the frame we're about to grab
//	const mdxaFrame_t	*aoldFrame = (mdxaFrame_t *)((byte *)TB.header + TB.header->ofsFrames + TB.currentFrame * TB.frameSize );

	// figure out where the bone hirearchy info is
	offsets = (mdxaSkelOffsets_t *)((byte *)TB.header + sizeof(mdxaHeader_t));
	skel = (mdxaSkel_t *)((byte *)TB.header + sizeof(mdxaHeader_t) + offsets->offsets[TB.child]);
  	//
  	// lerp this bone - use the temp space on the ref entity to put the bone transforms into
  	//
  	if (!TB.backlerp)
  	{
// 		MC_UnCompress(tbone[2].matrix,compBonePointer[aoldFrame->boneIndexes[TB.child]].Comp);
		UnCompressBone(tbone[2].matrix,TB.child, TB.header, TB.currentFrame);

		// blend in the other frame if we need to
		if (TB.blendMode)
		{
		float blendFrontlerp = 1.0 - TB.blendLerp;
	  		for ( j = 0 ; j < 12 ; j++ ) 
			{
  				((float *)&tbone[2])[j] = (TB.blendLerp * ((float *)&tbone[2])[j])
					+ (blendFrontlerp * ((float *)&tbone[5])[j]);
			}
		}

  		if (TB.rootBone)
		{
			// now multiply by the root matrix, so we can offset this model should we need to
			Multiply_3x4Matrix(&TB.bonePtr[TB.child].second, &TB.rootMatrix, &tbone[2]);
 		}
  	}
	else
  	{
		const float frontlerp = 1.0 - TB.backlerp;
		// figure out where the location of the bone animation data is
		assert (TB.header->numFrames > TB.newFrame);//validate the frame we're about to grab
//	 	const mdxaFrame_t	*aFrame = (mdxaFrame_t *)((byte *)TB.header + TB.header->ofsFrames + TB.newFrame * TB.frameSize );

// 		MC_UnCompress(tbone[0].matrix,compBonePointer[aFrame->boneIndexes[TB.child]].Comp);
//		MC_UnCompress(tbone[1].matrix,compBonePointer[aoldFrame->boneIndexes[TB.child]].Comp);
		UnCompressBone(tbone[0].matrix,TB.child, TB.header, TB.newFrame);
		UnCompressBone(tbone[1].matrix,TB.child, TB.header, TB.currentFrame);		

		for ( j = 0 ; j < 12 ; j++ ) 
		{
  			((float *)&tbone[2])[j] = (TB.backlerp * ((float *)&tbone[0])[j])
				+ (frontlerp * ((float *)&tbone[1])[j]);
		}

		// blend in the other frame if we need to
		if (TB.blendMode)
		{
			const float blendFrontlerp = 1.0 - TB.blendLerp;
	  		for ( j = 0 ; j < 12 ; j++ ) 
			{
				((float *)&tbone[2])[j] = (TB.blendLerp * ((float *)&tbone[2])[j])
					+ (blendFrontlerp * ((float *)&tbone[5])[j]);
			}
		}

  		if (TB.rootBone)
  		{
			// now multiply by the root matrix, so we can offset this model should we need to
			Multiply_3x4Matrix(&TB.bonePtr[TB.child].second, &TB.rootMatrix, &tbone[2]);
  		}
	}


	if (angleOverride & BONE_ANGLES_REPLACE)
	{
		mdxaBone_t temp, firstPass;
		mdxaBone_t &bone = TB.bonePtr[TB.child].second;
		boneInfo_t &boneOverride = boneList[boneListIndex];
		// give us the matrix the animation thinks we should have, so we can get the correct X&Y coors
		Multiply_3x4Matrix(&firstPass, &TB.bonePtr[TB.parent].second, &tbone[2]);

		// are we attempting to blend with the base animation? and still within blend time?
		if (boneOverride.boneBlendTime && (((boneOverride.boneBlendTime + boneOverride.boneBlendStart) < TB.incomingTime)))
		{
			// ok, we are supposed to be blending. Work out lerp
			const float blendTime = TB.incomingTime - boneList[boneListIndex].boneBlendStart;
			float blendLerp = (blendTime / boneList[boneListIndex].boneBlendTime);
			if (blendLerp <= 1)
			{
				if (blendLerp < 0)
				{
					assert(0);
				}

				// now work out the matrix we want to get *to* - firstPass is where we are coming *from*
				Multiply_3x4Matrix(&temp, &firstPass, &skel->BasePoseMat);

				const float	matrixScale = VectorLength((float*)&temp);
				
				for (int i=0; i<3;i++)
				{
					for(int x=0;x<3; x++)
					{
						boneOverride.newMatrix.matrix[i][x] *= matrixScale;
					}
				}

				boneOverride.newMatrix.matrix[0][3] = temp.matrix[0][3];
				boneOverride.newMatrix.matrix[1][3] = temp.matrix[1][3]; 
				boneOverride.newMatrix.matrix[2][3] = temp.matrix[2][3];
			
 				Multiply_3x4Matrix(&temp, &boneOverride.newMatrix,&skel->BasePoseMatInv);

				// now do the blend into the destination
				const float blendFrontlerp = 1.0 - blendLerp;
	  			for ( j = 0 ; j < 12 ; j++ ) 
				{
  					((float *)&bone)[j] = (blendLerp * ((float *)&temp)[j])
						+ (blendFrontlerp * ((float *)&firstPass)[j]);
				}
			}
			else
			{
				bone = firstPass;
			}
		}
		// no, so just override it directly
		else
		{

			Multiply_3x4Matrix(&temp, &firstPass, &skel->BasePoseMat);
			const float	matrixScale = VectorLength((float*)&temp);
			
			for (int i=0; i<3;i++)
			{
				for(int x=0;x<3; x++)
				{
					boneOverride.newMatrix.matrix[i][x] *= matrixScale;
				}
			}

			boneOverride.newMatrix.matrix[0][3] = temp.matrix[0][3];
			boneOverride.newMatrix.matrix[1][3] = temp.matrix[1][3]; 
			boneOverride.newMatrix.matrix[2][3] = temp.matrix[2][3];
		
 			Multiply_3x4Matrix(&bone, &boneOverride.newMatrix,&skel->BasePoseMatInv);
		}
	}
	else
	if (angleOverride & BONE_ANGLES_PREMULT)
	{
		if (TB.rootBone)
		{
			// use the in coming root matrix as our basis
			Multiply_3x4Matrix(&TB.bonePtr[TB.child].second, &TB.rootMatrix, &boneList[boneListIndex].newMatrix);
 		}
		else
		{
			// convert from 3x4 matrix to a 4x4 matrix
			Multiply_3x4Matrix(&TB.bonePtr[TB.child].second, &TB.bonePtr[TB.parent].second, &boneList[boneListIndex].newMatrix);
		}
	}
	else
	// now transform the matrix by it's TB.parent, asumming we have a TB.parent, and we aren't overriding the angles absolutely
	if (!TB.rootBone)
	{
		Multiply_3x4Matrix(&TB.bonePtr[TB.child].second, &TB.bonePtr[TB.parent].second, &tbone[2]);
	}

	// now multiply our resulting bone by an override matrix should we need to
	if (angleOverride & BONE_ANGLES_POSTMULT)
	{
		mdxaBone_t	tempMatrix;
		memcpy (&tempMatrix, &TB.bonePtr[TB.child].second, sizeof(mdxaBone_t));
	  	Multiply_3x4Matrix(&TB.bonePtr[TB.child].second, &tempMatrix, &boneList[boneListIndex].newMatrix);
	}

	// are the bone that we are resetting to the origin?
	if (TB.child == TB.boltNum)
	{
		//create a world matrix for the new origin
		mdxaBone_t tempMatrix;
		Multiply_3x4Matrix(&tempMatrix, &TB.bonePtr[TB.child].second, &skel->BasePoseMat);
		TB.newModelOrigin[0] = tempMatrix.matrix[0][3];
		TB.newModelOrigin[1] = tempMatrix.matrix[1][3];
		TB.newModelOrigin[2] = tempMatrix.matrix[2][3];
	}

	TB.rootBone = false;
	const int		parent = TB.child;
	const int		newFrame = TB.newFrame;
	const int		currentFrame = TB.currentFrame;
	const float	backLerp = TB.backlerp;
	const bool	blendMode = TB.blendMode;
	const float	blendFrame = TB.blendFrame;
	const int		blendOldFrame = TB.blendOldFrame;
	const float 	blendLerp = TB.blendLerp;
	// now work out what children we have to call this recursively for
	for (i=0; i< skel->numChildren; i++)
	{
		TB.newFrame = newFrame;
		TB.currentFrame = currentFrame;
		TB.backlerp = backLerp;
		TB.parent = parent;
		TB.child = skel->children[i];
		TB.blendFrame = blendFrame;
		TB.blendOldFrame = blendOldFrame;
		TB.blendMode = blendMode;
		TB.blendLerp = blendLerp;
		G2_TransformBone(TB);
	}
}

void G2_SetUpBolts( mdxaHeader_t *header, CGhoul2Info &ghoul2, mdxaBone_v &bonePtr, boltInfo_v &boltList)
{
	mdxaSkel_t		*skel;
	mdxaSkelOffsets_t *offsets;
	offsets = (mdxaSkelOffsets_t *)((byte *)header + sizeof(mdxaHeader_t));

	for (int i=0; i<boltList.size(); i++)
	{
		if (boltList[i].boneNumber != -1)
		{
			// figure out where the bone hirearchy info is
			skel = (mdxaSkel_t *)((byte *)header + sizeof(mdxaHeader_t) + offsets->offsets[boltList[i].boneNumber]);
			Multiply_3x4Matrix(&boltList[i].position, &bonePtr[boltList[i].boneNumber].second, &skel->BasePoseMat);
		}
	}
}


// start the recursive hirearchial bone transform and lerp process for this model
void G2_TransformGhoulBones( mdxaHeader_t *header, int *usedBoneList, boneInfo_v &rootBoneList, mdxaBone_v &bonePtr, boltInfo_v &boltList,
							mdxaBone_t &rootMatrix, CGhoul2Info &ghoul2, int time, int boneCount)
{
	int				frameSize, i;
	int				rootBoneIndex = 0;

    frameSize = 0;//	unused	(int)( &((mdxaFrame_t *)0)->boneIndexes[ header->numBones ] );   
	
	// figure out where our rootbone is
	for (i=0; i<boneCount; i++)
	{
		if (usedBoneList[i])
		{
			rootBoneIndex = i;
			break;
		}
	}
	 
	// now recursively call the bone transform routines using the bone hirearchy
	CTransformBone TB(ghoul2.mAnimFrameDefault, ghoul2.mAnimFrameDefault, 0, rootBoneIndex, frameSize, header, 0.0f, usedBoneList,
					  rootBoneList, bonePtr, boltList, rootMatrix, true, time, 0, 0, false, 0,  ghoul2.mNewOrigin);

	G2_TransformBone (TB);
	// now set up the bolt positions for those bolts required.
	G2_SetUpBolts(header, ghoul2, bonePtr, boltList);

}


#define MDX_TAG_ORIGIN 2

//======================================================================
//
// Surface Manipulation code 


// We've come across a surface that's designated as a bolt surface, process it and put it in the appropriate bolt place
void G2_ProcessSurfaceBolt(mdxaBone_v &bonePtr, mdxmSurface_t *surface, int boltNum, boltInfo_v &boltList, surfaceInfo_t *surfInfo, model_t *mod)
{
 	mdxmVertex_t 	*v, *vert0, *vert1, *vert2;
 	vec3_t			axes[3], sides[3];
 	float			pTri[3][3], d;
 	int				j, k;

	// now there are two types of tag surface - model ones and procedural generated types - lets decide which one we have here.
	if (surfInfo && surfInfo->offFlags == G2SURFACEFLAG_GENERATED)
	{
		int surfNumber = surfInfo->genPolySurfaceIndex & 0x0ffff;
		int	polyNumber = (surfInfo->genPolySurfaceIndex >> 16) & 0x0ffff;

		// find original surface our original poly was in.
		mdxmSurface_t	*originalSurf = (mdxmSurface_t *)G2_FindSurface((void*)mod, surfNumber, surfInfo->genLod);
		mdxmTriangle_t	*originalTriangleIndexes = (mdxmTriangle_t *)((byte*)originalSurf + originalSurf->ofsTriangles);

		// get the original polys indexes 
		int index0 = originalTriangleIndexes[polyNumber].indexes[0];
		int index1 = originalTriangleIndexes[polyNumber].indexes[1];
		int index2 = originalTriangleIndexes[polyNumber].indexes[2];

		// decide where the original verts are

 		vert0 = (mdxmVertex_t *) ((byte *)originalSurf + originalSurf->ofsVerts);
		vert0+= index0;

 		vert1 = (mdxmVertex_t *) ((byte *)originalSurf + originalSurf->ofsVerts);
		vert1+= index1;
		
 		vert2 = (mdxmVertex_t *) ((byte *)originalSurf + originalSurf->ofsVerts);
		vert2+= index2;

		// clear out the triangle verts to be
 	   	VectorClear( pTri[0] );
 	   	VectorClear( pTri[1] );
 	   	VectorClear( pTri[2] );

//		mdxmWeight_t	*w;

		int *piBoneRefs = (int*) ((byte*)originalSurf + originalSurf->ofsBoneReferences);		

		// now go and transform just the points we need from the surface that was hit originally
//		w = vert0->weights;
		float fTotalWeight = 0.0f;
		int iNumWeights = G2_GetVertWeights( vert0 );
 		for ( k = 0 ; k < iNumWeights ; k++ ) 
 		{
			int		iBoneIndex	= G2_GetVertBoneIndex( vert0, k );
			float	fBoneWeight	= G2_GetVertBoneWeight( vert0, k, fTotalWeight, iNumWeights );

 			pTri[0][0] += fBoneWeight * ( DotProduct( bonePtr[piBoneRefs[iBoneIndex]].second.matrix[0], vert0->vertCoords ) + bonePtr[piBoneRefs[iBoneIndex]].second.matrix[0][3] );
 			pTri[0][1] += fBoneWeight * ( DotProduct( bonePtr[piBoneRefs[iBoneIndex]].second.matrix[1], vert0->vertCoords ) + bonePtr[piBoneRefs[iBoneIndex]].second.matrix[1][3] );
 			pTri[0][2] += fBoneWeight * ( DotProduct( bonePtr[piBoneRefs[iBoneIndex]].second.matrix[2], vert0->vertCoords ) + bonePtr[piBoneRefs[iBoneIndex]].second.matrix[2][3] );
		}
//		w = vert1->weights;
		fTotalWeight = 0.0f;
		iNumWeights = G2_GetVertWeights( vert1 );
 		for ( k = 0 ; k < iNumWeights ; k++ ) 
 		{
			int		iBoneIndex	= G2_GetVertBoneIndex( vert1, k );
			float	fBoneWeight	= G2_GetVertBoneWeight( vert1, k, fTotalWeight, iNumWeights );

 			pTri[1][0] += fBoneWeight * ( DotProduct( bonePtr[piBoneRefs[iBoneIndex]].second.matrix[0], vert1->vertCoords ) + bonePtr[piBoneRefs[iBoneIndex]].second.matrix[0][3] );
 			pTri[1][1] += fBoneWeight * ( DotProduct( bonePtr[piBoneRefs[iBoneIndex]].second.matrix[1], vert1->vertCoords ) + bonePtr[piBoneRefs[iBoneIndex]].second.matrix[1][3] );
 			pTri[1][2] += fBoneWeight * ( DotProduct( bonePtr[piBoneRefs[iBoneIndex]].second.matrix[2], vert1->vertCoords ) + bonePtr[piBoneRefs[iBoneIndex]].second.matrix[2][3] );
		}
//		w = vert2->weights;
		fTotalWeight = 0.0f;
		iNumWeights = G2_GetVertWeights( vert2 );
 		for ( k = 0 ; k < iNumWeights ; k++ ) 
 		{
			int		iBoneIndex	= G2_GetVertBoneIndex( vert2, k );
			float	fBoneWeight	= G2_GetVertBoneWeight( vert2, k, fTotalWeight, iNumWeights );

 			pTri[2][0] += fBoneWeight * ( DotProduct( bonePtr[piBoneRefs[iBoneIndex]].second.matrix[0], vert2->vertCoords ) + bonePtr[piBoneRefs[iBoneIndex]].second.matrix[0][3] );
 			pTri[2][1] += fBoneWeight * ( DotProduct( bonePtr[piBoneRefs[iBoneIndex]].second.matrix[1], vert2->vertCoords ) + bonePtr[piBoneRefs[iBoneIndex]].second.matrix[1][3] );
 			pTri[2][2] += fBoneWeight * ( DotProduct( bonePtr[piBoneRefs[iBoneIndex]].second.matrix[2], vert2->vertCoords ) + bonePtr[piBoneRefs[iBoneIndex]].second.matrix[2][3] );
		}
 			
   		vec3_t normal;
		vec3_t up;
		vec3_t right;
		vec3_t vec0, vec1;
		// work out baryCentricK
		float baryCentricK = 1.0 - (surfInfo->genBarycentricI + surfInfo->genBarycentricJ);

		// now we have the model transformed into model space, now generate an origin.
		boltList[boltNum].position.matrix[0][3] = (pTri[0][0] * surfInfo->genBarycentricI) + (pTri[1][0] * surfInfo->genBarycentricJ) + (pTri[2][0] * baryCentricK);
		boltList[boltNum].position.matrix[1][3] = (pTri[0][1] * surfInfo->genBarycentricI) + (pTri[1][1] * surfInfo->genBarycentricJ) + (pTri[2][1] * baryCentricK);
		boltList[boltNum].position.matrix[2][3] = (pTri[0][2] * surfInfo->genBarycentricI) + (pTri[1][2] * surfInfo->genBarycentricJ) + (pTri[2][2] * baryCentricK);

		// generate a normal to this new triangle
		VectorSubtract(pTri[0], pTri[1], vec0);
		VectorSubtract(pTri[2], pTri[1], vec1);

		CrossProduct(vec0, vec1, normal);
		VectorNormalize(normal);

		// forward vector
		boltList[boltNum].position.matrix[0][0] = normal[0];
		boltList[boltNum].position.matrix[1][0] = normal[1];
		boltList[boltNum].position.matrix[2][0] = normal[2];

		// up will be towards point 0 of the original triangle.
		// so lets work it out. Vector is hit point - point 0
		up[0] = boltList[boltNum].position.matrix[0][3] - pTri[0][0];
		up[1] = boltList[boltNum].position.matrix[1][3] - pTri[0][1];
		up[2] = boltList[boltNum].position.matrix[2][3] - pTri[0][2];

		// normalise it
		VectorNormalize(up);

		// that's the up vector
		boltList[boltNum].position.matrix[0][1] = up[0];
		boltList[boltNum].position.matrix[1][1] = up[1];
		boltList[boltNum].position.matrix[2][1] = up[2];

		// right is always straight

		CrossProduct( normal, up, right );
		// that's the up vector
		boltList[boltNum].position.matrix[0][2] = right[0];
		boltList[boltNum].position.matrix[1][2] = right[1];
		boltList[boltNum].position.matrix[2][2] = right[2];


	}
	// no, we are looking at a normal model tag
	else
	{
		int *piBoneRefs = (int*) ((byte*)surface + surface->ofsBoneReferences);		

	 	// whip through and actually transform each vertex
 		v = (mdxmVertex_t *) ((byte *)surface + surface->ofsVerts);
 		for ( j = 0; j < 3; j++ ) 
 		{
// 			mdxmWeight_t	*w;

 			VectorClear( pTri[j] );
 //			w = v->weights;

			const int iNumWeights = G2_GetVertWeights( v );
			float fTotalWeight = 0.0f;
 			for ( k = 0 ; k < iNumWeights ; k++ ) 
 			{
				int		iBoneIndex	= G2_GetVertBoneIndex( v, k );
				float	fBoneWeight	= G2_GetVertBoneWeight( v, k, fTotalWeight, iNumWeights );

 				//bone = bonePtr + piBoneRefs[w->boneIndex];

 				pTri[j][0] += fBoneWeight * ( DotProduct( bonePtr[piBoneRefs[iBoneIndex]].second.matrix[0], v->vertCoords ) + bonePtr[piBoneRefs[iBoneIndex]].second.matrix[0][3] );
 				pTri[j][1] += fBoneWeight * ( DotProduct( bonePtr[piBoneRefs[iBoneIndex]].second.matrix[1], v->vertCoords ) + bonePtr[piBoneRefs[iBoneIndex]].second.matrix[1][3] );
 				pTri[j][2] += fBoneWeight * ( DotProduct( bonePtr[piBoneRefs[iBoneIndex]].second.matrix[2], v->vertCoords ) + bonePtr[piBoneRefs[iBoneIndex]].second.matrix[2][3] );
 			}
 			
 			v++;// = (mdxmVertex_t *)&v->weights[/*v->numWeights*/surface->maxVertBoneWeights];
 		}

 		// clear out used arrays
 		memset( axes, 0, sizeof( axes ) );
 		memset( sides, 0, sizeof( sides ) );

 		// work out actual sides of the tag triangle
 		for ( j = 0; j < 3; j++ )
 		{
 			sides[j][0] = pTri[(j+1)%3][0] - pTri[j][0];
 			sides[j][1] = pTri[(j+1)%3][1] - pTri[j][1];
 			sides[j][2] = pTri[(j+1)%3][2] - pTri[j][2];
 		}

 		// do math trig to work out what the matrix will be from this triangle's translated position
 		VectorNormalize2( sides[iG2_TRISIDE_LONGEST], axes[0] );
 		VectorNormalize2( sides[iG2_TRISIDE_SHORTEST], axes[1] );

 		// project shortest side so that it is exactly 90 degrees to the longer side
 		d = DotProduct( axes[0], axes[1] );
 		VectorMA( axes[0], -d, axes[1], axes[0] );
 		VectorNormalize2( axes[0], axes[0] );

 		CrossProduct( sides[iG2_TRISIDE_LONGEST], sides[iG2_TRISIDE_SHORTEST], axes[2] );
 		VectorNormalize2( axes[2], axes[2] );

 		// set up location in world space of the origin point in out going matrix
 		boltList[boltNum].position.matrix[0][3] = pTri[MDX_TAG_ORIGIN][0];
 		boltList[boltNum].position.matrix[1][3] = pTri[MDX_TAG_ORIGIN][1];
 		boltList[boltNum].position.matrix[2][3] = pTri[MDX_TAG_ORIGIN][2];

 		// copy axis to matrix - do some magic to orient minus Y to positive X and so on so bolt on stuff is oriented correctly
		boltList[boltNum].position.matrix[0][0] = axes[1][0];
		boltList[boltNum].position.matrix[0][1] = axes[0][0];
		boltList[boltNum].position.matrix[0][2] = -axes[2][0];

		boltList[boltNum].position.matrix[1][0] = axes[1][1];
		boltList[boltNum].position.matrix[1][1] = axes[0][1];
		boltList[boltNum].position.matrix[1][2] = -axes[2][1];

		boltList[boltNum].position.matrix[2][0] = axes[1][2];
		boltList[boltNum].position.matrix[2][1] = axes[0][2];
		boltList[boltNum].position.matrix[2][2] = -axes[2][2];
	}

}

 
// now go through all the generated surfaces that aren't included in the model surface hierarchy and create the correct bolt info for them				  
void G2_ProcessGeneratedSurfaceBolts(CGhoul2Info &ghoul2, mdxaBone_v &bonePtr, model_t *mod_t)
{
	// look through the surfaces off the end of the pre-defined model surfaces
	for (int i=0; i< ghoul2.mSlist.size(); i++)
	{
		// only look for bolts if we are actually a generated surface, and not just an overriden one
		if (ghoul2.mSlist[i].offFlags & G2SURFACEFLAG_GENERATED)
		{
	   		// well alrighty then. Lets see if there is a bolt that is attempting to use it
			int boltNum = G2_Find_Bolt_Surface_Num(ghoul2.mBltlist, i, G2SURFACEFLAG_GENERATED);
			// yes - ok, processing time.
			if (boltNum != -1)
			{
				G2_ProcessSurfaceBolt(bonePtr, NULL, boltNum, ghoul2.mBltlist, &ghoul2.mSlist[i], mod_t);
			}
		}
	}
}

#ifndef DEDICATED
// set up each surface ready for rendering in the back end
void RenderSurfaces(CRenderSurface &RS)
{
	int			i;
	shader_t	*shader = 0;
	int			offFlags = 0;
	
	// back track and get the surfinfo struct for this surface
	mdxmSurface_t			*surface = (mdxmSurface_t *)G2_FindSurface((void *)RS.currentModel, RS.surfaceNum, RS.lod);
	mdxmHierarchyOffsets_t	*surfIndexes = (mdxmHierarchyOffsets_t *)((byte *)RS.currentModel->mdxm + sizeof(mdxmHeader_t));
	mdxmSurfHierarchy_t		*surfInfo = (mdxmSurfHierarchy_t *)((byte *)surfIndexes + surfIndexes->offsets[surface->thisSurfaceIndex]);
	
	// see if we have an override surface in the surface list
	surfaceInfo_t	*surfOverride = G2_FindOverrideSurface(RS.surfaceNum, RS.rootSList);

	// really, we should use the default flags for this surface unless it's been overriden
	offFlags = surfInfo->flags;

	// set the off flags if we have some
	if (surfOverride)
	{
		offFlags = surfOverride->offFlags;
	}

	// if this surface is not off, add it to the shader render list
	if (!offFlags)
	{	
 		if ( RS.cust_shader ) 
		{
			shader = RS.cust_shader;
		} 
		else if ( RS.skin ) 
		{
			int		j;
			
			// match the surface name to something in the skin file
			shader = tr.defaultShader;
			for ( j = 0 ; j < RS.skin->numSurfaces ; j++ )
			{
				// the names have both been lowercased
				if ( !strcmp( RS.skin->surfaces[j]->name, surfInfo->name ) ) 
				{
					shader = RS.skin->surfaces[j]->shader;
					break;
				}
			}
		} 
		else 
		{
			shader = R_GetShaderByHandle( surfInfo->shaderIndex );
		}
		// we will add shadows even if the main object isn't visible in the view

		// stencil shadows can't do personal models unless I polyhedron clip
		if ( !RS.personalModel
			&& r_shadows->integer == 2 
			&& RS.fogNum == 0
			&& !(RS.renderfx & ( RF_NOSHADOW | RF_DEPTHHACK ) ) 
			&& shader->sort == SS_OPAQUE ) 
		{		// set the surface info to point at the where the transformed bone list is going to be for when the surface gets rendered out
			CRenderableSurface *newSurf = new CRenderableSurface;
			newSurf->surfaceData = surface;
			newSurf->boneList = &RS.bonePtr;
			R_AddDrawSurf( (surfaceType_t *)newSurf, tr.shadowShader, 0, qfalse );
		}

		// projection shadows work fine with personal models
		if ( r_shadows->integer == 3
			&& RS.fogNum == 0
			&& (RS.renderfx & RF_SHADOW_PLANE )
			&& shader->sort == SS_OPAQUE ) 
		{		// set the surface info to point at the where the transformed bone list is going to be for when the surface gets rendered out
			CRenderableSurface *newSurf = new CRenderableSurface;
			newSurf->surfaceData = surface;
			newSurf->boneList = &RS.bonePtr;
			R_AddDrawSurf( (surfaceType_t *)newSurf, tr.projectionShadowShader, 0, qfalse );
		}

		// don't add third_person objects if not viewing through a portal
		if ( !RS.personalModel ) 
		{		// set the surface info to point at the where the transformed bone list is going to be for when the surface gets rendered out
			CRenderableSurface *newSurf = new CRenderableSurface;
			newSurf->surfaceData = surface;
			newSurf->boneList = &RS.bonePtr;
			R_AddDrawSurf( (surfaceType_t *)newSurf, shader, RS.fogNum, qfalse );
		}
	}
	
	// is this surface considered a bolt surface?
	if (offFlags & G2SURFACEFLAG_ISBOLT)
	{
		// well alrighty then. Lets see if there is a bolt that is attempting to use it
		int boltNum = G2_Find_Bolt_Surface_Num(RS.boltList, RS.surfaceNum, 0);
		// yes - ok, processing time.
		if (boltNum != -1)
		{
			G2_ProcessSurfaceBolt(RS.bonePtr, surface, boltNum, RS.boltList, surfOverride, RS.currentModel);
		}
	}
	
	// if we are turning off all descendants, then stop this recursion now
	if (offFlags & G2SURFACEFLAG_NODESCENDANTS)
	{
		return;
	}

	// now recursively call for the children
	for (i=0; i< surfInfo->numChildren; i++)
	{
		RS.surfaceNum = surfInfo->childIndexes[i];
		RenderSurfaces(RS);
	}
}
#endif //!DEDICATED

// Go through the model and deal with just the surfaces that are tagged as bolt on points - this is for the server side skeleton construction
void ProcessModelBoltSurfaces(int surfaceNum, surfaceInfo_v &rootSList,
					mdxaBone_v &bonePtr, model_t *currentModel, int lod, boltInfo_v &boltList)
{
	int			i;
	int			offFlags = 0;
	
	// back track and get the surfinfo struct for this surface
	mdxmSurface_t			*surface = (mdxmSurface_t *)G2_FindSurface((void *)currentModel, surfaceNum, 0);
	mdxmHierarchyOffsets_t	*surfIndexes = (mdxmHierarchyOffsets_t *)((byte *)currentModel->mdxm + sizeof(mdxmHeader_t));
	mdxmSurfHierarchy_t		*surfInfo = (mdxmSurfHierarchy_t *)((byte *)surfIndexes + surfIndexes->offsets[surface->thisSurfaceIndex]);
	
	// see if we have an override surface in the surface list
	surfaceInfo_t	*surfOverride = G2_FindOverrideSurface(surfaceNum, rootSList);

	// really, we should use the default flags for this surface unless it's been overriden
	offFlags = surfInfo->flags;

	// set the off flags if we have some
	if (surfOverride)
	{
		offFlags = surfOverride->offFlags;
	}

	// is this surface considered a bolt surface?
	if (surfInfo->flags & G2SURFACEFLAG_ISBOLT)
	{
		// well alrighty then. Lets see if there is a bolt that is attempting to use it
		int boltNum = G2_Find_Bolt_Surface_Num(boltList, surfaceNum, 0);
		// yes - ok, processing time.
		if (boltNum != -1)
		{
			G2_ProcessSurfaceBolt(bonePtr, surface, boltNum, boltList, surfOverride, currentModel);
		}
	}
	
	// if we are turning off all descendants, then stop this recursion now
	if (offFlags & G2SURFACEFLAG_NODESCENDANTS)
	{
		return;
	}

	// now recursively call for the children
	for (i=0; i< surfInfo->numChildren; i++)
	{
		ProcessModelBoltSurfaces(surfInfo->childIndexes[i], rootSList, bonePtr, currentModel, lod, boltList);
	}
}


// build the used bone list so when doing bone transforms we can determine if we need to do it or not
void G2_ConstructUsedBoneList(CConstructBoneList &CBL)
{
	int	 		i, j;
	int			offFlags = 0;

	// back track and get the surfinfo struct for this surface
	const mdxmSurface_t			*surface = (mdxmSurface_t *)G2_FindSurface((void *)CBL.currentModel, CBL.surfaceNum, 0);
	const mdxmHierarchyOffsets_t	*surfIndexes = (mdxmHierarchyOffsets_t *)((byte *)CBL.currentModel->mdxm + sizeof(mdxmHeader_t));
	const mdxmSurfHierarchy_t	*surfInfo = (mdxmSurfHierarchy_t *)((byte *)surfIndexes + surfIndexes->offsets[surface->thisSurfaceIndex]);
	const model_t				*mod_a = R_GetModelByHandle(CBL.currentModel->mdxm->animIndex);
	const mdxaSkelOffsets_t		*offsets = (mdxaSkelOffsets_t *)((byte *)mod_a->mdxa + sizeof(mdxaHeader_t));
	const mdxaSkel_t			*skel, *childSkel;

	// see if we have an override surface in the surface list
	const surfaceInfo_t	*surfOverride = G2_FindOverrideSurface(CBL.surfaceNum, CBL.rootSList);

	// really, we should use the default flags for this surface unless it's been overriden
	offFlags = surfInfo->flags;

	// set the off flags if we have some
	if (surfOverride)
	{
		offFlags = surfOverride->offFlags;
	}

	// if this surface is not off, add it to the shader render list
	if (!(offFlags & G2SURFACEFLAG_OFF))
	{
		int	*bonesReferenced = (int *)((byte*)surface + surface->ofsBoneReferences);
		// now whip through the bones this surface uses
		for (i=0; i<surface->numBoneReferences;i++)
		{
			int iBoneIndex = bonesReferenced[i];
			CBL.boneUsedList[iBoneIndex] = 1;

			// now go and check all the descendant bones attached to this bone and see if any have the always flag on them. If so, activate them
 			skel = (mdxaSkel_t *)((byte *)mod_a->mdxa + sizeof(mdxaHeader_t) + offsets->offsets[iBoneIndex]);

			// for every child bone...
			for (j=0; j< skel->numChildren; j++)
			{
				// get the skel data struct for each child bone of the referenced bone
 				childSkel = (mdxaSkel_t *)((byte *)mod_a->mdxa + sizeof(mdxaHeader_t) + offsets->offsets[skel->children[j]]);

				// does it have the always on flag on?
				if (childSkel->flags & G2BONEFLAG_ALWAYSXFORM)
				{
					// yes, make sure it's in the list of bones to be transformed.
					CBL.boneUsedList[skel->children[j]] = 1;
				}
			}

			// now we need to ensure that the parents of this bone are actually active...
			//			
			int iParentBone = skel->parent;
			while (iParentBone != -1)
			{	
				if (CBL.boneUsedList[iParentBone])	// no need to go higher
					break;
				CBL.boneUsedList[iParentBone] = 1;
				skel = (mdxaSkel_t *)((byte *)mod_a->mdxa + sizeof(mdxaHeader_t) + offsets->offsets[iParentBone]);				
				iParentBone = skel->parent;
			}
		}
	}
 	else
	// if we are turning off all descendants, then stop this recursion now
	if (offFlags & G2SURFACEFLAG_NODESCENDANTS)
	{
		return;
	}

	// now recursively call for the children
	for (i=0; i< surfInfo->numChildren; i++)
	{
		CBL.surfaceNum = surfInfo->childIndexes[i];
		G2_ConstructUsedBoneList(CBL);
	}
}


// sort all the ghoul models in this list so if they go in reference order. This will ensure the bolt on's are attached to the right place
// on the previous model, since it ensures the model being attached to is built and rendered first.

// NOTE!! This assumes at least one model will NOT have a parent. If it does - we are screwed
static void G2_Sort_Models(CGhoul2Info_v &ghoul2, int * const modelList, int * const modelCount)
{
	int		startPoint, endPoint;
	int		i, boltTo, j;

	*modelCount = 0;

	// first walk all the possible ghoul2 models, and stuff the out array with those with no parents
	for (i=0; i<ghoul2.size();i++)
	{
		// have a ghoul model here?
		if (ghoul2[i].mModelindex == -1)
		{
			continue;
		}
		// are we attached to anything?
		if (ghoul2[i].mModelBoltLink == -1)
		{
			// no, insert us first
			modelList[(*modelCount)++] = i;
	 	}
	}

	startPoint = 0;
	endPoint = *modelCount;

	// now, using that list of parentless models, walk the descendant tree for each of them, inserting the descendents in the list
	while (startPoint != endPoint)
	{
		for (i=0; i<ghoul2.size(); i++)
		{
			// have a ghoul model here?
			if (ghoul2[i].mModelindex == -1)
			{
				continue;
			}

			// what does this model think it's attached to?
			if (ghoul2[i].mModelBoltLink != -1)
			{
				boltTo = (ghoul2[i].mModelBoltLink >> MODEL_SHIFT) & MODEL_AND;
				// is it any of the models we just added to the list?
				for (j=startPoint; j<endPoint; j++)
				{
					// is this my parent model?
					if (boltTo == modelList[j])
					{
						// yes, insert into list and exit now
						modelList[(*modelCount)++] = i;
						break;
					}
				}
			}
		}
		// update start and end points
		startPoint = endPoint;
		endPoint = *modelCount;
	}
}

const static mdxaBone_t		identityMatrix = 
{ 
	0.0f, -1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f
};

/*
==============
R_AddGHOULSurfaces
==============
*/

void R_AddGhoulSurfaces( trRefEntity_t *ent ) {
	mdxaHeader_t	*aHeader;
	shader_t		*cust_shader = 0;
	int				fogNum = 0;
	qboolean		personalModel;
	int				cull;
	int				i, whichLod, j;
	skin_t			*skin;
	model_t			*currentModel;
	model_t			*animModel;
	int				modelCount;
	int				*modelList;
	mdxaBone_t		rootMatrix;
	bool			setNewOrigin = false;
	CGhoul2Info_v	&ghoul2 = *((CGhoul2Info_v *)ent->e.ghoul2);

	// if we don't want server ghoul2 models and this is one, or we just don't want ghoul2 models at all, then return
	if ((r_noServerGhoul2->integer && !(ghoul2[0].mCreationID & WF_CLIENTONLY)) || (r_noGhoul2->integer))
	{
		return;
	}

	// cull the entire model if merged bounding box of both frames
	// is outside the view frustum.
	cull = R_GCullModel (ent );
	if ( cull == CULL_OUT ) 
	{
		return;
	}

	// are any of these models setting a new origin?
	for (i=0; i<ghoul2.size(); i++)
	{
		if (ghoul2[i].mModelindex != -1)
		{
			if (ghoul2[i].mFlags & GHOUL2_NEWORIGIN)
			{
				// yes. ooooookkkkkk - now we have to do a GetBoltMatrix for that bone on that model, so we can get it's data into the root matrix for this pass
				// this will call this same function recursively, so we set checkForNewOrigin to false on the GetBoltMatrixCall so we don't do this loop
				G2_ConstructGhoulSkeleton(ghoul2, tr.refdef.time -100, 0,false, ent->e.angles, ent->e.origin, ent->e.modelScale, true); 

				// scale the bolt position by the scale factor for this model since at this point its still in model space
				if (ent->e.modelScale[0])
				{
					ghoul2[i].mBltlist[ghoul2[i].mNewOrigin].position.matrix[0][3] *= ent->e.modelScale[0];
				}
				if (ent->e.modelScale[1])
				{
					ghoul2[i].mBltlist[ghoul2[i].mNewOrigin].position.matrix[1][3] *= ent->e.modelScale[1];
				}
				if (ent->e.modelScale[2])
				{ 
					ghoul2[i].mBltlist[ghoul2[i].mNewOrigin].position.matrix[2][3] *= ent->e.modelScale[2];
				}

				VectorNormalize((float*)&ghoul2[i].mBltlist[ghoul2[i].mNewOrigin].position.matrix[0]);
				VectorNormalize((float*)&ghoul2[i].mBltlist[ghoul2[i].mNewOrigin].position.matrix[1]);
				VectorNormalize((float*)&ghoul2[i].mBltlist[ghoul2[i].mNewOrigin].position.matrix[2]);
				mdxaBone_t		tempMatrix;
				Inverse_Matrix(&ghoul2[i].mBltlist[ghoul2[i].mNewOrigin].position, &tempMatrix);
				Multiply_3x4Matrix(&rootMatrix, &tempMatrix, (mdxaBone_t*)&identityMatrix);

				setNewOrigin = true;
			}
		}
	}

   	// don't add third_person objects if not in a portal
	personalModel = (qboolean)((ent->e.renderfx & RF_THIRD_PERSON) && !tr.viewParms.isPortal);

	modelList = (int*)Z_Malloc(ghoul2.size() * 4, TAG_GHOUL2, qtrue);
#ifndef DEDICATED
	// set up lighting now that we know we aren't culled
	if ( !personalModel || r_shadows->integer > 1 ) 
	{
		// FIXME!! Is there something here we should be looking at?
		R_SetupEntityLighting( &tr.refdef, ent );
	}

	// see if we are in a fog volume
	fogNum = R_GComputeFogNum( ent );
#endif // !DEDICATED

	// order sort the ghoul 2 models so bolt ons get bolted to the right model
	G2_Sort_Models(ghoul2, modelList, &modelCount);

	// construct a world matrix for this entity
	G2_GenerateWorldMatrix(ent->e.angles, ent->e.origin);

	// walk each possible model for this entity and try rendering it out
	for (j=0; j<modelCount; j++)
	{
		// get the sorted model to play with
		i = modelList[j];

		// do we really really want to deal with this model?
		if (!(ghoul2[i].mFlags & GHOUL2_NOMODEL))
		{
			currentModel = R_GetModelByHandle(ghoul2[i].mModel);
			animModel =  R_GetModelByHandle(currentModel->mdxm->animIndex);
			aHeader = animModel->mdxa;
 #ifndef DEDICATED 		
			//
			// figure out whether we should be using a custom shader for this model
			//
			skin = NULL;
			if (ent->e.customShader)
			{
				cust_shader = R_GetShaderByHandle(ent->e.customShader );
			}
			else
			{
				cust_shader = NULL;
				// figure out the custom skin thing
				if (ghoul2[i].mCustomSkin)
				{
					skin = R_GetSkinByHandle(ghoul2[i].mCustomSkin );
				}
				else if (ent->e.customSkin)
				{
					skin = R_GetSkinByHandle(ent->e.customSkin );
				}
				else if ( ghoul2[i].mSkin > 0 && ghoul2[i].mSkin < tr.numSkins ) 
				{
					skin = R_GetSkinByHandle( ghoul2[i].mSkin );
				}
			}
#endif //!DEDICATED

			if (ghoul2[i].mSkelFrameNum != tr.refdef.time)
			{
				int	*boneUsedList;

				ghoul2[i].mSkelFrameNum = tr.refdef.time;

				// construct a list of all bones used by this model - this makes the bone transform go a bit faster since it will dump out bones
				// that aren't being used. - NOTE this will screw up any models that have surfaces turned off where the lower surfaces aren't.
				boneUsedList = (int *)Z_Malloc(animModel->mdxa->numBones * 4, TAG_GHOUL2, qtrue);			
				memset(boneUsedList, 0, (animModel->mdxa->numBones * 4));

				CConstructBoneList	CBL(ghoul2[i].mSurfaceRoot, 
					boneUsedList,
					ghoul2[i].mSlist,
					currentModel,
					ghoul2[i].mBlist
					);

				G2_ConstructUsedBoneList(CBL);

				if (!ghoul2[i].mSurfaceRoot)
				{
					// make sure the root bone is marked as being referenced
					boneUsedList[0] =1;
				}

				if (ghoul2[i].mTempBoneList.size()!=animModel->mdxa->numBones+1)
				{
					ghoul2[i].mTempBoneList.resize(animModel->mdxa->numBones+1);
					int k;
					for (k=0;k<animModel->mdxa->numBones;k++)
					{
						ghoul2[i].mTempBoneList[k].first=-10000; //reset it to an invalid time
					}	
				}
				
				// if this is the root model, and we have a new root matrix because the model has a new origin, use that
				if (!setNewOrigin || j)
				{
					// decide what to do about the root matrix
					if (ghoul2[i].mModelBoltLink == -1)
					{
						// we aren't bolted to anything
						rootMatrix = identityMatrix;
					}
					// yes we are bolted to another model, better use the bolt as the root matrix then, so our model is offset correctly for the bone bolt
					else
					{
						int	boltMod = (ghoul2[i].mModelBoltLink >> MODEL_SHIFT) & MODEL_AND;
						int	boltNum = (ghoul2[i].mModelBoltLink >> BOLT_SHIFT) & BOLT_AND;
						rootMatrix = ghoul2[boltMod].mBltlist[boltNum].position;
					}
				}

				mdxaBone_v oldBones;
				if (r_Ghoul2AnimSmooth&&r_Ghoul2AnimSmooth->value>0.005f&&r_Ghoul2AnimSmooth->value<0.995f)
				{
					oldBones=ghoul2[i].mTempBoneList;
				}
				// pre-transform all the bones of this model
				G2_TransformGhoulBones( aHeader, boneUsedList, ghoul2[i].mBlist, ghoul2[i].mTempBoneList, ghoul2[i].mBltlist, rootMatrix, ghoul2[i], tr.refdef.time, animModel->mdxa->numBones);
				if (oldBones.size())
				{
					int b;
					for (b=0;b<animModel->mdxa->numBones;b++)
					{
						if (r_Ghoul2AnimSmooth&&r_Ghoul2AnimSmooth->value>0.005f&&r_Ghoul2AnimSmooth->value<0.995f)
						{
							if (tr.refdef.time-ghoul2[i].mTempBoneList[b].first<200&&tr.refdef.time-ghoul2[i].mTempBoneList[b].first>-200)
							{
								int k;
								float *oldM=&oldBones[b].second.matrix[0][0];
								float *newM=&ghoul2[i].mTempBoneList[b].second.matrix[0][0];
								for (k=0;k<12;k++,oldM++,newM++)
								{
									*newM=r_Ghoul2AnimSmooth->value*(*oldM-*newM)+*newM;
								}
							}
							ghoul2[i].mTempBoneList[b].first=tr.refdef.time;
						}
					}
				}

				if (r_Ghoul2UnSqashAfterSmooth&&r_Ghoul2UnSqashAfterSmooth->value>0.5f)
				{
					mdxaSkelOffsets_t *offsets = (mdxaSkelOffsets_t *)((byte *)aHeader + sizeof(mdxaHeader_t));
					int b;
					for (b=0;b<animModel->mdxa->numBones;b++)
					{
						if (!boneUsedList)
						{
							continue;
						}
						mdxaSkel_t		*skel= (mdxaSkel_t *)((byte *)aHeader + sizeof(mdxaHeader_t) + offsets->offsets[b]);
						mdxaBone_t tempMatrix;
						Multiply_3x4Matrix(&tempMatrix,&ghoul2[i].mTempBoneList[b].second, &skel->BasePoseMat);
						float maxl;
						maxl=VectorLength(&skel->BasePoseMat.matrix[0][0]);
						VectorNormalize(&tempMatrix.matrix[0][0]);
						VectorNormalize(&tempMatrix.matrix[1][0]);
						VectorNormalize(&tempMatrix.matrix[2][0]);

						VectorScale(&tempMatrix.matrix[0][0],maxl,&tempMatrix.matrix[0][0]);
						VectorScale(&tempMatrix.matrix[1][0],maxl,&tempMatrix.matrix[1][0]);
						VectorScale(&tempMatrix.matrix[2][0],maxl,&tempMatrix.matrix[2][0]);
						Multiply_3x4Matrix(&ghoul2[i].mTempBoneList[b].second,&tempMatrix,&skel->BasePoseMatInv);
					}
					
				}

				Z_Free(boneUsedList);
			}
			//
			// compute LOD
   			//
			whichLod = G2_ComputeLOD( ent, currentModel, ghoul2[i].mLodBias );

			// do we really want to render this?
			//-----------------------------------------------------------------------------
			// TEMP TEMP! Adding this in for the cheesy hacked first person mode, basically isn't supposed to render the player when in "first person"...but still renders the saber model
			//-----------------------------------------------------------------------------
			// TEMP TEMP! Adding this in for the cheesy hacked first person mode, basically isn't supposed to render the player when in "first person"...but still renders the saber model
			//-----------------------------------------------------------------------------
			// TEMP TEMP! Adding this in for the cheesy hacked first person mode, basically isn't supposed to render the player when in "first person"...but still renders the saber model
			//-----------------------------------------------------------------------------
			// TEMP TEMP! Adding this in for the cheesy hacked first person mode, basically isn't supposed to render the player when in "first person"...but still renders the saber model
			//-----------------------------------------------------------------------------
			if ( ghoul2[i].mFlags & GHOUL2_NORENDER)// /*temp ->*/|| ( ghoul2[i].mModelBoltLink == -1 && ent->e.renderfx & RF_ALPHA_FADE ))
			{
				// if we didn't render it, then we should still go sort out all the bolt surfaces in the model
				ProcessModelBoltSurfaces(ghoul2[i].mSurfaceRoot, ghoul2[i].mSlist, ghoul2[i].mTempBoneList, currentModel, whichLod, ghoul2[i].mBltlist);
			}
			else
			{ // start the walk of the surface hierarchy	
				CRenderSurface RS(ghoul2[i].mSurfaceRoot, ghoul2[i].mSlist, cust_shader, fogNum, personalModel, ghoul2[i].mTempBoneList, ent->e.renderfx, skin, currentModel, whichLod, ghoul2[i].mBltlist);
#ifndef DEDICATED
				RenderSurfaces(RS);
#endif
			}

			// go through all the generated surfaces and create their bolt info if we need it.
			G2_ProcessGeneratedSurfaceBolts(ghoul2[i], ghoul2[i].mTempBoneList, currentModel);

		}
	}
	Z_Free(modelList);
}	

/*
==============
G2_ConstructGhoulSkeleton - builds a complete skeleton for all ghoul models in a CGhoul2Info_v class	- using LOD 0
==============
*/
void G2_ConstructGhoulSkeleton( CGhoul2Info_v &ghoul2, const int frameNum, qhandle_t *modelPointerList, bool checkForNewOrigin, const vec3_t angles, const vec3_t position, const vec3_t scale, bool modelSet) {
	mdxaHeader_t	*aHeader;
	int				i, j;
	int				*boneUsedList;
	model_t			*currentModel;
	model_t			*animModel;
	int				modelCount;
	int				*modelList;
	bool			setNewOrigin = false;
	mdxaBone_t		rootMatrix;

	// if we don't want server ghoul2 models and this is one, or we just don't want ghoul2 models at all, then return
	if ((r_noServerGhoul2->integer && !(ghoul2[0].mCreationID & WF_CLIENTONLY)) || (r_noGhoul2->integer))
	{
		return;
	}

	// have we already transformed this group of ghoul2 skeletons this frame?
	if (ghoul2[0].mSkelFrameNum == frameNum)
	{
		return;
	}

	// should we be looking to see if any of these models are setting a new origin?
	if (checkForNewOrigin)
	{
		// are any of these models setting a new origin?
		for (int i=0; i<ghoul2.size(); i++)
		{
			if (ghoul2[i].mModelindex != -1)
			{
				if (ghoul2[i].mFlags & GHOUL2_NEWORIGIN)
				{
					// yes. ooooookkkkkk - now we have to do a GetBoltMatrix for that bone on that model, so we can get it's data into the root matrix for this pass
					// this will call this same function recursively, so we set checkForNewOrigin to false on the GetBoltMatrixCall so we don't do this loop
					G2_ConstructGhoulSkeleton(ghoul2, frameNum-100, modelPointerList,false, angles, position, scale, false); 
					// scale the bolt position by the scale factor for this model since at this point its still in model space
					if (scale[0])
					{
						ghoul2[i].mBltlist[ghoul2[i].mNewOrigin].position.matrix[0][3] *= scale[0];
					}
					if (scale[1])
					{
						ghoul2[i].mBltlist[ghoul2[i].mNewOrigin].position.matrix[1][3] *= scale[1];
					}
					if (scale[2])
					{
						ghoul2[i].mBltlist[ghoul2[i].mNewOrigin].position.matrix[2][3] *= scale[2];
					}

					VectorNormalize((float*)&ghoul2[i].mBltlist[ghoul2[i].mNewOrigin].position.matrix[0]);
					VectorNormalize((float*)&ghoul2[i].mBltlist[ghoul2[i].mNewOrigin].position.matrix[1]);
					VectorNormalize((float*)&ghoul2[i].mBltlist[ghoul2[i].mNewOrigin].position.matrix[2]);
					mdxaBone_t		tempMatrix;
					Inverse_Matrix(&ghoul2[i].mBltlist[ghoul2[i].mNewOrigin].position, &tempMatrix);
					Multiply_3x4Matrix(&rootMatrix, &tempMatrix, (mdxaBone_t*)&identityMatrix);
					setNewOrigin = true;
				}
			}
		}
	}

	modelList = (int*)Z_Malloc(ghoul2.size() * 4, TAG_GHOUL2, qtrue);
	// order sort the ghoul 2 models so bolt ons get bolted to the right model
	G2_Sort_Models(ghoul2, modelList, &modelCount);

	// walk each possible model for this entity and try rendering it out
	for (j=0; j<modelCount; j++)
	{
		// get the sorted model to play with
		i = modelList[j];

		// do we really really want to deal with this model?
		if (!(ghoul2[i].mFlags & GHOUL2_NOMODEL))
		{
			if (modelSet)
			{
				currentModel = R_GetModelByHandle(ghoul2[i].mModel);
			}
			else
			if (!ghoul2[i].mFileName[0])
			{
				int iModelIndex = ghoul2[i].mModelindex;
				currentModel = R_GetModelByHandle(modelPointerList[iModelIndex]);
			}
			else
			{
				char *psFilename = ghoul2[i].mFileName;
				currentModel = R_GetModelByHandle(RE_RegisterModel(psFilename));
			}
			animModel =  R_GetModelByHandle(currentModel->mdxm->animIndex);
			aHeader = animModel->mdxa;

			ghoul2[i].mSkelFrameNum = frameNum;
  		
			// construct a list of all bones used by this model - this makes the bone transform go a bit faster since it will dump out bones
			// that aren't being used. - NOTE this will screw up any models that have surfaces turned off where the lower surfaces aren't.
			boneUsedList = (int *)Z_Malloc(animModel->mdxa->numBones * 4, TAG_GHOUL2, qtrue);
			memset(boneUsedList, 0, (animModel->mdxa->numBones * 4));

			CConstructBoneList	CBL(
			ghoul2[i].mSurfaceRoot, 
			boneUsedList,
			ghoul2[i].mSlist,
			currentModel,
			ghoul2[i].mBlist
			);

			G2_ConstructUsedBoneList(CBL);

			if (!ghoul2[i].mSurfaceRoot)
			{
				// make sure the root bone is marked as being referenced
				boneUsedList[0] =1;
			}

			if (ghoul2[i].mTempBoneList.size()!=animModel->mdxa->numBones+1)
			{
				ghoul2[i].mTempBoneList.resize(animModel->mdxa->numBones+1);
				int k;
				for (k=0;k<animModel->mdxa->numBones;k++)
				{
					ghoul2[i].mTempBoneList[k].first=-10000; //reset it to an invalid time
				}	
			}

			// decide what to do about the root matrix

			// if this is the root model, and we have a new root matrix because the model has a new origin, use that
			if (!setNewOrigin || j)
			{
				if (ghoul2[i].mModelBoltLink == -1)
				{
					// we aren't bolted to anything
					rootMatrix = identityMatrix;
				}
				// yes we are bolted to another model, better use the bolt as the root matrix then, so our model is offset correctly for the bone bolt
				else
				{
					int	boltMod = (ghoul2[i].mModelBoltLink >> MODEL_SHIFT) & MODEL_AND;
					int	boltNum = (ghoul2[i].mModelBoltLink >> BOLT_SHIFT) & BOLT_AND;
					rootMatrix = ghoul2[boltMod].mBltlist[boltNum].position;
				}
			}

			mdxaBone_v oldBones;
			if (r_Ghoul2AnimSmooth&&r_Ghoul2AnimSmooth->value>0.005f&&r_Ghoul2AnimSmooth->value<0.995f)
			{
				oldBones=ghoul2[i].mTempBoneList;
			}
			// pre-transform all the bones of this model
			G2_TransformGhoulBones( aHeader, boneUsedList, ghoul2[i].mBlist, ghoul2[i].mTempBoneList, ghoul2[i].mBltlist, rootMatrix, ghoul2[i], frameNum, animModel->mdxa->numBones);

			if (oldBones.size())
			{
				int b;
				for (b=0;b<animModel->mdxa->numBones;b++)
				{
					if (r_Ghoul2AnimSmooth&&r_Ghoul2AnimSmooth->value>0.005f&&r_Ghoul2AnimSmooth->value<0.995f)
					{
						if (tr.refdef.time-ghoul2[i].mTempBoneList[b].first<200&&tr.refdef.time-ghoul2[i].mTempBoneList[b].first>-200)
						{
							int k;
							float *oldM=&oldBones[b].second.matrix[0][0];
							float *newM=&ghoul2[i].mTempBoneList[b].second.matrix[0][0];
							for (k=0;k<12;k++,oldM++,newM++)
							{
								*newM=r_Ghoul2AnimSmooth->value*(*oldM-*newM)+*newM;
							}
						}
						ghoul2[i].mTempBoneList[b].first=tr.refdef.time;
					}
				}
			}

			if (r_Ghoul2UnSqashAfterSmooth&&r_Ghoul2UnSqashAfterSmooth->value>0.5f)
			{
				mdxaSkelOffsets_t *offsets = (mdxaSkelOffsets_t *)((byte *)aHeader + sizeof(mdxaHeader_t));
				int b;
				for (b=0;b<animModel->mdxa->numBones;b++)
				{
					if (!boneUsedList)
					{
						continue;
					}
					mdxaSkel_t		*skel= (mdxaSkel_t *)((byte *)aHeader + sizeof(mdxaHeader_t) + offsets->offsets[b]);
					mdxaBone_t tempMatrix;
					Multiply_3x4Matrix(&tempMatrix,&ghoul2[i].mTempBoneList[b].second, &skel->BasePoseMat);
					float maxl;
					maxl=VectorLength(&skel->BasePoseMat.matrix[0][0]);
					VectorNormalize(&tempMatrix.matrix[0][0]);
					VectorNormalize(&tempMatrix.matrix[1][0]);
					VectorNormalize(&tempMatrix.matrix[2][0]);

					VectorScale(&tempMatrix.matrix[0][0],maxl,&tempMatrix.matrix[0][0]);
					VectorScale(&tempMatrix.matrix[1][0],maxl,&tempMatrix.matrix[1][0]);
					VectorScale(&tempMatrix.matrix[2][0],maxl,&tempMatrix.matrix[2][0]);
					Multiply_3x4Matrix(&ghoul2[i].mTempBoneList[b].second,&tempMatrix,&skel->BasePoseMatInv);
				}
				
			}

			Z_Free(boneUsedList);

			// call function that will go through the main model and generate all the bolts required
			ProcessModelBoltSurfaces(ghoul2[i].mSurfaceRoot, ghoul2[i].mSlist, ghoul2[i].mTempBoneList, currentModel, 0, ghoul2[i].mBltlist);

			// go through all the generated surfaces and create their bolt info if we need it.
			G2_ProcessGeneratedSurfaceBolts(ghoul2[i], ghoul2[i].mTempBoneList, currentModel);

		}
	}
	Z_Free(modelList);
	return;
}	

#ifndef DEDICATED
/*
==============
RB_SurfaceGhoul
==============
*/
void RB_SurfaceGhoul( CRenderableSurface *surf ) {
	int				 j, k;

	// grab the pointer to the surface info within the loaded mesh file
	mdxmSurface_t	*surface = (mdxmSurface_t *)surf->surfaceData;

	// point us at the bone structure that should have been pre-computed
	mdxaBone_v &bonePtr = *((mdxaBone_v *)surf->boneList);

	delete surf;
	//
	// deform the vertexes by the lerped bones
	//
   
	// first up, sanity check our numbers
	RB_CheckOverflow( surface->numVerts, surface->numTriangles );

	// now copy the right number of verts to the temporary area for verts for this shader
	const int baseVertex = tess.numVertexes;
	const int *triangles = (int *) ((byte *)surface + surface->ofsTriangles);
	const int baseIndex = tess.numIndexes;
#if 0
	const int indexes = surface->numTriangles * 3;
	for (j = 0 ; j < indexes ; j++) {
		tess.indexes[baseIndex + j] = baseVertex + triangles[j];
	}
	tess.numIndexes += indexes;
#else
	const int indexes = surface->numTriangles; //*3;	//unrolled 3 times, don't multiply
	unsigned int * tessIndexes = &tess.indexes[baseIndex];
	for (j = 0 ; j < indexes ; j++) {
		*tessIndexes++ = baseVertex + *triangles++;
		*tessIndexes++ = baseVertex + *triangles++;
		*tessIndexes++ = baseVertex + *triangles++;
	}
	tess.numIndexes += indexes*3;
#endif

	
	// whip through and actually transform each vertex

	const int *piBoneRefs = (int*) ((byte*)surface + surface->ofsBoneReferences);		
	const int numVerts = surface->numVerts;
	const mdxmVertex_t 	*v = (mdxmVertex_t *) ((byte *)surface + surface->ofsVerts);
	mdxmVertexTexCoord_t *pTexCoords = (mdxmVertexTexCoord_t *) &v[numVerts];

	int baseVert = tess.numVertexes;
	for ( j = 0; j < numVerts; j++, baseVert++ ) 
	{
		const int iNumWeights = G2_GetVertWeights( v );
//		const mdxmWeight_t	*w = v->weights;
		VectorClear( tess.xyz[baseVert]);
		VectorClear( tess.normal[baseVert]);

		float fTotalWeight = 0.0f;
		for ( k = 0 ; k < iNumWeights ; k++ ) 
		{
			int		iBoneIndex	= G2_GetVertBoneIndex( v, k );
			float	fBoneWeight	= G2_GetVertBoneWeight( v, k, fTotalWeight, iNumWeights );

			mdxaBone_t &bone = bonePtr[piBoneRefs[iBoneIndex]].second;

			tess.xyz[baseVert][0] += fBoneWeight * ( DotProduct( bone.matrix[0], v->vertCoords ) + bone.matrix[0][3] );
			tess.xyz[baseVert][1] += fBoneWeight * ( DotProduct( bone.matrix[1], v->vertCoords ) + bone.matrix[1][3] );
			tess.xyz[baseVert][2] += fBoneWeight * ( DotProduct( bone.matrix[2], v->vertCoords ) + bone.matrix[2][3] );

			tess.normal[baseVert][0] += fBoneWeight * DotProduct( bone.matrix[0], v->normal );
			tess.normal[baseVert][1] += fBoneWeight * DotProduct( bone.matrix[1], v->normal );
			tess.normal[baseVert][2] += fBoneWeight * DotProduct( bone.matrix[2], v->normal );
		}

		tess.texCoords[baseVert][0][0] = pTexCoords[j].texCoords[0];
		tess.texCoords[baseVert][0][1] = pTexCoords[j].texCoords[1];

		v++;// = (mdxmVertex_t *)&v->weights[/*v->numWeights*/surface->maxVertBoneWeights];
	}

	tess.numVertexes += surface->numVerts;
}
#endif // !DEDICATED
 
/*
=================
R_LoadMDXM - load a Ghoul 2 Mesh file
=================
*/
qboolean R_LoadMDXM( model_t *mod, void *buffer, const char *mod_name, qboolean bAlreadyCached ) {
	int					i,l, j;
	mdxmHeader_t		*pinmodel, *mdxm;
	mdxmLOD_t			*lod;
	mdxmSurface_t		*surf;
	int					version;
	int					size;
	shader_t			*sh;
	mdxmSurfHierarchy_t	*surfInfo;

#ifndef _M_IX86
	int					k;
	int					frameSize;
	mdxmTag_t			*tag;
	mdxmTriangle_t		*tri;
	mdxmVertex_t		*v;
 	mdxmFrame_t			*cframe;
	int					*boneRef;
#endif
    
	pinmodel= (mdxmHeader_t *)buffer;
	//
	// read some fields from the binary, but only LittleLong() them when we know this wasn't an already-cached model...
	//	
	version = (pinmodel->version);
	size	= (pinmodel->ofsEnd);

	if (!bAlreadyCached)
	{
		version = LittleLong(version);
		size	= LittleLong(size);
	}

	if (version != MDXM_VERSION) {
		ri.Printf( PRINT_WARNING, "R_LoadMDXM: %s has wrong version (%i should be %i)\n",
				 mod_name, version, MDXM_VERSION);
		return qfalse;
	}

	mod->type	   = MOD_MDXM;
	mod->dataSize += size;	
	
	qboolean bAlreadyFound = qfalse;
	mdxm = mod->mdxm = (mdxmHeader_t*) //ri.Hunk_Alloc( size );
										RE_RegisterModels_Malloc(size, mod_name, &bAlreadyFound, TAG_MODEL_GLM);

	assert(bAlreadyCached == bAlreadyFound);	// I should probably eliminate 'bAlreadyFound', but wtf?

	if (!bAlreadyFound)
	{
		memcpy( mdxm, buffer, size );	

		LL(mdxm->ident);
		LL(mdxm->version);
		LL(mdxm->numLODs);
		LL(mdxm->ofsLODs);
		LL(mdxm->numSurfaces);
		LL(mdxm->ofsSurfHierarchy);
		LL(mdxm->ofsEnd);
	}
		
	// first up, go load in the animation file we need that has the skeletal animation info for this model
	mdxm->animIndex = RE_RegisterModel(va ("%s.gla",mdxm->animName));
	if (!mdxm->animIndex) 
	{
		ri.Printf( PRINT_WARNING, "R_LoadMDXM: missing animation file %s for mesh %s\n", mdxm->animName, mdxm->name);
		return qfalse;
	}

	mod->numLods = mdxm->numLODs -1 ;	//copy this up to the model for ease of use - it wil get inced after this.

	if (bAlreadyFound)
	{
		return qtrue;	// All done. Stop, go no further, do not LittleLong(), do not pass Go...
	}

	surfInfo = (mdxmSurfHierarchy_t *)( (byte *)mdxm + mdxm->ofsSurfHierarchy);
 	for ( i = 0 ; i < mdxm->numSurfaces ; i++) 
	{
		LL(surfInfo->numChildren);
		LL(surfInfo->parentIndex);

		// do all the children indexs
		for (j=0; j<surfInfo->numChildren; j++)
		{
			LL(surfInfo->childIndexes[j]);
		}
#ifndef DEDICATED
		// get the shader name
		sh = R_FindShader( surfInfo->shader, lightmapsNone, stylesDefault, qtrue );
		// insert it in the surface list
#endif		
		if ( sh->defaultShader ) 
		{
			surfInfo->shaderIndex = 0;
		}
		else
		{
			surfInfo->shaderIndex = sh->index;
		}
		RE_RegisterModels_StoreShaderRequest(mod_name, &surfInfo->shader[0], &surfInfo->shaderIndex);		

		// find the next surface
		surfInfo = (mdxmSurfHierarchy_t *)( (byte *)surfInfo + (int)( &((mdxmSurfHierarchy_t *)0)->childIndexes[ surfInfo->numChildren ] ));
  	}
	
#if _DEBUG
	ri.Printf(0, "For Ghoul2 mesh file %s\n", mod_name);
#endif
	// swap all the LOD's	(we need to do the middle part of this even for intel, because of shader reg and err-check)
	lod = (mdxmLOD_t *) ( (byte *)mdxm + mdxm->ofsLODs );
	for ( l = 0 ; l < mdxm->numLODs ; l++)
	{
		int	triCount = 0;

		LL(lod->ofsEnd);
		// swap all the surfaces
		surf = (mdxmSurface_t *) ( (byte *)lod + sizeof (mdxmLOD_t) + (mdxm->numSurfaces * sizeof(mdxmLODSurfOffset_t)) );
		for ( i = 0 ; i < mdxm->numSurfaces ; i++) 
		{
			LL(surf->numTriangles);
			LL(surf->ofsTriangles);
			LL(surf->numVerts);
			LL(surf->ofsVerts);
			LL(surf->ofsEnd);
			LL(surf->ofsHeader);
			LL(surf->numBoneReferences);
			LL(surf->ofsBoneReferences);
//			LL(surf->maxVertBoneWeights);

			triCount += surf->numTriangles;
										
			if ( surf->numVerts > SHADER_MAX_VERTEXES ) {
				ri.Error (ERR_DROP, "R_LoadMDXM: %s has more than %i verts on a surface (%i)",
					mod_name, SHADER_MAX_VERTEXES, surf->numVerts );
			}
			if ( surf->numTriangles*3 > SHADER_MAX_INDEXES ) {
				ri.Error (ERR_DROP, "R_LoadMDXM: %s has more than %i triangles on a surface (%i)",
					mod_name, SHADER_MAX_INDEXES / 3, surf->numTriangles );
			}
		
			// change to surface identifier
			surf->ident = SF_MDX;

			// register the shaders
#ifndef _M_IX86
//
// optimisation, we don't bother doing this for standard intel case since our data's already in that format...
//
			// FIXME - is this correct? 
			// do all the bone reference data
			boneRef = (int *) ( (byte *)surf + surf->ofsBoneReferences );
			for ( j = 0 ; j < surf->numBoneReferences ; j++ ) 
			{
					LL(boneRef[j]);
			}

			
			// swap all the triangles
			tri = (mdxmTriangle_t *) ( (byte *)surf + surf->ofsTriangles );
			for ( j = 0 ; j < surf->numTriangles ; j++, tri++ ) 
			{
				LL(tri->indexes[0]);
				LL(tri->indexes[1]);
				LL(tri->indexes[2]);
			}

			// swap all the vertexes
			v = (mdxmVertex_t *) ( (byte *)surf + surf->ofsVerts );
			for ( j = 0 ; j < surf->numVerts ; j++ ) 
			{
				v->normal[0] = LittleFloat( v->normal[0] );
				v->normal[1] = LittleFloat( v->normal[1] );
				v->normal[2] = LittleFloat( v->normal[2] );

				v->texCoords[0] = LittleFloat( v->texCoords[0] );
				v->texCoords[1] = LittleFloat( v->texCoords[1] );

				v->numWeights = LittleLong( v->numWeights );
  			    v->offset[0] = LittleFloat( v->offset[0] );
				v->offset[1] = LittleFloat( v->offset[1] );
				v->offset[2] = LittleFloat( v->offset[2] );

				for ( k = 0 ; k < /*v->numWeights*/surf->maxVertBoneWeights ; k++ ) 
				{
					v->weights[k].boneIndex = LittleLong( v->weights[k].boneIndex );
					v->weights[k].boneWeight = LittleFloat( v->weights[k].boneWeight );
				}
				v = (mdxmVertex_t *)&v->weights[/*v->numWeights*/surf->maxVertBoneWeights];
			}
#endif

			// find the next surface
			surf = (mdxmSurface_t *)( (byte *)surf + surf->ofsEnd );
		}
#if _DEBUG
		ri.Printf(0, "Lod %d has %d tris in %d surfaces with %d bones\n", l, triCount, mdxm->numSurfaces, mdxm->numBones);
#endif
		// find the next LOD
		lod = (mdxmLOD_t *)( (byte *)lod + lod->ofsEnd );
	}


// This is junk, there's no such structure...
//
//#ifndef _M_IX86
////
//// optimisation, we don't bother doing this for standard intel case since our data's already in that format...
////
//	tag = (mdxmTag_t *) ( (byte *)mdxm + mdxm->ofsTags );
//	for ( i = 0 ; i < md4->numTags ; i++) {
//		LL(tag->boneIndex);
//		tag++;
//	}
//#endif

	return qtrue;
}

/*
=================
R_LoadMDXA - load a Ghoul 2 animation file
=================
*/
qboolean R_LoadMDXA( model_t *mod, void *buffer, const char *mod_name, qboolean bAlreadyCached ) {

	mdxaHeader_t		*pinmodel, *mdxa;
	int					version;
	int					size;

#ifndef _M_IX86
	int					j, k, i;
	int					frameSize;
	mdxaFrame_t			*cframe;
	mdxaSkel_t			*boneInfo;
#endif

 	pinmodel = (mdxaHeader_t *)buffer;
	//
	// read some fields from the binary, but only LittleLong() them when we know this wasn't an already-cached model...
	//	
	version = (pinmodel->version);
	size	= (pinmodel->ofsEnd);

	if (!bAlreadyCached)
	{
		version = LittleLong(version);
		size	= LittleLong(size);
	}
	
	if (version != MDXA_VERSION) {
		ri.Printf( PRINT_WARNING, "R_LoadMDXA: %s has wrong version (%i should be %i)\n",
				 mod_name, version, MDXA_VERSION);
		return qfalse;
	}

	mod->type		= MOD_MDXA;
	mod->dataSize  += size;

	qboolean bAlreadyFound = qfalse;
	mdxa = mod->mdxa = (mdxaHeader_t*) //ri.Hunk_Alloc( size );
										RE_RegisterModels_Malloc(size, mod_name, &bAlreadyFound, TAG_MODEL_GLA);

	assert(bAlreadyCached == bAlreadyFound);	// I should probably eliminate 'bAlreadyFound', but wtf?

	if (!bAlreadyFound)
	{
		memcpy( mdxa, buffer, size );

		LL(mdxa->ident);
		LL(mdxa->version);
		LL(mdxa->numFrames);
		LL(mdxa->numBones);
		LL(mdxa->ofsFrames);
		LL(mdxa->ofsEnd);
	}

 	if ( mdxa->numFrames < 1 ) {
		ri.Printf( PRINT_WARNING, "R_LoadMDXA: %s has no frames\n", mod_name );
		return qfalse;
	}

	if (bAlreadyFound)
	{
		return qtrue;	// All done, stop here, do not LittleLong() etc. Do not pass go...
	}

#ifndef _M_IX86

	//
	// optimisation, we don't bother doing this for standard intel case since our data's already in that format...
	//

	// swap all the skeletal info
	boneInfo = (mdxaSkel_t *)( (byte *)mdxa + mdxa->ofsSkel);
	for ( i = 0 ; i < mdxa->numBones ; i++) 
	{
		LL(boneInfo->numChildren);
		LL(boneInfo->parent);
		for (k=0; k<boneInfo->numChildren; k++)
		{
			LL(boneInfo->children[k]);
		}

		// get next bone
		boneInfo += (int)( &((mdxaSkel_t *)0)->children[ boneInfo->numChildren ] );
	}


	// swap all the frames
	frameSize = (int)( &((mdxaFrame_t *)0)->bones[ mdxa->numBones ] );
	for ( i = 0 ; i < mdxa->numFrames ; i++) 
	{
		cframe = (mdxaFrame_t *) ( (byte *)mdxa + mdxa->ofsFrames + i * frameSize );
   		cframe->radius = LittleFloat( cframe->radius );
		for ( j = 0 ; j < 3 ; j++ ) 
		{
			cframe->bounds[0][j] = LittleFloat( cframe->bounds[0][j] );
			cframe->bounds[1][j] = LittleFloat( cframe->bounds[1][j] );
    		cframe->localOrigin[j] = LittleFloat( cframe->localOrigin[j] );
		}
		for ( j = 0 ; j < mdxa->numBones * sizeof( mdxaBone_t ) / 2 ; j++ ) 
		{
			((short *)cframe->bones)[j] = LittleShort( ((short *)cframe->bones)[j] );
		}
	}
#endif
	return qtrue;
}







