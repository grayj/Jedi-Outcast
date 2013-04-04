// leave this as first line for PCH reasons...
//



#ifndef __Q_SHARED_H
	#include "../game/q_shared.h"
#endif

#if !defined(TR_LOCAL_H)
	#include "../renderer/tr_local.h"
#endif
#if !defined(_QCOMMON_H_)
	#include "../qcommon/qcommon.h"
#endif

#include "../renderer/MatComp.h"

#if !defined(G2_H_INC)
	#include "G2.h"
#endif
	#include "G2_local.h"
	 
//=====================================================================================================================
// Bone List handling routines - so entities can override bone info on a bone by bone level, and also interrogate this info

// Given a bone name, see if that bone is already in our bone list - note the model_t pointer that gets passed in here MUST point at the 
// gla file, not the glm file type.
int G2_Find_Bone(const model_t *mod, boneInfo_v &blist, const char *boneName)
{
	int					i;
	mdxaSkel_t			*skel;
	mdxaSkelOffsets_t	*offsets;
   	offsets = (mdxaSkelOffsets_t *)((byte *)mod->mdxa + sizeof(mdxaHeader_t));
	skel = (mdxaSkel_t *)((byte *)mod->mdxa + sizeof(mdxaHeader_t) + offsets->offsets[0]);

	// look through entire list
	for(i=0; i<blist.size(); i++)
	{
		// if this bone entry has no info in it, bounce over it
		if (blist[i].boneNumber == -1)
		{
			continue;
		}

		// figure out what skeletal info structure this bone entry is looking at
		skel = (mdxaSkel_t *)((byte *)mod->mdxa + sizeof(mdxaHeader_t) + offsets->offsets[blist[i].boneNumber]);

		// if name is the same, we found it
		if (!stricmp(skel->name, boneName))
		{
			return i;
		}
	}

	// didn't find it
	return -1;
}

// we need to add a bone to the list - find a free one and see if we can find a corresponding bone in the gla file
int G2_Add_Bone (const model_t *mod, boneInfo_v &blist, const char *boneName)
{
	int i, x;
	mdxaSkel_t			*skel;
	mdxaSkelOffsets_t	*offsets;
	boneInfo_t			tempBone;
	 
   	offsets = (mdxaSkelOffsets_t *)((byte *)mod->mdxa + sizeof(mdxaHeader_t));

 	// walk the entire list of bones in the gla file for this model and see if any match the name of the bone we want to find
 	for (x=0; x< mod->mdxa->numBones; x++)
 	{
 		skel = (mdxaSkel_t *)((byte *)mod->mdxa + sizeof(mdxaHeader_t) + offsets->offsets[x]);
 		// if name is the same, we found it
 		if (!stricmp(skel->name, boneName))
		{
			break;
		}
	}

	// check to see we did actually make a match with a bone in the model
	if (x == mod->mdxa->numBones)
	{
		// didn't find it? Error
		assert(0);
		return -1;
	}

	// look through entire list - see if it's already there first
	for(i=0; i<blist.size(); i++)
	{
		// if this bone entry has info in it, bounce over it
		if (blist[i].boneNumber != -1)
		{
			skel = (mdxaSkel_t *)((byte *)mod->mdxa + sizeof(mdxaHeader_t) + offsets->offsets[blist[i].boneNumber]);
			// if name is the same, we found it
			if (!stricmp(skel->name, boneName))
			{
				return i;
			}
		}
		else
		{
			// if we found an entry that had a -1 for the bonenumber, then we hit a bone slot that was empty
			blist[i].boneNumber = x;
			blist[i].flags = 0;
	 		return i;
		}
	}
	
	// ok, we didn't find an existing bone of that name, or an empty slot. Lets add an entry
	tempBone.boneNumber = x;
	tempBone.flags = 0;
	blist.push_back(tempBone);
	return blist.size()-1;
}


// Given a model handle, and a bone name, we want to remove this bone from the bone override list
qboolean G2_Remove_Bone_Index ( boneInfo_v &blist, int index)
{
	// did we find it?
	if (index != -1)
	{
		// check the flags first - if it's still being used Do NOT remove it
		if (!blist[index].flags)
		{

			// set this bone to not used
			blist[index].boneNumber = -1;

		   	int newSize = blist.size();
			// now look through the list from the back and see if there is a block of -1's we can resize off the end of the list
			for (int i=blist.size()-1; i>-1; i--)
			{
				if (blist[i].boneNumber == -1)
				{
					newSize = i;
				}
				// once we hit one that isn't a -1, we are done.
				else
				{
					break;
				}
			}
			// do we need to resize?
			if (newSize != blist.size())
			{
				// yes, so lets do it
				blist.resize(newSize);
			}
		
			return qtrue;
		}
	}

//	assert(0);
	// no
	return qfalse;
}

// given a bone number, see if there is an override bone in the bone list
int	G2_Find_Bone_In_List(boneInfo_v &blist, const int boneNum)
{
	int i; 
	
	// look through entire list
	for(i=0; i<blist.size(); i++)
	{
		if (blist[i].boneNumber == boneNum)
		{
			return i;
		}
	}
	return -1;
}

// given a model, bonelist and bonename, lets stop an anim if it's playing.
qboolean G2_Stop_Bone_Index( boneInfo_v &blist, int index, int flags)
{
	// did we find it?
	if (index != -1)
	{
		blist[index].flags &= ~(flags);
		// try and remove this bone if we can
		return G2_Remove_Bone_Index(blist, index);
	}

	assert(0);

	return qfalse;
}

// generate a matrix for a given bone given some new angles for it.
void G2_Generate_Matrix(const model_t *mod, boneInfo_v &blist, int index, const float *angles, int flags,
						const Eorientations up, const Eorientations left, const Eorientations forward)
{
	mdxaSkel_t		*skel;
	mdxaSkelOffsets_t *offsets;
	mdxaBone_t		temp1;
	mdxaBone_t		permutation;
	mdxaBone_t		*boneOverride = &blist[index].matrix;
	vec3_t			newAngles;

	if (flags & (BONE_ANGLES_PREMULT | BONE_ANGLES_POSTMULT))
	{
		// build us a matrix out of the angles we are fed - but swap y and z because of wacky Quake setup
		vec3_t	newAngles;

		// determine what axis newAngles Yaw should revolve around
		switch (up)
		{
		case NEGATIVE_X:
			newAngles[1] = angles[2] + 180;
			break;
		case POSITIVE_X:
			newAngles[1] = angles[2];
			break;
		case NEGATIVE_Y:
			newAngles[1] = angles[0];
			break;
		case POSITIVE_Y:
			newAngles[1] = angles[0];
			break;
		case NEGATIVE_Z:
			newAngles[1] = angles[1] + 180;
			break;
		case POSITIVE_Z:
			newAngles[1] = angles[1];
			break;
		}

		// determine what axis newAngles pitch should revolve around
		switch (left)
		{
		case NEGATIVE_X:
			newAngles[0] = angles[2];
			break;
		case POSITIVE_X:
			newAngles[0] = angles[2] + 180;
			break;
		case NEGATIVE_Y:
			newAngles[0] = angles[0];
			break;
		case POSITIVE_Y:
			newAngles[0] = angles[0] + 180;
			break;
		case NEGATIVE_Z:
			newAngles[0] = angles[1];
			break;
		case POSITIVE_Z:
			newAngles[0] = angles[1];
			break;
		}

		// determine what axis newAngles Roll should revolve around
		switch (forward)
		{
		case NEGATIVE_X:
			newAngles[2] = angles[2];
			break;
		case POSITIVE_X:
			newAngles[2] = angles[2];
			break;
		case NEGATIVE_Y:
			newAngles[2] = angles[0];
			break;
		case POSITIVE_Y:
			newAngles[2] = angles[0] + 180;
			break;
		case NEGATIVE_Z:
			newAngles[2] = angles[1];
			break;
		case POSITIVE_Z:
			newAngles[2] = angles[1] + 180;
			break;
		}

		Create_Matrix(newAngles, boneOverride);

		// figure out where the bone hirearchy info is
		offsets = (mdxaSkelOffsets_t *)((byte *)mod->mdxa + sizeof(mdxaHeader_t));
		skel = (mdxaSkel_t *)((byte *)mod->mdxa + sizeof(mdxaHeader_t) + offsets->offsets[blist[index].boneNumber]);

		Multiply_3x4Matrix(&temp1,  boneOverride,&skel->BasePoseMatInv);
		Multiply_3x4Matrix(boneOverride,&skel->BasePoseMat, &temp1);
	
	}
	else
	{
		VectorCopy(angles, newAngles);

		// why I should need do this Fuck alone knows. But I do.
		if (left == POSITIVE_Y)
		{
			newAngles[0] +=180;
		}

		Create_Matrix(newAngles, &temp1);

		permutation.matrix[0][0] = permutation.matrix[0][1] = permutation.matrix[0][2] = permutation.matrix[0][3] = 0;
		permutation.matrix[1][0] = permutation.matrix[1][1] = permutation.matrix[1][2] = permutation.matrix[1][3] = 0;
		permutation.matrix[2][0] = permutation.matrix[2][1] = permutation.matrix[2][2] = permutation.matrix[2][3] = 0;

		// determine what axis newAngles Yaw should revolve around
		switch (forward)
		{
		case NEGATIVE_X:
			permutation.matrix[0][0] = -1;		// works
			break;
		case POSITIVE_X:
			permutation.matrix[0][0] = 1;		// works
			break;
		case NEGATIVE_Y:
			permutation.matrix[1][0] = -1;
			break;
		case POSITIVE_Y:
			permutation.matrix[1][0] = 1;
			break;
		case NEGATIVE_Z:
			permutation.matrix[2][0] = -1;
			break;
		case POSITIVE_Z:
			permutation.matrix[2][0] = 1;
			break;
		}

		// determine what axis newAngles pitch should revolve around
		switch (left)
		{
		case NEGATIVE_X:
			permutation.matrix[0][1] = -1;
			break;
		case POSITIVE_X:
			permutation.matrix[0][1] = 1;
			break;
		case NEGATIVE_Y:
			permutation.matrix[1][1] = -1;		// works
			break;
		case POSITIVE_Y:
			permutation.matrix[1][1] = 1;		// works
			break;
		case NEGATIVE_Z:
			permutation.matrix[2][1] = -1;
			break;
		case POSITIVE_Z:
			permutation.matrix[2][1] = 1;
			break;
		}

		// determine what axis newAngles Roll should revolve around
		switch (up)
		{
		case NEGATIVE_X:
			permutation.matrix[0][2] = -1;
			break;
		case POSITIVE_X:
			permutation.matrix[0][2] = 1;
			break;
		case NEGATIVE_Y:
			permutation.matrix[1][2] = -1;
			break;
		case POSITIVE_Y:
			permutation.matrix[1][2] = 1;
			break;
		case NEGATIVE_Z:
			permutation.matrix[2][2] = -1;		// works
			break;
		case POSITIVE_Z:
			permutation.matrix[2][2] = 1;		// works
			break;
		}

		Multiply_3x4Matrix(boneOverride, &temp1,&permutation);

	}

	// keep a copy of the matrix in the newmatrix which is actually what we use
	memcpy(&blist[index].newMatrix, &blist[index].matrix, sizeof(mdxaBone_t));

}

//=========================================================================================
//// Public Bone Routines


// Given a model handle, and a bone name, we want to remove this bone from the bone override list
qboolean G2_Remove_Bone (const char *fileName, boneInfo_v &blist, const char *boneName)
{
	model_t		*mod_m = R_GetModelByHandle(RE_RegisterModel(fileName)); 
	model_t		*mod_a = R_GetModelByHandle(mod_m->mdxm->animIndex); 
	int			index = G2_Find_Bone(mod_a, blist, boneName);
 
	return G2_Remove_Bone_Index(blist, index);
}

#define DEBUG_PCJ (0)


// Given a model handle, and a bone name, we want to set angles specifically for overriding
qboolean G2_Set_Bone_Angles_Index( boneInfo_v &blist, const int index,
							const float *angles, const int flags, const Eorientations yaw,
							const Eorientations pitch, const Eorientations roll, qhandle_t *modelList,
							const int modelIndex, const int blendTime, const int currentTime)
{
	if ((index >= blist.size()) || (blist[index].boneNumber == -1))
	{
		// we are attempting to set a bone override that doesn't exist
		assert(0);
		return qfalse;
	}
	
	if (flags & (BONE_ANGLES_PREMULT | BONE_ANGLES_POSTMULT))
	{
		// you CANNOT call this with an index with these kinds of bone overrides - we need the model details for these kinds of bone angle overrides
		assert(0);
		return qfalse;
	}

	// yes, so set the angles and flags correctly
	blist[index].flags &= ~(BONE_ANGLES_TOTAL);
	blist[index].flags |= flags;
	blist[index].boneBlendStart = currentTime;
	blist[index].boneBlendTime = blendTime;
#if DEBUG_PCJ
	OutputDebugString(va("PCJ %2d %6d   (%6.2f,%6.2f,%6.2f) %d %d %d %d\n",index,currentTime,angles[0],angles[1],angles[2],yaw,pitch,roll,flags));
#endif

	G2_Generate_Matrix(NULL, blist, index, angles, flags, yaw, pitch, roll);
	return qtrue;

}

// Given a model handle, and a bone name, we want to set angles specifically for overriding
qboolean G2_Set_Bone_Angles(const char *fileName, boneInfo_v &blist, const char *boneName, const float *angles,
							const int flags, const Eorientations up, const Eorientations left, const Eorientations forward,
							qhandle_t *modelList, const int modelIndex, const int blendTime, const int currentTime)
{
	model_t		*mod_m;

	if (!fileName[0])
	{
		mod_m = R_GetModelByHandle(modelList[modelIndex]);
	}
	else
	{
		mod_m = R_GetModelByHandle(RE_RegisterModel(fileName));
	}

	model_t		*mod_a = R_GetModelByHandle(mod_m->mdxm->animIndex); 
	int			index = G2_Find_Bone(mod_a, blist, boneName);
 
	// did we find it?
	if (index != -1)
	{
		// yes, so set the angles and flags correctly
		blist[index].flags &= ~(BONE_ANGLES_TOTAL);
		blist[index].flags |= flags;
		blist[index].boneBlendStart = currentTime;
		blist[index].boneBlendTime = blendTime;
#if DEBUG_PCJ
		OutputDebugString(va("%2d %6d   (%6.2f,%6.2f,%6.2f) %d %d %d %d\n",index,currentTime,angles[0],angles[1],angles[2],up,left,forward,flags));
#endif

		G2_Generate_Matrix(mod_a, blist, index, angles, flags, up, left, forward);
		return qtrue;
	}

	// no - lets try and add this bone in
	index = G2_Add_Bone(mod_a, blist, boneName);

	// did we find a free one?
	if (index != -1)
	{
		// yes, so set the angles and flags correctly
		blist[index].flags &= ~(BONE_ANGLES_TOTAL);
		blist[index].flags |= flags;
		blist[index].boneBlendStart = currentTime;
		blist[index].boneBlendTime = blendTime;
#if DEBUG_PCJ
		OutputDebugString(va("%2d %6d   (%6.2f,%6.2f,%6.2f) %d %d %d %d\n",index,currentTime,angles[0],angles[1],angles[2],up,left,forward,flags));
#endif

		G2_Generate_Matrix(mod_a, blist, index, angles, flags, up, left, forward);
		return qtrue;
	}
	assert(0);

	// no
	return qfalse;
}

// Given a model handle, and a bone name, we want to set angles specifically for overriding - using a matrix directly
qboolean G2_Set_Bone_Angles_Matrix_Index(boneInfo_v &blist, const int index,
								   const mdxaBone_t &matrix, const int flags, qhandle_t *modelList,
								   const int modelIndex, const int blendTime, const int currentTime)
{

	if ((index >= blist.size()) || (blist[index].boneNumber == -1))
	{
		// we are attempting to set a bone override that doesn't exist
		assert(0);
		return qfalse;
	}
	// yes, so set the angles and flags correctly
	blist[index].flags &= ~(BONE_ANGLES_TOTAL);
	blist[index].flags |= flags;
	blist[index].boneBlendStart = currentTime;
	blist[index].boneBlendTime = blendTime;

	memcpy(&blist[index].matrix, &matrix, sizeof(mdxaBone_t));
	memcpy(&blist[index].newMatrix, &matrix, sizeof(mdxaBone_t));
	return qtrue;

}

// Given a model handle, and a bone name, we want to set angles specifically for overriding - using a matrix directly
qboolean G2_Set_Bone_Angles_Matrix(const char *fileName, boneInfo_v &blist, const char *boneName, const mdxaBone_t &matrix,
								   const int flags, qhandle_t *modelList, const int modelIndex, const int blendTime, const int currentTime)
{
		model_t		*mod_m;
	if (!fileName[0])
	{
		mod_m = R_GetModelByHandle(modelList[modelIndex]);
	}
	else
	{
		mod_m = R_GetModelByHandle(RE_RegisterModel(fileName));
	}
	model_t		*mod_a = R_GetModelByHandle(mod_m->mdxm->animIndex); 
	int			index = G2_Find_Bone(mod_a, blist, boneName);
 
	// did we find it?
	if (index != -1)
	{
		// yes, so set the angles and flags correctly
		blist[index].flags &= ~(BONE_ANGLES_TOTAL);
		blist[index].flags |= flags;

		memcpy(&blist[index].matrix, &matrix, sizeof(mdxaBone_t));
		memcpy(&blist[index].newMatrix, &matrix, sizeof(mdxaBone_t));
		return qtrue;
	}

	// no - lets try and add this bone in
	index = G2_Add_Bone(mod_a, blist, boneName);

	// did we find a free one?
	if (index != -1)
	{
		// yes, so set the angles and flags correctly
		blist[index].flags &= ~(BONE_ANGLES_TOTAL);
		blist[index].flags |= flags;

		memcpy(&blist[index].matrix, &matrix, sizeof(mdxaBone_t));
		memcpy(&blist[index].newMatrix, &matrix, sizeof(mdxaBone_t));
		return qtrue;
	}
	assert(0);

	// no
	return qfalse;
}

#define DEBUG_G2_TIMING (0)

// given a model, bone name, a bonelist, a start/end frame number, a anim speed and some anim flags, set up or modify an existing bone entry for a new set of anims
qboolean G2_Set_Bone_Anim_Index(
	boneInfo_v &blist, 
	const int index, 
	const int startFrame, 
	const int endFrame, 
	const int flags, 
	const float animSpeed, 
	const int currentTime, 
	const float setFrame, 
	const int blendTime)
{
	int			modFlags = flags;
 
	if ((index >= blist.size()) || (blist[index].boneNumber == -1))
	{
		// we are attempting to set a bone override that doesn't exist
		assert(0);
		return qfalse;
	}

	if (setFrame != -1)
	{
		assert((setFrame >= startFrame) && (setFrame <= endFrame));
	}
	if (flags & BONE_ANIM_BLEND)
	{
		float	currentFrame, animSpeed; 
		int 	startFrame, endFrame, flags;
		// figure out where we are now
		if (G2_Get_Bone_Anim_Index(blist, index, currentTime, &currentFrame, &startFrame, &endFrame, &flags, &animSpeed, NULL, 0))
		{
			if (blist[index].blendStart == currentTime)	//we're replacing a blend in progress which hasn't started
			{
				// set the amount of time it's going to take to blend this anim with the last frame of the last one
				blist[index].blendTime = blendTime;
			}
			else
			{
				if (animSpeed<0.0f)
				{
					blist[index].blendFrame = floor(currentFrame);
					blist[index].blendLerpFrame = floor(currentFrame);
				}
				else
				{
					blist[index].blendFrame = currentFrame;
					blist[index].blendLerpFrame = currentFrame+1;
					
					// cope with if the lerp frame is actually off the end of the anim
					if (blist[index].blendFrame >= endFrame )
					{
						// we only want to lerp with the first frame of the anim if we are looping 
						if (blist[index].flags & BONE_ANIM_OVERRIDE_LOOP)
						{
							blist[index].blendFrame = startFrame;
						}
						// if we intend to end this anim or freeze after this, then just keep on the last frame
						else
						{
						//	assert(endFrame>0);
							if (endFrame <= 0)
							{
								blist[index].blendLerpFrame = 0;
							}
							else
							{
								blist[index].blendFrame = endFrame -1;
							}
						}
					}
					
					// cope with if the lerp frame is actually off the end of the anim
					if (blist[index].blendLerpFrame >= endFrame )
					{
						// we only want to lerp with the first frame of the anim if we are looping 
						if (blist[index].flags & BONE_ANIM_OVERRIDE_LOOP)
						{
							blist[index].blendLerpFrame = startFrame;
						}
						// if we intend to end this anim or freeze after this, then just keep on the last frame
						else
						{
						//	assert(endFrame>0);
							if (endFrame <= 0)
							{
								blist[index].blendLerpFrame = 0;
							}
							else
							{
								blist[index].blendLerpFrame = endFrame - 1;
							}
						}
					}
				}
				// set the amount of time it's going to take to blend this anim with the last frame of the last one
				blist[index].blendTime = blendTime;
				blist[index].blendStart = currentTime;

			}
		}
		// hmm, we weren't animating on this bone. In which case disable the blend
		else
		{
			blist[index].blendFrame = blist[index].blendLerpFrame = 0;
			blist[index].blendTime = 0;
			modFlags &= ~(BONE_ANIM_BLEND);
		}
	}
	else
	{
		blist[index].blendFrame = blist[index].blendLerpFrame = 0;
		blist[index].blendTime = blist[index].blendStart = 0;
		// we aren't blending, so remove the option to do so
		modFlags &= ~BONE_ANIM_BLEND;
	}
	// yes, so set the anim data and flags correctly
	blist[index].endFrame = endFrame;
	blist[index].startFrame = startFrame;
	blist[index].animSpeed = animSpeed;
	blist[index].pauseTime = 0;
	// start up the animation:)
	if (setFrame != -1)
	{
		blist[index].lastTime = blist[index].startTime = (currentTime - (((setFrame - (float)startFrame) * 50.0)/ animSpeed));
	}
	else
	{
		blist[index].lastTime = blist[index].startTime = currentTime;
	}
	blist[index].flags &= ~(BONE_ANIM_TOTAL);
	blist[index].flags |= modFlags;

#if DEBUG_G2_TIMING
	if (index==2)
	{
		const boneInfo_t &bone=blist[index];
		char mess[1000];
		if (bone.flags&BONE_ANIM_BLEND)
		{
			sprintf(mess,"sab[%2d] %5d  %5d  (%5d-%5d) %4.2f %4x   bt(%5d-%5d) %7.2f %5d\n",
				index,
				currentTime,
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
			sprintf(mess,"saa[%2d] %5d  %5d  (%5d-%5d) %4.2f %4x\n",
				index,
				currentTime,
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

	return qtrue;

}

// given a model, bone name, a bonelist, a start/end frame number, a anim speed and some anim flags, set up or modify an existing bone entry for a new set of anims
qboolean G2_Set_Bone_Anim(const char *fileName, 
						  boneInfo_v &blist, 
						  const char *boneName, 
						  const int startFrame, 
						  const int endFrame, 
						  const int flags, 
						  const float animSpeed, 
						  const int currentTime, 
						  const float setFrame, 
						  const int blendTime)
{
  	model_t		*mod_m = R_GetModelByHandle(RE_RegisterModel(fileName)); 
	model_t		*mod_a = R_GetModelByHandle(mod_m->mdxm->animIndex); 
	int			index = G2_Find_Bone(mod_a, blist, boneName);
	if (index == -1)
	{
		index = G2_Add_Bone(mod_a, blist, boneName);
	}
	if (index != -1)
	{
		return G2_Set_Bone_Anim_Index(blist,index,startFrame,endFrame,flags,animSpeed,currentTime,setFrame,blendTime);
	}
	return qfalse;
}

qboolean G2_Get_Bone_Anim_Range(const char *fileName, boneInfo_v &blist, const char *boneName, int *startFrame, int *endFrame)
{
	model_t		*mod_m = R_GetModelByHandle(RE_RegisterModel(fileName)); 
	model_t		*mod_a = R_GetModelByHandle(mod_m->mdxm->animIndex); 
	int			index = G2_Find_Bone(mod_a, blist, boneName);
 
	// did we find it?
	if (index != -1)
	{ 
		// are we an animating bone?
		if (blist[index].flags & (BONE_ANIM_OVERRIDE_LOOP | BONE_ANIM_OVERRIDE))
		{
			*startFrame = blist[index].startFrame;
			*endFrame = blist[index].endFrame;
			return qtrue;
		}
	}
	return qfalse;
}

// given a model, bonelist and bonename, return the current frame, startframe and endframe of the current animation
// NOTE if we aren't running an animation, then qfalse is returned
qboolean G2_Get_Bone_Anim_Index( boneInfo_v &blist, const int index, const int currentTime, 
						  float *currentFrame, int *startFrame, int *endFrame, int *flags, float *retAnimSpeed, qhandle_t *modelList, int modelIndex)
{
  
	// did we find it?
	if ((index != -1) && !((index >= blist.size()) || (blist[index].boneNumber == -1)))
	{ 
		// are we an animating bone?
		if (blist[index].flags & (BONE_ANIM_OVERRIDE_LOOP | BONE_ANIM_OVERRIDE))
		{
			// yes - add in animation speed to current frame
			float	animSpeed = blist[index].animSpeed;
			float	time = (currentTime - blist[index].startTime) / 50.0f;
			float	mendFrame = (float)blist[index].endFrame ;

			// are we a paused anim?
			if (blist[index].pauseTime)
			{
				time = (blist[index].pauseTime - blist[index].startTime) / 50.0f;
			}
			if (time<0)
			{
				time=0;
			}

			float	newFrame_g = blist[index].startFrame + (time * animSpeed);
			*currentFrame = newFrame_g;

			int		animSize = blist[index].endFrame - blist[index].startFrame;
			// we are supposed to be animating right?
			if (animSize)
			{ 
				// did we run off the end?
				if (((animSpeed > 0.0f) && (newFrame_g > mendFrame-1)) || 
					((animSpeed < 0.0f) && (newFrame_g < mendFrame+1)))
				{
					// yep - decide what to do
					if (blist[index].flags & BONE_ANIM_OVERRIDE_LOOP)
					{

						if (animSpeed < 0.0f)
						{
							if (newFrame_g <= mendFrame+1)
							{
								newFrame_g=mendFrame+fmod(newFrame_g-mendFrame,animSize)-animSize;
							}
						}
						else
						{
							if (newFrame_g >= mendFrame)
							{
								newFrame_g=mendFrame+fmod(newFrame_g-mendFrame,animSize)-animSize;
							}
						}

						*currentFrame = newFrame_g;
					}
					else
					{
						// nope, we ran off the end of the current anim
						if (blist[index].flags & BONE_ANIM_OVERRIDE_FREEZE)
						{
							if (animSpeed > 0.0f)
							{
								*currentFrame = blist[index].endFrame -1;
							}
							else
							{
								*currentFrame = blist[index].endFrame +1;
							}
						}
						else
						{
							*currentFrame = 0;
						}
					}
				}
			}
			else
			{
				*currentFrame = blist[index].startFrame;
			}

			*startFrame = blist[index].startFrame;
			*endFrame = blist[index].endFrame;
			*flags = blist[index].flags;
			*retAnimSpeed = animSpeed;
			return qtrue;
		}
	}
	*startFrame =0;
	*endFrame =1;
	*currentFrame =0;
	*flags = 0;
	*retAnimSpeed = 1.0f;
	return qfalse;
}

// given a model, bonelist and bonename, return the current frame, startframe and endframe of the current animation
// NOTE if we aren't running an animation, then qfalse is returned
qboolean G2_Get_Bone_Anim(const char *fileName, boneInfo_v &blist, const char *boneName, const int currentTime, 
						  float *currentFrame, int *startFrame, int *endFrame, int *flags, float *retAnimSpeed, qhandle_t *modelList, int modelIndex)
{
  	model_t		*mod_m;
	if (!fileName[0])
	{
		mod_m = R_GetModelByHandle(modelList[modelIndex]);
	}
	else
	{
		mod_m = R_GetModelByHandle(RE_RegisterModel(fileName));
	}


	model_t		*mod_a = R_GetModelByHandle(mod_m->mdxm->animIndex); 
	int			index = G2_Find_Bone(mod_a, blist, boneName);
 
	// did we find it?
	if (index != -1)
	{ 
		// are we an animating bone?
		if (blist[index].flags & (BONE_ANIM_OVERRIDE_LOOP | BONE_ANIM_OVERRIDE))
		{
			// yes - add in animation speed to current frame
			float	animSpeed = blist[index].animSpeed;
			float	time = (currentTime - blist[index].startTime) / 50.0f;
			float	mendFrame = (float)blist[index].endFrame ;

			// are we a paused anim?
			if (blist[index].pauseTime)
			{
				time = (blist[index].pauseTime - blist[index].startTime) / 50.0f;
			}
			if (time<0)
			{
				time=0;
			}

			float	newFrame_g = blist[index].startFrame + (time * animSpeed);
			*currentFrame = newFrame_g;

			int		animSize = blist[index].endFrame - blist[index].startFrame;
			// we are supposed to be animating right?
			if (animSize)
			{ 
				// did we run off the end?
				if (((animSpeed > 0.0f) && (newFrame_g > mendFrame-1)) || 
					((animSpeed < 0.0f) && (newFrame_g < mendFrame+1)))
				{
					// yep - decide what to do
					if (blist[index].flags & BONE_ANIM_OVERRIDE_LOOP)
					{

						// get our new animation frame back within the bounds of the animation set
						if (animSpeed < 0.0f)
						{
							if (newFrame_g <= mendFrame+1)
							{
								newFrame_g=mendFrame+fmod(newFrame_g-mendFrame,animSize)-animSize;
							}
						}
						else
						{
							if (newFrame_g >= mendFrame)
							{
								newFrame_g=mendFrame+fmod(newFrame_g-mendFrame,animSize)-animSize;
							}
						}

						*currentFrame = newFrame_g;
					}
					else
					{
						// nope, we ran off the end of the current anim
						if (blist[index].flags & BONE_ANIM_OVERRIDE_FREEZE)
						{
							if (animSpeed > 0.0f)
							{
								*currentFrame = blist[index].endFrame -1;
							}
							else
							{
								*currentFrame = blist[index].endFrame +1;
							}
						}
						else
						{
							*currentFrame = 0;
						}
					}
				}
			}
			else
			{
				*currentFrame = blist[index].startFrame;
			}

			*startFrame = blist[index].startFrame;
			*endFrame = blist[index].endFrame;
			*flags = blist[index].flags;
			*retAnimSpeed = animSpeed;
			return qtrue;
		}
	}
	*startFrame =0;
	*endFrame =1 ;
	*currentFrame = 0;
	*flags = 0; //hmmmm this used to be -1
	*retAnimSpeed = 1.0f;
	return qfalse;
}

// given a model, bonelist and bonename, lets pause an anim if it's playing.
qboolean G2_Pause_Bone_Anim(const char *fileName, boneInfo_v &blist, const char *boneName, const int currentTime)
{
  	model_t		*mod_m = R_GetModelByHandle(RE_RegisterModel(fileName)); 
	model_t		*mod_a = R_GetModelByHandle(mod_m->mdxm->animIndex); 
	int			index = G2_Find_Bone(mod_a, blist, boneName);
 
	// did we find it?
	if (index != -1)
	{
		// are we pausing or un pausing?
		if (blist[index].pauseTime)
		{
			int		startFrame, endFrame, flags;
			float	currentFrame, animSpeed;

			// figure out what frame we are on now
			G2_Get_Bone_Anim(fileName, blist, boneName, blist[index].pauseTime, &currentFrame, &startFrame, &endFrame, &flags, &animSpeed, NULL, 0);
			// reset start time so we are actually on this frame right now
			G2_Set_Bone_Anim(fileName, blist, boneName, startFrame, endFrame, flags, animSpeed, currentTime, currentFrame, 0);
			// no pausing anymore
			blist[index].pauseTime = 0;
		}
		// ahh, just pausing, the easy bit
		else
		{
			blist[index].pauseTime = currentTime;
		}
		
		return qtrue;
	}
	assert(0);

	return qfalse;
}

qboolean	G2_IsPaused(const char *fileName, boneInfo_v &blist, const char *boneName)
{
  	model_t		*mod_m = R_GetModelByHandle(RE_RegisterModel(fileName)); 
	model_t		*mod_a = R_GetModelByHandle(mod_m->mdxm->animIndex); 
	int			index = G2_Find_Bone(mod_a, blist, boneName);
 
	// did we find it?
	if (index != -1)
	{
		// are we paused?
		if (blist[index].pauseTime)
		{
			// yup. paused.
			return qtrue;
		}
		return qfalse;
	}

	return qfalse;
}

// given a model, bonelist and bonename, lets stop an anim if it's playing.
qboolean G2_Stop_Bone_Anim_Index(boneInfo_v &blist, const int index)
{
 
	if ((index >= blist.size()) || (blist[index].boneNumber == -1))
	{
		// we are attempting to set a bone override that doesn't exist
		assert(0);
		return qfalse;
	}

	blist[index].flags &= ~(BONE_ANIM_TOTAL);
	// try and remove this bone if we can
	return G2_Remove_Bone_Index(blist, index);
}

// given a model, bonelist and bonename, lets stop an anim if it's playing.
qboolean G2_Stop_Bone_Anim(const char *fileName, boneInfo_v &blist, const char *boneName)
{
  	model_t		*mod_m = R_GetModelByHandle(RE_RegisterModel(fileName)); 
	model_t		*mod_a = R_GetModelByHandle(mod_m->mdxm->animIndex); 
	int			index = G2_Find_Bone(mod_a, blist, boneName);
 
	// did we find it?
	if (index != -1)
	{
		blist[index].flags &= ~(BONE_ANIM_TOTAL);
		// try and remove this bone if we can
		return G2_Remove_Bone_Index(blist, index);
	}
	assert(0);

	return qfalse;
}

// given a model, bonelist and bonename, lets stop an anim if it's playing.
qboolean G2_Stop_Bone_Angles_Index(boneInfo_v &blist, const int index)
{
 
	if ((index >= blist.size()) || (blist[index].boneNumber == -1))
	{
		// we are attempting to set a bone override that doesn't exist
		assert(0);
		return qfalse;
	} 

	blist[index].flags &= ~(BONE_ANGLES_TOTAL);
	// try and remove this bone if we can
	return G2_Remove_Bone_Index(blist, index);

}

// given a model, bonelist and bonename, lets stop an anim if it's playing.
qboolean G2_Stop_Bone_Angles(const char *fileName, boneInfo_v &blist, const char *boneName)
{
  	model_t		*mod_m = R_GetModelByHandle(RE_RegisterModel(fileName)); 
	model_t		*mod_a = R_GetModelByHandle(mod_m->mdxm->animIndex); 
	int			index = G2_Find_Bone(mod_a, blist, boneName);
 
	// did we find it?
	if (index != -1)
	{
		blist[index].flags &= ~(BONE_ANGLES_TOTAL);
		// try and remove this bone if we can
		return G2_Remove_Bone_Index(blist, index);
	}
	assert(0);

	return qfalse;
}


// actually walk the bone list and update each and every bone if we have ended an animation for them.
void G2_Animate_Bone_List(CGhoul2Info_v &ghoul2, const int currentTime, const int index )
{
	int i;
	boneInfo_v &blist = ghoul2[index].mBlist;

	// look through entire list
	for(i=0; i<blist.size(); i++)
	{
		// we we a valid bone override?
		if (blist[i].boneNumber != -1)
		{
			// are we animating?
			if (blist[i].flags & (BONE_ANIM_OVERRIDE_LOOP | BONE_ANIM_OVERRIDE))
			{
				// yes - add in animation speed to current frame
				float	animSpeed = blist[i].animSpeed;
				float	endFrame = (float)blist[i].endFrame ;
				float	time = (currentTime - blist[i].startTime) / 50.0f;
				// are we a paused anim?
				if (blist[i].pauseTime)
				{
					time = (blist[i].pauseTime - blist[i].startTime) / 50.0f;
				}
				if (time<0.0f)
				{
					time=0.0f;
				}
				float	newFrame_g = blist[i].startFrame + (time * animSpeed);

				int		animSize = endFrame - blist[i].startFrame;
				// we are supposed to be animating right?
				if (animSize)
				{
					// did we run off the end?
					if (((animSpeed > 0.0f) && (newFrame_g > endFrame-1 )) || 
						((animSpeed < 0.0f) && (newFrame_g < endFrame+1 )))
					{
						// yep - decide what to do
						if (blist[i].flags & BONE_ANIM_OVERRIDE_LOOP)
						{
							// get our new animation frame back within the bounds of the animation set
							if (animSpeed < 0.0f)
							{
								if (newFrame_g <= endFrame+1)
								{
									newFrame_g=endFrame+fmod(newFrame_g-endFrame,animSize)-animSize;
								}
							}
							else
							{
								if (newFrame_g >= endFrame)
								{
									newFrame_g=endFrame+fmod(newFrame_g-endFrame,animSize)-animSize;
								}
							}
							// figure out new start time
							float frameTime =  newFrame_g - blist[i].startFrame ;
							blist[i].startTime = currentTime - (int)((frameTime / animSpeed) * 50.0f);
							if (blist[i].startTime>currentTime)
							{
								blist[i].startTime=currentTime;
							}
							assert(blist[i].startTime <= currentTime);
							blist[i].lastTime = blist[i].startTime;
						}
						else
						{
							if ((blist[i].flags & BONE_ANIM_OVERRIDE_FREEZE) != BONE_ANIM_OVERRIDE_FREEZE)
							{
								// nope, just stop it. And remove the bone if possible
								G2_Stop_Bone_Index(blist, i, (BONE_ANIM_TOTAL));
							}
						}
					}
				}
			}
		}
	}
}

// set the bone list to all unused so the bone transformation routine ignores it.
void G2_Init_Bone_List(boneInfo_v &blist)
{
	blist.clear();
}

void G2_RemoveRedundantBoneOverrides(boneInfo_v &blist, int *activeBones)
{
	int		i;

	// walk the surface list, removing surface overrides or generated surfaces that are pointing at surfaces that aren't active anymore
	for (i=0; i<blist.size(); i++)
	{
		if (blist[i].boneNumber != -1)
		{
			if (!activeBones[blist[i].boneNumber])
			{
				blist[i].flags = 0;
				G2_Remove_Bone_Index(blist, i);
			}
		}
	}
}

int	G2_Get_Bone_Index(CGhoul2Info *ghoul2, const char *boneName)
{
  	model_t		*mod_m = R_GetModelByHandle(RE_RegisterModel(ghoul2->mFileName)); 
	model_t		*mod_a = R_GetModelByHandle(mod_m->mdxm->animIndex); 

	return (G2_Find_Bone(mod_a, ghoul2->mBlist, boneName));
}