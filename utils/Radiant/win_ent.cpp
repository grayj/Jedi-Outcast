#include "stdafx.h"
#include "qe3.h"
#include "entityw.h"
#include "TexWnd.h"
#include "oddbits.h"

int rgIds[EntLast] = {
	IDC_E_LIST,
	IDC_E_COMMENT,
	IDC_CHECK1,
	IDC_CHECK2,
	IDC_CHECK3,
	IDC_CHECK4,
	IDC_CHECK5,
	IDC_CHECK6,
	IDC_CHECK7,
	IDC_CHECK8,
	IDC_CHECK9,
	IDC_CHECK10,
	IDC_CHECK11,
	IDC_CHECK12,
#ifdef SOF
	IDC_CHECK13,
	IDC_CHECK14,
	IDC_CHECK15,
	IDC_CHECK16,
	IDC_CHECK17,
	IDC_CHECK18,
	IDC_CHECK19,
	IDC_CHECK20,
	IDC_CHECK21,
#endif
	IDC_E_PROPS,
	IDC_E_0,
	IDC_E_45,
	IDC_E_90,
	IDC_E_135,
	IDC_E_180,
	IDC_E_225,
	IDC_E_270,
	IDC_E_315,
	IDC_E_UP,
	IDC_E_DOWN,
	IDC_E_DELPROP,

	IDC_STATIC_KEY,
	IDC_E_KEY_FIELD,
	IDC_STATIC_VALUE,
	IDC_E_VALUE_FIELD,

	IDC_E_COLOR,

  IDC_BTN_ASSIGNSOUND, 
  IDC_BTN_ASSIGNMODEL

};

HWND hwndEnt[EntLast];

int		inspector_mode;		// W_TEXTURE, W_ENTITY, or W_CONSOLE

qboolean	multiple_entities;

entity_t	*edit_entity;


BOOL CALLBACK EntityWndProc(
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,		// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam);	// second message parameter

void SizeEntityDlg(int iWidth, int iHeight);
void AddProp();
void GetTexMods(void);


LRESULT (CALLBACK* OldFieldWindowProc) (HWND, UINT, WPARAM, LPARAM);
LRESULT (CALLBACK* OldEntityListWindowProc) (HWND, UINT, WPARAM, LPARAM);

/*
=========================
FieldWndProc

Just to handle tab and enter...
=========================
*/
BOOL CALLBACK FieldWndProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (uMsg)
    {
	case WM_CHAR:
		if (LOWORD(wParam) == VK_TAB)
			return FALSE;
		if (LOWORD(wParam) == VK_RETURN)
			return FALSE;
		if (LOWORD(wParam) == VK_ESCAPE)
		{
			SetFocus (g_qeglobals.d_hwndCamera);
			return FALSE;
		}
		break;

	case WM_KEYDOWN:
		if (LOWORD(wParam) == VK_TAB)
		{
			if (hwnd == hwndEnt[EntKeyField])
			{
				SendMessage (hwndEnt[EntValueField], WM_SETTEXT, 0, (long)"");
				SetFocus (hwndEnt[EntValueField]);
			}
			else
				SetFocus (hwndEnt[EntKeyField]);
		}
		if (LOWORD(wParam) == VK_RETURN)
		{
			if (hwnd == hwndEnt[EntKeyField])
			{
				SendMessage (hwndEnt[EntValueField], WM_SETTEXT, 0, (long)"");
				SetFocus (hwndEnt[EntValueField]);
			}
			else
			{
				AddProp ();
				SetFocus (g_qeglobals.d_hwndCamera);
			}
		}
		break;
//	case WM_NCHITTEST:
	case WM_LBUTTONDOWN:
		SetFocus (hwnd);
		break;
	}
	return CallWindowProc (OldFieldWindowProc, hwnd, uMsg, wParam, lParam);
}


/*
=========================
EntityListWndProc

Just to handle enter...
=========================
*/
BOOL CALLBACK EntityListWndProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (uMsg)
    {
	case WM_KEYDOWN:
		if (LOWORD(wParam) == VK_RETURN)
		{
			SendMessage ( g_qeglobals.d_hwndEntity, 
				          WM_COMMAND, 
			              (LBN_DBLCLK<<16) + IDC_E_LIST, 
						  0 );
			return 0;
		}
		break;
	}
	return CallWindowProc (OldEntityListWindowProc, hwnd, uMsg, wParam, lParam);
}


/*
================
GetEntityControls

Finds the controls from the dialog and
moves them to the window
================
*/
void GetEntityControls(HWND ghwndEntity)
{
	int i;

	for (i = 0; i < EntLast; i++)
	{
		if (i == EntList || i == EntProps || i == EntComment)
			continue;
		if (i == EntKeyField || i == EntValueField)
			continue;
		hwndEnt[i] = GetDlgItem(ghwndEntity, rgIds[i]);		
		if (hwndEnt[i])
    {
			SetParent (hwndEnt[i], g_qeglobals.d_hwndEntity );
			SendMessage(hwndEnt[i], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), (LPARAM)TRUE);
    }
	}


	// SetParent apears to not modify some internal state
	// on listboxes, so create it from scratch...

	hwndEnt[EntList] = CreateWindow ("listbox", NULL, 
		LBS_STANDARD | LBS_NOINTEGRALHEIGHT | LBS_WANTKEYBOARDINPUT 
		| WS_VSCROLL | WS_CHILD | WS_VISIBLE,
		5, 5, 180, 99,
		g_qeglobals.d_hwndEntity,
		(HMENU)IDC_E_LIST,
		g_qeglobals.d_hInstance,
		NULL);
	if (!hwndEnt[EntList])
		Error ("CreateWindow failed");

	hwndEnt[EntProps] = CreateWindow ("listbox", NULL, 
		LBS_STANDARD | LBS_NOINTEGRALHEIGHT | LBS_USETABSTOPS
		| WS_VSCROLL | WS_CHILD | WS_VISIBLE,
		5, 100, 180, 99,
		g_qeglobals.d_hwndEntity,
		(HMENU)IDC_E_PROPS,
		g_qeglobals.d_hInstance,
		NULL);
	if (!hwndEnt[EntProps])
		Error ("CreateWindow failed");

	hwndEnt[EntComment] = CreateWindow ("edit", NULL, 
		ES_MULTILINE | ES_READONLY | WS_VSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER,
		5, 100, 180, 99,
		g_qeglobals.d_hwndEntity,
		(HMENU)IDC_E_COMMENT,
		g_qeglobals.d_hInstance,
		NULL);
	if (!hwndEnt[EntComment])
		Error ("CreateWindow failed");

	hwndEnt[EntKeyField] = CreateWindow ("edit", NULL, 
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		5, 100, 180, 99,
		g_qeglobals.d_hwndEntity,
		(HMENU)IDC_E_KEY_FIELD,
		g_qeglobals.d_hInstance,
		NULL);
	if (!hwndEnt[EntKeyField])
		Error ("CreateWindow failed");

	hwndEnt[EntValueField] = CreateWindow ("edit", NULL, 
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		5, 100, 180, 99,
		g_qeglobals.d_hwndEntity,
		(HMENU)IDC_E_VALUE_FIELD,
		g_qeglobals.d_hInstance,
		NULL);
	if (!hwndEnt[EntValueField])
		Error ("CreateWindow failed");


  if (g_pParentWnd->CurrentStyle() > 0 && g_pParentWnd->CurrentStyle() < 3)
  {
	  g_qeglobals.d_hwndEdit = CreateWindow ("edit", NULL, ES_MULTILINE | ES_READONLY | WS_VSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER,
		                                       5, 100, 180, 99, g_qeglobals.d_hwndEntity, (HMENU)IDC_E_STATUS,
		                                       g_qeglobals.d_hInstance, NULL);
	  if (!g_qeglobals.d_hwndEdit)
		  Error ("CreateWindow failed");
  }

	SendMessage(hwndEnt[EntList], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), (LPARAM)TRUE);
	SendMessage(hwndEnt[EntProps], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), (LPARAM)TRUE);
	SendMessage(hwndEnt[EntComment], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), (LPARAM)TRUE);
	SendMessage(hwndEnt[EntKeyField], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), (LPARAM)TRUE);
	SendMessage(hwndEnt[EntValueField], WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), (LPARAM)TRUE);

	if (g_pParentWnd->CurrentStyle() > 0 && g_pParentWnd->CurrentStyle() < 3)
		SendMessage(g_qeglobals.d_hwndEdit, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), (LPARAM)TRUE);
}



/*
===============================================================

ENTITY WINDOW

===============================================================
*/


void FillClassList (void)
{
	eclass_t	*pec;
	int			iIndex;

	SendMessage(hwndEnt[EntList], LB_RESETCONTENT, 0 , 0);

	for (pec = eclass ; pec ; pec = pec->next)
	{
		iIndex = SendMessage(hwndEnt[EntList], LB_ADDSTRING, 0 , (LPARAM)pec->name);
		SendMessage(hwndEnt[EntList], LB_SETITEMDATA, iIndex, (LPARAM)pec);
	}	

}


/*
==============
WEnt_Create
==============
*/
void WEnt_Create (HINSTANCE hInstance)
{
    WNDCLASS   wc;

    /* Register the camera class */
	memset (&wc, 0, sizeof(wc));

    wc.style         = CS_NOCLOSE | CS_OWNDC;
    wc.lpfnWndProc   = (WNDPROC)EntityWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = 0;
    wc.hCursor       = LoadCursor (NULL,IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject (LTGRAY_BRUSH);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = ENT_WINDOW_CLASS;

    RegisterClass (&wc);

    int nStyle = (g_pParentWnd->CurrentStyle() == QR_QE4) ? QE3_STYLE : QE3_STYLE2;
	  g_qeglobals.d_hwndEntity = CreateWindow (ENT_WINDOW_CLASS ,
		"Entity",
		nStyle,
		20,
		20,
		100,
		480,	// size

		g_qeglobals.d_hwndMain,	// parent
		0,		// no menu
		hInstance,
		NULL);

	if (!g_qeglobals.d_hwndEntity )
		Error ("Couldn't create Entity window");
}

/*
==============
CreateEntityWindow
==============
*/
BOOL CreateEntityWindow(HINSTANCE hInstance)
{
	HWND hwndEntityPalette;

	inspector_mode = W_ENTITY;

	WEnt_Create (hInstance);

	hwndEntityPalette = CreateDialog(hInstance, (char *)IDD_ENTITY, g_qeglobals.d_hwndMain, (DLGPROC)NULL);
	if (!hwndEntityPalette)
		Error ("CreateDialog failed");

	GetEntityControls (hwndEntityPalette);
	DestroyWindow (hwndEntityPalette);

	OldFieldWindowProc = (WNDPROC)GetWindowLong (hwndEnt[EntKeyField], GWL_WNDPROC);
	SetWindowLong (hwndEnt[EntKeyField], GWL_WNDPROC, (long)FieldWndProc);
	SetWindowLong (hwndEnt[EntValueField], GWL_WNDPROC, (long)FieldWndProc);

	OldEntityListWindowProc = (WNDPROC)GetWindowLong (hwndEnt[EntList], GWL_WNDPROC);
	SetWindowLong (hwndEnt[EntList], GWL_WNDPROC, (long)EntityListWndProc);

	FillClassList ();


	LoadWindowPlacement(g_qeglobals.d_hwndEntity, "EntityWindowPlace");
	ShowWindow (g_qeglobals.d_hwndEntity, SW_HIDE);
	SetInspectorMode (W_CONSOLE);

	return TRUE;
}

/*
==============
SetInspectorMode
==============
*/
void SetInspectorMode(int iType)
{
	RECT rc;
	HMENU hMenu = GetMenu( g_qeglobals.d_hwndMain );

  if ((g_pParentWnd->CurrentStyle() == QR_SPLIT || g_pParentWnd->CurrentStyle() == QR_SPLITZ) && (iType == W_TEXTURE || iType == W_CONSOLE))
    return;


	// Is the caller asking us to cycle to the next window?

	if (iType == -1)
	{
		if (inspector_mode == W_ENTITY)
			iType = W_TEXTURE;
		else if (inspector_mode == W_TEXTURE)
			iType = W_CONSOLE;
		else
			iType = W_ENTITY;
	}		

	inspector_mode = iType;
	switch(iType)
	{
		
	case W_ENTITY:
		SetWindowText(g_qeglobals.d_hwndEntity, "Entity");
		EnableMenuItem( hMenu, ID_MISC_SELECTENTITYCOLOR, MF_ENABLED | MF_BYCOMMAND );
		break;

	case W_TEXTURE:
    SetWindowText(g_qeglobals.d_hwndEntity, "Textures");
    g_pParentWnd->GetTexWnd()->FocusEdit();
		EnableMenuItem( hMenu, ID_MISC_SELECTENTITYCOLOR, MF_GRAYED | MF_DISABLED | MF_BYCOMMAND );
		break;

	case W_CONSOLE:
		SetWindowText(g_qeglobals.d_hwndEntity, "Console");		
		EnableMenuItem( hMenu, ID_MISC_SELECTENTITYCOLOR, MF_GRAYED | MF_DISABLED | MF_BYCOMMAND );
		break;

	default:
		break;
	}

	GetWindowRect (g_qeglobals.d_hwndEntity, &rc);
	SizeEntityDlg( rc.right - rc.left - 8, rc.bottom - rc.top - 32);


//	InvalidateRect(entwindow, NULL, true);
//	ShowWindow (entwindow, SW_SHOW);
//	UpdateWindow (entwindow);

  HWND hFlag = (g_pParentWnd->CurrentStyle() == QR_QE4) ? HWND_TOP : HWND_TOPMOST;
	SetWindowPos( g_qeglobals.d_hwndEntity, hFlag, rc.left, rc.top,  rc.right - rc.left, rc.bottom - rc.top, SWP_NOSIZE | SWP_NOMOVE );
	RedrawWindow (g_qeglobals.d_hwndEntity, NULL, NULL, RDW_ERASE | RDW_INVALIDATE| RDW_ERASENOW | RDW_UPDATENOW | RDW_ALLCHILDREN);
}





// SetKeyValuePairs
//
// Reset the key/value (aka property) listbox and fill it with the 
// k/v pairs from the entity being edited.
//

void SetKeyValuePairs (bool bClearMD3)
{
	epair_t	*pep;
	RECT	rc;
	char	sz[4096];

	if (edit_entity == NULL)
		return;

	// set key/value pair list

	GetWindowRect(hwndEnt[EntProps], &rc);
	SendMessage(hwndEnt[EntProps], LB_SETCOLUMNWIDTH, (rc.right - rc.left)/2, 0);
	SendMessage(hwndEnt[EntProps], LB_RESETCONTENT, 0, 0);

	// Walk through list and add pairs

	for (pep = edit_entity->epairs ; pep ; pep = pep->next)
	{
		// if the key is less than 8 chars, add a tab for alignment
		if (strlen(pep->key) > 8)
			sprintf (sz, "%s\t%s", pep->key, pep->value);
		else
			sprintf (sz, "%s\t\t%s", pep->key, pep->value);
		SendMessage(hwndEnt[EntProps], LB_ADDSTRING, 0, (LPARAM)sz);
	}

  if (strnicmp(edit_entity->eclass->name, "misc_model",10) == 0)
  {
    // if this is a misc_model
    // cache the md3 for display later
    if (bClearMD3)
    {
      edit_entity->md3Class = NULL;
    }
    //char *pModel = ValueForKey(edit_entity, "model");

/*
    if (pModel != NULL)
    {
      GetCachedModel(pModel, vMin, vMax);
    }
*/
  }
  Sys_UpdateWindows(W_CAMERA | W_XY);

}

// SetSpawnFlags
// 
// Update the checkboxes to reflect the flag state of the entity
//
void SetSpawnFlags(void)
{
	int		f;
	int		i;
	int		v;

	f = atoi(ValueForKey (edit_entity, "spawnflags"));
	for (i=0 ; i<MAX_FLAGS; i++)
	{
		v = !!(f&(1<<i));
		SendMessage(hwndEnt[EntCheck1+i], BM_SETCHECK, v, 0);
	}
}


// GetSpawnFlags
// 
// Update the entity flags to reflect the state of the checkboxes
//
void GetSpawnFlags(void)
{
	int		f;
	int		i, v;
	char	sz[32];

	f = 0;
	for (i=0 ; i<MAX_FLAGS ; i++)
	{
		v = SendMessage(hwndEnt[EntCheck1+i], BM_GETCHECK, 0, 0);
		f |= v<<i;
	}

	sprintf (sz, "%i", f);

	if (multiple_entities)
	{
		brush_t	*b;

		for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
			SetKeyValue(b->owner, "spawnflags", sz);
	}
	else
		SetKeyValue (edit_entity, "spawnflags", sz);
	SetKeyValuePairs ();
}

// UpdateSel
//
// Update the listbox, checkboxes and k/v pairs to reflect the new selection
//

BOOL UpdateSel(int iIndex, eclass_t *pec)
{
	int		i;
	brush_t	*b;

	if (selected_brushes.next == &selected_brushes)
	{
		edit_entity = world_entity;
		multiple_entities = false;
	}
	else
	{
		edit_entity = selected_brushes.next->owner;
		for (b=selected_brushes.next->next ; b != &selected_brushes ; b=b->next)
		{
			if (b->owner != edit_entity)
			{
				multiple_entities = true;
				break;
			}
		}
	}

	if (iIndex != LB_ERR)
		SendMessage(hwndEnt[EntList], LB_SETCURSEL, iIndex, 0);

	if (pec == NULL)
		return TRUE;

	// Set up the description

	SendMessage(hwndEnt[EntComment], WM_SETTEXT, 0, 
			(LPARAM)TranslateString(pec->comments));

	for (i=0 ; i<MAX_FLAGS ; i++)
	{
		HWND hwnd = hwndEnt[EntCheck1+i];
		if (pec->flagnames[i] && pec->flagnames[i][0] != 0)
		{
			EnableWindow(hwnd, TRUE);
			SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)pec->flagnames[i]);
		} else {

			// disable check box
			SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)" ");
			EnableWindow(hwnd, FALSE);
		}
	}

	SetSpawnFlags();
	SetKeyValuePairs();
	return TRUE;
}

BOOL UpdateEntitySel(eclass_t *pec)
{
	int iIndex;
	
	iIndex = (int)SendMessage(hwndEnt[EntList], LB_FINDSTRINGEXACT, 
			(WPARAM)-1, (LPARAM)pec->name);

	return UpdateSel(iIndex, pec);
}

// CreateEntity
//
// Creates a new entity based on the currently selected brush and entity type.
//

void CreateEntity(void)
{
	eclass_t *pecNew;
	entity_t *petNew;
	int i;
	HWND hwnd;
	char sz[1024];

	// check to make sure we have a brush

	if (selected_brushes.next == &selected_brushes)
	{
	    MessageBox(g_qeglobals.d_hwndMain, "You must have a selected brush to create an entity"
			, "info", 0);
		return;
	}


	// find out what type of entity we are trying to create

	hwnd = hwndEnt[EntList];

	i = SendMessage(hwndEnt[EntList], LB_GETCURSEL, 0, 0);

	if (i < 0)
	{
	    MessageBox(g_qeglobals.d_hwndMain, "You must have a selected class to create an entity"
			, "info", 0);
		return;
	}

	SendMessage(hwnd, LB_GETTEXT, i, (LPARAM)sz);

	if (!stricmp(sz, "worldspawn"))
	{
	    MessageBox(g_qeglobals.d_hwndMain, "Can't create an entity with worldspawn.", "info", 0);
		return;
	}

	pecNew = Eclass_ForName(sz, false);

	// create it

	petNew = Entity_Create(pecNew);

	if (petNew == NULL)
	{
	    MessageBox(g_qeglobals.d_hwndMain, "Failed to create entity.", "info", 0);
		return;
	}

	if (selected_brushes.next == &selected_brushes)
		edit_entity = world_entity;
	else
		edit_entity = selected_brushes.next->owner;

	SetKeyValuePairs();
	Select_Deselect ();
	Select_Brush (edit_entity->brushes.onext);
  Sys_UpdateWindows(W_ALL);

}



/*
===============
AddProp

===============
*/
void AddProp()
{
	char	key[4096];
	char	value[4096];

	if (edit_entity == NULL)
		return;

	// Get current selection text

	SendMessage(hwndEnt[EntKeyField], WM_GETTEXT, sizeof(key)-1, (LPARAM)key);	
	SendMessage(hwndEnt[EntValueField], WM_GETTEXT, sizeof(value)-1, (LPARAM)value);	

	if (multiple_entities)
	{
		brush_t	*b;

		for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
			SetKeyValue(b->owner, key, value);
	}
	else
		SetKeyValue(edit_entity, key, value);

	// refresh the prop listbox

	SetKeyValuePairs();	
}

/*
===============
DelProp

===============
*/
void DelProp(void)
{
	char	sz[4096];

	if (edit_entity == NULL)
		return;

	// Get current selection text

	SendMessage(hwndEnt[EntKeyField], WM_GETTEXT, sizeof(sz)-1, (LPARAM)sz);	

	if (multiple_entities)
	{
		brush_t	*b;

		for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
			DeleteKey(b->owner, sz);
	}
	else
		DeleteKey(edit_entity, sz);

	// refresh the prop listbox

	SetKeyValuePairs();	
}

/*
===============
EditProp

===============
*/
void EditProp(void)
{
	int i;
	HWND hwnd;
	char	sz[4096];
	char	*val;

	if (edit_entity == NULL)
		return;

	hwnd = hwndEnt[EntProps];

	// Get current selection text

	i = SendMessage(hwnd, LB_GETCURSEL, 0, 0);	

	if (i < 0)
		return;

	SendMessage(hwnd, LB_GETTEXT, i, (LPARAM)sz);	

	// strip it down to the key name

	for(i=0;sz[i] != '\t';i++)
	;

	sz[i] = '\0';

	val = sz + i + 1;
	if (*val == '\t')
		val++;

	SendMessage(hwndEnt[EntKeyField], WM_SETTEXT, 0, (LPARAM)sz);	
	SendMessage(hwndEnt[EntValueField], WM_SETTEXT, 0, (LPARAM)val);	
}


HDWP	defer;
int		col;
void MOVE(HWND e, int x, int y, int w, int h)
{
//	defer=DeferWindowPos(defer,e,HWND_TOP,col+(x),y,w,h,SWP_SHOWWINDOW);
//	MoveWindow (e, col+x, y, w, h, FALSE);
	SetWindowPos (e, HWND_TOP, col+x, y, w, h, 
		SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOZORDER);
}


/*
===============
SizeEnitityDlg

Positions all controls so that the active inspector
is displayed correctly and the inactive ones are
off the side
===============
*/
void SizeEntityDlg(int iWidth, int iHeight)
{
	int y, x, xCheck, yCheck;
	int i, iRow;
	int	w, h;

	if (iWidth < 32 || iHeight < 32)
		return;

	SendMessage( g_qeglobals.d_hwndEntity, WM_SETREDRAW, 0, 0);

	//==========================================

	//
	// console
	//

	if (inspector_mode == W_CONSOLE)
		col = 0;
	else
		col = iWidth;

  if (g_pParentWnd->CurrentStyle() > 0 && g_pParentWnd->CurrentStyle() < 3)
	  MOVE(g_qeglobals.d_hwndEdit,	DlgXBorder, DlgYBorder, iWidth - (2 * DlgXBorder), iHeight - (2 * DlgYBorder) );

	//==========================================

	//
	// texture controls
	//
	if (inspector_mode == W_TEXTURE)
		col = 0;
	else
		col = iWidth;

  if (g_pParentWnd->CurrentStyle() > 0 && g_pParentWnd->CurrentStyle() < 3)
	  MOVE(g_qeglobals.d_hwndTexture,	DlgXBorder, DlgYBorder, iWidth - (2 * DlgXBorder), iHeight - (2 * DlgYBorder) );

	//==========================================

	//
	// entity controls
	//
	if (inspector_mode == W_ENTITY)
		col = 0;
	else
		col = iWidth;


	// top half includes the entity list (2/3) and the 
	// comments (1/3) - 2 gaps, above and below.

	y = iHeight/2;
	y -= 2 * DlgYBorder;
	y = y / 3;
	w = iWidth - (2 * DlgXBorder);
	MOVE(hwndEnt[EntList], DlgXBorder, DlgYBorder, w, 2 * y);
	
	MOVE(hwndEnt[EntComment], DlgXBorder, 2 * DlgYBorder + 2 * y, w, y - (2 * DlgYBorder));

	// bottom half includes flags (fixed), k/v pairs,
	// and buttons (fixed).

	// xCheck = width of a single check box
	// yCheck = distance from top of one check to the next

	xCheck = (iWidth - (2 * DlgXBorder)) / 3;
	yCheck = 20;

	x = DlgXBorder;

	for (iRow = 0; iRow <= MAX_FLAGS; iRow += COLUMN_SIZE)
	{
		y = iHeight/2;
	
		for (i = 0; i < COLUMN_SIZE; i++)
		{
			MOVE(hwndEnt[EntCheck1 + i + iRow],
				x, y, xCheck, yCheck);
			y += yCheck;
		}

		x += xCheck;
	}

	//
	// properties scroll box
	//
	y = iHeight/2 + COLUMN_SIZE * yCheck;

	w = iWidth - (2 * DlgXBorder);
	h = (iHeight - (yCheck * 5 + 2 * DlgYBorder) ) - y;

	MOVE(hwndEnt[EntProps], DlgXBorder, y, w, h);

	y += h + DlgYBorder;
	
	//
	// key / value fields
	//
	w = iWidth-(DlgXBorder+45);
	MOVE(hwndEnt[EntKeyLabel], DlgXBorder, y, 40, yCheck);
	MOVE(hwndEnt[EntKeyField], DlgXBorder+40, y, w, yCheck);
	y += yCheck;		

	MOVE(hwndEnt[EntValueLabel], DlgXBorder, y, 40, yCheck);
	MOVE(hwndEnt[EntValueField], DlgXBorder+40, y, w, yCheck);
	y += yCheck;		

	//
	// angle check boxes
	//
  y += 2;
	i = y;
	x = DlgXBorder;

	xCheck = yCheck*2;

	MOVE(hwndEnt[EntDir135], x, y, xCheck, yCheck);
	y += yCheck;		
	
	MOVE(hwndEnt[EntDir180], x, y, xCheck, yCheck);
	y += yCheck;		
	
	MOVE(hwndEnt[EntDir225], x, y, xCheck, yCheck);

	y = i;
	x += xCheck;

	
	MOVE(hwndEnt[EntDir90], x, y, xCheck, yCheck);
	y += yCheck;		
	y += yCheck;		
	
	MOVE(hwndEnt[EntDir270], x, y, xCheck, yCheck);

	y = i;
	x += xCheck;

	
	MOVE(hwndEnt[EntDir45], x, y, xCheck, yCheck);
	y += yCheck;		
	
	MOVE(hwndEnt[EntDir0], x, y, xCheck, yCheck);
	y += yCheck;		
	
	MOVE(hwndEnt[EntDir315], x, y, xCheck, yCheck);

	y = i + yCheck/2;
	x += xCheck + xCheck/2;

	
	MOVE(hwndEnt[EntDirUp], x, y, xCheck, yCheck);
	y += yCheck;		
	
	MOVE(hwndEnt[EntDirDown], x, y, xCheck, yCheck);

	y = i;
	x += 1.5 * xCheck;
	
	MOVE(hwndEnt[EntDelProp], x, y, xCheck*2, yCheck);
	y += yCheck + 4;		

  MOVE(hwndEnt[EntAssignSounds], x, y, xCheck*2, yCheck);
	y += yCheck;		
  MOVE(hwndEnt[EntAssignModels], x, y, xCheck*2, yCheck);

	SendMessage( g_qeglobals.d_hwndEntity, WM_SETREDRAW, 1, 0);
//	InvalidateRect(entwindow, NULL, TRUE);
}

void AssignSound()
{
  CString strBasePath = ValueForKey(g_qeglobals.d_project_entity, "basepath");
  AddSlash(strBasePath);
  CString strPath = strBasePath;
  strPath += "sound\\";

  CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Sound files (*.wav,*.mp3)|*.wav;*.mp3||", g_pParentWnd);
  dlgFile.m_ofn.lpstrInitialDir = strPath;
  if (dlgFile.DoModal() == IDOK)
  {
	  SendMessage(hwndEnt[EntKeyField], WM_SETTEXT, 0, (LPARAM)"noise");	
    CString str = dlgFile.GetPathName().GetBuffer(0);
    str.MakeLower();
    strBasePath.MakeLower();
    QE_ConvertDOSToUnixName(str.GetBuffer(0), str.GetBuffer(0));
    QE_ConvertDOSToUnixName(strBasePath.GetBuffer(0), strBasePath.GetBuffer(0));
    int n = str.Find(strBasePath);
    if (n == 0)
    {
      str = str.Right(str.GetLength() - strBasePath.GetLength());
    }

	  SendMessage(hwndEnt[EntValueField], WM_SETTEXT, 0, (LPARAM)str.GetBuffer(0));	
    AddProp();
    g_pParentWnd->GetXYWnd()->SetFocus();
  }
}

#ifdef QUAKE3
CString strASSIGNMODEL;
#endif

void AssignModel()
{
#ifdef QUAKE3
	if (strASSIGNMODEL.GetLength())
	{
		SendMessage(hwndEnt[EntKeyField],	WM_SETTEXT, 0, (LPARAM)"model");
		SendMessage(hwndEnt[EntValueField], WM_SETTEXT, 0, (LPARAM) ((LPCSTR)strASSIGNMODEL));//.GetBuffer(0));	
		strASSIGNMODEL="";		
		gEntityToSetBoundsOf = edit_entity;
		AddProp();
		edit_entity->md3Class = NULL;
		g_pParentWnd->GetXYWnd()->SetFocus();

//-----------
#if 1
		// brush is currently built at 0 angle, now fake a rotate away and back to fill inthe mins/maxs fields...
		//
		SetKeyValue(edit_entity, "angle", "0",	false);	// tell it that it has no angle, and don't rebuild
		SetFocus (g_qeglobals.d_hwndCamera);
		SetKeyValuePairs ();

		SetKeyValue(edit_entity, "angle", "90",	true);	// true = rebuild brush, rotate to some other angle
		SetFocus (g_qeglobals.d_hwndCamera);
		SetKeyValuePairs ();


		SetKeyValue(edit_entity, "angle", "0",	true);	// true = rebuild brush, rotate back to 0
		SetFocus (g_qeglobals.d_hwndCamera);
		SetKeyValuePairs ();
#endif
//-----------
	}
	else	
#endif
	{
	  CString strBasePath = ValueForKey(g_qeglobals.d_project_entity, "basepath");
	  AddSlash(strBasePath);
	  CString strPath = strBasePath;
	  strPath += "models\\mapobjects\\";

	  CFileDialog dlgFile(TRUE, NULL, NULL, OFN_OVERWRITEPROMPT, "Model files (*.md3)|*.md3||", g_pParentWnd);
	  dlgFile.m_ofn.lpstrInitialDir = strPath;
	  if (dlgFile.DoModal() == IDOK)
	  {
		  SendMessage(hwndEnt[EntKeyField], WM_SETTEXT, 0, (LPARAM)"model");
		CString str = dlgFile.GetPathName().GetBuffer(0);
		str.MakeLower();
		strBasePath.MakeLower();
		QE_ConvertDOSToUnixName(str.GetBuffer(0), str.GetBuffer(0));
		QE_ConvertDOSToUnixName(strBasePath.GetBuffer(0), strBasePath.GetBuffer(0));
		int n = str.Find(strBasePath);
		if (n == 0)
		{
		  str = str.Right(str.GetLength() - strBasePath.GetLength());
		}

		SendMessage(hwndEnt[EntValueField], WM_SETTEXT, 0, (LPARAM)str.GetBuffer(0));	
		gEntityToSetBoundsOf = edit_entity;
		AddProp();
		edit_entity->md3Class = NULL;
		g_pParentWnd->GetXYWnd()->SetFocus();
	  }
	}
}


/*
=========================
EntityWndProc
=========================
*/
BOOL CALLBACK EntityWndProc(
    HWND hwndDlg,	// handle to dialog box
    UINT uMsg,		// message
    WPARAM wParam,	// first message parameter
    LPARAM lParam)	// second message parameter
{
	RECT	rc;

	GetClientRect(hwndDlg, &rc);

    switch (uMsg)
    {

      case WM_CHAR :
      {
        char c = toupper(LOWORD(wParam));
        if (c == 'N')
          g_pParentWnd->PostMessage(WM_COMMAND, ID_VIEW_ENTITY, 0);
        else
        if (c == 'O')
          g_pParentWnd->PostMessage(WM_COMMAND, ID_VIEW_CONSOLE, 0);
        else
        if (c == 'T')
          g_pParentWnd->PostMessage(WM_COMMAND, ID_VIEW_TEXTURE, 0);
        else
			    DefWindowProc (hwndDlg, uMsg, wParam, lParam);
        break;
      }

	case WM_SIZE:

			DefWindowProc (hwndDlg, uMsg, wParam, lParam);
      break;

  case WM_DESTROY:
      SaveWindowPlacement(g_qeglobals.d_hwndEntity, "EntityWindowPlace");
      DefWindowProc(hwndDlg, uMsg, wParam, lParam);
      break;

	case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO	lpmmi;

			lpmmi = (LPMINMAXINFO) lParam;
			lpmmi->ptMinTrackSize.x = 320;
			lpmmi->ptMinTrackSize.y = 700;
		}
		return 0;

	case WM_WINDOWPOSCHANGING:
		{
			LPWINDOWPOS	lpwp;
			lpwp = (LPWINDOWPOS) lParam;

			DefWindowProc (hwndDlg, uMsg, wParam, lParam);

			lpwp->flags |= SWP_NOCOPYBITS;
			SizeEntityDlg(lpwp->cx-8, lpwp->cy-32);
			return 0;

		}
		return 0;


	case WM_COMMAND: 
		switch (LOWORD(wParam)) { 

    case IDC_BTN_ASSIGNSOUND:
      AssignSound();
      break;

    case IDC_BTN_ASSIGNMODEL:
      AssignModel();
      break;

		case IDC_E_DELPROP:
			DelProp();
			SetFocus (g_qeglobals.d_hwndCamera);
			break;

		case IDC_E_0:
			SetKeyValue (edit_entity, "angle", "360");
			SetFocus (g_qeglobals.d_hwndCamera);
			SetKeyValuePairs ();
			break;
		case IDC_E_45:
			SetKeyValue (edit_entity, "angle", "45");
			SetFocus (g_qeglobals.d_hwndCamera);
			SetKeyValuePairs ();
			break;
		case IDC_E_90:
			SetKeyValue (edit_entity, "angle", "90");
			SetFocus (g_qeglobals.d_hwndCamera);
			SetKeyValuePairs ();
			break;
		case IDC_E_135:
			SetKeyValue (edit_entity, "angle", "135");
			SetFocus (g_qeglobals.d_hwndCamera);
			SetKeyValuePairs ();
			break;
		case IDC_E_180:
			SetKeyValue (edit_entity, "angle", "180");
			SetFocus (g_qeglobals.d_hwndCamera);
			SetKeyValuePairs ();
			break;
		case IDC_E_225:
			SetKeyValue (edit_entity, "angle", "225");
			SetFocus (g_qeglobals.d_hwndCamera);
			SetKeyValuePairs ();
			break;
		case IDC_E_270:
			SetKeyValue (edit_entity, "angle", "270");
			SetFocus (g_qeglobals.d_hwndCamera);
			SetKeyValuePairs ();
			break;
		case IDC_E_315:
			SetKeyValue (edit_entity, "angle", "315");
			SetFocus (g_qeglobals.d_hwndCamera);
			SetKeyValuePairs ();
			break;
		case IDC_E_UP:
			SetKeyValue (edit_entity, "angle", "-1");
			SetFocus (g_qeglobals.d_hwndCamera);
			SetKeyValuePairs ();
			break;
		case IDC_E_DOWN:
			SetKeyValue (edit_entity, "angle", "-2");
			SetFocus (g_qeglobals.d_hwndCamera);
			SetKeyValuePairs ();
			break;

    case IDC_BTN_HIDE:
      ::PostMessage(g_qeglobals.d_hwndMain, WM_COMMAND, ID_VIEW_CAMERATOGGLE, 0);
      break;

		case IDC_CHECK1:
		case IDC_CHECK2:
		case IDC_CHECK3:
		case IDC_CHECK4:
		case IDC_CHECK5:
		case IDC_CHECK6:
		case IDC_CHECK7:
		case IDC_CHECK8:
		case IDC_CHECK9:
		case IDC_CHECK10:
		case IDC_CHECK11:
		case IDC_CHECK12:
		case IDC_CHECK13:
		case IDC_CHECK14:
		case IDC_CHECK15:
		case IDC_CHECK16:
		case IDC_CHECK17:
		case IDC_CHECK18:
		case IDC_CHECK19:
		case IDC_CHECK20:
		case IDC_CHECK21:
			GetSpawnFlags();
			SetFocus (g_qeglobals.d_hwndCamera);
			break;


		case IDC_E_PROPS: 
			switch (HIWORD(wParam))
			{ 
			case LBN_SELCHANGE:

				EditProp();
				return TRUE; 
			}
			break;

		case IDC_E_LIST: 
	   
			switch (HIWORD(wParam)) { 
			
			case LBN_SELCHANGE: 
			{
				int iIndex;
				eclass_t *pec;
				
				iIndex = SendMessage(hwndEnt[EntList], LB_GETCURSEL, 0, 0);	
				pec = (eclass_t *)SendMessage(hwndEnt[EntList], LB_GETITEMDATA, 
						iIndex, 0); 
			
				UpdateSel(iIndex, pec);

				return TRUE; 
				break; 
			}

			case LBN_DBLCLK: 
				CreateEntity ();
				SetFocus (g_qeglobals.d_hwndCamera);
				break; 
			} 
            break; 


            default: 
              return DefWindowProc( hwndDlg, uMsg, wParam, lParam ); 
        } 

		return 0;
	}

    return DefWindowProc (hwndDlg, uMsg, wParam, lParam);
}
