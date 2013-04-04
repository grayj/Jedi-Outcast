#ifndef __QE3_H__
#define __QE3_H__

// disable data conversion warnings for gl
#pragma warning(disable : 4244)     // MIPS
#pragma warning(disable : 4136)     // X86
#pragma warning(disable : 4051)     // ALPHA
 
#include <windows.h>

extern "C"
{
#include "qgl.h"
}

#include <math.h>
#include <stdlib.h>

#include "qertypes.h"
#include "cmdlib.h"
#include "mathlib.h"
#include "parse.h"
#include "lbmlib.h"

#include <commctrl.h>
#include "afxres.h"
#include "resource.h"

#include "qedefs.h"

#include "qfiles.h"

#include "textures.h"
#include "brush.h"
#include "entity.h"
#include "map.h"
#include "select.h"

#include "camera.h"
#include "xy.h"
#include "z.h"
#include "mru.h"

typedef struct
{
	int		p1, p2;
	face_t	*f1, *f2;
} pedge_t;

typedef struct
{
	int		  iSize;
	int		  iTexMenu;		// nearest, linear, etc
	float	  fGamma;			// gamma for textures
	char	  szProject[256];	// last project loaded
	vec3_t	colors[COLOR_LAST];
	qboolean  show_names;
  qboolean  show_coordinates;
	int       exclude;
  int     m_nTextureTweak;
} SavedInfo_t;

//
// system functions
//
void    Sys_UpdateStatusBar( void );
void    Sys_UpdateWindows (int bits);
void    Sys_Beep (void);
void    Sys_ClearPrintf (void);
void    Sys_Printf (char *text, ...);
double	Sys_DoubleTime (void);
void    Sys_GetCursorPos (int *x, int *y);
void    Sys_SetCursorPos (int x, int y);
void    Sys_SetTitle (char *text);
void    Sys_BeginWait (void);
void    Sys_EndWait (void);
void    Sys_Status(const char *psz, int part);

/*
** most of the QE globals are stored in this structure
*/
#define D_MAXPOINTS 4096
typedef struct
{
	qboolean d_showgrid;
	int      d_gridsize;

	int      d_num_entities;

	entity_t *d_project_entity;

	float     d_new_brush_bottom_z,
		      d_new_brush_top_z;

	HINSTANCE d_hInstance;

	HGLRC     d_hglrcBase;
	HDC       d_hdcBase;

	HWND      d_hwndMain;
	HWND      d_hwndCamera;
	HWND      d_hwndEdit;
	HWND      d_hwndEntity;
	HWND      d_hwndTexture;
	HWND      d_hwndXY;
	HWND      d_hwndZ;
	HWND      d_hwndStatus;

	vec3_t    d_points[MAX_POINTS];
	int       d_numpoints;
	pedge_t   d_edges[MAX_EDGES];
	int       d_numedges;

	int       d_num_move_points;
	float    *d_move_points[D_MAXPOINTS];

	qtexture_t	*d_qtextures;

	texturewin_t d_texturewin;

	int	         d_pointfile_display_list;

	xy_t         d_xyOld;

	LPMRUMENU    d_lpMruMenu;

	SavedInfo_t  d_savedinfo;

	int          d_workcount;

	// connect entities uses the last two brushes selected
	int			 d_select_count;
	brush_t		*d_select_order[2];
	vec3_t       d_select_translate;    // for dragging w/o making new display lists
	select_t     d_select_mode;

	int		     d_font_list;

	int          d_parsed_brushes;

	qboolean	show_blocks;

  vec3_t  d_vAreaTL;
  vec3_t  d_vAreaBR;

} QEGlobals_t;

void *qmalloc (size_t size);
char *copystring (char *s);
char *ExpandReletivePath (char *p);

char *Pointfile_Delete(void);
void Pointfile_Check (void);
void Pointfile_Next (void);
void Pointfile_Prev (void);
void Pointfile_Clear (void);
void Pointfile_Draw( void );
void Pointfile_Load( void );

//
// drag.c
//
void Drag_Begin (int x, int y, int buttons,
		   vec3_t xaxis, vec3_t yaxis,
		   vec3_t origin, vec3_t dir);
void Drag_MouseMoved (int x, int y, int buttons);
void Drag_MouseUp (int nButtons = 0);

//
// csg.c
//
void CSG_MakeHollow (void);
void CSG_Subtract (void);

//
// vertsel.c
//

void SetupVertexSelection (void);
void SelectEdgeByRay (vec3_t org, vec3_t dir);
void SelectVertexByRay (vec3_t org, vec3_t dir);

void ConnectEntities (bool bIncrementalTargetNames = false);

extern	int	update_bits;

extern	int	screen_width;
extern	int	screen_height;

extern	HANDLE	bsp_process;

char	*TranslateString (char *buf);

void ProjectDialog (void);

void FillTextureMenu (CStringArray* pArray = NULL);
void FillBSPMenu (void);

BOOL CALLBACK Win_Dialog (
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,	// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam 	// second message parameter
);


//
// win_cam.c
//
void WCam_Create (HINSTANCE hInstance);


//
// win_xy.c
//
void WXY_Create (HINSTANCE hInstance);

//
// win_z.c
//
void WZ_Create (HINSTANCE hInstance);

//
// win_ent.c
//


//
// win_main.c
//
void Main_Create (HINSTANCE hInstance);
extern BOOL SaveWindowState(HWND hWnd, const char *pszName);
extern BOOL LoadWindowState(HWND hWnd, const char *pszName);

extern BOOL SaveRegistryInfo(const char *pszName, void *pvBuf, long lSize);
extern BOOL LoadRegistryInfo(const char *pszName, void *pvBuf, long *plSize);

//
// entityw.c
//
BOOL CreateEntityWindow(HINSTANCE hInstance);
void FillClassList (void);
BOOL UpdateEntitySel(eclass_t *pec);	
void SetInspectorMode(int iType);
int DrawTexControls(HWND hWnd);
void SetSpawnFlags(void);
void GetSpawnFlags(void);
void SetKeyValuePairs(bool bClearMD3 = false);
extern void BuildGammaTable(float g);


// win_dlg.c

void DoGamma(void);
void DoFind(void);
void DoRotate(void);
void DoSides(bool bCone = false);
void DoAbout(void);
void DoSurface();

/*
** QE function declarations
*/
void     QE_CheckAutoSave( void );
void     QE_ConvertDOSToUnixName( char *dst, const char *src );
void     QE_CountBrushesAndUpdateStatusBar( void );
void     QE_CheckOpenGLForErrors(void);
void     QE_ExpandBspString (char *bspaction, char *out, char *mapname);
void     QE_Init (void);
qboolean QE_KeyDown (int key, int nFlags = 0);
qboolean QE_LoadProject (const char *projectfile);
qboolean QE_SingleBrush (bool bQuiet = false);


// sys stuff
void Sys_MarkMapModified (void);

/*
** QE Win32 function declarations
*/
int  QEW_SetupPixelFormat(HDC hDC, qboolean zbuffer );
void QEW_StopGL( HWND hWnd, HGLRC hGLRC, HDC hDC );

/*
** extern declarations
*/
extern QEGlobals_t   g_qeglobals;

enum VIEWTYPE {YZ, XZ, XY};
qboolean IsBrushSelected(brush_t* bSel);

// curve brushes

void Curve_MakeCurvedBrush (qboolean negative, qboolean top, qboolean bottom, 
					qboolean s1, qboolean s2, qboolean s3, qboolean s4);

void Curve_Invert (void);

void Curve_AddFakePlanes( brush_t *B );
void Curve_StripFakePlanes( brush_t *B );
void Curve_BuildPoints (brush_t *b);
void Curve_XYDraw (brush_t *b);
void Curve_CameraDraw (brush_t *b);

void Curve_WriteFile (char *name);


// patch stuff
brush_t* Patch_GenericMesh(int nWidth, int nHeight, int nOrientation = 2, bool bDeleteSource = true);
void Patch_ReadFile (const char *name);
void Patch_WriteFile (const char *name); 
void Patch_BuildPoints (brush_t *b);
void Patch_Move(int n, const vec3_t vMove, bool bRebuild = false);
void Patch_ApplyMatrix(int n, const vec3_t vOrigin, const vec3_t vMatrix[3]);
void Patch_EditPatch();
void Patch_Deselect();
void Patch_Deselect(int n);
void Patch_Delete(int n);
void Patch_Select(int n);
void Patch_Scale(int n, const vec3_t vOrigin, const vec3_t vAmt, bool bRebuilt = true);
void Patch_Cleanup();
void Patch_SetView(int n);
void Patch_SetTexture(int n, texdef_t *tex_def);
void Patch_BrushToMesh(bool bCone = false, bool bBevel = false, bool bEndcap = false, bool bSquare = false, int nHeight = 3);
bool Patch_DragScale(int n, vec3_t vAmt, vec3_t vMove);
void Patch_ReadBuffer(char* pBuff, bool bSelect = false);
void Patch_WriteFile (CMemFile* pMemFile);
void Patch_UpdateSelected(vec3_t vMove);
void Patch_AddRow(int n);
brush_t* Patch_Parse(bool bOld);
void Patch_Write (int n, FILE *f);
void Patch_Write (int n, CMemFile *file);
void Patch_AdjustColumns(int n, int nCols);
void Patch_AdjustRows(int n, int nRows);
void Patch_AdjustSelected(bool bInsert, bool bColumn, bool bFlag);
void Patch_RotateTexture(int n, float fAngle);
void Patch_ScaleTexture(int n, float fx, float fy, bool bFixup = true);
void Patch_ShiftTexture(int n, float fx, float fy);
void Patch_DrawCam(int n, bool bIsGhost);
void Patch_DrawXY(int n, bool bIsGhost);
void Patch_InsertColumn(int n, bool bAdd);
void Patch_InsertRow(int n, bool bAdd);
void Patch_RemoveRow(int n, bool bFirst);
void Patch_RemoveColumn(int n, bool bFirst);
void Patch_ToggleInverted();
void Patch_Restore(int n);
void Patch_Save(int n);
void Patch_SetTextureInfo(texdef_t* pt);
void Patch_NaturalTexturing();
void Patch_ResetTexturing(float fx, float fy);
void Patch_FitTexturing();
void Patch_BendToggle();
void Patch_StartInsDel();
void Patch_BendHandleTAB();
void Patch_BendHandleENTER();
void Patch_SelectBendNormal();
void Patch_SelectBendAxis();
bool OnlyPatchesSelected();
bool AnyPatchesSelected();
void Patch_CapCurrent(bool bInvertedBevel = false, bool bInvertedEndcap = false);
void Patch_DisperseRows();
void Patch_DisperseColumns();
void Patch_NaturalizeSelected(bool bCap = false, bool bCycleCap = false);
void Patch_SelectAreaPoints();
void Patch_InvertTexture(bool bY);
void Patch_InsDelToggle();
void Patch_InsDelHandleTAB();
void Patch_InsDelHandleENTER();
void Patch_SetOverlays();
void Patch_ClearOverlays();
void Patch_Thicken(int nAmount, bool bSeam);
void Patch_Transpose();
const char* Patch_GetTextureName();
bool Patch_FindReplaceTexture(brush_t *pb, const char *pFind, const char *pReplace, bool bForce);
const char *Patch_FromBrush_GetTextureName(brush_t* pb);
void Patch_ReplaceQTexture(brush_t *pb, qtexture_t *pOld, qtexture_t *pNew);
void Patch_MakeNonSolid(brush_t* pb);
void Patch_MakeSolid(brush_t* pb);
void Select_SnapToGrid();
extern bool g_bPatchShowBounds;
extern bool g_bPatchWireFrame;
extern bool g_bPatchWeld;
extern bool g_bPatchDrillDown;
extern bool g_bPatchInsertMode;
extern bool g_bPatchBendMode;
extern vec3_t g_vBendOrigin;

#define UpdatePatchInspector()

// some ent key/pair field names...
//
#define sKEYFIELD_AUTOBOUND "autobound"
#define sKEYFIELD_GROUPNAME "groupname"
#define sKEYFIELD_MODEL		"model"
#define fTEXTURE_SCALE		0.25f

#endif
