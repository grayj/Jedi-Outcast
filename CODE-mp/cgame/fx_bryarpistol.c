// Bryar Pistol Weapon Effects

#include "cg_local.h"

/*
-------------------------

	MAIN FIRE

-------------------------
FX_BryarProjectileThink
-------------------------
*/
void FX_BryarProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
	{
		forward[2] = 1.0f;
	}

	trap_FX_PlayEffectID( cgs.effects.bryarShotEffect, cent->lerpOrigin, forward );
}

/*
-------------------------
FX_BryarHitWall
-------------------------
*/
void FX_BryarHitWall( vec3_t origin, vec3_t normal )
{
	trap_FX_PlayEffectID( cgs.effects.bryarWallImpactEffect, origin, normal );
}

/*
-------------------------
FX_BryarHitPlayer
-------------------------
*/
void FX_BryarHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	if ( humanoid )
	{
		trap_FX_PlayEffectID( cgs.effects.bryarFleshImpactEffect, origin, normal );
	}
	else
	{
		trap_FX_PlayEffectID( cgs.effects.bryarDroidImpactEffect, origin, normal );
	}
}


/*
-------------------------

	ALT FIRE

-------------------------
FX_BryarAltProjectileThink
-------------------------
*/
void FX_BryarAltProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;
	int t;

	if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
	{
		forward[2] = 1.0f;
	}

	// see if we have some sort of extra charge going on
	for (t = 1; t < cent->currentState.generic1; t++ )
	{
		// just add ourselves over, and over, and over when we are charged
		trap_FX_PlayEffectID( cgs.effects.bryarPowerupShotEffect, cent->lerpOrigin, forward );
	}

	//	for ( int t = 1; t < cent->gent->count; t++ )	// The single player stores the charge in count, which isn't accessible on the client

	trap_FX_PlayEffectID( cgs.effects.bryarShotEffect, cent->lerpOrigin, forward );
}

/*
-------------------------
FX_BryarAltHitWall
-------------------------
*/
void FX_BryarAltHitWall( vec3_t origin, vec3_t normal, int power )
{
	switch( power )
	{
	case 4:
	case 5:
		trap_FX_PlayEffectID( cgs.effects.bryarWallImpactEffect3, origin, normal );
		break;

	case 2:
	case 3:
		trap_FX_PlayEffectID( cgs.effects.bryarWallImpactEffect2, origin, normal );
		break;

	default:
		trap_FX_PlayEffectID( cgs.effects.bryarWallImpactEffect, origin, normal );
		break;
	}
}

/*
-------------------------
FX_BryarAltHitPlayer
-------------------------
*/
void FX_BryarAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	if ( humanoid )
	{
		trap_FX_PlayEffectID( cgs.effects.bryarFleshImpactEffect, origin, normal );
	}
	else
	{
		trap_FX_PlayEffectID( cgs.effects.bryarDroidImpactEffect, origin, normal );
	}
}


//TURRET
/*
-------------------------
FX_TurretProjectileThink
-------------------------
*/
void FX_TurretProjectileThink(  centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
	{
		forward[2] = 1.0f;
	}

	trap_FX_PlayEffectID( cgs.effects.turretShotEffect, cent->lerpOrigin, forward );
}

/*
-------------------------
FX_TurretHitWall
-------------------------
*/
void FX_TurretHitWall( vec3_t origin, vec3_t normal )
{
	trap_FX_PlayEffectID( cgs.effects.bryarWallImpactEffect, origin, normal );
}

/*
-------------------------
FX_TurretHitPlayer
-------------------------
*/
void FX_TurretHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	if ( humanoid )
	{
		trap_FX_PlayEffectID( cgs.effects.bryarFleshImpactEffect, origin, normal );
	}
	else
	{
		trap_FX_PlayEffectID( cgs.effects.bryarDroidImpactEffect, origin, normal );
	}
}
