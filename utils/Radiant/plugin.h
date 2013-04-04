// PlugIn.h: interface for the CPlugIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLUGIN_H__B501A832_5755_11D2_B084_00AA00A410FC__INCLUDED_)
#define AFX_PLUGIN_H__B501A832_5755_11D2_B084_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "qerplugin.h"

class CPlugIn : public CObject
{
private:
  HMODULE m_hDLL;
  PFN_QERPLUG_INIT m_pfnInit;
  PFN_QERPLUG_GETNAME m_pfnGetName;
  PFN_QERPLUG_GETCOMMANDLIST m_pfnGetCommandList;
  PFN_QERPLUG_DISPATCH m_pfnDispatch;
  PFN_QERPLUG_GETFUNCTABLE m_pfnGetFuncTable;
  PFN_QERPLUG_GETTEXTUREINFO m_pfnGetTextureInfo;
  PFN_QERPLUG_LOADTEXTURE m_pfnLoadTexture;
  PFN_QERPLUG_GETSURFACEFLAGS m_pfnGetSurfaceFlags;
  CWordArray m_CommandIDs;
  CStringArray m_CommandStrings;
  CString m_strName;
  CString m_strVersion;

public:
  void* getFuncTable();
	bool ownsCommandID(int n);
	void addMenuID(int n);
	CPlugIn();
	virtual ~CPlugIn();
  bool load(const char *p);
  void free();
  const char* getVersionStr();
  const char* getMenuName();
  int getCommandCount();
  const char* getCommand(int n);
  void dispatchCommand(const char* p, vec3_t vMin, vec3_t vMax, BOOL bSingleBrush);

  _QERTextureInfo *getTextureInfo();
  void loadTexture(LPCSTR pFilename);

  LPVOID getSurfaceFlags();

};

#endif // !defined(AFX_PLUGIN_H__B501A832_5755_11D2_B084_00AA00A410FC__INCLUDED_)
