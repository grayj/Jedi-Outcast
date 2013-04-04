// 
// Preliminary patch stuff
//
// 


#include "stdafx.h"
#include "..\qe3.h"
#include "..\DialogInfo.h"
#include "..\CapDialog.h"
#include "..\groupnames.h"

// externs
extern void MemFile_fprintf(CMemFile* pMemFile, const char* pText, ...);
extern face_t *Face_Alloc( void );
void _Write3DMatrix (FILE *f, int y, int x, int z, float *m);
void _Write3DMatrix (CMemFile *f, int y, int x, int z, float *m);



#define	CBLOCK_SUBDIVISIONS		6


#define	MIN_PATCH_WIDTH		3
#define	MIN_PATCH_HEIGHT 	3

#define	MAX_PATCH_WIDTH		16
#define	MAX_PATCH_HEIGHT	16

// patch type info
// type in lower 16 bits, flags in upper
// endcaps directly follow this patch in the list

// types
#define PATCH_GENERIC     0x00000000    // generic flat patch
#define PATCH_CYLINDER    0x00000001    // cylinder
#define PATCH_BEVEL       0x00000002    // bevel
#define PATCH_ENDCAP      0x00000004    // endcap
#define PATCH_HEMISPHERE  0x00000008    // hemisphere
#define PATCH_CONE        0x00000010    // cone

// behaviour styles
#define PATCH_CAP         0x00001000    // flat patch applied as a cap
#define PATCH_SEAM        0x00002000    // flat patch applied as a seam
#define PATCH_THICK       0x00004000    // patch applied as a thick portion

// styles
#define PATCH_BEZIER      0x00000000    // default bezier
#define PATCH_BSPLINE     0x10000000    // bspline

#define PATCH_TYPEMASK     0x00000fff    // 
#define PATCH_BTYPEMASK    0x0000f000    // 
#define PATCH_STYLEMASK    0xffff0000    // 

typedef struct {
	vec3_t		xyz;
	float		st[2];
	float		lightmap[2];
	vec3_t		normal;
} drawVert_t;



typedef struct {
	int			width, height;		// in control points, not patches
  int     contents, flags, value, type;
	qtexture_t *d_texture;
	//--float		ctrl[MAX_PATCH_WIDTH][MAX_PATCH_HEIGHT][5];
	drawVert_t ctrl[MAX_PATCH_WIDTH][MAX_PATCH_HEIGHT];
  brush_t *pSymbiot;
  bool bSelected;
  bool bOverlay;
  bool bDirty;
  int  nListID;
} patchMesh_t;


// FIXME: this needs to be dynamic
#define	MAX_PATCH_MESHES	(4096 + 1024)
patchMesh_t		patchMeshes[MAX_PATCH_MESHES];
int numPatchMeshes = 0;

// used for a save spot
patchMesh_t patchSave;

// Tracks the selected patch for point manipulation/update. FIXME: Need to revert back to a generalized 
// brush approach
//--int  g_nSelectedPatch = -1;  

// HACK: for tracking which view generated the click
// as we dont want to deselect a point on a same point
// click if it is from a different view
int  g_nPatchClickedView = -1;
bool g_bSameView = false;


// globals
bool g_bPatchShowBounds = true;
bool g_bPatchWireFrame = false;
bool g_bPatchWeld = true;
bool g_bPatchDrillDown = true;
bool g_bPatchInsertMode = false;
bool g_bPatchBendMode = false;
int  g_nPatchBendState = -1;
int  g_nPatchInsertState = -1;
int  g_nBendOriginIndex = 0;
vec3_t g_vBendOrigin;

bool g_bPatchAxisOnRow = true;
int  g_nPatchAxisIndex = 0;
bool g_bPatchLowerEdge = true;

// BEND states
enum
{
  BEND_SELECT_ROTATION = 0,
  BEND_SELECT_ORIGIN,
  BEND_SELECT_EDGE,
  BEND_BENDIT,
  BEND_STATE_COUNT
};

const char *g_pBendStateMsg[] =
{
  "Use TAB to cycle through available bend axis. Press ENTER when the desired one is highlighted.",
  "Use TAB to cycle through available rotation axis. This will LOCK around that point. You may also use Shift + Middle Click to select an arbitrary point. Press ENTER when the desired one is highlighted",
  "Use TAB to choose which side to bend. Press ENTER when the desired one is highlighted.",
  "Use the MOUSE to bend the patch. It uses the same ui rules as Free Rotation. Press ENTER to accept the bend, press ESC to abandon it and exit Bend mode",
  ""
};

// INSERT states
enum
{
  INSERT_SELECT_EDGE = 0,
  INSERT_STATE_COUNT
};

const char* g_pInsertStateMsg[] =
{
  "Use TAB to cycle through available rows/columns for insertion/deletion. Press INS to insert at the highlight, DEL to remove the pair"
};


float *g_InversePoints[1024];

const float fFullBright = 1.0;
const float fLowerLimit = .50;
const float fDec = .05;
void _SetColor(face_t* f, float fColor[3])
{
  return;
  fColor[0] = f->d_color[0];
  fColor[1] = f->d_color[1];
  fColor[2] = f->d_color[2];
  qglColor3fv(fColor);
}


void _DecColor(float fColor[3])
{
  return;
  fColor[0] -= fDec;
  fColor[1] -= fDec ;
  fColor[2] -= fDec;
  for (int i = 0; i < 3; i++)
  {
    if (fColor[i] <= fLowerLimit)
    {
      fColor[0] = fFullBright;
      fColor[1] = fFullBright;
      fColor[2] = fFullBright;
      break;
    }
  }
	qglColor3fv(fColor);
}

#define VectorClear(x) {x[0] = x[1] = x[2] = 0;}
vec_t __VectorNormalize (vec3_t in, vec3_t out)
{
	vec_t	length, ilength;

	length = sqrt (in[0]*in[0] + in[1]*in[1] + in[2]*in[2]);
	if (length == 0)
	{
		VectorClear (out);
		return 0;
	}

	ilength = 1.0/length;
	out[0] = in[0]*ilength;
	out[1] = in[1]*ilength;
	out[2] = in[2]*ilength;

	return length;
}


void Patch_SetType(patchMesh_t *p, int nType)
{
  p->type = (p->type & PATCH_STYLEMASK) | nType;
}

void Patch_SetStyle(patchMesh_t *p, int nStyle)
{
  p->type = (p->type & PATCH_TYPEMASK) | nStyle;
}




/*
===============
InterpolateInteriorPoints
===============
*/
void InterpolateInteriorPoints( patchMesh_t *p ) 
{
	int		i, j, k;
	int		next, prev;

	for ( i = 0 ; i < p->width ; i += 2 ) 
  {

    next = ( i == p->width - 1 ) ? 1 : ( i + 1 ) % p->width;
    prev = ( i == 0 ) ? p->width - 2 : i - 1;

#if 0
		if ( i == 0 ) 
    {
			next = ( i + 1 ) % p->width;
			prev = p->width - 2;		      // joined wrap case
		} 
    else if ( i == p->width - 1 ) 
    {
			next = 1;
			prev = i - 1;
		} 
    else 
    {
			next = ( i + 1 ) % p->width;
			prev = i - 1;
		}
#endif

		for ( j = 0 ; j < p->height ; j++ ) 
    {
			for ( k = 0 ; k < 3 ; k++ ) 
      {
				p->ctrl[i][j].xyz[k] = ( p->ctrl[next][j].xyz[k] + p->ctrl[prev][j].xyz[k] ) * 0.5;
			}
		}
	}
}

/*
=================
MakeMeshNormals

=================
*/
int	neighbors[8][2] = {
	{0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}
};

void Patch_MeshNormals(patchMesh_t& in ) 
{
	int		i, j, k, dist;
	vec3_t	normal;
	vec3_t	sum;
	int		count;
	vec3_t	base;
	vec3_t	delta;
	int		x, y;
	drawVert_t	*dv;
	vec3_t		around[8], temp;
	qboolean	good[8];
	qboolean	wrapWidth, wrapHeight;
	float		len;

	wrapWidth = false;
	for ( i = 0 ; i < in.height ; i++ ) 
  {
		VectorSubtract( in.ctrl[i*in.width]->xyz, 
			              in.ctrl[i*in.width+in.width-1]->xyz, delta );
		len = VectorLength( delta );
		if ( len > 1.0 ) 
    {
			break;
		}
	}
	if ( i == in.height ) 
  {
		wrapWidth = true;
	}

	wrapHeight = false;
	for ( i = 0 ; i < in.width ; i++ ) 
  {
		VectorSubtract( in.ctrl[i]->xyz, 
			              in.ctrl[i + (in.height-1)*in.width]->xyz, delta );
		len = VectorLength( delta );
		if ( len > 1.0 ) 
    {
			break;
		}
	}
	if ( i == in.width) 
  {
		wrapHeight = true;
	}


	for ( i = 0 ; i < in.width ; i++ ) 
  {
		for ( j = 0 ; j < in.height ; j++ ) 
    {
			count = 0;
			//--dv = reinterpret_cast<drawVert_t*>(in.ctrl[j*in.width+i]);
			dv = &in.ctrl[i][j];
			VectorCopy( dv->xyz, base );
			for ( k = 0 ; k < 8 ; k++ ) 
      {
				VectorClear( around[k] );
				good[k] = false;

				for ( dist = 1 ; dist <= 3 ; dist++ ) 
        {
					x = i + neighbors[k][0] * dist;
					y = j + neighbors[k][1] * dist;
					if ( wrapWidth ) 
          {
						if ( x < 0 ) 
            {
							x = in.width - 1 + x;
						} 
            else if ( x >= in.width ) 
            {
							x = 1 + x - in.width;
						}
					}
					if ( wrapHeight ) 
          {
						if ( y < 0 ) 
            {
							y = in.height - 1 + y;
						} 
            else if ( y >= in.height ) 
            {
							y = 1 + y - in.height;
						}
					}

					if ( x < 0 || x >= in.width || y < 0 || y >= in.height ) 
          {
						break;					// edge of patch
					}
					//--VectorSubtract( in.ctrl[y*in.width+x]->xyz, base, temp );
					VectorSubtract( in.ctrl[x][y].xyz, base, temp );
					if ( __VectorNormalize( temp, temp ) == 0 ) 
          {
						continue;				// degenerate edge, get more dist
					} 
          else                 
          {
						good[k] = true;
						VectorCopy( temp, around[k] );
						break;					// good edge
					}
				}
			}

			VectorClear( sum );
			for ( k = 0 ; k < 8 ; k++ ) 
      {
				if ( !good[k] || !good[(k+1)&7] ) 
        {
					continue;	// didn't get two points
				}
				CrossProduct( around[(k+1)&7], around[k], normal );
				if ( __VectorNormalize( normal, normal ) == 0 ) 
        {
					continue;
				}
				VectorAdd( normal, sum, sum );
				count++;
			}
			if ( count == 0 ) 
      {
        //printf("bad normal\n");
				count = 1;
        //continue;
			}
			__VectorNormalize( sum, dv->normal );
		}
	}
}




/*
==================
Patch_CalcBounds
==================
*/
void Patch_CalcBounds(patchMesh_t *p, vec3_t& vMin, vec3_t& vMax)
{
  vMin[0] = vMin[1] = vMin[2] = MAX_WORLD_COORD;
  vMax[0] = vMax[1] = vMax[2] = MIN_WORLD_COORD;

  p->bDirty = true;
  for (int w = 0; w < p->width; w++)
  {
    for (int h = 0; h < p->height; h++)
    {
      for (int j = 0; j < 3; j++)
      {
        float f = p->ctrl[w][h].xyz[j];
        if (f < vMin[j])
          vMin[j] = f;
        if (f > vMax[j])
          vMax[j] = f;
      }
    }
  }
}

/*
==================
Brush_RebuildBrush
==================
*/
void Brush_RebuildBrush(brush_t *b, vec3_t vMins, vec3_t vMaxs)
{
  //
  // Total hack job 
  // Rebuilds a brush
	int		i, j;
	face_t	*f, *next;
	vec3_t	pts[4][2];
  texdef_t	texdef;
	// free faces

  for (j = 0; j < 3; j++)
  {
    if ((int)vMins[j] == (int)vMaxs[j])
    {
      vMins[j] -= 4;
      vMaxs[j] += 4;
    }
  }

  
  for (f=b->brush_faces ; f ; f=next)
	{
		next = f->next;
    if (f)
      texdef = f->texdef;
    Face_Free( f );
	}

  b->brush_faces = NULL;

  // left the last face so we can use its texdef

	for (i=0 ; i<3 ; i++)
		if (vMaxs[i] < vMins[i])
			Error ("Brush_RebuildBrush: backwards");

	pts[0][0][0] = vMins[0];
	pts[0][0][1] = vMins[1];
	
	pts[1][0][0] = vMins[0];
	pts[1][0][1] = vMaxs[1];
	
	pts[2][0][0] = vMaxs[0];
	pts[2][0][1] = vMaxs[1];
	
	pts[3][0][0] = vMaxs[0];
	pts[3][0][1] = vMins[1];
	
	for (i=0 ; i<4 ; i++)
	{
		pts[i][0][2] = vMins[2];
		pts[i][1][0] = pts[i][0][0];
		pts[i][1][1] = pts[i][0][1];
		pts[i][1][2] = vMaxs[2];
	}

	for (i=0 ; i<4 ; i++)
	{
		f = Face_Alloc();
		f->texdef = texdef;
		f->texdef.flags &= ~SURF_KEEP;
		f->texdef.contents &= ~CONTENTS_KEEP;
		f->texdef.flags |= SURF_PATCH; 
		f->next = b->brush_faces;
		b->brush_faces = f;
		j = (i+1)%4;

		VectorCopy (pts[j][1], f->planepts[0]);
		VectorCopy (pts[i][1], f->planepts[1]);
		VectorCopy (pts[i][0], f->planepts[2]);
	}
	
	f = Face_Alloc();
	f->texdef = texdef;
  f->texdef.flags &= ~SURF_KEEP;
	f->texdef.contents &= ~CONTENTS_KEEP;
  f->texdef.flags |= SURF_PATCH; 
	f->next = b->brush_faces;
	b->brush_faces = f;

	VectorCopy (pts[0][1], f->planepts[0]);
	VectorCopy (pts[1][1], f->planepts[1]);
	VectorCopy (pts[2][1], f->planepts[2]);

	f = Face_Alloc();
	f->texdef = texdef;
  f->texdef.flags &= ~SURF_KEEP;
	f->texdef.contents &= ~CONTENTS_KEEP;
  f->texdef.flags |= SURF_PATCH; 
	f->next = b->brush_faces;
	b->brush_faces = f;

	VectorCopy (pts[2][0], f->planepts[0]);
	VectorCopy (pts[1][0], f->planepts[1]);
	VectorCopy (pts[0][0], f->planepts[2]);

  Brush_Build(b);
}

void Patch_Rebuild(patchMesh_t *p)
{
  vec3_t vMin, vMax;
  Patch_CalcBounds(p, vMin, vMax);
  Brush_RebuildBrush(p->pSymbiot, vMin, vMax);
  p->bDirty = true;
}



/*
==================
AddBrushForPatch
==================
 adds a patch brush and ties it to this patch id
*/
brush_t* AddBrushForPatch(int n, bool bLinkToWorld = true)
{
  // find the farthest points in x,y,z
  vec3_t vMin, vMax;
  Patch_CalcBounds(&patchMeshes[n], vMin, vMax);

  for (int j = 0; j < 3; j++)
  {
    if (vMin[j] == vMax[j])
    {
      vMin[j] -= 4;
      vMax[j] += 4;
    }
  }

  brush_t *b = Brush_Create(vMin, vMax, &g_qeglobals.d_texturewin.texdef);
	face_t		*f;
	for (f=b->brush_faces ; f ; f=f->next) 
  {
		f->texdef.flags |= SURF_PATCH; 
	}

  // FIXME: this entire type of linkage needs to be fixed
  b->nPatchID = n;
  patchMeshes[n].pSymbiot = b;
  patchMeshes[n].bSelected = false;
  patchMeshes[n].bOverlay = false;
  patchMeshes[n].bDirty = true;
  patchMeshes[n].nListID = -1;

  if (bLinkToWorld)
  {
    Brush_AddToList (b, &active_brushes);
	  Entity_LinkBrush (world_entity, b);
    Brush_Build(b);
  }

  return b;
}

void Patch_SetPointIntensities(int n)
{
#if 0
	patchMesh_t	*p = patchMeshes[n];
  for (int i = 0; i < p->width; i++)
  {
    for (int j = 0; j < p->height; j++)
    {

    }
  }
#endif
}

// very approximate widths and heights

/*
==================
Patch_Width
==================
*/
float Patch_Width(patchMesh_t *p)
{
  float f = 0;
  for (int i = 0; i < p->width-1; i++)
  {
    vec3_t vTemp;
    VectorSubtract(p->ctrl[i][0].xyz, p->ctrl[i+1][0].xyz, vTemp);
    f += VectorLength(vTemp);
  }
  return f;
}

float Patch_WidthDistanceTo(patchMesh_t *p, int j)
{
  float f = 0;
  for (int i = 0; i < j; i++)
  {
    vec3_t vTemp;
    VectorSubtract(p->ctrl[i][0].xyz, p->ctrl[i+1][0].xyz, vTemp);
    f += VectorLength(vTemp);
  }
  return f;
}



/*
==================
Patch_Height
==================
*/
float Patch_Height(patchMesh_t *p)
{
  float f = 0;
  for (int i = 0; i < p->height-1; i++)
  {
    vec3_t vTemp;
    VectorSubtract(p->ctrl[0][i].xyz, p->ctrl[0][i+1].xyz, vTemp);
    f += VectorLength(vTemp);
  }
  return f;
}

float Patch_HeightDistanceTo(patchMesh_t *p, int j)
{
  float f = 0;
  for (int i = 0; i < j; i++)
  {
    vec3_t vTemp;
    VectorSubtract(p->ctrl[0][i].xyz, p->ctrl[0][i+1].xyz, vTemp);
    f += VectorLength(vTemp);
  }
  return f;
}



/*
==================
Patch_Naturalize
==================
texture = TotalTexture * LengthToThisControlPoint / TotalControlPointLength

dist( this control point to first control point ) / dist ( last control pt to first)
*/
void Patch_Naturalize(patchMesh_t *p)
{
  int nWidth = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? p->d_texture->width * fTEXTURE_SCALE : p->d_texture->width;
  int nHeight = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? p->d_texture->height * fTEXTURE_SCALE : p->d_texture->height;
  float fPWidth = Patch_Width(p);
  float fPHeight = Patch_Height(p);
  float xAccum = 0;
  for ( int i = 0 ; i < p->width ; i++ ) 
  {
    float yAccum = 0;
	  for ( int j = 0 ; j < p->height ; j++ ) 
    {
		  p->ctrl[i][j].st[0] = (fPWidth / nWidth) * xAccum / fPWidth;
		  p->ctrl[i][j].st[1] = (fPHeight / nHeight) * yAccum / fPHeight;
		  yAccum = Patch_HeightDistanceTo(p,j+1);
      //p->ctrl[i][j][3] = (fPWidth / nWidth) * (float)i / (p->width - 1);
		  //p->ctrl[i][j][4] = (fPHeight/ nHeight) * (float)j / (p->height - 1);
	  }
    xAccum = Patch_WidthDistanceTo(p,i+1);
  }
  p->bDirty = true;
}

/*
  if (bIBevel)
  {
    VectorCopy(p->ctrl[1][0], p->ctrl[1][1]);
  }

  if (bIEndcap)
  {
    VectorCopy(p->ctrl[3][0], p->ctrl[4][1]);
    VectorCopy(p->ctrl[2][0], p->ctrl[3][1]);
    VectorCopy(p->ctrl[2][0], p->ctrl[2][1]);
    VectorCopy(p->ctrl[2][0], p->ctrl[1][1]);
    VectorCopy(p->ctrl[1][0], p->ctrl[0][1]);
    VectorCopy(p->ctrl[1][0], p->ctrl[0][2]);
    VectorCopy(p->ctrl[1][0], p->ctrl[1][2]);
    VectorCopy(p->ctrl[2][0], p->ctrl[2][2]);
    VectorCopy(p->ctrl[3][0], p->ctrl[3][2]);
    VectorCopy(p->ctrl[3][0], p->ctrl[4][2]);
  }
*/

int Index3By[][2] =
{
  {0,0},
  {1,0},
  {2,0},
  {2,1},
  {2,2},
  {1,2},
  {0,2},
  {0,1},
  {0,0},
  {0,0},
  {0,0},
  {0,0},
  {0,0},
  {0,0},
  {0,0}
};

int Index5By[][2] =
{
  {0,0},
  {1,0},
  {2,0},
  {3,0},
  {4,0},
  {4,1},
  {4,2},
  {4,3},
  {4,4},
  {3,4},
  {2,4},
  {1,4},
  {0,4},
  {0,3},
  {0,2},
  {0,1}
};



int Interior3By[][2] =
{
  {1,1}
};

int Interior5By[][2] =
{
  {1,1},
  {2,1},
  {3,1},
  {1,2},
  {2,2},
  {3,2},
  {1,3},
  {2,3},
  {3,3}
};

int Interior3ByCount = sizeof(Interior3By) / sizeof(int[2]);
int Interior5ByCount = sizeof(Interior5By) / sizeof(int[2]);

face_t* Patch_GetAxisFace(patchMesh_t *p)
{
  face_t *f = NULL;
  vec3_t vTemp;
  brush_t *b = p->pSymbiot;

	for (f = b->brush_faces ; f ; f = f->next) 
  {
    VectorSubtract(f->face_winding->points[1], f->face_winding->points[0], vTemp);
    int nScore = 0;

    // default edge faces on caps are 8 high so
    // as soon as we hit one that is bigger it should be on the right axis
    for (int j = 0; j < 3; j++)
    {
      if (vTemp[j] > 8)
        nScore++;
    }

    if (nScore > 0)
      break;
  }

  if (f == NULL)
    f = b->brush_faces;
  return f;
}

int g_nFaceCycle = 0;

face_t* nextFace(patchMesh_t *p)
{
  brush_t *b = p->pSymbiot;
  face_t *f = NULL;
  int n = 0;
	for (f = b->brush_faces ; f && n <= g_nFaceCycle; f = f->next) 
  {
    n++;
  }
  g_nFaceCycle++;
  if (g_nFaceCycle > 5)
  {
    g_nFaceCycle =0;
    f = b->brush_faces;
  }

  return f;
}


extern void EmitTextureCoordinates ( float *xyzst, qtexture_t *q, face_t *f);
void Patch_CapTexture(patchMesh_t *p, bool bFaceCycle = false)
{
  Patch_MeshNormals(*p);
  face_t *f = (bFaceCycle) ? nextFace(p) : Patch_GetAxisFace(p);
  vec3_t vSave;
  VectorCopy(f->plane.normal, vSave);
  
  float fRotate = f->texdef.rotate;
  f->texdef.rotate = 0;
  float fScale[2];
  fScale[0] = f->texdef.scale[0];
  fScale[1] = f->texdef.scale[1];
  f->texdef.scale[0] = fTEXTURE_SCALE;
  f->texdef.scale[1] = fTEXTURE_SCALE;
  float fShift[2];
  fShift[0] = f->texdef.shift[0];
  fShift[1] = f->texdef.shift[1];
  f->texdef.shift[0] = 0;
  f->texdef.shift[1] = 0;
  
  for (int i = 0; i < p->width; i++)
  {
    for (int j = 0; j < p->height; j++)
    {
      //--VectorCopy(p->ctrl[0][0].normal, f->plane.normal);
		  EmitTextureCoordinates( p->ctrl[i][j].xyz, f->d_texture, f);
    }
  }
  VectorCopy(vSave, f->plane.normal);
  f->texdef.rotate = fRotate;
  f->texdef.scale[0] = fScale[0];
  f->texdef.scale[1] = fScale[1];
  f->texdef.shift[0] = fShift[0];
  f->texdef.shift[1] = fShift[1];
  p->bDirty = true;
}

void FillPatch(patchMesh_t *p, vec3_t v)
{
  for (int i = 0; i < p->width; i++)
  {
    for (int j = 0; j < p->height; j++)
    {
      VectorCopy(v, p->ctrl[i][j].xyz);
    }
  }
}

brush_t* Cap(patchMesh_t *pParent, bool bByColumn, bool bFirst)
{
  brush_t *b;
  patchMesh_t *p;
  vec3_t vMin, vMax;
  int i, j;

  bool bSmall = true;
  // make a generic patch
  if (pParent->width <= 9)
  {
    b = Patch_GenericMesh(3, 3, 2, false);
  }
  else
  {
    b = Patch_GenericMesh(5, 5, 2, false);
    bSmall = false;
  }

  if (!b)
  {
    Sys_Printf("Unable to cap. Make sure you ungroup before re-capping.");
    return NULL;
  }

  p = &patchMeshes[b->nPatchID];
  p->type |= PATCH_CAP;

  vMin[0] = vMin[1] = vMin[2] = MAX_WORLD_COORD;
  vMax[0] = vMax[1] = vMax[2] = MIN_WORLD_COORD;

  // we seam the column edge, FIXME: this might need to be able to seem either edge
  // 
  int nSize = (bByColumn) ? pParent->width : pParent->height;
  int nIndex = (bFirst) ? 0 : (bByColumn) ? pParent->height-1 : pParent->width-1;

  FillPatch(p, pParent->ctrl[0][nIndex].xyz);

  for (i = 0; i < nSize; i++)
  {
    if (bByColumn)
    {
      if (bSmall)
      {
        VectorCopy(pParent->ctrl[i][nIndex].xyz, p->ctrl[Index3By[i][0]][Index3By[i][1]].xyz);
      }
      else
      {
        VectorCopy(pParent->ctrl[i][nIndex].xyz, p->ctrl[Index5By[i][0]][Index5By[i][1]].xyz);
      }
    }
    else
    {
      if (bSmall)
      {
        VectorCopy(pParent->ctrl[nIndex][i].xyz, p->ctrl[Index3By[i][0]][Index3By[i][1]].xyz);
      }
      else
      {
        VectorCopy(pParent->ctrl[nIndex][i].xyz, p->ctrl[Index5By[i][0]][Index5By[i][1]].xyz);
      }
    }
  
    for (j = 0; j < 3; j++)
    {
      float f = (bSmall) ? p->ctrl[Index3By[i][0]][Index3By[i][1]].xyz[j] : p->ctrl[Index5By[i][0]][Index5By[i][1]].xyz[j];
      if (f < vMin[j])
        vMin[j] = f;
      if (f > vMax[j])
        vMax[j] = f;
    }
  }

  vec3_t vTemp;
  for (j = 0; j < 3; j++)
  {
    vTemp[j] = vMin[j] + abs((vMax[j] - vMin[j]) * 0.5);
  }

  int nCount = (bSmall) ? Interior3ByCount : Interior5ByCount;
  for (j = 0; j < nCount; j++)
  {
    if (bSmall)
    {
      VectorCopy(vTemp, p->ctrl[Interior3By[j][0]][Interior3By[j][1]].xyz);
    }
    else
    {
      VectorCopy(vTemp, p->ctrl[Interior5By[j][0]][Interior5By[j][1]].xyz);
    }
  }

  if (bFirst)
  {
    drawVert_t vertTemp;
    for (i = 0; i < p->width; i++)
    {
      for (j = 0; j < p->height / 2; j++)
      {
        memcpy(&vertTemp, &p->ctrl[i][p->height - 1- j], sizeof (drawVert_t));
        memcpy(&p->ctrl[i][p->height - 1 - j], &p->ctrl[i][j], sizeof(drawVert_t));
        memcpy(&p->ctrl[i][j], &vertTemp, sizeof(drawVert_t));
      }
    }
  }

  Patch_Rebuild(p);
  Patch_CapTexture(p);
  return p->pSymbiot;
}

brush_t* CapSpecial(patchMesh_t *pParent, int nType, bool bFirst)
{
  brush_t *b;
  patchMesh_t *p;
  vec3_t vMin, vMax, vTemp;
  int i, j;

  if (nType == CCapDialog::IENDCAP)
    b = Patch_GenericMesh(5, 3, 2, false);
  else
    b = Patch_GenericMesh(3, 3, 2, false);

  if (!b)
  {
    Sys_Printf("Unable to cap. Make sure you ungroup before re-capping.");
    return NULL;
  }

  p = &patchMeshes[b->nPatchID];
  p->type |= PATCH_CAP;

  vMin[0] = vMin[1] = vMin[2] = MAX_WORLD_COORD;
  vMax[0] = vMax[1] = vMax[2] = MIN_WORLD_COORD;

  int nSize = pParent->width;
  int nIndex = (bFirst) ? 0 : pParent->height-1;

  // parent bounds are used for some things
  Patch_CalcBounds(pParent, vMin, vMax);

  for (j = 0; j < 3; j++)
  {
    vTemp[j] = vMin[j] + abs((vMax[j] - vMin[j]) * 0.5);
  }

  if (nType == CCapDialog::IBEVEL)
  {
    VectorCopy(pParent->ctrl[0][nIndex].xyz, p->ctrl[0][0].xyz);
    VectorCopy(pParent->ctrl[2][nIndex].xyz, p->ctrl[0][2].xyz);
    VectorCopy(pParent->ctrl[1][nIndex].xyz, p->ctrl[0][1].xyz);
    VectorCopy(pParent->ctrl[1][nIndex].xyz, p->ctrl[2][2].xyz);
    VectorCopy(pParent->ctrl[1][nIndex].xyz, p->ctrl[1][0].xyz);
    VectorCopy(pParent->ctrl[1][nIndex].xyz, p->ctrl[1][1].xyz);
    VectorCopy(pParent->ctrl[1][nIndex].xyz, p->ctrl[1][2].xyz);
    VectorCopy(pParent->ctrl[1][nIndex].xyz, p->ctrl[2][0].xyz);
    VectorCopy(pParent->ctrl[1][nIndex].xyz, p->ctrl[2][1].xyz);
  }
  else if (nType == CCapDialog::BEVEL)
  {
    vec3_t p1, p2, p3, p4, temp, dir;

    VectorCopy(pParent->ctrl[0][nIndex].xyz, p3);
    VectorCopy(pParent->ctrl[1][nIndex].xyz, p1);
    VectorCopy(pParent->ctrl[2][nIndex].xyz, p2);

    VectorSubtract(p3, p2, dir);
    VectorNormalize(dir);
    VectorSubtract(p1, p2, temp);
    vec_t dist = _DotProduct(temp, dir);

    VectorScale(dir, dist, temp);

    VectorAdd(p2, temp, temp);

    VectorSubtract(temp, p1, temp);
    VectorScale(temp, 2, temp);
    VectorAdd(p1, temp, p4);

    VectorCopy(p4, p->ctrl[0][0].xyz);
    VectorCopy(p4, p->ctrl[1][0].xyz);
    VectorCopy(p4, p->ctrl[0][1].xyz);
    VectorCopy(p4, p->ctrl[1][1].xyz);
    VectorCopy(p4, p->ctrl[0][2].xyz);
    VectorCopy(p4, p->ctrl[1][2].xyz);
    VectorCopy(p3, p->ctrl[2][0].xyz);
    VectorCopy(p1, p->ctrl[2][1].xyz);
    VectorCopy(p2, p->ctrl[2][2].xyz);

  }
  else if (nType == CCapDialog::ENDCAP)
  {
    VectorAdd(pParent->ctrl[4][nIndex].xyz, pParent->ctrl[0][nIndex].xyz, vTemp);
    VectorScale(vTemp, 0.5, vTemp);
    VectorCopy(pParent->ctrl[0][nIndex].xyz, p->ctrl[0][0].xyz);
                       VectorCopy(vTemp, p->ctrl[1][0].xyz);
    VectorCopy(pParent->ctrl[4][nIndex].xyz, p->ctrl[2][0].xyz);

    VectorCopy(pParent->ctrl[2][nIndex].xyz, p->ctrl[0][2].xyz);
    VectorCopy(pParent->ctrl[2][nIndex].xyz, p->ctrl[1][2].xyz);
    VectorCopy(pParent->ctrl[2][nIndex].xyz, p->ctrl[2][2].xyz);
    VectorCopy(pParent->ctrl[2][nIndex].xyz, p->ctrl[1][1].xyz);
    
    VectorCopy(pParent->ctrl[1][nIndex].xyz, p->ctrl[0][1].xyz);
    VectorCopy(pParent->ctrl[3][nIndex].xyz, p->ctrl[2][1].xyz);
  }
  else
  {
    VectorCopy(pParent->ctrl[0][nIndex].xyz, p->ctrl[0][0].xyz);
    VectorCopy(pParent->ctrl[1][nIndex].xyz, p->ctrl[1][0].xyz);
    VectorCopy(pParent->ctrl[2][nIndex].xyz, p->ctrl[2][0].xyz);
    VectorCopy(pParent->ctrl[3][nIndex].xyz, p->ctrl[3][0].xyz);
    VectorCopy(pParent->ctrl[4][nIndex].xyz, p->ctrl[4][0].xyz);
    
    VectorCopy(pParent->ctrl[1][nIndex].xyz, p->ctrl[0][1].xyz);
    VectorCopy(pParent->ctrl[1][nIndex].xyz, p->ctrl[1][1].xyz);
    VectorCopy(pParent->ctrl[2][nIndex].xyz, p->ctrl[2][1].xyz);
    VectorCopy(pParent->ctrl[3][nIndex].xyz, p->ctrl[3][1].xyz);
    VectorCopy(pParent->ctrl[3][nIndex].xyz, p->ctrl[4][1].xyz);

    VectorCopy(pParent->ctrl[1][nIndex].xyz, p->ctrl[0][2].xyz);
    VectorCopy(pParent->ctrl[1][nIndex].xyz, p->ctrl[1][2].xyz);
    VectorCopy(pParent->ctrl[2][nIndex].xyz, p->ctrl[2][2].xyz);
    VectorCopy(pParent->ctrl[3][nIndex].xyz, p->ctrl[3][2].xyz);
    VectorCopy(pParent->ctrl[3][nIndex].xyz, p->ctrl[4][2].xyz);
  }


  bool bEndCap = (nType == CCapDialog::ENDCAP || nType == CCapDialog::IENDCAP);
  if ((!bFirst && !bEndCap) || (bFirst && bEndCap))
  {
    drawVert_t vertTemp;
    for (i = 0; i < p->width; i++)
    {
      for (j = 0; j < p->height / 2; j++)
      {
        memcpy(&vertTemp, &p->ctrl[i][p->height - 1- j], sizeof (drawVert_t));
        memcpy(&p->ctrl[i][p->height - 1 - j], &p->ctrl[i][j], sizeof(drawVert_t));
        memcpy(&p->ctrl[i][j], &vertTemp, sizeof(drawVert_t));
      }
    }
  }

  //--Patch_CalcBounds(p, vMin, vMax);
  //--Brush_RebuildBrush(p->pSymbiot, vMin, vMax);
  Patch_Rebuild(p);
  Patch_CapTexture(p);
  return p->pSymbiot;
}


void Patch_CapCurrent(bool bInvertedBevel, bool bInvertedEndcap)
{
  patchMesh_t *pParent;
  brush_t *b[4];
  b[0] = b[1] = b[2] = b[3] = NULL;
  int nIndex = 0;
	for (brush_t *pb = selected_brushes.next ; pb != NULL && pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      pParent = &patchMeshes[pb->nPatchID];
      // decide which if any ends we are going to cap
      // if any of these compares hit, it is a closed patch and as such
      // the generic capping will work.. if we do not find a closed edge 
      // then we need to ask which kind of cap to add
      if (VectorCompare(pParent->ctrl[0][0].xyz, pParent->ctrl[pParent->width-1][0].xyz))
      {
        b[nIndex++] = Cap(pParent, true, false);
      }
      if (VectorCompare(pParent->ctrl[0][pParent->height-1].xyz, pParent->ctrl[pParent->width-1][pParent->height-1].xyz))
      {
        b[nIndex++] = Cap(pParent, true, true);
      }
      if (VectorCompare(pParent->ctrl[0][0].xyz, pParent->ctrl[0][pParent->height-1].xyz))
      {
        b[nIndex++] = Cap(pParent, false, false);
      }
      if (VectorCompare(pParent->ctrl[pParent->width-1][0].xyz, pParent->ctrl[pParent->width-1][pParent->height-1].xyz))
      {
        b[nIndex++] = Cap(pParent, false, true);
      }
    }
  }

  // if we did not cap anything with the above tests
  if (nIndex == 0)
  {
    CCapDialog dlg;
    if (dlg.DoModal() == IDOK)
    {
      b[nIndex++] = CapSpecial(pParent, dlg.getCapType(), false);
      b[nIndex++] = CapSpecial(pParent, dlg.getCapType(), true);
    }
  }

  if (nIndex > 0)
  {
    while (nIndex > 0)
    {
      nIndex--;
      if (b[nIndex])
      {
        Select_Brush(b[nIndex]);
      }
    }
    eclass_t *pecNew = Eclass_ForName("func_group", false);
    if (pecNew)
    {
      entity_t *e = Entity_Create(pecNew);
      SetKeyValue(e, "type", "patchCapped");
    }
  }
}


//FIXME: Table drive all this crap
//
void GenerateEndCaps(brush_t *brushParent, bool bBevel, bool bEndcap, bool bInverted)
{
  brush_t *b, *b2;
  patchMesh_t *p, *p2, *pParent;
  vec3_t vTemp, vMin, vMax;
  int i, j;

  pParent = &patchMeshes[brushParent->nPatchID];

  Patch_CalcBounds(pParent, vMin, vMax);
  // basically generate two endcaps, place them, and link the three brushes with a func_group

  if (pParent->width > 9)
    b = Patch_GenericMesh(5, 3, 2, false);
  else
    b = Patch_GenericMesh(3, 3, 2, false);
  p = &patchMeshes[b->nPatchID];

  vMin[0] = vMin[1] = vMin[2] = MAX_WORLD_COORD;
  vMax[0] = vMax[1] = vMax[2] = MIN_WORLD_COORD;

  for (i = 0; i < pParent->width; i++)
  {
    VectorCopy(pParent->ctrl[i][0].xyz, p->ctrl[Index3By[i][0]][Index3By[i][1]].xyz);
    for (j = 0; j < 3; j++)
    {
      if (pParent->ctrl[i][0].xyz[j] < vMin[j])
        vMin[j] = pParent->ctrl[i][0].xyz[j];
      if (pParent->ctrl[i][0].xyz[j] > vMax[j])
        vMax[j] = pParent->ctrl[i][0].xyz[j];
    }
  }

  for (j = 0; j < 3; j++)
  {
    vTemp[j] = vMin[j] + abs((vMax[j] - vMin[j]) * 0.5);
  }

  for (i = 0; i < Interior3ByCount; i++)
  {
    VectorCopy(vTemp, p->ctrl[Interior3By[i][0]][Interior3By[i][1]].xyz);
  }

  Patch_CalcBounds(p, vMin, vMax);
  Brush_RebuildBrush(p->pSymbiot, vMin, vMax);
  Select_Brush(p->pSymbiot);
  return;

  bool bCreated = false;

  if (bInverted)
  {
    if (bBevel)
    {
      b = Patch_GenericMesh(3, 3, 2, false);
      p = &patchMeshes[b->nPatchID];
      VectorCopy(p->ctrl[2][2].xyz, p->ctrl[1][2].xyz);
      VectorCopy(p->ctrl[2][2].xyz, p->ctrl[2][1].xyz);
      VectorCopy(p->ctrl[2][2].xyz, p->ctrl[0][1].xyz);
      VectorCopy(p->ctrl[2][2].xyz, p->ctrl[1][0].xyz);
      VectorCopy(p->ctrl[2][2].xyz, p->ctrl[1][1].xyz);
      VectorCopy(p->ctrl[2][0].xyz, p->ctrl[0][0].xyz);

      b2 = Patch_GenericMesh(3, 3, 2, false);
      p2 = &patchMeshes[b2->nPatchID];
      VectorCopy(p2->ctrl[2][2].xyz, p2->ctrl[1][2].xyz);
      VectorCopy(p2->ctrl[2][2].xyz, p2->ctrl[2][1].xyz);
      VectorCopy(p2->ctrl[2][2].xyz, p2->ctrl[0][1].xyz);
      VectorCopy(p2->ctrl[2][2].xyz, p2->ctrl[1][0].xyz);
      VectorCopy(p2->ctrl[2][2].xyz, p2->ctrl[1][1].xyz);
      VectorCopy(p2->ctrl[2][0].xyz, p2->ctrl[0][0].xyz);


      bCreated = true;

    }
    else if (bEndcap)
    {
      b = Patch_GenericMesh(5, 5, 2, false);
      p = &patchMeshes[b->nPatchID];
      VectorCopy(p->ctrl[4][4].xyz, p->ctrl[4][3].xyz);
      VectorCopy(p->ctrl[0][4].xyz, p->ctrl[1][4].xyz);
      VectorCopy(p->ctrl[0][4].xyz, p->ctrl[2][4].xyz);
      VectorCopy(p->ctrl[0][4].xyz, p->ctrl[3][4].xyz);

      VectorCopy(p->ctrl[4][0].xyz, p->ctrl[4][1].xyz);
      VectorCopy(p->ctrl[0][0].xyz, p->ctrl[1][0].xyz);
      VectorCopy(p->ctrl[0][0].xyz, p->ctrl[2][0].xyz);
      VectorCopy(p->ctrl[0][0].xyz, p->ctrl[3][0].xyz);

      for (i = 1; i < 4; i++)
      {
        for (j = 0; j < 4; j++)
        {
          VectorCopy(p->ctrl[4][i].xyz, p->ctrl[j][i].xyz);
        }
      }


      b2 = Patch_GenericMesh(5, 5, 2, false);
      p2 = &patchMeshes[b2->nPatchID];
      VectorCopy(p2->ctrl[4][4].xyz, p2->ctrl[4][3].xyz);
      VectorCopy(p2->ctrl[0][4].xyz, p2->ctrl[1][4].xyz);
      VectorCopy(p2->ctrl[0][4].xyz, p2->ctrl[2][4].xyz);
      VectorCopy(p2->ctrl[0][4].xyz, p2->ctrl[3][4].xyz);

      VectorCopy(p2->ctrl[4][0].xyz, p2->ctrl[4][1].xyz);
      VectorCopy(p2->ctrl[0][0].xyz, p2->ctrl[1][0].xyz);
      VectorCopy(p2->ctrl[0][0].xyz, p2->ctrl[2][0].xyz);
      VectorCopy(p2->ctrl[0][0].xyz, p2->ctrl[3][0].xyz);

      for (i = 1; i < 4; i++)
      {
        for (j = 0; j < 4; j++)
        {
          VectorCopy(p2->ctrl[4][i].xyz, p2->ctrl[j][i].xyz);
        }
      }


      bCreated = true;
    }
  }
  else
  {
    if (bBevel)
    {
      b = Patch_GenericMesh(3, 3, 2, false);
      p = &patchMeshes[b->nPatchID];
      VectorCopy(p->ctrl[2][0].xyz, p->ctrl[2][1].xyz);
      VectorCopy(p->ctrl[0][0].xyz, p->ctrl[1][0].xyz);
      VectorCopy(p->ctrl[0][0].xyz, p->ctrl[2][0].xyz);

      b2 = Patch_GenericMesh(3, 3, 2, false);
      p2 = &patchMeshes[b2->nPatchID];
      VectorCopy(p2->ctrl[2][0].xyz, p2->ctrl[2][1].xyz);
      VectorCopy(p2->ctrl[0][0].xyz, p2->ctrl[1][0].xyz);
      VectorCopy(p2->ctrl[0][0].xyz, p2->ctrl[2][0].xyz);
      bCreated = true;
    }
    else if (bEndcap)
    {
      b = Patch_GenericMesh(5, 5, 2, false);
      p = &patchMeshes[b->nPatchID];
      VectorCopy(p->ctrl[0][0].xyz, p->ctrl[1][0].xyz);
      VectorCopy(p->ctrl[0][0].xyz, p->ctrl[2][0].xyz);
      VectorCopy(p->ctrl[0][0].xyz, p->ctrl[3][0].xyz);
      VectorCopy(p->ctrl[4][0].xyz, p->ctrl[4][1].xyz);
      VectorCopy(p->ctrl[0][0].xyz, p->ctrl[4][0].xyz);

      VectorCopy(p->ctrl[0][4].xyz, p->ctrl[1][4].xyz);
      VectorCopy(p->ctrl[0][4].xyz, p->ctrl[2][4].xyz);
      VectorCopy(p->ctrl[0][4].xyz, p->ctrl[3][4].xyz);
      VectorCopy(p->ctrl[4][4].xyz, p->ctrl[4][3].xyz);
      VectorCopy(p->ctrl[0][4].xyz, p->ctrl[4][4].xyz);

      b2 = Patch_GenericMesh(5, 5, 2, false);
      p2 = &patchMeshes[b2->nPatchID];
      VectorCopy(p2->ctrl[0][0].xyz, p2->ctrl[1][0].xyz);
      VectorCopy(p2->ctrl[0][0].xyz, p2->ctrl[2][0].xyz);
      VectorCopy(p2->ctrl[0][0].xyz, p2->ctrl[3][0].xyz);
      VectorCopy(p2->ctrl[4][0].xyz, p2->ctrl[4][1].xyz);
      VectorCopy(p2->ctrl[0][0].xyz, p2->ctrl[4][0].xyz);

      VectorCopy(p2->ctrl[0][4].xyz, p2->ctrl[1][4].xyz);
      VectorCopy(p2->ctrl[0][4].xyz, p2->ctrl[2][4].xyz);
      VectorCopy(p2->ctrl[0][4].xyz, p2->ctrl[3][4].xyz);
      VectorCopy(p2->ctrl[4][4].xyz, p2->ctrl[4][3].xyz);
      VectorCopy(p2->ctrl[0][4].xyz, p2->ctrl[4][4].xyz);
      bCreated = true;
    }
    else
    {
      b = Patch_GenericMesh(3, 3, 2, false);
      p = &patchMeshes[b->nPatchID];
      
      VectorCopy(p->ctrl[0][1].xyz, vTemp);
      VectorCopy(p->ctrl[0][2].xyz, p->ctrl[0][1].xyz)
      VectorCopy(p->ctrl[1][2].xyz, p->ctrl[0][2].xyz)
      VectorCopy(p->ctrl[2][2].xyz, p->ctrl[1][2].xyz)
      VectorCopy(p->ctrl[2][1].xyz, p->ctrl[2][2].xyz)
      VectorCopy(p->ctrl[2][0].xyz, p->ctrl[2][1].xyz)
      VectorCopy(p->ctrl[1][0].xyz, p->ctrl[2][0].xyz)
      VectorCopy(p->ctrl[0][0].xyz, p->ctrl[1][0].xyz)
      VectorCopy(vTemp, p->ctrl[0][0].xyz)

      b2 = Patch_GenericMesh(3, 3, 2, false);
      p2 = &patchMeshes[b2->nPatchID];
      VectorCopy(p2->ctrl[0][1].xyz, vTemp);
      VectorCopy(p2->ctrl[0][2].xyz, p2->ctrl[0][1].xyz)
      VectorCopy(p2->ctrl[1][2].xyz, p2->ctrl[0][2].xyz)
      VectorCopy(p2->ctrl[2][2].xyz, p2->ctrl[1][2].xyz)
      VectorCopy(p2->ctrl[2][1].xyz, p2->ctrl[2][2].xyz)
      VectorCopy(p2->ctrl[2][0].xyz, p2->ctrl[2][1].xyz)
      VectorCopy(p2->ctrl[1][0].xyz, p2->ctrl[2][0].xyz)
      VectorCopy(p2->ctrl[0][0].xyz, p2->ctrl[1][0].xyz)
      VectorCopy(vTemp, p2->ctrl[0][0].xyz)
      bCreated = true;
    }
  }

  if (bCreated)
  {
    drawVert_t vertTemp;
    for (i = 0; i < p->width; i++)
    {
      for (j = 0; j < p->height; j++)
      {
        p->ctrl[i][j].xyz[2] = vMin[2];
        p2->ctrl[i][j].xyz[2] = vMax[2];
      }

      for (j = 0; j < p->height / 2; j++)
      {
        memcpy(&vertTemp, &p->ctrl[i][p->height - 1- j], sizeof (drawVert_t));
        memcpy(&p->ctrl[i][p->height - 1 - j], &p->ctrl[i][j], sizeof(drawVert_t));
        memcpy(&p->ctrl[i][j], &vertTemp, sizeof(drawVert_t));
      }

    }
    //Select_Delete();

    Patch_CalcBounds(p, vMin, vMax);
    Brush_RebuildBrush(p->pSymbiot, vMin, vMax);
    Patch_CalcBounds(p2, vMin, vMax);
    Brush_RebuildBrush(p2->pSymbiot, vMin, vMax);
    Select_Brush(p->pSymbiot);
    Select_Brush(p2->pSymbiot);
  }
  else
  {
    Select_Delete();
  }
  //Select_Brush(brushParent);

}


/*
===============
BrushToPatchMesh
===============
*/
void Patch_BrushToMesh(bool bCone, bool bBevel, bool bEndcap, bool bSquare, int nHeight)
{
	brush_t		*b;
	patchMesh_t	*p;
	int			i,j;

	if (!QE_SingleBrush())
		return;

	b = selected_brushes.next;

	p = &patchMeshes[numPatchMeshes];

	p->d_texture = b->brush_faces->d_texture;

  p->height = nHeight;

  p->type = PATCH_CYLINDER;
  if (bBevel)
  {
    p->type = PATCH_BEVEL;
    p->width = 3;
    int nStep = (b->maxs[2] - b->mins[2]) / (p->height-1);
    int nStart = b->mins[2];
    for (i = 0; i < p->height; i++)
    {
	    p->ctrl[0][i].xyz[0] =  b->mins[0];
	    p->ctrl[0][i].xyz[1] =  b->mins[1];
      p->ctrl[0][i].xyz[2] = nStart;

	    p->ctrl[1][i].xyz[0] =  b->maxs[0];
	    p->ctrl[1][i].xyz[1] =  b->mins[1];
      p->ctrl[1][i].xyz[2] = nStart;

	    p->ctrl[2][i].xyz[0] =  b->maxs[0];
	    p->ctrl[2][i].xyz[1] =  b->maxs[1];
      p->ctrl[2][i].xyz[2] = nStart;
      nStart += nStep;
    }
  }
  else if (bEndcap)
  {
    p->type = PATCH_ENDCAP;
    p->width = 5;
    int nStep = (b->maxs[2] - b->mins[2]) / (p->height-1);
    int nStart = b->mins[2];
    for (i = 0; i < p->height; i++)
    {
	    p->ctrl[0][i].xyz[0] =  b->mins[0];
	    p->ctrl[0][i].xyz[1] =  b->mins[1];
      p->ctrl[0][i].xyz[2] = nStart;

	    p->ctrl[1][i].xyz[0] =  b->mins[0];
	    p->ctrl[1][i].xyz[1] =  b->maxs[1];
      p->ctrl[1][i].xyz[2] = nStart;

	    p->ctrl[2][i].xyz[0] =  b->mins[0] + ((b->maxs[0] - b->mins[0]) * 0.5);
	    p->ctrl[2][i].xyz[1] =  b->maxs[1];
      p->ctrl[2][i].xyz[2] = nStart;
	   
      p->ctrl[3][i].xyz[0] =  b->maxs[0];
	    p->ctrl[3][i].xyz[1] =  b->maxs[1];
      p->ctrl[3][i].xyz[2] = nStart;

	    p->ctrl[4][i].xyz[0] =  b->maxs[0];
	    p->ctrl[4][i].xyz[1] =  b->mins[1];
      p->ctrl[4][i].xyz[2] = nStart;
      nStart += nStep;
    }
  }
  else
  {
    p->width = 9;
	  p->ctrl[1][0].xyz[0] =  b->mins[0];
	  p->ctrl[1][0].xyz[1] =  b->mins[1];

	  p->ctrl[3][0].xyz[0] =  b->maxs[0];
	  p->ctrl[3][0].xyz[1] =  b->mins[1];

	  p->ctrl[5][0].xyz[0] =  b->maxs[0];
	  p->ctrl[5][0].xyz[1] =  b->maxs[1];

	  p->ctrl[7][0].xyz[0] =  b->mins[0];
	  p->ctrl[7][0].xyz[1] =  b->maxs[1];

	  for ( i = 1 ; i < p->width - 1 ; i += 2 ) 
    {

      p->ctrl[i][0].xyz[2] =  b->mins[2];

		  VectorCopy( p->ctrl[i][0].xyz, p->ctrl[i][2].xyz );

		  p->ctrl[i][2].xyz[2] =  b->maxs[2];

		  p->ctrl[i][1].xyz[0] = ( p->ctrl[i][0].xyz[0] + p->ctrl[i][2].xyz[0] ) * 0.5;
		  p->ctrl[i][1].xyz[1] = ( p->ctrl[i][0].xyz[1] + p->ctrl[i][2].xyz[1] ) * 0.5;
		  p->ctrl[i][1].xyz[2] = ( p->ctrl[i][0].xyz[2] + p->ctrl[i][2].xyz[2] ) * 0.5;
	  }
	  InterpolateInteriorPoints( p );

    if (bSquare)
    {
      for (i = 0; i < p->width-1; i ++)
      {
        for (j = 0; j < p->height; j++)
        {
          VectorCopy(p->ctrl[i+1][j].xyz, p->ctrl[i][j].xyz);
        }
      }
      for (j = 0; j < p->height; j++)
      {
        VectorCopy(p->ctrl[0][j].xyz, p->ctrl[8][j].xyz);
      }
    }
  }


  Patch_Naturalize(p);

  if (bCone)
  {
    p->type = PATCH_CONE;
    float xc = (b->maxs[0] + b->mins[0]) * 0.5; 
    float yc = (b->maxs[1] + b->mins[1]) * 0.5; 

    for ( i = 0 ; i < p->width ; i ++)
    {
      p->ctrl[i][2].xyz[0] = xc;
      p->ctrl[i][2].xyz[1] = yc;
    }
  }
/*
  if (bEndcap || bBevel)
  {
    if (bInverted)
    {
      for ( i = 0 ; i < p->height ; i ++)
      {
        if (bBevel)
        {
          VectorCopy(p->ctrl[7][i], p->ctrl[0][i]);
          VectorCopy(p->ctrl[7][i], p->ctrl[8][i]);
          VectorCopy(p->ctrl[3][i], p->ctrl[2][i]);
          VectorCopy(p->ctrl[5][i], p->ctrl[1][i]);
          VectorCopy(p->ctrl[5][i], p->ctrl[4][i]);
          VectorCopy(p->ctrl[5][i], p->ctrl[6][i]);
        }
        else
        {
          VectorCopy(p->ctrl[4][i], p->ctrl[8][i]);
          VectorCopy(p->ctrl[1][i], p->ctrl[0][i]);
          VectorCopy(p->ctrl[1][i], p->ctrl[10][i]);
          VectorCopy(p->ctrl[3][i], p->ctrl[2][i]);
          VectorCopy(p->ctrl[5][i], p->ctrl[4][i]);
          VectorCopy(p->ctrl[7][i], p->ctrl[6][i]);
          VectorCopy(p->ctrl[5][i], p->ctrl[7][i]);
          VectorCopy(p->ctrl[3][i], p->ctrl[9][i]);
        }
      }
    }
    else
    {
      for ( i = 0 ; i < p->height ; i ++)
      {
        VectorCopy(p->ctrl[1][i], p->ctrl[2][i]);
        VectorCopy(p->ctrl[7][i], p->ctrl[6][i]);
        if (bBevel)
        {
          VectorCopy(p->ctrl[5][i], p->ctrl[4][i]);
        }
      }
    }
  }
*/
  
  b = AddBrushForPatch(numPatchMeshes);

  numPatchMeshes++;

#if 1
    Select_Delete();
    Select_Brush(b);
#else
  if (!bCone)
  {
    Select_Delete();
    Select_Brush(b);
    GenerateEndCaps(b, bBevel, bEndcap, bInverted);
    eclass_t *pecNew = Eclass_ForName("func_group", false);
    if (pecNew)
    {
      Entity_Create(pecNew);
    }
  }
  else
  {
    Select_Delete();
    Select_Brush(b);
  }
#endif

}

/*
==================
Patch_GenericMesh
==================
*/
brush_t* Patch_GenericMesh(int nWidth, int nHeight, int nOrientation, bool bDeleteSource)
{
  int i,j;

  if (nHeight < 3 || nHeight > 15 || nWidth < 3 || nWidth > 15)
  {
    Sys_Printf("Invalid patch width or height.\n");
    return NULL;
  }

  patchMesh_t* p = &patchMeshes[numPatchMeshes];
	p->d_texture = Texture_ForName(g_qeglobals.d_texturewin.texdef.name);

	p->width = nWidth;
	p->height = nHeight;
  p->type = PATCH_GENERIC;

  int nFirst = 0;
  int nSecond = 1;
  if (nOrientation == 0)
  {
    nFirst = 1;
    nSecond = 2;
  }
  else if (nOrientation == 1)
  {
    nSecond = 2;
  }

	if (!QE_SingleBrush())
		return NULL;

	brush_t *b = selected_brushes.next;

  int xStep = b->mins[nFirst];
  float xAdj = abs((b->maxs[nFirst] - b->mins[nFirst]) / (nWidth - 1));
  float yAdj = abs((b->maxs[nSecond] - b->mins[nSecond]) / (nHeight - 1));

  for (i = 0; i < nWidth; i++)
  {
    int yStep = b->mins[nSecond];
    for (j = 0; j < nHeight; j++)
    {
      p->ctrl[i][j].xyz[nFirst] = xStep;
      p->ctrl[i][j].xyz[nSecond] = yStep;
      p->ctrl[i][j].xyz[nOrientation] = 0;
      yStep += yAdj;
    }
    xStep += xAdj;
  }

  Patch_Naturalize(p);

  b = AddBrushForPatch(numPatchMeshes);
  if (bDeleteSource)
  {
    Select_Delete();
    Select_Brush(b);
  }

  numPatchMeshes++;
  return b;
  //g_qeglobals.d_select_mode = sel_curvepoint;
}

/*
==================
PointInMoveList
==================
*/
int PointInMoveList(float *pf)
{
  for (int i = 0; i < g_qeglobals.d_num_move_points; i++)
  {
    if (pf == &g_qeglobals.d_move_points[i][0])
      return i;
  }
  return -1;
}

/*
==================
PointValueInMoveList
==================
*/
int PointValueInMoveList(vec3_t v)
{
  for (int i = 0; i < g_qeglobals.d_num_move_points; i++)
  {
    if (VectorCompare(v, g_qeglobals.d_move_points[i]))
      return i;
  }
  return -1;
}


/*
==================
RemovePointFromMoveList
==================
*/
void RemovePointFromMoveList(vec3_t v)
{
  int n;
  while ( (n = PointValueInMoveList(v)) >= 0)
  {
    for (int i = n; i < g_qeglobals.d_num_move_points-1; i++)
    {
      g_qeglobals.d_move_points[i] = g_qeglobals.d_move_points[i+1];
    }
    g_qeglobals.d_num_move_points--;
  }
}

/*
==================
ColumnSelected
==================
*/
bool ColumnSelected(patchMesh_t* p, int nCol)
{
  for (int i = 0; i < p->height; i++)
  {
    if (PointInMoveList(p->ctrl[nCol][i].xyz) == -1)
      return false;
  }
  return true;
}

/*
==================
AddPoint
==================
*/
void AddPoint(patchMesh_t* p, vec3_t v, bool bWeldOrDrill = true)
{
  int nDim1 = (g_pParentWnd->ActiveXY()->GetViewType() == YZ) ? 1 : 0;
  int nDim2 = (g_pParentWnd->ActiveXY()->GetViewType() == XY) ? 1 : 2;
  g_qeglobals.d_move_points[g_qeglobals.d_num_move_points++] = v;
  if ((g_bPatchWeld || g_bPatchDrillDown) && bWeldOrDrill)
  {
	  for ( int i = 0 ; i < p->width ; i++ ) 
    {
		  for ( int j = 0 ; j < p->height ; j++ ) 
      {
        if (g_bPatchWeld)
        {
          if ( VectorCompare(v, p->ctrl[i][j].xyz)
            && PointInMoveList(p->ctrl[i][j].xyz) == -1)
          {
            g_qeglobals.d_move_points[g_qeglobals.d_num_move_points++] = p->ctrl[i][j].xyz;
            continue;
          }
        }
        if (g_bPatchDrillDown && g_nPatchClickedView != W_CAMERA)
        {
          if ( (fabs(v[nDim1] - p->ctrl[i][j].xyz[nDim1]) <= EQUAL_EPSILON) 
             &&(fabs(v[nDim2] - p->ctrl[i][j].xyz[nDim2]) <= EQUAL_EPSILON)) 
          {
            if (PointInMoveList(p->ctrl[i][j].xyz) == -1)
            {
              g_qeglobals.d_move_points[g_qeglobals.d_num_move_points++] = p->ctrl[i][j].xyz;
              continue;
            }
          }
#if 0
          int l = 0;
	        for ( int k = 0; k < 2; k++ )
          {
		        if (fabs(v[k] - p->ctrl[i][j].xyz[k]) > EQUAL_EPSILON)
              continue;
             l++;
          }
          if (l >= 2 && PointInMoveList(p->ctrl[i][j].xyz) == -1)
          {
            g_qeglobals.d_move_points[g_qeglobals.d_num_move_points++] = p->ctrl[i][j].xyz;
            continue;
          }
#endif
        }
      }
    }
  }
#if 0
  if (g_qeglobals.d_num_move_points == 1)
  {
    // single point selected
    // FIXME: the two loops can probably be reduced to one
	  for ( int i = 0 ; i < p->width ; i++ ) 
    {
		  for ( int j = 0 ; j < p->height ; j++ ) 
      {
        int n = PointInMoveList(v);
        if (n >= 0)
        {
          if (((i & 0x01) && (j & 0x01)) == 0)
          {
            // put any sibling fixed points
            // into the inverse list
            int p1, p2, p3, p4;
            p1 = i + 2;
            p2 = i - 2;
            p3 = j + 2;
            p4 = j - 2;
            if (p1 < p->width)
            {

            }
            if (p2 >= 0)
            {
            }
            if (p3 < p->height)
            {
            }
            if (p4 >= 0)
            {
            }
          }
        }
      }
    }
  }
#endif
}

/*
==================
SelectRow
==================
*/
void SelectRow(patchMesh_t* p, int nRow, bool bMulti)
{
  if (!bMulti)
    g_qeglobals.d_num_move_points = 0;
  for (int i = 0; i < p->width; i++)
  {
    AddPoint(p, p->ctrl[i][nRow].xyz, false);
  }
  //Sys_Printf("Selected Row %d\n", nRow);
}

/*
==================
SelectColumn
==================
*/
void SelectColumn(patchMesh_t* p, int nCol, bool bMulti)
{
  if (!bMulti)
    g_qeglobals.d_num_move_points = 0;
  for (int i = 0; i < p->height; i++)
  {
    AddPoint(p, p->ctrl[nCol][i].xyz, false);
  }
  //Sys_Printf("Selected Col %d\n", nCol);
}


/*
==================
AddPatchMovePoint
==================
*/
void AddPatchMovePoint(vec3_t v, bool bMulti, bool bFull)
{
  if (!g_bSameView && !bMulti && !bFull)
  {
    g_bSameView = true;
    return;
  }

	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
	    patchMesh_t* p = &patchMeshes[pb->nPatchID];
	    for ( int i = 0 ; i < p->width ; i++ ) 
      {
		    for ( int j = 0 ; j < p->height ; j++ ) 
        {
          if (VectorCompare(v, p->ctrl[i][j].xyz))
          {
            if (PointInMoveList(p->ctrl[i][j].xyz) == -1)
            {
              if (bFull)        // if we want the full row/col this is on
              {
                SelectColumn(p, i, bMulti);
              }
              else
              {
                if (!bMulti)
                  g_qeglobals.d_num_move_points = 0;
                AddPoint(p, p->ctrl[i][j].xyz);
                //Sys_Printf("Selected col:row %d:%d\n", i, j);
              }
              //--if (!bMulti)
              return;
            }
            else
            {
              if (bFull)
              {
                if (ColumnSelected(p, i))
                {
                  SelectRow(p, j, bMulti);
                }
                else
                {
                  SelectColumn(p, i, bMulti);
                }
                return;
              }
              if (g_bSameView)
              {
                RemovePointFromMoveList(v);
                return;
              }
            }
          }
		    }
	    }
    }
  }
}

/*
==================
Patch_UpdateSelected
==================
*/
void Patch_UpdateSelected(vec3_t vMove)
{
  int i, j;
  for (i=0 ; i < g_qeglobals.d_num_move_points ; i++)
  {
	  VectorAdd (g_qeglobals.d_move_points[i], vMove, g_qeglobals.d_move_points[i]);
    if (g_qeglobals.d_num_move_points == 1)
    {
    }
  }

	//--patchMesh_t* p = &patchMeshes[g_nSelectedPatch];
	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
	    patchMesh_t* p = &patchMeshes[pb->nPatchID];


      g_qeglobals.d_numpoints = 0;
	    for (i = 0 ; i < p->width ; i++ ) 
      {
		    for ( j = 0 ; j < p->height ; j++ ) 
        {
	        VectorCopy (p->ctrl[i][j].xyz, g_qeglobals.d_points[g_qeglobals.d_numpoints]);
	        g_qeglobals.d_numpoints++;
		    }
      }
	    
      vec3_t vMin, vMax;
      Patch_CalcBounds(p, vMin, vMax);
      Brush_RebuildBrush(p->pSymbiot, vMin, vMax);
    }
  }
  //Brush_Free(p->pSymbiot);
  //Select_Brush(AddBrushForPatch(g_nSelectedPatch));
}



/*
===============
SampleSinglePatch
===============
*/
void SampleSinglePatch (float ctrl[3][3][5], float u, float v, float out[5]) {
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
DrawSinglePatch
===================
*/
void DrawSinglePatch (float ctrl[3][3][5], bool bPoints) 
{
	int		i, j;
	float	u, v;
	float	verts[CBLOCK_SUBDIVISIONS+1][CBLOCK_SUBDIVISIONS+1][5];

	for (i = 0 ; i <= CBLOCK_SUBDIVISIONS ; i++) {
		for (j = 0 ; j <= CBLOCK_SUBDIVISIONS ; j++) {
			u = (float)i / CBLOCK_SUBDIVISIONS;
			v = (float)j / CBLOCK_SUBDIVISIONS;
			SampleSinglePatch (ctrl, u, v, verts[i][j]);
		}
	}

	for (i = 0 ; i < CBLOCK_SUBDIVISIONS ; i++) {
    qglBegin (GL_QUAD_STRIP);
		for (j = 0 ; j <= CBLOCK_SUBDIVISIONS ; j++) {
			qglTexCoord2fv( verts[i+1][j] + 3 );
			qglVertex3fv( verts[i+1][j] );
			qglTexCoord2fv( verts[i][j] + 3 );
			qglVertex3fv( verts[i][j] );
		}
	  qglEnd ();
	}

}

/*
=================
DrawPatchMesh
=================
*/
//FIXME: this routine needs to be reorganized.. should be about 1/4 the size and complexity
void DrawPatchMesh( patchMesh_t *pm, bool bPoints, bool bShade = false ) {
	int		i, j, k, l;
	float	ctrl[3][3][5];

  bool bOverlay = pm->bOverlay;

  //--float fColor[3];
  //--if (bShade)
  //--{
  //--  face_t *f = pm->pSymbiot->brush_faces;
  //--  _SetColor(f, fColor);
  //--}
  if (g_PrefsDlg.m_bDisplayLists)
  {
    if (pm->bDirty || pm->nListID <= 0)
    {
      if (pm->nListID <= 0)
        pm->nListID = qglGenLists(1);
      if (pm->nListID > 0)
      {
        qglNewList(pm->nListID, GL_COMPILE_AND_EXECUTE);
      }
      Patch_MeshNormals(*pm);
	    for ( i = 0 ; i + 2 < pm->width ; i += 2 ) 
      {
		    for ( j = 0 ; j + 2 < pm->height ; j += 2 ) 
        {
			    for ( k = 0 ; k < 3 ; k++ ) 
          {
				    for ( l = 0 ; l < 3 ; l++ ) 
            {
					    ctrl[k][l][0] = pm->ctrl[ i + k ][ j + l ].xyz[ 0 ];
					    ctrl[k][l][1] = pm->ctrl[ i + k ][ j + l ].xyz[ 1 ];
					    ctrl[k][l][2] = pm->ctrl[ i + k ][ j + l ].xyz[ 2 ];
					    ctrl[k][l][3] = pm->ctrl[ i + k ][ j + l ].xyz[ 3 ];
					    ctrl[k][l][4] = pm->ctrl[ i + k ][ j + l ].xyz[ 4 ];
				    }
			    }
          //--if (bShade)
          //--{
          //--  _DecColor(fColor);
          //--}
          //--qglColor3f(pm->ctrl[i][j].lightmap[0],pm->ctrl[i][j].lightmap[0],pm->ctrl[i][j].lightmap[0]);
		    
          DrawSinglePatch( ctrl, bPoints );
        }
      }

      if (pm->nListID > 0)
      {
        qglEndList();
        pm->bDirty = false;
      }
    }
    else
    {
      qglCallList(pm->nListID);
    }
  }
  else
  {
	  for ( i = 0 ; i + 2 < pm->width ; i += 2 ) 
    {
		  for ( j = 0 ; j + 2 < pm->height ; j += 2 ) 
      {
			  for ( k = 0 ; k < 3 ; k++ ) 
        {
				  for ( l = 0 ; l < 3 ; l++ ) 
          {
					  ctrl[k][l][0] = pm->ctrl[ i + k ][ j + l ].xyz[ 0 ];
					  ctrl[k][l][1] = pm->ctrl[ i + k ][ j + l ].xyz[ 1 ];
					  ctrl[k][l][2] = pm->ctrl[ i + k ][ j + l ].xyz[ 2 ];
					  ctrl[k][l][3] = pm->ctrl[ i + k ][ j + l ].xyz[ 3 ];
					  ctrl[k][l][4] = pm->ctrl[ i + k ][ j + l ].xyz[ 4 ];
				  }
			  }
        //--if (bShade)
        //--{
        //--  _DecColor(fColor);
        //--}
			  DrawSinglePatch( ctrl, bPoints );
      }
    }
  }

  vec3_t* pSelectedPoints[256];
  int nIndex = 0;

  qglPushAttrib(GL_CURRENT_BIT);
  //--qglDisable(GL_BLEND);
  //--qglDisable (GL_DEPTH_TEST);
  //--qglDisable (GL_TEXTURE_2D);

  // FIXME: this bend painting code needs to be rolled up significantly as it is a cluster fuck right now
  if (bPoints && (g_qeglobals.d_select_mode == sel_curvepoint || g_qeglobals.d_select_mode == sel_area || g_bPatchBendMode || g_bPatchInsertMode))
  {
    bOverlay = false;

    // bending or inserting
    if (g_bPatchBendMode || g_bPatchInsertMode)
    {
      qglPointSize(6);
      if (g_bPatchAxisOnRow)
      {
        qglColor3f(1, 0, 1);
        qglBegin(GL_POINTS);
        for (i = 0; i < pm->width; i++)
        {
			    qglVertex3fv(reinterpret_cast<float(*)>(&pm->ctrl[i][g_nPatchAxisIndex].xyz));
        }
        qglEnd();
      
        // could do all of this in one loop but it was pretty messy
        if (g_bPatchInsertMode)
        {
          qglColor3f(0, 0, 1);
          qglBegin(GL_POINTS);
          for (i = 0; i < pm->width; i++)
          {
			      qglVertex3fv(reinterpret_cast<float(*)>(&pm->ctrl[i][g_nPatchAxisIndex].xyz));
			      qglVertex3fv(reinterpret_cast<float(*)>(&pm->ctrl[i][g_nPatchAxisIndex+1].xyz));
          }
          qglEnd();
        }
        else
        {
          if (g_nPatchBendState == BEND_SELECT_EDGE || g_nPatchBendState == BEND_BENDIT || g_nPatchBendState == BEND_SELECT_ORIGIN)
          {
            qglColor3f(0, 0, 1);
            qglBegin(GL_POINTS);
            if (g_nPatchBendState == BEND_SELECT_ORIGIN)
            {
              qglVertex3fv(g_vBendOrigin);
            }
            else
            {
              for (i = 0; i < pm->width; i++)
              {
                if (g_bPatchLowerEdge)
                {
                  for (j = 0; j < g_nPatchAxisIndex; j++)
			              qglVertex3fv(reinterpret_cast<float(*)>(&pm->ctrl[i][j].xyz));
                }
                else
                {
                  for (j = pm->height-1; j > g_nPatchAxisIndex; j--)
			              qglVertex3fv(reinterpret_cast<float(*)>(&pm->ctrl[i][j].xyz));
                }
              }
            }
            qglEnd();
          }
        }
      }
      else
      {
        qglColor3f(1, 0, 1);
        qglBegin(GL_POINTS);
        for (i = 0; i < pm->height; i++)
        {
			    qglVertex3fv(reinterpret_cast<float(*)>(&pm->ctrl[g_nPatchAxisIndex][i].xyz));
        }
        qglEnd();
        
        // could do all of this in one loop but it was pretty messy
        if (g_bPatchInsertMode)
        {
          qglColor3f(0, 0, 1);
          qglBegin(GL_POINTS);
          for (i = 0; i < pm->height; i++)
          {
			      qglVertex3fv(reinterpret_cast<float(*)>(&pm->ctrl[g_nPatchAxisIndex][i].xyz));
			      qglVertex3fv(reinterpret_cast<float(*)>(&pm->ctrl[g_nPatchAxisIndex+1][i].xyz));
          }
          qglEnd();
        }
        else
        {
          if (g_nPatchBendState == BEND_SELECT_EDGE || g_nPatchBendState == BEND_BENDIT || g_nPatchBendState == BEND_SELECT_ORIGIN)
          {
            qglColor3f(0, 0, 1);
            qglBegin(GL_POINTS);
            for (i = 0; i < pm->height; i++)
            {
              if (g_nPatchBendState == BEND_SELECT_ORIGIN)
              {
                qglVertex3fv(reinterpret_cast<float(*)>(&pm->ctrl[g_nBendOriginIndex][i].xyz));
              }
              else
              {
                if (g_bPatchLowerEdge)
                {
                  for (j = 0; j < g_nPatchAxisIndex; j++)
			              qglVertex3fv(reinterpret_cast<float(*)>(&pm->ctrl[j][i].xyz));
                }
                else
                {
                  for (j = pm->width-1; j > g_nPatchAxisIndex; j--)
			              qglVertex3fv(reinterpret_cast<float(*)>(&pm->ctrl[j][i].xyz));
                }
              }
            }
            qglEnd();
          }
        }
      }
    }
    else // just painting the grid for selection
    {
      qglPointSize(6);
	    for ( i = 0 ; i < pm->width ; i++ )
      {
		    for ( j = 0 ; j < pm->height ; j++ ) 
        {
	        qglBegin(GL_POINTS);
          // FIXME: need to not do loop lookups inside here
          int n = PointValueInMoveList(pm->ctrl[i][j].xyz);
          if (n >= 0)
          {
            pSelectedPoints[nIndex++] = reinterpret_cast<float(*)[3]>(&pm->ctrl[i][j].xyz);
          }

          if (i & 0x01 || j & 0x01)
            qglColor3f(1, 0, 1);
          else
            qglColor3f(0, 1, 0);
			    qglVertex3fv(pm->ctrl[i][j].xyz);
	        qglEnd();
        }
      }
    }
    if (nIndex > 0)
    {
      qglBegin(GL_POINTS);
      qglColor3f(0, 0, 1);
      while (nIndex-- > 0)
      {
			  qglVertex3fv(*pSelectedPoints[nIndex]);
      }
      qglEnd();
    }
  }

  if (bOverlay)
  {
    qglPointSize(6);
    qglColor3f(0.5, 0.5, 0.5);
	  for ( i = 0 ; i < pm->width ; i++ )
    {
      qglBegin(GL_POINTS);
		  for ( j = 0 ; j < pm->height ; j++ ) 
      {
			  qglVertex3fv(pm->ctrl[i][j].xyz);
      }
      qglEnd();
    }
  }
	//--qglEnable (GL_TEXTURE_2D);
	//--qglEnable (GL_DEPTH_TEST);

	qglPopAttrib();

}

/* OLD, NOT USED
=================
DrawPatchesXY
=================
*/
void DrawPatchesXY( void ) {
	int			i;
	patchMesh_t	*pm;

	qglPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
  
	for ( i = 0 ; i < numPatchMeshes ; i++ ) 
  {
		pm = &patchMeshes[i];

    if (pm->bSelected)
      qglColor3fv(g_qeglobals.d_savedinfo.colors[COLOR_SELBRUSHES]);
    else
      qglColor3fv(g_qeglobals.d_savedinfo.colors[COLOR_BRUSHES]);

		DrawPatchMesh( pm , pm->bSelected );
	}

	qglPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
}

/*
==================
Patch_DrawXY
==================
*/
void Patch_DrawXY(int n, bool bIsGhost)
{
  patchMesh_t *pm = &patchMeshes[n];
	qglPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
  if (pm->bSelected)
  {
    qglColor3fv(g_qeglobals.d_savedinfo.colors[COLOR_SELBRUSHES]);
    //qglDisable (GL_LINE_STIPPLE);
	  //qglLineWidth (1);
  }
  else
    qglColor3fv(g_qeglobals.d_savedinfo.colors[COLOR_BRUSHES]);

  	if (bIsGhost)
	{
		qglColor4fv(v4GhostColor);
		qglLineStipple( 8, 0xAAAA);
		qglEnable(GL_LINE_STIPPLE);				
		qglLineWidth(2);
	}

	DrawPatchMesh( pm , pm->bSelected );
	qglPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

  	if (bIsGhost)
	{
		qglDisable(GL_LINE_STIPPLE);				
		qglLineWidth(1);
	}

  if (pm->bSelected)
  {
	  //qglLineWidth (2);
    //qglEnable (GL_LINE_STIPPLE);
  }
}



#if 0
/* OLD, NOT USED
=================
DrawPatchesCamera
=================
*/
void DrawPatchesCamera( void ) {
	int			i;
	patchMesh_t	*pm;

	qglColor3f (1,1,1);

	qglDisable( GL_CULL_FACE );


  if (g_bPatchWireFrame)
  {
    qglPushAttrib(GL_ALL_ATTRIB_BITS);
	  qglPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	  qglDisable(GL_TEXTURE_2D);
  }

	for ( i = 0 ; i < numPatchMeshes ; i++ ) 
  {
		pm = &patchMeshes[i];
		qglBindTexture (GL_TEXTURE_2D, pm->d_texture->texture_number);
		DrawPatchMesh( pm , i == g_nSelectedPatch, true );
	}

  if (g_bPatchWireFrame)
  {
    qglPopAttrib();
  }
	qglEnable( GL_CULL_FACE );
}
#endif
/*
==================
Patch_DrawCam
==================
*/
void Patch_DrawCam(int n, bool bIsGhost)
{
  patchMesh_t *pm = &patchMeshes[n];
	qglColor3f (1,1,1);
  qglPushAttrib(GL_ALL_ATTRIB_BITS);

  if (g_bPatchWireFrame)
  {
	  qglDisable( GL_CULL_FACE );
	  qglPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	  qglDisable(GL_TEXTURE_2D);

	  if (bIsGhost)
	  {
		//qglColor4fv(v4GhostColor);
		//qglLineStipple( 8, 0xAAAA);
		//qglEnable(GL_LINE_STIPPLE);				
		  int z=1;
	  }

	  DrawPatchMesh( pm , pm->bSelected, true );

	  if (bIsGhost)
	  {
	  }

	  qglEnable( GL_CULL_FACE );
  }
  else
  {
	  qglEnable( GL_CULL_FACE );
    qglCullFace(GL_FRONT);
    qglBindTexture (GL_TEXTURE_2D, pm->d_texture->texture_number);

    if (pm->d_texture->bFromShader && pm->d_texture->fTrans < 1.0)
    {
	    //qglEnable ( GL_BLEND );
	    //qglBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      qglColor4f(pm->d_texture->color[0], pm->d_texture->color[1], pm->d_texture->color[2], pm->d_texture->fTrans); 
    }

	if (bIsGhost)
	{
		qglEnable ( GL_BLEND );
		qglBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		qglColor4fv(v4GhostColor);
	}

    DrawPatchMesh( pm , pm->bSelected, true );
    
    qglCullFace(GL_BACK);
	  //qglDisable(GL_TEXTURE_2D);
	  qglPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

	 if (!bIsGhost)
	 {
		qglDisable ( GL_BLEND );
	 }

	  DrawPatchMesh( pm , pm->bSelected, true );

	if (bIsGhost)
	{
		qglDisable ( GL_BLEND );		
		qglColor3f(1,1,1);
	}
  }

#if 0 // this paints normal indicators on the ctrl points
      //--qglDisable (GL_DEPTH_TEST);
      qglDisable (GL_TEXTURE_2D);
      qglColor3f (1,1,1);

      for (int i = 0; i < pm->width; i++)
      {
        for (int j = 0; j < pm->height; j++)
        {
          vec3_t temp;
	        qglBegin (GL_LINES);
		      qglVertex3fv (pm->ctrl[i][j].xyz);
		      VectorMA (pm->ctrl[i][j].xyz, 8, pm->ctrl[i][j].normal, temp);
		      qglVertex3fv (temp);
	        qglEnd ();
        }
      }
      qglEnable (GL_TEXTURE_2D);
      //--qglEnable (GL_DEPTH_TEST);
#endif


  qglPopAttrib();
}




void ConvexHullForSection( float section[2][4][7] ) {
}

void BrushesForSection( float section[2][4][7] ) {
}

//
//================
//Patch_BuildPoints
//================
void Patch_BuildPoints (brush_t *b) 
{
	face_t		*f;
	b->patchBrush = false;
	for (f=b->brush_faces ; f ; f=f->next) 
  {
		if (f->texdef.flags & SURF_PATCH) 
    {
			b->patchBrush = true;
      //vec3_t vMin, vMax;
      //Patch_CalcBounds(&patchMeshes[b->nPatchID], vMin, vMax);
      //VectorCopy(vMin, b->mins);
      //VectorCopy(vMax, b->maxs);
			break;
		}
	}
}



//
//===============
//Patch_WriteFile
//===============
//
#if 0
void Patch_WriteFile (char *name) 
{
	char patchName[1024];
  time_t ltime;
	strcpy(patchName, name);
	StripExtension (patchName);
	strcat (patchName, ".patch");

  FILE *file = fopen(patchName, "w");
  if (file)
  {
	  time(&ltime);
	  fprintf (file, "// %s saved on %s\n", name, ctime(&ltime) );

    for (int i = 0; i < numPatchMeshes; i++)
    {
		  fprintf(file, "{\n");
      fprintf(file, " %s\n", patchMeshes[i].d_texture->name);
      fprintf(file, " ( %i %i %i ) \n", patchMeshes[i].width, patchMeshes[i].height, patchMeshes[i].negative);
      for (int w = 0; w < patchMeshes[i].width; w++)
      {
        for (int h = 0; h < patchMeshes[i].height; h++)
        {
          fprintf(file, " ( ");
          for (int k = 0; k < 5; k++)
          {
            float f = patchMeshes[i].ctrl[w][h][k];
            if (f == int(f))
              fprintf(file, "%i ", (int)f);
            else
              fprintf(file, "%f ", f);
          }
          fprintf(file, ")\n");
        }
      }
      fprintf(file, "}\n");
    }
    fclose(file);
  }
}
#endif
/*
==================
Patch_ReadBuffer
==================
*/
void Patch_ReadBuffer(char* pBuff, bool bSelect)
{
	StartTokenParsing (pBuff);
  while (GetToken(true))
  {
    if (strcmp(token, "{"))
      break;

    // texture def
    GetToken(true);
    patchMeshes[numPatchMeshes].d_texture = Texture_ForName(token);

    GetToken(true);
    if (strcmp(token, "("))
      break;

    // width, height, flags (currently only negative)
    GetToken(false);
    patchMeshes[numPatchMeshes].width = atoi(token);

    GetToken(false);
    patchMeshes[numPatchMeshes].height = atoi(token);
    
    GetToken(false);
    //patchMeshes[numPatchMeshes].negative = atoi(token);

    GetToken(false);
    if (strcmp(token, ")"))
      break;

    //FIXME: need error bad file handling
    for (int w = 0; w < patchMeshes[numPatchMeshes].width; w++)
    {
      for (int h = 0; h < patchMeshes[numPatchMeshes].height; h++)
      {
        GetToken(true);
        // assumptions made here as i dont want to error out
        for (int k = 0; k < 3; k++)
        {
          GetToken(false);
          patchMeshes[numPatchMeshes].ctrl[w][h].xyz[k] = atof(token);
        }
        for (k = 0; k < 2; k++)
        {
          GetToken(false);
          patchMeshes[numPatchMeshes].ctrl[w][h].st[k] = atof(token);
        }
        GetToken(false);
      }
    }
    GetToken(true);
    if (strcmp(token, "}"))
      break;

    brush_t *b = AddBrushForPatch(numPatchMeshes);
    if (bSelect)
      Select_Brush(b);

    numPatchMeshes++;
  }
}

/*
==================
Patch_ReadFile 
==================
*/
void Patch_ReadFile (const char *name)
{
  char *pBuff;
	char patchName[1024];
	strcpy(patchName, name);
	StripExtension (patchName);
	strcat (patchName, ".patch");

  if (LoadFile (patchName, (void **)&pBuff) != -1)
  {
    if (g_pParentWnd->MessageBox("Found a .patch file associated with this map. Do you want it loaded?", "Load old .patch",  MB_YESNO) == IDYES)
    {
      //FIXME: need to clean up old patches
      numPatchMeshes = 0;
      Patch_ReadBuffer(pBuff);
    }
  }
}
/*
==================
Patch_Move
==================
*/
void Patch_Move(int n, const vec3_t vMove, bool bRebuild)
{
  patchMeshes[n].bDirty = true;
  for (int w = 0; w < patchMeshes[n].width; w++)
  {
    for (int h = 0; h < patchMeshes[n].height; h++)
    {
      VectorAdd(patchMeshes[n].ctrl[w][h].xyz, vMove, patchMeshes[n].ctrl[w][h].xyz);
    }
  }
  if (bRebuild)
  {
    vec3_t vMin, vMax;
    Patch_CalcBounds(&patchMeshes[n], vMin, vMax);
    //Brush_RebuildBrush(patchMeshes[n].pSymbiot, vMin, vMax);
  }
}

/*
==================
Patch_ApplyMatrix
==================
*/
void Patch_ApplyMatrix(int n, const vec3_t vOrigin, const vec3_t vMatrix[3])
{
  vec3_t vTemp;
  patchMesh_t* p = &patchMeshes[n];

  for (int w = 0; w < p->width; w++)
  {
    for (int h = 0; h < p->height; h++)
    {
      if ( (g_qeglobals.d_select_mode == sel_curvepoint || g_bPatchBendMode)
        && PointInMoveList(p->ctrl[w][h].xyz) == -1)
        continue;
			VectorSubtract (p->ctrl[w][h].xyz, vOrigin, vTemp);
			for (int j = 0; j < 3; j++)
        p->ctrl[w][h].xyz[j] = DotProduct(vTemp, vMatrix[j]) + vOrigin[j];
    }
  }
  vec3_t vMin, vMax;
  Patch_CalcBounds(p, vMin, vMax);
  Brush_RebuildBrush(p->pSymbiot, vMin, vMax);
}

/*
==================
Patch_EditPatch
==================
*/
void Patch_EditPatch()
{
  //--patchMesh_t* p = &patchMeshes[n];
  g_qeglobals.d_numpoints = 0;
  g_qeglobals.d_num_move_points = 0;
	
  for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
	    patchMesh_t* p = &patchMeshes[pb->nPatchID];
	    for ( int i = 0 ; i < p->width ; i++ ) 
      {
		    for ( int j = 0 ; j < p->height ; j++ ) 
        {
	        VectorCopy (p->ctrl[i][j].xyz, g_qeglobals.d_points[g_qeglobals.d_numpoints]);
	        g_qeglobals.d_numpoints++;
		    }
      }
    }
  }
  g_qeglobals.d_select_mode = sel_curvepoint;
  //--g_nSelectedPatch = n;
}



/*
==================
Patch_Deselect
==================
*/
//FIXME: need all sorts of asserts throughout a lot of this crap
void Patch_Deselect()
{
  //--g_nSelectedPatch = -1;
  g_qeglobals.d_select_mode = sel_brush;
  for (int i = 0; i < numPatchMeshes; i++)
    patchMeshes[i].bSelected = false;

  if (g_bPatchBendMode)
    Patch_BendToggle();
  if (g_bPatchInsertMode)
    Patch_InsDelToggle();
}


/*
==================
Patch_Select
==================
*/
void Patch_Select(int n)
{
  // maintained for point manip.. which i need to fix as this 
  // is pf error prone
  //--g_nSelectedPatch = n;
  patchMeshes[n].bSelected = true;
}


/*
==================
Patch_Deselect
==================
*/
void Patch_Deselect(int n)
{
  patchMeshes[n].bSelected = false;
}


/*
==================
Patch_Delete
==================
*/
void Patch_Delete(int n)
{
  // bump the array down
  for (int i = n; i < numPatchMeshes; i++)
  {
    patchMeshes[i].pSymbiot->nPatchID--;
    patchMeshes[i] = patchMeshes[i+1];
  }
  numPatchMeshes--;
}


/*
==================
Patch_Scale
==================
*/
void Patch_Scale(int n, const vec3_t vOrigin, const vec3_t vAmt, bool bRebuild)
{
  patchMesh_t* p = &patchMeshes[n];

  for (int w = 0; w < p->width; w++)
  {
    for (int h = 0; h < p->height; h++)
    {
      if (g_qeglobals.d_select_mode == sel_curvepoint && PointInMoveList(p->ctrl[w][h].xyz) == -1)
        continue;
			for (int i=0 ; i<3 ; i++)
			{
        p->ctrl[w][h].xyz[i] -= vOrigin[i];
        p->ctrl[w][h].xyz[i] *= vAmt[i];
        p->ctrl[w][h].xyz[i] += vOrigin[i];
      }
    }
  }
  if (bRebuild)
  {
    vec3_t vMin, vMax;
    Patch_CalcBounds(p, vMin, vMax);
    Brush_RebuildBrush(p->pSymbiot, vMin, vMax);
  }
}


/*
==================
Patch_Cleanup
==================
*/
void Patch_Cleanup()
{
  //--g_nSelectedPatch = -1;
  numPatchMeshes = 0;
}



/*
==================
Patch_SetView
==================
*/
void Patch_SetView(int n)
{
  g_bSameView = (n == g_nPatchClickedView);
  g_nPatchClickedView = n;
}


/*
==================
Patch_SetTexture
==================
*/
// FIXME: need array validation throughout
void Patch_SetTexture(int n, texdef_t *tex_def)
{
  patchMeshes[n].d_texture = Texture_ForName(tex_def->name);
}


/*
==================
Patch_DragScale
==================
*/
bool Patch_DragScale(int n, vec3_t vAmt, vec3_t vMove)
{
  vec3_t vMin, vMax, vScale, vTemp, vMid;
  int i;

  patchMesh_t* p = &patchMeshes[n];

  Patch_CalcBounds(p, vMin, vMax);

  VectorSubtract(vMax, vMin, vTemp);

  // if we are scaling in the same dimension the patch has no depth
  for (i = 0; i < 3; i ++)
  {
    if (vTemp[i] == 0 && vMove[i] != 0)
    {
      //Patch_Move(n, vMove, true);
      return false;
    }
  }
  
  for (i=0 ; i<3 ; i++)
    vMid[i] = (vMin[i] + ((vMax[i] - vMin[i]) / 2));

  for (i = 0; i < 3; i++)
  {
    if (vAmt[i] != 0)
    {
      vScale[i] = 1.0 + vAmt[i] / vTemp[i];
    }
    else
    {
      vScale[i] = 1.0;
    }
  }

  Patch_Scale(n, vMid, vScale, false);

  VectorSubtract(vMax, vMin, vTemp);

  Patch_CalcBounds(p, vMin, vMax);
  
  VectorSubtract(vMax, vMin, vMid);

  VectorSubtract(vMid, vTemp, vTemp);

  VectorScale(vTemp, 0.5, vTemp);

  // abs of both should always be equal
  if (!VectorCompare(vMove, vAmt))
  {
    for (i = 0; i < 3; i++)
    {
      if (vMove[i] != vAmt[i])
        vTemp[i] = -(vTemp[i]);
    }
  }

  Patch_Move(n, vTemp);
  return true;
}


/*
==================
Patch_WriteFile 
==================
*/
#if 0
void Patch_WriteFile (CMemFile* pMemFile)
{
  for (int i = 0; i < numPatchMeshes; i++)
  {
    if (patchMeshes[i].bSelected)
    {
      MemFile_fprintf(pMemFile, "{\n");
      MemFile_fprintf(pMemFile, " %s\n", patchMeshes[i].d_texture->name);
      MemFile_fprintf(pMemFile, " ( %i %i %i ) \n", patchMeshes[i].width, patchMeshes[i].height, patchMeshes[i].negative);
      for (int w = 0; w < patchMeshes[i].width; w++)
      {
        for (int h = 0; h < patchMeshes[i].height; h++)
        {
          MemFile_fprintf(pMemFile, " ( ");
          for (int k = 0; k < 5; k++)
          {
            float f = patchMeshes[i].ctrl[w][h][k];
            if (f == int(f))
              MemFile_fprintf(pMemFile, "%i ", (int)f);
            else
              MemFile_fprintf(pMemFile, "%f ", f);
          }
          MemFile_fprintf(pMemFile, ")\n");
        }
      }
      MemFile_fprintf(pMemFile, "}\n");
    }
  }
}
#endif

/*
==================
Patch_AddRow
==================
*/
void Patch_AddRow(int n)
{
  vec3_t vMin, vMax, vTemp;
  int i, j;

  patchMesh_t *p = &patchMeshes[n];

  if (p->height+2 < MAX_PATCH_HEIGHT)
  {
    Patch_CalcBounds(p, vMin, vMax);
    VectorSubtract(vMax, vMin, vTemp);
    for (i = 0; i < 3; i++)
    {
      vTemp[i] /= p->height + 2;
    }

    for (j = 0; j < p->width; j++)
    {
      VectorCopy(p->ctrl[j][p->height].xyz, p->ctrl[j][p->height+1].xyz);
      VectorCopy(p->ctrl[j][p->height].xyz, p->ctrl[j][p->height+2].xyz);
      p->height += 2;
      i = 1;
      while (i < p->height)
      {
        VectorAdd(p->ctrl[j][i].xyz, vTemp, p->ctrl[j][i].xyz);
        i++;
      }
    }
    
    Patch_CalcBounds(p, vMin, vMax);
    Brush_RebuildBrush(p->pSymbiot, vMin, vMax);
  }
}

/*
==================
Patch_InsertColumn
==================
*/
void Patch_InsertColumn(int n, bool bAdd)
{
  patchMesh_t *p = &patchMeshes[n];
  int h, w, i, j;
  vec3_t vTemp;
  
  if (p->width + 2 >= MAX_PATCH_WIDTH)
    return;

  if (bAdd) // add column?
  {
    for (h = 0; h < p->height; h++)
    {
      j = p->width-1;
      VectorSubtract(p->ctrl[j][h].xyz, p->ctrl[j-1][h].xyz, vTemp);
      for (i = 0; i < 3; i++)
        vTemp[i] /= 3;

      memcpy(&p->ctrl[j+2][h],&p->ctrl[j][h], sizeof(drawVert_t));
      memcpy(&p->ctrl[j][h],&p->ctrl[j-1][h], sizeof(drawVert_t));

      VectorAdd(p->ctrl[j][h].xyz, vTemp, p->ctrl[j][h].xyz);
      memcpy(&p->ctrl[j+1][h], &p->ctrl[j][h], sizeof(drawVert_t));
      VectorAdd(p->ctrl[j+1][h].xyz, vTemp, p->ctrl[j+1][h].xyz);
    }
  }
  else
  {
    for (h = 0; h < p->height; h++)
    {
      w = p->width-1;
      while (w >= 0)
      {
        memcpy(&p->ctrl[w+2][h],&p->ctrl[w][h], sizeof(drawVert_t));
        w--;
      }
      VectorSubtract(p->ctrl[1][h].xyz, p->ctrl[0][h].xyz, vTemp);
      for (i = 0; i < 3; i++)
        vTemp[i] /= 3;
      VectorCopy(p->ctrl[0][h].xyz, p->ctrl[1][h].xyz);
      VectorAdd(p->ctrl[1][h].xyz, vTemp, p->ctrl[1][h].xyz);
      VectorCopy(p->ctrl[1][h].xyz, p->ctrl[2][h].xyz);
      VectorAdd(p->ctrl[2][h].xyz, vTemp, p->ctrl[2][h].xyz);
    }
  }
  p->width += 2;
}


/*
==================
Patch_InsertRow
==================
*/
void Patch_InsertRow(int n, bool bAdd)
{
  patchMesh_t *p = &patchMeshes[n];
  int h, w, i, j;
  vec3_t vTemp;
  
  if (p->height + 2 >= MAX_PATCH_HEIGHT)
    return;

  if (bAdd) // add column?
  {
    for (w = 0; w < p->width; w++)
    {
      j = p->height-1;
      VectorSubtract(p->ctrl[w][j].xyz, p->ctrl[w][j-1].xyz, vTemp);
      for (i = 0; i < 3; i++)
        vTemp[i] /= 3;

      memcpy(&p->ctrl[w][j+2],&p->ctrl[w][j], sizeof(drawVert_t));
      memcpy(&p->ctrl[w][j],&p->ctrl[w][j-1], sizeof(drawVert_t));

      VectorAdd(p->ctrl[w][j].xyz, vTemp, p->ctrl[w][j].xyz);
      memcpy(&p->ctrl[w][j+1], &p->ctrl[w][j], sizeof(drawVert_t));
      VectorAdd(p->ctrl[w][j+1].xyz, vTemp, p->ctrl[w][j+1].xyz);
    }
  }
  else
  {
    for (w = 0; w < p->width; w++)
    {
      h = p->height-1;
      while (h >= 0)
      {
        memcpy(&p->ctrl[w][h+2],&p->ctrl[w][h], sizeof(drawVert_t));
        h--;
      }
      VectorSubtract(p->ctrl[w][1].xyz, p->ctrl[w][0].xyz, vTemp);
      for (i = 0; i < 3; i++)
        vTemp[i] /= 3;
      VectorCopy(p->ctrl[w][0].xyz, p->ctrl[w][1].xyz);
      VectorAdd(p->ctrl[w][1].xyz, vTemp, p->ctrl[w][1].xyz);
      VectorCopy(p->ctrl[w][1].xyz, p->ctrl[w][2].xyz);
      VectorAdd(p->ctrl[w][2].xyz, vTemp, p->ctrl[w][2].xyz);
    }
  }
  p->height += 2;
}


/*
==================
Patch_RemoveRow
==================
*/
void Patch_RemoveRow(int n, bool bFirst)
{
  patchMesh_t *p = &patchMeshes[n];
  
  if (p->height <= MIN_PATCH_HEIGHT)
    return;

  p->height -= 2;

  if (bFirst)
  {
    for (int w = 0; w < p->width; w++)
    {
      for (int h = 0; h < p->height; h++)
      {
        memcpy(&p->ctrl[w][h], &p->ctrl[w][h+2], sizeof(drawVert_t));
      }
    }
  }
}


/*
==================
Patch_RemoveColumn
==================
*/
void Patch_RemoveColumn(int n, bool bFirst)
{
  patchMesh_t *p = &patchMeshes[n];
  
  if (p->width <= MIN_PATCH_WIDTH)
    return;

  p->width -= 2;

  if (bFirst)
  {
    for (int h = 0; h < p->height; h++)
    {
      for (int w = 0; w < p->width; w++)
      {
        memcpy(&p->ctrl[w][h], &p->ctrl[w+2][h], sizeof(drawVert_t));
      }
    }
  }
}


/*
==================
Patch_AdjustColumns
==================
*/
void Patch_AdjustColumns(int n, int nCols)
{
  vec3_t vTemp, vTemp2;
  int i, w, h;
  patchMesh_t *p = &patchMeshes[n];

  if (nCols & 0x01 || p->width + nCols < 3 || p->width + nCols > MAX_PATCH_WIDTH)
    return;

  // add in column adjustment
  p->width += nCols;

  for (h = 0; h < p->height; h++)
  {
    // for each column, we need to evenly disperse p->width number 
    // of points across the old bounds
    
    // calc total distance to interpolate 
    VectorSubtract(p->ctrl[p->width - 1 - nCols][h].xyz, p->ctrl[0][h].xyz, vTemp);

    // amount per cycle
    for (i = 0; i < 3; i ++)
    {
      vTemp2[i] = vTemp[i] / (p->width - 1);
    }

    // move along
    for (w = 0; w < p->width-1; w++)
    {
      VectorAdd(p->ctrl[w][h].xyz, vTemp2, p->ctrl[w+1][h].xyz);
    }

  }
	for ( w = 0 ; w < p->width ; w++ ) 
  {
		for ( h = 0 ; h < p->height ; h++ ) 
    {
			p->ctrl[w][h].st[0] = 4 * (float)w / (p->width - 1);
			p->ctrl[w][h].st[1] = 4 * (float)h / (p->height - 1);
		}
	}
}


/*
==================
Patch_AdjustRows
==================
*/
void Patch_AdjustRows(int n, int nRows)
{
  vec3_t vTemp, vTemp2;
  int i, w, h;
  patchMesh_t *p = &patchMeshes[n];

  if (nRows & 0x01 || p->height + nRows < 3 || p->height + nRows > MAX_PATCH_HEIGHT)
    return;

  // add in column adjustment
  p->height += nRows;

  for (w = 0; w < p->width; w++)
  {
    // for each row, we need to evenly disperse p->height number 
    // of points across the old bounds

    // calc total distance to interpolate 
    VectorSubtract(p->ctrl[w][p->height - 1 - nRows].xyz, p->ctrl[w][0].xyz, vTemp);
    
    //vTemp[0] = vTemp[1] = vTemp[2] = 0;
    //for (h = 0; h < p->height - nRows; h ++)
    //{
    //  VectorAdd(vTemp, p->ctrl[w][h], vTemp);
    //}

    // amount per cycle
    for (i = 0; i < 3; i ++)
    {
      vTemp2[i] = vTemp[i] / (p->height - 1);
    }

    // move along
    for (h = 0; h < p->height-1; h++)
    {
      VectorAdd(p->ctrl[w][h].xyz, vTemp2, p->ctrl[w][h+1].xyz);
    }

  }
	for ( w = 0 ; w < p->width ; w++ ) 
  {
		for ( h = 0 ; h < p->height ; h++ ) 
    {
			p->ctrl[w][h].st[0] = 4 * (float)w / (p->width - 1);
			p->ctrl[w][h].st[1] = 4 * (float)h / (p->height - 1);
		}
	}
}


void Patch_DisperseRows()
{
  vec3_t vTemp, vTemp2;
  int i, w, h;


	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      patchMesh_t *p = &patchMeshes[pb->nPatchID];
      Patch_Rebuild(p);
      for (w = 0; w < p->width; w++)
      {
        // for each row, we need to evenly disperse p->height number 
        // of points across the old bounds

        // calc total distance to interpolate 
        VectorSubtract(p->ctrl[w][p->height - 1].xyz, p->ctrl[w][0].xyz, vTemp);
    
        //vTemp[0] = vTemp[1] = vTemp[2] = 0;
        //for (h = 0; h < p->height - nRows; h ++)
        //{
        //  VectorAdd(vTemp, p->ctrl[w][h], vTemp);
        //}

        // amount per cycle
        for (i = 0; i < 3; i ++)
        {
          vTemp2[i] = vTemp[i] / (p->height - 1);
        }

        // move along
        for (h = 0; h < p->height-1; h++)
        {
          VectorAdd(p->ctrl[w][h].xyz, vTemp2, p->ctrl[w][h+1].xyz);
        }
        Patch_Naturalize(p);

      }
    }
  }

}

/*
==================
Patch_AdjustColumns
==================
*/
void Patch_DisperseColumns()
{
  vec3_t vTemp, vTemp2;
  int i, w, h;

	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      patchMesh_t *p = &patchMeshes[pb->nPatchID];
      Patch_Rebuild(p);

      for (h = 0; h < p->height; h++)
      {
        // for each column, we need to evenly disperse p->width number 
        // of points across the old bounds
    
        // calc total distance to interpolate 
        VectorSubtract(p->ctrl[p->width - 1][h].xyz, p->ctrl[0][h].xyz, vTemp);

        // amount per cycle
        for (i = 0; i < 3; i ++)
        {
          vTemp2[i] = vTemp[i] / (p->width - 1);
        }

        // move along
        for (w = 0; w < p->width-1; w++)
        {
          VectorAdd(p->ctrl[w][h].xyz, vTemp2, p->ctrl[w+1][h].xyz);
        }

      }
      Patch_Naturalize(p);
    }
  }
}



/*
==================
Patch_AdjustSelected
==================
*/
void Patch_AdjustSelected(bool bInsert, bool bColumn, bool bFlag)
{
  bool bUpdate = false;
	for (brush_t* pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      if (bInsert)
      {
        if (bColumn)
        {
          Patch_InsertColumn(pb->nPatchID, bFlag);
        }
        else
        {
          Patch_InsertRow(pb->nPatchID, bFlag);
        }
      }
      else
      {
        if (bColumn)
        {
          Patch_RemoveColumn(pb->nPatchID, bFlag);
        }
        else
        {
          Patch_RemoveRow(pb->nPatchID, bFlag);
        }
      }
      bUpdate = true;
      vec3_t vMin, vMax;
      patchMesh_t *p = &patchMeshes[pb->nPatchID];
      Patch_CalcBounds(p, vMin, vMax);
      Brush_RebuildBrush(p->pSymbiot, vMin, vMax);
    }
  }
  if (bUpdate)
  {
    Sys_UpdateWindows(W_ALL);
  }
}


/*
==================
Patch_AdjustSelectedRowCols
==================
*/
void Patch_AdjustSelectedRowCols(int nRows, int nCols)
{
	for (brush_t* pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      Patch_InsertColumn(pb->nPatchID, false);
      if (nRows != 0)
      {
        Patch_AdjustRows(pb->nPatchID, nRows);
      }
      
      if (nCols != 0)
      {
        Patch_AdjustColumns(pb->nPatchID, nCols);
      }
		}
  }
}


// parses a patch
brush_t* Patch_ParseOld()
{
    GetToken(true);
    if (strcmp(token, "{"))
      return NULL;

    // texture def
    GetToken(true);
    patchMeshes[numPatchMeshes].d_texture = Texture_ForName(token);

    GetToken(true);
    if (strcmp(token, "("))
      return NULL;

    // width, height, flags (currently only negative)
    GetToken(false);
    patchMeshes[numPatchMeshes].width = atoi(token);

    GetToken(false);
    patchMeshes[numPatchMeshes].height = atoi(token);
    
    GetToken(false);
    //patchMeshes[numPatchMeshes].negative = atoi(token);

    GetToken(false);
    if (strcmp(token, ")"))
      return NULL;

    //FIXME: need error bad file handling
    for (int w = 0; w < patchMeshes[numPatchMeshes].width; w++)
    {
      for (int h = 0; h < patchMeshes[numPatchMeshes].height; h++)
      {
        GetToken(true);
        // assumptions made here as i dont want to error out
        for (int k = 0; k < 3; k++)
        {
          GetToken(false);
          patchMeshes[numPatchMeshes].ctrl[w][h].xyz[k] = atof(token);
        }
        for (k = 0; k < 2; k++)
        {
          GetToken(false);
          patchMeshes[numPatchMeshes].ctrl[w][h].st[k] = atof(token);
        }
        GetToken(false);
      }
    }
  
    GetToken(true);
    if (strcmp(token, "}"))
      return NULL;


    brush_t *b = AddBrushForPatch(numPatchMeshes, false);
    numPatchMeshes++;

  return b;
}

void Parse1DMatrix(int x, float* p)
{
  GetToken(true); // (
  for (int i = 0; i < x; i++)
  {
    GetToken(false);
    p[i] = atof(token);
  }
  GetToken(true); // )
}

void Parse2DMatrix(int y, int x, float* p)
{
  GetToken(true); // (
  for (int i = 0; i < y; i++)
  {
    Parse1DMatrix(x, p + i*x);
  }
  GetToken(true); // )
}

void Parse3DMatrix(int z, int y, int x, float* p)
{
  GetToken(true); // (
  for (int i = 0; i < z; i++)
  {
    Parse2DMatrix(y, x, p + i*(x*MAX_PATCH_HEIGHT));
  }
  GetToken(true); // )
}

// parses a patch
brush_t* Patch_Parse(bool bOld)
{
    //--if (bOld)
    //--{
    //--  return Patch_ParseOld();
    //--}

		if (numPatchMeshes == MAX_PATCH_MESHES)
		{	//ran out of patches, doh!
			Error ("Patch_Parse: numPatchMeshes reached!");
		}
    GetToken(true);
    
    if (strcmp(token, "{"))
      return NULL;

    // texture def
    GetToken(true);

    // band-aid 
    if (strcmp(token, "("))
    {
      patchMeshes[numPatchMeshes].d_texture = Texture_ForName(token);
      GetToken(true);
    }
    else
    {
      patchMeshes[numPatchMeshes].d_texture = notexture;
      Sys_Printf("Warning: Patch read with no texture, using notexture... \n");
    }               

    if (strcmp(token, "("))
      return NULL;

    // width, height, contents, flags, value, {type}
    GetToken(false);
    patchMeshes[numPatchMeshes].width = atoi(token);

    GetToken(false);
    patchMeshes[numPatchMeshes].height = atoi(token);
    
    GetToken(false);
    patchMeshes[numPatchMeshes].contents = atoi(token);

    GetToken(false);
    patchMeshes[numPatchMeshes].flags = atoi(token);

    GetToken(false);
    patchMeshes[numPatchMeshes].value = atoi(token);

    if (!bOld)
    {
      GetToken(false);
      patchMeshes[numPatchMeshes].type = atoi(token);
    }

    GetToken(false);
    if (strcmp(token, ")"))
      return NULL;

    float ctrl[MAX_PATCH_WIDTH][MAX_PATCH_HEIGHT][5];
    Parse3DMatrix(patchMeshes[numPatchMeshes].width, patchMeshes[numPatchMeshes].height, 5, reinterpret_cast<float*>(&ctrl));

    int w, h;
    patchMesh_t *p = &patchMeshes[numPatchMeshes];

    for (w = 0; w < p->width; w++)
    {
      for (h = 0; h < p->height; h++)
      {
        p->ctrl[w][h].xyz[0] = ctrl[w][h][0];  
        p->ctrl[w][h].xyz[1] = ctrl[w][h][1];  
        p->ctrl[w][h].xyz[2] = ctrl[w][h][2];  
        p->ctrl[w][h].st[0] = ctrl[w][h][3]; 
        p->ctrl[w][h].st[1] = ctrl[w][h][4];  
      }
    }

    GetToken(true);
    if (strcmp(token, "}"))
      return NULL;

    brush_t *b = AddBrushForPatch(numPatchMeshes, false);
    numPatchMeshes++;

  return b;
}


/*
==================
Patch_Write 
==================
*/
void Patch_Write (int n, CMemFile *file)
{
  patchMesh_t *p = &patchMeshes[n];
  //--MemFile_fprintf(file, " {\n  patchDef3\n  {\n");
  MemFile_fprintf(file, " {\n  patchDef2\n  {\n");
  MemFile_fprintf(file, "   %s\n",p->d_texture->name);
  //--MemFile_fprintf(file, "   ( %i %i %i %i %i %i ) \n", p->width, p->height, p->contents, p->flags, p->value, p->type);
  MemFile_fprintf(file, "   ( %i %i %i %i %i ) \n", p->width, p->height, p->contents, p->flags, p->value);

	float		ctrl[MAX_PATCH_WIDTH][MAX_PATCH_HEIGHT][5];

  int w, h;
  for (w = 0; w < p->width; w++)
  {
    for (h = 0; h < p->height; h++)
    {
      ctrl[w][h][0] = p->ctrl[w][h].xyz[0];
      ctrl[w][h][1] = p->ctrl[w][h].xyz[1];
      ctrl[w][h][2] = p->ctrl[w][h].xyz[2];
      ctrl[w][h][3] = p->ctrl[w][h].st[0];
      ctrl[w][h][4] = p->ctrl[w][h].st[1];
    }
  }

  _Write3DMatrix(file, p->width, p->height, 5, reinterpret_cast<float*>(&ctrl));
#if 0
  for (int w = 0; w < p->width; w++)
  {
    for (int h = 0; h < p->height; h++)
    {
      MemFile_fprintf(file, "   ( ");
      for (int k = 0; k < 5; k++)
      {
        float f = p->ctrl[w][h][k];
        if (f == int(f))
        {
          MemFile_fprintf(file, "%i ", (int)f);
        }
        else
        {
          MemFile_fprintf(file, "%f ", f);
        }
      }
      MemFile_fprintf(file, ")\n");
    }
  }
#endif
  MemFile_fprintf(file, "  }\n }\n");
}

void Patch_Write (int n, FILE *file)
{
  patchMesh_t *p = &patchMeshes[n];
  //--fprintf(file, " {\n  patchDef3\n  {\n");
  fprintf(file, " {\n  patchDef2\n  {\n");
  fprintf(file, "   %s\n",p->d_texture->name);
  //--fprintf(file, "   ( %i %i %i %i %i %i ) \n", p->width, p->height, p->contents, p->flags, p->value, p->type);
  fprintf(file, "   ( %i %i %i %i %i ) \n", p->width, p->height, p->contents, p->flags, p->value);

	float		ctrl[MAX_PATCH_WIDTH][MAX_PATCH_HEIGHT][5];

  int w, h;
  for (w = 0; w < p->width; w++)
  {
    for (h = 0; h < p->height; h++)
    {
      ctrl[w][h][0] = p->ctrl[w][h].xyz[0];
      ctrl[w][h][1] = p->ctrl[w][h].xyz[1];
      ctrl[w][h][2] = p->ctrl[w][h].xyz[2];
      ctrl[w][h][3] = p->ctrl[w][h].st[0];
      ctrl[w][h][4] = p->ctrl[w][h].st[1];
    }
  }

  _Write3DMatrix(file, p->width, p->height, 5, reinterpret_cast<float*>(&ctrl));
#if 0
  for (int w = 0; w < p->width; w++)
  {
    for (int h = 0; h < p->height; h++)
    {
      fprintf(file, "   ( ");
      for (int k = 0; k < 5; k++)
      {
        float f = p->ctrl[w][h][k];
        if (f == int(f))
        {
          fprintf(file, "%i ", (int)f);
        }
        else
        {
          fprintf(file, "%f ", f);
        }
      }
      fprintf(file, ")\n");
    }
  }
#endif
  fprintf(file, "  }\n }\n");
}


/*
==================
Patch_RotateTexture
==================
*/
void Patch_RotateTexture(int n, float fAngle)
{
  patchMesh_t *p = &patchMeshes[n];
  vec3_t vMin, vMax;
  Patch_CalcBounds(p, vMin, vMax);
  p->bDirty = true;
  for (int w = 0; w < p->width; w++)
  {
    for (int h = 0; h < p->height; h++)
    {
      if (g_qeglobals.d_select_mode == sel_curvepoint && PointInMoveList(p->ctrl[w][h].xyz) == -1)
        continue;

      float x = p->ctrl[w][h].st[0];
      float y = p->ctrl[w][h].st[1];
      p->ctrl[w][h].st[0] = x * cos(fAngle * Q_PI / 180) - y * sin(fAngle * Q_PI / 180);
      p->ctrl[w][h].st[1] = y * cos(fAngle * Q_PI / 180) + x * sin(fAngle * Q_PI / 180);
    }
  }
}


/*
==================
Patch_ScaleTexture
==================
*/
void Patch_ScaleTexture(int n, float fx, float fy, bool bFixup)
{
  // FIXME:
  // this hack turns scales into 1.1 or 0.9
  if (bFixup)
  {
    fx = (fx == 0) ? 1.0 : (fx > 0) ? 0.9 : 1.10;
    fy = (fy == 0) ? 1.0 : (fy > 0) ? 0.9 : 1.10;
  }
  else
  {
    if (fx == 0)
      fx = 1.0;
    if (fy == 0)
      fy = 1.0;
  }
  
  patchMesh_t *p = &patchMeshes[n];
  for (int w = 0; w < p->width; w++)
  {
    for (int h = 0; h < p->height; h++)
    {
      if (g_qeglobals.d_select_mode == sel_curvepoint && PointInMoveList(p->ctrl[w][h].xyz) == -1)
        continue;

      p->ctrl[w][h].st[0] *= fx;
      p->ctrl[w][h].st[1] *= fy;
    }
  }
  p->bDirty = true;
}


/*
==================
Patch_ShiftTexture
==================
*/
void Patch_ShiftTexture(int n, float fx, float fy)
{
  //if (fx)
  //  fx = (fx > 0) ? 0.1 : -0.1;
  //if (fy)
  //  fy = (fy > 0) ? 0.1 : -0.1;

  fx = (abs(fx) >= 1) ? fx / 10 : fx;
  fy = (abs(fy) >= 1) ? fy / 10 : fy;

  patchMesh_t *p = &patchMeshes[n];
  for (int w = 0; w < p->width; w++)
  {
    for (int h = 0; h < p->height; h++)
    {
      if (g_qeglobals.d_select_mode == sel_curvepoint && PointInMoveList(p->ctrl[w][h].xyz) == -1)
        continue;

      p->ctrl[w][h].st[0] += fx;
      p->ctrl[w][h].st[1] += fy;
    }
  }
  p->bDirty = true;
}

void patchInvert(patchMesh_t *p)
{
  drawVert_t vertTemp;
  p->bDirty = true;
	for ( int i = 0 ; i < p->width ; i++ ) 
  {
    for (int j = 0; j < p->height / 2; j++)
    {
      memcpy(&vertTemp, &p->ctrl[i][p->height - 1- j], sizeof (drawVert_t));
      memcpy(&p->ctrl[i][p->height - 1 - j], &p->ctrl[i][j], sizeof(drawVert_t));
      memcpy(&p->ctrl[i][j], &vertTemp, sizeof(drawVert_t));
		}
	}
}

/*
==================
Patch_ToggleInverted
==================
*/
void Patch_ToggleInverted()
{
  bool bUpdate = false;

	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      bUpdate = true;
      patchInvert(&patchMeshes[pb->nPatchID]);
    }
  }

  if (bUpdate)
  {
    Sys_UpdateWindows(W_ALL);
  }
}

/*
==================
Patch_ToggleInverted
==================
*/
void Patch_InvertTexture(bool bY)
{
  bool bUpdate = false;

  float fTemp[2];
	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      bUpdate = true;
      patchMesh_t *p = &patchMeshes[pb->nPatchID];
      p->bDirty = true;
      if (bY)
      {
	      for ( int i = 0 ; i < p->height ; i++ ) 
        {
          for (int j = 0; j < p->width / 2; j++)
          {
            memcpy(fTemp, &p->ctrl[p->width - 1- j][i].st[0], sizeof (float[2]));
            memcpy(&p->ctrl[p->width - 1- j][i].st[0], &p->ctrl[j][i].st[0], sizeof(float[2]));
            memcpy(&p->ctrl[j][i].st[0], fTemp, sizeof(float[2]));
		      }
	      }
      }
      else
      {
	      for ( int i = 0 ; i < p->width ; i++ ) 
        {
          for (int j = 0; j < p->height / 2; j++)
          {
            memcpy(fTemp, &p->ctrl[i][p->height - 1- j].st[0], sizeof (float[2]));
            memcpy(&p->ctrl[i][p->height - 1 - j].st[0], &p->ctrl[i][j].st[0], sizeof(float[2]));
            memcpy(&p->ctrl[i][j].st[0], fTemp, sizeof(float[2]));
		      }
	      }
      }
    }
  }

  if (bUpdate)
  {
    Sys_UpdateWindows(W_ALL);
  }
}




/*
==================
Patch_Save
==================
 Saves patch ctrl info (originally to deal with a 
 cancel in the surface dialog
*/
void Patch_Save(int n)
{
  patchMesh_t *p = &patchMeshes[n];
  patchSave.width = p->width;
  patchSave.height = p->height;
  memcpy(patchSave.ctrl, p->ctrl, sizeof(p->ctrl));
}


/*
==================
Patch_Restore
==================
*/
void Patch_Restore(int n)
{
  patchMesh_t *p = &patchMeshes[n];
  p->width = patchSave.width;
  p->height = patchSave.height;
  memcpy(p->ctrl, patchSave.ctrl, sizeof(p->ctrl));
}

void Patch_ResetTexturing(float fx, float fy)
{
	for (brush_t* pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      patchMesh_t *p = &patchMeshes[pb->nPatchID];
      p->bDirty = true;
	    for ( int i = 0 ; i < p->width ; i++ ) 
      {
		    for ( int j = 0 ; j < p->height ; j++ ) 
        {
			    p->ctrl[i][j].st[0] = fx * (float)i / (p->width - 1);
			    p->ctrl[i][j].st[1] = fy * (float)j / (p->height - 1);
		    }
	    }
    }
  }
}


void Patch_FitTexturing()
{
	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      patchMesh_t *p = &patchMeshes[pb->nPatchID];
      p->bDirty = true;
	    for ( int i = 0 ; i < p->width ; i++ ) 
      {
		    for ( int j = 0 ; j < p->height ; j++ ) 
        {
			    p->ctrl[i][j].st[0] = 1 * (float)i / (p->width - 1);
			    p->ctrl[i][j].st[1] = 1 * (float)j / (p->height - 1);
		    }
	    }
    }
  }
}


void Patch_SetTextureInfo(texdef_t *pt)
{
	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      if (pt->rotate)
        Patch_RotateTexture(pb->nPatchID, pt->rotate);

      if (pt->shift[0] || pt->shift[1])
        Patch_ShiftTexture(pb->nPatchID, pt->shift[0], pt->shift[1]);

      if (pt->scale[0] || pt->scale[1])
        Patch_ScaleTexture(pb->nPatchID, pt->scale[0], pt->scale[1], false);

      patchMesh_t *p = &patchMeshes[pb->nPatchID];
      p->contents = pt->contents;
      p->flags = pt->flags;
      p->value = pt->value;
    }
  }
}

bool OnlyPatchesSelected()
{
  if (selected_face || selected_brushes.next == &selected_brushes)
    return false;
	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (!pb->patchBrush)
    {
      return false;
    }
  }
  return true;
}

bool AnyPatchesSelected()
{
  if (selected_face || selected_brushes.next == &selected_brushes)
    return false;
	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      return true;
    }
  }
  return false;
}

void Patch_BendToggle()
{
  if (g_bPatchBendMode)
  {
    g_bPatchBendMode = false;
    HideInfoDialog();
    g_pParentWnd->UpdatePatchToolbarButtons() ;
    return;
  }

	brush_t* b = selected_brushes.next;

  if (!QE_SingleBrush() || !b->patchBrush)
  {
    Sys_Printf("Must bend a single patch");
		return;
  }

  Patch_Save(b->nPatchID);
	g_bPatchBendMode = true;
  g_nPatchBendState = BEND_SELECT_ROTATION;
  g_bPatchAxisOnRow = true;
  g_nPatchAxisIndex = 1;
  ShowInfoDialog(g_pBendStateMsg[BEND_SELECT_ROTATION]);
}

void Patch_BendHandleTAB()
{
  if (!g_bPatchBendMode)
  {
    return;
  }

	brush_t* b = selected_brushes.next;
  if (!QE_SingleBrush() || !b->patchBrush)
  {
    Patch_BendToggle();
    Sys_Printf("No patch to bend!");
		return;
  }

  patchMesh_t *p = &patchMeshes[b->nPatchID];

  bool bShift = (GetKeyState(VK_SHIFT) & 0x8000);

  if (g_nPatchBendState == BEND_SELECT_ROTATION)
  {
    // only able to deal with odd numbered rows/cols
    g_nPatchAxisIndex += (bShift) ? -2 : 2;
    if (g_bPatchAxisOnRow)
    {
      if ((bShift) ? g_nPatchAxisIndex <= 0 : g_nPatchAxisIndex >= p->height)
      {
        g_bPatchAxisOnRow = false;
        g_nPatchAxisIndex = (bShift) ? p->width-1 : 1;
      }
    }
    else
    {
      if ((bShift) ? g_nPatchAxisIndex <= 0 : g_nPatchAxisIndex >= p->width)
      {
        g_bPatchAxisOnRow = true;
        g_nPatchAxisIndex = (bShift) ? p->height-1 : 1;
      }
    }
  }
  else
  if (g_nPatchBendState == BEND_SELECT_ORIGIN)
  {
    g_nBendOriginIndex += (bShift) ? -1 : 1;
    if (g_bPatchAxisOnRow)
    {
      if (bShift)
      {
        if (g_nBendOriginIndex < 0)
          g_nBendOriginIndex = p->width-1;
      }
      else
      {
        if (g_nBendOriginIndex > p->width-1)
          g_nBendOriginIndex = 0;
      }
      VectorCopy(p->ctrl[g_nBendOriginIndex][g_nPatchAxisIndex].xyz, g_vBendOrigin);
    }
    else
    {
      if (bShift)
      {
        if (g_nBendOriginIndex < 0)
          g_nBendOriginIndex = p->height-1;
      }
      else
      {
        if (g_nBendOriginIndex > p->height-1)
          g_nBendOriginIndex = 0;
      }
      VectorCopy(p->ctrl[g_nPatchAxisIndex][g_nBendOriginIndex].xyz, g_vBendOrigin);
    }
  }
  else
  if (g_nPatchBendState == BEND_SELECT_EDGE)
  {
    g_bPatchLowerEdge ^= 1;
  }
  Sys_UpdateWindows(W_ALL);
}

void Patch_BendHandleENTER()
{
  if (!g_bPatchBendMode)
  {
    return;
  }

  if (g_nPatchBendState  < BEND_BENDIT)
  {
    g_nPatchBendState++;
    ShowInfoDialog(g_pBendStateMsg[g_nPatchBendState]);
    if (g_nPatchBendState == BEND_SELECT_ORIGIN)
    {
      g_vBendOrigin[0] = g_vBendOrigin[1] = g_vBendOrigin[2] = 0;
      g_nBendOriginIndex = 0;
      Patch_BendHandleTAB();
    }
    else
    if (g_nPatchBendState == BEND_SELECT_EDGE)
    {
      g_bPatchLowerEdge = true;
    }
    else
    if (g_nPatchBendState == BEND_BENDIT)
    {
      // basically we go into rotation mode, set the axis to the center of the 
    }
  }
  else
  {
    // done
    Patch_BendToggle();
  }
  Sys_UpdateWindows(W_ALL);

}


void Patch_BendHandleESC()
{
  if (!g_bPatchBendMode)
  {
    return;
  }
  Patch_BendToggle();
	brush_t* b = selected_brushes.next;
  if (QE_SingleBrush() && b->patchBrush)
  {
    Patch_Restore(b->nPatchID);
  }
  Sys_UpdateWindows(W_ALL);
}

void Patch_SetBendRotateOrigin(patchMesh_t *p)
{
#if 1
  int nType = g_pParentWnd->ActiveXY()->GetViewType();
  int nDim3 = (nType == XY) ? 2 : (nType == YZ) ? 0 : 1;

  g_vBendOrigin[nDim3] = 0;
  VectorCopy(g_vBendOrigin, g_pParentWnd->ActiveXY()->RotateOrigin());
  return;
#else
  int nDim1 = (g_pParentWnd->ActiveXY()->GetViewType() == YZ) ? 1 : 0;
  int nDim2 = (g_pParentWnd->ActiveXY()->GetViewType() == XY) ? 1 : 2;

  float fxLo, fyLo, fxHi, fyHi;
  fxLo = fyLo = MAX_WORLD_COORD;
  fxHi = fyHi = MIN_WORLD_COORD;

  if (g_bPatchAxisOnRow)
  {
    for (int i = 0; i < p->width; i++)
    {
      if (p->ctrl[i][g_nPatchAxisIndex].xyz[nDim1] < fxLo)
        fxLo = p->ctrl[i][g_nPatchAxisIndex].xyz[nDim1];

      if (p->ctrl[i][g_nPatchAxisIndex].xyz[nDim1] > fxHi)
        fxHi = p->ctrl[i][g_nPatchAxisIndex].xyz[nDim1];

      if (p->ctrl[i][g_nPatchAxisIndex].xyz[nDim2] < fyLo)
        fyLo = p->ctrl[i][g_nPatchAxisIndex].xyz[nDim2];

      if (p->ctrl[i][g_nPatchAxisIndex].xyz[nDim2] > fyHi)
        fyHi = p->ctrl[i][g_nPatchAxisIndex].xyz[nDim2];
    }
  }
  else
  {
    for (int i = 0; i < p->height; i++)
    {
      if (p->ctrl[g_nPatchAxisIndex][i].xyz[nDim1] < fxLo)
        fxLo = p->ctrl[g_nPatchAxisIndex][i].xyz[nDim1];

      if (p->ctrl[g_nPatchAxisIndex][i].xyz[nDim1] > fxHi)
        fxHi = p->ctrl[g_nPatchAxisIndex][i].xyz[nDim1];

      if (p->ctrl[g_nPatchAxisIndex][i].xyz[nDim2] < fyLo)
        fyLo = p->ctrl[g_nPatchAxisIndex][i].xyz[nDim2];

      if (p->ctrl[g_nPatchAxisIndex][i].xyz[nDim2] > fyHi)
        fyHi = p->ctrl[g_nPatchAxisIndex][i].xyz[nDim2];
    }
  }

  g_pParentWnd->ActiveXY()->RotateOrigin()[0] = g_pParentWnd->ActiveXY()->RotateOrigin()[1] = g_pParentWnd->ActiveXY()->RotateOrigin()[2] = 0.0;
  g_pParentWnd->ActiveXY()->RotateOrigin()[nDim1] = (fxLo + fxHi) * 0.5;
  g_pParentWnd->ActiveXY()->RotateOrigin()[nDim2] = (fyLo + fyHi) * 0.5;
#endif
}

// also sets the rotational origin
void Patch_SelectBendAxis()
{
	brush_t* b = selected_brushes.next;
  if (!QE_SingleBrush() || !b->patchBrush)
  {
    // should not ever happen
    Patch_BendToggle();
		return;
  }

  patchMesh_t *p = &patchMeshes[b->nPatchID];
  if (g_bPatchAxisOnRow)
  {
    SelectRow(p, g_nPatchAxisIndex, false);
  }
  else
  {
    SelectColumn(p, g_nPatchAxisIndex, false);
  }

  //FIXME: this only needs to be set once... 
  Patch_SetBendRotateOrigin(p);

}

void Patch_SelectBendNormal()
{
	brush_t* b = selected_brushes.next;
  if (!QE_SingleBrush() || !b->patchBrush)
  {
    // should not ever happen
    Patch_BendToggle();
		return;
  }

  patchMesh_t *p = &patchMeshes[b->nPatchID];

  g_qeglobals.d_num_move_points = 0;
  if (g_bPatchAxisOnRow)
  {
    if (g_bPatchLowerEdge)
    {
      for (int j = 0; j < g_nPatchAxisIndex; j++)
        SelectRow(p, j, true);
    }
    else
    {
      for (int j = p->height-1; j > g_nPatchAxisIndex; j--)
        SelectRow(p, j, true);
    }
  }
  else
  {
    if (g_bPatchLowerEdge)
    {
      for (int j = 0; j < g_nPatchAxisIndex; j++)
        SelectColumn(p, j, true);
    }
    else
    {
      for (int j = p->width-1; j > g_nPatchAxisIndex; j--)
        SelectColumn(p, j, true);
    }
  }
  Patch_SetBendRotateOrigin(p);
}



void Patch_InsDelToggle()
{
  if (g_bPatchInsertMode)
  {
    g_bPatchInsertMode = false;
    HideInfoDialog();
    g_pParentWnd->UpdatePatchToolbarButtons() ;
    return;
  }

	brush_t* b = selected_brushes.next;

  if (!QE_SingleBrush() || !b->patchBrush)
  {
    Sys_Printf("Must work with a single patch");
		return;
  }

  Patch_Save(b->nPatchID);
	g_bPatchInsertMode = true;
  g_nPatchInsertState = INSERT_SELECT_EDGE;
  g_bPatchAxisOnRow = true;
  g_nPatchAxisIndex = 0;
  ShowInfoDialog(g_pInsertStateMsg[INSERT_SELECT_EDGE]);

}

void Patch_InsDelESC()
{
  if (!g_bPatchInsertMode)
  {
    return;
  }
  Patch_InsDelToggle();
  Sys_UpdateWindows(W_ALL);
}


void Patch_InsDelHandleENTER()
{
}

void Patch_InsDelHandleTAB()
{
  if (!g_bPatchInsertMode)
  {
    Patch_InsDelToggle();
    return;
  }

	brush_t* b = selected_brushes.next;
  if (!QE_SingleBrush() || !b->patchBrush)
  {
    Patch_BendToggle();
    Sys_Printf("No patch to bend!");
		return;
  }

  patchMesh_t *p = &patchMeshes[b->nPatchID];

  // only able to deal with odd numbered rows/cols
  g_nPatchAxisIndex += 2;
  if (g_bPatchAxisOnRow)
  {
    if (g_nPatchAxisIndex >= p->height-1)
    {
      g_bPatchAxisOnRow = false;
      g_nPatchAxisIndex = 0;
    }
  }
  else
  {
    if (g_nPatchAxisIndex >= p->width-1)
    {
      g_bPatchAxisOnRow = true;
      g_nPatchAxisIndex = 0;
    }
  }
  Sys_UpdateWindows(W_ALL);
}


void _Write1DMatrix (FILE *f, int x, float *m) {
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

void _Write2DMatrix (FILE *f, int y, int x, float *m) {
	int		i;

	fprintf (f, "( ");
	for (i = 0 ; i < y ; i++) {
		_Write1DMatrix (f, x, m + i*x);
		fprintf (f, " ");
	}
	fprintf (f, ")\n");
}


void _Write3DMatrix (FILE *f, int z, int y, int x, float *m) {
	int		i;

	fprintf (f, "(\n");
	for (i = 0 ; i < z ; i++) {
		_Write2DMatrix (f, y, x, m + i*(x*MAX_PATCH_HEIGHT) );
	}
	fprintf (f, ")\n");
}

void _Write1DMatrix (CMemFile *f, int x, float *m) {
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

void _Write2DMatrix (CMemFile *f, int y, int x, float *m) {
	int		i;

	MemFile_fprintf (f, "( ");
	for (i = 0 ; i < y ; i++) {
		_Write1DMatrix (f, x, m + i*x);
		MemFile_fprintf (f, " ");
	}
	MemFile_fprintf (f, ")\n");
}


void _Write3DMatrix (CMemFile *f, int z, int y, int x, float *m) {
	int		i;

	MemFile_fprintf (f, "(\n");
	for (i = 0 ; i < z ; i++) {
		_Write2DMatrix (f, y, x, m + i*(x*MAX_PATCH_HEIGHT) );
	}
	MemFile_fprintf (f, ")\n");
}


void Patch_NaturalizeSelected(bool bCap, bool bCycleCap)
{
	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      if (bCap)
        Patch_CapTexture(&patchMeshes[pb->nPatchID], bCycleCap);
      else
        Patch_Naturalize(&patchMeshes[pb->nPatchID]);
    }
  }
}

bool within(vec3_t vTest, vec3_t vTL, vec3_t vBR)
{
  int nDim1 = (g_pParentWnd->ActiveXY()->GetViewType() == YZ) ? 1 : 0;
  int nDim2 = (g_pParentWnd->ActiveXY()->GetViewType() == XY) ? 1 : 2;
  if ((vTest[nDim1] > vTL[nDim1] && vTest[nDim1] < vBR[nDim1]) ||
      (vTest[nDim1] < vTL[nDim1] && vTest[nDim1] > vBR[nDim1]))
  {
    if ((vTest[nDim2] > vTL[nDim2] && vTest[nDim2] < vBR[nDim2]) ||
        (vTest[nDim2] < vTL[nDim2] && vTest[nDim2] > vBR[nDim2]))
      return true;
  }
  return false;
}


void Patch_SelectAreaPoints()
{
  g_qeglobals.d_num_move_points = 0;
  g_nPatchClickedView = -1;

	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      patchMesh_t *p = &patchMeshes[pb->nPatchID];
      for (int i = 0; i < p->width; i++)
      {
        for (int j = 0; j < p->height; j++)
        {
          if (within(p->ctrl[i][j].xyz, g_qeglobals.d_vAreaTL, g_qeglobals.d_vAreaBR))
          {
            g_qeglobals.d_move_points[g_qeglobals.d_num_move_points++] = p->ctrl[i][j].xyz;
          }
        }
      }
    }
  }
}

const char* Patch_GetTextureName()
{
	brush_t* b = selected_brushes.next;
  if (b->patchBrush)
  {
    patchMesh_t *p = &patchMeshes[b->nPatchID];
    if (p->d_texture->name)
      return p->d_texture->name;
  }
  return "";
}

patchMesh_t* Patch_Duplicate(patchMesh_t *pFrom)
{
  patchMesh_t* p = &patchMeshes[numPatchMeshes];
  memcpy(p, pFrom , sizeof(patchMesh_t));
  p->bSelected = false;
  p->bDirty = true;
  p->bOverlay = false;
  p->nListID = -1;
  AddBrushForPatch(numPatchMeshes);
  numPatchMeshes++;
  return p;
}


void Patch_Thicken(int nAmount, bool bSeam)
{
  int i, j, h, w;
  brush_t *b;
  patchMesh_t *pSeam;
  vec3_t vMin, vMax;
  brush_t* brushes[5];
  brushes[0] = brushes[1] = brushes[2] = brushes[3] = brushes[4] = NULL;
  int nIndex = 0;

  nAmount = -nAmount;

	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      patchMesh_t *p = &patchMeshes[pb->nPatchID];
      Patch_MeshNormals(*p);
      patchMesh_t *pNew = Patch_Duplicate(p);
      for (i = 0; i < p->width; i++)
      {
        for (j = 0; j < p->height; j++)
        {
		      VectorMA (p->ctrl[i][j].xyz, nAmount, p->ctrl[i][j].normal, pNew->ctrl[i][j].xyz);
        }
      }

      Patch_Rebuild(pNew);
      pNew->type |= PATCH_THICK;
      brushes[nIndex++] = pNew->pSymbiot;

      if (bSeam)
      {

        // FIXME: this should detect if any edges of the patch are closed and act appropriately
        // 
        if (!(p->type & PATCH_CYLINDER))
        {
          b = Patch_GenericMesh(3, p->height, 2, false);
          pSeam = &patchMeshes[b->nPatchID];
          pSeam->type |= PATCH_SEAM;
          for (i = 0; i < p->height; i++)
          {
            VectorCopy(p->ctrl[0][i].xyz, pSeam->ctrl[0][i].xyz);
            VectorCopy(pNew->ctrl[0][i].xyz, pSeam->ctrl[2][i].xyz);
            VectorAdd(pSeam->ctrl[0][i].xyz, pSeam->ctrl[2][i].xyz, pSeam->ctrl[1][i].xyz);
            VectorScale(pSeam->ctrl[1][i].xyz, 0.5, pSeam->ctrl[1][i].xyz);
          }


          Patch_CalcBounds(pSeam, vMin, vMax);
          Brush_RebuildBrush(pSeam->pSymbiot, vMin, vMax);
          //--Patch_CapTexture(pSeam);
          Patch_Naturalize(pSeam);
          patchInvert(pSeam);
          brushes[nIndex++] = b;

          w = p->width - 1;
          b = Patch_GenericMesh(3, p->height, 2, false);
          pSeam = &patchMeshes[b->nPatchID];
          pSeam->type |= PATCH_SEAM;
          for (i = 0; i < p->height; i++)
          {
            VectorCopy(p->ctrl[w][i].xyz, pSeam->ctrl[0][i].xyz);
            VectorCopy(pNew->ctrl[w][i].xyz, pSeam->ctrl[2][i].xyz);
            VectorAdd(pSeam->ctrl[0][i].xyz, pSeam->ctrl[2][i].xyz, pSeam->ctrl[1][i].xyz);
            VectorScale(pSeam->ctrl[1][i].xyz, 0.5, pSeam->ctrl[1][i].xyz);
          }
          Patch_CalcBounds(pSeam, vMin, vMax);
          Brush_RebuildBrush(pSeam->pSymbiot, vMin, vMax);
          //--Patch_CapTexture(pSeam);
          Patch_Naturalize(pSeam);
          brushes[nIndex++] = b;
        }
    
        //--{
          // otherwise we will add one per end
          b = Patch_GenericMesh(p->width, 3, 2, false);
          pSeam = &patchMeshes[b->nPatchID];
          pSeam->type |= PATCH_SEAM;
          for (i = 0; i < p->width; i++)
          {
            VectorCopy(p->ctrl[i][0].xyz, pSeam->ctrl[i][0].xyz);
            VectorCopy(pNew->ctrl[i][0].xyz, pSeam->ctrl[i][2].xyz);
            VectorAdd(pSeam->ctrl[i][0].xyz, pSeam->ctrl[i][2].xyz, pSeam->ctrl[i][1].xyz);
            VectorScale(pSeam->ctrl[i][1].xyz, 0.5, pSeam->ctrl[i][1].xyz);
          }


          Patch_CalcBounds(pSeam, vMin, vMax);
          Brush_RebuildBrush(pSeam->pSymbiot, vMin, vMax);
          //--Patch_CapTexture(pSeam);
          Patch_Naturalize(pSeam);
          patchInvert(pSeam);
          brushes[nIndex++] = b;

          h = p->height - 1;
          b = Patch_GenericMesh(p->width, 3, 2, false);
          pSeam = &patchMeshes[b->nPatchID];
          pSeam->type |= PATCH_SEAM;
          for (i = 0; i < p->width; i++)
          {
            VectorCopy(p->ctrl[i][h].xyz, pSeam->ctrl[i][0].xyz);
            VectorCopy(pNew->ctrl[i][h].xyz, pSeam->ctrl[i][2].xyz);
            VectorAdd(pSeam->ctrl[i][0].xyz, pSeam->ctrl[i][2].xyz, pSeam->ctrl[i][1].xyz);
            VectorScale(pSeam->ctrl[i][1].xyz, 0.5, pSeam->ctrl[i][1].xyz);
          }
          Patch_CalcBounds(pSeam, vMin, vMax);
          Brush_RebuildBrush(pSeam->pSymbiot, vMin, vMax);
          //--Patch_CapTexture(pSeam);
          Patch_Naturalize(pSeam);
          brushes[nIndex++] = b;

          while (nIndex > 0)
          {
            nIndex--;
            if (brushes[nIndex])
            {
              Select_Brush(brushes[nIndex]);
            }
          }
          eclass_t *pecNew = Eclass_ForName("func_group", false);
          if (pecNew)
          {
            entity_t *e = Entity_Create(pecNew);
            SetKeyValue(e, "type", "patchThick");
          }

        
        //--}
      }
      patchInvert(pNew);
    }
  }
}


/*
lets get another list together as far as necessities..

*snapping stuff to the grid (i will only snap movements by the mouse to the grid.. snapping the rotational bend stuff will fubar everything)

capping bevels/endcaps

hot keys

texture fix for caps

clear clipboard

*region fix

*surface dialog

*/

void Patch_SetOverlays()
{
	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      patchMeshes[pb->nPatchID].bOverlay = true;
    }
  }
}



void Patch_ClearOverlays()
{
	for ( int i = 0 ; i < numPatchMeshes ; i++ ) 
  {
		patchMeshes[i].bOverlay = false;
  }
}

void Patch_Transpose()
{
	int		i, j, l, w;
  drawVert_t dv;
	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      patchMesh_t *p = &patchMeshes[pb->nPatchID];

      patchMesh_t ptemp;
      memcpy(&ptemp, &p, sizeof(patchMesh_t));

	    l = p->width > p->height ? p->width : p->height;
	    for ( i = 0 ; i < l ; i++ ) 
      {
		    for ( j = i+1 ; j < l ; j++ ) 
        {
          memcpy(&dv, &p->ctrl[i][j], sizeof(drawVert_t));
          memcpy(&p->ctrl[i][j], &p->ctrl[j][i], sizeof(drawVert_t));
          memcpy(&p->ctrl[j][i], &dv, sizeof(drawVert_t));
        }
      }
      w = p->width;
      p->width = p->height;
      p->height = w;
      patchInvert(p);
      Patch_Rebuild(p);
		}
	}
}



void Select_SnapToGrid()
{
	int i,j, k;
	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    if (pb->patchBrush)
    {
      patchMesh_t *p = &patchMeshes[pb->nPatchID];
#if 0
	    float		ctrl[MAX_PATCH_WIDTH][MAX_PATCH_HEIGHT][5];
      memcpy(ctrl, p->ctrl, sizeof(p->ctrl));
      i = p->width;
      p->width = p->height;
      p->height = i;
      for (i = 0; i < p->width; i++)
      {
        int l = p->height-1;
        for (j = 0; j < p->height; j++)
        {
          for (k = 0; k < 5; k++)
          {
            p->ctrl[i][l][k] = ctrl[j][i][k];
          }
          l--;
        }
      }
#else
      for (i = 0; i < p->width; i++)
      {
        for (j = 0; j < p->height; j++)
        {
          for (k = 0; k < 3; k++)
          {
            p->ctrl[i][j].xyz[k] = floor(p->ctrl[i][j].xyz[k] / g_qeglobals.d_gridsize + 0.5) * g_qeglobals.d_gridsize;
          }
        }
      }
#endif
      vec3_t vMin, vMax;
      Patch_CalcBounds(p, vMin, vMax);
      Brush_RebuildBrush(p->pSymbiot, vMin, vMax);
    }
    else
    {
      Brush_SnapToGrid(pb);
    }
  }
}

// there's already a function called Patch_GetTextureName, but that assumes you want it from the selected list, 
//	rather than using one you supply, which is daft. Hence this hacky name...
//
const char *Patch_FromBrush_GetTextureName(brush_t* pb)
{
  if (pb->patchBrush)
  {
    patchMesh_t *p = &patchMeshes[pb->nPatchID];
    if (p->d_texture->name)
      return p->d_texture->name;
  }
  return "";
}

// return is just whether a replace occured or not (for friendly stat-print later)...
//
bool Patch_FindReplaceTexture(brush_t *pb, const char *pFind, const char *pReplace, bool bForce)
{
  if (pb->patchBrush)
  {
    patchMesh_t *p = &patchMeshes[pb->nPatchID];
    if (bForce || strcmpi(p->d_texture->name, pFind) == 0)
    {
      p->d_texture = Texture_ForName(pReplace);
      //strcpy(p->d_texture->name, pReplace);
	  return true;
    }
  }
  return false;
}

void Patch_ReplaceQTexture(brush_t *pb, qtexture_t *pOld, qtexture_t *pNew)
{
  if (pb->patchBrush)
  {
    patchMesh_t *p = &patchMeshes[pb->nPatchID];
    if (p->d_texture == pOld)
    {
      p->d_texture = pNew;
    }
  }
}

void Patch_MakeNonSolid(brush_t* pb)
{
/*	
	if (pb->patchBrush)
	{
		patchMesh_t *p = &patchMeshes[pb->nPatchID];

		brush_t* pb2 = p->pSymbiot;

		for (face_t*f=pb->brush_faces ; f ; f=f->next)
			f->texdef.flags |= SURF_NONSOLID;
	}
*/
}

void Patch_MakeSolid(brush_t* pb)
{
/*
	if (pb->patchBrush)
	{
		patchMesh_t *p = &patchMeshes[pb->nPatchID];

		brush_t* pb2 = p->pSymbiot;

		for (face_t*f=pb->brush_faces ; f ; f=f->next)
			f->texdef.flags &= ~SURF_NONSOLID;
	}  
*/
}

//Real nitpicky, but could you make CTRL-S save the current map with the current name? (ie: File/Save)
/*
Feature addition.
When reading in textures, please check for the presence of a file called "textures.link" or something, which contains one line such as;

g:\quake3\baseq3\textures\common

 So that, when I'm reading in, lets say, my \eerie directory, it goes through and adds my textures to the palette, along with everything in common.

  Don't forget to add "Finer texture alignment" to the list. I'd like to be able to move in 0.1 increments using the Shift-Arrow Keys.

  No. Sometimes textures are drawn the wrong way on patches. We'd like the ability to flip a texture. Like the way X/Y scale -1 used to worked.

  1) Easier way of deleting rows, columns
2) Fine tuning of textures on patches (X/Y shifts other than with the surface dialog)
2) Patch matrix transposition

  1) Actually, bump texture flipping on patches to the top of the list of things to do.
2) When you select a patch, and hit S, it should read in the selected patch texture. Should not work if you multiselect patches and hit S
3) Brandon has a wierd anomoly. He fine-tunes a patch with caps. It looks fine when the patch is selected, but as soon as he escapes out, it reverts to it's pre-tuned state. When he selects the patch again, it looks tuned


*1) Flipping textures on patches
*2) When you select a patch, and hit S, it should read in the selected patch texture. Should not work if you multiselect patches and hit S
3) Easier way of deleting rows columns
*4) Thick Curves
5) Patch matrix transposition
6) Inverted cylinder capping
*7) bugs
*8) curve speed

  Have a new feature request. "Compute Bounding Box" for mapobjects (md3 files). This would be used for misc_mapobject (essentially, drop in 3DS Max models into our maps)

  Ok, Feature Request. Load and draw MD3's in the Camera view with proper bounding boxes. This should be off misc_model

  Feature Addition: View/Hide Hint Brushes -- This should be a specific case.
*/