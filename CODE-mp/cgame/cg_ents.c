// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_ents.c -- present snapshot entities, happens every single frame

#include "cg_local.h"
/*
Ghoul2 Insert Start
*/
#include "..\game\q_shared.h"
#include "..\ghoul2\g2.h"
/*
Ghoul2 Insert end
*/

static void CG_Missile( centity_t *cent );

/*
======================
CG_PositionEntityOnTag

Modifies the entities position and axis by the given
tag location
======================
*/
void CG_PositionEntityOnTag( refEntity_t *entity, const refEntity_t *parent, 
							qhandle_t parentModel, char *tagName ) {
	int				i;
	orientation_t	lerped;
	
	// lerp the tag
	trap_R_LerpTag( &lerped, parentModel, parent->oldframe, parent->frame,
		1.0 - parent->backlerp, tagName );

	// FIXME: allow origin offsets along tag?
	VectorCopy( parent->origin, entity->origin );
	for ( i = 0 ; i < 3 ; i++ ) {
		VectorMA( entity->origin, lerped.origin[i], parent->axis[i], entity->origin );
	}

	// had to cast away the const to avoid compiler problems...
	MatrixMultiply( lerped.axis, ((refEntity_t *)parent)->axis, entity->axis );
	entity->backlerp = parent->backlerp;
}


/*
======================
CG_PositionRotatedEntityOnTag

Modifies the entities position and axis by the given
tag location
======================
*/
void CG_PositionRotatedEntityOnTag( refEntity_t *entity, const refEntity_t *parent, 
							qhandle_t parentModel, char *tagName ) {
	int				i;
	orientation_t	lerped;
	vec3_t			tempAxis[3];

//AxisClear( entity->axis );
	// lerp the tag
	trap_R_LerpTag( &lerped, parentModel, parent->oldframe, parent->frame,
		1.0 - parent->backlerp, tagName );

	// FIXME: allow origin offsets along tag?
	VectorCopy( parent->origin, entity->origin );
	for ( i = 0 ; i < 3 ; i++ ) {
		VectorMA( entity->origin, lerped.origin[i], parent->axis[i], entity->origin );
	}

	// had to cast away the const to avoid compiler problems...
	MatrixMultiply( entity->axis, lerped.axis, tempAxis );
	MatrixMultiply( tempAxis, ((refEntity_t *)parent)->axis, entity->axis );
}



/*
==========================================================================

FUNCTIONS CALLED EACH FRAME

==========================================================================
*/

/*
======================
CG_SetEntitySoundPosition

Also called by event processing code
======================
*/
void CG_SetEntitySoundPosition( centity_t *cent ) {
	if ( cent->currentState.solid == SOLID_BMODEL )
	{
		vec3_t	origin;
		float	*v;

		v = cgs.inlineModelMidpoints[ cent->currentState.modelindex ];
		VectorAdd( cent->lerpOrigin, v, origin );
		trap_S_UpdateEntityPosition( cent->currentState.number, origin );
	}
	else
	{
		trap_S_UpdateEntityPosition( cent->currentState.number, cent->lerpOrigin );
	}
}

/*
==================
CG_EntityEffects

Add continuous entity effects, like local entity emission and lighting
==================
*/
static void CG_EntityEffects( centity_t *cent ) {

	// update sound origins
	CG_SetEntitySoundPosition( cent );

	// add loop sound
	if ( cent->currentState.loopSound ) {
		//rww - doors and things with looping sounds have a crazy origin (being brush models and all)
		if ( cent->currentState.solid == SOLID_BMODEL )
		{
			vec3_t	origin;
			float	*v;

			v = cgs.inlineModelMidpoints[ cent->currentState.modelindex ];
			VectorAdd( cent->lerpOrigin, v, origin );
			trap_S_AddLoopingSound( cent->currentState.number, origin, vec3_origin, 
				cgs.gameSounds[ cent->currentState.loopSound ] );
		}
		else if (cent->currentState.eType != ET_SPEAKER) {
			trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, 
				cgs.gameSounds[ cent->currentState.loopSound ] );
		} else {
			trap_S_AddRealLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, 
				cgs.gameSounds[ cent->currentState.loopSound ] );
		}
	}


	// constant light glow
	if ( cent->currentState.constantLight ) {
		int		cl;
		int		i, r, g, b;

		cl = cent->currentState.constantLight;
		r = cl & 255;
		g = ( cl >> 8 ) & 255;
		b = ( cl >> 16 ) & 255;
		i = ( ( cl >> 24 ) & 255 ) * 4;
		trap_R_AddLightToScene( cent->lerpOrigin, i, r, g, b );
	}

}

localEntity_t *FX_AddOrientedLine(vec3_t start, vec3_t end, vec3_t normal, float stScale, float scale,
								  float dscale, float startalpha, float endalpha, float killTime, qhandle_t shader)
{
	localEntity_t	*le;
	
#ifdef _DEBUG
	if (!shader)
	{
		Com_Printf("FX_AddLine: NULL shader\n");
	}
#endif

	le = CG_AllocLocalEntity();
	le->leType = LE_OLINE;

	le->startTime = cg.time;
	le->endTime = le->startTime + killTime;
	le->data.line.width = scale;
	le->data.line.dwidth = dscale;

	le->alpha = startalpha;
	le->dalpha = endalpha - startalpha;

	le->refEntity.data.line.stscale = stScale;
	le->refEntity.data.line.width = scale;

	le->refEntity.customShader = shader;

	// set origin
	VectorCopy ( start, le->refEntity.origin);
	VectorCopy ( end, le->refEntity.oldorigin );

	AxisClear(le->refEntity.axis);
	VectorCopy( normal, le->refEntity.axis[0] );
	RotateAroundDirection( le->refEntity.axis, 0); // le->refEntity.data.sprite.rotation );	This is roll in quad land

	le->refEntity.shaderRGBA[0] = 0xff;
	le->refEntity.shaderRGBA[1] = 0xff;
	le->refEntity.shaderRGBA[2] = 0xff;
	le->refEntity.shaderRGBA[3] = 0xff;

	le->color[0] = 1.0;
	le->color[1] = 1.0;
	le->color[2] = 1.0;
	le->color[3] = 1.0;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );

	return(le);
}

void FX_DrawPortableShield(centity_t *cent)
{
	//rww - this code differs a bit from the draw code in EF, I don't know why I had to do
	//it this way yet it worked in EF the other way.

	int				xaxis, height, posWidth, negWidth, team;
	vec3_t			start, end, normal;
	localEntity_t	*le;
	qhandle_t		shader;
	char			buf[1024];

	trap_Cvar_VariableStringBuffer("cl_paused", buf, sizeof(buf));

	if (atoi(buf))
	{ //rww - fix to keep from rendering repeatedly while HUD menu is up
		return;
	}

	if (cent->currentState.eFlags & EF_NODRAW)
	{
		return;
	}

	// decode the data stored in time2
	xaxis = ((cent->currentState.time2 >> 24) & 1);
	height = ((cent->currentState.time2 >> 16) & 255);
	posWidth = ((cent->currentState.time2 >> 8) & 255);
	negWidth = (cent->currentState.time2 & 255);

	team = (cent->currentState.otherEntityNum2);

	VectorClear(normal);

	VectorCopy(cent->lerpOrigin, start);
	VectorCopy(cent->lerpOrigin, end);

	if (xaxis) // drawing along x-axis
	{
		start[0] -= negWidth;
		end[0] += posWidth;
	}
	else
	{
		start[1] -= negWidth;
		end[1] += posWidth;
	}

	normal[0] = 1;
	normal[1] = 1;

	start[2] += height/2;
	end[2] += height/2;

	if (team == TEAM_RED)
	{
		if (cent->currentState.trickedentindex)
		{
			shader = trap_R_RegisterShader( "gfx/misc/red_dmgshield" );
		}
		else
		{
			shader = trap_R_RegisterShader( "gfx/misc/red_portashield" );
		}
	}
	else
	{
		if (cent->currentState.trickedentindex)
		{
			shader = trap_R_RegisterShader( "gfx/misc/blue_dmgshield" );
		}
		else
		{
			shader = trap_R_RegisterShader( "gfx/misc/blue_portashield" );
		}
	}

	le = FX_AddOrientedLine(start, end, normal, 1.0f, height, 0.0f, 1.0f, 1.0f, 50.0, shader);
}

/*
==================
CG_Special
==================
*/
void CG_Special( centity_t *cent ) {
	entityState_t		*s1;

	s1 = &cent->currentState;

	if (!s1)
	{
		return;
	}

	// if set to invisible, skip
	if (!s1->modelindex) {
		return;
	}

	if (s1->modelindex == HI_SHIELD) 
	{	// The portable shield should go through a different rendering function.
		FX_DrawPortableShield(cent);
		return;
	}
}

/*
Ghoul2 Insert Start
*/

// Copy the ghoul2 data into the ref ent correctly
void CG_SetGhoul2Info( refEntity_t *ent, centity_t *cent)
{

	ent->ghoul2 = cent->ghoul2;
	VectorCopy( cent->modelScale, ent->modelScale);
	ent->radius = cent->radius;
	VectorCopy (cent->lerpAngles, ent->angles);
}



// create 8 new points on screen around a model so we can see it's bounding box
void CG_CreateBBRefEnts(entityState_t *s1, vec3_t origin )
{
/* 
//g2r
#if _DEBUG
	refEntity_t		point[8];
	int				i;
	vec3_t			angles = {0,0,0};

	for (i=0; i<8; i++)
	{
		memset (&point[i], 0, sizeof(refEntity_t));
		point[i].reType = RT_SPRITE;
		point[i].radius = 1;
		point[i].customShader = trap_R_RegisterShader("textures/tests/circle");
		point[i].shaderRGBA[0] = 255;
		point[i].shaderRGBA[1] = 255;
		point[i].shaderRGBA[2] = 255;
		point[i].shaderRGBA[3] = 255;

		AnglesToAxis( angles, point[i].axis );

		// now, we need to put the correct origins into each origin from the mins and max's
		switch(i)
		{
		case 0:
			VectorCopy(s1->mins, point[i].origin);
   			break;
		case 1:
			VectorCopy(s1->mins, point[i].origin);
			point[i].origin[0] = s1->maxs[0];
   			break;
		case 2:
			VectorCopy(s1->mins, point[i].origin);
			point[i].origin[1] = s1->maxs[1];
   			break;
		case 3:
			VectorCopy(s1->mins, point[i].origin);
			point[i].origin[0] = s1->maxs[0];
			point[i].origin[1] = s1->maxs[1];
   			break;
		case 4:
			VectorCopy(s1->maxs, point[i].origin);
   			break;
		case 5:
			VectorCopy(s1->maxs, point[i].origin);
			point[i].origin[0] = s1->mins[0];
   			break;
		case 6:
			VectorCopy(s1->maxs, point[i].origin);
			point[i].origin[1] = s1->mins[1];
   			break;
		case 7:
			VectorCopy(s1->maxs, point[i].origin);
			point[i].origin[0] = s1->mins[0];
			point[i].origin[1] = s1->mins[1];
   			break;
		}

		// add the original origin to each point and then stuff them out there
		VectorAdd(point[i].origin, origin, point[i].origin);

		trap_R_AddRefEntityToScene (&point[i]);
	}
#endif
	*/
}

// write in the axis and stuff
void G2_BoltToGhoul2Model(centity_t *cent, refEntity_t *ent)
{
		// extract the wraith ID from the bolt info
	int modelNum = cent->boltInfo >> MODEL_SHIFT;
	int boltNum	= cent->boltInfo >> BOLT_SHIFT;
	int	entNum = cent->boltInfo >> ENTITY_SHIFT;
 	mdxaBone_t 		boltMatrix;
	
	modelNum &= MODEL_AND;
	boltNum &= BOLT_AND;
	entNum &= ENTITY_AND;


	//NOTENOTE I put this here because the cgs.gamemodels array no longer gets initialized.
	assert(0);		


 	// go away and get me the bolt position for this frame please
	trap_G2API_GetBoltMatrix(cent->ghoul2, modelNum, boltNum, &boltMatrix, cg_entities[entNum].currentState.angles, cg_entities[entNum].currentState.origin, cg.time, cgs.gameModels, cent->modelScale);

	// set up the axis and origin we need for the actual effect spawning
 	ent->origin[0] = boltMatrix.matrix[0][3];
 	ent->origin[1] = boltMatrix.matrix[1][3];
 	ent->origin[2] = boltMatrix.matrix[2][3];

 	ent->axis[0][0] = boltMatrix.matrix[0][0];
 	ent->axis[0][1] = boltMatrix.matrix[1][0];
 	ent->axis[0][2] = boltMatrix.matrix[2][0];

 	ent->axis[1][0] = boltMatrix.matrix[0][1];
 	ent->axis[1][1] = boltMatrix.matrix[1][1];
 	ent->axis[1][2] = boltMatrix.matrix[2][1];

 	ent->axis[2][0] = boltMatrix.matrix[0][2];
 	ent->axis[2][1] = boltMatrix.matrix[1][2];
 	ent->axis[2][2] = boltMatrix.matrix[2][2];
}

void ScaleModelAxis(refEntity_t	*ent)

{		// scale the model should we need to
		if (ent->modelScale[0] && ent->modelScale[0] != 1.0f)
		{
			VectorScale( ent->axis[0], ent->modelScale[0] , ent->axis[0] );
			ent->nonNormalizedAxes = qtrue;
		}
		if (ent->modelScale[1] && ent->modelScale[1] != 1.0f)
		{
			VectorScale( ent->axis[1], ent->modelScale[1] , ent->axis[1] );
			ent->nonNormalizedAxes = qtrue;
		}
		if (ent->modelScale[2] && ent->modelScale[2] != 1.0f)
		{
			VectorScale( ent->axis[2], ent->modelScale[2] , ent->axis[2] );
			ent->nonNormalizedAxes = qtrue;
		}
}
/*
Ghoul2 Insert End
*/

char *forceHolocronModels[] = {
	"models/map_objects/mp/lt_heal.md3",		//FP_HEAL,
	"models/map_objects/mp/force_jump.md3",		//FP_LEVITATION,
	"models/map_objects/mp/force_speed.md3",	//FP_SPEED,
	"models/map_objects/mp/force_push.md3",		//FP_PUSH,
	"models/map_objects/mp/force_pull.md3",		//FP_PULL,
	"models/map_objects/mp/lt_telepathy.md3",	//FP_TELEPATHY,
	"models/map_objects/mp/dk_grip.md3",		//FP_GRIP,
	"models/map_objects/mp/dk_lightning.md3",	//FP_LIGHTNING,
	"models/map_objects/mp/dk_rage.md3",		//FP_RAGE,
	"models/map_objects/mp/lt_protect.md3",		//FP_PROTECT,
	"models/map_objects/mp/lt_absorb.md3",		//FP_ABSORB,
	"models/map_objects/mp/lt_healother.md3",	//FP_TEAM_HEAL,
	"models/map_objects/mp/dk_powerother.md3",	//FP_TEAM_FORCE,
	"models/map_objects/mp/dk_drain.md3",		//FP_DRAIN,
	"models/map_objects/mp/force_sight.md3",	//FP_SEE,
	"models/map_objects/mp/saber_attack.md3",	//FP_SABERATTACK,
	"models/map_objects/mp/saber_defend.md3",	//FP_SABERDEFEND,
	"models/map_objects/mp/saber_throw.md3"		//FP_SABERTHROW
};

/*
==================
CG_General
==================
*/
static void CG_General( centity_t *cent ) {
	refEntity_t			ent;
	entityState_t		*s1;
	float				val;
	int					beamID;
	vec3_t				beamOrg;
	mdxaBone_t			matrix;

	if (cent->currentState.modelGhoul2 == 127)
	{ //not ready to be drawn or initialized..
		return;
	}

	if (cent->ghoul2 && !cent->currentState.modelGhoul2 && cent->currentState.eType != ET_BODY &&
		cent->currentState.number >= MAX_CLIENTS)
	{ //this is a bad thing
		if (trap_G2_HaveWeGhoul2Models(cent->ghoul2))
		{
			trap_G2API_CleanGhoul2Models(&(cent->ghoul2));
		}
	}

	if (cent->currentState.modelGhoul2 >= G2_MODELPART_HEAD &&
		cent->currentState.modelGhoul2 <= G2_MODELPART_RLEG &&
		cent->currentState.modelindex < MAX_CLIENTS &&
		cent->currentState.weapon == G2_MODEL_PART)
	{ //special case for client limbs
		centity_t *clEnt = &cg_entities[cent->currentState.modelindex];
		int dismember_settings = cg_dismember.integer;

		if (!dismember_settings)
		{ //This client does not wish to see dismemberment.
			return;
		}

		if (dismember_settings < 2 && (cent->currentState.modelGhoul2 == G2_MODELPART_HEAD || cent->currentState.modelGhoul2 == G2_MODELPART_WAIST))
		{ //dismember settings are not high enough to display decaps and torso slashes
			return;
		}

		if (!cent->ghoul2)
		{
			const char *limbBone;
			const char *rotateBone;
			char *limbName;
			char *limbCapName;
			char *stubCapName;
			char *limbTagName;
			char *stubTagName;
			int limb_anim;
			int newBolt;

			if (clEnt && clEnt->torsoBolt)
			{ //already have a limb missing!
				return;
			}


			if (clEnt && !(clEnt->currentState.eFlags & EF_DEAD))
			{ //death flag hasn't made it through yet for the limb owner, we cannot create the limb until he's flagged as dead
				return;
			}

			cent->bolt4 = -1;
			cent->trailTime = 0;

			switch (cent->currentState.modelGhoul2)
			{
			case G2_MODELPART_HEAD:
				limbBone = "cervical";
				rotateBone = "cranium";
				limbName = "head";
				limbCapName = "head_cap_torso_off";
				stubCapName = "torso_cap_head_off";
				limbTagName = "*head_cap_torso";
				stubTagName = "*torso_cap_head";
				limb_anim = BOTH_DISMEMBER_HEAD1;
				break;
			case G2_MODELPART_WAIST:
				limbBone = "pelvis";
				rotateBone = "thoracic";
				limbName = "torso";
				limbCapName = "torso_cap_hips_off";
				stubCapName = "hips_cap_torso_off";
				limbTagName = "*torso_cap_hips";
				stubTagName = "*hips_cap_torso";
				limb_anim = BOTH_DISMEMBER_TORSO1;
				break;
			case G2_MODELPART_LARM:
				limbBone = "lhumerus";
				rotateBone = "lradius";
				limbName = "l_arm";
				limbCapName = "l_arm_cap_torso_off";
				stubCapName = "torso_cap_l_arm_off";
				limbTagName = "*l_arm_cap_torso";
				stubTagName = "*torso_cap_l_arm";
				limb_anim = BOTH_DISMEMBER_LARM;
				break;
			case G2_MODELPART_RARM:
				limbBone = "rhumerus";
				rotateBone = "rradius";
				limbName = "r_arm";
				limbCapName = "r_arm_cap_torso_off";
				stubCapName = "torso_cap_r_arm_off";
				limbTagName = "*r_arm_cap_torso";
				stubTagName = "*torso_cap_r_arm";
				limb_anim = BOTH_DISMEMBER_RARM;
				break;
			case G2_MODELPART_LLEG:
				limbBone = "lfemurYZ";
				rotateBone = "ltibia";
				limbName = "l_leg";
				limbCapName = "l_leg_cap_hips_off";
				stubCapName = "hips_cap_l_leg_off";
				limbTagName = "*l_leg_cap_hips";
				stubTagName = "*hips_cap_l_leg";
				limb_anim = BOTH_DISMEMBER_LLEG;
				break;
			case G2_MODELPART_RLEG:
				limbBone = "rfemurYZ";
				rotateBone = "rtibia";
				limbName = "r_leg";
				limbCapName = "r_leg_cap_hips_off";
				stubCapName = "hips_cap_r_leg_off";
				limbTagName = "*r_leg_cap_hips";
				stubTagName = "*hips_cap_r_leg";
				limb_anim = BOTH_DISMEMBER_RLEG;
				break;
			default:
				limbBone = "rfemurYZ";
				rotateBone = "rtibia";
				limbName = "r_leg";
				limbCapName = "r_leg_cap_hips_off";
				stubCapName = "hips_cap_r_leg_off";
				limbTagName = "*r_leg_cap_hips";
				stubTagName = "*hips_cap_r_leg";
				limb_anim = BOTH_DISMEMBER_RLEG;
				break;
			}

			if (clEnt && clEnt->ghoul2)
			{
				animation_t		*anim = NULL;
				float			animSpeed;
				int				flags=BONE_ANIM_OVERRIDE_FREEZE;
				clientInfo_t	*ci;

				ci = &cgs.clientinfo[ clEnt->currentState.number ];

				if (ci)
				{
					/*
					if (cent->currentState.modelGhoul2 == G2_MODELPART_WAIST)
					{
						anim = &ci->animations[ cent->currentState.modelindex2 ];
					}
					else
					*/
					{
						anim = &bgGlobalAnimations[ limb_anim ];
					}
				}

				trap_G2API_DuplicateGhoul2Instance(clEnt->ghoul2, &cent->ghoul2);

				if (anim)
				{
					animSpeed = 50.0f / anim->frameLerp;

					if (cent->currentState.modelGhoul2 == G2_MODELPART_WAIST)
					{
						trap_G2API_SetBoneAnim(cent->ghoul2, 0, "upper_lumbar", anim->firstFrame, anim->firstFrame + anim->numFrames, flags, animSpeed, cg.time, -1, 0);
						trap_G2API_SetBoneAnim(cent->ghoul2, 0, "model_root", anim->firstFrame, anim->firstFrame + anim->numFrames, flags, animSpeed, cg.time, -1, 0);
					}
					else
					{
						trap_G2API_SetBoneAnim(cent->ghoul2, 0, "upper_lumbar", anim->firstFrame + anim->numFrames-1, anim->firstFrame + anim->numFrames, flags, animSpeed, cg.time, -1, 0);
						trap_G2API_SetBoneAnim(cent->ghoul2, 0, "model_root", anim->firstFrame + anim->numFrames-1, anim->firstFrame + anim->numFrames, flags, animSpeed, cg.time, -1, 0);
					}
				}
			}

			if (!cent->ghoul2)
			{
				return;
			}

			newBolt = trap_G2API_AddBolt( cent->ghoul2, 0, limbTagName );
			if ( newBolt != -1 )
			{
				vec3_t boltOrg, boltAng;

				trap_G2API_GetBoltMatrix(cent->ghoul2, 0, newBolt, &matrix, cent->lerpAngles, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale);

				trap_G2API_GiveMeVectorFromMatrix(&matrix, ORIGIN, boltOrg);
				trap_G2API_GiveMeVectorFromMatrix(&matrix, NEGATIVE_Y, boltAng);

				trap_FX_PlayEffectID(trap_FX_RegisterEffect("blaster/smoke_bolton"), boltOrg, boltAng);
			}

			cent->bolt4 = newBolt;

			trap_G2API_SetRootSurface(cent->ghoul2, 0, limbName);

			trap_G2API_SetNewOrigin(cent->ghoul2, trap_G2API_AddBolt(cent->ghoul2, 0, rotateBone));

			trap_G2API_SetSurfaceOnOff(cent->ghoul2, limbCapName, 0);

			trap_G2API_SetSurfaceOnOff(clEnt->ghoul2, limbName, 0x00000100);
			trap_G2API_SetSurfaceOnOff(clEnt->ghoul2, stubCapName, 0);

			newBolt = trap_G2API_AddBolt( clEnt->ghoul2, 0, stubTagName );
			if ( newBolt != -1 )
			{
				vec3_t boltOrg, boltAng;

				trap_G2API_GetBoltMatrix(clEnt->ghoul2, 0, newBolt, &matrix, clEnt->lerpAngles, clEnt->lerpOrigin, cg.time, cgs.gameModels, clEnt->modelScale);

				trap_G2API_GiveMeVectorFromMatrix(&matrix, ORIGIN, boltOrg);
				trap_G2API_GiveMeVectorFromMatrix(&matrix, NEGATIVE_Y, boltAng);

				trap_FX_PlayEffectID(trap_FX_RegisterEffect("blaster/smoke_bolton"), boltOrg, boltAng);
			}

			if (cent->currentState.modelGhoul2 == G2_MODELPART_RARM || cent->currentState.modelGhoul2 == G2_MODELPART_WAIST)
			{ //Cut his weapon holding arm off, so remove the weapon
				if (trap_G2API_HasGhoul2ModelOnIndex(&(clEnt->ghoul2), 1))
				{
					trap_G2API_RemoveGhoul2Model(&(clEnt->ghoul2), 1);
				}
			}

			clEnt->torsoBolt = cent->currentState.modelGhoul2; //reinit model after copying limbless one to queue

			return;
		}

		if (cent->ghoul2 && cent->bolt4 != -1 && cent->trailTime < cg.time)
		{
			if ( cent->bolt4 != -1 && 
				(cent->currentState.pos.trDelta[0] || cent->currentState.pos.trDelta[1] || cent->currentState.pos.trDelta[2]) )
			{
				vec3_t boltOrg, boltAng;

				trap_G2API_GetBoltMatrix(cent->ghoul2, 0, cent->bolt4, &matrix, cent->lerpAngles, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale);

				trap_G2API_GiveMeVectorFromMatrix(&matrix, ORIGIN, boltOrg);
				trap_G2API_GiveMeVectorFromMatrix(&matrix, NEGATIVE_Y, boltAng);

				trap_FX_PlayEffectID(trap_FX_RegisterEffect("blaster/smoke_bolton"), boltOrg, boltAng);

				cent->trailTime = cg.time + 400;
			}
		}

		ent.radius = cent->currentState.g2radius;
		ent.hModel = 0;
	}

	if (cent->currentState.number >= MAX_CLIENTS &&
		cent->currentState.activeForcePass == NUM_FORCE_POWERS+1)
	{
		centity_t			*empOwn;
		vec3_t				empAngles, empOrg, anglesToOwner;
		float				angle_ideal, angle_current, angle_dif;
		float				degrees_positive, degrees_negative;
		int					overturn = 0;

		empOwn = &cg_entities[cent->currentState.emplacedOwner];

		if (empOwn)
		{
		
			if (cg.snap->ps.clientNum == empOwn->currentState.number &&
				!cg.renderingThirdPerson)
			{
				VectorCopy(cg.refdefViewAngles, empAngles);
				VectorCopy(cg.refdef.vieworg, empOrg);
			}
			else
			{
				VectorCopy(empOwn->lerpAngles, empAngles);
				VectorCopy(empOwn->lerpOrigin, empOrg);
			}

			VectorSubtract(cent->lerpOrigin, empOrg, anglesToOwner);

			vectoangles(anglesToOwner, anglesToOwner);

			if (empAngles[PITCH] > 40)
			{
				empAngles[PITCH] = 40;
			}

			angle_ideal = empAngles[YAW];
			angle_current = anglesToOwner[YAW];

			if (angle_current < 0)
			{
				angle_current += 360;
			}
			if (angle_current > 360)
			{
				angle_current -= 360;
			}
			if (angle_ideal < 0)
			{
				angle_ideal += 360;
			}
			if (angle_ideal > 360)
			{
				angle_ideal -= 360;
			}

			if (angle_ideal <= angle_current)
			{
				degrees_negative = (angle_current - angle_ideal);

				degrees_positive = (360 - angle_current) + angle_ideal;
			}
			else
			{
				degrees_negative = angle_current + (360 - angle_ideal);

				degrees_positive = (angle_ideal - angle_current);
			}

			if (degrees_negative < degrees_positive)
			{
				angle_dif = degrees_negative;
			}
			else
			{
				angle_dif = degrees_positive;
			}

			if (cg.snap->ps.clientNum == empOwn->currentState.number)
			{
				cg.constrictValue = anglesToOwner[YAW];

				cg.doConstrict = cg.time + 50;
			}

			if (angle_dif > 90)
			{
				overturn = 1;

				if (angle_dif == degrees_negative)
				{
					empAngles[YAW] += (angle_dif - 90);
				}
				else
				{
					empAngles[YAW] -= (angle_dif - 90);
				}
			}
			else if (angle_dif > 80)
			{
				overturn = 2;
			}

			if (!overturn && cg.snap->ps.clientNum == empOwn->currentState.number)
			{
				float plusExt = anglesToOwner[YAW]+70;
				float minusExt = anglesToOwner[YAW]-70;

				if (plusExt > 360)
				{
					plusExt -= 360;
				}
				if (minusExt < 0)
				{
					minusExt += 360;
				}

				trap_SetClientTurnExtent(minusExt, plusExt, cg.time+5000);

				VectorCopy(empAngles, cent->turAngles);
			}
			else if (cg.snap->ps.clientNum == empOwn->currentState.number)
			{
				trap_SetClientForceAngle(cg.time+5000, cent->turAngles);
			}

		//	empAngles[PITCH] -= 160;

			if (empAngles[PITCH] < 0)
			{
				empAngles[PITCH] += 360;
			}
			if (empAngles[YAW] < 0)
			{
				empAngles[YAW] += 360;
			}

			empAngles[YAW] -= cent->currentState.angles[YAW]; //slight hack so that upper rotated half looks right on angled turrets

			//AngleVectors(empAngles, NULL, NULL, up);

			//empAngles[ROLL] = -empAngles[YAW];
			//empAngles[YAW] = 0;

			//trap_G2API_SetBoneAngles(cent->ghoul2, 0, "swivel_bone", empAngles, BONE_ANGLES_REPLACE, POSITIVE_Z, NEGATIVE_X, NEGATIVE_Y, NULL, 0, cg.time);
			//trap_G2API_SetBoneAngles(cent->ghoul2, 0, "swivel_bone", empAngles, BONE_ANGLES_REPLACE, POSITIVE_Z, POSITIVE_X, POSITIVE_Y, NULL, 0, cg.time);

			//constrict the pitch angles of the turret but not the player view
			/*
			if (empAngles[PITCH] < 170)
			{
				empAngles[PITCH] = 170;
			}
			if (empAngles[PITCH] > 240)
			{
				empAngles[PITCH] = 240;
			}
			*/

			trap_G2API_SetBoneAngles( cent->ghoul2, 0, /*"swivel_bone"*/"Bone02", empAngles, BONE_ANGLES_REPLACE, NEGATIVE_Y, NEGATIVE_X, POSITIVE_Z, NULL, 0, cg.time); 
			//trap_G2API_SetBoneAngles( cent->ghoul2, 0, "swivel_bone", empAngles, BONE_ANGLES_POSTMULT, POSITIVE_Y, POSITIVE_Z, POSITIVE_X, NULL, 0, cg.time); 
		}
	}

	s1 = &cent->currentState;
/*
Ghoul2 Insert Start
*/
	// if set to invisible, skip
	if ((!s1->modelindex) && !(trap_G2_HaveWeGhoul2Models(cent->ghoul2))) 
	{
		return;
	}
/*
Ghoul2 Insert End
*/

	memset (&ent, 0, sizeof(ent));

	// set frame

	ent.frame = s1->frame;
	ent.oldframe = ent.frame;
	ent.backlerp = 0;

/*
Ghoul2 Insert Start
*/
	CG_SetGhoul2Info(&ent, cent);

/*
Ghoul2 Insert End
*/
	VectorCopy( cent->lerpOrigin, ent.origin);
	VectorCopy( cent->lerpOrigin, ent.oldorigin);

	if (cent->currentState.modelGhoul2)
	{ //If the game says this guy uses a ghoul2 model and the g2 instance handle is null, then initialize it
		if (!cent->ghoul2 && !cent->currentState.bolt1)
		{
			trap_G2API_InitGhoul2Model(&cent->ghoul2, CG_ConfigString( CS_MODELS+cent->currentState.modelindex ), 0, 0, 0, 0, 0);
		}
		else if (cent->currentState.bolt1)
		{
			TurretClientRun(cent);
		}

		if (cent->ghoul2)
		{ //give us a proper radius
			ent.radius = cent->currentState.g2radius;
		}
	}

	if (s1->eType == ET_BODY)
	{ //bodies should have a radius as well
		ent.radius = cent->currentState.g2radius;

		if (cent->ghoul2)
		{ //all bodies should already have a ghoul2 instance. Use it to set the torso/head angles to 0.
			if (cent->isATST)
			{
				trap_G2API_SetBoneAngles(cent->ghoul2, 0, "pelvis", vec3_origin, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 100, cg.time);
				ent.radius = 250;
			}
			else
			{
				cent->lerpAngles[PITCH] = 0;
				cent->lerpAngles[ROLL] = 0;
				trap_G2API_SetBoneAngles(cent->ghoul2, 0, "pelvis", vec3_origin, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 
				trap_G2API_SetBoneAngles(cent->ghoul2, 0, "thoracic", vec3_origin, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 
				trap_G2API_SetBoneAngles(cent->ghoul2, 0, "upper_lumbar", vec3_origin, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 100, cg.time);
				trap_G2API_SetBoneAngles(cent->ghoul2, 0, "lower_lumbar", vec3_origin, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 100, cg.time);
				trap_G2API_SetBoneAngles(cent->ghoul2, 0, "cranium", vec3_origin, BONE_ANGLES_POSTMULT, POSITIVE_Z, NEGATIVE_Y, POSITIVE_X, cgs.gameModels, 100, cg.time);
			}
		}
	}

	if (s1->eType == ET_HOLOCRON && s1->modelindex < -100)
	{ //special render, it's a holocron
		//Using actual models now:
		ent.hModel = trap_R_RegisterModel(forceHolocronModels[s1->modelindex+128]);

		//Rotate them
		VectorCopy( cg.autoAngles, cent->lerpAngles );
		AxisCopy( cg.autoAxis, ent.axis );
	}
	else
	{
		ent.hModel = cgs.gameModels[s1->modelindex];
	}

	// player model
	if (s1->number == cg.snap->ps.clientNum) {
		ent.renderfx |= RF_THIRD_PERSON;	// only draw from mirrors
	}
/*
Ghoul2 Insert Start
*/
	// are we bolted to a Ghoul2 model?
/* 
//g2r	if (s1->boltInfo)
	{
		G2_BoltToGhoul2Model(s1, &ent);
	}
	else */
	{
		// convert angles to axis
		AnglesToAxis( cent->lerpAngles, ent.axis );
	}

/*
Ghoul2 Insert End
*/
	if ( cent->currentState.time > cg.time && cent->currentState.weapon == WP_EMPLACED_GUN )
	{
		// make the gun pulse red to warn about it exploding
		val = (1.0f - (float)(cent->currentState.time - cg.time) / 3200.0f ) * 0.3f;

		ent.customShader = trap_R_RegisterShader( "gfx/effects/turretflashdie" );
		ent.shaderRGBA[0] = (sin( cg.time * 0.04f ) * val * 0.4f + val) * 255;
		ent.shaderRGBA[1] = ent.shaderRGBA[2] = 0;

		ent.shaderRGBA[3] = 100;
		trap_R_AddRefEntityToScene( &ent );
		ent.customShader = 0;
	}
	else if ( cent->currentState.time == -1 && cent->currentState.weapon == WP_EMPLACED_GUN)
	{
		ent.customShader = trap_R_RegisterShader( "models/map_objects/imp_mine/turret_chair_dmg.tga" );
		//trap_R_AddRefEntityToScene( &ent );
	}

	if ((cent->currentState.eFlags & EF_DISINTEGRATION) && cent->currentState.eType == ET_BODY)
	{
		vec3_t tempAng, hitLoc;
		float tempLength;

		if (!cent->dustTrailTime)
		{
			cent->dustTrailTime = cg.time;
		}

		VectorCopy(cent->currentState.origin2, hitLoc);

		VectorSubtract( hitLoc, ent.origin, ent.oldorigin );
		
		tempLength = VectorNormalize( ent.oldorigin );
		vectoangles( ent.oldorigin, tempAng );
		tempAng[YAW] -= cent->lerpAngles[YAW];
		AngleVectors( tempAng, ent.oldorigin, NULL, NULL );
		VectorScale( ent.oldorigin, tempLength, ent.oldorigin );

		ent.endTime = cent->dustTrailTime;

		/*
		ent.renderfx |= RF_DISINTEGRATE2;

		ent.customShader = cgs.media.disruptorShader;
		trap_R_AddRefEntityToScene( &ent );
		*/

		ent.renderfx &= ~(RF_DISINTEGRATE2);
		ent.renderfx |= (RF_DISINTEGRATE1);
		ent.customShader = 0;
		trap_R_AddRefEntityToScene( &ent );

		return;
	}
	else if (cent->currentState.eType == ET_BODY)
	{
		cent->dustTrailTime = 0;
	}

	// add to refresh list
	trap_R_AddRefEntityToScene (&ent);

	if (cent->bolt3 == 999)
	{ //this is an in-flight saber being rendered manually
		vec3_t org;
		float wv;
		int i;
		addspriteArgStruct_t fxSArgs;
		//refEntity_t sRef;
		//memcpy( &sRef, &ent, sizeof( sRef ) );

		ent.customShader = cgs.media.solidWhite;
		ent.renderfx = RF_RGB_TINT;
		wv = sin( cg.time * 0.003f ) * 0.08f + 0.1f;
		ent.shaderRGBA[0] = wv * 255;
		ent.shaderRGBA[1] = wv * 255;
		ent.shaderRGBA[2] = wv * 0;
		trap_R_AddRefEntityToScene (&ent);

		for ( i = -4; i < 10; i += 1 )
		{
			VectorMA( ent.origin, -i, ent.axis[2], org );

			VectorCopy(org, fxSArgs.origin);
			VectorClear(fxSArgs.vel);
			VectorClear(fxSArgs.accel);
			fxSArgs.scale = 5.5f;
			fxSArgs.dscale = 5.5f;
			fxSArgs.sAlpha = wv;
			fxSArgs.eAlpha = wv;
			fxSArgs.rotation = 0.0f;
			fxSArgs.bounce = 0.0f;
			fxSArgs.life = 1.0f;
			fxSArgs.shader = cgs.media.yellowDroppedSaberShader;
			fxSArgs.flags = 0x08000000;

			//trap_FX_AddSprite( org, NULL, NULL, 5.5f, 5.5f, wv, wv, 0.0f, 0.0f, 1.0f, cgs.media.yellowSaberGlowShader, 0x08000000 );
			trap_FX_AddSprite(&fxSArgs);
		}
	}
	else if (cent->currentState.trickedentindex3)
	{ //holocron special effects
		vec3_t org;
		float wv;
		addspriteArgStruct_t fxSArgs;
		//refEntity_t sRef;
		//memcpy( &sRef, &ent, sizeof( sRef ) );

		ent.customShader = cgs.media.solidWhite;
		ent.renderfx = RF_RGB_TINT;
		wv = sin( cg.time * 0.005f ) * 0.08f + 0.1f; //* 0.08f + 0.1f;

		if (cent->currentState.trickedentindex3 == 1)
		{ //dark
			ent.shaderRGBA[0] = wv*255;
			ent.shaderRGBA[1] = 0;
			ent.shaderRGBA[2] = 0;
		}
		else if (cent->currentState.trickedentindex3 == 2)
		{ //light
			ent.shaderRGBA[0] = wv*255;
			ent.shaderRGBA[1] = wv*255;
			ent.shaderRGBA[2] = wv*255;
		}
		else
		{ //neutral
			if ((s1->modelindex+128) == FP_SABERATTACK ||
				(s1->modelindex+128) == FP_SABERDEFEND ||
				(s1->modelindex+128) == FP_SABERTHROW)
			{ //saber power
				ent.shaderRGBA[0] = 0;
				ent.shaderRGBA[1] = wv*255;
				ent.shaderRGBA[2] = 0;
			}
			else
			{
				ent.shaderRGBA[0] = 0;
				ent.shaderRGBA[1] = wv*255;
				ent.shaderRGBA[2] = wv*255;
			}
		}

		ent.modelScale[0] = 1.1;
		ent.modelScale[1] = 1.1;
		ent.modelScale[2] = 1.1;

		ent.origin[2] -= 2;
		ScaleModelAxis(&ent);

		trap_R_AddRefEntityToScene (&ent);
		
		VectorMA( ent.origin, 1, ent.axis[2], org );

		org[2] += 18;

		wv = sin( cg.time * 0.002f ) * 0.08f + 0.1f; //* 0.08f + 0.1f;

		VectorCopy(org, fxSArgs.origin);
		VectorClear(fxSArgs.vel);
		VectorClear(fxSArgs.accel);
		fxSArgs.scale = wv*120;//16.0f;
		fxSArgs.dscale = wv*120;//16.0f;
		fxSArgs.sAlpha = wv*12;
		fxSArgs.eAlpha = wv*12;
		fxSArgs.rotation = 0.0f;
		fxSArgs.bounce = 0.0f;
		fxSArgs.life = 1.0f;

		fxSArgs.flags = 0x08000000|0x00000001;

		if (cent->currentState.trickedentindex3 == 1)
		{ //dark
			fxSArgs.sAlpha *= 3;
			fxSArgs.eAlpha *= 3;
			fxSArgs.shader = cgs.media.redSaberGlowShader;
			trap_FX_AddSprite(&fxSArgs);
		}
		else if (cent->currentState.trickedentindex3 == 2)
		{ //light
			fxSArgs.sAlpha *= 1.5;
			fxSArgs.eAlpha *= 1.5;
			fxSArgs.shader = cgs.media.redSaberGlowShader;
			trap_FX_AddSprite(&fxSArgs);
			fxSArgs.shader = cgs.media.greenSaberGlowShader;
			trap_FX_AddSprite(&fxSArgs);
			fxSArgs.shader = cgs.media.blueSaberGlowShader;
			trap_FX_AddSprite(&fxSArgs);
		}
		else
		{ //neutral
			if ((s1->modelindex+128) == FP_SABERATTACK ||
				(s1->modelindex+128) == FP_SABERDEFEND ||
				(s1->modelindex+128) == FP_SABERTHROW)
			{ //saber power
				fxSArgs.sAlpha *= 1.5;
				fxSArgs.eAlpha *= 1.5;
				fxSArgs.shader = cgs.media.greenSaberGlowShader;
				trap_FX_AddSprite(&fxSArgs);
			}
			else
			{
				fxSArgs.sAlpha *= 0.5;
				fxSArgs.eAlpha *= 0.5;
				fxSArgs.shader = cgs.media.greenSaberGlowShader;
				trap_FX_AddSprite(&fxSArgs);
				fxSArgs.shader = cgs.media.blueSaberGlowShader;
				trap_FX_AddSprite(&fxSArgs);
			}
		}
	}

	if ( cent->currentState.time == -1 && cent->currentState.weapon == WP_TRIP_MINE && (cent->currentState.eFlags & EF_FIRING) )
	{ //if force sight is active, render the laser multiple times up to the force sight level to increase visibility
		int i = 0;

		VectorMA( ent.origin, 6.6f, ent.axis[0], beamOrg );// forward
		beamID = cgs.effects.tripmineLaserFX;

		if (cg.snap->ps.fd.forcePowersActive & (1 << FP_SEE))
		{
			i = cg.snap->ps.fd.forcePowerLevel[FP_SEE];

			while (i > 0)
			{
				trap_FX_PlayEffectID( beamID, beamOrg, cent->currentState.pos.trDelta );
				trap_FX_PlayEffectID( beamID, beamOrg, cent->currentState.pos.trDelta );
				i--;
			}
		}

		trap_FX_PlayEffectID( beamID, beamOrg, cent->currentState.pos.trDelta );
	}
/*
Ghoul2 Insert Start
*/

	if (cg_debugBB.integer)
	{
		CG_CreateBBRefEnts(s1, cent->lerpOrigin);
	}
/*
Ghoul2 Insert End
*/
}

/*
==================
CG_Speaker

Speaker entities can automatically play sounds
==================
*/
static void CG_Speaker( centity_t *cent ) {
	if (cent->currentState.trickedentindex)
	{
		trap_S_StopLoopingSound(cent->currentState.number);
	}

	if ( ! cent->currentState.clientNum ) {	// FIXME: use something other than clientNum...
		return;		// not auto triggering
	}

	if ( cg.time < cent->miscTime ) {
		return;
	}

	trap_S_StartSound (NULL, cent->currentState.number, CHAN_ITEM, cgs.gameSounds[cent->currentState.eventParm] );

	//	ent->s.frame = ent->wait * 10;
	//	ent->s.clientNum = ent->random * 10;
	cent->miscTime = cg.time + cent->currentState.frame * 100 + cent->currentState.clientNum * 100 * crandom();
}

qboolean CG_GreyItem(int type, int tag, int plSide)
{
	if (type == IT_POWERUP &&
		(tag == PW_FORCE_ENLIGHTENED_LIGHT || tag == PW_FORCE_ENLIGHTENED_DARK))
	{
		if (plSide == FORCE_LIGHTSIDE)
		{
			if (tag == PW_FORCE_ENLIGHTENED_DARK)
			{
				return qtrue;
			}
		}
		else if (plSide == FORCE_DARKSIDE)
		{
			if (tag == PW_FORCE_ENLIGHTENED_LIGHT)
			{
				return qtrue;
			}
		}
	}

	return qfalse;
}

/*
==================
CG_Item
==================
*/
static void CG_Item( centity_t *cent ) {
	refEntity_t		ent;
	entityState_t	*es;
	gitem_t			*item;
	int				msec;
	float			scale;
	weaponInfo_t	*wi;

	es = &cent->currentState;
	if ( es->modelindex >= bg_numItems ) {
		CG_Error( "Bad item index %i on entity", es->modelindex );
	}

/*
Ghoul2 Insert Start
*/

	if ((es->eFlags & EF_NODRAW) && (es->eFlags & EF_ITEMPLACEHOLDER))
	{
		es->eFlags &= ~EF_NODRAW;
	}

	if ( !es->modelindex ) 
	{
		return;
	}

	item = &bg_itemlist[ es->modelindex ];

	if ((item->giType == IT_WEAPON || item->giType == IT_POWERUP) &&
		!(cent->currentState.eFlags & EF_DROPPEDWEAPON) &&
		!cg_simpleItems.integer)
	{
		vec3_t uNorm;
		qboolean doGrey;
		
		VectorClear(uNorm);

		uNorm[2] = 1;

		memset( &ent, 0, sizeof( ent ) );

		ent.customShader = 0;
		VectorCopy(cent->lerpOrigin, ent.origin);
		VectorCopy( cent->currentState.angles, cent->lerpAngles );
		AnglesToAxis(cent->lerpAngles, ent.axis);
		ent.hModel = cgs.media.itemHoloModel;

		doGrey = CG_GreyItem(item->giType, item->giTag, cg.snap->ps.fd.forceSide);

		if (doGrey)
		{
			ent.renderfx |= RF_RGB_TINT;

			ent.shaderRGBA[0] = 150;
			ent.shaderRGBA[1] = 150;
			ent.shaderRGBA[2] = 150;
		}

		trap_R_AddRefEntityToScene(&ent);

		if (!doGrey)
		{
			trap_FX_PlayEffectID(trap_FX_RegisterEffect("mp/itemcone.efx"), ent.origin, uNorm);
		}
	}

	// if set to invisible, skip
	if ( ( es->eFlags & EF_NODRAW ) ) 
	{
		return;
	}
/*
Ghoul2 Insert End
*/

	if ( cg_simpleItems.integer && item->giType != IT_TEAM ) {
		memset( &ent, 0, sizeof( ent ) );
		ent.reType = RT_SPRITE;
		VectorCopy( cent->lerpOrigin, ent.origin );
		ent.radius = 14;
		ent.customShader = cg_items[es->modelindex].icon;
		ent.shaderRGBA[0] = 255;
		ent.shaderRGBA[1] = 255;
		ent.shaderRGBA[2] = 255;

		ent.origin[2] += 16;

		if (item->giType != IT_POWERUP || item->giTag != PW_FORCE_BOON)
		{
			ent.renderfx |= RF_FORCE_ENT_ALPHA;
		}

		if ( es->eFlags & EF_ITEMPLACEHOLDER )
		{
			if (item->giType == IT_POWERUP && item->giTag == PW_FORCE_BOON)
			{
				return;
			}
			ent.shaderRGBA[0] = 200;
			ent.shaderRGBA[1] = 200;
			ent.shaderRGBA[2] = 200;
			ent.shaderRGBA[3] = 150 + sin(cg.time*0.01)*30;
		}
		else
		{
			ent.shaderRGBA[3] = 255;
		}

		if (CG_GreyItem(item->giType, item->giTag, cg.snap->ps.fd.forceSide))
		{
			ent.shaderRGBA[0] = 100;
			ent.shaderRGBA[1] = 100;
			ent.shaderRGBA[2] = 100;

			ent.shaderRGBA[3] = 200;

			if (item->giTag == PW_FORCE_ENLIGHTENED_LIGHT)
			{
				ent.customShader = trap_R_RegisterShader("gfx/misc/mp_light_enlight_disable");
			}
			else
			{
				ent.customShader = trap_R_RegisterShader("gfx/misc/mp_dark_enlight_disable");
			}
		}
		trap_R_AddRefEntityToScene(&ent);
		return;
	}

	if ((item->giType == IT_WEAPON || item->giType == IT_POWERUP) &&
		!(cent->currentState.eFlags & EF_DROPPEDWEAPON))
	{
		cent->lerpOrigin[2] += 16;
	}

	if ((!(cent->currentState.eFlags & EF_DROPPEDWEAPON) || item->giType == IT_POWERUP) &&
		(item->giType == IT_WEAPON || item->giType == IT_POWERUP))
	{
		// items bob up and down continuously
		scale = 0.005 + cent->currentState.number * 0.00001;
		cent->lerpOrigin[2] += 4 + cos( ( cg.time + 1000 ) *  scale ) * 4;
	}
	else
	{
		if (item->giType == IT_HOLDABLE)
		{
			if (item->giTag == HI_SEEKER)
			{
				cent->lerpOrigin[2] += 5;
			}
			if (item->giTag == HI_SHIELD)
			{
				cent->lerpOrigin[2] += 2;
			}
			if (item->giTag == HI_BINOCULARS)
			{
				cent->lerpOrigin[2] += 2;
			}
		}
		if (item->giType == IT_HEALTH)
		{
			cent->lerpOrigin[2] += 2;
		}
		if (item->giType == IT_ARMOR)
		{
			if (item->quantity == 100)
			{
				cent->lerpOrigin[2] += 7;
			}
		}
	}

	memset (&ent, 0, sizeof(ent));

	if ( (!(cent->currentState.eFlags & EF_DROPPEDWEAPON) || item->giType == IT_POWERUP) &&
		(item->giType == IT_WEAPON || item->giType == IT_POWERUP) )
	{ //only weapons and powerups rotate now
		// autorotate at one of two speeds
		VectorCopy( cg.autoAngles, cent->lerpAngles );
		AxisCopy( cg.autoAxis, ent.axis );
	}
	else
	{
		VectorCopy( cent->currentState.angles, cent->lerpAngles );
		AnglesToAxis(cent->lerpAngles, ent.axis);
	}

	wi = NULL;
	// the weapons have their origin where they attatch to player
	// models, so we need to offset them or they will rotate
	// eccentricly
	if (!(cent->currentState.eFlags & EF_DROPPEDWEAPON))
	{
		if ( item->giType == IT_WEAPON ) {
			wi = &cg_weapons[item->giTag];
			cent->lerpOrigin[0] -= 
				wi->weaponMidpoint[0] * ent.axis[0][0] +
				wi->weaponMidpoint[1] * ent.axis[1][0] +
				wi->weaponMidpoint[2] * ent.axis[2][0];
			cent->lerpOrigin[1] -= 
				wi->weaponMidpoint[0] * ent.axis[0][1] +
				wi->weaponMidpoint[1] * ent.axis[1][1] +
				wi->weaponMidpoint[2] * ent.axis[2][1];
			cent->lerpOrigin[2] -= 
				wi->weaponMidpoint[0] * ent.axis[0][2] +
				wi->weaponMidpoint[1] * ent.axis[1][2] +
				wi->weaponMidpoint[2] * ent.axis[2][2];

			cent->lerpOrigin[2] += 8;	// an extra height boost
		}
	}
	else
	{
		wi = &cg_weapons[item->giTag];

		switch(item->giTag)
		{
		case WP_BLASTER:
			cent->lerpOrigin[2] -= 12;
			break;
		case WP_DISRUPTOR:
			cent->lerpOrigin[2] -= 13;
			break;
		case WP_BOWCASTER:
			cent->lerpOrigin[2] -= 16;
			break;
		case WP_REPEATER:
			cent->lerpOrigin[2] -= 12;
			break;
		case WP_DEMP2:
			cent->lerpOrigin[2] -= 10;
			break;
		case WP_FLECHETTE:
			cent->lerpOrigin[2] -= 6;
			break;
		case WP_ROCKET_LAUNCHER:
			cent->lerpOrigin[2] -= 11;
			break;
		case WP_THERMAL:
			cent->lerpOrigin[2] -= 12;
			break;
		case WP_TRIP_MINE:
			cent->lerpOrigin[2] -= 16;
			break;
		case WP_DET_PACK:
			cent->lerpOrigin[2] -= 16;
			break;
		default:
			cent->lerpOrigin[2] -= 8;
			break;
		}
	}

	ent.hModel = cg_items[es->modelindex].models[0];
/*
Ghoul2 Insert Start
*/
	ent.ghoul2 = cg_items[es->modelindex].g2Models[0];
	ent.radius = cg_items[es->modelindex].radius[0];
	VectorCopy (cent->lerpAngles, ent.angles);
/*
Ghoul2 Insert End
*/
	VectorCopy( cent->lerpOrigin, ent.origin);
	VectorCopy( cent->lerpOrigin, ent.oldorigin);

	ent.nonNormalizedAxes = qfalse;

	// if just respawned, slowly scale up
	
	msec = cg.time - cent->miscTime;

	if (CG_GreyItem(item->giType, item->giTag, cg.snap->ps.fd.forceSide))
	{
		ent.renderfx |= RF_RGB_TINT;

		ent.shaderRGBA[0] = 150;
		ent.shaderRGBA[1] = 150;
		ent.shaderRGBA[2] = 150;

		ent.renderfx |= RF_FORCE_ENT_ALPHA;

		ent.shaderRGBA[3] = 200;

		if (item->giTag == PW_FORCE_ENLIGHTENED_LIGHT)
		{
			ent.customShader = trap_R_RegisterShader("gfx/misc/mp_light_enlight_disable");
		}
		else
		{
			ent.customShader = trap_R_RegisterShader("gfx/misc/mp_dark_enlight_disable");
		}

		trap_R_AddRefEntityToScene( &ent );
		return;
	}

	if ( es->eFlags & EF_ITEMPLACEHOLDER )		// item has been picked up
	{
		if ( es->eFlags & EF_DEAD )				// if item had been droped, don't show at all
			return;

		ent.renderfx |= RF_RGB_TINT;
		ent.shaderRGBA[0] = 0;
		ent.shaderRGBA[1] = 200;
		ent.shaderRGBA[2] = 85;
		ent.customShader = cgs.media.itemRespawningPlaceholder;
	}

	// increase the size of the weapons when they are presented as items
	if ( item->giType == IT_WEAPON ) {
		VectorScale( ent.axis[0], 1.5, ent.axis[0] );
		VectorScale( ent.axis[1], 1.5, ent.axis[1] );
		VectorScale( ent.axis[2], 1.5, ent.axis[2] );
		ent.nonNormalizedAxes = qtrue;
		//trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, cgs.media.weaponHoverSound );
	}

	if (!(cent->currentState.eFlags & EF_DROPPEDWEAPON) &&
		(item->giType == IT_WEAPON || item->giType == IT_POWERUP))
	{
		ent.renderfx |= RF_MINLIGHT;
	}

	if (item->giType != IT_TEAM && msec >= 0 && msec < ITEM_SCALEUP_TIME && !(es->eFlags & EF_ITEMPLACEHOLDER) && !(es->eFlags & EF_DROPPEDWEAPON)) 
	{	// if just respawned, fade in, but don't do this for flags.
		float alpha;
		int a;
		
		alpha = (float)msec / ITEM_SCALEUP_TIME;
		a = alpha * 255.0;
		if (a <= 0)
			a=1;

		ent.shaderRGBA[3] = a;
		if (item->giType != IT_POWERUP || item->giTag != PW_FORCE_BOON)
		{ //boon model uses a different blending mode for the sprite inside and doesn't look proper with this method
			ent.renderfx |= RF_FORCE_ENT_ALPHA;
		}
		trap_R_AddRefEntityToScene(&ent);
		
		ent.renderfx &= ~RF_FORCE_ENT_ALPHA;
	
		// Now draw the static shader over it.
		// Alpha in over half the time, out over half.
		
		//alpha = sin(M_PI*alpha);
		a = alpha * 255.0;

		a = 255 - a;

		if (a <= 0)
			a=1;
		if (a > 255)
			a=255;

		ent.customShader = cgs.media.itemRespawningRezOut;

		/*
		ent.shaderRGBA[0] = 0;
		ent.shaderRGBA[1] = a;
		ent.shaderRGBA[2] = a-100;

		if (ent.shaderRGBA[2] < 0)
		{
			ent.shaderRGBA[2] = 0;
		}
		*/

		/*
		ent.shaderRGBA[0] =
		ent.shaderRGBA[1] =
		ent.shaderRGBA[2] = a;
		*/

		ent.renderfx |= RF_RGB_TINT;
		ent.shaderRGBA[0] = 0;
		ent.shaderRGBA[1] = 200;
		ent.shaderRGBA[2] = 85;

		trap_R_AddRefEntityToScene( &ent );
	}
	else
	{	// add to refresh list  -- normal item
		if (item->giType == IT_TEAM &&
			(item->giTag == PW_REDFLAG || item->giTag == PW_BLUEFLAG))
		{
			ent.modelScale[0] = 0.7;
			ent.modelScale[1] = 0.7;
			ent.modelScale[2] = 0.7;
			ScaleModelAxis(&ent);
		}
		trap_R_AddRefEntityToScene(&ent);
	}

	//rww - As far as I can see, this is useless.
	/*
	if ( item->giType == IT_WEAPON && wi->barrelModel ) {
		refEntity_t	barrel;

		memset( &barrel, 0, sizeof( barrel ) );

		barrel.hModel = wi->barrelModel;

		VectorCopy( ent.lightingOrigin, barrel.lightingOrigin );
		barrel.shadowPlane = ent.shadowPlane;
		barrel.renderfx = ent.renderfx;

		barrel.customShader = ent.customShader;

		CG_PositionRotatedEntityOnTag( &barrel, &ent, wi->weaponModel, "tag_barrel" );

		AxisCopy( ent.axis, barrel.axis );
		barrel.nonNormalizedAxes = ent.nonNormalizedAxes;

		trap_R_AddRefEntityToScene( &barrel );
	}
	*/

	// accompanying rings / spheres for powerups
	if ( !cg_simpleItems.integer ) 
	{
		vec3_t spinAngles;

		VectorClear( spinAngles );

		if ( item->giType == IT_HEALTH || item->giType == IT_POWERUP )
		{
			if ( ( ent.hModel = cg_items[es->modelindex].models[1] ) != 0 )
			{
				if ( item->giType == IT_POWERUP )
				{
					ent.origin[2] += 12;
					spinAngles[1] = ( cg.time & 1023 ) * 360 / -1024.0f;
				}
				AnglesToAxis( spinAngles, ent.axis );
				
				trap_R_AddRefEntityToScene( &ent );
			}
		}
	}
}

//============================================================================

/*
===============
CG_Missile
===============
*/
static void CG_Missile( centity_t *cent ) {
	refEntity_t			ent;
	entityState_t		*s1;
	const weaponInfo_t		*weapon;
//	int	col;

	s1 = &cent->currentState;
	if ( s1->weapon > WP_NUM_WEAPONS && s1->weapon != G2_MODEL_PART ) {
		s1->weapon = 0;
	}

	if (cent->ghoul2 && s1->weapon == G2_MODEL_PART)
	{
		weapon = &cg_weapons[WP_SABER];
	}
	else
	{
		weapon = &cg_weapons[s1->weapon];
	}

	if (s1->weapon == WP_SABER)
	{
		if (!cent->ghoul2 && !(s1->eFlags & EF_NODRAW))
		{
			trap_G2API_InitGhoul2Model(&cent->ghoul2, "models/weapons2/saber/saber_w.glm", 0, 0, 0, 0, 0);
			return;
		}
		else if (s1->eFlags & EF_NODRAW)
		{
			return;
		}
	}

	if (cent->ghoul2)
	{ //give us a proper radius
		ent.radius = cent->currentState.g2radius;
	}

	// calculate the axis
	VectorCopy( s1->angles, cent->lerpAngles);

	if ( cent->currentState.eFlags & EF_ALT_FIRING )
	{
		// add trails
		if ( weapon->altMissileTrailFunc )  
			weapon->altMissileTrailFunc( cent, weapon );

		// add dynamic light
		if ( weapon->altMissileDlight ) 
		{
			trap_R_AddLightToScene(cent->lerpOrigin, weapon->altMissileDlight, 
				weapon->altMissileDlightColor[0], weapon->altMissileDlightColor[1], weapon->altMissileDlightColor[2] );
		}

		// add missile sound
		if ( weapon->altMissileSound ) {
			vec3_t	velocity;

			BG_EvaluateTrajectoryDelta( &cent->currentState.pos, cg.time, velocity );

			trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, velocity, weapon->altMissileSound );
		}

		//Don't draw something without a model
		if ( weapon->altMissileModel == NULL_HANDLE )
			return;
	}
	else
	{
		// add trails
		if ( weapon->missileTrailFunc )  
			weapon->missileTrailFunc( cent, weapon );

		// add dynamic light
		if ( weapon->missileDlight ) 
		{
			trap_R_AddLightToScene(cent->lerpOrigin, weapon->missileDlight, 
				weapon->missileDlightColor[0], weapon->missileDlightColor[1], weapon->missileDlightColor[2] );
		}

		// add missile sound
		if ( weapon->missileSound ) 
		{
			vec3_t	velocity;

			BG_EvaluateTrajectoryDelta( &cent->currentState.pos, cg.time, velocity );

			trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, velocity, weapon->missileSound );
		}

		//Don't draw something without a model
		if ( weapon->missileModel == NULL_HANDLE && s1->weapon != WP_SABER && s1->weapon != G2_MODEL_PART ) //saber uses ghoul2 model, doesn't matter
			return;
	}

	// create the render entity
	memset (&ent, 0, sizeof(ent));
	VectorCopy( cent->lerpOrigin, ent.origin);
	VectorCopy( cent->lerpOrigin, ent.oldorigin);
/*
Ghoul2 Insert Start
*/
	CG_SetGhoul2Info(&ent, cent);  

/*
Ghoul2 Insert End
*/

	// flicker between two skins
	ent.skinNum = cg.clientFrame & 1;
	ent.renderfx = /*weapon->missileRenderfx | */RF_NOSHADOW;

	if (s1->weapon != WP_SABER && s1->weapon != G2_MODEL_PART)
	{
		//if ( cent->currentState.eFlags | EF_ALT_FIRING )
		//rww - why was this like this?
		if ( cent->currentState.eFlags & EF_ALT_FIRING )
		{
			ent.hModel = weapon->altMissileModel;
		}
		else
		{
			ent.hModel = weapon->missileModel;
		}
	}

	// spin as it moves
	if ( s1->apos.trType != TR_INTERPOLATE )
	{
		// convert direction of travel into axis
		if ( VectorNormalize2( s1->pos.trDelta, ent.axis[0] ) == 0 ) {
			ent.axis[0][2] = 1;
		}

		// spin as it moves
		if ( s1->pos.trType != TR_STATIONARY ) 
		{
			if ( s1->eFlags & EF_MISSILE_STICK )
			{
				RotateAroundDirection( ent.axis, cg.time * 0.5f );//Did this so regular missiles don't get broken
			}
			else
			{
				RotateAroundDirection( ent.axis, cg.time * 0.25f );//JFM:FLOAT FIX
			}
		} 
		else 
		{
			if ( s1->eFlags & EF_MISSILE_STICK )
			{
				RotateAroundDirection( ent.axis, (float)s1->pos.trTime * 0.5f );
			}
			else
			{
				RotateAroundDirection( ent.axis, (float)s1->time );
			}
		}
	}
	else
	{
		AnglesToAxis( cent->lerpAngles, ent.axis );
	}

	if (s1->weapon == WP_SABER)
	{
		ent.radius = s1->g2radius;
	}

	// add to refresh list, possibly with quad glow
	CG_AddRefEntityWithPowerups( &ent, s1, TEAM_FREE );

	if (s1->weapon == WP_SABER && cgs.gametype == GT_JEDIMASTER)
	{ //in jedimaster always make the saber glow when on the ground
		vec3_t org;
		float wv;
		int i;
		addspriteArgStruct_t fxSArgs;
		//refEntity_t sRef;
		//memcpy( &sRef, &ent, sizeof( sRef ) );

		ent.customShader = cgs.media.solidWhite;
		ent.renderfx = RF_RGB_TINT;
		wv = sin( cg.time * 0.003f ) * 0.08f + 0.1f;
		ent.shaderRGBA[0] = wv * 255;
		ent.shaderRGBA[1] = wv * 255;
		ent.shaderRGBA[2] = wv * 0;
		trap_R_AddRefEntityToScene (&ent);

		for ( i = -4; i < 10; i += 1 )
		{
			VectorMA( ent.origin, -i, ent.axis[2], org );

			VectorCopy(org, fxSArgs.origin);
			VectorClear(fxSArgs.vel);
			VectorClear(fxSArgs.accel);
			fxSArgs.scale = 5.5f;
			fxSArgs.dscale = 5.5f;
			fxSArgs.sAlpha = wv;
			fxSArgs.eAlpha = wv;
			fxSArgs.rotation = 0.0f;
			fxSArgs.bounce = 0.0f;
			fxSArgs.life = 1.0f;
			fxSArgs.shader = cgs.media.yellowDroppedSaberShader;
			fxSArgs.flags = 0x08000000;

			//trap_FX_AddSprite( org, NULL, NULL, 5.5f, 5.5f, wv, wv, 0.0f, 0.0f, 1.0f, cgs.media.yellowSaberGlowShader, 0x08000000 );
			trap_FX_AddSprite(&fxSArgs);
		}

		if (cgs.gametype == GT_JEDIMASTER)
		{
			ent.shaderRGBA[0] = 255;
			ent.shaderRGBA[1] = 255;
			ent.shaderRGBA[2] = 0;

			ent.renderfx |= RF_DEPTHHACK;
			ent.customShader = cgs.media.forceSightBubble;
		
			trap_R_AddRefEntityToScene( &ent );
		}
	}

	if ( s1->eFlags & EF_FIRING )
	{//special code for adding the beam to the attached tripwire mine
		vec3_t	beamOrg;

		VectorMA( ent.origin, 8, ent.axis[0], beamOrg );// forward
		trap_FX_PlayEffect( "tripMine/laser.efx", beamOrg, ent.axis[0] );
	}
}

/*
===============
CG_Mover
===============
*/
static void CG_Mover( centity_t *cent ) {
	refEntity_t			ent;
	entityState_t		*s1;

	s1 = &cent->currentState;

	// create the render entity
	memset (&ent, 0, sizeof(ent));
	VectorCopy( cent->lerpOrigin, ent.origin);
	VectorCopy( cent->lerpOrigin, ent.oldorigin);
	AnglesToAxis( cent->lerpAngles, ent.axis );

	ent.renderfx = RF_NOSHADOW;
/*
Ghoul2 Insert Start
*/

	CG_SetGhoul2Info(&ent, cent);  
/*
Ghoul2 Insert End
*/
	// flicker between two skins (FIXME?)
	ent.skinNum = ( cg.time >> 6 ) & 1;

	// get the model, either as a bmodel or a modelindex
	if ( s1->solid == SOLID_BMODEL ) 
	{
		ent.hModel = cgs.inlineDrawModel[s1->modelindex];
	} 
	else 
	{
		ent.hModel = cgs.gameModels[s1->modelindex];
	}

	// add to refresh list
	trap_R_AddRefEntityToScene(&ent);

	// add the secondary model
	if ( s1->modelindex2 ) 
	{
		ent.skinNum = 0;
		ent.hModel = cgs.gameModels[s1->modelindex2];
		trap_R_AddRefEntityToScene(&ent);
	}

}

/*
===============
CG_Beam

Also called as an event
===============
*/
void CG_Beam( centity_t *cent ) {
	refEntity_t			ent;
	entityState_t		*s1;

	s1 = &cent->currentState;

	// create the render entity
	memset (&ent, 0, sizeof(ent));
	VectorCopy( s1->pos.trBase, ent.origin );
	VectorCopy( s1->origin2, ent.oldorigin );
	AxisClear( ent.axis );
	ent.reType = RT_BEAM;

	ent.renderfx = RF_NOSHADOW;
/*
Ghoul2 Insert Start
*/
	CG_SetGhoul2Info(&ent, cent);  

/*
Ghoul2 Insert End
*/
	// add to refresh list
	trap_R_AddRefEntityToScene(&ent);
}


/*
===============
CG_Portal
===============
*/
static void CG_Portal( centity_t *cent ) {
	refEntity_t			ent;
	entityState_t		*s1;

	s1 = &cent->currentState;

	// create the render entity
	memset (&ent, 0, sizeof(ent));
	VectorCopy( cent->lerpOrigin, ent.origin );
	VectorCopy( s1->origin2, ent.oldorigin );
	ByteToDir( s1->eventParm, ent.axis[0] );
	PerpendicularVector( ent.axis[1], ent.axis[0] );

	// negating this tends to get the directions like they want
	// we really should have a camera roll value
	VectorSubtract( vec3_origin, ent.axis[1], ent.axis[1] );

	CrossProduct( ent.axis[0], ent.axis[1], ent.axis[2] );
	ent.reType = RT_PORTALSURFACE;
	ent.oldframe = s1->powerups;
	ent.frame = s1->frame;		// rotation speed
	ent.skinNum = s1->clientNum/256.0 * 360;	// roll offset
/*
Ghoul2 Insert Start
*/
	CG_SetGhoul2Info(&ent, cent);  
/*
Ghoul2 Insert End
*/
	// add to refresh list
	trap_R_AddRefEntityToScene(&ent);
}


/*
=========================
CG_AdjustPositionForMover

Also called by client movement prediction code
=========================
*/
void CG_AdjustPositionForMover( const vec3_t in, int moverNum, int fromTime, int toTime, vec3_t out ) {
	centity_t	*cent;
	vec3_t	oldOrigin, origin, deltaOrigin;
	vec3_t	oldAngles, angles, deltaAngles;

	if ( moverNum <= 0 || moverNum >= ENTITYNUM_MAX_NORMAL ) {
		VectorCopy( in, out );
		return;
	}

	cent = &cg_entities[ moverNum ];
	if ( cent->currentState.eType != ET_MOVER ) {
		VectorCopy( in, out );
		return;
	}

	BG_EvaluateTrajectory( &cent->currentState.pos, fromTime, oldOrigin );
	BG_EvaluateTrajectory( &cent->currentState.apos, fromTime, oldAngles );

	BG_EvaluateTrajectory( &cent->currentState.pos, toTime, origin );
	BG_EvaluateTrajectory( &cent->currentState.apos, toTime, angles );

	VectorSubtract( origin, oldOrigin, deltaOrigin );
	VectorSubtract( angles, oldAngles, deltaAngles );

	VectorAdd( in, deltaOrigin, out );

	// FIXME: origin change when on a rotating object
}

/*
Ghoul2 Insert Start
*/
static void LerpBoneAngleOverrides( centity_t *cent)
{
	
}
/*
Ghoul2 Insert End
*/
/*
=============================
CG_InterpolateEntityPosition
=============================
*/
static void CG_InterpolateEntityPosition( centity_t *cent ) {
	vec3_t		current, next;
	float		f;

	// it would be an internal error to find an entity that interpolates without
	// a snapshot ahead of the current one
	if ( cg.nextSnap == NULL ) {
		CG_Error( "CG_InterpoateEntityPosition: cg.nextSnap == NULL" );
	}

	f = cg.frameInterpolation;

	// this will linearize a sine or parabolic curve, but it is important
	// to not extrapolate player positions if more recent data is available
	BG_EvaluateTrajectory( &cent->currentState.pos, cg.snap->serverTime, current );
	BG_EvaluateTrajectory( &cent->nextState.pos, cg.nextSnap->serverTime, next );

	cent->lerpOrigin[0] = current[0] + f * ( next[0] - current[0] );
	cent->lerpOrigin[1] = current[1] + f * ( next[1] - current[1] );
	cent->lerpOrigin[2] = current[2] + f * ( next[2] - current[2] );

	BG_EvaluateTrajectory( &cent->currentState.apos, cg.snap->serverTime, current );
	BG_EvaluateTrajectory( &cent->nextState.apos, cg.nextSnap->serverTime, next );

	cent->lerpAngles[0] = LerpAngle( current[0], next[0], f );
	cent->lerpAngles[1] = LerpAngle( current[1], next[1], f );
	cent->lerpAngles[2] = LerpAngle( current[2], next[2], f );
/*
Ghoul2 Insert Start
*/
		// now the nasty stuff - this will interpolate all ghoul2 models bone angle overrides per model attached to this cent
//	if (cent->currentState.ghoul2.size())
	{
		LerpBoneAngleOverrides(cent);
	}
/*
Ghoul2 Insert End
*/
}

/*
===============
CG_CalcEntityLerpPositions

===============
*/
void CG_CalcEntityLerpPositions( centity_t *cent ) {

	// if this player does not want to see extrapolated players
	if ( !cg_smoothClients.integer ) {
		// make sure the clients use TR_INTERPOLATE
		if ( cent->currentState.number < MAX_CLIENTS ) {
			cent->currentState.pos.trType = TR_INTERPOLATE;
			cent->nextState.pos.trType = TR_INTERPOLATE;
		}
	}

	if ( cent->interpolate && cent->currentState.pos.trType == TR_INTERPOLATE ) {
		CG_InterpolateEntityPosition( cent );
		return;
	}

	// first see if we can interpolate between two snaps for
	// linear extrapolated clients
	if ( cent->interpolate && cent->currentState.pos.trType == TR_LINEAR_STOP &&
											cent->currentState.number < MAX_CLIENTS) {
		CG_InterpolateEntityPosition( cent );
		return;
	}

	// just use the current frame and evaluate as best we can
	BG_EvaluateTrajectory( &cent->currentState.pos, cg.time, cent->lerpOrigin );
	BG_EvaluateTrajectory( &cent->currentState.apos, cg.time, cent->lerpAngles );

	// adjust for riding a mover if it wasn't rolled into the predicted
	// player state
	if ( cent != &cg.predictedPlayerEntity ) {
		CG_AdjustPositionForMover( cent->lerpOrigin, cent->currentState.groundEntityNum, 
		cg.snap->serverTime, cg.time, cent->lerpOrigin );
	}
/*
Ghoul2 Insert Start
*/
	// now the nasty stuff - this will interpolate all ghoul2 models bone angle overrides per model attached to this cent
//	if (cent->currentState.ghoul2.size())
	{
		LerpBoneAngleOverrides(cent);
	}
/*
Ghoul2 Insert End
*/
}

/*
===============
CG_TeamBase
===============
*/
static void CG_TeamBase( centity_t *cent ) {
	refEntity_t model;
	if ( cgs.gametype == GT_CTF || cgs.gametype == GT_CTY ) {
		// show the flag base
		memset(&model, 0, sizeof(model));
		model.reType = RT_MODEL;
		VectorCopy( cent->lerpOrigin, model.lightingOrigin );
		VectorCopy( cent->lerpOrigin, model.origin );
		AnglesToAxis( cent->currentState.angles, model.axis );
		if ( cent->currentState.modelindex == TEAM_RED ) {
			model.hModel = cgs.media.redFlagBaseModel;
		}
		else if ( cent->currentState.modelindex == TEAM_BLUE ) {
			model.hModel = cgs.media.blueFlagBaseModel;
		}
		else {
			model.hModel = cgs.media.neutralFlagBaseModel;
		}
		trap_R_AddRefEntityToScene( &model );
	}
}

/*
===============
CG_AddCEntity

===============
*/
static void CG_AddCEntity( centity_t *cent ) {
	// event-only entities will have been dealt with already
	if ( cent->currentState.eType >= ET_EVENTS ) {
		return;
	}

	// calculate the current origin
	CG_CalcEntityLerpPositions( cent );

	// add automatic effects
	CG_EntityEffects( cent );
/*
Ghoul2 Insert Start
*/

	// do this before we copy the data to refEnts
	if (trap_G2_HaveWeGhoul2Models(cent->ghoul2))
	{
		trap_G2_SetGhoul2ModelIndexes(cent->ghoul2, cgs.gameModels, cgs.skins);
	}

/*
Ghoul2 Insert End
*/
	switch ( cent->currentState.eType ) {
	default:
		CG_Error( "Bad entity type: %i\n", cent->currentState.eType );
		break;
	case ET_INVISIBLE:
	case ET_PUSH_TRIGGER:
	case ET_TELEPORT_TRIGGER:
		break;
	case ET_GENERAL:
		CG_General( cent );
		break;
	case ET_PLAYER:
		CG_Player( cent );
		break;
	case ET_ITEM:
		CG_Item( cent );
		break;
	case ET_MISSILE:
		CG_Missile( cent );
		break;
	case ET_SPECIAL:
		CG_Special( cent );
		break;
	case ET_HOLOCRON:
		CG_General( cent );
		break;
	case ET_MOVER:
		CG_Mover( cent );
		break;
	case ET_BEAM:
		CG_Beam( cent );
		break;
	case ET_PORTAL:
		CG_Portal( cent );
		break;
	case ET_SPEAKER:
		CG_Speaker( cent );
		break;
	case ET_TEAM:
		CG_TeamBase( cent );
		break;
	case ET_BODY:
		CG_General( cent );
		break;
	}
}

void CG_ManualEntityRender(centity_t *cent)
{
	CG_AddCEntity(cent);
}

/*
===============
CG_AddPacketEntities

===============
*/
void CG_AddPacketEntities( void ) {
	int					num;
	centity_t			*cent;
	playerState_t		*ps;

	// set cg.frameInterpolation
	if ( cg.nextSnap ) {
		int		delta;

		delta = (cg.nextSnap->serverTime - cg.snap->serverTime);
		if ( delta == 0 ) {
			cg.frameInterpolation = 0;
		} else {
			cg.frameInterpolation = (float)( cg.time - cg.snap->serverTime ) / delta;
		}
	} else {
		cg.frameInterpolation = 0;	// actually, it should never be used, because 
									// no entities should be marked as interpolating
	}

	// the auto-rotating items will all have the same axis
	cg.autoAngles[0] = 0;
	cg.autoAngles[1] = ( cg.time & 2047 ) * 360 / 2048.0;
	cg.autoAngles[2] = 0;

	cg.autoAnglesFast[0] = 0;
	cg.autoAnglesFast[1] = ( cg.time & 1023 ) * 360 / 1024.0f;
	cg.autoAnglesFast[2] = 0;

	AnglesToAxis( cg.autoAngles, cg.autoAxis );
	AnglesToAxis( cg.autoAnglesFast, cg.autoAxisFast );

	// generate and add the entity from the playerstate
	ps = &cg.predictedPlayerState;

	//rww - update the g2 pointer BEFORE the weapons, otherwise bad things could happen
	//FIXME: These two pointers seem to differ sometimes, they shouldn't, should they?
	//the one on predictedPlayerEntity also seems to often be invalid, so it can't be
	//reliably checked and cleared.
	cg.predictedPlayerEntity.ghoul2 = cg_entities[ cg.snap->ps.clientNum].ghoul2;
	CG_CheckPlayerG2Weapons(ps, &cg.predictedPlayerEntity);
	BG_PlayerStateToEntityState( ps, &cg.predictedPlayerEntity.currentState, qfalse );
	
	// add in the Ghoul2 stuff.
	VectorCopy( cg_entities[ cg.snap->ps.clientNum].modelScale, cg.predictedPlayerEntity.modelScale);
	cg.predictedPlayerEntity.radius = cg_entities[ cg.snap->ps.clientNum].radius;

	CG_AddCEntity( &cg.predictedPlayerEntity );

	// lerp the non-predicted value for lightning gun origins
	CG_CalcEntityLerpPositions( &cg_entities[ cg.snap->ps.clientNum ] );

	// add each entity sent over by the server
	for ( num = 0 ; num < cg.snap->numEntities ; num++ ) {
		// Don't re-add ents that have been predicted.
		if (cg.snap->entities[ num ].number != cg.snap->ps.clientNum)
		{
			cent = &cg_entities[ cg.snap->entities[ num ].number ];
			CG_AddCEntity( cent );
		}
	}
}

void CG_ROFF_NotetrackCallback( centity_t *cent, const char *notetrack)
{
	int i = 0, r = 0, objectID = 0, anglesGathered = 0, posoffsetGathered = 0;
	char type[256];
	char argument[512];
	char addlArg[512];
	char errMsg[256];
	char t[64];
	int addlArgs = 0;
	vec3_t parsedAngles, parsedOffset, useAngles, useOrigin, forward, right, up;

	if (!cent || !notetrack)
	{
		return;
	}

	//notetrack = "effect effects/explosion1.efx 0+0+64 0-0-1";

	while (notetrack[i] && notetrack[i] != ' ')
	{
		type[i] = notetrack[i];
		i++;
	}

	type[i] = '\0';

	if (notetrack[i] != ' ')
	{ //didn't pass in a valid notetrack type, or forgot the argument for it
		return;
	}

	i++;

	while (notetrack[i] && notetrack[i] != ' ')
	{
		argument[r] = notetrack[i];
		r++;
		i++;
	}
	argument[r] = '\0';

	if (!r)
	{
		return;
	}

	if (notetrack[i] == ' ')
	{ //additional arguments...
		addlArgs = 1;

		i++;
		r = 0;
		while (notetrack[i])
		{
			addlArg[r] = notetrack[i];
			r++;
			i++;
		}
		addlArg[r] = '\0';
	}

	if (strcmp(type, "effect") == 0)
	{
		if (!addlArgs)
		{
			//sprintf(errMsg, "Offset position argument for 'effect' type is invalid.");
			//goto functionend;
			VectorClear(parsedOffset);
			goto defaultoffsetposition;
		}

		i = 0;

		while (posoffsetGathered < 3)
		{
			r = 0;
			while (addlArg[i] && addlArg[i] != '+' && addlArg[i] != ' ')
			{
				t[r] = addlArg[i];
				r++;
				i++;
			}
			t[r] = '\0';
			i++;
			if (!r)
			{ //failure..
				//sprintf(errMsg, "Offset position argument for 'effect' type is invalid.");
				//goto functionend;
				VectorClear(parsedOffset);
				i = 0;
				goto defaultoffsetposition;
			}
			parsedOffset[posoffsetGathered] = atof(t);
			posoffsetGathered++;
		}

		if (posoffsetGathered < 3)
		{
			Com_sprintf(errMsg, sizeof(errMsg), "Offset position argument for 'effect' type is invalid.");
			goto functionend;
		}

		i--;

		if (addlArg[i] != ' ')
		{
			addlArgs = 0;
		}

defaultoffsetposition:

		objectID = trap_FX_RegisterEffect(argument);

		if (objectID)
		{
			if (addlArgs)
			{ //if there is an additional argument for an effect it is expected to be XANGLE-YANGLE-ZANGLE
				i++;
				while (anglesGathered < 3)
				{
					r = 0;
					while (addlArg[i] && addlArg[i] != '-')
					{
						t[r] = addlArg[i];
						r++;
						i++;
					}
					t[r] = '\0';
					i++;

					if (!r)
					{ //failed to get a new part of the vector
						anglesGathered = 0;
						break;
					}

					parsedAngles[anglesGathered] = atof(t);
					anglesGathered++;
				}

				if (anglesGathered)
				{
					VectorCopy(parsedAngles, useAngles);
				}
				else
				{ //failed to parse angles from the extra argument provided..
					VectorCopy(cent->lerpAngles, useAngles);
				}
			}
			else
			{ //if no constant angles, play in direction entity is facing
				VectorCopy(cent->lerpAngles, useAngles);
			}

			AngleVectors(useAngles, forward, right, up);

			VectorCopy(cent->lerpOrigin, useOrigin);

			//forward
			useOrigin[0] += forward[0]*parsedOffset[0];
			useOrigin[1] += forward[1]*parsedOffset[0];
			useOrigin[2] += forward[2]*parsedOffset[0];

			//right
			useOrigin[0] += right[0]*parsedOffset[1];
			useOrigin[1] += right[1]*parsedOffset[1];
			useOrigin[2] += right[2]*parsedOffset[1];

			//up
			useOrigin[0] += up[0]*parsedOffset[2];
			useOrigin[1] += up[1]*parsedOffset[2];
			useOrigin[2] += up[2]*parsedOffset[2];

			trap_FX_PlayEffectID(objectID, useOrigin, useAngles);
		}
	}
	else if (strcmp(type, "sound") == 0)
	{
		objectID = trap_S_RegisterSound(argument);
		trap_S_StartSound(cent->lerpOrigin, cent->currentState.number, CHAN_BODY, objectID);
	}
	else if (strcmp(type, "loop") == 0)
	{ //handled server-side
		return;
	}
	//else if ...
	else
	{
		if (type[0])
		{
			Com_Printf("^3Warning: \"%s\" is an invalid ROFF notetrack function\n", type);
		}
		else
		{
			Com_Printf("^3Warning: Notetrack is missing function and/or arguments\n");
		}
	}

	return;

functionend:
	Com_Printf("^3Type-specific notetrack error: %s\n", errMsg);
	return;
}

