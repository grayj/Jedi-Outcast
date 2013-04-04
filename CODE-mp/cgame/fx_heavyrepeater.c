// Heavy Repeater Weapon

#include "cg_local.h"

/*
---------------------------
FX_RepeaterProjectileThink
---------------------------
*/

void FX_RepeaterProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
	{
		forward[2] = 1.0f;
	}

	trap_FX_PlayEffectID( cgs.effects.repeaterProjectileEffect, cent->lerpOrigin, forward );
}

/*
------------------------
FX_RepeaterHitWall
------------------------
*/

void FX_RepeaterHitWall( vec3_t origin, vec3_t normal )
{
	trap_FX_PlayEffectID( cgs.effects.repeaterWallImpactEffect, origin, normal );
}

/*
------------------------
FX_RepeaterHitPlayer
------------------------
*/

void FX_RepeaterHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	trap_FX_PlayEffectID( cgs.effects.repeaterFleshImpactEffect, origin, normal );
}

/*
------------------------------
FX_RepeaterAltProjectileThink
-----------------------------
*/

void FX_RepeaterAltProjectileThink( centity_t *cent, const struct weaponInfo_s *weapon )
{
	vec3_t forward;

	if ( VectorNormalize2( cent->currentState.pos.trDelta, forward ) == 0.0f )
	{
		forward[2] = 1.0f;
	}

	trap_FX_PlayEffectID( cgs.effects.repeaterAltProjectileEffect, cent->lerpOrigin, forward );
}

/*
------------------------
FX_RepeaterAltHitWall
------------------------
*/

void FX_RepeaterAltHitWall( vec3_t origin, vec3_t normal )
{
	trap_FX_PlayEffectID( cgs.effects.repeaterAltWallImpactEffect, origin, normal );
}

/*
------------------------
FX_RepeaterAltHitPlayer
------------------------
*/

void FX_RepeaterAltHitPlayer( vec3_t origin, vec3_t normal, qboolean humanoid )
{
	trap_FX_PlayEffectID( cgs.effects.repeaterAltWallImpactEffect, origin, normal );
}
