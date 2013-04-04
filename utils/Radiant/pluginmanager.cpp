// PlugInManager.cpp: implementation of the CPlugInManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "io.h"
#include "Radiant.h"
#include "PlugInManager.h"
#include "PlugIn.h"
#include "DialogInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlugInManager::CPlugInManager()
{
  m_pTexturePlug = NULL;
  m_pSurfaceListPlug = NULL;
}

CPlugInManager::~CPlugInManager()
{
  Cleanup();
}

void CPlugInManager::Init(const char * pPath)
{
  Cleanup();

  CString strPath(pPath);
  strPath += "*.dll";
  
  bool bGo = true;

	struct _finddata_t fileinfo;
	int handle = _findfirst (strPath, &fileinfo);
	if (handle != -1)
	{
		do
		{
      strPath.Format("%s\\%s", pPath, fileinfo.name);
      CPlugIn *pPlug = new CPlugIn();
      if (pPlug->load(strPath))
      {
		  if(FillFuncTable(pPlug))		// PGM
		  {
			  m_PlugIns.Add(pPlug);
			  g_pParentWnd->AddPlugInMenuItem(pPlug);
			  
			  // if this thing handles textures
			  if (pPlug->getTextureInfo() != NULL)
			  {
				  this->m_pTexturePlug = pPlug;
				  
				  // if this is a wad style texture extension, have it load everything now
				  if (pPlug->getTextureInfo()->m_bWadStyle)
				  {
					  CString strPath = ValueForKey(g_qeglobals.d_project_entity, "texturepath");
					  pPlug->loadTexture(strPath);
				  }
			  }
			  
			  if (pPlug->getSurfaceFlags() != NULL)
			  {
				  this->m_pSurfaceListPlug = pPlug;
			  }
		  }
		  else
		  {
			  delete pPlug;				// PGM
		  }
      }
      else
      {
        delete pPlug;
      }
		} while (_findnext( handle, &fileinfo ) != -1);
		_findclose (handle);
	}

}

void CPlugInManager::Cleanup()
{
  int i;
  for (i = 0; i < m_PlugIns.GetSize(); i++)
  {
    CPlugIn *plug = reinterpret_cast<CPlugIn*>(m_PlugIns.GetAt(i));
    plug->free();
    delete plug;
  }
  m_PlugIns.RemoveAll();

  for (i = 0; i < m_BrushHandles.GetSize(); i++)
  {
    brush_t *pb = reinterpret_cast<brush_t*>(m_BrushHandles.GetAt(i));
    Brush_Free(pb);
  }
  m_BrushHandles.RemoveAll();
}

void CPlugInManager::Dispatch(int n, const char * p)
{
  for (int i = 0; i < m_PlugIns.GetSize(); i++)
  {
    CPlugIn *plug = reinterpret_cast<CPlugIn*>(m_PlugIns.GetAt(i));
    if (plug->ownsCommandID(n))
    {
      vec3_t vMin, vMax;
    	if (selected_brushes.next == &selected_brushes)
      {
        vMin[0] = vMin[1] = vMin[2] = 0;
        VectorCopy(vMin, vMax);
      }
      else
      {
        Select_GetBounds (vMin, vMax);
      }
      plug->dispatchCommand(p, vMin, vMax, QE_SingleBrush(true));	// PGM -- added quiet
      break;
    }
  }
}




void WINAPI QERApp_CreateBrush(vec3_t vMin, vec3_t vMax)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

  brush_t* pBrush = Brush_Create(vMin, vMax, &g_qeglobals.d_texturewin.texdef);
	Entity_LinkBrush (world_entity, pBrush);
  Brush_Build(pBrush);
	Brush_AddToList (pBrush, &active_brushes);
  Select_Brush(pBrush);
  Sys_UpdateWindows(W_ALL);
}


LPVOID CPlugInManager::CreateBrushHandle()
{
	brush_t *pb = reinterpret_cast<brush_t*>(qmalloc (sizeof(brush_t)));
  m_BrushHandles.Add(pb);
  return (LPVOID)pb;
}


void CPlugInManager::DeleteBrushHandle(void * vp)
{
  CPtrArray* pHandles[3];
  pHandles[0] = &m_SelectedBrushHandles;
  pHandles[1] = &m_ActiveBrushHandles;
  pHandles[2] = &m_BrushHandles;

  for (int j = 0; j < 3; j++)
  {
    for (int i = 0; i < pHandles[j]->GetSize(); i++)
    {
      brush_t *pb = reinterpret_cast<brush_t*>(pHandles[j]->GetAt(i));
      if (pb == reinterpret_cast<brush_t*>(vp))
      {
        if (j == 2)
        {
          // only remove it from the list if it is work area
          // this allows the selected and active list indexes to remain constant
          // throughout a session (i.e. between an allocate and release)
          pHandles[j]->RemoveAt(i);
        }
        Brush_Free(pb);
		Sys_MarkMapModified();		// PGM
        return;
      }
    }
  }
}

void CPlugInManager::CommitBrushHandleToMap(void * vp)
{
	g_bScreenUpdates = false;	//stop the madness
  
	for (int i = 0; i < m_BrushHandles.GetSize(); i++)
  {
    brush_t *pb = reinterpret_cast<brush_t*>(m_BrushHandles.GetAt(i));
    if (pb == reinterpret_cast<brush_t*>(vp))
    {
      m_BrushHandles.RemoveAt(i);
	    Entity_LinkBrush (world_entity, pb);
      Brush_Build(pb);
	    Brush_AddToList (pb, &active_brushes);
      Select_Brush(pb);
    }
  }
	g_bScreenUpdates = true;	//resume the madness
  Sys_UpdateWindows(W_ALL);
}


void CPlugInManager::AddFaceToBrushHandle(void * vp, vec3_t v1, vec3_t v2, vec3_t v3)
{
  brush_t *bp = FindBrushHandle(vp);
  if (bp != NULL)
  {
		face_t *f = Face_Alloc();
		f->texdef = g_qeglobals.d_texturewin.texdef;
		f->texdef.flags &= ~SURF_KEEP;
		f->texdef.contents &= ~CONTENTS_KEEP;
		f->next = bp->brush_faces;
		bp->brush_faces = f;
		VectorCopy (v1, f->planepts[0]);
		VectorCopy (v2, f->planepts[1]);
		VectorCopy (v3, f->planepts[2]);
  }
}

brush_t* CPlugInManager::FindBrushHandle(void * vp)
{
  CPtrArray* pHandles[3];
  pHandles[0] = &m_SelectedBrushHandles;
  pHandles[1] = &m_ActiveBrushHandles;
  pHandles[2] = &m_BrushHandles;

  for (int j = 0; j < 3; j++)
  {
    for (int i = 0; i < pHandles[j]->GetSize(); i++)
    {
      brush_t *pb = reinterpret_cast<brush_t*>(pHandles[j]->GetAt(i));
      if (pb == reinterpret_cast<brush_t*>(vp))
      {
        return pb;
      }
    }
  }
  return NULL;
}

LPVOID WINAPI QERApp_CreateBrushHandle()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  return g_pParentWnd->GetPlugInMgr().CreateBrushHandle();
}

void WINAPI QERApp_DeleteBrushHandle(LPVOID vp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  g_pParentWnd->GetPlugInMgr().DeleteBrushHandle(vp);
}

void WINAPI QERApp_CommitBrushHandleToMap(LPVOID vp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  g_pParentWnd->GetPlugInMgr().CommitBrushHandleToMap(vp);
}

void WINAPI QERApp_AddFace(LPVOID vp, vec3_t v1, vec3_t v2, vec3_t v3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  g_pParentWnd->GetPlugInMgr().AddFaceToBrushHandle(vp, v1, v2, v3);
}

void WINAPI QERApp_DeleteSelection()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  Select_Delete();
}

void WINAPI QERApp_SysMsg(LPCSTR pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  CString str = pMsg;
  Sys_Printf(str.GetBuffer(0));
}

void WINAPI QERApp_InfoMsg(LPCSTR pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ShowInfoDialog(pMsg);
}

void WINAPI QERApp_HideInfoMsg()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  HideInfoDialog();
}

//=====
//PGM
void WINAPI QERApp_PositionView(vec3_t	v1, vec3_t v2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	g_pParentWnd->ActiveXY()->SetOrigin(v1);
	// FIXME - implement this!
	Sys_UpdateWindows(W_ALL);
}
//PGM
//=====

//FIXME: this AcquirePath stuff is pretty much a mess and needs cleaned up
bool g_bPlugWait = false;
bool g_bPlugOK = false;
int  g_nPlugCount = 0;

void _PlugDone(bool b, int n)
{
  g_bPlugWait = false;
  g_bPlugOK = b;
  g_nPlugCount = n; //++timo added number of entered points
}

void WINAPI QERApp_GetPoints(int nMax, _QERPointData *pData, LPCSTR pMsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ShowInfoDialog(pMsg);
  g_bPlugWait = true;
  g_bPlugOK = false;
  g_nPlugCount = 0; //++timo replaced by number of points
//  g_nPlugCount=nMax-1;
  AcquirePath(nMax, &_PlugDone);
  while (g_bPlugWait)
  {
    MSG msg;
    if (::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )) 
    { 
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  HideInfoDialog();
  
  pData->m_nCount = 0;
  pData->m_pVectors = NULL;

  if (g_bPlugOK && g_nPlugCount > 0)
  {
    pData->m_nCount = g_nPlugCount;
    pData->m_pVectors = reinterpret_cast<vec3_t*>(qmalloc(g_nPlugCount * sizeof(vec3_t)));
    vec3_t *pOut = pData->m_pVectors;
    for (int i = 0; i < g_nPlugCount; i++)
    {
	//++timo
	/*
	memcpy(pOut, &g_PathPoints[i], sizeof(vec3_t));
	pOut += sizeof(vec3_t);
	*/
	// CClipPoint g_PathPoints[256];
	memcpy(pOut, &g_PathPoints[i],sizeof(vec3_t));
	pOut++;
    }
  }
}

void WINAPI QERApp_AddFaceData(LPVOID pv, _QERFaceData *pData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  brush_t* pBrush = g_pParentWnd->GetPlugInMgr().FindBrushHandle(pv);
  if (pBrush != NULL)
  {
		face_t *f = Face_Alloc();
		f->texdef = g_qeglobals.d_texturewin.texdef;
		f->texdef.flags = pData->m_nFlags;
    f->texdef.contents = pData->m_nContents;
    f->texdef.value = pData->m_nValue;
    f->texdef.rotate = pData->m_fRotate;
    f->texdef.shift[0] = pData->m_fShift[0];
    f->texdef.shift[1] = pData->m_fShift[1];
    f->texdef.scale[0] = pData->m_fScale[0];
    f->texdef.scale[1] = pData->m_fScale[1];
    strcpy(f->texdef.name, pData->m_TextureName);
		f->next = pBrush->brush_faces;
		pBrush->brush_faces = f;
		VectorCopy (pData->m_v1, f->planepts[0]);
		VectorCopy (pData->m_v2, f->planepts[1]);
		VectorCopy (pData->m_v3, f->planepts[2]);
		Sys_MarkMapModified();		// PGM
  }
}

int WINAPI QERApp_GetFaceCount(LPVOID pv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  int n = 0;
  brush_t *pBrush = g_pParentWnd->GetPlugInMgr().FindBrushHandle(pv);
  if (pBrush != NULL)
  {
	  for (face_t *f = pBrush->brush_faces ; f; f = f->next)
    {
      n++;
    }
  }
  return n;
}

_QERFaceData* WINAPI QERApp_GetFaceData(LPVOID pv, int nFaceIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  static _QERFaceData face;
  int n = 0;
  brush_t *pBrush = g_pParentWnd->GetPlugInMgr().FindBrushHandle(pv);
  if (pBrush != NULL)
  {
	  for (face_t *f = pBrush->brush_faces ; f; f = f->next)
    {
      if (n == nFaceIndex)
      {
        face.m_nContents = f->texdef.contents;
        face.m_nFlags = f->texdef.flags;
        face.m_fRotate = f->texdef.rotate;
        face.m_fScale[0] = f->texdef.scale[0];
        face.m_fScale[1] = f->texdef.scale[1];
        face.m_fShift[0] = f->texdef.shift[0];
        face.m_fShift[1] = f->texdef.shift[1];
        face.m_nValue = f->texdef.value;
        strcpy(face.m_TextureName, f->texdef.name);
        VectorCopy(f->planepts[0], face.m_v1);
        VectorCopy(f->planepts[1], face.m_v2);
        VectorCopy(f->planepts[2], face.m_v3);
        return &face;
      }
      n++;
    }
  }
  return NULL;
}

void WINAPI QERApp_SetFaceData(LPVOID pv, int nFaceIndex, _QERFaceData *pData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  int n = 0;
  brush_t *pBrush = g_pParentWnd->GetPlugInMgr().FindBrushHandle(pv);
  if (pBrush != NULL)
  {
	  for (face_t *f = pBrush->brush_faces ; f; f = f->next)
    {
      if (n == nFaceIndex)
      {
		    f->texdef.flags = pData->m_nFlags;
        f->texdef.contents = pData->m_nContents;
        f->texdef.value = pData->m_nValue;
        f->texdef.rotate = pData->m_fRotate;
        f->texdef.shift[0] = pData->m_fShift[0];
        f->texdef.shift[1] = pData->m_fShift[1];
        f->texdef.scale[0] = pData->m_fScale[0];
        f->texdef.scale[1] = pData->m_fScale[1];
        strcpy(f->texdef.name, pData->m_TextureName);
        VectorCopy(pData->m_v1, f->planepts[0]);
        VectorCopy(pData->m_v2, f->planepts[1]);
        VectorCopy(pData->m_v3, f->planepts[2]);
		    Sys_MarkMapModified();		// PGM
		return;						// PGM
      }
      n++;
    }
  }
}

void WINAPI QERApp_DeleteFace(LPVOID pv, int nFaceIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  int n = 0;
  brush_t *pBrush = g_pParentWnd->GetPlugInMgr().FindBrushHandle(pv);
  if (pBrush != NULL)
  {
    face_t *pPrev = pBrush->brush_faces;
	  for (face_t *f = pBrush->brush_faces; f; f = f->next)
    {
      if (n == nFaceIndex)
      {
        pPrev->next = f->next;
			  Face_Free (f);
		Sys_MarkMapModified();		// PGM
        return;
      }
      n++;
      pPrev = f;
    }
  }
}

//==========
//PGM
void WINAPI QERApp_BuildBrush (LPVOID pv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	brush_t *pBrush = g_pParentWnd->GetPlugInMgr().FindBrushHandle(pv);
	if (pBrush != NULL)
	{
		Brush_Build(pBrush);
		Sys_UpdateWindows(W_ALL);
	}
}

void WINAPI QERApp_SelectBrush (LPVOID pv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	brush_t *pBrush = g_pParentWnd->GetPlugInMgr().FindBrushHandle(pv);
	if (pBrush != NULL)
	{
		Select_Brush(pBrush, false);
		Sys_UpdateWindows(W_ALL);
	}

}

void WINAPI QERApp_DeselectBrush (LPVOID pv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// FIXME - implement this!
}

void WINAPI QERApp_ResetPlugins()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  g_pParentWnd->OnPluginsRefresh();
}

void WINAPI QERApp_DeselectAllBrushes ()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	Select_Deselect();
	Sys_UpdateWindows(W_ALL);
}
//PGM
//==========

void WINAPI QERApp_TextureBrush(LPVOID pv, LPCSTR pName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  brush_t *pBrush = g_pParentWnd->GetPlugInMgr().FindBrushHandle(pv);
  if (pBrush != NULL)
  {
	  for (face_t *f = pBrush->brush_faces ; f; f = f->next)
    {
      strcpy(f->texdef.name, pName);
    }
    Sys_MarkMapModified();		// PGM
  }
}

int WINAPI QERApp_SelectedBrushCount()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  int n = 0;
	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    n++;
  }
  return n;
}

int WINAPI QERApp_ActiveBrushCount()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  int n = 0;
	for (brush_t *pb = active_brushes.next ; pb != &active_brushes ; pb = pb->next)
	{
    n++;
  }
  return n;
}

int WINAPI QERApp_AllocateSelectedBrushHandles()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  int n = 0;
	for (brush_t *pb = selected_brushes.next ; pb != &selected_brushes ; pb = pb->next)
	{
    n++;
    g_pParentWnd->GetPlugInMgr().GetSelectedHandles().Add(pb);
  }
  return n;
}

int WINAPI QERApp_AllocateActiveBrushHandles()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  int n = 0;
	for (brush_t *pb = active_brushes.next ; pb != &active_brushes ; pb = pb->next)
	{
    n++;
    g_pParentWnd->GetPlugInMgr().GetActiveHandles().Add(pb);
  }
  return n;
}

void WINAPI QERApp_ReleaseSelectedBrushHandles()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  g_pParentWnd->GetPlugInMgr().GetSelectedHandles().RemoveAll();
  Sys_UpdateWindows(W_ALL);
}

void WINAPI QERApp_ReleaseActiveBrushHandles()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  g_pParentWnd->GetPlugInMgr().GetActiveHandles().RemoveAll();
  Sys_UpdateWindows(W_ALL);
}

LPVOID WINAPI QERApp_GetActiveBrushHandle(int nIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  if (nIndex < g_pParentWnd->GetPlugInMgr().GetActiveHandles().GetSize())
  {
    return reinterpret_cast<LPVOID>(g_pParentWnd->GetPlugInMgr().GetActiveHandles().GetAt(nIndex));
  }
  return NULL;
}

LPVOID WINAPI QERApp_GetSelectedBrushHandle(int nIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  if (nIndex < g_pParentWnd->GetPlugInMgr().GetSelectedHandles().GetSize())
  {
    return reinterpret_cast<LPVOID>(g_pParentWnd->GetPlugInMgr().GetSelectedHandles().GetAt(nIndex));
  }
  return NULL;
}

int WINAPI QERApp_TextureCount()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	Texture_StartPos ();
  int x, y;
  int n = 0;
	while (1)
	{
		qtexture_t *q = Texture_NextPos (&x, &y);
		if (!q)
			break;
    n++;
  }
  return n;
}

LPCSTR WINAPI QERApp_GetTexture(int nIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  static char name[QER_MAX_NAMELEN];
	Texture_StartPos ();
  int x, y;
  int n = 0;
	while (1)
	{
		qtexture_t *q = Texture_NextPos (&x, &y);
		if (!q)
			break;
    if (n == nIndex)
    {
      strcpy(name, q->name);
      return name;
    }
    n++;
  }
  return NULL;
}

LPCSTR WINAPI QERApp_GetCurrentTexture()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  return g_qeglobals.d_texturewin.texdef.name;
}

void WINAPI QERApp_SetCurrentTexture(LPCSTR strName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  texdef_t tex;
  strcpy(tex.name, strName);
  Texture_SetTexture(&tex);
}

int WINAPI QERApp_GetEClassCount()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  int n = 0;
  for (eclass_t *e = eclass ; e ; e = e->next)
	{
    n++;
  }
  return n;
}

LPCSTR WINAPI QERApp_GetEClass(int nIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  int n = 0;
  for (eclass_t *e = eclass ; e ; e = e->next)
	{
    if (n == nIndex)
    {
      return e->name;
    }
  }
  return NULL;
}

void WINAPI QERApp_LoadTextureRGBA(LPVOID vp)
{
  Texture_LoadFromPlugIn(vp);
}


_QERTextureInfo* CPlugInManager::GetTextureInfo()
{
  if (m_pTexturePlug != NULL)
  {
    return m_pTexturePlug->getTextureInfo();
  }
  else
  {
    return NULL;
  }
}

LPVOID CPlugInManager::GetSurfaceFlags()
{
  if (m_pSurfaceListPlug != NULL)
  {
    return m_pSurfaceListPlug->getSurfaceFlags();
  }
  else
  {
    return NULL;
  }
}

extern qtexture_t	*g_pluginTexture;

void CPlugInManager::LoadTexture(const char *pFilename)
{
	g_pluginTexture = NULL;
	if (m_pTexturePlug != NULL)
  {
    m_pTexturePlug->loadTexture(pFilename);
  }
}


int CPlugInManager::FillFuncTable(CPlugIn *pPlug)
{
  _QERFuncTable_1 *pTable = reinterpret_cast<_QERFuncTable_1*>(pPlug->getFuncTable());
  if (pTable != NULL)
  {
    if (pTable->m_fVersion != QER_PLUG_VERSION)
    {      
	  Sys_Printf("Radiant plugin manager was built with version %.2f, Plugin %s is version %.2f\n", QER_PLUG_VERSION, pPlug->getVersionStr(), pTable->m_fVersion);
    }
    if (pTable->m_fVersion >= QER_PLUG_VERSION_1)
    {
      pTable->m_pfnCreateBrush = &QERApp_CreateBrush;
      pTable->m_pfnCreateBrushHandle = &QERApp_CreateBrushHandle;
      pTable->m_pfnDeleteBrushHandle = &QERApp_DeleteBrushHandle;
      pTable->m_pfnCommitBrushHandle = &QERApp_CommitBrushHandleToMap;
      pTable->m_pfnAddFace = &QERApp_AddFace;
      pTable->m_pfnAddFaceData = &QERApp_AddFaceData;
      pTable->m_pfnGetFaceData = &QERApp_GetFaceData;
      pTable->m_pfnGetFaceCount = &QERApp_GetFaceCount;
      pTable->m_pfnSetFaceData = &QERApp_SetFaceData;
      pTable->m_pfnDeleteFace = &QERApp_DeleteFace;
      pTable->m_pfnTextureBrush = &QERApp_TextureBrush;
	    pTable->m_pfnBuildBrush = &QERApp_BuildBrush;						// PGM
  	  pTable->m_pfnSelectBrush = &QERApp_SelectBrush;					// PGM
	    pTable->m_pfnDeselectBrush = &QERApp_DeselectBrush;				// PGM
	    pTable->m_pfnDeselectAllBrushes = &QERApp_DeselectAllBrushes;		// PGM
      pTable->m_pfnDeleteSelection = &QERApp_DeleteSelection;
      pTable->m_pfnGetPoints = &QERApp_GetPoints;
      pTable->m_pfnSysMsg = &QERApp_SysMsg;
      pTable->m_pfnInfoMsg = &QERApp_InfoMsg;
      pTable->m_pfnHideInfoMsg = &QERApp_HideInfoMsg;
      pTable->m_pfnPositionView = &QERApp_PositionView;					// PGM
      pTable->m_pfnSelectedBrushCount = &QERApp_SelectedBrushCount;
      pTable->m_pfnAllocateSelectedBrushHandles  = &QERApp_AllocateSelectedBrushHandles;
      pTable->m_pfnReleaseSelectedBrushHandles  = &QERApp_ReleaseSelectedBrushHandles;
      pTable->m_pfnGetSelectedBrushHandle = &QERApp_GetSelectedBrushHandle;
      pTable->m_pfnActiveBrushCount = &QERApp_ActiveBrushCount;
      pTable->m_pfnAllocateActiveBrushHandles = &QERApp_AllocateActiveBrushHandles;
      pTable->m_pfnReleaseActiveBrushHandles = &QERApp_ReleaseActiveBrushHandles;
      pTable->m_pfnGetActiveBrushHandle = &QERApp_GetActiveBrushHandle;
      pTable->m_pfnTextureCount = &QERApp_TextureCount;
      pTable->m_pfnGetTexture = &QERApp_GetTexture;
      pTable->m_pfnGetCurrentTexture = &QERApp_GetCurrentTexture;
      pTable->m_pfnSetCurrentTexture = &QERApp_SetCurrentTexture;
      pTable->m_pfnGetEClassCount = &QERApp_GetEClassCount;
      pTable->m_pfnGetEClass = &QERApp_GetEClass;
      pTable->m_pfnResetPlugins = &QERApp_ResetPlugins;
    }
    // end of v1.00
    if (pTable->m_fVersion >= QER_PLUG_VERSION)
    {
      // v1.50 starts
      pTable->m_pfnLoadTextureRGBA = &QERApp_LoadTextureRGBA;
      // end of v1.50
    }

	  return true;
  }
  else
  {
    Sys_Printf("Unable to load %s because the function tables are not the same size\n", pPlug->getVersionStr());
  }
  return false;
}
