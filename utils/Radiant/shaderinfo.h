// ShaderInfo.h: interface for the CShaderInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHADERINFO_H__93B64600_A208_11D2_803D_0020AFEB881A__INCLUDED_)
#define AFX_SHADERINFO_H__93B64600_A208_11D2_803D_0020AFEB881A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShaderInfo  
{
public:
	CString m_strName;
	CString m_strShaderName;
	CString m_strTextureName;
	CStringList m_strEditorParams;
	CStringList m_lstSurfaceParams;
  float m_fTransValue;
  int m_nFlags;
	
  
  void Parse(const char *pName);
	CShaderInfo();
	virtual ~CShaderInfo();
  void setName(char *pName);

  

};

#endif // !defined(AFX_SHADERINFO_H__93B64600_A208_11D2_803D_0020AFEB881A__INCLUDED_)
