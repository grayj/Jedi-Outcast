// Filename:-	g2export_interface.h
//


#ifndef G2_EXPORT_INTERFACE_H
#define G2_EXPORT_INTERFACE_H

#ifndef LPCSTR
#define LPCSTR const char *
#endif

#ifndef MAX_PATH
#define MAX_PATH 160	// *not* MAX_QPATH
#endif



char	*va(char *format, ...);
char *Filename_WithoutExt(LPCSTR psFilename);


extern int giNumLODs;
extern int giNumSurfaces;
extern int giNumTags;



// ghoul2 exporter interface functions...
//
LPCSTR G2Exporter_Surface_GetName(int iSurfaceIndex);
LPCSTR G2Exporter_Surface_GetShaderName(int iSurfaceIndex);
int G2Exporter_Surface_GetNumVerts	(int iSurfaceIndex, int iLODIndex);
int G2Exporter_Surface_GetNumTris	(int iSurfaceIndex, int iLODIndex);
int G2Exporter_Surface_GetTriIndex	(int iSurfaceIndex, int iTriangleIndex, int iTriVert, int iLODIndex);
vec3_t *G2Exporter_Surface_GetVertNormal(int iSurfaceIndex, int iVertIndex, int iLODIndex);
vec3_t *G2Exporter_Surface_GetVertCoords(int iSurfaceIndex, int iVertIndex, int iLODIndex);
vec2_t *G2Exporter_Surface_GetTexCoords(int iSurfaceIndex, int iVertIndex, int iLODIndex);


#pragma warning ( disable : 4786)
#include <vector>
#include <map>
using namespace std;
#pragma warning ( disable : 4786)



// ghoul2 import structures used when appending to previous versions to add extra LODs...
//
typedef struct {
	vec3_t			normal;
	vec3_t			vertCoords;
	vec2_t			texCoords;										
} GLM_ImportVertex_t;

typedef struct
{
	vector <GLM_ImportVertex_t> ImportedVerts;
	vector <mdxmTriangle_t>		ImportedTris;
} GLM_ImportSurface_t;

typedef struct
{
	vector <GLM_ImportSurface_t> ImportedSurfaces;
} GLM_ImportLOD_t;

typedef struct
{
	vector <GLM_ImportLOD_t> ImportedLODs;
	map    <int,int>		 SurfaceIndexRemaps;  // key = q3data surface index, value = corresponding index in imported model
} GLM_ImportModel_t;

extern GLM_ImportModel_t ImportedModel;


#endif	// #ifndef G2_EXPORT_INTERFACE_H


//////////////////////// eof //////////////////////////


