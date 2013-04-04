export SP_info_camp
code
proc SP_info_camp 4 8
file "../g_misc.c"
line 22
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:// g_misc.c
;4:
;5:#include "g_local.h"
;6:
;7:#define HOLOCRON_RESPAWN_TIME 30000
;8:#define MAX_AMMO_GIVE 2
;9:#define STATION_RECHARGE_TIME 3000//800
;10:
;11:void HolocronThink(gentity_t *ent);
;12:extern vmCvar_t g_MaxHolocronCarry;
;13:
;14:/*QUAKED func_group (0 0 0) ?
;15:Used to group brushes together just for editor convenience.  They are turned into normal brushes by the utilities.
;16:*/
;17:
;18:
;19:/*QUAKED info_camp (0 0.5 0) (-4 -4 -4) (4 4 4)
;20:Used as a positional target for calculations in the utilities (spotlights, etc), but removed during gameplay.
;21:*/
;22:void SP_info_camp( gentity_t *self ) {
line 23
;23:	G_SetOrigin( self, self->s.origin );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 24
;24:}
LABELV $78
endproc SP_info_camp 4 8
export SP_info_null
proc SP_info_null 0 4
line 30
;25:
;26:
;27:/*QUAKED info_null (0 0.5 0) (-4 -4 -4) (4 4 4)
;28:Used as a positional target for calculations in the utilities (spotlights, etc), but removed during gameplay.
;29:*/
;30:void SP_info_null( gentity_t *self ) {
line 31
;31:	G_FreeEntity( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 32
;32:}
LABELV $79
endproc SP_info_null 0 4
export SP_info_notnull
proc SP_info_notnull 4 8
line 39
;33:
;34:
;35:/*QUAKED info_notnull (0 0.5 0) (-4 -4 -4) (4 4 4)
;36:Used as a positional target for in-game calculation, like jumppad targets.
;37:target_position does the same thing
;38:*/
;39:void SP_info_notnull( gentity_t *self ){
line 40
;40:	G_SetOrigin( self, self->s.origin );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 41
;41:}
LABELV $80
endproc SP_info_notnull 4 8
export SP_light
proc SP_light 0 4
line 51
;42:
;43:
;44:/*QUAKED light (0 1 0) (-8 -8 -8) (8 8 8) linear
;45:Non-displayed light.
;46:"light" overrides the default 300 intensity.
;47:Linear checbox gives linear falloff instead of inverse square
;48:Lights pointed at a target will be spotlights.
;49:"radius" overrides the default 64 unit radius of a spotlight at the target point.
;50:*/
;51:void SP_light( gentity_t *self ) {
line 52
;52:	G_FreeEntity( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 53
;53:}
LABELV $81
endproc SP_light 0 4
export TeleportPlayer
proc TeleportPlayer 40 16
line 65
;54:
;55:
;56:
;57:/*
;58:=================================================================================
;59:
;60:TELEPORTERS
;61:
;62:=================================================================================
;63:*/
;64:
;65:void TeleportPlayer( gentity_t *player, vec3_t origin, vec3_t angles ) {
line 70
;66:	gentity_t	*tent;
;67:
;68:	// use temp events at source and destination to prevent the effect
;69:	// from getting dropped by a second player event
;70:	if ( player->client->sess.sessionTeam != TEAM_SPECTATOR ) {
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
EQI4 $83
line 71
;71:		tent = G_TempEntity( player->client->ps.origin, EV_PLAYER_TELEPORT_OUT );
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
CNSTI4 60
ARGI4
ADDRLP4 4
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 72
;72:		tent->s.clientNum = player->s.clientNum;
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
line 74
;73:
;74:		tent = G_TempEntity( origin, EV_PLAYER_TELEPORT_IN );
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 59
ARGI4
ADDRLP4 12
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ASGNP4
line 75
;75:		tent->s.clientNum = player->s.clientNum;
ADDRLP4 16
CNSTI4 220
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 76
;76:	}
LABELV $83
line 79
;77:
;78:	// unlink to make sure it can't possibly interfere with G_KillBox
;79:	trap_UnlinkEntity (player);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 81
;80:
;81:	VectorCopy ( origin, player->client->ps.origin );
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 82
;82:	player->client->ps.origin[2] += 1;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 28
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRF4
CNSTF4 1065353216
ADDF4
ASGNF4
line 85
;83:
;84:	// spit the player out
;85:	AngleVectors( angles, player->client->ps.velocity, NULL, NULL );
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRLP4 8
CNSTP4 0
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 86
;86:	VectorScale( player->client->ps.velocity, 400, player->client->ps.velocity );
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTF4 1137180672
ADDRLP4 12
INDIRP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTF4 1137180672
ADDRLP4 16
INDIRP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTF4 1137180672
ADDRLP4 20
INDIRP4
INDIRF4
MULF4
ASGNF4
line 87
;87:	player->client->ps.pm_time = 160;		// hold time
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 16
ADDP4
CNSTI4 160
ASGNI4
line 88
;88:	player->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 91
;89:
;90:	// toggle the teleport bit so the client knows to not lerp
;91:	player->client->ps.eFlags ^= EF_TELEPORT_BIT;
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
CNSTI4 4
BXORI4
ASGNI4
line 94
;92:
;93:	// set angles
;94:	SetClientViewAngle( player, angles );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 SetClientViewAngle
CALLV
pop
line 97
;95:
;96:	// kill anything at the destination
;97:	if ( player->client->sess.sessionTeam != TEAM_SPECTATOR ) {
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
EQI4 $85
line 98
;98:		G_KillBox (player);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_KillBox
CALLV
pop
line 99
;99:	}
LABELV $85
line 102
;100:
;101:	// save results of pmove
;102:	BG_PlayerStateToEntityState( &player->client->ps, &player->s, qtrue );
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ARGP4
ADDRLP4 32
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 BG_PlayerStateToEntityState
CALLV
pop
line 105
;103:
;104:	// use the precise origin for linking
;105:	VectorCopy( player->client->ps.origin, player->r.currentOrigin );
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
CNSTI4 368
ADDP4
ADDRLP4 36
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 107
;106:
;107:	if ( player->client->sess.sessionTeam != TEAM_SPECTATOR ) {
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
EQI4 $87
line 108
;108:		trap_LinkEntity (player);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 109
;109:	}
LABELV $87
line 110
;110:}
LABELV $82
endproc TeleportPlayer 40 16
export SP_misc_teleporter_dest
proc SP_misc_teleporter_dest 0 0
line 118
;111:
;112:
;113:/*QUAKED misc_teleporter_dest (1 0 0) (-32 -32 -24) (32 32 -16)
;114:Point teleporters at these.
;115:Now that we don't have teleport destination pads, this is just
;116:an info_notnull
;117:*/
;118:void SP_misc_teleporter_dest( gentity_t *ent ) {
line 119
;119:}
LABELV $89
endproc SP_misc_teleporter_dest 0 0
export SP_misc_model
proc SP_misc_model 0 4
line 127
;120:
;121:
;122://===========================================================
;123:
;124:/*QUAKED misc_model (1 0 0) (-16 -16 -16) (16 16 16)
;125:"model"		arbitrary .md3 file to display
;126:*/
;127:void SP_misc_model( gentity_t *ent ) {
line 138
;128:
;129:#if 0
;130:	ent->s.modelindex = G_ModelIndex( ent->model );
;131:	VectorSet (ent->mins, -16, -16, -16);
;132:	VectorSet (ent->maxs, 16, 16, 16);
;133:	trap_LinkEntity (ent);
;134:
;135:	G_SetOrigin( ent, ent->s.origin );
;136:	VectorCopy( ent->s.angles, ent->s.apos.trBase );
;137:#else
;138:	G_FreeEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 140
;139:#endif
;140:}
LABELV $90
endproc SP_misc_model 0 4
export SP_misc_G2model
proc SP_misc_G2model 0 4
line 146
;141:
;142:
;143:/*QUAKED misc_G2model (1 0 0) (-16 -16 -16) (16 16 16)
;144:"model"		arbitrary .glm file to display
;145:*/
;146:void SP_misc_G2model( gentity_t *ent ) {
line 160
;147:
;148:#if 0
;149:	char name1[200] = "models/players/kyle/modelmp.glm";
;150:	trap_G2API_InitGhoul2Model(&ent->s, name1, G_ModelIndex( name1 ), 0, 0, 0, 0);
;151:	trap_G2API_SetBoneAnim(ent->s.ghoul2, 0, "model_root", 0, 12, BONE_ANIM_OVERRIDE_LOOP, 1.0f, level.time, -1, -1);
;152:	ent->s.radius = 150;
;153://	VectorSet (ent->mins, -16, -16, -16);
;154://	VectorSet (ent->maxs, 16, 16, 16);
;155:	trap_LinkEntity (ent);
;156:
;157:	G_SetOrigin( ent, ent->s.origin );
;158:	VectorCopy( ent->s.angles, ent->s.apos.trBase );
;159:#else
;160:	G_FreeEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 162
;161:#endif
;162:}
LABELV $91
endproc SP_misc_G2model 0 4
export locateCamera
proc locateCamera 52 8
line 166
;163:
;164://===========================================================
;165:
;166:void locateCamera( gentity_t *ent ) {
line 171
;167:	vec3_t		dir;
;168:	gentity_t	*target;
;169:	gentity_t	*owner;
;170:
;171:	owner = G_PickTarget( ent->target );
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 G_PickTarget
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
ASGNP4
line 172
;172:	if ( !owner ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $93
line 173
;173:		G_Printf( "Couldn't find target for misc_partal_surface\n" );
ADDRGP4 $95
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 174
;174:		G_FreeEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 175
;175:		return;
ADDRGP4 $92
JUMPV
LABELV $93
line 177
;176:	}
;177:	ent->r.ownerNum = owner->s.number;
ADDRFP4 0
INDIRP4
CNSTI4 396
ADDP4
ADDRLP4 0
INDIRP4
INDIRI4
ASGNI4
line 180
;178:
;179:	// frame holds the rotate speed
;180:	if ( owner->spawnflags & 1 ) {
ADDRLP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $96
line 181
;181:		ent->s.frame = 25;
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
CNSTI4 25
ASGNI4
line 182
;182:	} else if ( owner->spawnflags & 2 ) {
ADDRGP4 $97
JUMPV
LABELV $96
ADDRLP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $98
line 183
;183:		ent->s.frame = 75;
ADDRFP4 0
INDIRP4
CNSTI4 224
ADDP4
CNSTI4 75
ASGNI4
line 184
;184:	}
LABELV $98
LABELV $97
line 187
;185:
;186:	// swing camera ?
;187:	if ( owner->spawnflags & 4 ) {
ADDRLP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $100
line 189
;188:		// set to 0 for no rotation at all
;189:		ent->s.powerups = 0;
ADDRFP4 0
INDIRP4
CNSTI4 272
ADDP4
CNSTI4 0
ASGNI4
line 190
;190:	}
ADDRGP4 $101
JUMPV
LABELV $100
line 191
;191:	else {
line 192
;192:		ent->s.powerups = 1;
ADDRFP4 0
INDIRP4
CNSTI4 272
ADDP4
CNSTI4 1
ASGNI4
line 193
;193:	}
LABELV $101
line 196
;194:
;195:	// clientNum holds the rotate offset
;196:	ent->s.clientNum = owner->s.clientNum;
ADDRLP4 24
CNSTI4 220
ASGNI4
ADDRFP4 0
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
ADDRLP4 0
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 198
;197:
;198:	VectorCopy( owner->s.origin, ent->s.origin2 );
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 201
;199:
;200:	// see if the portal_camera has a target
;201:	target = G_PickTarget( owner->target );
ADDRLP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 G_PickTarget
CALLP4
ASGNP4
ADDRLP4 16
ADDRLP4 28
INDIRP4
ASGNP4
line 202
;202:	if ( target ) {
ADDRLP4 16
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $102
line 203
;203:		VectorSubtract( target->s.origin, owner->s.origin, dir );
ADDRLP4 36
CNSTI4 92
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 44
CNSTI4 96
ASGNI4
ADDRLP4 4+4
ADDRLP4 16
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 48
CNSTI4 100
ASGNI4
ADDRLP4 4+8
ADDRLP4 16
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 204
;204:		VectorNormalize( dir );
ADDRLP4 4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 205
;205:	} else {
ADDRGP4 $103
JUMPV
LABELV $102
line 206
;206:		G_SetMovedir( owner->s.angles, dir );
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 G_SetMovedir
CALLV
pop
line 207
;207:	}
LABELV $103
line 209
;208:
;209:	ent->s.eventParm = DirToByte( dir );
ADDRLP4 4
ARGP4
ADDRLP4 32
ADDRGP4 DirToByte
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 256
ADDP4
ADDRLP4 32
INDIRI4
ASGNI4
line 210
;210:}
LABELV $92
endproc locateCamera 52 8
export SP_misc_portal_surface
proc SP_misc_portal_surface 20 4
line 216
;211:
;212:/*QUAKED misc_portal_surface (0 0 1) (-8 -8 -8) (8 8 8)
;213:The portal surface nearest this entity will show a view from the targeted misc_portal_camera, or a mirror view if untargeted.
;214:This must be within 64 world units of the surface!
;215:*/
;216:void SP_misc_portal_surface(gentity_t *ent) {
line 217
;217:	VectorClear( ent->r.mins );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
CNSTF4 0
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 324
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 320
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 316
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
line 218
;218:	VectorClear( ent->r.maxs );
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
CNSTF4 0
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 336
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 332
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 328
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
line 219
;219:	trap_LinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 221
;220:
;221:	ent->r.svFlags = SVF_PORTAL;
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
CNSTI4 64
ASGNI4
line 222
;222:	ent->s.eType = ET_PORTAL;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 8
ASGNI4
line 224
;223:
;224:	if ( !ent->target ) {
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $107
line 225
;225:		VectorCopy( ent->s.origin, ent->s.origin2 );
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 16
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 226
;226:	} else {
ADDRGP4 $108
JUMPV
LABELV $107
line 227
;227:		ent->think = locateCamera;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 locateCamera
ASGNP4
line 228
;228:		ent->nextthink = level.time + 100;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 229
;229:	}
LABELV $108
line 230
;230:}
LABELV $106
endproc SP_misc_portal_surface 20 4
export SP_misc_portal_camera
proc SP_misc_portal_camera 20 12
line 236
;231:
;232:/*QUAKED misc_portal_camera (0 0 1) (-8 -8 -8) (8 8 8) slowrotate fastrotate noswing
;233:The target for a misc_portal_director.  You can set either angles or target another entity to determine the direction of view.
;234:"roll" an angle modifier to orient the camera around the target vector;
;235:*/
;236:void SP_misc_portal_camera(gentity_t *ent) {
line 239
;237:	float	roll;
;238:
;239:	VectorClear( ent->r.mins );
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
CNSTF4 0
ASGNF4
ADDRLP4 4
INDIRP4
CNSTI4 324
ADDP4
ADDRLP4 8
INDIRF4
ASGNF4
ADDRLP4 4
INDIRP4
CNSTI4 320
ADDP4
ADDRLP4 8
INDIRF4
ASGNF4
ADDRLP4 4
INDIRP4
CNSTI4 316
ADDP4
ADDRLP4 8
INDIRF4
ASGNF4
line 240
;240:	VectorClear( ent->r.maxs );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
CNSTF4 0
ASGNF4
ADDRLP4 12
INDIRP4
CNSTI4 336
ADDP4
ADDRLP4 16
INDIRF4
ASGNF4
ADDRLP4 12
INDIRP4
CNSTI4 332
ADDP4
ADDRLP4 16
INDIRF4
ASGNF4
ADDRLP4 12
INDIRP4
CNSTI4 328
ADDP4
ADDRLP4 16
INDIRF4
ASGNF4
line 241
;241:	trap_LinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 243
;242:
;243:	G_SpawnFloat( "roll", "0", &roll );
ADDRGP4 $111
ARGP4
ADDRGP4 $112
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
line 245
;244:
;245:	ent->s.clientNum = roll/360.0 * 256;
ADDRFP4 0
INDIRP4
CNSTI4 220
ADDP4
CNSTF4 1132462080
ADDRLP4 0
INDIRF4
CNSTF4 1135869952
DIVF4
MULF4
CVFI4 4
ASGNI4
line 246
;246:}
LABELV $110
endproc SP_misc_portal_camera 20 12
export HolocronRespawn
proc HolocronRespawn 4 0
line 292
;247:
;248:/*QUAKED misc_holocron (0 0 1) (-8 -8 -8) (8 8 8)
;249:count	Set to type of holocron (based on force power value)
;250:	HEAL = 0
;251:	JUMP = 1
;252:	SPEED = 2
;253:	PUSH = 3
;254:	PULL = 4
;255:	TELEPATHY = 5
;256:	GRIP = 6
;257:	LIGHTNING = 7
;258:	RAGE = 8
;259:	PROTECT = 9
;260:	ABSORB = 10
;261:	TEAM HEAL = 11
;262:	TEAM FORCE = 12
;263:	DRAIN = 13
;264:	SEE = 14
;265:	SABERATTACK = 15
;266:	SABERDEFEND = 16
;267:	SABERTHROW = 17
;268:*/
;269:
;270:/*char *holocronTypeModels[] = {
;271:	"models/chunks/rock/rock_big.md3",//FP_HEAL,
;272:	"models/chunks/rock/rock_big.md3",//FP_LEVITATION,
;273:	"models/chunks/rock/rock_big.md3",//FP_SPEED,
;274:	"models/chunks/rock/rock_big.md3",//FP_PUSH,
;275:	"models/chunks/rock/rock_big.md3",//FP_PULL,
;276:	"models/chunks/rock/rock_big.md3",//FP_TELEPATHY,
;277:	"models/chunks/rock/rock_big.md3",//FP_GRIP,
;278:	"models/chunks/rock/rock_big.md3",//FP_LIGHTNING,
;279:	"models/chunks/rock/rock_big.md3",//FP_RAGE,
;280:	"models/chunks/rock/rock_big.md3",//FP_PROTECT,
;281:	"models/chunks/rock/rock_big.md3",//FP_ABSORB,
;282:	"models/chunks/rock/rock_big.md3",//FP_TEAM_HEAL,
;283:	"models/chunks/rock/rock_big.md3",//FP_TEAM_FORCE,
;284:	"models/chunks/rock/rock_big.md3",//FP_DRAIN,
;285:	"models/chunks/rock/rock_big.md3",//FP_SEE
;286:	"models/chunks/rock/rock_big.md3",//FP_SABERATTACK
;287:	"models/chunks/rock/rock_big.md3",//FP_SABERDEFEND
;288:	"models/chunks/rock/rock_big.md3"//FP_SABERTHROW
;289:};*/
;290:
;291:void HolocronRespawn(gentity_t *self)
;292:{
line 293
;293:	self->s.modelindex = (self->count - 128);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 212
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 128
SUBI4
ASGNI4
line 294
;294:}
LABELV $113
endproc HolocronRespawn 4 0
export HolocronPopOut
proc HolocronPopOut 12 8
line 297
;295:
;296:void HolocronPopOut(gentity_t *self)
;297:{
line 298
;298:	if (Q_irand(1, 10) < 5)
CNSTI4 1
ARGI4
CNSTI4 10
ARGI4
ADDRLP4 0
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 5
GEI4 $115
line 299
;299:	{
line 300
;300:		self->s.pos.trDelta[0] = 150 + Q_irand(1, 100);
CNSTI4 1
ARGI4
CNSTI4 100
ARGI4
ADDRLP4 4
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 4
INDIRI4
CNSTI4 150
ADDI4
CVIF4 4
ASGNF4
line 301
;301:	}
ADDRGP4 $116
JUMPV
LABELV $115
line 303
;302:	else
;303:	{
line 304
;304:		self->s.pos.trDelta[0] = -150 - Q_irand(1, 100);
CNSTI4 1
ARGI4
CNSTI4 100
ARGI4
ADDRLP4 4
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
CNSTI4 -150
ADDRLP4 4
INDIRI4
SUBI4
CVIF4 4
ASGNF4
line 305
;305:	}
LABELV $116
line 306
;306:	if (Q_irand(1, 10) < 5)
CNSTI4 1
ARGI4
CNSTI4 10
ARGI4
ADDRLP4 4
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 5
GEI4 $117
line 307
;307:	{
line 308
;308:		self->s.pos.trDelta[1] = 150 + Q_irand(1, 100);
CNSTI4 1
ARGI4
CNSTI4 100
ARGI4
ADDRLP4 8
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 8
INDIRI4
CNSTI4 150
ADDI4
CVIF4 4
ASGNF4
line 309
;309:	}
ADDRGP4 $118
JUMPV
LABELV $117
line 311
;310:	else
;311:	{
line 312
;312:		self->s.pos.trDelta[1] = -150 - Q_irand(1, 100);
CNSTI4 1
ARGI4
CNSTI4 100
ARGI4
ADDRLP4 8
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
CNSTI4 -150
ADDRLP4 8
INDIRI4
SUBI4
CVIF4 4
ASGNF4
line 313
;313:	}
LABELV $118
line 314
;314:	self->s.pos.trDelta[2] = 150 + Q_irand(1, 100);
CNSTI4 1
ARGI4
CNSTI4 100
ARGI4
ADDRLP4 8
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 8
INDIRI4
CNSTI4 150
ADDI4
CVIF4 4
ASGNF4
line 315
;315:}
LABELV $114
endproc HolocronPopOut 12 8
export HolocronTouch
proc HolocronTouch 44 12
line 318
;316:
;317:void HolocronTouch(gentity_t *self, gentity_t *other, trace_t *trace)
;318:{
line 319
;319:	int i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 320
;320:	int othercarrying = 0;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 321
;321:	float time_lowest = 0;
ADDRLP4 8
CNSTF4 0
ASGNF4
line 322
;322:	int index_lowest = -1;
ADDRLP4 4
CNSTI4 -1
ASGNI4
line 323
;323:	int hasall = 1;
ADDRLP4 16
CNSTI4 1
ASGNI4
line 324
;324:	int forceReselect = WP_NONE;
ADDRLP4 20
CNSTI4 0
ASGNI4
line 326
;325:
;326:	if (trace)
ADDRFP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $120
line 327
;327:	{
line 328
;328:		self->s.groundEntityNum = trace->entityNum;
ADDRFP4 0
INDIRP4
CNSTI4 192
ADDP4
ADDRFP4 8
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
ASGNI4
line 329
;329:	}
LABELV $120
line 331
;330:
;331:	if (!other || !other->client || other->health < 1)
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
EQU4 $125
ADDRLP4 24
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 28
INDIRU4
EQU4 $125
ADDRLP4 24
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 1
GEI4 $122
LABELV $125
line 332
;332:	{
line 333
;333:		return;
ADDRGP4 $119
JUMPV
LABELV $122
line 336
;334:	}
;335:
;336:	if (!self->s.modelindex)
ADDRFP4 0
INDIRP4
CNSTI4 212
ADDP4
INDIRI4
CNSTI4 0
NEI4 $126
line 337
;337:	{
line 338
;338:		return;
ADDRGP4 $119
JUMPV
LABELV $126
line 341
;339:	}
;340:
;341:	if (self->enemy)
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $128
line 342
;342:	{
line 343
;343:		return;
ADDRGP4 $119
JUMPV
LABELV $128
line 346
;344:	}
;345:
;346:	if (other->client->ps.holocronsCarried[self->count])
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 632
ADDP4
ADDP4
INDIRF4
CNSTF4 0
EQF4 $130
line 347
;347:	{
line 348
;348:		return;
ADDRGP4 $119
JUMPV
LABELV $130
line 351
;349:	}
;350:
;351:	if (other->client->ps.holocronCantTouch == self->s.number && other->client->ps.holocronCantTouchTime > level.time)
ADDRLP4 32
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 704
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $136
ADDRLP4 32
INDIRP4
CNSTI4 708
ADDP4
INDIRF4
ADDRGP4 level+32
INDIRI4
CVIF4 4
LEF4 $136
line 352
;352:	{
line 353
;353:		return;
ADDRGP4 $119
JUMPV
LABELV $135
line 357
;354:	}
;355:
;356:	while (i < NUM_FORCE_POWERS)
;357:	{
line 358
;358:		if (other->client->ps.holocronsCarried[i])
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 632
ADDP4
ADDP4
INDIRF4
CNSTF4 0
EQF4 $138
line 359
;359:		{
line 360
;360:			othercarrying++;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 362
;361:
;362:			if (index_lowest == -1 || other->client->ps.holocronsCarried[i] < time_lowest)
ADDRLP4 4
INDIRI4
CNSTI4 -1
EQI4 $142
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 632
ADDP4
ADDP4
INDIRF4
ADDRLP4 8
INDIRF4
GEF4 $139
LABELV $142
line 363
;363:			{
line 364
;364:				index_lowest = i;
ADDRLP4 4
ADDRLP4 0
INDIRI4
ASGNI4
line 365
;365:				time_lowest = other->client->ps.holocronsCarried[i];
ADDRLP4 8
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 632
ADDP4
ADDP4
INDIRF4
ASGNF4
line 366
;366:			}
line 367
;367:		}
ADDRGP4 $139
JUMPV
LABELV $138
line 368
;368:		else if (i != self->count)
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
EQI4 $143
line 369
;369:		{
line 370
;370:			hasall = 0;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 371
;371:		}
LABELV $143
LABELV $139
line 372
;372:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 373
;373:	}
LABELV $136
line 356
ADDRLP4 0
INDIRI4
CNSTI4 18
LTI4 $135
line 375
;374:
;375:	if (hasall)
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $145
line 376
;376:	{ //once we pick up this holocron we'll have all of them, so give us super special best prize!
line 378
;377:		//G_Printf("You deserve a pat on the back.\n");
;378:	}
LABELV $145
line 380
;379:
;380:	if (!(other->client->ps.fd.forcePowersActive & (1 << other->client->ps.fd.forcePowerSelected)))
ADDRLP4 36
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
CNSTI4 836
ADDP4
INDIRI4
CNSTI4 1
ADDRLP4 36
INDIRP4
CNSTI4 840
ADDP4
INDIRI4
LSHI4
BANDI4
CNSTI4 0
NEI4 $147
line 381
;381:	{ //If the player isn't using his currently selected force power, select this one
line 382
;382:		if (self->count != FP_SABERATTACK && self->count != FP_SABERDEFEND && self->count != FP_SABERTHROW && self->count != FP_LEVITATION)
ADDRLP4 40
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 15
EQI4 $149
ADDRLP4 40
INDIRI4
CNSTI4 16
EQI4 $149
ADDRLP4 40
INDIRI4
CNSTI4 17
EQI4 $149
ADDRLP4 40
INDIRI4
CNSTI4 1
EQI4 $149
line 383
;383:		{
line 384
;384:			other->client->ps.fd.forcePowerSelected = self->count;
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 840
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 385
;385:		}
LABELV $149
line 386
;386:	}
LABELV $147
line 388
;387:
;388:	if (g_MaxHolocronCarry.integer && othercarrying >= g_MaxHolocronCarry.integer)
ADDRGP4 g_MaxHolocronCarry+12
INDIRI4
CNSTI4 0
EQI4 $151
ADDRLP4 12
INDIRI4
ADDRGP4 g_MaxHolocronCarry+12
INDIRI4
LTI4 $151
line 389
;389:	{ //make the oldest holocron carried by the player pop out to make room for this one
line 390
;390:		other->client->ps.holocronsCarried[index_lowest] = 0;
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 632
ADDP4
ADDP4
CNSTF4 0
ASGNF4
line 405
;391:
;392:		/*
;393:		if (index_lowest == FP_SABERATTACK && !HasSetSaberOnly())
;394:		{ //you lost your saberattack holocron, so no more saber for you
;395:			other->client->ps.stats[STAT_WEAPONS] |= (1 << WP_STUN_BATON);
;396:			other->client->ps.stats[STAT_WEAPONS] &= ~(1 << WP_SABER);
;397:
;398:			if (other->client->ps.weapon == WP_SABER)
;399:			{
;400:				forceReselect = WP_SABER;
;401:			}
;402:		}
;403:		*/
;404:		//NOTE: No longer valid as we are now always giving a force level 1 saber attack level in holocron
;405:	}
LABELV $151
line 408
;406:
;407:	//G_Sound(other, CHAN_AUTO, G_SoundIndex("sound/weapons/w_pkup.wav"));
;408:	G_AddEvent( other, EV_ITEM_PICKUP, self->s.number );
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
line 410
;409:
;410:	other->client->ps.holocronsCarried[self->count] = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 632
ADDP4
ADDP4
ADDRGP4 level+32
INDIRI4
CVIF4 4
ASGNF4
line 411
;411:	self->s.modelindex = 0;
ADDRFP4 0
INDIRP4
CNSTI4 212
ADDP4
CNSTI4 0
ASGNI4
line 412
;412:	self->enemy = other;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRFP4 4
INDIRP4
ASGNP4
line 414
;413:
;414:	self->pos2[0] = 1;
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
CNSTF4 1065353216
ASGNF4
line 415
;415:	self->pos2[1] = level.time + HOLOCRON_RESPAWN_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 564
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 30000
ADDI4
CVIF4 4
ASGNF4
line 430
;416:
;417:	/*
;418:	if (self->count == FP_SABERATTACK && !HasSetSaberOnly())
;419:	{ //player gets a saber
;420:		other->client->ps.stats[STAT_WEAPONS] |= (1 << WP_SABER);
;421:		other->client->ps.stats[STAT_WEAPONS] &= ~(1 << WP_STUN_BATON);
;422:
;423:		if (other->client->ps.weapon == WP_STUN_BATON)
;424:		{
;425:			forceReselect = WP_STUN_BATON;
;426:		}
;427:	}
;428:	*/
;429:
;430:	if (forceReselect != WP_NONE)
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $157
line 431
;431:	{
line 432
;432:		G_AddEvent(other, EV_NOAMMO, forceReselect);
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 22
ARGI4
ADDRLP4 20
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 433
;433:	}
LABELV $157
line 436
;434:
;435:	//G_Printf("DON'T TOUCH ME\n");
;436:}
LABELV $119
endproc HolocronTouch 44 12
export HolocronThink
proc HolocronThink 68 4
line 439
;437:
;438:void HolocronThink(gentity_t *ent)
;439:{
line 440
;440:	if (ent->pos2[0] && (!ent->enemy || !ent->enemy->client || ent->enemy->health < 1))
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
EQF4 $160
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 728
ADDP4
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
EQU4 $163
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 8
INDIRU4
EQU4 $163
ADDRLP4 4
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 1
GEI4 $160
LABELV $163
line 441
;441:	{
line 442
;442:		if (ent->enemy && ent->enemy->client)
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
ASGNP4
ADDRLP4 16
CNSTU4 0
ASGNU4
ADDRLP4 12
INDIRP4
CVPU4 4
ADDRLP4 16
INDIRU4
EQU4 $161
ADDRLP4 12
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 16
INDIRU4
EQU4 $161
line 443
;443:		{
line 444
;444:			HolocronRespawn(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 HolocronRespawn
CALLV
pop
line 445
;445:			VectorCopy(ent->enemy->client->ps.origin, ent->s.pos.trBase);
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 20
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
line 446
;446:			VectorCopy(ent->enemy->client->ps.origin, ent->s.origin);
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 24
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
line 447
;447:			VectorCopy(ent->enemy->client->ps.origin, ent->r.currentOrigin);
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 368
ADDP4
ADDRLP4 28
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
line 449
;448:			//copy to person carrying's origin before popping out of them
;449:			HolocronPopOut(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 HolocronPopOut
CALLV
pop
line 450
;450:			ent->enemy->client->ps.holocronsCarried[ent->count] = 0;
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 32
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 632
ADDP4
ADDP4
CNSTF4 0
ASGNF4
line 451
;451:			ent->enemy = NULL;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
CNSTP4 0
ASGNP4
line 453
;452:			
;453:			goto justthink;
ADDRGP4 $166
JUMPV
line 455
;454:		}
;455:	}
LABELV $160
line 456
;456:	else if (ent->pos2[0] && ent->enemy && ent->enemy->client)
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 560
ADDP4
INDIRF4
CNSTF4 0
EQF4 $167
ADDRLP4 16
ADDRLP4 12
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
ASGNP4
ADDRLP4 20
CNSTU4 0
ASGNU4
ADDRLP4 16
INDIRP4
CVPU4 4
ADDRLP4 20
INDIRU4
EQU4 $167
ADDRLP4 16
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 20
INDIRU4
EQU4 $167
line 457
;457:	{
line 458
;458:		ent->pos2[1] = level.time + HOLOCRON_RESPAWN_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 564
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 30000
ADDI4
CVIF4 4
ASGNF4
line 459
;459:	}
LABELV $167
LABELV $161
line 461
;460:
;461:	if (ent->enemy && ent->enemy->client)
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
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
EQU4 $170
ADDRLP4 24
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 28
INDIRU4
EQU4 $170
line 462
;462:	{
line 463
;463:		if (!ent->enemy->client->ps.holocronsCarried[ent->count])
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 32
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 632
ADDP4
ADDP4
INDIRF4
CNSTF4 0
NEF4 $172
line 464
;464:		{
line 465
;465:			ent->enemy->client->ps.holocronCantTouch = ent->s.number;
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 704
ADDP4
ADDRLP4 36
INDIRP4
INDIRI4
ASGNI4
line 466
;466:			ent->enemy->client->ps.holocronCantTouchTime = level.time + 5000;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 708
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 5000
ADDI4
CVIF4 4
ASGNF4
line 468
;467:
;468:			HolocronRespawn(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 HolocronRespawn
CALLV
pop
line 469
;469:			VectorCopy(ent->enemy->client->ps.origin, ent->s.pos.trBase);
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 40
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
line 470
;470:			VectorCopy(ent->enemy->client->ps.origin, ent->s.origin);
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 44
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
line 471
;471:			VectorCopy(ent->enemy->client->ps.origin, ent->r.currentOrigin);
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
CNSTI4 368
ADDP4
ADDRLP4 48
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
line 473
;472:			//copy to person carrying's origin before popping out of them
;473:			HolocronPopOut(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 HolocronPopOut
CALLV
pop
line 474
;474:			ent->enemy = NULL;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
CNSTP4 0
ASGNP4
line 476
;475:
;476:			goto justthink;
ADDRGP4 $166
JUMPV
LABELV $172
line 479
;477:		}
;478:
;479:		if (!ent->enemy->inuse || (ent->enemy->client && ent->enemy->client->ps.fallingToDeath))
ADDRLP4 36
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
ASGNP4
ADDRLP4 40
CNSTI4 0
ASGNI4
ADDRLP4 36
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
ADDRLP4 40
INDIRI4
EQI4 $177
ADDRLP4 44
ADDRLP4 36
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $175
ADDRLP4 44
INDIRP4
CNSTI4 1340
ADDP4
INDIRI4
ADDRLP4 40
INDIRI4
EQI4 $175
LABELV $177
line 480
;480:		{
line 481
;481:			if (ent->enemy->inuse && ent->enemy->client)
ADDRLP4 48
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $178
ADDRLP4 48
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $178
line 482
;482:			{
line 483
;483:				ent->enemy->client->ps.holocronBits &= ~(1 << ent->count);
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
CNSTI4 712
ASGNI4
ADDRLP4 60
ADDRLP4 52
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRI4
CNSTI4 1
ADDRLP4 52
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
INDIRI4
LSHI4
BCOMI4
BANDI4
ASGNI4
line 484
;484:				ent->enemy->client->ps.holocronsCarried[ent->count] = 0;
ADDRLP4 64
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 64
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 632
ADDP4
ADDP4
CNSTF4 0
ASGNF4
line 485
;485:			}
LABELV $178
line 486
;486:			ent->enemy = NULL;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
CNSTP4 0
ASGNP4
line 487
;487:			HolocronRespawn(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 HolocronRespawn
CALLV
pop
line 488
;488:			VectorCopy(ent->s.origin2, ent->s.pos.trBase);
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 52
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 489
;489:			VectorCopy(ent->s.origin2, ent->s.origin);
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 56
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 490
;490:			VectorCopy(ent->s.origin2, ent->r.currentOrigin);
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
CNSTI4 368
ADDP4
ADDRLP4 60
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 492
;491:
;492:			ent->s.pos.trTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 494
;493:
;494:			ent->pos2[0] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
CNSTF4 0
ASGNF4
line 496
;495:
;496:			trap_LinkEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 498
;497:
;498:			goto justthink;
ADDRGP4 $166
JUMPV
LABELV $175
line 500
;499:		}
;500:	}
LABELV $170
line 502
;501:
;502:	if (ent->pos2[0] && ent->pos2[1] < level.time)
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 560
ADDP4
INDIRF4
CNSTF4 0
EQF4 $181
ADDRLP4 32
INDIRP4
CNSTI4 564
ADDP4
INDIRF4
ADDRGP4 level+32
INDIRI4
CVIF4 4
GEF4 $181
line 503
;503:	{ //isn't in original place and has been there for (HOLOCRON_RESPAWN_TIME) seconds without being picked up, so respawn
line 504
;504:		VectorCopy(ent->s.origin2, ent->s.pos.trBase);
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 36
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 505
;505:		VectorCopy(ent->s.origin2, ent->s.origin);
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 40
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 506
;506:		VectorCopy(ent->s.origin2, ent->r.currentOrigin);
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
CNSTI4 368
ADDP4
ADDRLP4 44
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 508
;507:
;508:		ent->s.pos.trTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 510
;509:
;510:		ent->pos2[0] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
CNSTF4 0
ASGNF4
line 512
;511:
;512:		trap_LinkEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 513
;513:	}
LABELV $181
LABELV $166
line 516
;514:
;515:justthink:
;516:	ent->nextthink = level.time + 50;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 50
ADDI4
ASGNI4
line 518
;517:
;518:	if (ent->s.pos.trDelta[0] || ent->s.pos.trDelta[1] || ent->s.pos.trDelta[2])
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
CNSTF4 0
ASGNF4
ADDRLP4 36
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
ADDRLP4 40
INDIRF4
NEF4 $189
ADDRLP4 36
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
ADDRLP4 40
INDIRF4
NEF4 $189
ADDRLP4 36
INDIRP4
CNSTI4 44
ADDP4
INDIRF4
ADDRLP4 40
INDIRF4
EQF4 $186
LABELV $189
line 519
;519:	{
line 520
;520:		G_RunObject(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_RunObject
CALLV
pop
line 521
;521:	}
LABELV $186
line 522
;522:}
LABELV $159
endproc HolocronThink 68 4
export SP_misc_holocron
proc SP_misc_holocron 1132 28
line 525
;523:
;524:void SP_misc_holocron(gentity_t *ent)
;525:{
line 529
;526:	vec3_t dest;
;527:	trace_t tr;
;528:
;529:	if (g_gametype.integer != GT_HOLOCRON)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 1
EQI4 $191
line 530
;530:	{
line 531
;531:		G_FreeEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 532
;532:		return;
ADDRGP4 $190
JUMPV
LABELV $191
line 535
;533:	}
;534:
;535:	if (HasSetSaberOnly())
ADDRLP4 1092
ADDRGP4 HasSetSaberOnly
CALLI4
ASGNI4
ADDRLP4 1092
INDIRI4
CNSTI4 0
EQI4 $194
line 536
;536:	{
line 537
;537:		if (ent->count == FP_SABERATTACK ||
ADDRLP4 1096
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1096
INDIRI4
CNSTI4 15
EQI4 $199
ADDRLP4 1096
INDIRI4
CNSTI4 16
EQI4 $199
ADDRLP4 1096
INDIRI4
CNSTI4 17
NEI4 $196
LABELV $199
line 540
;538:			ent->count == FP_SABERDEFEND ||
;539:			ent->count == FP_SABERTHROW)
;540:		{ //having saber holocrons in saber only mode is pointless
line 541
;541:			G_FreeEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 542
;542:			return;
ADDRGP4 $190
JUMPV
LABELV $196
line 544
;543:		}
;544:	}
LABELV $194
line 546
;545:
;546:	ent->s.isJediMaster = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 244
ADDP4
CNSTI4 1
ASGNI4
line 548
;547:
;548:	VectorSet( ent->r.maxs, 8, 8, 8 );
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
CNSTF4 1090519040
ASGNF4
line 549
;549:	VectorSet( ent->r.mins, -8, -8, -8 );
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
CNSTF4 3238002688
ASGNF4
line 551
;550:
;551:	ent->s.origin[2] += 0.1;
ADDRLP4 1096
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
ASGNP4
ADDRLP4 1096
INDIRP4
ADDRLP4 1096
INDIRP4
INDIRF4
CNSTF4 1036831949
ADDF4
ASGNF4
line 552
;552:	ent->r.maxs[2] -= 0.1;
ADDRLP4 1100
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
ASGNP4
ADDRLP4 1100
INDIRP4
ADDRLP4 1100
INDIRP4
INDIRF4
CNSTF4 1036831949
SUBF4
ASGNF4
line 554
;553:
;554:	VectorSet( dest, ent->s.origin[0], ent->s.origin[1], ent->s.origin[2] - 4096 );
ADDRLP4 1104
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 1104
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 1104
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ASGNF4
ADDRLP4 0+8
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
CNSTF4 1166016512
SUBF4
ASGNF4
line 555
;555:	trap_Trace( &tr, ent->s.origin, ent->r.mins, ent->r.maxs, dest, ent->s.number, MASK_SOLID );
ADDRLP4 12
ARGP4
ADDRLP4 1108
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1108
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 1108
INDIRP4
CNSTI4 316
ADDP4
ARGP4
ADDRLP4 1108
INDIRP4
CNSTI4 328
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 1108
INDIRP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 556
;556:	if ( tr.startsolid )
ADDRLP4 12+4
INDIRI4
CNSTI4 0
EQI4 $202
line 557
;557:	{
line 558
;558:		G_Printf ("SP_misc_holocron: misc_holocron startsolid at %s\n", vtos(ent->s.origin));
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 1112
ADDRGP4 vtos
CALLP4
ASGNP4
ADDRGP4 $205
ARGP4
ADDRLP4 1112
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 559
;559:		G_FreeEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 560
;560:		return;
ADDRGP4 $190
JUMPV
LABELV $202
line 564
;561:	}
;562:
;563:	//add the 0.1 back after the trace
;564:	ent->r.maxs[2] += 0.1;
ADDRLP4 1112
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
ASGNP4
ADDRLP4 1112
INDIRP4
ADDRLP4 1112
INDIRP4
INDIRF4
CNSTF4 1036831949
ADDF4
ASGNF4
line 569
;565:
;566:	// allow to ride movers
;567://	ent->s.groundEntityNum = tr.entityNum;
;568:
;569:	G_SetOrigin( ent, tr.endpos );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12+12
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 571
;570:
;571:	if (ent->count < 0)
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
GEI4 $207
line 572
;572:	{
line 573
;573:		ent->count = 0;
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
CNSTI4 0
ASGNI4
line 574
;574:	}
LABELV $207
line 576
;575:
;576:	if (ent->count >= NUM_FORCE_POWERS)
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 18
LTI4 $209
line 577
;577:	{
line 578
;578:		ent->count = NUM_FORCE_POWERS-1;
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
CNSTI4 17
ASGNI4
line 579
;579:	}
LABELV $209
line 591
;580:/*
;581:	if (g_forcePowerDisable.integer &&
;582:		(g_forcePowerDisable.integer & (1 << ent->count)))
;583:	{
;584:		G_FreeEntity(ent);
;585:		return;
;586:	}
;587:*/
;588:	//No longer doing this, causing too many complaints about accidentally setting no force powers at all
;589:	//and starting a holocron game (making it basically just FFA)
;590:
;591:	ent->enemy = NULL;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
CNSTP4 0
ASGNP4
line 593
;592:
;593:	ent->s.eFlags = EF_BOUNCE_HALF;
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 32
ASGNI4
line 595
;594:
;595:	ent->s.modelindex = (ent->count - 128);//G_ModelIndex(holocronTypeModels[ent->count]);
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
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 128
SUBI4
ASGNI4
line 596
;596:	ent->s.eType = ET_HOLOCRON;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 5
ASGNI4
line 597
;597:	ent->s.pos.trType = TR_GRAVITY;
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 5
ASGNI4
line 598
;598:	ent->s.pos.trTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 600
;599:
;600:	ent->r.contents = CONTENTS_TRIGGER;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1024
ASGNI4
line 601
;601:	ent->clipmask = MASK_SOLID;
ADDRFP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 1
ASGNI4
line 603
;602:
;603:	ent->s.trickedentindex4 = ent->count;
ADDRLP4 1120
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1120
INDIRP4
CNSTI4 160
ADDP4
ADDRLP4 1120
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 605
;604:
;605:	if (forcePowerDarkLight[ent->count] == FORCE_DARKSIDE)
ADDRLP4 1124
CNSTI4 2
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ADDRLP4 1124
INDIRI4
LSHI4
ADDRGP4 forcePowerDarkLight
ADDP4
INDIRI4
ADDRLP4 1124
INDIRI4
NEI4 $212
line 606
;606:	{
line 607
;607:		ent->s.trickedentindex3 = 1;
ADDRFP4 0
INDIRP4
CNSTI4 156
ADDP4
CNSTI4 1
ASGNI4
line 608
;608:	}
ADDRGP4 $213
JUMPV
LABELV $212
line 609
;609:	else if (forcePowerDarkLight[ent->count] == FORCE_LIGHTSIDE)
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 forcePowerDarkLight
ADDP4
INDIRI4
CNSTI4 1
NEI4 $214
line 610
;610:	{
line 611
;611:		ent->s.trickedentindex3 = 2;
ADDRFP4 0
INDIRP4
CNSTI4 156
ADDP4
CNSTI4 2
ASGNI4
line 612
;612:	}
ADDRGP4 $215
JUMPV
LABELV $214
line 614
;613:	else
;614:	{
line 615
;615:		ent->s.trickedentindex3 = 3;
ADDRFP4 0
INDIRP4
CNSTI4 156
ADDP4
CNSTI4 3
ASGNI4
line 616
;616:	}
LABELV $215
LABELV $213
line 618
;617:
;618:	ent->physicsObject = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 500
ADDP4
CNSTI4 1
ASGNI4
line 620
;619:
;620:	VectorCopy(ent->s.pos.trBase, ent->s.origin2); //remember the spawn spot
ADDRLP4 1128
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1128
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 1128
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 622
;621:
;622:	ent->touch = HolocronTouch;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 HolocronTouch
ASGNP4
line 624
;623:
;624:	trap_LinkEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 626
;625:
;626:	ent->think = HolocronThink;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 HolocronThink
ASGNP4
line 627
;627:	ent->nextthink = level.time + 50;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 50
ADDI4
ASGNI4
line 628
;628:}
LABELV $190
endproc SP_misc_holocron 1132 28
export Use_Shooter
proc Use_Shooter 64 16
line 638
;629:
;630:/*
;631:======================================================================
;632:
;633:  SHOOTERS
;634:
;635:======================================================================
;636:*/
;637:
;638:void Use_Shooter( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
line 644
;639:	vec3_t		dir;
;640:	float		deg;
;641:	vec3_t		up, right;
;642:
;643:	// see if we have a target
;644:	if ( ent->enemy ) {
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $218
line 645
;645:		VectorSubtract( ent->enemy->r.currentOrigin, ent->s.origin, dir );
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
ADDRLP4 40
INDIRP4
CNSTI4 728
ADDP4
ASGNP4
ADDRLP4 0
ADDRLP4 44
INDIRP4
INDIRP4
CNSTI4 368
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 44
INDIRP4
INDIRP4
CNSTI4 372
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0+8
ADDRLP4 48
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 376
ADDP4
INDIRF4
ADDRLP4 48
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
SUBF4
ASGNF4
line 646
;646:		VectorNormalize( dir );
ADDRLP4 0
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 647
;647:	} else {
ADDRGP4 $219
JUMPV
LABELV $218
line 648
;648:		VectorCopy( ent->movedir, dir );
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 612
ADDP4
INDIRB
ASGNB 12
line 649
;649:	}
LABELV $219
line 652
;650:
;651:	// randomize a bit
;652:	PerpendicularVector( up, dir );
ADDRLP4 16
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 PerpendicularVector
CALLV
pop
line 653
;653:	CrossProduct( up, dir, right );
ADDRLP4 16
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 28
ARGP4
ADDRGP4 CrossProduct
CALLV
pop
line 655
;654:
;655:	deg = crandom() * ent->random;
ADDRLP4 40
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 12
CNSTF4 1073741824
ADDRLP4 40
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
ASGNF4
line 656
;656:	VectorMA( dir, deg, up, dir );
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 16
INDIRF4
ADDRLP4 12
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 16+4
INDIRF4
ADDRLP4 12
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 16+8
INDIRF4
ADDRLP4 12
INDIRF4
MULF4
ADDF4
ASGNF4
line 658
;657:
;658:	deg = crandom() * ent->random;
ADDRLP4 48
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 12
CNSTF4 1073741824
ADDRLP4 48
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
ASGNF4
line 659
;659:	VectorMA( dir, deg, right, dir );
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 28
INDIRF4
ADDRLP4 12
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 28+4
INDIRF4
ADDRLP4 12
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRLP4 28+8
INDIRF4
ADDRLP4 12
INDIRF4
MULF4
ADDF4
ASGNF4
line 661
;660:
;661:	VectorNormalize( dir );
ADDRLP4 0
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 663
;662:
;663:	switch ( ent->s.weapon ) {
ADDRLP4 56
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 4
EQI4 $237
ADDRGP4 $234
JUMPV
LABELV $237
line 665
;664:	case WP_BLASTER:
;665:		WP_FireBlasterMissile( ent, ent->s.origin, dir, qfalse );
ADDRLP4 60
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRLP4 60
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 WP_FireBlasterMissile
CALLV
pop
line 666
;666:		break;
LABELV $234
LABELV $235
line 669
;667:	}
;668:
;669:	G_AddEvent( ent, EV_FIRE_WEAPON, 0 );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 24
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 670
;670:}
LABELV $217
endproc Use_Shooter 64 16
proc InitShooter_Finish 8 4
line 673
;671:
;672:
;673:static void InitShooter_Finish( gentity_t *ent ) {
line 674
;674:	ent->enemy = G_PickTarget( ent->target );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 G_PickTarget
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRLP4 4
INDIRP4
ASGNP4
line 675
;675:	ent->think = 0;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
CNSTP4 0
ASGNP4
line 676
;676:	ent->nextthink = 0;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
CNSTI4 0
ASGNI4
line 677
;677:}
LABELV $238
endproc InitShooter_Finish 8 4
export InitShooter
proc InitShooter 16 8
line 679
;678:
;679:void InitShooter( gentity_t *ent, int weapon ) {
line 680
;680:	ent->use = Use_Shooter;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 Use_Shooter
ASGNP4
line 681
;681:	ent->s.weapon = weapon;
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 683
;682:
;683:	RegisterItem( BG_FindItemForWeapon( weapon ) );
ADDRFP4 4
INDIRI4
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
line 685
;684:
;685:	G_SetMovedir( ent->s.angles, ent->movedir );
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 116
ADDP4
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 612
ADDP4
ARGP4
ADDRGP4 G_SetMovedir
CALLV
pop
line 687
;686:
;687:	if ( !ent->random ) {
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
INDIRF4
CNSTF4 0
NEF4 $240
line 688
;688:		ent->random = 1.0;
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
CNSTF4 1065353216
ASGNF4
line 689
;689:	}
LABELV $240
line 690
;690:	ent->random = sin( M_PI * ent->random / 180 );
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
ASGNP4
CNSTF4 1078530011
ADDRLP4 8
INDIRP4
INDIRF4
MULF4
CNSTF4 1127481344
DIVF4
ARGF4
ADDRLP4 12
ADDRGP4 sin
CALLF4
ASGNF4
ADDRLP4 8
INDIRP4
ADDRLP4 12
INDIRF4
ASGNF4
line 692
;691:	// target might be a moving object, so we can't set movedir for it
;692:	if ( ent->target ) {
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $242
line 693
;693:		ent->think = InitShooter_Finish;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 InitShooter_Finish
ASGNP4
line 694
;694:		ent->nextthink = level.time + 500;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 500
ADDI4
ASGNI4
line 695
;695:	}
LABELV $242
line 696
;696:	trap_LinkEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 697
;697:}
LABELV $239
endproc InitShooter 16 8
export SP_shooter_blaster
proc SP_shooter_blaster 0 8
line 703
;698:
;699:/*QUAKED shooter_blaster (1 0 0) (-16 -16 -16) (16 16 16)
;700:Fires at either the target or the current direction.
;701:"random" is the number of degrees of deviance from the taget. (1.0 default)
;702:*/
;703:void SP_shooter_blaster( gentity_t *ent ) {
line 704
;704:	InitShooter( ent, WP_BLASTER);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 4
ARGI4
ADDRGP4 InitShooter
CALLV
pop
line 705
;705:}
LABELV $245
endproc SP_shooter_blaster 0 8
export check_recharge
proc check_recharge 8 0
line 708
;706:
;707:void check_recharge(gentity_t *ent)
;708:{
line 709
;709:	if (ent->fly_sound_debounce_time < level.time)
ADDRFP4 0
INDIRP4
CNSTI4 668
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $247
line 710
;710:	{
line 711
;711:		ent->s.loopSound = 0;
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
CNSTI4 0
ASGNI4
line 712
;712:	}
LABELV $247
line 714
;713:
;714:	if (ent->count < ent->boltpoint4)
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ADDRLP4 0
INDIRP4
CNSTI4 780
ADDP4
INDIRI4
GEI4 $250
line 715
;715:	{
line 716
;716:		ent->count++;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 712
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
line 717
;717:	}
LABELV $250
line 719
;718:
;719:	ent->nextthink = level.time + ent->bolt_Head;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
ADDI4
ASGNI4
line 720
;720:}
LABELV $246
endproc check_recharge 8 0
export EnergyShieldStationSettings
proc EnergyShieldStationSettings 0 12
line 728
;721:
;722:/*
;723:================
;724:EnergyShieldStationSettings
;725:================
;726:*/
;727:void EnergyShieldStationSettings(gentity_t *ent)
;728:{
line 729
;729:	G_SpawnInt( "count", "0", &ent->count );
ADDRGP4 $254
ARGP4
ADDRGP4 $112
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 731
;730:
;731:	if (!ent->count)
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
NEI4 $255
line 732
;732:	{
line 733
;733:		ent->count = 50; 
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
CNSTI4 50
ASGNI4
line 734
;734:	}
LABELV $255
line 736
;735:
;736:	G_SpawnInt("chargerate", "0", &ent->bolt_Head);
ADDRGP4 $257
ARGP4
ADDRGP4 $112
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 738
;737:
;738:	if (!ent->bolt_Head)
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
CNSTI4 0
NEI4 $258
line 739
;739:	{
line 740
;740:		ent->bolt_Head = STATION_RECHARGE_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
CNSTI4 3000
ASGNI4
line 741
;741:	}
LABELV $258
line 742
;742:}
LABELV $253
endproc EnergyShieldStationSettings 0 12
export shield_power_converter_use
proc shield_power_converter_use 32 12
line 750
;743:
;744:/*
;745:================
;746:shield_power_converter_use
;747:================
;748:*/
;749:void shield_power_converter_use( gentity_t *self, gentity_t *other, gentity_t *activator)
;750:{
line 752
;751:	int dif,add;
;752:	int stop = 1;
ADDRLP4 0
CNSTI4 1
ASGNI4
line 754
;753:
;754:	if (!activator || !activator->client)
ADDRLP4 12
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 16
CNSTU4 0
ASGNU4
ADDRLP4 12
INDIRP4
CVPU4 4
ADDRLP4 16
INDIRU4
EQU4 $263
ADDRLP4 12
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 16
INDIRU4
NEU4 $261
LABELV $263
line 755
;755:	{
line 756
;756:		return;
ADDRGP4 $260
JUMPV
LABELV $261
line 759
;757:	}
;758:
;759:	if (self->setTime < level.time)
ADDRFP4 0
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $264
line 760
;760:	{
line 761
;761:		if (!self->s.loopSound)
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
INDIRI4
CNSTI4 0
NEI4 $267
line 762
;762:		{
line 763
;763:			self->s.loopSound = G_SoundIndex("sound/interface/shieldcon_run.wav");
ADDRGP4 $269
ARGP4
ADDRLP4 20
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
ADDRLP4 20
INDIRI4
ASGNI4
line 764
;764:		}
LABELV $267
line 765
;765:		self->setTime = level.time + 100;
ADDRFP4 0
INDIRP4
CNSTI4 628
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 767
;766:
;767:		dif = activator->client->ps.stats[STAT_MAX_HEALTH] - activator->client->ps.stats[STAT_ARMOR];
ADDRLP4 20
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 20
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
ADDRLP4 20
INDIRP4
CNSTI4 236
ADDP4
INDIRI4
SUBI4
ASGNI4
line 769
;768:
;769:		if (dif > 0)					// Already at full armor?
ADDRLP4 4
INDIRI4
CNSTI4 0
LEI4 $271
line 770
;770:		{
line 771
;771:			if (dif >MAX_AMMO_GIVE)
ADDRLP4 4
INDIRI4
CNSTI4 2
LEI4 $273
line 772
;772:			{
line 773
;773:				add = MAX_AMMO_GIVE;
ADDRLP4 8
CNSTI4 2
ASGNI4
line 774
;774:			}
ADDRGP4 $274
JUMPV
LABELV $273
line 776
;775:			else
;776:			{
line 777
;777:				add = dif;
ADDRLP4 8
ADDRLP4 4
INDIRI4
ASGNI4
line 778
;778:			}
LABELV $274
line 780
;779:
;780:			if (self->count<add)
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ADDRLP4 8
INDIRI4
GEI4 $275
line 781
;781:			{
line 782
;782:				add = self->count;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 783
;783:			}
LABELV $275
line 785
;784:
;785:			self->count -= add;
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
ADDRLP4 8
INDIRI4
SUBI4
ASGNI4
line 786
;786:			if (self->count <= 0)
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
GTI4 $277
line 787
;787:			{
line 788
;788:				self->setTime = 0;
ADDRFP4 0
INDIRP4
CNSTI4 628
ADDP4
CNSTI4 0
ASGNI4
line 789
;789:			}
LABELV $277
line 790
;790:			stop = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 792
;791:
;792:			self->fly_sound_debounce_time = level.time + 50;
ADDRFP4 0
INDIRP4
CNSTI4 668
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 50
ADDI4
ASGNI4
line 794
;793:
;794:			activator->client->ps.stats[STAT_ARMOR] += add;
ADDRLP4 28
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 236
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
ADDRLP4 8
INDIRI4
ADDI4
ASGNI4
line 795
;795:		}
LABELV $271
line 796
;796:	}
LABELV $264
line 798
;797:
;798:	if (stop || self->count <= 0)
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 20
INDIRI4
NEI4 $282
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ADDRLP4 20
INDIRI4
GTI4 $280
LABELV $282
line 799
;799:	{
line 800
;800:		if (self->s.loopSound && self->setTime < level.time)
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CNSTI4 200
ADDP4
INDIRI4
CNSTI4 0
EQI4 $283
ADDRLP4 24
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $283
line 801
;801:		{
line 802
;802:			G_Sound(self, CHAN_AUTO, G_SoundIndex("sound/interface/shieldcon_done.mp3"));
ADDRGP4 $286
ARGP4
ADDRLP4 28
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 28
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 803
;803:		}
LABELV $283
line 804
;804:		self->s.loopSound = 0;
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
CNSTI4 0
ASGNI4
line 805
;805:		if (self->setTime < level.time)
ADDRFP4 0
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $287
line 806
;806:		{
line 807
;807:			self->setTime = level.time + self->bolt_Head+100;
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 628
ADDP4
ADDRGP4 level+32
INDIRI4
ADDRLP4 28
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
ADDI4
CNSTI4 100
ADDI4
ASGNI4
line 808
;808:		}
LABELV $287
line 809
;809:	}
LABELV $280
line 810
;810:}
LABELV $260
endproc shield_power_converter_use 32 12
export SP_misc_shield_floor_unit
proc SP_misc_shield_floor_unit 1136 28
line 815
;811:
;812://QED comment is in bg_misc
;813://------------------------------------------------------------
;814:void SP_misc_shield_floor_unit( gentity_t *ent )
;815:{
line 819
;816:	vec3_t dest;
;817:	trace_t tr;
;818:
;819:	if (g_gametype.integer != GT_CTF &&
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 7
EQI4 $292
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 8
EQI4 $292
line 821
;820:		g_gametype.integer != GT_CTY)
;821:	{
line 822
;822:		G_FreeEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 823
;823:		return;
ADDRGP4 $291
JUMPV
LABELV $292
line 826
;824:	}
;825:
;826:	VectorSet( ent->r.mins, -16, -16, 0 );
ADDRFP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3246391296
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3246391296
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 0
ASGNF4
line 827
;827:	VectorSet( ent->r.maxs, 16, 16, 40 );
ADDRFP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1098907648
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1098907648
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1109393408
ASGNF4
line 829
;828:
;829:	ent->s.origin[2] += 0.1;
ADDRLP4 1092
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
ASGNP4
ADDRLP4 1092
INDIRP4
ADDRLP4 1092
INDIRP4
INDIRF4
CNSTF4 1036831949
ADDF4
ASGNF4
line 830
;830:	ent->r.maxs[2] -= 0.1;
ADDRLP4 1096
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
ASGNP4
ADDRLP4 1096
INDIRP4
ADDRLP4 1096
INDIRP4
INDIRF4
CNSTF4 1036831949
SUBF4
ASGNF4
line 832
;831:
;832:	VectorSet( dest, ent->s.origin[0], ent->s.origin[1], ent->s.origin[2] - 4096 );
ADDRLP4 1100
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 1100
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 1100
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ASGNF4
ADDRLP4 0+8
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
CNSTF4 1166016512
SUBF4
ASGNF4
line 833
;833:	trap_Trace( &tr, ent->s.origin, ent->r.mins, ent->r.maxs, dest, ent->s.number, MASK_SOLID );
ADDRLP4 12
ARGP4
ADDRLP4 1104
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1104
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 1104
INDIRP4
CNSTI4 316
ADDP4
ARGP4
ADDRLP4 1104
INDIRP4
CNSTI4 328
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 1104
INDIRP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 834
;834:	if ( tr.startsolid )
ADDRLP4 12+4
INDIRI4
CNSTI4 0
EQI4 $298
line 835
;835:	{
line 836
;836:		G_Printf ("SP_misc_shield_floor_unit: misc_shield_floor_unit startsolid at %s\n", vtos(ent->s.origin));
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 1108
ADDRGP4 vtos
CALLP4
ASGNP4
ADDRGP4 $301
ARGP4
ADDRLP4 1108
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 837
;837:		G_FreeEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 838
;838:		return;
ADDRGP4 $291
JUMPV
LABELV $298
line 842
;839:	}
;840:
;841:	//add the 0.1 back after the trace
;842:	ent->r.maxs[2] += 0.1;
ADDRLP4 1108
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
ASGNP4
ADDRLP4 1108
INDIRP4
ADDRLP4 1108
INDIRP4
INDIRF4
CNSTF4 1036831949
ADDF4
ASGNF4
line 845
;843:
;844:	// allow to ride movers
;845:	ent->s.groundEntityNum = tr.entityNum;
ADDRFP4 0
INDIRP4
CNSTI4 192
ADDP4
ADDRLP4 12+52
INDIRI4
ASGNI4
line 847
;846:
;847:	G_SetOrigin( ent, tr.endpos );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12+12
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 849
;848:
;849:	if (!ent->health)
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 0
NEI4 $304
line 850
;850:	{
line 851
;851:		ent->health = 60;
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 60
ASGNI4
line 852
;852:	}
LABELV $304
line 854
;853:
;854:	if (!ent->model || !ent->model[0])
ADDRLP4 1112
ADDRFP4 0
INDIRP4
CNSTI4 476
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1112
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $308
ADDRLP4 1112
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $306
LABELV $308
line 855
;855:	{
line 856
;856:		ent->model = "/models/items/a_shield_converter.md3";
ADDRFP4 0
INDIRP4
CNSTI4 476
ADDP4
ADDRGP4 $309
ASGNP4
line 857
;857:	}
LABELV $306
line 859
;858:
;859:	ent->s.modelindex = G_ModelIndex( ent->model );
ADDRLP4 1116
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1116
INDIRP4
CNSTI4 476
ADDP4
INDIRP4
ARGP4
ADDRLP4 1120
ADDRGP4 G_ModelIndex
CALLI4
ASGNI4
ADDRLP4 1116
INDIRP4
CNSTI4 212
ADDP4
ADDRLP4 1120
INDIRI4
ASGNI4
line 861
;860:
;861:	ent->s.eFlags = 0;
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 0
ASGNI4
line 862
;862:	ent->r.svFlags |= SVF_PLAYER_USABLE;
ADDRLP4 1124
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 1124
INDIRP4
ADDRLP4 1124
INDIRP4
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 863
;863:	ent->r.contents = CONTENTS_SOLID;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1
ASGNI4
line 864
;864:	ent->clipmask = MASK_SOLID;
ADDRFP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 1
ASGNI4
line 866
;865:
;866:	EnergyShieldStationSettings(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 EnergyShieldStationSettings
CALLV
pop
line 868
;867:
;868:	ent->boltpoint4 = ent->count; //initial value
ADDRLP4 1128
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1128
INDIRP4
CNSTI4 780
ADDP4
ADDRLP4 1128
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 869
;869:	ent->think = check_recharge;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 check_recharge
ASGNP4
line 870
;870:	ent->nextthink = level.time + STATION_RECHARGE_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 3000
ADDI4
ASGNI4
line 872
;871:
;872:	ent->use = shield_power_converter_use;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 shield_power_converter_use
ASGNP4
line 874
;873:
;874:	VectorCopy( ent->s.angles, ent->s.apos.trBase );
ADDRLP4 1132
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1132
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 1132
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 875
;875:	trap_LinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 877
;876:
;877:	G_SoundIndex("sound/interface/shieldcon_run.wav");
ADDRGP4 $269
ARGP4
ADDRGP4 G_SoundIndex
CALLI4
pop
line 878
;878:	G_SoundIndex("sound/interface/shieldcon_done.mp3");
ADDRGP4 $286
ARGP4
ADDRGP4 G_SoundIndex
CALLI4
pop
line 879
;879:	G_SoundIndex("sound/interface/shieldcon_empty.mp3");
ADDRGP4 $311
ARGP4
ADDRGP4 G_SoundIndex
CALLI4
pop
line 880
;880:}
LABELV $291
endproc SP_misc_shield_floor_unit 1136 28
export SP_misc_model_shield_power_converter
proc SP_misc_model_shield_power_converter 28 8
line 891
;881:
;882:
;883:/*QUAKED misc_model_shield_power_converter (1 0 0) (-16 -16 -16) (16 16 16)
;884:#MODELNAME="models/items/psd_big.md3"
;885:Gives shield energy when used.
;886:
;887:"count" - the amount of ammo given when used (default 100)
;888:*/
;889://------------------------------------------------------------
;890:void SP_misc_model_shield_power_converter( gentity_t *ent )
;891:{
line 892
;892:	if (!ent->health)
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 0
NEI4 $313
line 893
;893:	{
line 894
;894:		ent->health = 60;
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 60
ASGNI4
line 895
;895:	}
LABELV $313
line 897
;896:
;897:	VectorSet (ent->r.mins, -16, -16, -16);
ADDRFP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3246391296
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3246391296
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3246391296
ASGNF4
line 898
;898:	VectorSet (ent->r.maxs, 16, 16, 16);
ADDRFP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1098907648
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1098907648
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1098907648
ASGNF4
line 900
;899:
;900:	ent->s.modelindex = G_ModelIndex( ent->model );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 476
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 G_ModelIndex
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 212
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 902
;901:
;902:	ent->s.eFlags = 0;
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 0
ASGNI4
line 903
;903:	ent->r.svFlags |= SVF_PLAYER_USABLE;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 904
;904:	ent->r.contents = CONTENTS_SOLID;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1
ASGNI4
line 905
;905:	ent->clipmask = MASK_SOLID;
ADDRFP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 1
ASGNI4
line 907
;906:
;907:	EnergyShieldStationSettings(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 EnergyShieldStationSettings
CALLV
pop
line 909
;908:
;909:	ent->boltpoint4 = ent->count; //initial value
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 780
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 910
;910:	ent->think = check_recharge;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 check_recharge
ASGNP4
line 911
;911:	ent->nextthink = level.time + STATION_RECHARGE_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 3000
ADDI4
ASGNI4
line 913
;912:
;913:	ent->use = shield_power_converter_use;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 shield_power_converter_use
ASGNP4
line 915
;914:
;915:	G_SetOrigin( ent, ent->s.origin );
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 16
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 916
;916:	VectorCopy( ent->s.angles, ent->s.apos.trBase );
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 20
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 917
;917:	trap_LinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 919
;918:
;919:	G_SoundIndex("sound/movers/objects/useshieldstation.wav");
ADDRGP4 $316
ARGP4
ADDRGP4 G_SoundIndex
CALLI4
pop
line 921
;920:
;921:	ent->s.modelindex2 = G_ModelIndex("/models/items/psd_big.md3");	// Precache model
ADDRGP4 $317
ARGP4
ADDRLP4 24
ADDRGP4 G_ModelIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 216
ADDP4
ADDRLP4 24
INDIRI4
ASGNI4
line 922
;922:}
LABELV $312
endproc SP_misc_model_shield_power_converter 28 8
export EnergyAmmoStationSettings
proc EnergyAmmoStationSettings 0 12
line 931
;923:
;924:
;925:/*
;926:================
;927:EnergyAmmoShieldStationSettings
;928:================
;929:*/
;930:void EnergyAmmoStationSettings(gentity_t *ent)
;931:{
line 932
;932:	G_SpawnInt( "count", "0", &ent->count );
ADDRGP4 $254
ARGP4
ADDRGP4 $112
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 934
;933:
;934:	if (!ent->count)
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
NEI4 $319
line 935
;935:	{
line 936
;936:		ent->count = 100; 
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
CNSTI4 100
ASGNI4
line 937
;937:	}
LABELV $319
line 938
;938:}
LABELV $318
endproc EnergyAmmoStationSettings 0 12
export ammo_power_converter_use
proc ammo_power_converter_use 52 4
line 946
;939:
;940:/*
;941:================
;942:ammo_power_converter_use
;943:================
;944:*/
;945:void ammo_power_converter_use( gentity_t *self, gentity_t *other, gentity_t *activator)
;946:{
line 950
;947:	int			add,highest;
;948:	qboolean	overcharge;
;949:	int			difBlaster,difPowerCell,difMetalBolts;
;950:	int			stop = 1;
ADDRLP4 0
CNSTI4 1
ASGNI4
line 952
;951:
;952:	if (!activator || !activator->client)
ADDRLP4 28
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 32
CNSTU4 0
ASGNU4
ADDRLP4 28
INDIRP4
CVPU4 4
ADDRLP4 32
INDIRU4
EQU4 $324
ADDRLP4 28
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 32
INDIRU4
NEU4 $322
LABELV $324
line 953
;953:	{
line 954
;954:		return;
ADDRGP4 $321
JUMPV
LABELV $322
line 957
;955:	}
;956:
;957:	if (self->setTime < level.time)
ADDRFP4 0
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $325
line 958
;958:	{
line 959
;959:		overcharge = qfalse;
ADDRLP4 24
CNSTI4 0
ASGNI4
line 961
;960:
;961:		if (!self->s.loopSound)
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
INDIRI4
CNSTI4 0
NEI4 $328
line 962
;962:		{
line 963
;963:			self->s.loopSound = G_SoundIndex("sound/player/pickupshield.wav");
ADDRGP4 $330
ARGP4
ADDRLP4 36
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
line 964
;964:		}
LABELV $328
line 966
;965:
;966:		self->setTime = level.time + 100;
ADDRFP4 0
INDIRP4
CNSTI4 628
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 968
;967:
;968:		if (self->count)	// Has it got any power left?
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
EQI4 $332
line 969
;969:		{
line 970
;970:			if (self->count > MAX_AMMO_GIVE)
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 2
LEI4 $334
line 971
;971:			{
line 972
;972:				add = MAX_AMMO_GIVE;
ADDRLP4 4
CNSTI4 2
ASGNI4
line 973
;973:			}
ADDRGP4 $335
JUMPV
LABELV $334
line 974
;974:			else if (self->count<0)
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
GEI4 $336
line 975
;975:			{
line 976
;976:				add = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 977
;977:			}
ADDRGP4 $337
JUMPV
LABELV $336
line 979
;978:			else
;979:			{
line 980
;980:				add = self->count;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 981
;981:			}
LABELV $337
LABELV $335
line 983
;982:
;983:			activator->client->ps.ammo[AMMO_BLASTER] += add;
ADDRLP4 36
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 416
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ASGNI4
line 984
;984:			activator->client->ps.ammo[AMMO_POWERCELL] += add;
ADDRLP4 40
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 420
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ASGNI4
line 985
;985:			activator->client->ps.ammo[AMMO_METAL_BOLTS] += add;
ADDRLP4 44
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 424
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ASGNI4
line 987
;986:
;987:			self->count -= add;
ADDRLP4 48
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
ASGNI4
line 988
;988:			stop = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 990
;989:
;990:			self->fly_sound_debounce_time = level.time + 50;
ADDRFP4 0
INDIRP4
CNSTI4 668
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 50
ADDI4
ASGNI4
line 992
;991:
;992:			difBlaster = activator->client->ps.ammo[AMMO_BLASTER] - ammoData[AMMO_BLASTER].max;
ADDRLP4 12
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 416
ADDP4
INDIRI4
ADDRGP4 ammoData+8
INDIRI4
SUBI4
ASGNI4
line 993
;993:			difPowerCell = activator->client->ps.ammo[AMMO_POWERCELL] - ammoData[AMMO_POWERCELL].max;
ADDRLP4 16
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
ADDRGP4 ammoData+12
INDIRI4
SUBI4
ASGNI4
line 994
;994:			difMetalBolts = activator->client->ps.ammo[AMMO_METAL_BOLTS] - ammoData[AMMO_METAL_BOLTS].max;
ADDRLP4 20
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
ADDRGP4 ammoData+16
INDIRI4
SUBI4
ASGNI4
line 997
;995:
;996:			// Find the highest one
;997:			highest = difBlaster;
ADDRLP4 8
ADDRLP4 12
INDIRI4
ASGNI4
line 998
;998:			if (difPowerCell>difBlaster)
ADDRLP4 16
INDIRI4
ADDRLP4 12
INDIRI4
LEI4 $342
line 999
;999:			{
line 1000
;1000:				highest = difPowerCell;
ADDRLP4 8
ADDRLP4 16
INDIRI4
ASGNI4
line 1001
;1001:			}
LABELV $342
line 1003
;1002:
;1003:			if (difMetalBolts > highest)
ADDRLP4 20
INDIRI4
ADDRLP4 8
INDIRI4
LEI4 $344
line 1004
;1004:			{
line 1005
;1005:				highest = difMetalBolts;
ADDRLP4 8
ADDRLP4 20
INDIRI4
ASGNI4
line 1006
;1006:			}
LABELV $344
line 1007
;1007:		}
LABELV $332
line 1008
;1008:	}
LABELV $325
line 1010
;1009:
;1010:	if (stop)
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $346
line 1011
;1011:	{
line 1012
;1012:		self->s.loopSound = 0;
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
CNSTI4 0
ASGNI4
line 1013
;1013:	}
LABELV $346
line 1014
;1014:}
LABELV $321
endproc ammo_power_converter_use 52 4
export SP_misc_model_ammo_power_converter
proc SP_misc_model_ammo_power_converter 24 8
line 1025
;1015:
;1016:
;1017:/*QUAKED misc_model_ammo_power_converter (1 0 0) (-16 -16 -16) (16 16 16)
;1018:#MODELNAME="models/items/power_converter.md3"
;1019:Gives ammo energy when used.
;1020:
;1021:"count" - the amount of ammo given when used (default 100)
;1022:*/
;1023://------------------------------------------------------------
;1024:void SP_misc_model_ammo_power_converter( gentity_t *ent )
;1025:{
line 1026
;1026:	if (!ent->health)
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 0
NEI4 $349
line 1027
;1027:	{
line 1028
;1028:		ent->health = 60;
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 60
ASGNI4
line 1029
;1029:	}
LABELV $349
line 1031
;1030:
;1031:	VectorSet (ent->r.mins, -16, -16, -16);
ADDRFP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3246391296
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3246391296
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3246391296
ASGNF4
line 1032
;1032:	VectorSet (ent->r.maxs, 16, 16, 16);
ADDRFP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1098907648
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1098907648
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1098907648
ASGNF4
line 1034
;1033:
;1034:	ent->s.modelindex = G_ModelIndex( ent->model );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 476
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 G_ModelIndex
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 212
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 1036
;1035:
;1036:	ent->s.eFlags = 0;
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 0
ASGNI4
line 1037
;1037:	ent->r.svFlags |= SVF_PLAYER_USABLE;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 1038
;1038:	ent->r.contents = CONTENTS_SOLID;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1
ASGNI4
line 1039
;1039:	ent->clipmask = MASK_SOLID;
ADDRFP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 1
ASGNI4
line 1041
;1040:
;1041:	ent->use = ammo_power_converter_use;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 ammo_power_converter_use
ASGNP4
line 1043
;1042:
;1043:	EnergyAmmoStationSettings(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 EnergyAmmoStationSettings
CALLV
pop
line 1045
;1044:
;1045:	ent->boltpoint4 = ent->count; //initial value
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 780
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 1046
;1046:	ent->think = check_recharge;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 check_recharge
ASGNP4
line 1047
;1047:	ent->nextthink = level.time + STATION_RECHARGE_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 3000
ADDI4
ASGNI4
line 1049
;1048:
;1049:	G_SetOrigin( ent, ent->s.origin );
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 16
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1050
;1050:	VectorCopy( ent->s.angles, ent->s.apos.trBase );
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 20
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 1051
;1051:	trap_LinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1053
;1052:
;1053:	G_SoundIndex("sound/movers/objects/useshieldstation.wav");
ADDRGP4 $316
ARGP4
ADDRGP4 G_SoundIndex
CALLI4
pop
line 1054
;1054:}
LABELV $348
endproc SP_misc_model_ammo_power_converter 24 8
export EnergyHealthStationSettings
proc EnergyHealthStationSettings 0 12
line 1062
;1055:
;1056:/*
;1057:================
;1058:EnergyHealthStationSettings
;1059:================
;1060:*/
;1061:void EnergyHealthStationSettings(gentity_t *ent)
;1062:{
line 1063
;1063:	G_SpawnInt( "count", "0", &ent->count );
ADDRGP4 $254
ARGP4
ADDRGP4 $112
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 1065
;1064:
;1065:	if (!ent->count)
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
NEI4 $353
line 1066
;1066:	{
line 1067
;1067:		ent->count = 100; 
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
CNSTI4 100
ASGNI4
line 1068
;1068:	}
LABELV $353
line 1069
;1069:}
LABELV $352
endproc EnergyHealthStationSettings 0 12
export health_power_converter_use
proc health_power_converter_use 32 4
line 1077
;1070:
;1071:/*
;1072:================
;1073:health_power_converter_use
;1074:================
;1075:*/
;1076:void health_power_converter_use( gentity_t *self, gentity_t *other, gentity_t *activator)
;1077:{
line 1079
;1078:	int dif,add;
;1079:	int stop = 1;
ADDRLP4 0
CNSTI4 1
ASGNI4
line 1081
;1080:
;1081:	if (!activator || !activator->client)
ADDRLP4 12
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 16
CNSTU4 0
ASGNU4
ADDRLP4 12
INDIRP4
CVPU4 4
ADDRLP4 16
INDIRU4
EQU4 $358
ADDRLP4 12
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 16
INDIRU4
NEU4 $356
LABELV $358
line 1082
;1082:	{
line 1083
;1083:		return;
ADDRGP4 $355
JUMPV
LABELV $356
line 1086
;1084:	}
;1085:
;1086:	if (self->setTime < level.time)
ADDRFP4 0
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $359
line 1087
;1087:	{
line 1088
;1088:		if (!self->s.loopSound)
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
INDIRI4
CNSTI4 0
NEI4 $362
line 1089
;1089:		{
line 1090
;1090:			self->s.loopSound = G_SoundIndex("sound/player/pickuphealth.wav");
ADDRGP4 $364
ARGP4
ADDRLP4 20
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
ADDRLP4 20
INDIRI4
ASGNI4
line 1091
;1091:		}
LABELV $362
line 1092
;1092:		self->setTime = level.time + 100;
ADDRFP4 0
INDIRP4
CNSTI4 628
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 1094
;1093:
;1094:		dif = activator->client->ps.stats[STAT_MAX_HEALTH] - activator->health;
ADDRLP4 20
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 20
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
ADDRLP4 20
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
SUBI4
ASGNI4
line 1096
;1095:
;1096:		if (dif > 0)					// Already at full armor?
ADDRLP4 4
INDIRI4
CNSTI4 0
LEI4 $366
line 1097
;1097:		{
line 1098
;1098:			if (dif >MAX_AMMO_GIVE)
ADDRLP4 4
INDIRI4
CNSTI4 2
LEI4 $368
line 1099
;1099:			{
line 1100
;1100:				add = MAX_AMMO_GIVE;
ADDRLP4 8
CNSTI4 2
ASGNI4
line 1101
;1101:			}
ADDRGP4 $369
JUMPV
LABELV $368
line 1103
;1102:			else
;1103:			{
line 1104
;1104:				add = dif;
ADDRLP4 8
ADDRLP4 4
INDIRI4
ASGNI4
line 1105
;1105:			}
LABELV $369
line 1107
;1106:
;1107:			if (self->count<add)
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ADDRLP4 8
INDIRI4
GEI4 $370
line 1108
;1108:			{
line 1109
;1109:				add = self->count;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 1110
;1110:			}
LABELV $370
line 1112
;1111:
;1112:			self->count -= add;
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
ADDRLP4 8
INDIRI4
SUBI4
ASGNI4
line 1113
;1113:			stop = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1115
;1114:
;1115:			self->fly_sound_debounce_time = level.time + 50;
ADDRFP4 0
INDIRP4
CNSTI4 668
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 50
ADDI4
ASGNI4
line 1117
;1116:
;1117:			activator->health += add;
ADDRLP4 28
ADDRFP4 8
INDIRP4
CNSTI4 676
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
ADDRLP4 8
INDIRI4
ADDI4
ASGNI4
line 1118
;1118:		}
LABELV $366
line 1119
;1119:	}
LABELV $359
line 1121
;1120:
;1121:	if (stop)
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $373
line 1122
;1122:	{
line 1123
;1123:		self->s.loopSound = 0;
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
CNSTI4 0
ASGNI4
line 1124
;1124:	}
LABELV $373
line 1125
;1125:}
LABELV $355
endproc health_power_converter_use 32 4
export SP_misc_model_health_power_converter
proc SP_misc_model_health_power_converter 24 8
line 1136
;1126:
;1127:
;1128:/*QUAKED misc_model_health_power_converter (1 0 0) (-16 -16 -16) (16 16 16)
;1129:#MODELNAME="models/items/power_converter.md3"
;1130:Gives ammo energy when used.
;1131:
;1132:"count" - the amount of ammo given when used (default 100)
;1133:*/
;1134://------------------------------------------------------------
;1135:void SP_misc_model_health_power_converter( gentity_t *ent )
;1136:{
line 1137
;1137:	if (!ent->health)
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 0
NEI4 $376
line 1138
;1138:	{
line 1139
;1139:		ent->health = 60;
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 60
ASGNI4
line 1140
;1140:	}
LABELV $376
line 1142
;1141:
;1142:	VectorSet (ent->r.mins, -16, -16, -16);
ADDRFP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3246391296
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3246391296
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3246391296
ASGNF4
line 1143
;1143:	VectorSet (ent->r.maxs, 16, 16, 16);
ADDRFP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1098907648
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1098907648
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1098907648
ASGNF4
line 1145
;1144:
;1145:	ent->s.modelindex = G_ModelIndex( ent->model );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 476
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 G_ModelIndex
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 212
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 1147
;1146:
;1147:	ent->s.eFlags = 0;
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 0
ASGNI4
line 1148
;1148:	ent->r.svFlags |= SVF_PLAYER_USABLE;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 1149
;1149:	ent->r.contents = CONTENTS_SOLID;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1
ASGNI4
line 1150
;1150:	ent->clipmask = MASK_SOLID;
ADDRFP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 1
ASGNI4
line 1152
;1151:
;1152:	ent->use = health_power_converter_use;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 health_power_converter_use
ASGNP4
line 1154
;1153:
;1154:	EnergyHealthStationSettings(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 EnergyHealthStationSettings
CALLV
pop
line 1156
;1155:
;1156:	ent->boltpoint4 = ent->count; //initial value
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 780
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ASGNI4
line 1157
;1157:	ent->think = check_recharge;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 check_recharge
ASGNP4
line 1158
;1158:	ent->nextthink = level.time + STATION_RECHARGE_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 3000
ADDI4
ASGNI4
line 1160
;1159:
;1160:	G_SetOrigin( ent, ent->s.origin );
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 16
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1161
;1161:	VectorCopy( ent->s.angles, ent->s.apos.trBase );
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 20
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 1162
;1162:	trap_LinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1164
;1163:
;1164:	G_SoundIndex("sound/movers/objects/useshieldstation.wav");
ADDRGP4 $316
ARGP4
ADDRGP4 G_SoundIndex
CALLI4
pop
line 1165
;1165:}
LABELV $375
endproc SP_misc_model_health_power_converter 24 8
export DmgBoxHit
proc DmgBoxHit 0 0
line 1168
;1166:
;1167:void DmgBoxHit( gentity_t *self, gentity_t *other, trace_t *trace )
;1168:{
line 1169
;1169:	return;
LABELV $379
endproc DmgBoxHit 0 0
export DmgBoxUpdateSelf
proc DmgBoxUpdateSelf 24 4
line 1173
;1170:}
;1171:
;1172:void DmgBoxUpdateSelf(gentity_t *self)
;1173:{
line 1174
;1174:	gentity_t *owner = &g_entities[self->r.ownerNum];
ADDRLP4 0
CNSTI4 828
ADDRFP4 0
INDIRP4
CNSTI4 396
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1176
;1175:
;1176:	if (!owner || !owner->client || !owner->inuse)
ADDRLP4 8
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 8
INDIRU4
EQU4 $384
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 8
INDIRU4
EQU4 $384
ADDRLP4 0
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
NEI4 $381
LABELV $384
line 1177
;1177:	{
line 1178
;1178:		goto killMe;
ADDRGP4 $385
JUMPV
LABELV $381
line 1181
;1179:	}
;1180:
;1181:	if (self->damageRedirect == DAMAGEREDIRECT_HEAD &&
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 816
ADDP4
INDIRI4
CNSTI4 1
NEI4 $386
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1636
ADDP4
INDIRI4
ADDRLP4 12
INDIRP4
INDIRI4
EQI4 $386
line 1183
;1182:		owner->client->damageBoxHandle_Head != self->s.number)
;1183:	{
line 1184
;1184:		goto killMe;
ADDRGP4 $385
JUMPV
LABELV $386
line 1187
;1185:	}
;1186:
;1187:	if (self->damageRedirect == DAMAGEREDIRECT_RLEG &&
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 816
ADDP4
INDIRI4
CNSTI4 2
NEI4 $388
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1640
ADDP4
INDIRI4
ADDRLP4 16
INDIRP4
INDIRI4
EQI4 $388
line 1189
;1188:		owner->client->damageBoxHandle_RLeg != self->s.number)
;1189:	{
line 1190
;1190:		goto killMe;
ADDRGP4 $385
JUMPV
LABELV $388
line 1193
;1191:	}
;1192:
;1193:	if (self->damageRedirect == DAMAGEREDIRECT_LLEG &&
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 816
ADDP4
INDIRI4
CNSTI4 3
NEI4 $390
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1644
ADDP4
INDIRI4
ADDRLP4 20
INDIRP4
INDIRI4
EQI4 $390
line 1195
;1194:		owner->client->damageBoxHandle_LLeg != self->s.number)
;1195:	{
line 1196
;1196:		goto killMe;
ADDRGP4 $385
JUMPV
LABELV $390
line 1199
;1197:	}
;1198:
;1199:	if (owner->health < 1)
ADDRLP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 1
GEI4 $392
line 1200
;1200:	{
line 1201
;1201:		goto killMe;
ADDRGP4 $385
JUMPV
LABELV $392
line 1206
;1202:	}
;1203:
;1204:	//G_TestLine(self->r.currentOrigin, owner->client->ps.origin, 0x0000ff, 100);
;1205:
;1206:	trap_LinkEntity(self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1208
;1207:
;1208:	self->nextthink = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1209
;1209:	return;
ADDRGP4 $380
JUMPV
LABELV $385
line 1212
;1210:
;1211:killMe:
;1212:	self->think = G_FreeEntity;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 G_FreeEntity
ASGNP4
line 1213
;1213:	self->nextthink = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1214
;1214:}
LABELV $380
endproc DmgBoxUpdateSelf 24 4
export DmgBoxAbsorb_Die
proc DmgBoxAbsorb_Die 0 0
line 1217
;1215:
;1216:void DmgBoxAbsorb_Die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod )
;1217:{
line 1218
;1218:	self->health = 1;
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 1
ASGNI4
line 1219
;1219:}
LABELV $396
endproc DmgBoxAbsorb_Die 0 0
export DmgBoxAbsorb_Pain
proc DmgBoxAbsorb_Pain 0 0
line 1222
;1220:
;1221:void DmgBoxAbsorb_Pain(gentity_t *self, gentity_t *attacker, int damage)
;1222:{
line 1223
;1223:	self->health = 1;
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 1
ASGNI4
line 1224
;1224:}
LABELV $397
endproc DmgBoxAbsorb_Pain 0 0
export CreateNewDamageBox
proc CreateNewDamageBox 16 0
line 1227
;1225:
;1226:gentity_t *CreateNewDamageBox( gentity_t *ent )
;1227:{
line 1232
;1228:	gentity_t *dmgBox;
;1229:
;1230:	//We do not want the client to have any real knowledge of the entity whatsoever. It will only
;1231:	//ever be used on the server.
;1232:	dmgBox = G_Spawn();
ADDRLP4 4
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 1233
;1233:	dmgBox->classname = "dmg_box";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $399
ASGNP4
line 1235
;1234:			
;1235:	dmgBox->r.svFlags = SVF_USE_CURRENT_ORIGIN;
ADDRLP4 0
INDIRP4
CNSTI4 304
ADDP4
CNSTI4 128
ASGNI4
line 1236
;1236:	dmgBox->r.ownerNum = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 396
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 1238
;1237:
;1238:	dmgBox->clipmask = 0;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 0
ASGNI4
line 1239
;1239:	dmgBox->r.contents = MASK_PLAYERSOLID;
ADDRLP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 273
ASGNI4
line 1241
;1240:
;1241:	dmgBox->mass = 5000;
ADDRLP4 0
INDIRP4
CNSTI4 624
ADDP4
CNSTF4 1167867904
ASGNF4
line 1243
;1242:
;1243:	dmgBox->s.eFlags |= EF_NODRAW;
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
CNSTI4 128
BORI4
ASGNI4
line 1244
;1244:	dmgBox->r.svFlags |= SVF_NOCLIENT;
ADDRLP4 12
ADDRLP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 1246
;1245:
;1246:	dmgBox->touch = DmgBoxHit;
ADDRLP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 DmgBoxHit
ASGNP4
line 1248
;1247:
;1248:	dmgBox->takedamage = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 1
ASGNI4
line 1250
;1249:
;1250:	dmgBox->health = 1;
ADDRLP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 1
ASGNI4
line 1252
;1251:
;1252:	dmgBox->pain = DmgBoxAbsorb_Pain;
ADDRLP4 0
INDIRP4
CNSTI4 656
ADDP4
ADDRGP4 DmgBoxAbsorb_Pain
ASGNP4
line 1253
;1253:	dmgBox->die = DmgBoxAbsorb_Die;
ADDRLP4 0
INDIRP4
CNSTI4 660
ADDP4
ADDRGP4 DmgBoxAbsorb_Die
ASGNP4
line 1255
;1254:
;1255:	dmgBox->think = DmgBoxUpdateSelf;
ADDRLP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 DmgBoxUpdateSelf
ASGNP4
line 1256
;1256:	dmgBox->nextthink = level.time + 50;
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 50
ADDI4
ASGNI4
line 1258
;1257:
;1258:	return dmgBox;
ADDRLP4 0
INDIRP4
RETP4
LABELV $398
endproc CreateNewDamageBox 16 0
export ATST_ManageDamageBoxes
proc ATST_ManageDamageBoxes 92 16
line 1262
;1259:}
;1260:
;1261:void ATST_ManageDamageBoxes(gentity_t *ent)
;1262:{
line 1266
;1263:	vec3_t headOrg, lLegOrg, rLegOrg;
;1264:	vec3_t fwd, right, up, flatAngle;
;1265:
;1266:	if (!ent->client->damageBoxHandle_Head)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1636
ADDP4
INDIRI4
CNSTI4 0
NEI4 $402
line 1267
;1267:	{
line 1268
;1268:		gentity_t *dmgBox = CreateNewDamageBox(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 88
ADDRGP4 CreateNewDamageBox
CALLP4
ASGNP4
ADDRLP4 84
ADDRLP4 88
INDIRP4
ASGNP4
line 1270
;1269:
;1270:		if (dmgBox)
ADDRLP4 84
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $404
line 1271
;1271:		{
line 1272
;1272:			VectorSet( dmgBox->r.mins, ATST_MINS0, ATST_MINS1, ATST_MINS2 );
ADDRLP4 84
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3256877056
ASGNF4
ADDRLP4 84
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3256877056
ASGNF4
ADDRLP4 84
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3250585600
ASGNF4
line 1273
;1273:			VectorSet( dmgBox->r.maxs, ATST_MAXS0, ATST_MAXS1, ATST_HEADSIZE );
ADDRLP4 84
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1109393408
ASGNF4
ADDRLP4 84
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1109393408
ASGNF4
ADDRLP4 84
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1119092736
ASGNF4
line 1275
;1274:
;1275:			ent->client->damageBoxHandle_Head = dmgBox->s.number;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1636
ADDP4
ADDRLP4 84
INDIRP4
INDIRI4
ASGNI4
line 1276
;1276:			dmgBox->damageRedirect = DAMAGEREDIRECT_HEAD;
ADDRLP4 84
INDIRP4
CNSTI4 816
ADDP4
CNSTI4 1
ASGNI4
line 1277
;1277:			dmgBox->damageRedirectTo = ent->s.number;
ADDRLP4 84
INDIRP4
CNSTI4 820
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 1278
;1278:		}
LABELV $404
line 1279
;1279:	}
LABELV $402
line 1280
;1280:	if (!ent->client->damageBoxHandle_RLeg)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1640
ADDP4
INDIRI4
CNSTI4 0
NEI4 $406
line 1281
;1281:	{
line 1282
;1282:		gentity_t *dmgBox = CreateNewDamageBox(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 88
ADDRGP4 CreateNewDamageBox
CALLP4
ASGNP4
ADDRLP4 84
ADDRLP4 88
INDIRP4
ASGNP4
line 1284
;1283:
;1284:		if (dmgBox)
ADDRLP4 84
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $408
line 1285
;1285:		{
line 1286
;1286:			VectorSet( dmgBox->r.mins, ATST_MINS0/4, ATST_MINS1/4, ATST_MINS2 );
ADDRLP4 84
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3240099840
ASGNF4
ADDRLP4 84
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3240099840
ASGNF4
ADDRLP4 84
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3250585600
ASGNF4
line 1287
;1287:			VectorSet( dmgBox->r.maxs, ATST_MAXS0/4, ATST_MAXS1/4, ATST_MAXS2-ATST_HEADSIZE );
ADDRLP4 84
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1092616192
ASGNF4
ADDRLP4 84
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1092616192
ASGNF4
ADDRLP4 84
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1126039552
ASGNF4
line 1289
;1288:
;1289:			ent->client->damageBoxHandle_RLeg = dmgBox->s.number;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1640
ADDP4
ADDRLP4 84
INDIRP4
INDIRI4
ASGNI4
line 1290
;1290:			dmgBox->damageRedirect = DAMAGEREDIRECT_RLEG;
ADDRLP4 84
INDIRP4
CNSTI4 816
ADDP4
CNSTI4 2
ASGNI4
line 1291
;1291:			dmgBox->damageRedirectTo = ent->s.number;
ADDRLP4 84
INDIRP4
CNSTI4 820
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 1292
;1292:		}
LABELV $408
line 1293
;1293:	}
LABELV $406
line 1294
;1294:	if (!ent->client->damageBoxHandle_LLeg)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1644
ADDP4
INDIRI4
CNSTI4 0
NEI4 $410
line 1295
;1295:	{
line 1296
;1296:		gentity_t *dmgBox = CreateNewDamageBox(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 88
ADDRGP4 CreateNewDamageBox
CALLP4
ASGNP4
ADDRLP4 84
ADDRLP4 88
INDIRP4
ASGNP4
line 1298
;1297:
;1298:		if (dmgBox)
ADDRLP4 84
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $412
line 1299
;1299:		{
line 1300
;1300:			VectorSet( dmgBox->r.mins, ATST_MINS0/4, ATST_MINS1/4, ATST_MINS2 );
ADDRLP4 84
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3240099840
ASGNF4
ADDRLP4 84
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3240099840
ASGNF4
ADDRLP4 84
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3250585600
ASGNF4
line 1301
;1301:			VectorSet( dmgBox->r.maxs, ATST_MAXS0/4, ATST_MAXS1/4, ATST_MAXS2-ATST_HEADSIZE );
ADDRLP4 84
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1092616192
ASGNF4
ADDRLP4 84
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1092616192
ASGNF4
ADDRLP4 84
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1126039552
ASGNF4
line 1303
;1302:
;1303:			ent->client->damageBoxHandle_LLeg = dmgBox->s.number;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1644
ADDP4
ADDRLP4 84
INDIRP4
INDIRI4
ASGNI4
line 1304
;1304:			dmgBox->damageRedirect = DAMAGEREDIRECT_LLEG;
ADDRLP4 84
INDIRP4
CNSTI4 816
ADDP4
CNSTI4 3
ASGNI4
line 1305
;1305:			dmgBox->damageRedirectTo = ent->s.number;
ADDRLP4 84
INDIRP4
CNSTI4 820
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 1306
;1306:		}
LABELV $412
line 1307
;1307:	}
LABELV $410
line 1309
;1308:
;1309:	if (!ent->client->damageBoxHandle_Head ||
ADDRLP4 84
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 88
CNSTI4 0
ASGNI4
ADDRLP4 84
INDIRP4
CNSTI4 1636
ADDP4
INDIRI4
ADDRLP4 88
INDIRI4
EQI4 $417
ADDRLP4 84
INDIRP4
CNSTI4 1644
ADDP4
INDIRI4
ADDRLP4 88
INDIRI4
EQI4 $417
ADDRLP4 84
INDIRP4
CNSTI4 1640
ADDP4
INDIRI4
ADDRLP4 88
INDIRI4
NEI4 $414
LABELV $417
line 1312
;1310:		!ent->client->damageBoxHandle_LLeg ||
;1311:		!ent->client->damageBoxHandle_RLeg)
;1312:	{
line 1313
;1313:		return;
ADDRGP4 $401
JUMPV
LABELV $414
line 1316
;1314:	}
;1315:
;1316:	VectorCopy(ent->client->ps.origin, headOrg);
ADDRLP4 48
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 1317
;1317:	headOrg[2] += (ATST_MAXS2-ATST_HEADSIZE);
ADDRLP4 48+8
ADDRLP4 48+8
INDIRF4
CNSTF4 1126039552
ADDF4
ASGNF4
line 1319
;1318:
;1319:	VectorCopy(ent->client->ps.viewangles, flatAngle);
ADDRLP4 36
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 156
ADDP4
INDIRB
ASGNB 12
line 1320
;1320:	flatAngle[PITCH] = 0;
ADDRLP4 36
CNSTF4 0
ASGNF4
line 1321
;1321:	flatAngle[ROLL] = 0;
ADDRLP4 36+8
CNSTF4 0
ASGNF4
line 1323
;1322:
;1323:	AngleVectors(flatAngle, fwd, right, up);
ADDRLP4 36
ARGP4
ADDRLP4 60
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 72
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 1325
;1324:
;1325:	VectorCopy(ent->client->ps.origin, lLegOrg);
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 1326
;1326:	VectorCopy(ent->client->ps.origin, rLegOrg);
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 1328
;1327:
;1328:	lLegOrg[0] -= right[0]*32;
ADDRLP4 12
ADDRLP4 12
INDIRF4
CNSTF4 1107296256
ADDRLP4 0
INDIRF4
MULF4
SUBF4
ASGNF4
line 1329
;1329:	lLegOrg[1] -= right[1]*32;
ADDRLP4 12+4
ADDRLP4 12+4
INDIRF4
CNSTF4 1107296256
ADDRLP4 0+4
INDIRF4
MULF4
SUBF4
ASGNF4
line 1330
;1330:	lLegOrg[2] -= right[2]*32;
ADDRLP4 12+8
ADDRLP4 12+8
INDIRF4
CNSTF4 1107296256
ADDRLP4 0+8
INDIRF4
MULF4
SUBF4
ASGNF4
line 1332
;1331:
;1332:	rLegOrg[0] += right[0]*32;
ADDRLP4 24
ADDRLP4 24
INDIRF4
CNSTF4 1107296256
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 1333
;1333:	rLegOrg[1] += right[1]*32;
ADDRLP4 24+4
ADDRLP4 24+4
INDIRF4
CNSTF4 1107296256
ADDRLP4 0+4
INDIRF4
MULF4
ADDF4
ASGNF4
line 1334
;1334:	rLegOrg[2] += right[2]*32;
ADDRLP4 24+8
ADDRLP4 24+8
INDIRF4
CNSTF4 1107296256
ADDRLP4 0+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 1336
;1335:
;1336:	G_SetOrigin(&g_entities[ent->client->damageBoxHandle_Head], headOrg);
CNSTI4 828
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1636
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRLP4 48
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1337
;1337:	G_SetOrigin(&g_entities[ent->client->damageBoxHandle_LLeg], lLegOrg);
CNSTI4 828
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1644
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRLP4 12
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1338
;1338:	G_SetOrigin(&g_entities[ent->client->damageBoxHandle_RLeg], rLegOrg);
CNSTI4 828
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1640
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRLP4 24
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1339
;1339:}
LABELV $401
endproc ATST_ManageDamageBoxes 92 16
export G_PlayerBecomeATST
proc G_PlayerBecomeATST 8 0
line 1342
;1340:
;1341:int G_PlayerBecomeATST(gentity_t *ent)
;1342:{
line 1343
;1343:	if (!ent || !ent->client)
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
EQU4 $431
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
NEU4 $429
LABELV $431
line 1344
;1344:	{
line 1345
;1345:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $428
JUMPV
LABELV $429
line 1348
;1346:	}
;1347:
;1348:	if (ent->client->ps.weaponTime > 0)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 0
LEI4 $432
line 1349
;1349:	{
line 1350
;1350:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $428
JUMPV
LABELV $432
line 1353
;1351:	}
;1352:
;1353:	if (ent->client->ps.forceHandExtend != HANDEXTEND_NONE)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1236
ADDP4
INDIRI4
CNSTI4 0
EQI4 $434
line 1354
;1354:	{
line 1355
;1355:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $428
JUMPV
LABELV $434
line 1358
;1356:	}
;1357:
;1358:	if (ent->client->ps.zoomMode)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1320
ADDP4
INDIRI4
CNSTI4 0
EQI4 $436
line 1359
;1359:	{
line 1360
;1360:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $428
JUMPV
LABELV $436
line 1363
;1361:	}
;1362:
;1363:	if (ent->client->ps.usingATST)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1304
ADDP4
INDIRI4
CNSTI4 0
EQI4 $438
line 1364
;1364:	{
line 1365
;1365:		ent->client->ps.usingATST = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1304
ADDP4
CNSTI4 0
ASGNI4
line 1366
;1366:		ent->client->ps.forceHandExtend = HANDEXTEND_WEAPONREADY;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1236
ADDP4
CNSTI4 6
ASGNI4
line 1367
;1367:	}
ADDRGP4 $439
JUMPV
LABELV $438
line 1369
;1368:	else
;1369:	{
line 1370
;1370:		ent->client->ps.usingATST = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1304
ADDP4
CNSTI4 1
ASGNI4
line 1371
;1371:	}
LABELV $439
line 1373
;1372:
;1373:	ent->client->ps.weaponTime = 1000;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 44
ADDP4
CNSTI4 1000
ASGNI4
line 1375
;1374:
;1375:	return 1;
CNSTI4 1
RETI4
LABELV $428
endproc G_PlayerBecomeATST 8 0
export fx_runner_think
proc fx_runner_think 16 12
line 1395
;1376:}
;1377:
;1378:/*QUAKED fx_runner (0 0 1) (-8 -8 -8) (8 8 8) STARTOFF ONESHOT
;1379:	STARTOFF - effect starts off, toggles on/off when used
;1380:	ONESHOT - effect fires only when used
;1381:
;1382:    "angles"   - 3-float vector, angle the effect should play (unless fxTarget is supplied)
;1383:	"fxFile"   - name of the effect file to play
;1384:	"fxTarget" - aim the effect toward this object, otherwise defaults to up
;1385:	"target"   - uses its target when the fx gets triggered
;1386:	"delay"    - how often to call the effect, don't over-do this ( default 400 )
;1387:			     note that it has to send an event each time it plays, so don't kill bandwidth or I will cry
;1388:	"random"   - random amount of time to add to delay, ( default 0, 200 = 0ms to 200ms )
;1389:*/
;1390:#define FX_RUNNER_RESERVED 0x800000
;1391:#define FX_ENT_RADIUS 8 //32
;1392:
;1393://----------------------------------------------------------
;1394:void fx_runner_think( gentity_t *ent )
;1395:{
line 1397
;1396:	// call the effect with the desired position and orientation
;1397:	G_AddEvent( ent, EV_PLAY_EFFECT_ID, ent->bolt_Head );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 1399
;1398:
;1399:	ent->nextthink = level.time + ent->delay + random() * ent->random;
ADDRLP4 4
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ADDRLP4 8
INDIRP4
CNSTI4 764
ADDP4
INDIRI4
ADDI4
CVIF4 4
ADDRLP4 4
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
INDIRF4
MULF4
ADDF4
CVFI4 4
ASGNI4
line 1401
;1400:
;1401:	if ( ent->target )
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $442
line 1402
;1402:	{
line 1404
;1403:		// let our target know that we have spawned an effect
;1404:		G_UseTargets( ent, ent );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
line 1405
;1405:	}
LABELV $442
line 1406
;1406:}
LABELV $440
endproc fx_runner_think 16 12
export fx_runner_use
proc fx_runner_use 4 8
line 1410
;1407:
;1408://----------------------------------------------------------
;1409:void fx_runner_use( gentity_t *self, gentity_t *other, gentity_t *activator )
;1410:{
line 1411
;1411:	if ( self->spawnflags & 2 ) // ONESHOT
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $445
line 1412
;1412:	{
line 1415
;1413:		// call the effect with the desired position and orientation, as a safety thing,
;1414:		//	make sure we aren't thinking at all.
;1415:		fx_runner_think( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 fx_runner_think
CALLV
pop
line 1416
;1416:		self->nextthink = -1;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
CNSTI4 -1
ASGNI4
line 1418
;1417:
;1418:		if ( self->target )
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $446
line 1419
;1419:		{
line 1421
;1420:			// let our target know that we have spawned an effect
;1421:			G_UseTargets( self, self );
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
ADDRGP4 G_UseTargets
CALLV
pop
line 1422
;1422:		}
line 1423
;1423:	}
ADDRGP4 $446
JUMPV
LABELV $445
line 1425
;1424:	else
;1425:	{
line 1427
;1426:		// ensure we are working with the right think function
;1427:		self->think = fx_runner_think;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 fx_runner_think
ASGNP4
line 1430
;1428:
;1429:		// toggle our state
;1430:		if ( self->nextthink == -1 )
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $449
line 1431
;1431:		{
line 1434
;1432:			// NOTE: we fire the effect immediately on use, the fx_runner_think func will set
;1433:			//	up the nextthink time.
;1434:			fx_runner_think( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 fx_runner_think
CALLV
pop
line 1435
;1435:		}
ADDRGP4 $450
JUMPV
LABELV $449
line 1437
;1436:		else
;1437:		{
line 1439
;1438:			// turn off for now
;1439:			self->nextthink = -1;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
CNSTI4 -1
ASGNI4
line 1440
;1440:		}
LABELV $450
line 1441
;1441:	}
LABELV $446
line 1442
;1442:}
LABELV $444
endproc fx_runner_use 4 8
export fx_runner_link
proc fx_runner_link 44 12
line 1446
;1443:
;1444://----------------------------------------------------------
;1445:void fx_runner_link( gentity_t *ent )
;1446:{
line 1449
;1447:	vec3_t	dir;
;1448:
;1449:	if ( ent->roffname && ent->roffname[0] )
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 428
ADDP4
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $452
ADDRLP4 12
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $452
line 1450
;1450:	{
line 1452
;1451:		// try to use the target to override the orientation
;1452:		gentity_t	*target = NULL;
ADDRLP4 16
CNSTP4 0
ASGNP4
line 1454
;1453:
;1454:		target = G_Find( target, FOFS(targetname), ent->roffname );
ADDRLP4 16
INDIRP4
ARGP4
CNSTI4 588
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 428
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 16
ADDRLP4 20
INDIRP4
ASGNP4
line 1456
;1455:
;1456:		if ( !target )
ADDRLP4 16
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $454
line 1457
;1457:		{
line 1459
;1458:			// Bah, no good, dump a warning, but continue on and use the UP vector
;1459:			Com_Printf( "fx_runner_link: target specified but not found: %s\n", ent->roffname );
ADDRGP4 $456
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 428
ADDP4
INDIRP4
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1460
;1460:			Com_Printf( "  -assuming UP orientation.\n" );
ADDRGP4 $457
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1461
;1461:		}
ADDRGP4 $455
JUMPV
LABELV $454
line 1463
;1462:		else
;1463:		{
line 1465
;1464:			// Our target is valid so let's override the default UP vector
;1465:			VectorSubtract( target->s.origin, ent->s.origin, dir );
ADDRLP4 28
CNSTI4 92
ASGNI4
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 16
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
INDIRF4
ADDRLP4 32
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 36
CNSTI4 96
ASGNI4
ADDRLP4 0+4
ADDRLP4 16
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRF4
ADDRLP4 32
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 40
CNSTI4 100
ASGNI4
ADDRLP4 0+8
ADDRLP4 16
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1466
;1466:			VectorNormalize( dir );
ADDRLP4 0
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 1467
;1467:			vectoangles( dir, ent->s.angles );
ADDRLP4 0
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1468
;1468:		}
LABELV $455
line 1469
;1469:	}
LABELV $452
line 1472
;1470:
;1471:	// don't really do anything with this right now other than do a check to warn the designers if the target is bogus
;1472:	if ( ent->target )
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $460
line 1473
;1473:	{
line 1474
;1474:		gentity_t	*target = NULL;
ADDRLP4 16
CNSTP4 0
ASGNP4
line 1476
;1475:
;1476:		target = G_Find( target, FOFS(targetname), ent->target );
ADDRLP4 16
INDIRP4
ARGP4
CNSTI4 588
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
ARGP4
ADDRLP4 20
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 16
ADDRLP4 20
INDIRP4
ASGNP4
line 1478
;1477:
;1478:		if ( !target )
ADDRLP4 16
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $462
line 1479
;1479:		{
line 1481
;1480:			// Target is bogus, but we can still continue
;1481:			Com_Printf( "fx_runner_link: target was specified but is not valid: %s\n", ent->target );
ADDRGP4 $464
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1482
;1482:		}
LABELV $462
line 1483
;1483:	}
LABELV $460
line 1485
;1484:
;1485:	G_SetAngles( ent, ent->s.angles );
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 16
INDIRP4
CNSTI4 116
ADDP4
ARGP4
ADDRGP4 G_SetAngles
CALLV
pop
line 1487
;1486:
;1487:	if ( ent->spawnflags & 1 || ent->spawnflags & 2 ) // STARTOFF || ONESHOT
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
ASGNI4
ADDRLP4 24
CNSTI4 0
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 1
BANDI4
ADDRLP4 24
INDIRI4
NEI4 $467
ADDRLP4 20
INDIRI4
CNSTI4 2
BANDI4
ADDRLP4 24
INDIRI4
EQI4 $465
LABELV $467
line 1488
;1488:	{
line 1490
;1489:		// We won't even consider thinking until we are used
;1490:		ent->nextthink = -1;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
CNSTI4 -1
ASGNI4
line 1491
;1491:	}
ADDRGP4 $466
JUMPV
LABELV $465
line 1493
;1492:	else
;1493:	{
line 1495
;1494:		// Let's get to work right now!
;1495:		ent->think = fx_runner_think;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 fx_runner_think
ASGNP4
line 1496
;1496:		ent->nextthink = level.time + 100; // wait a small bit, then start working
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 1497
;1497:	}
LABELV $466
line 1498
;1498:}
LABELV $451
endproc fx_runner_link 44 12
export SP_fx_runner
proc SP_fx_runner 36 12
line 1502
;1499:
;1500://----------------------------------------------------------
;1501:void SP_fx_runner( gentity_t *ent )
;1502:{
line 1506
;1503:	char		*fxFile;
;1504:
;1505:	// Get our defaults
;1506:	G_SpawnInt( "delay", "400", &ent->delay );
ADDRGP4 $470
ARGP4
ADDRGP4 $471
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 764
ADDP4
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 1507
;1507:	G_SpawnFloat( "random", "0", &ent->random );
ADDRGP4 $472
ARGP4
ADDRGP4 $112
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
line 1509
;1508:
;1509:	if (!ent->s.angles[0] && !ent->s.angles[1] && !ent->s.angles[2])
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
CNSTF4 0
ASGNF4
ADDRLP4 4
INDIRP4
CNSTI4 116
ADDP4
INDIRF4
ADDRLP4 8
INDIRF4
NEF4 $473
ADDRLP4 4
INDIRP4
CNSTI4 120
ADDP4
INDIRF4
ADDRLP4 8
INDIRF4
NEF4 $473
ADDRLP4 4
INDIRP4
CNSTI4 124
ADDP4
INDIRF4
ADDRLP4 8
INDIRF4
NEF4 $473
line 1510
;1510:	{
line 1512
;1511:		// didn't have angles, so give us the default of up
;1512:		VectorSet( ent->s.angles, -90, 0, 0 );
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
CNSTF4 3266576384
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
CNSTF4 0
ASGNF4
line 1513
;1513:	}
LABELV $473
line 1516
;1514:
;1515:	// make us useable if we can be targeted
;1516:	if ( ent->targetname )
ADDRFP4 0
INDIRP4
CNSTI4 588
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $475
line 1517
;1517:	{
line 1518
;1518:		ent->use = fx_runner_use;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 fx_runner_use
ASGNP4
line 1519
;1519:	}
LABELV $475
line 1521
;1520:
;1521:	G_SpawnString( "fxFile", "", &fxFile );
ADDRGP4 $477
ARGP4
ADDRGP4 $478
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SpawnString
CALLI4
pop
line 1523
;1522:
;1523:	G_SpawnString( "fxTarget", "", &ent->roffname );
ADDRGP4 $479
ARGP4
ADDRGP4 $478
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 428
ADDP4
ARGP4
ADDRGP4 G_SpawnString
CALLI4
pop
line 1525
;1524:
;1525:	if ( !fxFile || !fxFile[0] )
ADDRLP4 12
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $482
ADDRLP4 12
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $480
LABELV $482
line 1526
;1526:	{
line 1527
;1527:		Com_Printf( S_COLOR_RED"ERROR: fx_runner %s at %s has no fxFile specified\n", ent->targetname, vtos(ent->s.origin) );
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 16
ADDRGP4 vtos
CALLP4
ASGNP4
ADDRGP4 $483
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 588
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1528
;1528:		G_FreeEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 1529
;1529:		return;
ADDRGP4 $469
JUMPV
LABELV $480
line 1534
;1530:	}
;1531:
;1532:	// Try and associate an effect file, unfortunately we won't know if this worked or not 
;1533:	//	until the CGAME trys to register it...
;1534:	ent->bolt_Head = G_EffectIndex( fxFile );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 G_EffectIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
ADDRLP4 16
INDIRI4
ASGNI4
line 1538
;1535:	//It is dirty, yes. But no one likes adding things to the entity structure.
;1536:
;1537:	// Give us a bit of time to spawn in the other entities, since we may have to target one of 'em
;1538:	ent->think = fx_runner_link; 
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 fx_runner_link
ASGNP4
line 1539
;1539:	ent->nextthink = level.time + 300;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 300
ADDI4
ASGNI4
line 1542
;1540:
;1541:	// Save our position and link us up!
;1542:	G_SetOrigin( ent, ent->s.origin );
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 20
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1544
;1543:
;1544:	VectorSet( ent->r.maxs, FX_ENT_RADIUS, FX_ENT_RADIUS, FX_ENT_RADIUS );
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
CNSTF4 1090519040
ASGNF4
line 1545
;1545:	VectorScale( ent->r.maxs, -1, ent->r.mins );
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3212836864
ADDRLP4 24
INDIRP4
CNSTI4 328
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3212836864
ADDRLP4 28
INDIRP4
CNSTI4 332
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3212836864
ADDRLP4 32
INDIRP4
CNSTI4 336
ADDP4
INDIRF4
MULF4
ASGNF4
line 1547
;1546:
;1547:	trap_LinkEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1548
;1548:}
LABELV $469
endproc SP_fx_runner 36 12
import g_MaxHolocronCarry
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
LABELV $483
char 1 94
char 1 49
char 1 69
char 1 82
char 1 82
char 1 79
char 1 82
char 1 58
char 1 32
char 1 102
char 1 120
char 1 95
char 1 114
char 1 117
char 1 110
char 1 110
char 1 101
char 1 114
char 1 32
char 1 37
char 1 115
char 1 32
char 1 97
char 1 116
char 1 32
char 1 37
char 1 115
char 1 32
char 1 104
char 1 97
char 1 115
char 1 32
char 1 110
char 1 111
char 1 32
char 1 102
char 1 120
char 1 70
char 1 105
char 1 108
char 1 101
char 1 32
char 1 115
char 1 112
char 1 101
char 1 99
char 1 105
char 1 102
char 1 105
char 1 101
char 1 100
char 1 10
char 1 0
align 1
LABELV $479
char 1 102
char 1 120
char 1 84
char 1 97
char 1 114
char 1 103
char 1 101
char 1 116
char 1 0
align 1
LABELV $478
char 1 0
align 1
LABELV $477
char 1 102
char 1 120
char 1 70
char 1 105
char 1 108
char 1 101
char 1 0
align 1
LABELV $472
char 1 114
char 1 97
char 1 110
char 1 100
char 1 111
char 1 109
char 1 0
align 1
LABELV $471
char 1 52
char 1 48
char 1 48
char 1 0
align 1
LABELV $470
char 1 100
char 1 101
char 1 108
char 1 97
char 1 121
char 1 0
align 1
LABELV $464
char 1 102
char 1 120
char 1 95
char 1 114
char 1 117
char 1 110
char 1 110
char 1 101
char 1 114
char 1 95
char 1 108
char 1 105
char 1 110
char 1 107
char 1 58
char 1 32
char 1 116
char 1 97
char 1 114
char 1 103
char 1 101
char 1 116
char 1 32
char 1 119
char 1 97
char 1 115
char 1 32
char 1 115
char 1 112
char 1 101
char 1 99
char 1 105
char 1 102
char 1 105
char 1 101
char 1 100
char 1 32
char 1 98
char 1 117
char 1 116
char 1 32
char 1 105
char 1 115
char 1 32
char 1 110
char 1 111
char 1 116
char 1 32
char 1 118
char 1 97
char 1 108
char 1 105
char 1 100
char 1 58
char 1 32
char 1 37
char 1 115
char 1 10
char 1 0
align 1
LABELV $457
char 1 32
char 1 32
char 1 45
char 1 97
char 1 115
char 1 115
char 1 117
char 1 109
char 1 105
char 1 110
char 1 103
char 1 32
char 1 85
char 1 80
char 1 32
char 1 111
char 1 114
char 1 105
char 1 101
char 1 110
char 1 116
char 1 97
char 1 116
char 1 105
char 1 111
char 1 110
char 1 46
char 1 10
char 1 0
align 1
LABELV $456
char 1 102
char 1 120
char 1 95
char 1 114
char 1 117
char 1 110
char 1 110
char 1 101
char 1 114
char 1 95
char 1 108
char 1 105
char 1 110
char 1 107
char 1 58
char 1 32
char 1 116
char 1 97
char 1 114
char 1 103
char 1 101
char 1 116
char 1 32
char 1 115
char 1 112
char 1 101
char 1 99
char 1 105
char 1 102
char 1 105
char 1 101
char 1 100
char 1 32
char 1 98
char 1 117
char 1 116
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
char 1 58
char 1 32
char 1 37
char 1 115
char 1 10
char 1 0
align 1
LABELV $399
char 1 100
char 1 109
char 1 103
char 1 95
char 1 98
char 1 111
char 1 120
char 1 0
align 1
LABELV $364
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
LABELV $330
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
LABELV $317
char 1 47
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
char 1 100
char 1 95
char 1 98
char 1 105
char 1 103
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $316
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
char 1 111
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 115
char 1 47
char 1 117
char 1 115
char 1 101
char 1 115
char 1 104
char 1 105
char 1 101
char 1 108
char 1 100
char 1 115
char 1 116
char 1 97
char 1 116
char 1 105
char 1 111
char 1 110
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $311
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 105
char 1 110
char 1 116
char 1 101
char 1 114
char 1 102
char 1 97
char 1 99
char 1 101
char 1 47
char 1 115
char 1 104
char 1 105
char 1 101
char 1 108
char 1 100
char 1 99
char 1 111
char 1 110
char 1 95
char 1 101
char 1 109
char 1 112
char 1 116
char 1 121
char 1 46
char 1 109
char 1 112
char 1 51
char 1 0
align 1
LABELV $309
char 1 47
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
char 1 97
char 1 95
char 1 115
char 1 104
char 1 105
char 1 101
char 1 108
char 1 100
char 1 95
char 1 99
char 1 111
char 1 110
char 1 118
char 1 101
char 1 114
char 1 116
char 1 101
char 1 114
char 1 46
char 1 109
char 1 100
char 1 51
char 1 0
align 1
LABELV $301
char 1 83
char 1 80
char 1 95
char 1 109
char 1 105
char 1 115
char 1 99
char 1 95
char 1 115
char 1 104
char 1 105
char 1 101
char 1 108
char 1 100
char 1 95
char 1 102
char 1 108
char 1 111
char 1 111
char 1 114
char 1 95
char 1 117
char 1 110
char 1 105
char 1 116
char 1 58
char 1 32
char 1 109
char 1 105
char 1 115
char 1 99
char 1 95
char 1 115
char 1 104
char 1 105
char 1 101
char 1 108
char 1 100
char 1 95
char 1 102
char 1 108
char 1 111
char 1 111
char 1 114
char 1 95
char 1 117
char 1 110
char 1 105
char 1 116
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
LABELV $286
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 105
char 1 110
char 1 116
char 1 101
char 1 114
char 1 102
char 1 97
char 1 99
char 1 101
char 1 47
char 1 115
char 1 104
char 1 105
char 1 101
char 1 108
char 1 100
char 1 99
char 1 111
char 1 110
char 1 95
char 1 100
char 1 111
char 1 110
char 1 101
char 1 46
char 1 109
char 1 112
char 1 51
char 1 0
align 1
LABELV $269
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 105
char 1 110
char 1 116
char 1 101
char 1 114
char 1 102
char 1 97
char 1 99
char 1 101
char 1 47
char 1 115
char 1 104
char 1 105
char 1 101
char 1 108
char 1 100
char 1 99
char 1 111
char 1 110
char 1 95
char 1 114
char 1 117
char 1 110
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $257
char 1 99
char 1 104
char 1 97
char 1 114
char 1 103
char 1 101
char 1 114
char 1 97
char 1 116
char 1 101
char 1 0
align 1
LABELV $254
char 1 99
char 1 111
char 1 117
char 1 110
char 1 116
char 1 0
align 1
LABELV $205
char 1 83
char 1 80
char 1 95
char 1 109
char 1 105
char 1 115
char 1 99
char 1 95
char 1 104
char 1 111
char 1 108
char 1 111
char 1 99
char 1 114
char 1 111
char 1 110
char 1 58
char 1 32
char 1 109
char 1 105
char 1 115
char 1 99
char 1 95
char 1 104
char 1 111
char 1 108
char 1 111
char 1 99
char 1 114
char 1 111
char 1 110
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
LABELV $112
char 1 48
char 1 0
align 1
LABELV $111
char 1 114
char 1 111
char 1 108
char 1 108
char 1 0
align 1
LABELV $95
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
char 1 116
char 1 97
char 1 114
char 1 103
char 1 101
char 1 116
char 1 32
char 1 102
char 1 111
char 1 114
char 1 32
char 1 109
char 1 105
char 1 115
char 1 99
char 1 95
char 1 112
char 1 97
char 1 114
char 1 116
char 1 97
char 1 108
char 1 95
char 1 115
char 1 117
char 1 114
char 1 102
char 1 97
char 1 99
char 1 101
char 1 10
char 1 0
