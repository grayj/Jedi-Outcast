

#include "stdafx.h"
#include <assert.h>
#include "qe3.h"
#include "oddbits.h"
#include "groupnames.h"

#define MAX_POINTS_ON_WINDING	64

face_t *Face_Alloc( void );

winding_t	*NewWinding (int points);
void		FreeWinding (winding_t *w);
winding_t	*Winding_Clone( winding_t *w );
winding_t	*ClipWinding (winding_t *in, plane_t *split, qboolean keepon);

qboolean qbShowFaces = false;

void PrintWinding (winding_t *w)
{
	int		i;
	
	printf ("-------------\n");
	for (i=0 ; i<w->numpoints ; i++)
		printf ("(%5.2f, %5.2f, %5.2f)\n", w->points[i][0]
		, w->points[i][1], w->points[i][2]);
}

void PrintPlane (plane_t *p)
{
  printf ("(%5.2f, %5.2f, %5.2f) : %5.2f\n",  p->normal[0],  p->normal[1], 
  p->normal[2],  p->dist);
}

void PrintVector (vec3_t v)
{
  printf ("(%5.2f, %5.2f, %5.2f)\n",  v[0],  v[1], v[2]);
}


face_t	*Face_Clone (face_t *f)
{
	face_t	*n;

	n = Face_Alloc();
	n->texdef = f->texdef;
	memcpy (n->planepts, f->planepts, sizeof(n->planepts));

	// all other fields are derived, and will be set by Brush_Build
	return n;
}

//============================================================================





/*
==================
NewWinding
==================
*/
winding_t *NewWinding (int points)
{
	winding_t	*w;
	int			size;
	
	if (points > MAX_POINTS_ON_WINDING)
		Error ("NewWinding: %i points", points);
	
	size = (int)((winding_t *)0)->points[points];
	w = (winding_t*) malloc (size);
	memset (w, 0, size);
	w->maxpoints = points;
	
	return w;
}


void FreeWinding (winding_t *w)
{
	free (w);
}


/*
==================
Winding_Clone
==================
*/
winding_t *Winding_Clone(winding_t *w)
{
	int			size;
	winding_t	*c;
	
	size = (int)((winding_t *)0)->points[w->numpoints];
	c = (winding_t*)qmalloc (size);
	memcpy (c, w, size);
	return c;
}


/*
==================
ClipWinding

Clips the winding to the plane, returning the new winding on the positive side
Frees the input winding.
If keepon is true, an exactly on-plane winding will be saved, otherwise
it will be clipped away.
==================
*/
winding_t *ClipWinding (winding_t *in, plane_t *split, qboolean keepon)
{
	vec_t	dists[MAX_POINTS_ON_WINDING];
	int		sides[MAX_POINTS_ON_WINDING];
	int		counts[3];
	vec_t	dot;
	int		i, j;
	vec_t	*p1, *p2;
	vec3_t	mid;
	winding_t	*neww;
	int		maxpts;
	
	counts[0] = counts[1] = counts[2] = 0;

// determine sides for each point
	for (i=0 ; i<in->numpoints ; i++)
	{
		dot = DotProduct (in->points[i], split->normal);
		dot -= split->dist;
		dists[i] = dot;
		if (dot > ON_EPSILON)
			sides[i] = SIDE_FRONT;
		else if (dot < -ON_EPSILON)
			sides[i] = SIDE_BACK;
		else
		{
			sides[i] = SIDE_ON;
		}
		counts[sides[i]]++;
	}
	sides[i] = sides[0];
	dists[i] = dists[0];
	
	if (keepon && !counts[0] && !counts[1])
		return in;
		
	if (!counts[0])
	{
		FreeWinding (in);
		return NULL;
	}
	if (!counts[1])
		return in;
	
	maxpts = in->numpoints+4;	// can't use counts[0]+2 because
								// of fp grouping errors
	neww = NewWinding (maxpts);
		
	for (i=0 ; i<in->numpoints ; i++)
	{
		p1 = in->points[i];
		
		if (sides[i] == SIDE_ON)
		{
			VectorCopy (p1, neww->points[neww->numpoints]);
			neww->numpoints++;
			continue;
		}
	
		if (sides[i] == SIDE_FRONT)
		{
			VectorCopy (p1, neww->points[neww->numpoints]);
			neww->numpoints++;
		}
		
		if (sides[i+1] == SIDE_ON || sides[i+1] == sides[i])
			continue;
			
	// generate a split point
		p2 = in->points[(i+1)%in->numpoints];
		
		dot = dists[i] / (dists[i]-dists[i+1]);
		for (j=0 ; j<3 ; j++)
		{	// avoid round off error when possible
			if (split->normal[j] == 1)
				mid[j] = split->dist;
			else if (split->normal[j] == -1)
				mid[j] = -split->dist;
			else
				mid[j] = p1[j] + dot*(p2[j]-p1[j]);
		}
			
		VectorCopy (mid, neww->points[neww->numpoints]);
		neww->numpoints++;
	}
	
	if (neww->numpoints > maxpts)
		Error ("ClipWinding: points exceeded estimate");
		
// free the original winding
	FreeWinding (in);
	
	return neww;
}



/*
=============================================================================

			TEXTURE COORDINATES

=============================================================================
*/


/*
==================
textureAxisFromPlane
==================
*/
vec3_t	baseaxis[18] =
{
{0,0,1}, {1,0,0}, {0,-1,0},			// floor
{0,0,-1}, {1,0,0}, {0,-1,0},		// ceiling
{1,0,0}, {0,1,0}, {0,0,-1},			// west wall
{-1,0,0}, {0,1,0}, {0,0,-1},		// east wall
{0,1,0}, {1,0,0}, {0,0,-1},			// south wall
{0,-1,0}, {1,0,0}, {0,0,-1}			// north wall
};

void TextureAxisFromPlane(plane_t *pln, vec3_t xv, vec3_t yv)
{
	int		bestaxis;
	float	dot,best;
	int		i;
	
	best = 0;
	bestaxis = 0;
	
	for (i=0 ; i<6 ; i++)
	{
		dot = DotProduct (pln->normal, baseaxis[i*3]);
		if (dot > (best + 0.0001))
		{
			best = dot;
			bestaxis = i;
		}
	}
	
	VectorCopy (baseaxis[bestaxis*3+1], xv);
	VectorCopy (baseaxis[bestaxis*3+2], yv);
}



float	lightaxis[3] = {0.6, 0.8, 1.0};
/*
================
SetShadeForPlane

Light different planes differently to
improve recognition
================
*/
float SetShadeForPlane (plane_t *p)
{
	int		i;
	float	f;

	// axial plane
	for (i=0 ; i<3 ; i++)
		if (fabs(p->normal[i]) > 0.9)
		{
			f = lightaxis[i];
			return f;
		}

	// between two axial planes
	for (i=0 ; i<3 ; i++)
		if (fabs(p->normal[i]) < 0.1)
		{
			f = (lightaxis[(i+1)%3] + lightaxis[(i+2)%3])/2;
			return f;
		}

	// other
	f= (lightaxis[0] + lightaxis[1] + lightaxis[2]) / 3;
	return f;
}

vec3_t  vecs[2];
float	shift[2];

/*
================
SetFaceColor
================
*/
void SetFaceColor (brush_t *b, face_t *f, float fCurveColor) 
{
	float	shade;
	qtexture_t *q;

	q = f->d_texture;

	// set shading for face
  shade = SetShadeForPlane (&f->plane);
  if (g_pParentWnd->GetCamera()->Camera().draw_mode == cd_texture && !b->owner->eclass->fixedsize)
	{
    //if (b->curveBrush)
    //  shade = fCurveColor;
    f->d_color[0] = 
		f->d_color[1] = 
		f->d_color[2] = shade;
	}
	else
	{
		f->d_color[0] = shade*q->color[0];
		f->d_color[1] = shade*q->color[1];
		f->d_color[2] = shade*q->color[2];
	}
}

/*
================
FaceTextureVectors
================
*/
void FaceTextureVectors (face_t *f, float STfromXYZ[2][4])
{
	vec3_t		pvecs[2];
	int			sv, tv;
	float		ang, sinv, cosv;
	float		ns, nt;
	int			i,j;
	qtexture_t *q;
	texdef_t	*td;

	td = &f->texdef;
	q = f->d_texture;

	memset (STfromXYZ, 0, 8*sizeof(float));

	if (!td->scale[0])
		td->scale[0] = (g_PrefsDlg.m_bHiColorTextures) ? fTEXTURE_SCALE : 1;
	if (!td->scale[1])
		td->scale[1] = (g_PrefsDlg.m_bHiColorTextures) ? fTEXTURE_SCALE : 1;

	// get natural texture axis
	TextureAxisFromPlane(&f->plane, pvecs[0], pvecs[1]);

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

	if (pvecs[0][0])
		sv = 0;
	else if (pvecs[0][1])
		sv = 1;
	else
		sv = 2;
				
	if (pvecs[1][0])
		tv = 0;
	else if (pvecs[1][1])
		tv = 1;
	else
		tv = 2;
					
	for (i=0 ; i<2 ; i++) {
		ns = cosv * pvecs[i][sv] - sinv * pvecs[i][tv];
		nt = sinv * pvecs[i][sv] +  cosv * pvecs[i][tv];
		STfromXYZ[i][sv] = ns;
		STfromXYZ[i][tv] = nt;
	}

	// scale
	for (i=0 ; i<2 ; i++)
		for (j=0 ; j<3 ; j++)
			STfromXYZ[i][j] = STfromXYZ[i][j] / td->scale[i];

	// shift
	STfromXYZ[0][3] = td->shift[0];
	STfromXYZ[1][3] = td->shift[1];

	for (j=0 ; j<4 ; j++) {
		STfromXYZ[0][j] /= q->width;
		STfromXYZ[1][j] /= q->height;
	}
}



void EmitTextureCoordinates ( float *xyzst, qtexture_t *q, face_t *f)
{
	float	STfromXYZ[2][4];

	FaceTextureVectors (f,  STfromXYZ);
	xyzst[3] = DotProduct (xyzst, STfromXYZ[0]) + STfromXYZ[0][3];
	xyzst[4] = DotProduct (xyzst, STfromXYZ[1]) + STfromXYZ[1][3];

}


//==========================================================================


/*
=================
BasePolyForPlane
=================
*/
winding_t *BasePolyForPlane (plane_t *p)
{
	int		i, x;
	vec_t	max, v;
	vec3_t	org, vright, vup;
	winding_t	*w;
	
// find the major axis

	max = MIN_WORLD_COORD;
	x = -1;
	for (i=0 ; i<3; i++)
	{
		v = fabs(p->normal[i]);
		if (v > max)
		{
			x = i;
			max = v;
		}
	}
	if (x==-1)
		Error ("BasePolyForPlane: no axis found");
		
	VectorCopy (vec3_origin, vup);	
	switch (x)
	{
	case 0:
	case 1:
		vup[2] = 1;
		break;		
	case 2:
		vup[0] = 1;
		break;		
	}


	v = DotProduct (vup, p->normal);
	VectorMA (vup, -v, p->normal, vup);
	VectorNormalize (vup);
		
	VectorScale (p->normal, p->dist, org);
	
	CrossProduct (vup, p->normal, vright);
	
	VectorScale (vup, MAX_WORLD_COORD, vup);
	VectorScale (vright, MAX_WORLD_COORD, vright);

// project a really big	axis aligned box onto the plane
	w = NewWinding (4);
	
	VectorSubtract (org, vright, w->points[0]);
	VectorAdd (w->points[0], vup, w->points[0]);
	
	VectorAdd (org, vright, w->points[1]);
	VectorAdd (w->points[1], vup, w->points[1]);
	
	VectorAdd (org, vright, w->points[2]);
	VectorSubtract (w->points[2], vup, w->points[2]);
	
	VectorSubtract (org, vright, w->points[3]);
	VectorSubtract (w->points[3], vup, w->points[3]);
	
	w->numpoints = 4;
	
	return w;	
}

void Brush_MakeFacePlane (face_t *f) {
	int		j;
	vec3_t	t1, t2, t3;

// convert to a vector / dist plane
	for (j=0 ; j<3 ; j++)
	{
		t1[j] = f->planepts[0][j] - f->planepts[1][j];
		t2[j] = f->planepts[2][j] - f->planepts[1][j];
		t3[j] = f->planepts[1][j];
	}
	
	CrossProduct(t1,t2, f->plane.normal);
	if (VectorCompare (f->plane.normal, vec3_origin))
		printf ("WARNING: brush plane with no normal\n");
	VectorNormalize (f->plane.normal);
	f->plane.dist = DotProduct (t3, f->plane.normal);
}

void Brush_MakeFacePlanes (brush_t *b)
{
	face_t	*f;

	for (f=b->brush_faces ; f ; f=f->next)
	{
		Brush_MakeFacePlane (f);
	}
}


void DrawBrushEntityName (brush_t *b)
{
	char	*name;
	//float	a, s, c;
	//vec3_t	mid;
	//int		i;

	if (!b->owner)
		return;		// during contruction

	if (b->owner == world_entity)
		return;

	if (b != b->owner->brushes.onext)
		return;	// not key brush

// MERGEME
#if 0
	if (!(g_qeglobals.d_savedinfo.exclude & EXCLUDE_ANGLES))
	{
		// draw the angle pointer
		a = FloatForKey (b->owner, "angle");
		if (a)
		{
			s = sin (a/180*Q_PI);
			c = cos (a/180*Q_PI);
			for (i=0 ; i<3 ; i++)
				mid[i] = (b->mins[i] + b->maxs[i])*0.5; 

			qglBegin (GL_LINE_STRIP);
			qglVertex3fv (mid);
			mid[0] += c*8;
			mid[1] += s*8;
			mid[2] += s*8;
			qglVertex3fv (mid);
			mid[0] -= c*4;
			mid[1] -= s*4;
			mid[2] -= s*4;
			mid[0] -= s*4;
			mid[1] += c*4;
			mid[2] += c*4;
			qglVertex3fv (mid);
			mid[0] += c*4;
			mid[1] += s*4;
			mid[2] += s*4;
			mid[0] += s*4;
			mid[1] -= c*4;
			mid[2] -= c*4;
			qglVertex3fv (mid);
			mid[0] -= c*4;
			mid[1] -= s*4;
			mid[2] -= s*4;
			mid[0] += s*4;
			mid[1] -= c*4;
			mid[2] -= c*4;
			qglVertex3fv (mid);
			qglEnd ();
		}
	}
#endif


	// slightly odd code for SHOW_GROUPNAMES I know...
	//
	if (g_qeglobals.d_savedinfo.show_names)
	{
		name = ValueForKey (b->owner, "classname");

		if (g_qeglobals.d_savedinfo.exclude & SHOW_GROUPNAMES)
		{
			char *psGroupName = ValueForKey (b->owner, sKEYFIELD_GROUPNAME);

			if (strlen(psGroupName))
			{
				name = va("%s   ( %s )",name,psGroupName);
			}
		}

		qglRasterPos3f (b->mins[0]+4, b->mins[1]+4, b->mins[2]+4);
		qglCallLists (strlen(name), GL_UNSIGNED_BYTE, name);

#ifdef QUAKE3
		if (strnicmp(name,"waypoint",8)==0)
		{
			name = ValueForKey (b->owner, "targetname");
			if (name && strlen(name))	// just being safe....
			{
				CString name2;
				name2.Format("( %s )",name);
				qglRasterPos3f (b->mins[0]+4, b->mins[1]+4-10, b->mins[2]+4);
				qglCallLists (strlen(name2), GL_UNSIGNED_BYTE, name2);
			}
		}
#endif
	}
	else
	{	
		// (this is deliberately not subservient to the above bool)
		//
		if (g_qeglobals.d_savedinfo.exclude & SHOW_GROUPNAMES)
		{
			name = ValueForKey (b->owner, sKEYFIELD_GROUPNAME);

			if (strlen(name))
			{
				name = va("( Group: \"%s\" )",name);

				qglRasterPos3f (b->mins[0]+4, b->mins[1]+4, b->mins[2]+4);
				qglCallLists (strlen(name), GL_UNSIGNED_BYTE, name);
			}
		}
	}
}

/*
=================
MakeFaceWinding

returns the visible polygon on a face
=================
*/
winding_t	*MakeFaceWinding (brush_t *b, face_t *face)
{
	winding_t	*w;
	face_t		*clip;
	plane_t			plane;
	qboolean		past;

	// get a poly that covers an effectively infinite area
	w = BasePolyForPlane (&face->plane);

	// chop the poly by all of the other faces
	past = false;
	for (clip = b->brush_faces ; clip && w ; clip=clip->next)
	{
		if (clip == face)
		{
			past = true;
			continue;
		}
		if (DotProduct (face->plane.normal, clip->plane.normal) > 0.999
			&& fabs(face->plane.dist - clip->plane.dist) < 0.01 )
		{	// identical plane, use the later one
			if (past)
			{
				free (w);
				return NULL;
			}
			continue;
		}

		// flip the plane, because we want to keep the back side
		VectorSubtract (vec3_origin,clip->plane.normal, plane.normal);
		plane.dist = -clip->plane.dist;
		
		w = ClipWinding (w, &plane, false);
		if (!w)
			return w;
	}
	
	if (w->numpoints < 3)
	{
		free(w);
		w = NULL;
	}

	if (!w)
		printf ("unused plane\n");

	return w;
}


void Brush_SnapPlanepts (brush_t *b)
{
	int		i, j;
	face_t	*f;

  if (g_PrefsDlg.m_bNoClamp)
    return;

	for (f=b->brush_faces ; f; f=f->next)
		for (i=0 ; i<3 ; i++)
			for (j=0 ; j<3 ; j++)
				f->planepts[i][j] = floor (f->planepts[i][j] + 0.5);
}
	
/*
** Brush_Build
**
** Builds a brush rendering data and also sets the min/max bounds
*/
#define	ZERO_EPSILON	0.001
void Brush_Build( brush_t *b, bool bSnap, bool bMarkMap )
{
//	int				order;
//	face_t			*face;
//	winding_t		*w;


	/*
	** build the windings and generate the bounding box
	*/
	Brush_BuildWindings(b, bSnap);

  Patch_BuildPoints (b);
/*
  b->alphaBrush = false;
	for (face_t *f=b->brush_faces ; f; f=f->next)
  {
    if (f->texdef.flags & SURF_ALPHA)
    {
      b->alphaBrush = true;
      break;
    }
  }
*/

	/*
	** move the points and edges if in select mode
	*/
	if (g_qeglobals.d_select_mode == sel_vertex || g_qeglobals.d_select_mode == sel_edge)
		SetupVertexSelection ();

  if (bMarkMap)
  {
    Sys_MarkMapModified();
  }
}

/*
=================
Brush_Parse

The brush is NOT linked to any list
=================
*/
brush_t *Brush_Parse (void)
{
	brush_t		*b;
	face_t		*f;
	int			i,j;

	g_qeglobals.d_parsed_brushes++;
	b = (brush_t*)qmalloc(sizeof(brush_t));
		
	do
	{
		if (!GetToken (true))
			break;
		if (!strcmp (token, "}") )
			break;

    if (strcmpi(token, "patchDef2") == 0 || strcmpi(token, "patchDef3") == 0)
    {
      free (b);

      // double string compare but will go away soon
      b = Patch_Parse(strcmpi(token, "patchDef2") == 0);
      if (b == NULL)
      {
		    Warning ("parsing patch/brush");
        return NULL;
      }
      else
      {
        continue;
      }
      // handle inline patch
    }
    else
    {
		  f = Face_Alloc();

		  // add the brush to the end of the chain, so
		// loading and saving a map doesn't reverse the order

		  f->next = NULL;
  		if (!b->brush_faces)
	  	{
		  	b->brush_faces = f;
  		}
	  	else
		  {
			  face_t *scan;
			  for (scan=b->brush_faces ; scan->next ; scan=scan->next)
				  ;
  			scan->next = f;
	  	}

		  // read the three point plane definition
  		for (i=0 ; i<3 ; i++)
	  	{
		  	if (i != 0)
			  	GetToken (true);
  			if (strcmp (token, "(") )
        {
	  			Warning ("parsing brush");
          return NULL;
        }
			
		  	for (j=0 ; j<3 ; j++)
			  {
				  GetToken (false);
  				f->planepts[i][j] = atof(token);
	  		}
			
		  	GetToken (false);
			  if (strcmp (token, ")") )
        {
				  Warning ("parsing brush");
          return NULL;
        }
  		}
    }

	// read the texturedef
		GetToken (false);
		strcpy(f->texdef.name, token);
		GetToken (false);
		f->texdef.shift[0] = atoi(token);
		GetToken (false);
		f->texdef.shift[1] = atoi(token);
		GetToken (false);
		f->texdef.rotate = atoi(token);	
		GetToken (false);
		f->texdef.scale[0] = atof(token);
		GetToken (false);
		f->texdef.scale[1] = atof(token);


		// the flags and value field aren't necessarily present  (but all new maps saved in this version *do* write them)
		f->d_texture = Texture_ForName( f->texdef.name );
		f->texdef.flags = f->d_texture->flags;
		f->texdef.value = f->d_texture->value;
		f->texdef.contents = f->d_texture->contents;

		if (TokenAvailable ())
		{
			GetToken (false);
			f->texdef.contents = atoi(token);
			GetToken (false);
			f->texdef.flags = atoi(token);
			GetToken (false);
			f->texdef.value = atoi(token);
		}

#ifdef SOF
		// optional fields here depending on flags...
		//
		if(f->texdef.flags & 0x400) // tall wall, read lighting values
		{
			if (TokenAvailable())
			{
				GetToken (false);
				f->texdef.lighting[0] = atof(token);	// red
				GetToken (false);
				f->texdef.lighting[1] = atof(token);	// green
				GetToken (false);
				f->texdef.lighting[2] = atof(token);	// blue
				GetToken (false);
				f->texdef.lighting[3] = atof(token);	// alpha
			}
			else
			{
				Error ("Brush_Parse: Expecting tall-wall lighting values, but none found!");
			}
		}
#endif

	} while (1);


	return b;
}

/*
=================
Brush_Write
=================
*/
void Brush_Write (brush_t *b, FILE *f)
{
	face_t	*fa;
	char *pname;
	int		i;

  if (b->patchBrush)
  {
    Patch_Write(b->nPatchID, f);
    return;
  }
	fprintf (f, "{\n");
	for (fa=b->brush_faces ; fa ; fa=fa->next)
	{
    if (g_PrefsDlg.m_bNoClamp)
    {
		  for (i=0 ; i<3 ; i++)
      {
        fprintf(f, "( ");
        for (int j = 0; j < 3; j++)
        {
          if (fa->planepts[i][j] == static_cast<int>(fa->planepts[i][j]))
            fprintf(f, "%i ", static_cast<int>(fa->planepts[i][j]));
          else
            fprintf(f, "%f ", fa->planepts[i][j]);
        }
        fprintf(f, ") ");
      }
    }
    else
    {
		  for (i=0 ; i<3 ; i++)
      {
			  fprintf (f, "( %i %i %i ) ", (int)fa->planepts[i][0] , (int)fa->planepts[i][1], (int)fa->planepts[i][2]);
      }
    }
		pname = fa->texdef.name;
		if (pname[0] == 0)
			pname = "unnamed";

		fprintf (f, "%s %i %i %i ", 
#ifdef QUAKE3 
									strlwr
#endif
									(pname),
			(int)fa->texdef.shift[0], (int)fa->texdef.shift[1],
			(int)fa->texdef.rotate);

		if (fa->texdef.scale[0] == (int)fa->texdef.scale[0])
			fprintf (f, "%i ", (int)fa->texdef.scale[0]);
		else
			fprintf (f, "%f ", (float)fa->texdef.scale[0]);
		if (fa->texdef.scale[1] == (int)fa->texdef.scale[1])
			fprintf (f, "%i", (int)fa->texdef.scale[1]);
		else
			fprintf (f, "%f", (float)fa->texdef.scale[1]);

		// only output flags and value if not default
    // KPRadiant (and any tga setup always needs to write them unless the build tools read the ini file
#if 1
    //if (!(g_pParentWnd->GetPlugInMgr().GetTextureInfo() && g_pParentWnd->GetPlugInMgr().GetTextureInfo()->m_bHalfLife))
    //{
		  fprintf (f, " %i %i %i", fa->texdef.contents, fa->texdef.flags, fa->texdef.value);
    //}
#else
    bool bFlagsOut = false;
		if (!fa->d_texture || fa->texdef.value != fa->d_texture->value
			|| fa->texdef.flags != fa->d_texture->flags
			|| fa->texdef.contents != fa->d_texture->contents)
		{
			fprintf (f, " %i %i %i", fa->texdef.contents, fa->texdef.flags, fa->texdef.value);
      bFlagsOut = true;
		}
#endif

#ifdef SOF
		if(fa->texdef.flags & 0x400) // tall wall, write lighting values
		{
			fprintf (f, " %f %f %f %f", fa->texdef.lighting[0], fa->texdef.lighting[1], fa->texdef.lighting[2], fa->texdef.lighting[3]);
		}
#endif


		fprintf (f, "\n");
	}
	fprintf (f, "}\n");

}



/*
=================
Brush_Write to a CMemFile*
=================
*/
void Brush_Write (brush_t *b, CMemFile *pMemFile)
{
	face_t	*fa;
	char *pname;
	int		i;

  if (b->patchBrush)
  {
    Patch_Write(b->nPatchID, pMemFile);
    return;
  }

	MemFile_fprintf (pMemFile, "{\n");
	for (fa=b->brush_faces ; fa ; fa=fa->next)
	{
    if (g_PrefsDlg.m_bNoClamp)
    {
		  for (i=0 ; i<3 ; i++)
      {
        MemFile_fprintf (pMemFile, "( ");
        for (int j = 0; j < 3; j++)
        {
          if (fa->planepts[i][j] == static_cast<int>(fa->planepts[i][j]))
            MemFile_fprintf (pMemFile, "%i ", static_cast<int>(fa->planepts[i][j]));
          else
            MemFile_fprintf (pMemFile, "%f ", fa->planepts[i][j]);
        }
        MemFile_fprintf (pMemFile, ") ");
      }
    }
    else
    {
		  for (i=0 ; i<3 ; i++)
      {
			  MemFile_fprintf (pMemFile, "( %i %i %i ) ", (int)fa->planepts[i][0]
			  , (int)fa->planepts[i][1], (int)fa->planepts[i][2]);
      }
    }
		pname = fa->texdef.name;
		if (pname[0] == 0)
			pname = "unnamed";

		MemFile_fprintf (pMemFile, "%s %i %i %i ", pname,
			(int)fa->texdef.shift[0], (int)fa->texdef.shift[1],
			(int)fa->texdef.rotate);

		if (fa->texdef.scale[0] == (int)fa->texdef.scale[0])
			MemFile_fprintf (pMemFile, "%i ", (int)fa->texdef.scale[0]);
		else
			MemFile_fprintf (pMemFile, "%f ", (float)fa->texdef.scale[0]);
		if (fa->texdef.scale[1] == (int)fa->texdef.scale[1])
			MemFile_fprintf (pMemFile, "%i", (int)fa->texdef.scale[1]);
		else
			MemFile_fprintf (pMemFile, "%f", (float)fa->texdef.scale[1]);

		// only output flags and value if not default
    bool bFlagsOut = false;
		if (fa->texdef.value != fa->d_texture->value
			|| fa->texdef.flags != fa->d_texture->flags
			|| fa->texdef.contents != fa->d_texture->contents)
		{
			MemFile_fprintf (pMemFile, " %i %i %i", fa->texdef.contents, fa->texdef.flags, fa->texdef.value);
      bFlagsOut = true;
		}

#ifdef SOF
		if(fa->texdef.flags & 0x400) // tall wall, write lighting values
		{
			MemFile_fprintf (pMemFile, " %f %f %f %f", fa->texdef.lighting[0], fa->texdef.lighting[1], fa->texdef.lighting[2], fa->texdef.lighting[3]);
		}
#endif


		MemFile_fprintf (pMemFile, "\n");
	}
	MemFile_fprintf (pMemFile, "}\n");
}

/*
=============
Brush_Create

Create non-textured blocks for entities
The brush is NOT linked to any list
=============
*/
brush_t	*Brush_Create (vec3_t mins, vec3_t maxs, texdef_t *texdef)
{
	int		i, j;
	vec3_t	pts[4][2];
	face_t	*f;
	brush_t	*b;

	for (i=0 ; i<3 ; i++)
		if (maxs[i] < mins[i])
			Error ("Brush_InitSolid: backwards");

	b = (brush_t*)qmalloc (sizeof(brush_t));
	
	pts[0][0][0] = mins[0];
	pts[0][0][1] = mins[1];
	
	pts[1][0][0] = mins[0];
	pts[1][0][1] = maxs[1];
	
	pts[2][0][0] = maxs[0];
	pts[2][0][1] = maxs[1];
	
	pts[3][0][0] = maxs[0];
	pts[3][0][1] = mins[1];
	
	for (i=0 ; i<4 ; i++)
	{
		pts[i][0][2] = mins[2];
		pts[i][1][0] = pts[i][0][0];
		pts[i][1][1] = pts[i][0][1];
		pts[i][1][2] = maxs[2];
	}


	for (i=0 ; i<4 ; i++)
	{
		f = Face_Alloc();
		f->texdef = *texdef;
		f->texdef.flags &= ~SURF_KEEP;
		f->texdef.contents &= ~CONTENTS_KEEP;
		f->next = b->brush_faces;
		b->brush_faces = f;
		j = (i+1)%4;

		VectorCopy (pts[j][1], f->planepts[0]);
		VectorCopy (pts[i][1], f->planepts[1]);
		VectorCopy (pts[i][0], f->planepts[2]);
	}
	
	f = Face_Alloc();
	f->texdef = *texdef;
  f->texdef.flags &= ~SURF_KEEP;
	f->texdef.contents &= ~CONTENTS_KEEP;
	f->next = b->brush_faces;
	b->brush_faces = f;

	VectorCopy (pts[0][1], f->planepts[0]);
	VectorCopy (pts[1][1], f->planepts[1]);
	VectorCopy (pts[2][1], f->planepts[2]);

	f = Face_Alloc();
	f->texdef = *texdef;
  f->texdef.flags &= ~SURF_KEEP;
	f->texdef.contents &= ~CONTENTS_KEEP;
	f->next = b->brush_faces;
	b->brush_faces = f;

	VectorCopy (pts[2][0], f->planepts[0]);
	VectorCopy (pts[1][0], f->planepts[1]);
	VectorCopy (pts[0][0], f->planepts[2]);

	return b;
}

/*
=============
Brush_CreatePyramid

Create non-textured pyramid for light entities
The brush is NOT linked to any list
=============
*/
brush_t	*Brush_CreatePyramid (vec3_t mins, vec3_t maxs, texdef_t *texdef)
{
  return Brush_Create(mins, maxs, texdef);

	for (int i=0 ; i<3 ; i++)
		if (maxs[i] < mins[i])
			Error ("Brush_InitSolid: backwards");

	brush_t* b = (brush_t*)qmalloc (sizeof(brush_t));

  vec3_t corners[4];

  float fMid = Q_rint(mins[2] + (Q_rint((maxs[2] - mins[2]) / 2)));

  corners[0][0] = mins[0];
  corners[0][1] = mins[1];
  corners[0][2] = fMid;

  corners[1][0] = mins[0];
  corners[1][1] = maxs[1];
  corners[1][2] = fMid;

  corners[2][0] = maxs[0];
  corners[2][1] = maxs[1];
  corners[2][2] = fMid;

  corners[3][0] = maxs[0];
  corners[3][1] = mins[1];
  corners[3][2] = fMid;

  vec3_t top, bottom;

  top[0] = Q_rint(mins[0] + ((maxs[0] - mins[0]) / 2));
  top[1] = Q_rint(mins[1] + ((maxs[1] - mins[1]) / 2));
  top[2] = Q_rint(maxs[2]);

  VectorCopy(top, bottom);
  bottom[2] = mins[2];

  // sides
  for (i = 0; i < 4; i++)
  {
	  face_t* f = Face_Alloc();
	  f->texdef = *texdef;
    f->texdef.flags &= ~SURF_KEEP;
	  f->texdef.contents &= ~CONTENTS_KEEP;
	  f->next = b->brush_faces;
	  b->brush_faces = f;
		int j = (i+1)%4;

    VectorCopy (top, f->planepts[0]);
    VectorCopy (corners[i], f->planepts[1]);
    VectorCopy(corners[j], f->planepts[2]);

	  f = Face_Alloc();
	  f->texdef = *texdef;
    f->texdef.flags &= ~SURF_KEEP;
	  f->texdef.contents &= ~CONTENTS_KEEP;
	  f->next = b->brush_faces;
	  b->brush_faces = f;

    VectorCopy (bottom, f->planepts[2]);
    VectorCopy (corners[i], f->planepts[1]);
    VectorCopy(corners[j], f->planepts[0]);

  }

  return b;
}




/*
=============
Brush_MakeSided

Makes the current brushhave the given number of 2d sides
=============
*/
void Brush_MakeSided (int sides)
{
	int		i;
	vec3_t	mins, maxs;
	brush_t	*b;
	texdef_t	*texdef;
	face_t	*f;
	vec3_t	mid;
	float	width;
	float	sv, cv;

	if (sides < 3)
	{
		Sys_Status ("Bad sides number", 0);
		return;
	}

	if (!QE_SingleBrush ())
	{
		Sys_Status ("Must have a single brush selected", 0 );
		return;
	}

	b = selected_brushes.next;
	VectorCopy (b->mins, mins);
	VectorCopy (b->maxs, maxs);
	texdef = &g_qeglobals.d_texturewin.texdef;

	Brush_Free (b);

	// find center of brush
	width = 8;
	for (i=0 ; i<2 ; i++)
	{
		mid[i] = (maxs[i] + mins[i])*0.5;
		if (maxs[i] - mins[i] > width)
			width = maxs[i] - mins[i];
	}
	width /= 2;

	b = (brush_t*)qmalloc (sizeof(brush_t));
		
	// create top face
	f = Face_Alloc();
	f->texdef = *texdef;
	f->next = b->brush_faces;
	b->brush_faces = f;

  f->planepts[2][0] = mins[0];f->planepts[2][1] = mins[1];f->planepts[2][2] = maxs[2];
  f->planepts[1][0] = maxs[0];f->planepts[1][1] = mins[1];f->planepts[1][2] = maxs[2];
  f->planepts[0][0] = maxs[0];f->planepts[0][1] = maxs[1];f->planepts[0][2] = maxs[2];

	// create bottom face
	f = Face_Alloc();
	f->texdef = *texdef;
	f->next = b->brush_faces;
	b->brush_faces = f;

f->planepts[0][0] = mins[0];f->planepts[0][1] = mins[1];f->planepts[0][2] = mins[2];
f->planepts[1][0] = maxs[0];f->planepts[1][1] = mins[1];f->planepts[1][2] = mins[2];
f->planepts[2][0] = maxs[0];f->planepts[2][1] = maxs[1];f->planepts[2][2] = mins[2];

	for (i=0 ; i<sides ; i++)
	{
		f = Face_Alloc();
		f->texdef = *texdef;
		f->next = b->brush_faces;
		b->brush_faces = f;

		sv = sin (i*3.14159265*2/sides);
		cv = cos (i*3.14159265*2/sides);

		f->planepts[0][0] = floor(mid[0]+width*cv+0.5);
		f->planepts[0][1] = floor(mid[1]+width*sv+0.5);
		f->planepts[0][2] = mins[2];

		f->planepts[1][0] = f->planepts[0][0];
		f->planepts[1][1] = f->planepts[0][1];
		f->planepts[1][2] = maxs[2];

		f->planepts[2][0] = floor(f->planepts[0][0] - width*sv + 0.5);
		f->planepts[2][1] = floor(f->planepts[0][1] + width*cv + 0.5);
		f->planepts[2][2] = maxs[2];

	}

	Brush_AddToList (b, &selected_brushes);

	Entity_LinkBrush (world_entity, b);

	Brush_Build( b );

	Sys_UpdateWindows (W_ALL);
}


/*
=============
Brush_Free

Frees the brush with all of its faces and display list.
Unlinks the brush from whichever chain it is in.
Decrements the owner entity's brushcount.
Removes owner entity if this was the last brush
unless owner is the world.
=============
*/
void Brush_Free (brush_t *b)
{
	face_t	*f, *next;

	// free faces
	for (f=b->brush_faces ; f ; f=next)
	{
		next = f->next;
		Face_Free( f );
	}

	// unlink from active/selected list
	if (b->next)
		Brush_RemoveFromList (b);

	// unlink from entity list
	if (b->onext)
		Entity_UnlinkBrush (b);

	free (b);
}


/*
============
Brush_Clone

Does NOT add the new brush to any lists
============
*/
brush_t *Brush_Clone (brush_t *b)
{
	brush_t	*n;
	face_t	*f, *nf;

	n = (brush_t*)qmalloc(sizeof(brush_t));
	n->owner = b->owner;
	for (f=b->brush_faces ; f ; f=f->next)
	{
		nf = Face_Clone( f );
		nf->next = n->brush_faces;
		n->brush_faces = nf;
	}
	return n;
}

/*
==============
Brush_Ray

Itersects a ray with a brush
Returns the face hit and the distance along the ray the intersection occured at
Returns NULL and 0 if not hit at all
==============
*/
face_t *Brush_Ray (vec3_t origin, vec3_t dir, brush_t *b, float *dist)
{
	face_t	*f, *firstface;
	vec3_t	p1, p2;
	double	frac, d1, d2;
	float	scaleValue;
	bool	scale;
	int		i;

	VectorCopy (origin, p1);
	// Need to clamp the p1 - p2 vector to compelety inside the valid range
	// This is horrid.. =)
	scale = true;
	scaleValue = WORLD_SIZE;

	while(scale)
	{
		VectorMA(p1, scaleValue, dir, p2);

		scale = false;
		for(i = 0; i < 3; i++)
		{
			if(p2[i] > MAX_WORLD_COORD)
			{
				scale = true;
			}
			if(p2[i] < MIN_WORLD_COORD)
			{
				scale = true;
			}
		}
		scaleValue *= 0.8f;
	}
	scaleValue /= 0.8f;
	VectorMA(p1, scaleValue, dir, p2);
	// ..end of nastiness

	for (f=b->brush_faces ; f ; f=f->next)
	{
		d1 = (double) DotProduct (p1, f->plane.normal) - f->plane.dist;
		d2 = (double) DotProduct (p2, f->plane.normal) - f->plane.dist;
		if (d1 >= 0 && d2 >= 0)
		{
			*dist = 0;
			return NULL;	// ray is on front side of face
		}
		if (d1 <=0 && d2 <= 0)
			continue;
	// clip the ray to the plane
		frac = d1 / (d1 - d2);
		if (d1 > 0)
		{
			firstface = f;
			for (i=0 ; i<3 ; i++)
				p1[i] = (double)p1[i] + frac * ((double)p2[i] - (double)p1[i]);
		}
		else
		{
			for (i=0 ; i<3 ; i++)
				p2[i] = (double)p1[i] + frac * ((double)p2[i] - (double)p1[i]);
		}
	}

	// find distance p1 is along dir
	VectorSubtract (p1, origin, p1);
	d1 = DotProduct (p1, dir);

	*dist = d1;

	return firstface;
}

//PGM
face_t *Brush_Point (vec3_t origin, brush_t *b)
{
	face_t	*f;
	float	d1;

	for (f=b->brush_faces ; f ; f=f->next)
	{
		d1 = DotProduct (origin, f->plane.normal) - f->plane.dist;
		if (d1 > 0)
		{
			return NULL;	// point is on front side of face
		}
	}

	return b->brush_faces;
}
//PGM


void	Brush_AddToList (brush_t *b, brush_t *list, bool bAddToBackOfList /* = false */)
{
	if (b->next || b->prev)
		Error ("Brush_AddToList: already linked");
	if (b->patchBrush && list == &selected_brushes)
	{
		Patch_Select(b->nPatchID);
	}

	if (bAddToBackOfList)
	{
		// lists in radiant are circular, so no need to traverse to list end, just link to list-head previous...
		//
		b->prev = list->prev;
		list->prev->next = b;
		list->prev = b;
		b->next = list;
	}
	else
	{
		b->next = list->next;
		list->next->prev = b;
		list->next = b;
		b->prev = list;
	}
}

void	Brush_RemoveFromList (brush_t *b)
{
	if (!b->next || !b->prev)
		Error ("Brush_RemoveFromList: not linked");
	if (b->patchBrush)
	{
		Patch_Deselect(b->nPatchID);
	}
	b->next->prev = b->prev;
	b->prev->next = b->next;
	b->next = b->prev = NULL;
}

/*
===============
SetFaceTexdef

Doesn't set the curve flags
===============
*/
void SetFaceTexdef (brush_t *b, face_t *f, texdef_t *texdef, bool bFitScale, bool bNoSystemTextureOverwrite) 
{
	int		oldFlags;
	int		oldContents;
	face_t	*tf;

	if (bNoSystemTextureOverwrite && !strnicmp(f->texdef.name,"system/",7))
	{
		Sys_Printf("( System-shader protecion: Refusing to overwrite face using \"%s\" )\n",f->texdef.name);
		return;
	}

	oldFlags = f->texdef.flags;
	oldContents = f->texdef.contents;
	if (bFitScale)
	{
		f->texdef = *texdef;
		// fit the scaling of the texture on the actual plane
		vec3_t p1,p2,p3; // absolute coordinates
		// compute absolute coordinates
		ComputeAbsolute(f,p1,p2,p3);
		// compute the scale
		vec3_t vx,vy;
		VectorSubtract(p2,p1,vx);
		VectorNormalize(vx);
		VectorSubtract(p3,p1,vy);
		VectorNormalize(vy);
		// assign scale
		VectorScale(vx,texdef->scale[0],vx);
		VectorScale(vy,texdef->scale[1],vy);
		VectorAdd(p1,vx,p2);
		VectorAdd(p1,vy,p3);
		// compute back shift scale rot
		AbsoluteToLocal(f->plane,f,p1,p2,p3);
	}
	else
		f->texdef = *texdef;
	f->texdef.flags = (f->texdef.flags & ~SURF_KEEP) | (oldFlags & SURF_KEEP);
	f->texdef.contents = (f->texdef.contents & ~CONTENTS_KEEP) | (oldContents & CONTENTS_KEEP);

	// if this is a curve face, set all other curve faces to the same texdef
	if (f->texdef.flags & SURF_CURVE) 
  {
		for (tf = b->brush_faces ; tf ; tf = tf->next) 
    {
			if (tf->texdef.flags & SURF_CURVE) 
      {
				tf->texdef = f->texdef;
			}
		}
	}
}


void	Brush_SetTexture (brush_t *b, texdef_t *texdef, bool bFitScale/*=false*/, bool bNoSystemTextureOverwrite/*=false*/)
{
	for (face_t* f = b->brush_faces ; f ; f = f->next) 
  {
		SetFaceTexdef (b, f, texdef, bFitScale, bNoSystemTextureOverwrite);
	}
	Brush_Build( b );
  if (b->patchBrush)
  {
    Patch_SetTexture(b->nPatchID, texdef);
  }
}


qboolean ClipLineToFace (vec3_t p1, vec3_t p2, face_t *f)
{
	float	d1, d2, fr;
	int		i;
	float	*v;

	d1 = DotProduct (p1, f->plane.normal) - f->plane.dist;
	d2 = DotProduct (p2, f->plane.normal) - f->plane.dist;

	if (d1 >= 0 && d2 >= 0)
		return false;		// totally outside
	if (d1 <= 0 && d2 <= 0)
		return true;		// totally inside

	fr = d1 / (d1 - d2);

	if (d1 > 0)
		v = p1;
	else
		v = p2;

	for (i=0 ; i<3 ; i++)
		v[i] = p1[i] + fr*(p2[i] - p1[i]);

	return true;
}


int AddPlanept (float *f)
{
	int		i;
	static overflow = 0;

	if (D_MAXPOINTS == g_qeglobals.d_num_move_points)	//jfm: stop this overflow
	{
		overflow++;
		Sys_Printf ("Trying to drag too many points!! %d\n",D_MAXPOINTS + overflow);
		return 0;
	}
	overflow = 0;

	for (i=0 ; i<g_qeglobals.d_num_move_points ; i++)
		if (g_qeglobals.d_move_points[i] == f)
			return 0;
	
	g_qeglobals.d_move_points[g_qeglobals.d_num_move_points++] = f;
	return 1;
}

/*
==============
Brush_SelectFaceForDragging

Adds the faces planepts to move_points, and
rotates and adds the planepts of adjacent face if shear is set
==============
*/
void Brush_SelectFaceForDragging (brush_t *b, face_t *f, qboolean shear)
{
	int		i;
	face_t	*f2;
	winding_t	*w;
	float	d;
	brush_t	*b2;
	int		c;

	if (b->owner->eclass->fixedsize)
		return;

	c = 0;
	for (i=0 ; i<3 ; i++)
		c += AddPlanept (f->planepts[i]);
	if (c == 0)
		return;		// allready completely added

	// select all points on this plane in all brushes the selection
	for (b2=selected_brushes.next ; b2 != &selected_brushes ; b2 = b2->next)
	{
		if (b2 == b)
			continue;
		for (f2=b2->brush_faces ; f2 ; f2=f2->next)
		{
			for (i=0 ; i<3 ; i++)
				if (fabs(DotProduct(f2->planepts[i], f->plane.normal)
				-f->plane.dist) > ON_EPSILON)
					break;
			if (i==3)
			{	// move this face as well
				Brush_SelectFaceForDragging (b2, f2, shear);
				break;
			}
		}
	}


	// if shearing, take all the planes adjacent to 
	// selected faces and rotate their points so the
	// edge clipped by a selcted face has two of the points
	if (!shear)
		return;

	for (f2=b->brush_faces ; f2 ; f2=f2->next)
	{
		if (f2 == f)
			continue;
		w = MakeFaceWinding (b, f2);
		if (!w)
			continue;

		// any points on f will become new control points
		for (i=0 ; i<w->numpoints ; i++)
		{
			d = DotProduct (w->points[i], f->plane.normal) 
				- f->plane.dist;
			if (d > -ON_EPSILON && d < ON_EPSILON)
				break;
		}

		//
		// if none of the points were on the plane,
		// leave it alone
		//
		if (i != w->numpoints)
		{
			if (i == 0)
			{	// see if the first clockwise point was the
				// last point on the winding
				d = DotProduct (w->points[w->numpoints-1]
					, f->plane.normal) - f->plane.dist;
				if (d > -ON_EPSILON && d < ON_EPSILON)
					i = w->numpoints - 1;
			}

			AddPlanept (f2->planepts[0]);

			VectorCopy (w->points[i], f2->planepts[0]);
			if (++i == w->numpoints)
				i = 0;
			
			// see if the next point is also on the plane
			d = DotProduct (w->points[i]
				, f->plane.normal) - f->plane.dist;
			if (d > -ON_EPSILON && d < ON_EPSILON)
				AddPlanept (f2->planepts[1]);

			VectorCopy (w->points[i], f2->planepts[1]);
			if (++i == w->numpoints)
				i = 0;

			// the third point is never on the plane

			VectorCopy (w->points[i], f2->planepts[2]);
		}

		free(w);
	}
}

/*
==============
Brush_SideSelect

The mouse click did not hit the brush, so grab one or more side
planes for dragging
==============
*/
void Brush_SideSelect (brush_t *b, vec3_t origin, vec3_t dir
					   , qboolean shear)
{
	face_t	*f, *f2;
	vec3_t	p1, p2;

  //if (b->patchBrush)
  //  return;
    //Patch_SideSelect(b->nPatchID, origin, dir);
	for (f=b->brush_faces ; f ; f=f->next)
	{
		VectorCopy (origin, p1);
		VectorMA (origin, WORLD_SIZE, dir, p2);

		for (f2=b->brush_faces ; f2 ; f2=f2->next)
		{
			if (f2 == f)
				continue;
			ClipLineToFace (p1, p2, f2);
		}

		if (f2)
			continue;

		if (VectorCompare (p1, origin))
			continue;
		if (ClipLineToFace (p1, p2, f))
			continue;

		Brush_SelectFaceForDragging (b, f, shear);
	}

	
}

void Brush_BuildWindings( brush_t *b, bool bSnap )
{
	winding_t *w;
	face_t    *face;
	vec_t      v;

  if (bSnap)
	  Brush_SnapPlanepts( b );

	// clear the mins/maxs bounds
	b->mins[0] = b->mins[1] = b->mins[2] = MAX_WORLD_COORD;
	b->maxs[0] = b->maxs[1] = b->maxs[2] = MIN_WORLD_COORD;

	Brush_MakeFacePlanes (b);

	face = b->brush_faces;

  float fCurveColor = 1.0;

  for ( ; face ; face=face->next)
	{
		int i, j;
    free(face->face_winding);
		w = face->face_winding = MakeFaceWinding (b, face);
		face->d_texture = Texture_ForName( face->texdef.name );

		if (!w)
			continue;
	
	  for (i=0 ; i<w->numpoints ; i++)
	  {
			// add to bounding box
			for (j=0 ; j<3 ; j++)
			{
				v = w->points[i][j];
				if (v > b->maxs[j])
					b->maxs[j] = v;
				if (v < b->mins[j])
					b->mins[j] = v;
			}
	  }
		// setup s and t vectors, and set color
		SetFaceColor (b, face, fCurveColor);
    fCurveColor -= .10;
    if (fCurveColor <= 0)
      fCurveColor = 1.0;
		//BeginTexturingFace( b, face, face->d_texture);

    for (i=0 ; i<w->numpoints ; i++)
		  EmitTextureCoordinates( w->points[i], face->d_texture, face);
	}
}

/*
==================
Brush_RemoveEmptyFaces

Frees any overconstraining faces
==================
*/
void Brush_RemoveEmptyFaces ( brush_t *b )
{
	face_t	*f, *next;

	f = b->brush_faces;
	b->brush_faces = NULL;

	for ( ; f ; f=next)
	{
		next = f->next;
		if (!f->face_winding)
			Face_Free (f);
		else
		{
			f->next = b->brush_faces;
			b->brush_faces = f;
		}

	}
}

void Brush_SnapToGrid(brush_t *pb)
{
  for (face_t *f = pb->brush_faces ; f; f = f->next)
  {
    for (int i = 0 ;i < 3 ;i++)
    {
      for (int j = 0 ;j < 3 ; j++)
      {
				f->planepts[i][j] = floor (f->planepts[i][j] / g_qeglobals.d_gridsize + 0.5) * g_qeglobals.d_gridsize;
      }
    }
  }
	Brush_Build(pb);
}

void Brush_Rotate(brush_t *b, vec3_t vAngle, vec3_t vOrigin, bool bBuild)
{
	for (face_t* f=b->brush_faces ; f ; f=f->next)
	{
		for (int i=0 ; i<3 ; i++)
		{
      VectorRotate(f->planepts[i], vAngle, vOrigin, f->planepts[i]);
    }
  }
  if (bBuild)
  {
    Brush_Build(b, false, false);
  }
}

void Brush_Scale2 (eclass_t	*e,brush_t *b, float scale, vec3_t vOrigin, bool bBuild)
{
	vec3_t	pts[4][2];
	int		i,j;
	face_t* f;

	pts[0][0][0] = e->mins[0];
	pts[0][0][1] = e->mins[1];
	
	pts[1][0][0] = e->mins[0];
	pts[1][0][1] = e->maxs[1];
	
	pts[2][0][0] = e->maxs[0];
	pts[2][0][1] = e->maxs[1];
	
	pts[3][0][0] = e->maxs[0];
	pts[3][0][1] = e->mins[1];
	
	for (i=0 ; i<4 ; i++)
	{
		pts[i][0][2] = e->mins[2];
		pts[i][1][0] = pts[i][0][0];
		pts[i][1][1] = pts[i][0][1];
		pts[i][1][2] = e->maxs[2];
	}

	f=b->brush_faces;
	for (i=0;i<4; i++,f=f->next)
	{
		j = (i+1)%4;
		VectorCopy (pts[j][1], f->planepts[0]);
		VectorCopy (pts[i][1], f->planepts[1]);
		VectorCopy (pts[i][0], f->planepts[2]);
	}

	VectorCopy (pts[0][1], f->planepts[0]);
	VectorCopy (pts[1][1], f->planepts[1]);
	VectorCopy (pts[2][1], f->planepts[2]);

	f=f->next;
	VectorCopy (pts[2][0], f->planepts[0]);
	VectorCopy (pts[1][0], f->planepts[1]);
	VectorCopy (pts[0][0], f->planepts[2]);


	for (f=b->brush_faces ; f ; f=f->next)
	{
		for (int i=0 ; i<3 ; i++)
		{
			VectorScale(f->planepts[i], scale, f->planepts[i]);
			VectorAdd(vOrigin,f->planepts[i],f->planepts[i]);
		}
	}

	if (bBuild)
	{
		Brush_Build(b, false, false);
	}
}


// only designed for fixed size entity brushes
void Brush_Resize(brush_t *b, vec3_t vMin, vec3_t vMax)
{
  brush_t *b2 = Brush_Create(vMin, vMax, &b->brush_faces->texdef);

  face_t *next;
	for (face_t *f=b->brush_faces ; f ; f=next)
	{
		next = f->next;
		Face_Free( f );
	}

  b->brush_faces = b2->brush_faces;

	// unlink from active/selected list
	if (b2->next)
  Brush_RemoveFromList (b2);
  free(b2);
  Brush_Build(b, true);
}


eclass_t* HasModel(brush_t *b)
{
  vec3_t vMin, vMax;
  vMin[0] = vMin[1] = vMin[2] = MAX_WORLD_COORD;
  vMax[0] = vMax[1] = vMax[2] = MIN_WORLD_COORD;

  if (b->owner->md3Class != NULL)
  {
    return b->owner->md3Class;
  }

  if (Eclass_hasModel(b->owner->eclass, vMin, vMax))
  {
    return b->owner->eclass;
  }

  eclass_t *e = NULL;
  // FIXME: entity needs to track whether a cache hit failed and not ask again
  if (strnicmp(b->owner->eclass->name, "misc_model",10) == 0)
  {
    char *pModel = ValueForKey(b->owner, "model");
    if (pModel != NULL && strlen(pModel) > 0)
    {
      e = GetCachedModel(b->owner, pModel, vMin, vMax);
      if (e != NULL)
      {
        // we need to scale the brush to the proper size based on the model load
        // recreate brush just like in load/save

		  VectorAdd( vMin, b->owner->origin, vMin);
		  VectorAdd( vMax, b->owner->origin, vMax);
		  Brush_Resize(b, vMin, vMax);

/*
        // we need to scale the brush to the proper size based on the model load
        vec3_t vTemp, vTemp2;
        VectorSubtract(b->maxs, b->mins, vTemp);
        VectorSubtract(vMax, vMin, vTemp2);
        for (int i = 0; i < 3; i++)
        {
          if (vTemp[i] != 0)
          {
            vTemp2[i] /= vTemp[i];
          }
        }
        vec3_t vMid;
        vMid[0] = vMid[1] = vMid[2] = 0.0;
        for (int j=0 ; j<3 ; j++)
        {
          vMid[j] = (b->mins[j] + (vTemp[j] / 2));
        }
		    for (face_t* f=b->brush_faces ; f ; f=f->next)
		    {
			    for (int i=0 ; i<3 ; i++)
			    {
					//scale
            VectorSubtract(f->planepts[i], vMid, f->planepts[i]);
            f->planepts[i][0] *= vTemp2[0];
            f->planepts[i][1] *= vTemp2[1];
            f->planepts[i][2] *= vTemp2[2];
            VectorAdd(f->planepts[i], vMid, f->planepts[i]);
          }
        }
        //Brush_SnapToGrid(b);
        Brush_Build(b, true);
*/
        b->bModelFailed = false;
      }
      else
      {
        b->bModelFailed = true;
      }
    }
  }
  return e;
}


static bool g_bInPaintedModel = false;
bool PaintedModel(brush_t *b, bool bOkToTexture, bool bIsGhost)
{
    if (g_bInPaintedModel)
    { 
      return true;
    }
    
    if (g_PrefsDlg.m_nEntityShowState == ENTITY_BOX)
    {
      return false;
    }
    else if (!IsBrushSelected(b) && (g_PrefsDlg.m_nEntityShowState & ENTITY_SELECTED_ONLY))
    {
	    return false;
    }

    g_bInPaintedModel = true;
    bool bReturn = false;

    eclass_t *pEclass = HasModel(b);

    if (pEclass)
    {
      qglPushAttrib(GL_ALL_ATTRIB_BITS);
      entitymodel *model = pEclass->model;

		float a = FloatForKey (b->owner, "angle");

//==============
#ifdef QUAKE3		
		bool bUseAnglesCode = false;
		vec3_t v3Angles;
		if (bOkToTexture)
		{
			vec3_t v3;
			if (GetVectorForKey (b->owner, "angles", v3))
			{
				// for some reason this program's own maths routines expect entity axis orders differently, so...			
				//
				v3Angles[0] = v3[2];
				v3Angles[1] = v3[0];
				v3Angles[2] = v3[1];
				bUseAnglesCode = true;
			}
		}
#endif
//==============

      while (model != NULL)
      {
        if (bOkToTexture == false || g_PrefsDlg.m_nEntityShowState & ENTITY_WIREFRAME || model->nTextureBind == -1)	// skinned
        {
	        qglDisable( GL_CULL_FACE );
	        qglPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	        qglDisable(GL_TEXTURE_2D);
          qglColor3fv(pEclass->color);
        }
        else
        {
          qglColor3f(1, 1, 1);
          qglEnable(GL_TEXTURE_2D);
	        qglBindTexture( GL_TEXTURE_2D, model->nTextureBind );
        }
        vec3_t v;
        
        int i,j;
        VectorAdd(b->maxs, b->mins, v);		//
        VectorScale(v, 0.5, v);				//
        VectorCopy(b->owner->origin, v);	// this is dumb, 3rd line just overwrites the other two -slc

		if (bIsGhost)
		{
			qglColor4fv(v4GhostColor);
			qglLineStipple( 8, 0xAAAA);
			qglEnable(GL_LINE_STIPPLE);				
		}

				 
        //for (i = 0; i < 3; i++)
        //{
        //  v[i] -= (pEclass->mins[i] - b->mins[i]);
        //}

        //if (model->nModelPosition)
        //{
		      //v[2] = b->mins[2] - (pEclass->mins[2]);
        //}

        float s, c;
	      if (a)
        {
		      s = sin (a/180*Q_PI);
		      c = cos (a/180*Q_PI);
        }

	      qglBegin (GL_TRIANGLES);

        for (i = 0; i < model->nTriCount; i++)
        {
          for (j = 0; j < 3; j++)
          {
//==============
#ifdef QUAKE3
			if (bUseAnglesCode)
			{
				vec3_t v3;
				VectorSet(v3,	model->pTriList[i].v[j][0] + v[0],
								model->pTriList[i].v[j][1] + v[1],
								model->pTriList[i].v[j][2] + v[2]
							);
				VectorRotate(v3, v3Angles, b->owner->origin, v3);
				
				qglTexCoord2f (model->pTriList[i].st[j][0], model->pTriList[i].st[j][1]);
				qglVertex3f(v3[0], v3[1], v3[2]);
			}
			else
#endif
//==============
			{
				float x = model->pTriList[i].v[j][0] + v[0];
				float y = model->pTriList[i].v[j][1] + v[1];
				if (a)
				{
				  float x2 = (((x - v[0]) * c) - ((y - v[1]) * s)) + v[0];
				  float y2 = (((x - v[0]) * s) + ((y - v[1]) * c)) + v[1];
				  x = x2;
				  y = y2;
				}
				//qglTexCoord2f (pEclass->pTriList[i].st[j][0] / pEclass->nSkinWidth, pEclass->pTriList[i].st[j][1] / pEclass->nSkinHeight);
				qglTexCoord2f (model->pTriList[i].st[j][0], model->pTriList[i].st[j][1]);
				qglVertex3f(x, y, model->pTriList[i].v[j][2] + v[2]);
			}
          }
        }
        qglEnd();
        if (g_PrefsDlg.m_nEntityShowState & ENTITY_WIREFRAME)	// skinned
        {
          qglEnable(GL_CULL_FACE );
          qglEnable(GL_TEXTURE_2D);
	        qglPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        }
        else
        {
	        qglDisable(GL_TEXTURE_2D);
        }

		if (bIsGhost)
		{
			qglDisable(GL_LINE_STIPPLE);				
		}

        model = model->pNext;
      }

	    if (g_PrefsDlg.m_nEntityShowState & ENTITY_BOXED)
	    {
			if (bIsGhost)
			{
				qglColor4fv(v4GhostColor);
				qglLineStipple( 8, 0xAAAA);
				qglEnable(GL_LINE_STIPPLE);				
			}
			else
			{
				qglColor3fv(pEclass->color);
			}


		    qglBegin(GL_LINE_LOOP);
		    qglVertex3f(b->mins[0],b->mins[1],b->mins[2]);
		    qglVertex3f(b->maxs[0],b->mins[1],b->mins[2]);
		    qglVertex3f(b->maxs[0],b->maxs[1],b->mins[2]);
		    qglVertex3f(b->mins[0],b->maxs[1],b->mins[2]);
		    qglEnd();
		    
		    qglBegin(GL_LINE_LOOP);
		    qglVertex3f(b->mins[0],b->mins[1],b->maxs[2]);
		    qglVertex3f(b->maxs[0],b->mins[1],b->maxs[2]);
		    qglVertex3f(b->maxs[0],b->maxs[1],b->maxs[2]);
		    qglVertex3f(b->mins[0],b->maxs[1],b->maxs[2]);
		    qglEnd();

		    qglBegin(GL_LINES);
		    qglVertex3f(b->mins[0],b->mins[1],b->mins[2]);
		    qglVertex3f(b->mins[0],b->mins[1],b->maxs[2]);
		    qglVertex3f(b->mins[0],b->maxs[1],b->maxs[2]);
		    qglVertex3f(b->mins[0],b->maxs[1],b->mins[2]);
		    qglVertex3f(b->maxs[0],b->mins[1],b->mins[2]);
		    qglVertex3f(b->maxs[0],b->mins[1],b->maxs[2]);
		    qglVertex3f(b->maxs[0],b->maxs[1],b->maxs[2]);
		    qglVertex3f(b->maxs[0],b->maxs[1],b->mins[2]);
		    qglEnd();

			if (bIsGhost)
			{
				qglDisable(GL_LINE_STIPPLE);				
			}

      }
	    qglPopAttrib();
      bReturn = true;
    }
    else
    {
      b->bModelFailed = true;
    }

  g_bInPaintedModel = false;
  return bReturn;
}

void AngleVectors (vec3_t angles, vec3_t forward, vec3_t right, vec3_t up)
{
	float		angle;
	static float		sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs

	angle = angles[YAW] * Q_PI / 180;
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[PITCH] * Q_PI / 180;
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[ROLL] * Q_PI / 180;
	sr = sin(angle);
	cr = cos(angle);

	if (forward)
	{
		forward[0] = cp*cy;
		forward[1] = cp*sy;
		forward[2] = -sp;
	}
	if (right)
	{
		right[0] = (-1*sr*sp*cy+-1*cr*-sy);
		right[1] = (-1*sr*sp*sy+-1*cr*cy);
		right[2] = -1*sr*cp;
	}
	if (up)
	{
		up[0] = (cr*sp*cy+-sr*-sy);
		up[1] = (cr*sp*sy+-sr*cy);
		up[2] = cr*cp;
	}
}

void FacingVectors (entity_t *e, vec3_t forward, vec3_t right, vec3_t up)
{
	int			angleVal;
	vec3_t		angles;

	angleVal = IntForKey(e, "angle");
	if (angleVal == -1)				// up
	{
		VectorSet(angles, 270, 0, 0);
	}
	else if(angleVal == -2)		// down
	{
		VectorSet(angles, 90, 0, 0);
	}
	else
	{
		VectorSet(angles, 0, angleVal, 0);
	}

	AngleVectors(angles, forward, right, up);
}

void Brush_DrawFacingAngle (brush_t *b, entity_t *e)
{
	vec3_t	forward, right, up;
	vec3_t	endpoint, tip1, tip2;
	vec3_t	start;
	float	dist;

	VectorAdd(e->brushes.onext->mins, e->brushes.onext->maxs, start);
	VectorScale(start, 0.5, start);
	dist = (b->maxs[0] - start[0]) * 2.5;

	FacingVectors (e, forward, right, up);
	VectorMA (start, dist, forward, endpoint);

	dist = (b->maxs[0] - start[0]) * 0.5;
	VectorMA (endpoint, -dist, forward, tip1);
	VectorMA (tip1, -dist, up, tip1);
	VectorMA (tip1, 2*dist, up, tip2);

	qglColor4f (1, 1, 1, 1);
	qglLineWidth (4);
	qglBegin (GL_LINES);
	qglVertex3fv (start);
	qglVertex3fv (endpoint);
	qglVertex3fv (endpoint);
	qglVertex3fv (tip1);
	qglVertex3fv (endpoint);
	qglVertex3fv (tip2);
	qglEnd ();
	qglLineWidth (1);
}

void DrawLight(brush_t *b)
{
	vec3_t vTriColor;
	bool bTriPaint = false;

  vTriColor[0] = vTriColor[2] = 1.0;
  vTriColor[1]  = 1.0;
  bTriPaint = true;
  CString strColor = ValueForKey(b->owner, "_color");
  if (strColor.GetLength() > 0)
  {
    float fR, fG, fB;
	  int n = sscanf(strColor,"%f %f %f", &fR, &fG, &fB);
    if (n == 3)
    {
      vTriColor[0] = fR;
      vTriColor[1] = fG;
      vTriColor[2] = fB;
    }
  }
  qglColor3f(vTriColor[0], vTriColor[1], vTriColor[2]);

  vec3_t vCorners[4];
  float fMid = b->mins[2] + (b->maxs[2] - b->mins[2]) / 2;

  vCorners[0][0] = b->mins[0];
  vCorners[0][1] = b->mins[1];
  vCorners[0][2] = fMid;

  vCorners[1][0] = b->mins[0];
  vCorners[1][1] = b->maxs[1];
  vCorners[1][2] = fMid;

  vCorners[2][0] = b->maxs[0];
  vCorners[2][1] = b->maxs[1];
  vCorners[2][2] = fMid;

  vCorners[3][0] = b->maxs[0];
  vCorners[3][1] = b->mins[1];
  vCorners[3][2] = fMid;

  vec3_t vTop, vBottom;

  vTop[0] = b->mins[0] + ((b->maxs[0] - b->mins[0]) / 2);
  vTop[1] = b->mins[1] + ((b->maxs[1] - b->mins[1]) / 2);
  vTop[2] = b->maxs[2];

  VectorCopy(vTop, vBottom);
  vBottom[2] = b->mins[2];

  vec3_t vSave;
  VectorCopy(vTriColor, vSave);

  qglBegin(GL_TRIANGLE_FAN);
  qglVertex3fv(vTop);
  for (int i = 0; i <= 3; i++)
  {
    vTriColor[0] *= 0.95;
    vTriColor[1] *= 0.95;
    vTriColor[2] *= 0.95;
    qglColor3f(vTriColor[0], vTriColor[1], vTriColor[2]);
    qglVertex3fv(vCorners[i]);
  }
  qglVertex3fv(vCorners[0]);
  qglEnd();
  
  VectorCopy(vSave, vTriColor);
  vTriColor[0] *= 0.95;
  vTriColor[1] *= 0.95;
  vTriColor[2] *= 0.95;

  qglBegin(GL_TRIANGLE_FAN);
  qglVertex3fv(vBottom);
  qglVertex3fv(vCorners[0]);
  for (i = 3; i >= 0; i--)
  {
    vTriColor[0] *= 0.95;
    vTriColor[1] *= 0.95;
    vTriColor[2] *= 0.95;
    qglColor3f(vTriColor[0], vTriColor[1], vTriColor[2]);
    qglVertex3fv(vCorners[i]);
  }
  qglEnd();
}



extern qboolean qbCheckShowFaces;
void Brush_Draw( brush_t *b, bool bIsGhost )
{
	face_t			*face;
	int				i, order;
	qtexture_t		*prev = 0;
	winding_t *w;

	if (b->hiddenBrush)
	{
		return;
	}


  if (b->patchBrush)
  {	  	  
    Patch_DrawCam(b->nPatchID, bIsGhost);
//    if (!g_bPatchShowBounds)
    return;
  }

	int nDrawMode = g_pParentWnd->GetCamera()->Camera().draw_mode;

	if (b->owner->eclass->fixedsize)
	{

    /* MERGEME
		if(!(g_qeglobals.d_savedinfo.exclude & EXCLUDE_ANGLES) && 
			!strnicmp(b->owner->eclass->name, "info_player", 11))
		{
			Brush_DrawFacingAngle(b, b->owner);
		}
    */

    if (g_PrefsDlg.m_bNewLightDraw && strcmpi(b->owner->eclass->name, "light") == 0)
    {
			DrawLight(b);
			return;
		}
    if (nDrawMode == cd_texture || nDrawMode == cd_light)
		  qglDisable (GL_TEXTURE_2D);

    // if we are wireframing models
    bool bp = (b->bModelFailed) ? false : PaintedModel(b, true, bIsGhost);

    if (nDrawMode == cd_texture || nDrawMode == cd_light)
		  qglEnable (GL_TEXTURE_2D);

    if (bp)
      return;
  }

	// guarantee the texture will be set first
	prev = NULL;
	for (face = b->brush_faces,order = 0 ; face ; face=face->next, order++)
	{
		w = face->face_winding;
		if (!w)
			continue;		// freed face

#if 0
    if (b->alphaBrush)
    {
      if (!(face->texdef.flags & SURF_ALPHA))
        continue;
      //--qglPushAttrib(GL_ALL_ATTRIB_BITS);
      qglDisable(GL_CULL_FACE);
      //--qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		  //--qglBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		  //--qglDisable(GL_DEPTH_TEST);
		  //--qglBlendFunc (GL_SRC_ALPHA, GL_DST_ALPHA);
	    //--qglEnable (GL_BLEND);
    }
#endif

    if ((nDrawMode == cd_texture || nDrawMode == cd_light) && face->d_texture != prev)
    {
	    // set the texture for this face
			prev = face->d_texture;
			qglBindTexture( GL_TEXTURE_2D, face->d_texture->texture_number );
    }


    if (!b->patchBrush)
    {
    if (face->texdef.flags & SURF_TRANS33) 
      qglColor4f ( face->d_color[0], face->d_color[1], face->d_color[2], 0.33 );
    else if ( face->texdef.flags & SURF_TRANS66) 
      qglColor4f ( face->d_color[0], face->d_color[1], face->d_color[2], 0.66 );
	  else
	    qglColor3fv( face->d_color );
    }
    else
    {
      qglColor4f ( face->d_color[0], face->d_color[1], face->d_color[2], 0.13 );
    }

    // shader drawing stuff
    if (face->d_texture->bFromShader)
    {
      // setup shader drawing
      qglColor4f ( face->d_color[0], face->d_color[1], face->d_color[2], face->d_texture->fTrans );

    }


	if (bIsGhost)
	{
		qglColor4fv( v4GhostColor );
		qglDisable (GL_TEXTURE_2D);
	}


    // draw the polygon
		qglBegin(GL_POLYGON);
    if (nDrawMode == cd_light)
      qglNormal3fv(face->plane.normal);

	  for (i=0 ; i<w->numpoints ; i++)
		{
		  if (nDrawMode == cd_texture || nDrawMode == cd_light)
			  qglTexCoord2fv( &w->points[i][3] );
			qglVertex3fv(w->points[i]);
		}
		qglEnd();

		// draw face lines over top of faces (artist/designer request)...		
		// drawing them as tris helps show how much overlap.waste there is because of diagonal lines
		//
		if (qbCheckShowFaces && qbShowFaces)
		{
			qglDisable (GL_TEXTURE_2D);
			if (bIsGhost)
			{
				qglColor4fv( v4GhostColor );
			}
			else
			{
				qglColor3f(255,255,255);
			}
			for (i=2 ; i<w->numpoints ; i++)
			{
				qglBegin(GL_LINE_STRIP);
				qglVertex3fv (w->points[0]);
				qglVertex3fv (w->points[i-1]);
				qglVertex3fv (w->points[i]);
				qglVertex3fv (w->points[0]);
				qglEnd ();				
			}
		    if (nDrawMode == cd_texture || nDrawMode == cd_light)
				qglEnable (GL_TEXTURE_2D);
		}

		if (bIsGhost)
		{
			qglEnable (GL_TEXTURE_2D);
		}

	}

#if 0
  if (b->alphaBrush)
  {
    //--qglPopAttrib();
    qglEnable(GL_CULL_FACE);
    //--qglDisable (GL_BLEND);
    //--qglTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  }
#endif

  if (b->owner->eclass->fixedsize && (nDrawMode == cd_texture || nDrawMode == cd_light))
		qglEnable (GL_TEXTURE_2D);

	qglBindTexture( GL_TEXTURE_2D, 0 );
}



void Face_Draw( face_t *f )
{
	int i;

	if ( f->face_winding == 0 )
		return;
	qglBegin( GL_POLYGON );
	for ( i = 0 ; i < f->face_winding->numpoints; i++)
		qglVertex3fv( f->face_winding->points[i] );
	qglEnd();
}

bool gbSelectedLightBrushesAlwaysDrawRadii = false;
//#define DEGTORAD			0.01745329252
//#define RADTODEG			57.295779513
void Brush_DrawXY(brush_t *b, int nViewType, bool bIsGhost)
{
	face_t *face;
	int     order;
	winding_t *w;
	int        i;

	if (b->hiddenBrush)
	{
		return;
	}


  if (b->patchBrush)
  {
    Patch_DrawXY(b->nPatchID, bIsGhost);
    if (!g_bPatchShowBounds)
      return;
  }
                     

	if (b->owner->eclass->fixedsize)
	{
		if (g_PrefsDlg.m_bNewLightDraw && strcmpi(b->owner->eclass->name, "light") == 0)
		{
		  vec3_t vCorners[4];
		  float fMid = b->mins[2] + (b->maxs[2] - b->mins[2]) / 2;

		  vCorners[0][0] = b->mins[0];
		  vCorners[0][1] = b->mins[1];
		  vCorners[0][2] = fMid;

		  vCorners[1][0] = b->mins[0];
		  vCorners[1][1] = b->maxs[1];
		  vCorners[1][2] = fMid;

		  vCorners[2][0] = b->maxs[0];
		  vCorners[2][1] = b->maxs[1];
		  vCorners[2][2] = fMid;

		  vCorners[3][0] = b->maxs[0];
		  vCorners[3][1] = b->mins[1];
		  vCorners[3][2] = fMid;

		  vec3_t vTop, vBottom;

		  vTop[0] = b->mins[0] + ((b->maxs[0] - b->mins[0]) / 2);
		  vTop[1] = b->mins[1] + ((b->maxs[1] - b->mins[1]) / 2);
		  vTop[2] = b->maxs[2];

		  VectorCopy(vTop, vBottom);
		  vBottom[2] = b->mins[2];
			
		  qglPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
		  qglBegin(GL_TRIANGLE_FAN);
		  qglVertex3fv(vTop);
		  qglVertex3fv(vCorners[0]);
		  qglVertex3fv(vCorners[1]);
		  qglVertex3fv(vCorners[2]);
		  qglVertex3fv(vCorners[3]);
		  qglVertex3fv(vCorners[0]);
		  qglEnd();
		  qglBegin(GL_TRIANGLE_FAN);
		  qglVertex3fv(vBottom);
		  qglVertex3fv(vCorners[0]);
		  qglVertex3fv(vCorners[3]);
		  qglVertex3fv(vCorners[2]);
		  qglVertex3fv(vCorners[1]);
		  qglVertex3fv(vCorners[0]);
		  qglEnd();
			  DrawBrushEntityName (b);

		  // now draw lighting radius stuff...
		  //
			  if (!(g_qeglobals.d_savedinfo.exclude & EXCLUDE_LIGHTS_RADII) || (gbSelectedLightBrushesAlwaysDrawRadii && IsBrushSelected(b)) )
			{
				vec3_t v3Origin;
				v3Origin[0] = b->mins[0] + (b->maxs[0] - b->mins[0]) / 2;
				v3Origin[1] = b->mins[1] + (b->maxs[1] - b->mins[1]) / 2;
				v3Origin[2] = b->mins[2] + (b->maxs[2] - b->mins[2]) / 2;
				char *p = ValueForKey(b->owner, "light");
				if (p[0]==0)
				{
					p = "300";
				}
				float f = atof(p);			
				float fLight = f;
				float f45Len = 0.707;

				vec3_t v3TargetOrigin;
				bool bDrawSpotlightArc = false;
				LPCSTR psTargetName = ValueForKey(b->owner, "target");
				bool bIsSpotLight = !!psTargetName[0];
				if (bIsSpotLight)
				{
					char *p = ValueForKey(b->owner, "radius");
					if (p[0]==0)
					{
						p="64";
					}
					f = atof(p);

					// find the origin of the target...
					//					
					entity_t *e = FindEntity("targetname", psTargetName);
					if (e)
					{
						v3TargetOrigin[0] = e->brushes.mins[0] + (e->brushes.maxs[0] - e->brushes.mins[0]) / 2;
						v3TargetOrigin[1] = e->brushes.mins[1] + (e->brushes.maxs[1] - e->brushes.mins[1]) / 2;
						v3TargetOrigin[2] = e->brushes.mins[2] + (e->brushes.maxs[2] - e->brushes.mins[2]) / 2;
						bDrawSpotlightArc = true;
					}
				}

				for (int iPass=0; iPass<2; iPass++)
				{
					if (iPass)
					{
						float _f = f;
						#define iFLAGS_LINEAR		0x01
						#define iFLAGS_NOINCIDENCE	0x02

						if (bIsSpotLight)
						{
							if (bDrawSpotlightArc)
							{
								// I give up on this, it's beyond me
							}
						}
						else
						{
							int iFlags = atoi( ValueForKey(b->owner,"spawnflags") );
							if (iFlags & iFLAGS_NOINCIDENCE)
							{
								f *= 0.9;
							}
							else
							{
								f/=4;
							}

							p = ValueForKey(b->owner, "scale");
							if (p[0]==0)
							{
								p="1";
							}
							float fScale = atof(p);
							f *= fScale;

							// clamp inner dotted circle so it never exceeds outer...
							//						
							if (f>_f)
								f=_f;
						}
						qglLineStipple( 8, 0xAAAA);
						qglEnable(GL_LINE_STIPPLE);
					}

					qglBegin(GL_LINE_LOOP);
					{
						if (bIsSpotLight)
						{
							if (bDrawSpotlightArc)
							{
								// I give up on this, it's beyond me
								/*
								vec3_t v3Temp;
								VectorSubtract(v3TargetOrigin,v3Origin,v3Temp);
								float fAdjacent = VectorLength(v3Temp);
								float fOpposite = f/2;
								float f2 = atan2(fOpposite,fAdjacent);
								f2 *= 180*Q_PI;	// radtodeg

								float	x, y, a;

								switch (g_pParentWnd->ActiveXY()->GetViewType())
								{
									case XY:

										x = v3Origin[0];
										y = v3Origin[1];
										a = f2;
										break;

									case XZ:

										x = v3Origin[0];
										y = v3Origin[2];
										a = f2;
										break;

									case YZ:

										x = v3Origin[1];
										y = v3Origin[2];
										a = f2;
										break;
								}

								// findmeste																		
								
								qglVertex3f (x+48*cos(a+Q_PI/(360.0f/a)), y+48*sin(a+Q_PI/(360.0f/a)), 0);
								qglVertex3f (x, y, 0);
								qglVertex3f (x+48*cos(a-Q_PI/(360.0f/a)), y+48*sin(a-Q_PI/(360.0f/a)), 0);
								qglEnd ();
								*/
							}
						}
						else
						{
							switch (g_pParentWnd->ActiveXY()->GetViewType())
							{
								case XY:	

									qglVertex3f( v3Origin[0]+(f*0),			v3Origin[1]+(f*1),			v3Origin[2] );
									qglVertex3f( v3Origin[0]+(f*f45Len),	v3Origin[1]+(f*f45Len),		v3Origin[2] );
									qglVertex3f( v3Origin[0]+(f*1),			v3Origin[1]+(f*0),			v3Origin[2] );
									qglVertex3f( v3Origin[0]+(f*f45Len),	v3Origin[1]+(-f*f45Len),	v3Origin[2] );
									qglVertex3f( v3Origin[0]+(f*0),			v3Origin[1]+(-f*1),			v3Origin[2] );
									qglVertex3f( v3Origin[0]+(-f*f45Len),	v3Origin[1]+(-f*f45Len),	v3Origin[2] );
									qglVertex3f( v3Origin[0]+(-f*1),		v3Origin[1]+(f*0),			v3Origin[2] );
									qglVertex3f( v3Origin[0]+(-f*f45Len),	v3Origin[1]+(f*f45Len),		v3Origin[2] );
									break;

								case XZ:

									qglVertex3f( v3Origin[0]+(f*0),			v3Origin[1], v3Origin[2]+(f*1)			 );
									qglVertex3f( v3Origin[0]+(f*f45Len),	v3Origin[1], v3Origin[2]+(f*f45Len)		 );
									qglVertex3f( v3Origin[0]+(f*1),			v3Origin[1], v3Origin[2]+(f*0)			 );
									qglVertex3f( v3Origin[0]+(f*f45Len),	v3Origin[1], v3Origin[2]+(-f*f45Len)	 );
									qglVertex3f( v3Origin[0]+(f*0),			v3Origin[1], v3Origin[2]+(-f*1)			 );
									qglVertex3f( v3Origin[0]+(-f*f45Len),	v3Origin[1], v3Origin[2]+(-f*f45Len)	 );
									qglVertex3f( v3Origin[0]+(-f*1),		v3Origin[1], v3Origin[2]+(f*0)			 );
									qglVertex3f( v3Origin[0]+(-f*f45Len),	v3Origin[1], v3Origin[2]+(f*f45Len)		 );
									break;

								case YZ:

									qglVertex3f( v3Origin[0],	v3Origin[1]+(f*0),		 v3Origin[2]+(f*1)			 );
									qglVertex3f( v3Origin[0],	v3Origin[1]+(f*f45Len),	 v3Origin[2]+(f*f45Len)		 );
									qglVertex3f( v3Origin[0],	v3Origin[1]+(f*1),		 v3Origin[2]+(f*0)			 );
									qglVertex3f( v3Origin[0],	v3Origin[1]+(f*f45Len),	 v3Origin[2]+(-f*f45Len)	 );
									qglVertex3f( v3Origin[0],	v3Origin[1]+(f*0),		 v3Origin[2]+(-f*1)			 );
									qglVertex3f( v3Origin[0],	v3Origin[1]+(-f*f45Len), v3Origin[2]+(-f*f45Len)	 );
									qglVertex3f( v3Origin[0],	v3Origin[1]+(-f*1),		 v3Origin[2]+(f*0)			 );
									qglVertex3f( v3Origin[0],	v3Origin[1]+(-f*f45Len), v3Origin[2]+(f*f45Len)		 );															  
									break;
							}
						}
					}
					qglEnd();
				}
				qglDisable(GL_LINE_STIPPLE);
			}

		  return;
		}
		else if (strnicmp(b->owner->eclass->name, "misc_model",10) == 0)
		{
		  if (PaintedModel(b, false, bIsGhost))
			return;
		}
	}

	for (face = b->brush_faces,order = 0 ; face ; face=face->next, order++)
	{
		// only draw polygons facing in a direction we care about
		if (nViewType == XY)
		{
			if (face->plane.normal[2] <= 0)
				continue;
		}
		else
		{
			if (nViewType == XZ)
			{
				if (face->plane.normal[1] <= 0)
					continue;
			}
			else 
			{
				if (face->plane.normal[0] <= 0)
					continue;
			}
		}

		w = face->face_winding;
		if (!w)
			continue;

    //if (b->alphaBrush && !(face->texdef.flags & SURF_ALPHA))
    //  continue;

		//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
		// draw the polygon
		qglBegin(GL_LINE_LOOP);

		for (i=0 ; i<w->numpoints ; i++)
			qglVertex3fv(w->points[i]);
		
		qglEnd();


		// draw angle arrows, but only if viewing from top down, and it's an ent brush with an "angle" field...
		//
		if (nViewType == XY && b->owner && (b->owner != world_entity) && strlen(ValueForKey(b->owner,"angle")))
		{
			// draw directional arrow...
			//
			// (work out size first)
			//
			float fMaxX = MIN_WORLD_COORD;
			float fMinX = MAX_WORLD_COORD;
			float fMaxY = MIN_WORLD_COORD;
			float fMinY = MAX_WORLD_COORD;
			float fMaxZ = MIN_WORLD_COORD;
			float fMinZ = MAX_WORLD_COORD;

			for (i=0 ; i<w->numpoints ; i++)
			{
				fMaxX = max(fMaxX, w->points[i][0]);
				fMinX = min(fMinX, w->points[i][0]);

				fMaxY = max(fMaxY, w->points[i][1]);
				fMinY = min(fMinY, w->points[i][1]);
				
				fMaxZ = max(fMaxZ, w->points[i][2]);	// Z stuff not really important, but useful for coords later so WTF?
				fMinZ = min(fMinZ, w->points[i][2]);
			}

			float fSmallest= min( (fMaxX-fMinX) , (fMaxY-fMinY) );	// do NOT use Z for smallest calc!				  
				  fSmallest*= 0.8f;

			vec3_t v3Centre = 
			{  
				fMinX + ((fMaxX-fMinX)/2),
				fMinY + ((fMaxY-fMinY)/2),
				fMinZ + ((fMaxZ-fMinZ)/2),
			};

			vec3_t	v3Top;
			VectorCopy(v3Centre,v3Top);
			v3Top[1] += fSmallest/2;

			vec3_t	v3LTArrowEdge = {v3Centre[0] - fSmallest/4, v3Centre[1] - fSmallest/4, v3Top[2]};
			vec3_t	v3RTArrowEdge = {v3Centre[0] + fSmallest/4, v3Centre[1] - fSmallest/4, v3Top[2]};

			vec3_t v3Angle = {0,0, FloatForKey (b->owner, "angle")-90};

			VectorRotate(v3Top,		v3Angle, v3Centre, v3Top);
			VectorRotate(v3LTArrowEdge,	v3Angle, v3Centre, v3LTArrowEdge);
			VectorRotate(v3RTArrowEdge,	v3Angle, v3Centre, v3RTArrowEdge);

			qglBegin(GL_LINE_LOOP);

				qglVertex3fv(v3Top);
				qglVertex3fv(v3LTArrowEdge);
				qglVertex3fv(v3Centre);
				qglVertex3fv(v3RTArrowEdge);

			qglEnd();	
		}
	}

	DrawBrushEntityName (b);

}

face_t *Face_Alloc( void )
{
	face_t *f = (face_t*)qmalloc( sizeof( *f ) );

	return f;
}

void Face_Free( face_t *f )
{
	assert( f != 0 );

	if ( f->face_winding )
  {
		free( f->face_winding );
    f->face_winding = 0;
  }
	free( f );
}

void Clamp(float& f, int nClamp)
{
  float fFrac = f - static_cast<int>(f);
  f = static_cast<int>(f) % nClamp;
  f += fFrac;
}

void Face_MoveTexture(face_t *f, vec3_t delta)
{
  vec3_t vX, vY;
  TextureAxisFromPlane(&f->plane, vX, vY);
  
  vec3_t vDP, vShift;
  vDP[0] = DotProduct(delta, vX);
  vDP[1] = DotProduct(delta, vY);

  double fAngle = f->texdef.rotate  / 180 * Q_PI;
	double c = cos(fAngle);
	double s = sin(fAngle);

  vShift[0] = vDP[0] * c - vDP[1] * s;
  vShift[1] = vDP[0] * s + vDP[1] * c;

  if (!f->texdef.scale[0])
    f->texdef.scale[0] = 1;	// fTEXTURE_SCALE?
  if (!f->texdef.scale[1])
    f->texdef.scale[1] = 1;	// fTEXTURE_SCALE?

  f->texdef.shift[0] -= vShift[0] / f->texdef.scale[0];
  f->texdef.shift[1] -= vShift[1] / f->texdef.scale[1];
  
  // clamp the shifts
  //f->texdef.shift[0] = static_cast<int>(f->texdef.shift[0]) % f->d_texture->width;
  //f->texdef.shift[1] = static_cast<int>(f->texdef.shift[1]) % f->d_texture->height;
  Clamp(f->texdef.shift[0], f->d_texture->width);
  Clamp(f->texdef.shift[1], f->d_texture->height);


}


/*
============
Brush_Move
============
*/
void Brush_Move (brush_t *b, const vec3_t move, bool bSnap)
{
  int i;
  face_t *f;

  for (f=b->brush_faces ; f ; f=f->next)
  {
    vec3_t vTemp;
    VectorCopy(move, vTemp);

    if (g_PrefsDlg.m_bTextureLock)
      Face_MoveTexture(f, vTemp);
    
    for (i=0 ; i<3 ; i++)
      VectorAdd (f->planepts[i], move, f->planepts[i]);
  }
  Brush_Build( b, bSnap );


  if (b->patchBrush)
  {
    Patch_Move(b->nPatchID, move);
  }


  // PGM - keep the origin vector up to date on fixed size entities.
  if(b->owner->eclass->fixedsize)
  {
		VectorAdd(b->owner->origin, move, b->owner->origin);
	  //VectorAdd(b->maxs, b->mins, b->owner->origin);
	  //VectorScale(b->owner->origin, 0.5, b->owner->origin);
  }
}



void Brush_Print(brush_t* b)
{
  int nFace = 0;
  for (face_t* f = b->brush_faces ; f ; f=f->next)
  {
    Sys_Printf("Face %i\n", nFace++);
    Sys_Printf("%f %f %f\n", f->planepts[0][0], f->planepts[0][1], f->planepts[0][2]);
    Sys_Printf("%f %f %f\n", f->planepts[1][0], f->planepts[1][1], f->planepts[1][2]);
    Sys_Printf("%f %f %f\n", f->planepts[2][0], f->planepts[2][1], f->planepts[2][2]);
  }
}



/*
=============
Brush_MakeSided

Makes the current brushhave the given number of 2d sides and turns it into a cone
=============
*/
void Brush_MakeSidedCone(int sides)
{
	int		i;
	vec3_t	mins, maxs;
	brush_t	*b;
	texdef_t	*texdef;
	face_t	*f;
	vec3_t	mid;
	float	width;
	float	sv, cv;

	if (sides < 3)
	{
		Sys_Status ("Bad sides number", 0);
		return;
	}

	if (!QE_SingleBrush ())
	{
		Sys_Status ("Must have a single brush selected", 0 );
		return;
	}

	b = selected_brushes.next;
	VectorCopy (b->mins, mins);
	VectorCopy (b->maxs, maxs);
	texdef = &g_qeglobals.d_texturewin.texdef;

	Brush_Free (b);

	// find center of brush
	width = 8;
	for (i=0 ; i<2 ; i++)
	{
		mid[i] = (maxs[i] + mins[i])*0.5;
		if (maxs[i] - mins[i] > width)
			width = maxs[i] - mins[i];
	}
	width /= 2;

	b = (brush_t*)qmalloc (sizeof(brush_t));

	// create bottom face
	f = Face_Alloc();
	f->texdef = *texdef;
	f->next = b->brush_faces;
	b->brush_faces = f;

	f->planepts[0][0] = mins[0];f->planepts[0][1] = mins[1];f->planepts[0][2] = mins[2];
	f->planepts[1][0] = maxs[0];f->planepts[1][1] = mins[1];f->planepts[1][2] = mins[2];
	f->planepts[2][0] = maxs[0];f->planepts[2][1] = maxs[1];f->planepts[2][2] = mins[2];

	for (i=0 ; i<sides ; i++)
	{
		f = Face_Alloc();
		f->texdef = *texdef;
		f->next = b->brush_faces;
		b->brush_faces = f;

		sv = sin (i*3.14159265*2/sides);
		cv = cos (i*3.14159265*2/sides);


		f->planepts[0][0] = floor(mid[0]+width*cv+0.5);
		f->planepts[0][1] = floor(mid[1]+width*sv+0.5);
		f->planepts[0][2] = mins[2];

		f->planepts[1][0] = mid[0];
		f->planepts[1][1] = mid[1];
		f->planepts[1][2] = maxs[2];

		f->planepts[2][0] = floor(f->planepts[0][0] - width * sv + 0.5);
		f->planepts[2][1] = floor(f->planepts[0][1] + width * cv + 0.5);
		f->planepts[2][2] = maxs[2];

	}

	Brush_AddToList (b, &selected_brushes);

	Entity_LinkBrush (world_entity, b);

	Brush_Build( b );

	Sys_UpdateWindows (W_ALL);
}

/*
=============
Brush_MakeSided

Makes the current brushhave the given number of 2d sides and turns it into a sphere
=============

*/
void Brush_MakeSidedSphere(int sides)
{
	int		i,j;
	vec3_t	mins, maxs;
	brush_t	*b;
	texdef_t	*texdef;
	face_t	*f;
	vec3_t	mid;

	if (sides < 4)
	{
		Sys_Status ("Bad sides number", 0);
		return;
	}

	if (!QE_SingleBrush ())
	{
		Sys_Status ("Must have a single brush selected", 0 );
		return;
	}

	b = selected_brushes.next;
	VectorCopy (b->mins, mins);
	VectorCopy (b->maxs, maxs);
	texdef = &g_qeglobals.d_texturewin.texdef;

	Brush_Free (b);

	// find center of brush
	float radius = 8;
	for (i=0 ; i<2 ; i++)
	{
		mid[i] = (maxs[i] + mins[i])*0.5;
		if (maxs[i] - mins[i] > radius)
			radius = maxs[i] - mins[i];
	}
	radius /= 2;

	b = (brush_t*)qmalloc (sizeof(brush_t));


	float dt = float(2 * Q_PI / sides);
	float dp = float(Q_PI / sides);
  float t,p;
	for(i=0; i <= sides-1; i++)
  {
		for(j=0;j <= sides-2; j++)
		{
			t = i * dt;
			p = float(j * dp - Q_PI / 2);

      f = Face_Alloc();
	    f->texdef = *texdef;
	    f->next = b->brush_faces;
	    b->brush_faces = f;

      VectorPolar(f->planepts[0], radius, t, p);
      VectorPolar(f->planepts[1], radius, t, p + dp);
      VectorPolar(f->planepts[2], radius, t + dt, p + dp);

      for (int k = 0; k < 3; k++)
        VectorAdd(f->planepts[k], mid, f->planepts[k]);
		}
  }

  p = float((sides - 1) * dp - Q_PI / 2);
	for(i = 0; i <= sides-1; i++)
	{
		t = i * dt;

    f = Face_Alloc();
	  f->texdef = *texdef;
	  f->next = b->brush_faces;
	  b->brush_faces = f;

    VectorPolar(f->planepts[0], radius, t, p);
    VectorPolar(f->planepts[1], radius, t + dt, p + dp);
    VectorPolar(f->planepts[2], radius, t + dt, p);

    for (int k = 0; k < 3; k++)
      VectorAdd(f->planepts[k], mid, f->planepts[k]);
	}

	Brush_AddToList (b, &selected_brushes);

	Entity_LinkBrush (world_entity, b);

	Brush_Build( b );

	Sys_UpdateWindows (W_ALL);
}



void Brush_FitTexture( brush_t *b, int nHeight, int nWidth )
{
	face_t *face;

	for (face = b->brush_faces ; face ; face=face->next)
  {
    Face_FitTexture( face, nHeight, nWidth );
  }
}


void ClearBounds (vec3_t mins, vec3_t maxs)
{
	mins[0] = mins[1] = mins[2] = MAX_WORLD_COORD;
	maxs[0] = maxs[1] = maxs[2] = MIN_WORLD_COORD;
}


void AddPointToBounds (const vec3_t v, vec3_t mins, vec3_t maxs)
{
	int		i;
	vec_t	val;

	for (i=0 ; i<3 ; i++)
	{
		val = v[i];
		if (val < mins[i])
			mins[i] = val;
		if (val > maxs[i])
			maxs[i] = val;
	}
}



void Face_FitTexture( face_t * face, int nHeight, int nWidth )
{
  winding_t *w;
  vec3_t   mins,maxs;
  int i;
  float width, height, temp;
  float rot_width, rot_height;
  float cosv,sinv,ang;
  float min_t, min_s, max_t, max_s;
  float s,t;
	vec3_t	vecs[2];
  vec3_t   coords[4];
	texdef_t	*td;

  if (nHeight < 1)
  {
    nHeight = 1;
  }
  if (nWidth < 1)
  {
    nWidth = 1;
  }

  ClearBounds (mins, maxs);

	td = &face->texdef;
	w = face->face_winding;
	if (!w)
	{
    return;
	}
  for (i=0 ; i<w->numpoints ; i++)
  {
    AddPointToBounds( w->points[i], mins, maxs );
  }

	//fitcode
   // 
   // get the current angle
   //
  // hack, swap sides if 90 or 270 degrees, fixes a non-fit bug...
  //
	if (fabs(td->rotate - 90) < 0.001f || fabs(td->rotate - 270) < 0.001f)
	{
		int iTemp = nHeight;
					nHeight = nWidth;
							  nWidth = iTemp;
	}

	ang = td->rotate / 180 * Q_PI;
	sinv = sin(ang);
	cosv = cos(ang);

	// get natural texture axis
	TextureAxisFromPlane(&face->plane, vecs[0], vecs[1]);

  min_s = DotProduct( mins, vecs[0] );
  min_t = DotProduct( mins, vecs[1] );
  max_s = DotProduct( maxs, vecs[0] );
  max_t = DotProduct( maxs, vecs[1] );
  width = max_s - min_s;
  height = max_t - min_t;
  coords[0][0] = min_s;
  coords[0][1] = min_t;
  coords[1][0] = max_s;
  coords[1][1] = min_t;
  coords[2][0] = min_s;
  coords[2][1] = max_t;
  coords[3][0] = max_s;
  coords[3][1] = max_t;
  min_s = min_t = MAX_WORLD_COORD;
  max_s = max_t = MIN_WORLD_COORD;
  for (i=0; i<4; i++)
  {
    s = cosv * coords[i][0] - sinv * coords[i][1];
	  t = sinv * coords[i][0] + cosv * coords[i][1];
////////////
      if (s > max_s) 
      {
		  max_s = s;
      }
      if (s < min_s) 
      {
		  min_s = s;
      }
	  if (t < min_t) 
	  {
          min_t = t;
	  }
	  if (t > max_t) 
	  {
          max_t = t;
	  }

////////////
    if (i&1)
    {
      if (s > max_s) 
      {
        max_s = s;
      }
    }
    else
    {
      if (s < min_s) 
      {
        min_s = s;
      }
      if (i<2)
      {
        if (t < min_t) 
        {
          min_t = t;
        }
      }
      else
      {
        if (t > max_t) 
        {
          max_t = t;
        }
      }
    }
  }
  rot_width =  (max_s - min_s);
  rot_height = (max_t - min_t);
  td->scale[0] = (rot_width/((float)(face->d_texture->width*nWidth)));
  td->scale[1] = (rot_height/((float)(face->d_texture->height*nHeight)));

  td->shift[0] = min_s/td->scale[0];
  temp = (int)(td->shift[0] / (face->d_texture->width*nWidth));
  temp = (temp+1)*face->d_texture->width*nWidth;
  td->shift[0] = (int)(temp - td->shift[0])%(face->d_texture->width*nWidth);
  
  td->shift[1] = min_t/td->scale[1];
  temp = (int)(td->shift[1] / (face->d_texture->height*nHeight));
  temp = (temp+1)*(face->d_texture->height*nHeight);
  td->shift[1] = (int)(temp - td->shift[1])%(face->d_texture->height*nHeight);
}



