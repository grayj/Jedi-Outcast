// Filename:-	autocaulk.cpp
//
#include "stdafx.h"
#include "qe3.h"
#include "oddbits.h"
//
#include "autocaulk.h"


#pragma warning( disable : 4786)
#include <list>
using namespace std;
#pragma warning( disable : 4786)


// for some tedious reason, I can't include "brush.h" here, so...
//
extern void ClearBounds (vec3_t mins, vec3_t maxs);
extern void AddPointToBounds (const vec3_t v, vec3_t mins, vec3_t maxs);

static void FloorBounds(vec3_t mins, vec3_t maxs)
{
	for (int i=0 ; i<3 ; i++)
	{
		mins[i] = floor(mins[i] + 0.5);
		maxs[i] = floor(maxs[i] + 0.5);
	}
}

/*
typedef struct
{
	vec3_t v3Mins;
	vec3_t v3Maxs;
	vec3_t v3TL,v3TR,v3BL,v3BR;
	vec3_t v3NormalisedRotationVector;
	vec3_t v3NormalisedElevationVector;

} SquaredFace_t;

// make an artificial square face that encompasses all the points in the winding...
//
static void WindingToSquaredFace( SquaredFace_t *pSquareFaceDest, winding_t *pWinding)
{
//	memset(pSquareFaceDest,0,sizeof(*pSquareFaceDest);

	ClearBounds(pSquareFaceDest->v3Mins, pSquareFaceDest->v3Maxs);

	for (int i=0; i<pWinding->numpoints ; i++)
	{
		AddPointToBounds( pWinding->points[i], pSquareFaceDest->v3Mins, pSquareFaceDest->v3Maxs );
	}

	VectorCopy(pSquareFaceDest->v3Mins,	pSquareFaceDest->v3BL);
	VectorCopy(pSquareFaceDest->v3Maxs,	pSquareFaceDest->v3TR);

	VectorCopy(pSquareFaceDest->v3Mins,	pSquareFaceDest->v3TL);
										pSquareFaceDest->v3TL[2] = pSquareFaceDest->v3Maxs[2];
	VectorCopy(pSquareFaceDest->v3Maxs,	pSquareFaceDest->v3BR);
										pSquareFaceDest->v3BR[2] = pSquareFaceDest->v3Mins[2];

	VectorSubtract( pSquareFaceDest->v3TR, pSquareFaceDest->v3TL, pSquareFaceDest->v3NormalisedRotationVector);
	VectorNormalize(pSquareFaceDest->v3NormalisedRotationVector);

	VectorSubtract( pSquareFaceDest->v3TL, pSquareFaceDest->v3BL, pSquareFaceDest->v3NormalisedElevationVector);
	VectorNormalize(pSquareFaceDest->v3NormalisedElevationVector);
}
*/

static LPCSTR vtos(vec3_t v3)
{
	return va("%.3ff,%.3f,%.3f",v3[0],v3[1],v3[2]);
}
list < pair < face_t*, brush_t*> > FacesToCaulk;
void Select_AutoCaulk(bool bMakeDetail)
{
	Sys_Printf ("Caulking...\n");

	FacesToCaulk.clear();

	int iSystemBrushesSkipped = 0;

	brush_t *next;
	for (brush_t *pSelectedBrush = selected_brushes.next ; pSelectedBrush != &selected_brushes ; pSelectedBrush = next)
	{
		next = pSelectedBrush->next;
		
		if (pSelectedBrush->owner->eclass->fixedsize)
			continue;	// apparently this means it's a model, so skip it...

		// new check, we can't caulk a brush that has any "system/" faces...
		//
		bool bSystemFacePresent = false;
		for (face_t *pSelectedFace = pSelectedBrush->brush_faces; pSelectedFace; pSelectedFace = pSelectedFace->next)
		{				
			if (!strnicmp(pSelectedFace->d_texture->name,"system/",7))
			{
				bSystemFacePresent = true;
				break;
			}
		}
		if (bSystemFacePresent)
		{
			iSystemBrushesSkipped++;
			continue;	// verboten to caulk this.
		}		

		for (int iBrushListToScan = 0; iBrushListToScan<2; iBrushListToScan++)
		{		
			brush_t	*snext;
			for (brush_t *pScannedBrush = (iBrushListToScan?active_brushes.next:selected_brushes.next); pScannedBrush != (iBrushListToScan?&active_brushes:&selected_brushes) ; pScannedBrush = snext)
			{
				snext = pScannedBrush->next;

				if ( pScannedBrush == pSelectedBrush)
					continue;
				
				if (pScannedBrush->owner->eclass->fixedsize || pScannedBrush->patchBrush || pScannedBrush->hiddenBrush)
					continue;

		  		if (FilterBrush(pScannedBrush))
					continue;
				
				//face_t *pFace = pScannedBrush->brush_faces;
				if (pScannedBrush->brush_faces->d_texture->bFromShader && (pScannedBrush->brush_faces->d_texture->nShaderFlags & QER_NOCARVE))
					continue;
				
				// basic-reject first to see if brushes can even possibly touch (coplanar counts as touching)
				//
				for (int i=0 ; i<3 ; i++)
				{
					if (pSelectedBrush->mins[i] > pScannedBrush->maxs[i] ||
						pSelectedBrush->maxs[i] < pScannedBrush->mins[i])
					{
						break;
					}
				}
				if (i != 3)
					continue;	// can't be touching

				// ok, now for the clever stuff, we need to detect only those faces that are both coplanar and smaller
				//	or equal to the face they're coplanar with...
				//
				for (pSelectedFace = pSelectedBrush->brush_faces; pSelectedFace; pSelectedFace = pSelectedFace->next)
				{
					winding_t *pSelectedWinding = pSelectedFace->face_winding;

					if (!pSelectedWinding)
						continue;	// freed face, probably won't happen here, but who knows with this program?

	//				SquaredFace_t SelectedSquaredFace;
	//				WindingToSquaredFace( &SelectedSquaredFace, pSelectedWinding);

					for (face_t *pScannedFace = pScannedBrush->brush_faces; pScannedFace; pScannedFace = pScannedFace->next)
					{
						// don't even try caulking against a system face, because these are often transparent and will leave holes
						//
						if (!strnicmp(pScannedFace->d_texture->name,"system/",7))
							continue;

						// and don't try caulking against something inherently transparent...
						//
						if (pScannedFace->d_texture->nShaderFlags & QER_TRANS)
							continue;

						winding_t *pScannedWinding = pScannedFace->face_winding;

						if (!pScannedWinding)
							continue;	// freed face, probably won't happen here, but who knows with this program?

	//					SquaredFace_t ScannedSquaredFace;
	//					WindingToSquaredFace( &ScannedSquaredFace, pScannedWinding);

	/*					if (VectorCompare(ScannedSquaredFace.v3NormalisedRotationVector, SelectedSquaredFace.v3NormalisedRotationVector)
							&& 
							VectorCompare(ScannedSquaredFace.v3NormalisedElevationVector, SelectedSquaredFace.v3NormalisedElevationVector)
							)
	*/
						{
							// brush faces are in parallel planes to each other, so check that their normals
							//	are opposite, by adding them together and testing for zero...
							// (if normals are opposite, then faces can be against/touching each other?)						
							//
							vec3_t v3ZeroTest;
							static vec3_t v3Zero={0,0,0};
							VectorAdd(pSelectedFace->plane.normal,pScannedFace->plane.normal,v3ZeroTest);
							if (VectorCompare(v3ZeroTest,v3Zero))
							{
								// planes are facing each other...
								//
								// coplanar? (this is some maths of Gil's, which I don't even pretend to understand)
								//
								float fTotalDist = 0;
								for (int _i=0; _i<3; _i++)
								{
									fTotalDist += fabs(	DotProduct(pSelectedFace->plane.normal,pSelectedWinding->points[0]) 
														- 
														DotProduct(pSelectedFace->plane.normal,pScannedWinding->points[i])
														);
								}
								//OutputDebugString(va("Dist = %g\n",fTotalDist));
								
								if (fTotalDist > 0.01)
									continue;

								// every point in the selected face must be within (or equal to) the bounds of the
								//	scanned face...
								//
								// work out the bounds first...
								//
								vec3_t v3ScannedBoundsMins, v3ScannedBoundsMaxs;
								ClearBounds (v3ScannedBoundsMins, v3ScannedBoundsMaxs);
								for (int iPoint=0; iPoint<pScannedWinding->numpoints; iPoint++)
								{
									AddPointToBounds( pScannedWinding->points[iPoint], v3ScannedBoundsMins, v3ScannedBoundsMaxs);
								}
								// floor 'em... (or .001 differences mess things up...
								//
								FloorBounds(v3ScannedBoundsMins, v3ScannedBoundsMaxs);

								
								// now check points from selected face...
								//
								bool bWithin = true;
								for (iPoint=0; iPoint < pSelectedWinding->numpoints; iPoint++)
								{
									for (int iXYZ=0; iXYZ<3; iXYZ++)
									{
										float f = floor(pSelectedWinding->points[iPoint][iXYZ] + 0.5);
										if (!
												(
												f >= v3ScannedBoundsMins[iXYZ] 
												&&
												f <= v3ScannedBoundsMaxs[iXYZ]
												)
											 )
										{
											bWithin = false;
										}
									}
								}

								if (bWithin)
								{
									FacesToCaulk.push_back( pair<face_t*, brush_t*>(pSelectedFace,pSelectedBrush));
									
									/*
									OutputDebugString("Pair:\n");
									OutputDebugString(vtos(pSelectedFace->plane.normal));
									OutputDebugString("\n");
									OutputDebugString(vtos(pScannedFace->plane.normal));
									OutputDebugString("\n");

									int z=1;
									*/
								}
							}
						}
					}
				}			
			}
		}
	}

	
	// apply caulk...
	//
	int iFacesCaulked = 0;
	if (FacesToCaulk.size())
	{
		LPCSTR psCaulkName = "system/caulk";
		qtexture_t *pCaulk = Texture_ForName(psCaulkName);

		if (pCaulk)
		{
			//
			// and call some other junk that Radiant wants so so we can use it later...
			//
			texdef_t tex;
			memset (&tex, 0, sizeof(tex));
			tex.scale[0] = 1;
			tex.scale[1] = 1;
			tex.flags = pCaulk->flags;
			tex.value = pCaulk->value;
			tex.contents = pCaulk->contents;
			strcpy (tex.name, pCaulk->name);

			//Texture_SetTexture (&tex);

			for (list< pair < face_t*, brush_t*> >::iterator it = FacesToCaulk.begin(); it!= FacesToCaulk.end(); ++it)
			{
				face_t *pFace = (*it).first;
				brush_t*pBrush= (*it).second;

				memcpy(&pFace->texdef,&tex,sizeof(tex));

				Face_FitTexture(pFace, 1, 1);	// this doesn't work here for some reason... duh.
				Brush_Build(pBrush);

				iFacesCaulked++;
			}		
		}
		else
		{
			Sys_Printf(" Unable to locate caulk texture at: \"%s\"!\n",psCaulkName);
		}
	}

	Sys_Printf("( %d faces caulked )\n",iFacesCaulked);

	if (iSystemBrushesSkipped)
	{
		Sys_Printf("( %d system-faced brushes skipped )\n",iSystemBrushesSkipped);
	}

	if (bMakeDetail)
	{
		Select_MakeDetail( false );
	}

	Sys_UpdateWindows (W_ALL);
}

