//*************************************************************
//  File name: mru.h
//
//  Description:
//
//      Header for MRU support
//
//  Development Team:
//
//      Gilles Vollant (100144.2636@compuserve.com) 
//
//*************************************************************

#ifndef __MRU_H__
#define __MRU_H__

// although I've bumped the MRU count and this is a non-afx version, for safety etc I've kept the new limit
//	to the same as the AFX one of "_AFX_MRU_MAX_COUNT" -Ste
//
#define NBMRUMENUSHOW   16 //6       // Default number of MRU showed in the menu File
#define NBMRUMENU       16 //9       // Default number of MRU stored
#define IDMRU           8000    // Default First ID of MRU
#ifdef  OFS_MAXPATHNAME
#define MAXSIZEMRUITEM  OFS_MAXPATHNAME
#else
#define MAXSIZEMRUITEM  128     // Default max size of an entry
#endif

typedef struct
{
WORD wNbItemFill;
WORD wNbLruShow;
WORD wNbLruMenu;
WORD wMaxSizeLruItem;
WORD wIdMru;
LPSTR lpMRU;
} MRUMENU;

typedef MRUMENU FAR * LPMRUMENU;

#ifdef __cplusplus
LPMRUMENU       CreateMruMenu  (WORD wNbLruShowInit=NBMRUMENUSHOW,
                                WORD wNbLruMenuInit=NBMRUMENU,
                                WORD wMaxSizeLruItemInit=MAXSIZEMRUITEM,
                                WORD wIdMruInit=IDMRU);
#else
LPMRUMENU       CreateMruMenu  (WORD wNbLruShowInit,
                                WORD wNbLruMenuInit,
                                WORD wMaxSizeLruItemInit,
                                WORD wIdMruInit);
#endif

LPMRUMENU       CreateMruMenuDefault();
void            DeleteMruMenu  (LPMRUMENU lpMruMenu);  

void            SetNbLruShow   (LPMRUMENU lpMruMenu,WORD wNbLruShowInit);
BOOL            SetMenuItem    (LPMRUMENU lpMruMenu,WORD wItem,
                                LPSTR lpItem);
BOOL            GetMenuItem    (LPMRUMENU lpMruMenu,WORD wItem,
                                BOOL fIDMBased,LPSTR lpItem,UINT uiSize);
BOOL            DelMenuItem    (LPMRUMENU lpMruMenu,WORD wItem,BOOL fIDMBased);
void            AddNewItem     (LPMRUMENU lpMruMenu,LPSTR lpItem);
void            PlaceMenuMRUItem(LPMRUMENU lpMruMenu,HMENU hMenu,UINT uiItem);

BOOL            SaveMruInIni   (LPMRUMENU lpMruMenu,LPSTR lpszSection,LPSTR lpszFile);
BOOL            LoadMruInIni   (LPMRUMENU lpMruMenu,LPSTR lpszSection,LPSTR lpszFile);
#ifdef WIN32
BOOL            SaveMruInReg   (LPMRUMENU lpMruMenu,LPSTR lpszKey);
BOOL            LoadMruInReg   (LPMRUMENU lpMruMenu,LPSTR lpszKey);

typedef enum 
{
WIN32S,
WINNT,
WIN95ORGREATHER
} WIN32KIND;
WIN32KIND GetWin32Kind();
#endif


//////////////////////////////////////////////////////////////
#endif
