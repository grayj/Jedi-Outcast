#ifndef __POLYSET_H__
#define __POLYSET_H__

#define POLYSET_MAXTRIANGLES	4096
#define POLYSET_MAXPOLYSETS		64

typedef float st_t[2];
typedef float rgb_t[3];

typedef struct {
	vec3_t	verts[3];
	vec3_t	normals[3];
	st_t	texcoords[3];
	int		normal_recomputed[3];
} triangle_t;

typedef struct
{
	char name[100];
	char materialname[100];
	triangle_t *triangles;
	int numtriangles;
} polyset_t;

typedef struct
{
	triangle_t	*triangle;
	int			which_vert;
	int			can_use;
}normal_compute_t;

polyset_t *Polyset_LoadSets( const char *file, int *numpolysets, int maxTrisPerSet );
polyset_t *Polyset_CollapseSets( polyset_t *psets, int numpolysets );
polyset_t *Polyset_SplitSets( polyset_t *psets, int numpolysets, int *pNumNewPolysets, int maxTris );
void Polyset_SnapSets( polyset_t *psets, int numpolysets );
void Polyset_ComputeNormals( polyset_t *psets, int numpolysets );

#endif
