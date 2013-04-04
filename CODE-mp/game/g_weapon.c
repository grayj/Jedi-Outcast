// Copyright (C) 1999-2000 Id Software, Inc.
//
// g_weapon.c 
// perform the server side effects of a weapon firing

#include "g_local.h"
#include "be_aas.h"

static	float	s_quadFactor;
static	vec3_t	forward, right, up;
static	vec3_t	muzzle;

// Bryar Pistol
//--------
#define BRYAR_PISTOL_VEL			1600
#define BRYAR_PISTOL_DAMAGE			10
#define BRYAR_CHARGE_UNIT			200.0f	// bryar charging gives us one more unit every 200ms--if you change this, you'll have to do the same in bg_pmove
#define BRYAR_ALT_SIZE				1.0f

// E11 Blaster
//---------
#define BLASTER_SPREAD				1.6f//1.2f
#define BLASTER_VELOCITY			2300
#define BLASTER_DAMAGE				20

// Tenloss Disruptor
//----------
#define DISRUPTOR_MAIN_DAMAGE			30 //40
#define DISRUPTOR_NPC_MAIN_DAMAGE_CUT	0.25f

#define DISRUPTOR_ALT_DAMAGE			100 //125
#define DISRUPTOR_NPC_ALT_DAMAGE_CUT	0.2f
#define DISRUPTOR_ALT_TRACES			3		// can go through a max of 3 damageable(sp?) entities
#define DISRUPTOR_CHARGE_UNIT			50.0f	// distruptor charging gives us one more unit every 50ms--if you change this, you'll have to do the same in bg_pmove

// Wookiee Bowcaster
//----------
#define	BOWCASTER_DAMAGE			50
#define	BOWCASTER_VELOCITY			1300
#define BOWCASTER_SPLASH_DAMAGE		0
#define BOWCASTER_SPLASH_RADIUS		0
#define BOWCASTER_SIZE				2

#define BOWCASTER_ALT_SPREAD		5.0f
#define BOWCASTER_VEL_RANGE			0.3f
#define BOWCASTER_CHARGE_UNIT		200.0f	// bowcaster charging gives us one more unit every 200ms--if you change this, you'll have to do the same in bg_pmove

// Heavy Repeater
//----------
#define REPEATER_SPREAD				1.4f
#define	REPEATER_DAMAGE				14
#define	REPEATER_VELOCITY			1600

#define REPEATER_ALT_SIZE				3	// half of bbox size
#define	REPEATER_ALT_DAMAGE				60
#define REPEATER_ALT_SPLASH_DAMAGE		60
#define REPEATER_ALT_SPLASH_RADIUS		128
#define	REPEATER_ALT_VELOCITY			1100

// DEMP2
//----------
#define	DEMP2_DAMAGE				35
#define	DEMP2_VELOCITY				1800
#define	DEMP2_SIZE					2		// half of bbox size

#define DEMP2_ALT_DAMAGE			8 //12		// does 12, 36, 84 at each of the 3 charge levels.
#define DEMP2_CHARGE_UNIT			700.0f	// demp2 charging gives us one more unit every 700ms--if you change this, you'll have to do the same in bg_weapons
#define DEMP2_ALT_RANGE				4096
#define DEMP2_ALT_SPLASHRADIUS		256

// Golan Arms Flechette
//---------
#define FLECHETTE_SHOTS				5
#define FLECHETTE_SPREAD			4.0f
#define FLECHETTE_DAMAGE			12//15
#define FLECHETTE_VEL				3500
#define FLECHETTE_SIZE				1
#define FLECHETTE_MINE_RADIUS_CHECK	256
#define FLECHETTE_ALT_DAMAGE		60
#define FLECHETTE_ALT_SPLASH_DAM	60
#define FLECHETTE_ALT_SPLASH_RAD	128

// Personal Rocket Launcher
//---------
#define	ROCKET_VELOCITY				900
#define	ROCKET_DAMAGE				100
#define	ROCKET_SPLASH_DAMAGE		100
#define	ROCKET_SPLASH_RADIUS		160
#define ROCKET_SIZE					3
#define ROCKET_ALT_THINK_TIME		100

// Stun Baton
//--------------
#define STUN_BATON_DAMAGE			20
#define STUN_BATON_ALT_DAMAGE		20
#define STUN_BATON_RANGE			8


extern qboolean G_BoxInBounds( vec3_t point, vec3_t mins, vec3_t maxs, vec3_t boundsMins, vec3_t boundsMaxs );

static void WP_FireEmplaced( gentity_t *ent, qboolean altFire );

void laserTrapStick( gentity_t *ent, vec3_t endpos, vec3_t normal );

void touch_NULL( gentity_t *ent, gentity_t *other, trace_t *trace )
{

}

void laserTrapExplode( gentity_t *self );

//-----------------------------------------------------------------------------
void W_TraceSetStart( gentity_t *ent, vec3_t start, vec3_t mins, vec3_t maxs )
//-----------------------------------------------------------------------------
{
	//make sure our start point isn't on the other side of a wall
	trace_t	tr;
	vec3_t	entMins;
	vec3_t	entMaxs;
	vec3_t	eyePoint;

	VectorAdd( ent->r.currentOrigin, ent->r.mins, entMins );
	VectorAdd( ent->r.currentOrigin, ent->r.maxs, entMaxs );

	if ( G_BoxInBounds( start, mins, maxs, entMins, entMaxs ) )
	{
		return;
	}

	if ( !ent->client )
	{
		return;
	}

	VectorCopy( ent->s.pos.trBase, eyePoint);
	eyePoint[2] += ent->client->ps.viewheight;
		
	trap_Trace( &tr, eyePoint, mins, maxs, start, ent->s.number, MASK_SOLID|CONTENTS_SHOTCLIP );

	if ( tr.startsolid || tr.allsolid )
	{
		return;
	}

	if ( tr.fraction < 1.0f )
	{
		VectorCopy( tr.endpos, start );
	}
}


/*
----------------------------------------------
	PLAYER WEAPONS
----------------------------------------------
*/

/*
======================================================================

BRYAR PISTOL

======================================================================
*/

//----------------------------------------------
static void WP_FireBryarPistol( gentity_t *ent, qboolean altFire )
//---------------------------------------------------------
{
	int damage = BRYAR_PISTOL_DAMAGE;
	int count;

	gentity_t	*missile = CreateMissile( muzzle, forward, BRYAR_PISTOL_VEL, 10000, ent, altFire );

	missile->classname = "bryar_proj";
	missile->s.weapon = WP_BRYAR_PISTOL;

	if ( altFire )
	{
		float boxSize = 0;

		count = ( level.time - ent->client->ps.weaponChargeTime ) / BRYAR_CHARGE_UNIT;

		if ( count < 1 )
		{
			count = 1;
		}
		else if ( count > 5 )
		{
			count = 5;
		}

		if (count > 1)
		{
			damage *= (count*1.7);
		}
		else
		{
			damage *= (count*1.5);
		}

		missile->s.generic1 = count; // The missile will then render according to the charge level.
//		missile->count = count;		// The single player stores the charge in count, which isn't accessible on the client

		boxSize = BRYAR_ALT_SIZE*(count*0.5);

		VectorSet( missile->r.maxs, boxSize, boxSize, boxSize );
		VectorSet( missile->r.mins, -boxSize, -boxSize, -boxSize );
	}

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	if (altFire)
	{
		missile->methodOfDeath = MOD_BRYAR_PISTOL_ALT;
	}
	else
	{
		missile->methodOfDeath = MOD_BRYAR_PISTOL;
	}
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to bounce forever
	// NOTENOTE These don't bounce yet.
	missile->bounceCount = 8;
}

/*
======================================================================

GENERIC

======================================================================
*/

//---------------------------------------------------------
void WP_FireTurretMissile( gentity_t *ent, vec3_t start, vec3_t dir, qboolean altFire, int damage, int velocity, int mod, gentity_t *ignore )
//---------------------------------------------------------
{
	gentity_t *missile;

	missile = CreateMissile( start, dir, velocity, 10000, ent, altFire );

	missile->classname = "generic_proj";
	missile->s.weapon = WP_TURRET;

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = mod;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	if (ignore)
	{
		missile->passThroughNum = ignore->s.number+1;
	}

	// we don't want it to bounce forever
	// NOTENOTE These don't bounce yet.
	missile->bounceCount = 8;
}

//Currently only the seeker drone uses this, but it might be useful for other things as well.

//---------------------------------------------------------
void WP_FireGenericBlasterMissile( gentity_t *ent, vec3_t start, vec3_t dir, qboolean altFire, int damage, int velocity, int mod )
//---------------------------------------------------------
{
	gentity_t *missile;

	missile = CreateMissile( start, dir, velocity, 10000, ent, altFire );

	missile->classname = "generic_proj";
	missile->s.weapon = WP_BRYAR_PISTOL;

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = mod;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to bounce forever
	// NOTENOTE These don't bounce yet.
	missile->bounceCount = 8;
}

/*
======================================================================

BLASTER

======================================================================
*/

//---------------------------------------------------------
void WP_FireBlasterMissile( gentity_t *ent, vec3_t start, vec3_t dir, qboolean altFire )
//---------------------------------------------------------
{
	int velocity	= BLASTER_VELOCITY;
	int	damage		= BLASTER_DAMAGE;
	gentity_t *missile;

	// NOTENOTE Vehicle models are not yet implemented
/*	if ( ent->client && ent->client->ps.vehicleModel != 0 )
	{
		velocity = 10000;
	}
*/
	
	missile = CreateMissile( start, dir, velocity, 10000, ent, altFire );

	missile->classname = "blaster_proj";
	missile->s.weapon = WP_BLASTER;

	// NOTENOTE Vehicle models are not yet implemented
/*	if ( ent->client && ent->client->ps.vehicleModel != 0 )
	{
		damage = 250;
	}
	*/

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_BLASTER;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to bounce forever
	// NOTENOTE These don't bounce yet.
	missile->bounceCount = 8;
}

//---------------------------------------------------------
void WP_FireEmplacedMissile( gentity_t *ent, vec3_t start, vec3_t dir, qboolean altFire, gentity_t *ignore )
//---------------------------------------------------------
{
	int velocity	= BLASTER_VELOCITY;
	int	damage		= BLASTER_DAMAGE;
	gentity_t *missile;

	// NOTENOTE Vehicle models are not yet implemented
/*	if ( ent->client && ent->client->ps.vehicleModel != 0 )
	{
		velocity = 10000;
	}
*/
	
	missile = CreateMissile( start, dir, velocity, 10000, ent, altFire );

	missile->classname = "emplaced_gun_proj";
	missile->s.weapon = WP_TURRET;//WP_EMPLACED_GUN;

	// NOTENOTE Vehicle models are not yet implemented
/*	if ( ent->client && ent->client->ps.vehicleModel != 0 )
	{
		damage = 250;
	}
	*/

	missile->activator = ignore;

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_BLASTER;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	if (ignore)
	{
		missile->passThroughNum = ignore->s.number+1;
	}

	// we don't want it to bounce forever
	// NOTENOTE These don't bounce yet.
	missile->bounceCount = 8;
}

//---------------------------------------------------------
static void WP_FireBlaster( gentity_t *ent, qboolean altFire )
//---------------------------------------------------------
{
	vec3_t	dir, angs;

	vectoangles( forward, angs );

	if ( altFire )
	{
		// add some slop to the alt-fire direction
		angs[PITCH] += crandom() * BLASTER_SPREAD;
		angs[YAW]	+= crandom() * BLASTER_SPREAD;
	}

	AngleVectors( angs, dir, NULL, NULL );

	// FIXME: if temp_org does not have clear trace to inside the bbox, don't shoot!
	WP_FireBlasterMissile( ent, muzzle, dir, altFire );
}



int G_GetHitLocation(gentity_t *target, vec3_t ppoint);

/*
======================================================================

DISRUPTOR

======================================================================
*/
//---------------------------------------------------------
static void WP_DisruptorMainFire( gentity_t *ent )
//---------------------------------------------------------
{
	int			damage = DISRUPTOR_MAIN_DAMAGE;
	qboolean	render_impact = qtrue;
	vec3_t		start, end	/*, spot*/	;
	trace_t		tr;
	gentity_t	*traceEnt, *tent;
	float		/*dist, */shotRange = 8192;
	int			ignore, traces;

	memset(&tr, 0, sizeof(tr)); //to shut the compiler up

	VectorCopy( ent->client->ps.origin, start );
	start[2] += ent->client->ps.viewheight;//By eyes

	VectorMA( start, shotRange, forward, end );

//	trap_Trace( &tr, start, NULL, NULL, end, ent->s.number, MASK_SHOT);

	ignore = ent->s.number;
	traces = 0;
	while ( traces < 10 )
	{//need to loop this in case we hit a Jedi who dodges the shot
		trap_Trace( &tr, start, NULL, NULL, end, ignore, MASK_SHOT );

		traceEnt = &g_entities[tr.entityNum];

		if (traceEnt && traceEnt->client && traceEnt->client->ps.duelInProgress &&
			traceEnt->client->ps.duelIndex != ent->s.number)
		{
			VectorCopy( tr.endpos, start );
			ignore = tr.entityNum;
			traces++;
			continue;
		}

		if ( Jedi_DodgeEvasion( traceEnt, ent, &tr, G_GetHitLocation(traceEnt, tr.endpos) ) )
		{//act like we didn't even hit him
			VectorCopy( tr.endpos, start );
			ignore = tr.entityNum;
			traces++;
			continue;
		}
		else if (traceEnt && traceEnt->client && traceEnt->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] >= FORCE_LEVEL_3)
		{
			if (WP_SaberCanBlock(traceEnt, tr.endpos, 0, MOD_DISRUPTOR, qtrue, 0))
			{ //broadcast and stop the shot because it was blocked
				gentity_t *te = NULL;

				tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_MAIN_SHOT );
				VectorCopy( muzzle, tent->s.origin2 );
				tent->s.eventParm = ent->s.number;

				te = G_TempEntity( tr.endpos, EV_SABER_BLOCK );
				VectorCopy(tr.endpos, te->s.origin);
				VectorCopy(tr.plane.normal, te->s.angles);
				if (!te->s.angles[0] && !te->s.angles[1] && !te->s.angles[2])
				{
					te->s.angles[1] = 1;
				}
				te->s.eventParm = 0;

				return;
			}
		}
		//a Jedi is not dodging this shot
		break;
	}

	if ( tr.surfaceFlags & SURF_NOIMPACT ) 
	{
		render_impact = qfalse;
	}

	// always render a shot beam, doing this the old way because I don't much feel like overriding the effect.
	tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_MAIN_SHOT );
	VectorCopy( muzzle, tent->s.origin2 );
	tent->s.eventParm = ent->s.number;

	traceEnt = &g_entities[tr.entityNum];

	if ( render_impact )
	{
		if ( tr.entityNum < ENTITYNUM_WORLD && traceEnt->takedamage )
		{
			// Create a simple impact type mark that doesn't last long in the world
//			G_PlayEffect( G_EffectIndex( "disruptor/flesh_impact" ), tr.endpos, tr.plane.normal );

			if ( traceEnt->client && LogAccuracyHit( traceEnt, ent )) 
			{
				ent->client->accuracy_hits++;
			} 

			G_Damage( traceEnt, ent, ent, forward, tr.endpos, damage, DAMAGE_NORMAL, MOD_DISRUPTOR );
			
			tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_HIT );
			tent->s.eventParm = DirToByte( tr.plane.normal );
			if (traceEnt->client)
			{
				tent->s.weapon = 1;
			}
		}
		else 
		{
			 // Hmmm, maybe don't make any marks on things that could break
			tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_SNIPER_MISS );
			tent->s.eventParm = DirToByte( tr.plane.normal );
			tent->s.weapon = 1;
		}
	}
}


//---------------------------------------------------------
void WP_DisruptorAltFire( gentity_t *ent )
//---------------------------------------------------------
{
	int			damage = 0, skip;
	qboolean	render_impact = qtrue;
	vec3_t		start, end;
	vec3_t		muzzle2;
	trace_t		tr;
	gentity_t	*traceEnt, *tent;
	float		shotRange = 8192;
	// float	dist, shotDist;
	// vec3_t	spot, dir;
	int			i;
	int			count;
	int			traces = DISRUPTOR_ALT_TRACES;
	qboolean	fullCharge = qfalse;

	damage = DISRUPTOR_ALT_DAMAGE-30;

	/*
	if (ent->client->ps.zoomMode == 1 &&
		ent->client->ps.zoomLocked)
	{ //Scale the additional 25 damage based on the zoomFov for the client.
	  //In this instance, zoomFov 1 is minimum zoom while zoomFov 50 is maximum.
		damage += ent->client->ps.zoomFov/2;
	}
	*/

	VectorCopy( muzzle, muzzle2 ); // making a backup copy

	VectorCopy( ent->client->ps.origin, start );
	start[2] += ent->client->ps.viewheight;//By eyes

	count = ( level.time - ent->client->ps.weaponChargeTime ) / DISRUPTOR_CHARGE_UNIT;

	count *= 2;

	if ( count < 1 )
	{
		count = 1;
	}
	else if ( count >= 60 )
	{
		count = 60;
		fullCharge = qtrue;
	}

	// more powerful charges go through more things
	if ( count < 10 )
	{
		traces = 1;
	}
	else if ( count < 20 )
	{
		traces = 2;
	}

	damage += count;

	skip = ent->s.number;

	for (i = 0; i < traces; i++ )
	{
		VectorMA( start, shotRange, forward, end );

		trap_Trace ( &tr, start, NULL, NULL, end, skip, MASK_SHOT);

		if ( tr.surfaceFlags & SURF_NOIMPACT ) 
		{
			render_impact = qfalse;
		}

		traceEnt = &g_entities[tr.entityNum];

		if (traceEnt && traceEnt->client && traceEnt->client->ps.duelInProgress &&
			traceEnt->client->ps.duelIndex != ent->s.number)
		{
			skip = tr.entityNum;
			VectorCopy(tr.endpos, start);
			continue;
		}

		if (Jedi_DodgeEvasion(traceEnt, ent, &tr, G_GetHitLocation(traceEnt, tr.endpos)))
		{
			skip = tr.entityNum;
			VectorCopy(tr.endpos, start);
			continue;
		}
		else if (traceEnt && traceEnt->client && traceEnt->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] >= FORCE_LEVEL_3)
		{
			if (WP_SaberCanBlock(traceEnt, tr.endpos, 0, MOD_DISRUPTOR_SNIPER, qtrue, 0))
			{ //broadcast and stop the shot because it was blocked
				gentity_t *te = NULL;

				tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_SNIPER_SHOT );
				VectorCopy( muzzle, tent->s.origin2 );
				tent->s.shouldtarget = fullCharge;
				tent->s.eventParm = ent->s.number;

				te = G_TempEntity( tr.endpos, EV_SABER_BLOCK );
				VectorCopy(tr.endpos, te->s.origin);
				VectorCopy(tr.plane.normal, te->s.angles);
				if (!te->s.angles[0] && !te->s.angles[1] && !te->s.angles[2])
				{
					te->s.angles[1] = 1;
				}
				te->s.eventParm = 0;

				return;
			}
		}

		// always render a shot beam, doing this the old way because I don't much feel like overriding the effect.
		tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_SNIPER_SHOT );
		VectorCopy( muzzle, tent->s.origin2 );
		tent->s.shouldtarget = fullCharge;
		tent->s.eventParm = ent->s.number;

		// If the beam hits a skybox, etc. it would look foolish to add impact effects
		if ( render_impact ) 
		{
			if ( traceEnt->takedamage && traceEnt->client )
			{
				tent->s.otherEntityNum = traceEnt->s.number;

				// Create a simple impact type mark
//				G_PlayEffect( G_EffectIndex( "disruptor/alt_hit" ), tr.endpos, tr.plane.normal );
				tent = G_TempEntity(tr.endpos, EV_MISSILE_MISS);
				tent->s.eventParm = DirToByte(tr.plane.normal);
				tent->s.eFlags |= EF_ALT_FIRING;
	
				if ( LogAccuracyHit( traceEnt, ent )) 
				{
					ent->client->accuracy_hits++;
				}
			} 
			else 
			{
				 if ( traceEnt->r.svFlags & SVF_GLASS_BRUSH 
						|| traceEnt->takedamage 
						|| traceEnt->s.eType == ET_MOVER )
				 {
					//rww - is there some reason this was doing nothing?
					if ( traceEnt->takedamage )
					{
						G_Damage( traceEnt, ent, ent, forward, tr.endpos, damage, 
								DAMAGE_NO_KNOCKBACK/*|DAMAGE_HALF_ARMOR_REDUCTION*/, MOD_DISRUPTOR_SNIPER );

						tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_HIT );
						tent->s.eventParm = DirToByte( tr.plane.normal );
					}
				 }
				 else
				 {
					 // Hmmm, maybe don't make any marks on things that could break
					tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_SNIPER_MISS );
					tent->s.eventParm = DirToByte( tr.plane.normal );
				 }
				break; // and don't try any more traces
			}

			if ( traceEnt->takedamage )
			{
				vec3_t preAng;
				int preHealth = traceEnt->health;
				int preLegs = 0;
				int preTorso = 0;

				if (traceEnt->client)
				{
					preLegs = traceEnt->client->ps.legsAnim;
					preTorso = traceEnt->client->ps.torsoAnim;
					VectorCopy(traceEnt->client->ps.viewangles, preAng);
				}

				G_Damage( traceEnt, ent, ent, forward, tr.endpos, damage, DAMAGE_NO_KNOCKBACK, MOD_DISRUPTOR_SNIPER );

				if (traceEnt->client && preHealth > 0 && traceEnt->health <= 0 && fullCharge)
				{ //was killed by a fully charged sniper shot, so disintegrate
					VectorCopy(preAng, traceEnt->client->ps.viewangles);

					traceEnt->client->ps.eFlags |= EF_DISINTEGRATION;
					VectorCopy(tr.endpos, traceEnt->client->ps.lastHitLoc);

					traceEnt->client->ps.legsAnim = preLegs;
					traceEnt->client->ps.torsoAnim = preTorso;

					traceEnt->r.contents = 0;

					VectorClear(traceEnt->client->ps.velocity);
				}

				tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_HIT );
				tent->s.eventParm = DirToByte( tr.plane.normal );
				if (traceEnt->client)
				{
					tent->s.weapon = 1;
				}
			}
		}
		else // not rendering impact, must be a skybox or other similar thing?
		{
			break; // don't try anymore traces
		}

		// Get ready for an attempt to trace through another person
		VectorCopy( tr.endpos, muzzle );
		VectorCopy( tr.endpos, start );
		skip = tr.entityNum;
	}
}


//---------------------------------------------------------
static void WP_FireDisruptor( gentity_t *ent, qboolean altFire )
//---------------------------------------------------------
{
	if (!ent || !ent->client || ent->client->ps.zoomMode != 1)
	{ //do not ever let it do the alt fire when not zoomed
		altFire = qfalse;
	}

	if ( altFire )
	{
		WP_DisruptorAltFire( ent );
	}
	else
	{
		WP_DisruptorMainFire( ent );
	}
}


/*
======================================================================

BOWCASTER

======================================================================
*/

static void WP_BowcasterAltFire( gentity_t *ent )
{
	int	damage	= BOWCASTER_DAMAGE;

	gentity_t *missile = CreateMissile( muzzle, forward, BOWCASTER_VELOCITY, 10000, ent, qfalse);

	missile->classname = "bowcaster_proj";
	missile->s.weapon = WP_BOWCASTER;

	VectorSet( missile->r.maxs, BOWCASTER_SIZE, BOWCASTER_SIZE, BOWCASTER_SIZE );
	VectorScale( missile->r.maxs, -1, missile->r.mins );

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_BOWCASTER;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
//	missile->splashDamage = BOWCASTER_SPLASH_DAMAGE;
//	missile->splashRadius = BOWCASTER_SPLASH_RADIUS;

	missile->s.eFlags |= EF_BOUNCE;
	missile->bounceCount = 3;
}

//---------------------------------------------------------
static void WP_BowcasterMainFire( gentity_t *ent )
//---------------------------------------------------------
{
	int			damage	= BOWCASTER_DAMAGE, count;
	float		vel;
	vec3_t		angs, dir;
	gentity_t	*missile;
	int i;

	count = ( level.time - ent->client->ps.weaponChargeTime ) / BOWCASTER_CHARGE_UNIT;

	if ( count < 1 )
	{
		count = 1;
	}
	else if ( count > 5 )
	{
		count = 5;
	}

	if ( !(count & 1 ))
	{
		// if we aren't odd, knock us down a level
		count--;
	}

	//scale the damage down based on how many are about to be fired
	if (count <= 1)
	{
		damage = 50;
	}
	else if (count == 2)
	{
		damage = 45;
	}
	else if (count == 3)
	{
		damage = 40;
	}
	else if (count == 4)
	{
		damage = 35;
	}
	else
	{
		damage = 30;
	}

	for (i = 0; i < count; i++ )
	{
		// create a range of different velocities
		vel = BOWCASTER_VELOCITY * ( crandom() * BOWCASTER_VEL_RANGE + 1.0f );

		vectoangles( forward, angs );

		// add some slop to the alt-fire direction
		angs[PITCH] += crandom() * BOWCASTER_ALT_SPREAD * 0.2f;
		angs[YAW]	+= ((i+0.5f) * BOWCASTER_ALT_SPREAD - count * 0.5f * BOWCASTER_ALT_SPREAD );
		
		AngleVectors( angs, dir, NULL, NULL );

		missile = CreateMissile( muzzle, dir, vel, 10000, ent, qtrue );

		missile->classname = "bowcaster_alt_proj";
		missile->s.weapon = WP_BOWCASTER;

		VectorSet( missile->r.maxs, BOWCASTER_SIZE, BOWCASTER_SIZE, BOWCASTER_SIZE );
		VectorScale( missile->r.maxs, -1, missile->r.mins );

		missile->damage = damage;
		missile->dflags = DAMAGE_DEATH_KNOCKBACK;
		missile->methodOfDeath = MOD_BOWCASTER;
		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
//		missile->splashDamage = BOWCASTER_SPLASH_DAMAGE;
//		missile->splashRadius = BOWCASTER_SPLASH_RADIUS;

		// we don't want it to bounce
		missile->bounceCount = 0;
	}
}

//---------------------------------------------------------
static void WP_FireBowcaster( gentity_t *ent, qboolean altFire )
//---------------------------------------------------------
{
	if ( altFire )
	{
		WP_BowcasterAltFire( ent );
	}
	else
	{
		WP_BowcasterMainFire( ent );
	}
}



/*
======================================================================

REPEATER

======================================================================
*/

//---------------------------------------------------------
static void WP_RepeaterMainFire( gentity_t *ent, vec3_t dir )
//---------------------------------------------------------
{
	int	damage	= REPEATER_DAMAGE;

	gentity_t *missile = CreateMissile( muzzle, dir, REPEATER_VELOCITY, 10000, ent, qfalse );

	missile->classname = "repeater_proj";
	missile->s.weapon = WP_REPEATER;

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_REPEATER;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to bounce forever
	missile->bounceCount = 8;
}

//---------------------------------------------------------
static void WP_RepeaterAltFire( gentity_t *ent )
//---------------------------------------------------------
{
	int	damage	= REPEATER_ALT_DAMAGE;

	gentity_t *missile = CreateMissile( muzzle, forward, REPEATER_ALT_VELOCITY, 10000, ent, qtrue );

	missile->classname = "repeater_alt_proj";
	missile->s.weapon = WP_REPEATER;
//	missile->mass = 10;		// NOTENOTE No mass yet

	VectorSet( missile->r.maxs, REPEATER_ALT_SIZE, REPEATER_ALT_SIZE, REPEATER_ALT_SIZE );
	VectorScale( missile->r.maxs, -1, missile->r.mins );
	missile->s.pos.trType = TR_GRAVITY;
	missile->s.pos.trDelta[2] += 40.0f; //give a slight boost in the upward direction
	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_REPEATER_ALT;
	missile->splashMethodOfDeath = MOD_REPEATER_ALT_SPLASH;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
	missile->splashDamage = REPEATER_ALT_SPLASH_DAMAGE;
	missile->splashRadius = REPEATER_ALT_SPLASH_RADIUS;

	// we don't want it to bounce forever
	missile->bounceCount = 8;
}

//---------------------------------------------------------
static void WP_FireRepeater( gentity_t *ent, qboolean altFire )
//---------------------------------------------------------
{
	vec3_t	dir, angs;

	vectoangles( forward, angs );

	if ( altFire )
	{
		WP_RepeaterAltFire( ent );
	}
	else
	{
		// add some slop to the alt-fire direction
		angs[PITCH] += crandom() * REPEATER_SPREAD;
		angs[YAW]	+= crandom() * REPEATER_SPREAD;

		AngleVectors( angs, dir, NULL, NULL );

		// NOTENOTE if temp_org does not have clear trace to inside the bbox, don't shoot!
		WP_RepeaterMainFire( ent, dir );
	}
}


/*
======================================================================

DEMP2

======================================================================
*/

static void WP_DEMP2_MainFire( gentity_t *ent )
{
	int	damage	= DEMP2_DAMAGE;

	gentity_t *missile = CreateMissile( muzzle, forward, DEMP2_VELOCITY, 10000, ent, qfalse);

	missile->classname = "demp2_proj";
	missile->s.weapon = WP_DEMP2;

	VectorSet( missile->r.maxs, DEMP2_SIZE, DEMP2_SIZE, DEMP2_SIZE );
	VectorScale( missile->r.maxs, -1, missile->r.mins );
	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->methodOfDeath = MOD_DEMP2;
	//rww - Don't want this blockable, do we?
	missile->clipmask = MASK_SHOT;// | CONTENTS_LIGHTSABER;

	// we don't want it to ever bounce
	missile->bounceCount = 0;
}

static gentity_t *ent_list[MAX_GENTITIES];

void DEMP2_AltRadiusDamage( gentity_t *ent )
{
	float		frac = ( level.time - ent->bolt_Head ) / 800.0f; // / 1600.0f; // synchronize with demp2 effect
	float		dist, radius, fact;
	gentity_t	*gent;
	int			iEntityList[MAX_GENTITIES];
	gentity_t	*entityList[MAX_GENTITIES];
	gentity_t	*myOwner = NULL;
	int			numListedEntities, i, e;
	vec3_t		mins, maxs;
	vec3_t		v, dir;

	if (ent->r.ownerNum >= 0 &&
		ent->r.ownerNum < MAX_CLIENTS)
	{
		myOwner = &g_entities[ent->r.ownerNum];
	}

	if (!myOwner || !myOwner->inuse || !myOwner->client)
	{
		ent->think = G_FreeEntity;
		ent->nextthink = level.time;
		return;
	}

	frac *= frac * frac; // yes, this is completely ridiculous...but it causes the shell to grow slowly then "explode" at the end
	
	radius = frac * 200.0f; // 200 is max radius...the model is aprox. 100 units tall...the fx draw code mults. this by 2.

	fact = ent->count*0.6;

	if (fact < 1)
	{
		fact = 1;
	}

	radius *= fact;

	for ( i = 0 ; i < 3 ; i++ ) 
	{
		mins[i] = ent->r.currentOrigin[i] - radius;
		maxs[i] = ent->r.currentOrigin[i] + radius;
	}

	numListedEntities = trap_EntitiesInBox( mins, maxs, iEntityList, MAX_GENTITIES );

	i = 0;
	while (i < numListedEntities)
	{
		entityList[i] = &g_entities[iEntityList[i]];
		i++;
	}

	for ( e = 0 ; e < numListedEntities ; e++ ) 
	{
		gent = entityList[ e ];

		if ( !gent || !gent->takedamage || !gent->r.contents )
		{
			continue;
		}

		// find the distance from the edge of the bounding box
		for ( i = 0 ; i < 3 ; i++ ) 
		{
			if ( ent->r.currentOrigin[i] < gent->r.absmin[i] ) 
			{
				v[i] = gent->r.absmin[i] - ent->r.currentOrigin[i];
			} 
			else if ( ent->r.currentOrigin[i] > gent->r.absmax[i] ) 
			{
				v[i] = ent->r.currentOrigin[i] - gent->r.absmax[i];
			} 
			else 
			{
				v[i] = 0;
			}
		}

		// shape is an ellipsoid, so cut vertical distance in half`
		v[2] *= 0.5f;

		dist = VectorLength( v );

		if ( dist >= radius ) 
		{
			// shockwave hasn't hit them yet
			continue;
		}

		//if ( dist < ent->bolt_LArm )
		if (dist+(16*ent->count) < ent->bolt_LArm)
		{
			// shockwave has already hit this thing...
			continue;
		}

		VectorCopy( gent->r.currentOrigin, v );
		VectorSubtract( v, ent->r.currentOrigin, dir);

		// push the center of mass higher than the origin so players get knocked into the air more
		dir[2] += 12;

		if (gent != myOwner)
		{
			G_Damage( gent, myOwner, myOwner, dir, ent->r.currentOrigin, ent->damage, DAMAGE_DEATH_KNOCKBACK, ent->splashMethodOfDeath );
		}
	}

	// store the last fraction so that next time around we can test against those things that fall between that last point and where the current shockwave edge is
	ent->bolt_LArm = radius;

	if ( frac < 1.0f )
	{
		// shock is still happening so continue letting it expand
		ent->nextthink = level.time + 50;
	}
	else
	{ //don't just leave the entity around
		ent->think = G_FreeEntity;
		ent->nextthink = level.time;
	}
}

//---------------------------------------------------------
void DEMP2_AltDetonate( gentity_t *ent )
//---------------------------------------------------------
{
	gentity_t *efEnt;

	G_SetOrigin( ent, ent->r.currentOrigin );
	if (!ent->pos1[0] && !ent->pos1[1] && !ent->pos1[2])
	{ //don't play effect with a 0'd out directional vector
		ent->pos1[1] = 1;
	}
	//Let's just save ourself some bandwidth and play both the effect and sphere spawn in 1 event
	efEnt = G_PlayEffect( EFFECT_EXPLOSION_DEMP2ALT, ent->r.currentOrigin, ent->pos1 );

	if (efEnt)
	{
		efEnt->s.weapon = ent->count*2;
	}

	ent->bolt_Head = level.time;
	ent->bolt_LArm = 0;
	ent->nextthink = level.time + 50;
	ent->think = DEMP2_AltRadiusDamage;
	ent->s.eType = ET_GENERAL; // make us a missile no longer
}

//---------------------------------------------------------
static void WP_DEMP2_AltFire( gentity_t *ent )
//---------------------------------------------------------
{
	int		damage	= DEMP2_ALT_DAMAGE;
	int		count, origcount;
	float	fact;
	vec3_t	start, end;
	trace_t	tr;
	gentity_t *missile;

	VectorCopy( muzzle, start );

	VectorMA( start, DEMP2_ALT_RANGE, forward, end );

	count = ( level.time - ent->client->ps.weaponChargeTime ) / DEMP2_CHARGE_UNIT;

	origcount = count;

	if ( count < 1 )
	{
		count = 1;
	}
	else if ( count > 3 )
	{
		count = 3;
	}

	fact = count*0.8;
	if (fact < 1)
	{
		fact = 1;
	}
	damage *= fact;

	if (!origcount)
	{ //this was just a tap-fire
		damage = 1;
	}

	//damage *= ( 1 + ( count * ( count - 1 )));// yields damage of 12,36,84...gives a higher bonus for longer charge

	trap_Trace( &tr, start, NULL, NULL, end, ent->s.number, MASK_SHOT);

	// we treat the trace fraction like it's a time value, meaning that the shot can travel a whopping 4096 units in 1 second

	//missile = CreateMissile( start, forward, DEMP2_ALT_RANGE, tr.fraction * 1000/*time*/, ent, qtrue );
	missile = G_Spawn();
	G_SetOrigin(missile, tr.endpos);
	//rww - I guess it's rather pointless making it a missile anyway, at least for MP.

	VectorCopy( tr.plane.normal, missile->pos1 );

	missile->count = count;

	missile->classname = "demp2_alt_proj";
	missile->s.weapon = WP_DEMP2;

	missile->think = DEMP2_AltDetonate;
	missile->nextthink = level.time;

	missile->splashDamage = missile->damage = damage;
	missile->splashMethodOfDeath = missile->methodOfDeath = MOD_DEMP2;
	missile->splashRadius = DEMP2_ALT_SPLASHRADIUS;

	missile->r.ownerNum = ent->s.number;

	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to ever bounce
	missile->bounceCount = 0;
}

//---------------------------------------------------------
static void WP_FireDEMP2( gentity_t *ent, qboolean altFire )
//---------------------------------------------------------
{
	if ( altFire )
	{
		WP_DEMP2_AltFire( ent );
	}
	else
	{
		WP_DEMP2_MainFire( ent );
	}
}



/*
======================================================================

FLECHETTE

======================================================================
*/

//---------------------------------------------------------
static void WP_FlechetteMainFire( gentity_t *ent )
//---------------------------------------------------------
{
	vec3_t		fwd, angs;
	gentity_t	*missile;
	int i;

	for (i = 0; i < FLECHETTE_SHOTS; i++ )
	{
		vectoangles( forward, angs );

		if ( i == 0 )
		{
			// do nothing on the first shot, this one will hit the crosshairs
		}
		else
		{
			angs[PITCH] += crandom() * FLECHETTE_SPREAD;
			angs[YAW]	+= crandom() * FLECHETTE_SPREAD;
		}

		AngleVectors( angs, fwd, NULL, NULL );

		missile = CreateMissile( muzzle, fwd, FLECHETTE_VEL, 10000, ent, qfalse);

		missile->classname = "flech_proj";
		missile->s.weapon = WP_FLECHETTE;

		VectorSet( missile->r.maxs, FLECHETTE_SIZE, FLECHETTE_SIZE, FLECHETTE_SIZE );
		VectorScale( missile->r.maxs, -1, missile->r.mins );

		missile->damage = FLECHETTE_DAMAGE;
		missile->dflags = DAMAGE_DEATH_KNOCKBACK;// | DAMAGE_EXTRA_KNOCKBACK;
		missile->methodOfDeath = MOD_FLECHETTE;
		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

		// we don't want it to bounce forever
		missile->bounceCount = Q_irand(5,8);

		missile->s.eFlags |= EF_BOUNCE_SHRAPNEL;
	}
}

//---------------------------------------------------------
void prox_mine_think( gentity_t *ent )
//---------------------------------------------------------
{
	int			count, i;
	qboolean	blow = qfalse;

	// if it isn't time to auto-explode, do a small proximity check
	if ( ent->delay > level.time )
	{
		count = G_RadiusList( ent->r.currentOrigin, FLECHETTE_MINE_RADIUS_CHECK, ent, qtrue, ent_list );

		for ( i = 0; i < count; i++ )
		{
			if ( ent_list[i]->client && ent_list[i]->health > 0 && ent->activator && ent_list[i]->s.number != ent->activator->s.number )
			{
				blow = qtrue;
				break;
			}
		}
	}
	else
	{
		// well, we must die now
		blow = qtrue;
	}

	if ( blow )
	{
		//G_Sound( ent, G_SoundIndex( "sound/weapons/flechette/warning.wav" ));
		ent->think = laserTrapExplode;//thinkF_WP_Explode;
		ent->nextthink = level.time + 200;
	}
	else
	{
		// we probably don't need to do this thinking logic very often...maybe this is fast enough?
		ent->nextthink = level.time + 500;
	}
}

//-----------------------------------------------------------------------------
static void WP_TraceSetStart( gentity_t *ent, vec3_t start, vec3_t mins, vec3_t maxs )
//-----------------------------------------------------------------------------
{
	//make sure our start point isn't on the other side of a wall
	trace_t	tr;
	vec3_t	entMins;
	vec3_t	entMaxs;

	VectorAdd( ent->r.currentOrigin, ent->r.mins, entMins );
	VectorAdd( ent->r.currentOrigin, ent->r.maxs, entMaxs );

	if ( G_BoxInBounds( start, mins, maxs, entMins, entMaxs ) )
	{
		return;
	}

	if ( !ent->client )
	{
		return;
	}

	trap_Trace( &tr, ent->client->ps.origin, mins, maxs, start, ent->s.number, MASK_SOLID|CONTENTS_SHOTCLIP );

	if ( tr.startsolid || tr.allsolid )
	{
		return;
	}

	if ( tr.fraction < 1.0f )
	{
		VectorCopy( tr.endpos, start );
	}
}

void WP_ExplosiveDie(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod)
{
	laserTrapExplode(self);
}

//----------------------------------------------
void WP_flechette_alt_blow( gentity_t *ent )
//----------------------------------------------
{
	/*BG_EvaluateTrajectory( &ent->s.pos, level.time, ent->r.currentOrigin ); // Not sure if this is even necessary, but correct origins are cool?

	G_RadiusDamage( ent->r.currentOrigin, &g_entities[ent->r.ownerNum], ent->splashDamage, ent->splashRadius, NULL, MOD_FLECHETTE_ALT_SPLASH );
	G_PlayEffect( "flechette/alt_blow", ent->currentOrigin );
	G_FreeEntity( ent );*/

	ent->s.pos.trDelta[0] = 1;
	ent->s.pos.trDelta[1] = 0;
	ent->s.pos.trDelta[2] = 0;

	laserTrapExplode(ent);
}

//------------------------------------------------------------------------------
static void WP_CreateFlechetteBouncyThing( vec3_t start, vec3_t fwd, gentity_t *self )
//------------------------------------------------------------------------------
{
	gentity_t	*missile = CreateMissile( start, fwd, 700 + random() * 700, 1500 + random() * 2000, self, qtrue );
	
	missile->think = WP_flechette_alt_blow;

	missile->activator = self;

	missile->s.weapon = WP_FLECHETTE;
	missile->classname = "flech_alt";
	missile->mass = 4;

	// How 'bout we give this thing a size...
	VectorSet( missile->r.mins, -3.0f, -3.0f, -3.0f );
	VectorSet( missile->r.maxs, 3.0f, 3.0f, 3.0f );
	missile->clipmask = MASK_SHOT;

	missile->touch = touch_NULL;

	// normal ones bounce, alt ones explode on impact
	missile->s.pos.trType = TR_GRAVITY;

	missile->s.eFlags |= (EF_BOUNCE_HALF|EF_ALT_FIRING);

	missile->bounceCount = 50;

	missile->damage = FLECHETTE_ALT_DAMAGE;
	missile->dflags = 0;
	missile->splashDamage = FLECHETTE_ALT_SPLASH_DAM;
	missile->splashRadius = FLECHETTE_ALT_SPLASH_RAD;

	missile->r.svFlags = SVF_USE_CURRENT_ORIGIN;

	missile->methodOfDeath = MOD_FLECHETTE_ALT_SPLASH;
	missile->splashMethodOfDeath = MOD_FLECHETTE_ALT_SPLASH;
	//missile->splashMethodOfDeath = MOD_UNKNOWN;//MOD_THERMAL_SPLASH;

	VectorCopy( start, missile->pos2 );
}

//---------------------------------------------------------
static void WP_FlechetteAltFire( gentity_t *self )
//---------------------------------------------------------
{
	vec3_t 	dir, fwd, start, angs;
	int i;

	vectoangles( forward, angs );
	VectorCopy( muzzle, start );

	WP_TraceSetStart( self, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall

	for ( i = 0; i < 2; i++ )
	{
		VectorCopy( angs, dir );

		dir[PITCH] -= random() * 4 + 8; // make it fly upwards
		dir[YAW] += crandom() * 2;
		AngleVectors( dir, fwd, NULL, NULL );

		WP_CreateFlechetteBouncyThing( start, fwd, self );
	}
}

//---------------------------------------------------------
static void WP_FireFlechette( gentity_t *ent, qboolean altFire )
//---------------------------------------------------------
{
	if ( altFire )
	{
		//WP_FlechetteProxMine( ent );
		WP_FlechetteAltFire(ent);
	}
	else
	{
		WP_FlechetteMainFire( ent );
	}
}



/*
======================================================================

ROCKET LAUNCHER

======================================================================
*/

//---------------------------------------------------------
void rocketThink( gentity_t *ent )
//---------------------------------------------------------
{
	vec3_t newdir, targetdir, 
			up={0,0,1}, right; 
	vec3_t	org;
	float dot, dot2, dis;
	int i;
	float vel = ROCKET_VELOCITY;

	if (!ent->enemy || !ent->enemy->client || ent->enemy->health < 1)
	{
		ent->nextthink = level.time + 10000;
		ent->think = G_FreeEntity;
		return;
	}

	if ( ent->enemy && ent->enemy->inuse )
	{	
		VectorCopy( ent->enemy->r.currentOrigin, org );
		org[2] += (ent->enemy->r.mins[2] + ent->enemy->r.maxs[2]) * 0.5f;

		VectorSubtract( org, ent->r.currentOrigin, targetdir );
		VectorNormalize( targetdir );

		// Now the rocket can't do a 180 in space, so we'll limit the turn to about 45 degrees.
		dot = DotProduct( targetdir, ent->movedir );

		// a dot of 1.0 means right-on-target.
		if ( dot < 0.0f )
		{	
			// Go in the direction opposite, start a 180.
			CrossProduct( ent->movedir, up, right );
			dot2 = DotProduct( targetdir, right );

			if ( dot2 > 0 )
			{	
				// Turn 45 degrees right.
				VectorMA( ent->movedir, 0.4f, right, newdir );
			}
			else
			{	
				// Turn 45 degrees left.
				VectorMA(ent->movedir, -0.4f, right, newdir);
			}

			// Yeah we've adjusted horizontally, but let's split the difference vertically, so we kinda try to move towards it.
			newdir[2] = ( targetdir[2] + ent->movedir[2] ) * 0.5;

			// let's also slow down a lot
			vel *= 0.5f;
		}
		else if ( dot < 0.70f )
		{	
			// Still a bit off, so we turn a bit softer
			VectorMA( ent->movedir, 0.5f, targetdir, newdir );
		}
		else
		{	
			// getting close, so turn a bit harder
			VectorMA( ent->movedir, 0.9f, targetdir, newdir );
		}

		// add crazy drunkenness
		for (i = 0; i < 3; i++ )
		{
			newdir[i] += crandom() * ent->random * 0.25f;
		}

		// decay the randomness
		ent->random *= 0.9f;

		// Try to crash into the ground if we get close enough to do splash damage
		dis = Distance( ent->r.currentOrigin, org );

		if ( dis < 128 )
		{
			// the closer we get, the more we push the rocket down, heh heh.
			newdir[2] -= (1.0f - (dis / 128.0f)) * 0.6f;
		}

		VectorNormalize( newdir );

		VectorScale( newdir, vel * 0.5f, ent->s.pos.trDelta );
		VectorCopy( newdir, ent->movedir );
		SnapVector( ent->s.pos.trDelta );			// save net bandwidth
		VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );
		ent->s.pos.trTime = level.time;
	}

	ent->nextthink = level.time + ROCKET_ALT_THINK_TIME;	// Nothing at all spectacular happened, continue.
	return;
}

//---------------------------------------------------------
static void WP_FireRocket( gentity_t *ent, qboolean altFire )
//---------------------------------------------------------
{
	int	damage	= ROCKET_DAMAGE;
	int	vel = ROCKET_VELOCITY;
	int dif = 0;
	float rTime;
	gentity_t *missile;

	if ( altFire )
	{
		vel *= 0.5f;
	}

	missile = CreateMissile( muzzle, forward, vel, 10000, ent, altFire );

	if (ent->client && ent->client->ps.rocketLockIndex != MAX_CLIENTS)
	{
		rTime = ent->client->ps.rocketLockTime;

		if (rTime == -1)
		{
			rTime = ent->client->ps.rocketLastValidTime;
		}
		dif = ( level.time - rTime ) / ( 1200.0f / 16.0f );

		if (dif < 0)
		{
			dif = 0;
		}

		//It's 10 even though it locks client-side at 8, because we want them to have a sturdy lock first, and because there's a slight difference in time between server and client
		if ( dif >= 10/* || random() * dif > 2 || random() > 0.97f*/ && rTime != -1 )
		{
			missile->enemy = &g_entities[ent->client->ps.rocketLockIndex];

			if (missile->enemy && missile->enemy->client && missile->enemy->health > 0 && !OnSameTeam(ent, missile->enemy))
			{ //if enemy became invalid, died, or is on the same team, then don't seek it
				missile->think = rocketThink;
				missile->nextthink = level.time + ROCKET_ALT_THINK_TIME;
			}
		}

		ent->client->ps.rocketLockIndex = MAX_CLIENTS;
		ent->client->ps.rocketLockTime = 0;
		ent->client->ps.rocketTargetTime = 0;
	}

	missile->classname = "rocket_proj";
	missile->s.weapon = WP_ROCKET_LAUNCHER;

	// NOTENOTE No mass yet.
//	missile->mass = 10;

	// Make it easier to hit things
	VectorSet( missile->r.maxs, ROCKET_SIZE, ROCKET_SIZE, ROCKET_SIZE );
	VectorScale( missile->r.maxs, -1, missile->r.mins );

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	if (altFire)
	{
		missile->methodOfDeath = MOD_ROCKET_HOMING;
		missile->splashMethodOfDeath = MOD_ROCKET_HOMING_SPLASH;
	}
	else
	{
		missile->methodOfDeath = MOD_ROCKET;
		missile->splashMethodOfDeath = MOD_ROCKET_SPLASH;
	}
	
	//rww - We don't want rockets to be deflected, do we?
	missile->clipmask = MASK_SHOT;// | CONTENTS_LIGHTSABER;
	missile->splashDamage = ROCKET_SPLASH_DAMAGE;
	missile->splashRadius = ROCKET_SPLASH_RADIUS;

	// we don't want it to ever bounce
	missile->bounceCount = 0;
}

/*
======================================================================

THERMAL DETONATOR

======================================================================
*/

#define TD_DAMAGE			70 //only do 70 on a direct impact
#define TD_SPLASH_RAD		128
#define TD_SPLASH_DAM		90
#define TD_VELOCITY			900
#define TD_MIN_CHARGE		0.15f
#define TD_TIME				3000//6000
#define TD_ALT_TIME			3000

#define TD_ALT_DAMAGE		60//100
#define TD_ALT_SPLASH_RAD	128
#define TD_ALT_SPLASH_DAM	50//90
#define TD_ALT_VELOCITY		600
#define TD_ALT_MIN_CHARGE	0.15f
#define TD_ALT_TIME			3000

void thermalThinkStandard(gentity_t *ent);

//---------------------------------------------------------
void thermalDetonatorExplode( gentity_t *ent )
//---------------------------------------------------------
{
	if ( !ent->count )
	{
		G_Sound( ent, CHAN_VOICE, G_SoundIndex( "sound/weapons/thermal/warning.wav" ) );
		ent->count = 1;
		ent->bolt_Head = level.time + 500;
		ent->think = thermalThinkStandard;
		ent->nextthink = level.time;
		ent->r.svFlags |= SVF_BROADCAST;//so everyone hears/sees the explosion?
	}
	else
	{
		vec3_t	origin;
		vec3_t	dir={0,0,1};

		BG_EvaluateTrajectory( &ent->s.pos, level.time, origin );
		origin[2] += 8;
		SnapVector( origin );
		G_SetOrigin( ent, origin );

//		VectorSet( pos, ent->r.currentOrigin[0], ent->r.currentOrigin[1], ent->r.currentOrigin[2] + 8 );
		ent->s.eType = ET_GENERAL;
		G_AddEvent( ent, EV_MISSILE_MISS, DirToByte( dir ) );
		ent->freeAfterEvent = qtrue;

		if (G_RadiusDamage( ent->r.currentOrigin, ent->parent,  ent->splashDamage, ent->splashRadius, 
				ent, ent->splashMethodOfDeath))
		{
			g_entities[ent->r.ownerNum].client->accuracy_hits++;
		}

		trap_LinkEntity( ent );
	}
}

void thermalThinkStandard(gentity_t *ent)
{
	if (ent->bolt_Head < level.time)
	{
		ent->think = thermalDetonatorExplode;
		ent->nextthink = level.time;
		return;
	}

	G_RunObject(ent);
	ent->nextthink = level.time;
}

//---------------------------------------------------------
gentity_t *WP_FireThermalDetonator( gentity_t *ent, qboolean altFire )
//---------------------------------------------------------
{
	gentity_t	*bolt;
	vec3_t		dir, start;
	float chargeAmount = 1.0f; // default of full charge
	
	VectorCopy( forward, dir );
	VectorCopy( muzzle, start );

	bolt = G_Spawn();
	
	bolt->physicsObject = qtrue;

	bolt->classname = "thermal_detonator";
	bolt->think = thermalThinkStandard;
	bolt->nextthink = level.time;
	bolt->touch = touch_NULL;
//	bolt->mass = 10;		// NOTENOTE No mass implementation yet

	// How 'bout we give this thing a size...
	VectorSet( bolt->r.mins, -3.0f, -3.0f, -3.0f );
	VectorSet( bolt->r.maxs, 3.0f, 3.0f, 3.0f );
	bolt->clipmask = MASK_SHOT;

	W_TraceSetStart( ent, start, bolt->r.mins, bolt->r.maxs );//make sure our start point isn't on the other side of a wall

	if ( ent->client )
	{
		chargeAmount = level.time - ent->client->ps.weaponChargeTime;
	}

	// get charge amount
	chargeAmount = chargeAmount / (float)TD_VELOCITY;

	if ( chargeAmount > 1.0f )
	{
		chargeAmount = 1.0f;
	}
	else if ( chargeAmount < TD_MIN_CHARGE )
	{
		chargeAmount = TD_MIN_CHARGE;
	}

	// normal ones bounce, alt ones explode on impact
	bolt->bolt_Head = level.time + TD_TIME; // How long 'til she blows
	bolt->s.pos.trType = TR_GRAVITY;
	bolt->parent = ent;
	bolt->r.ownerNum = ent->s.number;
	VectorScale( dir, TD_VELOCITY * chargeAmount, bolt->s.pos.trDelta );

	if ( ent->health >= 0 )
	{
		bolt->s.pos.trDelta[2] += 120;
	}

	if ( !altFire )
	{
		//bolt->alt_fire = qtrue;
		bolt->s.eFlags |= EF_BOUNCE_HALF;
	}

	bolt->s.loopSound = G_SoundIndex( "sound/weapons/thermal/thermloop.wav" );

	bolt->damage = TD_DAMAGE;
	bolt->dflags = 0;
	bolt->splashDamage = TD_SPLASH_DAM;
	bolt->splashRadius = TD_SPLASH_RAD;

	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_THERMAL;

	bolt->methodOfDeath = MOD_THERMAL;
	bolt->splashMethodOfDeath = MOD_THERMAL_SPLASH;

	bolt->s.pos.trTime = level.time;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
	VectorCopy (start, bolt->r.currentOrigin);

	VectorCopy( start, bolt->pos2 );

	bolt->bounceCount = -5;

	return bolt;
}

gentity_t *WP_DropThermal( gentity_t *ent )
{
	AngleVectors( ent->client->ps.viewangles, forward, right, up );
	return (WP_FireThermalDetonator( ent, qfalse ));
}


/*
======================================================================

LASER TRAP / TRIP MINE

======================================================================
*/
#define LT_DAMAGE			100
#define LT_SPLASH_RAD		256.0f
#define LT_SPLASH_DAM		105
#define LT_VELOCITY			900.0f
#define LT_SIZE				1.5f
#define LT_ALT_TIME			2000
#define	LT_ACTIVATION_DELAY	1000
#define	LT_DELAY_TIME		50

void laserTrapExplode( gentity_t *self )
{
	vec3_t v;
	//FIXME: damage some along line?
	self->takedamage = qfalse;

	if (self->activator)
	{
		G_RadiusDamage( self->r.currentOrigin, self->activator, self->splashDamage, self->splashRadius, self, MOD_TRIP_MINE_SPLASH/*MOD_LT_SPLASH*/ );
	}
	//FIXME: clear me from owner's list of tripmines?

	if (self->s.weapon != WP_FLECHETTE)
	{
		G_AddEvent( self, EV_MISSILE_MISS, 0);
	}

	VectorCopy(self->s.pos.trDelta, v);
	//Explode outward from the surface

	if (self->s.time == -2)
	{
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
	}

	if (self->s.weapon == WP_FLECHETTE)
	{
		G_PlayEffect(EFFECT_EXPLOSION_FLECHETTE, self->r.currentOrigin, v);
	}
	else
	{
		G_PlayEffect(EFFECT_EXPLOSION_TRIPMINE, self->r.currentOrigin, v);
	}

	self->think = G_FreeEntity;
	self->nextthink = level.time;
}

void laserTrapDelayedExplode( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath )
{
	self->enemy = attacker;
	self->think = laserTrapExplode;
	self->nextthink = level.time + FRAMETIME;
	self->takedamage = qfalse;
	if ( attacker && !attacker->s.number )
	{
		//less damage when shot by player
		self->splashDamage /= 3;
		self->splashRadius /= 3;
		//FIXME: different effect?
	}
}

void touchLaserTrap( gentity_t *ent, gentity_t *other, trace_t *trace )
{
	// if the guy that touches this grenade can take damage, he's about to.
	//if ( other->takedamage )
	if (other && other->s.number < 1022)
	{ //just explode if we hit any entity. This way we don't have things happening like tripmines floating
	  //in the air after getting stuck to a moving door
		if ( ent->activator != other )
		{
			ent->touch = 0;
			ent->nextthink = level.time + FRAMETIME;
			ent->think = laserTrapExplode;
			VectorCopy(trace->plane.normal, ent->s.pos.trDelta);
		}
	}
	else
	{
		ent->touch = 0;
		if (trace->entityNum != ENTITYNUM_NONE)
		{
			ent->enemy = &g_entities[trace->entityNum];
		}
		laserTrapStick(ent, trace->endpos, trace->plane.normal);
	}
}

void laserTrapThink ( gentity_t *ent )
{
	gentity_t	*traceEnt;
	vec3_t		end;
	trace_t		tr;

	//G_RunObject(ent);
	trap_LinkEntity(ent);

	//turn on the beam effect
	if ( !(ent->s.eFlags&EF_FIRING) )
	{//arm me
		G_Sound( ent, CHAN_VOICE, G_SoundIndex( "sound/weapons/laser_trap/warning.wav" ) );
		ent->s.eFlags |= EF_FIRING;
	}
	ent->think = laserTrapThink;
	ent->nextthink = level.time + FRAMETIME;

	// Find the main impact point
	VectorMA ( ent->s.pos.trBase, 1024, ent->movedir, end );
	trap_Trace ( &tr, ent->r.currentOrigin, NULL, NULL, end, ent->s.number, MASK_SHOT);
	
	traceEnt = &g_entities[ tr.entityNum ];

	ent->s.time = -1; //let all clients know to draw a beam from this guy

	if ( traceEnt->client || tr.startsolid )
	{
		//go boom
		ent->touch = 0;
		ent->nextthink = level.time + LT_DELAY_TIME;
		ent->think = laserTrapExplode;
	}
}

void laserTrapStick( gentity_t *ent, vec3_t endpos, vec3_t normal )
{
	//vec3_t	org;

	// Back away from the wall
	//VectorMA( endpos, -1, normal, org );
	G_SetOrigin( ent, endpos );//org );
	VectorCopy( normal, ent->pos1 );

	VectorClear( ent->s.apos.trDelta );
	// This will orient the object to face in the direction of the normal
	VectorCopy( normal, ent->s.pos.trDelta );
	//VectorScale( normal, -1, ent->s.pos.trDelta );
	ent->s.pos.trTime = level.time;
	
	
	//This does nothing, cg_missile makes assumptions about direction of travel controlling angles
	vectoangles( normal, ent->s.apos.trBase );
	VectorClear( ent->s.apos.trDelta );
	ent->s.apos.trType = TR_STATIONARY;
	VectorCopy( ent->s.apos.trBase, ent->s.angles );
	VectorCopy( ent->s.angles, ent->r.currentAngles );
	

	G_Sound( ent, CHAN_VOICE, G_SoundIndex( "sound/weapons/laser_trap/stick.wav" ) );
	if ( ent->count )
	{//a tripwire
		//add draw line flag
		VectorCopy( normal, ent->movedir );
		ent->think = laserTrapThink;
		ent->nextthink = level.time + LT_ACTIVATION_DELAY;//delay the activation
		ent->touch = touch_NULL;
		//make it shootable
		ent->takedamage = qtrue;
		ent->health = 5;
		ent->die = laserTrapDelayedExplode;

		//shove the box through the wall
		VectorSet( ent->r.mins, -LT_SIZE*2, -LT_SIZE*2, -LT_SIZE*2 );
		VectorSet( ent->r.maxs, LT_SIZE*2, LT_SIZE*2, LT_SIZE*2 );

		//so that the owner can blow it up with projectiles
		ent->r.svFlags |= SVF_OWNERNOTSHARED;
	}
	else
	{
		ent->touch = touchLaserTrap;
		ent->think = laserTrapExplode;
		ent->nextthink = level.time + LT_ALT_TIME; // How long 'til she blows
	}
}

void TrapThink(gentity_t *ent)
{
	ent->nextthink = level.time + 50;

	G_RunObject(ent);
}

void CreateLaserTrap( gentity_t *laserTrap, vec3_t start, gentity_t *owner )
{
	laserTrap->classname = "laserTrap";
	laserTrap->s.eFlags = EF_BOUNCE_HALF;
	laserTrap->s.eFlags |= EF_MISSILE_STICK;
	laserTrap->splashDamage = LT_SPLASH_DAM;//*2;
	laserTrap->splashRadius = LT_SPLASH_RAD;//*2;
	laserTrap->damage = LT_DAMAGE;//*DMG_VAR;
	laserTrap->methodOfDeath = MOD_TRIP_MINE_SPLASH;//MOD_TRIP_WIRE;
	laserTrap->splashMethodOfDeath = MOD_TRIP_MINE_SPLASH;//MOD_TRIP_WIRE;
	laserTrap->s.eType = ET_GENERAL;
	laserTrap->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	laserTrap->s.weapon = WP_TRIP_MINE;
	laserTrap->s.pos.trType = TR_GRAVITY;
	laserTrap->r.contents = MASK_SHOT;
	laserTrap->parent = owner;
	laserTrap->activator = owner;
	laserTrap->r.ownerNum = owner->s.number;
	VectorSet( laserTrap->r.mins, -LT_SIZE, -LT_SIZE, -LT_SIZE );
	VectorSet( laserTrap->r.maxs, LT_SIZE, LT_SIZE, LT_SIZE );
	laserTrap->clipmask = MASK_SHOT;
	laserTrap->s.solid = 2;
	laserTrap->s.modelindex = G_ModelIndex( "models/weapons2/laser_trap/laser_trap_w.glm" );
	laserTrap->s.modelGhoul2 = 1;
	laserTrap->s.g2radius = 40;

	laserTrap->s.genericenemyindex = owner->s.number+1024;

	laserTrap->health = 1;

	laserTrap->s.time = 0;

	laserTrap->s.pos.trTime = level.time;		// move a bit on the very first frame
	VectorCopy( start, laserTrap->s.pos.trBase );
	SnapVector( laserTrap->s.pos.trBase );			// save net bandwidth
	
	SnapVector( laserTrap->s.pos.trDelta );			// save net bandwidth
	VectorCopy (start, laserTrap->r.currentOrigin);

	laserTrap->s.apos.trType = TR_GRAVITY;
	laserTrap->s.apos.trTime = level.time;
	laserTrap->s.apos.trBase[YAW] = rand()%360;
	laserTrap->s.apos.trBase[PITCH] = rand()%360;
	laserTrap->s.apos.trBase[ROLL] = rand()%360;

	if (rand()%10 < 5)
	{
		laserTrap->s.apos.trBase[YAW] = -laserTrap->s.apos.trBase[YAW];
	}

	VectorCopy( start, laserTrap->pos2 );
	laserTrap->touch = touchLaserTrap;
	laserTrap->think = TrapThink;
	laserTrap->nextthink = level.time + 50;
}

void WP_PlaceLaserTrap( gentity_t *ent, qboolean alt_fire )
{
	gentity_t	*laserTrap;
	gentity_t	*found = NULL;
	vec3_t		dir, start;
	int			trapcount = 0;
	int			foundLaserTraps[MAX_GENTITIES] = {ENTITYNUM_NONE};
	int			trapcount_org;
	int			lowestTimeStamp;
	int			removeMe;
	int			i;

	//FIXME: surface must be within 64

	VectorCopy( forward, dir );
	VectorCopy( muzzle, start );

	laserTrap = G_Spawn();
	
	//limit to 10 placed at any one time
	//see how many there are now
	while ( (found = G_Find( found, FOFS(classname), "laserTrap" )) != NULL )
	{
		if ( found->parent != ent )
		{
			continue;
		}
		foundLaserTraps[trapcount++] = found->s.number;
	}
	//now remove first ones we find until there are only 9 left
	found = NULL;
	trapcount_org = trapcount;
	lowestTimeStamp = level.time;
	while ( trapcount > 9 )
	{
		removeMe = -1;
		for ( i = 0; i < trapcount_org; i++ )
		{
			if ( foundLaserTraps[i] == ENTITYNUM_NONE )
			{
				continue;
			}
			found = &g_entities[foundLaserTraps[i]];
			if ( laserTrap && found->setTime < lowestTimeStamp )
			{
				removeMe = i;
				lowestTimeStamp = found->setTime;
			}
		}
		if ( removeMe != -1 )
		{
			//remove it... or blow it?
			if ( &g_entities[foundLaserTraps[removeMe]] == NULL )
			{
				break;
			}
			else
			{
				G_FreeEntity( &g_entities[foundLaserTraps[removeMe]] );
			}
			foundLaserTraps[removeMe] = ENTITYNUM_NONE;
			trapcount--;
		}
		else
		{
			break;
		}
	}

	//now make the new one
	CreateLaserTrap( laserTrap, start, ent );

	//set player-created-specific fields
	laserTrap->setTime = level.time;//remember when we placed it

	if (!alt_fire)
	{//tripwire
		laserTrap->count = 1;
	}

	//move it
	laserTrap->s.pos.trType = TR_GRAVITY;

	if (alt_fire)
	{
		VectorScale( dir, 512, laserTrap->s.pos.trDelta );
	}
	else
	{
		VectorScale( dir, 256, laserTrap->s.pos.trDelta );
	}

	trap_LinkEntity(laserTrap);
}


/*
======================================================================

DET PACK

======================================================================
*/
void VectorNPos(vec3_t in, vec3_t out)
{
	if (in[0] < 0) { out[0] = -in[0]; } else { out[0] = in[0]; }
	if (in[1] < 0) { out[1] = -in[1]; } else { out[1] = in[1]; }
	if (in[2] < 0) { out[2] = -in[2]; } else { out[2] = in[2]; }
}

void DetPackBlow(gentity_t *self);

void charge_stick (gentity_t *self, gentity_t *other, trace_t *trace)
{
	gentity_t	*tent;

	if (other && other->s.number < 1022 &&
		(other->client || !other->s.weapon))
	{
		vec3_t vNor, tN;

		VectorCopy(trace->plane.normal, vNor);
		VectorNormalize(vNor);
		VectorNPos(self->s.pos.trDelta, tN);
		self->s.pos.trDelta[0] += vNor[0]*(tN[0]*(((float)Q_irand(1, 10))*0.1));
		self->s.pos.trDelta[1] += vNor[1]*(tN[1]*(((float)Q_irand(1, 10))*0.1));
		self->s.pos.trDelta[2] += vNor[1]*(tN[2]*(((float)Q_irand(1, 10))*0.1));

		vectoangles(vNor, self->s.angles);
		vectoangles(vNor, self->s.apos.trBase);
		self->touch = charge_stick;
		return;
	}
	else if (other && other->s.number < 1022)
	{
		vec3_t v;

		self->touch = 0;
		self->think = 0;
		self->nextthink = 0;

		self->takedamage = qfalse;

		VectorClear(self->s.apos.trDelta);
		self->s.apos.trType = TR_STATIONARY;

		G_RadiusDamage( self->r.currentOrigin, self->parent, self->splashDamage, self->splashRadius, self, MOD_DET_PACK_SPLASH );
		VectorCopy(trace->plane.normal, v);
		VectorCopy(v, self->pos2);
		self->count = -1;
		G_PlayEffect(EFFECT_EXPLOSION_DETPACK, self->r.currentOrigin, v);

		self->think = G_FreeEntity;
		self->nextthink = level.time;
		return;
	}

	//self->s.eType = ET_GENERAL;
	//FIXME: once on ground, shouldn't explode if touched by someone?
	//FIXME: if owner touches it again, pick it up?  Or if he "uses" it?
	self->touch = 0;
	self->think = DetPackBlow;
	self->nextthink = level.time + 30000;

	VectorClear(self->s.apos.trDelta);
	self->s.apos.trType = TR_STATIONARY;

	self->s.pos.trType = TR_STATIONARY;
	VectorCopy( self->r.currentOrigin, self->s.origin );
	VectorCopy( self->r.currentOrigin, self->s.pos.trBase );
	VectorClear( self->s.pos.trDelta );

	VectorClear( self->s.apos.trDelta );

	VectorNormalize(trace->plane.normal);

	vectoangles(trace->plane.normal, self->s.angles);
	VectorCopy(self->s.angles, self->r.currentAngles );
	VectorCopy(self->s.angles, self->s.apos.trBase);

	VectorCopy(trace->plane.normal, self->pos2);
	self->count = -1;

	G_Sound(self, CHAN_VOICE, G_SoundIndex("sound/weapons/detpack/stick.wav"));
		
	tent = G_TempEntity( self->r.currentOrigin, EV_MISSILE_MISS );
	tent->s.weapon = 0;
	tent->parent = self;
	tent->r.ownerNum = self->s.number;

	//so that the owner can blow it up with projectiles
	self->r.svFlags |= SVF_OWNERNOTSHARED;
}

void DetPackBlow(gentity_t *self)
{
	vec3_t v;

	//self->touch = NULL;
	self->pain = 0;
	self->die = 0;
	self->takedamage = qfalse;

	G_RadiusDamage( self->r.currentOrigin, self->parent, self->splashDamage, self->splashRadius, self, MOD_DET_PACK_SPLASH );
	v[0] = 0;
	v[1] = 0;
	v[2] = 1;

	if (self->count == -1)
	{
		VectorCopy(self->pos2, v);
	}

	G_PlayEffect(EFFECT_EXPLOSION_DETPACK, self->r.currentOrigin, v);

	self->think = G_FreeEntity;
	self->nextthink = level.time;
}

void DetPackPain(gentity_t *self, gentity_t *attacker, int damage)
{
	self->think = DetPackBlow;
	self->nextthink = level.time + Q_irand(50, 100);
	self->takedamage = qfalse;
}

void DetPackDie(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod)
{
	self->think = DetPackBlow;
	self->nextthink = level.time + Q_irand(50, 100);
	self->takedamage = qfalse;
}

void drop_charge (gentity_t *self, vec3_t start, vec3_t dir) 
{
	gentity_t	*bolt;

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "detpack";
	bolt->nextthink = level.time + FRAMETIME;
	bolt->think = G_RunObject;
	bolt->s.eType = ET_GENERAL;
	bolt->s.g2radius = 100;
	bolt->s.modelGhoul2 = 1;
	bolt->s.modelindex = G_ModelIndex("models/weapons2/detpack/det_pack_proj.glm"); // w.md3");

	//bolt->playerTeam = self->client->playerTeam;
	bolt->parent = self;
	bolt->r.ownerNum = self->s.number;
	bolt->damage = 100;
	bolt->splashDamage = 200;
	bolt->splashRadius = 200;
	bolt->methodOfDeath = MOD_DET_PACK_SPLASH;//MOD_EXPLOSIVE;
	bolt->splashMethodOfDeath = MOD_DET_PACK_SPLASH;//MOD_EXPLOSIVE_SPLASH;
	bolt->clipmask = MASK_SHOT;
	bolt->s.solid = 2;
	bolt->r.contents = MASK_SHOT;
	bolt->touch = charge_stick;

	bolt->physicsObject = qtrue;

	bolt->s.genericenemyindex = self->s.number+1024;
	//rww - so client prediction knows we own this and won't hit it

//	VectorSet( bolt->r.mins, -3, -3, -3 );
//	VectorSet( bolt->r.maxs, 3, 3, 3 );
	VectorSet( bolt->r.mins, -2, -2, -2 );
	VectorSet( bolt->r.maxs, 2, 2, 2 );

	bolt->health = 1;
	bolt->takedamage = qtrue;
	bolt->pain = DetPackPain;
	bolt->die = DetPackDie;

	bolt->s.weapon = WP_DET_PACK;

	bolt->setTime = level.time;

	G_SetOrigin(bolt, start);
	bolt->s.pos.trType = TR_GRAVITY;
	VectorCopy( start, bolt->s.pos.trBase );
	VectorScale(dir, 300, bolt->s.pos.trDelta );
	bolt->s.pos.trTime = level.time;

	bolt->s.apos.trType = TR_GRAVITY;
	bolt->s.apos.trTime = level.time;
	bolt->s.apos.trBase[YAW] = rand()%360;
	bolt->s.apos.trBase[PITCH] = rand()%360;
	bolt->s.apos.trBase[ROLL] = rand()%360;

	if (rand()%10 < 5)
	{
		bolt->s.apos.trBase[YAW] = -bolt->s.apos.trBase[YAW];
	}

	vectoangles(dir, bolt->s.angles);
	VectorCopy(bolt->s.angles, bolt->s.apos.trBase);
	VectorSet(bolt->s.apos.trDelta, 300, 0, 0 );
	bolt->s.apos.trTime = level.time;

	trap_LinkEntity(bolt);
}

void BlowDetpacks(gentity_t *ent)
{
	gentity_t *found = NULL;

	if ( ent->client->ps.hasDetPackPlanted )
	{
		while ( (found = G_Find( found, FOFS(classname), "detpack") ) != NULL )
		{//loop through all ents and blow the crap out of them!
			if ( found->parent == ent )
			{
				VectorCopy( found->r.currentOrigin, found->s.origin );
				found->think = DetPackBlow;
				found->nextthink = level.time + 100 + random() * 200;
				G_Sound( found, CHAN_BODY, G_SoundIndex("sound/weapons/detpack/warning.wav") );
			}
		}
		ent->client->ps.hasDetPackPlanted = qfalse;
	}
}

qboolean CheatsOn(void) 
{
	if ( !g_cheats.integer )
	{
		return qfalse;
	}
	return qtrue;
}

void WP_DropDetPack( gentity_t *ent, qboolean alt_fire )
{
	gentity_t	*found = NULL;
	int			trapcount = 0;
	int			foundDetPacks[MAX_GENTITIES] = {ENTITYNUM_NONE};
	int			trapcount_org;
	int			lowestTimeStamp;
	int			removeMe;
	int			i;

	if ( !ent || !ent->client )
	{
		return;
	}

	//limit to 10 placed at any one time
	//see how many there are now
	while ( (found = G_Find( found, FOFS(classname), "detpack" )) != NULL )
	{
		if ( found->parent != ent )
		{
			continue;
		}
		foundDetPacks[trapcount++] = found->s.number;
	}
	//now remove first ones we find until there are only 9 left
	found = NULL;
	trapcount_org = trapcount;
	lowestTimeStamp = level.time;
	while ( trapcount > 9 )
	{
		removeMe = -1;
		for ( i = 0; i < trapcount_org; i++ )
		{
			if ( foundDetPacks[i] == ENTITYNUM_NONE )
			{
				continue;
			}
			found = &g_entities[foundDetPacks[i]];
			if ( found->setTime < lowestTimeStamp )
			{
				removeMe = i;
				lowestTimeStamp = found->setTime;
			}
		}
		if ( removeMe != -1 )
		{
			//remove it... or blow it?
			if ( &g_entities[foundDetPacks[removeMe]] == NULL )
			{
				break;
			}
			else
			{
				if (!CheatsOn())
				{ //Let them have unlimited if cheats are enabled
					G_FreeEntity( &g_entities[foundDetPacks[removeMe]] );
				}
			}
			foundDetPacks[removeMe] = ENTITYNUM_NONE;
			trapcount--;
		}
		else
		{
			break;
		}
	}

	if ( alt_fire  )
	{
		BlowDetpacks(ent);
	}
	else
	{
		AngleVectors( ent->client->ps.viewangles, forward, right, up );

		CalcMuzzlePoint( ent, forward, right, up, muzzle );

		VectorNormalize( forward );
		VectorMA( muzzle, -4, forward, muzzle );
		drop_charge( ent, muzzle, forward );

		ent->client->ps.hasDetPackPlanted = qtrue;
	}
}


//---------------------------------------------------------
// FireStunBaton
//---------------------------------------------------------
void WP_FireStunBaton( gentity_t *ent, qboolean alt_fire )
{
	gentity_t	*tr_ent;
	trace_t		tr;
	vec3_t		mins, maxs, end;
	vec3_t		muzzleStun;

	VectorCopy(ent->client->ps.origin, muzzleStun);
	muzzleStun[2] += ent->client->ps.viewheight-6;

	muzzleStun[0] += forward[0]*20;
	muzzleStun[1] += forward[1]*20;
	muzzleStun[2] += forward[2]*20;

	muzzleStun[0] += right[0]*4;
	muzzleStun[1] += right[1]*4;
	muzzleStun[2] += right[2]*4;

	VectorMA( muzzleStun, STUN_BATON_RANGE, forward, end );

	VectorSet( maxs, 6, 6, 6 );
	VectorScale( maxs, -1, mins );

	trap_Trace ( &tr, muzzleStun, mins, maxs, end, ent->s.number, MASK_SHOT );

	if ( tr.entityNum >= ENTITYNUM_WORLD )
	{
		return;
	}

	tr_ent = &g_entities[tr.entityNum];

	if (tr_ent && tr_ent->takedamage && tr_ent->client)
	{
		if (tr_ent->client->ps.duelInProgress &&
			tr_ent->client->ps.duelIndex != ent->s.number)
		{
			return;
		}

		if (ent->client->ps.duelInProgress &&
			ent->client->ps.duelIndex != tr_ent->s.number)
		{
			return;
		}
	}

	if ( tr_ent && tr_ent->takedamage )
	{
		G_PlayEffect( EFFECT_STUNHIT, tr.endpos, tr.plane.normal );

		// TEMP!
		G_Sound( tr_ent, CHAN_WEAPON, G_SoundIndex( va("sound/weapons/melee/punch%d", Q_irand(1, 4)) ) );

		G_Damage( tr_ent, ent, ent, forward, tr.endpos, STUN_BATON_DAMAGE, (DAMAGE_NO_KNOCKBACK|DAMAGE_HALF_ABSORB), MOD_STUN_BATON );
		//alt-fire?

		if (tr_ent->client)
		{ //if it's a player then use the shock effect
			tr_ent->client->ps.electrifyTime = level.time + 700;
		}
	}
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


/*
======================
SnapVectorTowards

Round a vector to integers for more efficient network
transmission, but make sure that it rounds towards a given point
rather than blindly truncating.  This prevents it from truncating 
into a wall.
======================
*/
void SnapVectorTowards( vec3_t v, vec3_t to ) {
	int		i;

	for ( i = 0 ; i < 3 ; i++ ) {
		if ( to[i] <= v[i] ) {
			v[i] = (int)v[i];
		} else {
			v[i] = (int)v[i] + 1;
		}
	}
}


//======================================================================


/*
===============
LogAccuracyHit
===============
*/
qboolean LogAccuracyHit( gentity_t *target, gentity_t *attacker ) {
	if( !target->takedamage ) {
		return qfalse;
	}

	if ( target == attacker ) {
		return qfalse;
	}

	if( !target->client ) {
		return qfalse;
	}

	if( !attacker->client ) {
		return qfalse;
	}

	if( target->client->ps.stats[STAT_HEALTH] <= 0 ) {
		return qfalse;
	}

	if ( OnSameTeam( target, attacker ) ) {
		return qfalse;
	}

	return qtrue;
}


/*
===============
CalcMuzzlePoint

set muzzle location relative to pivoting eye
===============
*/
void CalcMuzzlePoint ( gentity_t *ent, vec3_t forward, vec3_t right, vec3_t up, vec3_t muzzlePoint ) 
{
	int weapontype;
	vec3_t muzzleOffPoint;

	weapontype = ent->s.weapon;
	VectorCopy( ent->s.pos.trBase, muzzlePoint );

	VectorCopy(WP_MuzzlePoint[weapontype], muzzleOffPoint);

	if (ent->client->ps.usingATST)
	{
		gentity_t *headEnt = &g_entities[ent->client->damageBoxHandle_Head];

		VectorClear(muzzleOffPoint);
		muzzleOffPoint[0] = 16;
		muzzleOffPoint[2] = 128;

		if (headEnt && headEnt->s.number >= MAX_CLIENTS)
		{
			if (headEnt->bolt_Waist)
			{
				muzzleOffPoint[1] = 3;
			}
			else
			{
				muzzleOffPoint[1] = -4;
			}
		}
	}

#if 1
	if (weapontype > WP_NONE && weapontype < WP_NUM_WEAPONS)
	{	// Use the table to generate the muzzlepoint;
		{	// Crouching.  Use the add-to-Z method to adjust vertically.
			VectorMA(muzzlePoint, muzzleOffPoint[0], forward, muzzlePoint);
			VectorMA(muzzlePoint, muzzleOffPoint[1], right, muzzlePoint);
			muzzlePoint[2] += ent->client->ps.viewheight + muzzleOffPoint[2];
			// VectorMA(muzzlePoint, ent->client->ps.viewheight + WP_MuzzlePoint[weapontype][2], up, muzzlePoint);
		}
	}
#else	// Test code
	muzzlePoint[2] += ent->client->ps.viewheight;//By eyes
	muzzlePoint[2] += g_debugUp.value;
	VectorMA( muzzlePoint, g_debugForward.value, forward, muzzlePoint);
	VectorMA( muzzlePoint, g_debugRight.value, right, muzzlePoint);
#endif

	// snap to integer coordinates for more efficient network bandwidth usage
	SnapVector( muzzlePoint );
}

/*
===============
CalcMuzzlePointOrigin

set muzzle location relative to pivoting eye
===============
*/
void CalcMuzzlePointOrigin ( gentity_t *ent, vec3_t origin, vec3_t forward, vec3_t right, vec3_t up, vec3_t muzzlePoint ) {
	VectorCopy( ent->s.pos.trBase, muzzlePoint );
	muzzlePoint[2] += ent->client->ps.viewheight;
	VectorMA( muzzlePoint, 14, forward, muzzlePoint );
	// snap to integer coordinates for more efficient network bandwidth usage
	SnapVector( muzzlePoint );
}



/*
===============
FireWeapon
===============
*/
void FireWeapon( gentity_t *ent, qboolean altFire ) {
	if (ent->client->ps.powerups[PW_QUAD] ) {
		s_quadFactor = g_quadfactor.value;
	} else {
		s_quadFactor = 1;
	}

	// track shots taken for accuracy tracking.  Grapple is not a weapon and gauntet is just not tracked
	if( ent->s.weapon != WP_SABER && ent->s.weapon != WP_STUN_BATON ) 
	{
		if( ent->s.weapon == WP_FLECHETTE ) {
			ent->client->accuracy_shots += FLECHETTE_SHOTS;
		} else {
			ent->client->accuracy_shots++;
		}
	}

	// set aiming directions
	if (ent->s.weapon == WP_EMPLACED_GUN)
	{
		vec3_t viewAngCap;

		VectorCopy(ent->client->ps.viewangles, viewAngCap);
		if (viewAngCap[PITCH] > 40)
		{
			viewAngCap[PITCH] = 40;
		}
		AngleVectors( viewAngCap, forward, right, up );
	}
	else
	{
		AngleVectors( ent->client->ps.viewangles, forward, right, up );
	}

	if (ent->client->ps.usingATST)
	{
		gentity_t *headEnt = &g_entities[ent->client->damageBoxHandle_Head];

		if (headEnt && headEnt->s.number >= MAX_CLIENTS)
		{
			if (altFire)
			{
				headEnt->bolt_Waist = 0;
			}
			else
			{
				headEnt->bolt_Waist = 1;
			}
		}
	}

//	CalcMuzzlePointOrigin ( ent, ent->client->oldOrigin, forward, right, up, muzzle );
	CalcMuzzlePoint ( ent, forward, right, up, muzzle );

	if (ent->client->ps.usingATST)
	{
		WP_FireBryarPistol( ent, qfalse );
		return;
	}

	// fire the specific weapon
	switch( ent->s.weapon ) {
	case WP_STUN_BATON:
		WP_FireStunBaton( ent, altFire );
		break;

	case WP_SABER:
		break;

	case WP_BRYAR_PISTOL:
		WP_FireBryarPistol( ent, altFire );
		break;

	case WP_BLASTER:
		WP_FireBlaster( ent, altFire );
		break;

	case WP_DISRUPTOR:
		WP_FireDisruptor( ent, altFire );
		break;

	case WP_BOWCASTER:
		WP_FireBowcaster( ent, altFire );
		break;

	case WP_REPEATER:
		WP_FireRepeater( ent, altFire );
		break;

	case WP_DEMP2:
		WP_FireDEMP2( ent, altFire );
		break;

	case WP_FLECHETTE:
		WP_FireFlechette( ent, altFire );
		break;

	case WP_ROCKET_LAUNCHER:
		WP_FireRocket( ent, altFire );
		break;

	case WP_THERMAL:
		WP_FireThermalDetonator( ent, altFire );
		break;

	case WP_TRIP_MINE:
		WP_PlaceLaserTrap( ent, altFire );
		break;

	case WP_DET_PACK:
		WP_DropDetPack( ent, altFire );
		break;

	case WP_EMPLACED_GUN:
		WP_FireEmplaced( ent, altFire );
		break;
	default:
// FIXME		G_Error( "Bad ent->s.weapon" );
		break;
	}

	G_LogWeaponFire(ent->s.number, ent->s.weapon);
}

//---------------------------------------------------------
static void WP_FireEmplaced( gentity_t *ent, qboolean altFire )
//---------------------------------------------------------
{
	vec3_t	dir, angs, gunpoint; //g2r , gunaxis;
	vec3_t	right;
	gentity_t *gun;
//	mdxaBone_t matrix;

	if (!ent->client)
	{
		return;
	}

	if (!ent->client->ps.emplacedIndex)
	{
		return;
	}

	gun = &g_entities[ent->client->ps.emplacedIndex];

	if (!gun)
	{
		return;
	}

	VectorCopy(/*ent->client->ps.origin*/gun->s.origin, gunpoint);
	gunpoint[2] += 46;

	AngleVectors(ent->client->ps.viewangles, NULL, right, NULL);

	if (gun->bolt_Waist)
	{
		gunpoint[0] += right[0]*10;
		gunpoint[1] += right[1]*10;
		gunpoint[2] += right[2]*10;

		gun->bolt_Waist = 0;
		G_AddEvent(gun, EV_FIRE_WEAPON, 0);
	}
	else
	{
		gunpoint[0] -= right[0]*10;
		gunpoint[1] -= right[1]*10;
		gunpoint[2] -= right[2]*10;
		gun->bolt_Waist = 1;
		G_AddEvent(gun, EV_FIRE_WEAPON, 1);
	}

	vectoangles( forward, angs );

	if ( altFire )
	{
		// add some slop to the alt-fire direction
		angs[PITCH] += crandom() * BLASTER_SPREAD;
		angs[YAW]	+= crandom() * BLASTER_SPREAD;
	}

	AngleVectors( angs, dir, NULL, NULL );

	// FIXME: if temp_org does not have clear trace to inside the bbox, don't shoot!
	//WP_FireEmplacedMissile( ent, gunpoint, dir, altFire, gun );
	WP_FireEmplacedMissile( gun, gunpoint, dir, altFire, ent );
	//WP_FireTurretMissile(gun, gunpoint, dir, altFire, 15, 2000, MOD_BLASTER, ent);
}

#define EMPLACED_CANRESPAWN 1

//----------------------------------------------------------

/*QUAKED emplaced_gun (0 0 1) (-30 -20 8) (30 20 60) CANRESPAWN

 count - if CANRESPAWN spawnflag, decides how long it is before gun respawns (in ms)
*/
 
//----------------------------------------------------------
void emplaced_gun_use( gentity_t *self, gentity_t *other, trace_t *trace )
{
	vec3_t fwd1, fwd2;
	float dot;
	int oldWeapon;
	gentity_t *activator = other;
	float zoffset = 50;
	vec3_t anglesToOwner;
	vec3_t vLen;
	float ownLen = 0;

	if ( self->health <= 0 )
	{
		// can't use a dead gun.
		return;
	}

	if (self->activator)
	{
		return;
	}

	if (!activator->client)
	{
		return;
	}

	if (activator->client->ps.emplacedTime > level.time)
	{
		return;
	}

	if (activator->client->ps.weaponTime > 0)
	{
		return;
	}

	if (activator->client->ps.origin[2] > self->s.origin[2]+zoffset-8)
	{
		return;
	} //can't use it from the top

	if (activator->client->ps.pm_flags & PMF_DUCKED)
	{
		return;
	}

	if (activator->client->ps.isJediMaster)
	{ //;O
		return;
	}

	VectorSubtract(self->s.origin, activator->client->ps.origin, vLen);
	ownLen = VectorLength(vLen);

	if (ownLen > 64)
	{
		return;
	}

	// Let's get some direction vectors for the users
	AngleVectors( activator->client->ps.viewangles, fwd1, NULL, NULL );

	// Get the guns direction vector
	AngleVectors( self->pos1, fwd2, NULL, NULL );

	dot = DotProduct( fwd1, fwd2 );

	// Must be reasonably facing the way the gun points ( 110 degrees or so ), otherwise we don't allow to use it.
	if ( dot < -0.2f )
	{
		return;
	}

	VectorSubtract(self->s.origin, activator->client->ps.origin, fwd1);
	VectorNormalize(fwd1);

	dot = DotProduct( fwd1, fwd2 );

	// Must be reasonably facing the way the gun points ( 110 degrees or so ), otherwise we don't allow to use it.
	if ( dot < /*-0.2f*/0.6f/*0.8f*/ )
	{
		return;
	}

	self->boltpoint1 = 1;

	// don't allow using it again for half a second
//	if ( activator->s.number == 0 && self->delay + 500 < level.time )
//	{
		oldWeapon = activator->s.weapon;

		// swap the users weapon with the emplaced gun and add the ammo the gun has to the player
		activator->client->ps.weapon = self->s.weapon;
		activator->client->ps.weaponstate = WEAPON_READY;
		activator->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_EMPLACED_GUN );

		//SnapVector(self->s.origin);

		VectorCopy(activator->client->ps.origin, self->s.origin2);

		activator->client->ps.emplacedIndex = self->s.number;

		self->s.emplacedOwner = activator->s.number;
		self->s.activeForcePass = NUM_FORCE_POWERS+1;

		// the gun will track which weapon we used to have
		self->s.weapon = oldWeapon;

		// Lock the player
//		activator->client->ps.eFlags |= EF_LOCKED_TO_WEAPON;
		activator->r.ownerNum = self->s.number; // kind of dumb, but when we are locked to the weapon, we are owned by it.
		self->activator = activator;
//		self->delay = level.time; // can't disconnect from the thing for half a second

		// Let the client know that we want to start our emplaced camera clamping
		// FIXME:  if you are in the gun and you switch/restart maps, emplacedClamp will still be 1 and since
		//	you can't change it from the console, you are stuck with really bad viewangles
//		char	temp[32];
//		gi.cvar_set("cl_emplacedClamp", "1");
//		sprintf( temp, "%f", self->pos1[0] );
//		gi.cvar_set("cl_emplacedPitch", temp );
//		sprintf( temp, "%f", self->pos1[1] );
//		gi.cvar_set("cl_emplacedYaw", temp );

		// Let the gun be considered an enemy
//		self->svFlags |= SVF_NONNPC_ENEMY;

		// move the player to the center of the gun and make player not solid
//		activator->contents = 0;
//		VectorCopy( self->currentOrigin, activator->client->ps.origin );

		// FIXME: trying to force the gun to look forward, but it seems to pick up the players viewangles....and
		//	since you usually go up to the side of the gun to use it, you end up starting with a really annoying
		//	set of viewangles.
		//G_SetAngles( activator, self->s.angles );

		VectorSubtract(self->r.currentOrigin, activator->client->ps.origin, anglesToOwner);
		vectoangles(anglesToOwner, anglesToOwner);

		//SetClientViewAngle(activator, /*self->s.angles*/anglesToOwner);

//		VectorCopy(activator->s.angles, self->pos1);

		// Overriding these may be a bad thing....
//		gi.cvar_set("cg_thirdPersonRange", "20");
//		gi.cvar_set("cg_thirdPersonVertOffset", "35");
//	}
}

void emplaced_gun_realuse( gentity_t *self, gentity_t *other, gentity_t *activator )
{
	emplaced_gun_use(self, other, NULL);
}

//----------------------------------------------------------
void emplaced_gun_pain( gentity_t *self, gentity_t *attacker, int damage )
{
	if ( self->health <= 0 )
	{
		// play pain effect?
	}
	else
	{
//		if ( self->paintarget )
//		{
//			G_UseTargets2( self, self->activator, self->paintarget );
//		}

		//Don't do script if dead
//		G_ActivateBehavior( self, BSET_PAIN );
	}
}

#define EMPLACED_GUN_HEALTH 800

//----------------------------------------------------------
void emplaced_gun_update(gentity_t *self)
{
	vec3_t	smokeOrg, puffAngle;
	int oldWeap;
	float ownLen = 0;

	if (self->health < 1 && !self->bolt_Head)
	{
		if (self->spawnflags & EMPLACED_CANRESPAWN)
		{
			self->bolt_Head = level.time + 4000 + self->count;
		}
	}
	else if (self->health < 1 && self->bolt_Head < level.time)
	{
		self->s.time = 0;
		self->boltpoint4 = 0;
		self->boltpoint3 = 0;
		self->health = EMPLACED_GUN_HEALTH*0.4;
	}

	if (self->boltpoint4 && self->boltpoint4 < 2 && self->s.time < level.time)
	{
		vec3_t explOrg;

		VectorSet( puffAngle, 0, 0, 1 );

		VectorCopy(self->r.currentOrigin, explOrg);
		explOrg[2] += 16;

		//G_PlayEffect(EFFECT_EXPLOSION, explOrg, /*self->r.currentAngles*/puffAngle);
		G_PlayEffect(EFFECT_EXPLOSION_DETPACK, explOrg, /*self->r.currentAngles*/puffAngle);

		self->boltpoint3 = level.time + Q_irand(2500, 3500);

		G_RadiusDamage(self->r.currentOrigin, self, self->splashDamage, self->splashRadius, self, MOD_UNKNOWN);

		self->s.time = -1;

		self->boltpoint4 = 2;
	}

	if (self->boltpoint3 > level.time)
	{
		if (self->boltpoint2 < level.time)
		{
			VectorSet( puffAngle, 0, 0, 1 );
			VectorCopy(self->r.currentOrigin, smokeOrg);

			smokeOrg[2] += 60;

			//What.. was I thinking?
			G_PlayEffect(EFFECT_SMOKE, smokeOrg, puffAngle);

			self->boltpoint2 = level.time + Q_irand(250, 400);
			//This would be much better if we checked a value on the entity on the client
			//and then spawned smoke there instead of sending over a bunch of events. But
			//this will do for now, an event every 250-400ms isn't too bad.
		}
	}

	if (self->activator && self->activator->client && self->activator->inuse)
	{
		vec3_t vLen;
		VectorSubtract(self->s.origin, self->activator->client->ps.origin, vLen);
		ownLen = VectorLength(vLen);

		if (!(self->activator->client->pers.cmd.buttons & BUTTON_USE) && self->boltpoint1)
		{
			self->boltpoint1 = 0;
		}

		if ((self->activator->client->pers.cmd.buttons & BUTTON_USE) && !self->boltpoint1)
		{
			self->activator->client->ps.emplacedIndex = 0;
			self->nextthink = level.time + 50;
			return;
		}
	}

	if ((self->activator && self->activator->client) &&
		(!self->activator->inuse || self->activator->client->ps.emplacedIndex != self->s.number ||	self->boltpoint4 || ownLen > 64))
	{
		if (self->activator->client->ps.stats[STAT_WEAPONS] & ( 1 << WP_EMPLACED_GUN ))
		{
			self->activator->client->ps.stats[STAT_WEAPONS] -= ( 1 << WP_EMPLACED_GUN );
		}
		//VectorCopy(self->s.origin2, self->activator->client->ps.origin);
		oldWeap = self->activator->client->ps.weapon;
		self->activator->client->ps.weapon = self->s.weapon;
		self->s.weapon = oldWeap;
		self->activator->r.ownerNum = ENTITYNUM_NONE;
		self->activator->client->ps.emplacedTime = level.time + 1000;
		self->activator->client->ps.emplacedIndex = 0;
		self->activator = NULL;

		self->s.activeForcePass = 0;
	}
	else if (self->activator && self->activator->client)
	{
		self->activator->client->ps.weapon = WP_EMPLACED_GUN;
		self->activator->client->ps.weaponstate = WEAPON_READY;
	}
	self->nextthink = level.time + 50;
}

//----------------------------------------------------------
void emplaced_gun_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod )
{
	if (self->boltpoint4)
	{
		return;
	}

	self->boltpoint4 = 1;

	self->s.time = level.time + 3000;

	self->bolt_Head = 0;
}

void SP_emplaced_gun( gentity_t *ent )
{
	//char name[] = "models/map_objects/imp_mine/turret_chair.glm";
	char name[] = "models/map_objects/mp/turret_chair.glm";
	vec3_t down;
	trace_t tr;

	RegisterItem( BG_FindItemForWeapon(WP_BLASTER) );
	//Emplaced gun uses many of the same assets as the blaster, so just precache it

	ent->r.contents = CONTENTS_SOLID;
	ent->s.solid = SOLID_BBOX;

	ent->bolt_Head = 0;

	VectorSet( ent->r.mins, -30, -20, 8 );
	VectorSet( ent->r.maxs, 30, 20, 60 );

	VectorCopy(ent->s.origin, down);

	down[2] -= 1024;

	trap_Trace(&tr, ent->s.origin, ent->r.mins, ent->r.maxs, down, ent->s.number, MASK_SOLID);

	if (tr.fraction != 1 && !tr.allsolid && !tr.startsolid)
	{
		VectorCopy(tr.endpos, ent->s.origin);
	}

	ent->spawnflags |= 4; // deadsolid

	ent->health = EMPLACED_GUN_HEALTH;

	if (ent->spawnflags & EMPLACED_CANRESPAWN)
	{ //make it somewhat easier to kill if it can respawn
		ent->health *= 0.4;
	}

	ent->boltpoint4 = 0;

	ent->takedamage = qtrue;
	ent->pain = emplaced_gun_pain;
	ent->die = emplaced_gun_die;

	// being caught in this thing when it blows would be really bad.
	ent->splashDamage = 80;
	ent->splashRadius = 128;

//	G_EffectIndex( "emplaced/explode" );
//	G_EffectIndex( "emplaced/dead_smoke" );

	// amount of ammo that this little poochie has
	G_SpawnInt( "count", "600", &ent->count );

	ent->s.modelindex = G_ModelIndex( name );
	ent->s.modelGhoul2 = 1;
	ent->s.g2radius = 110;
	//trap_G2API_InitGhoul2Model( ent->s.ghoul2, name, ent->s.modelindex );
//g2r	trap_G2API_InitGhoul2Model( &ent->s, name, ent->s.modelindex, 0, 0, 0, 0 );

	// Activate our tags and bones
//	ent->headBolt = gi.G2API_AddBolt( &ent->s.ghoul2[0], "*seat" );
//	ent->handLBolt = gi.G2API_AddBolt( &ent->s.ghoul2[0], "*flash01" );
//	ent->handRBolt = gi.G2API_AddBolt( &ent->s.ghoul2[0], "*flash02" );
//	gi.G2API_SetBoneAngles( &ent->s.ghoul2[0], "swivel_bone", vec3_origin, BONE_ANGLES_POSTMULT, POSITIVE_Y, POSITIVE_Z, POSITIVE_X, NULL); 

//	RegisterItem( FindItemForWeapon( WP_EMPLACED_GUN ));
	ent->s.weapon = WP_EMPLACED_GUN;

//	SnapVector(ent->s.origin);

	G_SetOrigin( ent, ent->s.origin );
	
	//G_SetAngles( ent, ent->s.angles );

	// store base angles for later
	VectorCopy( ent->s.angles, ent->pos1 );
	VectorCopy( ent->s.angles, ent->r.currentAngles );
	VectorCopy( ent->s.angles, ent->s.apos.trBase );

	ent->think = emplaced_gun_update;
	ent->nextthink = level.time + 50;

//	ent->e_UseFunc = useF_emplaced_gun_use;
	ent->use = emplaced_gun_realuse;
	//ent->touch = emplaced_gun_use;

	ent->r.svFlags |= SVF_PLAYER_USABLE;

	ent->s.pos.trType = TR_STATIONARY;

	ent->s.owner = MAX_CLIENTS+1;
	ent->s.shouldtarget = qtrue;
	ent->s.teamowner = 0;

	/*
	angswiv[ROLL] = 0;
	angswiv[PITCH] = 0;
	angswiv[YAW] = 70;
	trap_G2API_SetBoneAngles(ent->s.ghoul2, 0, "swivel_bone", angswiv, BONE_ANGLES_REPLACE, POSITIVE_Z, NEGATIVE_X, NEGATIVE_Y, NULL, 0, level.time);
	*/


//g2r	ent->s.trickedentindex = trap_G2API_AddBolt(ent->s.ghoul2, 0, "*seat");
//g2r	ent->s.bolt1 = trap_G2API_AddBolt(ent->s.ghoul2, 0, "*flash01");
//g2r	ent->s.bolt2 = trap_G2API_AddBolt(ent->s.ghoul2, 0, "*flash02");

	trap_LinkEntity(ent);
}
