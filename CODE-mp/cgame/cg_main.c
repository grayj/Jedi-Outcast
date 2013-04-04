// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_main.c -- initialization and primary entry point for cgame
#include "cg_local.h"

#include "../ui/ui_shared.h"
// display context for new ui stuff
displayContextDef_t cgDC;

#if !defined(CL_LIGHT_H_INC)
	#include "cg_lights.h"
#endif

/*
Ghoul2 Insert Start
*/
void CG_InitItems(void);
/*
Ghoul2 Insert End
*/

vec4_t colorTable[CT_MAX] = 
{
{0, 0, 0, 0},			// CT_NONE
{0, 0, 0, 1},			// CT_BLACK
{1, 0, 0, 1},			// CT_RED
{0, 1, 0, 1},			// CT_GREEN
{0, 0, 1, 1},			// CT_BLUE
{1, 1, 0, 1},			// CT_YELLOW
{1, 0, 1, 1},			// CT_MAGENTA
{0, 1, 1, 1},			// CT_CYAN
{1, 1, 1, 1},			// CT_WHITE
{0.75f, 0.75f, 0.75f, 1},	// CT_LTGREY
{0.50f, 0.50f, 0.50f, 1},	// CT_MDGREY
{0.25f, 0.25f, 0.25f, 1},	// CT_DKGREY
{0.15f, 0.15f, 0.15f, 1},	// CT_DKGREY2

{0.810f, 0.530f, 0.0f,  1},	// CT_VLTORANGE -- needs values
{0.810f, 0.530f, 0.0f,  1},	// CT_LTORANGE
{0.610f, 0.330f, 0.0f,  1},	// CT_DKORANGE
{0.402f, 0.265f, 0.0f,  1},	// CT_VDKORANGE

{0.503f, 0.375f, 0.996f, 1},	// CT_VLTBLUE1
{0.367f, 0.261f, 0.722f, 1},	// CT_LTBLUE1
{0.199f, 0.0f,   0.398f, 1},	// CT_DKBLUE1
{0.160f, 0.117f, 0.324f, 1},	// CT_VDKBLUE1

{0.300f, 0.628f, 0.816f, 1},	// CT_VLTBLUE2 -- needs values
{0.300f, 0.628f, 0.816f, 1},	// CT_LTBLUE2
{0.191f, 0.289f, 0.457f, 1},	// CT_DKBLUE2
{0.125f, 0.250f, 0.324f, 1},	// CT_VDKBLUE2

{0.796f, 0.398f, 0.199f, 1},	// CT_VLTBROWN1 -- needs values
{0.796f, 0.398f, 0.199f, 1},	// CT_LTBROWN1
{0.558f, 0.207f, 0.027f, 1},	// CT_DKBROWN1
{0.328f, 0.125f, 0.035f, 1},	// CT_VDKBROWN1

{0.996f, 0.796f, 0.398f, 1},	// CT_VLTGOLD1 -- needs values
{0.996f, 0.796f, 0.398f, 1},	// CT_LTGOLD1
{0.605f, 0.441f, 0.113f, 1},	// CT_DKGOLD1
{0.386f, 0.308f, 0.148f, 1},	// CT_VDKGOLD1

{0.648f, 0.562f, 0.784f, 1},	// CT_VLTPURPLE1 -- needs values
{0.648f, 0.562f, 0.784f, 1},	// CT_LTPURPLE1
{0.437f, 0.335f, 0.597f, 1},	// CT_DKPURPLE1
{0.308f, 0.269f, 0.375f, 1},	// CT_VDKPURPLE1

{0.816f, 0.531f, 0.710f, 1},	// CT_VLTPURPLE2 -- needs values
{0.816f, 0.531f, 0.710f, 1},	// CT_LTPURPLE2
{0.566f, 0.269f, 0.457f, 1},	// CT_DKPURPLE2
{0.343f, 0.226f, 0.316f, 1},	// CT_VDKPURPLE2

{0.929f, 0.597f, 0.929f, 1},	// CT_VLTPURPLE3
{0.570f, 0.371f, 0.570f, 1},	// CT_LTPURPLE3
{0.355f, 0.199f, 0.355f, 1},	// CT_DKPURPLE3
{0.285f, 0.136f, 0.230f, 1},	// CT_VDKPURPLE3

{0.953f, 0.378f, 0.250f, 1},	// CT_VLTRED1
{0.953f, 0.378f, 0.250f, 1},	// CT_LTRED1
{0.593f, 0.121f, 0.109f, 1},	// CT_DKRED1
{0.429f, 0.171f, 0.113f, 1},	// CT_VDKRED1
{.25f, 0, 0, 1},					// CT_VDKRED
{.70f, 0, 0, 1},					// CT_DKRED
	
{0.717f, 0.902f, 1.0f,   1},		// CT_VLTAQUA
{0.574f, 0.722f, 0.804f, 1},		// CT_LTAQUA
{0.287f, 0.361f, 0.402f, 1},		// CT_DKAQUA
{0.143f, 0.180f, 0.201f, 1},		// CT_VDKAQUA

{0.871f, 0.386f, 0.375f, 1},		// CT_LTPINK
{0.435f, 0.193f, 0.187f, 1},		// CT_DKPINK
{	  0,    .5f,    .5f, 1},		// CT_LTCYAN
{	  0,   .25f,   .25f, 1},		// CT_DKCYAN
{   .179f, .51f,   .92f, 1},		// CT_LTBLUE3
{   .199f, .71f,   .92f, 1},		// CT_LTBLUE3
{   .5f,   .05f,    .4f, 1},		// CT_DKBLUE3

{   0.0f,   .613f,  .097f, 1},		// CT_HUD_GREEN
{   0.835f, .015f,  .015f, 1},		// CT_HUD_RED
{	.567f,	.685f,	1.0f,	.75f},	// CT_ICON_BLUE
{	.515f,	.406f,	.507f,	1},		// CT_NO_AMMO_RED
{   1.0f,   .658f,  .062f, 1},		// CT_HUD_ORANGE

};

char *HolocronIcons[] = {
	"gfx/mp/f_icon_lt_heal",		//FP_HEAL,
	"gfx/mp/f_icon_levitation",		//FP_LEVITATION,
	"gfx/mp/f_icon_speed",			//FP_SPEED,
	"gfx/mp/f_icon_push",			//FP_PUSH,
	"gfx/mp/f_icon_pull",			//FP_PULL,
	"gfx/mp/f_icon_lt_telepathy",	//FP_TELEPATHY,
	"gfx/mp/f_icon_dk_grip",		//FP_GRIP,
	"gfx/mp/f_icon_dk_l1",			//FP_LIGHTNING,
	"gfx/mp/f_icon_dk_rage",		//FP_RAGE,
	"gfx/mp/f_icon_lt_protect",		//FP_PROTECT,
	"gfx/mp/f_icon_lt_absorb",		//FP_ABSORB,
	"gfx/mp/f_icon_lt_healother",	//FP_TEAM_HEAL,
	"gfx/mp/f_icon_dk_forceother",	//FP_TEAM_FORCE,
	"gfx/mp/f_icon_dk_drain",		//FP_DRAIN,
	"gfx/mp/f_icon_sight",			//FP_SEE,
	"gfx/mp/f_icon_saber_attack",	//FP_SABERATTACK,
	"gfx/mp/f_icon_saber_defend",	//FP_SABERDEFEND,
	"gfx/mp/f_icon_saber_throw"		//FP_SABERTHROW
};

int forceModelModificationCount = -1;

void CG_Init( int serverMessageNum, int serverCommandSequence, int clientNum );
void CG_Shutdown( void );

void CG_CalcEntityLerpPositions( centity_t *cent );
void CG_ROFF_NotetrackCallback( centity_t *cent, const char *notetrack);

static int	C_PointContents(void);
static void C_GetLerpOrigin(void);
static void C_GetLerpAngles(void);
static void C_GetModelScale(void);
static void C_Trace(void);
static void C_GetBoltPos(void);
static void C_ImpactMark(void);

/*
================
vmMain

This is the only way control passes into the module.
This must be the very first function compiled into the .q3vm file
================
*/
int vmMain( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11  ) {

	switch ( command ) {
	case CG_INIT:
		CG_Init( arg0, arg1, arg2 );
		return 0;
	case CG_SHUTDOWN:
		CG_Shutdown();
		return 0;
	case CG_CONSOLE_COMMAND:
		return CG_ConsoleCommand();
	case CG_DRAW_ACTIVE_FRAME:
		CG_DrawActiveFrame( arg0, arg1, arg2 );
		return 0;
	case CG_CROSSHAIR_PLAYER:
		return CG_CrosshairPlayer();
	case CG_LAST_ATTACKER:
		return CG_LastAttacker();
	case CG_KEY_EVENT:
		CG_KeyEvent(arg0, arg1);
		return 0;
	case CG_MOUSE_EVENT:
		cgDC.cursorx = cgs.cursorX;
		cgDC.cursory = cgs.cursorY;
		CG_MouseEvent(arg0, arg1);
		return 0;
	case CG_EVENT_HANDLING:
		CG_EventHandling(arg0);
		return 0;

	case CG_POINT_CONTENTS:
		return C_PointContents();

	case CG_GET_LERP_ORIGIN:
		C_GetLerpOrigin();
		return 0;

	case CG_GET_LERP_ANGLES:
		C_GetLerpAngles();
		return 0;

	case CG_GET_MODEL_SCALE:
		C_GetModelScale();
		return 0;

	case CG_GET_GHOUL2:
		return (int)cg_entities[arg0].ghoul2; //NOTE: This is used by the effect bolting which is actually not used at all.
											  //I'm fairly sure if you try to use it with vm's it will just give you total
											  //garbage. In other words, use at your own risk.

	case CG_GET_MODEL_LIST:
		return (int)cgs.gameModels;

	case CG_CALC_LERP_POSITIONS:
		CG_CalcEntityLerpPositions( &cg_entities[arg0] );
		return 0;

	case CG_TRACE:
		C_Trace();
		return 0;

	case CG_GET_ORIGIN:
		VectorCopy(cg_entities[arg0].currentState.pos.trBase, (float *)arg1);
		return 0;

	case CG_GET_ANGLES:
		VectorCopy(cg_entities[arg0].currentState.apos.trBase, (float *)arg1);
		return 0;

	case CG_GET_BOLT_POS:
		C_GetBoltPos();
		return 0;

	case CG_GET_ORIGIN_TRAJECTORY:
		return (int)&cg_entities[arg0].nextState.pos;

	case CG_GET_ANGLE_TRAJECTORY:
		return (int)&cg_entities[arg0].nextState.apos;

	case CG_ROFF_NOTETRACK_CALLBACK:
		CG_ROFF_NotetrackCallback( &cg_entities[arg0], (const char *)arg1 );
		return 0;

	case CG_IMPACT_MARK:
		C_ImpactMark();
		return 0;

	case CG_MAP_CHANGE:
		// this trap map be called more than once for a given map change, as the
		// server is going to attempt to send out multiple broadcasts in hopes that
		// the client will receive one of them
		cg.mMapChange = qtrue;
		return 0;

	default:
		CG_Error( "vmMain: unknown command %i", command );
		break;
	}
	return -1;
}

static int C_PointContents(void)
{
	TCGPointContents	*data = (TCGPointContents *)cg.sharedBuffer;

	return CG_PointContents( data->mPoint, data->mPassEntityNum );
}

static void C_GetLerpOrigin(void)
{
	TCGVectorData		*data = (TCGVectorData *)cg.sharedBuffer;

	VectorCopy(cg_entities[data->mEntityNum].lerpOrigin, data->mPoint);
}

static void C_GetLerpAngles(void)
{
	TCGVectorData		*data = (TCGVectorData *)cg.sharedBuffer;

	VectorCopy(cg_entities[data->mEntityNum].lerpAngles, data->mPoint);
}

static void C_GetModelScale(void)
{
	TCGVectorData		*data = (TCGVectorData *)cg.sharedBuffer;

	VectorCopy(cg_entities[data->mEntityNum].modelScale, data->mPoint);
}

static void C_Trace(void)
{
	TCGTrace	*td = (TCGTrace *)cg.sharedBuffer;

	CG_Trace(&td->mResult, td->mStart, td->mMins, td->mMaxs, td->mEnd, td->mSkipNumber, td->mMask);
}

static void C_GetBoltPos(void)
{
	TCGBoltPos	*data = (TCGBoltPos *)cg.sharedBuffer;

	if (!cg_entities[data->mEntityNum].ghoul2)
	{
		VectorClear(data->mPoint);
		VectorClear(data->mAngle);
		return;
	}

	VectorCopy(cg_entities[data->mEntityNum].lerpOrigin, data->mPoint);
	if (data->mEntityNum < MAX_CLIENTS)
	{
		VectorCopy(cg_entities[data->mEntityNum].turAngles, data->mAngle);
	}
	else
	{
		VectorCopy(cg_entities[data->mEntityNum].lerpAngles, data->mAngle);
	}
}

static void C_ImpactMark(void)
{
	TCGImpactMark	*data = (TCGImpactMark *)cg.sharedBuffer;

	/*
	CG_ImpactMark((int)arg0, (const float *)arg1, (const float *)arg2, (float)arg3,
		(float)arg4, (float)arg5, (float)arg6, (float)arg7, qtrue, (float)arg8, qfalse);
	*/
	CG_ImpactMark(data->mHandle, data->mPoint, data->mAngle, data->mRotation,
		data->mRed, data->mGreen, data->mBlue, data->mAlphaStart, qtrue, data->mSizeStart, qfalse);
}

/*
Ghoul2 Insert Start
*/
/*
void CG_ResizeG2Bolt(boltInfo_v *bolt, int newCount)
{
	bolt->resize(newCount);
}

void CG_ResizeG2Surface(surfaceInfo_v *surface, int newCount)
{
	surface->resize(newCount);
}

void CG_ResizeG2Bone(boneInfo_v *bone, int newCount)
{
	bone->resize(newCount);
}

void CG_ResizeG2(CGhoul2Info_v *ghoul2, int newCount)
{
	ghoul2->resize(newCount);
}

void CG_ResizeG2TempBone(mdxaBone_v *tempBone, int newCount)
{
	tempBone->resize(newCount);
}
*/
/*
Ghoul2 Insert End
*/
cg_t				cg;
cgs_t				cgs;
centity_t			cg_entities[MAX_GENTITIES];
weaponInfo_t		cg_weapons[MAX_WEAPONS];
itemInfo_t			cg_items[MAX_ITEMS];


vmCvar_t	cg_centertime;
vmCvar_t	cg_runpitch;
vmCvar_t	cg_runroll;
vmCvar_t	cg_bobup;
vmCvar_t	cg_bobpitch;
vmCvar_t	cg_bobroll;
//vmCvar_t	cg_swingSpeed;
vmCvar_t	cg_shadows;
vmCvar_t	cg_gibs;
vmCvar_t	cg_drawTimer;
vmCvar_t	cg_drawFPS;
vmCvar_t	cg_drawSnapshot;
vmCvar_t	cg_draw3dIcons;
vmCvar_t	cg_drawIcons;
vmCvar_t	cg_drawAmmoWarning;
vmCvar_t	cg_drawCrosshair;
vmCvar_t	cg_drawCrosshairNames;
vmCvar_t	cg_dynamicCrosshair;
vmCvar_t	cg_drawRewards;
vmCvar_t	cg_drawScores;
vmCvar_t	cg_crosshairSize;
vmCvar_t	cg_crosshairX;
vmCvar_t	cg_crosshairY;
vmCvar_t	cg_crosshairHealth;
vmCvar_t	cg_draw2D;
vmCvar_t	cg_drawStatus;
vmCvar_t	cg_animSpeed;
vmCvar_t	cg_debugAnim;
vmCvar_t	cg_debugSaber;
vmCvar_t	cg_debugPosition;
vmCvar_t	cg_debugEvents;
vmCvar_t	cg_errorDecay;
vmCvar_t	cg_nopredict;
vmCvar_t	cg_noPlayerAnims;
vmCvar_t	cg_showmiss;
vmCvar_t	cg_footsteps;
vmCvar_t	cg_addMarks;
vmCvar_t	cg_viewsize;
vmCvar_t	cg_drawGun;
vmCvar_t	cg_gun_frame;
vmCvar_t	cg_gun_x;
vmCvar_t	cg_gun_y;
vmCvar_t	cg_gun_z;
vmCvar_t	cg_tracerChance;
vmCvar_t	cg_tracerWidth;
vmCvar_t	cg_tracerLength;
vmCvar_t	cg_autoswitch;
vmCvar_t	cg_ignore;
vmCvar_t	cg_simpleItems;
vmCvar_t	cg_fov;
vmCvar_t	cg_zoomFov;

vmCvar_t	cg_swingAngles;

vmCvar_t	cg_saberContact;
vmCvar_t	cg_saberTrail;

vmCvar_t	cg_speedTrail;
vmCvar_t	cg_auraShell;

vmCvar_t	cg_animBlend;

vmCvar_t	cg_dismember;

vmCvar_t	cg_thirdPerson;
vmCvar_t	cg_thirdPersonRange;
vmCvar_t	cg_thirdPersonAngle;
vmCvar_t	cg_thirdPersonPitchOffset;
vmCvar_t	cg_thirdPersonVertOffset;
vmCvar_t	cg_thirdPersonCameraDamp;
vmCvar_t	cg_thirdPersonTargetDamp;

vmCvar_t	cg_thirdPersonAlpha;
vmCvar_t	cg_thirdPersonHorzOffset;

vmCvar_t	cg_stereoSeparation;
vmCvar_t	cg_lagometer;
vmCvar_t	cg_drawEnemyInfo;
vmCvar_t	cg_synchronousClients;
vmCvar_t 	cg_teamChatTime;
vmCvar_t 	cg_teamChatHeight;
vmCvar_t 	cg_stats;
vmCvar_t 	cg_buildScript;
vmCvar_t 	cg_forceModel;
vmCvar_t	cg_paused;
vmCvar_t	cg_blood;
vmCvar_t	cg_predictItems;
vmCvar_t	cg_deferPlayers;
vmCvar_t	cg_drawTeamOverlay;
vmCvar_t	cg_teamOverlayUserinfo;
vmCvar_t	cg_drawFriend;
vmCvar_t	cg_teamChatsOnly;
vmCvar_t	cg_noVoiceChats;
vmCvar_t	cg_noVoiceText;
vmCvar_t	cg_hudFiles;
vmCvar_t 	cg_scorePlum;
vmCvar_t 	cg_smoothClients;
vmCvar_t	pmove_fixed;
//vmCvar_t	cg_pmove_fixed;
vmCvar_t	pmove_msec;
vmCvar_t	cg_pmove_msec;
vmCvar_t	cg_cameraMode;
vmCvar_t	cg_cameraOrbit;
vmCvar_t	cg_cameraOrbitDelay;
vmCvar_t	cg_timescaleFadeEnd;
vmCvar_t	cg_timescaleFadeSpeed;
vmCvar_t	cg_timescale;
vmCvar_t	cg_smallFont;
vmCvar_t	cg_bigFont;
vmCvar_t	cg_noTaunt;
vmCvar_t	cg_noProjectileTrail;
vmCvar_t	cg_trueLightning;
/*
Ghoul2 Insert Start
*/
vmCvar_t	cg_debugBB;
/*
Ghoul2 Insert End
*/
vmCvar_t 	cg_redTeamName;
vmCvar_t 	cg_blueTeamName;
vmCvar_t	cg_currentSelectedPlayer;
vmCvar_t	cg_currentSelectedPlayerName;
vmCvar_t	cg_singlePlayer;
vmCvar_t	cg_enableDust;
vmCvar_t	cg_enableBreath;
vmCvar_t	cg_singlePlayerActive;
vmCvar_t	cg_recordSPDemo;
vmCvar_t	cg_recordSPDemoName;

vmCvar_t	ui_myteam;

typedef struct {
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;
} cvarTable_t;

static cvarTable_t cvarTable[] = { // bk001129
	{ &cg_ignore, "cg_ignore", "0", 0 },	// used for debugging
	{ &cg_autoswitch, "cg_autoswitch", "1", CVAR_ARCHIVE },
	{ &cg_drawGun, "cg_drawGun", "1", CVAR_ARCHIVE },
	{ &cg_zoomFov, "cg_zoomfov", "40.0", CVAR_ARCHIVE },
	{ &cg_fov, "cg_fov", "80", CVAR_ARCHIVE },
	{ &cg_viewsize, "cg_viewsize", "100", CVAR_ARCHIVE },
	{ &cg_stereoSeparation, "cg_stereoSeparation", "0.4", CVAR_ARCHIVE  },
	{ &cg_shadows, "cg_shadows", "1", CVAR_ARCHIVE  },
	{ &cg_gibs, "cg_gibs", "1", CVAR_ARCHIVE  },
	{ &cg_draw2D, "cg_draw2D", "1", CVAR_ARCHIVE  },
	{ &cg_drawStatus, "cg_drawStatus", "1", CVAR_ARCHIVE  },
	{ &cg_drawTimer, "cg_drawTimer", "0", CVAR_ARCHIVE  },
	{ &cg_drawFPS, "cg_drawFPS", "0", CVAR_ARCHIVE  },
	{ &cg_drawSnapshot, "cg_drawSnapshot", "0", CVAR_ARCHIVE  },
	{ &cg_draw3dIcons, "cg_draw3dIcons", "1", CVAR_ARCHIVE  },
	{ &cg_drawIcons, "cg_drawIcons", "1", CVAR_ARCHIVE  },
	{ &cg_drawAmmoWarning, "cg_drawAmmoWarning", "0", CVAR_ARCHIVE  },
	{ &cg_drawEnemyInfo, "cg_drawEnemyInfo", "1", CVAR_ARCHIVE  },
	{ &cg_drawCrosshair, "cg_drawCrosshair", "1", CVAR_ARCHIVE },
	{ &cg_drawCrosshairNames, "cg_drawCrosshairNames", "1", CVAR_ARCHIVE },
	{ &cg_drawScores,		  "cg_drawScores", "1", CVAR_ARCHIVE },
	{ &cg_dynamicCrosshair, "cg_dynamicCrosshair", "1", CVAR_ARCHIVE },
	{ &cg_drawRewards, "cg_drawRewards", "1", CVAR_ARCHIVE },
	{ &cg_crosshairSize, "cg_crosshairSize", "24", CVAR_ARCHIVE },
	{ &cg_crosshairHealth, "cg_crosshairHealth", "0", CVAR_ARCHIVE },
	{ &cg_crosshairX, "cg_crosshairX", "0", CVAR_ARCHIVE },
	{ &cg_crosshairY, "cg_crosshairY", "0", CVAR_ARCHIVE },
	{ &cg_simpleItems, "cg_simpleItems", "0", CVAR_ARCHIVE },
	{ &cg_addMarks, "cg_marks", "1", CVAR_ARCHIVE },
	{ &cg_lagometer, "cg_lagometer", "0", CVAR_ARCHIVE },
	{ &cg_gun_x, "cg_gunX", "0", CVAR_CHEAT },
	{ &cg_gun_y, "cg_gunY", "0", CVAR_CHEAT },
	{ &cg_gun_z, "cg_gunZ", "0", CVAR_CHEAT },
	{ &cg_centertime, "cg_centertime", "3", CVAR_CHEAT },
	{ &cg_runpitch, "cg_runpitch", "0.002", CVAR_ARCHIVE},
	{ &cg_runroll, "cg_runroll", "0.005", CVAR_ARCHIVE },
	{ &cg_bobup , "cg_bobup", "0.005", CVAR_ARCHIVE },
	{ &cg_bobpitch, "cg_bobpitch", "0.002", CVAR_ARCHIVE },
	{ &cg_bobroll, "cg_bobroll", "0.002", CVAR_ARCHIVE },
	//{ &cg_swingSpeed, "cg_swingSpeed", "0.3", CVAR_CHEAT },
	{ &cg_animSpeed, "cg_animspeed", "1", CVAR_CHEAT },
	{ &cg_debugAnim, "cg_debuganim", "0", CVAR_CHEAT },
	{ &cg_debugSaber, "cg_debugsaber", "0", CVAR_CHEAT },
	{ &cg_debugPosition, "cg_debugposition", "0", CVAR_CHEAT },
	{ &cg_debugEvents, "cg_debugevents", "0", CVAR_CHEAT },
	{ &cg_errorDecay, "cg_errordecay", "100", 0 },
	{ &cg_nopredict, "cg_nopredict", "0", 0 },
	{ &cg_noPlayerAnims, "cg_noplayeranims", "0", CVAR_CHEAT },
	{ &cg_showmiss, "cg_showmiss", "0", 0 },
	{ &cg_footsteps, "cg_footsteps", "1", CVAR_CHEAT },
	{ &cg_tracerChance, "cg_tracerchance", "0.4", CVAR_CHEAT },
	{ &cg_tracerWidth, "cg_tracerwidth", "1", CVAR_CHEAT },
	{ &cg_tracerLength, "cg_tracerlength", "100", CVAR_CHEAT },

	{ &cg_swingAngles, "cg_swingAngles", "1", 0 },

	{ &cg_saberContact, "cg_saberContact", "1", 0 },
	{ &cg_saberTrail, "cg_saberTrail", "1", 0 },

	{ &cg_speedTrail, "cg_speedTrail", "1", 0 },
	{ &cg_auraShell, "cg_auraShell", "1", 0 },

	{ &cg_animBlend, "cg_animBlend", "1", 0 },

	{ &cg_dismember, "cg_dismember", "0", 0 },

	{ &cg_thirdPerson, "cg_thirdPerson", "0", 0 },
	{ &cg_thirdPersonRange, "cg_thirdPersonRange", "80", CVAR_CHEAT },
	{ &cg_thirdPersonAngle, "cg_thirdPersonAngle", "0", CVAR_CHEAT },
	{ &cg_thirdPersonPitchOffset, "cg_thirdPersonPitchOffset", "0", CVAR_CHEAT },
	{ &cg_thirdPersonVertOffset, "cg_thirdPersonVertOffset", "16", CVAR_CHEAT },
	{ &cg_thirdPersonCameraDamp, "cg_thirdPersonCameraDamp", "0.3", 0 },
	{ &cg_thirdPersonTargetDamp, "cg_thirdPersonTargetDamp", "0.5", CVAR_CHEAT },
	
	{ &cg_thirdPersonHorzOffset, "cg_thirdPersonHorzOffset", "0", CVAR_CHEAT },
	{ &cg_thirdPersonAlpha,	"cg_thirdPersonAlpha",	"1.0", CVAR_CHEAT },

	{ &cg_teamChatTime, "cg_teamChatTime", "3000", CVAR_ARCHIVE  },
	{ &cg_teamChatHeight, "cg_teamChatHeight", "0", CVAR_ARCHIVE  },
	{ &cg_forceModel, "cg_forceModel", "0", CVAR_ARCHIVE  },
	{ &cg_predictItems, "cg_predictItems", "1", CVAR_ARCHIVE },
	{ &cg_deferPlayers, "cg_deferPlayers", "1", CVAR_ARCHIVE },
	{ &cg_drawTeamOverlay, "cg_drawTeamOverlay", "0", CVAR_ARCHIVE },
	{ &cg_teamOverlayUserinfo, "teamoverlay", "0", CVAR_ROM | CVAR_USERINFO },
	{ &cg_stats, "cg_stats", "0", 0 },
	{ &cg_drawFriend, "cg_drawFriend", "1", CVAR_ARCHIVE },
	{ &cg_teamChatsOnly, "cg_teamChatsOnly", "0", CVAR_ARCHIVE },
	{ &cg_noVoiceChats, "cg_noVoiceChats", "0", CVAR_ARCHIVE },
	{ &cg_noVoiceText, "cg_noVoiceText", "0", CVAR_ARCHIVE },
	// the following variables are created in other parts of the system,
	// but we also reference them here
	{ &cg_buildScript, "com_buildScript", "0", 0 },	// force loading of all possible data amd error on failures
	{ &cg_paused, "cl_paused", "0", CVAR_ROM },
	{ &cg_blood, "com_blood", "1", CVAR_ARCHIVE },
	{ &cg_synchronousClients, "g_synchronousClients", "0", 0 },	// communicated by systeminfo

	{ &cg_redTeamName, "g_redteam", DEFAULT_REDTEAM_NAME, CVAR_ARCHIVE | CVAR_SERVERINFO | CVAR_USERINFO },
	{ &cg_blueTeamName, "g_blueteam", DEFAULT_BLUETEAM_NAME, CVAR_ARCHIVE | CVAR_SERVERINFO | CVAR_USERINFO },
	{ &cg_currentSelectedPlayer, "cg_currentSelectedPlayer", "0", CVAR_ARCHIVE},
	{ &cg_currentSelectedPlayerName, "cg_currentSelectedPlayerName", "", CVAR_ARCHIVE},
	{ &cg_singlePlayer, "ui_singlePlayerActive", "0", CVAR_USERINFO},
	{ &cg_enableDust, "g_enableDust", "0", 0},
	{ &cg_enableBreath, "g_enableBreath", "0", 0},
	{ &cg_singlePlayerActive, "ui_singlePlayerActive", "0", CVAR_USERINFO},
	{ &cg_recordSPDemo, "ui_recordSPDemo", "0", CVAR_ARCHIVE},
	{ &cg_recordSPDemoName, "ui_recordSPDemoName", "", CVAR_ARCHIVE},

	{ &cg_cameraOrbit, "cg_cameraOrbit", "0", CVAR_CHEAT},
	{ &cg_cameraOrbitDelay, "cg_cameraOrbitDelay", "50", CVAR_ARCHIVE},
	{ &cg_timescaleFadeEnd, "cg_timescaleFadeEnd", "1", 0},
	{ &cg_timescaleFadeSpeed, "cg_timescaleFadeSpeed", "0", 0},
	{ &cg_timescale, "timescale", "1", 0},
	{ &cg_scorePlum, "cg_scorePlums", "1", CVAR_USERINFO | CVAR_ARCHIVE},
	{ &cg_hudFiles, "cg_hudFiles", "0", CVAR_USERINFO | CVAR_ARCHIVE},
	{ &cg_smoothClients, "cg_smoothClients", "0", CVAR_USERINFO | CVAR_ARCHIVE},
	{ &cg_cameraMode, "com_cameraMode", "0", CVAR_CHEAT},

	{ &pmove_fixed, "pmove_fixed", "0", 0},
	{ &pmove_msec, "pmove_msec", "8", 0},
	{ &cg_noTaunt, "cg_noTaunt", "0", CVAR_ARCHIVE},
	{ &cg_noProjectileTrail, "cg_noProjectileTrail", "0", CVAR_ARCHIVE},
	{ &cg_smallFont, "ui_smallFont", "0.25", CVAR_ARCHIVE},
	{ &cg_bigFont, "ui_bigFont", "0.4", CVAR_ARCHIVE},
	{ &cg_trueLightning, "cg_trueLightning", "0.0", CVAR_ARCHIVE},

	{ &ui_myteam, "ui_myteam", "0", CVAR_ROM|CVAR_INTERNAL},

//	{ &cg_pmove_fixed, "cg_pmove_fixed", "0", CVAR_USERINFO | CVAR_ARCHIVE }
/*
Ghoul2 Insert Start
*/
	{ &cg_debugBB, "debugBB", "0", 0},
/*
Ghoul2 Insert End
*/
};

static int  cvarTableSize = sizeof( cvarTable ) / sizeof( cvarTable[0] );

/*
=================
CG_RegisterCvars
=================
*/
void CG_RegisterCvars( void ) {
	int			i;
	cvarTable_t	*cv;
	char		var[MAX_TOKEN_CHARS];

	for ( i = 0, cv = cvarTable ; i < cvarTableSize ; i++, cv++ ) {
		trap_Cvar_Register( cv->vmCvar, cv->cvarName,
			cv->defaultString, cv->cvarFlags );
	}

	// see if we are also running the server on this machine
	trap_Cvar_VariableStringBuffer( "sv_running", var, sizeof( var ) );
	cgs.localServer = atoi( var );

	forceModelModificationCount = cg_forceModel.modificationCount;

	trap_Cvar_Register(NULL, "model", DEFAULT_MODEL, CVAR_USERINFO | CVAR_ARCHIVE );
	//trap_Cvar_Register(NULL, "headmodel", DEFAULT_MODEL, CVAR_USERINFO | CVAR_ARCHIVE );
	trap_Cvar_Register(NULL, "team_model", DEFAULT_TEAM_MODEL, CVAR_USERINFO | CVAR_ARCHIVE );
	//trap_Cvar_Register(NULL, "team_headmodel", DEFAULT_TEAM_HEAD, CVAR_USERINFO | CVAR_ARCHIVE );
	trap_Cvar_Register(NULL, "forcepowers", DEFAULT_FORCEPOWERS, CVAR_USERINFO | CVAR_ARCHIVE );

	// Cvars uses for transferring data between client and server
	trap_Cvar_Register(NULL, "ui_about_gametype",		"0", CVAR_ROM|CVAR_INTERNAL );
	trap_Cvar_Register(NULL, "ui_about_fraglimit",		"0", CVAR_ROM|CVAR_INTERNAL );
	trap_Cvar_Register(NULL, "ui_about_capturelimit",	"0", CVAR_ROM|CVAR_INTERNAL );
	trap_Cvar_Register(NULL, "ui_about_duellimit",		"0", CVAR_ROM|CVAR_INTERNAL );
	trap_Cvar_Register(NULL, "ui_about_timelimit",		"0", CVAR_ROM|CVAR_INTERNAL );
	trap_Cvar_Register(NULL, "ui_about_maxclients",		"0", CVAR_ROM|CVAR_INTERNAL );
	trap_Cvar_Register(NULL, "ui_about_dmflags",		"0", CVAR_ROM|CVAR_INTERNAL );
	trap_Cvar_Register(NULL, "ui_about_mapname",		"0", CVAR_ROM|CVAR_INTERNAL );
	trap_Cvar_Register(NULL, "ui_about_hostname",		"0", CVAR_ROM|CVAR_INTERNAL );
	trap_Cvar_Register(NULL, "ui_about_needpass",		"0", CVAR_ROM|CVAR_INTERNAL );
	trap_Cvar_Register(NULL, "ui_about_botminplayers",	"0", CVAR_ROM|CVAR_INTERNAL );
}

/*																																			
===================
CG_ForceModelChange
===================
*/
static void CG_ForceModelChange( void ) {
	int		i;

	for (i=0 ; i<MAX_CLIENTS ; i++) {
		const char		*clientInfo;
		void	*oldGhoul2;

		clientInfo = CG_ConfigString( CS_PLAYERS+i );
		if ( !clientInfo[0] ) {
			continue;
		}

		oldGhoul2 = cgs.clientinfo[i].ghoul2Model;
		CG_NewClientInfo( i, qtrue);
	}
}

/*
=================
CG_UpdateCvars
=================
*/
void CG_UpdateCvars( void ) {
	int			i;
	cvarTable_t	*cv;

	for ( i = 0, cv = cvarTable ; i < cvarTableSize ; i++, cv++ ) {
		trap_Cvar_Update( cv->vmCvar );
	}

	// check for modications here

	// If team overlay is on, ask for updates from the server.  If its off,
	// let the server know so we don't receive it
	if ( drawTeamOverlayModificationCount != cg_drawTeamOverlay.modificationCount ) {
		drawTeamOverlayModificationCount = cg_drawTeamOverlay.modificationCount;

		if ( cg_drawTeamOverlay.integer > 0 ) {
			trap_Cvar_Set( "teamoverlay", "1" );
		} else {
			trap_Cvar_Set( "teamoverlay", "0" );
		}
		// FIXME E3 HACK
		trap_Cvar_Set( "teamoverlay", "1" );
	}

	// if force model changed
	if ( forceModelModificationCount != cg_forceModel.modificationCount ) {
		forceModelModificationCount = cg_forceModel.modificationCount;
		CG_ForceModelChange();
	}
}

int CG_CrosshairPlayer( void ) {
	if ( cg.time > ( cg.crosshairClientTime + 1000 ) ) {
		return -1;
	}

	if (cg.crosshairClientNum >= MAX_CLIENTS)
	{
		return -1;
	}

	return cg.crosshairClientNum;
}

int CG_LastAttacker( void ) {
	if ( !cg.attackerTime ) {
		return -1;
	}
	return cg.snap->ps.persistant[PERS_ATTACKER];
}

void QDECL CG_Printf( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	vsprintf (text, msg, argptr);
	va_end (argptr);

	trap_Print( text );
}

void QDECL CG_Error( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	vsprintf (text, msg, argptr);
	va_end (argptr);

	trap_Error( text );
}

#ifndef CGAME_HARD_LINKED
// this is only here so the functions in q_shared.c and bg_*.c can link (FIXME)

void QDECL Com_Error( int level, const char *error, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, error);
	vsprintf (text, error, argptr);
	va_end (argptr);

	CG_Error( "%s", text);
}

void QDECL Com_Printf( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	vsprintf (text, msg, argptr);
	va_end (argptr);

	CG_Printf ("%s", text);
}

#endif

/*
================
CG_Argv
================
*/
const char *CG_Argv( int arg ) {
	static char	buffer[MAX_STRING_CHARS];

	trap_Argv( arg, buffer, sizeof( buffer ) );

	return buffer;
}


//========================================================================

/*
=================
CG_RegisterItemSounds

The server says this item is used on this level
=================
*/
static void CG_RegisterItemSounds( int itemNum ) {
	gitem_t			*item;
	char			data[MAX_QPATH];
	char			*s, *start;
	int				len;

	item = &bg_itemlist[ itemNum ];

	if( item->pickup_sound ) {
		trap_S_RegisterSound( item->pickup_sound );
	}

	// parse the space seperated precache string for other media
	s = item->sounds;
	if (!s || !s[0])
		return;

	while (*s) {
		start = s;
		while (*s && *s != ' ') {
			s++;
		}

		len = s-start;
		if (len >= MAX_QPATH || len < 5) {
			CG_Error( "PrecacheItem: %s has bad precache string", 
				item->classname);
			return;
		}
		memcpy (data, start, len);
		data[len] = 0;
		if ( *s ) {
			s++;
		}

		if ( !strcmp(data+len-3, "wav" )) {
			trap_S_RegisterSound( data );
		}
	}
}


/*
=================
CG_RegisterSounds

called during a precache command
=================
*/
static void CG_RegisterSounds( void ) {
	int		i;
	char	items[MAX_ITEMS+1];
	char	name[MAX_QPATH];
	const char	*soundName;

	// voice commands
	// rww - no "voice commands" I guess.
	//CG_LoadVoiceChats();

	cgs.media.oneMinuteSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM004" );
	cgs.media.fiveMinuteSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM005" );
	cgs.media.oneFragSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM001" );
	cgs.media.twoFragSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM002" );
	cgs.media.threeFragSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM003");
	cgs.media.count3Sound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM035" );
	cgs.media.count2Sound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM036" );
	cgs.media.count1Sound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM037" );
	cgs.media.countFightSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM038" );

	cgs.media.redSaberGlowShader		= trap_R_RegisterShader( "gfx/effects/sabers/red_glow" );
	cgs.media.redSaberCoreShader		= trap_R_RegisterShader( "gfx/effects/sabers/red_line" );
	cgs.media.orangeSaberGlowShader		= trap_R_RegisterShader( "gfx/effects/sabers/orange_glow" );
	cgs.media.orangeSaberCoreShader		= trap_R_RegisterShader( "gfx/effects/sabers/orange_line" );
	cgs.media.yellowSaberGlowShader		= trap_R_RegisterShader( "gfx/effects/sabers/yellow_glow" );
	cgs.media.yellowSaberCoreShader		= trap_R_RegisterShader( "gfx/effects/sabers/yellow_line" );
	cgs.media.greenSaberGlowShader		= trap_R_RegisterShader( "gfx/effects/sabers/green_glow" );
	cgs.media.greenSaberCoreShader		= trap_R_RegisterShader( "gfx/effects/sabers/green_line" );
	cgs.media.blueSaberGlowShader		= trap_R_RegisterShader( "gfx/effects/sabers/blue_glow" );
	cgs.media.blueSaberCoreShader		= trap_R_RegisterShader( "gfx/effects/sabers/blue_line" );
	cgs.media.purpleSaberGlowShader		= trap_R_RegisterShader( "gfx/effects/sabers/purple_glow" );
	cgs.media.purpleSaberCoreShader		= trap_R_RegisterShader( "gfx/effects/sabers/purple_line" );
	cgs.media.saberBlurShader			= trap_R_RegisterShader( "gfx/effects/sabers/saberBlur" );

	cgs.media.yellowDroppedSaberShader	= trap_R_RegisterShader("gfx/effects/yellow_glow");

	cgs.media.rivetMarkShader			= trap_R_RegisterShader( "gfx/damage/rivetmark" );

	trap_R_RegisterShader( "gfx/effects/saberFlare" );

	trap_R_RegisterShader( "powerups/ysalimarishell" );
	trap_R_RegisterShader("gfx/effects/saberDamageGlow" );
	
	trap_R_RegisterShader( "gfx/effects/forcePush" );

	trap_R_RegisterShader( "gfx/misc/red_dmgshield" );
	trap_R_RegisterShader( "gfx/misc/red_portashield" );
	trap_R_RegisterShader( "gfx/misc/blue_dmgshield" );
	trap_R_RegisterShader( "gfx/misc/blue_portashield" );

	trap_R_RegisterShader( "models/map_objects/imp_mine/turret_chair_dmg.tga" );

	for (i=1 ; i<9 ; i++)
	{
		trap_S_RegisterSound(va("sound/weapons/saber/saberhup%i.wav", i));
	}

	for (i=1 ; i<10 ; i++)
	{
		trap_S_RegisterSound(va("sound/weapons/saber/saberblock%i.wav", i));
	}

	for (i=1 ; i<4 ; i++)
	{
		trap_S_RegisterSound(va("sound/weapons/saber/bounce%i.wav", i));
	}

	trap_S_RegisterSound( "sound/weapons/saber/saberhum1.wav" );
	trap_S_RegisterSound( "sound/weapons/saber/saberon.wav" );
	trap_S_RegisterSound( "sound/weapons/saber/saberoffquick.wav" );
	trap_S_RegisterSound( "sound/weapons/saber/saberhitwall1" );
	trap_S_RegisterSound( "sound/weapons/saber/saberhitwall2" );
	trap_S_RegisterSound( "sound/weapons/saber/saberhitwall3" );
	trap_S_RegisterSound("sound/weapons/saber/saberhit.wav");

	cgs.media.teamHealSound = trap_S_RegisterSound("sound/weapons/force/teamheal.wav");
	cgs.media.teamRegenSound = trap_S_RegisterSound("sound/weapons/force/teamforce.wav");

	trap_S_RegisterSound("sound/weapons/force/heal.wav");
	trap_S_RegisterSound("sound/weapons/force/speed.wav");
	trap_S_RegisterSound("sound/weapons/force/see.wav");
	trap_S_RegisterSound("sound/weapons/force/rage.wav");
	trap_S_RegisterSound("sound/weapons/force/lightning.wav");
	trap_S_RegisterSound("sound/weapons/force/lightninghit.wav");
	trap_S_RegisterSound("sound/weapons/force/drain.wav");
	trap_S_RegisterSound("sound/weapons/force/jumpbuild.wav");
	trap_S_RegisterSound("sound/weapons/force/distract.wav");
	trap_S_RegisterSound("sound/weapons/force/distractstop.wav");
	trap_S_RegisterSound("sound/weapons/force/pull.wav");
	trap_S_RegisterSound("sound/weapons/force/push.wav");

	if (cg_buildScript.integer)
	{
		trap_S_RegisterSound("sound/chars/atst/ATSTcrash.wav");
		trap_S_RegisterSound("sound/chars/atst/ATSTstart.wav");
		trap_S_RegisterSound("sound/chars/atst/ATSTstep1.wav");
		trap_S_RegisterSound("sound/chars/atst/ATSTstep2.wav");

		trap_S_RegisterSound("sound/weapons/atst/ATSTfire1.wav");
		trap_S_RegisterSound("sound/weapons/atst/ATSTfire2.wav");
		trap_S_RegisterSound("sound/weapons/atst/ATSTfire3.wav");
		trap_S_RegisterSound("sound/weapons/atst/ATSTfire4.wav");
	}

	for (i=1 ; i<3 ; i++)
	{
		trap_S_RegisterSound(va("sound/weapons/thermal/bounce%i.wav", i));
	}

	trap_S_RegisterSound("sound/movers/switches/switch2.wav");
	trap_S_RegisterSound("sound/movers/switches/switch3.wav");
	trap_S_RegisterSound("sound/ambience/spark5.wav");
	trap_S_RegisterSound("sound/chars/turret/ping.wav");
	trap_S_RegisterSound("sound/chars/turret/startup.wav");
	trap_S_RegisterSound("sound/chars/turret/shutdown.wav");
	trap_S_RegisterSound("sound/chars/turret/move.wav");
	trap_S_RegisterSound("sound/player/pickuphealth.wav");
	trap_S_RegisterSound("sound/player/pickupshield.wav");

	trap_S_RegisterSound("sound/effects/glassbreak1.wav");

	trap_S_RegisterSound( "sound/weapons/rocket/tick.wav" );
	trap_S_RegisterSound( "sound/weapons/rocket/lock.wav" );

	trap_S_RegisterSound("sound/weapons/force/speedloop.wav");

	trap_S_RegisterSound("sound/weapons/force/protecthit.mp3"); //PDSOUND_PROTECTHIT
	trap_S_RegisterSound("sound/weapons/force/protect.mp3"); //PDSOUND_PROTECT
	trap_S_RegisterSound("sound/weapons/force/absorbhit.mp3"); //PDSOUND_ABSORBHIT
	trap_S_RegisterSound("sound/weapons/force/absorb.mp3"); //PDSOUND_ABSORB
	trap_S_RegisterSound("sound/weapons/force/jump.mp3"); //PDSOUND_FORCEJUMP
	trap_S_RegisterSound("sound/weapons/force/grip.mp3"); //PDSOUND_FORCEGRIP

	if ( cgs.gametype >= GT_TEAM || cg_buildScript.integer ) {

#ifdef JK2AWARDS
		cgs.media.captureAwardSound = trap_S_RegisterSound( "sound/teamplay/flagcapture_yourteam.wav" );
#endif
		cgs.media.redLeadsSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM046");
		cgs.media.blueLeadsSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM045");
		cgs.media.teamsTiedSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM032" );

		cgs.media.redScoredSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM044");
		cgs.media.blueScoredSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM043" );

		if ( cgs.gametype == GT_CTF || cgs.gametype == GT_CTY || cg_buildScript.integer ) {
			cgs.media.redFlagReturnedSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM042" );
			cgs.media.blueFlagReturnedSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM041" );
			cgs.media.redTookFlagSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM040" );
			cgs.media.blueTookFlagSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM039" );

			cgs.media.redYsalReturnedSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM050" );
			cgs.media.blueYsalReturnedSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM049" );
			cgs.media.redTookYsalSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM048" );
			cgs.media.blueTookYsalSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM047" );
		}
	}

	cgs.media.drainSound = trap_S_RegisterSound("sound/weapons/force/drained.mp3");

	cgs.media.happyMusic = trap_S_RegisterSound("music/goodsmall.mp3");
	cgs.media.dramaticFailure = trap_S_RegisterSound("music/badsmall.mp3");

	//PRECACHE ALL MUSIC HERE (don't need to precache normally because it's streamed off the disk)
	if (cg_buildScript.integer)
	{
		trap_S_StartBackgroundTrack( "music/mp/duel.mp3", "music/mp/duel.mp3", qfalse );
	}

	cg.loadLCARSStage = 1;

	cgs.media.selectSound = trap_S_RegisterSound( "sound/weapons/change.wav" );

	cgs.media.teleInSound = trap_S_RegisterSound( "sound/player/telein.wav" );
	cgs.media.teleOutSound = trap_S_RegisterSound( "sound/player/teleout.wav" );
	cgs.media.respawnSound = trap_S_RegisterSound( "sound/items/respawn1.wav" );

	trap_S_RegisterSound( "sound/movers/objects/objectHit.wav" );

	cgs.media.talkSound = trap_S_RegisterSound( "sound/player/talk.wav" );
	cgs.media.landSound = trap_S_RegisterSound( "sound/player/land1.wav");
	cgs.media.fallSound = trap_S_RegisterSound( "sound/player/fallsplat.wav");

	cgs.media.crackleSound = trap_S_RegisterSound( "sound/effects/energy_crackle.wav" );
#ifdef JK2AWARDS
	cgs.media.impressiveSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM025" );
	cgs.media.excellentSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM053" );
	cgs.media.deniedSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM017" );
	cgs.media.humiliationSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM019" );
	cgs.media.defendSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM024" );
#endif

	cgs.media.takenLeadSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM051");
	cgs.media.tiedLeadSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM032");
	cgs.media.lostLeadSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM052");

	cgs.media.rollSound					= trap_S_RegisterSound( "sound/player/roll1.wav");

	cgs.media.watrInSound				= trap_S_RegisterSound( "sound/player/watr_in.wav");
	cgs.media.watrOutSound				= trap_S_RegisterSound( "sound/player/watr_out.wav");
	cgs.media.watrUnSound				= trap_S_RegisterSound( "sound/player/watr_un.wav");

	cgs.media.explosionModel			= trap_R_RegisterModel ( "models/map_objects/mp/sphere.md3" );
	cgs.media.surfaceExplosionShader	= trap_R_RegisterShader( "surfaceExplosion" );

	cgs.media.disruptorShader			= trap_R_RegisterShader( "gfx/effects/burn");

	if (cg_buildScript.integer)
	{
		trap_R_RegisterShader( "gfx/effects/turretflashdie" );
	}

	cgs.media.solidWhite = trap_R_RegisterShader( "gfx/effects/solidWhite_cull" );

	trap_R_RegisterShader("gfx/misc/mp_light_enlight_disable");
	trap_R_RegisterShader("gfx/misc/mp_dark_enlight_disable");

	trap_R_RegisterModel ( "models/map_objects/mp/sphere.md3" );
	trap_R_RegisterModel("models/items/remote.md3");

	cgs.media.holocronPickup = trap_S_RegisterSound( "sound/player/holocron.wav" );

	// Zoom
	cgs.media.zoomStart = trap_S_RegisterSound( "sound/interface/zoomstart.wav" );
	cgs.media.zoomLoop	= trap_S_RegisterSound( "sound/interface/zoomloop.wav" );
	cgs.media.zoomEnd	= trap_S_RegisterSound( "sound/interface/zoomend.wav" );

	for (i=0 ; i<4 ; i++) {
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/boot%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_NORMAL][i] = trap_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/splash%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_SPLASH][i] = trap_S_RegisterSound (name);

		Com_sprintf (name, sizeof(name), "sound/player/footsteps/clank%i.wav", i+1);
		cgs.media.footsteps[FOOTSTEP_METAL][i] = trap_S_RegisterSound (name);

		// should these always be registered??
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/step%i.wav", i+1);
		trap_S_RegisterSound (name);
	}

	// only register the items that the server says we need
	strcpy( items, CG_ConfigString( CS_ITEMS ) );

	for ( i = 1 ; i < bg_numItems ; i++ ) {
//		if ( items[ i ] == '1' || cg_buildScript.integer ) {
			CG_RegisterItemSounds( i );
//		}
	}

	for ( i = 1 ; i < MAX_SOUNDS ; i++ ) {
		soundName = CG_ConfigString( CS_SOUNDS+i );
		if ( !soundName[0] ) {
			break;
		}
		if ( soundName[0] == '*' ) {
			continue;	// custom sound
		}
		cgs.gameSounds[i] = trap_S_RegisterSound( soundName );
	}

	for ( i = 1 ; i < MAX_FX ; i++ ) {
		soundName = CG_ConfigString( CS_EFFECTS+i );
		if ( !soundName[0] ) {
			break;
		}
		if ( soundName[0] == '*' ) {
			continue;	// custom sound
		}
		cgs.gameEffects[i] = trap_FX_RegisterEffect( soundName );
	}

	cg.loadLCARSStage = 2;

	// FIXME: only needed with item
	cgs.media.deploySeeker = trap_S_RegisterSound ("sound/chars/seeker/misc/hiss");
	cgs.media.medkitSound = trap_S_RegisterSound ("sound/items/use_bacta.wav");
	
	cgs.media.winnerSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM006" );
	cgs.media.loserSound = trap_S_RegisterSound( "sound/chars/mothma/misc/40MOM010" );
}


//-------------------------------------
// CG_RegisterEffects
// 
// Handles precaching all effect files
//	and any shader, model, or sound
//	files an effect may use.
//-------------------------------------
static void CG_RegisterEffects( void )
{
	const char	*effectName;
	int			i;

	for ( i = 1 ; i < MAX_FX ; i++ ) 
	{
		effectName = CG_ConfigString( CS_EFFECTS + i );

		if ( !effectName[0] ) 
		{
			break;
		}

		trap_FX_RegisterEffect( effectName );
	}

	// Set up the glass effects mini-system.
	CG_InitGlass();
}

//===================================================================================

extern char *forceHolocronModels[];
/*
=================
CG_RegisterGraphics

This function may execute for a couple of minutes with a slow disk.
=================
*/
static void CG_RegisterGraphics( void ) {
	int			i;
	char		items[MAX_ITEMS+1];
	static char		*sb_nums[11] = {
		"gfx/2d/numbers/zero",
		"gfx/2d/numbers/one",
		"gfx/2d/numbers/two",
		"gfx/2d/numbers/three",
		"gfx/2d/numbers/four",
		"gfx/2d/numbers/five",
		"gfx/2d/numbers/six",
		"gfx/2d/numbers/seven",
		"gfx/2d/numbers/eight",
		"gfx/2d/numbers/nine",
		"gfx/2d/numbers/minus",
	};

	static char		*sb_t_nums[11] = {
		"gfx/2d/numbers/t_zero",
		"gfx/2d/numbers/t_one",
		"gfx/2d/numbers/t_two",
		"gfx/2d/numbers/t_three",
		"gfx/2d/numbers/t_four",
		"gfx/2d/numbers/t_five",
		"gfx/2d/numbers/t_six",
		"gfx/2d/numbers/t_seven",
		"gfx/2d/numbers/t_eight",
		"gfx/2d/numbers/t_nine",
		"gfx/2d/numbers/t_minus",
	};

	static char		*sb_c_nums[11] = {
		"gfx/2d/numbers/c_zero",
		"gfx/2d/numbers/c_one",
		"gfx/2d/numbers/c_two",
		"gfx/2d/numbers/c_three",
		"gfx/2d/numbers/c_four",
		"gfx/2d/numbers/c_five",
		"gfx/2d/numbers/c_six",
		"gfx/2d/numbers/c_seven",
		"gfx/2d/numbers/c_eight",
		"gfx/2d/numbers/c_nine",
		"gfx/2d/numbers/t_minus", //?????
	};

	// clear any references to old media
	memset( &cg.refdef, 0, sizeof( cg.refdef ) );
	trap_R_ClearScene();

	CG_LoadingString( cgs.mapname );

	trap_R_LoadWorldMap( cgs.mapname );

	// precache status bar pics
	CG_LoadingString( "game media" );

	for ( i=0 ; i<11 ; i++) {
		cgs.media.numberShaders[i] = trap_R_RegisterShader( sb_nums[i] );
	}

	cg.loadLCARSStage = 3;

	for ( i=0; i < 11; i++ )
	{
		cgs.media.numberShaders[i]			= trap_R_RegisterShaderNoMip( sb_nums[i] );
		cgs.media.smallnumberShaders[i]		= trap_R_RegisterShaderNoMip( sb_t_nums[i] );
		cgs.media.chunkyNumberShaders[i]	= trap_R_RegisterShaderNoMip( sb_c_nums[i] );
	}

	cgs.media.viewBloodShader = trap_R_RegisterShader( "viewBloodBlend" );

	cgs.media.deferShader = trap_R_RegisterShaderNoMip( "gfx/2d/defer.tga" );

	cgs.media.smokePuffShader = trap_R_RegisterShader( "smokePuff" );
	cgs.media.smokePuffRageProShader = trap_R_RegisterShader( "smokePuffRagePro" );
	cgs.media.shotgunSmokePuffShader = trap_R_RegisterShader( "shotgunSmokePuff" );
	cgs.media.plasmaBallShader = trap_R_RegisterShader( "sprites/plasma1" );
	cgs.media.bloodTrailShader = trap_R_RegisterShader( "bloodTrail" );
	cgs.media.lagometerShader = trap_R_RegisterShader("gfx/2d/lag" );
	cgs.media.connectionShader = trap_R_RegisterShader( "disconnected" );

	cgs.media.waterBubbleShader = trap_R_RegisterShader( "waterBubble" );

	cgs.media.tracerShader = trap_R_RegisterShader( "gfx/misc/tracer" );
	cgs.media.selectShader = trap_R_RegisterShader( "gfx/2d/select" );

	Com_Printf( S_COLOR_CYAN "---------- Fx System Initialization ---------\n" );
	trap_FX_InitSystem();
	Com_Printf( S_COLOR_CYAN "----- Fx System Initialization Complete -----\n" );
	CG_RegisterEffects();


	cgs.effects.turretShotEffect = trap_FX_RegisterEffect( "turret/shot" );

	trap_FX_RegisterEffect("effects/blaster/deflect.efx");

	trap_FX_RegisterEffect("emplaced/dead_smoke.efx");
	trap_FX_RegisterEffect("emplaced/explode.efx");

	trap_FX_RegisterEffect("turret/explode.efx");

	trap_FX_RegisterEffect("spark_explosion.efx");

	trap_FX_RegisterEffect("effects/turret/muzzle_flash.efx");
	trap_FX_RegisterEffect("saber/spark.efx");
	trap_FX_RegisterEffect("mp/spawn.efx");
	trap_FX_RegisterEffect("mp/jedispawn.efx");
	trap_FX_RegisterEffect("mp/itemcone.efx");
	trap_FX_RegisterEffect("blaster/deflect.efx");
	trap_FX_RegisterEffect("saber/saber_block.efx");
	trap_FX_RegisterEffect("saber/spark.efx");
	trap_FX_RegisterEffect("saber/blood_sparks.efx");
	trap_FX_RegisterEffect("blaster/smoke_bolton");
	trap_FX_RegisterEffect("force/confusion.efx");

	trap_FX_RegisterEffect("effects/force/lightning.efx");


	for ( i = 0 ; i < NUM_CROSSHAIRS ; i++ ) {
		cgs.media.crosshairShader[i] = trap_R_RegisterShader( va("gfx/2d/crosshair%c", 'a'+i) );
	}

	cg.loadLCARSStage = 4;

	cgs.media.backTileShader = trap_R_RegisterShader( "gfx/2d/backtile" );
	cgs.media.noammoShader = trap_R_RegisterShader( "icons/noammo" );

	// powerup shaders
	cgs.media.quadShader = trap_R_RegisterShader("powerups/quad" );
	cgs.media.quadWeaponShader = trap_R_RegisterShader("powerups/quadWeapon" );
	cgs.media.battleSuitShader = trap_R_RegisterShader("powerups/battleSuit" );
	cgs.media.battleWeaponShader = trap_R_RegisterShader("powerups/battleWeapon" );
	cgs.media.invisShader = trap_R_RegisterShader("powerups/invisibility" );
	cgs.media.regenShader = trap_R_RegisterShader("powerups/regen" );
	cgs.media.hastePuffShader = trap_R_RegisterShader("hasteSmokePuff" );

	cgs.media.itemRespawningPlaceholder = trap_R_RegisterShader("powerups/placeholder");
	cgs.media.itemRespawningRezOut = trap_R_RegisterShader("powerups/rezout");

	cgs.media.playerShieldDamage = trap_R_RegisterShader("gfx/misc/personalshield");
	cgs.media.forceSightBubble = trap_R_RegisterShader("gfx/misc/sightbubble");
	cgs.media.forceShell = trap_R_RegisterShader("powerups/forceshell");
	cgs.media.sightShell = trap_R_RegisterShader("powerups/sightshell");

	cgs.media.itemHoloModel = trap_R_RegisterModel("models/map_objects/mp/holo.md3");

	if (cgs.gametype == GT_HOLOCRON || cg_buildScript.integer)
	{
		for ( i=0; i < NUM_FORCE_POWERS; i++ )
		{
			if (forceHolocronModels[i] &&
				forceHolocronModels[i][0])
			{
				trap_R_RegisterModel(forceHolocronModels[i]);
			}
		}
	}

	if ( cgs.gametype == GT_CTF || cgs.gametype == GT_CTY || cg_buildScript.integer ) {
		if (cg_buildScript.integer)
		{
			trap_R_RegisterModel( "models/flags/r_flag.md3" );
			trap_R_RegisterModel( "models/flags/b_flag.md3" );
			trap_R_RegisterModel( "models/flags/r_flag_ysal.md3" );
			trap_R_RegisterModel( "models/flags/b_flag_ysal.md3" );
		}

		if (cgs.gametype == GT_CTF)
		{
			cgs.media.redFlagModel = trap_R_RegisterModel( "models/flags/r_flag.md3" );
			cgs.media.blueFlagModel = trap_R_RegisterModel( "models/flags/b_flag.md3" );
		}
		else
		{
			cgs.media.redFlagModel = trap_R_RegisterModel( "models/flags/r_flag_ysal.md3" );
			cgs.media.blueFlagModel = trap_R_RegisterModel( "models/flags/b_flag_ysal.md3" );
		}

		trap_R_RegisterShaderNoMip( "gfx/hud/mpi_rflag_x" );
		trap_R_RegisterShaderNoMip( "gfx/hud/mpi_bflag_x" );

		trap_R_RegisterShaderNoMip( "gfx/hud/mpi_rflag_ys" );
		trap_R_RegisterShaderNoMip( "gfx/hud/mpi_bflag_ys" );

		trap_R_RegisterShaderNoMip( "gfx/hud/mpi_rflag" );
		trap_R_RegisterShaderNoMip( "gfx/hud/mpi_bflag" );

		trap_R_RegisterShaderNoMip("gfx/2d/net.tga");

		cgs.media.flagPoleModel = trap_R_RegisterModel( "models/flag2/flagpole.md3" );
		cgs.media.flagFlapModel = trap_R_RegisterModel( "models/flag2/flagflap3.md3" );

		cgs.media.redFlagFlapSkin = trap_R_RegisterSkin( "models/flag2/red.skin" );
		cgs.media.blueFlagFlapSkin = trap_R_RegisterSkin( "models/flag2/blue.skin" );
		cgs.media.neutralFlagFlapSkin = trap_R_RegisterSkin( "models/flag2/white.skin" );

		cgs.media.redFlagBaseModel = trap_R_RegisterModel( "models/mapobjects/flagbase/red_base.md3" );
		cgs.media.blueFlagBaseModel = trap_R_RegisterModel( "models/mapobjects/flagbase/blue_base.md3" );
		cgs.media.neutralFlagBaseModel = trap_R_RegisterModel( "models/mapobjects/flagbase/ntrl_base.md3" );
	}

	if ( cg_buildScript.integer ) {
		cgs.media.neutralFlagModel = 0;//trap_R_RegisterModel( "models/flags/n_flag.md3" );
		cgs.media.flagShader[0] = 0;//trap_R_RegisterShaderNoMip( "icons/iconf_neutral1" );
		cgs.media.flagShader[1] = 0;//trap_R_RegisterShaderNoMip( "icons/iconf_red2" );
		cgs.media.flagShader[2] = 0;//trap_R_RegisterShaderNoMip( "icons/iconf_blu2" );
		cgs.media.flagShader[3] = 0;//trap_R_RegisterShaderNoMip( "icons/iconf_neutral3" );
	}


	if ( cgs.gametype >= GT_TEAM || cg_buildScript.integer ) {
		cgs.media.teamRedShader = trap_R_RegisterShader( "sprites/team_red" );
		cgs.media.teamBlueShader = trap_R_RegisterShader( "sprites/team_blue" );
		cgs.media.redQuadShader = trap_R_RegisterShader("powerups/blueflag" );
		cgs.media.teamStatusBar = trap_R_RegisterShader( "gfx/2d/colorbar.tga" );
	}
	else if ( cgs.gametype == GT_JEDIMASTER )
	{
		cgs.media.teamRedShader = trap_R_RegisterShader( "sprites/team_red" );
	}

	cgs.media.armorModel = 0;//trap_R_RegisterModel( "models/powerups/armor/armor_yel.md3" );
	cgs.media.armorIcon  = 0;//trap_R_RegisterShaderNoMip( "icons/iconr_yellow" );

	cgs.media.heartShader			= trap_R_RegisterShaderNoMip( "ui/assets/statusbar/selectedhealth.tga" );

	cgs.media.ysaliredShader		= trap_R_RegisterShader( "powerups/ysaliredshell");
	cgs.media.ysaliblueShader		= trap_R_RegisterShader( "powerups/ysaliblueshell");
	cgs.media.ysalimariShader		= trap_R_RegisterShader( "powerups/ysalimarishell");
	cgs.media.boonShader			= trap_R_RegisterShader( "powerups/boonshell");
	cgs.media.endarkenmentShader	= trap_R_RegisterShader( "powerups/endarkenmentshell");
	cgs.media.enlightenmentShader	= trap_R_RegisterShader( "powerups/enlightenmentshell");
	cgs.media.invulnerabilityShader = trap_R_RegisterShader( "powerups/invulnerabilityshell");

#ifdef JK2AWARDS
	cgs.media.medalImpressive		= trap_R_RegisterShaderNoMip( "medal_impressive" );
	cgs.media.medalExcellent		= trap_R_RegisterShaderNoMip( "medal_excellent" );
	cgs.media.medalGauntlet			= trap_R_RegisterShaderNoMip( "medal_gauntlet" );
	cgs.media.medalDefend			= trap_R_RegisterShaderNoMip( "medal_defend" );
	cgs.media.medalAssist			= trap_R_RegisterShaderNoMip( "medal_assist" );
	cgs.media.medalCapture			= trap_R_RegisterShaderNoMip( "medal_capture" );
#endif

	// Binocular interface
	cgs.media.binocularCircle		= trap_R_RegisterShader( "gfx/2d/binCircle" );
	cgs.media.binocularMask			= trap_R_RegisterShader( "gfx/2d/binMask" );
	cgs.media.binocularArrow		= trap_R_RegisterShader( "gfx/2d/binSideArrow" );
	cgs.media.binocularTri			= trap_R_RegisterShader( "gfx/2d/binTopTri" );
	cgs.media.binocularStatic		= trap_R_RegisterShader( "gfx/2d/binocularWindow" );
	cgs.media.binocularOverlay		= trap_R_RegisterShader( "gfx/2d/binocularNumOverlay" );

	cg.loadLCARSStage = 5;

/*
Ghoul2 Insert Start
*/
	CG_InitItems();
/*
Ghoul2 Insert End
*/
	memset( cg_weapons, 0, sizeof( cg_weapons ) );

	// only register the items that the server says we need
	strcpy( items, CG_ConfigString( CS_ITEMS) );

	for ( i = 1 ; i < bg_numItems ; i++ ) {
		if ( items[ i ] == '1' || cg_buildScript.integer ) {
			CG_LoadingItem( i );
			CG_RegisterItemVisuals( i );
		}
	}

	cg.loadLCARSStage = 6;

	cgs.media.glassShardShader	= trap_R_RegisterShader( "gfx/misc/test_crackle" );

	// doing one shader just makes it look like a shell.  By using two shaders with different bulge offsets and different texture scales, it has a much more chaotic look
	cgs.media.electricBodyShader			= trap_R_RegisterShader( "gfx/misc/electric" );
	cgs.media.electricBody2Shader			= trap_R_RegisterShader( "gfx/misc/fullbodyelectric2" );

	// wall marks
	cgs.media.bulletMarkShader	= trap_R_RegisterShader( "gfx/damage/bullet_mrk" );
	cgs.media.burnMarkShader	= trap_R_RegisterShader( "gfx/damage/burn_med_mrk" );
	cgs.media.holeMarkShader	= trap_R_RegisterShader( "gfx/damage/hole_lg_mrk" );
	cgs.media.energyMarkShader	= trap_R_RegisterShader( "gfx/damage/plasma_mrk" );
	cgs.media.shadowMarkShader	= trap_R_RegisterShader( "markShadow" );
	cgs.media.wakeMarkShader	= trap_R_RegisterShader( "wake" );
	cgs.media.bloodMarkShader	= trap_R_RegisterShader( "bloodMark" );

	cgs.media.viewPainShader					= trap_R_RegisterShader( "gfx/misc/borgeyeflare" );
	cgs.media.viewPainShader_Shields			= trap_R_RegisterShader( "gfx/mp/dmgshader_shields" );
	cgs.media.viewPainShader_ShieldsAndHealth	= trap_R_RegisterShader( "gfx/mp/dmgshader_shieldsandhealth" );

	// register the inline models
	cgs.numInlineModels = trap_CM_NumInlineModels();
	for ( i = 1 ; i < cgs.numInlineModels ; i++ ) {
		char	name[10];
		vec3_t			mins, maxs;
		int				j;

		Com_sprintf( name, sizeof(name), "*%i", i );
		cgs.inlineDrawModel[i] = trap_R_RegisterModel( name );
		trap_R_ModelBounds( cgs.inlineDrawModel[i], mins, maxs );
		for ( j = 0 ; j < 3 ; j++ ) {
			cgs.inlineModelMidpoints[i][j] = mins[j] + 0.5 * ( maxs[j] - mins[j] );
		}
	}

	cg.loadLCARSStage = 7;

	// register all the server specified models
	for (i=1 ; i<MAX_MODELS ; i++) {
		const char		*modelName;

		modelName = CG_ConfigString( CS_MODELS+i );
		if ( !modelName[0] ) {
			break;
		}
		cgs.gameModels[i] = trap_R_RegisterModel( modelName );
	}
	cg.loadLCARSStage = 8;
/*
Ghoul2 Insert Start
*/
	CG_LoadingString("skins");
	// register all the server specified models
	for (i=1 ; i<MAX_CHARSKINS ; i++) {
		const char		*modelName;

		modelName = CG_ConfigString( CS_CHARSKINS+i );
		if ( !modelName[0] ) {
			break;
		}
		cgs.skins[i] = trap_R_RegisterSkin( modelName );
	}

	CG_InitG2Weapons();

/*
Ghoul2 Insert End
*/
	cg.loadLCARSStage = 9;


	// new stuff
	cgs.media.patrolShader = trap_R_RegisterShaderNoMip("ui/assets/statusbar/patrol.tga");
	cgs.media.assaultShader = trap_R_RegisterShaderNoMip("ui/assets/statusbar/assault.tga");
	cgs.media.campShader = trap_R_RegisterShaderNoMip("ui/assets/statusbar/camp.tga");
	cgs.media.followShader = trap_R_RegisterShaderNoMip("ui/assets/statusbar/follow.tga");
	cgs.media.defendShader = trap_R_RegisterShaderNoMip("ui/assets/statusbar/defend.tga");
	cgs.media.teamLeaderShader = trap_R_RegisterShaderNoMip("ui/assets/statusbar/team_leader.tga");
	cgs.media.retrieveShader = trap_R_RegisterShaderNoMip("ui/assets/statusbar/retrieve.tga");
	cgs.media.escortShader = trap_R_RegisterShaderNoMip("ui/assets/statusbar/escort.tga");
	cgs.media.cursor = trap_R_RegisterShaderNoMip( "menu/art/3_cursor2" );
	cgs.media.sizeCursor = trap_R_RegisterShaderNoMip( "ui/assets/sizecursor.tga" );
	cgs.media.selectCursor = trap_R_RegisterShaderNoMip( "ui/assets/selectcursor.tga" );
	cgs.media.flagShaders[0] = trap_R_RegisterShaderNoMip("ui/assets/statusbar/flag_in_base.tga");
	cgs.media.flagShaders[1] = trap_R_RegisterShaderNoMip("ui/assets/statusbar/flag_capture.tga");
	cgs.media.flagShaders[2] = trap_R_RegisterShaderNoMip("ui/assets/statusbar/flag_missing.tga");

	cgs.media.halfShieldModel	= trap_R_RegisterModel ( "models/weaphits/testboom.md3" );
	cgs.media.halfShieldShader	= trap_R_RegisterShader( "halfShieldShell" );


	CG_ClearParticles ();
/*
	for (i=1; i<MAX_PARTICLES_AREAS; i++)
	{
		{
			int rval;

			rval = CG_NewParticleArea ( CS_PARTICLES + i);
			if (!rval)
				break;
		}
	}
*/
}


const char *CG_GetStripEdString(char *refSection, char *refName)
{
	static char text[2][1024]={0};	//just incase it's nested
	static int		index = 0;

	index ^= 1;
	trap_SP_GetStringTextString(va("%s_%s", refSection, refName), text[index], sizeof(text[0]));
	return text[index];
}


/*																																			
=======================
CG_BuildSpectatorString

=======================
*/
void CG_BuildSpectatorString(void) {
	int i;
	cg.spectatorList[0] = 0;
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (cgs.clientinfo[i].infoValid && cgs.clientinfo[i].team == TEAM_SPECTATOR ) {
			Q_strcat(cg.spectatorList, sizeof(cg.spectatorList), va("%s     ", cgs.clientinfo[i].name));
		}
	}
	i = strlen(cg.spectatorList);
	if (i != cg.spectatorLen) {
		cg.spectatorLen = i;
		cg.spectatorWidth = -1;
	}
}


/*																																			
===================
CG_RegisterClients
===================
*/
static void CG_RegisterClients( void ) {
	int		i;

	CG_LoadingClient(cg.clientNum);
	CG_NewClientInfo(cg.clientNum, qfalse);

	for (i=0 ; i<MAX_CLIENTS ; i++) {
		const char		*clientInfo;

		if (cg.clientNum == i) {
			continue;
		}

		clientInfo = CG_ConfigString( CS_PLAYERS+i );
		if ( !clientInfo[0]) {
			continue;
		}
		CG_LoadingClient( i );
		CG_NewClientInfo( i, qfalse);
	}
	CG_BuildSpectatorString();
}

//===========================================================================

/*
=================
CG_ConfigString
=================
*/
const char *CG_ConfigString( int index ) {
	if ( index < 0 || index >= MAX_CONFIGSTRINGS ) {
		CG_Error( "CG_ConfigString: bad index: %i", index );
	}
	return cgs.gameState.stringData + cgs.gameState.stringOffsets[ index ];
}

//==================================================================

/*
======================
CG_StartMusic

======================
*/
void CG_StartMusic( qboolean bForceStart ) {
	char	*s;
	char	parm1[MAX_QPATH], parm2[MAX_QPATH];

	// start the background music
	s = (char *)CG_ConfigString( CS_MUSIC );
	Q_strncpyz( parm1, COM_Parse( (const char **)&s ), sizeof( parm1 ) );
	Q_strncpyz( parm2, COM_Parse( (const char **)&s ), sizeof( parm2 ) );

	trap_S_StartBackgroundTrack( parm1, parm2, !bForceStart );
}

char *CG_GetMenuBuffer(const char *filename) {
	int	len;
	fileHandle_t	f;
	static char buf[MAX_MENUFILE];

	len = trap_FS_FOpenFile( filename, &f, FS_READ );
	if ( !f ) {
		trap_Print( va( S_COLOR_RED "menu file not found: %s, using default\n", filename ) );
		return NULL;
	}
	if ( len >= MAX_MENUFILE ) {
		trap_Print( va( S_COLOR_RED "menu file too large: %s is %i, max allowed is %i", filename, len, MAX_MENUFILE ) );
		trap_FS_FCloseFile( f );
		return NULL;
	}

	trap_FS_Read( buf, len, f );
	buf[len] = 0;
	trap_FS_FCloseFile( f );

	return buf;
}

//
// ==============================
// new hud stuff ( mission pack )
// ==============================
//
qboolean CG_Asset_Parse(int handle) {
	pc_token_t token;
	const char *tempStr;

	if (!trap_PC_ReadToken(handle, &token))
		return qfalse;
	if (Q_stricmp(token.string, "{") != 0) {
		return qfalse;
	}
    
	while ( 1 ) {
		if (!trap_PC_ReadToken(handle, &token))
			return qfalse;

		if (Q_stricmp(token.string, "}") == 0) {
			return qtrue;
		}

		// font
		if (Q_stricmp(token.string, "font") == 0) {
			int pointSize;
			if (!PC_String_Parse(handle, &tempStr) || !PC_Int_Parse(handle, &pointSize)) {
				return qfalse;
			}

//			cgDC.registerFont(tempStr, pointSize, &cgDC.Assets.textFont);
			cgDC.Assets.qhMediumFont = cgDC.RegisterFont(tempStr);
			continue;
		}

		// smallFont
		if (Q_stricmp(token.string, "smallFont") == 0) {
			int pointSize;
			if (!PC_String_Parse(handle, &tempStr) || !PC_Int_Parse(handle, &pointSize)) {
				return qfalse;
			}
//			cgDC.registerFont(tempStr, pointSize, &cgDC.Assets.smallFont);
			cgDC.Assets.qhSmallFont = cgDC.RegisterFont(tempStr);
			continue;
		}

		// font
		if (Q_stricmp(token.string, "bigfont") == 0) {
			int pointSize;
			if (!PC_String_Parse(handle, &tempStr) || !PC_Int_Parse(handle, &pointSize)) {
				return qfalse;
			}
//			cgDC.registerFont(tempStr, pointSize, &cgDC.Assets.bigFont);
			cgDC.Assets.qhBigFont = cgDC.RegisterFont(tempStr);
			continue;
		}

		// gradientbar
		if (Q_stricmp(token.string, "gradientbar") == 0) {
			if (!PC_String_Parse(handle, &tempStr)) {
				return qfalse;
			}
			cgDC.Assets.gradientBar = trap_R_RegisterShaderNoMip(tempStr);
			continue;
		}

		// enterMenuSound
		if (Q_stricmp(token.string, "menuEnterSound") == 0) {
			if (!PC_String_Parse(handle, &tempStr)) {
				return qfalse;
			}
			cgDC.Assets.menuEnterSound = trap_S_RegisterSound( tempStr );
			continue;
		}

		// exitMenuSound
		if (Q_stricmp(token.string, "menuExitSound") == 0) {
			if (!PC_String_Parse(handle, &tempStr)) {
				return qfalse;
			}
			cgDC.Assets.menuExitSound = trap_S_RegisterSound( tempStr );
			continue;
		}

		// itemFocusSound
		if (Q_stricmp(token.string, "itemFocusSound") == 0) {
			if (!PC_String_Parse(handle, &tempStr)) {
				return qfalse;
			}
			cgDC.Assets.itemFocusSound = trap_S_RegisterSound( tempStr );
			continue;
		}

		// menuBuzzSound
		if (Q_stricmp(token.string, "menuBuzzSound") == 0) {
			if (!PC_String_Parse(handle, &tempStr)) {
				return qfalse;
			}
			cgDC.Assets.menuBuzzSound = trap_S_RegisterSound( tempStr );
			continue;
		}

		if (Q_stricmp(token.string, "cursor") == 0) {
			if (!PC_String_Parse(handle, &cgDC.Assets.cursorStr)) {
				return qfalse;
			}
			cgDC.Assets.cursor = trap_R_RegisterShaderNoMip( cgDC.Assets.cursorStr);
			continue;
		}

		if (Q_stricmp(token.string, "fadeClamp") == 0) {
			if (!PC_Float_Parse(handle, &cgDC.Assets.fadeClamp)) {
				return qfalse;
			}
			continue;
		}

		if (Q_stricmp(token.string, "fadeCycle") == 0) {
			if (!PC_Int_Parse(handle, &cgDC.Assets.fadeCycle)) {
				return qfalse;
			}
			continue;
		}

		if (Q_stricmp(token.string, "fadeAmount") == 0) {
			if (!PC_Float_Parse(handle, &cgDC.Assets.fadeAmount)) {
				return qfalse;
			}
			continue;
		}

		if (Q_stricmp(token.string, "shadowX") == 0) {
			if (!PC_Float_Parse(handle, &cgDC.Assets.shadowX)) {
				return qfalse;
			}
			continue;
		}

		if (Q_stricmp(token.string, "shadowY") == 0) {
			if (!PC_Float_Parse(handle, &cgDC.Assets.shadowY)) {
				return qfalse;
			}
			continue;
		}

		if (Q_stricmp(token.string, "shadowColor") == 0) {
			if (!PC_Color_Parse(handle, &cgDC.Assets.shadowColor)) {
				return qfalse;
			}
			cgDC.Assets.shadowFadeClamp = cgDC.Assets.shadowColor[3];
			continue;
		}
	}
	return qfalse; // bk001204 - why not?
}

void CG_ParseMenu(const char *menuFile) {
	pc_token_t token;
	int handle;

	handle = trap_PC_LoadSource(menuFile);
	if (!handle)
		handle = trap_PC_LoadSource("ui/testhud.menu");
	if (!handle)
		return;

	while ( 1 ) {
		if (!trap_PC_ReadToken( handle, &token )) {
			break;
		}

		//if ( Q_stricmp( token, "{" ) ) {
		//	Com_Printf( "Missing { in menu file\n" );
		//	break;
		//}

		//if ( menuCount == MAX_MENUS ) {
		//	Com_Printf( "Too many menus!\n" );
		//	break;
		//}

		if ( token.string[0] == '}' ) {
			break;
		}

		if (Q_stricmp(token.string, "assetGlobalDef") == 0) {
			if (CG_Asset_Parse(handle)) {
				continue;
			} else {
				break;
			}
		}


		if (Q_stricmp(token.string, "menudef") == 0) {
			// start a new menu
			Menu_New(handle);
		}
	}
	trap_PC_FreeSource(handle);
}

qboolean CG_Load_Menu(char **p) {
	char *token;

	token = COM_ParseExt((const char **)p, qtrue);

	if (token[0] != '{') {
		return qfalse;
	}

	while ( 1 ) {

		token = COM_ParseExt((const char **)p, qtrue);
    
		if (Q_stricmp(token, "}") == 0) {
			return qtrue;
		}

		if ( !token || token[0] == 0 ) {
			return qfalse;
		}

		CG_ParseMenu(token); 
	}
	return qfalse;
}


static qboolean CG_OwnerDrawHandleKey(int ownerDraw, int flags, float *special, int key) {
	return qfalse;
}


static int CG_FeederCount(float feederID) {
	int i, count;
	count = 0;
	if (feederID == FEEDER_REDTEAM_LIST) {
		for (i = 0; i < cg.numScores; i++) {
			if (cg.scores[i].team == TEAM_RED) {
				count++;
			}
		}
	} else if (feederID == FEEDER_BLUETEAM_LIST) {
		for (i = 0; i < cg.numScores; i++) {
			if (cg.scores[i].team == TEAM_BLUE) {
				count++;
			}
		}
	} else if (feederID == FEEDER_SCOREBOARD) {
		return cg.numScores;
	}
	return count;
}


void CG_SetScoreSelection(void *p) {
	menuDef_t *menu = (menuDef_t*)p;
	playerState_t *ps = &cg.snap->ps;
	int i, red, blue;
	red = blue = 0;
	for (i = 0; i < cg.numScores; i++) {
		if (cg.scores[i].team == TEAM_RED) {
			red++;
		} else if (cg.scores[i].team == TEAM_BLUE) {
			blue++;
		}
		if (ps->clientNum == cg.scores[i].client) {
			cg.selectedScore = i;
		}
	}

	if (menu == NULL) {
		// just interested in setting the selected score
		return;
	}

	if ( cgs.gametype >= GT_TEAM ) {
		int feeder = FEEDER_REDTEAM_LIST;
		i = red;
		if (cg.scores[cg.selectedScore].team == TEAM_BLUE) {
			feeder = FEEDER_BLUETEAM_LIST;
			i = blue;
		}
		Menu_SetFeederSelection(menu, feeder, i, NULL);
	} else {
		Menu_SetFeederSelection(menu, FEEDER_SCOREBOARD, cg.selectedScore, NULL);
	}
}

// FIXME: might need to cache this info
static clientInfo_t * CG_InfoFromScoreIndex(int index, int team, int *scoreIndex) {
	int i, count;
	if ( cgs.gametype >= GT_TEAM ) {
		count = 0;
		for (i = 0; i < cg.numScores; i++) {
			if (cg.scores[i].team == team) {
				if (count == index) {
					*scoreIndex = i;
					return &cgs.clientinfo[cg.scores[i].client];
				}
				count++;
			}
		}
	}
	*scoreIndex = index;
	return &cgs.clientinfo[ cg.scores[index].client ];
}

static const char *CG_FeederItemText(float feederID, int index, int column, qhandle_t *handle) {
	gitem_t *item;
	int scoreIndex = 0;
	clientInfo_t *info = NULL;
	int team = -1;
	score_t *sp = NULL;

	*handle = -1;

	if (feederID == FEEDER_REDTEAM_LIST) {
		team = TEAM_RED;
	} else if (feederID == FEEDER_BLUETEAM_LIST) {
		team = TEAM_BLUE;
	}

	info = CG_InfoFromScoreIndex(index, team, &scoreIndex);
	sp = &cg.scores[scoreIndex];

	if (info && info->infoValid) {
		switch (column) {
			case 0:
				if ( info->powerups & ( 1 << PW_NEUTRALFLAG ) ) {
					item = BG_FindItemForPowerup( PW_NEUTRALFLAG );
					*handle = cg_items[ ITEM_INDEX(item) ].icon;
				} else if ( info->powerups & ( 1 << PW_REDFLAG ) ) {
					item = BG_FindItemForPowerup( PW_REDFLAG );
					*handle = cg_items[ ITEM_INDEX(item) ].icon;
				} else if ( info->powerups & ( 1 << PW_BLUEFLAG ) ) {
					item = BG_FindItemForPowerup( PW_BLUEFLAG );
					*handle = cg_items[ ITEM_INDEX(item) ].icon;
				} else {
					/*	
					if ( info->botSkill > 0 && info->botSkill <= 5 ) {
						*handle = cgs.media.botSkillShaders[ info->botSkill - 1 ];
					} else if ( info->handicap < 100 ) {
					return va("%i", info->handicap );
					}
					*/
				}
			break;
			case 1:
				if (team == -1) {
					return "";
				} else {
					*handle = CG_StatusHandle(info->teamTask);
				}
		  break;
			case 2:
				if ( cg.snap->ps.stats[ STAT_CLIENTS_READY ] & ( 1 << sp->client ) ) {
					return "Ready";
				}
				if (team == -1) {
					if (cgs.gametype == GT_TOURNAMENT) {
						return va("%i/%i", info->wins, info->losses);
					} else if (info->infoValid && info->team == TEAM_SPECTATOR ) {
						return "Spectator";
					} else {
						return "";
					}
				} else {
					if (info->teamLeader) {
						return "Leader";
					}
				}
			break;
			case 3:
				return info->name;
			break;
			case 4:
				return va("%i", info->score);
			break;
			case 5:
				return va("%4i", sp->time);
			break;
			case 6:
				if ( sp->ping == -1 ) {
					return "connecting";
				} 
				return va("%4i", sp->ping);
			break;
		}
	}

	return "";
}

static qhandle_t CG_FeederItemImage(float feederID, int index) {
	return 0;
}

static qboolean CG_FeederSelection(float feederID, int index) {
	if ( cgs.gametype >= GT_TEAM ) {
		int i, count;
		int team = (feederID == FEEDER_REDTEAM_LIST) ? TEAM_RED : TEAM_BLUE;
		count = 0;
		for (i = 0; i < cg.numScores; i++) {
			if (cg.scores[i].team == team) {
				if (index == count) {
					cg.selectedScore = i;
				}
				count++;
			}
		}
	} else {
		cg.selectedScore = index;
	}

	return qtrue;
}

static float CG_Cvar_Get(const char *cvar) {
	char buff[128];
	memset(buff, 0, sizeof(buff));
	trap_Cvar_VariableStringBuffer(cvar, buff, sizeof(buff));
	return atof(buff);
}

void CG_Text_PaintWithCursor(float x, float y, float scale, vec4_t color, const char *text, int cursorPos, char cursor, int limit, int style, int iMenuFont) {
	CG_Text_Paint(x, y, scale, color, text, 0, limit, style, iMenuFont);
}

static int CG_OwnerDrawWidth(int ownerDraw, float scale) {
	switch (ownerDraw) {
	  case CG_GAME_TYPE:
			return CG_Text_Width(CG_GameTypeString(), scale, FONT_MEDIUM);
	  case CG_GAME_STATUS:
			return CG_Text_Width(CG_GetGameStatusText(), scale, FONT_MEDIUM);
			break;
	  case CG_KILLER:
			return CG_Text_Width(CG_GetKillerText(), scale, FONT_MEDIUM);
			break;
	  case CG_RED_NAME:
			return CG_Text_Width(cg_redTeamName.string, scale, FONT_MEDIUM);
			break;
	  case CG_BLUE_NAME:
			return CG_Text_Width(cg_blueTeamName.string, scale, FONT_MEDIUM);
			break;


	}
	return 0;
}

static int CG_PlayCinematic(const char *name, float x, float y, float w, float h) {
  return trap_CIN_PlayCinematic(name, x, y, w, h, CIN_loop);
}

static void CG_StopCinematic(int handle) {
  trap_CIN_StopCinematic(handle);
}

static void CG_DrawCinematic(int handle, float x, float y, float w, float h) {
  trap_CIN_SetExtents(handle, x, y, w, h);
  trap_CIN_DrawCinematic(handle);
}

static void CG_RunCinematicFrame(int handle) {
  trap_CIN_RunCinematic(handle);
}

/*
=================
CG_LoadHudMenu();

=================
*/
void CG_LoadHudMenu() 
{
	cgDC.registerShaderNoMip = &trap_R_RegisterShaderNoMip;
	cgDC.setColor = &trap_R_SetColor;
	cgDC.drawHandlePic = &CG_DrawPic;
	cgDC.drawStretchPic = &trap_R_DrawStretchPic;
	cgDC.drawText = &CG_Text_Paint;
	cgDC.textWidth = &CG_Text_Width;
	cgDC.textHeight = &CG_Text_Height;
	cgDC.registerModel = &trap_R_RegisterModel;
	cgDC.modelBounds = &trap_R_ModelBounds;
	cgDC.fillRect = &CG_FillRect;
	cgDC.drawRect = &CG_DrawRect;   
	cgDC.drawSides = &CG_DrawSides;
	cgDC.drawTopBottom = &CG_DrawTopBottom;
	cgDC.clearScene = &trap_R_ClearScene;
	cgDC.addRefEntityToScene = &trap_R_AddRefEntityToScene;
	cgDC.renderScene = &trap_R_RenderScene;
	cgDC.RegisterFont = &trap_R_RegisterFont;
	cgDC.Font_StrLenPixels = &trap_R_Font_StrLenPixels;
	cgDC.Font_StrLenChars = &trap_R_Font_StrLenChars;
	cgDC.Font_HeightPixels = &trap_R_Font_HeightPixels;
	cgDC.Font_DrawString = &trap_R_Font_DrawString;
	cgDC.AnyLanguage_ReadCharFromString = &trap_AnyLanguage_ReadCharFromString;
	cgDC.ownerDrawItem = &CG_OwnerDraw;
	cgDC.getValue = &CG_GetValue;
	cgDC.ownerDrawVisible = &CG_OwnerDrawVisible;
	cgDC.runScript = &CG_RunMenuScript;
	cgDC.deferScript = &CG_DeferMenuScript;
	cgDC.getTeamColor = &CG_GetTeamColor;
	cgDC.setCVar = trap_Cvar_Set;
	cgDC.getCVarString = trap_Cvar_VariableStringBuffer;
	cgDC.getCVarValue = CG_Cvar_Get;
	cgDC.drawTextWithCursor = &CG_Text_PaintWithCursor;
	//cgDC.setOverstrikeMode = &trap_Key_SetOverstrikeMode;
	//cgDC.getOverstrikeMode = &trap_Key_GetOverstrikeMode;
	cgDC.startLocalSound = &trap_S_StartLocalSound;
	cgDC.ownerDrawHandleKey = &CG_OwnerDrawHandleKey;
	cgDC.feederCount = &CG_FeederCount;
	cgDC.feederItemImage = &CG_FeederItemImage;
	cgDC.feederItemText = &CG_FeederItemText;
	cgDC.feederSelection = &CG_FeederSelection;
	//cgDC.setBinding = &trap_Key_SetBinding;
	//cgDC.getBindingBuf = &trap_Key_GetBindingBuf;
	//cgDC.keynumToStringBuf = &trap_Key_KeynumToStringBuf;
	//cgDC.executeText = &trap_Cmd_ExecuteText;
	cgDC.Error = &Com_Error; 
	cgDC.Print = &Com_Printf; 
	cgDC.ownerDrawWidth = &CG_OwnerDrawWidth;
	//cgDC.Pause = &CG_Pause;
	cgDC.registerSound = &trap_S_RegisterSound;
	cgDC.startBackgroundTrack = &trap_S_StartBackgroundTrack;
	cgDC.stopBackgroundTrack = &trap_S_StopBackgroundTrack;
	cgDC.playCinematic = &CG_PlayCinematic;
	cgDC.stopCinematic = &CG_StopCinematic;
	cgDC.drawCinematic = &CG_DrawCinematic;
	cgDC.runCinematicFrame = &CG_RunCinematicFrame;
	
	Init_Display(&cgDC);

	Menu_Reset();
}

void CG_AssetCache() {
	//if (Assets.textFont == NULL) {
	//  trap_R_RegisterFont("fonts/arial.ttf", 72, &Assets.textFont);
	//}
	//Assets.background = trap_R_RegisterShaderNoMip( ASSET_BACKGROUND );
	//Com_Printf("Menu Size: %i bytes\n", sizeof(Menus));
	cgDC.Assets.gradientBar = trap_R_RegisterShaderNoMip( ASSET_GRADIENTBAR );
	cgDC.Assets.fxBasePic = trap_R_RegisterShaderNoMip( ART_FX_BASE );
	cgDC.Assets.fxPic[0] = trap_R_RegisterShaderNoMip( ART_FX_RED );
	cgDC.Assets.fxPic[1] = trap_R_RegisterShaderNoMip( ART_FX_YELLOW );
	cgDC.Assets.fxPic[2] = trap_R_RegisterShaderNoMip( ART_FX_GREEN );
	cgDC.Assets.fxPic[3] = trap_R_RegisterShaderNoMip( ART_FX_TEAL );
	cgDC.Assets.fxPic[4] = trap_R_RegisterShaderNoMip( ART_FX_BLUE );
	cgDC.Assets.fxPic[5] = trap_R_RegisterShaderNoMip( ART_FX_CYAN );
	cgDC.Assets.fxPic[6] = trap_R_RegisterShaderNoMip( ART_FX_WHITE );
	cgDC.Assets.scrollBar = trap_R_RegisterShaderNoMip( ASSET_SCROLLBAR );
	cgDC.Assets.scrollBarArrowDown = trap_R_RegisterShaderNoMip( ASSET_SCROLLBAR_ARROWDOWN );
	cgDC.Assets.scrollBarArrowUp = trap_R_RegisterShaderNoMip( ASSET_SCROLLBAR_ARROWUP );
	cgDC.Assets.scrollBarArrowLeft = trap_R_RegisterShaderNoMip( ASSET_SCROLLBAR_ARROWLEFT );
	cgDC.Assets.scrollBarArrowRight = trap_R_RegisterShaderNoMip( ASSET_SCROLLBAR_ARROWRIGHT );
	cgDC.Assets.scrollBarThumb = trap_R_RegisterShaderNoMip( ASSET_SCROLL_THUMB );
	cgDC.Assets.sliderBar = trap_R_RegisterShaderNoMip( ASSET_SLIDER_BAR );
	cgDC.Assets.sliderThumb = trap_R_RegisterShaderNoMip( ASSET_SLIDER_THUMB );
}

/*


/*
Ghoul2 Insert Start
*/

// initialise the cg_entities structure - take into account the ghoul2 stl stuff in the active snap shots
void CG_Init_CG(void)
{
	memset( &cg, 0, sizeof(cg));
}

// initialise the cg_entities structure - take into account the ghoul2 stl stuff
void CG_Init_CGents(void)
{
	
	memset(&cg_entities, 0, sizeof(cg_entities));
}


void CG_InitItems(void)
{
	memset( cg_items, 0, sizeof( cg_items ) );
}

/*
Ghoul2 Insert End
*/

forceTicPos_t forceTicPos[] = 
{

 11,  41,  20,  10,	"gfx/hud/force_tick1", 0,		// Left Top
 12,  45,  20,  10, "gfx/hud/force_tick2", 0,
 14,  49,  20,  10, "gfx/hud/force_tick3", 0,
 17,  52,  20,  10, "gfx/hud/force_tick4", 0,
 22,  55,  10,  10, "gfx/hud/force_tick5", 0,
 28,  57,  10,  20, "gfx/hud/force_tick6", 0,
 34,  59,  10,  10,	"gfx/hud/force_tick7", 0,		// Left bottom

 46,  59, -10,  10, "gfx/hud/force_tick7", 0,		// Right bottom
 52,  57, -10,  20, "gfx/hud/force_tick6", 0,
 58,  55, -10,  10, "gfx/hud/force_tick5", 0,
 63,  52, -20,  10, "gfx/hud/force_tick4", 0,
 66,  49, -20,  10, "gfx/hud/force_tick3", 0,
 68,  45, -20,  10, "gfx/hud/force_tick2", 0,
 69,  41, -20,  10,	"gfx/hud/force_tick1", 0,		// Right top
};

forceTicPos_t ammoTicPos[] = 
{
 12,  34,  10,  10, "gfx/hud/ammo_tick7", 0, 	// Bottom
 13,  28,  10,  10, "gfx/hud/ammo_tick6", 0,
 15,  23,  10,  10, "gfx/hud/ammo_tick5", 0,
 19,  19,  10,  10, "gfx/hud/ammo_tick4", 0,
 23,  15,  10,  10, "gfx/hud/ammo_tick3", 0,
 29,  12,  10,  10, "gfx/hud/ammo_tick2", 0,
 34,  11,  10,  10, "gfx/hud/ammo_tick1", 0,

 47,  11, -10,  10, "gfx/hud/ammo_tick1", 0,
 52,  12, -10,  10, "gfx/hud/ammo_tick2", 0,
 58,  15, -10,  10, "gfx/hud/ammo_tick3", 0,
 62,  19, -10,  10, "gfx/hud/ammo_tick4", 0,
 66,  23, -10,  10, "gfx/hud/ammo_tick5", 0,
 68,  28, -10,  10, "gfx/hud/ammo_tick6", 0,
 69,  34, -10,  10, "gfx/hud/ammo_tick7", 0,
};


/*
=================
CG_Init

Called after every level change or subsystem restart
Will perform callbacks to make the loading info screen update.
=================
*/
void CG_Init( int serverMessageNum, int serverCommandSequence, int clientNum ) {
	const char	*s;
	int i = 0;

	trap_CG_RegisterSharedMemory(cg.sharedBuffer);

	// clear everything
/*
Ghoul2 Insert Start
*/

//	memset( cg_entities, 0, sizeof( cg_entities ) );
	CG_Init_CGents();
// this is a No-No now we have stl vector classes in here.
//	memset( &cg, 0, sizeof( cg ) );
	CG_Init_CG();
	CG_InitItems();
/*
Ghoul2 Insert End
*/

	// this is kinda dumb as well, but I need to pre-load some fonts in order to have the text available
	//	to say I'm loading the assets.... which includes loading the fonts. So I'll set these up as reasonable
	//	defaults, then let the menu asset parser (which actually specifies the ingame fonts) load over them
	//	if desired during parse.  Dunno how legal it is to store in these cgDC things, but it causes no harm
	//	and even if/when they get overwritten they'll be legalised by the menu asset parser :-)
//	CG_LoadFonts();
	cgDC.Assets.qhSmallFont  = trap_R_RegisterFont("ocr_a");
	cgDC.Assets.qhMediumFont = trap_R_RegisterFont("ergoec");
	cgDC.Assets.qhBigFont = cgDC.Assets.qhMediumFont;

	memset( &cgs, 0, sizeof( cgs ) );
	memset( cg_weapons, 0, sizeof(cg_weapons) );

	cg.clientNum = clientNum;

	cgs.processedSnapshotNum = serverMessageNum;
	cgs.serverCommandSequence = serverCommandSequence;

	cg.loadLCARSStage		= 0;

	cg.itemSelect = -1;
	cg.forceSelect = -1;
	
	// load a few needed things before we do any screen updates
	cgs.media.charsetShader		= trap_R_RegisterShaderNoMip( "gfx/2d/charsgrid_med" );
	cgs.media.whiteShader		= trap_R_RegisterShader( "white" );

	cgs.media.loadBarLED		= trap_R_RegisterShaderNoMip( "gfx/hud/load_tick" );
	cgs.media.loadBarLEDCap		= trap_R_RegisterShaderNoMip( "gfx/hud/load_tick_cap" );
	cgs.media.loadBarLEDSurround= trap_R_RegisterShaderNoMip( "gfx/hud/mp_levelload" );

	//rww - precache HUD weapon icons here
	//actually, these should be stored in the icon field of each item def
	cgs.media.weaponIcons[WP_STUN_BATON] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_stunbaton");
	cgs.media.weaponIcons_NA[WP_STUN_BATON] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_stunbaton_na");

	cgs.media.weaponIcons[WP_SABER] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_lightsaber");
	cgs.media.weaponIcons_NA[WP_SABER] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_lightsaber_na");

	cgs.media.weaponIcons[WP_BRYAR_PISTOL] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_briar");
	cgs.media.weaponIcons_NA[WP_BRYAR_PISTOL] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_briar_na");

	cgs.media.weaponIcons[WP_BLASTER] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_blaster");
	cgs.media.weaponIcons_NA[WP_BLASTER] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_blaster_na");

	cgs.media.weaponIcons[WP_DISRUPTOR] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_disruptor");
	cgs.media.weaponIcons_NA[WP_DISRUPTOR] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_disruptor_na");

	cgs.media.weaponIcons[WP_BOWCASTER] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_bowcaster");
	cgs.media.weaponIcons_NA[WP_BOWCASTER] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_bowcaster_na");

	cgs.media.weaponIcons[WP_REPEATER] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_repeater");
	cgs.media.weaponIcons_NA[WP_REPEATER] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_repeater_na");

	cgs.media.weaponIcons[WP_DEMP2] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_demp2");
	cgs.media.weaponIcons_NA[WP_DEMP2] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_demp2_na");

	cgs.media.weaponIcons[WP_FLECHETTE] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_flechette");
	cgs.media.weaponIcons_NA[WP_FLECHETTE] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_flechette_na");

	cgs.media.weaponIcons[WP_ROCKET_LAUNCHER] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_merrsonn");
	cgs.media.weaponIcons_NA[WP_ROCKET_LAUNCHER] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_merrsonn_na");

	cgs.media.weaponIcons[WP_THERMAL] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_thermal");
	cgs.media.weaponIcons_NA[WP_THERMAL] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_thermal_na");

	cgs.media.weaponIcons[WP_TRIP_MINE] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_tripmine");
	cgs.media.weaponIcons_NA[WP_TRIP_MINE] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_tripmine_na");

	cgs.media.weaponIcons[WP_DET_PACK] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_detpack");
	cgs.media.weaponIcons_NA[WP_DET_PACK] = trap_R_RegisterShaderNoMip("gfx/hud/w_icon_detpack_na");

	// HUD artwork for cycling inventory,weapons and force powers 
	cgs.media.weaponIconBackground		= trap_R_RegisterShaderNoMip( "gfx/hud/background");
	cgs.media.weaponProngsOn			= trap_R_RegisterShaderNoMip( "gfx/hud/prong_on_w");
	cgs.media.weaponProngsOff			= trap_R_RegisterShaderNoMip( "gfx/hud/prong_off");
	cgs.media.forceProngsOn				= trap_R_RegisterShaderNoMip( "gfx/hud/prong_on_f");
	cgs.media.forceIconBackground		= trap_R_RegisterShaderNoMip( "gfx/hud/background_f");
	cgs.media.inventoryIconBackground	= trap_R_RegisterShaderNoMip( "gfx/hud/background_i");
	cgs.media.inventoryProngsOn			= trap_R_RegisterShaderNoMip( "gfx/hud/prong_on_i");

	//rww - precache holdable item icons here
	while (i < bg_numItems)
	{
		if (bg_itemlist[i].giType == IT_HOLDABLE)
		{
			if (bg_itemlist[i].icon)
			{
				cgs.media.invenIcons[bg_itemlist[i].giTag] = trap_R_RegisterShaderNoMip(bg_itemlist[i].icon);
			}
			else
			{
				cgs.media.invenIcons[bg_itemlist[i].giTag] = 0;
			}
		}

		i++;
	}

	//rww - precache force power icons here
	i = 0;

	while (i < NUM_FORCE_POWERS)
	{
		cgs.media.forcePowerIcons[i] = trap_R_RegisterShaderNoMip(HolocronIcons[i]);

		i++;
	}
	cgs.media.rageRecShader = trap_R_RegisterShaderNoMip("gfx/mp/f_icon_ragerec");

	//rww - precache other HUD graphics
	cgs.media.HUDLeftFrame		= trap_R_RegisterShaderNoMip( "gfx/hud/static_test" );
	cgs.media.HUDInnerLeft		= trap_R_RegisterShaderNoMip( "gfx/hud/hudleft_innerframe" );
	cgs.media.HUDArmor1			= trap_R_RegisterShaderNoMip( "gfx/hud/armor1" );
	cgs.media.HUDArmor2			= trap_R_RegisterShaderNoMip( "gfx/hud/armor2" );
	cgs.media.HUDHealth			= trap_R_RegisterShaderNoMip( "gfx/hud/health" );
	cgs.media.HUDHealthTic		= trap_R_RegisterShaderNoMip( "gfx/hud/health_tic" );
	cgs.media.HUDArmorTic		= trap_R_RegisterShaderNoMip( "gfx/hud/armor_tic" );
	
	cgs.media.HUDLeftStatic		= cgs.media.HUDLeftFrame;//trap_R_RegisterShaderNoMip( "gfx/hud/static_test" );
	cgs.media.HUDLeft			= cgs.media.HUDInnerLeft;//trap_R_RegisterShaderNoMip( "gfx/hud/hudleft" );

	cgs.media.HUDSaberStyle1	= trap_R_RegisterShader( "gfx/hud/saber_stylesFast"   );
	cgs.media.HUDSaberStyle2	= trap_R_RegisterShader( "gfx/hud/saber_stylesMed"	  );
	cgs.media.HUDSaberStyle3	= trap_R_RegisterShader( "gfx/hud/saber_stylesStrong" );

	cgs.media.HUDRightFrame		= trap_R_RegisterShaderNoMip("gfx/hud/hudrightframe");
	cgs.media.HUDInnerRight		= trap_R_RegisterShaderNoMip( "gfx/hud/hudright_innerframe" );

	// Load tics
	for (i=0;i<MAX_TICS;i++)
	{
		forceTicPos[i].tic		= trap_R_RegisterShaderNoMip( forceTicPos[i].file );
		ammoTicPos[i].tic		= trap_R_RegisterShaderNoMip( ammoTicPos[i].file );
	}


	CG_RegisterCvars();

	CG_InitConsoleCommands();

	cg.weaponSelect = WP_BRYAR_PISTOL;

	cgs.redflag = cgs.blueflag = -1; // For compatibily, default to unset for
	cgs.flagStatus = -1;
	// old servers

	// get the rendering configuration from the client system
	trap_GetGlconfig( &cgs.glconfig );
	cgs.screenXScale = cgs.glconfig.vidWidth / 640.0;
	cgs.screenYScale = cgs.glconfig.vidHeight / 480.0;

	// get the gamestate from the client system
	trap_GetGameState( &cgs.gameState );

	// check version
	s = CG_ConfigString( CS_GAME_VERSION );
	if ( strcmp( s, GAME_VERSION ) ) {
		CG_Error( "Client/Server game mismatch: %s/%s", GAME_VERSION, s );
	}

	s = CG_ConfigString( CS_LEVEL_START_TIME );
	cgs.levelStartTime = atoi( s );

	CG_ParseServerinfo();

	// load the new map
	CG_LoadingString( "collision map" );

	trap_CM_LoadMap( cgs.mapname );

	String_Init();

	cg.loading = qtrue;		// force players to load instead of defer

	CG_InitSagaMode();

	CG_LoadingString( "sounds" );

	CG_RegisterSounds();

	CG_LoadingString( "graphics" );

	CG_RegisterGraphics();

	CG_LoadingString( "clients" );

	CG_RegisterClients();		// if low on memory, some clients will be deferred

	CG_AssetCache();
	CG_LoadHudMenu();      // load new hud stuff

	cg.loading = qfalse;	// future players will be deferred

	CG_InitLocalEntities();

	CG_InitMarkPolys();

	// remove the last loading update
	cg.infoScreenText[0] = 0;

	// Make sure we have update values (scores)
	CG_SetConfigValues();

	CG_StartMusic(qfalse);

	CG_LoadingString( "Clearing light styles" );
	CG_ClearLightStyles();

	CG_LoadingString( "" );

	CG_InitTeamChat();

	CG_ShaderStateChanged();

	trap_S_ClearLoopingSounds( qtrue );
}

/*
=================
CG_Shutdown

Called before every level change or subsystem restart
=================
*/
void CG_Shutdown( void ) 
{
	trap_FX_FreeSystem();
	trap_ROFF_Clean();

	CG_ShutDownG2Weapons();

	// some mods may need to do cleanup work here,
	// like closing files or archiving session data
}


/*
===============
CG_NextForcePower_f
===============
*/
void CG_NextForcePower_f( void ) 
{
	if ( !cg.snap )
	{
		return;
	}

	if (cg.snap->ps.pm_flags & PMF_FOLLOW)
	{
		return;
	}

//	BG_CycleForce(&cg.snap->ps, 1);
	if (cg.forceSelect != -1)
	{
		cg.snap->ps.fd.forcePowerSelected = cg.forceSelect;
	}

	BG_CycleForce(&cg.snap->ps, 1);

	if (cg.snap->ps.fd.forcePowersKnown & (1 << cg.snap->ps.fd.forcePowerSelected))
	{
		cg.forceSelect = cg.snap->ps.fd.forcePowerSelected;
		cg.forceSelectTime = cg.time;
	}
}

/*
===============
CG_PrevForcePower_f
===============
*/
void CG_PrevForcePower_f( void ) 
{
	if ( !cg.snap )
	{
		return;
	}

	if (cg.snap->ps.pm_flags & PMF_FOLLOW)
	{
		return;
	}

//	BG_CycleForce(&cg.snap->ps, -1);
	if (cg.forceSelect != -1)
	{
		cg.snap->ps.fd.forcePowerSelected = cg.forceSelect;
	}

	BG_CycleForce(&cg.snap->ps, -1);

	if (cg.snap->ps.fd.forcePowersKnown & (1 << cg.snap->ps.fd.forcePowerSelected))
	{
		cg.forceSelect = cg.snap->ps.fd.forcePowerSelected;
		cg.forceSelectTime = cg.time;
	}
}

void CG_NextInventory_f(void)
{
	if ( !cg.snap )
	{
		return;
	}

	if (cg.snap->ps.pm_flags & PMF_FOLLOW)
	{
		return;
	}

	if (cg.itemSelect != -1)
	{
		cg.snap->ps.stats[STAT_HOLDABLE_ITEM] = BG_GetItemIndexByTag(cg.itemSelect, IT_HOLDABLE);
	}
	BG_CycleInven(&cg.snap->ps, 1);

	if (cg.snap->ps.stats[STAT_HOLDABLE_ITEM])
	{
		cg.itemSelect = bg_itemlist[cg.snap->ps.stats[STAT_HOLDABLE_ITEM]].giTag;
		cg.invenSelectTime = cg.time;
	}
}

void CG_PrevInventory_f(void)
{
	if ( !cg.snap )
	{
		return;
	}

	if (cg.snap->ps.pm_flags & PMF_FOLLOW)
	{
		return;
	}

	if (cg.itemSelect != -1)
	{
		cg.snap->ps.stats[STAT_HOLDABLE_ITEM] = BG_GetItemIndexByTag(cg.itemSelect, IT_HOLDABLE);
	}
	BG_CycleInven(&cg.snap->ps, -1);

	if (cg.snap->ps.stats[STAT_HOLDABLE_ITEM])
	{
		cg.itemSelect = bg_itemlist[cg.snap->ps.stats[STAT_HOLDABLE_ITEM]].giTag;
		cg.invenSelectTime = cg.time;
	}
}
