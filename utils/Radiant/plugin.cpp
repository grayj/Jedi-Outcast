// PlugIn.cpp: implementation of the CPlugIn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Radiant.h"
#include "PlugIn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlugIn::CPlugIn()
{
  m_hDLL = NULL;
}

CPlugIn::~CPlugIn()
{
  if (m_hDLL != NULL)
    free();
}



bool CPlugIn::load(const char *p)
{
  m_hDLL = ::LoadLibrary(p);
  if (m_hDLL != NULL)
  {
    m_pfnInit = reinterpret_cast<PFN_QERPLUG_INIT>(::GetProcAddress(m_hDLL, QERPLUG_INIT));
    if (m_pfnInit != NULL)
    {
      m_strVersion = (*m_pfnInit)(AfxGetApp()->m_hInstance, g_pParentWnd->GetSafeHwnd());
      Sys_Printf("Loaded plugin > %s\n", m_strVersion);
      
      m_pfnGetName = reinterpret_cast<PFN_QERPLUG_GETNAME>(::GetProcAddress(m_hDLL, QERPLUG_GETNAME));
      if (m_pfnGetName != NULL)
      {
        m_strName = (*m_pfnGetName)();
      }

      m_pfnGetCommandList = reinterpret_cast<PFN_QERPLUG_GETCOMMANDLIST>(::GetProcAddress(m_hDLL, QERPLUG_GETCOMMANDLIST));
      if (m_pfnGetCommandList)
      {
        CString str = (*m_pfnGetCommandList)();
        char cTemp[1024];
        strcpy(cTemp, str);
        char* token = strtok(cTemp, ",;");
        if (token && *token == ' ')
        {
          while (*token == ' ')
            token++;
        }
        while (token != NULL)
        {
          m_CommandStrings.Add(token);
          token = strtok(NULL, ",;");
        }
      }

      m_pfnDispatch = reinterpret_cast<PFN_QERPLUG_DISPATCH>(::GetProcAddress(m_hDLL, QERPLUG_DISPATCH));
      m_pfnGetFuncTable = reinterpret_cast<PFN_QERPLUG_GETFUNCTABLE>(::GetProcAddress(m_hDLL, QERPLUG_GETFUNCTABLE));

      m_pfnGetTextureInfo = reinterpret_cast<PFN_QERPLUG_GETTEXTUREINFO>(::GetProcAddress(m_hDLL, QERPLUG_GETTEXTUREINFO));
      m_pfnLoadTexture = reinterpret_cast<PFN_QERPLUG_LOADTEXTURE>(::GetProcAddress(m_hDLL, QERPLUG_LOADTEXTURE));

      m_pfnGetSurfaceFlags = reinterpret_cast<PFN_QERPLUG_GETSURFACEFLAGS>(::GetProcAddress(m_hDLL, QERPLUG_GETSURFACEFLAGS));

      return (m_pfnDispatch != NULL && m_pfnGetFuncTable != NULL);
      //--return true;
    }
	Sys_Printf("FAILED to Load plugin > %s\n", p);
  }
  free();
  return false;
}

_QERTextureInfo* CPlugIn::getTextureInfo()
{
  if (m_pfnGetTextureInfo != NULL)
  {
    return reinterpret_cast<_QERTextureInfo*>((*m_pfnGetTextureInfo)());
  }
  return NULL;
}

void CPlugIn::loadTexture(LPCSTR pFilename)
{
  if (m_pfnLoadTexture != NULL)
  {
    (*m_pfnLoadTexture)(pFilename);
  }
}

LPVOID CPlugIn::getSurfaceFlags()
{
  if (m_pfnGetSurfaceFlags != NULL)
  {
    return reinterpret_cast<LPVOID>((*m_pfnGetSurfaceFlags)());
  }
  return NULL;
}



void CPlugIn::free()
{
  if (m_hDLL != NULL)
    ::FreeLibrary(m_hDLL);
  m_hDLL = NULL;
}

const char* CPlugIn::getVersionStr()
{
  return  m_pfnGetName();
}


const char* CPlugIn::getMenuName()
{
  return m_strName;
}


int CPlugIn::getCommandCount()
{
  return m_CommandStrings.GetSize();
}

const char* CPlugIn::getCommand(int n)
{
  return m_CommandStrings.GetAt(n);
}


void CPlugIn::dispatchCommand(const char* p, vec3_t vMin, vec3_t vMax, BOOL bSingleBrush)
{
  if (m_pfnDispatch)
  {
    (*m_pfnDispatch)(p, vMin, vMax, bSingleBrush);
  }
}

void CPlugIn::addMenuID(int n)
{
  m_CommandIDs.Add(n);
}



bool CPlugIn::ownsCommandID(int n)
{
  for (int i = 0; i < m_CommandIDs.GetSize(); i++)
  {
    if (m_CommandIDs.GetAt(i) == n)
      return true;
  }
  return false;
}

void* CPlugIn::getFuncTable()
{
  if (m_pfnGetFuncTable)
  {
    return (*m_pfnGetFuncTable)();
  }
  return NULL;
}


