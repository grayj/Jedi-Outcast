// map.c

#include "stdafx.h"
#include "qe3.h"
#include "PrefsDlg.h"
#include "oddbits.h"
#ifdef QUAKE3
#include "sourcesafe.h"
#endif
#include "groupnames.h"

qboolean	modified;		// for quit confirmation (0 = clean, 1 = unsaved,
							// 2 = autosaved, but not regular saved) 

char		currentmap[1024];

brush_t	active_brushes;		// brushes currently being displayed
brush_t	selected_brushes;	// highlighted
face_t	*selected_face;
brush_t	*selected_face_brush;
brush_t	filtered_brushes;	// brushes that have been filtered or regioned

entity_t	entities;		// head/tail of doubly linked list

entity_t	*world_entity = NULL;

void AddRegionBrushes (void);
void RemoveRegionBrushes (void);


/*
=============================================================

  Cross map selection saving

  this could fuck up if you have only part of a complex entity selected...
=============================================================
*/

brush_t		between_brushes;
entity_t	between_entities;

bool g_bRestoreBetween = false;

void Map_SaveBetween (void)
{

	if (g_pParentWnd->ActiveXY())
  {
    g_bRestoreBetween = true;
    g_pParentWnd->ActiveXY()->Copy();
  }
  return;

#if 0

	brush_t		*b;
	entity_t	*e, *e2;

	between_brushes.next = selected_brushes.next;
	between_brushes.prev = selected_brushes.prev;
	between_brushes.next->prev = &between_brushes;
	between_brushes.prev->next = &between_brushes;

	between_entities.next = between_entities.prev = &between_entities;
	selected_brushes.next = selected_brushes.prev = &selected_brushes;

	for (b=between_brushes.next ; b != &between_brushes ; b=b->next)
	{
		e = b->owner;
		if (e == world_entity)
			b->owner = NULL;
		else
		{
			for (e2=between_entities.next ; e2 != &between_entities ; e2=e2->next)
				if (e2 == e)
					goto next;	// allready got the entity
			// move the entity over
			e->prev->next = e->next;
			e->next->prev = e->prev;
			e->next = between_entities.next;
			e->prev = &between_entities;
			e->next->prev = e;
			e->prev->next = e;
		}
next: ;
	}
#endif
}

void Map_RestoreBetween (void)
{
	if (g_pParentWnd->ActiveXY() && g_bRestoreBetween)
    g_pParentWnd->ActiveXY()->Paste();
  return;

#if 0  
  entity_t	*head, *tail;
	brush_t		*b;

	if (!between_brushes.next)
		return;

	for (b=between_brushes.next ; b != &between_brushes ; b=b->next)
	{
		if (!b->owner)
		{
			b->owner = world_entity;
			b->onext = world_entity->brushes.onext;
			b->oprev = &world_entity->brushes;
			b->onext->oprev = b;
			b->oprev->onext = b;
		}
	}

	selected_brushes.next = between_brushes.next;
	selected_brushes.prev = between_brushes.prev;
	selected_brushes.next->prev = &selected_brushes;
	selected_brushes.prev->next = &selected_brushes;

	head = between_entities.next;
	tail = between_entities.prev;

	if (head != tail)
	{
		entities.prev->next = head;
		head->prev = entities.prev;
		tail->next = &entities;
		entities.prev = tail;
	}

	between_brushes.next = NULL;
	between_entities.next = NULL;
#endif
}

//============================================================================

bool CheckForTinyBrush(brush_t* b, int n, float fSize)
{
  bool bTiny = false;
	for (int i=0 ; i<3 ; i++)
	{
    if (b->maxs[i] - b->mins[i] < fSize)
      bTiny = true;
  }
  if (bTiny)
    Sys_Printf("Possible problem brush (too small) #%i ", n);
  return bTiny;
}

void Map_BuildBrushData(void)
{
	brush_t	*b, *next;

	if (active_brushes.next == NULL)
		return;

	Sys_BeginWait ();	// this could take a while

  int n = 0;
	for (b=active_brushes.next ; b != NULL && b != &active_brushes ; b=next)
	{
		next = b->next;
		Brush_Build( b );
		if (!b->brush_faces || (g_PrefsDlg.m_bCleanTiny && CheckForTinyBrush(b, n++, g_PrefsDlg.m_fTinySize)))
		{
			Brush_Free (b);
			Sys_Printf ("Removed degenerate brush\n");
		}
	}
	Sys_EndWait();
}

entity_t *Map_FindClass (char *cname)
{
	entity_t	*ent;

	for (ent = entities.next ; ent != &entities ; ent=ent->next)
	{
		if (!strcmp(cname, ValueForKey (ent, "classname")))
			return ent;
	}
	return NULL;
}

/*
================
Map_Free
================
*/
void Map_Free (void)
{
#ifdef QUAKE3	
	// I'm going to try and offer s helpful auto-checkin menu when the user does a map free
	//
	LPSTR filename = currentmap;
	if ( world_entity && active_brushes.next && strcmp(filename,"unnamed.map") )	// apparently all this tells me that there's a valid map loaded
	{
		if ( SS_FunctionsAvailable() )
		{
			if ( SS_IsUnderSourceControl( filename ) )
			{
				if ( SS_IsCheckedOutByMe( filename ))
				{
					if (modified)
					{
						// if 'modified' flag is still set here then the user has clicked OK on "lose changes" OnNew, so...
						//
						if ( GetYesNo( va("Since you've decided to lose changes on the map:\n\n\"%s\"\n\n...do you want to Undo Checkout as well?",filename)))
						{
							if (SS_UndoCheckOut( filename ))
							{
								Sys_Printf ("(Undo Checkout performed on map)\n");
							}
							else
							{
								Sys_Printf ("(Undo Checkout failed on map)\n");
							}
						}
					}
					else
					{
						// if 'modified' is false here then the user has saved out a legit map, prompt for check in...
						//
						if ( GetYesNo( va("Since you've finished with the map:\n\n\"%s\"\n\n...do you want to do a Check In?",filename)))
						{
							if ( SS_CheckIn( filename ))
							{
								Sys_Printf ("(CheckIn performed on map)\n");
							}
							else
							{
								Sys_Printf ("(CheckIn failed on map)\n");
							}
						}
					}
				}
			}
		}
	}
#endif

	if (selected_brushes.next &&
		(selected_brushes.next != &selected_brushes) )
	{
    g_bRestoreBetween = false;
    if (MessageBox(g_qeglobals.d_hwndMain, "Copy selection?", "", MB_YESNO) == IDYES)
		  Map_SaveBetween ();
	}

	Grouping_Shutdown();

	Texture_ClearInuse ();
	Pointfile_Clear ();
	strcpy (currentmap, "unnamed.map");
	Sys_SetTitle (currentmap);
	g_qeglobals.d_num_entities = 0;

	if (!active_brushes.next)
	{	// first map
		active_brushes.prev = active_brushes.next = &active_brushes;
		selected_brushes.prev = selected_brushes.next = &selected_brushes;
		filtered_brushes.prev = filtered_brushes.next = &filtered_brushes;

		entities.prev = entities.next = &entities;
	}
	else
	{
		while (active_brushes.next != &active_brushes)
			Brush_Free (active_brushes.next);
		while (selected_brushes.next != &selected_brushes)
			Brush_Free (selected_brushes.next);
		while (filtered_brushes.next != &filtered_brushes)
			Brush_Free (filtered_brushes.next);

		while (entities.next != &entities)
			Entity_Free (entities.next);
	}

  if (world_entity)
    Entity_Free(world_entity);
	world_entity = NULL;
	selected_face= NULL;	// yet another crash-bug fix

	Patch_Cleanup();
}

/*
================
Map_LoadFile
================
*/
void Map_LoadFile (const char *filename)
{
    char		*buf;
	entity_t	*ent;
	char         temp[1024];

	Sys_BeginWait ();
  Select_Deselect();
	//SetInspectorMode(W_CONSOLE);

	Map_Free ();

	Sys_ClearPrintf ();

	QE_ConvertDOSToUnixName( temp, filename );
	Sys_Printf ("Map_LoadFile: %s\n", temp );	

	g_qeglobals.d_parsed_brushes = 0;

#ifdef QUAKE3
	// checkout the new map?
	if ( SS_FunctionsAvailable() )
	{
		if ( SS_IsUnderSourceControl( filename ) )
		{
			if ( SS_IsCheckedOut( filename ))
			{
				if ( !SS_IsCheckedOutByMe( filename ))
				{
					CString strCheckOuts;
					int iCount;
					
					if (SS_ListCheckOuts( filename, strCheckOuts, iCount ))
					{
						if (!GetYesNo(( va("Warning: Map \"%s\" is checked out by:\n\n%s\n... so you will be unable to compile or save\n\n\nDo you still want to load this map? (NO will abort load)",filename,(LPCSTR) strCheckOuts))))
						{
							Sys_Printf ("(map load aborted)\n");				
							return;
						}
					}
				}
				else
				{
					Sys_Printf ("(You own this file under SourceSafe)\n");				
				}
			}
			else
			{
				if ( GetYesNo( va("The map \"%s\"\n\n...is under SourceSafe control, check it out now?",filename) ))
				{
					if (SS_CheckOut( filename ))
					{
						Sys_Printf ("(Map checked out ok)\n");				
					}
				}
			}
		}
		else
		{
			Sys_Printf ("(This map is not under SourceSafe control)\n");				
		}
	}
#endif
	
	strcpy (currentmap, filename);

  if (LoadFile (filename, (void **)&buf) != -1)
  {

	  StartTokenParsing (buf);
	  g_qeglobals.d_num_entities = 0;

	  while (1)
	  {
		  ent = Entity_Parse (false, &active_brushes);
		  if (!ent)
			  break;
		  if (!strcmp(ValueForKey (ent, "classname"), "worldspawn"))
		  {
			  if (world_entity)
				  Sys_Printf ("WARNING: multiple worldspawn\n");
			  world_entity = ent;

		  }
		  else
		  {
			  // add the entity to the end of the entity list
			  ent->next = &entities;
			  ent->prev = entities.prev;
			  entities.prev->next = ent;
			  entities.prev = ent;
			  g_qeglobals.d_num_entities++;
		  }

#if 0
		  // special code to boost all (non-default) lights by 3.5. DO NOT LEAVE IN!!!! (may be useful again later)
			if (ent != world_entity)
			{
				if (!strcmp(ValueForKey (ent, "classname"), "light"))				
				{
					char *p = ValueForKey (ent, "light");
					if (strlen(p))
					{
						float f = atof(p);
						f *= 3.5f;
						p = va("%g",f);
						SetKeyValue(ent,"light",p);
					}
				}
			}
#endif

    }
	}

    free (buf);

	if (!world_entity)
	{
		Sys_Printf ("No worldspawn in map.\n");
		Map_New ();
		return;
	}

    Sys_Printf ("--- LoadMapFile ---\n");
    Sys_Printf ("%s\n", temp );

    Sys_Printf ("%5i brushes\n",  g_qeglobals.d_parsed_brushes );
    Sys_Printf ("%5i entities\n", g_qeglobals.d_num_entities);

	Map_RestoreBetween ();

	Sys_Printf ("Map_BuildAllDisplayLists\n");
    Map_BuildBrushData();

	//
	// move the view to a start position
	//
	ent = Map_FindClass ("info_player_start");
	if (!ent)
		ent = Map_FindClass ("info_player_deathmatch");
	g_pParentWnd->GetCamera()->Camera().angles[PITCH] = 0;
	if (ent)
	{
		GetVectorForKey (ent, "origin", g_pParentWnd->GetCamera()->Camera().origin);
		GetVectorForKey (ent, "origin", g_pParentWnd->GetXYWnd()->GetOrigin());
		g_pParentWnd->GetCamera()->Camera().angles[YAW] = FloatForKey (ent, "angle");
	}
	else
	{
		g_pParentWnd->GetCamera()->Camera().angles[YAW] = 0;
		VectorCopy (vec3_origin, g_pParentWnd->GetCamera()->Camera().origin);
		VectorCopy (vec3_origin, g_pParentWnd->GetXYWnd()->GetOrigin());
	}

	Sys_UpdateWindows (W_ALL);

  Patch_ReadFile (filename);
	
  Map_RegionOff ();


	modified = false;
	Sys_SetTitle (temp);

	Texture_ShowInuse ();

	Sys_EndWait();

}

/*
===========
Map_SaveFile
===========
*/
// now returns success/fail bool
bool Map_SaveFile (const char *filename, qboolean use_region )
{
	entity_t	*e, *next;
	FILE		*f;
	char         temp[1024];
	int			count;

  if (filename == NULL || strlen(filename) == 0)
  {
    CFileDialog dlgSave(FALSE, "map", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Map Files (*.map)|*.map||", AfxGetMainWnd());
    if (dlgSave.DoModal() == IDOK)
      filename = strdup(dlgSave.m_ofn.lpstrFile);
    else 
      return false;
  }

	Pointfile_Clear ();
	QE_ConvertDOSToUnixName( temp, filename );

	Sys_Printf ("Map_SaveFile: %s\n", filename);

#ifdef QUAKE3
	if ( SS_FunctionsAvailable() )
	{
		if ( SS_IsUnderSourceControl( filename ) )
		{
			if ( SS_IsCheckedOut( filename ))
			{
				if ( !SS_IsCheckedOutByMe( filename ))
				{
					CString strCheckOuts;
					int iCount;
					
					if (SS_ListCheckOuts( filename, strCheckOuts, iCount ))
					{
						ErrorBox( va("Map \"%s\" is checked out by:\n\n%s\n... so you can't save over it...\n\n... so you can't compile...\n\nTough luck matey!....(bwahahahaha!!!!!)",filename,(LPCSTR) strCheckOuts));
						return false;
					}
				}
				else
				{
					Sys_Printf ("(You own this file under SourceSafe)\n");				
				}
			}
			else
			{
				if ( GetYesNo( va("The map \"%s\"\n\n...needs to be checked out so I can save over it\n\nProceed? ('No' will abort the save)",filename) ))
				{
					if (SS_CheckOut( filename ))
					{
						Sys_Printf ("(Map checked out ok)\n");				
					}
					else
					{
						ASSERT(0);	// I want to know if this ever happens
						Sys_Printf ("(Error during map checkout, aborting save\n");				
						return false;
					}
				}
				else
				{
					Sys_Printf ("(Checkout cancelled, aborting save\n");				
					return false;
				}
			}
		}
		else
		{
			Sys_Printf ("(This map is not under SourceSafe control)\n");				
		}
	}

	// now do seperate check for files that are still write-protected...
	//
	DWORD dw = GetFileAttributes( filename );

	if (dw != 0xFFFFFFFF && ( dw & FILE_ATTRIBUTE_READONLY ))
	{
		// hmmm, still write protected...
		//
		if (SS_SetupOk())
		{
			if (GetYesNo( va("The file \"%s\" is write-protected, but probably not because of SourceSafe, just as a safety thing.\n\n(Tell me if you believe this is wrong -Ste)\n\nDo you want me to un-writeprotect it so you can save over it? ('No' will abort the save)",filename )))
			{
				if ( !SetFileAttributes( filename, dw&~FILE_ATTRIBUTE_READONLY) )
				{
					ErrorBox("Failed to remove write protect, aborting...");
					return false;
				}
			}
			else
			{
				Sys_Printf ("(Map was not write-enabled, aborting save)");
				return false;
			}
		}
		else
		{
			ErrorBox( va("The file \"%s\" is write-protected, but you don't appear to have SourceSafe set up properly on this machine, so I can't tell if the file is protected or just not checked out to you.\n\nIf you really want to edit this you'll have to write-enable it yourself (which I'm deliberately not offering to do for you here <g>)",filename));
		}
	}
#endif

	if (!use_region)
	{
		char	backup[1024];

		// rename current to .bak
		strcpy (backup, filename);
		StripExtension (backup);
		strcat (backup, ".bak");
		//
		// just before we try and delete the backup, make sure it's not write protected (if it exists)...
		//
		DWORD dw = GetFileAttributes( backup );
		if (dw != 0xFFFFFFFF && ( dw & FILE_ATTRIBUTE_READONLY ))		
			SetFileAttributes( backup, dw&~FILE_ATTRIBUTE_READONLY);
		_unlink (backup);
		rename (filename, backup);
	}	

	f = fopen(filename, "w");

	if (!f)
	{
		Sys_Printf ("ERROR!!!! Couldn't open %s for output\n", filename);
		return false;
	}

	if (use_region)
		AddRegionBrushes ();

	// write world entity first
	Entity_Write (world_entity, f, use_region);

	// then write all other ents
	count = 1;
	for (e=entities.next ; e != &entities ; e=next)
	{
		next = e->next;
		if (e->brushes.onext == &e->brushes)
    {
			Entity_Free (e);	// no brushes left, so remove it
    }
		else
    {
	   	fprintf (f, "// entity %i\n", count);
	  	count++;
			Entity_Write (e, f, use_region);
    }
	}

	fclose (f);

	if (use_region)
		RemoveRegionBrushes ();

	Sys_Printf ("Saved.\n");
	modified = false;

	if ( !strstr( temp, "autosave" ) )
		Sys_SetTitle (temp);

	if (!use_region)
	{
		time_t	timer;
		FILE	*f;

		time (&timer);
		MessageBeep (MB_ICONEXCLAMATION);
		f = fopen ("c:/tstamps.log", "a");
		if (f)
		{
			fprintf (f, "%s", filename);
			//fprintf (f, "%4i : %35s : %s", g_qeglobals.d_workcount, filename, ctime(&timer));
			fclose (f);
			g_qeglobals.d_workcount = 0;
		}
		fclose (f);
		Sys_Status ("Saved.\n", 0);
	}
	
  //Curve_WriteFile (filename);		//.trinity
  //Patch_WriteFile (filename);

	return true;
}

/*
===========
Map_New
===========
*/
void Map_New (void)
{
	Sys_Printf ("Map_New\n");
	Map_Free (); 

	world_entity = (entity_s*)qmalloc(sizeof(*world_entity));
	world_entity->brushes.onext = 
		world_entity->brushes.oprev = &world_entity->brushes;
	SetKeyValue (world_entity, "classname", "worldspawn");
	world_entity->eclass = Eclass_ForName ("worldspawn", true);

	g_pParentWnd->GetCamera()->Camera().angles[YAW] = 0;
	g_pParentWnd->GetCamera()->Camera().angles[PITCH] = 0;
	VectorCopy (vec3_origin, g_pParentWnd->GetCamera()->Camera().origin);
	g_pParentWnd->GetCamera()->Camera().origin[2] = 48;
	VectorCopy (vec3_origin, g_pParentWnd->GetXYWnd()->GetOrigin());

	Map_RestoreBetween ();

	Sys_UpdateWindows (W_ALL);
	modified = false;
}


/*
===========================================================

  REGION

===========================================================
*/
#if 0
qboolean	region_active;
vec3_t	region_mins = {MIN_WORLD_COORD, MIN_WORLD_COORD, MIN_WORLD_COORD};
vec3_t	region_maxs = {MAX_WORLD_COORD, MAX_WORLD_COORD, MAX_WORLD_COORD};

brush_t	*region_sides[6];

/*
===========
AddRegionBrushes

a regioned map will have temp walls put up at the region boundary
===========
*/
void AddRegionBrushes (void)
{
	vec3_t	mins, maxs;
	int		i;
	texdef_t	td;

	if (!region_active)
		return;

	memset (&td, 0, sizeof(td));
	strcpy (td.name, "REGION");

	mins[0] = region_mins[0] - 16;
	maxs[0] = region_mins[0] + 1;
	mins[1] = region_mins[1] - 16;
	maxs[1] = region_maxs[1] + 16;
	mins[2] = -2048;
	maxs[2] = 2048;
	region_sides[0] = Brush_Create (mins, maxs, &td);

	mins[0] = region_maxs[0] - 1;
	maxs[0] = region_maxs[0] + 16;
	region_sides[1] = Brush_Create (mins, maxs, &td);

	mins[0] = region_mins[0] - 16;
	maxs[0] = region_maxs[0] + 16;
	mins[1] = region_mins[1] - 16;
	maxs[1] = region_mins[1] + 1;
	region_sides[2] = Brush_Create (mins, maxs, &td);

	mins[1] = region_maxs[1] - 1;
	maxs[1] = region_maxs[1] + 16;
	region_sides[3] = Brush_Create (mins, maxs, &td);

	mins[0] = region_mins[0];
	maxs[0] = region_maxs[0];
	mins[1] = region_mins[1];
	maxs[1] = region_maxs[1];
	mins[2] = -2048;
	maxs[2] = -2048 + 16;
	region_sides[4] = Brush_Create (mins, maxs, &td);

	mins[2] = 2048;
	maxs[2] = 2048 + 16;
	region_sides[5] = Brush_Create (mins, maxs, &td);

	for (i=0 ; i<6 ; i++)
	{
		Brush_AddToList (region_sides[i], &selected_brushes);
		Entity_LinkBrush (world_entity, region_sides[i]);
		Brush_Build(region_sides[i]);
	}
}

void RemoveRegionBrushes (void)
{
	int		i;

	if (!region_active)
		return;
	for (i=0 ; i<6 ; i++)
		Brush_Free (region_sides[i]);
}

#endif/*
===========================================================

  REGION

===========================================================
*/

qboolean	region_active;
vec3_t	region_mins = {MIN_WORLD_COORD, MIN_WORLD_COORD, MIN_WORLD_COORD};
vec3_t	region_maxs = {MAX_WORLD_COORD, MAX_WORLD_COORD, MAX_WORLD_COORD};

brush_t	*region_sides[4];


float g_fMaxZ,g_fMinZ;
static void FillInWorldZBounds(void)
{
	g_fMaxZ = -MAX_WORLD_COORD;	// default as opposite-ends, so they'll get changed by anything
	g_fMinZ =  MAX_WORLD_COORD;
	
	for (brush_t *b = active_brushes.next ; b != &active_brushes ; b = b->next)
	{		
		if (g_fMaxZ < b->maxs[2])
			g_fMaxZ = b->maxs[2];
		if (g_fMinZ > b->mins[2])
			g_fMinZ = b->mins[2];
	}
}


/*
===========
AddRegionBrushes

a regioned map will have temp walls put up at the region boundary
===========
*/
void AddRegionBrushes (void)
{
	vec3_t	mins, maxs;
	int		i;
	texdef_t	td;

	if (!region_active)
		return;

	FillInWorldZBounds();

	memset (&td, 0, sizeof(td));
	strcpy (td.name, "REGION");	

	mins[0] = region_mins[0] - 16;
	maxs[0] = region_mins[0] + 1;
	mins[1] = region_mins[1] - 16;
	maxs[1] = region_maxs[1] + 16;
	mins[2] = g_fMinZ - 16;	// -MAX_WORLD_COORD/2;
	maxs[2] = g_fMaxZ + 16;	//  MAX_WORLD_COORD/2;
	region_sides[0] = Brush_Create (mins, maxs, &td);

	mins[0] = region_maxs[0] - 1;
	maxs[0] = region_maxs[0] + 16;
	region_sides[1] = Brush_Create (mins, maxs, &td);

	mins[0] = region_mins[0] - 16;
	maxs[0] = region_maxs[0] + 16;
	mins[1] = region_mins[1] - 16;
	maxs[1] = region_mins[1] + 1;
	region_sides[2] = Brush_Create (mins, maxs, &td);

	mins[1] = region_maxs[1] - 1;
	maxs[1] = region_maxs[1] + 16;
	region_sides[3] = Brush_Create (mins, maxs, &td);

	for (i=0 ; i<4 ; i++)
	{
		Brush_AddToList (region_sides[i], &selected_brushes);
		Entity_LinkBrush (world_entity, region_sides[i]);
		Brush_Build( region_sides[i] );
	}
}

void RemoveRegionBrushes (void)
{
	int		i;

	if (!region_active)
		return;
	for (i=0 ; i<4 ; i++)
		Brush_Free (region_sides[i]);
}


// new behaviour is to filter out stuff that's even partially outside the region, not just entirely
// ... (unless the owner ent has no origin)
//
qboolean Map_IsBrushFiltered (brush_t *b)
{
	bool bAllowOverlaps = true;

	if (b->owner)
	{
		if (strlen(ValueForKey(b->owner,"origin")))
			bAllowOverlaps = false;
	}


	int		i;

	if (bAllowOverlaps)
	{			
		for (i=0 ; i<3 ; i++)
		{
			if (b->mins[i] > region_maxs[i])
				return true;
			if (b->maxs[i] < region_mins[i])
				return true;
		}
	}
	else
	{
		for (i=0; i<3; i++)
		{
			if (b->maxs[i] > region_maxs[i])
				return true;
			if (b->mins[i] < region_mins[i])
				return true;
		}
	}

	return false;
}

/*
===========
Map_RegionOff

Other filtering options may still be on
===========
*/
void Map_RegionOff (void)
{
	brush_t	*b, *next;
	int			i;

	region_active = false;
	for (i=0 ; i<3 ; i++)
	{
		region_maxs[i] = MAX_WORLD_COORD;
		region_mins[i] = MIN_WORLD_COORD;
	}
	
	for (b=filtered_brushes.next ; b != &filtered_brushes ; b=next)
	{
		next = b->next;
		if (Map_IsBrushFiltered (b))
			continue;		// still filtered
		Brush_RemoveFromList (b);
    if (active_brushes.next == NULL || active_brushes.prev == NULL)
    {
      active_brushes.next = &active_brushes;
      active_brushes.prev = &active_brushes;
    }
		Brush_AddToList (b, &active_brushes);
	}

	Sys_UpdateWindows (W_ALL);
}

void Map_ApplyRegion (void)
{
	brush_t	*b, *next;

	region_active = true;
	for (b=active_brushes.next ; b != &active_brushes ; b=next)
	{
		next = b->next;
		if (!Map_IsBrushFiltered (b))
			continue;		// still filtered
		Brush_RemoveFromList (b);
		Brush_AddToList (b, &filtered_brushes);
	}

	Sys_UpdateWindows (W_ALL);
}


/*
========================
Map_RegionSelectedBrushes
========================
*/
void Map_RegionSelectedBrushes (void)
{
	Map_RegionOff ();

	if (selected_brushes.next == &selected_brushes)  // nothing selected
  {
    Sys_Printf("Tried to region with no selection...\n");
    return;
  }
	region_active = true;
	Select_GetBounds (region_mins, region_maxs);

	// move the entire active_brushes list to filtered_brushes
	filtered_brushes.next = active_brushes.next;
	filtered_brushes.prev = active_brushes.prev;
	filtered_brushes.next->prev = &filtered_brushes;
	filtered_brushes.prev->next = &filtered_brushes;

	// move the entire selected_brushes list to active_brushes
	active_brushes.next = selected_brushes.next;
	active_brushes.prev = selected_brushes.prev;
	active_brushes.next->prev = &active_brushes;
	active_brushes.prev->next = &active_brushes;

	// clear selected_brushes
	selected_brushes.next = selected_brushes.prev = &selected_brushes;

	Sys_UpdateWindows (W_ALL);
}


/*
===========
Map_RegionXY
===========
*/
void Map_RegionXY (void)
{
	Map_RegionOff ();

	FillInWorldZBounds();

	region_mins[0] = g_pParentWnd->GetXYWnd()->GetOrigin()[0] - 0.5 * g_pParentWnd->GetXYWnd()->Width() / g_pParentWnd->GetXYWnd()->Scale();
	region_maxs[0] = g_pParentWnd->GetXYWnd()->GetOrigin()[0] + 0.5 * g_pParentWnd->GetXYWnd()->Width() / g_pParentWnd->GetXYWnd()->Scale();
	region_mins[1] = g_pParentWnd->GetXYWnd()->GetOrigin()[1] - 0.5 * g_pParentWnd->GetXYWnd()->Height() / g_pParentWnd->GetXYWnd()->Scale();
	region_maxs[1] = g_pParentWnd->GetXYWnd()->GetOrigin()[1] + 0.5 * g_pParentWnd->GetXYWnd()->Height() / g_pParentWnd->GetXYWnd()->Scale();
	region_mins[2] = g_fMinZ - 64;	// -MAX_WORLD_COORD;	// +/- 64 is just for shits, rather than touching the surface
	region_maxs[2] = g_fMaxZ + 64;	// MAX_WORLD_COORD;
	Map_ApplyRegion ();
}

/*
===========
Map_RegionTallBrush
===========
*/
void Map_RegionTallBrush (void)
{
	brush_t	*b;

	if (!QE_SingleBrush ())
		return;

	b = selected_brushes.next;

	Map_RegionOff ();

	FillInWorldZBounds();

	VectorCopy (b->mins, region_mins);
	VectorCopy (b->maxs, region_maxs);
	region_mins[2] = g_fMinZ - 64;	// -MAX_WORLD_COORD;
	region_maxs[2] = g_fMaxZ + 64;	// MAX_WORLD_COORD;

	Select_Delete ();
	Map_ApplyRegion ();
}
/*
===========
Map_RegionBrush
===========
*/
void Map_RegionBrush (void)
{
	brush_t	*b;

	if (!QE_SingleBrush ())
		return;

	b = selected_brushes.next;

	Map_RegionOff ();

	VectorCopy (b->mins, region_mins);
	VectorCopy (b->maxs, region_maxs);

	Select_Delete ();
	Map_ApplyRegion ();
}


/*
=============
Q_strncpyz
 
Safe strncpy that ensures a trailing zero
=============
*/
void Q_strncpyz( char *dest, const char *src, int destsize ) 
{
	strncpy( dest, src, destsize-1 );
    dest[destsize-1] = 0;
}

void Q3_UniqueTargetName(CString& rStr, char *targNameNum, int &iHighest, qboolean targNameOnly)
{
	char	number[16];
	char	targName[128];
	int		len, numStart, i;
	int		maxtarg = 0;

	if (!targNameNum || !targNameNum[0])
	{//No intital value!
		return;
	}

	Q_strncpyz(targName, targNameNum, sizeof(targName));
	len = strlen(targName);
	numStart = 0;
	for(i = 0; i < len; i++)
	{
		if( atoi(&targName[i]) == 0 )//Not a number
		{
			numStart = i;
		}
		else
		{
			break;
		}
	}

	//Remember the name and number seperately
	Q_strncpyz(number, &targName[numStart + 1], sizeof(number));
	//truncate the name
	targName[numStart + 1] = 0;
	len = numStart;
	
	numStart += 1;

	for (entity_t* e=entities.next ; e != &entities ; e=e->next)
	{
		char* tn;

		if(targNameOnly)
		{//looking for new targetname only
			tn = ValueForKey (e, "targetname");
			if (tn && tn[0])
			{
				if(strncmp(tn, targName, len) == 0)
				{//Same name
					int targetnum = atoi(tn + numStart);
					if (targetnum > maxtarg)
					{
						maxtarg = targetnum;
					}
				}
			}
		}
		else
		{//looking for new target
			tn = ValueForKey (e, "target");
			if (tn && tn[0])
			{
			  if(strncmp(tn, targName, len) == 0)
			  {//Same name
				  int targetnum = atoi(tn+numStart);
				  if (targetnum > maxtarg)
				  {
					  maxtarg = targetnum;
				  }
			  }
			}

			tn = ValueForKey (e, "target2");
			if (tn && tn[0])
			{
			  if(strncmp(tn, targName, len) == 0)
			  {//Same name
				  int targetnum = atoi(tn+numStart);
				  if (targetnum > maxtarg)
				  {
					  maxtarg = targetnum;
				  }
			  }
			}

			tn = ValueForKey (e, "target3");
			if (tn && tn[0])
			{
			  if(strncmp(tn, targName, len) == 0)
			  {//Same name
				  int targetnum = atoi(tn+numStart);
				  if (targetnum > maxtarg)
				  {
					  maxtarg = targetnum;
				  }
			  }
			}

			tn = ValueForKey (e, "target4");
			if (tn && tn[0])
			{
			  if(strncmp(tn, targName, len) == 0)
			  {//Same name
				  int targetnum = atoi(tn+numStart);
				  if (targetnum > maxtarg)
				  {
					  maxtarg = targetnum;
				  }
			  }
			}
		}
	}

	if (maxtarg < iHighest)
	{
		maxtarg = iHighest++;
	}
	else
	{
		iHighest = maxtarg+1;
	}

	
  rStr.Format("%s%i", targName, maxtarg+1);
}

void UniqueTargetName(CString& rStr, int &iHighest)
{
	// make a unique target value
	int maxtarg = 0;
	for (entity_t* e=entities.next ; e != &entities ; e=e->next)
	{
		char* tn = ValueForKey (e, "targetname");

		if (tn && tn[0])
		{
			int targetnum = atoi(tn+1);
			if (targetnum > maxtarg)
				maxtarg = targetnum;
		}
#ifndef QUAKE3
		else
#endif
		{
			  tn = ValueForKey (e, "target");
			  if (tn && tn[0])
			  {
				  int targetnum = atoi(tn+1);
				  if (targetnum > maxtarg)
					  maxtarg = targetnum;
			  }
#ifdef QUAKE3
			  tn = ValueForKey (e, "target2");
			  if (tn && tn[0])
			  {
				  int targetnum = atoi(tn+1);
				  if (targetnum > maxtarg)
					  maxtarg = targetnum;
			  }

			  tn = ValueForKey (e, "target3");
			  if (tn && tn[0])
			  {
				  int targetnum = atoi(tn+1);
				  if (targetnum > maxtarg)
					  maxtarg = targetnum;
			  }

			  tn = ValueForKey (e, "target4");
			  if (tn && tn[0])
			  {
				  int targetnum = atoi(tn+1);
				  if (targetnum > maxtarg)
					  maxtarg = targetnum;
			  }
#endif
		}
	}
#ifdef QUAKE3
	if (maxtarg < iHighest)
	{
		maxtarg = iHighest++;
	}
	else
	{
		iHighest = maxtarg+1;
	}
#endif

	
  rStr.Format("t%i", maxtarg+1);
}

//
//================
//Map_ImportFile
//================
//
#ifdef QUAKE3
extern bool gbInhibitDupTargetCorrection;
#endif

void Map_ImportBuffer (char* buf)
{
	entity_t* ent;
  brush_t* b = NULL;
  CPtrArray ptrs;

  Select_Deselect();

	g_qeglobals.d_parsed_brushes = 0;
  if (buf)
  {
    CMapStringToString mapStr;
	  StartTokenParsing (buf);
	  g_qeglobals.d_num_entities = 0;

#ifdef QUAKE3		
	  entity_t *pPrevLastEnt = entities.prev;	// record this so later on we know who was the last existing brush before the new ones
#endif

	  while (1)
	  {

      // use the selected brushes list as it's handy
		  //ent = Entity_Parse (false, &selected_brushes);
		  ent = Entity_Parse (false, &active_brushes);
		  if (!ent)
			  break;
		  if (!strcmp(ValueForKey (ent, "classname"), "worldspawn"))
		  {
        // world brushes need to be added to the current world entity

		    b=ent->brushes.onext;
        while (b && b != &ent->brushes)
        {
          brush_t* bNext = b->onext;
          Entity_UnlinkBrush(b);
          Entity_LinkBrush(world_entity, b);
          ptrs.Add(b);
          b = bNext;
        }
		  }
		  else
		  {
        // the following bit remaps conflicting target/targetname key/value pairs
        
		
        CString strKey;
        CString strTarget;

		int iHighest = 0;	


#ifdef QUAKE3
		if (!gbInhibitDupTargetCorrection)	// a special request by Oz to make it easier to cut/paste 15(?!?!)-piece doors
#endif
		{

			CString str = ValueForKey(ent, "targetname");
			if (str.GetLength() > 0)
			{
			  if (FindEntity("targetname", str.GetBuffer(0)))
			  {
				if (!mapStr.Lookup(str, strKey))
				{
	#ifdef QUAKE3		
				  Q3_UniqueTargetName(strKey, str.GetBuffer(0), iHighest, true);
	#else
				  UniqueTargetName(strKey, iHighest);
	#endif
				  mapStr.SetAt(str, strKey);
				}
				SetKeyValue(ent, "targetname", strKey.GetBuffer(0));
			  }
			}

			str = ValueForKey(ent, "target");
			if (str.GetLength() > 0)
			{
			  if (FindEntity("target", str.GetBuffer(0)))
			  {
				if (!mapStr.Lookup(str, strKey))
				{
	#ifdef QUAKE3		
				  Q3_UniqueTargetName(strKey, str.GetBuffer(0), iHighest, false);
	#else
				  UniqueTargetName(strKey, iHighest);
	#endif
				  mapStr.SetAt(str, strKey);
				}
				strTarget = strKey;
				SetKeyValue(ent, "target", strTarget.GetBuffer(0));
			  }
			}

	#ifdef QUAKE3		
			str = ValueForKey(ent, "target2");
			if (str.GetLength() > 0)
			{
			  if (FindEntity("target2", str.GetBuffer(0)))
			  {
				if (!mapStr.Lookup(str, strKey))
				{
				  Q3_UniqueTargetName(strKey, str.GetBuffer(0), iHighest, false);
				  mapStr.SetAt(str, strKey);
				}
				strTarget = strKey;
				SetKeyValue(ent, "target2", strTarget.GetBuffer(0));
			  }
			}

			str = ValueForKey(ent, "target3");
			if (str.GetLength() > 0)
			{
			  if (FindEntity("target3", str.GetBuffer(0)))
			  {
				if (!mapStr.Lookup(str, strKey))
				{
				  Q3_UniqueTargetName(strKey, str.GetBuffer(0), iHighest, false);
				  mapStr.SetAt(str, strKey);
				}
				strTarget = strKey;
				SetKeyValue(ent, "target3", strTarget.GetBuffer(0));
			  }
			}

			str = ValueForKey(ent, "target4");
			if (str.GetLength() > 0)
			{
			  if (FindEntity("target4", str.GetBuffer(0)))
			  {
				if (!mapStr.Lookup(str, strKey))
				{
				  Q3_UniqueTargetName(strKey, str.GetBuffer(0), iHighest, false);
				  mapStr.SetAt(str, strKey);
				}
				strTarget = strKey;
				SetKeyValue(ent, "target4", strTarget.GetBuffer(0));
			  }
			}
	#endif
		}

		  // add the entity to the end of the entity list
			  ent->next = &entities;
			  ent->prev = entities.prev;
			  entities.prev->next = ent;
			  entities.prev = ent;
			  g_qeglobals.d_num_entities++;

	      for (b=ent->brushes.onext ; b != &ent->brushes ; b=b->onext)
        {
          ptrs.Add(b);
        }

		  }
    }

#ifdef QUAKE3		
	// now go through the new/cloned brushes and remove any target/targetnames that aren't aprt of our nice little new group
	//
	if (!gbInhibitDupTargetCorrection)	// a special request by Oz to make it easier to cut/paste 15(?!?!)-piece doors
	for ( entity_t *ent2 = entities.prev; ent2 != pPrevLastEnt; ent2 = ent2->prev )
	{
		CString str;
		/*//FIXME: Only do this if you're pasting in?
		// if no-one is targeting this ent2, zap it's targetname...
		//
		str = ValueForKey(ent2, "targetname");
        if (str.GetLength() > 0)
        {
          if (!FindEntity("target", str.GetBuffer(0)) &&
			  !FindEntity("target2", str.GetBuffer(0)) &&
			  !FindEntity("target3", str.GetBuffer(0)) &&
			  !FindEntity("target4", str.GetBuffer(0))
			  )
          {
            DeleteKey(ent2, "targetname");
          }
        }
		// if ent2 is targeting something that's not part of our new little group then lose the target info...
		//
		str = ValueForKey(ent2, "target");
        if (str.GetLength() > 0)
        {
          if (!FindEntity("targetname", str.GetBuffer(0)))
          {
            DeleteKey(ent2, "target");
          }
        }

		str = ValueForKey(ent2, "target2");
        if (str.GetLength() > 0)
        {
          if (!FindEntity("targetname", str.GetBuffer(0)))
          {
            DeleteKey(ent2, "target2");
          }
        }

		str = ValueForKey(ent2, "target3");
        if (str.GetLength() > 0)
        {
          if (!FindEntity("targetname", str.GetBuffer(0)))
          {
            DeleteKey(ent2, "target3");
          }
        }

		str = ValueForKey(ent2, "target4");
        if (str.GetLength() > 0)
        {
          if (!FindEntity("targetname", str.GetBuffer(0)))
          {
            DeleteKey(ent2, "target4");
          }
        }
		*/

		// now shuffle up the target ptrs so no gaps...
		//		
		bool bShuffleHappened;
		do
		{
			bShuffleHappened = false;

			CString str2;

			if (str = ValueForKey(ent2, "target4"))
			{
				if (str.GetLength() > 0)
				{
					if (str2 = ValueForKey(ent2, "target3"))
					{
						if (str2.GetLength() == 0)
						{
							SetKeyValue(ent2, "target3", str.GetBuffer(0));
							bShuffleHappened = true;
							DeleteKey(ent2,"target4");
						}
					}
				}
			}

			if (str = ValueForKey(ent2, "target3"))
			{
				if (str.GetLength() > 0)
				{
					if (str2 = ValueForKey(ent2, "target2"))
					{
						if (str2.GetLength() == 0)
						{
							SetKeyValue(ent2, "target2", str.GetBuffer(0));
							bShuffleHappened = true;
							DeleteKey(ent2,"target3");
						}
					}
				}
			}

			if (str = ValueForKey(ent2, "target2"))
			{
				if (str.GetLength() > 0)
				{
					if (str2 = ValueForKey(ent2, "target"))
					{
						if (str2.GetLength() == 0)
						{
							SetKeyValue(ent2, "target", str.GetBuffer(0));
							bShuffleHappened = true;
							DeleteKey(ent2,"target2");
						}
					}
				}
			}
		}
		while (bShuffleHappened);
	}
#endif

	}

	g_bScreenUpdates = false;	//deep inside Select_Brush's call chain, it might call UpdateWindows
  for (int i = 0; i < ptrs.GetSize(); i++)
  {
    Brush_Build(reinterpret_cast<brush_t*>(ptrs[i]), true, false);
    Select_Brush(reinterpret_cast<brush_t*>(ptrs[i]));
  }
	g_bScreenUpdates = true;

  ptrs.RemoveAll();

	Sys_UpdateWindows (W_ALL);
  Sys_MarkMapModified();
	modified = true;
}


//
//================
//Map_ImportFile
//================
//
void Map_ImportFile (char *filename)
{
  char* buf;
	char temp[1024];
	Sys_BeginWait ();
	QE_ConvertDOSToUnixName( temp, filename );
  if (LoadFile (filename, (void **)&buf) != -1)
  {
    Map_ImportBuffer(buf);
    free(buf);
    Map_BuildBrushData();
  }
	Sys_UpdateWindows (W_ALL);
	modified = true;
	Sys_EndWait();
}

//
//===========
//Map_SaveSelected
//===========
//
// Saves selected world brushes and whole entities with partial/full selections
//
void Map_SaveSelected(char* pFilename)
{
	entity_t	*e, *next;
	FILE *f;
	char temp[1024];
	int count;

	QE_ConvertDOSToUnixName(temp, pFilename);
	f = fopen(pFilename, "w");

	if (!f)
	{
		Sys_Printf ("ERROR!!!! Couldn't open %s\n", pFilename);
		return;
	}

	// write world entity first
	Entity_WriteSelected(world_entity, f);

	// then write all other ents
	count = 1;
	for (e=entities.next ; e != &entities ; e=next)
	{
  	fprintf (f, "// entity %i\n", count);
   	count++;
 		Entity_WriteSelected(e, f);
		next = e->next;
	}
	fclose (f);
}


//
//===========
//Map_SaveSelected
//===========
//
// Saves selected world brushes and whole entities with partial/full selections
//
void Map_SaveSelected(CMemFile* pMemFile, CMemFile* pPatchFile)
{
	entity_t	*e, *next;
	int count;
  
	// write world entity first
	Entity_WriteSelected(world_entity, pMemFile);

	// then write all other ents
	count = 1;
	for (e=entities.next ; e != &entities ; e=next)
	{
    MemFile_fprintf(pMemFile, "// entity %i\n", count);
   	count++;
 		Entity_WriteSelected(e, pMemFile);
		next = e->next;
	}

  //if (pPatchFile)
  //  Patch_WriteFile(pPatchFile);
}


void MemFile_fprintf(CMemFile* pMemFile, const char* pText, ...)
{
  char Buffer[4096];
  va_list args;
	va_start (args,pText);
  vsprintf(Buffer, pText, args);
  pMemFile->Write(Buffer, strlen(Buffer));
}