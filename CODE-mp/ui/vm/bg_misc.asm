data
export forceMasteryLevels
align 4
LABELV forceMasteryLevels
address $120
address $121
address $122
address $123
address $124
address $125
address $126
address $127
export forceMasteryPoints
align 4
LABELV forceMasteryPoints
byte 4 0
byte 4 5
byte 4 10
byte 4 20
byte 4 30
byte 4 50
byte 4 75
byte 4 100
export bgForcePowerCost
align 4
LABELV bgForcePowerCost
byte 4 0
byte 4 2
byte 4 4
byte 4 6
byte 4 0
byte 4 0
byte 4 2
byte 4 6
byte 4 0
byte 4 2
byte 4 4
byte 4 6
byte 4 0
byte 4 1
byte 4 3
byte 4 6
byte 4 0
byte 4 1
byte 4 3
byte 4 6
byte 4 0
byte 4 4
byte 4 6
byte 4 8
byte 4 0
byte 4 1
byte 4 3
byte 4 6
byte 4 0
byte 4 2
byte 4 5
byte 4 8
byte 4 0
byte 4 4
byte 4 6
byte 4 8
byte 4 0
byte 4 2
byte 4 5
byte 4 8
byte 4 0
byte 4 1
byte 4 3
byte 4 6
byte 4 0
byte 4 1
byte 4 3
byte 4 6
byte 4 0
byte 4 1
byte 4 3
byte 4 6
byte 4 0
byte 4 2
byte 4 4
byte 4 6
byte 4 0
byte 4 2
byte 4 5
byte 4 8
byte 4 0
byte 4 1
byte 4 5
byte 4 8
byte 4 0
byte 4 1
byte 4 5
byte 4 8
byte 4 0
byte 4 4
byte 4 6
byte 4 8
export forcePowerSorted
align 4
LABELV forcePowerSorted
byte 4 5
byte 4 0
byte 4 10
byte 4 9
byte 4 11
byte 4 1
byte 4 2
byte 4 3
byte 4 4
byte 4 14
byte 4 7
byte 4 13
byte 4 8
byte 4 6
byte 4 12
byte 4 15
byte 4 16
byte 4 17
export forcePowerDarkLight
align 4
LABELV forcePowerDarkLight
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
export WeaponReadyAnim
align 4
LABELV WeaponReadyAnim
byte 4 954
byte 4 964
byte 4 553
byte 4 963
byte 4 964
byte 4 964
byte 4 964
byte 4 964
byte 4 964
byte 4 964
byte 4 964
byte 4 971
byte 4 971
byte 4 971
byte 4 550
byte 4 962
export WeaponAttackAnim
align 4
LABELV WeaponAttackAnim
byte 4 106
byte 4 108
byte 4 553
byte 4 107
byte 4 108
byte 4 108
byte 4 108
byte 4 108
byte 4 108
byte 4 108
byte 4 108
byte 4 125
byte 4 108
byte 4 108
byte 4 550
byte 4 106
export BG_LegalizedForcePowers
code
proc BG_LegalizedForcePowers 436 12
file "../../game/bg_misc.c"
line 171
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:// bg_misc.c -- both games misc functions, all completely stateless
;4:
;5:#include "q_shared.h"
;6:#include "bg_public.h"
;7:
;8:#ifdef QAGAME
;9:#include "g_local.h"
;10:#endif
;11:
;12:#ifdef UI_EXPORTS
;13:#include "../ui/ui_local.h"
;14:#endif
;15:
;16:#ifndef UI_EXPORTS
;17:#ifndef QAGAME
;18:#include "../cgame/cg_local.h"
;19:#endif
;20:#endif
;21:
;22://rww - not putting @ in front of these because
;23://we don't need them in a cgame striped lookup.
;24://Let me know if this causes problems, pat.
;25:char *forceMasteryLevels[NUM_FORCE_MASTERY_LEVELS] = 
;26:{
;27:	"MASTERY0",	//"Uninitiated",	// FORCE_MASTERY_UNINITIATED,
;28:	"MASTERY1",	//"Initiate",		// FORCE_MASTERY_INITIATE,
;29:	"MASTERY2",	//"Padawan",		// FORCE_MASTERY_PADAWAN,
;30:	"MASTERY3",	//"Jedi",			// FORCE_MASTERY_JEDI,
;31:	"MASTERY4",	//"Jedi Adept",		// FORCE_MASTERY_JEDI_GUARDIAN,
;32:	"MASTERY5",	//"Jedi Guardian",	// FORCE_MASTERY_JEDI_ADEPT,
;33:	"MASTERY6",	//"Jedi Knight",	// FORCE_MASTERY_JEDI_KNIGHT,
;34:	"MASTERY7",	//"Jedi Master"		// FORCE_MASTERY_JEDI_MASTER,
;35:};
;36:
;37:int forceMasteryPoints[NUM_FORCE_MASTERY_LEVELS] =
;38:{
;39:	0,		// FORCE_MASTERY_UNINITIATED,
;40:	5,		// FORCE_MASTERY_INITIATE,
;41:	10,		// FORCE_MASTERY_PADAWAN,
;42:	20,		// FORCE_MASTERY_JEDI,
;43:	30,		// FORCE_MASTERY_JEDI_GUARDIAN,
;44:	50,		// FORCE_MASTERY_JEDI_ADEPT,
;45:	75,		// FORCE_MASTERY_JEDI_KNIGHT,
;46:	100		// FORCE_MASTERY_JEDI_MASTER,
;47:};
;48:
;49:int bgForcePowerCost[NUM_FORCE_POWERS][NUM_FORCE_POWER_LEVELS] = //0 == neutral
;50:{
;51:	{	0,	2,	4,	6	},	// Heal			// FP_HEAL
;52:	{	0,	0,	2,	6	},	// Jump			//FP_LEVITATION,//hold/duration
;53:	{	0,	2,	4,	6	},	// Speed		//FP_SPEED,//duration
;54:	{	0,	1,	3,	6	},	// Push			//FP_PUSH,//hold/duration
;55:	{	0,	1,	3,	6	},	// Pull			//FP_PULL,//hold/duration
;56:	{	0,	4,	6,	8	},	// Mind Trick	//FP_TELEPATHY,//instant
;57:	{	0,	1,	3,	6	},	// Grip			//FP_GRIP,//hold/duration
;58:	{	0,	2,	5,	8	},	// Lightning	//FP_LIGHTNING,//hold/duration
;59:	{	0,	4,	6,	8	},	// Dark Rage	//FP_RAGE,//duration
;60:	{	0,	2,	5,	8	},	// Protection	//FP_PROTECT,//duration
;61:	{	0,	1,	3,	6	},	// Absorb		//FP_ABSORB,//duration
;62:	{	0,	1,	3,	6	},	// Team Heal	//FP_TEAM_HEAL,//instant
;63:	{	0,	1,	3,	6	},	// Team Force	//FP_TEAM_FORCE,//instant
;64:	{	0,	2,	4,	6	},	// Drain		//FP_DRAIN,//hold/duration
;65:	{	0,	2,	5,	8	},	// Sight		//FP_SEE,//duration
;66:	{	0,	1,	5,	8	},	// Saber Attack	//FP_SABERATTACK,
;67:	{	0,	1,	5,	8	},	// Saber Defend	//FP_SABERDEFEND,
;68:	{	0,	4,	6,	8	}	// Saber Throw	//FP_SABERTHROW,
;69:	//NUM_FORCE_POWERS
;70:};
;71:
;72:int forcePowerSorted[NUM_FORCE_POWERS] = 
;73:{ //rww - always use this order when drawing force powers for any reason
;74:	FP_TELEPATHY,
;75:	FP_HEAL,
;76:	FP_ABSORB,
;77:	FP_PROTECT,
;78:	FP_TEAM_HEAL,
;79:	FP_LEVITATION,
;80:	FP_SPEED,
;81:	FP_PUSH,
;82:	FP_PULL,
;83:	FP_SEE,
;84:	FP_LIGHTNING,
;85:	FP_DRAIN,
;86:	FP_RAGE,
;87:	FP_GRIP,
;88:	FP_TEAM_FORCE,
;89:	FP_SABERATTACK,
;90:	FP_SABERDEFEND,
;91:	FP_SABERTHROW
;92:};
;93:
;94:int forcePowerDarkLight[NUM_FORCE_POWERS] = //0 == neutral
;95:{ //nothing should be usable at rank 0..
;96:	FORCE_LIGHTSIDE,//FP_HEAL,//instant
;97:	0,//FP_LEVITATION,//hold/duration
;98:	0,//FP_SPEED,//duration
;99:	0,//FP_PUSH,//hold/duration
;100:	0,//FP_PULL,//hold/duration
;101:	FORCE_LIGHTSIDE,//FP_TELEPATHY,//instant
;102:	FORCE_DARKSIDE,//FP_GRIP,//hold/duration
;103:	FORCE_DARKSIDE,//FP_LIGHTNING,//hold/duration
;104:	FORCE_DARKSIDE,//FP_RAGE,//duration
;105:	FORCE_LIGHTSIDE,//FP_PROTECT,//duration
;106:	FORCE_LIGHTSIDE,//FP_ABSORB,//duration
;107:	FORCE_LIGHTSIDE,//FP_TEAM_HEAL,//instant
;108:	FORCE_DARKSIDE,//FP_TEAM_FORCE,//instant
;109:	FORCE_DARKSIDE,//FP_DRAIN,//hold/duration
;110:	0,//FP_SEE,//duration
;111:	0,//FP_SABERATTACK,
;112:	0,//FP_SABERDEFEND,
;113:	0//FP_SABERTHROW,
;114:		//NUM_FORCE_POWERS
;115:};
;116:
;117:int WeaponReadyAnim[WP_NUM_WEAPONS] =
;118:{
;119:	TORSO_DROPWEAP1,//WP_NONE,
;120:
;121:	TORSO_WEAPONREADY3,//WP_STUN_BATON,
;122:	BOTH_STAND2,//WP_SABER,
;123:	TORSO_WEAPONREADY2,//WP_BRYAR_PISTOL,
;124:	TORSO_WEAPONREADY3,//WP_BLASTER,
;125:	TORSO_WEAPONREADY3,//TORSO_WEAPONREADY4,//WP_DISRUPTOR,
;126:	TORSO_WEAPONREADY3,//TORSO_WEAPONREADY5,//WP_BOWCASTER,
;127:	TORSO_WEAPONREADY3,//TORSO_WEAPONREADY6,//WP_REPEATER,
;128:	TORSO_WEAPONREADY3,//TORSO_WEAPONREADY7,//WP_DEMP2,
;129:	TORSO_WEAPONREADY3,//TORSO_WEAPONREADY8,//WP_FLECHETTE,
;130:	TORSO_WEAPONREADY3,//TORSO_WEAPONREADY9,//WP_ROCKET_LAUNCHER,
;131:	TORSO_WEAPONREADY10,//WP_THERMAL,
;132:	TORSO_WEAPONREADY10,//TORSO_WEAPONREADY11,//WP_TRIP_MINE,
;133:	TORSO_WEAPONREADY10,//TORSO_WEAPONREADY12,//WP_DET_PACK,
;134:
;135:	//NOT VALID (e.g. should never really be used):
;136:	BOTH_STAND1,//WP_EMPLACED_GUN,
;137:	TORSO_WEAPONREADY1//WP_TURRET,
;138:};
;139:
;140:int WeaponAttackAnim[WP_NUM_WEAPONS] =
;141:{
;142:	BOTH_ATTACK1,//WP_NONE, //(shouldn't happen)
;143:
;144:	BOTH_ATTACK3,//WP_STUN_BATON,
;145:	BOTH_STAND2,//WP_SABER, //(has its own handling)
;146:	BOTH_ATTACK2,//WP_BRYAR_PISTOL,
;147:	BOTH_ATTACK3,//WP_BLASTER,
;148:	BOTH_ATTACK3,//BOTH_ATTACK4,//WP_DISRUPTOR,
;149:	BOTH_ATTACK3,//BOTH_ATTACK5,//WP_BOWCASTER,
;150:	BOTH_ATTACK3,//BOTH_ATTACK6,//WP_REPEATER,
;151:	BOTH_ATTACK3,//BOTH_ATTACK7,//WP_DEMP2,
;152:	BOTH_ATTACK3,//BOTH_ATTACK8,//WP_FLECHETTE,
;153:	BOTH_ATTACK3,//BOTH_ATTACK9,//WP_ROCKET_LAUNCHER,
;154:	BOTH_THERMAL_THROW,//WP_THERMAL,
;155:	BOTH_ATTACK3,//BOTH_ATTACK11,//WP_TRIP_MINE,
;156:	BOTH_ATTACK3,//BOTH_ATTACK12,//WP_DET_PACK,
;157:
;158:	//NOT VALID (e.g. should never really be used):
;159:	BOTH_STAND1,//WP_EMPLACED_GUN,
;160:	BOTH_ATTACK1//WP_TURRET,
;161:};
;162:
;163:
;164://The magical function to end all functions.
;165://This will take the force power string in powerOut and parse through it, then legalize
;166://it based on the supposed rank and spit it into powerOut, returning true if it was legal
;167://to begin with and false if not.
;168://fpDisabled is actually only expected (needed) from the server, because the ui disables
;169://force power selection anyway when force powers are disabled on the server.
;170:qboolean BG_LegalizedForcePowers(char *powerOut, int maxRank, qboolean freeSaber, int teamForce, int gametype, int fpDisabled)
;171:{
line 174
;172:	char powerBuf[128];
;173:	char readBuf[128];
;174:	qboolean maintainsValidity = qtrue;
ADDRLP4 352
CNSTI4 1
ASGNI4
line 175
;175:	int powerLen = strlen(powerOut);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 360
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 356
ADDRLP4 360
INDIRI4
ASGNI4
line 176
;176:	int i = 0;
ADDRLP4 76
CNSTI4 0
ASGNI4
line 177
;177:	int c = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 178
;178:	int allowedPoints = 0;
ADDRLP4 88
CNSTI4 0
ASGNI4
line 179
;179:	int usedPoints = 0;
ADDRLP4 80
CNSTI4 0
ASGNI4
line 180
;180:	int countDown = 0;
ADDRLP4 84
CNSTI4 0
ASGNI4
line 185
;181:	
;182:	int final_Side;
;183:	int final_Powers[NUM_FORCE_POWERS];
;184:
;185:	if (powerLen >= 128)
ADDRLP4 356
INDIRI4
CNSTI4 128
LTI4 $129
line 186
;186:	{ //This should not happen. If it does, this is obviously a bogus string.
line 188
;187:		//They can have this string. Because I said so.
;188:		strcpy(powerBuf, "7-1-032330000000001333");
ADDRLP4 92
ARGP4
ADDRGP4 $131
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 189
;189:		maintainsValidity = qfalse;
ADDRLP4 352
CNSTI4 0
ASGNI4
line 190
;190:	}
ADDRGP4 $130
JUMPV
LABELV $129
line 192
;191:	else
;192:	{
line 193
;193:		strcpy(powerBuf, powerOut); //copy it as the original
ADDRLP4 92
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 194
;194:	}
LABELV $130
line 197
;195:
;196:	//first of all, print the max rank into the string as the rank
;197:	strcpy(powerOut, va("%i-", maxRank));
ADDRGP4 $132
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 364
ADDRGP4 va
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 364
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
ADDRGP4 $134
JUMPV
LABELV $133
line 200
;198:
;199:	while (i < 128 && powerBuf[i] && powerBuf[i] != '-')
;200:	{
line 201
;201:		i++;
ADDRLP4 76
ADDRLP4 76
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 202
;202:	}
LABELV $134
line 199
ADDRLP4 76
INDIRI4
CNSTI4 128
GEI4 $137
ADDRLP4 372
ADDRLP4 76
INDIRI4
ADDRLP4 92
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 372
INDIRI4
CNSTI4 0
EQI4 $137
ADDRLP4 372
INDIRI4
CNSTI4 45
NEI4 $133
LABELV $137
line 203
;203:	i++;
ADDRLP4 76
ADDRLP4 76
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRGP4 $139
JUMPV
LABELV $138
line 205
;204:	while (i < 128 && powerBuf[i] && powerBuf[i] != '-')
;205:	{
line 206
;206:		readBuf[c] = powerBuf[i];
ADDRLP4 0
INDIRI4
ADDRLP4 220
ADDP4
ADDRLP4 76
INDIRI4
ADDRLP4 92
ADDP4
INDIRI1
ASGNI1
line 207
;207:		c++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 208
;208:		i++;
ADDRLP4 76
ADDRLP4 76
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 209
;209:	}
LABELV $139
line 204
ADDRLP4 76
INDIRI4
CNSTI4 128
GEI4 $142
ADDRLP4 380
ADDRLP4 76
INDIRI4
ADDRLP4 92
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 380
INDIRI4
CNSTI4 0
EQI4 $142
ADDRLP4 380
INDIRI4
CNSTI4 45
NEI4 $138
LABELV $142
line 210
;210:	readBuf[c] = 0;
ADDRLP4 0
INDIRI4
ADDRLP4 220
ADDP4
CNSTI1 0
ASGNI1
line 211
;211:	i++;
ADDRLP4 76
ADDRLP4 76
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 213
;212:	//at this point, readBuf contains the intended side
;213:	final_Side = atoi(readBuf);
ADDRLP4 220
ARGP4
ADDRLP4 384
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 348
ADDRLP4 384
INDIRI4
ASGNI4
line 215
;214:
;215:	if (final_Side != FORCE_LIGHTSIDE &&
ADDRLP4 348
INDIRI4
CNSTI4 1
EQI4 $143
ADDRLP4 348
INDIRI4
CNSTI4 2
EQI4 $143
line 217
;216:		final_Side != FORCE_DARKSIDE)
;217:	{ //Not a valid side. You will be dark. Because I said so. (this is something that should never actually happen unless you purposely feed in an invalid config)
line 218
;218:		final_Side = FORCE_DARKSIDE;
ADDRLP4 348
CNSTI4 2
ASGNI4
line 219
;219:		maintainsValidity = qfalse;
ADDRLP4 352
CNSTI4 0
ASGNI4
line 220
;220:	}
LABELV $143
line 222
;221:
;222:	if (teamForce)
ADDRFP4 12
INDIRI4
CNSTI4 0
EQI4 $145
line 223
;223:	{ //If we are under force-aligned teams, make sure we're on the right side.
line 224
;224:		if (final_Side != teamForce)
ADDRLP4 348
INDIRI4
ADDRFP4 12
INDIRI4
EQI4 $147
line 225
;225:		{
line 226
;226:			final_Side = teamForce;
ADDRLP4 348
ADDRFP4 12
INDIRI4
ASGNI4
line 229
;227:			//maintainsValidity = qfalse;
;228:			//Not doing this, for now. Let them join the team with their filtered powers.
;229:		}
LABELV $147
line 230
;230:	}
LABELV $145
line 234
;231:
;232:	//Now we have established a valid rank, and a valid side.
;233:	//Read the force powers in, and cut them down based on the various rules supplied.
;234:	c = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $150
JUMPV
LABELV $149
line 236
;235:	while (i < 128 && powerBuf[i] && powerBuf[i] != '\n' && c < NUM_FORCE_POWERS)
;236:	{
line 237
;237:		readBuf[0] = powerBuf[i];
ADDRLP4 220
ADDRLP4 76
INDIRI4
ADDRLP4 92
ADDP4
INDIRI1
ASGNI1
line 238
;238:		readBuf[1] = 0;
ADDRLP4 220+1
CNSTI1 0
ASGNI1
line 239
;239:		final_Powers[c] = atoi(readBuf);
ADDRLP4 220
ARGP4
ADDRLP4 392
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
ADDRLP4 392
INDIRI4
ASGNI4
line 240
;240:		c++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 241
;241:		i++;
ADDRLP4 76
ADDRLP4 76
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 242
;242:	}
LABELV $150
line 235
ADDRLP4 76
INDIRI4
CNSTI4 128
GEI4 $155
ADDRLP4 396
ADDRLP4 76
INDIRI4
ADDRLP4 92
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 396
INDIRI4
CNSTI4 0
EQI4 $155
ADDRLP4 396
INDIRI4
CNSTI4 10
EQI4 $155
ADDRLP4 0
INDIRI4
CNSTI4 18
LTI4 $149
LABELV $155
line 246
;243:
;244:	//final_Powers now contains all the stuff from the string
;245:	//Set the maximum allowed points used based on the max rank level, and count the points actually used.
;246:	allowedPoints = forceMasteryPoints[maxRank];
ADDRLP4 88
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 forceMasteryPoints
ADDP4
INDIRI4
ASGNI4
line 248
;247:
;248:	i = 0;
ADDRLP4 76
CNSTI4 0
ASGNI4
ADDRGP4 $157
JUMPV
LABELV $156
line 250
;249:	while (i < NUM_FORCE_POWERS)
;250:	{ //if this power doesn't match the side we're on, then 0 it now.
line 251
;251:		if (final_Powers[i] &&
ADDRLP4 400
ADDRLP4 76
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 404
CNSTI4 0
ASGNI4
ADDRLP4 400
INDIRI4
ADDRLP4 4
ADDP4
INDIRI4
ADDRLP4 404
INDIRI4
EQI4 $159
ADDRLP4 408
ADDRLP4 400
INDIRI4
ADDRGP4 forcePowerDarkLight
ADDP4
INDIRI4
ASGNI4
ADDRLP4 408
INDIRI4
ADDRLP4 404
INDIRI4
EQI4 $159
ADDRLP4 408
INDIRI4
ADDRLP4 348
INDIRI4
EQI4 $159
line 254
;252:			forcePowerDarkLight[i] &&
;253:			forcePowerDarkLight[i] != final_Side)
;254:		{
line 255
;255:			final_Powers[i] = 0;
ADDRLP4 76
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
CNSTI4 0
ASGNI4
line 258
;256:			//This is only likely to happen with g_forceBasedTeams. Let it slide.
;257:			//maintainsValidity = 0;
;258:		}
LABELV $159
line 260
;259:
;260:		if ( final_Powers[i] &&
ADDRLP4 416
CNSTI4 0
ASGNI4
ADDRLP4 76
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ADDRLP4 416
INDIRI4
EQI4 $161
ADDRFP4 20
INDIRI4
CNSTI4 1
ADDRLP4 76
INDIRI4
LSHI4
BANDI4
ADDRLP4 416
INDIRI4
EQI4 $161
line 262
;261:			(fpDisabled & (1 << i)) )
;262:		{ //if this power is disabled on the server via said server option, then we don't get it.
line 263
;263:			final_Powers[i] = 0;
ADDRLP4 76
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
CNSTI4 0
ASGNI4
line 264
;264:		}
LABELV $161
line 266
;265:
;266:		i++;
ADDRLP4 76
ADDRLP4 76
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 267
;267:	}
LABELV $157
line 249
ADDRLP4 76
INDIRI4
CNSTI4 18
LTI4 $156
line 269
;268:
;269:	if (gametype < GT_TEAM)
ADDRFP4 16
INDIRI4
CNSTI4 5
GEI4 $163
line 270
;270:	{ //don't bother with team powers then
line 271
;271:		final_Powers[FP_TEAM_HEAL] = 0;
ADDRLP4 4+44
CNSTI4 0
ASGNI4
line 272
;272:		final_Powers[FP_TEAM_FORCE] = 0;
ADDRLP4 4+48
CNSTI4 0
ASGNI4
line 273
;273:	}
LABELV $163
line 275
;274:
;275:	usedPoints = 0;
ADDRLP4 80
CNSTI4 0
ASGNI4
line 276
;276:	i = 0;
ADDRLP4 76
CNSTI4 0
ASGNI4
ADDRGP4 $168
JUMPV
LABELV $167
line 278
;277:	while (i < NUM_FORCE_POWERS)
;278:	{
line 279
;279:		countDown = 0;
ADDRLP4 84
CNSTI4 0
ASGNI4
line 281
;280:
;281:		countDown = final_Powers[i];
ADDRLP4 84
ADDRLP4 76
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ASGNI4
ADDRGP4 $171
JUMPV
LABELV $170
line 284
;282:
;283:		while (countDown > 0)
;284:		{
line 285
;285:			usedPoints += bgForcePowerCost[i][countDown]; //[fp index][fp level]
ADDRLP4 80
ADDRLP4 80
INDIRI4
ADDRLP4 84
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 76
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 287
;286:			//if this is jump, or we have a free saber and it's offense or defense, take the level back down on level 1
;287:			if ( countDown == 1 &&
ADDRLP4 400
CNSTI4 1
ASGNI4
ADDRLP4 84
INDIRI4
ADDRLP4 400
INDIRI4
NEI4 $173
ADDRLP4 76
INDIRI4
ADDRLP4 400
INDIRI4
EQI4 $176
ADDRLP4 76
INDIRI4
CNSTI4 15
NEI4 $177
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $176
LABELV $177
ADDRLP4 76
INDIRI4
CNSTI4 16
NEI4 $173
ADDRFP4 8
INDIRI4
CNSTI4 0
EQI4 $173
LABELV $176
line 291
;288:				((i == FP_LEVITATION) ||
;289:				 (i == FP_SABERATTACK && freeSaber) ||
;290:				 (i == FP_SABERDEFEND && freeSaber)) )
;291:			{
line 292
;292:				usedPoints -= bgForcePowerCost[i][countDown];
ADDRLP4 80
ADDRLP4 80
INDIRI4
ADDRLP4 84
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 76
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
SUBI4
ASGNI4
line 293
;293:			}
LABELV $173
line 294
;294:			countDown--;
ADDRLP4 84
ADDRLP4 84
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 295
;295:		}
LABELV $171
line 283
ADDRLP4 84
INDIRI4
CNSTI4 0
GTI4 $170
line 297
;296:
;297:		i++;
ADDRLP4 76
ADDRLP4 76
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 298
;298:	}
LABELV $168
line 277
ADDRLP4 76
INDIRI4
CNSTI4 18
LTI4 $167
line 300
;299:
;300:	if (usedPoints > allowedPoints)
ADDRLP4 80
INDIRI4
ADDRLP4 88
INDIRI4
LEI4 $178
line 301
;301:	{ //Time to do the fancy stuff. (meaning, slowly cut parts off while taking a guess at what is most or least important in the config)
line 302
;302:		int attemptedCycles = 0;
ADDRLP4 408
CNSTI4 0
ASGNI4
line 303
;303:		int powerCycle = 2;
ADDRLP4 400
CNSTI4 2
ASGNI4
line 304
;304:		int minPow = 0;
ADDRLP4 404
CNSTI4 0
ASGNI4
line 306
;305:		
;306:		if (freeSaber)
ADDRFP4 8
INDIRI4
CNSTI4 0
EQI4 $180
line 307
;307:		{
line 308
;308:			minPow = 1;
ADDRLP4 404
CNSTI4 1
ASGNI4
line 309
;309:		}
LABELV $180
line 311
;310:
;311:		maintainsValidity = qfalse;
ADDRLP4 352
CNSTI4 0
ASGNI4
ADDRGP4 $183
JUMPV
LABELV $182
line 314
;312:
;313:		while (usedPoints > allowedPoints)
;314:		{
line 315
;315:			c = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $186
JUMPV
LABELV $185
line 318
;316:
;317:			while (c < NUM_FORCE_POWERS && usedPoints > allowedPoints)
;318:			{
line 319
;319:				if (final_Powers[c] && final_Powers[c] < powerCycle)
ADDRLP4 412
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 412
INDIRI4
CNSTI4 0
EQI4 $188
ADDRLP4 412
INDIRI4
ADDRLP4 400
INDIRI4
GEI4 $188
line 320
;320:				{ //kill in order of lowest powers, because the higher powers are probably more important
line 321
;321:					if (c == FP_SABERATTACK &&
ADDRLP4 0
INDIRI4
CNSTI4 15
NEI4 $207
ADDRLP4 4+64
INDIRI4
ADDRLP4 404
INDIRI4
GTI4 $194
ADDRLP4 4+68
INDIRI4
CNSTI4 0
LEI4 $207
LABELV $194
line 323
;322:						(final_Powers[FP_SABERDEFEND] > minPow || final_Powers[FP_SABERTHROW] > 0))
;323:					{ //if we're on saber attack, only suck it down if we have no def or throw either
line 324
;324:						int whichOne = FP_SABERTHROW; //first try throw
ADDRLP4 416
CNSTI4 17
ASGNI4
line 326
;325:
;326:						if (!final_Powers[whichOne])
ADDRLP4 416
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
CNSTI4 0
NEI4 $198
line 327
;327:						{
line 328
;328:							whichOne = FP_SABERDEFEND; //if no throw, drain defense
ADDRLP4 416
CNSTI4 16
ASGNI4
line 329
;329:						}
ADDRGP4 $198
JUMPV
LABELV $197
line 332
;330:
;331:						while (final_Powers[whichOne] > 0 && usedPoints > allowedPoints)
;332:						{
line 333
;333:							if ( final_Powers[whichOne] > 1 ||
ADDRLP4 416
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
CNSTI4 1
GTI4 $204
ADDRLP4 416
INDIRI4
CNSTI4 15
NEI4 $203
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $191
LABELV $203
ADDRLP4 416
INDIRI4
CNSTI4 16
NEI4 $204
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $191
LABELV $204
line 336
;334:								( (whichOne != FP_SABERATTACK || !freeSaber) &&
;335:								  (whichOne != FP_SABERDEFEND || !freeSaber) ) )
;336:							{ //don't take attack or defend down on level 1 still, if it's free
line 337
;337:								usedPoints -= bgForcePowerCost[whichOne][final_Powers[whichOne]];
ADDRLP4 428
CNSTI4 2
ASGNI4
ADDRLP4 80
ADDRLP4 80
INDIRI4
ADDRLP4 416
INDIRI4
ADDRLP4 428
INDIRI4
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ADDRLP4 428
INDIRI4
LSHI4
ADDRLP4 416
INDIRI4
CNSTI4 4
LSHI4
ADDRGP4 bgForcePowerCost
ADDP4
ADDP4
INDIRI4
SUBI4
ASGNI4
line 338
;338:								final_Powers[whichOne]--;
ADDRLP4 432
ADDRLP4 416
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
ASGNP4
ADDRLP4 432
INDIRP4
ADDRLP4 432
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 339
;339:							}
line 341
;340:							else
;341:							{
line 342
;342:								break;
LABELV $201
line 344
;343:							}
;344:						}
LABELV $198
line 331
ADDRLP4 416
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
CNSTI4 0
LEI4 $205
ADDRLP4 80
INDIRI4
ADDRLP4 88
INDIRI4
GTI4 $197
LABELV $205
line 345
;345:					}
ADDRGP4 $191
JUMPV
line 347
;346:					else
;347:					{
LABELV $206
line 349
;348:						while (final_Powers[c] > 0 && usedPoints > allowedPoints)
;349:						{
line 350
;350:							if ( final_Powers[c] > 1 ||
ADDRLP4 420
CNSTI4 1
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ADDRLP4 420
INDIRI4
GTI4 $213
ADDRLP4 0
INDIRI4
ADDRLP4 420
INDIRI4
EQI4 $208
ADDRLP4 0
INDIRI4
CNSTI4 15
NEI4 $212
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $208
LABELV $212
ADDRLP4 0
INDIRI4
CNSTI4 16
NEI4 $213
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $208
LABELV $213
line 354
;351:								((c != FP_LEVITATION) &&
;352:								(c != FP_SABERATTACK || !freeSaber) &&
;353:								(c != FP_SABERDEFEND || !freeSaber)) )
;354:							{
line 355
;355:								usedPoints -= bgForcePowerCost[c][final_Powers[c]];
ADDRLP4 428
CNSTI4 2
ASGNI4
ADDRLP4 80
ADDRLP4 80
INDIRI4
ADDRLP4 0
INDIRI4
ADDRLP4 428
INDIRI4
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ADDRLP4 428
INDIRI4
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
line 356
;356:								final_Powers[c]--;
ADDRLP4 432
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
ASGNP4
ADDRLP4 432
INDIRP4
ADDRLP4 432
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 357
;357:							}
line 359
;358:							else
;359:							{
line 360
;360:								break;
LABELV $210
line 362
;361:							}
;362:						}
LABELV $207
line 348
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
CNSTI4 0
LEI4 $214
ADDRLP4 80
INDIRI4
ADDRLP4 88
INDIRI4
GTI4 $206
LABELV $214
LABELV $208
line 363
;363:					}
LABELV $191
line 364
;364:				}
LABELV $188
line 366
;365:
;366:				c++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 367
;367:			}
LABELV $186
line 317
ADDRLP4 0
INDIRI4
CNSTI4 18
GEI4 $215
ADDRLP4 80
INDIRI4
ADDRLP4 88
INDIRI4
GTI4 $185
LABELV $215
line 369
;368:
;369:			powerCycle++;
ADDRLP4 400
ADDRLP4 400
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 370
;370:			attemptedCycles++;
ADDRLP4 408
ADDRLP4 408
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 372
;371:
;372:			if (attemptedCycles > NUM_FORCE_POWERS)
ADDRLP4 408
INDIRI4
CNSTI4 18
LEI4 $216
line 373
;373:			{ //I think this should be impossible. But just in case.
line 374
;374:				break;
ADDRGP4 $184
JUMPV
LABELV $216
line 376
;375:			}
;376:		}
LABELV $183
line 313
ADDRLP4 80
INDIRI4
ADDRLP4 88
INDIRI4
GTI4 $182
LABELV $184
line 378
;377:
;378:		if (usedPoints > allowedPoints)
ADDRLP4 80
INDIRI4
ADDRLP4 88
INDIRI4
LEI4 $218
line 379
;379:		{ //Still? Fine then.. we will kill all of your powers, except the freebies.
line 380
;380:			i = 0;
ADDRLP4 76
CNSTI4 0
ASGNI4
ADDRGP4 $221
JUMPV
LABELV $220
line 383
;381:
;382:			while (i < NUM_FORCE_POWERS)
;383:			{
line 384
;384:				final_Powers[i] = 0;
ADDRLP4 76
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
CNSTI4 0
ASGNI4
line 385
;385:				if (i == FP_LEVITATION ||
ADDRLP4 76
INDIRI4
CNSTI4 1
EQI4 $226
ADDRLP4 76
INDIRI4
CNSTI4 15
NEI4 $227
ADDRFP4 8
INDIRI4
CNSTI4 0
NEI4 $226
LABELV $227
ADDRLP4 76
INDIRI4
CNSTI4 16
NEI4 $223
ADDRFP4 8
INDIRI4
CNSTI4 0
EQI4 $223
LABELV $226
line 388
;386:					(i == FP_SABERATTACK && freeSaber) ||
;387:					(i == FP_SABERDEFEND && freeSaber))
;388:				{
line 389
;389:					final_Powers[i] = 1;
ADDRLP4 76
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
CNSTI4 1
ASGNI4
line 390
;390:				}
LABELV $223
line 391
;391:				i++;
ADDRLP4 76
ADDRLP4 76
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 392
;392:			}
LABELV $221
line 382
ADDRLP4 76
INDIRI4
CNSTI4 18
LTI4 $220
line 393
;393:			usedPoints = 0;
ADDRLP4 80
CNSTI4 0
ASGNI4
line 394
;394:		}
LABELV $218
line 395
;395:	}
LABELV $178
line 397
;396:
;397:	if (final_Powers[FP_SABERATTACK] < 1)
ADDRLP4 4+60
INDIRI4
CNSTI4 1
GEI4 $228
line 398
;398:	{
line 399
;399:		final_Powers[FP_SABERDEFEND] = 0;
ADDRLP4 4+64
CNSTI4 0
ASGNI4
line 400
;400:		final_Powers[FP_SABERTHROW] = 0;
ADDRLP4 4+68
CNSTI4 0
ASGNI4
line 401
;401:	}
LABELV $228
line 403
;402:
;403:	if (freeSaber)
ADDRFP4 8
INDIRI4
CNSTI4 0
EQI4 $233
line 404
;404:	{
line 405
;405:		if (final_Powers[FP_SABERATTACK] < 1)
ADDRLP4 4+60
INDIRI4
CNSTI4 1
GEI4 $235
line 406
;406:		{
line 407
;407:			final_Powers[FP_SABERATTACK] = 1;
ADDRLP4 4+60
CNSTI4 1
ASGNI4
line 408
;408:		}
LABELV $235
line 409
;409:		if (final_Powers[FP_SABERDEFEND] < 1)
ADDRLP4 4+64
INDIRI4
CNSTI4 1
GEI4 $239
line 410
;410:		{
line 411
;411:			final_Powers[FP_SABERDEFEND] = 1;
ADDRLP4 4+64
CNSTI4 1
ASGNI4
line 412
;412:		}
LABELV $239
line 413
;413:	}
LABELV $233
line 414
;414:	if (final_Powers[FP_LEVITATION] < 1)
ADDRLP4 4+4
INDIRI4
CNSTI4 1
GEI4 $243
line 415
;415:	{
line 416
;416:		final_Powers[FP_LEVITATION] = 1;
ADDRLP4 4+4
CNSTI4 1
ASGNI4
line 417
;417:	}
LABELV $243
line 419
;418:
;419:	if (fpDisabled)
ADDRFP4 20
INDIRI4
CNSTI4 0
EQI4 $247
line 420
;420:	{
line 421
;421:		final_Powers[FP_LEVITATION] = 1;
ADDRLP4 4+4
CNSTI4 1
ASGNI4
line 422
;422:		final_Powers[FP_SABERATTACK] = 3;
ADDRLP4 4+60
CNSTI4 3
ASGNI4
line 423
;423:		final_Powers[FP_SABERDEFEND] = 3;
ADDRLP4 4+64
CNSTI4 3
ASGNI4
line 424
;424:		final_Powers[FP_SABERTHROW] = 0;
ADDRLP4 4+68
CNSTI4 0
ASGNI4
line 425
;425:	}
LABELV $247
line 430
;426:
;427:	//We finally have all the force powers legalized and stored locally.
;428:	//Put them all into the string and return the result. We already have
;429:	//the rank there, so print the side and the powers now.
;430:	Q_strcat(powerOut, 128, va("%i-", final_Side));
ADDRGP4 $132
ARGP4
ADDRLP4 348
INDIRI4
ARGI4
ADDRLP4 400
ADDRGP4 va
CALLP4
ASGNP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 128
ARGI4
ADDRLP4 400
INDIRP4
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
line 432
;431:
;432:	i = strlen(powerOut);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 404
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 76
ADDRLP4 404
INDIRI4
ASGNI4
line 433
;433:	c = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $254
JUMPV
LABELV $253
line 435
;434:	while (c < NUM_FORCE_POWERS)
;435:	{
line 436
;436:		strcpy(readBuf, va("%i", final_Powers[c]));
ADDRGP4 $256
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ARGI4
ADDRLP4 408
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 220
ARGP4
ADDRLP4 408
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 437
;437:		powerOut[i] = readBuf[0];
ADDRLP4 76
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
ADDRLP4 220
INDIRI1
ASGNI1
line 438
;438:		c++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 439
;439:		i++;
ADDRLP4 76
ADDRLP4 76
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 440
;440:	}
LABELV $254
line 434
ADDRLP4 0
INDIRI4
CNSTI4 18
LTI4 $253
line 441
;441:	powerOut[i] = 0;
ADDRLP4 76
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
CNSTI1 0
ASGNI1
line 443
;442:
;443:	return maintainsValidity;
ADDRLP4 352
INDIRI4
RETI4
LABELV $128
endproc BG_LegalizedForcePowers 436 12
data
export bg_itemlist
align 4
LABELV bg_itemlist
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
address $257
address $257
address $258
address $259
address $260
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $261
byte 4 25
byte 4 3
byte 4 1
address $257
address $257
address $262
address $259
address $263
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $264
byte 4 100
byte 4 3
byte 4 2
address $257
address $257
address $265
address $266
address $267
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $268
byte 4 25
byte 4 4
byte 4 0
address $257
address $257
address $269
address $270
address $271
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $272
byte 4 120
byte 4 6
byte 4 1
address $257
address $257
address $273
address $270
address $274
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $275
byte 4 120
byte 4 6
byte 4 2
address $257
address $276
address $277
address $270
address $278
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $279
byte 4 25
byte 4 6
byte 4 3
address $257
address $257
address $280
address $270
address $281
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 0
byte 4 1
byte 4 6
byte 4 4
address $257
address $257
address $282
address $270
address $283
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $284
byte 4 60
byte 4 6
byte 4 5
address $257
address $257
address $285
address $270
address $286
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $287
byte 4 120
byte 4 6
byte 4 6
address $257
address $257
address $288
address $289
address $290
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $291
byte 4 25
byte 4 5
byte 4 12
address $257
address $257
address $292
address $289
address $293
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $294
byte 4 25
byte 4 5
byte 4 13
address $257
address $257
address $295
address $296
address $297
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $298
byte 4 25
byte 4 5
byte 4 14
address $257
address $257
address $299
address $300
address $301
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $302
byte 4 25
byte 4 5
byte 4 15
address $257
address $257
address $303
address $270
address $304
byte 4 0
byte 4 0
byte 4 0
address $305
address $306
byte 4 100
byte 4 1
byte 4 1
address $257
address $257
address $307
address $270
address $308
byte 4 0
byte 4 0
byte 4 0
address $309
address $310
byte 4 100
byte 4 1
byte 4 2
address $257
address $257
address $311
address $270
address $312
byte 4 0
byte 4 0
byte 4 0
address $313
address $314
byte 4 100
byte 4 1
byte 4 3
address $257
address $257
address $315
address $270
address $316
byte 4 0
byte 4 0
byte 4 0
address $317
address $318
byte 4 100
byte 4 1
byte 4 4
address $257
address $257
address $319
address $270
address $320
byte 4 0
byte 4 0
byte 4 0
address $321
address $322
byte 4 100
byte 4 1
byte 4 5
address $257
address $257
address $323
address $270
address $324
byte 4 0
byte 4 0
byte 4 0
address $325
address $326
byte 4 100
byte 4 1
byte 4 6
address $257
address $257
address $327
address $270
address $328
byte 4 0
byte 4 0
byte 4 0
address $329
address $330
byte 4 100
byte 4 1
byte 4 7
address $257
address $257
address $331
address $270
address $332
byte 4 0
byte 4 0
byte 4 0
address $333
address $334
byte 4 100
byte 4 1
byte 4 8
address $257
address $257
address $335
address $270
address $336
byte 4 0
byte 4 0
byte 4 0
address $337
address $338
byte 4 100
byte 4 1
byte 4 9
address $257
address $257
address $339
address $270
address $340
byte 4 0
byte 4 0
byte 4 0
address $341
address $342
byte 4 3
byte 4 1
byte 4 10
address $257
address $257
address $343
address $270
address $344
address $345
byte 4 0
byte 4 0
address $346
address $347
byte 4 4
byte 4 2
byte 4 7
address $257
address $257
address $348
address $270
address $349
address $350
byte 4 0
byte 4 0
address $351
address $352
byte 4 3
byte 4 2
byte 4 8
address $257
address $257
address $353
address $270
address $354
address $355
address $356
byte 4 0
address $357
address $358
byte 4 3
byte 4 2
byte 4 9
address $257
address $257
address $359
address $270
address $345
address $344
byte 4 0
byte 4 0
address $346
address $347
byte 4 4
byte 4 1
byte 4 11
address $257
address $257
address $360
address $270
address $350
address $349
byte 4 0
byte 4 0
address $351
address $352
byte 4 3
byte 4 1
byte 4 12
address $257
address $257
address $361
address $270
address $355
address $354
address $356
byte 4 0
address $357
address $358
byte 4 3
byte 4 1
byte 4 13
address $257
address $257
address $362
address $270
address $316
byte 4 0
byte 4 0
byte 4 0
address $317
address $318
byte 4 50
byte 4 1
byte 4 14
address $257
address $257
address $363
address $270
address $316
byte 4 0
byte 4 0
byte 4 0
address $317
address $318
byte 4 50
byte 4 1
byte 4 15
address $257
address $257
address $364
address $365
address $366
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $318
byte 4 100
byte 4 2
byte 4 1
address $257
address $257
address $367
address $365
address $366
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $368
byte 4 100
byte 4 2
byte 4 2
address $257
address $257
address $369
address $365
address $370
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $371
byte 4 100
byte 4 2
byte 4 3
address $257
address $257
address $372
address $365
address $373
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $374
byte 4 100
byte 4 2
byte 4 4
address $257
address $257
address $375
address $365
address $376
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $377
byte 4 3
byte 4 2
byte 4 5
address $257
address $257
address $378
byte 4 0
address $379
address $380
byte 4 0
byte 4 0
byte 4 0
address $381
byte 4 0
byte 4 8
byte 4 4
address $257
address $257
address $382
byte 4 0
address $383
address $384
byte 4 0
byte 4 0
byte 4 0
address $385
byte 4 0
byte 4 8
byte 4 5
address $257
address $257
address $386
byte 4 0
address $387
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $388
byte 4 0
byte 4 8
byte 4 6
address $257
address $257
address $389
address $365
address $390
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $391
byte 4 0
byte 4 8
byte 4 0
address $257
address $257
address $392
address $365
address $393
byte 4 0
byte 4 0
byte 4 0
byte 4 0
address $394
byte 4 0
byte 4 8
byte 4 0
address $257
address $257
byte 4 0
skip 48
export bg_numItems
align 4
LABELV bg_numItems
byte 4 42
export vectoyaw
code
proc vectoyaw 20 8
line 1235
;444:}
;445:
;446:/*QUAKED item_***** ( 0 0 0 ) (-16 -16 -16) (16 16 16) suspended
;447:DO NOT USE THIS CLASS, IT JUST HOLDS GENERAL INFORMATION.
;448:The suspended flag will allow items to hang in the air, otherwise they are dropped to the next surface.
;449:
;450:If an item is the target of another entity, it will not spawn in until fired.
;451:
;452:An item fires all of its targets when it is picked up.  If the toucher can't carry it, the targets won't be fired.
;453:
;454:"notfree" if set to 1, don't spawn in free for all games
;455:"notteam" if set to 1, don't spawn in team games
;456:"notsingle" if set to 1, don't spawn in single player games
;457:"wait"	override the default wait before respawning.  -1 = never respawn automatically, which can be used with targeted spawning.
;458:"random" random number of plus or minus seconds varied from the respawn time
;459:"count" override quantity or duration on most items.
;460:*/
;461:
;462:/*QUAKED misc_shield_floor_unit (1 0 0) (-16 -16 0) (16 16 40)
;463:#MODELNAME="/models/items/a_shield_converter.md3"
;464:Gives shield energy when used.
;465:
;466:"count" - max charge value (default 50)
;467:"chargerate" - rechage 1 point every this many milliseconds (default 3000)
;468:*/
;469:
;470:gitem_t	bg_itemlist[] = 
;471:{
;472:	{
;473:		NULL,				// classname	
;474:		NULL,				// pickup_sound
;475:		{	NULL,			// world_model[0]
;476:			NULL,			// world_model[1]
;477:			0, 0} ,			// world_model[2],[3]
;478:		NULL,				// view_model
;479:/* icon */		NULL,		// icon
;480:/* pickup */	//NULL,		// pickup_name
;481:		0,					// quantity
;482:		0,					// giType (IT_*)
;483:		0,					// giTag
;484:/* precache */ "",			// precaches
;485:/* sounds */ ""				// sounds
;486:	},	// leave index 0 alone
;487:
;488:	//
;489:	// Pickups
;490:	//
;491:
;492:/*QUAKED item_shield_sm_instant (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;493:Instant shield pickup, restores 25
;494:*/
;495:	{
;496:		"item_shield_sm_instant", 
;497:		"sound/player/pickupshield.wav",
;498:        { "models/map_objects/mp/psd_sm.md3",
;499:		0, 0, 0},
;500:/* view */		NULL,			
;501:/* icon */		"gfx/mp/small_shield",
;502:/* pickup *///	"Shield Small",
;503:		25,
;504:		IT_ARMOR,
;505:		1, //special for shield - max on pickup is maxhealth*tag, thus small shield goes up to 100 shield
;506:/* precache */ "",
;507:/* sounds */ ""
;508:	},
;509:
;510:/*QUAKED item_shield_lrg_instant (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;511:Instant shield pickup, restores 100
;512:*/
;513:	{
;514:		"item_shield_lrg_instant", 
;515:		"sound/player/pickupshield.wav",
;516:        { "models/map_objects/mp/psd.md3",
;517:		0, 0, 0},
;518:/* view */		NULL,			
;519:/* icon */		"gfx/mp/large_shield",
;520:/* pickup *///	"Shield Large",
;521:		100,
;522:		IT_ARMOR,
;523:		2, //special for shield - max on pickup is maxhealth*tag, thus large shield goes up to 200 shield
;524:/* precache */ "",
;525:/* sounds */ ""
;526:	},
;527:
;528:/*QUAKED item_medpak_instant (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;529:Instant medpack pickup, heals 25
;530:*/
;531:	{
;532:		"item_medpak_instant",
;533:		"sound/player/pickuphealth.wav",
;534:        { "models/map_objects/mp/medpac.md3", 
;535:		0, 0, 0 },
;536:/* view */		NULL,			
;537:/* icon */		"gfx/hud/i_icon_medkit",
;538:/* pickup *///	"Medpack",
;539:		25,
;540:		IT_HEALTH,
;541:		0,
;542:/* precache */ "",
;543:/* sounds */ ""
;544:	},
;545:
;546:
;547:	//
;548:	// ITEMS
;549:	//
;550:
;551:/*QUAKED item_seeker (.3 .3 1) (-8 -8 -0) (8 8 16) suspended
;552:30 seconds of seeker drone
;553:*/
;554:	{
;555:		"item_seeker", 
;556:		"sound/weapons/w_pkup.wav",
;557:		{ "models/items/remote.md3", 
;558:		0, 0, 0} ,
;559:/* view */		NULL,			
;560:/* icon */		"gfx/hud/i_icon_seeker",
;561:/* pickup *///	"Seeker Drone",
;562:		120,
;563:		IT_HOLDABLE,
;564:		HI_SEEKER,
;565:/* precache */ "",
;566:/* sounds */ ""
;567:	},
;568:
;569:/*QUAKED item_shield (.3 .3 1) (-8 -8 -0) (8 8 16) suspended
;570:Portable shield
;571:*/
;572:	{
;573:		"item_shield", 
;574:		"sound/weapons/w_pkup.wav",
;575:		{ "models/map_objects/mp/shield.md3", 
;576:		0, 0, 0} ,
;577:/* view */		NULL,			
;578:/* icon */		"gfx/hud/i_icon_shieldwall",
;579:/* pickup *///	"Forcefield",
;580:		120,
;581:		IT_HOLDABLE,
;582:		HI_SHIELD,
;583:/* precache */ "",
;584:/* sounds */ "sound/weapons/detpack/stick.wav sound/movers/doors/forcefield_on.wav sound/movers/doors/forcefield_off.wav sound/movers/doors/forcefield_lp.wav sound/effects/bumpfield.wav",
;585:	},
;586:
;587:/*QUAKED item_medpac (.3 .3 1) (-8 -8 -0) (8 8 16) suspended
;588:Bacta canister pickup, heals 25 on use
;589:*/
;590:	{
;591:		"item_medpac",	//should be item_bacta
;592:		"sound/weapons/w_pkup.wav",
;593:		{ "models/map_objects/mp/bacta.md3", 
;594:		0, 0, 0} ,
;595:/* view */		NULL,			
;596:/* icon */		"gfx/hud/i_icon_bacta",
;597:/* pickup *///	"Bacta Canister",
;598:		25,
;599:		IT_HOLDABLE,
;600:		HI_MEDPAC,
;601:/* precache */ "",
;602:/* sounds */ ""
;603:	},
;604:
;605:/*QUAKED item_datapad (.3 .3 1) (-8 -8 -0) (8 8 16) suspended
;606:Do not place this.
;607:*/
;608:	{
;609:		"item_datapad", 
;610:		"sound/weapons/w_pkup.wav",
;611:		{ "models/items/datapad.md3", 
;612:		0, 0, 0} ,
;613:/* view */		NULL,			
;614:/* icon */		NULL,
;615:/* pickup *///	"Datapad",
;616:		1,
;617:		IT_HOLDABLE,
;618:		HI_DATAPAD,
;619:/* precache */ "",
;620:/* sounds */ ""
;621:	},
;622:
;623:/*QUAKED item_binoculars (.3 .3 1) (-8 -8 -0) (8 8 16) suspended
;624:These will be standard equipment on the player - DO NOT PLACE
;625:*/
;626:	{
;627:		"item_binoculars", 
;628:		"sound/weapons/w_pkup.wav",
;629:		{ "models/items/binoculars.md3", 
;630:		0, 0, 0} ,
;631:/* view */		NULL,			
;632:/* icon */		"gfx/hud/i_icon_zoom",
;633:/* pickup *///	"Binoculars",
;634:		60,
;635:		IT_HOLDABLE,
;636:		HI_BINOCULARS,
;637:/* precache */ "",
;638:/* sounds */ ""
;639:	},
;640:
;641:/*QUAKED item_sentry_gun (.3 .3 1) (-8 -8 -0) (8 8 16) suspended
;642:Sentry gun inventory pickup.
;643:*/
;644:	{
;645:		"item_sentry_gun", 
;646:		"sound/weapons/w_pkup.wav",
;647:		{ "models/items/psgun.glm", 
;648:		0, 0, 0} ,
;649:/* view */		NULL,			
;650:/* icon */		"gfx/hud/i_icon_sentrygun",
;651:/* pickup *///	"Sentry Gun",
;652:		120,
;653:		IT_HOLDABLE,
;654:		HI_SENTRY_GUN,
;655:/* precache */ "",
;656:/* sounds */ ""
;657:	},
;658:
;659:/*QUAKED item_force_enlighten_light (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;660:Adds one rank to all Force powers temporarily. Only light jedi can use.
;661:*/
;662:	{
;663:		"item_force_enlighten_light",
;664:		"sound/player/enlightenment.wav",
;665:		{ "models/map_objects/mp/jedi_enlightenment.md3", 
;666:		0, 0, 0} ,
;667:/* view */		NULL,			
;668:/* icon */		"gfx/hud/mpi_jlight",
;669:/* pickup *///	"Light Force Enlightenment",
;670:		25,
;671:		IT_POWERUP,
;672:		PW_FORCE_ENLIGHTENED_LIGHT,
;673:/* precache */ "",
;674:/* sounds */ ""
;675:	},
;676:
;677:/*QUAKED item_force_enlighten_dark (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;678:Adds one rank to all Force powers temporarily. Only dark jedi can use.
;679:*/
;680:	{
;681:		"item_force_enlighten_dark",
;682:		"sound/player/enlightenment.wav",
;683:		{ "models/map_objects/mp/dk_enlightenment.md3", 
;684:		0, 0, 0} ,
;685:/* view */		NULL,			
;686:/* icon */		"gfx/hud/mpi_dklight",
;687:/* pickup *///	"Dark Force Enlightenment",
;688:		25,
;689:		IT_POWERUP,
;690:		PW_FORCE_ENLIGHTENED_DARK,
;691:/* precache */ "",
;692:/* sounds */ ""
;693:	},
;694:
;695:/*QUAKED item_force_boon (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;696:Unlimited Force Pool for a short time.
;697:*/
;698:	{
;699:		"item_force_boon",
;700:		"sound/player/boon.wav",
;701:		{ "models/map_objects/mp/force_boon.md3", 
;702:		0, 0, 0} ,
;703:/* view */		NULL,			
;704:/* icon */		"gfx/hud/mpi_fboon",
;705:/* pickup *///	"Force Boon",
;706:		25,
;707:		IT_POWERUP,
;708:		PW_FORCE_BOON,
;709:/* precache */ "",
;710:/* sounds */ ""
;711:	},
;712:
;713:/*QUAKED item_ysalimari (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;714:A small lizard carried on the player, which prevents the possessor from using any Force power.  However, he is unaffected by any Force power.
;715:*/
;716:	{
;717:		"item_ysalimari",
;718:		"sound/player/ysalimari.wav",
;719:		{ "models/map_objects/mp/ysalimari.md3", 
;720:		0, 0, 0} ,
;721:/* view */		NULL,			
;722:/* icon */		"gfx/hud/mpi_ysamari",
;723:/* pickup *///	"Ysalamiri",
;724:		25,
;725:		IT_POWERUP,
;726:		PW_YSALAMIRI,
;727:/* precache */ "",
;728:/* sounds */ ""
;729:	},
;730:
;731:	//
;732:	// WEAPONS 
;733:	//
;734:
;735:/*QUAKED weapon_stun_baton (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;736:Don't place this
;737:*/
;738:	{
;739:		"weapon_stun_baton", 
;740:		"sound/weapons/w_pkup.wav",
;741:        { "models/weapons2/stun_baton/baton_w.glm", 
;742:		0, 0, 0},
;743:/* view */		"models/weapons2/stun_baton/baton.md3", 
;744:/* icon */		"gfx/hud/w_icon_stunbaton",
;745:/* pickup *///	"Stun Baton",
;746:		100,
;747:		IT_WEAPON,
;748:		WP_STUN_BATON,
;749:/* precache */ "",
;750:/* sounds */ ""
;751:	},
;752:
;753:/*QUAKED weapon_saber (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;754:Don't place this
;755:*/
;756:	{
;757:		"weapon_saber", 
;758:		"sound/weapons/w_pkup.wav",
;759:        { "models/weapons2/saber/saber_w.glm",
;760:		0, 0, 0},
;761:/* view */		"models/weapons2/saber/saber_w.md3",
;762:/* icon */		"gfx/hud/w_icon_lightsaber",
;763:/* pickup *///	"Lightsaber",
;764:		100,
;765:		IT_WEAPON,
;766:		WP_SABER,
;767:/* precache */ "",
;768:/* sounds */ ""
;769:	},
;770:
;771:/*QUAKED weapon_bryar_pistol (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;772:Don't place this
;773:*/
;774:	{
;775:		"weapon_bryar_pistol", 
;776:		"sound/weapons/w_pkup.wav",
;777:        { "models/weapons2/briar_pistol/briar_pistol_w.glm", 
;778:		0, 0, 0},
;779:/* view */		"models/weapons2/briar_pistol/briar_pistol.md3", 
;780:/* icon */		"gfx/hud/w_icon_rifle",
;781:/* pickup *///	"Bryar Pistol",
;782:		100,
;783:		IT_WEAPON,
;784:		WP_BRYAR_PISTOL,
;785:/* precache */ "",
;786:/* sounds */ ""
;787:	},
;788:
;789:/*QUAKED weapon_blaster (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;790:*/
;791:	{
;792:		"weapon_blaster", 
;793:		"sound/weapons/w_pkup.wav",
;794:        { "models/weapons2/blaster_r/blaster_w.glm", 
;795:		0, 0, 0},
;796:/* view */		"models/weapons2/blaster_r/blaster.md3", 
;797:/* icon */		"gfx/hud/w_icon_blaster",
;798:/* pickup *///	"E11 Blaster Rifle",
;799:		100,
;800:		IT_WEAPON,
;801:		WP_BLASTER,
;802:/* precache */ "",
;803:/* sounds */ ""
;804:	},
;805:
;806:/*QUAKED weapon_disruptor (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;807:*/
;808:	{
;809:		"weapon_disruptor",
;810:		"sound/weapons/w_pkup.wav",
;811:        { "models/weapons2/disruptor/disruptor_w.glm", 
;812:		0, 0, 0},
;813:/* view */		"models/weapons2/disruptor/disruptor.md3", 
;814:/* icon */		"gfx/hud/w_icon_disruptor",
;815:/* pickup *///	"Tenloss Disruptor Rifle",
;816:		100,
;817:		IT_WEAPON,
;818:		WP_DISRUPTOR,
;819:/* precache */ "",
;820:/* sounds */ ""
;821:	},
;822:
;823:/*QUAKED weapon_bowcaster (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;824:*/
;825:	{
;826:		"weapon_bowcaster",
;827:		"sound/weapons/w_pkup.wav",
;828:        { "models/weapons2/bowcaster/bowcaster_w.glm", 
;829:		0, 0, 0},
;830:/* view */		"models/weapons2/bowcaster/bowcaster.md3", 
;831:/* icon */		"gfx/hud/w_icon_bowcaster",
;832:/* pickup *///	"Wookiee Bowcaster",
;833:		100,
;834:		IT_WEAPON,
;835:		WP_BOWCASTER,
;836:/* precache */ "",
;837:/* sounds */ ""
;838:	},
;839:
;840:/*QUAKED weapon_repeater (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;841:*/
;842:	{
;843:		"weapon_repeater", 
;844:		"sound/weapons/w_pkup.wav",
;845:        { "models/weapons2/heavy_repeater/heavy_repeater_w.glm", 
;846:		0, 0, 0},
;847:/* view */		"models/weapons2/heavy_repeater/heavy_repeater.md3", 
;848:/* icon */		"gfx/hud/w_icon_repeater",
;849:/* pickup *///	"Imperial Heavy Repeater",
;850:		100,
;851:		IT_WEAPON,
;852:		WP_REPEATER,
;853:/* precache */ "",
;854:/* sounds */ ""
;855:	},
;856:
;857:/*QUAKED weapon_demp2 (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;858:NOTENOTE This weapon is not yet complete.  Don't place it.
;859:*/
;860:	{
;861:		"weapon_demp2", 
;862:		"sound/weapons/w_pkup.wav",
;863:        { "models/weapons2/demp2/demp2_w.glm", 
;864:		0, 0, 0},
;865:/* view */		"models/weapons2/demp2/demp2.md3", 
;866:/* icon */		"gfx/hud/w_icon_demp2",
;867:/* pickup *///	"DEMP2",
;868:		100,
;869:		IT_WEAPON,
;870:		WP_DEMP2,
;871:/* precache */ "",
;872:/* sounds */ ""
;873:	},
;874:
;875:/*QUAKED weapon_flechette (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;876:*/
;877:	{
;878:		"weapon_flechette", 
;879:		"sound/weapons/w_pkup.wav",
;880:        { "models/weapons2/golan_arms/golan_arms_w.glm", 
;881:		0, 0, 0},
;882:/* view */		"models/weapons2/golan_arms/golan_arms.md3", 
;883:/* icon */		"gfx/hud/w_icon_flechette",
;884:/* pickup *///	"Golan Arms Flechette",
;885:		100,
;886:		IT_WEAPON,
;887:		WP_FLECHETTE,
;888:/* precache */ "",
;889:/* sounds */ ""
;890:	},
;891:
;892:/*QUAKED weapon_rocket_launcher (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;893:*/
;894:	{
;895:		"weapon_rocket_launcher",
;896:		"sound/weapons/w_pkup.wav",
;897:        { "models/weapons2/merr_sonn/merr_sonn_w.glm", 
;898:		0, 0, 0},
;899:/* view */		"models/weapons2/merr_sonn/merr_sonn.md3", 
;900:/* icon */		"gfx/hud/w_icon_merrsonn",
;901:/* pickup *///	"Merr-Sonn Missile System",
;902:		3,
;903:		IT_WEAPON,
;904:		WP_ROCKET_LAUNCHER,
;905:/* precache */ "",
;906:/* sounds */ ""
;907:	},
;908:
;909:/*QUAKED ammo_thermal (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;910:*/
;911:	{
;912:		"ammo_thermal",
;913:		"sound/weapons/w_pkup.wav",
;914:        { "models/weapons2/thermal/thermal_pu.md3", 
;915:		"models/weapons2/thermal/thermal_w.glm", 0, 0},
;916:/* view */		"models/weapons2/thermal/thermal.md3", 
;917:/* icon */		"gfx/hud/w_icon_thermal",
;918:/* pickup *///	"Thermal Detonators",
;919:		4,
;920:		IT_AMMO,
;921:		AMMO_THERMAL,
;922:/* precache */ "",
;923:/* sounds */ ""
;924:	},
;925:
;926:/*QUAKED ammo_tripmine (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;927:*/
;928:	{
;929:		"ammo_tripmine", 
;930:		"sound/weapons/w_pkup.wav",
;931:        { "models/weapons2/laser_trap/laser_trap_pu.md3", 
;932:		"models/weapons2/laser_trap/laser_trap_w.glm", 0, 0},
;933:/* view */		"models/weapons2/laser_trap/laser_trap.md3", 
;934:/* icon */		"gfx/hud/w_icon_tripmine",
;935:/* pickup *///	"Trip Mines",
;936:		3,
;937:		IT_AMMO,
;938:		AMMO_TRIPMINE,
;939:/* precache */ "",
;940:/* sounds */ ""
;941:	},
;942:
;943:/*QUAKED ammo_detpack (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;944:*/
;945:	{
;946:		"ammo_detpack", 
;947:		"sound/weapons/w_pkup.wav",
;948:        { "models/weapons2/detpack/det_pack_pu.md3", "models/weapons2/detpack/det_pack_proj.glm", "models/weapons2/detpack/det_pack_w.glm", 0},
;949:/* view */		"models/weapons2/detpack/det_pack.md3", 
;950:/* icon */		"gfx/hud/w_icon_detpack",
;951:/* pickup *///	"Det Packs",
;952:		3,
;953:		IT_AMMO,
;954:		AMMO_DETPACK,
;955:/* precache */ "",
;956:/* sounds */ ""
;957:	},
;958:
;959:/*QUAKED weapon_thermal (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;960:*/
;961:	{
;962:		"weapon_thermal",
;963:		"sound/weapons/w_pkup.wav",
;964:        { "models/weapons2/thermal/thermal_w.glm", "models/weapons2/thermal/thermal_pu.md3",
;965:		0, 0 },
;966:/* view */		"models/weapons2/thermal/thermal.md3", 
;967:/* icon */		"gfx/hud/w_icon_thermal",
;968:/* pickup *///	"Thermal Detonator",
;969:		4,
;970:		IT_WEAPON,
;971:		WP_THERMAL,
;972:/* precache */ "",
;973:/* sounds */ ""
;974:	},
;975:
;976:/*QUAKED weapon_trip_mine (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;977:*/
;978:	{
;979:		"weapon_trip_mine", 
;980:		"sound/weapons/w_pkup.wav",
;981:        { "models/weapons2/laser_trap/laser_trap_w.glm", "models/weapons2/laser_trap/laser_trap_pu.md3",
;982:		0, 0},
;983:/* view */		"models/weapons2/laser_trap/laser_trap.md3", 
;984:/* icon */		"gfx/hud/w_icon_tripmine",
;985:/* pickup *///	"Trip Mine",
;986:		3,
;987:		IT_WEAPON,
;988:		WP_TRIP_MINE,
;989:/* precache */ "",
;990:/* sounds */ ""
;991:	},
;992:
;993:/*QUAKED weapon_det_pack (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;994:*/
;995:	{
;996:		"weapon_det_pack", 
;997:		"sound/weapons/w_pkup.wav",
;998:        { "models/weapons2/detpack/det_pack_proj.glm", "models/weapons2/detpack/det_pack_pu.md3", "models/weapons2/detpack/det_pack_w.glm", 0},
;999:/* view */		"models/weapons2/detpack/det_pack.md3", 
;1000:/* icon */		"gfx/hud/w_icon_detpack",
;1001:/* pickup *///	"Det Pack",
;1002:		3,
;1003:		IT_WEAPON,
;1004:		WP_DET_PACK,
;1005:/* precache */ "",
;1006:/* sounds */ ""
;1007:	},
;1008:
;1009:/*QUAKED weapon_emplaced (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;1010:*/
;1011:	{
;1012:		"weapon_emplaced", 
;1013:		"sound/weapons/w_pkup.wav",
;1014:        { "models/weapons2/blaster_r/blaster_w.glm", 
;1015:		0, 0, 0},
;1016:/* view */		"models/weapons2/blaster_r/blaster.md3", 
;1017:/* icon */		"gfx/hud/w_icon_blaster",
;1018:/* pickup *///	"Emplaced Gun",
;1019:		50,
;1020:		IT_WEAPON,
;1021:		WP_EMPLACED_GUN,
;1022:/* precache */ "",
;1023:/* sounds */ ""
;1024:	},
;1025:
;1026:
;1027://NOTE: This is to keep things from messing up because the turret weapon type isn't real
;1028:	{
;1029:		"weapon_turretwp", 
;1030:		"sound/weapons/w_pkup.wav",
;1031:        { "models/weapons2/blaster_r/blaster_w.glm", 
;1032:		0, 0, 0},
;1033:/* view */		"models/weapons2/blaster_r/blaster.md3", 
;1034:/* icon */		"gfx/hud/w_icon_blaster",
;1035:/* pickup *///	"Turret Gun",
;1036:		50,
;1037:		IT_WEAPON,
;1038:		WP_TURRET,
;1039:/* precache */ "",
;1040:/* sounds */ ""
;1041:	},
;1042:
;1043:	//
;1044:	// AMMO ITEMS
;1045:	//
;1046:
;1047:/*QUAKED ammo_force (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;1048:Don't place this
;1049:*/
;1050:	{
;1051:		"ammo_force",
;1052:		"sound/player/pickupenergy.wav",
;1053:        { "models/items/energy_cell.md3", 
;1054:		0, 0, 0},
;1055:/* view */		NULL,			
;1056:/* icon */		"gfx/hud/w_icon_blaster",
;1057:/* pickup *///	"Force??",
;1058:		100,
;1059:		IT_AMMO,
;1060:		AMMO_FORCE,
;1061:/* precache */ "",
;1062:/* sounds */ ""
;1063:	},
;1064:
;1065:/*QUAKED ammo_blaster (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;1066:Ammo for the Bryar and Blaster pistols.
;1067:*/
;1068:	{
;1069:		"ammo_blaster",
;1070:		"sound/player/pickupenergy.wav",
;1071:        { "models/items/energy_cell.md3", 
;1072:		0, 0, 0},
;1073:/* view */		NULL,			
;1074:/* icon */		"gfx/hud/i_icon_battery",
;1075:/* pickup *///	"Blaster Pack",
;1076:		100,
;1077:		IT_AMMO,
;1078:		AMMO_BLASTER,
;1079:/* precache */ "",
;1080:/* sounds */ ""
;1081:	},
;1082:
;1083:/*QUAKED ammo_powercell (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;1084:Ammo for Tenloss Disruptor, Wookie Bowcaster, and the Destructive Electro Magnetic Pulse (demp2 ) guns
;1085:*/
;1086:	{
;1087:		"ammo_powercell",
;1088:		"sound/player/pickupenergy.wav",
;1089:        { "models/items/power_cell.md3", 
;1090:		0, 0, 0},
;1091:/* view */		NULL,			
;1092:/* icon */		"gfx/mp/ammo_power_cell",
;1093:/* pickup *///	"Power Cell",
;1094:		100,
;1095:		IT_AMMO,
;1096:		AMMO_POWERCELL,
;1097:/* precache */ "",
;1098:/* sounds */ ""
;1099:	},
;1100:
;1101:/*QUAKED ammo_metallic_bolts (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;1102:Ammo for Imperial Heavy Repeater and the Golan Arms Flechette
;1103:*/
;1104:	{
;1105:		"ammo_metallic_bolts",
;1106:		"sound/player/pickupenergy.wav",
;1107:        { "models/items/metallic_bolts.md3", 
;1108:		0, 0, 0},
;1109:/* view */		NULL,			
;1110:/* icon */		"gfx/mp/ammo_metallic_bolts",
;1111:/* pickup *///	"Metallic Bolts",
;1112:		100,
;1113:		IT_AMMO,
;1114:		AMMO_METAL_BOLTS,
;1115:/* precache */ "",
;1116:/* sounds */ ""
;1117:	},
;1118:
;1119:/*QUAKED ammo_rockets (.3 .3 1) (-16 -16 -16) (16 16 16) suspended
;1120:Ammo for Merr-Sonn portable missile launcher
;1121:*/
;1122:	{
;1123:		"ammo_rockets",
;1124:		"sound/player/pickupenergy.wav",
;1125:        { "models/items/rockets.md3", 
;1126:		0, 0, 0},
;1127:/* view */		NULL,			
;1128:/* icon */		"gfx/mp/ammo_rockets",
;1129:/* pickup *///	"Rockets",
;1130:		3,
;1131:		IT_AMMO,
;1132:		AMMO_ROCKETS,
;1133:/* precache */ "",
;1134:/* sounds */ ""
;1135:	},
;1136:
;1137:
;1138:	//
;1139:	// POWERUP ITEMS
;1140:	//
;1141:/*QUAKED team_CTF_redflag (1 0 0) (-16 -16 -16) (16 16 16)
;1142:Only in CTF games
;1143:*/
;1144:	{
;1145:		"team_CTF_redflag",
;1146:		NULL,
;1147:        { "models/flags/r_flag.md3",
;1148:		"models/flags/r_flag_ysal.md3", 0, 0 },
;1149:/* view */		NULL,			
;1150:/* icon */		"gfx/hud/mpi_rflag",
;1151:/* pickup *///	"Red Flag",
;1152:		0,
;1153:		IT_TEAM,
;1154:		PW_REDFLAG,
;1155:/* precache */ "",
;1156:/* sounds */ ""
;1157:	},
;1158:
;1159:/*QUAKED team_CTF_blueflag (0 0 1) (-16 -16 -16) (16 16 16)
;1160:Only in CTF games
;1161:*/
;1162:	{
;1163:		"team_CTF_blueflag",
;1164:		NULL,
;1165:        { "models/flags/b_flag.md3",
;1166:		"models/flags/b_flag_ysal.md3", 0, 0 },
;1167:/* view */		NULL,			
;1168:/* icon */		"gfx/hud/mpi_bflag",
;1169:/* pickup *///	"Blue Flag",
;1170:		0,
;1171:		IT_TEAM,
;1172:		PW_BLUEFLAG,
;1173:/* precache */ "",
;1174:/* sounds */ ""
;1175:	},
;1176:
;1177:	//
;1178:	// PERSISTANT POWERUP ITEMS
;1179:	//
;1180:
;1181:	/*QUAKED team_CTF_neutralflag (0 0 1) (-16 -16 -16) (16 16 16)
;1182:Only in One Flag CTF games
;1183:*/
;1184:	{
;1185:		"team_CTF_neutralflag",
;1186:		NULL,
;1187:        { "models/flags/n_flag.md3",
;1188:		0, 0, 0 },
;1189:/* view */		NULL,			
;1190:/* icon */		"icons/iconf_neutral1",
;1191:/* pickup *///	"Neutral Flag",
;1192:		0,
;1193:		IT_TEAM,
;1194:		PW_NEUTRALFLAG,
;1195:/* precache */ "",
;1196:/* sounds */ ""
;1197:	},
;1198:
;1199:	{
;1200:		"item_redcube",
;1201:		"sound/player/pickupenergy.wav",
;1202:        { "models/powerups/orb/r_orb.md3",
;1203:		0, 0, 0 },
;1204:/* view */		NULL,			
;1205:/* icon */		"icons/iconh_rorb",
;1206:/* pickup *///	"Red Cube",
;1207:		0,
;1208:		IT_TEAM,
;1209:		0,
;1210:/* precache */ "",
;1211:/* sounds */ ""
;1212:	},
;1213:
;1214:	{
;1215:		"item_bluecube",
;1216:		"sound/player/pickupenergy.wav",
;1217:        { "models/powerups/orb/b_orb.md3",
;1218:		0, 0, 0 },
;1219:/* view */		NULL,			
;1220:/* icon */		"icons/iconh_borb",
;1221:/* pickup *///	"Blue Cube",
;1222:		0,
;1223:		IT_TEAM,
;1224:		0,
;1225:/* precache */ "",
;1226:/* sounds */ ""
;1227:	},
;1228:
;1229:	// end of list marker
;1230:	{NULL}
;1231:};
;1232:
;1233:int		bg_numItems = sizeof(bg_itemlist) / sizeof(bg_itemlist[0]) - 1;
;1234:
;1235:float vectoyaw( const vec3_t vec ) {
line 1238
;1236:	float	yaw;
;1237:	
;1238:	if (vec[YAW] == 0 && vec[PITCH] == 0) {
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
CNSTF4 0
ASGNF4
ADDRLP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 8
INDIRF4
NEF4 $396
ADDRLP4 4
INDIRP4
INDIRF4
ADDRLP4 8
INDIRF4
NEF4 $396
line 1239
;1239:		yaw = 0;
ADDRLP4 0
CNSTF4 0
ASGNF4
line 1240
;1240:	} else {
ADDRGP4 $397
JUMPV
LABELV $396
line 1241
;1241:		if (vec[PITCH]) {
ADDRFP4 0
INDIRP4
INDIRF4
CNSTF4 0
EQF4 $398
line 1242
;1242:			yaw = ( atan2( vec[YAW], vec[PITCH]) * 180 / M_PI );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
ADDRLP4 12
INDIRP4
INDIRF4
ARGF4
ADDRLP4 16
ADDRGP4 atan2
CALLF4
ASGNF4
ADDRLP4 0
CNSTF4 1127481344
ADDRLP4 16
INDIRF4
MULF4
CNSTF4 1078530011
DIVF4
ASGNF4
line 1243
;1243:		} else if (vec[YAW] > 0) {
ADDRGP4 $399
JUMPV
LABELV $398
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 0
LEF4 $400
line 1244
;1244:			yaw = 90;
ADDRLP4 0
CNSTF4 1119092736
ASGNF4
line 1245
;1245:		} else {
ADDRGP4 $401
JUMPV
LABELV $400
line 1246
;1246:			yaw = 270;
ADDRLP4 0
CNSTF4 1132920832
ASGNF4
line 1247
;1247:		}
LABELV $401
LABELV $399
line 1248
;1248:		if (yaw < 0) {
ADDRLP4 0
INDIRF4
CNSTF4 0
GEF4 $402
line 1249
;1249:			yaw += 360;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1135869952
ADDF4
ASGNF4
line 1250
;1250:		}
LABELV $402
line 1251
;1251:	}
LABELV $397
line 1253
;1252:
;1253:	return yaw;
ADDRLP4 0
INDIRF4
RETF4
LABELV $395
endproc vectoyaw 20 8
export BG_HasYsalamiri
proc BG_HasYsalamiri 8 0
line 1257
;1254:}
;1255:
;1256:qboolean BG_HasYsalamiri(int gametype, playerState_t *ps)
;1257:{
line 1258
;1258:	if (gametype == GT_CTY &&
ADDRFP4 0
INDIRI4
CNSTI4 8
NEI4 $405
ADDRLP4 0
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 360
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
NEI4 $407
ADDRLP4 0
INDIRP4
CNSTI4 364
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $405
LABELV $407
line 1260
;1259:		(ps->powerups[PW_REDFLAG] || ps->powerups[PW_BLUEFLAG]))
;1260:	{
line 1261
;1261:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $404
JUMPV
LABELV $405
line 1264
;1262:	}
;1263:
;1264:	if (ps->powerups[PW_YSALAMIRI])
ADDRFP4 4
INDIRP4
CNSTI4 404
ADDP4
INDIRI4
CNSTI4 0
EQI4 $408
line 1265
;1265:	{
line 1266
;1266:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $404
JUMPV
LABELV $408
line 1269
;1267:	}
;1268:
;1269:	return qfalse;
CNSTI4 0
RETI4
LABELV $404
endproc BG_HasYsalamiri 8 0
export BG_CanUseFPNow
proc BG_CanUseFPNow 8 8
line 1273
;1270:}
;1271:
;1272:qboolean BG_CanUseFPNow(int gametype, playerState_t *ps, int time, forcePowers_t power)
;1273:{
line 1274
;1274:	if (BG_HasYsalamiri(gametype, ps))
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 BG_HasYsalamiri
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $411
line 1275
;1275:	{
line 1276
;1276:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $410
JUMPV
LABELV $411
line 1279
;1277:	}
;1278:
;1279:	if (ps->duelInProgress)
ADDRFP4 4
INDIRP4
CNSTI4 1292
ADDP4
INDIRI4
CNSTI4 0
EQI4 $413
line 1280
;1280:	{
line 1281
;1281:		if (power != FP_SABERATTACK && power != FP_SABERDEFEND && power != FP_SABERTHROW &&
ADDRLP4 4
ADDRFP4 12
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 15
EQI4 $415
ADDRLP4 4
INDIRI4
CNSTI4 16
EQI4 $415
ADDRLP4 4
INDIRI4
CNSTI4 17
EQI4 $415
ADDRLP4 4
INDIRI4
CNSTI4 1
EQI4 $415
line 1283
;1282:			power != FP_LEVITATION)
;1283:		{
line 1284
;1284:			if (!ps->saberLockFrame || power != FP_PUSH)
ADDRFP4 4
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
CNSTI4 0
EQI4 $419
ADDRFP4 12
INDIRI4
CNSTI4 3
EQI4 $417
LABELV $419
line 1285
;1285:			{
line 1286
;1286:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $410
JUMPV
LABELV $417
line 1288
;1287:			}
;1288:		}
LABELV $415
line 1289
;1289:	}
LABELV $413
line 1291
;1290:
;1291:	if (ps->saberLockFrame || ps->saberLockTime > time)
ADDRLP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
CNSTI4 0
NEI4 $422
ADDRLP4 4
INDIRP4
CNSTI4 524
ADDP4
INDIRI4
ADDRFP4 8
INDIRI4
LEI4 $420
LABELV $422
line 1292
;1292:	{
line 1293
;1293:		if (power != FP_PUSH)
ADDRFP4 12
INDIRI4
CNSTI4 3
EQI4 $423
line 1294
;1294:		{
line 1295
;1295:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $410
JUMPV
LABELV $423
line 1297
;1296:		}
;1297:	}
LABELV $420
line 1299
;1298:
;1299:	if (ps->fallingToDeath)
ADDRFP4 4
INDIRP4
CNSTI4 1340
ADDP4
INDIRI4
CNSTI4 0
EQI4 $425
line 1300
;1300:	{
line 1301
;1301:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $410
JUMPV
LABELV $425
line 1304
;1302:	}
;1303:
;1304:	return qtrue;
CNSTI4 1
RETI4
LABELV $410
endproc BG_CanUseFPNow 8 8
export BG_FindItemForPowerup
proc BG_FindItemForPowerup 8 0
line 1312
;1305:}
;1306:
;1307:/*
;1308:==============
;1309:BG_FindItemForPowerup
;1310:==============
;1311:*/
;1312:gitem_t	*BG_FindItemForPowerup( powerup_t pw ) {
line 1315
;1313:	int		i;
;1314:
;1315:	for ( i = 0 ; i < bg_numItems ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $431
JUMPV
LABELV $428
line 1316
;1316:		if ( (bg_itemlist[i].giType == IT_POWERUP || 
ADDRLP4 4
CNSTI4 52
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 4
INDIRI4
ADDRGP4 bg_itemlist+36
ADDP4
INDIRI4
CNSTI4 5
EQI4 $437
ADDRLP4 4
INDIRI4
ADDRGP4 bg_itemlist+36
ADDP4
INDIRI4
CNSTI4 8
NEI4 $432
LABELV $437
CNSTI4 52
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 bg_itemlist+40
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $432
line 1318
;1317:					bg_itemlist[i].giType == IT_TEAM) && 
;1318:			bg_itemlist[i].giTag == pw ) {
line 1319
;1319:			return &bg_itemlist[i];
CNSTI4 52
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 bg_itemlist
ADDP4
RETP4
ADDRGP4 $427
JUMPV
LABELV $432
line 1321
;1320:		}
;1321:	}
LABELV $429
line 1315
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $431
ADDRLP4 0
INDIRI4
ADDRGP4 bg_numItems
INDIRI4
LTI4 $428
line 1323
;1322:
;1323:	return NULL;
CNSTP4 0
RETP4
LABELV $427
endproc BG_FindItemForPowerup 8 0
export BG_FindItemForHoldable
proc BG_FindItemForHoldable 8 8
line 1332
;1324:}
;1325:
;1326:
;1327:/*
;1328:==============
;1329:BG_FindItemForHoldable
;1330:==============
;1331:*/
;1332:gitem_t	*BG_FindItemForHoldable( holdable_t pw ) {
line 1335
;1333:	int		i;
;1334:
;1335:	for ( i = 0 ; i < bg_numItems ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $442
JUMPV
LABELV $439
line 1336
;1336:		if ( bg_itemlist[i].giType == IT_HOLDABLE && bg_itemlist[i].giTag == pw ) {
ADDRLP4 4
CNSTI4 52
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 4
INDIRI4
ADDRGP4 bg_itemlist+36
ADDP4
INDIRI4
CNSTI4 6
NEI4 $443
ADDRLP4 4
INDIRI4
ADDRGP4 bg_itemlist+40
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $443
line 1337
;1337:			return &bg_itemlist[i];
CNSTI4 52
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 bg_itemlist
ADDP4
RETP4
ADDRGP4 $438
JUMPV
LABELV $443
line 1339
;1338:		}
;1339:	}
LABELV $440
line 1335
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $442
ADDRLP4 0
INDIRI4
ADDRGP4 bg_numItems
INDIRI4
LTI4 $439
line 1341
;1340:
;1341:	Com_Error( ERR_DROP, "HoldableItem not found" );
CNSTI4 1
ARGI4
ADDRGP4 $447
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 1343
;1342:
;1343:	return NULL;
CNSTP4 0
RETP4
LABELV $438
endproc BG_FindItemForHoldable 8 8
export BG_FindItemForWeapon
proc BG_FindItemForWeapon 8 12
line 1353
;1344:}
;1345:
;1346:
;1347:/*
;1348:===============
;1349:BG_FindItemForWeapon
;1350:
;1351:===============
;1352:*/
;1353:gitem_t	*BG_FindItemForWeapon( weapon_t weapon ) {
line 1356
;1354:	gitem_t	*it;
;1355:	
;1356:	for ( it = bg_itemlist + 1 ; it->classname ; it++) {
ADDRLP4 0
ADDRGP4 bg_itemlist+52
ASGNP4
ADDRGP4 $452
JUMPV
LABELV $449
line 1357
;1357:		if ( it->giType == IT_WEAPON && it->giTag == weapon ) {
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
CNSTI4 1
NEI4 $454
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $454
line 1358
;1358:			return it;
ADDRLP4 0
INDIRP4
RETP4
ADDRGP4 $448
JUMPV
LABELV $454
line 1360
;1359:		}
;1360:	}
LABELV $450
line 1356
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 52
ADDP4
ASGNP4
LABELV $452
ADDRLP4 0
INDIRP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $449
line 1362
;1361:
;1362:	Com_Error( ERR_DROP, "Couldn't find item for weapon %i", weapon);
CNSTI4 1
ARGI4
ADDRGP4 $456
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 Com_Error
CALLV
pop
line 1363
;1363:	return NULL;
CNSTP4 0
RETP4
LABELV $448
endproc BG_FindItemForWeapon 8 12
export BG_FindItem
proc BG_FindItem 8 8
line 1372
;1364:}
;1365:
;1366:/*
;1367:===============
;1368:BG_FindItem
;1369:
;1370:===============
;1371:*/
;1372:gitem_t	*BG_FindItem( const char *classname ) {
line 1375
;1373:	gitem_t	*it;
;1374:	
;1375:	for ( it = bg_itemlist + 1 ; it->classname ; it++ ) {
ADDRLP4 0
ADDRGP4 bg_itemlist+52
ASGNP4
ADDRGP4 $461
JUMPV
LABELV $458
line 1376
;1376:		if ( !Q_stricmp( it->classname, classname) )
ADDRLP4 0
INDIRP4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $463
line 1377
;1377:			return it;
ADDRLP4 0
INDIRP4
RETP4
ADDRGP4 $457
JUMPV
LABELV $463
line 1378
;1378:	}
LABELV $459
line 1375
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 52
ADDP4
ASGNP4
LABELV $461
ADDRLP4 0
INDIRP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $458
line 1380
;1379:
;1380:	return NULL;
CNSTP4 0
RETP4
LABELV $457
endproc BG_FindItem 8 8
export BG_PlayerTouchesItem
proc BG_PlayerTouchesItem 36 12
line 1391
;1381:}
;1382:
;1383:/*
;1384:============
;1385:BG_PlayerTouchesItem
;1386:
;1387:Items can be picked up without actually touching their physical bounds to make
;1388:grabbing them easier
;1389:============
;1390:*/
;1391:qboolean	BG_PlayerTouchesItem( playerState_t *ps, entityState_t *item, int atTime ) {
line 1394
;1392:	vec3_t		origin;
;1393:
;1394:	BG_EvaluateTrajectory( &item->pos, atTime, origin );
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
ARGP4
ADDRFP4 8
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BG_EvaluateTrajectory
CALLV
pop
line 1397
;1395:
;1396:	// we are ignoring ducked differences here
;1397:	if ( ps->origin[0] - origin[0] > 44
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
ADDRLP4 12
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
SUBF4
ASGNF4
ADDRLP4 16
INDIRF4
CNSTF4 1110441984
GTF4 $476
ADDRLP4 16
INDIRF4
CNSTF4 3259498496
LTF4 $476
ADDRLP4 20
ADDRLP4 12
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ASGNF4
ADDRLP4 24
CNSTF4 1108344832
ASGNF4
ADDRLP4 20
INDIRF4
ADDRLP4 0+4
INDIRF4
SUBF4
ADDRLP4 24
INDIRF4
GTF4 $476
ADDRLP4 28
CNSTF4 3255828480
ASGNF4
ADDRLP4 20
INDIRF4
ADDRLP4 0+4
INDIRF4
SUBF4
ADDRLP4 28
INDIRF4
LTF4 $476
ADDRLP4 32
ADDRLP4 12
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ASGNF4
ADDRLP4 32
INDIRF4
ADDRLP4 0+8
INDIRF4
SUBF4
ADDRLP4 24
INDIRF4
GTF4 $476
ADDRLP4 32
INDIRF4
ADDRLP4 0+8
INDIRF4
SUBF4
ADDRLP4 28
INDIRF4
GEF4 $466
LABELV $476
line 1402
;1398:		|| ps->origin[0] - origin[0] < -50
;1399:		|| ps->origin[1] - origin[1] > 36
;1400:		|| ps->origin[1] - origin[1] < -36
;1401:		|| ps->origin[2] - origin[2] > 36
;1402:		|| ps->origin[2] - origin[2] < -36 ) {
line 1403
;1403:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $465
JUMPV
LABELV $466
line 1406
;1404:	}
;1405:
;1406:	return qtrue;
CNSTI4 1
RETI4
LABELV $465
endproc BG_PlayerTouchesItem 36 12
export BG_ProperForceIndex
proc BG_ProperForceIndex 4 0
line 1410
;1407:}
;1408:
;1409:int BG_ProperForceIndex(int power)
;1410:{
line 1411
;1411:	int i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $479
JUMPV
LABELV $478
line 1414
;1412:
;1413:	while (i < NUM_FORCE_POWERS)
;1414:	{
line 1415
;1415:		if (forcePowerSorted[i] == power)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 forcePowerSorted
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $481
line 1416
;1416:		{
line 1417
;1417:			return i;
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $477
JUMPV
LABELV $481
line 1420
;1418:		}
;1419:
;1420:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1421
;1421:	}
LABELV $479
line 1413
ADDRLP4 0
INDIRI4
CNSTI4 18
LTI4 $478
line 1423
;1422:
;1423:	return -1;
CNSTI4 -1
RETI4
LABELV $477
endproc BG_ProperForceIndex 4 0
export BG_CycleForce
proc BG_CycleForce 40 4
line 1427
;1424:}
;1425:
;1426:void BG_CycleForce(playerState_t *ps, int direction)
;1427:{
line 1428
;1428:	int i = ps->fd.forcePowerSelected;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 840
ADDP4
INDIRI4
ASGNI4
line 1429
;1429:	int x = i;
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 1430
;1430:	int presel = i;
ADDRLP4 8
ADDRLP4 4
INDIRI4
ASGNI4
line 1431
;1431:	int foundnext = -1;
ADDRLP4 12
CNSTI4 -1
ASGNI4
line 1433
;1432:
;1433:	if (!ps->fd.forcePowersKnown & (1 << x) ||
ADDRFP4 0
INDIRP4
CNSTI4 832
ADDP4
INDIRI4
CNSTI4 0
NEI4 $488
ADDRLP4 16
CNSTI4 1
ASGNI4
ADDRGP4 $489
JUMPV
LABELV $488
ADDRLP4 16
CNSTI4 0
ASGNI4
LABELV $489
ADDRLP4 16
INDIRI4
CNSTI4 1
ADDRLP4 0
INDIRI4
LSHI4
BANDI4
CNSTI4 0
NEI4 $490
ADDRLP4 0
INDIRI4
CNSTI4 18
GEI4 $490
ADDRLP4 0
INDIRI4
CNSTI4 -1
NEI4 $484
LABELV $490
line 1436
;1434:		x >= NUM_FORCE_POWERS ||
;1435:		x == -1)
;1436:	{ //apparently we have no valid force powers
line 1437
;1437:		return;
ADDRGP4 $483
JUMPV
LABELV $484
line 1440
;1438:	}
;1439:
;1440:	x = BG_ProperForceIndex(x);
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 24
ADDRGP4 BG_ProperForceIndex
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 24
INDIRI4
ASGNI4
line 1441
;1441:	presel = x;
ADDRLP4 8
ADDRLP4 0
INDIRI4
ASGNI4
line 1443
;1442:
;1443:	if (direction == 1)
ADDRFP4 4
INDIRI4
CNSTI4 1
NEI4 $491
line 1444
;1444:	{
line 1445
;1445:		x++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1446
;1446:	}
ADDRGP4 $492
JUMPV
LABELV $491
line 1448
;1447:	else
;1448:	{
line 1449
;1449:		x--;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1450
;1450:	}
LABELV $492
line 1452
;1451:
;1452:	if (x >= NUM_FORCE_POWERS)
ADDRLP4 0
INDIRI4
CNSTI4 18
LTI4 $493
line 1453
;1453:	{
line 1454
;1454:		x = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1455
;1455:	}
LABELV $493
line 1456
;1456:	if (x < 0)
ADDRLP4 0
INDIRI4
CNSTI4 0
GEI4 $495
line 1457
;1457:	{
line 1458
;1458:		x = NUM_FORCE_POWERS-1;
ADDRLP4 0
CNSTI4 17
ASGNI4
line 1459
;1459:	}
LABELV $495
line 1461
;1460:
;1461:	i = forcePowerSorted[x];
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 forcePowerSorted
ADDP4
INDIRI4
ASGNI4
ADDRGP4 $498
JUMPV
LABELV $497
line 1464
;1462:
;1463:	while (x != presel)
;1464:	{
line 1465
;1465:		if (ps->fd.forcePowersKnown & (1 << i) && i != ps->fd.forcePowerSelected)
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 832
ADDP4
INDIRI4
CNSTI4 1
ADDRLP4 4
INDIRI4
LSHI4
BANDI4
CNSTI4 0
EQI4 $500
ADDRLP4 4
INDIRI4
ADDRLP4 28
INDIRP4
CNSTI4 840
ADDP4
INDIRI4
EQI4 $500
line 1466
;1466:		{
line 1467
;1467:			if (i != FP_LEVITATION &&
ADDRLP4 4
INDIRI4
CNSTI4 1
EQI4 $502
ADDRLP4 4
INDIRI4
CNSTI4 15
EQI4 $502
ADDRLP4 4
INDIRI4
CNSTI4 16
EQI4 $502
ADDRLP4 4
INDIRI4
CNSTI4 17
EQI4 $502
line 1471
;1468:				i != FP_SABERATTACK &&
;1469:				i != FP_SABERDEFEND &&
;1470:				i != FP_SABERTHROW)
;1471:			{
line 1472
;1472:				foundnext = i;
ADDRLP4 12
ADDRLP4 4
INDIRI4
ASGNI4
line 1473
;1473:				break;
ADDRGP4 $499
JUMPV
LABELV $502
line 1475
;1474:			}
;1475:		}
LABELV $500
line 1477
;1476:
;1477:		if (direction == 1)
ADDRFP4 4
INDIRI4
CNSTI4 1
NEI4 $504
line 1478
;1478:		{
line 1479
;1479:			x++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1480
;1480:		}
ADDRGP4 $505
JUMPV
LABELV $504
line 1482
;1481:		else
;1482:		{
line 1483
;1483:			x--;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1484
;1484:		}
LABELV $505
line 1486
;1485:	
;1486:		if (x >= NUM_FORCE_POWERS)
ADDRLP4 0
INDIRI4
CNSTI4 18
LTI4 $506
line 1487
;1487:		{
line 1488
;1488:			x = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1489
;1489:		}
LABELV $506
line 1490
;1490:		if (x < 0)
ADDRLP4 0
INDIRI4
CNSTI4 0
GEI4 $508
line 1491
;1491:		{
line 1492
;1492:			x = NUM_FORCE_POWERS-1;
ADDRLP4 0
CNSTI4 17
ASGNI4
line 1493
;1493:		}
LABELV $508
line 1495
;1494:
;1495:		i = forcePowerSorted[x];
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 forcePowerSorted
ADDP4
INDIRI4
ASGNI4
line 1496
;1496:	}
LABELV $498
line 1463
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
NEI4 $497
LABELV $499
line 1498
;1497:
;1498:	if (foundnext != -1)
ADDRLP4 12
INDIRI4
CNSTI4 -1
EQI4 $510
line 1499
;1499:	{
line 1500
;1500:		ps->fd.forcePowerSelected = foundnext;
ADDRFP4 0
INDIRP4
CNSTI4 840
ADDP4
ADDRLP4 12
INDIRI4
ASGNI4
line 1501
;1501:	}
LABELV $510
line 1502
;1502:}
LABELV $483
endproc BG_CycleForce 40 4
export BG_GetItemIndexByTag
proc BG_GetItemIndexByTag 8 0
line 1505
;1503:
;1504:int BG_GetItemIndexByTag(int tag, int type)
;1505:{
line 1506
;1506:	int i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $514
JUMPV
LABELV $513
line 1509
;1507:
;1508:	while (i < bg_numItems)
;1509:	{
line 1510
;1510:		if (bg_itemlist[i].giTag == tag &&
ADDRLP4 4
CNSTI4 52
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 4
INDIRI4
ADDRGP4 bg_itemlist+40
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $516
ADDRLP4 4
INDIRI4
ADDRGP4 bg_itemlist+36
ADDP4
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $516
line 1512
;1511:			bg_itemlist[i].giType == type)
;1512:		{
line 1513
;1513:			return i;
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $512
JUMPV
LABELV $516
line 1516
;1514:		}
;1515:
;1516:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1517
;1517:	}
LABELV $514
line 1508
ADDRLP4 0
INDIRI4
ADDRGP4 bg_numItems
INDIRI4
LTI4 $513
line 1519
;1518:
;1519:	return 0;
CNSTI4 0
RETI4
LABELV $512
endproc BG_GetItemIndexByTag 8 0
export BG_CycleInven
proc BG_CycleInven 12 8
line 1523
;1520:}
;1521:
;1522:void BG_CycleInven(playerState_t *ps, int direction)
;1523:{
line 1527
;1524:	int i;
;1525:	int original;
;1526:
;1527:	i = bg_itemlist[ps->stats[STAT_HOLDABLE_ITEM]].giTag;
ADDRLP4 0
CNSTI4 52
ADDRFP4 0
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
MULI4
ADDRGP4 bg_itemlist+40
ADDP4
INDIRI4
ASGNI4
line 1528
;1528:	original = i;
ADDRLP4 4
ADDRLP4 0
INDIRI4
ASGNI4
line 1530
;1529:
;1530:	if (direction == 1)
ADDRFP4 4
INDIRI4
CNSTI4 1
NEI4 $522
line 1531
;1531:	{
line 1532
;1532:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1533
;1533:	}
ADDRGP4 $525
JUMPV
LABELV $522
line 1535
;1534:	else
;1535:	{
line 1536
;1536:		i--;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1537
;1537:	}
ADDRGP4 $525
JUMPV
LABELV $524
line 1540
;1538:
;1539:	while (i != original)
;1540:	{ //go in a full loop until hitting something, if hit nothing then select nothing
line 1541
;1541:		if (ps->stats[STAT_HOLDABLE_ITEMS] & (1 << i))
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
INDIRI4
CNSTI4 1
ADDRLP4 0
INDIRI4
LSHI4
BANDI4
CNSTI4 0
EQI4 $527
line 1542
;1542:		{
line 1543
;1543:			ps->stats[STAT_HOLDABLE_ITEM] = BG_GetItemIndexByTag(i, IT_HOLDABLE);
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 6
ARGI4
ADDRLP4 8
ADDRGP4 BG_GetItemIndexByTag
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 220
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 1544
;1544:			break;
ADDRGP4 $526
JUMPV
LABELV $527
line 1547
;1545:		}
;1546:
;1547:		if (direction == 1)
ADDRFP4 4
INDIRI4
CNSTI4 1
NEI4 $529
line 1548
;1548:		{
line 1549
;1549:			i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1550
;1550:		}
ADDRGP4 $530
JUMPV
LABELV $529
line 1552
;1551:		else
;1552:		{
line 1553
;1553:			i--;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 1554
;1554:		}
LABELV $530
line 1556
;1555:
;1556:		if (i < 0)
ADDRLP4 0
INDIRI4
CNSTI4 0
GEI4 $531
line 1557
;1557:		{
line 1558
;1558:			i = HI_NUM_HOLDABLE;
ADDRLP4 0
CNSTI4 7
ASGNI4
line 1559
;1559:		}
ADDRGP4 $532
JUMPV
LABELV $531
line 1560
;1560:		else if (i >= HI_NUM_HOLDABLE)
ADDRLP4 0
INDIRI4
CNSTI4 7
LTI4 $533
line 1561
;1561:		{
line 1562
;1562:			i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1563
;1563:		}
LABELV $533
LABELV $532
line 1564
;1564:	}
LABELV $525
line 1539
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
NEI4 $524
LABELV $526
line 1565
;1565:}
LABELV $520
endproc BG_CycleInven 12 8
export BG_CanItemBeGrabbed
proc BG_CanItemBeGrabbed 76 8
line 1575
;1566:
;1567:/*
;1568:================
;1569:BG_CanItemBeGrabbed
;1570:
;1571:Returns false if the item should not be picked up.
;1572:This needs to be the same for client side prediction and server use.
;1573:================
;1574:*/
;1575:qboolean BG_CanItemBeGrabbed( int gametype, const entityState_t *ent, const playerState_t *ps ) {
line 1578
;1576:	gitem_t	*item;
;1577:
;1578:	if ( ent->modelindex < 1 || ent->modelindex >= bg_numItems ) {
ADDRLP4 4
ADDRFP4 4
INDIRP4
CNSTI4 212
ADDP4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 1
LTI4 $538
ADDRLP4 4
INDIRI4
ADDRGP4 bg_numItems
INDIRI4
LTI4 $536
LABELV $538
line 1579
;1579:		Com_Error( ERR_DROP, "BG_CanItemBeGrabbed: index out of range" );
CNSTI4 1
ARGI4
ADDRGP4 $539
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 1580
;1580:	}
LABELV $536
line 1582
;1581:
;1582:	item = &bg_itemlist[ent->modelindex];
ADDRLP4 0
CNSTI4 52
ADDRFP4 4
INDIRP4
CNSTI4 212
ADDP4
INDIRI4
MULI4
ADDRGP4 bg_itemlist
ADDP4
ASGNP4
line 1584
;1583:
;1584:	if (ps && ps->isJediMaster && item && (item->giType == IT_WEAPON || item->giType == IT_AMMO))
ADDRLP4 8
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 12
CNSTU4 0
ASGNU4
ADDRLP4 8
INDIRP4
CVPU4 4
ADDRLP4 12
INDIRU4
EQU4 $540
ADDRLP4 8
INDIRP4
CNSTI4 604
ADDP4
INDIRI4
CNSTI4 0
EQI4 $540
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 12
INDIRU4
EQU4 $540
ADDRLP4 20
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 1
EQI4 $542
ADDRLP4 20
INDIRI4
CNSTI4 2
NEI4 $540
LABELV $542
line 1585
;1585:	{
line 1586
;1586:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $535
JUMPV
LABELV $540
line 1589
;1587:	}
;1588:
;1589:	if (ps && ps->duelInProgress)
ADDRLP4 24
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $543
ADDRLP4 24
INDIRP4
CNSTI4 1292
ADDP4
INDIRI4
CNSTI4 0
EQI4 $543
line 1590
;1590:	{ //no picking stuff up while in a duel, no matter what the type is
line 1591
;1591:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $535
JUMPV
LABELV $543
line 1594
;1592:	}
;1593:
;1594:	switch( item->giType ) {
ADDRLP4 28
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
LTI4 $546
ADDRLP4 28
INDIRI4
CNSTI4 8
GTI4 $546
ADDRLP4 28
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $597
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $597
address $595
address $548
address $553
address $556
address $559
address $569
address $592
address $546
address $574
code
LABELV $548
line 1596
;1595:	case IT_WEAPON:
;1596:		if (ent->generic1 == ps->clientNum && ent->powerups)
ADDRLP4 36
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
ADDRFP4 8
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
NEI4 $549
ADDRLP4 36
INDIRP4
CNSTI4 272
ADDP4
INDIRI4
CNSTI4 0
EQI4 $549
line 1597
;1597:		{
line 1598
;1598:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $535
JUMPV
LABELV $549
line 1600
;1599:		}
;1600:		if (!(ent->eFlags & EF_DROPPEDWEAPON) && (ps->stats[STAT_WEAPONS] & (1 << item->giTag)) &&
ADDRLP4 40
CNSTI4 0
ASGNI4
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 16777216
BANDI4
ADDRLP4 40
INDIRI4
NEI4 $551
ADDRLP4 44
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ASGNI4
ADDRFP4 8
INDIRP4
CNSTI4 232
ADDP4
INDIRI4
CNSTI4 1
ADDRLP4 44
INDIRI4
LSHI4
BANDI4
ADDRLP4 40
INDIRI4
EQI4 $551
ADDRLP4 44
INDIRI4
CNSTI4 11
EQI4 $551
ADDRLP4 44
INDIRI4
CNSTI4 12
EQI4 $551
ADDRLP4 44
INDIRI4
CNSTI4 13
EQI4 $551
line 1602
;1601:			item->giTag != WP_THERMAL && item->giTag != WP_TRIP_MINE && item->giTag != WP_DET_PACK)
;1602:		{ //weaponstay stuff.. if this isn't dropped, and you already have it, you don't get it.
line 1603
;1603:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $535
JUMPV
LABELV $551
line 1605
;1604:		}
;1605:		return qtrue;	// weapons are always picked up
CNSTI4 1
RETI4
ADDRGP4 $535
JUMPV
LABELV $553
line 1608
;1606:
;1607:	case IT_AMMO:
;1608:		if ( ps->ammo[item->giTag] >= ammoData[item->giTag].max) {
ADDRLP4 48
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 48
INDIRI4
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
ADDP4
INDIRI4
ADDRLP4 48
INDIRI4
ADDRGP4 ammoData
ADDP4
INDIRI4
LTI4 $554
line 1609
;1609:			return qfalse;		// can't hold any more
CNSTI4 0
RETI4
ADDRGP4 $535
JUMPV
LABELV $554
line 1611
;1610:		}
;1611:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $535
JUMPV
LABELV $556
line 1614
;1612:
;1613:	case IT_ARMOR:
;1614:		if ( ps->stats[STAT_ARMOR] >= ps->stats[STAT_MAX_HEALTH] * item->giTag ) {
ADDRLP4 52
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 236
ADDP4
INDIRI4
ADDRLP4 52
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
MULI4
LTI4 $557
line 1615
;1615:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $535
JUMPV
LABELV $557
line 1617
;1616:		}
;1617:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $535
JUMPV
LABELV $559
line 1622
;1618:
;1619:	case IT_HEALTH:
;1620:		// small and mega healths will go over the max, otherwise
;1621:		// don't pick up if already at max
;1622:		if ((ps->fd.forcePowersActive & (1 << FP_RAGE)))
ADDRFP4 8
INDIRP4
CNSTI4 836
ADDP4
INDIRI4
CNSTI4 256
BANDI4
CNSTI4 0
EQI4 $560
line 1623
;1623:		{
line 1624
;1624:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $535
JUMPV
LABELV $560
line 1627
;1625:		}
;1626:
;1627:		if ( item->quantity == 5 || item->quantity == 100 ) {
ADDRLP4 56
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 5
EQI4 $564
ADDRLP4 56
INDIRI4
CNSTI4 100
NEI4 $562
LABELV $564
line 1628
;1628:			if ( ps->stats[STAT_HEALTH] >= ps->stats[STAT_MAX_HEALTH] * 2 ) {
ADDRLP4 60
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
ADDRLP4 60
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
CNSTI4 1
LSHI4
LTI4 $565
line 1629
;1629:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $535
JUMPV
LABELV $565
line 1631
;1630:			}
;1631:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $535
JUMPV
LABELV $562
line 1634
;1632:		}
;1633:
;1634:		if ( ps->stats[STAT_HEALTH] >= ps->stats[STAT_MAX_HEALTH] ) {
ADDRLP4 60
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
ADDRLP4 60
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
LTI4 $567
line 1635
;1635:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $535
JUMPV
LABELV $567
line 1637
;1636:		}
;1637:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $535
JUMPV
LABELV $569
line 1640
;1638:
;1639:	case IT_POWERUP:
;1640:		if (ps && (ps->powerups[PW_YSALAMIRI]))
ADDRLP4 64
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $570
ADDRLP4 64
INDIRP4
CNSTI4 404
ADDP4
INDIRI4
CNSTI4 0
EQI4 $570
line 1641
;1641:		{
line 1642
;1642:			if (item->giTag != PW_YSALAMIRI)
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 15
EQI4 $572
line 1643
;1643:			{
line 1644
;1644:				return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $535
JUMPV
LABELV $572
line 1646
;1645:			}
;1646:		}
LABELV $570
line 1647
;1647:		return qtrue;	// powerups are always picked up
CNSTI4 1
RETI4
ADDRGP4 $535
JUMPV
LABELV $574
line 1650
;1648:
;1649:	case IT_TEAM: // team items, such as flags
;1650:		if( gametype == GT_CTF || gametype == GT_CTY ) {
ADDRLP4 68
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 7
EQI4 $577
ADDRLP4 68
INDIRI4
CNSTI4 8
NEI4 $575
LABELV $577
line 1654
;1651:			// ent->modelindex2 is non-zero on items if they are dropped
;1652:			// we need to know this because we can pick up our dropped flag (and return it)
;1653:			// but we can't pick up our flag at base
;1654:			if (ps->persistant[PERS_TEAM] == TEAM_RED) {
ADDRFP4 8
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
CNSTI4 1
NEI4 $578
line 1655
;1655:				if (item->giTag == PW_BLUEFLAG ||
ADDRLP4 72
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 5
EQI4 $583
ADDRLP4 72
INDIRI4
CNSTI4 4
NEI4 $584
ADDRFP4 4
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CNSTI4 0
NEI4 $583
LABELV $584
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 4
NEI4 $579
ADDRFP4 8
INDIRP4
CNSTI4 364
ADDP4
INDIRI4
CNSTI4 0
EQI4 $579
LABELV $583
line 1658
;1656:					(item->giTag == PW_REDFLAG && ent->modelindex2) ||
;1657:					(item->giTag == PW_REDFLAG && ps->powerups[PW_BLUEFLAG]) )
;1658:					return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $535
JUMPV
line 1659
;1659:			} else if (ps->persistant[PERS_TEAM] == TEAM_BLUE) {
LABELV $578
ADDRFP4 8
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
CNSTI4 2
NEI4 $585
line 1660
;1660:				if (item->giTag == PW_REDFLAG ||
ADDRLP4 72
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 4
EQI4 $590
ADDRLP4 72
INDIRI4
CNSTI4 5
NEI4 $591
ADDRFP4 4
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CNSTI4 0
NEI4 $590
LABELV $591
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 5
NEI4 $587
ADDRFP4 8
INDIRP4
CNSTI4 360
ADDP4
INDIRI4
CNSTI4 0
EQI4 $587
LABELV $590
line 1663
;1661:					(item->giTag == PW_BLUEFLAG && ent->modelindex2) ||
;1662:					(item->giTag == PW_BLUEFLAG && ps->powerups[PW_REDFLAG]) )
;1663:					return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $535
JUMPV
LABELV $587
line 1664
;1664:			}
LABELV $585
LABELV $579
line 1665
;1665:		}
LABELV $575
line 1667
;1666:
;1667:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $535
JUMPV
LABELV $592
line 1670
;1668:
;1669:	case IT_HOLDABLE:
;1670:		if ( ps->stats[STAT_HOLDABLE_ITEMS] & (1 << item->giTag))
ADDRFP4 8
INDIRP4
CNSTI4 224
ADDP4
INDIRI4
CNSTI4 1
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
LSHI4
BANDI4
CNSTI4 0
EQI4 $593
line 1671
;1671:		{
line 1672
;1672:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $535
JUMPV
LABELV $593
line 1674
;1673:		}
;1674:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $535
JUMPV
LABELV $595
line 1677
;1675:
;1676:        case IT_BAD:
;1677:            Com_Error( ERR_DROP, "BG_CanItemBeGrabbed: IT_BAD" );
CNSTI4 1
ARGI4
ADDRGP4 $596
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 1684
;1678:        default:
;1679:#ifndef Q3_VM
;1680:#ifndef NDEBUG // bk0001204
;1681:          Com_Printf("BG_CanItemBeGrabbed: unknown enum %d\n", item->giType );
;1682:#endif
;1683:#endif
;1684:         break;
LABELV $546
line 1687
;1685:	}
;1686:
;1687:	return qfalse;
CNSTI4 0
RETI4
LABELV $535
endproc BG_CanItemBeGrabbed 76 8
export BG_EvaluateTrajectory
proc BG_EvaluateTrajectory 84 12
line 1698
;1688:}
;1689:
;1690://======================================================================
;1691:
;1692:/*
;1693:================
;1694:BG_EvaluateTrajectory
;1695:
;1696:================
;1697:*/
;1698:void BG_EvaluateTrajectory( const trajectory_t *tr, int atTime, vec3_t result ) {
line 1702
;1699:	float		deltaTime;
;1700:	float		phase;
;1701:
;1702:	switch( tr->trType ) {
ADDRLP4 8
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
LTI4 $599
ADDRLP4 8
INDIRI4
CNSTI4 5
GTI4 $599
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $612
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $612
address $602
address $602
address $603
address $605
address $604
address $610
code
LABELV $602
line 1705
;1703:	case TR_STATIONARY:
;1704:	case TR_INTERPOLATE:
;1705:		VectorCopy( tr->trBase, result );
ADDRFP4 8
INDIRP4
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRB
ASGNB 12
line 1706
;1706:		break;
ADDRGP4 $600
JUMPV
LABELV $603
line 1708
;1707:	case TR_LINEAR:
;1708:		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
ADDRLP4 0
CNSTF4 981668463
ADDRFP4 4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
SUBI4
CVIF4 4
MULF4
ASGNF4
line 1709
;1709:		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
INDIRP4
ADDRLP4 16
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDRLP4 16
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 20
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ADDRLP4 20
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 24
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 24
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 1710
;1710:		break;
ADDRGP4 $600
JUMPV
LABELV $604
line 1712
;1711:	case TR_SINE:
;1712:		deltaTime = ( atTime - tr->trTime ) / (float) tr->trDuration;
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRFP4 4
INDIRI4
ADDRLP4 28
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
SUBI4
CVIF4 4
ADDRLP4 28
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CVIF4 4
DIVF4
ASGNF4
line 1713
;1713:		phase = sin( deltaTime * M_PI * 2 );
CNSTF4 1073741824
CNSTF4 1078530011
ADDRLP4 0
INDIRF4
MULF4
MULF4
ARGF4
ADDRLP4 32
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 4
ADDRLP4 32
INDIRF4
ASGNF4
line 1714
;1714:		VectorMA( tr->trBase, phase, tr->trDelta, result );
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
INDIRP4
ADDRLP4 36
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDRLP4 36
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 40
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 44
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 44
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDRLP4 4
INDIRF4
MULF4
ADDF4
ASGNF4
line 1715
;1715:		break;
ADDRGP4 $600
JUMPV
LABELV $605
line 1717
;1716:	case TR_LINEAR_STOP:
;1717:		if ( atTime > tr->trTime + tr->trDuration ) {
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 4
INDIRI4
ADDRLP4 48
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRLP4 48
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ADDI4
LEI4 $606
line 1718
;1718:			atTime = tr->trTime + tr->trDuration;
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 4
ADDRLP4 52
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRLP4 52
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1719
;1719:		}
LABELV $606
line 1720
;1720:		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
ADDRLP4 0
CNSTF4 981668463
ADDRFP4 4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
SUBI4
CVIF4 4
MULF4
ASGNF4
line 1721
;1721:		if ( deltaTime < 0 ) {
ADDRLP4 0
INDIRF4
CNSTF4 0
GEF4 $608
line 1722
;1722:			deltaTime = 0;
ADDRLP4 0
CNSTF4 0
ASGNF4
line 1723
;1723:		}
LABELV $608
line 1724
;1724:		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
INDIRP4
ADDRLP4 52
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDRLP4 52
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 56
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ADDRLP4 56
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 60
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 60
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 1725
;1725:		break;
ADDRGP4 $600
JUMPV
LABELV $610
line 1727
;1726:	case TR_GRAVITY:
;1727:		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
ADDRLP4 0
CNSTF4 981668463
ADDRFP4 4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
SUBI4
CVIF4 4
MULF4
ASGNF4
line 1728
;1728:		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
ADDRLP4 64
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
INDIRP4
ADDRLP4 64
INDIRP4
CNSTI4 12
ADDP4
INDIRF4
ADDRLP4 64
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 68
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 68
INDIRP4
CNSTI4 16
ADDP4
INDIRF4
ADDRLP4 68
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 72
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 8
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 72
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 72
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 1729
;1729:		result[2] -= 0.5 * DEFAULT_GRAVITY * deltaTime * deltaTime;		// FIXME: local gravity...
ADDRLP4 76
ADDRFP4 8
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 80
ADDRLP4 0
INDIRF4
ASGNF4
ADDRLP4 76
INDIRP4
ADDRLP4 76
INDIRP4
INDIRF4
CNSTF4 1137180672
ADDRLP4 80
INDIRF4
MULF4
ADDRLP4 80
INDIRF4
MULF4
SUBF4
ASGNF4
line 1730
;1730:		break;
ADDRGP4 $600
JUMPV
LABELV $599
line 1735
;1731:	default:
;1732:#ifdef QAGAME
;1733:		Com_Error( ERR_DROP, "BG_EvaluateTrajectory: [GAME SIDE] unknown trType: %i", tr->trType );
;1734:#else
;1735:		Com_Error( ERR_DROP, "BG_EvaluateTrajectory: [CLIENTGAME SIDE] unknown trType: %i", tr->trType );
CNSTI4 1
ARGI4
ADDRGP4 $611
ARGP4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
ADDRGP4 Com_Error
CALLV
pop
line 1737
;1736:#endif
;1737:		break;
LABELV $600
line 1739
;1738:	}
;1739:}
LABELV $598
endproc BG_EvaluateTrajectory 84 12
export BG_EvaluateTrajectoryDelta
proc BG_EvaluateTrajectoryDelta 44 12
line 1748
;1740:
;1741:/*
;1742:================
;1743:BG_EvaluateTrajectoryDelta
;1744:
;1745:For determining velocity at a given time
;1746:================
;1747:*/
;1748:void BG_EvaluateTrajectoryDelta( const trajectory_t *tr, int atTime, vec3_t result ) {
line 1752
;1749:	float	deltaTime;
;1750:	float	phase;
;1751:
;1752:	switch( tr->trType ) {
ADDRLP4 8
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
LTI4 $614
ADDRLP4 8
INDIRI4
CNSTI4 5
GTI4 $614
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $625
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $625
address $617
address $617
address $618
address $620
address $619
address $623
code
LABELV $617
line 1755
;1753:	case TR_STATIONARY:
;1754:	case TR_INTERPOLATE:
;1755:		VectorClear( result );
ADDRLP4 16
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 20
CNSTF4 0
ASGNF4
ADDRLP4 16
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 16
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 16
INDIRP4
ADDRLP4 20
INDIRF4
ASGNF4
line 1756
;1756:		break;
ADDRGP4 $615
JUMPV
LABELV $618
line 1758
;1757:	case TR_LINEAR:
;1758:		VectorCopy( tr->trDelta, result );
ADDRFP4 8
INDIRP4
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 1759
;1759:		break;
ADDRGP4 $615
JUMPV
LABELV $619
line 1761
;1760:	case TR_SINE:
;1761:		deltaTime = ( atTime - tr->trTime ) / (float) tr->trDuration;
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRFP4 4
INDIRI4
ADDRLP4 24
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
SUBI4
CVIF4 4
ADDRLP4 24
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CVIF4 4
DIVF4
ASGNF4
line 1762
;1762:		phase = cos( deltaTime * M_PI * 2 );	// derivative of sin = cos
CNSTF4 1073741824
CNSTF4 1078530011
ADDRLP4 4
INDIRF4
MULF4
MULF4
ARGF4
ADDRLP4 28
ADDRGP4 cos
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 28
INDIRF4
ASGNF4
line 1763
;1763:		phase *= 0.5;
ADDRLP4 0
CNSTF4 1056964608
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
line 1764
;1764:		VectorScale( tr->trDelta, phase, result );
ADDRFP4 8
INDIRP4
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 4
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
ADDRFP4 8
INDIRP4
CNSTI4 8
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
line 1765
;1765:		break;
ADDRGP4 $615
JUMPV
LABELV $620
line 1767
;1766:	case TR_LINEAR_STOP:
;1767:		if ( atTime > tr->trTime + tr->trDuration ) {
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 4
INDIRI4
ADDRLP4 32
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRLP4 32
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
ADDI4
LEI4 $621
line 1768
;1768:			VectorClear( result );
ADDRLP4 36
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 40
CNSTF4 0
ASGNF4
ADDRLP4 36
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 40
INDIRF4
ASGNF4
ADDRLP4 36
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 40
INDIRF4
ASGNF4
ADDRLP4 36
INDIRP4
ADDRLP4 40
INDIRF4
ASGNF4
line 1769
;1769:			return;
ADDRGP4 $613
JUMPV
LABELV $621
line 1771
;1770:		}
;1771:		VectorCopy( tr->trDelta, result );
ADDRFP4 8
INDIRP4
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 1772
;1772:		break;
ADDRGP4 $615
JUMPV
LABELV $623
line 1774
;1773:	case TR_GRAVITY:
;1774:		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
ADDRLP4 4
CNSTF4 981668463
ADDRFP4 4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
SUBI4
CVIF4 4
MULF4
ASGNF4
line 1775
;1775:		VectorCopy( tr->trDelta, result );
ADDRFP4 8
INDIRP4
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 1776
;1776:		result[2] -= DEFAULT_GRAVITY * deltaTime;		// FIXME: local gravity...
ADDRLP4 36
ADDRFP4 8
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRF4
CNSTF4 1145569280
ADDRLP4 4
INDIRF4
MULF4
SUBF4
ASGNF4
line 1777
;1777:		break;
ADDRGP4 $615
JUMPV
LABELV $614
line 1779
;1778:	default:
;1779:		Com_Error( ERR_DROP, "BG_EvaluateTrajectoryDelta: unknown trType: %i", tr->trTime );
CNSTI4 1
ARGI4
ADDRGP4 $624
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_Error
CALLV
pop
line 1780
;1780:		break;
LABELV $615
line 1782
;1781:	}
;1782:}
LABELV $613
endproc BG_EvaluateTrajectoryDelta 44 12
data
export eventnames
align 4
LABELV eventnames
address $626
address $627
address $628
address $629
address $630
address $631
address $632
address $633
address $634
address $635
address $636
address $637
address $638
address $639
address $640
address $641
address $642
address $643
address $644
address $645
address $646
address $647
address $648
address $649
address $650
address $651
address $652
address $653
address $654
address $655
address $656
address $657
address $658
address $659
address $660
address $661
address $662
address $663
address $664
address $665
address $666
address $667
address $668
address $669
address $670
address $671
address $672
address $673
address $674
address $675
address $676
address $677
address $678
address $679
address $680
address $681
address $682
address $683
address $684
address $685
address $686
address $687
address $688
address $689
address $690
address $691
address $692
address $693
address $694
address $695
address $696
address $697
address $698
address $699
address $700
address $701
address $702
address $703
address $704
address $705
address $706
address $707
address $708
address $709
address $710
address $711
address $712
address $713
address $714
address $715
address $716
address $717
address $718
address $719
address $720
address $721
address $722
address $723
address $724
address $725
address $726
address $727
address $728
address $729
address $730
address $731
address $732
address $733
address $734
address $735
export BG_AddPredictableEventToPlayerstate
code
proc BG_AddPredictableEventToPlayerstate 12 0
line 1945
;1783:
;1784:char *eventnames[] = {
;1785:	"EV_NONE",
;1786:
;1787:	"EV_CLIENTJOIN",
;1788:
;1789:	"EV_FOOTSTEP",
;1790:	"EV_FOOTSTEP_METAL",
;1791:	"EV_FOOTSPLASH",
;1792:	"EV_FOOTWADE",
;1793:	"EV_SWIM",
;1794:
;1795:	"EV_STEP_4",
;1796:	"EV_STEP_8",
;1797:	"EV_STEP_12",
;1798:	"EV_STEP_16",
;1799:
;1800:	"EV_FALL",
;1801:
;1802:	"EV_JUMP_PAD",			// boing sound at origin", jump sound on player
;1803:
;1804:	"EV_PRIVATE_DUEL",
;1805:
;1806:	"EV_JUMP",
;1807:	"EV_ROLL",
;1808:	"EV_WATER_TOUCH",	// foot touches
;1809:	"EV_WATER_LEAVE",	// foot leaves
;1810:	"EV_WATER_UNDER",	// head touches
;1811:	"EV_WATER_CLEAR",	// head leaves
;1812:
;1813:	"EV_ITEM_PICKUP",			// normal item pickups are predictable
;1814:	"EV_GLOBAL_ITEM_PICKUP",	// powerup / team sounds are broadcast to everyone
;1815:
;1816:	"EV_NOAMMO",
;1817:	"EV_CHANGE_WEAPON",
;1818:	"EV_FIRE_WEAPON",
;1819:	"EV_ALT_FIRE",
;1820:	"EV_SABER_ATTACK",
;1821:	"EV_SABER_HIT",
;1822:	"EV_SABER_BLOCK",
;1823:	"EV_SABER_UNHOLSTER",
;1824:	"EV_BECOME_JEDIMASTER",
;1825:	"EV_DISRUPTOR_MAIN_SHOT",
;1826:	"EV_DISRUPTOR_SNIPER_SHOT",
;1827:	"EV_DISRUPTOR_SNIPER_MISS",
;1828:	"EV_DISRUPTOR_HIT",
;1829:	"EV_DISRUPTOR_ZOOMSOUND",
;1830:
;1831:	"EV_PREDEFSOUND",
;1832:
;1833:	"EV_TEAM_POWER",
;1834:
;1835:	"EV_SCREENSHAKE",
;1836:
;1837:	"EV_USE",			// +Use key
;1838:
;1839:	"EV_USE_ITEM0",
;1840:	"EV_USE_ITEM1",
;1841:	"EV_USE_ITEM2",
;1842:	"EV_USE_ITEM3",
;1843:	"EV_USE_ITEM4",
;1844:	"EV_USE_ITEM5",
;1845:	"EV_USE_ITEM6",
;1846:	"EV_USE_ITEM7",
;1847:	"EV_USE_ITEM8",
;1848:	"EV_USE_ITEM9",
;1849:	"EV_USE_ITEM10",
;1850:	"EV_USE_ITEM11",
;1851:	"EV_USE_ITEM12",
;1852:	"EV_USE_ITEM13",
;1853:	"EV_USE_ITEM14",
;1854:	"EV_USE_ITEM15",
;1855:
;1856:	"EV_ITEMUSEFAIL",
;1857:
;1858:	"EV_ITEM_RESPAWN",
;1859:	"EV_ITEM_POP",
;1860:	"EV_PLAYER_TELEPORT_IN",
;1861:	"EV_PLAYER_TELEPORT_OUT",
;1862:
;1863:	"EV_GRENADE_BOUNCE",		// eventParm will be the soundindex
;1864:	"EV_MISSILE_STICK",
;1865:
;1866:	"EV_PLAY_EFFECT",
;1867:	"EV_PLAY_EFFECT_ID", //finally gave in and added it..
;1868:
;1869:	"EV_MUTE_SOUND",
;1870:	"EV_GENERAL_SOUND",
;1871:	"EV_GLOBAL_SOUND",		// no attenuation
;1872:	"EV_GLOBAL_TEAM_SOUND",
;1873:	"EV_ENTITY_SOUND",
;1874:
;1875:	"EV_PLAY_ROFF",
;1876:
;1877:	"EV_GLASS_SHATTER",
;1878:	"EV_DEBRIS",
;1879:
;1880:	"EV_MISSILE_HIT",
;1881:	"EV_MISSILE_MISS",
;1882:	"EV_MISSILE_MISS_METAL",
;1883:	"EV_BULLET",				// otherEntity is the shooter
;1884:
;1885:	"EV_PAIN",
;1886:	"EV_DEATH1",
;1887:	"EV_DEATH2",
;1888:	"EV_DEATH3",
;1889:	"EV_OBITUARY",
;1890:
;1891:	"EV_POWERUP_QUAD",
;1892:	"EV_POWERUP_BATTLESUIT",
;1893:	//"EV_POWERUP_REGEN",
;1894:
;1895:	"EV_FORCE_DRAINED",
;1896:
;1897:	"EV_GIB_PLAYER",			// gib a previously living player
;1898:	"EV_SCOREPLUM",			// score plum
;1899:
;1900:	"EV_CTFMESSAGE",
;1901:
;1902:	"EV_SAGA_ROUNDOVER",
;1903:	"EV_SAGA_OBJECTIVECOMPLETE",
;1904:
;1905:	"EV_DESTROY_GHOUL2_INSTANCE",
;1906:
;1907:	"EV_DESTROY_WEAPON_MODEL",
;1908:
;1909:	"EV_GIVE_NEW_RANK",
;1910:	"EV_SET_FREE_SABER",
;1911:	"EV_SET_FORCE_DISABLE",
;1912:
;1913:	"EV_WEAPON_CHARGE",
;1914:	"EV_WEAPON_CHARGE_ALT",
;1915:
;1916:	"EV_SHIELD_HIT",
;1917:
;1918:	"EV_DEBUG_LINE",
;1919:	"EV_TESTLINE",
;1920:	"EV_STOPLOOPINGSOUND",
;1921:	"EV_STARTLOOPINGSOUND",
;1922:	"EV_TAUNT",
;1923:
;1924:	"EV_TAUNT_YES",
;1925:	"EV_TAUNT_NO",
;1926:	"EV_TAUNT_FOLLOWME",
;1927:	"EV_TAUNT_GETFLAG",
;1928:	"EV_TAUNT_GUARDBASE",
;1929:	"EV_TAUNT_PATROL",
;1930:
;1931:	"EV_BODY_QUEUE_COPY"
;1932:
;1933:};
;1934:
;1935:/*
;1936:===============
;1937:BG_AddPredictableEventToPlayerstate
;1938:
;1939:Handles the sequence numbers
;1940:===============
;1941:*/
;1942:
;1943:void	trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize );
;1944:
;1945:void BG_AddPredictableEventToPlayerstate( int newEvent, int eventParm, playerState_t *ps ) {
line 1960
;1946:
;1947:#ifdef _DEBUG
;1948:	{
;1949:		char buf[256];
;1950:		trap_Cvar_VariableStringBuffer("showevents", buf, sizeof(buf));
;1951:		if ( atof(buf) != 0 ) {
;1952:#ifdef QAGAME
;1953:			Com_Printf(" game event svt %5d -> %5d: num = %20s parm %d\n", ps->pmove_framecount/*ps->commandTime*/, ps->eventSequence, eventnames[newEvent], eventParm);
;1954:#else
;1955:			Com_Printf("Cgame event svt %5d -> %5d: num = %20s parm %d\n", ps->pmove_framecount/*ps->commandTime*/, ps->eventSequence, eventnames[newEvent], eventParm);
;1956:#endif
;1957:		}
;1958:	}
;1959:#endif
;1960:	ps->events[ps->eventSequence & (MAX_PS_EVENTS-1)] = newEvent;
ADDRLP4 0
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 112
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ADDP4
ADDRFP4 0
INDIRI4
ASGNI4
line 1961
;1961:	ps->eventParms[ps->eventSequence & (MAX_PS_EVENTS-1)] = eventParm;
ADDRLP4 4
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 112
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 2
LSHI4
ADDRLP4 4
INDIRP4
CNSTI4 124
ADDP4
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 1962
;1962:	ps->eventSequence++;
ADDRLP4 8
ADDRFP4 8
INDIRP4
CNSTI4 112
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1963
;1963:}
LABELV $736
endproc BG_AddPredictableEventToPlayerstate 12 0
export BG_TouchJumpPad
proc BG_TouchJumpPad 32 8
line 1970
;1964:
;1965:/*
;1966:========================
;1967:BG_TouchJumpPad
;1968:========================
;1969:*/
;1970:void BG_TouchJumpPad( playerState_t *ps, entityState_t *jumppad ) {
line 1976
;1971:	vec3_t	angles;
;1972:	float p;
;1973:	int effectNum;
;1974:
;1975:	// spectators don't use jump pads
;1976:	if ( ps->pm_type != PM_NORMAL && ps->pm_type != PM_FLOAT ) {
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $738
ADDRLP4 20
INDIRI4
CNSTI4 1
EQI4 $738
line 1977
;1977:		return;
ADDRGP4 $737
JUMPV
LABELV $738
line 1982
;1978:	}
;1979:
;1980:	// if we didn't hit this same jumppad the previous frame
;1981:	// then don't play the event sound again if we are in a fat trigger
;1982:	if ( ps->jumppad_ent != jumppad->number ) {
ADDRFP4 0
INDIRP4
CNSTI4 480
ADDP4
INDIRI4
ADDRFP4 4
INDIRP4
INDIRI4
EQI4 $740
line 1984
;1983:
;1984:		vectoangles( jumppad->origin2, angles);
ADDRFP4 4
INDIRP4
CNSTI4 104
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1985
;1985:		p = fabs( AngleNormalize180( angles[PITCH] ) );
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 24
ADDRGP4 AngleNormalize180
CALLF4
ASGNF4
ADDRLP4 24
INDIRF4
ARGF4
ADDRLP4 28
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 12
ADDRLP4 28
INDIRF4
ASGNF4
line 1986
;1986:		if( p < 45 ) {
ADDRLP4 12
INDIRF4
CNSTF4 1110704128
GEF4 $742
line 1987
;1987:			effectNum = 0;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 1988
;1988:		} else {
ADDRGP4 $743
JUMPV
LABELV $742
line 1989
;1989:			effectNum = 1;
ADDRLP4 16
CNSTI4 1
ASGNI4
line 1990
;1990:		}
LABELV $743
line 1992
;1991:		//BG_AddPredictableEventToPlayerstate( EV_JUMP_PAD, effectNum, ps );
;1992:	}
LABELV $740
line 1994
;1993:	// remember hitting this jumppad this frame
;1994:	ps->jumppad_ent = jumppad->number;
ADDRFP4 0
INDIRP4
CNSTI4 480
ADDP4
ADDRFP4 4
INDIRP4
INDIRI4
ASGNI4
line 1995
;1995:	ps->jumppad_frame = ps->pmove_framecount;
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CNSTI4 492
ADDP4
ADDRLP4 24
INDIRP4
CNSTI4 488
ADDP4
INDIRI4
ASGNI4
line 1997
;1996:	// give the player the velocity from the jumppad
;1997:	VectorCopy( jumppad->origin2, ps->velocity );
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
ADDRFP4 4
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 1998
;1998:}
LABELV $737
endproc BG_TouchJumpPad 32 8
export BG_PlayerStateToEntityState
proc BG_PlayerStateToEntityState 28 0
line 2008
;1999:
;2000:/*
;2001:========================
;2002:BG_PlayerStateToEntityState
;2003:
;2004:This is done after each set of usercmd_t on the server,
;2005:and after local prediction on the client
;2006:========================
;2007:*/
;2008:void BG_PlayerStateToEntityState( playerState_t *ps, entityState_t *s, qboolean snap ) {
line 2011
;2009:	int		i;
;2010:
;2011:	if ( ps->pm_type == PM_INTERMISSION || ps->pm_type == PM_SPECTATOR ) {
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 6
EQI4 $747
ADDRLP4 4
INDIRI4
CNSTI4 3
NEI4 $745
LABELV $747
line 2012
;2012:		s->eType = ET_INVISIBLE;
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 12
ASGNI4
line 2013
;2013:	} else if ( ps->stats[STAT_HEALTH] <= GIB_HEALTH ) {
ADDRGP4 $746
JUMPV
LABELV $745
ADDRFP4 0
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CNSTI4 -40
GTI4 $748
line 2014
;2014:		s->eType = ET_INVISIBLE;
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 12
ASGNI4
line 2015
;2015:	} else {
ADDRGP4 $749
JUMPV
LABELV $748
line 2016
;2016:		s->eType = ET_PLAYER;
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 1
ASGNI4
line 2017
;2017:	}
LABELV $749
LABELV $746
line 2019
;2018:
;2019:	s->number = ps->clientNum;
ADDRFP4 4
INDIRP4
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ASGNI4
line 2021
;2020:
;2021:	s->pos.trType = TR_INTERPOLATE;
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 1
ASGNI4
line 2022
;2022:	VectorCopy( ps->origin, s->pos.trBase );
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 2023
;2023:	if ( snap ) {
ADDRFP4 8
INDIRI4
CNSTI4 0
EQI4 $750
line 2024
;2024:		SnapVector( s->pos.trBase );
ADDRLP4 8
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 12
ADDRFP4 4
INDIRP4
CNSTI4 28
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 16
ADDRFP4 4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
line 2025
;2025:	}
LABELV $750
line 2027
;2026:	// set the trDelta for flag direction
;2027:	VectorCopy( ps->velocity, s->pos.trDelta );
ADDRFP4 4
INDIRP4
CNSTI4 36
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRB
ASGNB 12
line 2029
;2028:
;2029:	s->apos.trType = TR_INTERPOLATE;
ADDRFP4 4
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 1
ASGNI4
line 2030
;2030:	VectorCopy( ps->viewangles, s->apos.trBase );
ADDRFP4 4
INDIRP4
CNSTI4 60
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 156
ADDP4
INDIRB
ASGNB 12
line 2031
;2031:	if ( snap ) {
ADDRFP4 8
INDIRI4
CNSTI4 0
EQI4 $752
line 2032
;2032:		SnapVector( s->apos.trBase );
ADDRLP4 8
ADDRFP4 4
INDIRP4
CNSTI4 60
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 12
ADDRFP4 4
INDIRP4
CNSTI4 64
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 16
ADDRFP4 4
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
line 2033
;2033:	}
LABELV $752
line 2035
;2034:
;2035:	s->trickedentindex = ps->fd.forceMindtrickTargetIndex;
ADDRFP4 4
INDIRP4
CNSTI4 148
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 1144
ADDP4
INDIRI4
ASGNI4
line 2036
;2036:	s->trickedentindex2 = ps->fd.forceMindtrickTargetIndex2;
ADDRFP4 4
INDIRP4
CNSTI4 152
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 1148
ADDP4
INDIRI4
ASGNI4
line 2037
;2037:	s->trickedentindex3 = ps->fd.forceMindtrickTargetIndex3;
ADDRFP4 4
INDIRP4
CNSTI4 156
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 1152
ADDP4
INDIRI4
ASGNI4
line 2038
;2038:	s->trickedentindex4 = ps->fd.forceMindtrickTargetIndex4;
ADDRFP4 4
INDIRP4
CNSTI4 160
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 1156
ADDP4
INDIRI4
ASGNI4
line 2040
;2039:
;2040:	s->forceFrame = ps->saberLockFrame;
ADDRFP4 4
INDIRP4
CNSTI4 288
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ASGNI4
line 2042
;2041:
;2042:	s->emplacedOwner = ps->electrifyTime;
ADDRFP4 4
INDIRP4
CNSTI4 180
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRI4
ASGNI4
line 2044
;2043:
;2044:	s->speed = ps->speed;
ADDRFP4 4
INDIRP4
CNSTI4 164
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
INDIRI4
CVIF4 4
ASGNF4
line 2046
;2045:
;2046:	s->genericenemyindex = ps->genericEnemyIndex;
ADDRFP4 4
INDIRP4
CNSTI4 172
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 612
ADDP4
INDIRI4
ASGNI4
line 2048
;2047:
;2048:	s->activeForcePass = ps->activeForcePass;
ADDRFP4 4
INDIRP4
CNSTI4 176
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
ASGNI4
line 2050
;2049:
;2050:	s->angles2[YAW] = ps->movementDir;
ADDRFP4 4
INDIRP4
CNSTI4 132
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
INDIRI4
CVIF4 4
ASGNF4
line 2051
;2051:	s->legsAnim = ps->legsAnim;
ADDRFP4 4
INDIRP4
CNSTI4 280
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRI4
ASGNI4
line 2052
;2052:	s->torsoAnim = ps->torsoAnim;
ADDRFP4 4
INDIRP4
CNSTI4 284
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
ASGNI4
line 2053
;2053:	s->clientNum = ps->clientNum;		// ET_PLAYER looks here instead of at number
ADDRFP4 4
INDIRP4
CNSTI4 220
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ASGNI4
line 2055
;2054:										// so corpses can also reference the proper config
;2055:	s->eFlags = ps->eFlags;
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
ASGNI4
line 2057
;2056:
;2057:	s->saberInFlight = ps->saberInFlight;
ADDRFP4 4
INDIRP4
CNSTI4 228
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 504
ADDP4
INDIRI4
ASGNI4
line 2058
;2058:	s->saberEntityNum = ps->saberEntityNum;
ADDRFP4 4
INDIRP4
CNSTI4 232
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 544
ADDP4
INDIRI4
ASGNI4
line 2059
;2059:	s->saberMove = ps->saberMove;
ADDRFP4 4
INDIRP4
CNSTI4 236
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
ASGNI4
line 2060
;2060:	s->forcePowersActive = ps->fd.forcePowersActive;
ADDRFP4 4
INDIRP4
CNSTI4 240
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 836
ADDP4
INDIRI4
ASGNI4
line 2062
;2061:
;2062:	if (ps->duelInProgress)
ADDRFP4 0
INDIRP4
CNSTI4 1292
ADDP4
INDIRI4
CNSTI4 0
EQI4 $754
line 2063
;2063:	{
line 2064
;2064:		s->bolt1 = 1;
ADDRFP4 4
INDIRP4
CNSTI4 140
ADDP4
CNSTI4 1
ASGNI4
line 2065
;2065:	}
ADDRGP4 $755
JUMPV
LABELV $754
line 2067
;2066:	else
;2067:	{
line 2068
;2068:		s->bolt1 = 0;
ADDRFP4 4
INDIRP4
CNSTI4 140
ADDP4
CNSTI4 0
ASGNI4
line 2069
;2069:	}
LABELV $755
line 2071
;2070:
;2071:	if (ps->dualBlade)
ADDRFP4 0
INDIRP4
CNSTI4 1352
ADDP4
INDIRI4
CNSTI4 0
EQI4 $756
line 2072
;2072:	{
line 2073
;2073:		s->bolt2 = 1;
ADDRFP4 4
INDIRP4
CNSTI4 144
ADDP4
CNSTI4 1
ASGNI4
line 2074
;2074:	}
ADDRGP4 $757
JUMPV
LABELV $756
line 2076
;2075:	else
;2076:	{
line 2077
;2077:		s->bolt2 = 0;
ADDRFP4 4
INDIRP4
CNSTI4 144
ADDP4
CNSTI4 0
ASGNI4
line 2078
;2078:	}
LABELV $757
line 2080
;2079:
;2080:	s->otherEntityNum2 = ps->emplacedIndex;
ADDRFP4 4
INDIRP4
CNSTI4 188
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 596
ADDP4
INDIRI4
ASGNI4
line 2082
;2081:
;2082:	s->shouldtarget = ps->saberHolstered; //reuse bool in entitystate for players differently
ADDRFP4 4
INDIRP4
CNSTI4 268
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 1300
ADDP4
INDIRI4
ASGNI4
line 2083
;2083:	s->teamowner = ps->usingATST;
ADDRFP4 4
INDIRP4
CNSTI4 264
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 1304
ADDP4
INDIRI4
ASGNI4
line 2085
;2084:
;2085:	if (ps->genericEnemyIndex != -1)
ADDRFP4 0
INDIRP4
CNSTI4 612
ADDP4
INDIRI4
CNSTI4 -1
EQI4 $758
line 2086
;2086:	{
line 2087
;2087:		s->eFlags |= EF_SEEKERDRONE;
ADDRLP4 8
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1048576
BORI4
ASGNI4
line 2088
;2088:	}
LABELV $758
line 2090
;2089:
;2090:	if ( ps->stats[STAT_HEALTH] <= 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CNSTI4 0
GTI4 $760
line 2091
;2091:		s->eFlags |= EF_DEAD;
ADDRLP4 8
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 2092
;2092:	} else {
ADDRGP4 $761
JUMPV
LABELV $760
line 2093
;2093:		s->eFlags &= ~EF_DEAD;
ADDRLP4 8
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 -2
BANDI4
ASGNI4
line 2094
;2094:	}
LABELV $761
line 2096
;2095:
;2096:	if ( ps->externalEvent ) {
ADDRFP4 0
INDIRP4
CNSTI4 132
ADDP4
INDIRI4
CNSTI4 0
EQI4 $762
line 2097
;2097:		s->event = ps->externalEvent;
ADDRFP4 4
INDIRP4
CNSTI4 252
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 132
ADDP4
INDIRI4
ASGNI4
line 2098
;2098:		s->eventParm = ps->externalEventParm;
ADDRFP4 4
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 136
ADDP4
INDIRI4
ASGNI4
line 2099
;2099:	} else if ( ps->entityEventSequence < ps->eventSequence ) {
ADDRGP4 $763
JUMPV
LABELV $762
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 496
ADDP4
INDIRI4
ADDRLP4 8
INDIRP4
CNSTI4 112
ADDP4
INDIRI4
GEI4 $764
line 2102
;2100:		int		seq;
;2101:
;2102:		if ( ps->entityEventSequence < ps->eventSequence - MAX_PS_EVENTS) {
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 496
ADDP4
INDIRI4
ADDRLP4 16
INDIRP4
CNSTI4 112
ADDP4
INDIRI4
CNSTI4 2
SUBI4
GEI4 $766
line 2103
;2103:			ps->entityEventSequence = ps->eventSequence - MAX_PS_EVENTS;
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 496
ADDP4
ADDRLP4 20
INDIRP4
CNSTI4 112
ADDP4
INDIRI4
CNSTI4 2
SUBI4
ASGNI4
line 2104
;2104:		}
LABELV $766
line 2105
;2105:		seq = ps->entityEventSequence & (MAX_PS_EVENTS-1);
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
INDIRI4
CNSTI4 1
BANDI4
ASGNI4
line 2106
;2106:		s->event = ps->events[ seq ] | ( ( ps->entityEventSequence & 3 ) << 8 );
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 4
INDIRP4
CNSTI4 252
ADDP4
ADDRLP4 12
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 20
INDIRP4
CNSTI4 116
ADDP4
ADDP4
INDIRI4
ADDRLP4 20
INDIRP4
CNSTI4 496
ADDP4
INDIRI4
CNSTI4 3
BANDI4
CNSTI4 8
LSHI4
BORI4
ASGNI4
line 2107
;2107:		s->eventParm = ps->eventParms[ seq ];
ADDRFP4 4
INDIRP4
CNSTI4 256
ADDP4
ADDRLP4 12
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
ADDP4
INDIRI4
ASGNI4
line 2108
;2108:		ps->entityEventSequence++;
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2109
;2109:	}
LABELV $764
LABELV $763
line 2112
;2110:
;2111:
;2112:	s->weapon = ps->weapon;
ADDRFP4 4
INDIRP4
CNSTI4 276
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
ASGNI4
line 2113
;2113:	s->groundEntityNum = ps->groundEntityNum;
ADDRFP4 4
INDIRP4
CNSTI4 192
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 84
ADDP4
INDIRI4
ASGNI4
line 2115
;2114:
;2115:	s->powerups = 0;
ADDRFP4 4
INDIRP4
CNSTI4 272
ADDP4
CNSTI4 0
ASGNI4
line 2116
;2116:	for ( i = 0 ; i < MAX_POWERUPS ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $768
line 2117
;2117:		if ( ps->powerups[ i ] ) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 344
ADDP4
ADDP4
INDIRI4
CNSTI4 0
EQI4 $772
line 2118
;2118:			s->powerups |= 1 << i;
ADDRLP4 12
ADDRFP4 4
INDIRP4
CNSTI4 272
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1
ADDRLP4 0
INDIRI4
LSHI4
BORI4
ASGNI4
line 2119
;2119:		}
LABELV $772
line 2120
;2120:	}
LABELV $769
line 2116
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $768
line 2122
;2121:
;2122:	s->loopSound = ps->loopSound;
ADDRFP4 4
INDIRP4
CNSTI4 200
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 476
ADDP4
INDIRI4
ASGNI4
line 2123
;2123:	s->generic1 = ps->generic1;
ADDRFP4 4
INDIRP4
CNSTI4 292
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 472
ADDP4
INDIRI4
ASGNI4
line 2126
;2124:
;2125:	//NOT INCLUDED IN ENTITYSTATETOPLAYERSTATE:
;2126:	s->modelindex2 = ps->weaponstate;
ADDRFP4 4
INDIRP4
CNSTI4 216
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 152
ADDP4
INDIRI4
ASGNI4
line 2127
;2127:	s->constantLight = ps->weaponChargeTime;
ADDRFP4 4
INDIRP4
CNSTI4 196
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
ASGNI4
line 2129
;2128:
;2129:	VectorCopy(ps->lastHitLoc, s->origin2);
ADDRFP4 4
INDIRP4
CNSTI4 104
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 1356
ADDP4
INDIRB
ASGNB 12
line 2131
;2130:
;2131:	s->isJediMaster = ps->isJediMaster;
ADDRFP4 4
INDIRP4
CNSTI4 244
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 604
ADDP4
INDIRI4
ASGNI4
line 2133
;2132:
;2133:	s->time2 = ps->holocronBits;
ADDRFP4 4
INDIRP4
CNSTI4 88
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 2134
;2134:}
LABELV $744
endproc BG_PlayerStateToEntityState 28 0
export BG_PlayerStateToEntityStateExtraPolate
proc BG_PlayerStateToEntityStateExtraPolate 28 0
line 2144
;2135:
;2136:/*
;2137:========================
;2138:BG_PlayerStateToEntityStateExtraPolate
;2139:
;2140:This is done after each set of usercmd_t on the server,
;2141:and after local prediction on the client
;2142:========================
;2143:*/
;2144:void BG_PlayerStateToEntityStateExtraPolate( playerState_t *ps, entityState_t *s, int time, qboolean snap ) {
line 2147
;2145:	int		i;
;2146:
;2147:	if ( ps->pm_type == PM_INTERMISSION || ps->pm_type == PM_SPECTATOR ) {
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 6
EQI4 $777
ADDRLP4 4
INDIRI4
CNSTI4 3
NEI4 $775
LABELV $777
line 2148
;2148:		s->eType = ET_INVISIBLE;
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 12
ASGNI4
line 2149
;2149:	} else if ( ps->stats[STAT_HEALTH] <= GIB_HEALTH ) {
ADDRGP4 $776
JUMPV
LABELV $775
ADDRFP4 0
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CNSTI4 -40
GTI4 $778
line 2150
;2150:		s->eType = ET_INVISIBLE;
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 12
ASGNI4
line 2151
;2151:	} else {
ADDRGP4 $779
JUMPV
LABELV $778
line 2152
;2152:		s->eType = ET_PLAYER;
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 1
ASGNI4
line 2153
;2153:	}
LABELV $779
LABELV $776
line 2155
;2154:
;2155:	s->number = ps->clientNum;
ADDRFP4 4
INDIRP4
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ASGNI4
line 2157
;2156:
;2157:	s->pos.trType = TR_LINEAR_STOP;
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 3
ASGNI4
line 2158
;2158:	VectorCopy( ps->origin, s->pos.trBase );
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 2159
;2159:	if ( snap ) {
ADDRFP4 12
INDIRI4
CNSTI4 0
EQI4 $780
line 2160
;2160:		SnapVector( s->pos.trBase );
ADDRLP4 8
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 12
ADDRFP4 4
INDIRP4
CNSTI4 28
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 16
ADDRFP4 4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
line 2161
;2161:	}
LABELV $780
line 2163
;2162:	// set the trDelta for flag direction and linear prediction
;2163:	VectorCopy( ps->velocity, s->pos.trDelta );
ADDRFP4 4
INDIRP4
CNSTI4 36
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRB
ASGNB 12
line 2165
;2164:	// set the time for linear prediction
;2165:	s->pos.trTime = time;
ADDRFP4 4
INDIRP4
CNSTI4 16
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 2167
;2166:	// set maximum extra polation time
;2167:	s->pos.trDuration = 50; // 1000 / sv_fps (default = 20)
ADDRFP4 4
INDIRP4
CNSTI4 20
ADDP4
CNSTI4 50
ASGNI4
line 2169
;2168:
;2169:	s->apos.trType = TR_INTERPOLATE;
ADDRFP4 4
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 1
ASGNI4
line 2170
;2170:	VectorCopy( ps->viewangles, s->apos.trBase );
ADDRFP4 4
INDIRP4
CNSTI4 60
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 156
ADDP4
INDIRB
ASGNB 12
line 2171
;2171:	if ( snap ) {
ADDRFP4 12
INDIRI4
CNSTI4 0
EQI4 $782
line 2172
;2172:		SnapVector( s->apos.trBase );
ADDRLP4 8
ADDRFP4 4
INDIRP4
CNSTI4 60
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 12
ADDRFP4 4
INDIRP4
CNSTI4 64
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 16
ADDRFP4 4
INDIRP4
CNSTI4 68
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
line 2173
;2173:	}
LABELV $782
line 2175
;2174:
;2175:	s->trickedentindex = ps->fd.forceMindtrickTargetIndex;
ADDRFP4 4
INDIRP4
CNSTI4 148
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 1144
ADDP4
INDIRI4
ASGNI4
line 2176
;2176:	s->trickedentindex2 = ps->fd.forceMindtrickTargetIndex2;
ADDRFP4 4
INDIRP4
CNSTI4 152
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 1148
ADDP4
INDIRI4
ASGNI4
line 2177
;2177:	s->trickedentindex3 = ps->fd.forceMindtrickTargetIndex3;
ADDRFP4 4
INDIRP4
CNSTI4 156
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 1152
ADDP4
INDIRI4
ASGNI4
line 2178
;2178:	s->trickedentindex4 = ps->fd.forceMindtrickTargetIndex4;
ADDRFP4 4
INDIRP4
CNSTI4 160
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 1156
ADDP4
INDIRI4
ASGNI4
line 2180
;2179:
;2180:	s->forceFrame = ps->saberLockFrame;
ADDRFP4 4
INDIRP4
CNSTI4 288
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 532
ADDP4
INDIRI4
ASGNI4
line 2182
;2181:
;2182:	s->emplacedOwner = ps->electrifyTime;
ADDRFP4 4
INDIRP4
CNSTI4 180
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRI4
ASGNI4
line 2184
;2183:
;2184:	s->speed = ps->speed;
ADDRFP4 4
INDIRP4
CNSTI4 164
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
INDIRI4
CVIF4 4
ASGNF4
line 2186
;2185:
;2186:	s->genericenemyindex = ps->genericEnemyIndex;
ADDRFP4 4
INDIRP4
CNSTI4 172
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 612
ADDP4
INDIRI4
ASGNI4
line 2188
;2187:
;2188:	s->activeForcePass = ps->activeForcePass;
ADDRFP4 4
INDIRP4
CNSTI4 176
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 624
ADDP4
INDIRI4
ASGNI4
line 2190
;2189:
;2190:	s->angles2[YAW] = ps->movementDir;
ADDRFP4 4
INDIRP4
CNSTI4 132
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
INDIRI4
CVIF4 4
ASGNF4
line 2191
;2191:	s->legsAnim = ps->legsAnim;
ADDRFP4 4
INDIRP4
CNSTI4 280
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRI4
ASGNI4
line 2192
;2192:	s->torsoAnim = ps->torsoAnim;
ADDRFP4 4
INDIRP4
CNSTI4 284
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
ASGNI4
line 2193
;2193:	s->clientNum = ps->clientNum;		// ET_PLAYER looks here instead of at number
ADDRFP4 4
INDIRP4
CNSTI4 220
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ASGNI4
line 2195
;2194:										// so corpses can also reference the proper config
;2195:	s->eFlags = ps->eFlags;
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
ASGNI4
line 2197
;2196:
;2197:	s->saberInFlight = ps->saberInFlight;
ADDRFP4 4
INDIRP4
CNSTI4 228
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 504
ADDP4
INDIRI4
ASGNI4
line 2198
;2198:	s->saberEntityNum = ps->saberEntityNum;
ADDRFP4 4
INDIRP4
CNSTI4 232
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 544
ADDP4
INDIRI4
ASGNI4
line 2199
;2199:	s->saberMove = ps->saberMove;
ADDRFP4 4
INDIRP4
CNSTI4 236
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
ASGNI4
line 2200
;2200:	s->forcePowersActive = ps->fd.forcePowersActive;
ADDRFP4 4
INDIRP4
CNSTI4 240
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 836
ADDP4
INDIRI4
ASGNI4
line 2202
;2201:
;2202:	if (ps->duelInProgress)
ADDRFP4 0
INDIRP4
CNSTI4 1292
ADDP4
INDIRI4
CNSTI4 0
EQI4 $784
line 2203
;2203:	{
line 2204
;2204:		s->bolt1 = 1;
ADDRFP4 4
INDIRP4
CNSTI4 140
ADDP4
CNSTI4 1
ASGNI4
line 2205
;2205:	}
ADDRGP4 $785
JUMPV
LABELV $784
line 2207
;2206:	else
;2207:	{
line 2208
;2208:		s->bolt1 = 0;
ADDRFP4 4
INDIRP4
CNSTI4 140
ADDP4
CNSTI4 0
ASGNI4
line 2209
;2209:	}
LABELV $785
line 2211
;2210:
;2211:	if (ps->dualBlade)
ADDRFP4 0
INDIRP4
CNSTI4 1352
ADDP4
INDIRI4
CNSTI4 0
EQI4 $786
line 2212
;2212:	{
line 2213
;2213:		s->bolt2 = 1;
ADDRFP4 4
INDIRP4
CNSTI4 144
ADDP4
CNSTI4 1
ASGNI4
line 2214
;2214:	}
ADDRGP4 $787
JUMPV
LABELV $786
line 2216
;2215:	else
;2216:	{
line 2217
;2217:		s->bolt2 = 0;
ADDRFP4 4
INDIRP4
CNSTI4 144
ADDP4
CNSTI4 0
ASGNI4
line 2218
;2218:	}
LABELV $787
line 2220
;2219:
;2220:	s->otherEntityNum2 = ps->emplacedIndex;
ADDRFP4 4
INDIRP4
CNSTI4 188
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 596
ADDP4
INDIRI4
ASGNI4
line 2222
;2221:
;2222:	s->shouldtarget = ps->saberHolstered; //reuse bool in entitystate for players differently
ADDRFP4 4
INDIRP4
CNSTI4 268
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 1300
ADDP4
INDIRI4
ASGNI4
line 2223
;2223:	s->teamowner = ps->usingATST;
ADDRFP4 4
INDIRP4
CNSTI4 264
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 1304
ADDP4
INDIRI4
ASGNI4
line 2225
;2224:
;2225:	if (ps->genericEnemyIndex != -1)
ADDRFP4 0
INDIRP4
CNSTI4 612
ADDP4
INDIRI4
CNSTI4 -1
EQI4 $788
line 2226
;2226:	{
line 2227
;2227:		s->eFlags |= EF_SEEKERDRONE;
ADDRLP4 8
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1048576
BORI4
ASGNI4
line 2228
;2228:	}
LABELV $788
line 2230
;2229:
;2230:	if ( ps->stats[STAT_HEALTH] <= 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CNSTI4 0
GTI4 $790
line 2231
;2231:		s->eFlags |= EF_DEAD;
ADDRLP4 8
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 2232
;2232:	} else {
ADDRGP4 $791
JUMPV
LABELV $790
line 2233
;2233:		s->eFlags &= ~EF_DEAD;
ADDRLP4 8
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 -2
BANDI4
ASGNI4
line 2234
;2234:	}
LABELV $791
line 2236
;2235:
;2236:	if ( ps->externalEvent ) {
ADDRFP4 0
INDIRP4
CNSTI4 132
ADDP4
INDIRI4
CNSTI4 0
EQI4 $792
line 2237
;2237:		s->event = ps->externalEvent;
ADDRFP4 4
INDIRP4
CNSTI4 252
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 132
ADDP4
INDIRI4
ASGNI4
line 2238
;2238:		s->eventParm = ps->externalEventParm;
ADDRFP4 4
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 136
ADDP4
INDIRI4
ASGNI4
line 2239
;2239:	} else if ( ps->entityEventSequence < ps->eventSequence ) {
ADDRGP4 $793
JUMPV
LABELV $792
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 496
ADDP4
INDIRI4
ADDRLP4 8
INDIRP4
CNSTI4 112
ADDP4
INDIRI4
GEI4 $794
line 2242
;2240:		int		seq;
;2241:
;2242:		if ( ps->entityEventSequence < ps->eventSequence - MAX_PS_EVENTS) {
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 496
ADDP4
INDIRI4
ADDRLP4 16
INDIRP4
CNSTI4 112
ADDP4
INDIRI4
CNSTI4 2
SUBI4
GEI4 $796
line 2243
;2243:			ps->entityEventSequence = ps->eventSequence - MAX_PS_EVENTS;
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 496
ADDP4
ADDRLP4 20
INDIRP4
CNSTI4 112
ADDP4
INDIRI4
CNSTI4 2
SUBI4
ASGNI4
line 2244
;2244:		}
LABELV $796
line 2245
;2245:		seq = ps->entityEventSequence & (MAX_PS_EVENTS-1);
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
INDIRI4
CNSTI4 1
BANDI4
ASGNI4
line 2246
;2246:		s->event = ps->events[ seq ] | ( ( ps->entityEventSequence & 3 ) << 8 );
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 4
INDIRP4
CNSTI4 252
ADDP4
ADDRLP4 12
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 20
INDIRP4
CNSTI4 116
ADDP4
ADDP4
INDIRI4
ADDRLP4 20
INDIRP4
CNSTI4 496
ADDP4
INDIRI4
CNSTI4 3
BANDI4
CNSTI4 8
LSHI4
BORI4
ASGNI4
line 2247
;2247:		s->eventParm = ps->eventParms[ seq ];
ADDRFP4 4
INDIRP4
CNSTI4 256
ADDP4
ADDRLP4 12
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
ADDP4
INDIRI4
ASGNI4
line 2248
;2248:		ps->entityEventSequence++;
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2249
;2249:	}
LABELV $794
LABELV $793
line 2250
;2250:	s->weapon = ps->weapon;
ADDRFP4 4
INDIRP4
CNSTI4 276
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
ASGNI4
line 2251
;2251:	s->groundEntityNum = ps->groundEntityNum;
ADDRFP4 4
INDIRP4
CNSTI4 192
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 84
ADDP4
INDIRI4
ASGNI4
line 2253
;2252:
;2253:	s->powerups = 0;
ADDRFP4 4
INDIRP4
CNSTI4 272
ADDP4
CNSTI4 0
ASGNI4
line 2254
;2254:	for ( i = 0 ; i < MAX_POWERUPS ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $798
line 2255
;2255:		if ( ps->powerups[ i ] ) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 344
ADDP4
ADDP4
INDIRI4
CNSTI4 0
EQI4 $802
line 2256
;2256:			s->powerups |= 1 << i;
ADDRLP4 12
ADDRFP4 4
INDIRP4
CNSTI4 272
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1
ADDRLP4 0
INDIRI4
LSHI4
BORI4
ASGNI4
line 2257
;2257:		}
LABELV $802
line 2258
;2258:	}
LABELV $799
line 2254
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $798
line 2260
;2259:
;2260:	s->loopSound = ps->loopSound;
ADDRFP4 4
INDIRP4
CNSTI4 200
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 476
ADDP4
INDIRI4
ASGNI4
line 2261
;2261:	s->generic1 = ps->generic1;
ADDRFP4 4
INDIRP4
CNSTI4 292
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 472
ADDP4
INDIRI4
ASGNI4
line 2264
;2262:
;2263:	//NOT INCLUDED IN ENTITYSTATETOPLAYERSTATE:
;2264:	s->modelindex2 = ps->weaponstate;
ADDRFP4 4
INDIRP4
CNSTI4 216
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 152
ADDP4
INDIRI4
ASGNI4
line 2265
;2265:	s->constantLight = ps->weaponChargeTime;
ADDRFP4 4
INDIRP4
CNSTI4 196
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
ASGNI4
line 2267
;2266:
;2267:	VectorCopy(ps->lastHitLoc, s->origin2);
ADDRFP4 4
INDIRP4
CNSTI4 104
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 1356
ADDP4
INDIRB
ASGNB 12
line 2269
;2268:
;2269:	s->isJediMaster = ps->isJediMaster;
ADDRFP4 4
INDIRP4
CNSTI4 244
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 604
ADDP4
INDIRI4
ASGNI4
line 2271
;2270:
;2271:	s->time2 = ps->holocronBits;
ADDRFP4 4
INDIRP4
CNSTI4 88
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 2272
;2272:}
LABELV $774
endproc BG_PlayerStateToEntityStateExtraPolate 28 0
proc BG_SwingAngles 28 8
line 2288
;2273:
;2274:/*
;2275:=============================================================================
;2276:
;2277:PLAYER ANGLES
;2278:
;2279:=============================================================================
;2280:*/
;2281:
;2282:/*
;2283:==================
;2284:BG_SwingAngles
;2285:==================
;2286:*/
;2287:static void BG_SwingAngles( float destination, float swingTolerance, float clampTolerance,
;2288:					float speed, float *angle, qboolean *swinging, int frameTime ) {
line 2293
;2289:	float	swing;
;2290:	float	move;
;2291:	float	scale;
;2292:
;2293:	if ( !*swinging ) {
ADDRFP4 20
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $805
line 2295
;2294:		// see if a swing should be started
;2295:		swing = AngleSubtract( *angle, destination );
ADDRFP4 16
INDIRP4
INDIRF4
ARGF4
ADDRFP4 0
INDIRF4
ARGF4
ADDRLP4 12
ADDRGP4 AngleSubtract
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 12
INDIRF4
ASGNF4
line 2296
;2296:		if ( swing > swingTolerance || swing < -swingTolerance ) {
ADDRLP4 20
ADDRFP4 4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRF4
ADDRLP4 20
INDIRF4
GTF4 $809
ADDRLP4 0
INDIRF4
ADDRLP4 20
INDIRF4
NEGF4
GEF4 $807
LABELV $809
line 2297
;2297:			*swinging = qtrue;
ADDRFP4 20
INDIRP4
CNSTI4 1
ASGNI4
line 2298
;2298:		}
LABELV $807
line 2299
;2299:	}
LABELV $805
line 2301
;2300:
;2301:	if ( !*swinging ) {
ADDRFP4 20
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $810
line 2302
;2302:		return;
ADDRGP4 $804
JUMPV
LABELV $810
line 2307
;2303:	}
;2304:	
;2305:	// modify the speed depending on the delta
;2306:	// so it doesn't seem so linear
;2307:	swing = AngleSubtract( destination, *angle );
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 16
INDIRP4
INDIRF4
ARGF4
ADDRLP4 12
ADDRGP4 AngleSubtract
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 12
INDIRF4
ASGNF4
line 2308
;2308:	scale = fabs( swing );
ADDRLP4 0
INDIRF4
ARGF4
ADDRLP4 16
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 4
ADDRLP4 16
INDIRF4
ASGNF4
line 2309
;2309:	if ( scale < swingTolerance * 0.5 ) {
ADDRLP4 4
INDIRF4
CNSTF4 1056964608
ADDRFP4 4
INDIRF4
MULF4
GEF4 $812
line 2310
;2310:		scale = 0.5;
ADDRLP4 4
CNSTF4 1056964608
ASGNF4
line 2311
;2311:	} else if ( scale < swingTolerance ) {
ADDRGP4 $813
JUMPV
LABELV $812
ADDRLP4 4
INDIRF4
ADDRFP4 4
INDIRF4
GEF4 $814
line 2312
;2312:		scale = 1.0;
ADDRLP4 4
CNSTF4 1065353216
ASGNF4
line 2313
;2313:	} else {
ADDRGP4 $815
JUMPV
LABELV $814
line 2314
;2314:		scale = 2.0;
ADDRLP4 4
CNSTF4 1073741824
ASGNF4
line 2315
;2315:	}
LABELV $815
LABELV $813
line 2318
;2316:
;2317:	// swing towards the destination angle
;2318:	if ( swing >= 0 ) {
ADDRLP4 0
INDIRF4
CNSTF4 0
LTF4 $816
line 2319
;2319:		move = frameTime * scale * speed;
ADDRLP4 8
ADDRFP4 24
INDIRI4
CVIF4 4
ADDRLP4 4
INDIRF4
MULF4
ADDRFP4 12
INDIRF4
MULF4
ASGNF4
line 2320
;2320:		if ( move >= swing ) {
ADDRLP4 8
INDIRF4
ADDRLP4 0
INDIRF4
LTF4 $818
line 2321
;2321:			move = swing;
ADDRLP4 8
ADDRLP4 0
INDIRF4
ASGNF4
line 2322
;2322:			*swinging = qfalse;
ADDRFP4 20
INDIRP4
CNSTI4 0
ASGNI4
line 2323
;2323:		}
LABELV $818
line 2324
;2324:		*angle = AngleMod( *angle + move );
ADDRLP4 20
ADDRFP4 16
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
INDIRF4
ADDRLP4 8
INDIRF4
ADDF4
ARGF4
ADDRLP4 24
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 20
INDIRP4
ADDRLP4 24
INDIRF4
ASGNF4
line 2325
;2325:	} else if ( swing < 0 ) {
ADDRGP4 $817
JUMPV
LABELV $816
ADDRLP4 0
INDIRF4
CNSTF4 0
GEF4 $820
line 2326
;2326:		move = frameTime * scale * -speed;
ADDRLP4 8
ADDRFP4 24
INDIRI4
CVIF4 4
ADDRLP4 4
INDIRF4
MULF4
ADDRFP4 12
INDIRF4
NEGF4
MULF4
ASGNF4
line 2327
;2327:		if ( move <= swing ) {
ADDRLP4 8
INDIRF4
ADDRLP4 0
INDIRF4
GTF4 $822
line 2328
;2328:			move = swing;
ADDRLP4 8
ADDRLP4 0
INDIRF4
ASGNF4
line 2329
;2329:			*swinging = qfalse;
ADDRFP4 20
INDIRP4
CNSTI4 0
ASGNI4
line 2330
;2330:		}
LABELV $822
line 2331
;2331:		*angle = AngleMod( *angle + move );
ADDRLP4 20
ADDRFP4 16
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
INDIRF4
ADDRLP4 8
INDIRF4
ADDF4
ARGF4
ADDRLP4 24
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 20
INDIRP4
ADDRLP4 24
INDIRF4
ASGNF4
line 2332
;2332:	}
LABELV $820
LABELV $817
line 2335
;2333:
;2334:	// clamp to no more than tolerance
;2335:	swing = AngleSubtract( destination, *angle );
ADDRFP4 0
INDIRF4
ARGF4
ADDRFP4 16
INDIRP4
INDIRF4
ARGF4
ADDRLP4 20
ADDRGP4 AngleSubtract
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 20
INDIRF4
ASGNF4
line 2336
;2336:	if ( swing > clampTolerance ) {
ADDRLP4 0
INDIRF4
ADDRFP4 8
INDIRF4
LEF4 $824
line 2337
;2337:		*angle = AngleMod( destination - (clampTolerance - 1) );
ADDRFP4 0
INDIRF4
ADDRFP4 8
INDIRF4
CNSTF4 1065353216
SUBF4
SUBF4
ARGF4
ADDRLP4 24
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRFP4 16
INDIRP4
ADDRLP4 24
INDIRF4
ASGNF4
line 2338
;2338:	} else if ( swing < -clampTolerance ) {
ADDRGP4 $825
JUMPV
LABELV $824
ADDRLP4 0
INDIRF4
ADDRFP4 8
INDIRF4
NEGF4
GEF4 $826
line 2339
;2339:		*angle = AngleMod( destination + (clampTolerance - 1) );
ADDRFP4 0
INDIRF4
ADDRFP4 8
INDIRF4
CNSTF4 1065353216
SUBF4
ADDF4
ARGF4
ADDRLP4 24
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRFP4 16
INDIRP4
ADDRLP4 24
INDIRF4
ASGNF4
line 2340
;2340:	}
LABELV $826
LABELV $825
line 2341
;2341:}
LABELV $804
endproc BG_SwingAngles 28 8
proc BG_AddPainTwitch 12 0
line 2349
;2342:
;2343:/*
;2344:=================
;2345:CG_AddPainTwitch
;2346:=================
;2347:*/
;2348:#define	PAIN_TWITCH_TIME	200
;2349:static void BG_AddPainTwitch( int painTime, int painDirection, int currentTime,  vec3_t torsoAngles ) {
line 2353
;2350:	int		t;
;2351:	float	f;
;2352:
;2353:	t = currentTime - painTime;
ADDRLP4 0
ADDRFP4 8
INDIRI4
ADDRFP4 0
INDIRI4
SUBI4
ASGNI4
line 2354
;2354:	if ( t >= PAIN_TWITCH_TIME ) {
ADDRLP4 0
INDIRI4
CNSTI4 200
LTI4 $829
line 2355
;2355:		return;
ADDRGP4 $828
JUMPV
LABELV $829
line 2358
;2356:	}
;2357:
;2358:	f = 1.0 - (float)t / PAIN_TWITCH_TIME;
ADDRLP4 4
CNSTF4 1065353216
ADDRLP4 0
INDIRI4
CVIF4 4
CNSTF4 1128792064
DIVF4
SUBF4
ASGNF4
line 2360
;2359:
;2360:	if ( painDirection ) {
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $831
line 2361
;2361:		torsoAngles[ROLL] += 20 * f;
ADDRLP4 8
ADDRFP4 12
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRF4
CNSTF4 1101004800
ADDRLP4 4
INDIRF4
MULF4
ADDF4
ASGNF4
line 2362
;2362:	} else {
ADDRGP4 $832
JUMPV
LABELV $831
line 2363
;2363:		torsoAngles[ROLL] -= 20 * f;
ADDRLP4 8
ADDRFP4 12
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRF4
CNSTF4 1101004800
ADDRLP4 4
INDIRF4
MULF4
SUBF4
ASGNF4
line 2364
;2364:	}
LABELV $832
line 2365
;2365:}
LABELV $828
endproc BG_AddPainTwitch 12 0
data
align 4
LABELV $834
byte 4 0
byte 4 22
byte 4 45
byte 4 -22
byte 4 0
byte 4 22
byte 4 -45
byte 4 -22
export BG_G2PlayerAngles
code
proc BG_G2PlayerAngles 120 28
line 2369
;2366:
;2367:void BG_G2PlayerAngles( vec3_t startAngles, vec3_t legs[3], vec3_t legsAngles, int painTime, int painDirection, int currentTime,
;2368:					   qboolean *torso_yawing, float *torso_yawAngle, qboolean *torso_pitching, float *torso_pitchAngle, qboolean *legs_yawing, float *legs_yawAngle,
;2369:					   int frameTime, vec3_t velocity, int legsAnim, int torsoAnim, qboolean dead, float movementDir, void *ghoul2, qhandle_t *modelList, int weapon){
line 2376
;2370:	vec3_t		torsoAngles, headAngles;
;2371:	float		dest;
;2372:	static	int	movementOffsets[8] = { 0, 22, 45, -22, 0, 22, -45, -22 };
;2373:	float		speed;
;2374:	int			dir;
;2375:
;2376:	VectorCopy( startAngles, headAngles );
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 2377
;2377:	headAngles[YAW] = AngleMod( headAngles[YAW] );
ADDRLP4 12+4
INDIRF4
ARGF4
ADDRLP4 36
ADDRGP4 AngleMod
CALLF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 36
INDIRF4
ASGNF4
line 2378
;2378:	VectorClear( legsAngles );
ADDRLP4 40
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 44
CNSTF4 0
ASGNF4
ADDRLP4 40
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 44
INDIRF4
ASGNF4
ADDRLP4 40
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 44
INDIRF4
ASGNF4
ADDRLP4 40
INDIRP4
ADDRLP4 44
INDIRF4
ASGNF4
line 2379
;2379:	VectorClear( torsoAngles );
ADDRLP4 48
CNSTF4 0
ASGNF4
ADDRLP4 0+8
ADDRLP4 48
INDIRF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 48
INDIRF4
ASGNF4
ADDRLP4 0
ADDRLP4 48
INDIRF4
ASGNF4
line 2384
;2380:
;2381:	// --------- yaw -------------
;2382:
;2383:	// allow yaw to drift a bit
;2384:	if ( ( legsAnim & ~ANIM_TOGGLEBIT ) != WeaponReadyAnim[weapon] 
ADDRLP4 52
CNSTI4 -2049
ASGNI4
ADDRLP4 56
ADDRFP4 80
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 WeaponReadyAnim
ADDP4
INDIRI4
ASGNI4
ADDRFP4 56
INDIRI4
ADDRLP4 52
INDIRI4
BANDI4
ADDRLP4 56
INDIRI4
NEI4 $841
ADDRFP4 60
INDIRI4
ADDRLP4 52
INDIRI4
BANDI4
ADDRLP4 56
INDIRI4
EQI4 $839
LABELV $841
line 2385
;2385:		|| ( torsoAnim & ~ANIM_TOGGLEBIT ) != WeaponReadyAnim[weapon]  ) {
line 2387
;2386:		// if not standing still, always point all in the same direction
;2387:		*torso_yawing = qtrue;	// always center
ADDRFP4 24
INDIRP4
CNSTI4 1
ASGNI4
line 2388
;2388:		*torso_pitching = qtrue;	// always center
ADDRFP4 32
INDIRP4
CNSTI4 1
ASGNI4
line 2389
;2389:		*legs_yawing = qtrue;	// always center
ADDRFP4 40
INDIRP4
CNSTI4 1
ASGNI4
line 2390
;2390:	}
LABELV $839
line 2393
;2391:
;2392:	// adjust legs for movement dir
;2393:	if (dead  ) {
ADDRFP4 64
INDIRI4
CNSTI4 0
EQI4 $842
line 2395
;2394:		// don't let dead bodies twitch
;2395:		dir = 0;
ADDRLP4 28
CNSTI4 0
ASGNI4
line 2396
;2396:	} else {
ADDRGP4 $843
JUMPV
LABELV $842
line 2397
;2397:		dir = movementDir;
ADDRLP4 28
ADDRFP4 68
INDIRF4
CVFI4 4
ASGNI4
line 2401
;2398://		if ( dir < 0 || dir > 7 ) {
;2399://			CG_Error( "Bad player movement angle" );
;2400://		}
;2401:	}
LABELV $843
line 2402
;2402:	legsAngles[YAW] = headAngles[YAW] + movementOffsets[ dir ];
ADDRFP4 8
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 12+4
INDIRF4
ADDRLP4 28
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $834
ADDP4
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 2403
;2403:	torsoAngles[YAW] = headAngles[YAW] + 0.25 * movementOffsets[ dir ];
ADDRLP4 0+4
ADDRLP4 12+4
INDIRF4
CNSTF4 1048576000
ADDRLP4 28
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $834
ADDP4
INDIRI4
CVIF4 4
MULF4
ADDF4
ASGNF4
line 2406
;2404:
;2405:	// torso
;2406:	BG_SwingAngles( torsoAngles[YAW], 25, 90, /*cg_swingSpeed.value*/ 0.3, torso_yawAngle, torso_yawing, frameTime );
ADDRLP4 0+4
INDIRF4
ARGF4
CNSTF4 1103626240
ARGF4
CNSTF4 1119092736
ARGF4
CNSTF4 1050253722
ARGF4
ADDRFP4 28
INDIRP4
ARGP4
ADDRFP4 24
INDIRP4
ARGP4
ADDRFP4 48
INDIRI4
ARGI4
ADDRGP4 BG_SwingAngles
CALLV
pop
line 2407
;2407:	BG_SwingAngles( legsAngles[YAW], 40, 90, /*cg_swingSpeed.value*/ 0.3, legs_yawAngle, legs_yawing, frameTime );
ADDRFP4 8
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ARGF4
CNSTF4 1109393408
ARGF4
CNSTF4 1119092736
ARGF4
CNSTF4 1050253722
ARGF4
ADDRFP4 44
INDIRP4
ARGP4
ADDRFP4 40
INDIRP4
ARGP4
ADDRFP4 48
INDIRI4
ARGI4
ADDRGP4 BG_SwingAngles
CALLV
pop
line 2409
;2408:
;2409:	torsoAngles[YAW] = *torso_yawAngle;
ADDRLP4 0+4
ADDRFP4 28
INDIRP4
INDIRF4
ASGNF4
line 2410
;2410:	legsAngles[YAW] = *legs_yawAngle;
ADDRFP4 8
INDIRP4
CNSTI4 4
ADDP4
ADDRFP4 44
INDIRP4
INDIRF4
ASGNF4
line 2415
;2411:
;2412:	// --------- pitch -------------
;2413:
;2414:	// only show a fraction of the pitch angle in the torso
;2415:	if ( headAngles[PITCH] > 180 ) {
ADDRLP4 12
INDIRF4
CNSTF4 1127481344
LEF4 $849
line 2416
;2416:		dest = (-360 + headAngles[PITCH]) * 0.75;
ADDRLP4 32
CNSTF4 1061158912
ADDRLP4 12
INDIRF4
CNSTF4 3283353600
ADDF4
MULF4
ASGNF4
line 2417
;2417:	} else {
ADDRGP4 $850
JUMPV
LABELV $849
line 2418
;2418:		dest = headAngles[PITCH] * 0.75;
ADDRLP4 32
CNSTF4 1061158912
ADDRLP4 12
INDIRF4
MULF4
ASGNF4
line 2419
;2419:	}
LABELV $850
line 2420
;2420:	BG_SwingAngles( dest, 15, 30, 0.1, torso_pitchAngle, torso_pitching, frameTime );
ADDRLP4 32
INDIRF4
ARGF4
CNSTF4 1097859072
ARGF4
CNSTF4 1106247680
ARGF4
CNSTF4 1036831949
ARGF4
ADDRFP4 36
INDIRP4
ARGP4
ADDRFP4 32
INDIRP4
ARGP4
ADDRFP4 48
INDIRI4
ARGI4
ADDRGP4 BG_SwingAngles
CALLV
pop
line 2421
;2421:	torsoAngles[PITCH] = *torso_pitchAngle;
ADDRLP4 0
ADDRFP4 36
INDIRP4
INDIRF4
ASGNF4
line 2426
;2422:
;2423:	// --------- roll -------------
;2424:
;2425:	// lean towards the direction of travel
;2426:	speed = VectorNormalize( velocity );
ADDRFP4 52
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 VectorNormalize
CALLF4
ASGNF4
ADDRLP4 24
ADDRLP4 60
INDIRF4
ASGNF4
line 2427
;2427:	if ( speed ) {
ADDRLP4 24
INDIRF4
CNSTF4 0
EQF4 $851
line 2431
;2428:		vec3_t	axis[3];
;2429:		float	side;
;2430:
;2431:		speed *= 0.05;
ADDRLP4 24
CNSTF4 1028443341
ADDRLP4 24
INDIRF4
MULF4
ASGNF4
line 2433
;2432:
;2433:		AnglesToAxis( legsAngles, axis );
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 64
ARGP4
ADDRGP4 AnglesToAxis
CALLV
pop
line 2434
;2434:		side = speed * DotProduct( velocity, axis[1] );
ADDRLP4 104
ADDRFP4 52
INDIRP4
ASGNP4
ADDRLP4 100
ADDRLP4 24
INDIRF4
ADDRLP4 104
INDIRP4
INDIRF4
ADDRLP4 64+12
INDIRF4
MULF4
ADDRLP4 104
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 64+12+4
INDIRF4
MULF4
ADDF4
ADDRLP4 104
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRLP4 64+12+8
INDIRF4
MULF4
ADDF4
MULF4
ASGNF4
line 2435
;2435:		legsAngles[ROLL] -= side;
ADDRLP4 108
ADDRFP4 8
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 108
INDIRP4
ADDRLP4 108
INDIRP4
INDIRF4
ADDRLP4 100
INDIRF4
SUBF4
ASGNF4
line 2437
;2436:
;2437:		side = speed * DotProduct( velocity, axis[0] );
ADDRLP4 112
ADDRFP4 52
INDIRP4
ASGNP4
ADDRLP4 100
ADDRLP4 24
INDIRF4
ADDRLP4 112
INDIRP4
INDIRF4
ADDRLP4 64
INDIRF4
MULF4
ADDRLP4 112
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 64+4
INDIRF4
MULF4
ADDF4
ADDRLP4 112
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRLP4 64+8
INDIRF4
MULF4
ADDF4
MULF4
ASGNF4
line 2438
;2438:		legsAngles[PITCH] += side;
ADDRLP4 116
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 116
INDIRP4
ADDRLP4 116
INDIRP4
INDIRF4
ADDRLP4 100
INDIRF4
ADDF4
ASGNF4
line 2439
;2439:	}
LABELV $851
line 2442
;2440:
;2441:	// pain twitch
;2442:	BG_AddPainTwitch( painTime, painDirection, currentTime, torsoAngles );
ADDRFP4 12
INDIRI4
ARGI4
ADDRFP4 16
INDIRI4
ARGI4
ADDRFP4 20
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BG_AddPainTwitch
CALLV
pop
line 2445
;2443:
;2444:	// pull the angles back out of the hierarchial chain
;2445:	AnglesSubtract( headAngles, torsoAngles, headAngles );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 12
ARGP4
ADDRGP4 AnglesSubtract
CALLV
pop
line 2446
;2446:	AnglesSubtract( torsoAngles, legsAngles, torsoAngles );
ADDRLP4 0
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 AnglesSubtract
CALLV
pop
line 2447
;2447:	AnglesToAxis( legsAngles, legs );
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 AnglesToAxis
CALLV
pop
line 2452
;2448:	// we assume that model 0 is the player model.
;2449://g2r	trap_G2API_SetBoneAngles(ghoul2, 0, "upper_lumbar", torsoAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, modelList, 0, currentTime); 
;2450://g2r	trap_G2API_SetBoneAngles(ghoul2, 0, "cranium", headAngles, BONE_ANGLES_POSTMULT, POSITIVE_Z, NEGATIVE_Y, POSITIVE_X, modelList,0, currentTime); 
;2451:
;2452:}
LABELV $833
endproc BG_G2PlayerAngles 120 28
data
align 4
LABELV bg_poolSize
byte 4 0
align 4
LABELV bg_poolTail
byte 4 2048000
export BG_Alloc
code
proc BG_Alloc 8 16
line 2461
;2453:
;2454:#define MAX_POOL_SIZE	2048000 //1024000
;2455:
;2456:static char		bg_pool[MAX_POOL_SIZE];
;2457:static int		bg_poolSize = 0;
;2458:static int		bg_poolTail = MAX_POOL_SIZE;
;2459:
;2460:void *BG_Alloc ( int size )
;2461:{
line 2462
;2462:	bg_poolSize = ((bg_poolSize + 0x00000003) & 0xfffffffc);
ADDRLP4 0
ADDRGP4 bg_poolSize
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 3
ADDI4
CVIU4 4
CNSTU4 4294967292
BANDU4
CVUI4 4
ASGNI4
line 2464
;2463:
;2464:	if (bg_poolSize + size > bg_poolTail)
ADDRGP4 bg_poolSize
INDIRI4
ADDRFP4 0
INDIRI4
ADDI4
ADDRGP4 bg_poolTail
INDIRI4
LEI4 $861
line 2465
;2465:	{
line 2466
;2466:		Com_Error( ERR_DROP, "BG_Alloc: buffer exceeded tail (%d > %d)", bg_poolSize + size, bg_poolTail);
CNSTI4 1
ARGI4
ADDRGP4 $863
ARGP4
ADDRGP4 bg_poolSize
INDIRI4
ADDRFP4 0
INDIRI4
ADDI4
ARGI4
ADDRGP4 bg_poolTail
INDIRI4
ARGI4
ADDRGP4 Com_Error
CALLV
pop
line 2467
;2467:		return 0;
CNSTP4 0
RETP4
ADDRGP4 $860
JUMPV
LABELV $861
line 2470
;2468:	}
;2469:
;2470:	bg_poolSize += size;
ADDRLP4 4
ADDRGP4 bg_poolSize
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
ADDRFP4 0
INDIRI4
ADDI4
ASGNI4
line 2472
;2471:
;2472:	return &bg_pool[bg_poolSize-size];
ADDRGP4 bg_poolSize
INDIRI4
ADDRFP4 0
INDIRI4
SUBI4
ADDRGP4 bg_pool
ADDP4
RETP4
LABELV $860
endproc BG_Alloc 8 16
export BG_AllocUnaligned
proc BG_AllocUnaligned 4 16
line 2476
;2473:}
;2474:
;2475:void *BG_AllocUnaligned ( int size )
;2476:{
line 2477
;2477:	if (bg_poolSize + size > bg_poolTail)
ADDRGP4 bg_poolSize
INDIRI4
ADDRFP4 0
INDIRI4
ADDI4
ADDRGP4 bg_poolTail
INDIRI4
LEI4 $865
line 2478
;2478:	{
line 2479
;2479:		Com_Error( ERR_DROP, "BG_AllocUnaligned: buffer exceeded tail (%d > %d)", bg_poolSize + size, bg_poolTail);
CNSTI4 1
ARGI4
ADDRGP4 $867
ARGP4
ADDRGP4 bg_poolSize
INDIRI4
ADDRFP4 0
INDIRI4
ADDI4
ARGI4
ADDRGP4 bg_poolTail
INDIRI4
ARGI4
ADDRGP4 Com_Error
CALLV
pop
line 2480
;2480:		return 0;
CNSTP4 0
RETP4
ADDRGP4 $864
JUMPV
LABELV $865
line 2483
;2481:	}
;2482:
;2483:	bg_poolSize += size;
ADDRLP4 0
ADDRGP4 bg_poolSize
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRFP4 0
INDIRI4
ADDI4
ASGNI4
line 2485
;2484:
;2485:	return &bg_pool[bg_poolSize-size];
ADDRGP4 bg_poolSize
INDIRI4
ADDRFP4 0
INDIRI4
SUBI4
ADDRGP4 bg_pool
ADDP4
RETP4
LABELV $864
endproc BG_AllocUnaligned 4 16
export BG_TempAlloc
proc BG_TempAlloc 4 16
line 2489
;2486:}
;2487:
;2488:void *BG_TempAlloc( int size )
;2489:{
line 2490
;2490:	size = ((size + 0x00000003) & 0xfffffffc);
ADDRFP4 0
ADDRFP4 0
INDIRI4
CNSTI4 3
ADDI4
CVIU4 4
CNSTU4 4294967292
BANDU4
CVUI4 4
ASGNI4
line 2492
;2491:
;2492:	if (bg_poolTail - size < bg_poolSize)
ADDRGP4 bg_poolTail
INDIRI4
ADDRFP4 0
INDIRI4
SUBI4
ADDRGP4 bg_poolSize
INDIRI4
GEI4 $869
line 2493
;2493:	{
line 2494
;2494:		Com_Error( ERR_DROP, "BG_TempAlloc: buffer exceeded head (%d > %d)", bg_poolTail - size, bg_poolSize);
CNSTI4 1
ARGI4
ADDRGP4 $871
ARGP4
ADDRGP4 bg_poolTail
INDIRI4
ADDRFP4 0
INDIRI4
SUBI4
ARGI4
ADDRGP4 bg_poolSize
INDIRI4
ARGI4
ADDRGP4 Com_Error
CALLV
pop
line 2495
;2495:		return 0;
CNSTP4 0
RETP4
ADDRGP4 $868
JUMPV
LABELV $869
line 2498
;2496:	}
;2497:
;2498:	bg_poolTail -= size;
ADDRLP4 0
ADDRGP4 bg_poolTail
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRFP4 0
INDIRI4
SUBI4
ASGNI4
line 2500
;2499:
;2500:	return &bg_pool[bg_poolTail];
ADDRGP4 bg_poolTail
INDIRI4
ADDRGP4 bg_pool
ADDP4
RETP4
LABELV $868
endproc BG_TempAlloc 4 16
export BG_TempFree
proc BG_TempFree 4 16
line 2504
;2501:}
;2502:
;2503:void BG_TempFree( int size )
;2504:{
line 2505
;2505:	size = ((size + 0x00000003) & 0xfffffffc);
ADDRFP4 0
ADDRFP4 0
INDIRI4
CNSTI4 3
ADDI4
CVIU4 4
CNSTU4 4294967292
BANDU4
CVUI4 4
ASGNI4
line 2507
;2506:
;2507:	if (bg_poolTail+size > MAX_POOL_SIZE)
ADDRGP4 bg_poolTail
INDIRI4
ADDRFP4 0
INDIRI4
ADDI4
CNSTI4 2048000
LEI4 $873
line 2508
;2508:	{
line 2509
;2509:		Com_Error( ERR_DROP, "BG_TempFree: tail greater than size (%d > %d)", bg_poolTail+size, MAX_POOL_SIZE );
CNSTI4 1
ARGI4
ADDRGP4 $875
ARGP4
ADDRGP4 bg_poolTail
INDIRI4
ADDRFP4 0
INDIRI4
ADDI4
ARGI4
CNSTI4 2048000
ARGI4
ADDRGP4 Com_Error
CALLV
pop
line 2510
;2510:	}
LABELV $873
line 2512
;2511:
;2512:	bg_poolTail += size;
ADDRLP4 0
ADDRGP4 bg_poolTail
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRFP4 0
INDIRI4
ADDI4
ASGNI4
line 2513
;2513:}
LABELV $872
endproc BG_TempFree 4 16
export BG_StringAlloc
proc BG_StringAlloc 12 8
line 2516
;2514:
;2515:char *BG_StringAlloc ( const char *source )
;2516:{
line 2519
;2517:	char *dest;
;2518:
;2519:	dest = BG_Alloc ( strlen ( source ) + 1 );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 8
ADDRGP4 BG_Alloc
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 2520
;2520:	strcpy ( dest, source );
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 2521
;2521:	return dest;
ADDRLP4 0
INDIRP4
RETP4
LABELV $876
endproc BG_StringAlloc 12 8
export BG_OutOfMemory
proc BG_OutOfMemory 4 0
line 2525
;2522:}
;2523:
;2524:qboolean BG_OutOfMemory ( void )
;2525:{
line 2526
;2526:	return bg_poolSize >= MAX_POOL_SIZE;
ADDRGP4 bg_poolSize
INDIRI4
CNSTI4 2048000
LTI4 $879
ADDRLP4 0
CNSTI4 1
ASGNI4
ADDRGP4 $880
JUMPV
LABELV $879
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $880
ADDRLP4 0
INDIRI4
RETI4
LABELV $877
endproc BG_OutOfMemory 4 0
bss
align 1
LABELV bg_pool
skip 2048000
import g2WeaponInstances
import CG_CheckPlayerG2Weapons
import CG_CopyG2WeaponInstance
import CG_ShutDownG2Weapons
import CG_InitG2Weapons
import CG_CreateBBRefEnts
import CG_SetGhoul2Info
import CG_Init_CGents
import CG_Init_CG
import trap_G2API_SetNewOrigin
import trap_G2API_SetSurfaceOnOff
import trap_G2API_SetRootSurface
import trap_G2API_SetBoneAnim
import trap_G2API_GetGLAName
import trap_G2API_SetBoneAngles
import trap_G2API_CleanGhoul2Models
import trap_G2API_SetBoltInfo
import trap_G2API_AddBolt
import trap_G2API_RemoveGhoul2Model
import trap_G2API_HasGhoul2ModelOnIndex
import trap_G2API_DuplicateGhoul2Instance
import trap_G2API_CopySpecificGhoul2Model
import trap_G2API_CopyGhoul2Instance
import trap_G2API_GiveMeVectorFromMatrix
import trap_G2API_InitGhoul2Model
import trap_G2API_GetBoltMatrix_NoReconstruct
import trap_G2API_GetBoltMatrix
import trap_G2_HaveWeGhoul2Models
import trap_G2_SetGhoul2ModelIndexes
import trap_G2_ListModelBones
import trap_G2_ListModelSurfaces
import FX_ForceDrained
import FX_BlasterWeaponHitPlayer
import FX_BlasterWeaponHitWall
import FX_BlasterAltFireThink
import FX_BlasterProjectileThink
import FX_BryarAltHitPlayer
import FX_BryarHitPlayer
import FX_BryarAltHitWall
import FX_BryarHitWall
import CG_Spark
import FX_TurretHitPlayer
import FX_TurretHitWall
import FX_TurretProjectileThink
import CG_NewParticleArea
import initparticles
import CG_GetStripEdString
import CG_ParticleExplosion
import CG_ParticleMisc
import CG_ParticleDust
import CG_ParticleSparks
import CG_ParticleBulletDebris
import CG_ParticleSnowFlurry
import CG_AddParticleShrapnel
import CG_ParticleSmoke
import CG_ParticleSnow
import CG_AddParticles
import CG_ClearParticles
import trap_ROFF_Purge_Ent
import trap_ROFF_Play
import trap_ROFF_Cache
import trap_ROFF_UpdateEntities
import trap_ROFF_Clean
import trap_CG_RegisterSharedMemory
import trap_SP_GetStringTextString
import trap_SP_Print
import trap_FX_AddSprite
import trap_FX_AddPrimitive
import trap_FX_AddBezier
import trap_FX_AddPoly
import trap_FX_AdjustTime
import trap_FX_FreeSystem
import trap_FX_InitSystem
import trap_FX_AddScheduledEffects
import trap_FX_PlayBoltedEffectID
import trap_FX_PlayEntityEffectID
import trap_FX_PlayEffectID
import trap_FX_PlaySimpleEffectID
import trap_FX_PlayEntityEffect
import trap_FX_PlayEffect
import trap_FX_PlaySimpleEffect
import trap_FX_RegisterEffect
import trap_R_inPVS
import trap_GetEntityToken
import trap_getCameraInfo
import trap_startCamera
import trap_loadCamera
import trap_SnapVector
import trap_CIN_SetExtents
import trap_CIN_DrawCinematic
import trap_CIN_RunCinematic
import trap_CIN_StopCinematic
import trap_CIN_PlayCinematic
import trap_Key_GetKey
import trap_Key_SetCatcher
import trap_Key_GetCatcher
import trap_Key_IsDown
import trap_MemoryRemaining
import testPrintFloat
import testPrintInt
import trap_OpenUIMenu
import trap_SetClientTurnExtent
import trap_SetClientForceAngle
import trap_SetUserCmdValue
import trap_GetUserCmd
import trap_GetCurrentCmdNumber
import trap_GetServerCommand
import trap_GetSnapshot
import trap_GetCurrentSnapshotNumber
import trap_GetGameState
import trap_GetGlconfig
import trap_FX_AddLine
import trap_R_GetBModelVerts
import trap_R_SetLightStyle
import trap_R_GetLightStyle
import trap_R_RemapShader
import trap_R_DrawRotatePic2
import trap_R_DrawRotatePic
import trap_R_LerpTag
import trap_R_ModelBounds
import trap_R_DrawStretchPic
import trap_R_SetColor
import trap_R_RenderScene
import trap_R_LightForPoint
import trap_R_AddLightToScene
import trap_R_AddPolysToScene
import trap_R_AddPolyToScene
import trap_R_AddRefEntityToScene
import trap_R_ClearScene
import trap_AnyLanguage_ReadCharFromString
import trap_R_Font_DrawString
import trap_R_Font_HeightPixels
import trap_R_Font_StrLenChars
import trap_R_Font_StrLenPixels
import trap_R_RegisterFont
import trap_R_RegisterShaderNoMip
import trap_R_RegisterShader
import trap_R_RegisterSkin
import trap_R_RegisterModel
import trap_R_LoadWorldMap
import trap_S_StopBackgroundTrack
import trap_S_StartBackgroundTrack
import trap_S_RegisterSound
import trap_S_Respatialize
import trap_S_UpdateEntityPosition
import trap_S_AddRealLoopingSound
import trap_S_AddLoopingSound
import trap_S_ClearLoopingSounds
import trap_S_StartLocalSound
import trap_S_StopLoopingSound
import trap_S_StartSound
import trap_S_MuteSound
import trap_CM_MarkFragments
import trap_CM_TransformedBoxTrace
import trap_CM_BoxTrace
import trap_CM_TransformedPointContents
import trap_CM_PointContents
import trap_CM_TempBoxModel
import trap_CM_InlineModel
import trap_CM_NumInlineModels
import trap_CM_LoadMap
import trap_UpdateScreen
import trap_SendClientCommand
import trap_AddCommand
import trap_SendConsoleCommand
import trap_FS_FCloseFile
import trap_FS_Write
import trap_FS_Read
import trap_FS_FOpenFile
import trap_Args
import trap_Argv
import trap_Argc
import trap_Cvar_VariableStringBuffer
import trap_Cvar_Set
import trap_Cvar_Update
import trap_Cvar_Register
import trap_Milliseconds
import trap_Error
import trap_Print
import CG_SagaObjectiveCompleted
import CG_SagaRoundOver
import CG_InitSagaMode
import CG_CheckChangedPredictableEvents
import CG_TransitionPlayerState
import CG_Respawn
import CG_IsMindTricked
import CG_PlayBufferedVoiceChats
import CG_VoiceChatLocal
import CG_ShaderStateChanged
import CG_LoadVoiceChats
import CG_SetConfigValues
import CG_ParseServerinfo
import CG_ExecuteNewServerCommands
import CG_InitConsoleCommands
import CG_ConsoleCommand
import CG_DrawOldTourneyScoreboard
import CG_DrawOldScoreboard
import CG_DrawInformation
import CG_LoadingClient
import CG_LoadingItem
import CG_LoadingString
import CG_ProcessSnapshots
import CG_InitGlass
import CG_TestLine
import CG_SurfaceExplosion
import CG_MakeExplosion
import CG_Bleed
import CG_BigExplode
import CG_GibPlayer
import CG_ScorePlum
import CG_CreateDebris
import CG_GlassShatter
import CG_BubbleTrail
import CG_SmokePuff
import CG_AddLocalEntities
import CG_AllocLocalEntity
import CG_InitLocalEntities
import CG_ImpactMark
import CG_AddMarks
import CG_InitMarkPolys
import CG_OutOfAmmoChange
import CG_DrawIconBackground
import CG_DrawWeaponSelect
import CG_AddPlayerWeapon
import CG_AddViewWeapon
import CG_MissileHitPlayer
import CG_MissileHitWall
import CG_FireWeapon
import CG_RegisterItemVisuals
import CG_RegisterWeapon
import CG_Weapon_f
import CG_PrevWeapon_f
import CG_NextWeapon_f
import CG_GetClientWeaponMuzzleBoltPoint
import TurretClientRun
import ScaleModelAxis
import CG_PositionRotatedEntityOnTag
import CG_PositionEntityOnTag
import CG_AdjustPositionForMover
import CG_Beam
import CG_ManualEntityRender
import CG_AddPacketEntities
import CG_SetEntitySoundPosition
import CG_ReattachLimb
import CG_PainEvent
import CG_EntityEvent
import CG_PlaceString
import CG_CheckEvents
import CG_LoadDeferredPlayers
import CG_PredictPlayerState
import CG_Trace
import CG_PointContents
import CG_BuildSolidList
import CG_PlayerShieldHit
import CG_CustomSound
import CG_NewClientInfo
import CG_AddRefEntityWithPowerups
import CG_ResetPlayerEntity
import CG_Player
import CG_StatusHandle
import CG_OtherTeamHasFlag
import CG_YourTeamHasFlag
import CG_GameTypeString
import CG_CheckOrderPending
import CG_Text_PaintChar
import CG_Draw3DModel
import CG_GetKillerText
import CG_GetGameStatusText
import CG_GetTeamColor
import CG_InitTeamChat
import CG_SetPrintString
import CG_ShowResponseHead
import CG_DeferMenuScript
import CG_RunMenuScript
import CG_OwnerDrawVisible
import CG_GetValue
import CG_SelectNextPlayer
import CG_SelectPrevPlayer
import CG_Text_Height
import CG_Text_Width
import CG_Text_Paint
import CG_OwnerDraw
import CG_DrawTeamBackground
import CG_DrawFlagModel
import CG_DrawActive
import CG_DrawHead
import CG_CenterPrint
import CG_AddLagometerSnapshotInfo
import CG_AddLagometerFrameInfo
import teamChat2
import teamChat1
import systemChat
import drawTeamOverlayModificationCount
import numSortedTeamPlayers
import sortedTeamPlayers
import CG_DrawTopBottom
import CG_DrawSides
import CG_DrawRect
import UI_DrawScaledProportionalString
import UI_DrawProportionalString
import CG_GetColorForHealth
import CG_ColorForHealth
import CG_TileClear
import CG_TeamColor
import CG_FadeColor
import CG_DrawStrlen
import CG_DrawSmallStringColor
import CG_DrawSmallString
import CG_DrawBigStringColor
import CG_DrawBigString
import CG_DrawStringExt
import CG_DrawNumField
import CG_DrawString
import CG_DrawRotatePic2
import CG_DrawRotatePic
import CG_DrawPic
import CG_FillRect
import CG_TestModelAnimate_f
import CG_TestModelSetAnglespost_f
import CG_TestModelSetAnglespre_f
import CG_ListModelBones_f
import CG_ListModelSurfaces_f
import CG_TestModelSurfaceOnOff_f
import CG_TestG2Model_f
import CG_DrawActiveFrame
import CG_AddBufferedSound
import CG_ZoomUp_f
import CG_ZoomDown_f
import CG_TestModelPrevSkin_f
import CG_TestModelNextSkin_f
import CG_TestModelPrevFrame_f
import CG_TestModelNextFrame_f
import CG_TestGun_f
import CG_TestModel_f
import CG_PrevForcePower_f
import CG_NextForcePower_f
import CG_PrevInventory_f
import CG_NextInventory_f
import CG_BuildSpectatorString
import CG_SetScoreSelection
import CG_RankRunFrame
import CG_EventHandling
import CG_MouseEvent
import CG_KeyEvent
import CG_LoadMenus
import CG_LastAttacker
import CG_CrosshairPlayer
import CG_UpdateCvars
import CG_StartMusic
import CG_Error
import CG_Printf
import CG_Argv
import CG_ConfigString
import cg_debugBB
import ui_myteam
import cg_recordSPDemoName
import cg_recordSPDemo
import cg_singlePlayerActive
import cg_enableBreath
import cg_enableDust
import cg_singlePlayer
import cg_currentSelectedPlayerName
import cg_currentSelectedPlayer
import cg_blueTeamName
import cg_redTeamName
import cg_trueLightning
import cg_noProjectileTrail
import cg_noTaunt
import cg_bigFont
import cg_smallFont
import cg_cameraMode
import cg_timescale
import cg_timescaleFadeSpeed
import cg_timescaleFadeEnd
import cg_cameraOrbitDelay
import cg_cameraOrbit
import pmove_msec
import pmove_fixed
import cg_smoothClients
import cg_hudFiles
import cg_scorePlum
import cg_noVoiceText
import cg_noVoiceChats
import cg_teamChatsOnly
import cg_drawFriend
import cg_deferPlayers
import cg_predictItems
import cg_blood
import cg_paused
import cg_buildScript
import cg_forceModel
import cg_stats
import cg_teamChatHeight
import cg_teamChatTime
import cg_synchronousClients
import cg_drawEnemyInfo
import cg_lagometer
import cg_stereoSeparation
import cg_thirdPersonHorzOffset
import cg_thirdPersonAlpha
import cg_thirdPersonTargetDamp
import cg_thirdPersonCameraDamp
import cg_thirdPersonVertOffset
import cg_thirdPersonPitchOffset
import cg_thirdPersonAngle
import cg_thirdPersonRange
import cg_thirdPerson
import cg_dismember
import cg_animBlend
import cg_auraShell
import cg_speedTrail
import cg_saberTrail
import cg_saberContact
import cg_swingAngles
import cg_zoomFov
import cg_fov
import cg_simpleItems
import cg_ignore
import cg_autoswitch
import cg_tracerLength
import cg_tracerWidth
import cg_tracerChance
import cg_viewsize
import cg_drawGun
import cg_gun_z
import cg_gun_y
import cg_gun_x
import cg_gun_frame
import cg_addMarks
import cg_footsteps
import cg_showmiss
import cg_noPlayerAnims
import cg_nopredict
import cg_errorDecay
import cg_debugEvents
import cg_debugPosition
import cg_debugAnim
import cg_animSpeed
import cg_draw2D
import cg_drawStatus
import cg_crosshairHealth
import cg_crosshairSize
import cg_crosshairY
import cg_crosshairX
import cg_teamOverlayUserinfo
import cg_drawTeamOverlay
import cg_drawRewards
import cg_dynamicCrosshair
import cg_drawScores
import cg_drawCrosshairNames
import cg_drawCrosshair
import cg_drawAmmoWarning
import cg_drawIcons
import cg_draw3dIcons
import cg_drawSnapshot
import cg_drawFPS
import cg_drawTimer
import cg_gibs
import cg_shadows
import cg_bobroll
import cg_bobpitch
import cg_bobup
import cg_runroll
import cg_runpitch
import cg_centertime
import cg_markPolys
import cg_items
import cg_weapons
import cg_entities
import cg
import cgs
import CGCam_SetMusicMult
import CGCam_Shake
import cgScreenEffects
import ammoTicPos
import forceTicPos
import BG_ParseAnimationFile
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
import saberMoveData
import Pmove
import PM_UpdateViewAngles
import pm
import bgGlobalAnimations
import BGPAFtextLoaded
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
LABELV $875
char 1 66
char 1 71
char 1 95
char 1 84
char 1 101
char 1 109
char 1 112
char 1 70
char 1 114
char 1 101
char 1 101
char 1 58
char 1 32
char 1 116
char 1 97
char 1 105
char 1 108
char 1 32
char 1 103
char 1 114
char 1 101
char 1 97
char 1 116
char 1 101
char 1 114
char 1 32
char 1 116
char 1 104
char 1 97
char 1 110
char 1 32
char 1 115
char 1 105
char 1 122
char 1 101
char 1 32
char 1 40
char 1 37
char 1 100
char 1 32
char 1 62
char 1 32
char 1 37
char 1 100
char 1 41
char 1 0
align 1
LABELV $871
char 1 66
char 1 71
char 1 95
char 1 84
char 1 101
char 1 109
char 1 112
char 1 65
char 1 108
char 1 108
char 1 111
char 1 99
char 1 58
char 1 32
char 1 98
char 1 117
char 1 102
char 1 102
char 1 101
char 1 114
char 1 32
char 1 101
char 1 120
char 1 99
char 1 101
char 1 101
char 1 100
char 1 101
char 1 100
char 1 32
char 1 104
char 1 101
char 1 97
char 1 100
char 1 32
char 1 40
char 1 37
char 1 100
char 1 32
char 1 62
char 1 32
char 1 37
char 1 100
char 1 41
char 1 0
align 1
LABELV $867
char 1 66
char 1 71
char 1 95
char 1 65
char 1 108
char 1 108
char 1 111
char 1 99
char 1 85
char 1 110
char 1 97
char 1 108
char 1 105
char 1 103
char 1 110
char 1 101
char 1 100
char 1 58
char 1 32
char 1 98
char 1 117
char 1 102
char 1 102
char 1 101
char 1 114
char 1 32
char 1 101
char 1 120
char 1 99
char 1 101
char 1 101
char 1 100
char 1 101
char 1 100
char 1 32
char 1 116
char 1 97
char 1 105
char 1 108
char 1 32
char 1 40
char 1 37
char 1 100
char 1 32
char 1 62
char 1 32
char 1 37
char 1 100
char 1 41
char 1 0
align 1
LABELV $863
char 1 66
char 1 71
char 1 95
char 1 65
char 1 108
char 1 108
char 1 111
char 1 99
char 1 58
char 1 32
char 1 98
char 1 117
char 1 102
char 1 102
char 1 101
char 1 114
char 1 32
char 1 101
char 1 120
char 1 99
char 1 101
char 1 101
char 1 100
char 1 101
char 1 100
char 1 32
char 1 116
char 1 97
char 1 105
char 1 108
char 1 32
char 1 40
char 1 37
char 1 100
char 1 32
char 1 62
char 1 32
char 1 37
char 1 100
char 1 41
char 1 0
align 1
LABELV $735
char 1 69
char 1 86
char 1 95
char 1 66
char 1 79
char 1 68
char 1 89
char 1 95
char 1 81
char 1 85
char 1 69
char 1 85
char 1 69
char 1 95
char 1 67
char 1 79
char 1 80
char 1 89
char 1 0
align 1
LABELV $734
char 1 69
char 1 86
char 1 95
char 1 84
char 1 65
char 1 85
char 1 78
char 1 84
char 1 95
char 1 80
char 1 65
char 1 84
char 1 82
char 1 79
char 1 76
char 1 0
align 1
LABELV $733
char 1 69
char 1 86
char 1 95
char 1 84
char 1 65
char 1 85
char 1 78
char 1 84
char 1 95
char 1 71
char 1 85
char 1 65
char 1 82
char 1 68
char 1 66
char 1 65
char 1 83
char 1 69
char 1 0
align 1
LABELV $732
char 1 69
char 1 86
char 1 95
char 1 84
char 1 65
char 1 85
char 1 78
char 1 84
char 1 95
char 1 71
char 1 69
char 1 84
char 1 70
char 1 76
char 1 65
char 1 71
char 1 0
align 1
LABELV $731
char 1 69
char 1 86
char 1 95
char 1 84
char 1 65
char 1 85
char 1 78
char 1 84
char 1 95
char 1 70
char 1 79
char 1 76
char 1 76
char 1 79
char 1 87
char 1 77
char 1 69
char 1 0
align 1
LABELV $730
char 1 69
char 1 86
char 1 95
char 1 84
char 1 65
char 1 85
char 1 78
char 1 84
char 1 95
char 1 78
char 1 79
char 1 0
align 1
LABELV $729
char 1 69
char 1 86
char 1 95
char 1 84
char 1 65
char 1 85
char 1 78
char 1 84
char 1 95
char 1 89
char 1 69
char 1 83
char 1 0
align 1
LABELV $728
char 1 69
char 1 86
char 1 95
char 1 84
char 1 65
char 1 85
char 1 78
char 1 84
char 1 0
align 1
LABELV $727
char 1 69
char 1 86
char 1 95
char 1 83
char 1 84
char 1 65
char 1 82
char 1 84
char 1 76
char 1 79
char 1 79
char 1 80
char 1 73
char 1 78
char 1 71
char 1 83
char 1 79
char 1 85
char 1 78
char 1 68
char 1 0
align 1
LABELV $726
char 1 69
char 1 86
char 1 95
char 1 83
char 1 84
char 1 79
char 1 80
char 1 76
char 1 79
char 1 79
char 1 80
char 1 73
char 1 78
char 1 71
char 1 83
char 1 79
char 1 85
char 1 78
char 1 68
char 1 0
align 1
LABELV $725
char 1 69
char 1 86
char 1 95
char 1 84
char 1 69
char 1 83
char 1 84
char 1 76
char 1 73
char 1 78
char 1 69
char 1 0
align 1
LABELV $724
char 1 69
char 1 86
char 1 95
char 1 68
char 1 69
char 1 66
char 1 85
char 1 71
char 1 95
char 1 76
char 1 73
char 1 78
char 1 69
char 1 0
align 1
LABELV $723
char 1 69
char 1 86
char 1 95
char 1 83
char 1 72
char 1 73
char 1 69
char 1 76
char 1 68
char 1 95
char 1 72
char 1 73
char 1 84
char 1 0
align 1
LABELV $722
char 1 69
char 1 86
char 1 95
char 1 87
char 1 69
char 1 65
char 1 80
char 1 79
char 1 78
char 1 95
char 1 67
char 1 72
char 1 65
char 1 82
char 1 71
char 1 69
char 1 95
char 1 65
char 1 76
char 1 84
char 1 0
align 1
LABELV $721
char 1 69
char 1 86
char 1 95
char 1 87
char 1 69
char 1 65
char 1 80
char 1 79
char 1 78
char 1 95
char 1 67
char 1 72
char 1 65
char 1 82
char 1 71
char 1 69
char 1 0
align 1
LABELV $720
char 1 69
char 1 86
char 1 95
char 1 83
char 1 69
char 1 84
char 1 95
char 1 70
char 1 79
char 1 82
char 1 67
char 1 69
char 1 95
char 1 68
char 1 73
char 1 83
char 1 65
char 1 66
char 1 76
char 1 69
char 1 0
align 1
LABELV $719
char 1 69
char 1 86
char 1 95
char 1 83
char 1 69
char 1 84
char 1 95
char 1 70
char 1 82
char 1 69
char 1 69
char 1 95
char 1 83
char 1 65
char 1 66
char 1 69
char 1 82
char 1 0
align 1
LABELV $718
char 1 69
char 1 86
char 1 95
char 1 71
char 1 73
char 1 86
char 1 69
char 1 95
char 1 78
char 1 69
char 1 87
char 1 95
char 1 82
char 1 65
char 1 78
char 1 75
char 1 0
align 1
LABELV $717
char 1 69
char 1 86
char 1 95
char 1 68
char 1 69
char 1 83
char 1 84
char 1 82
char 1 79
char 1 89
char 1 95
char 1 87
char 1 69
char 1 65
char 1 80
char 1 79
char 1 78
char 1 95
char 1 77
char 1 79
char 1 68
char 1 69
char 1 76
char 1 0
align 1
LABELV $716
char 1 69
char 1 86
char 1 95
char 1 68
char 1 69
char 1 83
char 1 84
char 1 82
char 1 79
char 1 89
char 1 95
char 1 71
char 1 72
char 1 79
char 1 85
char 1 76
char 1 50
char 1 95
char 1 73
char 1 78
char 1 83
char 1 84
char 1 65
char 1 78
char 1 67
char 1 69
char 1 0
align 1
LABELV $715
char 1 69
char 1 86
char 1 95
char 1 83
char 1 65
char 1 71
char 1 65
char 1 95
char 1 79
char 1 66
char 1 74
char 1 69
char 1 67
char 1 84
char 1 73
char 1 86
char 1 69
char 1 67
char 1 79
char 1 77
char 1 80
char 1 76
char 1 69
char 1 84
char 1 69
char 1 0
align 1
LABELV $714
char 1 69
char 1 86
char 1 95
char 1 83
char 1 65
char 1 71
char 1 65
char 1 95
char 1 82
char 1 79
char 1 85
char 1 78
char 1 68
char 1 79
char 1 86
char 1 69
char 1 82
char 1 0
align 1
LABELV $713
char 1 69
char 1 86
char 1 95
char 1 67
char 1 84
char 1 70
char 1 77
char 1 69
char 1 83
char 1 83
char 1 65
char 1 71
char 1 69
char 1 0
align 1
LABELV $712
char 1 69
char 1 86
char 1 95
char 1 83
char 1 67
char 1 79
char 1 82
char 1 69
char 1 80
char 1 76
char 1 85
char 1 77
char 1 0
align 1
LABELV $711
char 1 69
char 1 86
char 1 95
char 1 71
char 1 73
char 1 66
char 1 95
char 1 80
char 1 76
char 1 65
char 1 89
char 1 69
char 1 82
char 1 0
align 1
LABELV $710
char 1 69
char 1 86
char 1 95
char 1 70
char 1 79
char 1 82
char 1 67
char 1 69
char 1 95
char 1 68
char 1 82
char 1 65
char 1 73
char 1 78
char 1 69
char 1 68
char 1 0
align 1
LABELV $709
char 1 69
char 1 86
char 1 95
char 1 80
char 1 79
char 1 87
char 1 69
char 1 82
char 1 85
char 1 80
char 1 95
char 1 66
char 1 65
char 1 84
char 1 84
char 1 76
char 1 69
char 1 83
char 1 85
char 1 73
char 1 84
char 1 0
align 1
LABELV $708
char 1 69
char 1 86
char 1 95
char 1 80
char 1 79
char 1 87
char 1 69
char 1 82
char 1 85
char 1 80
char 1 95
char 1 81
char 1 85
char 1 65
char 1 68
char 1 0
align 1
LABELV $707
char 1 69
char 1 86
char 1 95
char 1 79
char 1 66
char 1 73
char 1 84
char 1 85
char 1 65
char 1 82
char 1 89
char 1 0
align 1
LABELV $706
char 1 69
char 1 86
char 1 95
char 1 68
char 1 69
char 1 65
char 1 84
char 1 72
char 1 51
char 1 0
align 1
LABELV $705
char 1 69
char 1 86
char 1 95
char 1 68
char 1 69
char 1 65
char 1 84
char 1 72
char 1 50
char 1 0
align 1
LABELV $704
char 1 69
char 1 86
char 1 95
char 1 68
char 1 69
char 1 65
char 1 84
char 1 72
char 1 49
char 1 0
align 1
LABELV $703
char 1 69
char 1 86
char 1 95
char 1 80
char 1 65
char 1 73
char 1 78
char 1 0
align 1
LABELV $702
char 1 69
char 1 86
char 1 95
char 1 66
char 1 85
char 1 76
char 1 76
char 1 69
char 1 84
char 1 0
align 1
LABELV $701
char 1 69
char 1 86
char 1 95
char 1 77
char 1 73
char 1 83
char 1 83
char 1 73
char 1 76
char 1 69
char 1 95
char 1 77
char 1 73
char 1 83
char 1 83
char 1 95
char 1 77
char 1 69
char 1 84
char 1 65
char 1 76
char 1 0
align 1
LABELV $700
char 1 69
char 1 86
char 1 95
char 1 77
char 1 73
char 1 83
char 1 83
char 1 73
char 1 76
char 1 69
char 1 95
char 1 77
char 1 73
char 1 83
char 1 83
char 1 0
align 1
LABELV $699
char 1 69
char 1 86
char 1 95
char 1 77
char 1 73
char 1 83
char 1 83
char 1 73
char 1 76
char 1 69
char 1 95
char 1 72
char 1 73
char 1 84
char 1 0
align 1
LABELV $698
char 1 69
char 1 86
char 1 95
char 1 68
char 1 69
char 1 66
char 1 82
char 1 73
char 1 83
char 1 0
align 1
LABELV $697
char 1 69
char 1 86
char 1 95
char 1 71
char 1 76
char 1 65
char 1 83
char 1 83
char 1 95
char 1 83
char 1 72
char 1 65
char 1 84
char 1 84
char 1 69
char 1 82
char 1 0
align 1
LABELV $696
char 1 69
char 1 86
char 1 95
char 1 80
char 1 76
char 1 65
char 1 89
char 1 95
char 1 82
char 1 79
char 1 70
char 1 70
char 1 0
align 1
LABELV $695
char 1 69
char 1 86
char 1 95
char 1 69
char 1 78
char 1 84
char 1 73
char 1 84
char 1 89
char 1 95
char 1 83
char 1 79
char 1 85
char 1 78
char 1 68
char 1 0
align 1
LABELV $694
char 1 69
char 1 86
char 1 95
char 1 71
char 1 76
char 1 79
char 1 66
char 1 65
char 1 76
char 1 95
char 1 84
char 1 69
char 1 65
char 1 77
char 1 95
char 1 83
char 1 79
char 1 85
char 1 78
char 1 68
char 1 0
align 1
LABELV $693
char 1 69
char 1 86
char 1 95
char 1 71
char 1 76
char 1 79
char 1 66
char 1 65
char 1 76
char 1 95
char 1 83
char 1 79
char 1 85
char 1 78
char 1 68
char 1 0
align 1
LABELV $692
char 1 69
char 1 86
char 1 95
char 1 71
char 1 69
char 1 78
char 1 69
char 1 82
char 1 65
char 1 76
char 1 95
char 1 83
char 1 79
char 1 85
char 1 78
char 1 68
char 1 0
align 1
LABELV $691
char 1 69
char 1 86
char 1 95
char 1 77
char 1 85
char 1 84
char 1 69
char 1 95
char 1 83
char 1 79
char 1 85
char 1 78
char 1 68
char 1 0
align 1
LABELV $690
char 1 69
char 1 86
char 1 95
char 1 80
char 1 76
char 1 65
char 1 89
char 1 95
char 1 69
char 1 70
char 1 70
char 1 69
char 1 67
char 1 84
char 1 95
char 1 73
char 1 68
char 1 0
align 1
LABELV $689
char 1 69
char 1 86
char 1 95
char 1 80
char 1 76
char 1 65
char 1 89
char 1 95
char 1 69
char 1 70
char 1 70
char 1 69
char 1 67
char 1 84
char 1 0
align 1
LABELV $688
char 1 69
char 1 86
char 1 95
char 1 77
char 1 73
char 1 83
char 1 83
char 1 73
char 1 76
char 1 69
char 1 95
char 1 83
char 1 84
char 1 73
char 1 67
char 1 75
char 1 0
align 1
LABELV $687
char 1 69
char 1 86
char 1 95
char 1 71
char 1 82
char 1 69
char 1 78
char 1 65
char 1 68
char 1 69
char 1 95
char 1 66
char 1 79
char 1 85
char 1 78
char 1 67
char 1 69
char 1 0
align 1
LABELV $686
char 1 69
char 1 86
char 1 95
char 1 80
char 1 76
char 1 65
char 1 89
char 1 69
char 1 82
char 1 95
char 1 84
char 1 69
char 1 76
char 1 69
char 1 80
char 1 79
char 1 82
char 1 84
char 1 95
char 1 79
char 1 85
char 1 84
char 1 0
align 1
LABELV $685
char 1 69
char 1 86
char 1 95
char 1 80
char 1 76
char 1 65
char 1 89
char 1 69
char 1 82
char 1 95
char 1 84
char 1 69
char 1 76
char 1 69
char 1 80
char 1 79
char 1 82
char 1 84
char 1 95
char 1 73
char 1 78
char 1 0
align 1
LABELV $684
char 1 69
char 1 86
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 95
char 1 80
char 1 79
char 1 80
char 1 0
align 1
LABELV $683
char 1 69
char 1 86
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 95
char 1 82
char 1 69
char 1 83
char 1 80
char 1 65
char 1 87
char 1 78
char 1 0
align 1
LABELV $682
char 1 69
char 1 86
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 85
char 1 83
char 1 69
char 1 70
char 1 65
char 1 73
char 1 76
char 1 0
align 1
LABELV $681
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 49
char 1 53
char 1 0
align 1
LABELV $680
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 49
char 1 52
char 1 0
align 1
LABELV $679
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 49
char 1 51
char 1 0
align 1
LABELV $678
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 49
char 1 50
char 1 0
align 1
LABELV $677
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 49
char 1 49
char 1 0
align 1
LABELV $676
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 49
char 1 48
char 1 0
align 1
LABELV $675
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 57
char 1 0
align 1
LABELV $674
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 56
char 1 0
align 1
LABELV $673
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 55
char 1 0
align 1
LABELV $672
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 54
char 1 0
align 1
LABELV $671
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 53
char 1 0
align 1
LABELV $670
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 52
char 1 0
align 1
LABELV $669
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 51
char 1 0
align 1
LABELV $668
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 50
char 1 0
align 1
LABELV $667
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 49
char 1 0
align 1
LABELV $666
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 48
char 1 0
align 1
LABELV $665
char 1 69
char 1 86
char 1 95
char 1 85
char 1 83
char 1 69
char 1 0
align 1
LABELV $664
char 1 69
char 1 86
char 1 95
char 1 83
char 1 67
char 1 82
char 1 69
char 1 69
char 1 78
char 1 83
char 1 72
char 1 65
char 1 75
char 1 69
char 1 0
align 1
LABELV $663
char 1 69
char 1 86
char 1 95
char 1 84
char 1 69
char 1 65
char 1 77
char 1 95
char 1 80
char 1 79
char 1 87
char 1 69
char 1 82
char 1 0
align 1
LABELV $662
char 1 69
char 1 86
char 1 95
char 1 80
char 1 82
char 1 69
char 1 68
char 1 69
char 1 70
char 1 83
char 1 79
char 1 85
char 1 78
char 1 68
char 1 0
align 1
LABELV $661
char 1 69
char 1 86
char 1 95
char 1 68
char 1 73
char 1 83
char 1 82
char 1 85
char 1 80
char 1 84
char 1 79
char 1 82
char 1 95
char 1 90
char 1 79
char 1 79
char 1 77
char 1 83
char 1 79
char 1 85
char 1 78
char 1 68
char 1 0
align 1
LABELV $660
char 1 69
char 1 86
char 1 95
char 1 68
char 1 73
char 1 83
char 1 82
char 1 85
char 1 80
char 1 84
char 1 79
char 1 82
char 1 95
char 1 72
char 1 73
char 1 84
char 1 0
align 1
LABELV $659
char 1 69
char 1 86
char 1 95
char 1 68
char 1 73
char 1 83
char 1 82
char 1 85
char 1 80
char 1 84
char 1 79
char 1 82
char 1 95
char 1 83
char 1 78
char 1 73
char 1 80
char 1 69
char 1 82
char 1 95
char 1 77
char 1 73
char 1 83
char 1 83
char 1 0
align 1
LABELV $658
char 1 69
char 1 86
char 1 95
char 1 68
char 1 73
char 1 83
char 1 82
char 1 85
char 1 80
char 1 84
char 1 79
char 1 82
char 1 95
char 1 83
char 1 78
char 1 73
char 1 80
char 1 69
char 1 82
char 1 95
char 1 83
char 1 72
char 1 79
char 1 84
char 1 0
align 1
LABELV $657
char 1 69
char 1 86
char 1 95
char 1 68
char 1 73
char 1 83
char 1 82
char 1 85
char 1 80
char 1 84
char 1 79
char 1 82
char 1 95
char 1 77
char 1 65
char 1 73
char 1 78
char 1 95
char 1 83
char 1 72
char 1 79
char 1 84
char 1 0
align 1
LABELV $656
char 1 69
char 1 86
char 1 95
char 1 66
char 1 69
char 1 67
char 1 79
char 1 77
char 1 69
char 1 95
char 1 74
char 1 69
char 1 68
char 1 73
char 1 77
char 1 65
char 1 83
char 1 84
char 1 69
char 1 82
char 1 0
align 1
LABELV $655
char 1 69
char 1 86
char 1 95
char 1 83
char 1 65
char 1 66
char 1 69
char 1 82
char 1 95
char 1 85
char 1 78
char 1 72
char 1 79
char 1 76
char 1 83
char 1 84
char 1 69
char 1 82
char 1 0
align 1
LABELV $654
char 1 69
char 1 86
char 1 95
char 1 83
char 1 65
char 1 66
char 1 69
char 1 82
char 1 95
char 1 66
char 1 76
char 1 79
char 1 67
char 1 75
char 1 0
align 1
LABELV $653
char 1 69
char 1 86
char 1 95
char 1 83
char 1 65
char 1 66
char 1 69
char 1 82
char 1 95
char 1 72
char 1 73
char 1 84
char 1 0
align 1
LABELV $652
char 1 69
char 1 86
char 1 95
char 1 83
char 1 65
char 1 66
char 1 69
char 1 82
char 1 95
char 1 65
char 1 84
char 1 84
char 1 65
char 1 67
char 1 75
char 1 0
align 1
LABELV $651
char 1 69
char 1 86
char 1 95
char 1 65
char 1 76
char 1 84
char 1 95
char 1 70
char 1 73
char 1 82
char 1 69
char 1 0
align 1
LABELV $650
char 1 69
char 1 86
char 1 95
char 1 70
char 1 73
char 1 82
char 1 69
char 1 95
char 1 87
char 1 69
char 1 65
char 1 80
char 1 79
char 1 78
char 1 0
align 1
LABELV $649
char 1 69
char 1 86
char 1 95
char 1 67
char 1 72
char 1 65
char 1 78
char 1 71
char 1 69
char 1 95
char 1 87
char 1 69
char 1 65
char 1 80
char 1 79
char 1 78
char 1 0
align 1
LABELV $648
char 1 69
char 1 86
char 1 95
char 1 78
char 1 79
char 1 65
char 1 77
char 1 77
char 1 79
char 1 0
align 1
LABELV $647
char 1 69
char 1 86
char 1 95
char 1 71
char 1 76
char 1 79
char 1 66
char 1 65
char 1 76
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 95
char 1 80
char 1 73
char 1 67
char 1 75
char 1 85
char 1 80
char 1 0
align 1
LABELV $646
char 1 69
char 1 86
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
char 1 95
char 1 80
char 1 73
char 1 67
char 1 75
char 1 85
char 1 80
char 1 0
align 1
LABELV $645
char 1 69
char 1 86
char 1 95
char 1 87
char 1 65
char 1 84
char 1 69
char 1 82
char 1 95
char 1 67
char 1 76
char 1 69
char 1 65
char 1 82
char 1 0
align 1
LABELV $644
char 1 69
char 1 86
char 1 95
char 1 87
char 1 65
char 1 84
char 1 69
char 1 82
char 1 95
char 1 85
char 1 78
char 1 68
char 1 69
char 1 82
char 1 0
align 1
LABELV $643
char 1 69
char 1 86
char 1 95
char 1 87
char 1 65
char 1 84
char 1 69
char 1 82
char 1 95
char 1 76
char 1 69
char 1 65
char 1 86
char 1 69
char 1 0
align 1
LABELV $642
char 1 69
char 1 86
char 1 95
char 1 87
char 1 65
char 1 84
char 1 69
char 1 82
char 1 95
char 1 84
char 1 79
char 1 85
char 1 67
char 1 72
char 1 0
align 1
LABELV $641
char 1 69
char 1 86
char 1 95
char 1 82
char 1 79
char 1 76
char 1 76
char 1 0
align 1
LABELV $640
char 1 69
char 1 86
char 1 95
char 1 74
char 1 85
char 1 77
char 1 80
char 1 0
align 1
LABELV $639
char 1 69
char 1 86
char 1 95
char 1 80
char 1 82
char 1 73
char 1 86
char 1 65
char 1 84
char 1 69
char 1 95
char 1 68
char 1 85
char 1 69
char 1 76
char 1 0
align 1
LABELV $638
char 1 69
char 1 86
char 1 95
char 1 74
char 1 85
char 1 77
char 1 80
char 1 95
char 1 80
char 1 65
char 1 68
char 1 0
align 1
LABELV $637
char 1 69
char 1 86
char 1 95
char 1 70
char 1 65
char 1 76
char 1 76
char 1 0
align 1
LABELV $636
char 1 69
char 1 86
char 1 95
char 1 83
char 1 84
char 1 69
char 1 80
char 1 95
char 1 49
char 1 54
char 1 0
align 1
LABELV $635
char 1 69
char 1 86
char 1 95
char 1 83
char 1 84
char 1 69
char 1 80
char 1 95
char 1 49
char 1 50
char 1 0
align 1
LABELV $634
char 1 69
char 1 86
char 1 95
char 1 83
char 1 84
char 1 69
char 1 80
char 1 95
char 1 56
char 1 0
align 1
LABELV $633
char 1 69
char 1 86
char 1 95
char 1 83
char 1 84
char 1 69
char 1 80
char 1 95
char 1 52
char 1 0
align 1
LABELV $632
char 1 69
char 1 86
char 1 95
char 1 83
char 1 87
char 1 73
char 1 77
char 1 0
align 1
LABELV $631
char 1 69
char 1 86
char 1 95
char 1 70
char 1 79
char 1 79
char 1 84
char 1 87
char 1 65
char 1 68
char 1 69
char 1 0
align 1
LABELV $630
char 1 69
char 1 86
char 1 95
char 1 70
char 1 79
char 1 79
char 1 84
char 1 83
char 1 80
char 1 76
char 1 65
char 1 83
char 1 72
char 1 0
align 1
LABELV $629
char 1 69
char 1 86
char 1 95
char 1 70
char 1 79
char 1 79
char 1 84
char 1 83
char 1 84
char 1 69
char 1 80
char 1 95
char 1 77
char 1 69
char 1 84
char 1 65
char 1 76
char 1 0
align 1
LABELV $628
char 1 69
char 1 86
char 1 95
char 1 70
char 1 79
char 1 79
char 1 84
char 1 83
char 1 84
char 1 69
char 1 80
char 1 0
align 1
LABELV $627
char 1 69
char 1 86
char 1 95
char 1 67
char 1 76
char 1 73
char 1 69
char 1 78
char 1 84
char 1 74
char 1 79
char 1 73
char 1 78
char 1 0
align 1
LABELV $626
char 1 69
char 1 86
char 1 95
char 1 78
char 1 79
char 1 78
char 1 69
char 1 0
align 1
LABELV $624
char 1 66
char 1 71
char 1 95
char 1 69
char 1 118
char 1 97
char 1 108
char 1 117
char 1 97
char 1 116
char 1 101
char 1 84
char 1 114
char 1 97
char 1 106
char 1 101
char 1 99
char 1 116
char 1 111
char 1 114
char 1 121
char 1 68
char 1 101
char 1 108
char 1 116
char 1 97
char 1 58
char 1 32
char 1 117
char 1 110
char 1 107
char 1 110
char 1 111
char 1 119
char 1 110
char 1 32
char 1 116
char 1 114
char 1 84
char 1 121
char 1 112
char 1 101
char 1 58
char 1 32
char 1 37
char 1 105
char 1 0
align 1
LABELV $611
char 1 66
char 1 71
char 1 95
char 1 69
char 1 118
char 1 97
char 1 108
char 1 117
char 1 97
char 1 116
char 1 101
char 1 84
char 1 114
char 1 97
char 1 106
char 1 101
char 1 99
char 1 116
char 1 111
char 1 114
char 1 121
char 1 58
char 1 32
char 1 91
char 1 67
char 1 76
char 1 73
char 1 69
char 1 78
char 1 84
char 1 71
char 1 65
char 1 77
char 1 69
char 1 32
char 1 83
char 1 73
char 1 68
char 1 69
char 1 93
char 1 32
char 1 117
char 1 110
char 1 107
char 1 110
char 1 111
char 1 119
char 1 110
char 1 32
char 1 116
char 1 114
char 1 84
char 1 121
char 1 112
char 1 101
char 1 58
char 1 32
char 1 37
char 1 105
char 1 0
align 1
LABELV $596
char 1 66
char 1 71
char 1 95
char 1 67
char 1 97
char 1 110
char 1 73
char 1 116
char 1 101
char 1 109
char 1 66
char 1 101
char 1 71
char 1 114
char 1 97
char 1 98
char 1 98
char 1 101
char 1 100
char 1 58
char 1 32
char 1 73
char 1 84
char 1 95
char 1 66
char 1 65
char 1 68
char 1 0
align 1
LABELV $539
char 1 66
char 1 71
char 1 95
char 1 67
char 1 97
char 1 110
char 1 73
char 1 116
char 1 101
char 1 109
char 1 66
char 1 101
char 1 71
char 1 114
char 1 97
char 1 98
char 1 98
char 1 101
char 1 100
char 1 58
char 1 32
char 1 105
char 1 110
char 1 100
char 1 101
char 1 120
char 1 32
char 1 111
char 1 117
char 1 116
char 1 32
char 1 111
char 1 102
char 1 32
char 1 114
char 1 97
char 1 110
char 1 103
char 1 101
char 1 0
align 1
LABELV $456
char 1 67
char 1 111
char 1 117
char 1 108
char 1 100
char 1 110
char 1 39
char 1 116
char 1 32
char 1 102
char 1 105
char 1 110
char 1 100
char 1 32
char 1 105
char 1 116
char 1 101
char 1 109
char 1 32
char 1 102
char 1 111
char 1 114
char 1 32
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 32
char 1 37
char 1 105
char 1 0
align 1
LABELV $447
char 1 72
char 1 111
char 1 108
char 1 100
char 1 97
char 1 98
char 1 108
char 1 101
char 1 73
char 1 116
char 1 101
char 1 109
char 1 32
char 1 110
char 1 111
char 1 116
char 1 32
char 1 102
char 1 111
char 1 117
char 1 110
char 1 100
char 1 0
align 1
LABELV $394
char 1 105
char 1 99
char 1 111
char 1 110
char 1 115
char 1 47
char 1 105
char 1 99
char 1 111
char 1 110
char 1 104
char 1 95
char 1 98
char 1 111
char 1 114
char 1 98
char 1 0
align 1
LABELV $393
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 112
char 1 111
char 1 119
char 1 101
char 1 114
char 1 117
char 1 112
char 1 115
char 1 47
char 1 111
char 1 114
char 1 98
char 1 47
char 1 98
char 1 95
char 1 111
char 1 114
char 1 98
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $392
char 1 105
char 1 116
char 1 101
char 1 109
char 1 95
char 1 98
char 1 108
char 1 117
char 1 101
char 1 99
char 1 117
char 1 98
char 1 101
char 1 0
align 1
LABELV $391
char 1 105
char 1 99
char 1 111
char 1 110
char 1 115
char 1 47
char 1 105
char 1 99
char 1 111
char 1 110
char 1 104
char 1 95
char 1 114
char 1 111
char 1 114
char 1 98
char 1 0
align 1
LABELV $390
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 112
char 1 111
char 1 119
char 1 101
char 1 114
char 1 117
char 1 112
char 1 115
char 1 47
char 1 111
char 1 114
char 1 98
char 1 47
char 1 114
char 1 95
char 1 111
char 1 114
char 1 98
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $389
char 1 105
char 1 116
char 1 101
char 1 109
char 1 95
char 1 114
char 1 101
char 1 100
char 1 99
char 1 117
char 1 98
char 1 101
char 1 0
align 1
LABELV $388
char 1 105
char 1 99
char 1 111
char 1 110
char 1 115
char 1 47
char 1 105
char 1 99
char 1 111
char 1 110
char 1 102
char 1 95
char 1 110
char 1 101
char 1 117
char 1 116
char 1 114
char 1 97
char 1 108
char 1 49
char 1 0
align 1
LABELV $387
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 102
char 1 108
char 1 97
char 1 103
char 1 115
char 1 47
char 1 110
char 1 95
char 1 102
char 1 108
char 1 97
char 1 103
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $386
char 1 116
char 1 101
char 1 97
char 1 109
char 1 95
char 1 67
char 1 84
char 1 70
char 1 95
char 1 110
char 1 101
char 1 117
char 1 116
char 1 114
char 1 97
char 1 108
char 1 102
char 1 108
char 1 97
char 1 103
char 1 0
align 1
LABELV $385
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 109
char 1 112
char 1 105
char 1 95
char 1 98
char 1 102
char 1 108
char 1 97
char 1 103
char 1 0
align 1
LABELV $384
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 102
char 1 108
char 1 97
char 1 103
char 1 115
char 1 47
char 1 98
char 1 95
char 1 102
char 1 108
char 1 97
char 1 103
char 1 95
char 1 121
char 1 115
char 1 97
char 1 108
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $383
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 102
char 1 108
char 1 97
char 1 103
char 1 115
char 1 47
char 1 98
char 1 95
char 1 102
char 1 108
char 1 97
char 1 103
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $382
char 1 116
char 1 101
char 1 97
char 1 109
char 1 95
char 1 67
char 1 84
char 1 70
char 1 95
char 1 98
char 1 108
char 1 117
char 1 101
char 1 102
char 1 108
char 1 97
char 1 103
char 1 0
align 1
LABELV $381
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 109
char 1 112
char 1 105
char 1 95
char 1 114
char 1 102
char 1 108
char 1 97
char 1 103
char 1 0
align 1
LABELV $380
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 102
char 1 108
char 1 97
char 1 103
char 1 115
char 1 47
char 1 114
char 1 95
char 1 102
char 1 108
char 1 97
char 1 103
char 1 95
char 1 121
char 1 115
char 1 97
char 1 108
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $379
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 102
char 1 108
char 1 97
char 1 103
char 1 115
char 1 47
char 1 114
char 1 95
char 1 102
char 1 108
char 1 97
char 1 103
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $378
char 1 116
char 1 101
char 1 97
char 1 109
char 1 95
char 1 67
char 1 84
char 1 70
char 1 95
char 1 114
char 1 101
char 1 100
char 1 102
char 1 108
char 1 97
char 1 103
char 1 0
align 1
LABELV $377
char 1 103
char 1 102
char 1 120
char 1 47
char 1 109
char 1 112
char 1 47
char 1 97
char 1 109
char 1 109
char 1 111
char 1 95
char 1 114
char 1 111
char 1 99
char 1 107
char 1 101
char 1 116
char 1 115
char 1 0
align 1
LABELV $376
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 105
char 1 116
char 1 101
char 1 109
char 1 115
char 1 47
char 1 114
char 1 111
char 1 99
char 1 107
char 1 101
char 1 116
char 1 115
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $375
char 1 97
char 1 109
char 1 109
char 1 111
char 1 95
char 1 114
char 1 111
char 1 99
char 1 107
char 1 101
char 1 116
char 1 115
char 1 0
align 1
LABELV $374
char 1 103
char 1 102
char 1 120
char 1 47
char 1 109
char 1 112
char 1 47
char 1 97
char 1 109
char 1 109
char 1 111
char 1 95
char 1 109
char 1 101
char 1 116
char 1 97
char 1 108
char 1 108
char 1 105
char 1 99
char 1 95
char 1 98
char 1 111
char 1 108
char 1 116
char 1 115
char 1 0
align 1
LABELV $373
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 105
char 1 116
char 1 101
char 1 109
char 1 115
char 1 47
char 1 109
char 1 101
char 1 116
char 1 97
char 1 108
char 1 108
char 1 105
char 1 99
char 1 95
char 1 98
char 1 111
char 1 108
char 1 116
char 1 115
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $372
char 1 97
char 1 109
char 1 109
char 1 111
char 1 95
char 1 109
char 1 101
char 1 116
char 1 97
char 1 108
char 1 108
char 1 105
char 1 99
char 1 95
char 1 98
char 1 111
char 1 108
char 1 116
char 1 115
char 1 0
align 1
LABELV $371
char 1 103
char 1 102
char 1 120
char 1 47
char 1 109
char 1 112
char 1 47
char 1 97
char 1 109
char 1 109
char 1 111
char 1 95
char 1 112
char 1 111
char 1 119
char 1 101
char 1 114
char 1 95
char 1 99
char 1 101
char 1 108
char 1 108
char 1 0
align 1
LABELV $370
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 105
char 1 116
char 1 101
char 1 109
char 1 115
char 1 47
char 1 112
char 1 111
char 1 119
char 1 101
char 1 114
char 1 95
char 1 99
char 1 101
char 1 108
char 1 108
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $369
char 1 97
char 1 109
char 1 109
char 1 111
char 1 95
char 1 112
char 1 111
char 1 119
char 1 101
char 1 114
char 1 99
char 1 101
char 1 108
char 1 108
char 1 0
align 1
LABELV $368
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 105
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 98
char 1 97
char 1 116
char 1 116
char 1 101
char 1 114
char 1 121
char 1 0
align 1
LABELV $367
char 1 97
char 1 109
char 1 109
char 1 111
char 1 95
char 1 98
char 1 108
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 0
align 1
LABELV $366
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 105
char 1 116
char 1 101
char 1 109
char 1 115
char 1 47
char 1 101
char 1 110
char 1 101
char 1 114
char 1 103
char 1 121
char 1 95
char 1 99
char 1 101
char 1 108
char 1 108
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $365
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 112
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 47
char 1 112
char 1 105
char 1 99
char 1 107
char 1 117
char 1 112
char 1 101
char 1 110
char 1 101
char 1 114
char 1 103
char 1 121
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $364
char 1 97
char 1 109
char 1 109
char 1 111
char 1 95
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 0
align 1
LABELV $363
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 95
char 1 116
char 1 117
char 1 114
char 1 114
char 1 101
char 1 116
char 1 119
char 1 112
char 1 0
align 1
LABELV $362
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 95
char 1 101
char 1 109
char 1 112
char 1 108
char 1 97
char 1 99
char 1 101
char 1 100
char 1 0
align 1
LABELV $361
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 95
char 1 100
char 1 101
char 1 116
char 1 95
char 1 112
char 1 97
char 1 99
char 1 107
char 1 0
align 1
LABELV $360
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 95
char 1 116
char 1 114
char 1 105
char 1 112
char 1 95
char 1 109
char 1 105
char 1 110
char 1 101
char 1 0
align 1
LABELV $359
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 95
char 1 116
char 1 104
char 1 101
char 1 114
char 1 109
char 1 97
char 1 108
char 1 0
align 1
LABELV $358
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 119
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 100
char 1 101
char 1 116
char 1 112
char 1 97
char 1 99
char 1 107
char 1 0
align 1
LABELV $357
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 100
char 1 101
char 1 116
char 1 112
char 1 97
char 1 99
char 1 107
char 1 47
char 1 100
char 1 101
char 1 116
char 1 95
char 1 112
char 1 97
char 1 99
char 1 107
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $356
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 100
char 1 101
char 1 116
char 1 112
char 1 97
char 1 99
char 1 107
char 1 47
char 1 100
char 1 101
char 1 116
char 1 95
char 1 112
char 1 97
char 1 99
char 1 107
char 1 95
char 1 119
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
align 1
LABELV $355
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 100
char 1 101
char 1 116
char 1 112
char 1 97
char 1 99
char 1 107
char 1 47
char 1 100
char 1 101
char 1 116
char 1 95
char 1 112
char 1 97
char 1 99
char 1 107
char 1 95
char 1 112
char 1 114
char 1 111
char 1 106
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
align 1
LABELV $354
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 100
char 1 101
char 1 116
char 1 112
char 1 97
char 1 99
char 1 107
char 1 47
char 1 100
char 1 101
char 1 116
char 1 95
char 1 112
char 1 97
char 1 99
char 1 107
char 1 95
char 1 112
char 1 117
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $353
char 1 97
char 1 109
char 1 109
char 1 111
char 1 95
char 1 100
char 1 101
char 1 116
char 1 112
char 1 97
char 1 99
char 1 107
char 1 0
align 1
LABELV $352
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 119
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 116
char 1 114
char 1 105
char 1 112
char 1 109
char 1 105
char 1 110
char 1 101
char 1 0
align 1
LABELV $351
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 108
char 1 97
char 1 115
char 1 101
char 1 114
char 1 95
char 1 116
char 1 114
char 1 97
char 1 112
char 1 47
char 1 108
char 1 97
char 1 115
char 1 101
char 1 114
char 1 95
char 1 116
char 1 114
char 1 97
char 1 112
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $350
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 108
char 1 97
char 1 115
char 1 101
char 1 114
char 1 95
char 1 116
char 1 114
char 1 97
char 1 112
char 1 47
char 1 108
char 1 97
char 1 115
char 1 101
char 1 114
char 1 95
char 1 116
char 1 114
char 1 97
char 1 112
char 1 95
char 1 119
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
align 1
LABELV $349
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 108
char 1 97
char 1 115
char 1 101
char 1 114
char 1 95
char 1 116
char 1 114
char 1 97
char 1 112
char 1 47
char 1 108
char 1 97
char 1 115
char 1 101
char 1 114
char 1 95
char 1 116
char 1 114
char 1 97
char 1 112
char 1 95
char 1 112
char 1 117
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $348
char 1 97
char 1 109
char 1 109
char 1 111
char 1 95
char 1 116
char 1 114
char 1 105
char 1 112
char 1 109
char 1 105
char 1 110
char 1 101
char 1 0
align 1
LABELV $347
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 119
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 116
char 1 104
char 1 101
char 1 114
char 1 109
char 1 97
char 1 108
char 1 0
align 1
LABELV $346
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 116
char 1 104
char 1 101
char 1 114
char 1 109
char 1 97
char 1 108
char 1 47
char 1 116
char 1 104
char 1 101
char 1 114
char 1 109
char 1 97
char 1 108
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $345
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 116
char 1 104
char 1 101
char 1 114
char 1 109
char 1 97
char 1 108
char 1 47
char 1 116
char 1 104
char 1 101
char 1 114
char 1 109
char 1 97
char 1 108
char 1 95
char 1 119
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
align 1
LABELV $344
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 116
char 1 104
char 1 101
char 1 114
char 1 109
char 1 97
char 1 108
char 1 47
char 1 116
char 1 104
char 1 101
char 1 114
char 1 109
char 1 97
char 1 108
char 1 95
char 1 112
char 1 117
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $343
char 1 97
char 1 109
char 1 109
char 1 111
char 1 95
char 1 116
char 1 104
char 1 101
char 1 114
char 1 109
char 1 97
char 1 108
char 1 0
align 1
LABELV $342
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 119
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 109
char 1 101
char 1 114
char 1 114
char 1 115
char 1 111
char 1 110
char 1 110
char 1 0
align 1
LABELV $341
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 109
char 1 101
char 1 114
char 1 114
char 1 95
char 1 115
char 1 111
char 1 110
char 1 110
char 1 47
char 1 109
char 1 101
char 1 114
char 1 114
char 1 95
char 1 115
char 1 111
char 1 110
char 1 110
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $340
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 109
char 1 101
char 1 114
char 1 114
char 1 95
char 1 115
char 1 111
char 1 110
char 1 110
char 1 47
char 1 109
char 1 101
char 1 114
char 1 114
char 1 95
char 1 115
char 1 111
char 1 110
char 1 110
char 1 95
char 1 119
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
align 1
LABELV $339
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 95
char 1 114
char 1 111
char 1 99
char 1 107
char 1 101
char 1 116
char 1 95
char 1 108
char 1 97
char 1 117
char 1 110
char 1 99
char 1 104
char 1 101
char 1 114
char 1 0
align 1
LABELV $338
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 119
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 102
char 1 108
char 1 101
char 1 99
char 1 104
char 1 101
char 1 116
char 1 116
char 1 101
char 1 0
align 1
LABELV $337
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 103
char 1 111
char 1 108
char 1 97
char 1 110
char 1 95
char 1 97
char 1 114
char 1 109
char 1 115
char 1 47
char 1 103
char 1 111
char 1 108
char 1 97
char 1 110
char 1 95
char 1 97
char 1 114
char 1 109
char 1 115
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $336
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 103
char 1 111
char 1 108
char 1 97
char 1 110
char 1 95
char 1 97
char 1 114
char 1 109
char 1 115
char 1 47
char 1 103
char 1 111
char 1 108
char 1 97
char 1 110
char 1 95
char 1 97
char 1 114
char 1 109
char 1 115
char 1 95
char 1 119
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
align 1
LABELV $335
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 95
char 1 102
char 1 108
char 1 101
char 1 99
char 1 104
char 1 101
char 1 116
char 1 116
char 1 101
char 1 0
align 1
LABELV $334
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 119
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 100
char 1 101
char 1 109
char 1 112
char 1 50
char 1 0
align 1
LABELV $333
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 100
char 1 101
char 1 109
char 1 112
char 1 50
char 1 47
char 1 100
char 1 101
char 1 109
char 1 112
char 1 50
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $332
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 100
char 1 101
char 1 109
char 1 112
char 1 50
char 1 47
char 1 100
char 1 101
char 1 109
char 1 112
char 1 50
char 1 95
char 1 119
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
align 1
LABELV $331
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 95
char 1 100
char 1 101
char 1 109
char 1 112
char 1 50
char 1 0
align 1
LABELV $330
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 119
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 114
char 1 101
char 1 112
char 1 101
char 1 97
char 1 116
char 1 101
char 1 114
char 1 0
align 1
LABELV $329
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 104
char 1 101
char 1 97
char 1 118
char 1 121
char 1 95
char 1 114
char 1 101
char 1 112
char 1 101
char 1 97
char 1 116
char 1 101
char 1 114
char 1 47
char 1 104
char 1 101
char 1 97
char 1 118
char 1 121
char 1 95
char 1 114
char 1 101
char 1 112
char 1 101
char 1 97
char 1 116
char 1 101
char 1 114
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $328
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 104
char 1 101
char 1 97
char 1 118
char 1 121
char 1 95
char 1 114
char 1 101
char 1 112
char 1 101
char 1 97
char 1 116
char 1 101
char 1 114
char 1 47
char 1 104
char 1 101
char 1 97
char 1 118
char 1 121
char 1 95
char 1 114
char 1 101
char 1 112
char 1 101
char 1 97
char 1 116
char 1 101
char 1 114
char 1 95
char 1 119
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
align 1
LABELV $327
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 95
char 1 114
char 1 101
char 1 112
char 1 101
char 1 97
char 1 116
char 1 101
char 1 114
char 1 0
align 1
LABELV $326
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 119
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 98
char 1 111
char 1 119
char 1 99
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 0
align 1
LABELV $325
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 98
char 1 111
char 1 119
char 1 99
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 47
char 1 98
char 1 111
char 1 119
char 1 99
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $324
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 98
char 1 111
char 1 119
char 1 99
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 47
char 1 98
char 1 111
char 1 119
char 1 99
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 95
char 1 119
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
align 1
LABELV $323
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 95
char 1 98
char 1 111
char 1 119
char 1 99
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 0
align 1
LABELV $322
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 119
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 100
char 1 105
char 1 115
char 1 114
char 1 117
char 1 112
char 1 116
char 1 111
char 1 114
char 1 0
align 1
LABELV $321
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 100
char 1 105
char 1 115
char 1 114
char 1 117
char 1 112
char 1 116
char 1 111
char 1 114
char 1 47
char 1 100
char 1 105
char 1 115
char 1 114
char 1 117
char 1 112
char 1 116
char 1 111
char 1 114
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $320
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 100
char 1 105
char 1 115
char 1 114
char 1 117
char 1 112
char 1 116
char 1 111
char 1 114
char 1 47
char 1 100
char 1 105
char 1 115
char 1 114
char 1 117
char 1 112
char 1 116
char 1 111
char 1 114
char 1 95
char 1 119
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
align 1
LABELV $319
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 95
char 1 100
char 1 105
char 1 115
char 1 114
char 1 117
char 1 112
char 1 116
char 1 111
char 1 114
char 1 0
align 1
LABELV $318
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 119
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 98
char 1 108
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 0
align 1
LABELV $317
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 98
char 1 108
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 95
char 1 114
char 1 47
char 1 98
char 1 108
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $316
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 98
char 1 108
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 95
char 1 114
char 1 47
char 1 98
char 1 108
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 95
char 1 119
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
align 1
LABELV $315
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 95
char 1 98
char 1 108
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 0
align 1
LABELV $314
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 119
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 114
char 1 105
char 1 102
char 1 108
char 1 101
char 1 0
align 1
LABELV $313
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 98
char 1 114
char 1 105
char 1 97
char 1 114
char 1 95
char 1 112
char 1 105
char 1 115
char 1 116
char 1 111
char 1 108
char 1 47
char 1 98
char 1 114
char 1 105
char 1 97
char 1 114
char 1 95
char 1 112
char 1 105
char 1 115
char 1 116
char 1 111
char 1 108
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $312
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 98
char 1 114
char 1 105
char 1 97
char 1 114
char 1 95
char 1 112
char 1 105
char 1 115
char 1 116
char 1 111
char 1 108
char 1 47
char 1 98
char 1 114
char 1 105
char 1 97
char 1 114
char 1 95
char 1 112
char 1 105
char 1 115
char 1 116
char 1 111
char 1 108
char 1 95
char 1 119
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
align 1
LABELV $311
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 95
char 1 98
char 1 114
char 1 121
char 1 97
char 1 114
char 1 95
char 1 112
char 1 105
char 1 115
char 1 116
char 1 111
char 1 108
char 1 0
align 1
LABELV $310
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 119
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 108
char 1 105
char 1 103
char 1 104
char 1 116
char 1 115
char 1 97
char 1 98
char 1 101
char 1 114
char 1 0
align 1
LABELV $309
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 115
char 1 97
char 1 98
char 1 101
char 1 114
char 1 47
char 1 115
char 1 97
char 1 98
char 1 101
char 1 114
char 1 95
char 1 119
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $308
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 115
char 1 97
char 1 98
char 1 101
char 1 114
char 1 47
char 1 115
char 1 97
char 1 98
char 1 101
char 1 114
char 1 95
char 1 119
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
align 1
LABELV $307
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 95
char 1 115
char 1 97
char 1 98
char 1 101
char 1 114
char 1 0
align 1
LABELV $306
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 119
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 115
char 1 116
char 1 117
char 1 110
char 1 98
char 1 97
char 1 116
char 1 111
char 1 110
char 1 0
align 1
LABELV $305
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 115
char 1 116
char 1 117
char 1 110
char 1 95
char 1 98
char 1 97
char 1 116
char 1 111
char 1 110
char 1 47
char 1 98
char 1 97
char 1 116
char 1 111
char 1 110
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $304
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 50
char 1 47
char 1 115
char 1 116
char 1 117
char 1 110
char 1 95
char 1 98
char 1 97
char 1 116
char 1 111
char 1 110
char 1 47
char 1 98
char 1 97
char 1 116
char 1 111
char 1 110
char 1 95
char 1 119
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
align 1
LABELV $303
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 95
char 1 115
char 1 116
char 1 117
char 1 110
char 1 95
char 1 98
char 1 97
char 1 116
char 1 111
char 1 110
char 1 0
align 1
LABELV $302
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 109
char 1 112
char 1 105
char 1 95
char 1 121
char 1 115
char 1 97
char 1 109
char 1 97
char 1 114
char 1 105
char 1 0
align 1
LABELV $301
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 109
char 1 97
char 1 112
char 1 95
char 1 111
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 115
char 1 47
char 1 109
char 1 112
char 1 47
char 1 121
char 1 115
char 1 97
char 1 108
char 1 105
char 1 109
char 1 97
char 1 114
char 1 105
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $300
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 112
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 47
char 1 121
char 1 115
char 1 97
char 1 108
char 1 105
char 1 109
char 1 97
char 1 114
char 1 105
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $299
char 1 105
char 1 116
char 1 101
char 1 109
char 1 95
char 1 121
char 1 115
char 1 97
char 1 108
char 1 105
char 1 109
char 1 97
char 1 114
char 1 105
char 1 0
align 1
LABELV $298
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 109
char 1 112
char 1 105
char 1 95
char 1 102
char 1 98
char 1 111
char 1 111
char 1 110
char 1 0
align 1
LABELV $297
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 109
char 1 97
char 1 112
char 1 95
char 1 111
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 115
char 1 47
char 1 109
char 1 112
char 1 47
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 95
char 1 98
char 1 111
char 1 111
char 1 110
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $296
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 112
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 47
char 1 98
char 1 111
char 1 111
char 1 110
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $295
char 1 105
char 1 116
char 1 101
char 1 109
char 1 95
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 95
char 1 98
char 1 111
char 1 111
char 1 110
char 1 0
align 1
LABELV $294
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 109
char 1 112
char 1 105
char 1 95
char 1 100
char 1 107
char 1 108
char 1 105
char 1 103
char 1 104
char 1 116
char 1 0
align 1
LABELV $293
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 109
char 1 97
char 1 112
char 1 95
char 1 111
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 115
char 1 47
char 1 109
char 1 112
char 1 47
char 1 100
char 1 107
char 1 95
char 1 101
char 1 110
char 1 108
char 1 105
char 1 103
char 1 104
char 1 116
char 1 101
char 1 110
char 1 109
char 1 101
char 1 110
char 1 116
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $292
char 1 105
char 1 116
char 1 101
char 1 109
char 1 95
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 95
char 1 101
char 1 110
char 1 108
char 1 105
char 1 103
char 1 104
char 1 116
char 1 101
char 1 110
char 1 95
char 1 100
char 1 97
char 1 114
char 1 107
char 1 0
align 1
LABELV $291
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 109
char 1 112
char 1 105
char 1 95
char 1 106
char 1 108
char 1 105
char 1 103
char 1 104
char 1 116
char 1 0
align 1
LABELV $290
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 109
char 1 97
char 1 112
char 1 95
char 1 111
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 115
char 1 47
char 1 109
char 1 112
char 1 47
char 1 106
char 1 101
char 1 100
char 1 105
char 1 95
char 1 101
char 1 110
char 1 108
char 1 105
char 1 103
char 1 104
char 1 116
char 1 101
char 1 110
char 1 109
char 1 101
char 1 110
char 1 116
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $289
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 112
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 47
char 1 101
char 1 110
char 1 108
char 1 105
char 1 103
char 1 104
char 1 116
char 1 101
char 1 110
char 1 109
char 1 101
char 1 110
char 1 116
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $288
char 1 105
char 1 116
char 1 101
char 1 109
char 1 95
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 95
char 1 101
char 1 110
char 1 108
char 1 105
char 1 103
char 1 104
char 1 116
char 1 101
char 1 110
char 1 95
char 1 108
char 1 105
char 1 103
char 1 104
char 1 116
char 1 0
align 1
LABELV $287
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 105
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 115
char 1 101
char 1 110
char 1 116
char 1 114
char 1 121
char 1 103
char 1 117
char 1 110
char 1 0
align 1
LABELV $286
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 105
char 1 116
char 1 101
char 1 109
char 1 115
char 1 47
char 1 112
char 1 115
char 1 103
char 1 117
char 1 110
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
align 1
LABELV $285
char 1 105
char 1 116
char 1 101
char 1 109
char 1 95
char 1 115
char 1 101
char 1 110
char 1 116
char 1 114
char 1 121
char 1 95
char 1 103
char 1 117
char 1 110
char 1 0
align 1
LABELV $284
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 105
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 122
char 1 111
char 1 111
char 1 109
char 1 0
align 1
LABELV $283
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 105
char 1 116
char 1 101
char 1 109
char 1 115
char 1 47
char 1 98
char 1 105
char 1 110
char 1 111
char 1 99
char 1 117
char 1 108
char 1 97
char 1 114
char 1 115
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $282
char 1 105
char 1 116
char 1 101
char 1 109
char 1 95
char 1 98
char 1 105
char 1 110
char 1 111
char 1 99
char 1 117
char 1 108
char 1 97
char 1 114
char 1 115
char 1 0
align 1
LABELV $281
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 105
char 1 116
char 1 101
char 1 109
char 1 115
char 1 47
char 1 100
char 1 97
char 1 116
char 1 97
char 1 112
char 1 97
char 1 100
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $280
char 1 105
char 1 116
char 1 101
char 1 109
char 1 95
char 1 100
char 1 97
char 1 116
char 1 97
char 1 112
char 1 97
char 1 100
char 1 0
align 1
LABELV $279
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 105
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 98
char 1 97
char 1 99
char 1 116
char 1 97
char 1 0
align 1
LABELV $278
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 109
char 1 97
char 1 112
char 1 95
char 1 111
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 115
char 1 47
char 1 109
char 1 112
char 1 47
char 1 98
char 1 97
char 1 99
char 1 116
char 1 97
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $277
char 1 105
char 1 116
char 1 101
char 1 109
char 1 95
char 1 109
char 1 101
char 1 100
char 1 112
char 1 97
char 1 99
char 1 0
align 1
LABELV $276
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 47
char 1 100
char 1 101
char 1 116
char 1 112
char 1 97
char 1 99
char 1 107
char 1 47
char 1 115
char 1 116
char 1 105
char 1 99
char 1 107
char 1 46
char 1 119
char 1 97
char 1 118
char 1 32
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 109
char 1 111
char 1 118
char 1 101
char 1 114
char 1 115
char 1 47
char 1 100
char 1 111
char 1 111
char 1 114
char 1 115
char 1 47
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 102
char 1 105
char 1 101
char 1 108
char 1 100
char 1 95
char 1 111
char 1 110
char 1 46
char 1 119
char 1 97
char 1 118
char 1 32
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 109
char 1 111
char 1 118
char 1 101
char 1 114
char 1 115
char 1 47
char 1 100
char 1 111
char 1 111
char 1 114
char 1 115
char 1 47
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 102
char 1 105
char 1 101
char 1 108
char 1 100
char 1 95
char 1 111
char 1 102
char 1 102
char 1 46
char 1 119
char 1 97
char 1 118
char 1 32
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 109
char 1 111
char 1 118
char 1 101
char 1 114
char 1 115
char 1 47
char 1 100
char 1 111
char 1 111
char 1 114
char 1 115
char 1 47
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 102
char 1 105
char 1 101
char 1 108
char 1 100
char 1 95
char 1 108
char 1 112
char 1 46
char 1 119
char 1 97
char 1 118
char 1 32
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 101
char 1 102
char 1 102
char 1 101
char 1 99
char 1 116
char 1 115
char 1 47
char 1 98
char 1 117
char 1 109
char 1 112
char 1 102
char 1 105
char 1 101
char 1 108
char 1 100
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $275
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 105
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 115
char 1 104
char 1 105
char 1 101
char 1 108
char 1 100
char 1 119
char 1 97
char 1 108
char 1 108
char 1 0
align 1
LABELV $274
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 109
char 1 97
char 1 112
char 1 95
char 1 111
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 115
char 1 47
char 1 109
char 1 112
char 1 47
char 1 115
char 1 104
char 1 105
char 1 101
char 1 108
char 1 100
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $273
char 1 105
char 1 116
char 1 101
char 1 109
char 1 95
char 1 115
char 1 104
char 1 105
char 1 101
char 1 108
char 1 100
char 1 0
align 1
LABELV $272
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 105
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 115
char 1 101
char 1 101
char 1 107
char 1 101
char 1 114
char 1 0
align 1
LABELV $271
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 105
char 1 116
char 1 101
char 1 109
char 1 115
char 1 47
char 1 114
char 1 101
char 1 109
char 1 111
char 1 116
char 1 101
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $270
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 119
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 115
char 1 47
char 1 119
char 1 95
char 1 112
char 1 107
char 1 117
char 1 112
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $269
char 1 105
char 1 116
char 1 101
char 1 109
char 1 95
char 1 115
char 1 101
char 1 101
char 1 107
char 1 101
char 1 114
char 1 0
align 1
LABELV $268
char 1 103
char 1 102
char 1 120
char 1 47
char 1 104
char 1 117
char 1 100
char 1 47
char 1 105
char 1 95
char 1 105
char 1 99
char 1 111
char 1 110
char 1 95
char 1 109
char 1 101
char 1 100
char 1 107
char 1 105
char 1 116
char 1 0
align 1
LABELV $267
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 109
char 1 97
char 1 112
char 1 95
char 1 111
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 115
char 1 47
char 1 109
char 1 112
char 1 47
char 1 109
char 1 101
char 1 100
char 1 112
char 1 97
char 1 99
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $266
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 112
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 47
char 1 112
char 1 105
char 1 99
char 1 107
char 1 117
char 1 112
char 1 104
char 1 101
char 1 97
char 1 108
char 1 116
char 1 104
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $265
char 1 105
char 1 116
char 1 101
char 1 109
char 1 95
char 1 109
char 1 101
char 1 100
char 1 112
char 1 97
char 1 107
char 1 95
char 1 105
char 1 110
char 1 115
char 1 116
char 1 97
char 1 110
char 1 116
char 1 0
align 1
LABELV $264
char 1 103
char 1 102
char 1 120
char 1 47
char 1 109
char 1 112
char 1 47
char 1 108
char 1 97
char 1 114
char 1 103
char 1 101
char 1 95
char 1 115
char 1 104
char 1 105
char 1 101
char 1 108
char 1 100
char 1 0
align 1
LABELV $263
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 109
char 1 97
char 1 112
char 1 95
char 1 111
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 115
char 1 47
char 1 109
char 1 112
char 1 47
char 1 112
char 1 115
char 1 100
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $262
char 1 105
char 1 116
char 1 101
char 1 109
char 1 95
char 1 115
char 1 104
char 1 105
char 1 101
char 1 108
char 1 100
char 1 95
char 1 108
char 1 114
char 1 103
char 1 95
char 1 105
char 1 110
char 1 115
char 1 116
char 1 97
char 1 110
char 1 116
char 1 0
align 1
LABELV $261
char 1 103
char 1 102
char 1 120
char 1 47
char 1 109
char 1 112
char 1 47
char 1 115
char 1 109
char 1 97
char 1 108
char 1 108
char 1 95
char 1 115
char 1 104
char 1 105
char 1 101
char 1 108
char 1 100
char 1 0
align 1
LABELV $260
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 109
char 1 97
char 1 112
char 1 95
char 1 111
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 115
char 1 47
char 1 109
char 1 112
char 1 47
char 1 112
char 1 115
char 1 100
char 1 95
char 1 115
char 1 109
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $259
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 112
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 47
char 1 112
char 1 105
char 1 99
char 1 107
char 1 117
char 1 112
char 1 115
char 1 104
char 1 105
char 1 101
char 1 108
char 1 100
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $258
char 1 105
char 1 116
char 1 101
char 1 109
char 1 95
char 1 115
char 1 104
char 1 105
char 1 101
char 1 108
char 1 100
char 1 95
char 1 115
char 1 109
char 1 95
char 1 105
char 1 110
char 1 115
char 1 116
char 1 97
char 1 110
char 1 116
char 1 0
align 1
LABELV $257
char 1 0
align 1
LABELV $256
char 1 37
char 1 105
char 1 0
align 1
LABELV $132
char 1 37
char 1 105
char 1 45
char 1 0
align 1
LABELV $131
char 1 55
char 1 45
char 1 49
char 1 45
char 1 48
char 1 51
char 1 50
char 1 51
char 1 51
char 1 48
char 1 48
char 1 48
char 1 48
char 1 48
char 1 48
char 1 48
char 1 48
char 1 48
char 1 49
char 1 51
char 1 51
char 1 51
char 1 0
align 1
LABELV $127
char 1 77
char 1 65
char 1 83
char 1 84
char 1 69
char 1 82
char 1 89
char 1 55
char 1 0
align 1
LABELV $126
char 1 77
char 1 65
char 1 83
char 1 84
char 1 69
char 1 82
char 1 89
char 1 54
char 1 0
align 1
LABELV $125
char 1 77
char 1 65
char 1 83
char 1 84
char 1 69
char 1 82
char 1 89
char 1 53
char 1 0
align 1
LABELV $124
char 1 77
char 1 65
char 1 83
char 1 84
char 1 69
char 1 82
char 1 89
char 1 52
char 1 0
align 1
LABELV $123
char 1 77
char 1 65
char 1 83
char 1 84
char 1 69
char 1 82
char 1 89
char 1 51
char 1 0
align 1
LABELV $122
char 1 77
char 1 65
char 1 83
char 1 84
char 1 69
char 1 82
char 1 89
char 1 50
char 1 0
align 1
LABELV $121
char 1 77
char 1 65
char 1 83
char 1 84
char 1 69
char 1 82
char 1 89
char 1 49
char 1 0
align 1
LABELV $120
char 1 77
char 1 65
char 1 83
char 1 84
char 1 69
char 1 82
char 1 89
char 1 48
char 1 0
