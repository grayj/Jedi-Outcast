
typedef enum
{
	sel_brush,
	// sel_sticky_brush,
	// sel_face,
	sel_vertex,
	sel_edge,
  sel_singlevertex,
  sel_curvepoint,
  sel_area
} select_t;

typedef struct
{
	brush_t		*brush;
	face_t		*face;
	float		dist;
	qboolean	selected;
} trace_t;


#define	SF_SELECTED_ONLY	 0x01
#define	SF_ENTITIES_FIRST	 0x02
#define	SF_SINGLEFACE		   0x04
#define SF_IGNORECURVES    0x08
#define SF_IGNOREGROUPS    0x10
#define SF_CYCLE           0x20


trace_t Test_Ray (vec3_t origin, vec3_t dir, int flags);

void Select_GetBounds (vec3_t mins, vec3_t maxs);
void Select_Brush (brush_t *b, bool bComplete = true);
void Select_Ray (vec3_t origin, vec3_t dir, int flags);
void Select_Delete (void);
void Select_Deselect (bool bDeSelectToListBack = false);
void Select_Clone (void);
void Select_Move (vec3_t delta, bool bSnap = true);
void Select_SetTexture (texdef_t *texdef, bool bFitScale = false, bool bNoSystemTextureOverwrite = false);
void Select_FitTexture(int nHeight = 1, int nWidth = 1);
void Select_FlipAxis (int axis);
void Select_RotateAxis (int axis, float deg, bool bPaint = true, bool bMouse = false);
void Select_CompleteTall (void);
void Select_PartialTall (void);
void Select_Touching (void);
void Select_Inside (void);
void Select_MakeStructural (void);
void Select_MakeDetail (bool bDeselectAfterwards = true);
void Select_MakeNonSolid(void);
void Select_ClearNonSolid(void);
void Select_HideWaypointChildren(void);
void Select_UnHideWaypointChildren(void);
void Select_UnHideAllWaypoints(void);
void Select_Hide();
void Select_ShowAllHidden();
brush_t *GetTheSelectedBrush(bool bQuietError = false);
void ComputeAbsolute(face_t* f, vec3_t& p1, vec3_t& p2, vec3_t& p3);
void AbsoluteToLocal(plane_t normal2, face_t* f, vec3_t& p1, vec3_t& p2, vec3_t& p3);

#ifdef QUAKE3
int ScriptPopup_GetFieldsCount();
LPCSTR ScriptPopup_GetField(int iIndex);
LPCSTR ScriptPopup_GetScriptName(int iIndex);
bool ScriptPopup_Allowed();
#endif

#define sKEY_HIDDENWAYPOINT "hiddenwaypoint"
#define sKEY_WAYPOINTHIDE_RECURSION_PROTECT "recursed"
