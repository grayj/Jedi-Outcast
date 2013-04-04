
#include "stdafx.h"
#include "qe3.h"

/*
==============
CSG_SplitBrushByFace

The incoming brush is NOT freed.
The incoming face is NOT left referenced.
==============
*/
void CSG_SplitBrushByFace (brush_t *in, face_t *f, brush_t **front, brush_t **back)
{
	brush_t	*b;
	face_t	*nf;
	vec3_t	temp;

	b = Brush_Clone (in);
	nf = Face_Clone (f);

	nf->texdef = b->brush_faces->texdef;
	nf->next = b->brush_faces;
	b->brush_faces = nf;

	Brush_Build( b );
	Brush_RemoveEmptyFaces ( b );
	if ( !b->brush_faces )
	{	// completely clipped away
		Brush_Free (b);
		*back = NULL;
	}
	else
	{
		Entity_LinkBrush (in->owner, b);
		*back = b;
	}

	b = Brush_Clone (in);
	nf = Face_Clone (f);
	// swap the plane winding
	VectorCopy (nf->planepts[0], temp);
	VectorCopy (nf->planepts[1], nf->planepts[0]);
	VectorCopy (temp, nf->planepts[1]);

	nf->texdef = b->brush_faces->texdef;
	nf->next = b->brush_faces;
	b->brush_faces = nf;

	Brush_Build( b );
	Brush_RemoveEmptyFaces ( b );
	if ( !b->brush_faces )
	{	// completely clipped away
		Brush_Free (b);
		*front = NULL;
	}
	else
	{
		Entity_LinkBrush (in->owner, b);
		*front = b;
	}
}

/*
=============
CSG_MakeHollow
=============
*/
void CSG_MakeHollow (void)
{
	brush_t		*b, *front, *back, *next;
	face_t		*f;
	face_t		split;
	vec3_t		move;
	int			i;

	for (b = selected_brushes.next ; b != &selected_brushes ; b=next)
	{
		next = b->next;

    if (b->owner->eclass->fixedsize || b->patchBrush || b->hiddenBrush)
		  continue;

		for (f = b->brush_faces ; f ; f=f->next)
		{
			split = *f;
			VectorScale (f->plane.normal, g_qeglobals.d_gridsize, move);
			for (i=0 ; i<3 ; i++)
				VectorSubtract (split.planepts[i], move, split.planepts[i]);

			CSG_SplitBrushByFace (b, &split, &front, &back);
			if (back)
				Brush_Free (back);
			if (front)
				Brush_AddToList (front, &selected_brushes);
		}
		Brush_Free (b);
	}
	Sys_UpdateWindows (W_ALL);
}

/*
=============
CSG_Subtract
=============
*/
void CSG_Subtract (void)
{
	brush_t		*b, *s, *frag, *front, *back, *next, *snext;
	face_t		*f;
	int			i;

	Sys_Printf ("Subtracting...\n");

	for (b = selected_brushes.next ; b != &selected_brushes ; b=next)
	{
		next = b->next;

		if (b->owner->eclass->fixedsize)
			continue;	// can't use texture from a fixed entity, so don't subtract

		for (s=active_brushes.next ; s != &active_brushes ; s=snext)
		{
			snext = s->next;

      if (s->owner->eclass->fixedsize || s->patchBrush || s->hiddenBrush)
			continue;

	  	if (FilterBrush (s))
			continue;

      //face_t *pFace = s->brush_faces;	// not used
      if (s->brush_faces->d_texture->bFromShader && (s->brush_faces->d_texture->nShaderFlags & QER_NOCARVE))
      {
        continue;
      }

			for (i=0 ; i<3 ; i++)
				if (b->mins[i] >= s->maxs[i] - ON_EPSILON 
				|| b->maxs[i] <= s->mins[i] + ON_EPSILON)
					break;
			if (i != 3)
				continue;	// definately don't touch

			frag = s;
			for (f = b->brush_faces ; f && frag ; f=f->next)
			{
				CSG_SplitBrushByFace (frag, f, &front, &back);
				Brush_Free (frag);
				frag = back;
				if (front)
					Brush_AddToList (front, &active_brushes);
			}
			if (frag)
				Brush_Free (frag);
		}
	}

	Sys_Printf ("done.\n");
	Sys_UpdateWindows (W_ALL);
}
