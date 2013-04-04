#include "g_local.h"
#include "bg_local.h" //Only because we use PM_SetAnim here once.
#include "w_saber.h"
#include "ai_main.h"
#include "..\ghoul2\g2.h"

extern bot_state_t *botstates[MAX_CLIENTS];
extern qboolean InFront( vec3_t spot, vec3_t from, vec3_t fromAngles, float threshHold );

int saberSpinSound = 0;
int saberOffSound = 0;
int saberOnSound = 0;
int saberHumSound = 0;

float RandFloat(float min, float max) {
	return ((rand() * (max - min)) / 32768.0F) + min;
}

void SaberUpdateSelf(gentity_t *ent)
{
	if (ent->r.ownerNum == ENTITYNUM_NONE)
	{
		ent->think = G_FreeEntity;
		ent->nextthink = level.time;
		return;
	}

	if (!g_entities[ent->r.ownerNum].inuse ||
		!g_entities[ent->r.ownerNum].client ||
		g_entities[ent->r.ownerNum].client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		ent->think = G_FreeEntity;
		ent->nextthink = level.time;
		return;
	}

	if (g_entities[ent->r.ownerNum].client->ps.saberInFlight && g_entities[ent->r.ownerNum].health > 0)
	{ //let The Master take care of us now (we'll get treated like a missile until we return)
		ent->nextthink = level.time;
		return;
	}

	if (g_entities[ent->r.ownerNum].client->ps.usingATST)
	{
		ent->r.contents = 0;
		ent->clipmask = 0;
	}
	else if (g_entities[ent->r.ownerNum].client->ps.weapon != WP_SABER ||
		(g_entities[ent->r.ownerNum].client->ps.pm_flags & PMF_FOLLOW) ||
		g_entities[ent->r.ownerNum].health < 1 ||
		g_entities[ent->r.ownerNum].client->ps.saberHolstered ||
		!g_entities[ent->r.ownerNum].client->ps.fd.forcePowerLevel[FP_SABERATTACK]/* ||
		!g_entities[ent->r.ownerNum].client->ps.fd.forcePowerLevel[FP_SABERTHROW]*/)
	{
		ent->r.contents = 0;
		ent->clipmask = 0;
	}
	else
	{
		ent->r.contents = CONTENTS_LIGHTSABER;
		ent->clipmask = MASK_PLAYERSOLID | CONTENTS_LIGHTSABER;
	}

	trap_LinkEntity(ent);

	ent->nextthink = level.time;
}

void SaberGotHit( gentity_t *self, gentity_t *other, trace_t *trace )
{
	gentity_t *own = &g_entities[self->r.ownerNum];

	if (!own || !own->client)
	{
		return;
	}

	//Do something here..? Was handling projectiles here, but instead they're now handled in their own functions.
}

void WP_SaberInitBladeData( gentity_t *ent )
{
	gentity_t *saberent;

	//We do not want the client to have any real knowledge of the entity whatsoever. It will only
	//ever be used on the server.
	saberent = G_Spawn();
	ent->client->ps.saberEntityNum = saberent->s.number;
	saberent->classname = "lightsaber";
			
	saberent->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	saberent->r.ownerNum = ent->s.number;

	saberent->clipmask = MASK_PLAYERSOLID | CONTENTS_LIGHTSABER;
	saberent->r.contents = CONTENTS_LIGHTSABER;

	VectorSet( saberent->r.mins, -8.0f, -8.0f, -8.0f );
	VectorSet( saberent->r.maxs, 8.0f, 8.0f, 8.0f );

	saberent->mass = 10;

	saberent->s.eFlags |= EF_NODRAW;
	saberent->r.svFlags |= SVF_NOCLIENT;

	saberent->touch = SaberGotHit;

	saberent->think = SaberUpdateSelf;
	saberent->nextthink = level.time + 50;

	saberSpinSound = G_SoundIndex("sound/weapons/saber/saberspin.wav");
	saberOffSound = G_SoundIndex("sound/weapons/saber/saberoffquick.wav");
	saberOnSound = G_SoundIndex("sound/weapons/saber/saberon.wav");
	saberHumSound = G_SoundIndex("sound/weapons/saber/saberhum1.wav");
}

#if 0
static void G_SwingAngles( float destination, float swingTolerance, float clampTolerance,
					float speed, float *angle, qboolean *swinging ) {
	float	swing;
	float	move;
	float	scale;

	if ( !*swinging ) {
		// see if a swing should be started
		swing = AngleSubtract( *angle, destination );
		if ( swing > swingTolerance || swing < -swingTolerance ) {
			*swinging = qtrue;
		}
	}

	if ( !*swinging ) {
		return;
	}
	
	// modify the speed depending on the delta
	// so it doesn't seem so linear
	swing = AngleSubtract( destination, *angle );
	scale = fabs( swing );
	if ( scale < swingTolerance * 0.5 ) {
		scale = 0.5;
	} else if ( scale < swingTolerance ) {
		scale = 1.0;
	} else {
		scale = 2.0;
	}

	// swing towards the destination angle
	if ( swing >= 0 ) {
		move = FRAMETIME * scale * speed;
		if ( move >= swing ) {
			move = swing;
			*swinging = qfalse;
		}
		*angle = AngleMod( *angle + move );
	} else if ( swing < 0 ) {
		move = FRAMETIME * scale * -speed;
		if ( move <= swing ) {
			move = swing;
			*swinging = qfalse;
		}
		*angle = AngleMod( *angle + move );
	}

	// clamp to no more than tolerance
	swing = AngleSubtract( destination, *angle );
	if ( swing > clampTolerance ) {
		*angle = AngleMod( destination - (clampTolerance - 1) );
	} else if ( swing < -clampTolerance ) {
		*angle = AngleMod( destination + (clampTolerance - 1) );
	}
}
#endif

//NOTE: If C` is modified this function should be modified as well (and vice versa)
void G_G2ClientSpineAngles( gentity_t *ent, vec3_t viewAngles, const vec3_t angles, vec3_t thoracicAngles, vec3_t ulAngles, vec3_t llAngles )
{
	int ang = 0;

	VectorClear(ulAngles);
	VectorClear(llAngles);

	viewAngles[YAW] = AngleDelta( ent->client->ps.viewangles[YAW], angles[YAW] );

	if ( !BG_FlippingAnim( ent->client->ps.legsAnim ) &&
		!BG_SpinningSaberAnim( ent->client->ps.legsAnim ) &&
		!BG_SpinningSaberAnim( ent->client->ps.torsoAnim ) &&
		!BG_InSpecialJump( ent->client->ps.legsAnim ) &&
		!BG_InSpecialJump( ent->client->ps.torsoAnim ) &&
		!BG_InRoll(&ent->client->ps, ent->client->ps.legsAnim) &&
		!BG_SaberInSpecial(ent->client->ps.saberMove) &&
		!BG_SaberInSpecialAttack(ent->client->ps.torsoAnim) &&
		!BG_SaberInSpecialAttack(ent->client->ps.legsAnim) )
	{
		//adjust for motion offset
		mdxaBone_t	boltMatrix;
		vec3_t		motionFwd, motionAngles;

		trap_G2API_GetBoltMatrix_NoReconstruct( ent->client->ghoul2, 0, ent->bolt_Motion, &boltMatrix, vec3_origin, ent->client->ps.origin, level.time, /*cgs.gameModels*/0, vec3_origin);
		//trap_G2API_GiveMeVectorFromMatrix( &boltMatrix, POSITIVE_X, motionFwd );
		//POSITIVE_X:
		/*
		motionFwd[0] = boltMatrix.matrix[0][0];
		motionFwd[1] = boltMatrix.matrix[1][0];
		motionFwd[2] = boltMatrix.matrix[2][0];
		*/
		

		//NEGATIVE_Y:
		motionFwd[0] = -boltMatrix.matrix[0][1];
		motionFwd[1] = -boltMatrix.matrix[1][1];
		motionFwd[2] = -boltMatrix.matrix[2][1];

		vectoangles( motionFwd, motionAngles );
		for ( ang = 0; ang < 3; ang++ )
		{
			viewAngles[ang] = AngleNormalize180( viewAngles[ang] - AngleNormalize180( motionAngles[ang] ) );
		}

		if (viewAngles[YAW] < -90)
		{
			viewAngles[YAW] += 360;
		}

		viewAngles[YAW] -= 90;
	}
	//distribute the angles differently up the spine
	//NOTE: each of these distributions must add up to 1.0f
	thoracicAngles[PITCH] = 0;//viewAngles[PITCH]*0.20f;
	llAngles[PITCH] = 0;//viewAngles[PITCH]*0.40f;
	ulAngles[PITCH] = 0;//viewAngles[PITCH]*0.40f;

	thoracicAngles[YAW] = viewAngles[YAW]*0.20f - (viewAngles[PITCH]*(viewAngles[YAW]*.020f));
	ulAngles[YAW] = viewAngles[YAW]*0.25f - (viewAngles[PITCH]*(viewAngles[YAW]*.0005f));
	llAngles[YAW] = viewAngles[YAW]*0.25f - (viewAngles[PITCH]*(viewAngles[YAW]*.0005f));

	if (thoracicAngles[YAW] > 20)
	{
		thoracicAngles[YAW] = 20;
	}
	if (ulAngles[YAW] > 20)
	{
		ulAngles[YAW] = 20;
	}
	if (llAngles[YAW] > 20)
	{
		llAngles[YAW] = 20;
	}

	thoracicAngles[ROLL] = viewAngles[ROLL]*0.20f;
	ulAngles[ROLL] = viewAngles[ROLL]*0.35f;
	llAngles[ROLL] = viewAngles[ROLL]*0.45f;

	for ( ang = 0; ang < 3; ang++ )
	{
		if (ulAngles[ang] < 0)
		{
			ulAngles[ang] += 360;
		}
	}

	//thoracic is added modified again by neckAngle calculations, so don't set it until then
//	BG_G2SetBoneAngles( cent, cent->gent, cent->gent->upperLumbarBone, ulAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.model_draw); 
//	BG_G2SetBoneAngles( cent, cent->gent, cent->gent->lowerLumbarBone, llAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.model_draw); 

//	trap_G2API_SetBoneAngles(cent->ghoul2, 0, "upper_lumbar", ulAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 
//	trap_G2API_SetBoneAngles(cent->ghoul2, 0, "lower_lumbar", llAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 
//	trap_G2API_SetBoneAngles(cent->ghoul2, 0, "thoracic", thoracicAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 
}

void G_G2PlayerAngles( gentity_t *ent, vec3_t legs[3], vec3_t legsAngles){
	vec3_t		torsoAngles, headAngles;
	float		dest;
	static	int	movementOffsets[8] = { 0, 22, 45, -22, 0, 22, -45, -22 };
	vec3_t		velocity;
	float		speed;
	int			dir;
	vec3_t		velPos, velAng;
	int			adddir = 0;
	float		dif;
	float		degrees_negative = 0;
	float		degrees_positive = 0;
	qboolean	yawing = qfalse;
	vec3_t		ulAngles, llAngles, viewAngles, angles, thoracicAngles = {0,0,0};
	//float		pitchViewAng;
	//float		yawViewAng;

	VectorCopy( ent->client->ps.viewangles, headAngles );
	headAngles[YAW] = AngleMod( headAngles[YAW] );
	VectorClear( legsAngles );
	VectorClear( torsoAngles );

	// --------- yaw -------------

	// allow yaw to drift a bit
	if ((( ent->s.legsAnim & ~ANIM_TOGGLEBIT ) != BOTH_STAND1) || 
			( ent->s.torsoAnim & ~ANIM_TOGGLEBIT ) != WeaponReadyAnim[ent->s.weapon]  ) 
	{
		// if not standing still, always point all in the same direction
		/*cent->pe.torso.yawing = qtrue;	// always center
		cent->pe.torso.pitching = qtrue;	// always center
		cent->pe.legs.yawing = qtrue;	// always center
		*/
		yawing = qtrue;
	}

	// adjust legs for movement dir
	dir = ent->s.angles2[YAW];
	if ( dir < 0 || dir > 7 ) {
		return;
	}

	torsoAngles[YAW] = headAngles[YAW] + 0.25 * movementOffsets[ dir ];

	// --------- pitch -------------

	// only show a fraction of the pitch angle in the torso
	if ( headAngles[PITCH] > 180 ) {
		dest = (-360 + headAngles[PITCH]) * 0.75;
	} else {
		dest = headAngles[PITCH] * 0.75;
	}

	//G_SwingAngles call disabled, at least for now. It isn't necessary on the server.
//	pitchViewAng = ent->client->ps.viewangles[PITCH];
//	G_SwingAngles( dest, 15, 30, 0.1, &pitchViewAng, &yawing );

	torsoAngles[PITCH] = ent->client->ps.viewangles[PITCH];

	// --------- roll -------------


	// lean towards the direction of travel
	VectorCopy( ent->s.pos.trDelta, velocity );
	speed = VectorNormalize( velocity );

	/*
	speed_desired = ent->client->ps.speed/4;

	if (!speed)
	{
		speed_dif = 0;
	}
	else
	{
		speed_dif = (speed/speed_desired);
	}

	if (speed_dif > 1)
	{
		speed_dif = 1;
	}
	else if (speed_dif < 0)
	{
		speed_dif = 0;
	}
	*/

	if ( speed ) {
		vec3_t	axis[3];
		float	side;

		speed *= 0.05;

		AnglesToAxis( legsAngles, axis );
		side = speed * DotProduct( velocity, axis[1] );
		legsAngles[ROLL] -= side;

		side = speed * DotProduct( velocity, axis[0] );
		legsAngles[PITCH] += side;
	}

	//rww - crazy velocity-based leg angle calculation
	legsAngles[YAW] = headAngles[YAW];
	velPos[0] = ent->client->ps.origin[0] + velocity[0];
	velPos[1] = ent->client->ps.origin[1] + velocity[1];
	velPos[2] = ent->client->ps.origin[2] + velocity[2];

	if (ent->client->ps.groundEntityNum == ENTITYNUM_NONE)
	{ //off the ground, no direction-based leg angles
		VectorCopy(ent->client->ps.origin, velPos);
	}

	VectorSubtract(ent->client->ps.origin, velPos, velAng);

	if (!VectorCompare(velAng, vec3_origin))
	{
		vectoangles(velAng, velAng);

		if (velAng[YAW] <= legsAngles[YAW])
		{
			degrees_negative = (legsAngles[YAW] - velAng[YAW]);
			degrees_positive = (360 - legsAngles[YAW]) + velAng[YAW];
		}
		else
		{
			degrees_negative = legsAngles[YAW] + (360 - velAng[YAW]);
			degrees_positive = (velAng[YAW] - legsAngles[YAW]);
		}

		if (degrees_negative < degrees_positive)
		{
			dif = degrees_negative;
			adddir = 0;
		}
		else
		{
			dif = degrees_positive;
			adddir = 1;
		}

		if (dif > 90)
		{
			dif = (180 - dif);
		}

		if (dif > 60)
		{
			dif = 60;
		}

		//Slight hack for when playing is running backward
		if (dir == 3 || dir == 5)
		{
			dif = -dif;
		}

		if (adddir)
		{
			legsAngles[YAW] -= dif;
		}
		else
		{
			legsAngles[YAW] += dif;
		}
	}

	//G_SwingAngles call disabled, at least for now. It isn't necessary on the server.
//	yawViewAng = ent->client->ps.viewangles[YAW];
//	G_SwingAngles( legsAngles[YAW], 40, 90, /*cg_swingSpeed.value*/ 0.3, &yawViewAng, &yawing );

	legsAngles[YAW] = ent->client->ps.viewangles[YAW];

	legsAngles[ROLL] = 0;
	torsoAngles[ROLL] = 0;

	// pull the angles back out of the hierarchial chain
	AnglesSubtract( headAngles, torsoAngles, headAngles );
	AnglesSubtract( torsoAngles, legsAngles, torsoAngles );
	AnglesToAxis( legsAngles, legs );
	// we assume that model 0 is the player model.

//	trap_G2API_SetBoneAngles(ent->client->ghoul2, 0, "upper_lumbar", torsoAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, NULL, 0, level.time); 

	VectorCopy( ent->client->ps.viewangles, viewAngles );

	if (viewAngles[PITCH] > 290)
	{ //keep the same general range as lerpAngles on the client so we can use the same spine correction
		viewAngles[PITCH] -= 360;
	}

	viewAngles[YAW] = viewAngles[ROLL] = 0;
	viewAngles[PITCH] *= 0.5;

	VectorCopy( ent->client->ps.viewangles, angles );
	angles[PITCH] = 0;

	G_G2ClientSpineAngles(ent, viewAngles, angles, thoracicAngles, ulAngles, llAngles);

	ulAngles[YAW] += torsoAngles[YAW]*0.3;
	llAngles[YAW] += torsoAngles[YAW]*0.3;
	thoracicAngles[YAW] += torsoAngles[YAW]*0.4;

	ulAngles[PITCH] = torsoAngles[PITCH]*0.3;
	llAngles[PITCH] = torsoAngles[PITCH]*0.3;
	thoracicAngles[PITCH] = torsoAngles[PITCH]*0.4;

	ulAngles[ROLL] += torsoAngles[ROLL]*0.3;
	llAngles[ROLL] += torsoAngles[ROLL]*0.3;
	thoracicAngles[ROLL] += torsoAngles[ROLL]*0.4;

	trap_G2API_SetBoneAngles(ent->client->ghoul2, 0, "upper_lumbar", ulAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, NULL, 0, level.time); 
	trap_G2API_SetBoneAngles(ent->client->ghoul2, 0, "lower_lumbar", llAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, NULL, 0, level.time); 
	trap_G2API_SetBoneAngles(ent->client->ghoul2, 0, "thoracic", thoracicAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, NULL, 0, level.time); 

	//trap_G2API_SetBoneAngles(ent->client->ghoul2, 0, "cranium", headAngles, BONE_ANGLES_POSTMULT, POSITIVE_Z, NEGATIVE_Y, POSITIVE_X, NULL, 0, level.time); 
}

qboolean SaberAttacking(gentity_t *self)
{
	/*
	if (self->client->ps.torsoAnim == self->client->ps.saberAttackSequence)
	{
		return qtrue;
	}
	*/
	if (BG_SaberInAttack(self->client->ps.saberMove))
	{
		return qtrue;
	}

	return qfalse;
}

typedef enum
{
	LOCK_FIRST = 0,
	LOCK_TOP = LOCK_FIRST,
	LOCK_DIAG_TR,
	LOCK_DIAG_TL,
	LOCK_DIAG_BR,
	LOCK_DIAG_BL,
	LOCK_R,
	LOCK_L,
	LOCK_RANDOM
} sabersLockMode_t;

#define LOCK_IDEAL_DIST_TOP 32.0f
#define LOCK_IDEAL_DIST_CIRCLE 48.0f

#define SABER_HITDAMAGE 35
void WP_SaberBlockNonRandom( gentity_t *self, vec3_t hitloc, qboolean missileBlock );

qboolean WP_SabersCheckLock2( gentity_t *attacker, gentity_t *defender, sabersLockMode_t lockMode )
{
//	animation_t *anim;
	int		attAnim, defAnim = 0;
	float	attStart = 0.5f;
	float	idealDist = 48.0f;
	vec3_t	attAngles, defAngles, defDir;
	vec3_t	newOrg;
	vec3_t	attDir;
	float	diff = 0;
	trace_t trace;
	pmove_t		pmv;

	//MATCH ANIMS
	if ( lockMode == LOCK_RANDOM )
	{
		lockMode = (sabersLockMode_t)Q_irand( (int)LOCK_FIRST, (int)(LOCK_RANDOM)-1 );
	}
	switch ( lockMode )
	{
	case LOCK_TOP:
		attAnim = BOTH_BF2LOCK;
		defAnim = BOTH_BF1LOCK;
		attStart = 0.5f;
		idealDist = LOCK_IDEAL_DIST_TOP;
		break;
	case LOCK_DIAG_TR:
		attAnim = BOTH_CCWCIRCLELOCK;
		defAnim = BOTH_CWCIRCLELOCK;
		attStart = 0.5f;
		idealDist = LOCK_IDEAL_DIST_CIRCLE;
		break;
	case LOCK_DIAG_TL:
		attAnim = BOTH_CWCIRCLELOCK;
		defAnim = BOTH_CCWCIRCLELOCK;
		attStart = 0.5f;
		idealDist = LOCK_IDEAL_DIST_CIRCLE;
		break;
	case LOCK_DIAG_BR:
		attAnim = BOTH_CWCIRCLELOCK;
		defAnim = BOTH_CCWCIRCLELOCK;
		attStart = 0.85f;
		idealDist = LOCK_IDEAL_DIST_CIRCLE;
		break;
	case LOCK_DIAG_BL:
		attAnim = BOTH_CCWCIRCLELOCK;
		defAnim = BOTH_CWCIRCLELOCK;
		attStart = 0.85f;
		idealDist = LOCK_IDEAL_DIST_CIRCLE;
		break;
	case LOCK_R:
		attAnim = BOTH_CCWCIRCLELOCK;
		defAnim = BOTH_CWCIRCLELOCK;
		attStart = 0.75f;
		idealDist = LOCK_IDEAL_DIST_CIRCLE;
		break;
	case LOCK_L:
		attAnim = BOTH_CWCIRCLELOCK;
		defAnim = BOTH_CCWCIRCLELOCK;
		attStart = 0.75f;
		idealDist = LOCK_IDEAL_DIST_CIRCLE;
		break;
	default:
		return qfalse;
		break;
	}

	memset (&pmv, 0, sizeof(pmv));
	pmv.ps = &attacker->client->ps;
	pmv.animations = bgGlobalAnimations;
	pmv.cmd = attacker->client->pers.cmd;
	pmv.trace = trap_Trace;
	pmv.pointcontents = trap_PointContents;
	pmv.gametype = g_gametype.integer;

	//This is a rare exception, you should never really call PM_ utility functions from game or cgame (despite the fact that it's technically possible)
	pm = &pmv;
	PM_SetAnim(SETANIM_BOTH, attAnim, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD, 0);
	attacker->client->ps.saberLockFrame = bgGlobalAnimations[attAnim].firstFrame+(bgGlobalAnimations[attAnim].numFrames*0.5);

	pmv.ps = &defender->client->ps;
	pmv.animations = bgGlobalAnimations;
	pmv.cmd = defender->client->pers.cmd;

	pm = &pmv;
	PM_SetAnim(SETANIM_BOTH, defAnim, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD, 0);
	defender->client->ps.saberLockFrame = bgGlobalAnimations[defAnim].firstFrame+(bgGlobalAnimations[defAnim].numFrames*0.5);

	attacker->client->ps.saberLockHits = 0;
	defender->client->ps.saberLockHits = 0;

	attacker->client->ps.saberLockAdvance = qfalse;
	defender->client->ps.saberLockAdvance = qfalse;

	VectorClear( attacker->client->ps.velocity );
	VectorClear( defender->client->ps.velocity );
	attacker->client->ps.saberLockTime = defender->client->ps.saberLockTime = level.time + 10000;
	attacker->client->ps.saberLockEnemy = defender->s.number;
	defender->client->ps.saberLockEnemy = attacker->s.number;
	attacker->client->ps.weaponTime = defender->client->ps.weaponTime = Q_irand( 1000, 3000 );//delay 1 to 3 seconds before pushing

	VectorSubtract( defender->r.currentOrigin, attacker->r.currentOrigin, defDir );
	VectorCopy( attacker->client->ps.viewangles, attAngles );
	attAngles[YAW] = vectoyaw( defDir );
	SetClientViewAngle( attacker, attAngles );
	defAngles[PITCH] = attAngles[PITCH]*-1;
	defAngles[YAW] = AngleNormalize180( attAngles[YAW] + 180);
	defAngles[ROLL] = 0;
	SetClientViewAngle( defender, defAngles );
	
	//MATCH POSITIONS
	diff = VectorNormalize( defDir ) - idealDist;//diff will be the total error in dist
	//try to move attacker half the diff towards the defender
	VectorMA( attacker->r.currentOrigin, diff*0.5f, defDir, newOrg );

	trap_Trace( &trace, attacker->r.currentOrigin, attacker->r.mins, attacker->r.maxs, newOrg, attacker->s.number, attacker->clipmask );
	if ( !trace.startsolid && !trace.allsolid )
	{
		G_SetOrigin( attacker, trace.endpos );
		trap_LinkEntity( attacker );
	}
	//now get the defender's dist and do it for him too
	VectorSubtract( attacker->r.currentOrigin, defender->r.currentOrigin, attDir );
	diff = VectorNormalize( attDir ) - idealDist;//diff will be the total error in dist
	//try to move defender all of the remaining diff towards the attacker
	VectorMA( defender->r.currentOrigin, diff, attDir, newOrg );
	trap_Trace( &trace, defender->r.currentOrigin, defender->r.mins, defender->r.maxs, newOrg, defender->s.number, defender->clipmask );
	if ( !trace.startsolid && !trace.allsolid )
	{
		G_SetOrigin( defender, trace.endpos );
		trap_LinkEntity( defender );
	}

	//DONE!
	return qtrue;
}

qboolean WP_SabersCheckLock( gentity_t *ent1, gentity_t *ent2 )
{
	float dist;
	qboolean	ent1BlockingPlayer = qfalse;
	qboolean	ent2BlockingPlayer = qfalse;

	if (!g_saberLocking.integer)
	{
		return qfalse;
	}

	if (!ent1->client || !ent2->client)
	{
		return qfalse;
	}

	if (!ent1->client->ps.duelInProgress ||
		!ent2->client->ps.duelInProgress ||
		ent1->client->ps.duelIndex != ent2->s.number ||
		ent2->client->ps.duelIndex != ent1->s.number)
	{ //only allow saber locking if two players are dueling with each other directly
		if (g_gametype.integer != GT_TOURNAMENT)
		{
			return qfalse;
		}
	}

	if ( fabs( ent1->r.currentOrigin[2]-ent2->r.currentOrigin[2] ) > 16 )
	{
		return qfalse;
	}
	if ( ent1->client->ps.groundEntityNum == ENTITYNUM_NONE ||
		ent2->client->ps.groundEntityNum == ENTITYNUM_NONE )
	{
		return qfalse;
	}
	dist = DistanceSquared(ent1->r.currentOrigin,ent2->r.currentOrigin);
	if ( dist < 64 || dist > 6400 )//( dist < 128 || dist > 2304 )
	{//between 8 and 80 from each other//was 16 and 48
		return qfalse;
	}

	if (BG_InSpecialJump(ent1->client->ps.legsAnim))
	{
		return qfalse;
	}
	if (BG_InSpecialJump(ent2->client->ps.legsAnim))
	{
		return qfalse;
	}

	if (BG_InRoll(&ent1->client->ps, ent1->client->ps.legsAnim))
	{
		return qfalse;
	}
	if (BG_InRoll(&ent2->client->ps, ent2->client->ps.legsAnim))
	{
		return qfalse;
	}

	if (ent1->client->ps.forceHandExtend != HANDEXTEND_NONE ||
		ent2->client->ps.forceHandExtend != HANDEXTEND_NONE)
	{
		return qfalse;
	}

	if ((ent1->client->ps.pm_flags & PMF_DUCKED) ||
		(ent2->client->ps.pm_flags & PMF_DUCKED))
	{
		return qfalse;
	}
//	if ( !InFOV( ent1, ent2, 40, 180 ) || !InFOV( ent2, ent1, 40, 180 ) )
//	{
//		return qfalse;
//	}

	if (!InFront( ent1->client->ps.origin, ent2->client->ps.origin, ent2->client->ps.viewangles, 0.4f ))
	{
		return qfalse;
	}
	if (!InFront( ent2->client->ps.origin, ent1->client->ps.origin, ent1->client->ps.viewangles, 0.4f ))
	{
		return qfalse;
	}

	//T to B lock
	if ( ent1->client->ps.torsoAnim == BOTH_A1_T__B_ ||
		ent1->client->ps.torsoAnim == BOTH_A2_T__B_ ||
		ent1->client->ps.torsoAnim == BOTH_A3_T__B_ ||
		ent1->client->ps.torsoAnim == BOTH_A4_T__B_ ||
		ent1->client->ps.torsoAnim == BOTH_A5_T__B_ )
	{//ent1 is attacking top-down
		/*
		if ( ent2->client->ps.torsoAnim == BOTH_P1_S1_T_ ||
			ent2->client->ps.torsoAnim == BOTH_K1_S1_T_ )
		*/
		{//ent2 is blocking at top
			return WP_SabersCheckLock2( ent1, ent2, LOCK_TOP );
		}
		//return qfalse;
	}

	if ( ent2->client->ps.torsoAnim == BOTH_A1_T__B_ ||
		ent2->client->ps.torsoAnim == BOTH_A2_T__B_ ||
		ent2->client->ps.torsoAnim == BOTH_A3_T__B_ ||
		ent2->client->ps.torsoAnim == BOTH_A4_T__B_ ||
		ent2->client->ps.torsoAnim == BOTH_A5_T__B_ )
	{//ent2 is attacking top-down
		/*
		if ( ent1->client->ps.torsoAnim == BOTH_P1_S1_T_ ||
			ent1->client->ps.torsoAnim == BOTH_K1_S1_T_ )
		*/
		{//ent1 is blocking at top
			return WP_SabersCheckLock2( ent2, ent1, LOCK_TOP );
		}
		//return qfalse;
	}

	if ( ent1->s.number == 0 &&
		ent1->client->ps.saberBlocking == BLK_WIDE && ent1->client->ps.weaponTime <= 0 )
	{
		ent1BlockingPlayer = qtrue;
	}
	if ( ent2->s.number == 0 &&
		ent2->client->ps.saberBlocking == BLK_WIDE && ent2->client->ps.weaponTime <= 0 )
	{
		ent2BlockingPlayer = qtrue;
	}

	//TR to BL lock
	if ( ent1->client->ps.torsoAnim == BOTH_A1_TR_BL ||
		ent1->client->ps.torsoAnim == BOTH_A2_TR_BL ||
		ent1->client->ps.torsoAnim == BOTH_A3_TR_BL ||
		ent1->client->ps.torsoAnim == BOTH_A4_TR_BL ||
		ent1->client->ps.torsoAnim == BOTH_A5_TR_BL )
	{//ent1 is attacking diagonally
		if ( ent2BlockingPlayer )
		{//player will block this anyway
			return WP_SabersCheckLock2( ent1, ent2, LOCK_DIAG_TR );
		}
		if ( ent2->client->ps.torsoAnim == BOTH_A1_TR_BL ||
			ent2->client->ps.torsoAnim == BOTH_A2_TR_BL ||
			ent2->client->ps.torsoAnim == BOTH_A3_TR_BL ||
			ent2->client->ps.torsoAnim == BOTH_A4_TR_BL ||
			ent2->client->ps.torsoAnim == BOTH_A5_TR_BL ||
			ent2->client->ps.torsoAnim == BOTH_P1_S1_TL )
		{//ent2 is attacking in the opposite diagonal
			return WP_SabersCheckLock2( ent1, ent2, LOCK_DIAG_TR );
		}
		if ( ent2->client->ps.torsoAnim == BOTH_A1_BR_TL ||
			ent2->client->ps.torsoAnim == BOTH_A2_BR_TL ||
			ent2->client->ps.torsoAnim == BOTH_A3_BR_TL ||
			ent2->client->ps.torsoAnim == BOTH_A4_BR_TL ||
			ent2->client->ps.torsoAnim == BOTH_A5_BR_TL ||
			ent2->client->ps.torsoAnim == BOTH_P1_S1_BL )
		{//ent2 is attacking in the opposite diagonal
			return WP_SabersCheckLock2( ent1, ent2, LOCK_DIAG_BL );
		}
		return qfalse;
	}

	if ( ent2->client->ps.torsoAnim == BOTH_A1_TR_BL ||
		ent2->client->ps.torsoAnim == BOTH_A2_TR_BL ||
		ent2->client->ps.torsoAnim == BOTH_A3_TR_BL ||
		ent2->client->ps.torsoAnim == BOTH_A4_TR_BL ||
		ent2->client->ps.torsoAnim == BOTH_A5_TR_BL )
	{//ent2 is attacking diagonally
		if ( ent1BlockingPlayer )
		{//player will block this anyway
			return WP_SabersCheckLock2( ent2, ent1, LOCK_DIAG_TR );
		}
		if ( ent1->client->ps.torsoAnim == BOTH_A1_TR_BL ||
			ent1->client->ps.torsoAnim == BOTH_A2_TR_BL ||
			ent1->client->ps.torsoAnim == BOTH_A3_TR_BL ||
			ent1->client->ps.torsoAnim == BOTH_A4_TR_BL ||
			ent1->client->ps.torsoAnim == BOTH_A5_TR_BL ||
			ent1->client->ps.torsoAnim == BOTH_P1_S1_TL )
		{//ent1 is attacking in the opposite diagonal
			return WP_SabersCheckLock2( ent2, ent1, LOCK_DIAG_TR );
		}
		if ( ent1->client->ps.torsoAnim == BOTH_A1_BR_TL ||
			ent1->client->ps.torsoAnim == BOTH_A2_BR_TL ||
			ent1->client->ps.torsoAnim == BOTH_A3_BR_TL ||
			ent1->client->ps.torsoAnim == BOTH_A4_BR_TL ||
			ent1->client->ps.torsoAnim == BOTH_A5_BR_TL ||
			ent1->client->ps.torsoAnim == BOTH_P1_S1_BL )
		{//ent1 is attacking in the opposite diagonal
			return WP_SabersCheckLock2( ent2, ent1, LOCK_DIAG_BL );
		}
		return qfalse;
	}

	//TL to BR lock
	if ( ent1->client->ps.torsoAnim == BOTH_A1_TL_BR ||
		ent1->client->ps.torsoAnim == BOTH_A2_TL_BR ||
		ent1->client->ps.torsoAnim == BOTH_A3_TL_BR ||
		ent1->client->ps.torsoAnim == BOTH_A4_TL_BR ||
		ent1->client->ps.torsoAnim == BOTH_A5_TL_BR )
	{//ent1 is attacking diagonally
		if ( ent2BlockingPlayer )
		{//player will block this anyway
			return WP_SabersCheckLock2( ent1, ent2, LOCK_DIAG_TL );
		}
		if ( ent2->client->ps.torsoAnim == BOTH_A1_TL_BR ||
			ent2->client->ps.torsoAnim == BOTH_A2_TL_BR ||
			ent2->client->ps.torsoAnim == BOTH_A3_TL_BR ||
			ent2->client->ps.torsoAnim == BOTH_A4_TL_BR ||
			ent2->client->ps.torsoAnim == BOTH_A5_TL_BR ||
			ent2->client->ps.torsoAnim == BOTH_P1_S1_TR )
		{//ent2 is attacking in the opposite diagonal
			return WP_SabersCheckLock2( ent1, ent2, LOCK_DIAG_TL );
		}
		if ( ent2->client->ps.torsoAnim == BOTH_A1_BL_TR ||
			ent2->client->ps.torsoAnim == BOTH_A2_BL_TR ||
			ent2->client->ps.torsoAnim == BOTH_A3_BL_TR ||
			ent2->client->ps.torsoAnim == BOTH_A4_BL_TR ||
			ent2->client->ps.torsoAnim == BOTH_A5_BL_TR ||
			ent2->client->ps.torsoAnim == BOTH_P1_S1_BR )
		{//ent2 is attacking in the opposite diagonal
			return WP_SabersCheckLock2( ent1, ent2, LOCK_DIAG_BR );
		}
		return qfalse;
	}

	if ( ent2->client->ps.torsoAnim == BOTH_A1_TL_BR ||
		ent2->client->ps.torsoAnim == BOTH_A2_TL_BR ||
		ent2->client->ps.torsoAnim == BOTH_A3_TL_BR ||
		ent2->client->ps.torsoAnim == BOTH_A4_TL_BR ||
		ent2->client->ps.torsoAnim == BOTH_A5_TL_BR )
	{//ent2 is attacking diagonally
		if ( ent1BlockingPlayer )
		{//player will block this anyway
			return WP_SabersCheckLock2( ent2, ent1, LOCK_DIAG_TL );
		}
		if ( ent1->client->ps.torsoAnim == BOTH_A1_TL_BR ||
			ent1->client->ps.torsoAnim == BOTH_A2_TL_BR ||
			ent1->client->ps.torsoAnim == BOTH_A3_TL_BR ||
			ent1->client->ps.torsoAnim == BOTH_A4_TL_BR ||
			ent1->client->ps.torsoAnim == BOTH_A5_TL_BR ||
			ent1->client->ps.torsoAnim == BOTH_P1_S1_TR )
		{//ent1 is attacking in the opposite diagonal
			return WP_SabersCheckLock2( ent2, ent1, LOCK_DIAG_TL );
		}
		if ( ent1->client->ps.torsoAnim == BOTH_A1_BL_TR ||
			ent1->client->ps.torsoAnim == BOTH_A2_BL_TR ||
			ent1->client->ps.torsoAnim == BOTH_A3_BL_TR ||
			ent1->client->ps.torsoAnim == BOTH_A4_BL_TR ||
			ent1->client->ps.torsoAnim == BOTH_A5_BL_TR ||
			ent1->client->ps.torsoAnim == BOTH_P1_S1_BR )
		{//ent1 is attacking in the opposite diagonal
			return WP_SabersCheckLock2( ent2, ent1, LOCK_DIAG_BR );
		}
		return qfalse;
	}
	//L to R lock
	if ( ent1->client->ps.torsoAnim == BOTH_A1__L__R ||
		ent1->client->ps.torsoAnim == BOTH_A2__L__R ||
		ent1->client->ps.torsoAnim == BOTH_A3__L__R ||
		ent1->client->ps.torsoAnim == BOTH_A4__L__R ||
		ent1->client->ps.torsoAnim == BOTH_A5__L__R )
	{//ent1 is attacking l to r
		if ( ent2BlockingPlayer )
		{//player will block this anyway
			return WP_SabersCheckLock2( ent1, ent2, LOCK_L );
		}
		if ( ent2->client->ps.torsoAnim == BOTH_A1_TL_BR ||
			ent2->client->ps.torsoAnim == BOTH_A2_TL_BR ||
			ent2->client->ps.torsoAnim == BOTH_A3_TL_BR ||
			ent2->client->ps.torsoAnim == BOTH_A4_TL_BR ||
			ent2->client->ps.torsoAnim == BOTH_A5_TL_BR ||
			ent2->client->ps.torsoAnim == BOTH_P1_S1_TR ||
			ent2->client->ps.torsoAnim == BOTH_P1_S1_BL )
		{//ent2 is attacking or blocking on the r
			return WP_SabersCheckLock2( ent1, ent2, LOCK_L );
		}
		return qfalse;
	}
	if ( ent2->client->ps.torsoAnim == BOTH_A1__L__R ||
		ent2->client->ps.torsoAnim == BOTH_A2__L__R ||
		ent2->client->ps.torsoAnim == BOTH_A3__L__R ||
		ent2->client->ps.torsoAnim == BOTH_A4__L__R ||
		ent2->client->ps.torsoAnim == BOTH_A5__L__R )
	{//ent2 is attacking l to r
		if ( ent1BlockingPlayer )
		{//player will block this anyway
			return WP_SabersCheckLock2( ent2, ent1, LOCK_L );
		}
		if ( ent1->client->ps.torsoAnim == BOTH_A1_TL_BR ||
			ent1->client->ps.torsoAnim == BOTH_A2_TL_BR ||
			ent1->client->ps.torsoAnim == BOTH_A3_TL_BR ||
			ent1->client->ps.torsoAnim == BOTH_A4_TL_BR ||
			ent1->client->ps.torsoAnim == BOTH_A5_TL_BR ||
			ent1->client->ps.torsoAnim == BOTH_P1_S1_TR ||
			ent1->client->ps.torsoAnim == BOTH_P1_S1_BL )
		{//ent1 is attacking or blocking on the r
			return WP_SabersCheckLock2( ent2, ent1, LOCK_L );
		}
		return qfalse;
	}
	//R to L lock
	if ( ent1->client->ps.torsoAnim == BOTH_A1__R__L ||
		ent1->client->ps.torsoAnim == BOTH_A2__R__L ||
		ent1->client->ps.torsoAnim == BOTH_A3__R__L ||
		ent1->client->ps.torsoAnim == BOTH_A4__R__L ||
		ent1->client->ps.torsoAnim == BOTH_A5__R__L )
	{//ent1 is attacking r to l
		if ( ent2BlockingPlayer )
		{//player will block this anyway
			return WP_SabersCheckLock2( ent1, ent2, LOCK_R );
		}
		if ( ent2->client->ps.torsoAnim == BOTH_A1_TR_BL ||
			ent2->client->ps.torsoAnim == BOTH_A2_TR_BL ||
			ent2->client->ps.torsoAnim == BOTH_A3_TR_BL ||
			ent2->client->ps.torsoAnim == BOTH_A4_TR_BL ||
			ent2->client->ps.torsoAnim == BOTH_A5_TR_BL ||
			ent2->client->ps.torsoAnim == BOTH_P1_S1_TL ||
			ent2->client->ps.torsoAnim == BOTH_P1_S1_BR )
		{//ent2 is attacking or blocking on the l
			return WP_SabersCheckLock2( ent1, ent2, LOCK_R );
		}
		return qfalse;
	}
	if ( ent2->client->ps.torsoAnim == BOTH_A1__R__L ||
		ent2->client->ps.torsoAnim == BOTH_A2__R__L ||
		ent2->client->ps.torsoAnim == BOTH_A3__R__L ||
		ent2->client->ps.torsoAnim == BOTH_A4__R__L ||
		ent2->client->ps.torsoAnim == BOTH_A5__R__L )
	{//ent2 is attacking r to l
		if ( ent1BlockingPlayer )
		{//player will block this anyway
			return WP_SabersCheckLock2( ent2, ent1, LOCK_R );
		}
		if ( ent1->client->ps.torsoAnim == BOTH_A1_TR_BL ||
			ent1->client->ps.torsoAnim == BOTH_A2_TR_BL ||
			ent1->client->ps.torsoAnim == BOTH_A3_TR_BL ||
			ent1->client->ps.torsoAnim == BOTH_A4_TR_BL ||
			ent1->client->ps.torsoAnim == BOTH_A5_TR_BL ||
			ent1->client->ps.torsoAnim == BOTH_P1_S1_TL ||
			ent1->client->ps.torsoAnim == BOTH_P1_S1_BR )
		{//ent1 is attacking or blocking on the l
			return WP_SabersCheckLock2( ent2, ent1, LOCK_R );
		}
		return qfalse;
	}
	if ( !Q_irand( 0, 10 ) )
	{
		return WP_SabersCheckLock2( ent1, ent2, LOCK_RANDOM );
	}
	return qfalse;
}

qboolean CheckSaberDamage(gentity_t *self, vec3_t saberStart, vec3_t saberEnd, qboolean doInterpolate)
{
	trace_t tr;
	vec3_t dir;
	int dmg = 0;
	int attackStr = 0;
	qboolean idleDamage = qfalse;
	qboolean didHit = qfalse;

	if (self->client->ps.saberHolstered)
	{
		return qfalse;
	}

	if (doInterpolate)
	{ //This didn't quite work out like I hoped. But it's better than nothing. Sort of.
		vec3_t oldSaberStart, oldSaberEnd, saberDif, oldSaberDif;
		int traceTests = 0;
		float trDif = 8;

		VectorCopy(self->client->lastSaberBase, oldSaberStart);
		VectorCopy(self->client->lastSaberTip, oldSaberEnd);

		VectorSubtract(saberStart, saberEnd, saberDif);
		VectorSubtract(oldSaberStart, oldSaberEnd, oldSaberDif);

		VectorNormalize(saberDif);
		VectorNormalize(oldSaberDif);

		saberEnd[0] = saberStart[0] - (saberDif[0]*trDif);
		saberEnd[1] = saberStart[1] - (saberDif[1]*trDif);
		saberEnd[2] = saberStart[2] - (saberDif[2]*trDif);

		oldSaberEnd[0] = oldSaberStart[0] - (oldSaberDif[0]*trDif);
		oldSaberEnd[1] = oldSaberStart[1] - (oldSaberDif[1]*trDif);
		oldSaberEnd[2] = oldSaberStart[2] - (oldSaberDif[2]*trDif);

		//G_TestLine(oldSaberEnd, saberEnd, 0x0000ff, 50);

		trap_Trace(&tr, saberEnd, NULL, NULL, saberStart, self->s.number, (MASK_PLAYERSOLID|CONTENTS_LIGHTSABER));

		trDif++;

		while (tr.fraction == 1.0 && traceTests < 4 && tr.entityNum >= ENTITYNUM_NONE)
		{
			VectorCopy(self->client->lastSaberBase, oldSaberStart);
			VectorCopy(self->client->lastSaberTip, oldSaberEnd);

			VectorSubtract(saberStart, saberEnd, saberDif);
			VectorSubtract(oldSaberStart, oldSaberEnd, oldSaberDif);

			VectorNormalize(saberDif);
			VectorNormalize(oldSaberDif);

			saberEnd[0] = saberStart[0] - (saberDif[0]*trDif);
			saberEnd[1] = saberStart[1] - (saberDif[1]*trDif);
			saberEnd[2] = saberStart[2] - (saberDif[2]*trDif);

			oldSaberEnd[0] = oldSaberStart[0] - (oldSaberDif[0]*trDif);
			oldSaberEnd[1] = oldSaberStart[1] - (oldSaberDif[1]*trDif);
			oldSaberEnd[2] = oldSaberStart[2] - (oldSaberDif[2]*trDif);

			//G_TestLine(oldSaberEnd, saberEnd, 0x0000ff, 50);

			trap_Trace(&tr, saberEnd, NULL, NULL, saberStart, self->s.number, (MASK_PLAYERSOLID|CONTENTS_LIGHTSABER));

			traceTests++;
			trDif += 8;
		}
	}
	else
	{
		trap_Trace(&tr, saberStart, NULL, NULL, saberEnd, self->s.number, (MASK_PLAYERSOLID|CONTENTS_LIGHTSABER));
	}

	if (SaberAttacking(self) &&
		self->client->ps.saberAttackWound < level.time)
	{ //this animation is that of the last attack movement, and so it should do full damage
		dmg = SABER_HITDAMAGE;//*self->client->ps.fd.saberAnimLevel;

		if (self->client->ps.fd.saberAnimLevel == 3)
		{
			dmg = 100;
		}
		else if (self->client->ps.fd.saberAnimLevel == 2)
		{
			dmg = 60;
		}

		attackStr = self->client->ps.fd.saberAnimLevel;
	}
	else if (self->client->ps.saberIdleWound < level.time)
	{ //just touching, do minimal damage and only check for it every 200ms (mainly to cut down on network traffic for hit events)
		dmg = 5;
		self->client->ps.saberIdleWound = level.time + 200;
		idleDamage = qtrue;
	}

	if (!dmg)
	{
		return qfalse;
	}

	if (dmg > 5 && self->client->ps.isJediMaster)
	{ //give the Jedi Master more saber attack power
		dmg *= 2;
	}

	VectorSubtract(saberEnd, saberStart, dir);
	VectorNormalize(dir);

	//rww - I'm saying || tr.startsolid here, because otherwise your saber tends to skip positions and go through
	//people, and the compensation traces start in their bbox too. Which results in the saber passing through people
	//when you visually cut right through them. Which sucks.

	if ((tr.fraction != 1 || tr.startsolid) &&
		/*(!g_entities[tr.entityNum].client || !g_entities[tr.entityNum].client->ps.usingATST) &&*/
		//g_entities[tr.entityNum].client &&
		g_entities[tr.entityNum].takedamage &&
		tr.entityNum != self->s.number)
	{
		gentity_t *te;
		qboolean unblockable = qfalse;

		if (idleDamage &&
			g_entities[tr.entityNum].client &&
			OnSameTeam(self, &g_entities[tr.entityNum]) &&
			!g_friendlySaber.integer)
		{
			return qfalse;
		}

		if (g_entities[tr.entityNum].inuse && g_entities[tr.entityNum].client &&
			g_entities[tr.entityNum].client->ps.duelInProgress &&
			g_entities[tr.entityNum].client->ps.duelIndex != self->s.number)
		{
			return qfalse;
		}

		if (g_entities[tr.entityNum].inuse && g_entities[tr.entityNum].client &&
			self->client->ps.duelInProgress &&
			self->client->ps.duelIndex != g_entities[tr.entityNum].s.number)
		{
			return qfalse;
		}

		didHit = qtrue;

		if (self->client->ps.saberMove == LS_A_BACK ||
			self->client->ps.saberMove == LS_A_BACK_CR ||
			self->client->ps.saberMove == LS_A_BACKSTAB ||
			self->client->ps.saberMove == LS_A_JUMP_T__B_)
		{
			unblockable = qtrue;
			if (self->client->ps.saberMove == LS_A_JUMP_T__B_)
			{ //do extra damage for special unblockables
				dmg += 40;
			}
			else
			{
				dmg += 20;
			}
		}

		if (g_entities[tr.entityNum].client && !unblockable && WP_SaberCanBlock(&g_entities[tr.entityNum], tr.endpos, 0, MOD_SABER, qfalse, attackStr))
		{
			te = G_TempEntity( tr.endpos, EV_SABER_BLOCK );
			VectorCopy(tr.endpos, te->s.origin);
			VectorCopy(tr.plane.normal, te->s.angles);
			te->s.eventParm = 1;

			if (dmg > 5)
			{
				if ((g_entities[tr.entityNum].client->ps.fd.forcePowerLevel[FP_SABERATTACK] - self->client->ps.fd.forcePowerLevel[FP_SABERATTACK]) > 1 &&
					Q_irand(1, 10) < 9) //used to be < 7
				{ //Just got blocked by someone with a decently higher attack level, so enter into a lock (where they have the advantage due to a higher attack lev)
					if (WP_SabersCheckLock(self, &g_entities[tr.entityNum]))
					{	
						self->client->ps.saberBlocked = BLOCKED_NONE;
						g_entities[tr.entityNum].client->ps.saberBlocked = BLOCKED_NONE;
						return didHit;
					}
				}
				else if (Q_irand(1, 10) < 3)
				{ //Just got blocked by someone with a decently higher attack level, so enter into a lock (where they have the advantage due to a higher attack lev)
					if (WP_SabersCheckLock(self, &g_entities[tr.entityNum]))
					{	
						self->client->ps.saberBlocked = BLOCKED_NONE;
						g_entities[tr.entityNum].client->ps.saberBlocked = BLOCKED_NONE;
						return didHit;
					}
				}
			}

			//our attack was blocked, so bounce back?
			if (dmg > 5)
			{
				self->client->ps.weaponTime = 0;
				self->client->ps.weaponstate = WEAPON_READY;
				self->client->ps.saberBlocked = BLOCKED_ATK_BOUNCE;

				self->client->ps.saberBlockTime = level.time + (350 - (self->client->ps.fd.forcePowerLevel[FP_SABERDEFEND]*100));//300;
			}
			self->client->ps.saberAttackWound = level.time + 300;

			if (self->client->ps.fd.saberAnimLevel >= FORCE_LEVEL_3 && dmg > 5 && g_entities[tr.entityNum].client->ps.saberMove != LS_READY && g_entities[tr.entityNum].client->ps.saberMove != LS_NONE)
			{
				g_entities[tr.entityNum].client->ps.weaponTime = 0;
				g_entities[tr.entityNum].client->ps.weaponstate = WEAPON_READY;
				g_entities[tr.entityNum].client->ps.saberBlocked = BLOCKED_ATK_BOUNCE;

				g_entities[tr.entityNum].client->ps.saberBlockTime = level.time + (350 - (g_entities[tr.entityNum].client->ps.fd.forcePowerLevel[FP_SABERDEFEND]*100));//300;
			}

			//NOTE: Actual blocking is handled in WP_SaberCanBlock
			/*
			if (dmg > 5)
			{ //play block anim on other person
				gentity_t *otherhit = &g_entities[tr.entityNum];

				if (otherhit && otherhit->client)
				{
				//	WP_SaberBlockNonRandom(otherhit, tr.endpos, qfalse);

					otherhit->client->ps.weaponTime = 0;
					otherhit->client->ps.weaponstate = WEAPON_READY;
					otherhit->client->ps.saberBlocked = BLOCKED_ATK_BOUNCE;

					self->client->ps.saberBlockTime = level.time + 300;

					otherhit->client->ps.saberAttackWound = level.time + 300;
				}
			}
			*/
		}
		else
		{
			if (g_entities[tr.entityNum].client && g_entities[tr.entityNum].client->ps.usingATST)
			{
				dmg *= 0.1;
			}

			if (g_entities[tr.entityNum].client && !g_entities[tr.entityNum].client->ps.fd.forcePowerLevel[FP_SABERATTACK])
			{ //not a "jedi", so make them suffer more
				if (dmg > 5)
				{ //don't bother increasing just for idle touch damage
					dmg *= 1.5;
				}
			}

			G_Damage(&g_entities[tr.entityNum], self, self, dir, tr.endpos, dmg, 0, MOD_SABER);

			te = G_TempEntity( tr.endpos, EV_SABER_HIT );
			VectorCopy(tr.endpos, te->s.origin);
			VectorCopy(tr.plane.normal, te->s.angles);
			
			if (!te->s.angles[0] && !te->s.angles[1] && !te->s.angles[2])
			{ //don't let it play with no direction
				te->s.angles[1] = 1;
			}

			if (g_entities[tr.entityNum].client)
			{
				te->s.eventParm = 1;
			}
			else
			{
				te->s.eventParm = 0;
			}

			self->client->ps.saberAttackWound = level.time + 100;
		}
	}
	else if ((tr.fraction != 1 || tr.startsolid) &&
		(g_entities[tr.entityNum].r.contents & CONTENTS_LIGHTSABER) &&
		g_entities[tr.entityNum].r.contents != -1)
	{ //saber clash
		gentity_t *te;
		gentity_t *otherOwner = &g_entities[g_entities[tr.entityNum].r.ownerNum];

		if (otherOwner &&
			otherOwner->inuse &&
			otherOwner->client &&
			OnSameTeam(self, otherOwner) &&
			!g_friendlySaber.integer)
		{
			return qfalse;
		}

		if (otherOwner && otherOwner->client &&
			otherOwner->client->ps.duelInProgress &&
			otherOwner->client->ps.duelIndex != self->s.number)
		{
			return qfalse;
		}

		if (otherOwner && otherOwner->client &&
			self->client->ps.duelInProgress &&
			self->client->ps.duelIndex != otherOwner->s.number)
		{
			return qfalse;
		}

		didHit = qtrue;

		te = G_TempEntity( tr.endpos, EV_SABER_BLOCK );

		VectorCopy(tr.endpos, te->s.origin);
		VectorCopy(tr.plane.normal, te->s.angles);
		te->s.eventParm = 1;

		//WP_SaberBlockNonRandom(self, tr.endpos, qfalse);

		if (otherOwner && otherOwner->client && otherOwner->client->ps.saberInFlight)
		{
			return qfalse;
		}

		if (self->client->ps.fd.saberAnimLevel < FORCE_LEVEL_3)
		{
			self->client->ps.weaponTime = 0;
			self->client->ps.weaponstate = WEAPON_READY;
			self->client->ps.saberBlocked = BLOCKED_ATK_BOUNCE;
			self->client->ps.saberBlockTime = level.time + (350 - (self->client->ps.fd.forcePowerLevel[FP_SABERDEFEND]*100));//300;

			if (otherOwner && otherOwner->client)
			{
				if (otherOwner->client->ps.weaponTime < 1 || otherOwner->client->ps.fd.saberAnimLevel < FORCE_LEVEL_3)
				{
					WP_SaberCanBlock(otherOwner, tr.endpos, 0, MOD_SABER, qfalse, 1);
				}
			}
		}
		else if (otherOwner && otherOwner->client && otherOwner->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] <= self->client->ps.fd.saberAnimLevel)
		{ //block
			if (otherOwner->client->ps.weaponTime < 1 || otherOwner->client->ps.fd.saberAnimLevel < FORCE_LEVEL_3)
			{
				if (WP_SaberCanBlock(otherOwner, tr.endpos, 0, MOD_SABER, qfalse, 1) && dmg > 5)
				{
					otherOwner->client->ps.weaponTime = 0;
					otherOwner->client->ps.weaponstate = WEAPON_READY;
					otherOwner->client->ps.saberBlocked = BLOCKED_ATK_BOUNCE;
					otherOwner->client->ps.saberBlockTime = level.time + (350 - (otherOwner->client->ps.fd.forcePowerLevel[FP_SABERDEFEND]*100));//300;
				}
			}
		}

		self->client->ps.saberAttackWound = level.time + 300;

		if (dmg > 5)
		{
			if (Q_irand(1, 10) < 9) //used to be < 7
			{
				if (WP_SabersCheckLock(self, otherOwner))
				{
					self->client->ps.saberBlocked = BLOCKED_NONE;
					otherOwner->client->ps.saberBlocked = BLOCKED_NONE;
					return didHit;
				}
			}
		}

		if (dmg > 5)
		{ //we clashed into this person's saber while attacking, so make them feel it too
			if (otherOwner && otherOwner->client && otherOwner->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] <= self->client->ps.fd.saberAnimLevel)
			{
			//	WP_SaberBlockNonRandom(otherOwner, tr.endpos, qfalse);

				otherOwner->client->ps.weaponTime = 0;
				otherOwner->client->ps.weaponstate = WEAPON_READY;
				otherOwner->client->ps.saberBlocked = BLOCKED_ATK_BOUNCE;

				self->client->ps.saberBlockTime = level.time + (350 - (self->client->ps.fd.forcePowerLevel[FP_SABERDEFEND]*100));//300;

				otherOwner->client->ps.saberAttackWound = level.time + 300;
			}
		}
	}

	return didHit;
}

#define MIN_SABER_SLICE_DISTANCE 50

#define MIN_SABER_SLICE_RETURN_DISTANCE 30

#define SABER_THROWN_HIT_DAMAGE 30
#define SABER_THROWN_RETURN_HIT_DAMAGE 5

void thrownSaberTouch (gentity_t *saberent, gentity_t *other, trace_t *trace);

qboolean CheckThrownSaberDamaged(gentity_t *saberent, gentity_t *saberOwner, gentity_t *ent, int dist, int returning)
{
	vec3_t vecsub;
	float veclen;
	gentity_t *te;

	if (saberOwner && saberOwner->client && saberOwner->client->ps.saberAttackWound > level.time)
	{
		return qfalse;
	}

	if (ent && ent->client && ent->inuse && ent->s.number != saberOwner->s.number &&
		ent->health > 0 && ent->takedamage &&
		trap_InPVS(ent->client->ps.origin, saberent->r.currentOrigin) &&
		ent->client->sess.sessionTeam != TEAM_SPECTATOR &&
		ent->client->pers.connected)
	{
		if (ent->inuse && ent->client &&
			ent->client->ps.duelInProgress &&
			ent->client->ps.duelIndex != saberOwner->s.number)
		{
			return qfalse;
		}

		if (ent->inuse && ent->client &&
			saberOwner->client->ps.duelInProgress &&
			saberOwner->client->ps.duelIndex != ent->s.number)
		{
			return qfalse;
		}

		VectorSubtract(saberent->r.currentOrigin, ent->client->ps.origin, vecsub);
		veclen = VectorLength(vecsub);

		if (veclen < dist)
		{
			trace_t tr;

			trap_Trace(&tr, saberent->r.currentOrigin, NULL, NULL, ent->client->ps.origin, saberent->s.number, MASK_SHOT);

			/*
			if (tr.startsolid || tr.allsolid)
			{
				if (!returning)
				{ //return to owner if startsolid
					thrownSaberTouch(saberent, saberent, NULL);
				}

				return qfalse;
			}
			*/

			if (tr.fraction == 1 || tr.entityNum == ent->s.number)
			{ //Slice them
				if (!saberOwner->client->ps.isJediMaster && WP_SaberCanBlock(ent, tr.endpos, 0, MOD_SABER, qfalse, 8))
				{
					te = G_TempEntity( tr.endpos, EV_SABER_BLOCK );
					VectorCopy(tr.endpos, te->s.origin);
					VectorCopy(tr.plane.normal, te->s.angles);
					if (!te->s.angles[0] && !te->s.angles[1] && !te->s.angles[2])
					{
						te->s.angles[1] = 1;
					}
					te->s.eventParm = 1;

					if (!returning)
					{ //return to owner if blocked
						thrownSaberTouch(saberent, saberent, NULL);
					}

					saberOwner->client->ps.saberAttackWound = level.time + 500;
					return qfalse;
				}
				else
				{
					vec3_t dir;

					VectorSubtract(tr.endpos, saberent->r.currentOrigin, dir);
					VectorNormalize(dir);

					if (!dir[0] && !dir[1] && !dir[2])
					{
						dir[1] = 1;
					}

					if (saberOwner->client->ps.isJediMaster)
					{ //2x damage for the Jedi Master
						G_Damage(ent, saberOwner, saberOwner, dir, tr.endpos, saberent->damage*2, 0, MOD_SABER);
					}
					else
					{
						G_Damage(ent, saberOwner, saberOwner, dir, tr.endpos, saberent->damage, 0, MOD_SABER);
					}

					te = G_TempEntity( tr.endpos, EV_SABER_HIT );
					VectorCopy(tr.endpos, te->s.origin);
					VectorCopy(tr.plane.normal, te->s.angles);
					if (!te->s.angles[0] && !te->s.angles[1] && !te->s.angles[2])
					{
						te->s.angles[1] = 1;
					}

					te->s.eventParm = 1;

					if (!returning)
					{ //return to owner if blocked
						thrownSaberTouch(saberent, saberent, NULL);
					}
				}

				saberOwner->client->ps.saberAttackWound = level.time + 500;
			}
		}
	}
	else if (ent && !ent->client && ent->inuse && ent->takedamage && ent->health > 0 && ent->s.number != saberOwner->s.number &&
		ent->s.number != saberent->s.number && trap_InPVS(ent->r.currentOrigin, saberent->r.currentOrigin))
	{
		VectorSubtract(saberent->r.currentOrigin, ent->r.currentOrigin, vecsub);
		veclen = VectorLength(vecsub);

		if (veclen < dist)
		{
			trace_t tr;

			trap_Trace(&tr, saberent->r.currentOrigin, NULL, NULL, ent->r.currentOrigin, saberent->s.number, MASK_SHOT);

			if (tr.fraction == 1 || tr.entityNum == ent->s.number)
			{
				vec3_t dir;

				VectorSubtract(tr.endpos, saberent->r.currentOrigin, dir);
				VectorNormalize(dir);

				G_Damage(ent, saberOwner, saberOwner, dir, tr.endpos, 5, 0, MOD_SABER);

				te = G_TempEntity( tr.endpos, EV_SABER_HIT );
				VectorCopy(tr.endpos, te->s.origin);
				VectorCopy(tr.plane.normal, te->s.angles);
				if (!te->s.angles[0] && !te->s.angles[1] && !te->s.angles[2])
				{
					te->s.angles[1] = 1;
				}

				te->s.eventParm = 1;

				if (!returning)
				{ //return to owner if blocked
					thrownSaberTouch(saberent, saberent, NULL);
				}

				saberOwner->client->ps.saberAttackWound = level.time + 500;
			}
		}
	}

	return qtrue;
}

void saberCheckRadiusDamage(gentity_t *saberent, int returning)
{ //we're going to cheat and damage players within the saber's radius, just for the sake of doing things more "efficiently"
	int i = 0;
	int dist = 0;
	gentity_t *ent;
	gentity_t *saberOwner = &g_entities[saberent->r.ownerNum];

	if (returning && returning != 2)
	{
		dist = MIN_SABER_SLICE_RETURN_DISTANCE;
	}
	else
	{
		dist = MIN_SABER_SLICE_DISTANCE;
	}

	if (!saberOwner || !saberOwner->client)
	{
		return;
	}

	if (saberOwner->client->ps.saberAttackWound > level.time)
	{
		return;
	}

	while (i < /*MAX_CLIENTS*/MAX_GENTITIES)
	{
		ent = &g_entities[i];

		CheckThrownSaberDamaged(saberent, saberOwner, ent, dist, returning);

		i++;
	}
}

//#define THROWN_SABER_COMP

void saberMoveBack( gentity_t *ent, qboolean goingBack ) 
{
	vec3_t		origin, oldOrg;

	ent->s.pos.trType = TR_LINEAR;

	VectorCopy( ent->r.currentOrigin, oldOrg );
	// get current position
	BG_EvaluateTrajectory( &ent->s.pos, level.time, origin );
	//Get current angles?
	BG_EvaluateTrajectory( &ent->s.apos, level.time, ent->r.currentAngles );

	//compensation test code..
#ifdef THROWN_SABER_COMP
	if (!goingBack)
	{ //acts as a fallback in case touch code fails, keeps saber from going through things between predictions
		float originalLength = 0;
		int iCompensationLength = 32;
		trace_t tr;
		vec3_t mins, maxs;
		vec3_t calcComp, compensatedOrigin;
		VectorSet( mins, -24.0f, -24.0f, -8.0f );
		VectorSet( maxs, 24.0f, 24.0f, 8.0f );

		VectorSubtract(origin, oldOrg, calcComp);
		originalLength = VectorLength(calcComp);

		VectorNormalize(calcComp);

		compensatedOrigin[0] = oldOrg[0] + calcComp[0]*(originalLength+iCompensationLength);		
		compensatedOrigin[1] = oldOrg[1] + calcComp[1]*(originalLength+iCompensationLength);
		compensatedOrigin[2] = oldOrg[2] + calcComp[2]*(originalLength+iCompensationLength);

		trap_Trace(&tr, oldOrg, mins, maxs, compensatedOrigin, ent->r.ownerNum, MASK_PLAYERSOLID);

		if ((tr.fraction != 1 || tr.startsolid || tr.allsolid) && tr.entityNum != ent->r.ownerNum)
		{
			VectorClear(ent->s.pos.trDelta);

			//Unfortunately doing this would defeat the purpose of the compensation. We will have to settle for a jerk on the client.
			//VectorCopy( origin, ent->r.currentOrigin );

			CheckThrownSaberDamaged(ent, &g_entities[ent->r.ownerNum], &g_entities[tr.entityNum], 256, 0);

			tr.startsolid = 0;
			thrownSaberTouch(ent, &g_entities[tr.entityNum], &tr);
			return;
		}
	}
#endif

	VectorCopy( origin, ent->r.currentOrigin );
}

void SaberBounceSound( gentity_t *self, gentity_t *other, trace_t *trace )
{
	VectorCopy(self->r.currentAngles, self->s.apos.trBase);
	self->s.apos.trBase[PITCH] = 90;
}

void DeadSaberThink(gentity_t *saberent)
{
	if (saberent->speed < level.time)
	{
		saberent->think = G_FreeEntity;
		saberent->nextthink = level.time;
		return;
	}

	G_RunObject(saberent);
}

void MakeDeadSaber(gentity_t *ent)
{	//spawn a "dead" saber entity here so it looks like the saber fell out of the air.
	//This entity will remove itself after a very short time period.
	vec3_t startorg;
	vec3_t startang;
	gentity_t *saberent;
	
	if (g_gametype.integer == GT_JEDIMASTER)
	{ //never spawn a dead saber in JM, because the only saber on the level is really a world object
		G_Sound(ent, CHAN_AUTO, saberOffSound);
		return;
	}

	saberent = G_Spawn();

	VectorCopy(ent->r.currentOrigin, startorg);
	VectorCopy(ent->r.currentAngles, startang);

	saberent->classname = "deadsaber";
			
	saberent->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	saberent->r.ownerNum = ent->s.number;

	saberent->clipmask = MASK_PLAYERSOLID;
	saberent->r.contents = CONTENTS_TRIGGER;//0;

	VectorSet( saberent->r.mins, -3.0f, -3.0f, -3.0f );
	VectorSet( saberent->r.maxs, 3.0f, 3.0f, 3.0f );

	//saberent->mass = 10;

	saberent->touch = SaberBounceSound;

	saberent->think = DeadSaberThink;
	saberent->nextthink = level.time;

	VectorCopy(startorg, saberent->s.pos.trBase);
	VectorCopy(startang, saberent->s.apos.trBase);

	VectorCopy(startorg, saberent->s.origin);
	VectorCopy(startang, saberent->s.angles);

	VectorCopy(startorg, saberent->r.currentOrigin);
	VectorCopy(startang, saberent->r.currentAngles);

	saberent->s.apos.trType = TR_GRAVITY;
	saberent->s.apos.trDelta[0] = Q_irand(200, 800);
	saberent->s.apos.trDelta[1] = Q_irand(200, 800);
	saberent->s.apos.trDelta[2] = Q_irand(200, 800);
	saberent->s.apos.trTime = level.time-50;

	saberent->s.pos.trType = TR_GRAVITY;
	saberent->s.pos.trTime = level.time-50;
	saberent->s.eFlags = EF_BOUNCE_HALF;
	saberent->s.modelindex = G_ModelIndex("models/weapons2/saber/saber_w.glm");
	saberent->s.modelGhoul2 = 1;
	saberent->s.g2radius = 20;

	saberent->s.eType = ET_MISSILE;
	saberent->s.weapon = WP_SABER;

	saberent->speed = level.time + 4000;

	saberent->bounceCount = 12;

	//fall off in the direction the real saber was headed
	VectorCopy(ent->s.pos.trDelta, saberent->s.pos.trDelta);

	saberMoveBack(saberent, qtrue);
	saberent->s.pos.trType = TR_GRAVITY;

	trap_LinkEntity(saberent);	
}

void saberBackToOwner(gentity_t *saberent)
{
	gentity_t *saberOwner = &g_entities[saberent->r.ownerNum];
	vec3_t dir;
	float ownerLen;

	if (saberent->r.ownerNum == ENTITYNUM_NONE)
	{
		MakeDeadSaber(saberent);

		saberent->think = G_FreeEntity;
		saberent->nextthink = level.time;
		return;
	}

	if (!g_entities[saberent->r.ownerNum].inuse ||
		!g_entities[saberent->r.ownerNum].client ||
		g_entities[saberent->r.ownerNum].client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		MakeDeadSaber(saberent);

		saberent->think = G_FreeEntity;
		saberent->nextthink = level.time;
		return;
	}

	if (g_entities[saberent->r.ownerNum].health < 1 || !g_entities[saberent->r.ownerNum].client->ps.fd.forcePowerLevel[FP_SABERATTACK] || !g_entities[saberent->r.ownerNum].client->ps.fd.forcePowerLevel[FP_SABERTHROW])
	{ //He's dead, just go back to our normal saber status
		saberent->touch = SaberGotHit;
		saberent->think = SaberUpdateSelf;
		saberent->nextthink = level.time;

		MakeDeadSaber(saberent);

		saberent->r.svFlags |= (SVF_NOCLIENT);
		saberent->r.contents = CONTENTS_LIGHTSABER;
		VectorSet( saberent->r.mins, -8.0f, -8.0f, -8.0f );
		VectorSet( saberent->r.maxs, 8.0f, 8.0f, 8.0f );
		saberent->s.loopSound = 0;

		g_entities[saberent->r.ownerNum].client->ps.saberInFlight = qfalse;
		g_entities[saberent->r.ownerNum].client->ps.saberThrowDelay = level.time + 500;
		g_entities[saberent->r.ownerNum].client->ps.saberCanThrow = qfalse;

		return;
	}

	saberent->r.contents = CONTENTS_LIGHTSABER;

	//saberent->s.apos.trDelta[1] = 0;

	VectorSubtract(saberent->pos1, saberent->r.currentOrigin, dir);

	ownerLen = VectorLength(dir);

	if (saberent->speed < level.time)
	{
		VectorNormalize(dir);

		saberMoveBack(saberent, qtrue);
		VectorCopy(saberent->r.currentOrigin, saberent->s.pos.trBase);

		if (g_entities[saberent->r.ownerNum].client->ps.fd.forcePowerLevel[FP_SABERTHROW] >= FORCE_LEVEL_3)
		{ //allow players with high saber throw rank to control the return speed of the saber
			if (g_entities[saberent->r.ownerNum].client->buttons & BUTTON_ATTACK)
			{
				VectorScale(dir, 1200, saberent->s.pos.trDelta );
				saberent->speed = level.time + 50;
			}
			else
			{
				VectorScale(dir, 700, saberent->s.pos.trDelta );
				saberent->speed = level.time + 200;
			}
		}
		else
		{
			VectorScale(dir, 700, saberent->s.pos.trDelta );
			saberent->speed = level.time + 200;
		}
		saberent->s.pos.trTime = level.time;
	}

	if (ownerLen <= 512)
	{
		saberent->s.saberInFlight = qfalse;
		saberent->s.loopSound = saberHumSound;
	}

	if (ownerLen <= 32)
	{
		saberOwner->client->ps.saberInFlight = qfalse;
		saberOwner->client->ps.saberCanThrow = qfalse;
		saberOwner->client->ps.saberThrowDelay = level.time + 300;

		saberent->touch = SaberGotHit;

		saberent->think = SaberUpdateSelf;
		saberent->nextthink = level.time + 50;

		return;
	}

	if (!saberent->s.saberInFlight)
	{
		saberCheckRadiusDamage(saberent, 1);
	}
	else
	{
		saberCheckRadiusDamage(saberent, 2);
	}

	saberMoveBack(saberent, qtrue);
	//G_RunObject(saberent);

	saberent->nextthink = level.time;
}

void saberFirstThrown(gentity_t *saberent);

void thrownSaberTouch (gentity_t *saberent, gentity_t *other, trace_t *trace)
{
	gentity_t *hitEnt = other;

	if (other && other->s.number == saberent->r.ownerNum)
	{
		return;
	}
	VectorClear(saberent->s.pos.trDelta);
	saberent->s.pos.trTime = level.time;

	saberent->s.apos.trType = TR_LINEAR;
	saberent->s.apos.trDelta[0] = 0;
	saberent->s.apos.trDelta[1] = 800;
	saberent->s.apos.trDelta[2] = 0;

	VectorCopy(saberent->r.currentOrigin, saberent->s.pos.trBase);

	//saberent->damage = SABER_THROWN_RETURN_HIT_DAMAGE;

	saberent->think = saberBackToOwner;
	saberent->nextthink = level.time;

	//saberCheckRadiusDamage(saberent, 2);
	if (other && other->r.ownerNum < MAX_CLIENTS &&
		(other->r.contents & CONTENTS_LIGHTSABER) &&
		g_entities[other->r.ownerNum].client &&
		g_entities[other->r.ownerNum].inuse)
	{
		hitEnt = &g_entities[other->r.ownerNum];
	}

	CheckThrownSaberDamaged(saberent, &g_entities[saberent->r.ownerNum], hitEnt, 256, 0);

	saberent->speed = 0;
}

#define SABER_MAX_THROW_DISTANCE 700

void saberFirstThrown(gentity_t *saberent)
{
	vec3_t		vSub;
	float		vLen;
	gentity_t	*saberOwn = &g_entities[saberent->r.ownerNum];

	if (saberent->r.ownerNum == ENTITYNUM_NONE)
	{
		MakeDeadSaber(saberent);

		saberent->think = G_FreeEntity;
		saberent->nextthink = level.time;
		return;
	}

	if (!saberOwn ||
		!saberOwn->inuse ||
		!saberOwn->client ||
		saberOwn->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		MakeDeadSaber(saberent);

		saberent->think = G_FreeEntity;
		saberent->nextthink = level.time;
		return;
	}

	if (saberOwn->health < 1 || !saberOwn->client->ps.fd.forcePowerLevel[FP_SABERATTACK] || !saberOwn->client->ps.fd.forcePowerLevel[FP_SABERTHROW])
	{ //He's dead, just go back to our normal saber status
		saberent->touch = SaberGotHit;
		saberent->think = SaberUpdateSelf;
		saberent->nextthink = level.time;

		MakeDeadSaber(saberent);

		saberent->r.svFlags |= (SVF_NOCLIENT);
		saberent->r.contents = CONTENTS_LIGHTSABER;
		VectorSet( saberent->r.mins, -8.0f, -8.0f, -8.0f );
		VectorSet( saberent->r.maxs, 8.0f, 8.0f, 8.0f );
		saberent->s.loopSound = 0;

		saberOwn->client->ps.saberInFlight = qfalse;
		saberOwn->client->ps.saberThrowDelay = level.time + 500;
		saberOwn->client->ps.saberCanThrow = qfalse;

		return;
	}

	if ((level.time - saberOwn->client->ps.saberDidThrowTime) > 500)
	{
		if (!(saberOwn->client->buttons & BUTTON_ALT_ATTACK))
		{ //If owner releases altattack 500ms or later after throwing saber, it autoreturns
			thrownSaberTouch(saberent, saberent, NULL);
			goto runMin;
		}
		else if ((level.time - saberOwn->client->ps.saberDidThrowTime) > 6000)
		{ //if it's out longer than 6 seconds, return it
			thrownSaberTouch(saberent, saberent, NULL);
			goto runMin;
		}
	}

	if (BG_HasYsalamiri(g_gametype.integer, &saberOwn->client->ps))
	{
		thrownSaberTouch(saberent, saberent, NULL);
		goto runMin;
	}
	
	if (!BG_CanUseFPNow(g_gametype.integer, &saberOwn->client->ps, level.time, FP_SABERTHROW))
	{
		thrownSaberTouch(saberent, saberent, NULL);
		goto runMin;
	}

	VectorSubtract(saberOwn->client->ps.origin, saberent->r.currentOrigin, vSub);
	vLen = VectorLength(vSub);

	if (vLen >= (SABER_MAX_THROW_DISTANCE*saberOwn->client->ps.fd.forcePowerLevel[FP_SABERTHROW]))
	{
		thrownSaberTouch(saberent, saberent, NULL);
		goto runMin;
	}

	if (saberOwn->client->ps.fd.forcePowerLevel[FP_SABERTHROW] >= FORCE_LEVEL_2 &&
		saberent->speed < level.time)
	{ //if owner is rank 3 in saber throwing, the saber goes where he points
		vec3_t fwd, traceFrom, traceTo, dir;
		trace_t tr;

		AngleVectors(saberOwn->client->ps.viewangles, fwd, 0, 0);

		VectorCopy(saberOwn->client->ps.origin, traceFrom);
		traceFrom[2] += saberOwn->client->ps.viewheight;

		VectorCopy(traceFrom, traceTo);
		traceTo[0] += fwd[0]*4096;
		traceTo[1] += fwd[1]*4096;
		traceTo[2] += fwd[2]*4096;

		saberMoveBack(saberent, qfalse);
		VectorCopy(saberent->r.currentOrigin, saberent->s.pos.trBase);

		if (saberOwn->client->ps.fd.forcePowerLevel[FP_SABERTHROW] >= FORCE_LEVEL_3)
		{ //if highest saber throw rank, we can direct the saber toward players directly by looking at them
			trap_Trace(&tr, traceFrom, NULL, NULL, traceTo, saberOwn->s.number, MASK_PLAYERSOLID);
		}
		else
		{
			trap_Trace(&tr, traceFrom, NULL, NULL, traceTo, saberOwn->s.number, MASK_SOLID);
		}

		//G_TestLine(traceFrom, tr.endpos, 0x000000ff, 100);

		//if (tr.fraction != 1)
		{
			VectorSubtract(tr.endpos, saberent->r.currentOrigin, dir);

			VectorNormalize(dir);

			VectorScale(dir, 500, saberent->s.pos.trDelta );
			saberent->s.pos.trTime = level.time;

			if (saberOwn->client->ps.fd.forcePowerLevel[FP_SABERTHROW] >= FORCE_LEVEL_3)
			{ //we'll treat them to a quicker update rate if their throw rank is high enough
				saberent->speed = level.time + 100;
			}
			else
			{
				saberent->speed = level.time + 400;
			}
		}
	}

runMin:

	saberCheckRadiusDamage(saberent, 0);
	G_RunObject(saberent);
}

void WP_SaberPositionUpdate( gentity_t *self, usercmd_t *ucmd )
{ //rww - keep the saber position as updated as possible on the server so that we can try to do realistic-looking contact stuff
	mdxaBone_t	boltMatrix;
	vec3_t properAngles, properOrigin;
	vec3_t boltAngles, boltOrigin;
	vec3_t end;
	vec3_t legAxis[3];
	vec3_t addVel;
	vec3_t rawAngles;
	float fVSpeed = 0;
	int f;
	int torsoAnim;
	int legsAnim;
	int returnAfterUpdate = 0;
	float animSpeedScale = 1;
	qboolean setTorso = qfalse;

	if (self && self->inuse && self->client)
	{
		if (self->client->saberCycleQueue)
		{
			self->client->ps.fd.saberDrawAnimLevel = self->client->saberCycleQueue;
		}
		else
		{
			self->client->ps.fd.saberDrawAnimLevel = self->client->ps.fd.saberAnimLevel;
		}
	}

	if (self &&
		self->inuse &&
		self->client &&
		self->client->saberCycleQueue &&
		(self->client->ps.weaponTime <= 0 || self->health < 1))
	{ //we cycled attack levels while we were busy, so update now that we aren't (even if that means we're dead)
		self->client->ps.fd.saberAnimLevel = self->client->saberCycleQueue;
		self->client->saberCycleQueue = 0;
	}

	if (!self ||
		!self->client ||
		!self->client->ghoul2 ||
		!g2SaberInstance ||
		self->health < 1)
	{
		return;
	}

	if (self->client->ps.usingATST)
	{ //we don't update the server's G2 instance in the case of ATST use, so..
		return;
	}

	if (self->client->ps.weapon != WP_SABER ||
		self->client->ps.weaponstate == WEAPON_RAISING ||
		self->client->ps.weaponstate == WEAPON_DROPPING)
	{
		returnAfterUpdate = 1;
		//return;
	}

	if (self->client->ps.saberThrowDelay < level.time)
	{
		self->client->ps.saberCanThrow = qtrue;
	}

	if (self->client->ps.fd.forcePowersActive & (1 << FP_RAGE))
	{
		animSpeedScale = 2;
	}
	
	torsoAnim = (self->client->ps.torsoAnim & ~ANIM_TOGGLEBIT );
	legsAnim = (self->client->ps.legsAnim & ~ANIM_TOGGLEBIT );

	VectorCopy(self->client->ps.origin, properOrigin);
	VectorCopy(self->client->ps.viewangles, properAngles);

	//try to predict the origin based on velocity so it's more like what the client is seeing
	VectorCopy(self->client->ps.velocity, addVel);
	VectorNormalize(addVel);

	if (self->client->ps.velocity[0] < 0)
	{
		fVSpeed += (-self->client->ps.velocity[0]);
	}
	else
	{
		fVSpeed += self->client->ps.velocity[0];
	}
	if (self->client->ps.velocity[1] < 0)
	{
		fVSpeed += (-self->client->ps.velocity[1]);
	}
	else
	{
		fVSpeed += self->client->ps.velocity[1];
	}
	if (self->client->ps.velocity[2] < 0)
	{
		fVSpeed += (-self->client->ps.velocity[2]);
	}
	else
	{
		fVSpeed += self->client->ps.velocity[2];
	}

	fVSpeed *= 0.08;

	properOrigin[0] += addVel[0]*fVSpeed;
	properOrigin[1] += addVel[1]*fVSpeed;
	properOrigin[2] += addVel[2]*fVSpeed;

	properAngles[0] = 0;
	properAngles[1] = self->client->ps.viewangles[YAW];
	properAngles[2] = 0;

	AnglesToAxis( properAngles, legAxis );
	G_G2PlayerAngles( self, legAxis, properAngles );

	if (returnAfterUpdate)
	{ //We don't even need to do GetBoltMatrix if we're only in here to keep the g2 server instance in sync
		goto finalUpdate;
	}

	trap_G2API_GetBoltMatrix(self->client->ghoul2, 1, 0, &boltMatrix, properAngles, properOrigin, level.time, NULL, vec3_origin);

	boltOrigin[0] = boltMatrix.matrix[0][3];
	boltOrigin[1] = boltMatrix.matrix[1][3];
	boltOrigin[2] = boltMatrix.matrix[2][3];

	boltAngles[0] = -boltMatrix.matrix[0][1];
	boltAngles[1] = -boltMatrix.matrix[1][1];
	boltAngles[2] = -boltMatrix.matrix[2][1];

	VectorCopy(boltAngles, rawAngles);

	VectorMA( boltOrigin, 40, boltAngles, end );

	if (self->client->ps.saberEntityNum)
	{
		gentity_t *mySaber = &g_entities[self->client->ps.saberEntityNum];

		if (mySaber && (mySaber->r.contents & CONTENTS_LIGHTSABER) && !self->client->ps.saberInFlight)
		{ //place it roughly in the middle of the saber..
			VectorMA( boltOrigin, 20, boltAngles, mySaber->r.currentOrigin );

			if (self->client->ps.dualBlade)
			{
				VectorCopy(boltOrigin, mySaber->r.currentOrigin);
			}
			//VectorCopy(/*boltOrigin*/end, mySaber->r.currentOrigin);
		}
	}

	boltAngles[YAW] = self->client->ps.viewangles[YAW];

	//G_TestLine(boltOrigin, end, 0x000000ff, 50);

	if (self->client->ps.saberInFlight)
	{ //do the thrown-saber stuff
		gentity_t *saberent = &g_entities[self->client->ps.saberEntityNum];

		if (saberent)
		{
			if (!self->client->ps.saberEntityState)
			{
				vec3_t startorg, startang, dir;

				VectorCopy(boltOrigin, saberent->r.currentOrigin);

				VectorCopy(boltOrigin, startorg);
				VectorCopy(boltAngles, startang);

				//startang[0] = 90;
				//Instead of this we'll sort of fake it and slowly tilt it down on the client via
				//a perframe method

				saberent->r.svFlags &= ~(SVF_NOCLIENT);
				VectorCopy(startorg, saberent->s.pos.trBase);
				VectorCopy(startang, saberent->s.apos.trBase);

				VectorCopy(startorg, saberent->s.origin);
				VectorCopy(startang, saberent->s.angles);

				saberent->s.saberInFlight = qtrue;

				saberent->s.apos.trType = TR_LINEAR;
				saberent->s.apos.trDelta[0] = 0;
				saberent->s.apos.trDelta[1] = 800;
				saberent->s.apos.trDelta[2] = 0;

				saberent->s.pos.trType = TR_LINEAR;
				saberent->s.eType = ET_GENERAL;
				saberent->s.eFlags = 0;
				saberent->s.modelindex = G_ModelIndex("models/weapons2/saber/saber_w.glm");
				saberent->s.modelGhoul2 = 127;

				saberent->parent = self;

				self->client->ps.saberEntityState = 1;

				//Projectile stuff:
				AngleVectors(self->client->ps.viewangles, dir, NULL, NULL);

				saberent->nextthink = level.time + FRAMETIME;
				saberent->think = saberFirstThrown;//G_RunObject;

				saberent->damage = SABER_THROWN_HIT_DAMAGE;
				saberent->methodOfDeath = MOD_SABER;
				saberent->splashMethodOfDeath = MOD_SABER;
				saberent->s.solid = 2;
				saberent->r.contents = CONTENTS_LIGHTSABER;

				VectorSet( saberent->r.mins, -24.0f, -24.0f, -8.0f );
				VectorSet( saberent->r.maxs, 24.0f, 24.0f, 8.0f );

				saberent->s.genericenemyindex = self->s.number+1024;

				saberent->touch = thrownSaberTouch;

				saberent->s.weapon = WP_SABER;

				VectorScale(dir, 400, saberent->s.pos.trDelta );
				saberent->s.pos.trTime = level.time;

				saberent->s.loopSound = saberSpinSound;

				self->client->ps.saberDidThrowTime = level.time;

				self->client->dangerTime = level.time;
				self->client->ps.eFlags &= ~EF_INVULNERABLE;
				self->client->invulnerableTimer = 0;

				trap_LinkEntity(saberent);
			}
			else
			{
				VectorCopy(boltOrigin, saberent->pos1);
				trap_LinkEntity(saberent);
			}
		}
	}
	else if (!self->client->ps.saberHolstered)
	{
		gentity_t *saberent = &g_entities[self->client->ps.saberEntityNum];

		if (saberent)
		{
			saberent->r.svFlags |= (SVF_NOCLIENT);
			saberent->r.contents = CONTENTS_LIGHTSABER;
			VectorSet( saberent->r.mins, -8.0f, -8.0f, -8.0f );
			VectorSet( saberent->r.maxs, 8.0f, 8.0f, 8.0f );
			saberent->s.loopSound = 0;
		}

		if (self->client->ps.saberLockTime > level.time && self->client->ps.saberEntityNum)
		{
			if (self->client->ps.saberIdleWound < level.time)
			{
				gentity_t *te;
				vec3_t dir;
				te = G_TempEntity( g_entities[self->client->ps.saberEntityNum].r.currentOrigin, EV_SABER_BLOCK );
				VectorSet( dir, 0, 1, 0 );
				VectorCopy(g_entities[self->client->ps.saberEntityNum].r.currentOrigin, te->s.origin);
				VectorCopy(dir, te->s.angles);
				te->s.eventParm = 1;

				self->client->ps.saberIdleWound = level.time + Q_irand(400, 600);
			}

			VectorCopy(boltOrigin, self->client->lastSaberBase);
			VectorCopy(end, self->client->lastSaberTip);
			self->client->hasCurrentPosition = qtrue;

			self->client->ps.saberBlocked = BLOCKED_NONE;

			goto finalUpdate;
		}

		if (self->client->ps.dualBlade)
		{
			self->client->ps.saberIdleWound = 0;
			self->client->ps.saberAttackWound = 0;
		}

		if (self->client->hasCurrentPosition && g_saberInterpolate.integer)
		{
			if (!CheckSaberDamage(self, boltOrigin, end, qfalse))
			{
				CheckSaberDamage(self, boltOrigin, end, qtrue);
			}
		}
		else
		{
			CheckSaberDamage(self, boltOrigin, end, qfalse);
		}

		if (self->client->ps.dualBlade)
		{
			vec3_t otherOrg, otherEnd;

			VectorMA( boltOrigin, -12, rawAngles, otherOrg );
			VectorMA( otherOrg, -40, rawAngles, otherEnd );

			self->client->ps.saberIdleWound = 0;
			self->client->ps.saberAttackWound = 0;

			CheckSaberDamage(self, otherOrg, otherEnd, qfalse);
		}

		VectorCopy(boltOrigin, self->client->lastSaberBase);
		VectorCopy(end, self->client->lastSaberTip);
		self->client->hasCurrentPosition = qtrue;

		self->client->ps.saberEntityState = 0;
	}
finalUpdate:
	if (self->client->ps.saberLockFrame)
	{
		trap_G2API_SetBoneAnim(self->client->ghoul2, 0, "model_root", self->client->ps.saberLockFrame, self->client->ps.saberLockFrame+1, BONE_ANIM_OVERRIDE_FREEZE|BONE_ANIM_BLEND, animSpeedScale, level.time, -1, 150);
		trap_G2API_SetBoneAnim(self->client->ghoul2, 0, "upper_lumbar", self->client->ps.saberLockFrame, self->client->ps.saberLockFrame+1, BONE_ANIM_OVERRIDE_FREEZE|BONE_ANIM_BLEND, animSpeedScale, level.time, -1, 150);
		trap_G2API_SetBoneAnim(self->client->ghoul2, 0, "Motion", self->client->ps.saberLockFrame, self->client->ps.saberLockFrame+1, BONE_ANIM_OVERRIDE_FREEZE|BONE_ANIM_BLEND, animSpeedScale, level.time, -1, 150);
		return;
	}

	if (self->client->ps.legsAnimExecute != legsAnim)
	{
		float animSpeed = 50.0f / bgGlobalAnimations[legsAnim].frameLerp;
		int aFlags;
		animSpeedScale = (animSpeed *= animSpeedScale);

		if (bgGlobalAnimations[legsAnim].loopFrames != -1)
		{
			aFlags = BONE_ANIM_OVERRIDE_LOOP;
		}
		else
		{
			aFlags = BONE_ANIM_OVERRIDE_FREEZE;
		}

		aFlags |= BONE_ANIM_BLEND; //since client defaults to blend. Not sure if this will make much difference if any on client position, but it's here just for the sake of matching them.

		trap_G2API_SetBoneAnim(self->client->ghoul2, 0, "model_root", bgGlobalAnimations[legsAnim].firstFrame, bgGlobalAnimations[legsAnim].firstFrame+bgGlobalAnimations[legsAnim].numFrames, aFlags, animSpeedScale, level.time, -1, 150);
		self->client->ps.legsAnimExecute = legsAnim;
	}
	if (self->client->ps.torsoAnimExecute != torsoAnim)
	{
		int initialFrame;
		int aFlags = 0;
		float animSpeed = 0;

		f = torsoAnim;

		initialFrame = bgGlobalAnimations[f].firstFrame;
	
		/*
		if (bgGlobalAnimations[f].numFrames > 20)
		{
			initialFrame += 6;
		}
		else if (bgGlobalAnimations[f].numFrames > 3)
		{ //HACK: Force it a couple frames into the animation so it doesn't lag behind the client visual position as much..
			initialFrame += 2;
		}
		*/

		BG_SaberStartTransAnim(self->client->ps.fd.saberAnimLevel, f, &animSpeedScale);

		animSpeed = 50.0f / bgGlobalAnimations[f].frameLerp;
		animSpeedScale = (animSpeed *= animSpeedScale);

		if (bgGlobalAnimations[f].loopFrames != -1)
		{
			aFlags = BONE_ANIM_OVERRIDE_LOOP;
		}
		else
		{
			aFlags = BONE_ANIM_OVERRIDE_FREEZE;
		}

		aFlags |= BONE_ANIM_BLEND; //since client defaults to blend. Not sure if this will make much difference if any on client position, but it's here just for the sake of matching them.

		trap_G2API_SetBoneAnim(self->client->ghoul2, 0, "upper_lumbar", initialFrame, bgGlobalAnimations[f].firstFrame+bgGlobalAnimations[f].numFrames, aFlags, animSpeedScale, level.time, initialFrame, 150);

		self->client->ps.torsoAnimExecute = torsoAnim;
		
		setTorso = qtrue;
	}

	if (!BG_FlippingAnim( self->client->ps.legsAnim ) &&
		!BG_FlippingAnim( self->client->ps.torsoAnim ) &&
		!BG_SpinningSaberAnim( self->client->ps.legsAnim ) &&
		!BG_SpinningSaberAnim( self->client->ps.torsoAnim ) &&
		!BG_InSpecialJump( self->client->ps.legsAnim ) &&
		!BG_InSpecialJump( self->client->ps.torsoAnim ) &&
		!BG_InRoll(&self->client->ps, self->client->ps.legsAnim) &&
		!BG_SaberInSpecial(self->client->ps.saberMove) &&
		!BG_SaberInSpecialAttack(self->client->ps.legsAnim) &&
		!BG_SaberInSpecialAttack(self->client->ps.torsoAnim) &&
		setTorso )
	{
		float animSpeed = 50.0f / bgGlobalAnimations[torsoAnim].frameLerp;
		int aFlags;
		animSpeedScale = (animSpeed *= animSpeedScale);

		if (bgGlobalAnimations[torsoAnim].loopFrames != -1)
		{
			aFlags = BONE_ANIM_OVERRIDE_LOOP;
		}
		else
		{
			aFlags = BONE_ANIM_OVERRIDE_FREEZE;
		}

		aFlags |= BONE_ANIM_BLEND; //since client defaults to blend. Not sure if this will make much difference if any on client position, but it's here just for the sake of matching them.

		trap_G2API_SetBoneAnim(self->client->ghoul2, 0, "Motion", bgGlobalAnimations[torsoAnim].firstFrame, bgGlobalAnimations[torsoAnim].firstFrame+bgGlobalAnimations[torsoAnim].numFrames, aFlags, animSpeedScale, level.time, -1, 150);
	}
}

int WP_MissileBlockForBlock( int saberBlock )
{
	switch( saberBlock )
	{
	case BLOCKED_UPPER_RIGHT:
		return BLOCKED_UPPER_RIGHT_PROJ;
		break;
	case BLOCKED_UPPER_LEFT:
		return BLOCKED_UPPER_LEFT_PROJ;
		break;
	case BLOCKED_LOWER_RIGHT:
		return BLOCKED_LOWER_RIGHT_PROJ;
		break;
	case BLOCKED_LOWER_LEFT:
		return BLOCKED_LOWER_LEFT_PROJ;
		break;
	case BLOCKED_TOP:
		return BLOCKED_TOP_PROJ;
		break;
	}
	return saberBlock;
}

void WP_SaberBlockNonRandom( gentity_t *self, vec3_t hitloc, qboolean missileBlock )
{
	vec3_t diff, fwdangles={0,0,0}, right;
	vec3_t clEye;
	float rightdot;
	float zdiff;

	VectorCopy(self->client->ps.origin, clEye);
	clEye[2] += self->client->ps.viewheight;

	VectorSubtract( hitloc, clEye, diff );
	diff[2] = 0;
	VectorNormalize( diff );

	fwdangles[1] = self->client->ps.viewangles[1];
	// Ultimately we might care if the shot was ahead or behind, but for now, just quadrant is fine.
	AngleVectors( fwdangles, NULL, right, NULL );

	rightdot = DotProduct(right, diff);
	zdiff = hitloc[2] - clEye[2];
	
	//FIXME: take torsoAngles into account?
	if ( zdiff > 0 )//40 )
	{
		if ( rightdot > 0.3 )
		{
			self->client->ps.saberBlocked = BLOCKED_UPPER_RIGHT;
		}
		else if ( rightdot < -0.3 )
		{
			self->client->ps.saberBlocked = BLOCKED_UPPER_LEFT;
		}
		else
		{
			self->client->ps.saberBlocked = BLOCKED_TOP;
		}
	}
	else if ( zdiff > -20 )//20 )
	{
		if ( zdiff < -10 )//30 )
		{//hmm, pretty low, but not low enough to use the low block, so we need to duck
			//NPC should duck, but NPC should never get here
		}
		if ( rightdot > 0.1 )
		{
			self->client->ps.saberBlocked = BLOCKED_UPPER_RIGHT;
		}
		else if ( rightdot < -0.1 )
		{
			self->client->ps.saberBlocked = BLOCKED_UPPER_LEFT;
		}
		else
		{//FIXME: this looks really weird if the shot is too low!
			self->client->ps.saberBlocked = BLOCKED_TOP;
		}
	}
	else
	{
		if ( rightdot >= 0 )
		{
			self->client->ps.saberBlocked = BLOCKED_LOWER_RIGHT;
		}
		else
		{
			self->client->ps.saberBlocked = BLOCKED_LOWER_LEFT;
		}
	}

	/*
	if ( !self->s.number )
	{
		gi.Printf( "EyeZ: %4.2f  HitZ: %4.2f  zdiff: %4.2f  rdot: %4.2f\n", self->client->renderInfo.eyePoint[2], hitloc[2], zdiff, rightdot );
		switch ( self->client->ps.saberBlocked )
		{
		case BLOCKED_TOP:
			gi.Printf( "BLOCKED_TOP\n" );
			break;
		case BLOCKED_UPPER_RIGHT:
			gi.Printf( "BLOCKED_UPPER_RIGHT\n" );
			break;
		case BLOCKED_UPPER_LEFT:
			gi.Printf( "BLOCKED_UPPER_LEFT\n" );
			break;
		case BLOCKED_LOWER_RIGHT:
			gi.Printf( "BLOCKED_LOWER_RIGHT\n" );
			break;
		case BLOCKED_LOWER_LEFT:
			gi.Printf( "BLOCKED_LOWER_LEFT\n" );
			break;
		default:
			break;
		}
	}
	*/

	if ( missileBlock )
	{
		self->client->ps.saberBlocked = WP_MissileBlockForBlock( self->client->ps.saberBlocked );
		//if ( !self->s.number )
		//{
			//G_DynaMixEvent( DM_BLOCK );
		//}
	}
	else
	{
		//if ( !self->s.number )
		//{
			//G_DynaMixEvent( DM_PARRY );
		//}
	}
}

void WP_SaberBlock( gentity_t *playerent, vec3_t hitloc, qboolean missileBlock )
{
	//gentity_t *playerent;
	vec3_t diff, fwdangles={0,0,0}, right;
	float rightdot;
	float zdiff;

	/*
	if (saber && saber->owner)
	{
		playerent = saber->owner;
		if (!playerent->client)
		{
			return;
		}
	}
	else
	{	// Bad entity passed.
		return;
	}
	*/
	//I don't see what the point of this was anyway, saber isn't used anywhere in the function.

	VectorSubtract(hitloc, playerent->client->ps.origin, diff);
	VectorNormalize(diff);

	fwdangles[1] = playerent->client->ps.viewangles[1];
	// Ultimately we might care if the shot was ahead or behind, but for now, just quadrant is fine.
	AngleVectors( fwdangles, NULL, right, NULL );

	rightdot = DotProduct(right, diff) + RandFloat(-0.2f,0.2f);
	zdiff = hitloc[2] - playerent->client->ps.origin[2] + Q_irand(-8,8);
	
	// Figure out what quadrant the block was in.
	if (zdiff > 24)
	{	// Attack from above
		if (Q_irand(0,1))
		{
			playerent->client->ps.saberBlocked = BLOCKED_TOP;
		}
		else
		{
			playerent->client->ps.saberBlocked = BLOCKED_UPPER_LEFT;
		}
	}
	else if (zdiff > 13)
	{	// The upper half has three viable blocks...
		if (rightdot > 0.25)
		{	// In the right quadrant...
			if (Q_irand(0,1))
			{
				playerent->client->ps.saberBlocked = BLOCKED_UPPER_LEFT;
			}
			else
			{
				playerent->client->ps.saberBlocked = BLOCKED_LOWER_LEFT;
			}
		}
		else
		{
			switch(Q_irand(0,3))
			{
			case 0:
				playerent->client->ps.saberBlocked = BLOCKED_UPPER_RIGHT;
				break;
			case 1:
			case 2:
				playerent->client->ps.saberBlocked = BLOCKED_LOWER_RIGHT;
				break;
			case 3:
				playerent->client->ps.saberBlocked = BLOCKED_TOP;
				break;
			}
		}
	}
	else
	{	// The lower half is a bit iffy as far as block coverage.  Pick one of the "low" ones at random.
		if (Q_irand(0,1))
		{
			playerent->client->ps.saberBlocked = BLOCKED_LOWER_RIGHT;
		}
		else
		{
			playerent->client->ps.saberBlocked = BLOCKED_LOWER_LEFT;
		}
	}

	if ( missileBlock )
	{
		playerent->client->ps.saberBlocked = WP_MissileBlockForBlock( playerent->client->ps.saberBlocked );
	}
}

int WP_SaberCanBlock(gentity_t *self, vec3_t point, int dflags, int mod, qboolean projectile, int attackStr)
{
	qboolean thrownSaber = qfalse;
	float blockFactor = 0;

	if (!self || !self->client || !point)
	{
		return 0;
	}

	if (attackStr == 8)
	{
		attackStr = 0;
		thrownSaber = qtrue;
	}

	if (BG_SaberInAttack(self->client->ps.saberMove))
	{
		return 0;
	}

	if (PM_InSaberAnim(self->client->ps.torsoAnim) && !self->client->ps.saberBlocked &&
		self->client->ps.saberMove != LS_READY && self->client->ps.saberMove != LS_NONE)
	{
		if ( self->client->ps.saberMove < LS_PARRY_UP || self->client->ps.saberMove > LS_REFLECT_LL )
		{
			return 0;
		}
	}

	if (self->client->ps.saberHolstered)
	{
		return 0;
	}

	if (self->client->ps.usingATST)
	{
		return 0;
	}

	if (self->client->ps.weapon != WP_SABER)
	{
		return 0;
	}

	if (self->client->ps.weaponstate == WEAPON_RAISING)
	{
		return 0;
	}

	if (self->client->ps.saberInFlight)
	{
		return 0;
	}

	if ((self->client->pers.cmd.buttons & BUTTON_ATTACK)/* &&
		(projectile || attackStr == FORCE_LEVEL_3)*/)
	{ //don't block when the player is trying to slash, if it's a projectile or he's doing a very strong attack
		return 0;
	}

	if (attackStr == FORCE_LEVEL_3)
	{
		if (self->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] >= FORCE_LEVEL_3)
		{
			if (Q_irand(1, 10) < 3)
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}

	if (attackStr == FORCE_LEVEL_2 && Q_irand(1, 10) < 3)
	{
		if (self->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] >= FORCE_LEVEL_3)
		{
			//do nothing for now
		}
		else if (self->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] >= FORCE_LEVEL_2)
		{
			if (Q_irand(1, 10) < 5)
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	
	if (attackStr == FORCE_LEVEL_1 && !self->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] &&
		Q_irand(1, 40) < 3)
	{ //if I have no defense level at all then I might be unable to block a level 1 attack (but very rarely)
		return 0;
	}

	if (SaberAttacking(self))
	{ //attacking, can't block now

		//FIXME: Do a "saber box" check here to see if the enemy saber hit this guy's saber
		return 0;
	}

	if (self->client->ps.saberMove != LS_READY &&
		!self->client->ps.saberBlocking)
	{
		return 0;
	}

	if (self->client->ps.saberBlockTime >= level.time)
	{
		return 0;
	}

	if (self->client->ps.forceHandExtend != HANDEXTEND_NONE)
	{
		return 0;
	}

	if (self->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] == FORCE_LEVEL_3)
	{
		blockFactor = 0.05f;
	}
	else if (self->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] == FORCE_LEVEL_2)
	{
		blockFactor = 0.6f;
	}
	else if (self->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] == FORCE_LEVEL_1)
	{
		blockFactor = 0.9f;
	}
	else
	{ //for now we just don't get to autoblock with no def
		return 0;
	}

	if (thrownSaber)
	{
		blockFactor -= 0.25f;
	}

	if (!InFront( point, self->client->ps.origin, self->client->ps.viewangles, blockFactor )) //orig 0.2f
	{
		return 0;
	}

	WP_SaberBlockNonRandom(self, point, projectile);
	return 1;
}

qboolean HasSetSaberOnly(void)
{
	int i = 0;
	int wDisable = 0;

	if (g_gametype.integer == GT_JEDIMASTER)
	{ //set to 0 
		return qfalse;
	}

	if (g_gametype.integer == GT_TOURNAMENT)
	{
		wDisable = g_duelWeaponDisable.integer;
	}
	else
	{
		wDisable = g_weaponDisable.integer;
	}

	while (i < WP_NUM_WEAPONS)
	{
		if (!(wDisable & (1 << i)) &&
			i != WP_SABER && i != WP_NONE)
		{
			return qfalse;
		}

		i++;
	}

	return qtrue;
}

