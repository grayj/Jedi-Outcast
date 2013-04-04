// BG_PAnimate.c

#include "q_shared.h"
#include "bg_public.h"
#include "bg_local.h"
#include "anims.h"
#include "../cgame/animtable.h"

/*
==============================================================================
BEGIN: Animation utility functions (sequence checking)
==============================================================================
*/
//Called regardless of pm validity:
qboolean BG_InSpecialJump( int anim )
{
	switch ( (anim&~ANIM_TOGGLEBIT) )
	{
	case BOTH_WALL_RUN_RIGHT:
	case BOTH_WALL_RUN_RIGHT_FLIP:
	case BOTH_WALL_RUN_LEFT:
	case BOTH_WALL_RUN_LEFT_FLIP:
	case BOTH_WALL_FLIP_RIGHT:
	case BOTH_WALL_FLIP_LEFT:
	case BOTH_FLIP_BACK1:
	case BOTH_FLIP_BACK2:
	case BOTH_FLIP_BACK3:
	case BOTH_WALL_FLIP_BACK1:
	case BOTH_BUTTERFLY_LEFT:
	case BOTH_BUTTERFLY_RIGHT:
		return qtrue;
	}
	return qfalse;
}

qboolean BG_InSaberStandAnim( int anim )
{
	switch ( (anim&~ANIM_TOGGLEBIT) )
	{
	case BOTH_SABERFAST_STANCE:
	case BOTH_STAND2:
	case BOTH_SABERSLOW_STANCE:
		return qtrue;
	default:
		return qfalse;
	}
}

qboolean BG_DirectFlippingAnim( int anim )
{
	switch ( (anim&~ANIM_TOGGLEBIT) )
	{
	case BOTH_FLIP_F:			//# Flip forward
	case BOTH_FLIP_B:			//# Flip backwards
	case BOTH_FLIP_L:			//# Flip left
	case BOTH_FLIP_R:			//# Flip right
		return qtrue;
		break;
	}

	return qfalse;
}

qboolean BG_SaberInAttack( int move )
{
	if ( move >= LS_A_TL2BR && move <= LS_A_T2B )
	{
		return qtrue;
	}
	switch ( move )
	{
	case LS_A_BACK:
	case LS_A_BACK_CR:
	case LS_A_BACKSTAB:
	case LS_A_LUNGE:
	case LS_A_JUMP_T__B_:
	case LS_A_FLIP_STAB:
	case LS_A_FLIP_SLASH:
		return qtrue;
		break;
	}
	return qfalse;
}

qboolean BG_SaberInSpecial( int move )
{
	switch( move )
	{
	case LS_A_BACK:
	case LS_A_BACK_CR:
	case LS_A_BACKSTAB:
	case LS_A_LUNGE:
	case LS_A_JUMP_T__B_:
	case LS_A_FLIP_STAB:
	case LS_A_FLIP_SLASH:
		return qtrue;
	}
	return qfalse;
}

qboolean BG_SaberInIdle( int move )
{
	switch ( move )
	{
	case LS_NONE:
	case LS_READY:
	case LS_DRAW:
	case LS_PUTAWAY:
		return qtrue;
		break;
	}
	return qfalse;
}

qboolean BG_FlippingAnim( int anim )
{
	switch ( anim )
	{
	case BOTH_FLIP_F:			//# Flip forward
	case BOTH_FLIP_B:			//# Flip backwards
	case BOTH_FLIP_L:			//# Flip left
	case BOTH_FLIP_R:			//# Flip right
	case BOTH_WALL_RUN_RIGHT_FLIP:
	case BOTH_WALL_RUN_LEFT_FLIP:
	case BOTH_WALL_FLIP_RIGHT:
	case BOTH_WALL_FLIP_LEFT:
	case BOTH_FLIP_BACK1:
	case BOTH_FLIP_BACK2:
	case BOTH_FLIP_BACK3:
	case BOTH_WALL_FLIP_BACK1:
	//Not really flips, but...
	case BOTH_WALL_RUN_RIGHT:
	case BOTH_WALL_RUN_LEFT:
	case BOTH_WALL_RUN_RIGHT_STOP:
	case BOTH_WALL_RUN_LEFT_STOP:
	case BOTH_BUTTERFLY_LEFT:
	case BOTH_BUTTERFLY_RIGHT:
	//
	case BOTH_ARIAL_LEFT:
	case BOTH_ARIAL_RIGHT:
	case BOTH_ARIAL_F1:
	case BOTH_CARTWHEEL_LEFT:
	case BOTH_CARTWHEEL_RIGHT:
	case BOTH_JUMPFLIPSLASHDOWN1:
	case BOTH_JUMPFLIPSTABDOWN:
		return qtrue;
		break;
	}
	return qfalse;
}

qboolean BG_SpinningSaberAnim( int anim )
{
	switch ( anim )
	{
	//level 1 - FIXME: level 1 will have *no* spins
	case BOTH_T1_BR_BL:
	case BOTH_T1__R__L:
	case BOTH_T1__R_BL:
	case BOTH_T1_TR_BL:
	case BOTH_T1_BR_TL:
	case BOTH_T1_BR__L:
	case BOTH_T1_TL_BR:
	case BOTH_T1__L_BR:
	case BOTH_T1__L__R:
	case BOTH_T1_BL_BR:
	case BOTH_T1_BL__R:
	case BOTH_T1_BL_TR:
	//level 2
	case BOTH_T2_BR__L:
	case BOTH_T2_BR_BL:
	case BOTH_T2__R_BL:
	case BOTH_T2__L_BR:
	case BOTH_T2_BL_BR:
	case BOTH_T2_BL__R:
	//level 3
	case BOTH_T3_BR__L:
	case BOTH_T3_BR_BL:
	case BOTH_T3__R_BL:
	case BOTH_T3__L_BR:
	case BOTH_T3_BL_BR:
	case BOTH_T3_BL__R:
	//level 4
	case BOTH_T4_BR__L:
	case BOTH_T4_BR_BL:
	case BOTH_T4__R_BL:
	case BOTH_T4__L_BR:
	case BOTH_T4_BL_BR:
	case BOTH_T4_BL__R:
	//level 5
	case BOTH_T5_BR_BL:
	case BOTH_T5__R__L:
	case BOTH_T5__R_BL:
	case BOTH_T5_TR_BL:
	case BOTH_T5_BR_TL:
	case BOTH_T5_BR__L:
	case BOTH_T5_TL_BR:
	case BOTH_T5__L_BR:
	case BOTH_T5__L__R:
	case BOTH_T5_BL_BR:
	case BOTH_T5_BL__R:
	case BOTH_T5_BL_TR:
	//special
	//case BOTH_A2_STABBACK1:
	case BOTH_ATTACK_BACK:
	case BOTH_CROUCHATTACKBACK1:
	case BOTH_BUTTERFLY_LEFT:
	case BOTH_BUTTERFLY_RIGHT:
	case BOTH_FJSS_TR_BL:
	case BOTH_FJSS_TL_BR:
	case BOTH_JUMPFLIPSLASHDOWN1:
	case BOTH_JUMPFLIPSTABDOWN:
		return qtrue;
		break;
	}
	return qfalse;
}

qboolean BG_SaberInSpecialAttack( int anim )
{
	switch ( anim&~ANIM_TOGGLEBIT )
	{
	case BOTH_A2_STABBACK1:
	case BOTH_ATTACK_BACK:
	case BOTH_CROUCHATTACKBACK1:
	case BOTH_BUTTERFLY_LEFT:
	case BOTH_BUTTERFLY_RIGHT:
	case BOTH_FJSS_TR_BL:
	case BOTH_FJSS_TL_BR:
	case BOTH_LUNGE2_B__T_:
	case BOTH_FORCELEAP2_T__B_:
	case BOTH_JUMPFLIPSLASHDOWN1://#
	case BOTH_JUMPFLIPSTABDOWN://#
		return qtrue;
	}
	return qfalse;
}

qboolean BG_InRoll( playerState_t *ps, int anim )
{
	switch ( (anim&~ANIM_TOGGLEBIT) )
	{
	case BOTH_ROLL_F:
	case BOTH_ROLL_B:
	case BOTH_ROLL_R:
	case BOTH_ROLL_L:
		if ( ps->legsTimer > 0 )
		{
			return qtrue;
		}
		break;
	}
	return qfalse;
}

qboolean BG_InDeathAnim( int anim )
{
	switch((anim&~ANIM_TOGGLEBIT))
	{
	case BOTH_DIVE1:
	case BOTH_DEATHBACKWARD1:
	case BOTH_DEATHBACKWARD2:
	case BOTH_DEATHFORWARD1:
	case BOTH_DEATHFORWARD2:
	case BOTH_DEATH1:
	case BOTH_DEATH2:
	case BOTH_DEATH3:
	case BOTH_DEATH4:
	case BOTH_DEATH5:
	case BOTH_DEATH6:
	case BOTH_DEATH7:

	case BOTH_DEATH1IDLE:
	case BOTH_LYINGDEATH1:
	case BOTH_STUMBLEDEATH1:
	case BOTH_FALLDEATH1:
	case BOTH_FALLDEATH1INAIR:
	case BOTH_FALLDEATH1LAND:
		return qtrue;
		break;
	default:
		return qfalse;
		break;
	}
}

//Called only where pm is valid (not all require pm, but some do):
qboolean PM_SaberInParry( int move )
{
	if ( move >= LS_PARRY_UP && move <= LS_PARRY_LL )
	{
		return qtrue;
	}
	return qfalse;
}

qboolean PM_SaberInReflect( int move )
{
	if ( move >= LS_REFLECT_UP && move <= LS_REFLECT_LL )
	{
		return qtrue;
	}
	return qfalse;
}

qboolean PM_SaberInStart( int move )
{
	if ( move >= LS_S_TL2BR && move <= LS_S_T2B )
	{
		return qtrue;
	}
	return qfalse;
}

qboolean PM_InSaberAnim( int anim )
{
	if ( (anim&~ANIM_TOGGLEBIT) >= BOTH_A1_T__B_ && (anim&~ANIM_TOGGLEBIT) <= BOTH_H1_S1_BR )
	{
		return qtrue;
	}
	return qfalse;
}

qboolean PM_InKnockDown( playerState_t *ps )
{
	switch ( (ps->legsAnim&~ANIM_TOGGLEBIT) )
	{
	case BOTH_KNOCKDOWN1:
	case BOTH_KNOCKDOWN2:
	case BOTH_KNOCKDOWN3:
	case BOTH_KNOCKDOWN4:
	case BOTH_KNOCKDOWN5:
		return qtrue;
		break;
	case BOTH_GETUP1:
	case BOTH_GETUP2:
	case BOTH_GETUP3:
	case BOTH_GETUP4:
	case BOTH_GETUP5:
	case BOTH_FORCE_GETUP_F1:
	case BOTH_FORCE_GETUP_F2:
	case BOTH_FORCE_GETUP_B1:
	case BOTH_FORCE_GETUP_B2:
	case BOTH_FORCE_GETUP_B3:
	case BOTH_FORCE_GETUP_B4:
	case BOTH_FORCE_GETUP_B5:
		if ( ps->legsTimer )
		{
			return qtrue;
		}
		break;
	}
	return qfalse;
}

qboolean PM_PainAnim( int anim )
{
	switch ( (anim&~ANIM_TOGGLEBIT) )
	{
		case BOTH_PAIN1:				//# First take pain anim
		case BOTH_PAIN2:				//# Second take pain anim
		case BOTH_PAIN3:				//# Third take pain anim
		case BOTH_PAIN4:				//# Fourth take pain anim
		case BOTH_PAIN5:				//# Fifth take pain anim - from behind
		case BOTH_PAIN6:				//# Sixth take pain anim - from behind
		case BOTH_PAIN7:				//# Seventh take pain anim - from behind
		case BOTH_PAIN8:				//# Eigth take pain anim - from behind
		case BOTH_PAIN9:				//# 
		case BOTH_PAIN10:			//# 
		case BOTH_PAIN11:			//# 
		case BOTH_PAIN12:			//# 
		case BOTH_PAIN13:			//# 
		case BOTH_PAIN14:			//# 
		case BOTH_PAIN15:			//# 
		case BOTH_PAIN16:			//# 
		case BOTH_PAIN17:			//# 
		case BOTH_PAIN18:			//# 
		case BOTH_PAIN19:			//# 
		return qtrue;
		break;
	}
	return qfalse;
}

qboolean PM_JumpingAnim( int anim )
{
	switch ( (anim&~ANIM_TOGGLEBIT) )
	{
		case BOTH_JUMP1:				//# Jump - wind-up and leave ground
		case BOTH_INAIR1:			//# In air loop (from jump)
		case BOTH_LAND1:				//# Landing (from in air loop)
		case BOTH_LAND2:				//# Landing Hard (from a great height)
		case BOTH_JUMPBACK1:			//# Jump backwards - wind-up and leave ground
		case BOTH_INAIRBACK1:		//# In air loop (from jump back)
		case BOTH_LANDBACK1:			//# Landing backwards(from in air loop)
		case BOTH_JUMPLEFT1:			//# Jump left - wind-up and leave ground
		case BOTH_INAIRLEFT1:		//# In air loop (from jump left)
		case BOTH_LANDLEFT1:			//# Landing left(from in air loop)
		case BOTH_JUMPRIGHT1:		//# Jump right - wind-up and leave ground
		case BOTH_INAIRRIGHT1:		//# In air loop (from jump right)
		case BOTH_LANDRIGHT1:		//# Landing right(from in air loop)
		case BOTH_FORCEJUMP1:				//# Jump - wind-up and leave ground
		case BOTH_FORCEINAIR1:			//# In air loop (from jump)
		case BOTH_FORCELAND1:				//# Landing (from in air loop)
		case BOTH_FORCEJUMPBACK1:			//# Jump backwards - wind-up and leave ground
		case BOTH_FORCEINAIRBACK1:		//# In air loop (from jump back)
		case BOTH_FORCELANDBACK1:			//# Landing backwards(from in air loop)
		case BOTH_FORCEJUMPLEFT1:			//# Jump left - wind-up and leave ground
		case BOTH_FORCEINAIRLEFT1:		//# In air loop (from jump left)
		case BOTH_FORCELANDLEFT1:			//# Landing left(from in air loop)
		case BOTH_FORCEJUMPRIGHT1:		//# Jump right - wind-up and leave ground
		case BOTH_FORCEINAIRRIGHT1:		//# In air loop (from jump right)
		case BOTH_FORCELANDRIGHT1:		//# Landing right(from in air loop)
		return qtrue;
		break;
	}
	return qfalse;
}

qboolean PM_LandingAnim( int anim )
{
	switch ( (anim&~ANIM_TOGGLEBIT) )
	{
		case BOTH_LAND1:				//# Landing (from in air loop)
		case BOTH_LAND2:				//# Landing Hard (from a great height)
		case BOTH_LANDBACK1:			//# Landing backwards(from in air loop)
		case BOTH_LANDLEFT1:			//# Landing left(from in air loop)
		case BOTH_LANDRIGHT1:		//# Landing right(from in air loop)
		case BOTH_FORCELAND1:		//# Landing (from in air loop)
		case BOTH_FORCELANDBACK1:	//# Landing backwards(from in air loop)
		case BOTH_FORCELANDLEFT1:	//# Landing left(from in air loop)
		case BOTH_FORCELANDRIGHT1:	//# Landing right(from in air loop)
		return qtrue;
		break;
	}
	return qfalse;
}

qboolean PM_SpinningAnim( int anim )
{
	/*
	switch ( anim )
	{
	//FIXME: list any other spinning anims
	default:
		break;
	}
	*/
	return BG_SpinningSaberAnim( anim );
}

qboolean PM_InOnGroundAnim ( int anim )
{
	switch( anim&~ANIM_TOGGLEBIT )
	{
	case BOTH_DEAD1:
	case BOTH_DEAD2:
	case BOTH_DEAD3:
	case BOTH_DEAD4:
	case BOTH_DEAD5:
	case BOTH_DEADFORWARD1:
	case BOTH_DEADBACKWARD1:
	case BOTH_DEADFORWARD2:
	case BOTH_DEADBACKWARD2:
	case BOTH_LYINGDEATH1:
	case BOTH_LYINGDEAD1:
	case BOTH_PAIN2WRITHE1:		//# Transition from upright position to writhing on ground anim
	case BOTH_WRITHING1:			//# Lying on ground writhing in pain
	case BOTH_WRITHING1RLEG:		//# Lying on ground writhing in pain: holding right leg
	case BOTH_WRITHING1LLEG:		//# Lying on ground writhing in pain: holding left leg
	case BOTH_WRITHING2:			//# Lying on stomache writhing in pain
	case BOTH_INJURED1:			//# Lying down: against wall - can also be sleeping
	case BOTH_CRAWLBACK1:			//# Lying on back: crawling backwards with elbows
	case BOTH_INJURED2:			//# Injured pose 2
	case BOTH_INJURED3:			//# Injured pose 3
	case BOTH_INJURED6:			//# Injured pose 6
	case BOTH_INJURED6ATTACKSTART:	//# Start attack while in injured 6 pose 
	case BOTH_INJURED6ATTACKSTOP:	//# End attack while in injured 6 pose
	case BOTH_INJURED6COMBADGE:	//# Hit combadge while in injured 6 pose
	case BOTH_INJURED6POINT:		//# Chang points to door while in injured state
	case BOTH_KNOCKDOWN1:		//# 
	case BOTH_KNOCKDOWN2:		//# 
		return qtrue;
		break;
	}

	return qfalse;
}

qboolean PM_InRollComplete( playerState_t *ps, int anim )
{
	switch ( (anim&~ANIM_TOGGLEBIT) )
	{
	case BOTH_ROLL_F:
	case BOTH_ROLL_B:
	case BOTH_ROLL_R:
	case BOTH_ROLL_L:
		if ( ps->legsTimer < 1 )
		{
			return qtrue;
		}
		break;
	}
	return qfalse;
}

int PM_AnimLength( int index, animNumber_t anim )
{
	if (anim >= MAX_ANIMATIONS)
	{
		return -1;
	}
	return pm->animations[anim].numFrames * fabs(pm->animations[anim].frameLerp);
}

void PM_DebugLegsAnim(int anim)
{
	int oldAnim = (pm->ps->legsAnim & ~ANIM_TOGGLEBIT);
	int newAnim = (anim & ~ANIM_TOGGLEBIT);

	if (oldAnim < MAX_TOTALANIMATIONS && oldAnim >= BOTH_DEATH1 &&
		newAnim < MAX_TOTALANIMATIONS && newAnim >= BOTH_DEATH1)
	{
		Com_Printf("OLD: %s\n", animTable[oldAnim]);
		Com_Printf("NEW: %s\n", animTable[newAnim]);
	}
}
/*
==============================================================================
END: Animation utility functions (sequence checking)
==============================================================================
*/

/*
======================
BG_ParseAnimationFile

Read a configuration file containing animation coutns and rates
models/players/visor/animation.cfg, etc

======================
*/
char		BGPAFtext[40000];
qboolean	BGPAFtextLoaded = qfalse;
animation_t	bgGlobalAnimations[MAX_TOTALANIMATIONS];

qboolean BG_ParseAnimationFile(const char *filename) 
{
	char		*text_p;
	int			len;
	int			i;
	char		*token;
	float		fps;
	int			skip;

	fileHandle_t	f;
	int				animNum;


	// load the file
	if (!BGPAFtextLoaded)
	{ //rww - We are always using the same animation config now. So only load it once.
		len = trap_FS_FOpenFile( filename, &f, FS_READ );
		if ( len <= 0 ) 
		{
			return qfalse;
		}
		if ( len >= sizeof( BGPAFtext ) - 1 ) 
		{
			//Com_Printf( "File %s too long\n", filename );
			return qfalse;
		}

		trap_FS_Read( BGPAFtext, len, f );
		BGPAFtext[len] = 0;
		trap_FS_FCloseFile( f );
	}
	else
	{
		return qtrue;
	}

	// parse the text
	text_p = BGPAFtext;
	skip = 0;	// quiet the compiler warning

	//FIXME: have some way of playing anims backwards... negative numFrames?

	//initialize anim array so that from 0 to MAX_ANIMATIONS, set default values of 0 1 0 100
	for(i = 0; i < MAX_ANIMATIONS; i++)
	{
		bgGlobalAnimations[i].firstFrame = 0;
		bgGlobalAnimations[i].numFrames = 0;
		bgGlobalAnimations[i].loopFrames = -1;
		bgGlobalAnimations[i].frameLerp = 100;
		bgGlobalAnimations[i].initialLerp = 100;
	}

	// read information for each frame
	while(1) 
	{
		token = COM_Parse( (const char **)(&text_p) );

		if ( !token || !token[0]) 
		{
			break;
		}

		animNum = GetIDForString(animTable, token);
		if(animNum == -1)
		{
//#ifndef FINAL_BUILD
#ifdef _DEBUG
			Com_Printf(S_COLOR_RED"WARNING: Unknown token %s in %s\n", token, filename);
#endif
			continue;
		}

		token = COM_Parse( (const char **)(&text_p) );
		if ( !token ) 
		{
			break;
		}
		bgGlobalAnimations[animNum].firstFrame = atoi( token );

		token = COM_Parse( (const char **)(&text_p) );
		if ( !token ) 
		{
			break;
		}
		bgGlobalAnimations[animNum].numFrames = atoi( token );

		token = COM_Parse( (const char **)(&text_p) );
		if ( !token ) 
		{
			break;
		}
		bgGlobalAnimations[animNum].loopFrames = atoi( token );

		token = COM_Parse( (const char **)(&text_p) );
		if ( !token ) 
		{
			break;
		}
		fps = atof( token );
		if ( fps == 0 ) 
		{
			fps = 1;//Don't allow divide by zero error
		}
		if ( fps < 0 )
		{//backwards
			bgGlobalAnimations[animNum].frameLerp = floor(1000.0f / fps);
		}
		else
		{
			bgGlobalAnimations[animNum].frameLerp = ceil(1000.0f / fps);
		}

		bgGlobalAnimations[animNum].initialLerp = ceil(1000.0f / fabs(fps));
	}

#ifdef _DEBUG
	//Check the array, and print the ones that have nothing in them.
	for(i = 0; i < MAX_ANIMATIONS; i++)
	{	
		if (animTable[i].name != NULL)		// This animation reference exists.
		{
			if (bgGlobalAnimations[i].firstFrame <= 0 && bgGlobalAnimations[i].numFrames <=0)
			{	// This is an empty animation reference.
				Com_Printf("***ANIMTABLE reference #%d (%s) is empty!\n", i, animTable[i].name);
			}
		}
	}
#endif // _DEBUG

	BGPAFtextLoaded = qtrue;
	return qtrue;
}



/*
===================
LEGS Animations
Base animation for overall body
===================
*/
static void PM_StartLegsAnim( int anim ) {
	if ( pm->ps->pm_type >= PM_DEAD ) {
		return;
	}
	if ( pm->ps->legsTimer > 0 ) {
		return;		// a high priority animation is running
	}

	if (pm->ps->usingATST)
	{ //animation is handled mostly client-side with only a few exceptions
		return;
	}

	if (BG_InSaberStandAnim(anim) && pm->ps->weapon == WP_SABER && pm->ps->dualBlade)
	{ //a bit of a hack, but dualblade is cheat-only anyway
		anim = BOTH_STAND1;
	}

	pm->ps->legsAnim = ( ( pm->ps->legsAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT )
		| anim;

	if ( pm->debugLevel ) {
		Com_Printf("%d:  StartLegsAnim %d, on client#%d\n", pm->cmd.serverTime, anim, pm->ps->clientNum);
	}
}

void PM_ContinueLegsAnim( int anim ) {
	if ( ( pm->ps->legsAnim & ~ANIM_TOGGLEBIT ) == anim ) {
		return;
	}
	if ( pm->ps->legsTimer > 0 ) {
		return;		// a high priority animation is running
	}

	PM_StartLegsAnim( anim );
}

void PM_ForceLegsAnim( int anim) {
	if (BG_InSpecialJump(pm->ps->legsAnim) &&
		pm->ps->legsTimer > 0 &&
		!BG_InSpecialJump(anim))
	{
		return;
	}

	if (BG_InRoll(pm->ps, pm->ps->legsAnim) &&
		pm->ps->legsTimer > 0 &&
		!BG_InRoll(pm->ps, anim))
	{
		return;
	}

	pm->ps->legsTimer = 0;
	PM_StartLegsAnim( anim );
}



/*
===================
TORSO Animations
Override animations for upper body
===================
*/
void PM_StartTorsoAnim( int anim ) {
	if ( pm->ps->pm_type >= PM_DEAD ) {
		return;
	}

	if (pm->ps->usingATST)
	{ //animation is handled mostly client-side with only a few exceptions
		return;
	}

	if (BG_InSaberStandAnim(anim) && pm->ps->weapon == WP_SABER && pm->ps->dualBlade)
	{ //a bit of a hack, but dualblade is cheat-only anyway
		anim = BOTH_STAND1;
	}

	pm->ps->torsoAnim = ( ( pm->ps->torsoAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT )
		| anim;
}


/*
-------------------------
PM_SetLegsAnimTimer
-------------------------
*/

void PM_SetLegsAnimTimer(int time )
{
	pm->ps->legsTimer = time;

	if (pm->ps->legsTimer < 0 && time != -1 )
	{//Cap timer to 0 if was counting down, but let it be -1 if that was intentional.  NOTENOTE Yeah this seems dumb, but it mirrors SP.
		pm->ps->legsTimer = 0;
	}
}

/*
-------------------------
PM_SetTorsoAnimTimer
-------------------------
*/

void PM_SetTorsoAnimTimer(int time )
{
	pm->ps->torsoTimer = time;

	if (pm->ps->torsoTimer < 0 && time != -1 )
	{//Cap timer to 0 if was counting down, but let it be -1 if that was intentional.  NOTENOTE Yeah this seems dumb, but it mirrors SP.
		pm->ps->torsoTimer = 0;
	}
}

void BG_SaberStartTransAnim( int saberAnimLevel, int anim, float *animSpeed )
{
	if ( ( (anim&~ANIM_TOGGLEBIT) >= BOTH_T1_BR__R && 
		(anim&~ANIM_TOGGLEBIT) <= BOTH_T1_BL_TL ) ||
		( (anim&~ANIM_TOGGLEBIT) >= BOTH_T2_BR__R && 
		(anim&~ANIM_TOGGLEBIT) <= BOTH_T2_BL_TL ) ||
		( (anim&~ANIM_TOGGLEBIT) >= BOTH_T3_BR__R && 
		(anim&~ANIM_TOGGLEBIT) <= BOTH_T3_BL_TL ) )
	{
		if ( saberAnimLevel == FORCE_LEVEL_1 )
		{
			*animSpeed *= 1.5;
		}
		else if ( saberAnimLevel == FORCE_LEVEL_3 )
		{
			*animSpeed *= 0.75;
		}
	}
}

/*
-------------------------
PM_SetAnimFinal
-------------------------
*/
void PM_SetAnimFinal(int setAnimParts,int anim,int setAnimFlags,
					 int blendTime)		// default blendTime=350
{
	animation_t *animations = pm->animations;

	float editAnimSpeed = 0;

	if (!animations)
	{
		return;
	}

	//NOTE: Setting blendTime here breaks actual blending..
	blendTime = 0;

	BG_SaberStartTransAnim(pm->ps->fd.saberAnimLevel, anim, &editAnimSpeed);

	// Set torso anim
	if (setAnimParts & SETANIM_TORSO)
	{
		// Don't reset if it's already running the anim
		if( !(setAnimFlags & SETANIM_FLAG_RESTART) && (pm->ps->torsoAnim & ~ANIM_TOGGLEBIT ) == anim )
		{
			goto setAnimLegs;
		}
		// or if a more important anim is running
		if( !(setAnimFlags & SETANIM_FLAG_OVERRIDE) && ((pm->ps->torsoTimer > 0)||(pm->ps->torsoTimer == -1)) )
		{	
			goto setAnimLegs;
		}

		PM_StartTorsoAnim( anim );

		if (setAnimFlags & SETANIM_FLAG_HOLD)
		{//FIXME: allow to set a specific time?
			if (setAnimFlags & SETANIM_FLAG_HOLDLESS)
			{	// Make sure to only wait in full 1/20 sec server frame intervals.
				int dur;
				
				dur = (animations[anim].numFrames ) * fabs(animations[anim].frameLerp);
				//dur = ((int)(dur/50.0)) * 50 / timeScaleMod;
				dur -= blendTime+fabs(animations[anim].frameLerp)*2;
				if (dur > 1)
				{
					pm->ps->torsoTimer = dur-1;
				}
				else
				{
					pm->ps->torsoTimer = fabs(animations[anim].frameLerp);
				}
			}
			else
			{
				pm->ps->torsoTimer = ((animations[anim].numFrames ) * fabs(animations[anim].frameLerp));
			}

			if (pm->ps->fd.forcePowersActive & (1 << FP_RAGE))
			{
				pm->ps->torsoTimer /= 1.7;
			}

			if (editAnimSpeed)
			{
				pm->ps->torsoTimer /= editAnimSpeed;
			}
		}
	}

setAnimLegs:
	// Set legs anim
	if (setAnimParts & SETANIM_LEGS)
	{
		// Don't reset if it's already running the anim
		if( !(setAnimFlags & SETANIM_FLAG_RESTART) && (pm->ps->legsAnim & ~ANIM_TOGGLEBIT ) == anim )
		{
			goto setAnimDone;
		}
		// or if a more important anim is running
		if( !(setAnimFlags & SETANIM_FLAG_OVERRIDE) && ((pm->ps->legsTimer > 0)||(pm->ps->legsTimer == -1)) )
		{	
			goto setAnimDone;
		}

		PM_StartLegsAnim(anim);

		if (setAnimFlags & SETANIM_FLAG_HOLD)
		{//FIXME: allow to set a specific time?
			if (setAnimFlags & SETANIM_FLAG_HOLDLESS)
			{	// Make sure to only wait in full 1/20 sec server frame intervals.
				int dur;
				
				dur = (animations[anim].numFrames -1) * fabs(animations[anim].frameLerp);
				//dur = ((int)(dur/50.0)) * 50 / timeScaleMod;
				dur -= blendTime+fabs(animations[anim].frameLerp)*2;
				if (dur > 1)
				{
					pm->ps->legsTimer = dur-1;
				}
				else
				{
					pm->ps->legsTimer = fabs(animations[anim].frameLerp);
				}
			}
			else
			{
				pm->ps->legsTimer = ((animations[anim].numFrames ) * fabs(animations[anim].frameLerp));
			}

			/*
			PM_DebugLegsAnim(anim);
			Com_Printf("%i\n", pm->ps->legsTimer);
			*/

			if (pm->ps->fd.forcePowersActive & (1 << FP_RAGE))
			{
				pm->ps->legsTimer /= 1.3;
			}
			else if (pm->ps->fd.forcePowersActive & (1 << FP_SPEED))
			{
				pm->ps->legsTimer /= 1.7;
			}
		}
	}

setAnimDone:
	return;
}



// Imported from single-player, this function is mainly intended to make porting from SP easier.
void PM_SetAnim(int setAnimParts,int anim,int setAnimFlags, int blendTime)
{	
	assert(	bgGlobalAnimations[anim].firstFrame != 0 || 
			bgGlobalAnimations[anim].numFrames != 0);

	if (BG_InSpecialJump(anim))
	{
		setAnimFlags |= SETANIM_FLAG_RESTART;
	}

	if (BG_InRoll(pm->ps, pm->ps->legsAnim))
	{
		//setAnimFlags |= SETANIM_FLAG_RESTART;
		return;
	}

	if (setAnimFlags&SETANIM_FLAG_OVERRIDE)
	{
		if (setAnimParts & SETANIM_TORSO)
		{
			if( (setAnimFlags & SETANIM_FLAG_RESTART) || (pm->ps->torsoAnim & ~ANIM_TOGGLEBIT ) != anim )
			{
				PM_SetTorsoAnimTimer(0);
			}
		}
		if (setAnimParts & SETANIM_LEGS)
		{
			if( (setAnimFlags & SETANIM_FLAG_RESTART) || (pm->ps->legsAnim & ~ANIM_TOGGLEBIT ) != anim )
			{
				PM_SetLegsAnimTimer(0);
			}
		}
	}

	PM_SetAnimFinal(setAnimParts, anim, setAnimFlags, blendTime);
}


