// Copyright (C) 1999-2000 Id Software, Inc.
//
// g_combat.c

#include "g_local.h"

//rww - pd
void BotDamageNotification(gclient_t *bot, gentity_t *attacker);
//end rww

void ThrowSaberToAttacker(gentity_t *self, gentity_t *attacker);

void ObjectDie (gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath )
{
	if(self->target)
	{
		G_UseTargets(self, attacker);
	}

	//remove my script_targetname
	G_FreeEntity( self );
}

/*
int G_GetHitLocFromSurfName( gentity_t *ent, const char *surfName, vec3_t point )
{
	if ( !surfName || !surfName[0] )
	{
		return HL_NONE;
	}
	if ( !Q_strncmp( "hips", surfName, 4 ) )
	{//FIXME: test properly for legs
		if ( Q_irand( 0, 1 ) )
		{
			return HL_WAIST;
		}
		else if ( Q_irand( 0, 1 ) )
		{
			return HL_LEG_RT;
		}
		else
		{
			return HL_LEG_LT;
		}
	}
	else if ( !Q_strncmp( "torso", surfName, 5 ) )
	{
		if ( !ent->client )
		{
			return HL_CHEST;
		}
		else
		{
			vec3_t	t_fwd, t_rt, t_up, dirToImpact;
			float frontSide, rightSide, upSide;
			AngleVectors( ent->client->renderInfo.torsoAngles, t_fwd, t_rt, t_up );
			VectorSubtract( point, ent->client->renderInfo.torsoPoint, dirToImpact );
			frontSide = DotProduct( t_fwd, dirToImpact );
			rightSide = DotProduct( t_rt, dirToImpact );
			upSide = DotProduct( t_up, dirToImpact );
			if ( upSide < 0 )
			{//hit at waist
				return HL_WAIST;
			}
			else
			{//hit on upper torso
				if ( rightSide > 10 )
				{
					return HL_ARM_RT;
				}
				else if ( rightSide < -10 )
				{
					return HL_ARM_LT;
				}
				else if ( rightSide > 4 )
				{
					if ( frontSide > 0 )
					{
						return HL_CHEST_RT;
					}
					else
					{
						return HL_BACK_RT;
					}
				}
				else if ( rightSide < -4 )
				{
					if ( frontSide > 0 )
					{
						return HL_CHEST_LT;
					}
					else
					{
						return HL_BACK_LT;
					}
				}
				else if ( upSide > 6 )
				{
					return HL_HEAD;
				}
				else if ( frontSide > 0 )
				{
					return HL_CHEST;
				}
				else
				{
					return HL_BACK;
				}
			}
		}
	}
	else if ( !Q_strncmp( "head", surfName, 4 ) )
	{
		return HL_HEAD;
	}
	else if ( !Q_strncmp( "r_arm", surfName, 5 ) )
	{
		return HL_ARM_RT;
	}
	else if ( !Q_strncmp( "l_arm", surfName, 5 ) )
	{
		return HL_ARM_LT;
	}
	else if ( !Q_strncmp( "r_leg", surfName, 5 ) )
	{
		if ( ent->client && DistanceSquared( ent->client->renderInfo.crotchPoint, point ) > 144 )
		{//not close enough to the pelvis to be a dismemberment hit and we don't dismember at the ankle or knee
			return HL_FOOT_RT;
		}
		return HL_LEG_RT;
	}
	else if ( !Q_strncmp( "l_leg", surfName, 5 ) )
	{
		if ( ent->client && DistanceSquared( ent->client->renderInfo.crotchPoint, point ) > 144 )
		{//not close enough to the pelvis to be a dismemberment hit and we don't dismember at the ankle or knee
			return HL_FOOT_LT;
		}
		return HL_LEG_LT;
	}
	else if ( !Q_strncmp( "r_hand", surfName, 6 ) )
	{
		return HL_HAND_RT;
	}
	else if ( !Q_strncmp( "l_hand", surfName, 6 ) )
	{
		return HL_HAND_LT;
	}
	return HL_NONE;
}
*/
int G_GetHitLocation(gentity_t *target, vec3_t ppoint)
{
	vec3_t			point, point_dir;
	vec3_t			forward, right, up;
	vec3_t			tangles, tcenter;
	float			tradius;
	float			udot, fdot, rdot;
	int				Vertical, Forward, Lateral;
	int				HitLoc;

	// Get target forward, right and up.
	if(target->client)
	{
		// Ignore player's pitch and roll.
		VectorSet(tangles, 0, target->r.currentAngles[YAW], 0);
	}

	AngleVectors(tangles, forward, right, up);

	// Get center of target.
	VectorAdd(target->r.absmin, target->r.absmax, tcenter);
	VectorScale(tcenter, 0.5, tcenter);

	// Get radius width of target.
	tradius = (fabs(target->r.maxs[0]) + fabs(target->r.maxs[1]) + fabs(target->r.mins[0]) + fabs(target->r.mins[1]))/4;

	// Get impact point.
	if(ppoint && !VectorCompare(ppoint, vec3_origin))
	{
		VectorCopy(ppoint, point);
	}
	else
	{
		return HL_NONE;
	}

/*
//get impact dir
	if(pdir && !VectorCompare(pdir, vec3_origin))
	{
		VectorCopy(pdir, dir);
	}
	else
	{
		return;
	}

//put point at controlled distance from center
	VectorSubtract(point, tcenter, tempvec);
	tempvec[2] = 0;
	hdist = VectorLength(tempvec);

	VectorMA(point, hdist - tradius, dir, point);
	//now a point on the surface of a cylinder with a radius of tradius
*/	
	VectorSubtract(point, tcenter, point_dir);
	VectorNormalize(point_dir);

	// Get bottom to top (vertical) position index
	udot = DotProduct(up, point_dir);
	if(udot>.800)
	{
		Vertical = 4;
	}
	else if(udot>.400)
	{
		Vertical = 3;
	}
	else if(udot>-.333)
	{
		Vertical = 2;
	}
	else if(udot>-.666)
	{
		Vertical = 1;
	}
	else
	{
		Vertical = 0;
	}

	// Get back to front (forward) position index.
	fdot = DotProduct(forward, point_dir);
	if(fdot>.666)
	{
		Forward = 4;
	}
	else if(fdot>.333)
	{
		Forward = 3;
	}
	else if(fdot>-.333)
	{
		Forward = 2;
	}
	else if(fdot>-.666)
	{
		Forward = 1;
	}
	else
	{
		Forward = 0;
	}

	// Get left to right (lateral) position index.
	rdot = DotProduct(right, point_dir);
	if(rdot>.666)
	{
		Lateral = 4;
	}
	else if(rdot>.333)
	{
		Lateral = 3;
	}
	else if(rdot>-.333)
	{
		Lateral = 2;
	}
	else if(rdot>-.666)
	{
		Lateral = 1;
	}
	else
	{
		Lateral = 0;
	}

	HitLoc = Vertical * 25 + Forward * 5 + Lateral;

	if(HitLoc <= 10)
	{
		// Feet.
		if ( rdot > 0 )
		{
			return HL_FOOT_RT;
		}
		else
		{
			return HL_FOOT_LT;
		}
	}
	else if(HitLoc <= 50)
	{
		// Legs.
		if ( rdot > 0 )
		{
			return HL_LEG_RT;
		}
		else
		{
			return HL_LEG_LT;
		}
	}
	else if(HitLoc == 56||HitLoc == 60||HitLoc == 61||HitLoc == 65||HitLoc == 66||HitLoc == 70)
	{
		// Hands.
		if ( rdot > 0 )
		{
			return HL_HAND_RT;
		}
		else
		{
			return HL_HAND_LT;
		}
	}
	else if(HitLoc == 83||HitLoc == 87||HitLoc == 88||HitLoc == 92||HitLoc == 93||HitLoc == 97)
	{
		// Arms.
		if ( rdot > 0 )
		{
			return HL_ARM_RT;
		}
		else
		{
			return HL_ARM_LT;
		}
	}
	else if((HitLoc >= 107 && HitLoc <= 109)||(HitLoc >= 112 && HitLoc <= 114)||(HitLoc >= 117 && HitLoc <= 119))
	{
		// Head.
		return HL_HEAD;
	}
	else
	{
		if(udot < 0.3)
		{
			return HL_WAIST;
		}
		else if(fdot < 0)
		{
			if(rdot > 0.4)
			{
				return HL_BACK_RT;
			}
			else if(rdot < -0.4)
			{
				return HL_BACK_LT;
			}
			else if(fdot < 0)
			{
				return HL_BACK;
			}
		}
		else
		{
			if(rdot > 0.3)
			{
				return HL_CHEST_RT;
			}
			else if(rdot < -0.3)
			{
				return HL_CHEST_LT;
			}
			else if(fdot < 0)
			{
				return HL_CHEST;
			}
		}
	}
	return HL_NONE;
}

/*
int G_PickPainAnim( gentity_t *self, vec3_t point, int damage )
{
	switch( G_GetHitLocation( self, point ) )
	{
	case HL_FOOT_RT:
		return BOTH_PAIN12;
		//PAIN12 = right foot
		break;
	case HL_FOOT_LT:
		return -1;
		break;
	case HL_LEG_RT:
		if ( !Q_irand( 0, 1 ) )
		{
			return BOTH_PAIN11;
		}
		else
		{
			return BOTH_PAIN13;
		}
		//PAIN11 = twitch right leg
		//PAIN13 = right knee
		break;
	case HL_LEG_LT:
		return BOTH_PAIN14;
		//PAIN14 = twitch left leg
		break;
	case HL_BACK_RT:
		return BOTH_PAIN7;
		//PAIN7 = med left shoulder
		break;
	case HL_BACK_LT:
		return Q_irand( BOTH_PAIN15, BOTH_PAIN16 );
		//PAIN15 = med right shoulder
		//PAIN16 = twitch right shoulder
		break;
	case HL_BACK:
		if ( !Q_irand( 0, 1 ) )
		{
			return BOTH_PAIN1;
		}
		else
		{
			return BOTH_PAIN5;
		}
		//PAIN1 = back
		//PAIN5 = same as 1
		break;
	case HL_CHEST_RT:
		return BOTH_PAIN3;
		//PAIN3 = long, right shoulder
		break;
	case HL_CHEST_LT:
		return BOTH_PAIN2;
		//PAIN2 = long, left shoulder
		break;
	case HL_WAIST:
	case HL_CHEST:
		if ( !Q_irand( 0, 3 ) )
		{
			return BOTH_PAIN6;
		}
		else if ( !Q_irand( 0, 2 ) )
		{
			return BOTH_PAIN8;
		}
		else if ( !Q_irand( 0, 1 ) )
		{
			return BOTH_PAIN17;
		}
		else
		{
			return BOTH_PAIN19;
		}
		//PAIN6 = gut
		//PAIN8 = chest
		//PAIN17 = twitch crotch
		//PAIN19 = med crotch
		break;
	case HL_ARM_RT:
	case HL_HAND_RT:
		return BOTH_PAIN9;
		//PAIN9 = twitch right arm
		break;
	case HL_ARM_LT:
	case HL_HAND_LT:
		return BOTH_PAIN10;
		//PAIN10 = twitch left arm
		break;
	case HL_HEAD:
		return BOTH_PAIN4;
		//PAIN4 = head
		break;
	default:
		return -1;
		break;
	}
}
*/

void ExplodeDeath( gentity_t *self ) 
{
//	gentity_t	*tent;
	vec3_t		forward;

	self->takedamage = qfalse;//stop chain reaction runaway loops

	self->s.loopSound = 0;

	VectorCopy( self->r.currentOrigin, self->s.pos.trBase );

//	tent = G_TempEntity( self->s.origin, EV_FX_EXPLOSION );
	AngleVectors(self->s.angles, forward, NULL, NULL);

/*	
	if ( self->fxID > 0 )
	{
		G_PlayEffect( self->fxID, self->r.currentOrigin, forward );
	}
	else
	*/

	{
//		CG_SurfaceExplosion( self->r.currentOrigin, forward, 20.0f, 12.0f, ((self->spawnflags&4)==qfalse) );	//FIXME: This needs to be consistent to all exploders!
//		G_Sound(self, self->sounds );
	}
	
	if(self->splashDamage > 0 && self->splashRadius > 0)
	{
		gentity_t *attacker = self;
		if ( self->parent )
		{
			attacker = self->parent;
		}
		G_RadiusDamage( self->r.currentOrigin, attacker, self->splashDamage, self->splashRadius, 
				attacker, MOD_UNKNOWN );
	}

	ObjectDie( self, self, self, 20, 0 );
}


/*
============
ScorePlum
============
*/
void ScorePlum( gentity_t *ent, vec3_t origin, int score ) {
	gentity_t *plum;

	plum = G_TempEntity( origin, EV_SCOREPLUM );
	// only send this temp entity to a single client
	plum->r.svFlags |= SVF_SINGLECLIENT;
	plum->r.singleClient = ent->s.number;
	//
	plum->s.otherEntityNum = ent->s.number;
	plum->s.time = score;
}

/*
============
AddScore

Adds score to both the client and his team
============
*/
void AddScore( gentity_t *ent, vec3_t origin, int score ) {
	if ( !ent->client ) {
		return;
	}
	// no scoring during pre-match warmup
	if ( level.warmupTime ) {
		return;
	}
	// show score plum
	//ScorePlum(ent, origin, score);
	//
	ent->client->ps.persistant[PERS_SCORE] += score;
	if ( g_gametype.integer == GT_TEAM )
		level.teamScores[ ent->client->ps.persistant[PERS_TEAM] ] += score;
	CalculateRanks();
}

/*
=================
TossClientItems

rww - Toss the weapon away from the player in the specified direction
=================
*/
void TossClientWeapon(gentity_t *self, vec3_t direction, float speed)
{
	vec3_t vel;
	gitem_t *item;
	gentity_t *launched;
	int weapon = self->s.weapon;
	int ammoSub;

	if (weapon <= WP_BRYAR_PISTOL)
	{ //can't have this
		return;
	}

	if (weapon == WP_EMPLACED_GUN ||
		weapon == WP_TURRET)
	{
		return;
	}

	// find the item type for this weapon
	item = BG_FindItemForWeapon( weapon );

	ammoSub = (self->client->ps.ammo[weaponData[weapon].ammoIndex] - bg_itemlist[BG_GetItemIndexByTag(weapon, IT_WEAPON)].quantity);

	if (ammoSub < 0)
	{
		int ammoQuan = item->quantity;
		ammoQuan -= (-ammoSub);

		if (ammoQuan <= 0)
		{ //no ammo
			return;
		}
	}

	vel[0] = direction[0]*speed;
	vel[1] = direction[1]*speed;
	vel[2] = direction[2]*speed;

	launched = LaunchItem(item, self->client->ps.origin, vel);

	launched->s.generic1 = self->s.number;
	launched->s.powerups = level.time + 1500;

	launched->count = bg_itemlist[BG_GetItemIndexByTag(weapon, IT_WEAPON)].quantity;

	self->client->ps.ammo[weaponData[weapon].ammoIndex] -= bg_itemlist[BG_GetItemIndexByTag(weapon, IT_WEAPON)].quantity;

	if (self->client->ps.ammo[weaponData[weapon].ammoIndex] < 0)
	{
		launched->count -= (-self->client->ps.ammo[weaponData[weapon].ammoIndex]);
		self->client->ps.ammo[weaponData[weapon].ammoIndex] = 0;
	}

	if ((self->client->ps.ammo[weaponData[weapon].ammoIndex] < 1 && weapon != WP_DET_PACK) ||
		(weapon != WP_THERMAL && weapon != WP_DET_PACK && weapon != WP_TRIP_MINE))
	{
		self->client->ps.stats[STAT_WEAPONS] &= ~(1 << weapon);

		self->s.weapon = 0;
		self->client->ps.weapon = 0;

		G_AddEvent(self, EV_NOAMMO, weapon);
	}
}

/*
=================
TossClientItems

Toss the weapon and powerups for the killed player
=================
*/
void TossClientItems( gentity_t *self ) {
	gitem_t		*item;
	int			weapon;
	float		angle;
	int			i;
	gentity_t	*drop;

	// drop the weapon if not a gauntlet or machinegun
	weapon = self->s.weapon;

	// make a special check to see if they are changing to a new
	// weapon that isn't the mg or gauntlet.  Without this, a client
	// can pick up a weapon, be killed, and not drop the weapon because
	// their weapon change hasn't completed yet and they are still holding the MG.
	if ( weapon == WP_BRYAR_PISTOL) {
		if ( self->client->ps.weaponstate == WEAPON_DROPPING ) {
			weapon = self->client->pers.cmd.weapon;
		}
		if ( !( self->client->ps.stats[STAT_WEAPONS] & ( 1 << weapon ) ) ) {
			weapon = WP_NONE;
		}
	}

	self->s.bolt2 = weapon;

	if ( weapon > WP_BRYAR_PISTOL && 
		weapon != WP_EMPLACED_GUN &&
		weapon != WP_TURRET &&
		self->client->ps.ammo[ weaponData[weapon].ammoIndex ] ) {
		gentity_t *te;

		// find the item type for this weapon
		item = BG_FindItemForWeapon( weapon );

		// tell all clients to remove the weapon model on this guy until he respawns
		te = G_TempEntity( vec3_origin, EV_DESTROY_WEAPON_MODEL );
		te->r.svFlags |= SVF_BROADCAST;
		te->s.eventParm = self->s.number;

		// spawn the item
		Drop_Item( self, item, 0 );
	}

	// drop all the powerups if not in teamplay
	if ( g_gametype.integer != GT_TEAM ) {
		angle = 45;
		for ( i = 1 ; i < PW_NUM_POWERUPS ; i++ ) {
			if ( self->client->ps.powerups[ i ] > level.time ) {
				item = BG_FindItemForPowerup( i );
				if ( !item ) {
					continue;
				}
				drop = Drop_Item( self, item, angle );
				// decide how many seconds it has left
				drop->count = ( self->client->ps.powerups[ i ] - level.time ) / 1000;
				if ( drop->count < 1 ) {
					drop->count = 1;
				}
				angle += 45;
			}
		}
	}
}


/*
==================
LookAtKiller
==================
*/
void LookAtKiller( gentity_t *self, gentity_t *inflictor, gentity_t *attacker ) {
	vec3_t		dir;
	vec3_t		angles;

	if ( attacker && attacker != self ) {
		VectorSubtract (attacker->s.pos.trBase, self->s.pos.trBase, dir);
	} else if ( inflictor && inflictor != self ) {
		VectorSubtract (inflictor->s.pos.trBase, self->s.pos.trBase, dir);
	} else {
		self->client->ps.stats[STAT_DEAD_YAW] = self->s.angles[YAW];
		return;
	}

	self->client->ps.stats[STAT_DEAD_YAW] = vectoyaw ( dir );

	angles[YAW] = vectoyaw ( dir );
	angles[PITCH] = 0; 
	angles[ROLL] = 0;
}

/*
==================
GibEntity
==================
*/
void GibEntity( gentity_t *self, int killer ) {
	G_AddEvent( self, EV_GIB_PLAYER, killer );
	self->takedamage = qfalse;
	self->s.eType = ET_INVISIBLE;
	self->r.contents = 0;
}

void BodyRid(gentity_t *ent)
{
	trap_UnlinkEntity( ent );
	ent->physicsObject = qfalse;
}

/*
==================
body_die
==================
*/
void body_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath ) {
	// NOTENOTE No gibbing right now, this is star wars.
	qboolean doDisint = qfalse;

	if (self->health < (GIB_HEALTH+1))
	{
		self->health = GIB_HEALTH+1;

		if (self->client && (level.time - self->client->respawnTime) < 2000)
		{
			doDisint = qfalse;
		}
		else
		{
			doDisint = qtrue;
		}
	}

	if (self->client && (self->client->ps.eFlags & EF_DISINTEGRATION))
	{
		return;
	}
	else if (self->s.eFlags & EF_DISINTEGRATION)
	{
		return;
	}

	if (doDisint)
	{
		if (self->client)
		{
			self->client->ps.eFlags |= EF_DISINTEGRATION;
			VectorCopy(self->client->ps.origin, self->client->ps.lastHitLoc);
		}
		else
		{
			self->s.eFlags |= EF_DISINTEGRATION;
			VectorCopy(self->r.currentOrigin, self->s.origin2);

			//since it's the corpse entity, tell it to "remove" itself
			self->think = BodyRid;
			self->nextthink = level.time + 1000;
		}
		return;
	}
}


// these are just for logging, the client prints its own messages
char	*modNames[] = {
	"MOD_UNKNOWN",
	"MOD_STUN_BATON",
	"MOD_MELEE",
	"MOD_SABER",
	"MOD_BRYAR_PISTOL",
	"MOD_BRYAR_PISTOL_ALT",
	"MOD_BLASTER",
	"MOD_DISRUPTOR",
	"MOD_DISRUPTOR_SPLASH",
	"MOD_DISRUPTOR_SNIPER",
	"MOD_BOWCASTER",
	"MOD_REPEATER",
	"MOD_REPEATER_ALT",
	"MOD_REPEATER_ALT_SPLASH",
	"MOD_DEMP2",
	"MOD_DEMP2_ALT",
	"MOD_FLECHETTE",
	"MOD_FLECHETTE_ALT_SPLASH",
	"MOD_ROCKET",
	"MOD_ROCKET_SPLASH",
	"MOD_ROCKET_HOMING",
	"MOD_ROCKET_HOMING_SPLASH",
	"MOD_THERMAL",
	"MOD_THERMAL_SPLASH",
	"MOD_TRIP_MINE_SPLASH",
	"MOD_TIMED_MINE_SPLASH",
	"MOD_DET_PACK_SPLASH",
	"MOD_FORCE_DARK",
	"MOD_SENTRY",
	"MOD_WATER",
	"MOD_SLIME",
	"MOD_LAVA",
	"MOD_CRUSH",
	"MOD_TELEFRAG",
	"MOD_FALLING",
	"MOD_SUICIDE",
	"MOD_TARGET_LASER",
	"MOD_TRIGGER_HURT"
};


/*
==================
CheckAlmostCapture
==================
*/
void CheckAlmostCapture( gentity_t *self, gentity_t *attacker ) {
#if 0
	gentity_t	*ent;
	vec3_t		dir;
	char		*classname;

	// if this player was carrying a flag
	if ( self->client->ps.powerups[PW_REDFLAG] ||
		self->client->ps.powerups[PW_BLUEFLAG] ||
		self->client->ps.powerups[PW_NEUTRALFLAG] ) {
		// get the goal flag this player should have been going for
		if ( g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY ) {
			if ( self->client->sess.sessionTeam == TEAM_BLUE ) {
				classname = "team_CTF_blueflag";
			}
			else {
				classname = "team_CTF_redflag";
			}
		}
		else {
			if ( self->client->sess.sessionTeam == TEAM_BLUE ) {
				classname = "team_CTF_redflag";
			}
			else {
				classname = "team_CTF_blueflag";
			}
		}
		ent = NULL;
		do
		{
			ent = G_Find(ent, FOFS(classname), classname);
		} while (ent && (ent->flags & FL_DROPPED_ITEM));
		// if we found the destination flag and it's not picked up
		if (ent && !(ent->r.svFlags & SVF_NOCLIENT) ) {
			// if the player was *very* close
			VectorSubtract( self->client->ps.origin, ent->s.origin, dir );
			if ( VectorLength(dir) < 200 ) {
				self->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_HOLYSHIT;
				if ( attacker->client ) {
					attacker->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_HOLYSHIT;
				}
			}
		}
	}
#endif
}

static int G_PickDeathAnim( gentity_t *self, vec3_t point, int damage, int mod, int hitLoc )
{//FIXME: play dead flop anims on body if in an appropriate _DEAD anim when this func is called
	int deathAnim = -1;
	int max_health;

	if (!self || !self->client)
	{
		return 0;
	}

	max_health = self->client->ps.stats[STAT_MAX_HEALTH];

	if ( hitLoc == HL_NONE )
	{
		hitLoc = G_GetHitLocation( self, point );//self->hitLoc
	}
	//dead flops
	switch( self->client->ps.legsAnim&~ANIM_TOGGLEBIT )
	{
	case BOTH_DEATH1:		//# First Death anim
	case BOTH_DEAD1:
	case BOTH_DEATH2:			//# Second Death anim
	case BOTH_DEAD2:
	case BOTH_DEATH8:			//# 
	case BOTH_DEAD8:
	case BOTH_DEATH13:			//# 
	case BOTH_DEAD13:
	case BOTH_DEATH14:			//# 
	case BOTH_DEAD14:
	case BOTH_DEATH16:			//# 
	case BOTH_DEAD16:
	case BOTH_DEADBACKWARD1:		//# First thrown backward death finished pose
	case BOTH_DEADBACKWARD2:		//# Second thrown backward death finished pose
		deathAnim = -2;
		/*
		if ( PM_FinishedCurrentLegsAnim( self ) )
		{//done with the anim
			deathAnim = BOTH_DEADFLOP2;
		}
		else
		{
			deathAnim = -2;
		}
		break;
	case BOTH_DEADFLOP2:
		deathAnim = BOTH_DEADFLOP2;
		break;
		*/
	case BOTH_DEATH10:			//# 
	case BOTH_DEAD10:
	case BOTH_DEATH15:			//# 
	case BOTH_DEAD15:
	case BOTH_DEADFORWARD1:		//# First thrown forward death finished pose
	case BOTH_DEADFORWARD2:		//# Second thrown forward death finished pose
		deathAnim = -2;
		/*
		if ( PM_FinishedCurrentLegsAnim( self ) )
		{//done with the anim
			deathAnim = BOTH_DEADFLOP1;
		}
		else
		{
			deathAnim = -2;
		}
		break;
		*/
	case BOTH_DEADFLOP1:
		deathAnim = -2;
		//deathAnim = BOTH_DEADFLOP1;
		break;
	case BOTH_DEAD3:				//# Third Death finished pose
	case BOTH_DEAD4:				//# Fourth Death finished pose
	case BOTH_DEAD5:				//# Fifth Death finished pose
	case BOTH_DEAD6:				//# Sixth Death finished pose
	case BOTH_DEAD7:				//# Seventh Death finished pose
	case BOTH_DEAD9:				//# 
	case BOTH_DEAD11:			//#
	case BOTH_DEAD12:			//# 
	case BOTH_DEAD17:			//# 
	case BOTH_DEAD18:			//# 
	case BOTH_DEAD19:			//# 
	case BOTH_LYINGDEAD1:		//# Killed lying down death finished pose
	case BOTH_STUMBLEDEAD1:		//# Stumble forward death finished pose
	case BOTH_FALLDEAD1LAND:		//# Fall forward and splat death finished pose
	case BOTH_DEATH3:			//# Third Death anim
	case BOTH_DEATH4:			//# Fourth Death anim
	case BOTH_DEATH5:			//# Fifth Death anim
	case BOTH_DEATH6:			//# Sixth Death anim
	case BOTH_DEATH7:			//# Seventh Death anim
	case BOTH_DEATH9:			//# 
	case BOTH_DEATH11:			//#
	case BOTH_DEATH12:			//# 
	case BOTH_DEATH17:			//# 
	case BOTH_DEATH18:			//# 
	case BOTH_DEATH19:			//# 
	case BOTH_DEATHFORWARD1:		//# First Death in which they get thrown forward
	case BOTH_DEATHFORWARD2:		//# Second Death in which they get thrown forward
	case BOTH_DEATHBACKWARD1:	//# First Death in which they get thrown backward
	case BOTH_DEATHBACKWARD2:	//# Second Death in which they get thrown backward
	case BOTH_DEATH1IDLE:		//# Idle while close to death
	case BOTH_LYINGDEATH1:		//# Death to play when killed lying down
	case BOTH_STUMBLEDEATH1:		//# Stumble forward and fall face first death
	case BOTH_FALLDEATH1:		//# Fall forward off a high cliff and splat death - start
	case BOTH_FALLDEATH1INAIR:	//# Fall forward off a high cliff and splat death - loop
	case BOTH_FALLDEATH1LAND:	//# Fall forward off a high cliff and splat death - hit bottom
		deathAnim = -2;
		break;
	}
	if ( deathAnim == -1 )
	{
		//death anims
		switch( hitLoc )
		{
		case HL_FOOT_RT:
		case HL_FOOT_LT:
			if ( mod == MOD_SABER && !Q_irand( 0, 2 ) )
			{
				return BOTH_DEATH10;//chest: back flip
			}
			else if ( !Q_irand( 0, 2 ) )
			{
				deathAnim = BOTH_DEATH4;//back: forward
			}
			else if ( !Q_irand( 0, 1 ) )
			{
				deathAnim = BOTH_DEATH5;//same as 4
			}
			else
			{
				deathAnim = BOTH_DEATH15;//back: forward
			}
			break;
		case HL_LEG_RT:
			if ( !Q_irand( 0, 2 ) )
			{
				deathAnim = BOTH_DEATH4;//back: forward
			}
			else if ( !Q_irand( 0, 1 ) )
			{
				deathAnim = BOTH_DEATH5;//same as 4
			}
			else
			{
				deathAnim = BOTH_DEATH15;//back: forward
			}
			break;
		case HL_LEG_LT:
			if ( !Q_irand( 0, 2 ) )
			{
				deathAnim = BOTH_DEATH4;//back: forward
			}
			else if ( !Q_irand( 0, 1 ) )
			{
				deathAnim = BOTH_DEATH5;//same as 4
			}
			else
			{
				deathAnim = BOTH_DEATH15;//back: forward
			}
			break;
		case HL_BACK:
			if ( !VectorLengthSquared( self->client->ps.velocity ) )
			{
				deathAnim = BOTH_DEATH17;//head/back: croak
			}
			else
			{
				if ( !Q_irand( 0, 2 ) )
				{
					deathAnim = BOTH_DEATH4;//back: forward
				}
				else if ( !Q_irand( 0, 1 ) )
				{
					deathAnim = BOTH_DEATH5;//same as 4
				}
				else
				{
					deathAnim = BOTH_DEATH15;//back: forward
				}
			}
			break;
		case HL_CHEST_RT:
		case HL_ARM_RT:
		case HL_HAND_RT:
		case HL_BACK_RT:
			if ( damage <= max_health*0.25 )
			{
				deathAnim = BOTH_DEATH9;//chest right: snap, fall forward
			}
			else if ( damage <= max_health*0.5 )
			{
				deathAnim = BOTH_DEATH3;//chest right: back
			}
			else if ( damage <= max_health*0.75 )
			{
				deathAnim = BOTH_DEATH6;//chest right: spin
			}
			else 
			{
				//TEMP HACK: play spinny deaths less often
				if ( Q_irand( 0, 1 ) )
				{
					deathAnim = BOTH_DEATH8;//chest right: spin high
				}
				else
				{
					switch ( Q_irand( 0, 2 ) )
					{
					default:
					case 0:
						deathAnim = BOTH_DEATH9;//chest right: snap, fall forward
						break;
					case 1:
						deathAnim = BOTH_DEATH3;//chest right: back
						break;
					case 2:
						deathAnim = BOTH_DEATH6;//chest right: spin
						break;
					}
				}
			}
			break;
		case HL_CHEST_LT:
		case HL_ARM_LT:
		case HL_HAND_LT:
		case HL_BACK_LT:
			if ( damage <= max_health*0.25 )
			{
				deathAnim = BOTH_DEATH11;//chest left: snap, fall forward
			}
			else if ( damage <= max_health*0.5 )
			{
				deathAnim = BOTH_DEATH7;//chest left: back
			}
			else if ( damage <= max_health*0.75 )
			{
				deathAnim = BOTH_DEATH12;//chest left: spin
			}
			else
			{
				//TEMP HACK: play spinny deaths less often
				if ( Q_irand( 0, 1 ) )
				{
					deathAnim = BOTH_DEATH14;//chest left: spin high
				}
				else
				{
					switch ( Q_irand( 0, 2 ) )
					{
					default:
					case 0:
						deathAnim = BOTH_DEATH11;//chest left: snap, fall forward
						break;
					case 1:
						deathAnim = BOTH_DEATH7;//chest left: back
						break;
					case 2:
						deathAnim = BOTH_DEATH12;//chest left: spin
						break;
					}
				}
			}
			break;
		case HL_CHEST:
		case HL_WAIST:
			if ( damage <= max_health*0.25 || !VectorLengthSquared( self->client->ps.velocity ) )
			{
				if ( !Q_irand( 0, 1 ) )
				{
					deathAnim = BOTH_DEATH18;//gut: fall right
				}
				else
				{
					deathAnim = BOTH_DEATH19;//gut: fall left
				}
			}
			else if ( damage <= max_health*0.5 )
			{
				deathAnim = BOTH_DEATH2;//chest: backward short
			}
			else if ( damage <= max_health*0.75 )
			{
				if ( !Q_irand( 0, 1 ) )
				{
					deathAnim = BOTH_DEATH1;//chest: backward med
				}
				else
				{
					deathAnim = BOTH_DEATH16;//same as 1
				}
			}
			else
			{
				deathAnim = BOTH_DEATH10;//chest: back flip
			}
			break;
		case HL_HEAD:
			if ( damage <= max_health*0.5 )
			{
				deathAnim = BOTH_DEATH17;//head/back: croak
			}
			else
			{
				deathAnim = BOTH_DEATH13;//head: stumble, fall back
			}
			break;
		default:
			break;
		}
	}
	return deathAnim;
}

void G_CheckForDismemberment(gentity_t *ent, vec3_t point, int damage, int deathAnim);

gentity_t *G_GetJediMaster(void)
{
	int i = 0;
	gentity_t *ent;

	while (i < MAX_CLIENTS)
	{
		ent = &g_entities[i];

		if (ent && ent->inuse && ent->client && ent->client->ps.isJediMaster)
		{
			return ent;
		}

		i++;
	}

	return NULL;
}

/*
==================
player_die
==================
*/
void player_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath ) {
	gentity_t	*ent;
	int			anim;
	int			contents;
	int			killer;
	int			i;
	char		*killerName, *obit;
	qboolean	wasJediMaster = qfalse;

	if ( self->client->ps.pm_type == PM_DEAD ) {
		return;
	}

	if ( level.intermissiontime ) {
		return;
	}

	if (inflictor && inflictor->activator && !inflictor->client && !attacker->client &&
		inflictor->activator->client && inflictor->activator->inuse &&
		inflictor->s.weapon == WP_TURRET)
	{
		attacker = inflictor->activator;
	}

	if (self->client && self->client->ps.isJediMaster)
	{
		wasJediMaster = qtrue;
	}

	//if he was charging or anything else, kill the sound
	G_MuteSound(self->s.number, CHAN_WEAPON);

	BlowDetpacks(self); //blow detpacks if they're planted

	self->client->ps.fd.forceDeactivateAll = 1;

	if ((self == attacker || !attacker->client) &&
		(meansOfDeath == MOD_CRUSH || meansOfDeath == MOD_FALLING || meansOfDeath == MOD_TRIGGER_HURT || meansOfDeath == MOD_UNKNOWN) &&
		self->client->ps.otherKillerTime > level.time)
	{
		attacker = &g_entities[self->client->ps.otherKiller];
	}

	// check for an almost capture
	CheckAlmostCapture( self, attacker );

	self->client->ps.pm_type = PM_DEAD;

	if ( attacker ) {
		killer = attacker->s.number;
		if ( attacker->client ) {
			killerName = attacker->client->pers.netname;
		} else {
			killerName = "<non-client>";
		}
	} else {
		killer = ENTITYNUM_WORLD;
		killerName = "<world>";
	}

	if ( killer < 0 || killer >= MAX_CLIENTS ) {
		killer = ENTITYNUM_WORLD;
		killerName = "<world>";
	}

	if ( meansOfDeath < 0 || meansOfDeath >= sizeof( modNames ) / sizeof( modNames[0] ) ) {
		obit = "<bad obituary>";
	} else {
		obit = modNames[ meansOfDeath ];
	}

	G_LogPrintf("Kill: %i %i %i: %s killed %s by %s\n", 
		killer, self->s.number, meansOfDeath, killerName, 
		self->client->pers.netname, obit );

	G_LogWeaponKill(killer, meansOfDeath);
	G_LogWeaponDeath(self->s.number, self->s.weapon);
	if (attacker && attacker->client && attacker->inuse)
	{
		G_LogWeaponFrag(killer, self->s.number);
	}

	// broadcast the death event to everyone
	ent = G_TempEntity( self->r.currentOrigin, EV_OBITUARY );
	ent->s.eventParm = meansOfDeath;
	ent->s.otherEntityNum = self->s.number;
	ent->s.otherEntityNum2 = killer;
	ent->r.svFlags = SVF_BROADCAST;	// send to everyone
	ent->s.isJediMaster = wasJediMaster;

	self->enemy = attacker;

	self->client->ps.persistant[PERS_KILLED]++;

	if (self == attacker)
	{
		self->client->ps.fd.suicides++;
	}

	if (attacker && attacker->client) {
		attacker->client->lastkilled_client = self->s.number;

		if ( attacker == self || OnSameTeam (self, attacker ) ) {
			if (g_gametype.integer == GT_TOURNAMENT)
			{ //in duel, if you kill yourself, the person you are dueling against gets a kill for it
				int otherClNum = -1;
				if (level.sortedClients[0] == self->s.number)
				{
					otherClNum = level.sortedClients[1];
				}
				else if (level.sortedClients[1] == self->s.number)
				{
					otherClNum = level.sortedClients[0];
				}

				if (otherClNum >= 0 && otherClNum < MAX_CLIENTS &&
					g_entities[otherClNum].inuse && g_entities[otherClNum].client &&
					otherClNum != attacker->s.number)
				{
					AddScore( &g_entities[otherClNum], self->r.currentOrigin, 1 );
				}
				else
				{
					AddScore( attacker, self->r.currentOrigin, -1 );
				}
			}
			else
			{
				AddScore( attacker, self->r.currentOrigin, -1 );
			}
			if (g_gametype.integer == GT_JEDIMASTER)
			{
				if (self->client && self->client->ps.isJediMaster)
				{ //killed ourself so return the saber to the original position
				  //(to avoid people jumping off ledges and making the saber
				  //unreachable for 60 seconds)
					ThrowSaberToAttacker(self, NULL);
					self->client->ps.isJediMaster = qfalse;
				}
			}
		} else {
			if (g_gametype.integer == GT_JEDIMASTER)
			{
				if ((attacker->client && attacker->client->ps.isJediMaster) ||
					(self->client && self->client->ps.isJediMaster))
				{
					AddScore( attacker, self->r.currentOrigin, 1 );
					
					if (self->client && self->client->ps.isJediMaster)
					{
						ThrowSaberToAttacker(self, attacker);
						self->client->ps.isJediMaster = qfalse;
					}
				}
				else
				{
					gentity_t *jmEnt = G_GetJediMaster();

					if (jmEnt && jmEnt->client)
					{
						AddScore( jmEnt, self->r.currentOrigin, 1 );
					}
				}
			}
			else
			{
				AddScore( attacker, self->r.currentOrigin, 1 );
			}

			if( meansOfDeath == MOD_STUN_BATON ) {
				
				// play humiliation on player
				attacker->client->ps.persistant[PERS_GAUNTLET_FRAG_COUNT]++;

				// add the sprite over the player's head
				attacker->client->ps.eFlags &= ~(EF_AWARD_IMPRESSIVE | EF_AWARD_EXCELLENT | EF_AWARD_GAUNTLET | EF_AWARD_ASSIST | EF_AWARD_DEFEND | EF_AWARD_CAP );
				attacker->client->ps.eFlags |= EF_AWARD_GAUNTLET;
				attacker->client->rewardTime = level.time + REWARD_SPRITE_TIME;

				// also play humiliation on target
				self->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_GAUNTLETREWARD;
			}

			// check for two kills in a short amount of time
			// if this is close enough to the last kill, give a reward sound
			if ( level.time - attacker->client->lastKillTime < CARNAGE_REWARD_TIME ) {
				// play excellent on player
				attacker->client->ps.persistant[PERS_EXCELLENT_COUNT]++;

				// add the sprite over the player's head
				attacker->client->ps.eFlags &= ~(EF_AWARD_IMPRESSIVE | EF_AWARD_EXCELLENT | EF_AWARD_GAUNTLET | EF_AWARD_ASSIST | EF_AWARD_DEFEND | EF_AWARD_CAP );
				attacker->client->ps.eFlags |= EF_AWARD_EXCELLENT;
				attacker->client->rewardTime = level.time + REWARD_SPRITE_TIME;
			}
			attacker->client->lastKillTime = level.time;

		}
	} else {
		if (self->client && self->client->ps.isJediMaster)
		{ //killed ourself so return the saber to the original position
		  //(to avoid people jumping off ledges and making the saber
		  //unreachable for 60 seconds)
			ThrowSaberToAttacker(self, NULL);
			self->client->ps.isJediMaster = qfalse;
		}

		if (g_gametype.integer == GT_TOURNAMENT)
		{ //in duel, if you kill yourself, the person you are dueling against gets a kill for it
			int otherClNum = -1;
			if (level.sortedClients[0] == self->s.number)
			{
				otherClNum = level.sortedClients[1];
			}
			else if (level.sortedClients[1] == self->s.number)
			{
				otherClNum = level.sortedClients[0];
			}

			if (otherClNum >= 0 && otherClNum < MAX_CLIENTS &&
				g_entities[otherClNum].inuse && g_entities[otherClNum].client &&
				otherClNum != self->s.number)
			{
				AddScore( &g_entities[otherClNum], self->r.currentOrigin, 1 );
			}
			else
			{
				AddScore( self, self->r.currentOrigin, -1 );
			}
		}
		else
		{
			AddScore( self, self->r.currentOrigin, -1 );
		}
	}

	// Add team bonuses
	Team_FragBonuses(self, inflictor, attacker);

	// if I committed suicide, the flag does not fall, it returns.
	if (meansOfDeath == MOD_SUICIDE) {
		if ( self->client->ps.powerups[PW_NEUTRALFLAG] ) {		// only happens in One Flag CTF
			Team_ReturnFlag( TEAM_FREE );
			self->client->ps.powerups[PW_NEUTRALFLAG] = 0;
		}
		else if ( self->client->ps.powerups[PW_REDFLAG] ) {		// only happens in standard CTF
			Team_ReturnFlag( TEAM_RED );
			self->client->ps.powerups[PW_REDFLAG] = 0;
		}
		else if ( self->client->ps.powerups[PW_BLUEFLAG] ) {	// only happens in standard CTF
			Team_ReturnFlag( TEAM_BLUE );
			self->client->ps.powerups[PW_BLUEFLAG] = 0;
		}
	}

	// if client is in a nodrop area, don't drop anything (but return CTF flags!)
	contents = trap_PointContents( self->r.currentOrigin, -1 );
	if ( !( contents & CONTENTS_NODROP ) && !self->client->ps.fallingToDeath) {
		TossClientItems( self );
	}
	else {
		if ( self->client->ps.powerups[PW_NEUTRALFLAG] ) {		// only happens in One Flag CTF
			Team_ReturnFlag( TEAM_FREE );
		}
		else if ( self->client->ps.powerups[PW_REDFLAG] ) {		// only happens in standard CTF
			Team_ReturnFlag( TEAM_RED );
		}
		else if ( self->client->ps.powerups[PW_BLUEFLAG] ) {	// only happens in standard CTF
			Team_ReturnFlag( TEAM_BLUE );
		}
	}

	Cmd_Score_f( self );		// show scores
	// send updated scores to any clients that are following this one,
	// or they would get stale scoreboards
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		gclient_t	*client;

		client = &level.clients[i];
		if ( client->pers.connected != CON_CONNECTED ) {
			continue;
		}
		if ( client->sess.sessionTeam != TEAM_SPECTATOR ) {
			continue;
		}
		if ( client->sess.spectatorClient == self->s.number ) {
			Cmd_Score_f( g_entities + i );
		}
	}

	self->takedamage = qtrue;	// can still be gibbed

	self->s.weapon = WP_NONE;
	self->s.powerups = 0;
	self->r.contents = CONTENTS_CORPSE;
	self->client->ps.zoomMode = 0;	// Turn off zooming when we die

	self->s.angles[0] = 0;
	self->s.angles[2] = 0;
	LookAtKiller (self, inflictor, attacker);

	VectorCopy( self->s.angles, self->client->ps.viewangles );

	self->s.loopSound = 0;

	self->r.maxs[2] = -8;

	// don't allow respawn until the death anim is done
	// g_forcerespawn may force spawning at some later time
	self->client->respawnTime = level.time + 1700;

	// remove powerups
	memset( self->client->ps.powerups, 0, sizeof(self->client->ps.powerups) );

	// NOTENOTE No gib deaths right now, this is star wars.
	/*
	// never gib in a nodrop
	if ( (self->health <= GIB_HEALTH && !(contents & CONTENTS_NODROP) && g_blood.integer) || meansOfDeath == MOD_SUICIDE) 
	{
		// gib death
		GibEntity( self, killer );
	} 
	else 
	*/
	{
		// normal death
		
		static int i;

		switch ( i ) {
		case 0:
			anim = BOTH_DEATH1;
			break;
		case 1:
			anim = BOTH_DEATH2;
			break;
		case 2:
		default:
			anim = BOTH_DEATH3;
			break;
		}
		
		anim = G_PickDeathAnim(self, self->pos1, damage, meansOfDeath, HL_NONE);

		if (anim < 1)
		{
			anim = BOTH_DEATH1;
		}

		if (meansOfDeath == MOD_SABER)
		{
			G_CheckForDismemberment(self, self->pos1, damage, anim);
		}

		// for the no-blood option, we need to prevent the health
		// from going to gib level
		if ( self->health <= GIB_HEALTH ) {
			self->health = GIB_HEALTH+1;
		}

		self->client->respawnTime = level.time + 1000;//((self->client->animations[anim].numFrames*40)/(50.0f / self->client->animations[anim].frameLerp))+300;

		self->client->ps.legsAnim = 
			( ( self->client->ps.legsAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;
		self->client->ps.torsoAnim = 
			( ( self->client->ps.torsoAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;
//		self->client->ps.pm_flags |= PMF_UPDATE_ANIM;		// Make sure the pmove sets up the GHOUL2 anims.

		//rww - do this on respawn, not death
		//CopyToBodyQue (self);

		//G_AddEvent( self, EV_DEATH1 + i, killer );
		if (wasJediMaster)
		{
			G_AddEvent( self, EV_DEATH1 + i, 1 );
		}
		else
		{
			G_AddEvent( self, EV_DEATH1 + i, 0 );
		}

		// the body can still be gibbed
		self->die = body_die;

		//It won't gib, it will disintegrate (because this is Star Wars).
		self->takedamage = qtrue;

		// globally cycle through the different death animations
		i = ( i + 1 ) % 3;
	}

	trap_LinkEntity (self);

}


/*
================
CheckArmor
================
*/
int CheckArmor (gentity_t *ent, int damage, int dflags)
{
	gclient_t	*client;
	int			save;
	int			count;

	if (!damage)
		return 0;

	client = ent->client;

	if (!client)
		return 0;

	if (dflags & DAMAGE_NO_ARMOR)
		return 0;

	// armor
	count = client->ps.stats[STAT_ARMOR];

	if (dflags & DAMAGE_HALF_ABSORB)
	{	// Half the damage gets absorbed by the shields, rather than 100%
		save = ceil( damage * ARMOR_PROTECTION );
	}
	else
	{	// All the damage gets absorbed by the shields.
		save = damage;
	}

	// save is the most damage that the armor is elibigle to protect, of course, but it's limited by the total armor.
	if (save >= count)
		save = count;

	if (!save)
		return 0;

	if (dflags & DAMAGE_HALF_ARMOR_REDUCTION)		// Armor isn't whittled so easily by sniper shots.
	{
		client->ps.stats[STAT_ARMOR] -= (int)(save*ARMOR_REDUCTION_FACTOR);
	}
	else
	{
		client->ps.stats[STAT_ARMOR] -= save;
	}

	return save;
}


void G_ApplyKnockback( gentity_t *targ, vec3_t newDir, float knockback )
{
	vec3_t	kvel;
	float	mass;

	if (targ && targ->client && targ->client->ps.usingATST)
	{
		return;
	}

	if ( targ->physicsBounce > 0 )	//overide the mass
		mass = targ->physicsBounce;
	else
		mass = 200;

	if ( g_gravity.value > 0 )
	{
		VectorScale( newDir, g_knockback.value * (float)knockback / mass * 0.8, kvel );
		kvel[2] = newDir[2] * g_knockback.value * (float)knockback / mass * 1.5;
	}
	else
	{
		VectorScale( newDir, g_knockback.value * (float)knockback / mass, kvel );
	}

	if ( targ->client )
	{
		VectorAdd( targ->client->ps.velocity, kvel, targ->client->ps.velocity );
	}
	else if ( targ->s.pos.trType != TR_STATIONARY && targ->s.pos.trType != TR_LINEAR_STOP )
	{
		VectorAdd( targ->s.pos.trDelta, kvel, targ->s.pos.trDelta );
		VectorCopy( targ->r.currentOrigin, targ->s.pos.trBase );
		targ->s.pos.trTime = level.time;
	}

	// set the timer so that the other client can't cancel
	// out the movement immediately
	if ( targ->client && !targ->client->ps.pm_time ) 
	{
		int		t;

		t = knockback * 2;
		if ( t < 50 ) {
			t = 50;
		}
		if ( t > 200 ) {
			t = 200;
		}
		targ->client->ps.pm_time = t;
		targ->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
	}
}

/*
================
RaySphereIntersections
================
*/
int RaySphereIntersections( vec3_t origin, float radius, vec3_t point, vec3_t dir, vec3_t intersections[2] ) {
	float b, c, d, t;

	//	| origin - (point + t * dir) | = radius
	//	a = dir[0]^2 + dir[1]^2 + dir[2]^2;
	//	b = 2 * (dir[0] * (point[0] - origin[0]) + dir[1] * (point[1] - origin[1]) + dir[2] * (point[2] - origin[2]));
	//	c = (point[0] - origin[0])^2 + (point[1] - origin[1])^2 + (point[2] - origin[2])^2 - radius^2;

	// normalize dir so a = 1
	VectorNormalize(dir);
	b = 2 * (dir[0] * (point[0] - origin[0]) + dir[1] * (point[1] - origin[1]) + dir[2] * (point[2] - origin[2]));
	c = (point[0] - origin[0]) * (point[0] - origin[0]) +
		(point[1] - origin[1]) * (point[1] - origin[1]) +
		(point[2] - origin[2]) * (point[2] - origin[2]) -
		radius * radius;

	d = b * b - 4 * c;
	if (d > 0) {
		t = (- b + sqrt(d)) / 2;
		VectorMA(point, t, dir, intersections[0]);
		t = (- b - sqrt(d)) / 2;
		VectorMA(point, t, dir, intersections[1]);
		return 2;
	}
	else if (d == 0) {
		t = (- b ) / 2;
		VectorMA(point, t, dir, intersections[0]);
		return 1;
	}
	return 0;
}

void LimbTouch( gentity_t *self, gentity_t *other, trace_t *trace )
{
}

void LimbThink( gentity_t *ent )
{
	if (ent->speed < level.time)
	{
		ent->think = G_FreeEntity;
		ent->nextthink = level.time;
		return;
	}

	if (ent->s.pos.trType != TR_GRAVITY)
	{
		int addamt = (ent->speed - level.time);

		if (addamt > 5000)
		{
			addamt = 5000;
		}
		if (addamt < 0)
		{
			addamt = 0;
		}

		VectorClear(ent->s.pos.trDelta);
		ent->think = G_FreeEntity;
		ent->nextthink = level.time + addamt;
		return;
	}

	G_RunMissile(ent);

	G_RunObject(ent);
}

void G_G2PlayerAngles( gentity_t *ent, vec3_t legs[3], vec3_t legsAngles);

void G_GetDismemberBolt(gentity_t *self, vec3_t boltPoint, int limbType)
{
	int useBolt = self->bolt_Head;
	vec3_t properOrigin, properAngles, addVel;
	vec3_t legAxis[3];
	mdxaBone_t	boltMatrix;
	float fVSpeed = 0;

	switch (limbType)
	{
	case G2_MODELPART_HEAD:
		useBolt = self->bolt_Head;
		break;
	case G2_MODELPART_WAIST:
		useBolt = self->bolt_Waist;
		break;
	case G2_MODELPART_LARM:
		useBolt = self->bolt_LArm;
		break;
	case G2_MODELPART_RARM:
		useBolt = self->bolt_RArm;
		break;
	case G2_MODELPART_LLEG:
		useBolt = self->bolt_LLeg;
		break;
	case G2_MODELPART_RLEG:
		useBolt = self->bolt_RLeg;
		break;
	default:
		useBolt = self->bolt_Head;
		break;
	}

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

	trap_G2API_GetBoltMatrix(self->client->ghoul2, 0, useBolt, &boltMatrix, properAngles, properOrigin, level.time, NULL, vec3_origin);

	boltPoint[0] = boltMatrix.matrix[0][3];
	boltPoint[1] = boltMatrix.matrix[1][3];
	boltPoint[2] = boltMatrix.matrix[2][3];
}

void G_Dismember( gentity_t *ent, vec3_t point, int limbType, float limbRollBase, float limbPitchBase, int deathAnim )
{
	vec3_t	dir, newPoint, vel;
	gentity_t *limb;

	VectorCopy( point, newPoint );
	limb = G_Spawn();
	limb->classname = "playerlimb";
	G_SetOrigin( limb, newPoint );
	VectorCopy( newPoint, limb->s.pos.trBase );
	limb->think = LimbThink;
	limb->touch = LimbTouch;
	limb->speed = level.time + Q_irand(4000, 8000);
	limb->nextthink = level.time + FRAMETIME;

	//need size, contents, clipmask
	limb->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	limb->clipmask = MASK_SOLID;
	limb->r.contents = CONTENTS_TRIGGER;
	VectorSet( limb->r.mins, -3.0f, -3.0f, -3.0f );
	VectorSet( limb->r.maxs, 3.0f, 3.0f, 3.0f );
//	VectorClear(limb->r.mins);
//	VectorClear(limb->r.maxs);

	//move it
	limb->s.eType = ET_GENERAL;
	limb->s.weapon = G2_MODEL_PART;

	if (limbType == G2_MODELPART_HEAD)
	{
		limb->bounceCount = 2;
	}
	else
	{
		limb->bounceCount = 1;
	}
	limb->s.pos.trType = TR_GRAVITY;
	limb->s.pos.trTime = level.time;								// move a bit on the very first frame
	VectorSubtract( point, ent->r.currentOrigin, dir );
	VectorNormalize( dir );
	VectorCopy(ent->client->ps.velocity, vel);
	VectorMA( vel, 100, dir, limb->s.pos.trDelta );

	//make it bounce some
	limb->s.eFlags |= EF_BOUNCE_HALF;
	//no trDuration?
	//spin it
	VectorClear( limb->s.apos.trBase );
	limb->s.apos.trBase[0] = limbPitchBase;
	limb->s.apos.trBase[1] = ent->client->ps.viewangles[1];
	limb->s.apos.trBase[2] = limbRollBase;

	VectorClear( limb->s.apos.trDelta );

	limb->s.apos.trDelta[0] = Q_irand( -300, 300 );
	limb->s.apos.trDelta[2] = Q_irand( -300, 300 );
	limb->s.apos.trDelta[1] = Q_irand( -300, 300 );

	if (limbType == G2_MODELPART_WAIST)
	{
		limb->s.apos.trDelta[0] = Q_irand( -60, 60 );
		limb->s.apos.trDelta[2] = Q_irand( -60, 60 );
		limb->s.apos.trDelta[1] = Q_irand( -60, 60 );
	}

	limb->s.apos.trTime = level.time;
	limb->s.apos.trType = TR_LINEAR;

	limb->s.modelGhoul2 = limbType;
	limb->s.g2radius = 200;
	limb->s.modelindex = ent->s.number;
	limb->s.modelindex2 = deathAnim;

	trap_LinkEntity( limb );
}

/*
void DismembermentTest(gentity_t *self)
{
	int sect = G2_MODELPART_HEAD;
	vec3_t boltPoint;
	char *sectc = ConcatArgs( 1 );
	
	if (sectc && sectc[0])
	{
		sect = atoi(sectc)+G2_MODELPART_HEAD;
	}

	G_GetDismemberBolt(self, boltPoint, sect);
	G_Dismember( self, boltPoint, sect, 90, 0 );
}
*/

int G_GetHitQuad( gentity_t *self, vec3_t hitloc )
{
	vec3_t diff, fwdangles={0,0,0}, right;
	vec3_t clEye;
	float rightdot;
	float zdiff;
	int hitLoc = -1;

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
	
	if ( zdiff > 0 )
	{
		if ( rightdot > 0.3 )
		{
			hitLoc = G2_MODELPART_RARM;
		}
		else if ( rightdot < -0.3 )
		{
			hitLoc = G2_MODELPART_LARM;
		}
		else
		{
			hitLoc = G2_MODELPART_HEAD;
		}
	}
	else if ( zdiff > -20 )
	{
		if ( rightdot > 0.1 )
		{
			hitLoc = G2_MODELPART_RARM;
		}
		else if ( rightdot < -0.1 )
		{
			hitLoc = G2_MODELPART_LARM;
		}
		else
		{
			hitLoc = G2_MODELPART_HEAD;
		}
	}
	else
	{
		if ( rightdot >= 0 )
		{
			hitLoc = G2_MODELPART_RLEG;
		}
		else
		{
			hitLoc = G2_MODELPART_LLEG;
		}
	}

	return hitLoc;
}

void G_CheckForDismemberment(gentity_t *ent, vec3_t point, int damage, int deathAnim)
{
	int hitLoc, hitLocUse = -1;
	vec3_t boltPoint;
	int dismember = g_dismember.integer;

	if (!dismember)
	{
		return;
	}

	if (Q_irand(0, 100) > dismember)
	{
		return;
	}

	if (damage < 20)
	{
		return;
	}

	hitLoc = G_GetHitLocation( ent, point );

	switch(hitLoc)
	{
	case HL_FOOT_RT:
	case HL_LEG_RT:
		hitLocUse = G2_MODELPART_RLEG;
		break;
	case HL_FOOT_LT:
	case HL_LEG_LT:
		hitLocUse = G2_MODELPART_LLEG;
		
	case HL_WAIST:
		hitLocUse = G2_MODELPART_WAIST;
		break;
		/*
	case HL_BACK_RT:
	case HL_BACK_LT:
	case HL_BACK:
	case HL_CHEST_RT:
	case HL_CHEST_LT:
	case HL_CHEST:
		break;
		*/
	case HL_ARM_RT:
	case HL_HAND_RT:
		hitLocUse = G2_MODELPART_RARM;
		break;
	case HL_ARM_LT:
	case HL_HAND_LT:
		hitLocUse = G2_MODELPART_LARM;
		break;
	case HL_HEAD:
		hitLocUse = G2_MODELPART_HEAD;
	default:
		hitLocUse = G_GetHitQuad(ent, point);
		break;
	}

	if (hitLocUse == -1)
	{
		return;
	}

	G_GetDismemberBolt(ent, boltPoint, hitLocUse);
	G_Dismember(ent, boltPoint, hitLocUse, 90, 0, deathAnim);
}

qboolean G_ThereIsAMaster(void)
{
	int i = 0;
	gentity_t *ent;

	while (i < MAX_CLIENTS)
	{
		ent = &g_entities[i];

		if (ent && ent->client && ent->client->ps.isJediMaster)
		{
			return qtrue;
		}

		i++;
	}

	return qfalse;
}

/*
============
T_Damage

targ		entity that is being damaged
inflictor	entity that is causing the damage
attacker	entity that caused the inflictor to damage targ
	example: targ=monster, inflictor=rocket, attacker=player

dir			direction of the attack for knockback
point		point at which the damage is being inflicted, used for headshots
damage		amount of damage being inflicted
knockback	force to be applied against targ as a result of the damage

inflictor, attacker, dir, and point can be NULL for environmental effects

dflags		these flags are used to control how T_Damage works
	DAMAGE_RADIUS			damage was indirect (from a nearby explosion)
	DAMAGE_NO_ARMOR			armor does not protect from this damage
	DAMAGE_NO_KNOCKBACK		do not affect velocity, just view angles
	DAMAGE_NO_PROTECTION	kills godmode, armor, everything
	DAMAGE_HALF_ABSORB		half shields, half health
	DAMAGE_HALF_ARMOR_REDUCTION		Any damage that shields incur is halved
============
*/

void G_Damage( gentity_t *targ, gentity_t *inflictor, gentity_t *attacker,
			   vec3_t dir, vec3_t point, int damage, int dflags, int mod ) {
	gclient_t	*client;
	int			take;
	int			save;
	int			asave;
	int			knockback;
	int			max;
	int			subamt = 0;
	float		famt = 0;
	float		hamt = 0;
	float		shieldAbsorbed = 0;

	if (targ && targ->damageRedirect)
	{
		G_Damage(&g_entities[targ->damageRedirectTo], inflictor, attacker, dir, point, damage, dflags, mod);
		return;
	}

	if (!targ->takedamage) {
		return;
	}

	if (targ && targ->client && targ->client->ps.duelInProgress)
	{
		if (attacker && attacker->client && attacker->s.number != targ->client->ps.duelIndex)
		{
			return;
		}
		else if (attacker && attacker->client && mod != MOD_SABER)
		{
			return;
		}
	}
	if (attacker && attacker->client && attacker->client->ps.duelInProgress)
	{
		if (targ && targ->client && targ->s.number != attacker->client->ps.duelIndex)
		{
			return;
		}
		else if (targ && targ->client && mod != MOD_SABER)
		{
			return;
		}
	}

	if (targ && targ->client && (targ->client->ps.fd.forcePowersActive & (1 << FP_RAGE)))
	{
		damage *= 0.5;
	}

	// the intermission has allready been qualified for, so don't
	// allow any extra scoring
	if ( level.intermissionQueued ) {
		return;
	}
	if ( !inflictor ) {
		inflictor = &g_entities[ENTITYNUM_WORLD];
	}
	if ( !attacker ) {
		attacker = &g_entities[ENTITYNUM_WORLD];
	}

	// shootable doors / buttons don't actually have any health

	//if boltpoint4 == 1 then it's glass or a breakable and those do have health
	if ( targ->s.eType == ET_MOVER && targ->boltpoint4 != 1 ) {
		if ( targ->use && targ->moverState == MOVER_POS1 ) {
			targ->use( targ, inflictor, attacker );
		}
		return;
	}
	// reduce damage by the attacker's handicap value
	// unless they are rocket jumping
	if ( attacker->client && attacker != targ ) {
		max = attacker->client->ps.stats[STAT_MAX_HEALTH];
		damage = damage * max / 100;
	}

	client = targ->client;

	if ( client ) {
		if ( client->noclip ) {
			return;
		}
	}

	if ( !dir ) {
		dflags |= DAMAGE_NO_KNOCKBACK;
	} else {
		VectorNormalize(dir);
	}

	knockback = damage;
	if ( knockback > 200 ) {
		knockback = 200;
	}
	if ( targ->flags & FL_NO_KNOCKBACK ) {
		knockback = 0;
	}
	if ( dflags & DAMAGE_NO_KNOCKBACK ) {
		knockback = 0;
	}

	if (targ && targ->client && targ->client->ps.usingATST)
	{
		knockback = 0;
	}

	// figure momentum add, even if the damage won't be taken
	if ( knockback && targ->client ) {
		vec3_t	kvel;
		float	mass;

		mass = 200;

		VectorScale (dir, g_knockback.value * (float)knockback / mass, kvel);
		VectorAdd (targ->client->ps.velocity, kvel, targ->client->ps.velocity);

		if (attacker && attacker->client && attacker != targ)
		{
			targ->client->ps.otherKiller = attacker->s.number;
			targ->client->ps.otherKillerTime = level.time + 5000;
			targ->client->ps.otherKillerDebounceTime = level.time + 100;
		}
		// set the timer so that the other client can't cancel
		// out the movement immediately
		if ( !targ->client->ps.pm_time ) {
			int		t;

			t = knockback * 2;
			if ( t < 50 ) {
				t = 50;
			}
			if ( t > 200 ) {
				t = 200;
			}
			targ->client->ps.pm_time = t;
			targ->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
		}
	}

	// check for completely getting out of the damage
	if ( !(dflags & DAMAGE_NO_PROTECTION) ) {

		// if TF_NO_FRIENDLY_FIRE is set, don't do damage to the target
		// if the attacker was on the same team
		if ( targ != attacker && OnSameTeam (targ, attacker)  ) {
			if ( !g_friendlyFire.integer ) {
				return;
			}
		}

		if (g_gametype.integer == GT_JEDIMASTER && !g_friendlyFire.integer &&
			targ && targ->client && attacker && attacker->client &&
			targ != attacker && !targ->client->ps.isJediMaster && !attacker->client->ps.isJediMaster &&
			G_ThereIsAMaster())
		{
			return;
		}

		if (targ->client && targ->s.shouldtarget && targ->s.teamowner &&
			attacker && attacker->inuse && attacker->client && targ->s.owner >= 0 && targ->s.owner < MAX_CLIENTS)
		{
			gentity_t *targown = &g_entities[targ->s.owner];

			if (targown && targown->inuse && targown->client && OnSameTeam(targown, attacker))
			{
				if (!g_friendlyFire.integer)
				{
					return;
				}
			}
		}

		// check for godmode
		if ( targ->flags & FL_GODMODE ) {
			return;
		}

		if (targ && targ->client && (targ->client->ps.eFlags & EF_INVULNERABLE) &&
			attacker && attacker->client && targ != attacker)
		{
			if (targ->client->invulnerableTimer <= level.time)
			{
				targ->client->ps.eFlags &= ~EF_INVULNERABLE;
			}
			else
			{
				return;
			}
		}
	}

	if (attacker && attacker->client)
	{
		if (targ->teamnodmg &&
			targ->teamnodmg == attacker->client->sess.sessionTeam &&
			!g_ff_objectives.integer)
		{
			return;
		}
	}

	// battlesuit protects from all radius damage (but takes knockback)
	// and protects 50% against all damage
	if ( client && client->ps.powerups[PW_BATTLESUIT] ) {
		G_AddEvent( targ, EV_POWERUP_BATTLESUIT, 0 );
		if ( ( dflags & DAMAGE_RADIUS ) || ( mod == MOD_FALLING ) ) {
			return;
		}
		damage *= 0.5;
	}

	// add to the attacker's hit counter (if the target isn't a general entity like a prox mine)
	if ( attacker->client && targ != attacker && targ->health > 0
			&& targ->s.eType != ET_MISSILE
			&& targ->s.eType != ET_GENERAL
			&& client) {
		if ( OnSameTeam( targ, attacker ) ) {
			attacker->client->ps.persistant[PERS_HITS]--;
		} else {
			attacker->client->ps.persistant[PERS_HITS]++;
		}
		attacker->client->ps.persistant[PERS_ATTACKEE_ARMOR] = (targ->health<<8)|(client->ps.stats[STAT_ARMOR]);
	}

	// always give half damage if hurting self
	// calculated after knockback, so rocket jumping works
	if ( targ == attacker) {
		damage *= 0.5;
	}

	if ( damage < 1 ) {
		damage = 1;
	}
	take = damage;
	save = 0;

	// save some from armor
	asave = CheckArmor (targ, take, dflags);

	if (asave)
	{
		shieldAbsorbed = asave;
	}

	take -= asave;

	if (mod == MOD_DEMP2 || mod == MOD_DEMP2_ALT)
	{ //demp2 does full damage to shields, but only 1/3 normal damage to health
		if (take > 0)
		{
			take /= 3;
			if (take < 1)
			{
				take = 1;
			}
		}
	}

	if ( g_debugDamage.integer ) {
		G_Printf( "%i: client:%i health:%i damage:%i armor:%i\n", level.time, targ->s.number,
			targ->health, take, asave );
	}

	// add to the damage inflicted on a player this frame
	// the total will be turned into screen blends and view angle kicks
	// at the end of the frame
	if ( client ) {
		if ( attacker ) {
			client->ps.persistant[PERS_ATTACKER] = attacker->s.number;
		} else {
			client->ps.persistant[PERS_ATTACKER] = ENTITYNUM_WORLD;
		}
		client->damage_armor += asave;
		client->damage_blood += take;
		client->damage_knockback += knockback;
		if ( dir ) {
			VectorCopy ( dir, client->damage_from );
			client->damage_fromWorld = qfalse;
		} else {
			VectorCopy ( targ->r.currentOrigin, client->damage_from );
			client->damage_fromWorld = qtrue;
		}

		if (attacker && attacker->client)
		{
			BotDamageNotification(client, attacker);
		}
		else if (inflictor && inflictor->client)
		{
			BotDamageNotification(client, inflictor);
		}
	}

	// See if it's the player hurting the emeny flag carrier
	if( g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY) {
		Team_CheckHurtCarrier(targ, attacker);
	}

	if (targ->client) {
		// set the last client who damaged the target
		targ->client->lasthurt_client = attacker->s.number;
		targ->client->lasthurt_mod = mod;
	}

	if (take && targ->client && (targ->client->ps.fd.forcePowersActive & (1 << FP_PROTECT)))
	{
		if (targ->client->ps.fd.forcePower)
		{
			int maxtake = take;

			//G_Sound(targ, CHAN_AUTO, protectHitSound);
			G_PreDefSound(targ->client->ps.origin, PDSOUND_PROTECTHIT);

			if (targ->client->ps.fd.forcePowerLevel[FP_PROTECT] == FORCE_LEVEL_1)
			{
				famt = 1;
				hamt = 0.40;

				if (maxtake > 100)
				{
					maxtake = 100;
				}
			}
			else if (targ->client->ps.fd.forcePowerLevel[FP_PROTECT] == FORCE_LEVEL_2)
			{
				famt = 0.5;
				hamt = 0.60;

				if (maxtake > 200)
				{
					maxtake = 200;
				}
			}
			else if (targ->client->ps.fd.forcePowerLevel[FP_PROTECT] == FORCE_LEVEL_3)
			{
				famt = 0.25;
				hamt = 0.80;

				if (maxtake > 400)
				{
					maxtake = 400;
				}
			}

			if (!targ->client->ps.powerups[PW_FORCE_BOON])
			{
				targ->client->ps.fd.forcePower -= maxtake*famt;
			}
			else
			{
				targ->client->ps.fd.forcePower -= (maxtake*famt)/2;
			}
			subamt = (maxtake*hamt)+(take-maxtake);
			if (targ->client->ps.fd.forcePower < 0)
			{
				subamt += targ->client->ps.fd.forcePower;
				targ->client->ps.fd.forcePower = 0;
			}
			if (subamt)
			{
				take -= subamt;

				if (take < 0)
				{
					take = 0;
				}
			}
		}
	}

	if (shieldAbsorbed)
	{
		gentity_t	*evEnt;

		// Send off an event to show a shield shell on the player, pointing in the right direction.
		evEnt = G_TempEntity(vec3_origin, EV_SHIELD_HIT);
		evEnt->s.otherEntityNum = targ->s.number;
		evEnt->s.eventParm = DirToByte(dir);
		evEnt->s.time2=shieldAbsorbed;
/*
		shieldAbsorbed *= 20;

		if (shieldAbsorbed > 1500)
		{
			shieldAbsorbed = 1500;
		}
		if (shieldAbsorbed < 200)
		{
			shieldAbsorbed = 200;
		}

		if (targ->client->ps.powerups[PW_SHIELDHIT] < (level.time + shieldAbsorbed))
		{
			targ->client->ps.powerups[PW_SHIELDHIT] = level.time + shieldAbsorbed;
		}
		//flicker for as many ms as damage was absorbed (*20)
		//therefore 10 damage causes 1/5 of a seond of flickering, whereas
		//a full 100 causes 2 seconds (but is reduced to 1.5 seconds due to the max)

*/
	}

	// do the damage
	if (take) {
		if (targ->client && (targ->client->ps.fd.forcePowersActive & (1 << FP_RAGE)) && (inflictor->client || attacker->client))
		{
			take /= (targ->client->ps.fd.forcePowerLevel[FP_RAGE]+1);
		}
		targ->health = targ->health - take;
		if ( targ->client ) {
			targ->client->ps.stats[STAT_HEALTH] = targ->health;
		}

		if (targ->client && (targ->client->ps.fd.forcePowersActive & (1 << FP_RAGE)) && (inflictor->client || attacker->client))
		{
			if (targ->health <= 0)
			{
				targ->health = 1;
			}
			if (targ->client->ps.stats[STAT_HEALTH] <= 0)
			{
				targ->client->ps.stats[STAT_HEALTH] = 1;
			}
		}
	
		if ( targ->health <= 0 ) {
			if ( client )
			{
				targ->flags |= FL_NO_KNOCKBACK;

				if (point)
				{
					VectorCopy( point, targ->pos1 );
				}
				else
				{
					VectorCopy(targ->client->ps.origin, targ->pos1);
				}
			}

			if (targ->health < -999)
				targ->health = -999;

			// If we are a breaking glass brush, store the damage point so we can do cool things with it.
			if ( targ->r.svFlags & SVF_GLASS_BRUSH )
			{
				VectorCopy( point, targ->pos1 );
				VectorCopy( dir, targ->pos2 );
			}

			targ->enemy = attacker;
			targ->die (targ, inflictor, attacker, take, mod);
			return;
		} else if ( targ->pain ) {
			targ->pain (targ, attacker, take);
		}

		G_LogWeaponDamage(attacker->s.number, mod, take);
	}

}


/*
============
CanDamage

Returns qtrue if the inflictor can directly damage the target.  Used for
explosions and melee attacks.
============
*/
qboolean CanDamage (gentity_t *targ, vec3_t origin) {
	vec3_t	dest;
	trace_t	tr;
	vec3_t	midpoint;

	// use the midpoint of the bounds instead of the origin, because
	// bmodels may have their origin is 0,0,0
	VectorAdd (targ->r.absmin, targ->r.absmax, midpoint);
	VectorScale (midpoint, 0.5, midpoint);

	VectorCopy (midpoint, dest);
	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0 || tr.entityNum == targ->s.number)
		return qtrue;

	// this should probably check in the plane of projection, 
	// rather than in world coordinate, and also include Z
	VectorCopy (midpoint, dest);
	dest[0] += 15.0;
	dest[1] += 15.0;
	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	VectorCopy (midpoint, dest);
	dest[0] += 15.0;
	dest[1] -= 15.0;
	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	VectorCopy (midpoint, dest);
	dest[0] -= 15.0;
	dest[1] += 15.0;
	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	VectorCopy (midpoint, dest);
	dest[0] -= 15.0;
	dest[1] -= 15.0;
	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;


	return qfalse;
}


/*
============
G_RadiusDamage
============
*/
qboolean G_RadiusDamage ( vec3_t origin, gentity_t *attacker, float damage, float radius,
					 gentity_t *ignore, int mod) {
	float		points, dist;
	gentity_t	*ent;
	int			entityList[MAX_GENTITIES];
	int			numListedEntities;
	vec3_t		mins, maxs;
	vec3_t		v;
	vec3_t		dir;
	int			i, e;
	qboolean	hitClient = qfalse;

	if ( radius < 1 ) {
		radius = 1;
	}

	for ( i = 0 ; i < 3 ; i++ ) {
		mins[i] = origin[i] - radius;
		maxs[i] = origin[i] + radius;
	}

	numListedEntities = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );

	for ( e = 0 ; e < numListedEntities ; e++ ) {
		ent = &g_entities[entityList[ e ]];

		if (ent == ignore)
			continue;
		if (!ent->takedamage)
			continue;

		// find the distance from the edge of the bounding box
		for ( i = 0 ; i < 3 ; i++ ) {
			if ( origin[i] < ent->r.absmin[i] ) {
				v[i] = ent->r.absmin[i] - origin[i];
			} else if ( origin[i] > ent->r.absmax[i] ) {
				v[i] = origin[i] - ent->r.absmax[i];
			} else {
				v[i] = 0;
			}
		}

		dist = VectorLength( v );
		if ( dist >= radius ) {
			continue;
		}

		points = damage * ( 1.0 - dist / radius );

		if( CanDamage (ent, origin) ) {
			if( LogAccuracyHit( ent, attacker ) ) {
				hitClient = qtrue;
			}
			VectorSubtract (ent->r.currentOrigin, origin, dir);
			// push the center of mass higher than the origin so players
			// get knocked into the air more
			dir[2] += 24;
			G_Damage (ent, NULL, attacker, dir, origin, (int)points, DAMAGE_RADIUS, mod);
		}
	}

	return hitClient;
}
