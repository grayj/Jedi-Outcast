// PlugInManager.h: interface for the CPlugInManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLUGINMANAGER_H__CFB18412_55FE_11D2_B082_00AA00A410FC__INCLUDED_)
#define AFX_PLUGINMANAGER_H__CFB18412_55FE_11D2_B082_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "plugin.h"

class CPlugInManager  
{
private:
  CObArray m_PlugIns;
  CPtrArray m_BrushHandles;
  CPtrArray m_SelectedBrushHandles;
  CPtrArray m_ActiveBrushHandles;

  CPlugIn *m_pTexturePlug;
  CPlugIn *m_pSurfaceListPlug;

public:
  CPtrArray& GetActiveHandles() {return m_ActiveBrushHandles; };
  CPtrArray& GetSelectedHandles() {return m_SelectedBrushHandles; };
	brush_t* FindBrushHandle(void *vp);
	void AddFaceToBrushHandle(void *vp, vec3_t v1, vec3_t v2, vec3_t v3);
	void CommitBrushHandleToMap(void *vp);
	void DeleteBrushHandle(LPVOID vp);
	LPVOID CreateBrushHandle();
	void Dispatch(int n, const char *p);
	void Cleanup();
	void Init(const char* pPath);
	CPlugInManager();
	virtual ~CPlugInManager();

  // the texture manager front ends the single load
  // addins (texture, model, map formats.. etc.)
  _QERTextureInfo* GetTextureInfo();
  void LoadTexture(const char *pFilename);

  LPVOID GetSurfaceFlags();



protected:
	int FillFuncTable(CPlugIn *pPlug);		// PGM
};

#endif // !defined(AFX_PLUGINMANAGER_H__CFB18412_55FE_11D2_B082_00AA00A410FC__INCLUDED_)
