// qertypes.h
//
// common types
// merged from brush.h, etc. for plugin support
//
#ifndef _QERTYPE_H
#define _QERTYPE_H

#ifndef __BYTEBOOL__
#define __BYTEBOOL__
typedef boolean qboolean;
//typedef unsigned char byte;
#endif

#define	MAXPOINTS	16

typedef float vec_t;
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];

typedef struct texdef_s
{
	char	name[128];
	float	shift[2];
	float	rotate;
	float	scale[2];
	int		contents;
	int		flags;
	int		value;
	float	lighting[4];
} texdef_t;

typedef struct
{
	int			width, height;
	int			originy;
	texdef_t	texdef;
  int m_nTotalHeight;
} texturewin_t;

#define QER_TRANS     0x00000001
#define QER_NOCARVE   0x00000002

typedef struct qtexture_s
{
	struct	qtexture_s *next;
	char	name[64];		// includes partial directory and extension

	FILETIME ft;			// these 3 are for smart-replace filetime code
	char	sFTName[MAX_PATH];	// NOT MAX_QPATH. testing first char for NZ = validity (qmalloc does memset() 0)

  int		width,  height;
	int		contents;
	int		flags;
	int		value;
	int		texture_number;	// gl bind number
  
  char  shadername[1024]; // old shader stuff
  qboolean bFromShader;   // created from a shader
  float fTrans;           // amount of transparency
  int   nShaderFlags;     // qer_ shader flags
	vec3_t	color;			    // for flat shade mode
	qboolean	inuse;		    // true = is present on the level
	
	bool bForArchitecture;	// false until any world stuff requests this texture (used for filtering model textures out of available shader list)
	bool bPlaceHolder;		// not a real texture yet, still needs loading for real

} qtexture_t;


typedef struct
{
	int		numpoints;
	int		maxpoints;
	float 	points[8][5];			// variable sized
} winding_t;

typedef struct
{
    vec3_t	normal;
    double	dist;
    int		type;
} plane_t;

typedef struct face_s
{
	struct face_s	*next;
	vec3_t		planepts[3];
  texdef_t	texdef;
  plane_t		plane;

	winding_t  *face_winding;

	vec3_t		d_color;
	qtexture_t *d_texture;

} face_t;

typedef struct {
	vec3_t	xyz;
	float	sideST[2];
	float	capST[2];
} curveVertex_t;

typedef struct {
	curveVertex_t	v[2];
} sideVertex_t;

typedef struct brush_s
{
	struct brush_s	*prev, *next;	// links in active/selected
	struct brush_s	*oprev, *onext;	// links in entity
	struct entity_s	*owner;
	vec3_t	mins, maxs;
	face_t     *brush_faces;

	qboolean bModelFailed;
	//
	// curve brush extensions
	// all are derived from brush_faces
	qboolean	curveBrush;
	qboolean	patchBrush;
	qboolean	hiddenBrush;

    int nPatchID;
} brush_t;


#ifdef SOF
	#define	MAX_FLAGS		21
	#define COLUMN_SIZE		7
	#define	NOT_FLAGS_START	8
	#define NOT_FLAGS_SIZE	5
#else
	#define	MAX_FLAGS		12	//8   8, plus 3 fixed, plus one extra instead of !deathmatch
	#define COLUMN_SIZE		4
	#define	NOT_FLAGS_START	8	//0
	#define NOT_FLAGS_SIZE	3	//0
#endif


typedef struct trimodel_t
{
  vec3_t v[3];
  float  st[3][2];
} trimodel;

typedef struct entitymodel_t
{
  entitymodel_t *pNext;
  int nTriCount;
  trimodel *pTriList;
  int nTextureBind;
  int nSkinWidth;
  int nSkinHeight;
  int	nModelPosition;
} entitymodel;


typedef struct eclass_s
{
	struct eclass_s *next;
	char	*name;
	qboolean	fixedsize;
	qboolean	unknown;		// wasn't found in source
	vec3_t	mins, maxs;
	vec3_t	color;
	texdef_t	texdef;
	char	*comments;
	char	flagnames[MAX_FLAGS][32];

/*
  int nTriCount;
  trimodel *pTriList;
  int nTextureBind;
  int nSkinWidth, nSkinHeight;
*/
  entitymodel *model;
  char	*modelpath;
  char	*skinpath;
  int   nFrame;	   

  // new fields that I auto-stuff in via new QUAKED-type directives -ste
  //
  char	*psQuakEd_MODELNAME;

} eclass_t;

extern	eclass_t	*eclass;

#endif