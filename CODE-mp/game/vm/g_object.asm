lit
align 4
LABELV $79
byte 4 0
byte 4 0
byte 4 0
export pitch_roll_for_slope
code
proc pitch_roll_for_slope 1200 28
file "../g_object.c"
line 22
;1:// leave this line at the top for all g_xxxx.cpp files...
;2:#include "g_local.h"
;3:
;4:extern void G_MoverTouchPushTriggers( gentity_t *ent, vec3_t oldOrg );
;5:void G_StopObjectMoving( gentity_t *object );
;6:
;7:/*
;8:====================================================================
;9:void pitch_roll_for_slope (edict_t *forwhom, vec3_t *slope)
;10:
;11:MG
;12:
;13:This will adjust the pitch and roll of a monster to match
;14:a given slope - if a non-'0 0 0' slope is passed, it will
;15:use that value, otherwise it will use the ground underneath
;16:the monster.  If it doesn't find a surface, it does nothinh\g
;17:and returns.
;18:====================================================================
;19:*/
;20:
;21:void pitch_roll_for_slope( gentity_t *forwhom, vec3_t pass_slope )
;22:{
line 24
;23:	vec3_t	slope;
;24:	vec3_t	nvf, ovf, ovr, startspot, endspot, new_angles = { 0, 0, 0 };
ADDRLP4 12
ADDRGP4 $79
INDIRB
ASGNB 12
line 29
;25:	float	pitch, mod, dot;
;26:	float	oldmins2;
;27:
;28:	//if we don't have a slope, get one
;29:	if( !pass_slope || VectorCompare( vec3_origin, pass_slope ) )
ADDRLP4 100
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 100
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $82
ADDRGP4 vec3_origin
ARGP4
ADDRLP4 100
INDIRP4
ARGP4
ADDRLP4 104
ADDRGP4 VectorCompare
CALLI4
ASGNI4
ADDRLP4 104
INDIRI4
CNSTI4 0
EQI4 $80
LABELV $82
line 30
;30:	{
line 33
;31:		trace_t trace;
;32:
;33:		VectorCopy( forwhom->r.currentOrigin, startspot );
ADDRLP4 72
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 34
;34:		startspot[2] += forwhom->r.mins[2] + 4;
ADDRLP4 72+8
ADDRLP4 72+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 324
ADDP4
INDIRF4
CNSTF4 1082130432
ADDF4
ADDF4
ASGNF4
line 35
;35:		VectorCopy( startspot, endspot );
ADDRLP4 84
ADDRLP4 72
INDIRB
ASGNB 12
line 36
;36:		endspot[2] -= 300;
ADDRLP4 84+8
ADDRLP4 84+8
INDIRF4
CNSTF4 1133903872
SUBF4
ASGNF4
line 37
;37:		trap_Trace( &trace, forwhom->r.currentOrigin, vec3_origin, vec3_origin, endspot, forwhom->s.number, MASK_SOLID );
ADDRLP4 108
ARGP4
ADDRLP4 1188
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1188
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 1192
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 1192
INDIRP4
ARGP4
ADDRLP4 1192
INDIRP4
ARGP4
ADDRLP4 84
ARGP4
ADDRLP4 1188
INDIRP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 41
;38://		if(trace_fraction>0.05&&forwhom.movetype==MOVETYPE_STEP)
;39://			forwhom.flags(-)FL_ONGROUND;
;40:
;41:		if ( trace.fraction >= 1.0 )
ADDRLP4 108+8
INDIRF4
CNSTF4 1065353216
LTF4 $85
line 42
;42:			return;
ADDRGP4 $78
JUMPV
LABELV $85
line 44
;43:
;44:		if( !( &trace.plane ) )
ADDRLP4 108+24
CVPU4 4
CNSTU4 0
NEU4 $88
line 45
;45:			return;
ADDRGP4 $78
JUMPV
LABELV $88
line 47
;46:
;47:		if ( VectorCompare( vec3_origin, trace.plane.normal ) )
ADDRGP4 vec3_origin
ARGP4
ADDRLP4 108+24
ARGP4
ADDRLP4 1196
ADDRGP4 VectorCompare
CALLI4
ASGNI4
ADDRLP4 1196
INDIRI4
CNSTI4 0
EQI4 $91
line 48
;48:			return;
ADDRGP4 $78
JUMPV
LABELV $91
line 50
;49:
;50:		VectorCopy( trace.plane.normal, slope );
ADDRLP4 60
ADDRLP4 108+24
INDIRB
ASGNB 12
line 51
;51:	}
ADDRGP4 $81
JUMPV
LABELV $80
line 53
;52:	else
;53:	{
line 54
;54:		VectorCopy( pass_slope, slope );
ADDRLP4 60
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 55
;55:	}
LABELV $81
line 58
;56:
;57:
;58:	AngleVectors( forwhom->r.currentAngles, ovf, ovr, NULL );
ADDRFP4 0
INDIRP4
CNSTI4 380
ADDP4
ARGP4
ADDRLP4 24
ARGP4
ADDRLP4 36
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 60
;59:
;60:	vectoangles( slope, new_angles );
ADDRLP4 60
ARGP4
ADDRLP4 12
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 61
;61:	pitch = new_angles[PITCH] + 90;
ADDRLP4 52
ADDRLP4 12
INDIRF4
CNSTF4 1119092736
ADDF4
ASGNF4
line 62
;62:	new_angles[ROLL] = new_angles[PITCH] = 0;
ADDRLP4 108
CNSTF4 0
ASGNF4
ADDRLP4 12
ADDRLP4 108
INDIRF4
ASGNF4
ADDRLP4 12+8
ADDRLP4 108
INDIRF4
ASGNF4
line 64
;63:
;64:	AngleVectors( new_angles, nvf, NULL, NULL );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 112
CNSTP4 0
ASGNP4
ADDRLP4 112
INDIRP4
ARGP4
ADDRLP4 112
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 66
;65:
;66:	mod = DotProduct( nvf, ovr );
ADDRLP4 48
ADDRLP4 0
INDIRF4
ADDRLP4 36
INDIRF4
MULF4
ADDRLP4 0+4
INDIRF4
ADDRLP4 36+4
INDIRF4
MULF4
ADDF4
ADDRLP4 0+8
INDIRF4
ADDRLP4 36+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 68
;67:
;68:	if ( mod<0 )
ADDRLP4 48
INDIRF4
CNSTF4 0
GEF4 $100
line 69
;69:		mod = -1;
ADDRLP4 48
CNSTF4 3212836864
ASGNF4
ADDRGP4 $101
JUMPV
LABELV $100
line 71
;70:	else
;71:		mod = 1;
ADDRLP4 48
CNSTF4 1065353216
ASGNF4
LABELV $101
line 73
;72:
;73:	dot = DotProduct( nvf, ovf );
ADDRLP4 56
ADDRLP4 0
INDIRF4
ADDRLP4 24
INDIRF4
MULF4
ADDRLP4 0+4
INDIRF4
ADDRLP4 24+4
INDIRF4
MULF4
ADDF4
ADDRLP4 0+8
INDIRF4
ADDRLP4 24+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 75
;74:
;75:	if ( forwhom->client )
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $106
line 76
;76:	{
line 77
;77:		forwhom->client->ps.viewangles[PITCH] = dot * pitch;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 156
ADDP4
ADDRLP4 56
INDIRF4
ADDRLP4 52
INDIRF4
MULF4
ASGNF4
line 78
;78:		forwhom->client->ps.viewangles[ROLL] = ((1-Q_fabs(dot)) * pitch * mod);
ADDRLP4 56
INDIRF4
ARGF4
ADDRLP4 116
ADDRGP4 Q_fabs
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 164
ADDP4
CNSTF4 1065353216
ADDRLP4 116
INDIRF4
SUBF4
ADDRLP4 52
INDIRF4
MULF4
ADDRLP4 48
INDIRF4
MULF4
ASGNF4
line 79
;79:		oldmins2 = forwhom->r.mins[2];
ADDRLP4 96
ADDRFP4 0
INDIRP4
CNSTI4 324
ADDP4
INDIRF4
ASGNF4
line 80
;80:		forwhom->r.mins[2] = -24 + 12 * fabs(forwhom->client->ps.viewangles[PITCH])/180.0f;
ADDRLP4 120
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 120
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 156
ADDP4
INDIRF4
ARGF4
ADDRLP4 124
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 120
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 1094713344
ADDRLP4 124
INDIRF4
MULF4
CNSTF4 1127481344
DIVF4
CNSTF4 3250585600
ADDF4
ASGNF4
line 82
;81:		//FIXME: if it gets bigger, move up
;82:		if ( oldmins2 > forwhom->r.mins[2] )
ADDRLP4 96
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 324
ADDP4
INDIRF4
LEF4 $107
line 83
;83:		{//our mins is now lower, need to move up
line 85
;84:			//FIXME: trace?
;85:			forwhom->client->ps.origin[2] += (oldmins2 - forwhom->r.mins[2]);
ADDRLP4 128
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 132
ADDRLP4 128
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 28
ADDP4
ASGNP4
ADDRLP4 132
INDIRP4
ADDRLP4 132
INDIRP4
INDIRF4
ADDRLP4 96
INDIRF4
ADDRLP4 128
INDIRP4
CNSTI4 324
ADDP4
INDIRF4
SUBF4
ADDF4
ASGNF4
line 86
;86:			forwhom->r.currentOrigin[2] = forwhom->client->ps.origin[2];
ADDRLP4 136
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 136
INDIRP4
CNSTI4 376
ADDP4
ADDRLP4 136
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ASGNF4
line 87
;87:			trap_LinkEntity( forwhom );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 88
;88:		}
line 89
;89:	}
ADDRGP4 $107
JUMPV
LABELV $106
line 91
;90:	else
;91:	{
line 92
;92:		forwhom->r.currentAngles[PITCH] = dot * pitch;
ADDRFP4 0
INDIRP4
CNSTI4 380
ADDP4
ADDRLP4 56
INDIRF4
ADDRLP4 52
INDIRF4
MULF4
ASGNF4
line 93
;93:		forwhom->r.currentAngles[ROLL] = ((1-Q_fabs(dot)) * pitch * mod);
ADDRLP4 56
INDIRF4
ARGF4
ADDRLP4 116
ADDRGP4 Q_fabs
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 388
ADDP4
CNSTF4 1065353216
ADDRLP4 116
INDIRF4
SUBF4
ADDRLP4 52
INDIRF4
MULF4
ADDRLP4 48
INDIRF4
MULF4
ASGNF4
line 94
;94:	}
LABELV $107
line 95
;95:}
LABELV $78
endproc pitch_roll_for_slope 1200 28
export G_BounceObject
proc G_BounceObject 44 12
line 104
;96:
;97:/*
;98:================
;99:G_BounceObject
;100:
;101:================
;102:*/
;103:void G_BounceObject( gentity_t *ent, trace_t *trace ) 
;104:{
line 110
;105:	vec3_t	velocity;
;106:	float	dot, bounceFactor;
;107:	int		hitTime;
;108:
;109:	// reflect the velocity on the trace plane
;110:	hitTime = level.previousTime + ( level.time - level.previousTime ) * trace->fraction;
ADDRLP4 20
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
line 111
;111:	BG_EvaluateTrajectoryDelta( &ent->s.pos, hitTime, velocity );
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
ARGP4
ADDRLP4 20
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BG_EvaluateTrajectoryDelta
CALLV
pop
line 112
;112:	dot = DotProduct( velocity, trace->plane.normal );
ADDRLP4 24
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 16
ADDRLP4 0
INDIRF4
ADDRLP4 24
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
MULF4
ADDRLP4 0+4
INDIRF4
ADDRLP4 24
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
MULF4
ADDF4
ADDRLP4 0+8
INDIRF4
ADDRLP4 24
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 114
;113://	bounceFactor = 60/ent->mass;		// NOTENOTE Mass is not yet implemented
;114:	bounceFactor = 1.0f;
ADDRLP4 12
CNSTF4 1065353216
ASGNF4
line 115
;115:	if ( bounceFactor > 1.0f )
ADDRLP4 12
INDIRF4
CNSTF4 1065353216
LEF4 $116
line 116
;116:	{
line 117
;117:		bounceFactor = 1.0f;
ADDRLP4 12
CNSTF4 1065353216
ASGNF4
line 118
;118:	}
LABELV $116
line 119
;119:	VectorMA( velocity, -2*dot*bounceFactor, trace->plane.normal, ent->s.pos.trDelta );
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
ADDRLP4 16
INDIRF4
MULF4
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
ADDRLP4 16
INDIRF4
MULF4
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
ADDRLP4 16
INDIRF4
MULF4
ADDRLP4 12
INDIRF4
MULF4
MULF4
ADDF4
ASGNF4
line 122
;120:
;121:	//FIXME: customized or material-based impact/bounce sounds
;122:	if ( ent->s.eFlags & EF_BOUNCE_HALF ) 
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 32
BANDI4
CNSTI4 0
EQI4 $120
line 123
;123:	{
line 124
;124:		VectorScale( ent->s.pos.trDelta, 0.5, ent->s.pos.trDelta );
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTF4 1056964608
ADDRLP4 28
INDIRP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 32
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTF4 1056964608
ADDRLP4 32
INDIRP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 36
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
CNSTF4 1056964608
ADDRLP4 36
INDIRP4
INDIRF4
MULF4
ASGNF4
line 127
;125:
;126:		// check for stop
;127:		if ( ((trace->plane.normal[2] > 0.7&&g_gravity.value>0) || (trace->plane.normal[2]<-0.7&&g_gravity.value<0)) && ((ent->s.pos.trDelta[2]<40&&g_gravity.value>0)||(ent->s.pos.trDelta[2]>-40&&g_gravity.value<0)) ) //this can happen even on very slightly sloped walls, so changed it from > 0 to > 0.7
ADDRFP4 4
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
CNSTF4 1060320051
LEF4 $129
ADDRGP4 g_gravity+8
INDIRF4
CNSTF4 0
GTF4 $128
LABELV $129
ADDRFP4 4
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
CNSTF4 3207803699
GEF4 $122
ADDRGP4 g_gravity+8
INDIRF4
CNSTF4 0
GEF4 $122
LABELV $128
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRF4
CNSTF4 1109393408
GEF4 $131
ADDRGP4 g_gravity+8
INDIRF4
CNSTF4 0
GTF4 $130
LABELV $131
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
INDIRF4
CNSTF4 3256877056
LEF4 $122
ADDRGP4 g_gravity+8
INDIRF4
CNSTF4 0
GEF4 $122
LABELV $130
line 128
;128:		{
line 131
;129:			//G_SetOrigin( ent, trace->endpos );
;130:			//ent->nextthink = level.time + 500;
;131:			ent->s.apos.trType = TR_STATIONARY;
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 0
ASGNI4
line 132
;132:			VectorCopy( ent->r.currentAngles, ent->s.apos.trBase );
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 40
INDIRP4
CNSTI4 380
ADDP4
INDIRB
ASGNB 12
line 133
;133:			VectorCopy( trace->endpos, ent->r.currentOrigin );
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
INDIRB
ASGNB 12
line 134
;134:			VectorCopy( trace->endpos, ent->s.pos.trBase );
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
INDIRB
ASGNB 12
line 135
;135:			ent->s.pos.trTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 136
;136:			return;
ADDRGP4 $110
JUMPV
LABELV $122
line 138
;137:		}
;138:	}
LABELV $120
line 143
;139:
;140:	// NEW--It would seem that we want to set our trBase to the trace endpos
;141:	//	and set the trTime to the actual time of impact....
;142:	//	FIXME: Should we still consider adding the normal though??
;143:	VectorCopy( trace->endpos, ent->r.currentOrigin );
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDRFP4 4
INDIRP4
CNSTI4 12
ADDP4
INDIRB
ASGNB 12
line 144
;144:	ent->s.pos.trTime = hitTime;
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRLP4 20
INDIRI4
ASGNI4
line 146
;145:
;146:	VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 28
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 147
;147:	VectorCopy( trace->plane.normal, ent->pos1 );//???
ADDRFP4 0
INDIRP4
CNSTI4 548
ADDP4
ADDRFP4 4
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 148
;148:}
LABELV $110
endproc G_BounceObject 44 12
export G_RunObject
proc G_RunObject 1148 28
line 162
;149:
;150:
;151:/*
;152:================
;153:G_RunObject
;154:
;155:  TODO:  When transition to 0 grav, push away from surface you were resting on
;156:  TODO:  When free-floating in air, apply some friction to your trDelta (based on mass?)
;157:================
;158:*/
;159:extern void DoImpact( gentity_t *self, gentity_t *other, qboolean damageSelf );
;160:extern void pitch_roll_for_slope( gentity_t *forwhom, vec3_t pass_slope );
;161:void G_RunObject( gentity_t *ent ) 
;162:{
line 165
;163:	vec3_t		origin, oldOrg;
;164:	trace_t		tr;
;165:	gentity_t	*traceEnt = NULL;
ADDRLP4 1080
CNSTP4 0
ASGNP4
line 168
;166:
;167:	//FIXME: floaters need to stop floating up after a while, even if gravity stays negative?
;168:	if ( ent->s.pos.trType == TR_STATIONARY )//g_gravity.value <= 0 && 
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 0
NEI4 $134
line 169
;169:	{
line 170
;170:		ent->s.pos.trType = TR_GRAVITY;
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 5
ASGNI4
line 171
;171:		VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );
ADDRLP4 1108
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1108
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 1108
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 172
;172:		ent->s.pos.trTime = level.previousTime;//?necc?
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+36
INDIRI4
ASGNI4
line 173
;173:		if ( !g_gravity.value )
ADDRGP4 g_gravity+8
INDIRF4
CNSTF4 0
NEF4 $137
line 174
;174:		{
line 175
;175:			ent->s.pos.trDelta[2] += 100;
ADDRLP4 1112
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 1112
INDIRP4
ADDRLP4 1112
INDIRP4
INDIRF4
CNSTF4 1120403456
ADDF4
ASGNF4
line 176
;176:		}
LABELV $137
line 177
;177:	}
LABELV $134
line 179
;178:
;179:	ent->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 181
;180:
;181:	VectorCopy( ent->r.currentOrigin, oldOrg );
ADDRLP4 1096
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 183
;182:	// get current position
;183:	BG_EvaluateTrajectory( &ent->s.pos, level.time, origin );
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
ARGP4
ADDRGP4 level+32
INDIRI4
ARGI4
ADDRLP4 1084
ARGP4
ADDRGP4 BG_EvaluateTrajectory
CALLV
pop
line 185
;184:	//Get current angles?
;185:	BG_EvaluateTrajectory( &ent->s.apos, level.time, ent->r.currentAngles );
ADDRLP4 1108
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1108
INDIRP4
CNSTI4 48
ADDP4
ARGP4
ADDRGP4 level+32
INDIRI4
ARGI4
ADDRLP4 1108
INDIRP4
CNSTI4 380
ADDP4
ARGP4
ADDRGP4 BG_EvaluateTrajectory
CALLV
pop
line 187
;186:
;187:	if ( VectorCompare( ent->r.currentOrigin, origin ) )
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 1084
ARGP4
ADDRLP4 1112
ADDRGP4 VectorCompare
CALLI4
ASGNI4
ADDRLP4 1112
INDIRI4
CNSTI4 0
EQI4 $143
line 188
;188:	{//error - didn't move at all!
line 189
;189:		return;
ADDRGP4 $133
JUMPV
LABELV $143
line 193
;190:	}
;191:	// trace a line from the previous position to the current position,
;192:	// ignoring interactions with the missile owner
;193:	trap_Trace( &tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, origin, 
ADDRLP4 0
ARGP4
ADDRLP4 1120
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1120
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 1120
INDIRP4
CNSTI4 316
ADDP4
ARGP4
ADDRLP4 1120
INDIRP4
CNSTI4 328
ADDP4
ARGP4
ADDRLP4 1084
ARGP4
ADDRLP4 1120
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $146
ADDRLP4 1116
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
INDIRI4
ASGNI4
ADDRGP4 $147
JUMPV
LABELV $146
ADDRLP4 1116
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
LABELV $147
ADDRLP4 1116
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 508
ADDP4
INDIRI4
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 196
;194:		ent->parent ? ent->parent->s.number : ent->s.number, ent->clipmask );
;195:
;196:	if ( !tr.startsolid && !tr.allsolid && tr.fraction ) 
ADDRLP4 1124
CNSTI4 0
ASGNI4
ADDRLP4 0+4
INDIRI4
ADDRLP4 1124
INDIRI4
NEI4 $148
ADDRLP4 0
INDIRI4
ADDRLP4 1124
INDIRI4
NEI4 $148
ADDRLP4 0+8
INDIRF4
CNSTF4 0
EQF4 $148
line 197
;197:	{
line 198
;198:		VectorCopy( tr.endpos, ent->r.currentOrigin );
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDRLP4 0+12
INDIRB
ASGNB 12
line 199
;199:		trap_LinkEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 200
;200:	}
ADDRGP4 $149
JUMPV
LABELV $148
line 203
;201:	else
;202:	//if ( tr.startsolid ) 
;203:	{
line 204
;204:		tr.fraction = 0;
ADDRLP4 0+8
CNSTF4 0
ASGNF4
line 205
;205:	}
LABELV $149
line 207
;206:
;207:	G_MoverTouchPushTriggers( ent, oldOrg );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1096
ARGP4
ADDRGP4 G_MoverTouchPushTriggers
CALLV
pop
line 223
;208:	/*
;209:	if ( !(ent->s.eFlags & EF_TELEPORT_BIT) && !(ent->svFlags & SVF_NO_TELEPORT) )
;210:	{
;211:		G_MoverTouchTeleportTriggers( ent, oldOrg );
;212:		if ( ent->s.eFlags & EF_TELEPORT_BIT )
;213:		{//was teleported
;214:			return;
;215:		}
;216:	}
;217:	else
;218:	{
;219:		ent->s.eFlags &= ~EF_TELEPORT_BIT;
;220:	}
;221:	*/
;222:
;223:	if ( tr.fraction == 1 ) 
ADDRLP4 0+8
INDIRF4
CNSTF4 1065353216
NEF4 $154
line 224
;224:	{
line 225
;225:		if ( g_gravity.value <= 0 )
ADDRGP4 g_gravity+8
INDIRF4
CNSTF4 0
GTF4 $157
line 226
;226:		{
line 227
;227:			if ( ent->s.apos.trType == TR_STATIONARY )
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
CNSTI4 0
NEI4 $160
line 228
;228:			{
line 229
;229:				VectorCopy( ent->r.currentAngles, ent->s.apos.trBase );
ADDRLP4 1128
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1128
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 1128
INDIRP4
CNSTI4 380
ADDP4
INDIRB
ASGNB 12
line 230
;230:				ent->s.apos.trType = TR_LINEAR;
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 2
ASGNI4
line 231
;231:				ent->s.apos.trDelta[1] = flrand( -300, 300 );
CNSTF4 3281387520
ARGF4
CNSTF4 1133903872
ARGF4
ADDRLP4 1132
ADDRGP4 flrand
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 76
ADDP4
ADDRLP4 1132
INDIRF4
ASGNF4
line 232
;232:				ent->s.apos.trDelta[0] = flrand( -10, 10 );
CNSTF4 3240099840
ARGF4
CNSTF4 1092616192
ARGF4
ADDRLP4 1136
ADDRGP4 flrand
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 72
ADDP4
ADDRLP4 1136
INDIRF4
ASGNF4
line 233
;233:				ent->s.apos.trDelta[2] = flrand( -10, 10 );
CNSTF4 3240099840
ARGF4
CNSTF4 1092616192
ARGF4
ADDRLP4 1140
ADDRGP4 flrand
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 80
ADDP4
ADDRLP4 1140
INDIRF4
ASGNF4
line 234
;234:				ent->s.apos.trTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 52
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 235
;235:			}
LABELV $160
line 236
;236:		}
LABELV $157
line 238
;237:		//friction in zero-G
;238:		if ( !g_gravity.value )
ADDRGP4 g_gravity+8
INDIRF4
CNSTF4 0
NEF4 $133
line 239
;239:		{
line 240
;240:			float friction = 0.975f;
ADDRLP4 1128
CNSTF4 1064933786
ASGNF4
line 242
;241:			//friction -= ent->mass/1000.0f;
;242:			if ( friction < 0.1 )
ADDRLP4 1128
INDIRF4
CNSTF4 1036831949
GEF4 $166
line 243
;243:			{
line 244
;244:				friction = 0.1f;
ADDRLP4 1128
CNSTF4 1036831949
ASGNF4
line 245
;245:			}
LABELV $166
line 247
;246:
;247:			VectorScale( ent->s.pos.trDelta, friction, ent->s.pos.trDelta );
ADDRLP4 1132
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 1132
INDIRP4
ADDRLP4 1132
INDIRP4
INDIRF4
ADDRLP4 1128
INDIRF4
MULF4
ASGNF4
ADDRLP4 1136
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 1136
INDIRP4
ADDRLP4 1136
INDIRP4
INDIRF4
ADDRLP4 1128
INDIRF4
MULF4
ASGNF4
ADDRLP4 1140
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 1140
INDIRP4
ADDRLP4 1140
INDIRP4
INDIRF4
ADDRLP4 1128
INDIRF4
MULF4
ASGNF4
line 248
;248:			VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );
ADDRLP4 1144
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1144
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 1144
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 249
;249:			ent->s.pos.trTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 250
;250:		}
line 251
;251:		return;
ADDRGP4 $133
JUMPV
LABELV $154
line 257
;252:	}
;253:
;254:	//hit something
;255:
;256:	//Do impact damage
;257:	traceEnt = &g_entities[tr.entityNum];
ADDRLP4 1080
CNSTI4 828
ADDRLP4 0+52
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 258
;258:	if ( tr.fraction || (traceEnt && traceEnt->takedamage) )
ADDRLP4 0+8
INDIRF4
CNSTF4 0
NEF4 $173
ADDRLP4 1080
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $170
ADDRLP4 1080
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
CNSTI4 0
EQI4 $170
LABELV $173
line 259
;259:	{
line 260
;260:		if ( !VectorCompare( ent->r.currentOrigin, oldOrg ) )
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 1096
ARGP4
ADDRLP4 1132
ADDRGP4 VectorCompare
CALLI4
ASGNI4
ADDRLP4 1132
INDIRI4
CNSTI4 0
NEI4 $174
line 261
;261:		{//moved and impacted
line 262
;262:			if ( (traceEnt && traceEnt->takedamage) )
ADDRLP4 1080
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $176
ADDRLP4 1080
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
CNSTI4 0
EQI4 $176
line 263
;263:			{//hurt someone
line 265
;264://				G_Sound( ent, G_SoundIndex( "sound/movers/objects/objectHurt.wav" ) );
;265:			}
LABELV $176
line 267
;266://			G_Sound( ent, G_SoundIndex( "sound/movers/objects/objectHit.wav" ) );
;267:		}
LABELV $174
line 268
;268:		DoImpact( ent, traceEnt, qtrue );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1080
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 DoImpact
CALLV
pop
line 269
;269:	}
LABELV $170
line 271
;270:
;271:	if ( !ent || (ent->takedamage&&ent->health <= 0) )
ADDRLP4 1132
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1132
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $180
ADDRLP4 1136
CNSTI4 0
ASGNI4
ADDRLP4 1132
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
ADDRLP4 1136
INDIRI4
EQI4 $178
ADDRLP4 1132
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
ADDRLP4 1136
INDIRI4
GTI4 $178
LABELV $180
line 272
;272:	{//been destroyed by impact
line 275
;273:		//chunks?
;274://		G_Sound( ent, G_SoundIndex( "sound/movers/objects/objectBreak.wav" ) );
;275:		return;
ADDRGP4 $133
JUMPV
LABELV $178
line 279
;276:	}
;277:
;278:	//do impact physics
;279:	if ( ent->s.pos.trType == TR_GRAVITY )//tr.fraction < 1.0 && 
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 5
NEI4 $181
line 280
;280:	{//FIXME: only do this if no trDelta
line 281
;281:		if ( g_gravity.value <= 0 || tr.plane.normal[2] < 0.7 )
ADDRGP4 g_gravity+8
INDIRF4
CNSTF4 0
LEF4 $188
ADDRLP4 0+24+8
INDIRF4
CNSTF4 1060320051
GEF4 $183
LABELV $188
line 282
;282:		{
line 283
;283:			if ( ent->s.eFlags&(EF_BOUNCE|EF_BOUNCE_HALF) )
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 48
BANDI4
CNSTI4 0
EQI4 $182
line 284
;284:			{
line 285
;285:				if ( tr.fraction <= 0.0f )
ADDRLP4 0+8
INDIRF4
CNSTF4 0
GTF4 $191
line 286
;286:				{
line 287
;287:					VectorCopy( tr.endpos, ent->r.currentOrigin );
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDRLP4 0+12
INDIRB
ASGNB 12
line 288
;288:					VectorCopy( tr.endpos, ent->s.pos.trBase );
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 0+12
INDIRB
ASGNB 12
line 289
;289:					VectorClear( ent->s.pos.trDelta );
ADDRLP4 1140
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1144
CNSTF4 0
ASGNF4
ADDRLP4 1140
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 1144
INDIRF4
ASGNF4
ADDRLP4 1140
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 1144
INDIRF4
ASGNF4
ADDRLP4 1140
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 1144
INDIRF4
ASGNF4
line 290
;290:					ent->s.pos.trTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 291
;291:				}
ADDRGP4 $182
JUMPV
LABELV $191
line 293
;292:				else
;293:				{
line 294
;294:					G_BounceObject( ent, &tr );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_BounceObject
CALLV
pop
line 295
;295:				}
line 296
;296:			}
line 298
;297:			else
;298:			{//slide down?
line 300
;299:				//FIXME: slide off the slope
;300:			}
line 301
;301:		}
ADDRGP4 $182
JUMPV
LABELV $183
line 303
;302:		else
;303:		{
line 304
;304:			ent->s.apos.trType = TR_STATIONARY;
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 0
ASGNI4
line 305
;305:			pitch_roll_for_slope( ent, tr.plane.normal );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+24
ARGP4
ADDRGP4 pitch_roll_for_slope
CALLV
pop
line 308
;306:			//ent->r.currentAngles[0] = 0;//FIXME: match to slope
;307:			//ent->r.currentAngles[2] = 0;//FIXME: match to slope
;308:			VectorCopy( ent->r.currentAngles, ent->s.apos.trBase );
ADDRLP4 1140
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1140
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 1140
INDIRP4
CNSTI4 380
ADDP4
INDIRB
ASGNB 12
line 312
;309:			//okay, we hit the floor, might as well stop or prediction will
;310:			//make us go through the floor!
;311:			//FIXME: this means we can't fall if something is pulled out from under us...
;312:			G_StopObjectMoving( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_StopObjectMoving
CALLV
pop
line 313
;313:		}
line 314
;314:	}
ADDRGP4 $182
JUMPV
LABELV $181
line 315
;315:	else if (ent->s.weapon != WP_SABER)
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 2
EQI4 $198
line 316
;316:	{
line 317
;317:		ent->s.apos.trType = TR_STATIONARY;
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 0
ASGNI4
line 318
;318:		pitch_roll_for_slope( ent, tr.plane.normal );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0+24
ARGP4
ADDRGP4 pitch_roll_for_slope
CALLV
pop
line 321
;319:		//ent->r.currentAngles[0] = 0;//FIXME: match to slope
;320:		//ent->r.currentAngles[2] = 0;//FIXME: match to slope
;321:		VectorCopy( ent->r.currentAngles, ent->s.apos.trBase );
ADDRLP4 1140
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1140
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 1140
INDIRP4
CNSTI4 380
ADDP4
INDIRB
ASGNB 12
line 322
;322:	}
LABELV $198
LABELV $182
line 325
;323:
;324:	//call touch func
;325:	ent->touch( ent, &g_entities[tr.entityNum], &tr );
ADDRLP4 1140
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1140
INDIRP4
ARGP4
CNSTI4 828
ADDRLP4 0+52
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 1140
INDIRP4
CNSTI4 648
ADDP4
INDIRP4
CALLV
pop
line 326
;326:}
LABELV $133
endproc G_RunObject 1148 28
export G_StopObjectMoving
proc G_StopObjectMoving 16 0
line 330
;327:
;328:
;329:void G_StopObjectMoving( gentity_t *object )
;330:{
line 331
;331:	object->s.pos.trType = TR_STATIONARY;
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 332
;332:	VectorCopy( object->r.currentOrigin, object->s.origin );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 333
;333:	VectorCopy( object->r.currentOrigin, object->s.pos.trBase );
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
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 334
;334:	VectorClear( object->s.pos.trDelta );
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
CNSTF4 0
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
line 343
;335:
;336:	/*
;337:	//Stop spinning
;338:	VectorClear( self->s.apos.trDelta );
;339:	vectoangles(trace->plane.normal, self->s.angles);
;340:	VectorCopy(self->s.angles, self->r.currentAngles );
;341:	VectorCopy(self->s.angles, self->s.apos.trBase);
;342:	*/
;343:}
LABELV $202
endproc G_StopObjectMoving 16 0
export G_StartObjectMoving
proc G_StartObjectMoving 4 4
line 346
;344:
;345:void G_StartObjectMoving( gentity_t *object, vec3_t dir, float speed, trType_t trType )
;346:{
line 347
;347:	VectorNormalize (dir);
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 350
;348:
;349:	//object->s.eType = ET_GENERAL;
;350:	object->s.pos.trType = trType;
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
ADDRFP4 12
INDIRI4
ASGNI4
line 351
;351:	VectorCopy( object->r.currentOrigin, object->s.pos.trBase );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 352
;352:	VectorScale(dir, speed, object->s.pos.trDelta );
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRFP4 4
INDIRP4
INDIRF4
ADDRFP4 8
INDIRF4
MULF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRFP4 8
INDIRF4
MULF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRFP4 8
INDIRF4
MULF4
ASGNF4
line 353
;353:	object->s.pos.trTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 364
;354:
;355:	/*
;356:	//FIXME: incorporate spin?
;357:	vectoangles(dir, object->s.angles);
;358:	VectorCopy(object->s.angles, object->s.apos.trBase);
;359:	VectorSet(object->s.apos.trDelta, 300, 0, 0 );
;360:	object->s.apos.trTime = level.time;
;361:	*/
;362:
;363:	//FIXME: make these objects go through G_RunObject automatically, like missiles do
;364:	if ( object->think == NULL )
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $205
line 365
;365:	{
line 366
;366:		object->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 367
;367:		object->think = G_RunObject;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 G_RunObject
ASGNP4
line 368
;368:	}
LABELV $205
line 370
;369:	else
;370:	{//You're responsible for calling RunObject
line 371
;371:	}
LABELV $206
line 372
;372:}
LABELV $203
endproc G_StartObjectMoving 4 4
import DoImpact
import G_MoverTouchPushTriggers
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
