#include "stdafx.h"
#include "qe3.h"
#include "oddbits.h"
#include <assert.h>

int	GetNumKeys(entity_t *ent)
{
	int iCount = 0;
	for (epair_t* ep=ent->epairs ; ep ; ep=ep->next)
	{
		iCount++;
	}

	return iCount;
}

char *GetKeyString(entity_t *ent, int iIndex)
{
	for (epair_t* ep=ent->epairs ; ep ; ep=ep->next)
	{
		if (!iIndex--)
			return ep->key;
	}

	assert(0);	// why do I not find it surprising that this didn't compile until I added the header, ie assert never used?
	return NULL;
}

char 	*ValueForKey (entity_t *ent, LPCSTR key)
{
	epair_t	*ep;
	
	for (ep=ent->epairs ; ep ; ep=ep->next)
		if (!strcmp (ep->key, key) )
			return ep->value;
	return "";
}

void TrackMD3Angles(entity_t *e, LPCSTR key, LPCSTR value)
{
  if (strcmpi(key, "angle") != 0)
  {
    return;
  }

#ifdef SOF
  if (e->eclass->fixedsize && 
		(
			(strnicmp(e->eclass->name, "misc_",			5) == 0) ||
			(strnicmp(e->eclass->name, "light_",		6) == 0) || 
			(strnicmp(e->eclass->name, "m_",			2) == 0) ||
			(strnicmp(e->eclass->name, "item_weapon_",	12)== 0) ||
			(strnicmp(e->eclass->name, "item_ammo_",	10)== 0)
		)
	  )
#else
  if (e->eclass->fixedsize && strnicmp (e->eclass->name, "misc_model",10) == 0)
#endif
  {
    float a = FloatForKey (e, "angle");
    float b = atof(value);
    if (a != b)
    {
      vec3_t vAngle;
      vAngle[0] = vAngle[1] = 0;
      vAngle[2] = -a;
      Brush_Rotate(e->brushes.onext, vAngle, e->origin, true);
      vAngle[2] = b;
      Brush_Rotate(e->brushes.onext, vAngle, e->origin, true);

#ifdef QUAKE3
	  //
	  // auto assign new bounding box to model?
	  //
	  char *_p = ValueForKey(e, "mins");
	  char *_p2= ValueForKey(e, "maxs");
	  char *_p3= ValueForKey(e, sKEYFIELD_AUTOBOUND);
	  //
	  // if either key is missing then update both... (or if they do exist, but were only set by this code in the first place and therefore can be overwritten)
	  //
	  if ( ((strlen(_p) == 0) || (strlen(_p2) == 0)) || strlen(_p3) )
	  {
		SetKeyValue (e, sKEYFIELD_AUTOBOUND, "1");

		vec3_t vMins,vMaxs;

		VectorSubtract(e->brushes.onext->mins, e->origin, vMins);
		VectorSubtract(e->brushes.onext->maxs, e->origin, vMaxs);

		SetKeyValue (e, "mins", va("%i %i %i", (int)vMins[0], (int)vMins[1], (int)vMins[2]));
		SetKeyValue (e, "maxs", va("%i %i %i", (int)vMaxs[0], (int)vMaxs[1], (int)vMaxs[2]));
	  }
#endif


    }
  }
}

// Scale object bounding box
void TrackMD3Scale(entity_t *e, LPCSTR key, LPCSTR value)
{
	float oldscale,scale;

	if (strcmpi(key, "scale") != 0)
	{
		return;
	}

	if (e->eclass->fixedsize && ((strnicmp(e->eclass->name, "misc_",5) == 0) ||
			(strnicmp(e->eclass->name, "light_",6) == 0)))
	{
		oldscale = FloatForKey (e, "scale");
		scale = atof(value);
		if (oldscale != scale)	// Value unchanged??????
		{
	      Brush_Scale2(e->eclass,e->brushes.onext, scale,e->origin,true);

			float a = FloatForKey (e, "angle");
			if (a)
			{
				// Re-rotate bbox
				vec3_t vAngle;
				vAngle[0] = vAngle[1] = 0;
				vAngle[2] = a;
				Brush_Rotate(e->brushes.onext, vAngle, e->origin, true);
			}
		} 
	}
}


// make sure you add any new tracking functions inside the IF, this is important!
//
void SetKeyValue (entity_t *ent, LPCSTR key, LPCSTR value, bool bDoTracking/*=true*/)
{
	epair_t	*ep;

	if (ent == NULL)
		return;

	if (!key || !key[0])
		return;

	if (ent == world_entity && strcmp(key,sKEYFIELD_GROUPNAME)==0)
		return;	// dumb to do this

	if (bDoTracking)
	{
		#ifdef SOF
		TrackMD3Scale(ent, key, value);
		#endif

		TrackMD3Angles(ent, key, value);
	}

	for (ep=ent->epairs ; ep ; ep=ep->next)
	{
		if (!strcmp (ep->key, key) )
		{
			free (ep->value);
			ep->value = (char*)qmalloc(strlen(value)+1);
			strcpy (ep->value, value);
			return;
		}
	}
	ep = (epair_t*)qmalloc (sizeof(*ep));
	ep->next = ent->epairs;
	ent->epairs = ep;
	ep->key = (char*)qmalloc(strlen(key)+1);
	strcpy (ep->key, key);
	ep->value = (char*)qmalloc(strlen(value)+1);
	strcpy (ep->value, value);
}

void 	DeleteKey (entity_t *ent, LPCSTR key)
{
	epair_t	**ep, *next;
	
	ep = &ent->epairs;
	while (*ep)
	{
		next = *ep;
		if ( !strcmp (next->key, key) )
		{
			*ep = next->next;
			free(next->key);
			free(next->value);
			free(next);
			return;
		}
		ep = &next->next;
	}
}


float	FloatForKey (entity_t *ent, LPCSTR key)
{
	char	*k;
	
	k = ValueForKey (ent, key);
	return atof(k);
}

int IntForKey (entity_t *ent, LPCSTR key)
{
	char	*k;
	
	k = ValueForKey (ent, key);
	return atoi(k);
}

// now returns success/fail bool...
//
bool GetVectorForKey (entity_t *ent, LPCSTR key, vec3_t vec)
{
	char	*k;
	
	k = ValueForKey (ent, key);	
	int i = sscanf (k, "%f %f %f", &vec[0], &vec[1], &vec[2]);

	return !!strlen(k);
//	return !!(i==3);	// actually this would probably be more accurate, but I'll leave it as above for now
}


/*
===============
Entity_Free

Frees the entity and any brushes is has.
The entity is removed from the global entities list.
===============
*/
void Entity_Free (entity_t *e)
{
	epair_t	*ep, *next;

	while (e->brushes.onext != &e->brushes)
		Brush_Free (e->brushes.onext);

	if (e->next)
	{
		e->next->prev = e->prev;
		e->prev->next = e->next;
	}

	for (ep = e->epairs ; ep ; ep=next)
	{
		next = ep->next;
    free (ep->key);
    free (ep->value);
		free (ep);
	}
	free (e);
}

/*
=================
ParseEpair
=================
*/
epair_t *ParseEpair (void)
{
	epair_t	*e;
	
	e = (epair_t*)qmalloc (sizeof(*e));
	
	e->key = (char*)qmalloc(strlen(token)+1);
	strcpy (e->key, token);

	GetToken (false);
	e->value = (char*)qmalloc(strlen(token)+1);
	strcpy (e->value, token);

	return e;
}

/*
================
Entity_Parse

If onlypairs is set, the classname info will not
be looked up, and the entity will not be added
to the global list.  Used for parsing the project.
================
*/
entity_t	*Entity_Parse (qboolean onlypairs, brush_t* pList)
{
	entity_t	*ent;
	eclass_t	*e;
	brush_t		*b;
	vec3_t		mins, maxs;
	epair_t		*ep;
	qboolean	has_brushes;
#ifdef SOF
	float		scale;
#endif

	if (!GetToken (true))
		return NULL;

	if (strcmp (token, "{") )
		Error ("ParseEntity: { not found");
	
	ent = (entity_t*)qmalloc (sizeof(*ent));
	ent->brushes.onext = ent->brushes.oprev = &ent->brushes;

	do
	{
		if (!GetToken (true))
    {
			Warning ("ParseEntity: EOF without closing brace");
      return NULL;
    }
		if (!strcmp (token, "}") )
			break;
		if (!strcmp (token, "{") )
		{
			b = Brush_Parse ();
      if (b != NULL)
      {
			  b->owner = ent;

			  // add to the end of the entity chain
			  b->onext = &ent->brushes;
			  b->oprev = ent->brushes.oprev;
			  ent->brushes.oprev->onext = b;
			  ent->brushes.oprev = b;
      }
      else
      {
        break;
      }
    
		}
		else
		{
			ep = ParseEpair ();
			{
				// update: the original code here may have been simple, but it meant that every map load/save
				//	the key/value pair fields were reversed in the save file, which messes up SourceSafe when it
				//	tries to delta the two versions during check-in... -slc
#if 0
				ep->next = ent->epairs;
				ent->epairs = ep;
#else
				// join this onto the END of the chain instead...
				//
				if (ent->epairs == NULL)	// special case for if there isn't a chain yet... :-)
				{
					ep->next = ent->epairs;
					ent->epairs = ep;
				}
				else
				{						
					for (epair_t* ep2 = ent->epairs ; ep2 ; ep2=ep2->next)
					{
						if (ep2->next == NULL)
						{
							// found the end, so...
							//
							ep2->next = ep;							
							ep->next = NULL;
							break;
						}
					}
				}
#endif
			}
		}
	} while (1);

	if (onlypairs)
		return ent;

	if (ent->brushes.onext == &ent->brushes)
		has_brushes = false;
	else
		has_brushes = true;

	GetVectorForKey (ent, "origin", ent->origin);

	e = Eclass_ForName (ValueForKey (ent, "classname"), has_brushes);
	ent->eclass = e;
	if (e->fixedsize)
	{	// fixed size entity
		if (ent->brushes.onext != &ent->brushes)
		{
			printf ("Warning: Fixed size entity with brushes\n");
#if 0
			while (ent->brushes.onext != &ent->brushes)
			{	// FIXME: this will free the entity and crash!
				Brush_Free (b);
			}
#endif
      ent->brushes.next = ent->brushes.prev = &ent->brushes;
		}

    // create a custom brush
		VectorAdd (e->mins, ent->origin, mins);
		VectorAdd (e->maxs, ent->origin, maxs);

    float a = 0;
    if (strnicmp(e->name, "misc_model",10) == 0)
    {
      char* p = ValueForKey(ent, "model");
      if (p != NULL && strlen(p) > 0)
      {
        vec3_t vMin, vMax;
        a = FloatForKey (ent, "angle");
		gEntityToSetBoundsOf = ent;
        if (GetCachedModel(ent, p, vMin, vMax))
        {
	      // create a custom brush
	      VectorAdd (ent->md3Class->mins, ent->origin, mins);
	      VectorAdd (ent->md3Class->maxs, ent->origin, maxs);
        }
      }
    }
#ifdef SOF
	if (strnicmp(e->name, "misc_",			5) == 0 ||
		strnicmp(e->name, "light_",			6) == 0 ||
		strnicmp(e->name, "m_",				2) == 0 ||
		strnicmp(e->name, "item_weapon_",	12)== 0 ||
		strnicmp(e->name, "item_ammo_",		10)== 0
		)
		a = FloatForKey (ent, "angle");
#endif		
		b = Brush_Create (mins, maxs, &e->texdef);
///////
		b->owner = ent;

		b->onext = ent->brushes.onext;
		b->oprev = &ent->brushes;
		ent->brushes.onext->oprev = b;
		ent->brushes.onext = b;
///////
		Brush_Build(b);

#ifdef SOF
		scale = FloatForKey (ent, "scale");
		if (scale)
		{
		  Brush_Scale2(e, b, scale, ent->origin,false);
		}
#endif

//		if (a)
//		{
//			vec3_t vAngle;
//			vAngle[0] = vAngle[1] = 0;
//			vAngle[2] = a;
//			Brush_Rotate(b, vAngle, ent->origin, false);
//		}

/*
		b->owner = ent;

		b->onext = ent->brushes.onext;
		b->oprev = &ent->brushes;
		ent->brushes.onext->oprev = b;
		ent->brushes.onext = b;
*/
		// do this AFTER doing the brush stuff just above, so don't join to the other "if (a)"...
		//
		if (a)
		{
			// pick any old value to rotate away to, then back from, to avoid 0/360 weirdness on key-compares
			//
			SetKeyValue(ent, "angle", "0",			false);	// false = no tracking, ie just set the angle and nothing else			
			SetKeyValue(ent, "angle", va("%g",a),	true);	// true = do tracking, ie actually do the brush rotate
		}
	}
	else
	{	// brush entity
		if (ent->brushes.next == &ent->brushes)
			printf ("Warning: Brush entity with no brushes\n");
	}

	// add all the brushes to the main list
  if (pList)
  {
	  for (b=ent->brushes.onext ; b != &ent->brushes ; b=b->onext)
    {
		  b->next = pList->next;
		  pList->next->prev = b;
		  b->prev = pList;
		  pList->next = b;
    }
  }

	return ent;
}

void VectorMidpoint(vec3_t va, vec3_t vb, vec3_t& out)
{
  for (int i = 0; i < 3; i++)
    out[i] = va[i] + ((vb[i] - va[i]) / 2);
}


/*
============
Entity_Write
============
*/
void Entity_Write (entity_t *e, FILE *f, qboolean use_region)
{
	epair_t		*ep;
	brush_t		*b;
	vec3_t		origin;
	char		text[128];
	int			count;

	// if none of the entities brushes are in the region,
	// don't write the entity at all
	if (use_region)
	{
		// in region mode, save the camera position as playerstart
		if ( !strcmp(ValueForKey (e, "classname"), "info_player_start") )
		{
			// see if this ent is entirely within the region first...
			//
			bool bWithinRegion = true;
			for (b=e->brushes.onext ; b != &e->brushes ; b=b->onext)
				if (Map_IsBrushFiltered(b))
					bWithinRegion = false;

			// if the player_start is NOT within the region, then use the camera position instead, else keep it...
			//
			if (!bWithinRegion || !GetYesNo("\"player_info_start\" found within region, keep it?\n\n( 'NO' will generate a new one from the current camera position )"))
			{
				fprintf (f, "{\n");
				fprintf (f, "\"classname\" \"info_player_start\"\n");
				fprintf (f, "\"origin\" \"%i %i %i\"\n", (int)g_pParentWnd->GetCamera()->Camera().origin[0],
					(int)g_pParentWnd->GetCamera()->Camera().origin[1], (int)g_pParentWnd->GetCamera()->Camera().origin[2]);
				fprintf (f, "\"angle\" \"%i\"\n", (int)g_pParentWnd->GetCamera()->Camera().angles[YAW]);
				fprintf (f, "}\n");
				return;
			}
		}

		for (b=e->brushes.onext ; b != &e->brushes ; b=b->onext)
			if (!Map_IsBrushFiltered(b))
				break;	// got one

		if (b == &e->brushes)
			return;		// nothing visible
	}

	// if fixedsize, calculate a new origin based on the current
	// brush position
	if (e->eclass->fixedsize)
	{
#ifdef SOF
	// I know these 2 do the same thing, but if that VectorSubtract ever gets re-instated it'll break ours -slc
	if (strnicmp(e->eclass->name, "misc_",			5) == 0 ||
		strnicmp(e->eclass->name, "light_",			6) == 0 ||
		strnicmp(e->eclass->name, "m_",				2) == 0 ||
		strnicmp(e->eclass->name, "item_weapon_",	12)== 0 ||
		strnicmp(e->eclass->name, "item_ammo_",		10)== 0
		)
	{
		VectorCopy(e->origin, origin);
	}
#else
    if (strnicmp(e->eclass->name, "misc_model",10) == 0 && e->md3Class != NULL)
    {
      VectorCopy(e->origin, origin);
		  //VectorSubtract (e->brushes.onext->mins, e->md3Class->mins, origin);
    }
#endif
    else
    {
		  VectorSubtract (e->brushes.onext->mins, e->eclass->mins, origin);
    }
    sprintf (text, "%i %i %i", (int)origin[0], (int)origin[1], (int)origin[2]);
		SetKeyValue (e, "origin", text);
	}

	fprintf (f, "{\n");
	for (ep = e->epairs ; ep ; ep=ep->next)
		fprintf (f, "\"%s\" \"%s\"\n", ep->key, ep->value);

	if (!e->eclass->fixedsize)
	{
		count = 0;
		for (b=e->brushes.onext ; b != &e->brushes ; b=b->onext)
		{
			if (!use_region || !Map_IsBrushFiltered (b))
      {
				fprintf (f, "// brush %i\n", count);
				count++;
				Brush_Write (b, f);
			}
		}
	}
	fprintf (f, "}\n");
}



qboolean IsBrushSelected(brush_t* bSel)
{
	for (brush_t* b = selected_brushes.next ;b != NULL && b != &selected_brushes; b = b->next)
  {
    if (b == bSel)
      return true;
  }
  return false;
}

//
//============
//Entity_WriteSelected
//============
//
void Entity_WriteSelected(entity_t *e, FILE *f)
{
	epair_t		*ep;
	brush_t		*b;
	vec3_t		origin;
	char		text[128];
	int			count;

	for (b=e->brushes.onext ; b != &e->brushes ; b=b->onext)
		if (IsBrushSelected(b))
			break;	// got one

	if (b == &e->brushes)
		return;		// nothing selected

	// if fixedsize, calculate a new origin based on the current
	// brush position
	if (e->eclass->fixedsize)
	{
#ifdef SOF
	// I suspect these 2 will end up doing the same thing, but for now that VectorSubtract will break ours -slc
	if (strnicmp(e->eclass->name, "misc_",			5) == 0 ||
		strnicmp(e->eclass->name, "light_",			6) == 0 ||
		strnicmp(e->eclass->name, "m_",				2) == 0 ||
		strnicmp(e->eclass->name, "item_weapon_",	12)== 0 ||
		strnicmp(e->eclass->name, "item_ammo_",		10)== 0
		)
	{
		VectorCopy(e->origin, origin);
	}
#else
    if (strnicmp(e->eclass->name, "misc_model",10) == 0 && e->md3Class != NULL)
    {
		// Bugfix? I'm guessing this should have been the same as in Entity_Write, ie the copy version..... -slc
		//VectorCopy(e->origin, origin);	
		VectorSubtract (e->brushes.onext->mins, e->md3Class->mins, origin);
    }
#endif
    else
    {
		  VectorSubtract (e->brushes.onext->mins, e->eclass->mins, origin);
    }
    sprintf (text, "%i %i %i", (int)origin[0], (int)origin[1], (int)origin[2]);
		SetKeyValue (e, "origin", text);
	}

  fprintf (f, "{\n");
	for (ep = e->epairs ; ep ; ep=ep->next)
	  fprintf (f, "\"%s\" \"%s\"\n", ep->key, ep->value);

  if (!e->eclass->fixedsize)
  {
	  count = 0;
	  for (b=e->brushes.onext ; b != &e->brushes ; b=b->onext)
	  {
		  if (IsBrushSelected(b))
		  {
			  fprintf (f, "// brush %i\n", count);
			  count++;
			  Brush_Write (b, f);
		  }
	  }
  }
	fprintf (f, "}\n");
}


//
//============
//Entity_WriteSelected to a CMemFile
//============
//
void Entity_WriteSelected(entity_t *e, CMemFile* pMemFile)
{
	epair_t		*ep;
	brush_t		*b;
	vec3_t		origin;
	char		text[128];
	int			count;

	for (b=e->brushes.onext ; b != &e->brushes ; b=b->onext)
		if (IsBrushSelected(b))
			break;	// got one

	if (b == &e->brushes)
		return;		// nothing selected

	// if fixedsize, calculate a new origin based on the current
	// brush position
	if (e->eclass->fixedsize)
	{
#ifdef SOF
	// I suspect these 2 will end up doing the same thing, but for now that VectorSubtract will break ours -slc
	if (strnicmp(e->eclass->name, "misc_",			5) == 0 ||
		strnicmp(e->eclass->name, "light_",			6) == 0 ||
		strnicmp(e->eclass->name, "m_",				2) == 0 ||
		strnicmp(e->eclass->name, "item_weapon_",	12)== 0 ||
		strnicmp(e->eclass->name, "item_ammo_",		10)== 0
		)
	{
		VectorCopy(e->origin, origin);
	}
#else
    if (strnicmp(e->eclass->name, "misc_model",10) == 0 && e->md3Class != NULL)
    {
		// Bugfix? I'm guessing this should have been the same as in Entity_Write, ie the copy version..... -slc
		//VectorCopy(e->origin, origin);	
		VectorSubtract (e->brushes.onext->mins, e->md3Class->mins, origin);
    }
#endif
    else
    {
		  VectorSubtract (e->brushes.onext->mins, e->eclass->mins, origin);
    }
    sprintf (text, "%i %i %i", (int)origin[0], (int)origin[1], (int)origin[2]);
		SetKeyValue (e, "origin", text);
	}

  MemFile_fprintf(pMemFile, "{\n");
	for (ep = e->epairs ; ep ; ep=ep->next)
	  MemFile_fprintf(pMemFile, "\"%s\" \"%s\"\n", ep->key, ep->value);

  if (!e->eclass->fixedsize)
  {
	  count = 0;
	  for (b=e->brushes.onext ; b != &e->brushes ; b=b->onext)
	  {
		  if (IsBrushSelected(b))
		  {
			  MemFile_fprintf(pMemFile, "// brush %i\n", count);
			  count++;
			  Brush_Write (b, pMemFile);
		  }
	  }
  }
	MemFile_fprintf(pMemFile, "}\n");
}




/*
============
Entity_Create

Creates a new entity out of the selected_brushes list.
If the entity class is fixed size, the brushes are only
used to find a midpoint.  Otherwise, the brushes have
their ownershi[ transfered to the new entity.
============
*/
entity_t	*Entity_Create (eclass_t *c)
{
	entity_t	*e;
	brush_t		*b;
	vec3_t		mins, maxs;
	int			i;

	// check to make sure the brushes are ok

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
		if (b->owner != world_entity)
		{
			Sys_Printf ("Entity NOT created, brushes not all from world\n");
			Sys_Beep ();
			return NULL;
		}

	// create it

	e = (entity_t*)qmalloc(sizeof(*e));
	e->brushes.onext = e->brushes.oprev = &e->brushes;
	e->eclass = c;
	SetKeyValue (e, "classname", c->name);
							  
	// add the entity to the entity list
	e->next = entities.next;
	entities.next = e;
	e->next->prev = e;
	e->prev = &entities;

	if (c->fixedsize)
	{
		//
		// just use the selection for positioning
		//
		b = selected_brushes.next;
		for (i=0 ; i<3 ; i++)
			e->origin[i] = b->mins[i] - c->mins[i];

		// create a custom brush
		VectorAdd (c->mins, e->origin, mins);
		VectorAdd (c->maxs, e->origin, maxs);

	  b = Brush_Create (mins, maxs, &c->texdef);

		Entity_LinkBrush (e, b);

		// delete the current selection
		Select_Delete ();

		// select the new brush
		b->next = b->prev = &selected_brushes;
		selected_brushes.next = selected_brushes.prev = b;

		Brush_Build( b );
	}
	else
	{
		//
		// change the selected brushes over to the new entity
		//
		for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
		{
			Entity_UnlinkBrush (b);
			Entity_LinkBrush (e, b);
			Brush_Build( b );	// so the key brush gets a name
		}
	}

	Sys_UpdateWindows (W_ALL);
	return e;
}


/*
===========
Entity_LinkBrush
===========
*/
void Entity_LinkBrush (entity_t *e, brush_t *b)
{
	if (b->oprev || b->onext)
		Error ("Entity_LinkBrush: Allready linked");
	b->owner = e;

	b->onext = e->brushes.onext;
	b->oprev = &e->brushes;
	e->brushes.onext->oprev = b;
	e->brushes.onext = b;
}

/*
===========
Entity_UnlinkBrush
===========
*/
void Entity_UnlinkBrush (brush_t *b)
{
	//if (!b->owner || !b->onext || !b->oprev)
	if (!b->onext || !b->oprev)
		Error ("Entity_UnlinkBrush: Not currently linked");
	b->onext->oprev = b->oprev;
	b->oprev->onext = b->onext;
	b->onext = b->oprev = NULL;
	b->owner = NULL;
}



/*
===========
Entity_Clone
===========
*/
entity_t	*Entity_Clone (entity_t *e)
{
	entity_t	*n;
	epair_t		*ep, *np;

	n = (entity_t*)qmalloc(sizeof(*n));
	n->brushes.onext = n->brushes.oprev = &n->brushes;
	n->eclass = e->eclass;

	// add the entity to the entity list
	n->next = entities.next;
	entities.next = n;
	n->next->prev = n;
	n->prev = &entities;

	for (ep = e->epairs ; ep ; ep=ep->next)
	{
		np = (epair_t*)qmalloc(sizeof(*np));
		np->key = copystring(ep->key);
		np->value = copystring(ep->value);
		np->next = n->epairs;
		n->epairs = np;
	}
	return n;
}

int GetUniqueTargetId(int iHint)
{
	int iMin, iMax, i;
	BOOL fFound;
	entity_t *pe;
	
	fFound = FALSE;
	pe = entities.next;
	iMin = 0; 
	iMax = 0;
	
	for (; pe != NULL && pe != &entities ; pe = pe->next)
	{
		i = IntForKey(pe, "target");
		if (i)
		{
			iMin = min(i, iMin);
			iMax = max(i, iMax);
			if (i == iHint)
				fFound = TRUE;
		}
	}

	if (fFound)
		return iMax + 1;
	else
		return iHint;
}

entity_t *FindEntity(const char *pszKey, const char *pszValue)
{
	entity_t *pe;
	
	pe = entities.next;
	
	for (; pe != NULL && pe != &entities ; pe = pe->next)
	{
		if (!strcmp(ValueForKey(pe, pszKey), pszValue))
			return pe;
	}

	return NULL;
}

entity_t *FindEntityInt(const char *pszKey, int iValue)
{
	entity_t *pe;
	
	pe = entities.next;
	
	for (; pe != NULL && pe != &entities ; pe = pe->next)
	{
		if (IntForKey(pe, pszKey) == iValue)
			return pe;
	}

	return NULL;
}

