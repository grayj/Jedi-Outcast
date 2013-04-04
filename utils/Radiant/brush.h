
// brush.h

// all types moved to qertypes.h
//--typedef struct
//--{
//--	int		numpoints;
//--	int		maxpoints;
//--	float 	points[8][5];			// variable sized
//--} winding_t;


// the normals on planes point OUT of the brush
//--#define	MAXPOINTS	16
//--typedef struct face_s
//--{
//--	struct face_s	*next;
//--	vec3_t		planepts[3];
//--  texdef_t	texdef;
//--  plane_t		plane;
//--
//--	winding_t  *face_winding;
//--
//--	vec3_t		d_color;
//--	qtexture_t *d_texture;
//--
//--} face_t;
//--
//--typedef struct {
//--	vec3_t	xyz;
//--	float	sideST[2];
//--	float	capST[2];
//--} curveVertex_t;
//--
//--typedef struct {
//--	curveVertex_t	v[2];
//--} sideVertex_t;
//--
//--typedef struct brush_s
//--{
//--	struct brush_s	*prev, *next;	// links in active/selected
//--	struct brush_s	*oprev, *onext;	// links in entity
//--	struct entity_s	*owner;
//--	vec3_t	mins, maxs;
//--	face_t     *brush_faces;
//--
//--	qboolean bModelFailed;
//--	//
//--	// curve brush extensions
//--	// all are derived from brush_faces
//--	qboolean	curveBrush;
//--	qboolean	patchBrush;
//--    int nPatchID;
//--} brush_t;

void	 Brush_AddToList (brush_t *b, brush_t *list, bool bAddToBackOfList = false );
void     Brush_Build(brush_t *b, bool bSnap = true, bool bMarkMap = true);
void     Brush_BuildWindings( brush_t *b, bool bSnap = true );
brush_t *Brush_Clone (brush_t *b);
brush_t	*Brush_Create (vec3_t mins, vec3_t maxs, texdef_t *texdef);
void	 Brush_Draw( brush_t *b, bool bIsGhost );
void	 Brush_DrawXY(brush_t *b, int nViewType, bool bIsGhost = false);
void     Brush_Free (brush_t *b);
void     Brush_MakeSided (int sides);
void     Brush_MakeSidedCone (int sides);
void     Brush_Move (brush_t *b, const vec3_t move, bool bSnap = true);
brush_t *Brush_Parse (void);
face_t  *Brush_Ray (vec3_t origin, vec3_t dir, brush_t *b, float *dist);
void     Brush_RemoveFromList (brush_t *b);
void     Brush_SelectFaceForDragging (brush_t *b, face_t *f, qboolean shear);
void     Brush_SetTexture (brush_t *b, texdef_t *texdef, bool bFitScale = false, bool bNoSystemTextureOverwrite = false);
void     Brush_SideSelect (brush_t *b, vec3_t origin, vec3_t dir, qboolean shear);
void     Brush_Write (brush_t *b, FILE *f);
void	Brush_RemoveEmptyFaces ( brush_t *b );
void Brush_MakeFacePlane (face_t *f);
void    Face_Free( face_t *f );

int        AddPlanept (float *f);
face_t	  *Face_Clone (face_t *f);
void       Face_Draw( face_t *face );
winding_t *MakeFaceWinding (brush_t *b, face_t *face);
void FaceTextureVectors (face_t *f, float STfromXYZ[2][4]);
void SetFaceTexdef (brush_t *b, face_t *f, texdef_t *texdef, bool bFitScale/*=false*/, bool bNoSystemTextureOverwrite/*=false*/ );
void Brush_Write (brush_t *b, CMemFile* pMemFile);
void Brush_SnapToGrid(brush_t *pb);
face_t *Face_Alloc( void );
float SetShadeForPlane (plane_t *p);
void ClearBounds (vec3_t mins, vec3_t maxs);
void AddPointToBounds (const vec3_t v, vec3_t mins, vec3_t maxs);
void Brush_Rotate(brush_t *b, vec3_t vAngle, vec3_t vOrigin, bool bBuild = true);
void Brush_Scale2(eclass_t	*e,brush_t *b, float scale, vec3_t vAngle, bool bBuild = true);
void Brush_FitTexture( brush_t *b, int nHeight, int nWidth );
void Face_FitTexture( face_t * face, int nHeight, int nWidth );

extern qboolean qbShowFaces;

