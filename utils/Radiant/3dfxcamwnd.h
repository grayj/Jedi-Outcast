#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//
#define	ANGLE_SPEED	300
#define	MOVE_SPEED	400

/////////////////////////////////////////////////////////////////////////////
// C3DFXCamWnd window
class CXYWnd;

class C3DFXCamWnd : public CWnd
{
  DECLARE_DYNCREATE(C3DFXCamWnd);
// Construction
public:
	C3DFXCamWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3DFXCamWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	CXYWnd* m_pXYFriend;
	void SetXYFriend(CXYWnd* pWnd);
	virtual ~C3DFXCamWnd();
  camera_t& Camera(){return m_Camera;};
  void Cam_MouseControl(float dtime);
  void Cam_ChangeFloor(qboolean up);

protected:
  void Cam_Init();
  void Cam_BuildMatrix();
  void Cam_PositionDrag();
  void Cam_MouseDown(int x, int y, int buttons);
  void Cam_MouseUp (int x, int y, int buttons);
  void Cam_MouseMoved (int x, int y, int buttons);
  void InitCull();
  qboolean CullBrush (brush_t *b);
  void Cam_Draw();


  brush_t* m_TransBrushes[MAX_MAP_BRUSHES];
  bool     m_bTransBrushIsGhost[MAX_MAP_BRUSHES];	// is this header used? no-one here uses 3dfx. Oh well....

  int m_nNumTransBrushes;
  camera_t m_Camera;
  int	m_nCambuttonstate;
  CPoint m_ptButton;
  CPoint m_ptCursor;
  face_t* m_pSide_select;
  vec3_t m_vCull1;
  vec3_t m_vCull2;
  int m_nCullv1[3];
  int m_nCullv2[3];

  HDC m_hDC;
  HGLRC m_hRC;

	// Generated message map functions
protected:
	void OriginalMouseDown(UINT nFlags, CPoint point);
	void OriginalMouseUp(UINT nFlags, CPoint point);
	//{{AFX_MSG(C3DFXCamWnd)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

