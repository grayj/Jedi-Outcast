export ObjectDie
code
proc ObjectDie 0 8
file "../g_combat.c"
line 14
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:// g_combat.c
;4:
;5:#include "g_local.h"
;6:
;7://rww - pd
;8:void BotDamageNotification(gclient_t *bot, gentity_t *attacker);
;9://end rww
;10:
;11:void ThrowSaberToAttacker(gentity_t *self, gentity_t *attacker);
;12:
;13:void ObjectDie (gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath )
;14:{
line 15
;15:	if(self->target)
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $79
line 16
;16:	{
line 17
;17:		G_UseTargets(self, attacker);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 G_UseTargets
CALLV
pop
line 18
;18:	}
LABELV $79
line 21
;19:
;20:	//remove my script_targetname
;21:	G_FreeEntity( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 22
;22:}
LABELV $78
endproc ObjectDie 0 8
export G_GetHitLocation
proc G_GetHitLocation 172 16
line 152
;23:
;24:/*
;25:int G_GetHitLocFromSurfName( gentity_t *ent, const char *surfName, vec3_t point )
;26:{
;27:	if ( !surfName || !surfName[0] )
;28:	{
;29:		return HL_NONE;
;30:	}
;31:	if ( !Q_strncmp( "hips", surfName, 4 ) )
;32:	{//FIXME: test properly for legs
;33:		if ( Q_irand( 0, 1 ) )
;34:		{
;35:			return HL_WAIST;
;36:		}
;37:		else if ( Q_irand( 0, 1 ) )
;38:		{
;39:			return HL_LEG_RT;
;40:		}
;41:		else
;42:		{
;43:			return HL_LEG_LT;
;44:		}
;45:	}
;46:	else if ( !Q_strncmp( "torso", surfName, 5 ) )
;47:	{
;48:		if ( !ent->client )
;49:		{
;50:			return HL_CHEST;
;51:		}
;52:		else
;53:		{
;54:			vec3_t	t_fwd, t_rt, t_up, dirToImpact;
;55:			float frontSide, rightSide, upSide;
;56:			AngleVectors( ent->client->renderInfo.torsoAngles, t_fwd, t_rt, t_up );
;57:			VectorSubtract( point, ent->client->renderInfo.torsoPoint, dirToImpact );
;58:			frontSide = DotProduct( t_fwd, dirToImpact );
;59:			rightSide = DotProduct( t_rt, dirToImpact );
;60:			upSide = DotProduct( t_up, dirToImpact );
;61:			if ( upSide < 0 )
;62:			{//hit at waist
;63:				return HL_WAIST;
;64:			}
;65:			else
;66:			{//hit on upper torso
;67:				if ( rightSide > 10 )
;68:				{
;69:					return HL_ARM_RT;
;70:				}
;71:				else if ( rightSide < -10 )
;72:				{
;73:					return HL_ARM_LT;
;74:				}
;75:				else if ( rightSide > 4 )
;76:				{
;77:					if ( frontSide > 0 )
;78:					{
;79:						return HL_CHEST_RT;
;80:					}
;81:					else
;82:					{
;83:						return HL_BACK_RT;
;84:					}
;85:				}
;86:				else if ( rightSide < -4 )
;87:				{
;88:					if ( frontSide > 0 )
;89:					{
;90:						return HL_CHEST_LT;
;91:					}
;92:					else
;93:					{
;94:						return HL_BACK_LT;
;95:					}
;96:				}
;97:				else if ( upSide > 6 )
;98:				{
;99:					return HL_HEAD;
;100:				}
;101:				else if ( frontSide > 0 )
;102:				{
;103:					return HL_CHEST;
;104:				}
;105:				else
;106:				{
;107:					return HL_BACK;
;108:				}
;109:			}
;110:		}
;111:	}
;112:	else if ( !Q_strncmp( "head", surfName, 4 ) )
;113:	{
;114:		return HL_HEAD;
;115:	}
;116:	else if ( !Q_strncmp( "r_arm", surfName, 5 ) )
;117:	{
;118:		return HL_ARM_RT;
;119:	}
;120:	else if ( !Q_strncmp( "l_arm", surfName, 5 ) )
;121:	{
;122:		return HL_ARM_LT;
;123:	}
;124:	else if ( !Q_strncmp( "r_leg", surfName, 5 ) )
;125:	{
;126:		if ( ent->client && DistanceSquared( ent->client->renderInfo.crotchPoint, point ) > 144 )
;127:		{//not close enough to the pelvis to be a dismemberment hit and we don't dismember at the ankle or knee
;128:			return HL_FOOT_RT;
;129:		}
;130:		return HL_LEG_RT;
;131:	}
;132:	else if ( !Q_strncmp( "l_leg", surfName, 5 ) )
;133:	{
;134:		if ( ent->client && DistanceSquared( ent->client->renderInfo.crotchPoint, point ) > 144 )
;135:		{//not close enough to the pelvis to be a dismemberment hit and we don't dismember at the ankle or knee
;136:			return HL_FOOT_LT;
;137:		}
;138:		return HL_LEG_LT;
;139:	}
;140:	else if ( !Q_strncmp( "r_hand", surfName, 6 ) )
;141:	{
;142:		return HL_HAND_RT;
;143:	}
;144:	else if ( !Q_strncmp( "l_hand", surfName, 6 ) )
;145:	{
;146:		return HL_HAND_LT;
;147:	}
;148:	return HL_NONE;
;149:}
;150:*/
;151:int G_GetHitLocation(gentity_t *target, vec3_t ppoint)
;152:{
line 162
;153:	vec3_t			point, point_dir;
;154:	vec3_t			forward, right, up;
;155:	vec3_t			tangles, tcenter;
;156:	float			tradius;
;157:	float			udot, fdot, rdot;
;158:	int				Vertical, Forward, Lateral;
;159:	int				HitLoc;
;160:
;161:	// Get target forward, right and up.
;162:	if(target->client)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $82
line 163
;163:	{
line 165
;164:		// Ignore player's pitch and roll.
;165:		VectorSet(tangles, 0, target->r.currentAngles[YAW], 0);
ADDRLP4 88
CNSTF4 0
ASGNF4
ADDRLP4 88+4
ADDRFP4 0
INDIRP4
CNSTI4 384
ADDP4
INDIRF4
ASGNF4
ADDRLP4 88+8
CNSTF4 0
ASGNF4
line 166
;166:	}
LABELV $82
line 168
;167:
;168:	AngleVectors(tangles, forward, right, up);
ADDRLP4 88
ARGP4
ADDRLP4 28
ARGP4
ADDRLP4 40
ARGP4
ADDRLP4 52
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 171
;169:
;170:	// Get center of target.
;171:	VectorAdd(target->r.absmin, target->r.absmax, tcenter);
ADDRLP4 116
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
ADDRLP4 116
INDIRP4
CNSTI4 344
ADDP4
INDIRF4
ADDRLP4 116
INDIRP4
CNSTI4 356
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 116
INDIRP4
CNSTI4 348
ADDP4
INDIRF4
ADDRLP4 116
INDIRP4
CNSTI4 360
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 120
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12+8
ADDRLP4 120
INDIRP4
CNSTI4 352
ADDP4
INDIRF4
ADDRLP4 120
INDIRP4
CNSTI4 364
ADDP4
INDIRF4
ADDF4
ASGNF4
line 172
;172:	VectorScale(tcenter, 0.5, tcenter);
ADDRLP4 124
CNSTF4 1056964608
ASGNF4
ADDRLP4 12
ADDRLP4 124
INDIRF4
ADDRLP4 12
INDIRF4
MULF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 124
INDIRF4
ADDRLP4 12+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 12+8
CNSTF4 1056964608
ADDRLP4 12+8
INDIRF4
MULF4
ASGNF4
line 175
;173:
;174:	// Get radius width of target.
;175:	tradius = (fabs(target->r.maxs[0]) + fabs(target->r.maxs[1]) + fabs(target->r.mins[0]) + fabs(target->r.mins[1]))/4;
ADDRFP4 0
INDIRP4
CNSTI4 328
ADDP4
INDIRF4
ARGF4
ADDRLP4 128
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 332
ADDP4
INDIRF4
ARGF4
ADDRLP4 132
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 316
ADDP4
INDIRF4
ARGF4
ADDRLP4 136
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 320
ADDP4
INDIRF4
ARGF4
ADDRLP4 140
ADDRGP4 fabs
CALLF4
ASGNF4
ADDRLP4 112
ADDRLP4 128
INDIRF4
ADDRLP4 132
INDIRF4
ADDF4
ADDRLP4 136
INDIRF4
ADDF4
ADDRLP4 140
INDIRF4
ADDF4
CNSTF4 1082130432
DIVF4
ASGNF4
line 178
;176:
;177:	// Get impact point.
;178:	if(ppoint && !VectorCompare(ppoint, vec3_origin))
ADDRLP4 144
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 144
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $92
ADDRLP4 144
INDIRP4
ARGP4
ADDRGP4 vec3_origin
ARGP4
ADDRLP4 148
ADDRGP4 VectorCompare
CALLI4
ASGNI4
ADDRLP4 148
INDIRI4
CNSTI4 0
NEI4 $92
line 179
;179:	{
line 180
;180:		VectorCopy(ppoint, point);
ADDRLP4 68
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 181
;181:	}
ADDRGP4 $93
JUMPV
LABELV $92
line 183
;182:	else
;183:	{
line 184
;184:		return HL_NONE;
CNSTI4 0
RETI4
ADDRGP4 $81
JUMPV
LABELV $93
line 206
;185:	}
;186:
;187:/*
;188://get impact dir
;189:	if(pdir && !VectorCompare(pdir, vec3_origin))
;190:	{
;191:		VectorCopy(pdir, dir);
;192:	}
;193:	else
;194:	{
;195:		return;
;196:	}
;197:
;198://put point at controlled distance from center
;199:	VectorSubtract(point, tcenter, tempvec);
;200:	tempvec[2] = 0;
;201:	hdist = VectorLength(tempvec);
;202:
;203:	VectorMA(point, hdist - tradius, dir, point);
;204:	//now a point on the surface of a cylinder with a radius of tradius
;205:*/	
;206:	VectorSubtract(point, tcenter, point_dir);
ADDRLP4 0
ADDRLP4 68
INDIRF4
ADDRLP4 12
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 68+4
INDIRF4
ADDRLP4 12+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 68+8
INDIRF4
ADDRLP4 12+8
INDIRF4
SUBF4
ASGNF4
line 207
;207:	VectorNormalize(point_dir);
ADDRLP4 0
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 210
;208:
;209:	// Get bottom to top (vertical) position index
;210:	udot = DotProduct(up, point_dir);
ADDRLP4 80
ADDRLP4 52
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDRLP4 52+4
INDIRF4
ADDRLP4 0+4
INDIRF4
MULF4
ADDF4
ADDRLP4 52+8
INDIRF4
ADDRLP4 0+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 211
;211:	if(udot>.800)
ADDRLP4 80
INDIRF4
CNSTF4 1061997773
LEF4 $104
line 212
;212:	{
line 213
;213:		Vertical = 4;
ADDRLP4 100
CNSTI4 4
ASGNI4
line 214
;214:	}
ADDRGP4 $105
JUMPV
LABELV $104
line 215
;215:	else if(udot>.400)
ADDRLP4 80
INDIRF4
CNSTF4 1053609165
LEF4 $106
line 216
;216:	{
line 217
;217:		Vertical = 3;
ADDRLP4 100
CNSTI4 3
ASGNI4
line 218
;218:	}
ADDRGP4 $107
JUMPV
LABELV $106
line 219
;219:	else if(udot>-.333)
ADDRLP4 80
INDIRF4
CNSTF4 3198844666
LEF4 $108
line 220
;220:	{
line 221
;221:		Vertical = 2;
ADDRLP4 100
CNSTI4 2
ASGNI4
line 222
;222:	}
ADDRGP4 $109
JUMPV
LABELV $108
line 223
;223:	else if(udot>-.666)
ADDRLP4 80
INDIRF4
CNSTF4 3207233274
LEF4 $110
line 224
;224:	{
line 225
;225:		Vertical = 1;
ADDRLP4 100
CNSTI4 1
ASGNI4
line 226
;226:	}
ADDRGP4 $111
JUMPV
LABELV $110
line 228
;227:	else
;228:	{
line 229
;229:		Vertical = 0;
ADDRLP4 100
CNSTI4 0
ASGNI4
line 230
;230:	}
LABELV $111
LABELV $109
LABELV $107
LABELV $105
line 233
;231:
;232:	// Get back to front (forward) position index.
;233:	fdot = DotProduct(forward, point_dir);
ADDRLP4 84
ADDRLP4 28
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDRLP4 28+4
INDIRF4
ADDRLP4 0+4
INDIRF4
MULF4
ADDF4
ADDRLP4 28+8
INDIRF4
ADDRLP4 0+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 234
;234:	if(fdot>.666)
ADDRLP4 84
INDIRF4
CNSTF4 1059749626
LEF4 $116
line 235
;235:	{
line 236
;236:		Forward = 4;
ADDRLP4 104
CNSTI4 4
ASGNI4
line 237
;237:	}
ADDRGP4 $117
JUMPV
LABELV $116
line 238
;238:	else if(fdot>.333)
ADDRLP4 84
INDIRF4
CNSTF4 1051361018
LEF4 $118
line 239
;239:	{
line 240
;240:		Forward = 3;
ADDRLP4 104
CNSTI4 3
ASGNI4
line 241
;241:	}
ADDRGP4 $119
JUMPV
LABELV $118
line 242
;242:	else if(fdot>-.333)
ADDRLP4 84
INDIRF4
CNSTF4 3198844666
LEF4 $120
line 243
;243:	{
line 244
;244:		Forward = 2;
ADDRLP4 104
CNSTI4 2
ASGNI4
line 245
;245:	}
ADDRGP4 $121
JUMPV
LABELV $120
line 246
;246:	else if(fdot>-.666)
ADDRLP4 84
INDIRF4
CNSTF4 3207233274
LEF4 $122
line 247
;247:	{
line 248
;248:		Forward = 1;
ADDRLP4 104
CNSTI4 1
ASGNI4
line 249
;249:	}
ADDRGP4 $123
JUMPV
LABELV $122
line 251
;250:	else
;251:	{
line 252
;252:		Forward = 0;
ADDRLP4 104
CNSTI4 0
ASGNI4
line 253
;253:	}
LABELV $123
LABELV $121
LABELV $119
LABELV $117
line 256
;254:
;255:	// Get left to right (lateral) position index.
;256:	rdot = DotProduct(right, point_dir);
ADDRLP4 64
ADDRLP4 40
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDRLP4 40+4
INDIRF4
ADDRLP4 0+4
INDIRF4
MULF4
ADDF4
ADDRLP4 40+8
INDIRF4
ADDRLP4 0+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 257
;257:	if(rdot>.666)
ADDRLP4 64
INDIRF4
CNSTF4 1059749626
LEF4 $128
line 258
;258:	{
line 259
;259:		Lateral = 4;
ADDRLP4 108
CNSTI4 4
ASGNI4
line 260
;260:	}
ADDRGP4 $129
JUMPV
LABELV $128
line 261
;261:	else if(rdot>.333)
ADDRLP4 64
INDIRF4
CNSTF4 1051361018
LEF4 $130
line 262
;262:	{
line 263
;263:		Lateral = 3;
ADDRLP4 108
CNSTI4 3
ASGNI4
line 264
;264:	}
ADDRGP4 $131
JUMPV
LABELV $130
line 265
;265:	else if(rdot>-.333)
ADDRLP4 64
INDIRF4
CNSTF4 3198844666
LEF4 $132
line 266
;266:	{
line 267
;267:		Lateral = 2;
ADDRLP4 108
CNSTI4 2
ASGNI4
line 268
;268:	}
ADDRGP4 $133
JUMPV
LABELV $132
line 269
;269:	else if(rdot>-.666)
ADDRLP4 64
INDIRF4
CNSTF4 3207233274
LEF4 $134
line 270
;270:	{
line 271
;271:		Lateral = 1;
ADDRLP4 108
CNSTI4 1
ASGNI4
line 272
;272:	}
ADDRGP4 $135
JUMPV
LABELV $134
line 274
;273:	else
;274:	{
line 275
;275:		Lateral = 0;
ADDRLP4 108
CNSTI4 0
ASGNI4
line 276
;276:	}
LABELV $135
LABELV $133
LABELV $131
LABELV $129
line 278
;277:
;278:	HitLoc = Vertical * 25 + Forward * 5 + Lateral;
ADDRLP4 24
CNSTI4 25
ADDRLP4 100
INDIRI4
MULI4
CNSTI4 5
ADDRLP4 104
INDIRI4
MULI4
ADDI4
ADDRLP4 108
INDIRI4
ADDI4
ASGNI4
line 280
;279:
;280:	if(HitLoc <= 10)
ADDRLP4 24
INDIRI4
CNSTI4 10
GTI4 $136
line 281
;281:	{
line 283
;282:		// Feet.
;283:		if ( rdot > 0 )
ADDRLP4 64
INDIRF4
CNSTF4 0
LEF4 $138
line 284
;284:		{
line 285
;285:			return HL_FOOT_RT;
CNSTI4 1
RETI4
ADDRGP4 $81
JUMPV
LABELV $138
line 288
;286:		}
;287:		else
;288:		{
line 289
;289:			return HL_FOOT_LT;
CNSTI4 2
RETI4
ADDRGP4 $81
JUMPV
LABELV $136
line 292
;290:		}
;291:	}
;292:	else if(HitLoc <= 50)
ADDRLP4 24
INDIRI4
CNSTI4 50
GTI4 $140
line 293
;293:	{
line 295
;294:		// Legs.
;295:		if ( rdot > 0 )
ADDRLP4 64
INDIRF4
CNSTF4 0
LEF4 $142
line 296
;296:		{
line 297
;297:			return HL_LEG_RT;
CNSTI4 3
RETI4
ADDRGP4 $81
JUMPV
LABELV $142
line 300
;298:		}
;299:		else
;300:		{
line 301
;301:			return HL_LEG_LT;
CNSTI4 4
RETI4
ADDRGP4 $81
JUMPV
LABELV $140
line 304
;302:		}
;303:	}
;304:	else if(HitLoc == 56||HitLoc == 60||HitLoc == 61||HitLoc == 65||HitLoc == 66||HitLoc == 70)
ADDRLP4 24
INDIRI4
CNSTI4 56
EQI4 $150
ADDRLP4 24
INDIRI4
CNSTI4 60
EQI4 $150
ADDRLP4 24
INDIRI4
CNSTI4 61
EQI4 $150
ADDRLP4 24
INDIRI4
CNSTI4 65
EQI4 $150
ADDRLP4 24
INDIRI4
CNSTI4 66
EQI4 $150
ADDRLP4 24
INDIRI4
CNSTI4 70
NEI4 $144
LABELV $150
line 305
;305:	{
line 307
;306:		// Hands.
;307:		if ( rdot > 0 )
ADDRLP4 64
INDIRF4
CNSTF4 0
LEF4 $151
line 308
;308:		{
line 309
;309:			return HL_HAND_RT;
CNSTI4 14
RETI4
ADDRGP4 $81
JUMPV
LABELV $151
line 312
;310:		}
;311:		else
;312:		{
line 313
;313:			return HL_HAND_LT;
CNSTI4 15
RETI4
ADDRGP4 $81
JUMPV
LABELV $144
line 316
;314:		}
;315:	}
;316:	else if(HitLoc == 83||HitLoc == 87||HitLoc == 88||HitLoc == 92||HitLoc == 93||HitLoc == 97)
ADDRLP4 24
INDIRI4
CNSTI4 83
EQI4 $159
ADDRLP4 24
INDIRI4
CNSTI4 87
EQI4 $159
ADDRLP4 24
INDIRI4
CNSTI4 88
EQI4 $159
ADDRLP4 24
INDIRI4
CNSTI4 92
EQI4 $159
ADDRLP4 24
INDIRI4
CNSTI4 93
EQI4 $159
ADDRLP4 24
INDIRI4
CNSTI4 97
NEI4 $153
LABELV $159
line 317
;317:	{
line 319
;318:		// Arms.
;319:		if ( rdot > 0 )
ADDRLP4 64
INDIRF4
CNSTF4 0
LEF4 $160
line 320
;320:		{
line 321
;321:			return HL_ARM_RT;
CNSTI4 12
RETI4
ADDRGP4 $81
JUMPV
LABELV $160
line 324
;322:		}
;323:		else
;324:		{
line 325
;325:			return HL_ARM_LT;
CNSTI4 13
RETI4
ADDRGP4 $81
JUMPV
LABELV $153
line 328
;326:		}
;327:	}
;328:	else if((HitLoc >= 107 && HitLoc <= 109)||(HitLoc >= 112 && HitLoc <= 114)||(HitLoc >= 117 && HitLoc <= 119))
ADDRLP4 24
INDIRI4
CNSTI4 107
LTI4 $165
ADDRLP4 24
INDIRI4
CNSTI4 109
LEI4 $166
LABELV $165
ADDRLP4 24
INDIRI4
CNSTI4 112
LTI4 $167
ADDRLP4 24
INDIRI4
CNSTI4 114
LEI4 $166
LABELV $167
ADDRLP4 24
INDIRI4
CNSTI4 117
LTI4 $162
ADDRLP4 24
INDIRI4
CNSTI4 119
GTI4 $162
LABELV $166
line 329
;329:	{
line 331
;330:		// Head.
;331:		return HL_HEAD;
CNSTI4 16
RETI4
ADDRGP4 $81
JUMPV
LABELV $162
line 334
;332:	}
;333:	else
;334:	{
line 335
;335:		if(udot < 0.3)
ADDRLP4 80
INDIRF4
CNSTF4 1050253722
GEF4 $168
line 336
;336:		{
line 337
;337:			return HL_WAIST;
CNSTI4 5
RETI4
ADDRGP4 $81
JUMPV
LABELV $168
line 339
;338:		}
;339:		else if(fdot < 0)
ADDRLP4 84
INDIRF4
CNSTF4 0
GEF4 $170
line 340
;340:		{
line 341
;341:			if(rdot > 0.4)
ADDRLP4 64
INDIRF4
CNSTF4 1053609165
LEF4 $172
line 342
;342:			{
line 343
;343:				return HL_BACK_RT;
CNSTI4 6
RETI4
ADDRGP4 $81
JUMPV
LABELV $172
line 345
;344:			}
;345:			else if(rdot < -0.4)
ADDRLP4 64
INDIRF4
CNSTF4 3201092813
GEF4 $174
line 346
;346:			{
line 347
;347:				return HL_BACK_LT;
CNSTI4 7
RETI4
ADDRGP4 $81
JUMPV
LABELV $174
line 349
;348:			}
;349:			else if(fdot < 0)
ADDRLP4 84
INDIRF4
CNSTF4 0
GEF4 $171
line 350
;350:			{
line 351
;351:				return HL_BACK;
CNSTI4 8
RETI4
ADDRGP4 $81
JUMPV
line 353
;352:			}
;353:		}
LABELV $170
line 355
;354:		else
;355:		{
line 356
;356:			if(rdot > 0.3)
ADDRLP4 64
INDIRF4
CNSTF4 1050253722
LEF4 $178
line 357
;357:			{
line 358
;358:				return HL_CHEST_RT;
CNSTI4 9
RETI4
ADDRGP4 $81
JUMPV
LABELV $178
line 360
;359:			}
;360:			else if(rdot < -0.3)
ADDRLP4 64
INDIRF4
CNSTF4 3197737370
GEF4 $180
line 361
;361:			{
line 362
;362:				return HL_CHEST_LT;
CNSTI4 10
RETI4
ADDRGP4 $81
JUMPV
LABELV $180
line 364
;363:			}
;364:			else if(fdot < 0)
ADDRLP4 84
INDIRF4
CNSTF4 0
GEF4 $182
line 365
;365:			{
line 366
;366:				return HL_CHEST;
CNSTI4 11
RETI4
ADDRGP4 $81
JUMPV
LABELV $182
line 368
;367:			}
;368:		}
LABELV $171
line 369
;369:	}
line 370
;370:	return HL_NONE;
CNSTI4 0
RETI4
LABELV $81
endproc G_GetHitLocation 172 16
export ExplodeDeath
proc ExplodeDeath 40 24
line 475
;371:}
;372:
;373:/*
;374:int G_PickPainAnim( gentity_t *self, vec3_t point, int damage )
;375:{
;376:	switch( G_GetHitLocation( self, point ) )
;377:	{
;378:	case HL_FOOT_RT:
;379:		return BOTH_PAIN12;
;380:		//PAIN12 = right foot
;381:		break;
;382:	case HL_FOOT_LT:
;383:		return -1;
;384:		break;
;385:	case HL_LEG_RT:
;386:		if ( !Q_irand( 0, 1 ) )
;387:		{
;388:			return BOTH_PAIN11;
;389:		}
;390:		else
;391:		{
;392:			return BOTH_PAIN13;
;393:		}
;394:		//PAIN11 = twitch right leg
;395:		//PAIN13 = right knee
;396:		break;
;397:	case HL_LEG_LT:
;398:		return BOTH_PAIN14;
;399:		//PAIN14 = twitch left leg
;400:		break;
;401:	case HL_BACK_RT:
;402:		return BOTH_PAIN7;
;403:		//PAIN7 = med left shoulder
;404:		break;
;405:	case HL_BACK_LT:
;406:		return Q_irand( BOTH_PAIN15, BOTH_PAIN16 );
;407:		//PAIN15 = med right shoulder
;408:		//PAIN16 = twitch right shoulder
;409:		break;
;410:	case HL_BACK:
;411:		if ( !Q_irand( 0, 1 ) )
;412:		{
;413:			return BOTH_PAIN1;
;414:		}
;415:		else
;416:		{
;417:			return BOTH_PAIN5;
;418:		}
;419:		//PAIN1 = back
;420:		//PAIN5 = same as 1
;421:		break;
;422:	case HL_CHEST_RT:
;423:		return BOTH_PAIN3;
;424:		//PAIN3 = long, right shoulder
;425:		break;
;426:	case HL_CHEST_LT:
;427:		return BOTH_PAIN2;
;428:		//PAIN2 = long, left shoulder
;429:		break;
;430:	case HL_WAIST:
;431:	case HL_CHEST:
;432:		if ( !Q_irand( 0, 3 ) )
;433:		{
;434:			return BOTH_PAIN6;
;435:		}
;436:		else if ( !Q_irand( 0, 2 ) )
;437:		{
;438:			return BOTH_PAIN8;
;439:		}
;440:		else if ( !Q_irand( 0, 1 ) )
;441:		{
;442:			return BOTH_PAIN17;
;443:		}
;444:		else
;445:		{
;446:			return BOTH_PAIN19;
;447:		}
;448:		//PAIN6 = gut
;449:		//PAIN8 = chest
;450:		//PAIN17 = twitch crotch
;451:		//PAIN19 = med crotch
;452:		break;
;453:	case HL_ARM_RT:
;454:	case HL_HAND_RT:
;455:		return BOTH_PAIN9;
;456:		//PAIN9 = twitch right arm
;457:		break;
;458:	case HL_ARM_LT:
;459:	case HL_HAND_LT:
;460:		return BOTH_PAIN10;
;461:		//PAIN10 = twitch left arm
;462:		break;
;463:	case HL_HEAD:
;464:		return BOTH_PAIN4;
;465:		//PAIN4 = head
;466:		break;
;467:	default:
;468:		return -1;
;469:		break;
;470:	}
;471:}
;472:*/
;473:
;474:void ExplodeDeath( gentity_t *self ) 
;475:{
line 479
;476://	gentity_t	*tent;
;477:	vec3_t		forward;
;478:
;479:	self->takedamage = qfalse;//stop chain reaction runaway loops
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 0
ASGNI4
line 481
;480:
;481:	self->s.loopSound = 0;
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
CNSTI4 0
ASGNI4
line 483
;482:
;483:	VectorCopy( self->r.currentOrigin, self->s.pos.trBase );
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 486
;484:
;485://	tent = G_TempEntity( self->s.origin, EV_FX_EXPLOSION );
;486:	AngleVectors(self->s.angles, forward, NULL, NULL);
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 16
CNSTP4 0
ASGNP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRLP4 16
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 496
;487:
;488:/*	
;489:	if ( self->fxID > 0 )
;490:	{
;491:		G_PlayEffect( self->fxID, self->r.currentOrigin, forward );
;492:	}
;493:	else
;494:	*/
;495:
;496:	{
line 499
;497://		CG_SurfaceExplosion( self->r.currentOrigin, forward, 20.0f, 12.0f, ((self->spawnflags&4)==qfalse) );	//FIXME: This needs to be consistent to all exploders!
;498://		G_Sound(self, self->sounds );
;499:	}
line 501
;500:	
;501:	if(self->splashDamage > 0 && self->splashRadius > 0)
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 24
CNSTI4 0
ASGNI4
ADDRLP4 20
INDIRP4
CNSTI4 696
ADDP4
INDIRI4
ADDRLP4 24
INDIRI4
LEI4 $185
ADDRLP4 20
INDIRP4
CNSTI4 700
ADDP4
INDIRI4
ADDRLP4 24
INDIRI4
LEI4 $185
line 502
;502:	{
line 503
;503:		gentity_t *attacker = self;
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
line 504
;504:		if ( self->parent )
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $187
line 505
;505:		{
line 506
;506:			attacker = self->parent;
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 536
ADDP4
INDIRP4
ASGNP4
line 507
;507:		}
LABELV $187
line 508
;508:		G_RadiusDamage( self->r.currentOrigin, attacker, self->splashDamage, self->splashRadius, 
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 368
ADDP4
ARGP4
ADDRLP4 36
ADDRLP4 28
INDIRP4
ASGNP4
ADDRLP4 36
INDIRP4
ARGP4
ADDRLP4 32
INDIRP4
CNSTI4 696
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 32
INDIRP4
CNSTI4 700
ADDP4
INDIRI4
CVIF4 4
ARGF4
ADDRLP4 36
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRGP4 G_RadiusDamage
CALLI4
pop
line 510
;509:				attacker, MOD_UNKNOWN );
;510:	}
LABELV $185
line 512
;511:
;512:	ObjectDie( self, self, self, 20, 0 );
ADDRLP4 28
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRLP4 28
INDIRP4
ARGP4
CNSTI4 20
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 ObjectDie
CALLV
pop
line 513
;513:}
LABELV $184
endproc ExplodeDeath 40 24
export ScorePlum
proc ScorePlum 12 8
line 521
;514:
;515:
;516:/*
;517:============
;518:ScorePlum
;519:============
;520:*/
;521:void ScorePlum( gentity_t *ent, vec3_t origin, int score ) {
line 524
;522:	gentity_t *plum;
;523:
;524:	plum = G_TempEntity( origin, EV_SCOREPLUM );
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 86
ARGI4
ADDRLP4 4
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 526
;525:	// only send this temp entity to a single client
;526:	plum->r.svFlags |= SVF_SINGLECLIENT;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 256
BORI4
ASGNI4
line 527
;527:	plum->r.singleClient = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 308
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 529
;528:	//
;529:	plum->s.otherEntityNum = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 184
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 530
;530:	plum->s.time = score;
ADDRLP4 0
INDIRP4
CNSTI4 84
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 531
;531:}
LABELV $189
endproc ScorePlum 12 8
export AddScore
proc AddScore 8 0
line 540
;532:
;533:/*
;534:============
;535:AddScore
;536:
;537:Adds score to both the client and his team
;538:============
;539:*/
;540:void AddScore( gentity_t *ent, vec3_t origin, int score ) {
line 541
;541:	if ( !ent->client ) {
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $191
line 542
;542:		return;
ADDRGP4 $190
JUMPV
LABELV $191
line 545
;543:	}
;544:	// no scoring during pre-match warmup
;545:	if ( level.warmupTime ) {
ADDRGP4 level+16
INDIRI4
CNSTI4 0
EQI4 $193
line 546
;546:		return;
ADDRGP4 $190
JUMPV
LABELV $193
line 551
;547:	}
;548:	// show score plum
;549:	//ScorePlum(ent, origin, score);
;550:	//
;551:	ent->client->ps.persistant[PERS_SCORE] += score;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 280
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
line 552
;552:	if ( g_gametype.integer == GT_TEAM )
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
NEI4 $196
line 553
;553:		level.teamScores[ ent->client->ps.persistant[PERS_TEAM] ] += score;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 292
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 level+44
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
LABELV $196
line 554
;554:	CalculateRanks();
ADDRGP4 CalculateRanks
CALLV
pop
line 555
;555:}
LABELV $190
endproc AddScore 8 0
export TossClientWeapon
proc TossClientWeapon 92 12
line 565
;556:
;557:/*
;558:=================
;559:TossClientItems
;560:
;561:rww - Toss the weapon away from the player in the specified direction
;562:=================
;563:*/
;564:void TossClientWeapon(gentity_t *self, vec3_t direction, float speed)
;565:{
line 569
;566:	vec3_t vel;
;567:	gitem_t *item;
;568:	gentity_t *launched;
;569:	int weapon = self->s.weapon;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
ASGNI4
line 572
;570:	int ammoSub;
;571:
;572:	if (weapon <= WP_BRYAR_PISTOL)
ADDRLP4 0
INDIRI4
CNSTI4 3
GTI4 $201
line 573
;573:	{ //can't have this
line 574
;574:		return;
ADDRGP4 $200
JUMPV
LABELV $201
line 577
;575:	}
;576:
;577:	if (weapon == WP_EMPLACED_GUN ||
ADDRLP4 0
INDIRI4
CNSTI4 14
EQI4 $205
ADDRLP4 0
INDIRI4
CNSTI4 15
NEI4 $203
LABELV $205
line 579
;578:		weapon == WP_TURRET)
;579:	{
line 580
;580:		return;
ADDRGP4 $200
JUMPV
LABELV $203
line 584
;581:	}
;582:
;583:	// find the item type for this weapon
;584:	item = BG_FindItemForWeapon( weapon );
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 32
ADDRGP4 BG_FindItemForWeapon
CALLP4
ASGNP4
ADDRLP4 20
ADDRLP4 32
INDIRP4
ASGNP4
line 586
;585:
;586:	ammoSub = (self->client->ps.ammo[weaponData[weapon].ammoIndex] - bg_itemlist[BG_GetItemIndexByTag(weapon, IT_WEAPON)].quantity);
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 40
ADDRGP4 BG_GetItemIndexByTag
CALLI4
ASGNI4
ADDRLP4 44
CNSTI4 408
ASGNI4
ADDRLP4 24
CNSTI4 56
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 weaponData
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
ADDP4
INDIRI4
CNSTI4 52
ADDRLP4 40
INDIRI4
MULI4
ADDRGP4 bg_itemlist+32
ADDP4
INDIRI4
SUBI4
ASGNI4
line 588
;587:
;588:	if (ammoSub < 0)
ADDRLP4 24
INDIRI4
CNSTI4 0
GEI4 $207
line 589
;589:	{
line 590
;590:		int ammoQuan = item->quantity;
ADDRLP4 48
ADDRLP4 20
INDIRP4
CNSTI4 32
ADDP4
INDIRI4
ASGNI4
line 591
;591:		ammoQuan -= (-ammoSub);
ADDRLP4 48
ADDRLP4 48
INDIRI4
ADDRLP4 24
INDIRI4
NEGI4
SUBI4
ASGNI4
line 593
;592:
;593:		if (ammoQuan <= 0)
ADDRLP4 48
INDIRI4
CNSTI4 0
GTI4 $209
line 594
;594:		{ //no ammo
line 595
;595:			return;
ADDRGP4 $200
JUMPV
LABELV $209
line 597
;596:		}
;597:	}
LABELV $207
line 599
;598:
;599:	vel[0] = direction[0]*speed;
ADDRLP4 8
ADDRFP4 4
INDIRP4
INDIRF4
ADDRFP4 8
INDIRF4
MULF4
ASGNF4
line 600
;600:	vel[1] = direction[1]*speed;
ADDRLP4 8+4
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRFP4 8
INDIRF4
MULF4
ASGNF4
line 601
;601:	vel[2] = direction[2]*speed;
ADDRLP4 8+8
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRFP4 8
INDIRF4
MULF4
ASGNF4
line 603
;602:
;603:	launched = LaunchItem(item, self->client->ps.origin, vel);
ADDRLP4 20
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 48
ADDRGP4 LaunchItem
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 48
INDIRP4
ASGNP4
line 605
;604:
;605:	launched->s.generic1 = self->s.number;
ADDRLP4 4
INDIRP4
CNSTI4 292
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 606
;606:	launched->s.powerups = level.time + 1500;
ADDRLP4 4
INDIRP4
CNSTI4 272
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1500
ADDI4
ASGNI4
line 608
;607:
;608:	launched->count = bg_itemlist[BG_GetItemIndexByTag(weapon, IT_WEAPON)].quantity;
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 52
ADDRGP4 BG_GetItemIndexByTag
CALLI4
ASGNI4
ADDRLP4 4
INDIRP4
CNSTI4 712
ADDP4
CNSTI4 52
ADDRLP4 52
INDIRI4
MULI4
ADDRGP4 bg_itemlist+32
ADDP4
INDIRI4
ASGNI4
line 610
;609:
;610:	self->client->ps.ammo[weaponData[weapon].ammoIndex] -= bg_itemlist[BG_GetItemIndexByTag(weapon, IT_WEAPON)].quantity;
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 60
ADDRGP4 BG_GetItemIndexByTag
CALLI4
ASGNI4
ADDRLP4 64
CNSTI4 408
ASGNI4
ADDRLP4 68
CNSTI4 56
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 weaponData
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
ADDRLP4 64
INDIRI4
ADDP4
INDIRP4
ADDRLP4 64
INDIRI4
ADDP4
ADDP4
ASGNP4
ADDRLP4 68
INDIRP4
ADDRLP4 68
INDIRP4
INDIRI4
CNSTI4 52
ADDRLP4 60
INDIRI4
MULI4
ADDRGP4 bg_itemlist+32
ADDP4
INDIRI4
SUBI4
ASGNI4
line 612
;611:
;612:	if (self->client->ps.ammo[weaponData[weapon].ammoIndex] < 0)
ADDRLP4 72
CNSTI4 408
ASGNI4
CNSTI4 56
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 weaponData
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
ADDRLP4 72
INDIRI4
ADDP4
INDIRP4
ADDRLP4 72
INDIRI4
ADDP4
ADDP4
INDIRI4
CNSTI4 0
GEI4 $216
line 613
;613:	{
line 614
;614:		launched->count -= (-self->client->ps.ammo[weaponData[weapon].ammoIndex]);
ADDRLP4 76
ADDRLP4 4
INDIRP4
CNSTI4 712
ADDP4
ASGNP4
ADDRLP4 80
CNSTI4 408
ASGNI4
ADDRLP4 76
INDIRP4
ADDRLP4 76
INDIRP4
INDIRI4
CNSTI4 56
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 weaponData
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
ADDP4
INDIRI4
NEGI4
SUBI4
ASGNI4
line 615
;615:		self->client->ps.ammo[weaponData[weapon].ammoIndex] = 0;
ADDRLP4 84
CNSTI4 408
ASGNI4
CNSTI4 56
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 weaponData
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
ADDRLP4 84
INDIRI4
ADDP4
INDIRP4
ADDRLP4 84
INDIRI4
ADDP4
ADDP4
CNSTI4 0
ASGNI4
line 616
;616:	}
LABELV $216
line 618
;617:
;618:	if ((self->client->ps.ammo[weaponData[weapon].ammoIndex] < 1 && weapon != WP_DET_PACK) ||
ADDRLP4 80
CNSTI4 408
ASGNI4
CNSTI4 56
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 weaponData
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
ADDP4
INDIRI4
CNSTI4 1
GEI4 $221
ADDRLP4 0
INDIRI4
CNSTI4 13
NEI4 $220
LABELV $221
ADDRLP4 0
INDIRI4
CNSTI4 11
EQI4 $218
ADDRLP4 0
INDIRI4
CNSTI4 13
EQI4 $218
ADDRLP4 0
INDIRI4
CNSTI4 12
EQI4 $218
LABELV $220
line 620
;619:		(weapon != WP_THERMAL && weapon != WP_DET_PACK && weapon != WP_TRIP_MINE))
;620:	{
line 621
;621:		self->client->ps.stats[STAT_WEAPONS] &= ~(1 << weapon);
ADDRLP4 88
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 232
ADDP4
ASGNP4
ADDRLP4 88
INDIRP4
ADDRLP4 88
INDIRP4
INDIRI4
CNSTI4 1
ADDRLP4 0
INDIRI4
LSHI4
BCOMI4
BANDI4
ASGNI4
line 623
;622:
;623:		self->s.weapon = 0;
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 0
ASGNI4
line 624
;624:		self->client->ps.weapon = 0;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 148
ADDP4
CNSTI4 0
ASGNI4
line 626
;625:
;626:		G_AddEvent(self, EV_NOAMMO, weapon);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 22
ARGI4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 627
;627:	}
LABELV $218
line 628
;628:}
LABELV $200
endproc TossClientWeapon 92 12
export TossClientItems
proc TossClientItems 44 12
line 637
;629:
;630:/*
;631:=================
;632:TossClientItems
;633:
;634:Toss the weapon and powerups for the killed player
;635:=================
;636:*/
;637:void TossClientItems( gentity_t *self ) {
line 645
;638:	gitem_t		*item;
;639:	int			weapon;
;640:	float		angle;
;641:	int			i;
;642:	gentity_t	*drop;
;643:
;644:	// drop the weapon if not a gauntlet or machinegun
;645:	weapon = self->s.weapon;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
ASGNI4
line 651
;646:
;647:	// make a special check to see if they are changing to a new
;648:	// weapon that isn't the mg or gauntlet.  Without this, a client
;649:	// can pick up a weapon, be killed, and not drop the weapon because
;650:	// their weapon change hasn't completed yet and they are still holding the MG.
;651:	if ( weapon == WP_BRYAR_PISTOL) {
ADDRLP4 12
INDIRI4
CNSTI4 3
NEI4 $223
line 652
;652:		if ( self->client->ps.weaponstate == WEAPON_DROPPING ) {
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 152
ADDP4
INDIRI4
CNSTI4 2
NEI4 $225
line 653
;653:			weapon = self->client->pers.cmd.weapon;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1392
ADDP4
INDIRU1
CVUI4 1
ASGNI4
line 654
;654:		}
LABELV $225
line 655
;655:		if ( !( self->client->ps.stats[STAT_WEAPONS] & ( 1 << weapon ) ) ) {
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 232
ADDP4
INDIRI4
CNSTI4 1
ADDRLP4 12
INDIRI4
LSHI4
BANDI4
CNSTI4 0
NEI4 $227
line 656
;656:			weapon = WP_NONE;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 657
;657:		}
LABELV $227
line 658
;658:	}
LABELV $223
line 660
;659:
;660:	self->s.bolt2 = weapon;
ADDRFP4 0
INDIRP4
CNSTI4 144
ADDP4
ADDRLP4 12
INDIRI4
ASGNI4
line 662
;661:
;662:	if ( weapon > WP_BRYAR_PISTOL && 
ADDRLP4 12
INDIRI4
CNSTI4 3
LEI4 $229
ADDRLP4 12
INDIRI4
CNSTI4 14
EQI4 $229
ADDRLP4 12
INDIRI4
CNSTI4 15
EQI4 $229
ADDRLP4 24
CNSTI4 408
ASGNI4
CNSTI4 56
ADDRLP4 12
INDIRI4
MULI4
ADDRGP4 weaponData
ADDP4
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
ADDP4
INDIRI4
CNSTI4 0
EQI4 $229
line 665
;663:		weapon != WP_EMPLACED_GUN &&
;664:		weapon != WP_TURRET &&
;665:		self->client->ps.ammo[ weaponData[weapon].ammoIndex ] ) {
line 669
;666:		gentity_t *te;
;667:
;668:		// find the item type for this weapon
;669:		item = BG_FindItemForWeapon( weapon );
ADDRLP4 12
INDIRI4
ARGI4
ADDRLP4 32
ADDRGP4 BG_FindItemForWeapon
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 32
INDIRP4
ASGNP4
line 672
;670:
;671:		// tell all clients to remove the weapon model on this guy until he respawns
;672:		te = G_TempEntity( vec3_origin, EV_DESTROY_WEAPON_MODEL );
ADDRGP4 vec3_origin
ARGP4
CNSTI4 91
ARGI4
ADDRLP4 36
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 28
ADDRLP4 36
INDIRP4
ASGNP4
line 673
;673:		te->r.svFlags |= SVF_BROADCAST;
ADDRLP4 40
ADDRLP4 28
INDIRP4
CNSTI4 304
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
line 674
;674:		te->s.eventParm = self->s.number;
ADDRLP4 28
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 677
;675:
;676:		// spawn the item
;677:		Drop_Item( self, item, 0 );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
CNSTF4 0
ARGF4
ADDRGP4 Drop_Item
CALLP4
pop
line 678
;678:	}
LABELV $229
line 681
;679:
;680:	// drop all the powerups if not in teamplay
;681:	if ( g_gametype.integer != GT_TEAM ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
EQI4 $231
line 682
;682:		angle = 45;
ADDRLP4 16
CNSTF4 1110704128
ASGNF4
line 683
;683:		for ( i = 1 ; i < PW_NUM_POWERUPS ; i++ ) {
ADDRLP4 0
CNSTI4 1
ASGNI4
LABELV $234
line 684
;684:			if ( self->client->ps.powerups[ i ] > level.time ) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 344
ADDP4
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
LEI4 $238
line 685
;685:				item = BG_FindItemForPowerup( i );
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 28
ADDRGP4 BG_FindItemForPowerup
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 28
INDIRP4
ASGNP4
line 686
;686:				if ( !item ) {
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $241
line 687
;687:					continue;
ADDRGP4 $235
JUMPV
LABELV $241
line 689
;688:				}
;689:				drop = Drop_Item( self, item, angle );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 16
INDIRF4
ARGF4
ADDRLP4 32
ADDRGP4 Drop_Item
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 32
INDIRP4
ASGNP4
line 691
;690:				// decide how many seconds it has left
;691:				drop->count = ( self->client->ps.powerups[ i ] - level.time ) / 1000;
ADDRLP4 4
INDIRP4
CNSTI4 712
ADDP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 344
ADDP4
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
SUBI4
CNSTI4 1000
DIVI4
ASGNI4
line 692
;692:				if ( drop->count < 1 ) {
ADDRLP4 4
INDIRP4
CNSTI4 712
ADDP4
INDIRI4
CNSTI4 1
GEI4 $244
line 693
;693:					drop->count = 1;
ADDRLP4 4
INDIRP4
CNSTI4 712
ADDP4
CNSTI4 1
ASGNI4
line 694
;694:				}
LABELV $244
line 695
;695:				angle += 45;
ADDRLP4 16
ADDRLP4 16
INDIRF4
CNSTF4 1110704128
ADDF4
ASGNF4
line 696
;696:			}
LABELV $238
line 697
;697:		}
LABELV $235
line 683
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $234
line 698
;698:	}
LABELV $231
line 699
;699:}
LABELV $222
endproc TossClientItems 44 12
export LookAtKiller
proc LookAtKiller 52 4
line 707
;700:
;701:
;702:/*
;703:==================
;704:LookAtKiller
;705:==================
;706:*/
;707:void LookAtKiller( gentity_t *self, gentity_t *inflictor, gentity_t *attacker ) {
line 711
;708:	vec3_t		dir;
;709:	vec3_t		angles;
;710:
;711:	if ( attacker && attacker != self ) {
ADDRLP4 24
ADDRFP4 8
INDIRP4
CVPU4 4
ASGNU4
ADDRLP4 24
INDIRU4
CNSTU4 0
EQU4 $247
ADDRLP4 24
INDIRU4
ADDRFP4 0
INDIRP4
CVPU4 4
EQU4 $247
line 712
;712:		VectorSubtract (attacker->s.pos.trBase, self->s.pos.trBase, dir);
ADDRLP4 28
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 32
CNSTI4 24
ASGNI4
ADDRLP4 36
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 28
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
ADDRLP4 36
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 40
CNSTI4 28
ASGNI4
ADDRLP4 0+4
ADDRLP4 28
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
INDIRF4
ADDRLP4 36
INDIRP4
ADDRLP4 40
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 44
CNSTI4 32
ASGNI4
ADDRLP4 0+8
ADDRFP4 8
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 713
;713:	} else if ( inflictor && inflictor != self ) {
ADDRGP4 $248
JUMPV
LABELV $247
ADDRLP4 28
ADDRFP4 4
INDIRP4
CVPU4 4
ASGNU4
ADDRLP4 28
INDIRU4
CNSTU4 0
EQU4 $251
ADDRLP4 28
INDIRU4
ADDRFP4 0
INDIRP4
CVPU4 4
EQU4 $251
line 714
;714:		VectorSubtract (inflictor->s.pos.trBase, self->s.pos.trBase, dir);
ADDRLP4 32
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 36
CNSTI4 24
ASGNI4
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 32
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 44
CNSTI4 28
ASGNI4
ADDRLP4 0+4
ADDRLP4 32
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
ADDRLP4 44
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 48
CNSTI4 32
ASGNI4
ADDRLP4 0+8
ADDRFP4 4
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 715
;715:	} else {
ADDRGP4 $252
JUMPV
LABELV $251
line 716
;716:		self->client->ps.stats[STAT_DEAD_YAW] = self->s.angles[YAW];
ADDRLP4 32
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 240
ADDP4
ADDRLP4 32
INDIRP4
CNSTI4 120
ADDP4
INDIRF4
CVFI4 4
ASGNI4
line 717
;717:		return;
ADDRGP4 $246
JUMPV
LABELV $252
LABELV $248
line 720
;718:	}
;719:
;720:	self->client->ps.stats[STAT_DEAD_YAW] = vectoyaw ( dir );
ADDRLP4 0
ARGP4
ADDRLP4 32
ADDRGP4 vectoyaw
CALLF4
ASGNF4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 240
ADDP4
ADDRLP4 32
INDIRF4
CVFI4 4
ASGNI4
line 722
;721:
;722:	angles[YAW] = vectoyaw ( dir );
ADDRLP4 0
ARGP4
ADDRLP4 36
ADDRGP4 vectoyaw
CALLF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 36
INDIRF4
ASGNF4
line 723
;723:	angles[PITCH] = 0; 
ADDRLP4 12
CNSTF4 0
ASGNF4
line 724
;724:	angles[ROLL] = 0;
ADDRLP4 12+8
CNSTF4 0
ASGNF4
line 725
;725:}
LABELV $246
endproc LookAtKiller 52 4
export GibEntity
proc GibEntity 0 12
line 732
;726:
;727:/*
;728:==================
;729:GibEntity
;730:==================
;731:*/
;732:void GibEntity( gentity_t *self, int killer ) {
line 733
;733:	G_AddEvent( self, EV_GIB_PLAYER, killer );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 85
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 734
;734:	self->takedamage = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 0
ASGNI4
line 735
;735:	self->s.eType = ET_INVISIBLE;
ADDRFP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 12
ASGNI4
line 736
;736:	self->r.contents = 0;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 0
ASGNI4
line 737
;737:}
LABELV $257
endproc GibEntity 0 12
export BodyRid
proc BodyRid 0 4
line 740
;738:
;739:void BodyRid(gentity_t *ent)
;740:{
line 741
;741:	trap_UnlinkEntity( ent );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 742
;742:	ent->physicsObject = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 500
ADDP4
CNSTI4 0
ASGNI4
line 743
;743:}
LABELV $258
endproc BodyRid 0 4
export body_die
proc body_die 16 0
line 750
;744:
;745:/*
;746:==================
;747:body_die
;748:==================
;749:*/
;750:void body_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath ) {
line 752
;751:	// NOTENOTE No gibbing right now, this is star wars.
;752:	qboolean doDisint = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 754
;753:
;754:	if (self->health < (GIB_HEALTH+1))
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 -39
GEI4 $260
line 755
;755:	{
line 756
;756:		self->health = GIB_HEALTH+1;
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 -39
ASGNI4
line 758
;757:
;758:		if (self->client && (level.time - self->client->respawnTime) < 2000)
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $262
ADDRGP4 level+32
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 1672
ADDP4
INDIRI4
SUBI4
CNSTI4 2000
GEI4 $262
line 759
;759:		{
line 760
;760:			doDisint = qfalse;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 761
;761:		}
ADDRGP4 $263
JUMPV
LABELV $262
line 763
;762:		else
;763:		{
line 764
;764:			doDisint = qtrue;
ADDRLP4 0
CNSTI4 1
ASGNI4
line 765
;765:		}
LABELV $263
line 766
;766:	}
LABELV $260
line 768
;767:
;768:	if (self->client && (self->client->ps.eFlags & EF_DISINTEGRATION))
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $265
ADDRLP4 4
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
CNSTI4 33554432
BANDI4
CNSTI4 0
EQI4 $265
line 769
;769:	{
line 770
;770:		return;
ADDRGP4 $259
JUMPV
LABELV $265
line 772
;771:	}
;772:	else if (self->s.eFlags & EF_DISINTEGRATION)
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 33554432
BANDI4
CNSTI4 0
EQI4 $267
line 773
;773:	{
line 774
;774:		return;
ADDRGP4 $259
JUMPV
LABELV $267
line 777
;775:	}
;776:
;777:	if (doDisint)
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $269
line 778
;778:	{
line 779
;779:		if (self->client)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $271
line 780
;780:		{
line 781
;781:			self->client->ps.eFlags |= EF_DISINTEGRATION;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 33554432
BORI4
ASGNI4
line 782
;782:			VectorCopy(self->client->ps.origin, self->client->ps.lastHitLoc);
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 1356
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 783
;783:		}
ADDRGP4 $259
JUMPV
LABELV $271
line 785
;784:		else
;785:		{
line 786
;786:			self->s.eFlags |= EF_DISINTEGRATION;
ADDRLP4 8
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 33554432
BORI4
ASGNI4
line 787
;787:			VectorCopy(self->r.currentOrigin, self->s.origin2);
ADDRLP4 12
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CNSTI4 104
ADDP4
ADDRLP4 12
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 790
;788:
;789:			//since it's the corpse entity, tell it to "remove" itself
;790:			self->think = BodyRid;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 BodyRid
ASGNP4
line 791
;791:			self->nextthink = level.time + 1000;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1000
ADDI4
ASGNI4
line 792
;792:		}
line 793
;793:		return;
LABELV $269
line 795
;794:	}
;795:}
LABELV $259
endproc body_die 16 0
data
export modNames
align 4
LABELV modNames
address $274
address $275
address $276
address $277
address $278
address $279
address $280
address $281
address $282
address $283
address $284
address $285
address $286
address $287
address $288
address $289
address $290
address $291
address $292
address $293
address $294
address $295
address $296
address $297
address $298
address $299
address $300
address $301
address $302
address $303
address $304
address $305
address $306
address $307
address $308
address $309
address $310
address $311
export CheckAlmostCapture
code
proc CheckAlmostCapture 0 0
line 846
;796:
;797:
;798:// these are just for logging, the client prints its own messages
;799:char	*modNames[] = {
;800:	"MOD_UNKNOWN",
;801:	"MOD_STUN_BATON",
;802:	"MOD_MELEE",
;803:	"MOD_SABER",
;804:	"MOD_BRYAR_PISTOL",
;805:	"MOD_BRYAR_PISTOL_ALT",
;806:	"MOD_BLASTER",
;807:	"MOD_DISRUPTOR",
;808:	"MOD_DISRUPTOR_SPLASH",
;809:	"MOD_DISRUPTOR_SNIPER",
;810:	"MOD_BOWCASTER",
;811:	"MOD_REPEATER",
;812:	"MOD_REPEATER_ALT",
;813:	"MOD_REPEATER_ALT_SPLASH",
;814:	"MOD_DEMP2",
;815:	"MOD_DEMP2_ALT",
;816:	"MOD_FLECHETTE",
;817:	"MOD_FLECHETTE_ALT_SPLASH",
;818:	"MOD_ROCKET",
;819:	"MOD_ROCKET_SPLASH",
;820:	"MOD_ROCKET_HOMING",
;821:	"MOD_ROCKET_HOMING_SPLASH",
;822:	"MOD_THERMAL",
;823:	"MOD_THERMAL_SPLASH",
;824:	"MOD_TRIP_MINE_SPLASH",
;825:	"MOD_TIMED_MINE_SPLASH",
;826:	"MOD_DET_PACK_SPLASH",
;827:	"MOD_FORCE_DARK",
;828:	"MOD_SENTRY",
;829:	"MOD_WATER",
;830:	"MOD_SLIME",
;831:	"MOD_LAVA",
;832:	"MOD_CRUSH",
;833:	"MOD_TELEFRAG",
;834:	"MOD_FALLING",
;835:	"MOD_SUICIDE",
;836:	"MOD_TARGET_LASER",
;837:	"MOD_TRIGGER_HURT"
;838:};
;839:
;840:
;841:/*
;842:==================
;843:CheckAlmostCapture
;844:==================
;845:*/
;846:void CheckAlmostCapture( gentity_t *self, gentity_t *attacker ) {
line 891
;847:#if 0
;848:	gentity_t	*ent;
;849:	vec3_t		dir;
;850:	char		*classname;
;851:
;852:	// if this player was carrying a flag
;853:	if ( self->client->ps.powerups[PW_REDFLAG] ||
;854:		self->client->ps.powerups[PW_BLUEFLAG] ||
;855:		self->client->ps.powerups[PW_NEUTRALFLAG] ) {
;856:		// get the goal flag this player should have been going for
;857:		if ( g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY ) {
;858:			if ( self->client->sess.sessionTeam == TEAM_BLUE ) {
;859:				classname = "team_CTF_blueflag";
;860:			}
;861:			else {
;862:				classname = "team_CTF_redflag";
;863:			}
;864:		}
;865:		else {
;866:			if ( self->client->sess.sessionTeam == TEAM_BLUE ) {
;867:				classname = "team_CTF_redflag";
;868:			}
;869:			else {
;870:				classname = "team_CTF_blueflag";
;871:			}
;872:		}
;873:		ent = NULL;
;874:		do
;875:		{
;876:			ent = G_Find(ent, FOFS(classname), classname);
;877:		} while (ent && (ent->flags & FL_DROPPED_ITEM));
;878:		// if we found the destination flag and it's not picked up
;879:		if (ent && !(ent->r.svFlags & SVF_NOCLIENT) ) {
;880:			// if the player was *very* close
;881:			VectorSubtract( self->client->ps.origin, ent->s.origin, dir );
;882:			if ( VectorLength(dir) < 200 ) {
;883:				self->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_HOLYSHIT;
;884:				if ( attacker->client ) {
;885:					attacker->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_HOLYSHIT;
;886:				}
;887:			}
;888:		}
;889:	}
;890:#endif
;891:}
LABELV $312
endproc CheckAlmostCapture 0 0
proc G_PickDeathAnim 76 8
line 894
;892:
;893:static int G_PickDeathAnim( gentity_t *self, vec3_t point, int damage, int mod, int hitLoc )
;894:{//FIXME: play dead flop anims on body if in an appropriate _DEAD anim when this func is called
line 895
;895:	int deathAnim = -1;
ADDRLP4 0
CNSTI4 -1
ASGNI4
line 898
;896:	int max_health;
;897:
;898:	if (!self || !self->client)
ADDRLP4 8
ADDRFP4 0
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
EQU4 $316
ADDRLP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 12
INDIRU4
NEU4 $314
LABELV $316
line 899
;899:	{
line 900
;900:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $313
JUMPV
LABELV $314
line 903
;901:	}
;902:
;903:	max_health = self->client->ps.stats[STAT_MAX_HEALTH];
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
ASGNI4
line 905
;904:
;905:	if ( hitLoc == HL_NONE )
ADDRFP4 16
INDIRI4
CNSTI4 0
NEI4 $317
line 906
;906:	{
line 907
;907:		hitLoc = G_GetHitLocation( self, point );//self->hitLoc
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 G_GetHitLocation
CALLI4
ASGNI4
ADDRFP4 16
ADDRLP4 16
INDIRI4
ASGNI4
line 908
;908:	}
LABELV $317
line 910
;909:	//dead flops
;910:	switch( self->client->ps.legsAnim&~ANIM_TOGGLEBIT )
ADDRLP4 16
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
ADDRLP4 16
INDIRI4
CNSTI4 0
LTI4 $319
ADDRLP4 16
INDIRI4
CNSTI4 68
GTI4 $319
ADDRLP4 16
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $326
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $326
address $322
address $322
address $325
address $325
address $325
address $325
address $325
address $322
address $325
address $323
address $325
address $325
address $322
address $322
address $319
address $319
address $323
address $322
address $325
address $325
address $325
address $325
address $325
address $319
address $325
address $325
address $325
address $325
address $325
address $325
address $325
address $325
address $319
address $319
address $319
address $319
address $319
address $319
address $319
address $319
address $319
address $319
address $322
address $322
address $325
address $325
address $325
address $325
address $325
address $322
address $325
address $323
address $325
address $325
address $322
address $322
address $323
address $322
address $325
address $325
address $325
address $323
address $323
address $322
address $322
address $325
address $325
address $325
address $324
code
line 911
;911:	{
LABELV $322
line 926
;912:	case BOTH_DEATH1:		//# First Death anim
;913:	case BOTH_DEAD1:
;914:	case BOTH_DEATH2:			//# Second Death anim
;915:	case BOTH_DEAD2:
;916:	case BOTH_DEATH8:			//# 
;917:	case BOTH_DEAD8:
;918:	case BOTH_DEATH13:			//# 
;919:	case BOTH_DEAD13:
;920:	case BOTH_DEATH14:			//# 
;921:	case BOTH_DEAD14:
;922:	case BOTH_DEATH16:			//# 
;923:	case BOTH_DEAD16:
;924:	case BOTH_DEADBACKWARD1:		//# First thrown backward death finished pose
;925:	case BOTH_DEADBACKWARD2:		//# Second thrown backward death finished pose
;926:		deathAnim = -2;
ADDRLP4 0
CNSTI4 -2
ASGNI4
LABELV $323
line 947
;927:		/*
;928:		if ( PM_FinishedCurrentLegsAnim( self ) )
;929:		{//done with the anim
;930:			deathAnim = BOTH_DEADFLOP2;
;931:		}
;932:		else
;933:		{
;934:			deathAnim = -2;
;935:		}
;936:		break;
;937:	case BOTH_DEADFLOP2:
;938:		deathAnim = BOTH_DEADFLOP2;
;939:		break;
;940:		*/
;941:	case BOTH_DEATH10:			//# 
;942:	case BOTH_DEAD10:
;943:	case BOTH_DEATH15:			//# 
;944:	case BOTH_DEAD15:
;945:	case BOTH_DEADFORWARD1:		//# First thrown forward death finished pose
;946:	case BOTH_DEADFORWARD2:		//# Second thrown forward death finished pose
;947:		deathAnim = -2;
ADDRLP4 0
CNSTI4 -2
ASGNI4
LABELV $324
line 960
;948:		/*
;949:		if ( PM_FinishedCurrentLegsAnim( self ) )
;950:		{//done with the anim
;951:			deathAnim = BOTH_DEADFLOP1;
;952:		}
;953:		else
;954:		{
;955:			deathAnim = -2;
;956:		}
;957:		break;
;958:		*/
;959:	case BOTH_DEADFLOP1:
;960:		deathAnim = -2;
ADDRLP4 0
CNSTI4 -2
ASGNI4
line 962
;961:		//deathAnim = BOTH_DEADFLOP1;
;962:		break;
ADDRGP4 $320
JUMPV
LABELV $325
line 998
;963:	case BOTH_DEAD3:				//# Third Death finished pose
;964:	case BOTH_DEAD4:				//# Fourth Death finished pose
;965:	case BOTH_DEAD5:				//# Fifth Death finished pose
;966:	case BOTH_DEAD6:				//# Sixth Death finished pose
;967:	case BOTH_DEAD7:				//# Seventh Death finished pose
;968:	case BOTH_DEAD9:				//# 
;969:	case BOTH_DEAD11:			//#
;970:	case BOTH_DEAD12:			//# 
;971:	case BOTH_DEAD17:			//# 
;972:	case BOTH_DEAD18:			//# 
;973:	case BOTH_DEAD19:			//# 
;974:	case BOTH_LYINGDEAD1:		//# Killed lying down death finished pose
;975:	case BOTH_STUMBLEDEAD1:		//# Stumble forward death finished pose
;976:	case BOTH_FALLDEAD1LAND:		//# Fall forward and splat death finished pose
;977:	case BOTH_DEATH3:			//# Third Death anim
;978:	case BOTH_DEATH4:			//# Fourth Death anim
;979:	case BOTH_DEATH5:			//# Fifth Death anim
;980:	case BOTH_DEATH6:			//# Sixth Death anim
;981:	case BOTH_DEATH7:			//# Seventh Death anim
;982:	case BOTH_DEATH9:			//# 
;983:	case BOTH_DEATH11:			//#
;984:	case BOTH_DEATH12:			//# 
;985:	case BOTH_DEATH17:			//# 
;986:	case BOTH_DEATH18:			//# 
;987:	case BOTH_DEATH19:			//# 
;988:	case BOTH_DEATHFORWARD1:		//# First Death in which they get thrown forward
;989:	case BOTH_DEATHFORWARD2:		//# Second Death in which they get thrown forward
;990:	case BOTH_DEATHBACKWARD1:	//# First Death in which they get thrown backward
;991:	case BOTH_DEATHBACKWARD2:	//# Second Death in which they get thrown backward
;992:	case BOTH_DEATH1IDLE:		//# Idle while close to death
;993:	case BOTH_LYINGDEATH1:		//# Death to play when killed lying down
;994:	case BOTH_STUMBLEDEATH1:		//# Stumble forward and fall face first death
;995:	case BOTH_FALLDEATH1:		//# Fall forward off a high cliff and splat death - start
;996:	case BOTH_FALLDEATH1INAIR:	//# Fall forward off a high cliff and splat death - loop
;997:	case BOTH_FALLDEATH1LAND:	//# Fall forward off a high cliff and splat death - hit bottom
;998:		deathAnim = -2;
ADDRLP4 0
CNSTI4 -2
ASGNI4
line 999
;999:		break;
LABELV $319
LABELV $320
line 1001
;1000:	}
;1001:	if ( deathAnim == -1 )
ADDRLP4 0
INDIRI4
CNSTI4 -1
NEI4 $327
line 1002
;1002:	{
line 1004
;1003:		//death anims
;1004:		switch( hitLoc )
ADDRLP4 24
ADDRFP4 16
INDIRI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 1
LTI4 $330
ADDRLP4 24
INDIRI4
CNSTI4 16
GTI4 $330
ADDRLP4 24
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $400-4
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $400
address $331
address $331
address $338
address $343
address $385
address $355
address $370
address $348
address $355
address $370
address $385
address $355
address $370
address $355
address $370
address $397
code
line 1005
;1005:		{
LABELV $331
line 1008
;1006:		case HL_FOOT_RT:
;1007:		case HL_FOOT_LT:
;1008:			if ( mod == MOD_SABER && !Q_irand( 0, 2 ) )
ADDRFP4 12
INDIRI4
CNSTI4 3
NEI4 $332
CNSTI4 0
ARGI4
CNSTI4 2
ARGI4
ADDRLP4 28
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
NEI4 $332
line 1009
;1009:			{
line 1010
;1010:				return BOTH_DEATH10;//chest: back flip
CNSTI4 9
RETI4
ADDRGP4 $313
JUMPV
LABELV $332
line 1012
;1011:			}
;1012:			else if ( !Q_irand( 0, 2 ) )
CNSTI4 0
ARGI4
CNSTI4 2
ARGI4
ADDRLP4 32
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
NEI4 $334
line 1013
;1013:			{
line 1014
;1014:				deathAnim = BOTH_DEATH4;//back: forward
ADDRLP4 0
CNSTI4 3
ASGNI4
line 1015
;1015:			}
ADDRGP4 $330
JUMPV
LABELV $334
line 1016
;1016:			else if ( !Q_irand( 0, 1 ) )
CNSTI4 0
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 36
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 36
INDIRI4
CNSTI4 0
NEI4 $336
line 1017
;1017:			{
line 1018
;1018:				deathAnim = BOTH_DEATH5;//same as 4
ADDRLP4 0
CNSTI4 4
ASGNI4
line 1019
;1019:			}
ADDRGP4 $330
JUMPV
LABELV $336
line 1021
;1020:			else
;1021:			{
line 1022
;1022:				deathAnim = BOTH_DEATH15;//back: forward
ADDRLP4 0
CNSTI4 16
ASGNI4
line 1023
;1023:			}
line 1024
;1024:			break;
ADDRGP4 $330
JUMPV
LABELV $338
line 1026
;1025:		case HL_LEG_RT:
;1026:			if ( !Q_irand( 0, 2 ) )
CNSTI4 0
ARGI4
CNSTI4 2
ARGI4
ADDRLP4 40
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 40
INDIRI4
CNSTI4 0
NEI4 $339
line 1027
;1027:			{
line 1028
;1028:				deathAnim = BOTH_DEATH4;//back: forward
ADDRLP4 0
CNSTI4 3
ASGNI4
line 1029
;1029:			}
ADDRGP4 $330
JUMPV
LABELV $339
line 1030
;1030:			else if ( !Q_irand( 0, 1 ) )
CNSTI4 0
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 44
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
NEI4 $341
line 1031
;1031:			{
line 1032
;1032:				deathAnim = BOTH_DEATH5;//same as 4
ADDRLP4 0
CNSTI4 4
ASGNI4
line 1033
;1033:			}
ADDRGP4 $330
JUMPV
LABELV $341
line 1035
;1034:			else
;1035:			{
line 1036
;1036:				deathAnim = BOTH_DEATH15;//back: forward
ADDRLP4 0
CNSTI4 16
ASGNI4
line 1037
;1037:			}
line 1038
;1038:			break;
ADDRGP4 $330
JUMPV
LABELV $343
line 1040
;1039:		case HL_LEG_LT:
;1040:			if ( !Q_irand( 0, 2 ) )
CNSTI4 0
ARGI4
CNSTI4 2
ARGI4
ADDRLP4 48
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 48
INDIRI4
CNSTI4 0
NEI4 $344
line 1041
;1041:			{
line 1042
;1042:				deathAnim = BOTH_DEATH4;//back: forward
ADDRLP4 0
CNSTI4 3
ASGNI4
line 1043
;1043:			}
ADDRGP4 $330
JUMPV
LABELV $344
line 1044
;1044:			else if ( !Q_irand( 0, 1 ) )
CNSTI4 0
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 52
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 52
INDIRI4
CNSTI4 0
NEI4 $346
line 1045
;1045:			{
line 1046
;1046:				deathAnim = BOTH_DEATH5;//same as 4
ADDRLP4 0
CNSTI4 4
ASGNI4
line 1047
;1047:			}
ADDRGP4 $330
JUMPV
LABELV $346
line 1049
;1048:			else
;1049:			{
line 1050
;1050:				deathAnim = BOTH_DEATH15;//back: forward
ADDRLP4 0
CNSTI4 16
ASGNI4
line 1051
;1051:			}
line 1052
;1052:			break;
ADDRGP4 $330
JUMPV
LABELV $348
line 1054
;1053:		case HL_BACK:
;1054:			if ( !VectorLengthSquared( self->client->ps.velocity ) )
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRLP4 56
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 56
INDIRF4
CNSTF4 0
NEF4 $349
line 1055
;1055:			{
line 1056
;1056:				deathAnim = BOTH_DEATH17;//head/back: croak
ADDRLP4 0
CNSTI4 18
ASGNI4
line 1057
;1057:			}
ADDRGP4 $330
JUMPV
LABELV $349
line 1059
;1058:			else
;1059:			{
line 1060
;1060:				if ( !Q_irand( 0, 2 ) )
CNSTI4 0
ARGI4
CNSTI4 2
ARGI4
ADDRLP4 60
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 0
NEI4 $351
line 1061
;1061:				{
line 1062
;1062:					deathAnim = BOTH_DEATH4;//back: forward
ADDRLP4 0
CNSTI4 3
ASGNI4
line 1063
;1063:				}
ADDRGP4 $330
JUMPV
LABELV $351
line 1064
;1064:				else if ( !Q_irand( 0, 1 ) )
CNSTI4 0
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 64
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 64
INDIRI4
CNSTI4 0
NEI4 $353
line 1065
;1065:				{
line 1066
;1066:					deathAnim = BOTH_DEATH5;//same as 4
ADDRLP4 0
CNSTI4 4
ASGNI4
line 1067
;1067:				}
ADDRGP4 $330
JUMPV
LABELV $353
line 1069
;1068:				else
;1069:				{
line 1070
;1070:					deathAnim = BOTH_DEATH15;//back: forward
ADDRLP4 0
CNSTI4 16
ASGNI4
line 1071
;1071:				}
line 1072
;1072:			}
line 1073
;1073:			break;
ADDRGP4 $330
JUMPV
LABELV $355
line 1078
;1074:		case HL_CHEST_RT:
;1075:		case HL_ARM_RT:
;1076:		case HL_HAND_RT:
;1077:		case HL_BACK_RT:
;1078:			if ( damage <= max_health*0.25 )
ADDRFP4 8
INDIRI4
CVIF4 4
CNSTF4 1048576000
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
GTF4 $356
line 1079
;1079:			{
line 1080
;1080:				deathAnim = BOTH_DEATH9;//chest right: snap, fall forward
ADDRLP4 0
CNSTI4 8
ASGNI4
line 1081
;1081:			}
ADDRGP4 $330
JUMPV
LABELV $356
line 1082
;1082:			else if ( damage <= max_health*0.5 )
ADDRFP4 8
INDIRI4
CVIF4 4
CNSTF4 1056964608
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
GTF4 $358
line 1083
;1083:			{
line 1084
;1084:				deathAnim = BOTH_DEATH3;//chest right: back
ADDRLP4 0
CNSTI4 2
ASGNI4
line 1085
;1085:			}
ADDRGP4 $330
JUMPV
LABELV $358
line 1086
;1086:			else if ( damage <= max_health*0.75 )
ADDRFP4 8
INDIRI4
CVIF4 4
CNSTF4 1061158912
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
GTF4 $360
line 1087
;1087:			{
line 1088
;1088:				deathAnim = BOTH_DEATH6;//chest right: spin
ADDRLP4 0
CNSTI4 5
ASGNI4
line 1089
;1089:			}
ADDRGP4 $330
JUMPV
LABELV $360
line 1091
;1090:			else 
;1091:			{
line 1093
;1092:				//TEMP HACK: play spinny deaths less often
;1093:				if ( Q_irand( 0, 1 ) )
CNSTI4 0
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 60
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 0
EQI4 $362
line 1094
;1094:				{
line 1095
;1095:					deathAnim = BOTH_DEATH8;//chest right: spin high
ADDRLP4 0
CNSTI4 7
ASGNI4
line 1096
;1096:				}
ADDRGP4 $330
JUMPV
LABELV $362
line 1098
;1097:				else
;1098:				{
line 1099
;1099:					switch ( Q_irand( 0, 2 ) )
CNSTI4 0
ARGI4
CNSTI4 2
ARGI4
ADDRLP4 68
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 64
ADDRLP4 68
INDIRI4
ASGNI4
ADDRLP4 64
INDIRI4
CNSTI4 0
EQI4 $367
ADDRLP4 64
INDIRI4
CNSTI4 1
EQI4 $368
ADDRLP4 64
INDIRI4
CNSTI4 2
EQI4 $369
ADDRGP4 $364
JUMPV
line 1100
;1100:					{
LABELV $364
LABELV $367
line 1103
;1101:					default:
;1102:					case 0:
;1103:						deathAnim = BOTH_DEATH9;//chest right: snap, fall forward
ADDRLP4 0
CNSTI4 8
ASGNI4
line 1104
;1104:						break;
ADDRGP4 $330
JUMPV
LABELV $368
line 1106
;1105:					case 1:
;1106:						deathAnim = BOTH_DEATH3;//chest right: back
ADDRLP4 0
CNSTI4 2
ASGNI4
line 1107
;1107:						break;
ADDRGP4 $330
JUMPV
LABELV $369
line 1109
;1108:					case 2:
;1109:						deathAnim = BOTH_DEATH6;//chest right: spin
ADDRLP4 0
CNSTI4 5
ASGNI4
line 1110
;1110:						break;
line 1112
;1111:					}
;1112:				}
line 1113
;1113:			}
line 1114
;1114:			break;
ADDRGP4 $330
JUMPV
LABELV $370
line 1119
;1115:		case HL_CHEST_LT:
;1116:		case HL_ARM_LT:
;1117:		case HL_HAND_LT:
;1118:		case HL_BACK_LT:
;1119:			if ( damage <= max_health*0.25 )
ADDRFP4 8
INDIRI4
CVIF4 4
CNSTF4 1048576000
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
GTF4 $371
line 1120
;1120:			{
line 1121
;1121:				deathAnim = BOTH_DEATH11;//chest left: snap, fall forward
ADDRLP4 0
CNSTI4 10
ASGNI4
line 1122
;1122:			}
ADDRGP4 $330
JUMPV
LABELV $371
line 1123
;1123:			else if ( damage <= max_health*0.5 )
ADDRFP4 8
INDIRI4
CVIF4 4
CNSTF4 1056964608
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
GTF4 $373
line 1124
;1124:			{
line 1125
;1125:				deathAnim = BOTH_DEATH7;//chest left: back
ADDRLP4 0
CNSTI4 6
ASGNI4
line 1126
;1126:			}
ADDRGP4 $330
JUMPV
LABELV $373
line 1127
;1127:			else if ( damage <= max_health*0.75 )
ADDRFP4 8
INDIRI4
CVIF4 4
CNSTF4 1061158912
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
GTF4 $375
line 1128
;1128:			{
line 1129
;1129:				deathAnim = BOTH_DEATH12;//chest left: spin
ADDRLP4 0
CNSTI4 11
ASGNI4
line 1130
;1130:			}
ADDRGP4 $330
JUMPV
LABELV $375
line 1132
;1131:			else
;1132:			{
line 1134
;1133:				//TEMP HACK: play spinny deaths less often
;1134:				if ( Q_irand( 0, 1 ) )
CNSTI4 0
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 60
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 0
EQI4 $377
line 1135
;1135:				{
line 1136
;1136:					deathAnim = BOTH_DEATH14;//chest left: spin high
ADDRLP4 0
CNSTI4 13
ASGNI4
line 1137
;1137:				}
ADDRGP4 $330
JUMPV
LABELV $377
line 1139
;1138:				else
;1139:				{
line 1140
;1140:					switch ( Q_irand( 0, 2 ) )
CNSTI4 0
ARGI4
CNSTI4 2
ARGI4
ADDRLP4 68
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 64
ADDRLP4 68
INDIRI4
ASGNI4
ADDRLP4 64
INDIRI4
CNSTI4 0
EQI4 $382
ADDRLP4 64
INDIRI4
CNSTI4 1
EQI4 $383
ADDRLP4 64
INDIRI4
CNSTI4 2
EQI4 $384
ADDRGP4 $379
JUMPV
line 1141
;1141:					{
LABELV $379
LABELV $382
line 1144
;1142:					default:
;1143:					case 0:
;1144:						deathAnim = BOTH_DEATH11;//chest left: snap, fall forward
ADDRLP4 0
CNSTI4 10
ASGNI4
line 1145
;1145:						break;
ADDRGP4 $330
JUMPV
LABELV $383
line 1147
;1146:					case 1:
;1147:						deathAnim = BOTH_DEATH7;//chest left: back
ADDRLP4 0
CNSTI4 6
ASGNI4
line 1148
;1148:						break;
ADDRGP4 $330
JUMPV
LABELV $384
line 1150
;1149:					case 2:
;1150:						deathAnim = BOTH_DEATH12;//chest left: spin
ADDRLP4 0
CNSTI4 11
ASGNI4
line 1151
;1151:						break;
line 1153
;1152:					}
;1153:				}
line 1154
;1154:			}
line 1155
;1155:			break;
ADDRGP4 $330
JUMPV
LABELV $385
line 1158
;1156:		case HL_CHEST:
;1157:		case HL_WAIST:
;1158:			if ( damage <= max_health*0.25 || !VectorLengthSquared( self->client->ps.velocity ) )
ADDRFP4 8
INDIRI4
CVIF4 4
CNSTF4 1048576000
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
LEF4 $388
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 32
ADDP4
ARGP4
ADDRLP4 60
ADDRGP4 VectorLengthSquared
CALLF4
ASGNF4
ADDRLP4 60
INDIRF4
CNSTF4 0
NEF4 $386
LABELV $388
line 1159
;1159:			{
line 1160
;1160:				if ( !Q_irand( 0, 1 ) )
CNSTI4 0
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 64
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 64
INDIRI4
CNSTI4 0
NEI4 $389
line 1161
;1161:				{
line 1162
;1162:					deathAnim = BOTH_DEATH18;//gut: fall right
ADDRLP4 0
CNSTI4 19
ASGNI4
line 1163
;1163:				}
ADDRGP4 $330
JUMPV
LABELV $389
line 1165
;1164:				else
;1165:				{
line 1166
;1166:					deathAnim = BOTH_DEATH19;//gut: fall left
ADDRLP4 0
CNSTI4 20
ASGNI4
line 1167
;1167:				}
line 1168
;1168:			}
ADDRGP4 $330
JUMPV
LABELV $386
line 1169
;1169:			else if ( damage <= max_health*0.5 )
ADDRFP4 8
INDIRI4
CVIF4 4
CNSTF4 1056964608
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
GTF4 $391
line 1170
;1170:			{
line 1171
;1171:				deathAnim = BOTH_DEATH2;//chest: backward short
ADDRLP4 0
CNSTI4 1
ASGNI4
line 1172
;1172:			}
ADDRGP4 $330
JUMPV
LABELV $391
line 1173
;1173:			else if ( damage <= max_health*0.75 )
ADDRFP4 8
INDIRI4
CVIF4 4
CNSTF4 1061158912
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
GTF4 $393
line 1174
;1174:			{
line 1175
;1175:				if ( !Q_irand( 0, 1 ) )
CNSTI4 0
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 64
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 64
INDIRI4
CNSTI4 0
NEI4 $395
line 1176
;1176:				{
line 1177
;1177:					deathAnim = BOTH_DEATH1;//chest: backward med
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1178
;1178:				}
ADDRGP4 $330
JUMPV
LABELV $395
line 1180
;1179:				else
;1180:				{
line 1181
;1181:					deathAnim = BOTH_DEATH16;//same as 1
ADDRLP4 0
CNSTI4 17
ASGNI4
line 1182
;1182:				}
line 1183
;1183:			}
ADDRGP4 $330
JUMPV
LABELV $393
line 1185
;1184:			else
;1185:			{
line 1186
;1186:				deathAnim = BOTH_DEATH10;//chest: back flip
ADDRLP4 0
CNSTI4 9
ASGNI4
line 1187
;1187:			}
line 1188
;1188:			break;
ADDRGP4 $330
JUMPV
LABELV $397
line 1190
;1189:		case HL_HEAD:
;1190:			if ( damage <= max_health*0.5 )
ADDRFP4 8
INDIRI4
CVIF4 4
CNSTF4 1056964608
ADDRLP4 4
INDIRI4
CVIF4 4
MULF4
GTF4 $398
line 1191
;1191:			{
line 1192
;1192:				deathAnim = BOTH_DEATH17;//head/back: croak
ADDRLP4 0
CNSTI4 18
ASGNI4
line 1193
;1193:			}
ADDRGP4 $330
JUMPV
LABELV $398
line 1195
;1194:			else
;1195:			{
line 1196
;1196:				deathAnim = BOTH_DEATH13;//head: stumble, fall back
ADDRLP4 0
CNSTI4 12
ASGNI4
line 1197
;1197:			}
line 1198
;1198:			break;
line 1200
;1199:		default:
;1200:			break;
LABELV $330
line 1202
;1201:		}
;1202:	}
LABELV $327
line 1203
;1203:	return deathAnim;
ADDRLP4 0
INDIRI4
RETI4
LABELV $313
endproc G_PickDeathAnim 76 8
export G_GetJediMaster
proc G_GetJediMaster 24 0
line 1209
;1204:}
;1205:
;1206:void G_CheckForDismemberment(gentity_t *ent, vec3_t point, int damage, int deathAnim);
;1207:
;1208:gentity_t *G_GetJediMaster(void)
;1209:{
line 1210
;1210:	int i = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $404
JUMPV
LABELV $403
line 1214
;1211:	gentity_t *ent;
;1212:
;1213:	while (i < MAX_CLIENTS)
;1214:	{
line 1215
;1215:		ent = &g_entities[i];
ADDRLP4 0
CNSTI4 828
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1217
;1216:
;1217:		if (ent && ent->inuse && ent->client && ent->client->ps.isJediMaster)
ADDRLP4 12
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 12
INDIRU4
EQU4 $406
ADDRLP4 16
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $406
ADDRLP4 20
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 20
INDIRP4
CVPU4 4
ADDRLP4 12
INDIRU4
EQU4 $406
ADDRLP4 20
INDIRP4
CNSTI4 604
ADDP4
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $406
line 1218
;1218:		{
line 1219
;1219:			return ent;
ADDRLP4 0
INDIRP4
RETP4
ADDRGP4 $402
JUMPV
LABELV $406
line 1222
;1220:		}
;1221:
;1222:		i++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1223
;1223:	}
LABELV $404
line 1213
ADDRLP4 4
INDIRI4
CNSTI4 32
LTI4 $403
line 1225
;1224:
;1225:	return NULL;
CNSTP4 0
RETP4
LABELV $402
endproc G_GetJediMaster 24 0
bss
align 4
LABELV $536
skip 4
export player_die
code
proc player_die 152 28
line 1233
;1226:}
;1227:
;1228:/*
;1229:==================
;1230:player_die
;1231:==================
;1232:*/
;1233:void player_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath ) {
line 1240
;1234:	gentity_t	*ent;
;1235:	int			anim;
;1236:	int			contents;
;1237:	int			killer;
;1238:	int			i;
;1239:	char		*killerName, *obit;
;1240:	qboolean	wasJediMaster = qfalse;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 1242
;1241:
;1242:	if ( self->client->ps.pm_type == PM_DEAD ) {
ADDRLP4 32
CNSTI4 4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRI4
ADDRLP4 32
INDIRI4
NEI4 $409
line 1243
;1243:		return;
ADDRGP4 $408
JUMPV
LABELV $409
line 1246
;1244:	}
;1245:
;1246:	if ( level.intermissiontime ) {
ADDRGP4 level+9008
INDIRI4
CNSTI4 0
EQI4 $411
line 1247
;1247:		return;
ADDRGP4 $408
JUMPV
LABELV $411
line 1250
;1248:	}
;1249:
;1250:	if (inflictor && inflictor->activator && !inflictor->client && !attacker->client &&
ADDRLP4 36
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 40
CNSTU4 0
ASGNU4
ADDRLP4 36
INDIRP4
CVPU4 4
ADDRLP4 40
INDIRU4
EQU4 $414
ADDRLP4 44
ADDRLP4 36
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
CVPU4 4
ADDRLP4 40
INDIRU4
EQU4 $414
ADDRLP4 48
CNSTI4 408
ASGNI4
ADDRLP4 36
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 40
INDIRU4
NEU4 $414
ADDRFP4 8
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 40
INDIRU4
NEU4 $414
ADDRLP4 44
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 40
INDIRU4
EQU4 $414
ADDRLP4 44
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $414
ADDRLP4 36
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
CNSTI4 15
NEI4 $414
line 1253
;1251:		inflictor->activator->client && inflictor->activator->inuse &&
;1252:		inflictor->s.weapon == WP_TURRET)
;1253:	{
line 1254
;1254:		attacker = inflictor->activator;
ADDRFP4 8
ADDRFP4 4
INDIRP4
CNSTI4 732
ADDP4
INDIRP4
ASGNP4
line 1255
;1255:	}
LABELV $414
line 1257
;1256:
;1257:	if (self->client && self->client->ps.isJediMaster)
ADDRLP4 52
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $416
ADDRLP4 52
INDIRP4
CNSTI4 604
ADDP4
INDIRI4
CNSTI4 0
EQI4 $416
line 1258
;1258:	{
line 1259
;1259:		wasJediMaster = qtrue;
ADDRLP4 16
CNSTI4 1
ASGNI4
line 1260
;1260:	}
LABELV $416
line 1263
;1261:
;1262:	//if he was charging or anything else, kill the sound
;1263:	G_MuteSound(self->s.number, CHAN_WEAPON);
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
CNSTI4 2
ARGI4
ADDRGP4 G_MuteSound
CALLV
pop
line 1265
;1264:
;1265:	BlowDetpacks(self); //blow detpacks if they're planted
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 BlowDetpacks
CALLV
pop
line 1267
;1266:
;1267:	self->client->ps.fd.forceDeactivateAll = 1;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1184
ADDP4
CNSTI4 1
ASGNI4
line 1269
;1268:
;1269:	if ((self == attacker || !attacker->client) &&
ADDRLP4 56
ADDRFP4 8
INDIRP4
ASGNP4
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRLP4 56
INDIRP4
CVPU4 4
EQU4 $421
ADDRLP4 56
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $418
LABELV $421
ADDRLP4 60
ADDRFP4 16
INDIRI4
ASGNI4
ADDRLP4 60
INDIRI4
CNSTI4 32
EQI4 $424
ADDRLP4 60
INDIRI4
CNSTI4 34
EQI4 $424
ADDRLP4 60
INDIRI4
CNSTI4 37
EQI4 $424
ADDRLP4 60
INDIRI4
CNSTI4 0
NEI4 $418
LABELV $424
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 752
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
LEI4 $418
line 1272
;1270:		(meansOfDeath == MOD_CRUSH || meansOfDeath == MOD_FALLING || meansOfDeath == MOD_TRIGGER_HURT || meansOfDeath == MOD_UNKNOWN) &&
;1271:		self->client->ps.otherKillerTime > level.time)
;1272:	{
line 1273
;1273:		attacker = &g_entities[self->client->ps.otherKiller];
ADDRFP4 8
CNSTI4 828
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 748
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 1274
;1274:	}
LABELV $418
line 1277
;1275:
;1276:	// check for an almost capture
;1277:	CheckAlmostCapture( self, attacker );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 CheckAlmostCapture
CALLV
pop
line 1279
;1278:
;1279:	self->client->ps.pm_type = PM_DEAD;
ADDRLP4 64
CNSTI4 4
ASGNI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ADDRLP4 64
INDIRI4
ADDP4
ADDRLP4 64
INDIRI4
ASGNI4
line 1281
;1280:
;1281:	if ( attacker ) {
ADDRFP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $425
line 1282
;1282:		killer = attacker->s.number;
ADDRLP4 4
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
line 1283
;1283:		if ( attacker->client ) {
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $427
line 1284
;1284:			killerName = attacker->client->pers.netname;
ADDRLP4 20
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1416
ADDP4
ASGNP4
line 1285
;1285:		} else {
ADDRGP4 $426
JUMPV
LABELV $427
line 1286
;1286:			killerName = "<non-client>";
ADDRLP4 20
ADDRGP4 $429
ASGNP4
line 1287
;1287:		}
line 1288
;1288:	} else {
ADDRGP4 $426
JUMPV
LABELV $425
line 1289
;1289:		killer = ENTITYNUM_WORLD;
ADDRLP4 4
CNSTI4 1022
ASGNI4
line 1290
;1290:		killerName = "<world>";
ADDRLP4 20
ADDRGP4 $430
ASGNP4
line 1291
;1291:	}
LABELV $426
line 1293
;1292:
;1293:	if ( killer < 0 || killer >= MAX_CLIENTS ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
LTI4 $433
ADDRLP4 4
INDIRI4
CNSTI4 32
LTI4 $431
LABELV $433
line 1294
;1294:		killer = ENTITYNUM_WORLD;
ADDRLP4 4
CNSTI4 1022
ASGNI4
line 1295
;1295:		killerName = "<world>";
ADDRLP4 20
ADDRGP4 $430
ASGNP4
line 1296
;1296:	}
LABELV $431
line 1298
;1297:
;1298:	if ( meansOfDeath < 0 || meansOfDeath >= sizeof( modNames ) / sizeof( modNames[0] ) ) {
ADDRLP4 72
ADDRFP4 16
INDIRI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 0
LTI4 $436
ADDRLP4 72
INDIRI4
CVIU4 4
CNSTU4 38
LTU4 $434
LABELV $436
line 1299
;1299:		obit = "<bad obituary>";
ADDRLP4 28
ADDRGP4 $437
ASGNP4
line 1300
;1300:	} else {
ADDRGP4 $435
JUMPV
LABELV $434
line 1301
;1301:		obit = modNames[ meansOfDeath ];
ADDRLP4 28
ADDRFP4 16
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 modNames
ADDP4
INDIRP4
ASGNP4
line 1302
;1302:	}
LABELV $435
line 1304
;1303:
;1304:	G_LogPrintf("Kill: %i %i %i: %s killed %s by %s\n", 
ADDRGP4 $438
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 76
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 76
INDIRP4
INDIRI4
ARGI4
ADDRFP4 16
INDIRI4
ARGI4
ADDRLP4 20
INDIRP4
ARGP4
ADDRLP4 76
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1416
ADDP4
ARGP4
ADDRLP4 28
INDIRP4
ARGP4
ADDRGP4 G_LogPrintf
CALLV
pop
line 1308
;1305:		killer, self->s.number, meansOfDeath, killerName, 
;1306:		self->client->pers.netname, obit );
;1307:
;1308:	G_LogWeaponKill(killer, meansOfDeath);
ADDRLP4 4
INDIRI4
ARGI4
ADDRFP4 16
INDIRI4
ARGI4
ADDRGP4 G_LogWeaponKill
CALLV
pop
line 1309
;1309:	G_LogWeaponDeath(self->s.number, self->s.weapon);
ADDRLP4 80
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 80
INDIRP4
INDIRI4
ARGI4
ADDRLP4 80
INDIRP4
CNSTI4 276
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_LogWeaponDeath
CALLV
pop
line 1310
;1310:	if (attacker && attacker->client && attacker->inuse)
ADDRLP4 84
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 88
CNSTU4 0
ASGNU4
ADDRLP4 84
INDIRP4
CVPU4 4
ADDRLP4 88
INDIRU4
EQU4 $439
ADDRLP4 84
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 88
INDIRU4
EQU4 $439
ADDRLP4 84
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $439
line 1311
;1311:	{
line 1312
;1312:		G_LogWeaponFrag(killer, self->s.number);
ADDRLP4 4
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
ADDRGP4 G_LogWeaponFrag
CALLV
pop
line 1313
;1313:	}
LABELV $439
line 1316
;1314:
;1315:	// broadcast the death event to everyone
;1316:	ent = G_TempEntity( self->r.currentOrigin, EV_OBITUARY );
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
CNSTI4 81
ARGI4
ADDRLP4 92
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 92
INDIRP4
ASGNP4
line 1317
;1317:	ent->s.eventParm = meansOfDeath;
ADDRLP4 8
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 16
INDIRI4
ASGNI4
line 1318
;1318:	ent->s.otherEntityNum = self->s.number;
ADDRLP4 8
INDIRP4
CNSTI4 184
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 1319
;1319:	ent->s.otherEntityNum2 = killer;
ADDRLP4 8
INDIRP4
CNSTI4 188
ADDP4
ADDRLP4 4
INDIRI4
ASGNI4
line 1320
;1320:	ent->r.svFlags = SVF_BROADCAST;	// send to everyone
ADDRLP4 8
INDIRP4
CNSTI4 304
ADDP4
CNSTI4 32
ASGNI4
line 1321
;1321:	ent->s.isJediMaster = wasJediMaster;
ADDRLP4 8
INDIRP4
CNSTI4 244
ADDP4
ADDRLP4 16
INDIRI4
ASGNI4
line 1323
;1322:
;1323:	self->enemy = attacker;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRFP4 8
INDIRP4
ASGNP4
line 1325
;1324:
;1325:	self->client->ps.persistant[PERS_KILLED]++;
ADDRLP4 96
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 312
ADDP4
ASGNP4
ADDRLP4 96
INDIRP4
ADDRLP4 96
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1327
;1326:
;1327:	if (self == attacker)
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRFP4 8
INDIRP4
CVPU4 4
NEU4 $441
line 1328
;1328:	{
line 1329
;1329:		self->client->ps.fd.suicides++;
ADDRLP4 100
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1224
ADDP4
ASGNP4
ADDRLP4 100
INDIRP4
ADDRLP4 100
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1330
;1330:	}
LABELV $441
line 1332
;1331:
;1332:	if (attacker && attacker->client) {
ADDRLP4 100
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 104
CNSTU4 0
ASGNU4
ADDRLP4 100
INDIRP4
CVPU4 4
ADDRLP4 104
INDIRU4
EQU4 $443
ADDRLP4 100
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 104
INDIRU4
EQU4 $443
line 1333
;1333:		attacker->client->lastkilled_client = self->s.number;
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1660
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 1335
;1334:
;1335:		if ( attacker == self || OnSameTeam (self, attacker ) ) {
ADDRLP4 108
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 112
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 108
INDIRP4
CVPU4 4
ADDRLP4 112
INDIRP4
CVPU4 4
EQU4 $447
ADDRLP4 112
INDIRP4
ARGP4
ADDRLP4 108
INDIRP4
ARGP4
ADDRLP4 116
ADDRGP4 OnSameTeam
CALLI4
ASGNI4
ADDRLP4 116
INDIRI4
CNSTI4 0
EQI4 $445
LABELV $447
line 1336
;1336:			if (g_gametype.integer == GT_TOURNAMENT)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
NEI4 $448
line 1337
;1337:			{ //in duel, if you kill yourself, the person you are dueling against gets a kill for it
line 1338
;1338:				int otherClNum = -1;
ADDRLP4 120
CNSTI4 -1
ASGNI4
line 1339
;1339:				if (level.sortedClients[0] == self->s.number)
ADDRGP4 level+84
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $451
line 1340
;1340:				{
line 1341
;1341:					otherClNum = level.sortedClients[1];
ADDRLP4 120
ADDRGP4 level+84+4
INDIRI4
ASGNI4
line 1342
;1342:				}
ADDRGP4 $452
JUMPV
LABELV $451
line 1343
;1343:				else if (level.sortedClients[1] == self->s.number)
ADDRGP4 level+84+4
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $456
line 1344
;1344:				{
line 1345
;1345:					otherClNum = level.sortedClients[0];
ADDRLP4 120
ADDRGP4 level+84
INDIRI4
ASGNI4
line 1346
;1346:				}
LABELV $456
LABELV $452
line 1348
;1347:
;1348:				if (otherClNum >= 0 && otherClNum < MAX_CLIENTS &&
ADDRLP4 124
ADDRLP4 120
INDIRI4
ASGNI4
ADDRLP4 128
CNSTI4 0
ASGNI4
ADDRLP4 124
INDIRI4
ADDRLP4 128
INDIRI4
LTI4 $461
ADDRLP4 124
INDIRI4
CNSTI4 32
GEI4 $461
ADDRLP4 132
CNSTI4 828
ADDRLP4 124
INDIRI4
MULI4
ASGNI4
ADDRLP4 132
INDIRI4
ADDRGP4 g_entities+412
ADDP4
INDIRI4
ADDRLP4 128
INDIRI4
EQI4 $461
ADDRLP4 132
INDIRI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $461
ADDRLP4 124
INDIRI4
ADDRFP4 8
INDIRP4
INDIRI4
EQI4 $461
line 1351
;1349:					g_entities[otherClNum].inuse && g_entities[otherClNum].client &&
;1350:					otherClNum != attacker->s.number)
;1351:				{
line 1352
;1352:					AddScore( &g_entities[otherClNum], self->r.currentOrigin, 1 );
CNSTI4 828
ADDRLP4 120
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 1353
;1353:				}
ADDRGP4 $449
JUMPV
LABELV $461
line 1355
;1354:				else
;1355:				{
line 1356
;1356:					AddScore( attacker, self->r.currentOrigin, -1 );
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 1357
;1357:				}
line 1358
;1358:			}
ADDRGP4 $449
JUMPV
LABELV $448
line 1360
;1359:			else
;1360:			{
line 1361
;1361:				AddScore( attacker, self->r.currentOrigin, -1 );
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 1362
;1362:			}
LABELV $449
line 1363
;1363:			if (g_gametype.integer == GT_JEDIMASTER)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 2
NEI4 $444
line 1364
;1364:			{
line 1365
;1365:				if (self->client && self->client->ps.isJediMaster)
ADDRLP4 120
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 120
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $444
ADDRLP4 120
INDIRP4
CNSTI4 604
ADDP4
INDIRI4
CNSTI4 0
EQI4 $444
line 1366
;1366:				{ //killed ourself so return the saber to the original position
line 1369
;1367:				  //(to avoid people jumping off ledges and making the saber
;1368:				  //unreachable for 60 seconds)
;1369:					ThrowSaberToAttacker(self, NULL);
ADDRFP4 0
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 ThrowSaberToAttacker
CALLV
pop
line 1370
;1370:					self->client->ps.isJediMaster = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 604
ADDP4
CNSTI4 0
ASGNI4
line 1371
;1371:				}
line 1372
;1372:			}
line 1373
;1373:		} else {
ADDRGP4 $444
JUMPV
LABELV $445
line 1374
;1374:			if (g_gametype.integer == GT_JEDIMASTER)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 2
NEI4 $470
line 1375
;1375:			{
line 1376
;1376:				if ((attacker->client && attacker->client->ps.isJediMaster) ||
ADDRLP4 120
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 120
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $476
ADDRLP4 120
INDIRP4
CNSTI4 604
ADDP4
INDIRI4
CNSTI4 0
NEI4 $475
LABELV $476
ADDRLP4 124
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 124
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $473
ADDRLP4 124
INDIRP4
CNSTI4 604
ADDP4
INDIRI4
CNSTI4 0
EQI4 $473
LABELV $475
line 1378
;1377:					(self->client && self->client->ps.isJediMaster))
;1378:				{
line 1379
;1379:					AddScore( attacker, self->r.currentOrigin, 1 );
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 1381
;1380:					
;1381:					if (self->client && self->client->ps.isJediMaster)
ADDRLP4 128
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 128
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $471
ADDRLP4 128
INDIRP4
CNSTI4 604
ADDP4
INDIRI4
CNSTI4 0
EQI4 $471
line 1382
;1382:					{
line 1383
;1383:						ThrowSaberToAttacker(self, attacker);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 ThrowSaberToAttacker
CALLV
pop
line 1384
;1384:						self->client->ps.isJediMaster = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 604
ADDP4
CNSTI4 0
ASGNI4
line 1385
;1385:					}
line 1386
;1386:				}
ADDRGP4 $471
JUMPV
LABELV $473
line 1388
;1387:				else
;1388:				{
line 1389
;1389:					gentity_t *jmEnt = G_GetJediMaster();
ADDRLP4 132
ADDRGP4 G_GetJediMaster
CALLP4
ASGNP4
ADDRLP4 128
ADDRLP4 132
INDIRP4
ASGNP4
line 1391
;1390:
;1391:					if (jmEnt && jmEnt->client)
ADDRLP4 136
ADDRLP4 128
INDIRP4
ASGNP4
ADDRLP4 140
CNSTU4 0
ASGNU4
ADDRLP4 136
INDIRP4
CVPU4 4
ADDRLP4 140
INDIRU4
EQU4 $471
ADDRLP4 136
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 140
INDIRU4
EQU4 $471
line 1392
;1392:					{
line 1393
;1393:						AddScore( jmEnt, self->r.currentOrigin, 1 );
ADDRLP4 128
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 1394
;1394:					}
line 1395
;1395:				}
line 1396
;1396:			}
ADDRGP4 $471
JUMPV
LABELV $470
line 1398
;1397:			else
;1398:			{
line 1399
;1399:				AddScore( attacker, self->r.currentOrigin, 1 );
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 1400
;1400:			}
LABELV $471
line 1402
;1401:
;1402:			if( meansOfDeath == MOD_STUN_BATON ) {
ADDRFP4 16
INDIRI4
CNSTI4 1
NEI4 $481
line 1405
;1403:				
;1404:				// play humiliation on player
;1405:				attacker->client->ps.persistant[PERS_GAUNTLET_FRAG_COUNT]++;
ADDRLP4 120
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 332
ADDP4
ASGNP4
ADDRLP4 120
INDIRP4
ADDRLP4 120
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1408
;1406:
;1407:				// add the sprite over the player's head
;1408:				attacker->client->ps.eFlags &= ~(EF_AWARD_IMPRESSIVE | EF_AWARD_EXCELLENT | EF_AWARD_GAUNTLET | EF_AWARD_ASSIST | EF_AWARD_DEFEND | EF_AWARD_CAP );
ADDRLP4 124
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 124
INDIRP4
ADDRLP4 124
INDIRP4
INDIRI4
CNSTI4 -231497
BANDI4
ASGNI4
line 1409
;1409:				attacker->client->ps.eFlags |= EF_AWARD_GAUNTLET;
ADDRLP4 128
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 128
INDIRP4
ADDRLP4 128
INDIRP4
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 1410
;1410:				attacker->client->rewardTime = level.time + REWARD_SPRITE_TIME;
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1684
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 2000
ADDI4
ASGNI4
line 1413
;1411:
;1412:				// also play humiliation on target
;1413:				self->client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_GAUNTLETREWARD;
ADDRLP4 132
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 300
ADDP4
ASGNP4
ADDRLP4 132
INDIRP4
ADDRLP4 132
INDIRP4
INDIRI4
CNSTI4 2
BXORI4
ASGNI4
line 1414
;1414:			}
LABELV $481
line 1418
;1415:
;1416:			// check for two kills in a short amount of time
;1417:			// if this is close enough to the last kill, give a reward sound
;1418:			if ( level.time - attacker->client->lastKillTime < CARNAGE_REWARD_TIME ) {
ADDRGP4 level+32
INDIRI4
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1692
ADDP4
INDIRI4
SUBI4
CNSTI4 3000
GEI4 $484
line 1420
;1419:				// play excellent on player
;1420:				attacker->client->ps.persistant[PERS_EXCELLENT_COUNT]++;
ADDRLP4 120
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 320
ADDP4
ASGNP4
ADDRLP4 120
INDIRP4
ADDRLP4 120
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1423
;1421:
;1422:				// add the sprite over the player's head
;1423:				attacker->client->ps.eFlags &= ~(EF_AWARD_IMPRESSIVE | EF_AWARD_EXCELLENT | EF_AWARD_GAUNTLET | EF_AWARD_ASSIST | EF_AWARD_DEFEND | EF_AWARD_CAP );
ADDRLP4 124
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 124
INDIRP4
ADDRLP4 124
INDIRP4
INDIRI4
CNSTI4 -231497
BANDI4
ASGNI4
line 1424
;1424:				attacker->client->ps.eFlags |= EF_AWARD_EXCELLENT;
ADDRLP4 128
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 128
INDIRP4
ADDRLP4 128
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 1425
;1425:				attacker->client->rewardTime = level.time + REWARD_SPRITE_TIME;
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1684
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 2000
ADDI4
ASGNI4
line 1426
;1426:			}
LABELV $484
line 1427
;1427:			attacker->client->lastKillTime = level.time;
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1692
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1429
;1428:
;1429:		}
line 1430
;1430:	} else {
ADDRGP4 $444
JUMPV
LABELV $443
line 1431
;1431:		if (self->client && self->client->ps.isJediMaster)
ADDRLP4 108
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 108
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $489
ADDRLP4 108
INDIRP4
CNSTI4 604
ADDP4
INDIRI4
CNSTI4 0
EQI4 $489
line 1432
;1432:		{ //killed ourself so return the saber to the original position
line 1435
;1433:		  //(to avoid people jumping off ledges and making the saber
;1434:		  //unreachable for 60 seconds)
;1435:			ThrowSaberToAttacker(self, NULL);
ADDRFP4 0
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRGP4 ThrowSaberToAttacker
CALLV
pop
line 1436
;1436:			self->client->ps.isJediMaster = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 604
ADDP4
CNSTI4 0
ASGNI4
line 1437
;1437:		}
LABELV $489
line 1439
;1438:
;1439:		if (g_gametype.integer == GT_TOURNAMENT)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
NEI4 $491
line 1440
;1440:		{ //in duel, if you kill yourself, the person you are dueling against gets a kill for it
line 1441
;1441:			int otherClNum = -1;
ADDRLP4 112
CNSTI4 -1
ASGNI4
line 1442
;1442:			if (level.sortedClients[0] == self->s.number)
ADDRGP4 level+84
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $494
line 1443
;1443:			{
line 1444
;1444:				otherClNum = level.sortedClients[1];
ADDRLP4 112
ADDRGP4 level+84+4
INDIRI4
ASGNI4
line 1445
;1445:			}
ADDRGP4 $495
JUMPV
LABELV $494
line 1446
;1446:			else if (level.sortedClients[1] == self->s.number)
ADDRGP4 level+84+4
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $499
line 1447
;1447:			{
line 1448
;1448:				otherClNum = level.sortedClients[0];
ADDRLP4 112
ADDRGP4 level+84
INDIRI4
ASGNI4
line 1449
;1449:			}
LABELV $499
LABELV $495
line 1451
;1450:
;1451:			if (otherClNum >= 0 && otherClNum < MAX_CLIENTS &&
ADDRLP4 116
ADDRLP4 112
INDIRI4
ASGNI4
ADDRLP4 120
CNSTI4 0
ASGNI4
ADDRLP4 116
INDIRI4
ADDRLP4 120
INDIRI4
LTI4 $504
ADDRLP4 116
INDIRI4
CNSTI4 32
GEI4 $504
ADDRLP4 124
CNSTI4 828
ADDRLP4 116
INDIRI4
MULI4
ASGNI4
ADDRLP4 124
INDIRI4
ADDRGP4 g_entities+412
ADDP4
INDIRI4
ADDRLP4 120
INDIRI4
EQI4 $504
ADDRLP4 124
INDIRI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $504
ADDRLP4 116
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
EQI4 $504
line 1454
;1452:				g_entities[otherClNum].inuse && g_entities[otherClNum].client &&
;1453:				otherClNum != self->s.number)
;1454:			{
line 1455
;1455:				AddScore( &g_entities[otherClNum], self->r.currentOrigin, 1 );
CNSTI4 828
ADDRLP4 112
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 1456
;1456:			}
ADDRGP4 $492
JUMPV
LABELV $504
line 1458
;1457:			else
;1458:			{
line 1459
;1459:				AddScore( self, self->r.currentOrigin, -1 );
ADDRLP4 128
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 128
INDIRP4
ARGP4
ADDRLP4 128
INDIRP4
CNSTI4 368
ADDP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 1460
;1460:			}
line 1461
;1461:		}
ADDRGP4 $492
JUMPV
LABELV $491
line 1463
;1462:		else
;1463:		{
line 1464
;1464:			AddScore( self, self->r.currentOrigin, -1 );
ADDRLP4 112
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 112
INDIRP4
ARGP4
ADDRLP4 112
INDIRP4
CNSTI4 368
ADDP4
ARGP4
CNSTI4 -1
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 1465
;1465:		}
LABELV $492
line 1466
;1466:	}
LABELV $444
line 1469
;1467:
;1468:	// Add team bonuses
;1469:	Team_FragBonuses(self, inflictor, attacker);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Team_FragBonuses
CALLV
pop
line 1472
;1470:
;1471:	// if I committed suicide, the flag does not fall, it returns.
;1472:	if (meansOfDeath == MOD_SUICIDE) {
ADDRFP4 16
INDIRI4
CNSTI4 35
NEI4 $508
line 1473
;1473:		if ( self->client->ps.powerups[PW_NEUTRALFLAG] ) {		// only happens in One Flag CTF
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 368
ADDP4
INDIRI4
CNSTI4 0
EQI4 $510
line 1474
;1474:			Team_ReturnFlag( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 1475
;1475:			self->client->ps.powerups[PW_NEUTRALFLAG] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 368
ADDP4
CNSTI4 0
ASGNI4
line 1476
;1476:		}
ADDRGP4 $511
JUMPV
LABELV $510
line 1477
;1477:		else if ( self->client->ps.powerups[PW_REDFLAG] ) {		// only happens in standard CTF
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 360
ADDP4
INDIRI4
CNSTI4 0
EQI4 $512
line 1478
;1478:			Team_ReturnFlag( TEAM_RED );
CNSTI4 1
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 1479
;1479:			self->client->ps.powerups[PW_REDFLAG] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 360
ADDP4
CNSTI4 0
ASGNI4
line 1480
;1480:		}
ADDRGP4 $513
JUMPV
LABELV $512
line 1481
;1481:		else if ( self->client->ps.powerups[PW_BLUEFLAG] ) {	// only happens in standard CTF
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 364
ADDP4
INDIRI4
CNSTI4 0
EQI4 $514
line 1482
;1482:			Team_ReturnFlag( TEAM_BLUE );
CNSTI4 2
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 1483
;1483:			self->client->ps.powerups[PW_BLUEFLAG] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 364
ADDP4
CNSTI4 0
ASGNI4
line 1484
;1484:		}
LABELV $514
LABELV $513
LABELV $511
line 1485
;1485:	}
LABELV $508
line 1488
;1486:
;1487:	// if client is in a nodrop area, don't drop anything (but return CTF flags!)
;1488:	contents = trap_PointContents( self->r.currentOrigin, -1 );
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
CNSTI4 -1
ARGI4
ADDRLP4 108
ADDRGP4 trap_PointContents
CALLI4
ASGNI4
ADDRLP4 24
ADDRLP4 108
INDIRI4
ASGNI4
line 1489
;1489:	if ( !( contents & CONTENTS_NODROP ) && !self->client->ps.fallingToDeath) {
ADDRLP4 112
CNSTI4 0
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 2048
BANDI4
ADDRLP4 112
INDIRI4
NEI4 $516
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1340
ADDP4
INDIRI4
ADDRLP4 112
INDIRI4
NEI4 $516
line 1490
;1490:		TossClientItems( self );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 TossClientItems
CALLV
pop
line 1491
;1491:	}
ADDRGP4 $517
JUMPV
LABELV $516
line 1492
;1492:	else {
line 1493
;1493:		if ( self->client->ps.powerups[PW_NEUTRALFLAG] ) {		// only happens in One Flag CTF
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 368
ADDP4
INDIRI4
CNSTI4 0
EQI4 $518
line 1494
;1494:			Team_ReturnFlag( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 1495
;1495:		}
ADDRGP4 $519
JUMPV
LABELV $518
line 1496
;1496:		else if ( self->client->ps.powerups[PW_REDFLAG] ) {		// only happens in standard CTF
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 360
ADDP4
INDIRI4
CNSTI4 0
EQI4 $520
line 1497
;1497:			Team_ReturnFlag( TEAM_RED );
CNSTI4 1
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 1498
;1498:		}
ADDRGP4 $521
JUMPV
LABELV $520
line 1499
;1499:		else if ( self->client->ps.powerups[PW_BLUEFLAG] ) {	// only happens in standard CTF
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 364
ADDP4
INDIRI4
CNSTI4 0
EQI4 $522
line 1500
;1500:			Team_ReturnFlag( TEAM_BLUE );
CNSTI4 2
ARGI4
ADDRGP4 Team_ReturnFlag
CALLV
pop
line 1501
;1501:		}
LABELV $522
LABELV $521
LABELV $519
line 1502
;1502:	}
LABELV $517
line 1504
;1503:
;1504:	Cmd_Score_f( self );		// show scores
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Cmd_Score_f
CALLV
pop
line 1507
;1505:	// send updated scores to any clients that are following this one,
;1506:	// or they would get stale scoreboards
;1507:	for ( i = 0 ; i < level.maxclients ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $527
JUMPV
LABELV $524
line 1510
;1508:		gclient_t	*client;
;1509:
;1510:		client = &level.clients[i];
ADDRLP4 116
CNSTI4 1756
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 1511
;1511:		if ( client->pers.connected != CON_CONNECTED ) {
ADDRLP4 116
INDIRP4
CNSTI4 1368
ADDP4
INDIRI4
CNSTI4 2
EQI4 $529
line 1512
;1512:			continue;
ADDRGP4 $525
JUMPV
LABELV $529
line 1514
;1513:		}
;1514:		if ( client->sess.sessionTeam != TEAM_SPECTATOR ) {
ADDRLP4 116
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
EQI4 $531
line 1515
;1515:			continue;
ADDRGP4 $525
JUMPV
LABELV $531
line 1517
;1516:		}
;1517:		if ( client->sess.spectatorClient == self->s.number ) {
ADDRLP4 116
INDIRP4
CNSTI4 1532
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $533
line 1518
;1518:			Cmd_Score_f( g_entities + i );
CNSTI4 828
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRGP4 Cmd_Score_f
CALLV
pop
line 1519
;1519:		}
LABELV $533
line 1520
;1520:	}
LABELV $525
line 1507
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $527
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $524
line 1522
;1521:
;1522:	self->takedamage = qtrue;	// can still be gibbed
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 1
ASGNI4
line 1524
;1523:
;1524:	self->s.weapon = WP_NONE;
ADDRFP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 0
ASGNI4
line 1525
;1525:	self->s.powerups = 0;
ADDRFP4 0
INDIRP4
CNSTI4 272
ADDP4
CNSTI4 0
ASGNI4
line 1526
;1526:	self->r.contents = CONTENTS_CORPSE;
ADDRFP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 512
ASGNI4
line 1527
;1527:	self->client->ps.zoomMode = 0;	// Turn off zooming when we die
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1320
ADDP4
CNSTI4 0
ASGNI4
line 1529
;1528:
;1529:	self->s.angles[0] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
CNSTF4 0
ASGNF4
line 1530
;1530:	self->s.angles[2] = 0;
ADDRFP4 0
INDIRP4
CNSTI4 124
ADDP4
CNSTF4 0
ASGNF4
line 1531
;1531:	LookAtKiller (self, inflictor, attacker);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 LookAtKiller
CALLV
pop
line 1533
;1532:
;1533:	VectorCopy( self->s.angles, self->client->ps.viewangles );
ADDRLP4 116
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 116
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 156
ADDP4
ADDRLP4 116
INDIRP4
CNSTI4 116
ADDP4
INDIRB
ASGNB 12
line 1535
;1534:
;1535:	self->s.loopSound = 0;
ADDRFP4 0
INDIRP4
CNSTI4 200
ADDP4
CNSTI4 0
ASGNI4
line 1537
;1536:
;1537:	self->r.maxs[2] = -8;
ADDRFP4 0
INDIRP4
CNSTI4 336
ADDP4
CNSTF4 3238002688
ASGNF4
line 1541
;1538:
;1539:	// don't allow respawn until the death anim is done
;1540:	// g_forcerespawn may force spawning at some later time
;1541:	self->client->respawnTime = level.time + 1700;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1672
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1700
ADDI4
ASGNI4
line 1544
;1542:
;1543:	// remove powerups
;1544:	memset( self->client->ps.powerups, 0, sizeof(self->client->ps.powerups) );
ADDRFP4 0
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
line 1556
;1545:
;1546:	// NOTENOTE No gib deaths right now, this is star wars.
;1547:	/*
;1548:	// never gib in a nodrop
;1549:	if ( (self->health <= GIB_HEALTH && !(contents & CONTENTS_NODROP) && g_blood.integer) || meansOfDeath == MOD_SUICIDE) 
;1550:	{
;1551:		// gib death
;1552:		GibEntity( self, killer );
;1553:	} 
;1554:	else 
;1555:	*/
;1556:	{
line 1561
;1557:		// normal death
;1558:		
;1559:		static int i;
;1560:
;1561:		switch ( i ) {
ADDRLP4 120
ADDRGP4 $536
INDIRI4
ASGNI4
ADDRLP4 120
INDIRI4
CNSTI4 0
EQI4 $539
ADDRLP4 120
INDIRI4
CNSTI4 1
EQI4 $540
ADDRLP4 120
INDIRI4
CNSTI4 2
EQI4 $541
ADDRGP4 $537
JUMPV
LABELV $539
line 1563
;1562:		case 0:
;1563:			anim = BOTH_DEATH1;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 1564
;1564:			break;
ADDRGP4 $538
JUMPV
LABELV $540
line 1566
;1565:		case 1:
;1566:			anim = BOTH_DEATH2;
ADDRLP4 12
CNSTI4 1
ASGNI4
line 1567
;1567:			break;
ADDRGP4 $538
JUMPV
LABELV $541
LABELV $537
line 1570
;1568:		case 2:
;1569:		default:
;1570:			anim = BOTH_DEATH3;
ADDRLP4 12
CNSTI4 2
ASGNI4
line 1571
;1571:			break;
LABELV $538
line 1574
;1572:		}
;1573:		
;1574:		anim = G_PickDeathAnim(self, self->pos1, damage, meansOfDeath, HL_NONE);
ADDRLP4 124
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 124
INDIRP4
ARGP4
ADDRLP4 124
INDIRP4
CNSTI4 548
ADDP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRFP4 16
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRLP4 128
ADDRGP4 G_PickDeathAnim
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 128
INDIRI4
ASGNI4
line 1576
;1575:
;1576:		if (anim < 1)
ADDRLP4 12
INDIRI4
CNSTI4 1
GEI4 $542
line 1577
;1577:		{
line 1578
;1578:			anim = BOTH_DEATH1;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 1579
;1579:		}
LABELV $542
line 1581
;1580:
;1581:		if (meansOfDeath == MOD_SABER)
ADDRFP4 16
INDIRI4
CNSTI4 3
NEI4 $544
line 1582
;1582:		{
line 1583
;1583:			G_CheckForDismemberment(self, self->pos1, damage, anim);
ADDRLP4 132
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 132
INDIRP4
ARGP4
ADDRLP4 132
INDIRP4
CNSTI4 548
ADDP4
ARGP4
ADDRFP4 12
INDIRI4
ARGI4
ADDRLP4 12
INDIRI4
ARGI4
ADDRGP4 G_CheckForDismemberment
CALLV
pop
line 1584
;1584:		}
LABELV $544
line 1588
;1585:
;1586:		// for the no-blood option, we need to prevent the health
;1587:		// from going to gib level
;1588:		if ( self->health <= GIB_HEALTH ) {
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 -40
GTI4 $546
line 1589
;1589:			self->health = GIB_HEALTH+1;
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 -39
ASGNI4
line 1590
;1590:		}
LABELV $546
line 1592
;1591:
;1592:		self->client->respawnTime = level.time + 1000;//((self->client->animations[anim].numFrames*40)/(50.0f / self->client->animations[anim].frameLerp))+300;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1672
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 1000
ADDI4
ASGNI4
line 1594
;1593:
;1594:		self->client->ps.legsAnim = 
ADDRLP4 132
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 92
ADDP4
ASGNP4
ADDRLP4 136
CNSTI4 2048
ASGNI4
ADDRLP4 132
INDIRP4
ADDRLP4 132
INDIRP4
INDIRI4
ADDRLP4 136
INDIRI4
BANDI4
ADDRLP4 136
INDIRI4
BXORI4
ADDRLP4 12
INDIRI4
BORI4
ASGNI4
line 1596
;1595:			( ( self->client->ps.legsAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;
;1596:		self->client->ps.torsoAnim = 
ADDRLP4 140
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 100
ADDP4
ASGNP4
ADDRLP4 144
CNSTI4 2048
ASGNI4
ADDRLP4 140
INDIRP4
ADDRLP4 140
INDIRP4
INDIRI4
ADDRLP4 144
INDIRI4
BANDI4
ADDRLP4 144
INDIRI4
BXORI4
ADDRLP4 12
INDIRI4
BORI4
ASGNI4
line 1604
;1597:			( ( self->client->ps.torsoAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;
;1598://		self->client->ps.pm_flags |= PMF_UPDATE_ANIM;		// Make sure the pmove sets up the GHOUL2 anims.
;1599:
;1600:		//rww - do this on respawn, not death
;1601:		//CopyToBodyQue (self);
;1602:
;1603:		//G_AddEvent( self, EV_DEATH1 + i, killer );
;1604:		if (wasJediMaster)
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $549
line 1605
;1605:		{
line 1606
;1606:			G_AddEvent( self, EV_DEATH1 + i, 1 );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $536
INDIRI4
CNSTI4 78
ADDI4
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 1607
;1607:		}
ADDRGP4 $550
JUMPV
LABELV $549
line 1609
;1608:		else
;1609:		{
line 1610
;1610:			G_AddEvent( self, EV_DEATH1 + i, 0 );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $536
INDIRI4
CNSTI4 78
ADDI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 1611
;1611:		}
LABELV $550
line 1614
;1612:
;1613:		// the body can still be gibbed
;1614:		self->die = body_die;
ADDRFP4 0
INDIRP4
CNSTI4 660
ADDP4
ADDRGP4 body_die
ASGNP4
line 1617
;1615:
;1616:		//It won't gib, it will disintegrate (because this is Star Wars).
;1617:		self->takedamage = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
CNSTI4 1
ASGNI4
line 1620
;1618:
;1619:		// globally cycle through the different death animations
;1620:		i = ( i + 1 ) % 3;
ADDRLP4 148
ADDRGP4 $536
ASGNP4
ADDRLP4 148
INDIRP4
ADDRLP4 148
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
CNSTI4 3
MODI4
ASGNI4
line 1621
;1621:	}
line 1623
;1622:
;1623:	trap_LinkEntity (self);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1625
;1624:
;1625:}
LABELV $408
endproc player_die 152 28
export CheckArmor
proc CheckArmor 16 4
line 1634
;1626:
;1627:
;1628:/*
;1629:================
;1630:CheckArmor
;1631:================
;1632:*/
;1633:int CheckArmor (gentity_t *ent, int damage, int dflags)
;1634:{
line 1639
;1635:	gclient_t	*client;
;1636:	int			save;
;1637:	int			count;
;1638:
;1639:	if (!damage)
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $552
line 1640
;1640:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $551
JUMPV
LABELV $552
line 1642
;1641:
;1642:	client = ent->client;
ADDRLP4 4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
line 1644
;1643:
;1644:	if (!client)
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $554
line 1645
;1645:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $551
JUMPV
LABELV $554
line 1647
;1646:
;1647:	if (dflags & DAMAGE_NO_ARMOR)
ADDRFP4 8
INDIRI4
CNSTI4 2
BANDI4
CNSTI4 0
EQI4 $556
line 1648
;1648:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $551
JUMPV
LABELV $556
line 1651
;1649:
;1650:	// armor
;1651:	count = client->ps.stats[STAT_ARMOR];
ADDRLP4 8
ADDRLP4 4
INDIRP4
CNSTI4 236
ADDP4
INDIRI4
ASGNI4
line 1653
;1652:
;1653:	if (dflags & DAMAGE_HALF_ABSORB)
ADDRFP4 8
INDIRI4
CNSTI4 1024
BANDI4
CNSTI4 0
EQI4 $558
line 1654
;1654:	{	// Half the damage gets absorbed by the shields, rather than 100%
line 1655
;1655:		save = ceil( damage * ARMOR_PROTECTION );
CNSTF4 1056964608
ADDRFP4 4
INDIRI4
CVIF4 4
MULF4
ARGF4
ADDRLP4 12
ADDRGP4 ceil
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 12
INDIRF4
CVFI4 4
ASGNI4
line 1656
;1656:	}
ADDRGP4 $559
JUMPV
LABELV $558
line 1658
;1657:	else
;1658:	{	// All the damage gets absorbed by the shields.
line 1659
;1659:		save = damage;
ADDRLP4 0
ADDRFP4 4
INDIRI4
ASGNI4
line 1660
;1660:	}
LABELV $559
line 1663
;1661:
;1662:	// save is the most damage that the armor is elibigle to protect, of course, but it's limited by the total armor.
;1663:	if (save >= count)
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $560
line 1664
;1664:		save = count;
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
LABELV $560
line 1666
;1665:
;1666:	if (!save)
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $562
line 1667
;1667:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $551
JUMPV
LABELV $562
line 1669
;1668:
;1669:	if (dflags & DAMAGE_HALF_ARMOR_REDUCTION)		// Armor isn't whittled so easily by sniper shots.
ADDRFP4 8
INDIRI4
CNSTI4 2048
BANDI4
CNSTI4 0
EQI4 $564
line 1670
;1670:	{
line 1671
;1671:		client->ps.stats[STAT_ARMOR] -= (int)(save*ARMOR_REDUCTION_FACTOR);
ADDRLP4 12
ADDRLP4 4
INDIRP4
CNSTI4 236
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTF4 1056964608
ADDRLP4 0
INDIRI4
CVIF4 4
MULF4
CVFI4 4
SUBI4
ASGNI4
line 1672
;1672:	}
ADDRGP4 $565
JUMPV
LABELV $564
line 1674
;1673:	else
;1674:	{
line 1675
;1675:		client->ps.stats[STAT_ARMOR] -= save;
ADDRLP4 12
ADDRLP4 4
INDIRP4
CNSTI4 236
ADDP4
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
ASGNI4
line 1676
;1676:	}
LABELV $565
line 1678
;1677:
;1678:	return save;
ADDRLP4 0
INDIRI4
RETI4
LABELV $551
endproc CheckArmor 16 4
export G_ApplyKnockback
proc G_ApplyKnockback 48 0
line 1683
;1679:}
;1680:
;1681:
;1682:void G_ApplyKnockback( gentity_t *targ, vec3_t newDir, float knockback )
;1683:{
line 1687
;1684:	vec3_t	kvel;
;1685:	float	mass;
;1686:
;1687:	if (targ && targ->client && targ->client->ps.usingATST)
ADDRLP4 16
ADDRFP4 0
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
EQU4 $567
ADDRLP4 24
ADDRLP4 16
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 24
INDIRP4
CVPU4 4
ADDRLP4 20
INDIRU4
EQU4 $567
ADDRLP4 24
INDIRP4
CNSTI4 1304
ADDP4
INDIRI4
CNSTI4 0
EQI4 $567
line 1688
;1688:	{
line 1689
;1689:		return;
ADDRGP4 $566
JUMPV
LABELV $567
line 1692
;1690:	}
;1691:
;1692:	if ( targ->physicsBounce > 0 )	//overide the mass
ADDRFP4 0
INDIRP4
CNSTI4 504
ADDP4
INDIRF4
CNSTF4 0
LEF4 $569
line 1693
;1693:		mass = targ->physicsBounce;
ADDRLP4 12
ADDRFP4 0
INDIRP4
CNSTI4 504
ADDP4
INDIRF4
ASGNF4
ADDRGP4 $570
JUMPV
LABELV $569
line 1695
;1694:	else
;1695:		mass = 200;
ADDRLP4 12
CNSTF4 1128792064
ASGNF4
LABELV $570
line 1697
;1696:
;1697:	if ( g_gravity.value > 0 )
ADDRGP4 g_gravity+8
INDIRF4
CNSTF4 0
LEF4 $571
line 1698
;1698:	{
line 1699
;1699:		VectorScale( newDir, g_knockback.value * (float)knockback / mass * 0.8, kvel );
ADDRLP4 28
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 32
CNSTF4 1061997773
ASGNF4
ADDRLP4 36
ADDRFP4 8
INDIRF4
ASGNF4
ADDRLP4 0
ADDRLP4 28
INDIRP4
INDIRF4
ADDRLP4 32
INDIRF4
ADDRGP4 g_knockback+8
INDIRF4
ADDRLP4 36
INDIRF4
MULF4
ADDRLP4 12
INDIRF4
DIVF4
MULF4
MULF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 28
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 32
INDIRF4
ADDRGP4 g_knockback+8
INDIRF4
ADDRLP4 36
INDIRF4
MULF4
ADDRLP4 12
INDIRF4
DIVF4
MULF4
MULF4
ASGNF4
ADDRLP4 0+8
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CNSTF4 1061997773
ADDRGP4 g_knockback+8
INDIRF4
ADDRFP4 8
INDIRF4
MULF4
ADDRLP4 12
INDIRF4
DIVF4
MULF4
MULF4
ASGNF4
line 1700
;1700:		kvel[2] = newDir[2] * g_knockback.value * (float)knockback / mass * 1.5;
ADDRLP4 0+8
CNSTF4 1069547520
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRGP4 g_knockback+8
INDIRF4
MULF4
ADDRFP4 8
INDIRF4
MULF4
ADDRLP4 12
INDIRF4
DIVF4
MULF4
ASGNF4
line 1701
;1701:	}
ADDRGP4 $572
JUMPV
LABELV $571
line 1703
;1702:	else
;1703:	{
line 1704
;1704:		VectorScale( newDir, g_knockback.value * (float)knockback / mass, kvel );
ADDRLP4 28
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 32
ADDRFP4 8
INDIRF4
ASGNF4
ADDRLP4 0
ADDRLP4 28
INDIRP4
INDIRF4
ADDRGP4 g_knockback+8
INDIRF4
ADDRLP4 32
INDIRF4
MULF4
ADDRLP4 12
INDIRF4
DIVF4
MULF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 28
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRGP4 g_knockback+8
INDIRF4
ADDRLP4 32
INDIRF4
MULF4
ADDRLP4 12
INDIRF4
DIVF4
MULF4
ASGNF4
ADDRLP4 0+8
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRGP4 g_knockback+8
INDIRF4
ADDRFP4 8
INDIRF4
MULF4
ADDRLP4 12
INDIRF4
DIVF4
MULF4
ASGNF4
line 1705
;1705:	}
LABELV $572
line 1707
;1706:
;1707:	if ( targ->client )
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $586
line 1708
;1708:	{
line 1709
;1709:		VectorAdd( targ->client->ps.velocity, kvel, targ->client->ps.velocity );
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 28
INDIRP4
INDIRF4
ADDRLP4 0
INDIRF4
ADDF4
ASGNF4
ADDRLP4 32
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRF4
ADDRLP4 0+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 36
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRF4
ADDRLP4 0+8
INDIRF4
ADDF4
ASGNF4
line 1710
;1710:	}
ADDRGP4 $587
JUMPV
LABELV $586
line 1711
;1711:	else if ( targ->s.pos.trType != TR_STATIONARY && targ->s.pos.trType != TR_LINEAR_STOP )
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $590
ADDRLP4 28
INDIRI4
CNSTI4 3
EQI4 $590
line 1712
;1712:	{
line 1713
;1713:		VectorAdd( targ->s.pos.trDelta, kvel, targ->s.pos.trDelta );
ADDRLP4 32
ADDRFP4 0
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 32
INDIRP4
ADDRLP4 32
INDIRP4
INDIRF4
ADDRLP4 0
INDIRF4
ADDF4
ASGNF4
ADDRLP4 36
ADDRFP4 0
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 36
INDIRP4
INDIRF4
ADDRLP4 0+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 40
ADDRFP4 0
INDIRP4
CNSTI4 44
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRF4
ADDRLP4 0+8
INDIRF4
ADDF4
ASGNF4
line 1714
;1714:		VectorCopy( targ->r.currentOrigin, targ->s.pos.trBase );
ADDRLP4 44
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 44
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 1715
;1715:		targ->s.pos.trTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1716
;1716:	}
LABELV $590
LABELV $587
line 1720
;1717:
;1718:	// set the timer so that the other client can't cancel
;1719:	// out the movement immediately
;1720:	if ( targ->client && !targ->client->ps.pm_time ) 
ADDRLP4 32
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 32
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $595
ADDRLP4 32
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 0
NEI4 $595
line 1721
;1721:	{
line 1724
;1722:		int		t;
;1723:
;1724:		t = knockback * 2;
ADDRLP4 36
CNSTF4 1073741824
ADDRFP4 8
INDIRF4
MULF4
CVFI4 4
ASGNI4
line 1725
;1725:		if ( t < 50 ) {
ADDRLP4 36
INDIRI4
CNSTI4 50
GEI4 $597
line 1726
;1726:			t = 50;
ADDRLP4 36
CNSTI4 50
ASGNI4
line 1727
;1727:		}
LABELV $597
line 1728
;1728:		if ( t > 200 ) {
ADDRLP4 36
INDIRI4
CNSTI4 200
LEI4 $599
line 1729
;1729:			t = 200;
ADDRLP4 36
CNSTI4 200
ASGNI4
line 1730
;1730:		}
LABELV $599
line 1731
;1731:		targ->client->ps.pm_time = t;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 16
ADDP4
ADDRLP4 36
INDIRI4
ASGNI4
line 1732
;1732:		targ->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
ADDRLP4 40
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 40
INDIRP4
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 1733
;1733:	}
LABELV $595
line 1734
;1734:}
LABELV $566
endproc G_ApplyKnockback 48 0
export RaySphereIntersections
proc RaySphereIntersections 96 4
line 1741
;1735:
;1736:/*
;1737:================
;1738:RaySphereIntersections
;1739:================
;1740:*/
;1741:int RaySphereIntersections( vec3_t origin, float radius, vec3_t point, vec3_t dir, vec3_t intersections[2] ) {
line 1750
;1742:	float b, c, d, t;
;1743:
;1744:	//	| origin - (point + t * dir) | = radius
;1745:	//	a = dir[0]^2 + dir[1]^2 + dir[2]^2;
;1746:	//	b = 2 * (dir[0] * (point[0] - origin[0]) + dir[1] * (point[1] - origin[1]) + dir[2] * (point[2] - origin[2]));
;1747:	//	c = (point[0] - origin[0])^2 + (point[1] - origin[1])^2 + (point[2] - origin[2])^2 - radius^2;
;1748:
;1749:	// normalize dir so a = 1
;1750:	VectorNormalize(dir);
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 1751
;1751:	b = 2 * (dir[0] * (point[0] - origin[0]) + dir[1] * (point[1] - origin[1]) + dir[2] * (point[2] - origin[2]));
ADDRLP4 16
ADDRFP4 12
INDIRP4
ASGNP4
ADDRLP4 20
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
CNSTI4 4
ASGNI4
ADDRLP4 32
CNSTI4 8
ASGNI4
ADDRLP4 4
CNSTF4 1073741824
ADDRLP4 16
INDIRP4
INDIRF4
ADDRLP4 20
INDIRP4
INDIRF4
ADDRLP4 24
INDIRP4
INDIRF4
SUBF4
MULF4
ADDRLP4 16
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
INDIRF4
ADDRLP4 20
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
INDIRF4
ADDRLP4 24
INDIRP4
ADDRLP4 28
INDIRI4
ADDP4
INDIRF4
SUBF4
MULF4
ADDF4
ADDRLP4 16
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
ADDRLP4 20
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
ADDRLP4 24
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
SUBF4
MULF4
ADDF4
MULF4
ASGNF4
line 1752
;1752:	c = (point[0] - origin[0]) * (point[0] - origin[0]) +
ADDRLP4 36
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
ADDRLP4 36
INDIRP4
INDIRF4
ADDRLP4 40
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 48
CNSTI4 4
ASGNI4
ADDRLP4 52
ADDRLP4 36
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 56
CNSTI4 8
ASGNI4
ADDRLP4 60
ADDRLP4 36
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
INDIRF4
ADDRLP4 40
INDIRP4
ADDRLP4 56
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 64
ADDRFP4 4
INDIRF4
ASGNF4
ADDRLP4 12
ADDRLP4 44
INDIRF4
ADDRLP4 44
INDIRF4
MULF4
ADDRLP4 52
INDIRF4
ADDRLP4 52
INDIRF4
MULF4
ADDF4
ADDRLP4 60
INDIRF4
ADDRLP4 60
INDIRF4
MULF4
ADDF4
ADDRLP4 64
INDIRF4
ADDRLP4 64
INDIRF4
MULF4
SUBF4
ASGNF4
line 1757
;1753:		(point[1] - origin[1]) * (point[1] - origin[1]) +
;1754:		(point[2] - origin[2]) * (point[2] - origin[2]) -
;1755:		radius * radius;
;1756:
;1757:	d = b * b - 4 * c;
ADDRLP4 8
ADDRLP4 4
INDIRF4
ADDRLP4 4
INDIRF4
MULF4
CNSTF4 1082130432
ADDRLP4 12
INDIRF4
MULF4
SUBF4
ASGNF4
line 1758
;1758:	if (d > 0) {
ADDRLP4 8
INDIRF4
CNSTF4 0
LEF4 $602
line 1759
;1759:		t = (- b + sqrt(d)) / 2;
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 72
ADDRGP4 sqrt
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 4
INDIRF4
NEGF4
ADDRLP4 72
INDIRF4
ADDF4
CNSTF4 1073741824
DIVF4
ASGNF4
line 1760
;1760:		VectorMA(point, t, dir, intersections[0]);
ADDRFP4 16
INDIRP4
ADDRFP4 8
INDIRP4
INDIRF4
ADDRFP4 12
INDIRP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 76
CNSTI4 4
ASGNI4
ADDRFP4 16
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 80
CNSTI4 8
ASGNI4
ADDRFP4 16
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 80
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 1761
;1761:		t = (- b - sqrt(d)) / 2;
ADDRLP4 8
INDIRF4
ARGF4
ADDRLP4 84
ADDRGP4 sqrt
CALLF4
ASGNF4
ADDRLP4 0
ADDRLP4 4
INDIRF4
NEGF4
ADDRLP4 84
INDIRF4
SUBF4
CNSTF4 1073741824
DIVF4
ASGNF4
line 1762
;1762:		VectorMA(point, t, dir, intersections[1]);
ADDRFP4 16
INDIRP4
CNSTI4 12
ADDP4
ADDRFP4 8
INDIRP4
INDIRF4
ADDRFP4 12
INDIRP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 88
CNSTI4 4
ASGNI4
ADDRFP4 16
INDIRP4
CNSTI4 16
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 88
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 88
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 92
CNSTI4 8
ASGNI4
ADDRFP4 16
INDIRP4
CNSTI4 20
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 92
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 92
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 1763
;1763:		return 2;
CNSTI4 2
RETI4
ADDRGP4 $601
JUMPV
LABELV $602
line 1765
;1764:	}
;1765:	else if (d == 0) {
ADDRLP4 8
INDIRF4
CNSTF4 0
NEF4 $604
line 1766
;1766:		t = (- b ) / 2;
ADDRLP4 0
ADDRLP4 4
INDIRF4
NEGF4
CNSTF4 1073741824
DIVF4
ASGNF4
line 1767
;1767:		VectorMA(point, t, dir, intersections[0]);
ADDRFP4 16
INDIRP4
ADDRFP4 8
INDIRP4
INDIRF4
ADDRFP4 12
INDIRP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 72
CNSTI4 4
ASGNI4
ADDRFP4 16
INDIRP4
ADDRLP4 72
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 72
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 72
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 76
CNSTI4 8
ASGNI4
ADDRFP4 16
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
ADDRFP4 8
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRF4
ADDRFP4 12
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 1768
;1768:		return 1;
CNSTI4 1
RETI4
ADDRGP4 $601
JUMPV
LABELV $604
line 1770
;1769:	}
;1770:	return 0;
CNSTI4 0
RETI4
LABELV $601
endproc RaySphereIntersections 96 4
export LimbTouch
proc LimbTouch 0 0
line 1774
;1771:}
;1772:
;1773:void LimbTouch( gentity_t *self, gentity_t *other, trace_t *trace )
;1774:{
line 1775
;1775:}
LABELV $606
endproc LimbTouch 0 0
export LimbThink
proc LimbThink 12 4
line 1778
;1776:
;1777:void LimbThink( gentity_t *ent )
;1778:{
line 1779
;1779:	if (ent->speed < level.time)
ADDRFP4 0
INDIRP4
CNSTI4 608
ADDP4
INDIRF4
ADDRGP4 level+32
INDIRI4
CVIF4 4
GEF4 $608
line 1780
;1780:	{
line 1781
;1781:		ent->think = G_FreeEntity;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 G_FreeEntity
ASGNP4
line 1782
;1782:		ent->nextthink = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1783
;1783:		return;
ADDRGP4 $607
JUMPV
LABELV $608
line 1786
;1784:	}
;1785:
;1786:	if (ent->s.pos.trType != TR_GRAVITY)
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
INDIRI4
CNSTI4 5
EQI4 $612
line 1787
;1787:	{
line 1788
;1788:		int addamt = (ent->speed - level.time);
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 608
ADDP4
INDIRF4
ADDRGP4 level+32
INDIRI4
CVIF4 4
SUBF4
CVFI4 4
ASGNI4
line 1790
;1789:
;1790:		if (addamt > 5000)
ADDRLP4 0
INDIRI4
CNSTI4 5000
LEI4 $615
line 1791
;1791:		{
line 1792
;1792:			addamt = 5000;
ADDRLP4 0
CNSTI4 5000
ASGNI4
line 1793
;1793:		}
LABELV $615
line 1794
;1794:		if (addamt < 0)
ADDRLP4 0
INDIRI4
CNSTI4 0
GEI4 $617
line 1795
;1795:		{
line 1796
;1796:			addamt = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1797
;1797:		}
LABELV $617
line 1799
;1798:
;1799:		VectorClear(ent->s.pos.trDelta);
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
CNSTF4 0
ASGNF4
ADDRLP4 4
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 8
INDIRF4
ASGNF4
ADDRLP4 4
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 8
INDIRF4
ASGNF4
ADDRLP4 4
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 8
INDIRF4
ASGNF4
line 1800
;1800:		ent->think = G_FreeEntity;
ADDRFP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 G_FreeEntity
ASGNP4
line 1801
;1801:		ent->nextthink = level.time + addamt;
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
line 1802
;1802:		return;
ADDRGP4 $607
JUMPV
LABELV $612
line 1805
;1803:	}
;1804:
;1805:	G_RunMissile(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_RunMissile
CALLV
pop
line 1807
;1806:
;1807:	G_RunObject(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_RunObject
CALLV
pop
line 1808
;1808:}
LABELV $607
endproc LimbThink 12 4
export G_GetDismemberBolt
proc G_GetDismemberBolt 132 36
line 1813
;1809:
;1810:void G_G2PlayerAngles( gentity_t *ent, vec3_t legs[3], vec3_t legsAngles);
;1811:
;1812:void G_GetDismemberBolt(gentity_t *self, vec3_t boltPoint, int limbType)
;1813:{
line 1814
;1814:	int useBolt = self->bolt_Head;
ADDRLP4 88
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
ASGNI4
line 1818
;1815:	vec3_t properOrigin, properAngles, addVel;
;1816:	vec3_t legAxis[3];
;1817:	mdxaBone_t	boltMatrix;
;1818:	float fVSpeed = 0;
ADDRLP4 0
CNSTF4 0
ASGNF4
line 1820
;1819:
;1820:	switch (limbType)
ADDRLP4 128
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 128
INDIRI4
CNSTI4 10
LTI4 $621
ADDRLP4 128
INDIRI4
CNSTI4 15
GTI4 $621
ADDRLP4 128
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $629-40
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $629
address $623
address $624
address $625
address $626
address $627
address $628
code
line 1821
;1821:	{
LABELV $623
line 1823
;1822:	case G2_MODELPART_HEAD:
;1823:		useBolt = self->bolt_Head;
ADDRLP4 88
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
ASGNI4
line 1824
;1824:		break;
ADDRGP4 $622
JUMPV
LABELV $624
line 1826
;1825:	case G2_MODELPART_WAIST:
;1826:		useBolt = self->bolt_Waist;
ADDRLP4 88
ADDRFP4 0
INDIRP4
CNSTI4 804
ADDP4
INDIRI4
ASGNI4
line 1827
;1827:		break;
ADDRGP4 $622
JUMPV
LABELV $625
line 1829
;1828:	case G2_MODELPART_LARM:
;1829:		useBolt = self->bolt_LArm;
ADDRLP4 88
ADDRFP4 0
INDIRP4
CNSTI4 788
ADDP4
INDIRI4
ASGNI4
line 1830
;1830:		break;
ADDRGP4 $622
JUMPV
LABELV $626
line 1832
;1831:	case G2_MODELPART_RARM:
;1832:		useBolt = self->bolt_RArm;
ADDRLP4 88
ADDRFP4 0
INDIRP4
CNSTI4 792
ADDP4
INDIRI4
ASGNI4
line 1833
;1833:		break;
ADDRGP4 $622
JUMPV
LABELV $627
line 1835
;1834:	case G2_MODELPART_LLEG:
;1835:		useBolt = self->bolt_LLeg;
ADDRLP4 88
ADDRFP4 0
INDIRP4
CNSTI4 796
ADDP4
INDIRI4
ASGNI4
line 1836
;1836:		break;
ADDRGP4 $622
JUMPV
LABELV $628
line 1838
;1837:	case G2_MODELPART_RLEG:
;1838:		useBolt = self->bolt_RLeg;
ADDRLP4 88
ADDRFP4 0
INDIRP4
CNSTI4 800
ADDP4
INDIRI4
ASGNI4
line 1839
;1839:		break;
ADDRGP4 $622
JUMPV
LABELV $621
line 1841
;1840:	default:
;1841:		useBolt = self->bolt_Head;
ADDRLP4 88
ADDRFP4 0
INDIRP4
CNSTI4 784
ADDP4
INDIRI4
ASGNI4
line 1842
;1842:		break;
LABELV $622
line 1845
;1843:	}
;1844:
;1845:	VectorCopy(self->client->ps.origin, properOrigin);
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 1846
;1846:	VectorCopy(self->client->ps.viewangles, properAngles);
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
line 1849
;1847:
;1848:	//try to predict the origin based on velocity so it's more like what the client is seeing
;1849:	VectorCopy(self->client->ps.velocity, addVel);
ADDRLP4 28
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRB
ASGNB 12
line 1850
;1850:	VectorNormalize(addVel);
ADDRLP4 28
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 1852
;1851:
;1852:	if (self->client->ps.velocity[0] < 0)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
CNSTF4 0
GEF4 $631
line 1853
;1853:	{
line 1854
;1854:		fVSpeed += (-self->client->ps.velocity[0]);
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
NEGF4
ADDF4
ASGNF4
line 1855
;1855:	}
ADDRGP4 $632
JUMPV
LABELV $631
line 1857
;1856:	else
;1857:	{
line 1858
;1858:		fVSpeed += self->client->ps.velocity[0];
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRF4
ADDF4
ASGNF4
line 1859
;1859:	}
LABELV $632
line 1860
;1860:	if (self->client->ps.velocity[1] < 0)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
CNSTF4 0
GEF4 $633
line 1861
;1861:	{
line 1862
;1862:		fVSpeed += (-self->client->ps.velocity[1]);
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
NEGF4
ADDF4
ASGNF4
line 1863
;1863:	}
ADDRGP4 $634
JUMPV
LABELV $633
line 1865
;1864:	else
;1865:	{
line 1866
;1866:		fVSpeed += self->client->ps.velocity[1];
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 36
ADDP4
INDIRF4
ADDF4
ASGNF4
line 1867
;1867:	}
LABELV $634
line 1868
;1868:	if (self->client->ps.velocity[2] < 0)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
CNSTF4 0
GEF4 $635
line 1869
;1869:	{
line 1870
;1870:		fVSpeed += (-self->client->ps.velocity[2]);
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
NEGF4
ADDF4
ASGNF4
line 1871
;1871:	}
ADDRGP4 $636
JUMPV
LABELV $635
line 1873
;1872:	else
;1873:	{
line 1874
;1874:		fVSpeed += self->client->ps.velocity[2];
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 40
ADDP4
INDIRF4
ADDF4
ASGNF4
line 1875
;1875:	}
LABELV $636
line 1877
;1876:
;1877:	fVSpeed *= 0.08;
ADDRLP4 0
CNSTF4 1034147594
ADDRLP4 0
INDIRF4
MULF4
ASGNF4
line 1879
;1878:
;1879:	properOrigin[0] += addVel[0]*fVSpeed;
ADDRLP4 16
ADDRLP4 16
INDIRF4
ADDRLP4 28
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 1880
;1880:	properOrigin[1] += addVel[1]*fVSpeed;
ADDRLP4 16+4
ADDRLP4 16+4
INDIRF4
ADDRLP4 28+4
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 1881
;1881:	properOrigin[2] += addVel[2]*fVSpeed;
ADDRLP4 16+8
ADDRLP4 16+8
INDIRF4
ADDRLP4 28+8
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDF4
ASGNF4
line 1883
;1882:
;1883:	properAngles[0] = 0;
ADDRLP4 4
CNSTF4 0
ASGNF4
line 1884
;1884:	properAngles[1] = self->client->ps.viewangles[YAW];
ADDRLP4 4+4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 160
ADDP4
INDIRF4
ASGNF4
line 1885
;1885:	properAngles[2] = 0;
ADDRLP4 4+8
CNSTF4 0
ASGNF4
line 1887
;1886:
;1887:	AnglesToAxis( properAngles, legAxis );
ADDRLP4 4
ARGP4
ADDRLP4 92
ARGP4
ADDRGP4 AnglesToAxis
CALLV
pop
line 1888
;1888:	G_G2PlayerAngles( self, legAxis, properAngles );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 92
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 G_G2PlayerAngles
CALLV
pop
line 1890
;1889:
;1890:	trap_G2API_GetBoltMatrix(self->client->ghoul2, 0, useBolt, &boltMatrix, properAngles, properOrigin, level.time, NULL, vec3_origin);
ADDRFP4 0
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
ADDRLP4 88
INDIRI4
ARGI4
ADDRLP4 40
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 16
ARGP4
ADDRGP4 level+32
INDIRI4
ARGI4
CNSTP4 0
ARGP4
ADDRGP4 vec3_origin
ARGP4
ADDRGP4 trap_G2API_GetBoltMatrix
CALLI4
pop
line 1892
;1891:
;1892:	boltPoint[0] = boltMatrix.matrix[0][3];
ADDRFP4 4
INDIRP4
ADDRLP4 40+12
INDIRF4
ASGNF4
line 1893
;1893:	boltPoint[1] = boltMatrix.matrix[1][3];
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 40+16+12
INDIRF4
ASGNF4
line 1894
;1894:	boltPoint[2] = boltMatrix.matrix[2][3];
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 40+32+12
INDIRF4
ASGNF4
line 1895
;1895:}
LABELV $620
endproc G_GetDismemberBolt 132 36
export G_Dismember
proc G_Dismember 100 8
line 1898
;1896:
;1897:void G_Dismember( gentity_t *ent, vec3_t point, int limbType, float limbRollBase, float limbPitchBase, int deathAnim )
;1898:{
line 1902
;1899:	vec3_t	dir, newPoint, vel;
;1900:	gentity_t *limb;
;1901:
;1902:	VectorCopy( point, newPoint );
ADDRLP4 28
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 1903
;1903:	limb = G_Spawn();
ADDRLP4 40
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 40
INDIRP4
ASGNP4
line 1904
;1904:	limb->classname = "playerlimb";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $650
ASGNP4
line 1905
;1905:	G_SetOrigin( limb, newPoint );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 28
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 1906
;1906:	VectorCopy( newPoint, limb->s.pos.trBase );
ADDRLP4 0
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 28
INDIRB
ASGNB 12
line 1907
;1907:	limb->think = LimbThink;
ADDRLP4 0
INDIRP4
CNSTI4 636
ADDP4
ADDRGP4 LimbThink
ASGNP4
line 1908
;1908:	limb->touch = LimbTouch;
ADDRLP4 0
INDIRP4
CNSTI4 648
ADDP4
ADDRGP4 LimbTouch
ASGNP4
line 1909
;1909:	limb->speed = level.time + Q_irand(4000, 8000);
CNSTI4 4000
ARGI4
CNSTI4 8000
ARGI4
ADDRLP4 44
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 608
ADDP4
ADDRGP4 level+32
INDIRI4
ADDRLP4 44
INDIRI4
ADDI4
CVIF4 4
ASGNF4
line 1910
;1910:	limb->nextthink = level.time + FRAMETIME;
ADDRLP4 0
INDIRP4
CNSTI4 632
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 1913
;1911:
;1912:	//need size, contents, clipmask
;1913:	limb->r.svFlags = SVF_USE_CURRENT_ORIGIN;
ADDRLP4 0
INDIRP4
CNSTI4 304
ADDP4
CNSTI4 128
ASGNI4
line 1914
;1914:	limb->clipmask = MASK_SOLID;
ADDRLP4 0
INDIRP4
CNSTI4 508
ADDP4
CNSTI4 1
ASGNI4
line 1915
;1915:	limb->r.contents = CONTENTS_TRIGGER;
ADDRLP4 0
INDIRP4
CNSTI4 340
ADDP4
CNSTI4 1024
ASGNI4
line 1916
;1916:	VectorSet( limb->r.mins, -3.0f, -3.0f, -3.0f );
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
line 1917
;1917:	VectorSet( limb->r.maxs, 3.0f, 3.0f, 3.0f );
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
line 1922
;1918://	VectorClear(limb->r.mins);
;1919://	VectorClear(limb->r.maxs);
;1920:
;1921:	//move it
;1922:	limb->s.eType = ET_GENERAL;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
CNSTI4 0
ASGNI4
line 1923
;1923:	limb->s.weapon = G2_MODEL_PART;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
CNSTI4 50
ASGNI4
line 1925
;1924:
;1925:	if (limbType == G2_MODELPART_HEAD)
ADDRFP4 8
INDIRI4
CNSTI4 10
NEI4 $653
line 1926
;1926:	{
line 1927
;1927:		limb->bounceCount = 2;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 2
ASGNI4
line 1928
;1928:	}
ADDRGP4 $654
JUMPV
LABELV $653
line 1930
;1929:	else
;1930:	{
line 1931
;1931:		limb->bounceCount = 1;
ADDRLP4 0
INDIRP4
CNSTI4 716
ADDP4
CNSTI4 1
ASGNI4
line 1932
;1932:	}
LABELV $654
line 1933
;1933:	limb->s.pos.trType = TR_GRAVITY;
ADDRLP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 5
ASGNI4
line 1934
;1934:	limb->s.pos.trTime = level.time;								// move a bit on the very first frame
ADDRLP4 0
INDIRP4
CNSTI4 16
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1935
;1935:	VectorSubtract( point, ent->r.currentOrigin, dir );
ADDRLP4 48
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 52
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 48
INDIRP4
INDIRF4
ADDRLP4 52
INDIRP4
CNSTI4 368
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 48
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 52
INDIRP4
CNSTI4 372
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 4+8
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 376
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1936
;1936:	VectorNormalize( dir );
ADDRLP4 4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 1937
;1937:	VectorCopy(ent->client->ps.velocity, vel);
ADDRLP4 16
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 32
ADDP4
INDIRB
ASGNB 12
line 1938
;1938:	VectorMA( vel, 100, dir, limb->s.pos.trDelta );
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 16
INDIRF4
CNSTF4 1120403456
ADDRLP4 4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 16+4
INDIRF4
CNSTF4 1120403456
ADDRLP4 4+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 16+8
INDIRF4
CNSTF4 1120403456
ADDRLP4 4+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 1941
;1939:
;1940:	//make it bounce some
;1941:	limb->s.eFlags |= EF_BOUNCE_HALF;
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
CNSTI4 32
BORI4
ASGNI4
line 1944
;1942:	//no trDuration?
;1943:	//spin it
;1944:	VectorClear( limb->s.apos.trBase );
ADDRLP4 64
CNSTF4 0
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
ADDRLP4 64
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ADDRLP4 64
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 64
INDIRF4
ASGNF4
line 1945
;1945:	limb->s.apos.trBase[0] = limbPitchBase;
ADDRLP4 0
INDIRP4
CNSTI4 60
ADDP4
ADDRFP4 16
INDIRF4
ASGNF4
line 1946
;1946:	limb->s.apos.trBase[1] = ent->client->ps.viewangles[1];
ADDRLP4 0
INDIRP4
CNSTI4 64
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 160
ADDP4
INDIRF4
ASGNF4
line 1947
;1947:	limb->s.apos.trBase[2] = limbRollBase;
ADDRLP4 0
INDIRP4
CNSTI4 68
ADDP4
ADDRFP4 12
INDIRF4
ASGNF4
line 1949
;1948:
;1949:	VectorClear( limb->s.apos.trDelta );
ADDRLP4 72
CNSTF4 0
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 80
ADDP4
ADDRLP4 72
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 76
ADDP4
ADDRLP4 72
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 72
ADDP4
ADDRLP4 72
INDIRF4
ASGNF4
line 1951
;1950:
;1951:	limb->s.apos.trDelta[0] = Q_irand( -300, 300 );
CNSTI4 -300
ARGI4
CNSTI4 300
ARGI4
ADDRLP4 76
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 72
ADDP4
ADDRLP4 76
INDIRI4
CVIF4 4
ASGNF4
line 1952
;1952:	limb->s.apos.trDelta[2] = Q_irand( -300, 300 );
CNSTI4 -300
ARGI4
CNSTI4 300
ARGI4
ADDRLP4 80
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 80
ADDP4
ADDRLP4 80
INDIRI4
CVIF4 4
ASGNF4
line 1953
;1953:	limb->s.apos.trDelta[1] = Q_irand( -300, 300 );
CNSTI4 -300
ARGI4
CNSTI4 300
ARGI4
ADDRLP4 84
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 76
ADDP4
ADDRLP4 84
INDIRI4
CVIF4 4
ASGNF4
line 1955
;1954:
;1955:	if (limbType == G2_MODELPART_WAIST)
ADDRFP4 8
INDIRI4
CNSTI4 11
NEI4 $662
line 1956
;1956:	{
line 1957
;1957:		limb->s.apos.trDelta[0] = Q_irand( -60, 60 );
CNSTI4 -60
ARGI4
CNSTI4 60
ARGI4
ADDRLP4 88
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 72
ADDP4
ADDRLP4 88
INDIRI4
CVIF4 4
ASGNF4
line 1958
;1958:		limb->s.apos.trDelta[2] = Q_irand( -60, 60 );
CNSTI4 -60
ARGI4
CNSTI4 60
ARGI4
ADDRLP4 92
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 80
ADDP4
ADDRLP4 92
INDIRI4
CVIF4 4
ASGNF4
line 1959
;1959:		limb->s.apos.trDelta[1] = Q_irand( -60, 60 );
CNSTI4 -60
ARGI4
CNSTI4 60
ARGI4
ADDRLP4 96
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 76
ADDP4
ADDRLP4 96
INDIRI4
CVIF4 4
ASGNF4
line 1960
;1960:	}
LABELV $662
line 1962
;1961:
;1962:	limb->s.apos.trTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 52
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 1963
;1963:	limb->s.apos.trType = TR_LINEAR;
ADDRLP4 0
INDIRP4
CNSTI4 48
ADDP4
CNSTI4 2
ASGNI4
line 1965
;1964:
;1965:	limb->s.modelGhoul2 = limbType;
ADDRLP4 0
INDIRP4
CNSTI4 204
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 1966
;1966:	limb->s.g2radius = 200;
ADDRLP4 0
INDIRP4
CNSTI4 208
ADDP4
CNSTI4 200
ASGNI4
line 1967
;1967:	limb->s.modelindex = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 212
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 1968
;1968:	limb->s.modelindex2 = deathAnim;
ADDRLP4 0
INDIRP4
CNSTI4 216
ADDP4
ADDRFP4 20
INDIRI4
ASGNI4
line 1970
;1969:
;1970:	trap_LinkEntity( limb );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 1971
;1971:}
LABELV $649
endproc G_Dismember 100 8
lit
align 4
LABELV $666
byte 4 0
byte 4 0
byte 4 0
export G_GetHitQuad
code
proc G_GetHitQuad 68 16
line 1991
;1972:
;1973:/*
;1974:void DismembermentTest(gentity_t *self)
;1975:{
;1976:	int sect = G2_MODELPART_HEAD;
;1977:	vec3_t boltPoint;
;1978:	char *sectc = ConcatArgs( 1 );
;1979:	
;1980:	if (sectc && sectc[0])
;1981:	{
;1982:		sect = atoi(sectc)+G2_MODELPART_HEAD;
;1983:	}
;1984:
;1985:	G_GetDismemberBolt(self, boltPoint, sect);
;1986:	G_Dismember( self, boltPoint, sect, 90, 0 );
;1987:}
;1988:*/
;1989:
;1990:int G_GetHitQuad( gentity_t *self, vec3_t hitloc )
;1991:{
line 1992
;1992:	vec3_t diff, fwdangles={0,0,0}, right;
ADDRLP4 36
ADDRGP4 $666
INDIRB
ASGNB 12
line 1996
;1993:	vec3_t clEye;
;1994:	float rightdot;
;1995:	float zdiff;
;1996:	int hitLoc = -1;
ADDRLP4 48
CNSTI4 -1
ASGNI4
line 1998
;1997:
;1998:	VectorCopy(self->client->ps.origin, clEye);
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
line 1999
;1999:	clEye[2] += self->client->ps.viewheight;
ADDRLP4 12+8
ADDRLP4 12+8
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
line 2001
;2000:
;2001:	VectorSubtract( hitloc, clEye, diff );
ADDRLP4 60
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 60
INDIRP4
INDIRF4
ADDRLP4 12
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 60
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRLP4 12+4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 0+8
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRLP4 12+8
INDIRF4
SUBF4
ASGNF4
line 2002
;2002:	diff[2] = 0;
ADDRLP4 0+8
CNSTF4 0
ASGNF4
line 2003
;2003:	VectorNormalize( diff );
ADDRLP4 0
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 2005
;2004:
;2005:	fwdangles[1] = self->client->ps.viewangles[1];
ADDRLP4 36+4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 160
ADDP4
INDIRF4
ASGNF4
line 2007
;2006:	// Ultimately we might care if the shot was ahead or behind, but for now, just quadrant is fine.
;2007:	AngleVectors( fwdangles, NULL, right, NULL );
ADDRLP4 36
ARGP4
ADDRLP4 64
CNSTP4 0
ASGNP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRLP4 24
ARGP4
ADDRLP4 64
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 2009
;2008:
;2009:	rightdot = DotProduct(right, diff);
ADDRLP4 56
ADDRLP4 24
INDIRF4
ADDRLP4 0
INDIRF4
MULF4
ADDRLP4 24+4
INDIRF4
ADDRLP4 0+4
INDIRF4
MULF4
ADDF4
ADDRLP4 24+8
INDIRF4
ADDRLP4 0+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 2010
;2010:	zdiff = hitloc[2] - clEye[2];
ADDRLP4 52
ADDRFP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRLP4 12+8
INDIRF4
SUBF4
ASGNF4
line 2012
;2011:	
;2012:	if ( zdiff > 0 )
ADDRLP4 52
INDIRF4
CNSTF4 0
LEF4 $679
line 2013
;2013:	{
line 2014
;2014:		if ( rightdot > 0.3 )
ADDRLP4 56
INDIRF4
CNSTF4 1050253722
LEF4 $681
line 2015
;2015:		{
line 2016
;2016:			hitLoc = G2_MODELPART_RARM;
ADDRLP4 48
CNSTI4 13
ASGNI4
line 2017
;2017:		}
ADDRGP4 $680
JUMPV
LABELV $681
line 2018
;2018:		else if ( rightdot < -0.3 )
ADDRLP4 56
INDIRF4
CNSTF4 3197737370
GEF4 $683
line 2019
;2019:		{
line 2020
;2020:			hitLoc = G2_MODELPART_LARM;
ADDRLP4 48
CNSTI4 12
ASGNI4
line 2021
;2021:		}
ADDRGP4 $680
JUMPV
LABELV $683
line 2023
;2022:		else
;2023:		{
line 2024
;2024:			hitLoc = G2_MODELPART_HEAD;
ADDRLP4 48
CNSTI4 10
ASGNI4
line 2025
;2025:		}
line 2026
;2026:	}
ADDRGP4 $680
JUMPV
LABELV $679
line 2027
;2027:	else if ( zdiff > -20 )
ADDRLP4 52
INDIRF4
CNSTF4 3248488448
LEF4 $685
line 2028
;2028:	{
line 2029
;2029:		if ( rightdot > 0.1 )
ADDRLP4 56
INDIRF4
CNSTF4 1036831949
LEF4 $687
line 2030
;2030:		{
line 2031
;2031:			hitLoc = G2_MODELPART_RARM;
ADDRLP4 48
CNSTI4 13
ASGNI4
line 2032
;2032:		}
ADDRGP4 $686
JUMPV
LABELV $687
line 2033
;2033:		else if ( rightdot < -0.1 )
ADDRLP4 56
INDIRF4
CNSTF4 3184315597
GEF4 $689
line 2034
;2034:		{
line 2035
;2035:			hitLoc = G2_MODELPART_LARM;
ADDRLP4 48
CNSTI4 12
ASGNI4
line 2036
;2036:		}
ADDRGP4 $686
JUMPV
LABELV $689
line 2038
;2037:		else
;2038:		{
line 2039
;2039:			hitLoc = G2_MODELPART_HEAD;
ADDRLP4 48
CNSTI4 10
ASGNI4
line 2040
;2040:		}
line 2041
;2041:	}
ADDRGP4 $686
JUMPV
LABELV $685
line 2043
;2042:	else
;2043:	{
line 2044
;2044:		if ( rightdot >= 0 )
ADDRLP4 56
INDIRF4
CNSTF4 0
LTF4 $691
line 2045
;2045:		{
line 2046
;2046:			hitLoc = G2_MODELPART_RLEG;
ADDRLP4 48
CNSTI4 15
ASGNI4
line 2047
;2047:		}
ADDRGP4 $692
JUMPV
LABELV $691
line 2049
;2048:		else
;2049:		{
line 2050
;2050:			hitLoc = G2_MODELPART_LLEG;
ADDRLP4 48
CNSTI4 14
ASGNI4
line 2051
;2051:		}
LABELV $692
line 2052
;2052:	}
LABELV $686
LABELV $680
line 2054
;2053:
;2054:	return hitLoc;
ADDRLP4 48
INDIRI4
RETI4
LABELV $665
endproc G_GetHitQuad 68 16
export G_CheckForDismemberment
proc G_CheckForDismemberment 40 24
line 2058
;2055:}
;2056:
;2057:void G_CheckForDismemberment(gentity_t *ent, vec3_t point, int damage, int deathAnim)
;2058:{
line 2059
;2059:	int hitLoc, hitLocUse = -1;
ADDRLP4 0
CNSTI4 -1
ASGNI4
line 2061
;2060:	vec3_t boltPoint;
;2061:	int dismember = g_dismember.integer;
ADDRLP4 4
ADDRGP4 g_dismember+12
INDIRI4
ASGNI4
line 2063
;2062:
;2063:	if (!dismember)
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $695
line 2064
;2064:	{
line 2065
;2065:		return;
ADDRGP4 $693
JUMPV
LABELV $695
line 2068
;2066:	}
;2067:
;2068:	if (Q_irand(0, 100) > dismember)
CNSTI4 0
ARGI4
CNSTI4 100
ARGI4
ADDRLP4 24
ADDRGP4 Q_irand
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
ADDRLP4 4
INDIRI4
LEI4 $697
line 2069
;2069:	{
line 2070
;2070:		return;
ADDRGP4 $693
JUMPV
LABELV $697
line 2073
;2071:	}
;2072:
;2073:	if (damage < 20)
ADDRFP4 8
INDIRI4
CNSTI4 20
GEI4 $699
line 2074
;2074:	{
line 2075
;2075:		return;
ADDRGP4 $693
JUMPV
LABELV $699
line 2078
;2076:	}
;2077:
;2078:	hitLoc = G_GetHitLocation( ent, point );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 28
ADDRGP4 G_GetHitLocation
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 28
INDIRI4
ASGNI4
line 2080
;2079:
;2080:	switch(hitLoc)
ADDRLP4 32
ADDRLP4 8
INDIRI4
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 1
LTI4 $701
ADDRLP4 32
INDIRI4
CNSTI4 16
GTI4 $701
ADDRLP4 32
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $709-4
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $709
address $703
address $704
address $703
address $704
address $705
address $701
address $701
address $701
address $701
address $701
address $701
address $706
address $707
address $706
address $707
address $708
code
line 2081
;2081:	{
LABELV $703
line 2084
;2082:	case HL_FOOT_RT:
;2083:	case HL_LEG_RT:
;2084:		hitLocUse = G2_MODELPART_RLEG;
ADDRLP4 0
CNSTI4 15
ASGNI4
line 2085
;2085:		break;
ADDRGP4 $702
JUMPV
LABELV $704
line 2088
;2086:	case HL_FOOT_LT:
;2087:	case HL_LEG_LT:
;2088:		hitLocUse = G2_MODELPART_LLEG;
ADDRLP4 0
CNSTI4 14
ASGNI4
LABELV $705
line 2091
;2089:		
;2090:	case HL_WAIST:
;2091:		hitLocUse = G2_MODELPART_WAIST;
ADDRLP4 0
CNSTI4 11
ASGNI4
line 2092
;2092:		break;
ADDRGP4 $702
JUMPV
LABELV $706
line 2104
;2093:		/*
;2094:	case HL_BACK_RT:
;2095:	case HL_BACK_LT:
;2096:	case HL_BACK:
;2097:	case HL_CHEST_RT:
;2098:	case HL_CHEST_LT:
;2099:	case HL_CHEST:
;2100:		break;
;2101:		*/
;2102:	case HL_ARM_RT:
;2103:	case HL_HAND_RT:
;2104:		hitLocUse = G2_MODELPART_RARM;
ADDRLP4 0
CNSTI4 13
ASGNI4
line 2105
;2105:		break;
ADDRGP4 $702
JUMPV
LABELV $707
line 2108
;2106:	case HL_ARM_LT:
;2107:	case HL_HAND_LT:
;2108:		hitLocUse = G2_MODELPART_LARM;
ADDRLP4 0
CNSTI4 12
ASGNI4
line 2109
;2109:		break;
ADDRGP4 $702
JUMPV
LABELV $708
line 2111
;2110:	case HL_HEAD:
;2111:		hitLocUse = G2_MODELPART_HEAD;
ADDRLP4 0
CNSTI4 10
ASGNI4
LABELV $701
line 2113
;2112:	default:
;2113:		hitLocUse = G_GetHitQuad(ent, point);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 36
ADDRGP4 G_GetHitQuad
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 36
INDIRI4
ASGNI4
line 2114
;2114:		break;
LABELV $702
line 2117
;2115:	}
;2116:
;2117:	if (hitLocUse == -1)
ADDRLP4 0
INDIRI4
CNSTI4 -1
NEI4 $711
line 2118
;2118:	{
line 2119
;2119:		return;
ADDRGP4 $693
JUMPV
LABELV $711
line 2122
;2120:	}
;2121:
;2122:	G_GetDismemberBolt(ent, boltPoint, hitLocUse);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRGP4 G_GetDismemberBolt
CALLV
pop
line 2123
;2123:	G_Dismember(ent, boltPoint, hitLocUse, 90, 0, deathAnim);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
CNSTF4 1119092736
ARGF4
CNSTF4 0
ARGF4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 G_Dismember
CALLV
pop
line 2124
;2124:}
LABELV $693
endproc G_CheckForDismemberment 40 24
export G_ThereIsAMaster
proc G_ThereIsAMaster 20 0
line 2127
;2125:
;2126:qboolean G_ThereIsAMaster(void)
;2127:{
line 2128
;2128:	int i = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $715
JUMPV
LABELV $714
line 2132
;2129:	gentity_t *ent;
;2130:
;2131:	while (i < MAX_CLIENTS)
;2132:	{
line 2133
;2133:		ent = &g_entities[i];
ADDRLP4 0
CNSTI4 828
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 2135
;2134:
;2135:		if (ent && ent->client && ent->client->ps.isJediMaster)
ADDRLP4 12
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 12
INDIRU4
EQU4 $717
ADDRLP4 16
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CVPU4 4
ADDRLP4 12
INDIRU4
EQU4 $717
ADDRLP4 16
INDIRP4
CNSTI4 604
ADDP4
INDIRI4
CNSTI4 0
EQI4 $717
line 2136
;2136:		{
line 2137
;2137:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $713
JUMPV
LABELV $717
line 2140
;2138:		}
;2139:
;2140:		i++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2141
;2141:	}
LABELV $715
line 2131
ADDRLP4 4
INDIRI4
CNSTI4 32
LTI4 $714
line 2143
;2142:
;2143:	return qfalse;
CNSTI4 0
RETI4
LABELV $713
endproc G_ThereIsAMaster 20 0
export G_Damage
proc G_Damage 212 32
line 2173
;2144:}
;2145:
;2146:/*
;2147:============
;2148:T_Damage
;2149:
;2150:targ		entity that is being damaged
;2151:inflictor	entity that is causing the damage
;2152:attacker	entity that caused the inflictor to damage targ
;2153:	example: targ=monster, inflictor=rocket, attacker=player
;2154:
;2155:dir			direction of the attack for knockback
;2156:point		point at which the damage is being inflicted, used for headshots
;2157:damage		amount of damage being inflicted
;2158:knockback	force to be applied against targ as a result of the damage
;2159:
;2160:inflictor, attacker, dir, and point can be NULL for environmental effects
;2161:
;2162:dflags		these flags are used to control how T_Damage works
;2163:	DAMAGE_RADIUS			damage was indirect (from a nearby explosion)
;2164:	DAMAGE_NO_ARMOR			armor does not protect from this damage
;2165:	DAMAGE_NO_KNOCKBACK		do not affect velocity, just view angles
;2166:	DAMAGE_NO_PROTECTION	kills godmode, armor, everything
;2167:	DAMAGE_HALF_ABSORB		half shields, half health
;2168:	DAMAGE_HALF_ARMOR_REDUCTION		Any damage that shields incur is halved
;2169:============
;2170:*/
;2171:
;2172:void G_Damage( gentity_t *targ, gentity_t *inflictor, gentity_t *attacker,
;2173:			   vec3_t dir, vec3_t point, int damage, int dflags, int mod ) {
line 2180
;2174:	gclient_t	*client;
;2175:	int			take;
;2176:	int			save;
;2177:	int			asave;
;2178:	int			knockback;
;2179:	int			max;
;2180:	int			subamt = 0;
ADDRLP4 20
CNSTI4 0
ASGNI4
line 2181
;2181:	float		famt = 0;
ADDRLP4 24
CNSTF4 0
ASGNF4
line 2182
;2182:	float		hamt = 0;
ADDRLP4 28
CNSTF4 0
ASGNF4
line 2183
;2183:	float		shieldAbsorbed = 0;
ADDRLP4 16
CNSTF4 0
ASGNF4
line 2185
;2184:
;2185:	if (targ && targ->damageRedirect)
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 40
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $720
ADDRLP4 40
INDIRP4
CNSTI4 816
ADDP4
INDIRI4
CNSTI4 0
EQI4 $720
line 2186
;2186:	{
line 2187
;2187:		G_Damage(&g_entities[targ->damageRedirectTo], inflictor, attacker, dir, point, damage, dflags, mod);
CNSTI4 828
ADDRFP4 0
INDIRP4
CNSTI4 820
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
ADDRFP4 16
INDIRP4
ARGP4
ADDRFP4 20
INDIRI4
ARGI4
ADDRFP4 24
INDIRI4
ARGI4
ADDRFP4 28
INDIRI4
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 2188
;2188:		return;
ADDRGP4 $719
JUMPV
LABELV $720
line 2191
;2189:	}
;2190:
;2191:	if (!targ->takedamage) {
ADDRFP4 0
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
CNSTI4 0
NEI4 $722
line 2192
;2192:		return;
ADDRGP4 $719
JUMPV
LABELV $722
line 2195
;2193:	}
;2194:
;2195:	if (targ && targ->client && targ->client->ps.duelInProgress)
ADDRLP4 44
ADDRFP4 0
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
EQU4 $724
ADDRLP4 52
ADDRLP4 44
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 52
INDIRP4
CVPU4 4
ADDRLP4 48
INDIRU4
EQU4 $724
ADDRLP4 52
INDIRP4
CNSTI4 1292
ADDP4
INDIRI4
CNSTI4 0
EQI4 $724
line 2196
;2196:	{
line 2197
;2197:		if (attacker && attacker->client && attacker->s.number != targ->client->ps.duelIndex)
ADDRLP4 56
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 60
CNSTU4 0
ASGNU4
ADDRLP4 56
INDIRP4
CVPU4 4
ADDRLP4 60
INDIRU4
EQU4 $726
ADDRLP4 64
CNSTI4 408
ASGNI4
ADDRLP4 56
INDIRP4
ADDRLP4 64
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 60
INDIRU4
EQU4 $726
ADDRLP4 56
INDIRP4
INDIRI4
ADDRFP4 0
INDIRP4
ADDRLP4 64
INDIRI4
ADDP4
INDIRP4
CNSTI4 1284
ADDP4
INDIRI4
EQI4 $726
line 2198
;2198:		{
line 2199
;2199:			return;
ADDRGP4 $719
JUMPV
LABELV $726
line 2201
;2200:		}
;2201:		else if (attacker && attacker->client && mod != MOD_SABER)
ADDRLP4 68
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 72
CNSTU4 0
ASGNU4
ADDRLP4 68
INDIRP4
CVPU4 4
ADDRLP4 72
INDIRU4
EQU4 $728
ADDRLP4 68
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 72
INDIRU4
EQU4 $728
ADDRFP4 28
INDIRI4
CNSTI4 3
EQI4 $728
line 2202
;2202:		{
line 2203
;2203:			return;
ADDRGP4 $719
JUMPV
LABELV $728
line 2205
;2204:		}
;2205:	}
LABELV $724
line 2206
;2206:	if (attacker && attacker->client && attacker->client->ps.duelInProgress)
ADDRLP4 56
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 60
CNSTU4 0
ASGNU4
ADDRLP4 56
INDIRP4
CVPU4 4
ADDRLP4 60
INDIRU4
EQU4 $730
ADDRLP4 64
ADDRLP4 56
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 64
INDIRP4
CVPU4 4
ADDRLP4 60
INDIRU4
EQU4 $730
ADDRLP4 64
INDIRP4
CNSTI4 1292
ADDP4
INDIRI4
CNSTI4 0
EQI4 $730
line 2207
;2207:	{
line 2208
;2208:		if (targ && targ->client && targ->s.number != attacker->client->ps.duelIndex)
ADDRLP4 68
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 72
CNSTU4 0
ASGNU4
ADDRLP4 68
INDIRP4
CVPU4 4
ADDRLP4 72
INDIRU4
EQU4 $732
ADDRLP4 76
CNSTI4 408
ASGNI4
ADDRLP4 68
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 72
INDIRU4
EQU4 $732
ADDRLP4 68
INDIRP4
INDIRI4
ADDRFP4 8
INDIRP4
ADDRLP4 76
INDIRI4
ADDP4
INDIRP4
CNSTI4 1284
ADDP4
INDIRI4
EQI4 $732
line 2209
;2209:		{
line 2210
;2210:			return;
ADDRGP4 $719
JUMPV
LABELV $732
line 2212
;2211:		}
;2212:		else if (targ && targ->client && mod != MOD_SABER)
ADDRLP4 80
ADDRFP4 0
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
EQU4 $734
ADDRLP4 80
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 84
INDIRU4
EQU4 $734
ADDRFP4 28
INDIRI4
CNSTI4 3
EQI4 $734
line 2213
;2213:		{
line 2214
;2214:			return;
ADDRGP4 $719
JUMPV
LABELV $734
line 2216
;2215:		}
;2216:	}
LABELV $730
line 2218
;2217:
;2218:	if (targ && targ->client && (targ->client->ps.fd.forcePowersActive & (1 << FP_RAGE)))
ADDRLP4 68
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 72
CNSTU4 0
ASGNU4
ADDRLP4 68
INDIRP4
CVPU4 4
ADDRLP4 72
INDIRU4
EQU4 $736
ADDRLP4 76
ADDRLP4 68
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 76
INDIRP4
CVPU4 4
ADDRLP4 72
INDIRU4
EQU4 $736
ADDRLP4 76
INDIRP4
CNSTI4 836
ADDP4
INDIRI4
CNSTI4 256
BANDI4
CNSTI4 0
EQI4 $736
line 2219
;2219:	{
line 2220
;2220:		damage *= 0.5;
ADDRFP4 20
CNSTF4 1056964608
ADDRFP4 20
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 2221
;2221:	}
LABELV $736
line 2225
;2222:
;2223:	// the intermission has allready been qualified for, so don't
;2224:	// allow any extra scoring
;2225:	if ( level.intermissionQueued ) {
ADDRGP4 level+9004
INDIRI4
CNSTI4 0
EQI4 $738
line 2226
;2226:		return;
ADDRGP4 $719
JUMPV
LABELV $738
line 2228
;2227:	}
;2228:	if ( !inflictor ) {
ADDRFP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $741
line 2229
;2229:		inflictor = &g_entities[ENTITYNUM_WORLD];
ADDRFP4 4
ADDRGP4 g_entities+846216
ASGNP4
line 2230
;2230:	}
LABELV $741
line 2231
;2231:	if ( !attacker ) {
ADDRFP4 8
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $744
line 2232
;2232:		attacker = &g_entities[ENTITYNUM_WORLD];
ADDRFP4 8
ADDRGP4 g_entities+846216
ASGNP4
line 2233
;2233:	}
LABELV $744
line 2238
;2234:
;2235:	// shootable doors / buttons don't actually have any health
;2236:
;2237:	//if boltpoint4 == 1 then it's glass or a breakable and those do have health
;2238:	if ( targ->s.eType == ET_MOVER && targ->boltpoint4 != 1 ) {
ADDRLP4 80
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 80
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
CNSTI4 6
NEI4 $747
ADDRLP4 80
INDIRP4
CNSTI4 780
ADDP4
INDIRI4
CNSTI4 1
EQI4 $747
line 2239
;2239:		if ( targ->use && targ->moverState == MOVER_POS1 ) {
ADDRLP4 84
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 84
INDIRP4
CNSTI4 652
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $719
ADDRLP4 84
INDIRP4
CNSTI4 512
ADDP4
INDIRI4
CNSTI4 0
NEI4 $719
line 2240
;2240:			targ->use( targ, inflictor, attacker );
ADDRLP4 88
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 88
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 88
INDIRP4
CNSTI4 652
ADDP4
INDIRP4
CALLV
pop
line 2241
;2241:		}
line 2242
;2242:		return;
ADDRGP4 $719
JUMPV
LABELV $747
line 2246
;2243:	}
;2244:	// reduce damage by the attacker's handicap value
;2245:	// unless they are rocket jumping
;2246:	if ( attacker->client && attacker != targ ) {
ADDRLP4 84
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 84
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $751
ADDRLP4 84
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
EQU4 $751
line 2247
;2247:		max = attacker->client->ps.stats[STAT_MAX_HEALTH];
ADDRLP4 36
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 248
ADDP4
INDIRI4
ASGNI4
line 2248
;2248:		damage = damage * max / 100;
ADDRFP4 20
ADDRFP4 20
INDIRI4
ADDRLP4 36
INDIRI4
MULI4
CNSTI4 100
DIVI4
ASGNI4
line 2249
;2249:	}
LABELV $751
line 2251
;2250:
;2251:	client = targ->client;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
line 2253
;2252:
;2253:	if ( client ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $753
line 2254
;2254:		if ( client->noclip ) {
ADDRLP4 0
INDIRP4
CNSTI4 1576
ADDP4
INDIRI4
CNSTI4 0
EQI4 $755
line 2255
;2255:			return;
ADDRGP4 $719
JUMPV
LABELV $755
line 2257
;2256:		}
;2257:	}
LABELV $753
line 2259
;2258:
;2259:	if ( !dir ) {
ADDRFP4 12
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $757
line 2260
;2260:		dflags |= DAMAGE_NO_KNOCKBACK;
ADDRFP4 24
ADDRFP4 24
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 2261
;2261:	} else {
ADDRGP4 $758
JUMPV
LABELV $757
line 2262
;2262:		VectorNormalize(dir);
ADDRFP4 12
INDIRP4
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 2263
;2263:	}
LABELV $758
line 2265
;2264:
;2265:	knockback = damage;
ADDRLP4 8
ADDRFP4 20
INDIRI4
ASGNI4
line 2266
;2266:	if ( knockback > 200 ) {
ADDRLP4 8
INDIRI4
CNSTI4 200
LEI4 $759
line 2267
;2267:		knockback = 200;
ADDRLP4 8
CNSTI4 200
ASGNI4
line 2268
;2268:	}
LABELV $759
line 2269
;2269:	if ( targ->flags & FL_NO_KNOCKBACK ) {
ADDRFP4 0
INDIRP4
CNSTI4 472
ADDP4
INDIRI4
CNSTI4 2048
BANDI4
CNSTI4 0
EQI4 $761
line 2270
;2270:		knockback = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 2271
;2271:	}
LABELV $761
line 2272
;2272:	if ( dflags & DAMAGE_NO_KNOCKBACK ) {
ADDRFP4 24
INDIRI4
CNSTI4 4
BANDI4
CNSTI4 0
EQI4 $763
line 2273
;2273:		knockback = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 2274
;2274:	}
LABELV $763
line 2276
;2275:
;2276:	if (targ && targ->client && targ->client->ps.usingATST)
ADDRLP4 88
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 92
CNSTU4 0
ASGNU4
ADDRLP4 88
INDIRP4
CVPU4 4
ADDRLP4 92
INDIRU4
EQU4 $765
ADDRLP4 96
ADDRLP4 88
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 96
INDIRP4
CVPU4 4
ADDRLP4 92
INDIRU4
EQU4 $765
ADDRLP4 96
INDIRP4
CNSTI4 1304
ADDP4
INDIRI4
CNSTI4 0
EQI4 $765
line 2277
;2277:	{
line 2278
;2278:		knockback = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 2279
;2279:	}
LABELV $765
line 2282
;2280:
;2281:	// figure momentum add, even if the damage won't be taken
;2282:	if ( knockback && targ->client ) {
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $767
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $767
line 2286
;2283:		vec3_t	kvel;
;2284:		float	mass;
;2285:
;2286:		mass = 200;
ADDRLP4 112
CNSTF4 1128792064
ASGNF4
line 2288
;2287:
;2288:		VectorScale (dir, g_knockback.value * (float)knockback / mass, kvel);
ADDRLP4 116
ADDRFP4 12
INDIRP4
ASGNP4
ADDRLP4 120
ADDRLP4 8
INDIRI4
CVIF4 4
ASGNF4
ADDRLP4 124
ADDRLP4 112
INDIRF4
ASGNF4
ADDRLP4 100
ADDRLP4 116
INDIRP4
INDIRF4
ADDRGP4 g_knockback+8
INDIRF4
ADDRLP4 120
INDIRF4
MULF4
ADDRLP4 124
INDIRF4
DIVF4
MULF4
ASGNF4
ADDRLP4 100+4
ADDRLP4 116
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
ADDRGP4 g_knockback+8
INDIRF4
ADDRLP4 120
INDIRF4
MULF4
ADDRLP4 124
INDIRF4
DIVF4
MULF4
ASGNF4
ADDRLP4 100+8
ADDRFP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
ADDRGP4 g_knockback+8
INDIRF4
ADDRLP4 8
INDIRI4
CVIF4 4
MULF4
ADDRLP4 112
INDIRF4
DIVF4
MULF4
ASGNF4
line 2289
;2289:		VectorAdd (targ->client->ps.velocity, kvel, targ->client->ps.velocity);
ADDRLP4 128
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 32
ADDP4
ASGNP4
ADDRLP4 128
INDIRP4
ADDRLP4 128
INDIRP4
INDIRF4
ADDRLP4 100
INDIRF4
ADDF4
ASGNF4
ADDRLP4 132
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 36
ADDP4
ASGNP4
ADDRLP4 132
INDIRP4
ADDRLP4 132
INDIRP4
INDIRF4
ADDRLP4 100+4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 136
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 40
ADDP4
ASGNP4
ADDRLP4 136
INDIRP4
ADDRLP4 136
INDIRP4
INDIRF4
ADDRLP4 100+8
INDIRF4
ADDF4
ASGNF4
line 2291
;2290:
;2291:		if (attacker && attacker->client && attacker != targ)
ADDRLP4 140
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 144
ADDRLP4 140
INDIRP4
CVPU4 4
ASGNU4
ADDRLP4 148
CNSTU4 0
ASGNU4
ADDRLP4 144
INDIRU4
ADDRLP4 148
INDIRU4
EQU4 $776
ADDRLP4 140
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 148
INDIRU4
EQU4 $776
ADDRLP4 144
INDIRU4
ADDRFP4 0
INDIRP4
CVPU4 4
EQU4 $776
line 2292
;2292:		{
line 2293
;2293:			targ->client->ps.otherKiller = attacker->s.number;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 748
ADDP4
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
line 2294
;2294:			targ->client->ps.otherKillerTime = level.time + 5000;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 752
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 5000
ADDI4
ASGNI4
line 2295
;2295:			targ->client->ps.otherKillerDebounceTime = level.time + 100;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 756
ADDP4
ADDRGP4 level+32
INDIRI4
CNSTI4 100
ADDI4
ASGNI4
line 2296
;2296:		}
LABELV $776
line 2299
;2297:		// set the timer so that the other client can't cancel
;2298:		// out the movement immediately
;2299:		if ( !targ->client->ps.pm_time ) {
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 16
ADDP4
INDIRI4
CNSTI4 0
NEI4 $780
line 2302
;2300:			int		t;
;2301:
;2302:			t = knockback * 2;
ADDRLP4 152
ADDRLP4 8
INDIRI4
CNSTI4 1
LSHI4
ASGNI4
line 2303
;2303:			if ( t < 50 ) {
ADDRLP4 152
INDIRI4
CNSTI4 50
GEI4 $782
line 2304
;2304:				t = 50;
ADDRLP4 152
CNSTI4 50
ASGNI4
line 2305
;2305:			}
LABELV $782
line 2306
;2306:			if ( t > 200 ) {
ADDRLP4 152
INDIRI4
CNSTI4 200
LEI4 $784
line 2307
;2307:				t = 200;
ADDRLP4 152
CNSTI4 200
ASGNI4
line 2308
;2308:			}
LABELV $784
line 2309
;2309:			targ->client->ps.pm_time = t;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 16
ADDP4
ADDRLP4 152
INDIRI4
ASGNI4
line 2310
;2310:			targ->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
ADDRLP4 156
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 12
ADDP4
ASGNP4
ADDRLP4 156
INDIRP4
ADDRLP4 156
INDIRP4
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 2311
;2311:		}
LABELV $780
line 2312
;2312:	}
LABELV $767
line 2315
;2313:
;2314:	// check for completely getting out of the damage
;2315:	if ( !(dflags & DAMAGE_NO_PROTECTION) ) {
ADDRFP4 24
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
NEI4 $786
line 2319
;2316:
;2317:		// if TF_NO_FRIENDLY_FIRE is set, don't do damage to the target
;2318:		// if the attacker was on the same team
;2319:		if ( targ != attacker && OnSameTeam (targ, attacker)  ) {
ADDRLP4 100
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 104
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 100
INDIRP4
CVPU4 4
ADDRLP4 104
INDIRP4
CVPU4 4
EQU4 $788
ADDRLP4 100
INDIRP4
ARGP4
ADDRLP4 104
INDIRP4
ARGP4
ADDRLP4 108
ADDRGP4 OnSameTeam
CALLI4
ASGNI4
ADDRLP4 108
INDIRI4
CNSTI4 0
EQI4 $788
line 2320
;2320:			if ( !g_friendlyFire.integer ) {
ADDRGP4 g_friendlyFire+12
INDIRI4
CNSTI4 0
NEI4 $790
line 2321
;2321:				return;
ADDRGP4 $719
JUMPV
LABELV $790
line 2323
;2322:			}
;2323:		}
LABELV $788
line 2325
;2324:
;2325:		if (g_gametype.integer == GT_JEDIMASTER && !g_friendlyFire.integer &&
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 2
NEI4 $793
ADDRLP4 112
CNSTI4 0
ASGNI4
ADDRGP4 g_friendlyFire+12
INDIRI4
ADDRLP4 112
INDIRI4
NEI4 $793
ADDRLP4 116
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 120
ADDRLP4 116
INDIRP4
CVPU4 4
ASGNU4
ADDRLP4 124
CNSTU4 0
ASGNU4
ADDRLP4 120
INDIRU4
ADDRLP4 124
INDIRU4
EQU4 $793
ADDRLP4 128
CNSTI4 408
ASGNI4
ADDRLP4 132
ADDRLP4 116
INDIRP4
ADDRLP4 128
INDIRI4
ADDP4
INDIRP4
ASGNP4
ADDRLP4 132
INDIRP4
CVPU4 4
ADDRLP4 124
INDIRU4
EQU4 $793
ADDRLP4 136
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 140
ADDRLP4 136
INDIRP4
CVPU4 4
ASGNU4
ADDRLP4 140
INDIRU4
ADDRLP4 124
INDIRU4
EQU4 $793
ADDRLP4 144
ADDRLP4 136
INDIRP4
ADDRLP4 128
INDIRI4
ADDP4
INDIRP4
ASGNP4
ADDRLP4 144
INDIRP4
CVPU4 4
ADDRLP4 124
INDIRU4
EQU4 $793
ADDRLP4 120
INDIRU4
ADDRLP4 140
INDIRU4
EQU4 $793
ADDRLP4 148
CNSTI4 604
ASGNI4
ADDRLP4 132
INDIRP4
ADDRLP4 148
INDIRI4
ADDP4
INDIRI4
ADDRLP4 112
INDIRI4
NEI4 $793
ADDRLP4 144
INDIRP4
ADDRLP4 148
INDIRI4
ADDP4
INDIRI4
ADDRLP4 112
INDIRI4
NEI4 $793
ADDRLP4 152
ADDRGP4 G_ThereIsAMaster
CALLI4
ASGNI4
ADDRLP4 152
INDIRI4
CNSTI4 0
EQI4 $793
line 2329
;2326:			targ && targ->client && attacker && attacker->client &&
;2327:			targ != attacker && !targ->client->ps.isJediMaster && !attacker->client->ps.isJediMaster &&
;2328:			G_ThereIsAMaster())
;2329:		{
line 2330
;2330:			return;
ADDRGP4 $719
JUMPV
LABELV $793
line 2333
;2331:		}
;2332:
;2333:		if (targ->client && targ->s.shouldtarget && targ->s.teamowner &&
ADDRLP4 156
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 160
CNSTI4 408
ASGNI4
ADDRLP4 164
CNSTU4 0
ASGNU4
ADDRLP4 156
INDIRP4
ADDRLP4 160
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 164
INDIRU4
EQU4 $797
ADDRLP4 168
CNSTI4 0
ASGNI4
ADDRLP4 156
INDIRP4
CNSTI4 268
ADDP4
INDIRI4
ADDRLP4 168
INDIRI4
EQI4 $797
ADDRLP4 156
INDIRP4
CNSTI4 264
ADDP4
INDIRI4
ADDRLP4 168
INDIRI4
EQI4 $797
ADDRLP4 172
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 172
INDIRP4
CVPU4 4
ADDRLP4 164
INDIRU4
EQU4 $797
ADDRLP4 172
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
ADDRLP4 168
INDIRI4
EQI4 $797
ADDRLP4 172
INDIRP4
ADDRLP4 160
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 164
INDIRU4
EQU4 $797
ADDRLP4 176
ADDRLP4 156
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
ASGNI4
ADDRLP4 176
INDIRI4
ADDRLP4 168
INDIRI4
LTI4 $797
ADDRLP4 176
INDIRI4
CNSTI4 32
GEI4 $797
line 2335
;2334:			attacker && attacker->inuse && attacker->client && targ->s.owner >= 0 && targ->s.owner < MAX_CLIENTS)
;2335:		{
line 2336
;2336:			gentity_t *targown = &g_entities[targ->s.owner];
ADDRLP4 180
CNSTI4 828
ADDRFP4 0
INDIRP4
CNSTI4 260
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 2338
;2337:
;2338:			if (targown && targown->inuse && targown->client && OnSameTeam(targown, attacker))
ADDRLP4 184
ADDRLP4 180
INDIRP4
ASGNP4
ADDRLP4 188
CNSTU4 0
ASGNU4
ADDRLP4 184
INDIRP4
CVPU4 4
ADDRLP4 188
INDIRU4
EQU4 $799
ADDRLP4 184
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $799
ADDRLP4 184
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 188
INDIRU4
EQU4 $799
ADDRLP4 184
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 192
ADDRGP4 OnSameTeam
CALLI4
ASGNI4
ADDRLP4 192
INDIRI4
CNSTI4 0
EQI4 $799
line 2339
;2339:			{
line 2340
;2340:				if (!g_friendlyFire.integer)
ADDRGP4 g_friendlyFire+12
INDIRI4
CNSTI4 0
NEI4 $801
line 2341
;2341:				{
line 2342
;2342:					return;
ADDRGP4 $719
JUMPV
LABELV $801
line 2344
;2343:				}
;2344:			}
LABELV $799
line 2345
;2345:		}
LABELV $797
line 2348
;2346:
;2347:		// check for godmode
;2348:		if ( targ->flags & FL_GODMODE ) {
ADDRFP4 0
INDIRP4
CNSTI4 472
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
EQI4 $804
line 2349
;2349:			return;
ADDRGP4 $719
JUMPV
LABELV $804
line 2352
;2350:		}
;2351:
;2352:		if (targ && targ->client && (targ->client->ps.eFlags & EF_INVULNERABLE) &&
ADDRLP4 180
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 184
ADDRLP4 180
INDIRP4
CVPU4 4
ASGNU4
ADDRLP4 188
CNSTU4 0
ASGNU4
ADDRLP4 184
INDIRU4
ADDRLP4 188
INDIRU4
EQU4 $806
ADDRLP4 192
CNSTI4 408
ASGNI4
ADDRLP4 196
ADDRLP4 180
INDIRP4
ADDRLP4 192
INDIRI4
ADDP4
INDIRP4
ASGNP4
ADDRLP4 196
INDIRP4
CVPU4 4
ADDRLP4 188
INDIRU4
EQU4 $806
ADDRLP4 196
INDIRP4
CNSTI4 108
ADDP4
INDIRI4
CNSTI4 67108864
BANDI4
CNSTI4 0
EQI4 $806
ADDRLP4 200
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 204
ADDRLP4 200
INDIRP4
CVPU4 4
ASGNU4
ADDRLP4 204
INDIRU4
ADDRLP4 188
INDIRU4
EQU4 $806
ADDRLP4 200
INDIRP4
ADDRLP4 192
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 188
INDIRU4
EQU4 $806
ADDRLP4 184
INDIRU4
ADDRLP4 204
INDIRU4
EQU4 $806
line 2354
;2353:			attacker && attacker->client && targ != attacker)
;2354:		{
line 2355
;2355:			if (targ->client->invulnerableTimer <= level.time)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1564
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
GTI4 $719
line 2356
;2356:			{
line 2357
;2357:				targ->client->ps.eFlags &= ~EF_INVULNERABLE;
ADDRLP4 208
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 108
ADDP4
ASGNP4
ADDRLP4 208
INDIRP4
ADDRLP4 208
INDIRP4
INDIRI4
CNSTI4 -67108865
BANDI4
ASGNI4
line 2358
;2358:			}
line 2360
;2359:			else
;2360:			{
line 2361
;2361:				return;
LABELV $809
line 2363
;2362:			}
;2363:		}
LABELV $806
line 2364
;2364:	}
LABELV $786
line 2366
;2365:
;2366:	if (attacker && attacker->client)
ADDRLP4 100
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 104
CNSTU4 0
ASGNU4
ADDRLP4 100
INDIRP4
CVPU4 4
ADDRLP4 104
INDIRU4
EQU4 $811
ADDRLP4 100
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 104
INDIRU4
EQU4 $811
line 2367
;2367:	{
line 2368
;2368:		if (targ->teamnodmg &&
ADDRLP4 108
ADDRFP4 0
INDIRP4
CNSTI4 424
ADDP4
INDIRI4
ASGNI4
ADDRLP4 112
CNSTI4 0
ASGNI4
ADDRLP4 108
INDIRI4
ADDRLP4 112
INDIRI4
EQI4 $813
ADDRLP4 108
INDIRI4
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
NEI4 $813
ADDRGP4 g_ff_objectives+12
INDIRI4
ADDRLP4 112
INDIRI4
NEI4 $813
line 2371
;2369:			targ->teamnodmg == attacker->client->sess.sessionTeam &&
;2370:			!g_ff_objectives.integer)
;2371:		{
line 2372
;2372:			return;
ADDRGP4 $719
JUMPV
LABELV $813
line 2374
;2373:		}
;2374:	}
LABELV $811
line 2378
;2375:
;2376:	// battlesuit protects from all radius damage (but takes knockback)
;2377:	// and protects 50% against all damage
;2378:	if ( client && client->ps.powerups[PW_BATTLESUIT] ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $816
ADDRLP4 0
INDIRP4
CNSTI4 352
ADDP4
INDIRI4
CNSTI4 0
EQI4 $816
line 2379
;2379:		G_AddEvent( targ, EV_POWERUP_BATTLESUIT, 0 );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 83
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 G_AddEvent
CALLV
pop
line 2380
;2380:		if ( ( dflags & DAMAGE_RADIUS ) || ( mod == MOD_FALLING ) ) {
ADDRFP4 24
INDIRI4
CNSTI4 1
BANDI4
CNSTI4 0
NEI4 $820
ADDRFP4 28
INDIRI4
CNSTI4 34
NEI4 $818
LABELV $820
line 2381
;2381:			return;
ADDRGP4 $719
JUMPV
LABELV $818
line 2383
;2382:		}
;2383:		damage *= 0.5;
ADDRFP4 20
CNSTF4 1056964608
ADDRFP4 20
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 2384
;2384:	}
LABELV $816
line 2387
;2385:
;2386:	// add to the attacker's hit counter (if the target isn't a general entity like a prox mine)
;2387:	if ( attacker->client && targ != attacker && targ->health > 0
ADDRLP4 112
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 116
CNSTU4 0
ASGNU4
ADDRLP4 112
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 116
INDIRU4
EQU4 $821
ADDRLP4 120
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 120
INDIRP4
CVPU4 4
ADDRLP4 112
INDIRP4
CVPU4 4
EQU4 $821
ADDRLP4 124
CNSTI4 0
ASGNI4
ADDRLP4 120
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
ADDRLP4 124
INDIRI4
LEI4 $821
ADDRLP4 128
ADDRLP4 120
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 128
INDIRI4
CNSTI4 3
EQI4 $821
ADDRLP4 128
INDIRI4
ADDRLP4 124
INDIRI4
EQI4 $821
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 116
INDIRU4
EQU4 $821
line 2390
;2388:			&& targ->s.eType != ET_MISSILE
;2389:			&& targ->s.eType != ET_GENERAL
;2390:			&& client) {
line 2391
;2391:		if ( OnSameTeam( targ, attacker ) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 132
ADDRGP4 OnSameTeam
CALLI4
ASGNI4
ADDRLP4 132
INDIRI4
CNSTI4 0
EQI4 $823
line 2392
;2392:			attacker->client->ps.persistant[PERS_HITS]--;
ADDRLP4 136
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 284
ADDP4
ASGNP4
ADDRLP4 136
INDIRP4
ADDRLP4 136
INDIRP4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 2393
;2393:		} else {
ADDRGP4 $824
JUMPV
LABELV $823
line 2394
;2394:			attacker->client->ps.persistant[PERS_HITS]++;
ADDRLP4 136
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 284
ADDP4
ASGNP4
ADDRLP4 136
INDIRP4
ADDRLP4 136
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 2395
;2395:		}
LABELV $824
line 2396
;2396:		attacker->client->ps.persistant[PERS_ATTACKEE_ARMOR] = (targ->health<<8)|(client->ps.stats[STAT_ARMOR]);
ADDRFP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 308
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 8
LSHI4
ADDRLP4 0
INDIRP4
CNSTI4 236
ADDP4
INDIRI4
BORI4
ASGNI4
line 2397
;2397:	}
LABELV $821
line 2401
;2398:
;2399:	// always give half damage if hurting self
;2400:	// calculated after knockback, so rocket jumping works
;2401:	if ( targ == attacker) {
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRFP4 8
INDIRP4
CVPU4 4
NEU4 $825
line 2402
;2402:		damage *= 0.5;
ADDRFP4 20
CNSTF4 1056964608
ADDRFP4 20
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 2403
;2403:	}
LABELV $825
line 2405
;2404:
;2405:	if ( damage < 1 ) {
ADDRFP4 20
INDIRI4
CNSTI4 1
GEI4 $827
line 2406
;2406:		damage = 1;
ADDRFP4 20
CNSTI4 1
ASGNI4
line 2407
;2407:	}
LABELV $827
line 2408
;2408:	take = damage;
ADDRLP4 4
ADDRFP4 20
INDIRI4
ASGNI4
line 2409
;2409:	save = 0;
ADDRLP4 32
CNSTI4 0
ASGNI4
line 2412
;2410:
;2411:	// save some from armor
;2412:	asave = CheckArmor (targ, take, dflags);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRFP4 24
INDIRI4
ARGI4
ADDRLP4 132
ADDRGP4 CheckArmor
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 132
INDIRI4
ASGNI4
line 2414
;2413:
;2414:	if (asave)
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $829
line 2415
;2415:	{
line 2416
;2416:		shieldAbsorbed = asave;
ADDRLP4 16
ADDRLP4 12
INDIRI4
CVIF4 4
ASGNF4
line 2417
;2417:	}
LABELV $829
line 2419
;2418:
;2419:	take -= asave;
ADDRLP4 4
ADDRLP4 4
INDIRI4
ADDRLP4 12
INDIRI4
SUBI4
ASGNI4
line 2421
;2420:
;2421:	if (mod == MOD_DEMP2 || mod == MOD_DEMP2_ALT)
ADDRLP4 136
ADDRFP4 28
INDIRI4
ASGNI4
ADDRLP4 136
INDIRI4
CNSTI4 14
EQI4 $833
ADDRLP4 136
INDIRI4
CNSTI4 15
NEI4 $831
LABELV $833
line 2422
;2422:	{ //demp2 does full damage to shields, but only 1/3 normal damage to health
line 2423
;2423:		if (take > 0)
ADDRLP4 4
INDIRI4
CNSTI4 0
LEI4 $834
line 2424
;2424:		{
line 2425
;2425:			take /= 3;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 3
DIVI4
ASGNI4
line 2426
;2426:			if (take < 1)
ADDRLP4 4
INDIRI4
CNSTI4 1
GEI4 $836
line 2427
;2427:			{
line 2428
;2428:				take = 1;
ADDRLP4 4
CNSTI4 1
ASGNI4
line 2429
;2429:			}
LABELV $836
line 2430
;2430:		}
LABELV $834
line 2431
;2431:	}
LABELV $831
line 2433
;2432:
;2433:	if ( g_debugDamage.integer ) {
ADDRGP4 g_debugDamage+12
INDIRI4
CNSTI4 0
EQI4 $838
line 2434
;2434:		G_Printf( "%i: client:%i health:%i damage:%i armor:%i\n", level.time, targ->s.number,
ADDRGP4 $841
ARGP4
ADDRGP4 level+32
INDIRI4
ARGI4
ADDRLP4 140
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 140
INDIRP4
INDIRI4
ARGI4
ADDRLP4 140
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 12
INDIRI4
ARGI4
ADDRGP4 G_Printf
CALLV
pop
line 2436
;2435:			targ->health, take, asave );
;2436:	}
LABELV $838
line 2441
;2437:
;2438:	// add to the damage inflicted on a player this frame
;2439:	// the total will be turned into screen blends and view angle kicks
;2440:	// at the end of the frame
;2441:	if ( client ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $843
line 2442
;2442:		if ( attacker ) {
ADDRFP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $845
line 2443
;2443:			client->ps.persistant[PERS_ATTACKER] = attacker->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 304
ADDP4
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
line 2444
;2444:		} else {
ADDRGP4 $846
JUMPV
LABELV $845
line 2445
;2445:			client->ps.persistant[PERS_ATTACKER] = ENTITYNUM_WORLD;
ADDRLP4 0
INDIRP4
CNSTI4 304
ADDP4
CNSTI4 1022
ASGNI4
line 2446
;2446:		}
LABELV $846
line 2447
;2447:		client->damage_armor += asave;
ADDRLP4 140
ADDRLP4 0
INDIRP4
CNSTI4 1608
ADDP4
ASGNP4
ADDRLP4 140
INDIRP4
ADDRLP4 140
INDIRP4
INDIRI4
ADDRLP4 12
INDIRI4
ADDI4
ASGNI4
line 2448
;2448:		client->damage_blood += take;
ADDRLP4 144
ADDRLP4 0
INDIRP4
CNSTI4 1612
ADDP4
ASGNP4
ADDRLP4 144
INDIRP4
ADDRLP4 144
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ASGNI4
line 2449
;2449:		client->damage_knockback += knockback;
ADDRLP4 148
ADDRLP4 0
INDIRP4
CNSTI4 1616
ADDP4
ASGNP4
ADDRLP4 148
INDIRP4
ADDRLP4 148
INDIRP4
INDIRI4
ADDRLP4 8
INDIRI4
ADDI4
ASGNI4
line 2450
;2450:		if ( dir ) {
ADDRFP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $847
line 2451
;2451:			VectorCopy ( dir, client->damage_from );
ADDRLP4 0
INDIRP4
CNSTI4 1620
ADDP4
ADDRFP4 12
INDIRP4
INDIRB
ASGNB 12
line 2452
;2452:			client->damage_fromWorld = qfalse;
ADDRLP4 0
INDIRP4
CNSTI4 1632
ADDP4
CNSTI4 0
ASGNI4
line 2453
;2453:		} else {
ADDRGP4 $848
JUMPV
LABELV $847
line 2454
;2454:			VectorCopy ( targ->r.currentOrigin, client->damage_from );
ADDRLP4 0
INDIRP4
CNSTI4 1620
ADDP4
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
INDIRB
ASGNB 12
line 2455
;2455:			client->damage_fromWorld = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 1632
ADDP4
CNSTI4 1
ASGNI4
line 2456
;2456:		}
LABELV $848
line 2458
;2457:
;2458:		if (attacker && attacker->client)
ADDRLP4 152
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 156
CNSTU4 0
ASGNU4
ADDRLP4 152
INDIRP4
CVPU4 4
ADDRLP4 156
INDIRU4
EQU4 $849
ADDRLP4 152
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 156
INDIRU4
EQU4 $849
line 2459
;2459:		{
line 2460
;2460:			BotDamageNotification(client, attacker);
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 BotDamageNotification
CALLV
pop
line 2461
;2461:		}
ADDRGP4 $850
JUMPV
LABELV $849
line 2462
;2462:		else if (inflictor && inflictor->client)
ADDRLP4 160
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 164
CNSTU4 0
ASGNU4
ADDRLP4 160
INDIRP4
CVPU4 4
ADDRLP4 164
INDIRU4
EQU4 $851
ADDRLP4 160
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 164
INDIRU4
EQU4 $851
line 2463
;2463:		{
line 2464
;2464:			BotDamageNotification(client, inflictor);
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 BotDamageNotification
CALLV
pop
line 2465
;2465:		}
LABELV $851
LABELV $850
line 2466
;2466:	}
LABELV $843
line 2469
;2467:
;2468:	// See if it's the player hurting the emeny flag carrier
;2469:	if( g_gametype.integer == GT_CTF || g_gametype.integer == GT_CTY) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 7
EQI4 $857
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 8
NEI4 $853
LABELV $857
line 2470
;2470:		Team_CheckHurtCarrier(targ, attacker);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Team_CheckHurtCarrier
CALLV
pop
line 2471
;2471:	}
LABELV $853
line 2473
;2472:
;2473:	if (targ->client) {
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $858
line 2475
;2474:		// set the last client who damaged the target
;2475:		targ->client->lasthurt_client = attacker->s.number;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1664
ADDP4
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
line 2476
;2476:		targ->client->lasthurt_mod = mod;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1668
ADDP4
ADDRFP4 28
INDIRI4
ASGNI4
line 2477
;2477:	}
LABELV $858
line 2479
;2478:
;2479:	if (take && targ->client && (targ->client->ps.fd.forcePowersActive & (1 << FP_PROTECT)))
ADDRLP4 140
CNSTI4 0
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 140
INDIRI4
EQI4 $860
ADDRLP4 144
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 144
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $860
ADDRLP4 144
INDIRP4
CNSTI4 836
ADDP4
INDIRI4
CNSTI4 512
BANDI4
ADDRLP4 140
INDIRI4
EQI4 $860
line 2480
;2480:	{
line 2481
;2481:		if (targ->client->ps.fd.forcePower)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 920
ADDP4
INDIRI4
CNSTI4 0
EQI4 $862
line 2482
;2482:		{
line 2483
;2483:			int maxtake = take;
ADDRLP4 148
ADDRLP4 4
INDIRI4
ASGNI4
line 2486
;2484:
;2485:			//G_Sound(targ, CHAN_AUTO, protectHitSound);
;2486:			G_PreDefSound(targ->client->ps.origin, PDSOUND_PROTECTHIT);
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
CNSTI4 1
ARGI4
ADDRGP4 G_PreDefSound
CALLV
pop
line 2488
;2487:
;2488:			if (targ->client->ps.fd.forcePowerLevel[FP_PROTECT] == FORCE_LEVEL_1)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 968
ADDP4
INDIRI4
CNSTI4 1
NEI4 $864
line 2489
;2489:			{
line 2490
;2490:				famt = 1;
ADDRLP4 24
CNSTF4 1065353216
ASGNF4
line 2491
;2491:				hamt = 0.40;
ADDRLP4 28
CNSTF4 1053609165
ASGNF4
line 2493
;2492:
;2493:				if (maxtake > 100)
ADDRLP4 148
INDIRI4
CNSTI4 100
LEI4 $865
line 2494
;2494:				{
line 2495
;2495:					maxtake = 100;
ADDRLP4 148
CNSTI4 100
ASGNI4
line 2496
;2496:				}
line 2497
;2497:			}
ADDRGP4 $865
JUMPV
LABELV $864
line 2498
;2498:			else if (targ->client->ps.fd.forcePowerLevel[FP_PROTECT] == FORCE_LEVEL_2)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 968
ADDP4
INDIRI4
CNSTI4 2
NEI4 $868
line 2499
;2499:			{
line 2500
;2500:				famt = 0.5;
ADDRLP4 24
CNSTF4 1056964608
ASGNF4
line 2501
;2501:				hamt = 0.60;
ADDRLP4 28
CNSTF4 1058642330
ASGNF4
line 2503
;2502:
;2503:				if (maxtake > 200)
ADDRLP4 148
INDIRI4
CNSTI4 200
LEI4 $869
line 2504
;2504:				{
line 2505
;2505:					maxtake = 200;
ADDRLP4 148
CNSTI4 200
ASGNI4
line 2506
;2506:				}
line 2507
;2507:			}
ADDRGP4 $869
JUMPV
LABELV $868
line 2508
;2508:			else if (targ->client->ps.fd.forcePowerLevel[FP_PROTECT] == FORCE_LEVEL_3)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 968
ADDP4
INDIRI4
CNSTI4 3
NEI4 $872
line 2509
;2509:			{
line 2510
;2510:				famt = 0.25;
ADDRLP4 24
CNSTF4 1048576000
ASGNF4
line 2511
;2511:				hamt = 0.80;
ADDRLP4 28
CNSTF4 1061997773
ASGNF4
line 2513
;2512:
;2513:				if (maxtake > 400)
ADDRLP4 148
INDIRI4
CNSTI4 400
LEI4 $874
line 2514
;2514:				{
line 2515
;2515:					maxtake = 400;
ADDRLP4 148
CNSTI4 400
ASGNI4
line 2516
;2516:				}
LABELV $874
line 2517
;2517:			}
LABELV $872
LABELV $869
LABELV $865
line 2519
;2518:
;2519:			if (!targ->client->ps.powerups[PW_FORCE_BOON])
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 400
ADDP4
INDIRI4
CNSTI4 0
NEI4 $876
line 2520
;2520:			{
line 2521
;2521:				targ->client->ps.fd.forcePower -= maxtake*famt;
ADDRLP4 152
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 920
ADDP4
ASGNP4
ADDRLP4 152
INDIRP4
ADDRLP4 152
INDIRP4
INDIRI4
CVIF4 4
ADDRLP4 148
INDIRI4
CVIF4 4
ADDRLP4 24
INDIRF4
MULF4
SUBF4
CVFI4 4
ASGNI4
line 2522
;2522:			}
ADDRGP4 $877
JUMPV
LABELV $876
line 2524
;2523:			else
;2524:			{
line 2525
;2525:				targ->client->ps.fd.forcePower -= (maxtake*famt)/2;
ADDRLP4 152
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 920
ADDP4
ASGNP4
ADDRLP4 152
INDIRP4
ADDRLP4 152
INDIRP4
INDIRI4
CVIF4 4
ADDRLP4 148
INDIRI4
CVIF4 4
ADDRLP4 24
INDIRF4
MULF4
CNSTF4 1073741824
DIVF4
SUBF4
CVFI4 4
ASGNI4
line 2526
;2526:			}
LABELV $877
line 2527
;2527:			subamt = (maxtake*hamt)+(take-maxtake);
ADDRLP4 152
ADDRLP4 148
INDIRI4
ASGNI4
ADDRLP4 20
ADDRLP4 152
INDIRI4
CVIF4 4
ADDRLP4 28
INDIRF4
MULF4
ADDRLP4 4
INDIRI4
ADDRLP4 152
INDIRI4
SUBI4
CVIF4 4
ADDF4
CVFI4 4
ASGNI4
line 2528
;2528:			if (targ->client->ps.fd.forcePower < 0)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 920
ADDP4
INDIRI4
CNSTI4 0
GEI4 $878
line 2529
;2529:			{
line 2530
;2530:				subamt += targ->client->ps.fd.forcePower;
ADDRLP4 20
ADDRLP4 20
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 920
ADDP4
INDIRI4
ADDI4
ASGNI4
line 2531
;2531:				targ->client->ps.fd.forcePower = 0;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 920
ADDP4
CNSTI4 0
ASGNI4
line 2532
;2532:			}
LABELV $878
line 2533
;2533:			if (subamt)
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $880
line 2534
;2534:			{
line 2535
;2535:				take -= subamt;
ADDRLP4 4
ADDRLP4 4
INDIRI4
ADDRLP4 20
INDIRI4
SUBI4
ASGNI4
line 2537
;2536:
;2537:				if (take < 0)
ADDRLP4 4
INDIRI4
CNSTI4 0
GEI4 $882
line 2538
;2538:				{
line 2539
;2539:					take = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 2540
;2540:				}
LABELV $882
line 2541
;2541:			}
LABELV $880
line 2542
;2542:		}
LABELV $862
line 2543
;2543:	}
LABELV $860
line 2545
;2544:
;2545:	if (shieldAbsorbed)
ADDRLP4 16
INDIRF4
CNSTF4 0
EQF4 $884
line 2546
;2546:	{
line 2550
;2547:		gentity_t	*evEnt;
;2548:
;2549:		// Send off an event to show a shield shell on the player, pointing in the right direction.
;2550:		evEnt = G_TempEntity(vec3_origin, EV_SHIELD_HIT);
ADDRGP4 vec3_origin
ARGP4
CNSTI4 97
ARGI4
ADDRLP4 152
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 148
ADDRLP4 152
INDIRP4
ASGNP4
line 2551
;2551:		evEnt->s.otherEntityNum = targ->s.number;
ADDRLP4 148
INDIRP4
CNSTI4 184
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 2552
;2552:		evEnt->s.eventParm = DirToByte(dir);
ADDRFP4 12
INDIRP4
ARGP4
ADDRLP4 156
ADDRGP4 DirToByte
CALLI4
ASGNI4
ADDRLP4 148
INDIRP4
CNSTI4 256
ADDP4
ADDRLP4 156
INDIRI4
ASGNI4
line 2553
;2553:		evEnt->s.time2=shieldAbsorbed;
ADDRLP4 148
INDIRP4
CNSTI4 88
ADDP4
ADDRLP4 16
INDIRF4
CVFI4 4
ASGNI4
line 2575
;2554:/*
;2555:		shieldAbsorbed *= 20;
;2556:
;2557:		if (shieldAbsorbed > 1500)
;2558:		{
;2559:			shieldAbsorbed = 1500;
;2560:		}
;2561:		if (shieldAbsorbed < 200)
;2562:		{
;2563:			shieldAbsorbed = 200;
;2564:		}
;2565:
;2566:		if (targ->client->ps.powerups[PW_SHIELDHIT] < (level.time + shieldAbsorbed))
;2567:		{
;2568:			targ->client->ps.powerups[PW_SHIELDHIT] = level.time + shieldAbsorbed;
;2569:		}
;2570:		//flicker for as many ms as damage was absorbed (*20)
;2571:		//therefore 10 damage causes 1/5 of a seond of flickering, whereas
;2572:		//a full 100 causes 2 seconds (but is reduced to 1.5 seconds due to the max)
;2573:
;2574:*/
;2575:	}
LABELV $884
line 2578
;2576:
;2577:	// do the damage
;2578:	if (take) {
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $886
line 2579
;2579:		if (targ->client && (targ->client->ps.fd.forcePowersActive & (1 << FP_RAGE)) && (inflictor->client || attacker->client))
ADDRLP4 148
CNSTI4 408
ASGNI4
ADDRLP4 152
ADDRFP4 0
INDIRP4
ADDRLP4 148
INDIRI4
ADDP4
INDIRP4
ASGNP4
ADDRLP4 156
CNSTU4 0
ASGNU4
ADDRLP4 152
INDIRP4
CVPU4 4
ADDRLP4 156
INDIRU4
EQU4 $888
ADDRLP4 152
INDIRP4
CNSTI4 836
ADDP4
INDIRI4
CNSTI4 256
BANDI4
CNSTI4 0
EQI4 $888
ADDRFP4 4
INDIRP4
ADDRLP4 148
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 156
INDIRU4
NEU4 $890
ADDRFP4 8
INDIRP4
ADDRLP4 148
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 156
INDIRU4
EQU4 $888
LABELV $890
line 2580
;2580:		{
line 2581
;2581:			take /= (targ->client->ps.fd.forcePowerLevel[FP_RAGE]+1);
ADDRLP4 4
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 964
ADDP4
INDIRI4
CNSTI4 1
ADDI4
DIVI4
ASGNI4
line 2582
;2582:		}
LABELV $888
line 2583
;2583:		targ->health = targ->health - take;
ADDRLP4 160
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
ASGNP4
ADDRLP4 160
INDIRP4
ADDRLP4 160
INDIRP4
INDIRI4
ADDRLP4 4
INDIRI4
SUBI4
ASGNI4
line 2584
;2584:		if ( targ->client ) {
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $891
line 2585
;2585:			targ->client->ps.stats[STAT_HEALTH] = targ->health;
ADDRLP4 164
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 164
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 216
ADDP4
ADDRLP4 164
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
ASGNI4
line 2586
;2586:		}
LABELV $891
line 2588
;2587:
;2588:		if (targ->client && (targ->client->ps.fd.forcePowersActive & (1 << FP_RAGE)) && (inflictor->client || attacker->client))
ADDRLP4 164
CNSTI4 408
ASGNI4
ADDRLP4 168
ADDRFP4 0
INDIRP4
ADDRLP4 164
INDIRI4
ADDP4
INDIRP4
ASGNP4
ADDRLP4 172
CNSTU4 0
ASGNU4
ADDRLP4 168
INDIRP4
CVPU4 4
ADDRLP4 172
INDIRU4
EQU4 $893
ADDRLP4 168
INDIRP4
CNSTI4 836
ADDP4
INDIRI4
CNSTI4 256
BANDI4
CNSTI4 0
EQI4 $893
ADDRFP4 4
INDIRP4
ADDRLP4 164
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 172
INDIRU4
NEU4 $895
ADDRFP4 8
INDIRP4
ADDRLP4 164
INDIRI4
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 172
INDIRU4
EQU4 $893
LABELV $895
line 2589
;2589:		{
line 2590
;2590:			if (targ->health <= 0)
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 0
GTI4 $896
line 2591
;2591:			{
line 2592
;2592:				targ->health = 1;
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 1
ASGNI4
line 2593
;2593:			}
LABELV $896
line 2594
;2594:			if (targ->client->ps.stats[STAT_HEALTH] <= 0)
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 216
ADDP4
INDIRI4
CNSTI4 0
GTI4 $898
line 2595
;2595:			{
line 2596
;2596:				targ->client->ps.stats[STAT_HEALTH] = 1;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 216
ADDP4
CNSTI4 1
ASGNI4
line 2597
;2597:			}
LABELV $898
line 2598
;2598:		}
LABELV $893
line 2600
;2599:	
;2600:		if ( targ->health <= 0 ) {
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 0
GTI4 $900
line 2601
;2601:			if ( client )
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $902
line 2602
;2602:			{
line 2603
;2603:				targ->flags |= FL_NO_KNOCKBACK;
ADDRLP4 176
ADDRFP4 0
INDIRP4
CNSTI4 472
ADDP4
ASGNP4
ADDRLP4 176
INDIRP4
ADDRLP4 176
INDIRP4
INDIRI4
CNSTI4 2048
BORI4
ASGNI4
line 2605
;2604:
;2605:				if (point)
ADDRFP4 16
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $904
line 2606
;2606:				{
line 2607
;2607:					VectorCopy( point, targ->pos1 );
ADDRFP4 0
INDIRP4
CNSTI4 548
ADDP4
ADDRFP4 16
INDIRP4
INDIRB
ASGNB 12
line 2608
;2608:				}
ADDRGP4 $905
JUMPV
LABELV $904
line 2610
;2609:				else
;2610:				{
line 2611
;2611:					VectorCopy(targ->client->ps.origin, targ->pos1);
ADDRLP4 180
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 180
INDIRP4
CNSTI4 548
ADDP4
ADDRLP4 180
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 2612
;2612:				}
LABELV $905
line 2613
;2613:			}
LABELV $902
line 2615
;2614:
;2615:			if (targ->health < -999)
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
INDIRI4
CNSTI4 -999
GEI4 $906
line 2616
;2616:				targ->health = -999;
ADDRFP4 0
INDIRP4
CNSTI4 676
ADDP4
CNSTI4 -999
ASGNI4
LABELV $906
line 2619
;2617:
;2618:			// If we are a breaking glass brush, store the damage point so we can do cool things with it.
;2619:			if ( targ->r.svFlags & SVF_GLASS_BRUSH )
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
INDIRI4
CNSTI4 134217728
BANDI4
CNSTI4 0
EQI4 $908
line 2620
;2620:			{
line 2621
;2621:				VectorCopy( point, targ->pos1 );
ADDRFP4 0
INDIRP4
CNSTI4 548
ADDP4
ADDRFP4 16
INDIRP4
INDIRB
ASGNB 12
line 2622
;2622:				VectorCopy( dir, targ->pos2 );
ADDRFP4 0
INDIRP4
CNSTI4 560
ADDP4
ADDRFP4 12
INDIRP4
INDIRB
ASGNB 12
line 2623
;2623:			}
LABELV $908
line 2625
;2624:
;2625:			targ->enemy = attacker;
ADDRFP4 0
INDIRP4
CNSTI4 728
ADDP4
ADDRFP4 8
INDIRP4
ASGNP4
line 2626
;2626:			targ->die (targ, inflictor, attacker, take, mod);
ADDRLP4 176
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 176
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRFP4 28
INDIRI4
ARGI4
ADDRLP4 176
INDIRP4
CNSTI4 660
ADDP4
INDIRP4
CALLV
pop
line 2627
;2627:			return;
ADDRGP4 $719
JUMPV
LABELV $900
line 2628
;2628:		} else if ( targ->pain ) {
ADDRFP4 0
INDIRP4
CNSTI4 656
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $910
line 2629
;2629:			targ->pain (targ, attacker, take);
ADDRLP4 176
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 176
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 176
INDIRP4
CNSTI4 656
ADDP4
INDIRP4
CALLV
pop
line 2630
;2630:		}
LABELV $910
line 2632
;2631:
;2632:		G_LogWeaponDamage(attacker->s.number, mod, take);
ADDRFP4 8
INDIRP4
INDIRI4
ARGI4
ADDRFP4 28
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 G_LogWeaponDamage
CALLV
pop
line 2633
;2633:	}
LABELV $886
line 2635
;2634:
;2635:}
LABELV $719
endproc G_Damage 212 32
export CanDamage
proc CanDamage 1136 28
line 2646
;2636:
;2637:
;2638:/*
;2639:============
;2640:CanDamage
;2641:
;2642:Returns qtrue if the inflictor can directly damage the target.  Used for
;2643:explosions and melee attacks.
;2644:============
;2645:*/
;2646:qboolean CanDamage (gentity_t *targ, vec3_t origin) {
line 2653
;2647:	vec3_t	dest;
;2648:	trace_t	tr;
;2649:	vec3_t	midpoint;
;2650:
;2651:	// use the midpoint of the bounds instead of the origin, because
;2652:	// bmodels may have their origin is 0,0,0
;2653:	VectorAdd (targ->r.absmin, targ->r.absmax, midpoint);
ADDRLP4 1104
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
ADDRLP4 1104
INDIRP4
CNSTI4 344
ADDP4
INDIRF4
ADDRLP4 1104
INDIRP4
CNSTI4 356
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 1104
INDIRP4
CNSTI4 348
ADDP4
INDIRF4
ADDRLP4 1104
INDIRP4
CNSTI4 360
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 1108
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12+8
ADDRLP4 1108
INDIRP4
CNSTI4 352
ADDP4
INDIRF4
ADDRLP4 1108
INDIRP4
CNSTI4 364
ADDP4
INDIRF4
ADDF4
ASGNF4
line 2654
;2654:	VectorScale (midpoint, 0.5, midpoint);
ADDRLP4 1112
CNSTF4 1056964608
ASGNF4
ADDRLP4 12
ADDRLP4 1112
INDIRF4
ADDRLP4 12
INDIRF4
MULF4
ASGNF4
ADDRLP4 12+4
ADDRLP4 1112
INDIRF4
ADDRLP4 12+4
INDIRF4
MULF4
ASGNF4
ADDRLP4 12+8
CNSTF4 1056964608
ADDRLP4 12+8
INDIRF4
MULF4
ASGNF4
line 2656
;2655:
;2656:	VectorCopy (midpoint, dest);
ADDRLP4 0
ADDRLP4 12
INDIRB
ASGNB 12
line 2657
;2657:	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 1116
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 1116
INDIRP4
ARGP4
ADDRLP4 1116
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1023
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 2658
;2658:	if (tr.fraction == 1.0 || tr.entityNum == targ->s.number)
ADDRLP4 24+8
INDIRF4
CNSTF4 1065353216
EQF4 $923
ADDRLP4 24+52
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $919
LABELV $923
line 2659
;2659:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $912
JUMPV
LABELV $919
line 2663
;2660:
;2661:	// this should probably check in the plane of projection, 
;2662:	// rather than in world coordinate, and also include Z
;2663:	VectorCopy (midpoint, dest);
ADDRLP4 0
ADDRLP4 12
INDIRB
ASGNB 12
line 2664
;2664:	dest[0] += 15.0;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1097859072
ADDF4
ASGNF4
line 2665
;2665:	dest[1] += 15.0;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1097859072
ADDF4
ASGNF4
line 2666
;2666:	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 1120
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 1120
INDIRP4
ARGP4
ADDRLP4 1120
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1023
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 2667
;2667:	if (tr.fraction == 1.0)
ADDRLP4 24+8
INDIRF4
CNSTF4 1065353216
NEF4 $925
line 2668
;2668:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $912
JUMPV
LABELV $925
line 2670
;2669:
;2670:	VectorCopy (midpoint, dest);
ADDRLP4 0
ADDRLP4 12
INDIRB
ASGNB 12
line 2671
;2671:	dest[0] += 15.0;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1097859072
ADDF4
ASGNF4
line 2672
;2672:	dest[1] -= 15.0;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1097859072
SUBF4
ASGNF4
line 2673
;2673:	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 1124
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 1124
INDIRP4
ARGP4
ADDRLP4 1124
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1023
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 2674
;2674:	if (tr.fraction == 1.0)
ADDRLP4 24+8
INDIRF4
CNSTF4 1065353216
NEF4 $929
line 2675
;2675:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $912
JUMPV
LABELV $929
line 2677
;2676:
;2677:	VectorCopy (midpoint, dest);
ADDRLP4 0
ADDRLP4 12
INDIRB
ASGNB 12
line 2678
;2678:	dest[0] -= 15.0;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1097859072
SUBF4
ASGNF4
line 2679
;2679:	dest[1] += 15.0;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1097859072
ADDF4
ASGNF4
line 2680
;2680:	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 1128
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 1128
INDIRP4
ARGP4
ADDRLP4 1128
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1023
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 2681
;2681:	if (tr.fraction == 1.0)
ADDRLP4 24+8
INDIRF4
CNSTF4 1065353216
NEF4 $933
line 2682
;2682:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $912
JUMPV
LABELV $933
line 2684
;2683:
;2684:	VectorCopy (midpoint, dest);
ADDRLP4 0
ADDRLP4 12
INDIRB
ASGNB 12
line 2685
;2685:	dest[0] -= 15.0;
ADDRLP4 0
ADDRLP4 0
INDIRF4
CNSTF4 1097859072
SUBF4
ASGNF4
line 2686
;2686:	dest[1] -= 15.0;
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
CNSTF4 1097859072
SUBF4
ASGNF4
line 2687
;2687:	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
ADDRLP4 24
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 1132
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 1132
INDIRP4
ARGP4
ADDRLP4 1132
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 1023
ARGI4
CNSTI4 1
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 2688
;2688:	if (tr.fraction == 1.0)
ADDRLP4 24+8
INDIRF4
CNSTF4 1065353216
NEF4 $937
line 2689
;2689:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $912
JUMPV
LABELV $937
line 2692
;2690:
;2691:
;2692:	return qfalse;
CNSTI4 0
RETI4
LABELV $912
endproc CanDamage 1136 28
export G_RadiusDamage
proc G_RadiusDamage 4196 32
line 2702
;2693:}
;2694:
;2695:
;2696:/*
;2697:============
;2698:G_RadiusDamage
;2699:============
;2700:*/
;2701:qboolean G_RadiusDamage ( vec3_t origin, gentity_t *attacker, float damage, float radius,
;2702:					 gentity_t *ignore, int mod) {
line 2711
;2703:	float		points, dist;
;2704:	gentity_t	*ent;
;2705:	int			entityList[MAX_GENTITIES];
;2706:	int			numListedEntities;
;2707:	vec3_t		mins, maxs;
;2708:	vec3_t		v;
;2709:	vec3_t		dir;
;2710:	int			i, e;
;2711:	qboolean	hitClient = qfalse;
ADDRLP4 4168
CNSTI4 0
ASGNI4
line 2713
;2712:
;2713:	if ( radius < 1 ) {
ADDRFP4 12
INDIRF4
CNSTF4 1065353216
GEF4 $941
line 2714
;2714:		radius = 1;
ADDRFP4 12
CNSTF4 1065353216
ASGNF4
line 2715
;2715:	}
LABELV $941
line 2717
;2716:
;2717:	for ( i = 0 ; i < 3 ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $943
line 2718
;2718:		mins[i] = origin[i] - radius;
ADDRLP4 4172
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4172
INDIRI4
ADDRLP4 4144
ADDP4
ADDRLP4 4172
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRFP4 12
INDIRF4
SUBF4
ASGNF4
line 2719
;2719:		maxs[i] = origin[i] + radius;
ADDRLP4 4176
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4176
INDIRI4
ADDRLP4 4156
ADDP4
ADDRLP4 4176
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRFP4 12
INDIRF4
ADDF4
ASGNF4
line 2720
;2720:	}
LABELV $944
line 2717
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $943
line 2722
;2721:
;2722:	numListedEntities = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );
ADDRLP4 4144
ARGP4
ADDRLP4 4156
ARGP4
ADDRLP4 44
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 4172
ADDRGP4 trap_EntitiesInBox
CALLI4
ASGNI4
ADDRLP4 4140
ADDRLP4 4172
INDIRI4
ASGNI4
line 2724
;2723:
;2724:	for ( e = 0 ; e < numListedEntities ; e++ ) {
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRGP4 $950
JUMPV
LABELV $947
line 2725
;2725:		ent = &g_entities[entityList[ e ]];
ADDRLP4 4
CNSTI4 828
ADDRLP4 20
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 44
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 2727
;2726:
;2727:		if (ent == ignore)
ADDRLP4 4
INDIRP4
CVPU4 4
ADDRFP4 16
INDIRP4
CVPU4 4
NEU4 $951
line 2728
;2728:			continue;
ADDRGP4 $948
JUMPV
LABELV $951
line 2729
;2729:		if (!ent->takedamage)
ADDRLP4 4
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
CNSTI4 0
NEI4 $953
line 2730
;2730:			continue;
ADDRGP4 $948
JUMPV
LABELV $953
line 2733
;2731:
;2732:		// find the distance from the edge of the bounding box
;2733:		for ( i = 0 ; i < 3 ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $955
line 2734
;2734:			if ( origin[i] < ent->r.absmin[i] ) {
ADDRLP4 4176
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4176
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRLP4 4176
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 344
ADDP4
ADDP4
INDIRF4
GEF4 $959
line 2735
;2735:				v[i] = ent->r.absmin[i] - origin[i];
ADDRLP4 4180
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4180
INDIRI4
ADDRLP4 8
ADDP4
ADDRLP4 4180
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 344
ADDP4
ADDP4
INDIRF4
ADDRLP4 4180
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 2736
;2736:			} else if ( origin[i] > ent->r.absmax[i] ) {
ADDRGP4 $960
JUMPV
LABELV $959
ADDRLP4 4180
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4180
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRLP4 4180
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 356
ADDP4
ADDP4
INDIRF4
LEF4 $961
line 2737
;2737:				v[i] = origin[i] - ent->r.absmax[i];
ADDRLP4 4184
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4184
INDIRI4
ADDRLP4 8
ADDP4
ADDRLP4 4184
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRLP4 4184
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 356
ADDP4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 2738
;2738:			} else {
ADDRGP4 $962
JUMPV
LABELV $961
line 2739
;2739:				v[i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
CNSTF4 0
ASGNF4
line 2740
;2740:			}
LABELV $962
LABELV $960
line 2741
;2741:		}
LABELV $956
line 2733
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $955
line 2743
;2742:
;2743:		dist = VectorLength( v );
ADDRLP4 8
ARGP4
ADDRLP4 4176
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 24
ADDRLP4 4176
INDIRF4
ASGNF4
line 2744
;2744:		if ( dist >= radius ) {
ADDRLP4 24
INDIRF4
ADDRFP4 12
INDIRF4
LTF4 $963
line 2745
;2745:			continue;
ADDRGP4 $948
JUMPV
LABELV $963
line 2748
;2746:		}
;2747:
;2748:		points = damage * ( 1.0 - dist / radius );
ADDRLP4 40
ADDRFP4 8
INDIRF4
CNSTF4 1065353216
ADDRLP4 24
INDIRF4
ADDRFP4 12
INDIRF4
DIVF4
SUBF4
MULF4
ASGNF4
line 2750
;2749:
;2750:		if( CanDamage (ent, origin) ) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4180
ADDRGP4 CanDamage
CALLI4
ASGNI4
ADDRLP4 4180
INDIRI4
CNSTI4 0
EQI4 $965
line 2751
;2751:			if( LogAccuracyHit( ent, attacker ) ) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4184
ADDRGP4 LogAccuracyHit
CALLI4
ASGNI4
ADDRLP4 4184
INDIRI4
CNSTI4 0
EQI4 $967
line 2752
;2752:				hitClient = qtrue;
ADDRLP4 4168
CNSTI4 1
ASGNI4
line 2753
;2753:			}
LABELV $967
line 2754
;2754:			VectorSubtract (ent->r.currentOrigin, origin, dir);
ADDRLP4 4192
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
ADDRLP4 4
INDIRP4
CNSTI4 368
ADDP4
INDIRF4
ADDRLP4 4192
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 28+4
ADDRLP4 4
INDIRP4
CNSTI4 372
ADDP4
INDIRF4
ADDRLP4 4192
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 28+8
ADDRLP4 4
INDIRP4
CNSTI4 376
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
SUBF4
ASGNF4
line 2757
;2755:			// push the center of mass higher than the origin so players
;2756:			// get knocked into the air more
;2757:			dir[2] += 24;
ADDRLP4 28+8
ADDRLP4 28+8
INDIRF4
CNSTF4 1103101952
ADDF4
ASGNF4
line 2758
;2758:			G_Damage (ent, NULL, attacker, dir, origin, (int)points, DAMAGE_RADIUS, mod);
ADDRLP4 4
INDIRP4
ARGP4
CNSTP4 0
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 28
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 40
INDIRF4
CVFI4 4
ARGI4
CNSTI4 1
ARGI4
ADDRFP4 20
INDIRI4
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 2759
;2759:		}
LABELV $965
line 2760
;2760:	}
LABELV $948
line 2724
ADDRLP4 20
ADDRLP4 20
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $950
ADDRLP4 20
INDIRI4
ADDRLP4 4140
INDIRI4
LTI4 $947
line 2762
;2761:
;2762:	return hitClient;
ADDRLP4 4168
INDIRI4
RETI4
LABELV $940
endproc G_RadiusDamage 4196 32
import G_G2PlayerAngles
import ThrowSaberToAttacker
import BotDamageNotification
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
import TossClientCubes
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
LABELV $841
char 1 37
char 1 105
char 1 58
char 1 32
char 1 99
char 1 108
char 1 105
char 1 101
char 1 110
char 1 116
char 1 58
char 1 37
char 1 105
char 1 32
char 1 104
char 1 101
char 1 97
char 1 108
char 1 116
char 1 104
char 1 58
char 1 37
char 1 105
char 1 32
char 1 100
char 1 97
char 1 109
char 1 97
char 1 103
char 1 101
char 1 58
char 1 37
char 1 105
char 1 32
char 1 97
char 1 114
char 1 109
char 1 111
char 1 114
char 1 58
char 1 37
char 1 105
char 1 10
char 1 0
align 1
LABELV $650
char 1 112
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 108
char 1 105
char 1 109
char 1 98
char 1 0
align 1
LABELV $438
char 1 75
char 1 105
char 1 108
char 1 108
char 1 58
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 58
char 1 32
char 1 37
char 1 115
char 1 32
char 1 107
char 1 105
char 1 108
char 1 108
char 1 101
char 1 100
char 1 32
char 1 37
char 1 115
char 1 32
char 1 98
char 1 121
char 1 32
char 1 37
char 1 115
char 1 10
char 1 0
align 1
LABELV $437
char 1 60
char 1 98
char 1 97
char 1 100
char 1 32
char 1 111
char 1 98
char 1 105
char 1 116
char 1 117
char 1 97
char 1 114
char 1 121
char 1 62
char 1 0
align 1
LABELV $430
char 1 60
char 1 119
char 1 111
char 1 114
char 1 108
char 1 100
char 1 62
char 1 0
align 1
LABELV $429
char 1 60
char 1 110
char 1 111
char 1 110
char 1 45
char 1 99
char 1 108
char 1 105
char 1 101
char 1 110
char 1 116
char 1 62
char 1 0
align 1
LABELV $311
char 1 77
char 1 79
char 1 68
char 1 95
char 1 84
char 1 82
char 1 73
char 1 71
char 1 71
char 1 69
char 1 82
char 1 95
char 1 72
char 1 85
char 1 82
char 1 84
char 1 0
align 1
LABELV $310
char 1 77
char 1 79
char 1 68
char 1 95
char 1 84
char 1 65
char 1 82
char 1 71
char 1 69
char 1 84
char 1 95
char 1 76
char 1 65
char 1 83
char 1 69
char 1 82
char 1 0
align 1
LABELV $309
char 1 77
char 1 79
char 1 68
char 1 95
char 1 83
char 1 85
char 1 73
char 1 67
char 1 73
char 1 68
char 1 69
char 1 0
align 1
LABELV $308
char 1 77
char 1 79
char 1 68
char 1 95
char 1 70
char 1 65
char 1 76
char 1 76
char 1 73
char 1 78
char 1 71
char 1 0
align 1
LABELV $307
char 1 77
char 1 79
char 1 68
char 1 95
char 1 84
char 1 69
char 1 76
char 1 69
char 1 70
char 1 82
char 1 65
char 1 71
char 1 0
align 1
LABELV $306
char 1 77
char 1 79
char 1 68
char 1 95
char 1 67
char 1 82
char 1 85
char 1 83
char 1 72
char 1 0
align 1
LABELV $305
char 1 77
char 1 79
char 1 68
char 1 95
char 1 76
char 1 65
char 1 86
char 1 65
char 1 0
align 1
LABELV $304
char 1 77
char 1 79
char 1 68
char 1 95
char 1 83
char 1 76
char 1 73
char 1 77
char 1 69
char 1 0
align 1
LABELV $303
char 1 77
char 1 79
char 1 68
char 1 95
char 1 87
char 1 65
char 1 84
char 1 69
char 1 82
char 1 0
align 1
LABELV $302
char 1 77
char 1 79
char 1 68
char 1 95
char 1 83
char 1 69
char 1 78
char 1 84
char 1 82
char 1 89
char 1 0
align 1
LABELV $301
char 1 77
char 1 79
char 1 68
char 1 95
char 1 70
char 1 79
char 1 82
char 1 67
char 1 69
char 1 95
char 1 68
char 1 65
char 1 82
char 1 75
char 1 0
align 1
LABELV $300
char 1 77
char 1 79
char 1 68
char 1 95
char 1 68
char 1 69
char 1 84
char 1 95
char 1 80
char 1 65
char 1 67
char 1 75
char 1 95
char 1 83
char 1 80
char 1 76
char 1 65
char 1 83
char 1 72
char 1 0
align 1
LABELV $299
char 1 77
char 1 79
char 1 68
char 1 95
char 1 84
char 1 73
char 1 77
char 1 69
char 1 68
char 1 95
char 1 77
char 1 73
char 1 78
char 1 69
char 1 95
char 1 83
char 1 80
char 1 76
char 1 65
char 1 83
char 1 72
char 1 0
align 1
LABELV $298
char 1 77
char 1 79
char 1 68
char 1 95
char 1 84
char 1 82
char 1 73
char 1 80
char 1 95
char 1 77
char 1 73
char 1 78
char 1 69
char 1 95
char 1 83
char 1 80
char 1 76
char 1 65
char 1 83
char 1 72
char 1 0
align 1
LABELV $297
char 1 77
char 1 79
char 1 68
char 1 95
char 1 84
char 1 72
char 1 69
char 1 82
char 1 77
char 1 65
char 1 76
char 1 95
char 1 83
char 1 80
char 1 76
char 1 65
char 1 83
char 1 72
char 1 0
align 1
LABELV $296
char 1 77
char 1 79
char 1 68
char 1 95
char 1 84
char 1 72
char 1 69
char 1 82
char 1 77
char 1 65
char 1 76
char 1 0
align 1
LABELV $295
char 1 77
char 1 79
char 1 68
char 1 95
char 1 82
char 1 79
char 1 67
char 1 75
char 1 69
char 1 84
char 1 95
char 1 72
char 1 79
char 1 77
char 1 73
char 1 78
char 1 71
char 1 95
char 1 83
char 1 80
char 1 76
char 1 65
char 1 83
char 1 72
char 1 0
align 1
LABELV $294
char 1 77
char 1 79
char 1 68
char 1 95
char 1 82
char 1 79
char 1 67
char 1 75
char 1 69
char 1 84
char 1 95
char 1 72
char 1 79
char 1 77
char 1 73
char 1 78
char 1 71
char 1 0
align 1
LABELV $293
char 1 77
char 1 79
char 1 68
char 1 95
char 1 82
char 1 79
char 1 67
char 1 75
char 1 69
char 1 84
char 1 95
char 1 83
char 1 80
char 1 76
char 1 65
char 1 83
char 1 72
char 1 0
align 1
LABELV $292
char 1 77
char 1 79
char 1 68
char 1 95
char 1 82
char 1 79
char 1 67
char 1 75
char 1 69
char 1 84
char 1 0
align 1
LABELV $291
char 1 77
char 1 79
char 1 68
char 1 95
char 1 70
char 1 76
char 1 69
char 1 67
char 1 72
char 1 69
char 1 84
char 1 84
char 1 69
char 1 95
char 1 65
char 1 76
char 1 84
char 1 95
char 1 83
char 1 80
char 1 76
char 1 65
char 1 83
char 1 72
char 1 0
align 1
LABELV $290
char 1 77
char 1 79
char 1 68
char 1 95
char 1 70
char 1 76
char 1 69
char 1 67
char 1 72
char 1 69
char 1 84
char 1 84
char 1 69
char 1 0
align 1
LABELV $289
char 1 77
char 1 79
char 1 68
char 1 95
char 1 68
char 1 69
char 1 77
char 1 80
char 1 50
char 1 95
char 1 65
char 1 76
char 1 84
char 1 0
align 1
LABELV $288
char 1 77
char 1 79
char 1 68
char 1 95
char 1 68
char 1 69
char 1 77
char 1 80
char 1 50
char 1 0
align 1
LABELV $287
char 1 77
char 1 79
char 1 68
char 1 95
char 1 82
char 1 69
char 1 80
char 1 69
char 1 65
char 1 84
char 1 69
char 1 82
char 1 95
char 1 65
char 1 76
char 1 84
char 1 95
char 1 83
char 1 80
char 1 76
char 1 65
char 1 83
char 1 72
char 1 0
align 1
LABELV $286
char 1 77
char 1 79
char 1 68
char 1 95
char 1 82
char 1 69
char 1 80
char 1 69
char 1 65
char 1 84
char 1 69
char 1 82
char 1 95
char 1 65
char 1 76
char 1 84
char 1 0
align 1
LABELV $285
char 1 77
char 1 79
char 1 68
char 1 95
char 1 82
char 1 69
char 1 80
char 1 69
char 1 65
char 1 84
char 1 69
char 1 82
char 1 0
align 1
LABELV $284
char 1 77
char 1 79
char 1 68
char 1 95
char 1 66
char 1 79
char 1 87
char 1 67
char 1 65
char 1 83
char 1 84
char 1 69
char 1 82
char 1 0
align 1
LABELV $283
char 1 77
char 1 79
char 1 68
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
char 1 0
align 1
LABELV $282
char 1 77
char 1 79
char 1 68
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
char 1 80
char 1 76
char 1 65
char 1 83
char 1 72
char 1 0
align 1
LABELV $281
char 1 77
char 1 79
char 1 68
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
char 1 0
align 1
LABELV $280
char 1 77
char 1 79
char 1 68
char 1 95
char 1 66
char 1 76
char 1 65
char 1 83
char 1 84
char 1 69
char 1 82
char 1 0
align 1
LABELV $279
char 1 77
char 1 79
char 1 68
char 1 95
char 1 66
char 1 82
char 1 89
char 1 65
char 1 82
char 1 95
char 1 80
char 1 73
char 1 83
char 1 84
char 1 79
char 1 76
char 1 95
char 1 65
char 1 76
char 1 84
char 1 0
align 1
LABELV $278
char 1 77
char 1 79
char 1 68
char 1 95
char 1 66
char 1 82
char 1 89
char 1 65
char 1 82
char 1 95
char 1 80
char 1 73
char 1 83
char 1 84
char 1 79
char 1 76
char 1 0
align 1
LABELV $277
char 1 77
char 1 79
char 1 68
char 1 95
char 1 83
char 1 65
char 1 66
char 1 69
char 1 82
char 1 0
align 1
LABELV $276
char 1 77
char 1 79
char 1 68
char 1 95
char 1 77
char 1 69
char 1 76
char 1 69
char 1 69
char 1 0
align 1
LABELV $275
char 1 77
char 1 79
char 1 68
char 1 95
char 1 83
char 1 84
char 1 85
char 1 78
char 1 95
char 1 66
char 1 65
char 1 84
char 1 79
char 1 78
char 1 0
align 1
LABELV $274
char 1 77
char 1 79
char 1 68
char 1 95
char 1 85
char 1 78
char 1 75
char 1 78
char 1 79
char 1 87
char 1 78
char 1 0
