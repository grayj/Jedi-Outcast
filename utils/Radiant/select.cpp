// select.c
#include "stdafx.h"
#include "qe3.h"
#include "oddbits.h"

// externs

/*
===========
Test_Ray
===========
*/
#define	DIST_START	MAX_WORLD_COORD
trace_t Test_Ray (vec3_t origin, vec3_t dir, int flags)
{
	brush_t	*brush;
	face_t	*face;
	float	dist;
	trace_t	t;

	memset (&t, 0, sizeof(t));
	t.dist = DIST_START * sqrt(3) * 2;

  if (flags & SF_CYCLE)
  {
    CPtrArray array;
    brush_t *pToSelect = (selected_brushes.next != &selected_brushes) ? selected_brushes.next : NULL;
    Select_Deselect();

    // go through active brushes and accumulate all "hit" brushes
		for (brush = active_brushes.next ; brush != &active_brushes ; brush=brush->next)
		{
		 //if ( (flags & SF_ENTITIES_FIRST) && brush->owner == world_entity)
		 //  continue;
		 
     if (FilterBrush (brush))
		   continue;

     if (!g_PrefsDlg.m_bSelectCurves && (brush->curveBrush || brush->patchBrush))
       continue;

     //if (!g_bShowPatchBounds && brush->patchBrush)
     //  continue;

		 face = Brush_Ray (origin, dir, brush, &dist);
     if (face)
     {
       array.Add(brush);
     }
    }

    int nSize = array.GetSize();
    if (nSize > 0)
    {
      bool bFound = false;
      for (int i = 0; i < nSize; i++)
      {
        brush_t *b = reinterpret_cast<brush_t*>(array.GetAt(i));
        // did we hit the last one selected yet ?
        if (b == pToSelect)
        {
          // yes we want to select the next one in the list 
          int n = (i > 0) ? i-1 : nSize-1;
          pToSelect = reinterpret_cast<brush_t*>(array.GetAt(n));
          bFound = true;
          break;
        }
      }
      if (!bFound)
        pToSelect = reinterpret_cast<brush_t*>(array.GetAt(0));
    }
    if (pToSelect)
    {
		  face = Brush_Ray (origin, dir, pToSelect, &dist);
			t.dist = dist;
			t.brush = pToSelect;
			t.face = face;
			t.selected = false;
      return t;
    }
  }

	if (! (flags & SF_SELECTED_ONLY) )
  {
		for (brush = active_brushes.next ; brush != &active_brushes ; brush=brush->next)
		{
			if ( (flags & SF_ENTITIES_FIRST) && brush->owner == world_entity)
				continue;
			
      if (FilterBrush (brush))
				continue;

      if (!g_PrefsDlg.m_bSelectCurves && (brush->curveBrush || brush->patchBrush))
        continue;

      //if (!g_bShowPatchBounds && brush->patchBrush)
      //  continue;

			face = Brush_Ray (origin, dir, brush, &dist);
			if (dist > 0 && dist < t.dist)
			{
				t.dist = dist;
				t.brush = brush;
				t.face = face;
				t.selected = false;
			}
		}
  }


	for (brush = selected_brushes.next ; brush != &selected_brushes ; brush=brush->next)
	{
		if ( (flags & SF_ENTITIES_FIRST) && brush->owner == world_entity)
			continue;

    if (FilterBrush (brush))
			continue;

      if (!g_PrefsDlg.m_bSelectCurves && brush->curveBrush)
        continue;

		face = Brush_Ray (origin, dir, brush, &dist);
		if (dist > 0 && dist < t.dist)
		{
			t.dist = dist;
			t.brush = brush;
			t.face = face;
			t.selected = true;
		}
	}

	// if entites first, but didn't find any, check regular

	if ( (flags & SF_ENTITIES_FIRST) && t.brush == NULL)
		return Test_Ray (origin, dir, flags - SF_ENTITIES_FIRST);

	return t;

}


/*
============
Select_Brush

============
*/
void Select_Brush (brush_t *brush, bool bComplete /* = true */)
{
	brush_t	*b;
	entity_t	*e;

	selected_face = NULL;
	if (g_qeglobals.d_select_count < 2)
		g_qeglobals.d_select_order[g_qeglobals.d_select_count] = brush;
	g_qeglobals.d_select_count++;

  //if (brush->patchBrush)
  //  Patch_Select(brush->nPatchID);

	e = brush->owner;
	if (e)
	{
		// select complete entity on first click
		if (e != world_entity && bComplete == true)
		{
			for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
			{
				if (b->owner == e)
					goto singleselect;
			}

			for (b=e->brushes.onext ; b != &e->brushes ; b=b->onext)
			{
				Brush_RemoveFromList (b);
				Brush_AddToList (b, &selected_brushes);
			}
		}
		else
		{
singleselect:
			Brush_RemoveFromList (brush);
			Brush_AddToList (brush, &selected_brushes);
			UpdateSurfaceDialog();
			UpdatePatchInspector();
		}

		if (e->eclass)
		{
			UpdateEntitySel(brush->owner->eclass);
		}
	}
	
	vec3_t vMin, vMax, vSize;
	Select_GetBounds (vMin, vMax);
	VectorSubtract(vMax, vMin, vSize);
	CString strStatus;
	strStatus.Format("Selection X:: %.1f  Y:: %.1f  Z:: %.1f", vSize[0], vSize[1], vSize[2]);
	g_pParentWnd->SetStatusText(2, strStatus);
}

/*
============
Select_Ray

If the origin is inside a brush, that brush will be ignored.
============
*/
void Select_Ray (vec3_t origin, vec3_t dir, int flags)
{
	trace_t	t;

	t = Test_Ray (origin, dir, flags);
	if (!t.brush)
		return;

	if (flags == SF_SINGLEFACE)
	{
		selected_face = t.face;
		selected_face_brush = t.brush;
		Sys_UpdateWindows (W_ALL);
		g_qeglobals.d_select_mode = sel_brush;
		Texture_SetTexture (&t.face->texdef);
    UpdateSurfaceDialog();
		return;
	}

	// move the brush to the other list

	g_qeglobals.d_select_mode = sel_brush;

	if (t.selected)
	{		
		Brush_RemoveFromList (t.brush);
		Brush_AddToList (t.brush, &active_brushes);
	} else
	{
		Select_Brush (t.brush, !(GetKeyState(VK_MENU) & 0x8000));
	}

	Sys_UpdateWindows (W_ALL);
}


void Select_Delete (void)
{
	brush_t	*brush;

	selected_face = NULL;
	g_qeglobals.d_select_mode = sel_brush;

	g_qeglobals.d_select_count = 0;
	g_qeglobals.d_num_move_points = 0;
	while (selected_brushes.next != &selected_brushes)
	{
		brush = selected_brushes.next;
    if (brush->patchBrush)
      Patch_Delete(brush->nPatchID);
		Brush_Free (brush);
	}

	// FIXME: remove any entities with no brushes

	Sys_MarkMapModified();		
	Sys_UpdateWindows (W_ALL);
}

void Select_Deselect (bool bDeSelectToListBack /*= false*/)	// to avoid continuously re-finding same selected ents
{
	brush_t	*b;

  Patch_Deselect();

  g_qeglobals.d_workcount++;
	g_qeglobals.d_select_count = 0;
	g_qeglobals.d_num_move_points = 0;
	b = selected_brushes.next;

	if (b == &selected_brushes)
	{
		if (selected_face)
		{
			selected_face = NULL;
		}
 		Sys_UpdateWindows (W_ALL);
		return;
	}

	selected_face = NULL;
	g_qeglobals.d_select_mode = sel_brush;

	// grab top / bottom height for new brushes
	if (b->mins[2] < b->maxs[2])
	{
		g_qeglobals.d_new_brush_bottom_z = b->mins[2];
		g_qeglobals.d_new_brush_top_z = b->maxs[2];
	}

	if (bDeSelectToListBack)
	{
		selected_brushes.next->prev = active_brushes.prev;
		selected_brushes.prev->next = &active_brushes;
		active_brushes.prev->next	= selected_brushes.next;
		active_brushes.prev			= selected_brushes.prev;
	}
	else
	{	
		selected_brushes.next->prev = &active_brushes;
		selected_brushes.prev->next = active_brushes.next;
		active_brushes.next->prev = selected_brushes.prev;
		active_brushes.next = selected_brushes.next;
	}

	selected_brushes.prev = selected_brushes.next = &selected_brushes;	

	Sys_UpdateWindows (W_ALL);
}


/*
============
Select_Move
============
*/
void Select_Move (vec3_t delta, bool bSnap)
{
	brush_t	*b;
  
 
// actually move the selected brushes
	for (b = selected_brushes.next ; b != &selected_brushes ; b=b->next)
		Brush_Move (b, delta, bSnap);

  vec3_t vMin, vMax;
	Select_GetBounds (vMin, vMax);
  CString strStatus;
  strStatus.Format("Origin X:: %.1f  Y:: %.1f  Z:: %.1f", vMin[0], vMax[1], vMax[2]);
  g_pParentWnd->SetStatusText(2, strStatus);

//	Sys_UpdateWindows (W_ALL);
}

/*
============
Select_Clone

Creates an exact duplicate of the selection in place, then moves
the selected brushes off of their old positions
============
*/
void Select_Clone (void)
{
#if 1
  ASSERT(g_pParentWnd->ActiveXY());
  g_bScreenUpdates = false;  
  g_pParentWnd->ActiveXY()->Copy();
  g_pParentWnd->ActiveXY()->Paste();
  g_pParentWnd->NudgeSelection(2, g_qeglobals.d_gridsize);
  g_pParentWnd->NudgeSelection(3, g_qeglobals.d_gridsize);
  g_bScreenUpdates = true;  
  Sys_UpdateWindows(W_ALL);
#else
/*	// commenting-out helps stop me wasting time updating code that's #if'd out (as has happened)...

	brush_t		*b, *b2, *n, *next, *next2;
	vec3_t		delta;
	entity_t	*e;

	g_qeglobals.d_workcount++;
	g_qeglobals.d_select_mode = sel_brush;

	delta[0] = g_qeglobals.d_gridsize;
	delta[1] = g_qeglobals.d_gridsize;
	delta[2] = 0;

	for (b=selected_brushes.next ; b != &selected_brushes ; b=next)
	{
		next = b->next;
		// if the brush is a world brush, handle simply
		if (b->owner == world_entity)
		{
			n = Brush_Clone (b);
			Brush_AddToList (n, &active_brushes);
			Entity_LinkBrush (world_entity, n);
			Brush_Build( n );
			Brush_Move (b, delta);
			continue;
		}

		e = Entity_Clone (b->owner);
		// clear the target / targetname

		DeleteKey (e, "target");
		DeleteKey (e, "targetname");

		#ifdef QUAKE3
		DeleteKey (e, "target2");
		DeleteKey (e, "target3");	
		DeleteKey (e, "target4");
		#endif

		// if the brush is a fixed size entity, create a new entity
		if (b->owner->eclass->fixedsize)
		{
			n = Brush_Clone (b);
			Brush_AddToList (n, &active_brushes);
			Entity_LinkBrush (e, n);
			Brush_Build( n );
			Brush_Move (b, delta);
			continue;
		}
        
		// brush is a complex entity, grab all the other ones now

		next = &selected_brushes;

		for ( b2 = b ; b2 != &selected_brushes ; b2=next2)
		{
			next2 = b2->next;
			if (b2->owner != b->owner)
			{
				if (next == &selected_brushes)
					next = b2;
				continue;
			}

			// move b2 to the start of selected_brushes,
			// so it won't be hit again
			Brush_RemoveFromList (b2);
			Brush_AddToList (b2, &selected_brushes);
			
			n = Brush_Clone (b2);
			Brush_AddToList (n, &active_brushes);
			Entity_LinkBrush (e, n);
			Brush_Build( n );
			Brush_Move (b2, delta, true);
		}

	}
*/
#endif
	Sys_UpdateWindows (W_ALL);
}



/*
============
Select_SetTexture
bool bFitScale = compute scale on the plane and counteract plane->axial snapping
============
*/
void Select_SetTexture (texdef_t *texdef, bool bFitScale/*=false*/, bool bNoSystemTextureOverwrite/*=false*/)
{
	brush_t	*b;

	if (selected_face)
	{
		SetFaceTexdef (selected_face_brush, selected_face, texdef, bFitScale, bNoSystemTextureOverwrite);
		Brush_Build(selected_face_brush, bFitScale);
	}
	else
	{
		for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
			if (!b->owner->eclass->fixedsize)
				Brush_SetTexture (b, texdef, bFitScale, bNoSystemTextureOverwrite);
	}
	Sys_UpdateWindows (W_ALL);
}


void Select_FitTexture(int nHeight, int nWidth)
{
	brush_t		*b;

	if(selected_brushes.next == &selected_brushes && selected_face == NULL)
		return;

  for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
    Brush_FitTexture(b, nHeight, nWidth);
		Brush_Build(b);
	}

	if (selected_face)
	{
    Face_FitTexture(selected_face, nHeight, nWidth);
		Brush_Build(selected_face_brush);
	}

	Sys_UpdateWindows (W_CAMERA);
}


void Select_Hide()
{
	for (brush_t* b=selected_brushes.next ; b && b != &selected_brushes ; b=b->next)
	{
    b->hiddenBrush = true;
	}
	Sys_UpdateWindows (W_ALL);
}

void Select_ShowAllHidden()
{
  brush_t* b;
	for (b=selected_brushes.next ; b && b != &selected_brushes ; b=b->next)
	{
    b->hiddenBrush = false;
	}
	for (b=active_brushes.next ; b && b != &active_brushes ; b=b->next)
	{
    b->hiddenBrush = false;
	}
	Sys_UpdateWindows (W_ALL);
}



/*
================================================================

  TRANSFORMATIONS

================================================================
*/

void Select_GetBounds (vec3_t mins, vec3_t maxs)
{
	brush_t	*b;
	int		i;

	for (i=0 ; i<3 ; i++)
	{
		mins[i] = MAX_WORLD_COORD;
		maxs[i] = MIN_WORLD_COORD;
	}

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
		for (i=0 ; i<3 ; i++)
		{
			if (b->mins[i] < mins[i])
				mins[i] = b->mins[i];
			if (b->maxs[i] > maxs[i])
				maxs[i] = b->maxs[i];
		}
}


void Select_GetTrueMid (vec3_t mid)
{
	vec3_t	mins, maxs;
	Select_GetBounds (mins, maxs);

  for (int i=0 ; i<3 ; i++)
    mid[i] = (mins[i] + ((maxs[i] - mins[i]) / 2));
}


void Select_GetMid (vec3_t mid)
{
	vec3_t	mins, maxs;
	int		i;

  if (g_PrefsDlg.m_bNoClamp)
  {
    Select_GetTrueMid(mid);
    return;
  }

  Select_GetBounds (mins, maxs);

  for (i=0 ; i<3 ; i++)
		mid[i] = g_qeglobals.d_gridsize*floor ( ( (mins[i] + maxs[i])*0.5 )/g_qeglobals.d_gridsize );

}

vec3_t	select_origin;
vec3_t	select_matrix[3];
qboolean	select_fliporder;

void Select_AplyMatrix (bool bSnap)
{
	brush_t	*b;
	face_t	*f;
	int		i, j;
	vec3_t	temp;

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
    for (f=b->brush_faces ; f ; f=f->next)
	  {
		  for (i=0 ; i<3 ; i++)
			{
			  VectorSubtract (f->planepts[i], select_origin, temp);
				for (j=0 ; j<3 ; j++)
				  f->planepts[i][j] = DotProduct(temp, select_matrix[j]) + select_origin[j];
			}
			if (select_fliporder)
			{
			  VectorCopy (f->planepts[0], temp);
				VectorCopy (f->planepts[2], f->planepts[0]);
				VectorCopy (temp, f->planepts[2]);
			}
    }
		Brush_Build(b, bSnap);
    if (b->patchBrush)
    {
      Patch_ApplyMatrix(b->nPatchID, select_origin, select_matrix);
    }
	}
}

void ProjectOnPlane(vec3_t& normal,float dist,vec3_t& ez, vec3_t& p)
{
	if (fabs(ez[0]) == 1)
		p[0] = (dist - normal[1] * p[1] - normal[2] * p[2]) / normal[0];
	else if (fabs(ez[1]) == 1)
		p[1] = (dist - normal[0] * p[0] - normal[2] * p[2]) / normal[1];
	else
		p[2] = (dist - normal[0] * p[0] - normal[1] * p[1]) / normal[2];
}

void Back(vec3_t& dir, vec3_t& p)
{
	if (fabs(dir[0]) == 1)
		p[0] = 0;
	else if (fabs(dir[1]) == 1)
		p[1] = 0;
	else p[2] = 0;
}



// using scale[0] and scale[1]
void ComputeScale(vec3_t& rex, vec3_t& rey, vec3_t& p, face_t* f)
{
	float px = DotProduct(rex, p);
	float py = DotProduct(rey, p);
	px *= f->texdef.scale[0];
	py *= f->texdef.scale[1];
  vec3_t aux;
  VectorCopy(rex, aux);
  VectorScale(aux, px, aux);
  VectorCopy(aux, p);
  VectorCopy(rey, aux);
  VectorScale(aux, py, aux);
  VectorAdd(p, aux, p);
}



void AbsoluteToLocal(plane_t normal2, face_t* f, vec3_t& p1, vec3_t& p2, vec3_t& p3)
{
	vec3_t ex,ey,ez;
	// computing new local axis base
  TextureAxisFromPlane(&normal2, ex, ey);
  CrossProduct(ex, ey, ez);

  // projecting back on (ex,ey)
	Back(ez,p1);
	Back(ez,p2);
	Back(ez,p3);

	vec3_t aux;
	// rotation
  VectorCopy(p2, aux);
  VectorSubtract(aux, p1,aux);
	
	float x = DotProduct(aux,ex);
	float y = DotProduct(aux,ey);
  f->texdef.rotate = 180 * atan2(y,x) / Q_PI;

	vec3_t rex,rey;
	// computing rotated local axis base
  VectorCopy(ez, aux);
  VectorScale(aux, f->texdef.rotate, aux);
  VectorCopy(ex, rex);
  VectorRotate(rex, aux, rex);
  VectorCopy(ey, rey);
  VectorRotate(rey, aux, rey);

	// scale
  VectorCopy(p2, aux);
  VectorSubtract(aux, p1, aux);
  f->texdef.scale[0] = DotProduct(aux, rex);
  VectorCopy(p3, aux);
  VectorSubtract(aux, p1, aux);
  f->texdef.scale[1] = DotProduct(aux, rey);

	// shift
	// only using p1
	x = DotProduct(rex,p1);
	y = DotProduct(rey,p1);                 
	x /= f->texdef.scale[0];
	y /= f->texdef.scale[1];

  VectorCopy(rex, p1);
  VectorScale(p1, x, p1);
  VectorCopy(rey, aux);
  VectorScale(aux, y, aux);
  VectorAdd(p1, aux, p1);
  VectorCopy(ez, aux);
  VectorScale(aux, -f->texdef.rotate, aux);
  VectorRotate(p1, aux, p1);
	f->texdef.shift[0] = -DotProduct(p1, ex);
	f->texdef.shift[1] = -DotProduct(p1, ey);

	// stored rot is good considering local axis base
	// change it if necessary
	f->texdef.rotate = -f->texdef.rotate;

  Clamp(f->texdef.shift[0], f->d_texture->width);
  Clamp(f->texdef.shift[1], f->d_texture->height);
  Clamp(f->texdef.rotate, 360);

}


void RotateFaceTexture(face_t* f, int nAxis, float fDeg)
{
	vec3_t p1,p2,p3, rota;   
  p1[0] = p1[1] = p1[2] = 0;
  VectorCopy(p1, p2);
  VectorCopy(p1, p3);
  VectorCopy(p1, rota);
	ComputeAbsolute(f, p1, p2, p3);

  
  rota[nAxis] = fDeg;
  VectorRotate(p1, rota, select_origin, p1);
  VectorRotate(p2, rota, select_origin, p2);
  VectorRotate(p3, rota, select_origin, p3);

  plane_t normal2;
  vec3_t vNormal;
  vNormal[0] = f->plane.normal[0];
  vNormal[1] = f->plane.normal[1];
  vNormal[2] = f->plane.normal[2];
  VectorRotate(vNormal, rota, vNormal);
  normal2.normal[0] = vNormal[0];
  normal2.normal[1] = vNormal[1];
  normal2.normal[2] = vNormal[2];
	AbsoluteToLocal(normal2, f, p1, p2 ,p3);

}

void RotateTextures(int nAxis, float fDeg, vec3_t vOrigin)
{
	for (brush_t* b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
    for (face_t* f=b->brush_faces ; f ; f=f->next)
		{
      RotateFaceTexture(f, nAxis, fDeg);
      Brush_Build(b, false);
    }
    Brush_Build(b, false);
	}
}


void Select_FlipAxis (int axis)
{
	int		i;

	Select_GetMid (select_origin);
	for (i=0 ; i<3 ; i++)
	{
		VectorCopy (vec3_origin, select_matrix[i]);
		select_matrix[i][i] = 1;
	}
	select_matrix[axis][axis] = -1;

	select_fliporder = true;
	Select_AplyMatrix (true);
	Sys_UpdateWindows (W_ALL);
}


void Select_Scale(float x, float y, float z)
{
  Select_GetMid (select_origin);
	for (brush_t* b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		for (face_t* f=b->brush_faces ; f ; f=f->next)
		{
			for (int i=0 ; i<3 ; i++)
			{
        f->planepts[i][0] -= select_origin[0];
        f->planepts[i][1] -= select_origin[1];
        f->planepts[i][2] -= select_origin[2];
        f->planepts[i][0] *= x;
        //f->planepts[i][0] = floor(f->planepts[i][0] / g_qeglobals.d_gridsize + 0.5) * g_qeglobals.d_gridsize;

        f->planepts[i][1] *= y;
        //f->planepts[i][1] = floor(f->planepts[i][1] / g_qeglobals.d_gridsize + 0.5) * g_qeglobals.d_gridsize;

        f->planepts[i][2] *= z;
        //f->planepts[i][2] = floor(f->planepts[i][2] / g_qeglobals.d_gridsize + 0.5) * g_qeglobals.d_gridsize;
        
        f->planepts[i][0] += select_origin[0];
        f->planepts[i][1] += select_origin[1];
        f->planepts[i][2] += select_origin[2];
			}
		}
		Brush_Build(b, false);
    if (b->patchBrush)
    {
      vec3_t v;
      v[0] = x;
      v[1] = y;
      v[2] = z;
      Patch_Scale(b->nPatchID, select_origin, v);
    }
	}
}

void Select_RotateAxis (int axis, float deg, bool bPaint, bool bMouse)
{
	vec3_t	temp;
	int		i, j;
	vec_t	c, s;

	if (deg == 0)
  {
    //Sys_Printf("0 deg\n");
		return;
  }

  if (bMouse)
  {
    VectorCopy(g_pParentWnd->ActiveXY()->RotateOrigin(), select_origin);
  }
  else
  {
	  Select_GetMid (select_origin);
  }

	select_fliporder = false;

	if (deg == 90)
	{
		for (i=0 ; i<3 ; i++)
		{
			VectorCopy (vec3_origin, select_matrix[i]);
			select_matrix[i][i] = 1;
		}
		i = (axis+1)%3;
		j = (axis+2)%3;
		VectorCopy (select_matrix[i], temp);
		VectorCopy (select_matrix[j], select_matrix[i]);
		VectorSubtract (vec3_origin, temp, select_matrix[j]);
	}
	else
	{
		deg = -deg;
		if (deg == -180.0)
		{
			c = -1;
			s = 0;
		}
		else if (deg == -270.0)
		{
			c = 0;
			s = -1;
		}
		else
		{
			c = cos(deg * Q_PI / 180.0);
			s = sin(deg * Q_PI / 180.0);
		}

		for (i=0 ; i<3 ; i++)
		{
			VectorCopy (vec3_origin, select_matrix[i]);
			select_matrix[i][i] = 1;
		}

		switch (axis)
		{
		case 0:
			select_matrix[1][1] = c;
			select_matrix[1][2] = -s;
			select_matrix[2][1] = s;
			select_matrix[2][2] = c;
			break;
		case 1:
			select_matrix[0][0] = c;
			select_matrix[0][2] = s;
			select_matrix[2][0] = -s;
			select_matrix[2][2] = c;
			break;
		case 2:
			select_matrix[0][0] = c;
			select_matrix[0][1] = -s;
			select_matrix[1][0] = s;
			select_matrix[1][1] = c;
			break;
		}
	}


  if (g_PrefsDlg.m_bRotateLock)
    RotateTextures(axis, deg, select_origin);

	Select_AplyMatrix(!bMouse);

  if (bPaint)
	  Sys_UpdateWindows (W_ALL);
}

/*
================================================================

GROUP SELECTIONS

================================================================
*/

void Select_CompleteTall (void)
{
	brush_t	*b, *next;
	//int		i;
	vec3_t	mins, maxs;

	if (!QE_SingleBrush ())
		return;

	g_qeglobals.d_select_mode = sel_brush;

	VectorCopy (selected_brushes.next->mins, mins);
	VectorCopy (selected_brushes.next->maxs, maxs);
	Select_Delete ();

  int nDim1 = (g_pParentWnd->ActiveXY()->GetViewType() == YZ) ? 1 : 0;
  int nDim2 = (g_pParentWnd->ActiveXY()->GetViewType() == XY) ? 1 : 2;

	for (b=active_brushes.next ; b != &active_brushes ; b=next)
	{
		next = b->next;

    if ( (b->maxs[nDim1] > maxs[nDim1] || b->mins[nDim1] < mins[nDim1]) 
      || (b->maxs[nDim2] > maxs[nDim2] || b->mins[nDim2] < mins[nDim2]) )
      continue;

	 	if (FilterBrush (b))
	 		continue;

		Brush_RemoveFromList (b);
		Brush_AddToList (b, &selected_brushes);
#if 0
    // old stuff
    for (i=0 ; i<2 ; i++)
			if (b->maxs[i] > maxs[i] || b->mins[i] < mins[i])
				break;
		if (i == 2)
		{
			Brush_RemoveFromList (b);
			Brush_AddToList (b, &selected_brushes);
		}
#endif
	}
	Sys_UpdateWindows (W_ALL);
}

void Select_PartialTall (void)
{
	brush_t	*b, *next;
	//int		i;
	vec3_t	mins, maxs;

	if (!QE_SingleBrush ())
		return;

	g_qeglobals.d_select_mode = sel_brush;

	VectorCopy (selected_brushes.next->mins, mins);
	VectorCopy (selected_brushes.next->maxs, maxs);
	Select_Delete ();

  int nDim1 = (g_pParentWnd->ActiveXY()->GetViewType() == YZ) ? 1 : 0;
  int nDim2 = (g_pParentWnd->ActiveXY()->GetViewType() == XY) ? 1 : 2;

	for (b=active_brushes.next ; b != &active_brushes ; b=next)
	{
		next = b->next;

    if ( (b->mins[nDim1] > maxs[nDim1] || b->maxs[nDim1] < mins[nDim1]) 
      || (b->mins[nDim2] > maxs[nDim2] || b->maxs[nDim2] < mins[nDim2]) )
      continue;

	 	if (FilterBrush (b))
	 		continue;

  	Brush_RemoveFromList (b);
		Brush_AddToList (b, &selected_brushes);


#if 0
// old stuff
		for (i=0 ; i<2 ; i++)
			if (b->mins[i] > maxs[i] || b->maxs[i] < mins[i])
				break;
		if (i == 2)
		{
			Brush_RemoveFromList (b);
			Brush_AddToList (b, &selected_brushes);
		}
#endif
	}
	Sys_UpdateWindows (W_ALL);
}

void Select_Touching (void)
{
	brush_t	*b, *next;
	int		i;
	vec3_t	mins, maxs;

	if (!QE_SingleBrush ())
		return;

	g_qeglobals.d_select_mode = sel_brush;

	VectorCopy (selected_brushes.next->mins, mins);
	VectorCopy (selected_brushes.next->maxs, maxs);

	for (b=active_brushes.next ; b != &active_brushes ; b=next)
	{
		next = b->next;

	 	if (FilterBrush (b))
	 		continue;

		for (i=0 ; i<3 ; i++)
			if (b->mins[i] > maxs[i]+1 || b->maxs[i] < mins[i]-1)
				break;

		if (i == 3)
		{
			Brush_RemoveFromList (b);
			Brush_AddToList (b, &selected_brushes);
		}
	}
	Sys_UpdateWindows (W_ALL);
}

void Select_Inside (void)
{
	brush_t	*b, *next;
	int		i;
	vec3_t	mins, maxs;

	if (!QE_SingleBrush ())
		return;

	g_qeglobals.d_select_mode = sel_brush;

	VectorCopy (selected_brushes.next->mins, mins);
	VectorCopy (selected_brushes.next->maxs, maxs);
	Select_Delete ();

	for (b=active_brushes.next ; b != &active_brushes ; b=next)
	{
		next = b->next;

	 	if (FilterBrush (b))
	 		continue;

		for (i=0 ; i<3 ; i++)
			if (b->maxs[i] > maxs[i] || b->mins[i] < mins[i])
				break;
		if (i == 3)
		{
			Brush_RemoveFromList (b);
			Brush_AddToList (b, &selected_brushes);
		}
	}
	Sys_UpdateWindows (W_ALL);
}

/*
=============
Select_Ungroup

Turn the currently selected entity back into normal brushes
=============
*/
void  Select_Ungroup (void)
{
	int numselectedgroups;
	entity_t	*e;
	brush_t		*b, *sb;

	numselectedgroups = 0;
	for (sb = selected_brushes.next; sb != &selected_brushes; sb = sb->next)
	{
		e = sb->owner;

		if (!e || e == world_entity || e->eclass->fixedsize)
		{
			continue;
		}

		for (b = e->brushes.onext; b != &e->brushes; b = e->brushes.onext)
		{
			//Brush_RemoveFromList (b);
			//Brush_AddToList (b, &active_brushes);
			Entity_UnlinkBrush (b);
			Entity_LinkBrush (world_entity, b);
			Brush_Build( b );
			b->owner = world_entity;
		}
		Entity_Free (e);
		numselectedgroups++;
	}

	if (numselectedgroups <= 0)
	{
		Sys_Printf("No grouped entities selected.\n");
		return;
	}
	Sys_Printf("Ungrouped %d entit%s.\n", numselectedgroups, (numselectedgroups == 1)?"y":"ies");
	Sys_UpdateWindows (W_ALL);
}

/*
====================
Select_MakeStructural
====================
*/
void Select_MakeStructural (void)
{
	brush_t	*b;
	face_t	*f;

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
		for (f=b->brush_faces ; f ; f=f->next)
			f->texdef.contents &= ~CONTENTS_DETAIL;
	Select_Deselect ();
	Sys_UpdateWindows (W_ALL);
}

void Select_MakeDetail (bool bDeselectAfterwards /* = true */)
{
	brush_t	*b;
	face_t	*f;

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
		for (f=b->brush_faces ; f ; f=f->next)
			f->texdef.contents |= CONTENTS_DETAIL;

	if (bDeselectAfterwards)
	{
		Select_Deselect ();
	}
	Sys_UpdateWindows (W_ALL);
}

void Select_MakeNonSolid(void)
{
	brush_t	*b;
	face_t	*f;

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		for (f=b->brush_faces ; f ; f=f->next)
			f->texdef.flags |= SURF_NONSOLID;

		if (b->patchBrush)
		{
		  Patch_MakeNonSolid(b);
		}
	}
	Select_Deselect ();
	Sys_UpdateWindows (W_ALL);
}
void Select_ClearNonSolid(void)
{
	brush_t	*b;
	face_t	*f;

	for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		for (f=b->brush_faces ; f ; f=f->next)
			f->texdef.flags &= ~SURF_NONSOLID;

		if (b->patchBrush)
		{
		  Patch_MakeSolid(b);
		}
	}

	Select_Deselect ();
	Sys_UpdateWindows (W_ALL);
}


int GetSelectedBrushesCount(void)
{
	int iCount = 0;

	for (brush_t *pb=selected_brushes.next ; pb != &selected_brushes ; pb=pb->next)
	{
		iCount++;
	}

	return iCount;
}

// returns either single selected brush or NULL
//
brush_t *GetTheSelectedBrush(bool bQuietError)
{
	if (GetSelectedBrushesCount()==1)
	{
		return selected_brushes.next;
	}
	
	if (!bQuietError)
	{
		ErrorBox("This only works with 1 brush selected");
	}
	return NULL;
}


void R_Entity_UpdateAllTargetWaypoints(entity_t *pEnt, entity_t *pOriginalEnt, bool bHide)
{
	if (pEnt)
	{
		if (strlen(ValueForKey(pEnt, sKEY_WAYPOINTHIDE_RECURSION_PROTECT))==0)
		{
			SetKeyValue(pEnt, sKEY_WAYPOINTHIDE_RECURSION_PROTECT, "1");	// note: actual value irrelevant, just so long as strlen()!=0

			CString str;
			entity_t *pDestEnt;			

			str = ValueForKey(pEnt, "target");
			if (str.GetLength() > 0)
			{
				pDestEnt = FindEntity("targetname", str.GetBuffer(0));

				R_Entity_UpdateAllTargetWaypoints(pDestEnt, pOriginalEnt, bHide);
			}

			str = ValueForKey(pEnt, "target2");
			if (str.GetLength() > 0)
			{
				pDestEnt = FindEntity("targetname", str.GetBuffer(0));

				R_Entity_UpdateAllTargetWaypoints(pDestEnt, pOriginalEnt, bHide);
			}

			str = ValueForKey(pEnt, "target3");
			if (str.GetLength() > 0)
			{
				pDestEnt = FindEntity("targetname", str.GetBuffer(0));

				R_Entity_UpdateAllTargetWaypoints(pDestEnt, pOriginalEnt, bHide);				
			}

			str = ValueForKey(pEnt, "target4");
			if (str.GetLength() > 0)
			{
				pDestEnt = FindEntity("targetname", str.GetBuffer(0));

				R_Entity_UpdateAllTargetWaypoints(pDestEnt, pOriginalEnt, bHide);				
			}

			if (pEnt != pOriginalEnt)	// because we don't want to hide/unhide ourselves, just the children
			{
				if (bHide)
				{
					SetKeyValue(pEnt, sKEY_HIDDENWAYPOINT, "1");	// note: actual value irrelevant, just so long as strlen()!=0
				}
				else
				{
					DeleteKey(pEnt, sKEY_HIDDENWAYPOINT);
				}
			}
		}
	}
}


void Select_ClearAllRecursionProtection(void)
{
	for (entity_t* pEnt=entities.next ; pEnt != &entities ; pEnt=pEnt->next)
	{
		DeleteKey(pEnt, sKEY_WAYPOINTHIDE_RECURSION_PROTECT);		
	}
}

void Select_HideWaypointChildren(void)
{
	brush_t *pb = GetTheSelectedBrush();

	if (pb)
	{
		if (!strncmp(pb->owner->eclass->name, "waypoint", 8))
		{
			Select_ClearAllRecursionProtection();	// probably not needed beforehand, but wtf?
			R_Entity_UpdateAllTargetWaypoints(pb->owner, pb->owner, true);
			Select_ClearAllRecursionProtection();
			Sys_UpdateWindows (W_XY|W_CAMERA);	
		}
		else
		{
			ErrorBox("This only works with waypoint brushes");
		}
	}
}

void Select_UnHideWaypointChildren(void)
{
	brush_t *pb = GetTheSelectedBrush();

	if (pb)
	{
		if (!strncmp(pb->owner->eclass->name, "waypoint", 8))
		{
			Select_ClearAllRecursionProtection();	// probably not needed beforehand, but wtf?
			R_Entity_UpdateAllTargetWaypoints(pb->owner, pb->owner, false);
			Select_ClearAllRecursionProtection();
			Sys_UpdateWindows (W_XY|W_CAMERA);
		}
		else
		{
			ErrorBox("This only works with waypoint brushes");
		}
	}
}

void Select_UnHideAllWaypoints(void)
{
	for (entity_t* pEnt=entities.next ; pEnt != &entities ; pEnt=pEnt->next)
	{
		DeleteKey(pEnt, sKEY_HIDDENWAYPOINT);		
	}

	Sys_UpdateWindows (W_XY|W_CAMERA);
}


#ifdef QUAKE3
//
// you can safely add up to 100 fields here without problems, all automatically. Any more, and you need to
//	update the gap between ID_SCRIPTPOPUP_ID_START & ID_SCRIPTPOPUP_ID_END in resource.h -ste
//
static const char *ScriptRunFieldNames[]=
{
	"spawnscript",
	"idlescript",
	"touchscript",
	"usescript", 
	"awakescript",
	"angerscript",
	"attackscript", 
	"victoryscript",
	"painscript", 
	"fleescript", 
	"deathscript", 
	"delayscript", 
	"delayscripttime", 
	"blockedscript", 
	"affirmscript", 
	"negativescript", 
};

int ScriptPopup_GetFieldsCount()
{
	return (sizeof(ScriptRunFieldNames) / sizeof(ScriptRunFieldNames[0]));
}

LPCSTR ScriptPopup_GetField(int iIndex)
{
	ASSERT(iIndex<ScriptPopup_GetFieldsCount());

	if (iIndex>=ScriptPopup_GetFieldsCount())
		return "";		// won't happen if called only from submenus, but jic...

	return ScriptRunFieldNames[iIndex];
}

// returns NULL if field doesn't exist
LPCSTR Ent_GetScriptNameForScriptRunField(entity_t *ent, LPCSTR psFieldName)
{
	LPCSTR psReturn = ValueForKey(ent, psFieldName);

	if (strlen(psReturn))
		return psReturn;

	return NULL;
}

// returns NULL if field doesn't exist in the selected entity (used as bool check in menu greying)
//
LPCSTR ScriptPopup_GetScriptName(int iIndex)
{
	brush_t *pb = GetTheSelectedBrush(true);

	if (pb)
	{
		return Ent_GetScriptNameForScriptRunField(pb->owner, ScriptPopup_GetField(iIndex));			
	}

	return NULL;
}

bool ScriptPopup_Allowed()
{
	brush_t *pb = GetTheSelectedBrush(true);

	if (pb)
	{
		for (int i=0; i<ScriptPopup_GetFieldsCount(); i++)
		{
			if (Ent_GetScriptNameForScriptRunField(pb->owner,ScriptRunFieldNames[i]))
			{
				return true;
			}
		}
	}

	return false;
}
#endif

void Select_ShiftTexture(int x, int y)
{
	brush_t		*b;
	face_t		*f;

	if(selected_brushes.next == &selected_brushes && selected_face == NULL)
		return;

  for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		for (f=b->brush_faces ; f ; f=f->next)
		{
			f->texdef.shift[0] += x;
			f->texdef.shift[1] += y;
		}
		Brush_Build(b);
    if (b->patchBrush)
      Patch_ShiftTexture(b->nPatchID, x, y);
	}

	if (selected_face)
	{
		selected_face->texdef.shift[0] += x;
		selected_face->texdef.shift[1] += y;
		Brush_Build(selected_face_brush);
	}

	Sys_UpdateWindows (W_CAMERA);
}

void Select_ScaleTexture(int x, int y)
{
	brush_t		*b;
	face_t		*f;

	if(selected_brushes.next == &selected_brushes && selected_face == NULL)
	{
		return;
	}

  for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		for (f=b->brush_faces ; f ; f=f->next)
		{
			f->texdef.scale[0] += x;
			f->texdef.scale[1] += y;
		}
		Brush_Build(b);
    if (b->patchBrush)
      Patch_ScaleTexture(b->nPatchID, x, y);
	}

	if (selected_face)
	{
		selected_face->texdef.scale[0] += x;
		selected_face->texdef.scale[1] += y;
		Brush_Build(selected_face_brush);
	}

	Sys_UpdateWindows (W_CAMERA);
}

void Select_RotateTexture(int amt)
{
	brush_t		*b;
	face_t		*f;

	if(selected_brushes.next == &selected_brushes && selected_face == NULL)
		return;

  for (b=selected_brushes.next ; b != &selected_brushes ; b=b->next)
	{
		for (f=b->brush_faces ; f ; f=f->next)
		{
			f->texdef.rotate += amt;
      f->texdef.rotate = static_cast<int>(f->texdef.rotate) % 360;
		}
		Brush_Build(b);
    if (b->patchBrush)
      Patch_RotateTexture(b->nPatchID, amt);
	}
	
  if (selected_face)
	{
		selected_face->texdef.rotate += amt;
    selected_face->texdef.rotate = static_cast<int>(selected_face->texdef.rotate) % 360;
		Brush_Build(selected_face_brush);
	}

	Sys_UpdateWindows (W_CAMERA);
}

// return is simply a count of how many texture replacements took place, only used for friendly-stats on completion...
//
// the arg list for this is getting pretty gay now, the reason being that the various update-windows routines that
//	radiant has don't work like windows ones, where paint messages are just issued then it returns instantly 
//	(and eliminates dup paint calls), radiant actually goes off and performs them, so I need to stop it doing that 
//	when calling this in the middle of a loop...
//
int FindReplaceTextures(const char* pFind, const char* pReplace, bool bSelected, bool bForce, bool bReScale, bool bSelectOnlyNoReplace,
						bool bInhibitCameraUpdate /*= false*/,
						bool bCalledDuringLoopAndNotFirstTime /*= false*/	// sigh, but saves an update-all-windows call
						)
{
	int iReplacedCount = 0;

	if (bSelectOnlyNoReplace)
	{
		bSelected = false;
		bForce = false;
	}
	
	const brush_t* const pList = (bSelected) ? &selected_brushes : &active_brushes;
	if (!bSelected)
	{
		if (!bCalledDuringLoopAndNotFirstTime)
			Select_Deselect();
	}
	
	const qtexture_t * const pReplaceTex = Texture_ForName(pReplace);


	// count them first, so I can show progress..
	//
	int iBrushCount = 0;
	for (brush_t* pBrush = pList->next ; pBrush != pList; pBrush = pBrush->next)
	{
		iBrushCount++;
	}

	Sys_Printf("\n");
	brush_t* pNextBrush = pList->next->prev;	// doesn't matter what value assigned here really. Note sneaky workaround here because can't point at non-const item, so use next->prev
	for (pBrush = pList->next ; pBrush != pList; pBrush = pNextBrush)
	{
		pNextBrush = pBrush->next;

		// just so they don't think it's locked up...
		//
		if ( !((iBrushCount--) & 15))
		{
			Sys_Printf(".");
		}
		OutputDebugString(va("iBrushCount = %d\n",iBrushCount));
		

		if (pBrush->patchBrush)
		{
			if (bSelectOnlyNoReplace)
			{
				if (!stricmp(pFind,Patch_FromBrush_GetTextureName(pBrush)))
				{
					g_bScreenUpdates = false;	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!
					Select_Brush(pBrush, false);
					g_bScreenUpdates = true;	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!
					continue;
				}
			}
			else
			{
				iReplacedCount += (Patch_FindReplaceTexture(pBrush, pFind, pReplace, bForce))?1:0;
			}
		}
		
		for (face_t* pFace = pBrush->brush_faces; pFace; pFace = pFace->next)
		{
			if(bForce || strcmpi(pFace->texdef.name, pFind) == 0)
			{
				iReplacedCount++;
				if (bSelectOnlyNoReplace)
				{
					g_bScreenUpdates = false;	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!
					Select_Brush(pBrush, false);
					g_bScreenUpdates = true;	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!
					break;
				}
				else
				{
					if (bReScale)
					{
						if (pFace->d_texture == pReplaceTex)
						{//not changing textures, so reset the scale instead
							const float fXAspect = fTEXTURE_SCALE / (float) pFace->texdef.scale[0];
							const float fYAspect = fTEXTURE_SCALE / (float) pFace->texdef.scale[1];
							
							pFace->texdef.scale[0] = fTEXTURE_SCALE;
							pFace->texdef.scale[1] = fTEXTURE_SCALE;
							
							pFace->texdef.shift[0] /= fXAspect;
							pFace->texdef.shift[1] /= fYAspect;
						}
						else
						{
							const float fXAspect = (float)(pFace->d_texture->width)  / (float) pReplaceTex->width;
							const float fYAspect = (float)(pFace->d_texture->height) / (float) pReplaceTex->height;
							
							pFace->texdef.scale[0] *= fXAspect;
							pFace->texdef.scale[1] *= fYAspect;
							
							pFace->texdef.shift[0] /= fXAspect;
							pFace->texdef.shift[1] /= fYAspect;
						}
					}
					strcpy(pFace->texdef.name, pReplace);
					pFace->d_texture = (qtexture_t *)pReplaceTex;
				}
			}
		}
		if (!bSelectOnlyNoReplace)
		{
			Brush_Build(pBrush);
		}
	}
	Sys_Printf("\n");
	
	if (!bInhibitCameraUpdate)
	{
		Sys_UpdateWindows (W_CAMERA);
	}

	return iReplacedCount;
}


void ComputeAbsolute(face_t* f, vec3_t& p1, vec3_t& p2, vec3_t& p3)
{
	vec3_t ex,ey,ez;	        // local axis base

  // compute first local axis base
  TextureAxisFromPlane(&f->plane, ex, ey);
  CrossProduct(ex, ey, ez);
	    
	vec3_t aux;
  VectorCopy(ex, aux);
  VectorScale(aux, -f->texdef.shift[0], aux);
  VectorCopy(aux, p1);
  VectorCopy(ey, aux);
  VectorScale(aux, -f->texdef.shift[1], aux);
  VectorAdd(p1, aux, p1);
  VectorCopy(p1, p2);
  VectorAdd(p2, ex, p2);
  VectorCopy(p1, p3);
  VectorAdd(p3, ey, p3);
  VectorCopy(ez, aux);
  VectorScale(aux, -f->texdef.rotate, aux);
  VectorRotate(p1, aux, p1);
  VectorRotate(p2, aux, p2);
  VectorRotate(p3, aux, p3);
	// computing rotated local axis base
	vec3_t rex,rey;
  VectorCopy(ex, rex);
  VectorRotate(rex, aux, rex);
  VectorCopy(ey, rey);
  VectorRotate(rey, aux, rey);

  ComputeScale(rex,rey,p1,f);
	ComputeScale(rex,rey,p2,f);
	ComputeScale(rex,rey,p3,f);

	// project on normal plane
	// along ez 
	// assumes plane normal is normalized
	ProjectOnPlane(f->plane.normal,f->plane.dist,ez,p1);
	ProjectOnPlane(f->plane.normal,f->plane.dist,ez,p2);
	ProjectOnPlane(f->plane.normal,f->plane.dist,ez,p3);
};


