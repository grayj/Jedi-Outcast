// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "g_local.h"

/*

  Items are any object that a player can touch to gain some effect.

  Pickup will return the number of seconds until they should respawn.

  all items should pop when dropped in lava or slime

  Respawnable items don't actually go away when picked up, they are
  just made invisible and untouchable.  This allows them to ride
  movers and respawn apropriately.
*/


#define	RESPAWN_ARMOR		20
#define	RESPAWN_TEAM_WEAPON	30
#define	RESPAWN_HEALTH		30
#define	RESPAWN_AMMO		40
#define	RESPAWN_HOLDABLE	60
#define	RESPAWN_MEGAHEALTH	120
#define	RESPAWN_POWERUP		120

extern gentity_t *droppedRedFlag;
extern gentity_t *droppedBlueFlag;

//======================================================================
#define MAX_MEDPACK_HEAL_AMOUNT		100
#define MAX_SENTRY_DISTANCE			256

// For more than four players, adjust the respawn times, up to 1/4.
int adjustRespawnTime(float preRespawnTime, int itemType, int itemTag)
{
	float respawnTime = preRespawnTime;

	if (itemType == IT_WEAPON)
	{
		if (itemTag == WP_THERMAL ||
			itemTag == WP_TRIP_MINE ||
			itemTag == WP_DET_PACK)
		{ //special case for these, use ammo respawn rate
			respawnTime = RESPAWN_AMMO;
		}
	}

	if (!g_adaptRespawn.integer)
	{
		return((int)respawnTime);
	}

	if (level.numPlayingClients > 4)
	{	// Start scaling the respawn times.
		if (level.numPlayingClients > 32)
		{	// 1/4 time minimum.
			respawnTime *= 0.25;
		}
		else if (level.numPlayingClients > 12)
		{	// From 12-32, scale from 0.5 to 0.25;
			respawnTime *= 20.0 / (float)(level.numPlayingClients + 8);
		}
		else 
		{	// From 4-12, scale from 1.0 to 0.5;
			respawnTime *= 8.0 / (float)(level.numPlayingClients + 4);
		}
	}

	if (respawnTime < 1.0)
	{	// No matter what, don't go lower than 1 second, or the pickups become very noisy!
		respawnTime = 1.0;
	}

	return ((int)respawnTime);
}


#define SHIELD_HEALTH				250
#define SHIELD_HEALTH_DEC			10		// 25 seconds	
#define MAX_SHIELD_HEIGHT			254
#define MAX_SHIELD_HALFWIDTH		255
#define SHIELD_HALFTHICKNESS		4
#define SHIELD_PLACEDIST			64

static qhandle_t	shieldLoopSound=0;
static qhandle_t	shieldAttachSound=0;
static qhandle_t	shieldActivateSound=0;
static qhandle_t	shieldDeactivateSound=0;
static qhandle_t	shieldDamageSound=0;


void ShieldRemove(gentity_t *self)
{
	self->think = G_FreeEntity;
	self->nextthink = level.time + 100;

	// Play kill sound...
	G_AddEvent(self, EV_GENERAL_SOUND, shieldDeactivateSound);
	self->s.loopSound = 0;

	return;
}


// Count down the health of the shield.
void ShieldThink(gentity_t *self)
{
	self->s.trickedentindex = 0;

	self->health -= SHIELD_HEALTH_DEC;
	self->nextthink = level.time + 1000;
	if (self->health <= 0)
	{
		ShieldRemove(self);
	}
	return;
}


// The shield was damaged to below zero health.
void ShieldDie(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod)
{
	// Play damaging sound...
	G_AddEvent(self, EV_GENERAL_SOUND, shieldDamageSound);

	ShieldRemove(self);
}


// The shield had damage done to it.  Make it flicker.
void ShieldPain(gentity_t *self, gentity_t *attacker, int damage)
{
	// Set the itemplaceholder flag to indicate the the shield drawing that the shield pain should be drawn.
	self->think = ShieldThink;
	self->nextthink = level.time + 400;

	// Play damaging sound...
	G_AddEvent(self, EV_GENERAL_SOUND, shieldDamageSound);

	self->s.trickedentindex = 1;

	return;
}


// Try to turn the shield back on after a delay.
void ShieldGoSolid(gentity_t *self)
{
	trace_t		tr;

	// see if we're valid
	self->health--;
	if (self->health <= 0)
	{
		ShieldRemove(self);
		return;
	}
	
	trap_Trace (&tr, self->r.currentOrigin, self->r.mins, self->r.maxs, self->r.currentOrigin, self->s.number, CONTENTS_BODY );
	if(tr.startsolid)
	{	// gah, we can't activate yet
		self->nextthink = level.time + 200;
		self->think = ShieldGoSolid;
		trap_LinkEntity(self);
	}
	else
	{ // get hard... huh-huh...
		self->s.eFlags &= ~EF_NODRAW;

		self->r.contents = CONTENTS_SOLID;
		self->nextthink = level.time + 1000;
		self->think = ShieldThink;
		self->takedamage = qtrue;
		trap_LinkEntity(self);

		// Play raising sound...
		G_AddEvent(self, EV_GENERAL_SOUND, shieldActivateSound);
		self->s.loopSound = shieldLoopSound;
	}

	return;
}


// Turn the shield off to allow a friend to pass through.
void ShieldGoNotSolid(gentity_t *self)
{
	// make the shield non-solid very briefly
	self->r.contents = 0;
	self->s.eFlags |= EF_NODRAW;
	// nextthink needs to have a large enough interval to avoid excess accumulation of Activate messages
	self->nextthink = level.time + 200;
	self->think = ShieldGoSolid;
	self->takedamage = qfalse;
	trap_LinkEntity(self);

	// Play kill sound...
	G_AddEvent(self, EV_GENERAL_SOUND, shieldDeactivateSound);
	self->s.loopSound = 0;
}


// Somebody (a player) has touched the shield.  See if it is a "friend".
void ShieldTouch(gentity_t *self, gentity_t *other, trace_t *trace)
{
	if (g_gametype.integer >= GT_TEAM)
	{ // let teammates through
		// compare the parent's team to the "other's" team
		if (self->parent && ( self->parent->client) && (other->client))
		{
			if (OnSameTeam(self->parent, other))
			{
				ShieldGoNotSolid(self);
			}
		}
	}
	else
	{//let the person who dropped the shield through
		if (self->parent && self->parent->s.number == other->s.number)
		{
			ShieldGoNotSolid(self);
		}
	}
}


// After a short delay, create the shield by expanding in all directions.
void CreateShield(gentity_t *ent)
{
	trace_t		tr;
	vec3_t		mins, maxs, end, posTraceEnd, negTraceEnd, start;
	int			height, posWidth, negWidth, halfWidth = 0;
	qboolean	xaxis;
	int			paramData = 0;
	static int	shieldID;

	// trace upward to find height of shield
	VectorCopy(ent->r.currentOrigin, end);
	end[2] += MAX_SHIELD_HEIGHT;
	trap_Trace (&tr, ent->r.currentOrigin, NULL, NULL, end, ent->s.number, MASK_SHOT );
	height = (int)(MAX_SHIELD_HEIGHT * tr.fraction);

	// use angles to find the proper axis along which to align the shield
	VectorSet(mins, -SHIELD_HALFTHICKNESS, -SHIELD_HALFTHICKNESS, 0);
	VectorSet(maxs, SHIELD_HALFTHICKNESS, SHIELD_HALFTHICKNESS, height);
	VectorCopy(ent->r.currentOrigin, posTraceEnd);
	VectorCopy(ent->r.currentOrigin, negTraceEnd);

	if ((int)(ent->s.angles[YAW]) == 0) // shield runs along y-axis
	{
		posTraceEnd[1]+=MAX_SHIELD_HALFWIDTH;
		negTraceEnd[1]-=MAX_SHIELD_HALFWIDTH;
		xaxis = qfalse;
	}
	else  // shield runs along x-axis
	{
		posTraceEnd[0]+=MAX_SHIELD_HALFWIDTH;
		negTraceEnd[0]-=MAX_SHIELD_HALFWIDTH;
		xaxis = qtrue;
	}

	// trace horizontally to find extend of shield
	// positive trace
	VectorCopy(ent->r.currentOrigin, start);
	start[2] += (height>>1);
	trap_Trace (&tr, start, 0, 0, posTraceEnd, ent->s.number, MASK_SHOT );
	posWidth = MAX_SHIELD_HALFWIDTH * tr.fraction;
	// negative trace
	trap_Trace (&tr, start, 0, 0, negTraceEnd, ent->s.number, MASK_SHOT );
	negWidth = MAX_SHIELD_HALFWIDTH * tr.fraction;

	// kef -- monkey with dimensions and place origin in center
	halfWidth = (posWidth + negWidth)>>1;
	if (xaxis)
	{
		ent->r.currentOrigin[0] = ent->r.currentOrigin[0] - negWidth + halfWidth;
	}
	else
	{
		ent->r.currentOrigin[1] = ent->r.currentOrigin[1] - negWidth + halfWidth;
	}
	ent->r.currentOrigin[2] += (height>>1);

	// set entity's mins and maxs to new values, make it solid, and link it
	if (xaxis)
	{
		VectorSet(ent->r.mins, -halfWidth, -SHIELD_HALFTHICKNESS, -(height>>1));
		VectorSet(ent->r.maxs, halfWidth, SHIELD_HALFTHICKNESS, height>>1);
	}
	else
	{
		VectorSet(ent->r.mins, -SHIELD_HALFTHICKNESS, -halfWidth, -(height>>1));
		VectorSet(ent->r.maxs, SHIELD_HALFTHICKNESS, halfWidth, height);
	}
	ent->clipmask = MASK_SHOT;

	// Information for shield rendering.

//	xaxis - 1 bit
//	height - 0-254 8 bits
//	posWidth - 0-255 8 bits
//  negWidth - 0 - 255 8 bits

	paramData = (xaxis << 24) | (height << 16) | (posWidth << 8) | (negWidth);
	ent->s.time2 = paramData;

	ent->health = ceil(SHIELD_HEALTH*1);

	ent->s.time = ent->health;//???
	ent->pain = ShieldPain;
	ent->die = ShieldDie;
	ent->touch = ShieldTouch;

	// see if we're valid
	trap_Trace (&tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, ent->r.currentOrigin, ent->s.number, CONTENTS_BODY ); 

	if (tr.startsolid)
	{	// Something in the way!
		// make the shield non-solid very briefly
		ent->r.contents = 0;
		ent->s.eFlags |= EF_NODRAW;
		// nextthink needs to have a large enough interval to avoid excess accumulation of Activate messages
		ent->nextthink = level.time + 200;
		ent->think = ShieldGoSolid;
		ent->takedamage = qfalse;
		trap_LinkEntity(ent);
	}
	else
	{	// Get solid.
		ent->r.contents = CONTENTS_PLAYERCLIP|CONTENTS_SHOTCLIP;//CONTENTS_SOLID;

		ent->nextthink = level.time;
		ent->think = ShieldThink;

		ent->takedamage = qtrue;
		trap_LinkEntity(ent);

		// Play raising sound...
		G_AddEvent(ent, EV_GENERAL_SOUND, shieldActivateSound);
		ent->s.loopSound = shieldLoopSound;
	}

	ShieldGoSolid(ent);

	return;
}

qboolean PlaceShield(gentity_t *playerent)
{
	static const gitem_t *shieldItem = NULL;
	gentity_t	*shield = NULL;
	trace_t		tr;
	vec3_t		fwd, pos, dest, mins = {-4,-4, 0}, maxs = {4,4,4};

	if (shieldAttachSound==0)
	{
		shieldLoopSound = G_SoundIndex("sound/movers/doors/forcefield_lp.wav");
		shieldAttachSound = G_SoundIndex("sound/weapons/detpack/stick.wav");
		shieldActivateSound = G_SoundIndex("sound/movers/doors/forcefield_on.wav");
		shieldDeactivateSound = G_SoundIndex("sound/movers/doors/forcefield_off.wav");
		shieldDamageSound = G_SoundIndex("sound/effects/bumpfield.wav");
		shieldItem = BG_FindItemForHoldable(HI_SHIELD);
	}

	// can we place this in front of us?
	AngleVectors (playerent->client->ps.viewangles, fwd, NULL, NULL);
	fwd[2] = 0;
	VectorMA(playerent->client->ps.origin, SHIELD_PLACEDIST, fwd, dest);
	trap_Trace (&tr, playerent->client->ps.origin, mins, maxs, dest, playerent->s.number, MASK_SHOT );
	if (tr.fraction > 0.9)
	{//room in front
		VectorCopy(tr.endpos, pos);
		// drop to floor
		VectorSet( dest, pos[0], pos[1], pos[2] - 4096 );
		trap_Trace( &tr, pos, mins, maxs, dest, playerent->s.number, MASK_SOLID );
		if ( !tr.startsolid && !tr.allsolid )
		{
			// got enough room so place the portable shield
			shield = G_Spawn();

			// Figure out what direction the shield is facing.
			if (fabs(fwd[0]) > fabs(fwd[1]))
			{	// shield is north/south, facing east.
				shield->s.angles[YAW] = 0;
			}
			else
			{	// shield is along the east/west axis, facing north
				shield->s.angles[YAW] = 90;
			}
			shield->think = CreateShield;
			shield->nextthink = level.time + 500;	// power up after .5 seconds
			shield->parent = playerent;

			// Set team number.
			shield->s.otherEntityNum2 = playerent->client->sess.sessionTeam;

			shield->s.eType = ET_SPECIAL;
			shield->s.modelindex =  HI_SHIELD;	// this'll be used in CG_Useable() for rendering.
			shield->classname = shieldItem->classname;

			shield->r.contents = CONTENTS_TRIGGER;

			shield->touch = 0;
			// using an item causes it to respawn
			shield->use = 0; //Use_Item;

			// allow to ride movers
			shield->s.groundEntityNum = tr.entityNum;

			G_SetOrigin( shield, tr.endpos );

			shield->s.eFlags &= ~EF_NODRAW;
			shield->r.svFlags &= ~SVF_NOCLIENT;

			trap_LinkEntity (shield);

			shield->s.owner = playerent->s.number;
			shield->s.shouldtarget = qtrue;
			if (g_gametype.integer >= GT_TEAM)
			{
				shield->s.teamowner = playerent->client->sess.sessionTeam;
			}
			else
			{
				shield->s.teamowner = 16;
			}

			// Play placing sound...
			G_AddEvent(shield, EV_GENERAL_SOUND, shieldAttachSound);

			return qtrue;
		}
	}
	// no room
	return qfalse;
}

void ItemUse_Binoculars(gentity_t *ent)
{
	if (!ent || !ent->client)
	{
		return;
	}

	if (ent->client->ps.weaponstate != WEAPON_READY)
	{ //So we can't fool it and reactivate while switching to the saber or something.
		return;
	}

	if (ent->client->ps.weapon == WP_SABER)
	{ //No.
		return;
	}

	if (ent->client->ps.zoomMode == 0) // not zoomed or currently zoomed with the disruptor
	{
		ent->client->ps.zoomMode = 2;
		ent->client->ps.zoomLocked = qfalse;
		ent->client->ps.zoomFov = 40.0f;
	}
	else if (ent->client->ps.zoomMode == 2)
	{
		ent->client->ps.zoomMode = 0;
		ent->client->ps.zoomTime = level.time;
	}
}

void ItemUse_Shield(gentity_t *ent)
{
	PlaceShield(ent);
}

//--------------------------
// PERSONAL ASSAULT SENTRY
//--------------------------

#define PAS_DAMAGE	2

void SentryTouch(gentity_t *ent, gentity_t *other, trace_t *trace)
{
	return;
}

//----------------------------------------------------------------
void pas_fire( gentity_t *ent )
//----------------------------------------------------------------
{
	vec3_t fwd, myOrg, enOrg;

	VectorCopy(ent->r.currentOrigin, myOrg);
	myOrg[2] += 24;

	VectorCopy(ent->enemy->client->ps.origin, enOrg);
	enOrg[2] += 24;

	VectorSubtract(enOrg, myOrg, fwd);
	VectorNormalize(fwd);
	
	myOrg[0] += fwd[0]*16;
	myOrg[1] += fwd[1]*16;
	myOrg[2] += fwd[2]*16;

	WP_FireTurretMissile(&g_entities[ent->boltpoint3], myOrg, fwd, qfalse, 10, 2300, MOD_SENTRY, ent );

	G_RunObject(ent);
}

void G_SetEnemy( gentity_t *self, gentity_t *enemy )
{
	if (!enemy->client)
	{
		return;
	}

	self->enemy = enemy;
}

#define TURRET_RADIUS 800

//-----------------------------------------------------
static qboolean pas_find_enemies( gentity_t *self )
//-----------------------------------------------------
{
	qboolean	found = qfalse;
	int			count, i;
	float		bestDist = TURRET_RADIUS*TURRET_RADIUS;
	float		enemyDist;
	vec3_t		enemyDir, org, org2;
	gentity_t	*entity_list[MAX_GENTITIES], *target;
	trace_t		tr;

	if ( self->aimDebounceTime > level.time ) // time since we've been shut off
	{
		// We were active and alert, i.e. had an enemy in the last 3 secs
		if ( self->painDebounceTime < level.time )
		{
			G_Sound(self, CHAN_BODY, G_SoundIndex( "sound/chars/turret/ping.wav" ));
			self->painDebounceTime = level.time + 1000;
		}
	}

	VectorCopy(self->s.pos.trBase, org2);

	count = G_RadiusList( org2, TURRET_RADIUS, self, qtrue, entity_list );

	for ( i = 0; i < count; i++ )
	{
		target = entity_list[i];

		if ( !target->client )
		{
			continue;
		}
		if ( target == self || !target->takedamage || target->health <= 0 || ( target->flags & FL_NOTARGET ))
		{
			continue;
		}
		if ( self->noDamageTeam && target->client->sess.sessionTeam == self->noDamageTeam )
		{ 
			continue;
		}
		if (self->boltpoint3 == target->s.number)
		{
			continue;
		}
		if ( !trap_InPVS( org2, target->r.currentOrigin ))
		{
			continue;
		}

		if ( target->client )
		{
			VectorCopy( target->client->ps.origin, org );
		}
		else
		{
			VectorCopy( target->r.currentOrigin, org );
		}

		trap_Trace( &tr, org2, NULL, NULL, org, self->s.number, MASK_SHOT );

		if ( !tr.allsolid && !tr.startsolid && ( tr.fraction == 1.0 || tr.entityNum == target->s.number ))
		{
			// Only acquire if have a clear shot, Is it in range and closer than our best?
			VectorSubtract( target->r.currentOrigin, self->r.currentOrigin, enemyDir );
			enemyDist = VectorLengthSquared( enemyDir );

			if ( enemyDist < bestDist )// all things equal, keep current
			{
				if ( self->attackDebounceTime + 100 < level.time )
				{
					// We haven't fired or acquired an enemy in the last 2 seconds-start-up sound
					G_Sound( self, CHAN_BODY, G_SoundIndex( "sound/chars/turret/startup.wav" ));

					// Wind up turrets for a bit
					self->attackDebounceTime = level.time + 900 + random() * 200;
				}

				G_SetEnemy( self, target );
				bestDist = enemyDist;
				found = qtrue;
			}
		}
	}

	return found;
}

//---------------------------------
void pas_adjust_enemy( gentity_t *ent )
//---------------------------------
{
	trace_t	tr;
	qboolean keep = qtrue;

	if ( ent->enemy->health <= 0 )
	{
		keep = qfalse;
	}
	else
	{
		vec3_t		org, org2;

		VectorCopy(ent->s.pos.trBase, org2);

		if ( ent->enemy->client )
		{
			VectorCopy( ent->enemy->client->ps.origin, org );
			org[2] -= 15;
		}
		else
		{
			VectorCopy( ent->enemy->r.currentOrigin, org );
		}

		trap_Trace( &tr, org2, NULL, NULL, org, ent->s.number, MASK_SHOT );

		if ( tr.allsolid || tr.startsolid || tr.fraction < 0.9f || tr.entityNum == ent->s.number )
		{
			if (tr.entityNum != ent->enemy->s.number)
			{
				// trace failed
				keep = qfalse;
			}
		}
	}

	if ( keep )
	{
		//ent->bounceCount = level.time + 500 + random() * 150;
	}
	else if ( ent->bounceCount < level.time && ent->enemy ) // don't ping pong on and off
	{
		ent->enemy = NULL;
		// shut-down sound
		G_Sound( ent, CHAN_BODY, G_SoundIndex( "sound/chars/turret/shutdown.wav" ));
	
		ent->bounceCount = level.time + 500 + random() * 150;

		// make turret play ping sound for 5 seconds
		ent->aimDebounceTime = level.time + 5000;
	}
}

#define TURRET_DEATH_DELAY 2000
#define TURRET_LIFETIME 60000

void turret_die(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod);

void sentryExpire(gentity_t *self)
{
	turret_die(self, self, self, 1000, MOD_UNKNOWN);	
}

//---------------------------------
void pas_think( gentity_t *ent )
//---------------------------------
{
	qboolean	moved;
	float		diffYaw, diffPitch;
	vec3_t		enemyDir, org;
	vec3_t		frontAngles, backAngles;
	vec3_t		desiredAngles;
	int			iEntityList[MAX_GENTITIES];
	int			numListedEntities;
	int			i = 0;
	qboolean	clTrapped = qfalse;
	vec3_t		testMins, testMaxs;

	testMins[0] = ent->r.currentOrigin[0] + ent->r.mins[0]+4;
	testMins[1] = ent->r.currentOrigin[1] + ent->r.mins[1]+4;
	testMins[2] = ent->r.currentOrigin[2] + ent->r.mins[2]+4;

	testMaxs[0] = ent->r.currentOrigin[0] + ent->r.maxs[0]-4;
	testMaxs[1] = ent->r.currentOrigin[1] + ent->r.maxs[1]-4;
	testMaxs[2] = ent->r.currentOrigin[2] + ent->r.maxs[2]-4;

	numListedEntities = trap_EntitiesInBox( testMins, testMaxs, iEntityList, MAX_GENTITIES );

	while (i < numListedEntities)
	{
		if (iEntityList[i] < MAX_CLIENTS)
		{ //client stuck inside me. go nonsolid.
			int clNum = iEntityList[i];

			numListedEntities = trap_EntitiesInBox( g_entities[clNum].r.absmin, g_entities[clNum].r.absmax, iEntityList, MAX_GENTITIES );

			i = 0;
			while (i < numListedEntities)
			{
				if (iEntityList[i] == ent->s.number)
				{
					clTrapped = qtrue;
					break;
				}
				i++;
			}
			break;
		}

		i++;
	}

	if (clTrapped)
	{
		ent->r.contents = 0;
		ent->s.fireflag = 0;
		ent->nextthink = level.time + FRAMETIME;
		return;
	}
	else
	{
		ent->r.contents = CONTENTS_SOLID;
	}

	if (!g_entities[ent->boltpoint3].inuse || !g_entities[ent->boltpoint3].client ||
		g_entities[ent->boltpoint3].client->sess.sessionTeam != ent->boltpoint2)
	{
		ent->think = G_FreeEntity;
		ent->nextthink = level.time;
		return;
	}

//	G_RunObject(ent);

	if ( !ent->damage )
	{
		ent->damage = 1;
		ent->nextthink = level.time + FRAMETIME;
		return;
	}

	if ((ent->bolt_LLeg+TURRET_LIFETIME) < level.time)
	{
		G_Sound( ent, CHAN_BODY, G_SoundIndex( "sound/chars/turret/shutdown.wav" ));
		ent->s.bolt2 = ENTITYNUM_NONE;
		ent->s.fireflag = 2;

		ent->think = sentryExpire;
		ent->nextthink = level.time + TURRET_DEATH_DELAY;
		return;
	}

	ent->nextthink = level.time + FRAMETIME;

	if ( ent->enemy )
	{
		// make sure that the enemy is still valid
		pas_adjust_enemy( ent );
	}

	if (ent->enemy)
	{
		if (!ent->enemy->client)
		{
			ent->enemy = NULL;
		}
		else if (ent->enemy->s.number == ent->s.number)
		{
			ent->enemy = NULL;
		}
		else if (ent->enemy->health < 1)
		{
			ent->enemy = NULL;
		}
	}

	if ( !ent->enemy )
	{
		pas_find_enemies( ent );
	}

	if (ent->enemy)
	{
		ent->s.bolt2 = ent->enemy->s.number;
	}
	else
	{
		ent->s.bolt2 = ENTITYNUM_NONE;
	}

	moved = qfalse;
	diffYaw = 0.0f; diffPitch = 0.0f;

	ent->speed = AngleNormalize360( ent->speed );
	ent->random = AngleNormalize360( ent->random );

	if ( ent->enemy )
	{
		// ...then we'll calculate what new aim adjustments we should attempt to make this frame
		// Aim at enemy
		if ( ent->enemy->client )
		{
			VectorCopy( ent->enemy->client->ps.origin, org );
		}
		else
		{
			VectorCopy( ent->enemy->r.currentOrigin, org );
		}

		VectorSubtract( org, ent->r.currentOrigin, enemyDir );
		vectoangles( enemyDir, desiredAngles );

		diffYaw = AngleSubtract( ent->speed, desiredAngles[YAW] );
		diffPitch = AngleSubtract( ent->random, desiredAngles[PITCH] );
	}
	else
	{
		// no enemy, so make us slowly sweep back and forth as if searching for a new one
		diffYaw = sin( level.time * 0.0001f + ent->count ) * 2.0f;
	}

	if ( fabs(diffYaw) > 0.25f )
	{
		moved = qtrue;

		if ( fabs(diffYaw) > 10.0f )
		{
			// cap max speed
			ent->speed += (diffYaw > 0.0f) ? -10.0f : 10.0f;
		}
		else
		{
			// small enough
			ent->speed -= diffYaw;
		}
	}


	if ( fabs(diffPitch) > 0.25f )
	{
		moved = qtrue;

		if ( fabs(diffPitch) > 4.0f )
		{
			// cap max speed
			ent->random += (diffPitch > 0.0f) ? -4.0f : 4.0f;
		}
		else
		{
			// small enough
			ent->random -= diffPitch;
		}
	}

	// the bone axes are messed up, so hence some dumbness here
	VectorSet( frontAngles, -ent->random, 0.0f, 0.0f );
	VectorSet( backAngles, 0.0f, 0.0f, ent->speed );

	if ( moved )
	{
	//ent->s.loopSound = G_SoundIndex( "sound/chars/turret/move.wav" );
	}
	else
	{
		ent->s.loopSound = 0;
	}

	if ( ent->enemy && ent->attackDebounceTime < level.time )
	{
		ent->count--;

		if ( ent->count )
		{
			pas_fire( ent );
			ent->s.fireflag = 1;
			ent->attackDebounceTime = level.time + 200;
		}
		else
		{
			//ent->nextthink = 0;
			G_Sound( ent, CHAN_BODY, G_SoundIndex( "sound/chars/turret/shutdown.wav" ));
			ent->s.bolt2 = ENTITYNUM_NONE;
			ent->s.fireflag = 2;

			ent->think = sentryExpire;
			ent->nextthink = level.time + TURRET_DEATH_DELAY;
		}
	}
	else
	{
		ent->s.fireflag = 0;
	}
}

//------------------------------------------------------------------------------------------------------------
void turret_die(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod)
//------------------------------------------------------------------------------------------------------------
{
	// Turn off the thinking of the base & use it's targets
	self->think = 0;//NULL;
	self->use = 0;//NULL;

	if ( self->target )
	{
		G_UseTargets( self, attacker );
	}

	if (!g_entities[self->boltpoint3].inuse || !g_entities[self->boltpoint3].client)
	{
		G_FreeEntity(self);
		return;
	}

	// clear my data
	self->die  = 0;//NULL;
	self->takedamage = qfalse;
	self->health = 0;

	// hack the effect angle so that explode death can orient the effect properly
	VectorSet( self->s.angles, 0, 0, 1 );

	G_PlayEffect(EFFECT_EXPLOSION_PAS, self->s.pos.trBase, self->s.angles);
	G_RadiusDamage(self->s.pos.trBase, &g_entities[self->boltpoint3], 30, 256, self, MOD_UNKNOWN);

	g_entities[self->boltpoint3].client->ps.fd.sentryDeployed = qfalse;

	//ExplodeDeath( self );
	G_FreeEntity( self );
}

#define TURRET_AMMO_COUNT 40

//---------------------------------
void SP_PAS( gentity_t *base )
//---------------------------------
{
	if ( base->count == 0 )
	{
		// give ammo
		base->count = TURRET_AMMO_COUNT;
	}

	base->s.bolt1 = 1; //This is a sort of hack to indicate that this model needs special turret things done to it
	base->s.bolt2 = ENTITYNUM_NONE; //store our current enemy index

	base->damage = 0; // start animation flag

	VectorSet( base->r.mins, -8, -8, 0 );
	VectorSet( base->r.maxs, 8, 8, 24 );

	G_RunObject(base);

	base->think = pas_think;
	base->nextthink = level.time + FRAMETIME;

	if ( !base->health )
	{
		base->health = 50;
	}

	base->takedamage = qtrue;
	base->die  = turret_die;

	base->physicsObject = qtrue;

	G_Sound( base, CHAN_BODY, G_SoundIndex( "sound/chars/turret/startup.wav" ));
}

//------------------------------------------------------------------------
void ItemUse_Sentry( gentity_t *ent )
//------------------------------------------------------------------------
{
	vec3_t fwd, fwdorg;
	vec3_t yawonly;
	vec3_t mins, maxs;
	gentity_t *sentry;

	if (!ent || !ent->client)
	{
		return;
	}

	VectorSet( mins, -8, -8, 0 );
	VectorSet( maxs, 8, 8, 24 );


	yawonly[ROLL] = 0;
	yawonly[PITCH] = 0;
	yawonly[YAW] = ent->client->ps.viewangles[YAW];

	AngleVectors(yawonly, fwd, NULL, NULL);

	fwdorg[0] = ent->client->ps.origin[0] + fwd[0]*64;
	fwdorg[1] = ent->client->ps.origin[1] + fwd[1]*64;
	fwdorg[2] = ent->client->ps.origin[2] + fwd[2]*64;

	sentry = G_Spawn();

	sentry->classname = "sentryGun";
	sentry->s.modelindex = G_ModelIndex("models/items/psgun.glm"); //replace ASAP

	sentry->s.g2radius = 30.0f;
	sentry->s.modelGhoul2 = 1;

	G_SetOrigin(sentry, fwdorg);
	sentry->parent = ent;
	sentry->r.contents = CONTENTS_SOLID;
	sentry->s.solid = 2;
	sentry->clipmask = MASK_SOLID;
	VectorCopy(mins, sentry->r.mins);
	VectorCopy(maxs, sentry->r.maxs);
	sentry->boltpoint3 = ent->s.number;
	sentry->boltpoint2 = ent->client->sess.sessionTeam; //so we can remove ourself if our owner changes teams
	sentry->r.absmin[0] = sentry->s.pos.trBase[0] + sentry->r.mins[0];
	sentry->r.absmin[1] = sentry->s.pos.trBase[1] + sentry->r.mins[1];
	sentry->r.absmin[2] = sentry->s.pos.trBase[2] + sentry->r.mins[2];
	sentry->r.absmax[0] = sentry->s.pos.trBase[0] + sentry->r.maxs[0];
	sentry->r.absmax[1] = sentry->s.pos.trBase[1] + sentry->r.maxs[1];
	sentry->r.absmax[2] = sentry->s.pos.trBase[2] + sentry->r.maxs[2];
	sentry->s.eType = ET_GENERAL;
	sentry->s.pos.trType = TR_GRAVITY;//STATIONARY;
	sentry->s.pos.trTime = level.time;
	sentry->touch = SentryTouch;
	sentry->nextthink = level.time;
	sentry->boltpoint4 = ENTITYNUM_NONE; //boltpoint4 used as enemy index

	sentry->bolt_Head = 1000;

	sentry->bolt_LLeg = level.time;

	sentry->noDamageTeam = ent->client->sess.sessionTeam;

	ent->client->ps.fd.sentryDeployed = qtrue;

	trap_LinkEntity(sentry);

	sentry->s.owner = ent->s.number;
	sentry->s.shouldtarget = qtrue;
	if (g_gametype.integer >= GT_TEAM)
	{
		sentry->s.teamowner = ent->client->sess.sessionTeam;
	}
	else
	{
		sentry->s.teamowner = 16;
	}

	SP_PAS( sentry );
}


void ItemUse_Seeker(gentity_t *ent)
{
	ent->client->ps.eFlags |= EF_SEEKERDRONE;

	ent->client->ps.droneExistTime = level.time + 30000;
	ent->client->ps.droneFireTime = level.time + 1500;
}

void ItemUse_MedPack(gentity_t *ent)
{
	if (!ent || !ent->client)
	{
		return;
	}

	if (ent->health <= 0 ||
		ent->client->ps.stats[STAT_HEALTH] <= 0 ||
		(ent->client->ps.eFlags & EF_DEAD))
	{
		return;
	}

	if (ent->health >= ent->client->ps.stats[STAT_MAX_HEALTH])
	{
		return;
	}

	ent->health += MAX_MEDPACK_HEAL_AMOUNT;

	if (ent->health > ent->client->ps.stats[STAT_MAX_HEALTH])
	{
		ent->health = ent->client->ps.stats[STAT_MAX_HEALTH];
	}
}

int Pickup_Powerup( gentity_t *ent, gentity_t *other ) {
	int			quantity;
	int			i;
	gclient_t	*client;

	if ( !other->client->ps.powerups[ent->item->giTag] ) {
		// round timing to seconds to make multiple powerup timers
		// count in sync
		other->client->ps.powerups[ent->item->giTag] = 
			level.time - ( level.time % 1000 );

		G_LogWeaponPowerup(other->s.number, ent->item->giTag);
	}

	if ( ent->count ) {
		quantity = ent->count;
	} else {
		quantity = ent->item->quantity;
	}

	other->client->ps.powerups[ent->item->giTag] += quantity * 1000;

	if (ent->item->giTag == PW_YSALAMIRI)
	{
		other->client->ps.powerups[PW_FORCE_ENLIGHTENED_LIGHT] = 0;
		other->client->ps.powerups[PW_FORCE_ENLIGHTENED_DARK] = 0;
		other->client->ps.powerups[PW_FORCE_BOON] = 0;
	}

	// give any nearby players a "denied" anti-reward
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		vec3_t		delta;
		float		len;
		vec3_t		forward;
		trace_t		tr;

		client = &level.clients[i];
		if ( client == other->client ) {
			continue;
		}
		if ( client->pers.connected == CON_DISCONNECTED ) {
			continue;
		}
		if ( client->ps.stats[STAT_HEALTH] <= 0 ) {
			continue;
		}

    // if same team in team game, no sound
    // cannot use OnSameTeam as it expects to g_entities, not clients
  	if ( g_gametype.integer >= GT_TEAM && other->client->sess.sessionTeam == client->sess.sessionTeam  ) {
      continue;
    }

		// if too far away, no sound
		VectorSubtract( ent->s.pos.trBase, client->ps.origin, delta );
		len = VectorNormalize( delta );
		if ( len > 192 ) {
			continue;
		}

		// if not facing, no sound
		AngleVectors( client->ps.viewangles, forward, NULL, NULL );
		if ( DotProduct( delta, forward ) < 0.4 ) {
			continue;
		}

		// if not line of sight, no sound
		trap_Trace( &tr, client->ps.origin, NULL, NULL, ent->s.pos.trBase, ENTITYNUM_NONE, CONTENTS_SOLID );
		if ( tr.fraction != 1.0 ) {
			continue;
		}

		// anti-reward
		client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_DENIEDREWARD;
	}
	return RESPAWN_POWERUP;
}

//======================================================================

int Pickup_Holdable( gentity_t *ent, gentity_t *other ) {

	other->client->ps.stats[STAT_HOLDABLE_ITEM] = ent->item - bg_itemlist;

	other->client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << ent->item->giTag);

	G_LogWeaponItem(other->s.number, ent->item->giTag);

	return adjustRespawnTime(RESPAWN_HOLDABLE, ent->item->giType, ent->item->giTag);
}


//======================================================================

void Add_Ammo (gentity_t *ent, int weapon, int count)
{
	ent->client->ps.ammo[weapon] += count;
	if ( ent->client->ps.ammo[weapon] > ammoData[weapon].max ) {
		ent->client->ps.ammo[weapon] = ammoData[weapon].max;
	}
}

int Pickup_Ammo (gentity_t *ent, gentity_t *other)
{
	int		quantity;

	if ( ent->count ) {
		quantity = ent->count;
	} else {
		quantity = ent->item->quantity;
	}

	Add_Ammo (other, ent->item->giTag, quantity);

	return adjustRespawnTime(RESPAWN_AMMO, ent->item->giType, ent->item->giTag);
}

//======================================================================


int Pickup_Weapon (gentity_t *ent, gentity_t *other) {
	int		quantity;

	if ( ent->count < 0 ) {
		quantity = 0; // None for you, sir!
	} else {
		if ( ent->count ) {
			quantity = ent->count;
		} else {
			quantity = ent->item->quantity;
		}

		// dropped items and teamplay weapons always have full ammo
		if ( ! (ent->flags & FL_DROPPED_ITEM) && g_gametype.integer != GT_TEAM ) {
			// respawning rules

			// New method:  If the player has less than half the minimum, give them the minimum, else add 1/2 the min.

			// drop the quantity if the already have over the minimum
			if ( other->client->ps.ammo[ ent->item->giTag ] < quantity*0.5 ) {
				quantity = quantity - other->client->ps.ammo[ ent->item->giTag ];
			} else {
				quantity = quantity*0.5;		// only add half the value.
			}

			// Old method:  If the player has less than the minimum, give them the minimum, else just add 1.
/*
			// drop the quantity if the already have over the minimum
			if ( other->client->ps.ammo[ ent->item->giTag ] < quantity ) {
				quantity = quantity - other->client->ps.ammo[ ent->item->giTag ];
			} else {
				quantity = 1;		// only add a single shot
			}
			*/
		}
	}

	// add the weapon
	other->client->ps.stats[STAT_WEAPONS] |= ( 1 << ent->item->giTag );

	//Add_Ammo( other, ent->item->giTag, quantity );
	Add_Ammo( other, weaponData[ent->item->giTag].ammoIndex, quantity );

	G_LogWeaponPickup(other->s.number, ent->item->giTag);
	
	// team deathmatch has slow weapon respawns
	if ( g_gametype.integer == GT_TEAM ) 
	{
		return adjustRespawnTime(RESPAWN_TEAM_WEAPON, ent->item->giType, ent->item->giTag);
	}

	return adjustRespawnTime(g_weaponRespawn.integer, ent->item->giType, ent->item->giTag);
}


//======================================================================

int Pickup_Health (gentity_t *ent, gentity_t *other) {
	int			max;
	int			quantity;

	// small and mega healths will go over the max
	if ( ent->item->quantity != 5 && ent->item->quantity != 100 ) {
		max = other->client->ps.stats[STAT_MAX_HEALTH];
	} else {
		max = other->client->ps.stats[STAT_MAX_HEALTH] * 2;
	}

	if ( ent->count ) {
		quantity = ent->count;
	} else {
		quantity = ent->item->quantity;
	}

	other->health += quantity;

	if (other->health > max ) {
		other->health = max;
	}
	other->client->ps.stats[STAT_HEALTH] = other->health;

	if ( ent->item->quantity == 100 ) {		// mega health respawns slow
		return RESPAWN_MEGAHEALTH;
	}

	return adjustRespawnTime(RESPAWN_HEALTH, ent->item->giType, ent->item->giTag);
}

//======================================================================

int Pickup_Armor( gentity_t *ent, gentity_t *other ) 
{
	other->client->ps.stats[STAT_ARMOR] += ent->item->quantity;
	if ( other->client->ps.stats[STAT_ARMOR] > other->client->ps.stats[STAT_MAX_HEALTH] * ent->item->giTag ) 
	{
		other->client->ps.stats[STAT_ARMOR] = other->client->ps.stats[STAT_MAX_HEALTH] * ent->item->giTag;
	}

	return adjustRespawnTime(RESPAWN_ARMOR, ent->item->giType, ent->item->giTag);
}

//======================================================================

/*
===============
RespawnItem
===============
*/
void RespawnItem( gentity_t *ent ) {
	// randomly select from teamed entities
	if (ent->team) {
		gentity_t	*master;
		int	count;
		int choice;

		if ( !ent->teammaster ) {
			G_Error( "RespawnItem: bad teammaster");
		}
		master = ent->teammaster;

		for (count = 0, ent = master; ent; ent = ent->teamchain, count++)
			;

		choice = rand() % count;

		for (count = 0, ent = master; count < choice; ent = ent->teamchain, count++)
			;
	}

	ent->r.contents = CONTENTS_TRIGGER;
	//ent->s.eFlags &= ~EF_NODRAW;
	ent->s.eFlags &= ~(EF_NODRAW | EF_ITEMPLACEHOLDER);
	ent->r.svFlags &= ~SVF_NOCLIENT;
	trap_LinkEntity (ent);

	if ( ent->item->giType == IT_POWERUP ) {
		// play powerup spawn sound to all clients
		gentity_t	*te;

		// if the powerup respawn sound should Not be global
		if (ent->speed) {
			te = G_TempEntity( ent->s.pos.trBase, EV_GENERAL_SOUND );
		}
		else {
			te = G_TempEntity( ent->s.pos.trBase, EV_GLOBAL_SOUND );
		}
		te->s.eventParm = G_SoundIndex( "sound/items/respawn1" );
		te->r.svFlags |= SVF_BROADCAST;
	}

	// play the normal respawn sound only to nearby clients
	G_AddEvent( ent, EV_ITEM_RESPAWN, 0 );

	ent->nextthink = 0;
}


/*
===============
Touch_Item
===============
*/
void Touch_Item (gentity_t *ent, gentity_t *other, trace_t *trace) {
	int			respawn;
	qboolean	predict;

	if (ent->s.eFlags & EF_ITEMPLACEHOLDER)
	{
		return;
	}

	if (ent->s.eFlags & EF_NODRAW)
	{
		return;
	}

	if (ent->item->giType == IT_WEAPON &&
		ent->s.powerups &&
		ent->s.powerups < level.time)
	{
		ent->s.generic1 = 0;
		ent->s.powerups = 0;
	}

	if (!other->client)
		return;
	if (other->health < 1)
		return;		// dead people can't pickup

	if (ent->item->giType == IT_POWERUP &&
		(ent->item->giTag == PW_FORCE_ENLIGHTENED_LIGHT || ent->item->giTag == PW_FORCE_ENLIGHTENED_DARK))
	{
		if (ent->item->giTag == PW_FORCE_ENLIGHTENED_LIGHT)
		{
			if (other->client->ps.fd.forceSide != FORCE_LIGHTSIDE)
			{
				return;
			}
		}
		else
		{
			if (other->client->ps.fd.forceSide != FORCE_DARKSIDE)
			{
				return;
			}
		}
	}

	// the same pickup rules are used for client side and server side
	if ( !BG_CanItemBeGrabbed( g_gametype.integer, &ent->s, &other->client->ps ) ) {
		return;
	}

	G_LogPrintf( "Item: %i %s\n", other->s.number, ent->item->classname );

	predict = other->client->pers.predictItemPickup;

	// call the item-specific pickup function
	switch( ent->item->giType ) {
	case IT_WEAPON:
		respawn = Pickup_Weapon(ent, other);
//		predict = qfalse;
		predict = qtrue;
		break;
	case IT_AMMO:
		respawn = Pickup_Ammo(ent, other);
		if (ent->item->giTag == AMMO_THERMAL || ent->item->giTag == AMMO_TRIPMINE || ent->item->giTag == AMMO_DETPACK)
		{
			int weapForAmmo = 0;

			if (ent->item->giTag == AMMO_THERMAL)
			{
				weapForAmmo = WP_THERMAL;
			}
			else if (ent->item->giTag == AMMO_TRIPMINE)
			{
				weapForAmmo = WP_TRIP_MINE;
			}
			else
			{
				weapForAmmo = WP_DET_PACK;
			}

			if (other && other->client && other->client->ps.ammo[weaponData[weapForAmmo].ammoIndex] > 0 )
			{
				other->client->ps.stats[STAT_WEAPONS] |= (1 << weapForAmmo);
			}
		}
//		predict = qfalse;
		predict = qtrue;
		break;
	case IT_ARMOR:
		respawn = Pickup_Armor(ent, other);
//		predict = qfalse;
		predict = qtrue;
		break;
	case IT_HEALTH:
		respawn = Pickup_Health(ent, other);
//		predict = qfalse;
		predict = qtrue;
		break;
	case IT_POWERUP:
		respawn = Pickup_Powerup(ent, other);
		predict = qfalse;
//		predict = qtrue;
		break;
	case IT_TEAM:
		respawn = Pickup_Team(ent, other);
		break;
	case IT_HOLDABLE:
		respawn = Pickup_Holdable(ent, other);
		break;
	default:
		return;
	}

	if ( !respawn ) {
		return;
	}

	// play the normal pickup sound
	if (predict) {
		if (other->client)
		{
			BG_AddPredictableEventToPlayerstate( EV_ITEM_PICKUP, ent->s.number, &other->client->ps);
		}
		else
		{
			G_AddPredictableEvent( other, EV_ITEM_PICKUP, ent->s.number );
		}
	} else {
		G_AddEvent( other, EV_ITEM_PICKUP, ent->s.number );
	}

	// powerup pickups are global broadcasts
	if ( /*ent->item->giType == IT_POWERUP ||*/ ent->item->giType == IT_TEAM) {
		// if we want the global sound to play
		if (!ent->speed) {
			gentity_t	*te;

			te = G_TempEntity( ent->s.pos.trBase, EV_GLOBAL_ITEM_PICKUP );
			te->s.eventParm = ent->s.modelindex;
			te->r.svFlags |= SVF_BROADCAST;
		} else {
			gentity_t	*te;

			te = G_TempEntity( ent->s.pos.trBase, EV_GLOBAL_ITEM_PICKUP );
			te->s.eventParm = ent->s.modelindex;
			// only send this temp entity to a single client
			te->r.svFlags |= SVF_SINGLECLIENT;
			te->r.singleClient = other->s.number;
		}
	}

	// fire item targets
	G_UseTargets (ent, other);

	// wait of -1 will not respawn
	if ( ent->wait == -1 ) {
		ent->r.svFlags |= SVF_NOCLIENT;
		ent->s.eFlags |= EF_NODRAW;
		ent->r.contents = 0;
		ent->unlinkAfterEvent = qtrue;
		return;
	}

	// non zero wait overrides respawn time
	if ( ent->wait ) {
		respawn = ent->wait;
	}

	// random can be used to vary the respawn time
	if ( ent->random ) {
		respawn += crandom() * ent->random;
		if ( respawn < 1 ) {
			respawn = 1;
		}
	}

	// dropped items will not respawn
	if ( ent->flags & FL_DROPPED_ITEM ) {
		ent->freeAfterEvent = qtrue;
	}

	// picked up items still stay around, they just don't
	// draw anything.  This allows respawnable items
	// to be placed on movers.
	if (!(ent->flags & FL_DROPPED_ITEM) && (ent->item->giType==IT_WEAPON || ent->item->giType==IT_POWERUP))
	{
		ent->s.eFlags |= EF_ITEMPLACEHOLDER;
		ent->s.eFlags &= ~EF_NODRAW;
	}
	else
	{
		ent->s.eFlags |= EF_NODRAW;
		ent->r.svFlags |= SVF_NOCLIENT;
	}
	ent->r.contents = 0;

	// ZOID
	// A negative respawn times means to never respawn this item (but don't 
	// delete it).  This is used by items that are respawned by third party 
	// events such as ctf flags
	if ( respawn <= 0 ) {
		ent->nextthink = 0;
		ent->think = 0;
	} else {
		ent->nextthink = level.time + respawn * 1000;
		ent->think = RespawnItem;
	}
	trap_LinkEntity( ent );
}


//======================================================================

/*
================
LaunchItem

Spawns an item and tosses it forward
================
*/
gentity_t *LaunchItem( gitem_t *item, vec3_t origin, vec3_t velocity ) {
	gentity_t	*dropped;

	dropped = G_Spawn();

	dropped->s.eType = ET_ITEM;
	dropped->s.modelindex = item - bg_itemlist;	// store item number in modelindex
	if (dropped->s.modelindex < 0)
	{
		dropped->s.modelindex = 0;
	}
	dropped->s.modelindex2 = 1; // This is non-zero is it's a dropped item

	dropped->classname = item->classname;
	dropped->item = item;
	VectorSet (dropped->r.mins, -ITEM_RADIUS, -ITEM_RADIUS, -ITEM_RADIUS);
	VectorSet (dropped->r.maxs, ITEM_RADIUS, ITEM_RADIUS, ITEM_RADIUS);

	dropped->r.contents = CONTENTS_TRIGGER;

	dropped->touch = Touch_Item;

	G_SetOrigin( dropped, origin );
	dropped->s.pos.trType = TR_GRAVITY;
	dropped->s.pos.trTime = level.time;
	VectorCopy( velocity, dropped->s.pos.trDelta );

	dropped->s.eFlags |= EF_BOUNCE_HALF;
	if ((g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY) && item->giType == IT_TEAM) { // Special case for CTF flags
		dropped->think = Team_DroppedFlagThink;
		dropped->nextthink = level.time + 30000;
		Team_CheckDroppedItem( dropped );

		//rww - so bots know
		if (strcmp(dropped->classname, "team_CTF_redflag") == 0)
		{
			droppedRedFlag = dropped;
		}
		else if (strcmp(dropped->classname, "team_CTF_blueflag") == 0)
		{
			droppedBlueFlag = dropped;
		}
	} else { // auto-remove after 30 seconds
		dropped->think = G_FreeEntity;
		dropped->nextthink = level.time + 30000;
	}

	dropped->flags = FL_DROPPED_ITEM;

	if (item->giType == IT_WEAPON || item->giType == IT_POWERUP)
	{
		dropped->s.eFlags |= EF_DROPPEDWEAPON;
	}

	vectoangles(velocity, dropped->s.angles);
	dropped->s.angles[PITCH] = 0;

	if (item->giTag == WP_TRIP_MINE ||
		item->giTag == WP_DET_PACK)
	{
		dropped->s.angles[PITCH] = -90;
	}

	if (item->giTag != WP_BOWCASTER &&
		item->giTag != WP_DET_PACK &&
		item->giTag != WP_THERMAL)
	{
		dropped->s.angles[ROLL] = -90;
	}

	dropped->physicsObject = qtrue;

	trap_LinkEntity (dropped);

	return dropped;
}

/*
================
Drop_Item

Spawns an item and tosses it forward
================
*/
gentity_t *Drop_Item( gentity_t *ent, gitem_t *item, float angle ) {
	vec3_t	velocity;
	vec3_t	angles;

	VectorCopy( ent->s.apos.trBase, angles );
	angles[YAW] += angle;
	angles[PITCH] = 0;	// always forward

	AngleVectors( angles, velocity, NULL, NULL );
	VectorScale( velocity, 150, velocity );
	velocity[2] += 200 + crandom() * 50;
	
	return LaunchItem( item, ent->s.pos.trBase, velocity );
}


/*
================
Use_Item

Respawn the item
================
*/
void Use_Item( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	RespawnItem( ent );
}

//======================================================================

/*
================
FinishSpawningItem

Traces down to find where an item should rest, instead of letting them
free fall from their spawn points
================
*/
void FinishSpawningItem( gentity_t *ent ) {
	trace_t		tr;
	vec3_t		dest;
	int			wDisable = 0;
//	gitem_t		*item;

//	VectorSet( ent->r.mins, -ITEM_RADIUS, -ITEM_RADIUS, -ITEM_RADIUS );
//	VectorSet( ent->r.maxs, ITEM_RADIUS, ITEM_RADIUS, ITEM_RADIUS );

	if (g_gametype.integer == GT_TOURNAMENT)
	{
		wDisable = g_duelWeaponDisable.integer;
	}
	else
	{
		wDisable = g_weaponDisable.integer;
	}

	if (ent->item->giType == IT_WEAPON &&
		wDisable &&
		(wDisable & (1 << ent->item->giTag)))
	{
		if (g_gametype.integer != GT_JEDIMASTER)
		{
			G_FreeEntity( ent );
			return;
		}
	}

	if (g_gametype.integer != GT_JEDIMASTER)
	{
		if (HasSetSaberOnly())
		{
			if (ent->item->giType == IT_AMMO)
			{
				G_FreeEntity( ent );
				return;
			}

			if (ent->item->giType == IT_HOLDABLE)
			{
				if (ent->item->giTag == HI_SEEKER ||
					ent->item->giTag == HI_SHIELD ||
					ent->item->giTag == HI_SENTRY_GUN)
				{
					G_FreeEntity( ent );
					return;
				}
			}
		}
	}
	else
	{ //no powerups in jedi master
		if (ent->item->giType == IT_POWERUP)
		{
			G_FreeEntity(ent);
			return;
		}
	}

	if (g_gametype.integer == GT_HOLOCRON)
	{
		if (ent->item->giType == IT_POWERUP)
		{
			if (ent->item->giTag == PW_FORCE_ENLIGHTENED_LIGHT ||
				ent->item->giTag == PW_FORCE_ENLIGHTENED_DARK)
			{
				G_FreeEntity(ent);
				return;
			}
		}
	}

	if (g_forcePowerDisable.integer)
	{ //if force powers disabled, don't add force powerups
		if (ent->item->giType == IT_POWERUP)
		{
			if (ent->item->giTag == PW_FORCE_ENLIGHTENED_LIGHT ||
				ent->item->giTag == PW_FORCE_ENLIGHTENED_DARK ||
				ent->item->giTag == PW_FORCE_BOON)
			{
				G_FreeEntity(ent);
				return;
			}
		}
	}

	if (g_gametype.integer == GT_TOURNAMENT)
	{
		if ( ent->item->giType == IT_ARMOR ||
			ent->item->giType == IT_HEALTH ||
			(ent->item->giType == IT_HOLDABLE && ent->item->giTag == HI_MEDPAC) )
		{
			G_FreeEntity(ent);
			return;
		}
	}

	if (g_gametype.integer != GT_CTF &&
		g_gametype.integer != GT_CTY &&
		ent->item->giType == IT_TEAM)
	{
		int killMe = 0;

		switch (ent->item->giTag)
		{
		case PW_REDFLAG:
			killMe = 1;
			break;
		case PW_BLUEFLAG:
			killMe = 1;
			break;
		case PW_NEUTRALFLAG:
			killMe = 1;
			break;
		default:
			break;
		}

		if (killMe)
		{
			G_FreeEntity( ent );
			return;
		}
	}

	VectorSet (ent->r.mins, -8, -8, -0);
	VectorSet (ent->r.maxs, 8, 8, 16);

	ent->s.eType = ET_ITEM;
	ent->s.modelindex = ent->item - bg_itemlist;		// store item number in modelindex
	ent->s.modelindex2 = 0; // zero indicates this isn't a dropped item

	ent->r.contents = CONTENTS_TRIGGER;
	ent->touch = Touch_Item;
	// useing an item causes it to respawn
	ent->use = Use_Item;

	// create a Ghoul2 model if the world model is a glm
/*	item = &bg_itemlist[ ent->s.modelindex ];
	if (!stricmp(&item->world_model[0][strlen(item->world_model[0]) - 4], ".glm"))
	{
		trap_G2API_InitGhoul2Model(&ent->s, item->world_model[0], G_ModelIndex(item->world_model[0] ), 0, 0, 0, 0);
		ent->s.radius = 60;
	}
*/
	if ( ent->spawnflags & 1 ) {
		// suspended
		G_SetOrigin( ent, ent->s.origin );
	} else {
		// drop to floor

		//if it is directly even with the floor it will return startsolid, so raise up by 0.1
		//and temporarily subtract 0.1 from the z maxs so that going up doesn't push into the ceiling
		ent->s.origin[2] += 0.1;
		ent->r.maxs[2] -= 0.1;

		VectorSet( dest, ent->s.origin[0], ent->s.origin[1], ent->s.origin[2] - 4096 );
		trap_Trace( &tr, ent->s.origin, ent->r.mins, ent->r.maxs, dest, ent->s.number, MASK_SOLID );
		if ( tr.startsolid ) {
			G_Printf ("FinishSpawningItem: %s startsolid at %s\n", ent->classname, vtos(ent->s.origin));
			G_FreeEntity( ent );
			return;
		}

		//add the 0.1 back after the trace
		ent->r.maxs[2] += 0.1;

		// allow to ride movers
		ent->s.groundEntityNum = tr.entityNum;

		G_SetOrigin( ent, tr.endpos );
	}

	// team slaves and targeted items aren't present at start
	if ( ( ent->flags & FL_TEAMSLAVE ) || ent->targetname ) {
		ent->s.eFlags |= EF_NODRAW;
		ent->r.contents = 0;
		return;
	}

	// powerups don't spawn in for a while
	/*
	if ( ent->item->giType == IT_POWERUP ) {
		float	respawn;

		respawn = 45 + crandom() * 15;
		ent->s.eFlags |= EF_NODRAW;
		ent->r.contents = 0;
		ent->nextthink = level.time + respawn * 1000;
		ent->think = RespawnItem;
		return;
	}
	*/

	trap_LinkEntity (ent);
}


qboolean	itemRegistered[MAX_ITEMS];

/*
==================
G_CheckTeamItems
==================
*/
void G_CheckTeamItems( void ) {

	// Set up team stuff
	Team_InitGame();

	if( g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY ) {
		gitem_t	*item;

		// check for the two flags
		item = BG_FindItem( "team_CTF_redflag" );
		if ( !item || !itemRegistered[ item - bg_itemlist ] ) {
			G_Printf( S_COLOR_YELLOW "WARNING: No team_CTF_redflag in map" );
		}
		item = BG_FindItem( "team_CTF_blueflag" );
		if ( !item || !itemRegistered[ item - bg_itemlist ] ) {
			G_Printf( S_COLOR_YELLOW "WARNING: No team_CTF_blueflag in map" );
		}
	}
}

/*
==============
ClearRegisteredItems
==============
*/
void ClearRegisteredItems( void ) {
	memset( itemRegistered, 0, sizeof( itemRegistered ) );

	// players always start with the base weapon
	RegisterItem( BG_FindItemForWeapon( WP_BRYAR_PISTOL ) );
	RegisterItem( BG_FindItemForWeapon( WP_STUN_BATON ) );
	RegisterItem( BG_FindItemForWeapon( WP_SABER ) );
}

/*
===============
RegisterItem

The item will be added to the precache list
===============
*/
void RegisterItem( gitem_t *item ) {
	if ( !item ) {
		G_Error( "RegisterItem: NULL" );
	}
	itemRegistered[ item - bg_itemlist ] = qtrue;
}


/*
===============
SaveRegisteredItems

Write the needed items to a config string
so the client will know which ones to precache
===============
*/
void SaveRegisteredItems( void ) {
	char	string[MAX_ITEMS+1];
	int		i;
	int		count;

	count = 0;
	for ( i = 0 ; i < bg_numItems ; i++ ) {
		if ( itemRegistered[i] ) {
			count++;
			string[i] = '1';
		} else {
			string[i] = '0';
		}
	}
	string[ bg_numItems ] = 0;

	G_Printf( "%i items registered\n", count );
	trap_SetConfigstring(CS_ITEMS, string);
}

/*
============
G_ItemDisabled
============
*/
int G_ItemDisabled( gitem_t *item ) {

	char name[128];

	Com_sprintf(name, sizeof(name), "disable_%s", item->classname);
	return trap_Cvar_VariableIntegerValue( name );
}

/*
============
G_SpawnItem

Sets the clipping size and plants the object on the floor.

Items can't be immediately dropped to floor, because they might
be on an entity that hasn't spawned yet.
============
*/
void G_SpawnItem (gentity_t *ent, gitem_t *item) {
	G_SpawnFloat( "random", "0", &ent->random );
	G_SpawnFloat( "wait", "0", &ent->wait );

	RegisterItem( item );
	if ( G_ItemDisabled(item) )
		return;

	ent->item = item;
	// some movers spawn on the second frame, so delay item
	// spawns until the third frame so they can ride trains
	ent->nextthink = level.time + FRAMETIME * 2;
	ent->think = FinishSpawningItem;

	ent->physicsBounce = 0.50;		// items are bouncy

	if ( item->giType == IT_POWERUP ) {
		G_SoundIndex( "sound/items/respawn1" );
		G_SpawnFloat( "noglobalsound", "0", &ent->speed);
	}
}


/*
================
G_BounceItem

================
*/
void G_BounceItem( gentity_t *ent, trace_t *trace ) {
	vec3_t	velocity;
	float	dot;
	int		hitTime;

	// reflect the velocity on the trace plane
	hitTime = level.previousTime + ( level.time - level.previousTime ) * trace->fraction;
	BG_EvaluateTrajectoryDelta( &ent->s.pos, hitTime, velocity );
	dot = DotProduct( velocity, trace->plane.normal );
	VectorMA( velocity, -2*dot, trace->plane.normal, ent->s.pos.trDelta );

	// cut the velocity to keep from bouncing forever
	VectorScale( ent->s.pos.trDelta, ent->physicsBounce, ent->s.pos.trDelta );

	if ((ent->s.weapon == WP_DET_PACK && ent->s.eType == ET_GENERAL && ent->physicsObject))
	{ //detpacks only
		if (ent->touch)
		{
			ent->touch(ent, &g_entities[trace->entityNum], trace);
			return;
		}
	}

	// check for stop
	if ( trace->plane.normal[2] > 0 && ent->s.pos.trDelta[2] < 40 ) {
		trace->endpos[2] += 1.0;	// make sure it is off ground
		SnapVector( trace->endpos );
		G_SetOrigin( ent, trace->endpos );
		ent->s.groundEntityNum = trace->entityNum;
		return;
	}

	VectorAdd( ent->r.currentOrigin, trace->plane.normal, ent->r.currentOrigin);
	VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );
	ent->s.pos.trTime = level.time;

	if (ent->s.eType == ET_HOLOCRON ||
		(ent->s.shouldtarget && ent->s.eType == ET_GENERAL && ent->physicsObject))
	{ //holocrons and sentry guns
		if (ent->touch)
		{
			ent->touch(ent, &g_entities[trace->entityNum], trace);
		}
	}
}


/*
================
G_RunItem

================
*/
void G_RunItem( gentity_t *ent ) {
	vec3_t		origin;
	trace_t		tr;
	int			contents;
	int			mask;

	// if groundentity has been set to -1, it may have been pushed off an edge
	if ( ent->s.groundEntityNum == -1 ) {
		if ( ent->s.pos.trType != TR_GRAVITY ) {
			ent->s.pos.trType = TR_GRAVITY;
			ent->s.pos.trTime = level.time;
		}
	}

	if ( ent->s.pos.trType == TR_STATIONARY ) {
		// check think function
		G_RunThink( ent );
		return;
	}

	// get current position
	BG_EvaluateTrajectory( &ent->s.pos, level.time, origin );

	// trace a line from the previous position to the current position
	if ( ent->clipmask ) {
		mask = ent->clipmask;
	} else {
		mask = MASK_PLAYERSOLID & ~CONTENTS_BODY;//MASK_SOLID;
	}
	trap_Trace( &tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, origin, 
		ent->r.ownerNum, mask );

	VectorCopy( tr.endpos, ent->r.currentOrigin );

	if ( tr.startsolid ) {
		tr.fraction = 0;
	}

	trap_LinkEntity( ent );	// FIXME: avoid this for stationary?

	// check think function
	G_RunThink( ent );

	if ( tr.fraction == 1 ) {
		return;
	}

	// if it is in a nodrop volume, remove it
	contents = trap_PointContents( ent->r.currentOrigin, -1 );
	if ( contents & CONTENTS_NODROP ) {
		if (ent->item && ent->item->giType == IT_TEAM) {
			Team_FreeEntity(ent);
		} else {
			G_FreeEntity( ent );
		}
		return;
	}

	G_BounceItem( ent, &tr );
}

