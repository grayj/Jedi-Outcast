#include "g_local.h"
#include "w_saber.h"
#include "ai_main.h"
#include "..\ghoul2\g2.h"

#define METROID_JUMP 1

extern bot_state_t *botstates[MAX_CLIENTS];

int speedLoopSound = 0;
 
int rageLoopSound = 0;

int protectLoopSound = 0;

int absorbLoopSound = 0;

int seeLoopSound = 0;

int	ysalamiriLoopSound = 0;

#define FORCE_VELOCITY_DAMAGE 0

void G_PreDefSound(vec3_t org, int pdSound)
{
	gentity_t	*te;

	te = G_TempEntity( org, EV_PREDEFSOUND );
	te->s.eventParm = pdSound;
	VectorCopy(org, te->s.origin);
}

qboolean InFront( vec3_t spot, vec3_t from, vec3_t fromAngles, float threshHold )
{
	vec3_t	dir, forward, angles;
	float	dot;

	VectorSubtract( spot, from, dir );
	dir[2] = 0;
	VectorNormalize( dir );

	VectorCopy( fromAngles, angles );
	angles[0] = 0;
	AngleVectors( angles, forward, NULL, NULL );

	dot = DotProduct( dir, forward );

	return (dot > threshHold);
}

int forcePowerMinRank[NUM_FORCE_POWER_LEVELS][NUM_FORCE_POWERS] = //0 == neutral
{
	{
		999,//FP_HEAL,//instant
		999,//FP_LEVITATION,//hold/duration
		999,//FP_SPEED,//duration
		999,//FP_PUSH,//hold/duration
		999,//FP_PULL,//hold/duration
		999,//FP_TELEPATHY,//instant
		999,//FP_GRIP,//hold/duration
		999,//FP_LIGHTNING,//hold/duration
		999,//FP_RAGE,//duration
		999,//FP_PROTECT,//duration
		999,//FP_ABSORB,//duration
		999,//FP_TEAM_HEAL,//instant
		999,//FP_TEAM_FORCE,//instant
		999,//FP_DRAIN,//hold/duration
		999,//FP_SEE,//duration
		999,//FP_SABERATTACK,
		999,//FP_SABERDEFEND,
		999//FP_SABERTHROW,
		//NUM_FORCE_POWERS
	},
	{
		10,//FP_HEAL,//instant
		0,//FP_LEVITATION,//hold/duration
		0,//FP_SPEED,//duration
		0,//FP_PUSH,//hold/duration
		0,//FP_PULL,//hold/duration
		10,//FP_TELEPATHY,//instant
		15,//FP_GRIP,//hold/duration
		10,//FP_LIGHTNING,//hold/duration
		15,//FP_RAGE,//duration
		15,//FP_PROTECT,//duration
		15,//FP_ABSORB,//duration
		10,//FP_TEAM_HEAL,//instant
		10,//FP_TEAM_FORCE,//instant
		10,//FP_DRAIN,//hold/duration
		5,//FP_SEE,//duration
		0,//FP_SABERATTACK,
		0,//FP_SABERDEFEND,
		0//FP_SABERTHROW,
		//NUM_FORCE_POWERS
	},
	{
		10,//FP_HEAL,//instant
		0,//FP_LEVITATION,//hold/duration
		0,//FP_SPEED,//duration
		0,//FP_PUSH,//hold/duration
		0,//FP_PULL,//hold/duration
		10,//FP_TELEPATHY,//instant
		15,//FP_GRIP,//hold/duration
		10,//FP_LIGHTNING,//hold/duration
		15,//FP_RAGE,//duration
		15,//FP_PROTECT,//duration
		15,//FP_ABSORB,//duration
		10,//FP_TEAM_HEAL,//instant
		10,//FP_TEAM_FORCE,//instant
		10,//FP_DRAIN,//hold/duration
		5,//FP_SEE,//duration
		5,//FP_SABERATTACK,
		5,//FP_SABERDEFEND,
		5//FP_SABERTHROW,
		//NUM_FORCE_POWERS
	},
	{
		10,//FP_HEAL,//instant
		0,//FP_LEVITATION,//hold/duration
		0,//FP_SPEED,//duration
		0,//FP_PUSH,//hold/duration
		0,//FP_PULL,//hold/duration
		10,//FP_TELEPATHY,//instant
		15,//FP_GRIP,//hold/duration
		10,//FP_LIGHTNING,//hold/duration
		15,//FP_RAGE,//duration
		15,//FP_PROTECT,//duration
		15,//FP_ABSORB,//duration
		10,//FP_TEAM_HEAL,//instant
		10,//FP_TEAM_FORCE,//instant
		10,//FP_DRAIN,//hold/duration
		5,//FP_SEE,//duration
		10,//FP_SABERATTACK,
		10,//FP_SABERDEFEND,
		10//FP_SABERTHROW,
		//NUM_FORCE_POWERS
	}
};

void WP_InitForcePowers( gentity_t *ent )
{
	int i;
	int i_r;
	int maxRank = g_maxForceRank.integer;
	qboolean warnClient = qfalse;
	qboolean warnClientLimit = qfalse;
	char userinfo[MAX_INFO_STRING];
	char forcePowers[256];
	char readBuf[256];
	int lastFPKnown = -1;
	qboolean didEvent = qfalse;

	if (!maxRank)
	{ //if server has no max rank, default to max (50)
		maxRank = FORCE_MASTERY_JEDI_MASTER;
	}

	/*
	if (g_forcePowerDisable.integer)
	{
		maxRank = FORCE_MASTERY_UNINITIATED;
	}
	*/
	//rww - don't do this

	if ( !ent || !ent->client )
	{
		return;
	}

	ent->client->ps.fd.saberAnimLevel = ent->client->sess.saberLevel;

	if (ent->client->ps.fd.saberAnimLevel < FORCE_LEVEL_1 ||
		ent->client->ps.fd.saberAnimLevel > FORCE_LEVEL_3)
	{
		ent->client->ps.fd.saberAnimLevel = FORCE_LEVEL_1;
	}

	if (!speedLoopSound)
	{ //so that the client configstring is already modified with this when we need it
		speedLoopSound = G_SoundIndex("sound/weapons/force/speedloop.wav");
	}

	if (!rageLoopSound)
	{
		rageLoopSound = G_SoundIndex("sound/weapons/force/rageloop.wav");
	}

	if (!absorbLoopSound)
	{
		absorbLoopSound = G_SoundIndex("sound/weapons/force/absorbloop.wav");
	}

	if (!protectLoopSound)
	{
		protectLoopSound = G_SoundIndex("sound/weapons/force/protectloop.wav");
	}

	if (!seeLoopSound)
	{
		seeLoopSound = G_SoundIndex("sound/weapons/force/seeloop.wav");
	}

	if (!ysalamiriLoopSound)
	{
		ysalamiriLoopSound = G_SoundIndex("sound/player/nullifyloop.wav");
	}

	i = 0;
	while (i < NUM_FORCE_POWERS)
	{
		ent->client->ps.fd.forcePowerLevel[i] = 0;
		ent->client->ps.fd.forcePowersKnown &= ~(1 << i);
		i++;
	}

	ent->client->ps.fd.forcePowerSelected = -1;

	ent->client->ps.fd.forceSide = 0;

	trap_GetUserinfo( ent->s.number, userinfo, sizeof( userinfo ) );

	Q_strncpyz( forcePowers, Info_ValueForKey (userinfo, "forcepowers"), sizeof( forcePowers ) );

	if ( ent->r.svFlags & SVF_BOT && botstates[ent->s.number] )
	{ //if it's a bot just copy the info directly from its personality
		Com_sprintf(forcePowers, sizeof(forcePowers), "%s\0", botstates[ent->s.number]->forceinfo);
	}

	//rww - parse through the string manually and eat out all the appropriate data
	i = 0;

	if (g_forceBasedTeams.integer)
	{
		if (ent->client->sess.sessionTeam == TEAM_RED)
		{
			warnClient = !(BG_LegalizedForcePowers(forcePowers, maxRank, HasSetSaberOnly(), FORCE_DARKSIDE, g_gametype.integer, g_forcePowerDisable.integer));
		}
		else if (ent->client->sess.sessionTeam == TEAM_BLUE)
		{
			warnClient = !(BG_LegalizedForcePowers(forcePowers, maxRank, HasSetSaberOnly(), FORCE_LIGHTSIDE, g_gametype.integer, g_forcePowerDisable.integer));
		}
		else
		{
			warnClient = !(BG_LegalizedForcePowers(forcePowers, maxRank, HasSetSaberOnly(), 0, g_gametype.integer, g_forcePowerDisable.integer));
		}
	}
	else
	{
		warnClient = !(BG_LegalizedForcePowers(forcePowers, maxRank, HasSetSaberOnly(), 0, g_gametype.integer, g_forcePowerDisable.integer));
	}

	i_r = 0;
	while (forcePowers[i] && forcePowers[i] != '-')
	{
		readBuf[i_r] = forcePowers[i];
		i_r++;
		i++;
	}
	readBuf[i_r] = 0;
	//THE RANK
	ent->client->ps.fd.forceRank = atoi(readBuf);
	i++;

	i_r = 0;
	while (forcePowers[i] && forcePowers[i] != '-')
	{
		readBuf[i_r] = forcePowers[i];
		i_r++;
		i++;
	}
	readBuf[i_r] = 0;
	//THE SIDE
	ent->client->ps.fd.forceSide = atoi(readBuf);
	i++;

	i_r = 0;
	while (forcePowers[i] && forcePowers[i] != '\n' &&
		i_r < NUM_FORCE_POWERS)
	{
		readBuf[0] = forcePowers[i];
		readBuf[1] = 0;

		ent->client->ps.fd.forcePowerLevel[i_r] = atoi(readBuf);
		if (ent->client->ps.fd.forcePowerLevel[i_r])
		{
			ent->client->ps.fd.forcePowersKnown |= (1 << i_r);
		}
		else
		{
			ent->client->ps.fd.forcePowersKnown &= ~(1 << i_r);
		}
		i++;
		i_r++;
	}
	//THE POWERS

	if (HasSetSaberOnly())
	{
		gentity_t *te = G_TempEntity( vec3_origin, EV_SET_FREE_SABER );
		te->r.svFlags |= SVF_BROADCAST;
		te->s.eventParm = 1;
	}
	else
	{
		gentity_t *te = G_TempEntity( vec3_origin, EV_SET_FREE_SABER );
		te->r.svFlags |= SVF_BROADCAST;
		te->s.eventParm = 0;
	}

	if (g_forcePowerDisable.integer)
	{
		gentity_t *te = G_TempEntity( vec3_origin, EV_SET_FORCE_DISABLE );
		te->r.svFlags |= SVF_BROADCAST;
		te->s.eventParm = 1;
	}
	else
	{
		gentity_t *te = G_TempEntity( vec3_origin, EV_SET_FORCE_DISABLE );
		te->r.svFlags |= SVF_BROADCAST;
		te->s.eventParm = 0;
	}

	//rww - It seems we currently want to always do this, even if the player isn't exceeding the max
	//rank, so..
	if (g_gametype.integer == GT_TOURNAMENT)
	{ //totally messes duel up to force someone into spec mode, and besides, each "round" is
	  //counted as a full restart
		ent->client->sess.setForce = qtrue;
	}

	if (warnClient || !ent->client->sess.setForce)
	{ //the client's rank is too high for the server and has been autocapped, so tell them
		if (g_gametype.integer != GT_HOLOCRON && g_gametype.integer != GT_JEDIMASTER)
		{
#ifdef EVENT_FORCE_RANK
			gentity_t *te = G_TempEntity( vec3_origin, EV_GIVE_NEW_RANK );

			te->r.svFlags |= SVF_BROADCAST;
			te->s.trickedentindex = ent->s.number;
			te->s.eventParm = maxRank;
			te->s.bolt1 = 0;
#endif
			didEvent = qtrue;

			if (!(ent->r.svFlags & SVF_BOT) && g_gametype.integer != GT_TOURNAMENT)
			{
				if (g_gametype.integer < GT_TEAM || !g_teamAutoJoin.integer)
				{
					//Make them a spectator so they can set their powerups up without being bothered.
					ent->client->sess.sessionTeam = TEAM_SPECTATOR;
					ent->client->sess.spectatorState = SPECTATOR_FREE;
					ent->client->sess.spectatorClient = 0;

					ent->client->pers.teamState.state = TEAM_BEGIN;
				}
			}

#ifdef EVENT_FORCE_RANK
			te->s.bolt2 = ent->client->sess.sessionTeam;
#else
			//Event isn't very reliable, I made it a string. This way I can send it to just one
			//client also, as opposed to making a broadcast event.
			trap_SendServerCommand(ent->s.number, va("nfr %i %i %i", maxRank, 1, ent->client->sess.sessionTeam));
			//Arg1 is new max rank, arg2 is non-0 if force menu should be shown, arg3 is the current team
#endif
		}
		ent->client->sess.setForce = qtrue;
	}

	if (!didEvent)
	{
#ifdef EVENT_FORCE_RANK
		gentity_t *te = G_TempEntity( vec3_origin, EV_GIVE_NEW_RANK );

		te->r.svFlags |= SVF_BROADCAST;
		te->s.trickedentindex = ent->s.number;
		te->s.eventParm = maxRank;
		te->s.bolt1 = 1;
		te->s.bolt2 = ent->client->sess.sessionTeam;
#else
		trap_SendServerCommand(ent->s.number, va("nfr %i %i %i", maxRank, 0, ent->client->sess.sessionTeam));
#endif
	}

	if (warnClientLimit)
	{ //the server has one or more force powers disabled and the client is using them in his config
		//trap_SendServerCommand(ent-g_entities, va("print \"The server has one or more force powers that you have chosen disabled.\nYou will not be able to use the disable force power(s) while playing on this server.\n\""));
	}

	i = 0;
	while (i < NUM_FORCE_POWERS)
	{
		if ((ent->client->ps.fd.forcePowersKnown & (1 << i)) &&
			!ent->client->ps.fd.forcePowerLevel[i])
		{ //err..
			ent->client->ps.fd.forcePowersKnown &= ~(1 << i);
		}
		else
		{
			if (i != FP_LEVITATION && i != FP_SABERATTACK && i != FP_SABERDEFEND && i != FP_SABERTHROW)
			{
				lastFPKnown = i;
			}
		}

		i++;
	}

	if (ent->client->ps.fd.forcePowersKnown & ent->client->sess.selectedFP)
	{
		ent->client->ps.fd.forcePowerSelected = ent->client->sess.selectedFP;
	}

	if (!(ent->client->ps.fd.forcePowersKnown & (1 << ent->client->ps.fd.forcePowerSelected)))
	{
		if (lastFPKnown != -1)
		{
			ent->client->ps.fd.forcePowerSelected = lastFPKnown;
		}
		else
		{
			ent->client->ps.fd.forcePowerSelected = 0;
		}
	}
}

void WP_SpawnInitForcePowers( gentity_t *ent )
{
	int i = 0;

	ent->client->ps.saberAttackChainCount = 0;

	i = 0;

	while (i < NUM_FORCE_POWERS)
	{
		if (ent->client->ps.fd.forcePowersActive & (1 << i))
		{
			WP_ForcePowerStop(ent, i);
		}

		i++;
	}

	ent->client->ps.fd.forceDeactivateAll = 0;

	ent->client->ps.fd.forcePower = ent->client->ps.fd.forcePowerMax = FORCE_POWER_MAX;
	ent->client->ps.fd.forcePowerRegenDebounceTime = 0;
	ent->client->ps.fd.forceGripEntityNum = ENTITYNUM_NONE;
	ent->client->ps.fd.forceMindtrickTargetIndex = 0;
	ent->client->ps.fd.forceMindtrickTargetIndex2 = 0;
	ent->client->ps.fd.forceMindtrickTargetIndex3 = 0;
	ent->client->ps.fd.forceMindtrickTargetIndex4 = 0;

	ent->client->ps.holocronBits = 0;

	i = 0;
	while (i < NUM_FORCE_POWERS)
	{
		ent->client->ps.holocronsCarried[i] = 0;
		i++;
	}

	if (g_gametype.integer == GT_HOLOCRON)
	{
		i = 0;
		while (i < NUM_FORCE_POWERS)
		{
			ent->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_0;
			i++;
		}

		if (HasSetSaberOnly())
		{
			/*
			ent->client->ps.fd.forcePowerLevel[FP_SABERATTACK] = FORCE_LEVEL_3;
			ent->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] = FORCE_LEVEL_3;
			ent->client->ps.fd.forcePowerLevel[FP_SABERTHROW] = FORCE_LEVEL_3;
			*/
			if (ent->client->ps.fd.forcePowerLevel[FP_SABERATTACK] < FORCE_LEVEL_1)
			{
				ent->client->ps.fd.forcePowerLevel[FP_SABERATTACK] = FORCE_LEVEL_1;
			}
			if (ent->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] < FORCE_LEVEL_1)
			{
				ent->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] = FORCE_LEVEL_1;
			}
		}
	}

	i = 0;

	while (i < NUM_FORCE_POWERS)
	{
		ent->client->ps.fd.forcePowerBaseLevel[i] = ent->client->ps.fd.forcePowerLevel[i];

		ent->client->ps.fd.forcePowerDebounce[i] = 0;
		ent->client->ps.fd.forcePowerDuration[i] = 0;

		i++;
	}

	ent->client->ps.fd.forcePowerRegenDebounceTime = 0;
	ent->client->ps.fd.forceUsingAdded = 0;
	ent->client->ps.fd.forceJumpZStart = 0;
	ent->client->ps.fd.forceJumpCharge = 0;
	ent->client->ps.fd.forceJumpSound = 0;
	ent->client->ps.fd.forceGripDamageDebounceTime = 0;
	ent->client->ps.fd.forceGripBeingGripped = 0;
	ent->client->ps.fd.forceGripCripple = 0;
	ent->client->ps.fd.forceGripUseTime = 0;
	ent->client->ps.fd.forceGripSoundTime = 0;
	ent->client->ps.fd.forceGripStarted = 0;
	ent->client->ps.fd.forceSpeedSmash = 0;
	ent->client->ps.fd.forceSpeedDoDamage = 0;
	ent->client->ps.fd.forceSpeedHitIndex = 0;
	ent->client->ps.fd.forceHealTime = 0;
	ent->client->ps.fd.forceHealAmount = 0;
	ent->client->ps.fd.forceRageRecoveryTime = 0;
	ent->client->ps.fd.forceDrainEntNum = ENTITYNUM_NONE;
	ent->client->ps.fd.forceDrainTime = 0;

	i = 0;
	while (i < NUM_FORCE_POWERS)
	{
		if ((ent->client->ps.fd.forcePowersKnown & (1 << i)) &&
			!ent->client->ps.fd.forcePowerLevel[i])
		{ //err..
			ent->client->ps.fd.forcePowersKnown &= ~(1 << i);
		}

		i++;
	}
}

int ForcePowerUsableOn(gentity_t *attacker, gentity_t *other, forcePowers_t forcePower)
{
	if (other && other->client && other->client->ps.usingATST)
	{
		return 0;
	}

	if (other && other->client && BG_HasYsalamiri(g_gametype.integer, &other->client->ps))
	{
		return 0;
	}

	if (attacker && attacker->client && !BG_CanUseFPNow(g_gametype.integer, &attacker->client->ps, level.time, forcePower))
	{
		return 0;
	}

	//Dueling fighters cannot use force powers on others, with the exception of force push when locked with each other
	if (attacker && attacker->client && attacker->client->ps.duelInProgress)
	{
		return 0;
	}

	if (other && other->client && other->client->ps.duelInProgress)
	{
		return 0;
	}

	return 1;
}

qboolean WP_ForcePowerAvailable( gentity_t *self, forcePowers_t forcePower )
{
	int	drain = forcePowerNeeded[self->client->ps.fd.forcePowerLevel[forcePower]][forcePower];

	if (self->client->ps.fd.forcePowersActive & (1 << forcePower))
	{ //we're probably going to deactivate it..
		return qtrue;
	}

	if ( forcePower == FP_LEVITATION )
	{
		return qtrue;
	}
	if ( !drain )
	{
		return qtrue;
	}
	if ( self->client->ps.fd.forcePower < drain )
	{
		return qfalse;
	}
	return qtrue;
}

qboolean WP_ForcePowerInUse( gentity_t *self, forcePowers_t forcePower )
{
	if ( (self->client->ps.fd.forcePowersActive & ( 1 << forcePower )) )
	{//already using this power
		return qtrue;
	}

	return qfalse;
}

qboolean WP_ForcePowerUsable( gentity_t *self, forcePowers_t forcePower )
{
	if (BG_HasYsalamiri(g_gametype.integer, &self->client->ps))
	{
		return qfalse;
	}

	if (self->health <= 0 || self->client->ps.stats[STAT_HEALTH] <= 0 ||
		(self->client->ps.eFlags & EF_DEAD))
	{
		return qfalse;
	}

	if (self->client->ps.pm_flags & PMF_FOLLOW)
	{ //specs can't use powers through people
		return qfalse;
	}
	if (self->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		return qfalse;
	}

	if (!BG_CanUseFPNow(g_gametype.integer, &self->client->ps, level.time, forcePower))
	{
		return qfalse;
	}

	if ( !(self->client->ps.fd.forcePowersKnown & ( 1 << forcePower )) )
	{//don't know this power
		return qfalse;
	}
	
	if ( (self->client->ps.fd.forcePowersActive & ( 1 << forcePower )) )
	{//already using this power
		if (forcePower != FP_LEVITATION)
		{
			return qfalse;
		}
	}

	if (forcePower == FP_LEVITATION && self->client->fjDidJump)
	{
		return qfalse;
	}

	if (!self->client->ps.fd.forcePowerLevel[forcePower])
	{
		return qfalse;
	}

	return WP_ForcePowerAvailable( self, forcePower );
}

int WP_AbsorbConversion(gentity_t *attacked, int atdAbsLevel, gentity_t *attacker, int atPower, int atPowerLevel, int atForceSpent)
{
	int getLevel = 0;
	int addTot = 0;

	if (atPower != FP_LIGHTNING &&
		atPower != FP_DRAIN &&
		atPower != FP_GRIP &&
		atPower != FP_PUSH &&
		atPower != FP_PULL)
	{ //Only these powers can be absorbed
		return -1;
	}

	if (!atdAbsLevel)
	{ //looks like attacker doesn't have any absorb power
		return -1;
	}

	if (!(attacked->client->ps.fd.forcePowersActive & (1 << FP_ABSORB)))
	{ //absorb is not active
		return -1;
	}

	//Subtract absorb power level from the offensive force power
	getLevel = atPowerLevel;
	getLevel -= atdAbsLevel;

	if (getLevel < 0)
	{
		getLevel = 0;
	}

	//let the attacker absorb an amount of force used in this attack based on his level of absorb
	addTot = (atForceSpent/3)*attacked->client->ps.fd.forcePowerLevel[FP_ABSORB];

	if (addTot < 1 && atForceSpent >= 1)
	{
		addTot = 1;
	}
	attacked->client->ps.fd.forcePower += addTot;
	if (attacked->client->ps.fd.forcePower > 100)
	{
		attacked->client->ps.fd.forcePower = 100;
	}

	//play sound indicating that attack was absorbed
	G_PreDefSound(attacker->client->ps.origin, PDSOUND_ABSORBHIT);

	return getLevel;
}

void WP_ForcePowerRegenerate( gentity_t *self, int overrideAmt )
{
	if ( !self->client )
	{
		return;
	}

	if ( overrideAmt )
	{
		self->client->ps.fd.forcePower += overrideAmt;
	}
	else
	{
		self->client->ps.fd.forcePower++;
	}
	if ( self->client->ps.fd.forcePower > self->client->ps.fd.forcePowerMax )
	{
		self->client->ps.fd.forcePower = self->client->ps.fd.forcePowerMax;
	}
}

void WP_ForcePowerStart( gentity_t *self, forcePowers_t forcePower, int overrideAmt )
{
	int	duration = 0;
	qboolean hearable = qfalse;
	float hearDist = 0;

	if (!WP_ForcePowerAvailable( self, forcePower ))
	{
		return;
	}

	//FIXME: debounce some of these

	//and it in
	//set up duration time
	switch( (int)forcePower )
	{
	case FP_HEAL:
		hearable = qtrue;
		hearDist = 256;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_LEVITATION:
		hearable = qtrue;
		hearDist = 256;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_SPEED:
		hearable = qtrue;
		hearDist = 256;
		if (self->client->ps.fd.forcePowerLevel[FP_SPEED] == FORCE_LEVEL_1)
		{
			duration = 10000;
		}
		else if (self->client->ps.fd.forcePowerLevel[FP_SPEED] == FORCE_LEVEL_2)
		{
			duration = 15000;
		}
		else if (self->client->ps.fd.forcePowerLevel[FP_SPEED] == FORCE_LEVEL_3)
		{
			duration = 20000;
		}
		else //shouldn't get here
		{
			break;
		}

		if (overrideAmt)
		{
			duration = overrideAmt;
		}

		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_PUSH:
		hearable = qtrue;
		hearDist = 256;
		break;
	case FP_PULL:
		hearable = qtrue;
		hearDist = 256;
		break;
	case FP_TELEPATHY:
		hearable = qtrue;
		hearDist = 256;
		if (self->client->ps.fd.forcePowerLevel[FP_TELEPATHY] == FORCE_LEVEL_1)
		{
			duration = 20000;
		}
		else if (self->client->ps.fd.forcePowerLevel[FP_TELEPATHY] == FORCE_LEVEL_2)
		{
			duration = 25000;
		}
		else if (self->client->ps.fd.forcePowerLevel[FP_TELEPATHY] == FORCE_LEVEL_3)
		{
			duration = 30000;
		}
		else //shouldn't get here
		{
			break;
		}

		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_GRIP:
		hearable = qtrue;
		hearDist = 256;
		//duration = 1000;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		self->client->ps.powerups[PW_DISINT_4] = level.time + 60000;
		break;
	case FP_LIGHTNING:
		hearable = qtrue;
		hearDist = 512;
		duration = overrideAmt;
		overrideAmt = 0;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		self->client->ps.activeForcePass = self->client->ps.fd.forcePowerLevel[FP_LIGHTNING];
		break;
	case FP_RAGE:
		hearable = qtrue;
		hearDist = 256;
		if (self->client->ps.fd.forcePowerLevel[FP_RAGE] == FORCE_LEVEL_1)
		{
			duration = 8000;
		}
		else if (self->client->ps.fd.forcePowerLevel[FP_RAGE] == FORCE_LEVEL_2)
		{
			duration = 14000;
		}
		else if (self->client->ps.fd.forcePowerLevel[FP_RAGE] == FORCE_LEVEL_3)
		{
			duration = 20000;
		}
		else //shouldn't get here
		{
			break;
		}

		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_PROTECT:
		hearable = qtrue;
		hearDist = 256;
		duration = 20000;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_ABSORB:
		hearable = qtrue;
		hearDist = 256;
		duration = 20000;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_TEAM_HEAL:
		hearable = qtrue;
		hearDist = 256;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_TEAM_FORCE:
		hearable = qtrue;
		hearDist = 256;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_DRAIN:
		hearable = qtrue;
		hearDist = 256;
		duration = overrideAmt;
		overrideAmt = 0;
		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		//self->client->ps.activeForcePass = self->client->ps.fd.forcePowerLevel[FP_DRAIN];
		break;
	case FP_SEE:
		hearable = qtrue;
		hearDist = 256;
		if (self->client->ps.fd.forcePowerLevel[FP_SEE] == FORCE_LEVEL_1)
		{
			duration = 10000;
		}
		else if (self->client->ps.fd.forcePowerLevel[FP_SEE] == FORCE_LEVEL_2)
		{
			duration = 20000;
		}
		else if (self->client->ps.fd.forcePowerLevel[FP_SEE] == FORCE_LEVEL_3)
		{
			duration = 30000;
		}
		else //shouldn't get here
		{
			break;
		}

		self->client->ps.fd.forcePowersActive |= ( 1 << forcePower );
		break;
	case FP_SABERATTACK:
		break;
	case FP_SABERDEFEND:
		break;
	case FP_SABERTHROW:
		break;
	default:
		break;
	}
	if ( duration )
	{
		self->client->ps.fd.forcePowerDuration[forcePower] = level.time + duration;
	}
	else
	{
		self->client->ps.fd.forcePowerDuration[forcePower] = 0;
	}

	if (hearable)
	{
		self->client->ps.otherSoundLen = hearDist;
		self->client->ps.otherSoundTime = level.time + 100;
	}
	
	self->client->ps.fd.forcePowerDebounce[forcePower] = 0;

	if ((int)forcePower == FP_SPEED && overrideAmt)
	{
		BG_ForcePowerDrain( &self->client->ps, forcePower, overrideAmt*0.025 );
	}
	else if ((int)forcePower != FP_GRIP && (int)forcePower != FP_DRAIN)
	{ //grip and drain drain as damage is done
		BG_ForcePowerDrain( &self->client->ps, forcePower, overrideAmt );
	}
}

void ForceHeal( gentity_t *self )
{
	if ( self->health <= 0 )
	{
		return;
	}

	/*if ( WP_ForcePowerInUse(self, FP_HEAL))
	{
		WP_ForcePowerStop(self, FP_HEAL);
		return;
	}*/

	if ( !WP_ForcePowerUsable( self, FP_HEAL ) )
	{
		return;
	}

	if ( self->health >= self->client->ps.stats[STAT_MAX_HEALTH])
	{
		return;
	}

	if (self->client->ps.fd.forcePowerLevel[FP_HEAL] == FORCE_LEVEL_3)
	{
		self->health += 50;
		
		if (self->health > self->client->ps.stats[STAT_MAX_HEALTH])
		{
			self->health = self->client->ps.stats[STAT_MAX_HEALTH];
		}
		BG_ForcePowerDrain( &self->client->ps, FP_HEAL, 0 );
	}
	else if (self->client->ps.fd.forcePowerLevel[FP_HEAL] == FORCE_LEVEL_2)
	{
		self->health += 25;
		
		if (self->health > self->client->ps.stats[STAT_MAX_HEALTH])
		{
			self->health = self->client->ps.stats[STAT_MAX_HEALTH];
		}
		BG_ForcePowerDrain( &self->client->ps, FP_HEAL, 0 );
	}
	else
	{
		self->health += 10;
		
		if (self->health > self->client->ps.stats[STAT_MAX_HEALTH])
		{
			self->health = self->client->ps.stats[STAT_MAX_HEALTH];
		}
		BG_ForcePowerDrain( &self->client->ps, FP_HEAL, 0 );
	}
	/*
	else
	{
		WP_ForcePowerStart( self, FP_HEAL, 0 );
	}
	*/
	//NOTE: Decided to make all levels instant.

	G_Sound( self, CHAN_ITEM, G_SoundIndex("sound/weapons/force/heal.wav") );
	// No character heal voices
//	G_Sound( self, CHAN_VOICE, G_SoundIndex(va( "sound/weapons/force/heal%d.mp3", Q_irand( 1, 4 ) )) );
}

void WP_AddToClientBitflags(gentity_t *ent, int entNum)
{
	if (!ent)
	{
		return;
	}

	if (entNum > 47)
	{
		ent->s.trickedentindex4 |= (1 << (entNum-48));
	}
	else if (entNum > 31)
	{
		ent->s.trickedentindex3 |= (1 << (entNum-32));
	}
	else if (entNum > 15)
	{
		ent->s.trickedentindex2 |= (1 << (entNum-16));
	}
	else
	{
		ent->s.trickedentindex |= (1 << entNum);
	}
}

void ForceTeamHeal( gentity_t *self )
{
	float radius = 256;
	int i = 0;
	gentity_t *ent;
	vec3_t a;
	int numpl = 0;
	int pl[MAX_CLIENTS];
	int healthadd = 0;
	gentity_t *te = NULL;

	if ( self->health <= 0 )
	{
		return;
	}

	if ( !WP_ForcePowerUsable( self, FP_TEAM_HEAL ) )
	{
		return;
	}

	if (self->client->ps.fd.forcePowerLevel[FP_TEAM_HEAL] == FORCE_LEVEL_2)
	{
		radius *= 1.5;
	}
	if (self->client->ps.fd.forcePowerLevel[FP_TEAM_HEAL] == FORCE_LEVEL_3)
	{
		radius *= 2;
	}

	while (i < MAX_CLIENTS)
	{
		ent = &g_entities[i];

		if (ent && ent->client && self != ent && OnSameTeam(self, ent) && ent->client->ps.stats[STAT_HEALTH] < ent->client->ps.stats[STAT_MAX_HEALTH] && ent->client->ps.stats[STAT_HEALTH] > 0 && ForcePowerUsableOn(self, ent, FP_TEAM_HEAL) &&
			trap_InPVS(self->client->ps.origin, ent->client->ps.origin))
		{
			VectorSubtract(self->client->ps.origin, ent->client->ps.origin, a);

			if (VectorLength(a) <= radius)
			{
				pl[numpl] = i;
				numpl++;
			}
		}

		i++;
	}

	if (numpl < 1)
	{
		return;
	}

	if (numpl == 1)
	{
		healthadd = 50;
	}
	else if (numpl == 2)
	{
		healthadd = 33;
	}
	else
	{
		healthadd = 25;
	}

	i = 0;

	while (i < numpl)
	{
		if (g_entities[pl[i]].client->ps.stats[STAT_HEALTH] > 0 &&
			g_entities[pl[i]].health > 0)
		{
			g_entities[pl[i]].client->ps.stats[STAT_HEALTH] += healthadd;
			if (g_entities[pl[i]].client->ps.stats[STAT_HEALTH] > g_entities[pl[i]].client->ps.stats[STAT_MAX_HEALTH])
			{
				g_entities[pl[i]].client->ps.stats[STAT_HEALTH] = g_entities[pl[i]].client->ps.stats[STAT_MAX_HEALTH];
			}

			g_entities[pl[i]].health = g_entities[pl[i]].client->ps.stats[STAT_HEALTH];

			//At this point we know we got one, so add him into the collective event client bitflag
			if (!te)
			{
				te = G_TempEntity( self->client->ps.origin, EV_TEAM_POWER);
				te->s.eventParm = 1; //eventParm 1 is heal, eventParm 2 is force regen

				//since we had an extra check above, do the drain now because we got at least one guy
				BG_ForcePowerDrain( &self->client->ps, FP_TEAM_HEAL, forcePowerNeeded[self->client->ps.fd.forcePowerLevel[FP_TEAM_HEAL]][FP_TEAM_HEAL] );
			}

			WP_AddToClientBitflags(te, pl[i]);
			//Now cramming it all into one event.. doing this many g_sound events at once was a Bad Thing.
			//G_Sound( &g_entities[pl[i]], CHAN_ITEM, G_SoundIndex("sound/weapons/force/teamheal.wav") );
		}
		i++;
	}
}

void ForceTeamForceReplenish( gentity_t *self )
{
	float radius = 256;
	int i = 0;
	gentity_t *ent;
	vec3_t a;
	int numpl = 0;
	int pl[MAX_CLIENTS];
	int poweradd = 0;
	gentity_t *te = NULL;

	if ( self->health <= 0 )
	{
		return;
	}

	if ( !WP_ForcePowerUsable( self, FP_TEAM_FORCE ) )
	{
		return;
	}

	if (self->client->ps.fd.forcePowerLevel[FP_TEAM_FORCE] == FORCE_LEVEL_2)
	{
		radius *= 1.5;
	}
	if (self->client->ps.fd.forcePowerLevel[FP_TEAM_FORCE] == FORCE_LEVEL_3)
	{
		radius *= 2;
	}

	while (i < MAX_CLIENTS)
	{
		ent = &g_entities[i];

		if (ent && ent->client && self != ent && OnSameTeam(self, ent) && ent->client->ps.fd.forcePower < 100 && ForcePowerUsableOn(self, ent, FP_TEAM_FORCE) &&
			trap_InPVS(self->client->ps.origin, ent->client->ps.origin))
		{
			VectorSubtract(self->client->ps.origin, ent->client->ps.origin, a);

			if (VectorLength(a) <= radius)
			{
				pl[numpl] = i;
				numpl++;
			}
		}

		i++;
	}

	if (numpl < 1)
	{
		return;
	}

	if (numpl == 1)
	{
		poweradd = 50;
	}
	else if (numpl == 2)
	{
		poweradd = 33;
	}
	else
	{
		poweradd = 25;
	}

	BG_ForcePowerDrain( &self->client->ps, FP_TEAM_FORCE, forcePowerNeeded[self->client->ps.fd.forcePowerLevel[FP_TEAM_FORCE]][FP_TEAM_FORCE] );

	i = 0;

	while (i < numpl)
	{
		g_entities[pl[i]].client->ps.fd.forcePower += poweradd;
		if (g_entities[pl[i]].client->ps.fd.forcePower > 100)
		{
			g_entities[pl[i]].client->ps.fd.forcePower = 100;
		}

		//At this point we know we got one, so add him into the collective event client bitflag
		if (!te)
		{
			te = G_TempEntity( self->client->ps.origin, EV_TEAM_POWER);
			te->s.eventParm = 2; //eventParm 1 is heal, eventParm 2 is force regen
		}

		WP_AddToClientBitflags(te, pl[i]);
		//Now cramming it all into one event.. doing this many g_sound events at once was a Bad Thing.
		//G_Sound( &g_entities[pl[i]], CHAN_ITEM, G_SoundIndex("sound/weapons/force/teamforce.wav") );
		
		i++;
	}
}

void ForceGrip( gentity_t *self )
{
	trace_t tr;
	vec3_t tfrom, tto, fwd;

	if ( self->health <= 0 )
	{
		return;
	}

	if (self->client->ps.forceHandExtend != HANDEXTEND_NONE)
	{
		return;
	}

	if (self->client->ps.weaponTime > 0)
	{
		return;
	}

	if (self->client->ps.fd.forceGripUseTime > level.time)
	{
		return;
	}

	if ( !WP_ForcePowerUsable( self, FP_GRIP ) )
	{
		return;
	}

	VectorCopy(self->client->ps.origin, tfrom);
	tfrom[2] += self->client->ps.viewheight;
	AngleVectors(self->client->ps.viewangles, fwd, NULL, NULL);
	tto[0] = tfrom[0] + fwd[0]*MAX_GRIP_DISTANCE;
	tto[1] = tfrom[1] + fwd[1]*MAX_GRIP_DISTANCE;
	tto[2] = tfrom[2] + fwd[2]*MAX_GRIP_DISTANCE;

	trap_Trace(&tr, tfrom, NULL, NULL, tto, self->s.number, MASK_PLAYERSOLID);

	if ( tr.fraction != 1.0 &&
		tr.entityNum != ENTITYNUM_NONE &&
		g_entities[tr.entityNum].client &&
		!g_entities[tr.entityNum].client->ps.fd.forceGripCripple &&
		g_entities[tr.entityNum].client->ps.fd.forceGripBeingGripped < level.time &&
		ForcePowerUsableOn(self, &g_entities[tr.entityNum], FP_GRIP) &&
		//g_entities[tr.entityNum].client->ps.fd.forcePowerLevel[FP_ABSORB] < self->client->ps.fd.forcePowerLevel[FP_GRIP] &&
		(g_friendlyFire.integer || !OnSameTeam(self, &g_entities[tr.entityNum])) ) //don't grip someone who's still crippled
	{
		self->client->ps.fd.forceGripEntityNum = tr.entityNum;
		g_entities[tr.entityNum].client->ps.fd.forceGripStarted = level.time;
		self->client->ps.fd.forceGripDamageDebounceTime = 0;

		self->client->ps.forceHandExtend = HANDEXTEND_FORCEGRIP;//HANDEXTEND_FORCEPUSH;
		self->client->ps.forceHandExtendTime = level.time + 5000;
	}
	else
	{
		self->client->ps.fd.forceGripEntityNum = ENTITYNUM_NONE;
		return;
	}
}

void ForceSpeed( gentity_t *self, int forceDuration )
{
	if ( self->health <= 0 )
	{
		return;
	}

	if (self->client->ps.forceAllowDeactivateTime < level.time &&
		(self->client->ps.fd.forcePowersActive & (1 << FP_SPEED)) )
	{
		WP_ForcePowerStop( self, FP_SPEED );
		return;
	}

	if ( !WP_ForcePowerUsable( self, FP_SPEED ) )
	{
		return;
	}

	self->client->ps.forceAllowDeactivateTime = level.time + 1500;

	WP_ForcePowerStart( self, FP_SPEED, forceDuration );
	G_Sound( self, CHAN_BODY, G_SoundIndex("sound/weapons/force/speed.wav") );
	//G_AddEvent(self, EV_STARTLOOPINGSOUND, speedLoopSound);
	G_Sound( self, TRACK_CHANNEL_2, speedLoopSound );
	self->client->ps.fd.forceSpeedSmash = 2; //initial boost (will automax to whatever is appropriate for force level)
	self->client->ps.fd.forceSpeedDoDamage = 0;
}

void ForceSeeing( gentity_t *self )
{
	if ( self->health <= 0 )
	{
		return;
	}

	if (self->client->ps.forceAllowDeactivateTime < level.time &&
		(self->client->ps.fd.forcePowersActive & (1 << FP_SEE)) )
	{
		WP_ForcePowerStop( self, FP_SEE );
		return;
	}

	if ( !WP_ForcePowerUsable( self, FP_SEE ) )
	{
		return;
	}

	self->client->ps.forceAllowDeactivateTime = level.time + 1500;

	WP_ForcePowerStart( self, FP_SEE, 0 );

	G_Sound( self, CHAN_AUTO, G_SoundIndex("sound/weapons/force/see.wav") );
	G_Sound( self, TRACK_CHANNEL_5, seeLoopSound );
}

void ForceProtect( gentity_t *self )
{
	if ( self->health <= 0 )
	{
		return;
	}

	if (self->client->ps.forceAllowDeactivateTime < level.time &&
		(self->client->ps.fd.forcePowersActive & (1 << FP_PROTECT)) )
	{
		WP_ForcePowerStop( self, FP_PROTECT );
		return;
	}

	if ( !WP_ForcePowerUsable( self, FP_PROTECT ) )
	{
		return;
	}

	// Make sure to turn off Force Rage and Force Absorb.
	if (self->client->ps.fd.forcePowersActive & (1 << FP_RAGE) )
	{
		WP_ForcePowerStop( self, FP_RAGE );
	}
	if (self->client->ps.fd.forcePowersActive & (1 << FP_ABSORB) )
	{
		WP_ForcePowerStop( self, FP_ABSORB );
	}

	self->client->ps.forceAllowDeactivateTime = level.time + 1500;

	WP_ForcePowerStart( self, FP_PROTECT, 0 );
	G_PreDefSound(self->client->ps.origin, PDSOUND_PROTECT);
	G_Sound( self, TRACK_CHANNEL_3, protectLoopSound );
}

void ForceAbsorb( gentity_t *self )
{
	if ( self->health <= 0 )
	{
		return;
	}

	if (self->client->ps.forceAllowDeactivateTime < level.time &&
		(self->client->ps.fd.forcePowersActive & (1 << FP_ABSORB)) )
	{
		WP_ForcePowerStop( self, FP_ABSORB );
		return;
	}

	if ( !WP_ForcePowerUsable( self, FP_ABSORB ) )
	{
		return;
	}

	// Make sure to turn off Force Rage and Force Protection.
	if (self->client->ps.fd.forcePowersActive & (1 << FP_RAGE) )
	{
		WP_ForcePowerStop( self, FP_RAGE );
	}
	if (self->client->ps.fd.forcePowersActive & (1 << FP_PROTECT) )
	{
		WP_ForcePowerStop( self, FP_PROTECT );
	}

	self->client->ps.forceAllowDeactivateTime = level.time + 1500;

	WP_ForcePowerStart( self, FP_ABSORB, 0 );
	G_PreDefSound(self->client->ps.origin, PDSOUND_ABSORB);
	G_Sound( self, TRACK_CHANNEL_3, absorbLoopSound );
}

void ForceRage( gentity_t *self )
{
	if ( self->health <= 0 )
	{
		return;
	}

	if (self->client->ps.forceAllowDeactivateTime < level.time &&
		(self->client->ps.fd.forcePowersActive & (1 << FP_RAGE)) )
	{
		WP_ForcePowerStop( self, FP_RAGE );
		return;
	}

	if ( !WP_ForcePowerUsable( self, FP_RAGE ) )
	{
		return;
	}

	if (self->client->ps.fd.forceRageRecoveryTime >= level.time)
	{
		return;
	}

	if (self->health < 10)
	{
		return;
	}

	// Make sure to turn off Force Protection and Force Absorb.
	if (self->client->ps.fd.forcePowersActive & (1 << FP_PROTECT) )
	{
		WP_ForcePowerStop( self, FP_PROTECT );
	}
	if (self->client->ps.fd.forcePowersActive & (1 << FP_ABSORB) )
	{
		WP_ForcePowerStop( self, FP_ABSORB );
	}

	self->client->ps.forceAllowDeactivateTime = level.time + 1500;

	WP_ForcePowerStart( self, FP_RAGE, 0 );

	G_Sound( self, TRACK_CHANNEL_4, G_SoundIndex("sound/weapons/force/rage.wav") );
	G_Sound( self, TRACK_CHANNEL_3, rageLoopSound );
}

void ForceLightning( gentity_t *self )
{
	if ( self->health <= 0 )
	{
		return;
	}
	if ( self->client->ps.fd.forcePower < 25 || !WP_ForcePowerUsable( self, FP_LIGHTNING ) )
	{
		return;
	}
	if ( self->client->ps.fd.forcePowerDebounce[FP_LIGHTNING] > level.time )
	{//stops it while using it and also after using it, up to 3 second delay
		return;
	}

	if (self->client->ps.forceHandExtend != HANDEXTEND_NONE)
	{
		return;
	}

	if (self->client->ps.weaponTime > 0)
	{
		return;
	}

	//Shoot lightning from hand
//	self->client->ps.forceHandExtend = HANDEXTEND_FORCEPUSH;
//	self->client->ps.forceHandExtendTime = level.time + 1000;
	//rww - using grip anim now, to extend the burst time
	self->client->ps.forceHandExtend = HANDEXTEND_FORCEGRIP;//HANDEXTEND_FORCEPUSH;
	self->client->ps.forceHandExtendTime = level.time + 20000;


	//G_SoundOnEnt( self, CHAN_BODY, "sound/weapons/force/lightning.wav" );
	G_Sound( self, CHAN_BODY, G_SoundIndex("sound/weapons/force/lightning.wav") );
	
	WP_ForcePowerStart( self, FP_LIGHTNING, 500 );
}

void ForceLightningDamage( gentity_t *self, gentity_t *traceEnt, vec3_t dir, vec3_t impactPoint )
{
	self->client->dangerTime = level.time;
	self->client->ps.eFlags &= ~EF_INVULNERABLE;
	self->client->invulnerableTimer = 0;

	if ( traceEnt && traceEnt->takedamage )
	{
		if ( traceEnt->client )
		{//an enemy or object
			if (ForcePowerUsableOn(self, traceEnt, FP_LIGHTNING))
			{
				int	dmg = Q_irand(1,2); //Q_irand( 1, 3 );
				
				int modPowerLevel = -1;
				
				if (traceEnt->client)
				{
					modPowerLevel = WP_AbsorbConversion(traceEnt, traceEnt->client->ps.fd.forcePowerLevel[FP_ABSORB], self, FP_LIGHTNING, self->client->ps.fd.forcePowerLevel[FP_LIGHTNING], 1);
				}

				if (modPowerLevel != -1)
				{
					if (!modPowerLevel)
					{
						dmg = 0;
					}
					else if (modPowerLevel == 1)
					{
						dmg = 1;
					}
					else if (modPowerLevel == 2)
					{
						dmg = 1;
					}
				}

				if (dmg)
				{
					//rww - Shields can now absorb lightning too.
					G_Damage( traceEnt, self, self, dir, impactPoint, dmg, /*DAMAGE_NO_ARMOR*/0, MOD_FORCE_DARK );
				}
				if ( traceEnt->client )
				{
					if ( !Q_irand( 0, 2 ) )
					{
						G_Sound( traceEnt, CHAN_BODY, G_SoundIndex( "sound/weapons/force/lightninghit.wav" ) );
					}

					if (traceEnt->client->ps.electrifyTime < (level.time + 400))
					{ //only update every 400ms to reduce bandwidth usage (as it is passing a 32-bit time value)
						traceEnt->client->ps.electrifyTime = level.time + 800;
					}
				}
			}
		}
	}
}

void ForceShootLightning( gentity_t *self )
{
	trace_t	tr;
	vec3_t	end, forward;
	gentity_t	*traceEnt;

	if ( self->health <= 0 )
	{
		return;
	}
	AngleVectors( self->client->ps.viewangles, forward, NULL, NULL );
	VectorNormalize( forward );

	if ( self->client->ps.fd.forcePowerLevel[FP_LIGHTNING] > FORCE_LEVEL_2 )
	{//arc
		vec3_t	center, mins, maxs, dir, ent_org, size, v;
		float	radius = FORCE_LIGHTNING_RADIUS, dot, dist;
		gentity_t	*entityList[MAX_GENTITIES];
		int			iEntityList[MAX_GENTITIES];
		int		e, numListedEntities, i;

		VectorCopy( self->client->ps.origin, center );
		for ( i = 0 ; i < 3 ; i++ ) 
		{
			mins[i] = center[i] - radius;
			maxs[i] = center[i] + radius;
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
			traceEnt = entityList[e];

			if ( !traceEnt )
				continue;
			if ( traceEnt == self )
				continue;
			if ( traceEnt->r.ownerNum == self->s.number && traceEnt->s.weapon != WP_THERMAL )//can push your own thermals
				continue;
			if ( !traceEnt->inuse )
				continue;
			if ( !traceEnt->takedamage )
				continue;
			if ( traceEnt->health <= 0 )//no torturing corpses
				continue;
			if ( !g_friendlyFire.integer && OnSameTeam(self, traceEnt))
				continue;
			//this is all to see if we need to start a saber attack, if it's in flight, this doesn't matter
			// find the distance from the edge of the bounding box
			for ( i = 0 ; i < 3 ; i++ ) 
			{
				if ( center[i] < traceEnt->r.absmin[i] ) 
				{
					v[i] = traceEnt->r.absmin[i] - center[i];
				} else if ( center[i] > traceEnt->r.absmax[i] ) 
				{
					v[i] = center[i] - traceEnt->r.absmax[i];
				} else 
				{
					v[i] = 0;
				}
			}

			VectorSubtract( traceEnt->r.absmax, traceEnt->r.absmin, size );
			VectorMA( traceEnt->r.absmin, 0.5, size, ent_org );

			//see if they're in front of me
			//must be within the forward cone
			VectorSubtract( ent_org, center, dir );
			VectorNormalize( dir );
			if ( (dot = DotProduct( dir, forward )) < 0.5 )
				continue;

			//must be close enough
			dist = VectorLength( v );
			if ( dist >= radius ) 
			{
				continue;
			}
		
			//in PVS?
			if ( !traceEnt->r.bmodel && !trap_InPVS( ent_org, /*self->client->renderInfo.handLPoint*/self->client->ps.origin ) )
			{//must be in PVS
				continue;
			}

			//Now check and see if we can actually hit it
			trap_Trace( &tr, self->client->ps.origin, vec3_origin, vec3_origin, ent_org, self->s.number, MASK_SHOT );
			if ( tr.fraction < 1.0f && tr.entityNum != traceEnt->s.number )
			{//must have clear LOS
				continue;
			}

			// ok, we are within the radius, add us to the incoming list
			ForceLightningDamage( self, traceEnt, dir, ent_org );
		}

	}
	else
	{//trace-line
		VectorMA( self->client->ps.origin, 2048, forward, end );
		
		trap_Trace( &tr, self->client->ps.origin, vec3_origin, vec3_origin, end, self->s.number, MASK_SHOT );
		if ( tr.entityNum == ENTITYNUM_NONE || tr.fraction == 1.0 || tr.allsolid || tr.startsolid )
		{
			return;
		}
		
		traceEnt = &g_entities[tr.entityNum];
		ForceLightningDamage( self, traceEnt, forward, tr.endpos );
	}
}

int ForceShootDrain( gentity_t *self );

void ForceDrain( gentity_t *self )
{
	if ( self->health <= 0 )
	{
		return;
	}

	if (self->client->ps.forceHandExtend != HANDEXTEND_NONE)
	{
		return;
	}

	if (self->client->ps.weaponTime > 0)
	{
		return;
	}

	if ( self->client->ps.fd.forcePower < 25 || !WP_ForcePowerUsable( self, FP_DRAIN ) )
	{
		return;
	}
	if ( self->client->ps.fd.forcePowerDebounce[FP_DRAIN] > level.time )
	{//stops it while using it and also after using it, up to 3 second delay
		return;
	}

//	self->client->ps.forceHandExtend = HANDEXTEND_FORCEPUSH;
//	self->client->ps.forceHandExtendTime = level.time + 1000;
	self->client->ps.forceHandExtend = HANDEXTEND_FORCEGRIP;
	self->client->ps.forceHandExtendTime = level.time + 20000;

	G_Sound( self, CHAN_BODY, G_SoundIndex("sound/weapons/force/drain.wav") );
	
	WP_ForcePowerStart( self, FP_DRAIN, 500 );
}

void ForceDrainDamage( gentity_t *self, gentity_t *traceEnt, vec3_t dir, vec3_t impactPoint )
{
	gentity_t *tent;

	self->client->dangerTime = level.time;
	self->client->ps.eFlags &= ~EF_INVULNERABLE;
	self->client->invulnerableTimer = 0;

	if ( traceEnt && traceEnt->takedamage )
	{
		if ( traceEnt->client && (!OnSameTeam(self, traceEnt) || g_friendlyFire.integer) && self->client->ps.fd.forceDrainTime < level.time && traceEnt->client->ps.fd.forcePower )
		{//an enemy or object
			if (ForcePowerUsableOn(self, traceEnt, FP_DRAIN))
			{
				int modPowerLevel = -1;
				int	dmg = 0; //Q_irand( 1, 3 );
				if (self->client->ps.fd.forcePowerLevel[FP_DRAIN] == FORCE_LEVEL_1)
				{
					dmg = 2; //because it's one-shot
				}
				else if (self->client->ps.fd.forcePowerLevel[FP_DRAIN] == FORCE_LEVEL_2)
				{
					dmg = 3;
				}
				else if (self->client->ps.fd.forcePowerLevel[FP_DRAIN] == FORCE_LEVEL_3)
				{
					dmg = 4;
				}
			
				if (traceEnt->client)
				{
					modPowerLevel = WP_AbsorbConversion(traceEnt, traceEnt->client->ps.fd.forcePowerLevel[FP_ABSORB], self, FP_DRAIN, self->client->ps.fd.forcePowerLevel[FP_DRAIN], 0);
					//Since this is drain, don't absorb any power, but nullify the affect it has
				}

				if (modPowerLevel != -1)
				{
					if (!modPowerLevel)
					{
						dmg = 0;
					}
					else if (modPowerLevel == 1)
					{
						dmg = 1;
					}
					else if (modPowerLevel == 2)
					{
						dmg = 2;
					}
				}
				//G_Damage( traceEnt, self, self, dir, impactPoint, dmg, 0, MOD_FORCE_DARK );

				if (dmg)
				{
					traceEnt->client->ps.fd.forcePower -= dmg;
				}
				if (traceEnt->client->ps.fd.forcePower < 0)
				{
					traceEnt->client->ps.fd.forcePower = 0;
				}

				if (self->client->ps.stats[STAT_HEALTH] < self->client->ps.stats[STAT_MAX_HEALTH] &&
					self->health > 0 && self->client->ps.stats[STAT_HEALTH] > 0)
				{
					self->health += dmg;
					if (self->health > self->client->ps.stats[STAT_MAX_HEALTH])
					{
						self->health = self->client->ps.stats[STAT_MAX_HEALTH];
					}
					self->client->ps.stats[STAT_HEALTH] = self->health;
				}

				traceEnt->client->ps.fd.forcePowerRegenDebounceTime = level.time + 800; //don't let the client being drained get force power back right away

				//Drain the standard amount since we just drained someone else

				/*
				if (self->client->ps.fd.forcePowerLevel[FP_DRAIN] == FORCE_LEVEL_1)
				{
					BG_ForcePowerDrain( &self->client->ps, FP_DRAIN, 0 );
				}
				else
				{
					BG_ForcePowerDrain( &self->client->ps, FP_DRAIN, forcePowerNeeded[self->client->ps.fd.forcePowerLevel[FP_DRAIN]][FP_DRAIN]/5 );
				}

				if (self->client->ps.fd.forcePowerLevel[FP_DRAIN] == FORCE_LEVEL_1)
				{
					self->client->ps.fd.forceDrainTime = level.time + 100;
				}
				else
				{
					self->client->ps.fd.forceDrainTime = level.time + 20;
				}
				*/

				if ( traceEnt->client )
				{
					if ( !Q_irand( 0, 2 ) )
					{
						//G_Sound( traceEnt, CHAN_BODY, G_SoundIndex( "sound/weapons/force/lightninghit.wav" ) );
					}
				//	traceEnt->s.powerups |= ( 1 << PW_DISINT_1 );

				//	traceEnt->client->ps.powerups[PW_DISINT_1] = level.time + 500;
				}

				tent = G_TempEntity( impactPoint, EV_FORCE_DRAINED);
				tent->s.eventParm = DirToByte(dir);
				tent->s.owner = traceEnt->s.number;
			}
		}
	}
}

int ForceShootDrain( gentity_t *self )
{
	trace_t	tr;
	vec3_t	end, forward;
	gentity_t	*traceEnt;
	int			gotOneOrMore = 0;

	if ( self->health <= 0 )
	{
		return 0;
	}
	AngleVectors( self->client->ps.viewangles, forward, NULL, NULL );
	VectorNormalize( forward );

	if ( self->client->ps.fd.forcePowerLevel[FP_DRAIN] > FORCE_LEVEL_2 )
	{//arc
		vec3_t	center, mins, maxs, dir, ent_org, size, v;
		float	radius = MAX_DRAIN_DISTANCE, dot, dist;
		gentity_t	*entityList[MAX_GENTITIES];
		int			iEntityList[MAX_GENTITIES];
		int		e, numListedEntities, i;

		VectorCopy( self->client->ps.origin, center );
		for ( i = 0 ; i < 3 ; i++ ) 
		{
			mins[i] = center[i] - radius;
			maxs[i] = center[i] + radius;
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
			traceEnt = entityList[e];

			if ( !traceEnt )
				continue;
			if ( traceEnt == self )
				continue;
			if ( !traceEnt->inuse )
				continue;
			if ( !traceEnt->takedamage )
				continue;
			if ( traceEnt->health <= 0 )//no torturing corpses
				continue;
			if ( !traceEnt->client )
				continue;
			if ( !traceEnt->client->ps.fd.forcePower )
				continue;
//			if (traceEnt->client->ps.fd.forceSide == FORCE_DARKSIDE)	// We no longer care if the victim is dark or light
//				continue;
			if (OnSameTeam(self, traceEnt))
				continue;
			//this is all to see if we need to start a saber attack, if it's in flight, this doesn't matter
			// find the distance from the edge of the bounding box
			for ( i = 0 ; i < 3 ; i++ ) 
			{
				if ( center[i] < traceEnt->r.absmin[i] ) 
				{
					v[i] = traceEnt->r.absmin[i] - center[i];
				} else if ( center[i] > traceEnt->r.absmax[i] ) 
				{
					v[i] = center[i] - traceEnt->r.absmax[i];
				} else 
				{
					v[i] = 0;
				}
			}

			VectorSubtract( traceEnt->r.absmax, traceEnt->r.absmin, size );
			VectorMA( traceEnt->r.absmin, 0.5, size, ent_org );

			//see if they're in front of me
			//must be within the forward cone
			VectorSubtract( ent_org, center, dir );
			VectorNormalize( dir );
			if ( (dot = DotProduct( dir, forward )) < 0.5 )
				continue;

			//must be close enough
			dist = VectorLength( v );
			if ( dist >= radius ) 
			{
				continue;
			}
		
			//in PVS?
			if ( !traceEnt->r.bmodel && !trap_InPVS( ent_org, /*self->client->renderInfo.handLPoint*/self->client->ps.origin ) )
			{//must be in PVS
				continue;
			}

			//Now check and see if we can actually hit it
			trap_Trace( &tr, self->client->ps.origin, vec3_origin, vec3_origin, ent_org, self->s.number, MASK_SHOT );
			if ( tr.fraction < 1.0f && tr.entityNum != traceEnt->s.number )
			{//must have clear LOS
				continue;
			}

			// ok, we are within the radius, add us to the incoming list
			ForceDrainDamage( self, traceEnt, dir, ent_org );
			gotOneOrMore = 1;
		}

	}
	else
	{//trace-line
		VectorMA( self->client->ps.origin, 2048, forward, end );
		
		trap_Trace( &tr, self->client->ps.origin, vec3_origin, vec3_origin, end, self->s.number, MASK_SHOT );
		if ( tr.entityNum == ENTITYNUM_NONE || tr.fraction == 1.0 || tr.allsolid || tr.startsolid || !g_entities[tr.entityNum].client || !g_entities[tr.entityNum].inuse )
		{
			return 0;
		}
		
		traceEnt = &g_entities[tr.entityNum];
		ForceDrainDamage( self, traceEnt, forward, tr.endpos );
		gotOneOrMore = 1;
	}

	self->client->ps.activeForcePass = self->client->ps.fd.forcePowerLevel[FP_DRAIN] + FORCE_LEVEL_3;

/*	if (self->client->ps.fd.forcePowerLevel[FP_DRAIN] == FORCE_LEVEL_1)
	{
		BG_ForcePowerDrain( &self->client->ps, FP_DRAIN, 0 );
	}
	else*/
	{
		BG_ForcePowerDrain( &self->client->ps, FP_DRAIN, 1 );
	}

	self->client->ps.fd.forcePowerRegenDebounceTime = level.time + 500;

	return gotOneOrMore;
}

void ForceJumpCharge( gentity_t *self, usercmd_t *ucmd )
{
	float forceJumpChargeInterval = forceJumpStrength[0] / (FORCE_JUMP_CHARGE_TIME/FRAMETIME);

	if ( self->health <= 0 )
	{
		return;
	}

	if (!self->client->ps.fd.forceJumpCharge && self->client->ps.groundEntityNum == ENTITYNUM_NONE)
	{
		return;
	}

	if (self->client->ps.fd.forcePower < forcePowerNeeded[self->client->ps.fd.forcePowerLevel[FP_LEVITATION]][FP_LEVITATION])
	{
		G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_1-50], CHAN_VOICE);
		return;
	}

	if (!self->client->ps.fd.forceJumpCharge)
	{
		self->client->ps.fd.forceJumpAddTime = 0;
	}

	if (self->client->ps.fd.forceJumpAddTime >= level.time)
	{
		return;
	}

	//need to play sound
	if ( !self->client->ps.fd.forceJumpCharge )
	{
		G_Sound( self, TRACK_CHANNEL_1, G_SoundIndex("sound/weapons/force/jumpbuild.wav") );
	}

	//Increment
	if (self->client->ps.fd.forceJumpAddTime < level.time)
	{
		self->client->ps.fd.forceJumpCharge += forceJumpChargeInterval*50;
		self->client->ps.fd.forceJumpAddTime = level.time + 500;
	}

	//clamp to max strength for current level
	if ( self->client->ps.fd.forceJumpCharge > forceJumpStrength[self->client->ps.fd.forcePowerLevel[FP_LEVITATION]] )
	{
		self->client->ps.fd.forceJumpCharge = forceJumpStrength[self->client->ps.fd.forcePowerLevel[FP_LEVITATION]];
		G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_1-50], CHAN_VOICE);
	}

	//clamp to max available force power
	if ( self->client->ps.fd.forceJumpCharge/forceJumpChargeInterval/(FORCE_JUMP_CHARGE_TIME/FRAMETIME)*forcePowerNeeded[self->client->ps.fd.forcePowerLevel[FP_LEVITATION]][FP_LEVITATION] > self->client->ps.fd.forcePower )
	{//can't use more than you have
		G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_1-50], CHAN_VOICE);
		self->client->ps.fd.forceJumpCharge = self->client->ps.fd.forcePower*forceJumpChargeInterval/(FORCE_JUMP_CHARGE_TIME/FRAMETIME);
	}
	
	//G_Printf("%f\n", self->client->ps.fd.forceJumpCharge);
}

int WP_GetVelocityForForceJump( gentity_t *self, vec3_t jumpVel, usercmd_t *ucmd )
{
	float pushFwd = 0, pushRt = 0;
	vec3_t	view, forward, right;
	VectorCopy( self->client->ps.viewangles, view );
	view[0] = 0;
	AngleVectors( view, forward, right, NULL );
	if ( ucmd->forwardmove && ucmd->rightmove )
	{
		if ( ucmd->forwardmove > 0 )
		{
			pushFwd = 50;
		}
		else
		{
			pushFwd = -50;
		}
		if ( ucmd->rightmove > 0 )
		{
			pushRt = 50;
		}
		else
		{
			pushRt = -50;
		}
	}
	else if ( ucmd->forwardmove || ucmd->rightmove )
	{
		if ( ucmd->forwardmove > 0 )
		{
			pushFwd = 100;
		}
		else if ( ucmd->forwardmove < 0 )
		{
			pushFwd = -100;
		}
		else if ( ucmd->rightmove > 0 )
		{
			pushRt = 100;
		}
		else if ( ucmd->rightmove < 0 )
		{
			pushRt = -100;
		}
	}

	G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_1-50], CHAN_VOICE);

	G_PreDefSound(self->client->ps.origin, PDSOUND_FORCEJUMP);

	if (self->client->ps.fd.forceJumpCharge < JUMP_VELOCITY+40)
	{ //give him at least a tiny boost from just a tap
		self->client->ps.fd.forceJumpCharge = JUMP_VELOCITY+400;
	}

	if (self->client->ps.velocity[2] < -30)
	{ //so that we can get a good boost when force jumping in a fall
		self->client->ps.velocity[2] = -30;
	}

	VectorMA( self->client->ps.velocity, pushFwd, forward, jumpVel );
	VectorMA( self->client->ps.velocity, pushRt, right, jumpVel );
	jumpVel[2] += self->client->ps.fd.forceJumpCharge;//forceJumpStrength;
	if ( pushFwd > 0 && self->client->ps.fd.forceJumpCharge > 200 )
	{
		return FJ_FORWARD;
	}
	else if ( pushFwd < 0 && self->client->ps.fd.forceJumpCharge > 200 )
	{
		return FJ_BACKWARD;
	}
	else if ( pushRt > 0 && self->client->ps.fd.forceJumpCharge > 200 )
	{
		return FJ_RIGHT;
	}
	else if ( pushRt < 0 && self->client->ps.fd.forceJumpCharge > 200 )
	{
		return FJ_LEFT;
	}
	else
	{//FIXME: jump straight up anim
		return FJ_UP;
	}
}

void ForceJump( gentity_t *self, usercmd_t *ucmd )
{
	float forceJumpChargeInterval;
//	int anim;
	vec3_t	jumpVel;
//	int	parts = SETANIM_BOTH;

	if ( self->client->ps.fd.forcePowerDuration[FP_LEVITATION] > level.time )
	{
		return;
	}
	if ( !WP_ForcePowerUsable( self, FP_LEVITATION ) )
	{
		return;
	}
	if ( self->s.groundEntityNum == ENTITYNUM_NONE )
	{
		return;
	}
//	if ( self->client->ps.pm_flags&PMF_JUMP_HELD )
//	{
//		return;
//	}
	if ( self->health <= 0 )
	{
		return;
	}

	self->client->fjDidJump = qtrue;

	forceJumpChargeInterval = forceJumpStrength[self->client->ps.fd.forcePowerLevel[FP_LEVITATION]]/(FORCE_JUMP_CHARGE_TIME/FRAMETIME);

	switch( WP_GetVelocityForForceJump( self, jumpVel, ucmd ) )
	{
	case FJ_FORWARD:
//		anim = BOTH_FLIP_F;
		//dmEvent = DM_FLIP;
		break;
	case FJ_BACKWARD:
//		anim = BOTH_FLIP_B;
		//dmEvent = DM_FLIP;
		break;
	case FJ_RIGHT:
//		anim = BOTH_FLIP_R;
		//dmEvent = DM_FLIP;
		break;
	case FJ_LEFT:
//		anim = BOTH_FLIP_L;
		//dmEvent = DM_FLIP;
		break;
	default:
	case FJ_UP:
//		anim = BOTH_JUMP1;
		//dmEvent = DM_JUMP;
		break;
	}

	if ( self->client->ps.weaponTime )
	{//FIXME: really only care if we're in a saber attack anim.. maybe trail length?
//		parts = SETANIM_LEGS;
	}

	//NPC_SetAnim( self, parts, anim, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
	if ( !self->s.number )
	{
		//G_DynaMixEvent( dmEvent );
	}

	//FIXME: sound effect
	self->client->ps.fd.forceJumpZStart = self->client->ps.origin[2];//remember this for when we land
	VectorCopy( jumpVel, self->client->ps.velocity );
	//wasn't allowing them to attack when jumping, but that was annoying
	//self->client->ps.weaponTime = self->client->ps.torsoAnimTimer;

	WP_ForcePowerStart( self, FP_LEVITATION, self->client->ps.fd.forceJumpCharge/forceJumpChargeInterval/(FORCE_JUMP_CHARGE_TIME/FRAMETIME)*forcePowerNeeded[self->client->ps.fd.forcePowerLevel[FP_LEVITATION]][FP_LEVITATION] );
	//self->client->ps.fd.forcePowerDuration[FP_LEVITATION] = level.time + self->client->ps.weaponTime;
	self->client->ps.fd.forceJumpCharge = 0;
	self->client->ps.forceJumpFlip = qtrue;
}

void WP_AddAsMindtricked(forcedata_t *fd, int entNum)
{
	if (!fd)
	{
		return;
	}

	if (entNum > 47)
	{
		fd->forceMindtrickTargetIndex4 |= (1 << (entNum-48));
	}
	else if (entNum > 31)
	{
		fd->forceMindtrickTargetIndex3 |= (1 << (entNum-32));
	}
	else if (entNum > 15)
	{
		fd->forceMindtrickTargetIndex2 |= (1 << (entNum-16));
	}
	else
	{
		fd->forceMindtrickTargetIndex |= (1 << entNum);
	}
}

void ForceTelepathy(gentity_t *self)
{
	trace_t tr;
	vec3_t tfrom, tto, fwd, thispush_org, a;
	vec3_t mins, maxs, fwdangles, forward, right, center;
	int i, e;
	int entityList[MAX_GENTITIES];
	int numListedEntities;
	int gotatleastone;
	float visionArc;
	float radius;
	gentity_t *ent;

	visionArc = 0;

	radius = MAX_TRICK_DISTANCE;

	gotatleastone = 0;

	if ( self->health <= 0 )
	{
		return;
	}

	if (self->client->ps.forceHandExtend != HANDEXTEND_NONE)
	{
		return;
	}

	if (self->client->ps.weaponTime > 0)
	{
		return;
	}

	if (self->client->ps.powerups[PW_REDFLAG] ||
		self->client->ps.powerups[PW_BLUEFLAG])
	{ //can't mindtrick while carrying the flag
		return;
	}

	if (self->client->ps.forceAllowDeactivateTime < level.time &&
		(self->client->ps.fd.forcePowersActive & (1 << FP_TELEPATHY)) )
	{
		WP_ForcePowerStop( self, FP_TELEPATHY );
		return;
	}

/*	if ( WP_ForcePowerInUse(self, FP_TELEPATHY))
	{
		WP_ForcePowerStop(self, FP_TELEPATHY);
		return;
	}*/

	if ( !WP_ForcePowerUsable( self, FP_TELEPATHY ) )
	{
		return;
	}

	if (self->client->ps.fd.forcePowerLevel[FP_TELEPATHY] == FORCE_LEVEL_2)
	{
		visionArc = 360;//180;
	}
	else if (self->client->ps.fd.forcePowerLevel[FP_TELEPATHY] == FORCE_LEVEL_3)
	{
		visionArc = 360;
	}

	VectorCopy( self->client->ps.viewangles, fwdangles );
	AngleVectors( fwdangles, forward, right, NULL );
	VectorCopy( self->client->ps.origin, center );

	for ( i = 0 ; i < 3 ; i++ ) 
	{
		mins[i] = center[i] - radius;
		maxs[i] = center[i] + radius;
	}

	if (self->client->ps.fd.forcePowerLevel[FP_TELEPATHY] == FORCE_LEVEL_1)
	{
		VectorCopy(self->client->ps.origin, tfrom);
		tfrom[2] += self->client->ps.viewheight;
		AngleVectors(self->client->ps.viewangles, fwd, NULL, NULL);
		tto[0] = tfrom[0] + fwd[0]*radius/2;
		tto[1] = tfrom[1] + fwd[1]*radius/2;
		tto[2] = tfrom[2] + fwd[2]*radius/2;

		trap_Trace(&tr, tfrom, NULL, NULL, tto, self->s.number, MASK_PLAYERSOLID);

		if (tr.fraction != 1.0 &&
			tr.entityNum != ENTITYNUM_NONE &&
			g_entities[tr.entityNum].inuse &&
			g_entities[tr.entityNum].client &&
			g_entities[tr.entityNum].client->pers.connected &&
			g_entities[tr.entityNum].client->sess.sessionTeam != TEAM_SPECTATOR)
		{
			WP_AddAsMindtricked(&self->client->ps.fd, tr.entityNum);
			//self->client->ps.fd.forceMindtrickTargetIndex |= (1 << tr.entityNum);
			WP_ForcePowerStart( self, FP_TELEPATHY, 0 );

			G_Sound( self, CHAN_AUTO, G_SoundIndex("sound/weapons/force/distract.wav") );

			self->client->ps.forceHandExtend = HANDEXTEND_FORCEPUSH;
			self->client->ps.forceHandExtendTime = level.time + 1000;

			return;
		}
		else
		{
			//self->client->ps.fd.forceMindtrickTargetIndex = ENTITYNUM_NONE;
			return;
		}
	}
	else if (self->client->ps.fd.forcePowerLevel[FP_TELEPATHY] == FORCE_LEVEL_3)
	{ //Level 3 tricks everyone on the level..
		gentity_t *ent;
		qboolean gotAtLeastOne = qfalse;

		e = 0;

		while (e < MAX_CLIENTS)
		{
			ent = &g_entities[e];
			e++;

			if (ent && ent->inuse && ent->client)
			{
				if (!ForcePowerUsableOn(self, ent, FP_TELEPATHY))
				{
					continue;
				}
				
				if (OnSameTeam(self, ent))
				{
					continue;
				}

				if (self == ent)
				{
					continue;
				}

				if (self->s.number == ent->s.number)
				{
					continue;
				}

				if (!ent->client->pers.connected)
				{
					continue;
				}

				if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
				{
					continue;
				}

				if (!trap_InPVS(self->client->ps.origin, ent->client->ps.origin))
				{
					continue;
				}

				WP_AddAsMindtricked(&self->client->ps.fd, ent->s.number);
				gotAtLeastOne = qtrue;
			}
		}

		if (gotAtLeastOne)
		{
			self->client->ps.forceAllowDeactivateTime = level.time + 1500;

			WP_ForcePowerStart( self, FP_TELEPATHY, 0 );

			G_Sound( self, CHAN_AUTO, G_SoundIndex("sound/weapons/force/distract.wav") );

			self->client->ps.forceHandExtend = HANDEXTEND_FORCEPUSH;
			self->client->ps.forceHandExtendTime = level.time + 1000;
		}

		return;
	}
	else
	{
		numListedEntities = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );

		e = 0;

		while (e < numListedEntities)
		{
			ent = &g_entities[entityList[e]];

			if (ent)
			{
				if (ent->client)
				{
					VectorCopy(ent->client->ps.origin, thispush_org);
				}
				else
				{
					VectorCopy(ent->s.pos.trBase, thispush_org);
				}
			}

			if (ent)
			{ //not in the arc, don't consider it
				VectorCopy(self->client->ps.origin, tto);
				tto[2] += self->client->ps.viewheight;
				VectorSubtract(thispush_org, tto, a);
				vectoangles(a, a);

				if (!ent->client)
				{
					entityList[e] = ENTITYNUM_NONE;
				}
				else if (!InFieldOfVision(self->client->ps.viewangles, visionArc, a))
				{ //only bother with arc rules if the victim is a client
					entityList[e] = ENTITYNUM_NONE;
				}
				else if (!ForcePowerUsableOn(self, ent, FP_TELEPATHY))
				{
					entityList[e] = ENTITYNUM_NONE;
				}
				else if (OnSameTeam(self, ent))
				{
					entityList[e] = ENTITYNUM_NONE;
				}
			}
			e++;
		}
	}

	e = 0;

	while (e < numListedEntities)
	{
		ent = &g_entities[entityList[e]];

		if (ent && ent != self && ent->client)
		{
			gotatleastone = 1;
			WP_AddAsMindtricked(&self->client->ps.fd, ent->s.number);
			//self->client->ps.fd.forceMindtrickTargetIndex |= (1 << ent->s.number);
		}

		e++;
	}

	if (gotatleastone)
	{
		self->client->ps.forceAllowDeactivateTime = level.time + 1500;

		WP_ForcePowerStart( self, FP_TELEPATHY, 0 );

		G_Sound( self, CHAN_AUTO, G_SoundIndex("sound/weapons/force/distract.wav") );

		self->client->ps.forceHandExtend = HANDEXTEND_FORCEPUSH;
		self->client->ps.forceHandExtendTime = level.time + 1000;
	}
}

void GEntity_UseFunc( gentity_t *self, gentity_t *other, gentity_t *activator )
{
	self->use(self, other, activator);
}

qboolean CanCounterThrow(gentity_t *self, qboolean pull)
{
	int powerUse = 0;

	if (self->client->ps.forceHandExtend != HANDEXTEND_NONE)
	{
		return 0;
	}

	if (self->client->ps.weaponTime > 0)
	{
		return 0;
	}

	if ( self->health <= 0 )
	{
		return 0;
	}
	if ( self->client->ps.powerups[PW_DISINT_4] > level.time )
	{
		return 0;
	}
	if (pull)
	{
		powerUse = FP_PULL;
	}
	else
	{
		powerUse = FP_PUSH;
	}

	if ( !WP_ForcePowerUsable( self, powerUse ) )
	{
		return 0;
	}

	if (self->client->ps.groundEntityNum == ENTITYNUM_NONE)
	{ //you cannot counter a push/pull if you're in the air
		return 0;
	}

	return 1;
}

void ForceThrow( gentity_t *self, qboolean pull )
{
	//shove things in front of you away
	float		dist;
	gentity_t	*ent;
	int			entityList[MAX_GENTITIES];
	gentity_t	*push_list[MAX_GENTITIES];
	int			numListedEntities;
	vec3_t		mins, maxs;
	vec3_t		v;
	int			i, e;
	int			ent_count = 0;
	int			radius = 1024; //since it's view-based now. //350;
	int			powerLevel;
	int			visionArc;
	int			pushPower;
	int			pushPowerMod;
	vec3_t		center, ent_org, size, forward, right, end, dir, fwdangles = {0};
	float		dot1;
	trace_t		tr;
	int			x;
	vec3_t		pushDir;
	vec3_t		thispush_org;
	vec3_t		tfrom, tto, fwd, a;
	float		knockback = pull?0:200;
	int			powerUse = 0;

	visionArc = 0;

	if (self->client->ps.forceHandExtend != HANDEXTEND_NONE)
	{
		return;
	}

	if (self->client->ps.weaponTime > 0)
	{
		return;
	}

	if ( self->health <= 0 )
	{
		return;
	}
	if ( self->client->ps.powerups[PW_DISINT_4] > level.time )
	{
		return;
	}
	if (pull)
	{
		powerUse = FP_PULL;
	}
	else
	{
		powerUse = FP_PUSH;
	}

	if ( !WP_ForcePowerUsable( self, powerUse ) )
	{
		return;
	}

	if (!pull && self->client->ps.saberLockTime > level.time && self->client->ps.saberLockFrame)
	{
		G_Sound( self, CHAN_BODY, G_SoundIndex( "sound/weapons/force/push.wav" ) );
		//self->client->ps.forceHandExtend = HANDEXTEND_FORCEPUSH;
		//self->client->ps.forceHandExtendTime = level.time + 1000;
		self->client->ps.powerups[PW_DISINT_4] = level.time + 1500;

		self->client->ps.saberLockHits += self->client->ps.fd.forcePowerLevel[FP_PUSH]*2;

		WP_ForcePowerStart( self, FP_PUSH, 0 );
		return;
	}

	WP_ForcePowerStart( self, powerUse, 0 );

	//make sure this plays and that you cannot press fire for about 1 second after this
	if ( pull )
	{
		G_Sound( self, CHAN_BODY, G_SoundIndex( "sound/weapons/force/pull.wav" ) );
		self->client->ps.forceHandExtend = HANDEXTEND_FORCEPULL;
		self->client->ps.forceHandExtendTime = level.time + 400;
	}
	else
	{
		G_Sound( self, CHAN_BODY, G_SoundIndex( "sound/weapons/force/push.wav" ) );
		self->client->ps.forceHandExtend = HANDEXTEND_FORCEPUSH;
		self->client->ps.forceHandExtendTime = level.time + 1000;
	}
	self->client->ps.powerups[PW_DISINT_4] = self->client->ps.forceHandExtendTime + 200;

	VectorCopy( self->client->ps.viewangles, fwdangles );
	AngleVectors( fwdangles, forward, right, NULL );
	VectorCopy( self->client->ps.origin, center );

	for ( i = 0 ; i < 3 ; i++ ) 
	{
		mins[i] = center[i] - radius;
		maxs[i] = center[i] + radius;
	}


	if (pull)
	{
		powerLevel = self->client->ps.fd.forcePowerLevel[FP_PULL];
		pushPower = 256*self->client->ps.fd.forcePowerLevel[FP_PULL];
	}
	else
	{
		powerLevel = self->client->ps.fd.forcePowerLevel[FP_PUSH];
		pushPower = 256*self->client->ps.fd.forcePowerLevel[FP_PUSH];
	}

	if (!powerLevel)
	{ //Shouldn't have made it here..
		return;
	}

	if (powerLevel == FORCE_LEVEL_2)
	{
		visionArc = 60;
	}
	else if (powerLevel == FORCE_LEVEL_3)
	{
		visionArc = 180;
	}

	if (powerLevel == FORCE_LEVEL_1)
	{ //can only push/pull targeted things at level 1
		VectorCopy(self->client->ps.origin, tfrom);
		tfrom[2] += self->client->ps.viewheight;
		AngleVectors(self->client->ps.viewangles, fwd, NULL, NULL);
		tto[0] = tfrom[0] + fwd[0]*radius/2;
		tto[1] = tfrom[1] + fwd[1]*radius/2;
		tto[2] = tfrom[2] + fwd[2]*radius/2;

		trap_Trace(&tr, tfrom, NULL, NULL, tto, self->s.number, MASK_PLAYERSOLID);

		if (tr.fraction != 1.0 &&
			tr.entityNum != ENTITYNUM_NONE)
		{
			numListedEntities = 0;
			entityList[numListedEntities] = tr.entityNum;
			numListedEntities++;
		}
		else
		{
			//didn't get anything, so just
			return;
		}
	}
	else
	{
		numListedEntities = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );

		e = 0;

		while (e < numListedEntities)
		{
			ent = &g_entities[entityList[e]];

			if (ent)
			{
				if (ent->client)
				{
					VectorCopy(ent->client->ps.origin, thispush_org);
				}
				else
				{
					VectorCopy(ent->s.pos.trBase, thispush_org);
				}
			}

			//if (ent && !InFront(thispush_org, self->client->ps.origin, self->client->ps.viewangles, visionArc))
			if (ent)
			{ //not in the arc, don't consider it
				VectorCopy(self->client->ps.origin, tto);
				tto[2] += self->client->ps.viewheight;
				VectorSubtract(thispush_org, tto, a);
				vectoangles(a, a);

				if (ent->client && !InFieldOfVision(self->client->ps.viewangles, visionArc, a) &&
					ForcePowerUsableOn(self, ent, powerUse))
				{ //only bother with arc rules if the victim is a client
					entityList[e] = ENTITYNUM_NONE;
				}
				else if (ent->client)
				{
					if (pull)
					{
						if (!ForcePowerUsableOn(self, ent, FP_PULL))
						{
							entityList[e] = ENTITYNUM_NONE;
						}
					}
					else
					{
						if (!ForcePowerUsableOn(self, ent, FP_PUSH))
						{
							entityList[e] = ENTITYNUM_NONE;
						}
					}
				}
			}
			e++;
		}
	}

	for ( e = 0 ; e < numListedEntities ; e++ ) 
	{
		//ent = entityList[ e ];
		if (entityList[e] != ENTITYNUM_NONE &&
			entityList[e] >= 0 &&
			entityList[e] < MAX_GENTITIES)
		{
			ent = &g_entities[entityList[e]];
		}
		else
		{
			ent = NULL;
		}

		if (!ent)
			continue;
		if (ent == self)
			continue;
		if (ent->client && OnSameTeam(ent, self))
		{
			continue;
		}
//		if ( ent->owner == self && ent->s.weapon != WP_THERMAL )//can push your own thermals
//			continue;
		if ( !(ent->inuse) )
			continue;
		if ( ent->s.eType != ET_MISSILE )
		{
			if ( ent->s.eType != ET_ITEM /*&& ent->e_ThinkFunc != thinkF_G_RunObject*/ )//|| !(ent->flags&FL_DROPPED_ITEM) )//was only dropped items
			{
				//FIXME: need pushable objects
				if ( ent->s.eFlags & EF_NODRAW )
				{
					continue;
				}
				if ( !ent->client )
				{
					if ( Q_stricmp( "lightsaber", ent->classname ) != 0 )
					{//not a lightsaber 
//						if ( !(ent->svFlags&SVF_GLASS_BRUSH) )
//						{//and not glass
							if ( Q_stricmp( "func_door", ent->classname ) != 0 || !(ent->spawnflags & 2/*MOVER_FORCE_ACTIVATE*/) )
							{//not a force-usable door
								if ( Q_stricmp( "limb", ent->classname ) )
								{//not a limb
									continue;
								}
							}
							else if ( ent->moverState != MOVER_POS1 && ent->moverState != MOVER_POS2 )
							{//not at rest
								continue;
							}
//						}
					}
					//continue;
				}
			}
		}
		else
		{
			if ( ent->s.pos.trType == TR_STATIONARY && (ent->s.eFlags&EF_MISSILE_STICK) )
			{//can't force-push/pull stuck missiles (detpacks, tripmines)
				continue;
			}
			if ( ent->s.pos.trType == TR_STATIONARY && ent->s.weapon != WP_THERMAL )
			{//only thermal detonators can be pushed once stopped
				continue;
			}
		}

		//this is all to see if we need to start a saber attack, if it's in flight, this doesn't matter
		// find the distance from the edge of the bounding box
		for ( i = 0 ; i < 3 ; i++ ) 
		{
			if ( center[i] < ent->r.absmin[i] ) 
			{
				v[i] = ent->r.absmin[i] - center[i];
			} else if ( center[i] > ent->r.absmax[i] ) 
			{
				v[i] = center[i] - ent->r.absmax[i];
			} else 
			{
				v[i] = 0;
			}
		}

		VectorSubtract( ent->r.absmax, ent->r.absmin, size );
		VectorMA( ent->r.absmin, 0.5, size, ent_org );

		VectorSubtract( ent_org, center, dir );
		VectorNormalize( dir );
		if ( (dot1 = DotProduct( dir, forward )) < 0.6 )
			continue;

		dist = VectorLength( v );

		//Now check and see if we can actually deflect it
		//method1
		//if within a certain range, deflect it
		if ( dist >= radius ) 
		{
			continue;
		}
	
		//in PVS?
		if ( !ent->r.bmodel && !trap_InPVS( ent_org, self->client->ps.origin ) )
		{//must be in PVS
			continue;
		}

		//really should have a clear LOS to this thing...
		trap_Trace( &tr, self->client->ps.origin, vec3_origin, vec3_origin, ent_org, self->s.number, MASK_SHOT );
		if ( tr.fraction < 1.0f && tr.entityNum != ent->s.number )
		{//must have clear LOS
			continue;
		}

		// ok, we are within the radius, add us to the incoming list
		push_list[ent_count] = ent;
		ent_count++;
	}

	if ( ent_count )
	{
		//vec3_t	fx_dir;
		// we are done, do we have any to deflect?
		//method1:
		for ( x = 0; x < ent_count; x++ )
		{
			int modPowerLevel = powerLevel;

	
			if (push_list[x]->client)
			{
				modPowerLevel = WP_AbsorbConversion(push_list[x], push_list[x]->client->ps.fd.forcePowerLevel[FP_ABSORB], self, powerUse, powerLevel, forcePowerNeeded[self->client->ps.fd.forcePowerLevel[powerUse]][powerUse]);
				if (modPowerLevel == -1)
				{
					modPowerLevel = powerLevel;
				}
			}

			pushPower = 256*modPowerLevel;

			if (push_list[x]->client)
			{
				VectorCopy(push_list[x]->client->ps.origin, thispush_org);
			}
			else
			{
				VectorCopy(push_list[x]->s.origin, thispush_org);
			}

			if ( push_list[x]->client )
			{//FIXME: make enemy jedi able to hunker down and resist this?
				int otherPushPower = push_list[x]->client->ps.fd.forcePowerLevel[powerUse];
				qboolean canPullWeapon = qtrue;
				float dirLen = 0;

				knockback = pull?0:200;

				pushPowerMod = pushPower;

				if (push_list[x]->client->pers.cmd.forwardmove ||
					push_list[x]->client->pers.cmd.rightmove)
				{ //if you are moving, you get one less level of defense
					otherPushPower--;

					if (otherPushPower < 0)
					{
						otherPushPower = 0;
					}
				}

				if (otherPushPower && CanCounterThrow(push_list[x], pull))
				{
					if ( pull )
					{
						G_Sound( push_list[x], CHAN_BODY, G_SoundIndex( "sound/weapons/force/pull.wav" ) );
						push_list[x]->client->ps.forceHandExtend = HANDEXTEND_FORCEPULL;
						push_list[x]->client->ps.forceHandExtendTime = level.time + 400;
					}
					else
					{
						G_Sound( push_list[x], CHAN_BODY, G_SoundIndex( "sound/weapons/force/push.wav" ) );
						push_list[x]->client->ps.forceHandExtend = HANDEXTEND_FORCEPUSH;
						push_list[x]->client->ps.forceHandExtendTime = level.time + 1000;
					}
					push_list[x]->client->ps.powerups[PW_DISINT_4] = push_list[x]->client->ps.forceHandExtendTime + 200;

					//Make a counter-throw effect

					if (otherPushPower >= modPowerLevel)
					{
						pushPowerMod = 0;
						canPullWeapon = qfalse;
					}
					else
					{
						int powerDif = (modPowerLevel - otherPushPower);

						if (powerDif >= 3)
						{
							pushPowerMod -= pushPowerMod*0.2;
						}
						else if (powerDif == 2)
						{
							pushPowerMod -= pushPowerMod*0.4;
						}
						else if (powerDif == 1)
						{
							pushPowerMod -= pushPowerMod*0.8;
						}

						if (pushPowerMod < 0)
						{
							pushPowerMod = 0;
						}
					}
				}

				//shove them
				if ( pull )
				{
					VectorSubtract( self->client->ps.origin, thispush_org, pushDir );

					if (push_list[x]->client && VectorLength(pushDir) <= 256)
					{
						int randfact = 0;

						if (modPowerLevel == FORCE_LEVEL_1)
						{
							randfact = 3;
						}
						else if (modPowerLevel == FORCE_LEVEL_2)
						{
							randfact = 7;
						}
						else if (modPowerLevel == FORCE_LEVEL_3)
						{
							randfact = 10;
						}

						if (!OnSameTeam(self, push_list[x]) && Q_irand(1, 10) <= randfact && canPullWeapon)
						{
							vec3_t uorg, vecnorm;

							VectorCopy(self->client->ps.origin, uorg);
							uorg[2] += 64;

							VectorSubtract(uorg, thispush_org, vecnorm);
							VectorNormalize(vecnorm);

							TossClientWeapon(push_list[x], vecnorm, 500);
						}
					}
				}
				else
				{
					VectorSubtract( thispush_org, self->client->ps.origin, pushDir );
				}

				if (modPowerLevel > otherPushPower && push_list[x]->client)
				{
					//int levDif = modPowerLevel - otherPushPower;

					if (/*Q_irand(1, 5) <= levDif &&*/
						modPowerLevel == FORCE_LEVEL_3 &&
						push_list[x]->client->ps.forceHandExtend != HANDEXTEND_KNOCKDOWN)
					{
						dirLen = VectorLength(pushDir);

						if (dirLen <= 128)
						{ //can only do a knockdown if fairly close
							push_list[x]->client->ps.forceHandExtend = HANDEXTEND_KNOCKDOWN;
							push_list[x]->client->ps.forceHandExtendTime = level.time + 1100;
							push_list[x]->client->ps.forceDodgeAnim = 0; //this toggles between 1 and 0, when it's 1 we should play the get up anim
							push_list[x]->client->ps.quickerGetup = qtrue;
						}
					}
				}

				if (!dirLen)
				{
					dirLen = VectorLength(pushDir);
				}

				VectorNormalize(pushDir);

				if (push_list[x]->client)
				{
					//escape a force grip if we're in one
					if (self->client->ps.fd.forceGripBeingGripped > level.time)
					{ //force the enemy to stop gripping me if I managed to push him
						if (push_list[x]->client->ps.fd.forceGripEntityNum == self->s.number)
						{
							if (modPowerLevel >= push_list[x]->client->ps.fd.forcePowerLevel[FP_GRIP])
							{ //only break the grip if our push/pull level is >= their grip level
								WP_ForcePowerStop(push_list[x], FP_GRIP);
								self->client->ps.fd.forceGripBeingGripped = 0;
								push_list[x]->client->ps.fd.forceGripUseTime = level.time + 1000; //since we just broke out of it..
							}
						}
					}

					push_list[x]->client->ps.otherKiller = self->s.number;
					push_list[x]->client->ps.otherKillerTime = level.time + 5000;
					push_list[x]->client->ps.otherKillerDebounceTime = level.time + 100;

					pushPowerMod -= (dirLen*0.7);
					if (pushPowerMod < 16)
					{
						pushPowerMod = 16;
					}

					push_list[x]->client->ps.velocity[0] = pushDir[0]*pushPowerMod;
					push_list[x]->client->ps.velocity[1] = pushDir[1]*pushPowerMod;

					if ((int)push_list[x]->client->ps.velocity[2] == 0)
					{ //if not going anywhere vertically, boost them up a bit
						push_list[x]->client->ps.velocity[2] = pushDir[2]*pushPowerMod;

						if (push_list[x]->client->ps.velocity[2] < 128)
						{
							push_list[x]->client->ps.velocity[2] = 128;
						}
					}
					else
					{
						push_list[x]->client->ps.velocity[2] = pushDir[2]*pushPowerMod;
					}
				}
			}
			else if ( push_list[x]->s.eType == ET_MISSILE && push_list[x]->s.pos.trType != TR_STATIONARY && (push_list[x]->s.pos.trType != TR_INTERPOLATE||push_list[x]->s.weapon != WP_THERMAL) )//rolling and stationary thermal detonators are dealt with below
			{
				if ( pull )
				{//deflect rather than reflect?
				}
				else 
				{
					G_ReflectMissile( self, push_list[x], forward );
					//deflect sound
					//G_Sound( push_list[x], G_SoundIndex( va("sound/weapons/blaster/reflect%d.wav", Q_irand( 1, 3 ) ) ) );
				}
			}
			else if ( !Q_stricmp( "func_door", push_list[x]->classname ) && (push_list[x]->spawnflags&2/*MOVER_FORCE_ACTIVATE*/) )
			{//push/pull the door
				vec3_t	pos1, pos2;

				AngleVectors( self->client->ps.viewangles, forward, NULL, NULL );
				VectorNormalize( forward );
				VectorMA( self->client->ps.origin, radius, forward, end );
				trap_Trace( &tr, self->client->ps.origin, vec3_origin, vec3_origin, end, self->s.number, MASK_SHOT );
				if ( tr.entityNum != push_list[x]->s.number || tr.fraction == 1.0 || tr.allsolid || tr.startsolid )
				{//must be pointing right at it
					continue;
				}

				VectorSubtract( push_list[x]->r.absmax, push_list[x]->r.absmin, size );
				VectorMA( push_list[x]->r.absmin, 0.5, size, center );
				VectorAdd( center, push_list[x]->pos1, pos1 );
				VectorAdd( center, push_list[x]->pos2, pos2 );

				if ( Distance( pos1, self->client->ps.origin ) < Distance( pos2, self->client->ps.origin ) )
				{//pos1 is closer
					if ( push_list[x]->moverState == MOVER_POS1 )
					{//at the closest pos
						if ( pull )
						{//trying to pull, but already at closest point, so screw it
							continue;
						}
					}
					else if ( push_list[x]->moverState == MOVER_POS2 )
					{//at farthest pos
						if ( !pull )
						{//trying to push, but already at farthest point, so screw it
							continue;
						}
					}
				}
				else
				{//pos2 is closer
					if ( push_list[x]->moverState == MOVER_POS1 )
					{//at the farthest pos
						if ( !pull )
						{//trying to push, but already at farthest point, so screw it
							continue;
						}
					}
					else if ( push_list[x]->moverState == MOVER_POS2 )
					{//at closest pos
						if ( pull )
						{//trying to pull, but already at closest point, so screw it
							continue;
						}
					}
				}
				GEntity_UseFunc( push_list[x], self, self );
			}
		}
	}

	//attempt to break any leftover grips
	//if we're still in a current grip that wasn't broken by the push, it will still remain
	self->client->dangerTime = level.time;
	self->client->ps.eFlags &= ~EF_INVULNERABLE;
	self->client->invulnerableTimer = 0;

	if (self->client->ps.fd.forceGripBeingGripped > level.time)
	{
		self->client->ps.fd.forceGripBeingGripped = 0;
	}
}

void WP_ForcePowerStop( gentity_t *self, forcePowers_t forcePower )
{
	int wasActive = self->client->ps.fd.forcePowersActive;

	self->client->ps.fd.forcePowersActive &= ~( 1 << forcePower );

	switch( (int)forcePower )
	{
	case FP_HEAL:
		self->client->ps.fd.forceHealAmount = 0;
		self->client->ps.fd.forceHealTime = 0;
		break;
	case FP_LEVITATION:
		break;
	case FP_SPEED:
		//G_AddEvent(self, EV_STOPLOOPINGSOUND, 0);
		if (wasActive & (1 << FP_SPEED))
		{
			G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_2-50], CHAN_VOICE);
		}
		break;
	case FP_PUSH:
		break;
	case FP_PULL:
		break;
	case FP_TELEPATHY:
		if (wasActive & (1 << FP_TELEPATHY))
		{
			G_Sound( self, CHAN_AUTO, G_SoundIndex("sound/weapons/force/distractstop.wav") );
		}
		self->client->ps.fd.forceMindtrickTargetIndex = 0;//ENTITYNUM_NONE;
		self->client->ps.fd.forceMindtrickTargetIndex2 = 0;
		self->client->ps.fd.forceMindtrickTargetIndex3 = 0;
		self->client->ps.fd.forceMindtrickTargetIndex4 = 0;
		break;
	case FP_SEE:
		if (wasActive & (1 << FP_SEE))
		{
			G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_5-50], CHAN_VOICE);
		}
		break;
	case FP_GRIP:
		self->client->ps.fd.forceGripUseTime = level.time + 3000;
		if (self->client->ps.fd.forcePowerLevel[FP_GRIP] > FORCE_LEVEL_1 &&
			g_entities[self->client->ps.fd.forceGripEntityNum].client &&
			g_entities[self->client->ps.fd.forceGripEntityNum].health > 0 &&
			g_entities[self->client->ps.fd.forceGripEntityNum].inuse &&
			(level.time - g_entities[self->client->ps.fd.forceGripEntityNum].client->ps.fd.forceGripStarted) > 500)
		{ //if we had our throat crushed in for more than half a second, gasp for air when we're let go
			if (wasActive & (1 << FP_GRIP))
			{
				G_EntitySound( &g_entities[self->client->ps.fd.forceGripEntityNum], CHAN_VOICE, G_SoundIndex("*gasp.wav") );
			}
		}

		if (g_entities[self->client->ps.fd.forceGripEntityNum].client &&
			g_entities[self->client->ps.fd.forceGripEntityNum].inuse)
		{
			
			g_entities[self->client->ps.fd.forceGripEntityNum].client->ps.forceGripChangeMovetype = PM_NORMAL;
		}

		if (self->client->ps.forceHandExtend == HANDEXTEND_FORCEGRIP)
		{
			self->client->ps.forceHandExtendTime = 0;
		}

		self->client->ps.fd.forceGripEntityNum = ENTITYNUM_NONE;

		self->client->ps.powerups[PW_DISINT_4] = 0;
		break;
	case FP_LIGHTNING:
		if ( self->client->ps.fd.forcePowerLevel[FP_LIGHTNING] < FORCE_LEVEL_2 )
		{//don't do it again for 3 seconds, minimum... FIXME: this should be automatic once regeneration is slower (normal)
			self->client->ps.fd.forcePowerDebounce[FP_LIGHTNING] = level.time + 3000;//FIXME: define?
		}
		else
		{
			self->client->ps.fd.forcePowerDebounce[FP_LIGHTNING] = level.time + 1500;//500; //500 is still too powerful
		}
		if (self->client->ps.forceHandExtend == /*HANDEXTEND_FORCEPUSH*/HANDEXTEND_FORCEGRIP)
		{
			self->client->ps.forceHandExtendTime = 0; //reset hand position
		}

		self->client->ps.activeForcePass = 0;
		break;
	case FP_RAGE:
		self->client->ps.fd.forceRageRecoveryTime = level.time + 10000;
		if (wasActive & (1 << FP_RAGE))
		{
			G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_3-50], CHAN_VOICE);
		}
		break;
	case FP_ABSORB:
		if (wasActive & (1 << FP_ABSORB))
		{
			G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_3-50], CHAN_VOICE);
		}
		break;
	case FP_PROTECT:
		if (wasActive & (1 << FP_PROTECT))
		{
			G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_3-50], CHAN_VOICE);
		}
		break;
	case FP_DRAIN:
		if ( self->client->ps.fd.forcePowerLevel[FP_DRAIN] < FORCE_LEVEL_2 )
		{//don't do it again for 3 seconds, minimum... FIXME: this should be automatic once regeneration is slower (normal)
			self->client->ps.fd.forcePowerDebounce[FP_DRAIN] = level.time + 3000;//FIXME: define?
		}
		else
		{
			self->client->ps.fd.forcePowerDebounce[FP_DRAIN] = level.time + 1500;//500; //500 is still too powerful
		}

		if (self->client->ps.forceHandExtend == /*HANDEXTEND_FORCEPUSH*/HANDEXTEND_FORCEGRIP)
		{
			self->client->ps.forceHandExtendTime = 0; //reset hand position
		}

		self->client->ps.activeForcePass = 0;
	default:
		break;
	}
}

void DoGripAction(gentity_t *self, forcePowers_t forcePower)
{
	gentity_t *gripEnt;
	int gripLevel = 0;
	trace_t tr;
	vec3_t a;
	vec3_t fwd, fwd_o, start_o, nvel;

	self->client->dangerTime = level.time;
	self->client->ps.eFlags &= ~EF_INVULNERABLE;
	self->client->invulnerableTimer = 0;

	gripEnt = &g_entities[self->client->ps.fd.forceGripEntityNum];

	if (!gripEnt || !gripEnt->client || !gripEnt->inuse || gripEnt->health < 1 || !ForcePowerUsableOn(self, gripEnt, FP_GRIP))
	{
		WP_ForcePowerStop(self, forcePower);
		self->client->ps.fd.forceGripEntityNum = ENTITYNUM_NONE;

		if (gripEnt && gripEnt->client && gripEnt->inuse)
		{
			gripEnt->client->ps.forceGripChangeMovetype = PM_NORMAL;
		}
		return;
	}

	VectorSubtract(gripEnt->client->ps.origin, self->client->ps.origin, a);
	
	trap_Trace(&tr, self->client->ps.origin, NULL, NULL, gripEnt->client->ps.origin, self->s.number, MASK_PLAYERSOLID);

	gripLevel = WP_AbsorbConversion(gripEnt, gripEnt->client->ps.fd.forcePowerLevel[FP_ABSORB], self, FP_GRIP, self->client->ps.fd.forcePowerLevel[FP_GRIP], forcePowerNeeded[self->client->ps.fd.forcePowerLevel[FP_GRIP]][FP_GRIP]);

	if (gripLevel == -1)
	{
		gripLevel = self->client->ps.fd.forcePowerLevel[FP_GRIP];
	}

	if (!gripLevel)
	{
		WP_ForcePowerStop(self, forcePower);
		return;
	}

	if (VectorLength(a) > MAX_GRIP_DISTANCE)
	{
		WP_ForcePowerStop(self, forcePower);
		return;
	}

	if ( !InFront( gripEnt->client->ps.origin, self->client->ps.origin, self->client->ps.viewangles, 0.9f ) &&
		gripLevel < FORCE_LEVEL_3)
	{
		WP_ForcePowerStop(self, forcePower);
		return;
	}

	if (tr.fraction != 1 &&
		tr.entityNum != gripEnt->s.number &&
		gripLevel < FORCE_LEVEL_3)
	{
		WP_ForcePowerStop(self, forcePower);
		return;
	}

	if (self->client->ps.fd.forcePowerDebounce[FP_GRIP] < level.time)
	{ //2 damage per second while choking, resulting in 10 damage total (not including The Squeeze<tm>)
		self->client->ps.fd.forcePowerDebounce[FP_GRIP] = level.time + 1000;
		G_Damage(gripEnt, self, self, NULL, NULL, 2, DAMAGE_NO_ARMOR, MOD_FORCE_DARK);
	}

	if (gripLevel == FORCE_LEVEL_1)
	{
		gripEnt->client->ps.fd.forceGripBeingGripped = level.time + 1000;
		
		if ((level.time - gripEnt->client->ps.fd.forceGripStarted) > 5000)
		{
			WP_ForcePowerStop(self, forcePower);
		}
		return;
	}

	if (gripLevel == FORCE_LEVEL_2)
	{
		gripEnt->client->ps.fd.forceGripBeingGripped = level.time + 1000;

		if (gripEnt->client->ps.forceGripMoveInterval < level.time)
		{
			gripEnt->client->ps.velocity[2] = 30;

			gripEnt->client->ps.forceGripMoveInterval = level.time + 300; //only update velocity every 300ms, so as to avoid heavy bandwidth usage
		}

		gripEnt->client->ps.otherKiller = self->s.number;
		gripEnt->client->ps.otherKillerTime = level.time + 5000;
		gripEnt->client->ps.otherKillerDebounceTime = level.time + 100;

		gripEnt->client->ps.forceGripChangeMovetype = PM_FLOAT;

		if ((level.time - gripEnt->client->ps.fd.forceGripStarted) > 3000 && !self->client->ps.fd.forceGripDamageDebounceTime)
		{ //if we managed to lift him into the air for 2 seconds, give him a crack
			self->client->ps.fd.forceGripDamageDebounceTime = 1;
			G_Damage(gripEnt, self, self, NULL, NULL, 20, DAMAGE_NO_ARMOR, MOD_FORCE_DARK);
			//NOTE: Must play custom sounds on the actual entity. Don't use G_Sound (it creates a temp entity for the sound)
			G_EntitySound( gripEnt, CHAN_VOICE, G_SoundIndex(va( "*choke%d.wav", Q_irand( 1, 3 ) )) );

			gripEnt->client->ps.forceHandExtend = HANDEXTEND_CHOKE;
			gripEnt->client->ps.forceHandExtendTime = level.time + 2000;

			if (gripEnt->client->ps.fd.forcePowersActive & (1 << FP_GRIP))
			{ //choking, so don't let him keep gripping himself
				WP_ForcePowerStop(gripEnt, FP_GRIP);
			}
		}
		else if ((level.time - gripEnt->client->ps.fd.forceGripStarted) > 4000)
		{
			WP_ForcePowerStop(self, forcePower);
		}
		return;
	}

	if (gripLevel == FORCE_LEVEL_3)
	{
		gripEnt->client->ps.fd.forceGripBeingGripped = level.time + 1000;

		gripEnt->client->ps.otherKiller = self->s.number;
		gripEnt->client->ps.otherKillerTime = level.time + 5000;
		gripEnt->client->ps.otherKillerDebounceTime = level.time + 100;

		gripEnt->client->ps.forceGripChangeMovetype = PM_FLOAT;

		if (gripEnt->client->ps.forceGripMoveInterval < level.time)
		{
			float nvLen = 0;

			VectorCopy(gripEnt->client->ps.origin, start_o);
			AngleVectors(self->client->ps.viewangles, fwd, NULL, NULL);
			fwd_o[0] = self->client->ps.origin[0] + fwd[0]*128;
			fwd_o[1] = self->client->ps.origin[1] + fwd[1]*128;
			fwd_o[2] = self->client->ps.origin[2] + fwd[2]*128;
			fwd_o[2] += 16;
			VectorSubtract(fwd_o, start_o, nvel);

			nvLen = VectorLength(nvel);

			if (nvLen < 16)
			{ //within x units of desired spot
				VectorNormalize(nvel);
				gripEnt->client->ps.velocity[0] = nvel[0]*8;
				gripEnt->client->ps.velocity[1] = nvel[1]*8;
				gripEnt->client->ps.velocity[2] = nvel[2]*8;
			}
			else if (nvLen < 64)
			{
				VectorNormalize(nvel);
				gripEnt->client->ps.velocity[0] = nvel[0]*128;
				gripEnt->client->ps.velocity[1] = nvel[1]*128;
				gripEnt->client->ps.velocity[2] = nvel[2]*128;
			}
			else if (nvLen < 128)
			{
				VectorNormalize(nvel);
				gripEnt->client->ps.velocity[0] = nvel[0]*256;
				gripEnt->client->ps.velocity[1] = nvel[1]*256;
				gripEnt->client->ps.velocity[2] = nvel[2]*256;
			}
			else if (nvLen < 200)
			{
				VectorNormalize(nvel);
				gripEnt->client->ps.velocity[0] = nvel[0]*512;
				gripEnt->client->ps.velocity[1] = nvel[1]*512;
				gripEnt->client->ps.velocity[2] = nvel[2]*512;
			}
			else
			{
				VectorNormalize(nvel);
				gripEnt->client->ps.velocity[0] = nvel[0]*700;
				gripEnt->client->ps.velocity[1] = nvel[1]*700;
				gripEnt->client->ps.velocity[2] = nvel[2]*700;
			}

			gripEnt->client->ps.forceGripMoveInterval = level.time + 300; //only update velocity every 300ms, so as to avoid heavy bandwidth usage
		}

		if ((level.time - gripEnt->client->ps.fd.forceGripStarted) > 3000 && !self->client->ps.fd.forceGripDamageDebounceTime)
		{ //if we managed to lift him into the air for 2 seconds, give him a crack
			self->client->ps.fd.forceGripDamageDebounceTime = 1;
			G_Damage(gripEnt, self, self, NULL, NULL, 40, DAMAGE_NO_ARMOR, MOD_FORCE_DARK);
			//NOTE: Must play custom sounds on the actual entity. Don't use G_Sound (it creates a temp entity for the sound)
			G_EntitySound( gripEnt, CHAN_VOICE, G_SoundIndex(va( "*choke%d.wav", Q_irand( 1, 3 ) )) );

			gripEnt->client->ps.forceHandExtend = HANDEXTEND_CHOKE;
			gripEnt->client->ps.forceHandExtendTime = level.time + 2000;

			if (gripEnt->client->ps.fd.forcePowersActive & (1 << FP_GRIP))
			{ //choking, so don't let him keep gripping himself
				WP_ForcePowerStop(gripEnt, FP_GRIP);
			}
		}
		else if ((level.time - gripEnt->client->ps.fd.forceGripStarted) > 4000)
		{
			WP_ForcePowerStop(self, forcePower);
		}
		return;
	}
}

qboolean G_IsMindTricked(forcedata_t *fd, int client)
{
	int checkIn;
	int trickIndex1, trickIndex2, trickIndex3, trickIndex4;
	int sub = 0;

	if (!fd)
	{
		return qfalse;
	}

	trickIndex1 = fd->forceMindtrickTargetIndex;
	trickIndex2 = fd->forceMindtrickTargetIndex2;
	trickIndex3 = fd->forceMindtrickTargetIndex3;
	trickIndex4 = fd->forceMindtrickTargetIndex4;

	if (client > 47)
	{
		checkIn = trickIndex4;
		sub = 48;
	}
	else if (client > 31)
	{
		checkIn = trickIndex3;
		sub = 32;
	}
	else if (client > 15)
	{
		checkIn = trickIndex2;
		sub = 16;
	}
	else
	{
		checkIn = trickIndex1;
	}

	if (checkIn & (1 << (client-sub)))
	{
		return qtrue;
	}
	
	return qfalse;
}

static void RemoveTrickedEnt(forcedata_t *fd, int client)
{
	if (!fd)
	{
		return;
	}

	if (client > 47)
	{
		fd->forceMindtrickTargetIndex4 &= ~(1 << (client-48));
	}
	else if (client > 31)
	{
		fd->forceMindtrickTargetIndex3 &= ~(1 << (client-32));
	}
	else if (client > 15)
	{
		fd->forceMindtrickTargetIndex2 &= ~(1 << (client-16));
	}
	else
	{
		fd->forceMindtrickTargetIndex &= ~(1 << client);
	}
}

extern int g_LastFrameTime;
extern int g_TimeSinceLastFrame;

static void WP_UpdateMindtrickEnts(gentity_t *self)
{
	int i = 0;

	while (i < MAX_CLIENTS)
	{
		if (G_IsMindTricked(&self->client->ps.fd, i))
		{
			gentity_t *ent = &g_entities[i];

			if ( !ent || !ent->client || !ent->inuse || ent->health < 1 ||
				(ent->client->ps.fd.forcePowersActive & (1 << FP_SEE)) )
			{
				RemoveTrickedEnt(&self->client->ps.fd, i);
			}
			else if ((level.time - self->client->dangerTime) < g_TimeSinceLastFrame*4)
			{ //Untrick this entity if the tricker (self) fires while in his fov
				if (/*InFront(self->client->ps.origin, ent->client->ps.origin, ent->client->ps.viewangles, 0.8f ) &&*/
					//NOTE: No longer has to be in fov, just in visible area of PVS
					trap_InPVS(ent->client->ps.origin, self->client->ps.origin) &&
					OrgVisible(ent->client->ps.origin, self->client->ps.origin, ent->s.number))
				{
					RemoveTrickedEnt(&self->client->ps.fd, i);
				}
			}
			else if (BG_HasYsalamiri(g_gametype.integer, &ent->client->ps))
			{
				RemoveTrickedEnt(&self->client->ps.fd, i);
			}
		}

		i++;
	}

	if (!self->client->ps.fd.forceMindtrickTargetIndex &&
		!self->client->ps.fd.forceMindtrickTargetIndex2 &&
		!self->client->ps.fd.forceMindtrickTargetIndex3 &&
		!self->client->ps.fd.forceMindtrickTargetIndex4)
	{ //everyone who we had tricked is no longer tricked, so stop the power
		WP_ForcePowerStop(self, FP_TELEPATHY);
	}
	else if (self->client->ps.powerups[PW_REDFLAG] ||
		self->client->ps.powerups[PW_BLUEFLAG])
	{
		WP_ForcePowerStop(self, FP_TELEPATHY);
	}
}

static void WP_ForcePowerRun( gentity_t *self, forcePowers_t forcePower, usercmd_t *cmd )
{
	extern usercmd_t	ucmd;

	switch( (int)forcePower )
	{
	case FP_HEAL:
		if (self->client->ps.fd.forcePowerLevel[FP_HEAL] == FORCE_LEVEL_1)
		{
			if (self->client->ps.velocity[0] || self->client->ps.velocity[1] || self->client->ps.velocity[2])
			{
				WP_ForcePowerStop( self, forcePower );
				break;
			}
		}

		if (self->health < 1 || self->client->ps.stats[STAT_HEALTH] < 1)
		{
			WP_ForcePowerStop( self, forcePower );
			break;
		}

		if (self->client->ps.fd.forceHealTime > level.time)
		{
			break;
		}
		if ( self->health > self->client->ps.stats[STAT_MAX_HEALTH])
		{ //rww - we might start out over max_health and we don't want force heal taking us down to 100 or whatever max_health is
			WP_ForcePowerStop( self, forcePower );
			break;
		}
		self->client->ps.fd.forceHealTime = level.time + 1000;
		self->health++;
		self->client->ps.fd.forceHealAmount++;
		//BG_ForcePowerDrain( &self->client->ps, forcePower, 0 );
		if ( self->health > self->client->ps.stats[STAT_MAX_HEALTH])	// Past max health
		{
			self->health = self->client->ps.stats[STAT_MAX_HEALTH];
			WP_ForcePowerStop( self, forcePower );
		}

		if ( (self->client->ps.fd.forcePowerLevel[FP_HEAL] == FORCE_LEVEL_1 && self->client->ps.fd.forceHealAmount >= 25) ||
			(self->client->ps.fd.forcePowerLevel[FP_HEAL] == FORCE_LEVEL_2 && self->client->ps.fd.forceHealAmount >= 33))
		{
			WP_ForcePowerStop( self, forcePower );
		}
		break;
	case FP_SPEED:
		//This is handled in PM_WalkMove and PM_StepSlideMove
		break;
	case FP_GRIP:
		if (self->client->ps.forceHandExtend != HANDEXTEND_FORCEGRIP)
		{
			WP_ForcePowerStop(self, FP_GRIP);
			break;
		}
		DoGripAction(self, forcePower);
		break;
	case FP_LEVITATION:
		if ( self->client->ps.groundEntityNum != ENTITYNUM_NONE && !self->client->ps.fd.forceJumpZStart )
		{//done with jump
			WP_ForcePowerStop( self, forcePower );
		}
		else
		{
			/*
			if (self->client->ps.fd.forcePowerDebounce[FP_LEVITATION] < level.time)
			{
				BG_ForcePowerDrain( &self->client->ps, forcePower, 5 );
				if (self->client->ps.fd.forcePowerLevel[FP_LEVITATION] >= FORCE_LEVEL_2)
				{
					self->client->ps.fd.forcePowerDebounce[FP_LEVITATION] = level.time + 300;
				}
				else
				{
					self->client->ps.fd.forcePowerDebounce[FP_LEVITATION] = level.time + 200;
				}
			}
			*/
			//NOTE: Now handled in bg code for prediction
		}
		break;
	case FP_RAGE:
		if (self->health < 1)
		{
			WP_ForcePowerStop(self, forcePower);
			break;
		}
		if (self->client->ps.forceRageDrainTime < level.time)
		{
			int addTime = 400;

			self->health -= 2;

			if (self->client->ps.fd.forcePowerLevel[FP_RAGE] == FORCE_LEVEL_1)
			{
				addTime = 150;
			}
			else if (self->client->ps.fd.forcePowerLevel[FP_RAGE] == FORCE_LEVEL_2)
			{
				addTime = 300;
			}
			else if (self->client->ps.fd.forcePowerLevel[FP_RAGE] == FORCE_LEVEL_3)
			{
				addTime = 450;
			}
			self->client->ps.forceRageDrainTime = level.time + addTime;
		}

		if (self->health < 1)
		{
			self->health = 1;
			WP_ForcePowerStop(self, forcePower);
		}

		self->client->ps.stats[STAT_HEALTH] = self->health;
		break;
	case FP_DRAIN:
		if (self->client->ps.forceHandExtend != /*HANDEXTEND_FORCEPUSH*/HANDEXTEND_FORCEGRIP)
		{
			WP_ForcePowerStop(self, forcePower);
			break;
		}

		if ( self->client->ps.fd.forcePowerLevel[FP_DRAIN] > FORCE_LEVEL_1 )
		{//higher than level 1
			if ( (cmd->buttons & BUTTON_FORCE_DRAIN) || ((cmd->buttons & BUTTON_FORCEPOWER) && self->client->ps.fd.forcePowerSelected == FP_DRAIN) )
			{//holding it keeps it going
				self->client->ps.fd.forcePowerDuration[FP_DRAIN] = level.time + 500;
			}
		}
		if ( !WP_ForcePowerAvailable( self, forcePower ) || self->client->ps.fd.forcePowerDuration[FP_DRAIN] < level.time ||
			self->client->ps.fd.forcePower < 25)
		{
			WP_ForcePowerStop( self, forcePower );
		}
		else
		{
			ForceShootDrain( self );
		}
		break;
	case FP_LIGHTNING:
		//self->client->ps.forceHandExtendTime = level.time + 200;
		if (self->client->ps.forceHandExtend != /*HANDEXTEND_FORCEPUSH*/HANDEXTEND_FORCEGRIP)
		{ //Animation for hand extend doesn't end with hand out, so we have to limit lightning intervals by animation intervals (once hand starts to go in in animation, lightning should stop)
			WP_ForcePowerStop(self, forcePower);
			break;
		}

		if ( self->client->ps.fd.forcePowerLevel[FP_LIGHTNING] > FORCE_LEVEL_1 )
		{//higher than level 1
			if ( (cmd->buttons & BUTTON_FORCE_LIGHTNING) || ((cmd->buttons & BUTTON_FORCEPOWER) && self->client->ps.fd.forcePowerSelected == FP_LIGHTNING) )
			{//holding it keeps it going
				self->client->ps.fd.forcePowerDuration[FP_LIGHTNING] = level.time + 500;
			}
		}
		if ( !WP_ForcePowerAvailable( self, forcePower ) || self->client->ps.fd.forcePowerDuration[FP_LIGHTNING] < level.time ||
			self->client->ps.fd.forcePower < 25)
		{
			WP_ForcePowerStop( self, forcePower );
		}
		else
		{
			ForceShootLightning( self );
			BG_ForcePowerDrain( &self->client->ps, forcePower, 0 );
		}
		break;
	case FP_TELEPATHY:
		WP_UpdateMindtrickEnts(self);
		break;
	case FP_SABERATTACK:
		break;
	case FP_SABERDEFEND:
		break;
	case FP_SABERTHROW:
		break;
	case FP_PROTECT:
	case FP_ABSORB:
		if (self->client->ps.fd.forcePowerDebounce[forcePower] < level.time)
		{
			BG_ForcePowerDrain( &self->client->ps, forcePower, 1 );
			if (self->client->ps.fd.forcePower < 1)
			{
				WP_ForcePowerStop(self, forcePower);
			}

			self->client->ps.fd.forcePowerDebounce[forcePower] = level.time + 300;
		}
		break;
	default:
		break;
	}
}

int WP_DoSpecificPower( gentity_t *self, usercmd_t *ucmd, forcePowers_t forcepower)
{
	int powerSucceeded;

	powerSucceeded = 1;

	if ( !WP_ForcePowerAvailable( self, forcepower ) )
	{
		return 0;
	}

	switch(forcepower)
	{
	case FP_HEAL:
		powerSucceeded = 0; //always 0 for nonhold powers
		if (self->client->ps.fd.forceButtonNeedRelease)
		{ //need to release before we can use nonhold powers again
			break;
		}
		ForceHeal(self);
		self->client->ps.fd.forceButtonNeedRelease = 1;
		break;
	case FP_LEVITATION:
		//if leave the ground by some other means, cancel the force jump so we don't suddenly jump when we land.
		
		if ( self->client->ps.groundEntityNum == ENTITYNUM_NONE )
		{
			self->client->ps.fd.forceJumpCharge = 0;
			G_MuteSound( self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_1-50], CHAN_VOICE );
			//This only happens if the groundEntityNum == ENTITYNUM_NONE when the button is actually released
		}
		else
		{//still on ground, so jump
			ForceJump( self, ucmd );
		}
		break;
	case FP_SPEED:
		powerSucceeded = 0; //always 0 for nonhold powers
		if (self->client->ps.fd.forceButtonNeedRelease)
		{ //need to release before we can use nonhold powers again
			break;
		}
		ForceSpeed(self, 0);
		self->client->ps.fd.forceButtonNeedRelease = 1;
		break;
	case FP_GRIP:
		if (self->client->ps.fd.forceGripEntityNum == ENTITYNUM_NONE)
		{
			ForceGrip( self );
		}

		if (self->client->ps.fd.forceGripEntityNum != ENTITYNUM_NONE)
		{
			if (!(self->client->ps.fd.forcePowersActive & (1 << FP_GRIP)))
			{
				WP_ForcePowerStart( self, FP_GRIP, 0 );
				BG_ForcePowerDrain( &self->client->ps, FP_GRIP, GRIP_DRAIN_AMOUNT );
			}
		}
		else
		{
			powerSucceeded = 0;
		}
		break;
	case FP_LIGHTNING:
		ForceLightning(self);
		break;
	case FP_PUSH:
		powerSucceeded = 0; //always 0 for nonhold powers
		if (self->client->ps.fd.forceButtonNeedRelease)
		{ //need to release before we can use nonhold powers again
			break;
		}
		ForceThrow(self, qfalse);
		self->client->ps.fd.forceButtonNeedRelease = 1;
		break;
	case FP_PULL:
		powerSucceeded = 0; //always 0 for nonhold powers
		if (self->client->ps.fd.forceButtonNeedRelease)
		{ //need to release before we can use nonhold powers again
			break;
		}
		ForceThrow(self, qtrue);
		self->client->ps.fd.forceButtonNeedRelease = 1;
		break;
	case FP_TELEPATHY:
		powerSucceeded = 0; //always 0 for nonhold powers
		if (self->client->ps.fd.forceButtonNeedRelease)
		{ //need to release before we can use nonhold powers again
			break;
		}
		ForceTelepathy(self);
		self->client->ps.fd.forceButtonNeedRelease = 1;
		break;
	case FP_RAGE:
		powerSucceeded = 0; //always 0 for nonhold powers
		if (self->client->ps.fd.forceButtonNeedRelease)
		{ //need to release before we can use nonhold powers again
			break;
		}
		ForceRage(self);
		self->client->ps.fd.forceButtonNeedRelease = 1;
		break;
	case FP_PROTECT:
		powerSucceeded = 0; //always 0 for nonhold powers
		if (self->client->ps.fd.forceButtonNeedRelease)
		{ //need to release before we can use nonhold powers again
			break;
		}
		ForceProtect(self);
		self->client->ps.fd.forceButtonNeedRelease = 1;
		break;
	case FP_ABSORB:
		powerSucceeded = 0; //always 0 for nonhold powers
		if (self->client->ps.fd.forceButtonNeedRelease)
		{ //need to release before we can use nonhold powers again
			break;
		}
		ForceAbsorb(self);
		self->client->ps.fd.forceButtonNeedRelease = 1;
		break;
	case FP_TEAM_HEAL:
		powerSucceeded = 0; //always 0 for nonhold powers
		if (self->client->ps.fd.forceButtonNeedRelease)
		{ //need to release before we can use nonhold powers again
			break;
		}
		ForceTeamHeal(self);
		self->client->ps.fd.forceButtonNeedRelease = 1;
		break;
	case FP_TEAM_FORCE:
		powerSucceeded = 0; //always 0 for nonhold powers
		if (self->client->ps.fd.forceButtonNeedRelease)
		{ //need to release before we can use nonhold powers again
			break;
		}
		ForceTeamForceReplenish(self);
		self->client->ps.fd.forceButtonNeedRelease = 1;
		break;
	case FP_DRAIN:
		ForceDrain(self);
		break;
	case FP_SEE:
		powerSucceeded = 0; //always 0 for nonhold powers
		if (self->client->ps.fd.forceButtonNeedRelease)
		{ //need to release before we can use nonhold powers again
			break;
		}
		ForceSeeing(self);
		self->client->ps.fd.forceButtonNeedRelease = 1;
		break;
	case FP_SABERATTACK:
		break;
	case FP_SABERDEFEND:
		break;
	case FP_SABERTHROW:
		break;
	default:
		break;
	}

	return powerSucceeded;
}

void FindGenericEnemyIndex(gentity_t *self)
{
	int i = 0;
	float tlen;
	gentity_t *ent;
	gentity_t *besten = NULL;
	float blen = 99999999;
	vec3_t a;

	while (i < MAX_CLIENTS)
	{
		ent = &g_entities[i];

		if (ent && ent->client && ent->s.number != self->s.number && ent->health > 0 && !OnSameTeam(self, ent) && ent->client->ps.pm_type != PM_INTERMISSION && ent->client->ps.pm_type != PM_SPECTATOR)
		{
			VectorSubtract(ent->client->ps.origin, self->client->ps.origin, a);
			tlen = VectorLength(a);

			if (tlen < blen &&
				//InFieldOfVision(ent->client->ps.viewangles, 90, a) &&
				InFront(ent->client->ps.origin, self->client->ps.origin, self->client->ps.viewangles, 0.8f ) &&
				OrgVisible(self->client->ps.origin, ent->client->ps.origin, self->s.number))
			{
				blen = tlen;
				besten = ent;
			}
		}

		i++;
	}

	if (!besten)
	{
		return;
	}

	self->client->ps.genericEnemyIndex = besten->s.number;
}

void SeekerDroneUpdate(gentity_t *self)
{
	vec3_t org, elevated, dir, a, endir;
	gentity_t *en;
	float angle;
	float prefig = 0;
	trace_t tr;

	if (!(self->client->ps.eFlags & EF_SEEKERDRONE))
	{
		self->client->ps.genericEnemyIndex = -1;
		return;
	}

	if (self->health < 1)
	{
		VectorCopy(self->client->ps.origin, elevated);
		elevated[2] += 40;

		angle = ((level.time / 12) & 255) * (M_PI * 2) / 255; //magical numbers make magic happen
		dir[0] = cos(angle) * 20;
		dir[1] = sin(angle) * 20;
		dir[2] = cos(angle) * 5;
		VectorAdd(elevated, dir, org);

		a[ROLL] = 0;
		a[YAW] = 0;
		a[PITCH] = 1;

		G_PlayEffect(EFFECT_SPARK_EXPLOSION, org, a);

		self->client->ps.eFlags -= EF_SEEKERDRONE;
		self->client->ps.genericEnemyIndex = -1;

		return;
	}

	if (self->client->ps.droneExistTime >= level.time && 
		self->client->ps.droneExistTime < (level.time+5000))
	{
		self->client->ps.genericEnemyIndex = 1024+self->client->ps.droneExistTime;
		if (self->client->ps.droneFireTime < level.time)
		{
			G_Sound( self, CHAN_BODY, G_SoundIndex("sound/weapons/laser_trap/warning.wav") );
			self->client->ps.droneFireTime = level.time + 100;
		}
		return;
	}
	else if (self->client->ps.droneExistTime < level.time)
	{
		VectorCopy(self->client->ps.origin, elevated);
		elevated[2] += 40;

		prefig = (self->client->ps.droneExistTime-level.time)/80;

		if (prefig > 55)
		{
			prefig = 55;
		}
		else if (prefig < 1)
		{
			prefig = 1;
		}

		elevated[2] -= 55-prefig;

		angle = ((level.time / 12) & 255) * (M_PI * 2) / 255; //magical numbers make magic happen
		dir[0] = cos(angle) * 20;
		dir[1] = sin(angle) * 20;
		dir[2] = cos(angle) * 5;
		VectorAdd(elevated, dir, org);

		a[ROLL] = 0;
		a[YAW] = 0;
		a[PITCH] = 1;

		G_PlayEffect(EFFECT_SPARK_EXPLOSION, org, a);

		self->client->ps.eFlags -= EF_SEEKERDRONE;
		self->client->ps.genericEnemyIndex = -1;

		return;
	}

	if (self->client->ps.genericEnemyIndex == -1)
	{
		self->client->ps.genericEnemyIndex = ENTITYNUM_NONE;
	}

	if (self->client->ps.genericEnemyIndex != ENTITYNUM_NONE && self->client->ps.genericEnemyIndex != -1)
	{
		en = &g_entities[self->client->ps.genericEnemyIndex];

		if (!en || !en->client)
		{
			self->client->ps.genericEnemyIndex = ENTITYNUM_NONE;
		}
		else if (en->s.number == self->s.number)
		{
			self->client->ps.genericEnemyIndex = ENTITYNUM_NONE;
		}
		else if (en->health < 1)
		{
			self->client->ps.genericEnemyIndex = ENTITYNUM_NONE;
		}
		else if (OnSameTeam(self, en))
		{
			self->client->ps.genericEnemyIndex = ENTITYNUM_NONE;
		}
		else
		{
			//if (!InFieldOfVision(en->client->ps.viewangles, 90, a))
			if (!InFront(en->client->ps.origin, self->client->ps.origin, self->client->ps.viewangles, 0.8f ))
			{
				self->client->ps.genericEnemyIndex = ENTITYNUM_NONE;
			}
			else if (!OrgVisible(self->client->ps.origin, en->client->ps.origin, self->s.number))
			{
				self->client->ps.genericEnemyIndex = ENTITYNUM_NONE;
			}
		}
	}

	if (self->client->ps.genericEnemyIndex == ENTITYNUM_NONE || self->client->ps.genericEnemyIndex == -1)
	{
		FindGenericEnemyIndex(self);
	}

	if (self->client->ps.genericEnemyIndex != ENTITYNUM_NONE && self->client->ps.genericEnemyIndex != -1)
	{
		en = &g_entities[self->client->ps.genericEnemyIndex];

		VectorCopy(self->client->ps.origin, elevated);
		elevated[2] += 40;

		angle = ((level.time / 12) & 255) * (M_PI * 2) / 255; //magical numbers make magic happen
		dir[0] = cos(angle) * 20;
		dir[1] = sin(angle) * 20;
		dir[2] = cos(angle) * 5;
		VectorAdd(elevated, dir, org);

		//org is now where the thing should be client-side because it uses the same time-based offset
		if (self->client->ps.droneFireTime < level.time)
		{
			trap_Trace(&tr, org, NULL, NULL, en->client->ps.origin, -1, MASK_SOLID);

			if (tr.fraction == 1 && !tr.startsolid && !tr.allsolid)
			{
				VectorSubtract(en->client->ps.origin, org, endir);
				VectorNormalize(endir);

				WP_FireGenericBlasterMissile(self, org, endir, 0, 15, 2000, MOD_BLASTER);
				G_SoundAtLoc( org, CHAN_WEAPON, G_SoundIndex("sound/weapons/bryar/fire.wav") );

				self->client->ps.droneFireTime = level.time + Q_irand(400, 700);
			}
		}
	}
}

void HolocronUpdate(gentity_t *self)
{
	int i = 0;
	int noHRank = 0;

	if (noHRank < FORCE_LEVEL_0)
	{
		noHRank = FORCE_LEVEL_0;
	}
	if (noHRank > FORCE_LEVEL_3)
	{
		noHRank = FORCE_LEVEL_3;
	}

	trap_Cvar_Update(&g_MaxHolocronCarry);

	while (i < NUM_FORCE_POWERS)
	{
		if (self->client->ps.holocronsCarried[i])
		{
			self->client->ps.holocronBits |= (1 << i);
			self->client->ps.fd.forcePowersKnown |= (1 << i);
			self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_3;
		}
		else
		{
			self->client->ps.fd.forcePowerLevel[i] = 0;
			if (self->client->ps.holocronBits & (1 << i))
			{
				self->client->ps.holocronBits -= (1 << i);
			}

			if ((self->client->ps.fd.forcePowersKnown & (1 << i)) && i != FP_LEVITATION && i != FP_SABERATTACK)
			{
				self->client->ps.fd.forcePowersKnown -= (1 << i);
			}

			if ((self->client->ps.fd.forcePowersActive & (1 << i)) && i != FP_LEVITATION && i != FP_SABERATTACK)
			{
				WP_ForcePowerStop(self, i);
			}

			if (i == FP_LEVITATION)
			{
				if (noHRank >= FORCE_LEVEL_1)
				{
					self->client->ps.fd.forcePowerLevel[i] = noHRank;
				}
				else
				{
					self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_1;
				}
			}
			else if (i == FP_SABERATTACK)
			{
				self->client->ps.fd.forcePowersKnown |= (1 << i);

				if (noHRank >= FORCE_LEVEL_1)
				{
					self->client->ps.fd.forcePowerLevel[i] = noHRank;
				}
				else
				{
					self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_1;
				}
			}
			else
			{
				self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_0;
			}
		}

		i++;
	}

	if (HasSetSaberOnly())
	{
		/*
		self->client->ps.fd.forcePowerLevel[FP_SABERATTACK] = FORCE_LEVEL_3;
		self->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] = FORCE_LEVEL_3;
		self->client->ps.fd.forcePowerLevel[FP_SABERTHROW] = FORCE_LEVEL_3;
		*/
		if (self->client->ps.fd.forcePowerLevel[FP_SABERATTACK] < FORCE_LEVEL_1)
		{
			self->client->ps.fd.forcePowerLevel[FP_SABERATTACK] = FORCE_LEVEL_1;
		}
		if (self->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] < FORCE_LEVEL_1)
		{
			self->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] = FORCE_LEVEL_1;
		}
	}
}

void JediMasterUpdate(gentity_t *self)
{
	int i = 0;

	trap_Cvar_Update(&g_MaxHolocronCarry);

	while (i < NUM_FORCE_POWERS)
	{
		if (self->client->ps.isJediMaster)
		{
			self->client->ps.fd.forcePowersKnown |= (1 << i);
			self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_3;

			if (i == FP_TEAM_HEAL || i == FP_TEAM_FORCE ||
				i == FP_DRAIN || i == FP_ABSORB)
			{ //team powers are useless in JM, absorb is too because no one else has powers to absorb. Drain is just
			  //relatively useless in comparison, because its main intent is not to heal, but rather to cripple others
			  //by draining their force at the same time. And no one needs force in JM except the JM himself.
				self->client->ps.fd.forcePowersKnown &= ~(1 << i);
				self->client->ps.fd.forcePowerLevel[i] = 0;
			}

			if (i == FP_TELEPATHY)
			{ //this decision was made because level 3 mindtrick allows the JM to just hide too much, and no one else has force
			  //sight to counteract it. Since the JM himself is the focus of gameplay in this mode, having him hidden for large
			  //durations is indeed a bad thing.
				self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_2;
			}
		}
		else
		{
			if ((self->client->ps.fd.forcePowersKnown & (1 << i)) && i != FP_LEVITATION)
			{
				self->client->ps.fd.forcePowersKnown -= (1 << i);
			}

			if ((self->client->ps.fd.forcePowersActive & (1 << i)) && i != FP_LEVITATION)
			{
				WP_ForcePowerStop(self, i);
			}

			if (i == FP_LEVITATION)
			{
				self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_1;
			}
			else
			{
				self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_0;
			}
		}

		i++;
	}
}

void WP_ForcePowersUpdate( gentity_t *self, usercmd_t *ucmd )
{
	qboolean	usingForce = qfalse;
	vec3_t		dmgdir;
	int			i, holo, holoregen;
	int			prepower = 0;
	//see if any force powers are running
	if ( !self )
	{
		return;
	}

	if ( !self->client )
	{
		return;
	}

	if (self->client->ps.pm_flags & PMF_FOLLOW)
	{ //not a "real" game client, it's a spectator following someone
		return;
	}
	if (self->client->sess.sessionTeam == TEAM_SPECTATOR)
	{
		return;
	}

	if (self->client->ps.fd.saberAnimLevel > self->client->ps.fd.forcePowerLevel[FP_SABERATTACK])
	{
		self->client->ps.fd.saberAnimLevel = self->client->ps.fd.forcePowerLevel[FP_SABERATTACK];
	}
	else if (!self->client->ps.fd.saberAnimLevel)
	{
		self->client->ps.fd.saberAnimLevel = FORCE_LEVEL_1;
	}

	if (!(self->client->ps.fd.forcePowersKnown & (1 << FP_LEVITATION)))
	{
		self->client->ps.fd.forcePowersKnown |= (1 << FP_LEVITATION);
	}

	if (self->client->ps.fd.forcePowerLevel[FP_LEVITATION] < FORCE_LEVEL_1)
	{
		self->client->ps.fd.forcePowerLevel[FP_LEVITATION] = FORCE_LEVEL_1;
	}

	if (self->client->ps.fd.forcePowerSelected < 0)
	{ //bad
		self->client->ps.fd.forcePowerSelected = 0;
	}

	if ( ((self->client->sess.selectedFP != self->client->ps.fd.forcePowerSelected) ||
		(self->client->sess.saberLevel != self->client->ps.fd.saberAnimLevel)) &&
		!(self->r.svFlags & SVF_BOT) )
	{
		if (self->client->sess.updateUITime < level.time)
		{ //a bit hackish, but we don't want the client to flood with userinfo updates if they rapidly cycle
		  //through their force powers or saber attack levels

			self->client->sess.selectedFP = self->client->ps.fd.forcePowerSelected;
			self->client->sess.saberLevel = self->client->ps.fd.saberAnimLevel;
		}
	}

	if (!g_LastFrameTime)
	{
		g_LastFrameTime = level.time;
	}

	if (self->client->ps.forceHandExtend == HANDEXTEND_KNOCKDOWN)
	{
		self->client->ps.zoomFov = 0;
		self->client->ps.zoomMode = 0;
		self->client->ps.zoomLocked = qfalse;
		self->client->ps.zoomTime = 0;
	}

	if (self->client->ps.forceHandExtend != HANDEXTEND_NONE &&
		self->client->ps.forceHandExtendTime < level.time)
	{
		if (self->client->ps.forceHandExtend == HANDEXTEND_KNOCKDOWN &&
			!self->client->ps.forceDodgeAnim)
		{
			if (self->client->pers.cmd.upmove &&
				self->client->ps.fd.forcePowerLevel[FP_LEVITATION] > FORCE_LEVEL_1)
			{ //force getup
				G_PreDefSound(self->client->ps.origin, PDSOUND_FORCEJUMP);
				self->client->ps.forceDodgeAnim = 2;
				self->client->ps.forceHandExtendTime = level.time + 800;//1000;

				self->client->ps.velocity[2] = 300;
			}
			else if (self->client->ps.quickerGetup)
			{
				self->client->ps.quickerGetup = qfalse;
				G_EntitySound( self, CHAN_VOICE, G_SoundIndex("*jump1.wav") );
				self->client->ps.forceDodgeAnim = 3;
				self->client->ps.forceHandExtendTime = level.time + 600;
				self->client->ps.velocity[2] = 200;
			}
			else
			{
				self->client->ps.forceDodgeAnim = 1;
				self->client->ps.forceHandExtendTime = level.time + 1000;
			}
		}
		else
		{
			self->client->ps.forceHandExtend = HANDEXTEND_WEAPONREADY;
		}
	}

	if (g_gametype.integer == GT_HOLOCRON)
	{
		HolocronUpdate(self);
	}
	if (g_gametype.integer == GT_JEDIMASTER)
	{
		JediMasterUpdate(self);
	}

	SeekerDroneUpdate(self);

	if (self->client->ps.powerups[PW_FORCE_BOON])
	{
		prepower = self->client->ps.fd.forcePower;
	}

	if (self && self->client && (BG_HasYsalamiri(g_gametype.integer, &self->client->ps) ||
		self->client->ps.fd.forceDeactivateAll))
	{
		i = 0;

		while (i < NUM_FORCE_POWERS)
		{
			if ((self->client->ps.fd.forcePowersActive & (1 << i)) && i != FP_LEVITATION)
			{
				WP_ForcePowerStop(self, i);
			}

			i++;
		}

		self->client->ps.fd.forceDeactivateAll = 0;

		if (self->client->ps.fd.forceJumpCharge)
		{
			G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_1-50], CHAN_VOICE);
			self->client->ps.fd.forceJumpCharge = 0;
		}
	}
	else
	{
		i = 0;

		while (i < NUM_FORCE_POWERS)
		{
			if ((self->client->ps.fd.forcePowersActive & (1 << i)) && i != FP_LEVITATION &&
				!BG_CanUseFPNow(g_gametype.integer, &self->client->ps, level.time, i))
			{
				WP_ForcePowerStop(self, i);
			}

			i++;
		}
	}

	i = 0;

	if (self->client->ps.powerups[PW_FORCE_ENLIGHTENED_LIGHT] || self->client->ps.powerups[PW_FORCE_ENLIGHTENED_DARK])
	{
		if (!self->client->ps.fd.forceUsingAdded)
		{
			i = 0;

			while (i < NUM_FORCE_POWERS)
			{
				self->client->ps.fd.forcePowerBaseLevel[i] = self->client->ps.fd.forcePowerLevel[i];

				if (!forcePowerDarkLight[i] ||
					self->client->ps.fd.forceSide == forcePowerDarkLight[i])
				{
					self->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_3;
					self->client->ps.fd.forcePowersKnown |= (1 << i);
				}

				i++;
			}

			self->client->ps.fd.forceUsingAdded = 1;
		}
	}
	else if (self->client->ps.fd.forceUsingAdded)
	{
		i = 0;

		while (i < NUM_FORCE_POWERS)
		{
			self->client->ps.fd.forcePowerLevel[i] = self->client->ps.fd.forcePowerBaseLevel[i];
			if (!self->client->ps.fd.forcePowerLevel[i])
			{
				if (self->client->ps.fd.forcePowersActive & (1 << i))
				{
					WP_ForcePowerStop(self, i);
				}
				self->client->ps.fd.forcePowersKnown &= ~(1 << i);
			}

			i++;
		}

		self->client->ps.fd.forceUsingAdded = 0;
	}

	i = 0;

	if (!(self->client->ps.fd.forcePowersActive & (1 << FP_TELEPATHY)))
	{
		self->client->ps.fd.forceMindtrickTargetIndex = 0;
		self->client->ps.fd.forceMindtrickTargetIndex2 = 0;
		self->client->ps.fd.forceMindtrickTargetIndex3 = 0;
		self->client->ps.fd.forceMindtrickTargetIndex4 = 0;
	}
	
	/*if (self->s.number == 0)
	//if (self->s.number == 1)
	{
		G_Printf("FP: %i\n", self->client->ps.fd.forcePower);
	}*/
	

	if (self->health < 1)
	{
		self->client->ps.fd.forceGripBeingGripped = 0;
	}

	if (self->client->ps.fd.forceGripBeingGripped > level.time)
	{
		self->client->ps.fd.forceGripCripple = 1;
	}
	else
	{
		self->client->ps.fd.forceGripCripple = 0;
	}

	if (self->client->ps.fd.forceJumpSound)
	{
		G_PreDefSound(self->client->ps.origin, PDSOUND_FORCEJUMP);
		self->client->ps.fd.forceJumpSound = 0;
	}

	if (self->client->ps.fd.forceGripCripple)
	{
		if (self->client->ps.fd.forceGripSoundTime < level.time)
		{
			G_PreDefSound(self->client->ps.origin, PDSOUND_FORCEGRIP);
			self->client->ps.fd.forceGripSoundTime = level.time + 1000;
		}
	}

	if (self->client->ps.fd.forcePowersActive & (1 << FP_SPEED))
	{
		self->client->ps.powerups[PW_SPEED] = level.time + 100;
	}

	if (self->client->ps.fd.forceSpeedDoDamage && FORCE_VELOCITY_DAMAGE)
	{ //we set the flag somewhere to do damage for some reason, so do it
		G_Damage (self, NULL, NULL, NULL, NULL, self->client->ps.fd.forceSpeedDoDamage, DAMAGE_NO_ARMOR, MOD_FALLING);
		//self->client->ps.pm_time = 20000;
		//self->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;

		if (self->client->ps.fd.forceSpeedHitIndex != ENTITYNUM_NONE &&
			g_entities[self->client->ps.fd.forceSpeedHitIndex].client)
		{
			VectorSubtract(g_entities[self->client->ps.fd.forceSpeedHitIndex].client->ps.origin, self->client->ps.origin, dmgdir);

			G_Damage (&g_entities[self->client->ps.fd.forceSpeedHitIndex], self, self, dmgdir, NULL, self->client->ps.fd.forceSpeedDoDamage, DAMAGE_NO_ARMOR, MOD_CRUSH);
			self->client->ps.fd.forceSpeedHitIndex = ENTITYNUM_NONE;
		}

		//placeholder sound:
		//G_Sound(self, CHAN_BODY, G_SoundIndex("sound/test/objectBreak.wav"));
		self->client->ps.fd.forceSpeedDoDamage = 0;
	}

	if ( self->health <= 0 )
	{//if dead, deactivate any active force powers
		for ( i = 0; i < NUM_FORCE_POWERS; i++ )
		{
			if ( self->client->ps.fd.forcePowerDuration[i] || (self->client->ps.fd.forcePowersActive&( 1 << i )) )
			{
				WP_ForcePowerStop( self, (forcePowers_t)i );
				self->client->ps.fd.forcePowerDuration[i] = 0;
			}
		}
		//return;
		goto powersetcheck;
	}

//	if (!ucmd->upmove || (self->client->ps.groundEntityNum != ENTITYNUM_NONE && !self->client->ps.fd.forceJumpCharge))
//	{
//		self->client->groundTime = 0;
//	}

	if (self->client->ps.groundEntityNum != ENTITYNUM_NONE)
	{
		self->client->fjDidJump = qfalse;
	}

	if (self->client->ps.fd.forceJumpCharge && self->client->ps.groundEntityNum == ENTITYNUM_NONE && self->client->fjDidJump)
	{
		if (ucmd->upmove < 10 && /*!(ucmd->buttons & BUTTON_FORCEJUMP) &&*/ (!(ucmd->buttons & BUTTON_FORCEPOWER) || self->client->ps.fd.forcePowerSelected != FP_LEVITATION))
		{
			G_MuteSound(self->client->ps.fd.killSoundEntIndex[TRACK_CHANNEL_1-50], CHAN_VOICE);
			self->client->ps.fd.forceJumpCharge = 0;
		}
	}

	/*
	if ( (ucmd->buttons & BUTTON_FORCEJUMP) && !BG_HasYsalamiri(g_gametype.integer, &self->client->ps) && BG_CanUseFPNow(g_gametype.integer, &self->client->ps, level.time, FP_LEVITATION) )
	{//just charging up
		ForceJumpCharge( self, ucmd );
		usingForce = qtrue;
	}
	*/
#ifndef METROID_JUMP
	else if ( /*!self->client->fjDidJump &&*/ (ucmd->upmove > 10) && (self->client->ps.pm_flags & PMF_JUMP_HELD) && self->client->ps.groundTime && (level.time - self->client->ps.groundTime) > 150 && !BG_HasYsalamiri(g_gametype.integer, &self->client->ps) && BG_CanUseFPNow(g_gametype.integer, &self->client->ps, level.time, FP_LEVITATION)/*&& !self->client->ps.fd.forceJumpZStart*/ )
	{//just charging up
		ForceJumpCharge( self, ucmd );
		usingForce = qtrue;
	}
	else if (ucmd->upmove < 10 && self->client->ps.groundEntityNum == ENTITYNUM_NONE && self->client->ps.fd.forceJumpCharge)
	{
		self->client->ps.pm_flags &= ~(PMF_JUMP_HELD);
	}
#endif

	if (/*!(ucmd->buttons & BUTTON_FORCEJUMP) &&*/ !(self->client->ps.pm_flags & PMF_JUMP_HELD) && self->client->ps.fd.forceJumpCharge)
	{
		if (!(ucmd->buttons & BUTTON_FORCEPOWER) ||
			self->client->ps.fd.forcePowerSelected != FP_LEVITATION)
		{
			if (WP_DoSpecificPower( self, ucmd, FP_LEVITATION ))
			{
				usingForce = qtrue;
			}
		}
	}

	if ( ucmd->buttons & BUTTON_FORCEGRIP )
	{
		if (WP_DoSpecificPower( self, ucmd, FP_GRIP ))
		{
			usingForce = qtrue;
		}
		else
		{ //don't let recharge even if the grip misses if the player still has the button down
			usingForce = qtrue;
		}
	}
	else
	{
		if (self->client->ps.fd.forcePowersActive & (1 << FP_GRIP))
		{
			if (!(ucmd->buttons & BUTTON_FORCEPOWER) || self->client->ps.fd.forcePowerSelected != FP_GRIP)
			{
				WP_ForcePowerStop(self, FP_GRIP);
			}
		}
	}

	if ( ucmd->buttons & BUTTON_FORCE_LIGHTNING )
	{
		WP_DoSpecificPower(self, ucmd, FP_LIGHTNING);
		usingForce = qtrue;
	}
	else
	{
		if (self->client->ps.fd.forcePowersActive & (1 << FP_LIGHTNING))
		{
			if (!(ucmd->buttons & BUTTON_FORCEPOWER) || self->client->ps.fd.forcePowerSelected != FP_LIGHTNING)
			{
				WP_ForcePowerStop(self, FP_LIGHTNING);
			}
		}
	}

	if ( ucmd->buttons & BUTTON_FORCE_DRAIN )
	{
		WP_DoSpecificPower(self, ucmd, FP_DRAIN);
		usingForce = qtrue;
	}
	else
	{
		if (self->client->ps.fd.forcePowersActive & (1 << FP_DRAIN))
		{
			if (!(ucmd->buttons & BUTTON_FORCEPOWER) || self->client->ps.fd.forcePowerSelected != FP_DRAIN)
			{
				WP_ForcePowerStop(self, FP_DRAIN);
			}
		}
	}

	if ( (ucmd->buttons & BUTTON_FORCEPOWER) /*&&
		!BG_HasYsalamiri(g_gametype.integer, &self->client->ps)*/ &&
	//	WP_ForcePowerUsable(self, self->client->ps.fd.forcePowerSelected) &&
		BG_CanUseFPNow(g_gametype.integer, &self->client->ps, level.time, self->client->ps.fd.forcePowerSelected))
	{
		if (self->client->ps.fd.forcePowerSelected == FP_LEVITATION /*&& !self->client->fjDidJump*/)
		{
			ForceJumpCharge( self, ucmd );
			usingForce = qtrue;
		}
		else if (WP_DoSpecificPower( self, ucmd, self->client->ps.fd.forcePowerSelected ))
		{
			usingForce = qtrue;
		}
		else if (self->client->ps.fd.forcePowerSelected == FP_GRIP)
		{
			usingForce = qtrue;
		}
	}
	else
	{
		self->client->ps.fd.forceButtonNeedRelease = 0;
	}

	for ( i = 0; i < NUM_FORCE_POWERS; i++ )
	{
		if ( self->client->ps.fd.forcePowerDuration[i] )
		{
			if ( self->client->ps.fd.forcePowerDuration[i] < level.time )
			{
				if ( (self->client->ps.fd.forcePowersActive&( 1 << i )) )
				{//turn it off
					WP_ForcePowerStop( self, (forcePowers_t)i );
				}
				self->client->ps.fd.forcePowerDuration[i] = 0;
			}
		}
		if ( (self->client->ps.fd.forcePowersActive&( 1 << i )) )
		{
			usingForce = qtrue;
			WP_ForcePowerRun( self, (forcePowers_t)i, ucmd );
		}
	}
	if ( self->client->ps.saberInFlight )
	{//don't regen force power while throwing saber
		if ( self->client->ps.saberEntityNum < ENTITYNUM_NONE && self->client->ps.saberEntityNum > 0 )//player is 0
		{//
			if ( &g_entities[self->client->ps.saberEntityNum] != NULL && g_entities[self->client->ps.saberEntityNum].s.pos.trType == TR_LINEAR )
			{//fell to the ground and we're trying to pull it back
				usingForce = qtrue;
			}
		}
	}
	if ( /*!usingForce*/!self->client->ps.fd.forcePowersActive || self->client->ps.fd.forcePowersActive == (1 << FP_DRAIN) )
	{//when not using the force, regenerate at 1 point per half second
		if ( !self->client->ps.saberInFlight && self->client->ps.fd.forcePowerRegenDebounceTime < level.time )
		{
			if (g_gametype.integer != GT_HOLOCRON || g_MaxHolocronCarry.value)
			{
				if (self->client->ps.powerups[PW_FORCE_BOON])
				{
					WP_ForcePowerRegenerate( self, 6 );
				}
				else if (self->client->ps.isJediMaster && g_gametype.integer == GT_JEDIMASTER)
				{
					WP_ForcePowerRegenerate( self, 4 ); //jedi master regenerates 4 times as fast
				}
				else
				{
					WP_ForcePowerRegenerate( self, 0 );
				}
			}
			else
			{ //regenerate based on the number of holocrons carried
				holoregen = 0;
				holo = 0;
				while (holo < NUM_FORCE_POWERS)
				{
					if (self->client->ps.holocronsCarried[holo])
					{
						holoregen++;
					}
					holo++;
				}

				WP_ForcePowerRegenerate(self, holoregen);
			}

			self->client->ps.fd.forcePowerRegenDebounceTime = level.time + g_forceRegenTime.integer; //500?
		}
	}

powersetcheck:

	if (prepower && self->client->ps.fd.forcePower < prepower)
	{
		int dif = ((prepower - self->client->ps.fd.forcePower)/2);
		if (dif < 1)
		{
			dif = 1;
		}

		self->client->ps.fd.forcePower = (prepower-dif);
	}
}

qboolean Jedi_DodgeEvasion( gentity_t *self, gentity_t *shooter, trace_t *tr, int hitLoc )
{
	int	dodgeAnim = -1;

	if ( !self || !self->client || self->health <= 0 )
	{
		return qfalse;
	}

	if (!g_forceDodge.integer)
	{
		return qfalse;
	}

	if (g_forceDodge.integer != 2)
	{
		if (!(self->client->ps.fd.forcePowersActive & (1 << FP_SEE)))
		{
			return qfalse;
		}
	}

	if (self->client->ps.usingATST)
	{
		return qfalse;
	}

	if ( self->client->ps.groundEntityNum == ENTITYNUM_NONE )
	{//can't dodge in mid-air
		return qfalse;
	}

	if ( self->client->ps.weaponTime > 0 || self->client->ps.forceHandExtend != HANDEXTEND_NONE )
	{//in some effect that stops me from moving on my own
		return qfalse;
	}

	if (g_forceDodge.integer == 2)
	{
		if (self->client->ps.fd.forcePowersActive)
		{ //for now just don't let us dodge if we're using a force power at all
			return qfalse;
		}
	}

	if (g_forceDodge.integer == 2)
	{
		if ( !WP_ForcePowerUsable( self, FP_SPEED ) )
		{//make sure we have it and have enough force power
			return qfalse;
		}
	}

	//check force speed power level to determine if I should be able to dodge it
//	if ( Q_irand( 1, 10 ) > self->client->ps.fd.forcePowerLevel[FP_SPEED] )

	if (g_forceDodge.integer == 2)
	{
		if ( Q_irand( 1, 7 ) > self->client->ps.fd.forcePowerLevel[FP_SPEED] )
		{//more likely to fail on lower force speed level
			return qfalse;
		}
	}
	else
	{
		//if ( Q_irand( 1, 4 ) > self->client->ps.fd.forcePowerLevel[FP_SEE] )
		//NOTE: We now dodge all the time, but only on level 3
		if (self->client->ps.fd.forcePowerLevel[FP_SEE] < FORCE_LEVEL_3)
		{//more likely to fail on lower force sight level
			return qfalse;
		}
	}

	switch( hitLoc )
	{
	case HL_NONE:
		return qfalse;
		break;

	case HL_FOOT_RT:
	case HL_FOOT_LT:
	case HL_LEG_RT:
	case HL_LEG_LT:
	//case HL_WAIST:
		return qfalse;

	case HL_BACK_RT:
		dodgeAnim = BOTH_DODGE_FL;
		break;
	case HL_CHEST_RT:
		dodgeAnim = BOTH_DODGE_FR;//BOTH_DODGE_BL;
		break;
	case HL_BACK_LT:
		dodgeAnim = BOTH_DODGE_FR;
		break;
	case HL_CHEST_LT:
		dodgeAnim = BOTH_DODGE_FR;//BOTH_DODGE_BR;
		break;
	case HL_BACK:
	case HL_CHEST:
	case HL_WAIST:
		dodgeAnim = BOTH_DODGE_FL;//Q_irand( BOTH_DODGE_FL, BOTH_DODGE_R );
		break;
	case HL_ARM_RT:
	case HL_HAND_RT:
		dodgeAnim = BOTH_DODGE_L;
		break;
	case HL_ARM_LT:
	case HL_HAND_LT:
		dodgeAnim = BOTH_DODGE_R;
		break;
	case HL_HEAD:
		dodgeAnim = BOTH_DODGE_FL;//Q_irand( BOTH_DODGE_FL, BOTH_DODGE_BR );
		break;
	default:
		return qfalse;
	}

	if ( dodgeAnim != -1 )
	{
		//set the dodge anim we chose
		//NPC_SetAnim( self, SETANIM_BOTH, dodgeAnim, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );//type

		//Our own happy way of forcing an anim:
		self->client->ps.forceHandExtend = HANDEXTEND_DODGE;
		self->client->ps.forceDodgeAnim = dodgeAnim;
		self->client->ps.forceHandExtendTime = level.time + 300;

		self->client->ps.powerups[PW_SPEEDBURST] = level.time + 100;

		if (g_forceDodge.integer == 2)
		{
			ForceSpeed( self, 500 );
		}
		else
		{
			G_Sound( self, CHAN_BODY, G_SoundIndex("sound/weapons/force/speed.wav") );
		}
		return qtrue;
	}
	return qfalse;
}
