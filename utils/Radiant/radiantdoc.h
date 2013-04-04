// RadiantDoc.h : interface of the CRadiantDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RADIANTDOC_H__330BBF0E_731C_11D1_B539_00AA00A410FC__INCLUDED_)
#define AFX_RADIANTDOC_H__330BBF0E_731C_11D1_B539_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CRadiantDoc : public CDocument
{
protected: // create from serialization only
	CRadiantDoc();
	DECLARE_DYNCREATE(CRadiantDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRadiantDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRadiantDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRadiantDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RADIANTDOC_H__330BBF0E_731C_11D1_B539_00AA00A410FC__INCLUDED_)
