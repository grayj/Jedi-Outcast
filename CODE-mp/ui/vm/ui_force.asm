data
export uiForceSide
align 4
LABELV uiForceSide
byte 4 1
export uiForceRank
align 4
LABELV uiForceRank
byte 4 6
export uiMaxRank
align 4
LABELV uiMaxRank
byte 4 7
export uiMaxPoints
align 4
LABELV uiMaxPoints
byte 4 20
export uiForceUsed
align 4
LABELV uiForceUsed
byte 4 0
export uiForceAvailable
align 4
LABELV uiForceAvailable
byte 4 0
export gTouchedForce
align 4
LABELV gTouchedForce
byte 4 0
export uiForcePowersRank
align 4
LABELV uiForcePowersRank
byte 4 0
byte 4 1
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 1
byte 4 1
byte 4 0
export uiForcePowerDarkLight
align 4
LABELV uiForcePowerDarkLight
byte 4 1
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 1
byte 4 2
byte 4 2
byte 4 2
byte 4 1
byte 4 1
byte 4 1
byte 4 2
byte 4 2
byte 4 0
byte 4 0
byte 4 0
byte 4 0
export UI_InitForceShaders
code
proc UI_InitForceShaders 96 4
file "../ui_force.c"
line 73
;1://
;2:/*
;3:=======================================================================
;4:
;5:FORCE INTERFACE 
;6:
;7:=======================================================================
;8:*/
;9:
;10:// use this to get a demo build without an explicit demo build, i.e. to get the demo ui files to build
;11:#include "ui_local.h"
;12:#include "../qcommon/qfiles.h"
;13:#include "ui_force.h"
;14:
;15:int uiForceSide = FORCE_LIGHTSIDE;
;16:int uiForceRank = FORCE_MASTERY_JEDI_KNIGHT;
;17:int uiMaxRank = MAX_FORCE_RANK;
;18:int uiMaxPoints = 20;
;19:int	uiForceUsed = 0;
;20:int uiForceAvailable=0;
;21:
;22:qboolean gTouchedForce = qfalse;
;23:vmCvar_t	ui_freeSaber, ui_forcePowerDisable;
;24:void Menu_ShowItemByName(menuDef_t *menu, const char *p, qboolean bShow);
;25:
;26:int uiForcePowersRank[NUM_FORCE_POWERS] = {
;27:	0,//FP_HEAL = 0,//instant
;28:	1,//FP_LEVITATION,//hold/duration, this one defaults to 1 (gives a free point)
;29:	0,//FP_SPEED,//duration
;30:	0,//FP_PUSH,//hold/duration
;31:	0,//FP_PULL,//hold/duration
;32:	0,//FP_TELEPATHY,//instant
;33:	0,//FP_GRIP,//hold/duration
;34:	0,//FP_LIGHTNING,//hold/duration
;35:	0,//FP_RAGE,//duration
;36:	0,//FP_PROTECT,
;37:	0,//FP_ABSORB,
;38:	0,//FP_TEAM_HEAL,
;39:	0,//FP_TEAM_FORCE,
;40:	0,//FP_DRAIN,
;41:	0,//FP_SEE,
;42:	1,//FP_SABERATTACK, //default to 1 point in attack
;43:	1,//FP_SABERDEFEND, //defualt to 1 point in defense
;44:	0//FP_SABERTHROW,
;45:};
;46:
;47:int uiForcePowerDarkLight[NUM_FORCE_POWERS] = //0 == neutral
;48:{ //nothing should be usable at rank 0..
;49:	FORCE_LIGHTSIDE,//FP_HEAL,//instant
;50:	0,//FP_LEVITATION,//hold/duration
;51:	0,//FP_SPEED,//duration
;52:	0,//FP_PUSH,//hold/duration
;53:	0,//FP_PULL,//hold/duration
;54:	FORCE_LIGHTSIDE,//FP_TELEPATHY,//instant
;55:	FORCE_DARKSIDE,//FP_GRIP,//hold/duration
;56:	FORCE_DARKSIDE,//FP_LIGHTNING,//hold/duration
;57:	FORCE_DARKSIDE,//FP_RAGE,//duration
;58:	FORCE_LIGHTSIDE,//FP_PROTECT,//duration
;59:	FORCE_LIGHTSIDE,//FP_ABSORB,//duration
;60:	FORCE_LIGHTSIDE,//FP_TEAM_HEAL,//instant
;61:	FORCE_DARKSIDE,//FP_TEAM_FORCE,//instant
;62:	FORCE_DARKSIDE,//FP_DRAIN,//hold/duration
;63:	0,//FP_SEE,//duration
;64:	0,//FP_SABERATTACK,
;65:	0,//FP_SABERDEFEND,
;66:	0//FP_SABERTHROW,
;67:		//NUM_FORCE_POWERS
;68:};
;69:
;70:int uiForceStarShaders[NUM_FORCE_STAR_IMAGES][2];
;71:int uiSaberColorShaders[NUM_SABER_COLORS];
;72:void UI_InitForceShaders(void)
;73:{
line 74
;74:	uiForceStarShaders[0][0] = trap_R_RegisterShaderNoMip("forcestar0");
ADDRGP4 $150
ARGP4
ADDRLP4 0
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders
ADDRLP4 0
INDIRI4
ASGNI4
line 75
;75:	uiForceStarShaders[0][1] = trap_R_RegisterShaderNoMip("forcestar0");
ADDRGP4 $150
ARGP4
ADDRLP4 4
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+4
ADDRLP4 4
INDIRI4
ASGNI4
line 76
;76:	uiForceStarShaders[1][0] = trap_R_RegisterShaderNoMip("forcecircle1");
ADDRGP4 $153
ARGP4
ADDRLP4 8
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+8
ADDRLP4 8
INDIRI4
ASGNI4
line 77
;77:	uiForceStarShaders[1][1] = trap_R_RegisterShaderNoMip("forcestar1");
ADDRGP4 $156
ARGP4
ADDRLP4 12
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+8+4
ADDRLP4 12
INDIRI4
ASGNI4
line 78
;78:	uiForceStarShaders[2][0] = trap_R_RegisterShaderNoMip("forcecircle2");
ADDRGP4 $158
ARGP4
ADDRLP4 16
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+16
ADDRLP4 16
INDIRI4
ASGNI4
line 79
;79:	uiForceStarShaders[2][1] = trap_R_RegisterShaderNoMip("forcestar2");
ADDRGP4 $161
ARGP4
ADDRLP4 20
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+16+4
ADDRLP4 20
INDIRI4
ASGNI4
line 80
;80:	uiForceStarShaders[3][0] = trap_R_RegisterShaderNoMip("forcecircle3");
ADDRGP4 $163
ARGP4
ADDRLP4 24
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+24
ADDRLP4 24
INDIRI4
ASGNI4
line 81
;81:	uiForceStarShaders[3][1] = trap_R_RegisterShaderNoMip("forcestar3");
ADDRGP4 $166
ARGP4
ADDRLP4 28
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+24+4
ADDRLP4 28
INDIRI4
ASGNI4
line 82
;82:	uiForceStarShaders[4][0] = trap_R_RegisterShaderNoMip("forcecircle4");
ADDRGP4 $168
ARGP4
ADDRLP4 32
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+32
ADDRLP4 32
INDIRI4
ASGNI4
line 83
;83:	uiForceStarShaders[4][1] = trap_R_RegisterShaderNoMip("forcestar4");
ADDRGP4 $171
ARGP4
ADDRLP4 36
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+32+4
ADDRLP4 36
INDIRI4
ASGNI4
line 84
;84:	uiForceStarShaders[5][0] = trap_R_RegisterShaderNoMip("forcecircle5");
ADDRGP4 $173
ARGP4
ADDRLP4 40
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+40
ADDRLP4 40
INDIRI4
ASGNI4
line 85
;85:	uiForceStarShaders[5][1] = trap_R_RegisterShaderNoMip("forcestar5");
ADDRGP4 $176
ARGP4
ADDRLP4 44
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+40+4
ADDRLP4 44
INDIRI4
ASGNI4
line 86
;86:	uiForceStarShaders[6][0] = trap_R_RegisterShaderNoMip("forcecircle6");
ADDRGP4 $178
ARGP4
ADDRLP4 48
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+48
ADDRLP4 48
INDIRI4
ASGNI4
line 87
;87:	uiForceStarShaders[6][1] = trap_R_RegisterShaderNoMip("forcestar6");
ADDRGP4 $181
ARGP4
ADDRLP4 52
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+48+4
ADDRLP4 52
INDIRI4
ASGNI4
line 88
;88:	uiForceStarShaders[7][0] = trap_R_RegisterShaderNoMip("forcecircle7");
ADDRGP4 $183
ARGP4
ADDRLP4 56
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+56
ADDRLP4 56
INDIRI4
ASGNI4
line 89
;89:	uiForceStarShaders[7][1] = trap_R_RegisterShaderNoMip("forcestar7");
ADDRGP4 $186
ARGP4
ADDRLP4 60
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+56+4
ADDRLP4 60
INDIRI4
ASGNI4
line 90
;90:	uiForceStarShaders[8][0] = trap_R_RegisterShaderNoMip("forcecircle8");
ADDRGP4 $188
ARGP4
ADDRLP4 64
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+64
ADDRLP4 64
INDIRI4
ASGNI4
line 91
;91:	uiForceStarShaders[8][1] = trap_R_RegisterShaderNoMip("forcestar8");
ADDRGP4 $191
ARGP4
ADDRLP4 68
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiForceStarShaders+64+4
ADDRLP4 68
INDIRI4
ASGNI4
line 93
;92:
;93:	uiSaberColorShaders[SABER_RED]		= trap_R_RegisterShaderNoMip("menu/art/saber_red");
ADDRGP4 $192
ARGP4
ADDRLP4 72
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiSaberColorShaders
ADDRLP4 72
INDIRI4
ASGNI4
line 94
;94:	uiSaberColorShaders[SABER_ORANGE]	= trap_R_RegisterShaderNoMip("menu/art/saber_orange");
ADDRGP4 $194
ARGP4
ADDRLP4 76
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiSaberColorShaders+4
ADDRLP4 76
INDIRI4
ASGNI4
line 95
;95:	uiSaberColorShaders[SABER_YELLOW]	= trap_R_RegisterShaderNoMip("menu/art/saber_yellow");
ADDRGP4 $196
ARGP4
ADDRLP4 80
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiSaberColorShaders+8
ADDRLP4 80
INDIRI4
ASGNI4
line 96
;96:	uiSaberColorShaders[SABER_GREEN]	= trap_R_RegisterShaderNoMip("menu/art/saber_green");
ADDRGP4 $198
ARGP4
ADDRLP4 84
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiSaberColorShaders+12
ADDRLP4 84
INDIRI4
ASGNI4
line 97
;97:	uiSaberColorShaders[SABER_BLUE]		= trap_R_RegisterShaderNoMip("menu/art/saber_blue");
ADDRGP4 $200
ARGP4
ADDRLP4 88
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiSaberColorShaders+16
ADDRLP4 88
INDIRI4
ASGNI4
line 98
;98:	uiSaberColorShaders[SABER_PURPLE]	= trap_R_RegisterShaderNoMip("menu/art/saber_purple");
ADDRGP4 $202
ARGP4
ADDRLP4 92
ADDRGP4 trap_R_RegisterShaderNoMip
CALLI4
ASGNI4
ADDRGP4 uiSaberColorShaders+20
ADDRLP4 92
INDIRI4
ASGNI4
line 99
;99:}
LABELV $149
endproc UI_InitForceShaders 96 4
export UI_DrawForceStars
proc UI_DrawForceStars 28 20
line 103
;100:
;101:// Draw the stars spent on the current force power
;102:void UI_DrawForceStars(rectDef_t *rect, float scale, vec4_t color, int textStyle, int forceindex, int val, int min, int max) 
;103:{
line 104
;104:	int	i,pad = 4;
ADDRLP4 16
CNSTI4 4
ASGNI4
line 105
;105:	int	xPos,width = 16;
ADDRLP4 4
CNSTI4 16
ASGNI4
line 108
;106:	int starcolor;
;107:
;108:	if (val < min || val > max) 
ADDRLP4 20
ADDRFP4 20
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
ADDRFP4 24
INDIRI4
LTI4 $206
ADDRLP4 20
INDIRI4
ADDRFP4 28
INDIRI4
LEI4 $204
LABELV $206
line 109
;109:	{
line 110
;110:		val = min;
ADDRFP4 20
ADDRFP4 24
INDIRI4
ASGNI4
line 111
;111:	}
LABELV $204
line 113
;112:
;113:	if (1)	// if (val)
line 114
;114:	{
line 115
;115:		xPos = rect->x;
ADDRLP4 8
ADDRFP4 0
INDIRP4
INDIRF4
CVFI4 4
ASGNI4
line 117
;116:
;117:		for (i=FORCE_LEVEL_1;i<=max;i++)
ADDRLP4 0
CNSTI4 1
ASGNI4
ADDRGP4 $212
JUMPV
LABELV $209
line 118
;118:		{
line 119
;119:			starcolor = bgForcePowerCost[forceindex][i];
ADDRLP4 12
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 16
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
ASGNI4
line 121
;120:
;121:			if (val >= i)
ADDRFP4 20
INDIRI4
ADDRLP4 0
INDIRI4
LTI4 $213
line 122
;122:			{	// Draw a star.
line 123
;123:				UI_DrawHandlePic( xPos, rect->y+6, width, width, uiForceStarShaders[starcolor][1] );
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 1086324736
ADDF4
ARGF4
ADDRLP4 24
ADDRLP4 4
INDIRI4
CVIF4 4
ASGNF4
ADDRLP4 24
INDIRF4
ARGF4
ADDRLP4 24
INDIRF4
ARGF4
ADDRLP4 12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiForceStarShaders+4
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 124
;124:			}
ADDRGP4 $214
JUMPV
LABELV $213
line 126
;125:			else
;126:			{	// Draw a circle.
line 127
;127:				UI_DrawHandlePic( xPos, rect->y+6, width, width, uiForceStarShaders[starcolor][0] );
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 1086324736
ADDF4
ARGF4
ADDRLP4 24
ADDRLP4 4
INDIRI4
CVIF4 4
ASGNF4
ADDRLP4 24
INDIRF4
ARGF4
ADDRLP4 24
INDIRF4
ARGF4
ADDRLP4 12
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 uiForceStarShaders
ADDP4
INDIRI4
ARGI4
ADDRGP4 UI_DrawHandlePic
CALLV
pop
line 128
;128:			}
LABELV $214
line 129
;129:			xPos += width + pad;
ADDRLP4 8
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDRLP4 16
INDIRI4
ADDI4
ADDI4
ASGNI4
line 130
;130:		}
LABELV $210
line 117
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $212
ADDRLP4 0
INDIRI4
ADDRFP4 28
INDIRI4
LEI4 $209
line 131
;131:	}
LABELV $207
line 132
;132:}
LABELV $203
endproc UI_DrawForceStars 28 20
export UI_UpdateClientForcePowers
proc UI_UpdateClientForcePowers 12 84
line 136
;133:
;134:// Set the client's force power layout.
;135:void UI_UpdateClientForcePowers(const char *teamArg)
;136:{
line 137
;137:	trap_Cvar_Set( "forcepowers", va("%i-%i-%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i%i",
ADDRGP4 $218
ARGP4
ADDRGP4 uiForceRank
INDIRI4
ARGI4
ADDRGP4 uiForceSide
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+4
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+8
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+12
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+16
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+20
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+24
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+28
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+32
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+36
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+40
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+44
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+48
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+52
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+56
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+60
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+64
INDIRI4
ARGI4
ADDRGP4 uiForcePowersRank+68
INDIRI4
ARGI4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $217
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 146
;138:		uiForceRank, uiForceSide, uiForcePowersRank[0], uiForcePowersRank[1],
;139:		uiForcePowersRank[2], uiForcePowersRank[3], uiForcePowersRank[4],
;140:		uiForcePowersRank[5], uiForcePowersRank[6], uiForcePowersRank[7],
;141:		uiForcePowersRank[8], uiForcePowersRank[9], uiForcePowersRank[10],
;142:		uiForcePowersRank[11], uiForcePowersRank[12], uiForcePowersRank[13],
;143:		uiForcePowersRank[14], uiForcePowersRank[15], uiForcePowersRank[16],
;144:		uiForcePowersRank[17]) );
;145:
;146:	if (gTouchedForce)
ADDRGP4 gTouchedForce
INDIRI4
CNSTI4 0
EQI4 $236
line 147
;147:	{
line 148
;148:		if (teamArg && teamArg[0])
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $238
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $238
line 149
;149:		{
line 150
;150:			trap_Cmd_ExecuteText( EXEC_APPEND, va("forcechanged \"%s\"\n", teamArg) );
ADDRGP4 $240
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 151
;151:		}
ADDRGP4 $239
JUMPV
LABELV $238
line 153
;152:		else
;153:		{
line 154
;154:			trap_Cmd_ExecuteText( EXEC_APPEND, "forcechanged\n" );
CNSTI4 2
ARGI4
ADDRGP4 $241
ARGP4
ADDRGP4 trap_Cmd_ExecuteText
CALLV
pop
line 155
;155:		}
LABELV $239
line 156
;156:	}
LABELV $236
line 158
;157:
;158:	gTouchedForce = qfalse;
ADDRGP4 gTouchedForce
CNSTI4 0
ASGNI4
line 159
;159:}
LABELV $216
endproc UI_UpdateClientForcePowers 12 84
export UI_TranslateFCFIndex
proc UI_TranslateFCFIndex 0 0
line 162
;160:
;161:int UI_TranslateFCFIndex(int index)
;162:{
line 163
;163:	if (uiForceSide == FORCE_LIGHTSIDE)
ADDRGP4 uiForceSide
INDIRI4
CNSTI4 1
NEI4 $243
line 164
;164:	{
line 165
;165:		return index-uiInfo.forceConfigLightIndexBegin;
ADDRFP4 0
INDIRI4
ADDRGP4 uiInfo+95056
INDIRI4
SUBI4
RETI4
ADDRGP4 $242
JUMPV
LABELV $243
line 168
;166:	}
;167:
;168:	return index-uiInfo.forceConfigDarkIndexBegin;
ADDRFP4 0
INDIRI4
ADDRGP4 uiInfo+95052
INDIRI4
SUBI4
RETI4
LABELV $242
endproc UI_TranslateFCFIndex 0 0
export UI_SaveForceTemplate
proc UI_SaveForceTemplate 568 20
line 172
;169:}
;170:
;171:void UI_SaveForceTemplate()
;172:{
line 173
;173:	char *selectedName = UI_Cvar_VariableString("ui_SaveFCF");
ADDRGP4 $248
ARGP4
ADDRLP4 540
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 528
ADDRLP4 540
INDIRP4
ASGNP4
line 177
;174:	char fcfString[512];
;175:	char forceStringValue[4];
;176:	fileHandle_t f;
;177:	int strPlace = 0;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 178
;178:	int forcePlace = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 179
;179:	int i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 180
;180:	qboolean foundFeederItem = qfalse;
ADDRLP4 532
CNSTI4 0
ASGNI4
line 182
;181:
;182:	if (!selectedName || !selectedName[0])
ADDRLP4 528
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $251
ADDRLP4 528
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $249
LABELV $251
line 183
;183:	{
line 184
;184:		Com_Printf("You did not provide a name for the template.\n");
ADDRGP4 $252
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 185
;185:		return;
ADDRGP4 $247
JUMPV
LABELV $249
line 188
;186:	}
;187:
;188:	if (uiForceSide == FORCE_LIGHTSIDE)
ADDRGP4 uiForceSide
INDIRI4
CNSTI4 1
NEI4 $253
line 189
;189:	{ //write it into the light side folder
line 190
;190:		trap_FS_FOpenFile(va("forcecfg/light/%s.fcf", selectedName), &f, FS_WRITE);
ADDRGP4 $255
ARGP4
ADDRLP4 528
INDIRP4
ARGP4
ADDRLP4 548
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 548
INDIRP4
ARGP4
ADDRLP4 536
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 trap_FS_FOpenFile
CALLI4
pop
line 191
;191:	}
ADDRGP4 $254
JUMPV
LABELV $253
line 193
;192:	else
;193:	{ //if it isn't light it must be dark
line 194
;194:		trap_FS_FOpenFile(va("forcecfg/dark/%s.fcf", selectedName), &f, FS_WRITE);
ADDRGP4 $256
ARGP4
ADDRLP4 528
INDIRP4
ARGP4
ADDRLP4 548
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 548
INDIRP4
ARGP4
ADDRLP4 536
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 trap_FS_FOpenFile
CALLI4
pop
line 195
;195:	}
LABELV $254
line 197
;196:
;197:	if (!f)
ADDRLP4 536
INDIRI4
CNSTI4 0
NEI4 $257
line 198
;198:	{
line 199
;199:		Com_Printf("There was an error writing the template file (read-only?).\n");
ADDRGP4 $259
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 200
;200:		return;
ADDRGP4 $247
JUMPV
LABELV $257
line 203
;201:	}
;202:
;203:	Com_sprintf(fcfString, sizeof(fcfString), "%i-%i-", uiForceRank, uiForceSide);
ADDRLP4 16
ARGP4
CNSTI4 512
ARGI4
ADDRGP4 $260
ARGP4
ADDRGP4 uiForceRank
INDIRI4
ARGI4
ADDRGP4 uiForceSide
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 204
;204:	strPlace = strlen(fcfString);
ADDRLP4 16
ARGP4
ADDRLP4 548
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 548
INDIRI4
ASGNI4
ADDRGP4 $262
JUMPV
LABELV $261
line 207
;205:
;206:	while (forcePlace < NUM_FORCE_POWERS)
;207:	{
line 208
;208:		Com_sprintf(forceStringValue, sizeof(forceStringValue), "%i", uiForcePowersRank[forcePlace]);
ADDRLP4 8
ARGP4
CNSTI4 4
ARGI4
ADDRGP4 $264
ARGP4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 210
;209:		//Just use the force digit even if multiple digits. Shouldn't be longer than 1.
;210:		fcfString[strPlace] = forceStringValue[0];
ADDRLP4 12
INDIRI4
ADDRLP4 16
ADDP4
ADDRLP4 8
INDIRI1
ASGNI1
line 211
;211:		strPlace++;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 212
;212:		forcePlace++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 213
;213:	}
LABELV $262
line 206
ADDRLP4 4
INDIRI4
CNSTI4 18
LTI4 $261
line 214
;214:	fcfString[strPlace] = '\n';
ADDRLP4 12
INDIRI4
ADDRLP4 16
ADDP4
CNSTI1 10
ASGNI1
line 215
;215:	fcfString[strPlace+1] = 0;
ADDRLP4 12
INDIRI4
ADDRLP4 16+1
ADDP4
CNSTI1 0
ASGNI1
line 217
;216:
;217:	trap_FS_Write(fcfString, strlen(fcfString), f);
ADDRLP4 16
ARGP4
ADDRLP4 552
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 16
ARGP4
ADDRLP4 552
INDIRI4
ARGI4
ADDRLP4 536
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 218
;218:	trap_FS_FCloseFile(f);
ADDRLP4 536
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 220
;219:
;220:	Com_Printf("Template saved as \"%s\".\n", selectedName);
ADDRGP4 $266
ARGP4
ADDRLP4 528
INDIRP4
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 223
;221:
;222:	//Now, update the FCF list
;223:	UI_LoadForceConfig_List();
ADDRGP4 UI_LoadForceConfig_List
CALLV
pop
ADDRGP4 $268
JUMPV
LABELV $267
line 227
;224:
;225:	//Then, scroll through and select the template for the file we just saved
;226:	while (i < uiInfo.forceConfigCount)
;227:	{
line 228
;228:		if (!Q_stricmp(uiInfo.forceConfigNames[i], selectedName))
ADDRLP4 0
INDIRI4
CNSTI4 7
LSHI4
ADDRGP4 uiInfo+78156
ADDP4
ARGP4
ADDRLP4 528
INDIRP4
ARGP4
ADDRLP4 556
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 556
INDIRI4
CNSTI4 0
NEI4 $271
line 229
;229:		{
line 230
;230:			if ((uiForceSide == FORCE_LIGHTSIDE && uiInfo.forceConfigSide[i]) ||
ADDRGP4 uiForceSide
INDIRI4
CNSTI4 1
NEI4 $279
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiInfo+94540
ADDP4
INDIRI4
CNSTI4 0
NEI4 $278
LABELV $279
ADDRLP4 560
CNSTI4 2
ASGNI4
ADDRGP4 uiForceSide
INDIRI4
ADDRLP4 560
INDIRI4
NEI4 $274
ADDRLP4 0
INDIRI4
ADDRLP4 560
INDIRI4
LSHI4
ADDRGP4 uiInfo+94540
ADDP4
INDIRI4
CNSTI4 0
NEI4 $274
LABELV $278
line 232
;231:				(uiForceSide == FORCE_DARKSIDE && !uiInfo.forceConfigSide[i]))
;232:			{
line 233
;233:				Menu_SetFeederSelection(NULL, FEEDER_FORCECFG, UI_TranslateFCFIndex(i), NULL);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 564
ADDRGP4 UI_TranslateFCFIndex
CALLI4
ASGNI4
CNSTP4 0
ARGP4
CNSTI4 16
ARGI4
ADDRLP4 564
INDIRI4
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 Menu_SetFeederSelection
CALLV
pop
line 234
;234:				foundFeederItem = qtrue;
ADDRLP4 532
CNSTI4 1
ASGNI4
line 235
;235:			}
LABELV $274
line 236
;236:		}
LABELV $271
line 238
;237:
;238:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 239
;239:	}
LABELV $268
line 226
ADDRLP4 0
INDIRI4
ADDRGP4 uiInfo+78148
INDIRI4
LTI4 $267
line 242
;240:
;241:	//Else, go back to 0
;242:	if (!foundFeederItem)
ADDRLP4 532
INDIRI4
CNSTI4 0
NEI4 $280
line 243
;243:	{
line 244
;244:		Menu_SetFeederSelection(NULL, FEEDER_FORCECFG, 0, NULL);
CNSTP4 0
ARGP4
CNSTI4 16
ARGI4
CNSTI4 0
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 Menu_SetFeederSelection
CALLV
pop
line 245
;245:	}
LABELV $280
line 246
;246:}
LABELV $247
endproc UI_SaveForceTemplate 568 20
export UpdateForceUsed
proc UpdateForceUsed 28 12
line 251
;247:
;248:// 
;249:
;250:void UpdateForceUsed()
;251:{
line 256
;252:	int curpower, currank;
;253:	menuDef_t *menu;
;254:
;255:	// Currently we don't make a distinction between those that wish to play Jedi of lower than maximum skill.
;256:	uiForceRank = uiMaxRank;
ADDRGP4 uiForceRank
ADDRGP4 uiMaxRank
INDIRI4
ASGNI4
line 258
;257:
;258:	uiForceUsed = 0;
ADDRGP4 uiForceUsed
CNSTI4 0
ASGNI4
line 259
;259:	uiForceAvailable = forceMasteryPoints[uiForceRank];
ADDRGP4 uiForceAvailable
ADDRGP4 uiForceRank
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 forceMasteryPoints
ADDP4
INDIRI4
ASGNI4
line 262
;260:
;261:	// Make sure that we have one freebie in jump.
;262:	if (uiForcePowersRank[FP_LEVITATION]<1)
ADDRGP4 uiForcePowersRank+4
INDIRI4
CNSTI4 1
GEI4 $283
line 263
;263:	{
line 264
;264:		uiForcePowersRank[FP_LEVITATION]=1;
ADDRGP4 uiForcePowersRank+4
CNSTI4 1
ASGNI4
line 265
;265:	}
LABELV $283
line 267
;266:	
;267:	menu = Menus_FindByName("ingame_playerforce");
ADDRGP4 $287
ARGP4
ADDRLP4 12
ADDRGP4 Menus_FindByName
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 12
INDIRP4
ASGNP4
line 269
;268:	// Set the cost of the saberattack according to whether its free.
;269:	if (ui_freeSaber.integer)
ADDRGP4 ui_freeSaber+12
INDIRI4
CNSTI4 0
EQI4 $288
line 270
;270:	{	// Make saber free
line 271
;271:		bgForcePowerCost[FP_SABERATTACK][FORCE_LEVEL_1] = 0;
ADDRGP4 bgForcePowerCost+240+4
CNSTI4 0
ASGNI4
line 272
;272:		bgForcePowerCost[FP_SABERDEFEND][FORCE_LEVEL_1] = 0;
ADDRGP4 bgForcePowerCost+256+4
CNSTI4 0
ASGNI4
line 274
;273:		// Make sure that we have one freebie in saber if applicable.
;274:		if (uiForcePowersRank[FP_SABERATTACK]<1)
ADDRGP4 uiForcePowersRank+60
INDIRI4
CNSTI4 1
GEI4 $295
line 275
;275:		{
line 276
;276:			uiForcePowersRank[FP_SABERATTACK]=1;
ADDRGP4 uiForcePowersRank+60
CNSTI4 1
ASGNI4
line 277
;277:		}
LABELV $295
line 278
;278:		if (uiForcePowersRank[FP_SABERDEFEND]<1)
ADDRGP4 uiForcePowersRank+64
INDIRI4
CNSTI4 1
GEI4 $299
line 279
;279:		{
line 280
;280:			uiForcePowersRank[FP_SABERDEFEND]=1;
ADDRGP4 uiForcePowersRank+64
CNSTI4 1
ASGNI4
line 281
;281:		}
LABELV $299
line 282
;282:		if (menu)
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $289
line 283
;283:		{
line 284
;284:			Menu_ShowItemByName(menu, "setfp_saberdefend", qtrue);
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $305
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 285
;285:			Menu_ShowItemByName(menu, "setfp_saberthrow", qtrue);
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $306
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 286
;286:			Menu_ShowItemByName(menu, "effectentry", qtrue);
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $307
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 287
;287:			Menu_ShowItemByName(menu, "effectfield", qtrue);
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $308
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 288
;288:			Menu_ShowItemByName(menu, "nosaber", qfalse);
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $309
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 289
;289:		}
line 290
;290:	}
ADDRGP4 $289
JUMPV
LABELV $288
line 292
;291:	else
;292:	{	// Make saber normal cost
line 293
;293:		bgForcePowerCost[FP_SABERATTACK][FORCE_LEVEL_1] = 1;
ADDRGP4 bgForcePowerCost+240+4
CNSTI4 1
ASGNI4
line 294
;294:		bgForcePowerCost[FP_SABERDEFEND][FORCE_LEVEL_1] = 1;
ADDRGP4 bgForcePowerCost+256+4
CNSTI4 1
ASGNI4
line 296
;295:		// Also, check if there is no saberattack.  If there isn't, there had better not be any defense or throw!
;296:		if (uiForcePowersRank[FP_SABERATTACK]<1)
ADDRGP4 uiForcePowersRank+60
INDIRI4
CNSTI4 1
GEI4 $314
line 297
;297:		{
line 298
;298:			uiForcePowersRank[FP_SABERDEFEND]=0;
ADDRGP4 uiForcePowersRank+64
CNSTI4 0
ASGNI4
line 299
;299:			uiForcePowersRank[FP_SABERTHROW]=0;
ADDRGP4 uiForcePowersRank+68
CNSTI4 0
ASGNI4
line 300
;300:			if (menu)
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $315
line 301
;301:			{
line 302
;302:				Menu_ShowItemByName(menu, "setfp_saberdefend", qfalse);
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $305
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 303
;303:				Menu_ShowItemByName(menu, "setfp_saberthrow", qfalse);
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $306
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 304
;304:				Menu_ShowItemByName(menu, "effectentry", qfalse);
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $307
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 305
;305:				Menu_ShowItemByName(menu, "effectfield", qfalse);
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $308
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 306
;306:				Menu_ShowItemByName(menu, "nosaber", qtrue);
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $309
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 307
;307:			}
line 308
;308:		}
ADDRGP4 $315
JUMPV
LABELV $314
line 310
;309:		else
;310:		{
line 311
;311:			if (menu)
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $321
line 312
;312:			{
line 313
;313:				Menu_ShowItemByName(menu, "setfp_saberdefend", qtrue);
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $305
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 314
;314:				Menu_ShowItemByName(menu, "setfp_saberthrow", qtrue);
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $306
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 315
;315:				Menu_ShowItemByName(menu, "effectentry", qtrue);
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $307
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 316
;316:				Menu_ShowItemByName(menu, "effectfield", qtrue);
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $308
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 317
;317:				Menu_ShowItemByName(menu, "nosaber", qfalse);
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $309
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 Menu_ShowItemByName
CALLV
pop
line 318
;318:			}
LABELV $321
line 319
;319:		}
LABELV $315
line 320
;320:	}
LABELV $289
line 323
;321:
;322:	// Make sure that we're still legal.
;323:	for (curpower=0;curpower<NUM_FORCE_POWERS;curpower++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $323
line 324
;324:	{	// Make sure that our ranks are within legal limits.
line 325
;325:		if (uiForcePowersRank[curpower]<0)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
INDIRI4
CNSTI4 0
GEI4 $327
line 326
;326:			uiForcePowersRank[curpower]=0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
CNSTI4 0
ASGNI4
ADDRGP4 $328
JUMPV
LABELV $327
line 327
;327:		else if (uiForcePowersRank[curpower]>=NUM_FORCE_POWER_LEVELS)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
INDIRI4
CNSTI4 4
LTI4 $329
line 328
;328:			uiForcePowersRank[curpower]=(NUM_FORCE_POWER_LEVELS-1);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
CNSTI4 3
ASGNI4
LABELV $329
LABELV $328
line 330
;329:
;330:		for (currank=FORCE_LEVEL_1;currank<=uiForcePowersRank[curpower];currank++)
ADDRLP4 4
CNSTI4 1
ASGNI4
ADDRGP4 $334
JUMPV
LABELV $331
line 331
;331:		{	// Check on this force power
line 332
;332:			if (uiForcePowersRank[curpower]>0)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
INDIRI4
CNSTI4 0
LEI4 $335
line 333
;333:			{	// Do not charge the player for the one freebie in jump, or if there is one in saber.
line 334
;334:				if  (	(curpower == FP_LEVITATION && currank == FORCE_LEVEL_1) ||
ADDRLP4 16
CNSTI4 1
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 16
INDIRI4
NEI4 $342
ADDRLP4 4
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $343
LABELV $342
ADDRLP4 0
INDIRI4
CNSTI4 15
NEI4 $345
ADDRLP4 4
INDIRI4
CNSTI4 1
NEI4 $345
ADDRGP4 ui_freeSaber+12
INDIRI4
CNSTI4 0
NEI4 $343
LABELV $345
ADDRLP4 0
INDIRI4
CNSTI4 16
NEI4 $337
ADDRLP4 4
INDIRI4
CNSTI4 1
NEI4 $337
ADDRGP4 ui_freeSaber+12
INDIRI4
CNSTI4 0
EQI4 $337
LABELV $343
line 337
;335:						(curpower == FP_SABERATTACK && currank == FORCE_LEVEL_1 && ui_freeSaber.integer) ||
;336:						(curpower == FP_SABERDEFEND && currank == FORCE_LEVEL_1 && ui_freeSaber.integer) )
;337:				{
line 339
;338:					// Do nothing (written this way for clarity)
;339:				}
ADDRGP4 $338
JUMPV
LABELV $337
line 341
;340:				else
;341:				{	// Check if we can accrue the cost of this power.
line 342
;342:					if (bgForcePowerCost[curpower][currank] > uiForceAvailable)
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
ADDRGP4 uiForceAvailable
INDIRI4
LEI4 $346
line 343
;343:					{	// We can't afford this power.  Break to the next one.
line 345
;344:						// Remove this power from the player's roster.
;345:						uiForcePowersRank[curpower] = currank-1;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
ADDRLP4 4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 346
;346:						break;
ADDRGP4 $333
JUMPV
LABELV $346
line 349
;347:					}
;348:					else
;349:					{	// Sure we can afford it.
line 350
;350:						uiForceUsed += bgForcePowerCost[curpower][currank];
ADDRLP4 20
ADDRGP4 uiForceUsed
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 351
;351:						uiForceAvailable -= bgForcePowerCost[curpower][currank];
ADDRLP4 24
ADDRGP4 uiForceAvailable
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
SUBI4
ASGNI4
line 352
;352:					}
line 353
;353:				}
LABELV $338
line 354
;354:			}
LABELV $335
line 355
;355:		}
LABELV $332
line 330
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $334
ADDRLP4 4
INDIRI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
INDIRI4
LEI4 $331
LABELV $333
line 356
;356:	}
LABELV $324
line 323
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 18
LTI4 $323
line 358
;357:
;358:}
LABELV $282
endproc UpdateForceUsed 28 12
export UI_ReadLegalForce
proc UI_ReadLegalForce 1712 24
line 364
;359:
;360:
;361://Mostly parts of other functions merged into one another.
;362://Puts the current UI stuff into a string, legalizes it, and then reads it back out.
;363:void UI_ReadLegalForce(void)
;364:{
line 367
;365:	char fcfString[512];
;366:	char forceStringValue[4];
;367:	int strPlace = 0;
ADDRLP4 604
CNSTI4 0
ASGNI4
line 368
;368:	int forcePlace = 0;
ADDRLP4 596
CNSTI4 0
ASGNI4
line 369
;369:	int i = 0;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 372
;370:	char singleBuf[64];
;371:	char info[MAX_INFO_VALUE];
;372:	int c = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 373
;373:	int iBuf = 0;
ADDRLP4 592
CNSTI4 0
ASGNI4
line 374
;374:	int forcePowerRank = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 375
;375:	int currank = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 376
;376:	int forceTeam = 0;
ADDRLP4 1636
CNSTI4 0
ASGNI4
line 377
;377:	qboolean updateForceLater = qfalse;
ADDRLP4 1632
CNSTI4 0
ASGNI4
line 380
;378:
;379:	//First, stick them into a string.
;380:	Com_sprintf(fcfString, sizeof(fcfString), "%i-%i-", uiForceRank, uiForceSide);
ADDRLP4 16
ARGP4
CNSTI4 512
ARGI4
ADDRGP4 $260
ARGP4
ADDRGP4 uiForceRank
INDIRI4
ARGI4
ADDRGP4 uiForceSide
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 381
;381:	strPlace = strlen(fcfString);
ADDRLP4 16
ARGP4
ADDRLP4 1640
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 604
ADDRLP4 1640
INDIRI4
ASGNI4
ADDRGP4 $350
JUMPV
LABELV $349
line 384
;382:
;383:	while (forcePlace < NUM_FORCE_POWERS)
;384:	{
line 385
;385:		Com_sprintf(forceStringValue, sizeof(forceStringValue), "%i", uiForcePowersRank[forcePlace]);
ADDRLP4 600
ARGP4
CNSTI4 4
ARGI4
ADDRGP4 $264
ARGP4
ADDRLP4 596
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 387
;386:		//Just use the force digit even if multiple digits. Shouldn't be longer than 1.
;387:		fcfString[strPlace] = forceStringValue[0];
ADDRLP4 604
INDIRI4
ADDRLP4 16
ADDP4
ADDRLP4 600
INDIRI1
ASGNI1
line 388
;388:		strPlace++;
ADDRLP4 604
ADDRLP4 604
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 389
;389:		forcePlace++;
ADDRLP4 596
ADDRLP4 596
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 390
;390:	}
LABELV $350
line 383
ADDRLP4 596
INDIRI4
CNSTI4 18
LTI4 $349
line 391
;391:	fcfString[strPlace] = '\n';
ADDRLP4 604
INDIRI4
ADDRLP4 16
ADDP4
CNSTI1 10
ASGNI1
line 392
;392:	fcfString[strPlace+1] = 0;
ADDRLP4 604
INDIRI4
ADDRLP4 16+1
ADDP4
CNSTI1 0
ASGNI1
line 394
;393:
;394:	info[0] = '\0';
ADDRLP4 608
CNSTI1 0
ASGNI1
line 395
;395:	trap_GetConfigString(CS_SERVERINFO, info, sizeof(info));
CNSTI4 0
ARGI4
ADDRLP4 608
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigString
CALLI4
pop
line 397
;396:
;397:	if (atoi( Info_ValueForKey( info, "g_forceBasedTeams" ) ))
ADDRLP4 608
ARGP4
ADDRGP4 $355
ARGP4
ADDRLP4 1644
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1644
INDIRP4
ARGP4
ADDRLP4 1648
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1648
INDIRI4
CNSTI4 0
EQI4 $353
line 398
;398:	{
line 399
;399:		switch((int)(trap_Cvar_VariableValue("ui_myteam")))
ADDRGP4 $358
ARGP4
ADDRLP4 1656
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1652
ADDRLP4 1656
INDIRF4
CVFI4 4
ASGNI4
ADDRLP4 1652
INDIRI4
CNSTI4 1
EQI4 $360
ADDRLP4 1652
INDIRI4
CNSTI4 2
EQI4 $361
ADDRGP4 $357
JUMPV
line 400
;400:		{
LABELV $360
line 402
;401:		case TEAM_RED:
;402:			forceTeam = FORCE_DARKSIDE;
ADDRLP4 1636
CNSTI4 2
ASGNI4
line 403
;403:			break;
ADDRGP4 $357
JUMPV
LABELV $361
line 405
;404:		case TEAM_BLUE:
;405:			forceTeam = FORCE_LIGHTSIDE;
ADDRLP4 1636
CNSTI4 1
ASGNI4
line 406
;406:			break;
line 408
;407:		default:
;408:			break;
LABELV $357
line 410
;409:		}
;410:	}
LABELV $353
line 412
;411:	//Second, legalize them.
;412:	if (!BG_LegalizedForcePowers(fcfString, uiMaxRank, ui_freeSaber.integer, forceTeam, atoi( Info_ValueForKey( info, "g_gametype" )), 0))
ADDRLP4 608
ARGP4
ADDRGP4 $365
ARGP4
ADDRLP4 1652
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1652
INDIRP4
ARGP4
ADDRLP4 1656
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 16
ARGP4
ADDRGP4 uiMaxRank
INDIRI4
ARGI4
ADDRGP4 ui_freeSaber+12
INDIRI4
ARGI4
ADDRLP4 1636
INDIRI4
ARGI4
ADDRLP4 1656
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 1660
ADDRGP4 BG_LegalizedForcePowers
CALLI4
ASGNI4
ADDRLP4 1660
INDIRI4
CNSTI4 0
NEI4 $362
line 413
;413:	{ //if they were illegal, we should refresh them.
line 414
;414:		updateForceLater = qtrue;
ADDRLP4 1632
CNSTI4 1
ASGNI4
line 415
;415:	}
LABELV $362
line 418
;416:
;417:	//Lastly, put them back into the UI storage from the legalized string
;418:	i = 0;
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRGP4 $367
JUMPV
LABELV $366
line 421
;419:
;420:	while (fcfString[i] && fcfString[i] != '-')
;421:	{
line 422
;422:		singleBuf[c] = fcfString[i];
ADDRLP4 0
INDIRI4
ADDRLP4 528
ADDP4
ADDRLP4 12
INDIRI4
ADDRLP4 16
ADDP4
INDIRI1
ASGNI1
line 423
;423:		c++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 424
;424:		i++;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 425
;425:	}
LABELV $367
line 420
ADDRLP4 1664
ADDRLP4 12
INDIRI4
ADDRLP4 16
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 1664
INDIRI4
CNSTI4 0
EQI4 $369
ADDRLP4 1664
INDIRI4
CNSTI4 45
NEI4 $366
LABELV $369
line 426
;426:	singleBuf[c] = 0;
ADDRLP4 0
INDIRI4
ADDRLP4 528
ADDP4
CNSTI1 0
ASGNI1
line 427
;427:	c = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 428
;428:	i++;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 430
;429:
;430:	iBuf = atoi(singleBuf);
ADDRLP4 528
ARGP4
ADDRLP4 1668
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 592
ADDRLP4 1668
INDIRI4
ASGNI4
line 432
;431:
;432:	if (iBuf > uiMaxRank || iBuf < 0)
ADDRLP4 592
INDIRI4
ADDRGP4 uiMaxRank
INDIRI4
GTI4 $372
ADDRLP4 592
INDIRI4
CNSTI4 0
GEI4 $370
LABELV $372
line 433
;433:	{ //this force config uses a rank level higher than our currently restricted level.. so we can't use it
line 436
;434:	  //FIXME: Print a message indicating this to the user
;435:	//	return;
;436:	}
LABELV $370
line 438
;437:
;438:	uiForceRank = iBuf;
ADDRGP4 uiForceRank
ADDRLP4 592
INDIRI4
ASGNI4
ADDRGP4 $374
JUMPV
LABELV $373
line 441
;439:
;440:	while (fcfString[i] && fcfString[i] != '-')
;441:	{
line 442
;442:		singleBuf[c] = fcfString[i];
ADDRLP4 0
INDIRI4
ADDRLP4 528
ADDP4
ADDRLP4 12
INDIRI4
ADDRLP4 16
ADDP4
INDIRI1
ASGNI1
line 443
;443:		c++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 444
;444:		i++;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 445
;445:	}
LABELV $374
line 440
ADDRLP4 1676
ADDRLP4 12
INDIRI4
ADDRLP4 16
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 1676
INDIRI4
CNSTI4 0
EQI4 $376
ADDRLP4 1676
INDIRI4
CNSTI4 45
NEI4 $373
LABELV $376
line 446
;446:	singleBuf[c] = 0;
ADDRLP4 0
INDIRI4
ADDRLP4 528
ADDP4
CNSTI1 0
ASGNI1
line 447
;447:	c = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 448
;448:	i++;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 450
;449:
;450:	uiForceSide = atoi(singleBuf);
ADDRLP4 528
ARGP4
ADDRLP4 1680
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRGP4 uiForceSide
ADDRLP4 1680
INDIRI4
ASGNI4
line 452
;451:
;452:	if (uiForceSide != FORCE_LIGHTSIDE &&
ADDRLP4 1684
ADDRGP4 uiForceSide
INDIRI4
ASGNI4
ADDRLP4 1684
INDIRI4
CNSTI4 1
EQI4 $380
ADDRLP4 1684
INDIRI4
CNSTI4 2
EQI4 $380
line 454
;453:		uiForceSide != FORCE_DARKSIDE)
;454:	{
line 455
;455:		uiForceSide = FORCE_LIGHTSIDE;
ADDRGP4 uiForceSide
CNSTI4 1
ASGNI4
line 456
;456:		return;
ADDRGP4 $348
JUMPV
LABELV $379
line 461
;457:	}
;458:
;459:	//clear out the existing powers
;460:	while (c < NUM_FORCE_POWERS)
;461:	{
line 462
;462:		uiForcePowersRank[c] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
CNSTI4 0
ASGNI4
line 463
;463:		c++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 464
;464:	}
LABELV $380
line 460
ADDRLP4 0
INDIRI4
CNSTI4 18
LTI4 $379
line 465
;465:	uiForceUsed = 0;
ADDRGP4 uiForceUsed
CNSTI4 0
ASGNI4
line 466
;466:	uiForceAvailable = forceMasteryPoints[uiForceRank];
ADDRGP4 uiForceAvailable
ADDRGP4 uiForceRank
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 forceMasteryPoints
ADDP4
INDIRI4
ASGNI4
line 467
;467:	gTouchedForce = qtrue;
ADDRGP4 gTouchedForce
CNSTI4 1
ASGNI4
line 469
;468:
;469:	for (c=0;fcfString[i]&&c<NUM_FORCE_POWERS;c++,i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $385
JUMPV
LABELV $382
line 470
;470:	{
line 471
;471:		singleBuf[0] = fcfString[i];
ADDRLP4 528
ADDRLP4 12
INDIRI4
ADDRLP4 16
ADDP4
INDIRI1
ASGNI1
line 472
;472:		singleBuf[1] = 0;
ADDRLP4 528+1
CNSTI1 0
ASGNI1
line 473
;473:		iBuf = atoi(singleBuf);	// So, that means that Force Power "c" wants to be set to rank "iBuf".
ADDRLP4 528
ARGP4
ADDRLP4 1688
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 592
ADDRLP4 1688
INDIRI4
ASGNI4
line 475
;474:		
;475:		if (iBuf < 0)
ADDRLP4 592
INDIRI4
CNSTI4 0
GEI4 $387
line 476
;476:		{
line 477
;477:			iBuf = 0;
ADDRLP4 592
CNSTI4 0
ASGNI4
line 478
;478:		}
LABELV $387
line 480
;479:
;480:		forcePowerRank = iBuf;
ADDRLP4 8
ADDRLP4 592
INDIRI4
ASGNI4
line 482
;481:
;482:		if (forcePowerRank > FORCE_LEVEL_3 || forcePowerRank < 0)
ADDRLP4 8
INDIRI4
CNSTI4 3
GTI4 $391
ADDRLP4 8
INDIRI4
CNSTI4 0
GEI4 $389
LABELV $391
line 483
;483:		{ //err..  not correct
line 484
;484:			continue;  // skip this power
ADDRGP4 $383
JUMPV
LABELV $389
line 487
;485:		}
;486:
;487:		if (uiForcePowerDarkLight[c] && uiForcePowerDarkLight[c] != uiForceSide)
ADDRLP4 1696
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowerDarkLight
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1696
INDIRI4
CNSTI4 0
EQI4 $392
ADDRLP4 1696
INDIRI4
ADDRGP4 uiForceSide
INDIRI4
EQI4 $392
line 488
;488:		{ //Apparently the user has crafted a force config that has powers that don't fit with the config's side.
line 489
;489:			continue;  // skip this power
ADDRGP4 $383
JUMPV
LABELV $392
line 493
;490:		}
;491:
;492:		// Accrue cost for each assigned rank for this power.
;493:		for (currank=FORCE_LEVEL_1;currank<=forcePowerRank;currank++)
ADDRLP4 4
CNSTI4 1
ASGNI4
ADDRGP4 $397
JUMPV
LABELV $394
line 494
;494:		{	
line 495
;495:			if (bgForcePowerCost[c][currank] > uiForceAvailable)
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
ADDRGP4 uiForceAvailable
INDIRI4
LEI4 $398
line 496
;496:			{	// Break out, we can't afford any more power.
line 497
;497:				break;
ADDRGP4 $396
JUMPV
LABELV $398
line 500
;498:			}
;499:			// Pay for this rank of this power.
;500:			uiForceUsed += bgForcePowerCost[c][currank];
ADDRLP4 1700
ADDRGP4 uiForceUsed
ASGNP4
ADDRLP4 1700
INDIRP4
ADDRLP4 1700
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 501
;501:			uiForceAvailable -= bgForcePowerCost[c][currank];
ADDRLP4 1704
ADDRGP4 uiForceAvailable
ASGNP4
ADDRLP4 1704
INDIRP4
ADDRLP4 1704
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
SUBI4
ASGNI4
line 503
;502:
;503:			uiForcePowersRank[c]++;
ADDRLP4 1708
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
ASGNP4
ADDRLP4 1708
INDIRP4
ADDRLP4 1708
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 504
;504:		}
LABELV $395
line 493
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $397
ADDRLP4 4
INDIRI4
ADDRLP4 8
INDIRI4
LEI4 $394
LABELV $396
line 505
;505:	}
LABELV $383
line 469
ADDRLP4 1688
CNSTI4 1
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 1688
INDIRI4
ADDI4
ASGNI4
ADDRLP4 12
ADDRLP4 12
INDIRI4
ADDRLP4 1688
INDIRI4
ADDI4
ASGNI4
LABELV $385
ADDRLP4 12
INDIRI4
ADDRLP4 16
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $400
ADDRLP4 0
INDIRI4
CNSTI4 18
LTI4 $382
LABELV $400
line 507
;506:
;507:	if (uiForcePowersRank[FP_LEVITATION] < 1)
ADDRGP4 uiForcePowersRank+4
INDIRI4
CNSTI4 1
GEI4 $401
line 508
;508:	{
line 509
;509:		uiForcePowersRank[FP_LEVITATION]=1;
ADDRGP4 uiForcePowersRank+4
CNSTI4 1
ASGNI4
line 510
;510:	}
LABELV $401
line 511
;511:	if (uiForcePowersRank[FP_SABERATTACK] < 1 && ui_freeSaber.integer)
ADDRGP4 uiForcePowersRank+60
INDIRI4
CNSTI4 1
GEI4 $405
ADDRGP4 ui_freeSaber+12
INDIRI4
CNSTI4 0
EQI4 $405
line 512
;512:	{
line 513
;513:		uiForcePowersRank[FP_SABERATTACK]=1;
ADDRGP4 uiForcePowersRank+60
CNSTI4 1
ASGNI4
line 514
;514:	}
LABELV $405
line 515
;515:	if (uiForcePowersRank[FP_SABERDEFEND] < 1 && ui_freeSaber.integer)
ADDRGP4 uiForcePowersRank+64
INDIRI4
CNSTI4 1
GEI4 $410
ADDRGP4 ui_freeSaber+12
INDIRI4
CNSTI4 0
EQI4 $410
line 516
;516:	{
line 517
;517:		uiForcePowersRank[FP_SABERDEFEND]=1;
ADDRGP4 uiForcePowersRank+64
CNSTI4 1
ASGNI4
line 518
;518:	}
LABELV $410
line 520
;519:
;520:	UpdateForceUsed();
ADDRGP4 UpdateForceUsed
CALLV
pop
line 522
;521:
;522:	if (updateForceLater)
ADDRLP4 1632
INDIRI4
CNSTI4 0
EQI4 $415
line 523
;523:	{
line 524
;524:		gTouchedForce = qtrue;
ADDRGP4 gTouchedForce
CNSTI4 1
ASGNI4
line 525
;525:		UI_UpdateClientForcePowers(NULL);
CNSTP4 0
ARGP4
ADDRGP4 UI_UpdateClientForcePowers
CALLV
pop
line 526
;526:	}
LABELV $415
line 527
;527:}
LABELV $348
endproc UI_ReadLegalForce 1712 24
export UI_UpdateForcePowers
proc UI_UpdateForcePowers 324 4
line 530
;528:
;529:void UI_UpdateForcePowers()
;530:{
line 531
;531:	char *forcePowers = UI_Cvar_VariableString("forcepowers");
ADDRGP4 $217
ARGP4
ADDRLP4 272
ADDRGP4 UI_Cvar_VariableString
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 272
INDIRP4
ASGNP4
line 533
;532:	char readBuf[256];
;533:	int i = 0, i_f = 0, i_r = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 12
CNSTI4 0
ASGNI4
line 535
;534:
;535:	uiForceSide = 0;
ADDRGP4 uiForceSide
CNSTI4 0
ASGNI4
line 537
;536:
;537:	if (forcePowers && forcePowers[0])
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $418
ADDRLP4 8
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $418
line 538
;538:	{
ADDRGP4 $421
JUMPV
LABELV $420
line 540
;539:		while (forcePowers[i])
;540:		{
line 541
;541:			i_r = 0;
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRGP4 $424
JUMPV
LABELV $423
line 544
;542:
;543:			while (forcePowers[i] && forcePowers[i] != '-' && i_r < 255)
;544:			{
line 545
;545:				readBuf[i_r] = forcePowers[i];
ADDRLP4 12
INDIRI4
ADDRLP4 16
ADDP4
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRP4
ADDP4
INDIRI1
ASGNI1
line 546
;546:				i_r++;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 547
;547:				i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 548
;548:			}
LABELV $424
line 543
ADDRLP4 280
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRP4
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 280
INDIRI4
CNSTI4 0
EQI4 $427
ADDRLP4 280
INDIRI4
CNSTI4 45
EQI4 $427
ADDRLP4 12
INDIRI4
CNSTI4 255
LTI4 $423
LABELV $427
line 549
;549:			readBuf[i_r] = '\0';
ADDRLP4 12
INDIRI4
ADDRLP4 16
ADDP4
CNSTI1 0
ASGNI1
line 550
;550:			if (i_r >= 255 || !forcePowers[i] || forcePowers[i] != '-')
ADDRLP4 12
INDIRI4
CNSTI4 255
GEI4 $431
ADDRLP4 284
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRP4
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 284
INDIRI4
CNSTI4 0
EQI4 $431
ADDRLP4 284
INDIRI4
CNSTI4 45
EQI4 $428
LABELV $431
line 551
;551:			{
line 552
;552:				uiForceSide = 0;
ADDRGP4 uiForceSide
CNSTI4 0
ASGNI4
line 553
;553:				goto validitycheck;
ADDRGP4 $432
JUMPV
LABELV $428
line 555
;554:			}
;555:			uiForceRank = atoi(readBuf);
ADDRLP4 16
ARGP4
ADDRLP4 288
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRGP4 uiForceRank
ADDRLP4 288
INDIRI4
ASGNI4
line 556
;556:			i_r = 0;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 558
;557:
;558:			if (uiForceRank > uiMaxRank)
ADDRGP4 uiForceRank
INDIRI4
ADDRGP4 uiMaxRank
INDIRI4
LEI4 $433
line 559
;559:			{
line 560
;560:				uiForceRank = uiMaxRank;
ADDRGP4 uiForceRank
ADDRGP4 uiMaxRank
INDIRI4
ASGNI4
line 561
;561:			}
LABELV $433
line 563
;562:
;563:			i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRGP4 $436
JUMPV
LABELV $435
line 566
;564:
;565:			while (forcePowers[i] && forcePowers[i] != '-' && i_r < 255)
;566:			{
line 567
;567:				readBuf[i_r] = forcePowers[i];
ADDRLP4 12
INDIRI4
ADDRLP4 16
ADDP4
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRP4
ADDP4
INDIRI1
ASGNI1
line 568
;568:				i_r++;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 569
;569:				i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 570
;570:			}
LABELV $436
line 565
ADDRLP4 292
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRP4
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 292
INDIRI4
CNSTI4 0
EQI4 $439
ADDRLP4 292
INDIRI4
CNSTI4 45
EQI4 $439
ADDRLP4 12
INDIRI4
CNSTI4 255
LTI4 $435
LABELV $439
line 571
;571:			readBuf[i_r] = '\0';
ADDRLP4 12
INDIRI4
ADDRLP4 16
ADDP4
CNSTI1 0
ASGNI1
line 572
;572:			if (i_r >= 255 || !forcePowers[i] || forcePowers[i] != '-')
ADDRLP4 12
INDIRI4
CNSTI4 255
GEI4 $443
ADDRLP4 296
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRP4
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 296
INDIRI4
CNSTI4 0
EQI4 $443
ADDRLP4 296
INDIRI4
CNSTI4 45
EQI4 $440
LABELV $443
line 573
;573:			{
line 574
;574:				uiForceSide = 0;
ADDRGP4 uiForceSide
CNSTI4 0
ASGNI4
line 575
;575:				goto validitycheck;
ADDRGP4 $432
JUMPV
LABELV $440
line 577
;576:			}
;577:			uiForceSide = atoi(readBuf);
ADDRLP4 16
ARGP4
ADDRLP4 300
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRGP4 uiForceSide
ADDRLP4 300
INDIRI4
ASGNI4
line 578
;578:			i_r = 0;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 580
;579:
;580:			i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 582
;581:
;582:			i_f = FP_HEAL;
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $445
JUMPV
LABELV $444
line 585
;583:
;584:			while (forcePowers[i] && i_f < NUM_FORCE_POWERS)
;585:			{
line 586
;586:				readBuf[0] = forcePowers[i];
ADDRLP4 16
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRP4
ADDP4
INDIRI1
ASGNI1
line 587
;587:				readBuf[1] = '\0';
ADDRLP4 16+1
CNSTI1 0
ASGNI1
line 588
;588:				uiForcePowersRank[i_f] = atoi(readBuf);
ADDRLP4 16
ARGP4
ADDRLP4 304
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
ADDRLP4 304
INDIRI4
ASGNI4
line 590
;589:
;590:				if (i_f == FP_LEVITATION &&
ADDRLP4 312
CNSTI4 1
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 312
INDIRI4
NEI4 $448
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
INDIRI4
ADDRLP4 312
INDIRI4
GEI4 $448
line 592
;591:					uiForcePowersRank[i_f] < 1)
;592:				{
line 593
;593:					uiForcePowersRank[i_f] = 1;
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
CNSTI4 1
ASGNI4
line 594
;594:				}
LABELV $448
line 596
;595:
;596:				if (i_f == FP_SABERATTACK &&
ADDRLP4 4
INDIRI4
CNSTI4 15
NEI4 $450
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
INDIRI4
CNSTI4 1
GEI4 $450
ADDRGP4 ui_freeSaber+12
INDIRI4
CNSTI4 0
EQI4 $450
line 599
;597:					uiForcePowersRank[i_f] < 1 &&
;598:					ui_freeSaber.integer)
;599:				{
line 600
;600:					uiForcePowersRank[i_f] = 1;
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
CNSTI4 1
ASGNI4
line 601
;601:				}
LABELV $450
line 603
;602:
;603:				if (i_f == FP_SABERDEFEND &&
ADDRLP4 4
INDIRI4
CNSTI4 16
NEI4 $453
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
INDIRI4
CNSTI4 1
GEI4 $453
ADDRGP4 ui_freeSaber+12
INDIRI4
CNSTI4 0
EQI4 $453
line 606
;604:					uiForcePowersRank[i_f] < 1 &&
;605:					ui_freeSaber.integer)
;606:				{
line 607
;607:					uiForcePowersRank[i_f] = 1;
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
CNSTI4 1
ASGNI4
line 608
;608:				}
LABELV $453
line 610
;609:
;610:				i_f++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 611
;611:				i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 612
;612:			}
LABELV $445
line 584
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $456
ADDRLP4 4
INDIRI4
CNSTI4 18
LTI4 $444
LABELV $456
line 614
;613:
;614:			if (i_f < NUM_FORCE_POWERS)
ADDRLP4 4
INDIRI4
CNSTI4 18
GEI4 $457
line 615
;615:			{ //info for all the powers wasn't there..
line 616
;616:				uiForceSide = 0;
ADDRGP4 uiForceSide
CNSTI4 0
ASGNI4
line 617
;617:				goto validitycheck;
ADDRGP4 $432
JUMPV
LABELV $457
line 619
;618:			}
;619:			i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 620
;620:		}
LABELV $421
line 539
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $420
line 621
;621:	}
LABELV $418
LABELV $432
line 625
;622:
;623:validitycheck:
;624:
;625:	if (!uiForceSide)
ADDRGP4 uiForceSide
INDIRI4
CNSTI4 0
NEI4 $459
line 626
;626:	{
line 627
;627:		uiForceSide = 1;
ADDRGP4 uiForceSide
CNSTI4 1
ASGNI4
line 628
;628:		uiForceRank = 1;
ADDRGP4 uiForceRank
CNSTI4 1
ASGNI4
line 629
;629:		i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $462
JUMPV
LABELV $461
line 631
;630:		while (i < NUM_FORCE_POWERS)
;631:		{
line 632
;632:			if (i == FP_LEVITATION)
ADDRLP4 0
INDIRI4
CNSTI4 1
NEI4 $464
line 633
;633:			{
line 634
;634:				uiForcePowersRank[i] = 1;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
CNSTI4 1
ASGNI4
line 635
;635:			}
ADDRGP4 $465
JUMPV
LABELV $464
line 636
;636:			else if (i == FP_SABERATTACK && ui_freeSaber.integer)
ADDRLP4 0
INDIRI4
CNSTI4 15
NEI4 $466
ADDRGP4 ui_freeSaber+12
INDIRI4
CNSTI4 0
EQI4 $466
line 637
;637:			{
line 638
;638:				uiForcePowersRank[i] = 1;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
CNSTI4 1
ASGNI4
line 639
;639:			}
ADDRGP4 $467
JUMPV
LABELV $466
line 640
;640:			else if (i == FP_SABERDEFEND && ui_freeSaber.integer)
ADDRLP4 0
INDIRI4
CNSTI4 16
NEI4 $469
ADDRGP4 ui_freeSaber+12
INDIRI4
CNSTI4 0
EQI4 $469
line 641
;641:			{
line 642
;642:				uiForcePowersRank[i] = 1;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
CNSTI4 1
ASGNI4
line 643
;643:			}
ADDRGP4 $470
JUMPV
LABELV $469
line 645
;644:			else
;645:			{
line 646
;646:				uiForcePowersRank[i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
CNSTI4 0
ASGNI4
line 647
;647:			}
LABELV $470
LABELV $467
LABELV $465
line 649
;648:
;649:			i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 650
;650:		}
LABELV $462
line 630
ADDRLP4 0
INDIRI4
CNSTI4 18
LTI4 $461
line 652
;651:
;652:		UI_UpdateClientForcePowers(NULL);
CNSTP4 0
ARGP4
ADDRGP4 UI_UpdateClientForcePowers
CALLV
pop
line 653
;653:	}
LABELV $459
line 655
;654:
;655:	UpdateForceUsed();
ADDRGP4 UpdateForceUsed
CALLV
pop
line 656
;656:}
LABELV $417
endproc UI_UpdateForcePowers 324 4
export UI_SkinColor_HandleKey
proc UI_SkinColor_HandleKey 8 8
line 660
;657:extern int	uiSkinColor;
;658:
;659:qboolean UI_SkinColor_HandleKey(int flags, float *special, int key, int num, int min, int max, int type) 
;660:{
line 661
;661:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) 
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $477
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $477
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $477
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $473
LABELV $477
line 662
;662:  {
line 663
;663:  	int i = num;
ADDRLP4 4
ADDRFP4 12
INDIRI4
ASGNI4
line 665
;664:
;665:	if (key == K_MOUSE2)
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $478
line 666
;666:	{
line 667
;667:	    i--;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 668
;668:	}
ADDRGP4 $479
JUMPV
LABELV $478
line 670
;669:	else
;670:	{
line 671
;671:	    i++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 672
;672:	}
LABELV $479
line 674
;673:
;674:    if (i < min)
ADDRLP4 4
INDIRI4
ADDRFP4 16
INDIRI4
GEI4 $480
line 675
;675:	{
line 676
;676:		i = max;
ADDRLP4 4
ADDRFP4 20
INDIRI4
ASGNI4
line 677
;677:	}
ADDRGP4 $481
JUMPV
LABELV $480
line 678
;678:	else if (i > max)
ADDRLP4 4
INDIRI4
ADDRFP4 20
INDIRI4
LEI4 $482
line 679
;679:	{
line 680
;680:      i = min;
ADDRLP4 4
ADDRFP4 16
INDIRI4
ASGNI4
line 681
;681:    }
LABELV $482
LABELV $481
line 683
;682:
;683:    num = i;
ADDRFP4 12
ADDRLP4 4
INDIRI4
ASGNI4
line 685
;684:
;685:	uiSkinColor = num;
ADDRGP4 uiSkinColor
ADDRFP4 12
INDIRI4
ASGNI4
line 687
;686:
;687:	UI_FeederSelection(FEEDER_Q3HEADS, uiInfo.q3SelectedHead);
CNSTF4 1094713344
ARGF4
ADDRGP4 uiInfo+78144
INDIRI4
ARGI4
ADDRGP4 UI_FeederSelection
CALLI4
pop
line 689
;688:
;689:    return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $472
JUMPV
LABELV $473
line 691
;690:  }
;691:  return qfalse;
CNSTI4 0
RETI4
LABELV $472
endproc UI_SkinColor_HandleKey 8 8
export UI_ForceSide_HandleKey
proc UI_ForceSide_HandleKey 1048 16
line 698
;692:}
;693:
;694:
;695:
;696:
;697:qboolean UI_ForceSide_HandleKey(int flags, float *special, int key, int num, int min, int max, int type) 
;698:{
line 701
;699:	char info[MAX_INFO_VALUE];
;700:
;701:	info[0] = '\0';
ADDRLP4 0
CNSTI1 0
ASGNI1
line 702
;702:	trap_GetConfigString(CS_SERVERINFO, info, sizeof(info));
CNSTI4 0
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigString
CALLI4
pop
line 704
;703:
;704:	if (atoi( Info_ValueForKey( info, "g_forceBasedTeams" ) ))
ADDRLP4 0
ARGP4
ADDRGP4 $355
ARGP4
ADDRLP4 1024
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1024
INDIRP4
ARGP4
ADDRLP4 1028
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 1028
INDIRI4
CNSTI4 0
EQI4 $486
line 705
;705:	{
line 706
;706:		switch((int)(trap_Cvar_VariableValue("ui_myteam")))
ADDRGP4 $358
ARGP4
ADDRLP4 1036
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 1032
ADDRLP4 1036
INDIRF4
CVFI4 4
ASGNI4
ADDRLP4 1032
INDIRI4
CNSTI4 1
EQI4 $491
ADDRLP4 1032
INDIRI4
CNSTI4 2
EQI4 $492
ADDRGP4 $489
JUMPV
line 707
;707:		{
LABELV $491
line 709
;708:		case TEAM_RED:
;709:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $485
JUMPV
LABELV $492
line 711
;710:		case TEAM_BLUE:
;711:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $485
JUMPV
line 713
;712:		default:
;713:			break;
LABELV $489
line 715
;714:		}
;715:	}
LABELV $486
line 717
;716:
;717:	if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) 
ADDRLP4 1032
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 1032
INDIRI4
CNSTI4 178
EQI4 $497
ADDRLP4 1032
INDIRI4
CNSTI4 179
EQI4 $497
ADDRLP4 1032
INDIRI4
CNSTI4 13
EQI4 $497
ADDRLP4 1032
INDIRI4
CNSTI4 169
NEI4 $493
LABELV $497
line 718
;718:	{
line 719
;719:		int i = num;
ADDRLP4 1040
ADDRFP4 12
INDIRI4
ASGNI4
line 720
;720:		int x = 0;
ADDRLP4 1036
CNSTI4 0
ASGNI4
line 723
;721:
;722:		//update the feeder item selection, it might be different depending on side
;723:		Menu_SetFeederSelection(NULL, FEEDER_FORCECFG, 0, NULL);
CNSTP4 0
ARGP4
CNSTI4 16
ARGI4
CNSTI4 0
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 Menu_SetFeederSelection
CALLV
pop
line 725
;724:
;725:		if (key == K_MOUSE2)
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $498
line 726
;726:		{
line 727
;727:			i--;
ADDRLP4 1040
ADDRLP4 1040
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 728
;728:		}
ADDRGP4 $499
JUMPV
LABELV $498
line 730
;729:		else
;730:		{
line 731
;731:			i++;
ADDRLP4 1040
ADDRLP4 1040
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 732
;732:		}
LABELV $499
line 734
;733:
;734:		if (i < min)
ADDRLP4 1040
INDIRI4
ADDRFP4 16
INDIRI4
GEI4 $500
line 735
;735:		{
line 736
;736:			i = max;
ADDRLP4 1040
ADDRFP4 20
INDIRI4
ASGNI4
line 737
;737:		}
ADDRGP4 $501
JUMPV
LABELV $500
line 738
;738:		else if (i > max)
ADDRLP4 1040
INDIRI4
ADDRFP4 20
INDIRI4
LEI4 $502
line 739
;739:		{
line 740
;740:			i = min;
ADDRLP4 1040
ADDRFP4 16
INDIRI4
ASGNI4
line 741
;741:		}
LABELV $502
LABELV $501
line 743
;742:
;743:		num = i;
ADDRFP4 12
ADDRLP4 1040
INDIRI4
ASGNI4
line 745
;744:
;745:		uiForceSide = num;
ADDRGP4 uiForceSide
ADDRFP4 12
INDIRI4
ASGNI4
ADDRGP4 $505
JUMPV
LABELV $504
line 749
;746:
;747:		// Resetting power ranks based on if light or dark side is chosen
;748:		while (x < NUM_FORCE_POWERS)
;749:		{
line 750
;750:			if (uiForcePowerDarkLight[x] && uiForceSide != uiForcePowerDarkLight[x])
ADDRLP4 1044
ADDRLP4 1036
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowerDarkLight
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
EQI4 $507
ADDRGP4 uiForceSide
INDIRI4
ADDRLP4 1044
INDIRI4
EQI4 $507
line 751
;751:			{
line 752
;752:				uiForcePowersRank[x] = 0;
ADDRLP4 1036
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
CNSTI4 0
ASGNI4
line 753
;753:			}
LABELV $507
line 754
;754:			x++;
ADDRLP4 1036
ADDRLP4 1036
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 755
;755:		}
LABELV $505
line 748
ADDRLP4 1036
INDIRI4
CNSTI4 18
LTI4 $504
line 757
;756:
;757:		UpdateForceUsed();
ADDRGP4 UpdateForceUsed
CALLV
pop
line 759
;758:
;759:		gTouchedForce = qtrue;
ADDRGP4 gTouchedForce
CNSTI4 1
ASGNI4
line 760
;760:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $485
JUMPV
LABELV $493
line 762
;761:	}
;762:	return qfalse;
CNSTI4 0
RETI4
LABELV $485
endproc UI_ForceSide_HandleKey 1048 16
export UI_ForceMaxRank_HandleKey
proc UI_ForceMaxRank_HandleKey 12 8
line 767
;763:}
;764:
;765:
;766:qboolean UI_ForceMaxRank_HandleKey(int flags, float *special, int key, int num, int min, int max, int type) 
;767:{
line 768
;768:  if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) 
ADDRLP4 0
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 178
EQI4 $514
ADDRLP4 0
INDIRI4
CNSTI4 179
EQI4 $514
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $514
ADDRLP4 0
INDIRI4
CNSTI4 169
NEI4 $510
LABELV $514
line 769
;769:  {
line 770
;770:  	int i = num;
ADDRLP4 4
ADDRFP4 12
INDIRI4
ASGNI4
line 772
;771:
;772:	if (key == K_MOUSE2)
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $515
line 773
;773:	{
line 774
;774:	    i--;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 775
;775:	}
ADDRGP4 $516
JUMPV
LABELV $515
line 777
;776:	else
;777:	{
line 778
;778:	    i++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 779
;779:	}
LABELV $516
line 781
;780:
;781:    if (i < min)
ADDRLP4 4
INDIRI4
ADDRFP4 16
INDIRI4
GEI4 $517
line 782
;782:	{
line 783
;783:		i = max;
ADDRLP4 4
ADDRFP4 20
INDIRI4
ASGNI4
line 784
;784:	}
ADDRGP4 $518
JUMPV
LABELV $517
line 785
;785:	else if (i > max)
ADDRLP4 4
INDIRI4
ADDRFP4 20
INDIRI4
LEI4 $519
line 786
;786:	{
line 787
;787:      i = min;
ADDRLP4 4
ADDRFP4 16
INDIRI4
ASGNI4
line 788
;788:    }
LABELV $519
LABELV $518
line 790
;789:
;790:    num = i;
ADDRFP4 12
ADDRLP4 4
INDIRI4
ASGNI4
line 792
;791:
;792:	uiMaxRank = num;
ADDRGP4 uiMaxRank
ADDRFP4 12
INDIRI4
ASGNI4
line 794
;793:
;794:	trap_Cvar_Set( "g_maxForceRank", va("%i", num));
ADDRGP4 $264
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $521
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 797
;795:
;796:	// The update force used will remove overallocated powers automatically.
;797:	UpdateForceUsed();
ADDRGP4 UpdateForceUsed
CALLV
pop
line 799
;798:
;799:	gTouchedForce = qtrue;
ADDRGP4 gTouchedForce
CNSTI4 1
ASGNI4
line 801
;800:
;801:    return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $509
JUMPV
LABELV $510
line 803
;802:  }
;803:  return qfalse;
CNSTI4 0
RETI4
LABELV $509
endproc UI_ForceMaxRank_HandleKey 12 8
export UI_ForcePowerRank_HandleKey
proc UI_ForcePowerRank_HandleKey 36 0
line 809
;804:}
;805:
;806:
;807:// This function will either raise or lower a power by one rank.
;808:qboolean UI_ForcePowerRank_HandleKey(int flags, float *special, int key, int num, int min, int max, int type) 
;809:{
line 812
;810:	qboolean raising;
;811:
;812:	if (key == K_MOUSE1 || key == K_MOUSE2 || key == K_ENTER || key == K_KP_ENTER) 
ADDRLP4 4
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 178
EQI4 $527
ADDRLP4 4
INDIRI4
CNSTI4 179
EQI4 $527
ADDRLP4 4
INDIRI4
CNSTI4 13
EQI4 $527
ADDRLP4 4
INDIRI4
CNSTI4 169
NEI4 $523
LABELV $527
line 813
;813:	{
line 817
;814:		int forcepower, rank;
;815:
;816:		//this will give us the index as long as UI_FORCE_RANK is always one below the first force rank index
;817:		forcepower = (type-UI_FORCE_RANK)-1;
ADDRLP4 8
ADDRFP4 24
INDIRI4
CNSTI4 258
SUBI4
CNSTI4 1
SUBI4
ASGNI4
line 820
;818:				
;819:		// If we are not on the same side as a power, or if we are not of any rank at all.
;820:		if (uiForcePowerDarkLight[forcepower] && uiForceSide != uiForcePowerDarkLight[forcepower])
ADDRLP4 16
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowerDarkLight
ADDP4
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $528
ADDRGP4 uiForceSide
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $528
line 821
;821:		{
line 822
;822:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $522
JUMPV
LABELV $528
line 824
;823:		}
;824:		else if (forcepower == FP_SABERDEFEND || forcepower == FP_SABERTHROW)
ADDRLP4 8
INDIRI4
CNSTI4 16
EQI4 $532
ADDRLP4 8
INDIRI4
CNSTI4 17
NEI4 $530
LABELV $532
line 825
;825:		{	// Saberdefend and saberthrow can't be bought if there is no saberattack
line 826
;826:			if (uiForcePowersRank[FP_SABERATTACK] < 1)
ADDRGP4 uiForcePowersRank+60
INDIRI4
CNSTI4 1
GEI4 $533
line 827
;827:			{
line 828
;828:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $522
JUMPV
LABELV $533
line 830
;829:			}
;830:		}
LABELV $530
line 832
;831:
;832:		if (type == UI_FORCE_RANK_LEVITATION)
ADDRFP4 24
INDIRI4
CNSTI4 260
NEI4 $536
line 833
;833:		{
line 834
;834:			min += 1;
ADDRFP4 16
ADDRFP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 835
;835:		}
LABELV $536
line 836
;836:		if (type == UI_FORCE_RANK_SABERATTACK && ui_freeSaber.integer)
ADDRFP4 24
INDIRI4
CNSTI4 274
NEI4 $538
ADDRGP4 ui_freeSaber+12
INDIRI4
CNSTI4 0
EQI4 $538
line 837
;837:		{
line 838
;838:			min += 1;
ADDRFP4 16
ADDRFP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 839
;839:		}
LABELV $538
line 840
;840:		if (type == UI_FORCE_RANK_SABERDEFEND && ui_freeSaber.integer)
ADDRFP4 24
INDIRI4
CNSTI4 275
NEI4 $541
ADDRGP4 ui_freeSaber+12
INDIRI4
CNSTI4 0
EQI4 $541
line 841
;841:		{
line 842
;842:			min += 1;
ADDRFP4 16
ADDRFP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 843
;843:		}
LABELV $541
line 845
;844:
;845:		if (key == K_MOUSE2)
ADDRFP4 8
INDIRI4
CNSTI4 179
NEI4 $544
line 846
;846:		{	// Lower a point.
line 847
;847:			if (uiForcePowersRank[forcepower]<=min)
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
INDIRI4
ADDRFP4 16
INDIRI4
GTI4 $546
line 848
;848:			{
line 849
;849:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $522
JUMPV
LABELV $546
line 851
;850:			}
;851:			raising = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 852
;852:		}
ADDRGP4 $545
JUMPV
LABELV $544
line 854
;853:		else
;854:		{	// Raise a point.
line 855
;855:			if (uiForcePowersRank[forcepower]>=max)
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
INDIRI4
ADDRFP4 20
INDIRI4
LTI4 $548
line 856
;856:			{
line 857
;857:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $522
JUMPV
LABELV $548
line 859
;858:			}
;859:			raising = qtrue;
ADDRLP4 0
CNSTI4 1
ASGNI4
line 860
;860:		}
LABELV $545
line 862
;861:
;862:		if (raising)
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $550
line 863
;863:		{	// Check if we can accrue the cost of this power.
line 864
;864:			rank = uiForcePowersRank[forcepower]+1;
ADDRLP4 12
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 865
;865:			if (bgForcePowerCost[forcepower][rank] > uiForceAvailable)
ADDRLP4 12
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
ADDRGP4 uiForceAvailable
INDIRI4
LEI4 $552
line 866
;866:			{	// We can't afford this power.  Abandon ship.
line 867
;867:				return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $522
JUMPV
LABELV $552
line 870
;868:			}
;869:			else
;870:			{	// Sure we can afford it.
line 871
;871:				uiForceUsed += bgForcePowerCost[forcepower][rank];
ADDRLP4 24
ADDRGP4 uiForceUsed
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
ADDRLP4 12
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 872
;872:				uiForceAvailable -= bgForcePowerCost[forcepower][rank];
ADDRLP4 28
ADDRGP4 uiForceAvailable
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
ADDRLP4 12
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
SUBI4
ASGNI4
line 873
;873:				uiForcePowersRank[forcepower]=rank;
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
ADDRLP4 12
INDIRI4
ASGNI4
line 874
;874:			}
line 875
;875:		}
ADDRGP4 $551
JUMPV
LABELV $550
line 877
;876:		else
;877:		{	// Lower the point.
line 878
;878:			rank = uiForcePowersRank[forcepower];
ADDRLP4 12
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
INDIRI4
ASGNI4
line 879
;879:			uiForceUsed -= bgForcePowerCost[forcepower][rank];
ADDRLP4 24
ADDRGP4 uiForceUsed
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
ADDRLP4 12
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
SUBI4
ASGNI4
line 880
;880:			uiForceAvailable += bgForcePowerCost[forcepower][rank];
ADDRLP4 28
ADDRGP4 uiForceAvailable
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
ADDRLP4 12
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 881
;881:			uiForcePowersRank[forcepower]--;
ADDRLP4 32
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 882
;882:		}
LABELV $551
line 884
;883:
;884:		UpdateForceUsed();
ADDRGP4 UpdateForceUsed
CALLV
pop
line 886
;885:
;886:		gTouchedForce = qtrue;
ADDRGP4 gTouchedForce
CNSTI4 1
ASGNI4
line 888
;887:
;888:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $522
JUMPV
LABELV $523
line 890
;889:	}
;890:	return qfalse;
CNSTI4 0
RETI4
LABELV $522
endproc UI_ForcePowerRank_HandleKey 36 0
data
export gCustRank
align 4
LABELV gCustRank
byte 4 0
export gCustSide
align 4
LABELV gCustSide
byte 4 0
export gCustPowersRank
align 4
LABELV gCustPowersRank
byte 4 0
byte 4 1
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
export UI_ForceConfigHandle
code
proc UI_ForceConfigHandle 9376 24
line 924
;891:}
;892:
;893:
;894:int gCustRank = 0;
;895:int gCustSide = 0;
;896:
;897:int gCustPowersRank[NUM_FORCE_POWERS] = {
;898:	0,//FP_HEAL = 0,//instant
;899:	1,//FP_LEVITATION,//hold/duration, this one defaults to 1 (gives a free point)
;900:	0,//FP_SPEED,//duration
;901:	0,//FP_PUSH,//hold/duration
;902:	0,//FP_PULL,//hold/duration
;903:	0,//FP_TELEPATHY,//instant
;904:	0,//FP_GRIP,//hold/duration
;905:	0,//FP_LIGHTNING,//hold/duration
;906:	0,//FP_RAGE,//duration
;907:	0,//FP_PROTECT,
;908:	0,//FP_ABSORB,
;909:	0,//FP_TEAM_HEAL,
;910:	0,//FP_TEAM_FORCE,
;911:	0,//FP_DRAIN,
;912:	0,//FP_SEE,
;913:	0,//FP_SABERATTACK,
;914:	0,//FP_SABERDEFEND,
;915:	0//FP_SABERTHROW,
;916:};
;917:
;918:/*
;919:=================
;920:UI_ForceConfigHandle
;921:=================
;922:*/
;923:void UI_ForceConfigHandle( int oldindex, int newindex )
;924:{
line 926
;925:	fileHandle_t f;
;926:	int len = 0;
ADDRLP4 8276
CNSTI4 0
ASGNI4
line 927
;927:	int i = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 928
;928:	int c = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 929
;929:	int iBuf = 0, forcePowerRank, currank;
ADDRLP4 8272
CNSTI4 0
ASGNI4
line 933
;930:	char fcfBuffer[8192];
;931:	char singleBuf[64];
;932:	char info[MAX_INFO_VALUE];
;933:	int forceTeam = 0;
ADDRLP4 9308
CNSTI4 0
ASGNI4
line 935
;934:
;935:	if (oldindex == 0)
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $555
line 936
;936:	{ //switching out from custom config, so first shove the current values into the custom storage
line 937
;937:		i = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $558
JUMPV
LABELV $557
line 940
;938:
;939:		while (i < NUM_FORCE_POWERS)
;940:		{
line 941
;941:			gCustPowersRank[i] = uiForcePowersRank[i];
ADDRLP4 9312
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 9312
INDIRI4
ADDRGP4 gCustPowersRank
ADDP4
ADDRLP4 9312
INDIRI4
ADDRGP4 uiForcePowersRank
ADDP4
INDIRI4
ASGNI4
line 942
;942:			i++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 943
;943:		}
LABELV $558
line 939
ADDRLP4 8
INDIRI4
CNSTI4 18
LTI4 $557
line 944
;944:		gCustRank = uiForceRank;
ADDRGP4 gCustRank
ADDRGP4 uiForceRank
INDIRI4
ASGNI4
line 945
;945:		gCustSide = uiForceSide;
ADDRGP4 gCustSide
ADDRGP4 uiForceSide
INDIRI4
ASGNI4
line 946
;946:	}
LABELV $555
line 948
;947:
;948:	if (newindex == 0)
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $560
line 949
;949:	{ //switching back to custom, shove the values back in from the custom storage
line 950
;950:		i = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 951
;951:		uiForceUsed = 0;
ADDRGP4 uiForceUsed
CNSTI4 0
ASGNI4
line 952
;952:		gTouchedForce = qtrue;
ADDRGP4 gTouchedForce
CNSTI4 1
ASGNI4
ADDRGP4 $563
JUMPV
LABELV $562
line 955
;953:
;954:		while (i < NUM_FORCE_POWERS)
;955:		{
line 956
;956:			uiForcePowersRank[i] = gCustPowersRank[i];
ADDRLP4 9312
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 9312
INDIRI4
ADDRGP4 uiForcePowersRank
ADDP4
ADDRLP4 9312
INDIRI4
ADDRGP4 gCustPowersRank
ADDP4
INDIRI4
ASGNI4
line 957
;957:			uiForceUsed += uiForcePowersRank[i];
ADDRLP4 9316
ADDRGP4 uiForceUsed
ASGNP4
ADDRLP4 9316
INDIRP4
ADDRLP4 9316
INDIRP4
INDIRI4
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
INDIRI4
ADDI4
ASGNI4
line 958
;958:			i++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 959
;959:		}
LABELV $563
line 954
ADDRLP4 8
INDIRI4
CNSTI4 18
LTI4 $562
line 960
;960:		uiForceRank = gCustRank;
ADDRGP4 uiForceRank
ADDRGP4 gCustRank
INDIRI4
ASGNI4
line 961
;961:		uiForceSide = gCustSide;
ADDRGP4 uiForceSide
ADDRGP4 gCustSide
INDIRI4
ASGNI4
line 963
;962:
;963:		UpdateForceUsed();
ADDRGP4 UpdateForceUsed
CALLV
pop
line 964
;964:		return;
ADDRGP4 $554
JUMPV
LABELV $560
line 968
;965:	}
;966:
;967:	//If we made it here, we want to load in a new config
;968:	if (uiForceSide == FORCE_LIGHTSIDE)
ADDRGP4 uiForceSide
INDIRI4
CNSTI4 1
NEI4 $565
line 969
;969:	{ //we should only be displaying lightside configs, so.. look in the light folder
line 970
;970:		newindex += uiInfo.forceConfigLightIndexBegin;
ADDRFP4 4
ADDRFP4 4
INDIRI4
ADDRGP4 uiInfo+95056
INDIRI4
ADDI4
ASGNI4
line 971
;971:		if (newindex >= uiInfo.forceConfigCount)
ADDRFP4 4
INDIRI4
ADDRGP4 uiInfo+78148
INDIRI4
LTI4 $568
line 972
;972:		{
line 973
;973:			return;
ADDRGP4 $554
JUMPV
LABELV $568
line 975
;974:		}
;975:		len = trap_FS_FOpenFile(va("forcecfg/light/%s.fcf", uiInfo.forceConfigNames[newindex]), &f, FS_READ);
ADDRGP4 $255
ARGP4
ADDRFP4 4
INDIRI4
CNSTI4 7
LSHI4
ADDRGP4 uiInfo+78156
ADDP4
ARGP4
ADDRLP4 9312
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 9312
INDIRP4
ARGP4
ADDRLP4 9304
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 9316
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 8276
ADDRLP4 9316
INDIRI4
ASGNI4
line 976
;976:	}
ADDRGP4 $566
JUMPV
LABELV $565
line 978
;977:	else
;978:	{ //else dark
line 979
;979:		newindex += uiInfo.forceConfigDarkIndexBegin;
ADDRFP4 4
ADDRFP4 4
INDIRI4
ADDRGP4 uiInfo+95052
INDIRI4
ADDI4
ASGNI4
line 980
;980:		if (newindex >= uiInfo.forceConfigCount || newindex > uiInfo.forceConfigLightIndexBegin)
ADDRLP4 9312
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 9312
INDIRI4
ADDRGP4 uiInfo+78148
INDIRI4
GEI4 $577
ADDRLP4 9312
INDIRI4
ADDRGP4 uiInfo+95056
INDIRI4
LEI4 $573
LABELV $577
line 981
;981:		{ //dark gets read in before light
line 982
;982:			return;
ADDRGP4 $554
JUMPV
LABELV $573
line 984
;983:		}
;984:		len = trap_FS_FOpenFile(va("forcecfg/dark/%s.fcf", uiInfo.forceConfigNames[newindex]), &f, FS_READ);
ADDRGP4 $256
ARGP4
ADDRFP4 4
INDIRI4
CNSTI4 7
LSHI4
ADDRGP4 uiInfo+78156
ADDP4
ARGP4
ADDRLP4 9316
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 9316
INDIRP4
ARGP4
ADDRLP4 9304
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 9320
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 8276
ADDRLP4 9320
INDIRI4
ASGNI4
line 985
;985:	}
LABELV $566
line 987
;986:
;987:	if (len <= 0)
ADDRLP4 8276
INDIRI4
CNSTI4 0
GTI4 $579
line 988
;988:	{ //This should not have happened. But, before we quit out, attempt searching the other light/dark folder for the file.
line 989
;989:		if (uiForceSide == FORCE_LIGHTSIDE)
ADDRGP4 uiForceSide
INDIRI4
CNSTI4 1
NEI4 $581
line 990
;990:		{
line 991
;991:			len = trap_FS_FOpenFile(va("forcecfg/dark/%s.fcf", uiInfo.forceConfigNames[newindex]), &f, FS_READ);
ADDRGP4 $256
ARGP4
ADDRFP4 4
INDIRI4
CNSTI4 7
LSHI4
ADDRGP4 uiInfo+78156
ADDP4
ARGP4
ADDRLP4 9312
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 9312
INDIRP4
ARGP4
ADDRLP4 9304
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 9316
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 8276
ADDRLP4 9316
INDIRI4
ASGNI4
line 992
;992:		}
ADDRGP4 $582
JUMPV
LABELV $581
line 994
;993:		else
;994:		{
line 995
;995:			len = trap_FS_FOpenFile(va("forcecfg/light/%s.fcf", uiInfo.forceConfigNames[newindex]), &f, FS_READ);
ADDRGP4 $255
ARGP4
ADDRFP4 4
INDIRI4
CNSTI4 7
LSHI4
ADDRGP4 uiInfo+78156
ADDP4
ARGP4
ADDRLP4 9312
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 9312
INDIRP4
ARGP4
ADDRLP4 9304
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 9316
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 8276
ADDRLP4 9316
INDIRI4
ASGNI4
line 996
;996:		}
LABELV $582
line 998
;997:
;998:		if (len <= 0)
ADDRLP4 8276
INDIRI4
CNSTI4 0
GTI4 $585
line 999
;999:		{ //still failure? Oh well.
line 1000
;1000:			return;
ADDRGP4 $554
JUMPV
LABELV $585
line 1002
;1001:		}
;1002:	}
LABELV $579
line 1004
;1003:
;1004:	if (len >= 8192)
ADDRLP4 8276
INDIRI4
CNSTI4 8192
LTI4 $587
line 1005
;1005:	{
line 1006
;1006:		return;
ADDRGP4 $554
JUMPV
LABELV $587
line 1009
;1007:	}
;1008:
;1009:	trap_FS_Read(fcfBuffer, len, f);
ADDRLP4 16
ARGP4
ADDRLP4 8276
INDIRI4
ARGI4
ADDRLP4 9304
INDIRI4
ARGI4
ADDRGP4 trap_FS_Read
CALLV
pop
line 1010
;1010:	fcfBuffer[len] = 0;
ADDRLP4 8276
INDIRI4
ADDRLP4 16
ADDP4
CNSTI1 0
ASGNI1
line 1011
;1011:	trap_FS_FCloseFile(f);
ADDRLP4 9304
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 1013
;1012:
;1013:	i = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 1015
;1014:
;1015:	info[0] = '\0';
ADDRLP4 8280
CNSTI1 0
ASGNI1
line 1016
;1016:	trap_GetConfigString(CS_SERVERINFO, info, sizeof(info));
CNSTI4 0
ARGI4
ADDRLP4 8280
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigString
CALLI4
pop
line 1018
;1017:
;1018:	if (atoi( Info_ValueForKey( info, "g_forceBasedTeams" ) ))
ADDRLP4 8280
ARGP4
ADDRGP4 $355
ARGP4
ADDRLP4 9312
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 9312
INDIRP4
ARGP4
ADDRLP4 9316
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 9316
INDIRI4
CNSTI4 0
EQI4 $589
line 1019
;1019:	{
line 1020
;1020:		switch((int)(trap_Cvar_VariableValue("ui_myteam")))
ADDRGP4 $358
ARGP4
ADDRLP4 9324
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 9320
ADDRLP4 9324
INDIRF4
CVFI4 4
ASGNI4
ADDRLP4 9320
INDIRI4
CNSTI4 1
EQI4 $594
ADDRLP4 9320
INDIRI4
CNSTI4 2
EQI4 $595
ADDRGP4 $592
JUMPV
line 1021
;1021:		{
LABELV $594
line 1023
;1022:		case TEAM_RED:
;1023:			forceTeam = FORCE_DARKSIDE;
ADDRLP4 9308
CNSTI4 2
ASGNI4
line 1024
;1024:			break;
ADDRGP4 $592
JUMPV
LABELV $595
line 1026
;1025:		case TEAM_BLUE:
;1026:			forceTeam = FORCE_LIGHTSIDE;
ADDRLP4 9308
CNSTI4 1
ASGNI4
line 1027
;1027:			break;
line 1029
;1028:		default:
;1029:			break;
LABELV $592
line 1031
;1030:		}
;1031:	}
LABELV $589
line 1033
;1032:
;1033:	BG_LegalizedForcePowers(fcfBuffer, uiMaxRank, ui_freeSaber.integer, forceTeam, atoi( Info_ValueForKey( info, "g_gametype" )), 0);
ADDRLP4 8280
ARGP4
ADDRGP4 $365
ARGP4
ADDRLP4 9320
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 9320
INDIRP4
ARGP4
ADDRLP4 9324
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 16
ARGP4
ADDRGP4 uiMaxRank
INDIRI4
ARGI4
ADDRGP4 ui_freeSaber+12
INDIRI4
ARGI4
ADDRLP4 9308
INDIRI4
ARGI4
ADDRLP4 9324
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 BG_LegalizedForcePowers
CALLI4
pop
ADDRGP4 $598
JUMPV
LABELV $597
line 1039
;1034:	//legalize the config based on the max rank
;1035:
;1036:	//now that we're done with the handle, it's time to parse our force data out of the string
;1037:	//we store strings in rank-side-xxxxxxxxx format (where the x's are individual force power levels)
;1038:	while (fcfBuffer[i] && fcfBuffer[i] != '-')
;1039:	{
line 1040
;1040:		singleBuf[c] = fcfBuffer[i];
ADDRLP4 0
INDIRI4
ADDRLP4 8208
ADDP4
ADDRLP4 8
INDIRI4
ADDRLP4 16
ADDP4
INDIRI1
ASGNI1
line 1041
;1041:		c++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1042
;1042:		i++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1043
;1043:	}
LABELV $598
line 1038
ADDRLP4 9328
ADDRLP4 8
INDIRI4
ADDRLP4 16
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 9328
INDIRI4
CNSTI4 0
EQI4 $600
ADDRLP4 9328
INDIRI4
CNSTI4 45
NEI4 $597
LABELV $600
line 1044
;1044:	singleBuf[c] = 0;
ADDRLP4 0
INDIRI4
ADDRLP4 8208
ADDP4
CNSTI1 0
ASGNI1
line 1045
;1045:	c = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1046
;1046:	i++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1048
;1047:
;1048:	iBuf = atoi(singleBuf);
ADDRLP4 8208
ARGP4
ADDRLP4 9332
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 8272
ADDRLP4 9332
INDIRI4
ASGNI4
line 1050
;1049:
;1050:	if (iBuf > uiMaxRank || iBuf < 0)
ADDRLP4 8272
INDIRI4
ADDRGP4 uiMaxRank
INDIRI4
GTI4 $603
ADDRLP4 8272
INDIRI4
CNSTI4 0
GEI4 $601
LABELV $603
line 1051
;1051:	{ //this force config uses a rank level higher than our currently restricted level.. so we can't use it
line 1053
;1052:	  //FIXME: Print a message indicating this to the user
;1053:		return;
ADDRGP4 $554
JUMPV
LABELV $601
line 1056
;1054:	}
;1055:
;1056:	uiForceRank = iBuf;
ADDRGP4 uiForceRank
ADDRLP4 8272
INDIRI4
ASGNI4
ADDRGP4 $605
JUMPV
LABELV $604
line 1059
;1057:
;1058:	while (fcfBuffer[i] && fcfBuffer[i] != '-')
;1059:	{
line 1060
;1060:		singleBuf[c] = fcfBuffer[i];
ADDRLP4 0
INDIRI4
ADDRLP4 8208
ADDP4
ADDRLP4 8
INDIRI4
ADDRLP4 16
ADDP4
INDIRI1
ASGNI1
line 1061
;1061:		c++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1062
;1062:		i++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1063
;1063:	}
LABELV $605
line 1058
ADDRLP4 9340
ADDRLP4 8
INDIRI4
ADDRLP4 16
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 9340
INDIRI4
CNSTI4 0
EQI4 $607
ADDRLP4 9340
INDIRI4
CNSTI4 45
NEI4 $604
LABELV $607
line 1064
;1064:	singleBuf[c] = 0;
ADDRLP4 0
INDIRI4
ADDRLP4 8208
ADDP4
CNSTI1 0
ASGNI1
line 1065
;1065:	c = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1066
;1066:	i++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1068
;1067:
;1068:	uiForceSide = atoi(singleBuf);
ADDRLP4 8208
ARGP4
ADDRLP4 9344
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRGP4 uiForceSide
ADDRLP4 9344
INDIRI4
ASGNI4
line 1070
;1069:
;1070:	if (uiForceSide != FORCE_LIGHTSIDE &&
ADDRLP4 9348
ADDRGP4 uiForceSide
INDIRI4
ASGNI4
ADDRLP4 9348
INDIRI4
CNSTI4 1
EQI4 $611
ADDRLP4 9348
INDIRI4
CNSTI4 2
EQI4 $611
line 1072
;1071:		uiForceSide != FORCE_DARKSIDE)
;1072:	{
line 1073
;1073:		uiForceSide = FORCE_LIGHTSIDE;
ADDRGP4 uiForceSide
CNSTI4 1
ASGNI4
line 1074
;1074:		return;
ADDRGP4 $554
JUMPV
LABELV $610
line 1079
;1075:	}
;1076:
;1077:	//clear out the existing powers
;1078:	while (c < NUM_FORCE_POWERS)
;1079:	{
line 1099
;1080:		/*
;1081:		if (c==FP_LEVITATION)
;1082:		{
;1083:			uiForcePowersRank[c]=1;
;1084:		}
;1085:		else if (c==FP_SABERATTACK && ui_freeSaber.integer)
;1086:		{
;1087:			uiForcePowersRank[c]=1;
;1088:		}
;1089:		else if (c==FP_SABERDEFEND && ui_freeSaber.integer)
;1090:		{
;1091:			uiForcePowersRank[c]=1;
;1092:		}
;1093:		else
;1094:		{
;1095:			uiForcePowersRank[c] = 0;
;1096:		}
;1097:		*/
;1098:		//rww - don't need to do these checks. Just trust whatever the saber config says.
;1099:		uiForcePowersRank[c] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
CNSTI4 0
ASGNI4
line 1100
;1100:		c++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1101
;1101:	}
LABELV $611
line 1078
ADDRLP4 0
INDIRI4
CNSTI4 18
LTI4 $610
line 1102
;1102:	uiForceUsed = 0;
ADDRGP4 uiForceUsed
CNSTI4 0
ASGNI4
line 1103
;1103:	uiForceAvailable = forceMasteryPoints[uiForceRank];
ADDRGP4 uiForceAvailable
ADDRGP4 uiForceRank
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 forceMasteryPoints
ADDP4
INDIRI4
ASGNI4
line 1104
;1104:	gTouchedForce = qtrue;
ADDRGP4 gTouchedForce
CNSTI4 1
ASGNI4
line 1106
;1105:
;1106:	for (c=0;fcfBuffer[i]&&c<NUM_FORCE_POWERS;c++,i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $616
JUMPV
LABELV $613
line 1107
;1107:	{
line 1108
;1108:		singleBuf[0] = fcfBuffer[i];
ADDRLP4 8208
ADDRLP4 8
INDIRI4
ADDRLP4 16
ADDP4
INDIRI1
ASGNI1
line 1109
;1109:		singleBuf[1] = 0;
ADDRLP4 8208+1
CNSTI1 0
ASGNI1
line 1110
;1110:		iBuf = atoi(singleBuf);	// So, that means that Force Power "c" wants to be set to rank "iBuf".
ADDRLP4 8208
ARGP4
ADDRLP4 9352
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 8272
ADDRLP4 9352
INDIRI4
ASGNI4
line 1112
;1111:		
;1112:		if (iBuf < 0)
ADDRLP4 8272
INDIRI4
CNSTI4 0
GEI4 $618
line 1113
;1113:		{
line 1114
;1114:			iBuf = 0;
ADDRLP4 8272
CNSTI4 0
ASGNI4
line 1115
;1115:		}
LABELV $618
line 1117
;1116:
;1117:		forcePowerRank = iBuf;
ADDRLP4 12
ADDRLP4 8272
INDIRI4
ASGNI4
line 1119
;1118:
;1119:		if (forcePowerRank > FORCE_LEVEL_3 || forcePowerRank < 0)
ADDRLP4 12
INDIRI4
CNSTI4 3
GTI4 $622
ADDRLP4 12
INDIRI4
CNSTI4 0
GEI4 $620
LABELV $622
line 1120
;1120:		{ //err..  not correct
line 1121
;1121:			continue;  // skip this power
ADDRGP4 $614
JUMPV
LABELV $620
line 1124
;1122:		}
;1123:
;1124:		if (uiForcePowerDarkLight[c] && uiForcePowerDarkLight[c] != uiForceSide)
ADDRLP4 9360
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowerDarkLight
ADDP4
INDIRI4
ASGNI4
ADDRLP4 9360
INDIRI4
CNSTI4 0
EQI4 $623
ADDRLP4 9360
INDIRI4
ADDRGP4 uiForceSide
INDIRI4
EQI4 $623
line 1125
;1125:		{ //Apparently the user has crafted a force config that has powers that don't fit with the config's side.
line 1126
;1126:			continue;  // skip this power
ADDRGP4 $614
JUMPV
LABELV $623
line 1130
;1127:		}
;1128:
;1129:		// Accrue cost for each assigned rank for this power.
;1130:		for (currank=FORCE_LEVEL_1;currank<=forcePowerRank;currank++)
ADDRLP4 4
CNSTI4 1
ASGNI4
ADDRGP4 $628
JUMPV
LABELV $625
line 1131
;1131:		{	
line 1132
;1132:			if (bgForcePowerCost[c][currank] > uiForceAvailable)
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
ADDRGP4 uiForceAvailable
INDIRI4
LEI4 $629
line 1133
;1133:			{	// Break out, we can't afford any more power.
line 1134
;1134:				break;
ADDRGP4 $627
JUMPV
LABELV $629
line 1137
;1135:			}
;1136:			// Pay for this rank of this power.
;1137:			uiForceUsed += bgForcePowerCost[c][currank];
ADDRLP4 9364
ADDRGP4 uiForceUsed
ASGNP4
ADDRLP4 9364
INDIRP4
ADDRLP4 9364
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1138
;1138:			uiForceAvailable -= bgForcePowerCost[c][currank];
ADDRLP4 9368
ADDRGP4 uiForceAvailable
ASGNP4
ADDRLP4 9368
INDIRP4
ADDRLP4 9368
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
SUBI4
ASGNI4
line 1140
;1139:
;1140:			uiForcePowersRank[c]++;
ADDRLP4 9372
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 uiForcePowersRank
ADDP4
ASGNP4
ADDRLP4 9372
INDIRP4
ADDRLP4 9372
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1141
;1141:		}
LABELV $626
line 1130
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $628
ADDRLP4 4
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $625
LABELV $627
line 1142
;1142:	}
LABELV $614
line 1106
ADDRLP4 9352
CNSTI4 1
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRLP4 9352
INDIRI4
ADDI4
ASGNI4
ADDRLP4 8
ADDRLP4 8
INDIRI4
ADDRLP4 9352
INDIRI4
ADDI4
ASGNI4
LABELV $616
ADDRLP4 8
INDIRI4
ADDRLP4 16
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $631
ADDRLP4 0
INDIRI4
CNSTI4 18
LTI4 $613
LABELV $631
line 1144
;1143:
;1144:	if (uiForcePowersRank[FP_LEVITATION] < 1)
ADDRGP4 uiForcePowersRank+4
INDIRI4
CNSTI4 1
GEI4 $632
line 1145
;1145:	{
line 1146
;1146:		uiForcePowersRank[FP_LEVITATION]=1;
ADDRGP4 uiForcePowersRank+4
CNSTI4 1
ASGNI4
line 1147
;1147:	}
LABELV $632
line 1148
;1148:	if (uiForcePowersRank[FP_SABERATTACK] < 1 && ui_freeSaber.integer)
ADDRGP4 uiForcePowersRank+60
INDIRI4
CNSTI4 1
GEI4 $636
ADDRGP4 ui_freeSaber+12
INDIRI4
CNSTI4 0
EQI4 $636
line 1149
;1149:	{
line 1150
;1150:		uiForcePowersRank[FP_SABERATTACK]=1;
ADDRGP4 uiForcePowersRank+60
CNSTI4 1
ASGNI4
line 1151
;1151:	}
LABELV $636
line 1152
;1152:	if (uiForcePowersRank[FP_SABERDEFEND] < 1 && ui_freeSaber.integer)
ADDRGP4 uiForcePowersRank+64
INDIRI4
CNSTI4 1
GEI4 $641
ADDRGP4 ui_freeSaber+12
INDIRI4
CNSTI4 0
EQI4 $641
line 1153
;1153:	{
line 1154
;1154:		uiForcePowersRank[FP_SABERDEFEND]=1;
ADDRGP4 uiForcePowersRank+64
CNSTI4 1
ASGNI4
line 1155
;1155:	}
LABELV $641
line 1157
;1156:
;1157:	UpdateForceUsed();
ADDRGP4 UpdateForceUsed
CALLV
pop
line 1158
;1158:}
LABELV $554
endproc UI_ForceConfigHandle 9376 24
import uiSkinColor
bss
export uiForceStarShaders
align 4
LABELV uiForceStarShaders
skip 72
import Menu_ShowItemByName
import UI_DrawTotalForceStars
export ui_forcePowerDisable
align 4
LABELV ui_forcePowerDisable
skip 272
export ui_freeSaber
align 4
LABELV ui_freeSaber
skip 272
export uiSaberColorShaders
align 4
LABELV uiSaberColorShaders
skip 24
import UI_RankStatusMenu
import RankStatus_Cache
import UI_SignupMenu
import Signup_Cache
import UI_LoginMenu
import Login_Cache
import UI_InitGameinfo
import UI_SPUnlockMedals_f
import UI_SPUnlock_f
import UI_GetAwardLevel
import UI_LogAwardData
import UI_NewGame
import UI_GetCurrentGame
import UI_CanShowTierVideo
import UI_ShowTierVideo
import UI_TierCompleted
import UI_SetBestScore
import UI_GetBestScore
import UI_GetBotNameByNumber
import UI_LoadBots
import UI_GetNumBots
import UI_GetBotInfoByName
import UI_GetBotInfoByNumber
import UI_GetNumSPTiers
import UI_GetNumSPArenas
import UI_GetNumArenas
import UI_GetSpecialArenaInfo
import UI_GetArenaInfoByMap
import UI_GetArenaInfoByNumber
import UI_NetworkOptionsMenu
import UI_NetworkOptionsMenu_Cache
import UI_SoundOptionsMenu
import UI_SoundOptionsMenu_Cache
import UI_DisplayOptionsMenu
import UI_DisplayOptionsMenu_Cache
import UI_SaveConfigMenu
import UI_SaveConfigMenu_Cache
import UI_LoadConfigMenu
import UI_LoadConfig_Cache
import UI_TeamOrdersMenu_Cache
import UI_TeamOrdersMenu_f
import UI_TeamOrdersMenu
import UI_RemoveBotsMenu
import UI_RemoveBots_Cache
import UI_AddBotsMenu
import UI_AddBots_Cache
import trap_G2API_SetBoneAngles
import trap_R_RemapShader
import trap_RealTime
import trap_CIN_SetExtents
import trap_CIN_DrawCinematic
import trap_CIN_RunCinematic
import trap_CIN_StopCinematic
import trap_CIN_PlayCinematic
import trap_S_StartBackgroundTrack
import trap_S_StopBackgroundTrack
import trap_AnyLanguage_ReadCharFromString
import trap_R_Font_DrawString
import trap_R_Font_HeightPixels
import trap_R_Font_StrLenChars
import trap_R_Font_StrLenPixels
import trap_R_RegisterFont
import trap_MemoryRemaining
import trap_LAN_CompareServers
import trap_LAN_ServerStatus
import trap_LAN_ResetPings
import trap_LAN_RemoveServer
import trap_LAN_AddServer
import trap_LAN_UpdateVisiblePings
import trap_LAN_ServerIsVisible
import trap_LAN_MarkServerVisible
import trap_LAN_SaveCachedServers
import trap_LAN_LoadCachedServers
import trap_LAN_GetPingInfo
import trap_LAN_GetPing
import trap_LAN_ClearPing
import trap_LAN_GetPingQueueCount
import trap_LAN_GetServerPing
import trap_LAN_GetServerInfo
import trap_LAN_GetServerAddressString
import trap_LAN_GetServerCount
import trap_GetConfigString
import trap_GetGlconfig
import trap_GetClientState
import trap_GetClipboardData
import trap_Key_SetCatcher
import trap_Key_GetCatcher
import trap_Key_ClearStates
import trap_Key_SetOverstrikeMode
import trap_Key_GetOverstrikeMode
import trap_Key_IsDown
import trap_Key_SetBinding
import trap_Key_GetBindingBuf
import trap_Key_KeynumToStringBuf
import trap_S_RegisterSound
import trap_S_StartLocalSound
import trap_CM_LerpTag
import trap_UpdateScreen
import trap_R_ModelBounds
import trap_R_DrawStretchPic
import trap_R_SetColor
import trap_R_RenderScene
import trap_R_AddLightToScene
import trap_R_AddPolyToScene
import trap_R_AddRefEntityToScene
import trap_R_ClearScene
import trap_R_RegisterShaderNoMip
import trap_R_RegisterSkin
import trap_R_RegisterModel
import trap_FS_GetFileList
import trap_FS_FCloseFile
import trap_FS_Write
import trap_FS_Read
import trap_FS_FOpenFile
import trap_Cmd_ExecuteText
import trap_Argv
import trap_Argc
import trap_Cvar_InfoStringBuffer
import trap_Cvar_Create
import trap_Cvar_Reset
import trap_Cvar_SetValue
import trap_Cvar_VariableStringBuffer
import trap_Cvar_VariableValue
import trap_Cvar_Set
import trap_Cvar_Update
import trap_Cvar_Register
import trap_Milliseconds
import trap_Error
import trap_Print
import UI_SPSkillMenu_Cache
import UI_SPSkillMenu
import UI_SPPostgameMenu_f
import UI_SPPostgameMenu_Cache
import UI_SPArena_Start
import UI_SPLevelMenu_ReInit
import UI_SPLevelMenu_f
import UI_SPLevelMenu
import UI_SPLevelMenu_Cache
import uis
import UI_LoadBestScores
import m_entersound
import UI_StartDemoLoop
import UI_Cvar_VariableString
import UI_Argv
import UI_ForceMenuOff
import UI_PopMenu
import UI_PushMenu
import UI_SetActiveMenu
import UI_IsFullscreen
import UI_DrawTextBox
import UI_CursorInRect
import UI_DrawChar
import UI_DrawString
import UI_ProportionalStringWidth
import UI_DrawProportionalString
import UI_ProportionalSizeScale
import UI_DrawBannerString
import UI_LerpColor
import UI_SetColor
import UI_UpdateScreen
import UI_DrawSides
import UI_DrawTopBottom
import UI_DrawRect
import UI_FillRect
import UI_DrawHandlePic
import UI_DrawNamedPic
import UI_ClampCvar
import UI_ConsoleCommand
import UI_Refresh
import UI_MouseEvent
import UI_KeyEvent
import UI_Shutdown
import UI_Init
import uiInfo
import DriverInfo_Cache
import GraphicsOptions_Cache
import UI_GraphicsOptionsMenu
import ServerInfo_Cache
import UI_ServerInfoMenu
import UI_BotSelectMenu_Cache
import UI_BotSelectMenu
import ServerOptions_Cache
import StartServer_Cache
import UI_StartServerMenu
import ArenaServers_Cache
import UI_ArenaServersMenu
import SpecifyServer_Cache
import UI_SpecifyServerMenu
import SpecifyLeague_Cache
import UI_SpecifyLeagueMenu
import Preferences_Cache
import UI_PreferencesMenu
import PlayerSettings_Cache
import UI_PlayerSettingsMenu
import PlayerModel_Cache
import UI_PlayerModelMenu
import UI_CDKeyMenu_f
import UI_CDKeyMenu_Cache
import UI_CDKeyMenu
import UI_ModsMenu_Cache
import UI_ModsMenu
import UI_CinematicsMenu_Cache
import UI_CinematicsMenu_f
import UI_CinematicsMenu
import Demos_Cache
import UI_DemosMenu
import Controls_Cache
import UI_ControlsMenu
import UI_DrawConnectScreen
import TeamMain_Cache
import UI_TeamMainMenu
import UI_SetupMenu
import UI_SetupMenu_Cache
import UI_ConfirmMenu
import ConfirmMenu_Cache
import UI_InGameMenu
import InGame_Cache
import UI_CreditMenu
import UI_UpdateCvars
import UI_RegisterCvars
import UI_MainMenu
import MainMenu_Cache
import UI_LoadForceConfig_List
import UI_LoadArenas
import UI_ClearScores
import UI_ShowPostGame
import UI_AdjustTimeByGame
import _UI_SetActiveMenu
import UI_LoadMenus
import UI_Load
import UI_Report
import UI_FeederSelection
import MenuField_Key
import MenuField_Draw
import MenuField_Init
import MField_Draw
import MField_CharEvent
import MField_KeyDownEvent
import MField_Clear
import ui_medalSounds
import ui_medalPicNames
import ui_medalNames
import text_color_highlight
import text_color_normal
import text_color_disabled
import listbar_color
import list_color
import name_color
import color_dim
import color_red
import color_orange
import color_blue
import color_yellow
import color_white
import color_black
import menu_dim_color
import menu_black_color
import menu_red_color
import menu_highlight_color
import menu_dark_color
import menu_grayed_color
import menu_text_color
import weaponChangeSound
import menu_null_sound
import menu_buzz_sound
import menu_out_sound
import menu_move_sound
import menu_in_sound
import ScrollList_Key
import ScrollList_Draw
import Bitmap_Draw
import Bitmap_Init
import Menu_DefaultKey
import Menu_SetCursorToItem
import Menu_SetCursor
import Menu_ActivateItem
import Menu_ItemAtCursor
import Menu_Draw
import Menu_AdjustCursor
import Menu_AddItem
import Menu_Focus
import Menu_Cache
import ui_serverStatusTimeOut
import ui_bigFont
import ui_smallFont
import ui_scoreTime
import ui_scoreShutoutBonus
import ui_scoreSkillBonus
import ui_scoreTimeBonus
import ui_scoreBase
import ui_scoreTeam
import ui_scorePerfect
import ui_scoreScore
import ui_scoreGauntlets
import ui_scoreAssists
import ui_scoreDefends
import ui_scoreExcellents
import ui_scoreImpressives
import ui_scoreAccuracy
import ui_singlePlayerActive
import ui_lastServerRefresh_3
import ui_lastServerRefresh_2
import ui_lastServerRefresh_1
import ui_lastServerRefresh_0
import ui_selectedPlayerName
import ui_selectedPlayer
import ui_currentOpponent
import ui_mapIndex
import ui_currentNetMap
import ui_currentMap
import ui_currentTier
import ui_menuFiles
import ui_opponentName
import ui_dedicated
import ui_serverFilterType
import ui_netSource
import ui_joinGameType
import ui_actualNetGameType
import ui_netGameType
import ui_gameType
import ui_fragLimit
import ui_captureLimit
import ui_cdkeychecked
import ui_cdkey
import ui_server16
import ui_server15
import ui_server14
import ui_server13
import ui_server12
import ui_server11
import ui_server10
import ui_server9
import ui_server8
import ui_server7
import ui_server6
import ui_server5
import ui_server4
import ui_server3
import ui_server2
import ui_server1
import ui_marks
import ui_drawCrosshairNames
import ui_drawCrosshair
import ui_browserShowEmpty
import ui_browserShowFull
import ui_browserSortKey
import ui_browserGameType
import ui_browserMaster
import ui_spSelection
import ui_spSkill
import ui_spVideos
import ui_spAwards
import ui_spScores5
import ui_spScores4
import ui_spScores3
import ui_spScores2
import ui_spScores1
import ui_botsFile
import ui_arenasFile
import ui_ctf_friendly
import ui_ctf_timelimit
import ui_ctf_capturelimit
import ui_team_friendly
import ui_team_timelimit
import ui_team_fraglimit
import ui_selectedModelIndex
import ui_tourney_timelimit
import ui_tourney_fraglimit
import ui_ffa_timelimit
import ui_ffa_fraglimit
import trap_SP_GetStringTextString
import trap_SP_Register
import trap_SP_RegisterServer
import trap_PC_RemoveAllGlobalDefines
import trap_PC_LoadGlobalDefines
import trap_PC_SourceFileAndLine
import trap_PC_ReadToken
import trap_PC_FreeSource
import trap_PC_LoadSource
import trap_PC_AddGlobalDefine
import Controls_SetDefaults
import Controls_SetConfig
import Controls_GetConfig
import UI_OutOfMemory
import UI_InitMemory
import UI_Alloc
import Display_CacheAll
import Menu_SetFeederSelection
import Menu_Paint
import Menus_CloseAll
import LerpColor
import Display_HandleKey
import Menus_CloseByName
import Menus_ShowByName
import Menus_FindByName
import Menus_OpenByName
import Display_KeyBindPending
import Display_CursorType
import Display_MouseMove
import Display_CaptureItem
import Display_GetContext
import Menus_Activate
import Menus_AnyFullScreenVisible
import Menu_Reset
import Menus_ActivateByName
import Menu_PaintAll
import Menu_New
import Menu_Count
import PC_Script_Parse
import PC_String_Parse
import PC_Rect_Parse
import PC_Int_Parse
import PC_Color_Parse
import PC_Float_Parse
import Script_Parse
import String_Parse
import Rect_Parse
import Int_Parse
import Color_Parse
import Float_Parse
import Menu_ScrollFeeder
import Menu_HandleMouseMove
import Menu_HandleKey
import Menu_GetFocused
import Menu_PostParse
import Item_Init
import Menu_Init
import Display_ExpandMacros
import Init_Display
import String_Report
import String_Init
import String_Alloc
import forcePowerDarkLight
import WeaponAttackAnim
import WeaponReadyAnim
import BG_OutOfMemory
import BG_StringAlloc
import BG_TempFree
import BG_TempAlloc
import BG_AllocUnaligned
import BG_Alloc
import BG_CanUseFPNow
import BG_HasYsalamiri
import BG_GetItemIndexByTag
import BG_ParseAnimationFile
import BG_PlayerTouchesItem
import BG_G2PlayerAngles
import BG_PlayerStateToEntityStateExtraPolate
import BG_PlayerStateToEntityState
import BG_TouchJumpPad
import BG_AddPredictableEventToPlayerstate
import BG_EvaluateTrajectoryDelta
import BG_EvaluateTrajectory
import BG_ForcePowerDrain
import BG_SaberStartTransAnim
import BG_InDeathAnim
import BG_InRoll
import BG_SaberInSpecialAttack
import BG_SpinningSaberAnim
import BG_FlippingAnim
import BG_SaberInIdle
import BG_SaberInSpecial
import BG_SaberInAttack
import BG_DirectFlippingAnim
import BG_InSaberStandAnim
import BG_InSpecialJump
import BG_LegalizedForcePowers
import saberMoveData
import BG_CanItemBeGrabbed
import BG_FindItemForHoldable
import BG_FindItemForPowerup
import BG_FindItemForWeapon
import BG_FindItem
import vectoyaw
import bg_numItems
import bg_itemlist
import Pmove
import PM_UpdateViewAngles
import pm
import bgForcePowerCost
import forceMasteryPoints
import forceMasteryLevels
import bgGlobalAnimations
import BGPAFtextLoaded
import forcePowerSorted
import WP_MuzzlePoint
import ammoData
import weaponData
import GetStringForID
import GetIDForString
import Q_irand
import irand
import flrand
import Rand_Init
import Com_Printf
import Com_Error
import Info_NextPair
import Info_Validate
import Info_SetValueForKey_Big
import Info_SetValueForKey
import Info_RemoveKey_big
import Info_RemoveKey
import Info_ValueForKey
import va
import Q_CleanStr
import Q_PrintStrlen
import Q_strcat
import Q_strncpyz
import Q_strrchr
import Q_strupr
import Q_strlwr
import Q_stricmpn
import Q_strncmp
import Q_stricmp
import Q_isalpha
import Q_isupper
import Q_islower
import Q_isprint
import Com_sprintf
import Parse3DMatrix
import Parse2DMatrix
import Parse1DMatrix
import SkipRestOfLine
import SkipBracedSection
import COM_MatchToken
import COM_ParseWarning
import COM_ParseError
import COM_Compress
import COM_ParseExt
import COM_Parse
import SkipWhitespace
import COM_GetCurrentParseLine
import COM_BeginParseSession
import COM_DefaultExtension
import COM_StripExtension
import COM_SkipPath
import Com_Clamp
import PerpendicularVector
import AngleVectors
import MatrixMultiply
import MakeNormalVectors
import RotateAroundDirection
import RotatePointAroundVector
import ProjectPointOnPlane
import PlaneFromPoints
import AngleDelta
import AngleNormalize180
import AngleNormalize360
import AnglesSubtract
import AngleSubtract
import LerpAngle
import AngleMod
import BoxOnPlaneSide
import SetPlaneSignbits
import AxisCopy
import AxisClear
import AnglesToAxis
import vectoangles
import Q_crandom
import Q_random
import Q_rand
import Q_acos
import Q_log2
import VectorRotate
import Vector4Scale
import VectorNormalize2
import VectorNormalize
import CrossProduct
import VectorInverse
import VectorNormalizeFast
import DistanceSquared
import Distance
import VectorLengthSquared
import VectorLength
import VectorCompare
import AddPointToBounds
import ClearBounds
import RadiusFromBounds
import NormalizeColor
import ColorBytes4
import ColorBytes3
import _VectorMA
import _VectorScale
import _VectorCopy
import _VectorAdd
import _VectorSubtract
import _DotProduct
import ByteToDir
import DirToByte
import powf
import ClampShort
import ClampChar
import Q_rsqrt
import Q_fabs
import axisDefault
import vec3_origin
import g_color_table
import colorDkBlue
import colorLtBlue
import colorDkGrey
import colorMdGrey
import colorLtGrey
import colorWhite
import colorCyan
import colorMagenta
import colorYellow
import colorBlue
import colorGreen
import colorRed
import colorBlack
import colorTable
import bytedirs
import Com_Memcpy
import Com_Memset
import Hunk_Alloc
import forceSpeedLevels
import FloatSwap
import LongSwap
import ShortSwap
import acos
import fabs
import abs
import tan
import atan2
import cos
import sin
import sqrt
import floor
import ceil
import memcpy
import memset
import memmove
import sscanf
import vsprintf
import _atoi
import atoi
import _atof
import atof
import toupper
import tolower
import strncpy
import strstr
import strchr
import strcmp
import strcpy
import strcat
import strlen
import rand
import srand
import qsort
lit
align 1
LABELV $521
char 1 103
char 1 95
char 1 109
char 1 97
char 1 120
char 1 70
char 1 111
char 1 114
char 1 99
char 1 101
char 1 82
char 1 97
char 1 110
char 1 107
char 1 0
align 1
LABELV $365
char 1 103
char 1 95
char 1 103
char 1 97
char 1 109
char 1 101
char 1 116
char 1 121
char 1 112
char 1 101
char 1 0
align 1
LABELV $358
char 1 117
char 1 105
char 1 95
char 1 109
char 1 121
char 1 116
char 1 101
char 1 97
char 1 109
char 1 0
align 1
LABELV $355
char 1 103
char 1 95
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 66
char 1 97
char 1 115
char 1 101
char 1 100
char 1 84
char 1 101
char 1 97
char 1 109
char 1 115
char 1 0
align 1
LABELV $309
char 1 110
char 1 111
char 1 115
char 1 97
char 1 98
char 1 101
char 1 114
char 1 0
align 1
LABELV $308
char 1 101
char 1 102
char 1 102
char 1 101
char 1 99
char 1 116
char 1 102
char 1 105
char 1 101
char 1 108
char 1 100
char 1 0
align 1
LABELV $307
char 1 101
char 1 102
char 1 102
char 1 101
char 1 99
char 1 116
char 1 101
char 1 110
char 1 116
char 1 114
char 1 121
char 1 0
align 1
LABELV $306
char 1 115
char 1 101
char 1 116
char 1 102
char 1 112
char 1 95
char 1 115
char 1 97
char 1 98
char 1 101
char 1 114
char 1 116
char 1 104
char 1 114
char 1 111
char 1 119
char 1 0
align 1
LABELV $305
char 1 115
char 1 101
char 1 116
char 1 102
char 1 112
char 1 95
char 1 115
char 1 97
char 1 98
char 1 101
char 1 114
char 1 100
char 1 101
char 1 102
char 1 101
char 1 110
char 1 100
char 1 0
align 1
LABELV $287
char 1 105
char 1 110
char 1 103
char 1 97
char 1 109
char 1 101
char 1 95
char 1 112
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 0
align 1
LABELV $266
char 1 84
char 1 101
char 1 109
char 1 112
char 1 108
char 1 97
char 1 116
char 1 101
char 1 32
char 1 115
char 1 97
char 1 118
char 1 101
char 1 100
char 1 32
char 1 97
char 1 115
char 1 32
char 1 34
char 1 37
char 1 115
char 1 34
char 1 46
char 1 10
char 1 0
align 1
LABELV $264
char 1 37
char 1 105
char 1 0
align 1
LABELV $260
char 1 37
char 1 105
char 1 45
char 1 37
char 1 105
char 1 45
char 1 0
align 1
LABELV $259
char 1 84
char 1 104
char 1 101
char 1 114
char 1 101
char 1 32
char 1 119
char 1 97
char 1 115
char 1 32
char 1 97
char 1 110
char 1 32
char 1 101
char 1 114
char 1 114
char 1 111
char 1 114
char 1 32
char 1 119
char 1 114
char 1 105
char 1 116
char 1 105
char 1 110
char 1 103
char 1 32
char 1 116
char 1 104
char 1 101
char 1 32
char 1 116
char 1 101
char 1 109
char 1 112
char 1 108
char 1 97
char 1 116
char 1 101
char 1 32
char 1 102
char 1 105
char 1 108
char 1 101
char 1 32
char 1 40
char 1 114
char 1 101
char 1 97
char 1 100
char 1 45
char 1 111
char 1 110
char 1 108
char 1 121
char 1 63
char 1 41
char 1 46
char 1 10
char 1 0
align 1
LABELV $256
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 99
char 1 102
char 1 103
char 1 47
char 1 100
char 1 97
char 1 114
char 1 107
char 1 47
char 1 37
char 1 115
char 1 46
char 1 102
char 1 99
char 1 102
char 1 0
align 1
LABELV $255
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 99
char 1 102
char 1 103
char 1 47
char 1 108
char 1 105
char 1 103
char 1 104
char 1 116
char 1 47
char 1 37
char 1 115
char 1 46
char 1 102
char 1 99
char 1 102
char 1 0
align 1
LABELV $252
char 1 89
char 1 111
char 1 117
char 1 32
char 1 100
char 1 105
char 1 100
char 1 32
char 1 110
char 1 111
char 1 116
char 1 32
char 1 112
char 1 114
char 1 111
char 1 118
char 1 105
char 1 100
char 1 101
char 1 32
char 1 97
char 1 32
char 1 110
char 1 97
char 1 109
char 1 101
char 1 32
char 1 102
char 1 111
char 1 114
char 1 32
char 1 116
char 1 104
char 1 101
char 1 32
char 1 116
char 1 101
char 1 109
char 1 112
char 1 108
char 1 97
char 1 116
char 1 101
char 1 46
char 1 10
char 1 0
align 1
LABELV $248
char 1 117
char 1 105
char 1 95
char 1 83
char 1 97
char 1 118
char 1 101
char 1 70
char 1 67
char 1 70
char 1 0
align 1
LABELV $241
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 99
char 1 104
char 1 97
char 1 110
char 1 103
char 1 101
char 1 100
char 1 10
char 1 0
align 1
LABELV $240
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 99
char 1 104
char 1 97
char 1 110
char 1 103
char 1 101
char 1 100
char 1 32
char 1 34
char 1 37
char 1 115
char 1 34
char 1 10
char 1 0
align 1
LABELV $218
char 1 37
char 1 105
char 1 45
char 1 37
char 1 105
char 1 45
char 1 37
char 1 105
char 1 37
char 1 105
char 1 37
char 1 105
char 1 37
char 1 105
char 1 37
char 1 105
char 1 37
char 1 105
char 1 37
char 1 105
char 1 37
char 1 105
char 1 37
char 1 105
char 1 37
char 1 105
char 1 37
char 1 105
char 1 37
char 1 105
char 1 37
char 1 105
char 1 37
char 1 105
char 1 37
char 1 105
char 1 37
char 1 105
char 1 37
char 1 105
char 1 37
char 1 105
char 1 0
align 1
LABELV $217
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 112
char 1 111
char 1 119
char 1 101
char 1 114
char 1 115
char 1 0
align 1
LABELV $202
char 1 109
char 1 101
char 1 110
char 1 117
char 1 47
char 1 97
char 1 114
char 1 116
char 1 47
char 1 115
char 1 97
char 1 98
char 1 101
char 1 114
char 1 95
char 1 112
char 1 117
char 1 114
char 1 112
char 1 108
char 1 101
char 1 0
align 1
LABELV $200
char 1 109
char 1 101
char 1 110
char 1 117
char 1 47
char 1 97
char 1 114
char 1 116
char 1 47
char 1 115
char 1 97
char 1 98
char 1 101
char 1 114
char 1 95
char 1 98
char 1 108
char 1 117
char 1 101
char 1 0
align 1
LABELV $198
char 1 109
char 1 101
char 1 110
char 1 117
char 1 47
char 1 97
char 1 114
char 1 116
char 1 47
char 1 115
char 1 97
char 1 98
char 1 101
char 1 114
char 1 95
char 1 103
char 1 114
char 1 101
char 1 101
char 1 110
char 1 0
align 1
LABELV $196
char 1 109
char 1 101
char 1 110
char 1 117
char 1 47
char 1 97
char 1 114
char 1 116
char 1 47
char 1 115
char 1 97
char 1 98
char 1 101
char 1 114
char 1 95
char 1 121
char 1 101
char 1 108
char 1 108
char 1 111
char 1 119
char 1 0
align 1
LABELV $194
char 1 109
char 1 101
char 1 110
char 1 117
char 1 47
char 1 97
char 1 114
char 1 116
char 1 47
char 1 115
char 1 97
char 1 98
char 1 101
char 1 114
char 1 95
char 1 111
char 1 114
char 1 97
char 1 110
char 1 103
char 1 101
char 1 0
align 1
LABELV $192
char 1 109
char 1 101
char 1 110
char 1 117
char 1 47
char 1 97
char 1 114
char 1 116
char 1 47
char 1 115
char 1 97
char 1 98
char 1 101
char 1 114
char 1 95
char 1 114
char 1 101
char 1 100
char 1 0
align 1
LABELV $191
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 115
char 1 116
char 1 97
char 1 114
char 1 56
char 1 0
align 1
LABELV $188
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 99
char 1 105
char 1 114
char 1 99
char 1 108
char 1 101
char 1 56
char 1 0
align 1
LABELV $186
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 115
char 1 116
char 1 97
char 1 114
char 1 55
char 1 0
align 1
LABELV $183
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 99
char 1 105
char 1 114
char 1 99
char 1 108
char 1 101
char 1 55
char 1 0
align 1
LABELV $181
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 115
char 1 116
char 1 97
char 1 114
char 1 54
char 1 0
align 1
LABELV $178
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 99
char 1 105
char 1 114
char 1 99
char 1 108
char 1 101
char 1 54
char 1 0
align 1
LABELV $176
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 115
char 1 116
char 1 97
char 1 114
char 1 53
char 1 0
align 1
LABELV $173
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 99
char 1 105
char 1 114
char 1 99
char 1 108
char 1 101
char 1 53
char 1 0
align 1
LABELV $171
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 115
char 1 116
char 1 97
char 1 114
char 1 52
char 1 0
align 1
LABELV $168
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 99
char 1 105
char 1 114
char 1 99
char 1 108
char 1 101
char 1 52
char 1 0
align 1
LABELV $166
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 115
char 1 116
char 1 97
char 1 114
char 1 51
char 1 0
align 1
LABELV $163
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 99
char 1 105
char 1 114
char 1 99
char 1 108
char 1 101
char 1 51
char 1 0
align 1
LABELV $161
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 115
char 1 116
char 1 97
char 1 114
char 1 50
char 1 0
align 1
LABELV $158
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 99
char 1 105
char 1 114
char 1 99
char 1 108
char 1 101
char 1 50
char 1 0
align 1
LABELV $156
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 115
char 1 116
char 1 97
char 1 114
char 1 49
char 1 0
align 1
LABELV $153
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 99
char 1 105
char 1 114
char 1 99
char 1 108
char 1 101
char 1 49
char 1 0
align 1
LABELV $150
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 115
char 1 116
char 1 97
char 1 114
char 1 48
char 1 0
