// TexWnd.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "Radiant.h"
#include "TexWnd.h"
#include "qe3.h"
#include "io.h"
#include "inc.h"
#include "PrefsDlg.h"
#include "shaderinfo.h"
#include "oddbits.h"
#include "..\libs\pakstuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TYP_MIPTEX	68
static unsigned	tex_palette[256];

qtexture_t	*notexture = NULL;
qtexture_t	*g_pluginTexture = NULL;

static qboolean	nomips = false;

#define	FONT_HEIGHT	10

HGLRC s_hglrcTexture = NULL;
HDC	 s_hdcTexture = NULL;

//int		texture_mode = GL_NEAREST;
//int		texture_mode = GL_NEAREST_MIPMAP_NEAREST;
//int		texture_mode = GL_NEAREST_MIPMAP_LINEAR;
//int		texture_mode = GL_LINEAR;
//int		texture_mode = GL_LINEAR_MIPMAP_NEAREST;
int		texture_mode = GL_LINEAR_MIPMAP_LINEAR;

int		texture_extension_number = 1;
int g_nCurrentTextureMenuName;

int g_nTextureOffset = 0;

// current active texture directory.  if empty, show textures in use
char		texture_directory[128];	// use if texture_showinuse is false
qboolean	texture_showinuse;

bool g_bFilterEnabled = false;
CString g_strFilter;

// texture layout functions
qtexture_t	*current_texture = NULL;
int			current_x, current_y, current_row;

int			texture_nummenus;
#define		MAX_TEXTUREDIRS	128
char		texture_menunames[MAX_TEXTUREDIRS][128];

qboolean	g_dontuse = true;		// set to true to load the texture but not flag as used

void SelectTexture (int mx, int my, bool bShift, bool bFitScale = false, bool bNoSystemTextureOverwrite = false);

void	Texture_MouseDown (int x, int y, int buttons);
void	Texture_MouseUp (int x, int y, int buttons);
void	Texture_MouseMoved (int x, int y, int buttons);

CPtrArray g_lstShaders;
CPtrArray g_lstDeferred;

struct DeferredShaderLoad
{
  CString m_strImage;
  CString m_strShader;
  CString m_strPath;

  DeferredShaderLoad(const char *pImage, const char *pShader, const char *pPath)
  {
    m_strImage = pImage;
    m_strShader = pShader;
    m_strPath = pPath;
    m_strPath.MakeLower();
  }

};

void loadDeferredByName(const char *pName)
{
}

bool DeferredExists(const char *pName)
{
  for (qtexture_t *q=g_qeglobals.d_qtextures ; q ; q=q->next)
  {
    if (!strcmp(q->name,  pName))
    {
      return true;
    }
  }
  return false;

}

bool ShaderQTextureExists(const char *pName)
{
  for (qtexture_t *q=g_qeglobals.d_qtextures ; q ; q=q->next)
  {
    if (!strcmp(q->name,  pName))
    {
      return true;
    }
  }
  return false;

}



void LoadDeferred(const char *pPath)
{
  int nSize = g_lstDeferred.GetSize();
  for (int i = 0; i < nSize; i++)
  {
    DeferredShaderLoad *p = reinterpret_cast<DeferredShaderLoad*>(g_lstDeferred.ElementAt(i));
    if (p != NULL)
    {
      if (p->m_strPath.CompareNoCase(pPath) == 0)
      {
        if (DeferredExists(p->m_strShader) == false)
        {
          qtexture_t *q = Texture_ForName(p->m_strImage.GetBuffer(0));
          if (q != NULL)
          {
            strcpy(q->name, p->m_strShader);
          }
        }
      }
    }
  }
}


CShaderInfo* hasShader(const char *pName)
{
  int nSize = g_lstShaders.GetSize();
  for (int i = 0; i < nSize; i++)
  {
    CShaderInfo *pInfo = reinterpret_cast<CShaderInfo*>(g_lstShaders.ElementAt(i));
    if (pInfo != NULL)
    {
      if (pInfo->m_strName.CompareNoCase(pName) == 0)
      {
        return pInfo;
      }
    }
  }
  return NULL;
}


/*
struct ShaderInfo
{
  ShaderInfo(){ };
  ~ShaderInfo() { };
  
  CString m_strName;
  CStringList m_strEditorParams;
  CStringList m_strSurfaceParams;

};
*/

// gets active texture extension
// 
// FIXME: fix this to be generic from project file
//
int GetTextureExtensionCount()
{
  return 2;
}

const char* GetTextureExtension(int nIndex)
{
  if ( nIndex == 0)
  {
  _QERTextureInfo *pInfo = g_pParentWnd->GetPlugInMgr().GetTextureInfo();
  const char *pTex = (pInfo != NULL) ? pInfo->m_TextureExtension : NULL;
  return (pTex == NULL) ? (g_PrefsDlg.m_bHiColorTextures == FALSE) ? "wal" : "tga" : pTex;
  }
  // return jpg for 2nd extension
  return "jpg";
}



/*
=================
CheckTexturePrefix

Check the texture name for a prefix, if it exists, do nothing
if it does not exist, append it
=================
*/
void CheckTexturePrefix (char *texname)
{
	char	string[128];
	char	*temp;

	// if the texture directory prefix is not in the texture name (i.e. imported file), add it
	if (!strstr (texname, texture_menunames[g_nCurrentTextureMenuName]))
	{	
		// if the texture name has an asterisk in it, remove it.
		if (texname[0] == '*')
		{
			temp = texname + 1;
			strcpy (texname, temp);
		}

		sprintf (string, "%s%s", 
			texture_menunames[g_nCurrentTextureMenuName],
			texname);

		temp = _strlwr (string);
		strcpy (texname, temp);
	}
}


//=====================================================

void SortTextures(void)
{	
	qtexture_t	*q, *qtemp, *qhead, *qcur, *qprev;

	// standard insertion sort
	// Take the first texture from the list and
	// add it to our new list
	if ( g_qeglobals.d_qtextures == NULL)
		return;	

	qhead = g_qeglobals.d_qtextures;
	q = g_qeglobals.d_qtextures->next;
	qhead->next = NULL;
	
	// while there are still things on the old
	// list, keep adding them to the new list
	while (q)
	{
		qtemp = q;
		q = q->next;
		
		qprev = NULL;
		qcur = qhead;

		while (qcur)
		{
			// Insert it here?
			if (strcmp(qtemp->name, qcur->name) < 0)
			{
				qtemp->next = qcur;
				if (qprev)
					qprev->next = qtemp;
				else
					qhead = qtemp;
				break;
			}
			
			// Move on

			qprev = qcur;
			qcur = qcur->next;


			// is this one at the end?

			if (qcur == NULL)
			{
				qprev->next = qtemp;
				qtemp->next = NULL;
			}
		}


	}

	g_qeglobals.d_qtextures = qhead;
}

//=====================================================


/*
==============
Texture_InitPalette
==============
*/
void Texture_InitPalette (byte *pal)
{
    int		r,g,b,v;
    int		i;
	int		inf;
	byte	gammatable[256];
	float	gamma;

	gamma = g_qeglobals.d_savedinfo.fGamma;

	if (gamma == 1.0)
	{
		for (i=0 ; i<256 ; i++)
			gammatable[i] = i;
	}
	else
	{
		for (i=0 ; i<256 ; i++)
		{
			inf = 255 * pow ( (i+0.5)/255.5 , gamma ) + 0.5;
			if (inf < 0)
				inf = 0;
			if (inf > 255)
				inf = 255;
			gammatable[i] = inf;
		}
	}

    for (i=0 ; i<256 ; i++)
    {
		  r = gammatable[pal[0]];
		  g = gammatable[pal[1]];
		  b = gammatable[pal[2]];
		  pal += 3;
		
		  v = (r<<24) + (g<<16) + (b<<8) + 255;
		  v = BigLong (v);
		
		  tex_palette[i] = v;
    }
}

void SetTexParameters (void)
{
	qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_mode );
	
	switch ( texture_mode )
	{
	case GL_NEAREST:
	case GL_NEAREST_MIPMAP_NEAREST:
	case GL_NEAREST_MIPMAP_LINEAR:
		qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		break;
	case GL_LINEAR:
	case GL_LINEAR_MIPMAP_NEAREST:
	case GL_LINEAR_MIPMAP_LINEAR:
		qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		break;
	}
}

/*
============
Texture_SetMode
============
*/
void Texture_SetMode(int iMenu)
{
	int	i, iMode;
	HMENU hMenu;
	qboolean texturing = true;

	hMenu = GetMenu(g_qeglobals.d_hwndMain);

	switch(iMenu) {
	case ID_VIEW_NEAREST:					
		iMode = GL_NEAREST;
		break;
	case ID_VIEW_NEARESTMIPMAP:
		iMode = GL_NEAREST_MIPMAP_NEAREST;
		break;
	case ID_VIEW_LINEAR:
		iMode = GL_NEAREST_MIPMAP_LINEAR;
		break;
	case ID_VIEW_BILINEAR:
		iMode = GL_LINEAR;
		break;
	case ID_VIEW_BILINEARMIPMAP:
		iMode = GL_LINEAR_MIPMAP_NEAREST;
		break;
	case ID_VIEW_TRILINEAR:
		iMode = GL_LINEAR_MIPMAP_LINEAR;
		break;

	case ID_TEXTURES_WIREFRAME:
		iMode = 0;
		texturing = false;
		break;

	case ID_TEXTURES_FLATSHADE:
		iMode = 0;
		texturing = false;
		break;

	}

	CheckMenuItem(hMenu, ID_VIEW_NEAREST, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_NEARESTMIPMAP, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_LINEAR, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_BILINEARMIPMAP, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_BILINEAR, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_TRILINEAR, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_TEXTURES_WIREFRAME, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_TEXTURES_FLATSHADE, MF_BYCOMMAND | MF_UNCHECKED);

	CheckMenuItem(hMenu, iMenu, MF_BYCOMMAND | MF_CHECKED);

	g_qeglobals.d_savedinfo.iTexMenu = iMenu;
	texture_mode = iMode;

  if (g_PrefsDlg.m_bSGIOpenGL)
  {
    if (s_hdcTexture && s_hglrcTexture)
    {
      //if (!qwglMakeCurrent(g_qeglobals.d_hdcBase, g_qeglobals.d_hglrcBase))
      if (!qwglMakeCurrent(s_hdcTexture, s_hglrcTexture))
		    Error ("wglMakeCurrent in LoadTexture failed");
    }
    else
      return;
  }

	if ( texturing )
		SetTexParameters ();

	if ( !texturing && iMenu == ID_TEXTURES_WIREFRAME)
	{
		g_pParentWnd->GetCamera()->Camera().draw_mode = cd_wire;
		Map_BuildBrushData();
		Sys_UpdateWindows (W_ALL);
		return;

	} else if ( !texturing && iMenu == ID_TEXTURES_FLATSHADE) {

		g_pParentWnd->GetCamera()->Camera().draw_mode = cd_solid;
		Map_BuildBrushData();
		Sys_UpdateWindows (W_ALL);
		return;
	}

	for (i=1 ; i<texture_extension_number ; i++)
	{
		qglBindTexture( GL_TEXTURE_2D, i );
		SetTexParameters ();
	}

	// select the default texture
	qglBindTexture( GL_TEXTURE_2D, 0 );

	qglFinish();

	if (g_pParentWnd->GetCamera()->Camera().draw_mode != cd_texture)
	{
		g_pParentWnd->GetCamera()->Camera().draw_mode = cd_texture;
		Map_BuildBrushData();
	}

	Sys_UpdateWindows (W_ALL);
}

/*
================
R_MipMap

Operates in place, quartering the size of the texture
================
*/
void R_MipMap (byte *in, int &width, int &height)
{
	int		i, j;
	byte	*out;
	int		row;
	
	row = width * 4;
	width >>= 1;
	height >>= 1;
	out = in;
	for (i=0 ; i<height ; i++, in+=row)
	{
		for (j=0 ; j<width ; j++, out+=4, in+=8)
		{
			out[0] = (in[0] + in[4] + in[row+0] + in[row+4])>>2;
			out[1] = (in[1] + in[5] + in[row+1] + in[row+5])>>2;
			out[2] = (in[2] + in[6] + in[row+2] + in[row+6])>>2;
			out[3] = (in[3] + in[7] + in[row+3] + in[row+7])>>2;
		}
	}
}

/*
=================
Texture_LoadTexture
=================
*/
//extern void Handle3DfxTexturing(qtexture_t* q, int nWidth, int nHeight, unsigned* pDest);
qtexture_t *Texture_LoadTexture (miptex_t *qtex)
{
  byte		*source;
  unsigned	char *dest;
  int			width, height, i, count;
	int			total[3];
  qtexture_t	*q;
    
  
  width = LittleLong(qtex->width);
  height = LittleLong(qtex->height);

  //if (g_PrefsDlg.m_bSGIOpenGL)
  //{
  //  if (width == 320)
  //    return NULL;
  //}

  q = (qtexture_t*)qmalloc(sizeof(*q));

  q->width = width;
  q->height = height;

	q->flags = qtex->flags;
	q->value = qtex->value;
	q->contents = qtex->contents;

	dest = (unsigned char*)qmalloc (width*height*4);

  count = width*height;
  source = (byte *)qtex + LittleLong(qtex->offsets[0]);

	// The dib is upside down so we want to copy it into 
	// the buffer bottom up.

	total[0] = total[1] = total[2] = 0;
  for (i=0 ; i<count ; i++)
	{
		dest[i] = tex_palette[source[i]];

		total[0] += ((byte *)(dest+i))[0];
		total[1] += ((byte *)(dest+i))[1];
		total[2] += ((byte *)(dest+i))[2];
	}

	q->color[0] = (float)total[0]/(count*255);
	q->color[1] = (float)total[1]/(count*255);
	q->color[2] = (float)total[2]/(count*255);

  q->texture_number = texture_extension_number++;

  if (g_PrefsDlg.m_bSGIOpenGL)
  {
    //if (!qwglMakeCurrent(g_qeglobals.d_hdcBase, g_qeglobals.d_hglrcBase))
    if (!qwglMakeCurrent(s_hdcTexture, s_hglrcTexture))
		  Error ("wglMakeCurrent in LoadTexture failed");
  }

  qglBindTexture( GL_TEXTURE_2D, q->texture_number );

  //Handle3DfxTexturing(q, width, height, dest);

  SetTexParameters ();

  int nCount = MAX_TEXTURE_QUALITY - g_PrefsDlg.m_nTextureQuality;
  while (nCount-- > 0)
  {
    if (width > 16 && height > 16)
    {
      R_MipMap(dest, width, height);
    }
    else
    {
      break;
    }
  }

  if (g_PrefsDlg.m_bSGIOpenGL)
  {
	  if (nomips)
    {
		  qglTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dest);
    }
	  else
		  qgluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,GL_RGBA, GL_UNSIGNED_BYTE, dest);
  }
  else
  {
	  if (nomips)
		  qglTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dest);
	  else
		  qgluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,GL_RGBA, GL_UNSIGNED_BYTE, dest);
  }

	free (dest);

	qglBindTexture( GL_TEXTURE_2D, 0 );

  return q;
}




/*
=================
Texture_LoadTexture
=================
*/
//extern void Handle3DfxTexturing(qtexture_t* q, int nWidth, int nHeight, unsigned* pDest);

// (the TGA part of this function name appears to be irrelevant, the only criteria is that the pixels supplied are
//	32bpp, and in a normal sequential memory buffer...)
//
qtexture_t *Texture_LoadTGATexture (unsigned char* pPixels, int nWidth, int nHeight, char* pPath, int nFlags, int nContents, int nValue )
{
  int i, j, inf;
	byte	gammatable[256];
	float fGamma = g_qeglobals.d_savedinfo.fGamma;


  qtexture_t* q = (qtexture_t*)qmalloc(sizeof(*q));
  q->width = nWidth;
  q->height = nHeight;
	q->flags = nFlags;
	q->value = nValue;
	q->contents = nContents;


  int nCount = nWidth * nHeight;
  float total[3];
  total[0] = total[1] = total[2] = 0.0f;

	if (fGamma == 1.0)
	{
		for (i=0 ; i<256 ; i++)
			gammatable[i] = i;
	}
	else
	{
		for (i=0 ; i<256 ; i++)
		{
			inf = 255 * pow ( (i+0.5)/255.5 , fGamma ) + 0.5;
			if (inf < 0)
				inf = 0;
			if (inf > 255)
				inf = 255;
			gammatable[i] = inf;
		}
	}


  // all targas are stored internally as 32bit so rgba = 4 bytes
  for (i = 0 ; i < (nCount * 4) ; i += 4)
	{
    for (j = 0; j < 3; j++)
    {
	    total[j] += (pPixels+i)[j];
      byte b = (pPixels+i)[j];
      (pPixels+i)[j] = gammatable[b];
              
    }
	}

	q->color[0] = total[0] / (nCount * 255);
	q->color[1] = total[1] / (nCount * 255);
	q->color[2] = total[2] / (nCount * 255);


  q->texture_number = texture_extension_number++;

  if (g_PrefsDlg.m_bSGIOpenGL)
  {
    //if (!qwglMakeCurrent(g_qeglobals.d_hdcBase, g_qeglobals.d_hglrcBase))
    if (!qwglMakeCurrent(s_hdcTexture, s_hglrcTexture))
		  Error ("wglMakeCurrent in LoadTexture failed");
  }

  qglBindTexture( GL_TEXTURE_2D, q->texture_number );

  //Handle3DfxTexturing(q, width, height, dest);

  SetTexParameters();

  nCount = MAX_TEXTURE_QUALITY - g_PrefsDlg.m_nTextureQuality;
  while (nCount-- > 0)
  {
    if (nWidth > 16 && nHeight > 16)
    {
      R_MipMap(pPixels, nWidth, nHeight);
    }
    else
    {
      break;
    }
  }

  if (g_PrefsDlg.m_bSGIOpenGL)
  {
	  if (nomips)
    {
		  qglTexImage2D(GL_TEXTURE_2D, 0, 3, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pPixels);
    }
	  else
		  qgluBuild2DMipmaps(GL_TEXTURE_2D, 3, nWidth, nHeight,GL_RGBA, GL_UNSIGNED_BYTE, pPixels);
  }
  else
  {
	  if (nomips)
		  qglTexImage2D(GL_TEXTURE_2D, 0, 3, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pPixels);
	  else
		  qgluBuild2DMipmaps(GL_TEXTURE_2D, 3, nWidth, nHeight,GL_RGBA, GL_UNSIGNED_BYTE, pPixels);
  }

	qglBindTexture( GL_TEXTURE_2D, 0 );

  return q;
}


qtexture_t *Texture_LoadTGATexture (unsigned char* pPixels, int nWidth, int nHeight, char *pPath)
{
  CString strName;
  CString strPath;
  ExtractPath_and_Filename(pPath, strPath, strName);
  AddSlash(strPath);
  strPath += "textureinfo.ini";
  strName.MakeLower();
  StripExtension (strName.GetBuffer(0));
  strName.ReleaseBuffer();
  
  int nFlags = GetPrivateProfileInt(strName, "Flags", 0, strPath);
  int nValue = GetPrivateProfileInt(strName, "Value", 0, strPath);
  int nContents = GetPrivateProfileInt(strName, "Contents", 0, strPath);
  return Texture_LoadTGATexture(pPixels, nWidth, nHeight, pPath, nFlags, nValue, nContents);
}


void Texture_LoadFromPlugIn(LPVOID vp)
{
  _QERTextureLoad *pLoad = reinterpret_cast<_QERTextureLoad*>(vp);
  if (pLoad != NULL)
  {
	  qtexture_t	*q;
    q = Texture_LoadTGATexture(pLoad->m_pRGBA, pLoad->m_nWidth, pLoad->m_nHeight, NULL, pLoad->m_nFlags, pLoad->m_nContents, pLoad->m_nValue);
    if (q != NULL)
    {
		// to save duplicate code (since one always ends up getting forgotten and out of sync) this is now done later by caller
//		  strcpy (q->name, pLoad->m_pName);
//		  StripExtension (q->name);
//		  if (!g_dontuse)
//			q->inuse = true;
//	    q->next = g_qeglobals.d_qtextures;
//	    g_qeglobals.d_qtextures = q;
      g_pluginTexture = q;
    }
  }
}


/*
===============
Texture_CreateSolid

Create a single pixel texture of the apropriate color
===============
*/
qtexture_t *Texture_CreateSolid (const char *name)
{
	byte	data[4];
	qtexture_t	*q;

  q = (qtexture_t*)qmalloc(sizeof(*q));
	
	sscanf (name, "(%f %f %f)", &q->color[0], &q->color[1], &q->color[2]);

	data[0] = q->color[0]*255;
	data[1] = q->color[1]*255;
	data[2] = q->color[2]*255;
	data[3] = 255;

	q->width = q->height = 1;
	//q->width = q->height = 2;
  q->texture_number = texture_extension_number++;
	qglBindTexture( GL_TEXTURE_2D, q->texture_number );
	SetTexParameters ();

  if (g_PrefsDlg.m_bSGIOpenGL)
  {
		qglTexImage2D(GL_TEXTURE_2D, 0, 3, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  }
  else
  {
	  if (nomips)
		  qglTexImage2D(GL_TEXTURE_2D, 0, 3, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	  else
		  qgluBuild2DMipmaps(GL_TEXTURE_2D, 3, 1, 1,GL_RGBA, GL_UNSIGNED_BYTE, data);
  }
	qglBindTexture( GL_TEXTURE_2D, 0 );

	return q;
}


/*
=================
Texture_MakeNotexture
=================
*/
void Texture_MakeNotexture (void)
{
  qtexture_t	*q;
  byte		data[4][4];


  if (g_PrefsDlg.m_bSGIOpenGL)
  {
    if (s_hdcTexture && s_hglrcTexture)
    { 
       //if (!qwglMakeCurrent(g_qeglobals.d_hdcBase, g_qeglobals.d_hglrcBase))
       if (!qwglMakeCurrent(s_hdcTexture, s_hglrcTexture))
		     Error ("wglMakeCurrent in LoadTexture failed");
    }
    else
      return;
  }

	notexture = q = (qtexture_t*)qmalloc(sizeof(*q));
	strcpy (q->name, "notexture");
  q->width = q->height = 64;
    
	memset (data, 0, sizeof(data));
	data[0][2] = data[3][2] = 255;

	q->color[0] = 0;
	q->color[1] = 0;
	q->color[2] = 0.5;

  q->texture_number = texture_extension_number++;
	qglBindTexture( GL_TEXTURE_2D, q->texture_number );
	SetTexParameters ();

	if (nomips)
		qglTexImage2D(GL_TEXTURE_2D, 0, 3, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		VERIFY(qgluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2,GL_RGBA, GL_UNSIGNED_BYTE, data) == 0);

  qglBindTexture( GL_TEXTURE_2D, 0 );
}

qtexture_t *Texture_MakePlaceHolder(void)
{
	qtexture_t *q;
	byte		data[4][4];
	
	if (g_PrefsDlg.m_bSGIOpenGL)
	{
		if (s_hdcTexture && s_hglrcTexture)
		{ 
			//if (!qwglMakeCurrent(g_qeglobals.d_hdcBase, g_qeglobals.d_hglrcBase))
			if (!qwglMakeCurrent(s_hdcTexture, s_hglrcTexture))
				Error ("wglMakeCurrent in Texture_MakePlaceHolder failed");
		}
		else
			return NULL;
	}
	
	q = (qtexture_t*)qmalloc(sizeof(*q));
	strcpy (q->name, "placeholder");	// overwritten on return
	q->width = q->height = 64;
	q->bPlaceHolder = true;
    
	memset (data, 0, sizeof(data));
	data[0][1] = data[3][1] = 255;		// green and black squares
	
	q->color[0] = 0;
	q->color[1] = 0;
	q->color[2] = 0.5;
	
	q->texture_number = texture_extension_number++;
	qglBindTexture( GL_TEXTURE_2D, q->texture_number );
	SetTexParameters ();
	
	if (nomips)
		qglTexImage2D(GL_TEXTURE_2D, 0, 3, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		VERIFY(qgluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2,GL_RGBA, GL_UNSIGNED_BYTE, data) == 0);
	
	qglBindTexture( GL_TEXTURE_2D, 0 );

	return q;
}



void LoadShader(const char* pFilename, qtexture_t *q)
{
  char* pBuff = NULL;
  CString strTexture;
  int nSize = LoadFile(pFilename, reinterpret_cast<void**>(&pBuff));
  if (nSize == -1)
  {
    nSize = PakLoadAnyFile(pFilename, reinterpret_cast<void**>(&pBuff));
  }
  if (nSize > 0)
  {
    StartTokenParsing(pBuff);
    while (GetToken(true))
    {
      // first token should be the path + name.. (from base)
      CShaderInfo *pShader = new CShaderInfo();
      pShader->setName(token);
      pShader->m_strShaderName = pFilename;
      strTexture = token;
      bool bGood = true;
      float fTrans = 1.0;
      GetToken(true);
      if (strcmp(token, "{"))
      {
        bGood = false;
        break;
      }
      else
      {
        // we need to read until we hit a balanced }
        int nMatch = 1;
        while (nMatch > 0 && GetToken(true))
        {
          if (strcmp(token, "{") == 0)
          {
            nMatch++;
          }
          else if (strcmp(token, "}") == 0)
          {
            nMatch--;
          }
          else if (strcmpi(token, "qer_nocarve") == 0)
          {
            pShader->m_nFlags |= QER_NOCARVE;
          }
          else if (strcmpi(token, "qer_trans") == 0)
          {
            if (GetToken(true))
            {
              fTrans = atof(token);
            }
            pShader->m_nFlags |= QER_TRANS;
          }
          else if (strcmpi(token, "qer_editorimage") == 0)
          {
            if (GetToken(true))
            {
              char* pTex = copystring(token);
          		QE_ConvertDOSToUnixName( pTex, pTex );
              CString str = pTex;
              free (pTex);
              FindReplace(str, "textures/", "");
              FindReplace(str, ".tga", "");
              int nPos = str.Find('/');
              if (nPos == -1)
              {
                nPos = str.Find('\\');
              }
              if (nPos >= 0)
              {
                if (g_PrefsDlg.m_bShaderTest)
                {
                  pShader->m_strTextureName = str;
				  pShader->m_strTextureName.MakeLower();	//avoid problems with case
                }
                else
                {
                  CString strPath = str.Left(nPos+1);
                  DeferredShaderLoad *deferred = new DeferredShaderLoad(str, pShader->m_strName, strPath); 
                  g_lstDeferred.Add(deferred);
                }
              }
            }
          }
          else if (strcmpi(token, "surfaceparm") == 0)
          {
            //--while (GetToken(false))
            //--{
            //--
            //--}
            if (GetToken(true))
            {
              // next token should be a surface parm
              //--if (strcmpi(token, "trans") == 0)
              //--{
              //--  fTrans = 0.33;
              //--}
              if (strcmpi(token, "fog") == 0)
              {
                if (fTrans == 1.0) // has not been explicitly set by qer_trans
                {
                  fTrans = 0.35;
                }
              }
            }
          }
        }
        if (nMatch != 0)
        {
          bGood = false;
          break;
        }
      }
      //--if (bGood && q)
      if (bGood)
      {
        pShader->m_fTransValue = fTrans;
        g_lstShaders.Add(pShader);

		if (g_PrefsDlg.m_bShaderTest)
        {
// new 
          if (pShader->m_strTextureName.GetLength() > 0)
          {
            if (!ShaderQTextureExists(pShader->m_strName))
            {
		          q = Texture_ForName (	pShader->m_strTextureName.GetBuffer(0),
										true,			// bool bForArchitecture
										NULL,			// const char *psBasePath
										eReplace_NO,	// eReplace_t eReplace
										true			// bool bPlaceHolderLoad
										);
              if (q != NULL)
              {
				  strcpy(q->shadername, pShader->m_strShaderName);
				  q->bFromShader = true;
				  q->fTrans = pShader->m_fTransValue;
				  q->nShaderFlags = pShader->m_nFlags;
				  strcpy(q->name, pShader->m_strName);
              }
            }
          }
        }
// end new 
        //--q->bFromShader = true;
        //--q->fTrans = fTrans;

        //--// good texture here
        //--//Sys_Printf("Test load texture %s\n", strTexture);
        //--// FIXME.. this is a load of crap
        //--strcpy(dirstring, strTexture);
        //--QE_ConvertDOSToUnixName(dirstring, dirstring);
        //--strTexture = dirstring;
        //--FindReplace(strTexture, "textures/", "");
		    //--qtexture_t *q = Texture_ForName (strTexture.GetBuffer(0));
        //--if (q != NULL)
        //--{
        //--  q->bFromShader = true;
        //--  q->fTrans = fTrans;
        //--}
      }
      else
      {
        Sys_Printf("Error parsing shader at texture %s\n", strTexture);
      }

    }
    free (pBuff);
  }
  else
  {
    Sys_Printf("Unabled to read shader %s\n", pFilename);
  }
}


extern bool DoesFileExist(const char* pBuff, long& lSize);
void SetNameShaderInfo(qtexture_t* q, const char* pPath, const char* pName)
{
  CShaderInfo *pInfo = hasShader(pName);
  if (pInfo)
  {
    strcpy(q->shadername, pInfo->m_strShaderName);
    q->bFromShader = true;
    q->fTrans = pInfo->m_fTransValue;
    q->nShaderFlags = pInfo->m_nFlags;
  }
  else
  {
    q->shadername[0] = 0;
  }
  strncpy (q->name, pName, sizeof(q->name) - 1);
	StripExtension (q->name);
}

void ReplaceQTexture(qtexture_t *pOld, qtexture_t *pNew)
{
  brush_t* pList = &active_brushes;
 
  for (brush_t* pBrush = pList->next ; pBrush != pList; pBrush = pBrush->next)
	{
    if (pBrush->patchBrush)
    {
      Patch_ReplaceQTexture(pBrush, pOld, pNew);
    }

    for (face_t* pFace = pBrush->brush_faces; pFace; pFace = pFace->next)
    {
      if (pFace->d_texture == pOld)
      {
        pFace->d_texture = pNew;
      }
    }
    //Brush_Build(pBrush);
	}

}


void Texture_Remove(qtexture_t *q)
{
  qtexture_t* pTex = g_qeglobals.d_qtextures->next;
  if (q == g_qeglobals.d_qtextures)   // it is the head
  {
    g_qeglobals.d_qtextures->next = q->next->next;
    g_qeglobals.d_qtextures = q->next;
  }
  else
  {
    qtexture_t* pLast = g_qeglobals.d_qtextures;
    while (pTex != NULL && pTex != g_qeglobals.d_qtextures)
    {
      if (pTex == q)
      {
        pLast->next = q->next;
        break;
      }
      pLast = pTex;
      pTex = pTex->next;
    }
  }
  qglDeleteTextures(1, reinterpret_cast<const unsigned int*>(&q->texture_number));
  free(q);

}


bool GetFileTime(LPCSTR psFileName, FILETIME &ft)
{
	bool bSuccess = false;
	HANDLE hFile = CreateFile(	psFileName,				// LPCTSTR lpFileName,          // pointer to name of the file
								GENERIC_READ,			// DWORD dwDesiredAccess,       // access (read-write) mode
								FILE_SHARE_READ,		// DWORD dwShareMode,           // share mode
								NULL,					// LPSECURITY_ATTRIBUTES lpSecurityAttributes,	// pointer to security attributes
								OPEN_EXISTING,			// DWORD dwCreationDisposition,  // how to create
								FILE_FLAG_SEQUENTIAL_SCAN,// DWORD dwFlagsAndAttributes,   // file attributes
								NULL					// HANDLE hTemplateFile          // handle to file with attributes to 
	  							);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		if (GetFileTime(hFile,		// handle to file
						NULL,		// LPFILETIME lpCreationTime
						NULL,		// LPFILETIME lpLastAccessTime
						&ft			// LPFILETIME lpLastWriteTime
						)
			)
		{
			bSuccess = true;
		}
	
		CloseHandle(hFile);
	}

	return bSuccess;
}

bool CurrentFileOutOfDate(FILETIME &ftCurrent, FILETIME &ftProposed)
{
	LONG l = CompareFileTime( &ftCurrent, &ftProposed );

	return (l<0);
}

static qtexture_t *Texture_ForName_Actual (const char *name, const char *psBasePath /* may be NULL */, qtexture_t *pRemove, bool bPlaceHolderLoad)
{
	byte    *lump;
	qtexture_t	*q;
	char	filename[1024];

	if (bPlaceHolderLoad)
	{
		Sys_Printf ("Placeholding %s...", name);	
		q = Texture_MakePlaceHolder();
		if (q == NULL)
		{
			Sys_Printf ("ERROR in SGI code!\n");	
			return notexture;	// only happens if Errorbox pops up using SGI OpenGL, so who cares?
		}
		SetNameShaderInfo(q, filename, name);
		Sys_Printf ("ok\n");	
	}
	else
	{
		if (name[0] == '(')
		{
			q = Texture_CreateSolid (name);
			strncpy (q->name, name, sizeof(q->name)-1);
		}
		else
		{
			// FIXME: this is a mess.. need to move consolidate stuff
			// down to a single routine.. 
			// 
			// if plugins have a texture loader
			// {
			//   
			// }
			// else
			// 
			if (g_pParentWnd->GetPlugInMgr().GetTextureInfo() != NULL)
			{
				// rad: 12/19/98
				// if the plugin is not a wad style then we need to treat it normally
				// otherwise return without trying to explicitly load the texture
				// as it should have been loaded by the wad style plugin at init time
				CString strTex = GetTextureExtension(0);
				sprintf (filename, "%s\\%s.%s", (psBasePath?psBasePath:ValueForKey (g_qeglobals.d_project_entity, "texturepath")), name, strTex);
				if (!g_pParentWnd->GetPlugInMgr().GetTextureInfo()->m_bWadStyle)
				{
					g_pParentWnd->GetPlugInMgr().LoadTexture(filename);        
					q = g_pluginTexture;
				}
				else
				{
					return notexture;
					// wadstyle.. if we get here then we do not have it
				}
			}
			else
			{
				if (g_PrefsDlg.m_bHiColorTextures == TRUE)
				{
					sprintf (filename, "%s/%s.tga", (psBasePath?psBasePath:ValueForKey (g_qeglobals.d_project_entity, "texturepath")), name);
					if (strchr(/*file*/name,' '))	// only check 'name', not 'filename' since we don't care about the "q:\quake" part (which CAN have spaces)
					{
						ErrorBox( va("Dammit!\n\nThe texture name \"%s\" has a space in it\n\nFix this now!!!",name));
					}
					else
					{
						Sys_Printf ("Loading %s...", name);
						unsigned char* pPixels = NULL;
						int nWidth;
						int nHeight;
						LoadTGA(filename, &pPixels, &nWidth, &nHeight);
						if (pPixels)
						{
							q = Texture_LoadTGATexture(pPixels, nWidth, nHeight, NULL, 0, 0, 0);
							
							SetNameShaderInfo(q, filename, name);
							Sys_Printf ("done.\n", name);
							free(pPixels);
						}
						else
						{
							Sys_Printf ("failed. Trying Shader...");
							
							//=========== findmeste2	
							CShaderInfo *pInfo = hasShader(name);
							if (pInfo)
							{
								sprintf (filename, "%s/%s.tga", (psBasePath?psBasePath:ValueForKey (g_qeglobals.d_project_entity, "texturepath")), pInfo->m_strTextureName);
								LoadTGA(filename, &pPixels, &nWidth, &nHeight);
							}
							
							if (pPixels)
							{
								q = Texture_LoadTGATexture(pPixels, nWidth, nHeight, NULL, 0, 0, 0);
								
								SetNameShaderInfo(q, filename, name);
								free(pPixels);
								
								Sys_Printf (" success, shader: \"%s\"\n", q->shadername);
							}
							else
							{
								Sys_Printf (" failed. Using default.  !! !! !!\n");
								q = notexture;
								SetNameShaderInfo(q, filename, name);
								Texture_MakeNotexture();
							}
						}
					}
				}
				else
				{
					// load the file
					sprintf (filename, "%s/%s.wal", (psBasePath?psBasePath:ValueForKey (g_qeglobals.d_project_entity, "texturepath")),	name);
					Sys_Printf ("Loading %s...", name);
					if (LoadFile (filename, (void**)&lump) == -1)
					{
						sprintf (filename, "%s.wal", name);
						Sys_Printf("failed.. trying pak0.pak..");
						if(!PakLoadFile(filename, (void**)&lump))
						{
							Sys_Printf (" load failed!\n");
							return notexture;
						}
					}
					Sys_Printf("successful.\n");
					q = Texture_LoadTexture ((miptex_t *)lump);
					free (lump);
					strncpy (q->name, name, sizeof(q->name)-1);
					StripExtension (q->name);
				}
			}
				
			if(!q)
				return notexture;
			strncpy (q->name, name, sizeof(q->name)-1);
			StripExtension (q->name);
		}// name[0] != '('
	}
	
	if(!q)	// safety
		return notexture;
	
	if (!g_dontuse)
		q->inuse = true;
	q->next = g_qeglobals.d_qtextures;
	g_qeglobals.d_qtextures = q;
	
	if (pRemove != NULL)
	{
		Sys_Printf ("** Replacing %s with %s **\n", pRemove->name, q->name);
		ReplaceQTexture(pRemove, q);
		Texture_Remove(pRemove);
	}
	
	return q;
}


extern char		gsFoundFilename[MAX_PATH];
extern FILETIME	gtFileTime;
extern bool		gbFileTimeValid;
//bool GetFileTime(LPCSTR psFileName, FILETIME &ft);
qtexture_t *Texture_ForName(const char *name, bool bForArchitecture /* = true */, const char *psBasePath /* = NULL */, eReplace_t eReplace /* = eReplace_NO */, bool bPlaceHolderLoad /* = false */ )
{
	qtexture_t	*q;
	
#ifdef QUAKE3
	// this is really dirty, I'm going to write back into a constant char array (which is part of a CString buffer)
	//	purely because I know that changing the case won't overrun any internal buffers... :-)
	//
	char *__p = const_cast <char *> (name);
	strlwr(__p);
#endif
	
	if (strlen(name) == 0)
		return notexture;
		
	qtexture_t *pRemove = NULL;

	for (q=g_qeglobals.d_qtextures ; q ; q=q->next)
	{
		if (!stricmp(name,  q->name))
		{
			if (bPlaceHolderLoad)	// don't worry about this until we try and load it seriously, this is just establishing the name slot
			{
				return q;
			}
			else
			{
				if (q->bPlaceHolder)
				{
					eReplace = eReplace_YES;	// neat, huh?
				}
			}

			switch ( eReplace )
			{
				case eReplace_NO:
					
					if (!g_dontuse)
						q->inuse = true;

					q->bForArchitecture = bForArchitecture;
					return q;
					
				case eReplace_YES:
					
					pRemove = q;
					//Texture_Remove(q);
					break;
					
					
				case eReplace_TIMESTAMP:

					if (q->sFTName[0])
					{
						FILETIME ftDiskVersion;

						if (GetFileTime(q->sFTName, ftDiskVersion))
						{
							if (!CurrentFileOutOfDate(q->ft, ftDiskVersion))
							{
								if (!g_dontuse)
									q->inuse = true;

								q->bForArchitecture = bForArchitecture;
								return q;
							}
						}
					}

					pRemove = q;
					break;
			}
			break;
		}
	}

	gbFileTimeValid = false;

	q = Texture_ForName_Actual(name, psBasePath, pRemove, bPlaceHolderLoad);

	if (gbFileTimeValid && gsFoundFilename[0])
	{
		q->ft = gtFileTime;
		strcpy(q->sFTName,gsFoundFilename);
	}
	else
	{
		q->sFTName[0] = '\0';
	}

	q->bForArchitecture = bForArchitecture;
	return q;
}

/*
===============
Texture_ForName
===============
*/
/*
// appears not to be used now
qtexture_t *Texture_ForNamePath(char* name, char* pFullPath)
{
  byte    *lump;
	qtexture_t	*q;
	char	filename[1024];

  if (strlen(name) == 0)
    return notexture;

	for (q=g_qeglobals.d_qtextures ; q ; q=q->next)
  {
	  if (!strcmp(name,  q->name))
		{
			if (!g_dontuse)
				q->inuse = true;
		    return q;
		}
  }

	if (name[0] == '(')
	{
		q = Texture_CreateSolid (name);
		strncpy (q->name, name, sizeof(q->name)-1);
	}
	else
	{
		// load the file
    if (g_PrefsDlg.m_bHiColorTextures == TRUE)
    {
      sprintf(filename, "%s%s", pFullPath, ".tga");
		  Sys_Printf ("Loading %s...", name);
      unsigned char* pPixels = NULL;
      int nWidth;
      int nHeight;
      LoadTGA(filename, &pPixels, &nWidth, &nHeight);
      if (pPixels)
      {
        q = Texture_LoadTGATexture(pPixels, nWidth, nHeight, NULL, 0, 0, 0);
      }
      else
      {
        return notexture;
      }
      free(pPixels);
    }
    else
    {
      sprintf(filename, "%s%s", pFullPath, ".wal");
		  Sys_Printf ("Loading %s...", name);
		  if (LoadFile (filename, (void**)&lump) == -1)
      {
			  Sys_Printf (" load failed!\n");
			  return notexture;
      }
      Sys_Printf("successful.\n");
		  q = Texture_LoadTexture ((miptex_t *)lump);
		  free (lump);
    }
    if (g_PrefsDlg.m_bSGIOpenGL)
    {
		  if(!q)
			  return notexture;
    }
		strncpy (q->name, name, sizeof(q->name)-1);
		StripExtension (q->name);
	}

	if (!g_dontuse)
		q->inuse = true;
	q->next = g_qeglobals.d_qtextures;
	g_qeglobals.d_qtextures = q;

  return q;
}
*/


/*
==================
FillTextureMenu

==================
*/
void FillTextureMenu (CStringArray* pArray)
{
	HMENU	hmenu;
	int		i;
	struct _finddata_t fileinfo;
	int		handle;
	char	dirstring[1024];
	char	*path;
	DIRLIST	*list = NULL, *temp;

  if (g_pParentWnd->GetPlugInMgr().GetTextureInfo() != NULL)
  {
    if (g_pParentWnd->GetPlugInMgr().GetTextureInfo()->m_bWadStyle)
      return;
  }

	hmenu = GetSubMenu (GetMenu(g_qeglobals.d_hwndMain), MENU_TEXTURE);

	// delete everything
	for (i=0 ; i<texture_nummenus ; i++)
		DeleteMenu (hmenu, CMD_TEXTUREWAD+i, MF_BYCOMMAND);

  texture_nummenus = 0;

	// add everything
  if (g_qeglobals.d_project_entity)
  {
    //--if (g_PrefsDlg.m_bUseShaders)
    //--{
	  //--  path = ValueForKey (g_qeglobals.d_project_entity, "basepath");
	  //--  sprintf (dirstring, "%s/scripts/*.shader", path);
    //--
    //--}
    //--else
    //--{
	    path = ValueForKey (g_qeglobals.d_project_entity, "texturepath");
	    sprintf (dirstring, "%s/*.*", path);
    //--}

	  handle = _findfirst (dirstring, &fileinfo);
	  if (handle != -1)
    {
	    do
	    {
        //--if (g_PrefsDlg.m_bUseShaders)
        //--{
		    //--  if ((fileinfo.attrib & _A_SUBDIR))
        //--    continue;
        //--}
        //--else
        //--{
		      if (!(fileinfo.attrib & _A_SUBDIR))
		        continue;
		      if (fileinfo.name[0] == '.')
		        continue;
        //--}
        // add this directory to the menu
	      AddToDirListAlphabetized(&list, fileinfo.name, FROMDISK);
	    } while (_findnext( handle, &fileinfo ) != -1);

	    _findclose (handle);
    }

    //--if (!g_PrefsDlg.m_bUseShaders)
    //--{
      GetPackTextureDirs(&list);
    //--}

	  for(temp = list; temp; temp = temp->next)
	  {
		  AppendMenu (hmenu, MF_ENABLED|MF_STRING, CMD_TEXTUREWAD+texture_nummenus, (LPCTSTR)temp->dirname);
		  strcpy (texture_menunames[texture_nummenus], temp->dirname);
      //--if (!g_PrefsDlg.m_bUseShaders)
      //--{
		    strcat (texture_menunames[texture_nummenus], "/");
      //--}
      if (pArray)
        pArray->Add(temp->dirname);
		  if (++texture_nummenus == MAX_TEXTUREDIRS)
		   break;
	  }

	  ClearDirList(&list);
  }


}


/*
==================
Texture_ClearInuse

A new map is being loaded, so clear inuse markers
==================
*/
void Texture_ClearInuse (void)
{
	qtexture_t	*q;

	for (q=g_qeglobals.d_qtextures ; q ; q=q->next)
    {
		q->inuse = false;
	}
}



/*
==============
Texture_LoadPalette
==============
*/
void Texture_LoadPalette (char *palette)
{
	char	name[1024];
	byte	*pal;						// added for daikatana

	Texture_ClearInuse ();

	//load the palette													
	sprintf (name, "%s/textures/%scolormap.bmp",						// added for daikatana
		ValueForKey(g_qeglobals.d_project_entity, "basepath"), palette);// added for daikatana	

	Load256Image (name, NULL, &pal, NULL, NULL);						// added for daikatana
	if (!pal)															// added for daikatana
		Error ("Couldn't load %s", name);								// added for daikatana
	Texture_InitPalette (pal);											// added for daikatana
	free (pal);															// added for daikatana	
	
	// create the fallback texture										// added for daikatana
	Texture_MakeNotexture ();											// added for daikatana
}

/*
==============
Texture_LoadPalette
==============
*/
void Texture_LoadPaletteExplicit(char *palette)
{
	char	name[1024];
	byte	*pal;						// added for daikatana

	Texture_ClearInuse ();

	//load the palette													
	sprintf (name, "%s%s",palette,"colormap.bmp");

	Load256Image (name, NULL, &pal, NULL, NULL);						// added for daikatana
	if (!pal)															// added for daikatana
		Error ("Couldn't load %s", name);								// added for daikatana
	Texture_InitPalette (pal);											// added for daikatana
	free (pal);															// added for daikatana	
	
	// create the fallback texture										// added for daikatana
	Texture_MakeNotexture ();											// added for daikatana
}


/*
==============
Texture_ShowDirectory
==============
*/
void	Texture_ShowDirectory (int menunum, bool bLinked)
{
	struct _finddata_t fileinfo;
	int		handle;
	char	name[1024];
	char	dirstring[1024];
	char	linkstring[1024];
	FILELIST			*list = NULL, *temp;
  CString strTemp;

  //Texture_Flush(false);
	Select_Deselect();
	texture_showinuse = false;
	strcpy (texture_directory, texture_menunames[menunum-CMD_TEXTUREWAD]);

  if (g_pParentWnd->GetPlugInMgr().GetTextureInfo() != NULL)
  {
    if (g_pParentWnd->GetPlugInMgr().GetTextureInfo()->m_bWadStyle)
      return;
  }

  // new
  if (!g_PrefsDlg.m_bShaderTest)
  {
	g_dontuse = true;	// needed because this next piece of code calls Texture_ForName() internally! -slc
    LoadDeferred(texture_directory);
    g_dontuse = false;
  }

  if (g_PrefsDlg.m_bHiColorTextures == FALSE)
  {
  }

	g_qeglobals.d_texturewin.originy = 0;

  //--if (g_PrefsDlg.m_bUseShaders)
  //--{
  //--  sprintf (dirstring, "%s/scripts/%s", ValueForKey (g_qeglobals.d_project_entity, "basepath"), texture_directory);
	//--  Sys_Printf("loading textures from shader %s\n", dirstring);
  //--  LoadShader(dirstring);
  //--}
  //--else
  //--{
	  Sys_Status("Loading textures\n", 0);

	  // load all image files

                                          
    sprintf (linkstring, "%s/textures/%stextureinfo.ini", ValueForKey (g_qeglobals.d_project_entity, "basepath"), texture_menunames[menunum-CMD_TEXTUREWAD]);

	for (int nExt = 0; nExt < GetTextureExtensionCount(); nExt++)	
	{
		sprintf (dirstring, "%s/textures/%s*.%s", ValueForKey (g_qeglobals.d_project_entity, "basepath"), texture_menunames[menunum-CMD_TEXTUREWAD],GetTextureExtension(nExt));
    Sys_Printf ("Scanning %s\n", dirstring);

	  handle = _findfirst (dirstring, &fileinfo);

    if (handle == -1)
    {
      sprintf(dirstring, "%s/%s*.%s", ValueForKey (g_qeglobals.d_project_entity, "texturepath"), texture_menunames[menunum-CMD_TEXTUREWAD],GetTextureExtension(nExt));

      handle = _findfirst (dirstring, &fileinfo);
    }


    if (handle != -1)
	  {
		  do
		  {
			  sprintf (name, "%s%s", texture_directory, fileinfo.name);
			  AddToFileListAlphabetized(&list, name, FROMDISK, 0, false);
		  } while (_findnext( handle, &fileinfo ) != -1);
		  _findclose (handle);
	  }
	  else
	  {//if you don't put textures/ in here, it will also return other crap like, menu/common/*.tga
	    sprintf (dirstring, "textures/%s*.%s", texture_menunames[menunum-CMD_TEXTUREWAD],GetTextureExtension(nExt));
		  if(!GetPackFileList(&list, dirstring)) {
//			  return;
		  }
	  }
	}

	  g_dontuse = true;
	  for(temp = list; temp; temp = temp->next)
	  {
		  if(temp->offset == -1)
			  sprintf(name, "%s", temp->filename);
		  else
			  sprintf(name, "%s%s", texture_menunames[menunum-CMD_TEXTUREWAD], temp->filename);
		  StripExtension (name);
      strTemp = name;
      strTemp.MakeLower();
      if ( strTemp.Find(".specular") >= 0 ||
           strTemp.Find(".glow") >= 0 ||
           strTemp.Find(".bump") >= 0 ||
           strTemp.Find(".diffuse") >= 0 ||
           strTemp.Find(".blend") >= 0 ||
           strTemp.Find(".alpha") >= 0
         )
        continue;
      else
      {
		  //if pref do replacing
		    //Texture_ForName (name, true);
		  //else
			Texture_ForName (name, true, NULL, eReplace_TIMESTAMP);

			// any architecture placeholder textures still need loading?...
			//
			bool bPlaceHoldersStillExist = false;
			for (qtexture_t *q=g_qeglobals.d_qtextures ; q ; q=q->next)
			{
				if (q->bForArchitecture && q->bPlaceHolder)
				{
					bPlaceHoldersStillExist = true;
					break;
				}
			}

			if (bPlaceHoldersStillExist)
			{
				// new bit, because of placeholder loading, we should also force-load any other shaders that use this
				//	texture...
				CString strTextureName(name);
				CShaderInfo *pParentShader = hasShader(name);
				if (pParentShader && !pParentShader->m_strTextureName.IsEmpty())
				{
					strTextureName = pParentShader->m_strTextureName;
				}
				
				// find all other shaders that also use this texture and load them as well...
				//
				int nSize = g_lstShaders.GetSize();
				for (int i = 0; i < nSize; i++)
				{
					CShaderInfo *pInfo = reinterpret_cast<CShaderInfo*>(g_lstShaders.ElementAt(i));
					if (pInfo != NULL && pInfo != pParentShader)
					{
						// does this shader same the same actual texture as the one we've just loaded?...
						//
						if (pInfo->m_strTextureName.CompareNoCase(strTextureName) == 0)
						{
							// yep, so ensure this one is loaded as well...
							//
							Texture_ForName(pInfo->m_strName, true, NULL, eReplace_TIMESTAMP);
						}
					}
				}
			}
      }
	  }

	  ClearFileList(&list);
  //--}


	g_dontuse = false;

  if (!bLinked)
  {

    for (int k = 0; k < 10; k++)
    {
      sprintf(name, "Path%d", k);
      if (GetPrivateProfileString("Include", name, "", dirstring, 1024, linkstring) > 0)
      {
        Texture_ShowDirectory(dirstring, true);
      }
    }

    SortTextures();
	  
    sprintf (name, "Textures: %s", texture_directory);
	  SetWindowText(g_qeglobals.d_hwndEntity, name);

	  // select the first texture in the list
	  if (!g_qeglobals.d_texturewin.texdef.name[0])
		  SelectTexture (16, g_qeglobals.d_texturewin.height -16, false);
  }

      Sys_Printf ("Done\n", dirstring);
}


// this can be combined with the above, but per usual i am in a hurry
//
void	Texture_ShowDirectory (char* pPath, bool bLinked)
{
	struct _finddata_t fileinfo;
	int		handle;
	char	name[1024];
	char	dirstring[1024];
	char	linkstring[1024];
	FILELIST			*list = NULL, *temp;

  //Texture_Flush(false);

	texture_showinuse = false;
	strcpy (texture_directory, pPath);

  if (g_PrefsDlg.m_bHiColorTextures == FALSE)
  {
  }

	g_qeglobals.d_texturewin.originy = 0;
	Sys_Status("loading all textures\n", 0);

	// load all .wal files
	for (int nExt = 0; nExt < GetTextureExtensionCount(); nExt++)
	{
		sprintf(dirstring, "%s*.%s", pPath,GetTextureExtension(nExt));
                                          
		Sys_Printf ("Scanning %s\n", dirstring);

		handle = _findfirst (dirstring, &fileinfo);

	  if (handle != -1)
		{
			do
			{
				sprintf (name, "%s%s", texture_directory, fileinfo.name);
				AddToFileListAlphabetized(&list, name, FROMDISK, 0, false);
			} while (_findnext( handle, &fileinfo ) != -1);
			_findclose (handle);
		}
		else
		{
			//sprintf (dirstring, "%s*.wal", texture_menunames[menunum-CMD_TEXTUREWAD]);
			//if(!GetPackFileList(&list, dirstring))
			//	return;
		}
	}

	g_dontuse = true;
	for(temp = list; temp; temp = temp->next)
	{
		if(temp->offset == -1)
			sprintf(name, "%s", temp->filename);
		else
		  sprintf(name, "%s%s", pPath, temp->filename);
		StripExtension (name);

    int nLen = strlen(name)-1;
    ASSERT(nLen > 0);
    while (name[nLen] != '\\')
      nLen--;
    // found first one
    nLen--;
    ASSERT(nLen > 0);
    while (name[nLen] != '\\')
      nLen--;
    ASSERT(nLen >= 0);
    QE_ConvertDOSToUnixName(name, name);
    Texture_ForName(&name[nLen+1]);

	}

	ClearFileList(&list);

	g_dontuse = false;


  if (!bLinked)
  {

    for (int k = 0; k < 10; k++)
    {
      sprintf(name, "Path%d", k);
      if (GetPrivateProfileString("Include", name, "", dirstring, 1024, linkstring) > 0)
      {
        Texture_ShowDirectory(dirstring, true);
      }
    }

	  SortTextures();

	  sprintf (name, "Textures: %s", texture_directory);
	  SetWindowText(g_qeglobals.d_hwndEntity, name);

	  // select the first texture in the list
	  if (!g_qeglobals.d_texturewin.texdef.name[0])
		  SelectTexture (16, g_qeglobals.d_texturewin.height -16, false);
  }
}



void Texture_ResetPosition()
{
  SelectTexture (16, g_qeglobals.d_texturewin.height -16, false);
  g_qeglobals.d_texturewin.originy = 0;
}


/*
==============
Texture_ShowInuse
==============
*/
void	Texture_ShowInuse (void)
{
	face_t	*f;
	brush_t	*b;
	char	name[1024];

	texture_showinuse = true;
	g_dontuse = false;

	g_qeglobals.d_texturewin.originy = 0;	

	Texture_ClearInuse();
	Sys_Status("Selecting active textures\n", 0);

	for (b=active_brushes.next ; b != NULL && b != &active_brushes ; b=b->next)
	{
		const char *p = Patch_FromBrush_GetTextureName(b);
		if (strlen(p))
			Texture_ForName (p);

		for (f=b->brush_faces ; f ; f=f->next)
			Texture_ForName (f->texdef.name);
	}

	for (b=selected_brushes.next ; b != NULL && b != &selected_brushes ; b=b->next)
	{
		const char *p = Patch_FromBrush_GetTextureName(b);
		if (strlen(p))
			Texture_ForName (p);

		for (f=b->brush_faces ; f ; f=f->next)
			Texture_ForName (f->texdef.name);
	}

	SortTextures();
	//SetInspectorMode(W_TEXTURE);
	Sys_UpdateWindows (W_TEXTURE);

	sprintf (name, "Textures: in use");
	SetWindowText(g_qeglobals.d_hwndEntity, name);

	// select the first texture in the list
	if (!g_qeglobals.d_texturewin.texdef.name[0])
	{
		SelectTexture (16, g_qeglobals.d_texturewin.height -16, false);
	}
}

/*
============================================================================

TEXTURE LAYOUT

============================================================================
*/

void Texture_StartPos (void)
{
	current_texture = g_qeglobals.d_qtextures;
	current_x = 8;
	current_y = -8;
	current_row = 0;
}

qtexture_t *Texture_NextPos (int *x, int *y)
{
	qtexture_t	*q;

	while (1)
	{
		q = current_texture;
		if (!q)
			return q;
		current_texture = current_texture->next;
		if (q->name[0] == '(')	// fake color texture
			continue;

	if (!q->bForArchitecture)
		continue;


    if (g_bFilterEnabled)
    {
      CString strName = q->name;
      int nPos = strName.Find('\\');
      if (nPos == -1)
        nPos = strName.Find('/');
      if (nPos >= 0)
        strName = strName.Right(strName.GetLength() - nPos - 1);
      if (strnicmp(g_strFilter.GetBuffer(0), strName, g_strFilter.GetLength()) == 0)
        break;
      else
        continue;
    }

    if (q->bFromShader && g_PrefsDlg.m_bShowShaders == FALSE)
    {
      continue;
    }

		if (q->inuse)
			break;			// always show in use
    
    if (!texture_showinuse && !strnicmp (q->name, texture_directory, strlen(texture_directory)))
			break;
		continue;
	}

  int nWidth = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? q->width * ((float)g_PrefsDlg.m_nTextureScale / 100) : q->width;
  int nHeight = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? q->height * ((float)g_PrefsDlg.m_nTextureScale / 100) : q->height;
	if (current_x + nWidth > g_qeglobals.d_texturewin.width-8 && current_row)
	{	// go to the next row unless the texture is the first on the row
		current_x = 8;
		current_y -= current_row + FONT_HEIGHT + 4;
		current_row = 0;
	}

	*x = current_x;
	*y = current_y;

	// Is our texture larger than the row? If so, grow the 
	// row height to match it

    if (current_row < nHeight)
		  current_row = nHeight;

	// never go less than 64, or the names get all crunched up
	current_x += nWidth < 64 ? 64 : nWidth;
	current_x += 8;

	return q;
}

/*
============================================================================

  MOUSE ACTIONS

============================================================================
*/

static	int	textures_cursorx, textures_cursory;


/*
============
Texture_SetTexture

============
*/
void Texture_SetTexture (texdef_t *texdef, bool bFitScale/*=false*/, bool bNoSystemTextureOverwrite/*=false*/)
{
	qtexture_t	*q;
	int			x,y;

	if (texdef->name[0] == '(')
	{
		Sys_Status("Can't select an entity texture\n", 0);
		return;
	}
	g_qeglobals.d_texturewin.texdef = *texdef;
	g_qeglobals.d_texturewin.texdef.flags &= ~SURF_KEEP;
	g_qeglobals.d_texturewin.texdef.contents &= ~CONTENTS_KEEP;

	Sys_UpdateWindows (W_TEXTURE);

  g_dlgFind.updateTextures(texdef->name);

  if (!g_dlgFind.isOpen())// && bSetSelection)
  {
	Select_SetTexture(texdef, bFitScale, bNoSystemTextureOverwrite);
  }

// scroll origin so the texture is completely on screen
	Texture_StartPos ();
	while (1)
	{
		q = Texture_NextPos (&x, &y);
		if (!q)
			break;

    int nWidth = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? q->width * ((float)g_PrefsDlg.m_nTextureScale / 100) : q->width;
    int nHeight = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? q->height * ((float)g_PrefsDlg.m_nTextureScale / 100) : q->height;
		if (!strcmpi(texdef->name, q->name))
		{
			if (y > g_qeglobals.d_texturewin.originy)
			{
				g_qeglobals.d_texturewin.originy = y;
				Sys_UpdateWindows (W_TEXTURE);
				return;
			}

			if (y-nHeight-2*FONT_HEIGHT < g_qeglobals.d_texturewin.originy-g_qeglobals.d_texturewin.height)
			{
				g_qeglobals.d_texturewin.originy = y-nHeight-2*FONT_HEIGHT+g_qeglobals.d_texturewin.height;
				Sys_UpdateWindows (W_TEXTURE);
				return;
			}

			return;
		}
	}
}


/*
==============
SelectTexture

  By mouse click
==============
*/
void SelectTexture (int mx, int my, bool bShift, bool bFitScale/*=false*/, bool bNoSystemTextureOverwrite/*=false*/)
{
	int		x, y;
	qtexture_t	*q, *pPrev = NULL;
	texdef_t	tex;
	
	my += g_qeglobals.d_texturewin.originy-g_qeglobals.d_texturewin.height;
	
	Texture_StartPos ();
	for (;;pPrev = q)
	{
		q = Texture_NextPos (&x, &y);
		if (!q)
			break;
		int nWidth = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? q->width * ((float)g_PrefsDlg.m_nTextureScale / 100) : q->width;
		int nHeight = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? q->height * ((float)g_PrefsDlg.m_nTextureScale / 100) : q->height;
		if (mx > x && mx - x < nWidth
			&& my < y && y - my < nHeight + FONT_HEIGHT)
		{
			if (q->bPlaceHolder)
			{
				CWaitCursor wait;

				// this next bit will change the order of the link list by inserting us at the front, so we need to
				//	restore the list order for niceness...
				//
				qtexture_t *pNext = q->next;
				
				q = Texture_ForName(q->name, true, NULL, eReplace_YES, false);	// q is now different (new malloc, linked to list start)
				//
				// move new q ptr back to old position in list...
				//
				if (pPrev)
				{
					assert(g_qeglobals.d_qtextures == q);
					g_qeglobals.d_qtextures = q->next;
					pPrev->next = q;
					q->next = pNext;
				}
				else
				{
					// was at the head now, was at the head before, ergo nothing to do...
				}
				
				Sys_UpdateWindows( W_TEXTURE);
				return;
			}
			if (bShift)
			{
				if (g_PrefsDlg.m_bHiColorTextures && q->shadername[0] != 0)
				{
					CString s = "notepad ";
					s += q->shadername;
					WinExec(s, SW_SHOWNORMAL);
				}
			}
			memset (&tex, 0, sizeof(tex));
			tex.scale[0] = (g_PrefsDlg.m_bHiColorTextures) ? fTEXTURE_SCALE : 1;
			tex.scale[1] = (g_PrefsDlg.m_bHiColorTextures) ? fTEXTURE_SCALE : 1;
			tex.flags = q->flags;
			tex.value = q->value;
			tex.contents = q->contents;
			strcpy (tex.name, q->name);
			Texture_SetTexture (&tex, bFitScale,bNoSystemTextureOverwrite);
			CString strTex;
			CString strName = q->name;
			//int nPos = strName.Find('\\');
			//if (nPos == -1)
			//  nPos = strName.Find('/');
			//if (nPos >= 0)
			//  strName = strName.Right(strName.GetLength() - nPos - 1);
			strTex.Format("%s W: %i H: %i", strName.GetBuffer(0), q->width, q->height);
			g_pParentWnd->SetStatusText(3, strTex);
			return;
		}
	}
	
	Sys_Status("Did not select a texture\n", 0);
}

/*
==============
Texture_MouseDown
==============
*/
void Texture_MouseDown (int x, int y, int buttons)
{
	Sys_GetCursorPos (&textures_cursorx, &textures_cursory);

	// lbutton = select texture
	if (buttons == MK_LBUTTON || buttons == (MK_LBUTTON | MK_SHIFT) || buttons == (MK_LBUTTON | MK_CONTROL) || buttons == (MK_LBUTTON | MK_CONTROL | MK_SHIFT))
	{
		// I now distinguish shift and ctrl from shift&ctrl...
		//
		bool bShift						= (buttons & MK_SHIFT)		&& !(buttons & MK_CONTROL);
		bool bFitScale					= (buttons & MK_CONTROL)	&& !(buttons & MK_SHIFT);
		bool bNoSystemTextureOverwrite	= (buttons & MK_CONTROL)	&&  (buttons & MK_SHIFT);
		
		SelectTexture (x, g_qeglobals.d_texturewin.height - 1 - y, bShift, bFitScale, bNoSystemTextureOverwrite);
		UpdateSurfaceDialog();
		UpdatePatchInspector();
	}
}

/*
==============
Texture_MouseUp
==============
*/
void Texture_MouseUp (int x, int y, int buttons)
{
}

/*
==============
Texture_MouseMoved
==============
*/
void Texture_MouseMoved (int x, int y, int buttons)
{
	int scale = 1;

	if ( buttons & MK_SHIFT )
		scale = 4;

	// rbutton = drag texture origin
	if (buttons & MK_RBUTTON)
	{
		Sys_GetCursorPos (&x, &y);
		if ( y != textures_cursory)
		{
			g_qeglobals.d_texturewin.originy += ( y-textures_cursory) * scale;
			if (g_qeglobals.d_texturewin.originy > 0)
				g_qeglobals.d_texturewin.originy = 0;
			Sys_SetCursorPos (textures_cursorx, textures_cursory);
      CWnd *pWnd = CWnd::FromHandle(g_qeglobals.d_hwndTexture);
      if (g_PrefsDlg.m_bTextureScrollbar && pWnd != NULL)
      {
        pWnd->SetScrollPos(SB_VERT, abs(g_qeglobals.d_texturewin.originy));
      }
		  InvalidateRect(g_qeglobals.d_hwndTexture, NULL, false);
		  UpdateWindow (g_qeglobals.d_hwndTexture);
		}
		return;
	}
}


/*
============================================================================

DRAWING

============================================================================
*/

int imax(int iFloor, int i) { if (i>iFloor) return iFloor; return i; }
HFONT ghFont = NULL;

/*
============
Texture_Draw2
============
*/
void Texture_Draw2 (int width, int height)
{
	qtexture_t	*q;
	int			x, y;
	char		*name;

	qglClearColor (
		g_qeglobals.d_savedinfo.colors[COLOR_TEXTUREBACK][0],
		g_qeglobals.d_savedinfo.colors[COLOR_TEXTUREBACK][1],
		g_qeglobals.d_savedinfo.colors[COLOR_TEXTUREBACK][2],
		0);
	qglViewport (0,0,width,height);
	qglMatrixMode(GL_PROJECTION);
	qglLoadIdentity ();

	qglClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	qglDisable (GL_DEPTH_TEST);
	qglDisable(GL_BLEND);
	qglOrtho (0, width, g_qeglobals.d_texturewin.originy-height, g_qeglobals.d_texturewin.originy, -100, 100);
	qglEnable (GL_TEXTURE_2D);

	qglPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	g_qeglobals.d_texturewin.width = width;
	g_qeglobals.d_texturewin.height = height;
	Texture_StartPos ();

	while (1)
	{
		q = Texture_NextPos (&x, &y);
		if (!q)
			break;

    int nWidth = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? q->width * ((float)g_PrefsDlg.m_nTextureScale / 100) : q->width;
    int nHeight = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? q->height * ((float)g_PrefsDlg.m_nTextureScale / 100) : q->height;
		// Is this texture visible?
		if ( (y-nHeight-FONT_HEIGHT < g_qeglobals.d_texturewin.originy)
			&& (y > g_qeglobals.d_texturewin.originy - height) )
		{

			// if in use, draw a background
			if ((q->inuse && !texture_showinuse) || q->bFromShader)
			{
				qglLineWidth (1);

        if (q->bFromShader)
        {
				  qglColor3f (1,1,1);
        }
        else
        {
				  qglColor3f (0.5,1,0.5);
        }
				qglDisable (GL_TEXTURE_2D);

				qglBegin (GL_LINE_LOOP);
				qglVertex2f (x-1,y+1-FONT_HEIGHT);
				qglVertex2f (x-1,y-nHeight-1-FONT_HEIGHT);
				qglVertex2f (x+1+nWidth,y-nHeight-1-FONT_HEIGHT);
				qglVertex2f (x+1+nWidth,y+1-FONT_HEIGHT);
				qglEnd ();

				qglEnable (GL_TEXTURE_2D);
			}

			// Draw the texture
      float fScale = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? ((float)g_PrefsDlg.m_nTextureScale / 100) : 1.0;

			qglBindTexture( GL_TEXTURE_2D, q->texture_number );
      QE_CheckOpenGLForErrors();
			qglColor3f (1,1,1);
			qglBegin (GL_QUADS);
			qglTexCoord2f (0,0);
			qglVertex2f (x,y-FONT_HEIGHT);
			qglTexCoord2f (1,0);
			qglVertex2f (x+nWidth,y-FONT_HEIGHT);
			qglTexCoord2f (1,1);
			qglVertex2f (x+nWidth,y-FONT_HEIGHT-nHeight);
			qglTexCoord2f (0,1);
			qglVertex2f (x,y-FONT_HEIGHT-nHeight);
			qglEnd ();

			// draw the selection border
			if (!strcmpi(g_qeglobals.d_texturewin.texdef.name, q->name))
			{
				qglLineWidth (3);
				qglColor3f (1,0,0);
				qglDisable (GL_TEXTURE_2D);

				qglBegin (GL_LINE_LOOP);
				qglVertex2f (x-4,y-FONT_HEIGHT+4);
				qglVertex2f (x-4,y-FONT_HEIGHT-nHeight-4);
				qglVertex2f (x+4+nWidth,y-FONT_HEIGHT-nHeight-4);
				qglVertex2f (x+4+nWidth,y-FONT_HEIGHT+4);
				qglEnd ();

				qglEnable (GL_TEXTURE_2D);
				qglLineWidth (1);
			}

			// draw the texture name
  	  qglColor3f (0,0,0);
			
      qglRasterPos2f (x, y-FONT_HEIGHT+2);

			// don't draw the directory name
			for (name = q->name ; *name && *name != '/' && *name != '\\' ; name++)
				;
			if (!*name)
				name = q->name;
			else
				name++;

      if (g_PrefsDlg.m_bHiColorTextures && q->shadername[0] != 0)
      {
        // slow as shit
        CString s = "[";
        s += name;
        s += "]";
			  qglCallLists (s.GetLength(), GL_UNSIGNED_BYTE, s.GetBuffer(0));
      }
      else
      {
			  qglCallLists (strlen(name), GL_UNSIGNED_BYTE, name);
      }
		}
	}

	g_qeglobals.d_texturewin.m_nTotalHeight = abs(y) + 100;
	// reset the current texture
	qglBindTexture( GL_TEXTURE_2D, 0 );
	qglFinish();
}

#if 0
void Texture_LoadColorMap()
{
	char	name[1024];
	byte	*pal;

	// load the palette

	sprintf (name, "%s/pics/colormap.pcx", ValueForKey (g_qeglobals.d_project_entity, "basepath"));

	Load256Image (name, NULL, &pal, NULL, NULL);
	if (!pal)
  {
    // before dropping out, try to load it from the QERadiant directory
    CString strFile = g_strAppPath;
    AddSlash(strFile);
    strFile += "colormap.pcx";
	  Load256Image (strFile.GetBuffer(0), NULL, &pal, NULL, NULL);
	  if (!pal)
		  Error ("Couldn't load %s or %s", name, strFile);
  }
  else
  {
	  Texture_InitPalette (pal);
	  free (pal);
  }
}


//
//==================
//Texture_Init
//==================
//
void Texture_Init (bool bHardInit)
{
  Texture_LoadColorMap();

  // create the fallback texture
	Texture_MakeNotexture ();
	g_qeglobals.d_qtextures = NULL;
}
#else

void Texture_Init (bool bHardInit)
{
	char	name[1024];
	byte	*pal;

  if (g_PrefsDlg.m_bHiColorTextures == FALSE)
  {
	  // load the palette
	  sprintf (name, "%s/pics/colormap.pcx", ValueForKey (g_qeglobals.d_project_entity, "basepath"));

	  Load256Image (name, NULL, &pal, NULL, NULL);
	  if (!pal)
    {
      // before dropping out, try to load it from the QERadiant directory
      CString strFile = g_strAppPath;
      AddSlash(strFile);
      strFile += "colormap.pcx";
	    Load256Image (strFile.GetBuffer(0), NULL, &pal, NULL, NULL);
	    if (!pal)
		    Error ("Couldn't load %s or %s", name, strFile);
    }
	  Texture_InitPalette (pal);
	  free (pal);
  }

	// create the fallback texture

  if (bHardInit)
  {
	  Texture_MakeNotexture();
	  g_qeglobals.d_qtextures = NULL;
  }
  LoadShaders();

}


#endif

/*
==================
Texture_Flush
==================
*/
void Texture_Flush (bool bReload)
{
  if (!ConfirmModified())
    return;

  Map_New ();
  CWaitCursor cursor;
  CStringList strList;
  Texture_Init(false);

  if (g_qeglobals.d_qtextures)
  {
	  qtexture_t* pTex = g_qeglobals.d_qtextures->next;
    while (pTex != NULL && pTex != g_qeglobals.d_qtextures)
    {
      qtexture_t* pNextTex = pTex->next;
      if (bReload)
      {
        if (pTex->name[0] != '(')
        {
          strList.AddTail(pTex->name);
        }
      }
      free(pTex);
      pTex = pNextTex;
    }
  }

  GLuint* pGln = new GLuint[texture_extension_number-1];
  qglGenTextures(texture_extension_number-1, pGln);
  QE_CheckOpenGLForErrors();
  qglDeleteTextures(texture_extension_number-1, pGln);
  QE_CheckOpenGLForErrors();
  delete []pGln;
  texture_extension_number = 1;
	g_qeglobals.d_qtextures = NULL;

  if (bReload)
  {
    POSITION pos = strList.GetHeadPosition();
    while (pos)
    {
      CString strTex = strList.GetNext(pos);
		  Texture_ForName (strTex.GetBuffer(0));
    }
  }
}



/////////////////////////////////////////////////////////////////////////////
// CTexWnd
IMPLEMENT_DYNCREATE(CTexWnd, CWnd);

CTexWnd::CTexWnd()
{
  m_bNeedRange = true;
}

CTexWnd::~CTexWnd()
{
}


BEGIN_MESSAGE_MAP(CTexWnd, CWnd)
	//{{AFX_MSG_MAP(CTexWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PARENTNOTIFY()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_TEXTURES_FLUSH, OnTexturesFlush)
  ON_BN_CLICKED(1200, OnShaderClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTexWnd message handlers

/*
============
WTexWndProc
============
*/
LONG WINAPI TexWndProc (
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam)
{
	int		xPos, yPos;
    RECT	rect;

    GetClientRect(hWnd, &rect);

    switch (uMsg)
    {
	case WM_CREATE:
    s_hdcTexture = GetDC(hWnd);
		QEW_SetupPixelFormat(s_hdcTexture, false);

		if ( ( s_hglrcTexture = qwglCreateContext( s_hdcTexture ) ) == 0 )
			Error( "wglCreateContext in WTex_WndProc failed" );

		if (!qwglShareLists( g_qeglobals.d_hglrcBase, s_hglrcTexture ) )
			Error( "wglShareLists in WTex_WndProc failed" );

    if (!qwglMakeCurrent( s_hdcTexture, s_hglrcTexture ))
		  Error ("wglMakeCurrent in WTex_WndProc failed");

	  g_qeglobals.d_hwndTexture = hWnd;
		return 0;

	case WM_DESTROY:
		//wglMakeCurrent( NULL, NULL );
		//wglDeleteContext( s_hglrcTexture );
		 ReleaseDC( hWnd, s_hdcTexture );
		return 0;
#if 0
	case WM_PAINT:
        { 
		    PAINTSTRUCT	ps;

		    BeginPaint(hWnd, &ps);

        if ( !qwglMakeCurrent( s_hdcTexture, s_hglrcTexture ) )
        //if ( !wglMakeCurrent( ps.hdc, s_hglrcTexture ) )
        {
          Sys_Printf("ERROR: wglMakeCurrent failed..\n ");
          Sys_Printf("Please restart QERadiant if the Texture view is not working\n");
        }
        else
        {
			    Texture_Draw2 (rect.right-rect.left, rect.bottom-rect.top - g_nTextureOffset);
			    qwglSwapBuffers(s_hdcTexture);
//          TRACE("Texture Paint\n");
        }
		    EndPaint(hWnd, &ps);
        }
		return 0;
#endif
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		SetCapture( g_qeglobals.d_hwndTexture );
		xPos = (short)LOWORD(lParam);  // horizontal position of cursor 
		yPos = (short)HIWORD(lParam);  // vertical position of cursor 
		
		Texture_MouseDown (xPos, yPos - g_nTextureOffset, wParam);
		return 0;

	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
		xPos = (short)LOWORD(lParam);  // horizontal position of cursor 
		yPos = (short)HIWORD(lParam);  // vertical position of cursor 
		
		Texture_MouseUp (xPos, yPos - g_nTextureOffset, wParam);
		if (! (wParam & (MK_LBUTTON|MK_RBUTTON|MK_MBUTTON)))
			ReleaseCapture ();
		return 0;

	case WM_MOUSEMOVE:
		xPos = (short)LOWORD(lParam);  // horizontal position of cursor 
		yPos = (short)HIWORD(lParam);  // vertical position of cursor 
		
		Texture_MouseMoved (xPos, yPos - g_nTextureOffset, wParam);
		return 0;
    }

    return DefWindowProc (hWnd, uMsg, wParam, lParam);
}



BOOL CTexWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
  WNDCLASS wc;
  HINSTANCE hInstance = AfxGetInstanceHandle();
  if (::GetClassInfo(hInstance, TEXTURE_WINDOW_CLASS, &wc) == FALSE)
  {
    // Register a new class
  	memset (&wc, 0, sizeof(wc));
    wc.style         = CS_NOCLOSE | CS_OWNDC;
    wc.lpszClassName = TEXTURE_WINDOW_CLASS;
    wc.hCursor       = LoadCursor (NULL,IDC_ARROW);
    wc.lpfnWndProc = TexWndProc;
    if (AfxRegisterClass(&wc) == FALSE)
      Error ("CZWnd RegisterClass: failed");
  }

  cs.lpszClass = TEXTURE_WINDOW_CLASS;
  cs.lpszName = "TEX";
  if (cs.style != QE3_CHILDSTYLE && cs.style != QE3_STYLE)
    cs.style = QE3_SPLITTER_STYLE;

	return CWnd::PreCreateWindow(cs);
}

int CTexWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

  CRect rctEdit(8, 5, 20, 20);
  g_nTextureOffset = 0;

  if (g_PrefsDlg.m_bShaderTest)
  {
    m_wndShaders.Create("Show Shaders", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, rctEdit, this, 1200);
    m_wndShaders.ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0);
    m_wndShaders.SetCheck(g_PrefsDlg.m_bShowShaders);
    g_nTextureOffset = 25;
  }

  rctEdit.SetRect(8, g_nTextureOffset, 20, 20);
  m_wndFilter.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_LEFT, rctEdit, this, 1201);
  m_wndFilter.ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0);
  m_wndFilter.SetTexWnd(this);

  g_nTextureOffset += 25;
  if (!g_PrefsDlg.m_bTextureWindow)
  {
    m_wndFilter.ShowWindow(SW_HIDE);
    g_nTextureOffset -= 25;
  }

  ShowScrollBar(SB_VERT, g_PrefsDlg.m_bTextureScrollbar);
  m_bNeedRange = true;

	return 0;
}

void CTexWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
  CRect rctClient;
  GetClientRect(rctClient);
  if (g_PrefsDlg.m_bShaderTest)
  {
    m_wndShaders.SetWindowPos(NULL, rctClient.left + 8, rctClient.top + 5, rctClient.right - 16, 20, 0);
  }
  m_wndFilter.SetWindowPos(NULL, rctClient.left + 8, rctClient.top + 25, rctClient.right - 16, 20, 0);
  m_bNeedRange = true;
}

void CTexWnd::OnShaderClick()
{
  g_PrefsDlg.m_bShowShaders = (m_wndShaders.GetCheck() != 0);
  g_PrefsDlg.SavePrefs();
	RedrawWindow();
}

void CTexWnd::OnParentNotify(UINT message, LPARAM lParam) 
{
	CWnd::OnParentNotify(message, lParam);
}

int g_nLastLen = 0;
int g_nTimerHandle = -1;
char g_cLastChar;

void CTexWnd::UpdateFilter(const char* pFilter)
{
  if (g_nTimerHandle > 0)
    KillTimer(1);
  g_bFilterEnabled = false;
  if (pFilter)
  {
    g_strFilter = pFilter;
    if (g_strFilter.GetLength() > 0)
    {
      g_bFilterEnabled = true;
      if (g_pParentWnd->CurrentStyle() == QR_QE4 || g_pParentWnd->CurrentStyle() == QR_4WAY)
      {
        if (g_strFilter.GetLength() > g_nLastLen)
        {
          g_cLastChar = toupper(g_strFilter.GetAt(g_strFilter.GetLength()-1));
          if (g_cLastChar == 'N' || g_cLastChar == 'O') // one of the other popups
          {
            g_nTimerHandle = SetTimer(1, 800, NULL);   // half second timer
          }
        }
      }
    }
    g_nLastLen = g_strFilter.GetLength();
	  SortTextures();
  }
  Sys_UpdateWindows (W_TEXTURE);
}

void CTexWnd::UpdatePrefs()
{
  if (!g_PrefsDlg.m_bTextureWindow)
  {
    m_wndFilter.ShowWindow(SW_HIDE);
    g_nTextureOffset = 0;
  }
  else
  {
    m_wndFilter.ShowWindow(SW_SHOW);
    g_nTextureOffset = 25;
  }
  ShowScrollBar(SB_VERT, g_PrefsDlg.m_bTextureScrollbar);
  m_bNeedRange = true;
  Invalidate();
  UpdateWindow();
}

void CTexWnd::FocusEdit()
{
  if (m_wndFilter.IsWindowVisible())
    m_wndFilter.SetFocus();
}

void CTexWnd::OnTimer(UINT nIDEvent) 
{
  KillTimer(1);
  g_nLastLen = 0;
  g_nTimerHandle = -1;
  ::SetFocus(g_qeglobals.d_hwndEntity);
  ::PostMessage(g_qeglobals.d_hwndEntity, WM_CHAR, g_cLastChar, 0);
}

void CTexWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  g_pParentWnd->HandleKey(nChar, nRepCnt, nFlags);
	//CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTexWnd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  g_pParentWnd->HandleKey(nChar, nRepCnt, nFlags, false);
}

void CTexWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
  CRect rctClient;
  GetClientRect(rctClient);
  int nOld = g_qeglobals.d_texturewin.m_nTotalHeight;
  if (!qwglMakeCurrent(s_hdcTexture, s_hglrcTexture))
  //if ( !qwglMakeCurrent(dc.m_hDC, s_hglrcTexture ) )
  {
    Sys_Printf("ERROR: wglMakeCurrent failed..\n ");
    Sys_Printf("Please restart QERadiant if the Texture view is not working\n");
  }
  else
  {
    Texture_Draw2 (rctClient.right-rctClient.left, rctClient.bottom-rctClient.top - g_nTextureOffset);
		qwglSwapBuffers(s_hdcTexture);
//    TRACE("Texture Paint\n");
  }
  if (g_PrefsDlg.m_bTextureScrollbar && (m_bNeedRange || g_qeglobals.d_texturewin.m_nTotalHeight != nOld))
  {
    m_bNeedRange = false;
    SetScrollRange(SB_VERT, 0, g_qeglobals.d_texturewin.m_nTotalHeight, TRUE);
  }
}

void CTexWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);

  int n = GetScrollPos(SB_VERT);;
  switch (nSBCode)
  {
    case SB_LINEUP :
    {
      n = (n - 15 >  0) ? n - 15 : 0; 
      break;
    }
    case SB_LINEDOWN :
    {
      n = (n + 15 < g_qeglobals.d_texturewin.m_nTotalHeight) ? n + 15 : n; 
      break;
    }
    case SB_PAGEUP :
    {
      n = (n - g_qeglobals.d_texturewin.height >  0) ? n - g_qeglobals.d_texturewin.height : 0; 
      break;
    }
    case SB_PAGEDOWN :
    {
      n = (n + g_qeglobals.d_texturewin.height < g_qeglobals.d_texturewin.m_nTotalHeight) ? n + g_qeglobals.d_texturewin.height : n; 
      break;
    }
    case SB_THUMBPOSITION :
    {
      n = nPos;
      break;
    }
    case SB_THUMBTRACK :
    {
      n = nPos;
      break;
    }
  }
  SetScrollPos(SB_VERT, n);
	g_qeglobals.d_texturewin.originy = -((int)n);
  Invalidate();
  UpdateWindow();
  //Sys_UpdateWindows(W_TEXTURE);
}

/*
and are the caps new caps?  anything done with older stuff will be fubar'd.. which brings up the point if you ever naturalize a cap, you cannot force it back to cap texturing.. i will add that too
*/

void CTexWnd::OnTexturesFlush() 
{
	// TODO: Add your command handler code here
	
}

void LoadShaders()
{
	char	dirstring[1024];
	char	*path;
	struct _finddata_t fileinfo;
	int		handle;
  path = ValueForKey (g_qeglobals.d_project_entity, "basepath");
  sprintf (dirstring, "%s/shaders/*.shader", path);
/*
  sprintf (dirstring, "%s/shaders/shaderlist.txt", path);
  char *pBuff = NULL;
  
  int nLen = LoadFile(dirstring, reinterpret_cast<void**>(&pBuff));
  if (nLen == -1)
  {
    nLen = PakLoadAnyFile(dirstring, reinterpret_cast<void**>(&pBuff));
  }
  if (nLen > 0)
  {
    CStringList lst;
    StartTokenParsing(pBuff);
    nLen = 0;
    while (GetToken(true))
    {
      // each token should be a shader filename
      sprintf(dirstring, "%s/shaders/%s.shader", path, token);
      lst.AddTail(dirstring);
      nLen++;
    }
    POSITION pos = lst.GetHeadPosition();
    while (pos != NULL)
    {
      LoadShader(lst.GetAt(pos).GetBuffer(0), NULL);
      lst.GetNext(pos);
    }
    free(pBuff);
  }
  else
  {
    Sys_Printf("Unable to load shaderlist.txt, shaders not loaded!");
  }
*/

  handle = _findfirst (dirstring, &fileinfo);
  if (handle != -1)
  {
    do
    {
      if ((fileinfo.attrib & _A_SUBDIR))
        continue;
      sprintf(dirstring, "%s/shaders/%s", path, fileinfo.name);
      LoadShader(dirstring, NULL);
	  } while (_findnext( handle, &fileinfo ) != -1);

	  _findclose (handle);
  }
}

void FreeShaders()
{
  int nSize = g_lstShaders.GetSize();
  for (int i = 0; i < nSize; i++)
  {
    CShaderInfo *pInfo = reinterpret_cast<CShaderInfo*>(g_lstShaders.ElementAt(i));
    delete pInfo;
  }

  nSize = g_lstDeferred.GetSize();
  for (i = 0; i < nSize; i++)
  {
    DeferredShaderLoad *p = reinterpret_cast<DeferredShaderLoad*>(g_lstDeferred.ElementAt(i));
    delete p;
  }

  g_lstShaders.RemoveAll();
  g_lstDeferred.RemoveAll();
}

void ReloadShaders()
{
  FreeShaders();
  LoadShaders();
  qtexture_t* pTex = g_qeglobals.d_qtextures;
  while (pTex != NULL)
  {
    SetNameShaderInfo(pTex, NULL, pTex->name);
    pTex = pTex->next;
  }

}

int Texture_LoadSkin(char *pName, int *pnWidth, int *pnHeight, const char *psBasePath)
{
#if 1
	qtexture_t *pTex = Texture_ForName(Filename_WithoutExt(pName), false, psBasePath);
	*pnWidth = pTex->width;
	*pnHeight= pTex->height;

	return pTex->texture_number;

#else
  byte *pic = NULL;
  byte *pic32 = NULL;
	byte	*pal;
  int nTex = -1;

  if (strstr(pName, ".pcx") != NULL)
  {
    LoadPCX(pName, &pic, &pal, pnWidth, pnHeight);
    int c = (*pnWidth) * (*pnHeight);
	  pic32 = reinterpret_cast<unsigned char*>(qmalloc(4 * c ));
    byte* out = pic32;
	  for (int i = 0 ; i < c ; i++) 
    {
		  int p = pic[i];
		  out[0] = pal[p*3];
		  out[1] = pal[p*3 + 1];
		  out[2] = pal[p*3 + 2];
		  out[3] = 255;
		  out += 4;
	  }
    free(pic);
    free(pal);
  }
  else if (strstr(pName, ".tga") != NULL)
  {
    unsigned char* pPixels = NULL;
    LoadTGA(pName, &pPixels, pnWidth, pnHeight);
    if (pPixels)
    {
	    pic32 = reinterpret_cast<unsigned char*>(pPixels);
    }
  }

  if (pic32 != NULL)
  {
    nTex = texture_extension_number++;
    if (g_PrefsDlg.m_bSGIOpenGL)
    {
      //if (!qwglMakeCurrent(g_qeglobals.d_hdcBase, g_qeglobals.d_hglrcBase))
      if (!qwglMakeCurrent(s_hdcTexture, s_hglrcTexture))
		    Error ("wglMakeCurrent in LoadTexture failed");
    }

    qglBindTexture( GL_TEXTURE_2D, nTex);
    SetTexParameters ();

	  int nCount = MAX_TEXTURE_QUALITY - g_PrefsDlg.m_nTextureQuality;
	  while (nCount-- > 0)
	  {
	    if (*pnWidth > 16 && *pnHeight > 16)
	    {
	      R_MipMap(pic32, *pnWidth, *pnHeight);
	    }
	    else
	    {
	      break;
	    }
	  }

    if (g_PrefsDlg.m_bSGIOpenGL)
    {
	    if (nomips)
      {
		    qglTexImage2D(GL_TEXTURE_2D, 0, 3, *pnWidth, *pnHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pic32);
      }
	    else
		    qgluBuild2DMipmaps(GL_TEXTURE_2D, 3, *pnWidth, *pnHeight,GL_RGBA, GL_UNSIGNED_BYTE, pic32);
    }
    else
    {
	    if (nomips)
		    qglTexImage2D(GL_TEXTURE_2D, 0, 3, *pnWidth, *pnHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pic32);
	    else
		    qgluBuild2DMipmaps(GL_TEXTURE_2D, 3, *pnWidth, *pnHeight,GL_RGBA, GL_UNSIGNED_BYTE, pic32);
    }
	  free (pic32);
	  qglBindTexture( GL_TEXTURE_2D, 0 );
  }
  return nTex;
#endif
}


