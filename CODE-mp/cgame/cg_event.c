// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_event.c -- handle entity events at snapshot or playerstate transitions

#include "cg_local.h"
#include "fx_local.h"
#include "..\ghoul2\g2.h"
#include "../ui/ui_shared.h"

// for the voice chats
#include "../../ui/menudef.h"
//==========================================================================

extern int g_saberFlashTime;
extern vec3_t g_saberFlashPos;
extern char *showPowersName[];

/*
===================
CG_PlaceString

Also called by scoreboard drawing
===================
*/
const char	*CG_PlaceString( int rank ) {
	static char	str[64];
	char	*s, *t;
	// number extenstions, eg 1st, 2nd, 3rd, 4th etc.
	// note that the rules are different for french, but by changing the required strip strings they seem to work
	char sST[10];
	char sND[10];
	char sRD[10];
	char sTH[10];
	char sTiedFor[64];	// german is much longer, super safe...

	trap_SP_GetStringTextString("INGAMETEXT_NUMBER_ST",sST, sizeof(sST) );
	trap_SP_GetStringTextString("INGAMETEXT_NUMBER_ND",sND, sizeof(sND) );
	trap_SP_GetStringTextString("INGAMETEXT_NUMBER_RD",sRD, sizeof(sRD) );
	trap_SP_GetStringTextString("INGAMETEXT_NUMBER_TH",sTH, sizeof(sTH) );
	trap_SP_GetStringTextString("INGAMETEXT_TIED_FOR" ,sTiedFor,sizeof(sTiedFor) );
	strcat(sTiedFor," ");	// save worrying about translators adding spaces or not

	if ( rank & RANK_TIED_FLAG ) {
		rank &= ~RANK_TIED_FLAG;
		t = sTiedFor;//"Tied for ";
	} else {
		t = "";
	}

	if ( rank == 1 ) {
		s = va("1%s",sST);//S_COLOR_BLUE "1st" S_COLOR_WHITE;		// draw in blue
	} else if ( rank == 2 ) {
		s = va("2%s",sND);//S_COLOR_RED "2nd" S_COLOR_WHITE;		// draw in red
	} else if ( rank == 3 ) {
		s = va("3%s",sRD);//S_COLOR_YELLOW "3rd" S_COLOR_WHITE;		// draw in yellow
	} else if ( rank == 11 ) {
		s = va("11%s",sTH);
	} else if ( rank == 12 ) {
		s = va("12%s",sTH);
	} else if ( rank == 13 ) {
		s = va("13%s",sTH);
	} else if ( rank % 10 == 1 ) {
		s = va("%i%s", rank,sST);
	} else if ( rank % 10 == 2 ) {
		s = va("%i%s", rank,sND);
	} else if ( rank % 10 == 3 ) {
		s = va("%i%s", rank,sRD);
	} else {
		s = va("%i%s", rank,sTH);
	}

	Com_sprintf( str, sizeof( str ), "%s%s", t, s );
	return str;
}

qboolean CG_ThereIsAMaster(void);

/*
=============
CG_Obituary
=============
*/
static void CG_Obituary( entityState_t *ent ) {
	int			mod;
	int			target, attacker;
	char		*message;
	const char	*targetInfo;
	const char	*attackerInfo;
	char		targetName[32];
	char		attackerName[32];
	gender_t	gender;
	clientInfo_t	*ci;

	target = ent->otherEntityNum;
	attacker = ent->otherEntityNum2;
	mod = ent->eventParm;

	if ( target < 0 || target >= MAX_CLIENTS ) {
		CG_Error( "CG_Obituary: target out of range" );
	}
	ci = &cgs.clientinfo[target];

	if ( attacker < 0 || attacker >= MAX_CLIENTS ) {
		attacker = ENTITYNUM_WORLD;
		attackerInfo = NULL;
	} else {
		attackerInfo = CG_ConfigString( CS_PLAYERS + attacker );
	}

	targetInfo = CG_ConfigString( CS_PLAYERS + target );
	if ( !targetInfo ) {
		return;
	}
	Q_strncpyz( targetName, Info_ValueForKey( targetInfo, "n" ), sizeof(targetName) - 2);
	strcat( targetName, S_COLOR_WHITE );

	// check for single client messages

	switch( mod ) {
	case MOD_SUICIDE:
	case MOD_FALLING:
	case MOD_CRUSH:
	case MOD_WATER:
	case MOD_SLIME:
	case MOD_LAVA:
	case MOD_TARGET_LASER:
	case MOD_TRIGGER_HURT:
		message = "DIED_GENERIC";
		break;
	default:
		message = NULL;
		break;
	}

	// Attacker killed themselves.  Ridicule them for it.
	if (attacker == target) {
		gender = ci->gender;
		switch (mod) {
		case MOD_BRYAR_PISTOL:
		case MOD_BRYAR_PISTOL_ALT:
		case MOD_BLASTER:
		case MOD_DISRUPTOR:
		case MOD_DISRUPTOR_SPLASH:
		case MOD_DISRUPTOR_SNIPER:
		case MOD_BOWCASTER:
		case MOD_REPEATER:
		case MOD_REPEATER_ALT:
		case MOD_FLECHETTE:
			if ( gender == GENDER_FEMALE )
				message = "SUICIDE_SHOT_FEMALE";
			else if ( gender == GENDER_NEUTER )
				message = "SUICIDE_SHOT_GENDERLESS";
			else
				message = "SUICIDE_SHOT_MALE";
			break;
		case MOD_REPEATER_ALT_SPLASH:
		case MOD_FLECHETTE_ALT_SPLASH:
		case MOD_ROCKET:
		case MOD_ROCKET_SPLASH:
		case MOD_ROCKET_HOMING:
		case MOD_ROCKET_HOMING_SPLASH:
		case MOD_THERMAL:
		case MOD_THERMAL_SPLASH:
		case MOD_TRIP_MINE_SPLASH:
		case MOD_TIMED_MINE_SPLASH:
		case MOD_DET_PACK_SPLASH:
			if ( gender == GENDER_FEMALE )
				message = "SUICIDE_EXPLOSIVES_FEMALE";
			else if ( gender == GENDER_NEUTER )
				message = "SUICIDE_EXPLOSIVES_GENDERLESS";
			else
				message = "SUICIDE_EXPLOSIVES_MALE";
			break;
		case MOD_DEMP2:
			if ( gender == GENDER_FEMALE )
				message = "SUICIDE_ELECTROCUTED_FEMALE";
			else if ( gender == GENDER_NEUTER )
				message = "SUICIDE_ELECTROCUTED_GENDERLESS";
			else
				message = "SUICIDE_ELECTROCUTED_MALE";
			break;
		case MOD_FALLING:
			if ( gender == GENDER_FEMALE )
				message = "SUICIDE_FALLDEATH_FEMALE";
			else if ( gender == GENDER_NEUTER )
				message = "SUICIDE_FALLDEATH_GENDERLESS";
			else
				message = "SUICIDE_FALLDEATH_MALE";
			break;
		default:
			if ( gender == GENDER_FEMALE )
				message = "SUICIDE_GENERICDEATH_FEMALE";
			else if ( gender == GENDER_NEUTER )
				message = "SUICIDE_GENERICDEATH_GENDERLESS";
			else
				message = "SUICIDE_GENERICDEATH_MALE";
			break;
		}
	}

	if (target != attacker && target < MAX_CLIENTS && attacker < MAX_CLIENTS)
	{
		goto clientkilled;
	}

	if (message) {
		gender = ci->gender;

		if (!message[0])
		{
			if ( gender == GENDER_FEMALE )
				message = "SUICIDE_GENERICDEATH_FEMALE";
			else if ( gender == GENDER_NEUTER )
				message = "SUICIDE_GENERICDEATH_GENDERLESS";
			else
				message = "SUICIDE_GENERICDEATH_MALE";
		}
		message = (char *)CG_GetStripEdString("INGAMETEXT", message);

		CG_Printf( "%s %s\n", targetName, message);
		return;
	}

clientkilled:

	// check for kill messages from the current clientNum
	if ( attacker == cg.snap->ps.clientNum ) {
		char	*s;

		if ( cgs.gametype < GT_TEAM && cgs.gametype != GT_TOURNAMENT ) {
			if (cgs.gametype == GT_JEDIMASTER &&
				attacker < MAX_CLIENTS &&
				!ent->isJediMaster &&
				!cg.snap->ps.isJediMaster &&
				CG_ThereIsAMaster())
			{
				char part1[512];
				char part2[512];
				trap_SP_GetStringTextString("INGAMETEXT_KILLED_MESSAGE", part1, sizeof(part1));
				trap_SP_GetStringTextString("INGAMETEXT_JMKILLED_NOTJM", part2, sizeof(part2));
				s = va("%s %s\n%s\n", part1, targetName, part2);
			}
			else if (cgs.gametype == GT_JEDIMASTER &&
				attacker < MAX_CLIENTS &&
				!ent->isJediMaster &&
				!cg.snap->ps.isJediMaster)
			{ //no JM, saber must be out
				char part1[512];
				trap_SP_GetStringTextString("INGAMETEXT_KILLED_MESSAGE", part1, sizeof(part1));
				/*
				kmsg1 = "for 0 points.\nGo for the saber!";
				strcpy(part2, kmsg1);

				s = va("%s %s %s\n", part1, targetName, part2);
				*/
				s = va("%s %s\n", part1, targetName);
			}
			else
			{
				char sPlaceWith[256];
				char sKilledStr[256];
				trap_SP_GetStringTextString("INGAMETEXT_PLACE_WITH",     sPlaceWith, sizeof(sPlaceWith));
				trap_SP_GetStringTextString("INGAMETEXT_KILLED_MESSAGE", sKilledStr, sizeof(sKilledStr));

				s = va("%s %s.\n%s %s %i.", sKilledStr, targetName, 
					CG_PlaceString( cg.snap->ps.persistant[PERS_RANK] + 1 ), 
					sPlaceWith,
					cg.snap->ps.persistant[PERS_SCORE] );
			}
		} else {
			char sKilledStr[256];
			trap_SP_GetStringTextString("INGAMETEXT_KILLED_MESSAGE", sKilledStr, sizeof(sKilledStr));
			s = va("%s %s", sKilledStr, targetName );
		}
		if (!(cg_singlePlayerActive.integer && cg_cameraOrbit.integer)) {
			CG_CenterPrint( s, SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH );
		} 
		// print the text message as well
	}

	// check for double client messages
	if ( !attackerInfo ) {
		attacker = ENTITYNUM_WORLD;
		strcpy( attackerName, "noname" );
	} else {
		Q_strncpyz( attackerName, Info_ValueForKey( attackerInfo, "n" ), sizeof(attackerName) - 2);
		strcat( attackerName, S_COLOR_WHITE );
		// check for kill messages about the current clientNum
		if ( target == cg.snap->ps.clientNum ) {
			Q_strncpyz( cg.killerName, attackerName, sizeof( cg.killerName ) );
		}
	}

	if ( attacker != ENTITYNUM_WORLD ) {
		switch (mod) {
		case MOD_STUN_BATON:
			message = "KILLED_STUN";
			break;
		case MOD_MELEE:
			message = "KILLED_MELEE";
			break;
		case MOD_SABER:
			message = "KILLED_SABER";
			break;
		case MOD_BRYAR_PISTOL:
		case MOD_BRYAR_PISTOL_ALT:
			message = "KILLED_BRYAR";
			break;
		case MOD_BLASTER:
			message = "KILLED_BLASTER";
			break;
		case MOD_DISRUPTOR:
		case MOD_DISRUPTOR_SPLASH:
			message = "KILLED_DISRUPTOR";
			break;
		case MOD_DISRUPTOR_SNIPER:
			message = "KILLED_DISRUPTORSNIPE";
			break;
		case MOD_BOWCASTER:
			message = "KILLED_BOWCASTER";
			break;
		case MOD_REPEATER:
			message = "KILLED_REPEATER";
			break;
		case MOD_REPEATER_ALT:
		case MOD_REPEATER_ALT_SPLASH:
			message = "KILLED_REPEATERALT";
			break;
		case MOD_DEMP2:
		case MOD_DEMP2_ALT:
			message = "KILLED_DEMP2";
			break;
		case MOD_FLECHETTE:
			message = "KILLED_FLECHETTE";
			break;
		case MOD_FLECHETTE_ALT_SPLASH:
			message = "KILLED_FLECHETTE_MINE";
			break;
		case MOD_ROCKET:
		case MOD_ROCKET_SPLASH:
			message = "KILLED_ROCKET";
			break;
		case MOD_ROCKET_HOMING:
		case MOD_ROCKET_HOMING_SPLASH:
			message = "KILLED_ROCKET_HOMING";
			break;
		case MOD_THERMAL:
		case MOD_THERMAL_SPLASH:
			message = "KILLED_THERMAL";
			break;
		case MOD_TRIP_MINE_SPLASH:
			message = "KILLED_TRIPMINE";
			break;
		case MOD_TIMED_MINE_SPLASH:
			message = "KILLED_TRIPMINE_TIMED";
			break;
		case MOD_DET_PACK_SPLASH:
			message = "KILLED_DETPACK";
			break;
		case MOD_FORCE_DARK:
			message = "KILLED_DARKFORCE";
			break;
		case MOD_SENTRY:
			message = "KILLED_SENTRY";
			break;
		case MOD_TELEFRAG:
			message = "KILLED_TELEFRAG";
			break;
		case MOD_CRUSH:
			message = "KILLED_GENERIC";//"KILLED_FORCETOSS";
			break;
		case MOD_FALLING:
			message = "KILLED_FORCETOSS";
			break;
		case MOD_TRIGGER_HURT:
			message = "KILLED_GENERIC";//"KILLED_FORCETOSS";
			break;
		default:
			message = "KILLED_GENERIC";
			break;
		}

		if (message) {
			message = (char *)CG_GetStripEdString("INGAMETEXT", message);

			CG_Printf( "%s %s %s\n", 
				targetName, message, attackerName);
			return;
		}
	}

	// we don't know what it was
	CG_Printf( "%s %s\n", targetName, (char *)CG_GetStripEdString("INGAMETEXT", "DIED_GENERIC") );
}

//==========================================================================

void CG_ToggleBinoculars(centity_t *cent, int forceZoom)
{
	if (cent->currentState.number != cg.snap->ps.clientNum)
	{
		return;
	}

	if (cg.snap->ps.weaponstate != WEAPON_READY)
	{ //So we can't fool it and reactivate while switching to the saber or something.
		return;
	}

	if (cg.snap->ps.weapon == WP_SABER)
	{ //No.
		return;
	}

	if (forceZoom)
	{
		if (forceZoom == 2)
		{
			cg.snap->ps.zoomMode = 0;
		}
		else if (forceZoom == 1)
		{
			cg.snap->ps.zoomMode = 2;
		}
	}

	if (cg.snap->ps.zoomMode == 0)
	{
		trap_S_StartSound( NULL, cg.snap->ps.clientNum, CHAN_AUTO, cgs.media.zoomStart );
	}
	else if (cg.snap->ps.zoomMode == 2)
	{
		trap_S_StartSound( NULL, cg.snap->ps.clientNum, CHAN_AUTO, cgs.media.zoomEnd );
	}
}

/*
===============
CG_UseItem
===============
*/
static void CG_UseItem( centity_t *cent ) {
	clientInfo_t *ci;
	int			itemNum, clientNum;
	gitem_t		*item;
	entityState_t *es;

	es = &cent->currentState;
	
	itemNum = (es->event & ~EV_EVENT_BITS) - EV_USE_ITEM0;
	if ( itemNum < 0 || itemNum > HI_NUM_HOLDABLE ) {
		itemNum = 0;
	}

	// print a message if the local player
	if ( es->number == cg.snap->ps.clientNum ) {
		if ( !itemNum ) {
			//CG_CenterPrint( "No item to use", SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH );
		} else {
			item = BG_FindItemForHoldable( itemNum );
		}
	}

	switch ( itemNum ) {
	default:
	case HI_NONE:
		//trap_S_StartSound (NULL, es->number, CHAN_BODY, cgs.media.useNothingSound );
		break;

	case HI_BINOCULARS:
		CG_ToggleBinoculars(cent, es->eventParm);
		break;

	case HI_SEEKER:
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.deploySeeker );
		break;

	case HI_SHIELD:
	case HI_DATAPAD:
	case HI_SENTRY_GUN:
		break;

//	case HI_MEDKIT:
	case HI_MEDPAC:
		clientNum = cent->currentState.clientNum;
		if ( clientNum >= 0 && clientNum < MAX_CLIENTS ) {
			ci = &cgs.clientinfo[ clientNum ];
			ci->medkitUsageTime = cg.time;
		}
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.medkitSound );
		break;
	}

	if (cg.snap && cg.snap->ps.clientNum == cent->currentState.number && itemNum != HI_BINOCULARS)
	{ //if not using binoculars, we just used that item up, so switch
		BG_CycleInven(&cg.snap->ps, 1);
		cg.itemSelect = -1; //update the client-side selection display
	}
}


/*
================
CG_ItemPickup

A new item was picked up this frame
================
*/
static void CG_ItemPickup( int itemNum ) {
	cg.itemPickup = itemNum;
	cg.itemPickupTime = cg.time;
	cg.itemPickupBlendTime = cg.time;
	// see if it should be the grabbed weapon
	if ( cg.snap && bg_itemlist[itemNum].giType == IT_WEAPON ) {

		// 0 == no switching
		// 1 == automatically switch to best SAFE weapon
		// 2 == automatically switch to best weapon, safe or otherwise
		// 3 == if not saber, automatically switch to best weapon, safe or otherwise

		if (0 == cg_autoswitch.integer)
		{
			// don't switch
		}
		else if ( cg_autoswitch.integer == 1)
		{ //only autoselect if not explosive ("safe")
			if (bg_itemlist[itemNum].giTag != WP_TRIP_MINE &&
				bg_itemlist[itemNum].giTag != WP_DET_PACK &&
				bg_itemlist[itemNum].giTag != WP_THERMAL &&
				bg_itemlist[itemNum].giTag != WP_ROCKET_LAUNCHER &&
				bg_itemlist[itemNum].giTag > cg.snap->ps.weapon &&
				cg.snap->ps.weapon != WP_SABER)
			{
				if (!cg.snap->ps.emplacedIndex)
				{
					cg.weaponSelectTime = cg.time;
				}
				cg.weaponSelect = bg_itemlist[itemNum].giTag;
			}
		}
		else if ( cg_autoswitch.integer == 2)
		{ //autoselect if better
			if (bg_itemlist[itemNum].giTag > cg.snap->ps.weapon &&
				cg.snap->ps.weapon != WP_SABER)
			{
				if (!cg.snap->ps.emplacedIndex)
				{
					cg.weaponSelectTime = cg.time;
				}
				cg.weaponSelect = bg_itemlist[itemNum].giTag;
			}
		}
		/*
		else if ( cg_autoswitch.integer == 3)
		{ //autoselect if better and not using the saber as a weapon
			if (bg_itemlist[itemNum].giTag > cg.snap->ps.weapon &&
				cg.snap->ps.weapon != WP_SABER)
			{
				if (!cg.snap->ps.emplacedIndex)
				{
					cg.weaponSelectTime = cg.time;
				}
				cg.weaponSelect = bg_itemlist[itemNum].giTag;
			}
		}
		*/
		//No longer required - just not switching ever if using saber
	}

	//rww - print pickup messages
	if (bg_itemlist[itemNum].classname && bg_itemlist[itemNum].classname[0] &&
		(bg_itemlist[itemNum].giType != IT_TEAM || (bg_itemlist[itemNum].giTag != PW_REDFLAG && bg_itemlist[itemNum].giTag != PW_BLUEFLAG)) )
	{ //don't print messages for flags, they have their own pickup event broadcasts
		char	text[1024];

		if ( trap_SP_GetStringTextString( va("INGAME_%s",bg_itemlist[itemNum].classname), text, sizeof( text )))
		{
			Com_Printf("%s %s\n", CG_GetStripEdString("INGAMETEXT", "PICKUPLINE"), text);
		}
		else
		{
			Com_Printf("%s %s\n", CG_GetStripEdString("INGAMETEXT", "PICKUPLINE"), bg_itemlist[itemNum].classname);
		}
	}
}


/*
================
CG_PainEvent

Also called by playerstate transition
================
*/
void CG_PainEvent( centity_t *cent, int health ) {
	char	*snd;

	// don't do more than two pain sounds a second
	if ( cg.time - cent->pe.painTime < 500 ) {
		return;
	}

	if ( health < 25 ) {
		snd = "*pain25.wav";
	} else if ( health < 50 ) {
		snd = "*pain50.wav";
	} else if ( health < 75 ) {
		snd = "*pain75.wav";
	} else {
		snd = "*pain100.wav";
	}
	trap_S_StartSound( NULL, cent->currentState.number, CHAN_VOICE, 
		CG_CustomSound( cent->currentState.number, snd ) );

	// save pain time for programitic twitch animation
	cent->pe.painTime = cg.time;
	cent->pe.painDirection	^= 1;
}

void CG_ReattachLimb(centity_t *source)
{
	char *limbName;
	char *stubCapName;

	switch (source->torsoBolt)
	{
	case G2_MODELPART_HEAD:
		limbName = "head";
		stubCapName = "torso_cap_head_off";
		break;
	case G2_MODELPART_WAIST:
		limbName = "torso";
		stubCapName = "hips_cap_torso_off";
		break;
	case G2_MODELPART_LARM:
		limbName = "l_arm";
		stubCapName = "torso_cap_l_arm_off";
		break;
	case G2_MODELPART_RARM:
		limbName = "r_arm";
		stubCapName = "torso_cap_r_arm_off";
		break;
	case G2_MODELPART_LLEG:
		limbName = "l_leg";
		stubCapName = "hips_cap_l_leg_off";
		break;
	case G2_MODELPART_RLEG:
		limbName = "r_leg";
		stubCapName = "hips_cap_r_leg_off";
		break;
	default:
		limbName = "r_leg";
		stubCapName = "hips_cap_r_leg_off";
		break;
	}

	trap_G2API_SetSurfaceOnOff(source->ghoul2, limbName, 0);
	trap_G2API_SetSurfaceOnOff(source->ghoul2, stubCapName, 0x00000100);

	source->torsoBolt = 0;

	source->ghoul2weapon = NULL;
}

static void CG_BodyQueueCopy(centity_t *cent, int clientNum, int knownWeapon)
{
	centity_t		*source;
	animation_t		*anim;
	float			animSpeed;
	int				flags=BONE_ANIM_OVERRIDE_FREEZE;
	clientInfo_t	*ci;

	if (cent->ghoul2)
	{
		trap_G2API_CleanGhoul2Models(&cent->ghoul2);
	}

	if (clientNum < 0 || clientNum >= MAX_CLIENTS)
	{
		return;
	}

	source = &cg_entities[ clientNum ];
	ci = &cgs.clientinfo[ clientNum ];

	if (!source)
	{
		return;
	}

	if (!source->ghoul2)
	{
		return;
	}

	cent->isATST = source->isATST;

	cent->dustTrailTime = source->dustTrailTime;

	trap_G2API_DuplicateGhoul2Instance(source->ghoul2, &cent->ghoul2);

	//either force the weapon from when we died or remove it if it was a dropped weapon
	if (knownWeapon > WP_BRYAR_PISTOL && trap_G2API_HasGhoul2ModelOnIndex(&(cent->ghoul2), 1))
	{
		trap_G2API_RemoveGhoul2Model(&(cent->ghoul2), 1);
	}
	else if (trap_G2API_HasGhoul2ModelOnIndex(&(cent->ghoul2), 1))
	{
		trap_G2API_CopySpecificGhoul2Model(g2WeaponInstances[knownWeapon], 0, cent->ghoul2, 1);
	}

	anim = &bgGlobalAnimations[ cent->currentState.torsoAnim ];
	animSpeed = 50.0f / anim->frameLerp;

	//this will just set us to the last frame of the animation, in theory
	if (source->isATST)
	{
		int aNum = cgs.clientinfo[source->currentState.number].frame+1;
		anim = &bgGlobalAnimations[ BOTH_DEAD1 ];
		animSpeed = 1;

		flags &= ~BONE_ANIM_OVERRIDE_LOOP;

		while (aNum >= anim->firstFrame+anim->numFrames)
		{
			aNum--;
		}

		trap_G2API_SetBoneAnim(cent->ghoul2, 0, "pelvis", aNum, anim->firstFrame + anim->numFrames, flags, animSpeed, cg.time, -1, 150);
	}
	else
	{
		int aNum = cgs.clientinfo[source->currentState.number].frame+1;

		while (aNum >= anim->firstFrame+anim->numFrames)
		{
			aNum--;
		}

		if (aNum < anim->firstFrame-1)
		{ //wrong animation...?
			aNum = (anim->firstFrame+anim->numFrames)-1;
		}

		//if (!cgs.clientinfo[source->currentState.number].frame || (cent->currentState.torsoAnim&~ANIM_TOGGLEBIT) != (source->currentState.torsoAnim&~ANIM_TOGGLEBIT) )
		//{
		//	aNum = (anim->firstFrame+anim->numFrames)-1;
		//}

		trap_G2API_SetBoneAnim(cent->ghoul2, 0, "upper_lumbar", aNum, anim->firstFrame + anim->numFrames, flags, animSpeed, cg.time, -1, 150);
		trap_G2API_SetBoneAnim(cent->ghoul2, 0, "model_root", aNum, anim->firstFrame + anim->numFrames, flags, animSpeed, cg.time, -1, 150);
		trap_G2API_SetBoneAnim(cent->ghoul2, 0, "Motion", aNum, anim->firstFrame + anim->numFrames, flags, animSpeed, cg.time, -1, 150);
	}

	//After we create the bodyqueue, regenerate any limbs on the real instance
	if (source->torsoBolt)
	{
		CG_ReattachLimb(source);
	}
}

const char *CG_TeamName(int team)
{
	if (team==TEAM_RED)
		return "RED";
	else if (team==TEAM_BLUE)
		return "BLUE";
	else if (team==TEAM_SPECTATOR)
		return "SPECTATOR";
	return "FREE";
}

void CG_PrintCTFMessage(clientInfo_t *ci, const char *teamName, int ctfMessage)
{
	char printMsg[1024];
	char *refName = NULL;
	const char *stripEdString = NULL;

	switch (ctfMessage)
	{
	case CTFMESSAGE_FRAGGED_FLAG_CARRIER:
		refName = "FRAGGED_FLAG_CARRIER";
		break;
	case CTFMESSAGE_FLAG_RETURNED:
		refName = "FLAG_RETURNED";
		break;
	case CTFMESSAGE_PLAYER_RETURNED_FLAG:
		refName = "PLAYER_RETURNED_FLAG";
		break;
	case CTFMESSAGE_PLAYER_CAPTURED_FLAG:
		refName = "PLAYER_CAPTURED_FLAG";
		break;
	case CTFMESSAGE_PLAYER_GOT_FLAG:
		refName = "PLAYER_GOT_FLAG";
		break;
	default:
		return;
	}

	stripEdString = CG_GetStripEdString("INGAMETEXT", refName);

	if (!stripEdString || !stripEdString[0])
	{
		return;
	}

	if (teamName && teamName[0])
	{
		const char *f = strstr(stripEdString, "%s");

		if (f)
		{
			int strLen = 0;
			int i = 0;

			if (ci)
			{
				Com_sprintf(printMsg, sizeof(printMsg), "%s ", ci->name);
				strLen = strlen(printMsg);
			}

			while (stripEdString[i] && i < 512)
			{
				if (stripEdString[i] == '%' &&
					stripEdString[i+1] == 's')
				{
					printMsg[strLen] = '\0';
					Q_strcat(printMsg, sizeof(printMsg), teamName);
					strLen = strlen(printMsg);

					i++;
				}
				else
				{
					printMsg[strLen] = stripEdString[i];
					strLen++;
				}

				i++;
			}

			printMsg[strLen] = '\0';

			goto doPrint;
		}
	}

	if (ci)
	{
		Com_sprintf(printMsg, sizeof(printMsg), "%s %s", ci->name, stripEdString);
	}
	else
	{
		Com_sprintf(printMsg, sizeof(printMsg), "%s", stripEdString);
	}

doPrint:
	Com_Printf("%s\n", printMsg);
}

void CG_GetCTFMessageEvent(entityState_t *es)
{
	int clIndex = es->trickedentindex;
	int teamIndex = es->trickedentindex2;
	clientInfo_t *ci = NULL;
	const char *teamName = NULL;

	if (clIndex < MAX_CLIENTS)
	{
		ci = &cgs.clientinfo[clIndex];
	}

	if (teamIndex < 50)
	{
		teamName = CG_TeamName(teamIndex);
	}

	CG_PrintCTFMessage(ci, teamName, es->eventParm);
}

void DoFall(centity_t *cent, entityState_t *es, int clientNum)
{
	int delta = es->eventParm;

	if (cent->currentState.eFlags & EF_DEAD)
	{ //corpses crack into the ground ^_^
		if (delta > 25)
		{
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.fallSound );
		}
		else
		{
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, trap_S_RegisterSound( "sound/movers/objects/objectHit.wav" ) );
		}
	}
	else if (delta > 50)
	{
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.fallSound );
		trap_S_StartSound( NULL, cent->currentState.number, CHAN_VOICE, 
			CG_CustomSound( cent->currentState.number, "*land1.wav" ) );
		cent->pe.painTime = cg.time;	// don't play a pain sound right after this
	}
	else if (delta > 44)
	{
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.fallSound );
		trap_S_StartSound( NULL, cent->currentState.number, CHAN_VOICE, 
			CG_CustomSound( cent->currentState.number, "*land1.wav" ) );
		cent->pe.painTime = cg.time;	// don't play a pain sound right after this
	}
	else
	{
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.landSound );
	}
	
	if ( clientNum == cg.predictedPlayerState.clientNum )
	{
		// smooth landing z changes
		cg.landChange = -delta;
		if (cg.landChange > 32)
		{
			cg.landChange = 32;
		}
		if (cg.landChange < -32)
		{
			cg.landChange = -32;
		}
		cg.landTime = cg.time;
	}
}

int CG_InClientBitflags(entityState_t *ent, int client)
{
	int checkIn;
	int sub = 0;

	if (client > 47)
	{
		checkIn = ent->trickedentindex4;
		sub = 48;
	}
	else if (client > 31)
	{
		checkIn = ent->trickedentindex3;
		sub = 32;
	}
	else if (client > 15)
	{
		checkIn = ent->trickedentindex2;
		sub = 16;
	}
	else
	{
		checkIn = ent->trickedentindex;
	}

	if (checkIn & (1 << (client-sub)))
	{
		return 1;
	}
	
	return 0;
}

/*
==============
CG_EntityEvent

An entity has an event value
also called by CG_CheckPlayerstateEvents
==============
*/
#define	DEBUGNAME(x) if(cg_debugEvents.integer){CG_Printf(x"\n");}
void CG_EntityEvent( centity_t *cent, vec3_t position ) {
	entityState_t	*es;
	int				event;
	vec3_t			dir;
	const char		*s;
	int				clientNum;
	clientInfo_t	*ci;
	int				eID = 0;
	int				isnd = 0;
	centity_t		*cl_ent;

	es = &cent->currentState;
	event = es->event & ~EV_EVENT_BITS;

	if ( cg_debugEvents.integer ) {
		CG_Printf( "ent:%3i  event:%3i ", es->number, event );
	}

	if ( !event ) {
		DEBUGNAME("ZEROEVENT");
		return;
	}

	clientNum = es->clientNum;
	if ( clientNum < 0 || clientNum >= MAX_CLIENTS ) {
		clientNum = 0;
	}
	ci = &cgs.clientinfo[ clientNum ];

	switch ( event ) {
	//
	// movement generated events
	//
	case EV_CLIENTJOIN:
		DEBUGNAME("EV_CLIENTJOIN");

		//Slight hack to force a local reinit of client entity on join.
		cl_ent = &cg_entities[es->eventParm];

		if (cl_ent)
		{
			cl_ent->isATST = 0;
			cl_ent->atstFootClang = 0;
			cl_ent->atstSwinging = 0;
			cl_ent->torsoBolt = 0;
			cl_ent->bolt1 = 0;
			cl_ent->bolt2 = 0;
			cl_ent->bolt3 = 0;
			cl_ent->bolt4 = 0;
			cl_ent->saberLength = SABER_LENGTH_MAX;
			cl_ent->saberExtendTime = 0;
			cl_ent->boltInfo = 0;
			cl_ent->frame_minus1_refreshed = 0;
			cl_ent->frame_minus2_refreshed = 0;
			cl_ent->frame_hold_time = 0;
			cl_ent->frame_hold_refreshed = 0;
			cl_ent->trickAlpha = 0;
			cl_ent->trickAlphaTime = 0;
			cl_ent->ghoul2weapon = NULL;
			cl_ent->weapon = WP_NONE;
			cl_ent->teamPowerEffectTime = 0;
			cl_ent->teamPowerType = 0;
		}
		break;

	case EV_FOOTSTEP:
		DEBUGNAME("EV_FOOTSTEP");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ ci->footsteps ][rand()&3] );
		}
		break;
	case EV_FOOTSTEP_METAL:
		DEBUGNAME("EV_FOOTSTEP_METAL");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_METAL ][rand()&3] );
		}
		break;
	case EV_FOOTSPLASH:
		DEBUGNAME("EV_FOOTSPLASH");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_SPLASH ][rand()&3] );
		}
		break;
	case EV_FOOTWADE:
		DEBUGNAME("EV_FOOTWADE");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_SPLASH ][rand()&3] );
		}
		break;
	case EV_SWIM:
		DEBUGNAME("EV_SWIM");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_SPLASH ][rand()&3] );
		}
		break;


	case EV_FALL:
		DEBUGNAME("EV_FALL");
		if (es->number == cg.snap->ps.clientNum && cg.snap->ps.fallingToDeath)
		{
			break;
		}
		DoFall(cent, es, clientNum);
		break;
	case EV_STEP_4:
	case EV_STEP_8:
	case EV_STEP_12:
	case EV_STEP_16:		// smooth out step up transitions
		DEBUGNAME("EV_STEP");
	{
		float	oldStep;
		int		delta;
		int		step;

		if ( clientNum != cg.predictedPlayerState.clientNum ) {
			break;
		}
		// if we are interpolating, we don't need to smooth steps
		if ( cg.demoPlayback || (cg.snap->ps.pm_flags & PMF_FOLLOW) ||
			cg_nopredict.integer || cg_synchronousClients.integer ) {
			break;
		}
		// check for stepping up before a previous step is completed
		delta = cg.time - cg.stepTime;
		if (delta < STEP_TIME) {
			oldStep = cg.stepChange * (STEP_TIME - delta) / STEP_TIME;
		} else {
			oldStep = 0;
		}

		// add this amount
		step = 4 * (event - EV_STEP_4 + 1 );
		cg.stepChange = oldStep + step;
		if ( cg.stepChange > MAX_STEP_CHANGE ) {
			cg.stepChange = MAX_STEP_CHANGE;
		}
		cg.stepTime = cg.time;
		break;
	}

	case EV_JUMP_PAD:
		DEBUGNAME("EV_JUMP_PAD");
		break;

	case EV_PRIVATE_DUEL:
		DEBUGNAME("EV_PRIVATE_DUEL");

		if (cg.snap->ps.clientNum != es->number)
		{
			break;
		}

		if (es->eventParm)
		{ //starting the duel
			if (es->eventParm == 2)
			{
				CG_CenterPrint( CG_GetStripEdString("SVINGAME", "BEGIN_DUEL"), 120, GIANTCHAR_WIDTH*2 );				
				trap_S_StartLocalSound( cgs.media.countFightSound, CHAN_ANNOUNCER );
			}
			else
			{
				trap_S_StartBackgroundTrack( "music/mp/duel.mp3", "music/mp/duel.mp3", qfalse );
			}
		}
		else
		{ //ending the duel
			CG_StartMusic(qtrue);
		}
		break;

	case EV_JUMP:
		DEBUGNAME("EV_JUMP");
		trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*jump1.wav" ) );
		break;
	case EV_ROLL:
		DEBUGNAME("EV_ROLL");
		if (es->number == cg.snap->ps.clientNum && cg.snap->ps.fallingToDeath)
		{
			break;
		}
		if (es->eventParm)
		{ //fall-roll-in-one event
			DoFall(cent, es, clientNum);
		}

		trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*jump1.wav" ) );
		trap_S_StartSound( NULL, es->number, CHAN_BODY, cgs.media.rollSound  );

		//FIXME: need some sort of body impact on ground sound and maybe kick up some dust?
		break;

	case EV_TAUNT:
		DEBUGNAME("EV_TAUNT");
		trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*taunt.wav" ) );
		break;
	case EV_TAUNT_YES:
		DEBUGNAME("EV_TAUNT_YES");
		CG_VoiceChatLocal(SAY_TEAM, qfalse, es->number, COLOR_CYAN, VOICECHAT_YES);
		break;
	case EV_TAUNT_NO:
		DEBUGNAME("EV_TAUNT_NO");
		CG_VoiceChatLocal(SAY_TEAM, qfalse, es->number, COLOR_CYAN, VOICECHAT_NO);
		break;
	case EV_TAUNT_FOLLOWME:
		DEBUGNAME("EV_TAUNT_FOLLOWME");
		CG_VoiceChatLocal(SAY_TEAM, qfalse, es->number, COLOR_CYAN, VOICECHAT_FOLLOWME);
		break;
	case EV_TAUNT_GETFLAG:
		DEBUGNAME("EV_TAUNT_GETFLAG");
		CG_VoiceChatLocal(SAY_TEAM, qfalse, es->number, COLOR_CYAN, VOICECHAT_ONGETFLAG);
		break;
	case EV_TAUNT_GUARDBASE:
		DEBUGNAME("EV_TAUNT_GUARDBASE");
		CG_VoiceChatLocal(SAY_TEAM, qfalse, es->number, COLOR_CYAN, VOICECHAT_ONDEFENSE);
		break;
	case EV_TAUNT_PATROL:
		DEBUGNAME("EV_TAUNT_PATROL");
		CG_VoiceChatLocal(SAY_TEAM, qfalse, es->number, COLOR_CYAN, VOICECHAT_ONPATROL);
		break;
	case EV_WATER_TOUCH:
		DEBUGNAME("EV_WATER_TOUCH");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.watrInSound );
		break;
	case EV_WATER_LEAVE:
		DEBUGNAME("EV_WATER_LEAVE");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.watrOutSound );
		break;
	case EV_WATER_UNDER:
		DEBUGNAME("EV_WATER_UNDER");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.watrUnSound );
		break;
	case EV_WATER_CLEAR:
		DEBUGNAME("EV_WATER_CLEAR");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, CG_CustomSound( es->number, "*gasp.wav" ) );
		break;

	case EV_ITEM_PICKUP:
		DEBUGNAME("EV_ITEM_PICKUP");
		{
			gitem_t	*item;
			int		index;
			qboolean	newindex = qfalse;

			index = cg_entities[es->eventParm].currentState.modelindex;		// player predicted

			if (index < 1 && cg_entities[es->eventParm].currentState.isJediMaster)
			{ //a holocron most likely
				index = cg_entities[es->eventParm].currentState.trickedentindex4;
				trap_S_StartSound (NULL, es->number, CHAN_AUTO,	cgs.media.holocronPickup );
								
				if (es->number == cg.snap->ps.clientNum && showPowersName[index])
				{
					const char *strText = CG_GetStripEdString("INGAMETEXT", "PICKUPLINE");

					//Com_Printf("%s %s\n", strText, showPowersName[index]);
					CG_CenterPrint( va("%s %s\n", strText, showPowersName[index]), SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH );
				}

				//Show the player their force selection bar in case picking the holocron up changed the current selection
				if (index != FP_SABERATTACK && index != FP_SABERDEFEND && index != FP_SABERTHROW &&
					index != FP_LEVITATION &&
					es->number == cg.snap->ps.clientNum &&
					(index == cg.snap->ps.fd.forcePowerSelected || !(cg.snap->ps.fd.forcePowersActive & (1 << cg.snap->ps.fd.forcePowerSelected))))
				{
					if (cg.forceSelect != index)
					{
						cg.forceSelect = index;
						newindex = qtrue;
					}
				}

				if (es->number == cg.snap->ps.clientNum && newindex)
				{
					if (cg.forceSelectTime < cg.time)
					{
						cg.forceSelectTime = cg.time;
					}
				}

				break;
			}

			if (cg_entities[es->eventParm].weapon >= cg.time)
			{ //rww - an unfortunately necessary hack to prevent double item pickups
				break;
			}

			//Hopefully even if this entity is somehow removed and replaced with, say, another
			//item, this time will have expired by the time that item needs to be picked up.
			//Of course, it's quite possible this will fail miserably, so if you've got a better
			//solution then please do use it.
			cg_entities[es->eventParm].weapon = cg.time+500;

			if ( index < 1 || index >= bg_numItems ) {
				break;
			}
			item = &bg_itemlist[ index ];

			if ( /*item->giType != IT_POWERUP && */item->giType != IT_TEAM) {
				if (item->pickup_sound && item->pickup_sound[0])
				{
					trap_S_StartSound (NULL, es->number, CHAN_AUTO,	trap_S_RegisterSound( item->pickup_sound ) );
				}
			}

			// show icon and name on status bar
			if ( es->number == cg.snap->ps.clientNum ) {
				CG_ItemPickup( index );
			}
		}
		break;

	case EV_GLOBAL_ITEM_PICKUP:
		DEBUGNAME("EV_GLOBAL_ITEM_PICKUP");
		{
			gitem_t	*item;
			int		index;

			index = es->eventParm;		// player predicted

			if ( index < 1 || index >= bg_numItems ) {
				break;
			}
			item = &bg_itemlist[ index ];
			// powerup pickups are global
			if( item->pickup_sound && item->pickup_sound[0] ) {
				trap_S_StartSound (NULL, cg.snap->ps.clientNum, CHAN_AUTO, trap_S_RegisterSound( item->pickup_sound) );
			}

			// show icon and name on status bar
			if ( es->number == cg.snap->ps.clientNum ) {
				CG_ItemPickup( index );
			}
		}
		break;

	//
	// weapon events
	//
	case EV_NOAMMO:
		DEBUGNAME("EV_NOAMMO");
//		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.noAmmoSound );
		if ( es->number == cg.snap->ps.clientNum )
		{
			int weap = 0;

			if (es->eventParm && es->eventParm < WP_NUM_WEAPONS)
			{
				cg.snap->ps.stats[STAT_WEAPONS] &= ~(1 << es->eventParm);
				weap = cg.snap->ps.weapon;
			}
			else if (es->eventParm)
			{
				weap = (es->eventParm-WP_NUM_WEAPONS);
			}
			CG_OutOfAmmoChange(weap);
		}
		break;
	case EV_CHANGE_WEAPON:
		DEBUGNAME("EV_CHANGE_WEAPON");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.selectSound );
		break;
	case EV_FIRE_WEAPON:
		DEBUGNAME("EV_FIRE_WEAPON");
		if (cent->currentState.number >= MAX_CLIENTS)
		{ //special case for turret firing
			vec3_t gunpoint, gunangle;
			mdxaBone_t matrix;

			weaponInfo_t *weaponInfo = &cg_weapons[WP_TURRET];

			if ( !weaponInfo->registered )
			{
				memset( weaponInfo, 0, sizeof( *weaponInfo ) );

				weaponInfo->flashSound[0]		= NULL_SOUND;
				weaponInfo->firingSound			= NULL_SOUND;
				weaponInfo->chargeSound			= NULL_SOUND;
				weaponInfo->muzzleEffect		= NULL_HANDLE;
				weaponInfo->missileModel		= NULL_HANDLE;
				weaponInfo->missileSound		= NULL_SOUND;
				weaponInfo->missileDlight		= 0;
				weaponInfo->missileHitSound		= NULL_SOUND;
				weaponInfo->missileTrailFunc	= FX_TurretProjectileThink;

				trap_FX_RegisterEffect("effects/blaster/wall_impact.efx");
				trap_FX_RegisterEffect("effects/blaster/flesh_impact.efx");

				weaponInfo->registered = qtrue;
			}

			if (cent->ghoul2)
			{
				if (!cent->bolt1)
				{
					cent->bolt1 = trap_G2API_AddBolt(cent->ghoul2, 0, "*flash01");
				}
				if (!cent->bolt2)
				{
					cent->bolt2 = trap_G2API_AddBolt(cent->ghoul2, 0, "*flash02");
				}
			}
			else
			{
				break;
			}

			if (cent->currentState.eventParm)
			{
				trap_G2API_GetBoltMatrix(cent->ghoul2, 0, cent->bolt2, &matrix, cent->currentState.angles, cent->currentState.origin, cg.time, cgs.gameModels, cent->modelScale);
			}
			else
			{
				trap_G2API_GetBoltMatrix(cent->ghoul2, 0, cent->bolt1, &matrix, cent->currentState.angles, cent->currentState.origin, cg.time, cgs.gameModels, cent->modelScale);
			}

			gunpoint[0] = matrix.matrix[0][3];
			gunpoint[1] = matrix.matrix[1][3];
			gunpoint[2] = matrix.matrix[2][3];

			gunangle[0] = -matrix.matrix[0][0];
			gunangle[1] = -matrix.matrix[1][0];
			gunangle[2] = -matrix.matrix[2][0];

			trap_FX_PlayEffectID(trap_FX_RegisterEffect( "effects/turret/muzzle_flash.efx" ), gunpoint, gunangle);
		}
		else
		{
			CG_FireWeapon( cent, qfalse );
		}
		break;

	case EV_ALT_FIRE:
		DEBUGNAME("EV_ALT_FIRE");
		CG_FireWeapon( cent, qtrue );

		//if you just exploded your detpacks and you have no ammo left for them, autoswitch
		if ( cg.snap->ps.clientNum == cent->currentState.number &&
			cg.snap->ps.weapon == WP_DET_PACK )
		{
			if (cg.snap->ps.ammo[weaponData[WP_DET_PACK].ammoIndex] == 0) 
			{
				CG_OutOfAmmoChange(WP_DET_PACK);
			}
		}

		break;

	case EV_SABER_ATTACK:
		DEBUGNAME("EV_SABER_ATTACK");
		trap_S_StartSound(es->pos.trBase, es->number, CHAN_WEAPON, trap_S_RegisterSound(va("sound/weapons/saber/saberhup%i.wav", Q_irand(1, 8))));
		break;

	case EV_SABER_HIT:
		DEBUGNAME("EV_SABER_HIT");
		if (es->eventParm)
		{ //hit a person
			vec3_t fxDir;
			VectorCopy(es->angles, fxDir);
			if (!fxDir[0] && !fxDir[1] && !fxDir[2])
			{
				fxDir[1] = 1;
			}
			trap_S_StartSound(es->origin, es->number, CHAN_AUTO, trap_S_RegisterSound("sound/weapons/saber/saberhit.wav"));
			trap_FX_PlayEffectID( trap_FX_RegisterEffect("saber/blood_sparks.efx"), es->origin, fxDir );
		}
		else
		{ //hit something else
			vec3_t fxDir;
			VectorCopy(es->angles, fxDir);
			if (!fxDir[0] && !fxDir[1] && !fxDir[2])
			{
				fxDir[1] = 1;
			}			
			trap_S_StartSound(es->origin, es->number, CHAN_AUTO, trap_S_RegisterSound("sound/weapons/saber/saberhit.wav"));
			trap_FX_PlayEffectID( trap_FX_RegisterEffect("saber/spark.efx"), es->origin, fxDir );
		}
		break;

	case EV_SABER_BLOCK:
		DEBUGNAME("EV_SABER_BLOCK");

		if (es->eventParm)
		{ //saber block
			vec3_t fxDir;
			VectorCopy(es->angles, fxDir);
			if (!fxDir[0] && !fxDir[1] && !fxDir[2])
			{
				fxDir[1] = 1;
			}
			trap_S_StartSound(es->origin, es->number, CHAN_AUTO, trap_S_RegisterSound(va( "sound/weapons/saber/saberblock%d.wav", Q_irand(1, 9) )));
			trap_FX_PlayEffectID( trap_FX_RegisterEffect("saber/saber_block.efx"), es->origin, fxDir );

			g_saberFlashTime = cg.time-50;
			VectorCopy( es->origin, g_saberFlashPos );
		}
		else
		{ //projectile block
			vec3_t fxDir;
			VectorCopy(es->angles, fxDir);
			if (!fxDir[0] && !fxDir[1] && !fxDir[2])
			{
				fxDir[1] = 1;
			}
			trap_FX_PlayEffectID(trap_FX_RegisterEffect("blaster/deflect.efx"), es->origin, fxDir);
		}
		break;

	case EV_SABER_UNHOLSTER:
		DEBUGNAME("EV_SABER_UNHOLSTER");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, trap_S_RegisterSound( "sound/weapons/saber/saberon.wav" ) );
		break;

	case EV_BECOME_JEDIMASTER:
		DEBUGNAME("EV_SABER_UNHOLSTER");
		{
			trace_t tr;
			vec3_t playerMins = {-15, -15, DEFAULT_MINS_2+8};
			vec3_t playerMaxs = {15, 15, DEFAULT_MAXS_2};
			vec3_t ang, pos, dpos;

			VectorClear(ang);
			ang[ROLL] = 1;

			VectorCopy(position, dpos);
			dpos[2] -= 4096;

			CG_Trace(&tr, position, playerMins, playerMaxs, dpos, es->number, MASK_SOLID);
			VectorCopy(tr.endpos, pos);
			
			if (tr.fraction == 1)
			{
				break;
			}
			trap_FX_PlayEffectID(trap_FX_RegisterEffect("mp/jedispawn.efx"), pos, ang);

			trap_S_StartSound (NULL, es->number, CHAN_AUTO, trap_S_RegisterSound( "sound/weapons/saber/saberon.wav" ) );

			if (cg.snap->ps.clientNum == es->number)
			{
				trap_S_StartLocalSound(cgs.media.happyMusic, CHAN_LOCAL);
				CGCam_SetMusicMult(0.3, 5000);
			}
		}
		break;

	case EV_DISRUPTOR_MAIN_SHOT:
		DEBUGNAME("EV_DISRUPTOR_MAIN_SHOT");
		if (cent->currentState.eventParm != cg.snap->ps.clientNum ||
			cg.renderingThirdPerson)
		{ //h4q3ry
			CG_GetClientWeaponMuzzleBoltPoint(cent->currentState.eventParm, cent->currentState.origin2);
		}
		else
		{
			if (cg.lastFPFlashPoint[0] ||cg.lastFPFlashPoint[1] || cg.lastFPFlashPoint[2])
			{ //get the position of the muzzle flash for the first person weapon model from the last frame
				VectorCopy(cg.lastFPFlashPoint, cent->currentState.origin2);
			}
		}
		FX_DisruptorMainShot( cent->currentState.origin2, cent->lerpOrigin ); 
		break;

	case EV_DISRUPTOR_SNIPER_SHOT:
		DEBUGNAME("EV_DISRUPTOR_SNIPER_SHOT");
		if (cent->currentState.eventParm != cg.snap->ps.clientNum ||
			cg.renderingThirdPerson)
		{ //h4q3ry
			CG_GetClientWeaponMuzzleBoltPoint(cent->currentState.eventParm, cent->currentState.origin2);
		}
		else
		{
			if (cg.lastFPFlashPoint[0] ||cg.lastFPFlashPoint[1] || cg.lastFPFlashPoint[2])
			{ //get the position of the muzzle flash for the first person weapon model from the last frame
				VectorCopy(cg.lastFPFlashPoint, cent->currentState.origin2);
			}
		}
		FX_DisruptorAltShot( cent->currentState.origin2, cent->lerpOrigin, cent->currentState.shouldtarget );
		break;

	case EV_DISRUPTOR_SNIPER_MISS:
		DEBUGNAME("EV_DISRUPTOR_SNIPER_MISS");
		ByteToDir( es->eventParm, dir );
		if (es->weapon)
		{ //primary
			FX_DisruptorHitWall( cent->lerpOrigin, dir );
		}
		else
		{ //secondary
			FX_DisruptorAltMiss( cent->lerpOrigin, dir );
		}
		break;

	case EV_DISRUPTOR_HIT:
		DEBUGNAME("EV_DISRUPTOR_HIT");
		ByteToDir( es->eventParm, dir );
		if (es->weapon)
		{ //client
			FX_DisruptorHitPlayer( cent->lerpOrigin, dir, qtrue );
		}
		else
		{ //non-client
			FX_DisruptorHitWall( cent->lerpOrigin, dir );
		}
		break;

	case EV_DISRUPTOR_ZOOMSOUND:
		DEBUGNAME("EV_DISRUPTOR_ZOOMSOUND");
		if (es->number == cg.snap->ps.clientNum)
		{
			if (cg.snap->ps.zoomMode)
			{
				trap_S_StartLocalSound(trap_S_RegisterSound("sound/weapons/disruptor/zoomstart.wav"), CHAN_AUTO);
			}
			else
			{
				trap_S_StartLocalSound(trap_S_RegisterSound("sound/weapons/disruptor/zoomend.wav"), CHAN_AUTO);
			}
		}
		break;
	case EV_PREDEFSOUND:
		DEBUGNAME("EV_PREDEFSOUND");
		{
			int sID = -1;

			switch (es->eventParm)
			{
			case PDSOUND_PROTECTHIT:
				sID = trap_S_RegisterSound("sound/weapons/force/protecthit.mp3");
				break;
			case PDSOUND_PROTECT:
				sID = trap_S_RegisterSound("sound/weapons/force/protect.mp3");
				break;
			case PDSOUND_ABSORBHIT:
				sID = trap_S_RegisterSound("sound/weapons/force/absorbhit.mp3");
				break;
			case PDSOUND_ABSORB:
				sID = trap_S_RegisterSound("sound/weapons/force/absorb.mp3");
				break;
			case PDSOUND_FORCEJUMP:
				sID = trap_S_RegisterSound("sound/weapons/force/jump.mp3");
				break;
			case PDSOUND_FORCEGRIP:
				sID = trap_S_RegisterSound("sound/weapons/force/grip.mp3");
				break;
			default:
				break;
			}

			if (sID != 1)
			{
				trap_S_StartSound(es->origin, es->number, CHAN_AUTO, sID);
			}
		}
		break;

	case EV_TEAM_POWER:
		DEBUGNAME("EV_TEAM_POWER");
		{
			int clnum = 0;

			while (clnum < MAX_CLIENTS)
			{
				if (CG_InClientBitflags(es, clnum))
				{
					if (es->eventParm == 1)
					{ //eventParm 1 is heal
						trap_S_StartSound (NULL, clnum, CHAN_AUTO, cgs.media.teamHealSound );
						cg_entities[clnum].teamPowerEffectTime = cg.time + 1000;
						cg_entities[clnum].teamPowerType = 1;
					}
					else
					{ //eventParm 2 is force regen
						trap_S_StartSound (NULL, clnum, CHAN_AUTO, cgs.media.teamRegenSound );
						cg_entities[clnum].teamPowerEffectTime = cg.time + 1000;
						cg_entities[clnum].teamPowerType = 0;
					}
				}
				clnum++;
			}
		}
		break;

	case EV_SCREENSHAKE:
		DEBUGNAME("EV_SCREENSHAKE");
		if (!es->modelindex || cg.predictedPlayerState.clientNum == es->modelindex-1)
		{
			CGCam_Shake(es->angles[0], es->time);
		}
		break;
	case EV_USE_ITEM0:
		DEBUGNAME("EV_USE_ITEM0");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM1:
		DEBUGNAME("EV_USE_ITEM1");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM2:
		DEBUGNAME("EV_USE_ITEM2");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM3:
		DEBUGNAME("EV_USE_ITEM3");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM4:
		DEBUGNAME("EV_USE_ITEM4");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM5:
		DEBUGNAME("EV_USE_ITEM5");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM6:
		DEBUGNAME("EV_USE_ITEM6");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM7:
		DEBUGNAME("EV_USE_ITEM7");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM8:
		DEBUGNAME("EV_USE_ITEM8");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM9:
		DEBUGNAME("EV_USE_ITEM9");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM10:
		DEBUGNAME("EV_USE_ITEM10");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM11:
		DEBUGNAME("EV_USE_ITEM11");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM12:
		DEBUGNAME("EV_USE_ITEM12");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM13:
		DEBUGNAME("EV_USE_ITEM13");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM14:
		DEBUGNAME("EV_USE_ITEM14");
		CG_UseItem( cent );
		break;

	case EV_ITEMUSEFAIL:
		DEBUGNAME("EV_ITEMUSEFAIL");
		if (cg.snap->ps.clientNum == es->number)
		{
			char *stripedref = NULL;

			switch(es->eventParm)
			{
			case SENTRY_NOROOM:
				stripedref = (char *)CG_GetStripEdString("INGAMETEXT", "SENTRY_NOROOM");
				break;
			case SENTRY_ALREADYPLACED:
				stripedref = (char *)CG_GetStripEdString("INGAMETEXT", "SENTRY_ALREADYPLACED");
				break;
			case SHIELD_NOROOM:
				stripedref = (char *)CG_GetStripEdString("INGAMETEXT", "SHIELD_NOROOM");
				break;
			case SEEKER_ALREADYDEPLOYED:
				stripedref = (char *)CG_GetStripEdString("INGAMETEXT", "SEEKER_ALREADYDEPLOYED");
				break;
			default:
				break;
			}

			if (!stripedref)
			{
				break;
			}

			Com_Printf("%s\n", stripedref);
		}
		break;

	//=================================================================

	//
	// other events
	//
	case EV_PLAYER_TELEPORT_IN:
		DEBUGNAME("EV_PLAYER_TELEPORT_IN");
		{
			trace_t tr;
			vec3_t playerMins = {-15, -15, DEFAULT_MINS_2+8};
			vec3_t playerMaxs = {15, 15, DEFAULT_MAXS_2};
			vec3_t ang, pos, dpos;

			VectorClear(ang);
			ang[ROLL] = 1;

			VectorCopy(position, dpos);
			dpos[2] -= 4096;

			CG_Trace(&tr, position, playerMins, playerMaxs, dpos, es->number, MASK_SOLID);
			VectorCopy(tr.endpos, pos);
			
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.teleInSound );

			if (tr.fraction == 1)
			{
				break;
			}
			trap_FX_PlayEffectID(trap_FX_RegisterEffect("mp/spawn.efx"), pos, ang);
		}
		break;

	case EV_PLAYER_TELEPORT_OUT:
		DEBUGNAME("EV_PLAYER_TELEPORT_OUT");
		{
			trace_t tr;
			vec3_t playerMins = {-15, -15, DEFAULT_MINS_2+8};
			vec3_t playerMaxs = {15, 15, DEFAULT_MAXS_2};
			vec3_t ang, pos, dpos;

			VectorClear(ang);
			ang[ROLL] = 1;

			VectorCopy(position, dpos);
			dpos[2] -= 4096;

			CG_Trace(&tr, position, playerMins, playerMaxs, dpos, es->number, MASK_SOLID);
			VectorCopy(tr.endpos, pos);

			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.teleOutSound );

			if (tr.fraction == 1)
			{
				break;
			}
			trap_FX_PlayEffectID(trap_FX_RegisterEffect("mp/spawn.efx"), pos, ang);
		}
		break;

	case EV_ITEM_POP:
		DEBUGNAME("EV_ITEM_POP");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.respawnSound );
		break;
	case EV_ITEM_RESPAWN:
		DEBUGNAME("EV_ITEM_RESPAWN");
		cent->miscTime = cg.time;	// scale up from this
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.respawnSound );
		break;

	case EV_GRENADE_BOUNCE:
		DEBUGNAME("EV_GRENADE_BOUNCE");
		//Do something here?
		break;

	case EV_SCOREPLUM:
		DEBUGNAME("EV_SCOREPLUM");
		CG_ScorePlum( cent->currentState.otherEntityNum, cent->lerpOrigin, cent->currentState.time );
		break;

	case EV_CTFMESSAGE:
		DEBUGNAME("EV_CTFMESSAGE");
		CG_GetCTFMessageEvent(es);
		break;

	//
	// saga gameplay events
	//
	case EV_SAGA_ROUNDOVER:
		DEBUGNAME("EV_SAGA_ROUNDOVER");
		CG_SagaRoundOver(&cg_entities[cent->currentState.weapon], cent->currentState.eventParm);
		break;
	case EV_SAGA_OBJECTIVECOMPLETE:
		DEBUGNAME("EV_SAGA_OBJECTIVECOMPLETE");
		CG_SagaObjectiveCompleted(&cg_entities[cent->currentState.weapon], cent->currentState.eventParm, cent->currentState.trickedentindex);
		break;

	case EV_DESTROY_GHOUL2_INSTANCE:
		DEBUGNAME("EV_DESTROY_GHOUL2_INSTANCE");
		if (cg_entities[es->eventParm].ghoul2 && trap_G2_HaveWeGhoul2Models(cg_entities[es->eventParm].ghoul2))
		{
			if (es->eventParm < MAX_CLIENTS)
			{ //You try to do very bad thing!
#ifdef _DEBUG
				Com_Printf("WARNING: Tried to kill a client ghoul2 instance with a server event!\n");
#endif
				break;
			}
			trap_G2API_CleanGhoul2Models(&(cg_entities[es->eventParm].ghoul2));
		}
		break;

	case EV_DESTROY_WEAPON_MODEL:
		DEBUGNAME("EV_DESTROY_WEAPON_MODEL");
		if (cg_entities[es->eventParm].ghoul2 && trap_G2_HaveWeGhoul2Models(cg_entities[es->eventParm].ghoul2) &&
			trap_G2API_HasGhoul2ModelOnIndex(&(cg_entities[es->eventParm].ghoul2), 1))
		{
			trap_G2API_RemoveGhoul2Model(&(cg_entities[es->eventParm].ghoul2), 1);
		}
		break;

	case EV_GIVE_NEW_RANK:
		DEBUGNAME("EV_GIVE_NEW_RANK");
		if (es->trickedentindex == cg.snap->ps.clientNum)
		{
			trap_Cvar_Set("ui_rankChange", va("%i", es->eventParm));

			trap_Cvar_Set("ui_myteam", va("%i", es->bolt2));

			if (!( trap_Key_GetCatcher() & KEYCATCH_UI ) && !es->bolt1)
			{
				trap_OpenUIMenu(3);
			}
		}
		break;

	case EV_SET_FREE_SABER:
		DEBUGNAME("EV_SET_FREE_SABER");

		trap_Cvar_Set("ui_freeSaber", va("%i", es->eventParm));
		break;

	case EV_SET_FORCE_DISABLE:
		DEBUGNAME("EV_SET_FORCE_DISABLE");

		trap_Cvar_Set("ui_forcePowerDisable", va("%i", es->eventParm));
		break;

	//
	// missile impacts
	//
	case EV_MISSILE_STICK:
		DEBUGNAME("EV_MISSILE_STICK");
//		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.missileStick );
		break;

	case EV_MISSILE_HIT:
		DEBUGNAME("EV_MISSILE_HIT");
		ByteToDir( es->eventParm, dir );
		if (cent->currentState.eFlags & EF_ALT_FIRING)
		{
			CG_MissileHitPlayer( es->weapon, position, dir, es->otherEntityNum, qtrue);
		}
		else
		{
			CG_MissileHitPlayer( es->weapon, position, dir, es->otherEntityNum, qfalse);
		}
		break;

	case EV_MISSILE_MISS:
		DEBUGNAME("EV_MISSILE_MISS");
		ByteToDir( es->eventParm, dir );
		if (cent->currentState.eFlags & EF_ALT_FIRING)
		{
			CG_MissileHitWall(es->weapon, 0, position, dir, IMPACTSOUND_DEFAULT, qtrue, es->generic1);
		}
		else
		{
			CG_MissileHitWall(es->weapon, 0, position, dir, IMPACTSOUND_DEFAULT, qfalse, 0);
		}
		break;

	case EV_MISSILE_MISS_METAL:
		DEBUGNAME("EV_MISSILE_MISS_METAL");
		ByteToDir( es->eventParm, dir );
		if (cent->currentState.eFlags & EF_ALT_FIRING)
		{
			CG_MissileHitWall(es->weapon, 0, position, dir, IMPACTSOUND_METAL, qtrue, es->generic1);
		}
		else
		{
			CG_MissileHitWall(es->weapon, 0, position, dir, IMPACTSOUND_METAL, qfalse, 0);
		}
		break;

	case EV_PLAY_EFFECT:
		DEBUGNAME("EV_PLAY_EFFECT");
		switch(es->eventParm)
		{ //it isn't a hack, it's ingenuity!
		case EFFECT_SMOKE:
			eID = trap_FX_RegisterEffect("emplaced/dead_smoke.efx");
			break;
		case EFFECT_EXPLOSION:
			eID = trap_FX_RegisterEffect("emplaced/explode.efx");
			break;
		case EFFECT_EXPLOSION_PAS:
			eID = trap_FX_RegisterEffect("turret/explode.efx");
			break;
		case EFFECT_SPARK_EXPLOSION:
			eID = trap_FX_RegisterEffect("spark_explosion.efx");
			break;
		case EFFECT_EXPLOSION_TRIPMINE:
			eID = trap_FX_RegisterEffect("tripMine/explosion.efx");
			break;
		case EFFECT_EXPLOSION_DETPACK:
			eID = trap_FX_RegisterEffect("detpack/explosion.efx");
			break;
		case EFFECT_EXPLOSION_FLECHETTE:
			eID = trap_FX_RegisterEffect("flechette/alt_blow.efx");
			break;
		case EFFECT_STUNHIT:
			eID = trap_FX_RegisterEffect("stunBaton/flesh_impact.efx");
			break;
		case EFFECT_EXPLOSION_DEMP2ALT:
			FX_DEMP2_AltDetonate( cent->lerpOrigin, es->weapon );
			eID = trap_FX_RegisterEffect("demp2/altDetonate.efx");
			break;
		default:
			eID = -1;
			break;
		}

		if (eID != -1)
		{
			vec3_t fxDir;

			VectorCopy(es->angles, fxDir);

			if (!fxDir[0] && !fxDir[1] && !fxDir[2])
			{
				fxDir[1] = 1;
			}

			trap_FX_PlayEffectID(eID, es->origin, fxDir);
		}
		break;

	case EV_PLAY_EFFECT_ID:
		DEBUGNAME("EV_PLAY_EFFECT_ID");
		{
			vec3_t fxDir;

			AngleVectors(es->angles, fxDir, 0, 0);
			
			if (!fxDir[0] && !fxDir[1] && !fxDir[2])
			{
				fxDir[1] = 1;
			}

			if ( cgs.gameEffects[ es->eventParm ] )
			{
				trap_FX_PlayEffectID(cgs.gameEffects[es->eventParm], es->origin, fxDir );
			}
			else
			{
				s = CG_ConfigString( CS_EFFECTS + es->eventParm );
				if (s && s[0])
				{
					trap_FX_PlayEffectID(trap_FX_RegisterEffect(s), es->origin, fxDir );
				}
			}
		}
		break;

	case EV_MUTE_SOUND:
		DEBUGNAME("EV_MUTE_SOUND");
		if (cg_entities[es->trickedentindex2].currentState.eFlags & EF_SOUNDTRACKER)
		{
			cg_entities[es->trickedentindex2].currentState.eFlags -= EF_SOUNDTRACKER;
		}
		trap_S_MuteSound(es->trickedentindex2, es->trickedentindex);
		trap_S_StopLoopingSound(es->trickedentindex2);
		break;

	case EV_GENERAL_SOUND:
		DEBUGNAME("EV_GENERAL_SOUND");
		if (es->saberEntityNum == TRACK_CHANNEL_2 || es->saberEntityNum == TRACK_CHANNEL_3 ||
			es->saberEntityNum == TRACK_CHANNEL_5)
		{ //channels 2 and 3 are for speed and rage, 5 for sight
			if ( cgs.gameSounds[ es->eventParm ] )
			{
				trap_S_AddRealLoopingSound(es->number, es->pos.trBase, vec3_origin, cgs.gameSounds[ es->eventParm ] );
			}
		}
		else
		{
			if ( cgs.gameSounds[ es->eventParm ] ) {
				trap_S_StartSound (NULL, es->number, CHAN_VOICE, cgs.gameSounds[ es->eventParm ] );
			} else {
				s = CG_ConfigString( CS_SOUNDS + es->eventParm );
				trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, s ) );
			}
		}
		break;

	case EV_GLOBAL_SOUND:	// play from the player's head so it never diminishes
		DEBUGNAME("EV_GLOBAL_SOUND");
		if ( cgs.gameSounds[ es->eventParm ] ) {
			trap_S_StartSound (NULL, cg.snap->ps.clientNum, CHAN_AUTO, cgs.gameSounds[ es->eventParm ] );
		} else {
			s = CG_ConfigString( CS_SOUNDS + es->eventParm );
			trap_S_StartSound (NULL, cg.snap->ps.clientNum, CHAN_AUTO, CG_CustomSound( es->number, s ) );
		}
		break;

	case EV_GLOBAL_TEAM_SOUND:	// play from the player's head so it never diminishes
		{
			DEBUGNAME("EV_GLOBAL_TEAM_SOUND");
			switch( es->eventParm ) {
				case GTS_RED_CAPTURE: // CTF: red team captured the blue flag, 1FCTF: red team captured the neutral flag
					//CG_AddBufferedSound( cgs.media.redScoredSound );
					break;
				case GTS_BLUE_CAPTURE: // CTF: blue team captured the red flag, 1FCTF: blue team captured the neutral flag
					//CG_AddBufferedSound( cgs.media.blueScoredSound );
					break;
				case GTS_RED_RETURN: // CTF: blue flag returned, 1FCTF: never used
					if (cgs.gametype == GT_CTY)
					{
						CG_AddBufferedSound( cgs.media.blueYsalReturnedSound );
					}
					else
					{
						CG_AddBufferedSound( cgs.media.blueFlagReturnedSound );
					}
					break;
				case GTS_BLUE_RETURN: // CTF red flag returned, 1FCTF: neutral flag returned
					if (cgs.gametype == GT_CTY)
					{
						CG_AddBufferedSound( cgs.media.redYsalReturnedSound );
					}
					else
					{
						CG_AddBufferedSound( cgs.media.redFlagReturnedSound );
					}
					break;

				case GTS_RED_TAKEN: // CTF: red team took blue flag, 1FCTF: blue team took the neutral flag
					// if this player picked up the flag then a sound is played in CG_CheckLocalSounds
					if (cgs.gametype == GT_CTY)
					{
						CG_AddBufferedSound( cgs.media.redTookYsalSound );
					}
					else
					{
					 	CG_AddBufferedSound( cgs.media.redTookFlagSound );
					}
					break;
				case GTS_BLUE_TAKEN: // CTF: blue team took the red flag, 1FCTF red team took the neutral flag
					// if this player picked up the flag then a sound is played in CG_CheckLocalSounds
					if (cgs.gametype == GT_CTY)
					{
						CG_AddBufferedSound( cgs.media.blueTookYsalSound );
					}
					else
					{
						CG_AddBufferedSound( cgs.media.blueTookFlagSound );
					}
					break;
				case GTS_REDTEAM_SCORED:
					CG_AddBufferedSound(cgs.media.redScoredSound);
					break;
				case GTS_BLUETEAM_SCORED:
					CG_AddBufferedSound(cgs.media.blueScoredSound);
					break;
				case GTS_REDTEAM_TOOK_LEAD:
					CG_AddBufferedSound(cgs.media.redLeadsSound);
					break;
				case GTS_BLUETEAM_TOOK_LEAD:
					CG_AddBufferedSound(cgs.media.blueLeadsSound);
					break;
				case GTS_TEAMS_ARE_TIED:
					CG_AddBufferedSound( cgs.media.teamsTiedSound );
					break;
				default:
					break;
			}
			break;
		}

	case EV_ENTITY_SOUND:
		DEBUGNAME("EV_ENTITY_SOUND");
		//somewhat of a hack - weapon is the caller entity's index, trickedentindex is the proper sound channel
		if ( cgs.gameSounds[ es->eventParm ] ) {
			trap_S_StartSound (NULL, es->weapon, es->trickedentindex, cgs.gameSounds[ es->eventParm ] );
		} else {
			s = CG_ConfigString( CS_SOUNDS + es->eventParm );
			trap_S_StartSound (NULL, es->weapon, es->trickedentindex, CG_CustomSound( es->weapon, s ) );
		}
		break;

	case EV_PLAY_ROFF:
		DEBUGNAME("EV_PLAY_ROFF");
		trap_ROFF_Play(es->weapon, es->eventParm, es->trickedentindex);
		break;

	case EV_GLASS_SHATTER:
		DEBUGNAME("EV_GLASS_SHATTER");
		CG_GlassShatter(es->genericenemyindex, es->origin, es->angles, es->trickedentindex, es->pos.trTime);
		break;

	case EV_DEBRIS:
		DEBUGNAME("EV_DEBRIS");
		if (es->weapon)
		{
			if (cgs.gameSounds[es->weapon])
			{
				isnd = cgs.gameSounds[es->weapon];
			}
			else
			{
				s = CG_ConfigString( CS_SOUNDS + es->eventParm );
				isnd = CG_CustomSound( es->number, s );
			}
		}
		else
		{
			isnd = 0;
		}

		if (es->trickedentindex > 0)
		{
			if (cgs.gameModels[es->trickedentindex])
			{
				CG_CreateDebris(es->number, es->pos.trBase, es->angles, es->origin, isnd, cgs.gameModels[es->trickedentindex]);
			}
			else
			{ //default to "rock" type
				CG_CreateDebris(es->number, es->pos.trBase, es->angles, es->origin, isnd, -1);
			}
		}
		else
		{
			CG_CreateDebris(es->number, es->pos.trBase, es->angles, es->origin, isnd, es->trickedentindex);
		}
		break;

	case EV_PAIN:
		// local player sounds are triggered in CG_CheckLocalSounds,
		// so ignore events on the player
		DEBUGNAME("EV_PAIN");
		if ( cent->currentState.number != cg.snap->ps.clientNum ) {
			CG_PainEvent( cent, es->eventParm );
		}
		break;

	case EV_DEATH1:
	case EV_DEATH2:
	case EV_DEATH3:
		DEBUGNAME("EV_DEATHx");
		trap_S_StartSound( NULL, es->number, CHAN_VOICE, 
				CG_CustomSound( es->number, va("*death%i.wav", event - EV_DEATH1 + 1) ) );
		if (es->eventParm && es->number == cg.snap->ps.clientNum)
		{
			trap_S_StartLocalSound(cgs.media.dramaticFailure, CHAN_LOCAL);
			CGCam_SetMusicMult(0.3, 5000);
		}
		break;


	case EV_OBITUARY:
		DEBUGNAME("EV_OBITUARY");
		CG_Obituary( es );
		break;

	//
	// powerup events
	//
	case EV_POWERUP_QUAD:
		DEBUGNAME("EV_POWERUP_QUAD");
		if ( es->number == cg.snap->ps.clientNum ) {
			cg.powerupActive = PW_QUAD;
			cg.powerupTime = cg.time;
		}
		//trap_S_StartSound (NULL, es->number, CHAN_ITEM, cgs.media.quadSound );
		break;
	case EV_POWERUP_BATTLESUIT:
		DEBUGNAME("EV_POWERUP_BATTLESUIT");
		if ( es->number == cg.snap->ps.clientNum ) {
			cg.powerupActive = PW_BATTLESUIT;
			cg.powerupTime = cg.time;
		}
		//trap_S_StartSound (NULL, es->number, CHAN_ITEM, cgs.media.protectSound );
		break;

	case EV_FORCE_DRAINED:
		DEBUGNAME("EV_FORCE_DRAINED");
		ByteToDir( es->eventParm, dir );
		//FX_ForceDrained(position, dir);
		trap_S_StartSound (NULL, es->owner, CHAN_AUTO, cgs.media.drainSound );
		cg_entities[es->owner].teamPowerEffectTime = cg.time + 1000;
		cg_entities[es->owner].teamPowerType = 2;
		break;

	case EV_GIB_PLAYER:
		DEBUGNAME("EV_GIB_PLAYER");
		//trap_S_StartSound( NULL, es->number, CHAN_BODY, cgs.media.gibSound );
		CG_GibPlayer( cent->lerpOrigin );
		break;

	case EV_STARTLOOPINGSOUND:
		DEBUGNAME("EV_STARTLOOPINGSOUND");
		if ( cgs.gameSounds[ es->eventParm ] )
		{
			isnd = cgs.gameSounds[es->eventParm];
		}
		else
		{
			s = CG_ConfigString( CS_SOUNDS + es->eventParm );
			isnd = CG_CustomSound(es->number, s);
		}

		trap_S_AddRealLoopingSound( es->number, es->pos.trBase, vec3_origin, isnd );
		es->loopSound = isnd;
		break;

	case EV_STOPLOOPINGSOUND:
		DEBUGNAME("EV_STOPLOOPINGSOUND");
		trap_S_StopLoopingSound( es->number );
		es->loopSound = 0;
		break;

	case EV_WEAPON_CHARGE:
		DEBUGNAME("EV_WEAPON_CHARGE");
		assert(es->eventParm > WP_NONE && es->eventParm < WP_NUM_WEAPONS);
		if (cg_weapons[es->eventParm].chargeSound)
		{
			trap_S_StartSound(NULL, es->number, CHAN_WEAPON, cg_weapons[es->eventParm].chargeSound);
		}
		break;

	case EV_WEAPON_CHARGE_ALT:
		DEBUGNAME("EV_WEAPON_CHARGE_ALT");
		assert(es->eventParm > WP_NONE && es->eventParm < WP_NUM_WEAPONS);
		if (cg_weapons[es->eventParm].altChargeSound)
		{
			trap_S_StartSound(NULL, es->number, CHAN_WEAPON, cg_weapons[es->eventParm].altChargeSound);
		}
		break;

	case EV_SHIELD_HIT:
		DEBUGNAME("EV_SHIELD_HIT");
		ByteToDir(es->eventParm, dir);
		CG_PlayerShieldHit(es->otherEntityNum, dir, es->time2);
		break;

	case EV_DEBUG_LINE:
		DEBUGNAME("EV_DEBUG_LINE");
		CG_Beam( cent );
		break;

	case EV_TESTLINE:
		DEBUGNAME("EV_TESTLINE");
		CG_TestLine(es->origin, es->origin2, es->time2, es->weapon, 1);
		break;

	case EV_BODY_QUEUE_COPY:
		DEBUGNAME("EV_BODY_QUEUE_COPY");
		CG_BodyQueueCopy(cent, es->eventParm, es->weapon);
		break;

	default:
		DEBUGNAME("UNKNOWN");
		CG_Error( "Unknown event: %i", event );
		break;
	}

}


/*
==============
CG_CheckEvents

==============
*/
void CG_CheckEvents( centity_t *cent ) {
	// check for event-only entities
	if ( cent->currentState.eType > ET_EVENTS ) {
		if ( cent->previousEvent ) {
			return;	// already fired
		}
		// if this is a player event set the entity number of the client entity number
		if ( cent->currentState.eFlags & EF_PLAYER_EVENT ) {
			cent->currentState.number = cent->currentState.otherEntityNum;
		}

		cent->previousEvent = 1;

		cent->currentState.event = cent->currentState.eType - ET_EVENTS;
	} else {
		// check for events riding with another entity
		if ( cent->currentState.event == cent->previousEvent ) {
			return;
		}
		cent->previousEvent = cent->currentState.event;
		if ( ( cent->currentState.event & ~EV_EVENT_BITS ) == 0 ) {
			return;
		}
	}

	// calculate the position at exactly the frame time
	BG_EvaluateTrajectory( &cent->currentState.pos, cg.snap->serverTime, cent->lerpOrigin );
	CG_SetEntitySoundPosition( cent );

	CG_EntityEvent( cent, cent->lerpOrigin );
}

