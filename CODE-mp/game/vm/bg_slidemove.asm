export PM_SlideMove
code
proc PM_SlideMove 1332 28
file "../bg_slidemove.c"
line 25
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:// bg_slidemove.c -- part of bg_pmove functionality
;4:
;5:#include "q_shared.h"
;6:#include "bg_public.h"
;7:#include "bg_local.h"
;8:
;9:/*
;10:
;11:input: origin, velocity, bounds, groundPlane, trace function
;12:
;13:output: origin, velocity, impacts, stairup boolean
;14:
;15:*/
;16:
;17:/*
;18:==================
;19:PM_SlideMove
;20:
;21:Returns qtrue if the velocity was clipped in some way
;22:==================
;23:*/
;24:#define	MAX_CLIP_PLANES	5
;25:qboolean	PM_SlideMove( qboolean gravity ) {
line 41
;26:	int			bumpcount, numbumps;
;27:	vec3_t		dir;
;28:	float		d;
;29:	int			numplanes;
;30:	vec3_t		planes[MAX_CLIP_PLANES];
;31:	vec3_t		primal_velocity;
;32:	vec3_t		clipVelocity;
;33:	int			i, j, k;
;34:	trace_t	trace;
;35:	vec3_t		end;
;36:	float		time_left;
;37:	float		into;
;38:	vec3_t		endVelocity;
;39:	vec3_t		endClipVelocity;
;40:	
;41:	numbumps = 4;
ADDRLP4 1232
CNSTI4 4
ASGNI4
line 43
;42:
;43:	VectorCopy (pm->ps->velocity, primal_velocity);
ADDRLP4 1236
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
INDIRB
ASGNB 12
line 45
;44:
;45:	if ( gravity ) {
ADDRFP4 0
INDIRI4
CNSTI4 0
EQI4 $66
line 46
;46:		VectorCopy( pm->ps->velocity, endVelocity );
ADDRLP4 116
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
INDIRB
ASGNB 12
line 47
;47:		endVelocity[2] -= pm->ps->gravity * pml.frametime;
ADDRLP4 116+8
ADDRLP4 116+8
INDIRF4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 56
ADDP4
INDIRI4
CVIF4 4
ADDRGP4 pml+36
INDIRF4
MULF4
SUBF4
ASGNF4
line 48
;48:		pm->ps->velocity[2] = ( pm->ps->velocity[2] + endVelocity[2] ) * 0.5;
ADDRLP4 1248
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 1248
INDIRP4
CNSTF4 1056964608
ADDRLP4 1248
INDIRP4
INDIRF4
ADDRLP4 116+8
INDIRF4
ADDF4
MULF4
ASGNF4
line 49
;49:		primal_velocity[2] = endVelocity[2];
ADDRLP4 1236+8
ADDRLP4 116+8
INDIRF4
ASGNF4
line 50
;50:		if ( pml.groundPlane ) {
ADDRGP4 pml+48
INDIRI4
CNSTI4 0
EQI4 $73
line 52
;51:			// slide along the ground plane
;52:			PM_ClipVelocity (pm->ps->velocity, pml.groundTrace.plane.normal, 
ADDRLP4 1252
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 1252
INDIRP4
ARGP4
ADDRGP4 pml+52+24
ARGP4
ADDRLP4 1252
INDIRP4
ARGP4
CNSTF4 1065361605
ARGF4
ADDRGP4 PM_ClipVelocity
CALLV
pop
line 54
;53:				pm->ps->velocity, OVERCLIP );
;54:		}
LABELV $73
line 55
;55:	}
LABELV $66
line 57
;56:
;57:	time_left = pml.frametime;
ADDRLP4 1212
ADDRGP4 pml+36
INDIRF4
ASGNF4
line 60
;58:
;59:	// never turn against the ground plane
;60:	if ( pml.groundPlane ) {
ADDRGP4 pml+48
INDIRI4
CNSTI4 0
EQI4 $79
line 61
;61:		numplanes = 1;
ADDRLP4 96
CNSTI4 1
ASGNI4
line 62
;62:		VectorCopy( pml.groundTrace.plane.normal, planes[0] );
ADDRLP4 4
ADDRGP4 pml+52+24
INDIRB
ASGNB 12
line 63
;63:	} else {
ADDRGP4 $80
JUMPV
LABELV $79
line 64
;64:		numplanes = 0;
ADDRLP4 96
CNSTI4 0
ASGNI4
line 65
;65:	}
LABELV $80
line 68
;66:
;67:	// never turn against original velocity
;68:	VectorNormalize2( pm->ps->velocity, planes[numplanes] );
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ARGP4
CNSTI4 12
ADDRLP4 96
INDIRI4
MULI4
ADDRLP4 4
ADDP4
ARGP4
ADDRGP4 VectorNormalize2
CALLF4
pop
line 69
;69:	numplanes++;
ADDRLP4 96
ADDRLP4 96
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 71
;70:
;71:	for ( bumpcount=0 ; bumpcount < numbumps ; bumpcount++ ) {
ADDRLP4 1228
CNSTI4 0
ASGNI4
ADDRGP4 $87
JUMPV
LABELV $84
line 74
;72:
;73:		// calculate position we are trying to move to
;74:		VectorMA( pm->ps->origin, time_left, pm->ps->velocity, end );
ADDRLP4 1248
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 1252
ADDRLP4 1248
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 1216
ADDRLP4 1252
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 1252
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDRLP4 1212
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 1260
ADDRLP4 1248
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 1216+4
ADDRLP4 1260
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 1260
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
ADDRLP4 1212
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 1264
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 1216+8
ADDRLP4 1264
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 1264
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
ADDRLP4 1212
INDIRF4
MULF4
ADDF4
ASGNF4
line 77
;75:
;76:		// see if we can make it there
;77:		pm->trace ( &trace, pm->ps->origin, pm->mins, pm->maxs, end, pm->ps->clientNum, pm->tracemask);
ADDRLP4 128
ARGP4
ADDRLP4 1268
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 1272
ADDRLP4 1268
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 1272
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 1268
INDIRP4
CNSTI4 188
ADDP4
ARGP4
ADDRLP4 1268
INDIRP4
CNSTI4 200
ADDP4
ARGP4
ADDRLP4 1216
ARGP4
ADDRLP4 1272
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ARGI4
ADDRLP4 1268
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ARGI4
ADDRLP4 1268
INDIRP4
CNSTI4 240
ADDP4
INDIRP4
CALLV
pop
line 79
;78:
;79:		if (trace.allsolid) {
ADDRLP4 128
INDIRI4
CNSTI4 0
EQI4 $90
line 81
;80:			// entity is completely trapped in another solid
;81:			pm->ps->velocity[2] = 0;	// don't build up falling damage, but allow sideways acceleration
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 0
ASGNF4
line 82
;82:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $65
JUMPV
LABELV $90
line 85
;83:		}
;84:
;85:		if (trace.fraction > 0) {
ADDRLP4 128+8
INDIRF4
CNSTF4 0
LEF4 $92
line 87
;86:			// actually covered some distance
;87:			VectorCopy (trace.endpos, pm->ps->origin);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 128+12
INDIRB
ASGNB 12
line 88
;88:		}
LABELV $92
line 90
;89:
;90:		if (trace.fraction == 1) {
ADDRLP4 128+8
INDIRF4
CNSTF4 1065353216
NEF4 $96
line 91
;91:			 break;		// moved the entire distance
ADDRGP4 $86
JUMPV
LABELV $96
line 95
;92:		}
;93:
;94:		// save entity for contact
;95:		PM_AddTouchEnt( trace.entityNum );
ADDRLP4 128+52
INDIRI4
ARGI4
ADDRGP4 PM_AddTouchEnt
CALLV
pop
line 97
;96:
;97:		time_left -= time_left * trace.fraction;
ADDRLP4 1212
ADDRLP4 1212
INDIRF4
ADDRLP4 1212
INDIRF4
ADDRLP4 128+8
INDIRF4
MULF4
SUBF4
ASGNF4
line 99
;98:
;99:		if (numplanes >= MAX_CLIP_PLANES) {
ADDRLP4 96
INDIRI4
CNSTI4 5
LTI4 $101
line 101
;100:			// this shouldn't really happen
;101:			VectorClear( pm->ps->velocity );
ADDRLP4 1280
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 1284
CNSTF4 0
ASGNF4
ADDRLP4 1280
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 1284
INDIRF4
ASGNF4
ADDRLP4 1280
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 1284
INDIRF4
ASGNF4
ADDRLP4 1280
INDIRP4
CNSTI4 32
ADDP4
ADDRLP4 1284
INDIRF4
ASGNF4
line 102
;102:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $65
JUMPV
LABELV $101
line 110
;103:		}
;104:
;105:		//
;106:		// if this is the same plane we hit before, nudge velocity
;107:		// out along it, which fixes some epsilon issues with
;108:		// non-axial planes
;109:		//
;110:		for ( i = 0 ; i < numplanes ; i++ ) {
ADDRLP4 80
CNSTI4 0
ASGNI4
ADDRGP4 $106
JUMPV
LABELV $103
line 111
;111:			if ( DotProduct( trace.plane.normal, planes[i] ) > 0.99 ) {
ADDRLP4 1280
CNSTI4 12
ADDRLP4 80
INDIRI4
MULI4
ASGNI4
ADDRLP4 128+24
INDIRF4
ADDRLP4 1280
INDIRI4
ADDRLP4 4
ADDP4
INDIRF4
MULF4
ADDRLP4 128+24+4
INDIRF4
ADDRLP4 1280
INDIRI4
ADDRLP4 4+4
ADDP4
INDIRF4
MULF4
ADDF4
ADDRLP4 128+24+8
INDIRF4
ADDRLP4 1280
INDIRI4
ADDRLP4 4+8
ADDP4
INDIRF4
MULF4
ADDF4
CNSTF4 1065185444
LEF4 $107
line 112
;112:				VectorAdd( trace.plane.normal, pm->ps->velocity, pm->ps->velocity );
ADDRLP4 1284
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 1284
INDIRP4
ADDRLP4 128+24
INDIRF4
ADDRLP4 1284
INDIRP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 1288
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 1288
INDIRP4
ADDRLP4 128+24+4
INDIRF4
ADDRLP4 1288
INDIRP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 1292
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 1292
INDIRP4
ADDRLP4 128+24+8
INDIRF4
ADDRLP4 1292
INDIRP4
INDIRF4
ADDF4
ASGNF4
line 113
;113:				break;
ADDRGP4 $105
JUMPV
LABELV $107
line 115
;114:			}
;115:		}
LABELV $104
line 110
ADDRLP4 80
ADDRLP4 80
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $106
ADDRLP4 80
INDIRI4
ADDRLP4 96
INDIRI4
LTI4 $103
LABELV $105
line 116
;116:		if ( i < numplanes ) {
ADDRLP4 80
INDIRI4
ADDRLP4 96
INDIRI4
GEI4 $121
line 117
;117:			continue;
ADDRGP4 $85
JUMPV
LABELV $121
line 119
;118:		}
;119:		VectorCopy (trace.plane.normal, planes[numplanes]);
CNSTI4 12
ADDRLP4 96
INDIRI4
MULI4
ADDRLP4 4
ADDP4
ADDRLP4 128+24
INDIRB
ASGNB 12
line 120
;120:		numplanes++;
ADDRLP4 96
ADDRLP4 96
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 127
;121:
;122:		//
;123:		// modify velocity so it parallels all of the clip planes
;124:		//
;125:
;126:		// find a plane that it enters
;127:		for ( i = 0 ; i < numplanes ; i++ ) {
ADDRLP4 80
CNSTI4 0
ASGNI4
ADDRGP4 $127
JUMPV
LABELV $124
line 128
;128:			into = DotProduct( pm->ps->velocity, planes[i] );
ADDRLP4 1280
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 1284
CNSTI4 12
ADDRLP4 80
INDIRI4
MULI4
ASGNI4
ADDRLP4 1208
ADDRLP4 1280
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDRLP4 1284
INDIRI4
ADDRLP4 4
ADDP4
INDIRF4
MULF4
ADDRLP4 1280
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
ADDRLP4 1284
INDIRI4
ADDRLP4 4+4
ADDP4
INDIRF4
MULF4
ADDF4
ADDRLP4 1280
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
ADDRLP4 1284
INDIRI4
ADDRLP4 4+8
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 129
;129:			if ( into >= 0.1 ) {
ADDRLP4 1208
INDIRF4
CNSTF4 1036831949
LTF4 $130
line 130
;130:				continue;		// move doesn't interact with the plane
ADDRGP4 $125
JUMPV
LABELV $130
line 134
;131:			}
;132:
;133:			// see how hard we are hitting things
;134:			if ( -into > pml.impactSpeed ) {
ADDRLP4 1208
INDIRF4
NEGF4
ADDRGP4 pml+1132
INDIRF4
LEF4 $132
line 135
;135:				pml.impactSpeed = -into;
ADDRGP4 pml+1132
ADDRLP4 1208
INDIRF4
NEGF4
ASGNF4
line 136
;136:			}
LABELV $132
line 139
;137:
;138:			// slide along the plane
;139:			PM_ClipVelocity (pm->ps->velocity, planes[i], clipVelocity, OVERCLIP );
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ARGP4
CNSTI4 12
ADDRLP4 80
INDIRI4
MULI4
ADDRLP4 4
ADDP4
ARGP4
ADDRLP4 64
ARGP4
CNSTF4 1065361605
ARGF4
ADDRGP4 PM_ClipVelocity
CALLV
pop
line 142
;140:
;141:			// slide along the plane
;142:			PM_ClipVelocity (endVelocity, planes[i], endClipVelocity, OVERCLIP );
ADDRLP4 116
ARGP4
CNSTI4 12
ADDRLP4 80
INDIRI4
MULI4
ADDRLP4 4
ADDP4
ARGP4
ADDRLP4 104
ARGP4
CNSTF4 1065361605
ARGF4
ADDRGP4 PM_ClipVelocity
CALLV
pop
line 145
;143:
;144:			// see if there is a second plane that the new move enters
;145:			for ( j = 0 ; j < numplanes ; j++ ) {
ADDRLP4 76
CNSTI4 0
ASGNI4
ADDRGP4 $139
JUMPV
LABELV $136
line 146
;146:				if ( j == i ) {
ADDRLP4 76
INDIRI4
ADDRLP4 80
INDIRI4
NEI4 $140
line 147
;147:					continue;
ADDRGP4 $137
JUMPV
LABELV $140
line 149
;148:				}
;149:				if ( DotProduct( clipVelocity, planes[j] ) >= 0.1 ) {
ADDRLP4 1288
CNSTI4 12
ADDRLP4 76
INDIRI4
MULI4
ASGNI4
ADDRLP4 64
INDIRF4
ADDRLP4 1288
INDIRI4
ADDRLP4 4
ADDP4
INDIRF4
MULF4
ADDRLP4 64+4
INDIRF4
ADDRLP4 1288
INDIRI4
ADDRLP4 4+4
ADDP4
INDIRF4
MULF4
ADDF4
ADDRLP4 64+8
INDIRF4
ADDRLP4 1288
INDIRI4
ADDRLP4 4+8
ADDP4
INDIRF4
MULF4
ADDF4
CNSTF4 1036831949
LTF4 $142
line 150
;150:					continue;		// move doesn't interact with the plane
ADDRGP4 $137
JUMPV
LABELV $142
line 154
;151:				}
;152:
;153:				// try clipping the move to the plane
;154:				PM_ClipVelocity( clipVelocity, planes[j], clipVelocity, OVERCLIP );
ADDRLP4 64
ARGP4
CNSTI4 12
ADDRLP4 76
INDIRI4
MULI4
ADDRLP4 4
ADDP4
ARGP4
ADDRLP4 64
ARGP4
CNSTF4 1065361605
ARGF4
ADDRGP4 PM_ClipVelocity
CALLV
pop
line 155
;155:				PM_ClipVelocity( endClipVelocity, planes[j], endClipVelocity, OVERCLIP );
ADDRLP4 104
ARGP4
CNSTI4 12
ADDRLP4 76
INDIRI4
MULI4
ADDRLP4 4
ADDP4
ARGP4
ADDRLP4 104
ARGP4
CNSTF4 1065361605
ARGF4
ADDRGP4 PM_ClipVelocity
CALLV
pop
line 158
;156:
;157:				// see if it goes back into the first clip plane
;158:				if ( DotProduct( clipVelocity, planes[i] ) >= 0 ) {
ADDRLP4 1292
CNSTI4 12
ADDRLP4 80
INDIRI4
MULI4
ASGNI4
ADDRLP4 64
INDIRF4
ADDRLP4 1292
INDIRI4
ADDRLP4 4
ADDP4
INDIRF4
MULF4
ADDRLP4 64+4
INDIRF4
ADDRLP4 1292
INDIRI4
ADDRLP4 4+4
ADDP4
INDIRF4
MULF4
ADDF4
ADDRLP4 64+8
INDIRF4
ADDRLP4 1292
INDIRI4
ADDRLP4 4+8
ADDP4
INDIRF4
MULF4
ADDF4
CNSTF4 0
LTF4 $148
line 159
;159:					continue;
ADDRGP4 $137
JUMPV
LABELV $148
line 163
;160:				}
;161:
;162:				// slide the original velocity along the crease
;163:				CrossProduct (planes[i], planes[j], dir);
ADDRLP4 1296
CNSTI4 12
ASGNI4
ADDRLP4 1296
INDIRI4
ADDRLP4 80
INDIRI4
MULI4
ADDRLP4 4
ADDP4
ARGP4
ADDRLP4 1296
INDIRI4
ADDRLP4 76
INDIRI4
MULI4
ADDRLP4 4
ADDP4
ARGP4
ADDRLP4 84
ARGP4
ADDRGP4 CrossProduct
CALLV
pop
line 164
;164:				VectorNormalize( dir );
ADDRLP4 84
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 165
;165:				d = DotProduct( dir, pm->ps->velocity );
ADDRLP4 1300
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 100
ADDRLP4 84
INDIRF4
ADDRLP4 1300
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
MULF4
ADDRLP4 84+4
INDIRF4
ADDRLP4 1300
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
MULF4
ADDF4
ADDRLP4 84+8
INDIRF4
ADDRLP4 1300
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 166
;166:				VectorScale( dir, d, clipVelocity );
ADDRLP4 64
ADDRLP4 84
INDIRF4
ADDRLP4 100
INDIRF4
MULF4
ASGNF4
ADDRLP4 64+4
ADDRLP4 84+4
INDIRF4
ADDRLP4 100
INDIRF4
MULF4
ASGNF4
ADDRLP4 64+8
ADDRLP4 84+8
INDIRF4
ADDRLP4 100
INDIRF4
MULF4
ASGNF4
line 168
;167:
;168:				CrossProduct (planes[i], planes[j], dir);
ADDRLP4 1308
CNSTI4 12
ASGNI4
ADDRLP4 1308
INDIRI4
ADDRLP4 80
INDIRI4
MULI4
ADDRLP4 4
ADDP4
ARGP4
ADDRLP4 1308
INDIRI4
ADDRLP4 76
INDIRI4
MULI4
ADDRLP4 4
ADDP4
ARGP4
ADDRLP4 84
ARGP4
ADDRGP4 CrossProduct
CALLV
pop
line 169
;169:				VectorNormalize( dir );
ADDRLP4 84
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 170
;170:				d = DotProduct( dir, endVelocity );
ADDRLP4 100
ADDRLP4 84
INDIRF4
ADDRLP4 116
INDIRF4
MULF4
ADDRLP4 84+4
INDIRF4
ADDRLP4 116+4
INDIRF4
MULF4
ADDF4
ADDRLP4 84+8
INDIRF4
ADDRLP4 116+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 171
;171:				VectorScale( dir, d, endClipVelocity );
ADDRLP4 104
ADDRLP4 84
INDIRF4
ADDRLP4 100
INDIRF4
MULF4
ASGNF4
ADDRLP4 104+4
ADDRLP4 84+4
INDIRF4
ADDRLP4 100
INDIRF4
MULF4
ASGNF4
ADDRLP4 104+8
ADDRLP4 84+8
INDIRF4
ADDRLP4 100
INDIRF4
MULF4
ASGNF4
line 174
;172:
;173:				// see if there is a third plane the the new move enters
;174:				for ( k = 0 ; k < numplanes ; k++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $171
JUMPV
LABELV $168
line 175
;175:					if ( k == i || k == j ) {
ADDRLP4 0
INDIRI4
ADDRLP4 80
INDIRI4
EQI4 $174
ADDRLP4 0
INDIRI4
ADDRLP4 76
INDIRI4
NEI4 $172
LABELV $174
line 176
;176:						continue;
ADDRGP4 $169
JUMPV
LABELV $172
line 178
;177:					}
;178:					if ( DotProduct( clipVelocity, planes[k] ) >= 0.1 ) {
ADDRLP4 1320
CNSTI4 12
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 64
INDIRF4
ADDRLP4 1320
INDIRI4
ADDRLP4 4
ADDP4
INDIRF4
MULF4
ADDRLP4 64+4
INDIRF4
ADDRLP4 1320
INDIRI4
ADDRLP4 4+4
ADDP4
INDIRF4
MULF4
ADDF4
ADDRLP4 64+8
INDIRF4
ADDRLP4 1320
INDIRI4
ADDRLP4 4+8
ADDP4
INDIRF4
MULF4
ADDF4
CNSTF4 1036831949
LTF4 $175
line 179
;179:						continue;		// move doesn't interact with the plane
ADDRGP4 $169
JUMPV
LABELV $175
line 183
;180:					}
;181:
;182:					// stop dead at a tripple plane interaction
;183:					VectorClear( pm->ps->velocity );
ADDRLP4 1324
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 1328
CNSTF4 0
ASGNF4
ADDRLP4 1324
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 1328
INDIRF4
ASGNF4
ADDRLP4 1324
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 1328
INDIRF4
ASGNF4
ADDRLP4 1324
INDIRP4
CNSTI4 32
ADDP4
ADDRLP4 1328
INDIRF4
ASGNF4
line 184
;184:					return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $65
JUMPV
LABELV $169
line 174
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $171
ADDRLP4 0
INDIRI4
ADDRLP4 96
INDIRI4
LTI4 $168
line 186
;185:				}
;186:			}
LABELV $137
line 145
ADDRLP4 76
ADDRLP4 76
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $139
ADDRLP4 76
INDIRI4
ADDRLP4 96
INDIRI4
LTI4 $136
line 189
;187:
;188:			// if we have fixed all interactions, try another move
;189:			VectorCopy( clipVelocity, pm->ps->velocity );
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ADDRLP4 64
INDIRB
ASGNB 12
line 190
;190:			VectorCopy( endClipVelocity, endVelocity );
ADDRLP4 116
ADDRLP4 104
INDIRB
ASGNB 12
line 191
;191:			break;
ADDRGP4 $126
JUMPV
LABELV $125
line 127
ADDRLP4 80
ADDRLP4 80
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $127
ADDRLP4 80
INDIRI4
ADDRLP4 96
INDIRI4
LTI4 $124
LABELV $126
line 193
;192:		}
;193:	}
LABELV $85
line 71
ADDRLP4 1228
ADDRLP4 1228
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $87
ADDRLP4 1228
INDIRI4
ADDRLP4 1232
INDIRI4
LTI4 $84
LABELV $86
line 195
;194:
;195:	if ( gravity ) {
ADDRFP4 0
INDIRI4
CNSTI4 0
EQI4 $181
line 196
;196:		VectorCopy( endVelocity, pm->ps->velocity );
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ADDRLP4 116
INDIRB
ASGNB 12
line 197
;197:	}
LABELV $181
line 200
;198:
;199:	// don't change velocity if in a timer (FIXME: is this correct?)
;200:	if ( pm->ps->pm_time ) {
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 0
EQI4 $183
line 201
;201:		VectorCopy( primal_velocity, pm->ps->velocity );
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ADDRLP4 1236
INDIRB
ASGNB 12
line 202
;202:	}
LABELV $183
line 204
;203:
;204:	return ( bumpcount != 0 );
ADDRLP4 1228
INDIRI4
CNSTI4 0
EQI4 $186
ADDRLP4 1248
CNSTI4 1
ASGNI4
ADDRGP4 $187
JUMPV
LABELV $186
ADDRLP4 1248
CNSTI4 0
ASGNI4
LABELV $187
ADDRLP4 1248
INDIRI4
RETI4
LABELV $65
endproc PM_SlideMove 1332 28
export PM_StepSlideMove
proc PM_StepSlideMove 1244 28
line 213
;205:}
;206:
;207:/*
;208:==================
;209:PM_StepSlideMove
;210:
;211:==================
;212:*/
;213:void PM_StepSlideMove( qboolean gravity ) {
line 227
;214:	vec3_t		start_o, start_v;
;215:	vec3_t		down_o, down_v;
;216:	trace_t		trace;
;217://	float		down_dist, up_dist;
;218://	vec3_t		delta, delta2;
;219:	vec3_t		up, down;
;220:	vec3_t		nvel, prevel;
;221:	float		stepSize;
;222:	float		totalVel;
;223:	float		pre_z;
;224:	int			usingspeed;
;225:	int			i;
;226:
;227:	i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 229
;228:
;229:	usingspeed = 0;
ADDRLP4 1136
CNSTI4 0
ASGNI4
line 231
;230:
;231:	VectorCopy (pm->ps->origin, start_o);
ADDRLP4 1108
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 232
;232:	VectorCopy (pm->ps->velocity, start_v);
ADDRLP4 1152
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
INDIRB
ASGNB 12
line 234
;233:
;234:	if ( PM_SlideMove( gravity ) == 0 ) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 1196
ADDRGP4 PM_SlideMove
CALLI4
ASGNI4
ADDRLP4 1196
INDIRI4
CNSTI4 0
NEI4 $189
line 235
;235:		return;		// we got exactly where we wanted to go first try	
ADDRGP4 $188
JUMPV
LABELV $189
line 238
;236:	}
;237:
;238:	if (pm->ps->fd.forcePowersActive & (1 << FP_SPEED))
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 836
ADDP4
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $191
line 239
;239:	{
line 240
;240:		usingspeed = 1;
ADDRLP4 1136
CNSTI4 1
ASGNI4
line 241
;241:	}
LABELV $191
line 243
;242:
;243:	VectorCopy(start_o, down);
ADDRLP4 1120
ADDRLP4 1108
INDIRB
ASGNB 12
line 244
;244:	down[2] -= STEPSIZE;
ADDRLP4 1120+8
ADDRLP4 1120+8
INDIRF4
CNSTF4 1099956224
SUBF4
ASGNF4
line 245
;245:	pm->trace (&trace, start_o, pm->mins, pm->maxs, down, pm->ps->clientNum, pm->tracemask);
ADDRLP4 16
ARGP4
ADDRLP4 1108
ARGP4
ADDRLP4 1200
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 1200
INDIRP4
CNSTI4 188
ADDP4
ARGP4
ADDRLP4 1200
INDIRP4
CNSTI4 200
ADDP4
ARGP4
ADDRLP4 1120
ARGP4
ADDRLP4 1200
INDIRP4
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ARGI4
ADDRLP4 1200
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ARGI4
ADDRLP4 1200
INDIRP4
CNSTI4 240
ADDP4
INDIRP4
CALLV
pop
line 246
;246:	VectorSet(up, 0, 0, 1);
ADDRLP4 1204
CNSTF4 0
ASGNF4
ADDRLP4 1096
ADDRLP4 1204
INDIRF4
ASGNF4
ADDRLP4 1096+4
ADDRLP4 1204
INDIRF4
ASGNF4
ADDRLP4 1096+8
CNSTF4 1065353216
ASGNF4
line 248
;247:	// never step up when you still have up velocity
;248:	if ( pm->ps->velocity[2] > 0 && (trace.fraction == 1.0 ||
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
CNSTF4 0
LEF4 $196
ADDRLP4 16+8
INDIRF4
CNSTF4 1065353216
EQF4 $206
ADDRLP4 16+24
INDIRF4
ADDRLP4 1096
INDIRF4
MULF4
ADDRLP4 16+24+4
INDIRF4
ADDRLP4 1096+4
INDIRF4
MULF4
ADDF4
ADDRLP4 16+24+8
INDIRF4
ADDRLP4 1096+8
INDIRF4
MULF4
ADDF4
CNSTF4 1060320051
GEF4 $196
LABELV $206
line 249
;249:										DotProduct(trace.plane.normal, up) < 0.7)) {
line 251
;250:
;251:		if (!usingspeed)
ADDRLP4 1136
INDIRI4
CNSTI4 0
NEI4 $207
line 252
;252:		{
line 253
;253:			return;
ADDRGP4 $188
JUMPV
LABELV $207
line 255
;254:		}
;255:	}
LABELV $196
line 257
;256:
;257:	VectorCopy (pm->ps->origin, down_o);
ADDRLP4 1172
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 258
;258:	VectorCopy (pm->ps->velocity, down_v);
ADDRLP4 1184
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
INDIRB
ASGNB 12
line 260
;259:
;260:	VectorCopy (start_o, up);
ADDRLP4 1096
ADDRLP4 1108
INDIRB
ASGNB 12
line 261
;261:	up[2] += STEPSIZE;
ADDRLP4 1096+8
ADDRLP4 1096+8
INDIRF4
CNSTF4 1099956224
ADDF4
ASGNF4
line 264
;262:
;263:	// test the player position if they were a stepheight higher
;264:	pm->trace (&trace, start_o, pm->mins, pm->maxs, up, pm->ps->clientNum, pm->tracemask);
ADDRLP4 16
ARGP4
ADDRLP4 1108
ARGP4
ADDRLP4 1208
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 1208
INDIRP4
CNSTI4 188
ADDP4
ARGP4
ADDRLP4 1208
INDIRP4
CNSTI4 200
ADDP4
ARGP4
ADDRLP4 1096
ARGP4
ADDRLP4 1208
INDIRP4
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ARGI4
ADDRLP4 1208
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ARGI4
ADDRLP4 1208
INDIRP4
CNSTI4 240
ADDP4
INDIRP4
CALLV
pop
line 265
;265:	if ( trace.allsolid ) {
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $210
line 266
;266:		if ( pm->debugLevel ) {
ADDRGP4 pm
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
CNSTI4 0
EQI4 $212
line 267
;267:			Com_Printf("%i:bend can't step\n", c_pmove);
ADDRGP4 $214
ARGP4
ADDRGP4 c_pmove
INDIRI4
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 268
;268:		}
LABELV $212
line 269
;269:		if (!usingspeed)
ADDRLP4 1136
INDIRI4
CNSTI4 0
NEI4 $215
line 270
;270:		{
line 271
;271:			return;		// can't step up
ADDRGP4 $188
JUMPV
LABELV $215
line 273
;272:		}
;273:	}
LABELV $210
line 275
;274:
;275:	stepSize = trace.endpos[2] - start_o[2];
ADDRLP4 1164
ADDRLP4 16+12+8
INDIRF4
ADDRLP4 1108+8
INDIRF4
SUBF4
ASGNF4
line 277
;276:	// try slidemove from this position
;277:	VectorCopy (trace.endpos, pm->ps->origin);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 16+12
INDIRB
ASGNB 12
line 278
;278:	VectorCopy (start_v, pm->ps->velocity);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ADDRLP4 1152
INDIRB
ASGNB 12
line 280
;279:
;280:	VectorCopy(pm->ps->velocity, prevel);
ADDRLP4 4
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
INDIRB
ASGNB 12
line 282
;281:
;282:	pre_z = prevel[2];
ADDRLP4 1132
ADDRLP4 4+8
INDIRF4
ASGNF4
line 284
;283:
;284:	PM_SlideMove( gravity );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 PM_SlideMove
CALLI4
pop
line 286
;285:
;286:	VectorSubtract(pm->ps->velocity, prevel, prevel);
ADDRLP4 1212
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 1212
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDRLP4 4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 1212
INDIRP4
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
ADDRLP4 4+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+8
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
ADDRLP4 4+8
INDIRF4
SUBF4
ASGNF4
line 287
;287:	if (prevel[0] < 0)
ADDRLP4 4
INDIRF4
CNSTF4 0
GEF4 $226
line 288
;288:	{
line 289
;289:		prevel[0] = -prevel[0];
ADDRLP4 4
ADDRLP4 4
INDIRF4
NEGF4
ASGNF4
line 290
;290:	}
LABELV $226
line 291
;291:	if (prevel[1] < 0)
ADDRLP4 4+4
INDIRF4
CNSTF4 0
GEF4 $228
line 292
;292:	{
line 293
;293:		prevel[1] = -prevel[1];
ADDRLP4 4+4
ADDRLP4 4+4
INDIRF4
NEGF4
ASGNF4
line 294
;294:	}
LABELV $228
line 296
;295:
;296:	totalVel = prevel[0]+prevel[1];
ADDRLP4 1168
ADDRLP4 4
INDIRF4
ADDRLP4 4+4
INDIRF4
ADDF4
ASGNF4
line 298
;297:
;298:	if (pre_z > 480 && (pre_z - pm->ps->velocity[2]) >= 480 && pm->ps->fd.forceJumpZStart)
ADDRLP4 1220
CNSTF4 1139802112
ASGNF4
ADDRLP4 1132
INDIRF4
ADDRLP4 1220
INDIRF4
LEF4 $234
ADDRLP4 1224
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 1132
INDIRF4
ADDRLP4 1224
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
SUBF4
ADDRLP4 1220
INDIRF4
LTF4 $234
ADDRLP4 1224
INDIRP4
CNSTI4 1080
ADDP4
INDIRF4
CNSTF4 0
EQF4 $234
line 299
;299:	{ //smashed head on the ceiling during a force jump
line 300
;300:		pm->ps->fd.forceSpeedDoDamage = (pre_z - pm->ps->velocity[2])*0.04;
ADDRLP4 1228
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 1228
INDIRP4
CNSTI4 1128
ADDP4
CNSTF4 1025758986
ADDRLP4 1132
INDIRF4
ADDRLP4 1228
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
SUBF4
MULF4
ASGNF4
line 301
;301:		if (pm->numtouch)
ADDRGP4 pm
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CNSTI4 0
EQI4 $236
line 302
;302:		{ //do damage to the other player if we hit one
ADDRGP4 $239
JUMPV
LABELV $238
line 304
;303:			while (i < pm->numtouch)
;304:			{
line 305
;305:				if (pm->touchents[i] < MAX_CLIENTS && pm->touchents[i] != pm->ps->clientNum)
ADDRLP4 1232
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 1236
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1232
INDIRP4
CNSTI4 56
ADDP4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1236
INDIRI4
CNSTI4 32
GEI4 $241
ADDRLP4 1236
INDIRI4
ADDRLP4 1232
INDIRP4
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
EQI4 $241
line 306
;306:				{
line 307
;307:					pm->ps->fd.forceSpeedHitIndex = pm->touchents[i];
ADDRLP4 1240
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 1240
INDIRP4
INDIRP4
CNSTI4 1132
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1240
INDIRP4
CNSTI4 56
ADDP4
ADDP4
INDIRI4
ASGNI4
line 308
;308:					break;
ADDRGP4 $240
JUMPV
LABELV $241
line 311
;309:				}
;310:
;311:				i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 312
;312:			}
LABELV $239
line 303
ADDRLP4 0
INDIRI4
ADDRGP4 pm
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
LTI4 $238
LABELV $240
line 313
;313:		}
LABELV $236
line 315
;314:
;315:		i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 316
;316:	}
LABELV $234
line 318
;317:
;318:	if (usingspeed)
ADDRLP4 1136
INDIRI4
CNSTI4 0
EQI4 $243
line 319
;319:	{
line 320
;320:		if (pm->ps->fd.forceSpeedSmash > 1.3 && totalVel > 500)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1124
ADDP4
INDIRF4
CNSTF4 1067869798
LEF4 $245
ADDRLP4 1168
INDIRF4
CNSTF4 1140457472
LEF4 $245
line 321
;321:		{ //if we were going fast enough and hadn't hit a while in a while then smash into it hard
line 324
;322:		  //the difference between our velocity pre and post colide must also be greater than 600 to do damage
;323:			//Com_Printf("SMASH %f\n", pm->ps->fd.forceSpeedSmash);
;324:			VectorCopy(start_v, nvel); //then bounce the player back a bit in the opposite of the direction he was going
ADDRLP4 1140
ADDRLP4 1152
INDIRB
ASGNB 12
line 325
;325:			nvel[0] += start_o[0];
ADDRLP4 1140
ADDRLP4 1140
INDIRF4
ADDRLP4 1108
INDIRF4
ADDF4
ASGNF4
line 326
;326:			nvel[1] += start_o[1];
ADDRLP4 1140+4
ADDRLP4 1140+4
INDIRF4
ADDRLP4 1108+4
INDIRF4
ADDF4
ASGNF4
line 327
;327:			nvel[2] += start_o[2];
ADDRLP4 1140+8
ADDRLP4 1140+8
INDIRF4
ADDRLP4 1108+8
INDIRF4
ADDF4
ASGNF4
line 328
;328:			VectorSubtract(start_o, nvel, nvel);
ADDRLP4 1140
ADDRLP4 1108
INDIRF4
ADDRLP4 1140
INDIRF4
SUBF4
ASGNF4
ADDRLP4 1140+4
ADDRLP4 1108+4
INDIRF4
ADDRLP4 1140+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 1140+8
ADDRLP4 1108+8
INDIRF4
ADDRLP4 1140+8
INDIRF4
SUBF4
ASGNF4
line 329
;329:			pm->ps->velocity[0] = nvel[0]*0.1;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
CNSTF4 1036831949
ADDRLP4 1140
INDIRF4
MULF4
ASGNF4
line 330
;330:			pm->ps->velocity[1] = nvel[1]*0.1;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 36
ADDP4
CNSTF4 1036831949
ADDRLP4 1140+4
INDIRF4
MULF4
ASGNF4
line 331
;331:			pm->ps->velocity[2] = 64;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1115684864
ASGNF4
line 332
;332:			pm->ps->fd.forceSpeedDoDamage = pm->ps->fd.forceSpeedSmash*10; //do somewhere in the range of 15-25 damage, depending on speed
ADDRLP4 1228
ADDRGP4 pm
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 1228
INDIRP4
CNSTI4 1128
ADDP4
CNSTF4 1092616192
ADDRLP4 1228
INDIRP4
CNSTI4 1124
ADDP4
INDIRF4
MULF4
ASGNF4
line 333
;333:			pm->ps->fd.forceSpeedSmash = 0;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1124
ADDP4
CNSTF4 0
ASGNF4
line 335
;334:
;335:			if (pm->numtouch)
ADDRGP4 pm
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CNSTI4 0
EQI4 $188
line 336
;336:			{
ADDRGP4 $261
JUMPV
LABELV $260
line 338
;337:				while (i < pm->numtouch)
;338:				{
line 339
;339:					if (pm->touchents[i] < MAX_CLIENTS && pm->touchents[i] != pm->ps->clientNum)
ADDRLP4 1232
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 1236
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1232
INDIRP4
CNSTI4 56
ADDP4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 1236
INDIRI4
CNSTI4 32
GEI4 $263
ADDRLP4 1236
INDIRI4
ADDRLP4 1232
INDIRP4
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
EQI4 $263
line 340
;340:					{
line 341
;341:						pm->ps->fd.forceSpeedHitIndex = pm->touchents[i];
ADDRLP4 1240
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 1240
INDIRP4
INDIRP4
CNSTI4 1132
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 1240
INDIRP4
CNSTI4 56
ADDP4
ADDP4
INDIRI4
ASGNI4
line 342
;342:						break;
ADDRGP4 $188
JUMPV
LABELV $263
line 345
;343:					}
;344:
;345:					i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 346
;346:				}
LABELV $261
line 337
ADDRLP4 0
INDIRI4
ADDRGP4 pm
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
LTI4 $260
line 347
;347:			}
line 348
;348:			return;
ADDRGP4 $188
JUMPV
LABELV $245
line 351
;349:		}
;350:
;351:		pm->ps->fd.forceSpeedSmash -= 0.1;
ADDRLP4 1228
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1124
ADDP4
ASGNP4
ADDRLP4 1228
INDIRP4
ADDRLP4 1228
INDIRP4
INDIRF4
CNSTF4 1036831949
SUBF4
ASGNF4
line 354
;352:		//we hit a wall so decrease speed
;353:
;354:		if (pm->ps->fd.forceSpeedSmash < 1)
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1124
ADDP4
INDIRF4
CNSTF4 1065353216
GEF4 $265
line 355
;355:		{
line 356
;356:			pm->ps->fd.forceSpeedSmash = 1;
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 1124
ADDP4
CNSTF4 1065353216
ASGNF4
line 357
;357:		}
LABELV $265
line 358
;358:	}
LABELV $243
line 361
;359:
;360:	// push down the final amount
;361:	VectorCopy (pm->ps->origin, down);
ADDRLP4 1120
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 362
;362:	down[2] -= stepSize;
ADDRLP4 1120+8
ADDRLP4 1120+8
INDIRF4
ADDRLP4 1164
INDIRF4
SUBF4
ASGNF4
line 363
;363:	pm->trace (&trace, pm->ps->origin, pm->mins, pm->maxs, down, pm->ps->clientNum, pm->tracemask);
ADDRLP4 16
ARGP4
ADDRLP4 1228
ADDRGP4 pm
INDIRP4
ASGNP4
ADDRLP4 1232
ADDRLP4 1228
INDIRP4
INDIRP4
ASGNP4
ADDRLP4 1232
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 1228
INDIRP4
CNSTI4 188
ADDP4
ARGP4
ADDRLP4 1228
INDIRP4
CNSTI4 200
ADDP4
ARGP4
ADDRLP4 1120
ARGP4
ADDRLP4 1232
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
ARGI4
ADDRLP4 1228
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ARGI4
ADDRLP4 1228
INDIRP4
CNSTI4 240
ADDP4
INDIRP4
CALLV
pop
line 364
;364:	if ( !trace.allsolid ) {
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $268
line 365
;365:		VectorCopy (trace.endpos, pm->ps->origin);
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
ADDRLP4 16+12
INDIRB
ASGNB 12
line 366
;366:	}
LABELV $268
line 367
;367:	if ( trace.fraction < 1.0 ) {
ADDRLP4 16+8
INDIRF4
CNSTF4 1065353216
GEF4 $271
line 368
;368:		PM_ClipVelocity( pm->ps->velocity, trace.plane.normal, pm->ps->velocity, OVERCLIP );
ADDRLP4 1236
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 1236
INDIRP4
ARGP4
ADDRLP4 16+24
ARGP4
ADDRLP4 1236
INDIRP4
ARGP4
CNSTF4 1065361605
ARGF4
ADDRGP4 PM_ClipVelocity
CALLV
pop
line 369
;369:	}
LABELV $271
line 383
;370:
;371:#if 0
;372:	// if the down trace can trace back to the original position directly, don't step
;373:	pm->trace( &trace, pm->ps->origin, pm->mins, pm->maxs, start_o, pm->ps->clientNum, pm->tracemask);
;374:	if ( trace.fraction == 1.0 ) {
;375:		// use the original move
;376:		VectorCopy (down_o, pm->ps->origin);
;377:		VectorCopy (down_v, pm->ps->velocity);
;378:		if ( pm->debugLevel ) {
;379:			Com_Printf("%i:bend\n", c_pmove);
;380:		}
;381:	} else 
;382:#endif
;383:	{
line 387
;384:		// use the step move
;385:		float	delta;
;386:
;387:		delta = pm->ps->origin[2] - start_o[2];
ADDRLP4 1236
ADDRGP4 pm
INDIRP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 1108+8
INDIRF4
SUBF4
ASGNF4
line 388
;388:		if ( delta > 2 ) {
ADDRLP4 1236
INDIRF4
CNSTF4 1073741824
LEF4 $276
line 389
;389:			if ( delta < 7 ) {
ADDRLP4 1236
INDIRF4
CNSTF4 1088421888
GEF4 $278
line 390
;390:				PM_AddEvent( EV_STEP_4 );
CNSTI4 7
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 391
;391:			} else if ( delta < 11 ) {
ADDRGP4 $279
JUMPV
LABELV $278
ADDRLP4 1236
INDIRF4
CNSTF4 1093664768
GEF4 $280
line 392
;392:				PM_AddEvent( EV_STEP_8 );
CNSTI4 8
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 393
;393:			} else if ( delta < 15 ) {
ADDRGP4 $281
JUMPV
LABELV $280
ADDRLP4 1236
INDIRF4
CNSTF4 1097859072
GEF4 $282
line 394
;394:				PM_AddEvent( EV_STEP_12 );
CNSTI4 9
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 395
;395:			} else {
ADDRGP4 $283
JUMPV
LABELV $282
line 396
;396:				PM_AddEvent( EV_STEP_16 );
CNSTI4 10
ARGI4
ADDRGP4 PM_AddEvent
CALLV
pop
line 397
;397:			}
LABELV $283
LABELV $281
LABELV $279
line 398
;398:		}
LABELV $276
line 399
;399:		if ( pm->debugLevel ) {
ADDRGP4 pm
INDIRP4
CNSTI4 36
ADDP4
INDIRI4
CNSTI4 0
EQI4 $284
line 400
;400:			Com_Printf("%i:stepped\n", c_pmove);
ADDRGP4 $286
ARGP4
ADDRGP4 c_pmove
INDIRI4
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 401
;401:		}
LABELV $284
line 402
;402:	}
line 403
;403:}
LABELV $188
endproc PM_StepSlideMove 1244 28
import PM_SetSaberMove
import PM_WeaponLightsaber
import PM_SetAnim
import PM_FinishWeaponChange
import PM_BeginWeaponChange
import PM_ForceLegsAnim
import PM_ContinueLegsAnim
import PM_StartTorsoAnim
import BG_CycleInven
import PM_AddEvent
import PM_AddTouchEnt
import PM_ClipVelocity
import PM_SaberJumpAttackMove
import PM_SaberFlipOverAttackMove
import PM_SomeoneInFront
import PM_GroundDistance
import PM_GetSaberStance
import PM_AnimLength
import PM_InRollComplete
import PM_InOnGroundAnim
import PM_SpinningAnim
import PM_LandingAnim
import PM_JumpingAnim
import PM_PainAnim
import PM_InKnockDown
import PM_InSaberAnim
import PM_SaberInStart
import PM_SaberInReflect
import PM_SaberInParry
import trap_FS_FCloseFile
import trap_FS_Write
import trap_FS_Read
import trap_FS_FOpenFile
import forcePowerNeeded
import c_pmove
import pm_flightfriction
import pm_waterfriction
import pm_friction
import pm_flyaccelerate
import pm_wateraccelerate
import pm_airaccelerate
import pm_accelerate
import pm_wadeScale
import pm_swimScale
import pm_duckScale
import pm_stopspeed
import pml
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
LABELV $286
char 1 37
char 1 105
char 1 58
char 1 115
char 1 116
char 1 101
char 1 112
char 1 112
char 1 101
char 1 100
char 1 10
char 1 0
align 1
LABELV $214
char 1 37
char 1 105
char 1 58
char 1 98
char 1 101
char 1 110
char 1 100
char 1 32
char 1 99
char 1 97
char 1 110
char 1 39
char 1 116
char 1 32
char 1 115
char 1 116
char 1 101
char 1 112
char 1 10
char 1 0
