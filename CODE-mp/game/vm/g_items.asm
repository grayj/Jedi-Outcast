export adjustRespawnTime
code
proc adjustRespawnTime 8 0
file "../g_items.c"
line 36
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:#include "g_local.h"
;4:
;5:/*
;6:
;7:  Items are any object that a player can touch to gain some effect.
;8:
;9:  Pickup will return the number of seconds until they should respawn.
;10:
;11:  all items should pop when dropped in lava or slime
;12:
;13:  Respawnable items don't actually go away when picked up, they are
;14:  just made invisible and untouchable.  This allows them to ride
;15:  movers and respawn apropriately.
;16:*/
;17:
;18:
;19:#define	RESPAWN_ARMOR		20
;20:#define	RESPAWN_TEAM_WEAPON	30
;21:#define	RESPAWN_HEALTH		30
;22:#define	RESPAWN_AMMO		40
;23:#define	RESPAWN_HOLDABLE	60
;24:#define	RESPAWN_MEGAHEALTH	120
;25:#define	RESPAWN_POWERUP		120
;26:
;27:extern gentity_t *droppedRedFlag;
;28:extern gentity_t *droppedBlueFlag;
;29:
;30://======================================================================
;31:#define MAX_MEDPACK_HEAL_AMOUNT		100
;32:#define MAX_SENTRY_DISTANCE			256
;33:
;34:// For more than four players, adjust the respawn times, up to 1/4.
;35:int adjustRespawnTime(float preRespawnTime, int itemType, int itemTag)
;36:{
line 37
;37:	float respawnTime = preRespawnTime;
ADDRLP4 0
ADDRFP4 0
INDIRF4
ASGNF4
line 39
;38:
;39:	if (itemType == IT_WEAPON)
ADDRFP4 4
INDIRI4
CNSTI4 1
NEI4 $79
line 40
;40:	{
line 41
;41:		if (itemTag == WP_THERMAL ||
ADDRLP4 4
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 11
EQI4 $84
ADDRLP4 4
INDIRI4
CNSTI4 12
EQI4 $84
ADDRLP4 4
INDIRI4
CNSTI4 13
NEI4 $81
LABELV $84
line 44
;42:			itemTag == WP_TRIP_MINE ||
;43:			itemTag == WP_DET_PACK)
;44:		{ //special case for these, use ammo respawn rate
line 45
;45:			respawnTime = RESPAWN_AMMO;
ADDRLP4 0
CNSTF4 1109393408
ASGNF4
line 46
;46:		}
LABELV $81
line 47
;47:	}
LABELV $79
line 49
;48:
;49:	if (!g_adaptRespawn.integer)
ADDRGP4 g_adaptRespawn+12
INDIRI4
CNSTI4 0
NEI4 $85
line 50
;50:	{
line 51
;51:		return((int)respawnTime);
ADDRLP4 0
INDIRF4
CVFI4 4
RETI4
ADDRGP4 $78
JUMPV
LABELV $85
line 54
;52:	}
;53:
;54:	if (level.numPlayingClients > 4)
ADDRGP4 level+80
INDIRI4
CNSTI4 4
LEI4 $88
line 55
;55:	{	// Start scaling the respawn times.
line 56
;56:		if (level.numPlayingClients > 32)
ADDRGP4 level+80
INDIRI4
CNSTI4 32
LEI4 $91
line 57
;57:		{	// 1/4 time minimum.
line 58
;58:			respawnTime *= 0.25;
ADDRLP4 0
CNSTF4 1048576000
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
line 59
;59:		}
ADDRGP4 $92
JUMPV
LABELV $91
line 60
;60:		else if (level.numPlayingClients > 12)
ADDRGP4 level+80
INDIRI4
CNSTI4 12
LEI4 $94
line 61
;61:		{	// From 12-32, scale from 0.5 to 0.25;
line 62
;62:			respawnTime *= 20.0 / (float)(level.numPlayingClients + 8);
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1101004800
ADDRGP4 level+80
INDIRI4
CNSTI4 8
ADDI4
CVIF4 4
DIVF4
MULF4
ASGNF4
line 63
;63:		}
ADDRGP4 $95
JUMPV
LABELV $94
line 65
;64:		else 
;65:		{	// From 4-12, scale from 1.0 to 0.5;
line 66
;66:			respawnTime *= 8.0 / (float)(level.numPlayingClients + 4);
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1090519040
ADDRGP4 level+80
INDIRI4
CNSTI4 4
ADDI4
CVIF4 4
DIVF4
MULF4
ASGNF4
line 67
;67:		}
LABELV $95
LABELV $92
line 68
;68:	}
LABELV $88
line 70
;69:
;70:	if (respawnTime < 1.0)
ADDRLP4 0
INDIRF4
CNSTF4 1065353216
GEF4 $99
line 71
;71:	{	// No matter what, don't go lower than 1 second, or the pickups become very noisy!
line 72
;72:		respawnTime = 1.0;
ADDRLP4 0
CNSTF4 1065353216
ASGNF4
line 73
;73:	}
LABELV $99
line 75
;74:
;75:	return ((int)respawnTime);
ADDRLP4 0
INDIRF4
CVFI4 4
RETI4
LABELV $78
endproc adjustRespawnTime 8 0
data
align 4
LABELV shieldLoopSound
byte 4 0
align 4
LABELV shieldAttachSound
byte 4 0
align 4
LABELV shieldActivateSound
byte 4 0
align 4
LABELV shieldDeactivateSound
byte 4 0
align 4
LABELV shieldDamageSound
byte 4 0
export ShieldRemove
code
proc ShieldRemove 0 12
line 94
;76:}
;77:
;78:
;79:#define SHIELD_HEALTH				250
;80:#define SHIELD_HEALTH_DEC			10		// 25 seconds	
;81:#define MAX_SHIELD_HEIGHT			254
;82:#define MAX_SHIELD_HALFWIDTH		255
;83:#define SHIELD_HALFTHICKNESS		4
;84:#define SHIELD_PLACEDIST			64
;85:
;86:static qhandle_t	shieldLoopSound=0;
;87:static qhandle_t	shieldAttachSound=0;
;88:static qhandle_t	shieldActivateSound=0;
;89:static qhandle_t	shieldDeactivateSound=0;
;90:static qhandle_t	shieldDamageSound=0;
;91:
;92:
;93:void ShieldRemove(gentity_t *self)
;94:{
line 95
;95:	self->think = G_FreeEntity;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 G_FreeEntity
ASGNP4
line 96
;96:	self->nextthink = level.time + 100;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 99
;97:
;98:	// Play kill sound...
;99:	G_AddEvent(self, EV_GENERAL_SOUND, shieldDeactivateSound);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 66
ARGI4
ADDRGP4 shieldDeactivateSound
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 100
;100:	self->s.loopSound = 0;
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
CNSTI4 0
ASGNI4
line 102
;101:
;102:	return;
LABELV $101
endproc ShieldRemove 0 12
export ShieldThink
proc ShieldThink 4 4
line 108
;103:}
;104:
;105:
;106:// Count down the health of the shield.
;107:void ShieldThink(gentity_t *self)
;108:{
line 109
;109:	self->s.trickedentindex = 0;
ADDRFP4 0
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 0
ASGNI4
line 111
;110:
;111:	self->health -= SHIELD_HEALTH_DEC;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 10
SUBI4
ASGNI4
line 112
;112:	self->nextthink = level.time + 1000;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1000
ADDI4
ASGNI4
line 113
;113:	if (self->health <= 0)
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 0
GTI4 $103
line 114
;114:	{
line 115
;115:		ShieldRemove(self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 ShieldRemove
CALLV
pop
line 116
;116:	}
line 117
;117:	return;
LABELV $103
endproc ShieldThink 4 4
export ShieldDie
proc ShieldDie 0 12
line 123
;118:}
;119:
;120:
;121:// The shield was damaged to below zero health.
;122:void ShieldDie(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod)
;123:{
line 125
;124:	// Play damaging sound...
;125:	G_AddEvent(self, EV_GENERAL_SOUND, shieldDamageSound);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 66
ARGI4
ADDRGP4 shieldDamageSound
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 127
;126:
;127:	ShieldRemove(self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 ShieldRemove
CALLV
pop
line 128
;128:}
LABELV $107
endproc ShieldDie 0 12
export ShieldPain
proc ShieldPain 0 12
line 133
;129:
;130:
;131:// The shield had damage done to it.  Make it flicker.
;132:void ShieldPain(gentity_t *self, gentity_t *attacker, int damage)
;133:{
line 135
;134:	// Set the itemplaceholder flag to indicate the the shield drawing that the shield pain should be drawn.
;135:	self->think = ShieldThink;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 ShieldThink
ASGNP4
line 136
;136:	self->nextthink = level.time + 400;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 400
ADDI4
ASGNI4
line 139
;137:
;138:	// Play damaging sound...
;139:	G_AddEvent(self, EV_GENERAL_SOUND, shieldDamageSound);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 66
ARGI4
ADDRGP4 shieldDamageSound
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 141
;140:
;141:	self->s.trickedentindex = 1;
ADDRFP4 0
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 1
ASGNI4
line 143
;142:
;143:	return;
LABELV $108
endproc ShieldPain 0 12
export ShieldGoSolid
proc ShieldGoSolid 1096 28
line 149
;144:}
;145:
;146:
;147:// Try to turn the shield back on after a delay.
;148:void ShieldGoSolid(gentity_t *self)
;149:{
line 153
;150:	trace_t		tr;
;151:
;152:	// see if we're valid
;153:	self->health--;
ADDRLP4 1080
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
ASGNP4
ADDRLP4 1080
INDIRP4
ADDRLP4 1080
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 154
;154:	if (self->health <= 0)
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 0
GTI4 $111
line 155
;155:	{
line 156
;156:		ShieldRemove(self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 ShieldRemove
CALLV
pop
line 157
;157:		return;
ADDRGP4 $110
JUMPV
LABELV $111
line 160
;158:	}
;159:	
;160:	trap_Trace (&tr, self->r.currentOrigin, self->r.mins, self->r.maxs, self->r.currentOrigin, self->s.number, CONTENTS_BODY );
ADDRLP4 0
ARGP4
ADDRLP4 1084
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1088
ADDRLP4 1084
INDIRP4
CNSTI4 368
ADDP4
ASGNP4
ADDRLP4 1088
INDIRP4
ARGP4
ADDRLP4 1084
INDIRP4
CNSTI4 316
ADDP4
ARGP4
ADDRLP4 1084
INDIRP4
CNSTI4 328
ADDP4
ARGP4
ADDRLP4 1088
INDIRP4
ARGP4
ADDRLP4 1084
INDIRP4
INDIRI4
ARGI4
CNSTI4 256
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 161
;161:	if(tr.startsolid)
ADDRLP4 0+4
INDIRI4
CNSTI4 0
EQI4 $113
line 162
;162:	{	// gah, we can't activate yet
line 163
;163:		self->nextthink = level.time + 200;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 200
ADDI4
ASGNI4
line 164
;164:		self->think = ShieldGoSolid;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 ShieldGoSolid
ASGNP4
line 165
;165:		trap_LinkEntity(self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 166
;166:	}
ADDRGP4 $110
JUMPV
LABELV $113
line 168
;167:	else
;168:	{ // get hard... huh-huh...
line 169
;169:		self->s.eFlags &= ~EF_NODRAW;
ADDRLP4 1092
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 1092
INDIRP4
ADDRLP4 1092
INDIRP4
INDIRI4
CNSTI4 -129
BANDI4
ASGNI4
line 171
;170:
;171:		self->r.contents = CONTENTS_SOLID;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1
ASGNI4
line 172
;172:		self->nextthink = level.time + 1000;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1000
ADDI4
ASGNI4
line 173
;173:		self->think = ShieldThink;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 ShieldThink
ASGNP4
line 174
;174:		self->takedamage = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 1
ASGNI4
line 175
;175:		trap_LinkEntity(self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 178
;176:
;177:		// Play raising sound...
;178:		G_AddEvent(self, EV_GENERAL_SOUND, shieldActivateSound);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 66
ARGI4
ADDRGP4 shieldActivateSound
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 179
;179:		self->s.loopSound = shieldLoopSound;
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
ADDRGP4 shieldLoopSound
INDIRI4
ASGNI4
line 180
;180:	}
line 182
;181:
;182:	return;
LABELV $110
endproc ShieldGoSolid 1096 28
export ShieldGoNotSolid
proc ShieldGoNotSolid 4 12
line 188
;183:}
;184:
;185:
;186:// Turn the shield off to allow a friend to pass through.
;187:void ShieldGoNotSolid(gentity_t *self)
;188:{
line 190
;189:	// make the shield non-solid very briefly
;190:	self->r.contents = 0;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 0
ASGNI4
line 191
;191:	self->s.eFlags |= EF_NODRAW;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 128
BORI4
ASGNI4
line 193
;192:	// nextthink needs to have a large enough interval to avoid excess accumulation of Activate messages
;193:	self->nextthink = level.time + 200;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 200
ADDI4
ASGNI4
line 194
;194:	self->think = ShieldGoSolid;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 ShieldGoSolid
ASGNP4
line 195
;195:	self->takedamage = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 0
ASGNI4
line 196
;196:	trap_LinkEntity(self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 199
;197:
;198:	// Play kill sound...
;199:	G_AddEvent(self, EV_GENERAL_SOUND, shieldDeactivateSound);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 66
ARGI4
ADDRGP4 shieldDeactivateSound
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 200
;200:	self->s.loopSound = 0;
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
CNSTI4 0
ASGNI4
line 201
;201:}
LABELV $118
endproc ShieldGoNotSolid 4 12
export ShieldTouch
proc ShieldTouch 16 8
line 206
;202:
;203:
;204:// Somebody (a player) has touched the shield.  See if it is a "friend".
;205:void ShieldTouch(gentity_t *self, gentity_t *other, trace_t *trace)
;206:{
line 207
;207:	if (g_gametype.integer >= GT_TEAM)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
LTI4 $121
line 208
;208:	{ // let teammates through
line 210
;209:		// compare the parent's team to the "other's" team
;210:		if (self->parent && ( self->parent->client) && (other->client))
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
EQU4 $122
ADDRLP4 8
CNSTI4 408
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
EQU4 $122
ADDRFP4 4
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
EQU4 $122
line 211
;211:		{
line 212
;212:			if (OnSameTeam(self->parent, other))
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 OnSameTeam
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $122
line 213
;213:			{
line 214
;214:				ShieldGoNotSolid(self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 ShieldGoNotSolid
CALLV
pop
line 215
;215:			}
line 216
;216:		}
line 217
;217:	}
ADDRGP4 $122
JUMPV
LABELV $121
line 219
;218:	else
;219:	{//let the person who dropped the shield through
line 220
;220:		if (self->parent && self->parent->s.number == other->s.number)
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $128
ADDRLP4 0
INDIRP4
INDIRI4
ADDRFP4 4
INDIRP4
INDIRI4
NEI4 $128
line 221
;221:		{
line 222
;222:			ShieldGoNotSolid(self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 ShieldGoNotSolid
CALLV
pop
line 223
;223:		}
LABELV $128
line 224
;224:	}
LABELV $122
line 225
;225:}
LABELV $120
endproc ShieldTouch 16 8
bss
align 4
LABELV $131
skip 4
export CreateShield
code
proc CreateShield 1224 28
line 230
;226:
;227:
;228:// After a short delay, create the shield by expanding in all directions.
;229:void CreateShield(gentity_t *ent)
;230:{
line 233
;231:	trace_t		tr;
;232:	vec3_t		mins, maxs, end, posTraceEnd, negTraceEnd, start;
;233:	int			height, posWidth, negWidth, halfWidth = 0;
ADDRLP4 1084
CNSTI4 0
ASGNI4
line 235
;234:	qboolean	xaxis;
;235:	int			paramData = 0;
ADDRLP4 1172
CNSTI4 0
ASGNI4
line 239
;236:	static int	shieldID;
;237:
;238:	// trace upward to find height of shield
;239:	VectorCopy(ent->r.currentOrigin, end);
ADDRLP4 1132
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 240
;240:	end[2] += MAX_SHIELD_HEIGHT;
ADDRLP4 1132+8
ADDRLP4 1132+8
INDIRF4
CNSTF4 1132331008
ADDF4
ASGNF4
line 241
;241:	trap_Trace (&tr, ent->r.currentOrigin, NULL, NULL, end, ent->s.number, MASK_SHOT );
ADDRLP4 0
ARGP4
ADDRLP4 1176
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1176
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 1180
CNSTP4 0
ASGNP4
ADDRLP4 1180
INDIRP4
ARGP4
ADDRLP4 1180
INDIRP4
ARGP4
ADDRLP4 1132
ARGP4
ADDRLP4 1176
INDIRP4
INDIRI4
ARGI4
CNSTI4 769
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 242
;242:	height = (int)(MAX_SHIELD_HEIGHT * tr.fraction);
ADDRLP4 1080
CNSTF4 1132331008
ADDRLP4 0+8
INDIRF4
MULF4
CVFI4 4
ASGNI4
line 245
;243:
;244:	// use angles to find the proper axis along which to align the shield
;245:	VectorSet(mins, -SHIELD_HALFTHICKNESS, -SHIELD_HALFTHICKNESS, 0);
ADDRLP4 1184
CNSTF4 3229614080
ASGNF4
ADDRLP4 1108
ADDRLP4 1184
INDIRF4
ASGNF4
ADDRLP4 1108+4
ADDRLP4 1184
INDIRF4
ASGNF4
ADDRLP4 1108+8
CNSTF4 0
ASGNF4
line 246
;246:	VectorSet(maxs, SHIELD_HALFTHICKNESS, SHIELD_HALFTHICKNESS, height);
ADDRLP4 1188
CNSTF4 1082130432
ASGNF4
ADDRLP4 1120
ADDRLP4 1188
INDIRF4
ASGNF4
ADDRLP4 1120+4
ADDRLP4 1188
INDIRF4
ASGNF4
ADDRLP4 1120+8
ADDRLP4 1080
INDIRI4
CVIF4 4
ASGNF4
line 247
;247:	VectorCopy(ent->r.currentOrigin, posTraceEnd);
ADDRLP4 1144
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 248
;248:	VectorCopy(ent->r.currentOrigin, negTraceEnd);
ADDRLP4 1156
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 250
;249:
;250:	if ((int)(ent->s.angles[YAW]) == 0) // shield runs along y-axis
ADDRFP4 0
INDIRP4
CNSTI4 120
ADDP4
INDIRF4
CVFI4 4
CNSTI4 0
NEI4 $138
line 251
;251:	{
line 252
;252:		posTraceEnd[1]+=MAX_SHIELD_HALFWIDTH;
ADDRLP4 1144+4
ADDRLP4 1144+4
INDIRF4
CNSTF4 1132396544
ADDF4
ASGNF4
line 253
;253:		negTraceEnd[1]-=MAX_SHIELD_HALFWIDTH;
ADDRLP4 1156+4
ADDRLP4 1156+4
INDIRF4
CNSTF4 1132396544
SUBF4
ASGNF4
line 254
;254:		xaxis = qfalse;
ADDRLP4 1104
CNSTI4 0
ASGNI4
line 255
;255:	}
ADDRGP4 $139
JUMPV
LABELV $138
line 257
;256:	else  // shield runs along x-axis
;257:	{
line 258
;258:		posTraceEnd[0]+=MAX_SHIELD_HALFWIDTH;
ADDRLP4 1144
ADDRLP4 1144
INDIRF4
CNSTF4 1132396544
ADDF4
ASGNF4
line 259
;259:		negTraceEnd[0]-=MAX_SHIELD_HALFWIDTH;
ADDRLP4 1156
ADDRLP4 1156
INDIRF4
CNSTF4 1132396544
SUBF4
ASGNF4
line 260
;260:		xaxis = qtrue;
ADDRLP4 1104
CNSTI4 1
ASGNI4
line 261
;261:	}
LABELV $139
line 265
;262:
;263:	// trace horizontally to find extend of shield
;264:	// positive trace
;265:	VectorCopy(ent->r.currentOrigin, start);
ADDRLP4 1088
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 266
;266:	start[2] += (height>>1);
ADDRLP4 1088+8
ADDRLP4 1088+8
INDIRF4
ADDRLP4 1080
INDIRI4
CNSTI4 1
RSHI4
CVIF4 4
ADDF4
ASGNF4
line 267
;267:	trap_Trace (&tr, start, 0, 0, posTraceEnd, ent->s.number, MASK_SHOT );
ADDRLP4 0
ARGP4
ADDRLP4 1088
ARGP4
ADDRLP4 1192
CNSTP4 0
ASGNP4
ADDRLP4 1192
INDIRP4
ARGP4
ADDRLP4 1192
INDIRP4
ARGP4
ADDRLP4 1144
ARGP4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
CNSTI4 769
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 268
;268:	posWidth = MAX_SHIELD_HALFWIDTH * tr.fraction;
ADDRLP4 1168
CNSTF4 1132396544
ADDRLP4 0+8
INDIRF4
MULF4
CVFI4 4
ASGNI4
line 270
;269:	// negative trace
;270:	trap_Trace (&tr, start, 0, 0, negTraceEnd, ent->s.number, MASK_SHOT );
ADDRLP4 0
ARGP4
ADDRLP4 1088
ARGP4
ADDRLP4 1196
CNSTP4 0
ASGNP4
ADDRLP4 1196
INDIRP4
ARGP4
ADDRLP4 1196
INDIRP4
ARGP4
ADDRLP4 1156
ARGP4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
CNSTI4 769
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 271
;271:	negWidth = MAX_SHIELD_HALFWIDTH * tr.fraction;
ADDRLP4 1100
CNSTF4 1132396544
ADDRLP4 0+8
INDIRF4
MULF4
CVFI4 4
ASGNI4
line 274
;272:
;273:	// kef -- monkey with dimensions and place origin in center
;274:	halfWidth = (posWidth + negWidth)>>1;
ADDRLP4 1084
ADDRLP4 1168
INDIRI4
ADDRLP4 1100
INDIRI4
ADDI4
CNSTI4 1
RSHI4
ASGNI4
line 275
;275:	if (xaxis)
ADDRLP4 1104
INDIRI4
CNSTI4 0
EQI4 $145
line 276
;276:	{
line 277
;277:		ent->r.currentOrigin[0] = ent->r.currentOrigin[0] - negWidth + halfWidth;
ADDRLP4 1200
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ASGNP4
ADDRLP4 1200
INDIRP4
ADDRLP4 1200
INDIRP4
INDIRF4
ADDRLP4 1100
INDIRI4
CVIF4 4
SUBF4
ADDRLP4 1084
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 278
;278:	}
ADDRGP4 $146
JUMPV
LABELV $145
line 280
;279:	else
;280:	{
line 281
;281:		ent->r.currentOrigin[1] = ent->r.currentOrigin[1] - negWidth + halfWidth;
ADDRLP4 1200
ADDRFP4 0
INDIRP4
CNSTI4 372
ADDP4
ASGNP4
ADDRLP4 1200
INDIRP4
ADDRLP4 1200
INDIRP4
INDIRF4
ADDRLP4 1100
INDIRI4
CVIF4 4
SUBF4
ADDRLP4 1084
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 282
;282:	}
LABELV $146
line 283
;283:	ent->r.currentOrigin[2] += (height>>1);
ADDRLP4 1200
ADDRFP4 0
INDIRP4
CNSTI4 376
ADDP4
ASGNP4
ADDRLP4 1200
INDIRP4
ADDRLP4 1200
INDIRP4
INDIRF4
ADDRLP4 1080
INDIRI4
CNSTI4 1
RSHI4
CVIF4 4
ADDF4
ASGNF4
line 286
;284:
;285:	// set entity's mins and maxs to new values, make it solid, and link it
;286:	if (xaxis)
ADDRLP4 1104
INDIRI4
CNSTI4 0
EQI4 $147
line 287
;287:	{
line 288
;288:		VectorSet(ent->r.mins, -halfWidth, -SHIELD_HALFTHICKNESS, -(height>>1));
ADDRFP4 0
INDIRP4
CNSTI4 316
ADDP4
ADDRLP4 1084
INDIRI4
NEGI4
CVIF4 4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3229614080
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 324
ADDP4
ADDRLP4 1080
INDIRI4
CNSTI4 1
RSHI4
NEGI4
CVIF4 4
ASGNF4
line 289
;289:		VectorSet(ent->r.maxs, halfWidth, SHIELD_HALFTHICKNESS, height>>1);
ADDRFP4 0
INDIRP4
CNSTI4 328
ADDP4
ADDRLP4 1084
INDIRI4
CVIF4 4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1082130432
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
ADDRLP4 1080
INDIRI4
CNSTI4 1
RSHI4
CVIF4 4
ASGNF4
line 290
;290:	}
ADDRGP4 $148
JUMPV
LABELV $147
line 292
;291:	else
;292:	{
line 293
;293:		VectorSet(ent->r.mins, -SHIELD_HALFTHICKNESS, -halfWidth, -(height>>1));
ADDRFP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3229614080
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 320
ADDP4
ADDRLP4 1084
INDIRI4
NEGI4
CVIF4 4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 324
ADDP4
ADDRLP4 1080
INDIRI4
CNSTI4 1
RSHI4
NEGI4
CVIF4 4
ASGNF4
line 294
;294:		VectorSet(ent->r.maxs, SHIELD_HALFTHICKNESS, halfWidth, height);
ADDRFP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1082130432
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 332
ADDP4
ADDRLP4 1084
INDIRI4
CVIF4 4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
ADDRLP4 1080
INDIRI4
CVIF4 4
ASGNF4
line 295
;295:	}
LABELV $148
line 296
;296:	ent->clipmask = MASK_SHOT;
ADDRFP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 769
ASGNI4
line 305
;297:
;298:	// Information for shield rendering.
;299:
;300://	xaxis - 1 bit
;301://	height - 0-254 8 bits
;302://	posWidth - 0-255 8 bits
;303://  negWidth - 0 - 255 8 bits
;304:
;305:	paramData = (xaxis << 24) | (height << 16) | (posWidth << 8) | (negWidth);
ADDRLP4 1172
ADDRLP4 1104
INDIRI4
CNSTI4 24
LSHI4
ADDRLP4 1080
INDIRI4
CNSTI4 16
LSHI4
BORI4
ADDRLP4 1168
INDIRI4
CNSTI4 8
LSHI4
BORI4
ADDRLP4 1100
INDIRI4
BORI4
ASGNI4
line 306
;306:	ent->s.time2 = paramData;
ADDRFP4 0
INDIRP4
CNSTI4 88
ADDP4
ADDRLP4 1172
INDIRI4
ASGNI4
line 308
;307:
;308:	ent->health = ceil(SHIELD_HEALTH*1);
CNSTF4 1132068864
ARGF4
ADDRLP4 1204
ADDRGP4 ceil
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
ADDRLP4 1204
INDIRF4
CVFI4 4
ASGNI4
line 310
;309:
;310:	ent->s.time = ent->health;//???
ADDRLP4 1208
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1208
INDIRP4
CNSTI4 84
ADDP4
ADDRLP4 1208
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
ASGNI4
line 311
;311:	ent->pain = ShieldPain;
ADDRFP4 0
INDIRP4
CNSTI4 656
ADDP4
ADDRGP4 ShieldPain
ASGNP4
line 312
;312:	ent->die = ShieldDie;
ADDRFP4 0
INDIRP4
CNSTI4 660
ADDP4
ADDRGP4 ShieldDie
ASGNP4
line 313
;313:	ent->touch = ShieldTouch;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 ShieldTouch
ASGNP4
line 316
;314:
;315:	// see if we're valid
;316:	trap_Trace (&tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, ent->r.currentOrigin, ent->s.number, CONTENTS_BODY ); 
ADDRLP4 0
ARGP4
ADDRLP4 1212
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1216
ADDRLP4 1212
INDIRP4
CNSTI4 368
ADDP4
ASGNP4
ADDRLP4 1216
INDIRP4
ARGP4
ADDRLP4 1212
INDIRP4
CNSTI4 316
ADDP4
ARGP4
ADDRLP4 1212
INDIRP4
CNSTI4 328
ADDP4
ARGP4
ADDRLP4 1216
INDIRP4
ARGP4
ADDRLP4 1212
INDIRP4
INDIRI4
ARGI4
CNSTI4 256
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 318
;317:
;318:	if (tr.startsolid)
ADDRLP4 0+4
INDIRI4
CNSTI4 0
EQI4 $149
line 319
;319:	{	// Something in the way!
line 321
;320:		// make the shield non-solid very briefly
;321:		ent->r.contents = 0;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 0
ASGNI4
line 322
;322:		ent->s.eFlags |= EF_NODRAW;
ADDRLP4 1220
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 1220
INDIRP4
ADDRLP4 1220
INDIRP4
INDIRI4
CNSTI4 128
BORI4
ASGNI4
line 324
;323:		// nextthink needs to have a large enough interval to avoid excess accumulation of Activate messages
;324:		ent->nextthink = level.time + 200;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 200
ADDI4
ASGNI4
line 325
;325:		ent->think = ShieldGoSolid;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 ShieldGoSolid
ASGNP4
line 326
;326:		ent->takedamage = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 0
ASGNI4
line 327
;327:		trap_LinkEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 328
;328:	}
ADDRGP4 $150
JUMPV
LABELV $149
line 330
;329:	else
;330:	{	// Get solid.
line 331
;331:		ent->r.contents = CONTENTS_PLAYERCLIP|CONTENTS_SHOTCLIP;//CONTENTS_SOLID;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 144
ASGNI4
line 333
;332:
;333:		ent->nextthink = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 334
;334:		ent->think = ShieldThink;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 ShieldThink
ASGNP4
line 336
;335:
;336:		ent->takedamage = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 1
ASGNI4
line 337
;337:		trap_LinkEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 340
;338:
;339:		// Play raising sound...
;340:		G_AddEvent(ent, EV_GENERAL_SOUND, shieldActivateSound);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 66
ARGI4
ADDRGP4 shieldActivateSound
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 341
;341:		ent->s.loopSound = shieldLoopSound;
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
ADDRGP4 shieldLoopSound
INDIRI4
ASGNI4
line 342
;342:	}
LABELV $150
line 344
;343:
;344:	ShieldGoSolid(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 ShieldGoSolid
CALLV
pop
line 346
;345:
;346:	return;
LABELV $130
endproc CreateShield 1224 28
data
align 4
LABELV $155
byte 4 0
lit
align 4
LABELV $156
byte 4 3229614080
byte 4 3229614080
byte 4 0
align 4
LABELV $157
byte 4 1082130432
byte 4 1082130432
byte 4 1082130432
export PlaceShield
code
proc PlaceShield 1188 28
line 350
;347:}
;348:
;349:qboolean PlaceShield(gentity_t *playerent)
;350:{
line 352
;351:	static const gitem_t *shieldItem = NULL;
;352:	gentity_t	*shield = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
line 354
;353:	trace_t		tr;
;354:	vec3_t		fwd, pos, dest, mins = {-4,-4, 0}, maxs = {4,4,4};
ADDRLP4 1120
ADDRGP4 $156
INDIRB
ASGNB 12
ADDRLP4 1132
ADDRGP4 $157
INDIRB
ASGNB 12
line 356
;355:
;356:	if (shieldAttachSound==0)
ADDRGP4 shieldAttachSound
INDIRI4
CNSTI4 0
NEI4 $158
line 357
;357:	{
line 358
;358:		shieldLoopSound = G_SoundIndex("sound/movers/doors/forcefield_lp.wav");
ADDRGP4 $160
ARGP4
ADDRLP4 1144
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRGP4 shieldLoopSound
ADDRLP4 1144
INDIRI4
ASGNI4
line 359
;359:		shieldAttachSound = G_SoundIndex("sound/weapons/detpack/stick.wav");
ADDRGP4 $161
ARGP4
ADDRLP4 1148
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRGP4 shieldAttachSound
ADDRLP4 1148
INDIRI4
ASGNI4
line 360
;360:		shieldActivateSound = G_SoundIndex("sound/movers/doors/forcefield_on.wav");
ADDRGP4 $162
ARGP4
ADDRLP4 1152
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRGP4 shieldActivateSound
ADDRLP4 1152
INDIRI4
ASGNI4
line 361
;361:		shieldDeactivateSound = G_SoundIndex("sound/movers/doors/forcefield_off.wav");
ADDRGP4 $163
ARGP4
ADDRLP4 1156
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRGP4 shieldDeactivateSound
ADDRLP4 1156
INDIRI4
ASGNI4
line 362
;362:		shieldDamageSound = G_SoundIndex("sound/effects/bumpfield.wav");
ADDRGP4 $164
ARGP4
ADDRLP4 1160
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRGP4 shieldDamageSound
ADDRLP4 1160
INDIRI4
ASGNI4
line 363
;363:		shieldItem = BG_FindItemForHoldable(HI_SHIELD);
CNSTI4 2
ARGI4
ADDRLP4 1164
ADDRGP4 BG_FindItemForHoldable
CALLP4
ASGNP4
ADDRGP4 $155
ADDRLP4 1164
INDIRP4
ASGNP4
line 364
;364:	}
LABELV $158
line 367
;365:
;366:	// can we place this in front of us?
;367:	AngleVectors (playerent->client->ps.viewangles, fwd, NULL, NULL);
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 156
ADDP4
ARGP4
ADDRLP4 16
ARGP4
ADDRLP4 1144
CNSTP4 0
ASGNP4
ADDRLP4 1144
INDIRP4
ARGP4
ADDRLP4 1144
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 368
;368:	fwd[2] = 0;
ADDRLP4 16+8
CNSTF4 0
ASGNF4
line 369
;369:	VectorMA(playerent->client->ps.origin, SHIELD_PLACEDIST, fwd, dest);
ADDRLP4 1148
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
ASGNP4
ADDRLP4 1152
CNSTF4 1115684864
ASGNF4
ADDRLP4 4
ADDRLP4 1148
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 1152
INDIRF4
ADDRLP4 16
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 1148
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 1152
INDIRF4
ADDRLP4 16+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 4+8
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
CNSTF4 1115684864
ADDRLP4 16+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 370
;370:	trap_Trace (&tr, playerent->client->ps.origin, mins, maxs, dest, playerent->s.number, MASK_SHOT );
ADDRLP4 28
ARGP4
ADDRLP4 1156
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1156
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 1120
ARGP4
ADDRLP4 1132
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 1156
INDIRP4
INDIRI4
ARGI4
CNSTI4 769
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 371
;371:	if (tr.fraction > 0.9)
ADDRLP4 28+8
INDIRF4
CNSTF4 1063675494
LEF4 $170
line 372
;372:	{//room in front
line 373
;373:		VectorCopy(tr.endpos, pos);
ADDRLP4 1108
ADDRLP4 28+12
INDIRB
ASGNB 12
line 375
;374:		// drop to floor
;375:		VectorSet( dest, pos[0], pos[1], pos[2] - 4096 );
ADDRLP4 4
ADDRLP4 1108
INDIRF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 1108+4
INDIRF4
ASGNF4
ADDRLP4 4+8
ADDRLP4 1108+8
INDIRF4
CNSTF4 1166016512
SUBF4
ASGNF4
line 376
;376:		trap_Trace( &tr, pos, mins, maxs, dest, playerent->s.number, MASK_SOLID );
ADDRLP4 28
ARGP4
ADDRLP4 1108
ARGP4
ADDRLP4 1120
ARGP4
ADDRLP4 1132
ARGP4
ADDRLP4 4
ARGP4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 377
;377:		if ( !tr.startsolid && !tr.allsolid )
ADDRLP4 1160
CNSTI4 0
ASGNI4
ADDRLP4 28+4
INDIRI4
ADDRLP4 1160
INDIRI4
NEI4 $178
ADDRLP4 28
INDIRI4
ADDRLP4 1160
INDIRI4
NEI4 $178
line 378
;378:		{
line 380
;379:			// got enough room so place the portable shield
;380:			shield = G_Spawn();
ADDRLP4 1164
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 1164
INDIRP4
ASGNP4
line 383
;381:
;382:			// Figure out what direction the shield is facing.
;383:			if (fabs(fwd[0]) > fabs(fwd[1]))
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 1168
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 16+4
INDIRF4
ARGF4
ADDRLP4 1172
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 1168
INDIRF4
ADDRLP4 1172
INDIRF4
LEF4 $181
line 384
;384:			{	// shield is north/south, facing east.
line 385
;385:				shield->s.angles[YAW] = 0;
ADDRLP4 0
INDIRP4
CNSTI4 120
ADDP4
CNSTF4 0
ASGNF4
line 386
;386:			}
ADDRGP4 $182
JUMPV
LABELV $181
line 388
;387:			else
;388:			{	// shield is along the east/west axis, facing north
line 389
;389:				shield->s.angles[YAW] = 90;
ADDRLP4 0
INDIRP4
CNSTI4 120
ADDP4
CNSTF4 1119092736
ASGNF4
line 390
;390:			}
LABELV $182
line 391
;391:			shield->think = CreateShield;
ADDRLP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 CreateShield
ASGNP4
line 392
;392:			shield->nextthink = level.time + 500;	// power up after .5 seconds
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 500
ADDI4
ASGNI4
line 393
;393:			shield->parent = playerent;
ADDRLP4 0
INDIRP4
CNSTI4 536
ADDP4
ADDRFP4 0
INDIRP4
ASGNP4
line 396
;394:
;395:			// Set team number.
;396:			shield->s.otherEntityNum2 = playerent->client->sess.sessionTeam;
ADDRLP4 0
INDIRP4
CNSTI4 188
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ASGNI4
line 398
;397:
;398:			shield->s.eType = ET_SPECIAL;
ADDRLP4 1176
CNSTI4 4
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 1176
INDIRI4
ADDP4
ADDRLP4 1176
INDIRI4
ASGNI4
line 399
;399:			shield->s.modelindex =  HI_SHIELD;	// this'll be used in CG_Useable() for rendering.
ADDRLP4 0
INDIRP4
CNSTI4 212
ADDP4
CNSTI4 2
ASGNI4
line 400
;400:			shield->classname = shieldItem->classname;
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $155
INDIRP4
INDIRP4
ASGNP4
line 402
;401:
;402:			shield->r.contents = CONTENTS_TRIGGER;
ADDRLP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1024
ASGNI4
line 404
;403:
;404:			shield->touch = 0;
ADDRLP4 0
INDIRP4
CNSTI4 648
ADDP4
CNSTP4 0
ASGNP4
line 406
;405:			// using an item causes it to respawn
;406:			shield->use = 0; //Use_Item;
ADDRLP4 0
INDIRP4
CNSTI4 652
ADDP4
CNSTP4 0
ASGNP4
line 409
;407:
;408:			// allow to ride movers
;409:			shield->s.groundEntityNum = tr.entityNum;
ADDRLP4 0
INDIRP4
CNSTI4 192
ADDP4
ADDRLP4 28+52
INDIRI4
ASGNI4
line 411
;410:
;411:			G_SetOrigin( shield, tr.endpos );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 28+12
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 413
;412:
;413:			shield->s.eFlags &= ~EF_NODRAW;
ADDRLP4 1180
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 1180
INDIRP4
ADDRLP4 1180
INDIRP4
INDIRI4
CNSTI4 -129
BANDI4
ASGNI4
line 414
;414:			shield->r.svFlags &= ~SVF_NOCLIENT;
ADDRLP4 1184
ADDRLP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 1184
INDIRP4
ADDRLP4 1184
INDIRP4
INDIRI4
CNSTI4 -2
BANDI4
ASGNI4
line 416
;415:
;416:			trap_LinkEntity (shield);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 418
;417:
;418:			shield->s.owner = playerent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 419
;419:			shield->s.shouldtarget = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 268
ADDP4
CNSTI4 1
ASGNI4
line 420
;420:			if (g_gametype.integer >= GT_TEAM)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
LTI4 $187
line 421
;421:			{
line 422
;422:				shield->s.teamowner = playerent->client->sess.sessionTeam;
ADDRLP4 0
INDIRP4
CNSTI4 264
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ASGNI4
line 423
;423:			}
ADDRGP4 $188
JUMPV
LABELV $187
line 425
;424:			else
;425:			{
line 426
;426:				shield->s.teamowner = 16;
ADDRLP4 0
INDIRP4
CNSTI4 264
ADDP4
CNSTI4 16
ASGNI4
line 427
;427:			}
LABELV $188
line 430
;428:
;429:			// Play placing sound...
;430:			G_AddEvent(shield, EV_GENERAL_SOUND, shieldAttachSound);
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 66
ARGI4
ADDRGP4 shieldAttachSound
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 432
;431:
;432:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $154
JUMPV
LABELV $178
line 434
;433:		}
;434:	}
LABELV $170
line 436
;435:	// no room
;436:	return qfalse;
CNSTI4 0
RETI4
LABELV $154
endproc PlaceShield 1188 28
export ItemUse_Binoculars
proc ItemUse_Binoculars 8 0
line 440
;437:}
;438:
;439:void ItemUse_Binoculars(gentity_t *ent)
;440:{
line 441
;441:	if (!ent || !ent->client)
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
EQU4 $193
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
NEU4 $191
LABELV $193
line 442
;442:	{
line 443
;443:		return;
ADDRGP4 $190
JUMPV
LABELV $191
line 446
;444:	}
;445:
;446:	if (ent->client->ps.weaponstate != WEAPON_READY)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 152
ADDP4
INDIRI4
CNSTI4 0
EQI4 $194
line 447
;447:	{ //So we can't fool it and reactivate while switching to the saber or something.
line 448
;448:		return;
ADDRGP4 $190
JUMPV
LABELV $194
line 451
;449:	}
;450:
;451:	if (ent->client->ps.weapon == WP_SABER)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
CNSTI4 2
NEI4 $196
line 452
;452:	{ //No.
line 453
;453:		return;
ADDRGP4 $190
JUMPV
LABELV $196
line 456
;454:	}
;455:
;456:	if (ent->client->ps.zoomMode == 0) // not zoomed or currently zoomed with the disruptor
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1320
ADDP4
INDIRI4
CNSTI4 0
NEI4 $198
line 457
;457:	{
line 458
;458:		ent->client->ps.zoomMode = 2;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1320
ADDP4
CNSTI4 2
ASGNI4
line 459
;459:		ent->client->ps.zoomLocked = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1328
ADDP4
CNSTI4 0
ASGNI4
line 460
;460:		ent->client->ps.zoomFov = 40.0f;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1332
ADDP4
CNSTF4 1109393408
ASGNF4
line 461
;461:	}
ADDRGP4 $199
JUMPV
LABELV $198
line 462
;462:	else if (ent->client->ps.zoomMode == 2)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1320
ADDP4
INDIRI4
CNSTI4 2
NEI4 $200
line 463
;463:	{
line 464
;464:		ent->client->ps.zoomMode = 0;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1320
ADDP4
CNSTI4 0
ASGNI4
line 465
;465:		ent->client->ps.zoomTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1324
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 466
;466:	}
LABELV $200
LABELV $199
line 467
;467:}
LABELV $190
endproc ItemUse_Binoculars 8 0
export ItemUse_Shield
proc ItemUse_Shield 0 4
line 470
;468:
;469:void ItemUse_Shield(gentity_t *ent)
;470:{
line 471
;471:	PlaceShield(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 PlaceShield
CALLI4
pop
line 472
;472:}
LABELV $203
endproc ItemUse_Shield 0 4
export SentryTouch
proc SentryTouch 0 0
line 481
;473:
;474://--------------------------
;475:// PERSONAL ASSAULT SENTRY
;476://--------------------------
;477:
;478:#define PAS_DAMAGE	2
;479:
;480:void SentryTouch(gentity_t *ent, gentity_t *other, trace_t *trace)
;481:{
line 482
;482:	return;
LABELV $204
endproc SentryTouch 0 0
export pas_fire
proc pas_fire 40 32
line 488
;483:}
;484:
;485://----------------------------------------------------------------
;486:void pas_fire( gentity_t *ent )
;487://----------------------------------------------------------------
;488:{
line 491
;489:	vec3_t fwd, myOrg, enOrg;
;490:
;491:	VectorCopy(ent->r.currentOrigin, myOrg);
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 492
;492:	myOrg[2] += 24;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1103101952
ADDF4
ASGNF4
line 494
;493:
;494:	VectorCopy(ent->enemy->client->ps.origin, enOrg);
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 495
;495:	enOrg[2] += 24;
ADDRLP4 24+8
ADDRLP4 24+8
INDIRF4
CNSTF4 1103101952
ADDF4
ASGNF4
line 497
;496:
;497:	VectorSubtract(enOrg, myOrg, fwd);
ADDRLP4 12
ADDRLP4 24
INDIRF4
ADDRLP4 0
INDIRF4
SUBF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 24+4
INDIRF4
ADDRLP4 0+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 12+8
ADDRLP4 24+8
INDIRF4
ADDRLP4 0+8
INDIRF4
SUBF4
ASGNF4
line 498
;498:	VectorNormalize(fwd);
ADDRLP4 12
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 500
;499:	
;500:	myOrg[0] += fwd[0]*16;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1098907648
ADDRLP4 12
INDIRF4
MULF4
ADDF4
ASGNF4
line 501
;501:	myOrg[1] += fwd[1]*16;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1098907648
ADDRLP4 12+4
INDIRF4
MULF4
ADDF4
ASGNF4
line 502
;502:	myOrg[2] += fwd[2]*16;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1098907648
ADDRLP4 12+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 504
;503:
;504:	WP_FireTurretMissile(&g_entities[ent->boltpoint3], myOrg, fwd, qfalse, 10, 2300, MOD_SENTRY, ent );
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
CNSTI4 828
ADDRLP4 36
INDIRP4
CNSTI4 776
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 12
ARGP4
CNSTI4 0
ARGI4
CNSTI4 10
ARGI4
CNSTI4 2300
ARGI4
CNSTI4 28
ARGI4
ADDRLP4 36
INDIRP4
ARGP4
ADDRGP4 WP_FireTurretMissile
CALLV
pop
line 506
;505:
;506:	G_RunObject(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_RunObject
CALLV
pop
line 507
;507:}
LABELV $205
endproc pas_fire 40 32
export G_SetEnemy
proc G_SetEnemy 0 0
line 510
;508:
;509:void G_SetEnemy( gentity_t *self, gentity_t *enemy )
;510:{
line 511
;511:	if (!enemy->client)
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $219
line 512
;512:	{
line 513
;513:		return;
ADDRGP4 $218
JUMPV
LABELV $219
line 516
;514:	}
;515:
;516:	self->enemy = enemy;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRFP4 4
INDIRP4
ASGNP4
line 517
;517:}
LABELV $218
endproc G_SetEnemy 0 0
proc pas_find_enemies 5296 28
line 524
;518:
;519:#define TURRET_RADIUS 800
;520:
;521://-----------------------------------------------------
;522:static qboolean pas_find_enemies( gentity_t *self )
;523://-----------------------------------------------------
;524:{
line 525
;525:	qboolean	found = qfalse;
ADDRLP4 5232
CNSTI4 0
ASGNI4
line 527
;526:	int			count, i;
;527:	float		bestDist = TURRET_RADIUS*TURRET_RADIUS;
ADDRLP4 5228
CNSTF4 1226588160
ASGNF4
line 533
;528:	float		enemyDist;
;529:	vec3_t		enemyDir, org, org2;
;530:	gentity_t	*entity_list[MAX_GENTITIES], *target;
;531:	trace_t		tr;
;532:
;533:	if ( self->aimDebounceTime > level.time ) // time since we've been shut off
ADDRFP4 0
INDIRP4
CNSTI4 448
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
LEI4 $222
line 534
;534:	{
line 536
;535:		// We were active and alert, i.e. had an enemy in the last 3 secs
;536:		if ( self->painDebounceTime < level.time )
ADDRFP4 0
INDIRP4
CNSTI4 452
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $225
line 537
;537:		{
line 538
;538:			G_Sound(self, CHAN_BODY, G_SoundIndex( "sound/chars/turret/ping.wav" ));
ADDRGP4 $228
ARGP4
ADDRLP4 5236
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 5
ARGI4
ADDRLP4 5236
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 539
;539:			self->painDebounceTime = level.time + 1000;
ADDRFP4 0
INDIRP4
CNSTI4 452
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1000
ADDI4
ASGNI4
line 540
;540:		}
LABELV $225
line 541
;541:	}
LABELV $222
line 543
;542:
;543:	VectorCopy(self->s.pos.trBase, org2);
ADDRLP4 1088
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 545
;544:
;545:	count = G_RadiusList( org2, TURRET_RADIUS, self, qtrue, entity_list );
ADDRLP4 1088
ARGP4
CNSTF4 1145569280
ARGF4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 1132
ARGP4
ADDRLP4 5236
ADDRGP4 G_RadiusList
CALLI4
ASGNI4
ADDRLP4 1128
ADDRLP4 5236
INDIRI4
ASGNI4
line 547
;546:
;547:	for ( i = 0; i < count; i++ )
ADDRLP4 1084
CNSTI4 0
ASGNI4
ADDRGP4 $233
JUMPV
LABELV $230
line 548
;548:	{
line 549
;549:		target = entity_list[i];
ADDRLP4 0
ADDRLP4 1084
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1132
ADDP4
INDIRP4
ASGNP4
line 551
;550:
;551:		if ( !target->client )
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $234
line 552
;552:		{
line 553
;553:			continue;
ADDRGP4 $231
JUMPV
LABELV $234
line 555
;554:		}
;555:		if ( target == self || !target->takedamage || target->health <= 0 || ( target->flags & FL_NOTARGET ))
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
EQU4 $240
ADDRLP4 5244
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
ADDRLP4 5244
INDIRI4
EQI4 $240
ADDRLP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
ADDRLP4 5244
INDIRI4
LEI4 $240
ADDRLP4 0
INDIRP4
CNSTI4 472
ADDP4
INDIRI4
CNSTI4 32
BANDI4
ADDRLP4 5244
INDIRI4
EQI4 $236
LABELV $240
line 556
;556:		{
line 557
;557:			continue;
ADDRGP4 $231
JUMPV
LABELV $236
line 559
;558:		}
;559:		if ( self->noDamageTeam && target->client->sess.sessionTeam == self->noDamageTeam )
ADDRLP4 5248
ADDRFP4 0
INDIRP4
CNSTI4 460
ADDP4
INDIRI4
ASGNI4
ADDRLP4 5248
INDIRI4
CNSTI4 0
EQI4 $241
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ADDRLP4 5248
INDIRI4
NEI4 $241
line 560
;560:		{ 
line 561
;561:			continue;
ADDRGP4 $231
JUMPV
LABELV $241
line 563
;562:		}
;563:		if (self->boltpoint3 == target->s.number)
ADDRFP4 0
INDIRP4
CNSTI4 776
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
NEI4 $243
line 564
;564:		{
line 565
;565:			continue;
ADDRGP4 $231
JUMPV
LABELV $243
line 567
;566:		}
;567:		if ( !trap_InPVS( org2, target->r.currentOrigin ))
ADDRLP4 1088
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 5252
ADDRGP4 trap_InPVS
CALLI4
ASGNI4
ADDRLP4 5252
INDIRI4
CNSTI4 0
NEI4 $245
line 568
;568:		{
line 569
;569:			continue;
ADDRGP4 $231
JUMPV
LABELV $245
line 572
;570:		}
;571:
;572:		if ( target->client )
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $247
line 573
;573:		{
line 574
;574:			VectorCopy( target->client->ps.origin, org );
ADDRLP4 1112
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 575
;575:		}
ADDRGP4 $248
JUMPV
LABELV $247
line 577
;576:		else
;577:		{
line 578
;578:			VectorCopy( target->r.currentOrigin, org );
ADDRLP4 1112
ADDRLP4 0
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 579
;579:		}
LABELV $248
line 581
;580:
;581:		trap_Trace( &tr, org2, NULL, NULL, org, self->s.number, MASK_SHOT );
ADDRLP4 4
ARGP4
ADDRLP4 1088
ARGP4
ADDRLP4 5256
CNSTP4 0
ASGNP4
ADDRLP4 5256
INDIRP4
ARGP4
ADDRLP4 5256
INDIRP4
ARGP4
ADDRLP4 1112
ARGP4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
CNSTI4 769
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 583
;582:
;583:		if ( !tr.allsolid && !tr.startsolid && ( tr.fraction == 1.0 || tr.entityNum == target->s.number ))
ADDRLP4 5260
CNSTI4 0
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 5260
INDIRI4
NEI4 $249
ADDRLP4 4+4
INDIRI4
ADDRLP4 5260
INDIRI4
NEI4 $249
ADDRLP4 4+8
INDIRF4
CNSTF4 1065353216
EQF4 $254
ADDRLP4 4+52
INDIRI4
ADDRLP4 0
INDIRP4
INDIRI4
NEI4 $249
LABELV $254
line 584
;584:		{
line 586
;585:			// Only acquire if have a clear shot, Is it in range and closer than our best?
;586:			VectorSubtract( target->r.currentOrigin, self->r.currentOrigin, enemyDir );
ADDRLP4 5268
CNSTI4 368
ASGNI4
ADDRLP4 5272
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1100
ADDRLP4 0
INDIRP4
ADDRLP4 5268
INDIRI4
ADDP4
INDIRF4
ADDRLP4 5272
INDIRP4
ADDRLP4 5268
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 5276
CNSTI4 372
ASGNI4
ADDRLP4 1100+4
ADDRLP4 0
INDIRP4
ADDRLP4 5276
INDIRI4
ADDP4
INDIRF4
ADDRLP4 5272
INDIRP4
ADDRLP4 5276
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 5280
CNSTI4 376
ASGNI4
ADDRLP4 1100+8
ADDRLP4 0
INDIRP4
ADDRLP4 5280
INDIRI4
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
ADDRLP4 5280
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 587
;587:			enemyDist = VectorLengthSquared( enemyDir );
ADDRLP4 1100
ARGP4
ADDRLP4 5284
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 1124
ADDRLP4 5284
INDIRF4
ASGNF4
line 589
;588:
;589:			if ( enemyDist < bestDist )// all things equal, keep current
ADDRLP4 1124
INDIRF4
ADDRLP4 5228
INDIRF4
GEF4 $257
line 590
;590:			{
line 591
;591:				if ( self->attackDebounceTime + 100 < level.time )
ADDRFP4 0
INDIRP4
CNSTI4 456
ADDP4
INDIRI4
CNSTI4 100
ADDI4
ADDRGP4 level+32
INDIRI4
GEI4 $259
line 592
;592:				{
line 594
;593:					// We haven't fired or acquired an enemy in the last 2 seconds-start-up sound
;594:					G_Sound( self, CHAN_BODY, G_SoundIndex( "sound/chars/turret/startup.wav" ));
ADDRGP4 $262
ARGP4
ADDRLP4 5288
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 5
ARGI4
ADDRLP4 5288
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 597
;595:
;596:					// Wind up turrets for a bit
;597:					self->attackDebounceTime = level.time + 900 + random() * 200;
ADDRLP4 5292
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 456
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 900
ADDI4
CVIF4 4
CNSTF4 1128792064
ADDRLP4 5292
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
CVFI4 4
ASGNI4
line 598
;598:				}
LABELV $259
line 600
;599:
;600:				G_SetEnemy( self, target );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_SetEnemy
CALLV
pop
line 601
;601:				bestDist = enemyDist;
ADDRLP4 5228
ADDRLP4 1124
INDIRF4
ASGNF4
line 602
;602:				found = qtrue;
ADDRLP4 5232
CNSTI4 1
ASGNI4
line 603
;603:			}
LABELV $257
line 604
;604:		}
LABELV $249
line 605
;605:	}
LABELV $231
line 547
ADDRLP4 1084
ADDRLP4 1084
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $233
ADDRLP4 1084
INDIRI4
ADDRLP4 1128
INDIRI4
LTI4 $230
line 607
;606:
;607:	return found;
ADDRLP4 5232
INDIRI4
RETI4
LABELV $221
endproc pas_find_enemies 5296 28
export pas_adjust_enemy
proc pas_adjust_enemy 1116 28
line 613
;608:}
;609:
;610://---------------------------------
;611:void pas_adjust_enemy( gentity_t *ent )
;612://---------------------------------
;613:{
line 615
;614:	trace_t	tr;
;615:	qboolean keep = qtrue;
ADDRLP4 1080
CNSTI4 1
ASGNI4
line 617
;616:
;617:	if ( ent->enemy->health <= 0 )
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 0
GTI4 $265
line 618
;618:	{
line 619
;619:		keep = qfalse;
ADDRLP4 1080
CNSTI4 0
ASGNI4
line 620
;620:	}
ADDRGP4 $266
JUMPV
LABELV $265
line 622
;621:	else
;622:	{
line 625
;623:		vec3_t		org, org2;
;624:
;625:		VectorCopy(ent->s.pos.trBase, org2);
ADDRLP4 1096
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 627
;626:
;627:		if ( ent->enemy->client )
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $267
line 628
;628:		{
line 629
;629:			VectorCopy( ent->enemy->client->ps.origin, org );
ADDRLP4 1084
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 630
;630:			org[2] -= 15;
ADDRLP4 1084+8
ADDRLP4 1084+8
INDIRF4
CNSTF4 1097859072
SUBF4
ASGNF4
line 631
;631:		}
ADDRGP4 $268
JUMPV
LABELV $267
line 633
;632:		else
;633:		{
line 634
;634:			VectorCopy( ent->enemy->r.currentOrigin, org );
ADDRLP4 1084
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 635
;635:		}
LABELV $268
line 637
;636:
;637:		trap_Trace( &tr, org2, NULL, NULL, org, ent->s.number, MASK_SHOT );
ADDRLP4 0
ARGP4
ADDRLP4 1096
ARGP4
ADDRLP4 1108
CNSTP4 0
ASGNP4
ADDRLP4 1108
INDIRP4
ARGP4
ADDRLP4 1108
INDIRP4
ARGP4
ADDRLP4 1084
ARGP4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
CNSTI4 769
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 639
;638:
;639:		if ( tr.allsolid || tr.startsolid || tr.fraction < 0.9f || tr.entityNum == ent->s.number )
ADDRLP4 1112
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 1112
INDIRI4
NEI4 $277
ADDRLP4 0+4
INDIRI4
ADDRLP4 1112
INDIRI4
NEI4 $277
ADDRLP4 0+8
INDIRF4
CNSTF4 1063675494
LTF4 $277
ADDRLP4 0+52
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $270
LABELV $277
line 640
;640:		{
line 641
;641:			if (tr.entityNum != ent->enemy->s.number)
ADDRLP4 0+52
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
INDIRI4
EQI4 $278
line 642
;642:			{
line 644
;643:				// trace failed
;644:				keep = qfalse;
ADDRLP4 1080
CNSTI4 0
ASGNI4
line 645
;645:			}
LABELV $278
line 646
;646:		}
LABELV $270
line 647
;647:	}
LABELV $266
line 649
;648:
;649:	if ( keep )
ADDRLP4 1080
INDIRI4
CNSTI4 0
EQI4 $281
line 650
;650:	{
line 652
;651:		//ent->bounceCount = level.time + 500 + random() * 150;
;652:	}
ADDRGP4 $282
JUMPV
LABELV $281
line 653
;653:	else if ( ent->bounceCount < level.time && ent->enemy ) // don't ping pong on and off
ADDRLP4 1084
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1084
INDIRP4
CNSTI4 716
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $283
ADDRLP4 1084
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $283
line 654
;654:	{
line 655
;655:		ent->enemy = NULL;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
CNSTP4 0
ASGNP4
line 657
;656:		// shut-down sound
;657:		G_Sound( ent, CHAN_BODY, G_SoundIndex( "sound/chars/turret/shutdown.wav" ));
ADDRGP4 $286
ARGP4
ADDRLP4 1088
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 5
ARGI4
ADDRLP4 1088
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 659
;658:	
;659:		ent->bounceCount = level.time + 500 + random() * 150;
ADDRLP4 1092
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 716
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 500
ADDI4
CVIF4 4
CNSTF4 1125515264
ADDRLP4 1092
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
ADDF4
CVFI4 4
ASGNI4
line 662
;660:
;661:		// make turret play ping sound for 5 seconds
;662:		ent->aimDebounceTime = level.time + 5000;
ADDRFP4 0
INDIRP4
CNSTI4 448
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 5000
ADDI4
ASGNI4
line 663
;663:	}
LABELV $283
LABELV $282
line 664
;664:}
LABELV $264
endproc pas_adjust_enemy 1116 28
export sentryExpire
proc sentryExpire 4 20
line 672
;665:
;666:#define TURRET_DEATH_DELAY 2000
;667:#define TURRET_LIFETIME 60000
;668:
;669:void turret_die(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod);
;670:
;671:void sentryExpire(gentity_t *self)
;672:{
line 673
;673:	turret_die(self, self, self, 1000, MOD_UNKNOWN);	
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 1000
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 turret_die
CALLV
pop
line 674
;674:}
LABELV $289
endproc sentryExpire 4 20
export pas_think
proc pas_think 4280 16
line 679
;675:
;676://---------------------------------
;677:void pas_think( gentity_t *ent )
;678://---------------------------------
;679:{
line 687
;680:	qboolean	moved;
;681:	float		diffYaw, diffPitch;
;682:	vec3_t		enemyDir, org;
;683:	vec3_t		frontAngles, backAngles;
;684:	vec3_t		desiredAngles;
;685:	int			iEntityList[MAX_GENTITIES];
;686:	int			numListedEntities;
;687:	int			i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 688
;688:	qboolean	clTrapped = qfalse;
ADDRLP4 4104
CNSTI4 0
ASGNI4
line 691
;689:	vec3_t		testMins, testMaxs;
;690:
;691:	testMins[0] = ent->r.currentOrigin[0] + ent->r.mins[0]+4;
ADDRLP4 4204
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4112
ADDRLP4 4204
INDIRP4
CNSTI4 368
ADDP4
INDIRF4
ADDRLP4 4204
INDIRP4
CNSTI4 316
ADDP4
INDIRF4
ADDF4
CNSTF4 1082130432
ADDF4
ASGNF4
line 692
;692:	testMins[1] = ent->r.currentOrigin[1] + ent->r.mins[1]+4;
ADDRLP4 4208
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4112+4
ADDRLP4 4208
INDIRP4
CNSTI4 372
ADDP4
INDIRF4
ADDRLP4 4208
INDIRP4
CNSTI4 320
ADDP4
INDIRF4
ADDF4
CNSTF4 1082130432
ADDF4
ASGNF4
line 693
;693:	testMins[2] = ent->r.currentOrigin[2] + ent->r.mins[2]+4;
ADDRLP4 4212
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4112+8
ADDRLP4 4212
INDIRP4
CNSTI4 376
ADDP4
INDIRF4
ADDRLP4 4212
INDIRP4
CNSTI4 324
ADDP4
INDIRF4
ADDF4
CNSTF4 1082130432
ADDF4
ASGNF4
line 695
;694:
;695:	testMaxs[0] = ent->r.currentOrigin[0] + ent->r.maxs[0]-4;
ADDRLP4 4216
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4124
ADDRLP4 4216
INDIRP4
CNSTI4 368
ADDP4
INDIRF4
ADDRLP4 4216
INDIRP4
CNSTI4 328
ADDP4
INDIRF4
ADDF4
CNSTF4 1082130432
SUBF4
ASGNF4
line 696
;696:	testMaxs[1] = ent->r.currentOrigin[1] + ent->r.maxs[1]-4;
ADDRLP4 4220
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4124+4
ADDRLP4 4220
INDIRP4
CNSTI4 372
ADDP4
INDIRF4
ADDRLP4 4220
INDIRP4
CNSTI4 332
ADDP4
INDIRF4
ADDF4
CNSTF4 1082130432
SUBF4
ASGNF4
line 697
;697:	testMaxs[2] = ent->r.currentOrigin[2] + ent->r.maxs[2]-4;
ADDRLP4 4224
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4124+8
ADDRLP4 4224
INDIRP4
CNSTI4 376
ADDP4
INDIRF4
ADDRLP4 4224
INDIRP4
CNSTI4 336
ADDP4
INDIRF4
ADDF4
CNSTF4 1082130432
SUBF4
ASGNF4
line 699
;698:
;699:	numListedEntities = trap_EntitiesInBox( testMins, testMaxs, iEntityList, MAX_GENTITIES );
ADDRLP4 4112
ARGP4
ADDRLP4 4124
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 4228
ADDRGP4 trap_EntitiesInBox
CALLI4
ASGNI4
ADDRLP4 4100
ADDRLP4 4228
INDIRI4
ASGNI4
ADDRGP4 $296
JUMPV
LABELV $295
line 702
;700:
;701:	while (i < numListedEntities)
;702:	{
line 703
;703:		if (iEntityList[i] < MAX_CLIENTS)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
CNSTI4 32
GEI4 $298
line 704
;704:		{ //client stuck inside me. go nonsolid.
line 705
;705:			int clNum = iEntityList[i];
ADDRLP4 4232
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ASGNI4
line 707
;706:
;707:			numListedEntities = trap_EntitiesInBox( g_entities[clNum].r.absmin, g_entities[clNum].r.absmax, iEntityList, MAX_GENTITIES );
ADDRLP4 4236
CNSTI4 828
ADDRLP4 4232
INDIRI4
MULI4
ASGNI4
ADDRLP4 4236
INDIRI4
ADDRGP4 g_entities+296+48
ADDP4
ARGP4
ADDRLP4 4236
INDIRI4
ADDRGP4 g_entities+296+60
ADDP4
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 4240
ADDRGP4 trap_EntitiesInBox
CALLI4
ASGNI4
ADDRLP4 4100
ADDRLP4 4240
INDIRI4
ASGNI4
line 709
;708:
;709:			i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $305
JUMPV
LABELV $304
line 711
;710:			while (i < numListedEntities)
;711:			{
line 712
;712:				if (iEntityList[i] == ent->s.number)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $307
line 713
;713:				{
line 714
;714:					clTrapped = qtrue;
ADDRLP4 4104
CNSTI4 1
ASGNI4
line 715
;715:					break;
ADDRGP4 $297
JUMPV
LABELV $307
line 717
;716:				}
;717:				i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 718
;718:			}
LABELV $305
line 710
ADDRLP4 0
INDIRI4
ADDRLP4 4100
INDIRI4
LTI4 $304
line 719
;719:			break;
ADDRGP4 $297
JUMPV
LABELV $298
line 722
;720:		}
;721:
;722:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 723
;723:	}
LABELV $296
line 701
ADDRLP4 0
INDIRI4
ADDRLP4 4100
INDIRI4
LTI4 $295
LABELV $297
line 725
;724:
;725:	if (clTrapped)
ADDRLP4 4104
INDIRI4
CNSTI4 0
EQI4 $309
line 726
;726:	{
line 727
;727:		ent->r.contents = 0;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 0
ASGNI4
line 728
;728:		ent->s.fireflag = 0;
ADDRFP4 0
INDIRP4
CNSTI4 168
ADDP4
CNSTI4 0
ASGNI4
line 729
;729:		ent->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 730
;730:		return;
ADDRGP4 $290
JUMPV
LABELV $309
line 733
;731:	}
;732:	else
;733:	{
line 734
;734:		ent->r.contents = CONTENTS_SOLID;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1
ASGNI4
line 735
;735:	}
line 737
;736:
;737:	if (!g_entities[ent->boltpoint3].inuse || !g_entities[ent->boltpoint3].client ||
ADDRLP4 4232
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4236
CNSTI4 828
ADDRLP4 4232
INDIRP4
CNSTI4 776
ADDP4
INDIRI4
MULI4
ASGNI4
ADDRLP4 4236
INDIRI4
ADDRGP4 g_entities+412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $318
ADDRLP4 4236
INDIRI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $318
ADDRLP4 4236
INDIRI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ADDRLP4 4232
INDIRP4
CNSTI4 772
ADDP4
INDIRI4
EQI4 $312
LABELV $318
line 739
;738:		g_entities[ent->boltpoint3].client->sess.sessionTeam != ent->boltpoint2)
;739:	{
line 740
;740:		ent->think = G_FreeEntity;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 G_FreeEntity
ASGNP4
line 741
;741:		ent->nextthink = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 742
;742:		return;
ADDRGP4 $290
JUMPV
LABELV $312
line 747
;743:	}
;744:
;745://	G_RunObject(ent);
;746:
;747:	if ( !ent->damage )
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
INDIRI4
CNSTI4 0
NEI4 $320
line 748
;748:	{
line 749
;749:		ent->damage = 1;
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
CNSTI4 1
ASGNI4
line 750
;750:		ent->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 751
;751:		return;
ADDRGP4 $290
JUMPV
LABELV $320
line 754
;752:	}
;753:
;754:	if ((ent->bolt_LLeg+TURRET_LIFETIME) < level.time)
ADDRFP4 0
INDIRP4
CNSTI4 796
ADDP4
INDIRI4
CNSTI4 60000
ADDI4
ADDRGP4 level+32
INDIRI4
GEI4 $323
line 755
;755:	{
line 756
;756:		G_Sound( ent, CHAN_BODY, G_SoundIndex( "sound/chars/turret/shutdown.wav" ));
ADDRGP4 $286
ARGP4
ADDRLP4 4240
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 5
ARGI4
ADDRLP4 4240
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 757
;757:		ent->s.bolt2 = ENTITYNUM_NONE;
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
CNSTI4 1023
ASGNI4
line 758
;758:		ent->s.fireflag = 2;
ADDRFP4 0
INDIRP4
CNSTI4 168
ADDP4
CNSTI4 2
ASGNI4
line 760
;759:
;760:		ent->think = sentryExpire;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 sentryExpire
ASGNP4
line 761
;761:		ent->nextthink = level.time + TURRET_DEATH_DELAY;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 2000
ADDI4
ASGNI4
line 762
;762:		return;
ADDRGP4 $290
JUMPV
LABELV $323
line 765
;763:	}
;764:
;765:	ent->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 767
;766:
;767:	if ( ent->enemy )
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $328
line 768
;768:	{
line 770
;769:		// make sure that the enemy is still valid
;770:		pas_adjust_enemy( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 pas_adjust_enemy
CALLV
pop
line 771
;771:	}
LABELV $328
line 773
;772:
;773:	if (ent->enemy)
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $330
line 774
;774:	{
line 775
;775:		if (!ent->enemy->client)
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $332
line 776
;776:		{
line 777
;777:			ent->enemy = NULL;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
CNSTP4 0
ASGNP4
line 778
;778:		}
ADDRGP4 $333
JUMPV
LABELV $332
line 779
;779:		else if (ent->enemy->s.number == ent->s.number)
ADDRLP4 4240
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4240
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
INDIRI4
ADDRLP4 4240
INDIRP4
INDIRI4
NEI4 $334
line 780
;780:		{
line 781
;781:			ent->enemy = NULL;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
CNSTP4 0
ASGNP4
line 782
;782:		}
ADDRGP4 $335
JUMPV
LABELV $334
line 783
;783:		else if (ent->enemy->health < 1)
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 1
GEI4 $336
line 784
;784:		{
line 785
;785:			ent->enemy = NULL;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
CNSTP4 0
ASGNP4
line 786
;786:		}
LABELV $336
LABELV $335
LABELV $333
line 787
;787:	}
LABELV $330
line 789
;788:
;789:	if ( !ent->enemy )
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $338
line 790
;790:	{
line 791
;791:		pas_find_enemies( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 pas_find_enemies
CALLI4
pop
line 792
;792:	}
LABELV $338
line 794
;793:
;794:	if (ent->enemy)
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $340
line 795
;795:	{
line 796
;796:		ent->s.bolt2 = ent->enemy->s.number;
ADDRLP4 4240
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4240
INDIRP4
CNSTI4 144
ADDP4
ADDRLP4 4240
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
INDIRI4
ASGNI4
line 797
;797:	}
ADDRGP4 $341
JUMPV
LABELV $340
line 799
;798:	else
;799:	{
line 800
;800:		ent->s.bolt2 = ENTITYNUM_NONE;
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
CNSTI4 1023
ASGNI4
line 801
;801:	}
LABELV $341
line 803
;802:
;803:	moved = qfalse;
ADDRLP4 4140
CNSTI4 0
ASGNI4
line 804
;804:	diffYaw = 0.0f; diffPitch = 0.0f;
ADDRLP4 4108
CNSTF4 0
ASGNF4
ADDRLP4 4136
CNSTF4 0
ASGNF4
line 806
;805:
;806:	ent->speed = AngleNormalize360( ent->speed );
ADDRLP4 4240
ADDRFP4 0
INDIRP4
CNSTI4 608
ADDP4
ASGNP4
ADDRLP4 4240
INDIRP4
INDIRF4
ARGF4
ADDRLP4 4244
ADDRGP4 AngleNormalize360
CALLF4
ASGNF4
ADDRLP4 4240
INDIRP4
ADDRLP4 4244
INDIRF4
ASGNF4
line 807
;807:	ent->random = AngleNormalize360( ent->random );
ADDRLP4 4248
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
ASGNP4
ADDRLP4 4248
INDIRP4
INDIRF4
ARGF4
ADDRLP4 4252
ADDRGP4 AngleNormalize360
CALLF4
ASGNF4
ADDRLP4 4248
INDIRP4
ADDRLP4 4252
INDIRF4
ASGNF4
line 809
;808:
;809:	if ( ent->enemy )
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $342
line 810
;810:	{
line 813
;811:		// ...then we'll calculate what new aim adjustments we should attempt to make this frame
;812:		// Aim at enemy
;813:		if ( ent->enemy->client )
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $344
line 814
;814:		{
line 815
;815:			VectorCopy( ent->enemy->client->ps.origin, org );
ADDRLP4 4180
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 816
;816:		}
ADDRGP4 $345
JUMPV
LABELV $344
line 818
;817:		else
;818:		{
line 819
;819:			VectorCopy( ent->enemy->r.currentOrigin, org );
ADDRLP4 4180
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 820
;820:		}
LABELV $345
line 822
;821:
;822:		VectorSubtract( org, ent->r.currentOrigin, enemyDir );
ADDRLP4 4256
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4168
ADDRLP4 4180
INDIRF4
ADDRLP4 4256
INDIRP4
CNSTI4 368
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4168+4
ADDRLP4 4180+4
INDIRF4
ADDRLP4 4256
INDIRP4
CNSTI4 372
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4168+8
ADDRLP4 4180+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 376
ADDP4
INDIRF4
SUBF4
ASGNF4
line 823
;823:		vectoangles( enemyDir, desiredAngles );
ADDRLP4 4168
ARGP4
ADDRLP4 4192
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 825
;824:
;825:		diffYaw = AngleSubtract( ent->speed, desiredAngles[YAW] );
ADDRFP4 0
INDIRP4
CNSTI4 608
ADDP4
INDIRF4
ARGF4
ADDRLP4 4192+4
INDIRF4
ARGF4
ADDRLP4 4260
ADDRGP4 AngleSubtract
CALLF4
ASGNF4
ADDRLP4 4108
ADDRLP4 4260
INDIRF4
ASGNF4
line 826
;826:		diffPitch = AngleSubtract( ent->random, desiredAngles[PITCH] );
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
INDIRF4
ARGF4
ADDRLP4 4192
INDIRF4
ARGF4
ADDRLP4 4264
ADDRGP4 AngleSubtract
CALLF4
ASGNF4
ADDRLP4 4136
ADDRLP4 4264
INDIRF4
ASGNF4
line 827
;827:	}
ADDRGP4 $343
JUMPV
LABELV $342
line 829
;828:	else
;829:	{
line 831
;830:		// no enemy, so make us slowly sweep back and forth as if searching for a new one
;831:		diffYaw = sin( level.time * 0.0001f + ent->count ) * 2.0f;
CNSTF4 953267991
ADDRGP4 level+32
INDIRI4
CVIF4 4
MULF4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CVIF4 4
ADDF4
ARGF4
ADDRLP4 4256
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 4108
CNSTF4 1073741824
ADDRLP4 4256
INDIRF4
MULF4
ASGNF4
line 832
;832:	}
LABELV $343
line 834
;833:
;834:	if ( fabs(diffYaw) > 0.25f )
ADDRLP4 4108
INDIRF4
ARGF4
ADDRLP4 4256
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 4256
INDIRF4
CNSTF4 1048576000
LEF4 $352
line 835
;835:	{
line 836
;836:		moved = qtrue;
ADDRLP4 4140
CNSTI4 1
ASGNI4
line 838
;837:
;838:		if ( fabs(diffYaw) > 10.0f )
ADDRLP4 4108
INDIRF4
ARGF4
ADDRLP4 4260
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 4260
INDIRF4
CNSTF4 1092616192
LEF4 $354
line 839
;839:		{
line 841
;840:			// cap max speed
;841:			ent->speed += (diffYaw > 0.0f) ? -10.0f : 10.0f;
ADDRLP4 4108
INDIRF4
CNSTF4 0
LEF4 $357
ADDRLP4 4264
CNSTF4 3240099840
ASGNF4
ADDRGP4 $358
JUMPV
LABELV $357
ADDRLP4 4264
CNSTF4 1092616192
ASGNF4
LABELV $358
ADDRLP4 4268
ADDRFP4 0
INDIRP4
CNSTI4 608
ADDP4
ASGNP4
ADDRLP4 4268
INDIRP4
ADDRLP4 4268
INDIRP4
INDIRF4
ADDRLP4 4264
INDIRF4
ADDF4
ASGNF4
line 842
;842:		}
ADDRGP4 $355
JUMPV
LABELV $354
line 844
;843:		else
;844:		{
line 846
;845:			// small enough
;846:			ent->speed -= diffYaw;
ADDRLP4 4264
ADDRFP4 0
INDIRP4
CNSTI4 608
ADDP4
ASGNP4
ADDRLP4 4264
INDIRP4
ADDRLP4 4264
INDIRP4
INDIRF4
ADDRLP4 4108
INDIRF4
SUBF4
ASGNF4
line 847
;847:		}
LABELV $355
line 848
;848:	}
LABELV $352
line 851
;849:
;850:
;851:	if ( fabs(diffPitch) > 0.25f )
ADDRLP4 4136
INDIRF4
ARGF4
ADDRLP4 4260
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 4260
INDIRF4
CNSTF4 1048576000
LEF4 $359
line 852
;852:	{
line 853
;853:		moved = qtrue;
ADDRLP4 4140
CNSTI4 1
ASGNI4
line 855
;854:
;855:		if ( fabs(diffPitch) > 4.0f )
ADDRLP4 4136
INDIRF4
ARGF4
ADDRLP4 4264
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 4264
INDIRF4
CNSTF4 1082130432
LEF4 $361
line 856
;856:		{
line 858
;857:			// cap max speed
;858:			ent->random += (diffPitch > 0.0f) ? -4.0f : 4.0f;
ADDRLP4 4136
INDIRF4
CNSTF4 0
LEF4 $364
ADDRLP4 4268
CNSTF4 3229614080
ASGNF4
ADDRGP4 $365
JUMPV
LABELV $364
ADDRLP4 4268
CNSTF4 1082130432
ASGNF4
LABELV $365
ADDRLP4 4272
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
ASGNP4
ADDRLP4 4272
INDIRP4
ADDRLP4 4272
INDIRP4
INDIRF4
ADDRLP4 4268
INDIRF4
ADDF4
ASGNF4
line 859
;859:		}
ADDRGP4 $362
JUMPV
LABELV $361
line 861
;860:		else
;861:		{
line 863
;862:			// small enough
;863:			ent->random -= diffPitch;
ADDRLP4 4268
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
ASGNP4
ADDRLP4 4268
INDIRP4
ADDRLP4 4268
INDIRP4
INDIRF4
ADDRLP4 4136
INDIRF4
SUBF4
ASGNF4
line 864
;864:		}
LABELV $362
line 865
;865:	}
LABELV $359
line 868
;866:
;867:	// the bone axes are messed up, so hence some dumbness here
;868:	VectorSet( frontAngles, -ent->random, 0.0f, 0.0f );
ADDRLP4 4144
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
INDIRF4
NEGF4
ASGNF4
ADDRLP4 4144+4
CNSTF4 0
ASGNF4
ADDRLP4 4144+8
CNSTF4 0
ASGNF4
line 869
;869:	VectorSet( backAngles, 0.0f, 0.0f, ent->speed );
ADDRLP4 4264
CNSTF4 0
ASGNF4
ADDRLP4 4156
ADDRLP4 4264
INDIRF4
ASGNF4
ADDRLP4 4156+4
ADDRLP4 4264
INDIRF4
ASGNF4
ADDRLP4 4156+8
ADDRFP4 0
INDIRP4
CNSTI4 608
ADDP4
INDIRF4
ASGNF4
line 871
;870:
;871:	if ( moved )
ADDRLP4 4140
INDIRI4
CNSTI4 0
EQI4 $370
line 872
;872:	{
line 874
;873:	//ent->s.loopSound = G_SoundIndex( "sound/chars/turret/move.wav" );
;874:	}
ADDRGP4 $371
JUMPV
LABELV $370
line 876
;875:	else
;876:	{
line 877
;877:		ent->s.loopSound = 0;
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
CNSTI4 0
ASGNI4
line 878
;878:	}
LABELV $371
line 880
;879:
;880:	if ( ent->enemy && ent->attackDebounceTime < level.time )
ADDRLP4 4268
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4268
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $372
ADDRLP4 4268
INDIRP4
CNSTI4 456
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $372
line 881
;881:	{
line 882
;882:		ent->count--;
ADDRLP4 4272
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ASGNP4
ADDRLP4 4272
INDIRP4
ADDRLP4 4272
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 884
;883:
;884:		if ( ent->count )
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
EQI4 $375
line 885
;885:		{
line 886
;886:			pas_fire( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 pas_fire
CALLV
pop
line 887
;887:			ent->s.fireflag = 1;
ADDRFP4 0
INDIRP4
CNSTI4 168
ADDP4
CNSTI4 1
ASGNI4
line 888
;888:			ent->attackDebounceTime = level.time + 200;
ADDRFP4 0
INDIRP4
CNSTI4 456
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 200
ADDI4
ASGNI4
line 889
;889:		}
ADDRGP4 $373
JUMPV
LABELV $375
line 891
;890:		else
;891:		{
line 893
;892:			//ent->nextthink = 0;
;893:			G_Sound( ent, CHAN_BODY, G_SoundIndex( "sound/chars/turret/shutdown.wav" ));
ADDRGP4 $286
ARGP4
ADDRLP4 4276
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 5
ARGI4
ADDRLP4 4276
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 894
;894:			ent->s.bolt2 = ENTITYNUM_NONE;
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
CNSTI4 1023
ASGNI4
line 895
;895:			ent->s.fireflag = 2;
ADDRFP4 0
INDIRP4
CNSTI4 168
ADDP4
CNSTI4 2
ASGNI4
line 897
;896:
;897:			ent->think = sentryExpire;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 sentryExpire
ASGNP4
line 898
;898:			ent->nextthink = level.time + TURRET_DEATH_DELAY;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 2000
ADDI4
ASGNI4
line 899
;899:		}
line 900
;900:	}
ADDRGP4 $373
JUMPV
LABELV $372
line 902
;901:	else
;902:	{
line 903
;903:		ent->s.fireflag = 0;
ADDRFP4 0
INDIRP4
CNSTI4 168
ADDP4
CNSTI4 0
ASGNI4
line 904
;904:	}
LABELV $373
line 905
;905:}
LABELV $290
endproc pas_think 4280 16
export turret_die
proc turret_die 12 24
line 910
;906:
;907://------------------------------------------------------------------------------------------------------------
;908:void turret_die(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod)
;909://------------------------------------------------------------------------------------------------------------
;910:{
line 912
;911:	// Turn off the thinking of the base & use it's targets
;912:	self->think = 0;//NULL;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
CNSTP4 0
ASGNP4
line 913
;913:	self->use = 0;//NULL;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
CNSTP4 0
ASGNP4
line 915
;914:
;915:	if ( self->target )
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $380
line 916
;916:	{
line 917
;917:		G_UseTargets( self, attacker );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
line 918
;918:	}
LABELV $380
line 920
;919:
;920:	if (!g_entities[self->boltpoint3].inuse || !g_entities[self->boltpoint3].client)
ADDRLP4 0
CNSTI4 828
ADDRFP4 0
INDIRP4
CNSTI4 776
ADDP4
INDIRI4
MULI4
ASGNI4
ADDRLP4 0
INDIRI4
ADDRGP4 g_entities+412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $386
ADDRLP4 0
INDIRI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $382
LABELV $386
line 921
;921:	{
line 922
;922:		G_FreeEntity(self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 923
;923:		return;
ADDRGP4 $379
JUMPV
LABELV $382
line 927
;924:	}
;925:
;926:	// clear my data
;927:	self->die  = 0;//NULL;
ADDRFP4 0
INDIRP4
CNSTI4 660
ADDP4
CNSTP4 0
ASGNP4
line 928
;928:	self->takedamage = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 0
ASGNI4
line 929
;929:	self->health = 0;
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 0
ASGNI4
line 932
;930:
;931:	// hack the effect angle so that explode death can orient the effect properly
;932:	VectorSet( self->s.angles, 0, 0, 1 );
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
CNSTF4 0
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 120
ADDP4
CNSTF4 0
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
CNSTF4 1065353216
ASGNF4
line 934
;933:
;934:	G_PlayEffect(EFFECT_EXPLOSION_PAS, self->s.pos.trBase, self->s.angles);
CNSTI4 3
ARGI4
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 24
ADDP4
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 116
ADDP4
ARGP4
ADDRGP4 G_PlayEffect
CALLP4
pop
line 935
;935:	G_RadiusDamage(self->s.pos.trBase, &g_entities[self->boltpoint3], 30, 256, self, MOD_UNKNOWN);
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 24
ADDP4
ARGP4
CNSTI4 828
ADDRLP4 8
INDIRP4
CNSTI4 776
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
CNSTF4 1106247680
ARGF4
CNSTF4 1132462080
ARGF4
ADDRLP4 8
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 G_RadiusDamage
CALLI4
pop
line 937
;936:
;937:	g_entities[self->boltpoint3].client->ps.fd.sentryDeployed = qfalse;
CNSTI4 828
ADDRFP4 0
INDIRP4
CNSTI4 776
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CNSTI4 1212
ADDP4
CNSTI4 0
ASGNI4
line 940
;938:
;939:	//ExplodeDeath( self );
;940:	G_FreeEntity( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 941
;941:}
LABELV $379
endproc turret_die 12 24
export SP_PAS
proc SP_PAS 4 12
line 948
;942:
;943:#define TURRET_AMMO_COUNT 40
;944:
;945://---------------------------------
;946:void SP_PAS( gentity_t *base )
;947://---------------------------------
;948:{
line 949
;949:	if ( base->count == 0 )
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
NEI4 $389
line 950
;950:	{
line 952
;951:		// give ammo
;952:		base->count = TURRET_AMMO_COUNT;
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
CNSTI4 40
ASGNI4
line 953
;953:	}
LABELV $389
line 955
;954:
;955:	base->s.bolt1 = 1; //This is a sort of hack to indicate that this model needs special turret things done to it
ADDRFP4 0
INDIRP4
CNSTI4 140
ADDP4
CNSTI4 1
ASGNI4
line 956
;956:	base->s.bolt2 = ENTITYNUM_NONE; //store our current enemy index
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
CNSTI4 1023
ASGNI4
line 958
;957:
;958:	base->damage = 0; // start animation flag
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
CNSTI4 0
ASGNI4
line 960
;959:
;960:	VectorSet( base->r.mins, -8, -8, 0 );
ADDRFP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 0
ASGNF4
line 961
;961:	VectorSet( base->r.maxs, 8, 8, 24 );
ADDRFP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1103101952
ASGNF4
line 963
;962:
;963:	G_RunObject(base);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_RunObject
CALLV
pop
line 965
;964:
;965:	base->think = pas_think;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 pas_think
ASGNP4
line 966
;966:	base->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 968
;967:
;968:	if ( !base->health )
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 0
NEI4 $392
line 969
;969:	{
line 970
;970:		base->health = 50;
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 50
ASGNI4
line 971
;971:	}
LABELV $392
line 973
;972:
;973:	base->takedamage = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 1
ASGNI4
line 974
;974:	base->die  = turret_die;
ADDRFP4 0
INDIRP4
CNSTI4 660
ADDP4
ADDRGP4 turret_die
ASGNP4
line 976
;975:
;976:	base->physicsObject = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 500
ADDP4
CNSTI4 1
ASGNI4
line 978
;977:
;978:	G_Sound( base, CHAN_BODY, G_SoundIndex( "sound/chars/turret/startup.wav" ));
ADDRGP4 $262
ARGP4
ADDRLP4 0
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 5
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 979
;979:}
LABELV $388
endproc SP_PAS 4 12
export ItemUse_Sentry
proc ItemUse_Sentry 116 16
line 984
;980:
;981://------------------------------------------------------------------------
;982:void ItemUse_Sentry( gentity_t *ent )
;983://------------------------------------------------------------------------
;984:{
line 990
;985:	vec3_t fwd, fwdorg;
;986:	vec3_t yawonly;
;987:	vec3_t mins, maxs;
;988:	gentity_t *sentry;
;989:
;990:	if (!ent || !ent->client)
ADDRLP4 64
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 68
CNSTU4 0
ASGNU4
ADDRLP4 64
INDIRP4
CVPU4 4
ADDRLP4 68
INDIRU4
EQU4 $397
ADDRLP4 64
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 68
INDIRU4
NEU4 $395
LABELV $397
line 991
;991:	{
line 992
;992:		return;
ADDRGP4 $394
JUMPV
LABELV $395
line 995
;993:	}
;994:
;995:	VectorSet( mins, -8, -8, 0 );
ADDRLP4 72
CNSTF4 3238002688
ASGNF4
ADDRLP4 40
ADDRLP4 72
INDIRF4
ASGNF4
ADDRLP4 40+4
ADDRLP4 72
INDIRF4
ASGNF4
ADDRLP4 40+8
CNSTF4 0
ASGNF4
line 996
;996:	VectorSet( maxs, 8, 8, 24 );
ADDRLP4 76
CNSTF4 1090519040
ASGNF4
ADDRLP4 52
ADDRLP4 76
INDIRF4
ASGNF4
ADDRLP4 52+4
ADDRLP4 76
INDIRF4
ASGNF4
ADDRLP4 52+8
CNSTF4 1103101952
ASGNF4
line 999
;997:
;998:
;999:	yawonly[ROLL] = 0;
ADDRLP4 28+8
CNSTF4 0
ASGNF4
line 1000
;1000:	yawonly[PITCH] = 0;
ADDRLP4 28
CNSTF4 0
ASGNF4
line 1001
;1001:	yawonly[YAW] = ent->client->ps.viewangles[YAW];
ADDRLP4 28+4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 160
ADDP4
INDIRF4
ASGNF4
line 1003
;1002:
;1003:	AngleVectors(yawonly, fwd, NULL, NULL);
ADDRLP4 28
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 80
CNSTP4 0
ASGNP4
ADDRLP4 80
INDIRP4
ARGP4
ADDRLP4 80
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 1005
;1004:
;1005:	fwdorg[0] = ent->client->ps.origin[0] + fwd[0]*64;
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
CNSTF4 1115684864
ADDRLP4 4
INDIRF4
MULF4
ADDF4
ASGNF4
line 1006
;1006:	fwdorg[1] = ent->client->ps.origin[1] + fwd[1]*64;
ADDRLP4 16+4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
CNSTF4 1115684864
ADDRLP4 4+4
INDIRF4
MULF4
ADDF4
ASGNF4
line 1007
;1007:	fwdorg[2] = ent->client->ps.origin[2] + fwd[2]*64;
ADDRLP4 16+8
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
CNSTF4 1115684864
ADDRLP4 4+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 1009
;1008:
;1009:	sentry = G_Spawn();
ADDRLP4 84
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 84
INDIRP4
ASGNP4
line 1011
;1010:
;1011:	sentry->classname = "sentryGun";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $408
ASGNP4
line 1012
;1012:	sentry->s.modelindex = G_ModelIndex("models/items/psgun.glm"); //replace ASAP
ADDRGP4 $409
ARGP4
ADDRLP4 88
ADDRGP4 G_ModelIndex
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 212
ADDP4
ADDRLP4 88
INDIRI4
ASGNI4
line 1014
;1013:
;1014:	sentry->s.g2radius = 30.0f;
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
CNSTI4 30
ASGNI4
line 1015
;1015:	sentry->s.modelGhoul2 = 1;
ADDRLP4 0
INDIRP4
CNSTI4 204
ADDP4
CNSTI4 1
ASGNI4
line 1017
;1016:
;1017:	G_SetOrigin(sentry, fwdorg);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 16
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1018
;1018:	sentry->parent = ent;
ADDRLP4 0
INDIRP4
CNSTI4 536
ADDP4
ADDRFP4 0
INDIRP4
ASGNP4
line 1019
;1019:	sentry->r.contents = CONTENTS_SOLID;
ADDRLP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1
ASGNI4
line 1020
;1020:	sentry->s.solid = 2;
ADDRLP4 0
INDIRP4
CNSTI4 248
ADDP4
CNSTI4 2
ASGNI4
line 1021
;1021:	sentry->clipmask = MASK_SOLID;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 1
ASGNI4
line 1022
;1022:	VectorCopy(mins, sentry->r.mins);
ADDRLP4 0
INDIRP4
CNSTI4 316
ADDP4
ADDRLP4 40
INDIRB
ASGNB 12
line 1023
;1023:	VectorCopy(maxs, sentry->r.maxs);
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
ADDRLP4 52
INDIRB
ASGNB 12
line 1024
;1024:	sentry->boltpoint3 = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 776
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 1025
;1025:	sentry->boltpoint2 = ent->client->sess.sessionTeam; //so we can remove ourself if our owner changes teams
ADDRLP4 0
INDIRP4
CNSTI4 772
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ASGNI4
line 1026
;1026:	sentry->r.absmin[0] = sentry->s.pos.trBase[0] + sentry->r.mins[0];
ADDRLP4 0
INDIRP4
CNSTI4 344
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 316
ADDP4
INDIRF4
ADDF4
ASGNF4
line 1027
;1027:	sentry->r.absmin[1] = sentry->s.pos.trBase[1] + sentry->r.mins[1];
ADDRLP4 0
INDIRP4
CNSTI4 348
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 320
ADDP4
INDIRF4
ADDF4
ASGNF4
line 1028
;1028:	sentry->r.absmin[2] = sentry->s.pos.trBase[2] + sentry->r.mins[2];
ADDRLP4 0
INDIRP4
CNSTI4 352
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 324
ADDP4
INDIRF4
ADDF4
ASGNF4
line 1029
;1029:	sentry->r.absmax[0] = sentry->s.pos.trBase[0] + sentry->r.maxs[0];
ADDRLP4 0
INDIRP4
CNSTI4 356
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
INDIRF4
ADDF4
ASGNF4
line 1030
;1030:	sentry->r.absmax[1] = sentry->s.pos.trBase[1] + sentry->r.maxs[1];
ADDRLP4 0
INDIRP4
CNSTI4 360
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
INDIRF4
ADDF4
ASGNF4
line 1031
;1031:	sentry->r.absmax[2] = sentry->s.pos.trBase[2] + sentry->r.maxs[2];
ADDRLP4 0
INDIRP4
CNSTI4 364
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
INDIRF4
ADDF4
ASGNF4
line 1032
;1032:	sentry->s.eType = ET_GENERAL;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 1033
;1033:	sentry->s.pos.trType = TR_GRAVITY;//STATIONARY;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 5
ASGNI4
line 1034
;1034:	sentry->s.pos.trTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1035
;1035:	sentry->touch = SentryTouch;
ADDRLP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 SentryTouch
ASGNP4
line 1036
;1036:	sentry->nextthink = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1037
;1037:	sentry->boltpoint4 = ENTITYNUM_NONE; //boltpoint4 used as enemy index
ADDRLP4 0
INDIRP4
CNSTI4 780
ADDP4
CNSTI4 1023
ASGNI4
line 1039
;1038:
;1039:	sentry->bolt_Head = 1000;
ADDRLP4 0
INDIRP4
CNSTI4 784
ADDP4
CNSTI4 1000
ASGNI4
line 1041
;1040:
;1041:	sentry->bolt_LLeg = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 796
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1043
;1042:
;1043:	sentry->noDamageTeam = ent->client->sess.sessionTeam;
ADDRLP4 0
INDIRP4
CNSTI4 460
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ASGNI4
line 1045
;1044:
;1045:	ent->client->ps.fd.sentryDeployed = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1212
ADDP4
CNSTI4 1
ASGNI4
line 1047
;1046:
;1047:	trap_LinkEntity(sentry);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1049
;1048:
;1049:	sentry->s.owner = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 260
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 1050
;1050:	sentry->s.shouldtarget = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 268
ADDP4
CNSTI4 1
ASGNI4
line 1051
;1051:	if (g_gametype.integer >= GT_TEAM)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
LTI4 $413
line 1052
;1052:	{
line 1053
;1053:		sentry->s.teamowner = ent->client->sess.sessionTeam;
ADDRLP4 0
INDIRP4
CNSTI4 264
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ASGNI4
line 1054
;1054:	}
ADDRGP4 $414
JUMPV
LABELV $413
line 1056
;1055:	else
;1056:	{
line 1057
;1057:		sentry->s.teamowner = 16;
ADDRLP4 0
INDIRP4
CNSTI4 264
ADDP4
CNSTI4 16
ASGNI4
line 1058
;1058:	}
LABELV $414
line 1060
;1059:
;1060:	SP_PAS( sentry );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 SP_PAS
CALLV
pop
line 1061
;1061:}
LABELV $394
endproc ItemUse_Sentry 116 16
export ItemUse_Seeker
proc ItemUse_Seeker 4 0
line 1065
;1062:
;1063:
;1064:void ItemUse_Seeker(gentity_t *ent)
;1065:{
line 1066
;1066:	ent->client->ps.eFlags |= EF_SEEKERDRONE;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1048576
BORI4
ASGNI4
line 1068
;1067:
;1068:	ent->client->ps.droneExistTime = level.time + 30000;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 620
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 30000
ADDI4
CVIF4 4
ASGNF4
line 1069
;1069:	ent->client->ps.droneFireTime = level.time + 1500;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 616
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1500
ADDI4
CVIF4 4
ASGNF4
line 1070
;1070:}
LABELV $416
endproc ItemUse_Seeker 4 0
export ItemUse_MedPack
proc ItemUse_MedPack 36 0
line 1073
;1071:
;1072:void ItemUse_MedPack(gentity_t *ent)
;1073:{
line 1074
;1074:	if (!ent || !ent->client)
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
EQU4 $422
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
NEU4 $420
LABELV $422
line 1075
;1075:	{
line 1076
;1076:		return;
ADDRGP4 $419
JUMPV
LABELV $420
line 1079
;1077:	}
;1078:
;1079:	if (ent->health <= 0 ||
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRLP4 8
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $426
ADDRLP4 16
ADDRLP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $426
ADDRLP4 16
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
CNSTI4 1
BANDI4
ADDRLP4 12
INDIRI4
EQI4 $423
LABELV $426
line 1082
;1080:		ent->client->ps.stats[STAT_HEALTH] <= 0 ||
;1081:		(ent->client->ps.eFlags & EF_DEAD))
;1082:	{
line 1083
;1083:		return;
ADDRGP4 $419
JUMPV
LABELV $423
line 1086
;1084:	}
;1085:
;1086:	if (ent->health >= ent->client->ps.stats[STAT_MAX_HEALTH])
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
ADDRLP4 20
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
LTI4 $427
line 1087
;1087:	{
line 1088
;1088:		return;
ADDRGP4 $419
JUMPV
LABELV $427
line 1091
;1089:	}
;1090:
;1091:	ent->health += MAX_MEDPACK_HEAL_AMOUNT;
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 1093
;1092:
;1093:	if (ent->health > ent->client->ps.stats[STAT_MAX_HEALTH])
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
ADDRLP4 28
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
LEI4 $429
line 1094
;1094:	{
line 1095
;1095:		ent->health = ent->client->ps.stats[STAT_MAX_HEALTH];
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 676
ADDP4
ADDRLP4 32
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
ASGNI4
line 1096
;1096:	}
LABELV $429
line 1097
;1097:}
LABELV $419
endproc ItemUse_MedPack 36 0
export Pickup_Powerup
proc Pickup_Powerup 1156 28
line 1099
;1098:
;1099:int Pickup_Powerup( gentity_t *ent, gentity_t *other ) {
line 1104
;1100:	int			quantity;
;1101:	int			i;
;1102:	gclient_t	*client;
;1103:
;1104:	if ( !other->client->ps.powerups[ent->item->giTag] ) {
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 344
ADDP4
ADDP4
INDIRI4
CNSTI4 0
NEI4 $432
line 1107
;1105:		// round timing to seconds to make multiple powerup timers
;1106:		// count in sync
;1107:		other->client->ps.powerups[ent->item->giTag] = 
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 344
ADDP4
ADDP4
ADDRGP4 level+32
INDIRI4
ADDRGP4 level+32
INDIRI4
CNSTI4 1000
MODI4
SUBI4
ASGNI4
line 1110
;1108:			level.time - ( level.time % 1000 );
;1109:
;1110:		G_LogWeaponPowerup(other->s.number, ent->item->giTag);
ADDRFP4 4
INDIRP4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_LogWeaponPowerup
CALLV
pop
line 1111
;1111:	}
LABELV $432
line 1113
;1112:
;1113:	if ( ent->count ) {
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
EQI4 $436
line 1114
;1114:		quantity = ent->count;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 1115
;1115:	} else {
ADDRGP4 $437
JUMPV
LABELV $436
line 1116
;1116:		quantity = ent->item->quantity;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ASGNI4
line 1117
;1117:	}
LABELV $437
line 1119
;1118:
;1119:	other->client->ps.powerups[ent->item->giTag] += quantity * 1000;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 344
ADDP4
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1000
ADDRLP4 8
INDIRI4
MULI4
ADDI4
ASGNI4
line 1121
;1120:
;1121:	if (ent->item->giTag == PW_YSALAMIRI)
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 15
NEI4 $438
line 1122
;1122:	{
line 1123
;1123:		other->client->ps.powerups[PW_FORCE_ENLIGHTENED_LIGHT] = 0;
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 392
ADDP4
CNSTI4 0
ASGNI4
line 1124
;1124:		other->client->ps.powerups[PW_FORCE_ENLIGHTENED_DARK] = 0;
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 396
ADDP4
CNSTI4 0
ASGNI4
line 1125
;1125:		other->client->ps.powerups[PW_FORCE_BOON] = 0;
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 400
ADDP4
CNSTI4 0
ASGNI4
line 1126
;1126:	}
LABELV $438
line 1129
;1127:
;1128:	// give any nearby players a "denied" anti-reward
;1129:	for ( i = 0 ; i < level.maxclients ; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $443
JUMPV
LABELV $440
line 1135
;1130:		vec3_t		delta;
;1131:		float		len;
;1132:		vec3_t		forward;
;1133:		trace_t		tr;
;1134:
;1135:		client = &level.clients[i];
ADDRLP4 0
CNSTI4 1756
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 1136
;1136:		if ( client == other->client ) {
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
NEU4 $445
line 1137
;1137:			continue;
ADDRGP4 $441
JUMPV
LABELV $445
line 1139
;1138:		}
;1139:		if ( client->pers.connected == CON_DISCONNECTED ) {
ADDRLP4 0
INDIRP4
CNSTI4 1368
ADDP4
INDIRI4
CNSTI4 0
NEI4 $447
line 1140
;1140:			continue;
ADDRGP4 $441
JUMPV
LABELV $447
line 1142
;1141:		}
;1142:		if ( client->ps.stats[STAT_HEALTH] <= 0 ) {
ADDRLP4 0
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CNSTI4 0
GTI4 $449
line 1143
;1143:			continue;
ADDRGP4 $441
JUMPV
LABELV $449
line 1148
;1144:		}
;1145:
;1146:    // if same team in team game, no sound
;1147:    // cannot use OnSameTeam as it expects to g_entities, not clients
;1148:  	if ( g_gametype.integer >= GT_TEAM && other->client->sess.sessionTeam == client->sess.sessionTeam  ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
LTI4 $451
ADDRLP4 1124
CNSTI4 1520
ASGNI4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ADDRLP4 1124
INDIRI4
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
ADDRLP4 1124
INDIRI4
ADDP4
INDIRI4
NEI4 $451
line 1149
;1149:      continue;
ADDRGP4 $441
JUMPV
LABELV $451
line 1153
;1150:    }
;1151:
;1152:		// if too far away, no sound
;1153:		VectorSubtract( ent->s.pos.trBase, client->ps.origin, delta );
ADDRLP4 1128
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1132
CNSTI4 24
ASGNI4
ADDRLP4 16
ADDRLP4 1128
INDIRP4
ADDRLP4 1132
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 16+4
ADDRLP4 1128
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
ADDRLP4 1132
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 16+8
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1154
;1154:		len = VectorNormalize( delta );
ADDRLP4 16
ARGP4
ADDRLP4 1140
ADDRGP4 VectorNormalize
CALLF4
ASGNF4
ADDRLP4 40
ADDRLP4 1140
INDIRF4
ASGNF4
line 1155
;1155:		if ( len > 192 ) {
ADDRLP4 40
INDIRF4
CNSTF4 1128267776
LEF4 $456
line 1156
;1156:			continue;
ADDRGP4 $441
JUMPV
LABELV $456
line 1160
;1157:		}
;1158:
;1159:		// if not facing, no sound
;1160:		AngleVectors( client->ps.viewangles, forward, NULL, NULL );
ADDRLP4 0
INDIRP4
CNSTI4 156
ADDP4
ARGP4
ADDRLP4 28
ARGP4
ADDRLP4 1144
CNSTP4 0
ASGNP4
ADDRLP4 1144
INDIRP4
ARGP4
ADDRLP4 1144
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 1161
;1161:		if ( DotProduct( delta, forward ) < 0.4 ) {
ADDRLP4 16
INDIRF4
ADDRLP4 28
INDIRF4
MULF4
ADDRLP4 16+4
INDIRF4
ADDRLP4 28+4
INDIRF4
MULF4
ADDF4
ADDRLP4 16+8
INDIRF4
ADDRLP4 28+8
INDIRF4
MULF4
ADDF4
CNSTF4 1053609165
GEF4 $458
line 1162
;1162:			continue;
ADDRGP4 $441
JUMPV
LABELV $458
line 1166
;1163:		}
;1164:
;1165:		// if not line of sight, no sound
;1166:		trap_Trace( &tr, client->ps.origin, NULL, NULL, ent->s.pos.trBase, ENTITYNUM_NONE, CONTENTS_SOLID );
ADDRLP4 44
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 1148
CNSTP4 0
ASGNP4
ADDRLP4 1148
INDIRP4
ARGP4
ADDRLP4 1148
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
ARGP4
CNSTI4 1023
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 1167
;1167:		if ( tr.fraction != 1.0 ) {
ADDRLP4 44+8
INDIRF4
CNSTF4 1065353216
EQF4 $464
line 1168
;1168:			continue;
ADDRGP4 $441
JUMPV
LABELV $464
line 1172
;1169:		}
;1170:
;1171:		// anti-reward
;1172:		client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_DENIEDREWARD;
ADDRLP4 1152
ADDRLP4 0
INDIRP4
CNSTI4 300
ADDP4
ASGNP4
ADDRLP4 1152
INDIRP4
ADDRLP4 1152
INDIRP4
INDIRI4
CNSTI4 1
BXORI4
ASGNI4
line 1173
;1173:	}
LABELV $441
line 1129
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $443
ADDRLP4 4
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $440
line 1174
;1174:	return RESPAWN_POWERUP;
CNSTI4 120
RETI4
LABELV $431
endproc Pickup_Powerup 1156 28
export Pickup_Holdable
proc Pickup_Holdable 12 12
line 1179
;1175:}
;1176:
;1177://======================================================================
;1178:
;1179:int Pickup_Holdable( gentity_t *ent, gentity_t *other ) {
line 1181
;1180:
;1181:	other->client->ps.stats[STAT_HOLDABLE_ITEM] = ent->item - bg_itemlist;
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 220
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CVPU4 4
ADDRGP4 bg_itemlist
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 52
DIVI4
ASGNI4
line 1183
;1182:
;1183:	other->client->ps.stats[STAT_HOLDABLE_ITEMS] |= (1 << ent->item->giTag);
ADDRLP4 0
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 224
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
LSHI4
BORI4
ASGNI4
line 1185
;1184:
;1185:	G_LogWeaponItem(other->s.number, ent->item->giTag);
ADDRFP4 4
INDIRP4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_LogWeaponItem
CALLV
pop
line 1187
;1186:
;1187:	return adjustRespawnTime(RESPAWN_HOLDABLE, ent->item->giType, ent->item->giTag);
CNSTF4 1114636288
ARGF4
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 adjustRespawnTime
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
RETI4
LABELV $467
endproc Pickup_Holdable 12 12
export Add_Ammo
proc Add_Ammo 24 0
line 1194
;1188:}
;1189:
;1190:
;1191://======================================================================
;1192:
;1193:void Add_Ammo (gentity_t *ent, int weapon, int count)
;1194:{
line 1195
;1195:	ent->client->ps.ammo[weapon] += count;
ADDRLP4 0
CNSTI4 408
ASGNI4
ADDRLP4 4
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRI4
ADDP4
INDIRP4
ADDRLP4 0
INDIRI4
ADDP4
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
ADDRFP4 8
INDIRI4
ADDI4
ASGNI4
line 1196
;1196:	if ( ent->client->ps.ammo[weapon] > ammoData[weapon].max ) {
ADDRLP4 8
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 12
CNSTI4 408
ASGNI4
ADDRLP4 8
INDIRI4
ADDRFP4 0
INDIRP4
ADDRLP4 12
INDIRI4
ADDP4
INDIRP4
ADDRLP4 12
INDIRI4
ADDP4
ADDP4
INDIRI4
ADDRLP4 8
INDIRI4
ADDRGP4 ammoData
ADDP4
INDIRI4
LEI4 $469
line 1197
;1197:		ent->client->ps.ammo[weapon] = ammoData[weapon].max;
ADDRLP4 16
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 20
CNSTI4 408
ASGNI4
ADDRLP4 16
INDIRI4
ADDRFP4 0
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
ADDP4
ADDRLP4 16
INDIRI4
ADDRGP4 ammoData
ADDP4
INDIRI4
ASGNI4
line 1198
;1198:	}
LABELV $469
line 1199
;1199:}
LABELV $468
endproc Add_Ammo 24 0
export Pickup_Ammo
proc Pickup_Ammo 12 12
line 1202
;1200:
;1201:int Pickup_Ammo (gentity_t *ent, gentity_t *other)
;1202:{
line 1205
;1203:	int		quantity;
;1204:
;1205:	if ( ent->count ) {
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
EQI4 $472
line 1206
;1206:		quantity = ent->count;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 1207
;1207:	} else {
ADDRGP4 $473
JUMPV
LABELV $472
line 1208
;1208:		quantity = ent->item->quantity;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ASGNI4
line 1209
;1209:	}
LABELV $473
line 1211
;1210:
;1211:	Add_Ammo (other, ent->item->giTag, quantity);
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 Add_Ammo
CALLV
pop
line 1213
;1212:
;1213:	return adjustRespawnTime(RESPAWN_AMMO, ent->item->giType, ent->item->giTag);
CNSTF4 1109393408
ARGF4
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
ADDRGP4 adjustRespawnTime
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
RETI4
LABELV $471
endproc Pickup_Ammo 12 12
export Pickup_Weapon
proc Pickup_Weapon 16 12
line 1219
;1214:}
;1215:
;1216://======================================================================
;1217:
;1218:
;1219:int Pickup_Weapon (gentity_t *ent, gentity_t *other) {
line 1222
;1220:	int		quantity;
;1221:
;1222:	if ( ent->count < 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
GEI4 $475
line 1223
;1223:		quantity = 0; // None for you, sir!
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1224
;1224:	} else {
ADDRGP4 $476
JUMPV
LABELV $475
line 1225
;1225:		if ( ent->count ) {
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
EQI4 $477
line 1226
;1226:			quantity = ent->count;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 1227
;1227:		} else {
ADDRGP4 $478
JUMPV
LABELV $477
line 1228
;1228:			quantity = ent->item->quantity;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ASGNI4
line 1229
;1229:		}
LABELV $478
line 1232
;1230:
;1231:		// dropped items and teamplay weapons always have full ammo
;1232:		if ( ! (ent->flags & FL_DROPPED_ITEM) && g_gametype.integer != GT_TEAM ) {
ADDRFP4 0
INDIRP4
CNSTI4 472
ADDP4
INDIRI4
CNSTI4 4096
BANDI4
CNSTI4 0
NEI4 $479
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
EQI4 $479
line 1238
;1233:			// respawning rules
;1234:
;1235:			// New method:  If the player has less than half the minimum, give them the minimum, else add 1/2 the min.
;1236:
;1237:			// drop the quantity if the already have over the minimum
;1238:			if ( other->client->ps.ammo[ ent->item->giTag ] < quantity*0.5 ) {
ADDRLP4 4
CNSTI4 408
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
ADDRLP4 4
INDIRI4
ADDP4
INDIRP4
ADDRLP4 4
INDIRI4
ADDP4
ADDP4
INDIRI4
CVIF4 4
CNSTF4 1056964608
ADDRLP4 0
INDIRI4
CVIF4 4
MULF4
GEF4 $482
line 1239
;1239:				quantity = quantity - other->client->ps.ammo[ ent->item->giTag ];
ADDRLP4 8
CNSTI4 408
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
ADDP4
INDIRI4
SUBI4
ASGNI4
line 1240
;1240:			} else {
ADDRGP4 $483
JUMPV
LABELV $482
line 1241
;1241:				quantity = quantity*0.5;		// only add half the value.
ADDRLP4 0
CNSTF4 1056964608
ADDRLP4 0
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 1242
;1242:			}
LABELV $483
line 1253
;1243:
;1244:			// Old method:  If the player has less than the minimum, give them the minimum, else just add 1.
;1245:/*
;1246:			// drop the quantity if the already have over the minimum
;1247:			if ( other->client->ps.ammo[ ent->item->giTag ] < quantity ) {
;1248:				quantity = quantity - other->client->ps.ammo[ ent->item->giTag ];
;1249:			} else {
;1250:				quantity = 1;		// only add a single shot
;1251:			}
;1252:			*/
;1253:		}
LABELV $479
line 1254
;1254:	}
LABELV $476
line 1257
;1255:
;1256:	// add the weapon
;1257:	other->client->ps.stats[STAT_WEAPONS] |= ( 1 << ent->item->giTag );
ADDRLP4 4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 232
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
LSHI4
BORI4
ASGNI4
line 1260
;1258:
;1259:	//Add_Ammo( other, ent->item->giTag, quantity );
;1260:	Add_Ammo( other, weaponData[ent->item->giTag].ammoIndex, quantity );
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 56
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
MULI4
ADDRGP4 weaponData
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 Add_Ammo
CALLV
pop
line 1262
;1261:
;1262:	G_LogWeaponPickup(other->s.number, ent->item->giTag);
ADDRFP4 4
INDIRP4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_LogWeaponPickup
CALLV
pop
line 1265
;1263:	
;1264:	// team deathmatch has slow weapon respawns
;1265:	if ( g_gametype.integer == GT_TEAM ) 
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
NEI4 $484
line 1266
;1266:	{
line 1267
;1267:		return adjustRespawnTime(RESPAWN_TEAM_WEAPON, ent->item->giType, ent->item->giTag);
CNSTF4 1106247680
ARGF4
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 adjustRespawnTime
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
RETI4
ADDRGP4 $474
JUMPV
LABELV $484
line 1270
;1268:	}
;1269:
;1270:	return adjustRespawnTime(g_weaponRespawn.integer, ent->item->giType, ent->item->giTag);
ADDRGP4 g_weaponRespawn+12
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 adjustRespawnTime
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
RETI4
LABELV $474
endproc Pickup_Weapon 16 12
export Pickup_Health
proc Pickup_Health 28 12
line 1276
;1271:}
;1272:
;1273:
;1274://======================================================================
;1275:
;1276:int Pickup_Health (gentity_t *ent, gentity_t *other) {
line 1281
;1277:	int			max;
;1278:	int			quantity;
;1279:
;1280:	// small and mega healths will go over the max
;1281:	if ( ent->item->quantity != 5 && ent->item->quantity != 100 ) {
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 5
EQI4 $489
ADDRLP4 8
INDIRI4
CNSTI4 100
EQI4 $489
line 1282
;1282:		max = other->client->ps.stats[STAT_MAX_HEALTH];
ADDRLP4 0
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
ASGNI4
line 1283
;1283:	} else {
ADDRGP4 $490
JUMPV
LABELV $489
line 1284
;1284:		max = other->client->ps.stats[STAT_MAX_HEALTH] * 2;
ADDRLP4 0
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
CNSTI4 1
LSHI4
ASGNI4
line 1285
;1285:	}
LABELV $490
line 1287
;1286:
;1287:	if ( ent->count ) {
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
EQI4 $491
line 1288
;1288:		quantity = ent->count;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 1289
;1289:	} else {
ADDRGP4 $492
JUMPV
LABELV $491
line 1290
;1290:		quantity = ent->item->quantity;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ASGNI4
line 1291
;1291:	}
LABELV $492
line 1293
;1292:
;1293:	other->health += quantity;
ADDRLP4 12
ADDRFP4 4
INDIRP4
CNSTI4 676
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ASGNI4
line 1295
;1294:
;1295:	if (other->health > max ) {
ADDRFP4 4
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
ADDRLP4 0
INDIRI4
LEI4 $493
line 1296
;1296:		other->health = max;
ADDRFP4 4
INDIRP4
CNSTI4 676
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 1297
;1297:	}
LABELV $493
line 1298
;1298:	other->client->ps.stats[STAT_HEALTH] = other->health;
ADDRLP4 16
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 216
ADDP4
ADDRLP4 16
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
ASGNI4
line 1300
;1299:
;1300:	if ( ent->item->quantity == 100 ) {		// mega health respawns slow
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
CNSTI4 100
NEI4 $495
line 1301
;1301:		return RESPAWN_MEGAHEALTH;
CNSTI4 120
RETI4
ADDRGP4 $488
JUMPV
LABELV $495
line 1304
;1302:	}
;1303:
;1304:	return adjustRespawnTime(RESPAWN_HEALTH, ent->item->giType, ent->item->giTag);
CNSTF4 1106247680
ARGF4
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ARGI4
ADDRLP4 20
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ARGI4
ADDRLP4 24
ADDRGP4 adjustRespawnTime
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
RETI4
LABELV $488
endproc Pickup_Health 28 12
export Pickup_Armor
proc Pickup_Armor 16 12
line 1310
;1305:}
;1306:
;1307://======================================================================
;1308:
;1309:int Pickup_Armor( gentity_t *ent, gentity_t *other ) 
;1310:{
line 1311
;1311:	other->client->ps.stats[STAT_ARMOR] += ent->item->quantity;
ADDRLP4 0
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 236
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ADDI4
ASGNI4
line 1312
;1312:	if ( other->client->ps.stats[STAT_ARMOR] > other->client->ps.stats[STAT_MAX_HEALTH] * ent->item->giTag ) 
ADDRLP4 4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 236
ADDP4
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
MULI4
LEI4 $498
line 1313
;1313:	{
line 1314
;1314:		other->client->ps.stats[STAT_ARMOR] = other->client->ps.stats[STAT_MAX_HEALTH] * ent->item->giTag;
ADDRLP4 8
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 236
ADDP4
ADDRLP4 8
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
MULI4
ASGNI4
line 1315
;1315:	}
LABELV $498
line 1317
;1316:
;1317:	return adjustRespawnTime(RESPAWN_ARMOR, ent->item->giType, ent->item->giTag);
CNSTF4 1101004800
ARGF4
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 adjustRespawnTime
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
RETI4
LABELV $497
endproc Pickup_Armor 16 12
export RespawnItem
proc RespawnItem 20 12
line 1327
;1318:}
;1319:
;1320://======================================================================
;1321:
;1322:/*
;1323:===============
;1324:RespawnItem
;1325:===============
;1326:*/
;1327:void RespawnItem( gentity_t *ent ) {
line 1329
;1328:	// randomly select from teamed entities
;1329:	if (ent->team) {
ADDRFP4 0
INDIRP4
CNSTI4 592
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $501
line 1334
;1330:		gentity_t	*master;
;1331:		int	count;
;1332:		int choice;
;1333:
;1334:		if ( !ent->teammaster ) {
ADDRFP4 0
INDIRP4
CNSTI4 740
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $503
line 1335
;1335:			G_Error( "RespawnItem: bad teammaster");
ADDRGP4 $505
ARGP4
ADDRGP4 G_Error
CALLV
pop
line 1336
;1336:		}
LABELV $503
line 1337
;1337:		master = ent->teammaster;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 740
ADDP4
INDIRP4
ASGNP4
line 1339
;1338:
;1339:		for (count = 0, ent = master; ent; ent = ent->teamchain, count++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRFP4 0
ADDRLP4 8
INDIRP4
ASGNP4
ADDRGP4 $509
JUMPV
LABELV $506
line 1340
;1340:			;
LABELV $507
line 1339
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 736
ADDP4
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $509
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $506
line 1342
;1341:
;1342:		choice = rand() % count;
ADDRLP4 12
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 12
INDIRI4
ADDRLP4 0
INDIRI4
MODI4
ASGNI4
line 1344
;1343:
;1344:		for (count = 0, ent = master; count < choice; ent = ent->teamchain, count++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRFP4 0
ADDRLP4 8
INDIRP4
ASGNP4
ADDRGP4 $513
JUMPV
LABELV $510
line 1345
;1345:			;
LABELV $511
line 1344
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 736
ADDP4
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $513
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $510
line 1346
;1346:	}
LABELV $501
line 1348
;1347:
;1348:	ent->r.contents = CONTENTS_TRIGGER;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1024
ASGNI4
line 1350
;1349:	//ent->s.eFlags &= ~EF_NODRAW;
;1350:	ent->s.eFlags &= ~(EF_NODRAW | EF_ITEMPLACEHOLDER);
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 -4194433
BANDI4
ASGNI4
line 1351
;1351:	ent->r.svFlags &= ~SVF_NOCLIENT;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 -2
BANDI4
ASGNI4
line 1352
;1352:	trap_LinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1354
;1353:
;1354:	if ( ent->item->giType == IT_POWERUP ) {
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
CNSTI4 5
NEI4 $514
line 1359
;1355:		// play powerup spawn sound to all clients
;1356:		gentity_t	*te;
;1357:
;1358:		// if the powerup respawn sound should Not be global
;1359:		if (ent->speed) {
ADDRFP4 0
INDIRP4
CNSTI4 608
ADDP4
INDIRF4
CNSTF4 0
EQF4 $516
line 1360
;1360:			te = G_TempEntity( ent->s.pos.trBase, EV_GENERAL_SOUND );
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
ARGP4
CNSTI4 66
ARGI4
ADDRLP4 12
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 12
INDIRP4
ASGNP4
line 1361
;1361:		}
ADDRGP4 $517
JUMPV
LABELV $516
line 1362
;1362:		else {
line 1363
;1363:			te = G_TempEntity( ent->s.pos.trBase, EV_GLOBAL_SOUND );
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
ARGP4
CNSTI4 67
ARGI4
ADDRLP4 12
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 12
INDIRP4
ASGNP4
line 1364
;1364:		}
LABELV $517
line 1365
;1365:		te->s.eventParm = G_SoundIndex( "sound/items/respawn1" );
ADDRGP4 $518
ARGP4
ADDRLP4 12
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRLP4 8
INDIRP4
CNSTI4 256
ADDP4
ADDRLP4 12
INDIRI4
ASGNI4
line 1366
;1366:		te->r.svFlags |= SVF_BROADCAST;
ADDRLP4 16
ADDRLP4 8
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 1367
;1367:	}
LABELV $514
line 1370
;1368:
;1369:	// play the normal respawn sound only to nearby clients
;1370:	G_AddEvent( ent, EV_ITEM_RESPAWN, 0 );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 57
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 1372
;1371:
;1372:	ent->nextthink = 0;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
CNSTI4 0
ASGNI4
line 1373
;1373:}
LABELV $500
endproc RespawnItem 20 12
export Touch_Item
proc Touch_Item 72 12
line 1381
;1374:
;1375:
;1376:/*
;1377:===============
;1378:Touch_Item
;1379:===============
;1380:*/
;1381:void Touch_Item (gentity_t *ent, gentity_t *other, trace_t *trace) {
line 1385
;1382:	int			respawn;
;1383:	qboolean	predict;
;1384:
;1385:	if (ent->s.eFlags & EF_ITEMPLACEHOLDER)
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 4194304
BANDI4
CNSTI4 0
EQI4 $520
line 1386
;1386:	{
line 1387
;1387:		return;
ADDRGP4 $519
JUMPV
LABELV $520
line 1390
;1388:	}
;1389:
;1390:	if (ent->s.eFlags & EF_NODRAW)
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 128
BANDI4
CNSTI4 0
EQI4 $522
line 1391
;1391:	{
line 1392
;1392:		return;
ADDRGP4 $519
JUMPV
LABELV $522
line 1395
;1393:	}
;1394:
;1395:	if (ent->item->giType == IT_WEAPON &&
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
CNSTI4 1
NEI4 $524
ADDRLP4 12
ADDRLP4 8
INDIRP4
CNSTI4 272
ADDP4
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $524
ADDRLP4 12
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $524
line 1398
;1396:		ent->s.powerups &&
;1397:		ent->s.powerups < level.time)
;1398:	{
line 1399
;1399:		ent->s.generic1 = 0;
ADDRFP4 0
INDIRP4
CNSTI4 292
ADDP4
CNSTI4 0
ASGNI4
line 1400
;1400:		ent->s.powerups = 0;
ADDRFP4 0
INDIRP4
CNSTI4 272
ADDP4
CNSTI4 0
ASGNI4
line 1401
;1401:	}
LABELV $524
line 1403
;1402:
;1403:	if (!other->client)
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $527
line 1404
;1404:		return;
ADDRGP4 $519
JUMPV
LABELV $527
line 1405
;1405:	if (other->health < 1)
ADDRFP4 4
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 1
GEI4 $529
line 1406
;1406:		return;		// dead people can't pickup
ADDRGP4 $519
JUMPV
LABELV $529
line 1408
;1407:
;1408:	if (ent->item->giType == IT_POWERUP &&
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
CNSTI4 5
NEI4 $531
ADDRLP4 20
ADDRLP4 16
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 12
EQI4 $533
ADDRLP4 20
INDIRI4
CNSTI4 13
NEI4 $531
LABELV $533
line 1410
;1409:		(ent->item->giTag == PW_FORCE_ENLIGHTENED_LIGHT || ent->item->giTag == PW_FORCE_ENLIGHTENED_DARK))
;1410:	{
line 1411
;1411:		if (ent->item->giTag == PW_FORCE_ENLIGHTENED_LIGHT)
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 12
NEI4 $534
line 1412
;1412:		{
line 1413
;1413:			if (other->client->ps.fd.forceSide != FORCE_LIGHTSIDE)
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1176
ADDP4
INDIRI4
CNSTI4 1
EQI4 $535
line 1414
;1414:			{
line 1415
;1415:				return;
ADDRGP4 $519
JUMPV
line 1417
;1416:			}
;1417:		}
LABELV $534
line 1419
;1418:		else
;1419:		{
line 1420
;1420:			if (other->client->ps.fd.forceSide != FORCE_DARKSIDE)
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1176
ADDP4
INDIRI4
CNSTI4 2
EQI4 $538
line 1421
;1421:			{
line 1422
;1422:				return;
ADDRGP4 $519
JUMPV
LABELV $538
line 1424
;1423:			}
;1424:		}
LABELV $535
line 1425
;1425:	}
LABELV $531
line 1428
;1426:
;1427:	// the same pickup rules are used for client side and server side
;1428:	if ( !BG_CanItemBeGrabbed( g_gametype.integer, &ent->s, &other->client->ps ) ) {
ADDRGP4 g_gametype+12
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 BG_CanItemBeGrabbed
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $540
line 1429
;1429:		return;
ADDRGP4 $519
JUMPV
LABELV $540
line 1432
;1430:	}
;1431:
;1432:	G_LogPrintf( "Item: %i %s\n", other->s.number, ent->item->classname );
ADDRGP4 $543
ARGP4
ADDRFP4 4
INDIRP4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
INDIRP4
ARGP4
ADDRGP4 G_LogPrintf
CALLV
pop
line 1434
;1433:
;1434:	predict = other->client->pers.predictItemPickup;
ADDRLP4 4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1408
ADDP4
INDIRI4
ASGNI4
line 1437
;1435:
;1436:	// call the item-specific pickup function
;1437:	switch( ent->item->giType ) {
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 1
LTI4 $519
ADDRLP4 28
INDIRI4
CNSTI4 8
GTI4 $519
ADDRLP4 28
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $564-4
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $564
address $547
address $548
address $559
address $560
address $561
address $563
address $519
address $562
code
LABELV $547
line 1439
;1438:	case IT_WEAPON:
;1439:		respawn = Pickup_Weapon(ent, other);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 Pickup_Weapon
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 36
INDIRI4
ASGNI4
line 1441
;1440://		predict = qfalse;
;1441:		predict = qtrue;
ADDRLP4 4
CNSTI4 1
ASGNI4
line 1442
;1442:		break;
ADDRGP4 $545
JUMPV
LABELV $548
line 1444
;1443:	case IT_AMMO:
;1444:		respawn = Pickup_Ammo(ent, other);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 40
ADDRGP4 Pickup_Ammo
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 40
INDIRI4
ASGNI4
line 1445
;1445:		if (ent->item->giTag == AMMO_THERMAL || ent->item->giTag == AMMO_TRIPMINE || ent->item->giTag == AMMO_DETPACK)
ADDRLP4 44
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 7
EQI4 $552
ADDRLP4 44
INDIRI4
CNSTI4 8
EQI4 $552
ADDRLP4 44
INDIRI4
CNSTI4 9
NEI4 $549
LABELV $552
line 1446
;1446:		{
line 1447
;1447:			int weapForAmmo = 0;
ADDRLP4 48
CNSTI4 0
ASGNI4
line 1449
;1448:
;1449:			if (ent->item->giTag == AMMO_THERMAL)
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 7
NEI4 $553
line 1450
;1450:			{
line 1451
;1451:				weapForAmmo = WP_THERMAL;
ADDRLP4 48
CNSTI4 11
ASGNI4
line 1452
;1452:			}
ADDRGP4 $554
JUMPV
LABELV $553
line 1453
;1453:			else if (ent->item->giTag == AMMO_TRIPMINE)
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
CNSTI4 8
NEI4 $555
line 1454
;1454:			{
line 1455
;1455:				weapForAmmo = WP_TRIP_MINE;
ADDRLP4 48
CNSTI4 12
ASGNI4
line 1456
;1456:			}
ADDRGP4 $556
JUMPV
LABELV $555
line 1458
;1457:			else
;1458:			{
line 1459
;1459:				weapForAmmo = WP_DET_PACK;
ADDRLP4 48
CNSTI4 13
ASGNI4
line 1460
;1460:			}
LABELV $556
LABELV $554
line 1462
;1461:
;1462:			if (other && other->client && other->client->ps.ammo[weaponData[weapForAmmo].ammoIndex] > 0 )
ADDRLP4 52
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 56
CNSTU4 0
ASGNU4
ADDRLP4 52
INDIRP4
CVPU4 4
ADDRLP4 56
INDIRU4
EQU4 $557
ADDRLP4 60
CNSTI4 408
ASGNI4
ADDRLP4 64
ADDRLP4 52
INDIRP4
ADDRLP4 60
INDIRI4
ADDP4
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
CVPU4 4
ADDRLP4 56
INDIRU4
EQU4 $557
CNSTI4 56
ADDRLP4 48
INDIRI4
MULI4
ADDRGP4 weaponData
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 64
INDIRP4
ADDRLP4 60
INDIRI4
ADDP4
ADDP4
INDIRI4
CNSTI4 0
LEI4 $557
line 1463
;1463:			{
line 1464
;1464:				other->client->ps.stats[STAT_WEAPONS] |= (1 << weapForAmmo);
ADDRLP4 68
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 232
ADDP4
ASGNP4
ADDRLP4 68
INDIRP4
ADDRLP4 68
INDIRP4
INDIRI4
CNSTI4 1
ADDRLP4 48
INDIRI4
LSHI4
BORI4
ASGNI4
line 1465
;1465:			}
LABELV $557
line 1466
;1466:		}
LABELV $549
line 1468
;1467://		predict = qfalse;
;1468:		predict = qtrue;
ADDRLP4 4
CNSTI4 1
ASGNI4
line 1469
;1469:		break;
ADDRGP4 $545
JUMPV
LABELV $559
line 1471
;1470:	case IT_ARMOR:
;1471:		respawn = Pickup_Armor(ent, other);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 48
ADDRGP4 Pickup_Armor
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 48
INDIRI4
ASGNI4
line 1473
;1472://		predict = qfalse;
;1473:		predict = qtrue;
ADDRLP4 4
CNSTI4 1
ASGNI4
line 1474
;1474:		break;
ADDRGP4 $545
JUMPV
LABELV $560
line 1476
;1475:	case IT_HEALTH:
;1476:		respawn = Pickup_Health(ent, other);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 52
ADDRGP4 Pickup_Health
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 52
INDIRI4
ASGNI4
line 1478
;1477://		predict = qfalse;
;1478:		predict = qtrue;
ADDRLP4 4
CNSTI4 1
ASGNI4
line 1479
;1479:		break;
ADDRGP4 $545
JUMPV
LABELV $561
line 1481
;1480:	case IT_POWERUP:
;1481:		respawn = Pickup_Powerup(ent, other);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 56
ADDRGP4 Pickup_Powerup
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 56
INDIRI4
ASGNI4
line 1482
;1482:		predict = qfalse;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 1484
;1483://		predict = qtrue;
;1484:		break;
ADDRGP4 $545
JUMPV
LABELV $562
line 1486
;1485:	case IT_TEAM:
;1486:		respawn = Pickup_Team(ent, other);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 60
ADDRGP4 Pickup_Team
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 60
INDIRI4
ASGNI4
line 1487
;1487:		break;
ADDRGP4 $545
JUMPV
LABELV $563
line 1489
;1488:	case IT_HOLDABLE:
;1489:		respawn = Pickup_Holdable(ent, other);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 64
ADDRGP4 Pickup_Holdable
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 64
INDIRI4
ASGNI4
line 1490
;1490:		break;
line 1492
;1491:	default:
;1492:		return;
LABELV $545
line 1495
;1493:	}
;1494:
;1495:	if ( !respawn ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $566
line 1496
;1496:		return;
ADDRGP4 $519
JUMPV
LABELV $566
line 1500
;1497:	}
;1498:
;1499:	// play the normal pickup sound
;1500:	if (predict) {
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $568
line 1501
;1501:		if (other->client)
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $570
line 1502
;1502:		{
line 1503
;1503:			BG_AddPredictableEventToPlayerstate( EV_ITEM_PICKUP, ent->s.number, &other->client->ps);
CNSTI4 20
ARGI4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ARGP4
ADDRGP4 BG_AddPredictableEventToPlayerstate
CALLV
pop
line 1504
;1504:		}
ADDRGP4 $569
JUMPV
LABELV $570
line 1506
;1505:		else
;1506:		{
line 1507
;1507:			G_AddPredictableEvent( other, EV_ITEM_PICKUP, ent->s.number );
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 20
ARGI4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
ADDRGP4 G_AddPredictableEvent
CALLV
pop
line 1508
;1508:		}
line 1509
;1509:	} else {
ADDRGP4 $569
JUMPV
LABELV $568
line 1510
;1510:		G_AddEvent( other, EV_ITEM_PICKUP, ent->s.number );
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 20
ARGI4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 1511
;1511:	}
LABELV $569
line 1514
;1512:
;1513:	// powerup pickups are global broadcasts
;1514:	if ( /*ent->item->giType == IT_POWERUP ||*/ ent->item->giType == IT_TEAM) {
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
CNSTI4 8
NEI4 $572
line 1516
;1515:		// if we want the global sound to play
;1516:		if (!ent->speed) {
ADDRFP4 0
INDIRP4
CNSTI4 608
ADDP4
INDIRF4
CNSTF4 0
NEF4 $574
line 1519
;1517:			gentity_t	*te;
;1518:
;1519:			te = G_TempEntity( ent->s.pos.trBase, EV_GLOBAL_ITEM_PICKUP );
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
ARGP4
CNSTI4 21
ARGI4
ADDRLP4 40
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 36
ADDRLP4 40
INDIRP4
ASGNP4
line 1520
;1520:			te->s.eventParm = ent->s.modelindex;
ADDRLP4 36
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 212
ADDP4
INDIRI4
ASGNI4
line 1521
;1521:			te->r.svFlags |= SVF_BROADCAST;
ADDRLP4 44
ADDRLP4 36
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 1522
;1522:		} else {
ADDRGP4 $575
JUMPV
LABELV $574
line 1525
;1523:			gentity_t	*te;
;1524:
;1525:			te = G_TempEntity( ent->s.pos.trBase, EV_GLOBAL_ITEM_PICKUP );
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
ARGP4
CNSTI4 21
ARGI4
ADDRLP4 40
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 36
ADDRLP4 40
INDIRP4
ASGNP4
line 1526
;1526:			te->s.eventParm = ent->s.modelindex;
ADDRLP4 36
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 212
ADDP4
INDIRI4
ASGNI4
line 1528
;1527:			// only send this temp entity to a single client
;1528:			te->r.svFlags |= SVF_SINGLECLIENT;
ADDRLP4 44
ADDRLP4 36
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRI4
CNSTI4 256
BORI4
ASGNI4
line 1529
;1529:			te->r.singleClient = other->s.number;
ADDRLP4 36
INDIRP4
CNSTI4 308
ADDP4
ADDRFP4 4
INDIRP4
INDIRI4
ASGNI4
line 1530
;1530:		}
LABELV $575
line 1531
;1531:	}
LABELV $572
line 1534
;1532:
;1533:	// fire item targets
;1534:	G_UseTargets (ent, other);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
line 1537
;1535:
;1536:	// wait of -1 will not respawn
;1537:	if ( ent->wait == -1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 756
ADDP4
INDIRF4
CNSTF4 3212836864
NEF4 $576
line 1538
;1538:		ent->r.svFlags |= SVF_NOCLIENT;
ADDRLP4 36
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 1539
;1539:		ent->s.eFlags |= EF_NODRAW;
ADDRLP4 40
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRI4
CNSTI4 128
BORI4
ASGNI4
line 1540
;1540:		ent->r.contents = 0;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 0
ASGNI4
line 1541
;1541:		ent->unlinkAfterEvent = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 496
ADDP4
CNSTI4 1
ASGNI4
line 1542
;1542:		return;
ADDRGP4 $519
JUMPV
LABELV $576
line 1546
;1543:	}
;1544:
;1545:	// non zero wait overrides respawn time
;1546:	if ( ent->wait ) {
ADDRFP4 0
INDIRP4
CNSTI4 756
ADDP4
INDIRF4
CNSTF4 0
EQF4 $578
line 1547
;1547:		respawn = ent->wait;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 756
ADDP4
INDIRF4
CVFI4 4
ASGNI4
line 1548
;1548:	}
LABELV $578
line 1551
;1549:
;1550:	// random can be used to vary the respawn time
;1551:	if ( ent->random ) {
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
INDIRF4
CNSTF4 0
EQF4 $580
line 1552
;1552:		respawn += crandom() * ent->random;
ADDRLP4 36
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRI4
CVIF4 4
CNSTF4 1073741824
ADDRLP4 36
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
INDIRF4
MULF4
ADDF4
CVFI4 4
ASGNI4
line 1553
;1553:		if ( respawn < 1 ) {
ADDRLP4 0
INDIRI4
CNSTI4 1
GEI4 $582
line 1554
;1554:			respawn = 1;
ADDRLP4 0
CNSTI4 1
ASGNI4
line 1555
;1555:		}
LABELV $582
line 1556
;1556:	}
LABELV $580
line 1559
;1557:
;1558:	// dropped items will not respawn
;1559:	if ( ent->flags & FL_DROPPED_ITEM ) {
ADDRFP4 0
INDIRP4
CNSTI4 472
ADDP4
INDIRI4
CNSTI4 4096
BANDI4
CNSTI4 0
EQI4 $584
line 1560
;1560:		ent->freeAfterEvent = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 492
ADDP4
CNSTI4 1
ASGNI4
line 1561
;1561:	}
LABELV $584
line 1566
;1562:
;1563:	// picked up items still stay around, they just don't
;1564:	// draw anything.  This allows respawnable items
;1565:	// to be placed on movers.
;1566:	if (!(ent->flags & FL_DROPPED_ITEM) && (ent->item->giType==IT_WEAPON || ent->item->giType==IT_POWERUP))
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
CNSTI4 472
ADDP4
INDIRI4
CNSTI4 4096
BANDI4
CNSTI4 0
NEI4 $586
ADDRLP4 40
ADDRLP4 36
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 1
EQI4 $588
ADDRLP4 40
INDIRI4
CNSTI4 5
NEI4 $586
LABELV $588
line 1567
;1567:	{
line 1568
;1568:		ent->s.eFlags |= EF_ITEMPLACEHOLDER;
ADDRLP4 44
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRI4
CNSTI4 4194304
BORI4
ASGNI4
line 1569
;1569:		ent->s.eFlags &= ~EF_NODRAW;
ADDRLP4 48
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRI4
CNSTI4 -129
BANDI4
ASGNI4
line 1570
;1570:	}
ADDRGP4 $587
JUMPV
LABELV $586
line 1572
;1571:	else
;1572:	{
line 1573
;1573:		ent->s.eFlags |= EF_NODRAW;
ADDRLP4 44
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRI4
CNSTI4 128
BORI4
ASGNI4
line 1574
;1574:		ent->r.svFlags |= SVF_NOCLIENT;
ADDRLP4 48
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 1575
;1575:	}
LABELV $587
line 1576
;1576:	ent->r.contents = 0;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 0
ASGNI4
line 1582
;1577:
;1578:	// ZOID
;1579:	// A negative respawn times means to never respawn this item (but don't 
;1580:	// delete it).  This is used by items that are respawned by third party 
;1581:	// events such as ctf flags
;1582:	if ( respawn <= 0 ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
GTI4 $589
line 1583
;1583:		ent->nextthink = 0;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
CNSTI4 0
ASGNI4
line 1584
;1584:		ent->think = 0;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
CNSTP4 0
ASGNP4
line 1585
;1585:	} else {
ADDRGP4 $590
JUMPV
LABELV $589
line 1586
;1586:		ent->nextthink = level.time + respawn * 1000;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1000
ADDRLP4 0
INDIRI4
MULI4
ADDI4
ASGNI4
line 1587
;1587:		ent->think = RespawnItem;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 RespawnItem
ASGNP4
line 1588
;1588:	}
LABELV $590
line 1589
;1589:	trap_LinkEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1590
;1590:}
LABELV $519
endproc Touch_Item 72 12
export LaunchItem
proc LaunchItem 24 8
line 1602
;1591:
;1592:
;1593://======================================================================
;1594:
;1595:/*
;1596:================
;1597:LaunchItem
;1598:
;1599:Spawns an item and tosses it forward
;1600:================
;1601:*/
;1602:gentity_t *LaunchItem( gitem_t *item, vec3_t origin, vec3_t velocity ) {
line 1605
;1603:	gentity_t	*dropped;
;1604:
;1605:	dropped = G_Spawn();
ADDRLP4 4
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 1607
;1606:
;1607:	dropped->s.eType = ET_ITEM;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 2
ASGNI4
line 1608
;1608:	dropped->s.modelindex = item - bg_itemlist;	// store item number in modelindex
ADDRLP4 0
INDIRP4
CNSTI4 212
ADDP4
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRGP4 bg_itemlist
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 52
DIVI4
ASGNI4
line 1609
;1609:	if (dropped->s.modelindex < 0)
ADDRLP4 0
INDIRP4
CNSTI4 212
ADDP4
INDIRI4
CNSTI4 0
GEI4 $593
line 1610
;1610:	{
line 1611
;1611:		dropped->s.modelindex = 0;
ADDRLP4 0
INDIRP4
CNSTI4 212
ADDP4
CNSTI4 0
ASGNI4
line 1612
;1612:	}
LABELV $593
line 1613
;1613:	dropped->s.modelindex2 = 1; // This is non-zero is it's a dropped item
ADDRLP4 0
INDIRP4
CNSTI4 216
ADDP4
CNSTI4 1
ASGNI4
line 1615
;1614:
;1615:	dropped->classname = item->classname;
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRFP4 0
INDIRP4
INDIRP4
ASGNP4
line 1616
;1616:	dropped->item = item;
ADDRLP4 0
INDIRP4
CNSTI4 824
ADDP4
ADDRFP4 0
INDIRP4
ASGNP4
line 1617
;1617:	VectorSet (dropped->r.mins, -ITEM_RADIUS, -ITEM_RADIUS, -ITEM_RADIUS);
ADDRLP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3245342720
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3245342720
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3245342720
ASGNF4
line 1618
;1618:	VectorSet (dropped->r.maxs, ITEM_RADIUS, ITEM_RADIUS, ITEM_RADIUS);
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1097859072
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1097859072
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1097859072
ASGNF4
line 1620
;1619:
;1620:	dropped->r.contents = CONTENTS_TRIGGER;
ADDRLP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1024
ASGNI4
line 1622
;1621:
;1622:	dropped->touch = Touch_Item;
ADDRLP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 Touch_Item
ASGNP4
line 1624
;1623:
;1624:	G_SetOrigin( dropped, origin );
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1625
;1625:	dropped->s.pos.trType = TR_GRAVITY;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 5
ASGNI4
line 1626
;1626:	dropped->s.pos.trTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1627
;1627:	VectorCopy( velocity, dropped->s.pos.trDelta );
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRFP4 8
INDIRP4
INDIRB
ASGNB 12
line 1629
;1628:
;1629:	dropped->s.eFlags |= EF_BOUNCE_HALF;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 1630
;1630:	if ((g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY) && item->giType == IT_TEAM) { // Special case for CTF flags
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 7
EQI4 $600
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 8
NEI4 $596
LABELV $600
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
CNSTI4 8
NEI4 $596
line 1631
;1631:		dropped->think = Team_DroppedFlagThink;
ADDRLP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 Team_DroppedFlagThink
ASGNP4
line 1632
;1632:		dropped->nextthink = level.time + 30000;
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 30000
ADDI4
ASGNI4
line 1633
;1633:		Team_CheckDroppedItem( dropped );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Team_CheckDroppedItem
CALLV
pop
line 1636
;1634:
;1635:		//rww - so bots know
;1636:		if (strcmp(dropped->classname, "team_CTF_redflag") == 0)
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
INDIRP4
ARGP4
ADDRGP4 $604
ARGP4
ADDRLP4 12
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $602
line 1637
;1637:		{
line 1638
;1638:			droppedRedFlag = dropped;
ADDRGP4 droppedRedFlag
ADDRLP4 0
INDIRP4
ASGNP4
line 1639
;1639:		}
ADDRGP4 $597
JUMPV
LABELV $602
line 1640
;1640:		else if (strcmp(dropped->classname, "team_CTF_blueflag") == 0)
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
INDIRP4
ARGP4
ADDRGP4 $607
ARGP4
ADDRLP4 16
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $597
line 1641
;1641:		{
line 1642
;1642:			droppedBlueFlag = dropped;
ADDRGP4 droppedBlueFlag
ADDRLP4 0
INDIRP4
ASGNP4
line 1643
;1643:		}
line 1644
;1644:	} else { // auto-remove after 30 seconds
ADDRGP4 $597
JUMPV
LABELV $596
line 1645
;1645:		dropped->think = G_FreeEntity;
ADDRLP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 G_FreeEntity
ASGNP4
line 1646
;1646:		dropped->nextthink = level.time + 30000;
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 30000
ADDI4
ASGNI4
line 1647
;1647:	}
LABELV $597
line 1649
;1648:
;1649:	dropped->flags = FL_DROPPED_ITEM;
ADDRLP4 0
INDIRP4
CNSTI4 472
ADDP4
CNSTI4 4096
ASGNI4
line 1651
;1650:
;1651:	if (item->giType == IT_WEAPON || item->giType == IT_POWERUP)
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 1
EQI4 $611
ADDRLP4 12
INDIRI4
CNSTI4 5
NEI4 $609
LABELV $611
line 1652
;1652:	{
line 1653
;1653:		dropped->s.eFlags |= EF_DROPPEDWEAPON;
ADDRLP4 16
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 16777216
BORI4
ASGNI4
line 1654
;1654:	}
LABELV $609
line 1656
;1655:
;1656:	vectoangles(velocity, dropped->s.angles);
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1657
;1657:	dropped->s.angles[PITCH] = 0;
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
CNSTF4 0
ASGNF4
line 1659
;1658:
;1659:	if (item->giTag == WP_TRIP_MINE ||
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 12
EQI4 $614
ADDRLP4 16
INDIRI4
CNSTI4 13
NEI4 $612
LABELV $614
line 1661
;1660:		item->giTag == WP_DET_PACK)
;1661:	{
line 1662
;1662:		dropped->s.angles[PITCH] = -90;
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
CNSTF4 3266576384
ASGNF4
line 1663
;1663:	}
LABELV $612
line 1665
;1664:
;1665:	if (item->giTag != WP_BOWCASTER &&
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 6
EQI4 $615
ADDRLP4 20
INDIRI4
CNSTI4 13
EQI4 $615
ADDRLP4 20
INDIRI4
CNSTI4 11
EQI4 $615
line 1668
;1666:		item->giTag != WP_DET_PACK &&
;1667:		item->giTag != WP_THERMAL)
;1668:	{
line 1669
;1669:		dropped->s.angles[ROLL] = -90;
ADDRLP4 0
INDIRP4
CNSTI4 124
ADDP4
CNSTF4 3266576384
ASGNF4
line 1670
;1670:	}
LABELV $615
line 1672
;1671:
;1672:	dropped->physicsObject = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 500
ADDP4
CNSTI4 1
ASGNI4
line 1674
;1673:
;1674:	trap_LinkEntity (dropped);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1676
;1675:
;1676:	return dropped;
ADDRLP4 0
INDIRP4
RETP4
LABELV $592
endproc LaunchItem 24 8
export Drop_Item
proc Drop_Item 40 16
line 1686
;1677:}
;1678:
;1679:/*
;1680:================
;1681:Drop_Item
;1682:
;1683:Spawns an item and tosses it forward
;1684:================
;1685:*/
;1686:gentity_t *Drop_Item( gentity_t *ent, gitem_t *item, float angle ) {
line 1690
;1687:	vec3_t	velocity;
;1688:	vec3_t	angles;
;1689:
;1690:	VectorCopy( ent->s.apos.trBase, angles );
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
INDIRB
ASGNB 12
line 1691
;1691:	angles[YAW] += angle;
ADDRLP4 12+4
ADDRLP4 12+4
INDIRF4
ADDRFP4 8
INDIRF4
ADDF4
ASGNF4
line 1692
;1692:	angles[PITCH] = 0;	// always forward
ADDRLP4 12
CNSTF4 0
ASGNF4
line 1694
;1693:
;1694:	AngleVectors( angles, velocity, NULL, NULL );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 24
CNSTP4 0
ASGNP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRLP4 24
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 1695
;1695:	VectorScale( velocity, 150, velocity );
ADDRLP4 28
CNSTF4 1125515264
ASGNF4
ADDRLP4 0
ADDRLP4 28
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 28
INDIRF4
ADDRLP4 0+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0+8
CNSTF4 1125515264
ADDRLP4 0+8
INDIRF4
MULF4
ASGNF4
line 1696
;1696:	velocity[2] += 200 + crandom() * 50;
ADDRLP4 32
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1112014848
CNSTF4 1073741824
ADDRLP4 32
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
CNSTF4 1056964608
SUBF4
MULF4
MULF4
CNSTF4 1128792064
ADDF4
ADDF4
ASGNF4
line 1698
;1697:	
;1698:	return LaunchItem( item, ent->s.pos.trBase, velocity );
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 36
ADDRGP4 LaunchItem
CALLP4
ASGNP4
ADDRLP4 36
INDIRP4
RETP4
LABELV $617
endproc Drop_Item 40 16
export Use_Item
proc Use_Item 0 4
line 1709
;1699:}
;1700:
;1701:
;1702:/*
;1703:================
;1704:Use_Item
;1705:
;1706:Respawn the item
;1707:================
;1708:*/
;1709:void Use_Item( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
line 1710
;1710:	RespawnItem( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 RespawnItem
CALLV
pop
line 1711
;1711:}
LABELV $624
endproc Use_Item 0 4
export FinishSpawningItem
proc FinishSpawningItem 1140 28
line 1723
;1712:
;1713://======================================================================
;1714:
;1715:/*
;1716:================
;1717:FinishSpawningItem
;1718:
;1719:Traces down to find where an item should rest, instead of letting them
;1720:free fall from their spawn points
;1721:================
;1722:*/
;1723:void FinishSpawningItem( gentity_t *ent ) {
line 1726
;1724:	trace_t		tr;
;1725:	vec3_t		dest;
;1726:	int			wDisable = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1732
;1727://	gitem_t		*item;
;1728:
;1729://	VectorSet( ent->r.mins, -ITEM_RADIUS, -ITEM_RADIUS, -ITEM_RADIUS );
;1730://	VectorSet( ent->r.maxs, ITEM_RADIUS, ITEM_RADIUS, ITEM_RADIUS );
;1731:
;1732:	if (g_gametype.integer == GT_TOURNAMENT)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
NEI4 $626
line 1733
;1733:	{
line 1734
;1734:		wDisable = g_duelWeaponDisable.integer;
ADDRLP4 0
ADDRGP4 g_duelWeaponDisable+12
INDIRI4
ASGNI4
line 1735
;1735:	}
ADDRGP4 $627
JUMPV
LABELV $626
line 1737
;1736:	else
;1737:	{
line 1738
;1738:		wDisable = g_weaponDisable.integer;
ADDRLP4 0
ADDRGP4 g_weaponDisable+12
INDIRI4
ASGNI4
line 1739
;1739:	}
LABELV $627
line 1741
;1740:
;1741:	if (ent->item->giType == IT_WEAPON &&
ADDRLP4 1096
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1100
CNSTI4 1
ASGNI4
ADDRLP4 1096
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ADDRLP4 1100
INDIRI4
NEI4 $631
ADDRLP4 1108
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 1108
INDIRI4
EQI4 $631
ADDRLP4 0
INDIRI4
ADDRLP4 1100
INDIRI4
ADDRLP4 1096
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
LSHI4
BANDI4
ADDRLP4 1108
INDIRI4
EQI4 $631
line 1744
;1742:		wDisable &&
;1743:		(wDisable & (1 << ent->item->giTag)))
;1744:	{
line 1745
;1745:		if (g_gametype.integer != GT_JEDIMASTER)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 2
EQI4 $633
line 1746
;1746:		{
line 1747
;1747:			G_FreeEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 1748
;1748:			return;
ADDRGP4 $625
JUMPV
LABELV $633
line 1750
;1749:		}
;1750:	}
LABELV $631
line 1752
;1751:
;1752:	if (g_gametype.integer != GT_JEDIMASTER)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 2
EQI4 $636
line 1753
;1753:	{
line 1754
;1754:		if (HasSetSaberOnly())
ADDRLP4 1112
ADDRGP4 HasSetSaberOnly
CALLI4
ASGNI4
ADDRLP4 1112
INDIRI4
CNSTI4 0
EQI4 $637
line 1755
;1755:		{
line 1756
;1756:			if (ent->item->giType == IT_AMMO)
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
CNSTI4 2
NEI4 $641
line 1757
;1757:			{
line 1758
;1758:				G_FreeEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 1759
;1759:				return;
ADDRGP4 $625
JUMPV
LABELV $641
line 1762
;1760:			}
;1761:
;1762:			if (ent->item->giType == IT_HOLDABLE)
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
CNSTI4 6
NEI4 $637
line 1763
;1763:			{
line 1764
;1764:				if (ent->item->giTag == HI_SEEKER ||
ADDRLP4 1116
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1116
INDIRI4
CNSTI4 1
EQI4 $648
ADDRLP4 1116
INDIRI4
CNSTI4 2
EQI4 $648
ADDRLP4 1116
INDIRI4
CNSTI4 6
NEI4 $637
LABELV $648
line 1767
;1765:					ent->item->giTag == HI_SHIELD ||
;1766:					ent->item->giTag == HI_SENTRY_GUN)
;1767:				{
line 1768
;1768:					G_FreeEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 1769
;1769:					return;
ADDRGP4 $625
JUMPV
line 1771
;1770:				}
;1771:			}
line 1772
;1772:		}
line 1773
;1773:	}
LABELV $636
line 1775
;1774:	else
;1775:	{ //no powerups in jedi master
line 1776
;1776:		if (ent->item->giType == IT_POWERUP)
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
CNSTI4 5
NEI4 $649
line 1777
;1777:		{
line 1778
;1778:			G_FreeEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 1779
;1779:			return;
ADDRGP4 $625
JUMPV
LABELV $649
line 1781
;1780:		}
;1781:	}
LABELV $637
line 1783
;1782:
;1783:	if (g_gametype.integer == GT_HOLOCRON)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 1
NEI4 $651
line 1784
;1784:	{
line 1785
;1785:		if (ent->item->giType == IT_POWERUP)
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
CNSTI4 5
NEI4 $654
line 1786
;1786:		{
line 1787
;1787:			if (ent->item->giTag == PW_FORCE_ENLIGHTENED_LIGHT ||
ADDRLP4 1112
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1112
INDIRI4
CNSTI4 12
EQI4 $658
ADDRLP4 1112
INDIRI4
CNSTI4 13
NEI4 $656
LABELV $658
line 1789
;1788:				ent->item->giTag == PW_FORCE_ENLIGHTENED_DARK)
;1789:			{
line 1790
;1790:				G_FreeEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 1791
;1791:				return;
ADDRGP4 $625
JUMPV
LABELV $656
line 1793
;1792:			}
;1793:		}
LABELV $654
line 1794
;1794:	}
LABELV $651
line 1796
;1795:
;1796:	if (g_forcePowerDisable.integer)
ADDRGP4 g_forcePowerDisable+12
INDIRI4
CNSTI4 0
EQI4 $659
line 1797
;1797:	{ //if force powers disabled, don't add force powerups
line 1798
;1798:		if (ent->item->giType == IT_POWERUP)
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
CNSTI4 5
NEI4 $662
line 1799
;1799:		{
line 1800
;1800:			if (ent->item->giTag == PW_FORCE_ENLIGHTENED_LIGHT ||
ADDRLP4 1112
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1112
INDIRI4
CNSTI4 12
EQI4 $667
ADDRLP4 1112
INDIRI4
CNSTI4 13
EQI4 $667
ADDRLP4 1112
INDIRI4
CNSTI4 14
NEI4 $664
LABELV $667
line 1803
;1801:				ent->item->giTag == PW_FORCE_ENLIGHTENED_DARK ||
;1802:				ent->item->giTag == PW_FORCE_BOON)
;1803:			{
line 1804
;1804:				G_FreeEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 1805
;1805:				return;
ADDRGP4 $625
JUMPV
LABELV $664
line 1807
;1806:			}
;1807:		}
LABELV $662
line 1808
;1808:	}
LABELV $659
line 1810
;1809:
;1810:	if (g_gametype.integer == GT_TOURNAMENT)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
NEI4 $668
line 1811
;1811:	{
line 1812
;1812:		if ( ent->item->giType == IT_ARMOR ||
ADDRLP4 1112
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1116
ADDRLP4 1112
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1120
CNSTI4 3
ASGNI4
ADDRLP4 1116
INDIRI4
ADDRLP4 1120
INDIRI4
EQI4 $674
ADDRLP4 1116
INDIRI4
CNSTI4 4
EQI4 $674
ADDRLP4 1116
INDIRI4
CNSTI4 6
NEI4 $671
ADDRLP4 1112
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ADDRLP4 1120
INDIRI4
NEI4 $671
LABELV $674
line 1815
;1813:			ent->item->giType == IT_HEALTH ||
;1814:			(ent->item->giType == IT_HOLDABLE && ent->item->giTag == HI_MEDPAC) )
;1815:		{
line 1816
;1816:			G_FreeEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 1817
;1817:			return;
ADDRGP4 $625
JUMPV
LABELV $671
line 1819
;1818:		}
;1819:	}
LABELV $668
line 1821
;1820:
;1821:	if (g_gametype.integer != GT_CTF &&
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 7
EQI4 $675
ADDRLP4 1112
CNSTI4 8
ASGNI4
ADDRGP4 g_gametype+12
INDIRI4
ADDRLP4 1112
INDIRI4
EQI4 $675
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
ADDRLP4 1112
INDIRI4
NEI4 $675
line 1824
;1822:		g_gametype.integer != GT_CTY &&
;1823:		ent->item->giType == IT_TEAM)
;1824:	{
line 1825
;1825:		int killMe = 0;
ADDRLP4 1116
CNSTI4 0
ASGNI4
line 1827
;1826:
;1827:		switch (ent->item->giTag)
ADDRLP4 1120
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1120
INDIRI4
CNSTI4 4
EQI4 $682
ADDRLP4 1120
INDIRI4
CNSTI4 5
EQI4 $683
ADDRLP4 1120
INDIRI4
CNSTI4 6
EQI4 $684
ADDRGP4 $680
JUMPV
line 1828
;1828:		{
LABELV $682
line 1830
;1829:		case PW_REDFLAG:
;1830:			killMe = 1;
ADDRLP4 1116
CNSTI4 1
ASGNI4
line 1831
;1831:			break;
ADDRGP4 $680
JUMPV
LABELV $683
line 1833
;1832:		case PW_BLUEFLAG:
;1833:			killMe = 1;
ADDRLP4 1116
CNSTI4 1
ASGNI4
line 1834
;1834:			break;
ADDRGP4 $680
JUMPV
LABELV $684
line 1836
;1835:		case PW_NEUTRALFLAG:
;1836:			killMe = 1;
ADDRLP4 1116
CNSTI4 1
ASGNI4
line 1837
;1837:			break;
line 1839
;1838:		default:
;1839:			break;
LABELV $680
line 1842
;1840:		}
;1841:
;1842:		if (killMe)
ADDRLP4 1116
INDIRI4
CNSTI4 0
EQI4 $685
line 1843
;1843:		{
line 1844
;1844:			G_FreeEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 1845
;1845:			return;
ADDRGP4 $625
JUMPV
LABELV $685
line 1847
;1846:		}
;1847:	}
LABELV $675
line 1849
;1848:
;1849:	VectorSet (ent->r.mins, -8, -8, -0);
ADDRFP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3238002688
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 0
ASGNF4
line 1850
;1850:	VectorSet (ent->r.maxs, 8, 8, 16);
ADDRFP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1090519040
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1098907648
ASGNF4
line 1852
;1851:
;1852:	ent->s.eType = ET_ITEM;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 2
ASGNI4
line 1853
;1853:	ent->s.modelindex = ent->item - bg_itemlist;		// store item number in modelindex
ADDRLP4 1116
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1116
INDIRP4
CNSTI4 212
ADDP4
ADDRLP4 1116
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CVPU4 4
ADDRGP4 bg_itemlist
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 52
DIVI4
ASGNI4
line 1854
;1854:	ent->s.modelindex2 = 0; // zero indicates this isn't a dropped item
ADDRFP4 0
INDIRP4
CNSTI4 216
ADDP4
CNSTI4 0
ASGNI4
line 1856
;1855:
;1856:	ent->r.contents = CONTENTS_TRIGGER;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1024
ASGNI4
line 1857
;1857:	ent->touch = Touch_Item;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 Touch_Item
ASGNP4
line 1859
;1858:	// useing an item causes it to respawn
;1859:	ent->use = Use_Item;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 Use_Item
ASGNP4
line 1869
;1860:
;1861:	// create a Ghoul2 model if the world model is a glm
;1862:/*	item = &bg_itemlist[ ent->s.modelindex ];
;1863:	if (!stricmp(&item->world_model[0][strlen(item->world_model[0]) - 4], ".glm"))
;1864:	{
;1865:		trap_G2API_InitGhoul2Model(&ent->s, item->world_model[0], G_ModelIndex(item->world_model[0] ), 0, 0, 0, 0);
;1866:		ent->s.radius = 60;
;1867:	}
;1868:*/
;1869:	if ( ent->spawnflags & 1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $687
line 1871
;1870:		// suspended
;1871:		G_SetOrigin( ent, ent->s.origin );
ADDRLP4 1120
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1120
INDIRP4
ARGP4
ADDRLP4 1120
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1872
;1872:	} else {
ADDRGP4 $688
JUMPV
LABELV $687
line 1877
;1873:		// drop to floor
;1874:
;1875:		//if it is directly even with the floor it will return startsolid, so raise up by 0.1
;1876:		//and temporarily subtract 0.1 from the z maxs so that going up doesn't push into the ceiling
;1877:		ent->s.origin[2] += 0.1;
ADDRLP4 1120
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
ASGNP4
ADDRLP4 1120
INDIRP4
ADDRLP4 1120
INDIRP4
INDIRF4
CNSTF4 1036831949
ADDF4
ASGNF4
line 1878
;1878:		ent->r.maxs[2] -= 0.1;
ADDRLP4 1124
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
ASGNP4
ADDRLP4 1124
INDIRP4
ADDRLP4 1124
INDIRP4
INDIRF4
CNSTF4 1036831949
SUBF4
ASGNF4
line 1880
;1879:
;1880:		VectorSet( dest, ent->s.origin[0], ent->s.origin[1], ent->s.origin[2] - 4096 );
ADDRLP4 1128
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1084
ADDRLP4 1128
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ASGNF4
ADDRLP4 1084+4
ADDRLP4 1128
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ASGNF4
ADDRLP4 1084+8
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
CNSTF4 1166016512
SUBF4
ASGNF4
line 1881
;1881:		trap_Trace( &tr, ent->s.origin, ent->r.mins, ent->r.maxs, dest, ent->s.number, MASK_SOLID );
ADDRLP4 4
ARGP4
ADDRLP4 1132
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1132
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 1132
INDIRP4
CNSTI4 316
ADDP4
ARGP4
ADDRLP4 1132
INDIRP4
CNSTI4 328
ADDP4
ARGP4
ADDRLP4 1084
ARGP4
ADDRLP4 1132
INDIRP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 1882
;1882:		if ( tr.startsolid ) {
ADDRLP4 4+4
INDIRI4
CNSTI4 0
EQI4 $691
line 1883
;1883:			G_Printf ("FinishSpawningItem: %s startsolid at %s\n", ent->classname, vtos(ent->s.origin));
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 1136
ADDRGP4 vtos
CALLP4
ASGNP4
ADDRGP4 $694
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 416
ADDP4
INDIRP4
ARGP4
ADDRLP4 1136
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 1884
;1884:			G_FreeEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 1885
;1885:			return;
ADDRGP4 $625
JUMPV
LABELV $691
line 1889
;1886:		}
;1887:
;1888:		//add the 0.1 back after the trace
;1889:		ent->r.maxs[2] += 0.1;
ADDRLP4 1136
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
ASGNP4
ADDRLP4 1136
INDIRP4
ADDRLP4 1136
INDIRP4
INDIRF4
CNSTF4 1036831949
ADDF4
ASGNF4
line 1892
;1890:
;1891:		// allow to ride movers
;1892:		ent->s.groundEntityNum = tr.entityNum;
ADDRFP4 0
INDIRP4
CNSTI4 192
ADDP4
ADDRLP4 4+52
INDIRI4
ASGNI4
line 1894
;1893:
;1894:		G_SetOrigin( ent, tr.endpos );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4+12
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1895
;1895:	}
LABELV $688
line 1898
;1896:
;1897:	// team slaves and targeted items aren't present at start
;1898:	if ( ( ent->flags & FL_TEAMSLAVE ) || ent->targetname ) {
ADDRLP4 1120
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1120
INDIRP4
CNSTI4 472
ADDP4
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
NEI4 $699
ADDRLP4 1120
INDIRP4
CNSTI4 588
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $697
LABELV $699
line 1899
;1899:		ent->s.eFlags |= EF_NODRAW;
ADDRLP4 1124
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 1124
INDIRP4
ADDRLP4 1124
INDIRP4
INDIRI4
CNSTI4 128
BORI4
ASGNI4
line 1900
;1900:		ent->r.contents = 0;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 0
ASGNI4
line 1901
;1901:		return;
ADDRGP4 $625
JUMPV
LABELV $697
line 1918
;1902:	}
;1903:
;1904:	// powerups don't spawn in for a while
;1905:	/*
;1906:	if ( ent->item->giType == IT_POWERUP ) {
;1907:		float	respawn;
;1908:
;1909:		respawn = 45 + crandom() * 15;
;1910:		ent->s.eFlags |= EF_NODRAW;
;1911:		ent->r.contents = 0;
;1912:		ent->nextthink = level.time + respawn * 1000;
;1913:		ent->think = RespawnItem;
;1914:		return;
;1915:	}
;1916:	*/
;1917:
;1918:	trap_LinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1919
;1919:}
LABELV $625
endproc FinishSpawningItem 1140 28
export G_CheckTeamItems
proc G_CheckTeamItems 20 4
line 1929
;1920:
;1921:
;1922:qboolean	itemRegistered[MAX_ITEMS];
;1923:
;1924:/*
;1925:==================
;1926:G_CheckTeamItems
;1927:==================
;1928:*/
;1929:void G_CheckTeamItems( void ) {
line 1932
;1930:
;1931:	// Set up team stuff
;1932:	Team_InitGame();
ADDRGP4 Team_InitGame
CALLV
pop
line 1934
;1933:
;1934:	if( g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 7
EQI4 $705
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 8
NEI4 $701
LABELV $705
line 1938
;1935:		gitem_t	*item;
;1936:
;1937:		// check for the two flags
;1938:		item = BG_FindItem( "team_CTF_redflag" );
ADDRGP4 $604
ARGP4
ADDRLP4 4
ADDRGP4 BG_FindItem
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 1939
;1939:		if ( !item || !itemRegistered[ item - bg_itemlist ] ) {
ADDRLP4 8
ADDRLP4 0
INDIRP4
CVPU4 4
ASGNU4
ADDRLP4 8
INDIRU4
CNSTU4 0
EQU4 $708
ADDRLP4 8
INDIRU4
ADDRGP4 bg_itemlist
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 52
DIVI4
CNSTI4 2
LSHI4
ADDRGP4 itemRegistered
ADDP4
INDIRI4
CNSTI4 0
NEI4 $706
LABELV $708
line 1940
;1940:			G_Printf( S_COLOR_YELLOW "WARNING: No team_CTF_redflag in map" );
ADDRGP4 $709
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 1941
;1941:		}
LABELV $706
line 1942
;1942:		item = BG_FindItem( "team_CTF_blueflag" );
ADDRGP4 $607
ARGP4
ADDRLP4 12
ADDRGP4 BG_FindItem
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ASGNP4
line 1943
;1943:		if ( !item || !itemRegistered[ item - bg_itemlist ] ) {
ADDRLP4 16
ADDRLP4 0
INDIRP4
CVPU4 4
ASGNU4
ADDRLP4 16
INDIRU4
CNSTU4 0
EQU4 $712
ADDRLP4 16
INDIRU4
ADDRGP4 bg_itemlist
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 52
DIVI4
CNSTI4 2
LSHI4
ADDRGP4 itemRegistered
ADDP4
INDIRI4
CNSTI4 0
NEI4 $710
LABELV $712
line 1944
;1944:			G_Printf( S_COLOR_YELLOW "WARNING: No team_CTF_blueflag in map" );
ADDRGP4 $713
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 1945
;1945:		}
LABELV $710
line 1946
;1946:	}
LABELV $701
line 1947
;1947:}
LABELV $700
endproc G_CheckTeamItems 20 4
export ClearRegisteredItems
proc ClearRegisteredItems 12 12
line 1954
;1948:
;1949:/*
;1950:==============
;1951:ClearRegisteredItems
;1952:==============
;1953:*/
;1954:void ClearRegisteredItems( void ) {
line 1955
;1955:	memset( itemRegistered, 0, sizeof( itemRegistered ) );
ADDRGP4 itemRegistered
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1024
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1958
;1956:
;1957:	// players always start with the base weapon
;1958:	RegisterItem( BG_FindItemForWeapon( WP_BRYAR_PISTOL ) );
CNSTI4 3
ARGI4
ADDRLP4 0
ADDRGP4 BG_FindItemForWeapon
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 RegisterItem
CALLV
pop
line 1959
;1959:	RegisterItem( BG_FindItemForWeapon( WP_STUN_BATON ) );
CNSTI4 1
ARGI4
ADDRLP4 4
ADDRGP4 BG_FindItemForWeapon
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 RegisterItem
CALLV
pop
line 1960
;1960:	RegisterItem( BG_FindItemForWeapon( WP_SABER ) );
CNSTI4 2
ARGI4
ADDRLP4 8
ADDRGP4 BG_FindItemForWeapon
CALLP4
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 RegisterItem
CALLV
pop
line 1961
;1961:}
LABELV $714
endproc ClearRegisteredItems 12 12
export RegisterItem
proc RegisterItem 0 4
line 1970
;1962:
;1963:/*
;1964:===============
;1965:RegisterItem
;1966:
;1967:The item will be added to the precache list
;1968:===============
;1969:*/
;1970:void RegisterItem( gitem_t *item ) {
line 1971
;1971:	if ( !item ) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $716
line 1972
;1972:		G_Error( "RegisterItem: NULL" );
ADDRGP4 $718
ARGP4
ADDRGP4 G_Error
CALLV
pop
line 1973
;1973:	}
LABELV $716
line 1974
;1974:	itemRegistered[ item - bg_itemlist ] = qtrue;
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRGP4 bg_itemlist
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 52
DIVI4
CNSTI4 2
LSHI4
ADDRGP4 itemRegistered
ADDP4
CNSTI4 1
ASGNI4
line 1975
;1975:}
LABELV $715
endproc RegisterItem 0 4
export SaveRegisteredItems
proc SaveRegisteredItems 268 8
line 1986
;1976:
;1977:
;1978:/*
;1979:===============
;1980:SaveRegisteredItems
;1981:
;1982:Write the needed items to a config string
;1983:so the client will know which ones to precache
;1984:===============
;1985:*/
;1986:void SaveRegisteredItems( void ) {
line 1991
;1987:	char	string[MAX_ITEMS+1];
;1988:	int		i;
;1989:	int		count;
;1990:
;1991:	count = 0;
ADDRLP4 264
CNSTI4 0
ASGNI4
line 1992
;1992:	for ( i = 0 ; i < bg_numItems ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $723
JUMPV
LABELV $720
line 1993
;1993:		if ( itemRegistered[i] ) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 itemRegistered
ADDP4
INDIRI4
CNSTI4 0
EQI4 $724
line 1994
;1994:			count++;
ADDRLP4 264
ADDRLP4 264
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1995
;1995:			string[i] = '1';
ADDRLP4 0
INDIRI4
ADDRLP4 4
ADDP4
CNSTI1 49
ASGNI1
line 1996
;1996:		} else {
ADDRGP4 $725
JUMPV
LABELV $724
line 1997
;1997:			string[i] = '0';
ADDRLP4 0
INDIRI4
ADDRLP4 4
ADDP4
CNSTI1 48
ASGNI1
line 1998
;1998:		}
LABELV $725
line 1999
;1999:	}
LABELV $721
line 1992
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $723
ADDRLP4 0
INDIRI4
ADDRGP4 bg_numItems
INDIRI4
LTI4 $720
line 2000
;2000:	string[ bg_numItems ] = 0;
ADDRGP4 bg_numItems
INDIRI4
ADDRLP4 4
ADDP4
CNSTI1 0
ASGNI1
line 2002
;2001:
;2002:	G_Printf( "%i items registered\n", count );
ADDRGP4 $726
ARGP4
ADDRLP4 264
INDIRI4
ARGI4
ADDRGP4 G_Printf
CALLV
pop
line 2003
;2003:	trap_SetConfigstring(CS_ITEMS, string);
CNSTI4 27
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 2004
;2004:}
LABELV $719
endproc SaveRegisteredItems 268 8
export G_ItemDisabled
proc G_ItemDisabled 132 16
line 2011
;2005:
;2006:/*
;2007:============
;2008:G_ItemDisabled
;2009:============
;2010:*/
;2011:int G_ItemDisabled( gitem_t *item ) {
line 2015
;2012:
;2013:	char name[128];
;2014:
;2015:	Com_sprintf(name, sizeof(name), "disable_%s", item->classname);
ADDRLP4 0
ARGP4
CNSTI4 128
ARGI4
ADDRGP4 $728
ARGP4
ADDRFP4 0
INDIRP4
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 2016
;2016:	return trap_Cvar_VariableIntegerValue( name );
ADDRLP4 0
ARGP4
ADDRLP4 128
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRLP4 128
INDIRI4
RETI4
LABELV $727
endproc G_ItemDisabled 132 16
export G_SpawnItem
proc G_SpawnItem 4 12
line 2029
;2017:}
;2018:
;2019:/*
;2020:============
;2021:G_SpawnItem
;2022:
;2023:Sets the clipping size and plants the object on the floor.
;2024:
;2025:Items can't be immediately dropped to floor, because they might
;2026:be on an entity that hasn't spawned yet.
;2027:============
;2028:*/
;2029:void G_SpawnItem (gentity_t *ent, gitem_t *item) {
line 2030
;2030:	G_SpawnFloat( "random", "0", &ent->random );
ADDRGP4 $730
ARGP4
ADDRGP4 $731
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
line 2031
;2031:	G_SpawnFloat( "wait", "0", &ent->wait );
ADDRGP4 $732
ARGP4
ADDRGP4 $731
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 756
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
line 2033
;2032:
;2033:	RegisterItem( item );
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 RegisterItem
CALLV
pop
line 2034
;2034:	if ( G_ItemDisabled(item) )
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 G_ItemDisabled
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $733
line 2035
;2035:		return;
ADDRGP4 $729
JUMPV
LABELV $733
line 2037
;2036:
;2037:	ent->item = item;
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
ADDRFP4 4
INDIRP4
ASGNP4
line 2040
;2038:	// some movers spawn on the second frame, so delay item
;2039:	// spawns until the third frame so they can ride trains
;2040:	ent->nextthink = level.time + FRAMETIME * 2;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 200
ADDI4
ASGNI4
line 2041
;2041:	ent->think = FinishSpawningItem;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 FinishSpawningItem
ASGNP4
line 2043
;2042:
;2043:	ent->physicsBounce = 0.50;		// items are bouncy
ADDRFP4 0
INDIRP4
CNSTI4 504
ADDP4
CNSTF4 1056964608
ASGNF4
line 2045
;2044:
;2045:	if ( item->giType == IT_POWERUP ) {
ADDRFP4 4
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
CNSTI4 5
NEI4 $736
line 2046
;2046:		G_SoundIndex( "sound/items/respawn1" );
ADDRGP4 $518
ARGP4
ADDRGP4 G_SoundIndex
CALLI4
pop
line 2047
;2047:		G_SpawnFloat( "noglobalsound", "0", &ent->speed);
ADDRGP4 $738
ARGP4
ADDRGP4 $731
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 608
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
line 2048
;2048:	}
LABELV $736
line 2049
;2049:}
LABELV $729
endproc G_SpawnItem 4 12
export G_BounceItem
proc G_BounceItem 92 12
line 2058
;2050:
;2051:
;2052:/*
;2053:================
;2054:G_BounceItem
;2055:
;2056:================
;2057:*/
;2058:void G_BounceItem( gentity_t *ent, trace_t *trace ) {
line 2064
;2059:	vec3_t	velocity;
;2060:	float	dot;
;2061:	int		hitTime;
;2062:
;2063:	// reflect the velocity on the trace plane
;2064:	hitTime = level.previousTime + ( level.time - level.previousTime ) * trace->fraction;
ADDRLP4 16
ADDRGP4 level+36
INDIRI4
CVIF4 4
ADDRGP4 level+32
INDIRI4
ADDRGP4 level+36
INDIRI4
SUBI4
CVIF4 4
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
MULF4
ADDF4
CVFI4 4
ASGNI4
line 2065
;2065:	BG_EvaluateTrajectoryDelta( &ent->s.pos, hitTime, velocity );
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
ARGP4
ADDRLP4 16
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BG_EvaluateTrajectoryDelta
CALLV
pop
line 2066
;2066:	dot = DotProduct( velocity, trace->plane.normal );
ADDRLP4 20
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 12
ADDRLP4 0
INDIRF4
ADDRLP4 20
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
MULF4
ADDRLP4 0+4
INDIRF4
ADDRLP4 20
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
MULF4
ADDF4
ADDRLP4 0+8
INDIRF4
ADDRLP4 20
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 2067
;2067:	VectorMA( velocity, -2*dot, trace->plane.normal, ent->s.pos.trDelta );
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 0
INDIRF4
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
CNSTF4 3221225472
ADDRLP4 12
INDIRF4
MULF4
MULF4
ADDF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 0+4
INDIRF4
ADDRFP4 4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
CNSTF4 3221225472
ADDRLP4 12
INDIRF4
MULF4
MULF4
ADDF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 0+8
INDIRF4
ADDRFP4 4
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
CNSTF4 3221225472
ADDRLP4 12
INDIRF4
MULF4
MULF4
ADDF4
ASGNF4
line 2070
;2068:
;2069:	// cut the velocity to keep from bouncing forever
;2070:	VectorScale( ent->s.pos.trDelta, ent->physicsBounce, ent->s.pos.trDelta );
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
ADDRLP4 24
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRF4
ADDRLP4 24
INDIRP4
CNSTI4 504
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
ADDRLP4 32
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRF4
ADDRLP4 32
INDIRP4
CNSTI4 504
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
ADDRLP4 40
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRF4
ADDRLP4 40
INDIRP4
CNSTI4 504
ADDP4
INDIRF4
MULF4
ASGNF4
line 2072
;2071:
;2072:	if ((ent->s.weapon == WP_DET_PACK && ent->s.eType == ET_GENERAL && ent->physicsObject))
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 13
NEI4 $747
ADDRLP4 52
CNSTI4 0
ASGNI4
ADDRLP4 48
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ADDRLP4 52
INDIRI4
NEI4 $747
ADDRLP4 48
INDIRP4
CNSTI4 500
ADDP4
INDIRI4
ADDRLP4 52
INDIRI4
EQI4 $747
line 2073
;2073:	{ //detpacks only
line 2074
;2074:		if (ent->touch)
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $749
line 2075
;2075:		{
line 2076
;2076:			ent->touch(ent, &g_entities[trace->entityNum], trace);
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
ARGP4
ADDRLP4 60
ADDRFP4 4
INDIRP4
ASGNP4
CNSTI4 828
ADDRLP4 60
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRLP4 56
INDIRP4
CNSTI4 648
ADDP4
INDIRP4
CALLV
pop
line 2077
;2077:			return;
ADDRGP4 $739
JUMPV
LABELV $749
line 2079
;2078:		}
;2079:	}
LABELV $747
line 2082
;2080:
;2081:	// check for stop
;2082:	if ( trace->plane.normal[2] > 0 && ent->s.pos.trDelta[2] < 40 ) {
ADDRFP4 4
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
CNSTF4 0
LEF4 $751
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRF4
CNSTF4 1109393408
GEF4 $751
line 2083
;2083:		trace->endpos[2] += 1.0;	// make sure it is off ground
ADDRLP4 56
ADDRFP4 4
INDIRP4
CNSTI4 20
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 2084
;2084:		SnapVector( trace->endpos );
ADDRLP4 60
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 64
ADDRFP4 4
INDIRP4
CNSTI4 16
ADDP4
ASGNP4
ADDRLP4 64
INDIRP4
ADDRLP4 64
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 68
ADDRFP4 4
INDIRP4
CNSTI4 20
ADDP4
ASGNP4
ADDRLP4 68
INDIRP4
ADDRLP4 68
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
line 2085
;2085:		G_SetOrigin( ent, trace->endpos );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 2086
;2086:		ent->s.groundEntityNum = trace->entityNum;
ADDRFP4 0
INDIRP4
CNSTI4 192
ADDP4
ADDRFP4 4
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
ASGNI4
line 2087
;2087:		return;
ADDRGP4 $739
JUMPV
LABELV $751
line 2090
;2088:	}
;2089:
;2090:	VectorAdd( ent->r.currentOrigin, trace->plane.normal, ent->r.currentOrigin);
ADDRLP4 56
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRF4
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 60
ADDRFP4 0
INDIRP4
CNSTI4 372
ADDP4
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRF4
ADDRFP4 4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 64
ADDRFP4 0
INDIRP4
CNSTI4 376
ADDP4
ASGNP4
ADDRLP4 64
INDIRP4
ADDRLP4 64
INDIRP4
INDIRF4
ADDRFP4 4
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDF4
ASGNF4
line 2091
;2091:	VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );
ADDRLP4 68
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 68
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 68
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 2092
;2092:	ent->s.pos.trTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 2094
;2093:
;2094:	if (ent->s.eType == ET_HOLOCRON ||
ADDRLP4 72
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 76
ADDRLP4 72
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 76
INDIRI4
CNSTI4 5
EQI4 $756
ADDRLP4 80
CNSTI4 0
ASGNI4
ADDRLP4 72
INDIRP4
CNSTI4 268
ADDP4
INDIRI4
ADDRLP4 80
INDIRI4
EQI4 $754
ADDRLP4 76
INDIRI4
ADDRLP4 80
INDIRI4
NEI4 $754
ADDRLP4 72
INDIRP4
CNSTI4 500
ADDP4
INDIRI4
ADDRLP4 80
INDIRI4
EQI4 $754
LABELV $756
line 2096
;2095:		(ent->s.shouldtarget && ent->s.eType == ET_GENERAL && ent->physicsObject))
;2096:	{ //holocrons and sentry guns
line 2097
;2097:		if (ent->touch)
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $757
line 2098
;2098:		{
line 2099
;2099:			ent->touch(ent, &g_entities[trace->entityNum], trace);
ADDRLP4 84
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 84
INDIRP4
ARGP4
ADDRLP4 88
ADDRFP4 4
INDIRP4
ASGNP4
CNSTI4 828
ADDRLP4 88
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRLP4 88
INDIRP4
ARGP4
ADDRLP4 84
INDIRP4
CNSTI4 648
ADDP4
INDIRP4
CALLV
pop
line 2100
;2100:		}
LABELV $757
line 2101
;2101:	}
LABELV $754
line 2102
;2102:}
LABELV $739
endproc G_BounceItem 92 12
export G_RunItem
proc G_RunItem 1112 28
line 2111
;2103:
;2104:
;2105:/*
;2106:================
;2107:G_RunItem
;2108:
;2109:================
;2110:*/
;2111:void G_RunItem( gentity_t *ent ) {
line 2118
;2112:	vec3_t		origin;
;2113:	trace_t		tr;
;2114:	int			contents;
;2115:	int			mask;
;2116:
;2117:	// if groundentity has been set to -1, it may have been pushed off an edge
;2118:	if ( ent->s.groundEntityNum == -1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 192
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $760
line 2119
;2119:		if ( ent->s.pos.trType != TR_GRAVITY ) {
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 5
EQI4 $762
line 2120
;2120:			ent->s.pos.trType = TR_GRAVITY;
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 5
ASGNI4
line 2121
;2121:			ent->s.pos.trTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 2122
;2122:		}
LABELV $762
line 2123
;2123:	}
LABELV $760
line 2125
;2124:
;2125:	if ( ent->s.pos.trType == TR_STATIONARY ) {
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 0
NEI4 $765
line 2127
;2126:		// check think function
;2127:		G_RunThink( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_RunThink
CALLV
pop
line 2128
;2128:		return;
ADDRGP4 $759
JUMPV
LABELV $765
line 2132
;2129:	}
;2130:
;2131:	// get current position
;2132:	BG_EvaluateTrajectory( &ent->s.pos, level.time, origin );
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
ARGP4
ADDRGP4 level+32
INDIRI4
ARGI4
ADDRLP4 1080
ARGP4
ADDRGP4 BG_EvaluateTrajectory
CALLV
pop
line 2135
;2133:
;2134:	// trace a line from the previous position to the current position
;2135:	if ( ent->clipmask ) {
ADDRFP4 0
INDIRP4
CNSTI4 508
ADDP4
INDIRI4
CNSTI4 0
EQI4 $768
line 2136
;2136:		mask = ent->clipmask;
ADDRLP4 1096
ADDRFP4 0
INDIRP4
CNSTI4 508
ADDP4
INDIRI4
ASGNI4
line 2137
;2137:	} else {
ADDRGP4 $769
JUMPV
LABELV $768
line 2138
;2138:		mask = MASK_PLAYERSOLID & ~CONTENTS_BODY;//MASK_SOLID;
ADDRLP4 1096
CNSTI4 17
ASGNI4
line 2139
;2139:	}
LABELV $769
line 2140
;2140:	trap_Trace( &tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, origin, 
ADDRLP4 0
ARGP4
ADDRLP4 1100
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1100
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 1100
INDIRP4
CNSTI4 316
ADDP4
ARGP4
ADDRLP4 1100
INDIRP4
CNSTI4 328
ADDP4
ARGP4
ADDRLP4 1080
ARGP4
ADDRLP4 1100
INDIRP4
CNSTI4 396
ADDP4
INDIRI4
ARGI4
ADDRLP4 1096
INDIRI4
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 2143
;2141:		ent->r.ownerNum, mask );
;2142:
;2143:	VectorCopy( tr.endpos, ent->r.currentOrigin );
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDRLP4 0+12
INDIRB
ASGNB 12
line 2145
;2144:
;2145:	if ( tr.startsolid ) {
ADDRLP4 0+4
INDIRI4
CNSTI4 0
EQI4 $771
line 2146
;2146:		tr.fraction = 0;
ADDRLP4 0+8
CNSTF4 0
ASGNF4
line 2147
;2147:	}
LABELV $771
line 2149
;2148:
;2149:	trap_LinkEntity( ent );	// FIXME: avoid this for stationary?
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 2152
;2150:
;2151:	// check think function
;2152:	G_RunThink( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_RunThink
CALLV
pop
line 2154
;2153:
;2154:	if ( tr.fraction == 1 ) {
ADDRLP4 0+8
INDIRF4
CNSTF4 1065353216
NEF4 $775
line 2155
;2155:		return;
ADDRGP4 $759
JUMPV
LABELV $775
line 2159
;2156:	}
;2157:
;2158:	// if it is in a nodrop volume, remove it
;2159:	contents = trap_PointContents( ent->r.currentOrigin, -1 );
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
CNSTI4 -1
ARGI4
ADDRLP4 1104
ADDRGP4 trap_PointContents
CALLI4
ASGNI4
ADDRLP4 1092
ADDRLP4 1104
INDIRI4
ASGNI4
line 2160
;2160:	if ( contents & CONTENTS_NODROP ) {
ADDRLP4 1092
INDIRI4
CNSTI4 2048
BANDI4
CNSTI4 0
EQI4 $778
line 2161
;2161:		if (ent->item && ent->item->giType == IT_TEAM) {
ADDRLP4 1108
ADDRFP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1108
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $780
ADDRLP4 1108
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
CNSTI4 8
NEI4 $780
line 2162
;2162:			Team_FreeEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Team_FreeEntity
CALLV
pop
line 2163
;2163:		} else {
ADDRGP4 $759
JUMPV
LABELV $780
line 2164
;2164:			G_FreeEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 2165
;2165:		}
line 2166
;2166:		return;
ADDRGP4 $759
JUMPV
LABELV $778
line 2169
;2167:	}
;2168:
;2169:	G_BounceItem( ent, &tr );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_BounceItem
CALLV
pop
line 2170
;2170:}
LABELV $759
endproc G_RunItem 1112 28
bss
export itemRegistered
align 4
LABELV itemRegistered
skip 1024
import droppedBlueFlag
import droppedRedFlag
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
import G_ClearClientLog
import G_LogExit
import G_LogWeaponOutput
import G_LogWeaponInit
import G_LogWeaponItem
import G_LogWeaponPowerup
import G_LogWeaponFrag
import G_LogWeaponDeath
import G_LogWeaponKill
import G_LogWeaponDamage
import G_LogWeaponFire
import G_LogWeaponPickup
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
import ArmorIndex
import Think_Weapon
import SetRespawn
import PrecacheItem
import UseHoldableItem
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
LABELV $738
char 1 110
char 1 111
char 1 103
char 1 108
char 1 111
char 1 98
char 1 97
char 1 108
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 0
align 1
LABELV $732
char 1 119
char 1 97
char 1 105
char 1 116
char 1 0
align 1
LABELV $731
char 1 48
char 1 0
align 1
LABELV $730
char 1 114
char 1 97
char 1 110
char 1 100
char 1 111
char 1 109
char 1 0
align 1
LABELV $728
char 1 100
char 1 105
char 1 115
char 1 97
char 1 98
char 1 108
char 1 101
char 1 95
char 1 37
char 1 115
char 1 0
align 1
LABELV $726
char 1 37
char 1 105
char 1 32
char 1 105
char 1 116
char 1 101
char 1 109
char 1 115
char 1 32
char 1 114
char 1 101
char 1 103
char 1 105
char 1 115
char 1 116
char 1 101
char 1 114
char 1 101
char 1 100
char 1 10
char 1 0
align 1
LABELV $718
char 1 82
char 1 101
char 1 103
char 1 105
char 1 115
char 1 116
char 1 101
char 1 114
char 1 73
char 1 116
char 1 101
char 1 109
char 1 58
char 1 32
char 1 78
char 1 85
char 1 76
char 1 76
char 1 0
align 1
LABELV $713
char 1 94
char 1 51
char 1 87
char 1 65
char 1 82
char 1 78
char 1 73
char 1 78
char 1 71
char 1 58
char 1 32
char 1 78
char 1 111
char 1 32
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
char 1 32
char 1 105
char 1 110
char 1 32
char 1 109
char 1 97
char 1 112
char 1 0
align 1
LABELV $709
char 1 94
char 1 51
char 1 87
char 1 65
char 1 82
char 1 78
char 1 73
char 1 78
char 1 71
char 1 58
char 1 32
char 1 78
char 1 111
char 1 32
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
char 1 32
char 1 105
char 1 110
char 1 32
char 1 109
char 1 97
char 1 112
char 1 0
align 1
LABELV $694
char 1 70
char 1 105
char 1 110
char 1 105
char 1 115
char 1 104
char 1 83
char 1 112
char 1 97
char 1 119
char 1 110
char 1 105
char 1 110
char 1 103
char 1 73
char 1 116
char 1 101
char 1 109
char 1 58
char 1 32
char 1 37
char 1 115
char 1 32
char 1 115
char 1 116
char 1 97
char 1 114
char 1 116
char 1 115
char 1 111
char 1 108
char 1 105
char 1 100
char 1 32
char 1 97
char 1 116
char 1 32
char 1 37
char 1 115
char 1 10
char 1 0
align 1
LABELV $607
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
LABELV $604
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
LABELV $543
char 1 73
char 1 116
char 1 101
char 1 109
char 1 58
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 115
char 1 10
char 1 0
align 1
LABELV $518
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 105
char 1 116
char 1 101
char 1 109
char 1 115
char 1 47
char 1 114
char 1 101
char 1 115
char 1 112
char 1 97
char 1 119
char 1 110
char 1 49
char 1 0
align 1
LABELV $505
char 1 82
char 1 101
char 1 115
char 1 112
char 1 97
char 1 119
char 1 110
char 1 73
char 1 116
char 1 101
char 1 109
char 1 58
char 1 32
char 1 98
char 1 97
char 1 100
char 1 32
char 1 116
char 1 101
char 1 97
char 1 109
char 1 109
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 0
align 1
LABELV $409
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
LABELV $408
char 1 115
char 1 101
char 1 110
char 1 116
char 1 114
char 1 121
char 1 71
char 1 117
char 1 110
char 1 0
align 1
LABELV $286
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 99
char 1 104
char 1 97
char 1 114
char 1 115
char 1 47
char 1 116
char 1 117
char 1 114
char 1 114
char 1 101
char 1 116
char 1 47
char 1 115
char 1 104
char 1 117
char 1 116
char 1 100
char 1 111
char 1 119
char 1 110
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $262
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 99
char 1 104
char 1 97
char 1 114
char 1 115
char 1 47
char 1 116
char 1 117
char 1 114
char 1 114
char 1 101
char 1 116
char 1 47
char 1 115
char 1 116
char 1 97
char 1 114
char 1 116
char 1 117
char 1 112
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $228
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 99
char 1 104
char 1 97
char 1 114
char 1 115
char 1 47
char 1 116
char 1 117
char 1 114
char 1 114
char 1 101
char 1 116
char 1 47
char 1 112
char 1 105
char 1 110
char 1 103
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $164
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
LABELV $163
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
char 1 0
align 1
LABELV $162
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
char 1 0
align 1
LABELV $161
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
char 1 0
align 1
LABELV $160
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
char 1 0
