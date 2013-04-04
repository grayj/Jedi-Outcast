data
export weaponFromMOD
align 4
LABELV weaponFromMOD
byte 4 0
byte 4 1
byte 4 0
byte 4 2
byte 4 3
byte 4 3
byte 4 4
byte 4 5
byte 4 5
byte 4 5
byte 4 6
byte 4 7
byte 4 7
byte 4 7
byte 4 8
byte 4 8
byte 4 9
byte 4 9
byte 4 10
byte 4 10
byte 4 10
byte 4 10
byte 4 11
byte 4 11
byte 4 12
byte 4 12
byte 4 13
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
export weaponNameFromIndex
align 4
LABELV weaponNameFromIndex
address $78
address $79
address $80
address $81
address $82
address $83
address $84
address $85
address $86
address $87
address $88
address $89
address $90
address $91
address $92
address $93
export G_LogWeaponInit
code
proc G_LogWeaponInit 0 12
file "../g_log.c"
line 106
;1:#include "g_local.h"
;2:
;3:#define LOGGING_WEAPONS	
;4:
;5:// Weapon statistic logging.
;6:// Nothing super-fancy here, I just want to keep track of, per player:
;7://		--hom many times a weapon/item is picked up
;8://		--how many times a weapon/item is used/fired
;9://		--the total damage done by that weapon
;10://		--the number of kills by that weapon
;11://		--the number of deaths while holding that weapon
;12://		--the time spent with each weapon
;13://
;14:// Additionally,
;15://		--how many times each powerup or item is picked up
;16:
;17:
;18:#ifdef LOGGING_WEAPONS
;19:int G_WeaponLogPickups[MAX_CLIENTS][WP_NUM_WEAPONS];
;20:int G_WeaponLogFired[MAX_CLIENTS][WP_NUM_WEAPONS];
;21:int G_WeaponLogDamage[MAX_CLIENTS][MOD_MAX];
;22:int G_WeaponLogKills[MAX_CLIENTS][MOD_MAX];
;23:int G_WeaponLogDeaths[MAX_CLIENTS][WP_NUM_WEAPONS];
;24:int G_WeaponLogFrags[MAX_CLIENTS][MAX_CLIENTS];
;25:int G_WeaponLogTime[MAX_CLIENTS][WP_NUM_WEAPONS];
;26:int G_WeaponLogLastTime[MAX_CLIENTS];
;27:qboolean G_WeaponLogClientTouch[MAX_CLIENTS];
;28:int G_WeaponLogPowerups[MAX_CLIENTS][HI_NUM_HOLDABLE];
;29:int	G_WeaponLogItems[MAX_CLIENTS][PW_NUM_POWERUPS];
;30:
;31:extern vmCvar_t	g_statLog;
;32:extern vmCvar_t	g_statLogFile;
;33:
;34:// MOD-weapon mapping array.
;35:int weaponFromMOD[MOD_MAX] =
;36:{
;37:	WP_NONE,				//MOD_UNKNOWN,
;38:	WP_STUN_BATON,			//MOD_STUN_BATON,
;39:	WP_NONE,				//MOD_MELEE,
;40:	WP_SABER,				//MOD_SABER,
;41:	WP_BRYAR_PISTOL,		//MOD_BRYAR_PISTOL,
;42:	WP_BRYAR_PISTOL,		//MOD_BRYAR_PISTOL_ALT,
;43:	WP_BLASTER,				//MOD_BLASTER,
;44:	WP_DISRUPTOR,			//MOD_DISRUPTOR,
;45:	WP_DISRUPTOR,			//MOD_DISRUPTOR_SPLASH,
;46:	WP_DISRUPTOR,			//MOD_DISRUPTOR_SNIPER,
;47:	WP_BOWCASTER,			//MOD_BOWCASTER,
;48:	WP_REPEATER,			//MOD_REPEATER,
;49:	WP_REPEATER,			//MOD_REPEATER_ALT,
;50:	WP_REPEATER,			//MOD_REPEATER_ALT_SPLASH,
;51:	WP_DEMP2,				//MOD_DEMP2,
;52:	WP_DEMP2,				//MOD_DEMP2_ALT,
;53:	WP_FLECHETTE,			//MOD_FLECHETTE,
;54:	WP_FLECHETTE,			//MOD_FLECHETTE_ALT_SPLASH,
;55:	WP_ROCKET_LAUNCHER,		//MOD_ROCKET,
;56:	WP_ROCKET_LAUNCHER,		//MOD_ROCKET_SPLASH,
;57:	WP_ROCKET_LAUNCHER,		//MOD_ROCKET_HOMING,
;58:	WP_ROCKET_LAUNCHER,		//MOD_ROCKET_HOMING_SPLASH,
;59:	WP_THERMAL,				//MOD_THERMAL,
;60:	WP_THERMAL,				//MOD_THERMAL_SPLASH,
;61:	WP_TRIP_MINE,			//MOD_TRIP_MINE_SPLASH,
;62:	WP_TRIP_MINE,			//MOD_TIMED_MINE_SPLASH,
;63:	WP_DET_PACK,			//MOD_DET_PACK_SPLASH,
;64:	WP_NONE,				//MOD_FORCE_DARK,
;65:	WP_NONE,				//MOD_SENTRY,
;66:	WP_NONE,				//MOD_WATER,
;67:	WP_NONE,				//MOD_SLIME,
;68:	WP_NONE,				//MOD_LAVA,
;69:	WP_NONE,				//MOD_CRUSH,
;70:	WP_NONE,				//MOD_TELEFRAG,
;71:	WP_NONE,				//MOD_FALLING,
;72:	WP_NONE,				//MOD_SUICIDE,
;73:	WP_NONE,				//MOD_TARGET_LASER,
;74:	WP_NONE,				//MOD_TRIGGER_HURT,
;75:};
;76:
;77:char *weaponNameFromIndex[WP_NUM_WEAPONS] = 
;78:{
;79:	"No Weapon",
;80:	"Stun Baton",				
;81:	"Saber",	
;82:	"Bryar Pistol",				
;83:	"Blaster",		
;84:	"Disruptor",				
;85:	"Bowcaster",	
;86:	"Repeater",	
;87:	"Demp2",
;88:	"Flechette",
;89:	"Rocket Launcher",
;90:	"Thermal",
;91:	"Tripmine",
;92:	"Detpack",
;93:	"Emplaced gun",
;94:	"Turret"
;95:};
;96:
;97:extern char	*modNames[];
;98:
;99:#endif //LOGGING_WEAPONS
;100:
;101:/*
;102:=================
;103:G_LogWeaponInit
;104:=================
;105:*/
;106:void G_LogWeaponInit(void) {
line 108
;107:#ifdef LOGGING_WEAPONS
;108:	memset(G_WeaponLogPickups, 0, sizeof(G_WeaponLogPickups));
ADDRGP4 G_WeaponLogPickups
ARGP4
CNSTI4 0
ARGI4
CNSTI4 2048
ARGI4
ADDRGP4 memset
CALLP4
pop
line 109
;109:	memset(G_WeaponLogFired, 0, sizeof(G_WeaponLogFired));
ADDRGP4 G_WeaponLogFired
ARGP4
CNSTI4 0
ARGI4
CNSTI4 2048
ARGI4
ADDRGP4 memset
CALLP4
pop
line 110
;110:	memset(G_WeaponLogDamage, 0, sizeof(G_WeaponLogDamage));
ADDRGP4 G_WeaponLogDamage
ARGP4
CNSTI4 0
ARGI4
CNSTI4 4864
ARGI4
ADDRGP4 memset
CALLP4
pop
line 111
;111:	memset(G_WeaponLogKills, 0, sizeof(G_WeaponLogKills));
ADDRGP4 G_WeaponLogKills
ARGP4
CNSTI4 0
ARGI4
CNSTI4 4864
ARGI4
ADDRGP4 memset
CALLP4
pop
line 112
;112:	memset(G_WeaponLogDeaths, 0, sizeof(G_WeaponLogDeaths));
ADDRGP4 G_WeaponLogDeaths
ARGP4
CNSTI4 0
ARGI4
CNSTI4 2048
ARGI4
ADDRGP4 memset
CALLP4
pop
line 113
;113:	memset(G_WeaponLogFrags, 0, sizeof(G_WeaponLogFrags));
ADDRGP4 G_WeaponLogFrags
ARGP4
CNSTI4 0
ARGI4
CNSTI4 4096
ARGI4
ADDRGP4 memset
CALLP4
pop
line 114
;114:	memset(G_WeaponLogTime, 0, sizeof(G_WeaponLogTime));
ADDRGP4 G_WeaponLogTime
ARGP4
CNSTI4 0
ARGI4
CNSTI4 2048
ARGI4
ADDRGP4 memset
CALLP4
pop
line 115
;115:	memset(G_WeaponLogLastTime, 0, sizeof(G_WeaponLogLastTime));
ADDRGP4 G_WeaponLogLastTime
ARGP4
CNSTI4 0
ARGI4
CNSTI4 128
ARGI4
ADDRGP4 memset
CALLP4
pop
line 116
;116:	memset(G_WeaponLogPowerups, 0, sizeof(G_WeaponLogPowerups));
ADDRGP4 G_WeaponLogPowerups
ARGP4
CNSTI4 0
ARGI4
CNSTI4 896
ARGI4
ADDRGP4 memset
CALLP4
pop
line 117
;117:	memset(G_WeaponLogItems, 0, sizeof(G_WeaponLogItems));
ADDRGP4 G_WeaponLogItems
ARGP4
CNSTI4 0
ARGI4
CNSTI4 2048
ARGI4
ADDRGP4 memset
CALLP4
pop
line 119
;118:#endif //LOGGING_WEAPONS
;119:}
LABELV $94
endproc G_LogWeaponInit 0 12
export G_LogWeaponPickup
proc G_LogWeaponPickup 4 0
line 122
;120:
;121:void QDECL G_LogWeaponPickup(int client, int weaponid)
;122:{
line 124
;123:#ifdef LOGGING_WEAPONS
;124:	G_WeaponLogPickups[client][weaponid]++;
ADDRLP4 0
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogPickups
ADDP4
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 125
;125:	G_WeaponLogClientTouch[client] = qtrue;
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
CNSTI4 1
ASGNI4
line 127
;126:#endif //_LOGGING_WEAPONS
;127:}
LABELV $95
endproc G_LogWeaponPickup 4 0
export G_LogWeaponFire
proc G_LogWeaponFire 16 0
line 130
;128:
;129:void QDECL G_LogWeaponFire(int client, int weaponid)
;130:{
line 134
;131:#ifdef LOGGING_WEAPONS
;132:	int dur;
;133:
;134:	G_WeaponLogFired[client][weaponid]++;
ADDRLP4 4
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogFired
ADDP4
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 135
;135:	dur = level.time - G_WeaponLogLastTime[client];
ADDRLP4 0
ADDRGP4 level+32
INDIRI4
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogLastTime
ADDP4
INDIRI4
SUBI4
ASGNI4
line 136
;136:	if (dur > 5000)		// 5 second max.
ADDRLP4 0
INDIRI4
CNSTI4 5000
LEI4 $98
line 137
;137:		G_WeaponLogTime[client][weaponid] += 5000;
ADDRLP4 8
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogTime
ADDP4
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 5000
ADDI4
ASGNI4
ADDRGP4 $99
JUMPV
LABELV $98
line 139
;138:	else
;139:		G_WeaponLogTime[client][weaponid] += dur;
ADDRLP4 12
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogTime
ADDP4
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
ASGNI4
LABELV $99
line 140
;140:	G_WeaponLogLastTime[client] = level.time;
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogLastTime
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 141
;141:	G_WeaponLogClientTouch[client] = qtrue;
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
CNSTI4 1
ASGNI4
line 143
;142:#endif //_LOGGING_WEAPONS
;143:}
LABELV $96
endproc G_LogWeaponFire 16 0
export G_LogWeaponDamage
proc G_LogWeaponDamage 4 0
line 146
;144:
;145:void QDECL G_LogWeaponDamage(int client, int mod, int amount)
;146:{
line 148
;147:#ifdef LOGGING_WEAPONS
;148:	if (client>=MAX_CLIENTS)
ADDRFP4 0
INDIRI4
CNSTI4 32
LTI4 $102
line 149
;149:		return;
ADDRGP4 $101
JUMPV
LABELV $102
line 150
;150:	G_WeaponLogDamage[client][mod] += amount;
ADDRLP4 0
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 152
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogDamage
ADDP4
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRFP4 8
INDIRI4
ADDI4
ASGNI4
line 151
;151:	G_WeaponLogClientTouch[client] = qtrue;
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
CNSTI4 1
ASGNI4
line 153
;152:#endif //_LOGGING_WEAPONS
;153:}
LABELV $101
endproc G_LogWeaponDamage 4 0
export G_LogWeaponKill
proc G_LogWeaponKill 4 0
line 156
;154:
;155:void QDECL G_LogWeaponKill(int client, int mod)
;156:{
line 158
;157:#ifdef LOGGING_WEAPONS
;158:	if (client>=MAX_CLIENTS)
ADDRFP4 0
INDIRI4
CNSTI4 32
LTI4 $105
line 159
;159:		return;
ADDRGP4 $104
JUMPV
LABELV $105
line 160
;160:	G_WeaponLogKills[client][mod]++;
ADDRLP4 0
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 152
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills
ADDP4
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 161
;161:	G_WeaponLogClientTouch[client] = qtrue;
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
CNSTI4 1
ASGNI4
line 163
;162:#endif //_LOGGING_WEAPONS
;163:}
LABELV $104
endproc G_LogWeaponKill 4 0
export G_LogWeaponFrag
proc G_LogWeaponFrag 8 0
line 166
;164:
;165:void QDECL G_LogWeaponFrag(int attacker, int deadguy)
;166:{
line 168
;167:#ifdef LOGGING_WEAPONS
;168:	if ( (attacker>=MAX_CLIENTS) || (deadguy>=MAX_CLIENTS) )
ADDRLP4 0
CNSTI4 32
ASGNI4
ADDRFP4 0
INDIRI4
ADDRLP4 0
INDIRI4
GEI4 $110
ADDRFP4 4
INDIRI4
ADDRLP4 0
INDIRI4
LTI4 $108
LABELV $110
line 169
;169:		return;
ADDRGP4 $107
JUMPV
LABELV $108
line 170
;170:	G_WeaponLogFrags[attacker][deadguy]++;
ADDRLP4 4
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 7
LSHI4
ADDRGP4 G_WeaponLogFrags
ADDP4
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 171
;171:	G_WeaponLogClientTouch[attacker] = qtrue;
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
CNSTI4 1
ASGNI4
line 173
;172:#endif //_LOGGING_WEAPONS
;173:}
LABELV $107
endproc G_LogWeaponFrag 8 0
export G_LogWeaponDeath
proc G_LogWeaponDeath 4 0
line 176
;174:
;175:void QDECL G_LogWeaponDeath(int client, int weaponid)
;176:{
line 178
;177:#ifdef LOGGING_WEAPONS
;178:	if (client>=MAX_CLIENTS)
ADDRFP4 0
INDIRI4
CNSTI4 32
LTI4 $112
line 179
;179:		return;
ADDRGP4 $111
JUMPV
LABELV $112
line 180
;180:	G_WeaponLogDeaths[client][weaponid]++;
ADDRLP4 0
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogDeaths
ADDP4
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 181
;181:	G_WeaponLogClientTouch[client] = qtrue;
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
CNSTI4 1
ASGNI4
line 183
;182:#endif //_LOGGING_WEAPONS
;183:}
LABELV $111
endproc G_LogWeaponDeath 4 0
export G_LogWeaponPowerup
proc G_LogWeaponPowerup 4 0
line 186
;184:
;185:void QDECL G_LogWeaponPowerup(int client, int powerupid)
;186:{
line 188
;187:#ifdef LOGGING_WEAPONS
;188:	if (client>=MAX_CLIENTS)
ADDRFP4 0
INDIRI4
CNSTI4 32
LTI4 $115
line 189
;189:		return;
ADDRGP4 $114
JUMPV
LABELV $115
line 190
;190:	G_WeaponLogPowerups[client][powerupid]++;
ADDRLP4 0
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 28
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogPowerups
ADDP4
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 191
;191:	G_WeaponLogClientTouch[client] = qtrue;
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
CNSTI4 1
ASGNI4
line 193
;192:#endif //_LOGGING_WEAPONS
;193:}
LABELV $114
endproc G_LogWeaponPowerup 4 0
export G_LogWeaponItem
proc G_LogWeaponItem 4 0
line 196
;194:
;195:void QDECL G_LogWeaponItem(int client, int itemid)
;196:{
line 198
;197:#ifdef LOGGING_WEAPONS
;198:	if (client>=MAX_CLIENTS)
ADDRFP4 0
INDIRI4
CNSTI4 32
LTI4 $118
line 199
;199:		return;
ADDRGP4 $117
JUMPV
LABELV $118
line 200
;200:	G_WeaponLogItems[client][itemid]++;
ADDRLP4 0
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogItems
ADDP4
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 201
;201:	G_WeaponLogClientTouch[client] = qtrue;
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
CNSTI4 1
ASGNI4
line 203
;202:#endif //_LOGGING_WEAPONS
;203:}
LABELV $117
endproc G_LogWeaponItem 4 0
export G_LogWeaponOutput
proc G_LogWeaponOutput 3124 20
line 226
;204:
;205:
;206:// Run through each player.  Print out:
;207://	-- Most commonly picked up weapon.
;208://  -- Weapon with which the most time was spent.
;209://  -- Weapon that was most often died with.
;210://  -- Damage type with which the most damage was done.
;211://  -- Damage type with the most kills.
;212://  -- Weapon with which the most damage was done.
;213://	-- Weapon with which the most damage was done per shot.
;214://
;215:// For the whole game, print out:
;216://  -- Total pickups of each weapon.
;217://  -- Total time spent with each weapon.
;218://  -- Total damage done with each weapon.
;219://  -- Total damage done for each damage type.
;220://  -- Number of kills with each weapon.
;221://  -- Number of kills for each damage type.
;222://  -- Damage per shot with each weapon.
;223://  -- Number of deaths with each weapon.
;224:
;225:void G_LogWeaponOutput(void)
;226:{
line 244
;227:#ifdef LOGGING_WEAPONS
;228:	int i,j,curwp;
;229:	float pershot;
;230:	fileHandle_t weaponfile;
;231:	char string[1024];
;232:
;233:	int totalpickups[WP_NUM_WEAPONS];
;234:	int totaltime[WP_NUM_WEAPONS];
;235:	int totaldeaths[WP_NUM_WEAPONS];
;236:	int totaldamageMOD[MOD_MAX];
;237:	int totalkillsMOD[MOD_MAX];
;238:	int totaldamage[WP_NUM_WEAPONS];
;239:	int totalkills[WP_NUM_WEAPONS];
;240:	int totalshots[WP_NUM_WEAPONS];
;241:	int percharacter[WP_NUM_WEAPONS];
;242:	char info[1024];
;243:	char mapname[128];
;244:	char *nameptr, *unknownname="<Unknown>";
ADDRLP4 1796
ADDRGP4 $121
ASGNP4
line 246
;245:
;246:	if (!g_statLog.integer)
ADDRGP4 g_statLog+12
INDIRI4
CNSTI4 0
NEI4 $122
line 247
;247:	{
line 248
;248:		return;
ADDRGP4 $120
JUMPV
LABELV $122
line 251
;249:	}
;250:
;251:	G_LogPrintf("*****************************Weapon Log:\n" );
ADDRGP4 $125
ARGP4
ADDRGP4 G_LogPrintf
CALLV
pop
line 253
;252:
;253:	memset(totalpickups, 0, sizeof(totalpickups));
ADDRLP4 1476
ARGP4
CNSTI4 0
ARGI4
CNSTI4 64
ARGI4
ADDRGP4 memset
CALLP4
pop
line 254
;254:	memset(totaltime, 0, sizeof(totaltime));
ADDRLP4 1540
ARGP4
CNSTI4 0
ARGI4
CNSTI4 64
ARGI4
ADDRGP4 memset
CALLP4
pop
line 255
;255:	memset(totaldeaths, 0, sizeof(totaldeaths));
ADDRLP4 1604
ARGP4
CNSTI4 0
ARGI4
CNSTI4 64
ARGI4
ADDRGP4 memset
CALLP4
pop
line 256
;256:	memset(totaldamageMOD, 0, sizeof(totaldamageMOD));
ADDRLP4 1108
ARGP4
CNSTI4 0
ARGI4
CNSTI4 152
ARGI4
ADDRGP4 memset
CALLP4
pop
line 257
;257:	memset(totalkillsMOD, 0, sizeof(totalkillsMOD));
ADDRLP4 1260
ARGP4
CNSTI4 0
ARGI4
CNSTI4 152
ARGI4
ADDRGP4 memset
CALLP4
pop
line 258
;258:	memset(totaldamage, 0, sizeof(totaldamage));
ADDRLP4 1668
ARGP4
CNSTI4 0
ARGI4
CNSTI4 64
ARGI4
ADDRGP4 memset
CALLP4
pop
line 259
;259:	memset(totalkills, 0, sizeof(totalkills));
ADDRLP4 1732
ARGP4
CNSTI4 0
ARGI4
CNSTI4 64
ARGI4
ADDRGP4 memset
CALLP4
pop
line 260
;260:	memset(totalshots, 0, sizeof(totalshots));
ADDRLP4 1412
ARGP4
CNSTI4 0
ARGI4
CNSTI4 64
ARGI4
ADDRGP4 memset
CALLP4
pop
line 262
;261:
;262:	for (i=0; i<MAX_CLIENTS; i++)
ADDRLP4 1028
CNSTI4 0
ASGNI4
LABELV $126
line 263
;263:	{
line 264
;264:		if (G_WeaponLogClientTouch[i])
ADDRLP4 1028
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
INDIRI4
CNSTI4 0
EQI4 $130
line 265
;265:		{	// Ignore any entity/clients we don't care about!
line 266
;266:			for (j=0;j<WP_NUM_WEAPONS;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $132
line 267
;267:			{
line 268
;268:				totalpickups[j] += G_WeaponLogPickups[i][j];
ADDRLP4 2956
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 2960
ADDRLP4 2956
INDIRI4
ADDRLP4 1476
ADDP4
ASGNP4
ADDRLP4 2960
INDIRP4
ADDRLP4 2960
INDIRP4
INDIRI4
ADDRLP4 2956
INDIRI4
ADDRLP4 1028
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogPickups
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 269
;269:				totaltime[j] += G_WeaponLogTime[i][j];
ADDRLP4 2964
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 2968
ADDRLP4 2964
INDIRI4
ADDRLP4 1540
ADDP4
ASGNP4
ADDRLP4 2968
INDIRP4
ADDRLP4 2968
INDIRP4
INDIRI4
ADDRLP4 2964
INDIRI4
ADDRLP4 1028
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogTime
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 270
;270:				totaldeaths[j] += G_WeaponLogDeaths[i][j];
ADDRLP4 2972
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 2976
ADDRLP4 2972
INDIRI4
ADDRLP4 1604
ADDP4
ASGNP4
ADDRLP4 2976
INDIRP4
ADDRLP4 2976
INDIRP4
INDIRI4
ADDRLP4 2972
INDIRI4
ADDRLP4 1028
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogDeaths
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 271
;271:				totalshots[j] += G_WeaponLogFired[i][j];
ADDRLP4 2980
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 2984
ADDRLP4 2980
INDIRI4
ADDRLP4 1412
ADDP4
ASGNP4
ADDRLP4 2984
INDIRP4
ADDRLP4 2984
INDIRP4
INDIRI4
ADDRLP4 2980
INDIRI4
ADDRLP4 1028
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogFired
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 272
;272:			}
LABELV $133
line 266
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $132
line 274
;273:
;274:			for (j=0;j<MOD_MAX;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $136
line 275
;275:			{
line 276
;276:				totaldamageMOD[j] += G_WeaponLogDamage[i][j];
ADDRLP4 2956
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 2960
ADDRLP4 2956
INDIRI4
ADDRLP4 1108
ADDP4
ASGNP4
ADDRLP4 2960
INDIRP4
ADDRLP4 2960
INDIRP4
INDIRI4
ADDRLP4 2956
INDIRI4
CNSTI4 152
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 G_WeaponLogDamage
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 277
;277:				totalkillsMOD[j] += G_WeaponLogKills[i][j];
ADDRLP4 2964
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 2968
ADDRLP4 2964
INDIRI4
ADDRLP4 1260
ADDP4
ASGNP4
ADDRLP4 2968
INDIRP4
ADDRLP4 2968
INDIRP4
INDIRI4
ADDRLP4 2964
INDIRI4
CNSTI4 152
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 278
;278:			}
LABELV $137
line 274
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 38
LTI4 $136
line 279
;279:		}
LABELV $130
line 280
;280:	}
LABELV $127
line 262
ADDRLP4 1028
ADDRLP4 1028
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 1028
INDIRI4
CNSTI4 32
LTI4 $126
line 283
;281:
;282:	// Now total the weapon data from the MOD data.
;283:	for (j=0; j<MOD_MAX; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $140
line 284
;284:	{
line 285
;285:		if (j <= MOD_SENTRY)
ADDRLP4 0
INDIRI4
CNSTI4 28
GTI4 $144
line 286
;286:		{
line 287
;287:			curwp = weaponFromMOD[j];
ADDRLP4 1104
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 weaponFromMOD
ADDP4
INDIRI4
ASGNI4
line 288
;288:			totaldamage[curwp] += totaldamageMOD[j];
ADDRLP4 2956
CNSTI4 2
ASGNI4
ADDRLP4 2960
ADDRLP4 1104
INDIRI4
ADDRLP4 2956
INDIRI4
LSHI4
ADDRLP4 1668
ADDP4
ASGNP4
ADDRLP4 2960
INDIRP4
ADDRLP4 2960
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
ADDRLP4 2956
INDIRI4
LSHI4
ADDRLP4 1108
ADDP4
INDIRI4
ADDI4
ASGNI4
line 289
;289:			totalkills[curwp] += totalkillsMOD[j];
ADDRLP4 2964
CNSTI4 2
ASGNI4
ADDRLP4 2968
ADDRLP4 1104
INDIRI4
ADDRLP4 2964
INDIRI4
LSHI4
ADDRLP4 1732
ADDP4
ASGNP4
ADDRLP4 2968
INDIRP4
ADDRLP4 2968
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
ADDRLP4 2964
INDIRI4
LSHI4
ADDRLP4 1260
ADDP4
INDIRI4
ADDI4
ASGNI4
line 290
;290:		}
LABELV $144
line 291
;291:	}
LABELV $141
line 283
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 38
LTI4 $140
line 293
;292:
;293:	G_LogPrintf(  "\n****Data by Weapon:\n" );
ADDRGP4 $146
ARGP4
ADDRGP4 G_LogPrintf
CALLV
pop
line 294
;294:	for (j=0; j<WP_NUM_WEAPONS; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $147
line 295
;295:	{
line 296
;296:		G_LogPrintf("%15s:  Pickups: %4d,  Time:  %5d,  Deaths: %5d\n", 
ADDRGP4 $151
ARGP4
ADDRLP4 2956
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 2956
INDIRI4
ADDRGP4 weaponNameFromIndex
ADDP4
INDIRP4
ARGP4
ADDRLP4 2956
INDIRI4
ADDRLP4 1476
ADDP4
INDIRI4
ARGI4
ADDRLP4 2956
INDIRI4
ADDRLP4 1540
ADDP4
INDIRI4
CNSTI4 1000
DIVI4
ARGI4
ADDRLP4 2956
INDIRI4
ADDRLP4 1604
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_LogPrintf
CALLV
pop
line 298
;297:				weaponNameFromIndex[j], totalpickups[j], (int)(totaltime[j]/1000), totaldeaths[j]);
;298:	}
LABELV $148
line 294
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $147
line 300
;299:
;300:	G_LogPrintf(  "\n****Combat Data by Weapon:\n" );
ADDRGP4 $152
ARGP4
ADDRGP4 G_LogPrintf
CALLV
pop
line 301
;301:	for (j=0; j<WP_NUM_WEAPONS; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $153
line 302
;302:	{
line 303
;303:		if (totalshots[j] > 0)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1412
ADDP4
INDIRI4
CNSTI4 0
LEI4 $157
line 304
;304:		{
line 305
;305:			pershot = (float)(totaldamage[j])/(float)(totalshots[j]);
ADDRLP4 2956
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 1800
ADDRLP4 2956
INDIRI4
ADDRLP4 1668
ADDP4
INDIRI4
CVIF4 4
ADDRLP4 2956
INDIRI4
ADDRLP4 1412
ADDP4
INDIRI4
CVIF4 4
DIVF4
ASGNF4
line 306
;306:		}
ADDRGP4 $158
JUMPV
LABELV $157
line 308
;307:		else
;308:		{
line 309
;309:			pershot = 0;
ADDRLP4 1800
CNSTF4 0
ASGNF4
line 310
;310:		}
LABELV $158
line 311
;311:		G_LogPrintf("%15s:  Damage: %6d,  Kills: %5d,  Dmg per Shot: %f\n", 
ADDRGP4 $159
ARGP4
ADDRLP4 2956
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 2956
INDIRI4
ADDRGP4 weaponNameFromIndex
ADDP4
INDIRP4
ARGP4
ADDRLP4 2956
INDIRI4
ADDRLP4 1668
ADDP4
INDIRI4
ARGI4
ADDRLP4 2956
INDIRI4
ADDRLP4 1732
ADDP4
INDIRI4
ARGI4
ADDRLP4 1800
INDIRF4
ARGF4
ADDRGP4 G_LogPrintf
CALLV
pop
line 313
;312:				weaponNameFromIndex[j], totaldamage[j], totalkills[j], pershot);
;313:	}
LABELV $154
line 301
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $153
line 315
;314:
;315:	G_LogPrintf(  "\n****Combat Data By Damage Type:\n" );
ADDRGP4 $160
ARGP4
ADDRGP4 G_LogPrintf
CALLV
pop
line 316
;316:	for (j=0; j<MOD_MAX; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $161
line 317
;317:	{
line 318
;318:		G_LogPrintf("%25s:  Damage: %6d,  Kills: %5d\n", 
ADDRGP4 $165
ARGP4
ADDRLP4 2956
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 2956
INDIRI4
ADDRGP4 modNames
ADDP4
INDIRP4
ARGP4
ADDRLP4 2956
INDIRI4
ADDRLP4 1108
ADDP4
INDIRI4
ARGI4
ADDRLP4 2956
INDIRI4
ADDRLP4 1260
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_LogPrintf
CALLV
pop
line 320
;319:				modNames[j], totaldamageMOD[j], totalkillsMOD[j]);
;320:	}
LABELV $162
line 316
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 38
LTI4 $161
line 322
;321:
;322:	G_LogPrintf("\n");
ADDRGP4 $166
ARGP4
ADDRGP4 G_LogPrintf
CALLV
pop
line 327
;323:
;324:
;325:
;326:	// Write the whole weapon statistic log out to a file.
;327:	trap_FS_FOpenFile( g_statLogFile.string, &weaponfile, FS_APPEND );
ADDRGP4 g_statLogFile+16
ARGP4
ADDRLP4 1032
ARGP4
CNSTI4 2
ARGI4
ADDRGP4 trap_FS_FOpenFile
CALLI4
pop
line 328
;328:	if (!weaponfile) {	//failed to open file, let's not crash, shall we?
ADDRLP4 1032
INDIRI4
CNSTI4 0
NEI4 $168
line 329
;329:		return;
ADDRGP4 $120
JUMPV
LABELV $168
line 333
;330:	}
;331:
;332:	// Write out the level name
;333:	trap_GetServerinfo(info, sizeof(info));
ADDRLP4 1932
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetServerinfo
CALLV
pop
line 334
;334:	strncpy(mapname, Info_ValueForKey( info, "mapname" ), sizeof(mapname)-1);
ADDRLP4 1932
ARGP4
ADDRGP4 $170
ARGP4
ADDRLP4 2956
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1804
ARGP4
ADDRLP4 2956
INDIRP4
ARGP4
CNSTI4 127
ARGI4
ADDRGP4 strncpy
CALLP4
pop
line 335
;335:	mapname[sizeof(mapname)-1] = '\0';
ADDRLP4 1804+127
CNSTI1 0
ASGNI1
line 337
;336:
;337:	Com_sprintf(string, sizeof(string), "\n\n\nLevel:\t%s\n\n\n", mapname);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $172
ARGP4
ADDRLP4 1804
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 338
;338:	trap_FS_Write( string, strlen( string ), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 2960
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 2960
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 344
;339:
;340:
;341:	// Combat data per character
;342:	
;343:	// Start with Pickups per character
;344:	Com_sprintf(string, sizeof(string), "Weapon Pickups per Player:\n\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $173
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 345
;345:	trap_FS_Write( string, strlen( string ), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 2964
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 2964
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 347
;346:
;347:	Com_sprintf(string, sizeof(string), "Player");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $174
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 348
;348:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 2968
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 2968
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 350
;349:
;350:	for (j=0; j<WP_NUM_WEAPONS; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $175
line 351
;351:	{
line 352
;352:		Com_sprintf(string, sizeof(string), "\t%s", weaponNameFromIndex[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $179
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 weaponNameFromIndex
ADDP4
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 353
;353:		trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 2972
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 2972
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 354
;354:	}
LABELV $176
line 350
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $175
line 355
;355:	Com_sprintf(string, sizeof(string), "\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $166
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 356
;356:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 2972
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 2972
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 359
;357:
;358:	// Cycle through each player, give their name and the number of times they picked up each weapon.
;359:	for (i=0; i<MAX_CLIENTS; i++)
ADDRLP4 1028
CNSTI4 0
ASGNI4
LABELV $180
line 360
;360:	{
line 361
;361:		if (G_WeaponLogClientTouch[i])
ADDRLP4 1028
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
INDIRI4
CNSTI4 0
EQI4 $184
line 362
;362:		{	// Ignore any entity/clients we don't care about!
line 363
;363:			if ( g_entities[i].client ) 
CNSTI4 828
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $186
line 364
;364:			{
line 365
;365:				nameptr = g_entities[i].client->pers.netname;
ADDRLP4 1100
CNSTI4 828
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CNSTI4 1416
ADDP4
ASGNP4
line 366
;366:			} 
ADDRGP4 $187
JUMPV
LABELV $186
line 368
;367:			else 
;368:			{
line 369
;369:				nameptr = unknownname;
ADDRLP4 1100
ADDRLP4 1796
INDIRP4
ASGNP4
line 370
;370:			}
LABELV $187
line 371
;371:			trap_FS_Write(nameptr, strlen(nameptr), weaponfile);
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 2976
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 2976
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 373
;372:
;373:			for (j=0;j<WP_NUM_WEAPONS;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $190
line 374
;374:			{
line 375
;375:				Com_sprintf(string, sizeof(string), "\t%d", G_WeaponLogPickups[i][j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $194
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1028
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogPickups
ADDP4
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 376
;376:				trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 2980
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 2980
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 377
;377:			}
LABELV $191
line 373
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $190
line 379
;378:
;379:			Com_sprintf(string, sizeof(string), "\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $166
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 380
;380:			trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 2980
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 2980
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 381
;381:		}
LABELV $184
line 382
;382:	}
LABELV $181
line 359
ADDRLP4 1028
ADDRLP4 1028
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 1028
INDIRI4
CNSTI4 32
LTI4 $180
line 385
;383:
;384:	// Sum up the totals.
;385:	Com_sprintf(string, sizeof(string), "\n***TOTAL:");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $195
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 386
;386:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 2976
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 2976
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 388
;387:
;388:	for (j=0;j<WP_NUM_WEAPONS;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $196
line 389
;389:	{
line 390
;390:		Com_sprintf(string, sizeof(string), "\t%d", totalpickups[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $194
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1476
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 391
;391:		trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 2980
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 2980
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 392
;392:	}
LABELV $197
line 388
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $196
line 394
;393:
;394:	Com_sprintf(string, sizeof(string), "\n\n\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $200
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 395
;395:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 2980
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 2980
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 399
;396:
;397:	
;398:	// Weapon fires per character
;399:	Com_sprintf(string, sizeof(string), "Weapon Shots per Player:\n\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $201
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 400
;400:	trap_FS_Write( string, strlen( string ), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 2984
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 2984
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 402
;401:
;402:	Com_sprintf(string, sizeof(string), "Player");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $174
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 403
;403:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 2988
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 2988
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 405
;404:
;405:	for (j=0; j<WP_NUM_WEAPONS; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $202
line 406
;406:	{
line 407
;407:		Com_sprintf(string, sizeof(string), "\t%s", weaponNameFromIndex[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $179
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 weaponNameFromIndex
ADDP4
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 408
;408:		trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 2992
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 2992
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 409
;409:	}
LABELV $203
line 405
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $202
line 410
;410:	Com_sprintf(string, sizeof(string), "\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $166
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 411
;411:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 2992
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 2992
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 414
;412:
;413:	// Cycle through each player, give their name and the number of times they picked up each weapon.
;414:	for (i=0; i<MAX_CLIENTS; i++)
ADDRLP4 1028
CNSTI4 0
ASGNI4
LABELV $206
line 415
;415:	{
line 416
;416:		if (G_WeaponLogClientTouch[i])
ADDRLP4 1028
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
INDIRI4
CNSTI4 0
EQI4 $210
line 417
;417:		{	// Ignore any entity/clients we don't care about!
line 418
;418:			if ( g_entities[i].client ) 
CNSTI4 828
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $212
line 419
;419:			{
line 420
;420:				nameptr = g_entities[i].client->pers.netname;
ADDRLP4 1100
CNSTI4 828
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CNSTI4 1416
ADDP4
ASGNP4
line 421
;421:			} 
ADDRGP4 $213
JUMPV
LABELV $212
line 423
;422:			else 
;423:			{
line 424
;424:				nameptr = unknownname;
ADDRLP4 1100
ADDRLP4 1796
INDIRP4
ASGNP4
line 425
;425:			}
LABELV $213
line 426
;426:			trap_FS_Write(nameptr, strlen(nameptr), weaponfile);
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 2996
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 2996
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 428
;427:
;428:			for (j=0;j<WP_NUM_WEAPONS;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $216
line 429
;429:			{
line 430
;430:				Com_sprintf(string, sizeof(string), "\t%d", G_WeaponLogFired[i][j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $194
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1028
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogFired
ADDP4
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 431
;431:				trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3000
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3000
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 432
;432:			}
LABELV $217
line 428
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $216
line 434
;433:
;434:			Com_sprintf(string, sizeof(string), "\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $166
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 435
;435:			trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3000
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3000
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 436
;436:		}
LABELV $210
line 437
;437:	}
LABELV $207
line 414
ADDRLP4 1028
ADDRLP4 1028
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 1028
INDIRI4
CNSTI4 32
LTI4 $206
line 440
;438:
;439:	// Sum up the totals.
;440:	Com_sprintf(string, sizeof(string), "\n***TOTAL:");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $195
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 441
;441:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 2996
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 2996
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 443
;442:	
;443:	for (j=0;j<WP_NUM_WEAPONS;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $220
line 444
;444:	{
line 445
;445:		Com_sprintf(string, sizeof(string), "\t%d", totalshots[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $194
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1412
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 446
;446:		trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3000
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3000
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 447
;447:	}
LABELV $221
line 443
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $220
line 449
;448:
;449:	Com_sprintf(string, sizeof(string), "\n\n\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $200
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 450
;450:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3000
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3000
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 454
;451:
;452:
;453:	// Weapon time per character
;454:	Com_sprintf(string, sizeof(string), "Weapon Use Time per Player:\n\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $224
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 455
;455:	trap_FS_Write( string, strlen( string ), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3004
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3004
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 457
;456:
;457:	Com_sprintf(string, sizeof(string), "Player");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $174
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 458
;458:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3008
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3008
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 460
;459:
;460:	for (j=0; j<WP_NUM_WEAPONS; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $225
line 461
;461:	{
line 462
;462:		Com_sprintf(string, sizeof(string), "\t%s", weaponNameFromIndex[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $179
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 weaponNameFromIndex
ADDP4
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 463
;463:		trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3012
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3012
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 464
;464:	}
LABELV $226
line 460
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $225
line 465
;465:	Com_sprintf(string, sizeof(string), "\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $166
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 466
;466:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3012
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3012
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 469
;467:
;468:	// Cycle through each player, give their name and the number of times they picked up each weapon.
;469:	for (i=0; i<MAX_CLIENTS; i++)
ADDRLP4 1028
CNSTI4 0
ASGNI4
LABELV $229
line 470
;470:	{
line 471
;471:		if (G_WeaponLogClientTouch[i])
ADDRLP4 1028
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
INDIRI4
CNSTI4 0
EQI4 $233
line 472
;472:		{	// Ignore any entity/clients we don't care about!
line 473
;473:			if ( g_entities[i].client ) 
CNSTI4 828
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $235
line 474
;474:			{
line 475
;475:				nameptr = g_entities[i].client->pers.netname;
ADDRLP4 1100
CNSTI4 828
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CNSTI4 1416
ADDP4
ASGNP4
line 476
;476:			} 
ADDRGP4 $236
JUMPV
LABELV $235
line 478
;477:			else 
;478:			{
line 479
;479:				nameptr = unknownname;
ADDRLP4 1100
ADDRLP4 1796
INDIRP4
ASGNP4
line 480
;480:			}
LABELV $236
line 481
;481:			trap_FS_Write(nameptr, strlen(nameptr), weaponfile);
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 3016
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 3016
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 483
;482:
;483:			for (j=0;j<WP_NUM_WEAPONS;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $239
line 484
;484:			{
line 485
;485:				Com_sprintf(string, sizeof(string), "\t%d", G_WeaponLogTime[i][j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $194
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1028
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogTime
ADDP4
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 486
;486:				trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3020
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3020
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 487
;487:			}
LABELV $240
line 483
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $239
line 489
;488:
;489:			Com_sprintf(string, sizeof(string), "\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $166
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 490
;490:			trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3020
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3020
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 491
;491:		}
LABELV $233
line 492
;492:	}
LABELV $230
line 469
ADDRLP4 1028
ADDRLP4 1028
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 1028
INDIRI4
CNSTI4 32
LTI4 $229
line 495
;493:
;494:	// Sum up the totals.
;495:	Com_sprintf(string, sizeof(string), "\n***TOTAL:");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $195
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 496
;496:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3016
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3016
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 498
;497:	
;498:	for (j=0;j<WP_NUM_WEAPONS;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $243
line 499
;499:	{
line 500
;500:		Com_sprintf(string, sizeof(string), "\t%d", totaltime[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $194
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1540
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 501
;501:		trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3020
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3020
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 502
;502:	}
LABELV $244
line 498
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $243
line 504
;503:
;504:	Com_sprintf(string, sizeof(string), "\n\n\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $200
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 505
;505:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3020
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3020
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 510
;506:
;507:
;508:	
;509:	// Weapon deaths per character
;510:	Com_sprintf(string, sizeof(string), "Weapon Deaths per Player:\n\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $247
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 511
;511:	trap_FS_Write( string, strlen( string ), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3024
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3024
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 513
;512:
;513:	Com_sprintf(string, sizeof(string), "Player");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $174
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 514
;514:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3028
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3028
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 516
;515:
;516:	for (j=0; j<WP_NUM_WEAPONS; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $248
line 517
;517:	{
line 518
;518:		Com_sprintf(string, sizeof(string), "\t%s", weaponNameFromIndex[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $179
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 weaponNameFromIndex
ADDP4
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 519
;519:		trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3032
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3032
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 520
;520:	}
LABELV $249
line 516
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $248
line 521
;521:	Com_sprintf(string, sizeof(string), "\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $166
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 522
;522:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3032
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3032
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 525
;523:
;524:	// Cycle through each player, give their name and the number of times they picked up each weapon.
;525:	for (i=0; i<MAX_CLIENTS; i++)
ADDRLP4 1028
CNSTI4 0
ASGNI4
LABELV $252
line 526
;526:	{
line 527
;527:		if (G_WeaponLogClientTouch[i])
ADDRLP4 1028
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
INDIRI4
CNSTI4 0
EQI4 $256
line 528
;528:		{	// Ignore any entity/clients we don't care about!
line 529
;529:			if ( g_entities[i].client ) 
CNSTI4 828
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $258
line 530
;530:			{
line 531
;531:				nameptr = g_entities[i].client->pers.netname;
ADDRLP4 1100
CNSTI4 828
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CNSTI4 1416
ADDP4
ASGNP4
line 532
;532:			} 
ADDRGP4 $259
JUMPV
LABELV $258
line 534
;533:			else 
;534:			{
line 535
;535:				nameptr = unknownname;
ADDRLP4 1100
ADDRLP4 1796
INDIRP4
ASGNP4
line 536
;536:			}
LABELV $259
line 537
;537:			trap_FS_Write(nameptr, strlen(nameptr), weaponfile);
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 3036
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 3036
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 539
;538:
;539:			for (j=0;j<WP_NUM_WEAPONS;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $262
line 540
;540:			{
line 541
;541:				Com_sprintf(string, sizeof(string), "\t%d", G_WeaponLogDeaths[i][j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $194
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1028
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogDeaths
ADDP4
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 542
;542:				trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3040
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3040
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 543
;543:			}
LABELV $263
line 539
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $262
line 545
;544:
;545:			Com_sprintf(string, sizeof(string), "\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $166
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 546
;546:			trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3040
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3040
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 547
;547:		}
LABELV $256
line 548
;548:	}
LABELV $253
line 525
ADDRLP4 1028
ADDRLP4 1028
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 1028
INDIRI4
CNSTI4 32
LTI4 $252
line 551
;549:
;550:	// Sum up the totals.
;551:	Com_sprintf(string, sizeof(string), "\n***TOTAL:");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $195
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 552
;552:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3036
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3036
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 554
;553:	
;554:	for (j=0;j<WP_NUM_WEAPONS;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $266
line 555
;555:	{
line 556
;556:		Com_sprintf(string, sizeof(string), "\t%d", totaldeaths[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $194
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1604
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 557
;557:		trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3040
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3040
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 558
;558:	}
LABELV $267
line 554
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $266
line 560
;559:
;560:	Com_sprintf(string, sizeof(string), "\n\n\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $200
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 561
;561:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3040
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3040
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 568
;562:
;563:
;564:
;565:	
;566:	// Weapon damage per character
;567:
;568:	Com_sprintf(string, sizeof(string), "Weapon Damage per Player:\n\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $270
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 569
;569:	trap_FS_Write( string, strlen( string ), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3044
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3044
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 571
;570:
;571:	Com_sprintf(string, sizeof(string), "Player");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $174
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 572
;572:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3048
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3048
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 574
;573:
;574:	for (j=0; j<WP_NUM_WEAPONS; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $271
line 575
;575:	{
line 576
;576:		Com_sprintf(string, sizeof(string), "\t%s", weaponNameFromIndex[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $179
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 weaponNameFromIndex
ADDP4
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 577
;577:		trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3052
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3052
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 578
;578:	}
LABELV $272
line 574
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $271
line 579
;579:	Com_sprintf(string, sizeof(string), "\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $166
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 580
;580:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3052
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3052
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 583
;581:
;582:	// Cycle through each player, give their name and the number of times they picked up each weapon.
;583:	for (i=0; i<MAX_CLIENTS; i++)
ADDRLP4 1028
CNSTI4 0
ASGNI4
LABELV $275
line 584
;584:	{
line 585
;585:		if (G_WeaponLogClientTouch[i])
ADDRLP4 1028
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
INDIRI4
CNSTI4 0
EQI4 $279
line 586
;586:		{	// Ignore any entity/clients we don't care about!
line 589
;587:
;588:			// We must grab the totals from the damage types for the player and map them to the weapons.
;589:			memset(percharacter, 0, sizeof(percharacter));
ADDRLP4 1036
ARGP4
CNSTI4 0
ARGI4
CNSTI4 64
ARGI4
ADDRGP4 memset
CALLP4
pop
line 590
;590:			for (j=0; j<MOD_MAX; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $281
line 591
;591:			{
line 592
;592:				if (j <= MOD_SENTRY)
ADDRLP4 0
INDIRI4
CNSTI4 28
GTI4 $285
line 593
;593:				{
line 594
;594:					curwp = weaponFromMOD[j];
ADDRLP4 1104
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 weaponFromMOD
ADDP4
INDIRI4
ASGNI4
line 595
;595:					percharacter[curwp] += G_WeaponLogDamage[i][j];
ADDRLP4 3056
CNSTI4 2
ASGNI4
ADDRLP4 3060
ADDRLP4 1104
INDIRI4
ADDRLP4 3056
INDIRI4
LSHI4
ADDRLP4 1036
ADDP4
ASGNP4
ADDRLP4 3060
INDIRP4
ADDRLP4 3060
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
ADDRLP4 3056
INDIRI4
LSHI4
CNSTI4 152
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 G_WeaponLogDamage
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 596
;596:				}
LABELV $285
line 597
;597:			}
LABELV $282
line 590
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 38
LTI4 $281
line 599
;598:
;599:			if ( g_entities[i].client ) 
CNSTI4 828
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $287
line 600
;600:			{
line 601
;601:				nameptr = g_entities[i].client->pers.netname;
ADDRLP4 1100
CNSTI4 828
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CNSTI4 1416
ADDP4
ASGNP4
line 602
;602:			} 
ADDRGP4 $288
JUMPV
LABELV $287
line 604
;603:			else 
;604:			{
line 605
;605:				nameptr = unknownname;
ADDRLP4 1100
ADDRLP4 1796
INDIRP4
ASGNP4
line 606
;606:			}
LABELV $288
line 607
;607:			trap_FS_Write(nameptr, strlen(nameptr), weaponfile);
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 3056
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 3056
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 609
;608:
;609:			for (j=0;j<WP_NUM_WEAPONS;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $291
line 610
;610:			{
line 611
;611:				Com_sprintf(string, sizeof(string), "\t%d", percharacter[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $194
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1036
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 612
;612:				trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3060
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3060
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 613
;613:			}
LABELV $292
line 609
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $291
line 615
;614:
;615:			Com_sprintf(string, sizeof(string), "\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $166
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 616
;616:			trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3060
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3060
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 617
;617:		}
LABELV $279
line 618
;618:	}
LABELV $276
line 583
ADDRLP4 1028
ADDRLP4 1028
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 1028
INDIRI4
CNSTI4 32
LTI4 $275
line 621
;619:
;620:	// Sum up the totals.
;621:	Com_sprintf(string, sizeof(string), "\n***TOTAL:");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $195
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 622
;622:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3056
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3056
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 624
;623:	
;624:	for (j=0;j<WP_NUM_WEAPONS;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $295
line 625
;625:	{
line 626
;626:		Com_sprintf(string, sizeof(string), "\t%d", totaldamage[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $194
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1668
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 627
;627:		trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3060
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3060
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 628
;628:	}
LABELV $296
line 624
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $295
line 630
;629:
;630:	Com_sprintf(string, sizeof(string), "\n\n\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $200
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 631
;631:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3060
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3060
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 637
;632:
;633:
;634:	
;635:	// Weapon kills per character
;636:
;637:	Com_sprintf(string, sizeof(string), "Weapon Kills per Player:\n\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $299
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 638
;638:	trap_FS_Write( string, strlen( string ), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3064
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3064
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 640
;639:
;640:	Com_sprintf(string, sizeof(string), "Player");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $174
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 641
;641:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3068
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3068
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 643
;642:
;643:	for (j=0; j<WP_NUM_WEAPONS; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $300
line 644
;644:	{
line 645
;645:		Com_sprintf(string, sizeof(string), "\t%s", weaponNameFromIndex[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $179
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 weaponNameFromIndex
ADDP4
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 646
;646:		trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3072
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3072
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 647
;647:	}
LABELV $301
line 643
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $300
line 648
;648:	Com_sprintf(string, sizeof(string), "\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $166
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 649
;649:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3072
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3072
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 652
;650:
;651:	// Cycle through each player, give their name and the number of times they picked up each weapon.
;652:	for (i=0; i<MAX_CLIENTS; i++)
ADDRLP4 1028
CNSTI4 0
ASGNI4
LABELV $304
line 653
;653:	{
line 654
;654:		if (G_WeaponLogClientTouch[i])
ADDRLP4 1028
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
INDIRI4
CNSTI4 0
EQI4 $308
line 655
;655:		{	// Ignore any entity/clients we don't care about!
line 658
;656:
;657:			// We must grab the totals from the damage types for the player and map them to the weapons.
;658:			memset(percharacter, 0, sizeof(percharacter));
ADDRLP4 1036
ARGP4
CNSTI4 0
ARGI4
CNSTI4 64
ARGI4
ADDRGP4 memset
CALLP4
pop
line 659
;659:			for (j=0; j<MOD_MAX; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $310
line 660
;660:			{
line 661
;661:				if (j <= MOD_SENTRY)
ADDRLP4 0
INDIRI4
CNSTI4 28
GTI4 $314
line 662
;662:				{
line 663
;663:					curwp = weaponFromMOD[j];
ADDRLP4 1104
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 weaponFromMOD
ADDP4
INDIRI4
ASGNI4
line 664
;664:					percharacter[curwp] += G_WeaponLogKills[i][j];
ADDRLP4 3076
CNSTI4 2
ASGNI4
ADDRLP4 3080
ADDRLP4 1104
INDIRI4
ADDRLP4 3076
INDIRI4
LSHI4
ADDRLP4 1036
ADDP4
ASGNP4
ADDRLP4 3080
INDIRP4
ADDRLP4 3080
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
ADDRLP4 3076
INDIRI4
LSHI4
CNSTI4 152
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 665
;665:				}
LABELV $314
line 666
;666:			}
LABELV $311
line 659
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 38
LTI4 $310
line 668
;667:
;668:			if ( g_entities[i].client ) 
CNSTI4 828
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $316
line 669
;669:			{
line 670
;670:				nameptr = g_entities[i].client->pers.netname;
ADDRLP4 1100
CNSTI4 828
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CNSTI4 1416
ADDP4
ASGNP4
line 671
;671:			} 
ADDRGP4 $317
JUMPV
LABELV $316
line 673
;672:			else 
;673:			{
line 674
;674:				nameptr = unknownname;
ADDRLP4 1100
ADDRLP4 1796
INDIRP4
ASGNP4
line 675
;675:			}
LABELV $317
line 676
;676:			trap_FS_Write(nameptr, strlen(nameptr), weaponfile);
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 3076
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 3076
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 678
;677:
;678:			for (j=0;j<WP_NUM_WEAPONS;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $320
line 679
;679:			{
line 680
;680:				Com_sprintf(string, sizeof(string), "\t%d", percharacter[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $194
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1036
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 681
;681:				trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3080
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3080
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 682
;682:			}
LABELV $321
line 678
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $320
line 684
;683:
;684:			Com_sprintf(string, sizeof(string), "\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $166
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 685
;685:			trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3080
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3080
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 686
;686:		}
LABELV $308
line 687
;687:	}
LABELV $305
line 652
ADDRLP4 1028
ADDRLP4 1028
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 1028
INDIRI4
CNSTI4 32
LTI4 $304
line 690
;688:
;689:	// Sum up the totals.
;690:	Com_sprintf(string, sizeof(string), "\n***TOTAL:");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $195
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 691
;691:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3076
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3076
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 693
;692:	
;693:	for (j=0;j<WP_NUM_WEAPONS;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $324
line 694
;694:	{
line 695
;695:		Com_sprintf(string, sizeof(string), "\t%d", totalkills[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $194
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1732
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 696
;696:		trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3080
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3080
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 697
;697:	}
LABELV $325
line 693
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $324
line 699
;698:
;699:	Com_sprintf(string, sizeof(string), "\n\n\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $200
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 700
;700:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3080
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3080
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 705
;701:
;702:
;703:	
;704:	// Damage type damage per character
;705:	Com_sprintf(string, sizeof(string), "Typed Damage per Player:\n\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $328
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 706
;706:	trap_FS_Write( string, strlen( string ), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3084
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3084
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 708
;707:
;708:	Com_sprintf(string, sizeof(string), "Player");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $174
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 709
;709:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3088
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3088
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 711
;710:
;711:	for (j=0; j<MOD_MAX; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $329
line 712
;712:	{
line 713
;713:		Com_sprintf(string, sizeof(string), "\t%s", modNames[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $179
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 modNames
ADDP4
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 714
;714:		trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3092
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3092
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 715
;715:	}
LABELV $330
line 711
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 38
LTI4 $329
line 716
;716:	Com_sprintf(string, sizeof(string), "\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $166
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 717
;717:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3092
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3092
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 720
;718:
;719:	// Cycle through each player, give their name and the number of times they picked up each weapon.
;720:	for (i=0; i<MAX_CLIENTS; i++)
ADDRLP4 1028
CNSTI4 0
ASGNI4
LABELV $333
line 721
;721:	{
line 722
;722:		if (G_WeaponLogClientTouch[i])
ADDRLP4 1028
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
INDIRI4
CNSTI4 0
EQI4 $337
line 723
;723:		{	// Ignore any entity/clients we don't care about!
line 724
;724:			if ( g_entities[i].client ) 
CNSTI4 828
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $339
line 725
;725:			{
line 726
;726:				nameptr = g_entities[i].client->pers.netname;
ADDRLP4 1100
CNSTI4 828
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CNSTI4 1416
ADDP4
ASGNP4
line 727
;727:			} 
ADDRGP4 $340
JUMPV
LABELV $339
line 729
;728:			else 
;729:			{
line 730
;730:				nameptr = unknownname;
ADDRLP4 1100
ADDRLP4 1796
INDIRP4
ASGNP4
line 731
;731:			}
LABELV $340
line 732
;732:			trap_FS_Write(nameptr, strlen(nameptr), weaponfile);
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 3096
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 3096
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 734
;733:
;734:			for (j=0;j<MOD_MAX;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $343
line 735
;735:			{
line 736
;736:				Com_sprintf(string, sizeof(string), "\t%d", G_WeaponLogDamage[i][j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $194
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 152
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 G_WeaponLogDamage
ADDP4
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 737
;737:				trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3100
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3100
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 738
;738:			}
LABELV $344
line 734
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 38
LTI4 $343
line 740
;739:
;740:			Com_sprintf(string, sizeof(string), "\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $166
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 741
;741:			trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3100
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3100
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 742
;742:		}
LABELV $337
line 743
;743:	}
LABELV $334
line 720
ADDRLP4 1028
ADDRLP4 1028
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 1028
INDIRI4
CNSTI4 32
LTI4 $333
line 746
;744:
;745:	// Sum up the totals.
;746:	Com_sprintf(string, sizeof(string), "\n***TOTAL:");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $195
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 747
;747:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3096
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3096
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 749
;748:	
;749:	for (j=0;j<MOD_MAX;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $347
line 750
;750:	{
line 751
;751:		Com_sprintf(string, sizeof(string), "\t%d", totaldamageMOD[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $194
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1108
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 752
;752:		trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3100
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3100
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 753
;753:	}
LABELV $348
line 749
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 38
LTI4 $347
line 755
;754:
;755:	Com_sprintf(string, sizeof(string), "\n\n\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $200
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 756
;756:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3100
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3100
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 761
;757:
;758:
;759:	
;760:	// Damage type kills per character
;761:	Com_sprintf(string, sizeof(string), "Damage-Typed Kills per Player:\n\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $351
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 762
;762:	trap_FS_Write( string, strlen( string ), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3104
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3104
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 764
;763:
;764:	Com_sprintf(string, sizeof(string), "Player");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $174
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 765
;765:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3108
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3108
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 767
;766:
;767:	for (j=0; j<MOD_MAX; j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $352
line 768
;768:	{
line 769
;769:		Com_sprintf(string, sizeof(string), "\t%s", modNames[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $179
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 modNames
ADDP4
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 770
;770:		trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3112
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3112
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 771
;771:	}
LABELV $353
line 767
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 38
LTI4 $352
line 772
;772:	Com_sprintf(string, sizeof(string), "\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $166
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 773
;773:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3112
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3112
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 776
;774:
;775:	// Cycle through each player, give their name and the number of times they picked up each weapon.
;776:	for (i=0; i<MAX_CLIENTS; i++)
ADDRLP4 1028
CNSTI4 0
ASGNI4
LABELV $356
line 777
;777:	{
line 778
;778:		if (G_WeaponLogClientTouch[i])
ADDRLP4 1028
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
INDIRI4
CNSTI4 0
EQI4 $360
line 779
;779:		{	// Ignore any entity/clients we don't care about!
line 780
;780:			if ( g_entities[i].client ) 
CNSTI4 828
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $362
line 781
;781:			{
line 782
;782:				nameptr = g_entities[i].client->pers.netname;
ADDRLP4 1100
CNSTI4 828
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CNSTI4 1416
ADDP4
ASGNP4
line 783
;783:			} 
ADDRGP4 $363
JUMPV
LABELV $362
line 785
;784:			else 
;785:			{
line 786
;786:				nameptr = unknownname;
ADDRLP4 1100
ADDRLP4 1796
INDIRP4
ASGNP4
line 787
;787:			}
LABELV $363
line 788
;788:			trap_FS_Write(nameptr, strlen(nameptr), weaponfile);
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 3116
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1100
INDIRP4
ARGP4
ADDRLP4 3116
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 790
;789:
;790:			for (j=0;j<MOD_MAX;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $366
line 791
;791:			{
line 792
;792:				Com_sprintf(string, sizeof(string), "\t%d", G_WeaponLogKills[i][j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $194
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 152
ADDRLP4 1028
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills
ADDP4
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 793
;793:				trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3120
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3120
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 794
;794:			}
LABELV $367
line 790
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 38
LTI4 $366
line 796
;795:
;796:			Com_sprintf(string, sizeof(string), "\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $166
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 797
;797:			trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3120
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3120
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 798
;798:		}
LABELV $360
line 799
;799:	}
LABELV $357
line 776
ADDRLP4 1028
ADDRLP4 1028
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 1028
INDIRI4
CNSTI4 32
LTI4 $356
line 802
;800:
;801:	// Sum up the totals.
;802:	Com_sprintf(string, sizeof(string), "\n***TOTAL:");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $195
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 803
;803:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3116
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3116
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 805
;804:	
;805:	for (j=0;j<MOD_MAX;j++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $370
line 806
;806:	{
line 807
;807:		Com_sprintf(string, sizeof(string), "\t%d", totalkillsMOD[j]);
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $194
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1260
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 808
;808:		trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3120
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3120
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 809
;809:	}
LABELV $371
line 805
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 38
LTI4 $370
line 811
;810:
;811:	Com_sprintf(string, sizeof(string), "\n\n\n");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $200
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 812
;812:	trap_FS_Write(string, strlen(string), weaponfile);
ADDRLP4 4
ARGP4
ADDRLP4 3120
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 3120
INDIRI4
ARGI4
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_Write
CALLV
pop
line 815
;813:
;814:
;815:	trap_FS_FCloseFile(weaponfile);
ADDRLP4 1032
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 819
;816:
;817:
;818:#endif //LOGGING_WEAPONS
;819:}
LABELV $120
endproc G_LogWeaponOutput 3124 20
export CalculateEfficiency
proc CalculateEfficiency 32 0
line 823
;820:
;821:// did this player earn the efficiency award?
;822:qboolean CalculateEfficiency(gentity_t *ent, int *efficiency)
;823:{
line 825
;824:#ifdef LOGGING_WEAPONS
;825:	float		fAccuracyRatio = 0, fBestRatio = 0;
ADDRLP4 8
CNSTF4 0
ASGNF4
ADDRLP4 20
CNSTF4 0
ASGNF4
line 826
;826:	int			i = 0, nShotsFired = 0, nShotsHit = 0, nBestPlayer = -1, tempEff = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRLP4 16
CNSTI4 0
ASGNI4
ADDRLP4 24
CNSTI4 -1
ASGNI4
ADDRLP4 28
CNSTI4 0
ASGNI4
line 827
;827:	gentity_t	*player = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
line 830
;828:
;829:
;830:	for (i = 0; i < g_maxclients.integer; i++)
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $378
JUMPV
LABELV $375
line 831
;831:	{
line 832
;832:		player = g_entities + i;
ADDRLP4 0
CNSTI4 828
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 833
;833:		if (!player->inuse)
ADDRLP4 0
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
NEI4 $380
line 834
;834:			continue;
ADDRGP4 $376
JUMPV
LABELV $380
line 835
;835:		nShotsFired = player->client->accuracy_shots; //player->client->ps.persistant[PERS_ACCURACY_SHOTS];
ADDRLP4 12
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1652
ADDP4
INDIRI4
ASGNI4
line 836
;836:		nShotsHit = player->client->accuracy_hits; //player->client->ps.persistant[PERS_ACCURACY_HITS];
ADDRLP4 16
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1656
ADDP4
INDIRI4
ASGNI4
line 837
;837:		fAccuracyRatio = ( ((float)nShotsHit)/((float)nShotsFired) );
ADDRLP4 8
ADDRLP4 16
INDIRI4
CVIF4 4
ADDRLP4 12
INDIRI4
CVIF4 4
DIVF4
ASGNF4
line 838
;838:		if (fAccuracyRatio > fBestRatio)
ADDRLP4 8
INDIRF4
ADDRLP4 20
INDIRF4
LEF4 $382
line 839
;839:		{
line 840
;840:			fBestRatio = fAccuracyRatio;
ADDRLP4 20
ADDRLP4 8
INDIRF4
ASGNF4
line 841
;841:			nBestPlayer = i;
ADDRLP4 24
ADDRLP4 4
INDIRI4
ASGNI4
line 842
;842:		}
LABELV $382
line 843
;843:	}
LABELV $376
line 830
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $378
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $375
line 844
;844:	if (-1 == nBestPlayer)
ADDRLP4 24
INDIRI4
CNSTI4 -1
NEI4 $384
line 845
;845:	{
line 847
;846:		// huh?
;847:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $374
JUMPV
LABELV $384
line 849
;848:	}
;849:	if (nBestPlayer == ent->s.number)
ADDRLP4 24
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $386
line 850
;850:	{
line 851
;851:		tempEff = (int)(100*fBestRatio);
ADDRLP4 28
CNSTF4 1120403456
ADDRLP4 20
INDIRF4
MULF4
CVFI4 4
ASGNI4
line 852
;852:		if (tempEff > 50)
ADDRLP4 28
INDIRI4
CNSTI4 50
LEI4 $388
line 853
;853:		{
line 854
;854:			*efficiency = tempEff;
ADDRFP4 4
INDIRP4
ADDRLP4 28
INDIRI4
ASGNI4
line 855
;855:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $374
JUMPV
LABELV $388
line 857
;856:		}
;857:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $374
JUMPV
LABELV $386
line 860
;858:	}
;859:#endif // LOGGING_WEAPONS
;860:	return qfalse;
CNSTI4 0
RETI4
LABELV $374
endproc CalculateEfficiency 32 0
export CalculateSharpshooter
proc CalculateSharpshooter 24 0
line 865
;861:}
;862:
;863:// did this player earn the sharpshooter award?
;864:qboolean CalculateSharpshooter(gentity_t *ent, int *frags)
;865:{
line 867
;866:#ifdef LOGGING_WEAPONS
;867:	int			i = 0, nBestPlayer = -1, nKills = 0, nMostKills = 0,
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRLP4 16
CNSTI4 -1
ASGNI4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 12
CNSTI4 0
ASGNI4
line 868
;868:				playTime = (level.time - ent->client->pers.enterTime)/60000;
ADDRLP4 20
ADDRGP4 level+32
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1456
ADDP4
INDIRI4
SUBI4
CNSTI4 60000
DIVI4
ASGNI4
line 869
;869:	gentity_t	*player = NULL;
ADDRLP4 8
CNSTP4 0
ASGNP4
line 872
;870:
;871:	// if this guy didn't get one kill per minute, reject him right now
;872:	if ( ((float)(G_WeaponLogKills[ent-g_entities][MOD_DISRUPTOR_SNIPER]))/((float)(playTime)) < 1.0 )
CNSTI4 152
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRGP4 g_entities
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 828
DIVI4
MULI4
ADDRGP4 G_WeaponLogKills+36
ADDP4
INDIRI4
CVIF4 4
ADDRLP4 20
INDIRI4
CVIF4 4
DIVF4
CNSTF4 1065353216
GEF4 $392
line 873
;873:	{
line 874
;874:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $390
JUMPV
LABELV $392
line 877
;875:	}
;876:
;877:	for (i = 0; i < g_maxclients.integer; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $398
JUMPV
LABELV $395
line 878
;878:	{
line 879
;879:		nKills = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 880
;880:		player = g_entities + i;
ADDRLP4 8
CNSTI4 828
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 881
;881:		if (!player->inuse)
ADDRLP4 8
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
NEI4 $400
line 882
;882:			continue;
ADDRGP4 $396
JUMPV
LABELV $400
line 883
;883:		nKills = G_WeaponLogKills[i][MOD_DISRUPTOR_SNIPER];
ADDRLP4 4
CNSTI4 152
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills+36
ADDP4
INDIRI4
ASGNI4
line 884
;884:		if (nKills > nMostKills)
ADDRLP4 4
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $403
line 885
;885:		{
line 886
;886:			nMostKills = nKills;
ADDRLP4 12
ADDRLP4 4
INDIRI4
ASGNI4
line 887
;887:			nBestPlayer = i;
ADDRLP4 16
ADDRLP4 0
INDIRI4
ASGNI4
line 888
;888:		}
LABELV $403
line 889
;889:	}
LABELV $396
line 877
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $398
ADDRLP4 0
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $395
line 890
;890:	if (-1 == nBestPlayer)
ADDRLP4 16
INDIRI4
CNSTI4 -1
NEI4 $405
line 891
;891:	{
line 892
;892:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $390
JUMPV
LABELV $405
line 894
;893:	}
;894:	if (nBestPlayer == ent->s.number)
ADDRLP4 16
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $407
line 895
;895:	{
line 896
;896:		*frags = nMostKills;
ADDRFP4 4
INDIRP4
ADDRLP4 12
INDIRI4
ASGNI4
line 897
;897:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $390
JUMPV
LABELV $407
line 900
;898:	}
;899:#endif // LOGGING_WEAPONS
;900:	return qfalse;
CNSTI4 0
RETI4
LABELV $390
endproc CalculateSharpshooter 24 0
export CalculateUntouchable
proc CalculateUntouchable 8 0
line 905
;901:}
;902:
;903:// did this player earn the untouchable award?
;904:qboolean CalculateUntouchable(gentity_t *ent)
;905:{
line 908
;906:#ifdef LOGGING_WEAPONS
;907:	int			playTime;
;908:	playTime = (level.time - ent->client->pers.enterTime)/60000;
ADDRLP4 0
ADDRGP4 level+32
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1456
ADDP4
INDIRI4
SUBI4
CNSTI4 60000
DIVI4
ASGNI4
line 910
;909:
;910:	if ( g_gametype.integer == GT_JEDIMASTER && ent->client->ps.isJediMaster )
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 2
NEI4 $411
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 604
ADDP4
INDIRI4
CNSTI4 0
EQI4 $411
line 911
;911:	{//Jedi Master (was Borg queen) can only be killed once anyway
line 912
;912:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $409
JUMPV
LABELV $411
line 915
;913:	}
;914:	//------------------------------------------------------ MUST HAVE ACHIEVED 2 KILLS PER MINUTE
;915:	if ( ((float)ent->client->ps.persistant[PERS_SCORE])/((float)(playTime)) < 2.0  || playTime==0)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 280
ADDP4
INDIRI4
CVIF4 4
ADDRLP4 0
INDIRI4
CVIF4 4
DIVF4
CNSTF4 1073741824
LTF4 $416
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $414
LABELV $416
line 916
;916:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $409
JUMPV
LABELV $414
line 921
;917:	//------------------------------------------------------ MUST HAVE ACHIEVED 2 KILLS PER MINUTE
;918:
;919:
;920:	// if this guy was never killed...  Award Away!!!
;921:	if (ent->client->ps.persistant[PERS_KILLED]==0)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 312
ADDP4
INDIRI4
CNSTI4 0
NEI4 $417
line 922
;922:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $409
JUMPV
LABELV $417
line 925
;923:
;924:#endif // LOGGING_WEAPONS
;925:	return qfalse;
CNSTI4 0
RETI4
LABELV $409
endproc CalculateUntouchable 8 0
export CalculateLogistics
proc CalculateLogistics 36 0
line 930
;926:}
;927:
;928:// did this player earn the logistics award?
;929:qboolean CalculateLogistics(gentity_t *ent, int *stuffUsed)
;930:{
line 932
;931:#ifdef LOGGING_WEAPONS
;932:	int			i = 0, j = 0, nBestPlayer = -1, nStuffUsed = 0, nMostStuffUsed = 0,
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRLP4 28
CNSTI4 -1
ASGNI4
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRLP4 24
CNSTI4 0
ASGNI4
line 933
;933:				nDifferent = 0, nMostDifferent = 0;
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRLP4 20
CNSTI4 0
ASGNI4
line 934
;934:	gentity_t	*player = NULL;
ADDRLP4 16
CNSTP4 0
ASGNP4
line 936
;935:
;936:	for (i = 0; i < g_maxclients.integer; i++)
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $423
JUMPV
LABELV $420
line 937
;937:	{
line 938
;938:		nStuffUsed = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 939
;939:		nDifferent = 0;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 940
;940:		player = g_entities + i;
ADDRLP4 16
CNSTI4 828
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 941
;941:		if (!player->inuse)
ADDRLP4 16
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
NEI4 $425
line 942
;942:			continue;
ADDRGP4 $421
JUMPV
LABELV $425
line 943
;943:		for (j = HI_NONE+1; j < HI_NUM_HOLDABLE; j++)
ADDRLP4 0
CNSTI4 1
ASGNI4
LABELV $427
line 944
;944:		{
line 945
;945:			if (G_WeaponLogPowerups[i][j])
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 28
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 G_WeaponLogPowerups
ADDP4
ADDP4
INDIRI4
CNSTI4 0
EQI4 $431
line 946
;946:			{
line 947
;947:				nDifferent++;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 948
;948:			}
LABELV $431
line 949
;949:			nStuffUsed += G_WeaponLogPowerups[i][j];
ADDRLP4 8
ADDRLP4 8
INDIRI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 28
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 G_WeaponLogPowerups
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 950
;950:		}
LABELV $428
line 943
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 7
LTI4 $427
line 951
;951:		for (j = PW_NONE+1; j < PW_NUM_POWERUPS; j++)
ADDRLP4 0
CNSTI4 1
ASGNI4
LABELV $433
line 952
;952:		{
line 953
;953:			if (G_WeaponLogItems[i][j])
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogItems
ADDP4
ADDP4
INDIRI4
CNSTI4 0
EQI4 $437
line 954
;954:			{
line 955
;955:				nDifferent++;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 956
;956:			}
LABELV $437
line 957
;957:			nStuffUsed += G_WeaponLogItems[i][j];
ADDRLP4 8
ADDRLP4 8
INDIRI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogItems
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 958
;958:		}
LABELV $434
line 951
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $433
line 959
;959:		if ( (nDifferent >= 4) && (nDifferent >= nMostDifferent) )
ADDRLP4 12
INDIRI4
CNSTI4 4
LTI4 $439
ADDRLP4 12
INDIRI4
ADDRLP4 20
INDIRI4
LTI4 $439
line 960
;960:		{
line 961
;961:			if (nStuffUsed > nMostStuffUsed)
ADDRLP4 8
INDIRI4
ADDRLP4 24
INDIRI4
LEI4 $441
line 962
;962:			{
line 963
;963:				nMostDifferent = nDifferent;
ADDRLP4 20
ADDRLP4 12
INDIRI4
ASGNI4
line 964
;964:				nMostStuffUsed = nStuffUsed;
ADDRLP4 24
ADDRLP4 8
INDIRI4
ASGNI4
line 965
;965:				nBestPlayer = i;
ADDRLP4 28
ADDRLP4 4
INDIRI4
ASGNI4
line 966
;966:			}
LABELV $441
line 967
;967:		}
LABELV $439
line 968
;968:	}
LABELV $421
line 936
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $423
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $420
line 969
;969:	if (-1 == nBestPlayer)
ADDRLP4 28
INDIRI4
CNSTI4 -1
NEI4 $443
line 970
;970:	{
line 971
;971:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $419
JUMPV
LABELV $443
line 973
;972:	}
;973:	if (nBestPlayer == ent->s.number)
ADDRLP4 28
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $445
line 974
;974:	{
line 975
;975:		*stuffUsed = nMostDifferent;
ADDRFP4 4
INDIRP4
ADDRLP4 20
INDIRI4
ASGNI4
line 976
;976:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $419
JUMPV
LABELV $445
line 979
;977:	}
;978:#endif // LOGGING_WEAPONS
;979:	return qfalse;
CNSTI4 0
RETI4
LABELV $419
endproc CalculateLogistics 36 0
export CalculateTactician
proc CalculateTactician 176 0
line 987
;980:}
;981:
;982:
;983:
;984:
;985:// did this player earn the tactician award?
;986:qboolean CalculateTactician(gentity_t *ent, int *kills)
;987:{
line 989
;988:#ifdef LOGGING_WEAPONS
;989:	int			i = 0, nBestPlayer = -1, nKills = 0, nMostKills = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 152
CNSTI4 -1
ASGNI4
ADDRLP4 140
CNSTI4 0
ASGNI4
ADDRLP4 148
CNSTI4 0
ASGNI4
line 990
;990:	int			person = 0, weapon = 0;
ADDRLP4 72
CNSTI4 0
ASGNI4
ADDRLP4 0
CNSTI4 0
ASGNI4
line 991
;991:	gentity_t	*player = NULL;
ADDRLP4 144
CNSTP4 0
ASGNP4
line 994
;992:	int			wasPickedUpBySomeone[WP_NUM_WEAPONS];
;993:	int			killsWithWeapon[WP_NUM_WEAPONS];
;994:	int			playTime = (level.time - ent->client->pers.enterTime)/60000;
ADDRLP4 156
ADDRGP4 level+32
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1456
ADDP4
INDIRI4
SUBI4
CNSTI4 60000
DIVI4
ASGNI4
line 996
;995:
;996:	if ( HasSetSaberOnly() )
ADDRLP4 160
ADDRGP4 HasSetSaberOnly
CALLI4
ASGNI4
ADDRLP4 160
INDIRI4
CNSTI4 0
EQI4 $449
line 997
;997:	{//duh, only 1 weapon
line 998
;998:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $447
JUMPV
LABELV $449
line 1000
;999:	}
;1000:	if ( g_gametype.integer == GT_JEDIMASTER && ent->client->ps.isJediMaster )
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 2
NEI4 $451
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 604
ADDP4
INDIRI4
CNSTI4 0
EQI4 $451
line 1001
;1001:	{//Jedi Master (was Borg queen) has only 1 weapon
line 1002
;1002:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $447
JUMPV
LABELV $451
line 1005
;1003:	}
;1004:	//------------------------------------------------------ MUST HAVE ACHIEVED 2 KILLS PER MINUTE
;1005:	if (playTime<0.3)
ADDRLP4 156
INDIRI4
CVIF4 4
CNSTF4 1050253722
GEF4 $454
line 1006
;1006:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $447
JUMPV
LABELV $454
line 1008
;1007:
;1008:	if ( ((float)ent->client->ps.persistant[PERS_SCORE])/((float)(playTime)) < 2.0 )
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 280
ADDP4
INDIRI4
CVIF4 4
ADDRLP4 156
INDIRI4
CVIF4 4
DIVF4
CNSTF4 1073741824
GEF4 $456
line 1009
;1009:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $447
JUMPV
LABELV $456
line 1016
;1010:	//------------------------------------------------------ MUST HAVE ACHIEVED 2 KILLS PER MINUTE
;1011:
;1012:
;1013:
;1014:
;1015:	//------------------------------------------------------ FOR EVERY WEAPON, ADD UP TOTAL PICKUPS
;1016:	for (weapon = 0; weapon<WP_NUM_WEAPONS; weapon++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $458
line 1017
;1017:			wasPickedUpBySomeone[weapon] = 0;				// CLEAR
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 76
ADDP4
CNSTI4 0
ASGNI4
LABELV $459
line 1016
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $458
line 1019
;1018:
;1019:	for (person=0; person<g_maxclients.integer; person++)
ADDRLP4 72
CNSTI4 0
ASGNI4
ADDRGP4 $465
JUMPV
LABELV $462
line 1020
;1020:	{
line 1021
;1021:		for (weapon = 0; weapon<WP_NUM_WEAPONS; weapon++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $467
line 1022
;1022:		{
line 1023
;1023:			if (G_WeaponLogPickups[person][weapon]>0)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 72
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogPickups
ADDP4
ADDP4
INDIRI4
CNSTI4 0
LEI4 $471
line 1024
;1024:				wasPickedUpBySomeone[weapon]++;
ADDRLP4 164
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 76
ADDP4
ASGNP4
ADDRLP4 164
INDIRP4
ADDRLP4 164
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $471
line 1025
;1025:		}
LABELV $468
line 1021
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $467
line 1026
;1026:	}
LABELV $463
line 1019
ADDRLP4 72
ADDRLP4 72
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $465
ADDRLP4 72
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $462
line 1033
;1027:	//------------------------------------------------------ FOR EVERY WEAPON, ADD UP TOTAL PICKUPS
;1028:
;1029:
;1030:
;1031:
;1032:	//------------------------------------------------------ FOR EVERY PERSON, CHECK FOR CANDIDATE
;1033:	for (person=0; person<g_maxclients.integer; person++)
ADDRLP4 72
CNSTI4 0
ASGNI4
ADDRGP4 $476
JUMPV
LABELV $473
line 1034
;1034:	{
line 1035
;1035:		player = g_entities + person;
ADDRLP4 144
CNSTI4 828
ADDRLP4 72
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1036
;1036:		if (!player->inuse)			continue;
ADDRLP4 144
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
NEI4 $478
ADDRGP4 $474
JUMPV
LABELV $478
line 1038
;1037:
;1038:		nKills = 0;											// This Persons's Kills
ADDRLP4 140
CNSTI4 0
ASGNI4
line 1039
;1039:		for (weapon=0; weapon<WP_NUM_WEAPONS; weapon++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $480
line 1040
;1040:			killsWithWeapon[weapon] = 0;					// CLEAR
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
CNSTI4 0
ASGNI4
LABELV $481
line 1039
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $480
line 1042
;1041:
;1042:		for (i=0; i<MOD_MAX; i++)
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $484
line 1043
;1043:		{
line 1044
;1044:			weapon = weaponFromMOD[i];									// Select Weapon
ADDRLP4 0
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 weaponFromMOD
ADDP4
INDIRI4
ASGNI4
line 1045
;1045:			killsWithWeapon[weapon] += G_WeaponLogKills[person][i];		// Store Num Kills With Weapon
ADDRLP4 164
CNSTI4 2
ASGNI4
ADDRLP4 168
ADDRLP4 0
INDIRI4
ADDRLP4 164
INDIRI4
LSHI4
ADDRLP4 8
ADDP4
ASGNP4
ADDRLP4 168
INDIRP4
ADDRLP4 168
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDRLP4 164
INDIRI4
LSHI4
CNSTI4 152
ADDRLP4 72
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1046
;1046:		}
LABELV $485
line 1042
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 38
LTI4 $484
line 1048
;1047:
;1048:		weapon=WP_STUN_BATON;		// Start At Stun Baton
ADDRLP4 0
CNSTI4 1
ASGNI4
ADDRGP4 $489
JUMPV
LABELV $488
line 1051
;1049:		//   keep looking through weapons if weapon is not on map, or if it is and we used it
;1050:		while( weapon<WP_NUM_WEAPONS && (!wasPickedUpBySomeone[weapon] || killsWithWeapon[weapon]>0) )
;1051:		{
line 1052
;1052:			weapon++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1053
;1053:			nKills+=killsWithWeapon[weapon];							//  Update the number of kills
ADDRLP4 140
ADDRLP4 140
INDIRI4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1054
;1054:		}
LABELV $489
line 1050
ADDRLP4 0
INDIRI4
CNSTI4 16
GEI4 $491
ADDRLP4 168
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 172
CNSTI4 0
ASGNI4
ADDRLP4 168
INDIRI4
ADDRLP4 76
ADDP4
INDIRI4
ADDRLP4 172
INDIRI4
EQI4 $488
ADDRLP4 168
INDIRI4
ADDRLP4 8
ADDP4
INDIRI4
ADDRLP4 172
INDIRI4
GTI4 $488
LABELV $491
line 1062
;1055:		//
;1056:		// At this point we have either successfully gone through every weapon on the map and saw it had
;1057:		// been used, or we found one that WAS on the map and was NOT used
;1058:		//
;1059:		// so we look to see if the weapon==Max (i.e. we used every one) and then we check to see
;1060:		// if we got the most kills out of anyone else who did this.
;1061:		//
;1062:		if (weapon>=WP_NUM_WEAPONS && nKills>nMostKills)
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $492
ADDRLP4 140
INDIRI4
ADDRLP4 148
INDIRI4
LEI4 $492
line 1063
;1063:		{
line 1065
;1064:			// WE ARE A TACTICION CANDIDATE
;1065:			nMostKills  = nKills;
ADDRLP4 148
ADDRLP4 140
INDIRI4
ASGNI4
line 1066
;1066:			nBestPlayer = person;
ADDRLP4 152
ADDRLP4 72
INDIRI4
ASGNI4
line 1067
;1067:		}
LABELV $492
line 1068
;1068:	}
LABELV $474
line 1033
ADDRLP4 72
ADDRLP4 72
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $476
ADDRLP4 72
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $473
line 1072
;1069:	//------------------------------------------------------ FOR EVERY PERSON, CHECK FOR CANDIDATE
;1070:
;1071:	//Now, if we are the best player, return true and the number of kills we got
;1072:	if (nBestPlayer == ent->s.number)
ADDRLP4 152
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $494
line 1073
;1073:	{
line 1074
;1074:		*kills = nMostKills;
ADDRFP4 4
INDIRP4
ADDRLP4 148
INDIRI4
ASGNI4
line 1075
;1075:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $447
JUMPV
LABELV $494
line 1078
;1076:	}
;1077:#endif // LOGGING_WEAPONS
;1078:	return qfalse;
CNSTI4 0
RETI4
LABELV $447
endproc CalculateTactician 176 0
export CalculateDemolitionist
proc CalculateDemolitionist 24 0
line 1086
;1079:}
;1080:
;1081:
;1082:
;1083:
;1084:// did this player earn the demolitionist award?
;1085:qboolean CalculateDemolitionist(gentity_t *ent, int *kills)
;1086:{
line 1088
;1087:#ifdef LOGGING_WEAPONS
;1088:	int			i = 0, nBestPlayer = -1, nKills = 0, nMostKills = 0,
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRLP4 20
CNSTI4 -1
ASGNI4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 12
CNSTI4 0
ASGNI4
line 1089
;1089:				playTime = (level.time - ent->client->pers.enterTime)/60000;
ADDRLP4 16
ADDRGP4 level+32
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1456
ADDP4
INDIRI4
SUBI4
CNSTI4 60000
DIVI4
ASGNI4
line 1090
;1090:	gentity_t	*player = NULL;
ADDRLP4 8
CNSTP4 0
ASGNP4
line 1092
;1091:
;1092:	for (i = 0; i < g_maxclients.integer; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $501
JUMPV
LABELV $498
line 1093
;1093:	{
line 1094
;1094:		nKills = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 1095
;1095:		player = g_entities + i;
ADDRLP4 8
CNSTI4 828
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1096
;1096:		if (!player->inuse)
ADDRLP4 8
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
NEI4 $503
line 1097
;1097:			continue;
ADDRGP4 $499
JUMPV
LABELV $503
line 1099
;1098:
;1099:		nKills = G_WeaponLogKills[i][MOD_THERMAL];
ADDRLP4 4
CNSTI4 152
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills+88
ADDP4
INDIRI4
ASGNI4
line 1100
;1100:		nKills += G_WeaponLogKills[i][MOD_THERMAL_SPLASH];
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 152
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills+92
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1101
;1101:		nKills += G_WeaponLogKills[i][MOD_ROCKET];
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 152
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills+72
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1102
;1102:		nKills += G_WeaponLogKills[i][MOD_ROCKET_SPLASH];
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 152
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills+76
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1103
;1103:		nKills += G_WeaponLogKills[i][MOD_ROCKET_HOMING];
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 152
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills+80
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1104
;1104:		nKills += G_WeaponLogKills[i][MOD_ROCKET_HOMING_SPLASH];
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 152
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills+84
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1105
;1105:		nKills += G_WeaponLogKills[i][MOD_TRIP_MINE_SPLASH];
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 152
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills+96
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1106
;1106:		nKills += G_WeaponLogKills[i][MOD_TIMED_MINE_SPLASH];
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 152
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills+100
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1107
;1107:		nKills += G_WeaponLogKills[i][MOD_DET_PACK_SPLASH];
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 152
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills+104
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1110
;1108:
;1109:		// if this guy didn't get two explosive kills per minute, reject him right now
;1110:		if ( ((float)nKills)/((float)(playTime)) < 2.0 )
ADDRLP4 4
INDIRI4
CVIF4 4
ADDRLP4 16
INDIRI4
CVIF4 4
DIVF4
CNSTF4 1073741824
GEF4 $514
line 1111
;1111:		{
line 1112
;1112:			continue;
ADDRGP4 $499
JUMPV
LABELV $514
line 1115
;1113:		}
;1114:
;1115:		if (nKills > nMostKills)
ADDRLP4 4
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $516
line 1116
;1116:		{
line 1117
;1117:			nMostKills = nKills;
ADDRLP4 12
ADDRLP4 4
INDIRI4
ASGNI4
line 1118
;1118:			nBestPlayer = i;
ADDRLP4 20
ADDRLP4 0
INDIRI4
ASGNI4
line 1119
;1119:		}
LABELV $516
line 1120
;1120:	}
LABELV $499
line 1092
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $501
ADDRLP4 0
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $498
line 1121
;1121:	if (-1 == nBestPlayer)
ADDRLP4 20
INDIRI4
CNSTI4 -1
NEI4 $518
line 1122
;1122:	{
line 1123
;1123:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $496
JUMPV
LABELV $518
line 1125
;1124:	}
;1125:	if (nBestPlayer == ent->s.number)
ADDRLP4 20
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $520
line 1126
;1126:	{
line 1127
;1127:		*kills = nMostKills;
ADDRFP4 4
INDIRP4
ADDRLP4 12
INDIRI4
ASGNI4
line 1128
;1128:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $496
JUMPV
LABELV $520
line 1131
;1129:	}
;1130:#endif // LOGGING_WEAPONS
;1131:	return qfalse;
CNSTI4 0
RETI4
LABELV $496
endproc CalculateDemolitionist 24 0
export CalculateStreak
proc CalculateStreak 0 0
line 1135
;1132:}
;1133:
;1134:int CalculateStreak(gentity_t *ent)
;1135:{
line 1155
;1136:#if 0
;1137:	if (ent->client->ps.persistant[PERS_STREAK_COUNT] >= STREAK_CHAMPION)
;1138:	{
;1139:		return STREAK_CHAMPION;
;1140:	}
;1141:	if (ent->client->ps.persistant[PERS_STREAK_COUNT] >= STREAK_MASTER)
;1142:	{
;1143:		return STREAK_MASTER;
;1144:	}
;1145:	if (ent->client->ps.persistant[PERS_STREAK_COUNT] >= STREAK_EXPERT)
;1146:	{
;1147:		return STREAK_EXPERT;
;1148:	}
;1149:	if (ent->client->ps.persistant[PERS_STREAK_COUNT] >= STREAK_ACE)
;1150:	{
;1151:		return STREAK_ACE;
;1152:	}
;1153:#endif
;1154:	//No streak calculation, at least for now.
;1155:	return 0;
CNSTI4 0
RETI4
LABELV $522
endproc CalculateStreak 0 0
export CalculateTeamMVP
proc CalculateTeamMVP 28 0
line 1159
;1156:}
;1157:
;1158:qboolean CalculateTeamMVP(gentity_t *ent)
;1159:{
line 1160
;1160:	int			i = 0, nBestPlayer = -1, nScore = 0, nHighestScore = 0,
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 20
CNSTI4 -1
ASGNI4
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRLP4 12
CNSTI4 0
ASGNI4
line 1161
;1161:				team = ent->client->ps.persistant[PERS_TEAM];
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
ASGNI4
line 1162
;1162:	gentity_t	*player = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
line 1164
;1163:
;1164:	for (i = 0; i < g_maxclients.integer; i++)
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $527
JUMPV
LABELV $524
line 1165
;1165:	{
line 1166
;1166:		nScore = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 1167
;1167:		player = g_entities + i;
ADDRLP4 0
CNSTI4 828
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1168
;1168:		if (!player->inuse || (player->client->ps.persistant[PERS_TEAM] != team))
ADDRLP4 0
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $531
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $529
LABELV $531
line 1169
;1169:			continue;
ADDRGP4 $525
JUMPV
LABELV $529
line 1170
;1170:		nScore = player->client->ps.persistant[PERS_SCORE];
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 280
ADDP4
INDIRI4
ASGNI4
line 1171
;1171:		if (nScore > nHighestScore)
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $532
line 1172
;1172:		{
line 1173
;1173:			nHighestScore = nScore;
ADDRLP4 12
ADDRLP4 8
INDIRI4
ASGNI4
line 1174
;1174:			nBestPlayer = i;
ADDRLP4 20
ADDRLP4 4
INDIRI4
ASGNI4
line 1175
;1175:		}
LABELV $532
line 1176
;1176:	}
LABELV $525
line 1164
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $527
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $524
line 1177
;1177:	if (-1 == nBestPlayer)
ADDRLP4 20
INDIRI4
CNSTI4 -1
NEI4 $534
line 1178
;1178:	{
line 1179
;1179:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $523
JUMPV
LABELV $534
line 1181
;1180:	}
;1181:	if (nBestPlayer == ent->s.number)
ADDRLP4 20
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $536
line 1182
;1182:	{
line 1183
;1183:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $523
JUMPV
LABELV $536
line 1185
;1184:	}
;1185:	return qfalse;
CNSTI4 0
RETI4
LABELV $523
endproc CalculateTeamMVP 28 0
export CalculateTeamMVPByRank
proc CalculateTeamMVPByRank 32 0
line 1189
;1186:}
;1187:
;1188:qboolean CalculateTeamMVPByRank(gentity_t *ent)
;1189:{
line 1190
;1190:	int			i = 0, nBestPlayer = -1, nScore = 0, nHighestScore = 0,
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRLP4 20
CNSTI4 -1
ASGNI4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 12
CNSTI4 0
ASGNI4
line 1191
;1191:				team = ent->client->ps.persistant[PERS_RANK]+1;
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 288
ADDP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1192
;1192:	qboolean	bTied = (team == 3);
ADDRLP4 24
INDIRI4
CNSTI4 3
NEI4 $540
ADDRLP4 28
CNSTI4 1
ASGNI4
ADDRGP4 $541
JUMPV
LABELV $540
ADDRLP4 28
CNSTI4 0
ASGNI4
LABELV $541
ADDRLP4 16
ADDRLP4 28
INDIRI4
ASGNI4
line 1193
;1193:	gentity_t	*player = NULL;
ADDRLP4 8
CNSTP4 0
ASGNP4
line 1209
;1194:
;1195:	/*
;1196:	if ( team == ent->client->ps.persistant[PERS_TEAM] && ent->client->ps.persistant[PERS_CLASS] == PC_BORG )
;1197:	{//only the queen can be the MVP
;1198:		if ( borgQueenClientNum == ent->s.number )
;1199:		{
;1200:			return qtrue;
;1201:		}
;1202:		else
;1203:		{
;1204:			return qfalse;
;1205:		}
;1206:	}
;1207:	*/
;1208:
;1209:	for (i = 0; i < g_maxclients.integer; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $545
JUMPV
LABELV $542
line 1210
;1210:	{
line 1211
;1211:		nScore = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 1212
;1212:		player = g_entities + i;
ADDRLP4 8
CNSTI4 828
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1213
;1213:		if (!player->inuse)
ADDRLP4 8
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
NEI4 $547
line 1214
;1214:			continue;
ADDRGP4 $543
JUMPV
LABELV $547
line 1215
;1215:		if (!bTied)
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $549
line 1216
;1216:		{
line 1217
;1217:			 if (player->client->ps.persistant[PERS_TEAM] != team)
ADDRLP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
ADDRLP4 24
INDIRI4
EQI4 $551
line 1218
;1218:			 {
line 1219
;1219:				 continue;
ADDRGP4 $543
JUMPV
LABELV $551
line 1221
;1220:			 }
;1221:		}
LABELV $549
line 1222
;1222:		nScore = player->client->ps.persistant[PERS_SCORE];
ADDRLP4 4
ADDRLP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 280
ADDP4
INDIRI4
ASGNI4
line 1223
;1223:		if (nScore > nHighestScore)
ADDRLP4 4
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $553
line 1224
;1224:		{
line 1225
;1225:			nHighestScore = nScore;
ADDRLP4 12
ADDRLP4 4
INDIRI4
ASGNI4
line 1226
;1226:			nBestPlayer = i;
ADDRLP4 20
ADDRLP4 0
INDIRI4
ASGNI4
line 1227
;1227:		}
LABELV $553
line 1228
;1228:	}
LABELV $543
line 1209
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $545
ADDRLP4 0
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $542
line 1229
;1229:	if (-1 == nBestPlayer)
ADDRLP4 20
INDIRI4
CNSTI4 -1
NEI4 $555
line 1230
;1230:	{
line 1231
;1231:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $538
JUMPV
LABELV $555
line 1233
;1232:	}
;1233:	if (nBestPlayer == ent->s.number)
ADDRLP4 20
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $557
line 1234
;1234:	{
line 1235
;1235:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $538
JUMPV
LABELV $557
line 1237
;1236:	}
;1237:	return qfalse;
CNSTI4 0
RETI4
LABELV $538
endproc CalculateTeamMVPByRank 32 0
export CalculateTeamDefender
proc CalculateTeamDefender 28 0
line 1241
;1238:}
;1239:
;1240:qboolean CalculateTeamDefender(gentity_t *ent)
;1241:{
line 1242
;1242:	int			i = 0, nBestPlayer = -1, nScore = 0, nHighestScore = 0,
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 20
CNSTI4 -1
ASGNI4
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRLP4 12
CNSTI4 0
ASGNI4
line 1243
;1243:				team = ent->client->ps.persistant[PERS_TEAM];
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
ASGNI4
line 1244
;1244:	gentity_t	*player = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
line 1252
;1245:
;1246:	/*
;1247:	if (CalculateTeamMVP(ent))
;1248:	{
;1249:		return qfalse;
;1250:	}
;1251:	*/
;1252:	for (i = 0; i < g_maxclients.integer; i++)
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $563
JUMPV
LABELV $560
line 1253
;1253:	{
line 1254
;1254:		nScore = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 1255
;1255:		player = g_entities + i;
ADDRLP4 0
CNSTI4 828
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1256
;1256:		if (!player->inuse || (player->client->ps.persistant[PERS_TEAM] != team))
ADDRLP4 0
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $567
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $565
LABELV $567
line 1257
;1257:			continue;
ADDRGP4 $561
JUMPV
LABELV $565
line 1258
;1258:		nScore = player->client->pers.teamState.basedefense;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1472
ADDP4
INDIRI4
ASGNI4
line 1259
;1259:		if (nScore > nHighestScore)
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $568
line 1260
;1260:		{
line 1261
;1261:			nHighestScore = nScore;
ADDRLP4 12
ADDRLP4 8
INDIRI4
ASGNI4
line 1262
;1262:			nBestPlayer = i;
ADDRLP4 20
ADDRLP4 4
INDIRI4
ASGNI4
line 1263
;1263:		}
LABELV $568
line 1264
;1264:	}
LABELV $561
line 1252
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $563
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $560
line 1265
;1265:	if (-1 == nBestPlayer)
ADDRLP4 20
INDIRI4
CNSTI4 -1
NEI4 $570
line 1266
;1266:	{
line 1267
;1267:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $559
JUMPV
LABELV $570
line 1269
;1268:	}
;1269:	if (nBestPlayer == ent->s.number)
ADDRLP4 20
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $572
line 1270
;1270:	{
line 1271
;1271:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $559
JUMPV
LABELV $572
line 1273
;1272:	}
;1273:	return qfalse;
CNSTI4 0
RETI4
LABELV $559
endproc CalculateTeamDefender 28 0
export CalculateTeamWarrior
proc CalculateTeamWarrior 28 0
line 1277
;1274:}
;1275:
;1276:qboolean CalculateTeamWarrior(gentity_t *ent)
;1277:{
line 1278
;1278:	int			i = 0, nBestPlayer = -1, nScore = 0, nHighestScore = 0,
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 20
CNSTI4 -1
ASGNI4
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRLP4 12
CNSTI4 0
ASGNI4
line 1279
;1279:				team = ent->client->ps.persistant[PERS_TEAM];
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
ASGNI4
line 1280
;1280:	gentity_t	*player = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
line 1288
;1281:
;1282:	/*
;1283:	if (CalculateTeamMVP(ent) || CalculateTeamDefender(ent))
;1284:	{
;1285:		return qfalse;
;1286:	}
;1287:	*/
;1288:	for (i = 0; i < g_maxclients.integer; i++)
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $578
JUMPV
LABELV $575
line 1289
;1289:	{
line 1290
;1290:		nScore = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 1291
;1291:		player = g_entities + i;
ADDRLP4 0
CNSTI4 828
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1292
;1292:		if (!player->inuse || (player->client->ps.persistant[PERS_TEAM] != team))
ADDRLP4 0
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $582
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $580
LABELV $582
line 1293
;1293:			continue;
ADDRGP4 $576
JUMPV
LABELV $580
line 1294
;1294:		nScore = player->client->ps.persistant[PERS_SCORE];
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 280
ADDP4
INDIRI4
ASGNI4
line 1295
;1295:		if (nScore > nHighestScore)
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $583
line 1296
;1296:		{
line 1297
;1297:			nHighestScore = nScore;
ADDRLP4 12
ADDRLP4 8
INDIRI4
ASGNI4
line 1298
;1298:			nBestPlayer = i;
ADDRLP4 20
ADDRLP4 4
INDIRI4
ASGNI4
line 1299
;1299:		}
LABELV $583
line 1300
;1300:	}
LABELV $576
line 1288
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $578
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $575
line 1301
;1301:	if (-1 == nBestPlayer)
ADDRLP4 20
INDIRI4
CNSTI4 -1
NEI4 $585
line 1302
;1302:	{
line 1303
;1303:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $574
JUMPV
LABELV $585
line 1305
;1304:	}
;1305:	if (nBestPlayer == ent->s.number)
ADDRLP4 20
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $587
line 1306
;1306:	{
line 1307
;1307:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $574
JUMPV
LABELV $587
line 1309
;1308:	}
;1309:	return qfalse;
CNSTI4 0
RETI4
LABELV $574
endproc CalculateTeamWarrior 28 0
export CalculateTeamCarrier
proc CalculateTeamCarrier 28 0
line 1313
;1310:}
;1311:
;1312:qboolean CalculateTeamCarrier(gentity_t *ent)
;1313:{
line 1314
;1314:	int			i = 0, nBestPlayer = -1, nScore = 0, nHighestScore = 0,
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 20
CNSTI4 -1
ASGNI4
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRLP4 12
CNSTI4 0
ASGNI4
line 1315
;1315:				team = ent->client->ps.persistant[PERS_TEAM];
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
ASGNI4
line 1316
;1316:	gentity_t	*player = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
line 1324
;1317:
;1318:	/*
;1319:	if (CalculateTeamMVP(ent) || CalculateTeamDefender(ent) || CalculateTeamWarrior(ent))
;1320:	{
;1321:		return qfalse;
;1322:	}
;1323:	*/
;1324:	for (i = 0; i < g_maxclients.integer; i++)
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $593
JUMPV
LABELV $590
line 1325
;1325:	{
line 1326
;1326:		nScore = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 1327
;1327:		player = g_entities + i;
ADDRLP4 0
CNSTI4 828
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1328
;1328:		if (!player->inuse || (player->client->ps.persistant[PERS_TEAM] != team))
ADDRLP4 0
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $597
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $595
LABELV $597
line 1329
;1329:			continue;
ADDRGP4 $591
JUMPV
LABELV $595
line 1330
;1330:		nScore = player->client->pers.teamState.captures;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1468
ADDP4
INDIRI4
ASGNI4
line 1331
;1331:		if (nScore > nHighestScore)
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $598
line 1332
;1332:		{
line 1333
;1333:			nHighestScore = nScore;
ADDRLP4 12
ADDRLP4 8
INDIRI4
ASGNI4
line 1334
;1334:			nBestPlayer = i;
ADDRLP4 20
ADDRLP4 4
INDIRI4
ASGNI4
line 1335
;1335:		}
LABELV $598
line 1336
;1336:	}
LABELV $591
line 1324
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $593
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $590
line 1337
;1337:	if (-1 == nBestPlayer)
ADDRLP4 20
INDIRI4
CNSTI4 -1
NEI4 $600
line 1338
;1338:	{
line 1339
;1339:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $589
JUMPV
LABELV $600
line 1341
;1340:	}
;1341:	if (nBestPlayer == ent->s.number)
ADDRLP4 20
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $602
line 1342
;1342:	{
line 1343
;1343:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $589
JUMPV
LABELV $602
line 1345
;1344:	}
;1345:	return qfalse;
CNSTI4 0
RETI4
LABELV $589
endproc CalculateTeamCarrier 28 0
export CalculateTeamInterceptor
proc CalculateTeamInterceptor 28 0
line 1349
;1346:}
;1347:
;1348:qboolean CalculateTeamInterceptor(gentity_t *ent)
;1349:{
line 1350
;1350:	int			i = 0, nBestPlayer = -1, nScore = 0, nHighestScore = 0,
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRLP4 20
CNSTI4 -1
ASGNI4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 12
CNSTI4 0
ASGNI4
line 1351
;1351:				team = ent->client->ps.persistant[PERS_TEAM];
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
ASGNI4
line 1352
;1352:	gentity_t	*player = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
line 1361
;1353:
;1354:	/*
;1355:	if (CalculateTeamMVP(ent) || CalculateTeamDefender(ent) || CalculateTeamWarrior(ent) ||
;1356:		CalculateTeamCarrier(ent))
;1357:	{
;1358:		return qfalse;
;1359:	}
;1360:	*/
;1361:	for (i = 0; i < g_maxclients.integer; i++)
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $608
JUMPV
LABELV $605
line 1362
;1362:	{
line 1363
;1363:		nScore = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 1364
;1364:		player = g_entities + i;
ADDRLP4 0
CNSTI4 828
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1365
;1365:		if (!player->inuse || (player->client->ps.persistant[PERS_TEAM] != team))
ADDRLP4 0
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $612
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $610
LABELV $612
line 1366
;1366:			continue;
ADDRGP4 $606
JUMPV
LABELV $610
line 1367
;1367:		nScore = player->client->pers.teamState.flagrecovery;
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1480
ADDP4
INDIRI4
ASGNI4
line 1368
;1368:		nScore += player->client->pers.teamState.fragcarrier;
ADDRLP4 4
ADDRLP4 4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1484
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1369
;1369:		if (nScore > nHighestScore)
ADDRLP4 4
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $613
line 1370
;1370:		{
line 1371
;1371:			nHighestScore = nScore;
ADDRLP4 12
ADDRLP4 4
INDIRI4
ASGNI4
line 1372
;1372:			nBestPlayer = i;
ADDRLP4 20
ADDRLP4 8
INDIRI4
ASGNI4
line 1373
;1373:		}
LABELV $613
line 1374
;1374:	}
LABELV $606
line 1361
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $608
ADDRLP4 8
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $605
line 1375
;1375:	if (-1 == nBestPlayer)
ADDRLP4 20
INDIRI4
CNSTI4 -1
NEI4 $615
line 1376
;1376:	{
line 1377
;1377:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $604
JUMPV
LABELV $615
line 1379
;1378:	}
;1379:	if (nBestPlayer == ent->s.number)
ADDRLP4 20
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $617
line 1380
;1380:	{
line 1381
;1381:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $604
JUMPV
LABELV $617
line 1383
;1382:	}
;1383:	return qfalse;
CNSTI4 0
RETI4
LABELV $604
endproc CalculateTeamInterceptor 28 0
export CalculateTeamRedShirt
proc CalculateTeamRedShirt 28 0
line 1387
;1384:}
;1385:
;1386:qboolean CalculateTeamRedShirt(gentity_t *ent)
;1387:{
line 1388
;1388:	int			i = 0, nBestPlayer = -1, nScore = 0, nHighestScore = 0,
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRLP4 20
CNSTI4 -1
ASGNI4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 12
CNSTI4 0
ASGNI4
line 1389
;1389:				team = ent->client->ps.persistant[PERS_TEAM];
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
ASGNI4
line 1390
;1390:	gentity_t	*player = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
line 1399
;1391:
;1392:	/*
;1393:	if (CalculateTeamMVP(ent) || CalculateTeamDefender(ent) || CalculateTeamWarrior(ent) ||
;1394:		CalculateTeamCarrier(ent) || CalculateTeamInterceptor(ent))
;1395:	{
;1396:		return qfalse;
;1397:	}
;1398:	*/
;1399:	for (i = 0; i < g_maxclients.integer; i++)
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $623
JUMPV
LABELV $620
line 1400
;1400:	{
line 1401
;1401:		nScore = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 1402
;1402:		player = g_entities + i;
ADDRLP4 0
CNSTI4 828
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1403
;1403:		if (!player->inuse || (player->client->ps.persistant[PERS_TEAM] != team))
ADDRLP4 0
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $627
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $625
LABELV $627
line 1404
;1404:			continue;
ADDRGP4 $621
JUMPV
LABELV $625
line 1405
;1405:		nScore = player->client->ps.persistant[PERS_KILLED];
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 312
ADDP4
INDIRI4
ASGNI4
line 1406
;1406:		nScore -= player->client->ps.fd.suicides; // suicides don't count, you big cheater.
ADDRLP4 4
ADDRLP4 4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1224
ADDP4
INDIRI4
SUBI4
ASGNI4
line 1407
;1407:		if (nScore > nHighestScore)
ADDRLP4 4
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $628
line 1408
;1408:		{
line 1409
;1409:			nHighestScore = nScore;
ADDRLP4 12
ADDRLP4 4
INDIRI4
ASGNI4
line 1410
;1410:			nBestPlayer = i;
ADDRLP4 20
ADDRLP4 8
INDIRI4
ASGNI4
line 1411
;1411:		}
LABELV $628
line 1412
;1412:	}
LABELV $621
line 1399
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $623
ADDRLP4 8
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $620
line 1413
;1413:	if (-1 == nBestPlayer)
ADDRLP4 20
INDIRI4
CNSTI4 -1
NEI4 $630
line 1414
;1414:	{
line 1415
;1415:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $619
JUMPV
LABELV $630
line 1417
;1416:	}
;1417:	if (nBestPlayer == ent->s.number)
ADDRLP4 20
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $632
line 1418
;1418:	{
line 1419
;1419:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $619
JUMPV
LABELV $632
line 1421
;1420:	}
;1421:	return qfalse;
CNSTI4 0
RETI4
LABELV $619
endproc CalculateTeamRedShirt 28 0
export CalculateTeamAward
proc CalculateTeamAward 24 4
line 1450
;1422:}
;1423:
;1424:typedef enum {
;1425:	AWARD_EFFICIENCY,		// Accuracy
;1426:	AWARD_SHARPSHOOTER,		// Most compression rifle frags
;1427:	AWARD_UNTOUCHABLE,		// Perfect (no deaths)
;1428:	AWARD_LOGISTICS,		// Most pickups
;1429:	AWARD_TACTICIAN,		// Kills with all weapons
;1430:	AWARD_DEMOLITIONIST,	// Most explosive damage kills
;1431:	AWARD_STREAK,			// Ace/Expert/Master/Champion
;1432:	AWARD_TEAM,				// MVP/Defender/Warrior/Carrier/Interceptor/Bravery
;1433:	AWARD_SECTION31,		// All-around god
;1434:	AWARD_MAX
;1435:} awardType_t;
;1436:
;1437:typedef enum
;1438:{
;1439:	TEAM_NONE = 0,			// ha ha! you suck!
;1440:	TEAM_MVP,				// most overall points
;1441:	TEAM_DEFENDER,			// killed the most baddies near your flag
;1442:	TEAM_WARRIOR,			// most frags
;1443:	TEAM_CARRIER,			// infected the most people with plague
;1444:	TEAM_INTERCEPTOR,		// returned your own flag the most
;1445:	TEAM_BRAVERY,			// Red Shirt Award (tm). you died more than anybody. 
;1446:	TEAM_MAX
;1447:} teamAward_e;
;1448:
;1449:int CalculateTeamAward(gentity_t *ent)
;1450:{
line 1451
;1451:	int teamAwards = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1453
;1452:
;1453:	if (CalculateTeamMVP(ent))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 CalculateTeamMVP
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $637
line 1454
;1454:	{
line 1455
;1455:		teamAwards |= (1<<TEAM_MVP);
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 1456
;1456:	}
LABELV $637
line 1457
;1457:	if (GT_CTF == g_gametype.integer ||
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 7
EQI4 $643
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 8
NEI4 $639
LABELV $643
line 1459
;1458:		GT_CTY == g_gametype.integer)
;1459:	{
line 1460
;1460:		if (CalculateTeamDefender(ent))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 CalculateTeamDefender
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $644
line 1461
;1461:		{
line 1462
;1462:			teamAwards |= (1<<TEAM_DEFENDER);
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 1463
;1463:		}
LABELV $644
line 1464
;1464:		if (CalculateTeamWarrior(ent))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 CalculateTeamWarrior
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $646
line 1465
;1465:		{
line 1466
;1466:			teamAwards |= (1<<TEAM_WARRIOR);
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 1467
;1467:		}
LABELV $646
line 1468
;1468:		if (CalculateTeamCarrier(ent))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 CalculateTeamCarrier
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $648
line 1469
;1469:		{
line 1470
;1470:			teamAwards |= (1<<TEAM_CARRIER);
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 1471
;1471:		}
LABELV $648
line 1472
;1472:		if (CalculateTeamInterceptor(ent))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 CalculateTeamInterceptor
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $650
line 1473
;1473:		{
line 1474
;1474:			teamAwards |= (1<<TEAM_INTERCEPTOR);
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 1475
;1475:		}
LABELV $650
line 1476
;1476:	}
LABELV $639
line 1477
;1477:	if ( !teamAwards && CalculateTeamRedShirt(ent) )
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $652
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 CalculateTeamRedShirt
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $652
line 1478
;1478:	{//if you got nothing else and died a lot, at least get bravery
line 1479
;1479:		teamAwards |= (1<<TEAM_BRAVERY);
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 1480
;1480:	}
LABELV $652
line 1481
;1481:	return teamAwards;
ADDRLP4 0
INDIRI4
RETI4
LABELV $636
endproc CalculateTeamAward 24 4
export CalculateSection31Award
proc CalculateSection31Award 24 8
line 1485
;1482:}
;1483:
;1484:qboolean CalculateSection31Award(gentity_t *ent)
;1485:{
line 1486
;1486:	int			i = 0, frags = 0, efficiency = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRLP4 4
CNSTI4 0
ASGNI4
line 1487
;1487:	gentity_t	*player = NULL;
ADDRLP4 8
CNSTP4 0
ASGNP4
line 1489
;1488:
;1489:	for (i = 0; i < g_maxclients.integer; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $658
JUMPV
LABELV $655
line 1490
;1490:	{
line 1491
;1491:		player = g_entities + i;
ADDRLP4 8
CNSTI4 828
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1492
;1492:		if (!player->inuse)
ADDRLP4 8
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
NEI4 $660
line 1493
;1493:			continue;
ADDRGP4 $656
JUMPV
LABELV $660
line 1501
;1494://
;1495://	kef -- heh.
;1496://
;1497://		if (strcmp("JaxxonPhred", ent->client->pers.netname))
;1498://		{
;1499://			continue;
;1500://		}
;1501:		CalculateEfficiency(ent, &efficiency);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 CalculateEfficiency
CALLI4
pop
line 1502
;1502:		if (!CalculateSharpshooter(ent, &frags) ||
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 16
ADDRGP4 CalculateSharpshooter
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $665
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 CalculateUntouchable
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $665
ADDRLP4 4
INDIRI4
CNSTI4 75
GEI4 $662
LABELV $665
line 1506
;1503:			!CalculateUntouchable(ent) ||
;1504:			/*(CalculateStreak(ent) < STREAK_CHAMPION) ||*/
;1505:			(efficiency < 75))
;1506:		{
line 1507
;1507:			continue;
ADDRGP4 $656
JUMPV
LABELV $662
line 1509
;1508:		}
;1509:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $654
JUMPV
LABELV $656
line 1489
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $658
ADDRLP4 0
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $655
line 1511
;1510:	}
;1511:	return qfalse;
CNSTI4 0
RETI4
LABELV $654
endproc CalculateSection31Award 24 8
export CalculateAwards
proc CalculateAwards 568 24
line 1517
;1512:}
;1513:
;1514:#define AWARDS_MSG_LENGTH		256
;1515:
;1516:void CalculateAwards(gentity_t *ent, char *msg)
;1517:{
line 1520
;1518:#ifdef LOGGING_WEAPONS
;1519:	char		buf1[AWARDS_MSG_LENGTH], buf2[AWARDS_MSG_LENGTH];
;1520:	int			awardFlags = 0, efficiency = 0, stuffUsed = 0, kills = 0, streak = 0, teamAwards = 0;
ADDRLP4 512
CNSTI4 0
ASGNI4
ADDRLP4 524
CNSTI4 0
ASGNI4
ADDRLP4 528
CNSTI4 0
ASGNI4
ADDRLP4 516
CNSTI4 0
ASGNI4
ADDRLP4 520
CNSTI4 0
ASGNI4
ADDRLP4 532
CNSTI4 0
ASGNI4
line 1522
;1521:
;1522:	memset(buf1, 0, AWARDS_MSG_LENGTH);
ADDRLP4 256
ARGP4
CNSTI4 0
ARGI4
CNSTI4 256
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1523
;1523:	memset(buf2, 0, AWARDS_MSG_LENGTH);
ADDRLP4 0
ARGP4
CNSTI4 0
ARGI4
CNSTI4 256
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1524
;1524:	if (CalculateEfficiency(ent, &efficiency))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 524
ARGP4
ADDRLP4 536
ADDRGP4 CalculateEfficiency
CALLI4
ASGNI4
ADDRLP4 536
INDIRI4
CNSTI4 0
EQI4 $667
line 1525
;1525:	{
line 1526
;1526:		awardFlags |= (1<<AWARD_EFFICIENCY);
ADDRLP4 512
ADDRLP4 512
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 1527
;1527:		Com_sprintf(buf1, AWARDS_MSG_LENGTH, " %d", efficiency);
ADDRLP4 256
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $669
ARGP4
ADDRLP4 524
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1528
;1528:	}
LABELV $667
line 1529
;1529:	if (CalculateSharpshooter(ent, &kills))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 516
ARGP4
ADDRLP4 540
ADDRGP4 CalculateSharpshooter
CALLI4
ASGNI4
ADDRLP4 540
INDIRI4
CNSTI4 0
EQI4 $670
line 1530
;1530:	{
line 1531
;1531:		awardFlags |= (1<<AWARD_SHARPSHOOTER);
ADDRLP4 512
ADDRLP4 512
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 1532
;1532:		strcpy(buf2, buf1);
ADDRLP4 0
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1533
;1533:		Com_sprintf(buf1, AWARDS_MSG_LENGTH, "%s %d", buf2, kills);
ADDRLP4 256
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $672
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 516
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1534
;1534:	}
LABELV $670
line 1535
;1535:	if (CalculateUntouchable(ent))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 544
ADDRGP4 CalculateUntouchable
CALLI4
ASGNI4
ADDRLP4 544
INDIRI4
CNSTI4 0
EQI4 $673
line 1536
;1536:	{
line 1537
;1537:		awardFlags |= (1<<AWARD_UNTOUCHABLE);
ADDRLP4 512
ADDRLP4 512
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 1538
;1538:		strcpy(buf2, buf1);
ADDRLP4 0
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1539
;1539:		Com_sprintf(buf1, AWARDS_MSG_LENGTH, "%s %d", buf2, 0);
ADDRLP4 256
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $672
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1540
;1540:	}
LABELV $673
line 1541
;1541:	if (CalculateLogistics(ent, &stuffUsed))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 528
ARGP4
ADDRLP4 548
ADDRGP4 CalculateLogistics
CALLI4
ASGNI4
ADDRLP4 548
INDIRI4
CNSTI4 0
EQI4 $675
line 1542
;1542:	{
line 1543
;1543:		awardFlags |= (1<<AWARD_LOGISTICS);
ADDRLP4 512
ADDRLP4 512
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 1544
;1544:		strcpy(buf2, buf1);
ADDRLP4 0
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1545
;1545:		Com_sprintf(buf1, AWARDS_MSG_LENGTH, "%s %d", buf2, stuffUsed);
ADDRLP4 256
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $672
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 528
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1546
;1546:	}
LABELV $675
line 1547
;1547:	if (CalculateTactician(ent, &kills))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 516
ARGP4
ADDRLP4 552
ADDRGP4 CalculateTactician
CALLI4
ASGNI4
ADDRLP4 552
INDIRI4
CNSTI4 0
EQI4 $677
line 1548
;1548:	{
line 1549
;1549:		awardFlags |= (1<<AWARD_TACTICIAN);
ADDRLP4 512
ADDRLP4 512
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 1550
;1550:		strcpy(buf2, buf1);
ADDRLP4 0
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1551
;1551:		Com_sprintf(buf1, AWARDS_MSG_LENGTH, "%s %d", buf2, kills);
ADDRLP4 256
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $672
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 516
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1552
;1552:	}
LABELV $677
line 1553
;1553:	if (CalculateDemolitionist(ent, &kills))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 516
ARGP4
ADDRLP4 556
ADDRGP4 CalculateDemolitionist
CALLI4
ASGNI4
ADDRLP4 556
INDIRI4
CNSTI4 0
EQI4 $679
line 1554
;1554:	{
line 1555
;1555:		awardFlags |= (1<<AWARD_DEMOLITIONIST);
ADDRLP4 512
ADDRLP4 512
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 1556
;1556:		strcpy(buf2, buf1);
ADDRLP4 0
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1557
;1557:		Com_sprintf(buf1, AWARDS_MSG_LENGTH, "%s %d", buf2, kills);
ADDRLP4 256
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $672
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 516
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1558
;1558:	}
LABELV $679
line 1559
;1559:	streak = CalculateStreak(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 560
ADDRGP4 CalculateStreak
CALLI4
ASGNI4
ADDRLP4 520
ADDRLP4 560
INDIRI4
ASGNI4
line 1560
;1560:	if (streak)
ADDRLP4 520
INDIRI4
CNSTI4 0
EQI4 $681
line 1561
;1561:	{
line 1562
;1562:		awardFlags |= (1<<AWARD_STREAK);
ADDRLP4 512
ADDRLP4 512
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 1563
;1563:		strcpy(buf2, buf1);
ADDRLP4 0
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1564
;1564:		Com_sprintf(buf1, AWARDS_MSG_LENGTH, "%s %d", buf2, streak);
ADDRLP4 256
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $672
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 520
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1565
;1565:	}
LABELV $681
line 1566
;1566:	if (g_gametype.integer >= GT_TEAM)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
LTI4 $683
line 1567
;1567:	{
line 1568
;1568:		teamAwards = CalculateTeamAward(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 564
ADDRGP4 CalculateTeamAward
CALLI4
ASGNI4
ADDRLP4 532
ADDRLP4 564
INDIRI4
ASGNI4
line 1569
;1569:		if (teamAwards)
ADDRLP4 532
INDIRI4
CNSTI4 0
EQI4 $686
line 1570
;1570:		{
line 1571
;1571:			awardFlags |= (1<<AWARD_TEAM);
ADDRLP4 512
ADDRLP4 512
INDIRI4
CNSTI4 128
BORI4
ASGNI4
line 1572
;1572:			strcpy(buf2, buf1);
ADDRLP4 0
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1573
;1573:			Com_sprintf(buf1, AWARDS_MSG_LENGTH, "%s %d", buf2, teamAwards);
ADDRLP4 256
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $672
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 532
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1574
;1574:		}
LABELV $686
line 1575
;1575:	}
LABELV $683
line 1576
;1576:	if (CalculateSection31Award(ent))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 564
ADDRGP4 CalculateSection31Award
CALLI4
ASGNI4
ADDRLP4 564
INDIRI4
CNSTI4 0
EQI4 $688
line 1577
;1577:	{
line 1578
;1578:		awardFlags |= (1<<AWARD_SECTION31);
ADDRLP4 512
ADDRLP4 512
INDIRI4
CNSTI4 256
BORI4
ASGNI4
line 1579
;1579:		strcpy(buf2, buf1);
ADDRLP4 0
ARGP4
ADDRLP4 256
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1580
;1580:		Com_sprintf(buf1, AWARDS_MSG_LENGTH, "%s %d", buf2, 0);
ADDRLP4 256
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $672
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 1581
;1581:	}
LABELV $688
line 1582
;1582:	strcpy(buf2, msg);
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1583
;1583:	Com_sprintf( msg, AWARDS_MSG_LENGTH, "%s %d%s", buf2, awardFlags, buf1);
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 256
ARGI4
ADDRGP4 $690
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 512
INDIRI4
ARGI4
ADDRLP4 256
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 1585
;1584:#endif // LOGGING_WEAPONS
;1585:}
LABELV $666
endproc CalculateAwards 568 24
export GetMaxDeathsForClient
proc GetMaxDeathsForClient 12 0
line 1588
;1586:
;1587:int GetMaxDeathsForClient(int nClient)
;1588:{
line 1589
;1589:	int i = 0, nMostDeaths = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRLP4 4
CNSTI4 0
ASGNI4
line 1591
;1590:
;1591:	if ((nClient < 0) || (nClient >= MAX_CLIENTS))
ADDRLP4 8
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
LTI4 $694
ADDRLP4 8
INDIRI4
CNSTI4 32
LTI4 $692
LABELV $694
line 1592
;1592:	{
line 1593
;1593:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $691
JUMPV
LABELV $692
line 1595
;1594:	}
;1595:	for (i = 0; i < MAX_CLIENTS; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $695
line 1596
;1596:	{
line 1597
;1597:		if (G_WeaponLogFrags[i][nClient] > nMostDeaths)
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRI4
CNSTI4 7
LSHI4
ADDRGP4 G_WeaponLogFrags
ADDP4
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
LEI4 $699
line 1598
;1598:		{
line 1599
;1599:			nMostDeaths = G_WeaponLogFrags[i][nClient];
ADDRLP4 4
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRI4
CNSTI4 7
LSHI4
ADDRGP4 G_WeaponLogFrags
ADDP4
ADDP4
INDIRI4
ASGNI4
line 1600
;1600:		}
LABELV $699
line 1601
;1601:	}
LABELV $696
line 1595
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $695
line 1602
;1602:	return nMostDeaths;
ADDRLP4 4
INDIRI4
RETI4
LABELV $691
endproc GetMaxDeathsForClient 12 0
export GetMaxKillsForClient
proc GetMaxKillsForClient 12 0
line 1606
;1603:}
;1604:
;1605:int GetMaxKillsForClient(int nClient)
;1606:{
line 1607
;1607:	int i = 0, nMostKills = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRLP4 4
CNSTI4 0
ASGNI4
line 1609
;1608:
;1609:	if ((nClient < 0) || (nClient >= MAX_CLIENTS))
ADDRLP4 8
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
LTI4 $704
ADDRLP4 8
INDIRI4
CNSTI4 32
LTI4 $702
LABELV $704
line 1610
;1610:	{
line 1611
;1611:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $701
JUMPV
LABELV $702
line 1613
;1612:	}
;1613:	for (i = 0; i < MAX_CLIENTS; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $705
line 1614
;1614:	{
line 1615
;1615:		if (G_WeaponLogFrags[nClient][i] > nMostKills)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 7
LSHI4
ADDRGP4 G_WeaponLogFrags
ADDP4
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
LEI4 $709
line 1616
;1616:		{
line 1617
;1617:			nMostKills = G_WeaponLogFrags[nClient][i];
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 7
LSHI4
ADDRGP4 G_WeaponLogFrags
ADDP4
ADDP4
INDIRI4
ASGNI4
line 1618
;1618:		}
LABELV $709
line 1619
;1619:	}
LABELV $706
line 1613
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $705
line 1620
;1620:	return nMostKills;
ADDRLP4 4
INDIRI4
RETI4
LABELV $701
endproc GetMaxKillsForClient 12 0
export GetFavoriteTargetForClient
proc GetFavoriteTargetForClient 16 0
line 1624
;1621:}
;1622:
;1623:int GetFavoriteTargetForClient(int nClient)
;1624:{
line 1625
;1625:	int i = 0, nMostKills = 0, nFavoriteTarget = -1;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 8
CNSTI4 -1
ASGNI4
line 1627
;1626:
;1627:	if ((nClient < 0) || (nClient >= MAX_CLIENTS))
ADDRLP4 12
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
LTI4 $714
ADDRLP4 12
INDIRI4
CNSTI4 32
LTI4 $712
LABELV $714
line 1628
;1628:	{
line 1629
;1629:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $711
JUMPV
LABELV $712
line 1631
;1630:	}
;1631:	for (i = 0; i < MAX_CLIENTS; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $715
line 1632
;1632:	{
line 1633
;1633:		if (G_WeaponLogFrags[nClient][i] > nMostKills)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 7
LSHI4
ADDRGP4 G_WeaponLogFrags
ADDP4
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
LEI4 $719
line 1634
;1634:		{
line 1635
;1635:			nMostKills = G_WeaponLogFrags[nClient][i];
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 7
LSHI4
ADDRGP4 G_WeaponLogFrags
ADDP4
ADDP4
INDIRI4
ASGNI4
line 1636
;1636:			nFavoriteTarget = i;
ADDRLP4 8
ADDRLP4 0
INDIRI4
ASGNI4
line 1637
;1637:		}
LABELV $719
line 1638
;1638:	}
LABELV $716
line 1631
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $715
line 1639
;1639:	return nFavoriteTarget;
ADDRLP4 8
INDIRI4
RETI4
LABELV $711
endproc GetFavoriteTargetForClient 16 0
export GetWorstEnemyForClient
proc GetWorstEnemyForClient 32 0
line 1643
;1640:}
;1641:
;1642:int GetWorstEnemyForClient(int nClient)
;1643:{
line 1644
;1644:	int i = 0, nMostDeaths = 0, nWorstEnemy = -1;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 8
CNSTI4 -1
ASGNI4
line 1646
;1645:
;1646:	if ((nClient < 0) || (nClient >= MAX_CLIENTS))
ADDRLP4 12
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
LTI4 $724
ADDRLP4 12
INDIRI4
CNSTI4 32
LTI4 $722
LABELV $724
line 1647
;1647:	{
line 1648
;1648:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $721
JUMPV
LABELV $722
line 1650
;1649:	}
;1650:	for (i = 0; i < MAX_CLIENTS; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $725
line 1651
;1651:	{
line 1656
;1652:		// If there is a tie for most deaths, we want to choose anybody else
;1653:		// over the client...  I.E. Most deaths should not tie with yourself and
;1654:		// have yourself show up...
;1655:
;1656:		if ( G_WeaponLogFrags[i][nClient] > nMostDeaths ||
ADDRLP4 16
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 24
ADDRLP4 16
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRI4
CNSTI4 7
LSHI4
ADDRGP4 G_WeaponLogFrags
ADDP4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 24
INDIRI4
ADDRLP4 4
INDIRI4
GTI4 $731
ADDRLP4 24
INDIRI4
ADDRLP4 4
INDIRI4
NEI4 $729
ADDRLP4 0
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $729
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $729
LABELV $731
line 1658
;1657:			(G_WeaponLogFrags[i][nClient]== nMostDeaths && i!=nClient && nMostDeaths!=0) )
;1658:		{
line 1659
;1659:			nMostDeaths = G_WeaponLogFrags[i][nClient];
ADDRLP4 4
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRI4
CNSTI4 7
LSHI4
ADDRGP4 G_WeaponLogFrags
ADDP4
ADDP4
INDIRI4
ASGNI4
line 1660
;1660:			nWorstEnemy = i;
ADDRLP4 8
ADDRLP4 0
INDIRI4
ASGNI4
line 1661
;1661:		}
LABELV $729
line 1662
;1662:	}
LABELV $726
line 1650
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $725
line 1663
;1663:	return nWorstEnemy;
ADDRLP4 8
INDIRI4
RETI4
LABELV $721
endproc GetWorstEnemyForClient 32 0
export GetFavoriteWeaponForClient
proc GetFavoriteWeaponForClient 88 0
line 1667
;1664:}
;1665:
;1666:int GetFavoriteWeaponForClient(int nClient)
;1667:{
line 1668
;1668:	int i = 0, nMostKills = 0, fav=0, weapon=WP_STUN_BATON;
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 72
CNSTI4 0
ASGNI4
ADDRLP4 76
CNSTI4 0
ASGNI4
ADDRLP4 0
CNSTI4 1
ASGNI4
line 1675
;1669:	int	killsWithWeapon[WP_NUM_WEAPONS];
;1670:
;1671:
;1672:	// First thing we need to do is cycle through all the MOD types and convert
;1673:	// number of kills to a single weapon.
;1674:	//----------------------------------------------------------------
;1675:	for (weapon=0; weapon<WP_NUM_WEAPONS; weapon++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $733
line 1676
;1676:		killsWithWeapon[weapon] = 0;					// CLEAR
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
CNSTI4 0
ASGNI4
LABELV $734
line 1675
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $733
line 1678
;1677:
;1678:	for (i=MOD_STUN_BATON; i<=MOD_FORCE_DARK; i++)
ADDRLP4 4
CNSTI4 1
ASGNI4
LABELV $737
line 1679
;1679:	{
line 1680
;1680:		weapon = weaponFromMOD[i];									// Select Weapon
ADDRLP4 0
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 weaponFromMOD
ADDP4
INDIRI4
ASGNI4
line 1682
;1681:
;1682:		if (weapon != WP_NONE)
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $741
line 1683
;1683:		{
line 1684
;1684:			killsWithWeapon[weapon] += G_WeaponLogKills[nClient][i];	// Store Num Kills With Weapon
ADDRLP4 80
CNSTI4 2
ASGNI4
ADDRLP4 84
ADDRLP4 0
INDIRI4
ADDRLP4 80
INDIRI4
LSHI4
ADDRLP4 8
ADDP4
ASGNP4
ADDRLP4 84
INDIRP4
ADDRLP4 84
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDRLP4 80
INDIRI4
LSHI4
CNSTI4 152
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills
ADDP4
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1685
;1685:		}
LABELV $741
line 1686
;1686:	}
LABELV $738
line 1678
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 27
LEI4 $737
line 1690
;1687:
;1688:	// now look through our list of kills per weapon and pick the biggest
;1689:	//----------------------------------------------------------------
;1690:	nMostKills=0;
ADDRLP4 72
CNSTI4 0
ASGNI4
line 1691
;1691:	for (weapon=WP_STUN_BATON; weapon<WP_NUM_WEAPONS; weapon++)
ADDRLP4 0
CNSTI4 1
ASGNI4
LABELV $743
line 1692
;1692:	{
line 1693
;1693:		if (killsWithWeapon[weapon]>nMostKills)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRI4
ADDRLP4 72
INDIRI4
LEI4 $747
line 1694
;1694:		{
line 1695
;1695:			nMostKills = killsWithWeapon[weapon];
ADDRLP4 72
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRI4
ASGNI4
line 1696
;1696:			fav = weapon;
ADDRLP4 76
ADDRLP4 0
INDIRI4
ASGNI4
line 1697
;1697:		}
LABELV $747
line 1698
;1698:	}
LABELV $744
line 1691
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $743
line 1699
;1699:	return fav;
ADDRLP4 76
INDIRI4
RETI4
LABELV $732
endproc GetFavoriteWeaponForClient 88 0
export G_ClearClientLog
proc G_ClearClientLog 4 0
line 1704
;1700:}
;1701:
;1702:// kef -- if a client leaves the game, clear out all counters he may have set
;1703:void QDECL G_ClearClientLog(int client)
;1704:{
line 1705
;1705:	int i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1707
;1706:
;1707:	for (i = 0; i < WP_NUM_WEAPONS; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $750
line 1708
;1708:	{
line 1709
;1709:		G_WeaponLogPickups[client][i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogPickups
ADDP4
ADDP4
CNSTI4 0
ASGNI4
line 1710
;1710:	}
LABELV $751
line 1707
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $750
line 1711
;1711:	for (i = 0; i < WP_NUM_WEAPONS; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $754
line 1712
;1712:	{
line 1713
;1713:		G_WeaponLogFired[client][i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogFired
ADDP4
ADDP4
CNSTI4 0
ASGNI4
line 1714
;1714:	}
LABELV $755
line 1711
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $754
line 1715
;1715:	for (i = 0; i < MOD_MAX; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $758
line 1716
;1716:	{
line 1717
;1717:		G_WeaponLogDamage[client][i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 152
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogDamage
ADDP4
ADDP4
CNSTI4 0
ASGNI4
line 1718
;1718:	}
LABELV $759
line 1715
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 38
LTI4 $758
line 1719
;1719:	for (i = 0; i < MOD_MAX; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $762
line 1720
;1720:	{
line 1721
;1721:		G_WeaponLogKills[client][i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 152
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogKills
ADDP4
ADDP4
CNSTI4 0
ASGNI4
line 1722
;1722:	}
LABELV $763
line 1719
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 38
LTI4 $762
line 1723
;1723:	for (i = 0; i < WP_NUM_WEAPONS; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $766
line 1724
;1724:	{
line 1725
;1725:		G_WeaponLogDeaths[client][i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogDeaths
ADDP4
ADDP4
CNSTI4 0
ASGNI4
line 1726
;1726:	}
LABELV $767
line 1723
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $766
line 1727
;1727:	for (i = 0; i < MAX_CLIENTS; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $770
line 1728
;1728:	{
line 1729
;1729:		G_WeaponLogFrags[client][i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 7
LSHI4
ADDRGP4 G_WeaponLogFrags
ADDP4
ADDP4
CNSTI4 0
ASGNI4
line 1730
;1730:	}
LABELV $771
line 1727
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $770
line 1731
;1731:	for (i = 0; i < MAX_CLIENTS; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $774
line 1732
;1732:	{
line 1733
;1733:		G_WeaponLogFrags[i][client] = 0;
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRI4
CNSTI4 7
LSHI4
ADDRGP4 G_WeaponLogFrags
ADDP4
ADDP4
CNSTI4 0
ASGNI4
line 1734
;1734:	}
LABELV $775
line 1731
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $774
line 1735
;1735:	for (i = 0; i < WP_NUM_WEAPONS; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $778
line 1736
;1736:	{
line 1737
;1737:		G_WeaponLogTime[client][i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogTime
ADDP4
ADDP4
CNSTI4 0
ASGNI4
line 1738
;1738:	}
LABELV $779
line 1735
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $778
line 1739
;1739:	G_WeaponLogLastTime[client] = 0;
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogLastTime
ADDP4
CNSTI4 0
ASGNI4
line 1740
;1740:	G_WeaponLogClientTouch[client] = qfalse;
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 G_WeaponLogClientTouch
ADDP4
CNSTI4 0
ASGNI4
line 1741
;1741:	for (i = 0; i < HI_NUM_HOLDABLE; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $782
line 1742
;1742:	{
line 1743
;1743:		G_WeaponLogPowerups[client][i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 28
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 G_WeaponLogPowerups
ADDP4
ADDP4
CNSTI4 0
ASGNI4
line 1744
;1744:	}
LABELV $783
line 1741
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 7
LTI4 $782
line 1745
;1745:	for (i = 0; i < PW_NUM_POWERUPS; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $786
line 1746
;1746:	{
line 1747
;1747:		G_WeaponLogItems[client][i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRI4
CNSTI4 6
LSHI4
ADDRGP4 G_WeaponLogItems
ADDP4
ADDP4
CNSTI4 0
ASGNI4
line 1748
;1748:	}
LABELV $787
line 1745
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $786
line 1749
;1749:}
LABELV $749
endproc G_ClearClientLog 4 0
import modNames
import g_statLogFile
import g_statLog
bss
export G_WeaponLogItems
align 4
LABELV G_WeaponLogItems
skip 2048
export G_WeaponLogPowerups
align 4
LABELV G_WeaponLogPowerups
skip 896
export G_WeaponLogClientTouch
align 4
LABELV G_WeaponLogClientTouch
skip 128
export G_WeaponLogLastTime
align 4
LABELV G_WeaponLogLastTime
skip 128
export G_WeaponLogTime
align 4
LABELV G_WeaponLogTime
skip 2048
export G_WeaponLogFrags
align 4
LABELV G_WeaponLogFrags
skip 4096
export G_WeaponLogDeaths
align 4
LABELV G_WeaponLogDeaths
skip 2048
export G_WeaponLogKills
align 4
LABELV G_WeaponLogKills
skip 4864
export G_WeaponLogDamage
align 4
LABELV G_WeaponLogDamage
skip 4864
export G_WeaponLogFired
align 4
LABELV G_WeaponLogFired
skip 2048
export G_WeaponLogPickups
align 4
LABELV G_WeaponLogPickups
skip 2048
import trap_ROFF_Purge_Ent
import trap_ROFF_Play
import trap_ROFF_Cache
import trap_ROFF_UpdateEntities
import trap_ROFF_Clean
import trap_SP_GetStringTextString
import trap_SP_Register
import trap_SP_RegisterServer
import trap_SnapVector
import trap_GeneticParentsAndChildSelection
import trap_BotResetWeaponState
import trap_BotFreeWeaponState
import trap_BotAllocWeaponState
import trap_BotLoadWeaponWeights
import trap_BotGetWeaponInfo
import trap_BotChooseBestFightWeapon
import trap_BotAddAvoidSpot
import trap_BotInitMoveState
import trap_BotFreeMoveState
import trap_BotAllocMoveState
import trap_BotPredictVisiblePosition
import trap_BotMovementViewTarget
import trap_BotReachabilityArea
import trap_BotResetLastAvoidReach
import trap_BotResetAvoidReach
import trap_BotMoveInDirection
import trap_BotMoveToGoal
import trap_BotResetMoveState
import trap_BotFreeGoalState
import trap_BotAllocGoalState
import trap_BotMutateGoalFuzzyLogic
import trap_BotSaveGoalFuzzyLogic
import trap_BotInterbreedGoalFuzzyLogic
import trap_BotFreeItemWeights
import trap_BotLoadItemWeights
import trap_BotUpdateEntityItems
import trap_BotInitLevelItems
import trap_BotSetAvoidGoalTime
import trap_BotAvoidGoalTime
import trap_BotGetLevelItemGoal
import trap_BotGetMapLocationGoal
import trap_BotGetNextCampSpotGoal
import trap_BotItemGoalInVisButNotVisible
import trap_BotTouchingGoal
import trap_BotChooseNBGItem
import trap_BotChooseLTGItem
import trap_BotGetSecondGoal
import trap_BotGetTopGoal
import trap_BotGoalName
import trap_BotDumpGoalStack
import trap_BotDumpAvoidGoals
import trap_BotEmptyGoalStack
import trap_BotPopGoal
import trap_BotPushGoal
import trap_BotResetAvoidGoals
import trap_BotRemoveFromAvoidGoals
import trap_BotResetGoalState
import trap_BotSetChatName
import trap_BotSetChatGender
import trap_BotLoadChatFile
import trap_BotReplaceSynonyms
import trap_UnifyWhiteSpaces
import trap_BotMatchVariable
import trap_BotFindMatch
import trap_StringContains
import trap_BotGetChatMessage
import trap_BotEnterChat
import trap_BotChatLength
import trap_BotReplyChat
import trap_BotNumInitialChats
import trap_BotInitialChat
import trap_BotNumConsoleMessages
import trap_BotNextConsoleMessage
import trap_BotRemoveConsoleMessage
import trap_BotQueueConsoleMessage
import trap_BotFreeChatState
import trap_BotAllocChatState
import trap_Characteristic_String
import trap_Characteristic_BInteger
import trap_Characteristic_Integer
import trap_Characteristic_BFloat
import trap_Characteristic_Float
import trap_BotFreeCharacter
import trap_BotLoadCharacter
import trap_EA_ResetInput
import trap_EA_GetInput
import trap_EA_EndRegular
import trap_EA_ForcePower
import trap_EA_Alt_Attack
import trap_EA_View
import trap_EA_Move
import trap_EA_DelayedJump
import trap_EA_Jump
import trap_EA_SelectWeapon
import trap_EA_MoveRight
import trap_EA_MoveLeft
import trap_EA_MoveBack
import trap_EA_MoveForward
import trap_EA_MoveDown
import trap_EA_MoveUp
import trap_EA_Crouch
import trap_EA_Respawn
import trap_EA_Use
import trap_EA_Attack
import trap_EA_Talk
import trap_EA_Gesture
import trap_EA_Action
import trap_EA_Command
import trap_EA_SayTeam
import trap_EA_Say
import trap_AAS_PredictClientMovement
import trap_AAS_Swimming
import trap_AAS_AlternativeRouteGoals
import trap_AAS_PredictRoute
import trap_AAS_EnableRoutingArea
import trap_AAS_AreaTravelTimeToGoalArea
import trap_AAS_AreaReachability
import trap_AAS_IntForBSPEpairKey
import trap_AAS_FloatForBSPEpairKey
import trap_AAS_VectorForBSPEpairKey
import trap_AAS_ValueForBSPEpairKey
import trap_AAS_NextBSPEntity
import trap_AAS_PointContents
import trap_AAS_TraceAreas
import trap_AAS_PointReachabilityAreaIndex
import trap_AAS_PointAreaNum
import trap_AAS_Time
import trap_AAS_PresenceTypeBoundingBox
import trap_AAS_Initialized
import trap_AAS_EntityInfo
import trap_AAS_AreaInfo
import trap_AAS_BBoxAreas
import trap_BotUserCommand
import trap_BotGetServerCommand
import trap_BotGetSnapshotEntity
import trap_BotLibTest
import trap_BotLibUpdateEntity
import trap_BotLibLoadMap
import trap_BotLibStartFrame
import trap_BotLibDefine
import trap_BotLibVarGet
import trap_BotLibVarSet
import trap_BotLibShutdown
import trap_BotLibSetup
import trap_DebugPolygonDelete
import trap_DebugPolygonCreate
import trap_GetEntityToken
import trap_GetUsercmd
import trap_BotFreeClient
import trap_BotAllocateClient
import trap_EntityContact
import trap_EntitiesInBox
import trap_UnlinkEntity
import trap_LinkEntity
import trap_AreasConnected
import trap_AdjustAreaPortalState
import trap_InPVSIgnorePortals
import trap_InPVS
import trap_PointContents
import trap_Trace
import trap_SetBrushModel
import trap_GetServerinfo
import trap_SetUserinfo
import trap_GetUserinfo
import trap_GetConfigstring
import trap_SetConfigstring
import trap_SendServerCommand
import trap_DropClient
import trap_LocateGameData
import trap_Cvar_VariableStringBuffer
import trap_Cvar_VariableValue
import trap_Cvar_VariableIntegerValue
import trap_Cvar_Set
import trap_Cvar_Update
import trap_Cvar_Register
import trap_SendConsoleCommand
import trap_FS_GetFileList
import trap_FS_FCloseFile
import trap_FS_Write
import trap_FS_Read
import trap_FS_FOpenFile
import trap_Args
import trap_Argv
import trap_Argc
import trap_Milliseconds
import trap_Error
import trap_Printf
import g_timeouttospec
import g_forceDodge
import g_dismember
import g_singlePlayer
import g_enableBreath
import g_enableDust
import g_rankings
import pmove_msec
import pmove_fixed
import g_smoothClients
import g_blueteam
import g_redteam
import g_debugUp
import g_debugRight
import g_debugForward
import g_filterBan
import g_banIPs
import g_teamForceBalance
import g_teamAutoJoin
import g_allowVote
import g_blood
import g_doWarmup
import g_warmup
import g_motd
import g_synchronousClients
import g_adaptRespawn
import g_weaponTeamRespawn
import g_weaponRespawn
import g_debugDamage
import g_debugAlloc
import g_debugMove
import g_inactivity
import g_forcerespawn
import g_quadfactor
import g_knockback
import g_speed
import g_gravity
import g_needpass
import g_password
import g_friendlySaber
import g_friendlyFire
import g_saberInterpolate
import g_capturelimit
import g_timelimit
import g_duel_fraglimit
import g_fraglimit
import g_duelWeaponDisable
import g_weaponDisable
import g_forcePowerDisable
import g_spawnInvulnerability
import g_forceRegenTime
import g_saberLocking
import g_privateDuel
import g_forceBasedTeams
import g_maxForceRank
import g_dmflags
import g_autoMapCycle
import g_restarted
import g_maxGameClients
import g_maxclients
import g_cheats
import g_dedicated
import g_gametype
import g_entities
import level
import Pickup_Team
import CheckTeamStatus
import TeamplayInfoMessage
import Team_GetLocationMsg
import Team_GetLocation
import SelectSagaSpawnPoint
import SelectCTFSpawnPoint
import Team_FreeEntity
import Team_ReturnFlag
import Team_InitGame
import Team_CheckHurtCarrier
import Team_FragBonuses
import Team_DroppedFlagThink
import AddTeamScore
import TeamColorString
import OtherTeamName
import TeamName
import OtherTeam
import BotAIStartFrame
import BotAIShutdownClient
import BotAISetupClient
import BotAILoadMap
import BotAIShutdown
import BotAISetup
import B_CleanupAlloc
import B_InitAlloc
import InFieldOfVision
import BotOrder
import OrgVisible
import InitSagaMode
import G_LogExit
import Jedi_DodgeEvasion
import ForceTelepathy
import ForceThrow
import ForceSeeing
import ForceTeamForceReplenish
import ForceTeamHeal
import ForceAbsorb
import ForceProtect
import ForceGrip
import ForceRage
import ForceSpeed
import ForceHeal
import ForcePowerUsableOn
import WP_ForcePowersUpdate
import WP_SpawnInitForcePowers
import WP_InitForcePowers
import WP_SaberInitBladeData
import WP_SaberCanBlock
import WP_SaberPositionUpdate
import WP_ForcePowerStop
import HasSetSaberOnly
import G_PreDefSound
import G_RefreshNextMap
import G_DoesMapSupportGametype
import BotInterbreedEndMatch
import Svcmd_BotList_f
import Svcmd_AddBot_f
import G_BotConnect
import G_RemoveQueuedBotBegin
import G_CheckBotSpawn
import G_GetBotInfoByName
import G_GetBotInfoByNumber
import G_InitBots
import UpdateTournamentInfo
import G_WriteSessionData
import G_InitWorldSession
import G_InitSessionData
import G_ReadSessionData
import Svcmd_GameMem_f
import G_InitMemory
import G_Alloc
import Team_CheckDroppedItem
import OnSameTeam
import G_RunClient
import ClientEndFrame
import ClientThink
import G_CheckClientTimeouts
import ClientCommand
import ClientBegin
import ClientDisconnect
import ClientUserinfoChanged
import ClientConnect
import G_GetStripEdString
import G_Error
import G_Printf
import SendScoreboardMessageToAllClients
import G_LogPrintf
import G_RunThink
import CheckTeamLeader
import SetLeader
import FindIntermissionPoint
import g_ff_objectives
import DeathmatchScoreboardMessage
import G_SetStats
import MoveClientToIntermission
import BlowDetpacks
import FireWeapon
import G_FilterPacket
import G_ProcessIPBans
import ConsoleCommand
import gJMSaberEnt
import SpotWouldTelefrag
import CalculateRanks
import AddScore
import player_die
import ClientSpawn
import InitBodyQue
import BeginIntermission
import respawn
import CopyToBodyQue
import SelectSpawnPoint
import SetClientViewAngle
import PickTeam
import TeamLeader
import TeamCount
import CheckGauntletAttack
import SnapVectorTowards
import CalcMuzzlePoint
import LogAccuracyHit
import WP_FireGenericBlasterMissile
import WP_FireTurretMissile
import G_PlayerBecomeATST
import ATST_ManageDamageBoxes
import TeleportPlayer
import trigger_teleporter_touch
import Touch_DoorTrigger
import G_RunMover
import WP_FireBlasterMissile
import G_ExplodeMissile
import G_BounceProjectile
import CreateMissile
import G_RunMissile
import G_ReflectMissile
import ExplodeDeath
import TossClientCubes
import TossClientItems
import TossClientWeapon
import body_die
import G_RadiusDamage
import G_Damage
import CanDamage
import trap_G2API_SetBoneAnim
import trap_G2API_GetGLAName
import trap_G2API_SetBoneAngles
import trap_G2API_CleanGhoul2Models
import trap_G2API_RemoveGhoul2Model
import trap_G2API_HasGhoul2ModelOnIndex
import trap_G2API_DuplicateGhoul2Instance
import trap_G2API_CopySpecificGhoul2Model
import trap_G2API_CopyGhoul2Instance
import trap_G2API_SetBoltInfo
import trap_G2API_AddBolt
import trap_G2API_InitGhoul2Model
import trap_G2API_GetBoltMatrix_NoReconstruct
import trap_G2API_GetBoltMatrix
import trap_G2_HaveWeGhoul2Models
import trap_G2_SetGhoul2ModelIndexes
import trap_G2_ListModelBones
import trap_G2_ListModelSurfaces
import G_SkinIndex
import BuildShaderStateConfig
import AddRemap
import G_SetOrigin
import G_AddEvent
import G_AddPredictableEvent
import vtos
import tv
import G_RunObject
import G_TouchSolids
import G_TouchTriggers
import G_EntitiesFree
import G_FreeEntity
import G_KillG2Queue
import G_SendG2KillQueue
import TryUse
import G_EntitySound
import G_SoundAtLoc
import G_Sound
import G_MuteSound
import G_ScreenShake
import G_PlayEffect
import G_TempEntity
import G_Spawn
import G_InitGentity
import G_SetAngles
import G_SetMovedir
import G_UseTargets
import G_PickTarget
import G_RadiusList
import G_Find
import G_KillBox
import G_TeamCommand
import G_EffectIndex
import G_SoundIndex
import G_ModelIndex
import SaveRegisteredItems
import RegisterItem
import ClearRegisteredItems
import Touch_Item
import Add_Ammo
import ArmorIndex
import Think_Weapon
import FinishSpawningItem
import G_SpawnItem
import SetRespawn
import LaunchItem
import Drop_Item
import PrecacheItem
import UseHoldableItem
import RespawnItem
import G_RunItem
import G_CheckTeamItems
import ItemUse_MedPack
import ItemUse_Seeker
import ItemUse_Sentry
import ItemUse_Shield
import ItemUse_Binoculars
import G_GetDuelWinner
import Cmd_EngageDuel_f
import Cmd_ToggleSaber_f
import G_ItemUsable
import Cmd_SaberAttackCycle_f
import Cmd_FollowCycle_f
import SetTeam
import BroadcastTeamChange
import StopFollowing
import Cmd_Score_f
import G_NewString
import G_SpawnEntitiesFromString
import G_SpawnVector
import G_SpawnInt
import G_SpawnFloat
import G_SpawnString
import g2SaberInstance
import precachedKyle
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
LABELV $690
char 1 37
char 1 115
char 1 32
char 1 37
char 1 100
char 1 37
char 1 115
char 1 0
align 1
LABELV $672
char 1 37
char 1 115
char 1 32
char 1 37
char 1 100
char 1 0
align 1
LABELV $669
char 1 32
char 1 37
char 1 100
char 1 0
align 1
LABELV $351
char 1 68
char 1 97
char 1 109
char 1 97
char 1 103
char 1 101
char 1 45
char 1 84
char 1 121
char 1 112
char 1 101
char 1 100
char 1 32
char 1 75
char 1 105
char 1 108
char 1 108
char 1 115
char 1 32
char 1 112
char 1 101
char 1 114
char 1 32
char 1 80
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 58
char 1 10
char 1 10
char 1 0
align 1
LABELV $328
char 1 84
char 1 121
char 1 112
char 1 101
char 1 100
char 1 32
char 1 68
char 1 97
char 1 109
char 1 97
char 1 103
char 1 101
char 1 32
char 1 112
char 1 101
char 1 114
char 1 32
char 1 80
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 58
char 1 10
char 1 10
char 1 0
align 1
LABELV $299
char 1 87
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 32
char 1 75
char 1 105
char 1 108
char 1 108
char 1 115
char 1 32
char 1 112
char 1 101
char 1 114
char 1 32
char 1 80
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 58
char 1 10
char 1 10
char 1 0
align 1
LABELV $270
char 1 87
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 32
char 1 68
char 1 97
char 1 109
char 1 97
char 1 103
char 1 101
char 1 32
char 1 112
char 1 101
char 1 114
char 1 32
char 1 80
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 58
char 1 10
char 1 10
char 1 0
align 1
LABELV $247
char 1 87
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 32
char 1 68
char 1 101
char 1 97
char 1 116
char 1 104
char 1 115
char 1 32
char 1 112
char 1 101
char 1 114
char 1 32
char 1 80
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 58
char 1 10
char 1 10
char 1 0
align 1
LABELV $224
char 1 87
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 32
char 1 85
char 1 115
char 1 101
char 1 32
char 1 84
char 1 105
char 1 109
char 1 101
char 1 32
char 1 112
char 1 101
char 1 114
char 1 32
char 1 80
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 58
char 1 10
char 1 10
char 1 0
align 1
LABELV $201
char 1 87
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 32
char 1 83
char 1 104
char 1 111
char 1 116
char 1 115
char 1 32
char 1 112
char 1 101
char 1 114
char 1 32
char 1 80
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 58
char 1 10
char 1 10
char 1 0
align 1
LABELV $200
char 1 10
char 1 10
char 1 10
char 1 0
align 1
LABELV $195
char 1 10
char 1 42
char 1 42
char 1 42
char 1 84
char 1 79
char 1 84
char 1 65
char 1 76
char 1 58
char 1 0
align 1
LABELV $194
char 1 9
char 1 37
char 1 100
char 1 0
align 1
LABELV $179
char 1 9
char 1 37
char 1 115
char 1 0
align 1
LABELV $174
char 1 80
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 0
align 1
LABELV $173
char 1 87
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 32
char 1 80
char 1 105
char 1 99
char 1 107
char 1 117
char 1 112
char 1 115
char 1 32
char 1 112
char 1 101
char 1 114
char 1 32
char 1 80
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 58
char 1 10
char 1 10
char 1 0
align 1
LABELV $172
char 1 10
char 1 10
char 1 10
char 1 76
char 1 101
char 1 118
char 1 101
char 1 108
char 1 58
char 1 9
char 1 37
char 1 115
char 1 10
char 1 10
char 1 10
char 1 0
align 1
LABELV $170
char 1 109
char 1 97
char 1 112
char 1 110
char 1 97
char 1 109
char 1 101
char 1 0
align 1
LABELV $166
char 1 10
char 1 0
align 1
LABELV $165
char 1 37
char 1 50
char 1 53
char 1 115
char 1 58
char 1 32
char 1 32
char 1 68
char 1 97
char 1 109
char 1 97
char 1 103
char 1 101
char 1 58
char 1 32
char 1 37
char 1 54
char 1 100
char 1 44
char 1 32
char 1 32
char 1 75
char 1 105
char 1 108
char 1 108
char 1 115
char 1 58
char 1 32
char 1 37
char 1 53
char 1 100
char 1 10
char 1 0
align 1
LABELV $160
char 1 10
char 1 42
char 1 42
char 1 42
char 1 42
char 1 67
char 1 111
char 1 109
char 1 98
char 1 97
char 1 116
char 1 32
char 1 68
char 1 97
char 1 116
char 1 97
char 1 32
char 1 66
char 1 121
char 1 32
char 1 68
char 1 97
char 1 109
char 1 97
char 1 103
char 1 101
char 1 32
char 1 84
char 1 121
char 1 112
char 1 101
char 1 58
char 1 10
char 1 0
align 1
LABELV $159
char 1 37
char 1 49
char 1 53
char 1 115
char 1 58
char 1 32
char 1 32
char 1 68
char 1 97
char 1 109
char 1 97
char 1 103
char 1 101
char 1 58
char 1 32
char 1 37
char 1 54
char 1 100
char 1 44
char 1 32
char 1 32
char 1 75
char 1 105
char 1 108
char 1 108
char 1 115
char 1 58
char 1 32
char 1 37
char 1 53
char 1 100
char 1 44
char 1 32
char 1 32
char 1 68
char 1 109
char 1 103
char 1 32
char 1 112
char 1 101
char 1 114
char 1 32
char 1 83
char 1 104
char 1 111
char 1 116
char 1 58
char 1 32
char 1 37
char 1 102
char 1 10
char 1 0
align 1
LABELV $152
char 1 10
char 1 42
char 1 42
char 1 42
char 1 42
char 1 67
char 1 111
char 1 109
char 1 98
char 1 97
char 1 116
char 1 32
char 1 68
char 1 97
char 1 116
char 1 97
char 1 32
char 1 98
char 1 121
char 1 32
char 1 87
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 58
char 1 10
char 1 0
align 1
LABELV $151
char 1 37
char 1 49
char 1 53
char 1 115
char 1 58
char 1 32
char 1 32
char 1 80
char 1 105
char 1 99
char 1 107
char 1 117
char 1 112
char 1 115
char 1 58
char 1 32
char 1 37
char 1 52
char 1 100
char 1 44
char 1 32
char 1 32
char 1 84
char 1 105
char 1 109
char 1 101
char 1 58
char 1 32
char 1 32
char 1 37
char 1 53
char 1 100
char 1 44
char 1 32
char 1 32
char 1 68
char 1 101
char 1 97
char 1 116
char 1 104
char 1 115
char 1 58
char 1 32
char 1 37
char 1 53
char 1 100
char 1 10
char 1 0
align 1
LABELV $146
char 1 10
char 1 42
char 1 42
char 1 42
char 1 42
char 1 68
char 1 97
char 1 116
char 1 97
char 1 32
char 1 98
char 1 121
char 1 32
char 1 87
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 58
char 1 10
char 1 0
align 1
LABELV $125
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 42
char 1 87
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 32
char 1 76
char 1 111
char 1 103
char 1 58
char 1 10
char 1 0
align 1
LABELV $121
char 1 60
char 1 85
char 1 110
char 1 107
char 1 110
char 1 111
char 1 119
char 1 110
char 1 62
char 1 0
align 1
LABELV $93
char 1 84
char 1 117
char 1 114
char 1 114
char 1 101
char 1 116
char 1 0
align 1
LABELV $92
char 1 69
char 1 109
char 1 112
char 1 108
char 1 97
char 1 99
char 1 101
char 1 100
char 1 32
char 1 103
char 1 117
char 1 110
char 1 0
align 1
LABELV $91
char 1 68
char 1 101
char 1 116
char 1 112
char 1 97
char 1 99
char 1 107
char 1 0
align 1
LABELV $90
char 1 84
char 1 114
char 1 105
char 1 112
char 1 109
char 1 105
char 1 110
char 1 101
char 1 0
align 1
LABELV $89
char 1 84
char 1 104
char 1 101
char 1 114
char 1 109
char 1 97
char 1 108
char 1 0
align 1
LABELV $88
char 1 82
char 1 111
char 1 99
char 1 107
char 1 101
char 1 116
char 1 32
char 1 76
char 1 97
char 1 117
char 1 110
char 1 99
char 1 104
char 1 101
char 1 114
char 1 0
align 1
LABELV $87
char 1 70
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
LABELV $86
char 1 68
char 1 101
char 1 109
char 1 112
char 1 50
char 1 0
align 1
LABELV $85
char 1 82
char 1 101
char 1 112
char 1 101
char 1 97
char 1 116
char 1 101
char 1 114
char 1 0
align 1
LABELV $84
char 1 66
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
LABELV $83
char 1 68
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
LABELV $82
char 1 66
char 1 108
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 0
align 1
LABELV $81
char 1 66
char 1 114
char 1 121
char 1 97
char 1 114
char 1 32
char 1 80
char 1 105
char 1 115
char 1 116
char 1 111
char 1 108
char 1 0
align 1
LABELV $80
char 1 83
char 1 97
char 1 98
char 1 101
char 1 114
char 1 0
align 1
LABELV $79
char 1 83
char 1 116
char 1 117
char 1 110
char 1 32
char 1 66
char 1 97
char 1 116
char 1 111
char 1 110
char 1 0
align 1
LABELV $78
char 1 78
char 1 111
char 1 32
char 1 87
char 1 101
char 1 97
char 1 112
char 1 111
char 1 110
char 1 0
