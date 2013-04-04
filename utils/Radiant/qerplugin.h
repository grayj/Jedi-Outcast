// QERadiant PlugIns
//
//
#include <windows.h>
#include "qertypes.h"

#define QER_PLUG_VERSION_1 1.00
#define QER_PLUG_VERSION 1.50

#define QER_MAX_NAMELEN 1024

// the editor will look for plugins in two places, the plugins path 
// under the application path, and the path under the basepath as defined
// in the project (.qe4) file.
//
// you can drop any number of new texture, model format DLL's in the standard plugin path
// but only one plugin that overrides map loading/saving, surface dialog, surface flags, etc.. 
// should be used at one time.. if multiples are loaded then the last one loaded will be the 
// active one
//
// type of services the plugin supplies, pass any combo of these flags
// it is assumed the plugin will have a matching function as defined below
// to correlate to the implied functionality
// 
// FIXME: after specing this crap i went to a simpler model so this may disappear
#define QER_PLUG_GAME_TEXTURE       0x0001      // defines new texture format
#define QER_PLUG_GAME_MODEL         0x0002      // defines new model format
#define QER_PLUG_GAME_MAP           0x0004      // handles map load/save
#define QER_PLUG_GAME_SURFACEDLG    0x0008      // handles surface dialog
#define QER_PLUG_GAME_SURFACEFLAGS  0x0010      // renames surface/content names

// basics
#define QERPLUG_INIT "QERPlug_Init"
#define QERPLUG_GETNAME "QERPlug_GetName"
#define QERPLUG_GETCOMMANDLIST "QERPlug_GetCommandList"
#define QERPLUG_DISPATCH "QERPlug_Dispatch"
#define QERPLUG_GETFUNCTABLE "QERPlug_GetFuncTable"

// FIXME: not used, probably should remove
#define QERPLUG_GETSERVICETPE "QERPlug_GetServiceType"

// game stuff
#define QERPLUG_GETTEXTUREINFO "QERPlug_GetTextureInfo"   // gets a texture info structure
#define QERPLUG_LOADTEXTURE    "QERPlug_LoadTexture"      // loads a texture, will return an RGBA structure
                                                          // and any surface flags/contents for it
#define QERPLUG_GETTMODELINFO "QERPlug_GetModelInfo"      // gets a model info structure
#define QERPLUG_LOADMODEL     "QERPlug_LoadModel"         // loads model data from a plugin
#define QERPLUG_DOSURFACEDLG  "QERPlug_DoSurfaceDlg"      // runs the surface dialog in a plugin
                                                          // this is going to get icky
#define QERPLUG_GETSURFACEFLAGS "QERPlug_GetSurfaceFlags" // gets a list of surface/content flag names from a plugin

struct _QERTextureInfo
{
  char m_TextureExtension[QER_MAX_NAMELEN];   // the extension these textures have
  bool m_bHiColor;    // if textures are NOT high color, the default 
                      // palette (as described inthe qe4 file will be used for gamma correction)
                      // if they are high color, gamma and shading are computed on the fly 
                      // based on the rgba data
  //--bool m_bIsShader;   // will probably do q3 shaders this way when i merge
  bool m_bWadStyle;   // if this is true, the plugin will be presented with the texture path
                      // defined in the .qe4 file and is expected to preload all the textures
  bool m_bHalfLife;   // causes brushes to be saved/parsed without the surface contents/flags/value
};

struct _QERTextureLoad    // returned by a plugin
{
  _QERTextureLoad()
  { 
    memset(reinterpret_cast<void*>(this), 0, sizeof(_QERTextureLoad));
  };

  ~_QERTextureLoad()
  {
    delete []m_pRGBA;
    delete []m_pName;
  };

  void makeSpace(int nSize)
  {
    m_pRGBA = new unsigned char[nSize+1];
  };

  void setName(const char* p)
  {
    m_pName = new char[strlen(p)+1];
    strcpy(m_pName, p);
  };


  unsigned char *m_pRGBA; // rgba data (alpha channel is supported and drawn appropriately)
  int m_nWidth;           // width
  int m_nHeight;          // height
  int m_nContents;        // default contents
  int m_nFlags;           // "" flags
  int m_nValue;           // "" value
  char *m_pName;          // name to be referenced in map, build tools, etc.
};

struct _QERModelInfo
{
  char m_ModelExtension[QER_MAX_NAMELEN];
  bool m_bSkinned;
  bool m_bMultipart;
};

struct _QERModelLoad
{
  // vertex and skin data
};


// hook functions
// FIXME: none of the hook stuff works for v1.00
#define  QERPLUG_MAPLOAD "QERPlug_MapLoad"
#define  QERPLUG_MAPSAVE "QERPlug_MapSave"


//=========================================
// plugin functions version 1.0
typedef LPCSTR (WINAPI* PFN_QERPLUG_INIT)(HMODULE hApp, HWND hwndMain);
typedef LPCSTR (WINAPI* PFN_QERPLUG_GETNAME)();
typedef LPCSTR (WINAPI* PFN_QERPLUG_GETCOMMANDLIST)();
typedef void (WINAPI* PFN_QERPLUG_DISPATCH)(LPCSTR p, vec3_t vMin, vec3_t vMax, BOOL bSingleBrush);
typedef LPVOID (WINAPI* PFN_QERPLUG_GETFUNCTABLE)();
typedef void (WINAPI* PFN_QERPLUG_MAPLOAD)();
typedef void (WINAPI* PFN_QERPLUG_MAPSAVE)();

// editor defined plugin dispatches
// these are used to signal the completion after a 'Get' function is called in the editor
#define QERPLUG_DISPATCH_POINTDONE "PointDone"
#define QERPLUG_DISPATCH_BRUSHDONE "BrushDone"

// v1.5
//
// Texture loading
// returns a ptr to _QERTextureInfo
typedef LPVOID (WINAPI* PFN_QERPLUG_GETTEXTUREINFO)();
//
// loads a texture by calling the texture load func in the editor (defined below)
// transparency (for water, fog, lava, etc.. ) can be emulated in the editor
// by passing in appropriate alpha data or by setting the appropriate surface flags
// expected by q2 (which the editor will use.. )
typedef void (WINAPI* PFN_QERPLUG_LOADTEXTURE)(LPCSTR pFilename); 

// v1.6
typedef LPVOID (WINAPI* PFN_QERPLUG_GETSURFACEFLAGS)();




//=========================================
// editor functions

// There are 3 potential brush handle lists
// 1. the list that contains brushes a plugin creates using CreateBrushHandle
// 2. the selected brush list (brushes the user has selected)
// 3. the active brush list (brushes in the map that are not selected)
// 
// In general, the same things can be done to brush handles (face manip, delete brushhandle, etc.. ) in each
// list. There are a few exceptions. 
// 1. You cannot commit a selected or active brush handle to the map. This is because it is already in the map. 
// 2. You cannot bind brush handles from the selected or active brush list to an entity. As of v1.0 of the plugins
// the only way for a plugin to create entities is to create a brush handles (or a list of handles) and then bind
// them to an entity. This will commit the brush(s) and/or the entities to the map as well.
// 
// To use the active or selected brush lists, you must first allocate them (which returns a count) and then
// release them when you are finish manipulating brushes in one of those lists. 


// brush manipulation routines
#define QERAPP_CREATEBRUSH "QERApp_CreateBrush"
#define QERAPP_CREATEBRUSHHANDLE "QERApp_CreateBrushHandle"
#define QERAPP_DELETEBRUSHHANDLE "QERApp_DeleteBrushHandle"
#define QERAPP_COMMITBRUSHHANDLETOMAP "QERApp_CommitBrushHandleToMap"
#define QERAPP_BINDHANDLESTOENTITY "QERApp_BindHandlesToEntity"
#define QERAPP_ADDFACE "QERApp_AddFace"
#define QERAPP_ADDFACEDATA "QERApp_AddFaceData"
#define QERAPP_GETFACECOUNT "QERApp_GetFaceCount"
#define QERAPP_GETFACEDATA "QERApp_GetFaceData"
#define QERAPP_SETFACEDATA "QERApp_SetFaceData"
#define QERAPP_DELETEFACE "QERApp_DeleteFace"
#define QERAPP_TEXTUREBRUSH "QERApp_TextureBrush"
#define QERAPP_BUILDBRUSH "QERApp_BuildBrush"					// PGM
#define QERAPP_SELECTEDBRUSHCOUNT "QERApp_SelectedBrushCount"
#define QERAPP_ALLOCATESELECTEDBRUSHHANDLES "QERApp_AllocateSelectedBrushHandles"
#define QERAPP_RELEASESELECTEDBRUSHHANDLES "QERApp_ReleaseSelectedBrushHandles"
#define QERAPP_GETSELECTEDBRUSHHANDLE "QERApp_GetSelectedBrushHandle"
#define QERAPP_ACTIVEBRUSHCOUNT "QERApp_ActiveBrushCount"
#define QERAPP_ALLOCATEACTIVEBRUSHHANDLES "QERApp_AllocateActiveBrushHandles"
#define QERAPP_RELEASEACTIVEBRUSHHANDLES "QERApp_ReleaseActiveBrushHandles"
#define QERAPP_GETACTIVEBRUSHHANDLE "QERApp_GetActiveBrushHandle"

// texture stuff
#define QERAPP_TEXTURECOUNT "QERApp_TextureCount"
#define QERAPP_GETTEXTURE "QERApp_GetTexture"
#define QERAPP_GETCURRENTTEXTURE "QERApp_GetCurrentTexture"
#define QERAPP_SETCURRENTTEXTURE "QERApp_SetCurrentTexture"

// selection 
#define QERAPP_DELETESELECTION "QERApp_DeleteSelection"
#define QERAPP_SELECTBRUSH "QERApp_SelectBrush"					// PGM
#define QERAPP_DESELECTBRUSH "QERApp_DeselectBrush"				// PGM
#define QERAPP_DESELECTALLBRUSHES "QERApp_DeselectAllBrushes"	// PGM

// data gathering
#define QERAPP_GETPOINTS "QERApp_GetPoints"
#define QERAPP_SELECTBRUSHES "QERApp_GetBrushes"

// entity class stuff
// the entity handling is very basic for 1.0
#define QERAPP_GETECLASSCOUNT "QERApp_GetEClassCount"
#define QERAPP_GETECLASS "QERApp_GetEClass"

// misc
#define QERAPP_SYSMSG "QERApp_SysMsg"
#define QERAPP_INFOMSG "QERApp_InfoMsg"
#define QERAPP_HIDEINFOMSG "QERApp_HideInfoMsg"
#define QERAPP_POSITIONVIEW	"QERApp_PositionView"			// PGM
#define QERAPP_RESET_PLUGINS "QERApp_ResetPlugins"

// texture loading
#define QERAPP_LOADTEXTURERGBA "QERApp_LoadTextureRGBA"

// FIXME: the following are not implemented yet
// hook registrations
#define QERAPP_REGISTER_MAPLOADFUNC "QERApp_Register_MapLoadFunc"
#define QERAPP_REGISTER_MAPSAVEFUNC "QERApp_Register_MapSaveFunc"

// FIXME: the following are not implemented yet
#define QERAPP_REGISTER_PROJECTLOADFUNC "QERApp_Register_ProjectLoadFunc"
#define QERAPP_REGISTER_MOUSEHANDLER "QERApp_Register_MouseHandler"
#define QERAPP_REGISTER_KEYHANDLER "QERApp_Register_KeyHandler"

// FIXME: new primtives do not work in v1.00
// primitives are new types of things in the map
// for instance, the Q3 curves could have been done as 
// primitives instead of being built in 
// it will be a plugins responsibility to hook the map load and save funcs to load
// and/or save any additional data (like new primitives of some type)
// the editor will call each registered renderer during the rendering process to repaint
// any primitives the plugin owns
// each primitive object has a temporary sibling brush that lives in the map
// FIXME: go backwards on this a bit.. orient it more towards the temp brush mode as it will be cleaner
// basically a plugin will hook the map load and save and will add the primitives to the map.. this will
// produce a temporary 'primitive' brush and the appropriate renderer will be called as well as the 
// edit handler (for edge drags, sizes, rotates, etc.. ) and the vertex maker will be called when vertex
// mode is attemped on the brush.. there will need to be a GetPrimitiveBounds callback in the edit handler
// so the brush can resize appropriately as needed.. this might be the plugins responsibility to set the 
// sibling brushes size.. it will then be the plugins responsibility to hook map save to save the primitives
// as the editor will discard any temp primitive brushes.. (there probably needs to be some kind of sanity check
// here as far as keeping the brushes and the plugin in sync.. i suppose the edit handler can deal with all of that
// crap but it looks like a nice place for a mess)
#define QERAPP_REGISTER_PRIMITIVE "QERApp_Register_Primitive"
#define QERAPP_REGISTER_RENDERER "QERApp_Register_Renderer"
#define QERAPP_REGISTER_EDITHANDLER "QERApp_Register_EditHandler"
#define QERAPP_REGISTER_VERTEXMAKER "QERApp_Register_VertexMaker"
#define QERAPP_ADDPRIMITIVE "QERApp_AddPrimitive"

#define QERAPP_GETENTITYCOUNT "QERApp_GetEntityCount"
#define QERAPP_GETENTITYHANDLE "QERApp_GetEntityHandle"
#define QERAPP_GETENTITYINFO "QERApp_GetEntityInfo"
#define QERAPP_GETKEYVALLIST "QERApp_GetKeyValList"
#define QERAPP_SETKEYVALLIST "QERApp_SetKeyValList"
#define QERAPP_ALLOCATEENTITYBRUSHHANDLES "QERApp_AllocateEntityBrushHandles"
#define QERAPP_RELEASEENTITYBRUSHHANDLES "QERApp_ReleaseEntityBrushHandles"
#define QERAPP_GETENTITYBRUSHHANDLE "QERApp_GetEntityBrushHandle"


struct _QERPointData
{
  int     m_nCount;
  vec3_t *m_pVectors;
};

struct _QERFaceData
{
  char  m_TextureName[QER_MAX_NAMELEN];
  int   m_nContents;
  int   m_nFlags;
  int   m_nValue;
  float m_fShift[2];
  float m_fRotate;
  float m_fScale[2];
  vec3_t m_v1, m_v2, m_v3;
};


typedef void (WINAPI* PFN_QERAPP_CREATEBRUSH)(vec3_t vMin, vec3_t vMax);

typedef LPVOID (WINAPI* PFN_QERAPP_CREATEBRUSHHANDLE)();
typedef void (WINAPI* PFN_QERAPP_DELETEBRUSHHANDLE)(LPVOID pv);
typedef void (WINAPI* PFN_QERAPP_COMMITBRUSHHANDLETOMAP)(LPVOID pv);
typedef void (WINAPI* PFN_QERAPP_ADDFACE)(LPVOID pv, vec3_t v1, vec3_t v2, vec3_t v3);

typedef void (WINAPI* PFN_QERAPP_ADDFACEDATA)(LPVOID pv, _QERFaceData *pData);
typedef int  (WINAPI* PFN_QERAPP_GETFACECOUNT)(LPVOID pv);
typedef _QERFaceData* (WINAPI* PFN_QERAPP_GETFACEDATA)(LPVOID pv, int nFaceIndex);
typedef void (WINAPI* PFN_QERAPP_SETFACEDATA)(LPVOID pv, int nFaceIndex, _QERFaceData *pData);
typedef void (WINAPI* PFN_QERAPP_DELETEFACE)(LPVOID pv, int nFaceIndex);
typedef void (WINAPI* PFN_QERAPP_TEXTUREBRUSH)(LPVOID pv, LPCSTR pName);
typedef void (WINAPI* PFN_QERAPP_BUILDBRUSH)(LPVOID pv);		// PGM
typedef void (WINAPI* PFN_QERAPP_SELECTBRUSH)(LPVOID pv);		// PGM
typedef void (WINAPI* PFN_QERAPP_DESELECTBRUSH)(LPVOID pv);		// PGM
typedef void (WINAPI* PFN_QERAPP_DESELECTALLBRUSHES)();			// PGM

typedef void (WINAPI* PFN_QERAPP_DELETESELECTION)();
typedef void (WINAPI* PFN_QERAPP_GETPOINTS)(int nMax, _QERPointData *pData, LPCSTR pMsg);
typedef void (WINAPI* PFN_QERAPP_SYSMSG)(LPCSTR pMsg);
typedef void (WINAPI* PFN_QERAPP_INFOMSG)(LPCSTR pMsg);
typedef void (WINAPI* PFN_QERAPP_HIDEINFOMSG)();
typedef void (WINAPI* PFN_QERAPP_POSITIONVIEW)(vec3_t v1, vec3_t v2);	//PGM

typedef int  (WINAPI* PFN_QERAPP_SELECTEDBRUSHCOUNT)();
typedef int (WINAPI* PFN_QERAPP_ALLOCATESELECTEDBRUSHHANDLES)();
typedef void (WINAPI* PFN_QERAPP_RELEASESELECTEDBRUSHHANDLES)();
typedef LPVOID (WINAPI* PFN_QERAPP_GETSELECTEDBRUSHHANDLE)(int nIndex);

typedef int  (WINAPI* PFN_QERAPP_ACTIVEBRUSHCOUNT)();
typedef int (WINAPI* PFN_QERAPP_ALLOCATEACTIVEBRUSHHANDLES)();
typedef void (WINAPI* PFN_QERAPP_RELEASEACTIVEBRUSHHANDLES)();
typedef LPVOID (WINAPI* PFN_QERAPP_GETACTIVEBRUSHHANDLE)(int nIndex);

typedef int  (WINAPI* PFN_QERAPP_TEXTURECOUNT)();
typedef LPCSTR (WINAPI* PFN_QERAPP_GETTEXTURE)(int nIndex);
typedef LPCSTR (WINAPI* PFN_QERAPP_GETCURRENTTEXTURE)();
typedef void (WINAPI* PFN_QERAPP_SETCURRENTTEXTURE)(LPCSTR pName);

typedef void (WINAPI* PFN_QERAPP_REGISTERMAPLOAD)(LPVOID vp);
typedef void (WINAPI* PFN_QERAPP_REGISTERMAPSAVE)(LPVOID vp);

typedef int (WINAPI* PFN_QERAPP_GETECLASSCOUNT)();
typedef LPCSTR (WINAPI* PFN_QERAPP_GETECLASS)(int nIndex);

typedef void (WINAPI* PFN_QERAPP_RESETPLUGINS)();
//--typedef int (WINAPI* PFN_QERAPP_GETENTITYCOUNT)();


// called by texture loaders for each texture loaded
typedef void (WINAPI* PFN_QERAPP_LOADTEXTURERGBA)(LPVOID vp);

//--typedef LPCSTR (WINAPI* PFN_QERAPP_GETENTITY)(int nIndex);


// FIXME:
// add map format extensions
// add texture format handlers
// add surface dialog handler
// add model handler/displayer

// v1 func table
// Plugins need to declare one of these and implement the getfunctable as described above
struct _QERFuncTable_1
{
  float m_fVersion;
  int   m_nSize;
  PFN_QERAPP_CREATEBRUSH            m_pfnCreateBrush;
  PFN_QERAPP_CREATEBRUSHHANDLE      m_pfnCreateBrushHandle;
  PFN_QERAPP_DELETEBRUSHHANDLE      m_pfnDeleteBrushHandle;
  PFN_QERAPP_COMMITBRUSHHANDLETOMAP m_pfnCommitBrushHandle;
  PFN_QERAPP_ADDFACE                m_pfnAddFace;
  PFN_QERAPP_ADDFACEDATA            m_pfnAddFaceData;
  PFN_QERAPP_GETFACEDATA            m_pfnGetFaceData;
  PFN_QERAPP_GETFACECOUNT           m_pfnGetFaceCount;
  PFN_QERAPP_SETFACEDATA            m_pfnSetFaceData;
  PFN_QERAPP_DELETEFACE             m_pfnDeleteFace;
  PFN_QERAPP_TEXTUREBRUSH           m_pfnTextureBrush;
  PFN_QERAPP_BUILDBRUSH				m_pfnBuildBrush;				// PGM
  PFN_QERAPP_SELECTBRUSH			m_pfnSelectBrush;				// PGM
  PFN_QERAPP_DESELECTBRUSH			m_pfnDeselectBrush;				// PGM
  PFN_QERAPP_DESELECTALLBRUSHES		m_pfnDeselectAllBrushes;		// PGM

  PFN_QERAPP_DELETESELECTION        m_pfnDeleteSelection;
  PFN_QERAPP_GETPOINTS              m_pfnGetPoints;
  PFN_QERAPP_SYSMSG                 m_pfnSysMsg;
  PFN_QERAPP_INFOMSG                m_pfnInfoMsg;
  PFN_QERAPP_HIDEINFOMSG            m_pfnHideInfoMsg;
  PFN_QERAPP_POSITIONVIEW			m_pfnPositionView;				// PGM

  PFN_QERAPP_SELECTEDBRUSHCOUNT           m_pfnSelectedBrushCount;
  PFN_QERAPP_ALLOCATESELECTEDBRUSHHANDLES m_pfnAllocateSelectedBrushHandles;
  PFN_QERAPP_RELEASESELECTEDBRUSHHANDLES  m_pfnReleaseSelectedBrushHandles;
  PFN_QERAPP_GETSELECTEDBRUSHHANDLE       m_pfnGetSelectedBrushHandle;

  PFN_QERAPP_ACTIVEBRUSHCOUNT             m_pfnActiveBrushCount;
  PFN_QERAPP_ALLOCATEACTIVEBRUSHHANDLES   m_pfnAllocateActiveBrushHandles;
  PFN_QERAPP_RELEASEACTIVEBRUSHHANDLES    m_pfnReleaseActiveBrushHandles;
  PFN_QERAPP_GETACTIVEBRUSHHANDLE         m_pfnGetActiveBrushHandle;

  PFN_QERAPP_TEXTURECOUNT                 m_pfnTextureCount;
  PFN_QERAPP_GETTEXTURE                   m_pfnGetTexture;
  PFN_QERAPP_GETCURRENTTEXTURE            m_pfnGetCurrentTexture;
  PFN_QERAPP_SETCURRENTTEXTURE            m_pfnSetCurrentTexture;

  PFN_QERAPP_GETECLASSCOUNT         m_pfnGetEClassCount;
  PFN_QERAPP_GETECLASS              m_pfnGetEClass;
  PFN_QERAPP_RESETPLUGINS           m_pfnResetPlugins;
  // v1.00 ends here
  // v1.50 starts here
  PFN_QERAPP_LOADTEXTURERGBA        m_pfnLoadTextureRGBA;
  // v1.50 ends here
};


//--typedef int (WINAPI*      PFN_QERAPP_BRUSHCOUNT     )();
//--typedef brush_t* (WINAPI* PFN_QERAPP_GETBRUSH       )();
//--typedef void (WINAPI*     PFN_QERAPP_DELETEBRUSH    )();
