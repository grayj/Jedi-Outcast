export touch_NULL
code
proc touch_NULL 0 0
file "../g_weapon.c"
line 106
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:// g_weapon.c 
;4:// perform the server side effects of a weapon firing
;5:
;6:#include "g_local.h"
;7:#include "be_aas.h"
;8:
;9:static	float	s_quadFactor;
;10:static	vec3_t	forward, right, up;
;11:static	vec3_t	muzzle;
;12:
;13:// Bryar Pistol
;14://--------
;15:#define BRYAR_PISTOL_VEL			1600
;16:#define BRYAR_PISTOL_DAMAGE			10
;17:#define BRYAR_CHARGE_UNIT			200.0f	// bryar charging gives us one more unit every 200ms--if you change this, you'll have to do the same in bg_pmove
;18:#define BRYAR_ALT_SIZE				1.0f
;19:
;20:// E11 Blaster
;21://---------
;22:#define BLASTER_SPREAD				1.6f//1.2f
;23:#define BLASTER_VELOCITY			2300
;24:#define BLASTER_DAMAGE				20
;25:
;26:// Tenloss Disruptor
;27://----------
;28:#define DISRUPTOR_MAIN_DAMAGE			30 //40
;29:#define DISRUPTOR_NPC_MAIN_DAMAGE_CUT	0.25f
;30:
;31:#define DISRUPTOR_ALT_DAMAGE			100 //125
;32:#define DISRUPTOR_NPC_ALT_DAMAGE_CUT	0.2f
;33:#define DISRUPTOR_ALT_TRACES			3		// can go through a max of 3 damageable(sp?) entities
;34:#define DISRUPTOR_CHARGE_UNIT			50.0f	// distruptor charging gives us one more unit every 50ms--if you change this, you'll have to do the same in bg_pmove
;35:
;36:// Wookiee Bowcaster
;37://----------
;38:#define	BOWCASTER_DAMAGE			50
;39:#define	BOWCASTER_VELOCITY			1300
;40:#define BOWCASTER_SPLASH_DAMAGE		0
;41:#define BOWCASTER_SPLASH_RADIUS		0
;42:#define BOWCASTER_SIZE				2
;43:
;44:#define BOWCASTER_ALT_SPREAD		5.0f
;45:#define BOWCASTER_VEL_RANGE			0.3f
;46:#define BOWCASTER_CHARGE_UNIT		200.0f	// bowcaster charging gives us one more unit every 200ms--if you change this, you'll have to do the same in bg_pmove
;47:
;48:// Heavy Repeater
;49://----------
;50:#define REPEATER_SPREAD				1.4f
;51:#define	REPEATER_DAMAGE				14
;52:#define	REPEATER_VELOCITY			1600
;53:
;54:#define REPEATER_ALT_SIZE				3	// half of bbox size
;55:#define	REPEATER_ALT_DAMAGE				60
;56:#define REPEATER_ALT_SPLASH_DAMAGE		60
;57:#define REPEATER_ALT_SPLASH_RADIUS		128
;58:#define	REPEATER_ALT_VELOCITY			1100
;59:
;60:// DEMP2
;61://----------
;62:#define	DEMP2_DAMAGE				35
;63:#define	DEMP2_VELOCITY				1800
;64:#define	DEMP2_SIZE					2		// half of bbox size
;65:
;66:#define DEMP2_ALT_DAMAGE			8 //12		// does 12, 36, 84 at each of the 3 charge levels.
;67:#define DEMP2_CHARGE_UNIT			700.0f	// demp2 charging gives us one more unit every 700ms--if you change this, you'll have to do the same in bg_weapons
;68:#define DEMP2_ALT_RANGE				4096
;69:#define DEMP2_ALT_SPLASHRADIUS		256
;70:
;71:// Golan Arms Flechette
;72://---------
;73:#define FLECHETTE_SHOTS				5
;74:#define FLECHETTE_SPREAD			4.0f
;75:#define FLECHETTE_DAMAGE			12//15
;76:#define FLECHETTE_VEL				3500
;77:#define FLECHETTE_SIZE				1
;78:#define FLECHETTE_MINE_RADIUS_CHECK	256
;79:#define FLECHETTE_ALT_DAMAGE		60
;80:#define FLECHETTE_ALT_SPLASH_DAM	60
;81:#define FLECHETTE_ALT_SPLASH_RAD	128
;82:
;83:// Personal Rocket Launcher
;84://---------
;85:#define	ROCKET_VELOCITY				900
;86:#define	ROCKET_DAMAGE				100
;87:#define	ROCKET_SPLASH_DAMAGE		100
;88:#define	ROCKET_SPLASH_RADIUS		160
;89:#define ROCKET_SIZE					3
;90:#define ROCKET_ALT_THINK_TIME		100
;91:
;92:// Stun Baton
;93://--------------
;94:#define STUN_BATON_DAMAGE			20
;95:#define STUN_BATON_ALT_DAMAGE		20
;96:#define STUN_BATON_RANGE			8
;97:
;98:
;99:extern qboolean G_BoxInBounds( vec3_t point, vec3_t mins, vec3_t maxs, vec3_t boundsMins, vec3_t boundsMaxs );
;100:
;101:static void WP_FireEmplaced( gentity_t *ent, qboolean altFire );
;102:
;103:void laserTrapStick( gentity_t *ent, vec3_t endpos, vec3_t normal );
;104:
;105:void touch_NULL( gentity_t *ent, gentity_t *other, trace_t *trace )
;106:{
line 108
;107:
;108:}
LABELV $79
endproc touch_NULL 0 0
export W_TraceSetStart
proc W_TraceSetStart 1140 28
line 115
;109:
;110:void laserTrapExplode( gentity_t *self );
;111:
;112://-----------------------------------------------------------------------------
;113:void W_TraceSetStart( gentity_t *ent, vec3_t start, vec3_t mins, vec3_t maxs )
;114://-----------------------------------------------------------------------------
;115:{
line 122
;116:	//make sure our start point isn't on the other side of a wall
;117:	trace_t	tr;
;118:	vec3_t	entMins;
;119:	vec3_t	entMaxs;
;120:	vec3_t	eyePoint;
;121:
;122:	VectorAdd( ent->r.currentOrigin, ent->r.mins, entMins );
ADDRLP4 1116
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1080
ADDRLP4 1116
INDIRP4
CNSTI4 368
ADDP4
INDIRF4
ADDRLP4 1116
INDIRP4
CNSTI4 316
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 1080+4
ADDRLP4 1116
INDIRP4
CNSTI4 372
ADDP4
INDIRF4
ADDRLP4 1116
INDIRP4
CNSTI4 320
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 1120
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1080+8
ADDRLP4 1120
INDIRP4
CNSTI4 376
ADDP4
INDIRF4
ADDRLP4 1120
INDIRP4
CNSTI4 324
ADDP4
INDIRF4
ADDF4
ASGNF4
line 123
;123:	VectorAdd( ent->r.currentOrigin, ent->r.maxs, entMaxs );
ADDRLP4 1124
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1092
ADDRLP4 1124
INDIRP4
CNSTI4 368
ADDP4
INDIRF4
ADDRLP4 1124
INDIRP4
CNSTI4 328
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 1092+4
ADDRLP4 1124
INDIRP4
CNSTI4 372
ADDP4
INDIRF4
ADDRLP4 1124
INDIRP4
CNSTI4 332
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 1128
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1092+8
ADDRLP4 1128
INDIRP4
CNSTI4 376
ADDP4
INDIRF4
ADDRLP4 1128
INDIRP4
CNSTI4 336
ADDP4
INDIRF4
ADDF4
ASGNF4
line 125
;124:
;125:	if ( G_BoxInBounds( start, mins, maxs, entMins, entMaxs ) )
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
ADDRLP4 1080
ARGP4
ADDRLP4 1092
ARGP4
ADDRLP4 1132
ADDRGP4 G_BoxInBounds
CALLI4
ASGNI4
ADDRLP4 1132
INDIRI4
CNSTI4 0
EQI4 $85
line 126
;126:	{
line 127
;127:		return;
ADDRGP4 $80
JUMPV
LABELV $85
line 130
;128:	}
;129:
;130:	if ( !ent->client )
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $87
line 131
;131:	{
line 132
;132:		return;
ADDRGP4 $80
JUMPV
LABELV $87
line 135
;133:	}
;134:
;135:	VectorCopy( ent->s.pos.trBase, eyePoint);
ADDRLP4 1104
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 136
;136:	eyePoint[2] += ent->client->ps.viewheight;
ADDRLP4 1104+8
ADDRLP4 1104+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 168
ADDP4
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 138
;137:		
;138:	trap_Trace( &tr, eyePoint, mins, maxs, start, ent->s.number, MASK_SOLID|CONTENTS_SHOTCLIP );
ADDRLP4 0
ARGP4
ADDRLP4 1104
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
CNSTI4 129
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 140
;139:
;140:	if ( tr.startsolid || tr.allsolid )
ADDRLP4 1136
CNSTI4 0
ASGNI4
ADDRLP4 0+4
INDIRI4
ADDRLP4 1136
INDIRI4
NEI4 $93
ADDRLP4 0
INDIRI4
ADDRLP4 1136
INDIRI4
EQI4 $90
LABELV $93
line 141
;141:	{
line 142
;142:		return;
ADDRGP4 $80
JUMPV
LABELV $90
line 145
;143:	}
;144:
;145:	if ( tr.fraction < 1.0f )
ADDRLP4 0+8
INDIRF4
CNSTF4 1065353216
GEF4 $94
line 146
;146:	{
line 147
;147:		VectorCopy( tr.endpos, start );
ADDRFP4 4
INDIRP4
ADDRLP4 0+12
INDIRB
ASGNB 12
line 148
;148:	}
LABELV $94
line 149
;149:}
LABELV $80
endproc W_TraceSetStart 1140 28
proc WP_FireBryarPistol 20 24
line 169
;150:
;151:
;152:/*
;153:----------------------------------------------
;154:	PLAYER WEAPONS
;155:----------------------------------------------
;156:*/
;157:
;158:/*
;159:======================================================================
;160:
;161:BRYAR PISTOL
;162:
;163:======================================================================
;164:*/
;165:
;166://----------------------------------------------
;167:static void WP_FireBryarPistol( gentity_t *ent, qboolean altFire )
;168://---------------------------------------------------------
;169:{
line 170
;170:	int damage = BRYAR_PISTOL_DAMAGE;
ADDRLP4 8
CNSTI4 10
ASGNI4
line 173
;171:	int count;
;172:
;173:	gentity_t	*missile = CreateMissile( muzzle, forward, BRYAR_PISTOL_VEL, 10000, ent, altFire );
ADDRGP4 muzzle
ARGP4
ADDRGP4 forward
ARGP4
CNSTF4 1153957888
ARGF4
CNSTI4 10000
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 CreateMissile
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ASGNP4
line 175
;174:
;175:	missile->classname = "bryar_proj";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $99
ASGNP4
line 176
;176:	missile->s.weapon = WP_BRYAR_PISTOL;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 3
ASGNI4
line 178
;177:
;178:	if ( altFire )
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $100
line 179
;179:	{
line 180
;180:		float boxSize = 0;
ADDRLP4 16
CNSTF4 0
ASGNF4
line 182
;181:
;182:		count = ( level.time - ent->client->ps.weaponChargeTime ) / BRYAR_CHARGE_UNIT;
ADDRLP4 4
ADDRGP4 level+32
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
SUBI4
CVIF4 4
CNSTF4 1128792064
DIVF4
CVFI4 4
ASGNI4
line 184
;183:
;184:		if ( count < 1 )
ADDRLP4 4
INDIRI4
CNSTI4 1
GEI4 $103
line 185
;185:		{
line 186
;186:			count = 1;
ADDRLP4 4
CNSTI4 1
ASGNI4
line 187
;187:		}
ADDRGP4 $104
JUMPV
LABELV $103
line 188
;188:		else if ( count > 5 )
ADDRLP4 4
INDIRI4
CNSTI4 5
LEI4 $105
line 189
;189:		{
line 190
;190:			count = 5;
ADDRLP4 4
CNSTI4 5
ASGNI4
line 191
;191:		}
LABELV $105
LABELV $104
line 193
;192:
;193:		if (count > 1)
ADDRLP4 4
INDIRI4
CNSTI4 1
LEI4 $107
line 194
;194:		{
line 195
;195:			damage *= (count*1.7);
ADDRLP4 8
ADDRLP4 8
INDIRI4
CVIF4 4
CNSTF4 1071225242
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
MULF4
CVFI4 4
ASGNI4
line 196
;196:		}
ADDRGP4 $108
JUMPV
LABELV $107
line 198
;197:		else
;198:		{
line 199
;199:			damage *= (count*1.5);
ADDRLP4 8
ADDRLP4 8
INDIRI4
CVIF4 4
CNSTF4 1069547520
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
MULF4
CVFI4 4
ASGNI4
line 200
;200:		}
LABELV $108
line 202
;201:
;202:		missile->s.generic1 = count; // The missile will then render according to the charge level.
ADDRLP4 0
INDIRP4
CNSTI4 292
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 205
;203://		missile->count = count;		// The single player stores the charge in count, which isn't accessible on the client
;204:
;205:		boxSize = BRYAR_ALT_SIZE*(count*0.5);
ADDRLP4 16
CNSTF4 1065353216
CNSTF4 1056964608
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
MULF4
ASGNF4
line 207
;206:
;207:		VectorSet( missile->r.maxs, boxSize, boxSize, boxSize );
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
ADDRLP4 16
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
ADDRLP4 16
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
ADDRLP4 16
INDIRF4
ASGNF4
line 208
;208:		VectorSet( missile->r.mins, -boxSize, -boxSize, -boxSize );
ADDRLP4 0
INDIRP4
CNSTI4 316
ADDP4
ADDRLP4 16
INDIRF4
NEGF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 320
ADDP4
ADDRLP4 16
INDIRF4
NEGF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 324
ADDP4
ADDRLP4 16
INDIRF4
NEGF4
ASGNF4
line 209
;209:	}
LABELV $100
line 211
;210:
;211:	missile->damage = damage;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 212
;212:	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
CNSTI4 128
ASGNI4
line 213
;213:	if (altFire)
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $109
line 214
;214:	{
line 215
;215:		missile->methodOfDeath = MOD_BRYAR_PISTOL_ALT;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 5
ASGNI4
line 216
;216:	}
ADDRGP4 $110
JUMPV
LABELV $109
line 218
;217:	else
;218:	{
line 219
;219:		missile->methodOfDeath = MOD_BRYAR_PISTOL;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 4
ASGNI4
line 220
;220:	}
LABELV $110
line 221
;221:	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 262913
ASGNI4
line 225
;222:
;223:	// we don't want it to bounce forever
;224:	// NOTENOTE These don't bounce yet.
;225:	missile->bounceCount = 8;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 8
ASGNI4
line 226
;226:}
LABELV $98
endproc WP_FireBryarPistol 20 24
export WP_FireTurretMissile
proc WP_FireTurretMissile 8 24
line 239
;227:
;228:/*
;229:======================================================================
;230:
;231:GENERIC
;232:
;233:======================================================================
;234:*/
;235:
;236://---------------------------------------------------------
;237:void WP_FireTurretMissile( gentity_t *ent, vec3_t start, vec3_t dir, qboolean altFire, int damage, int velocity, int mod, gentity_t *ignore )
;238://---------------------------------------------------------
;239:{
line 242
;240:	gentity_t *missile;
;241:
;242:	missile = CreateMissile( start, dir, velocity, 10000, ent, altFire );
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 20
INDIRI4
CVIF4 4
ARGF4
CNSTI4 10000
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 CreateMissile
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 244
;243:
;244:	missile->classname = "generic_proj";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $112
ASGNP4
line 245
;245:	missile->s.weapon = WP_TURRET;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 15
ASGNI4
line 247
;246:
;247:	missile->damage = damage;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRFP4 16
INDIRI4
ASGNI4
line 248
;248:	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
CNSTI4 128
ASGNI4
line 249
;249:	missile->methodOfDeath = mod;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
ADDRFP4 24
INDIRI4
ASGNI4
line 250
;250:	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 262913
ASGNI4
line 252
;251:
;252:	if (ignore)
ADDRFP4 28
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $113
line 253
;253:	{
line 254
;254:		missile->passThroughNum = ignore->s.number+1;
ADDRLP4 0
INDIRP4
CNSTI4 444
ADDP4
ADDRFP4 28
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 255
;255:	}
LABELV $113
line 259
;256:
;257:	// we don't want it to bounce forever
;258:	// NOTENOTE These don't bounce yet.
;259:	missile->bounceCount = 8;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 8
ASGNI4
line 260
;260:}
LABELV $111
endproc WP_FireTurretMissile 8 24
export WP_FireGenericBlasterMissile
proc WP_FireGenericBlasterMissile 8 24
line 267
;261:
;262://Currently only the seeker drone uses this, but it might be useful for other things as well.
;263:
;264://---------------------------------------------------------
;265:void WP_FireGenericBlasterMissile( gentity_t *ent, vec3_t start, vec3_t dir, qboolean altFire, int damage, int velocity, int mod )
;266://---------------------------------------------------------
;267:{
line 270
;268:	gentity_t *missile;
;269:
;270:	missile = CreateMissile( start, dir, velocity, 10000, ent, altFire );
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 20
INDIRI4
CVIF4 4
ARGF4
CNSTI4 10000
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 CreateMissile
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 272
;271:
;272:	missile->classname = "generic_proj";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $112
ASGNP4
line 273
;273:	missile->s.weapon = WP_BRYAR_PISTOL;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 3
ASGNI4
line 275
;274:
;275:	missile->damage = damage;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRFP4 16
INDIRI4
ASGNI4
line 276
;276:	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
CNSTI4 128
ASGNI4
line 277
;277:	missile->methodOfDeath = mod;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
ADDRFP4 24
INDIRI4
ASGNI4
line 278
;278:	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 262913
ASGNI4
line 282
;279:
;280:	// we don't want it to bounce forever
;281:	// NOTENOTE These don't bounce yet.
;282:	missile->bounceCount = 8;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 8
ASGNI4
line 283
;283:}
LABELV $115
endproc WP_FireGenericBlasterMissile 8 24
export WP_FireBlasterMissile
proc WP_FireBlasterMissile 16 24
line 296
;284:
;285:/*
;286:======================================================================
;287:
;288:BLASTER
;289:
;290:======================================================================
;291:*/
;292:
;293://---------------------------------------------------------
;294:void WP_FireBlasterMissile( gentity_t *ent, vec3_t start, vec3_t dir, qboolean altFire )
;295://---------------------------------------------------------
;296:{
line 297
;297:	int velocity	= BLASTER_VELOCITY;
ADDRLP4 4
CNSTI4 2300
ASGNI4
line 298
;298:	int	damage		= BLASTER_DAMAGE;
ADDRLP4 8
CNSTI4 20
ASGNI4
line 308
;299:	gentity_t *missile;
;300:
;301:	// NOTENOTE Vehicle models are not yet implemented
;302:/*	if ( ent->client && ent->client->ps.vehicleModel != 0 )
;303:	{
;304:		velocity = 10000;
;305:	}
;306:*/
;307:	
;308:	missile = CreateMissile( start, dir, velocity, 10000, ent, altFire );
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
CNSTI4 10000
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 CreateMissile
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ASGNP4
line 310
;309:
;310:	missile->classname = "blaster_proj";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $117
ASGNP4
line 311
;311:	missile->s.weapon = WP_BLASTER;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 4
ASGNI4
line 320
;312:
;313:	// NOTENOTE Vehicle models are not yet implemented
;314:/*	if ( ent->client && ent->client->ps.vehicleModel != 0 )
;315:	{
;316:		damage = 250;
;317:	}
;318:	*/
;319:
;320:	missile->damage = damage;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 321
;321:	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
CNSTI4 128
ASGNI4
line 322
;322:	missile->methodOfDeath = MOD_BLASTER;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 6
ASGNI4
line 323
;323:	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 262913
ASGNI4
line 327
;324:
;325:	// we don't want it to bounce forever
;326:	// NOTENOTE These don't bounce yet.
;327:	missile->bounceCount = 8;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 8
ASGNI4
line 328
;328:}
LABELV $116
endproc WP_FireBlasterMissile 16 24
export WP_FireEmplacedMissile
proc WP_FireEmplacedMissile 16 24
line 333
;329:
;330://---------------------------------------------------------
;331:void WP_FireEmplacedMissile( gentity_t *ent, vec3_t start, vec3_t dir, qboolean altFire, gentity_t *ignore )
;332://---------------------------------------------------------
;333:{
line 334
;334:	int velocity	= BLASTER_VELOCITY;
ADDRLP4 4
CNSTI4 2300
ASGNI4
line 335
;335:	int	damage		= BLASTER_DAMAGE;
ADDRLP4 8
CNSTI4 20
ASGNI4
line 345
;336:	gentity_t *missile;
;337:
;338:	// NOTENOTE Vehicle models are not yet implemented
;339:/*	if ( ent->client && ent->client->ps.vehicleModel != 0 )
;340:	{
;341:		velocity = 10000;
;342:	}
;343:*/
;344:	
;345:	missile = CreateMissile( start, dir, velocity, 10000, ent, altFire );
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 4
INDIRI4
CVIF4 4
ARGF4
CNSTI4 10000
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 12
ADDRGP4 CreateMissile
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ASGNP4
line 347
;346:
;347:	missile->classname = "emplaced_gun_proj";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $119
ASGNP4
line 348
;348:	missile->s.weapon = WP_TURRET;//WP_EMPLACED_GUN;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 15
ASGNI4
line 357
;349:
;350:	// NOTENOTE Vehicle models are not yet implemented
;351:/*	if ( ent->client && ent->client->ps.vehicleModel != 0 )
;352:	{
;353:		damage = 250;
;354:	}
;355:	*/
;356:
;357:	missile->activator = ignore;
ADDRLP4 0
INDIRP4
CNSTI4 732
ADDP4
ADDRFP4 16
INDIRP4
ASGNP4
line 359
;358:
;359:	missile->damage = damage;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 360
;360:	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
CNSTI4 128
ASGNI4
line 361
;361:	missile->methodOfDeath = MOD_BLASTER;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 6
ASGNI4
line 362
;362:	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 262913
ASGNI4
line 364
;363:
;364:	if (ignore)
ADDRFP4 16
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $120
line 365
;365:	{
line 366
;366:		missile->passThroughNum = ignore->s.number+1;
ADDRLP4 0
INDIRP4
CNSTI4 444
ADDP4
ADDRFP4 16
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 367
;367:	}
LABELV $120
line 371
;368:
;369:	// we don't want it to bounce forever
;370:	// NOTENOTE These don't bounce yet.
;371:	missile->bounceCount = 8;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 8
ASGNI4
line 372
;372:}
LABELV $118
endproc WP_FireEmplacedMissile 16 24
proc WP_FireBlaster 32 16
line 377
;373:
;374://---------------------------------------------------------
;375:static void WP_FireBlaster( gentity_t *ent, qboolean altFire )
;376://---------------------------------------------------------
;377:{
line 380
;378:	vec3_t	dir, angs;
;379:
;380:	vectoangles( forward, angs );
ADDRGP4 forward
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 382
;381:
;382:	if ( altFire )
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $123
line 383
;383:	{
line 385
;384:		// add some slop to the alt-fire direction
;385:		angs[PITCH] += crandom() * BLASTER_SPREAD;
ADDRLP4 24
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1070386381
CNSTF4 1073741824
ADDRLP4 24
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
ASGNF4
line 386
;386:		angs[YAW]	+= crandom() * BLASTER_SPREAD;
ADDRLP4 28
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1070386381
CNSTF4 1073741824
ADDRLP4 28
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
ASGNF4
line 387
;387:	}
LABELV $123
line 389
;388:
;389:	AngleVectors( angs, dir, NULL, NULL );
ADDRLP4 0
ARGP4
ADDRLP4 12
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
line 392
;390:
;391:	// FIXME: if temp_org does not have clear trace to inside the bbox, don't shoot!
;392:	WP_FireBlasterMissile( ent, muzzle, dir, altFire );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 muzzle
ARGP4
ADDRLP4 12
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 WP_FireBlasterMissile
CALLV
pop
line 393
;393:}
LABELV $122
endproc WP_FireBlaster 32 16
proc WP_DisruptorMainFire 1200 32
line 409
;394:
;395:
;396:
;397:int G_GetHitLocation(gentity_t *target, vec3_t ppoint);
;398:
;399:/*
;400:======================================================================
;401:
;402:DISRUPTOR
;403:
;404:======================================================================
;405:*/
;406://---------------------------------------------------------
;407:static void WP_DisruptorMainFire( gentity_t *ent )
;408://---------------------------------------------------------
;409:{
line 410
;410:	int			damage = DISRUPTOR_MAIN_DAMAGE;
ADDRLP4 1128
CNSTI4 30
ASGNI4
line 411
;411:	qboolean	render_impact = qtrue;
ADDRLP4 1124
CNSTI4 1
ASGNI4
line 415
;412:	vec3_t		start, end	/*, spot*/	;
;413:	trace_t		tr;
;414:	gentity_t	*traceEnt, *tent;
;415:	float		/*dist, */shotRange = 8192;
ADDRLP4 1120
CNSTF4 1174405120
ASGNF4
line 418
;416:	int			ignore, traces;
;417:
;418:	memset(&tr, 0, sizeof(tr)); //to shut the compiler up
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1080
ARGI4
ADDRGP4 memset
CALLP4
pop
line 420
;419:
;420:	VectorCopy( ent->client->ps.origin, start );
ADDRLP4 1084
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 421
;421:	start[2] += ent->client->ps.viewheight;//By eyes
ADDRLP4 1084+8
ADDRLP4 1084+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 168
ADDP4
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 423
;422:
;423:	VectorMA( start, shotRange, forward, end );
ADDRLP4 1104
ADDRLP4 1084
INDIRF4
ADDRGP4 forward
INDIRF4
ADDRLP4 1120
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 1104+4
ADDRLP4 1084+4
INDIRF4
ADDRGP4 forward+4
INDIRF4
ADDRLP4 1120
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 1104+8
ADDRLP4 1084+8
INDIRF4
ADDRGP4 forward+8
INDIRF4
ADDRLP4 1120
INDIRF4
MULF4
ADDF4
ASGNF4
line 427
;424:
;425://	trap_Trace( &tr, start, NULL, NULL, end, ent->s.number, MASK_SHOT);
;426:
;427:	ignore = ent->s.number;
ADDRLP4 1096
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 428
;428:	traces = 0;
ADDRLP4 1100
CNSTI4 0
ASGNI4
ADDRGP4 $135
JUMPV
LABELV $134
line 430
;429:	while ( traces < 10 )
;430:	{//need to loop this in case we hit a Jedi who dodges the shot
line 431
;431:		trap_Trace( &tr, start, NULL, NULL, end, ignore, MASK_SHOT );
ADDRLP4 4
ARGP4
ADDRLP4 1084
ARGP4
ADDRLP4 1136
CNSTP4 0
ASGNP4
ADDRLP4 1136
INDIRP4
ARGP4
ADDRLP4 1136
INDIRP4
ARGP4
ADDRLP4 1104
ARGP4
ADDRLP4 1096
INDIRI4
ARGI4
CNSTI4 769
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 433
;432:
;433:		traceEnt = &g_entities[tr.entityNum];
ADDRLP4 0
CNSTI4 828
ADDRLP4 4+52
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 435
;434:
;435:		if (traceEnt && traceEnt->client && traceEnt->client->ps.duelInProgress &&
ADDRLP4 1144
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 1144
INDIRU4
EQU4 $138
ADDRLP4 1148
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1148
INDIRP4
CVPU4 4
ADDRLP4 1144
INDIRU4
EQU4 $138
ADDRLP4 1148
INDIRP4
CNSTI4 1292
ADDP4
INDIRI4
CNSTI4 0
EQI4 $138
ADDRLP4 1148
INDIRP4
CNSTI4 1284
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
EQI4 $138
line 437
;436:			traceEnt->client->ps.duelIndex != ent->s.number)
;437:		{
line 438
;438:			VectorCopy( tr.endpos, start );
ADDRLP4 1084
ADDRLP4 4+12
INDIRB
ASGNB 12
line 439
;439:			ignore = tr.entityNum;
ADDRLP4 1096
ADDRLP4 4+52
INDIRI4
ASGNI4
line 440
;440:			traces++;
ADDRLP4 1100
ADDRLP4 1100
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 441
;441:			continue;
ADDRGP4 $135
JUMPV
LABELV $138
line 444
;442:		}
;443:
;444:		if ( Jedi_DodgeEvasion( traceEnt, ent, &tr, G_GetHitLocation(traceEnt, tr.endpos) ) )
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4+12
ARGP4
ADDRLP4 1152
ADDRGP4 G_GetHitLocation
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 1152
INDIRI4
ARGI4
ADDRLP4 1156
ADDRGP4 Jedi_DodgeEvasion
CALLI4
ASGNI4
ADDRLP4 1156
INDIRI4
CNSTI4 0
EQI4 $142
line 445
;445:		{//act like we didn't even hit him
line 446
;446:			VectorCopy( tr.endpos, start );
ADDRLP4 1084
ADDRLP4 4+12
INDIRB
ASGNB 12
line 447
;447:			ignore = tr.entityNum;
ADDRLP4 1096
ADDRLP4 4+52
INDIRI4
ASGNI4
line 448
;448:			traces++;
ADDRLP4 1100
ADDRLP4 1100
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 449
;449:			continue;
ADDRGP4 $135
JUMPV
LABELV $142
line 451
;450:		}
;451:		else if (traceEnt && traceEnt->client && traceEnt->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] >= FORCE_LEVEL_3)
ADDRLP4 1164
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 1164
INDIRU4
EQU4 $136
ADDRLP4 1168
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1168
INDIRP4
CVPU4 4
ADDRLP4 1164
INDIRU4
EQU4 $136
ADDRLP4 1168
INDIRP4
CNSTI4 996
ADDP4
INDIRI4
CNSTI4 3
LTI4 $136
line 452
;452:		{
line 453
;453:			if (WP_SaberCanBlock(traceEnt, tr.endpos, 0, MOD_DISRUPTOR, qtrue, 0))
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4+12
ARGP4
ADDRLP4 1172
CNSTI4 0
ASGNI4
ADDRLP4 1172
INDIRI4
ARGI4
CNSTI4 7
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 1172
INDIRI4
ARGI4
ADDRLP4 1176
ADDRGP4 WP_SaberCanBlock
CALLI4
ASGNI4
ADDRLP4 1176
INDIRI4
CNSTI4 0
EQI4 $136
line 454
;454:			{ //broadcast and stop the shot because it was blocked
line 455
;455:				gentity_t *te = NULL;
ADDRLP4 1180
CNSTP4 0
ASGNP4
line 457
;456:
;457:				tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_MAIN_SHOT );
ADDRLP4 4+12
ARGP4
CNSTI4 31
ARGI4
ADDRLP4 1184
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 1116
ADDRLP4 1184
INDIRP4
ASGNP4
line 458
;458:				VectorCopy( muzzle, tent->s.origin2 );
ADDRLP4 1116
INDIRP4
CNSTI4 104
ADDP4
ADDRGP4 muzzle
INDIRB
ASGNB 12
line 459
;459:				tent->s.eventParm = ent->s.number;
ADDRLP4 1116
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 461
;460:
;461:				te = G_TempEntity( tr.endpos, EV_SABER_BLOCK );
ADDRLP4 4+12
ARGP4
CNSTI4 28
ARGI4
ADDRLP4 1188
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 1180
ADDRLP4 1188
INDIRP4
ASGNP4
line 462
;462:				VectorCopy(tr.endpos, te->s.origin);
ADDRLP4 1180
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 4+12
INDIRB
ASGNB 12
line 463
;463:				VectorCopy(tr.plane.normal, te->s.angles);
ADDRLP4 1180
INDIRP4
CNSTI4 116
ADDP4
ADDRLP4 4+24
INDIRB
ASGNB 12
line 464
;464:				if (!te->s.angles[0] && !te->s.angles[1] && !te->s.angles[2])
ADDRLP4 1196
CNSTF4 0
ASGNF4
ADDRLP4 1180
INDIRP4
CNSTI4 116
ADDP4
INDIRF4
ADDRLP4 1196
INDIRF4
NEF4 $156
ADDRLP4 1180
INDIRP4
CNSTI4 120
ADDP4
INDIRF4
ADDRLP4 1196
INDIRF4
NEF4 $156
ADDRLP4 1180
INDIRP4
CNSTI4 124
ADDP4
INDIRF4
ADDRLP4 1196
INDIRF4
NEF4 $156
line 465
;465:				{
line 466
;466:					te->s.angles[1] = 1;
ADDRLP4 1180
INDIRP4
CNSTI4 120
ADDP4
CNSTF4 1065353216
ASGNF4
line 467
;467:				}
LABELV $156
line 468
;468:				te->s.eventParm = 0;
ADDRLP4 1180
INDIRP4
CNSTI4 256
ADDP4
CNSTI4 0
ASGNI4
line 470
;469:
;470:				return;
ADDRGP4 $126
JUMPV
line 472
;471:			}
;472:		}
line 474
;473:		//a Jedi is not dodging this shot
;474:		break;
LABELV $135
line 429
ADDRLP4 1100
INDIRI4
CNSTI4 10
LTI4 $134
LABELV $136
line 477
;475:	}
;476:
;477:	if ( tr.surfaceFlags & SURF_NOIMPACT ) 
ADDRLP4 4+44
INDIRI4
CNSTI4 524288
BANDI4
CNSTI4 0
EQI4 $158
line 478
;478:	{
line 479
;479:		render_impact = qfalse;
ADDRLP4 1124
CNSTI4 0
ASGNI4
line 480
;480:	}
LABELV $158
line 483
;481:
;482:	// always render a shot beam, doing this the old way because I don't much feel like overriding the effect.
;483:	tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_MAIN_SHOT );
ADDRLP4 4+12
ARGP4
CNSTI4 31
ARGI4
ADDRLP4 1136
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 1116
ADDRLP4 1136
INDIRP4
ASGNP4
line 484
;484:	VectorCopy( muzzle, tent->s.origin2 );
ADDRLP4 1116
INDIRP4
CNSTI4 104
ADDP4
ADDRGP4 muzzle
INDIRB
ASGNB 12
line 485
;485:	tent->s.eventParm = ent->s.number;
ADDRLP4 1116
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 487
;486:
;487:	traceEnt = &g_entities[tr.entityNum];
ADDRLP4 0
CNSTI4 828
ADDRLP4 4+52
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 489
;488:
;489:	if ( render_impact )
ADDRLP4 1124
INDIRI4
CNSTI4 0
EQI4 $163
line 490
;490:	{
line 491
;491:		if ( tr.entityNum < ENTITYNUM_WORLD && traceEnt->takedamage )
ADDRLP4 4+52
INDIRI4
CNSTI4 1022
GEI4 $165
ADDRLP4 0
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
CNSTI4 0
EQI4 $165
line 492
;492:		{
line 496
;493:			// Create a simple impact type mark that doesn't last long in the world
;494://			G_PlayEffect( G_EffectIndex( "disruptor/flesh_impact" ), tr.endpos, tr.plane.normal );
;495:
;496:			if ( traceEnt->client && LogAccuracyHit( traceEnt, ent )) 
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $168
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1144
ADDRGP4 LogAccuracyHit
CALLI4
ASGNI4
ADDRLP4 1144
INDIRI4
CNSTI4 0
EQI4 $168
line 497
;497:			{
line 498
;498:				ent->client->accuracy_hits++;
ADDRLP4 1148
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1656
ADDP4
ASGNP4
ADDRLP4 1148
INDIRP4
ADDRLP4 1148
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 499
;499:			} 
LABELV $168
line 501
;500:
;501:			G_Damage( traceEnt, ent, ent, forward, tr.endpos, damage, DAMAGE_NORMAL, MOD_DISRUPTOR );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1148
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1148
INDIRP4
ARGP4
ADDRLP4 1148
INDIRP4
ARGP4
ADDRGP4 forward
ARGP4
ADDRLP4 4+12
ARGP4
ADDRLP4 1128
INDIRI4
ARGI4
CNSTI4 0
ARGI4
CNSTI4 7
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 503
;502:			
;503:			tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_HIT );
ADDRLP4 4+12
ARGP4
CNSTI4 34
ARGI4
ADDRLP4 1152
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 1116
ADDRLP4 1152
INDIRP4
ASGNP4
line 504
;504:			tent->s.eventParm = DirToByte( tr.plane.normal );
ADDRLP4 4+24
ARGP4
ADDRLP4 1156
ADDRGP4 DirToByte
CALLI4
ASGNI4
ADDRLP4 1116
INDIRP4
CNSTI4 256
ADDP4
ADDRLP4 1156
INDIRI4
ASGNI4
line 505
;505:			if (traceEnt->client)
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $166
line 506
;506:			{
line 507
;507:				tent->s.weapon = 1;
ADDRLP4 1116
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 1
ASGNI4
line 508
;508:			}
line 509
;509:		}
ADDRGP4 $166
JUMPV
LABELV $165
line 511
;510:		else 
;511:		{
line 513
;512:			 // Hmmm, maybe don't make any marks on things that could break
;513:			tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_SNIPER_MISS );
ADDRLP4 4+12
ARGP4
CNSTI4 33
ARGI4
ADDRLP4 1140
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 1116
ADDRLP4 1140
INDIRP4
ASGNP4
line 514
;514:			tent->s.eventParm = DirToByte( tr.plane.normal );
ADDRLP4 4+24
ARGP4
ADDRLP4 1144
ADDRGP4 DirToByte
CALLI4
ASGNI4
ADDRLP4 1116
INDIRP4
CNSTI4 256
ADDP4
ADDRLP4 1144
INDIRI4
ASGNI4
line 515
;515:			tent->s.weapon = 1;
ADDRLP4 1116
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 1
ASGNI4
line 516
;516:		}
LABELV $166
line 517
;517:	}
LABELV $163
line 518
;518:}
LABELV $126
endproc WP_DisruptorMainFire 1200 32
export WP_DisruptorAltFire
proc WP_DisruptorAltFire 1252 32
line 524
;519:
;520:
;521://---------------------------------------------------------
;522:void WP_DisruptorAltFire( gentity_t *ent )
;523://---------------------------------------------------------
;524:{
line 525
;525:	int			damage = 0, skip;
ADDRLP4 1140
CNSTI4 0
ASGNI4
line 526
;526:	qboolean	render_impact = qtrue;
ADDRLP4 1124
CNSTI4 1
ASGNI4
line 531
;527:	vec3_t		start, end;
;528:	vec3_t		muzzle2;
;529:	trace_t		tr;
;530:	gentity_t	*traceEnt, *tent;
;531:	float		shotRange = 8192;
ADDRLP4 1116
CNSTF4 1174405120
ASGNF4
line 536
;532:	// float	dist, shotDist;
;533:	// vec3_t	spot, dir;
;534:	int			i;
;535:	int			count;
;536:	int			traces = DISRUPTOR_ALT_TRACES;
ADDRLP4 1132
CNSTI4 3
ASGNI4
line 537
;537:	qboolean	fullCharge = qfalse;
ADDRLP4 1128
CNSTI4 0
ASGNI4
line 539
;538:
;539:	damage = DISRUPTOR_ALT_DAMAGE-30;
ADDRLP4 1140
CNSTI4 70
ASGNI4
line 550
;540:
;541:	/*
;542:	if (ent->client->ps.zoomMode == 1 &&
;543:		ent->client->ps.zoomLocked)
;544:	{ //Scale the additional 25 damage based on the zoomFov for the client.
;545:	  //In this instance, zoomFov 1 is minimum zoom while zoomFov 50 is maximum.
;546:		damage += ent->client->ps.zoomFov/2;
;547:	}
;548:	*/
;549:
;550:	VectorCopy( muzzle, muzzle2 ); // making a backup copy
ADDRLP4 1144
ADDRGP4 muzzle
INDIRB
ASGNB 12
line 552
;551:
;552:	VectorCopy( ent->client->ps.origin, start );
ADDRLP4 1088
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 553
;553:	start[2] += ent->client->ps.viewheight;//By eyes
ADDRLP4 1088+8
ADDRLP4 1088+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 168
ADDP4
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 555
;554:
;555:	count = ( level.time - ent->client->ps.weaponChargeTime ) / DISRUPTOR_CHARGE_UNIT;
ADDRLP4 1136
ADDRGP4 level+32
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
SUBI4
CVIF4 4
CNSTF4 1112014848
DIVF4
CVFI4 4
ASGNI4
line 557
;556:
;557:	count *= 2;
ADDRLP4 1136
ADDRLP4 1136
INDIRI4
CNSTI4 1
LSHI4
ASGNI4
line 559
;558:
;559:	if ( count < 1 )
ADDRLP4 1136
INDIRI4
CNSTI4 1
GEI4 $180
line 560
;560:	{
line 561
;561:		count = 1;
ADDRLP4 1136
CNSTI4 1
ASGNI4
line 562
;562:	}
ADDRGP4 $181
JUMPV
LABELV $180
line 563
;563:	else if ( count >= 60 )
ADDRLP4 1136
INDIRI4
CNSTI4 60
LTI4 $182
line 564
;564:	{
line 565
;565:		count = 60;
ADDRLP4 1136
CNSTI4 60
ASGNI4
line 566
;566:		fullCharge = qtrue;
ADDRLP4 1128
CNSTI4 1
ASGNI4
line 567
;567:	}
LABELV $182
LABELV $181
line 570
;568:
;569:	// more powerful charges go through more things
;570:	if ( count < 10 )
ADDRLP4 1136
INDIRI4
CNSTI4 10
GEI4 $184
line 571
;571:	{
line 572
;572:		traces = 1;
ADDRLP4 1132
CNSTI4 1
ASGNI4
line 573
;573:	}
ADDRGP4 $185
JUMPV
LABELV $184
line 574
;574:	else if ( count < 20 )
ADDRLP4 1136
INDIRI4
CNSTI4 20
GEI4 $186
line 575
;575:	{
line 576
;576:		traces = 2;
ADDRLP4 1132
CNSTI4 2
ASGNI4
line 577
;577:	}
LABELV $186
LABELV $185
line 579
;578:
;579:	damage += count;
ADDRLP4 1140
ADDRLP4 1140
INDIRI4
ADDRLP4 1136
INDIRI4
ADDI4
ASGNI4
line 581
;580:
;581:	skip = ent->s.number;
ADDRLP4 1112
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 583
;582:
;583:	for (i = 0; i < traces; i++ )
ADDRLP4 1120
CNSTI4 0
ASGNI4
ADDRGP4 $191
JUMPV
LABELV $188
line 584
;584:	{
line 585
;585:		VectorMA( start, shotRange, forward, end );
ADDRLP4 1100
ADDRLP4 1088
INDIRF4
ADDRGP4 forward
INDIRF4
ADDRLP4 1116
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 1100+4
ADDRLP4 1088+4
INDIRF4
ADDRGP4 forward+4
INDIRF4
ADDRLP4 1116
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 1100+8
ADDRLP4 1088+8
INDIRF4
ADDRGP4 forward+8
INDIRF4
ADDRLP4 1116
INDIRF4
MULF4
ADDF4
ASGNF4
line 587
;586:
;587:		trap_Trace ( &tr, start, NULL, NULL, end, skip, MASK_SHOT);
ADDRLP4 4
ARGP4
ADDRLP4 1088
ARGP4
ADDRLP4 1160
CNSTP4 0
ASGNP4
ADDRLP4 1160
INDIRP4
ARGP4
ADDRLP4 1160
INDIRP4
ARGP4
ADDRLP4 1100
ARGP4
ADDRLP4 1112
INDIRI4
ARGI4
CNSTI4 769
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 589
;588:
;589:		if ( tr.surfaceFlags & SURF_NOIMPACT ) 
ADDRLP4 4+44
INDIRI4
CNSTI4 524288
BANDI4
CNSTI4 0
EQI4 $198
line 590
;590:		{
line 591
;591:			render_impact = qfalse;
ADDRLP4 1124
CNSTI4 0
ASGNI4
line 592
;592:		}
LABELV $198
line 594
;593:
;594:		traceEnt = &g_entities[tr.entityNum];
ADDRLP4 0
CNSTI4 828
ADDRLP4 4+52
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 596
;595:
;596:		if (traceEnt && traceEnt->client && traceEnt->client->ps.duelInProgress &&
ADDRLP4 1168
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 1168
INDIRU4
EQU4 $202
ADDRLP4 1172
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1172
INDIRP4
CVPU4 4
ADDRLP4 1168
INDIRU4
EQU4 $202
ADDRLP4 1172
INDIRP4
CNSTI4 1292
ADDP4
INDIRI4
CNSTI4 0
EQI4 $202
ADDRLP4 1172
INDIRP4
CNSTI4 1284
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
EQI4 $202
line 598
;597:			traceEnt->client->ps.duelIndex != ent->s.number)
;598:		{
line 599
;599:			skip = tr.entityNum;
ADDRLP4 1112
ADDRLP4 4+52
INDIRI4
ASGNI4
line 600
;600:			VectorCopy(tr.endpos, start);
ADDRLP4 1088
ADDRLP4 4+12
INDIRB
ASGNB 12
line 601
;601:			continue;
ADDRGP4 $189
JUMPV
LABELV $202
line 604
;602:		}
;603:
;604:		if (Jedi_DodgeEvasion(traceEnt, ent, &tr, G_GetHitLocation(traceEnt, tr.endpos)))
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4+12
ARGP4
ADDRLP4 1176
ADDRGP4 G_GetHitLocation
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 1176
INDIRI4
ARGI4
ADDRLP4 1180
ADDRGP4 Jedi_DodgeEvasion
CALLI4
ASGNI4
ADDRLP4 1180
INDIRI4
CNSTI4 0
EQI4 $206
line 605
;605:		{
line 606
;606:			skip = tr.entityNum;
ADDRLP4 1112
ADDRLP4 4+52
INDIRI4
ASGNI4
line 607
;607:			VectorCopy(tr.endpos, start);
ADDRLP4 1088
ADDRLP4 4+12
INDIRB
ASGNB 12
line 608
;608:			continue;
ADDRGP4 $189
JUMPV
LABELV $206
line 610
;609:		}
;610:		else if (traceEnt && traceEnt->client && traceEnt->client->ps.fd.forcePowerLevel[FP_SABERDEFEND] >= FORCE_LEVEL_3)
ADDRLP4 1188
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 1188
INDIRU4
EQU4 $211
ADDRLP4 1192
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1192
INDIRP4
CVPU4 4
ADDRLP4 1188
INDIRU4
EQU4 $211
ADDRLP4 1192
INDIRP4
CNSTI4 996
ADDP4
INDIRI4
CNSTI4 3
LTI4 $211
line 611
;611:		{
line 612
;612:			if (WP_SaberCanBlock(traceEnt, tr.endpos, 0, MOD_DISRUPTOR_SNIPER, qtrue, 0))
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4+12
ARGP4
ADDRLP4 1196
CNSTI4 0
ASGNI4
ADDRLP4 1196
INDIRI4
ARGI4
CNSTI4 9
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 1196
INDIRI4
ARGI4
ADDRLP4 1200
ADDRGP4 WP_SaberCanBlock
CALLI4
ASGNI4
ADDRLP4 1200
INDIRI4
CNSTI4 0
EQI4 $213
line 613
;613:			{ //broadcast and stop the shot because it was blocked
line 614
;614:				gentity_t *te = NULL;
ADDRLP4 1204
CNSTP4 0
ASGNP4
line 616
;615:
;616:				tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_SNIPER_SHOT );
ADDRLP4 4+12
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 1208
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 1084
ADDRLP4 1208
INDIRP4
ASGNP4
line 617
;617:				VectorCopy( muzzle, tent->s.origin2 );
ADDRLP4 1084
INDIRP4
CNSTI4 104
ADDP4
ADDRGP4 muzzle
INDIRB
ASGNB 12
line 618
;618:				tent->s.shouldtarget = fullCharge;
ADDRLP4 1084
INDIRP4
CNSTI4 268
ADDP4
ADDRLP4 1128
INDIRI4
ASGNI4
line 619
;619:				tent->s.eventParm = ent->s.number;
ADDRLP4 1084
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 621
;620:
;621:				te = G_TempEntity( tr.endpos, EV_SABER_BLOCK );
ADDRLP4 4+12
ARGP4
CNSTI4 28
ARGI4
ADDRLP4 1212
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 1204
ADDRLP4 1212
INDIRP4
ASGNP4
line 622
;622:				VectorCopy(tr.endpos, te->s.origin);
ADDRLP4 1204
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 4+12
INDIRB
ASGNB 12
line 623
;623:				VectorCopy(tr.plane.normal, te->s.angles);
ADDRLP4 1204
INDIRP4
CNSTI4 116
ADDP4
ADDRLP4 4+24
INDIRB
ASGNB 12
line 624
;624:				if (!te->s.angles[0] && !te->s.angles[1] && !te->s.angles[2])
ADDRLP4 1220
CNSTF4 0
ASGNF4
ADDRLP4 1204
INDIRP4
CNSTI4 116
ADDP4
INDIRF4
ADDRLP4 1220
INDIRF4
NEF4 $220
ADDRLP4 1204
INDIRP4
CNSTI4 120
ADDP4
INDIRF4
ADDRLP4 1220
INDIRF4
NEF4 $220
ADDRLP4 1204
INDIRP4
CNSTI4 124
ADDP4
INDIRF4
ADDRLP4 1220
INDIRF4
NEF4 $220
line 625
;625:				{
line 626
;626:					te->s.angles[1] = 1;
ADDRLP4 1204
INDIRP4
CNSTI4 120
ADDP4
CNSTF4 1065353216
ASGNF4
line 627
;627:				}
LABELV $220
line 628
;628:				te->s.eventParm = 0;
ADDRLP4 1204
INDIRP4
CNSTI4 256
ADDP4
CNSTI4 0
ASGNI4
line 630
;629:
;630:				return;
ADDRGP4 $177
JUMPV
LABELV $213
line 632
;631:			}
;632:		}
LABELV $211
line 635
;633:
;634:		// always render a shot beam, doing this the old way because I don't much feel like overriding the effect.
;635:		tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_SNIPER_SHOT );
ADDRLP4 4+12
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 1196
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 1084
ADDRLP4 1196
INDIRP4
ASGNP4
line 636
;636:		VectorCopy( muzzle, tent->s.origin2 );
ADDRLP4 1084
INDIRP4
CNSTI4 104
ADDP4
ADDRGP4 muzzle
INDIRB
ASGNB 12
line 637
;637:		tent->s.shouldtarget = fullCharge;
ADDRLP4 1084
INDIRP4
CNSTI4 268
ADDP4
ADDRLP4 1128
INDIRI4
ASGNI4
line 638
;638:		tent->s.eventParm = ent->s.number;
ADDRLP4 1084
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 641
;639:
;640:		// If the beam hits a skybox, etc. it would look foolish to add impact effects
;641:		if ( render_impact ) 
ADDRLP4 1124
INDIRI4
CNSTI4 0
EQI4 $190
line 642
;642:		{
line 643
;643:			if ( traceEnt->takedamage && traceEnt->client )
ADDRLP4 0
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
CNSTI4 0
EQI4 $225
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $225
line 644
;644:			{
line 645
;645:				tent->s.otherEntityNum = traceEnt->s.number;
ADDRLP4 1084
INDIRP4
CNSTI4 184
ADDP4
ADDRLP4 0
INDIRP4
INDIRI4
ASGNI4
line 649
;646:
;647:				// Create a simple impact type mark
;648://				G_PlayEffect( G_EffectIndex( "disruptor/alt_hit" ), tr.endpos, tr.plane.normal );
;649:				tent = G_TempEntity(tr.endpos, EV_MISSILE_MISS);
ADDRLP4 4+12
ARGP4
CNSTI4 74
ARGI4
ADDRLP4 1204
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 1084
ADDRLP4 1204
INDIRP4
ASGNP4
line 650
;650:				tent->s.eventParm = DirToByte(tr.plane.normal);
ADDRLP4 4+24
ARGP4
ADDRLP4 1208
ADDRGP4 DirToByte
CALLI4
ASGNI4
ADDRLP4 1084
INDIRP4
CNSTI4 256
ADDP4
ADDRLP4 1208
INDIRI4
ASGNI4
line 651
;651:				tent->s.eFlags |= EF_ALT_FIRING;
ADDRLP4 1212
ADDRLP4 1084
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 1212
INDIRP4
ADDRLP4 1212
INDIRP4
INDIRI4
CNSTI4 512
BORI4
ASGNI4
line 653
;652:	
;653:				if ( LogAccuracyHit( traceEnt, ent )) 
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1216
ADDRGP4 LogAccuracyHit
CALLI4
ASGNI4
ADDRLP4 1216
INDIRI4
CNSTI4 0
EQI4 $226
line 654
;654:				{
line 655
;655:					ent->client->accuracy_hits++;
ADDRLP4 1220
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1656
ADDP4
ASGNP4
ADDRLP4 1220
INDIRP4
ADDRLP4 1220
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 656
;656:				}
line 657
;657:			} 
ADDRGP4 $226
JUMPV
LABELV $225
line 659
;658:			else 
;659:			{
line 660
;660:				 if ( traceEnt->r.svFlags & SVF_GLASS_BRUSH 
ADDRLP4 1208
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 304
ADDP4
INDIRI4
CNSTI4 134217728
BANDI4
ADDRLP4 1208
INDIRI4
NEI4 $234
ADDRLP4 0
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
ADDRLP4 1208
INDIRI4
NEI4 $234
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 6
NEI4 $231
LABELV $234
line 663
;661:						|| traceEnt->takedamage 
;662:						|| traceEnt->s.eType == ET_MOVER )
;663:				 {
line 665
;664:					//rww - is there some reason this was doing nothing?
;665:					if ( traceEnt->takedamage )
ADDRLP4 0
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
CNSTI4 0
EQI4 $190
line 666
;666:					{
line 667
;667:						G_Damage( traceEnt, ent, ent, forward, tr.endpos, damage, 
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1212
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1212
INDIRP4
ARGP4
ADDRLP4 1212
INDIRP4
ARGP4
ADDRGP4 forward
ARGP4
ADDRLP4 4+12
ARGP4
ADDRLP4 1140
INDIRI4
ARGI4
CNSTI4 4
ARGI4
CNSTI4 9
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 670
;668:								DAMAGE_NO_KNOCKBACK/*|DAMAGE_HALF_ARMOR_REDUCTION*/, MOD_DISRUPTOR_SNIPER );
;669:
;670:						tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_HIT );
ADDRLP4 4+12
ARGP4
CNSTI4 34
ARGI4
ADDRLP4 1216
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 1084
ADDRLP4 1216
INDIRP4
ASGNP4
line 671
;671:						tent->s.eventParm = DirToByte( tr.plane.normal );
ADDRLP4 4+24
ARGP4
ADDRLP4 1220
ADDRGP4 DirToByte
CALLI4
ASGNI4
ADDRLP4 1084
INDIRP4
CNSTI4 256
ADDP4
ADDRLP4 1220
INDIRI4
ASGNI4
line 672
;672:					}
line 673
;673:				 }
ADDRGP4 $190
JUMPV
LABELV $231
line 675
;674:				 else
;675:				 {
line 677
;676:					 // Hmmm, maybe don't make any marks on things that could break
;677:					tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_SNIPER_MISS );
ADDRLP4 4+12
ARGP4
CNSTI4 33
ARGI4
ADDRLP4 1212
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 1084
ADDRLP4 1212
INDIRP4
ASGNP4
line 678
;678:					tent->s.eventParm = DirToByte( tr.plane.normal );
ADDRLP4 4+24
ARGP4
ADDRLP4 1216
ADDRGP4 DirToByte
CALLI4
ASGNI4
ADDRLP4 1084
INDIRP4
CNSTI4 256
ADDP4
ADDRLP4 1216
INDIRI4
ASGNI4
line 679
;679:				 }
line 680
;680:				break; // and don't try any more traces
ADDRGP4 $190
JUMPV
LABELV $226
line 683
;681:			}
;682:
;683:			if ( traceEnt->takedamage )
ADDRLP4 0
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
CNSTI4 0
EQI4 $224
line 684
;684:			{
line 686
;685:				vec3_t preAng;
;686:				int preHealth = traceEnt->health;
ADDRLP4 1204
ADDRLP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
ASGNI4
line 687
;687:				int preLegs = 0;
ADDRLP4 1208
CNSTI4 0
ASGNI4
line 688
;688:				int preTorso = 0;
ADDRLP4 1212
CNSTI4 0
ASGNI4
line 690
;689:
;690:				if (traceEnt->client)
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $244
line 691
;691:				{
line 692
;692:					preLegs = traceEnt->client->ps.legsAnim;
ADDRLP4 1208
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 92
ADDP4
INDIRI4
ASGNI4
line 693
;693:					preTorso = traceEnt->client->ps.torsoAnim;
ADDRLP4 1212
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 100
ADDP4
INDIRI4
ASGNI4
line 694
;694:					VectorCopy(traceEnt->client->ps.viewangles, preAng);
ADDRLP4 1216
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 156
ADDP4
INDIRB
ASGNB 12
line 695
;695:				}
LABELV $244
line 697
;696:
;697:				G_Damage( traceEnt, ent, ent, forward, tr.endpos, damage, DAMAGE_NO_KNOCKBACK, MOD_DISRUPTOR_SNIPER );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1228
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1228
INDIRP4
ARGP4
ADDRLP4 1228
INDIRP4
ARGP4
ADDRGP4 forward
ARGP4
ADDRLP4 4+12
ARGP4
ADDRLP4 1140
INDIRI4
ARGI4
CNSTI4 4
ARGI4
CNSTI4 9
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 699
;698:
;699:				if (traceEnt->client && preHealth > 0 && traceEnt->health <= 0 && fullCharge)
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $247
ADDRLP4 1236
CNSTI4 0
ASGNI4
ADDRLP4 1204
INDIRI4
ADDRLP4 1236
INDIRI4
LEI4 $247
ADDRLP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
ADDRLP4 1236
INDIRI4
GTI4 $247
ADDRLP4 1128
INDIRI4
ADDRLP4 1236
INDIRI4
EQI4 $247
line 700
;700:				{ //was killed by a fully charged sniper shot, so disintegrate
line 701
;701:					VectorCopy(preAng, traceEnt->client->ps.viewangles);
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 156
ADDP4
ADDRLP4 1216
INDIRB
ASGNB 12
line 703
;702:
;703:					traceEnt->client->ps.eFlags |= EF_DISINTEGRATION;
ADDRLP4 1240
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 1240
INDIRP4
ADDRLP4 1240
INDIRP4
INDIRI4
CNSTI4 33554432
BORI4
ASGNI4
line 704
;704:					VectorCopy(tr.endpos, traceEnt->client->ps.lastHitLoc);
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1356
ADDP4
ADDRLP4 4+12
INDIRB
ASGNB 12
line 706
;705:
;706:					traceEnt->client->ps.legsAnim = preLegs;
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 1208
INDIRI4
ASGNI4
line 707
;707:					traceEnt->client->ps.torsoAnim = preTorso;
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 100
ADDP4
ADDRLP4 1212
INDIRI4
ASGNI4
line 709
;708:
;709:					traceEnt->r.contents = 0;
ADDRLP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 0
ASGNI4
line 711
;710:
;711:					VectorClear(traceEnt->client->ps.velocity);
ADDRLP4 1244
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1248
CNSTF4 0
ASGNF4
ADDRLP4 1244
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 1248
INDIRF4
ASGNF4
ADDRLP4 1244
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 1248
INDIRF4
ASGNF4
ADDRLP4 1244
INDIRP4
CNSTI4 32
ADDP4
ADDRLP4 1248
INDIRF4
ASGNF4
line 712
;712:				}
LABELV $247
line 714
;713:
;714:				tent = G_TempEntity( tr.endpos, EV_DISRUPTOR_HIT );
ADDRLP4 4+12
ARGP4
CNSTI4 34
ARGI4
ADDRLP4 1240
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 1084
ADDRLP4 1240
INDIRP4
ASGNP4
line 715
;715:				tent->s.eventParm = DirToByte( tr.plane.normal );
ADDRLP4 4+24
ARGP4
ADDRLP4 1244
ADDRGP4 DirToByte
CALLI4
ASGNI4
ADDRLP4 1084
INDIRP4
CNSTI4 256
ADDP4
ADDRLP4 1244
INDIRI4
ASGNI4
line 716
;716:				if (traceEnt->client)
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $224
line 717
;717:				{
line 718
;718:					tent->s.weapon = 1;
ADDRLP4 1084
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 1
ASGNI4
line 719
;719:				}
line 720
;720:			}
line 721
;721:		}
line 723
;722:		else // not rendering impact, must be a skybox or other similar thing?
;723:		{
line 724
;724:			break; // don't try anymore traces
LABELV $224
line 728
;725:		}
;726:
;727:		// Get ready for an attempt to trace through another person
;728:		VectorCopy( tr.endpos, muzzle );
ADDRGP4 muzzle
ADDRLP4 4+12
INDIRB
ASGNB 12
line 729
;729:		VectorCopy( tr.endpos, start );
ADDRLP4 1088
ADDRLP4 4+12
INDIRB
ASGNB 12
line 730
;730:		skip = tr.entityNum;
ADDRLP4 1112
ADDRLP4 4+52
INDIRI4
ASGNI4
line 731
;731:	}
LABELV $189
line 583
ADDRLP4 1120
ADDRLP4 1120
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $191
ADDRLP4 1120
INDIRI4
ADDRLP4 1132
INDIRI4
LTI4 $188
LABELV $190
line 732
;732:}
LABELV $177
endproc WP_DisruptorAltFire 1252 32
proc WP_FireDisruptor 12 4
line 738
;733:
;734:
;735://---------------------------------------------------------
;736:static void WP_FireDisruptor( gentity_t *ent, qboolean altFire )
;737://---------------------------------------------------------
;738:{
line 739
;739:	if (!ent || !ent->client || ent->client->ps.zoomMode != 1)
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
EQU4 $261
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
EQU4 $261
ADDRLP4 8
INDIRP4
CNSTI4 1320
ADDP4
INDIRI4
CNSTI4 1
EQI4 $258
LABELV $261
line 740
;740:	{ //do not ever let it do the alt fire when not zoomed
line 741
;741:		altFire = qfalse;
ADDRFP4 4
CNSTI4 0
ASGNI4
line 742
;742:	}
LABELV $258
line 744
;743:
;744:	if ( altFire )
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $262
line 745
;745:	{
line 746
;746:		WP_DisruptorAltFire( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 WP_DisruptorAltFire
CALLV
pop
line 747
;747:	}
ADDRGP4 $263
JUMPV
LABELV $262
line 749
;748:	else
;749:	{
line 750
;750:		WP_DisruptorMainFire( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 WP_DisruptorMainFire
CALLV
pop
line 751
;751:	}
LABELV $263
line 752
;752:}
LABELV $257
endproc WP_FireDisruptor 12 4
proc WP_BowcasterAltFire 28 24
line 764
;753:
;754:
;755:/*
;756:======================================================================
;757:
;758:BOWCASTER
;759:
;760:======================================================================
;761:*/
;762:
;763:static void WP_BowcasterAltFire( gentity_t *ent )
;764:{
line 765
;765:	int	damage	= BOWCASTER_DAMAGE;
ADDRLP4 4
CNSTI4 50
ASGNI4
line 767
;766:
;767:	gentity_t *missile = CreateMissile( muzzle, forward, BOWCASTER_VELOCITY, 10000, ent, qfalse);
ADDRGP4 muzzle
ARGP4
ADDRGP4 forward
ARGP4
CNSTF4 1151500288
ARGF4
CNSTI4 10000
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 8
ADDRGP4 CreateMissile
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 769
;768:
;769:	missile->classname = "bowcaster_proj";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $265
ASGNP4
line 770
;770:	missile->s.weapon = WP_BOWCASTER;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 6
ASGNI4
line 772
;771:
;772:	VectorSet( missile->r.maxs, BOWCASTER_SIZE, BOWCASTER_SIZE, BOWCASTER_SIZE );
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1073741824
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1073741824
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1073741824
ASGNF4
line 773
;773:	VectorScale( missile->r.maxs, -1, missile->r.mins );
ADDRLP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
INDIRF4
MULF4
ASGNF4
line 775
;774:
;775:	missile->damage = damage;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 776
;776:	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
CNSTI4 128
ASGNI4
line 777
;777:	missile->methodOfDeath = MOD_BOWCASTER;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 10
ASGNI4
line 778
;778:	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 262913
ASGNI4
line 782
;779://	missile->splashDamage = BOWCASTER_SPLASH_DAMAGE;
;780://	missile->splashRadius = BOWCASTER_SPLASH_RADIUS;
;781:
;782:	missile->s.eFlags |= EF_BOUNCE;
ADDRLP4 24
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 783
;783:	missile->bounceCount = 3;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 3
ASGNI4
line 784
;784:}
LABELV $264
endproc WP_BowcasterAltFire 28 24
proc WP_BowcasterMainFire 80 24
line 789
;785:
;786://---------------------------------------------------------
;787:static void WP_BowcasterMainFire( gentity_t *ent )
;788://---------------------------------------------------------
;789:{
line 790
;790:	int			damage	= BOWCASTER_DAMAGE, count;
ADDRLP4 40
CNSTI4 50
ASGNI4
line 796
;791:	float		vel;
;792:	vec3_t		angs, dir;
;793:	gentity_t	*missile;
;794:	int i;
;795:
;796:	count = ( level.time - ent->client->ps.weaponChargeTime ) / BOWCASTER_CHARGE_UNIT;
ADDRLP4 20
ADDRGP4 level+32
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
SUBI4
CVIF4 4
CNSTF4 1128792064
DIVF4
CVFI4 4
ASGNI4
line 798
;797:
;798:	if ( count < 1 )
ADDRLP4 20
INDIRI4
CNSTI4 1
GEI4 $268
line 799
;799:	{
line 800
;800:		count = 1;
ADDRLP4 20
CNSTI4 1
ASGNI4
line 801
;801:	}
ADDRGP4 $269
JUMPV
LABELV $268
line 802
;802:	else if ( count > 5 )
ADDRLP4 20
INDIRI4
CNSTI4 5
LEI4 $270
line 803
;803:	{
line 804
;804:		count = 5;
ADDRLP4 20
CNSTI4 5
ASGNI4
line 805
;805:	}
LABELV $270
LABELV $269
line 807
;806:
;807:	if ( !(count & 1 ))
ADDRLP4 20
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $272
line 808
;808:	{
line 810
;809:		// if we aren't odd, knock us down a level
;810:		count--;
ADDRLP4 20
ADDRLP4 20
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 811
;811:	}
LABELV $272
line 814
;812:
;813:	//scale the damage down based on how many are about to be fired
;814:	if (count <= 1)
ADDRLP4 20
INDIRI4
CNSTI4 1
GTI4 $274
line 815
;815:	{
line 816
;816:		damage = 50;
ADDRLP4 40
CNSTI4 50
ASGNI4
line 817
;817:	}
ADDRGP4 $275
JUMPV
LABELV $274
line 818
;818:	else if (count == 2)
ADDRLP4 20
INDIRI4
CNSTI4 2
NEI4 $276
line 819
;819:	{
line 820
;820:		damage = 45;
ADDRLP4 40
CNSTI4 45
ASGNI4
line 821
;821:	}
ADDRGP4 $277
JUMPV
LABELV $276
line 822
;822:	else if (count == 3)
ADDRLP4 20
INDIRI4
CNSTI4 3
NEI4 $278
line 823
;823:	{
line 824
;824:		damage = 40;
ADDRLP4 40
CNSTI4 40
ASGNI4
line 825
;825:	}
ADDRGP4 $279
JUMPV
LABELV $278
line 826
;826:	else if (count == 4)
ADDRLP4 20
INDIRI4
CNSTI4 4
NEI4 $280
line 827
;827:	{
line 828
;828:		damage = 35;
ADDRLP4 40
CNSTI4 35
ASGNI4
line 829
;829:	}
ADDRGP4 $281
JUMPV
LABELV $280
line 831
;830:	else
;831:	{
line 832
;832:		damage = 30;
ADDRLP4 40
CNSTI4 30
ASGNI4
line 833
;833:	}
LABELV $281
LABELV $279
LABELV $277
LABELV $275
line 835
;834:
;835:	for (i = 0; i < count; i++ )
ADDRLP4 16
CNSTI4 0
ASGNI4
ADDRGP4 $285
JUMPV
LABELV $282
line 836
;836:	{
line 838
;837:		// create a range of different velocities
;838:		vel = BOWCASTER_VELOCITY * ( crandom() * BOWCASTER_VEL_RANGE + 1.0f );
ADDRLP4 44
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 24
CNSTF4 1151500288
CNSTF4 1050253722
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
CNSTF4 1065353216
ADDF4
MULF4
ASGNF4
line 840
;839:
;840:		vectoangles( forward, angs );
ADDRGP4 forward
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 843
;841:
;842:		// add some slop to the alt-fire direction
;843:		angs[PITCH] += crandom() * BOWCASTER_ALT_SPREAD * 0.2f;
ADDRLP4 48
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 4
INDIRF4
CNSTF4 1045220557
CNSTF4 1084227584
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
MULF4
MULF4
ADDF4
ASGNF4
line 844
;844:		angs[YAW]	+= ((i+0.5f) * BOWCASTER_ALT_SPREAD - count * 0.5f * BOWCASTER_ALT_SPREAD );
ADDRLP4 52
CNSTF4 1084227584
ASGNF4
ADDRLP4 56
CNSTF4 1056964608
ASGNF4
ADDRLP4 4+4
ADDRLP4 4+4
INDIRF4
ADDRLP4 52
INDIRF4
ADDRLP4 16
INDIRI4
CVIF4 4
ADDRLP4 56
INDIRF4
ADDF4
MULF4
ADDRLP4 52
INDIRF4
ADDRLP4 56
INDIRF4
ADDRLP4 20
INDIRI4
CVIF4 4
MULF4
MULF4
SUBF4
ADDF4
ASGNF4
line 846
;845:		
;846:		AngleVectors( angs, dir, NULL, NULL );
ADDRLP4 4
ARGP4
ADDRLP4 28
ARGP4
ADDRLP4 60
CNSTP4 0
ASGNP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRLP4 60
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 848
;847:
;848:		missile = CreateMissile( muzzle, dir, vel, 10000, ent, qtrue );
ADDRGP4 muzzle
ARGP4
ADDRLP4 28
ARGP4
ADDRLP4 24
INDIRF4
ARGF4
CNSTI4 10000
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 64
ADDRGP4 CreateMissile
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 64
INDIRP4
ASGNP4
line 850
;849:
;850:		missile->classname = "bowcaster_alt_proj";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $287
ASGNP4
line 851
;851:		missile->s.weapon = WP_BOWCASTER;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 6
ASGNI4
line 853
;852:
;853:		VectorSet( missile->r.maxs, BOWCASTER_SIZE, BOWCASTER_SIZE, BOWCASTER_SIZE );
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1073741824
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1073741824
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1073741824
ASGNF4
line 854
;854:		VectorScale( missile->r.maxs, -1, missile->r.mins );
ADDRLP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
INDIRF4
MULF4
ASGNF4
line 856
;855:
;856:		missile->damage = damage;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRLP4 40
INDIRI4
ASGNI4
line 857
;857:		missile->dflags = DAMAGE_DEATH_KNOCKBACK;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
CNSTI4 128
ASGNI4
line 858
;858:		missile->methodOfDeath = MOD_BOWCASTER;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 10
ASGNI4
line 859
;859:		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 262913
ASGNI4
line 864
;860://		missile->splashDamage = BOWCASTER_SPLASH_DAMAGE;
;861://		missile->splashRadius = BOWCASTER_SPLASH_RADIUS;
;862:
;863:		// we don't want it to bounce
;864:		missile->bounceCount = 0;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 0
ASGNI4
line 865
;865:	}
LABELV $283
line 835
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $285
ADDRLP4 16
INDIRI4
ADDRLP4 20
INDIRI4
LTI4 $282
line 866
;866:}
LABELV $266
endproc WP_BowcasterMainFire 80 24
proc WP_FireBowcaster 0 4
line 871
;867:
;868://---------------------------------------------------------
;869:static void WP_FireBowcaster( gentity_t *ent, qboolean altFire )
;870://---------------------------------------------------------
;871:{
line 872
;872:	if ( altFire )
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $289
line 873
;873:	{
line 874
;874:		WP_BowcasterAltFire( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 WP_BowcasterAltFire
CALLV
pop
line 875
;875:	}
ADDRGP4 $290
JUMPV
LABELV $289
line 877
;876:	else
;877:	{
line 878
;878:		WP_BowcasterMainFire( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 WP_BowcasterMainFire
CALLV
pop
line 879
;879:	}
LABELV $290
line 880
;880:}
LABELV $288
endproc WP_FireBowcaster 0 4
proc WP_RepeaterMainFire 12 24
line 895
;881:
;882:
;883:
;884:/*
;885:======================================================================
;886:
;887:REPEATER
;888:
;889:======================================================================
;890:*/
;891:
;892://---------------------------------------------------------
;893:static void WP_RepeaterMainFire( gentity_t *ent, vec3_t dir )
;894://---------------------------------------------------------
;895:{
line 896
;896:	int	damage	= REPEATER_DAMAGE;
ADDRLP4 4
CNSTI4 14
ASGNI4
line 898
;897:
;898:	gentity_t *missile = CreateMissile( muzzle, dir, REPEATER_VELOCITY, 10000, ent, qfalse );
ADDRGP4 muzzle
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
CNSTF4 1153957888
ARGF4
CNSTI4 10000
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 8
ADDRGP4 CreateMissile
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 900
;899:
;900:	missile->classname = "repeater_proj";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $292
ASGNP4
line 901
;901:	missile->s.weapon = WP_REPEATER;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 7
ASGNI4
line 903
;902:
;903:	missile->damage = damage;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 904
;904:	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
CNSTI4 128
ASGNI4
line 905
;905:	missile->methodOfDeath = MOD_REPEATER;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 11
ASGNI4
line 906
;906:	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 262913
ASGNI4
line 909
;907:
;908:	// we don't want it to bounce forever
;909:	missile->bounceCount = 8;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 8
ASGNI4
line 910
;910:}
LABELV $291
endproc WP_RepeaterMainFire 12 24
proc WP_RepeaterAltFire 28 24
line 915
;911:
;912://---------------------------------------------------------
;913:static void WP_RepeaterAltFire( gentity_t *ent )
;914://---------------------------------------------------------
;915:{
line 916
;916:	int	damage	= REPEATER_ALT_DAMAGE;
ADDRLP4 4
CNSTI4 60
ASGNI4
line 918
;917:
;918:	gentity_t *missile = CreateMissile( muzzle, forward, REPEATER_ALT_VELOCITY, 10000, ent, qtrue );
ADDRGP4 muzzle
ARGP4
ADDRGP4 forward
ARGP4
CNSTF4 1149861888
ARGF4
CNSTI4 10000
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 8
ADDRGP4 CreateMissile
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 920
;919:
;920:	missile->classname = "repeater_alt_proj";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $294
ASGNP4
line 921
;921:	missile->s.weapon = WP_REPEATER;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 7
ASGNI4
line 924
;922://	missile->mass = 10;		// NOTENOTE No mass yet
;923:
;924:	VectorSet( missile->r.maxs, REPEATER_ALT_SIZE, REPEATER_ALT_SIZE, REPEATER_ALT_SIZE );
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1077936128
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1077936128
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1077936128
ASGNF4
line 925
;925:	VectorScale( missile->r.maxs, -1, missile->r.mins );
ADDRLP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
INDIRF4
MULF4
ASGNF4
line 926
;926:	missile->s.pos.trType = TR_GRAVITY;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 5
ASGNI4
line 927
;927:	missile->s.pos.trDelta[2] += 40.0f; //give a slight boost in the upward direction
ADDRLP4 24
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRF4
CNSTF4 1109393408
ADDF4
ASGNF4
line 928
;928:	missile->damage = damage;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 929
;929:	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
CNSTI4 128
ASGNI4
line 930
;930:	missile->methodOfDeath = MOD_REPEATER_ALT;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 12
ASGNI4
line 931
;931:	missile->splashMethodOfDeath = MOD_REPEATER_ALT_SPLASH;
ADDRLP4 0
INDIRP4
CNSTI4 708
ADDP4
CNSTI4 13
ASGNI4
line 932
;932:	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 262913
ASGNI4
line 933
;933:	missile->splashDamage = REPEATER_ALT_SPLASH_DAMAGE;
ADDRLP4 0
INDIRP4
CNSTI4 696
ADDP4
CNSTI4 60
ASGNI4
line 934
;934:	missile->splashRadius = REPEATER_ALT_SPLASH_RADIUS;
ADDRLP4 0
INDIRP4
CNSTI4 700
ADDP4
CNSTI4 128
ASGNI4
line 937
;935:
;936:	// we don't want it to bounce forever
;937:	missile->bounceCount = 8;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 8
ASGNI4
line 938
;938:}
LABELV $293
endproc WP_RepeaterAltFire 28 24
proc WP_FireRepeater 36 16
line 943
;939:
;940://---------------------------------------------------------
;941:static void WP_FireRepeater( gentity_t *ent, qboolean altFire )
;942://---------------------------------------------------------
;943:{
line 946
;944:	vec3_t	dir, angs;
;945:
;946:	vectoangles( forward, angs );
ADDRGP4 forward
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 948
;947:
;948:	if ( altFire )
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $296
line 949
;949:	{
line 950
;950:		WP_RepeaterAltFire( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 WP_RepeaterAltFire
CALLV
pop
line 951
;951:	}
ADDRGP4 $297
JUMPV
LABELV $296
line 953
;952:	else
;953:	{
line 955
;954:		// add some slop to the alt-fire direction
;955:		angs[PITCH] += crandom() * REPEATER_SPREAD;
ADDRLP4 24
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1068708659
CNSTF4 1073741824
ADDRLP4 24
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
ASGNF4
line 956
;956:		angs[YAW]	+= crandom() * REPEATER_SPREAD;
ADDRLP4 28
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1068708659
CNSTF4 1073741824
ADDRLP4 28
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
ASGNF4
line 958
;957:
;958:		AngleVectors( angs, dir, NULL, NULL );
ADDRLP4 0
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 32
CNSTP4 0
ASGNP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 961
;959:
;960:		// NOTENOTE if temp_org does not have clear trace to inside the bbox, don't shoot!
;961:		WP_RepeaterMainFire( ent, dir );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRGP4 WP_RepeaterMainFire
CALLV
pop
line 962
;962:	}
LABELV $297
line 963
;963:}
LABELV $295
endproc WP_FireRepeater 36 16
proc WP_DEMP2_MainFire 24 24
line 975
;964:
;965:
;966:/*
;967:======================================================================
;968:
;969:DEMP2
;970:
;971:======================================================================
;972:*/
;973:
;974:static void WP_DEMP2_MainFire( gentity_t *ent )
;975:{
line 976
;976:	int	damage	= DEMP2_DAMAGE;
ADDRLP4 4
CNSTI4 35
ASGNI4
line 978
;977:
;978:	gentity_t *missile = CreateMissile( muzzle, forward, DEMP2_VELOCITY, 10000, ent, qfalse);
ADDRGP4 muzzle
ARGP4
ADDRGP4 forward
ARGP4
CNSTF4 1155596288
ARGF4
CNSTI4 10000
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 8
ADDRGP4 CreateMissile
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 980
;979:
;980:	missile->classname = "demp2_proj";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $300
ASGNP4
line 981
;981:	missile->s.weapon = WP_DEMP2;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 8
ASGNI4
line 983
;982:
;983:	VectorSet( missile->r.maxs, DEMP2_SIZE, DEMP2_SIZE, DEMP2_SIZE );
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1073741824
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1073741824
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1073741824
ASGNF4
line 984
;984:	VectorScale( missile->r.maxs, -1, missile->r.mins );
ADDRLP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
INDIRF4
MULF4
ASGNF4
line 985
;985:	missile->damage = damage;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 986
;986:	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
CNSTI4 128
ASGNI4
line 987
;987:	missile->methodOfDeath = MOD_DEMP2;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 14
ASGNI4
line 989
;988:	//rww - Don't want this blockable, do we?
;989:	missile->clipmask = MASK_SHOT;// | CONTENTS_LIGHTSABER;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 769
ASGNI4
line 992
;990:
;991:	// we don't want it to ever bounce
;992:	missile->bounceCount = 0;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 0
ASGNI4
line 993
;993:}
LABELV $299
endproc WP_DEMP2_MainFire 24 24
export DEMP2_AltRadiusDamage
proc DEMP2_AltRadiusDamage 8324 32
line 998
;994:
;995:static gentity_t *ent_list[MAX_GENTITIES];
;996:
;997:void DEMP2_AltRadiusDamage( gentity_t *ent )
;998:{
line 999
;999:	float		frac = ( level.time - ent->bolt_Head ) / 800.0f; // / 1600.0f; // synchronize with demp2 effect
ADDRLP4 8268
ADDRGP4 level+32
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
SUBI4
CVIF4 4
CNSTF4 1145569280
DIVF4
ASGNF4
line 1004
;1000:	float		dist, radius, fact;
;1001:	gentity_t	*gent;
;1002:	int			iEntityList[MAX_GENTITIES];
;1003:	gentity_t	*entityList[MAX_GENTITIES];
;1004:	gentity_t	*myOwner = NULL;
ADDRLP4 44
CNSTP4 0
ASGNP4
line 1009
;1005:	int			numListedEntities, i, e;
;1006:	vec3_t		mins, maxs;
;1007:	vec3_t		v, dir;
;1008:
;1009:	if (ent->r.ownerNum >= 0 &&
ADDRLP4 8276
ADDRFP4 0
INDIRP4
CNSTI4 396
ADDP4
INDIRI4
ASGNI4
ADDRLP4 8276
INDIRI4
CNSTI4 0
LTI4 $303
ADDRLP4 8276
INDIRI4
CNSTI4 32
GEI4 $303
line 1011
;1010:		ent->r.ownerNum < MAX_CLIENTS)
;1011:	{
line 1012
;1012:		myOwner = &g_entities[ent->r.ownerNum];
ADDRLP4 44
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
line 1013
;1013:	}
LABELV $303
line 1015
;1014:
;1015:	if (!myOwner || !myOwner->inuse || !myOwner->client)
ADDRLP4 8284
CNSTU4 0
ASGNU4
ADDRLP4 44
INDIRP4
CVPU4 4
ADDRLP4 8284
INDIRU4
EQU4 $308
ADDRLP4 44
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $308
ADDRLP4 44
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 8284
INDIRU4
NEU4 $305
LABELV $308
line 1016
;1016:	{
line 1017
;1017:		ent->think = G_FreeEntity;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 G_FreeEntity
ASGNP4
line 1018
;1018:		ent->nextthink = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1019
;1019:		return;
ADDRGP4 $301
JUMPV
LABELV $305
line 1022
;1020:	}
;1021:
;1022:	frac *= frac * frac; // yes, this is completely ridiculous...but it causes the shell to grow slowly then "explode" at the end
ADDRLP4 8268
ADDRLP4 8268
INDIRF4
ADDRLP4 8268
INDIRF4
ADDRLP4 8268
INDIRF4
MULF4
MULF4
ASGNF4
line 1024
;1023:	
;1024:	radius = frac * 200.0f; // 200 is max radius...the model is aprox. 100 units tall...the fx draw code mults. this by 2.
ADDRLP4 32
CNSTF4 1128792064
ADDRLP4 8268
INDIRF4
MULF4
ASGNF4
line 1026
;1025:
;1026:	fact = ent->count*0.6;
ADDRLP4 8272
CNSTF4 1058642330
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CVIF4 4
MULF4
ASGNF4
line 1028
;1027:
;1028:	if (fact < 1)
ADDRLP4 8272
INDIRF4
CNSTF4 1065353216
GEF4 $310
line 1029
;1029:	{
line 1030
;1030:		fact = 1;
ADDRLP4 8272
CNSTF4 1065353216
ASGNF4
line 1031
;1031:	}
LABELV $310
line 1033
;1032:
;1033:	radius *= fact;
ADDRLP4 32
ADDRLP4 32
INDIRF4
ADDRLP4 8272
INDIRF4
MULF4
ASGNF4
line 1035
;1034:
;1035:	for ( i = 0 ; i < 3 ; i++ ) 
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $312
line 1036
;1036:	{
line 1037
;1037:		mins[i] = ent->r.currentOrigin[i] - radius;
ADDRLP4 8292
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 8292
INDIRI4
ADDRLP4 8244
ADDP4
ADDRLP4 8292
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDP4
INDIRF4
ADDRLP4 32
INDIRF4
SUBF4
ASGNF4
line 1038
;1038:		maxs[i] = ent->r.currentOrigin[i] + radius;
ADDRLP4 8296
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 8296
INDIRI4
ADDRLP4 8256
ADDP4
ADDRLP4 8296
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDP4
INDIRF4
ADDRLP4 32
INDIRF4
ADDF4
ASGNF4
line 1039
;1039:	}
LABELV $313
line 1035
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $312
line 1041
;1040:
;1041:	numListedEntities = trap_EntitiesInBox( mins, maxs, iEntityList, MAX_GENTITIES );
ADDRLP4 8244
ARGP4
ADDRLP4 8256
ARGP4
ADDRLP4 4148
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 8292
ADDRGP4 trap_EntitiesInBox
CALLI4
ASGNI4
ADDRLP4 48
ADDRLP4 8292
INDIRI4
ASGNI4
line 1043
;1042:
;1043:	i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $317
JUMPV
LABELV $316
line 1045
;1044:	while (i < numListedEntities)
;1045:	{
line 1046
;1046:		entityList[i] = &g_entities[iEntityList[i]];
ADDRLP4 8296
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 8296
INDIRI4
ADDRLP4 52
ADDP4
CNSTI4 828
ADDRLP4 8296
INDIRI4
ADDRLP4 4148
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1047
;1047:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1048
;1048:	}
LABELV $317
line 1044
ADDRLP4 0
INDIRI4
ADDRLP4 48
INDIRI4
LTI4 $316
line 1050
;1049:
;1050:	for ( e = 0 ; e < numListedEntities ; e++ ) 
ADDRLP4 36
CNSTI4 0
ASGNI4
ADDRGP4 $322
JUMPV
LABELV $319
line 1051
;1051:	{
line 1052
;1052:		gent = entityList[ e ];
ADDRLP4 4
ADDRLP4 36
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 52
ADDP4
INDIRP4
ASGNP4
line 1054
;1053:
;1054:		if ( !gent || !gent->takedamage || !gent->r.contents )
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $326
ADDRLP4 8300
CNSTI4 0
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
ADDRLP4 8300
INDIRI4
EQI4 $326
ADDRLP4 4
INDIRP4
CNSTI4 340
ADDP4
INDIRI4
ADDRLP4 8300
INDIRI4
NEI4 $323
LABELV $326
line 1055
;1055:		{
line 1056
;1056:			continue;
ADDRGP4 $320
JUMPV
LABELV $323
line 1060
;1057:		}
;1058:
;1059:		// find the distance from the edge of the bounding box
;1060:		for ( i = 0 ; i < 3 ; i++ ) 
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $327
line 1061
;1061:		{
line 1062
;1062:			if ( ent->r.currentOrigin[i] < gent->r.absmin[i] ) 
ADDRLP4 8304
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 8304
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDP4
INDIRF4
ADDRLP4 8304
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 344
ADDP4
ADDP4
INDIRF4
GEF4 $331
line 1063
;1063:			{
line 1064
;1064:				v[i] = gent->r.absmin[i] - ent->r.currentOrigin[i];
ADDRLP4 8308
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 8308
INDIRI4
ADDRLP4 8
ADDP4
ADDRLP4 8308
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 344
ADDP4
ADDP4
INDIRF4
ADDRLP4 8308
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1065
;1065:			} 
ADDRGP4 $332
JUMPV
LABELV $331
line 1066
;1066:			else if ( ent->r.currentOrigin[i] > gent->r.absmax[i] ) 
ADDRLP4 8308
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 8308
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDP4
INDIRF4
ADDRLP4 8308
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 356
ADDP4
ADDP4
INDIRF4
LEF4 $333
line 1067
;1067:			{
line 1068
;1068:				v[i] = ent->r.currentOrigin[i] - gent->r.absmax[i];
ADDRLP4 8312
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 8312
INDIRI4
ADDRLP4 8
ADDP4
ADDRLP4 8312
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDP4
INDIRF4
ADDRLP4 8312
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 356
ADDP4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1069
;1069:			} 
ADDRGP4 $334
JUMPV
LABELV $333
line 1071
;1070:			else 
;1071:			{
line 1072
;1072:				v[i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
CNSTF4 0
ASGNF4
line 1073
;1073:			}
LABELV $334
LABELV $332
line 1074
;1074:		}
LABELV $328
line 1060
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $327
line 1077
;1075:
;1076:		// shape is an ellipsoid, so cut vertical distance in half`
;1077:		v[2] *= 0.5f;
ADDRLP4 8+8
CNSTF4 1056964608
ADDRLP4 8+8
INDIRF4
MULF4
ASGNF4
line 1079
;1078:
;1079:		dist = VectorLength( v );
ADDRLP4 8
ARGP4
ADDRLP4 8304
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 40
ADDRLP4 8304
INDIRF4
ASGNF4
line 1081
;1080:
;1081:		if ( dist >= radius ) 
ADDRLP4 40
INDIRF4
ADDRLP4 32
INDIRF4
LTF4 $336
line 1082
;1082:		{
line 1084
;1083:			// shockwave hasn't hit them yet
;1084:			continue;
ADDRGP4 $320
JUMPV
LABELV $336
line 1088
;1085:		}
;1086:
;1087:		//if ( dist < ent->bolt_LArm )
;1088:		if (dist+(16*ent->count) < ent->bolt_LArm)
ADDRLP4 8308
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRF4
ADDRLP4 8308
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 4
LSHI4
CVIF4 4
ADDF4
ADDRLP4 8308
INDIRP4
CNSTI4 788
ADDP4
INDIRI4
CVIF4 4
GEF4 $338
line 1089
;1089:		{
line 1091
;1090:			// shockwave has already hit this thing...
;1091:			continue;
ADDRGP4 $320
JUMPV
LABELV $338
line 1094
;1092:		}
;1093:
;1094:		VectorCopy( gent->r.currentOrigin, v );
ADDRLP4 8
ADDRLP4 4
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 1095
;1095:		VectorSubtract( v, ent->r.currentOrigin, dir);
ADDRLP4 8312
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
ADDRLP4 8
INDIRF4
ADDRLP4 8312
INDIRP4
CNSTI4 368
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 20+4
ADDRLP4 8+4
INDIRF4
ADDRLP4 8312
INDIRP4
CNSTI4 372
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 20+8
ADDRLP4 8+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 376
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1098
;1096:
;1097:		// push the center of mass higher than the origin so players get knocked into the air more
;1098:		dir[2] += 12;
ADDRLP4 20+8
ADDRLP4 20+8
INDIRF4
CNSTF4 1094713344
ADDF4
ASGNF4
line 1100
;1099:
;1100:		if (gent != myOwner)
ADDRLP4 4
INDIRP4
CVPU4 4
ADDRLP4 44
INDIRP4
CVPU4 4
EQU4 $345
line 1101
;1101:		{
line 1102
;1102:			G_Damage( gent, myOwner, myOwner, dir, ent->r.currentOrigin, ent->damage, DAMAGE_DEATH_KNOCKBACK, ent->splashMethodOfDeath );
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 44
INDIRP4
ARGP4
ADDRLP4 44
INDIRP4
ARGP4
ADDRLP4 20
ARGP4
ADDRLP4 8320
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8320
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 8320
INDIRP4
CNSTI4 688
ADDP4
INDIRI4
ARGI4
CNSTI4 128
ARGI4
ADDRLP4 8320
INDIRP4
CNSTI4 708
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 1103
;1103:		}
LABELV $345
line 1104
;1104:	}
LABELV $320
line 1050
ADDRLP4 36
ADDRLP4 36
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $322
ADDRLP4 36
INDIRI4
ADDRLP4 48
INDIRI4
LTI4 $319
line 1107
;1105:
;1106:	// store the last fraction so that next time around we can test against those things that fall between that last point and where the current shockwave edge is
;1107:	ent->bolt_LArm = radius;
ADDRFP4 0
INDIRP4
CNSTI4 788
ADDP4
ADDRLP4 32
INDIRF4
CVFI4 4
ASGNI4
line 1109
;1108:
;1109:	if ( frac < 1.0f )
ADDRLP4 8268
INDIRF4
CNSTF4 1065353216
GEF4 $347
line 1110
;1110:	{
line 1112
;1111:		// shock is still happening so continue letting it expand
;1112:		ent->nextthink = level.time + 50;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 50
ADDI4
ASGNI4
line 1113
;1113:	}
ADDRGP4 $348
JUMPV
LABELV $347
line 1115
;1114:	else
;1115:	{ //don't just leave the entity around
line 1116
;1116:		ent->think = G_FreeEntity;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 G_FreeEntity
ASGNP4
line 1117
;1117:		ent->nextthink = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1118
;1118:	}
LABELV $348
line 1119
;1119:}
LABELV $301
endproc DEMP2_AltRadiusDamage 8324 32
export DEMP2_AltDetonate
proc DEMP2_AltDetonate 24 12
line 1124
;1120:
;1121://---------------------------------------------------------
;1122:void DEMP2_AltDetonate( gentity_t *ent )
;1123://---------------------------------------------------------
;1124:{
line 1127
;1125:	gentity_t *efEnt;
;1126:
;1127:	G_SetOrigin( ent, ent->r.currentOrigin );
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1128
;1128:	if (!ent->pos1[0] && !ent->pos1[1] && !ent->pos1[2])
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
CNSTF4 0
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 548
ADDP4
INDIRF4
ADDRLP4 12
INDIRF4
NEF4 $352
ADDRLP4 8
INDIRP4
CNSTI4 552
ADDP4
INDIRF4
ADDRLP4 12
INDIRF4
NEF4 $352
ADDRLP4 8
INDIRP4
CNSTI4 556
ADDP4
INDIRF4
ADDRLP4 12
INDIRF4
NEF4 $352
line 1129
;1129:	{ //don't play effect with a 0'd out directional vector
line 1130
;1130:		ent->pos1[1] = 1;
ADDRFP4 0
INDIRP4
CNSTI4 552
ADDP4
CNSTF4 1065353216
ASGNF4
line 1131
;1131:	}
LABELV $352
line 1133
;1132:	//Let's just save ourself some bandwidth and play both the effect and sphere spawn in 1 event
;1133:	efEnt = G_PlayEffect( EFFECT_EXPLOSION_DEMP2ALT, ent->r.currentOrigin, ent->pos1 );
CNSTI4 9
ARGI4
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 16
INDIRP4
CNSTI4 548
ADDP4
ARGP4
ADDRLP4 20
ADDRGP4 G_PlayEffect
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
ASGNP4
line 1135
;1134:
;1135:	if (efEnt)
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $354
line 1136
;1136:	{
line 1137
;1137:		efEnt->s.weapon = ent->count*2;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 1
LSHI4
ASGNI4
line 1138
;1138:	}
LABELV $354
line 1140
;1139:
;1140:	ent->bolt_Head = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1141
;1141:	ent->bolt_LArm = 0;
ADDRFP4 0
INDIRP4
CNSTI4 788
ADDP4
CNSTI4 0
ASGNI4
line 1142
;1142:	ent->nextthink = level.time + 50;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 50
ADDI4
ASGNI4
line 1143
;1143:	ent->think = DEMP2_AltRadiusDamage;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 DEMP2_AltRadiusDamage
ASGNP4
line 1144
;1144:	ent->s.eType = ET_GENERAL; // make us a missile no longer
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 1145
;1145:}
LABELV $351
endproc DEMP2_AltDetonate 24 12
proc WP_DEMP2_AltFire 1152 28
line 1150
;1146:
;1147://---------------------------------------------------------
;1148:static void WP_DEMP2_AltFire( gentity_t *ent )
;1149://---------------------------------------------------------
;1150:{
line 1151
;1151:	int		damage	= DEMP2_ALT_DAMAGE;
ADDRLP4 32
CNSTI4 8
ASGNI4
line 1158
;1152:	int		count, origcount;
;1153:	float	fact;
;1154:	vec3_t	start, end;
;1155:	trace_t	tr;
;1156:	gentity_t *missile;
;1157:
;1158:	VectorCopy( muzzle, start );
ADDRLP4 8
ADDRGP4 muzzle
INDIRB
ASGNB 12
line 1160
;1159:
;1160:	VectorMA( start, DEMP2_ALT_RANGE, forward, end );
ADDRLP4 1124
CNSTF4 1166016512
ASGNF4
ADDRLP4 20
ADDRLP4 8
INDIRF4
ADDRLP4 1124
INDIRF4
ADDRGP4 forward
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 20+4
ADDRLP4 8+4
INDIRF4
ADDRLP4 1124
INDIRF4
ADDRGP4 forward+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 20+8
ADDRLP4 8+8
INDIRF4
CNSTF4 1166016512
ADDRGP4 forward+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 1162
;1161:
;1162:	count = ( level.time - ent->client->ps.weaponChargeTime ) / DEMP2_CHARGE_UNIT;
ADDRLP4 4
ADDRGP4 level+32
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
SUBI4
CVIF4 4
CNSTF4 1143930880
DIVF4
CVFI4 4
ASGNI4
line 1164
;1163:
;1164:	origcount = count;
ADDRLP4 1120
ADDRLP4 4
INDIRI4
ASGNI4
line 1166
;1165:
;1166:	if ( count < 1 )
ADDRLP4 4
INDIRI4
CNSTI4 1
GEI4 $366
line 1167
;1167:	{
line 1168
;1168:		count = 1;
ADDRLP4 4
CNSTI4 1
ASGNI4
line 1169
;1169:	}
ADDRGP4 $367
JUMPV
LABELV $366
line 1170
;1170:	else if ( count > 3 )
ADDRLP4 4
INDIRI4
CNSTI4 3
LEI4 $368
line 1171
;1171:	{
line 1172
;1172:		count = 3;
ADDRLP4 4
CNSTI4 3
ASGNI4
line 1173
;1173:	}
LABELV $368
LABELV $367
line 1175
;1174:
;1175:	fact = count*0.8;
ADDRLP4 36
CNSTF4 1061997773
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
ASGNF4
line 1176
;1176:	if (fact < 1)
ADDRLP4 36
INDIRF4
CNSTF4 1065353216
GEF4 $370
line 1177
;1177:	{
line 1178
;1178:		fact = 1;
ADDRLP4 36
CNSTF4 1065353216
ASGNF4
line 1179
;1179:	}
LABELV $370
line 1180
;1180:	damage *= fact;
ADDRLP4 32
ADDRLP4 32
INDIRI4
CVIF4 4
ADDRLP4 36
INDIRF4
MULF4
CVFI4 4
ASGNI4
line 1182
;1181:
;1182:	if (!origcount)
ADDRLP4 1120
INDIRI4
CNSTI4 0
NEI4 $372
line 1183
;1183:	{ //this was just a tap-fire
line 1184
;1184:		damage = 1;
ADDRLP4 32
CNSTI4 1
ASGNI4
line 1185
;1185:	}
LABELV $372
line 1189
;1186:
;1187:	//damage *= ( 1 + ( count * ( count - 1 )));// yields damage of 12,36,84...gives a higher bonus for longer charge
;1188:
;1189:	trap_Trace( &tr, start, NULL, NULL, end, ent->s.number, MASK_SHOT);
ADDRLP4 40
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 1128
CNSTP4 0
ASGNP4
ADDRLP4 1128
INDIRP4
ARGP4
ADDRLP4 1128
INDIRP4
ARGP4
ADDRLP4 20
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
line 1194
;1190:
;1191:	// we treat the trace fraction like it's a time value, meaning that the shot can travel a whopping 4096 units in 1 second
;1192:
;1193:	//missile = CreateMissile( start, forward, DEMP2_ALT_RANGE, tr.fraction * 1000/*time*/, ent, qtrue );
;1194:	missile = G_Spawn();
ADDRLP4 1132
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 1132
INDIRP4
ASGNP4
line 1195
;1195:	G_SetOrigin(missile, tr.endpos);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 40+12
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1198
;1196:	//rww - I guess it's rather pointless making it a missile anyway, at least for MP.
;1197:
;1198:	VectorCopy( tr.plane.normal, missile->pos1 );
ADDRLP4 0
INDIRP4
CNSTI4 548
ADDP4
ADDRLP4 40+24
INDIRB
ASGNB 12
line 1200
;1199:
;1200:	missile->count = count;
ADDRLP4 0
INDIRP4
CNSTI4 712
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 1202
;1201:
;1202:	missile->classname = "demp2_alt_proj";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $376
ASGNP4
line 1203
;1203:	missile->s.weapon = WP_DEMP2;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 8
ASGNI4
line 1205
;1204:
;1205:	missile->think = DEMP2_AltDetonate;
ADDRLP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 DEMP2_AltDetonate
ASGNP4
line 1206
;1206:	missile->nextthink = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1208
;1207:
;1208:	missile->splashDamage = missile->damage = damage;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRLP4 32
INDIRI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 696
ADDP4
ADDRLP4 32
INDIRI4
ASGNI4
line 1209
;1209:	missile->splashMethodOfDeath = missile->methodOfDeath = MOD_DEMP2;
ADDRLP4 1148
CNSTI4 14
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
ADDRLP4 1148
INDIRI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 708
ADDP4
ADDRLP4 1148
INDIRI4
ASGNI4
line 1210
;1210:	missile->splashRadius = DEMP2_ALT_SPLASHRADIUS;
ADDRLP4 0
INDIRP4
CNSTI4 700
ADDP4
CNSTI4 256
ASGNI4
line 1212
;1211:
;1212:	missile->r.ownerNum = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 396
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 1214
;1213:
;1214:	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
CNSTI4 128
ASGNI4
line 1215
;1215:	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 262913
ASGNI4
line 1218
;1216:
;1217:	// we don't want it to ever bounce
;1218:	missile->bounceCount = 0;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 0
ASGNI4
line 1219
;1219:}
LABELV $358
endproc WP_DEMP2_AltFire 1152 28
proc WP_FireDEMP2 0 4
line 1224
;1220:
;1221://---------------------------------------------------------
;1222:static void WP_FireDEMP2( gentity_t *ent, qboolean altFire )
;1223://---------------------------------------------------------
;1224:{
line 1225
;1225:	if ( altFire )
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $379
line 1226
;1226:	{
line 1227
;1227:		WP_DEMP2_AltFire( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 WP_DEMP2_AltFire
CALLV
pop
line 1228
;1228:	}
ADDRGP4 $380
JUMPV
LABELV $379
line 1230
;1229:	else
;1230:	{
line 1231
;1231:		WP_DEMP2_MainFire( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 WP_DEMP2_MainFire
CALLV
pop
line 1232
;1232:	}
LABELV $380
line 1233
;1233:}
LABELV $378
endproc WP_FireDEMP2 0 4
proc WP_FlechetteMainFire 60 24
line 1248
;1234:
;1235:
;1236:
;1237:/*
;1238:======================================================================
;1239:
;1240:FLECHETTE
;1241:
;1242:======================================================================
;1243:*/
;1244:
;1245://---------------------------------------------------------
;1246:static void WP_FlechetteMainFire( gentity_t *ent )
;1247://---------------------------------------------------------
;1248:{
line 1253
;1249:	vec3_t		fwd, angs;
;1250:	gentity_t	*missile;
;1251:	int i;
;1252:
;1253:	for (i = 0; i < FLECHETTE_SHOTS; i++ )
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $382
line 1254
;1254:	{
line 1255
;1255:		vectoangles( forward, angs );
ADDRGP4 forward
ARGP4
ADDRLP4 8
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1257
;1256:
;1257:		if ( i == 0 )
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $386
line 1258
;1258:		{
line 1260
;1259:			// do nothing on the first shot, this one will hit the crosshairs
;1260:		}
ADDRGP4 $387
JUMPV
LABELV $386
line 1262
;1261:		else
;1262:		{
line 1263
;1263:			angs[PITCH] += crandom() * FLECHETTE_SPREAD;
ADDRLP4 32
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 8
INDIRF4
CNSTF4 1082130432
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
ADDF4
ASGNF4
line 1264
;1264:			angs[YAW]	+= crandom() * FLECHETTE_SPREAD;
ADDRLP4 36
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 8+4
ADDRLP4 8+4
INDIRF4
CNSTF4 1082130432
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
MULF4
ADDF4
ASGNF4
line 1265
;1265:		}
LABELV $387
line 1267
;1266:
;1267:		AngleVectors( angs, fwd, NULL, NULL );
ADDRLP4 8
ARGP4
ADDRLP4 20
ARGP4
ADDRLP4 32
CNSTP4 0
ASGNP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRLP4 32
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 1269
;1268:
;1269:		missile = CreateMissile( muzzle, fwd, FLECHETTE_VEL, 10000, ent, qfalse);
ADDRGP4 muzzle
ARGP4
ADDRLP4 20
ARGP4
CNSTF4 1163575296
ARGF4
CNSTI4 10000
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 36
ADDRGP4 CreateMissile
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 36
INDIRP4
ASGNP4
line 1271
;1270:
;1271:		missile->classname = "flech_proj";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $389
ASGNP4
line 1272
;1272:		missile->s.weapon = WP_FLECHETTE;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 9
ASGNI4
line 1274
;1273:
;1274:		VectorSet( missile->r.maxs, FLECHETTE_SIZE, FLECHETTE_SIZE, FLECHETTE_SIZE );
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1065353216
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1065353216
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1065353216
ASGNF4
line 1275
;1275:		VectorScale( missile->r.maxs, -1, missile->r.mins );
ADDRLP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
INDIRF4
MULF4
ASGNF4
line 1277
;1276:
;1277:		missile->damage = FLECHETTE_DAMAGE;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
CNSTI4 12
ASGNI4
line 1278
;1278:		missile->dflags = DAMAGE_DEATH_KNOCKBACK;// | DAMAGE_EXTRA_KNOCKBACK;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
CNSTI4 128
ASGNI4
line 1279
;1279:		missile->methodOfDeath = MOD_FLECHETTE;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 16
ASGNI4
line 1280
;1280:		missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 262913
ASGNI4
line 1283
;1281:
;1282:		// we don't want it to bounce forever
;1283:		missile->bounceCount = Q_irand(5,8);
CNSTI4 5
ARGI4
CNSTI4 8
ARGI4
ADDRLP4 52
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
ADDRLP4 52
INDIRI4
ASGNI4
line 1285
;1284:
;1285:		missile->s.eFlags |= EF_BOUNCE_SHRAPNEL;
ADDRLP4 56
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 1286
;1286:	}
LABELV $383
line 1253
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 5
LTI4 $382
line 1287
;1287:}
LABELV $381
endproc WP_FlechetteMainFire 60 24
export prox_mine_think
proc prox_mine_think 32 20
line 1292
;1288:
;1289://---------------------------------------------------------
;1290:void prox_mine_think( gentity_t *ent )
;1291://---------------------------------------------------------
;1292:{
line 1294
;1293:	int			count, i;
;1294:	qboolean	blow = qfalse;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 1297
;1295:
;1296:	// if it isn't time to auto-explode, do a small proximity check
;1297:	if ( ent->delay > level.time )
ADDRFP4 0
INDIRP4
CNSTI4 764
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
LEI4 $391
line 1298
;1298:	{
line 1299
;1299:		count = G_RadiusList( ent->r.currentOrigin, FLECHETTE_MINE_RADIUS_CHECK, ent, qtrue, ent_list );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 368
ADDP4
ARGP4
CNSTF4 1132462080
ARGF4
ADDRLP4 12
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 ent_list
ARGP4
ADDRLP4 16
ADDRGP4 G_RadiusList
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRI4
ASGNI4
line 1301
;1300:
;1301:		for ( i = 0; i < count; i++ )
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $397
JUMPV
LABELV $394
line 1302
;1302:		{
line 1303
;1303:			if ( ent_list[i]->client && ent_list[i]->health > 0 && ent->activator && ent_list[i]->s.number != ent->activator->s.number )
ADDRLP4 20
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ent_list
ADDP4
INDIRP4
ASGNP4
ADDRLP4 24
CNSTU4 0
ASGNU4
ADDRLP4 20
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 24
INDIRU4
EQU4 $398
ADDRLP4 20
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 0
LEI4 $398
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CVPU4 4
ADDRLP4 24
INDIRU4
EQU4 $398
ADDRLP4 20
INDIRP4
INDIRI4
ADDRLP4 28
INDIRP4
INDIRI4
EQI4 $398
line 1304
;1304:			{
line 1305
;1305:				blow = qtrue;
ADDRLP4 8
CNSTI4 1
ASGNI4
line 1306
;1306:				break;
ADDRGP4 $392
JUMPV
LABELV $398
line 1308
;1307:			}
;1308:		}
LABELV $395
line 1301
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $397
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $394
line 1309
;1309:	}
ADDRGP4 $392
JUMPV
LABELV $391
line 1311
;1310:	else
;1311:	{
line 1313
;1312:		// well, we must die now
;1313:		blow = qtrue;
ADDRLP4 8
CNSTI4 1
ASGNI4
line 1314
;1314:	}
LABELV $392
line 1316
;1315:
;1316:	if ( blow )
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $400
line 1317
;1317:	{
line 1319
;1318:		//G_Sound( ent, G_SoundIndex( "sound/weapons/flechette/warning.wav" ));
;1319:		ent->think = laserTrapExplode;//thinkF_WP_Explode;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 laserTrapExplode
ASGNP4
line 1320
;1320:		ent->nextthink = level.time + 200;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 200
ADDI4
ASGNI4
line 1321
;1321:	}
ADDRGP4 $401
JUMPV
LABELV $400
line 1323
;1322:	else
;1323:	{
line 1325
;1324:		// we probably don't need to do this thinking logic very often...maybe this is fast enough?
;1325:		ent->nextthink = level.time + 500;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 500
ADDI4
ASGNI4
line 1326
;1326:	}
LABELV $401
line 1327
;1327:}
LABELV $390
endproc prox_mine_think 32 20
proc WP_TraceSetStart 1132 28
line 1332
;1328:
;1329://-----------------------------------------------------------------------------
;1330:static void WP_TraceSetStart( gentity_t *ent, vec3_t start, vec3_t mins, vec3_t maxs )
;1331://-----------------------------------------------------------------------------
;1332:{
line 1338
;1333:	//make sure our start point isn't on the other side of a wall
;1334:	trace_t	tr;
;1335:	vec3_t	entMins;
;1336:	vec3_t	entMaxs;
;1337:
;1338:	VectorAdd( ent->r.currentOrigin, ent->r.mins, entMins );
ADDRLP4 1104
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1080
ADDRLP4 1104
INDIRP4
CNSTI4 368
ADDP4
INDIRF4
ADDRLP4 1104
INDIRP4
CNSTI4 316
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 1080+4
ADDRLP4 1104
INDIRP4
CNSTI4 372
ADDP4
INDIRF4
ADDRLP4 1104
INDIRP4
CNSTI4 320
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 1108
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1080+8
ADDRLP4 1108
INDIRP4
CNSTI4 376
ADDP4
INDIRF4
ADDRLP4 1108
INDIRP4
CNSTI4 324
ADDP4
INDIRF4
ADDF4
ASGNF4
line 1339
;1339:	VectorAdd( ent->r.currentOrigin, ent->r.maxs, entMaxs );
ADDRLP4 1112
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1092
ADDRLP4 1112
INDIRP4
CNSTI4 368
ADDP4
INDIRF4
ADDRLP4 1112
INDIRP4
CNSTI4 328
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 1092+4
ADDRLP4 1112
INDIRP4
CNSTI4 372
ADDP4
INDIRF4
ADDRLP4 1112
INDIRP4
CNSTI4 332
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 1116
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1092+8
ADDRLP4 1116
INDIRP4
CNSTI4 376
ADDP4
INDIRF4
ADDRLP4 1116
INDIRP4
CNSTI4 336
ADDP4
INDIRF4
ADDF4
ASGNF4
line 1341
;1340:
;1341:	if ( G_BoxInBounds( start, mins, maxs, entMins, entMaxs ) )
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
ADDRLP4 1080
ARGP4
ADDRLP4 1092
ARGP4
ADDRLP4 1120
ADDRGP4 G_BoxInBounds
CALLI4
ASGNI4
ADDRLP4 1120
INDIRI4
CNSTI4 0
EQI4 $409
line 1342
;1342:	{
line 1343
;1343:		return;
ADDRGP4 $404
JUMPV
LABELV $409
line 1346
;1344:	}
;1345:
;1346:	if ( !ent->client )
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $411
line 1347
;1347:	{
line 1348
;1348:		return;
ADDRGP4 $404
JUMPV
LABELV $411
line 1351
;1349:	}
;1350:
;1351:	trap_Trace( &tr, ent->client->ps.origin, mins, maxs, start, ent->s.number, MASK_SOLID|CONTENTS_SHOTCLIP );
ADDRLP4 0
ARGP4
ADDRLP4 1124
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1124
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 1124
INDIRP4
INDIRI4
ARGI4
CNSTI4 129
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 1353
;1352:
;1353:	if ( tr.startsolid || tr.allsolid )
ADDRLP4 1128
CNSTI4 0
ASGNI4
ADDRLP4 0+4
INDIRI4
ADDRLP4 1128
INDIRI4
NEI4 $416
ADDRLP4 0
INDIRI4
ADDRLP4 1128
INDIRI4
EQI4 $413
LABELV $416
line 1354
;1354:	{
line 1355
;1355:		return;
ADDRGP4 $404
JUMPV
LABELV $413
line 1358
;1356:	}
;1357:
;1358:	if ( tr.fraction < 1.0f )
ADDRLP4 0+8
INDIRF4
CNSTF4 1065353216
GEF4 $417
line 1359
;1359:	{
line 1360
;1360:		VectorCopy( tr.endpos, start );
ADDRFP4 4
INDIRP4
ADDRLP4 0+12
INDIRB
ASGNB 12
line 1361
;1361:	}
LABELV $417
line 1362
;1362:}
LABELV $404
endproc WP_TraceSetStart 1132 28
export WP_ExplosiveDie
proc WP_ExplosiveDie 0 4
line 1365
;1363:
;1364:void WP_ExplosiveDie(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod)
;1365:{
line 1366
;1366:	laserTrapExplode(self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 laserTrapExplode
CALLV
pop
line 1367
;1367:}
LABELV $421
endproc WP_ExplosiveDie 0 4
export WP_flechette_alt_blow
proc WP_flechette_alt_blow 0 4
line 1372
;1368:
;1369://----------------------------------------------
;1370:void WP_flechette_alt_blow( gentity_t *ent )
;1371://----------------------------------------------
;1372:{
line 1379
;1373:	/*BG_EvaluateTrajectory( &ent->s.pos, level.time, ent->r.currentOrigin ); // Not sure if this is even necessary, but correct origins are cool?
;1374:
;1375:	G_RadiusDamage( ent->r.currentOrigin, &g_entities[ent->r.ownerNum], ent->splashDamage, ent->splashRadius, NULL, MOD_FLECHETTE_ALT_SPLASH );
;1376:	G_PlayEffect( "flechette/alt_blow", ent->currentOrigin );
;1377:	G_FreeEntity( ent );*/
;1378:
;1379:	ent->s.pos.trDelta[0] = 1;
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
CNSTF4 1065353216
ASGNF4
line 1380
;1380:	ent->s.pos.trDelta[1] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 0
ASGNF4
line 1381
;1381:	ent->s.pos.trDelta[2] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
CNSTF4 0
ASGNF4
line 1383
;1382:
;1383:	laserTrapExplode(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 laserTrapExplode
CALLV
pop
line 1384
;1384:}
LABELV $422
endproc WP_flechette_alt_blow 0 4
proc WP_CreateFlechetteBouncyThing 20 24
line 1389
;1385:
;1386://------------------------------------------------------------------------------
;1387:static void WP_CreateFlechetteBouncyThing( vec3_t start, vec3_t fwd, gentity_t *self )
;1388://------------------------------------------------------------------------------
;1389:{
line 1390
;1390:	gentity_t	*missile = CreateMissile( start, fwd, 700 + random() * 700, 1500 + random() * 2000, self, qtrue );
ADDRLP4 4
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 8
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
CNSTF4 1143930880
ADDRLP4 4
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
CNSTF4 1143930880
ADDF4
ARGF4
CNSTF4 1157234688
ADDRLP4 8
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
CNSTF4 1153138688
ADDF4
CVFI4 4
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 12
ADDRGP4 CreateMissile
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ASGNP4
line 1392
;1391:	
;1392:	missile->think = WP_flechette_alt_blow;
ADDRLP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 WP_flechette_alt_blow
ASGNP4
line 1394
;1393:
;1394:	missile->activator = self;
ADDRLP4 0
INDIRP4
CNSTI4 732
ADDP4
ADDRFP4 8
INDIRP4
ASGNP4
line 1396
;1395:
;1396:	missile->s.weapon = WP_FLECHETTE;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 9
ASGNI4
line 1397
;1397:	missile->classname = "flech_alt";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $424
ASGNP4
line 1398
;1398:	missile->mass = 4;
ADDRLP4 0
INDIRP4
CNSTI4 624
ADDP4
CNSTF4 1082130432
ASGNF4
line 1401
;1399:
;1400:	// How 'bout we give this thing a size...
;1401:	VectorSet( missile->r.mins, -3.0f, -3.0f, -3.0f );
ADDRLP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3225419776
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3225419776
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3225419776
ASGNF4
line 1402
;1402:	VectorSet( missile->r.maxs, 3.0f, 3.0f, 3.0f );
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1077936128
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1077936128
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1077936128
ASGNF4
line 1403
;1403:	missile->clipmask = MASK_SHOT;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 769
ASGNI4
line 1405
;1404:
;1405:	missile->touch = touch_NULL;
ADDRLP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 touch_NULL
ASGNP4
line 1408
;1406:
;1407:	// normal ones bounce, alt ones explode on impact
;1408:	missile->s.pos.trType = TR_GRAVITY;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 5
ASGNI4
line 1410
;1409:
;1410:	missile->s.eFlags |= (EF_BOUNCE_HALF|EF_ALT_FIRING);
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
CNSTI4 544
BORI4
ASGNI4
line 1412
;1411:
;1412:	missile->bounceCount = 50;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 50
ASGNI4
line 1414
;1413:
;1414:	missile->damage = FLECHETTE_ALT_DAMAGE;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
CNSTI4 60
ASGNI4
line 1415
;1415:	missile->dflags = 0;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
CNSTI4 0
ASGNI4
line 1416
;1416:	missile->splashDamage = FLECHETTE_ALT_SPLASH_DAM;
ADDRLP4 0
INDIRP4
CNSTI4 696
ADDP4
CNSTI4 60
ASGNI4
line 1417
;1417:	missile->splashRadius = FLECHETTE_ALT_SPLASH_RAD;
ADDRLP4 0
INDIRP4
CNSTI4 700
ADDP4
CNSTI4 128
ASGNI4
line 1419
;1418:
;1419:	missile->r.svFlags = SVF_USE_CURRENT_ORIGIN;
ADDRLP4 0
INDIRP4
CNSTI4 304
ADDP4
CNSTI4 128
ASGNI4
line 1421
;1420:
;1421:	missile->methodOfDeath = MOD_FLECHETTE_ALT_SPLASH;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 17
ASGNI4
line 1422
;1422:	missile->splashMethodOfDeath = MOD_FLECHETTE_ALT_SPLASH;
ADDRLP4 0
INDIRP4
CNSTI4 708
ADDP4
CNSTI4 17
ASGNI4
line 1425
;1423:	//missile->splashMethodOfDeath = MOD_UNKNOWN;//MOD_THERMAL_SPLASH;
;1424:
;1425:	VectorCopy( start, missile->pos2 );
ADDRLP4 0
INDIRP4
CNSTI4 560
ADDP4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 1426
;1426:}
LABELV $423
endproc WP_CreateFlechetteBouncyThing 20 24
proc WP_FlechetteAltFire 72 16
line 1431
;1427:
;1428://---------------------------------------------------------
;1429:static void WP_FlechetteAltFire( gentity_t *self )
;1430://---------------------------------------------------------
;1431:{
line 1435
;1432:	vec3_t 	dir, fwd, start, angs;
;1433:	int i;
;1434:
;1435:	vectoangles( forward, angs );
ADDRGP4 forward
ARGP4
ADDRLP4 40
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1436
;1436:	VectorCopy( muzzle, start );
ADDRLP4 28
ADDRGP4 muzzle
INDIRB
ASGNB 12
line 1438
;1437:
;1438:	WP_TraceSetStart( self, start, vec3_origin, vec3_origin );//make sure our start point isn't on the other side of a wall
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 28
ARGP4
ADDRLP4 52
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRGP4 WP_TraceSetStart
CALLV
pop
line 1440
;1439:
;1440:	for ( i = 0; i < 2; i++ )
ADDRLP4 12
CNSTI4 0
ASGNI4
LABELV $426
line 1441
;1441:	{
line 1442
;1442:		VectorCopy( angs, dir );
ADDRLP4 0
ADDRLP4 40
INDIRB
ASGNB 12
line 1444
;1443:
;1444:		dir[PITCH] -= random() * 4 + 8; // make it fly upwards
ADDRLP4 56
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1082130432
ADDRLP4 56
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
MULF4
CNSTF4 1090519040
ADDF4
SUBF4
ASGNF4
line 1445
;1445:		dir[YAW] += crandom() * 2;
ADDRLP4 60
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 64
CNSTF4 1073741824
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 64
INDIRF4
ADDRLP4 64
INDIRF4
ADDRLP4 60
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
ASGNF4
line 1446
;1446:		AngleVectors( dir, fwd, NULL, NULL );
ADDRLP4 0
ARGP4
ADDRLP4 16
ARGP4
ADDRLP4 68
CNSTP4 0
ASGNP4
ADDRLP4 68
INDIRP4
ARGP4
ADDRLP4 68
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 1448
;1447:
;1448:		WP_CreateFlechetteBouncyThing( start, fwd, self );
ADDRLP4 28
ARGP4
ADDRLP4 16
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 WP_CreateFlechetteBouncyThing
CALLV
pop
line 1449
;1449:	}
LABELV $427
line 1440
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 2
LTI4 $426
line 1450
;1450:}
LABELV $425
endproc WP_FlechetteAltFire 72 16
proc WP_FireFlechette 0 4
line 1455
;1451:
;1452://---------------------------------------------------------
;1453:static void WP_FireFlechette( gentity_t *ent, qboolean altFire )
;1454://---------------------------------------------------------
;1455:{
line 1456
;1456:	if ( altFire )
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $432
line 1457
;1457:	{
line 1459
;1458:		//WP_FlechetteProxMine( ent );
;1459:		WP_FlechetteAltFire(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 WP_FlechetteAltFire
CALLV
pop
line 1460
;1460:	}
ADDRGP4 $433
JUMPV
LABELV $432
line 1462
;1461:	else
;1462:	{
line 1463
;1463:		WP_FlechetteMainFire( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 WP_FlechetteMainFire
CALLV
pop
line 1464
;1464:	}
LABELV $433
line 1465
;1465:}
LABELV $431
endproc WP_FireFlechette 0 4
lit
align 4
LABELV $435
byte 4 0
byte 4 0
byte 4 1065353216
export rocketThink
code
proc rocketThink 124 12
line 1480
;1466:
;1467:
;1468:
;1469:/*
;1470:======================================================================
;1471:
;1472:ROCKET LAUNCHER
;1473:
;1474:======================================================================
;1475:*/
;1476:
;1477://---------------------------------------------------------
;1478:void rocketThink( gentity_t *ent )
;1479://---------------------------------------------------------
;1480:{
line 1482
;1481:	vec3_t newdir, targetdir, 
;1482:			up={0,0,1}, right; 
ADDRLP4 56
ADDRGP4 $435
INDIRB
ASGNB 12
line 1486
;1483:	vec3_t	org;
;1484:	float dot, dot2, dis;
;1485:	int i;
;1486:	float vel = ROCKET_VELOCITY;
ADDRLP4 40
CNSTF4 1147207680
ASGNF4
line 1488
;1487:
;1488:	if (!ent->enemy || !ent->enemy->client || ent->enemy->health < 1)
ADDRLP4 80
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
ASGNP4
ADDRLP4 84
CNSTU4 0
ASGNU4
ADDRLP4 80
INDIRP4
CVPU4 4
ADDRLP4 84
INDIRU4
EQU4 $439
ADDRLP4 80
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 84
INDIRU4
EQU4 $439
ADDRLP4 80
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 1
GEI4 $436
LABELV $439
line 1489
;1489:	{
line 1490
;1490:		ent->nextthink = level.time + 10000;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 10000
ADDI4
ASGNI4
line 1491
;1491:		ent->think = G_FreeEntity;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 G_FreeEntity
ASGNP4
line 1492
;1492:		return;
ADDRGP4 $434
JUMPV
LABELV $436
line 1495
;1493:	}
;1494:
;1495:	if ( ent->enemy && ent->enemy->inuse )
ADDRLP4 88
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
ASGNP4
ADDRLP4 88
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $441
ADDRLP4 88
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $441
line 1496
;1496:	{	
line 1497
;1497:		VectorCopy( ent->enemy->r.currentOrigin, org );
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 1498
;1498:		org[2] += (ent->enemy->r.mins[2] + ent->enemy->r.maxs[2]) * 0.5f;
ADDRLP4 92
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
INDIRP4
ASGNP4
ADDRLP4 28+8
ADDRLP4 28+8
INDIRF4
CNSTF4 1056964608
ADDRLP4 92
INDIRP4
CNSTI4 324
ADDP4
INDIRF4
ADDRLP4 92
INDIRP4
CNSTI4 336
ADDP4
INDIRF4
ADDF4
MULF4
ADDF4
ASGNF4
line 1500
;1499:
;1500:		VectorSubtract( org, ent->r.currentOrigin, targetdir );
ADDRLP4 96
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
ADDRLP4 28
INDIRF4
ADDRLP4 96
INDIRP4
CNSTI4 368
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 16+4
ADDRLP4 28+4
INDIRF4
ADDRLP4 96
INDIRP4
CNSTI4 372
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 16+8
ADDRLP4 28+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 376
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1501
;1501:		VectorNormalize( targetdir );
ADDRLP4 16
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 1504
;1502:
;1503:		// Now the rocket can't do a 180 in space, so we'll limit the turn to about 45 degrees.
;1504:		dot = DotProduct( targetdir, ent->movedir );
ADDRLP4 100
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 68
ADDRLP4 16
INDIRF4
ADDRLP4 100
INDIRP4
CNSTI4 612
ADDP4
INDIRF4
MULF4
ADDRLP4 16+4
INDIRF4
ADDRLP4 100
INDIRP4
CNSTI4 616
ADDP4
INDIRF4
MULF4
ADDF4
ADDRLP4 16+8
INDIRF4
ADDRLP4 100
INDIRP4
CNSTI4 620
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 1507
;1505:
;1506:		// a dot of 1.0 means right-on-target.
;1507:		if ( dot < 0.0f )
ADDRLP4 68
INDIRF4
CNSTF4 0
GEF4 $450
line 1508
;1508:		{	
line 1510
;1509:			// Go in the direction opposite, start a 180.
;1510:			CrossProduct( ent->movedir, up, right );
ADDRFP4 0
INDIRP4
CNSTI4 612
ADDP4
ARGP4
ADDRLP4 56
ARGP4
ADDRLP4 44
ARGP4
ADDRGP4 CrossProduct
CALLV
pop
line 1511
;1511:			dot2 = DotProduct( targetdir, right );
ADDRLP4 76
ADDRLP4 16
INDIRF4
ADDRLP4 44
INDIRF4
MULF4
ADDRLP4 16+4
INDIRF4
ADDRLP4 44+4
INDIRF4
MULF4
ADDF4
ADDRLP4 16+8
INDIRF4
ADDRLP4 44+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 1513
;1512:
;1513:			if ( dot2 > 0 )
ADDRLP4 76
INDIRF4
CNSTF4 0
LEF4 $456
line 1514
;1514:			{	
line 1516
;1515:				// Turn 45 degrees right.
;1516:				VectorMA( ent->movedir, 0.4f, right, newdir );
ADDRLP4 104
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 108
CNSTF4 1053609165
ASGNF4
ADDRLP4 4
ADDRLP4 104
INDIRP4
CNSTI4 612
ADDP4
INDIRF4
ADDRLP4 108
INDIRF4
ADDRLP4 44
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 104
INDIRP4
CNSTI4 616
ADDP4
INDIRF4
ADDRLP4 108
INDIRF4
ADDRLP4 44+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 4+8
ADDRFP4 0
INDIRP4
CNSTI4 620
ADDP4
INDIRF4
CNSTF4 1053609165
ADDRLP4 44+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 1517
;1517:			}
ADDRGP4 $457
JUMPV
LABELV $456
line 1519
;1518:			else
;1519:			{	
line 1521
;1520:				// Turn 45 degrees left.
;1521:				VectorMA(ent->movedir, -0.4f, right, newdir);
ADDRLP4 104
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 108
CNSTF4 3201092813
ASGNF4
ADDRLP4 4
ADDRLP4 104
INDIRP4
CNSTI4 612
ADDP4
INDIRF4
ADDRLP4 108
INDIRF4
ADDRLP4 44
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 104
INDIRP4
CNSTI4 616
ADDP4
INDIRF4
ADDRLP4 108
INDIRF4
ADDRLP4 44+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 4+8
ADDRFP4 0
INDIRP4
CNSTI4 620
ADDP4
INDIRF4
CNSTF4 3201092813
ADDRLP4 44+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 1522
;1522:			}
LABELV $457
line 1525
;1523:
;1524:			// Yeah we've adjusted horizontally, but let's split the difference vertically, so we kinda try to move towards it.
;1525:			newdir[2] = ( targetdir[2] + ent->movedir[2] ) * 0.5;
ADDRLP4 4+8
CNSTF4 1056964608
ADDRLP4 16+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 620
ADDP4
INDIRF4
ADDF4
MULF4
ASGNF4
line 1528
;1526:
;1527:			// let's also slow down a lot
;1528:			vel *= 0.5f;
ADDRLP4 40
CNSTF4 1056964608
ADDRLP4 40
INDIRF4
MULF4
ASGNF4
line 1529
;1529:		}
ADDRGP4 $451
JUMPV
LABELV $450
line 1530
;1530:		else if ( dot < 0.70f )
ADDRLP4 68
INDIRF4
CNSTF4 1060320051
GEF4 $468
line 1531
;1531:		{	
line 1533
;1532:			// Still a bit off, so we turn a bit softer
;1533:			VectorMA( ent->movedir, 0.5f, targetdir, newdir );
ADDRLP4 104
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 108
CNSTF4 1056964608
ASGNF4
ADDRLP4 4
ADDRLP4 104
INDIRP4
CNSTI4 612
ADDP4
INDIRF4
ADDRLP4 108
INDIRF4
ADDRLP4 16
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 104
INDIRP4
CNSTI4 616
ADDP4
INDIRF4
ADDRLP4 108
INDIRF4
ADDRLP4 16+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 4+8
ADDRFP4 0
INDIRP4
CNSTI4 620
ADDP4
INDIRF4
CNSTF4 1056964608
ADDRLP4 16+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 1534
;1534:		}
ADDRGP4 $469
JUMPV
LABELV $468
line 1536
;1535:		else
;1536:		{	
line 1538
;1537:			// getting close, so turn a bit harder
;1538:			VectorMA( ent->movedir, 0.9f, targetdir, newdir );
ADDRLP4 104
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 108
CNSTF4 1063675494
ASGNF4
ADDRLP4 4
ADDRLP4 104
INDIRP4
CNSTI4 612
ADDP4
INDIRF4
ADDRLP4 108
INDIRF4
ADDRLP4 16
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 104
INDIRP4
CNSTI4 616
ADDP4
INDIRF4
ADDRLP4 108
INDIRF4
ADDRLP4 16+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 4+8
ADDRFP4 0
INDIRP4
CNSTI4 620
ADDP4
INDIRF4
CNSTF4 1063675494
ADDRLP4 16+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 1539
;1539:		}
LABELV $469
LABELV $451
line 1542
;1540:
;1541:		// add crazy drunkenness
;1542:		for (i = 0; i < 3; i++ )
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $478
line 1543
;1543:		{
line 1544
;1544:			newdir[i] += crandom() * ent->random * 0.25f;
ADDRLP4 104
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 108
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 4
ADDP4
ASGNP4
ADDRLP4 108
INDIRP4
ADDRLP4 108
INDIRP4
INDIRF4
CNSTF4 1048576000
CNSTF4 1073741824
ADDRLP4 104
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
MULF4
ADDF4
ASGNF4
line 1545
;1545:		}
LABELV $479
line 1542
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $478
line 1548
;1546:
;1547:		// decay the randomness
;1548:		ent->random *= 0.9f;
ADDRLP4 104
ADDRFP4 0
INDIRP4
CNSTI4 760
ADDP4
ASGNP4
ADDRLP4 104
INDIRP4
CNSTF4 1063675494
ADDRLP4 104
INDIRP4
INDIRF4
MULF4
ASGNF4
line 1551
;1549:
;1550:		// Try to crash into the ground if we get close enough to do splash damage
;1551:		dis = Distance( ent->r.currentOrigin, org );
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 28
ARGP4
ADDRLP4 108
ADDRGP4 Distance
CALLF4
ASGNF4
ADDRLP4 72
ADDRLP4 108
INDIRF4
ASGNF4
line 1553
;1552:
;1553:		if ( dis < 128 )
ADDRLP4 72
INDIRF4
CNSTF4 1124073472
GEF4 $482
line 1554
;1554:		{
line 1556
;1555:			// the closer we get, the more we push the rocket down, heh heh.
;1556:			newdir[2] -= (1.0f - (dis / 128.0f)) * 0.6f;
ADDRLP4 4+8
ADDRLP4 4+8
INDIRF4
CNSTF4 1058642330
CNSTF4 1065353216
ADDRLP4 72
INDIRF4
CNSTF4 1124073472
DIVF4
SUBF4
MULF4
SUBF4
ASGNF4
line 1557
;1557:		}
LABELV $482
line 1559
;1558:
;1559:		VectorNormalize( newdir );
ADDRLP4 4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 1561
;1560:
;1561:		VectorScale( newdir, vel * 0.5f, ent->s.pos.trDelta );
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 4
INDIRF4
CNSTF4 1056964608
ADDRLP4 40
INDIRF4
MULF4
MULF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 4+4
INDIRF4
CNSTF4 1056964608
ADDRLP4 40
INDIRF4
MULF4
MULF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 4+8
INDIRF4
CNSTF4 1056964608
ADDRLP4 40
INDIRF4
MULF4
MULF4
ASGNF4
line 1562
;1562:		VectorCopy( newdir, ent->movedir );
ADDRFP4 0
INDIRP4
CNSTI4 612
ADDP4
ADDRLP4 4
INDIRB
ASGNB 12
line 1563
;1563:		SnapVector( ent->s.pos.trDelta );			// save net bandwidth
ADDRLP4 112
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 112
INDIRP4
ADDRLP4 112
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 116
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 116
INDIRP4
ADDRLP4 116
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 120
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 120
INDIRP4
ADDRLP4 120
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
line 1564
;1564:		VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );
ADDRLP4 112
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 112
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 112
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 1565
;1565:		ent->s.pos.trTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1566
;1566:	}
LABELV $441
line 1568
;1567:
;1568:	ent->nextthink = level.time + ROCKET_ALT_THINK_TIME;	// Nothing at all spectacular happened, continue.
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 1569
;1569:	return;
LABELV $434
endproc rocketThink 124 12
proc WP_FireRocket 40 24
line 1575
;1570:}
;1571:
;1572://---------------------------------------------------------
;1573:static void WP_FireRocket( gentity_t *ent, qboolean altFire )
;1574://---------------------------------------------------------
;1575:{
line 1576
;1576:	int	damage	= ROCKET_DAMAGE;
ADDRLP4 16
CNSTI4 100
ASGNI4
line 1577
;1577:	int	vel = ROCKET_VELOCITY;
ADDRLP4 8
CNSTI4 900
ASGNI4
line 1578
;1578:	int dif = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 1582
;1579:	float rTime;
;1580:	gentity_t *missile;
;1581:
;1582:	if ( altFire )
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $490
line 1583
;1583:	{
line 1584
;1584:		vel *= 0.5f;
ADDRLP4 8
CNSTF4 1056964608
ADDRLP4 8
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 1585
;1585:	}
LABELV $490
line 1587
;1586:
;1587:	missile = CreateMissile( muzzle, forward, vel, 10000, ent, altFire );
ADDRGP4 muzzle
ARGP4
ADDRGP4 forward
ARGP4
ADDRLP4 8
INDIRI4
CVIF4 4
ARGF4
CNSTI4 10000
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 20
ADDRGP4 CreateMissile
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
ASGNP4
line 1589
;1588:
;1589:	if (ent->client && ent->client->ps.rocketLockIndex != MAX_CLIENTS)
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
EQU4 $492
ADDRLP4 24
INDIRP4
CNSTI4 580
ADDP4
INDIRI4
CNSTI4 32
EQI4 $492
line 1590
;1590:	{
line 1591
;1591:		rTime = ent->client->ps.rocketLockTime;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 588
ADDP4
INDIRF4
ASGNF4
line 1593
;1592:
;1593:		if (rTime == -1)
ADDRLP4 12
INDIRF4
CNSTF4 3212836864
NEF4 $494
line 1594
;1594:		{
line 1595
;1595:			rTime = ent->client->ps.rocketLastValidTime;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 584
ADDP4
INDIRF4
ASGNF4
line 1596
;1596:		}
LABELV $494
line 1597
;1597:		dif = ( level.time - rTime ) / ( 1200.0f / 16.0f );
ADDRLP4 4
ADDRGP4 level+32
INDIRI4
CVIF4 4
ADDRLP4 12
INDIRF4
SUBF4
CNSTF4 1117126656
DIVF4
CVFI4 4
ASGNI4
line 1599
;1598:
;1599:		if (dif < 0)
ADDRLP4 4
INDIRI4
CNSTI4 0
GEI4 $497
line 1600
;1600:		{
line 1601
;1601:			dif = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 1602
;1602:		}
LABELV $497
line 1605
;1603:
;1604:		//It's 10 even though it locks client-side at 8, because we want them to have a sturdy lock first, and because there's a slight difference in time between server and client
;1605:		if ( dif >= 10/* || random() * dif > 2 || random() > 0.97f*/ && rTime != -1 )
ADDRLP4 4
INDIRI4
CNSTI4 10
LTI4 $499
ADDRLP4 12
INDIRF4
CNSTF4 3212836864
EQF4 $499
line 1606
;1606:		{
line 1607
;1607:			missile->enemy = &g_entities[ent->client->ps.rocketLockIndex];
ADDRLP4 0
INDIRP4
CNSTI4 728
ADDP4
CNSTI4 828
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 580
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1609
;1608:
;1609:			if (missile->enemy && missile->enemy->client && missile->enemy->health > 0 && !OnSameTeam(ent, missile->enemy))
ADDRLP4 28
ADDRLP4 0
INDIRP4
CNSTI4 728
ADDP4
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
EQU4 $501
ADDRLP4 28
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 32
INDIRU4
EQU4 $501
ADDRLP4 28
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 0
LEI4 $501
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 OnSameTeam
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
NEI4 $501
line 1610
;1610:			{ //if enemy became invalid, died, or is on the same team, then don't seek it
line 1611
;1611:				missile->think = rocketThink;
ADDRLP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 rocketThink
ASGNP4
line 1612
;1612:				missile->nextthink = level.time + ROCKET_ALT_THINK_TIME;
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 1613
;1613:			}
LABELV $501
line 1614
;1614:		}
LABELV $499
line 1616
;1615:
;1616:		ent->client->ps.rocketLockIndex = MAX_CLIENTS;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 580
ADDP4
CNSTI4 32
ASGNI4
line 1617
;1617:		ent->client->ps.rocketLockTime = 0;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 588
ADDP4
CNSTF4 0
ASGNF4
line 1618
;1618:		ent->client->ps.rocketTargetTime = 0;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 592
ADDP4
CNSTF4 0
ASGNF4
line 1619
;1619:	}
LABELV $492
line 1621
;1620:
;1621:	missile->classname = "rocket_proj";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $504
ASGNP4
line 1622
;1622:	missile->s.weapon = WP_ROCKET_LAUNCHER;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 10
ASGNI4
line 1628
;1623:
;1624:	// NOTENOTE No mass yet.
;1625://	missile->mass = 10;
;1626:
;1627:	// Make it easier to hit things
;1628:	VectorSet( missile->r.maxs, ROCKET_SIZE, ROCKET_SIZE, ROCKET_SIZE );
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1077936128
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1077936128
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1077936128
ASGNF4
line 1629
;1629:	VectorScale( missile->r.maxs, -1, missile->r.mins );
ADDRLP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3212836864
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
INDIRF4
MULF4
ASGNF4
line 1631
;1630:
;1631:	missile->damage = damage;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
ADDRLP4 16
INDIRI4
ASGNI4
line 1632
;1632:	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
CNSTI4 128
ASGNI4
line 1633
;1633:	if (altFire)
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $505
line 1634
;1634:	{
line 1635
;1635:		missile->methodOfDeath = MOD_ROCKET_HOMING;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 20
ASGNI4
line 1636
;1636:		missile->splashMethodOfDeath = MOD_ROCKET_HOMING_SPLASH;
ADDRLP4 0
INDIRP4
CNSTI4 708
ADDP4
CNSTI4 21
ASGNI4
line 1637
;1637:	}
ADDRGP4 $506
JUMPV
LABELV $505
line 1639
;1638:	else
;1639:	{
line 1640
;1640:		missile->methodOfDeath = MOD_ROCKET;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 18
ASGNI4
line 1641
;1641:		missile->splashMethodOfDeath = MOD_ROCKET_SPLASH;
ADDRLP4 0
INDIRP4
CNSTI4 708
ADDP4
CNSTI4 19
ASGNI4
line 1642
;1642:	}
LABELV $506
line 1645
;1643:	
;1644:	//rww - We don't want rockets to be deflected, do we?
;1645:	missile->clipmask = MASK_SHOT;// | CONTENTS_LIGHTSABER;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 769
ASGNI4
line 1646
;1646:	missile->splashDamage = ROCKET_SPLASH_DAMAGE;
ADDRLP4 0
INDIRP4
CNSTI4 696
ADDP4
CNSTI4 100
ASGNI4
line 1647
;1647:	missile->splashRadius = ROCKET_SPLASH_RADIUS;
ADDRLP4 0
INDIRP4
CNSTI4 700
ADDP4
CNSTI4 160
ASGNI4
line 1650
;1648:
;1649:	// we don't want it to ever bounce
;1650:	missile->bounceCount = 0;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 0
ASGNI4
line 1651
;1651:}
LABELV $489
endproc WP_FireRocket 40 24
lit
align 4
LABELV $513
byte 4 0
byte 4 0
byte 4 1065353216
export thermalDetonatorExplode
code
proc thermalDetonatorExplode 40 24
line 1681
;1652:
;1653:/*
;1654:======================================================================
;1655:
;1656:THERMAL DETONATOR
;1657:
;1658:======================================================================
;1659:*/
;1660:
;1661:#define TD_DAMAGE			70 //only do 70 on a direct impact
;1662:#define TD_SPLASH_RAD		128
;1663:#define TD_SPLASH_DAM		90
;1664:#define TD_VELOCITY			900
;1665:#define TD_MIN_CHARGE		0.15f
;1666:#define TD_TIME				3000//6000
;1667:#define TD_ALT_TIME			3000
;1668:
;1669:#define TD_ALT_DAMAGE		60//100
;1670:#define TD_ALT_SPLASH_RAD	128
;1671:#define TD_ALT_SPLASH_DAM	50//90
;1672:#define TD_ALT_VELOCITY		600
;1673:#define TD_ALT_MIN_CHARGE	0.15f
;1674:#define TD_ALT_TIME			3000
;1675:
;1676:void thermalThinkStandard(gentity_t *ent);
;1677:
;1678://---------------------------------------------------------
;1679:void thermalDetonatorExplode( gentity_t *ent )
;1680://---------------------------------------------------------
;1681:{
line 1682
;1682:	if ( !ent->count )
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
NEI4 $508
line 1683
;1683:	{
line 1684
;1684:		G_Sound( ent, CHAN_VOICE, G_SoundIndex( "sound/weapons/thermal/warning.wav" ) );
ADDRGP4 $510
ARGP4
ADDRLP4 0
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 3
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 1685
;1685:		ent->count = 1;
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
CNSTI4 1
ASGNI4
line 1686
;1686:		ent->bolt_Head = level.time + 500;
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 500
ADDI4
ASGNI4
line 1687
;1687:		ent->think = thermalThinkStandard;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 thermalThinkStandard
ASGNP4
line 1688
;1688:		ent->nextthink = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1689
;1689:		ent->r.svFlags |= SVF_BROADCAST;//so everyone hears/sees the explosion?
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
CNSTI4 32
BORI4
ASGNI4
line 1690
;1690:	}
ADDRGP4 $509
JUMPV
LABELV $508
line 1692
;1691:	else
;1692:	{
line 1694
;1693:		vec3_t	origin;
;1694:		vec3_t	dir={0,0,1};
ADDRLP4 12
ADDRGP4 $513
INDIRB
ASGNB 12
line 1696
;1695:
;1696:		BG_EvaluateTrajectory( &ent->s.pos, level.time, origin );
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
ARGP4
ADDRGP4 level+32
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 BG_EvaluateTrajectory
CALLV
pop
line 1697
;1697:		origin[2] += 8;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1090519040
ADDF4
ASGNF4
line 1698
;1698:		SnapVector( origin );
ADDRLP4 0
ADDRLP4 0
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
line 1699
;1699:		G_SetOrigin( ent, origin );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1702
;1700:
;1701://		VectorSet( pos, ent->r.currentOrigin[0], ent->r.currentOrigin[1], ent->r.currentOrigin[2] + 8 );
;1702:		ent->s.eType = ET_GENERAL;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 1703
;1703:		G_AddEvent( ent, EV_MISSILE_MISS, DirToByte( dir ) );
ADDRLP4 12
ARGP4
ADDRLP4 24
ADDRGP4 DirToByte
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 74
ARGI4
ADDRLP4 24
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 1704
;1704:		ent->freeAfterEvent = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 492
ADDP4
CNSTI4 1
ASGNI4
line 1706
;1705:
;1706:		if (G_RadiusDamage( ent->r.currentOrigin, ent->parent,  ent->splashDamage, ent->splashRadius, 
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 28
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ARGP4
ADDRLP4 28
INDIRP4
CNSTI4 696
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 28
INDIRP4
CNSTI4 700
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 28
INDIRP4
CNSTI4 708
ADDP4
INDIRI4
ARGI4
ADDRLP4 32
ADDRGP4 G_RadiusDamage
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
EQI4 $520
line 1708
;1707:				ent, ent->splashMethodOfDeath))
;1708:		{
line 1709
;1709:			g_entities[ent->r.ownerNum].client->accuracy_hits++;
ADDRLP4 36
CNSTI4 828
ADDRFP4 0
INDIRP4
CNSTI4 396
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CNSTI4 1656
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1710
;1710:		}
LABELV $520
line 1712
;1711:
;1712:		trap_LinkEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1713
;1713:	}
LABELV $509
line 1714
;1714:}
LABELV $507
endproc thermalDetonatorExplode 40 24
export thermalThinkStandard
proc thermalThinkStandard 0 4
line 1717
;1715:
;1716:void thermalThinkStandard(gentity_t *ent)
;1717:{
line 1718
;1718:	if (ent->bolt_Head < level.time)
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $524
line 1719
;1719:	{
line 1720
;1720:		ent->think = thermalDetonatorExplode;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 thermalDetonatorExplode
ASGNP4
line 1721
;1721:		ent->nextthink = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1722
;1722:		return;
ADDRGP4 $523
JUMPV
LABELV $524
line 1725
;1723:	}
;1724:
;1725:	G_RunObject(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_RunObject
CALLV
pop
line 1726
;1726:	ent->nextthink = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1727
;1727:}
LABELV $523
endproc thermalThinkStandard 0 4
export WP_FireThermalDetonator
proc WP_FireThermalDetonator 56 16
line 1732
;1728:
;1729://---------------------------------------------------------
;1730:gentity_t *WP_FireThermalDetonator( gentity_t *ent, qboolean altFire )
;1731://---------------------------------------------------------
;1732:{
line 1735
;1733:	gentity_t	*bolt;
;1734:	vec3_t		dir, start;
;1735:	float chargeAmount = 1.0f; // default of full charge
ADDRLP4 4
CNSTF4 1065353216
ASGNF4
line 1737
;1736:	
;1737:	VectorCopy( forward, dir );
ADDRLP4 20
ADDRGP4 forward
INDIRB
ASGNB 12
line 1738
;1738:	VectorCopy( muzzle, start );
ADDRLP4 8
ADDRGP4 muzzle
INDIRB
ASGNB 12
line 1740
;1739:
;1740:	bolt = G_Spawn();
ADDRLP4 32
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 32
INDIRP4
ASGNP4
line 1742
;1741:	
;1742:	bolt->physicsObject = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 500
ADDP4
CNSTI4 1
ASGNI4
line 1744
;1743:
;1744:	bolt->classname = "thermal_detonator";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $530
ASGNP4
line 1745
;1745:	bolt->think = thermalThinkStandard;
ADDRLP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 thermalThinkStandard
ASGNP4
line 1746
;1746:	bolt->nextthink = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1747
;1747:	bolt->touch = touch_NULL;
ADDRLP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 touch_NULL
ASGNP4
line 1751
;1748://	bolt->mass = 10;		// NOTENOTE No mass implementation yet
;1749:
;1750:	// How 'bout we give this thing a size...
;1751:	VectorSet( bolt->r.mins, -3.0f, -3.0f, -3.0f );
ADDRLP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3225419776
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3225419776
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3225419776
ASGNF4
line 1752
;1752:	VectorSet( bolt->r.maxs, 3.0f, 3.0f, 3.0f );
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1077936128
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1077936128
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1077936128
ASGNF4
line 1753
;1753:	bolt->clipmask = MASK_SHOT;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 769
ASGNI4
line 1755
;1754:
;1755:	W_TraceSetStart( ent, start, bolt->r.mins, bolt->r.maxs );//make sure our start point isn't on the other side of a wall
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 316
ADDP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
ARGP4
ADDRGP4 W_TraceSetStart
CALLV
pop
line 1757
;1756:
;1757:	if ( ent->client )
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $532
line 1758
;1758:	{
line 1759
;1759:		chargeAmount = level.time - ent->client->ps.weaponChargeTime;
ADDRLP4 4
ADDRGP4 level+32
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 48
ADDP4
INDIRI4
SUBI4
CVIF4 4
ASGNF4
line 1760
;1760:	}
LABELV $532
line 1763
;1761:
;1762:	// get charge amount
;1763:	chargeAmount = chargeAmount / (float)TD_VELOCITY;
ADDRLP4 4
ADDRLP4 4
INDIRF4
CNSTF4 1147207680
DIVF4
ASGNF4
line 1765
;1764:
;1765:	if ( chargeAmount > 1.0f )
ADDRLP4 4
INDIRF4
CNSTF4 1065353216
LEF4 $535
line 1766
;1766:	{
line 1767
;1767:		chargeAmount = 1.0f;
ADDRLP4 4
CNSTF4 1065353216
ASGNF4
line 1768
;1768:	}
ADDRGP4 $536
JUMPV
LABELV $535
line 1769
;1769:	else if ( chargeAmount < TD_MIN_CHARGE )
ADDRLP4 4
INDIRF4
CNSTF4 1041865114
GEF4 $537
line 1770
;1770:	{
line 1771
;1771:		chargeAmount = TD_MIN_CHARGE;
ADDRLP4 4
CNSTF4 1041865114
ASGNF4
line 1772
;1772:	}
LABELV $537
LABELV $536
line 1775
;1773:
;1774:	// normal ones bounce, alt ones explode on impact
;1775:	bolt->bolt_Head = level.time + TD_TIME; // How long 'til she blows
ADDRLP4 0
INDIRP4
CNSTI4 784
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 3000
ADDI4
ASGNI4
line 1776
;1776:	bolt->s.pos.trType = TR_GRAVITY;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 5
ASGNI4
line 1777
;1777:	bolt->parent = ent;
ADDRLP4 0
INDIRP4
CNSTI4 536
ADDP4
ADDRFP4 0
INDIRP4
ASGNP4
line 1778
;1778:	bolt->r.ownerNum = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 396
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 1779
;1779:	VectorScale( dir, TD_VELOCITY * chargeAmount, bolt->s.pos.trDelta );
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 20
INDIRF4
CNSTF4 1147207680
ADDRLP4 4
INDIRF4
MULF4
MULF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 20+4
INDIRF4
CNSTF4 1147207680
ADDRLP4 4
INDIRF4
MULF4
MULF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 20+8
INDIRF4
CNSTF4 1147207680
ADDRLP4 4
INDIRF4
MULF4
MULF4
ASGNF4
line 1781
;1780:
;1781:	if ( ent->health >= 0 )
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 0
LTI4 $542
line 1782
;1782:	{
line 1783
;1783:		bolt->s.pos.trDelta[2] += 120;
ADDRLP4 40
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRF4
CNSTF4 1123024896
ADDF4
ASGNF4
line 1784
;1784:	}
LABELV $542
line 1786
;1785:
;1786:	if ( !altFire )
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $544
line 1787
;1787:	{
line 1789
;1788:		//bolt->alt_fire = qtrue;
;1789:		bolt->s.eFlags |= EF_BOUNCE_HALF;
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
CNSTI4 32
BORI4
ASGNI4
line 1790
;1790:	}
LABELV $544
line 1792
;1791:
;1792:	bolt->s.loopSound = G_SoundIndex( "sound/weapons/thermal/thermloop.wav" );
ADDRGP4 $546
ARGP4
ADDRLP4 40
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 200
ADDP4
ADDRLP4 40
INDIRI4
ASGNI4
line 1794
;1793:
;1794:	bolt->damage = TD_DAMAGE;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
CNSTI4 70
ASGNI4
line 1795
;1795:	bolt->dflags = 0;
ADDRLP4 0
INDIRP4
CNSTI4 692
ADDP4
CNSTI4 0
ASGNI4
line 1796
;1796:	bolt->splashDamage = TD_SPLASH_DAM;
ADDRLP4 0
INDIRP4
CNSTI4 696
ADDP4
CNSTI4 90
ASGNI4
line 1797
;1797:	bolt->splashRadius = TD_SPLASH_RAD;
ADDRLP4 0
INDIRP4
CNSTI4 700
ADDP4
CNSTI4 128
ASGNI4
line 1799
;1798:
;1799:	bolt->s.eType = ET_MISSILE;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 3
ASGNI4
line 1800
;1800:	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
ADDRLP4 0
INDIRP4
CNSTI4 304
ADDP4
CNSTI4 128
ASGNI4
line 1801
;1801:	bolt->s.weapon = WP_THERMAL;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 11
ASGNI4
line 1803
;1802:
;1803:	bolt->methodOfDeath = MOD_THERMAL;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 22
ASGNI4
line 1804
;1804:	bolt->splashMethodOfDeath = MOD_THERMAL_SPLASH;
ADDRLP4 0
INDIRP4
CNSTI4 708
ADDP4
CNSTI4 23
ASGNI4
line 1806
;1805:
;1806:	bolt->s.pos.trTime = level.time;		// move a bit on the very first frame
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1807
;1807:	VectorCopy( start, bolt->s.pos.trBase );
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 8
INDIRB
ASGNB 12
line 1809
;1808:	
;1809:	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
ADDRLP4 44
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 44
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 48
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 52
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 52
INDIRP4
ADDRLP4 52
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
line 1810
;1810:	VectorCopy (start, bolt->r.currentOrigin);
ADDRLP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDRLP4 8
INDIRB
ASGNB 12
line 1812
;1811:
;1812:	VectorCopy( start, bolt->pos2 );
ADDRLP4 0
INDIRP4
CNSTI4 560
ADDP4
ADDRLP4 8
INDIRB
ASGNB 12
line 1814
;1813:
;1814:	bolt->bounceCount = -5;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 -5
ASGNI4
line 1816
;1815:
;1816:	return bolt;
ADDRLP4 0
INDIRP4
RETP4
LABELV $529
endproc WP_FireThermalDetonator 56 16
export WP_DropThermal
proc WP_DropThermal 4 16
line 1820
;1817:}
;1818:
;1819:gentity_t *WP_DropThermal( gentity_t *ent )
;1820:{
line 1821
;1821:	AngleVectors( ent->client->ps.viewangles, forward, right, up );
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 156
ADDP4
ARGP4
ADDRGP4 forward
ARGP4
ADDRGP4 right
ARGP4
ADDRGP4 up
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 1822
;1822:	return (WP_FireThermalDetonator( ent, qfalse ));
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 0
ADDRGP4 WP_FireThermalDetonator
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
RETP4
LABELV $548
endproc WP_DropThermal 4 16
export laserTrapExplode
proc laserTrapExplode 16 24
line 1843
;1823:}
;1824:
;1825:
;1826:/*
;1827:======================================================================
;1828:
;1829:LASER TRAP / TRIP MINE
;1830:
;1831:======================================================================
;1832:*/
;1833:#define LT_DAMAGE			100
;1834:#define LT_SPLASH_RAD		256.0f
;1835:#define LT_SPLASH_DAM		105
;1836:#define LT_VELOCITY			900.0f
;1837:#define LT_SIZE				1.5f
;1838:#define LT_ALT_TIME			2000
;1839:#define	LT_ACTIVATION_DELAY	1000
;1840:#define	LT_DELAY_TIME		50
;1841:
;1842:void laserTrapExplode( gentity_t *self )
;1843:{
line 1846
;1844:	vec3_t v;
;1845:	//FIXME: damage some along line?
;1846:	self->takedamage = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 0
ASGNI4
line 1848
;1847:
;1848:	if (self->activator)
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $550
line 1849
;1849:	{
line 1850
;1850:		G_RadiusDamage( self->r.currentOrigin, self->activator, self->splashDamage, self->splashRadius, self, MOD_TRIP_MINE_SPLASH/*MOD_LT_SPLASH*/ );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 12
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
CNSTI4 696
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 700
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 12
INDIRP4
ARGP4
CNSTI4 24
ARGI4
ADDRGP4 G_RadiusDamage
CALLI4
pop
line 1851
;1851:	}
LABELV $550
line 1854
;1852:	//FIXME: clear me from owner's list of tripmines?
;1853:
;1854:	if (self->s.weapon != WP_FLECHETTE)
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 9
EQI4 $552
line 1855
;1855:	{
line 1856
;1856:		G_AddEvent( self, EV_MISSILE_MISS, 0);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 74
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 1857
;1857:	}
LABELV $552
line 1859
;1858:
;1859:	VectorCopy(self->s.pos.trDelta, v);
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
INDIRB
ASGNB 12
line 1862
;1860:	//Explode outward from the surface
;1861:
;1862:	if (self->s.time == -2)
ADDRFP4 0
INDIRP4
CNSTI4 84
ADDP4
INDIRI4
CNSTI4 -2
NEI4 $554
line 1863
;1863:	{
line 1864
;1864:		v[0] = 0;
ADDRLP4 0
CNSTF4 0
ASGNF4
line 1865
;1865:		v[1] = 0;
ADDRLP4 0+4
CNSTF4 0
ASGNF4
line 1866
;1866:		v[2] = 0;
ADDRLP4 0+8
CNSTF4 0
ASGNF4
line 1867
;1867:	}
LABELV $554
line 1869
;1868:
;1869:	if (self->s.weapon == WP_FLECHETTE)
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 9
NEI4 $558
line 1870
;1870:	{
line 1871
;1871:		G_PlayEffect(EFFECT_EXPLOSION_FLECHETTE, self->r.currentOrigin, v);
CNSTI4 7
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_PlayEffect
CALLP4
pop
line 1872
;1872:	}
ADDRGP4 $559
JUMPV
LABELV $558
line 1874
;1873:	else
;1874:	{
line 1875
;1875:		G_PlayEffect(EFFECT_EXPLOSION_TRIPMINE, self->r.currentOrigin, v);
CNSTI4 5
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_PlayEffect
CALLP4
pop
line 1876
;1876:	}
LABELV $559
line 1878
;1877:
;1878:	self->think = G_FreeEntity;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 G_FreeEntity
ASGNP4
line 1879
;1879:	self->nextthink = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1880
;1880:}
LABELV $549
endproc laserTrapExplode 16 24
export laserTrapDelayedExplode
proc laserTrapDelayedExplode 12 0
line 1883
;1881:
;1882:void laserTrapDelayedExplode( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath )
;1883:{
line 1884
;1884:	self->enemy = attacker;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRFP4 8
INDIRP4
ASGNP4
line 1885
;1885:	self->think = laserTrapExplode;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 laserTrapExplode
ASGNP4
line 1886
;1886:	self->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 1887
;1887:	self->takedamage = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 0
ASGNI4
line 1888
;1888:	if ( attacker && !attacker->s.number )
ADDRLP4 0
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $563
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 0
NEI4 $563
line 1889
;1889:	{
line 1891
;1890:		//less damage when shot by player
;1891:		self->splashDamage /= 3;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 696
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 3
DIVI4
ASGNI4
line 1892
;1892:		self->splashRadius /= 3;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 700
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 3
DIVI4
ASGNI4
line 1894
;1893:		//FIXME: different effect?
;1894:	}
LABELV $563
line 1895
;1895:}
LABELV $561
endproc laserTrapDelayedExplode 12 0
export touchLaserTrap
proc touchLaserTrap 8 12
line 1898
;1896:
;1897:void touchLaserTrap( gentity_t *ent, gentity_t *other, trace_t *trace )
;1898:{
line 1901
;1899:	// if the guy that touches this grenade can take damage, he's about to.
;1900:	//if ( other->takedamage )
;1901:	if (other && other->s.number < 1022)
ADDRLP4 0
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $566
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1022
GEI4 $566
line 1902
;1902:	{ //just explode if we hit any entity. This way we don't have things happening like tripmines floating
line 1904
;1903:	  //in the air after getting stuck to a moving door
;1904:		if ( ent->activator != other )
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CVPU4 4
ADDRFP4 4
INDIRP4
CVPU4 4
EQU4 $567
line 1905
;1905:		{
line 1906
;1906:			ent->touch = 0;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
CNSTP4 0
ASGNP4
line 1907
;1907:			ent->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 1908
;1908:			ent->think = laserTrapExplode;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 laserTrapExplode
ASGNP4
line 1909
;1909:			VectorCopy(trace->plane.normal, ent->s.pos.trDelta);
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRFP4 8
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 1910
;1910:		}
line 1911
;1911:	}
ADDRGP4 $567
JUMPV
LABELV $566
line 1913
;1912:	else
;1913:	{
line 1914
;1914:		ent->touch = 0;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
CNSTP4 0
ASGNP4
line 1915
;1915:		if (trace->entityNum != ENTITYNUM_NONE)
ADDRFP4 8
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
CNSTI4 1023
EQI4 $571
line 1916
;1916:		{
line 1917
;1917:			ent->enemy = &g_entities[trace->entityNum];
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
CNSTI4 828
ADDRFP4 8
INDIRP4
CNSTI4 52
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1918
;1918:		}
LABELV $571
line 1919
;1919:		laserTrapStick(ent, trace->endpos, trace->plane.normal);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CNSTI4 12
ADDP4
ARGP4
ADDRLP4 4
INDIRP4
CNSTI4 24
ADDP4
ARGP4
ADDRGP4 laserTrapStick
CALLV
pop
line 1920
;1920:	}
LABELV $567
line 1921
;1921:}
LABELV $565
endproc touchLaserTrap 8 12
export laserTrapThink
proc laserTrapThink 1116 28
line 1924
;1922:
;1923:void laserTrapThink ( gentity_t *ent )
;1924:{
line 1930
;1925:	gentity_t	*traceEnt;
;1926:	vec3_t		end;
;1927:	trace_t		tr;
;1928:
;1929:	//G_RunObject(ent);
;1930:	trap_LinkEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1933
;1931:
;1932:	//turn on the beam effect
;1933:	if ( !(ent->s.eFlags&EF_FIRING) )
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 256
BANDI4
CNSTI4 0
NEI4 $574
line 1934
;1934:	{//arm me
line 1935
;1935:		G_Sound( ent, CHAN_VOICE, G_SoundIndex( "sound/weapons/laser_trap/warning.wav" ) );
ADDRGP4 $576
ARGP4
ADDRLP4 1096
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 3
ARGI4
ADDRLP4 1096
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 1936
;1936:		ent->s.eFlags |= EF_FIRING;
ADDRLP4 1100
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 1100
INDIRP4
ADDRLP4 1100
INDIRP4
INDIRI4
CNSTI4 256
BORI4
ASGNI4
line 1937
;1937:	}
LABELV $574
line 1938
;1938:	ent->think = laserTrapThink;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 laserTrapThink
ASGNP4
line 1939
;1939:	ent->nextthink = level.time + FRAMETIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 1942
;1940:
;1941:	// Find the main impact point
;1942:	VectorMA ( ent->s.pos.trBase, 1024, ent->movedir, end );
ADDRLP4 1096
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1100
CNSTF4 1149239296
ASGNF4
ADDRLP4 0
ADDRLP4 1096
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 1100
INDIRF4
ADDRLP4 1096
INDIRP4
CNSTI4 612
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 1096
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 1100
INDIRF4
ADDRLP4 1096
INDIRP4
CNSTI4 616
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 1104
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0+8
ADDRLP4 1104
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
CNSTF4 1149239296
ADDRLP4 1104
INDIRP4
CNSTI4 620
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 1943
;1943:	trap_Trace ( &tr, ent->r.currentOrigin, NULL, NULL, end, ent->s.number, MASK_SHOT);
ADDRLP4 12
ARGP4
ADDRLP4 1108
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1108
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 1112
CNSTP4 0
ASGNP4
ADDRLP4 1112
INDIRP4
ARGP4
ADDRLP4 1112
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 1108
INDIRP4
INDIRI4
ARGI4
CNSTI4 769
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 1945
;1944:	
;1945:	traceEnt = &g_entities[ tr.entityNum ];
ADDRLP4 1092
CNSTI4 828
ADDRLP4 12+52
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1947
;1946:
;1947:	ent->s.time = -1; //let all clients know to draw a beam from this guy
ADDRFP4 0
INDIRP4
CNSTI4 84
ADDP4
CNSTI4 -1
ASGNI4
line 1949
;1948:
;1949:	if ( traceEnt->client || tr.startsolid )
ADDRLP4 1092
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $584
ADDRLP4 12+4
INDIRI4
CNSTI4 0
EQI4 $581
LABELV $584
line 1950
;1950:	{
line 1952
;1951:		//go boom
;1952:		ent->touch = 0;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
CNSTP4 0
ASGNP4
line 1953
;1953:		ent->nextthink = level.time + LT_DELAY_TIME;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 50
ADDI4
ASGNI4
line 1954
;1954:		ent->think = laserTrapExplode;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 laserTrapExplode
ASGNP4
line 1955
;1955:	}
LABELV $581
line 1956
;1956:}
LABELV $573
endproc laserTrapThink 1116 28
export laserTrapStick
proc laserTrapStick 32 12
line 1959
;1957:
;1958:void laserTrapStick( gentity_t *ent, vec3_t endpos, vec3_t normal )
;1959:{
line 1964
;1960:	//vec3_t	org;
;1961:
;1962:	// Back away from the wall
;1963:	//VectorMA( endpos, -1, normal, org );
;1964:	G_SetOrigin( ent, endpos );//org );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1965
;1965:	VectorCopy( normal, ent->pos1 );
ADDRFP4 0
INDIRP4
CNSTI4 548
ADDP4
ADDRFP4 8
INDIRP4
INDIRB
ASGNB 12
line 1967
;1966:
;1967:	VectorClear( ent->s.apos.trDelta );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
CNSTF4 0
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 80
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 76
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 72
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
line 1969
;1968:	// This will orient the object to face in the direction of the normal
;1969:	VectorCopy( normal, ent->s.pos.trDelta );
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRFP4 8
INDIRP4
INDIRB
ASGNB 12
line 1971
;1970:	//VectorScale( normal, -1, ent->s.pos.trDelta );
;1971:	ent->s.pos.trTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1975
;1972:	
;1973:	
;1974:	//This does nothing, cg_missile makes assumptions about direction of travel controlling angles
;1975:	vectoangles( normal, ent->s.apos.trBase );
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 1976
;1976:	VectorClear( ent->s.apos.trDelta );
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
CNSTF4 0
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 80
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 76
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 72
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
line 1977
;1977:	ent->s.apos.trType = TR_STATIONARY;
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 0
ASGNI4
line 1978
;1978:	VectorCopy( ent->s.apos.trBase, ent->s.angles );
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 116
ADDP4
ADDRLP4 16
INDIRP4
CNSTI4 60
ADDP4
INDIRB
ASGNB 12
line 1979
;1979:	VectorCopy( ent->s.angles, ent->r.currentAngles );
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CNSTI4 380
ADDP4
ADDRLP4 20
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 1982
;1980:	
;1981:
;1982:	G_Sound( ent, CHAN_VOICE, G_SoundIndex( "sound/weapons/laser_trap/stick.wav" ) );
ADDRGP4 $588
ARGP4
ADDRLP4 24
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 3
ARGI4
ADDRLP4 24
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 1983
;1983:	if ( ent->count )
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 0
EQI4 $589
line 1984
;1984:	{//a tripwire
line 1986
;1985:		//add draw line flag
;1986:		VectorCopy( normal, ent->movedir );
ADDRFP4 0
INDIRP4
CNSTI4 612
ADDP4
ADDRFP4 8
INDIRP4
INDIRB
ASGNB 12
line 1987
;1987:		ent->think = laserTrapThink;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 laserTrapThink
ASGNP4
line 1988
;1988:		ent->nextthink = level.time + LT_ACTIVATION_DELAY;//delay the activation
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1000
ADDI4
ASGNI4
line 1989
;1989:		ent->touch = touch_NULL;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 touch_NULL
ASGNP4
line 1991
;1990:		//make it shootable
;1991:		ent->takedamage = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 1
ASGNI4
line 1992
;1992:		ent->health = 5;
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 5
ASGNI4
line 1993
;1993:		ent->die = laserTrapDelayedExplode;
ADDRFP4 0
INDIRP4
CNSTI4 660
ADDP4
ADDRGP4 laserTrapDelayedExplode
ASGNP4
line 1996
;1994:
;1995:		//shove the box through the wall
;1996:		VectorSet( ent->r.mins, -LT_SIZE*2, -LT_SIZE*2, -LT_SIZE*2 );
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
line 1997
;1997:		VectorSet( ent->r.maxs, LT_SIZE*2, LT_SIZE*2, LT_SIZE*2 );
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
line 2000
;1998:
;1999:		//so that the owner can blow it up with projectiles
;2000:		ent->r.svFlags |= SVF_OWNERNOTSHARED;
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRI4
CNSTI4 4096
BORI4
ASGNI4
line 2001
;2001:	}
ADDRGP4 $590
JUMPV
LABELV $589
line 2003
;2002:	else
;2003:	{
line 2004
;2004:		ent->touch = touchLaserTrap;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 touchLaserTrap
ASGNP4
line 2005
;2005:		ent->think = laserTrapExplode;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 laserTrapExplode
ASGNP4
line 2006
;2006:		ent->nextthink = level.time + LT_ALT_TIME; // How long 'til she blows
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 2000
ADDI4
ASGNI4
line 2007
;2007:	}
LABELV $590
line 2008
;2008:}
LABELV $586
endproc laserTrapStick 32 12
export TrapThink
proc TrapThink 0 4
line 2011
;2009:
;2010:void TrapThink(gentity_t *ent)
;2011:{
line 2012
;2012:	ent->nextthink = level.time + 50;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 50
ADDI4
ASGNI4
line 2014
;2013:
;2014:	G_RunObject(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_RunObject
CALLV
pop
line 2015
;2015:}
LABELV $593
endproc TrapThink 0 4
export CreateLaserTrap
proc CreateLaserTrap 28 4
line 2018
;2016:
;2017:void CreateLaserTrap( gentity_t *laserTrap, vec3_t start, gentity_t *owner )
;2018:{
line 2019
;2019:	laserTrap->classname = "laserTrap";
ADDRFP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $596
ASGNP4
line 2020
;2020:	laserTrap->s.eFlags = EF_BOUNCE_HALF;
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 32
ASGNI4
line 2021
;2021:	laserTrap->s.eFlags |= EF_MISSILE_STICK;
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
CNSTI4 2097152
BORI4
ASGNI4
line 2022
;2022:	laserTrap->splashDamage = LT_SPLASH_DAM;//*2;
ADDRFP4 0
INDIRP4
CNSTI4 696
ADDP4
CNSTI4 105
ASGNI4
line 2023
;2023:	laserTrap->splashRadius = LT_SPLASH_RAD;//*2;
ADDRFP4 0
INDIRP4
CNSTI4 700
ADDP4
CNSTI4 256
ASGNI4
line 2024
;2024:	laserTrap->damage = LT_DAMAGE;//*DMG_VAR;
ADDRFP4 0
INDIRP4
CNSTI4 688
ADDP4
CNSTI4 100
ASGNI4
line 2025
;2025:	laserTrap->methodOfDeath = MOD_TRIP_MINE_SPLASH;//MOD_TRIP_WIRE;
ADDRFP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 24
ASGNI4
line 2026
;2026:	laserTrap->splashMethodOfDeath = MOD_TRIP_MINE_SPLASH;//MOD_TRIP_WIRE;
ADDRFP4 0
INDIRP4
CNSTI4 708
ADDP4
CNSTI4 24
ASGNI4
line 2027
;2027:	laserTrap->s.eType = ET_GENERAL;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 2028
;2028:	laserTrap->r.svFlags = SVF_USE_CURRENT_ORIGIN;
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
CNSTI4 128
ASGNI4
line 2029
;2029:	laserTrap->s.weapon = WP_TRIP_MINE;
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 12
ASGNI4
line 2030
;2030:	laserTrap->s.pos.trType = TR_GRAVITY;
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 5
ASGNI4
line 2031
;2031:	laserTrap->r.contents = MASK_SHOT;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 769
ASGNI4
line 2032
;2032:	laserTrap->parent = owner;
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
ADDRFP4 8
INDIRP4
ASGNP4
line 2033
;2033:	laserTrap->activator = owner;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ADDRFP4 8
INDIRP4
ASGNP4
line 2034
;2034:	laserTrap->r.ownerNum = owner->s.number;
ADDRFP4 0
INDIRP4
CNSTI4 396
ADDP4
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
line 2035
;2035:	VectorSet( laserTrap->r.mins, -LT_SIZE, -LT_SIZE, -LT_SIZE );
ADDRFP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3217031168
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3217031168
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3217031168
ASGNF4
line 2036
;2036:	VectorSet( laserTrap->r.maxs, LT_SIZE, LT_SIZE, LT_SIZE );
ADDRFP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1069547520
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1069547520
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1069547520
ASGNF4
line 2037
;2037:	laserTrap->clipmask = MASK_SHOT;
ADDRFP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 769
ASGNI4
line 2038
;2038:	laserTrap->s.solid = 2;
ADDRFP4 0
INDIRP4
CNSTI4 248
ADDP4
CNSTI4 2
ASGNI4
line 2039
;2039:	laserTrap->s.modelindex = G_ModelIndex( "models/weapons2/laser_trap/laser_trap_w.glm" );
ADDRGP4 $597
ARGP4
ADDRLP4 4
ADDRGP4 G_ModelIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 212
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 2040
;2040:	laserTrap->s.modelGhoul2 = 1;
ADDRFP4 0
INDIRP4
CNSTI4 204
ADDP4
CNSTI4 1
ASGNI4
line 2041
;2041:	laserTrap->s.g2radius = 40;
ADDRFP4 0
INDIRP4
CNSTI4 208
ADDP4
CNSTI4 40
ASGNI4
line 2043
;2042:
;2043:	laserTrap->s.genericenemyindex = owner->s.number+1024;
ADDRFP4 0
INDIRP4
CNSTI4 172
ADDP4
ADDRFP4 8
INDIRP4
INDIRI4
CNSTI4 1024
ADDI4
ASGNI4
line 2045
;2044:
;2045:	laserTrap->health = 1;
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 1
ASGNI4
line 2047
;2046:
;2047:	laserTrap->s.time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 84
ADDP4
CNSTI4 0
ASGNI4
line 2049
;2048:
;2049:	laserTrap->s.pos.trTime = level.time;		// move a bit on the very first frame
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 2050
;2050:	VectorCopy( start, laserTrap->s.pos.trBase );
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 2051
;2051:	SnapVector( laserTrap->s.pos.trBase );			// save net bandwidth
ADDRLP4 8
ADDRFP4 0
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
ADDRFP4 0
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
ADDRFP4 0
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
line 2053
;2052:	
;2053:	SnapVector( laserTrap->s.pos.trDelta );			// save net bandwidth
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 36
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
ADDRFP4 0
INDIRP4
CNSTI4 40
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
ADDRFP4 0
INDIRP4
CNSTI4 44
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
line 2054
;2054:	VectorCopy (start, laserTrap->r.currentOrigin);
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 2056
;2055:
;2056:	laserTrap->s.apos.trType = TR_GRAVITY;
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 5
ASGNI4
line 2057
;2057:	laserTrap->s.apos.trTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 52
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 2058
;2058:	laserTrap->s.apos.trBase[YAW] = rand()%360;
ADDRLP4 8
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
ADDRLP4 8
INDIRI4
CNSTI4 360
MODI4
CVIF4 4
ASGNF4
line 2059
;2059:	laserTrap->s.apos.trBase[PITCH] = rand()%360;
ADDRLP4 12
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 12
INDIRI4
CNSTI4 360
MODI4
CVIF4 4
ASGNF4
line 2060
;2060:	laserTrap->s.apos.trBase[ROLL] = rand()%360;
ADDRLP4 16
ADDRGP4 rand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 68
ADDP4
ADDRLP4 16
INDIRI4
CNSTI4 360
MODI4
CVIF4 4
ASGNF4
line 2062
;2061:
;2062:	if (rand()%10 < 5)
ADDRLP4 20
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 10
MODI4
CNSTI4 5
GEI4 $600
line 2063
;2063:	{
line 2064
;2064:		laserTrap->s.apos.trBase[YAW] = -laserTrap->s.apos.trBase[YAW];
ADDRLP4 24
ADDRFP4 0
INDIRP4
CNSTI4 64
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRF4
NEGF4
ASGNF4
line 2065
;2065:	}
LABELV $600
line 2067
;2066:
;2067:	VectorCopy( start, laserTrap->pos2 );
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 2068
;2068:	laserTrap->touch = touchLaserTrap;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 touchLaserTrap
ASGNP4
line 2069
;2069:	laserTrap->think = TrapThink;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 TrapThink
ASGNP4
line 2070
;2070:	laserTrap->nextthink = level.time + 50;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 50
ADDI4
ASGNI4
line 2071
;2071:}
LABELV $595
endproc CreateLaserTrap 28 4
lit
align 4
LABELV $604
byte 4 1023
skip 4092
export WP_PlaceLaserTrap
code
proc WP_PlaceLaserTrap 4156 12
line 2074
;2072:
;2073:void WP_PlaceLaserTrap( gentity_t *ent, qboolean alt_fire )
;2074:{
line 2076
;2075:	gentity_t	*laserTrap;
;2076:	gentity_t	*found = NULL;
ADDRLP4 4
CNSTP4 0
ASGNP4
line 2078
;2077:	vec3_t		dir, start;
;2078:	int			trapcount = 0;
ADDRLP4 4120
CNSTI4 0
ASGNI4
line 2079
;2079:	int			foundLaserTraps[MAX_GENTITIES] = {ENTITYNUM_NONE};
ADDRLP4 8
ADDRGP4 $604
INDIRB
ASGNB 4096
line 2087
;2080:	int			trapcount_org;
;2081:	int			lowestTimeStamp;
;2082:	int			removeMe;
;2083:	int			i;
;2084:
;2085:	//FIXME: surface must be within 64
;2086:
;2087:	VectorCopy( forward, dir );
ADDRLP4 4124
ADDRGP4 forward
INDIRB
ASGNB 12
line 2088
;2088:	VectorCopy( muzzle, start );
ADDRLP4 4136
ADDRGP4 muzzle
INDIRB
ASGNB 12
line 2090
;2089:
;2090:	laserTrap = G_Spawn();
ADDRLP4 4148
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 4108
ADDRLP4 4148
INDIRP4
ASGNP4
ADDRGP4 $606
JUMPV
LABELV $605
line 2095
;2091:	
;2092:	//limit to 10 placed at any one time
;2093:	//see how many there are now
;2094:	while ( (found = G_Find( found, FOFS(classname), "laserTrap" )) != NULL )
;2095:	{
line 2096
;2096:		if ( found->parent != ent )
ADDRLP4 4
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
EQU4 $608
line 2097
;2097:		{
line 2098
;2098:			continue;
ADDRGP4 $606
JUMPV
LABELV $608
line 2100
;2099:		}
;2100:		foundLaserTraps[trapcount++] = found->s.number;
ADDRLP4 4152
ADDRLP4 4120
INDIRI4
ASGNI4
ADDRLP4 4120
ADDRLP4 4152
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4152
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
ADDRLP4 4
INDIRP4
INDIRI4
ASGNI4
line 2101
;2101:	}
LABELV $606
line 2094
ADDRLP4 4
INDIRP4
ARGP4
CNSTI4 416
ARGI4
ADDRGP4 $596
ARGP4
ADDRLP4 4152
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 4152
INDIRP4
ASGNP4
ADDRLP4 4152
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $605
line 2103
;2102:	//now remove first ones we find until there are only 9 left
;2103:	found = NULL;
ADDRLP4 4
CNSTP4 0
ASGNP4
line 2104
;2104:	trapcount_org = trapcount;
ADDRLP4 4112
ADDRLP4 4120
INDIRI4
ASGNI4
line 2105
;2105:	lowestTimeStamp = level.time;
ADDRLP4 4104
ADDRGP4 level+32
INDIRI4
ASGNI4
ADDRGP4 $612
JUMPV
LABELV $611
line 2107
;2106:	while ( trapcount > 9 )
;2107:	{
line 2108
;2108:		removeMe = -1;
ADDRLP4 4116
CNSTI4 -1
ASGNI4
line 2109
;2109:		for ( i = 0; i < trapcount_org; i++ )
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $617
JUMPV
LABELV $614
line 2110
;2110:		{
line 2111
;2111:			if ( foundLaserTraps[i] == ENTITYNUM_NONE )
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRI4
CNSTI4 1023
NEI4 $618
line 2112
;2112:			{
line 2113
;2113:				continue;
ADDRGP4 $615
JUMPV
LABELV $618
line 2115
;2114:			}
;2115:			found = &g_entities[foundLaserTraps[i]];
ADDRLP4 4
CNSTI4 828
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 2116
;2116:			if ( laserTrap && found->setTime < lowestTimeStamp )
ADDRLP4 4108
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $620
ADDRLP4 4
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
ADDRLP4 4104
INDIRI4
GEI4 $620
line 2117
;2117:			{
line 2118
;2118:				removeMe = i;
ADDRLP4 4116
ADDRLP4 0
INDIRI4
ASGNI4
line 2119
;2119:				lowestTimeStamp = found->setTime;
ADDRLP4 4104
ADDRLP4 4
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
ASGNI4
line 2120
;2120:			}
LABELV $620
line 2121
;2121:		}
LABELV $615
line 2109
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $617
ADDRLP4 0
INDIRI4
ADDRLP4 4112
INDIRI4
LTI4 $614
line 2122
;2122:		if ( removeMe != -1 )
ADDRLP4 4116
INDIRI4
CNSTI4 -1
EQI4 $613
line 2123
;2123:		{
line 2125
;2124:			//remove it... or blow it?
;2125:			if ( &g_entities[foundLaserTraps[removeMe]] == NULL )
CNSTI4 828
ADDRLP4 4116
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
CVPU4 4
CNSTU4 0
NEU4 $624
line 2126
;2126:			{
line 2127
;2127:				break;
ADDRGP4 $613
JUMPV
LABELV $624
line 2130
;2128:			}
;2129:			else
;2130:			{
line 2131
;2131:				G_FreeEntity( &g_entities[foundLaserTraps[removeMe]] );
CNSTI4 828
ADDRLP4 4116
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 2132
;2132:			}
line 2133
;2133:			foundLaserTraps[removeMe] = ENTITYNUM_NONE;
ADDRLP4 4116
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
CNSTI4 1023
ASGNI4
line 2134
;2134:			trapcount--;
ADDRLP4 4120
ADDRLP4 4120
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2135
;2135:		}
line 2137
;2136:		else
;2137:		{
line 2138
;2138:			break;
LABELV $623
line 2140
;2139:		}
;2140:	}
LABELV $612
line 2106
ADDRLP4 4120
INDIRI4
CNSTI4 9
GTI4 $611
LABELV $613
line 2143
;2141:
;2142:	//now make the new one
;2143:	CreateLaserTrap( laserTrap, start, ent );
ADDRLP4 4108
INDIRP4
ARGP4
ADDRLP4 4136
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 CreateLaserTrap
CALLV
pop
line 2146
;2144:
;2145:	//set player-created-specific fields
;2146:	laserTrap->setTime = level.time;//remember when we placed it
ADDRLP4 4108
INDIRP4
CNSTI4 628
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 2148
;2147:
;2148:	if (!alt_fire)
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $627
line 2149
;2149:	{//tripwire
line 2150
;2150:		laserTrap->count = 1;
ADDRLP4 4108
INDIRP4
CNSTI4 712
ADDP4
CNSTI4 1
ASGNI4
line 2151
;2151:	}
LABELV $627
line 2154
;2152:
;2153:	//move it
;2154:	laserTrap->s.pos.trType = TR_GRAVITY;
ADDRLP4 4108
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 5
ASGNI4
line 2156
;2155:
;2156:	if (alt_fire)
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $629
line 2157
;2157:	{
line 2158
;2158:		VectorScale( dir, 512, laserTrap->s.pos.trDelta );
ADDRLP4 4108
INDIRP4
CNSTI4 36
ADDP4
CNSTF4 1140850688
ADDRLP4 4124
INDIRF4
MULF4
ASGNF4
ADDRLP4 4108
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1140850688
ADDRLP4 4124+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 4108
INDIRP4
CNSTI4 44
ADDP4
CNSTF4 1140850688
ADDRLP4 4124+8
INDIRF4
MULF4
ASGNF4
line 2159
;2159:	}
ADDRGP4 $630
JUMPV
LABELV $629
line 2161
;2160:	else
;2161:	{
line 2162
;2162:		VectorScale( dir, 256, laserTrap->s.pos.trDelta );
ADDRLP4 4108
INDIRP4
CNSTI4 36
ADDP4
CNSTF4 1132462080
ADDRLP4 4124
INDIRF4
MULF4
ASGNF4
ADDRLP4 4108
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1132462080
ADDRLP4 4124+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 4108
INDIRP4
CNSTI4 44
ADDP4
CNSTF4 1132462080
ADDRLP4 4124+8
INDIRF4
MULF4
ASGNF4
line 2163
;2163:	}
LABELV $630
line 2165
;2164:
;2165:	trap_LinkEntity(laserTrap);
ADDRLP4 4108
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 2166
;2166:}
LABELV $603
endproc WP_PlaceLaserTrap 4156 12
export VectorNPos
proc VectorNPos 4 0
line 2177
;2167:
;2168:
;2169:/*
;2170:======================================================================
;2171:
;2172:DET PACK
;2173:
;2174:======================================================================
;2175:*/
;2176:void VectorNPos(vec3_t in, vec3_t out)
;2177:{
line 2178
;2178:	if (in[0] < 0) { out[0] = -in[0]; } else { out[0] = in[0]; }
ADDRFP4 0
INDIRP4
INDIRF4
CNSTF4 0
GEF4 $636
ADDRFP4 4
INDIRP4
ADDRFP4 0
INDIRP4
INDIRF4
NEGF4
ASGNF4
ADDRGP4 $637
JUMPV
LABELV $636
ADDRFP4 4
INDIRP4
ADDRFP4 0
INDIRP4
INDIRF4
ASGNF4
LABELV $637
line 2179
;2179:	if (in[1] < 0) { out[1] = -in[1]; } else { out[1] = in[1]; }
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CNSTF4 0
GEF4 $638
ADDRLP4 0
CNSTI4 4
ASGNI4
ADDRFP4 4
INDIRP4
ADDRLP4 0
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRI4
ADDP4
INDIRF4
NEGF4
ASGNF4
ADDRGP4 $639
JUMPV
LABELV $638
ADDRLP4 0
CNSTI4 4
ASGNI4
ADDRFP4 4
INDIRP4
ADDRLP4 0
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRI4
ADDP4
INDIRF4
ASGNF4
LABELV $639
line 2180
;2180:	if (in[2] < 0) { out[2] = -in[2]; } else { out[2] = in[2]; }
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CNSTF4 0
GEF4 $640
ADDRLP4 0
CNSTI4 8
ASGNI4
ADDRFP4 4
INDIRP4
ADDRLP4 0
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRI4
ADDP4
INDIRF4
NEGF4
ASGNF4
ADDRGP4 $641
JUMPV
LABELV $640
ADDRLP4 0
CNSTI4 8
ASGNI4
ADDRFP4 4
INDIRP4
ADDRLP4 0
INDIRI4
ADDP4
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRI4
ADDP4
INDIRF4
ASGNF4
LABELV $641
line 2181
;2181:}
LABELV $635
endproc VectorNPos 4 0
export charge_stick
proc charge_stick 68 24
line 2186
;2182:
;2183:void DetPackBlow(gentity_t *self);
;2184:
;2185:void charge_stick (gentity_t *self, gentity_t *other, trace_t *trace)
;2186:{
line 2189
;2187:	gentity_t	*tent;
;2188:
;2189:	if (other && other->s.number < 1022 &&
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
EQU4 $643
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1022
GEI4 $643
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 8
INDIRU4
NEU4 $645
ADDRLP4 4
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 0
NEI4 $643
LABELV $645
line 2191
;2190:		(other->client || !other->s.weapon))
;2191:	{
line 2194
;2192:		vec3_t vNor, tN;
;2193:
;2194:		VectorCopy(trace->plane.normal, vNor);
ADDRLP4 12
ADDRFP4 8
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 2195
;2195:		VectorNormalize(vNor);
ADDRLP4 12
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 2196
;2196:		VectorNPos(self->s.pos.trDelta, tN);
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ARGP4
ADDRLP4 24
ARGP4
ADDRGP4 VectorNPos
CALLV
pop
line 2197
;2197:		self->s.pos.trDelta[0] += vNor[0]*(tN[0]*(((float)Q_irand(1, 10))*0.1));
CNSTI4 1
ARGI4
CNSTI4 10
ARGI4
ADDRLP4 36
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 40
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRF4
ADDRLP4 12
INDIRF4
ADDRLP4 24
INDIRF4
CNSTF4 1036831949
ADDRLP4 36
INDIRI4
CVIF4 4
MULF4
MULF4
MULF4
ADDF4
ASGNF4
line 2198
;2198:		self->s.pos.trDelta[1] += vNor[1]*(tN[1]*(((float)Q_irand(1, 10))*0.1));
CNSTI4 1
ARGI4
CNSTI4 10
ARGI4
ADDRLP4 44
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 48
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRF4
ADDRLP4 12+4
INDIRF4
ADDRLP4 24+4
INDIRF4
CNSTF4 1036831949
ADDRLP4 44
INDIRI4
CVIF4 4
MULF4
MULF4
MULF4
ADDF4
ASGNF4
line 2199
;2199:		self->s.pos.trDelta[2] += vNor[1]*(tN[2]*(((float)Q_irand(1, 10))*0.1));
CNSTI4 1
ARGI4
CNSTI4 10
ARGI4
ADDRLP4 52
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 56
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRF4
ADDRLP4 12+4
INDIRF4
ADDRLP4 24+8
INDIRF4
CNSTF4 1036831949
ADDRLP4 52
INDIRI4
CVIF4 4
MULF4
MULF4
MULF4
ADDF4
ASGNF4
line 2201
;2200:
;2201:		vectoangles(vNor, self->s.angles);
ADDRLP4 12
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 2202
;2202:		vectoangles(vNor, self->s.apos.trBase);
ADDRLP4 12
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 2203
;2203:		self->touch = charge_stick;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 charge_stick
ASGNP4
line 2204
;2204:		return;
ADDRGP4 $642
JUMPV
LABELV $643
line 2206
;2205:	}
;2206:	else if (other && other->s.number < 1022)
ADDRLP4 12
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $650
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1022
GEI4 $650
line 2207
;2207:	{
line 2210
;2208:		vec3_t v;
;2209:
;2210:		self->touch = 0;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
CNSTP4 0
ASGNP4
line 2211
;2211:		self->think = 0;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
CNSTP4 0
ASGNP4
line 2212
;2212:		self->nextthink = 0;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
CNSTI4 0
ASGNI4
line 2214
;2213:
;2214:		self->takedamage = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 0
ASGNI4
line 2216
;2215:
;2216:		VectorClear(self->s.apos.trDelta);
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
CNSTF4 0
ASGNF4
ADDRLP4 28
INDIRP4
CNSTI4 80
ADDP4
ADDRLP4 32
INDIRF4
ASGNF4
ADDRLP4 28
INDIRP4
CNSTI4 76
ADDP4
ADDRLP4 32
INDIRF4
ASGNF4
ADDRLP4 28
INDIRP4
CNSTI4 72
ADDP4
ADDRLP4 32
INDIRF4
ASGNF4
line 2217
;2217:		self->s.apos.trType = TR_STATIONARY;
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 0
ASGNI4
line 2219
;2218:
;2219:		G_RadiusDamage( self->r.currentOrigin, self->parent, self->splashDamage, self->splashRadius, self, MOD_DET_PACK_SPLASH );
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 36
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ARGP4
ADDRLP4 36
INDIRP4
CNSTI4 696
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 36
INDIRP4
CNSTI4 700
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 36
INDIRP4
ARGP4
CNSTI4 26
ARGI4
ADDRGP4 G_RadiusDamage
CALLI4
pop
line 2220
;2220:		VectorCopy(trace->plane.normal, v);
ADDRLP4 16
ADDRFP4 8
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 2221
;2221:		VectorCopy(v, self->pos2);
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
ADDRLP4 16
INDIRB
ASGNB 12
line 2222
;2222:		self->count = -1;
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
CNSTI4 -1
ASGNI4
line 2223
;2223:		G_PlayEffect(EFFECT_EXPLOSION_DETPACK, self->r.currentOrigin, v);
CNSTI4 6
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 16
ARGP4
ADDRGP4 G_PlayEffect
CALLP4
pop
line 2225
;2224:
;2225:		self->think = G_FreeEntity;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 G_FreeEntity
ASGNP4
line 2226
;2226:		self->nextthink = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 2227
;2227:		return;
ADDRGP4 $642
JUMPV
LABELV $650
line 2233
;2228:	}
;2229:
;2230:	//self->s.eType = ET_GENERAL;
;2231:	//FIXME: once on ground, shouldn't explode if touched by someone?
;2232:	//FIXME: if owner touches it again, pick it up?  Or if he "uses" it?
;2233:	self->touch = 0;
ADDRFP4 0
INDIRP4
CNSTI4 648
ADDP4
CNSTP4 0
ASGNP4
line 2234
;2234:	self->think = DetPackBlow;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 DetPackBlow
ASGNP4
line 2235
;2235:	self->nextthink = level.time + 30000;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 30000
ADDI4
ASGNI4
line 2237
;2236:
;2237:	VectorClear(self->s.apos.trDelta);
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 20
CNSTF4 0
ASGNF4
ADDRLP4 16
INDIRP4
CNSTI4 80
ADDP4
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 16
INDIRP4
CNSTI4 76
ADDP4
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 16
INDIRP4
CNSTI4 72
ADDP4
ADDRLP4 20
INDIRF4
ASGNF4
line 2238
;2238:	self->s.apos.trType = TR_STATIONARY;
ADDRFP4 0
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 0
ASGNI4
line 2240
;2239:
;2240:	self->s.pos.trType = TR_STATIONARY;
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 2241
;2241:	VectorCopy( self->r.currentOrigin, self->s.origin );
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
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 2242
;2242:	VectorCopy( self->r.currentOrigin, self->s.pos.trBase );
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
line 2243
;2243:	VectorClear( self->s.pos.trDelta );
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
CNSTF4 0
ASGNF4
ADDRLP4 32
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 36
INDIRF4
ASGNF4
ADDRLP4 32
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 36
INDIRF4
ASGNF4
ADDRLP4 32
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 36
INDIRF4
ASGNF4
line 2245
;2244:
;2245:	VectorClear( self->s.apos.trDelta );
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
CNSTF4 0
ASGNF4
ADDRLP4 40
INDIRP4
CNSTI4 80
ADDP4
ADDRLP4 44
INDIRF4
ASGNF4
ADDRLP4 40
INDIRP4
CNSTI4 76
ADDP4
ADDRLP4 44
INDIRF4
ASGNF4
ADDRLP4 40
INDIRP4
CNSTI4 72
ADDP4
ADDRLP4 44
INDIRF4
ASGNF4
line 2247
;2246:
;2247:	VectorNormalize(trace->plane.normal);
ADDRFP4 8
INDIRP4
CNSTI4 24
ADDP4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 2249
;2248:
;2249:	vectoangles(trace->plane.normal, self->s.angles);
ADDRFP4 8
INDIRP4
CNSTI4 24
ADDP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 2250
;2250:	VectorCopy(self->s.angles, self->r.currentAngles );
ADDRLP4 48
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 48
INDIRP4
CNSTI4 380
ADDP4
ADDRLP4 48
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 2251
;2251:	VectorCopy(self->s.angles, self->s.apos.trBase);
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 52
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 2253
;2252:
;2253:	VectorCopy(trace->plane.normal, self->pos2);
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
ADDRFP4 8
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 2254
;2254:	self->count = -1;
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
CNSTI4 -1
ASGNI4
line 2256
;2255:
;2256:	G_Sound(self, CHAN_VOICE, G_SoundIndex("sound/weapons/detpack/stick.wav"));
ADDRGP4 $654
ARGP4
ADDRLP4 56
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 3
ARGI4
ADDRLP4 56
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 2258
;2257:		
;2258:	tent = G_TempEntity( self->r.currentOrigin, EV_MISSILE_MISS );
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
CNSTI4 74
ARGI4
ADDRLP4 60
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 60
INDIRP4
ASGNP4
line 2259
;2259:	tent->s.weapon = 0;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 0
ASGNI4
line 2260
;2260:	tent->parent = self;
ADDRLP4 0
INDIRP4
CNSTI4 536
ADDP4
ADDRFP4 0
INDIRP4
ASGNP4
line 2261
;2261:	tent->r.ownerNum = self->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 396
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 2264
;2262:
;2263:	//so that the owner can blow it up with projectiles
;2264:	self->r.svFlags |= SVF_OWNERNOTSHARED;
ADDRLP4 64
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 64
INDIRP4
ADDRLP4 64
INDIRP4
INDIRI4
CNSTI4 4096
BORI4
ASGNI4
line 2265
;2265:}
LABELV $642
endproc charge_stick 68 24
export DetPackBlow
proc DetPackBlow 16 24
line 2268
;2266:
;2267:void DetPackBlow(gentity_t *self)
;2268:{
line 2272
;2269:	vec3_t v;
;2270:
;2271:	//self->touch = NULL;
;2272:	self->pain = 0;
ADDRFP4 0
INDIRP4
CNSTI4 656
ADDP4
CNSTP4 0
ASGNP4
line 2273
;2273:	self->die = 0;
ADDRFP4 0
INDIRP4
CNSTI4 660
ADDP4
CNSTP4 0
ASGNP4
line 2274
;2274:	self->takedamage = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 0
ASGNI4
line 2276
;2275:
;2276:	G_RadiusDamage( self->r.currentOrigin, self->parent, self->splashDamage, self->splashRadius, self, MOD_DET_PACK_SPLASH );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 12
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
INDIRP4
CNSTI4 696
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 12
INDIRP4
CNSTI4 700
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 12
INDIRP4
ARGP4
CNSTI4 26
ARGI4
ADDRGP4 G_RadiusDamage
CALLI4
pop
line 2277
;2277:	v[0] = 0;
ADDRLP4 0
CNSTF4 0
ASGNF4
line 2278
;2278:	v[1] = 0;
ADDRLP4 0+4
CNSTF4 0
ASGNF4
line 2279
;2279:	v[2] = 1;
ADDRLP4 0+8
CNSTF4 1065353216
ASGNF4
line 2281
;2280:
;2281:	if (self->count == -1)
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 -1
NEI4 $658
line 2282
;2282:	{
line 2283
;2283:		VectorCopy(self->pos2, v);
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
INDIRB
ASGNB 12
line 2284
;2284:	}
LABELV $658
line 2286
;2285:
;2286:	G_PlayEffect(EFFECT_EXPLOSION_DETPACK, self->r.currentOrigin, v);
CNSTI4 6
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_PlayEffect
CALLP4
pop
line 2288
;2287:
;2288:	self->think = G_FreeEntity;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 G_FreeEntity
ASGNP4
line 2289
;2289:	self->nextthink = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 2290
;2290:}
LABELV $655
endproc DetPackBlow 16 24
export DetPackPain
proc DetPackPain 4 8
line 2293
;2291:
;2292:void DetPackPain(gentity_t *self, gentity_t *attacker, int damage)
;2293:{
line 2294
;2294:	self->think = DetPackBlow;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 DetPackBlow
ASGNP4
line 2295
;2295:	self->nextthink = level.time + Q_irand(50, 100);
CNSTI4 50
ARGI4
CNSTI4 100
ARGI4
ADDRLP4 0
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
ASGNI4
line 2296
;2296:	self->takedamage = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 0
ASGNI4
line 2297
;2297:}
LABELV $661
endproc DetPackPain 4 8
export DetPackDie
proc DetPackDie 4 8
line 2300
;2298:
;2299:void DetPackDie(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod)
;2300:{
line 2301
;2301:	self->think = DetPackBlow;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 DetPackBlow
ASGNP4
line 2302
;2302:	self->nextthink = level.time + Q_irand(50, 100);
CNSTI4 50
ARGI4
CNSTI4 100
ARGI4
ADDRLP4 0
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ADDRLP4 0
INDIRI4
ADDI4
ASGNI4
line 2303
;2303:	self->takedamage = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 0
ASGNI4
line 2304
;2304:}
LABELV $663
endproc DetPackDie 4 8
export drop_charge
proc drop_charge 32 8
line 2307
;2305:
;2306:void drop_charge (gentity_t *self, vec3_t start, vec3_t dir) 
;2307:{
line 2310
;2308:	gentity_t	*bolt;
;2309:
;2310:	VectorNormalize (dir);
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 2312
;2311:
;2312:	bolt = G_Spawn();
ADDRLP4 4
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 2313
;2313:	bolt->classname = "detpack";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $666
ASGNP4
line 2314
;2314:	bolt->nextthink = level.time + FRAMETIME;
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 2315
;2315:	bolt->think = G_RunObject;
ADDRLP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 G_RunObject
ASGNP4
line 2316
;2316:	bolt->s.eType = ET_GENERAL;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 2317
;2317:	bolt->s.g2radius = 100;
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
CNSTI4 100
ASGNI4
line 2318
;2318:	bolt->s.modelGhoul2 = 1;
ADDRLP4 0
INDIRP4
CNSTI4 204
ADDP4
CNSTI4 1
ASGNI4
line 2319
;2319:	bolt->s.modelindex = G_ModelIndex("models/weapons2/detpack/det_pack_proj.glm"); // w.md3");
ADDRGP4 $668
ARGP4
ADDRLP4 8
ADDRGP4 G_ModelIndex
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 212
ADDP4
ADDRLP4 8
INDIRI4
ASGNI4
line 2322
;2320:
;2321:	//bolt->playerTeam = self->client->playerTeam;
;2322:	bolt->parent = self;
ADDRLP4 0
INDIRP4
CNSTI4 536
ADDP4
ADDRFP4 0
INDIRP4
ASGNP4
line 2323
;2323:	bolt->r.ownerNum = self->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 396
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 2324
;2324:	bolt->damage = 100;
ADDRLP4 0
INDIRP4
CNSTI4 688
ADDP4
CNSTI4 100
ASGNI4
line 2325
;2325:	bolt->splashDamage = 200;
ADDRLP4 0
INDIRP4
CNSTI4 696
ADDP4
CNSTI4 200
ASGNI4
line 2326
;2326:	bolt->splashRadius = 200;
ADDRLP4 0
INDIRP4
CNSTI4 700
ADDP4
CNSTI4 200
ASGNI4
line 2327
;2327:	bolt->methodOfDeath = MOD_DET_PACK_SPLASH;//MOD_EXPLOSIVE;
ADDRLP4 0
INDIRP4
CNSTI4 704
ADDP4
CNSTI4 26
ASGNI4
line 2328
;2328:	bolt->splashMethodOfDeath = MOD_DET_PACK_SPLASH;//MOD_EXPLOSIVE_SPLASH;
ADDRLP4 0
INDIRP4
CNSTI4 708
ADDP4
CNSTI4 26
ASGNI4
line 2329
;2329:	bolt->clipmask = MASK_SHOT;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 769
ASGNI4
line 2330
;2330:	bolt->s.solid = 2;
ADDRLP4 0
INDIRP4
CNSTI4 248
ADDP4
CNSTI4 2
ASGNI4
line 2331
;2331:	bolt->r.contents = MASK_SHOT;
ADDRLP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 769
ASGNI4
line 2332
;2332:	bolt->touch = charge_stick;
ADDRLP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 charge_stick
ASGNP4
line 2334
;2333:
;2334:	bolt->physicsObject = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 500
ADDP4
CNSTI4 1
ASGNI4
line 2336
;2335:
;2336:	bolt->s.genericenemyindex = self->s.number+1024;
ADDRLP4 0
INDIRP4
CNSTI4 172
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
CNSTI4 1024
ADDI4
ASGNI4
line 2341
;2337:	//rww - so client prediction knows we own this and won't hit it
;2338:
;2339://	VectorSet( bolt->r.mins, -3, -3, -3 );
;2340://	VectorSet( bolt->r.maxs, 3, 3, 3 );
;2341:	VectorSet( bolt->r.mins, -2, -2, -2 );
ADDRLP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3221225472
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3221225472
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 3221225472
ASGNF4
line 2342
;2342:	VectorSet( bolt->r.maxs, 2, 2, 2 );
ADDRLP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1073741824
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1073741824
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1073741824
ASGNF4
line 2344
;2343:
;2344:	bolt->health = 1;
ADDRLP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 1
ASGNI4
line 2345
;2345:	bolt->takedamage = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 1
ASGNI4
line 2346
;2346:	bolt->pain = DetPackPain;
ADDRLP4 0
INDIRP4
CNSTI4 656
ADDP4
ADDRGP4 DetPackPain
ASGNP4
line 2347
;2347:	bolt->die = DetPackDie;
ADDRLP4 0
INDIRP4
CNSTI4 660
ADDP4
ADDRGP4 DetPackDie
ASGNP4
line 2349
;2348:
;2349:	bolt->s.weapon = WP_DET_PACK;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 13
ASGNI4
line 2351
;2350:
;2351:	bolt->setTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 628
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 2353
;2352:
;2353:	G_SetOrigin(bolt, start);
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 2354
;2354:	bolt->s.pos.trType = TR_GRAVITY;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 5
ASGNI4
line 2355
;2355:	VectorCopy( start, bolt->s.pos.trBase );
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 2356
;2356:	VectorScale(dir, 300, bolt->s.pos.trDelta );
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
CNSTF4 1133903872
ADDRFP4 8
INDIRP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
CNSTF4 1133903872
ADDRFP4 8
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
MULF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
CNSTF4 1133903872
ADDRFP4 8
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
MULF4
ASGNF4
line 2357
;2357:	bolt->s.pos.trTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 2359
;2358:
;2359:	bolt->s.apos.trType = TR_GRAVITY;
ADDRLP4 0
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 5
ASGNI4
line 2360
;2360:	bolt->s.apos.trTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 52
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 2361
;2361:	bolt->s.apos.trBase[YAW] = rand()%360;
ADDRLP4 12
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ADDRLP4 12
INDIRI4
CNSTI4 360
MODI4
CVIF4 4
ASGNF4
line 2362
;2362:	bolt->s.apos.trBase[PITCH] = rand()%360;
ADDRLP4 16
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 16
INDIRI4
CNSTI4 360
MODI4
CVIF4 4
ASGNF4
line 2363
;2363:	bolt->s.apos.trBase[ROLL] = rand()%360;
ADDRLP4 20
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
ADDRLP4 20
INDIRI4
CNSTI4 360
MODI4
CVIF4 4
ASGNF4
line 2365
;2364:
;2365:	if (rand()%10 < 5)
ADDRLP4 24
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 10
MODI4
CNSTI4 5
GEI4 $672
line 2366
;2366:	{
line 2367
;2367:		bolt->s.apos.trBase[YAW] = -bolt->s.apos.trBase[YAW];
ADDRLP4 28
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRF4
NEGF4
ASGNF4
line 2368
;2368:	}
LABELV $672
line 2370
;2369:
;2370:	vectoangles(dir, bolt->s.angles);
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
line 2371
;2371:	VectorCopy(bolt->s.angles, bolt->s.apos.trBase);
ADDRLP4 0
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 2372
;2372:	VectorSet(bolt->s.apos.trDelta, 300, 0, 0 );
ADDRLP4 0
INDIRP4
CNSTI4 72
ADDP4
CNSTF4 1133903872
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 76
ADDP4
CNSTF4 0
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 80
ADDP4
CNSTF4 0
ASGNF4
line 2373
;2373:	bolt->s.apos.trTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 52
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 2375
;2374:
;2375:	trap_LinkEntity(bolt);
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 2376
;2376:}
LABELV $665
endproc drop_charge 32 8
export BlowDetpacks
proc BlowDetpacks 16 12
line 2379
;2377:
;2378:void BlowDetpacks(gentity_t *ent)
;2379:{
line 2380
;2380:	gentity_t *found = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
line 2382
;2381:
;2382:	if ( ent->client->ps.hasDetPackPlanted )
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
CNSTI4 0
EQI4 $676
line 2383
;2383:	{
ADDRGP4 $679
JUMPV
LABELV $678
line 2385
;2384:		while ( (found = G_Find( found, FOFS(classname), "detpack") ) != NULL )
;2385:		{//loop through all ents and blow the crap out of them!
line 2386
;2386:			if ( found->parent == ent )
ADDRLP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
NEU4 $681
line 2387
;2387:			{
line 2388
;2388:				VectorCopy( found->r.currentOrigin, found->s.origin );
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
line 2389
;2389:				found->think = DetPackBlow;
ADDRLP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 DetPackBlow
ASGNP4
line 2390
;2390:				found->nextthink = level.time + 100 + random() * 200;
ADDRLP4 8
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
CVIF4 4
CNSTF4 1128792064
ADDRLP4 8
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
line 2391
;2391:				G_Sound( found, CHAN_BODY, G_SoundIndex("sound/weapons/detpack/warning.wav") );
ADDRGP4 $684
ARGP4
ADDRLP4 12
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 5
ARGI4
ADDRLP4 12
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 2392
;2392:			}
LABELV $681
line 2393
;2393:		}
LABELV $679
line 2384
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 416
ARGI4
ADDRGP4 $666
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
NEU4 $678
line 2394
;2394:		ent->client->ps.hasDetPackPlanted = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 628
ADDP4
CNSTI4 0
ASGNI4
line 2395
;2395:	}
LABELV $676
line 2396
;2396:}
LABELV $675
endproc BlowDetpacks 16 12
export CheatsOn
proc CheatsOn 0 0
line 2399
;2397:
;2398:qboolean CheatsOn(void) 
;2399:{
line 2400
;2400:	if ( !g_cheats.integer )
ADDRGP4 g_cheats+12
INDIRI4
CNSTI4 0
NEI4 $686
line 2401
;2401:	{
line 2402
;2402:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $685
JUMPV
LABELV $686
line 2404
;2403:	}
;2404:	return qtrue;
CNSTI4 1
RETI4
LABELV $685
endproc CheatsOn 0 0
lit
align 4
LABELV $690
byte 4 1023
skip 4092
export WP_DropDetPack
code
proc WP_DropDetPack 4140 20
line 2408
;2405:}
;2406:
;2407:void WP_DropDetPack( gentity_t *ent, qboolean alt_fire )
;2408:{
line 2409
;2409:	gentity_t	*found = NULL;
ADDRLP4 4
CNSTP4 0
ASGNP4
line 2410
;2410:	int			trapcount = 0;
ADDRLP4 4116
CNSTI4 0
ASGNI4
line 2411
;2411:	int			foundDetPacks[MAX_GENTITIES] = {ENTITYNUM_NONE};
ADDRLP4 8
ADDRGP4 $690
INDIRB
ASGNB 4096
line 2417
;2412:	int			trapcount_org;
;2413:	int			lowestTimeStamp;
;2414:	int			removeMe;
;2415:	int			i;
;2416:
;2417:	if ( !ent || !ent->client )
ADDRLP4 4120
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4124
CNSTU4 0
ASGNU4
ADDRLP4 4120
INDIRP4
CVPU4 4
ADDRLP4 4124
INDIRU4
EQU4 $693
ADDRLP4 4120
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 4124
INDIRU4
NEU4 $695
LABELV $693
line 2418
;2418:	{
line 2419
;2419:		return;
ADDRGP4 $689
JUMPV
LABELV $694
line 2425
;2420:	}
;2421:
;2422:	//limit to 10 placed at any one time
;2423:	//see how many there are now
;2424:	while ( (found = G_Find( found, FOFS(classname), "detpack" )) != NULL )
;2425:	{
line 2426
;2426:		if ( found->parent != ent )
ADDRLP4 4
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
EQU4 $697
line 2427
;2427:		{
line 2428
;2428:			continue;
ADDRGP4 $695
JUMPV
LABELV $697
line 2430
;2429:		}
;2430:		foundDetPacks[trapcount++] = found->s.number;
ADDRLP4 4128
ADDRLP4 4116
INDIRI4
ASGNI4
ADDRLP4 4116
ADDRLP4 4128
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4128
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
ADDRLP4 4
INDIRP4
INDIRI4
ASGNI4
line 2431
;2431:	}
LABELV $695
line 2424
ADDRLP4 4
INDIRP4
ARGP4
CNSTI4 416
ARGI4
ADDRGP4 $666
ARGP4
ADDRLP4 4128
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 4128
INDIRP4
ASGNP4
ADDRLP4 4128
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $694
line 2433
;2432:	//now remove first ones we find until there are only 9 left
;2433:	found = NULL;
ADDRLP4 4
CNSTP4 0
ASGNP4
line 2434
;2434:	trapcount_org = trapcount;
ADDRLP4 4108
ADDRLP4 4116
INDIRI4
ASGNI4
line 2435
;2435:	lowestTimeStamp = level.time;
ADDRLP4 4104
ADDRGP4 level+32
INDIRI4
ASGNI4
ADDRGP4 $701
JUMPV
LABELV $700
line 2437
;2436:	while ( trapcount > 9 )
;2437:	{
line 2438
;2438:		removeMe = -1;
ADDRLP4 4112
CNSTI4 -1
ASGNI4
line 2439
;2439:		for ( i = 0; i < trapcount_org; i++ )
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $706
JUMPV
LABELV $703
line 2440
;2440:		{
line 2441
;2441:			if ( foundDetPacks[i] == ENTITYNUM_NONE )
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRI4
CNSTI4 1023
NEI4 $707
line 2442
;2442:			{
line 2443
;2443:				continue;
ADDRGP4 $704
JUMPV
LABELV $707
line 2445
;2444:			}
;2445:			found = &g_entities[foundDetPacks[i]];
ADDRLP4 4
CNSTI4 828
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 2446
;2446:			if ( found->setTime < lowestTimeStamp )
ADDRLP4 4
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
ADDRLP4 4104
INDIRI4
GEI4 $709
line 2447
;2447:			{
line 2448
;2448:				removeMe = i;
ADDRLP4 4112
ADDRLP4 0
INDIRI4
ASGNI4
line 2449
;2449:				lowestTimeStamp = found->setTime;
ADDRLP4 4104
ADDRLP4 4
INDIRP4
CNSTI4 628
ADDP4
INDIRI4
ASGNI4
line 2450
;2450:			}
LABELV $709
line 2451
;2451:		}
LABELV $704
line 2439
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $706
ADDRLP4 0
INDIRI4
ADDRLP4 4108
INDIRI4
LTI4 $703
line 2452
;2452:		if ( removeMe != -1 )
ADDRLP4 4112
INDIRI4
CNSTI4 -1
EQI4 $702
line 2453
;2453:		{
line 2455
;2454:			//remove it... or blow it?
;2455:			if ( &g_entities[foundDetPacks[removeMe]] == NULL )
CNSTI4 828
ADDRLP4 4112
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
CVPU4 4
CNSTU4 0
NEU4 $713
line 2456
;2456:			{
line 2457
;2457:				break;
ADDRGP4 $702
JUMPV
LABELV $713
line 2460
;2458:			}
;2459:			else
;2460:			{
line 2461
;2461:				if (!CheatsOn())
ADDRLP4 4132
ADDRGP4 CheatsOn
CALLI4
ASGNI4
ADDRLP4 4132
INDIRI4
CNSTI4 0
NEI4 $715
line 2462
;2462:				{ //Let them have unlimited if cheats are enabled
line 2463
;2463:					G_FreeEntity( &g_entities[foundDetPacks[removeMe]] );
CNSTI4 828
ADDRLP4 4112
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 2464
;2464:				}
LABELV $715
line 2465
;2465:			}
line 2466
;2466:			foundDetPacks[removeMe] = ENTITYNUM_NONE;
ADDRLP4 4112
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
CNSTI4 1023
ASGNI4
line 2467
;2467:			trapcount--;
ADDRLP4 4116
ADDRLP4 4116
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2468
;2468:		}
line 2470
;2469:		else
;2470:		{
line 2471
;2471:			break;
LABELV $712
line 2473
;2472:		}
;2473:	}
LABELV $701
line 2436
ADDRLP4 4116
INDIRI4
CNSTI4 9
GTI4 $700
LABELV $702
line 2475
;2474:
;2475:	if ( alt_fire  )
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $717
line 2476
;2476:	{
line 2477
;2477:		BlowDetpacks(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BlowDetpacks
CALLV
pop
line 2478
;2478:	}
ADDRGP4 $718
JUMPV
LABELV $717
line 2480
;2479:	else
;2480:	{
line 2481
;2481:		AngleVectors( ent->client->ps.viewangles, forward, right, up );
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 156
ADDP4
ARGP4
ADDRGP4 forward
ARGP4
ADDRGP4 right
ARGP4
ADDRGP4 up
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 2483
;2482:
;2483:		CalcMuzzlePoint( ent, forward, right, up, muzzle );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 forward
ARGP4
ADDRGP4 right
ARGP4
ADDRGP4 up
ARGP4
ADDRGP4 muzzle
ARGP4
ADDRGP4 CalcMuzzlePoint
CALLV
pop
line 2485
;2484:
;2485:		VectorNormalize( forward );
ADDRGP4 forward
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 2486
;2486:		VectorMA( muzzle, -4, forward, muzzle );
ADDRLP4 4132
ADDRGP4 muzzle
ASGNP4
ADDRLP4 4136
CNSTF4 3229614080
ASGNF4
ADDRLP4 4132
INDIRP4
ADDRLP4 4132
INDIRP4
INDIRF4
ADDRLP4 4136
INDIRF4
ADDRGP4 forward
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRGP4 muzzle+4
ADDRGP4 muzzle+4
INDIRF4
ADDRLP4 4136
INDIRF4
ADDRGP4 forward+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRGP4 muzzle+8
ADDRGP4 muzzle+8
INDIRF4
CNSTF4 3229614080
ADDRGP4 forward+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 2487
;2487:		drop_charge( ent, muzzle, forward );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 muzzle
ARGP4
ADDRGP4 forward
ARGP4
ADDRGP4 drop_charge
CALLV
pop
line 2489
;2488:
;2489:		ent->client->ps.hasDetPackPlanted = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 628
ADDP4
CNSTI4 1
ASGNI4
line 2490
;2490:	}
LABELV $718
line 2491
;2491:}
LABELV $689
endproc WP_DropDetPack 4140 20
export WP_FireStunBaton
proc WP_FireStunBaton 1172 32
line 2498
;2492:
;2493:
;2494://---------------------------------------------------------
;2495:// FireStunBaton
;2496://---------------------------------------------------------
;2497:void WP_FireStunBaton( gentity_t *ent, qboolean alt_fire )
;2498:{
line 2504
;2499:	gentity_t	*tr_ent;
;2500:	trace_t		tr;
;2501:	vec3_t		mins, maxs, end;
;2502:	vec3_t		muzzleStun;
;2503:
;2504:	VectorCopy(ent->client->ps.origin, muzzleStun);
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 2505
;2505:	muzzleStun[2] += ent->client->ps.viewheight-6;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 168
ADDP4
INDIRI4
CNSTI4 6
SUBI4
CVIF4 4
ADDF4
ASGNF4
line 2507
;2506:
;2507:	muzzleStun[0] += forward[0]*20;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1101004800
ADDRGP4 forward
INDIRF4
MULF4
ADDF4
ASGNF4
line 2508
;2508:	muzzleStun[1] += forward[1]*20;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1101004800
ADDRGP4 forward+4
INDIRF4
MULF4
ADDF4
ASGNF4
line 2509
;2509:	muzzleStun[2] += forward[2]*20;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1101004800
ADDRGP4 forward+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 2511
;2510:
;2511:	muzzleStun[0] += right[0]*4;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1082130432
ADDRGP4 right
INDIRF4
MULF4
ADDF4
ASGNF4
line 2512
;2512:	muzzleStun[1] += right[1]*4;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1082130432
ADDRGP4 right+4
INDIRF4
MULF4
ADDF4
ASGNF4
line 2513
;2513:	muzzleStun[2] += right[2]*4;
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1082130432
ADDRGP4 right+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 2515
;2514:
;2515:	VectorMA( muzzleStun, STUN_BATON_RANGE, forward, end );
ADDRLP4 1132
CNSTF4 1090519040
ASGNF4
ADDRLP4 1120
ADDRLP4 0
INDIRF4
ADDRLP4 1132
INDIRF4
ADDRGP4 forward
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 1120+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 1132
INDIRF4
ADDRGP4 forward+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 1120+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1090519040
ADDRGP4 forward+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 2517
;2516:
;2517:	VectorSet( maxs, 6, 6, 6 );
ADDRLP4 1136
CNSTF4 1086324736
ASGNF4
ADDRLP4 16
ADDRLP4 1136
INDIRF4
ASGNF4
ADDRLP4 16+4
ADDRLP4 1136
INDIRF4
ASGNF4
ADDRLP4 16+8
CNSTF4 1086324736
ASGNF4
line 2518
;2518:	VectorScale( maxs, -1, mins );
ADDRLP4 1140
CNSTF4 3212836864
ASGNF4
ADDRLP4 1108
ADDRLP4 1140
INDIRF4
ADDRLP4 16
INDIRF4
MULF4
ASGNF4
ADDRLP4 1108+4
ADDRLP4 1140
INDIRF4
ADDRLP4 16+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 1108+8
CNSTF4 3212836864
ADDRLP4 16+8
INDIRF4
MULF4
ASGNF4
line 2520
;2519:
;2520:	trap_Trace ( &tr, muzzleStun, mins, maxs, end, ent->s.number, MASK_SHOT );
ADDRLP4 28
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 1108
ARGP4
ADDRLP4 16
ARGP4
ADDRLP4 1120
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
line 2522
;2521:
;2522:	if ( tr.entityNum >= ENTITYNUM_WORLD )
ADDRLP4 28+52
INDIRI4
CNSTI4 1022
LTI4 $747
line 2523
;2523:	{
line 2524
;2524:		return;
ADDRGP4 $725
JUMPV
LABELV $747
line 2527
;2525:	}
;2526:
;2527:	tr_ent = &g_entities[tr.entityNum];
ADDRLP4 12
CNSTI4 828
ADDRLP4 28+52
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 2529
;2528:
;2529:	if (tr_ent && tr_ent->takedamage && tr_ent->client)
ADDRLP4 1148
CNSTU4 0
ASGNU4
ADDRLP4 12
INDIRP4
CVPU4 4
ADDRLP4 1148
INDIRU4
EQU4 $751
ADDRLP4 12
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
CNSTI4 0
EQI4 $751
ADDRLP4 12
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 1148
INDIRU4
EQU4 $751
line 2530
;2530:	{
line 2531
;2531:		if (tr_ent->client->ps.duelInProgress &&
ADDRLP4 1152
ADDRLP4 12
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1152
INDIRP4
CNSTI4 1292
ADDP4
INDIRI4
CNSTI4 0
EQI4 $753
ADDRLP4 1152
INDIRP4
CNSTI4 1284
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
EQI4 $753
line 2533
;2532:			tr_ent->client->ps.duelIndex != ent->s.number)
;2533:		{
line 2534
;2534:			return;
ADDRGP4 $725
JUMPV
LABELV $753
line 2537
;2535:		}
;2536:
;2537:		if (ent->client->ps.duelInProgress &&
ADDRLP4 1156
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1156
INDIRP4
CNSTI4 1292
ADDP4
INDIRI4
CNSTI4 0
EQI4 $755
ADDRLP4 1156
INDIRP4
CNSTI4 1284
ADDP4
INDIRI4
ADDRLP4 12
INDIRP4
INDIRI4
EQI4 $755
line 2539
;2538:			ent->client->ps.duelIndex != tr_ent->s.number)
;2539:		{
line 2540
;2540:			return;
ADDRGP4 $725
JUMPV
LABELV $755
line 2542
;2541:		}
;2542:	}
LABELV $751
line 2544
;2543:
;2544:	if ( tr_ent && tr_ent->takedamage )
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $757
ADDRLP4 12
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
CNSTI4 0
EQI4 $757
line 2545
;2545:	{
line 2546
;2546:		G_PlayEffect( EFFECT_STUNHIT, tr.endpos, tr.plane.normal );
CNSTI4 8
ARGI4
ADDRLP4 28+12
ARGP4
ADDRLP4 28+24
ARGP4
ADDRGP4 G_PlayEffect
CALLP4
pop
line 2549
;2547:
;2548:		// TEMP!
;2549:		G_Sound( tr_ent, CHAN_WEAPON, G_SoundIndex( va("sound/weapons/melee/punch%d", Q_irand(1, 4)) ) );
CNSTI4 1
ARGI4
CNSTI4 4
ARGI4
ADDRLP4 1156
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRGP4 $761
ARGP4
ADDRLP4 1156
INDIRI4
ARGI4
ADDRLP4 1160
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1160
INDIRP4
ARGP4
ADDRLP4 1164
ADDRGP4 G_SoundIndex
CALLI4
ASGNI4
ADDRLP4 12
INDIRP4
ARGP4
CNSTI4 2
ARGI4
ADDRLP4 1164
INDIRI4
ARGI4
ADDRGP4 G_Sound
CALLV
pop
line 2551
;2550:
;2551:		G_Damage( tr_ent, ent, ent, forward, tr.endpos, STUN_BATON_DAMAGE, (DAMAGE_NO_KNOCKBACK|DAMAGE_HALF_ABSORB), MOD_STUN_BATON );
ADDRLP4 12
INDIRP4
ARGP4
ADDRLP4 1168
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1168
INDIRP4
ARGP4
ADDRLP4 1168
INDIRP4
ARGP4
ADDRGP4 forward
ARGP4
ADDRLP4 28+12
ARGP4
CNSTI4 20
ARGI4
CNSTI4 1028
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 2554
;2552:		//alt-fire?
;2553:
;2554:		if (tr_ent->client)
ADDRLP4 12
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $763
line 2555
;2555:		{ //if it's a player then use the shock effect
line 2556
;2556:			tr_ent->client->ps.electrifyTime = level.time + 700;
ADDRLP4 12
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 728
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 700
ADDI4
ASGNI4
line 2557
;2557:		}
LABELV $763
line 2558
;2558:	}
LABELV $757
line 2559
;2559:}
LABELV $725
endproc WP_FireStunBaton 1172 32
export SnapVectorTowards
proc SnapVectorTowards 12 0
line 2579
;2560:
;2561:
;2562:////////////////////////////////////////////////////////////////////////////
;2563:////////////////////////////////////////////////////////////////////////////
;2564:////////////////////////////////////////////////////////////////////////////
;2565:////////////////////////////////////////////////////////////////////////////
;2566:////////////////////////////////////////////////////////////////////////////
;2567:
;2568:
;2569:/*
;2570:======================
;2571:SnapVectorTowards
;2572:
;2573:Round a vector to integers for more efficient network
;2574:transmission, but make sure that it rounds towards a given point
;2575:rather than blindly truncating.  This prevents it from truncating 
;2576:into a wall.
;2577:======================
;2578:*/
;2579:void SnapVectorTowards( vec3_t v, vec3_t to ) {
line 2582
;2580:	int		i;
;2581:
;2582:	for ( i = 0 ; i < 3 ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $767
line 2583
;2583:		if ( to[i] <= v[i] ) {
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4
INDIRI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRF4
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
GTF4 $771
line 2584
;2584:			v[i] = (int)v[i];
ADDRLP4 8
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
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
line 2585
;2585:		} else {
ADDRGP4 $772
JUMPV
LABELV $771
line 2586
;2586:			v[i] = (int)v[i] + 1;
ADDRLP4 8
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRF4
CVFI4 4
CNSTI4 1
ADDI4
CVIF4 4
ASGNF4
line 2587
;2587:		}
LABELV $772
line 2588
;2588:	}
LABELV $768
line 2582
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $767
line 2589
;2589:}
LABELV $766
endproc SnapVectorTowards 12 0
export LogAccuracyHit
proc LogAccuracyHit 4 8
line 2600
;2590:
;2591:
;2592://======================================================================
;2593:
;2594:
;2595:/*
;2596:===============
;2597:LogAccuracyHit
;2598:===============
;2599:*/
;2600:qboolean LogAccuracyHit( gentity_t *target, gentity_t *attacker ) {
line 2601
;2601:	if( !target->takedamage ) {
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
CNSTI4 0
NEI4 $774
line 2602
;2602:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $773
JUMPV
LABELV $774
line 2605
;2603:	}
;2604:
;2605:	if ( target == attacker ) {
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRFP4 4
INDIRP4
CVPU4 4
NEU4 $776
line 2606
;2606:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $773
JUMPV
LABELV $776
line 2609
;2607:	}
;2608:
;2609:	if( !target->client ) {
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $778
line 2610
;2610:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $773
JUMPV
LABELV $778
line 2613
;2611:	}
;2612:
;2613:	if( !attacker->client ) {
ADDRFP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $780
line 2614
;2614:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $773
JUMPV
LABELV $780
line 2617
;2615:	}
;2616:
;2617:	if( target->client->ps.stats[STAT_HEALTH] <= 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CNSTI4 0
GTI4 $782
line 2618
;2618:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $773
JUMPV
LABELV $782
line 2621
;2619:	}
;2620:
;2621:	if ( OnSameTeam( target, attacker ) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
ADDRGP4 OnSameTeam
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $784
line 2622
;2622:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $773
JUMPV
LABELV $784
line 2625
;2623:	}
;2624:
;2625:	return qtrue;
CNSTI4 1
RETI4
LABELV $773
endproc LogAccuracyHit 4 8
export CalcMuzzlePoint
proc CalcMuzzlePoint 64 0
line 2637
;2626:}
;2627:
;2628:
;2629:/*
;2630:===============
;2631:CalcMuzzlePoint
;2632:
;2633:set muzzle location relative to pivoting eye
;2634:===============
;2635:*/
;2636:void CalcMuzzlePoint ( gentity_t *ent, vec3_t forward, vec3_t right, vec3_t up, vec3_t muzzlePoint ) 
;2637:{
line 2641
;2638:	int weapontype;
;2639:	vec3_t muzzleOffPoint;
;2640:
;2641:	weapontype = ent->s.weapon;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
ASGNI4
line 2642
;2642:	VectorCopy( ent->s.pos.trBase, muzzlePoint );
ADDRFP4 16
INDIRP4
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 2644
;2643:
;2644:	VectorCopy(WP_MuzzlePoint[weapontype], muzzleOffPoint);
ADDRLP4 0
CNSTI4 12
ADDRLP4 12
INDIRI4
MULI4
ADDRGP4 WP_MuzzlePoint
ADDP4
INDIRB
ASGNB 12
line 2646
;2645:
;2646:	if (ent->client->ps.usingATST)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1304
ADDP4
INDIRI4
CNSTI4 0
EQI4 $787
line 2647
;2647:	{
line 2648
;2648:		gentity_t *headEnt = &g_entities[ent->client->damageBoxHandle_Head];
ADDRLP4 16
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
ASGNP4
line 2650
;2649:
;2650:		VectorClear(muzzleOffPoint);
ADDRLP4 20
CNSTF4 0
ASGNF4
ADDRLP4 0+8
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 0
ADDRLP4 20
INDIRF4
ASGNF4
line 2651
;2651:		muzzleOffPoint[0] = 16;
ADDRLP4 0
CNSTF4 1098907648
ASGNF4
line 2652
;2652:		muzzleOffPoint[2] = 128;
ADDRLP4 0+8
CNSTF4 1124073472
ASGNF4
line 2654
;2653:
;2654:		if (headEnt && headEnt->s.number >= MAX_CLIENTS)
ADDRLP4 24
ADDRLP4 16
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $792
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 32
LTI4 $792
line 2655
;2655:		{
line 2656
;2656:			if (headEnt->bolt_Waist)
ADDRLP4 16
INDIRP4
CNSTI4 804
ADDP4
INDIRI4
CNSTI4 0
EQI4 $794
line 2657
;2657:			{
line 2658
;2658:				muzzleOffPoint[1] = 3;
ADDRLP4 0+4
CNSTF4 1077936128
ASGNF4
line 2659
;2659:			}
ADDRGP4 $795
JUMPV
LABELV $794
line 2661
;2660:			else
;2661:			{
line 2662
;2662:				muzzleOffPoint[1] = -4;
ADDRLP4 0+4
CNSTF4 3229614080
ASGNF4
line 2663
;2663:			}
LABELV $795
line 2664
;2664:		}
LABELV $792
line 2665
;2665:	}
LABELV $787
line 2668
;2666:
;2667:#if 1
;2668:	if (weapontype > WP_NONE && weapontype < WP_NUM_WEAPONS)
ADDRLP4 12
INDIRI4
CNSTI4 0
LEI4 $798
ADDRLP4 12
INDIRI4
CNSTI4 16
GEI4 $798
line 2669
;2669:	{	// Use the table to generate the muzzlepoint;
line 2670
;2670:		{	// Crouching.  Use the add-to-Z method to adjust vertically.
line 2671
;2671:			VectorMA(muzzlePoint, muzzleOffPoint[0], forward, muzzlePoint);
ADDRLP4 20
ADDRFP4 16
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRF4
ADDRFP4 4
INDIRP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 24
CNSTI4 4
ASGNI4
ADDRLP4 28
ADDRFP4 16
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRF4
ADDRFP4 4
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 32
CNSTI4 8
ASGNI4
ADDRLP4 36
ADDRFP4 16
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRF4
ADDRFP4 4
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 2672
;2672:			VectorMA(muzzlePoint, muzzleOffPoint[1], right, muzzlePoint);
ADDRLP4 40
ADDRFP4 16
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRF4
ADDRFP4 8
INDIRP4
INDIRF4
ADDRLP4 0+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 44
CNSTI4 4
ASGNI4
ADDRLP4 48
ADDRFP4 16
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
ASGNP4
ADDRLP4 48
INDIRP4
ADDRLP4 48
INDIRP4
INDIRF4
ADDRFP4 8
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 52
CNSTI4 8
ASGNI4
ADDRLP4 56
ADDRFP4 16
INDIRP4
ADDRLP4 52
INDIRI4
ADDP4
ASGNP4
ADDRLP4 56
INDIRP4
ADDRLP4 56
INDIRP4
INDIRF4
ADDRFP4 8
INDIRP4
ADDRLP4 52
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0+4
INDIRF4
MULF4
ADDF4
ASGNF4
line 2673
;2673:			muzzlePoint[2] += ent->client->ps.viewheight + muzzleOffPoint[2];
ADDRLP4 60
ADDRFP4 16
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 60
INDIRP4
ADDRLP4 60
INDIRP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 168
ADDP4
INDIRI4
CVIF4 4
ADDRLP4 0+8
INDIRF4
ADDF4
ADDF4
ASGNF4
line 2675
;2674:			// VectorMA(muzzlePoint, ent->client->ps.viewheight + WP_MuzzlePoint[weapontype][2], up, muzzlePoint);
;2675:		}
line 2676
;2676:	}
LABELV $798
line 2685
;2677:#else	// Test code
;2678:	muzzlePoint[2] += ent->client->ps.viewheight;//By eyes
;2679:	muzzlePoint[2] += g_debugUp.value;
;2680:	VectorMA( muzzlePoint, g_debugForward.value, forward, muzzlePoint);
;2681:	VectorMA( muzzlePoint, g_debugRight.value, right, muzzlePoint);
;2682:#endif
;2683:
;2684:	// snap to integer coordinates for more efficient network bandwidth usage
;2685:	SnapVector( muzzlePoint );
ADDRLP4 20
ADDRFP4 16
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 24
ADDRFP4 16
INDIRP4
CNSTI4 4
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 28
ADDRFP4 16
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
line 2686
;2686:}
LABELV $786
endproc CalcMuzzlePoint 64 0
export CalcMuzzlePointOrigin
proc CalcMuzzlePointOrigin 36 0
line 2695
;2687:
;2688:/*
;2689:===============
;2690:CalcMuzzlePointOrigin
;2691:
;2692:set muzzle location relative to pivoting eye
;2693:===============
;2694:*/
;2695:void CalcMuzzlePointOrigin ( gentity_t *ent, vec3_t origin, vec3_t forward, vec3_t right, vec3_t up, vec3_t muzzlePoint ) {
line 2696
;2696:	VectorCopy( ent->s.pos.trBase, muzzlePoint );
ADDRFP4 20
INDIRP4
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
INDIRB
ASGNB 12
line 2697
;2697:	muzzlePoint[2] += ent->client->ps.viewheight;
ADDRLP4 0
ADDRFP4 20
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 168
ADDP4
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 2698
;2698:	VectorMA( muzzlePoint, 14, forward, muzzlePoint );
ADDRLP4 4
ADDRFP4 20
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRF4
CNSTF4 1096810496
ADDRFP4 8
INDIRP4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 8
CNSTI4 4
ASGNI4
ADDRLP4 12
ADDRFP4 20
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRF4
CNSTF4 1096810496
ADDRFP4 8
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 16
CNSTI4 8
ASGNI4
ADDRLP4 20
ADDRFP4 20
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
ASGNP4
ADDRLP4 20
INDIRP4
ADDRLP4 20
INDIRP4
INDIRF4
CNSTF4 1096810496
ADDRFP4 8
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
INDIRF4
MULF4
ADDF4
ASGNF4
line 2700
;2699:	// snap to integer coordinates for more efficient network bandwidth usage
;2700:	SnapVector( muzzlePoint );
ADDRLP4 24
ADDRFP4 20
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 28
ADDRFP4 20
INDIRP4
CNSTI4 4
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 32
ADDRFP4 20
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
line 2701
;2701:}
LABELV $804
endproc CalcMuzzlePointOrigin 36 0
export FireWeapon
proc FireWeapon 16 20
line 2710
;2702:
;2703:
;2704:
;2705:/*
;2706:===============
;2707:FireWeapon
;2708:===============
;2709:*/
;2710:void FireWeapon( gentity_t *ent, qboolean altFire ) {
line 2711
;2711:	if (ent->client->ps.powerups[PW_QUAD] ) {
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 348
ADDP4
INDIRI4
CNSTI4 0
EQI4 $806
line 2712
;2712:		s_quadFactor = g_quadfactor.value;
ADDRGP4 s_quadFactor
ADDRGP4 g_quadfactor+8
INDIRF4
ASGNF4
line 2713
;2713:	} else {
ADDRGP4 $807
JUMPV
LABELV $806
line 2714
;2714:		s_quadFactor = 1;
ADDRGP4 s_quadFactor
CNSTF4 1065353216
ASGNF4
line 2715
;2715:	}
LABELV $807
line 2718
;2716:
;2717:	// track shots taken for accuracy tracking.  Grapple is not a weapon and gauntet is just not tracked
;2718:	if( ent->s.weapon != WP_SABER && ent->s.weapon != WP_STUN_BATON ) 
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 2
EQI4 $809
ADDRLP4 0
INDIRI4
CNSTI4 1
EQI4 $809
line 2719
;2719:	{
line 2720
;2720:		if( ent->s.weapon == WP_FLECHETTE ) {
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 9
NEI4 $811
line 2721
;2721:			ent->client->accuracy_shots += FLECHETTE_SHOTS;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1652
ADDP4
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 5
ADDI4
ASGNI4
line 2722
;2722:		} else {
ADDRGP4 $812
JUMPV
LABELV $811
line 2723
;2723:			ent->client->accuracy_shots++;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1652
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
line 2724
;2724:		}
LABELV $812
line 2725
;2725:	}
LABELV $809
line 2728
;2726:
;2727:	// set aiming directions
;2728:	if (ent->s.weapon == WP_EMPLACED_GUN)
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 14
NEI4 $813
line 2729
;2729:	{
line 2732
;2730:		vec3_t viewAngCap;
;2731:
;2732:		VectorCopy(ent->client->ps.viewangles, viewAngCap);
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 156
ADDP4
INDIRB
ASGNB 12
line 2733
;2733:		if (viewAngCap[PITCH] > 40)
ADDRLP4 4
INDIRF4
CNSTF4 1109393408
LEF4 $815
line 2734
;2734:		{
line 2735
;2735:			viewAngCap[PITCH] = 40;
ADDRLP4 4
CNSTF4 1109393408
ASGNF4
line 2736
;2736:		}
LABELV $815
line 2737
;2737:		AngleVectors( viewAngCap, forward, right, up );
ADDRLP4 4
ARGP4
ADDRGP4 forward
ARGP4
ADDRGP4 right
ARGP4
ADDRGP4 up
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 2738
;2738:	}
ADDRGP4 $814
JUMPV
LABELV $813
line 2740
;2739:	else
;2740:	{
line 2741
;2741:		AngleVectors( ent->client->ps.viewangles, forward, right, up );
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 156
ADDP4
ARGP4
ADDRGP4 forward
ARGP4
ADDRGP4 right
ARGP4
ADDRGP4 up
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 2742
;2742:	}
LABELV $814
line 2744
;2743:
;2744:	if (ent->client->ps.usingATST)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1304
ADDP4
INDIRI4
CNSTI4 0
EQI4 $817
line 2745
;2745:	{
line 2746
;2746:		gentity_t *headEnt = &g_entities[ent->client->damageBoxHandle_Head];
ADDRLP4 4
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
ASGNP4
line 2748
;2747:
;2748:		if (headEnt && headEnt->s.number >= MAX_CLIENTS)
ADDRLP4 8
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $819
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 32
LTI4 $819
line 2749
;2749:		{
line 2750
;2750:			if (altFire)
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $821
line 2751
;2751:			{
line 2752
;2752:				headEnt->bolt_Waist = 0;
ADDRLP4 4
INDIRP4
CNSTI4 804
ADDP4
CNSTI4 0
ASGNI4
line 2753
;2753:			}
ADDRGP4 $822
JUMPV
LABELV $821
line 2755
;2754:			else
;2755:			{
line 2756
;2756:				headEnt->bolt_Waist = 1;
ADDRLP4 4
INDIRP4
CNSTI4 804
ADDP4
CNSTI4 1
ASGNI4
line 2757
;2757:			}
LABELV $822
line 2758
;2758:		}
LABELV $819
line 2759
;2759:	}
LABELV $817
line 2762
;2760:
;2761://	CalcMuzzlePointOrigin ( ent, ent->client->oldOrigin, forward, right, up, muzzle );
;2762:	CalcMuzzlePoint ( ent, forward, right, up, muzzle );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 forward
ARGP4
ADDRGP4 right
ARGP4
ADDRGP4 up
ARGP4
ADDRGP4 muzzle
ARGP4
ADDRGP4 CalcMuzzlePoint
CALLV
pop
line 2764
;2763:
;2764:	if (ent->client->ps.usingATST)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1304
ADDP4
INDIRI4
CNSTI4 0
EQI4 $823
line 2765
;2765:	{
line 2766
;2766:		WP_FireBryarPistol( ent, qfalse );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 WP_FireBryarPistol
CALLV
pop
line 2767
;2767:		return;
ADDRGP4 $805
JUMPV
LABELV $823
line 2771
;2768:	}
;2769:
;2770:	// fire the specific weapon
;2771:	switch( ent->s.weapon ) {
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 1
LTI4 $826
ADDRLP4 4
INDIRI4
CNSTI4 14
GTI4 $826
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $842-4
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $842
address $828
address $826
address $830
address $831
address $832
address $833
address $834
address $835
address $836
address $837
address $838
address $839
address $840
address $841
code
LABELV $828
line 2773
;2772:	case WP_STUN_BATON:
;2773:		WP_FireStunBaton( ent, altFire );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 WP_FireStunBaton
CALLV
pop
line 2774
;2774:		break;
ADDRGP4 $826
JUMPV
line 2777
;2775:
;2776:	case WP_SABER:
;2777:		break;
LABELV $830
line 2780
;2778:
;2779:	case WP_BRYAR_PISTOL:
;2780:		WP_FireBryarPistol( ent, altFire );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 WP_FireBryarPistol
CALLV
pop
line 2781
;2781:		break;
ADDRGP4 $826
JUMPV
LABELV $831
line 2784
;2782:
;2783:	case WP_BLASTER:
;2784:		WP_FireBlaster( ent, altFire );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 WP_FireBlaster
CALLV
pop
line 2785
;2785:		break;
ADDRGP4 $826
JUMPV
LABELV $832
line 2788
;2786:
;2787:	case WP_DISRUPTOR:
;2788:		WP_FireDisruptor( ent, altFire );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 WP_FireDisruptor
CALLV
pop
line 2789
;2789:		break;
ADDRGP4 $826
JUMPV
LABELV $833
line 2792
;2790:
;2791:	case WP_BOWCASTER:
;2792:		WP_FireBowcaster( ent, altFire );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 WP_FireBowcaster
CALLV
pop
line 2793
;2793:		break;
ADDRGP4 $826
JUMPV
LABELV $834
line 2796
;2794:
;2795:	case WP_REPEATER:
;2796:		WP_FireRepeater( ent, altFire );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 WP_FireRepeater
CALLV
pop
line 2797
;2797:		break;
ADDRGP4 $826
JUMPV
LABELV $835
line 2800
;2798:
;2799:	case WP_DEMP2:
;2800:		WP_FireDEMP2( ent, altFire );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 WP_FireDEMP2
CALLV
pop
line 2801
;2801:		break;
ADDRGP4 $826
JUMPV
LABELV $836
line 2804
;2802:
;2803:	case WP_FLECHETTE:
;2804:		WP_FireFlechette( ent, altFire );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 WP_FireFlechette
CALLV
pop
line 2805
;2805:		break;
ADDRGP4 $826
JUMPV
LABELV $837
line 2808
;2806:
;2807:	case WP_ROCKET_LAUNCHER:
;2808:		WP_FireRocket( ent, altFire );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 WP_FireRocket
CALLV
pop
line 2809
;2809:		break;
ADDRGP4 $826
JUMPV
LABELV $838
line 2812
;2810:
;2811:	case WP_THERMAL:
;2812:		WP_FireThermalDetonator( ent, altFire );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 WP_FireThermalDetonator
CALLP4
pop
line 2813
;2813:		break;
ADDRGP4 $826
JUMPV
LABELV $839
line 2816
;2814:
;2815:	case WP_TRIP_MINE:
;2816:		WP_PlaceLaserTrap( ent, altFire );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 WP_PlaceLaserTrap
CALLV
pop
line 2817
;2817:		break;
ADDRGP4 $826
JUMPV
LABELV $840
line 2820
;2818:
;2819:	case WP_DET_PACK:
;2820:		WP_DropDetPack( ent, altFire );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 WP_DropDetPack
CALLV
pop
line 2821
;2821:		break;
ADDRGP4 $826
JUMPV
LABELV $841
line 2824
;2822:
;2823:	case WP_EMPLACED_GUN:
;2824:		WP_FireEmplaced( ent, altFire );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 WP_FireEmplaced
CALLV
pop
line 2825
;2825:		break;
line 2828
;2826:	default:
;2827:// FIXME		G_Error( "Bad ent->s.weapon" );
;2828:		break;
LABELV $826
line 2831
;2829:	}
;2830:
;2831:	G_LogWeaponFire(ent->s.number, ent->s.weapon);
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
INDIRI4
ARGI4
ADDRLP4 12
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_LogWeaponFire
CALLV
pop
line 2832
;2832:}
LABELV $805
endproc FireWeapon 16 20
proc WP_FireEmplaced 64 20
line 2837
;2833:
;2834://---------------------------------------------------------
;2835:static void WP_FireEmplaced( gentity_t *ent, qboolean altFire )
;2836://---------------------------------------------------------
;2837:{
line 2843
;2838:	vec3_t	dir, angs, gunpoint; //g2r , gunaxis;
;2839:	vec3_t	right;
;2840:	gentity_t *gun;
;2841://	mdxaBone_t matrix;
;2842:
;2843:	if (!ent->client)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $845
line 2844
;2844:	{
line 2845
;2845:		return;
ADDRGP4 $844
JUMPV
LABELV $845
line 2848
;2846:	}
;2847:
;2848:	if (!ent->client->ps.emplacedIndex)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 596
ADDP4
INDIRI4
CNSTI4 0
NEI4 $847
line 2849
;2849:	{
line 2850
;2850:		return;
ADDRGP4 $844
JUMPV
LABELV $847
line 2853
;2851:	}
;2852:
;2853:	gun = &g_entities[ent->client->ps.emplacedIndex];
ADDRLP4 0
CNSTI4 828
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 596
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 2855
;2854:
;2855:	if (!gun)
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $849
line 2856
;2856:	{
line 2857
;2857:		return;
ADDRGP4 $844
JUMPV
LABELV $849
line 2860
;2858:	}
;2859:
;2860:	VectorCopy(/*ent->client->ps.origin*/gun->s.origin, gunpoint);
ADDRLP4 4
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 2861
;2861:	gunpoint[2] += 46;
ADDRLP4 4+8
ADDRLP4 4+8
INDIRF4
CNSTF4 1110966272
ADDF4
ASGNF4
line 2863
;2862:
;2863:	AngleVectors(ent->client->ps.viewangles, NULL, right, NULL);
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 156
ADDP4
ARGP4
ADDRLP4 52
CNSTP4 0
ASGNP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRLP4 16
ARGP4
ADDRLP4 52
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 2865
;2864:
;2865:	if (gun->bolt_Waist)
ADDRLP4 0
INDIRP4
CNSTI4 804
ADDP4
INDIRI4
CNSTI4 0
EQI4 $852
line 2866
;2866:	{
line 2867
;2867:		gunpoint[0] += right[0]*10;
ADDRLP4 4
ADDRLP4 4
INDIRF4
CNSTF4 1092616192
ADDRLP4 16
INDIRF4
MULF4
ADDF4
ASGNF4
line 2868
;2868:		gunpoint[1] += right[1]*10;
ADDRLP4 4+4
ADDRLP4 4+4
INDIRF4
CNSTF4 1092616192
ADDRLP4 16+4
INDIRF4
MULF4
ADDF4
ASGNF4
line 2869
;2869:		gunpoint[2] += right[2]*10;
ADDRLP4 4+8
ADDRLP4 4+8
INDIRF4
CNSTF4 1092616192
ADDRLP4 16+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 2871
;2870:
;2871:		gun->bolt_Waist = 0;
ADDRLP4 0
INDIRP4
CNSTI4 804
ADDP4
CNSTI4 0
ASGNI4
line 2872
;2872:		G_AddEvent(gun, EV_FIRE_WEAPON, 0);
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 24
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 2873
;2873:	}
ADDRGP4 $853
JUMPV
LABELV $852
line 2875
;2874:	else
;2875:	{
line 2876
;2876:		gunpoint[0] -= right[0]*10;
ADDRLP4 4
ADDRLP4 4
INDIRF4
CNSTF4 1092616192
ADDRLP4 16
INDIRF4
MULF4
SUBF4
ASGNF4
line 2877
;2877:		gunpoint[1] -= right[1]*10;
ADDRLP4 4+4
ADDRLP4 4+4
INDIRF4
CNSTF4 1092616192
ADDRLP4 16+4
INDIRF4
MULF4
SUBF4
ASGNF4
line 2878
;2878:		gunpoint[2] -= right[2]*10;
ADDRLP4 4+8
ADDRLP4 4+8
INDIRF4
CNSTF4 1092616192
ADDRLP4 16+8
INDIRF4
MULF4
SUBF4
ASGNF4
line 2879
;2879:		gun->bolt_Waist = 1;
ADDRLP4 0
INDIRP4
CNSTI4 804
ADDP4
CNSTI4 1
ASGNI4
line 2880
;2880:		G_AddEvent(gun, EV_FIRE_WEAPON, 1);
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 24
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 2881
;2881:	}
LABELV $853
line 2883
;2882:
;2883:	vectoangles( forward, angs );
ADDRGP4 forward
ARGP4
ADDRLP4 28
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 2885
;2884:
;2885:	if ( altFire )
ADDRFP4 4
INDIRI4
CNSTI4 0
EQI4 $862
line 2886
;2886:	{
line 2888
;2887:		// add some slop to the alt-fire direction
;2888:		angs[PITCH] += crandom() * BLASTER_SPREAD;
ADDRLP4 56
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 28
ADDRLP4 28
INDIRF4
CNSTF4 1070386381
CNSTF4 1073741824
ADDRLP4 56
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
ASGNF4
line 2889
;2889:		angs[YAW]	+= crandom() * BLASTER_SPREAD;
ADDRLP4 60
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 28+4
ADDRLP4 28+4
INDIRF4
CNSTF4 1070386381
CNSTF4 1073741824
ADDRLP4 60
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
ASGNF4
line 2890
;2890:	}
LABELV $862
line 2892
;2891:
;2892:	AngleVectors( angs, dir, NULL, NULL );
ADDRLP4 28
ARGP4
ADDRLP4 40
ARGP4
ADDRLP4 56
CNSTP4 0
ASGNP4
ADDRLP4 56
INDIRP4
ARGP4
ADDRLP4 56
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 2896
;2893:
;2894:	// FIXME: if temp_org does not have clear trace to inside the bbox, don't shoot!
;2895:	//WP_FireEmplacedMissile( ent, gunpoint, dir, altFire, gun );
;2896:	WP_FireEmplacedMissile( gun, gunpoint, dir, altFire, ent );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 40
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 WP_FireEmplacedMissile
CALLV
pop
line 2898
;2897:	//WP_FireTurretMissile(gun, gunpoint, dir, altFire, 15, 2000, MOD_BLASTER, ent);
;2898:}
LABELV $844
endproc WP_FireEmplaced 64 20
export emplaced_gun_use
proc emplaced_gun_use 108 16
line 2911
;2899:
;2900:#define EMPLACED_CANRESPAWN 1
;2901:
;2902://----------------------------------------------------------
;2903:
;2904:/*QUAKED emplaced_gun (0 0 1) (-30 -20 8) (30 20 60) CANRESPAWN
;2905:
;2906: count - if CANRESPAWN spawnflag, decides how long it is before gun respawns (in ms)
;2907:*/
;2908: 
;2909://----------------------------------------------------------
;2910:void emplaced_gun_use( gentity_t *self, gentity_t *other, trace_t *trace )
;2911:{
line 2915
;2912:	vec3_t fwd1, fwd2;
;2913:	float dot;
;2914:	int oldWeapon;
;2915:	gentity_t *activator = other;
ADDRLP4 0
ADDRFP4 4
INDIRP4
ASGNP4
line 2916
;2916:	float zoffset = 50;
ADDRLP4 64
CNSTF4 1112014848
ASGNF4
line 2919
;2917:	vec3_t anglesToOwner;
;2918:	vec3_t vLen;
;2919:	float ownLen = 0;
ADDRLP4 56
CNSTF4 0
ASGNF4
line 2921
;2920:
;2921:	if ( self->health <= 0 )
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 0
GTI4 $866
line 2922
;2922:	{
line 2924
;2923:		// can't use a dead gun.
;2924:		return;
ADDRGP4 $865
JUMPV
LABELV $866
line 2927
;2925:	}
;2926:
;2927:	if (self->activator)
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $868
line 2928
;2928:	{
line 2929
;2929:		return;
ADDRGP4 $865
JUMPV
LABELV $868
line 2932
;2930:	}
;2931:
;2932:	if (!activator->client)
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $870
line 2933
;2933:	{
line 2934
;2934:		return;
ADDRGP4 $865
JUMPV
LABELV $870
line 2937
;2935:	}
;2936:
;2937:	if (activator->client->ps.emplacedTime > level.time)
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 600
ADDP4
INDIRF4
ADDRGP4 level+32
INDIRI4
CVIF4 4
LEF4 $872
line 2938
;2938:	{
line 2939
;2939:		return;
ADDRGP4 $865
JUMPV
LABELV $872
line 2942
;2940:	}
;2941:
;2942:	if (activator->client->ps.weaponTime > 0)
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 44
ADDP4
INDIRI4
CNSTI4 0
LEI4 $875
line 2943
;2943:	{
line 2944
;2944:		return;
ADDRGP4 $865
JUMPV
LABELV $875
line 2947
;2945:	}
;2946:
;2947:	if (activator->client->ps.origin[2] > self->s.origin[2]+zoffset-8)
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRLP4 64
INDIRF4
ADDF4
CNSTF4 1090519040
SUBF4
LEF4 $877
line 2948
;2948:	{
line 2949
;2949:		return;
ADDRGP4 $865
JUMPV
LABELV $877
line 2952
;2950:	} //can't use it from the top
;2951:
;2952:	if (activator->client->ps.pm_flags & PMF_DUCKED)
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $879
line 2953
;2953:	{
line 2954
;2954:		return;
ADDRGP4 $865
JUMPV
LABELV $879
line 2957
;2955:	}
;2956:
;2957:	if (activator->client->ps.isJediMaster)
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 604
ADDP4
INDIRI4
CNSTI4 0
EQI4 $881
line 2958
;2958:	{ //;O
line 2959
;2959:		return;
ADDRGP4 $865
JUMPV
LABELV $881
line 2962
;2960:	}
;2961:
;2962:	VectorSubtract(self->s.origin, activator->client->ps.origin, vLen);
ADDRLP4 68
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 72
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
ASGNP4
ADDRLP4 44
ADDRLP4 68
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRLP4 72
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 44+4
ADDRLP4 68
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRLP4 72
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 44+8
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
SUBF4
ASGNF4
line 2963
;2963:	ownLen = VectorLength(vLen);
ADDRLP4 44
ARGP4
ADDRLP4 76
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 56
ADDRLP4 76
INDIRF4
ASGNF4
line 2965
;2964:
;2965:	if (ownLen > 64)
ADDRLP4 56
INDIRF4
CNSTF4 1115684864
LEF4 $885
line 2966
;2966:	{
line 2967
;2967:		return;
ADDRGP4 $865
JUMPV
LABELV $885
line 2971
;2968:	}
;2969:
;2970:	// Let's get some direction vectors for the users
;2971:	AngleVectors( activator->client->ps.viewangles, fwd1, NULL, NULL );
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 156
ADDP4
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
line 2974
;2972:
;2973:	// Get the guns direction vector
;2974:	AngleVectors( self->pos1, fwd2, NULL, NULL );
ADDRFP4 0
INDIRP4
CNSTI4 548
ADDP4
ARGP4
ADDRLP4 16
ARGP4
ADDRLP4 84
CNSTP4 0
ASGNP4
ADDRLP4 84
INDIRP4
ARGP4
ADDRLP4 84
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 2976
;2975:
;2976:	dot = DotProduct( fwd1, fwd2 );
ADDRLP4 40
ADDRLP4 4
INDIRF4
ADDRLP4 16
INDIRF4
MULF4
ADDRLP4 4+4
INDIRF4
ADDRLP4 16+4
INDIRF4
MULF4
ADDF4
ADDRLP4 4+8
INDIRF4
ADDRLP4 16+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 2979
;2977:
;2978:	// Must be reasonably facing the way the gun points ( 110 degrees or so ), otherwise we don't allow to use it.
;2979:	if ( dot < -0.2f )
ADDRLP4 40
INDIRF4
CNSTF4 3192704205
GEF4 $891
line 2980
;2980:	{
line 2981
;2981:		return;
ADDRGP4 $865
JUMPV
LABELV $891
line 2984
;2982:	}
;2983:
;2984:	VectorSubtract(self->s.origin, activator->client->ps.origin, fwd1);
ADDRLP4 88
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 92
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
ASGNP4
ADDRLP4 4
ADDRLP4 88
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRLP4 92
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 88
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRLP4 92
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+8
ADDRFP4 0
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
SUBF4
ASGNF4
line 2985
;2985:	VectorNormalize(fwd1);
ADDRLP4 4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 2987
;2986:
;2987:	dot = DotProduct( fwd1, fwd2 );
ADDRLP4 40
ADDRLP4 4
INDIRF4
ADDRLP4 16
INDIRF4
MULF4
ADDRLP4 4+4
INDIRF4
ADDRLP4 16+4
INDIRF4
MULF4
ADDF4
ADDRLP4 4+8
INDIRF4
ADDRLP4 16+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 2990
;2988:
;2989:	// Must be reasonably facing the way the gun points ( 110 degrees or so ), otherwise we don't allow to use it.
;2990:	if ( dot < /*-0.2f*/0.6f/*0.8f*/ )
ADDRLP4 40
INDIRF4
CNSTF4 1058642330
GEF4 $899
line 2991
;2991:	{
line 2992
;2992:		return;
ADDRGP4 $865
JUMPV
LABELV $899
line 2995
;2993:	}
;2994:
;2995:	self->boltpoint1 = 1;
ADDRFP4 0
INDIRP4
CNSTI4 768
ADDP4
CNSTI4 1
ASGNI4
line 3000
;2996:
;2997:	// don't allow using it again for half a second
;2998://	if ( activator->s.number == 0 && self->delay + 500 < level.time )
;2999://	{
;3000:		oldWeapon = activator->s.weapon;
ADDRLP4 60
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
ASGNI4
line 3003
;3001:
;3002:		// swap the users weapon with the emplaced gun and add the ammo the gun has to the player
;3003:		activator->client->ps.weapon = self->s.weapon;
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 148
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
ASGNI4
line 3004
;3004:		activator->client->ps.weaponstate = WEAPON_READY;
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 152
ADDP4
CNSTI4 0
ASGNI4
line 3005
;3005:		activator->client->ps.stats[STAT_WEAPONS] |= ( 1 << WP_EMPLACED_GUN );
ADDRLP4 96
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 232
ADDP4
ASGNP4
ADDRLP4 96
INDIRP4
ADDRLP4 96
INDIRP4
INDIRI4
CNSTI4 16384
BORI4
ASGNI4
line 3009
;3006:
;3007:		//SnapVector(self->s.origin);
;3008:
;3009:		VectorCopy(activator->client->ps.origin, self->s.origin2);
ADDRFP4 0
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 3011
;3010:
;3011:		activator->client->ps.emplacedIndex = self->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 596
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 3013
;3012:
;3013:		self->s.emplacedOwner = activator->s.number;
ADDRFP4 0
INDIRP4
CNSTI4 180
ADDP4
ADDRLP4 0
INDIRP4
INDIRI4
ASGNI4
line 3014
;3014:		self->s.activeForcePass = NUM_FORCE_POWERS+1;
ADDRFP4 0
INDIRP4
CNSTI4 176
ADDP4
CNSTI4 19
ASGNI4
line 3017
;3015:
;3016:		// the gun will track which weapon we used to have
;3017:		self->s.weapon = oldWeapon;
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
ADDRLP4 60
INDIRI4
ASGNI4
line 3021
;3018:
;3019:		// Lock the player
;3020://		activator->client->ps.eFlags |= EF_LOCKED_TO_WEAPON;
;3021:		activator->r.ownerNum = self->s.number; // kind of dumb, but when we are locked to the weapon, we are owned by it.
ADDRLP4 0
INDIRP4
CNSTI4 396
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 3022
;3022:		self->activator = activator;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 3047
;3023://		self->delay = level.time; // can't disconnect from the thing for half a second
;3024:
;3025:		// Let the client know that we want to start our emplaced camera clamping
;3026:		// FIXME:  if you are in the gun and you switch/restart maps, emplacedClamp will still be 1 and since
;3027:		//	you can't change it from the console, you are stuck with really bad viewangles
;3028://		char	temp[32];
;3029://		gi.cvar_set("cl_emplacedClamp", "1");
;3030://		sprintf( temp, "%f", self->pos1[0] );
;3031://		gi.cvar_set("cl_emplacedPitch", temp );
;3032://		sprintf( temp, "%f", self->pos1[1] );
;3033://		gi.cvar_set("cl_emplacedYaw", temp );
;3034:
;3035:		// Let the gun be considered an enemy
;3036://		self->svFlags |= SVF_NONNPC_ENEMY;
;3037:
;3038:		// move the player to the center of the gun and make player not solid
;3039://		activator->contents = 0;
;3040://		VectorCopy( self->currentOrigin, activator->client->ps.origin );
;3041:
;3042:		// FIXME: trying to force the gun to look forward, but it seems to pick up the players viewangles....and
;3043:		//	since you usually go up to the side of the gun to use it, you end up starting with a really annoying
;3044:		//	set of viewangles.
;3045:		//G_SetAngles( activator, self->s.angles );
;3046:
;3047:		VectorSubtract(self->r.currentOrigin, activator->client->ps.origin, anglesToOwner);
ADDRLP4 100
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 104
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
ASGNP4
ADDRLP4 28
ADDRLP4 100
INDIRP4
CNSTI4 368
ADDP4
INDIRF4
ADDRLP4 104
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 28+4
ADDRLP4 100
INDIRP4
CNSTI4 372
ADDP4
INDIRF4
ADDRLP4 104
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 28+8
ADDRFP4 0
INDIRP4
CNSTI4 376
ADDP4
INDIRF4
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3048
;3048:		vectoangles(anglesToOwner, anglesToOwner);
ADDRLP4 28
ARGP4
ADDRLP4 28
ARGP4
ADDRGP4 vectoangles
CALLV
pop
line 3058
;3049:
;3050:		//SetClientViewAngle(activator, /*self->s.angles*/anglesToOwner);
;3051:
;3052://		VectorCopy(activator->s.angles, self->pos1);
;3053:
;3054:		// Overriding these may be a bad thing....
;3055://		gi.cvar_set("cg_thirdPersonRange", "20");
;3056://		gi.cvar_set("cg_thirdPersonVertOffset", "35");
;3057://	}
;3058:}
LABELV $865
endproc emplaced_gun_use 108 16
export emplaced_gun_realuse
proc emplaced_gun_realuse 0 12
line 3061
;3059:
;3060:void emplaced_gun_realuse( gentity_t *self, gentity_t *other, gentity_t *activator )
;3061:{
line 3062
;3062:	emplaced_gun_use(self, other, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 emplaced_gun_use
CALLV
pop
line 3063
;3063:}
LABELV $903
endproc emplaced_gun_realuse 0 12
export emplaced_gun_pain
proc emplaced_gun_pain 0 0
line 3067
;3064:
;3065://----------------------------------------------------------
;3066:void emplaced_gun_pain( gentity_t *self, gentity_t *attacker, int damage )
;3067:{
line 3068
;3068:	if ( self->health <= 0 )
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 0
GTI4 $905
line 3069
;3069:	{
line 3071
;3070:		// play pain effect?
;3071:	}
LABELV $905
line 3073
;3072:	else
;3073:	{
line 3081
;3074://		if ( self->paintarget )
;3075://		{
;3076://			G_UseTargets2( self, self->activator, self->paintarget );
;3077://		}
;3078:
;3079:		//Don't do script if dead
;3080://		G_ActivateBehavior( self, BSET_PAIN );
;3081:	}
LABELV $906
line 3082
;3082:}
LABELV $904
endproc emplaced_gun_pain 0 0
export emplaced_gun_update
proc emplaced_gun_update 104 24
line 3088
;3083:
;3084:#define EMPLACED_GUN_HEALTH 800
;3085:
;3086://----------------------------------------------------------
;3087:void emplaced_gun_update(gentity_t *self)
;3088:{
line 3091
;3089:	vec3_t	smokeOrg, puffAngle;
;3090:	int oldWeap;
;3091:	float ownLen = 0;
ADDRLP4 12
CNSTF4 0
ASGNF4
line 3093
;3092:
;3093:	if (self->health < 1 && !self->bolt_Head)
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 1
GEI4 $908
ADDRLP4 32
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
CNSTI4 0
NEI4 $908
line 3094
;3094:	{
line 3095
;3095:		if (self->spawnflags & EMPLACED_CANRESPAWN)
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $909
line 3096
;3096:		{
line 3097
;3097:			self->bolt_Head = level.time + 4000 + self->count;
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
CNSTI4 784
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 4000
ADDI4
ADDRLP4 36
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
ADDI4
ASGNI4
line 3098
;3098:		}
line 3099
;3099:	}
ADDRGP4 $909
JUMPV
LABELV $908
line 3100
;3100:	else if (self->health < 1 && self->bolt_Head < level.time)
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 1
GEI4 $913
ADDRLP4 36
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $913
line 3101
;3101:	{
line 3102
;3102:		self->s.time = 0;
ADDRFP4 0
INDIRP4
CNSTI4 84
ADDP4
CNSTI4 0
ASGNI4
line 3103
;3103:		self->boltpoint4 = 0;
ADDRFP4 0
INDIRP4
CNSTI4 780
ADDP4
CNSTI4 0
ASGNI4
line 3104
;3104:		self->boltpoint3 = 0;
ADDRFP4 0
INDIRP4
CNSTI4 776
ADDP4
CNSTI4 0
ASGNI4
line 3105
;3105:		self->health = EMPLACED_GUN_HEALTH*0.4;
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 320
ASGNI4
line 3106
;3106:	}
LABELV $913
LABELV $909
line 3108
;3107:
;3108:	if (self->boltpoint4 && self->boltpoint4 < 2 && self->s.time < level.time)
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
ADDRLP4 40
INDIRP4
CNSTI4 780
ADDP4
INDIRI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
EQI4 $916
ADDRLP4 44
INDIRI4
CNSTI4 2
GEI4 $916
ADDRLP4 40
INDIRP4
CNSTI4 84
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $916
line 3109
;3109:	{
line 3112
;3110:		vec3_t explOrg;
;3111:
;3112:		VectorSet( puffAngle, 0, 0, 1 );
ADDRLP4 60
CNSTF4 0
ASGNF4
ADDRLP4 0
ADDRLP4 60
INDIRF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 60
INDIRF4
ASGNF4
ADDRLP4 0+8
CNSTF4 1065353216
ASGNF4
line 3114
;3113:
;3114:		VectorCopy(self->r.currentOrigin, explOrg);
ADDRLP4 48
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 3115
;3115:		explOrg[2] += 16;
ADDRLP4 48+8
ADDRLP4 48+8
INDIRF4
CNSTF4 1098907648
ADDF4
ASGNF4
line 3118
;3116:
;3117:		//G_PlayEffect(EFFECT_EXPLOSION, explOrg, /*self->r.currentAngles*/puffAngle);
;3118:		G_PlayEffect(EFFECT_EXPLOSION_DETPACK, explOrg, /*self->r.currentAngles*/puffAngle);
CNSTI4 6
ARGI4
ADDRLP4 48
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_PlayEffect
CALLP4
pop
line 3120
;3119:
;3120:		self->boltpoint3 = level.time + Q_irand(2500, 3500);
CNSTI4 2500
ARGI4
CNSTI4 3500
ARGI4
ADDRLP4 64
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 776
ADDP4
ADDRGP4 level+32
INDIRI4
ADDRLP4 64
INDIRI4
ADDI4
ASGNI4
line 3122
;3121:
;3122:		G_RadiusDamage(self->r.currentOrigin, self, self->splashDamage, self->splashRadius, self, MOD_UNKNOWN);
ADDRLP4 68
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 68
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 68
INDIRP4
ARGP4
ADDRLP4 68
INDIRP4
CNSTI4 696
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 68
INDIRP4
CNSTI4 700
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 68
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 G_RadiusDamage
CALLI4
pop
line 3124
;3123:
;3124:		self->s.time = -1;
ADDRFP4 0
INDIRP4
CNSTI4 84
ADDP4
CNSTI4 -1
ASGNI4
line 3126
;3125:
;3126:		self->boltpoint4 = 2;
ADDRFP4 0
INDIRP4
CNSTI4 780
ADDP4
CNSTI4 2
ASGNI4
line 3127
;3127:	}
LABELV $916
line 3129
;3128:
;3129:	if (self->boltpoint3 > level.time)
ADDRFP4 0
INDIRP4
CNSTI4 776
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
LEI4 $923
line 3130
;3130:	{
line 3131
;3131:		if (self->boltpoint2 < level.time)
ADDRFP4 0
INDIRP4
CNSTI4 772
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GEI4 $926
line 3132
;3132:		{
line 3133
;3133:			VectorSet( puffAngle, 0, 0, 1 );
ADDRLP4 48
CNSTF4 0
ASGNF4
ADDRLP4 0
ADDRLP4 48
INDIRF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 48
INDIRF4
ASGNF4
ADDRLP4 0+8
CNSTF4 1065353216
ASGNF4
line 3134
;3134:			VectorCopy(self->r.currentOrigin, smokeOrg);
ADDRLP4 20
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 3136
;3135:
;3136:			smokeOrg[2] += 60;
ADDRLP4 20+8
ADDRLP4 20+8
INDIRF4
CNSTF4 1114636288
ADDF4
ASGNF4
line 3139
;3137:
;3138:			//What.. was I thinking?
;3139:			G_PlayEffect(EFFECT_SMOKE, smokeOrg, puffAngle);
CNSTI4 1
ARGI4
ADDRLP4 20
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 G_PlayEffect
CALLP4
pop
line 3141
;3140:
;3141:			self->boltpoint2 = level.time + Q_irand(250, 400);
CNSTI4 250
ARGI4
CNSTI4 400
ARGI4
ADDRLP4 52
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 772
ADDP4
ADDRGP4 level+32
INDIRI4
ADDRLP4 52
INDIRI4
ADDI4
ASGNI4
line 3145
;3142:			//This would be much better if we checked a value on the entity on the client
;3143:			//and then spawned smoke there instead of sending over a bunch of events. But
;3144:			//this will do for now, an event every 250-400ms isn't too bad.
;3145:		}
LABELV $926
line 3146
;3146:	}
LABELV $923
line 3148
;3147:
;3148:	if (self->activator && self->activator->client && self->activator->inuse)
ADDRLP4 48
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
ASGNP4
ADDRLP4 52
CNSTU4 0
ASGNU4
ADDRLP4 48
INDIRP4
CVPU4 4
ADDRLP4 52
INDIRU4
EQU4 $933
ADDRLP4 48
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 52
INDIRU4
EQU4 $933
ADDRLP4 48
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $933
line 3149
;3149:	{
line 3151
;3150:		vec3_t vLen;
;3151:		VectorSubtract(self->s.origin, self->activator->client->ps.origin, vLen);
ADDRLP4 68
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 72
ADDRLP4 68
INDIRP4
CNSTI4 732
ADDP4
ASGNP4
ADDRLP4 76
CNSTI4 408
ASGNI4
ADDRLP4 56
ADDRLP4 68
INDIRP4
CNSTI4 92
ADDP4
INDIRF4
ADDRLP4 72
INDIRP4
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 56+4
ADDRLP4 68
INDIRP4
CNSTI4 96
ADDP4
INDIRF4
ADDRLP4 72
INDIRP4
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 80
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 56+8
ADDRLP4 80
INDIRP4
CNSTI4 100
ADDP4
INDIRF4
ADDRLP4 80
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
SUBF4
ASGNF4
line 3152
;3152:		ownLen = VectorLength(vLen);
ADDRLP4 56
ARGP4
ADDRLP4 84
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 12
ADDRLP4 84
INDIRF4
ASGNF4
line 3154
;3153:
;3154:		if (!(self->activator->client->pers.cmd.buttons & BUTTON_USE) && self->boltpoint1)
ADDRLP4 88
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 92
CNSTI4 0
ASGNI4
ADDRLP4 88
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1388
ADDP4
INDIRI4
CNSTI4 32
BANDI4
ADDRLP4 92
INDIRI4
NEI4 $937
ADDRLP4 88
INDIRP4
CNSTI4 768
ADDP4
INDIRI4
ADDRLP4 92
INDIRI4
EQI4 $937
line 3155
;3155:		{
line 3156
;3156:			self->boltpoint1 = 0;
ADDRFP4 0
INDIRP4
CNSTI4 768
ADDP4
CNSTI4 0
ASGNI4
line 3157
;3157:		}
LABELV $937
line 3159
;3158:
;3159:		if ((self->activator->client->pers.cmd.buttons & BUTTON_USE) && !self->boltpoint1)
ADDRLP4 96
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 100
CNSTI4 0
ASGNI4
ADDRLP4 96
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1388
ADDP4
INDIRI4
CNSTI4 32
BANDI4
ADDRLP4 100
INDIRI4
EQI4 $939
ADDRLP4 96
INDIRP4
CNSTI4 768
ADDP4
INDIRI4
ADDRLP4 100
INDIRI4
NEI4 $939
line 3160
;3160:		{
line 3161
;3161:			self->activator->client->ps.emplacedIndex = 0;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 596
ADDP4
CNSTI4 0
ASGNI4
line 3162
;3162:			self->nextthink = level.time + 50;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 50
ADDI4
ASGNI4
line 3163
;3163:			return;
ADDRGP4 $907
JUMPV
LABELV $939
line 3165
;3164:		}
;3165:	}
LABELV $933
line 3167
;3166:
;3167:	if ((self->activator && self->activator->client) &&
ADDRLP4 56
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
ADDRLP4 56
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
ASGNP4
ADDRLP4 64
CNSTU4 0
ASGNU4
ADDRLP4 60
INDIRP4
CVPU4 4
ADDRLP4 64
INDIRU4
EQU4 $942
ADDRLP4 68
ADDRLP4 60
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 68
INDIRP4
CVPU4 4
ADDRLP4 64
INDIRU4
EQU4 $942
ADDRLP4 72
CNSTI4 0
ASGNI4
ADDRLP4 60
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
ADDRLP4 72
INDIRI4
EQI4 $946
ADDRLP4 68
INDIRP4
CNSTI4 596
ADDP4
INDIRI4
ADDRLP4 56
INDIRP4
INDIRI4
NEI4 $946
ADDRLP4 56
INDIRP4
CNSTI4 780
ADDP4
INDIRI4
ADDRLP4 72
INDIRI4
NEI4 $946
ADDRLP4 12
INDIRF4
CNSTF4 1115684864
LEF4 $942
LABELV $946
line 3169
;3168:		(!self->activator->inuse || self->activator->client->ps.emplacedIndex != self->s.number ||	self->boltpoint4 || ownLen > 64))
;3169:	{
line 3170
;3170:		if (self->activator->client->ps.stats[STAT_WEAPONS] & ( 1 << WP_EMPLACED_GUN ))
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 232
ADDP4
INDIRI4
CNSTI4 16384
BANDI4
CNSTI4 0
EQI4 $947
line 3171
;3171:		{
line 3172
;3172:			self->activator->client->ps.stats[STAT_WEAPONS] -= ( 1 << WP_EMPLACED_GUN );
ADDRLP4 76
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 232
ADDP4
ASGNP4
ADDRLP4 76
INDIRP4
ADDRLP4 76
INDIRP4
INDIRI4
CNSTI4 16384
SUBI4
ASGNI4
line 3173
;3173:		}
LABELV $947
line 3175
;3174:		//VectorCopy(self->s.origin2, self->activator->client->ps.origin);
;3175:		oldWeap = self->activator->client->ps.weapon;
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 148
ADDP4
INDIRI4
ASGNI4
line 3176
;3176:		self->activator->client->ps.weapon = self->s.weapon;
ADDRLP4 76
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 76
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 148
ADDP4
ADDRLP4 76
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
ASGNI4
line 3177
;3177:		self->s.weapon = oldWeap;
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
ADDRLP4 16
INDIRI4
ASGNI4
line 3178
;3178:		self->activator->r.ownerNum = ENTITYNUM_NONE;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CNSTI4 396
ADDP4
CNSTI4 1023
ASGNI4
line 3179
;3179:		self->activator->client->ps.emplacedTime = level.time + 1000;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 600
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1000
ADDI4
CVIF4 4
ASGNF4
line 3180
;3180:		self->activator->client->ps.emplacedIndex = 0;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 596
ADDP4
CNSTI4 0
ASGNI4
line 3181
;3181:		self->activator = NULL;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
CNSTP4 0
ASGNP4
line 3183
;3182:
;3183:		self->s.activeForcePass = 0;
ADDRFP4 0
INDIRP4
CNSTI4 176
ADDP4
CNSTI4 0
ASGNI4
line 3184
;3184:	}
ADDRGP4 $943
JUMPV
LABELV $942
line 3185
;3185:	else if (self->activator && self->activator->client)
ADDRLP4 76
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
ASGNP4
ADDRLP4 80
CNSTU4 0
ASGNU4
ADDRLP4 76
INDIRP4
CVPU4 4
ADDRLP4 80
INDIRU4
EQU4 $950
ADDRLP4 76
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 80
INDIRU4
EQU4 $950
line 3186
;3186:	{
line 3187
;3187:		self->activator->client->ps.weapon = WP_EMPLACED_GUN;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 14
ASGNI4
line 3188
;3188:		self->activator->client->ps.weaponstate = WEAPON_READY;
ADDRFP4 0
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 152
ADDP4
CNSTI4 0
ASGNI4
line 3189
;3189:	}
LABELV $950
LABELV $943
line 3190
;3190:	self->nextthink = level.time + 50;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 50
ADDI4
ASGNI4
line 3191
;3191:}
LABELV $907
endproc emplaced_gun_update 104 24
export emplaced_gun_die
proc emplaced_gun_die 0 0
line 3195
;3192:
;3193://----------------------------------------------------------
;3194:void emplaced_gun_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod )
;3195:{
line 3196
;3196:	if (self->boltpoint4)
ADDRFP4 0
INDIRP4
CNSTI4 780
ADDP4
INDIRI4
CNSTI4 0
EQI4 $954
line 3197
;3197:	{
line 3198
;3198:		return;
ADDRGP4 $953
JUMPV
LABELV $954
line 3201
;3199:	}
;3200:
;3201:	self->boltpoint4 = 1;
ADDRFP4 0
INDIRP4
CNSTI4 780
ADDP4
CNSTI4 1
ASGNI4
line 3203
;3202:
;3203:	self->s.time = level.time + 3000;
ADDRFP4 0
INDIRP4
CNSTI4 84
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 3000
ADDI4
ASGNI4
line 3205
;3204:
;3205:	self->bolt_Head = 0;
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
CNSTI4 0
ASGNI4
line 3206
;3206:}
LABELV $953
endproc emplaced_gun_die 0 0
lit
align 1
LABELV $958
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
char 1 116
char 1 117
char 1 114
char 1 114
char 1 101
char 1 116
char 1 95
char 1 99
char 1 104
char 1 97
char 1 105
char 1 114
char 1 46
char 1 103
char 1 108
char 1 109
char 1 0
export SP_emplaced_gun
code
proc SP_emplaced_gun 1172 28
line 3209
;3207:
;3208:void SP_emplaced_gun( gentity_t *ent )
;3209:{
line 3211
;3210:	//char name[] = "models/map_objects/imp_mine/turret_chair.glm";
;3211:	char name[] = "models/map_objects/mp/turret_chair.glm";
ADDRLP4 1092
ADDRGP4 $958
INDIRB
ASGNB 39
line 3215
;3212:	vec3_t down;
;3213:	trace_t tr;
;3214:
;3215:	RegisterItem( BG_FindItemForWeapon(WP_BLASTER) );
CNSTI4 4
ARGI4
ADDRLP4 1132
ADDRGP4 BG_FindItemForWeapon
CALLP4
ASGNP4
ADDRLP4 1132
INDIRP4
ARGP4
ADDRGP4 RegisterItem
CALLV
pop
line 3218
;3216:	//Emplaced gun uses many of the same assets as the blaster, so just precache it
;3217:
;3218:	ent->r.contents = CONTENTS_SOLID;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1
ASGNI4
line 3219
;3219:	ent->s.solid = SOLID_BBOX;
ADDRFP4 0
INDIRP4
CNSTI4 248
ADDP4
CNSTI4 2
ASGNI4
line 3221
;3220:
;3221:	ent->bolt_Head = 0;
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
CNSTI4 0
ASGNI4
line 3223
;3222:
;3223:	VectorSet( ent->r.mins, -30, -20, 8 );
ADDRFP4 0
INDIRP4
CNSTI4 316
ADDP4
CNSTF4 3253731328
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 320
ADDP4
CNSTF4 3248488448
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 324
ADDP4
CNSTF4 1090519040
ASGNF4
line 3224
;3224:	VectorSet( ent->r.maxs, 30, 20, 60 );
ADDRFP4 0
INDIRP4
CNSTI4 328
ADDP4
CNSTF4 1106247680
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 332
ADDP4
CNSTF4 1101004800
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 1114636288
ASGNF4
line 3226
;3225:
;3226:	VectorCopy(ent->s.origin, down);
ADDRLP4 1080
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
INDIRB
ASGNB 12
line 3228
;3227:
;3228:	down[2] -= 1024;
ADDRLP4 1080+8
ADDRLP4 1080+8
INDIRF4
CNSTF4 1149239296
SUBF4
ASGNF4
line 3230
;3229:
;3230:	trap_Trace(&tr, ent->s.origin, ent->r.mins, ent->r.maxs, down, ent->s.number, MASK_SOLID);
ADDRLP4 0
ARGP4
ADDRLP4 1136
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1136
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRLP4 1136
INDIRP4
CNSTI4 316
ADDP4
ARGP4
ADDRLP4 1136
INDIRP4
CNSTI4 328
ADDP4
ARGP4
ADDRLP4 1080
ARGP4
ADDRLP4 1136
INDIRP4
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 3232
;3231:
;3232:	if (tr.fraction != 1 && !tr.allsolid && !tr.startsolid)
ADDRLP4 0+8
INDIRF4
CNSTF4 1065353216
EQF4 $960
ADDRLP4 1140
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 1140
INDIRI4
NEI4 $960
ADDRLP4 0+4
INDIRI4
ADDRLP4 1140
INDIRI4
NEI4 $960
line 3233
;3233:	{
line 3234
;3234:		VectorCopy(tr.endpos, ent->s.origin);
ADDRFP4 0
INDIRP4
CNSTI4 92
ADDP4
ADDRLP4 0+12
INDIRB
ASGNB 12
line 3235
;3235:	}
LABELV $960
line 3237
;3236:
;3237:	ent->spawnflags |= 4; // deadsolid
ADDRLP4 1144
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
ASGNP4
ADDRLP4 1144
INDIRP4
ADDRLP4 1144
INDIRP4
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 3239
;3238:
;3239:	ent->health = EMPLACED_GUN_HEALTH;
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 800
ASGNI4
line 3241
;3240:
;3241:	if (ent->spawnflags & EMPLACED_CANRESPAWN)
ADDRFP4 0
INDIRP4
CNSTI4 420
ADDP4
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
EQI4 $965
line 3242
;3242:	{ //make it somewhat easier to kill if it can respawn
line 3243
;3243:		ent->health *= 0.4;
ADDRLP4 1148
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
ASGNP4
ADDRLP4 1148
INDIRP4
CNSTF4 1053609165
ADDRLP4 1148
INDIRP4
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 3244
;3244:	}
LABELV $965
line 3246
;3245:
;3246:	ent->boltpoint4 = 0;
ADDRFP4 0
INDIRP4
CNSTI4 780
ADDP4
CNSTI4 0
ASGNI4
line 3248
;3247:
;3248:	ent->takedamage = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 1
ASGNI4
line 3249
;3249:	ent->pain = emplaced_gun_pain;
ADDRFP4 0
INDIRP4
CNSTI4 656
ADDP4
ADDRGP4 emplaced_gun_pain
ASGNP4
line 3250
;3250:	ent->die = emplaced_gun_die;
ADDRFP4 0
INDIRP4
CNSTI4 660
ADDP4
ADDRGP4 emplaced_gun_die
ASGNP4
line 3253
;3251:
;3252:	// being caught in this thing when it blows would be really bad.
;3253:	ent->splashDamage = 80;
ADDRFP4 0
INDIRP4
CNSTI4 696
ADDP4
CNSTI4 80
ASGNI4
line 3254
;3254:	ent->splashRadius = 128;
ADDRFP4 0
INDIRP4
CNSTI4 700
ADDP4
CNSTI4 128
ASGNI4
line 3260
;3255:
;3256://	G_EffectIndex( "emplaced/explode" );
;3257://	G_EffectIndex( "emplaced/dead_smoke" );
;3258:
;3259:	// amount of ammo that this little poochie has
;3260:	G_SpawnInt( "count", "600", &ent->count );
ADDRGP4 $967
ARGP4
ADDRGP4 $968
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 712
ADDP4
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 3262
;3261:
;3262:	ent->s.modelindex = G_ModelIndex( name );
ADDRLP4 1092
ARGP4
ADDRLP4 1148
ADDRGP4 G_ModelIndex
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 212
ADDP4
ADDRLP4 1148
INDIRI4
ASGNI4
line 3263
;3263:	ent->s.modelGhoul2 = 1;
ADDRFP4 0
INDIRP4
CNSTI4 204
ADDP4
CNSTI4 1
ASGNI4
line 3264
;3264:	ent->s.g2radius = 110;
ADDRFP4 0
INDIRP4
CNSTI4 208
ADDP4
CNSTI4 110
ASGNI4
line 3275
;3265:	//trap_G2API_InitGhoul2Model( ent->s.ghoul2, name, ent->s.modelindex );
;3266://g2r	trap_G2API_InitGhoul2Model( &ent->s, name, ent->s.modelindex, 0, 0, 0, 0 );
;3267:
;3268:	// Activate our tags and bones
;3269://	ent->headBolt = gi.G2API_AddBolt( &ent->s.ghoul2[0], "*seat" );
;3270://	ent->handLBolt = gi.G2API_AddBolt( &ent->s.ghoul2[0], "*flash01" );
;3271://	ent->handRBolt = gi.G2API_AddBolt( &ent->s.ghoul2[0], "*flash02" );
;3272://	gi.G2API_SetBoneAngles( &ent->s.ghoul2[0], "swivel_bone", vec3_origin, BONE_ANGLES_POSTMULT, POSITIVE_Y, POSITIVE_Z, POSITIVE_X, NULL); 
;3273:
;3274://	RegisterItem( FindItemForWeapon( WP_EMPLACED_GUN ));
;3275:	ent->s.weapon = WP_EMPLACED_GUN;
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 14
ASGNI4
line 3279
;3276:
;3277://	SnapVector(ent->s.origin);
;3278:
;3279:	G_SetOrigin( ent, ent->s.origin );
ADDRLP4 1152
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1152
INDIRP4
ARGP4
ADDRLP4 1152
INDIRP4
CNSTI4 92
ADDP4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 3284
;3280:	
;3281:	//G_SetAngles( ent, ent->s.angles );
;3282:
;3283:	// store base angles for later
;3284:	VectorCopy( ent->s.angles, ent->pos1 );
ADDRLP4 1156
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1156
INDIRP4
CNSTI4 548
ADDP4
ADDRLP4 1156
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 3285
;3285:	VectorCopy( ent->s.angles, ent->r.currentAngles );
ADDRLP4 1160
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1160
INDIRP4
CNSTI4 380
ADDP4
ADDRLP4 1160
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 3286
;3286:	VectorCopy( ent->s.angles, ent->s.apos.trBase );
ADDRLP4 1164
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1164
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 1164
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 3288
;3287:
;3288:	ent->think = emplaced_gun_update;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 emplaced_gun_update
ASGNP4
line 3289
;3289:	ent->nextthink = level.time + 50;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 50
ADDI4
ASGNI4
line 3292
;3290:
;3291://	ent->e_UseFunc = useF_emplaced_gun_use;
;3292:	ent->use = emplaced_gun_realuse;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 emplaced_gun_realuse
ASGNP4
line 3295
;3293:	//ent->touch = emplaced_gun_use;
;3294:
;3295:	ent->r.svFlags |= SVF_PLAYER_USABLE;
ADDRLP4 1168
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 1168
INDIRP4
ADDRLP4 1168
INDIRP4
INDIRI4
CNSTI4 16
BORI4
ASGNI4
line 3297
;3296:
;3297:	ent->s.pos.trType = TR_STATIONARY;
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 3299
;3298:
;3299:	ent->s.owner = MAX_CLIENTS+1;
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
CNSTI4 33
ASGNI4
line 3300
;3300:	ent->s.shouldtarget = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 268
ADDP4
CNSTI4 1
ASGNI4
line 3301
;3301:	ent->s.teamowner = 0;
ADDRFP4 0
INDIRP4
CNSTI4 264
ADDP4
CNSTI4 0
ASGNI4
line 3315
;3302:
;3303:	/*
;3304:	angswiv[ROLL] = 0;
;3305:	angswiv[PITCH] = 0;
;3306:	angswiv[YAW] = 70;
;3307:	trap_G2API_SetBoneAngles(ent->s.ghoul2, 0, "swivel_bone", angswiv, BONE_ANGLES_REPLACE, POSITIVE_Z, NEGATIVE_X, NEGATIVE_Y, NULL, 0, level.time);
;3308:	*/
;3309:
;3310:
;3311://g2r	ent->s.trickedentindex = trap_G2API_AddBolt(ent->s.ghoul2, 0, "*seat");
;3312://g2r	ent->s.bolt1 = trap_G2API_AddBolt(ent->s.ghoul2, 0, "*flash01");
;3313://g2r	ent->s.bolt2 = trap_G2API_AddBolt(ent->s.ghoul2, 0, "*flash02");
;3314:
;3315:	trap_LinkEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 3316
;3316:}
LABELV $957
endproc SP_emplaced_gun 1172 28
bss
align 4
LABELV ent_list
skip 4096
import G_GetHitLocation
import G_BoxInBounds
align 4
LABELV muzzle
skip 12
align 4
LABELV up
skip 12
align 4
LABELV right
skip 12
align 4
LABELV forward
skip 12
align 4
LABELV s_quadFactor
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
import G_PlayerBecomeATST
import ATST_ManageDamageBoxes
import TeleportPlayer
import trigger_teleporter_touch
import Touch_DoorTrigger
import G_RunMover
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
LABELV $968
char 1 54
char 1 48
char 1 48
char 1 0
align 1
LABELV $967
char 1 99
char 1 111
char 1 117
char 1 110
char 1 116
char 1 0
align 1
LABELV $761
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
char 1 109
char 1 101
char 1 108
char 1 101
char 1 101
char 1 47
char 1 112
char 1 117
char 1 110
char 1 99
char 1 104
char 1 37
char 1 100
char 1 0
align 1
LABELV $684
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
char 1 119
char 1 97
char 1 114
char 1 110
char 1 105
char 1 110
char 1 103
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $668
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
LABELV $666
char 1 100
char 1 101
char 1 116
char 1 112
char 1 97
char 1 99
char 1 107
char 1 0
align 1
LABELV $654
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
LABELV $597
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
LABELV $596
char 1 108
char 1 97
char 1 115
char 1 101
char 1 114
char 1 84
char 1 114
char 1 97
char 1 112
char 1 0
align 1
LABELV $588
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
LABELV $576
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
char 1 119
char 1 97
char 1 114
char 1 110
char 1 105
char 1 110
char 1 103
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $546
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
char 1 108
char 1 111
char 1 111
char 1 112
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $530
char 1 116
char 1 104
char 1 101
char 1 114
char 1 109
char 1 97
char 1 108
char 1 95
char 1 100
char 1 101
char 1 116
char 1 111
char 1 110
char 1 97
char 1 116
char 1 111
char 1 114
char 1 0
align 1
LABELV $510
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
char 1 116
char 1 104
char 1 101
char 1 114
char 1 109
char 1 97
char 1 108
char 1 47
char 1 119
char 1 97
char 1 114
char 1 110
char 1 105
char 1 110
char 1 103
char 1 46
char 1 119
char 1 97
char 1 118
char 1 0
align 1
LABELV $504
char 1 114
char 1 111
char 1 99
char 1 107
char 1 101
char 1 116
char 1 95
char 1 112
char 1 114
char 1 111
char 1 106
char 1 0
align 1
LABELV $424
char 1 102
char 1 108
char 1 101
char 1 99
char 1 104
char 1 95
char 1 97
char 1 108
char 1 116
char 1 0
align 1
LABELV $389
char 1 102
char 1 108
char 1 101
char 1 99
char 1 104
char 1 95
char 1 112
char 1 114
char 1 111
char 1 106
char 1 0
align 1
LABELV $376
char 1 100
char 1 101
char 1 109
char 1 112
char 1 50
char 1 95
char 1 97
char 1 108
char 1 116
char 1 95
char 1 112
char 1 114
char 1 111
char 1 106
char 1 0
align 1
LABELV $300
char 1 100
char 1 101
char 1 109
char 1 112
char 1 50
char 1 95
char 1 112
char 1 114
char 1 111
char 1 106
char 1 0
align 1
LABELV $294
char 1 114
char 1 101
char 1 112
char 1 101
char 1 97
char 1 116
char 1 101
char 1 114
char 1 95
char 1 97
char 1 108
char 1 116
char 1 95
char 1 112
char 1 114
char 1 111
char 1 106
char 1 0
align 1
LABELV $292
char 1 114
char 1 101
char 1 112
char 1 101
char 1 97
char 1 116
char 1 101
char 1 114
char 1 95
char 1 112
char 1 114
char 1 111
char 1 106
char 1 0
align 1
LABELV $287
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
char 1 97
char 1 108
char 1 116
char 1 95
char 1 112
char 1 114
char 1 111
char 1 106
char 1 0
align 1
LABELV $265
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
char 1 112
char 1 114
char 1 111
char 1 106
char 1 0
align 1
LABELV $119
char 1 101
char 1 109
char 1 112
char 1 108
char 1 97
char 1 99
char 1 101
char 1 100
char 1 95
char 1 103
char 1 117
char 1 110
char 1 95
char 1 112
char 1 114
char 1 111
char 1 106
char 1 0
align 1
LABELV $117
char 1 98
char 1 108
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 95
char 1 112
char 1 114
char 1 111
char 1 106
char 1 0
align 1
LABELV $112
char 1 103
char 1 101
char 1 110
char 1 101
char 1 114
char 1 105
char 1 99
char 1 95
char 1 112
char 1 114
char 1 111
char 1 106
char 1 0
align 1
LABELV $99
char 1 98
char 1 114
char 1 121
char 1 97
char 1 114
char 1 95
char 1 112
char 1 114
char 1 111
char 1 106
char 1 0
