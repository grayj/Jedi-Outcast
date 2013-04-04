//-----------------------------------------------------------------------------
// #includes follow...
//-----------------------------------------------------------------------------
//
#include "StdAfx.h"                       // For: Precompiled Headers
//
//-----------------------------------------------------------------------------
// #defines, consts, etc follow...
//-----------------------------------------------------------------------------
//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//
//-----------------------------------------------------------------------------
// Implementation follows...
//-----------------------------------------------------------------------------
//

// msvc 5.xx does not do well optimizing this code
#pragma optimize("",off)

HINSTANCE g_hQBSPDLL = NULL;

typedef void (WINAPI* PFN_QBSP)(char*, HWND, const char*);

void RunTools(char* pCommandLine, HWND hwnd, const char* pPAKFile)
{
  static PFN_QBSP lpfnQBSP = NULL;
  if (g_hQBSPDLL == NULL)
    g_hQBSPDLL = ::LoadLibrary("qbspdll.dll");
  else
  {
    ::FreeLibrary(g_hQBSPDLL);
    g_hQBSPDLL = ::LoadLibrary("qbspdll.dll");
    lpfnQBSP = NULL;
  }
  if (g_hQBSPDLL != NULL)
  {
    if (lpfnQBSP == NULL && g_hQBSPDLL != NULL)
    {
      FARPROC pFunction = ::GetProcAddress(g_hQBSPDLL, "FullProcess");
      lpfnQBSP = reinterpret_cast<PFN_QBSP>(pFunction);
    }
    if (lpfnQBSP != NULL)
    {
      (*lpfnQBSP)(pCommandLine, hwnd, pPAKFile);
    }
  }
}


#pragma optimize("",on)

