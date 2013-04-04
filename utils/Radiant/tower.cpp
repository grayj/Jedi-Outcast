/*
	The tower command will take two brushes at different Z's and
	create a brush between the two.  For this to work, the low Z plane
	of the upper brush and the high Z plane of the lower brush need to
	have the same number of sides
*/

#include "stdafx.h"
#include "tower.h"

//from Brush.cpp
face_t *Face_Alloc( void );
winding_t	*NewWinding (int points);

void DoTower(void)
{
	brush_t*	brush1 = NULL;
	brush_t*	brush2 = NULL;
	brush_t*	newBrush = NULL;
	brush_t*	tempBrush = NULL;
	face_t*		topFace = NULL;
	face_t*		bottomFace = NULL;
	face_t*		curFace;
	face_t*		faceList;
	int			zFlag = 0;
	int			i = 0; // loop counter

	if (g_qeglobals.d_select_count != 2)
	{
		Sys_Printf ("Error: you must have exactly 2 brushes selected\n");
		Sys_Beep ();
		return;
	}
	brush1 = selected_brushes.next;
	brush2 = selected_brushes.next->next;
//	establish brush1 as the upper brush
	if (brush2->maxs[2] > brush1->mins[2])
	{
		tempBrush = brush1;
		brush1 = brush2;
		brush2 = tempBrush;
	}
//	test to insure brushes do not "overlap" in the Z direction
	if (brush2->maxs[2] > brush1->mins[2])
	{
		Sys_Printf ("Brushes are not separated in the Z direction!");
		Sys_Beep();
		return;
	}
//  find the bottom Z plane (topFace) in 1 and top Z plane in 2 (bottomFace)
	topFace = brush1->brush_faces;
	while (topFace != NULL)
	{
		zFlag = 0;
		for (i = 0; i<3; i++)
		{
			if (abs(topFace->planepts[i][2] - brush1->mins[2]) < TOWER_EPSILON)
			{
				zFlag++;
			}
		}
		if (zFlag == 3)
		{
			break;
		}
		else
		{
			topFace = topFace->next;
		}
	}
	if (topFace == NULL)
	{
		Sys_Printf("Couldn't find flat bottom-face in top brush", 0);
		Sys_Beep();
		return;
	}
	bottomFace = brush2->brush_faces;
	while (bottomFace != NULL)
	{
		zFlag = 0;
		for (i = 0; i<3; i++)
		{
			if (abs(bottomFace->planepts[i][2] - brush2->maxs[2]) < TOWER_EPSILON)
			{
				zFlag++;
			}
		}
		if (zFlag == 3)
		{
			break;
		}
		else
		{
			bottomFace = bottomFace->next;
		}
	}
	if (bottomFace == NULL)
	{
		Sys_Printf ("Couldn't find flat top-face in bottom brush", 0);
		Sys_Beep();
		return;
	}
//	count vertices on top and bottom planes to make sure they are equal
	if (topFace->face_winding->numpoints != bottomFace->face_winding->numpoints)
	{

		Sys_Printf ("Top and Bottom faces don't have same #'s of vertices!", 0);
		Sys_Beep();
		return;
	}
//  put top and bottom faces on brush
//  reverse winding for top and bottom
	faceList = Face_Alloc();
	for ( i = 0; i<3; i++)
	{
		VectorCopy(topFace->planepts[2-i],faceList->planepts[i]);
	}
	curFace = Face_Alloc();
	for ( i = 0; i < 3; i++)
	{
		VectorCopy(bottomFace->planepts[2-i],curFace->planepts[i]);
	}
	curFace->next = faceList;
	faceList = curFace;
	
	curFace = MakePlaneList(topFace, bottomFace);
	if (curFace == NULL)
	{
		Sys_Printf ("Couldn't make planes for tower!", 0);
		Sys_Beep();
		return;
	}
	else
	{
		faceList->next->next = curFace;
	}

	newBrush = (brush_t*)qmalloc(sizeof(brush_t));
	newBrush->brush_faces = faceList;
	Select_Deselect();
	Brush_AddToList (newBrush, &selected_brushes);

	Entity_LinkBrush (world_entity, newBrush);

	Brush_Build(newBrush);
//	UNDO_FinishBrushAdd("&Undo Tower");
	Sys_UpdateWindows(W_ALL);
	return;

}

face_t* MakePlaneList(face_t* top, face_t* bottom)
{
	face_t*		fList;
	face_t*		curFace;
	face_t*		newTop;
	face_t*		newBot;
	int			i;
	int			j;
	int			k;
	vec3_t		t1, t2, t3;
	
	fList = NULL;
	newTop = Face_Alloc();
	CopyFace(top, newTop);
	newBot = Face_Alloc();
	CopyFace(bottom, newBot);
	WrapFaces(newTop, newBot);
	for (i = 0; i<newTop->face_winding->numpoints; i++)
	{
		if (i == (newTop->face_winding->numpoints - 1))
		{
			j = 0;
		}
		else
		{
			j = i + 1;
		}
		
		curFace = Face_Alloc();
		VectorCopy(newTop->face_winding->points[j],curFace->planepts[0]);
		VectorCopy(newTop->face_winding->points[i],curFace->planepts[1]);
		VectorCopy(newBot->face_winding->points[i],curFace->planepts[2]);
	
		for (k=0 ; k<3 ; k++)
		{
			t1[k] = curFace->planepts[0][k] - curFace->planepts[1][k];
			t2[k] = curFace->planepts[2][k] - curFace->planepts[1][k];
			t3[k] = curFace->planepts[1][k];
		}
		
		CrossProduct(t1,t2, curFace->plane.normal);
		if (VectorCompare (curFace->plane.normal, vec3_origin))
		{
			printf ("WARNING: brush plane with no normal\n");
		
		}
		VectorNormalize (curFace->plane.normal);
		curFace->plane.dist = DotProduct (t3, curFace->plane.normal);
	
		curFace->next = fList;
		fList = curFace;
  	}	// for loop
	return fList;
}


void WrapFaces( face_t* top, face_t* bottom)
{
	face_t*	tempFace;
	int		i;	
	float	maxX;
	float	maxY;
	int pointFlag;

	tempFace = Face_Alloc();
	//wrap the top face points the other way
	CopyFace(top, tempFace);
	for ( i = 0; i<top->face_winding->numpoints; i++)
	{
		VectorCopy(top->face_winding->points[top->face_winding->numpoints-1-i],tempFace->face_winding->points[i]);
	}
	CopyFace(tempFace,top);
	// top and bottom are now wrapped with normals pointing upward
	// now grab the point in top with most positive x (and y if there are more than one)
	maxX = top->face_winding->points[0][0];
	maxY = top->face_winding->points[0][1];
	pointFlag = 0;

	for ( i = 1; i<top->face_winding->numpoints; i++)
	{
		if ( maxX > top->face_winding->points[i][0] )
		{
			continue;
		}
		else
		{
			if ( maxX == top->face_winding->points[i][0] )
			{
				if (top->face_winding->points[i][1] > maxY)
				{
					maxY = top->face_winding->points[i][1];
					pointFlag = i;
				}
			}
			else
			{
				maxX = top->face_winding->points[i][0];
				maxY = top->face_winding->points[i][1];
				pointFlag = i;
			}
		}
	}
//  now, starting at the point[pointflag] in top, write the sequence starting at [0] in tempFace
	for ( i = 0; i<top->face_winding->numpoints; i++)
	{
		if (pointFlag == top->face_winding->numpoints)
		{
			pointFlag = 0;
		}
		VectorCopy(top->face_winding->points[pointFlag], tempFace->face_winding->points[i]);
		pointFlag++;
	}
	CopyFace(tempFace,top);
//repeat with bottom
	CopyFace(bottom, tempFace);
	maxX = bottom->face_winding->points[0][0];
	maxY = bottom->face_winding->points[0][1];
	pointFlag = 0;

	for ( i = 1; i<bottom->face_winding->numpoints; i++)
	{
		if ( maxX > bottom->face_winding->points[i][0] )
		{
			continue;
		}
		else
		{
			if ( maxX == bottom->face_winding->points[i][0] )
			{
				if (bottom->face_winding->points[i][1] > maxY)
				{
					maxY = bottom->face_winding->points[i][1];
					pointFlag = i;
				}
			}
			else
			{
				maxX = bottom->face_winding->points[i][0];
				maxY = bottom->face_winding->points[i][1];
				pointFlag = i;
			}
		}
	}
//  now, starting at the point[pointflag] in bottom, write the sequence starting at [0] in tempFace
	for ( i = 0; i<bottom->face_winding->numpoints; i++)
	{
		if (pointFlag == bottom->face_winding->numpoints)
		{
			pointFlag = 0;
		}
		VectorCopy(bottom->face_winding->points[pointFlag], tempFace->face_winding->points[i]);
		pointFlag++;
	}
	CopyFace(tempFace,bottom);
	Face_Free(tempFace);
	return;
}

void CopyFace( face_t* in, face_t* out)
{
	int		i;

	out->face_winding = NewWinding(in->face_winding->numpoints);
	out->face_winding->numpoints = in->face_winding->numpoints;
	for (i = 0; i<in->face_winding->numpoints; i++)
	{
		VectorCopy(in->face_winding->points[i],out->face_winding->points[i]);
	}
	for (i = 0; i<3; i++)
	{
		VectorCopy(in->planepts[i],out->planepts[i]);
	}
}
