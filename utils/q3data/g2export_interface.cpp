// Filename:-	g2export_interface.cpp
//
// This file contains interface between the functions that the ghoul2 exporter queries to get model info, and the app itself.

#include <assert.h>
#include "q3data.h"
#include "models.h"
#include "mdx_format.h"
#include "matrix4.h"
//
#include "g2export_interface.h"


int giNumLODs;
int giNumSurfaces;
int giNumTags;


char	*va(char *format, ...)
{	
	va_list		argptr;
	static char		string[8][1024];
	static int i=0;

	i=(i+1)&7;
	
	va_start (argptr, format);
	vsprintf (string[i], format,argptr);
	va_end (argptr);

	return string[i];	
}


// returns (eg) "\dir\name" for "\dir\name.bmp"
//
char *Filename_WithoutExt(LPCSTR psFilename)
{
	static char sString[MAX_PATH];	// *not* MAX_QPATH!
	char *p;
	char *p2;

	strcpy(sString,psFilename);

	while ((p = strchr(sString,'/'))!=NULL) *p='\\';	// q3data filenames have both types of slashes... sigh

	p = strrchr(sString,'.');		
	p2= strrchr(sString,'\\');

	// special check, make sure the first suffix we found from the end wasn't just a directory suffix (eg on a path'd filename with no extension anyway)
	//
	if (p && (p2==0 || (p>p2)))
		*p=0;	

	return sString;

}


static md3SurfaceData_t *GetMD3SurfaceData(int iSurfaceIndex)
{
	// unfortunately, tags are still left as surfaces internally to this app, so I need to skip over them instead of
	//	just using the index directly...(sigh)

	int iSurfNumber = 0;
	for ( int i = 0; i < g_data.model.numSurfaces; i++ )
	{
		md3SurfaceData_t *pSurfData = &g_data.surfData[i];
		if ( strstr( pSurfData->header.name, "tag_" ) != pSurfData->header.name )
		{
			md3Surface_t *psurf = &pSurfData->header;

			if ( psurf->numTriangles == 0 || psurf->numVerts == 0 )
				continue;

			if (iSurfNumber++ == iSurfaceIndex)
			{
				return pSurfData;
			}
		}
	}

	assert(0);
	return NULL;
}

LPCSTR G2Exporter_Surface_GetName(int iSurfaceIndex)
{
	if (iSurfaceIndex < giNumSurfaces)
	{
		// standard surface...
		//		
		md3SurfaceData_t *pSurfaceData = GetMD3SurfaceData(iSurfaceIndex);
		if (pSurfaceData)
		{
			md3Surface_t *pSurf = &pSurfaceData->header;
			
			// return it without any trailing "_n" digits...
			//
			static char sTemp[1024];
			strcpy(sTemp,pSurf->name);
			char *pSuffix = &sTemp[strlen(sTemp)-2];
			if (pSuffix[0] == '_' && isdigit(pSuffix[1]))
			{
				*pSuffix = '\0';
			}
			return sTemp;
		}
	}
	else
	{
		// tag surface...
		//
		static char sTemp[1024];
		md3Tag_t *pTag = &g_data.tags[0][iSurfaceIndex - giNumSurfaces];
		//
		// prepend name with a '*', and remove "tag_" from name start if present...
		//
		strcpy(sTemp,"*");
		strcat(sTemp, (!strnicmp(pTag->name,"tag_",4)) ? &pTag->name[4] : pTag->name);
		return sTemp;
	}

	assert(0);
	return "Error";
}

LPCSTR G2Exporter_Surface_GetShaderName(int iSurfaceIndex)
{	
	if (iSurfaceIndex < giNumSurfaces)
	{
		// standard surface...
		//
		md3SurfaceData_t *pSurfaceData = GetMD3SurfaceData(iSurfaceIndex);
		if (pSurfaceData)
		{
			return pSurfaceData->shaders[0].name;
		}
	}
	else
	{
		// tag surface...
		//
		return "[NoMaterial]";
	}

	assert(0);
	return "Error";
}

int G2Exporter_Surface_GetNumVerts(int iSurfaceIndex, int iLODIndex)
{
	if (iLODIndex == giNumLODs-1)
	{
		// q3data surface...
		//
		if (iSurfaceIndex < giNumSurfaces)
		{
			// standard surface...
			//
			md3SurfaceData_t *pSurfaceData = GetMD3SurfaceData(iSurfaceIndex);
			if (pSurfaceData)
			{
				md3Surface_t *pSurf = &pSurfaceData->header;
				return pSurf->numVerts;
			}
		}
		else
		{
			// tag surface...
			//
			return 3;	// for one triangle
		}
	}
	else
	{
		// imported surface...
		//
		return ImportedModel.ImportedLODs[iLODIndex].ImportedSurfaces[ImportedModel.SurfaceIndexRemaps[iSurfaceIndex]].ImportedVerts.size();
	}

	assert(0);
	return 0;
}

int G2Exporter_Surface_GetNumTris(int iSurfaceIndex, int iLODIndex)
{
	if (iLODIndex == giNumLODs-1)
	{
		// q3data surface...
		//
		if (iSurfaceIndex < giNumSurfaces)
		{
			// standard surface...
			//
			md3SurfaceData_t *pSurfaceData = GetMD3SurfaceData(iSurfaceIndex);
			if (pSurfaceData)
			{
				md3Surface_t *pSurf = &pSurfaceData->header;
				return pSurf->numTriangles;
			}
		}
		else
		{
			// tag surface...
			//
			return 1;	// for one triangle
		}
	}
	else
	{
		// imported surface...
		//
		return ImportedModel.ImportedLODs[iLODIndex].ImportedSurfaces[ImportedModel.SurfaceIndexRemaps[iSurfaceIndex]].ImportedTris.size();
	}

	assert(0);
	return 0;
}

int G2Exporter_Surface_GetTriIndex(int iSurfaceIndex, int iTriangleIndex, int iTriVert, int iLODIndex)
{
	if (iLODIndex == giNumLODs-1)
	{
		// q3data surface...
		//
		if (iSurfaceIndex < giNumSurfaces)
		{
			// standard surface...
			//
			md3SurfaceData_t *pSurfaceData = GetMD3SurfaceData(iSurfaceIndex);
			if (pSurfaceData)
			{
				return pSurfaceData->orderedTriangles[iTriangleIndex][iTriVert];
			}
		}
		else
		{
			// tag surface...
			//
			return iTriVert;
		}
	}
	else
	{
		// imported surface...
		//
		return ImportedModel.ImportedLODs[iLODIndex].ImportedSurfaces[ImportedModel.SurfaceIndexRemaps[iSurfaceIndex]].ImportedTris[iTriangleIndex].indexes[iTriVert];
	}

	assert(0);
	return 0;
}

vec3_t *G2Exporter_Surface_GetVertNormal(int iSurfaceIndex, int iVertIndex, int iLODIndex)
{
	static vec3_t v3={0};
	memset(v3,0,sizeof(v3));

	if (iLODIndex == giNumLODs-1)
	{
		// q3data surface...
		//
		if (iSurfaceIndex < giNumSurfaces)
		{
			// standard surface...
			//
			md3SurfaceData_t *pSurfaceData = GetMD3SurfaceData(iSurfaceIndex);
			if (pSurfaceData)
			{
				// this logic is kinda gay, not sure why the *6 etc, but that's how other q3data code works, so...
				//
				float **ppVerts = pSurfaceData->verts;
				memcpy(v3,(vec3_t*) &ppVerts[0][iVertIndex*6+3], sizeof(v3));

				{		
					Matrix4 Swap;
							Swap.Identity();
					
					if (1)
					{
						Swap.SetRow(0,Vect3(0.0f,-1.0f,0.0f));
						Swap.SetRow(1,Vect3(1.0f,0.0f,0.0f));
					}
					Swap.CalcFlags();

					Vect3 v3In((const float *)v3);
					static Vect3 v3Out;
					Swap.XFormVect(v3Out,v3In);
					return (vec3_t*) &v3Out;
				}
			}
		}
		else
		{
			// tag surface...
			//
			// I don't think tag-surfaces normals have any meaning for ghoul2, so...
			//		
			return &v3;
		}
	}
	else
	{
		// imported surface...
		//
		vec3_t &v3Src = ImportedModel.ImportedLODs[iLODIndex].ImportedSurfaces[ImportedModel.SurfaceIndexRemaps[iSurfaceIndex]].ImportedVerts[iVertIndex].normal;
		memcpy(v3,v3Src,sizeof(v3));
		return &v3;
	}

	assert(0);	
	return &v3;
}

vec3_t *G2Exporter_Surface_GetVertCoords(int iSurfaceIndex, int iVertIndex, int iLODIndex)
{
	static vec3_t v3={0};
	memset(&v3,0,sizeof(v3));

	if (iLODIndex == giNumLODs-1)
	{
		// q3data surface...
		//
		if (iSurfaceIndex < giNumSurfaces)
		{
			// standard surface...
			//
			md3SurfaceData_t *pSurfaceData = GetMD3SurfaceData(iSurfaceIndex);
			if (pSurfaceData)
			{
				// this logic is kinda gay, not sure why the *6 etc, but that's how other q3data code works, so...
				//
				float **ppVerts = pSurfaceData->verts;
				static vec3_t v3;
				for (int i=0; i<3; i++)
				{
					v3[i] = ppVerts[0][iVertIndex*6+i];// /MD3_XYZ_SCALE;
				}
				// return &v3;
					
				Matrix4 Swap;
						Swap.Identity();
				
				if (1)
				{
					Swap.SetRow(0,Vect3(0.0f,-1.0f,0.0f));
					Swap.SetRow(1,Vect3(1.0f,0.0f,0.0f));
				}
				Swap.CalcFlags();

				Vect3 v3In((const float *)v3);
				static Vect3 v3Out;
				Swap.XFormVect(v3Out,v3In);
				return (vec3_t*) &v3Out;
			}
		}
		else
		{
			// tag surface...
			//
			assert(iVertIndex<3);

			md3Tag_t *pTag = &g_data.tags[0][iSurfaceIndex - giNumSurfaces];			
			vec3_t v3New;

	//#ifdef PERFECT_CONVERSION
			
			v3New[0] = pTag->axis[0][iVertIndex] ;
			v3New[1] = pTag->axis[1][iVertIndex] ;
			v3New[2] = pTag->axis[2][iVertIndex] ;

			// don't worry about how this crap works, it just does (arrived at by empirical methods... :-)
			//
			//  (mega-thanks to Gil as usual)
			//
			if (iVertIndex==2)
			{
				VectorCopy(pTag->origin,v3);
			}
			else
			if (iVertIndex==1)
			{
				v3New[0] =   2.0f * pTag->axis[1][iG2_TRISIDE_MIDDLE];
				v3New[1] = -(2.0f * pTag->axis[0][iG2_TRISIDE_MIDDLE]);
				v3New[2] =   2.0f * pTag->axis[2][iG2_TRISIDE_MIDDLE];
				
				VectorSubtract(pTag->origin,v3New,v3);
			}
			else
			{					
				v3New[0] =  pTag->axis[1][iG2_TRISIDE_LONGEST];
				v3New[1] = -pTag->axis[0][iG2_TRISIDE_LONGEST];
				v3New[2] =  pTag->axis[2][iG2_TRISIDE_LONGEST];
				
				VectorSubtract(pTag->origin,v3New,v3);
			}

	//		return (vec3_t*) &v3;

			Matrix4 Swap;
					Swap.Identity();
			
			if (1)
			{
				Swap.SetRow(0,Vect3(0.0f,-1.0f,0.0f));
				Swap.SetRow(1,Vect3(1.0f,0.0f,0.0f));
			}
			Swap.CalcFlags();

			Vect3 v3In((const float *)v3);
			static Vect3 v3Out;
			Swap.XFormVect(v3Out,v3In);

			return (vec3_t*) &v3Out;
		}
	}
	else
	{
		// imported surface...
		//
		vec3_t &v3Src = ImportedModel.ImportedLODs[iLODIndex].ImportedSurfaces[ImportedModel.SurfaceIndexRemaps[iSurfaceIndex]].ImportedVerts[iVertIndex].vertCoords;
		memcpy(v3,v3Src,sizeof(v3));
		return &v3;
	}

	assert(0);
	return &v3;
}

vec2_t *G2Exporter_Surface_GetTexCoords(int iSurfaceIndex, int iVertIndex, int iLODIndex)
{
	static vec2_t v2={0};
	memset(v2,0,sizeof(v2));

	if (iLODIndex == giNumLODs-1)
	{
		// q3data surface...
		//
		if (iSurfaceIndex < giNumSurfaces)
		{
			// standard surface...
			//
			md3SurfaceData_t *pSurfaceData = GetMD3SurfaceData(iSurfaceIndex);
			if (pSurfaceData)
			{
				baseVertex_t *pBaseVertex = pSurfaceData->baseVertexes;
				return (vec2_t*) &pBaseVertex[iVertIndex].st[0];
			}
		}
		else
		{					
			// tag surface...
			//
			// Texture coords aren't relevant for tag-surfaces, so...
			//
			return &v2;
		}
	}
	else
	{
		// imported surface...
		//
		vec2_t &v2Src = ImportedModel.ImportedLODs[iLODIndex].ImportedSurfaces[ImportedModel.SurfaceIndexRemaps[iSurfaceIndex]].ImportedVerts[iVertIndex].texCoords;
		memcpy(v2,v2Src,sizeof(v2));
		return &v2;
	}

	assert(0);
	return &v2;
}


//////////////////// eof /////////////////////

