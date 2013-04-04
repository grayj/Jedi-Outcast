// Filename:-	models.h
//
//	removed from .c file so I could get at this stuff for the ghoul2 exporter	-ste
//


#ifndef MODELS_H
#define MODELS_H


#ifdef __cplusplus
extern "C"
{
#endif

		#define MAX_SURFACE_TRIS	(SHADER_MAX_INDEXES / 3)
		#define MAX_SURFACE_VERTS	SHADER_MAX_VERTEXES

		#define MD3_TYPE_UNKNOWN 0
		#define MD3_TYPE_BASE3DS 1
		#define MD3_TYPE_SPRITE  2
		#define MD3_TYPE_ASE	 3

		//#define MAX_ANIM_FRAMES		1024
		#define MAX_ANIM_FRAMES		1280

		#define MAX_ANIM_SURFACES	32 + 32

		typedef struct
		{
			polyset_t *frames;
			int numFrames;
			int	valid_frame;
		} SurfaceAnimation_t;

		typedef struct
		{
			polyset_t *surfaces[MAX_ANIM_SURFACES];
			int numSurfaces;
		} ObjectAnimationFrame_t;

		typedef struct {
			vec3_t		xyz;
			vec3_t		normal;
			vec3_t		color;
			float		st[2];
			int			index;
		} baseVertex_t;
			
		typedef struct {
			baseVertex_t	v[3];
		} baseTriangle_t;

		//================================================================

		typedef struct
		{
			md3Surface_t	header;
			md3Shader_t		shaders[MD3_MAX_SHADERS];
			// all verts (xyz_normal)
			float	*verts[MD3_MAX_FRAMES];

			baseTriangle_t	baseTriangles[MD3_MAX_TRIANGLES];

			// the triangles will be sorted so that they form long generalized tristrips
			int				orderedTriangles[MD3_MAX_TRIANGLES][3];
			int				lodTriangles[MD3_MAX_TRIANGLES][3];
			baseVertex_t	baseVertexes[MD3_MAX_VERTS];

		} md3SurfaceData_t;

		typedef struct
		{
			int			skinwidth, skinheight;
			
			md3SurfaceData_t surfData[MD3_MAX_SURFACES];

			md3Tag_t		tags[MD3_MAX_FRAMES][MD3_MAX_TAGS];
			md3Frame_t		frames[MD3_MAX_FRAMES];

			md3Header_t	model;
			float		scale_up;			// set by $scale
			vec3_t		adjust;				// set by $origin
			vec3_t		aseAdjust;
			int			fixedwidth, fixedheight;	// set by $skinsize

			int			maxSurfaceTris;

			int			lowerSkipFrameStart, lowerSkipFrameEnd;
			int			maxUpperFrames;
			int			maxHeadFrames;
			int			currentLod;
			float		lodBias;

			int			type;		// MD3_TYPE_BASE, MD3_TYPE_OLDBASE, MD3_TYPE_ASE, or MD3_TYPE_SPRITE

		} q3data;


		extern q3data g_data;


#ifdef __cplusplus
}
#endif

#endif	// #ifndef MODELS_H

////////////////////// eof /////////////////////





