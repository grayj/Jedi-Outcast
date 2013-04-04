//
/*
=======================================================================

FORCE INTERFACE 

=======================================================================
*/

// use this to get a demo build without an explicit demo build, i.e. to get the demo ui files to build
#include "ui_local.h"
#include "../qcommon/qfiles.h"
#include "ui_force.h"

int uiForceSide = FORCE_LIGHTSIDE;
int uiForceRank = FORCE_MASTERY_JEDI_KNIGHT;
int uiMaxRank = MAX_FORCE_RANK;
int uiMaxPoints = 20;
int	uiForceUsed = 0;
int uiForceAvailable=0;

qboolean gTouchedForce = qfalse;
vmCvar_t	ui_freeSaber, ui_forcePowerDisable;
void Menu_ShowItemByName(menuDef_t *menu, const char *p, qboolean bShow);

int uiForcePowersRank[NUM_FORCE_POWERS] = {
	0,//FP_HEAL = 0,//instant
	1,//FP_LEVITATION,//hold/duration, this one defaults to 1 (gives a free point)
	0,//FP_SPEED,//duration
	0,//FP_PUSH,//hold/duration
	0,//FP_PULL,//hold/duration
	0,//FP_TELEPATHY,//instant
	0,//FP_GRIP,//hold/duration
	0,//FP_LIGHTNING,//hold/duration
	0,//FP_RAGE,//duration
	0,//FP_PROTECT,
	0,//FP_ABSORB,
	0,//FP_TEAM_HEAL,
	0,//FP_TEAM_FORCE,
	0,//FP_DRAIN,
	0,//FP_SEE,
	1,//FP_SABERATTACK, //default to 1 point in attack
	1,//FP_SABERDEFEND, //defualt to 1 point in defense
	0//FP_SABERTHROW,
};

int uiForcePowerDarkLight[NUM_FORCE_POWERS] = //0 == neutral
{ //nothing should be usable at rank 0..
	FORCE_LIGHTSIDE,//FP_HEAL,//instant
	0,//FP_LEVITATION,//hold/duration
	0,//FP_SPEED,//duration
	0,//FP_PUSH,//hold/duration
	0,//FP_PULL,//hold/duration
	FORCE_LIGHTSIDE,//FP_TELEPATHY,//instant
	FORCE_DARKSIDE,//FP_GRIP,//hold/duration
	FORCE_DARKSIDE,//FP_LIGHTNING,//hold/duration
	FORCE_DARKSIDE,//FP_RAGE,//duration
	FORCE_LIGHTSIDE,//FP_PROTECT,//duration
	FORCE_LIGHTSIDE,//FP_ABSORB,//duration
	FORCE_LIGHTSIDE,//FP_TEAM_HEAL,//instant
	FORCE_DARKSIDE,//FP_TEAM_FORCE,//instant
	FORCE_DARKSIDE,//FP_DRAIN,//hold/duration
	0,//FP_SEE,//duration
	0,//FP_SABERATTACK,
	0,//FP_SABERDEFEND,
	0//FP_SABERTHROW,
		//NUM_FORCE_POWERS
};

int uiForceStarShaders[NUM_FORCE_STAR_IMAGES][2];
int uiSaberColorShaders[NUM_SABER_COLORS];
void UI_InitForceShaders(void)
{
	uiForceStarShaders[0][0] = trap_R_RegisterShaderNoMip("forcestar0");
	uiForceStarShaders[0][1] = trap_R_RegisterShaderNoMip("forcestar0");
	uiForceStarShaders[1][0] = trap_R_RegisterShaderNoMip("forcecircle1");
	uiForceStarShaders[1][1] = trap_R_RegisterShaderNoMip("forcestar1");
	uiForceStarShaders[2][0] = trap_R_RegisterShaderNoMip("forcecircle2");
	uiForceStarShaders[2][1] = trap_R_RegisterShaderNoMip("forcestar2");
	uiForceStarShaders[3][0] = trap_R_RegisterShaderNoMip("forcecircle3");
	uiForceStarShaders[3][1] = trap_R_RegisterShaderNoMip("forcestar3");
	uiForceStarShaders[4][0] = trap_R_RegisterShaderNoMip("forcecircle4");
	uiForceStarShaders[4][1] = trap_R_RegisterShaderNoMip("forcestar4");
	uiForceStarShaders[5][0] = trap_R_RegisterShaderNoMip("forcecircle5");
	uiForceStarShaders[5][1] = trap_R_RegisterShaderNoMip("forcestar5");
	uiForceStarShaders[6][0] = trap_R_RegisterShaderNoMip("forcecircle6");
	uiForceStarShaders[6][1] = trap_R_RegisterShaderNoMip("forcestar6");
	uiForceStarShaders[7][0] = trap_R_RegisterShaderNoMip("forcecircle7");
	uiForceStarShaders[7][1] = trap_R_RegisterShaderNoMip("forcestar7");
	uiForceStarShaders[8][0] = trap_R_RegisterShaderNoMip("forcecircle8");
	uiForceStarShaders[8][1] = trap_R_RegisterShaderNoMip("forcestar8");

	uiSaberColorShaders[SABER_RED]		= trap_R_RegisterShaderNoMip("menu/art/saber_red");
	uiSaberColorShaders[SABER_ORANGE]	= trap_R_RegisterShaderNoMip("menu/art/saber_orange");
	uiSaberColorShaders[SABER_YELLOW]	= trap_R_RegisterShaderNoMip("menu/art/saber_yellow");
	uiSaberColorShaders[SABER_GREEN]	= trap_R_RegisterShaderNoMip("menu/art/saber_green");
	uiSaberColorShaders[SABER_BLUE]		= trap_R_RegisterShaderNoMip("menu/art/saber_blue");
	uiSaberColorShaders[SABER_PURPLE]	= trap_R_RegisterShaderNoMip("menu/art/saber_purple");
}

// Draw the stars spent on the current force power
void UI_DrawForceStars(rectDef_t *rect, float scale, vec4_t color, int textStyle, int forceindex, int val, int min, int max) 
{
	int	i,pad = 4;
	int	xPos,width = 16;
	int starcolor;

	if (val < min || val > max) 
	{
		val = min;
	}

	if (1)	// if (val)
	{
		xPos = rect->x;

		for (i=FORCE_LEVEL_1;i<=max;i++)
		{
			starcolor = bgForcePowerCost[forceindex][i];

			if (val >= i)
			{	// Draw a star.
				UI_DrawHandlePic( xPos, rect->y+6, width, width, uiForceStarShaders[starcolor][1] );
			}
			else
			{	// Draw a circle.
				UI_DrawHandlePic( xPos, rect->y+6, width, width, uiForceStarShaders[starcolor][0] );
			}
			xPos += width + pad;
		}
	}
}

// Set the client's force power layout.
void UI_UpdateClientForcePowers(const char *teamArg)
{
	trap_Cvar_Set( "forcepowers", va("%i-%i-%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i",
		uiForceRank, uiForceSide, uiForcePowersRank[0], uiForcePowersRank[1],
		uiForcePowersRank[2], uiForcePowersRank[3], uiForcePowersRank[4],
		uiForcePowersRank[5], uiForcePowersRank[6], uiForcePowersRank[7],
		uiForcePowersRank[8], uiForcePowersRank[9], uiForcePowersRank[10],
		uiForcePowersRank[11], uiForcePowersRank[12], uiForcePowersRank[13],
		uiForcePowersRank[14], uiForcePowersRank[15], uiForcePowersRank[16],
		uiForcePowersRank[17]) );

	if (gTouchedForce)
	{
		if (teamArg && teamArg[0])
		{
			trap_Cmd_ExecuteText( EXEC_APPEND, va("forcechanged \"%s\"\n", teamArg) );
		}
		else
		{
			trap_Cmd_ExecuteText( EXEC_APPEND, "forcechanged\n" );
		}
	}

	gTouchedForce = qfalse;
}

int UI_TranslateFCFIndex(int index)
{
	if (uiForceSide == FORCE_LIGHTSIDE)
	{
		return index-uiInfo.forceConfigLightIndexBegin;
	}

	return index-uiInfo.forceConfigDarkIndexBegin;
}

void UI_SaveForceTemplate()
{
	char *selectedName = UI_Cvar_VariableString("ui_SaveFCF");
	char fcfString[512];
	char forceStringValue[4];
	fileHandle_t f;
	int strPlace = 0;
	int forcePlace = 0;
	int i = 0;
	qboolean foundFeederItem = qfalse;

	if (!selectedName || !selectedName[0])
	{
		Com_Printf("You did not provide a name for the template.\n");
		return;
	}

	if (uiForceSide == FORCE_LIGHTSIDE)
	{ //write it into the light side folder
		trap_FS_FOpenFile(va("forcecfg/light/%s.fcf", selectedName), &f, FS_WRITE);
	}
	else
	{ //if it isn't light it must be dark
		trap_FS_FOpenFile(va("forcecfg/dark/%s.fcf", selectedName), &f, FS_WRITE);
	}

	if (!f)
	{
		Com_Printf("There was an error writing the template file (read-only?).\n");
		return;
	}

	Com_sprintf(fcfString, sizeof(fcfString), "%i-%i-", uiForceRank, uiForceSide);
	strPlace = strlen(fcfString);

	while (forcePlace < NUM_FORCE_POWERS)
	{
		Com_sprintf(forceStringValue, sizeof(forceStringValue), "%i", uiForcePowersRank[forcePlace]);
		//Just use the force digit even if multiple digits. Shouldn't be longer than 1.
		fcfString[strPlace] = forceStringValue[0];
		strPlace++;
		forcePlace++;
	}
	fcfString[strPlace] = '\n';
	fcfString[strPlace+1] = 0;

	trap_FS_Write(fcfString, strlen(fcfString), f);
	trap_FS_FCloseFile(f);

	Com_Printf("Template saved as \"%s\".\n", selectedName);

	//Now, update the FCF list
	UI_LoadForceConfig_List();

	//Then, scroll through and select the template for the file we just saved
	while (i < uiInfo.forceConfigCount)
	{
		if (!Q_stricmp(uiInfo.forceConfigNames[i], selectedName))
		{
			if ((uiForceSide == FORCE_LIGHTSIDE && uiInfo.forceConfigSide[i]) ||
				(uiForceSide == FORCE_DARKSIDE && !uiInfo.forceConfigSide[i]))
			{
				Menu_SetFeederSelection(NULL, FEEDER_FORCECFG, UI_TranslateFCFIndex(i), NULL);
				foundFeederItem = qtrue;
			}
		}

		i++;
	}

	//Else, go back to 0
	if (!foundFeederItem)
	{
		Menu_SetFeederSelection(NULL, FEEDER_FORCECFG, 0, NULL);
	}
}

// 

void UpdateForceUsed()
{
	int curpower, currank;
	menuDef_t *menu;

	// Currently we don't make a distinction between those that wish to play Jedi of lower than maximum skill.
	uiForceRank = uiMaxRank;

	uiForceUsed = 0;
	uiForceAvailable = forceMasteryPoints[uiForceRank];

	// Make sure that we have one freebie in jump.
	if (uiForcePowersRank[FP_LEVITATION]<1)
	{
		uiForcePowersRank[FP_LEVITATION]=1;
	}
	
	menu = Menus_FindByName("ingame_playerforce");
	// Set the cost of the saberattack according to whether its free.
	if (ui_freeSaber.integer)
	{	// Make saber free
		bgForcePowerCost[FP_SABERATTACK][FORCE_LEVEL_1] = 0;
		bgForcePowerCost[FP_SABERDEFEND][FORCE_LEVEL_1] = 0;
		// Make sure that we have one freebie in saber if applicable.
		if (uiForcePowersRank[FP_SABERATTACK]<1)
		{
			uiForcePowersRank[FP_SABERATTACK]=1;
		}
		if (uiForcePowersRank[FP_SABERDEFEND]<1)
		{
			uiForcePowersRank[FP_SABERDEFEND]=1;
		}
		if (menu)
		{
			Menu_ShowItemByName(menu, "setfp_saberdefend", qtrue);
			Menu_ShowItemByName(menu, "setfp_saberthrow", qtrue);
			Menu_ShowItemByName(menu, "effectentry", qtrue);
			Menu_ShowItemByName(menu, "effectfield", qtrue);
			Menu_ShowItemByName(menu, "nosaber", qfalse);
		}
	}
	else
	{	// Make saber normal cost
		bgForcePowerCost[FP_SABERATTACK][FORCE_LEVEL_1] = 1;
		bgForcePowerCost[FP_SABERDEFEND][FORCE_LEVEL_1] = 1;
		// Also, check if there is no saberattack.  If there isn't, there had better not be any defense or throw!
		if (uiForcePowersRank[FP_SABERATTACK]<1)
		{
			uiForcePowersRank[FP_SABERDEFEND]=0;
			uiForcePowersRank[FP_SABERTHROW]=0;
			if (menu)
			{
				Menu_ShowItemByName(menu, "setfp_saberdefend", qfalse);
				Menu_ShowItemByName(menu, "setfp_saberthrow", qfalse);
				Menu_ShowItemByName(menu, "effectentry", qfalse);
				Menu_ShowItemByName(menu, "effectfield", qfalse);
				Menu_ShowItemByName(menu, "nosaber", qtrue);
			}
		}
		else
		{
			if (menu)
			{
				Menu_ShowItemByName(menu, "setfp_saberdefend", qtrue);
				Menu_ShowItemByName(menu, "setfp_saberthrow", qtrue);
				Menu_ShowItemByName(menu, "effectentry", qtrue);
				Menu_ShowItemByName(menu, "effectfield", qtrue);
				Menu_ShowItemByName(menu, "nosaber", qfalse);
			}
		}
	}

	// Make sure that we're still legal.
	for (curpower=0;curpower<NUM_FORCE_POWERS;curpower++)
	{	// Make sure that our ranks are within legal limits.
		if (uiForcePowersRank[curpower]<0)
			uiForcePowersRank[curpower]=0;
		else if (uiForcePowersRank[curpower]>=NUM_FORCE_POWER_LEVELS)
			uiForcePowersRank[curpower]=(NUM_FORCE_POWER_LEVELS-1);

		for (currank=FORCE_LEVEL_1;currank<=uiForcePowersRank[curpower];currank++)
		{	// Check on this force power
			if (uiForcePowersRank[curpower]>0)
			{	// Do not charge the player for the one freebie in jump, or if there is one in saber.
				if  (	(curpower == FP_LEVITATION && currank == FORCE_LEVEL_1) ||
						(curpower == FP_SABERATTACK && currank == FORCE_LEVEL_1 && ui_freeSaber.integer) ||
						(curpower == FP_SABERDEFEND && currank == FORCE_LEVEL_1 && ui_freeSaber.integer) )
				{
					// Do nothing (written this way for clarity)
				}
				else
				{	// Check if we can accrue the cost of this power.
					if (bgForcePowerCost[curpower][currank] > uiForceAvailable)
					{	// We can't afford this power.  Break to the next one.
						// Remove this power from the player's roster.
						uiForcePowersRank[curpower] = currank-1;
						break;
					}
					else
					{	// Sure we can afford it.
						uiForceUsed += bgForcePowerCost[curpower][currank];
						uiForceAvailable -= bgForcePowerCost[curpower][currank];
					}
				}
			}
		}
	}

}


//Mostly parts of other functions merged into one another.
//Puts the current UI stuff into a string, legalizes it, and then reads it back out.
void UI_ReadLegalForce(void)
{
	char fcfString[512];
	char forceStringValue[4];
	int strPlace = 0;
	int forcePlace = 0;
	int i = 0;
	char singleBuf[64];
	char info[MAX_INFO_VALUE];
	int c = 0;
	int iBuf = 0;
	int forcePowerRank = 0;
	int currank = 0;
	int forceTeam = 0;
	qboolean updateForceLater = qfalse;

	//First, stick them into a string.
	Com_sprintf(fcfString, sizeof(fcfString), "%i-%i-", uiForceRank, uiForceSide);
	strPlace = strlen(fcfString);

	while (forcePlace < NUM_FORCE_POWERS)
	{
		Com_sprintf(forceStringValue, sizeof(forceStringValue), "%i", uiForcePowersRank[forcePlace]);
		//Just use the force digit even if multiple digits. Shouldn't be longer than 1.
		fcfString[strPlace] = forceStringValue[0];
		strPlace++;
		forcePlace++;
	}
	fcfString[strPlace] = '\n';
	fcfString[strPlace+1] = 0;

	info[0] = '\0';
	trap_GetConfigString(CS_SERVERINFO, info, sizeof(info));

	if (atoi( Info_ValueForKey( info, "g_forceBasedTeams" ) ))
	{
		switch((int)(trap_Cvar_VariableValue("ui_myteam")))
		{
		case TEAM_RED:
			forceTeam = FORCE_DARKSIDE;
			break;
		case TEAM_BLUE:
			forceTeam = FORCE_LIGHTSIDE;
			break;
		default:
			break;
		}
	}
	//Second, legalize them.
	if (!BG_LegalizedForcePowers(fcfString, uiMaxRank, ui_freeSaber.integer, forceTeam, atoi( Info_ValueForKey( info, "g_gametype" )), 0))
	{ //if they were illegal, we should refresh them.
		updateForceLater = qtrue;
	}

	//Lastly, put them back into the UI storage from the legalized string
	i = 0;

	while (fcfString[i] && fcfString[i] != '-')
	{
		singleBuf[c] = fcfString[i];
		c++;
		i++;
	}
	singleBuf[c] = 0;
	c = 0;
	i++;

	iBuf = atoi(singleBuf);

	if (iBuf > uiMaxRank || iBuf < 0)
	{ //this force config uses a rank level higher than our currently restricted level.. so we can't use it
	  //FIXME: Print a message indicating this to the user
	//	return;
	}

	uiForceRank = iBuf;

	while (fcfString[i] && fcfString[i] != '-')
	{
		singleBuf[c] = fcfString[i];
		c++;
		i++;
	}
	singleBuf[c] = 0;
	c = 0;
	i++;

	uiForceSide = atoi(singleBuf);

	if (uiForceSide != FORCE_LIGHTSIDE &&
		uiForceSide != FORCE_DARKSIDE)
	{
		uiForceSide = FORCE_LIGHTSIDE;
		return;
	}

	//clear out the existing powers
	while (c < NUM_FORCE_POWERS)
	{
		uiForcePowersRank[c] = 0;
		c++;
	}
	uiForceUsed = 0;
	uiForceAvailable = forceMasteryPoints[uiForceRank];
	gTouchedForce = qtrue;

	for (c=0;fcfString[i]&&c<NUM_FORCE_POWERS;c++,i++)
	{
		singleBuf[0] = fcfString[i];
		singleBuf[1] = 0;
		iBuf = atoi(singleBuf);	// So, that means that Force Power "c" wants to be set to rank "iBuf".
		
		if (iBuf < 0)
		{
			iBuf = 0;
		}

		forcePowerRank = iBuf;

		if (forcePowerRank > FORCE_LEVEL_3 || forcePowerRank < 0)
		{ //err..  not correct
			continue;  // skip this power
		}

		if (uiForcePowerDarkLight[c] && uiForcePowerDarkLight[c] != uiForceSide)
		{ //Apparently the user has crafted a force config that has powers that don't fit with the config's side.
			continue;  // skip this power
		}

		// Accrue cost for each assigned rank for this power.
		for (currank=FORCE_LEVEL_1;currank<=forcePowerRank;currank++)
		{	
			if (bgForcePowerCost[c][currank] > uiForceAvailable)
			{	// Break out, we can't afford any more power.
				break;
			}
			// Pay for this rank of this power.
			uiForceUsed += bgForcePowerCost[c][currank];
			uiForceAvailable -= bgForcePowerCost[c][currank];

			uiForcePowersRank[c]++;
		}
	}

	if (uiForcePowersRank[FP_LEVITATION] < 1)
	{
		uiForcePowersRank[FP_LEVITATION]=1;
	}
	if (uiForcePowersRank[FP_SABERATTACK] < 1 && ui_freeSaber.integer)
	{
		uiForcePowersRank[FP_SABERATTACK]=1;
	}
	if (uiForcePowersRank[FP_SABERDEFEND] < 1 && ui_freeSaber.integer)
	{
		uiForcePowersRank[FP_SABERDEFEND]=1;
	}

	UpdateForceUsed();

	if (updateForceLater)
	{
		gTouchedForce = qtrue;
		UI_UpdateClientForcePowers(NULL);
	}
}

void UI_UpdateForcePowers()
{
	char *forcePowers = UI_Cvar_VariableString("forcepowers");
	char readBuf[256];
	int i = 0, i_f = 0, i_r = 0;

	uiForceSide = 0;

	if (forcePowers && forcePowers[0])
	{
		while (forcePowers[i])
		{
			i_r = 0;

			while (forcePowers[i] && forcePowers[i] != '-' && i_r < 255)
			{
				readBuf[i_r] = forcePowers[i];
				i_r++;
				i++;
			}
			readBuf[i_r] = '\0';
			if (i_r >= 255 || !forcePowers[i] || forcePowers[i] != '-')
			{
				uiForceSide = 0;
				goto validitycheck;
			}
			uiForceRank = atoi(readBuf);
			i_r = 0;

			if (uiForceRank > uiMaxRank)
			{
				uiForceRank = uiMaxRank;
			}

			i++;

			while (forcePowers[i] && forcePowers[i] != '-' && i_r < 255)
			{
				readBuf[i_r] = forcePowers[i];
				i_r++;
				i++;
			}
			readBuf[i_r] = '\0';
			if (i_r >= 255 || !forcePowers[i] || forcePowers[i] != '-')
			{
				uiForceSide = 0;
				goto validitycheck;
			}
			uiForceSide = atoi(readBuf);
			i_r = 0;

			i++;

			i_f = FP_HEAL;

			while (forcePowers[i] && i_f < NUM_FORCE_POWERS)
			{
				readBuf[0] = forcePowers[i];
				readBuf[1] = '\0';
				uiForcePowersRank[i_f] = atoi(readBuf);

				if (i_f == FP_LEVITATION &&
					uiForcePowersRank[i_f] < 1)
				{
					uiForcePowersRank[i_f] = 1;
				}

				if (i_f == FP_SABERATTACK &&
					uiForcePowersRank[i_f] < 1 &&
					ui_freeSaber.integer)
				{
					uiForcePowersRank[i_f] = 1;
				}

				if (i_f == FP_SABERDEFEND &&
					uiForcePowersRank[i_f] < 1 &&
					ui_freeSaber.integer)
				{
					uiForcePowersRank[i_f] = 1;
				}

				i_f++;
				i++;
			}

			if (i_f < NUM_FORCE_POWERS)
			{ //info for all the powers wasn't there..
				uiForceSide = 0;
				goto validitycheck;
			}
			i++;
		}
	}

validitycheck:

	if (!uiForceSide)
	{
		uiForceSide = 1;
		uiForceRank = 1;
		i = 0;
		while (i < NUM_FORCE_POWERS)
		{
			if (i == FP_LEVITATION)
			{
				uiForcePowersRank[i] = 1;
			}
			else if (i == FP_SABERATTACK && ui_freeSaber.integer)
			{
				uiForcePowersRank[i] = 1;
			}
			else if (i == FP_SABERDEFEND && ui_freeSaber.integer)
			{
				uiForcePowersRank[i] = 1;
			}
			else
			{
				uiForcePowersRank[i] = 0;
			}

			i++;
		}

		UI_UpdateClientForcePowers(NULL);
	}

	UpdateForceUsed();
}
extern int	uiSkinColor;

qboolean UI_SkinColor_HandleKey(int flags, float *special, int key, int num, int min, int max, int type) 
{
  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) 
  {
  	int i = num;

	if (key == K_MOUSE2)
	{
	    i--;
	}
	else
	{
	    i++;
	}

    if (i < min)
	{
		i = max;
	}
	else if (i > max)
	{
      i = min;
    }

    num = i;

	uiSkinColor = num;

	UI_FeederSelection(FEEDER_Q3HEADS, uiInfo.q3SelectedHead);

    return qtrue;
  }
  return qfalse;
}




qboolean UI_ForceSide_HandleKey(int flags, float *special, int key, int num, int min, int max, int type) 
{
	char info[MAX_INFO_VALUE];

	info[0] = '\0';
	trap_GetConfigString(CS_SERVERINFO, info, sizeof(info));

	if (atoi( Info_ValueForKey( info, "g_forceBasedTeams" ) ))
	{
		switch((int)(trap_Cvar_VariableValue("ui_myteam")))
		{
		case TEAM_RED:
			return qfalse;
		case TEAM_BLUE:
			return qfalse;
		default:
			break;
		}
	}

	if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) 
	{
		int i = num;
		int x = 0;

		//update the feeder item selection, it might be different depending on side
		Menu_SetFeederSelection(NULL, FEEDER_FORCECFG, 0, NULL);

		if (key == K_MOUSE2)
		{
			i--;
		}
		else
		{
			i++;
		}

		if (i < min)
		{
			i = max;
		}
		else if (i > max)
		{
			i = min;
		}

		num = i;

		uiForceSide = num;

		// Resetting power ranks based on if light or dark side is chosen
		while (x < NUM_FORCE_POWERS)
		{
			if (uiForcePowerDarkLight[x] && uiForceSide != uiForcePowerDarkLight[x])
			{
				uiForcePowersRank[x] = 0;
			}
			x++;
		}

		UpdateForceUsed();

		gTouchedForce = qtrue;
		return qtrue;
	}
	return qfalse;
}


qboolean UI_ForceMaxRank_HandleKey(int flags, float *special, int key, int num, int min, int max, int type) 
{
  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) 
  {
  	int i = num;

	if (key == K_MOUSE2)
	{
	    i--;
	}
	else
	{
	    i++;
	}

    if (i < min)
	{
		i = max;
	}
	else if (i > max)
	{
      i = min;
    }

    num = i;

	uiMaxRank = num;

	trap_Cvar_Set( "g_maxForceRank", va("%i", num));

	// The update force used will remove overallocated powers automatically.
	UpdateForceUsed();

	gTouchedForce = qtrue;

    return qtrue;
  }
  return qfalse;
}


// This function will either raise or lower a power by one rank.
qboolean UI_ForcePowerRank_HandleKey(int flags, float *special, int key, int num, int min, int max, int type) 
{
	qboolean raising;

	if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) 
	{
		int forcepower, rank;

		//this will give us the index as long as UI_FORCE_RANK is always one below the first force rank index
		forcepower = (type-UI_FORCE_RANK)-1;
				
		// If we are not on the same side as a power, or if we are not of any rank at all.
		if (uiForcePowerDarkLight[forcepower] && uiForceSide != uiForcePowerDarkLight[forcepower])
		{
			return qtrue;
		}
		else if (forcepower == FP_SABERDEFEND || forcepower == FP_SABERTHROW)
		{	// Saberdefend and saberthrow can't be bought if there is no saberattack
			if (uiForcePowersRank[FP_SABERATTACK] < 1)
			{
				return qtrue;
			}
		}

		if (type == UI_FORCE_RANK_LEVITATION)
		{
			min += 1;
		}
		if (type == UI_FORCE_RANK_SABERATTACK && ui_freeSaber.integer)
		{
			min += 1;
		}
		if (type == UI_FORCE_RANK_SABERDEFEND && ui_freeSaber.integer)
		{
			min += 1;
		}

		if (key == K_MOUSE2)
		{	// Lower a point.
			if (uiForcePowersRank[forcepower]<=min)
			{
				return qtrue;
			}
			raising = qfalse;
		}
		else
		{	// Raise a point.
			if (uiForcePowersRank[forcepower]>=max)
			{
				return qtrue;
			}
			raising = qtrue;
		}

		if (raising)
		{	// Check if we can accrue the cost of this power.
			rank = uiForcePowersRank[forcepower]+1;
			if (bgForcePowerCost[forcepower][rank] > uiForceAvailable)
			{	// We can't afford this power.  Abandon ship.
				return qtrue;
			}
			else
			{	// Sure we can afford it.
				uiForceUsed += bgForcePowerCost[forcepower][rank];
				uiForceAvailable -= bgForcePowerCost[forcepower][rank];
				uiForcePowersRank[forcepower]=rank;
			}
		}
		else
		{	// Lower the point.
			rank = uiForcePowersRank[forcepower];
			uiForceUsed -= bgForcePowerCost[forcepower][rank];
			uiForceAvailable += bgForcePowerCost[forcepower][rank];
			uiForcePowersRank[forcepower]--;
		}

		UpdateForceUsed();

		gTouchedForce = qtrue;

		return qtrue;
	}
	return qfalse;
}


int gCustRank = 0;
int gCustSide = 0;

int gCustPowersRank[NUM_FORCE_POWERS] = {
	0,//FP_HEAL = 0,//instant
	1,//FP_LEVITATION,//hold/duration, this one defaults to 1 (gives a free point)
	0,//FP_SPEED,//duration
	0,//FP_PUSH,//hold/duration
	0,//FP_PULL,//hold/duration
	0,//FP_TELEPATHY,//instant
	0,//FP_GRIP,//hold/duration
	0,//FP_LIGHTNING,//hold/duration
	0,//FP_RAGE,//duration
	0,//FP_PROTECT,
	0,//FP_ABSORB,
	0,//FP_TEAM_HEAL,
	0,//FP_TEAM_FORCE,
	0,//FP_DRAIN,
	0,//FP_SEE,
	0,//FP_SABERATTACK,
	0,//FP_SABERDEFEND,
	0//FP_SABERTHROW,
};

/*
=================
UI_ForceConfigHandle
=================
*/
void UI_ForceConfigHandle( int oldindex, int newindex )
{
	fileHandle_t f;
	int len = 0;
	int i = 0;
	int c = 0;
	int iBuf = 0, forcePowerRank, currank;
	char fcfBuffer[8192];
	char singleBuf[64];
	char info[MAX_INFO_VALUE];
	int forceTeam = 0;

	if (oldindex == 0)
	{ //switching out from custom config, so first shove the current values into the custom storage
		i = 0;

		while (i < NUM_FORCE_POWERS)
		{
			gCustPowersRank[i] = uiForcePowersRank[i];
			i++;
		}
		gCustRank = uiForceRank;
		gCustSide = uiForceSide;
	}

	if (newindex == 0)
	{ //switching back to custom, shove the values back in from the custom storage
		i = 0;
		uiForceUsed = 0;
		gTouchedForce = qtrue;

		while (i < NUM_FORCE_POWERS)
		{
			uiForcePowersRank[i] = gCustPowersRank[i];
			uiForceUsed += uiForcePowersRank[i];
			i++;
		}
		uiForceRank = gCustRank;
		uiForceSide = gCustSide;

		UpdateForceUsed();
		return;
	}

	//If we made it here, we want to load in a new config
	if (uiForceSide == FORCE_LIGHTSIDE)
	{ //we should only be displaying lightside configs, so.. look in the light folder
		newindex += uiInfo.forceConfigLightIndexBegin;
		if (newindex >= uiInfo.forceConfigCount)
		{
			return;
		}
		len = trap_FS_FOpenFile(va("forcecfg/light/%s.fcf", uiInfo.forceConfigNames[newindex]), &f, FS_READ);
	}
	else
	{ //else dark
		newindex += uiInfo.forceConfigDarkIndexBegin;
		if (newindex >= uiInfo.forceConfigCount || newindex > uiInfo.forceConfigLightIndexBegin)
		{ //dark gets read in before light
			return;
		}
		len = trap_FS_FOpenFile(va("forcecfg/dark/%s.fcf", uiInfo.forceConfigNames[newindex]), &f, FS_READ);
	}

	if (len <= 0)
	{ //This should not have happened. But, before we quit out, attempt searching the other light/dark folder for the file.
		if (uiForceSide == FORCE_LIGHTSIDE)
		{
			len = trap_FS_FOpenFile(va("forcecfg/dark/%s.fcf", uiInfo.forceConfigNames[newindex]), &f, FS_READ);
		}
		else
		{
			len = trap_FS_FOpenFile(va("forcecfg/light/%s.fcf", uiInfo.forceConfigNames[newindex]), &f, FS_READ);
		}

		if (len <= 0)
		{ //still failure? Oh well.
			return;
		}
	}

	if (len >= 8192)
	{
		return;
	}

	trap_FS_Read(fcfBuffer, len, f);
	fcfBuffer[len] = 0;
	trap_FS_FCloseFile(f);

	i = 0;

	info[0] = '\0';
	trap_GetConfigString(CS_SERVERINFO, info, sizeof(info));

	if (atoi( Info_ValueForKey( info, "g_forceBasedTeams" ) ))
	{
		switch((int)(trap_Cvar_VariableValue("ui_myteam")))
		{
		case TEAM_RED:
			forceTeam = FORCE_DARKSIDE;
			break;
		case TEAM_BLUE:
			forceTeam = FORCE_LIGHTSIDE;
			break;
		default:
			break;
		}
	}

	BG_LegalizedForcePowers(fcfBuffer, uiMaxRank, ui_freeSaber.integer, forceTeam, atoi( Info_ValueForKey( info, "g_gametype" )), 0);
	//legalize the config based on the max rank

	//now that we're done with the handle, it's time to parse our force data out of the string
	//we store strings in rank-side-xxxxxxxxx format (where the x's are individual force power levels)
	while (fcfBuffer[i] && fcfBuffer[i] != '-')
	{
		singleBuf[c] = fcfBuffer[i];
		c++;
		i++;
	}
	singleBuf[c] = 0;
	c = 0;
	i++;

	iBuf = atoi(singleBuf);

	if (iBuf > uiMaxRank || iBuf < 0)
	{ //this force config uses a rank level higher than our currently restricted level.. so we can't use it
	  //FIXME: Print a message indicating this to the user
		return;
	}

	uiForceRank = iBuf;

	while (fcfBuffer[i] && fcfBuffer[i] != '-')
	{
		singleBuf[c] = fcfBuffer[i];
		c++;
		i++;
	}
	singleBuf[c] = 0;
	c = 0;
	i++;

	uiForceSide = atoi(singleBuf);

	if (uiForceSide != FORCE_LIGHTSIDE &&
		uiForceSide != FORCE_DARKSIDE)
	{
		uiForceSide = FORCE_LIGHTSIDE;
		return;
	}

	//clear out the existing powers
	while (c < NUM_FORCE_POWERS)
	{
		/*
		if (c==FP_LEVITATION)
		{
			uiForcePowersRank[c]=1;
		}
		else if (c==FP_SABERATTACK && ui_freeSaber.integer)
		{
			uiForcePowersRank[c]=1;
		}
		else if (c==FP_SABERDEFEND && ui_freeSaber.integer)
		{
			uiForcePowersRank[c]=1;
		}
		else
		{
			uiForcePowersRank[c] = 0;
		}
		*/
		//rww - don't need to do these checks. Just trust whatever the saber config says.
		uiForcePowersRank[c] = 0;
		c++;
	}
	uiForceUsed = 0;
	uiForceAvailable = forceMasteryPoints[uiForceRank];
	gTouchedForce = qtrue;

	for (c=0;fcfBuffer[i]&&c<NUM_FORCE_POWERS;c++,i++)
	{
		singleBuf[0] = fcfBuffer[i];
		singleBuf[1] = 0;
		iBuf = atoi(singleBuf);	// So, that means that Force Power "c" wants to be set to rank "iBuf".
		
		if (iBuf < 0)
		{
			iBuf = 0;
		}

		forcePowerRank = iBuf;

		if (forcePowerRank > FORCE_LEVEL_3 || forcePowerRank < 0)
		{ //err..  not correct
			continue;  // skip this power
		}

		if (uiForcePowerDarkLight[c] && uiForcePowerDarkLight[c] != uiForceSide)
		{ //Apparently the user has crafted a force config that has powers that don't fit with the config's side.
			continue;  // skip this power
		}

		// Accrue cost for each assigned rank for this power.
		for (currank=FORCE_LEVEL_1;currank<=forcePowerRank;currank++)
		{	
			if (bgForcePowerCost[c][currank] > uiForceAvailable)
			{	// Break out, we can't afford any more power.
				break;
			}
			// Pay for this rank of this power.
			uiForceUsed += bgForcePowerCost[c][currank];
			uiForceAvailable -= bgForcePowerCost[c][currank];

			uiForcePowersRank[c]++;
		}
	}

	if (uiForcePowersRank[FP_LEVITATION] < 1)
	{
		uiForcePowersRank[FP_LEVITATION]=1;
	}
	if (uiForcePowersRank[FP_SABERATTACK] < 1 && ui_freeSaber.integer)
	{
		uiForcePowersRank[FP_SABERATTACK]=1;
	}
	if (uiForcePowersRank[FP_SABERDEFEND] < 1 && ui_freeSaber.integer)
	{
		uiForcePowersRank[FP_SABERDEFEND]=1;
	}

	UpdateForceUsed();
}
