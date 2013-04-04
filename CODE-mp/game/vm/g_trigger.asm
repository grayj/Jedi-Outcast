export InitTrigger
code
proc InitTrigger 12 8
file "../g_trigger.c"
line 7
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:#include "g_local.h"
;4:
;5:int gTrigFallSound;
;6:
;7:void InitTrigger( gentity_t *self ) {
line 8
;8:	if (!VectorCompare (self->s.angles, vec3_origin))
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
ARGP4
ADDRGP4 vec3_origin
ARGP4
ADDRLP4 0
ADDRGP4 VectorCompare
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $79
line 9
;9:		G_SetMovedir (self->s.angles, self->movedir);
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
LABELV $79
line 11
;10:
;11:	trap_SetBrushModel( self, self->model );
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
CNSTI4 476
ADDP4
INDIRP4
ARGP4
ADDRGP4 trap_SetBrushModel
CALLV
pop
line 12
;12:	self->r.contents = CONTENTS_TRIGGER;		// replaces the -1 from trap_SetBrushModel
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1024
ASGNI4
line 13
;13:	self->r.svFlags = SVF_NOCLIENT;
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
CNSTI4 1
ASGNI4
line 14
;14:}
LABELV $78
endproc InitTrigger 12 8
export multi_wait
proc multi_wait 0 0
line 18
;15:
;16:
;17:// the wait time has passed, so set back up for another activation
;18:void multi_wait( gentity_t *ent ) {
line 19
;19:	ent->nextthink = 0;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
CNSTI4 0
ASGNI4
line 20
;20:}
LABELV $81
endproc multi_wait 0 0
export multi_trigger
proc multi_trigger 60 12
line 26
;21:
;22:
;23:// the trigger was just activated
;24:// ent->activator should be set to the activator so it can be held through a delay
;25:// so wait for the delay time before firing
;26:void multi_trigger( gentity_t *ent, gentity_t *activator ) {
line 27
;27:	gentity_t *rofftarget = NULL, *testent = NULL;
ADDRLP4 8
CNSTP4 0
ASGNP4
ADDRLP4 0
CNSTP4 0
ASGNP4
line 29
;28:	gentity_t *te;
;29:	int i = MAX_CLIENTS;
ADDRLP4 4
CNSTI4 32
ASGNI4
line 31
;30:
;31:	if (ent->teamnodmg &&
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
ASGNI4
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRLP4 16
INDIRI4
ADDRLP4 20
INDIRI4
EQI4 $83
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
EQU4 $83
ADDRLP4 32
ADDRLP4 24
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CVPU4 4
ADDRLP4 28
INDIRU4
EQU4 $83
ADDRLP4 16
INDIRI4
ADDRLP4 32
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
NEI4 $83
ADDRGP4 g_ff_objectives+12
INDIRI4
ADDRLP4 20
INDIRI4
NEI4 $83
line 35
;32:		activator && activator->client &&
;33:		ent->teamnodmg == activator->client->sess.sessionTeam &&
;34:		!g_ff_objectives.integer)
;35:	{
line 36
;36:		return;
ADDRGP4 $82
JUMPV
LABELV $83
line 39
;37:	}
;38:
;39:	if (ent->spawnflags & 1)
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $86
line 40
;40:	{
line 41
;41:		if (!activator->client)
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $88
line 42
;42:		{
line 43
;43:			return;
ADDRGP4 $82
JUMPV
LABELV $88
line 46
;44:		}
;45:
;46:		if (!(activator->client->pers.cmd.buttons & BUTTON_USE))
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1388
ADDP4
INDIRI4
CNSTI4 32
BANDI4
CNSTI4 0
NEI4 $90
line 47
;47:		{
line 48
;48:			return;
ADDRGP4 $82
JUMPV
LABELV $90
line 50
;49:		}
;50:	}
LABELV $86
line 52
;51:
;52:	ent->activator = activator;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ADDRFP4 4
INDIRP4
ASGNP4
line 53
;53:	if ( ent->nextthink ) {
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
INDIRI4
CNSTI4 0
EQI4 $92
line 54
;54:		return;		// can't retrigger until the wait is over
ADDRGP4 $82
JUMPV
LABELV $92
line 57
;55:	}
;56:
;57:	if ( activator->client ) {
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $94
line 58
;58:		if ( ( ent->spawnflags & 2 ) &&
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $96
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 1
EQI4 $96
line 59
;59:			activator->client->sess.sessionTeam != TEAM_RED ) {
line 60
;60:			return;
ADDRGP4 $82
JUMPV
LABELV $96
line 62
;61:		}
;62:		if ( ( ent->spawnflags & 4 ) &&
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $98
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 2
EQI4 $98
line 63
;63:			activator->client->sess.sessionTeam != TEAM_BLUE ) {
line 64
;64:			return;
ADDRGP4 $82
JUMPV
LABELV $98
line 66
;65:		}
;66:	}
LABELV $94
line 68
;67:
;68:	G_UseTargets (ent, ent->activator);
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 36
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
line 70
;69:
;70:	if (ent->roffname && ent->roffid != -1)
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 428
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $100
ADDRLP4 40
INDIRP4
CNSTI4 464
ADDP4
INDIRI4
CNSTI4 -1
EQI4 $100
line 71
;71:	{
line 72
;72:		if (ent->rofftarget)
ADDRFP4 0
INDIRP4
CNSTI4 432
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $102
line 73
;73:		{
ADDRGP4 $105
JUMPV
LABELV $104
line 75
;74:			while (i < MAX_GENTITIES)
;75:			{
line 76
;76:				testent = &g_entities[i];
ADDRLP4 0
CNSTI4 828
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 78
;77:
;78:				if (testent && testent->targetname && strcmp(testent->targetname, ent->rofftarget) == 0)
ADDRLP4 48
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 48
INDIRU4
EQU4 $107
ADDRLP4 52
ADDRLP4 0
INDIRP4
CNSTI4 588
ADDP4
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CVPU4 4
ADDRLP4 48
INDIRU4
EQU4 $107
ADDRLP4 52
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 432
ADDP4
INDIRP4
ARGP4
ADDRLP4 56
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 56
INDIRI4
CNSTI4 0
NEI4 $107
line 79
;79:				{
line 80
;80:					rofftarget = testent;
ADDRLP4 8
ADDRLP4 0
INDIRP4
ASGNP4
line 81
;81:					break;
ADDRGP4 $103
JUMPV
LABELV $107
line 83
;82:				}
;83:				i++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 84
;84:			}
LABELV $105
line 74
ADDRLP4 4
INDIRI4
CNSTI4 1024
LTI4 $104
line 85
;85:		}
ADDRGP4 $103
JUMPV
LABELV $102
line 87
;86:		else
;87:		{
line 88
;88:			rofftarget = activator;
ADDRLP4 8
ADDRFP4 4
INDIRP4
ASGNP4
line 89
;89:		}
LABELV $103
line 91
;90:
;91:		if (rofftarget)
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $109
line 92
;92:		{
line 93
;93:			trap_ROFF_Play(rofftarget->s.number, ent->roffid, qfalse);
ADDRLP4 8
INDIRP4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 464
ADDP4
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 trap_ROFF_Play
CALLI4
pop
line 100
;94:
;95:			//Play it at the same time on the client, so that we can catch client-side notetrack events and not have to send
;96:			//them over from the server (this wouldn't work for things like effects due to lack of ability to precache them
;97:			//on the server)
;98:
;99:			//remember the entity's original position in case of a server-side "loop" notetrack
;100:			VectorCopy(rofftarget->s.pos.trBase, rofftarget->s.origin2);
ADDRLP4 8
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 8
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 101
;101:			VectorCopy(rofftarget->s.apos.trBase, rofftarget->s.angles2);
ADDRLP4 8
INDIRP4
CNSTI4 128
ADDP4
ADDRLP4 8
INDIRP4
CNSTI4 60
ADDP4
INDIRB
ASGNB 12
line 103
;102:
;103:			te = G_TempEntity( rofftarget->s.pos.trBase, EV_PLAY_ROFF );
ADDRLP4 8
INDIRP4
CNSTI4 24
ADDP4
ARGP4
CNSTI4 70
ARGI4
ADDRLP4 52
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 12
ADDRLP4 52
INDIRP4
ASGNP4
line 104
;104:			te->s.eventParm = ent->roffid;
ADDRLP4 12
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 464
ADDP4
INDIRI4
ASGNI4
line 105
;105:			te->s.weapon = rofftarget->s.number;
ADDRLP4 12
INDIRP4
CNSTI4 276
ADDP4
ADDRLP4 8
INDIRP4
INDIRI4
ASGNI4
line 106
;106:			te->s.trickedentindex = 0;
ADDRLP4 12
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 0
ASGNI4
line 110
;107:
;108:			//But.. this may not produce desired results for clients who connect while a ROFF is playing.
;109:
;110:			rofftarget->roffid = ent->roffid; //let this entity know the most recent ROFF played on him
ADDRLP4 56
CNSTI4 464
ASGNI4
ADDRLP4 8
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
INDIRI4
ASGNI4
line 111
;111:		}
LABELV $109
line 112
;112:	}
LABELV $100
line 114
;113:
;114:	if ( ent->wait > 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 756
ADDP4
INDIRF4
CNSTF4 0
LEF4 $111
line 115
;115:		ent->think = multi_wait;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 multi_wait
ASGNP4
line 116
;116:		ent->nextthink = level.time + ( ent->wait + ent->random * crandom() ) * 1000;
ADDRLP4 44
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CVIF4 4
CNSTF4 1148846080
ADDRLP4 48
INDIRP4
CNSTI4 756
ADDP4
INDIRF4
ADDRLP4 48
INDIRP4
CNSTI4 760
ADDP4
INDIRF4
CNSTF4 1073741824
ADDRLP4 44
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
line 117
;117:	} else {
ADDRGP4 $112
JUMPV
LABELV $111
line 120
;118:		// we can't just remove (self) here, because this is a touch function
;119:		// called while looping through area links...
;120:		ent->touch = 0;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
CNSTP4 0
ASGNP4
line 121
;121:		ent->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 122
;122:		ent->think = G_FreeEntity;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 G_FreeEntity
ASGNP4
line 123
;123:	}
LABELV $112
line 124
;124:}
LABELV $82
endproc multi_trigger 60 12
export Use_Multi
proc Use_Multi 0 8
line 126
;125:
;126:void Use_Multi( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
line 127
;127:	multi_trigger( ent, activator );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 multi_trigger
CALLV
pop
line 128
;128:}
LABELV $115
endproc Use_Multi 0 8
export Touch_Multi
proc Touch_Multi 0 8
line 130
;129:
;130:void Touch_Multi( gentity_t *self, gentity_t *other, trace_t *trace ) {
line 131
;131:	if( !other->client ) {
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $117
line 132
;132:		return;
ADDRGP4 $116
JUMPV
LABELV $117
line 134
;133:	}
;134:	multi_trigger( self, other );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 multi_trigger
CALLV
pop
line 135
;135:}
LABELV $116
endproc Touch_Multi 0 8
export SP_trigger_multiple
proc SP_trigger_multiple 12 12
line 151
;136:
;137:/*QUAKED trigger_multiple (.5 .5 .5) ? USE_BUTTON RED_ONLY BLUE_ONLY
;138:USE_BUTTON - Won't fire unless player is in it and pressing use button (in addition to any other conditions)
;139:RED_ONLY - Only red team can use
;140:BLUE_ONLY - Only blue team can use
;141:
;142:"roffname"		If set, will play a roff upon activation
;143:"rofftarget"	If set with roffname, will activate the roff an entity with
;144:				this as its targetname. Otherwise uses roff on activating entity.
;145:"wait" : Seconds between triggerings, 0.5 default, -1 = one time only.
;146:"random"	wait variance, default is 0
;147:Variable sized repeatable trigger.  Must be targeted at one or more entities.
;148:so, the basic time between firing is a random time between
;149:(wait - random) and (wait + random)
;150:*/
;151:void SP_trigger_multiple( gentity_t *ent ) {
line 152
;152:	if (ent->roffname)
ADDRFP4 0
INDIRP4
CNSTI4 428
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $120
line 153
;153:	{
line 154
;154:		ent->roffid = trap_ROFF_Cache(ent->roffname);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 428
ADDP4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 trap_ROFF_Cache
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 464
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 155
;155:	}
ADDRGP4 $121
JUMPV
LABELV $120
line 157
;156:	else
;157:	{
line 158
;158:		ent->roffid = -1;
ADDRFP4 0
INDIRP4
CNSTI4 464
ADDP4
CNSTI4 -1
ASGNI4
line 159
;159:	}
LABELV $121
line 161
;160:
;161:	G_SpawnFloat( "wait", "0.5", &ent->wait );
ADDRGP4 $122
ARGP4
ADDRGP4 $123
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 756
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
line 162
;162:	G_SpawnFloat( "random", "0", &ent->random );
ADDRGP4 $124
ARGP4
ADDRGP4 $125
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
line 164
;163:
;164:	if ( ent->random >= ent->wait && ent->wait >= 0 ) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 756
ADDP4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 760
ADDP4
INDIRF4
ADDRLP4 4
INDIRF4
LTF4 $126
ADDRLP4 4
INDIRF4
CNSTF4 0
LTF4 $126
line 165
;165:		ent->random = ent->wait - FRAMETIME;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CNSTI4 760
ADDP4
ADDRLP4 8
INDIRP4
CNSTI4 756
ADDP4
INDIRF4
CNSTF4 1120403456
SUBF4
ASGNF4
line 166
;166:		G_Printf( "trigger_multiple has random >= wait\n" );
ADDRGP4 $128
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 167
;167:	}
LABELV $126
line 169
;168:
;169:	ent->touch = Touch_Multi;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 Touch_Multi
ASGNP4
line 170
;170:	ent->use = Use_Multi;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 Use_Multi
ASGNP4
line 172
;171:
;172:	InitTrigger( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 InitTrigger
CALLV
pop
line 173
;173:	trap_LinkEntity (ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 174
;174:}
LABELV $119
endproc SP_trigger_multiple 12 12
export trigger_always_think
proc trigger_always_think 4 8
line 186
;175:
;176:
;177:
;178:/*
;179:==============================================================================
;180:
;181:trigger_always
;182:
;183:==============================================================================
;184:*/
;185:
;186:void trigger_always_think( gentity_t *ent ) {
line 187
;187:	G_UseTargets(ent, ent);
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
line 188
;188:	G_FreeEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 189
;189:}
LABELV $129
endproc trigger_always_think 4 8
export SP_trigger_always
proc SP_trigger_always 0 0
line 194
;190:
;191:/*QUAKED trigger_always (.5 .5 .5) (-8 -8 -8) (8 8 8)
;192:This trigger will always fire.  It is activated by the world.
;193:*/
;194:void SP_trigger_always (gentity_t *ent) {
line 196
;195:	// we must have some delay to make sure our use targets are present
;196:	ent->nextthink = level.time + 300;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 300
ADDI4
ASGNI4
line 197
;197:	ent->think = trigger_always_think;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 trigger_always_think
ASGNP4
line 198
;198:}
LABELV $130
endproc SP_trigger_always 0 0
export trigger_push_touch
proc trigger_push_touch 0 8
line 209
;199:
;200:
;201:/*
;202:==============================================================================
;203:
;204:trigger_push
;205:
;206:==============================================================================
;207:*/
;208:
;209:void trigger_push_touch (gentity_t *self, gentity_t *other, trace_t *trace ) {
line 211
;210:
;211:	if ( !other->client ) {
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $133
line 212
;212:		return;
ADDRGP4 $132
JUMPV
LABELV $133
line 215
;213:	}
;214:
;215:	BG_TouchJumpPad( &other->client->ps, &self->s );
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BG_TouchJumpPad
CALLV
pop
line 216
;216:}
LABELV $132
endproc trigger_push_touch 0 8
export AimAtTarget
proc AimAtTarget 72 4
line 226
;217:
;218:
;219:/*
;220:=================
;221:AimAtTarget
;222:
;223:Calculate origin2 so the target apogee will be hit
;224:=================
;225:*/
;226:void AimAtTarget( gentity_t *self ) {
line 232
;227:	gentity_t	*ent;
;228:	vec3_t		origin;
;229:	float		height, gravity, time, forward;
;230:	float		dist;
;231:
;232:	VectorAdd( self->r.absmin, self->r.absmax, origin );
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 36
INDIRP4
CNSTI4 344
ADDP4
INDIRF4
ADDRLP4 36
INDIRP4
CNSTI4 356
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 36
INDIRP4
CNSTI4 348
ADDP4
INDIRF4
ADDRLP4 36
INDIRP4
CNSTI4 360
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0+8
ADDRLP4 40
INDIRP4
CNSTI4 352
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
CNSTI4 364
ADDP4
INDIRF4
ADDF4
ASGNF4
line 233
;233:	VectorScale ( origin, 0.5, origin );
ADDRLP4 44
CNSTF4 1056964608
ASGNF4
ADDRLP4 0
ADDRLP4 44
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 44
INDIRF4
ADDRLP4 0+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0+8
CNSTF4 1056964608
ADDRLP4 0+8
INDIRF4
MULF4
ASGNF4
line 235
;234:
;235:	ent = G_PickTarget( self->target );
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
ARGP4
ADDRLP4 48
ADDRGP4 G_PickTarget
CALLP4
ASGNP4
ADDRLP4 12
ADDRLP4 48
INDIRP4
ASGNP4
line 236
;236:	if ( !ent ) {
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $142
line 237
;237:		G_FreeEntity( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 238
;238:		return;
ADDRGP4 $135
JUMPV
LABELV $142
line 241
;239:	}
;240:
;241:	height = ent->s.origin[2] - origin[2];
ADDRLP4 28
ADDRLP4 12
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRLP4 0+8
INDIRF4
SUBF4
ASGNF4
line 242
;242:	gravity = g_gravity.value;
ADDRLP4 24
ADDRGP4 g_gravity+8
INDIRF4
ASGNF4
line 243
;243:	time = sqrt( height / ( .5 * gravity ) );
ADDRLP4 28
INDIRF4
CNSTF4 1056964608
ADDRLP4 24
INDIRF4
MULF4
DIVF4
ARGF4
ADDRLP4 52
ADDRGP4 sqrt
CALLF4
ASGNF4
ADDRLP4 16
ADDRLP4 52
INDIRF4
ASGNF4
line 244
;244:	if ( !time ) {
ADDRLP4 16
INDIRF4
CNSTF4 0
NEF4 $146
line 245
;245:		G_FreeEntity( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 246
;246:		return;
ADDRGP4 $135
JUMPV
LABELV $146
line 250
;247:	}
;248:
;249:	// set s.origin2 to the push velocity
;250:	VectorSubtract ( ent->s.origin, origin, self->s.origin2 );
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
SUBF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRLP4 0+4
INDIRF4
SUBF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRLP4 0+8
INDIRF4
SUBF4
ASGNF4
line 251
;251:	self->s.origin2[2] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
CNSTF4 0
ASGNF4
line 252
;252:	dist = VectorNormalize( self->s.origin2);
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ARGP4
ADDRLP4 56
ADDRGP4 VectorNormalize
CALLF4
ASGNF4
ADDRLP4 32
ADDRLP4 56
INDIRF4
ASGNF4
line 254
;253:
;254:	forward = dist / time;
ADDRLP4 20
ADDRLP4 32
INDIRF4
ADDRLP4 16
INDIRF4
DIVF4
ASGNF4
line 255
;255:	VectorScale( self->s.origin2, forward, self->s.origin2 );
ADDRLP4 60
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRF4
ADDRLP4 20
INDIRF4
MULF4
ASGNF4
ADDRLP4 64
ADDRFP4 0
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 64
INDIRP4
ADDRLP4 64
INDIRP4
INDIRF4
ADDRLP4 20
INDIRF4
MULF4
ASGNF4
ADDRLP4 68
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ASGNP4
ADDRLP4 68
INDIRP4
ADDRLP4 68
INDIRP4
INDIRF4
ADDRLP4 20
INDIRF4
MULF4
ASGNF4
line 257
;256:
;257:	self->s.origin2[2] = time * gravity;
ADDRFP4 0
INDIRP4
CNSTI4 112
ADDP4
ADDRLP4 16
INDIRF4
ADDRLP4 24
INDIRF4
MULF4
ASGNF4
line 258
;258:}
LABELV $135
endproc AimAtTarget 72 4
export SP_trigger_push
proc SP_trigger_push 4 4
line 265
;259:
;260:
;261:/*QUAKED trigger_push (.5 .5 .5) ?
;262:Must point at a target_position, which will be the apex of the leap.
;263:This will be client side predicted, unlike target_push
;264:*/
;265:void SP_trigger_push( gentity_t *self ) {
line 266
;266:	InitTrigger (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 InitTrigger
CALLV
pop
line 269
;267:
;268:	// unlike other triggers, we need to send this one to the client
;269:	self->r.svFlags &= ~SVF_NOCLIENT;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 -2
BANDI4
ASGNI4
line 272
;270:
;271:	// make sure the client precaches this sound
;272:	G_SoundIndex("sound/weapons/force/jump.wav");
ADDRGP4 $151
ARGP4
ADDRGP4 G_SoundIndex
CALLI4
pop
line 274
;273:
;274:	self->s.eType = ET_PUSH_TRIGGER;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 10
ASGNI4
line 275
;275:	self->touch = trigger_push_touch;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 trigger_push_touch
ASGNP4
line 276
;276:	self->think = AimAtTarget;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 AimAtTarget
ASGNP4
line 277
;277:	self->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 278
;278:	trap_LinkEntity (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 279
;279:}
LABELV $150
endproc SP_trigger_push 4 4
export Use_target_push
proc Use_target_push 4 12
line 282
;280:
;281:
;282:void Use_target_push( gentity_t *self, gentity_t *other, gentity_t *activator ) {
line 283
;283:	if ( !activator->client ) {
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $154
line 284
;284:		return;
ADDRGP4 $153
JUMPV
LABELV $154
line 287
;285:	}
;286:
;287:	if ( activator->client->ps.pm_type != PM_NORMAL && activator->client->ps.pm_type != PM_FLOAT ) {
ADDRLP4 0
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $156
ADDRLP4 0
INDIRI4
CNSTI4 1
EQI4 $156
line 288
;288:		return;
ADDRGP4 $153
JUMPV
LABELV $156
line 291
;289:	}
;290:
;291:	VectorCopy (self->s.origin2, activator->client->ps.velocity);
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 32
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 294
;292:
;293:	// play fly sound every 1.5 seconds
;294:	if ( activator->fly_sound_debounce_time < level.time ) {
ADDRFP4 8
INDIRP4
CNSTI4 668
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $158
line 295
;295:		activator->fly_sound_debounce_time = level.time + 1500;
ADDRFP4 8
INDIRP4
CNSTI4 668
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1500
ADDI4
ASGNI4
line 296
;296:		G_Sound( activator, CHAN_AUTO, self->noise_index );
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 752
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 297
;297:	}
LABELV $158
line 298
;298:}
LABELV $153
endproc Use_target_push 4 12
export SP_target_push
proc SP_target_push 36 8
line 305
;299:
;300:/*QUAKED target_push (.5 .5 .5) (-8 -8 -8) (8 8 8) bouncepad
;301:Pushes the activator in the direction.of angle, or towards a target apex.
;302:"speed"		defaults to 1000
;303:if "bouncepad", play bounce noise instead of windfly
;304:*/
;305:void SP_target_push( gentity_t *self ) {
line 306
;306:	if (!self->speed) {
ADDRFP4 0
INDIRP4
CNSTI4 608
ADDP4
INDIRF4
CNSTF4 0
NEF4 $163
line 307
;307:		self->speed = 1000;
ADDRFP4 0
INDIRP4
CNSTI4 608
ADDP4
CNSTF4 1148846080
ASGNF4
line 308
;308:	}
LABELV $163
line 309
;309:	G_SetMovedir (self->s.angles, self->s.origin2);
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 104
ADDP4
ARGP4
ADDRGP4 G_SetMovedir
CALLV
pop
line 310
;310:	VectorScale (self->s.origin2, self->speed, self->s.origin2);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 4
INDIRP4
CNSTI4 104
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRF4
ADDRLP4 4
INDIRP4
CNSTI4 608
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
ADDRLP4 12
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRF4
ADDRLP4 12
INDIRP4
CNSTI4 608
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
ADDRLP4 20
INDIRP4
CNSTI4 112
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRF4
ADDRLP4 20
INDIRP4
CNSTI4 608
ADDP4
INDIRF4
MULF4
ASGNF4
line 312
;311:
;312:	if ( self->spawnflags & 1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $165
line 313
;313:		self->noise_index = G_SoundIndex("sound/weapons/force/jump.wav");
ADDRGP4 $151
ARGP4
ADDRLP4 28
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 752
ADDP4
ADDRLP4 28
INDIRI4
ASGNI4
line 314
;314:	} else {
ADDRGP4 $166
JUMPV
LABELV $165
line 315
;315:		self->noise_index = 0;	//G_SoundIndex("sound/misc/windfly.wav");
ADDRFP4 0
INDIRP4
CNSTI4 752
ADDP4
CNSTI4 0
ASGNI4
line 316
;316:	}
LABELV $166
line 317
;317:	if ( self->target ) {
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $167
line 318
;318:		VectorCopy( self->s.origin, self->r.absmin );
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 344
ADDP4
ADDRLP4 28
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 319
;319:		VectorCopy( self->s.origin, self->r.absmax );
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 356
ADDP4
ADDRLP4 32
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 320
;320:		self->think = AimAtTarget;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 AimAtTarget
ASGNP4
line 321
;321:		self->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 322
;322:	}
LABELV $167
line 323
;323:	self->use = Use_target_push;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 Use_target_push
ASGNP4
line 324
;324:}
LABELV $162
endproc SP_target_push 36 8
export trigger_teleporter_touch
proc trigger_teleporter_touch 16 12
line 334
;325:
;326:/*
;327:==============================================================================
;328:
;329:trigger_teleport
;330:
;331:==============================================================================
;332:*/
;333:
;334:void trigger_teleporter_touch (gentity_t *self, gentity_t *other, trace_t *trace ) {
line 337
;335:	gentity_t	*dest;
;336:
;337:	if ( !other->client ) {
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $171
line 338
;338:		return;
ADDRGP4 $170
JUMPV
LABELV $171
line 340
;339:	}
;340:	if ( other->client->ps.pm_type == PM_DEAD ) {
ADDRLP4 4
CNSTI4 4
ASGNI4
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ADDRLP4 4
INDIRI4
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
NEI4 $173
line 341
;341:		return;
ADDRGP4 $170
JUMPV
LABELV $173
line 344
;342:	}
;343:	// Spectators only?
;344:	if ( ( self->spawnflags & 1 ) && 
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $175
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
EQI4 $175
line 345
;345:		other->client->sess.sessionTeam != TEAM_SPECTATOR ) {
line 346
;346:		return;
ADDRGP4 $170
JUMPV
LABELV $175
line 350
;347:	}
;348:
;349:
;350:	dest = 	G_PickTarget( self->target );
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 G_PickTarget
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 351
;351:	if (!dest) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $177
line 352
;352:		G_Printf ("Couldn't find teleporter destination\n");
ADDRGP4 $179
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 353
;353:		return;
ADDRGP4 $170
JUMPV
LABELV $177
line 356
;354:	}
;355:
;356:	TeleportPlayer( other, dest->s.origin, dest->s.angles );
ADDRFP4 4
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
line 357
;357:}
LABELV $170
endproc trigger_teleporter_touch 16 12
export SP_trigger_teleport
proc SP_trigger_teleport 4 4
line 368
;358:
;359:
;360:/*QUAKED trigger_teleport (.5 .5 .5) ? SPECTATOR
;361:Allows client side prediction of teleportation events.
;362:Must point at a target_position, which will be the teleport destination.
;363:
;364:If spectator is set, only spectators can use this teleport
;365:Spectator teleporters are not normally placed in the editor, but are created
;366:automatically near doors to allow spectators to move through them
;367:*/
;368:void SP_trigger_teleport( gentity_t *self ) {
line 369
;369:	InitTrigger (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 InitTrigger
CALLV
pop
line 373
;370:
;371:	// unlike other triggers, we need to send this one to the client
;372:	// unless is a spectator trigger
;373:	if ( self->spawnflags & 1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $181
line 374
;374:		self->r.svFlags |= SVF_NOCLIENT;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 375
;375:	} else {
ADDRGP4 $182
JUMPV
LABELV $181
line 376
;376:		self->r.svFlags &= ~SVF_NOCLIENT;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 -2
BANDI4
ASGNI4
line 377
;377:	}
LABELV $182
line 380
;378:
;379:	// make sure the client precaches this sound
;380:	G_SoundIndex("sound/weapons/force/speed.wav");
ADDRGP4 $183
ARGP4
ADDRGP4 G_SoundIndex
CALLI4
pop
line 382
;381:
;382:	self->s.eType = ET_TELEPORT_TRIGGER;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 11
ASGNI4
line 383
;383:	self->touch = trigger_teleporter_touch;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 trigger_teleporter_touch
ASGNP4
line 385
;384:
;385:	trap_LinkEntity (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 386
;386:}
LABELV $180
endproc SP_trigger_teleport 4 4
export hurt_use
proc hurt_use 8 4
line 410
;387:
;388:
;389:/*
;390:==============================================================================
;391:
;392:trigger_hurt
;393:
;394:==============================================================================
;395:*/
;396:
;397:/*QUAKED trigger_hurt (.5 .5 .5) ? START_OFF CAN_TARGET SILENT NO_PROTECTION SLOW
;398:Any entity that touches this will be hurt.
;399:It does dmg points of damage each server frame
;400:Targeting the trigger will toggle its on / off state.
;401:
;402:SILENT			supresses playing the sound
;403:SLOW			changes the damage rate to once per second
;404:NO_PROTECTION	*nothing* stops the damage
;405:
;406:"dmg"			default 5 (whole numbers only)
;407:If dmg is set to -1 this brush will use the fade-kill method
;408:
;409:*/
;410:void hurt_use( gentity_t *self, gentity_t *other, gentity_t *activator ) {
line 411
;411:	if (activator && activator->inuse && activator->client)
ADDRLP4 0
ADDRFP4 8
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
EQU4 $185
ADDRLP4 0
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $185
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
EQU4 $185
line 412
;412:	{
line 413
;413:		self->activator = activator;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ADDRFP4 8
INDIRP4
ASGNP4
line 414
;414:	}
ADDRGP4 $186
JUMPV
LABELV $185
line 416
;415:	else
;416:	{
line 417
;417:		self->activator = NULL;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
CNSTP4 0
ASGNP4
line 418
;418:	}
LABELV $186
line 420
;419:
;420:	if ( self->r.linked ) {
ADDRFP4 0
INDIRP4
CNSTI4 296
ADDP4
INDIRI4
CNSTI4 0
EQI4 $187
line 421
;421:		trap_UnlinkEntity( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 422
;422:	} else {
ADDRGP4 $188
JUMPV
LABELV $187
line 423
;423:		trap_LinkEntity( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 424
;424:	}
LABELV $188
line 425
;425:}
LABELV $184
endproc hurt_use 8 4
export hurt_touch
proc hurt_touch 56 32
line 427
;426:
;427:void hurt_touch( gentity_t *self, gentity_t *other, trace_t *trace ) {
line 430
;428:	int		dflags;
;429:
;430:	if ( !other->takedamage ) {
ADDRFP4 4
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
CNSTI4 0
NEI4 $190
line 431
;431:		return;
ADDRGP4 $189
JUMPV
LABELV $190
line 434
;432:	}
;433:
;434:	if ( self->timestamp > level.time ) {
ADDRFP4 0
INDIRP4
CNSTI4 576
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
LEI4 $192
line 435
;435:		return;
ADDRGP4 $189
JUMPV
LABELV $192
line 438
;436:	}
;437:
;438:	if (self->damage == -1 && other && other->client && other->health < 1)
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $195
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
EQU4 $195
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 8
INDIRU4
EQU4 $195
ADDRLP4 4
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 1
GEI4 $195
line 439
;439:	{
line 440
;440:		other->client->ps.fallingToDeath = 0;
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1340
ADDP4
CNSTI4 0
ASGNI4
line 441
;441:		respawn(other);
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 respawn
CALLV
pop
line 442
;442:		return;
ADDRGP4 $189
JUMPV
LABELV $195
line 445
;443:	}
;444:
;445:	if (self->damage == -1 && other && other->client && other->client->ps.fallingToDeath)
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $197
ADDRLP4 12
ADDRFP4 4
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
EQU4 $197
ADDRLP4 20
ADDRLP4 12
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CVPU4 4
ADDRLP4 16
INDIRU4
EQU4 $197
ADDRLP4 20
INDIRP4
CNSTI4 1340
ADDP4
INDIRI4
CNSTI4 0
EQI4 $197
line 446
;446:	{
line 447
;447:		return;
ADDRGP4 $189
JUMPV
LABELV $197
line 450
;448:	}
;449:
;450:	if ( self->spawnflags & 16 ) {
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $199
line 451
;451:		self->timestamp = level.time + 1000;
ADDRFP4 0
INDIRP4
CNSTI4 576
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1000
ADDI4
ASGNI4
line 452
;452:	} else {
ADDRGP4 $200
JUMPV
LABELV $199
line 453
;453:		self->timestamp = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 576
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 454
;454:	}
LABELV $200
line 457
;455:
;456:	// play sound
;457:	if ( !(self->spawnflags & 4) && self->damage != -1 ) {
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
NEI4 $203
ADDRLP4 24
INDIRP4
CNSTI4 688
ADDP4
INDIRI4
CNSTI4 -1
EQI4 $203
line 458
;458:		G_Sound( other, CHAN_AUTO, self->noise_index );
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 752
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 459
;459:	}
LABELV $203
line 461
;460:
;461:	if (self->spawnflags & 8)
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $205
line 462
;462:		dflags = DAMAGE_NO_PROTECTION;
ADDRLP4 0
CNSTI4 8
ASGNI4
ADDRGP4 $206
JUMPV
LABELV $205
line 464
;463:	else
;464:		dflags = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $206
line 466
;465:
;466:	if (self->damage == -1 && other && other->client)
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $207
ADDRLP4 28
ADDRFP4 4
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
EQU4 $207
ADDRLP4 28
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 32
INDIRU4
EQU4 $207
line 467
;467:	{
line 468
;468:		if (other->client->ps.otherKillerTime > level.time)
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 752
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
LEI4 $209
line 469
;469:		{ //we're as good as dead, so if someone pushed us into this then remember them
line 470
;470:			other->client->ps.otherKillerTime = level.time + 20000;
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 752
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 20000
ADDI4
ASGNI4
line 471
;471:			other->client->ps.otherKillerDebounceTime = level.time + 10000;
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 756
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 10000
ADDI4
ASGNI4
line 472
;472:		}
LABELV $209
line 473
;473:		other->client->ps.fallingToDeath = level.time;
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1340
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 475
;474:
;475:		self->timestamp = 0; //do not ignore others
ADDRFP4 0
INDIRP4
CNSTI4 576
ADDP4
CNSTI4 0
ASGNI4
line 476
;476:		G_EntitySound(other, CHAN_VOICE, G_SoundIndex("*falling1.wav"));
ADDRGP4 $215
ARGP4
ADDRLP4 36
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 3
ARGI4
ADDRLP4 36
INDIRI4
ARGI4
ADDRGP4 G_EntitySound
CALLV
pop
line 477
;477:	}
ADDRGP4 $208
JUMPV
LABELV $207
line 479
;478:	else	
;479:	{
line 480
;480:		int dmg = self->damage;
ADDRLP4 36
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
INDIRI4
ASGNI4
line 482
;481:
;482:		if (dmg == -1)
ADDRLP4 36
INDIRI4
CNSTI4 -1
NEI4 $216
line 483
;483:		{ //so fall-to-blackness triggers destroy evertyhing
line 484
;484:			dmg = 99999;
ADDRLP4 36
CNSTI4 99999
ASGNI4
line 485
;485:			self->timestamp = 0;
ADDRFP4 0
INDIRP4
CNSTI4 576
ADDP4
CNSTI4 0
ASGNI4
line 486
;486:		}
LABELV $216
line 487
;487:		if (self->activator && self->activator->inuse && self->activator->client)
ADDRLP4 40
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
ASGNP4
ADDRLP4 44
CNSTU4 0
ASGNU4
ADDRLP4 40
INDIRP4
CVPU4 4
ADDRLP4 44
INDIRU4
EQU4 $218
ADDRLP4 40
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $218
ADDRLP4 40
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 44
INDIRU4
EQU4 $218
line 488
;488:		{
line 489
;489:			G_Damage (other, self->activator, self->activator, NULL, NULL, dmg, dflags|DAMAGE_NO_PROTECTION, MOD_TRIGGER_HURT);
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 48
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
ARGP4
ADDRLP4 48
INDIRP4
ARGP4
ADDRLP4 52
CNSTP4 0
ASGNP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRLP4 36
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
CNSTI4 8
BORI4
ARGI4
CNSTI4 37
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 490
;490:		}
ADDRGP4 $219
JUMPV
LABELV $218
line 492
;491:		else
;492:		{
line 493
;493:			G_Damage (other, self, self, NULL, NULL, dmg, dflags|DAMAGE_NO_PROTECTION, MOD_TRIGGER_HURT);
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
ARGP4
ADDRLP4 48
INDIRP4
ARGP4
ADDRLP4 52
CNSTP4 0
ASGNP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRLP4 36
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
CNSTI4 8
BORI4
ARGI4
CNSTI4 37
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 494
;494:		}
LABELV $219
line 495
;495:	}
LABELV $208
line 496
;496:}
LABELV $189
endproc hurt_touch 56 32
export SP_trigger_hurt
proc SP_trigger_hurt 8 4
line 498
;497:
;498:void SP_trigger_hurt( gentity_t *self ) {
line 499
;499:	InitTrigger (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 InitTrigger
CALLV
pop
line 501
;500:
;501:	gTrigFallSound = G_SoundIndex("*falling1.wav");
ADDRGP4 $215
ARGP4
ADDRLP4 0
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRGP4 gTrigFallSound
ADDRLP4 0
INDIRI4
ASGNI4
line 503
;502:
;503:	self->noise_index = G_SoundIndex( "sound/weapons/force/speed.wav" );
ADDRGP4 $183
ARGP4
ADDRLP4 4
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 752
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 504
;504:	self->touch = hurt_touch;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 hurt_touch
ASGNP4
line 506
;505:
;506:	if ( !self->damage ) {
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
INDIRI4
CNSTI4 0
NEI4 $221
line 507
;507:		self->damage = 5;
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
CNSTI4 5
ASGNI4
line 508
;508:	}
LABELV $221
line 510
;509:
;510:	self->r.contents = CONTENTS_TRIGGER;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1024
ASGNI4
line 512
;511:
;512:	if ( self->spawnflags & 2 ) {
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $223
line 513
;513:		self->use = hurt_use;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 hurt_use
ASGNP4
line 514
;514:	}
LABELV $223
line 517
;515:
;516:	// link in to the world if starting active
;517:	if ( ! (self->spawnflags & 1) ) {
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $225
line 518
;518:		trap_LinkEntity (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 519
;519:	}
ADDRGP4 $226
JUMPV
LABELV $225
line 520
;520:	else if (self->r.linked)
ADDRFP4 0
INDIRP4
CNSTI4 296
ADDP4
INDIRI4
CNSTI4 0
EQI4 $227
line 521
;521:	{
line 522
;522:		trap_UnlinkEntity(self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 523
;523:	}
LABELV $227
LABELV $226
line 524
;524:}
LABELV $220
endproc SP_trigger_hurt 8 4
export func_timer_think
proc func_timer_think 12 8
line 547
;525:
;526:
;527:/*
;528:==============================================================================
;529:
;530:timer
;531:
;532:==============================================================================
;533:*/
;534:
;535:
;536:/*QUAKED func_timer (0.3 0.1 0.6) (-8 -8 -8) (8 8 8) START_ON
;537:This should be renamed trigger_timer...
;538:Repeatedly fires its targets.
;539:Can be turned on or off by using.
;540:
;541:"wait"			base time between triggering all targets, default is 1
;542:"random"		wait variance, default is 0
;543:so, the basic time between firing is a random time between
;544:(wait - random) and (wait + random)
;545:
;546:*/
;547:void func_timer_think( gentity_t *self ) {
line 548
;548:	G_UseTargets (self, self->activator);
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
line 550
;549:	// set time before next firing
;550:	self->nextthink = level.time + 1000 * ( self->wait + crandom() * self->random );
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
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
INDIRF4
MULF4
ADDF4
MULF4
ADDF4
CVFI4 4
ASGNI4
line 551
;551:}
LABELV $229
endproc func_timer_think 12 8
export func_timer_use
proc func_timer_use 0 4
line 553
;552:
;553:void func_timer_use( gentity_t *self, gentity_t *other, gentity_t *activator ) {
line 554
;554:	self->activator = activator;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ADDRFP4 8
INDIRP4
ASGNP4
line 557
;555:
;556:	// if on, turn it off
;557:	if ( self->nextthink ) {
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
INDIRI4
CNSTI4 0
EQI4 $232
line 558
;558:		self->nextthink = 0;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
CNSTI4 0
ASGNI4
line 559
;559:		return;
ADDRGP4 $231
JUMPV
LABELV $232
line 563
;560:	}
;561:
;562:	// turn it on
;563:	func_timer_think (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 func_timer_think
CALLV
pop
line 564
;564:}
LABELV $231
endproc func_timer_use 0 4
export SP_func_timer
proc SP_func_timer 12 12
line 566
;565:
;566:void SP_func_timer( gentity_t *self ) {
line 567
;567:	G_SpawnFloat( "random", "1", &self->random);
ADDRGP4 $124
ARGP4
ADDRGP4 $235
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
line 568
;568:	G_SpawnFloat( "wait", "1", &self->wait );
ADDRGP4 $122
ARGP4
ADDRGP4 $235
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 756
ADDP4
ARGP4
ADDRGP4 G_SpawnFloat
CALLI4
pop
line 570
;569:
;570:	self->use = func_timer_use;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 func_timer_use
ASGNP4
line 571
;571:	self->think = func_timer_think;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 func_timer_think
ASGNP4
line 573
;572:
;573:	if ( self->random >= self->wait ) {
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 760
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 756
ADDP4
INDIRF4
LTF4 $236
line 574
;574:		self->random = self->wait - FRAMETIME;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 760
ADDP4
ADDRLP4 4
INDIRP4
CNSTI4 756
ADDP4
INDIRF4
CNSTF4 1120403456
SUBF4
ASGNF4
line 575
;575:		G_Printf( "func_timer at %s has random >= wait\n", vtos( self->s.origin ) );
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 8
ADDRGP4 vtos
CALLP4
ASGNP4
ADDRGP4 $238
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 576
;576:	}
LABELV $236
line 578
;577:
;578:	if ( self->spawnflags & 1 ) {
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $239
line 579
;579:		self->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 580
;580:		self->activator = self;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 732
ADDP4
ADDRLP4 4
INDIRP4
ASGNP4
line 581
;581:	}
LABELV $239
line 583
;582:
;583:	self->r.svFlags = SVF_NOCLIENT;
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
CNSTI4 1
ASGNI4
line 584
;584:}
LABELV $234
endproc SP_func_timer 12 12
bss
export gTrigFallSound
align 4
LABELV gTrigFallSound
skip 4
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
LABELV $238
char 1 102
char 1 117
char 1 110
char 1 99
char 1 95
char 1 116
char 1 105
char 1 109
char 1 101
char 1 114
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
char 1 114
char 1 97
char 1 110
char 1 100
char 1 111
char 1 109
char 1 32
char 1 62
char 1 61
char 1 32
char 1 119
char 1 97
char 1 105
char 1 116
char 1 10
char 1 0
align 1
LABELV $235
char 1 49
char 1 0
align 1
LABELV $215
char 1 42
char 1 102
char 1 97
char 1 108
char 1 108
char 1 105
char 1 110
char 1 103
char 1 49
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $183
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
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 47
char 1 115
char 1 112
char 1 101
char 1 101
char 1 100
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $179
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
LABELV $151
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
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 47
char 1 106
char 1 117
char 1 109
char 1 112
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $128
char 1 116
char 1 114
char 1 105
char 1 103
char 1 103
char 1 101
char 1 114
char 1 95
char 1 109
char 1 117
char 1 108
char 1 116
char 1 105
char 1 112
char 1 108
char 1 101
char 1 32
char 1 104
char 1 97
char 1 115
char 1 32
char 1 114
char 1 97
char 1 110
char 1 100
char 1 111
char 1 109
char 1 32
char 1 62
char 1 61
char 1 32
char 1 119
char 1 97
char 1 105
char 1 116
char 1 10
char 1 0
align 1
LABELV $125
char 1 48
char 1 0
align 1
LABELV $124
char 1 114
char 1 97
char 1 110
char 1 100
char 1 111
char 1 109
char 1 0
align 1
LABELV $123
char 1 48
char 1 46
char 1 53
char 1 0
align 1
LABELV $122
char 1 119
char 1 97
char 1 105
char 1 116
char 1 0
