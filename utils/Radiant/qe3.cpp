#include "stdafx.h"
#include "qe3.h"
#include "PrefsDlg.h"
#include <direct.h>  
#include <sys\stat.h> 
#ifdef SOF
#include "materials.h"
#endif
#include "oddbits.h"

QEGlobals_t  g_qeglobals;



void QE_CheckOpenGLForErrors(void)
{
  CString strMsg;
  int i = qglGetError();
  if (i != GL_NO_ERROR)
  {
    if (i == GL_OUT_OF_MEMORY)
    {
      //strMsg.Format("OpenGL out of memory error %s\nDo you wish to save before exiting?", qgluErrorString((GLenum)i));
      if (MessageBox(g_qeglobals.d_hwndMain, strMsg, "QERadiant Error", MB_YESNO) == IDYES)
      {
        Map_SaveFile(NULL, false);
      }
		  exit(1);
    }
    else
    {
      //strMsg.Format("Warning: OpenGL Error %s\n ", qgluErrorString((GLenum)i));
		  Sys_Printf (strMsg.GetBuffer(0));
    }
  }
}


char *ExpandReletivePath (char *const p)
{
	static char	temp[1024];
	char	*base;

	if (!p || !p[0])
		return NULL;
	if (p[0] == '/' || p[0] == '\\')
		return p;

	base = ValueForKey(g_qeglobals.d_project_entity, "basepath");
	sprintf (temp, "%s/%s", base, p);
	return temp;
}



void *qmalloc (size_t size)
{
	void *b;
	b = malloc(size);
	memset (b, 0, size);
	return b;
}

char *copystring (char *const s)
{
	char	*b;
	b = (char*)malloc(strlen(s)+1);
	strcpy (b,s);
	return b;
}


bool DoesFileExist(const char* const pBuff, long& lSize)
{
  CFile file;
  if (file.Open(pBuff, CFile::modeRead | CFile::shareDenyNone))
  {
    lSize += file.GetLength();
    file.Close();
    return true;
  }
  return false;
}

bool RollDownFiles(const char *const fName, int nCount, int nTop)
{	
	//delete the 0 file and move 1-nCount down
    CString strOld, strNew;
	int i;
    
	strOld.Format("%s.%i", fName, 0);
	remove (strOld);
	for (i=0;i<nCount;)
	{
		strNew.Format("%s.%i", fName, i);
		i++;
		strOld.Format("%s.%i", fName, i);
		if (rename (strOld, strNew) != 0)
			return false;
	}
	//make sure the top is clear
	for (i=nCount;i<=nTop;i++)
	{
		strOld.Format("%s.%i", fName, i);
		remove (strOld);
	}
	return true;
}


void Map_Snapshot()
{
	// we need to do the following
  // 1. make sure the snapshot directory exists (create it if it doesn't)
  // 2. find out what the lastest save is based on number
  // 3. inc that and save the map
	
	//only save if not a region!
	//using rolling filenames, preference for how many to keep.

	const int lastNumToKeep = g_PrefsDlg.m_numSnapShots - 1;
	if (region_active)
	{
		Sys_Printf("Skipping region snapshot...");
		return;
	}

  CString strOrgPath, strOrgFile;
  ExtractPath_and_Filename(currentmap, strOrgPath, strOrgFile);
  AddSlash(strOrgPath);
  strOrgPath += "snapshots";
  bool bGo = true;
  struct _stat Stat;
  if (_stat(strOrgPath, &Stat) == -1)
  {
    bGo = (_mkdir(strOrgPath) != -1);
  }
  AddSlash(strOrgPath);
  if (bGo)
  {
    int nCount = 0;
    long lSize = 0;
    CString strNewPath = strOrgPath;
    strNewPath += strOrgFile;
    CString strFile;
    while (1)
    {
      strFile.Format("%s.%i", strNewPath, nCount);
      if (!DoesFileExist(strFile, lSize))
		  break;
      nCount++;
    }
	if (nCount > lastNumToKeep)	//the roll limit is full
	{
		nCount--;
		RollDownFiles(strNewPath,lastNumToKeep, nCount);
	    strFile.Format("%s.%i", strNewPath, lastNumToKeep);	//set it back one to fill in top pos
	}
    // strFile has the next available slot
    Map_SaveFile(strFile, false);
	Sys_SetTitle (currentmap);
    /*if (lSize > 8 * 1024 * 1024) // total size of saves > 8 mb
    {
      Sys_Printf("The snapshot files in the [%s] directory total more than 4 megabytes. You might consider cleaning the directory up.", strOrgPath);
    }
	*/
  }
  else
  {
	CString strMsg;
    strMsg.Format("Snapshot save failed.. unabled to create directory\n%s", strOrgPath);
    g_pParentWnd->MessageBox(strMsg);
  }
}
/*
===============
QE_CheckAutoSave

If five minutes have passed since making a change
and the map hasn't been saved, save it out.
===============
*/


void QE_CheckAutoSave( void )
{
	static clock_t s_start;
	clock_t        now;

	now = clock();

	if ( modified != 1 || !s_start)
	{
		s_start = now;
		return;
	}

	if ( now - s_start > ( CLOCKS_PER_SEC * 60 * g_PrefsDlg.m_nAutoSave))
	{

    if (g_PrefsDlg.m_bAutoSave)
    {
      // only snapshot if not working on a default map
/*      if (g_PrefsDlg.m_bSnapShots && stricmp(currentmap, "unnamed.map") != 0)
      {
		  Sys_Printf("Autosaving snapshot...");
	      Sys_Printf("\n");
		  Sys_Status ("Autosaving snapshot...",0);
        Map_Snapshot();
      }
      else
*/
	  {
		  Sys_Printf("Autosaving ...");
	      Sys_Printf("\n");
		  Sys_Status ("Autosaving snapshot...",0);
		  Map_SaveFile (ValueForKey(g_qeglobals.d_project_entity, "autosave"), false);
	  }

		  Sys_Status ("Autosaving...Saved.", 0 );
		  modified = 2;
    }
    else
    {
		  Sys_Printf ("Autosave skipped...\n");
		  Sys_Status ("Autosave skipped...", 0 );
    }
		s_start = now;
	}
}


int
BuildShortPathName(const char* pPath, char* pBuffer, int nBufferLen)
{
  int nResult = GetShortPathName(pPath, pBuffer, nBufferLen);
  if (nResult == 0)
    strcpy(pBuffer, pPath);                     // Use long filename
  return nResult;
}




/*
===========
QE_LoadProject
===========
*/
qboolean QE_LoadProject (const char *const projectfile)
{
	char	*data;


	Sys_Printf ("QE_LoadProject (%s)\n", projectfile);

#ifdef SOF
	char	path2[MAX_PATH];
	strcpy(path2, projectfile);
	StripExtension(path2);
	DefaultExtension(path2, ".mat");
	QFile_ReadMaterialTypes(ExpandArg(path2));
#endif

	if ( LoadFileNoCrash (projectfile, (void **)&data) == -1)
		return false;

  g_strProject = projectfile;

  CString strData = data;
  free(data);

  CString strQ2Path = g_PrefsDlg.m_strQuake2;
  CString strQ2File;
  ExtractPath_and_Filename(g_PrefsDlg.m_strQuake2, strQ2Path, strQ2File);
  AddSlash(strQ2Path);


  char* pBuff = new char[1024];

  BuildShortPathName(strQ2Path, pBuff, 1024);
  FindReplace(strData, "__Q2PATH", pBuff);
  BuildShortPathName(g_strAppPath, pBuff, 1024);
  FindReplace(strData, "__QERPATH", pBuff);

  char* pFile;
  if (GetFullPathName(projectfile, 1024, pBuff, &pFile))
  {
    g_PrefsDlg.m_strLastProject = pBuff;
    BuildShortPathName(g_PrefsDlg.m_strLastProject, pBuff, 1024);
    g_PrefsDlg.m_strLastProject = pBuff;
    g_PrefsDlg.SavePrefs();

    ExtractPath_and_Filename(pBuff, strQ2Path, strQ2File);
    int nLen = strQ2Path.GetLength();
    if (nLen > 0)
    {
      if (strQ2Path[nLen - 1] == '\\')
        strQ2Path.SetAt(nLen-1,'\0');
      char* pBuffer = strQ2Path.GetBufferSetLength(_MAX_PATH + 1);
      int n = strQ2Path.ReverseFind('\\');
      if (n >=0 )
        pBuffer[n + 1] = '\0';
      strQ2Path.ReleaseBuffer();
      FindReplace(strData, "__QEPROJPATH", strQ2Path);
    }
  }

  delete []pBuff;

	StartTokenParsing (strData.GetBuffer(0));
	g_qeglobals.d_project_entity = Entity_Parse (true);
	if (!g_qeglobals.d_project_entity)
		Error ("Couldn't parse %s", projectfile);

	int len;
	
	strQ2Path = ValueForKey(g_qeglobals.d_project_entity, "basepath");
	len = strQ2Path.GetLength() - 1;
	if (strQ2Path[len] == '\\') {
		strQ2Path.SetAt(len,0);
		SetKeyValue(g_qeglobals.d_project_entity, "basepath",strQ2Path);	//make sure there is no trailing slash
	}

	strQ2Path = ValueForKey(g_qeglobals.d_project_entity, "remotebasepath");
	len = strQ2Path.GetLength() - 1;
	if (strQ2Path[len] == '\\') {
		strQ2Path.SetAt(len,0);
		SetKeyValue(g_qeglobals.d_project_entity, "remotebasepath",strQ2Path);	//make sure there is no trailing slash
	}

	Eclass_InitForSourceDirectory (ValueForKey (g_qeglobals.d_project_entity, "entitypath"));
	FillClassList();		// list in entity window

	Map_New();


	FillTextureMenu();
	FillBSPMenu();

	return true;
}
#if 0
qboolean QE_LoadProject (const char *projectfile)
{
	char	*data;
	char	path2[MAX_PATH];

	Sys_Printf ("QE_LoadProject (%s)\n", projectfile);

	strcpy(path2, projectfile);
	StripExtension(path2);
	DefaultExtension(path2, ".mat");
	QFile_ReadMaterialTypes(ExpandArg(path2));
	if ( LoadFileNoCrash (projectfile, (void *)&data) == -1)
		return false;
	StartTokenParsing (data);
	g_qeglobals.d_project_entity = Entity_Parse (true); // parses project file and 
	// stores key value pairs in linked list g_qeglobals.d_project_entity.epairs, accessed
	// using ValueforKey(g_qeglobals.d_project_entity, "keyname"));
	if (!g_qeglobals.d_project_entity)
		Error ("Couldn't parse %s", projectfile);
	free (data);

	Eclass_InitForSourceDirectory (ValueForKey (g_qeglobals.d_project_entity, "entitypath"));

	FillClassList ();		// list in entity window

	Map_New ();

	FillTextureMenu ();
	FillBSPMenu ();

	return true;
}

#endif

/*
===========
QE_SaveProject
===========
*/
//extern char	*bsp_commands[256];

qboolean QE_SaveProject (const char* pProjectFile)
{
	//char	filename[1024];
	FILE	*fp;
	epair_t	*ep;

	//sprintf (filename, "%s\\%s.prj", g_projectdir, g_username);

	if (!(fp = fopen (pProjectFile, "w+")))
		Error ("Could not open project file!");
	
	fprintf (fp, "{\n");
	for (ep = g_qeglobals.d_project_entity->epairs; ep; ep=ep->next)
		fprintf (fp, "\"%s\" \"%s\"\n", ep->key, ep->value);
	fprintf (fp, "}\n");

	fclose (fp);
	
	return TRUE;
}



/*
===========
QE_KeyDown
===========
*/
#define	SPEED_MOVE	32
#define	SPEED_TURN	22.5


/*
===============
ConnectEntities

Sets target / targetname on the two entities selected
from the first selected to the secon
===============
*/
void ConnectEntities (bool bIncrementalTargetNames /* = false */ )
{
	entity_t	*e1, *e2, *e;
	char		*target, *tn;
	int			maxtarg, targetnum;
	char		newtarg[32]={0};

	if (g_qeglobals.d_select_count != 2)
	{
		Sys_Status ("Must have two brushes selected.", 0);
		Sys_Printf ("Must have two brushes selected.\n");
		Sys_Beep ();
		return;
	}

	e1 = g_qeglobals.d_select_order[0]->owner;
	e2 = g_qeglobals.d_select_order[1]->owner;

	if (e1 == world_entity || e2 == world_entity)
	{
		Sys_Status ("Can't connect to the world.", 0);
		Sys_Printf ("Can't connect to the world.\n");
		Sys_Beep ();
		return;
	}

	if (e1 == e2)
	{
		Sys_Status ("Brushes are from same entity.", 0);
		Sys_Printf ("Brushes are from same entity.\n");
		Sys_Beep ();
		return;
	}

	static const char *pTargetNames[]=
	{
		"target",
		"target2",
		"target3",
		"target4"
	};
	const int iTargetNames = sizeof(pTargetNames)/sizeof(pTargetNames[0]);
	int iTarget = 0;	// leave as zero for "else" case below

	if (bIncrementalTargetNames)
	{
		for (iTarget=0; iTarget<iTargetNames; iTarget++)
		{
			target = ValueForKey (e1, pTargetNames[iTarget]);

			if (!target || !target[0])
			{
				// "target(n)"unused, so grab it...
				//
				break;
			}
		}
		if (iTarget == iTargetNames)
		{
			Sys_Status(va("ConnectEntities(): All %d 'target' slots full on first entity",iTargetNames),0);
			Sys_Printf(va("ConnectEntities(): All %d 'target' slots full on first entity\n",iTargetNames));
			Sys_Beep();
			return;
		}		
	}
	else
	{
		target = ValueForKey (e1, pTargetNames[iTarget]);
		if (target && target[0])
			strcpy (newtarg, target);
	}

	if (newtarg[0])
		strcpy (newtarg, target);
	else
	{
		target = ValueForKey (e2, "targetname");
		if (target && target[0])
			strcpy (newtarg, target);
		else
		{
			// make a unique target value
			maxtarg = 0;
			for (e=entities.next ; e != &entities ; e=e->next)
			{
				tn = ValueForKey (e, "targetname");
				if (tn && tn[0])
				{
					targetnum = atoi(tn+1);
					if (targetnum > maxtarg)
						maxtarg = targetnum;
				}
			}
			sprintf (newtarg, "t%i", maxtarg+1);
		}
	}

	SetKeyValue (e1, pTargetNames[iTarget], newtarg);
	SetKeyValue (e2, "targetname", newtarg);
	Sys_UpdateWindows (W_XY | W_CAMERA);

	Select_Deselect();
	Select_Brush (g_qeglobals.d_select_order[1]);
}

qboolean QE_SingleBrush (const bool bQuiet)
{
	if ( (selected_brushes.next == &selected_brushes)
		|| (selected_brushes.next->next != &selected_brushes) )
	{
	  if (!bQuiet)
    {
	  	Sys_Printf ("Error: you must have a single brush selected\n");
	  }
		return false;
	}
	if (selected_brushes.next->owner->eclass->fixedsize)
	{
	  if (!bQuiet)
	  {
		  Sys_Printf ("Error: you cannot manipulate fixed size entities\n");
	  }
		return false;
	}

	return true;
}

void QE_Init (void)
{
	/*
	** initialize variables
	*/
	g_qeglobals.d_gridsize = 8;
	g_qeglobals.d_showgrid = true;

	/*
	** other stuff
	*/
	Texture_Init (true);
	//Cam_Init ();
	//XY_Init ();
	Z_Init ();
}

void QE_ConvertDOSToUnixName( char *dst, const char *src )
{
	while ( *src )
	{
		if ( *src == '\\' )
			*dst = '/';
		else
			*dst = *src;
		dst++; src++;
	}
	*dst = 0;
}

int g_numbrushes, g_numentities;

void QE_CountBrushesAndUpdateStatusBar( void )
{
	static int      s_lastbrushcount, s_lastentitycount;
	static qboolean s_didonce;
	
	//entity_t   *e;
	brush_t	   *b, *next;

	g_numbrushes = 0;
	g_numentities = 0;
	
	if ( active_brushes.next != NULL )
	{
		for ( b = active_brushes.next ; b != NULL && b != &active_brushes ; b=next)
		{
			next = b->next;
			if (b->brush_faces )
			{
				if ( !b->owner->eclass->fixedsize)
					g_numbrushes++;
				else
					g_numentities++;
			}
		}
	}
/*
	if ( entities.next != NULL )
	{
		for ( e = entities.next ; e != &entities && g_numentities != MAX_MAP_ENTITIES ; e = e->next)
		{
			g_numentities++;
		}
	}
*/
	if ( ( ( g_numbrushes != s_lastbrushcount ) || ( g_numentities != s_lastentitycount ) ) || ( !s_didonce ) )
	{
		Sys_UpdateStatusBar();

		s_lastbrushcount = g_numbrushes;
		s_lastentitycount = g_numentities;
		s_didonce = true;
	}
}

