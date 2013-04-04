// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_effects.c -- these functions generate localentities, usually as a result
// of event processing

#include "cg_local.h"

/*
==================
CG_BubbleTrail

Bullets shot underwater
==================
*/
void CG_BubbleTrail( vec3_t start, vec3_t end, float spacing ) {
	vec3_t		move;
	vec3_t		vec;
	float		len;
	int			i;

	if ( cg_noProjectileTrail.integer ) {
		return;
	}

	VectorCopy (start, move);
	VectorSubtract (end, start, vec);
	len = VectorNormalize (vec);

	// advance a random amount first
	i = rand() % (int)spacing;
	VectorMA( move, i, vec, move );

	VectorScale (vec, spacing, vec);

	for ( ; i < len; i += spacing ) {
		localEntity_t	*le;
		refEntity_t		*re;

		le = CG_AllocLocalEntity();
		le->leFlags = LEF_PUFF_DONT_SCALE;
		le->leType = LE_MOVE_SCALE_FADE;
		le->startTime = cg.time;
		le->endTime = cg.time + 1000 + random() * 250;
		le->lifeRate = 1.0 / ( le->endTime - le->startTime );

		re = &le->refEntity;
		re->shaderTime = cg.time / 1000.0f;

		re->reType = RT_SPRITE;
		re->rotation = 0;
		re->radius = 3;
		re->customShader = cgs.media.waterBubbleShader;
		re->shaderRGBA[0] = 0xff;
		re->shaderRGBA[1] = 0xff;
		re->shaderRGBA[2] = 0xff;
		re->shaderRGBA[3] = 0xff;

		le->color[3] = 1.0;

		le->pos.trType = TR_LINEAR;
		le->pos.trTime = cg.time;
		VectorCopy( move, le->pos.trBase );
		le->pos.trDelta[0] = crandom()*5;
		le->pos.trDelta[1] = crandom()*5;
		le->pos.trDelta[2] = crandom()*5 + 6;

		VectorAdd (move, vec, move);
	}
}

/*
=====================
CG_SmokePuff

Adds a smoke puff or blood trail localEntity.
=====================
*/
localEntity_t *CG_SmokePuff( const vec3_t p, const vec3_t vel, 
				   float radius,
				   float r, float g, float b, float a,
				   float duration,
				   int startTime,
				   int fadeInTime,
				   int leFlags,
				   qhandle_t hShader ) {
	static int	seed = 0x92;
	localEntity_t	*le;
	refEntity_t		*re;
//	int fadeInTime = startTime + duration / 2;

	le = CG_AllocLocalEntity();
	le->leFlags = leFlags;
	le->radius = radius;

	re = &le->refEntity;
	re->rotation = Q_random( &seed ) * 360;
	re->radius = radius;
	re->shaderTime = startTime / 1000.0f;

	le->leType = LE_MOVE_SCALE_FADE;
	le->startTime = startTime;
	le->fadeInTime = fadeInTime;
	le->endTime = startTime + duration;
	if ( fadeInTime > startTime ) {
		le->lifeRate = 1.0 / ( le->endTime - le->fadeInTime );
	}
	else {
		le->lifeRate = 1.0 / ( le->endTime - le->startTime );
	}
	le->color[0] = r;
	le->color[1] = g; 
	le->color[2] = b;
	le->color[3] = a;


	le->pos.trType = TR_LINEAR;
	le->pos.trTime = startTime;
	VectorCopy( vel, le->pos.trDelta );
	VectorCopy( p, le->pos.trBase );

	VectorCopy( p, re->origin );
	re->customShader = hShader;

	re->shaderRGBA[0] = le->color[0] * 0xff;
	re->shaderRGBA[1] = le->color[1] * 0xff;
	re->shaderRGBA[2] = le->color[2] * 0xff;
	re->shaderRGBA[3] = 0xff;

	re->reType = RT_SPRITE;
	re->radius = le->radius;

	return le;
}

void CG_TestLine( vec3_t start, vec3_t end, int time, unsigned int color, int radius) {
	localEntity_t	*le;
	refEntity_t		*re;

	le = CG_AllocLocalEntity();
	le->leType = LE_LINE;
	le->startTime = cg.time;
	le->endTime = cg.time + time;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );

	re = &le->refEntity;
	VectorCopy( start, re->origin );
	VectorCopy( end, re->oldorigin);
	re->shaderTime = cg.time / 1000.0f;

	re->reType = RT_LINE;
	re->radius = 0.5*radius;
	re->customShader = cgs.media.whiteShader; //trap_R_RegisterShaderNoMip("textures/colombia/canvas_doublesided");

	re->shaderTexCoord[0] = re->shaderTexCoord[1] = 1.0f;

	if (color==0)
	{
		re->shaderRGBA[0] = re->shaderRGBA[1] = re->shaderRGBA[2] = re->shaderRGBA[3] = 0xff;
	}
	else
	{
		re->shaderRGBA[0] = color & 0xff;
		color >>= 8;
		re->shaderRGBA[1] = color & 0xff;
		color >>= 8;
		re->shaderRGBA[2] = color & 0xff;
//		color >>= 8;
//		re->shaderRGBA[3] = color & 0xff;
		re->shaderRGBA[3] = 0xff;
	}

	le->color[3] = 1.0;
}

/*
==================
CG_ThrowChunk
==================
*/
void CG_ThrowChunk( vec3_t origin, vec3_t velocity, qhandle_t hModel, int optionalSound, int startalpha ) {
	localEntity_t	*le;
	refEntity_t		*re;

	le = CG_AllocLocalEntity();
	re = &le->refEntity;

	le->leType = LE_FRAGMENT;
	le->startTime = cg.time;
	le->endTime = le->startTime + 5000 + random() * 3000;

	VectorCopy( origin, re->origin );
	AxisCopy( axisDefault, re->axis );
	re->hModel = hModel;

	le->pos.trType = TR_GRAVITY;
	le->angles.trType = TR_GRAVITY;
	VectorCopy( origin, le->pos.trBase );
	VectorCopy( velocity, le->pos.trDelta );
	VectorSet(le->angles.trBase, 20, 20, 20);
	VectorCopy( velocity, le->angles.trDelta );
	le->pos.trTime = cg.time;
	le->angles.trTime = cg.time;

	le->leFlags = LEF_TUMBLE;

	le->angles.trBase[YAW] = 180;

	le->bounceFactor = 0.3f;
	le->bounceSound = optionalSound;

	le->forceAlpha = startalpha;
}

//----------------------------
//
// Breaking Glass Technology
//
//----------------------------

// Since we have shared verts when we tesselate the glass sheet, it helps to have a 
//	random offset table set up up front.

static float offX[20][20],
			offZ[20][20];

#define	FX_ALPHA_NONLINEAR	0x00000004
#define FX_APPLY_PHYSICS	0x02000000
#define FX_USE_ALPHA		0x08000000

static void CG_DoGlassQuad( vec3_t p[4], vec2_t uv[4], qboolean stick, int time, vec3_t dmgDir )
{
	float	bounce;
	vec3_t	rotDelta;
	vec3_t	vel, accel;
	vec3_t	rgb1;
	addpolyArgStruct_t apArgs;
	int		i, i_2;

	VectorSet( vel, crandom() * 12, crandom() * 12, -1 );

	if ( !stick )
	{
		// We aren't a motion delayed chunk, so let us move quickly
		VectorMA( vel, 0.3f, dmgDir, vel );
	}

	// Set up acceleration due to gravity, 800 is standard QuakeIII gravity, so let's use something close
	VectorSet( accel, 0.0f, 0.0f, -(600.0f + random() * 100.0f ) );

	// We are using an additive shader, so let's set the RGB low so we look more like transparent glass
//	VectorSet( rgb1, 0.1f, 0.1f, 0.1f );
	VectorSet( rgb1, 1.0f, 1.0f, 1.0f );

	// Being glass, we don't want to bounce much
	bounce = random() * 0.2f + 0.15f;

	// Set up our random rotate, we only do PITCH and YAW, not ROLL.  This is something like degrees per second
	VectorSet( rotDelta, crandom() * 40.0f, crandom() * 40.0f, 0.0f );

	//In an ideal world, this might actually work.
	/*
	CPoly *pol = FX_AddPoly(p, uv, 4,			// verts, ST, vertCount
			vel, accel,				// motion
			0.15f, 0.0f, 85.0f,		// alpha start, alpha end, alpha parm ( begin alpha fade when 85% of life is complete )
			rgb1, rgb1, 0.0f,		// rgb start, rgb end, rgb parm ( not used )
			rotDelta, bounce, time,	// rotation amount, bounce, and time to delay motion for ( zero if no delay );
			6000,					// life
			cgi_R_RegisterShader( "gfx/misc/test_crackle" ), 
			FX_APPLY_PHYSICS | FX_ALPHA_NONLINEAR | FX_USE_ALPHA );

	if ( random() > 0.95f && pol )
	{
		pol->AddFlags( FX_IMPACT_RUNS_FX | FX_KILL_ON_IMPACT );
		pol->SetImpactFxID( theFxScheduler.RegisterEffect( "glass_impact" ));
	}
	*/

	//rww - this is dirty.

	i = 0;
	i_2 = 0;

	while (i < 4)
	{
		while (i_2 < 3)
		{
			apArgs.p[i][i_2] = p[i][i_2];

			i_2++;
		}

		i_2 = 0;
		i++;
	}

	i = 0;
	i_2 = 0;

	while (i < 4)
	{
		while (i_2 < 2)
		{
			apArgs.ev[i][i_2] = uv[i][i_2];

			i_2++;
		}

		i_2 = 0;
		i++;
	}

	apArgs.numVerts = 4;
	VectorCopy(vel, apArgs.vel);
	VectorCopy(accel, apArgs.accel);

	apArgs.alpha1 = 0.15f;
	apArgs.alpha2 = 0.0f;
	apArgs.alphaParm = 85.0f;

	VectorCopy(rgb1, apArgs.rgb1);
	VectorCopy(rgb1, apArgs.rgb2);

	apArgs.rgbParm = 0.0f;

	VectorCopy(rotDelta, apArgs.rotationDelta);

	apArgs.bounce = bounce;
	apArgs.motionDelay = time;
	apArgs.killTime = 6000;
	apArgs.shader = cgs.media.glassShardShader;
	apArgs.flags = (FX_APPLY_PHYSICS | FX_ALPHA_NONLINEAR | FX_USE_ALPHA);

	trap_FX_AddPoly(&apArgs);
}

static void CG_CalcBiLerp( vec3_t verts[4], vec3_t subVerts[4], vec2_t uv[4] )
{
	vec3_t	temp;

	// Nasty crap
	VectorScale( verts[0],	1.0f - uv[0][0],	subVerts[0] );
	VectorMA( subVerts[0],	uv[0][0],			verts[1], subVerts[0] );
	VectorScale( subVerts[0], 1.0f - uv[0][1],	temp );
	VectorScale( verts[3],	1.0f - uv[0][0],	subVerts[0] );
	VectorMA( subVerts[0],	uv[0][0],			verts[2], subVerts[0] );
	VectorMA( temp,			uv[0][1],			subVerts[0], subVerts[0] );

	VectorScale( verts[0],	1.0f - uv[1][0],	subVerts[1] );
	VectorMA( subVerts[1],	uv[1][0],			verts[1], subVerts[1] );
	VectorScale( subVerts[1], 1.0f - uv[1][1],	temp );
	VectorScale( verts[3],	1.0f - uv[1][0],	subVerts[1] );
	VectorMA( subVerts[1],	uv[1][0],			verts[2], subVerts[1] );
	VectorMA( temp,			uv[1][1],			subVerts[1], subVerts[1] );

	VectorScale( verts[0],	1.0f - uv[2][0],	subVerts[2] );
	VectorMA( subVerts[2],	uv[2][0],			verts[1], subVerts[2] );
	VectorScale( subVerts[2], 1.0f - uv[2][1],		temp );
	VectorScale( verts[3],	1.0f - uv[2][0],	subVerts[2] );
	VectorMA( subVerts[2],	uv[2][0],			verts[2], subVerts[2] );
	VectorMA( temp,			uv[2][1],			subVerts[2], subVerts[2] );

	VectorScale( verts[0],	1.0f - uv[3][0],	subVerts[3] );
	VectorMA( subVerts[3],	uv[3][0],			verts[1], subVerts[3] );
	VectorScale( subVerts[3], 1.0f - uv[3][1],	temp );
	VectorScale( verts[3],	1.0f - uv[3][0],	subVerts[3] );
	VectorMA( subVerts[3],	uv[3][0],			verts[2], subVerts[3] );
	VectorMA( temp,			uv[3][1],			subVerts[3], subVerts[3] );
}
// bilinear
//f(p',q') = (1 - y) × {[(1 - x) × f(p,q)] + [x × f(p,q+1)]} + y × {[(1 - x) × f(p+1,q)] + [x × f(p+1,q+1)]}. 


static void CG_CalcHeightWidth( vec3_t verts[4], float *height, float *width )
{
	vec3_t	dir1, dir2, cross;

	VectorSubtract( verts[3], verts[0], dir1 ); // v
	VectorSubtract( verts[1], verts[0], dir2 ); // p-a
	CrossProduct( dir1, dir2, cross );
	*width = VectorNormalize( cross ) / VectorNormalize( dir1 ); // v 
	VectorSubtract( verts[2], verts[0], dir2 ); // p-a
	CrossProduct( dir1, dir2, cross );
	*width += VectorNormalize( cross ) / VectorNormalize( dir1 ); // v 
	*width *= 0.5f;

	VectorSubtract( verts[1], verts[0], dir1 ); // v
	VectorSubtract( verts[2], verts[0], dir2 ); // p-a
	CrossProduct( dir1, dir2, cross );
	*height = VectorNormalize( cross ) / VectorNormalize( dir1 ); // v 
	VectorSubtract( verts[3], verts[0], dir2 ); // p-a
	CrossProduct( dir1, dir2, cross );
	*height += VectorNormalize( cross ) / VectorNormalize( dir1 ); // v 
	*height *= 0.5f;
}
//Consider a line in 3D with position vector "a" and direction vector "v" and 
// let "p" be the position vector of an arbitrary point in 3D
//dist = len( crossprod(p-a,v) ) / len(v); 

void CG_InitGlass( void )
{
	int i, t;

	// Build a table first, so that we can do a more unpredictable crack scheme
	//	do it once, up front to save a bit of time.
	for ( i = 0; i < 20; i++ )
	{
		for ( t = 0; t < 20; t++ )
		{
			offX[t][i] = crandom() * 0.03f;
			offZ[i][t] = crandom() * 0.03f;
		}
	}
}

void Vector2Set(vec2_t a,float b,float c)
{
	a[0] = b;
	a[1] = c;
}

#define TIME_DECAY_SLOW		0.1f
#define TIME_DECAY_MED		0.04f
#define TIME_DECAY_FAST		0.009f

void CG_DoGlass( vec3_t verts[4], vec3_t normal, vec3_t dmgPt, vec3_t dmgDir, float dmgRadius, int maxShards )
{
	int			i, t;
	int			mxHeight, mxWidth;
	float		height, width;
	float		stepWidth, stepHeight;
	float		timeDecay;
	float		x, z;
	float		xx, zz;
	float		dif;
	int			time = 0;
	int			glassShards = 0;
	qboolean	stick = qtrue;
	vec3_t		subVerts[4];
	vec2_t		biPoints[4];

	// To do a smarter tesselation, we should figure out the relative height and width of the brush face,
	//	then use this to pick a lod value from 1-3 in each axis.  This will give us 1-9 lod levels, which will
	//	hopefully be sufficient.
	CG_CalcHeightWidth( verts, &height, &width );

	trap_S_StartSound( dmgPt, -1, CHAN_AUTO, trap_S_RegisterSound("sound/effects/glassbreak1.wav"));

	// Pick "LOD" for height
	if ( height < 100 )
	{
		stepHeight = 0.2f;
		mxHeight = 5;
		timeDecay = TIME_DECAY_SLOW;
	}
	else if ( height > 220 )
	{
		stepHeight = 0.05f;
		mxHeight = 20;
		timeDecay = TIME_DECAY_FAST;
	}
	else
	{
		stepHeight = 0.1f;
		mxHeight = 10;
		timeDecay = TIME_DECAY_MED;
	}

	// Pick "LOD" for width
	/*
	if ( width < 100 )
	{
		stepWidth = 0.2f;
		mxWidth = 5;
		timeDecay = ( timeDecay + TIME_DECAY_SLOW ) * 0.5f;
	}
	else if ( width > 220 )
	{
		stepWidth = 0.05f;
		mxWidth = 20;
		timeDecay = ( timeDecay + TIME_DECAY_FAST ) * 0.5f;
	}
	else
	{
		stepWidth = 0.1f;
		mxWidth = 10;
		timeDecay = ( timeDecay + TIME_DECAY_MED ) * 0.5f;
	}
	*/

	//Attempt to scale the glass directly to the size of the window

	stepWidth = (0.25f - (width*0.0002)); //(width*0.0005));
	mxWidth = width*0.2;
	timeDecay = ( timeDecay + TIME_DECAY_FAST ) * 0.5f;

	if (stepWidth < 0.01f)
	{
		stepWidth = 0.01f;
	}
	if (mxWidth < 5)
	{
		mxWidth = 5;
	}

	for ( z = 0.0f, i = 0; z < 1.0f; z += stepHeight, i++ )
	{
		for ( x = 0.0f, t = 0; x < 1.0f; x += stepWidth, t++ )
		{
			// This is nasty..
			if ( t > 0 && t < mxWidth )
			{
				xx = x - offX[i][t];
			}
			else
			{
				xx = x;
			}

			if ( i > 0 && i < mxHeight )
			{
				zz = z - offZ[t][i];
			}
			else
			{
				zz = z;
			}

			Vector2Set( biPoints[0], xx, zz );

			if ( t + 1 > 0 && t + 1 < mxWidth )
			{
				xx = x - offX[i][t + 1];
			}
			else
			{
				xx = x;
			}

			if ( i > 0 && i < mxHeight )
			{
				zz = z - offZ[t + 1][i];
			}
			else
			{
				zz = z;
			}

			Vector2Set( biPoints[1], xx + stepWidth, zz );

			if ( t + 1 > 0 && t + 1 < mxWidth )
			{
				xx = x - offX[i + 1][t + 1];
			}
			else
			{
				xx = x;
			}

			if ( i + 1 > 0 && i + 1 < mxHeight )
			{
				zz = z - offZ[t + 1][i + 1];
			}
			else
			{
				zz = z;
			}

			Vector2Set( biPoints[2], xx + stepWidth, zz + stepHeight);

			if ( t > 0 && t < mxWidth )
			{
				xx = x - offX[i + 1][t];
			}
			else
			{
				xx = x;
			}

			if ( i + 1 > 0 && i + 1 < mxHeight )
			{
				zz = z - offZ[t][i + 1];
			}
			else
			{
				zz = z;
			}

			Vector2Set( biPoints[3], xx, zz + stepHeight );

			CG_CalcBiLerp( verts, subVerts, biPoints );
			
			dif = DistanceSquared( subVerts[0], dmgPt ) * timeDecay - random() * 32;
			
			// If we decrease dif, we are increasing the impact area, making it more likely to blow out large holes
			dif -= dmgRadius * dmgRadius;

			if ( dif > 1 )
			{
				stick = qtrue;
				time = dif + random() * 200;
			}
			else
			{
				stick = qfalse;
				time = 0;
			}

			CG_DoGlassQuad( subVerts, biPoints, stick, time, dmgDir );
			glassShards++;

			if (maxShards && glassShards >= maxShards)
			{
				return;
			}
		}
	}
}

/*
==================
CG_GlassShatter
Break glass with fancy method
==================
*/
void CG_GlassShatter(int entnum, vec3_t dmgPt, vec3_t dmgDir, float dmgRadius, int maxShards)
{
	vec3_t		verts[4], normal;

	if (cgs.inlineDrawModel[cg_entities[entnum].currentState.modelindex])
	{
		trap_R_GetBModelVerts(cgs.inlineDrawModel[cg_entities[entnum].currentState.modelindex], verts, normal);
		CG_DoGlass(verts, normal, dmgPt, dmgDir, dmgRadius, maxShards);
	}
	//otherwise something awful has happened.
}

/*
==================
CG_GlassShatter_Old
Throws glass shards from within a given bounding box in the world
==================
*/
void CG_GlassShatter_Old(int entnum, vec3_t org, vec3_t mins, vec3_t maxs)
{
	vec3_t velocity, a, shardorg, dif, difx;
	float windowmass;
	float shardsthrow = 0;
	char chunkname[256];

	trap_S_StartSound(org, entnum, CHAN_BODY, trap_S_RegisterSound("sound/effects/glassbreak1.wav"));

	VectorSubtract(maxs, mins, a);

	windowmass = VectorLength(a); //should give us some idea of how big the chunk of glass is

	while (shardsthrow < windowmass)
	{
		velocity[0] = crandom()*150;
		velocity[1] = crandom()*150;
		velocity[2] = 150 + crandom()*75;

		Com_sprintf(chunkname, sizeof(chunkname), "models/chunks/glass/glchunks_%i.md3", Q_irand(1, 6));
		VectorCopy(org, shardorg);
	
		dif[0] = (maxs[0]-mins[0])/2;
		dif[1] = (maxs[1]-mins[1])/2;
		dif[2] = (maxs[2]-mins[2])/2;

		if (dif[0] < 2)
		{
			dif[0] = 2;
		}
		if (dif[1] < 2)
		{
			dif[1] = 2;
		}
		if (dif[2] < 2)
		{
			dif[2] = 2;
		}

		difx[0] = Q_irand(1, (dif[0]*0.9)*2);
		difx[1] = Q_irand(1, (dif[1]*0.9)*2);
		difx[2] = Q_irand(1, (dif[2]*0.9)*2);

		if (difx[0] > dif[0])
		{
			shardorg[0] += difx[0]-(dif[0]);
		}
		else
		{
			shardorg[0] -= difx[0];
		}
		if (difx[1] > dif[1])
		{
			shardorg[1] += difx[1]-(dif[1]);
		}
		else
		{
			shardorg[1] -= difx[1];
		}
		if (difx[2] > dif[2])
		{
			shardorg[2] += difx[2]-(dif[2]);
		}
		else
		{
			shardorg[2] -= difx[2];
		}

		//CG_TestLine(org, shardorg, 5000, 0x0000ff, 3);

		CG_ThrowChunk( shardorg, velocity, trap_R_RegisterModel( chunkname ), 0, 254 );

		shardsthrow += 10;
	}
}

/*
==================
CG_CreateDebris
Throws specified debris from within a given bounding box in the world
==================
*/
#define DEBRIS_SPECIALCASE_ROCK			-1
#define DEBRIS_SPECIALCASE_CHUNKS		-2
#define DEBRIS_SPECIALCASE_WOOD			-3
#define DEBRIS_SPECIALCASE_GLASS		-4
void CG_CreateDebris(int entnum, vec3_t org, vec3_t mins, vec3_t maxs, int debrissound, int debrismodel)
{
	vec3_t velocity, a, shardorg, dif, difx;
	float windowmass;
	float shardsthrow = 0;
	char chunkname[256];
	int rfact = 0;
	int omodel = debrismodel;

	VectorSubtract(maxs, mins, a);

	windowmass = VectorLength(a); //should give us some idea of how big the chunk of glass is

	while (shardsthrow < windowmass)
	{
		velocity[0] = crandom()*150;
		velocity[1] = crandom()*150;
		velocity[2] = 150 + crandom()*75;

		if (omodel == DEBRIS_SPECIALCASE_GLASS)
		{
			Com_sprintf(chunkname, sizeof(chunkname), "models/chunks/glass/glchunks_%i.md3", Q_irand(1, 6));
			debrismodel = trap_R_RegisterModel(chunkname);
		}
		else if (omodel == DEBRIS_SPECIALCASE_WOOD)
		{
			Com_sprintf(chunkname, sizeof(chunkname), "models/chunks/generic/wood_%i.md3", Q_irand(1, 3));
			debrismodel = trap_R_RegisterModel(chunkname);
		}
		else if (omodel == DEBRIS_SPECIALCASE_CHUNKS)
		{
			Com_sprintf(chunkname, sizeof(chunkname), "models/chunks/generic/chunks_%i.md3", Q_irand(1, 6));
			debrismodel = trap_R_RegisterModel(chunkname);
		}
		else if (omodel == DEBRIS_SPECIALCASE_ROCK)
		{
			rfact = Q_irand(1, 3);
			if (rfact == 1)
			{
				Com_sprintf(chunkname, sizeof(chunkname), "models/chunks/rock/rock_small.md3");
			}
			else if (rfact == 2)
			{
				Com_sprintf(chunkname, sizeof(chunkname), "models/chunks/rock/rock_med.md3");
			}
			else
			{
				Com_sprintf(chunkname, sizeof(chunkname), "models/chunks/rock/rock_big.md3");
			}
			debrismodel = trap_R_RegisterModel(chunkname);
		}

		VectorCopy(org, shardorg);
	
		dif[0] = (maxs[0]-mins[0])/2;
		dif[1] = (maxs[1]-mins[1])/2;
		dif[2] = (maxs[2]-mins[2])/2;

		if (dif[0] < 2)
		{
			dif[0] = 2;
		}
		if (dif[1] < 2)
		{
			dif[1] = 2;
		}
		if (dif[2] < 2)
		{
			dif[2] = 2;
		}

		difx[0] = Q_irand(1, (dif[0]*0.9)*2);
		difx[1] = Q_irand(1, (dif[1]*0.9)*2);
		difx[2] = Q_irand(1, (dif[2]*0.9)*2);

		if (difx[0] > dif[0])
		{
			shardorg[0] += difx[0]-(dif[0]);
		}
		else
		{
			shardorg[0] -= difx[0];
		}
		if (difx[1] > dif[1])
		{
			shardorg[1] += difx[1]-(dif[1]);
		}
		else
		{
			shardorg[1] -= difx[1];
		}
		if (difx[2] > dif[2])
		{
			shardorg[2] += difx[2]-(dif[2]);
		}
		else
		{
			shardorg[2] -= difx[2];
		}

		//CG_TestLine(org, shardorg, 5000, 0x0000ff, 3);

		CG_ThrowChunk( shardorg, velocity, debrismodel, debrissound, 0 );

		shardsthrow += 10;
	}
}

/*
==================
CG_ScorePlum
==================
*/
void CG_ScorePlum( int client, vec3_t org, int score ) {
	localEntity_t	*le;
	refEntity_t		*re;
	vec3_t			angles;
	static vec3_t lastPos;

	// only visualize for the client that scored
	if (client != cg.predictedPlayerState.clientNum || cg_scorePlum.integer == 0) {
		return;
	}

	le = CG_AllocLocalEntity();
	le->leFlags = 0;
	le->leType = LE_SCOREPLUM;
	le->startTime = cg.time;
	le->endTime = cg.time + 4000;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );

	
	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;
	le->radius = score;
	
	VectorCopy( org, le->pos.trBase );
	if (org[2] >= lastPos[2] - 20 && org[2] <= lastPos[2] + 20) {
		le->pos.trBase[2] -= 20;
	}

	//CG_Printf( "Plum origin %i %i %i -- %i\n", (int)org[0], (int)org[1], (int)org[2], (int)Distance(org, lastPos));
	VectorCopy(org, lastPos);


	re = &le->refEntity;

	re->reType = RT_SPRITE;
	re->radius = 16;

	VectorClear(angles);
	AnglesToAxis( angles, re->axis );
}


/*
-------------------------
CG_ExplosionEffects

Used to find the player and shake the camera if close enough
intensity ranges from 1 (minor tremble) to 16 (major quake)
-------------------------
*/

void CG_ExplosionEffects( vec3_t origin, float intensity, int radius)
{
	//FIXME: When exactly is the vieworg calculated in relation to the rest of the frame?s

	vec3_t	dir;
	float	dist, intensityScale;
	float	realIntensity;

	VectorSubtract( cg.refdef.vieworg, origin, dir );
	dist = VectorNormalize( dir );

	//Use the dir to add kick to the explosion

	if ( dist > radius )
		return;

	intensityScale = 1 - ( dist / (float) radius );
	realIntensity = intensity * intensityScale;

//	CGCam_Shake( realIntensity, 750 ); // 500 seemed a bit too quick
}


/*
====================
CG_MakeExplosion
====================
*/
localEntity_t *CG_MakeExplosion( vec3_t origin, vec3_t dir, 
								qhandle_t hModel, int numFrames, qhandle_t shader,
								int msec, qboolean isSprite, float scale, int flags )
{
	float			ang = 0;
	localEntity_t	*ex;
	int				offset;
	vec3_t			tmpVec, newOrigin;

	if ( msec <= 0 ) {
		CG_Error( "CG_MakeExplosion: msec = %i", msec );
	}

	// skew the time a bit so they aren't all in sync
	offset = rand() & 63;

	ex = CG_AllocLocalEntity();
	if ( isSprite ) {
		ex->leType = LE_SPRITE_EXPLOSION; 
		ex->refEntity.rotation = rand() % 360;
		ex->radius = scale;
		VectorScale( dir, 16, tmpVec );
		VectorAdd( tmpVec, origin, newOrigin );
	} else {
		ex->leType = LE_EXPLOSION;
		VectorCopy( origin, newOrigin );

		// set axis with random rotate when necessary
		if ( !dir )
		{
			AxisClear( ex->refEntity.axis );
		}
		else
		{
			if ( !(flags & LEF_NO_RANDOM_ROTATE) )
				ang = rand() % 360;
			VectorCopy( dir, ex->refEntity.axis[0] );
			RotateAroundDirection( ex->refEntity.axis, ang );
		}
	}

	ex->startTime = cg.time - offset;
	ex->endTime = ex->startTime + msec;
	
	// bias the time so all shader effects start correctly
	ex->refEntity.shaderTime = ex->startTime / 1000.0f;

	ex->refEntity.hModel = hModel;
	ex->refEntity.customShader = shader;
	ex->lifeRate = (float)numFrames / msec;
	ex->leFlags = flags;

	//Scale the explosion
	if (scale != 1) {
		ex->refEntity.nonNormalizedAxes = qtrue;

		VectorScale( ex->refEntity.axis[0], scale, ex->refEntity.axis[0] );
		VectorScale( ex->refEntity.axis[1], scale, ex->refEntity.axis[1] );
		VectorScale( ex->refEntity.axis[2], scale, ex->refEntity.axis[2] );
	}
	// set origin
	VectorCopy ( newOrigin, ex->refEntity.origin);
	VectorCopy ( newOrigin, ex->refEntity.oldorigin );

	ex->color[0] = ex->color[1] = ex->color[2] = 1.0;

	return ex;
}


/*
-------------------------
CG_SurfaceExplosion

Adds an explosion to a surface
-------------------------
*/

#define NUM_SPARKS		12
#define NUM_PUFFS		1
#define NUM_EXPLOSIONS	4

void CG_SurfaceExplosion( vec3_t origin, vec3_t normal, float radius, float shake_speed, qboolean smoke )
{
	localEntity_t	*le;
	//FXTrail			*particle;
	vec3_t			direction, new_org;
	vec3_t			velocity		= { 0, 0, 0 };
	vec3_t			temp_org, temp_vel;
	float			scale, dscale;
	int				i, numSparks;

	//Sparks
	numSparks = 16 + (random() * 16.0f);
	
	for ( i = 0; i < numSparks; i++ )
	{	
		scale = 0.25f + (random() * 2.0f);
		dscale = -scale*0.5;

/*		particle = FX_AddTrail( origin,
								NULL,
								NULL,
								32.0f,
								-64.0f,
								scale,
								-scale,
								1.0f,
								0.0f,
								0.25f,
								4000.0f,
								cgs.media.sparkShader,
								rand() & FXF_BOUNCE);
		if ( particle == NULL )
			return;

		FXE_Spray( normal, 500, 150, 1.0f, 768 + (rand() & 255), (FXPrimitive *) particle );*/
	}

	//Smoke
	//Move this out a little from the impact surface
	VectorMA( origin, 4, normal, new_org );
	VectorSet( velocity, 0.0f, 0.0f, 16.0f );

	for ( i = 0; i < 4; i++ )
	{
		VectorSet( temp_org, new_org[0] + (crandom() * 16.0f), new_org[1] + (crandom() * 16.0f), new_org[2] + (random() * 4.0f) );
		VectorSet( temp_vel, velocity[0] + (crandom() * 8.0f), velocity[1] + (crandom() * 8.0f), velocity[2] + (crandom() * 8.0f) );

/*		FX_AddSprite(	temp_org,
						temp_vel, 
						NULL, 
						64.0f + (random() * 32.0f), 
						16.0f, 
						1.0f, 
						0.0f,
						20.0f + (crandom() * 90.0f),
						0.5f,
						1500.0f, 
						cgs.media.smokeShader, FXF_USE_ALPHA_CHAN );*/
	}

	//Core of the explosion

	//Orient the explosions to face the camera
	VectorSubtract( cg.refdef.vieworg, origin, direction );
	VectorNormalize( direction );

	//Tag the last one with a light
	le = CG_MakeExplosion( origin, direction, cgs.media.explosionModel, 6, cgs.media.surfaceExplosionShader, 500, qfalse, radius * 0.02f + (random() * 0.3f), 0);
	le->light = 150;
	VectorSet( le->lightColor, 0.9f, 0.8f, 0.5f );

	for ( i = 0; i < NUM_EXPLOSIONS-1; i ++)
	{
		VectorSet( new_org, (origin[0] + (16 + (crandom() * 8))*crandom()), (origin[1] + (16 + (crandom() * 8))*crandom()), (origin[2] + (16 + (crandom() * 8))*crandom()) );
		le = CG_MakeExplosion( new_org, direction, cgs.media.explosionModel, 6, cgs.media.surfaceExplosionShader, 300 + (rand() & 99), qfalse, radius * 0.05f + (crandom() *0.3f), 0);
	}

	//Shake the camera
	CG_ExplosionEffects( origin, shake_speed, 350 );

	// The level designers wanted to be able to turn the smoke spawners off.  The rationale is that they
	//	want to blow up catwalks and such that fall down...when that happens, it shouldn't really leave a mark
	//	and a smoke spewer at the explosion point...
	if ( smoke )
	{
		VectorMA( origin, -8, normal, temp_org );
//		FX_AddSpawner( temp_org, normal, NULL, NULL, 100, random()*25.0f, 5000.0f, (void *) CG_SmokeSpawn );

		//Impact mark
		//FIXME: Replace mark
		//CG_ImpactMark( cgs.media.burnMarkShader, origin, normal, random()*360, 1,1,1,1, qfalse, 8, qfalse );
	}
}

/*
=================
CG_Bleed

This is the spurt of blood when a character gets hit
=================
*/
void CG_Bleed( vec3_t origin, int entityNum ) {
	localEntity_t	*ex;

	if ( !cg_blood.integer ) {
		return;
	}

	ex = CG_AllocLocalEntity();
	ex->leType = LE_EXPLOSION;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 500;
	
	VectorCopy ( origin, ex->refEntity.origin);
	ex->refEntity.reType = RT_SPRITE;
	ex->refEntity.rotation = rand() % 360;
	ex->refEntity.radius = 24;

	ex->refEntity.customShader = 0;//cgs.media.bloodExplosionShader;

	// don't show player's own blood in view
	if ( entityNum == cg.snap->ps.clientNum ) {
		ex->refEntity.renderfx |= RF_THIRD_PERSON;
	}
}



/*
==================
CG_LaunchGib
==================
*/
void CG_LaunchGib( vec3_t origin, vec3_t velocity, qhandle_t hModel ) {
	localEntity_t	*le;
	refEntity_t		*re;

	le = CG_AllocLocalEntity();
	re = &le->refEntity;

	le->leType = LE_FRAGMENT;
	le->startTime = cg.time;
	le->endTime = le->startTime + 5000 + random() * 3000;

	VectorCopy( origin, re->origin );
	AxisCopy( axisDefault, re->axis );
	re->hModel = hModel;

	le->pos.trType = TR_GRAVITY;
	VectorCopy( origin, le->pos.trBase );
	VectorCopy( velocity, le->pos.trDelta );
	le->pos.trTime = cg.time;

	le->bounceFactor = 0.6f;

	le->leBounceSoundType = LEBS_BLOOD;
	le->leMarkType = LEMT_BLOOD;
}

/*
===================
CG_GibPlayer

Generated a bunch of gibs launching out from the bodies location
===================
*/
#define	GIB_VELOCITY	250
#define	GIB_JUMP		250
void CG_GibPlayer( vec3_t playerOrigin ) {
	vec3_t	origin, velocity;

	if ( !cg_blood.integer ) {
		return;
	}

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	if ( rand() & 1 ) {
//		CG_LaunchGib( origin, velocity, cgs.media.gibSkull );
	} else {
//		CG_LaunchGib( origin, velocity, cgs.media.gibBrain );
	}

	// allow gibs to be turned off for speed
	if ( !cg_gibs.integer ) {
		return;
	}

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
//	CG_LaunchGib( origin, velocity, cgs.media.gibAbdomen );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
//	CG_LaunchGib( origin, velocity, cgs.media.gibArm );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
//	CG_LaunchGib( origin, velocity, cgs.media.gibChest );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
//	CG_LaunchGib( origin, velocity, cgs.media.gibFist );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
//	CG_LaunchGib( origin, velocity, cgs.media.gibFoot );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
//	CG_LaunchGib( origin, velocity, cgs.media.gibForearm );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
//	CG_LaunchGib( origin, velocity, cgs.media.gibIntestine );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
//	CG_LaunchGib( origin, velocity, cgs.media.gibLeg );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
//	CG_LaunchGib( origin, velocity, cgs.media.gibLeg );
}

/*
==================
CG_LaunchGib
==================
*/
void CG_LaunchExplode( vec3_t origin, vec3_t velocity, qhandle_t hModel ) {
	localEntity_t	*le;
	refEntity_t		*re;

	le = CG_AllocLocalEntity();
	re = &le->refEntity;

	le->leType = LE_FRAGMENT;
	le->startTime = cg.time;
	le->endTime = le->startTime + 10000 + random() * 6000;

	VectorCopy( origin, re->origin );
	AxisCopy( axisDefault, re->axis );
	re->hModel = hModel;

	le->pos.trType = TR_GRAVITY;
	VectorCopy( origin, le->pos.trBase );
	VectorCopy( velocity, le->pos.trDelta );
	le->pos.trTime = cg.time;

	le->bounceFactor = 0.1f;

	le->leBounceSoundType = LEBS_BRASS;
	le->leMarkType = LEMT_NONE;
}

#define	EXP_VELOCITY	100
#define	EXP_JUMP		150
/*
===================
CG_GibPlayer

Generated a bunch of gibs launching out from the bodies location
===================
*/
void CG_BigExplode( vec3_t playerOrigin ) {
	vec3_t	origin, velocity;

	if ( !cg_blood.integer ) {
		return;
	}

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY;
	velocity[1] = crandom()*EXP_VELOCITY;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
//	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY;
	velocity[1] = crandom()*EXP_VELOCITY;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
//	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY*1.5;
	velocity[1] = crandom()*EXP_VELOCITY*1.5;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
//	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY*2.0;
	velocity[1] = crandom()*EXP_VELOCITY*2.0;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
//	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY*2.5;
	velocity[1] = crandom()*EXP_VELOCITY*2.5;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
//	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );
}

