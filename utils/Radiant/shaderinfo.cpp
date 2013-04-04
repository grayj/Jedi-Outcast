// ShaderInfo.cpp: implementation of the CShaderInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Radiant.h"
#include "ShaderInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShaderInfo::CShaderInfo()
{
  m_fTransValue = 1.0;
  m_nFlags = 0;
}

CShaderInfo::~CShaderInfo()
{

}

void CShaderInfo::Parse(const char *pName)
{

}

void CShaderInfo::setName(char *pName)
{
  //--char path[1024];
  //--strcpy(path, pName);
  //--DefaultExtension(path, ".tga");
  m_strName = pName;
  m_strName.MakeLower();
  if (m_strName.Find("textures") == 0)
  {
    CString s = m_strName.Right(m_strName.GetLength() - strlen("textures") - 1);
    m_strName = s;
  }
}
