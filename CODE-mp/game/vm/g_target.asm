export Use_Target_Give
code
proc Use_Target_Give 1088 12
file "../g_target.c"
line 10
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:#include "g_local.h"
;4:
;5://==========================================================
;6:
;7:/*QUAKED target_give (1 0 0) (-8 -8 -8) (8 8 8)
;8:Gives the activator all the items pointed to.
;9:*/
;10:void Use_Target_Give( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
line 14
;11:	gentity_t	*t;
;12:	trace_t		trace;
;13:
;14:	if ( !activator->client ) {
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $79
line 15
;15:		return;
ADDRGP4 $78
JUMPV
LABELV $79
line 18
;16:	}
;17:
;18:	if ( !ent->target ) {
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $81
line 19
;19:		return;
ADDRGP4 $78
JUMPV
LABELV $81
line 22
;20:	}
;21:
;22:	memset( &trace, 0, sizeof( trace ) );
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1080
ARGI4
ADDRGP4 memset
CALLP4
pop
line 23
;23:	t = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
ADDRGP4 $84
JUMPV
LABELV $83
line 24
;24:	while ( (t = G_Find (t, FOFS(targetname), ent->target)) != NULL ) {
line 25
;25:		if ( !t->item ) {
ADDRLP4 0
INDIRP4
CNSTI4 824
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $86
line 26
;26:			continue;
ADDRGP4 $84
JUMPV
LABELV $86
line 28
;27:		}
;28:		Touch_Item( t, activator, &trace );
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 Touch_Item
CALLV
pop
line 31
;29:
;30:		// make sure it isn't going to respawn or show any events
;31:		t->nextthink = 0;
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
CNSTI4 0
ASGNI4
line 32
;32:		trap_UnlinkEntity( t );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 33
;33:	}
LABELV $84
line 24
ADDRLP4 0
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
ADDRLP4 1084
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 1084
INDIRP4
ASGNP4
ADDRLP4 1084
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $83
line 34
;34:}
LABELV $78
endproc Use_Target_Give 1088 12
export SP_target_give
proc SP_target_give 0 0
line 36
;35:
;36:void SP_target_give( gentity_t *ent ) {
line 37
;37:	ent->use = Use_Target_Give;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 Use_Target_Give
ASGNP4
line 38
;38:}
LABELV $88
endproc SP_target_give 0 0
export Use_target_remove_powerups
proc Use_target_remove_powerups 0 12
line 47
;39:
;40:
;41://==========================================================
;42:
;43:/*QUAKED target_remove_powerups (1 0 0) (-8 -8 -8) (8 8 8)
;44:takes away all the activators powerups.
;45:Used to drop flight powerups into death puts.
;46:*/
;47:void Use_target_remove_powerups( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
line 48
;48:	if( !activator->client ) {
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $90
line 49
;49:		return;
ADDRGP4 $89
JUMPV
LABELV $90
line 52
;50:	}
;51:
;52:	if( activator->client->ps.powerups[PW_REDFLAG] ) {
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 360
ADDP4
INDIRI4
CNSTI4 0
EQI4 $92
line 53
;53:		Team_ReturnFlag( TEAM_RED );
CNSTI4 1
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 54
;54:	} else if( activator->client->ps.powerups[PW_BLUEFLAG] ) {
ADDRGP4 $93
JUMPV
LABELV $92
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 364
ADDP4
INDIRI4
CNSTI4 0
EQI4 $94
line 55
;55:		Team_ReturnFlag( TEAM_BLUE );
CNSTI4 2
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 56
;56:	} else if( activator->client->ps.powerups[PW_NEUTRALFLAG] ) {
ADDRGP4 $95
JUMPV
LABELV $94
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 368
ADDP4
INDIRI4
CNSTI4 0
EQI4 $96
line 57
;57:		Team_ReturnFlag( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 58
;58:	}
LABELV $96
LABELV $95
LABELV $93
line 60
;59:
;60:	memset( activator->client->ps.powerups, 0, sizeof( activator->client->ps.powerups ) );
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 344
ADDP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 64
ARGI4
ADDRGP4 memset
CALLP4
pop
line 61
;61:}
LABELV $89
endproc Use_target_remove_powerups 0 12
export SP_target_remove_powerups
proc SP_target_remove_powerups 0 0
line 63
;62:
;63:void SP_target_remove_powerups( gentity_t *ent ) {
line 64
;64:	ent->use = Use_target_remove_powerups;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 Use_target_remove_powerups
ASGNP4
line 65
;65:}
LABELV $98
endproc SP_target_remove_powerups 0 0
export Think_Target_Delay
proc Think_Target_Delay 4 8
line 78
;66:
;67:
;68://==========================================================
;69:
;70:/*QUAKED target_delay (1 0 0) (-8 -8 -8) (8 8 8) NO_RETRIGGER
;71:
;72:NO_RETRIGGER - Keeps the delay from resetting the time if it is
;73:activated again while it is counting down to an event.
;74:
;75:"wait" seconds to pause before firing targets.
;76:"random" delay variance, total delay = delay +/- random seconds
;77:*/
;78:void Think_Target_Delay( gentity_t *ent ) {
line 79
;79:	G_UseTargets( ent, ent->activator );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
line 80
;80:}
LABELV $99
endproc Think_Target_Delay 4 8
export Use_Target_Delay
proc Use_Target_Delay 12 0
line 82
;81:
;82:void Use_Target_Delay( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
line 83
;83:	if (ent->nextthink > level.time && (ent->spawnflags & 1))
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
LEI4 $101
ADDRLP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $101
line 84
;84:	{ //Leave me alone, I am thinking.
line 85
;85:		return;
ADDRGP4 $100
JUMPV
LABELV $101
line 87
;86:	}
;87:	ent->nextthink = level.time + ( ent->wait + ent->random * crandom() ) * 1000;
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
CVIF4 4
CNSTF4 1148846080
ADDRLP4 8
INDIRP4
CNSTI4 756
ADDP4
INDIRF4
ADDRLP4 8
INDIRP4
CNSTI4 760
ADDP4
INDIRF4
CNSTF4 1073741824
ADDRLP4 4
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
ADDF4
MULF4
ADDF4
CVFI4 4
ASGNI4
line 88
;88:	ent->think = Think_Target_Delay;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 Think_Target_Delay
ASGNP4
line 89
;89:	ent->activator = activator;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ADDRFP4 8
INDIRP4
ASGNP4
line 90
;90:}
LABELV $100
endproc Use_Target_Delay 12 0
export SP_target_delay
proc SP_target_delay 4 12
line 92
;91:
;92:void SP_target_delay( gentity_t *ent ) {
line 94
;93:	// check delay for backwards compatability
;94:	if ( !G_SpawnFloat( "delay", "0", &ent->wait ) ) {
ADDRGP4 $108
ARGP4
ADDRGP4 $109
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 756
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 G_SpawnFloat
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $106
line 95
;95:		G_SpawnFloat( "wait", "1", &ent->wait );
ADDRGP4 $110
ARGP4
ADDRGP4 $111
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 756
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
line 96
;96:	}
LABELV $106
line 98
;97:
;98:	if ( !ent->wait ) {
ADDRFP4 0
INDIRP4
CNSTI4 756
ADDP4
INDIRF4
CNSTF4 0
NEF4 $112
line 99
;99:		ent->wait = 1;
ADDRFP4 0
INDIRP4
CNSTI4 756
ADDP4
CNSTF4 1065353216
ASGNF4
line 100
;100:	}
LABELV $112
line 101
;101:	ent->use = Use_Target_Delay;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 Use_Target_Delay
ASGNP4
line 102
;102:}
LABELV $105
endproc SP_target_delay 4 12
export Use_Target_Score
proc Use_Target_Score 4 12
line 112
;103:
;104:
;105://==========================================================
;106:
;107:/*QUAKED target_score (1 0 0) (-8 -8 -8) (8 8 8)
;108:"count" number of points to add, default 1
;109:
;110:The activator is given this many points.
;111:*/
;112:void Use_Target_Score (gentity_t *ent, gentity_t *other, gentity_t *activator) {
line 113
;113:	AddScore( activator, ent->r.currentOrigin, ent->count );
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 114
;114:}
LABELV $114
endproc Use_Target_Score 4 12
export SP_target_score
proc SP_target_score 0 0
line 116
;115:
;116:void SP_target_score( gentity_t *ent ) {
line 117
;117:	if ( !ent->count ) {
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
NEI4 $116
line 118
;118:		ent->count = 1;
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
CNSTI4 1
ASGNI4
line 119
;119:	}
LABELV $116
line 120
;120:	ent->use = Use_Target_Score;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 Use_Target_Score
ASGNP4
line 121
;121:}
LABELV $115
endproc SP_target_score 0 0
export Use_Target_Print
proc Use_Target_Print 4 8
line 130
;122:
;123:
;124://==========================================================
;125:
;126:/*QUAKED target_print (1 0 0) (-8 -8 -8) (8 8 8) redteam blueteam private
;127:"message"	text to print
;128:If "private", only the activator gets the message.  If no checks, all clients get the message.
;129:*/
;130:void Use_Target_Print (gentity_t *ent, gentity_t *other, gentity_t *activator) {
line 131
;131:	if ( activator->client && ( ent->spawnflags & 4 ) ) {
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $119
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $119
line 132
;132:		trap_SendServerCommand( activator-g_entities, va("cp \"%s\"", ent->message ));
ADDRGP4 $121
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 572
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
ADDRFP4 8
INDIRP4
CVPU4 4
ADDRGP4 g_entities
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 828
DIVI4
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 133
;133:		return;
ADDRGP4 $118
JUMPV
LABELV $119
line 136
;134:	}
;135:
;136:	if ( ent->spawnflags & 3 ) {
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 3
BANDI4
CNSTI4 0
EQI4 $122
line 137
;137:		if ( ent->spawnflags & 1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $124
line 138
;138:			G_TeamCommand( TEAM_RED, va("cp \"%s\"", ent->message) );
ADDRGP4 $121
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 572
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 1
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_TeamCommand
CALLV
pop
line 139
;139:		}
LABELV $124
line 140
;140:		if ( ent->spawnflags & 2 ) {
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $118
line 141
;141:			G_TeamCommand( TEAM_BLUE, va("cp \"%s\"", ent->message) );
ADDRGP4 $121
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 572
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_TeamCommand
CALLV
pop
line 142
;142:		}
line 143
;143:		return;
ADDRGP4 $118
JUMPV
LABELV $122
line 146
;144:	}
;145:
;146:	trap_SendServerCommand( -1, va("cp \"%s\"", ent->message ));
ADDRGP4 $121
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 572
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 147
;147:}
LABELV $118
endproc Use_Target_Print 4 8
export SP_target_print
proc SP_target_print 0 0
line 149
;148:
;149:void SP_target_print( gentity_t *ent ) {
line 150
;150:	ent->use = Use_Target_Print;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 Use_Target_Print
ASGNP4
line 151
;151:}
LABELV $128
endproc SP_target_print 0 0
export Use_Target_Speaker
proc Use_Target_Speaker 4 12
line 169
;152:
;153:
;154://==========================================================
;155:
;156:
;157:/*QUAKED target_speaker (1 0 0) (-8 -8 -8) (8 8 8) looped-on looped-off global activator
;158:"noise"		wav file to play
;159:
;160:A global sound will play full volume throughout the level.
;161:Activator sounds will play on the player that activated the target.
;162:Global and activator sounds can't be combined with looping.
;163:Normal sounds play each time the target is used.
;164:Looped sounds will be toggled by use functions.
;165:Multiple identical looping sounds will just increase volume without any speed cost.
;166:"wait" : Seconds between auto triggerings, 0 = don't auto trigger
;167:"random"	wait variance, default is 0
;168:*/
;169:void Use_Target_Speaker (gentity_t *ent, gentity_t *other, gentity_t *activator) {
line 170
;170:	if (ent->spawnflags & 3) {	// looping sound toggles
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 3
BANDI4
CNSTI4 0
EQI4 $130
line 171
;171:		if (ent->s.loopSound)
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
INDIRI4
CNSTI4 0
EQI4 $132
line 172
;172:		{
line 173
;173:			ent->s.loopSound = 0;	// turn it off
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
CNSTI4 0
ASGNI4
line 174
;174:			ent->s.trickedentindex = 1;
ADDRFP4 0
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 1
ASGNI4
line 175
;175:		}
ADDRGP4 $131
JUMPV
LABELV $132
line 177
;176:		else
;177:		{
line 178
;178:			ent->s.loopSound = ent->noise_index;	// start it
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 200
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 752
ADDP4
INDIRI4
ASGNI4
line 179
;179:			ent->s.trickedentindex = 0;
ADDRFP4 0
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 0
ASGNI4
line 180
;180:		}
line 181
;181:	}else {	// normal sound
ADDRGP4 $131
JUMPV
LABELV $130
line 182
;182:		if ( ent->spawnflags & 8 ) {
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $134
line 183
;183:			G_AddEvent( activator, EV_GENERAL_SOUND, ent->noise_index );
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 66
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 752
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 184
;184:		} else if (ent->spawnflags & 4) {
ADDRGP4 $135
JUMPV
LABELV $134
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $136
line 185
;185:			G_AddEvent( ent, EV_GLOBAL_SOUND, ent->noise_index );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 67
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 752
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 186
;186:		} else {
ADDRGP4 $137
JUMPV
LABELV $136
line 187
;187:			G_AddEvent( ent, EV_GENERAL_SOUND, ent->noise_index );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 66
ARGI4
ADDRLP4 0
INDIRP4
CNSTI4 752
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 188
;188:		}
LABELV $137
LABELV $135
line 189
;189:	}
LABELV $131
line 190
;190:}
LABELV $129
endproc Use_Target_Speaker 4 12
export SP_target_speaker
proc SP_target_speaker 92 12
line 192
;191:
;192:void SP_target_speaker( gentity_t *ent ) {
line 196
;193:	char	buffer[MAX_QPATH];
;194:	char	*s;
;195:
;196:	G_SpawnFloat( "wait", "0", &ent->wait );
ADDRGP4 $110
ARGP4
ADDRGP4 $109
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 756
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
line 197
;197:	G_SpawnFloat( "random", "0", &ent->random );
ADDRGP4 $139
ARGP4
ADDRGP4 $109
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
line 199
;198:
;199:	if ( !G_SpawnString( "noise", "NOSOUND", &s ) ) {
ADDRGP4 $142
ARGP4
ADDRGP4 $143
ARGP4
ADDRLP4 64
ARGP4
ADDRLP4 68
ADDRGP4 G_SpawnString
CALLI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 0
NEI4 $140
line 200
;200:		G_Error( "target_speaker without a noise key at %s", vtos( ent->s.origin ) );
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 72
ADDRGP4 vtos
CALLP4
ASGNP4
ADDRGP4 $144
ARGP4
ADDRLP4 72
INDIRP4
ARGP4
ADDRGP4 G_Error
CALLV
pop
line 201
;201:	}
LABELV $140
line 205
;202:
;203:	// force all client reletive sounds to be "activator" speakers that
;204:	// play on the entity that activates it
;205:	if ( s[0] == '*' ) {
ADDRLP4 64
INDIRP4
INDIRI1
CVII4 1
CNSTI4 42
NEI4 $145
line 206
;206:		ent->spawnflags |= 8;
ADDRLP4 72
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
ASGNP4
ADDRLP4 72
INDIRP4
ADDRLP4 72
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 207
;207:	}
LABELV $145
line 209
;208:
;209:	Q_strncpyz( buffer, s, sizeof(buffer) );
ADDRLP4 0
ARGP4
ADDRLP4 64
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 211
;210:
;211:	ent->noise_index = G_SoundIndex(buffer);
ADDRLP4 0
ARGP4
ADDRLP4 72
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 752
ADDP4
ADDRLP4 72
INDIRI4
ASGNI4
line 214
;212:
;213:	// a repeating speaker can be done completely client side
;214:	ent->s.eType = ET_SPEAKER;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 9
ASGNI4
line 215
;215:	ent->s.eventParm = ent->noise_index;
ADDRLP4 76
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 76
INDIRP4
CNSTI4 256
ADDP4
ADDRLP4 76
INDIRP4
CNSTI4 752
ADDP4
INDIRI4
ASGNI4
line 216
;216:	ent->s.frame = ent->wait * 10;
ADDRLP4 80
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 80
INDIRP4
CNSTI4 224
ADDP4
CNSTF4 1092616192
ADDRLP4 80
INDIRP4
CNSTI4 756
ADDP4
INDIRF4
MULF4
CVFI4 4
ASGNI4
line 217
;217:	ent->s.clientNum = ent->random * 10;
ADDRLP4 84
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 84
INDIRP4
CNSTI4 220
ADDP4
CNSTF4 1092616192
ADDRLP4 84
INDIRP4
CNSTI4 760
ADDP4
INDIRF4
MULF4
CVFI4 4
ASGNI4
line 221
;218:
;219:
;220:	// check for prestarted looping sound
;221:	if ( ent->spawnflags & 1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $147
line 222
;222:		ent->s.loopSound = ent->noise_index;
ADDRLP4 88
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 88
INDIRP4
CNSTI4 200
ADDP4
ADDRLP4 88
INDIRP4
CNSTI4 752
ADDP4
INDIRI4
ASGNI4
line 223
;223:	}
LABELV $147
line 225
;224:
;225:	ent->use = Use_Target_Speaker;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 Use_Target_Speaker
ASGNP4
line 227
;226:
;227:	if (ent->spawnflags & 4) {
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $149
line 228
;228:		ent->r.svFlags |= SVF_BROADCAST;
ADDRLP4 88
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 88
INDIRP4
ADDRLP4 88
INDIRP4
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 229
;229:	}
LABELV $149
line 231
;230:
;231:	VectorCopy( ent->s.origin, ent->s.pos.trBase );
ADDRLP4 88
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 88
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 88
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 235
;232:
;233:	// must link the entity so we get areas and clusters so
;234:	// the server can determine who to send updates to
;235:	trap_LinkEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 236
;236:}
LABELV $138
endproc SP_target_speaker 92 12
export target_laser_think
proc target_laser_think 1144 32
line 245
;237:
;238:
;239:
;240://==========================================================
;241:
;242:/*QUAKED target_laser (0 .5 .8) (-8 -8 -8) (8 8 8) START_ON
;243:When triggered, fires a laser.  You can either set a target or a direction.
;244:*/
;245:void target_laser_think (gentity_t *self) {
line 251
;246:	vec3_t	end;
;247:	trace_t	tr;
;248:	vec3_t	point;
;249:
;250:	// if pointed at another entity, set movedir to point at it
;251:	if ( self->enemy ) {
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $152
line 252
;252:		VectorMA (self->enemy->s.origin, 0.5, self->enemy->r.mins, point);
ADDRLP4 1104
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ASGNP4
ADDRLP4 1108
ADDRLP4 1104
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 1112
CNSTF4 1056964608
ASGNF4
ADDRLP4 0
ADDRLP4 1108
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRLP4 1112
INDIRF4
ADDRLP4 1108
INDIRP4
CNSTI4 316
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 1116
ADDRLP4 1104
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 0+4
ADDRLP4 1116
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRLP4 1112
INDIRF4
ADDRLP4 1116
INDIRP4
CNSTI4 320
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 1120
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
ASGNP4
ADDRLP4 0+8
ADDRLP4 1120
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
CNSTF4 1056964608
ADDRLP4 1120
INDIRP4
CNSTI4 324
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 253
;253:		VectorMA (point, 0.5, self->enemy->r.maxs, point);
ADDRLP4 1124
CNSTF4 1056964608
ASGNF4
ADDRLP4 1128
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ASGNP4
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 1124
INDIRF4
ADDRLP4 1128
INDIRP4
INDIRP4
CNSTI4 328
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 1124
INDIRF4
ADDRLP4 1128
INDIRP4
INDIRP4
CNSTI4 332
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1056964608
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 336
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 254
;254:		VectorSubtract (point, self->s.origin, self->movedir);
ADDRLP4 1132
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1132
INDIRP4
CNSTI4 612
ADDP4
ADDRLP4 0
INDIRF4
ADDRLP4 1132
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 1136
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1136
INDIRP4
CNSTI4 616
ADDP4
ADDRLP4 0+4
INDIRF4
ADDRLP4 1136
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 1140
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1140
INDIRP4
CNSTI4 620
ADDP4
ADDRLP4 0+8
INDIRF4
ADDRLP4 1140
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
SUBF4
ASGNF4
line 255
;255:		VectorNormalize (self->movedir);
ADDRFP4 0
INDIRP4
CNSTI4 612
ADDP4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 256
;256:	}
LABELV $152
line 259
;257:
;258:	// fire forward and see what we hit
;259:	VectorMA (self->s.origin, 2048, self->movedir, end);
ADDRLP4 1104
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1108
CNSTF4 1157627904
ASGNF4
ADDRLP4 12
ADDRLP4 1104
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRLP4 1108
INDIRF4
ADDRLP4 1104
INDIRP4
CNSTI4 612
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 1104
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRLP4 1108
INDIRF4
ADDRLP4 1104
INDIRP4
CNSTI4 616
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 1112
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12+8
ADDRLP4 1112
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
CNSTF4 1157627904
ADDRLP4 1112
INDIRP4
CNSTI4 620
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 261
;260:
;261:	trap_Trace( &tr, self->s.origin, NULL, NULL, end, self->s.number, CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE);
ADDRLP4 24
ARGP4
ADDRLP4 1116
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1116
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 1120
CNSTP4 0
ASGNP4
ADDRLP4 1120
INDIRP4
ARGP4
ADDRLP4 1120
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 1116
INDIRP4
INDIRI4
ARGI4
CNSTI4 769
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 263
;262:
;263:	if ( tr.entityNum ) {
ADDRLP4 24+52
INDIRI4
CNSTI4 0
EQI4 $164
line 265
;264:		// hurt it if we can
;265:		G_Damage ( &g_entities[tr.entityNum], self, self->activator, self->movedir, 
CNSTI4 828
ADDRLP4 24+52
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRLP4 1124
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1124
INDIRP4
ARGP4
ADDRLP4 1124
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
ARGP4
ADDRLP4 1124
INDIRP4
CNSTI4 612
ADDP4
ARGP4
ADDRLP4 24+12
ARGP4
ADDRLP4 1124
INDIRP4
CNSTI4 688
ADDP4
INDIRI4
ARGI4
CNSTI4 4
ARGI4
CNSTI4 36
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 267
;266:			tr.endpos, self->damage, DAMAGE_NO_KNOCKBACK, MOD_TARGET_LASER);
;267:	}
LABELV $164
line 269
;268:
;269:	VectorCopy (tr.endpos, self->s.origin2);
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 24+12
INDIRB
ASGNB 12
line 271
;270:
;271:	trap_LinkEntity( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 272
;272:	self->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 273
;273:}
LABELV $151
endproc target_laser_think 1144 32
export target_laser_on
proc target_laser_on 4 4
line 276
;274:
;275:void target_laser_on (gentity_t *self)
;276:{
line 277
;277:	if (!self->activator)
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $172
line 278
;278:		self->activator = self;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 732
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
LABELV $172
line 279
;279:	target_laser_think (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 target_laser_think
CALLV
pop
line 280
;280:}
LABELV $171
endproc target_laser_on 4 4
export target_laser_off
proc target_laser_off 0 4
line 283
;281:
;282:void target_laser_off (gentity_t *self)
;283:{
line 284
;284:	trap_UnlinkEntity( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 285
;285:	self->nextthink = 0;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
CNSTI4 0
ASGNI4
line 286
;286:}
LABELV $174
endproc target_laser_off 0 4
export target_laser_use
proc target_laser_use 0 4
line 289
;287:
;288:void target_laser_use (gentity_t *self, gentity_t *other, gentity_t *activator)
;289:{
line 290
;290:	self->activator = activator;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ADDRFP4 8
INDIRP4
ASGNP4
line 291
;291:	if ( self->nextthink > 0 )
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
INDIRI4
CNSTI4 0
LEI4 $176
line 292
;292:		target_laser_off (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 target_laser_off
CALLV
pop
ADDRGP4 $177
JUMPV
LABELV $176
line 294
;293:	else
;294:		target_laser_on (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 target_laser_on
CALLV
pop
LABELV $177
line 295
;295:}
LABELV $175
endproc target_laser_use 0 4
export target_laser_start
proc target_laser_start 16 16
line 298
;296:
;297:void target_laser_start (gentity_t *self)
;298:{
line 301
;299:	gentity_t *ent;
;300:
;301:	self->s.eType = ET_BEAM;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 7
ASGNI4
line 303
;302:
;303:	if (self->target) {
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $179
line 304
;304:		ent = G_Find (NULL, FOFS(targetname), self->target);
CNSTP4 0
ARGP4
CNSTI4 588
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 305
;305:		if (!ent) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $181
line 306
;306:			G_Printf ("%s at %s: %s is a bad target\n", self->classname, vtos(self->s.origin), self->target);
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 8
ADDRGP4 vtos
CALLP4
ASGNP4
ADDRGP4 $183
ARGP4
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 416
ADDP4
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 307
;307:		}
LABELV $181
line 308
;308:		self->enemy = ent;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 309
;309:	} else {
ADDRGP4 $180
JUMPV
LABELV $179
line 310
;310:		G_SetMovedir (self->s.angles, self->movedir);
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
line 311
;311:	}
LABELV $180
line 313
;312:
;313:	self->use = target_laser_use;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 target_laser_use
ASGNP4
line 314
;314:	self->think = target_laser_think;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 target_laser_think
ASGNP4
line 316
;315:
;316:	if ( !self->damage ) {
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
INDIRI4
CNSTI4 0
NEI4 $184
line 317
;317:		self->damage = 1;
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
CNSTI4 1
ASGNI4
line 318
;318:	}
LABELV $184
line 320
;319:
;320:	if (self->spawnflags & 1)
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $186
line 321
;321:		target_laser_on (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 target_laser_on
CALLV
pop
ADDRGP4 $187
JUMPV
LABELV $186
line 323
;322:	else
;323:		target_laser_off (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 target_laser_off
CALLV
pop
LABELV $187
line 324
;324:}
LABELV $178
endproc target_laser_start 16 16
export SP_target_laser
proc SP_target_laser 0 0
line 327
;325:
;326:void SP_target_laser (gentity_t *self)
;327:{
line 329
;328:	// let everything else get spawned before we start firing
;329:	self->think = target_laser_start;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 target_laser_start
ASGNP4
line 330
;330:	self->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 331
;331:}
LABELV $188
endproc SP_target_laser 0 0
export target_teleporter_use
proc target_teleporter_use 12 12
line 336
;332:
;333:
;334://==========================================================
;335:
;336:void target_teleporter_use( gentity_t *self, gentity_t *other, gentity_t *activator ) {
line 339
;337:	gentity_t	*dest;
;338:
;339:	if (!activator->client)
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $191
line 340
;340:		return;
ADDRGP4 $190
JUMPV
LABELV $191
line 341
;341:	dest = 	G_PickTarget( self->target );
ADDRFP4 0
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
ADDRLP4 4
INDIRP4
ASGNP4
line 342
;342:	if (!dest) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $193
line 343
;343:		G_Printf ("Couldn't find teleporter destination\n");
ADDRGP4 $195
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 344
;344:		return;
ADDRGP4 $190
JUMPV
LABELV $193
line 347
;345:	}
;346:
;347:	TeleportPlayer( activator, dest->s.origin, dest->s.angles );
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ARGP4
ADDRGP4 TeleportPlayer
CALLV
pop
line 348
;348:}
LABELV $190
endproc target_teleporter_use 12 12
export SP_target_teleporter
proc SP_target_teleporter 4 12
line 353
;349:
;350:/*QUAKED target_teleporter (1 0 0) (-8 -8 -8) (8 8 8)
;351:The activator will be teleported away.
;352:*/
;353:void SP_target_teleporter( gentity_t *self ) {
line 354
;354:	if (!self->targetname)
ADDRFP4 0
INDIRP4
CNSTI4 588
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $197
line 355
;355:		G_Printf("untargeted %s at %s\n", self->classname, vtos(self->s.origin));
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 0
ADDRGP4 vtos
CALLP4
ASGNP4
ADDRGP4 $199
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 416
ADDP4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
LABELV $197
line 357
;356:
;357:	self->use = target_teleporter_use;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 target_teleporter_use
ASGNP4
line 358
;358:}
LABELV $196
endproc SP_target_teleporter 4 12
export target_relay_use
proc target_relay_use 36 12
line 368
;359:
;360://==========================================================
;361:
;362:
;363:/*QUAKED target_relay (.5 .5 .5) (-8 -8 -8) (8 8 8) RED_ONLY BLUE_ONLY RANDOM
;364:This doesn't perform any actions except fire its targets.
;365:The activator can be forced to be from a certain team.
;366:if RANDOM is checked, only one of the targets will be fired, not all of them
;367:*/
;368:void target_relay_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
line 369
;369:	if ( ( self->spawnflags & 1 ) && activator->client 
ADDRLP4 0
CNSTI4 1
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
ADDRLP4 0
INDIRI4
BANDI4
CNSTI4 0
EQI4 $201
ADDRLP4 4
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $201
ADDRLP4 4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ADDRLP4 0
INDIRI4
EQI4 $201
line 370
;370:		&& activator->client->sess.sessionTeam != TEAM_RED ) {
line 371
;371:		return;
ADDRGP4 $200
JUMPV
LABELV $201
line 373
;372:	}
;373:	if ( ( self->spawnflags & 2 ) && activator->client 
ADDRLP4 8
CNSTI4 2
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
ADDRLP4 8
INDIRI4
BANDI4
CNSTI4 0
EQI4 $203
ADDRLP4 12
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $203
ADDRLP4 12
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ADDRLP4 8
INDIRI4
EQI4 $203
line 374
;374:		&& activator->client->sess.sessionTeam != TEAM_BLUE ) {
line 375
;375:		return;
ADDRGP4 $200
JUMPV
LABELV $203
line 377
;376:	}
;377:	if ( self->spawnflags & 4 ) {
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $205
line 380
;378:		gentity_t	*ent;
;379:
;380:		ent = G_PickTarget( self->target );
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
ADDRLP4 16
ADDRLP4 20
INDIRP4
ASGNP4
line 381
;381:		if ( ent && ent->use ) {
ADDRLP4 24
ADDRLP4 16
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
EQU4 $200
ADDRLP4 24
INDIRP4
CNSTI4 652
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 28
INDIRU4
EQU4 $200
line 382
;382:			ent->use( ent, self, activator );
ADDRLP4 32
ADDRLP4 16
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 32
INDIRP4
CNSTI4 652
ADDP4
INDIRP4
CALLV
pop
line 383
;383:		}
line 384
;384:		return;
ADDRGP4 $200
JUMPV
LABELV $205
line 386
;385:	}
;386:	G_UseTargets (self, activator);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
line 387
;387:}
LABELV $200
endproc target_relay_use 36 12
export SP_target_relay
proc SP_target_relay 0 0
line 389
;388:
;389:void SP_target_relay (gentity_t *self) {
line 390
;390:	self->use = target_relay_use;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 target_relay_use
ASGNP4
line 391
;391:}
LABELV $209
endproc SP_target_relay 0 0
export target_kill_use
proc target_kill_use 8 32
line 399
;392:
;393:
;394://==========================================================
;395:
;396:/*QUAKED target_kill (.5 .5 .5) (-8 -8 -8) (8 8 8)
;397:Kills the activator.
;398:*/
;399:void target_kill_use( gentity_t *self, gentity_t *other, gentity_t *activator ) {
line 400
;400:	G_Damage ( activator, NULL, NULL, NULL, NULL, 100000, DAMAGE_NO_PROTECTION, MOD_TELEFRAG);
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
CNSTP4 0
ASGNP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4
CNSTP4 0
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
CNSTI4 100000
ARGI4
CNSTI4 8
ARGI4
CNSTI4 33
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 401
;401:}
LABELV $210
endproc target_kill_use 8 32
export SP_target_kill
proc SP_target_kill 0 0
line 403
;402:
;403:void SP_target_kill( gentity_t *self ) {
line 404
;404:	self->use = target_kill_use;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 target_kill_use
ASGNP4
line 405
;405:}
LABELV $211
endproc SP_target_kill 0 0
export SP_target_position
proc SP_target_position 4 8
line 410
;406:
;407:/*QUAKED target_position (0 0.5 0) (-4 -4 -4) (4 4 4)
;408:Used as a positional target for in-game calculation, like jumppad targets.
;409:*/
;410:void SP_target_position( gentity_t *self ){
line 411
;411:	G_SetOrigin( self, self->s.origin );
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
line 412
;412:}
LABELV $212
endproc SP_target_position 4 8
proc target_location_linkup 16 8
line 415
;413:
;414:static void target_location_linkup(gentity_t *ent)
;415:{
line 419
;416:	int i;
;417:	int n;
;418:
;419:	if (level.locationLinked) 
ADDRGP4 level+9048
INDIRI4
CNSTI4 0
EQI4 $214
line 420
;420:		return;
ADDRGP4 $213
JUMPV
LABELV $214
line 422
;421:
;422:	level.locationLinked = qtrue;
ADDRGP4 level+9048
CNSTI4 1
ASGNI4
line 424
;423:
;424:	level.locationHead = NULL;
ADDRGP4 level+9052
CNSTP4 0
ASGNP4
line 426
;425:
;426:	trap_SetConfigstring( CS_LOCATIONS, "unknown" );
CNSTI4 640
ARGI4
ADDRGP4 $219
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 428
;427:
;428:	for (i = 0, ent = g_entities, n = 1;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRFP4 0
ADDRGP4 g_entities
ASGNP4
ADDRLP4 4
CNSTI4 1
ASGNI4
ADDRGP4 $223
JUMPV
LABELV $220
line 430
;429:			i < level.num_entities;
;430:			i++, ent++) {
line 431
;431:		if (ent->classname && !Q_stricmp(ent->classname, "target_location")) {
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 416
ADDP4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $225
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 $227
ARGP4
ADDRLP4 12
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $225
line 433
;432:			// lets overload some variables!
;433:			ent->health = n; // use for location marking
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 434
;434:			trap_SetConfigstring( CS_LOCATIONS + n, ent->message );
ADDRLP4 4
INDIRI4
CNSTI4 640
ADDI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 572
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 435
;435:			n++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 436
;436:			ent->nextTrain = level.locationHead;
ADDRFP4 0
INDIRP4
CNSTI4 540
ADDP4
ADDRGP4 level+9052
INDIRP4
ASGNP4
line 437
;437:			level.locationHead = ent;
ADDRGP4 level+9052
ADDRFP4 0
INDIRP4
ASGNP4
line 438
;438:		}
LABELV $225
line 439
;439:	}
LABELV $221
line 430
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 828
ADDP4
ASGNP4
LABELV $223
line 429
ADDRLP4 0
INDIRI4
ADDRGP4 level+12
INDIRI4
LTI4 $220
line 442
;440:
;441:	// All linked together now
;442:}
LABELV $213
endproc target_location_linkup 16 8
export SP_target_location
proc SP_target_location 4 8
line 452
;443:
;444:/*QUAKED target_location (0 0.5 0) (-8 -8 -8) (8 8 8)
;445:Set "message" to the name of this location.
;446:Set "count" to 0-7 for color.
;447:0:white 1:red 2:green 3:yellow 4:blue 5:cyan 6:magenta 7:white
;448:
;449:Closest target_location in sight used for the location, if none
;450:in site, closest in distance
;451:*/
;452:void SP_target_location( gentity_t *self ){
line 453
;453:	self->think = target_location_linkup;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 target_location_linkup
ASGNP4
line 454
;454:	self->nextthink = level.time + 200;  // Let them all spawn first
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 200
ADDI4
ASGNI4
line 456
;455:
;456:	G_SetOrigin( self, self->s.origin );
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
line 457
;457:}
LABELV $230
endproc SP_target_location 4 8
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
LABELV $227
char 1 116
char 1 97
char 1 114
char 1 103
char 1 101
char 1 116
char 1 95
char 1 108
char 1 111
char 1 99
char 1 97
char 1 116
char 1 105
char 1 111
char 1 110
char 1 0
align 1
LABELV $219
char 1 117
char 1 110
char 1 107
char 1 110
char 1 111
char 1 119
char 1 110
char 1 0
align 1
LABELV $199
char 1 117
char 1 110
char 1 116
char 1 97
char 1 114
char 1 103
char 1 101
char 1 116
char 1 101
char 1 100
char 1 32
char 1 37
char 1 115
char 1 32
char 1 97
char 1 116
char 1 32
char 1 37
char 1 115
char 1 10
char 1 0
align 1
LABELV $195
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
char 1 101
char 1 108
char 1 101
char 1 112
char 1 111
char 1 114
char 1 116
char 1 101
char 1 114
char 1 32
char 1 100
char 1 101
char 1 115
char 1 116
char 1 105
char 1 110
char 1 97
char 1 116
char 1 105
char 1 111
char 1 110
char 1 10
char 1 0
align 1
LABELV $183
char 1 37
char 1 115
char 1 32
char 1 97
char 1 116
char 1 32
char 1 37
char 1 115
char 1 58
char 1 32
char 1 37
char 1 115
char 1 32
char 1 105
char 1 115
char 1 32
char 1 97
char 1 32
char 1 98
char 1 97
char 1 100
char 1 32
char 1 116
char 1 97
char 1 114
char 1 103
char 1 101
char 1 116
char 1 10
char 1 0
align 1
LABELV $144
char 1 116
char 1 97
char 1 114
char 1 103
char 1 101
char 1 116
char 1 95
char 1 115
char 1 112
char 1 101
char 1 97
char 1 107
char 1 101
char 1 114
char 1 32
char 1 119
char 1 105
char 1 116
char 1 104
char 1 111
char 1 117
char 1 116
char 1 32
char 1 97
char 1 32
char 1 110
char 1 111
char 1 105
char 1 115
char 1 101
char 1 32
char 1 107
char 1 101
char 1 121
char 1 32
char 1 97
char 1 116
char 1 32
char 1 37
char 1 115
char 1 0
align 1
LABELV $143
char 1 78
char 1 79
char 1 83
char 1 79
char 1 85
char 1 78
char 1 68
char 1 0
align 1
LABELV $142
char 1 110
char 1 111
char 1 105
char 1 115
char 1 101
char 1 0
align 1
LABELV $139
char 1 114
char 1 97
char 1 110
char 1 100
char 1 111
char 1 109
char 1 0
align 1
LABELV $121
char 1 99
char 1 112
char 1 32
char 1 34
char 1 37
char 1 115
char 1 34
char 1 0
align 1
LABELV $111
char 1 49
char 1 0
align 1
LABELV $110
char 1 119
char 1 97
char 1 105
char 1 116
char 1 0
align 1
LABELV $109
char 1 48
char 1 0
align 1
LABELV $108
char 1 100
char 1 101
char 1 108
char 1 97
char 1 121
char 1 0
