#include "stdafx.h"
#include "..\qe3.h"

#define	CBLOCK_SUBDIVISIONS		4

/*

  treat CONTENTS_DETAIL like the curve flags


  get / set textures should not change any of the curve flags
  setting a curved texture should set all of the curved faces

*/

typedef float vec5_t[5];

typedef enum {
	CS_FLAT, CS_CURVE, CS_SPLIT
} cubeSide_t;

typedef struct {
	int			points[4];
	int			neighbors[4];
	cubeSide_t	type;
	face_t	*brushFace;
} cubeFace_t;

typedef struct {
	vec5_t		points[8];
	cubeFace_t	faces[6];
	qboolean	negativeCurve;
} curveBlock_t;

float	STfromUXYZ[2][5];
float	STfromXYZ[2][4];

FILE	*curveFile = NULL;
brush_t	*bevelBrush = NULL;

void Write1DMatrix (FILE *f, int x, float *m);
void Write2DMatrix (FILE *f, int y, int x, float *m);
void Write3DMatrix (FILE *f, int y, int x, int z, float *m);


bool g_bCamPaint = false;
const float fFullBright = 1.0;
const float fLowerLimit = .70;
const float fDec = .06;
void SetColor(face_t* f, float fColor[3])
{
	return;
  if (g_bCamPaint)
  {
    fColor[0] = f->d_color[0];
    fColor[1] = f->d_color[1];
    fColor[2] = f->d_color[2];
	  qglColor3fv(fColor);
  }
}


void DecColor(float fColor[3])
{
	return;
  if (g_bCamPaint)
  {
    fColor[0] -= fDec;
    fColor[1] -= fDec ;
    fColor[2] -= fDec;
    for (int i = 0; i < 3; i++)
    {
      if (fColor[i] <= fLowerLimit)
        fColor[i] = fFullBright;
    }
	  qglColor3fv(fColor);
  }
}




/*
===================
CurveTextureMatrix
===================
*/
void CurveTextureMatrix (face_t *f, vec3_t top, vec3_t bottom, float STfromUXYZ[2][5]) {
	texdef_t *td;
	vec3_t	dir;
	int		i, j;
	float	sinv, cosv;
	float	ang;
	float	temp[2][4];
	float	ns, nt;
	float	max;

	td = &f->texdef;
	if (!td->scale[0])
		td->scale[0] = 1;
	if (!td->scale[1])
		td->scale[1] = 1;

	temp[0][0] = 64;		// assume 64 wide
	temp[0][1] = 0;
	temp[0][2] = 0;
	temp[0][3] = 0;

	// calculate the T vector
	VectorSubtract (top, bottom, dir);
	VectorNormalize (dir);
	max = 0;
	j = 0;
	for (i = 0 ; i < 3 ; i++) {
		if (fabs(dir[i]) > max) {
			max = fabs(dir[i]);
			j = i;
		}
	}
	if (dir[j] > 0) {
		VectorSubtract (vec3_origin, dir, dir);
	}

	temp[1][0] = 0;
	temp[1][1] = dir[0];
	temp[1][2] = dir[1];
	temp[1][3] = dir[2];


	// rotate axis
	if (td->rotate == 0)
		{ sinv = 0 ; cosv = 1; }
	else if (td->rotate == 90)
		{ sinv = 1 ; cosv = 0; }
	else if (td->rotate == 180)
		{ sinv = 0 ; cosv = -1; }
	else if (td->rotate == 270)
		{ sinv = -1 ; cosv = 0; }
	else
	{	
		ang = td->rotate / 180 * Q_PI;
		sinv = sin(ang);
		cosv = cos(ang);
	}

	for (i=0 ; i<4 ; i++) {
		ns = cosv * temp[0][i] - sinv * temp[1][i];
		nt = sinv * temp[0][i] +  cosv * temp[1][i];
		STfromUXYZ[0][i] = ns;
		STfromUXYZ[1][i] = nt;
	}

	// scale
	for (i=0 ; i<2 ; i++)
		for (j=0 ; j<4 ; j++)
			STfromUXYZ[i][j] /= td->scale[i];

	// shift
	STfromUXYZ[0][4] = td->shift[0];
	STfromUXYZ[1][4] = td->shift[1];

	// scale by texture
	for (i = 0 ; i < 5 ; i++) {
		STfromUXYZ[0][i] /= f->d_texture->width;
	}

	for (i = 0 ; i < 5 ; i++) {
		STfromUXYZ[1][i] /= f->d_texture->height;
	}

}


/*
===================
OppositeFace
===================
*/
int OppositeFace (curveBlock_t *cb, int face) {
	int			i;
	qboolean	neighbor[6];

	memset (neighbor, 0, sizeof(neighbor));
	for (i = 0 ; i < 4 ; i++) {
		neighbor[cb->faces[face].neighbors[i]] = true;
	}

	for (i = 0 ; i < 6 ; i++) {
		if (i == face) {
			continue;
		}
		if (!neighbor[i]) {
			return i;
		}
	}
	return 0;	// should never happen
}


/*
================
BrushToCurveBlock
================
*/
curveBlock_t	*BrushToCurveBlock(brush_t *b) {
	curveBlock_t	*cb;
	face_t			*f;
	int				i, j, k, l;
	winding_t		*w;
	int				side;
	int				point;
	int				p1, p2;

	cb = reinterpret_cast<curveBlock_t*>(malloc(sizeof(*cb)));
	memset (cb, 0, sizeof(*cb));
	side = 0;
	point = 0;

	// see if it is a negative curve
	for (f = b->brush_faces ; f ; f = f->next) {
		if (f->texdef.contents & CONTENTS_NEGATIVE_CURVE) {
			cb->negativeCurve = true;
			break;
		}
	}

	// find all the shared points
	for (f = b->brush_faces ; f ; f = f->next) {
		if (side >= 6) {
			free(cb);
			return NULL;	// must be six sided
		}

    if (f->texdef.flags & SURF_CURVE_FAKE) {
      continue;
    }

		cb->faces[side].brushFace = f;
		if (f->texdef.flags & SURF_CURVE) {
			cb->faces[side].type = CS_CURVE;
		} else {
			cb->faces[side].type = CS_FLAT;
		}

		w = f->face_winding;
    
    if (!w ) {
      continue;
    }

    if (w->numpoints != 4) {
			free(cb);
			return NULL;	// must be four sided
		}
		for (i = 0 ; i < w->numpoints ; i++) {
			for (j = 0 ; j < point ; j++) {
				if (fabs(w->points[i][0] - cb->points[j][0]) < 0.1
				&& fabs(w->points[i][1] - cb->points[j][1]) < 0.1
				&& fabs(w->points[i][2] - cb->points[j][2]) < 0.1 ) {
					break;
				}
			}
			if (j == 8) {
				free(cb);
				return NULL;	// must have eight points
			}
			VectorCopy (w->points[i], cb->points[j]);
			cb->faces[side].points[i] = j;
			if (j == point) {
				point++;
			}
		}
		side++;
	}

	// find all the neighbor relations
	for (i = 0 ; i < 6 ; i++) {
		for (j = 0 ; j < 4 ; j++) {
			p1 = cb->faces[i].points[j];
			p2 = cb->faces[i].points[(j+1)&3];

			// find the other face that matches this edge
			for (k = 0 ; k < 6 ; k++) {
				for (l = 0 ; l < 4 ; l++) {
					if (cb->faces[k].points[l] == p2 && 
						cb->faces[k].points[(l+1)&3] == p1) {
						cb->faces[i].neighbors[j] = k;
						break;
					}
				}
				if (l < 4) {
					break;
				}
			}
			if (k == 6) {
				free(cb);
				return NULL;	// couldn't match neighbor
			}
		}
	}

	return cb;
}



//====================================================================


/*
================
DrawFullFace
================
*/
void DrawFullFace (curveBlock_t *cb, cubeFace_t *cf) {
	int		k;
	float	*next;

  if (bevelBrush) {
    return;
  }

	// set the texturing matrix
  FaceTextureVectors (cf->brushFace, STfromXYZ);


	if (curveFile) {
		vec3_t	vecs[4];

		for (k = 0 ; k < 4 ; k++) {
			VectorCopy (cb->points[cf->points[k]], vecs[k]);
		}

		fprintf (curveFile, "FACE {\n");
		fprintf (curveFile, "4\n");
		fprintf (curveFile, "textures/%s\n", cf->brushFace->texdef.name);
		Write2DMatrix (curveFile, 2, 4, (float *)STfromXYZ);
		Write2DMatrix (curveFile, 4, 3, (float *)vecs);
		fprintf (curveFile, "}\n");
		return;
	}

	qglBindTexture (GL_TEXTURE_2D, cf->brushFace->d_texture->texture_number);

  float fColor[3];
  SetColor(cf->brushFace, fColor);


	qglBegin (GL_POLYGON);
	for (k = 0 ; k < 4 ; k++) {
		next = cb->points[cf->points[k]];
		qglTexCoord2f (DotProduct(next, STfromXYZ[0]) + STfromXYZ[0][3],
			DotProduct(next, STfromXYZ[1]) + STfromXYZ[1][3]);
		qglVertex3fv (next);
	  DecColor(fColor);
	}
	qglEnd ();
}


/*
===============
DrawCurveFan

Draws a curve as part of a flat surface
===============
*/
void DrawCurveFan (face_t *cf, vec5_t opposite, vec5_t prev, vec5_t peak, vec5_t next) {
	int			i, k, l;
	float		coef[5][3];

	// write it out
	if (curveFile) {
		vec5_t	vecs[4];

		for ( i = 0 ; i < 5 ; i++ ) {
			vecs[0][i] = opposite[i];
			vecs[1][i] = prev[i];
			vecs[2][i] = peak[i];
			vecs[3][i] = next[i];
		}
		fprintf (curveFile, "CURVEFAN {\n");
		fprintf (curveFile, "textures/%s\n", cf->texdef.name);
		Write2DMatrix (curveFile, 4, 5, (float *)vecs);
		fprintf (curveFile, "}\n");
		return;
	}

	// calculate the coefficients
	for (l = 0 ; l < 5 ; l++) {
		float	a, b, c;

		a = prev[l];
		b = peak[l];
		c = next[l];
		coef[l][0] = a;
		coef[l][1] = 2 * b - 2 * a;
		coef[l][2] = a - 2 * b + c;
	}


	// draw it
	qglBindTexture (GL_TEXTURE_2D, cf->d_texture->texture_number);


  float fColor[3];
  SetColor(cf, fColor);


	qglBegin (GL_TRIANGLE_FAN);

	qglTexCoord2fv( opposite + 3 );
	qglVertex3fv( opposite );

	for ( k = 0 ; k <= CBLOCK_SUBDIVISIONS ; k++ ) {
		vec5_t		curve;
		float		f;

		f = (float)k / CBLOCK_SUBDIVISIONS;
		for ( l = 0 ; l < 5 ; l++ ) {
			curve[l] = coef[l][2]*f*f + coef[l][1]*f + coef[l][0];
		}

		qglTexCoord2fv( curve + 3 );
		qglVertex3fv( curve );
    DecColor(fColor);
	}

	qglEnd ();
}

/*
===============
DrawRuledSurface
===============
*/
void DrawRuledSurface (face_t *cf, float ctrl[2][3][5] ) {
	int			j, k, l;
	vec5_t		curve[2][CBLOCK_SUBDIVISIONS+1];
	//float		u;
	float		*v;

	if (curveFile) {
		fprintf (curveFile, "RULED {\n");
		fprintf (curveFile, "textures/%s\n", cf->texdef.name);
		Write3DMatrix (curveFile, 2, 3, 5, (float *)ctrl);
		fprintf (curveFile, "}\n");
		return;
	}

	for (j = 0 ; j < 2 ; j++) {
		for (l = 0 ; l < 5 ; l++) {
			float	a, b, c;
			float	qA, qB, qC;
			float	f;
			int		k;

			a = ctrl[j][0][l];
			b = ctrl[j][1][l];
			c = ctrl[j][2][l];
			qA = a - 2 * b + c;
			qB = 2 * b - 2 * a;
			qC = a;

			for (k = 0 ; k <= CBLOCK_SUBDIVISIONS ; k++) {
				f = (float)k / CBLOCK_SUBDIVISIONS;
				curve[j][k][l] = qA*f*f + qB*f + qC;
			}
		}
	}

	if ( bevelBrush ) {
		face_t	*f;

		for (k = 0 ; k < CBLOCK_SUBDIVISIONS ; k++) {
			f = Face_Clone( bevelBrush->brush_faces );
			f->texdef.flags |= SURF_CURVE_FAKE;
			f->next = bevelBrush->brush_faces;
			bevelBrush->brush_faces = f;

			VectorCopy( curve[0][k], f->planepts[0] );
			VectorCopy( curve[1][k], f->planepts[1] );
			VectorCopy( curve[0][k+1], f->planepts[2] );
			Brush_MakeFacePlane( f );
		}
		return;
	}



	qglBindTexture (GL_TEXTURE_2D, cf->d_texture->texture_number);

  float fColor[3];
  SetColor(cf, fColor);

	qglBegin (GL_QUAD_STRIP);
	for (k = 0 ; k <= CBLOCK_SUBDIVISIONS ; k++) {
		v = curve[0][k];
		qglTexCoord2fv( v + 3 );
		qglVertex3fv( v );

		v = curve[1][k];
		qglTexCoord2fv( v + 3 );
		qglVertex3fv( v );
	}


	qglEnd ();
}


/*
===============
SamplePatch
===============
*/
void SamplePatch (float ctrl[3][3][5], float u, float v, float out[5]) {
	float	vCtrl[3][5];
	int		vPoint;
	int		axis;

	// find the control points for the v coordinate
	for (vPoint = 0 ; vPoint < 3 ; vPoint++) {
		for (axis = 0 ; axis < 5 ; axis++) {
			float	a, b, c;
			float	qA, qB, qC;

			a = ctrl[0][vPoint][axis];
			b = ctrl[1][vPoint][axis];
			c = ctrl[2][vPoint][axis];
			qA = a - 2 * b + c;
			qB = 2 * b - 2 * a;
			qC = a;

			vCtrl[vPoint][axis] = qA * u * u + qB * u + qC;
		}
	}

	// interpolate the v value
	for (axis = 0 ; axis < 5 ; axis++) {
		float	a, b, c;
		float	qA, qB, qC;

		a = vCtrl[0][axis];
		b = vCtrl[1][axis];
		c = vCtrl[2][axis];
		qA = a - 2 * b + c;
		qB = 2 * b - 2 * a;
		qC = a;

		out[axis] = qA * v * v + qB * v + qC;
	}
}

/*
===================
DrawPatch
===================
*/
void DrawPatch (face_t *cf, float ctrl[3][3][5]) {
	int		i, j;
	float	u, v;
	vec5_t	verts[CBLOCK_SUBDIVISIONS+1][CBLOCK_SUBDIVISIONS+1];

	if (curveFile) {
		fprintf (curveFile, "PATCH {\n");
		fprintf (curveFile, "textures/%s\n", cf->texdef.name);
		Write3DMatrix (curveFile, 3, 3, 5, (float *)ctrl);
		fprintf (curveFile, "}\n");
		return;
	}

	for (i = 0 ; i <= CBLOCK_SUBDIVISIONS ; i++) {
		for (j = 0 ; j <= CBLOCK_SUBDIVISIONS ; j++) {
			u = (float)i / CBLOCK_SUBDIVISIONS;
			v = (float)j / CBLOCK_SUBDIVISIONS;
			SamplePatch (ctrl, u, v, verts[i][j]);
		}
	}

	if ( bevelBrush ) {
		face_t		*f;
		vec3_t		v0, v1, v2;
		vec3_t		d1, d2, cross;

		for (i = 0 ; i < CBLOCK_SUBDIVISIONS ; i++) {
			for (j = 0 ; j < CBLOCK_SUBDIVISIONS ; j++) {
				VectorCopy( verts[i][j], v0 );
				VectorCopy( verts[i][j+1], v1 );
				VectorCopy( verts[i+1][j], v2 );

				VectorSubtract( v0, v1, d1 );
				VectorSubtract( v2, v1, d2 );
				CrossProduct( d1, d2, cross );
				if ( VectorLength( cross ) == 0 ) {
					continue;	// degenerate
				}
				f = Face_Clone( bevelBrush->brush_faces );
				f->texdef.flags |= SURF_CURVE_FAKE;
				VectorCopy( v0, f->planepts[0] );
				VectorCopy( v1, f->planepts[1] );
				VectorCopy( v2, f->planepts[2] );
				Brush_MakeFacePlane( f );
				f->next = bevelBrush->brush_faces;
				bevelBrush->brush_faces = f;
			}
		}
		return;
	}



	qglBindTexture (GL_TEXTURE_2D, cf->d_texture->texture_number);

  float fColor[3];
  SetColor(cf, fColor);


	for (i = 0 ; i < CBLOCK_SUBDIVISIONS ; i++) {
		qglBegin (GL_QUAD_STRIP);
		for (j = 0 ; j <= CBLOCK_SUBDIVISIONS ; j++) {
			qglTexCoord2fv( verts[i+1][j] + 3 );
			qglVertex3fv( verts[i+1][j] );
			qglTexCoord2fv( verts[i][j] + 3 );
			qglVertex3fv( verts[i][j] );
      DecColor(fColor);
		}
		qglEnd ();
	}
}


//====================================================================

/*
===================
DrawCurveBlock
===================
*/
void DrawCurveBlock (curveBlock_t *cb) {
	qboolean	curveDrawn;
	cubeFace_t	*cf, *side;
	int			cfNum, j, k;
	float		*prev, *peak, *next, *opposite;
	float		*tTop, *tBottom;
	float		ctrl[3][3][5];

	// the brush has only two or three curve sides, so draw it
	curveDrawn = false;

	for (cfNum = 0 ; cfNum < 6 ; cfNum++) {
		cf = &cb->faces[cfNum];
		if (cf->type == CS_SPLIT) {
			// split sides are never drawn
//	cf->type = CS_FLAT;
			continue;
		}

		if (cf->type == CS_FLAT) {
drawFlat:
			// the neighbors of flat sides determine the outline

			// find the neighbor curve sode
			for (j = 0 ; j < 4 ; j++) {
				side = &cb->faces[cf->neighbors[j]];
				if (side->type == CS_CURVE) {
					break;
				}
			}

			// make sure the first curve didn't start at the last slot
			if (j == 0) {
				if (cb->faces[cf->neighbors[3]].type == CS_CURVE) {
					j = 3;
					side = &cb->faces[cf->neighbors[j]];
				}
			}

			if ( j == 4 || cb->faces[cf->neighbors[(j+1)&3]].type != CS_CURVE) {
				// no double curves
				if (!cb->negativeCurve) {
					DrawFullFace (cb, cf);
				}
				continue;
			}

			// set the texture coordinates for all control points
			FaceTextureVectors (cf->brushFace, STfromXYZ);
			for ( k = 0 ; k < 4 ; k++ ) {
				float	*v;

				v = cb->points[cf->points[k]];
				v[3] = DotProduct( v, STfromXYZ[0] ) + STfromXYZ[0][3];
				v[4] = DotProduct( v, STfromXYZ[1] ) + STfromXYZ[1][3];
			}

			// draw the curve first	
			if (cb->negativeCurve) {
				next = cb->points[cf->points[j]];
				peak = cb->points[cf->points[(j+1)&3]];
				prev = cb->points[cf->points[(j+2)&3]];

				// opposite corner first
				DrawCurveFan (cf->brushFace, peak, prev, peak, next);
			} else {
				prev = cb->points[cf->points[j]];
				peak = cb->points[cf->points[(j+1)&3]];
				next = cb->points[cf->points[(j+2)&3]];

				// opposite corner first
				opposite = cb->points[cf->points[(j+3)&3]];
				DrawCurveFan (cf->brushFace, opposite, prev, peak, next);
			}
			continue;
		}

		memset( ctrl, 0, sizeof(ctrl) );

		if (cf->type != CS_CURVE) {
			continue;
		}

		if (cb->negativeCurve) {
			FaceTextureVectors (cf->brushFace, STfromXYZ);
			qglBindTexture (GL_TEXTURE_2D, cf->brushFace->d_texture->texture_number);
			DrawFullFace (cb, cf);
		}

		if (curveDrawn) {
			continue;
		}
		curveDrawn = true;

		// find the neighboring curved side
		for (j = 0 ; j < 4 ; j++) {
			side = &cb->faces[cf->neighbors[j]];
			if (side->type == CS_CURVE) {
				break;
			}
		}

		// make sure the first curve didn't start at the last slot
		if (j == 0) {
			if (cb->faces[cf->neighbors[3]].type == CS_CURVE) {
				j = 3;
				side = &cb->faces[cf->neighbors[j]];
			}
		}

		// there should never be a curve without a neighboring
		// curve unless a designer has manually messed it up
		if (j == 4) {
			goto drawFlat;
		}

		//
		// if three sides are curved, draw a sphere section
		//
		if (cb->faces[cf->neighbors[(j+1)&3]].type == CS_CURVE
		|| cb->faces[cf->neighbors[(j+3)&3]].type == CS_CURVE ){

			VectorCopy (cb->points[cf->points[(j+3)&3]], ctrl[0][0]);
			VectorCopy (cb->points[cf->points[(j+3)&3]], ctrl[1][0]);
			VectorCopy (cb->points[cf->points[(j+3)&3]], ctrl[2][0]);

			VectorCopy (cb->points[cf->points[(j+0)&3]], ctrl[0][1]);
			VectorCopy (cb->points[cf->points[(j+1)&3]], ctrl[1][1]);
			VectorCopy (cb->points[cf->points[(j+2)&3]], ctrl[2][1]);

			for (k = 0 ; k < 4 ; k++) {
				if (side->points[k] == cf->points[j]) {
					break;
				}
			}
			VectorCopy (cb->points[side->points[(k+1)&3]], ctrl[0][2]);
			VectorCopy (cb->points[side->points[(k+2)&3]], ctrl[1][2]);

			side = &cb->faces[cf->neighbors[(j+1)&3]];
			for (k = 0 ; k < 4 ; k++) {
				if (side->points[k] == cf->points[(j+2)&3]) {
					break;
				}
			}
			VectorCopy (cb->points[side->points[(k+3)&3]], ctrl[2][2]);

			if (cb->negativeCurve) {
				vec3_t	temp;

				for (j = 0 ; j < 3 ; j++) {
					VectorCopy (ctrl[j][0], temp);
					VectorCopy (ctrl[j][2], ctrl[j][0]);
					VectorCopy (temp, ctrl[j][2]);
				}
			}

			// calculate texture coordinates for the control points
			FaceTextureVectors (cf->brushFace, STfromXYZ);
			for ( j = 0 ; j < 3 ; j++ ) {
				for ( k = 0 ; k < 3 ; k++ ) {
					ctrl[j][k][3] = DotProduct( ctrl[j][k], STfromXYZ[0] )
						+ STfromUXYZ[0][3];
					ctrl[j][k][4] = DotProduct( ctrl[j][k], STfromXYZ[1] )
						+ STfromUXYZ[1][3];
				}
			}
			// draw it
			DrawPatch (cf->brushFace, ctrl);
			continue;
		}

		//
		// the curve is a cylinder section
		//

		// prev
		VectorCopy (cb->points[cf->points[(j+2)&3]], ctrl[0][0]);
		VectorCopy (cb->points[cf->points[(j+3)&3]], ctrl[1][0]);

		// peak
		VectorCopy (cb->points[cf->points[(j+1)&3]], ctrl[0][1]);
		VectorCopy (cb->points[cf->points[(j+0)&3]], ctrl[1][1]);

		// next
		for (k = 0 ; k < 4 ; k++) {
			if (side->points[k] == cf->points[j]) {
				break;
			}
		}
		VectorCopy (cb->points[side->points[(k+2)&3]], ctrl[0][2]);
		VectorCopy (cb->points[side->points[(k+1)&3]], ctrl[1][2]);

		if (cb->negativeCurve) {
			vec3_t	temp;
	
			for (k = 0 ; k < 2 ; k++) {
				VectorCopy (ctrl[k][0], temp);
				VectorCopy (ctrl[k][2], ctrl[k][0]);
				VectorCopy (temp, ctrl[k][2]);
			}
		}

		// find the edge opposite peak to use as the T texture axis
		side = &cb->faces[cf->neighbors[(j+3)&3]];
		for (k = 0 ; k < 4 ; k++) {
			if (side->points[k] == cf->points[j]) {
				break;
			}
		}
		tTop = cb->points[side->points[(k+2)&3]];

		side = &cb->faces[cf->neighbors[(j+1)&3]];
		for (k = 0 ; k < 4 ; k++) {
			if (side->points[k] == cf->points[(j+1)&3]) {
				break;
			}
		}
		tBottom = cb->points[side->points[(k+2)&3]];

		// set the texturing matrix
		CurveTextureMatrix (cf->brushFace, tTop, tBottom, STfromUXYZ);
		// calculate texture coordinates at control points
		for ( j = 0 ; j < 2 ; j++ ) {
			for ( k = 0 ; k < 3 ; k++ ) {
				ctrl[j][k][3] = DotProduct( ctrl[j][k], (STfromUXYZ[0]+1) )
					+ STfromUXYZ[0][0] * (k / 2.0) + STfromUXYZ[0][4];
				ctrl[j][k][4] = DotProduct( ctrl[j][k], (STfromUXYZ[1]+1) )
					+ STfromUXYZ[1][0] * (k / 2.0) + STfromUXYZ[1][4];
			}
		}
		DrawRuledSurface (cf->brushFace, ctrl);
	}
}

/*
===================
SubdivideCurveBlock

The curve block will be freed before return.
===================
*/
void SubdivideCurveBlock (curveBlock_t *cb) {
	int				i, j, k, l;
	curveBlock_t	*b1, *b2;
	cubeFace_t		*cf, *side;
	int				p1, p2;

	// if any curved side has curves on both
	// sides, it must be subdivided
	for (i = 0 ; i < 6 ; i++) {
		cf = &cb->faces[i];
		if (cf->type != CS_CURVE) {
			continue;
		}

		for (j = 0 ; j < 2 ; j++) {
			if (cb->faces[cf->neighbors[j]].type == CS_CURVE
				&& cb->faces[cf->neighbors[j+2]].type == CS_CURVE) {
				break;
			}
		}
		if (j == 2) {
			continue;
		}

		// subdivide it
		b1 = reinterpret_cast<curveBlock_t*>(malloc(sizeof(*b1)));
		*b1 = *cb;
		b2 = reinterpret_cast<curveBlock_t*>(malloc(sizeof(*b2)));
		*b2 = *cb;

		b1->faces[cf->neighbors[j+2]].type = CS_SPLIT;
		b2->faces[cf->neighbors[j]].type = CS_SPLIT;

		p1 = cf->points[(j+3)&3];
		p2 = cf->points[j];
		for (k = 0 ; k < 3 ; k++) {
			b1->points[p1][k] =	b2->points[p2][k] = 
				0.5*(cb->points[p1][k] + cb->points[p2][k]);
		}

		side = &cb->faces[cf->neighbors[(j+3)&3]];
		for (l = 0 ; l < 4 ; l++) {
			if (side->points[l] == p2 && side->points[(l+1)&3] == p1) {
				break;
			}
		}

		p2 = side->points[(l+3)&3];
		p1 = side->points[(l+2)&3];
		for (k = 0 ; k < 3 ; k++) {
			b1->points[p1][k] = b2->points[p2][k] = 
				0.5*(cb->points[p1][k] + cb->points[p2][k]);
		}



		p1 = cf->points[(j+2)&3];
		p2 = cf->points[(j+1)&3];
		for (k = 0 ; k < 3 ; k++) {
			b1->points[p1][k] = b2->points[p2][k] = 
				0.5*(cb->points[p1][k] + cb->points[p2][k]);
		}


		side = &cb->faces[cf->neighbors[(j+1)&3]];
		for (l = 0 ; l < 4 ; l++) {
			if (side->points[l] == p1 && side->points[(l+1)&3] == p2) {
				break;
			}
		}

		p2 = side->points[(l+2)&3];
		p1 = side->points[(l+3)&3];
		for (k = 0 ; k < 3 ; k++) {
			b1->points[p1][k] = b2->points[p2][k] = 
				0.5*(cb->points[p1][k] + cb->points[p2][k]);
		}


		free (cb);
		SubdivideCurveBlock (b1);
		SubdivideCurveBlock (b2);
		return;
	}

	// the cb is in canonical form
	DrawCurveBlock (cb);
	free (cb);
}



/*
===============
Curve_Invert
===============
*/
void Curve_Invert (void) {
	brush_t		*b;
	face_t		*f;
	qboolean	curve, negative;

	if (!QE_SingleBrush())
		return;

	b = selected_brushes.next;

	curve = false;
	negative = false;
	for (f = b->brush_faces ; f ; f = f->next) {
		if (f->texdef.contents & CONTENTS_NEGATIVE_CURVE) {
			negative = true;
		}
		if (f->texdef.flags & SURF_CURVE) {
			curve = true;
		}
	}
	if (!curve) {
		return;
	}

	negative ^= 1;
	for (f = b->brush_faces ; f ; f = f->next) {
		if (negative) {
			f->texdef.contents |= CONTENTS_NEGATIVE_CURVE;
		} else {
			f->texdef.contents &= ~CONTENTS_NEGATIVE_CURVE;
		}
	}

	Curve_BuildPoints (b);
	Sys_UpdateWindows (W_ALL);
}

/*
===================
Curve_MakeCurvedBrush
===================
*/
void Curve_MakeCurvedBrush (qboolean negative, qboolean top, qboolean bottom, 
					qboolean s1, qboolean s2, qboolean s3, qboolean s4) {
	brush_t		*b;
	curveBlock_t	*cb;
	cubeFace_t		*cf;
	int			i;
	float		best;
	int			bestFace;
	int			opposite;
	face_t		*f;

	if (!QE_SingleBrush())
		return;

	b = selected_brushes.next;
	cb = BrushToCurveBlock(b);
	if (!cb) {
		return;
	}

	// set as detail and clear all curve flags
	for (f = b->brush_faces ; f ; f = f->next ) {
		f->texdef.flags &= ~SURF_KEEP;
		f->texdef.contents |= CONTENTS_DETAIL;
		if (negative) {
			f->texdef.contents |= CONTENTS_NEGATIVE_CURVE;
		} else {
			f->texdef.contents &= ~CONTENTS_NEGATIVE_CURVE;
		}
	}

	// find the top face
	best = MIN_WORLD_COORD;
	bestFace = 0;

	for (i = 0 ; i < 6 ; i++) {
		if (cb->faces[i].brushFace->plane.normal[2] > best) {
			best = cb->faces[i].brushFace->plane.normal[2];
			bestFace = i;
		}
	}

	cf = &cb->faces[bestFace];
	if (top) {
		cf->brushFace->texdef.flags |= SURF_CURVE;
	}
	if (bottom) {
		opposite = OppositeFace(cb, bestFace);
		cb->faces[opposite].brushFace->texdef.flags |= SURF_CURVE;
	}
	if (s1) {
		cb->faces[cf->neighbors[0]].brushFace->texdef.flags |= SURF_CURVE;
	}
	if (s2) {
		cb->faces[cf->neighbors[1]].brushFace->texdef.flags |= SURF_CURVE;
	}
	if (s3) {
		cb->faces[cf->neighbors[2]].brushFace->texdef.flags |= SURF_CURVE;
	}
	if (s4) {
		cb->faces[cf->neighbors[3]].brushFace->texdef.flags |= SURF_CURVE;
	}

	Curve_BuildPoints (b);
	Sys_UpdateWindows (W_ALL);

}


/*
================
Curve_AddFakePlanes

Call before saving the map to generate the extra clipping planes
the game uses for collision testing
================
*/
void Curve_AddFakePlanes( brush_t *b ) {
	curveBlock_t	*cb;

	// generate CURVE_FAKE faces
	bevelBrush = b;

	cb = BrushToCurveBlock(b);
	if ( !cb ) {
		b->curveBrush = false;
	  bevelBrush = NULL;
		return;
	}
	SubdivideCurveBlock (cb);

	bevelBrush = NULL;
}


/*
================
Curve_StripFakePlanes

Strips out any fakeplanes
================
*/
void Curve_StripFakePlanes( brush_t *b ) {
	face_t		*f;
	face_t		**ptr; //, **next;

	// remove any CURVE_FAKE faces
	for ( ptr = &b->brush_faces ; *ptr ; ) {
		f = *ptr;
		if ( f->texdef.flags & SURF_CURVE_FAKE ) {
			*ptr = f->next;
			Face_Free( f );
		} else {
			ptr = &f->next;
		}
	}
}


/*
================
Curve_BuildPoints

================
*/
void Curve_BuildPoints (brush_t *b) {
	face_t		*f;

	b->curveBrush = false;
	for (f=b->brush_faces ; f ; f=f->next) {
		if (f->texdef.flags & SURF_CURVE) {
			b->curveBrush = true;
			break;
		}
	}
	if (!b->curveBrush) {
		return;
	}

	// FIXME: build display list here?
}

//=================================================================


/*
===============
Curve_CameraDraw
===============
*/
void Curve_CameraDraw (brush_t *b) {
	curveBlock_t	*cb;

	cb = BrushToCurveBlock(b);
	if (!cb) {
		b->curveBrush = false;
		return;
	}

  g_bCamPaint = true;
	qglColor3f (1,1,1);
	SubdivideCurveBlock (cb);
}


/*
===============
Curve_XYDraw
===============
*/
void Curve_XYDraw (brush_t *b) {
	curveBlock_t	*cb;

	cb = BrushToCurveBlock(b);
	if (!cb) {
		b->curveBrush = false;
		return;
	}

  g_bCamPaint = false;
	qglPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	SubdivideCurveBlock (cb);
	qglPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
}

//================================================================


void Write1DMatrix (FILE *f, int x, float *m) {
	int		i;

	fprintf (f, "( ");
	for (i = 0 ; i < x ; i++) {
		if (m[i] == (int)m[i] ) {
			fprintf (f, "%i ", (int)m[i]);
		} else {
			fprintf (f, "%f ", m[i]);
		}
	}
	fprintf (f, ")");
}

void Write2DMatrix (FILE *f, int y, int x, float *m) {
	int		i;

	fprintf (f, "( ");
	for (i = 0 ; i < y ; i++) {
		Write1DMatrix (f, x, m + i*x);
		fprintf (f, " ");
	}
	fprintf (f, ")\n");
}


void Write3DMatrix (FILE *f, int z, int y, int x, float *m) {
	int		i;

	fprintf (f, "(\n");
	for (i = 0 ; i < z ; i++) {
		Write2DMatrix (f, y, x, m + i*(x*y) );
	}
	fprintf (f, ")\n");
}

void Write1DMatrix (CMemFile *f, int x, float *m) {
	int		i;

	MemFile_fprintf (f, "( ");
	for (i = 0 ; i < x ; i++) {
		if (m[i] == (int)m[i] ) {
			MemFile_fprintf (f, "%i ", (int)m[i]);
		} else {
			MemFile_fprintf (f, "%f ", m[i]);
		}
	}
	MemFile_fprintf (f, ")");
}

void Write2DMatrix (CMemFile *f, int y, int x, float *m) {
	int		i;

	MemFile_fprintf (f, "( ");
	for (i = 0 ; i < y ; i++) {
		Write1DMatrix (f, x, m + i*x);
		MemFile_fprintf (f, " ");
	}
	MemFile_fprintf (f, ")\n");
}


void Write3DMatrix (CMemFile *f, int z, int y, int x, float *m) {
	int		i;

	MemFile_fprintf (f, "(\n");
	for (i = 0 ; i < z ; i++) {
		Write2DMatrix (f, y, x, m + i*(x*y) );
	}
	MemFile_fprintf (f, ")\n");
}



/*
===============
Curve_WriteFile
===============
*/
void Curve_WriteFile (char *name) {
	char	curveName[1024];
	brush_t	*b;
	curveBlock_t	*cb;
	time_t	ltime;

	strcpy(curveName, name);
	StripExtension (curveName);
	strcat (curveName, ".bnd");
	curveFile = fopen(curveName, "w");
	if (!curveFile) {
		return;
	}

	time(&ltime);
	fprintf (curveFile, "// %s saved on %s\n", name, ctime(&ltime) );

	for (b=world_entity->brushes.onext ; b != &world_entity->brushes 
		; b=b->onext) {
		if (!b->curveBrush) {
			continue;		// only write curve brushes
		}

		cb = BrushToCurveBlock(b);
		SubdivideCurveBlock (cb);

	}
	fclose (curveFile);

	curveFile = NULL;
}


#if 0
void Curve_BevelBrush( brush_t *b ) {
	curveBlock_t	*cb;
	face_t			*f;

	// make a copy without any curve flags, but keeping the negative flag
	bevelBrush = Brush_Clone( b );
	for (f = bevelBrush->brush_faces ; f ; f = f->next) {
		f->texdef.flags &= ~SURF_CURVE;
	}
	bevelBrush->curveBrush = false;

	cb = BrushToCurveBlock(b);
	SubdivideCurveBlock (cb);

	Brush_Build( bevelBrush );
	Brush_AddToList( bevelBrush, &active_brushes );
	Entity_LinkBrush( b->owner, bevelBrush );

	Sys_UpdateWindows (W_ALL);

	bevelBrush = NULL;
}
#endif
