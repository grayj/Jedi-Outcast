data
align 4
LABELV playerMins
byte 4 3245342720
byte 4 3245342720
byte 4 3250585600
align 4
LABELV playerMaxs
byte 4 1097859072
byte 4 1097859072
byte 4 1109393408
export SP_info_player_deathmatch
code
proc SP_info_player_deathmatch 8 12
file "../g_client.c"
line 20
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:#include "g_local.h"
;4:#include "..\ghoul2\g2.h"
;5:
;6:// g_client.c -- client functions that don't happen every frame
;7:
;8:static vec3_t	playerMins = {-15, -15, DEFAULT_MINS_2};
;9:static vec3_t	playerMaxs = {15, 15, DEFAULT_MAXS_2};
;10:
;11:forcedata_t Client_Force[MAX_CLIENTS];
;12:
;13:/*QUAKED info_player_deathmatch (1 0 1) (-16 -16 -24) (16 16 32) initial
;14:potential spawning position for deathmatch games.
;15:The first time a player enters the game, they will be at an 'initial' spot.
;16:Targets will be fired when someone spawns in on them.
;17:"nobots" will prevent bots from using this spot.
;18:"nohumans" will prevent non-bots from using this spot.
;19:*/
;20:void SP_info_player_deathmatch( gentity_t *ent ) {
line 23
;21:	int		i;
;22:
;23:	G_SpawnInt( "nobots", "0", &i);
ADDRGP4 $79
ARGP4
ADDRGP4 $80
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 24
;24:	if ( i ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $81
line 25
;25:		ent->flags |= FL_NO_BOTS;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 472
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 8192
BORI4
ASGNI4
line 26
;26:	}
LABELV $81
line 27
;27:	G_SpawnInt( "nohumans", "0", &i );
ADDRGP4 $83
ARGP4
ADDRGP4 $80
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 28
;28:	if ( i ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $84
line 29
;29:		ent->flags |= FL_NO_HUMANS;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 472
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
line 30
;30:	}
LABELV $84
line 31
;31:}
LABELV $78
endproc SP_info_player_deathmatch 8 12
export SP_info_player_start
proc SP_info_player_start 0 4
line 36
;32:
;33:/*QUAKED info_player_start (1 0 0) (-16 -16 -24) (16 16 32)
;34:equivelant to info_player_deathmatch
;35:*/
;36:void SP_info_player_start(gentity_t *ent) {
line 37
;37:	ent->classname = "info_player_deathmatch";
ADDRFP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $87
ASGNP4
line 38
;38:	SP_info_player_deathmatch( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 SP_info_player_deathmatch
CALLV
pop
line 39
;39:}
LABELV $86
endproc SP_info_player_start 0 4
export SP_info_player_imperial
proc SP_info_player_imperial 0 4
line 44
;40:
;41:/*QUAKED info_player_imperial (1 0 0) (-16 -16 -24) (16 16 32)
;42:saga start point - imperial
;43:*/
;44:void SP_info_player_imperial(gentity_t *ent) {
line 45
;45:	if (g_gametype.integer != GT_SAGA)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 6
EQI4 $89
line 46
;46:	{ //turn into a DM spawn if not in saga game mode
line 47
;47:		ent->classname = "info_player_deathmatch";
ADDRFP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $87
ASGNP4
line 48
;48:		SP_info_player_deathmatch( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 SP_info_player_deathmatch
CALLV
pop
line 49
;49:	}
LABELV $89
line 50
;50:}
LABELV $88
endproc SP_info_player_imperial 0 4
export SP_info_player_rebel
proc SP_info_player_rebel 0 4
line 55
;51:
;52:/*QUAKED info_player_rebel (1 0 0) (-16 -16 -24) (16 16 32)
;53:saga start point - rebel
;54:*/
;55:void SP_info_player_rebel(gentity_t *ent) {
line 56
;56:	if (g_gametype.integer != GT_SAGA)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 6
EQI4 $93
line 57
;57:	{ //turn into a DM spawn if not in saga game mode
line 58
;58:		ent->classname = "info_player_deathmatch";
ADDRFP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $87
ASGNP4
line 59
;59:		SP_info_player_deathmatch( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 SP_info_player_deathmatch
CALLV
pop
line 60
;60:	}
LABELV $93
line 61
;61:}
LABELV $92
endproc SP_info_player_rebel 0 4
export SP_info_player_intermission
proc SP_info_player_intermission 0 0
line 66
;62:
;63:/*QUAKED info_player_intermission (1 0 1) (-16 -16 -24) (16 16 32)
;64:The intermission will be viewed from this point.  Target an info_notnull for the view direction.
;65:*/
;66:void SP_info_player_intermission( gentity_t *ent ) {
line 68
;67:
;68:}
LABELV $96
endproc SP_info_player_intermission 0 0
export ThrowSaberToAttacker
proc ThrowSaberToAttacker 68 8
line 73
;69:
;70:#define JMSABER_RESPAWN_TIME 20000 //in case it gets stuck somewhere no one can reach
;71:
;72:void ThrowSaberToAttacker(gentity_t *self, gentity_t *attacker)
;73:{
line 74
;74:	gentity_t *ent = &g_entities[self->client->ps.saberIndex];
ADDRLP4 0
CNSTI4 828
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 608
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 76
;75:	vec3_t a;
;76:	int altVelocity = 0;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 78
;77:
;78:	if (!ent || ent->enemy != self)
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $100
ADDRLP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
EQU4 $98
LABELV $100
line 79
;79:	{ //something has gone very wrong (this should never happen)
line 84
;80:		//but in case it does.. find the saber manually
;81:#ifdef _DEBUG
;82:		Com_Printf("Lost the saber! Attempting to use global pointer..\n");
;83:#endif
;84:		ent = gJMSaberEnt;
ADDRLP4 0
ADDRGP4 gJMSaberEnt
INDIRP4
ASGNP4
line 86
;85:
;86:		if (!ent)
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $101
line 87
;87:		{
line 91
;88:#ifdef _DEBUG
;89:			Com_Printf("The global pointer was NULL. This is a bad thing.\n");
;90:#endif
;91:			return;
ADDRGP4 $97
JUMPV
LABELV $101
line 98
;92:		}
;93:
;94:#ifdef _DEBUG
;95:		Com_Printf("Got it (%i). Setting enemy to client %i.\n", ent->s.number, self->s.number);
;96:#endif
;97:
;98:		ent->enemy = self;
ADDRLP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRFP4 0
INDIRP4
ASGNP4
line 99
;99:		self->client->ps.saberIndex = ent->s.number;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 608
ADDP4
ADDRLP4 0
INDIRP4
INDIRI4
ASGNI4
line 100
;100:	}
LABELV $98
line 102
;101:
;102:	trap_SetConfigstring ( CS_CLIENT_JEDIMASTER, "-1" );
CNSTI4 28
ARGI4
ADDRGP4 $103
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 104
;103:
;104:	if (attacker && attacker->client && self->client->ps.saberInFlight)
ADDRLP4 24
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 28
CNSTU4 0
ASGNU4
ADDRLP4 24
INDIRP4
CVPU4 4
ADDRLP4 28
INDIRU4
EQU4 $104
ADDRLP4 32
CNSTI4 408
ASGNI4
ADDRLP4 24
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 28
INDIRU4
EQU4 $104
ADDRFP4 0
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRP4
CNSTI4 504
ADDP4
INDIRI4
CNSTI4 0
EQI4 $104
line 105
;105:	{ //someone killed us and we had the saber thrown, so actually move this saber to the saber location
line 108
;106:	  //if we killed ourselves with saber thrown, however, same suicide rules of respawning at spawn spot still
;107:	  //apply.
;108:		gentity_t *flyingsaber = &g_entities[self->client->ps.saberEntityNum];
ADDRLP4 36
CNSTI4 828
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 544
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 110
;109:
;110:		if (flyingsaber && flyingsaber->inuse)
ADDRLP4 36
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $106
ADDRLP4 36
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $106
line 111
;111:		{
line 112
;112:			VectorCopy(flyingsaber->s.pos.trBase, ent->s.pos.trBase);
ADDRLP4 44
CNSTI4 24
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
ADDRLP4 36
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 113
;113:			VectorCopy(flyingsaber->s.pos.trDelta, ent->s.pos.trDelta);
ADDRLP4 48
CNSTI4 36
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
ADDRLP4 36
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 114
;114:			VectorCopy(flyingsaber->s.apos.trBase, ent->s.apos.trBase);
ADDRLP4 52
CNSTI4 60
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 52
INDIRI4
ADDP4
ADDRLP4 36
INDIRP4
ADDRLP4 52
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 115
;115:			VectorCopy(flyingsaber->s.apos.trDelta, ent->s.apos.trDelta);
ADDRLP4 56
CNSTI4 72
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
ADDRLP4 36
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 117
;116:
;117:			VectorCopy(flyingsaber->r.currentOrigin, ent->r.currentOrigin);
ADDRLP4 60
CNSTI4 368
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 60
INDIRI4
ADDP4
ADDRLP4 36
INDIRP4
ADDRLP4 60
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 118
;118:			VectorCopy(flyingsaber->r.currentAngles, ent->r.currentAngles);
ADDRLP4 64
CNSTI4 380
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 64
INDIRI4
ADDP4
ADDRLP4 36
INDIRP4
ADDRLP4 64
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 119
;119:			altVelocity = 1;
ADDRLP4 16
CNSTI4 1
ASGNI4
line 120
;120:		}
LABELV $106
line 121
;121:	}
LABELV $104
line 123
;122:
;123:	self->client->ps.saberInFlight = qtrue; //say he threw it anyway in order to properly remove from dead body
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 504
ADDP4
CNSTI4 1
ASGNI4
line 125
;124:
;125:	ent->s.modelindex = G_ModelIndex("models/weapons2/saber/saber_w.glm");
ADDRGP4 $108
ARGP4
ADDRLP4 36
ADDRGP4 G_ModelIndex
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 212
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
line 126
;126:	ent->s.eFlags &= ~(EF_NODRAW);
ADDRLP4 40
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRI4
CNSTI4 -129
BANDI4
ASGNI4
line 127
;127:	ent->s.modelGhoul2 = 1;
ADDRLP4 0
INDIRP4
CNSTI4 204
ADDP4
CNSTI4 1
ASGNI4
line 128
;128:	ent->s.eType = ET_MISSILE;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 3
ASGNI4
line 129
;129:	ent->enemy = NULL;
ADDRLP4 0
INDIRP4
CNSTI4 728
ADDP4
CNSTP4 0
ASGNP4
line 131
;130:
;131:	if (!attacker || !attacker->client)
ADDRLP4 44
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 48
CNSTU4 0
ASGNU4
ADDRLP4 44
INDIRP4
CVPU4 4
ADDRLP4 48
INDIRU4
EQU4 $111
ADDRLP4 44
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 48
INDIRU4
NEU4 $109
LABELV $111
line 132
;132:	{
line 133
;133:		VectorCopy(ent->s.origin2, ent->s.pos.trBase);
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 134
;134:		VectorCopy(ent->s.origin2, ent->s.origin);
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 135
;135:		VectorCopy(ent->s.origin2, ent->r.currentOrigin);
ADDRLP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 136
;136:		ent->pos2[0] = 0;
ADDRLP4 0
INDIRP4
CNSTI4 560
ADDP4
CNSTF4 0
ASGNF4
line 137
;137:		trap_LinkEntity(ent);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 138
;138:		return;
ADDRGP4 $97
JUMPV
LABELV $109
line 141
;139:	}
;140:
;141:	if (!altVelocity)
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $112
line 142
;142:	{
line 143
;143:		VectorCopy(self->s.pos.trBase, ent->s.pos.trBase);
ADDRLP4 52
CNSTI4 24
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 52
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 52
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 144
;144:		VectorCopy(self->s.pos.trBase, ent->s.origin);
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 145
;145:		VectorCopy(self->s.pos.trBase, ent->r.currentOrigin);
ADDRLP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 147
;146:
;147:		VectorSubtract(attacker->client->ps.origin, ent->s.pos.trBase, a);
ADDRLP4 56
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
ASGNP4
ADDRLP4 64
CNSTI4 24
ASGNI4
ADDRLP4 4
ADDRLP4 56
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
ADDRLP4 64
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 56
INDIRP4
INDIRP4
ADDRLP4 64
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+8
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
SUBF4
ASGNF4
line 149
;148:
;149:		VectorNormalize(a);
ADDRLP4 4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 151
;150:
;151:		ent->s.pos.trDelta[0] = a[0]*256;
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
CNSTF4 1132462080
ADDRLP4 4
INDIRF4
MULF4
ASGNF4
line 152
;152:		ent->s.pos.trDelta[1] = a[1]*256;
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1132462080
ADDRLP4 4+4
INDIRF4
MULF4
ASGNF4
line 153
;153:		ent->s.pos.trDelta[2] = 256;
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
CNSTF4 1132462080
ASGNF4
line 154
;154:	}
LABELV $112
line 156
;155:
;156:	trap_LinkEntity(ent);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 157
;157:}
LABELV $97
endproc ThrowSaberToAttacker 68 8
export JMSaberThink
proc JMSaberThink 28 4
line 160
;158:
;159:void JMSaberThink(gentity_t *ent)
;160:{
line 161
;161:	gJMSaberEnt = ent;
ADDRGP4 gJMSaberEnt
ADDRFP4 0
INDIRP4
ASGNP4
line 163
;162:
;163:	if (ent->enemy)
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $118
line 164
;164:	{
line 165
;165:		if (!ent->enemy->client || !ent->enemy->inuse)
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $122
ADDRLP4 0
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
NEI4 $120
LABELV $122
line 166
;166:		{ //disconnected?
line 167
;167:			VectorCopy(ent->enemy->s.pos.trBase, ent->s.pos.trBase);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
CNSTI4 24
ASGNI4
ADDRLP4 4
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 168
;168:			VectorCopy(ent->enemy->s.pos.trBase, ent->s.origin);
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 169
;169:			VectorCopy(ent->enemy->s.pos.trBase, ent->r.currentOrigin);
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 368
ADDP4
ADDRLP4 16
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 170
;170:			ent->s.modelindex = G_ModelIndex("models/weapons2/saber/saber_w.glm");
ADDRGP4 $108
ARGP4
ADDRLP4 20
ADDRGP4 G_ModelIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 212
ADDP4
ADDRLP4 20
INDIRI4
ASGNI4
line 171
;171:			ent->s.eFlags &= ~(EF_NODRAW);
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 -129
BANDI4
ASGNI4
line 172
;172:			ent->s.modelGhoul2 = 1;
ADDRFP4 0
INDIRP4
CNSTI4 204
ADDP4
CNSTI4 1
ASGNI4
line 173
;173:			ent->s.eType = ET_MISSILE;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 3
ASGNI4
line 174
;174:			ent->enemy = NULL;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
CNSTP4 0
ASGNP4
line 176
;175:
;176:			ent->pos2[0] = 1;
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
CNSTF4 1065353216
ASGNF4
line 177
;177:			ent->pos2[1] = 0; //respawn next think
ADDRFP4 0
INDIRP4
CNSTI4 564
ADDP4
CNSTF4 0
ASGNF4
line 178
;178:			trap_LinkEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 179
;179:		}
ADDRGP4 $119
JUMPV
LABELV $120
line 181
;180:		else
;181:		{
line 182
;182:			ent->pos2[1] = level.time + JMSABER_RESPAWN_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 564
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 20000
ADDI4
CVIF4 4
ASGNF4
line 183
;183:		}
line 184
;184:	}
ADDRGP4 $119
JUMPV
LABELV $118
line 185
;185:	else if (ent->pos2[0] && ent->pos2[1] < level.time)
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRF4
CNSTF4 0
EQF4 $124
ADDRLP4 0
INDIRP4
CNSTI4 564
ADDP4
INDIRF4
ADDRGP4 level+32
INDIRI4
CVIF4 4
GEF4 $124
line 186
;186:	{
line 187
;187:		VectorCopy(ent->s.origin2, ent->s.pos.trBase);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 188
;188:		VectorCopy(ent->s.origin2, ent->s.origin);
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 8
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 189
;189:		VectorCopy(ent->s.origin2, ent->r.currentOrigin);
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 368
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 190
;190:		ent->pos2[0] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
CNSTF4 0
ASGNF4
line 191
;191:		trap_LinkEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 192
;192:	}
LABELV $124
LABELV $119
line 194
;193:
;194:	ent->nextthink = level.time + 50;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 50
ADDI4
ASGNI4
line 195
;195:	G_RunObject(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_RunObject
CALLV
pop
line 196
;196:}
LABELV $117
endproc JMSaberThink 28 4
export JMSaberTouch
proc JMSaberTouch 36 12
line 199
;197:
;198:void JMSaberTouch(gentity_t *self, gentity_t *other, trace_t *trace)
;199:{
line 200
;200:	int i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 203
;201://	gentity_t *te;
;202:
;203:	if (!other || !other->client || other->health < 1)
ADDRLP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 8
CNSTU4 0
ASGNU4
ADDRLP4 4
INDIRP4
CVPU4 4
ADDRLP4 8
INDIRU4
EQU4 $132
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 8
INDIRU4
EQU4 $132
ADDRLP4 4
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 1
GEI4 $129
LABELV $132
line 204
;204:	{
line 205
;205:		return;
ADDRGP4 $128
JUMPV
LABELV $129
line 208
;206:	}
;207:
;208:	if (self->enemy)
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $133
line 209
;209:	{
line 210
;210:		return;
ADDRGP4 $128
JUMPV
LABELV $133
line 213
;211:	}
;212:
;213:	if (!self->s.modelindex)
ADDRFP4 0
INDIRP4
CNSTI4 212
ADDP4
INDIRI4
CNSTI4 0
NEI4 $135
line 214
;214:	{
line 215
;215:		return;
ADDRGP4 $128
JUMPV
LABELV $135
line 218
;216:	}
;217:
;218:	if (other->client->ps.stats[STAT_WEAPONS] & (1 << WP_SABER))
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 232
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $137
line 219
;219:	{
line 220
;220:		return;
ADDRGP4 $128
JUMPV
LABELV $137
line 223
;221:	}
;222:
;223:	if (other->client->ps.isJediMaster)
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 604
ADDP4
INDIRI4
CNSTI4 0
EQI4 $139
line 224
;224:	{
line 225
;225:		return;
ADDRGP4 $128
JUMPV
LABELV $139
line 228
;226:	}
;227:
;228:	self->enemy = other;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRFP4 4
INDIRP4
ASGNP4
line 229
;229:	other->client->ps.stats[STAT_WEAPONS] = (1 << WP_SABER);
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 232
ADDP4
CNSTI4 4
ASGNI4
line 230
;230:	other->client->ps.weapon = WP_SABER;
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 2
ASGNI4
line 231
;231:	other->s.weapon = WP_SABER;
ADDRFP4 4
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 2
ASGNI4
line 232
;232:	G_AddEvent(other, EV_BECOME_JEDIMASTER, 0);
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 30
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 235
;233:
;234:	// Track the jedi master 
;235:	trap_SetConfigstring ( CS_CLIENT_JEDIMASTER, va("%i", other->s.number ) );
ADDRGP4 $141
ARGP4
ADDRFP4 4
INDIRP4
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 28
ARGI4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 237
;236:
;237:	if (g_spawnInvulnerability.integer)
ADDRGP4 g_spawnInvulnerability+12
INDIRI4
CNSTI4 0
EQI4 $142
line 238
;238:	{
line 239
;239:		other->client->ps.eFlags |= EF_INVULNERABLE;
ADDRLP4 16
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 67108864
BORI4
ASGNI4
line 240
;240:		other->client->invulnerableTimer = level.time + g_spawnInvulnerability.integer;
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1564
ADDP4
ADDRGP4 level+32
INDIRI4
ADDRGP4 g_spawnInvulnerability+12
INDIRI4
ADDI4
ASGNI4
line 241
;241:	}
LABELV $142
line 243
;242:
;243:	trap_SendServerCommand( -1, va("cp \"%s %s\n\"", other->client->pers.netname, G_GetStripEdString("SVINGAME", "BECOMEJM")) );
ADDRGP4 $148
ARGP4
ADDRGP4 $149
ARGP4
ADDRLP4 16
ADDRGP4 G_GetStripEdString
CALLP4
ASGNP4
ADDRGP4 $147
ARGP4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1416
ADDP4
ARGP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 20
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 245
;244:
;245:	other->client->ps.isJediMaster = qtrue;
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 604
ADDP4
CNSTI4 1
ASGNI4
line 246
;246:	other->client->ps.saberIndex = self->s.number;
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 608
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 248
;247:
;248:	if (other->health < 200 && other->health > 0)
ADDRLP4 24
ADDRFP4 4
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 200
GEI4 $150
ADDRLP4 24
INDIRI4
CNSTI4 0
LEI4 $150
line 249
;249:	{ //full health when you become the Jedi Master
line 250
;250:		other->client->ps.stats[STAT_HEALTH] = other->health = 200;
ADDRLP4 28
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 32
CNSTI4 200
ASGNI4
ADDRLP4 28
INDIRP4
CNSTI4 676
ADDP4
ADDRLP4 32
INDIRI4
ASGNI4
ADDRLP4 28
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 216
ADDP4
ADDRLP4 32
INDIRI4
ASGNI4
line 251
;251:	}
LABELV $150
line 253
;252:
;253:	if (other->client->ps.fd.forcePower < 100)
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 920
ADDP4
INDIRI4
CNSTI4 100
GEI4 $155
line 254
;254:	{
line 255
;255:		other->client->ps.fd.forcePower = 100;
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 920
ADDP4
CNSTI4 100
ASGNI4
line 256
;256:	}
ADDRGP4 $155
JUMPV
LABELV $154
line 259
;257:
;258:	while (i < NUM_FORCE_POWERS)
;259:	{
line 260
;260:		other->client->ps.fd.forcePowersKnown |= (1 << i);
ADDRLP4 28
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 832
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
CNSTI4 1
ADDRLP4 0
INDIRI4
LSHI4
BORI4
ASGNI4
line 261
;261:		other->client->ps.fd.forcePowerLevel[i] = FORCE_LEVEL_3;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 932
ADDP4
ADDP4
CNSTI4 3
ASGNI4
line 263
;262:
;263:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 264
;264:	}
LABELV $155
line 258
ADDRLP4 0
INDIRI4
CNSTI4 18
LTI4 $154
line 266
;265:
;266:	self->pos2[0] = 1;
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
CNSTF4 1065353216
ASGNF4
line 267
;267:	self->pos2[1] = level.time + JMSABER_RESPAWN_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 564
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 20000
ADDI4
CVIF4 4
ASGNF4
line 269
;268:
;269:	self->s.modelindex = 0;
ADDRFP4 0
INDIRP4
CNSTI4 212
ADDP4
CNSTI4 0
ASGNI4
line 270
;270:	self->s.eFlags |= EF_NODRAW;
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
CNSTI4 128
BORI4
ASGNI4
line 271
;271:	self->s.modelGhoul2 = 0;
ADDRFP4 0
INDIRP4
CNSTI4 204
ADDP4
CNSTI4 0
ASGNI4
line 272
;272:	self->s.eType = ET_GENERAL;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 279
;273:
;274:	/*
;275:	te = G_TempEntity( vec3_origin, EV_DESTROY_GHOUL2_INSTANCE );
;276:	te->r.svFlags |= SVF_BROADCAST;
;277:	te->s.eventParm = self->s.number;
;278:	*/
;279:	G_KillG2Queue(self->s.number);
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
ADDRGP4 G_KillG2Queue
CALLV
pop
line 281
;280:
;281:	return;
LABELV $128
endproc JMSaberTouch 36 12
data
export gJMSaberEnt
align 4
LABELV gJMSaberEnt
byte 4 0
export SP_info_jedimaster_start
code
proc SP_info_jedimaster_start 8 4
line 290
;282:}
;283:
;284:gentity_t *gJMSaberEnt = NULL;
;285:
;286:/*QUAKED info_jedimaster_start (1 0 0) (-16 -16 -24) (16 16 32)
;287:"jedi master" saber spawn point
;288:*/
;289:void SP_info_jedimaster_start(gentity_t *ent)
;290:{
line 291
;291:	if (g_gametype.integer != GT_JEDIMASTER)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 2
EQI4 $159
line 292
;292:	{
line 293
;293:		gJMSaberEnt = NULL;
ADDRGP4 gJMSaberEnt
CNSTP4 0
ASGNP4
line 294
;294:		G_FreeEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 295
;295:		return;
ADDRGP4 $158
JUMPV
LABELV $159
line 298
;296:	}
;297:
;298:	ent->enemy = NULL;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
CNSTP4 0
ASGNP4
line 300
;299:
;300:	ent->s.eFlags = EF_BOUNCE_HALF;
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 32
ASGNI4
line 302
;301:
;302:	ent->s.modelindex = G_ModelIndex("models/weapons2/saber/saber_w.glm");
ADDRGP4 $108
ARGP4
ADDRLP4 0
ADDRGP4 G_ModelIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 212
ADDP4
ADDRLP4 0
INDIRI4
ASGNI4
line 303
;303:	ent->s.modelGhoul2 = 1;
ADDRFP4 0
INDIRP4
CNSTI4 204
ADDP4
CNSTI4 1
ASGNI4
line 304
;304:	ent->s.g2radius = 20;
ADDRFP4 0
INDIRP4
CNSTI4 208
ADDP4
CNSTI4 20
ASGNI4
line 306
;305:	//ent->s.eType = ET_GENERAL;
;306:	ent->s.eType = ET_MISSILE;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 3
ASGNI4
line 307
;307:	ent->s.weapon = WP_SABER;
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 2
ASGNI4
line 308
;308:	ent->s.pos.trType = TR_GRAVITY;
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 5
ASGNI4
line 309
;309:	ent->s.pos.trTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 310
;310:	VectorSet( ent->r.maxs, 3, 3, 3 );
ADDRFP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1077936128
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1077936128
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1077936128
ASGNF4
line 311
;311:	VectorSet( ent->r.mins, -3, -3, -3 );
ADDRFP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3225419776
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3225419776
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3225419776
ASGNF4
line 312
;312:	ent->r.contents = CONTENTS_TRIGGER;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1024
ASGNI4
line 313
;313:	ent->clipmask = MASK_SOLID;
ADDRFP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 1
ASGNI4
line 315
;314:
;315:	ent->isSaberEntity = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 812
ADDP4
CNSTI4 1
ASGNI4
line 317
;316:
;317:	ent->bounceCount = -5;
ADDRFP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 -5
ASGNI4
line 319
;318:
;319:	ent->physicsObject = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 500
ADDP4
CNSTI4 1
ASGNI4
line 321
;320:
;321:	VectorCopy(ent->s.pos.trBase, ent->s.origin2); //remember the spawn spot
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 323
;322:
;323:	ent->touch = JMSaberTouch;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 JMSaberTouch
ASGNP4
line 325
;324:
;325:	trap_LinkEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 327
;326:
;327:	ent->think = JMSaberThink;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 JMSaberThink
ASGNP4
line 328
;328:	ent->nextthink = level.time + 50;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 50
ADDI4
ASGNI4
line 329
;329:}
LABELV $158
endproc SP_info_jedimaster_start 8 4
export SpotWouldTelefrag
proc SpotWouldTelefrag 4144 16
line 345
;330:
;331:/*
;332:=======================================================================
;333:
;334:  SelectSpawnPoint
;335:
;336:=======================================================================
;337:*/
;338:
;339:/*
;340:================
;341:SpotWouldTelefrag
;342:
;343:================
;344:*/
;345:qboolean SpotWouldTelefrag( gentity_t *spot ) {
line 351
;346:	int			i, num;
;347:	int			touch[MAX_GENTITIES];
;348:	gentity_t	*hit;
;349:	vec3_t		mins, maxs;
;350:
;351:	VectorAdd( spot->s.origin, playerMins, mins );
ADDRLP4 4132
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4108
ADDRLP4 4132
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRGP4 playerMins
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4108+4
ADDRLP4 4132
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRGP4 playerMins+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4108+8
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRGP4 playerMins+8
INDIRF4
ADDF4
ASGNF4
line 352
;352:	VectorAdd( spot->s.origin, playerMaxs, maxs );
ADDRLP4 4136
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4120
ADDRLP4 4136
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRGP4 playerMaxs
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4120+4
ADDRLP4 4136
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRGP4 playerMaxs+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4120+8
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRGP4 playerMaxs+8
INDIRF4
ADDF4
ASGNF4
line 353
;353:	num = trap_EntitiesInBox( mins, maxs, touch, MAX_GENTITIES );
ADDRLP4 4108
ARGP4
ADDRLP4 4120
ARGP4
ADDRLP4 12
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 4140
ADDRGP4 trap_EntitiesInBox
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 4140
INDIRI4
ASGNI4
line 355
;354:
;355:	for (i=0 ; i<num ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $176
JUMPV
LABELV $173
line 356
;356:		hit = &g_entities[touch[i]];
ADDRLP4 4
CNSTI4 828
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 12
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 358
;357:		//if ( hit->client && hit->client->ps.stats[STAT_HEALTH] > 0 ) {
;358:		if ( hit->client) {
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $177
line 359
;359:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $164
JUMPV
LABELV $177
line 362
;360:		}
;361:
;362:	}
LABELV $174
line 355
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $176
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $173
line 364
;363:
;364:	return qfalse;
CNSTI4 0
RETI4
LABELV $164
endproc SpotWouldTelefrag 4144 16
export SelectNearestDeathmatchSpawnPoint
proc SelectNearestDeathmatchSpawnPoint 40 12
line 375
;365:}
;366:
;367:/*
;368:================
;369:SelectNearestDeathmatchSpawnPoint
;370:
;371:Find the spot that we DON'T want to use
;372:================
;373:*/
;374:#define	MAX_SPAWN_POINTS	128
;375:gentity_t *SelectNearestDeathmatchSpawnPoint( vec3_t from ) {
line 381
;376:	gentity_t	*spot;
;377:	vec3_t		delta;
;378:	float		dist, nearestDist;
;379:	gentity_t	*nearestSpot;
;380:
;381:	nearestDist = 999999;
ADDRLP4 20
CNSTF4 1232348144
ASGNF4
line 382
;382:	nearestSpot = NULL;
ADDRLP4 24
CNSTP4 0
ASGNP4
line 383
;383:	spot = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
ADDRGP4 $181
JUMPV
LABELV $180
line 385
;384:
;385:	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
line 387
;386:
;387:		VectorSubtract( spot->s.origin, from, delta );
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRLP4 32
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 0
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRLP4 32
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+8
ADDRLP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
SUBF4
ASGNF4
line 388
;388:		dist = VectorLength( delta );
ADDRLP4 4
ARGP4
ADDRLP4 36
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 16
ADDRLP4 36
INDIRF4
ASGNF4
line 389
;389:		if ( dist < nearestDist ) {
ADDRLP4 16
INDIRF4
ADDRLP4 20
INDIRF4
GEF4 $185
line 390
;390:			nearestDist = dist;
ADDRLP4 20
ADDRLP4 16
INDIRF4
ASGNF4
line 391
;391:			nearestSpot = spot;
ADDRLP4 24
ADDRLP4 0
INDIRP4
ASGNP4
line 392
;392:		}
LABELV $185
line 393
;393:	}
LABELV $181
line 385
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 416
ARGI4
ADDRGP4 $87
ARGP4
ADDRLP4 28
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 28
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $180
line 395
;394:
;395:	return nearestSpot;
ADDRLP4 24
INDIRP4
RETP4
LABELV $179
endproc SelectNearestDeathmatchSpawnPoint 40 12
export SelectRandomDeathmatchSpawnPoint
proc SelectRandomDeathmatchSpawnPoint 532 12
line 407
;396:}
;397:
;398:
;399:/*
;400:================
;401:SelectRandomDeathmatchSpawnPoint
;402:
;403:go to a random point that doesn't telefrag
;404:================
;405:*/
;406:#define	MAX_SPAWN_POINTS	128
;407:gentity_t *SelectRandomDeathmatchSpawnPoint( void ) {
line 413
;408:	gentity_t	*spot;
;409:	int			count;
;410:	int			selection;
;411:	gentity_t	*spots[MAX_SPAWN_POINTS];
;412:
;413:	count = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 414
;414:	spot = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
ADDRGP4 $189
JUMPV
LABELV $188
line 416
;415:
;416:	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
line 417
;417:		if ( SpotWouldTelefrag( spot ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 524
ADDRGP4 SpotWouldTelefrag
CALLI4
ASGNI4
ADDRLP4 524
INDIRI4
CNSTI4 0
EQI4 $191
line 418
;418:			continue;
ADDRGP4 $189
JUMPV
LABELV $191
line 420
;419:		}
;420:		spots[ count ] = spot;
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 421
;421:		count++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 422
;422:	}
LABELV $189
line 416
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 416
ARGI4
ADDRGP4 $87
ARGP4
ADDRLP4 524
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 524
INDIRP4
ASGNP4
ADDRLP4 524
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $188
line 424
;423:
;424:	if ( !count ) {	// no spots that won't telefrag
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $193
line 425
;425:		return G_Find( NULL, FOFS(classname), "info_player_deathmatch");
CNSTP4 0
ARGP4
CNSTI4 416
ARGI4
ADDRGP4 $87
ARGP4
ADDRLP4 528
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 528
INDIRP4
RETP4
ADDRGP4 $187
JUMPV
LABELV $193
line 428
;426:	}
;427:
;428:	selection = rand() % count;
ADDRLP4 528
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 520
ADDRLP4 528
INDIRI4
ADDRLP4 4
INDIRI4
MODI4
ASGNI4
line 429
;429:	return spots[ selection ];
ADDRLP4 520
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRP4
RETP4
LABELV $187
endproc SelectRandomDeathmatchSpawnPoint 532 12
export SelectRandomFurthestSpawnPoint
proc SelectRandomFurthestSpawnPoint 572 12
line 439
;430:}
;431:
;432:/*
;433:===========
;434:SelectRandomFurthestSpawnPoint
;435:
;436:Chooses a player start, deathmatch start, etc
;437:============
;438:*/
;439:gentity_t *SelectRandomFurthestSpawnPoint ( vec3_t avoidPoint, vec3_t origin, vec3_t angles ) {
line 447
;440:	gentity_t	*spot;
;441:	vec3_t		delta;
;442:	float		dist;
;443:	float		list_dist[64];
;444:	gentity_t	*list_spot[64];
;445:	int			numSpots, rnd, i, j;
;446:
;447:	numSpots = 0;
ADDRLP4 520
CNSTI4 0
ASGNI4
line 448
;448:	spot = NULL;
ADDRLP4 528
CNSTP4 0
ASGNP4
ADDRGP4 $197
JUMPV
LABELV $196
line 450
;449:
;450:	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
line 451
;451:		if ( SpotWouldTelefrag( spot ) ) {
ADDRLP4 528
INDIRP4
ARGP4
ADDRLP4 548
ADDRGP4 SpotWouldTelefrag
CALLI4
ASGNI4
ADDRLP4 548
INDIRI4
CNSTI4 0
EQI4 $199
line 452
;452:			continue;
ADDRGP4 $197
JUMPV
LABELV $199
line 454
;453:		}
;454:		VectorSubtract( spot->s.origin, avoidPoint, delta );
ADDRLP4 556
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 532
ADDRLP4 528
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRLP4 556
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 532+4
ADDRLP4 528
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRLP4 556
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 532+8
ADDRLP4 528
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
SUBF4
ASGNF4
line 455
;455:		dist = VectorLength( delta );
ADDRLP4 532
ARGP4
ADDRLP4 560
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 524
ADDRLP4 560
INDIRF4
ASGNF4
line 456
;456:		for (i = 0; i < numSpots; i++) {
ADDRLP4 516
CNSTI4 0
ASGNI4
ADDRGP4 $206
JUMPV
LABELV $203
line 457
;457:			if ( dist > list_dist[i] ) {
ADDRLP4 524
INDIRF4
ADDRLP4 516
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
INDIRF4
LEF4 $207
line 458
;458:				if ( numSpots >= 64 )
ADDRLP4 520
INDIRI4
CNSTI4 64
LTI4 $209
line 459
;459:					numSpots = 64-1;
ADDRLP4 520
CNSTI4 63
ASGNI4
LABELV $209
line 460
;460:				for (j = numSpots; j > i; j--) {
ADDRLP4 0
ADDRLP4 520
INDIRI4
ASGNI4
ADDRGP4 $214
JUMPV
LABELV $211
line 461
;461:					list_dist[j] = list_dist[j-1];
ADDRLP4 564
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 564
INDIRI4
ADDRLP4 4
ADDP4
ADDRLP4 564
INDIRI4
ADDRLP4 4-4
ADDP4
INDIRF4
ASGNF4
line 462
;462:					list_spot[j] = list_spot[j-1];
ADDRLP4 568
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 568
INDIRI4
ADDRLP4 260
ADDP4
ADDRLP4 568
INDIRI4
ADDRLP4 260-4
ADDP4
INDIRP4
ASGNP4
line 463
;463:				}
LABELV $212
line 460
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
LABELV $214
ADDRLP4 0
INDIRI4
ADDRLP4 516
INDIRI4
GTI4 $211
line 464
;464:				list_dist[i] = dist;
ADDRLP4 516
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
ADDRLP4 524
INDIRF4
ASGNF4
line 465
;465:				list_spot[i] = spot;
ADDRLP4 516
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 260
ADDP4
ADDRLP4 528
INDIRP4
ASGNP4
line 466
;466:				numSpots++;
ADDRLP4 520
ADDRLP4 520
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 467
;467:				if (numSpots > 64)
ADDRLP4 520
INDIRI4
CNSTI4 64
LEI4 $205
line 468
;468:					numSpots = 64;
ADDRLP4 520
CNSTI4 64
ASGNI4
line 469
;469:				break;
ADDRGP4 $205
JUMPV
LABELV $207
line 471
;470:			}
;471:		}
LABELV $204
line 456
ADDRLP4 516
ADDRLP4 516
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $206
ADDRLP4 516
INDIRI4
ADDRLP4 520
INDIRI4
LTI4 $203
LABELV $205
line 472
;472:		if (i >= numSpots && numSpots < 64) {
ADDRLP4 516
INDIRI4
ADDRLP4 520
INDIRI4
LTI4 $219
ADDRLP4 520
INDIRI4
CNSTI4 64
GEI4 $219
line 473
;473:			list_dist[numSpots] = dist;
ADDRLP4 520
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
ADDRLP4 524
INDIRF4
ASGNF4
line 474
;474:			list_spot[numSpots] = spot;
ADDRLP4 520
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 260
ADDP4
ADDRLP4 528
INDIRP4
ASGNP4
line 475
;475:			numSpots++;
ADDRLP4 520
ADDRLP4 520
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 476
;476:		}
LABELV $219
line 477
;477:	}
LABELV $197
line 450
ADDRLP4 528
INDIRP4
ARGP4
CNSTI4 416
ARGI4
ADDRGP4 $87
ARGP4
ADDRLP4 548
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 528
ADDRLP4 548
INDIRP4
ASGNP4
ADDRLP4 548
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $196
line 478
;478:	if (!numSpots) {
ADDRLP4 520
INDIRI4
CNSTI4 0
NEI4 $221
line 479
;479:		spot = G_Find( NULL, FOFS(classname), "info_player_deathmatch");
CNSTP4 0
ARGP4
CNSTI4 416
ARGI4
ADDRGP4 $87
ARGP4
ADDRLP4 552
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 528
ADDRLP4 552
INDIRP4
ASGNP4
line 480
;480:		if (!spot)
ADDRLP4 528
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $223
line 481
;481:			G_Error( "Couldn't find a spawn point" );
ADDRGP4 $225
ARGP4
ADDRGP4 G_Error
CALLV
pop
LABELV $223
line 482
;482:		VectorCopy (spot->s.origin, origin);
ADDRFP4 4
INDIRP4
ADDRLP4 528
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 483
;483:		origin[2] += 9;
ADDRLP4 556
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 556
INDIRP4
ADDRLP4 556
INDIRP4
INDIRF4
CNSTF4 1091567616
ADDF4
ASGNF4
line 484
;484:		VectorCopy (spot->s.angles, angles);
ADDRFP4 8
INDIRP4
ADDRLP4 528
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 485
;485:		return spot;
ADDRLP4 528
INDIRP4
RETP4
ADDRGP4 $195
JUMPV
LABELV $221
line 489
;486:	}
;487:
;488:	// select a random spot from the spawn points furthest away
;489:	rnd = random() * (numSpots / 2);
ADDRLP4 552
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 544
ADDRLP4 552
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 520
INDIRI4
CNSTI4 2
DIVI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 491
;490:
;491:	VectorCopy (list_spot[rnd]->s.origin, origin);
ADDRFP4 4
INDIRP4
ADDRLP4 544
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 260
ADDP4
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 492
;492:	origin[2] += 9;
ADDRLP4 556
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 556
INDIRP4
ADDRLP4 556
INDIRP4
INDIRF4
CNSTF4 1091567616
ADDF4
ASGNF4
line 493
;493:	VectorCopy (list_spot[rnd]->s.angles, angles);
ADDRFP4 8
INDIRP4
ADDRLP4 544
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 260
ADDP4
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 495
;494:
;495:	return list_spot[rnd];
ADDRLP4 544
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 260
ADDP4
INDIRP4
RETP4
LABELV $195
endproc SelectRandomFurthestSpawnPoint 572 12
export SelectSpawnPoint
proc SelectSpawnPoint 4 12
line 505
;496:}
;497:
;498:/*
;499:===========
;500:SelectSpawnPoint
;501:
;502:Chooses a player start, deathmatch start, etc
;503:============
;504:*/
;505:gentity_t *SelectSpawnPoint ( vec3_t avoidPoint, vec3_t origin, vec3_t angles ) {
line 506
;506:	return SelectRandomFurthestSpawnPoint( avoidPoint, origin, angles );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 SelectRandomFurthestSpawnPoint
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
RETP4
LABELV $226
endproc SelectSpawnPoint 4 12
export SelectInitialSpawnPoint
proc SelectInitialSpawnPoint 20 12
line 545
;507:
;508:	/*
;509:	gentity_t	*spot;
;510:	gentity_t	*nearestSpot;
;511:
;512:	nearestSpot = SelectNearestDeathmatchSpawnPoint( avoidPoint );
;513:
;514:	spot = SelectRandomDeathmatchSpawnPoint ( );
;515:	if ( spot == nearestSpot ) {
;516:		// roll again if it would be real close to point of death
;517:		spot = SelectRandomDeathmatchSpawnPoint ( );
;518:		if ( spot == nearestSpot ) {
;519:			// last try
;520:			spot = SelectRandomDeathmatchSpawnPoint ( );
;521:		}		
;522:	}
;523:
;524:	// find a single player start spot
;525:	if (!spot) {
;526:		G_Error( "Couldn't find a spawn point" );
;527:	}
;528:
;529:	VectorCopy (spot->s.origin, origin);
;530:	origin[2] += 9;
;531:	VectorCopy (spot->s.angles, angles);
;532:
;533:	return spot;
;534:	*/
;535:}
;536:
;537:/*
;538:===========
;539:SelectInitialSpawnPoint
;540:
;541:Try to find a spawn point marked 'initial', otherwise
;542:use normal spawn selection.
;543:============
;544:*/
;545:gentity_t *SelectInitialSpawnPoint( vec3_t origin, vec3_t angles ) {
line 548
;546:	gentity_t	*spot;
;547:
;548:	spot = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
ADDRGP4 $229
JUMPV
LABELV $228
line 549
;549:	while ((spot = G_Find (spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
line 550
;550:		if ( spot->spawnflags & 1 ) {
ADDRLP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $231
line 551
;551:			break;
ADDRGP4 $230
JUMPV
LABELV $231
line 553
;552:		}
;553:	}
LABELV $229
line 549
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 416
ARGI4
ADDRGP4 $87
ARGP4
ADDRLP4 4
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $228
LABELV $230
line 555
;554:
;555:	if ( !spot || SpotWouldTelefrag( spot ) ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $235
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 SpotWouldTelefrag
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $233
LABELV $235
line 556
;556:		return SelectSpawnPoint( vec3_origin, origin, angles );
ADDRGP4 vec3_origin
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 SelectSpawnPoint
CALLP4
ASGNP4
ADDRLP4 16
INDIRP4
RETP4
ADDRGP4 $227
JUMPV
LABELV $233
line 559
;557:	}
;558:
;559:	VectorCopy (spot->s.origin, origin);
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 560
;560:	origin[2] += 9;
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRF4
CNSTF4 1091567616
ADDF4
ASGNF4
line 561
;561:	VectorCopy (spot->s.angles, angles);
ADDRFP4 4
INDIRP4
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 563
;562:
;563:	return spot;
ADDRLP4 0
INDIRP4
RETP4
LABELV $227
endproc SelectInitialSpawnPoint 20 12
export SelectSpectatorSpawnPoint
proc SelectSpectatorSpawnPoint 0 0
line 572
;564:}
;565:
;566:/*
;567:===========
;568:SelectSpectatorSpawnPoint
;569:
;570:============
;571:*/
;572:gentity_t *SelectSpectatorSpawnPoint( vec3_t origin, vec3_t angles ) {
line 573
;573:	FindIntermissionPoint();
ADDRGP4 FindIntermissionPoint
CALLV
pop
line 575
;574:
;575:	VectorCopy( level.intermission_origin, origin );
ADDRFP4 0
INDIRP4
ADDRGP4 level+9024
INDIRB
ASGNB 12
line 576
;576:	VectorCopy( level.intermission_angle, angles );
ADDRFP4 4
INDIRP4
ADDRGP4 level+9036
INDIRB
ASGNB 12
line 578
;577:
;578:	return NULL;
CNSTP4 0
RETP4
LABELV $236
endproc SelectSpectatorSpawnPoint 0 0
export InitBodyQue
proc InitBodyQue 12 0
line 604
;579:}
;580:
;581:/*
;582:=======================================================================
;583:
;584:BODYQUE
;585:
;586:=======================================================================
;587:*/
;588:
;589:/*
;590:=======================================================================
;591:
;592:BODYQUE
;593:
;594:=======================================================================
;595:*/
;596:
;597:#define BODY_SINK_TIME		45000
;598:
;599:/*
;600:===============
;601:InitBodyQue
;602:===============
;603:*/
;604:void InitBodyQue (void) {
line 608
;605:	int		i;
;606:	gentity_t	*ent;
;607:
;608:	level.bodyQueIndex = 0;
ADDRGP4 level+9056
CNSTI4 0
ASGNI4
line 609
;609:	for (i=0; i<BODY_QUEUE_SIZE ; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $241
line 610
;610:		ent = G_Spawn();
ADDRLP4 8
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 611
;611:		ent->classname = "bodyque";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $245
ASGNP4
line 612
;612:		ent->neverFree = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 468
ADDP4
CNSTI4 1
ASGNI4
line 613
;613:		level.bodyQue[i] = ent;
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 level+9060
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 614
;614:	}
LABELV $242
line 609
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 8
LTI4 $241
line 615
;615:}
LABELV $239
endproc InitBodyQue 12 0
export BodySink
proc BodySink 4 4
line 624
;616:
;617:/*
;618:=============
;619:BodySink
;620:
;621:After sitting around for five seconds, fall into the ground and dissapear
;622:=============
;623:*/
;624:void BodySink( gentity_t *ent ) {
line 625
;625:	if ( level.time - ent->timestamp > BODY_SINK_TIME + 1500 ) {
ADDRGP4 level+32
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 576
ADDP4
INDIRI4
SUBI4
CNSTI4 46500
LEI4 $248
line 627
;626:		// the body ques are never actually freed, they are just unlinked
;627:		trap_UnlinkEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 628
;628:		ent->physicsObject = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 500
ADDP4
CNSTI4 0
ASGNI4
line 629
;629:		return;	
ADDRGP4 $247
JUMPV
LABELV $248
line 631
;630:	}
;631:	ent->nextthink = level.time + 100;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 632
;632:	ent->s.pos.trBase[2] -= 1;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRF4
CNSTF4 1065353216
SUBF4
ASGNF4
line 633
;633:}
LABELV $247
endproc BodySink 4 4
export CopyToBodyQue
proc CopyToBodyQue 64 12
line 643
;634:
;635:/*
;636:=============
;637:CopyToBodyQue
;638:
;639:A player is respawning, so make an entity that looks
;640:just like the existing corpse to leave behind.
;641:=============
;642:*/
;643:void CopyToBodyQue( gentity_t *ent ) {
line 647
;644:	gentity_t		*body;
;645:	int			contents;
;646:
;647:	if (level.intermissiontime)
ADDRGP4 level+9008
INDIRI4
CNSTI4 0
EQI4 $253
line 648
;648:	{
line 649
;649:		return;
ADDRGP4 $252
JUMPV
LABELV $253
line 652
;650:	}
;651:
;652:	trap_UnlinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 655
;653:
;654:	// if client is in a nodrop area, don't leave the body
;655:	contents = trap_PointContents( ent->s.origin, -1 );
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
CNSTI4 -1
ARGI4
ADDRLP4 8
ADDRGP4 trap_PointContents
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 8
INDIRI4
ASGNI4
line 656
;656:	if ( contents & CONTENTS_NODROP ) {
ADDRLP4 4
INDIRI4
CNSTI4 2048
BANDI4
CNSTI4 0
EQI4 $256
line 657
;657:		return;
ADDRGP4 $252
JUMPV
LABELV $256
line 660
;658:	}
;659:
;660:	if (ent->client && (ent->client->ps.eFlags & EF_DISINTEGRATION))
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $258
ADDRLP4 12
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
CNSTI4 33554432
BANDI4
CNSTI4 0
EQI4 $258
line 661
;661:	{ //for now, just don't spawn a body if you got disint'd
line 662
;662:		return;
ADDRGP4 $252
JUMPV
LABELV $258
line 666
;663:	}
;664:
;665:	// grab a body que and cycle to the next one
;666:	body = level.bodyQue[ level.bodyQueIndex ];
ADDRLP4 0
ADDRGP4 level+9056
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 level+9060
ADDP4
INDIRP4
ASGNP4
line 667
;667:	level.bodyQueIndex = (level.bodyQueIndex + 1) % BODY_QUEUE_SIZE;
ADDRGP4 level+9056
ADDRGP4 level+9056
INDIRI4
CNSTI4 1
ADDI4
CNSTI4 8
MODI4
ASGNI4
line 669
;668:
;669:	trap_UnlinkEntity (body);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 670
;670:	body->s = ent->s;
ADDRLP4 0
INDIRP4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 296
line 673
;671:
;672:	//avoid oddly angled corpses floating around
;673:	body->s.angles[PITCH] = body->s.angles[ROLL] = body->s.apos.trBase[PITCH] = body->s.apos.trBase[ROLL] = 0;
ADDRLP4 20
CNSTF4 0
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 124
ADDP4
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ADDRLP4 20
INDIRF4
ASGNF4
line 675
;674:
;675:	body->s.g2radius = 100;
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
CNSTI4 100
ASGNI4
line 677
;676:
;677:	body->s.eType = ET_BODY;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 15
ASGNI4
line 678
;678:	body->s.eFlags = EF_DEAD;		// clear EF_TALK, etc
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 1
ASGNI4
line 680
;679:
;680:	if (ent->client && (ent->client->ps.eFlags & EF_DISINTEGRATION))
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $264
ADDRLP4 24
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
CNSTI4 33554432
BANDI4
CNSTI4 0
EQI4 $264
line 681
;681:	{
line 682
;682:		body->s.eFlags |= EF_DISINTEGRATION;
ADDRLP4 28
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
CNSTI4 33554432
BORI4
ASGNI4
line 683
;683:	}
LABELV $264
line 685
;684:
;685:	VectorCopy(ent->client->ps.lastHitLoc, body->s.origin2);
ADDRLP4 0
INDIRP4
CNSTI4 104
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1356
ADDP4
INDIRB
ASGNB 12
line 687
;686:
;687:	body->s.powerups = 0;	// clear powerups
ADDRLP4 0
INDIRP4
CNSTI4 272
ADDP4
CNSTI4 0
ASGNI4
line 688
;688:	body->s.loopSound = 0;	// clear lava burning
ADDRLP4 0
INDIRP4
CNSTI4 200
ADDP4
CNSTI4 0
ASGNI4
line 689
;689:	body->s.number = body - g_entities;
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRGP4 g_entities
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 828
DIVI4
ASGNI4
line 690
;690:	body->timestamp = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 576
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 691
;691:	body->physicsObject = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 500
ADDP4
CNSTI4 1
ASGNI4
line 692
;692:	body->physicsBounce = 0;		// don't bounce
ADDRLP4 0
INDIRP4
CNSTI4 504
ADDP4
CNSTF4 0
ASGNF4
line 693
;693:	if ( body->s.groundEntityNum == ENTITYNUM_NONE ) {
ADDRLP4 0
INDIRP4
CNSTI4 192
ADDP4
INDIRI4
CNSTI4 1023
NEI4 $267
line 694
;694:		body->s.pos.trType = TR_GRAVITY;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 5
ASGNI4
line 695
;695:		body->s.pos.trTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 696
;696:		VectorCopy( ent->client->ps.velocity, body->s.pos.trDelta );
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRB
ASGNB 12
line 697
;697:	} else {
ADDRGP4 $268
JUMPV
LABELV $267
line 698
;698:		body->s.pos.trType = TR_STATIONARY;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 699
;699:	}
LABELV $268
line 700
;700:	body->s.event = 0;
ADDRLP4 0
INDIRP4
CNSTI4 252
ADDP4
CNSTI4 0
ASGNI4
line 702
;701:
;702:	body->s.weapon = ent->s.bolt2;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ASGNI4
line 704
;703:
;704:	if (body->s.weapon == WP_SABER && ent->client->ps.saberInFlight)
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 2
NEI4 $270
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 504
ADDP4
INDIRI4
CNSTI4 0
EQI4 $270
line 705
;705:	{
line 706
;706:		body->s.weapon = WP_BLASTER; //lie to keep from putting a saber on the corpse, because it was thrown at death
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 4
ASGNI4
line 707
;707:	}
LABELV $270
line 709
;708:
;709:	G_AddEvent(body, EV_BODY_QUEUE_COPY, ent->s.clientNum);
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 109
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 220
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 711
;710:
;711:	body->r.svFlags = ent->r.svFlags | SVF_BROADCAST;
ADDRLP4 32
CNSTI4 304
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 712
;712:	VectorCopy (ent->r.mins, body->r.mins);
ADDRLP4 36
CNSTI4 316
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 713
;713:	VectorCopy (ent->r.maxs, body->r.maxs);
ADDRLP4 40
CNSTI4 328
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 714
;714:	VectorCopy (ent->r.absmin, body->r.absmin);
ADDRLP4 44
CNSTI4 344
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 715
;715:	VectorCopy (ent->r.absmax, body->r.absmax);
ADDRLP4 48
CNSTI4 356
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRB
ASGNB 12
line 717
;716:
;717:	body->s.torsoAnim = body->s.legsAnim = ent->client->ps.legsAnim & ~ANIM_TOGGLEBIT;
ADDRLP4 56
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 92
ADDP4
INDIRI4
CNSTI4 -2049
BANDI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 280
ADDP4
ADDRLP4 56
INDIRI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 284
ADDP4
ADDRLP4 56
INDIRI4
ASGNI4
line 719
;718:
;719:	body->clipmask = CONTENTS_SOLID | CONTENTS_PLAYERCLIP;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 17
ASGNI4
line 720
;720:	body->r.contents = CONTENTS_CORPSE;
ADDRLP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 512
ASGNI4
line 721
;721:	body->r.ownerNum = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 396
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 723
;722:
;723:	body->nextthink = level.time + BODY_SINK_TIME;
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 45000
ADDI4
ASGNI4
line 724
;724:	body->think = BodySink;
ADDRLP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 BodySink
ASGNP4
line 726
;725:
;726:	body->die = body_die;
ADDRLP4 0
INDIRP4
CNSTI4 660
ADDP4
ADDRGP4 body_die
ASGNP4
line 729
;727:
;728:	// don't take more damage if already gibbed
;729:	if ( ent->health <= GIB_HEALTH ) {
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 -40
GTI4 $273
line 730
;730:		body->takedamage = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 0
ASGNI4
line 731
;731:	} else {
ADDRGP4 $274
JUMPV
LABELV $273
line 732
;732:		body->takedamage = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 1
ASGNI4
line 733
;733:	}
LABELV $274
line 735
;734:
;735:	VectorCopy ( body->s.pos.trBase, body->r.currentOrigin );
ADDRLP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 736
;736:	trap_LinkEntity (body);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 737
;737:}
LABELV $252
endproc CopyToBodyQue 64 12
export SetClientViewAngle
proc SetClientViewAngle 16 0
line 748
;738:
;739://======================================================================
;740:
;741:
;742:/*
;743:==================
;744:SetClientViewAngle
;745:
;746:==================
;747:*/
;748:void SetClientViewAngle( gentity_t *ent, vec3_t angle ) {
line 752
;749:	int			i;
;750:
;751:	// set the delta angle
;752:	for (i=0 ; i<3 ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $276
line 755
;753:		int		cmdAngle;
;754:
;755:		cmdAngle = ANGLE2SHORT(angle[i]);
ADDRLP4 4
CNSTF4 1199570944
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRF4
MULF4
CNSTF4 1135869952
DIVF4
CVFI4 4
CNSTI4 65535
BANDI4
ASGNI4
line 756
;756:		ent->client->ps.delta_angles[i] = cmdAngle - ent->client->pers.cmd.angles[i];
ADDRLP4 8
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRP4
CNSTI4 68
ADDP4
ADDP4
ADDRLP4 4
INDIRI4
ADDRLP4 8
INDIRI4
ADDRLP4 12
INDIRP4
CNSTI4 1376
ADDP4
ADDP4
INDIRI4
SUBI4
ASGNI4
line 757
;757:	}
LABELV $277
line 752
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $276
line 758
;758:	VectorCopy( angle, ent->s.angles );
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 759
;759:	VectorCopy (ent->s.angles, ent->client->ps.viewangles);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 156
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 760
;760:}
LABELV $275
endproc SetClientViewAngle 16 0
export respawn
proc respawn 12 8
line 767
;761:
;762:/*
;763:================
;764:respawn
;765:================
;766:*/
;767:void respawn( gentity_t *ent ) {
line 770
;768:	gentity_t	*tent;
;769:
;770:	CopyToBodyQue (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 CopyToBodyQue
CALLV
pop
line 772
;771:
;772:	trap_UnlinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 773
;773:	ClientSpawn(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 ClientSpawn
CALLV
pop
line 776
;774:
;775:	// add a teleportation effect
;776:	tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_IN );
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
CNSTI4 59
ARGI4
ADDRLP4 4
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 777
;777:	tent->s.clientNum = ent->s.clientNum;
ADDRLP4 8
CNSTI4 220
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 778
;778:}
LABELV $280
endproc respawn 12 8
export TeamCount
proc TeamCount 8 0
line 787
;779:
;780:/*
;781:================
;782:TeamCount
;783:
;784:Returns number of players on a team
;785:================
;786:*/
;787:team_t TeamCount( int ignoreClientNum, int team ) {
line 789
;788:	int		i;
;789:	int		count = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 791
;790:
;791:	for ( i = 0 ; i < level.maxclients ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $285
JUMPV
LABELV $282
line 792
;792:		if ( i == ignoreClientNum ) {
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $287
line 793
;793:			continue;
ADDRGP4 $283
JUMPV
LABELV $287
line 795
;794:		}
;795:		if ( level.clients[i].pers.connected == CON_DISCONNECTED ) {
CNSTI4 1756
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 1368
ADDP4
INDIRI4
CNSTI4 0
NEI4 $289
line 796
;796:			continue;
ADDRGP4 $283
JUMPV
LABELV $289
line 798
;797:		}
;798:		if ( level.clients[i].sess.sessionTeam == team ) {
CNSTI4 1756
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 1520
ADDP4
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $291
line 799
;799:			count++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 800
;800:		}
LABELV $291
line 801
;801:	}
LABELV $283
line 791
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $285
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $282
line 803
;802:
;803:	return count;
ADDRLP4 4
INDIRI4
RETI4
LABELV $281
endproc TeamCount 8 0
export TeamLeader
proc TeamLeader 4 0
line 813
;804:}
;805:
;806:/*
;807:================
;808:TeamLeader
;809:
;810:Returns the client number of the team leader
;811:================
;812:*/
;813:int TeamLeader( int team ) {
line 816
;814:	int		i;
;815:
;816:	for ( i = 0 ; i < level.maxclients ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $297
JUMPV
LABELV $294
line 817
;817:		if ( level.clients[i].pers.connected == CON_DISCONNECTED ) {
CNSTI4 1756
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 1368
ADDP4
INDIRI4
CNSTI4 0
NEI4 $299
line 818
;818:			continue;
ADDRGP4 $295
JUMPV
LABELV $299
line 820
;819:		}
;820:		if ( level.clients[i].sess.sessionTeam == team ) {
CNSTI4 1756
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 1520
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $301
line 821
;821:			if ( level.clients[i].sess.teamLeader )
CNSTI4 1756
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 1560
ADDP4
INDIRI4
CNSTI4 0
EQI4 $303
line 822
;822:				return i;
ADDRLP4 0
INDIRI4
RETI4
ADDRGP4 $293
JUMPV
LABELV $303
line 823
;823:		}
LABELV $301
line 824
;824:	}
LABELV $295
line 816
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $297
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $294
line 826
;825:
;826:	return -1;
CNSTI4 -1
RETI4
LABELV $293
endproc TeamLeader 4 0
export PickTeam
proc PickTeam 24 8
line 836
;827:}
;828:
;829:
;830:/*
;831:================
;832:PickTeam
;833:
;834:================
;835:*/
;836:team_t PickTeam( int ignoreClientNum ) {
line 839
;837:	int		counts[TEAM_NUM_TEAMS];
;838:
;839:	counts[TEAM_BLUE] = TeamCount( ignoreClientNum, TEAM_BLUE );
ADDRFP4 0
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRLP4 16
ADDRGP4 TeamCount
CALLI4
ASGNI4
ADDRLP4 0+8
ADDRLP4 16
INDIRI4
ASGNI4
line 840
;840:	counts[TEAM_RED] = TeamCount( ignoreClientNum, TEAM_RED );
ADDRFP4 0
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 20
ADDRGP4 TeamCount
CALLI4
ASGNI4
ADDRLP4 0+4
ADDRLP4 20
INDIRI4
ASGNI4
line 842
;841:
;842:	if ( counts[TEAM_BLUE] > counts[TEAM_RED] ) {
ADDRLP4 0+8
INDIRI4
ADDRLP4 0+4
INDIRI4
LEI4 $308
line 843
;843:		return TEAM_RED;
CNSTI4 1
RETI4
ADDRGP4 $305
JUMPV
LABELV $308
line 845
;844:	}
;845:	if ( counts[TEAM_RED] > counts[TEAM_BLUE] ) {
ADDRLP4 0+4
INDIRI4
ADDRLP4 0+8
INDIRI4
LEI4 $312
line 846
;846:		return TEAM_BLUE;
CNSTI4 2
RETI4
ADDRGP4 $305
JUMPV
LABELV $312
line 849
;847:	}
;848:	// equal team count, so join the team with the lowest score
;849:	if ( level.teamScores[TEAM_BLUE] > level.teamScores[TEAM_RED] ) {
ADDRGP4 level+44+8
INDIRI4
ADDRGP4 level+44+4
INDIRI4
LEI4 $316
line 850
;850:		return TEAM_RED;
CNSTI4 1
RETI4
ADDRGP4 $305
JUMPV
LABELV $316
line 852
;851:	}
;852:	return TEAM_BLUE;
CNSTI4 2
RETI4
LABELV $305
endproc PickTeam 24 8
proc ClientCleanName 40 12
line 880
;853:}
;854:
;855:/*
;856:===========
;857:ForceClientSkin
;858:
;859:Forces a client's skin (for teamplay)
;860:===========
;861:*/
;862:/*
;863:static void ForceClientSkin( gclient_t *client, char *model, const char *skin ) {
;864:	char *p;
;865:
;866:	if ((p = Q_strrchr(model, '/')) != 0) {
;867:		*p = 0;
;868:	}
;869:
;870:	Q_strcat(model, MAX_QPATH, "/");
;871:	Q_strcat(model, MAX_QPATH, skin);
;872:}
;873:*/
;874:
;875:/*
;876:===========
;877:ClientCheckName
;878:============
;879:*/
;880:static void ClientCleanName( const char *in, char *out, int outSize ) {
line 887
;881:	int		len, colorlessLen;
;882:	char	ch;
;883:	char	*p;
;884:	int		spaces;
;885:
;886:	//save room for trailing null byte
;887:	outSize--;
ADDRFP4 8
ADDRFP4 8
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 889
;888:
;889:	len = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 890
;890:	colorlessLen = 0;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 891
;891:	p = out;
ADDRLP4 12
ADDRFP4 4
INDIRP4
ASGNP4
line 892
;892:	*p = 0;
ADDRLP4 12
INDIRP4
CNSTI1 0
ASGNI1
line 893
;893:	spaces = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $324
JUMPV
LABELV $323
line 895
;894:
;895:	while( 1 ) {
line 896
;896:		ch = *in++;
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 20
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
INDIRI1
ASGNI1
line 897
;897:		if( !ch ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $326
line 898
;898:			break;
ADDRGP4 $325
JUMPV
LABELV $326
line 902
;899:		}
;900:
;901:		// don't allow leading spaces
;902:		if( !*p && ch == ' ' ) {
ADDRLP4 12
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $328
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 32
NEI4 $328
line 903
;903:			continue;
ADDRGP4 $324
JUMPV
LABELV $328
line 907
;904:		}
;905:
;906:		// check colors
;907:		if( ch == Q_COLOR_ESCAPE ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 94
NEI4 $330
line 909
;908:			// solo trailing carat is not a color prefix
;909:			if( !*in ) {
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $332
line 910
;910:				break;
ADDRGP4 $325
JUMPV
LABELV $332
line 914
;911:			}
;912:
;913:			// don't allow black in a name, period
;914:			if( ColorIndex(*in) == 0 ) {
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 48
SUBI4
CNSTI4 7
BANDI4
CNSTI4 0
NEI4 $334
line 915
;915:				in++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 916
;916:				continue;
ADDRGP4 $324
JUMPV
LABELV $334
line 920
;917:			}
;918:
;919:			// make sure room in dest for both chars
;920:			if( len > outSize - 2 ) {
ADDRLP4 4
INDIRI4
ADDRFP4 8
INDIRI4
CNSTI4 2
SUBI4
LEI4 $336
line 921
;921:				break;
ADDRGP4 $325
JUMPV
LABELV $336
line 924
;922:			}
;923:
;924:			*out++ = ch;
ADDRLP4 24
ADDRFP4 4
INDIRP4
ASGNP4
ADDRFP4 4
ADDRLP4 24
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 0
INDIRI1
ASGNI1
line 925
;925:			*out++ = *in++;
ADDRLP4 28
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 36
CNSTI4 1
ASGNI4
ADDRFP4 4
ADDRLP4 28
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
ASGNP4
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 32
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 32
INDIRP4
INDIRI1
ASGNI1
line 926
;926:			len += 2;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 2
ADDI4
ASGNI4
line 927
;927:			continue;
ADDRGP4 $324
JUMPV
LABELV $330
line 931
;928:		}
;929:
;930:		// don't allow too many consecutive spaces
;931:		if( ch == ' ' ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 32
NEI4 $338
line 932
;932:			spaces++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 933
;933:			if( spaces > 3 ) {
ADDRLP4 8
INDIRI4
CNSTI4 3
LEI4 $339
line 934
;934:				continue;
ADDRGP4 $324
JUMPV
line 936
;935:			}
;936:		}
LABELV $338
line 937
;937:		else {
line 938
;938:			spaces = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 939
;939:		}
LABELV $339
line 941
;940:
;941:		if( len > outSize - 1 ) {
ADDRLP4 4
INDIRI4
ADDRFP4 8
INDIRI4
CNSTI4 1
SUBI4
LEI4 $342
line 942
;942:			break;
ADDRGP4 $325
JUMPV
LABELV $342
line 945
;943:		}
;944:
;945:		*out++ = ch;
ADDRLP4 24
ADDRFP4 4
INDIRP4
ASGNP4
ADDRFP4 4
ADDRLP4 24
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 0
INDIRI1
ASGNI1
line 946
;946:		colorlessLen++;
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 947
;947:		len++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 948
;948:	}
LABELV $324
line 895
ADDRGP4 $323
JUMPV
LABELV $325
line 949
;949:	*out = 0;
ADDRFP4 4
INDIRP4
CNSTI1 0
ASGNI1
line 952
;950:
;951:	// don't allow empty names
;952:	if( *p == 0 || colorlessLen == 0 ) {
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRLP4 12
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 20
INDIRI4
EQI4 $346
ADDRLP4 16
INDIRI4
ADDRLP4 20
INDIRI4
NEI4 $344
LABELV $346
line 953
;953:		Q_strncpyz( p, "Padawan", outSize );
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 $347
ARGP4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 954
;954:	}
LABELV $344
line 955
;955:}
LABELV $322
endproc ClientCleanName 40 12
data
export g2SaberInstance
align 4
LABELV g2SaberInstance
byte 4 0
lit
align 4
LABELV $349
byte 4 0
byte 4 0
byte 4 0
export SetupGameGhoul2Model
code
proc SetupGameGhoul2Model 188 44
line 980
;956:
;957:#ifdef _DEBUG
;958:void G_DebugWrite(const char *path, const char *text)
;959:{
;960:	fileHandle_t f;
;961:
;962:	trap_FS_FOpenFile( path, &f, FS_APPEND );
;963:	trap_FS_Write(text, strlen(text), f);
;964:	trap_FS_FCloseFile(f);
;965:}
;966:#endif
;967:
;968:/*
;969:===========
;970:SetupGameGhoul2Model
;971:
;972:There are two ghoul2 model instances per player (actually three).  One is on the clientinfo (the base for the client side 
;973:player, and copied for player spawns and for corpses).  One is attached to the centity itself, which is the model acutally 
;974:animated and rendered by the system.  The final is the game ghoul2 model.  This is animated by pmove on the server, and
;975:is used for determining where the lightsaber should be, and for per-poly collision tests.
;976:===========
;977:*/
;978:void *g2SaberInstance = NULL;
;979:void SetupGameGhoul2Model(gclient_t *client, char *modelname)
;980:{
line 985
;981:	int handle;
;982:	char		afilename[MAX_QPATH];
;983:	char		/**GLAName,*/ *slash;
;984:	char		GLAName[MAX_QPATH];
;985:	vec3_t	tempVec = {0,0,0};
ADDRLP4 64
ADDRGP4 $349
INDIRB
ASGNB 12
line 988
;986:
;987:	// First things first.  If this is a ghoul2 model, then let's make sure we demolish this first.
;988:	if (client->ghoul2 && trap_G2_HaveWeGhoul2Models(client->ghoul2))
ADDRLP4 148
ADDRFP4 0
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
ASGNP4
ADDRLP4 148
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $350
ADDRLP4 148
INDIRP4
ARGP4
ADDRLP4 152
ADDRGP4 trap_G2_HaveWeGhoul2Models
CALLI4
ASGNI4
ADDRLP4 152
INDIRI4
CNSTI4 0
EQI4 $350
line 989
;989:	{
line 990
;990:		trap_G2API_CleanGhoul2Models(&(client->ghoul2));
ADDRFP4 0
INDIRP4
CNSTI4 1716
ADDP4
ARGP4
ADDRGP4 trap_G2API_CleanGhoul2Models
CALLV
pop
line 991
;991:	}
LABELV $350
line 1009
;992:
;993:	/*
;994:	Com_sprintf( afilename, sizeof( afilename ), "models/players/%s/model.glm", modelname );
;995:	handle = trap_G2API_InitGhoul2Model(&client->ghoul2, afilename, 0, 0, -20, 0, 0);
;996:	if (handle<0)
;997:	{
;998:		Com_sprintf( afilename, sizeof( afilename ), "models/players/kyle/model.glm" );
;999:		handle = trap_G2API_InitGhoul2Model(&client->ghoul2, afilename, 0, 0, -20, 0, 0);
;1000:
;1001:		if (handle<0)
;1002:		{
;1003:			return;
;1004:		}
;1005:	}
;1006:	*/
;1007:
;1008:	//rww - just load the "standard" model for the server"
;1009:	if (!precachedKyle)
ADDRGP4 precachedKyle
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $352
line 1010
;1010:	{
line 1011
;1011:		Com_sprintf( afilename, sizeof( afilename ), "models/players/kyle/model.glm" );
ADDRLP4 0
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $354
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 1012
;1012:		handle = trap_G2API_InitGhoul2Model(&precachedKyle, afilename, 0, 0, -20, 0, 0);
ADDRGP4 precachedKyle
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 156
CNSTI4 0
ASGNI4
ADDRLP4 156
INDIRI4
ARGI4
ADDRLP4 156
INDIRI4
ARGI4
CNSTI4 -20
ARGI4
ADDRLP4 156
INDIRI4
ARGI4
ADDRLP4 156
INDIRI4
ARGI4
ADDRLP4 160
ADDRGP4 trap_G2API_InitGhoul2Model
CALLI4
ASGNI4
ADDRLP4 144
ADDRLP4 160
INDIRI4
ASGNI4
line 1014
;1013:
;1014:		if (handle<0)
ADDRLP4 144
INDIRI4
CNSTI4 0
GEI4 $355
line 1015
;1015:		{
line 1016
;1016:			return;
ADDRGP4 $348
JUMPV
LABELV $355
line 1018
;1017:		}
;1018:	}
LABELV $352
line 1020
;1019:
;1020:	if (precachedKyle && trap_G2_HaveWeGhoul2Models(precachedKyle))
ADDRLP4 156
ADDRGP4 precachedKyle
INDIRP4
ASGNP4
ADDRLP4 156
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $348
ADDRLP4 156
INDIRP4
ARGP4
ADDRLP4 160
ADDRGP4 trap_G2_HaveWeGhoul2Models
CALLI4
ASGNI4
ADDRLP4 160
INDIRI4
CNSTI4 0
EQI4 $348
line 1021
;1021:	{
line 1022
;1022:		trap_G2API_DuplicateGhoul2Instance(precachedKyle, &client->ghoul2);
ADDRGP4 precachedKyle
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 1716
ADDP4
ARGP4
ADDRGP4 trap_G2API_DuplicateGhoul2Instance
CALLV
pop
line 1023
;1023:	}
line 1025
;1024:	else
;1025:	{
line 1026
;1026:		return;
LABELV $358
line 1031
;1027:	}
;1028:
;1029:	// The model is now loaded.
;1030:
;1031:	GLAName[0] = 0;
ADDRLP4 76
CNSTI1 0
ASGNI1
line 1033
;1032:
;1033:	if (!BGPAFtextLoaded)
ADDRGP4 BGPAFtextLoaded
INDIRI4
CNSTI4 0
NEI4 $359
line 1034
;1034:	{
line 1037
;1035:		//get the location of the animation.cfg
;1036:		//GLAName = trap_G2API_GetGLAName( client->ghoul2, 0);
;1037:		trap_G2API_GetGLAName( client->ghoul2, 0, GLAName);
ADDRFP4 0
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 76
ARGP4
ADDRGP4 trap_G2API_GetGLAName
CALLV
pop
line 1039
;1038:
;1039:		if (!GLAName[0])
ADDRLP4 76
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $361
line 1040
;1040:		{
line 1041
;1041:			if (!BG_ParseAnimationFile("models/players/_humanoid/animation.cfg"))
ADDRGP4 $365
ARGP4
ADDRLP4 164
ADDRGP4 BG_ParseAnimationFile
CALLI4
ASGNI4
ADDRLP4 164
INDIRI4
CNSTI4 0
NEI4 $348
line 1042
;1042:			{
line 1043
;1043:				Com_Printf( "Failed to load animation file %s\n", afilename );
ADDRGP4 $366
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1044
;1044:				return;
ADDRGP4 $348
JUMPV
line 1046
;1045:			}
;1046:			return;
LABELV $361
line 1048
;1047:		}
;1048:		Q_strncpyz( afilename, GLAName, sizeof( afilename ));
ADDRLP4 0
ARGP4
ADDRLP4 76
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1049
;1049:		slash = Q_strrchr( afilename, '/' );
ADDRLP4 0
ARGP4
CNSTI4 47
ARGI4
ADDRLP4 164
ADDRGP4 Q_strrchr
CALLP4
ASGNP4
ADDRLP4 140
ADDRLP4 164
INDIRP4
ASGNP4
line 1050
;1050:		if ( slash )
ADDRLP4 140
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $348
line 1051
;1051:		{
line 1052
;1052:			strcpy(slash, "/animation.cfg");
ADDRLP4 140
INDIRP4
ARGP4
ADDRGP4 $369
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1053
;1053:		}	// Now afilename holds just the path to the animation.cfg
line 1055
;1054:		else 
;1055:		{	// Didn't find any slashes, this is a raw filename right in base (whish isn't a good thing)
line 1056
;1056:			return;
LABELV $368
line 1060
;1057:		}
;1058:
;1059:		// Try to load the animation.cfg for this model then.
;1060:		if ( !BG_ParseAnimationFile( afilename ) )
ADDRLP4 0
ARGP4
ADDRLP4 168
ADDRGP4 BG_ParseAnimationFile
CALLI4
ASGNI4
ADDRLP4 168
INDIRI4
CNSTI4 0
NEI4 $370
line 1061
;1061:		{	// The GLA's animations failed
line 1062
;1062:			if (!BG_ParseAnimationFile("models/players/_humanoid/animation.cfg"))
ADDRGP4 $365
ARGP4
ADDRLP4 172
ADDRGP4 BG_ParseAnimationFile
CALLI4
ASGNI4
ADDRLP4 172
INDIRI4
CNSTI4 0
NEI4 $372
line 1063
;1063:			{
line 1064
;1064:				Com_Printf( "Failed to load animation file %s\n", afilename );
ADDRGP4 $366
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1065
;1065:				return;
ADDRGP4 $348
JUMPV
LABELV $372
line 1067
;1066:			}
;1067:		}
LABELV $370
line 1068
;1068:	}
LABELV $359
line 1070
;1069:
;1070:	trap_G2API_AddBolt(client->ghoul2, 0, "*r_hand");
ADDRFP4 0
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 $374
ARGP4
ADDRGP4 trap_G2API_AddBolt
CALLI4
pop
line 1071
;1071:	trap_G2API_AddBolt(client->ghoul2, 0, "*l_hand");
ADDRFP4 0
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 $375
ARGP4
ADDRGP4 trap_G2API_AddBolt
CALLI4
pop
line 1074
;1072:
;1073:	// NOTE - ensure this sequence of bolt and bone accessing are always the same because the client expects them in a certain order
;1074:	trap_G2API_SetBoneAnim(client->ghoul2, 0, "model_root", 0, 12, BONE_ANIM_OVERRIDE_LOOP, 1.0f, level.time, -1, -1);
ADDRFP4 0
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
ARGP4
ADDRLP4 164
CNSTI4 0
ASGNI4
ADDRLP4 164
INDIRI4
ARGI4
ADDRGP4 $376
ARGP4
ADDRLP4 164
INDIRI4
ARGI4
CNSTI4 12
ARGI4
CNSTI4 16
ARGI4
CNSTF4 1065353216
ARGF4
ADDRGP4 level+32
INDIRI4
ARGI4
CNSTF4 3212836864
ARGF4
CNSTI4 -1
ARGI4
ADDRGP4 trap_G2API_SetBoneAnim
CALLI4
pop
line 1075
;1075:	trap_G2API_SetBoneAngles(client->ghoul2, 0, "upper_lumbar", tempVec, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, NULL, 0, level.time);
ADDRFP4 0
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
ARGP4
ADDRLP4 168
CNSTI4 0
ASGNI4
ADDRLP4 168
INDIRI4
ARGI4
ADDRGP4 $378
ARGP4
ADDRLP4 64
ARGP4
CNSTI4 2
ARGI4
CNSTI4 1
ARGI4
CNSTI4 6
ARGI4
CNSTI4 5
ARGI4
CNSTP4 0
ARGP4
ADDRLP4 168
INDIRI4
ARGI4
ADDRGP4 level+32
INDIRI4
ARGI4
ADDRGP4 trap_G2API_SetBoneAngles
CALLI4
pop
line 1076
;1076:	trap_G2API_SetBoneAngles(client->ghoul2, 0, "cranium", tempVec, BONE_ANGLES_POSTMULT, POSITIVE_Z, NEGATIVE_Y, POSITIVE_X, NULL, 0, level.time);
ADDRFP4 0
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
ARGP4
ADDRLP4 172
CNSTI4 0
ASGNI4
ADDRLP4 172
INDIRI4
ARGI4
ADDRGP4 $380
ARGP4
ADDRLP4 64
ARGP4
ADDRLP4 176
CNSTI4 2
ASGNI4
ADDRLP4 176
INDIRI4
ARGI4
ADDRLP4 176
INDIRI4
ARGI4
CNSTI4 6
ARGI4
CNSTI4 1
ARGI4
CNSTP4 0
ARGP4
ADDRLP4 172
INDIRI4
ARGI4
ADDRGP4 level+32
INDIRI4
ARGI4
ADDRGP4 trap_G2API_SetBoneAngles
CALLI4
pop
line 1078
;1077:
;1078:	if (!g2SaberInstance)
ADDRGP4 g2SaberInstance
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $382
line 1079
;1079:	{
line 1080
;1080:		trap_G2API_InitGhoul2Model(&g2SaberInstance, "models/weapons2/saber/saber_w.glm", 0, 0, -20, 0, 0);
ADDRGP4 g2SaberInstance
ARGP4
ADDRGP4 $108
ARGP4
ADDRLP4 180
CNSTI4 0
ASGNI4
ADDRLP4 180
INDIRI4
ARGI4
ADDRLP4 180
INDIRI4
ARGI4
CNSTI4 -20
ARGI4
ADDRLP4 180
INDIRI4
ARGI4
ADDRLP4 180
INDIRI4
ARGI4
ADDRGP4 trap_G2API_InitGhoul2Model
CALLI4
pop
line 1082
;1081:
;1082:		if (g2SaberInstance)
ADDRGP4 g2SaberInstance
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $384
line 1083
;1083:		{
line 1085
;1084:			// indicate we will be bolted to model 0 (ie the player) on bolt 0 (always the right hand) when we get copied
;1085:			trap_G2API_SetBoltInfo(g2SaberInstance, 0, 0);
ADDRGP4 g2SaberInstance
INDIRP4
ARGP4
ADDRLP4 184
CNSTI4 0
ASGNI4
ADDRLP4 184
INDIRI4
ARGI4
ADDRLP4 184
INDIRI4
ARGI4
ADDRGP4 trap_G2API_SetBoltInfo
CALLV
pop
line 1087
;1086:			// now set up the gun bolt on it
;1087:			trap_G2API_AddBolt(g2SaberInstance, 0, "*flash");
ADDRGP4 g2SaberInstance
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 $386
ARGP4
ADDRGP4 trap_G2API_AddBolt
CALLI4
pop
line 1088
;1088:		}
LABELV $384
line 1089
;1089:	}
LABELV $382
line 1091
;1090:
;1091:	if (g2SaberInstance)
ADDRGP4 g2SaberInstance
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $387
line 1092
;1092:	{
line 1093
;1093:		trap_G2API_CopySpecificGhoul2Model(g2SaberInstance, 0, client->ghoul2, 1); 
ADDRGP4 g2SaberInstance
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 trap_G2API_CopySpecificGhoul2Model
CALLV
pop
line 1094
;1094:	}
LABELV $387
line 1095
;1095:}
LABELV $348
endproc SetupGameGhoul2Model 188 44
export ClientUserinfoChanged
proc ClientUserinfoChanged 6372 52
line 1111
;1096:
;1097:
;1098:
;1099:
;1100:/*
;1101:===========
;1102:ClientUserInfoChanged
;1103:
;1104:Called from ClientConnect when the player first connects and
;1105:directly by the server system when the player updates a userinfo variable.
;1106:
;1107:The game can override any of the settings and call trap_SetUserinfo
;1108:if desired.
;1109:============
;1110:*/
;1111:void ClientUserinfoChanged( int clientNum ) {
line 1126
;1112:	gentity_t *ent;
;1113:	int		teamTask, teamLeader, team, health;
;1114:	char	*s;
;1115:	char	model[MAX_QPATH];
;1116:	//char	headModel[MAX_QPATH];
;1117:	char	forcePowers[MAX_QPATH];
;1118:	char	oldname[MAX_STRING_CHARS];
;1119:	gclient_t	*client;
;1120:	char	c1[MAX_INFO_STRING];
;1121:	char	c2[MAX_INFO_STRING];
;1122:	char	redTeam[MAX_INFO_STRING];
;1123:	char	blueTeam[MAX_INFO_STRING];
;1124:	char	userinfo[MAX_INFO_STRING];
;1125:
;1126:	ent = g_entities + clientNum;
ADDRLP4 1032
CNSTI4 828
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1127
;1127:	client = ent->client;
ADDRLP4 0
ADDRLP4 1032
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
line 1129
;1128:
;1129:	trap_GetUserinfo( clientNum, userinfo, sizeof( userinfo ) );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 1132
;1130:
;1131:	// check for malformed or illegal info strings
;1132:	if ( !Info_Validate(userinfo) ) {
ADDRLP4 4
ARGP4
ADDRLP4 6300
ADDRGP4 Info_Validate
CALLI4
ASGNI4
ADDRLP4 6300
INDIRI4
CNSTI4 0
NEI4 $390
line 1133
;1133:		strcpy (userinfo, "\\name\\badinfo");
ADDRLP4 4
ARGP4
ADDRGP4 $392
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1134
;1134:	}
LABELV $390
line 1137
;1135:
;1136:	// check for local client
;1137:	s = Info_ValueForKey( userinfo, "ip" );
ADDRLP4 4
ARGP4
ADDRGP4 $393
ARGP4
ADDRLP4 6304
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6304
INDIRP4
ASGNP4
line 1138
;1138:	if ( !strcmp( s, "localhost" ) ) {
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 $396
ARGP4
ADDRLP4 6308
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 6308
INDIRI4
CNSTI4 0
NEI4 $394
line 1139
;1139:		client->pers.localClient = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 1400
ADDP4
CNSTI4 1
ASGNI4
line 1140
;1140:	}
LABELV $394
line 1143
;1141:
;1142:	// check the item prediction
;1143:	s = Info_ValueForKey( userinfo, "cg_predictItems" );
ADDRLP4 4
ARGP4
ADDRGP4 $397
ARGP4
ADDRLP4 6312
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6312
INDIRP4
ASGNP4
line 1144
;1144:	if ( !atoi( s ) ) {
ADDRLP4 1028
INDIRP4
ARGP4
ADDRLP4 6316
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 6316
INDIRI4
CNSTI4 0
NEI4 $398
line 1145
;1145:		client->pers.predictItemPickup = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 1408
ADDP4
CNSTI4 0
ASGNI4
line 1146
;1146:	} else {
ADDRGP4 $399
JUMPV
LABELV $398
line 1147
;1147:		client->pers.predictItemPickup = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 1408
ADDP4
CNSTI4 1
ASGNI4
line 1148
;1148:	}
LABELV $399
line 1151
;1149:
;1150:	// set name
;1151:	Q_strncpyz ( oldname, client->pers.netname, sizeof( oldname ) );
ADDRLP4 1100
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 1416
ADDP4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1152
;1152:	s = Info_ValueForKey (userinfo, "name");
ADDRLP4 4
ARGP4
ADDRGP4 $400
ARGP4
ADDRLP4 6320
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6320
INDIRP4
ASGNP4
line 1153
;1153:	ClientCleanName( s, client->pers.netname, sizeof(client->pers.netname) );
ADDRLP4 1028
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 1416
ADDP4
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 ClientCleanName
CALLV
pop
line 1155
;1154:
;1155:	if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
ADDRLP4 0
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
NEI4 $401
line 1156
;1156:		if ( client->sess.spectatorState == SPECTATOR_SCOREBOARD ) {
ADDRLP4 0
INDIRP4
CNSTI4 1528
ADDP4
INDIRI4
CNSTI4 3
NEI4 $403
line 1157
;1157:			Q_strncpyz( client->pers.netname, "scoreboard", sizeof(client->pers.netname) );
ADDRLP4 0
INDIRP4
CNSTI4 1416
ADDP4
ARGP4
ADDRGP4 $405
ARGP4
CNSTI4 36
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1158
;1158:		}
LABELV $403
line 1159
;1159:	}
LABELV $401
line 1161
;1160:
;1161:	if ( client->pers.connected == CON_CONNECTED ) {
ADDRLP4 0
INDIRP4
CNSTI4 1368
ADDP4
INDIRI4
CNSTI4 2
NEI4 $406
line 1162
;1162:		if ( strcmp( oldname, client->pers.netname ) ) {
ADDRLP4 1100
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 1416
ADDP4
ARGP4
ADDRLP4 6324
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 6324
INDIRI4
CNSTI4 0
EQI4 $408
line 1163
;1163:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " %s %s\n\"", oldname, G_GetStripEdString("SVINGAME", "PLRENAME"),
ADDRGP4 $148
ARGP4
ADDRGP4 $411
ARGP4
ADDRLP4 6328
ADDRGP4 G_GetStripEdString
CALLP4
ASGNP4
ADDRGP4 $410
ARGP4
ADDRLP4 1100
ARGP4
ADDRLP4 6328
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 1416
ADDP4
ARGP4
ADDRLP4 6332
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 6332
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 1165
;1164:				client->pers.netname) );
;1165:		}
LABELV $408
line 1166
;1166:	}
LABELV $406
line 1169
;1167:
;1168:	// set max health
;1169:	health = 100; //atoi( Info_ValueForKey( userinfo, "handicap" ) );
ADDRLP4 2132
CNSTI4 100
ASGNI4
line 1170
;1170:	client->pers.maxHealth = health;
ADDRLP4 0
INDIRP4
CNSTI4 1452
ADDP4
ADDRLP4 2132
INDIRI4
ASGNI4
line 1171
;1171:	if ( client->pers.maxHealth < 1 || client->pers.maxHealth > 100 ) {
ADDRLP4 6324
ADDRLP4 0
INDIRP4
CNSTI4 1452
ADDP4
INDIRI4
ASGNI4
ADDRLP4 6324
INDIRI4
CNSTI4 1
LTI4 $414
ADDRLP4 6324
INDIRI4
CNSTI4 100
LEI4 $412
LABELV $414
line 1172
;1172:		client->pers.maxHealth = 100;
ADDRLP4 0
INDIRP4
CNSTI4 1452
ADDP4
CNSTI4 100
ASGNI4
line 1173
;1173:	}
LABELV $412
line 1174
;1174:	client->ps.stats[STAT_MAX_HEALTH] = client->pers.maxHealth;
ADDRLP4 0
INDIRP4
CNSTI4 248
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 1452
ADDP4
INDIRI4
ASGNI4
line 1177
;1175:
;1176:	// set model
;1177:	if( g_gametype.integer >= GT_TEAM ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
LTI4 $415
line 1178
;1178:		Q_strncpyz( model, Info_ValueForKey (userinfo, "team_model"), sizeof( model ) );
ADDRLP4 4
ARGP4
ADDRGP4 $418
ARGP4
ADDRLP4 6332
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1036
ARGP4
ADDRLP4 6332
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1180
;1179:		//Q_strncpyz( headModel, Info_ValueForKey (userinfo, "team_headmodel"), sizeof( headModel ) );
;1180:	} else {
ADDRGP4 $416
JUMPV
LABELV $415
line 1181
;1181:		Q_strncpyz( model, Info_ValueForKey (userinfo, "model"), sizeof( model ) );
ADDRLP4 4
ARGP4
ADDRGP4 $419
ARGP4
ADDRLP4 6332
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1036
ARGP4
ADDRLP4 6332
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1183
;1182:		//Q_strncpyz( headModel, Info_ValueForKey (userinfo, "headmodel"), sizeof( headModel ) );
;1183:	}
LABELV $416
line 1185
;1184:
;1185:	Q_strncpyz( forcePowers, Info_ValueForKey (userinfo, "forcepowers"), sizeof( forcePowers ) );
ADDRLP4 4
ARGP4
ADDRGP4 $420
ARGP4
ADDRLP4 6332
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 2136
ARGP4
ADDRLP4 6332
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1188
;1186:
;1187:	// bots set their team a few frames later
;1188:	if (g_gametype.integer >= GT_TEAM && g_entities[clientNum].r.svFlags & SVF_BOT) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
LTI4 $421
CNSTI4 828
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities+296+8
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $421
line 1189
;1189:		s = Info_ValueForKey( userinfo, "team" );
ADDRLP4 4
ARGP4
ADDRGP4 $426
ARGP4
ADDRLP4 6336
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6336
INDIRP4
ASGNP4
line 1190
;1190:		if ( !Q_stricmp( s, "red" ) || !Q_stricmp( s, "r" ) ) {
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 $429
ARGP4
ADDRLP4 6340
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 6340
INDIRI4
CNSTI4 0
EQI4 $431
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 $430
ARGP4
ADDRLP4 6344
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 6344
INDIRI4
CNSTI4 0
NEI4 $427
LABELV $431
line 1191
;1191:			team = TEAM_RED;
ADDRLP4 4248
CNSTI4 1
ASGNI4
line 1192
;1192:		} else if ( !Q_stricmp( s, "blue" ) || !Q_stricmp( s, "b" ) ) {
ADDRGP4 $422
JUMPV
LABELV $427
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 $434
ARGP4
ADDRLP4 6348
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 6348
INDIRI4
CNSTI4 0
EQI4 $436
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 $435
ARGP4
ADDRLP4 6352
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 6352
INDIRI4
CNSTI4 0
NEI4 $432
LABELV $436
line 1193
;1193:			team = TEAM_BLUE;
ADDRLP4 4248
CNSTI4 2
ASGNI4
line 1194
;1194:		} else {
ADDRGP4 $422
JUMPV
LABELV $432
line 1196
;1195:			// pick the team with the least number of players
;1196:			team = PickTeam( clientNum );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 6356
ADDRGP4 PickTeam
CALLI4
ASGNI4
ADDRLP4 4248
ADDRLP4 6356
INDIRI4
ASGNI4
line 1197
;1197:		}
line 1198
;1198:	}
ADDRGP4 $422
JUMPV
LABELV $421
line 1199
;1199:	else {
line 1200
;1200:		team = client->sess.sessionTeam;
ADDRLP4 4248
ADDRLP4 0
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ASGNI4
line 1201
;1201:	}
LABELV $422
line 1224
;1202:
;1203:/*	NOTE: all client side now
;1204:
;1205:	// team
;1206:	switch( team ) {
;1207:	case TEAM_RED:
;1208:		ForceClientSkin(client, model, "red");
;1209://		ForceClientSkin(client, headModel, "red");
;1210:		break;
;1211:	case TEAM_BLUE:
;1212:		ForceClientSkin(client, model, "blue");
;1213://		ForceClientSkin(client, headModel, "blue");
;1214:		break;
;1215:	}
;1216:	// don't ever use a default skin in teamplay, it would just waste memory
;1217:	// however bots will always join a team but they spawn in as spectator
;1218:	if ( g_gametype.integer >= GT_TEAM && team == TEAM_SPECTATOR) {
;1219:		ForceClientSkin(client, model, "red");
;1220://		ForceClientSkin(client, headModel, "red");
;1221:	}
;1222:*/
;1223:
;1224:	if (g_gametype.integer >= GT_TEAM) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
LTI4 $437
line 1225
;1225:		client->pers.teamInfo = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 1516
ADDP4
CNSTI4 1
ASGNI4
line 1226
;1226:	} else {
ADDRGP4 $438
JUMPV
LABELV $437
line 1227
;1227:		s = Info_ValueForKey( userinfo, "teamoverlay" );
ADDRLP4 4
ARGP4
ADDRGP4 $440
ARGP4
ADDRLP4 6336
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6336
INDIRP4
ASGNP4
line 1228
;1228:		if ( ! *s || atoi( s ) != 0 ) {
ADDRLP4 1028
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $443
ADDRLP4 1028
INDIRP4
ARGP4
ADDRLP4 6344
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 6344
INDIRI4
CNSTI4 0
EQI4 $441
LABELV $443
line 1229
;1229:			client->pers.teamInfo = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 1516
ADDP4
CNSTI4 1
ASGNI4
line 1230
;1230:		} else {
ADDRGP4 $442
JUMPV
LABELV $441
line 1231
;1231:			client->pers.teamInfo = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 1516
ADDP4
CNSTI4 0
ASGNI4
line 1232
;1232:		}
LABELV $442
line 1233
;1233:	}
LABELV $438
line 1245
;1234:	/*
;1235:	s = Info_ValueForKey( userinfo, "cg_pmove_fixed" );
;1236:	if ( !*s || atoi( s ) == 0 ) {
;1237:		client->pers.pmoveFixed = qfalse;
;1238:	}
;1239:	else {
;1240:		client->pers.pmoveFixed = qtrue;
;1241:	}
;1242:	*/
;1243:
;1244:	// team task (0 = none, 1 = offence, 2 = defence)
;1245:	teamTask = atoi(Info_ValueForKey(userinfo, "teamtask"));
ADDRLP4 4
ARGP4
ADDRGP4 $444
ARGP4
ADDRLP4 6336
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 6336
INDIRP4
ARGP4
ADDRLP4 6340
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 2124
ADDRLP4 6340
INDIRI4
ASGNI4
line 1247
;1246:	// team Leader (1 = leader, 0 is normal player)
;1247:	teamLeader = client->sess.teamLeader;
ADDRLP4 2128
ADDRLP4 0
INDIRP4
CNSTI4 1560
ADDP4
INDIRI4
ASGNI4
line 1250
;1248:
;1249:	// colors
;1250:	strcpy(c1, Info_ValueForKey( userinfo, "color1" ));
ADDRLP4 4
ARGP4
ADDRGP4 $445
ARGP4
ADDRLP4 6344
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 2200
ARGP4
ADDRLP4 6344
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1251
;1251:	strcpy(c2, Info_ValueForKey( userinfo, "color2" ));
ADDRLP4 4
ARGP4
ADDRGP4 $446
ARGP4
ADDRLP4 6348
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 3224
ARGP4
ADDRLP4 6348
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1253
;1252:
;1253:	strcpy(redTeam, Info_ValueForKey( userinfo, "g_redteam" ));
ADDRLP4 4
ARGP4
ADDRGP4 $447
ARGP4
ADDRLP4 6352
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4252
ARGP4
ADDRLP4 6352
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1254
;1254:	strcpy(blueTeam, Info_ValueForKey( userinfo, "g_blueteam" ));
ADDRLP4 4
ARGP4
ADDRGP4 $448
ARGP4
ADDRLP4 6356
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 5276
ARGP4
ADDRLP4 6356
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1258
;1255:
;1256:	// send over a subset of the userinfo keys so other clients can
;1257:	// print scoreboards, display models, and play custom sounds
;1258:	if ( ent->r.svFlags & SVF_BOT ) {
ADDRLP4 1032
INDIRP4
CNSTI4 304
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $449
line 1259
;1259:		s = va("n\\%s\\t\\%i\\model\\%s\\c1\\%s\\c2\\%s\\hc\\%i\\w\\%i\\l\\%i\\skill\\%s\\tt\\%d\\tl\\%d",
ADDRLP4 4
ARGP4
ADDRGP4 $452
ARGP4
ADDRLP4 6360
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRGP4 $451
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 1416
ADDP4
ARGP4
ADDRLP4 4248
INDIRI4
ARGI4
ADDRLP4 1036
ARGP4
ADDRLP4 2200
ARGP4
ADDRLP4 3224
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 1452
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 1536
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 1540
ADDP4
INDIRI4
ARGI4
ADDRLP4 6360
INDIRP4
ARGP4
ADDRLP4 2124
INDIRI4
ARGI4
ADDRLP4 2128
INDIRI4
ARGI4
ADDRLP4 6368
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6368
INDIRP4
ASGNP4
line 1263
;1260:			client->pers.netname, team, model,  c1, c2, 
;1261:			client->pers.maxHealth, client->sess.wins, client->sess.losses,
;1262:			Info_ValueForKey( userinfo, "skill" ), teamTask, teamLeader );
;1263:	} else {
ADDRGP4 $450
JUMPV
LABELV $449
line 1264
;1264:		s = va("n\\%s\\t\\%i\\model\\%s\\g_redteam\\%s\\g_blueteam\\%s\\c1\\%s\\c2\\%s\\hc\\%i\\w\\%i\\l\\%i\\tt\\%d\\tl\\%d",
ADDRGP4 $453
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 1416
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ARGI4
ADDRLP4 1036
ARGP4
ADDRLP4 4252
ARGP4
ADDRLP4 5276
ARGP4
ADDRLP4 2200
ARGP4
ADDRLP4 3224
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 1452
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 1536
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 1540
ADDP4
INDIRI4
ARGI4
ADDRLP4 2124
INDIRI4
ARGI4
ADDRLP4 2128
INDIRI4
ARGI4
ADDRLP4 6364
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 6364
INDIRP4
ASGNP4
line 1267
;1265:			client->pers.netname, client->sess.sessionTeam, model, redTeam, blueTeam, c1, c2, 
;1266:			client->pers.maxHealth, client->sess.wins, client->sess.losses, teamTask, teamLeader);
;1267:	}
LABELV $450
line 1269
;1268:
;1269:	trap_SetConfigstring( CS_PLAYERS+clientNum, s );
ADDRFP4 0
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 1271
;1270:
;1271:	G_LogPrintf( "ClientUserinfoChanged: %i %s\n", clientNum, s );
ADDRGP4 $454
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 G_LogPrintf
CALLV
pop
line 1272
;1272:}
LABELV $389
endproc ClientUserinfoChanged 6372 52
export ClientConnect
proc ClientConnect 1068 12
line 1295
;1273:
;1274:
;1275:/*
;1276:===========
;1277:ClientConnect
;1278:
;1279:Called when a player begins connecting to the server.
;1280:Called again for every map change or tournement restart.
;1281:
;1282:The session information will be valid after exit.
;1283:
;1284:Return NULL if the client should be allowed, otherwise return
;1285:a string with the reason for denial.
;1286:
;1287:Otherwise, the client will be sent the current gamestate
;1288:and will eventually get to ClientBegin.
;1289:
;1290:firstTime will be qtrue the very first time a client connects
;1291:to the server machine, but qfalse on map changes and tournement
;1292:restarts.
;1293:============
;1294:*/
;1295:char *ClientConnect( int clientNum, qboolean firstTime, qboolean isBot ) {
line 1303
;1296:	char		*value;
;1297://	char		*areabits;
;1298:	gclient_t	*client;
;1299:	char		userinfo[MAX_INFO_STRING];
;1300:	gentity_t	*ent;
;1301:	gentity_t	*te;
;1302:
;1303:	ent = &g_entities[ clientNum ];
ADDRLP4 4
CNSTI4 828
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1305
;1304:
;1305:	trap_GetUserinfo( clientNum, userinfo, sizeof( userinfo ) );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 8
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 1308
;1306:
;1307:	// check to see if they are on the banned IP list
;1308:	value = Info_ValueForKey (userinfo, "ip");
ADDRLP4 8
ARGP4
ADDRGP4 $393
ARGP4
ADDRLP4 1040
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1032
ADDRLP4 1040
INDIRP4
ASGNP4
line 1309
;1309:	if ( G_FilterPacket( value ) ) {
ADDRLP4 1032
INDIRP4
ARGP4
ADDRLP4 1044
ADDRGP4 G_FilterPacket
CALLI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
EQI4 $456
line 1310
;1310:		return "Banned.";
ADDRGP4 $458
RETP4
ADDRGP4 $455
JUMPV
LABELV $456
line 1313
;1311:	}
;1312:
;1313:	if ( !( ent->r.svFlags & SVF_BOT ) && !isBot && g_needpass.integer ) {
ADDRLP4 1048
CNSTI4 0
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 304
ADDP4
INDIRI4
CNSTI4 8
BANDI4
ADDRLP4 1048
INDIRI4
NEI4 $459
ADDRFP4 8
INDIRI4
ADDRLP4 1048
INDIRI4
NEI4 $459
ADDRGP4 g_needpass+12
INDIRI4
ADDRLP4 1048
INDIRI4
EQI4 $459
line 1315
;1314:		// check for a password
;1315:		value = Info_ValueForKey (userinfo, "password");
ADDRLP4 8
ARGP4
ADDRGP4 $462
ARGP4
ADDRLP4 1052
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1032
ADDRLP4 1052
INDIRP4
ASGNP4
line 1316
;1316:		if ( g_password.string[0] && Q_stricmp( g_password.string, "none" ) &&
ADDRGP4 g_password+16
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $463
ADDRGP4 g_password+16
ARGP4
ADDRGP4 $467
ARGP4
ADDRLP4 1056
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
EQI4 $463
ADDRGP4 g_password+16
ARGP4
ADDRLP4 1032
INDIRP4
ARGP4
ADDRLP4 1060
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 1060
INDIRI4
CNSTI4 0
EQI4 $463
line 1317
;1317:			strcmp( g_password.string, value) != 0) {
line 1318
;1318:			return "Invalid password";
ADDRGP4 $469
RETP4
ADDRGP4 $455
JUMPV
LABELV $463
line 1320
;1319:		}
;1320:	}
LABELV $459
line 1323
;1321:
;1322:	// they can connect
;1323:	ent->client = level.clients + clientNum;
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
CNSTI4 1756
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 1324
;1324:	client = ent->client;
ADDRLP4 0
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
line 1328
;1325:
;1326://	areabits = client->areabits;
;1327:
;1328:	memset( client, 0, sizeof(*client) );
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1756
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1330
;1329:
;1330:	client->pers.connected = CON_CONNECTING;
ADDRLP4 0
INDIRP4
CNSTI4 1368
ADDP4
CNSTI4 1
ASGNI4
line 1333
;1331:
;1332:	// read or initialize the session data
;1333:	if ( firstTime || level.newSession ) {
ADDRLP4 1052
CNSTI4 0
ASGNI4
ADDRFP4 4
INDIRI4
ADDRLP4 1052
INDIRI4
NEI4 $473
ADDRGP4 level+64
INDIRI4
ADDRLP4 1052
INDIRI4
EQI4 $470
LABELV $473
line 1334
;1334:		G_InitSessionData( client, userinfo, isBot );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRFP4 8
INDIRI4
ARGI4
ADDRGP4 G_InitSessionData
CALLV
pop
line 1335
;1335:	}
LABELV $470
line 1336
;1336:	G_ReadSessionData( client );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_ReadSessionData
CALLV
pop
line 1338
;1337:
;1338:	if( isBot ) {
ADDRFP4 8
INDIRI4
CNSTI4 0
EQI4 $474
line 1339
;1339:		ent->r.svFlags |= SVF_BOT;
ADDRLP4 1056
ADDRLP4 4
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 1056
INDIRP4
ADDRLP4 1056
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 1340
;1340:		ent->inuse = qtrue;
ADDRLP4 4
INDIRP4
CNSTI4 412
ADDP4
CNSTI4 1
ASGNI4
line 1341
;1341:		if( !G_BotConnect( clientNum, !firstTime ) ) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $479
ADDRLP4 1060
CNSTI4 1
ASGNI4
ADDRGP4 $480
JUMPV
LABELV $479
ADDRLP4 1060
CNSTI4 0
ASGNI4
LABELV $480
ADDRLP4 1060
INDIRI4
ARGI4
ADDRLP4 1064
ADDRGP4 G_BotConnect
CALLI4
ASGNI4
ADDRLP4 1064
INDIRI4
CNSTI4 0
NEI4 $476
line 1342
;1342:			return "BotConnectfailed";
ADDRGP4 $481
RETP4
ADDRGP4 $455
JUMPV
LABELV $476
line 1344
;1343:		}
;1344:	}
LABELV $474
line 1347
;1345:
;1346:	// get and distribute relevent paramters
;1347:	G_LogPrintf( "ClientConnect: %i\n", clientNum );
ADDRGP4 $482
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 G_LogPrintf
CALLV
pop
line 1348
;1348:	ClientUserinfoChanged( clientNum );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 ClientUserinfoChanged
CALLV
pop
line 1351
;1349:
;1350:	// don't do the "xxx connected" messages if they were caried over from previous level
;1351:	if ( firstTime ) {
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $483
line 1352
;1352:		trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " %s\n\"", client->pers.netname, G_GetStripEdString("SVINGAME", "PLCONNECT")) );
ADDRGP4 $148
ARGP4
ADDRGP4 $486
ARGP4
ADDRLP4 1056
ADDRGP4 G_GetStripEdString
CALLP4
ASGNP4
ADDRGP4 $485
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 1416
ADDP4
ARGP4
ADDRLP4 1056
INDIRP4
ARGP4
ADDRLP4 1060
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 1060
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 1353
;1353:	}
LABELV $483
line 1355
;1354:
;1355:	if ( g_gametype.integer >= GT_TEAM &&
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
LTI4 $487
ADDRLP4 0
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
EQI4 $487
line 1356
;1356:		client->sess.sessionTeam != TEAM_SPECTATOR ) {
line 1357
;1357:		BroadcastTeamChange( client, -1 );
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 BroadcastTeamChange
CALLV
pop
line 1358
;1358:	}
LABELV $487
line 1361
;1359:
;1360:	// count current clients and rank for scoreboard
;1361:	CalculateRanks();
ADDRGP4 CalculateRanks
CALLV
pop
line 1363
;1362:
;1363:	te = G_TempEntity( vec3_origin, EV_CLIENTJOIN );
ADDRGP4 vec3_origin
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 1056
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 1036
ADDRLP4 1056
INDIRP4
ASGNP4
line 1364
;1364:	te->r.svFlags |= SVF_BROADCAST;
ADDRLP4 1060
ADDRLP4 1036
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 1060
INDIRP4
ADDRLP4 1060
INDIRP4
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 1365
;1365:	te->s.eventParm = clientNum;
ADDRLP4 1036
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 0
INDIRI4
ASGNI4
line 1372
;1366:
;1367:	// for statistics
;1368://	client->areabits = areabits;
;1369://	if ( !client->areabits )
;1370://		client->areabits = G_Alloc( (trap_AAS_PointReachabilityAreaIndex( NULL ) + 7) / 8 );
;1371:
;1372:	return NULL;
CNSTP4 0
RETP4
LABELV $455
endproc ClientConnect 1068 12
export ClientBegin
proc ClientBegin 1108 12
line 1386
;1373:}
;1374:
;1375:void G_WriteClientSessionData( gclient_t *client );
;1376:
;1377:/*
;1378:===========
;1379:ClientBegin
;1380:
;1381:called when a client has finished connecting, and is ready
;1382:to be placed into the level.  This will happen every level load,
;1383:and on transition between teams, but doesn't happen on respawns
;1384:============
;1385:*/
;1386:void ClientBegin( int clientNum, qboolean allowTeamReset ) {
line 1393
;1387:	gentity_t	*ent;
;1388:	gclient_t	*client;
;1389:	gentity_t	*tent;
;1390:	int			flags, i;
;1391:	char		userinfo[MAX_INFO_VALUE], *modelname;
;1392:
;1393:	ent = g_entities + clientNum;
ADDRLP4 4
CNSTI4 828
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1395
;1394:
;1395:	if ((ent->r.svFlags & SVF_BOT) && g_gametype.integer >= GT_TEAM)
ADDRLP4 4
INDIRP4
CNSTI4 304
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $491
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
LTI4 $491
line 1396
;1396:	{
line 1397
;1397:		if (allowTeamReset)
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $494
line 1398
;1398:		{
line 1399
;1399:			const char *team = "Red";
ADDRLP4 1048
ADDRGP4 $496
ASGNP4
line 1403
;1400:			int preSess;
;1401:
;1402:			//SetTeam(ent, "");
;1403:			ent->client->sess.sessionTeam = PickTeam(-1);
CNSTI4 -1
ARGI4
ADDRLP4 1056
ADDRGP4 PickTeam
CALLI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
ADDRLP4 1056
INDIRI4
ASGNI4
line 1404
;1404:			trap_GetUserinfo(clientNum, userinfo, MAX_INFO_STRING);
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 12
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 1406
;1405:
;1406:			if (ent->client->sess.sessionTeam == TEAM_SPECTATOR)
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
NEI4 $497
line 1407
;1407:			{
line 1408
;1408:				ent->client->sess.sessionTeam = TEAM_RED;
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
CNSTI4 1
ASGNI4
line 1409
;1409:			}
LABELV $497
line 1411
;1410:
;1411:			if (ent->client->sess.sessionTeam == TEAM_RED)
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 1
NEI4 $499
line 1412
;1412:			{
line 1413
;1413:				team = "Red";
ADDRLP4 1048
ADDRGP4 $496
ASGNP4
line 1414
;1414:			}
ADDRGP4 $500
JUMPV
LABELV $499
line 1416
;1415:			else
;1416:			{
line 1417
;1417:				team = "Blue";
ADDRLP4 1048
ADDRGP4 $501
ASGNP4
line 1418
;1418:			}
LABELV $500
line 1420
;1419:
;1420:			Info_SetValueForKey( userinfo, "team", team );
ADDRLP4 12
ARGP4
ADDRGP4 $426
ARGP4
ADDRLP4 1048
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 1422
;1421:
;1422:			trap_SetUserinfo( clientNum, userinfo );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 12
ARGP4
ADDRGP4 trap_SetUserinfo
CALLV
pop
line 1424
;1423:
;1424:			ent->client->ps.persistant[ PERS_TEAM ] = ent->client->sess.sessionTeam;
ADDRLP4 1060
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1060
INDIRP4
CNSTI4 292
ADDP4
ADDRLP4 1060
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ASGNI4
line 1426
;1425:
;1426:			preSess = ent->client->sess.sessionTeam;
ADDRLP4 1052
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ASGNI4
line 1427
;1427:			G_ReadSessionData( ent->client );
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ARGP4
ADDRGP4 G_ReadSessionData
CALLV
pop
line 1428
;1428:			ent->client->sess.sessionTeam = preSess;
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
ADDRLP4 1052
INDIRI4
ASGNI4
line 1429
;1429:			G_WriteClientSessionData(ent->client);
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ARGP4
ADDRGP4 G_WriteClientSessionData
CALLV
pop
line 1430
;1430:			ClientUserinfoChanged( clientNum );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 ClientUserinfoChanged
CALLV
pop
line 1431
;1431:			ClientBegin(clientNum, qfalse);
ADDRFP4 0
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 ClientBegin
CALLV
pop
line 1432
;1432:			return;
ADDRGP4 $490
JUMPV
LABELV $494
line 1434
;1433:		}
;1434:	}
LABELV $491
line 1436
;1435:
;1436:	client = level.clients + clientNum;
ADDRLP4 8
CNSTI4 1756
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 1438
;1437:
;1438:	if ( ent->r.linked ) {
ADDRLP4 4
INDIRP4
CNSTI4 296
ADDP4
INDIRI4
CNSTI4 0
EQI4 $502
line 1439
;1439:		trap_UnlinkEntity( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 1440
;1440:	}
LABELV $502
line 1441
;1441:	G_InitGentity( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 G_InitGentity
CALLV
pop
line 1442
;1442:	ent->touch = 0;
ADDRLP4 4
INDIRP4
CNSTI4 648
ADDP4
CNSTP4 0
ASGNP4
line 1443
;1443:	ent->pain = 0;
ADDRLP4 4
INDIRP4
CNSTI4 656
ADDP4
CNSTP4 0
ASGNP4
line 1444
;1444:	ent->client = client;
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
ADDRLP4 8
INDIRP4
ASGNP4
line 1446
;1445:
;1446:	client->pers.connected = CON_CONNECTED;
ADDRLP4 8
INDIRP4
CNSTI4 1368
ADDP4
CNSTI4 2
ASGNI4
line 1447
;1447:	client->pers.enterTime = level.time;
ADDRLP4 8
INDIRP4
CNSTI4 1456
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1448
;1448:	client->pers.teamState.state = TEAM_BEGIN;
ADDRLP4 8
INDIRP4
CNSTI4 1460
ADDP4
CNSTI4 0
ASGNI4
line 1455
;1449:
;1450:	// save eflags around this, because changing teams will
;1451:	// cause this to happen with a valid entity, and we
;1452:	// want to make sure the teleport bit is set right
;1453:	// so the viewpoint doesn't interpolate through the
;1454:	// world to the new position
;1455:	flags = client->ps.eFlags;
ADDRLP4 1036
ADDRLP4 8
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
ASGNI4
line 1457
;1456:
;1457:	i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $506
JUMPV
LABELV $505
line 1460
;1458:
;1459:	while (i < NUM_FORCE_POWERS)
;1460:	{
line 1461
;1461:		if (ent->client->ps.fd.forcePowersActive & (1 << i))
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 836
ADDP4
INDIRI4
CNSTI4 1
ADDRLP4 0
INDIRI4
LSHI4
BANDI4
CNSTI4 0
EQI4 $508
line 1462
;1462:		{
line 1463
;1463:			WP_ForcePowerStop(ent, i);
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 WP_ForcePowerStop
CALLV
pop
line 1464
;1464:		}
LABELV $508
line 1465
;1465:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1466
;1466:	}
LABELV $506
line 1459
ADDRLP4 0
INDIRI4
CNSTI4 18
LTI4 $505
line 1468
;1467:
;1468:	i = TRACK_CHANNEL_1;
ADDRLP4 0
CNSTI4 51
ASGNI4
ADDRGP4 $511
JUMPV
LABELV $510
line 1471
;1469:
;1470:	while (i < NUM_TRACK_CHANNELS)
;1471:	{
line 1472
;1472:		if (ent->client->ps.fd.killSoundEntIndex[i-50] && ent->client->ps.fd.killSoundEntIndex[i-50] < MAX_GENTITIES && ent->client->ps.fd.killSoundEntIndex[i-50] > 0)
ADDRLP4 1048
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 200
SUBI4
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1188
ADDP4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1052
CNSTI4 0
ASGNI4
ADDRLP4 1048
INDIRI4
ADDRLP4 1052
INDIRI4
EQI4 $513
ADDRLP4 1048
INDIRI4
CNSTI4 1024
GEI4 $513
ADDRLP4 1048
INDIRI4
ADDRLP4 1052
INDIRI4
LEI4 $513
line 1473
;1473:		{
line 1474
;1474:			G_MuteSound(ent->client->ps.fd.killSoundEntIndex[i-50], CHAN_VOICE);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 200
SUBI4
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1188
ADDP4
ADDP4
INDIRI4
ARGI4
CNSTI4 3
ARGI4
ADDRGP4 G_MuteSound
CALLV
pop
line 1475
;1475:		}
LABELV $513
line 1476
;1476:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1477
;1477:	}
LABELV $511
line 1470
ADDRLP4 0
INDIRI4
CNSTI4 56
LTI4 $510
line 1478
;1478:	i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1480
;1479:
;1480:	memset( &client->ps, 0, sizeof( client->ps ) );
ADDRLP4 8
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1368
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1481
;1481:	client->ps.eFlags = flags;
ADDRLP4 8
INDIRP4
CNSTI4 108
ADDP4
ADDRLP4 1036
INDIRI4
ASGNI4
line 1483
;1482:
;1483:	client->ps.hasDetPackPlanted = qfalse;
ADDRLP4 8
INDIRP4
CNSTI4 628
ADDP4
CNSTI4 0
ASGNI4
line 1486
;1484:
;1485:	//first-time force power initialization
;1486:	WP_InitForcePowers( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 WP_InitForcePowers
CALLV
pop
line 1489
;1487:
;1488:	//init saber ent
;1489:	WP_SaberInitBladeData( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 WP_SaberInitBladeData
CALLV
pop
line 1492
;1490:
;1491:	// First time model setup for that player.
;1492:	trap_GetUserinfo( clientNum, userinfo, sizeof(userinfo) );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 12
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 1493
;1493:	modelname = Info_ValueForKey (userinfo, "model");
ADDRLP4 12
ARGP4
ADDRGP4 $419
ARGP4
ADDRLP4 1048
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1040
ADDRLP4 1048
INDIRP4
ASGNP4
line 1494
;1494:	SetupGameGhoul2Model(client, modelname);
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 1040
INDIRP4
ARGP4
ADDRGP4 SetupGameGhoul2Model
CALLV
pop
line 1496
;1495:
;1496:	if (ent->client->ghoul2)
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $515
line 1497
;1497:	{
line 1498
;1498:		ent->bolt_Head = trap_G2API_AddBolt(ent->client->ghoul2, 0, "cranium");
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 $380
ARGP4
ADDRLP4 1056
ADDRGP4 trap_G2API_AddBolt
CALLI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 784
ADDP4
ADDRLP4 1056
INDIRI4
ASGNI4
line 1499
;1499:		ent->bolt_Waist = trap_G2API_AddBolt(ent->client->ghoul2, 0, "thoracic");
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 $517
ARGP4
ADDRLP4 1064
ADDRGP4 trap_G2API_AddBolt
CALLI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 804
ADDP4
ADDRLP4 1064
INDIRI4
ASGNI4
line 1500
;1500:		ent->bolt_LArm = trap_G2API_AddBolt(ent->client->ghoul2, 0, "lradius");
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 $518
ARGP4
ADDRLP4 1072
ADDRGP4 trap_G2API_AddBolt
CALLI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 788
ADDP4
ADDRLP4 1072
INDIRI4
ASGNI4
line 1501
;1501:		ent->bolt_RArm = trap_G2API_AddBolt(ent->client->ghoul2, 0, "rradius");
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 $519
ARGP4
ADDRLP4 1080
ADDRGP4 trap_G2API_AddBolt
CALLI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 792
ADDP4
ADDRLP4 1080
INDIRI4
ASGNI4
line 1502
;1502:		ent->bolt_LLeg = trap_G2API_AddBolt(ent->client->ghoul2, 0, "ltibia");
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 $520
ARGP4
ADDRLP4 1088
ADDRGP4 trap_G2API_AddBolt
CALLI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 796
ADDP4
ADDRLP4 1088
INDIRI4
ASGNI4
line 1503
;1503:		ent->bolt_RLeg = trap_G2API_AddBolt(ent->client->ghoul2, 0, "rtibia");
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 $521
ARGP4
ADDRLP4 1096
ADDRGP4 trap_G2API_AddBolt
CALLI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 800
ADDP4
ADDRLP4 1096
INDIRI4
ASGNI4
line 1504
;1504:		ent->bolt_Motion = trap_G2API_AddBolt(ent->client->ghoul2, 0, "Motion");
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 $522
ARGP4
ADDRLP4 1104
ADDRGP4 trap_G2API_AddBolt
CALLI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 808
ADDP4
ADDRLP4 1104
INDIRI4
ASGNI4
line 1505
;1505:	}
LABELV $515
line 1508
;1506:
;1507:	// locate ent at a spawn point
;1508:	ClientSpawn( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 ClientSpawn
CALLV
pop
line 1510
;1509:
;1510:	if ( client->sess.sessionTeam != TEAM_SPECTATOR ) {
ADDRLP4 8
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
EQI4 $523
line 1512
;1511:		// send event
;1512:		tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_IN );
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
CNSTI4 59
ARGI4
ADDRLP4 1052
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 1044
ADDRLP4 1052
INDIRP4
ASGNP4
line 1513
;1513:		tent->s.clientNum = ent->s.clientNum;
ADDRLP4 1056
CNSTI4 220
ASGNI4
ADDRLP4 1044
INDIRP4
ADDRLP4 1056
INDIRI4
ADDP4
ADDRLP4 4
INDIRP4
ADDRLP4 1056
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 1515
;1514:
;1515:		if ( g_gametype.integer != GT_TOURNAMENT  ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
EQI4 $525
line 1516
;1516:			trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " %s\n\"", client->pers.netname, G_GetStripEdString("SVINGAME", "PLENTER")) );
ADDRGP4 $148
ARGP4
ADDRGP4 $528
ARGP4
ADDRLP4 1060
ADDRGP4 G_GetStripEdString
CALLP4
ASGNP4
ADDRGP4 $485
ARGP4
ADDRLP4 8
INDIRP4
CNSTI4 1416
ADDP4
ARGP4
ADDRLP4 1060
INDIRP4
ARGP4
ADDRLP4 1064
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 1064
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 1517
;1517:		}
LABELV $525
line 1518
;1518:	}
LABELV $523
line 1519
;1519:	G_LogPrintf( "ClientBegin: %i\n", clientNum );
ADDRGP4 $529
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 G_LogPrintf
CALLV
pop
line 1522
;1520:
;1521:	// count current clients and rank for scoreboard
;1522:	CalculateRanks();
ADDRGP4 CalculateRanks
CALLV
pop
line 1524
;1523:
;1524:	G_ClearClientLog(clientNum);
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 G_ClearClientLog
CALLV
pop
line 1525
;1525:}
LABELV $490
endproc ClientBegin 1108 12
export ClientSpawn
proc ClientSpawn 860 16
line 1536
;1526:
;1527:/*
;1528:===========
;1529:ClientSpawn
;1530:
;1531:Called every time a client is placed fresh in the world:
;1532:after the first ClientBegin, and after each respawn
;1533:Initializes all non-persistant parts of playerState
;1534:============
;1535:*/
;1536:void ClientSpawn(gentity_t *ent) {
line 1553
;1537:	int		index;
;1538:	vec3_t	spawn_origin, spawn_angles;
;1539:	gclient_t	*client;
;1540:	int		i;
;1541:	clientPersistant_t	saved;
;1542:	clientSession_t		savedSess;
;1543:	int		persistant[MAX_PERSISTANT];
;1544:	gentity_t	*spawnPoint;
;1545:	int		flags;
;1546:	int		savedPing;
;1547://	char	*savedAreaBits;
;1548:	int		accuracy_hits, accuracy_shots;
;1549:	int		eventSequence;
;1550://	char	userinfo[MAX_INFO_STRING];
;1551:	forcedata_t			savedForce;
;1552:	void		*ghoul2save;
;1553:	int		saveSaberNum = ENTITYNUM_NONE;
ADDRLP4 112
CNSTI4 1023
ASGNI4
line 1554
;1554:	int		wDisable = 0;
ADDRLP4 88
CNSTI4 0
ASGNI4
line 1556
;1555:
;1556:	index = ent - g_entities;
ADDRLP4 104
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRGP4 g_entities
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 828
DIVI4
ASGNI4
line 1557
;1557:	client = ent->client;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
line 1559
;1558:
;1559:	if (client->ps.fd.forceDoInit)
ADDRLP4 0
INDIRP4
CNSTI4 1172
ADDP4
INDIRI4
CNSTI4 0
EQI4 $531
line 1560
;1560:	{ //force a reread of force powers
line 1561
;1561:		WP_InitForcePowers( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 WP_InitForcePowers
CALLV
pop
line 1562
;1562:		client->ps.fd.forceDoInit = 0;
ADDRLP4 0
INDIRP4
CNSTI4 1172
ADDP4
CNSTI4 0
ASGNI4
line 1563
;1563:	}
LABELV $531
line 1567
;1564:	// find a spawn point
;1565:	// do it before setting health back up, so farthest
;1566:	// ranging doesn't count this client
;1567:	if ( client->sess.sessionTeam == TEAM_SPECTATOR ) {
ADDRLP4 0
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
NEI4 $533
line 1568
;1568:		spawnPoint = SelectSpectatorSpawnPoint ( 
ADDRLP4 76
ARGP4
ADDRLP4 92
ARGP4
ADDRLP4 804
ADDRGP4 SelectSpectatorSpawnPoint
CALLP4
ASGNP4
ADDRLP4 72
ADDRLP4 804
INDIRP4
ASGNP4
line 1570
;1569:						spawn_origin, spawn_angles);
;1570:	} else if (g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY) {
ADDRGP4 $534
JUMPV
LABELV $533
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 7
EQI4 $539
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 8
NEI4 $535
LABELV $539
line 1572
;1571:		// all base oriented team games use the CTF spawn points
;1572:		spawnPoint = SelectCTFSpawnPoint ( 
ADDRLP4 0
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 1460
ADDP4
INDIRI4
ARGI4
ADDRLP4 76
ARGP4
ADDRLP4 92
ARGP4
ADDRLP4 808
ADDRGP4 SelectCTFSpawnPoint
CALLP4
ASGNP4
ADDRLP4 72
ADDRLP4 808
INDIRP4
ASGNP4
line 1576
;1573:						client->sess.sessionTeam, 
;1574:						client->pers.teamState.state, 
;1575:						spawn_origin, spawn_angles);
;1576:	}
ADDRGP4 $536
JUMPV
LABELV $535
line 1577
;1577:	else if (g_gametype.integer == GT_SAGA)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 6
NEI4 $540
line 1578
;1578:	{
line 1579
;1579:		spawnPoint = SelectSagaSpawnPoint ( 
ADDRLP4 0
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 1460
ADDP4
INDIRI4
ARGI4
ADDRLP4 76
ARGP4
ADDRLP4 92
ARGP4
ADDRLP4 808
ADDRGP4 SelectSagaSpawnPoint
CALLP4
ASGNP4
ADDRLP4 72
ADDRLP4 808
INDIRP4
ASGNP4
line 1583
;1580:						client->sess.sessionTeam, 
;1581:						client->pers.teamState.state, 
;1582:						spawn_origin, spawn_angles);
;1583:	}
ADDRGP4 $541
JUMPV
LABELV $540
line 1584
;1584:	else {
LABELV $543
line 1585
;1585:		do {
line 1587
;1586:			// the first spawn should be at a good looking spot
;1587:			if ( !client->pers.initialSpawn && client->pers.localClient ) {
ADDRLP4 808
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 1404
ADDP4
INDIRI4
ADDRLP4 808
INDIRI4
NEI4 $546
ADDRLP4 0
INDIRP4
CNSTI4 1400
ADDP4
INDIRI4
ADDRLP4 808
INDIRI4
EQI4 $546
line 1588
;1588:				client->pers.initialSpawn = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 1404
ADDP4
CNSTI4 1
ASGNI4
line 1589
;1589:				spawnPoint = SelectInitialSpawnPoint( spawn_origin, spawn_angles );
ADDRLP4 76
ARGP4
ADDRLP4 92
ARGP4
ADDRLP4 812
ADDRGP4 SelectInitialSpawnPoint
CALLP4
ASGNP4
ADDRLP4 72
ADDRLP4 812
INDIRP4
ASGNP4
line 1590
;1590:			} else {
ADDRGP4 $547
JUMPV
LABELV $546
line 1592
;1591:				// don't spawn near existing origin if possible
;1592:				spawnPoint = SelectSpawnPoint ( 
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 76
ARGP4
ADDRLP4 92
ARGP4
ADDRLP4 812
ADDRGP4 SelectSpawnPoint
CALLP4
ASGNP4
ADDRLP4 72
ADDRLP4 812
INDIRP4
ASGNP4
line 1595
;1593:					client->ps.origin, 
;1594:					spawn_origin, spawn_angles);
;1595:			}
LABELV $547
line 1599
;1596:
;1597:			// Tim needs to prevent bots from spawning at the initial point
;1598:			// on q3dm0...
;1599:			if ( ( spawnPoint->flags & FL_NO_BOTS ) && ( ent->r.svFlags & SVF_BOT ) ) {
ADDRLP4 812
CNSTI4 0
ASGNI4
ADDRLP4 72
INDIRP4
CNSTI4 472
ADDP4
INDIRI4
CNSTI4 8192
BANDI4
ADDRLP4 812
INDIRI4
EQI4 $548
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
INDIRI4
CNSTI4 8
BANDI4
ADDRLP4 812
INDIRI4
EQI4 $548
line 1600
;1600:				continue;	// try again
ADDRGP4 $544
JUMPV
LABELV $548
line 1603
;1601:			}
;1602:			// just to be symetric, we have a nohumans option...
;1603:			if ( ( spawnPoint->flags & FL_NO_HUMANS ) && !( ent->r.svFlags & SVF_BOT ) ) {
ADDRLP4 816
CNSTI4 0
ASGNI4
ADDRLP4 72
INDIRP4
CNSTI4 472
ADDP4
INDIRI4
CNSTI4 16384
BANDI4
ADDRLP4 816
INDIRI4
EQI4 $545
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
INDIRI4
CNSTI4 8
BANDI4
ADDRLP4 816
INDIRI4
NEI4 $545
line 1604
;1604:				continue;	// try again
line 1607
;1605:			}
;1606:
;1607:			break;
LABELV $544
line 1609
;1608:
;1609:		} while ( 1 );
ADDRGP4 $543
JUMPV
LABELV $545
line 1610
;1610:	}
LABELV $541
LABELV $536
LABELV $534
line 1611
;1611:	client->pers.teamState.state = TEAM_ACTIVE;
ADDRLP4 0
INDIRP4
CNSTI4 1460
ADDP4
CNSTI4 1
ASGNI4
line 1615
;1612:
;1613:	// toggle the teleport bit so the client knows to not lerp
;1614:	// and never clear the voted flag
;1615:	flags = ent->client->ps.eFlags & (EF_TELEPORT_BIT | EF_VOTED | EF_TEAMVOTED);
ADDRLP4 108
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
CNSTI4 540676
BANDI4
ASGNI4
line 1616
;1616:	flags ^= EF_TELEPORT_BIT;
ADDRLP4 108
ADDRLP4 108
INDIRI4
CNSTI4 4
BXORI4
ASGNI4
line 1620
;1617:
;1618:	// clear everything but the persistant data
;1619:
;1620:	saved = client->pers;
ADDRLP4 116
ADDRLP4 0
INDIRP4
CNSTI4 1368
ADDP4
INDIRB
ASGNB 152
line 1621
;1621:	savedSess = client->sess;
ADDRLP4 268
ADDRLP4 0
INDIRP4
CNSTI4 1520
ADDP4
INDIRB
ASGNB 44
line 1622
;1622:	savedPing = client->ps.ping;
ADDRLP4 312
ADDRLP4 0
INDIRP4
CNSTI4 484
ADDP4
INDIRI4
ASGNI4
line 1624
;1623://	savedAreaBits = client->areabits;
;1624:	accuracy_hits = client->accuracy_hits;
ADDRLP4 316
ADDRLP4 0
INDIRP4
CNSTI4 1656
ADDP4
INDIRI4
ASGNI4
line 1625
;1625:	accuracy_shots = client->accuracy_shots;
ADDRLP4 320
ADDRLP4 0
INDIRP4
CNSTI4 1652
ADDP4
INDIRI4
ASGNI4
line 1626
;1626:	for ( i = 0 ; i < MAX_PERSISTANT ; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $552
line 1627
;1627:		persistant[i] = client->ps.persistant[i];
ADDRLP4 804
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 804
INDIRI4
ADDRLP4 8
ADDP4
ADDRLP4 804
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 280
ADDP4
ADDP4
INDIRI4
ASGNI4
line 1628
;1628:	}
LABELV $553
line 1626
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 16
LTI4 $552
line 1629
;1629:	eventSequence = client->ps.eventSequence;
ADDRLP4 324
ADDRLP4 0
INDIRP4
CNSTI4 112
ADDP4
INDIRI4
ASGNI4
line 1631
;1630:
;1631:	savedForce = client->ps.fd;
ADDRLP4 328
ADDRLP4 0
INDIRP4
CNSTI4 760
ADDP4
INDIRB
ASGNB 472
line 1633
;1632:
;1633:	ghoul2save = client->ghoul2;
ADDRLP4 800
ADDRLP4 0
INDIRP4
CNSTI4 1716
ADDP4
INDIRP4
ASGNP4
line 1635
;1634:
;1635:	saveSaberNum = client->ps.saberEntityNum;
ADDRLP4 112
ADDRLP4 0
INDIRP4
CNSTI4 544
ADDP4
INDIRI4
ASGNI4
line 1637
;1636:
;1637:	memset (client, 0, sizeof(*client)); // bk FIXME: Com_Memset?
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1756
ARGI4
ADDRGP4 memset
CALLP4
pop
line 1640
;1638:
;1639:	//rww - Don't wipe the ghoul2 instance or the animation data
;1640:	client->ghoul2 = ghoul2save;
ADDRLP4 0
INDIRP4
CNSTI4 1716
ADDP4
ADDRLP4 800
INDIRP4
ASGNP4
line 1643
;1641:
;1642:	//or the saber ent num
;1643:	client->ps.saberEntityNum = saveSaberNum;
ADDRLP4 0
INDIRP4
CNSTI4 544
ADDP4
ADDRLP4 112
INDIRI4
ASGNI4
line 1645
;1644:
;1645:	client->ps.fd = savedForce;
ADDRLP4 0
INDIRP4
CNSTI4 760
ADDP4
ADDRLP4 328
INDIRB
ASGNB 472
line 1647
;1646:
;1647:	client->ps.duelIndex = ENTITYNUM_NONE;
ADDRLP4 0
INDIRP4
CNSTI4 1284
ADDP4
CNSTI4 1023
ASGNI4
line 1649
;1648:
;1649:	client->pers = saved;
ADDRLP4 0
INDIRP4
CNSTI4 1368
ADDP4
ADDRLP4 116
INDIRB
ASGNB 152
line 1650
;1650:	client->sess = savedSess;
ADDRLP4 0
INDIRP4
CNSTI4 1520
ADDP4
ADDRLP4 268
INDIRB
ASGNB 44
line 1651
;1651:	client->ps.ping = savedPing;
ADDRLP4 0
INDIRP4
CNSTI4 484
ADDP4
ADDRLP4 312
INDIRI4
ASGNI4
line 1653
;1652://	client->areabits = savedAreaBits;
;1653:	client->accuracy_hits = accuracy_hits;
ADDRLP4 0
INDIRP4
CNSTI4 1656
ADDP4
ADDRLP4 316
INDIRI4
ASGNI4
line 1654
;1654:	client->accuracy_shots = accuracy_shots;
ADDRLP4 0
INDIRP4
CNSTI4 1652
ADDP4
ADDRLP4 320
INDIRI4
ASGNI4
line 1655
;1655:	client->lastkilled_client = -1;
ADDRLP4 0
INDIRP4
CNSTI4 1660
ADDP4
CNSTI4 -1
ASGNI4
line 1657
;1656:
;1657:	for ( i = 0 ; i < MAX_PERSISTANT ; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $556
line 1658
;1658:		client->ps.persistant[i] = persistant[i];
ADDRLP4 804
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 804
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 280
ADDP4
ADDP4
ADDRLP4 804
INDIRI4
ADDRLP4 8
ADDP4
INDIRI4
ASGNI4
line 1659
;1659:	}
LABELV $557
line 1657
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 16
LTI4 $556
line 1660
;1660:	client->ps.eventSequence = eventSequence;
ADDRLP4 0
INDIRP4
CNSTI4 112
ADDP4
ADDRLP4 324
INDIRI4
ASGNI4
line 1662
;1661:	// increment the spawncount so the client will detect the respawn
;1662:	client->ps.persistant[PERS_SPAWN_COUNT]++;
ADDRLP4 804
ADDRLP4 0
INDIRP4
CNSTI4 296
ADDP4
ASGNP4
ADDRLP4 804
INDIRP4
ADDRLP4 804
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1663
;1663:	client->ps.persistant[PERS_TEAM] = client->sess.sessionTeam;
ADDRLP4 0
INDIRP4
CNSTI4 292
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ASGNI4
line 1665
;1664:
;1665:	client->airOutTime = level.time + 12000;
ADDRLP4 0
INDIRP4
CNSTI4 1688
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 12000
ADDI4
ASGNI4
line 1669
;1666:
;1667://	trap_GetUserinfo( index, userinfo, sizeof(userinfo) );
;1668:	// set max health
;1669:	client->pers.maxHealth = 100;//atoi( Info_ValueForKey( userinfo, "handicap" ) );
ADDRLP4 0
INDIRP4
CNSTI4 1452
ADDP4
CNSTI4 100
ASGNI4
line 1670
;1670:	if ( client->pers.maxHealth < 1 || client->pers.maxHealth > 100 ) {
ADDRLP4 812
ADDRLP4 0
INDIRP4
CNSTI4 1452
ADDP4
INDIRI4
ASGNI4
ADDRLP4 812
INDIRI4
CNSTI4 1
LTI4 $563
ADDRLP4 812
INDIRI4
CNSTI4 100
LEI4 $561
LABELV $563
line 1671
;1671:		client->pers.maxHealth = 100;
ADDRLP4 0
INDIRP4
CNSTI4 1452
ADDP4
CNSTI4 100
ASGNI4
line 1672
;1672:	}
LABELV $561
line 1674
;1673:	// clear entity values
;1674:	client->ps.stats[STAT_MAX_HEALTH] = client->pers.maxHealth;
ADDRLP4 0
INDIRP4
CNSTI4 248
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 1452
ADDP4
INDIRI4
ASGNI4
line 1675
;1675:	client->ps.eFlags = flags;
ADDRLP4 0
INDIRP4
CNSTI4 108
ADDP4
ADDRLP4 108
INDIRI4
ASGNI4
line 1677
;1676:
;1677:	ent->s.groundEntityNum = ENTITYNUM_NONE;
ADDRFP4 0
INDIRP4
CNSTI4 192
ADDP4
CNSTI4 1023
ASGNI4
line 1678
;1678:	ent->client = &level.clients[index];
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
CNSTI4 1756
ADDRLP4 104
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 1679
;1679:	ent->takedamage = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 1
ASGNI4
line 1680
;1680:	ent->inuse = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 412
ADDP4
CNSTI4 1
ASGNI4
line 1681
;1681:	ent->classname = "player";
ADDRFP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $564
ASGNP4
line 1682
;1682:	ent->r.contents = CONTENTS_BODY;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 256
ASGNI4
line 1683
;1683:	ent->clipmask = MASK_PLAYERSOLID;
ADDRFP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 273
ASGNI4
line 1684
;1684:	ent->die = player_die;
ADDRFP4 0
INDIRP4
CNSTI4 660
ADDP4
ADDRGP4 player_die
ASGNP4
line 1685
;1685:	ent->waterlevel = 0;
ADDRFP4 0
INDIRP4
CNSTI4 748
ADDP4
CNSTI4 0
ASGNI4
line 1686
;1686:	ent->watertype = 0;
ADDRFP4 0
INDIRP4
CNSTI4 744
ADDP4
CNSTI4 0
ASGNI4
line 1687
;1687:	ent->flags = 0;
ADDRFP4 0
INDIRP4
CNSTI4 472
ADDP4
CNSTI4 0
ASGNI4
line 1689
;1688:	
;1689:	VectorCopy (playerMins, ent->r.mins);
ADDRFP4 0
INDIRP4
CNSTI4 316
ADDP4
ADDRGP4 playerMins
INDIRB
ASGNB 12
line 1690
;1690:	VectorCopy (playerMaxs, ent->r.maxs);
ADDRFP4 0
INDIRP4
CNSTI4 328
ADDP4
ADDRGP4 playerMaxs
INDIRB
ASGNB 12
line 1692
;1691:
;1692:	client->ps.clientNum = index;
ADDRLP4 0
INDIRP4
CNSTI4 144
ADDP4
ADDRLP4 104
INDIRI4
ASGNI4
line 1694
;1693:	//give default weapons
;1694:	client->ps.stats[STAT_WEAPONS] = ( 1 << WP_NONE );
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
CNSTI4 1
ASGNI4
line 1696
;1695:
;1696:	if (g_gametype.integer == GT_HOLOCRON)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 1
NEI4 $565
line 1697
;1697:	{
line 1699
;1698:		//always get free saber level 1 in holocron
;1699:		client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_SABER );	//these are precached in g_items, ClearRegisteredItems()
ADDRLP4 820
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
ASGNP4
ADDRLP4 820
INDIRP4
ADDRLP4 820
INDIRP4
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 1700
;1700:	}
ADDRGP4 $566
JUMPV
LABELV $565
line 1702
;1701:	else
;1702:	{
line 1703
;1703:		if (client->ps.fd.forcePowerLevel[FP_SABERATTACK])
ADDRLP4 0
INDIRP4
CNSTI4 992
ADDP4
INDIRI4
CNSTI4 0
EQI4 $568
line 1704
;1704:		{
line 1705
;1705:			client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_SABER );	//these are precached in g_items, ClearRegisteredItems()
ADDRLP4 820
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
ASGNP4
ADDRLP4 820
INDIRP4
ADDRLP4 820
INDIRP4
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 1706
;1706:		}
ADDRGP4 $569
JUMPV
LABELV $568
line 1708
;1707:		else
;1708:		{ //if you don't have saber attack rank then you don't get a saber
line 1709
;1709:			client->ps.stats[STAT_WEAPONS] |= (1 << WP_STUN_BATON);
ADDRLP4 820
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
ASGNP4
ADDRLP4 820
INDIRP4
ADDRLP4 820
INDIRP4
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 1710
;1710:		}
LABELV $569
line 1711
;1711:	}
LABELV $566
line 1713
;1712:
;1713:	if (g_gametype.integer == GT_TOURNAMENT)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
NEI4 $570
line 1714
;1714:	{
line 1715
;1715:		wDisable = g_duelWeaponDisable.integer;
ADDRLP4 88
ADDRGP4 g_duelWeaponDisable+12
INDIRI4
ASGNI4
line 1716
;1716:	}
ADDRGP4 $571
JUMPV
LABELV $570
line 1718
;1717:	else
;1718:	{
line 1719
;1719:		wDisable = g_weaponDisable.integer;
ADDRLP4 88
ADDRGP4 g_weaponDisable+12
INDIRI4
ASGNI4
line 1720
;1720:	}
LABELV $571
line 1722
;1721:
;1722:	if (!wDisable || !(wDisable & (1 << WP_BRYAR_PISTOL)))
ADDRLP4 824
CNSTI4 0
ASGNI4
ADDRLP4 88
INDIRI4
ADDRLP4 824
INDIRI4
EQI4 $577
ADDRLP4 88
INDIRI4
CNSTI4 8
BANDI4
ADDRLP4 824
INDIRI4
NEI4 $575
LABELV $577
line 1723
;1723:	{
line 1724
;1724:		client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_BRYAR_PISTOL );
ADDRLP4 828
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
ASGNP4
ADDRLP4 828
INDIRP4
ADDRLP4 828
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 1725
;1725:	}
ADDRGP4 $576
JUMPV
LABELV $575
line 1726
;1726:	else if (g_gametype.integer == GT_JEDIMASTER)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 2
NEI4 $578
line 1727
;1727:	{
line 1728
;1728:		client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_BRYAR_PISTOL );
ADDRLP4 828
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
ASGNP4
ADDRLP4 828
INDIRP4
ADDRLP4 828
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 1729
;1729:	}
LABELV $578
LABELV $576
line 1731
;1730:
;1731:	if (g_gametype.integer == GT_JEDIMASTER)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 2
NEI4 $581
line 1732
;1732:	{
line 1733
;1733:		client->ps.stats[STAT_WEAPONS] &= ~(1 << WP_SABER);
ADDRLP4 828
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
ASGNP4
ADDRLP4 828
INDIRP4
ADDRLP4 828
INDIRP4
INDIRI4
CNSTI4 -5
BANDI4
ASGNI4
line 1734
;1734:		client->ps.stats[STAT_WEAPONS] |= (1 << WP_STUN_BATON);
ADDRLP4 832
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
ASGNP4
ADDRLP4 832
INDIRP4
ADDRLP4 832
INDIRP4
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 1735
;1735:	}
LABELV $581
line 1737
;1736:
;1737:	if (client->ps.stats[STAT_WEAPONS] & (1 << WP_BRYAR_PISTOL))
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $584
line 1738
;1738:	{
line 1739
;1739:		client->ps.weapon = WP_BRYAR_PISTOL;
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 3
ASGNI4
line 1740
;1740:	}
ADDRGP4 $585
JUMPV
LABELV $584
line 1741
;1741:	else if (client->ps.stats[STAT_WEAPONS] & (1 << WP_SABER))
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $586
line 1742
;1742:	{
line 1743
;1743:		client->ps.weapon = WP_SABER;
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 2
ASGNI4
line 1744
;1744:	}
ADDRGP4 $587
JUMPV
LABELV $586
line 1746
;1745:	else
;1746:	{
line 1747
;1747:		client->ps.weapon = WP_STUN_BATON;
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 1
ASGNI4
line 1748
;1748:	}
LABELV $587
LABELV $585
line 1755
;1749:
;1750:	/*
;1751:	client->ps.stats[STAT_HOLDABLE_ITEMS] |= ( 1 << HI_BINOCULARS );
;1752:	client->ps.stats[STAT_HOLDABLE_ITEM] = BG_GetItemIndexByTag(HI_BINOCULARS, IT_HOLDABLE);
;1753:	*/
;1754:
;1755:	client->ps.stats[STAT_HOLDABLE_ITEMS] = 0;
ADDRLP4 0
INDIRP4
CNSTI4 224
ADDP4
CNSTI4 0
ASGNI4
line 1756
;1756:	client->ps.stats[STAT_HOLDABLE_ITEM] = 0;
ADDRLP4 0
INDIRP4
CNSTI4 220
ADDP4
CNSTI4 0
ASGNI4
line 1758
;1757:
;1758:	if ( client->sess.sessionTeam == TEAM_SPECTATOR )
ADDRLP4 0
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
NEI4 $588
line 1759
;1759:	{
line 1760
;1760:		client->ps.stats[STAT_WEAPONS] = 0;
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
CNSTI4 0
ASGNI4
line 1761
;1761:		client->ps.stats[STAT_HOLDABLE_ITEMS] = 0;
ADDRLP4 0
INDIRP4
CNSTI4 224
ADDP4
CNSTI4 0
ASGNI4
line 1762
;1762:		client->ps.stats[STAT_HOLDABLE_ITEM] = 0;
ADDRLP4 0
INDIRP4
CNSTI4 220
ADDP4
CNSTI4 0
ASGNI4
line 1763
;1763:	}
LABELV $588
line 1765
;1764:
;1765:	client->ps.ammo[AMMO_BLASTER] = 100; //ammoData[AMMO_BLASTER].max; //100 seems fair.
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
CNSTI4 100
ASGNI4
line 1778
;1766://	client->ps.ammo[AMMO_POWERCELL] = ammoData[AMMO_POWERCELL].max;
;1767://	client->ps.ammo[AMMO_FORCE] = ammoData[AMMO_FORCE].max;
;1768://	client->ps.ammo[AMMO_METAL_BOLTS] = ammoData[AMMO_METAL_BOLTS].max;
;1769://	client->ps.ammo[AMMO_ROCKETS] = ammoData[AMMO_ROCKETS].max;
;1770:/*
;1771:	client->ps.stats[STAT_WEAPONS] = ( 1 << WP_BRYAR_PISTOL);
;1772:	if ( g_gametype.integer == GT_TEAM ) {
;1773:		client->ps.ammo[WP_BRYAR_PISTOL] = 50;
;1774:	} else {
;1775:		client->ps.ammo[WP_BRYAR_PISTOL] = 100;
;1776:	}
;1777:*/
;1778:	client->ps.rocketLockIndex = MAX_CLIENTS;
ADDRLP4 0
INDIRP4
CNSTI4 580
ADDP4
CNSTI4 32
ASGNI4
line 1779
;1779:	client->ps.rocketLockTime = 0;
ADDRLP4 0
INDIRP4
CNSTI4 588
ADDP4
CNSTF4 0
ASGNF4
line 1790
;1780:
;1781:	//rww - Set here to initialize the circling seeker drone to off.
;1782:	//A quick note about this so I don't forget how it works again:
;1783:	//ps.genericEnemyIndex is kept in sync between the server and client.
;1784:	//When it gets set then an entitystate value of the same name gets
;1785:	//set along with an entitystate flag in the shared bg code. Which
;1786:	//is why a value needs to be both on the player state and entity state.
;1787:	//(it doesn't seem to just carry over the entitystate value automatically
;1788:	//because entity state value is derived from player state data or some
;1789:	//such)
;1790:	client->ps.genericEnemyIndex = -1;
ADDRLP4 0
INDIRP4
CNSTI4 612
ADDP4
CNSTI4 -1
ASGNI4
line 1792
;1791:
;1792:	client->ps.isJediMaster = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 604
ADDP4
CNSTI4 0
ASGNI4
line 1794
;1793:
;1794:	client->ps.fallingToDeath = 0;
ADDRLP4 0
INDIRP4
CNSTI4 1340
ADDP4
CNSTI4 0
ASGNI4
line 1797
;1795:
;1796:	//Do per-spawn force power initialization
;1797:	WP_SpawnInitForcePowers( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 WP_SpawnInitForcePowers
CALLV
pop
line 1800
;1798:
;1799:	// health will count down towards max_health
;1800:	ent->health = client->ps.stats[STAT_HEALTH] = client->ps.stats[STAT_MAX_HEALTH] * 1.25;
ADDRLP4 832
CNSTF4 1067450368
ADDRLP4 0
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 216
ADDP4
ADDRLP4 832
INDIRI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
ADDRLP4 832
INDIRI4
ASGNI4
line 1803
;1801:
;1802:	// Start with a small amount of armor as well.
;1803:	client->ps.stats[STAT_ARMOR] = client->ps.stats[STAT_MAX_HEALTH] * 0.25;
ADDRLP4 0
INDIRP4
CNSTI4 236
ADDP4
CNSTF4 1048576000
ADDRLP4 0
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 1805
;1804:
;1805:	G_SetOrigin( ent, spawn_origin );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 76
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1806
;1806:	VectorCopy( spawn_origin, client->ps.origin );
ADDRLP4 0
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 76
INDIRB
ASGNB 12
line 1809
;1807:
;1808:	// the respawned flag will be cleared after the attack and jump keys come up
;1809:	client->ps.pm_flags |= PMF_RESPAWNED;
ADDRLP4 840
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 840
INDIRP4
ADDRLP4 840
INDIRP4
INDIRI4
CNSTI4 512
BORI4
ASGNI4
line 1811
;1810:
;1811:	trap_GetUsercmd( client - level.clients, &ent->client->pers.cmd );
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRGP4 level
INDIRP4
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 1756
DIVI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1372
ADDP4
ARGP4
ADDRGP4 trap_GetUsercmd
CALLV
pop
line 1812
;1812:	SetClientViewAngle( ent, spawn_angles );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 92
ARGP4
ADDRGP4 SetClientViewAngle
CALLV
pop
line 1814
;1813:
;1814:	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
NEI4 $590
line 1816
;1815:
;1816:	} else {
ADDRGP4 $591
JUMPV
LABELV $590
line 1817
;1817:		G_KillBox( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_KillBox
CALLV
pop
line 1818
;1818:		trap_LinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1821
;1819:
;1820:		// force the base weapon up
;1821:		client->ps.weapon = WP_BRYAR_PISTOL;
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 3
ASGNI4
line 1822
;1822:		client->ps.weaponstate = FIRST_WEAPON;
ADDRLP4 0
INDIRP4
CNSTI4 152
ADDP4
CNSTI4 3
ASGNI4
line 1824
;1823:
;1824:	}
LABELV $591
line 1827
;1825:
;1826:	// don't allow full run speed for a bit
;1827:	client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
ADDRLP4 844
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 844
INDIRP4
ADDRLP4 844
INDIRP4
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 1828
;1828:	client->ps.pm_time = 100;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
CNSTI4 100
ASGNI4
line 1830
;1829:
;1830:	client->respawnTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 1672
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1831
;1831:	client->inactivityTime = level.time + g_inactivity.integer * 1000;
ADDRLP4 0
INDIRP4
CNSTI4 1676
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1000
ADDRGP4 g_inactivity+12
INDIRI4
MULI4
ADDI4
ASGNI4
line 1832
;1832:	client->latched_buttons = 0;
ADDRLP4 0
INDIRP4
CNSTI4 1592
ADDP4
CNSTI4 0
ASGNI4
line 1835
;1833:
;1834:	// set default animations
;1835:	client->ps.torsoAnim = WeaponReadyAnim[client->ps.weapon];
ADDRLP4 0
INDIRP4
CNSTI4 100
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 WeaponReadyAnim
ADDP4
INDIRI4
ASGNI4
line 1836
;1836:	client->ps.legsAnim = WeaponReadyAnim[client->ps.weapon];
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 WeaponReadyAnim
ADDP4
INDIRI4
ASGNI4
line 1838
;1837:
;1838:	if ( level.intermissiontime ) {
ADDRGP4 level+9008
INDIRI4
CNSTI4 0
EQI4 $595
line 1839
;1839:		MoveClientToIntermission( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 MoveClientToIntermission
CALLV
pop
line 1840
;1840:	} else {
ADDRGP4 $596
JUMPV
LABELV $595
line 1842
;1841:		// fire the targets of the spawn point
;1842:		G_UseTargets( spawnPoint, ent );
ADDRLP4 72
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
line 1846
;1843:
;1844:		// select the highest weapon number available, after any
;1845:		// spawn given items have fired
;1846:		client->ps.weapon = 1;
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 1
ASGNI4
line 1847
;1847:		for ( i = WP_NUM_WEAPONS - 1 ; i > 0 ; i-- ) {
ADDRLP4 4
CNSTI4 15
ASGNI4
LABELV $598
line 1848
;1848:			if ( client->ps.stats[STAT_WEAPONS] & ( 1 << i ) ) {
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
INDIRI4
CNSTI4 1
ADDRLP4 4
INDIRI4
LSHI4
BANDI4
CNSTI4 0
EQI4 $602
line 1849
;1849:				client->ps.weapon = i;
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 1850
;1850:				break;
ADDRGP4 $600
JUMPV
LABELV $602
line 1852
;1851:			}
;1852:		}
LABELV $599
line 1847
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
GTI4 $598
LABELV $600
line 1853
;1853:	}
LABELV $596
line 1857
;1854:
;1855:	// run a client frame to drop exactly to the floor,
;1856:	// initialize animations and other things
;1857:	client->ps.commandTime = level.time - 100;
ADDRLP4 0
INDIRP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
SUBI4
ASGNI4
line 1858
;1858:	ent->client->pers.cmd.serverTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1372
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1859
;1859:	ClientThink( ent-g_entities );
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRGP4 g_entities
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 828
DIVI4
ARGI4
ADDRGP4 ClientThink
CALLV
pop
line 1862
;1860:
;1861:	// positively link the client, even if the command times are weird
;1862:	if ( ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
EQI4 $606
line 1863
;1863:		BG_PlayerStateToEntityState( &client->ps, &ent->s, qtrue );
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityState
CALLV
pop
line 1864
;1864:		VectorCopy( ent->client->ps.origin, ent->r.currentOrigin );
ADDRLP4 856
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 856
INDIRP4
CNSTI4 368
ADDP4
ADDRLP4 856
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 1865
;1865:		trap_LinkEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1866
;1866:	}
LABELV $606
line 1868
;1867:
;1868:	if (g_spawnInvulnerability.integer)
ADDRGP4 g_spawnInvulnerability+12
INDIRI4
CNSTI4 0
EQI4 $608
line 1869
;1869:	{
line 1870
;1870:		ent->client->ps.eFlags |= EF_INVULNERABLE;
ADDRLP4 856
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 856
INDIRP4
ADDRLP4 856
INDIRP4
INDIRI4
CNSTI4 67108864
BORI4
ASGNI4
line 1871
;1871:		ent->client->invulnerableTimer = level.time + g_spawnInvulnerability.integer;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1564
ADDP4
ADDRGP4 level+32
INDIRI4
ADDRGP4 g_spawnInvulnerability+12
INDIRI4
ADDI4
ASGNI4
line 1872
;1872:	}
LABELV $608
line 1875
;1873:
;1874:	// run the presend to set anything else
;1875:	ClientEndFrame( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 ClientEndFrame
CALLV
pop
line 1878
;1876:
;1877:	// clear entity state values
;1878:	BG_PlayerStateToEntityState( &client->ps, &ent->s, qtrue );
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityState
CALLV
pop
line 1879
;1879:}
LABELV $530
endproc ClientSpawn 860 16
export ClientDisconnect
proc ClientDisconnect 24 8
line 1894
;1880:
;1881:
;1882:/*
;1883:===========
;1884:ClientDisconnect
;1885:
;1886:Called when a player drops from the server.
;1887:Will not be called between levels.
;1888:
;1889:This should NOT be called directly by any game logic,
;1890:call trap_DropClient(), which will call this and do
;1891:server system housekeeping.
;1892:============
;1893:*/
;1894:void ClientDisconnect( int clientNum ) {
line 1901
;1895:	gentity_t	*ent;
;1896:	gentity_t	*tent;
;1897:	int			i;
;1898:
;1899:	// cleanup if we are kicking a bot that
;1900:	// hasn't spawned yet
;1901:	G_RemoveQueuedBotBegin( clientNum );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 G_RemoveQueuedBotBegin
CALLV
pop
line 1903
;1902:
;1903:	ent = g_entities + clientNum;
ADDRLP4 4
CNSTI4 828
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1904
;1904:	if ( !ent->client ) {
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $614
line 1905
;1905:		return;
ADDRGP4 $613
JUMPV
LABELV $614
line 1908
;1906:	}
;1907:
;1908:	i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $617
JUMPV
LABELV $616
line 1911
;1909:
;1910:	while (i < NUM_FORCE_POWERS)
;1911:	{
line 1912
;1912:		if (ent->client->ps.fd.forcePowersActive & (1 << i))
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 836
ADDP4
INDIRI4
CNSTI4 1
ADDRLP4 0
INDIRI4
LSHI4
BANDI4
CNSTI4 0
EQI4 $619
line 1913
;1913:		{
line 1914
;1914:			WP_ForcePowerStop(ent, i);
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 WP_ForcePowerStop
CALLV
pop
line 1915
;1915:		}
LABELV $619
line 1916
;1916:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1917
;1917:	}
LABELV $617
line 1910
ADDRLP4 0
INDIRI4
CNSTI4 18
LTI4 $616
line 1919
;1918:
;1919:	i = TRACK_CHANNEL_1;
ADDRLP4 0
CNSTI4 51
ASGNI4
ADDRGP4 $622
JUMPV
LABELV $621
line 1922
;1920:
;1921:	while (i < NUM_TRACK_CHANNELS)
;1922:	{
line 1923
;1923:		if (ent->client->ps.fd.killSoundEntIndex[i-50] && ent->client->ps.fd.killSoundEntIndex[i-50] < MAX_GENTITIES && ent->client->ps.fd.killSoundEntIndex[i-50] > 0)
ADDRLP4 12
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 200
SUBI4
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1188
ADDP4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 16
CNSTI4 0
ASGNI4
ADDRLP4 12
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $624
ADDRLP4 12
INDIRI4
CNSTI4 1024
GEI4 $624
ADDRLP4 12
INDIRI4
ADDRLP4 16
INDIRI4
LEI4 $624
line 1924
;1924:		{
line 1925
;1925:			G_MuteSound(ent->client->ps.fd.killSoundEntIndex[i-50], CHAN_VOICE);
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 200
SUBI4
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1188
ADDP4
ADDP4
INDIRI4
ARGI4
CNSTI4 3
ARGI4
ADDRGP4 G_MuteSound
CALLV
pop
line 1926
;1926:		}
LABELV $624
line 1927
;1927:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1928
;1928:	}
LABELV $622
line 1921
ADDRLP4 0
INDIRI4
CNSTI4 56
LTI4 $621
line 1929
;1929:	i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1932
;1930:
;1931:	// stop any following clients
;1932:	for ( i = 0 ; i < level.maxclients ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $629
JUMPV
LABELV $626
line 1933
;1933:		if ( level.clients[i].sess.sessionTeam == TEAM_SPECTATOR
ADDRLP4 12
CNSTI4 1756
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
NEI4 $631
ADDRLP4 12
INDIRP4
CNSTI4 1528
ADDP4
INDIRI4
CNSTI4 2
NEI4 $631
ADDRLP4 12
INDIRP4
CNSTI4 1532
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $631
line 1935
;1934:			&& level.clients[i].sess.spectatorState == SPECTATOR_FOLLOW
;1935:			&& level.clients[i].sess.spectatorClient == clientNum ) {
line 1936
;1936:			StopFollowing( &g_entities[i] );
CNSTI4 828
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRGP4 StopFollowing
CALLV
pop
line 1937
;1937:		}
LABELV $631
line 1938
;1938:	}
LABELV $627
line 1932
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $629
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $626
line 1941
;1939:
;1940:	// send effect if they were completely connected
;1941:	if ( ent->client->pers.connected == CON_CONNECTED 
ADDRLP4 12
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 1368
ADDP4
INDIRI4
CNSTI4 2
NEI4 $633
ADDRLP4 12
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
EQI4 $633
line 1942
;1942:		&& ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
line 1943
;1943:		tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_OUT );
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
CNSTI4 60
ARGI4
ADDRLP4 16
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 16
INDIRP4
ASGNP4
line 1944
;1944:		tent->s.clientNum = ent->s.clientNum;
ADDRLP4 20
CNSTI4 220
ASGNI4
ADDRLP4 8
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
ADDRLP4 4
INDIRP4
ADDRLP4 20
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 1948
;1945:
;1946:		// They don't get to take powerups with them!
;1947:		// Especially important for stuff like CTF flags
;1948:		TossClientItems( ent );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 TossClientItems
CALLV
pop
line 1949
;1949:	}
LABELV $633
line 1951
;1950:
;1951:	G_LogPrintf( "ClientDisconnect: %i\n", clientNum );
ADDRGP4 $635
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 G_LogPrintf
CALLV
pop
line 1954
;1952:
;1953:	// if we are playing in tourney mode and losing, give a win to the other player
;1954:	if ( (g_gametype.integer == GT_TOURNAMENT )
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
NEI4 $636
ADDRLP4 16
CNSTI4 0
ASGNI4
ADDRGP4 level+9008
INDIRI4
ADDRLP4 16
INDIRI4
NEI4 $636
ADDRGP4 level+16
INDIRI4
ADDRLP4 16
INDIRI4
NEI4 $636
ADDRGP4 level+84+4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $636
line 1956
;1955:		&& !level.intermissiontime
;1956:		&& !level.warmupTime && level.sortedClients[1] == clientNum ) {
line 1957
;1957:		level.clients[ level.sortedClients[0] ].sess.wins++;
ADDRLP4 20
CNSTI4 1756
ADDRGP4 level+84
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 1536
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1958
;1958:		ClientUserinfoChanged( level.sortedClients[0] );
ADDRGP4 level+84
INDIRI4
ARGI4
ADDRGP4 ClientUserinfoChanged
CALLV
pop
line 1959
;1959:	}
LABELV $636
line 1961
;1960:
;1961:	trap_UnlinkEntity (ent);
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 1962
;1962:	ent->s.modelindex = 0;
ADDRLP4 4
INDIRP4
CNSTI4 212
ADDP4
CNSTI4 0
ASGNI4
line 1963
;1963:	ent->inuse = qfalse;
ADDRLP4 4
INDIRP4
CNSTI4 412
ADDP4
CNSTI4 0
ASGNI4
line 1964
;1964:	ent->classname = "disconnected";
ADDRLP4 4
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $645
ASGNP4
line 1965
;1965:	ent->client->pers.connected = CON_DISCONNECTED;
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1368
ADDP4
CNSTI4 0
ASGNI4
line 1966
;1966:	ent->client->ps.persistant[PERS_TEAM] = TEAM_FREE;
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 292
ADDP4
CNSTI4 0
ASGNI4
line 1967
;1967:	ent->client->sess.sessionTeam = TEAM_FREE;
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
CNSTI4 0
ASGNI4
line 1969
;1968:
;1969:	trap_SetConfigstring( CS_PLAYERS + clientNum, "");
ADDRFP4 0
INDIRI4
CNSTI4 544
ADDI4
ARGI4
ADDRGP4 $646
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 1971
;1970:
;1971:	CalculateRanks();
ADDRGP4 CalculateRanks
CALLV
pop
line 1973
;1972:
;1973:	if ( ent->r.svFlags & SVF_BOT ) {
ADDRLP4 4
INDIRP4
CNSTI4 304
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $647
line 1974
;1974:		BotAIShutdownClient( clientNum, qfalse );
ADDRFP4 0
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 BotAIShutdownClient
CALLI4
pop
line 1975
;1975:	}
LABELV $647
line 1977
;1976:
;1977:	G_ClearClientLog(clientNum);
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 G_ClearClientLog
CALLV
pop
line 1978
;1978:}
LABELV $613
endproc ClientDisconnect 24 8
import G_WriteClientSessionData
bss
export Client_Force
align 4
LABELV Client_Force
skip 15104
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
import CalculateRanks
import AddScore
import player_die
import BeginIntermission
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
LABELV $646
char 1 0
align 1
LABELV $645
char 1 100
char 1 105
char 1 115
char 1 99
char 1 111
char 1 110
char 1 110
char 1 101
char 1 99
char 1 116
char 1 101
char 1 100
char 1 0
align 1
LABELV $635
char 1 67
char 1 108
char 1 105
char 1 101
char 1 110
char 1 116
char 1 68
char 1 105
char 1 115
char 1 99
char 1 111
char 1 110
char 1 110
char 1 101
char 1 99
char 1 116
char 1 58
char 1 32
char 1 37
char 1 105
char 1 10
char 1 0
align 1
LABELV $564
char 1 112
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 0
align 1
LABELV $529
char 1 67
char 1 108
char 1 105
char 1 101
char 1 110
char 1 116
char 1 66
char 1 101
char 1 103
char 1 105
char 1 110
char 1 58
char 1 32
char 1 37
char 1 105
char 1 10
char 1 0
align 1
LABELV $528
char 1 80
char 1 76
char 1 69
char 1 78
char 1 84
char 1 69
char 1 82
char 1 0
align 1
LABELV $522
char 1 77
char 1 111
char 1 116
char 1 105
char 1 111
char 1 110
char 1 0
align 1
LABELV $521
char 1 114
char 1 116
char 1 105
char 1 98
char 1 105
char 1 97
char 1 0
align 1
LABELV $520
char 1 108
char 1 116
char 1 105
char 1 98
char 1 105
char 1 97
char 1 0
align 1
LABELV $519
char 1 114
char 1 114
char 1 97
char 1 100
char 1 105
char 1 117
char 1 115
char 1 0
align 1
LABELV $518
char 1 108
char 1 114
char 1 97
char 1 100
char 1 105
char 1 117
char 1 115
char 1 0
align 1
LABELV $517
char 1 116
char 1 104
char 1 111
char 1 114
char 1 97
char 1 99
char 1 105
char 1 99
char 1 0
align 1
LABELV $501
char 1 66
char 1 108
char 1 117
char 1 101
char 1 0
align 1
LABELV $496
char 1 82
char 1 101
char 1 100
char 1 0
align 1
LABELV $486
char 1 80
char 1 76
char 1 67
char 1 79
char 1 78
char 1 78
char 1 69
char 1 67
char 1 84
char 1 0
align 1
LABELV $485
char 1 112
char 1 114
char 1 105
char 1 110
char 1 116
char 1 32
char 1 34
char 1 37
char 1 115
char 1 94
char 1 55
char 1 32
char 1 37
char 1 115
char 1 10
char 1 34
char 1 0
align 1
LABELV $482
char 1 67
char 1 108
char 1 105
char 1 101
char 1 110
char 1 116
char 1 67
char 1 111
char 1 110
char 1 110
char 1 101
char 1 99
char 1 116
char 1 58
char 1 32
char 1 37
char 1 105
char 1 10
char 1 0
align 1
LABELV $481
char 1 66
char 1 111
char 1 116
char 1 67
char 1 111
char 1 110
char 1 110
char 1 101
char 1 99
char 1 116
char 1 102
char 1 97
char 1 105
char 1 108
char 1 101
char 1 100
char 1 0
align 1
LABELV $469
char 1 73
char 1 110
char 1 118
char 1 97
char 1 108
char 1 105
char 1 100
char 1 32
char 1 112
char 1 97
char 1 115
char 1 115
char 1 119
char 1 111
char 1 114
char 1 100
char 1 0
align 1
LABELV $467
char 1 110
char 1 111
char 1 110
char 1 101
char 1 0
align 1
LABELV $462
char 1 112
char 1 97
char 1 115
char 1 115
char 1 119
char 1 111
char 1 114
char 1 100
char 1 0
align 1
LABELV $458
char 1 66
char 1 97
char 1 110
char 1 110
char 1 101
char 1 100
char 1 46
char 1 0
align 1
LABELV $454
char 1 67
char 1 108
char 1 105
char 1 101
char 1 110
char 1 116
char 1 85
char 1 115
char 1 101
char 1 114
char 1 105
char 1 110
char 1 102
char 1 111
char 1 67
char 1 104
char 1 97
char 1 110
char 1 103
char 1 101
char 1 100
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
LABELV $453
char 1 110
char 1 92
char 1 37
char 1 115
char 1 92
char 1 116
char 1 92
char 1 37
char 1 105
char 1 92
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 92
char 1 37
char 1 115
char 1 92
char 1 103
char 1 95
char 1 114
char 1 101
char 1 100
char 1 116
char 1 101
char 1 97
char 1 109
char 1 92
char 1 37
char 1 115
char 1 92
char 1 103
char 1 95
char 1 98
char 1 108
char 1 117
char 1 101
char 1 116
char 1 101
char 1 97
char 1 109
char 1 92
char 1 37
char 1 115
char 1 92
char 1 99
char 1 49
char 1 92
char 1 37
char 1 115
char 1 92
char 1 99
char 1 50
char 1 92
char 1 37
char 1 115
char 1 92
char 1 104
char 1 99
char 1 92
char 1 37
char 1 105
char 1 92
char 1 119
char 1 92
char 1 37
char 1 105
char 1 92
char 1 108
char 1 92
char 1 37
char 1 105
char 1 92
char 1 116
char 1 116
char 1 92
char 1 37
char 1 100
char 1 92
char 1 116
char 1 108
char 1 92
char 1 37
char 1 100
char 1 0
align 1
LABELV $452
char 1 115
char 1 107
char 1 105
char 1 108
char 1 108
char 1 0
align 1
LABELV $451
char 1 110
char 1 92
char 1 37
char 1 115
char 1 92
char 1 116
char 1 92
char 1 37
char 1 105
char 1 92
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 92
char 1 37
char 1 115
char 1 92
char 1 99
char 1 49
char 1 92
char 1 37
char 1 115
char 1 92
char 1 99
char 1 50
char 1 92
char 1 37
char 1 115
char 1 92
char 1 104
char 1 99
char 1 92
char 1 37
char 1 105
char 1 92
char 1 119
char 1 92
char 1 37
char 1 105
char 1 92
char 1 108
char 1 92
char 1 37
char 1 105
char 1 92
char 1 115
char 1 107
char 1 105
char 1 108
char 1 108
char 1 92
char 1 37
char 1 115
char 1 92
char 1 116
char 1 116
char 1 92
char 1 37
char 1 100
char 1 92
char 1 116
char 1 108
char 1 92
char 1 37
char 1 100
char 1 0
align 1
LABELV $448
char 1 103
char 1 95
char 1 98
char 1 108
char 1 117
char 1 101
char 1 116
char 1 101
char 1 97
char 1 109
char 1 0
align 1
LABELV $447
char 1 103
char 1 95
char 1 114
char 1 101
char 1 100
char 1 116
char 1 101
char 1 97
char 1 109
char 1 0
align 1
LABELV $446
char 1 99
char 1 111
char 1 108
char 1 111
char 1 114
char 1 50
char 1 0
align 1
LABELV $445
char 1 99
char 1 111
char 1 108
char 1 111
char 1 114
char 1 49
char 1 0
align 1
LABELV $444
char 1 116
char 1 101
char 1 97
char 1 109
char 1 116
char 1 97
char 1 115
char 1 107
char 1 0
align 1
LABELV $440
char 1 116
char 1 101
char 1 97
char 1 109
char 1 111
char 1 118
char 1 101
char 1 114
char 1 108
char 1 97
char 1 121
char 1 0
align 1
LABELV $435
char 1 98
char 1 0
align 1
LABELV $434
char 1 98
char 1 108
char 1 117
char 1 101
char 1 0
align 1
LABELV $430
char 1 114
char 1 0
align 1
LABELV $429
char 1 114
char 1 101
char 1 100
char 1 0
align 1
LABELV $426
char 1 116
char 1 101
char 1 97
char 1 109
char 1 0
align 1
LABELV $420
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
LABELV $419
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 0
align 1
LABELV $418
char 1 116
char 1 101
char 1 97
char 1 109
char 1 95
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 0
align 1
LABELV $411
char 1 80
char 1 76
char 1 82
char 1 69
char 1 78
char 1 65
char 1 77
char 1 69
char 1 0
align 1
LABELV $410
char 1 112
char 1 114
char 1 105
char 1 110
char 1 116
char 1 32
char 1 34
char 1 37
char 1 115
char 1 94
char 1 55
char 1 32
char 1 37
char 1 115
char 1 32
char 1 37
char 1 115
char 1 10
char 1 34
char 1 0
align 1
LABELV $405
char 1 115
char 1 99
char 1 111
char 1 114
char 1 101
char 1 98
char 1 111
char 1 97
char 1 114
char 1 100
char 1 0
align 1
LABELV $400
char 1 110
char 1 97
char 1 109
char 1 101
char 1 0
align 1
LABELV $397
char 1 99
char 1 103
char 1 95
char 1 112
char 1 114
char 1 101
char 1 100
char 1 105
char 1 99
char 1 116
char 1 73
char 1 116
char 1 101
char 1 109
char 1 115
char 1 0
align 1
LABELV $396
char 1 108
char 1 111
char 1 99
char 1 97
char 1 108
char 1 104
char 1 111
char 1 115
char 1 116
char 1 0
align 1
LABELV $393
char 1 105
char 1 112
char 1 0
align 1
LABELV $392
char 1 92
char 1 110
char 1 97
char 1 109
char 1 101
char 1 92
char 1 98
char 1 97
char 1 100
char 1 105
char 1 110
char 1 102
char 1 111
char 1 0
align 1
LABELV $386
char 1 42
char 1 102
char 1 108
char 1 97
char 1 115
char 1 104
char 1 0
align 1
LABELV $380
char 1 99
char 1 114
char 1 97
char 1 110
char 1 105
char 1 117
char 1 109
char 1 0
align 1
LABELV $378
char 1 117
char 1 112
char 1 112
char 1 101
char 1 114
char 1 95
char 1 108
char 1 117
char 1 109
char 1 98
char 1 97
char 1 114
char 1 0
align 1
LABELV $376
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 95
char 1 114
char 1 111
char 1 111
char 1 116
char 1 0
align 1
LABELV $375
char 1 42
char 1 108
char 1 95
char 1 104
char 1 97
char 1 110
char 1 100
char 1 0
align 1
LABELV $374
char 1 42
char 1 114
char 1 95
char 1 104
char 1 97
char 1 110
char 1 100
char 1 0
align 1
LABELV $369
char 1 47
char 1 97
char 1 110
char 1 105
char 1 109
char 1 97
char 1 116
char 1 105
char 1 111
char 1 110
char 1 46
char 1 99
char 1 102
char 1 103
char 1 0
align 1
LABELV $366
char 1 70
char 1 97
char 1 105
char 1 108
char 1 101
char 1 100
char 1 32
char 1 116
char 1 111
char 1 32
char 1 108
char 1 111
char 1 97
char 1 100
char 1 32
char 1 97
char 1 110
char 1 105
char 1 109
char 1 97
char 1 116
char 1 105
char 1 111
char 1 110
char 1 32
char 1 102
char 1 105
char 1 108
char 1 101
char 1 32
char 1 37
char 1 115
char 1 10
char 1 0
align 1
LABELV $365
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 115
char 1 47
char 1 112
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 115
char 1 47
char 1 95
char 1 104
char 1 117
char 1 109
char 1 97
char 1 110
char 1 111
char 1 105
char 1 100
char 1 47
char 1 97
char 1 110
char 1 105
char 1 109
char 1 97
char 1 116
char 1 105
char 1 111
char 1 110
char 1 46
char 1 99
char 1 102
char 1 103
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
char 1 112
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 115
char 1 47
char 1 107
char 1 121
char 1 108
char 1 101
char 1 47
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
align 1
LABELV $347
char 1 80
char 1 97
char 1 100
char 1 97
char 1 119
char 1 97
char 1 110
char 1 0
align 1
LABELV $245
char 1 98
char 1 111
char 1 100
char 1 121
char 1 113
char 1 117
char 1 101
char 1 0
align 1
LABELV $225
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
char 1 97
char 1 32
char 1 115
char 1 112
char 1 97
char 1 119
char 1 110
char 1 32
char 1 112
char 1 111
char 1 105
char 1 110
char 1 116
char 1 0
align 1
LABELV $149
char 1 66
char 1 69
char 1 67
char 1 79
char 1 77
char 1 69
char 1 74
char 1 77
char 1 0
align 1
LABELV $148
char 1 83
char 1 86
char 1 73
char 1 78
char 1 71
char 1 65
char 1 77
char 1 69
char 1 0
align 1
LABELV $147
char 1 99
char 1 112
char 1 32
char 1 34
char 1 37
char 1 115
char 1 32
char 1 37
char 1 115
char 1 10
char 1 34
char 1 0
align 1
LABELV $141
char 1 37
char 1 105
char 1 0
align 1
LABELV $108
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
LABELV $103
char 1 45
char 1 49
char 1 0
align 1
LABELV $87
char 1 105
char 1 110
char 1 102
char 1 111
char 1 95
char 1 112
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 95
char 1 100
char 1 101
char 1 97
char 1 116
char 1 104
char 1 109
char 1 97
char 1 116
char 1 99
char 1 104
char 1 0
align 1
LABELV $83
char 1 110
char 1 111
char 1 104
char 1 117
char 1 109
char 1 97
char 1 110
char 1 115
char 1 0
align 1
LABELV $80
char 1 48
char 1 0
align 1
LABELV $79
char 1 110
char 1 111
char 1 98
char 1 111
char 1 116
char 1 115
char 1 0
