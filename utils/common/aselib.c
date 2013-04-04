#include "aselib.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ASE_MATERIALS			32
#define MAX_ASE_OBJECTS				64 + 32
#define MAX_ASE_ANIMATIONS			32
//#define MAX_ASE_ANIMATION_FRAMES	1024
#define MAX_ASE_ANIMATION_FRAMES	1280

#define VERBOSE( x ) { if ( ase.verbose ) { printf x ; } }

typedef struct
{
	float x, y, z;
	float nx, ny, nz;
	float s, t;
} aseVertex_t;

typedef struct
{
	float s, t;
} aseTVertex_t;

typedef int aseFace_t[3];

typedef struct
{
	int numFaces;
	int numVertexes;
	int numTVertexes;

	int timeValue;

	aseVertex_t		*vertexes;
	aseTVertex_t	*tvertexes;
	aseFace_t		*faces, *tfaces;

	int currentFace, currentVertex;
} aseMesh_t;

typedef struct
{
	int			numFrames;
	aseMesh_t	frames[MAX_ASE_ANIMATION_FRAMES];

	int		   currentFrame;
} aseMeshAnimation_t;

typedef struct
{
	char name[128];
} aseMaterial_t;

/*
** contains the animate sequence of a single surface
** using a single material
*/
typedef struct
{
	char name[128];

	int materialRef;
	int numAnimations;

	aseMeshAnimation_t	anim;

} aseGeomObject_t;

typedef struct
{
	int				numMaterials;
	aseMaterial_t	materials[MAX_ASE_MATERIALS];
	aseGeomObject_t objects[MAX_ASE_OBJECTS];

	char	*buffer;
	char	*curpos;
	int		 len;

	int			currentObject;
	qboolean	verbose;
	qboolean	grabAnims;

} ase_t;


static char s_token[1024];
static ase_t ase;
static ase_t aseGrab;

static void ASE_Process( int type );
static void ASE_FreeGeomObject( int ndx );

/*
** ASE_Load
*/
void ASE_Load( const char *filename, qboolean verbose, qboolean grabAnims, int type )
{
	FILE *fp = fopen( filename, "rb" );

	if ( !fp )
		Error( "File not found '%s'", filename );

	memset( &ase, 0, sizeof( ase ) );

	ase.verbose = verbose;
	ase.grabAnims = grabAnims;
	ase.len = Q_filelength( fp );

	ase.curpos = ase.buffer = malloc( ase.len );
	assert (ase.buffer != NULL);

	printf( "Processing '%s'\n", filename );

	if ( fread( ase.buffer, ase.len, 1, fp ) != 1 )
	{
		fclose( fp );
		Error( "fread() != 1 for '%s'", filename );
	}

	fclose( fp );

	ASE_Process(type);
	free (ase.buffer);
}

/*
** ASE_Free
*/
void ASE_Free( void )
{
	int i;

	for ( i = 0; i < ase.currentObject; i++ )
	{
		ASE_FreeGeomObject( i );
	}
}

/*
** ASE_GetNumSurfaces
*/
int ASE_GetNumSurfaces( void )
{
	return ase.currentObject;
}

/*
** ASE_GetSurfaceName
*/
const char *ASE_GetSurfaceName( int which )
{
	aseGeomObject_t *pObject = &ase.objects[which];

	if ( !pObject->anim.numFrames )
		return "";

	return pObject->name;
}

/*
** ASE_GetSurfaceAnimation
**
** Returns an animation (sequence of polysets)
*/
polyset_t *ASE_GetSurfaceAnimation( int which, int *pNumFrames, int skipFrameStart, int skipFrameEnd, int maxFrames )
{
	aseGeomObject_t *pObject = &ase.objects[which];
	polyset_t *psets;
	int numFramesInAnimation;
	int numFramesToKeep;
	int i, f;

	if ( !pObject->anim.numFrames ) {
			printf( "WARNING: ASE_GetSurfaceAnimation no frames on %s!\n", pObject->name);
		return 0;
	}

	if ( pObject->anim.numFrames >= maxFrames && maxFrames != -1 )
	{
		numFramesInAnimation = maxFrames;
	}
	else 
	{
		numFramesInAnimation = pObject->anim.numFrames;
		if ( maxFrames != -1 )
			printf( "WARNING: ASE_GetSurfaceAnimation maxFrames(%d) > numFramesInAnimation(%d)\n",numFramesInAnimation, maxFrames );
	}

	if ( skipFrameEnd != -1 )
		numFramesToKeep = numFramesInAnimation - ( skipFrameEnd - skipFrameStart + 1 );
	else
		numFramesToKeep = numFramesInAnimation;

	*pNumFrames = numFramesToKeep;

	psets = calloc( sizeof( polyset_t ) * numFramesToKeep, 1 );

	for ( f = 0, i = 0; i < numFramesInAnimation; i++ )
	{
		int t;
		aseMesh_t *pMesh = &pObject->anim.frames[i];

		if ( skipFrameStart != -1 )
		{
			if ( i >= skipFrameStart && i <= skipFrameEnd )
				continue;
		}

		strcpy( psets[f].name, pObject->name );
		strcpy( psets[f].materialname, ase.materials[pObject->materialRef].name );

		psets[f].triangles = calloc( sizeof( triangle_t ) * pObject->anim.frames[i].numFaces, 1 );
		psets[f].numtriangles = pObject->anim.frames[i].numFaces;

		for ( t = 0; t < pObject->anim.frames[i].numFaces; t++ )
		{
			int k;

			for ( k = 0; k < 3; k++ )
			{
				psets[f].triangles[t].verts[k][0] = pMesh->vertexes[pMesh->faces[t][k]].x;
				psets[f].triangles[t].verts[k][1] = pMesh->vertexes[pMesh->faces[t][k]].y;
				psets[f].triangles[t].verts[k][2] = pMesh->vertexes[pMesh->faces[t][k]].z;

				if ( pMesh->tvertexes && pMesh->tfaces )
				{
					psets[f].triangles[t].texcoords[k][0] = pMesh->tvertexes[pMesh->tfaces[t][k]].s;
					psets[f].triangles[t].texcoords[k][1] = pMesh->tvertexes[pMesh->tfaces[t][k]].t;
				}

			}
		}

		f++;
	}

	return psets;
}

static void ASE_FreeGeomObject( int ndx )
{
	aseGeomObject_t *pObject;
	int i;

	pObject = &ase.objects[ndx];

	for ( i = 0; i < pObject->anim.numFrames; i++ )
	{
		if ( pObject->anim.frames[i].vertexes )
		{
			free( pObject->anim.frames[i].vertexes );
		}
		if ( pObject->anim.frames[i].tvertexes )
		{
			free( pObject->anim.frames[i].tvertexes );
		}
		if ( pObject->anim.frames[i].faces )
		{
			free( pObject->anim.frames[i].faces );
		}
		if ( pObject->anim.frames[i].tfaces )
		{
			free( pObject->anim.frames[i].tfaces );
		}
	}

	memset( pObject, 0, sizeof( *pObject ) );
}

static aseMesh_t *ASE_GetCurrentMesh( void )
{
	aseGeomObject_t *pObject;

	if ( ase.currentObject >= MAX_ASE_OBJECTS )
	{
		Error( "Too many GEOMOBJECTs" );
		return 0; // never called
	}

	pObject = &ase.objects[ase.currentObject];

	if ( pObject->anim.currentFrame >= MAX_ASE_ANIMATION_FRAMES )
	{
		Error( "Too many MESHes" );
		return 0;
	}

	return &pObject->anim.frames[pObject->anim.currentFrame];
}

static int CharIsTokenDelimiter( int ch )
{
	if ( ch <= 32 )
		return 1;
	return 0;
}

static int ASE_GetToken( qboolean restOfLine )
{
	int i = 0;

	if ( ase.buffer == 0 )
		return 0;

	if ( ( ase.curpos - ase.buffer ) == ase.len )
		return 0;

	// skip over crap
	while ( ( ( ase.curpos - ase.buffer ) < ase.len ) &&
		    ( *ase.curpos <= 32 ) )
	{
		ase.curpos++;
	}

	while ( ( ase.curpos - ase.buffer ) < ase.len )
	{
		s_token[i] = *ase.curpos;

		ase.curpos++;
		i++;

		if ( ( CharIsTokenDelimiter( s_token[i-1] ) && !restOfLine ) ||
			 ( ( s_token[i-1] == '\n' ) || ( s_token[i-1] == '\r' ) ) )
		{
			s_token[i-1] = 0;
			break;
		}
	}

	s_token[i] = 0;

	return 1;
}

static void ASE_ParseBracedBlock( void (*parser)( const char *token ) )
{
	int indent = 0;

	while ( ASE_GetToken( qfalse ) )
	{
		if ( !strcmp( s_token, "{" ) )
		{
			indent++;
		}
		else if ( !strcmp( s_token, "}" ) )
		{
			--indent;
			if ( indent == 0 )
				break;
			else if ( indent < 0 )
				Error( "Unexpected '}'" );
		}
		else
		{
			if ( parser )
				parser( s_token );
		}
	}
}

static void ASE_SkipEnclosingBraces( void )
{
	int indent = 0;

	while ( ASE_GetToken( qfalse ) )
	{
		if ( !strcmp( s_token, "{" ) )
		{
			indent++;
		}
		else if ( !strcmp( s_token, "}" ) )
		{
			indent--;
			if ( indent == 0 )
				break;
			else if ( indent < 0 )
				Error( "Unexpected '}'" );
		}
	}
}

static void ASE_SkipRestOfLine( void )
{
	ASE_GetToken( qtrue );
}

static void ASE_KeyMAP_DIFFUSE( const char *token )
{
	char buffer[1024], buff1[1024], buff2[1024];
  char *buf1, *buf2;
	int i = 0, count;

	if ( !strcmp( token, "*BITMAP" ) )
	{
		ASE_GetToken( qfalse );

		strcpy( buffer, s_token + 1 );
		if ( strchr( buffer, '"' ) )
			*strchr( buffer, '"' ) = 0;

		while ( buffer[i] )
		{
			if ( buffer[i] == '\\' )
				buffer[i] = '/';
			i++;
		}

    buf1 = gamedir;
    buf2 = buffer;
    // need to compare win32 volumes to potential unix junk
    // 
    if ( (gamedir[1] == ':' && (buffer[0] == '/' && buffer[1] == '/')) ||
      (buffer[1] == ':' && (gamedir[0] == '/' && gamedir[1] == '/')) ) {
      if (buffer[1] == ':') {
        buf1 = gamedir + 2;
        buf2 = buffer + 2;
      } else {
        buf1 = gamedir + 2;
        buf2 = buffer +2;
      }
      count = 0;
      while (*buf2 && count < 2) {
        if (*buf2 == '/') {
          count++;
        }
        buf2++;
      }
    } 
    strcpy(buff1, buf1);
    strlwr(buff1);
    strcpy(buff2, buf2);
    strlwr(buff2);
    if ( strstr( buff2, buff1 + 2 ) )
		{
			strcpy( ase.materials[ase.numMaterials].name, strstr( buff2, buff1 + 2 ) + strlen( buff1 ) - 2 );
		}
		else
		{
			sprintf( ase.materials[ase.numMaterials].name, "(not converted: '%s')", buffer );
			printf( "WARNING: illegal material name '%s'\n", buffer );
		}
	}
}

static void ASE_KeyMATERIAL( const char *token )
{
	if ( !strcmp( token, "*MAP_DIFFUSE" ) )
	{
		ASE_ParseBracedBlock( ASE_KeyMAP_DIFFUSE );
	}
	else
	{
	}
}

static void ASE_KeyMATERIAL_LIST( const char *token )
{
	if ( !strcmp( token, "*MATERIAL_COUNT" ) )
	{
		ASE_GetToken( qfalse );
		VERBOSE( ( "..num materials: %s\n", s_token ) );
		if ( atoi( s_token ) > MAX_ASE_MATERIALS )
		{
			Error( "Too many materials!" );
		}
		ase.numMaterials = 0;
	}
	else if ( !strcmp( token, "*MATERIAL" ) )
	{
		VERBOSE( ( "..material %d ", ase.numMaterials ) );
		ASE_ParseBracedBlock( ASE_KeyMATERIAL );
		ase.numMaterials++;
	}
}

static void ASE_KeyMESH_VERTEX_LIST( const char *token )
{
	aseMesh_t *pMesh = ASE_GetCurrentMesh();

	if ( !strcmp( token, "*MESH_VERTEX" ) )
	{
		ASE_GetToken( qfalse );		// skip number

		ASE_GetToken( qfalse );
		pMesh->vertexes[pMesh->currentVertex].y = atof( s_token );

		ASE_GetToken( qfalse );
		pMesh->vertexes[pMesh->currentVertex].x = -atof( s_token );

		ASE_GetToken( qfalse );
		pMesh->vertexes[pMesh->currentVertex].z = atof( s_token );

		pMesh->currentVertex++;

		if ( pMesh->currentVertex > pMesh->numVertexes )
		{
			Error( "pMesh->currentVertex >= pMesh->numVertexes" );
		}
	}
	else
	{
		Error( "Unknown token '%s' while parsing MESH_VERTEX_LIST", token );
	}
}

static void ASE_KeyMESH_FACE_LIST( const char *token )
{
	aseMesh_t *pMesh = ASE_GetCurrentMesh();

	if ( !strcmp( token, "*MESH_FACE" ) )
	{
		ASE_GetToken( qfalse );	// skip face number

		ASE_GetToken( qfalse );	// skip label
		ASE_GetToken( qfalse );	// first vertex
		pMesh->faces[pMesh->currentFace][0] = atoi( s_token );

		ASE_GetToken( qfalse );	// skip label
		ASE_GetToken( qfalse );	// second vertex
		pMesh->faces[pMesh->currentFace][2] = atoi( s_token );

		ASE_GetToken( qfalse );	// skip label
		ASE_GetToken( qfalse );	// third vertex
		pMesh->faces[pMesh->currentFace][1] = atoi( s_token );

		ASE_GetToken( qtrue );

/*
		if ( ( p = strstr( s_token, "*MESH_MTLID" ) ) != 0 )
		{
			p += strlen( "*MESH_MTLID" ) + 1;
			mtlID = atoi( p );
		}
		else
		{
			Error( "No *MESH_MTLID found for face!" );
		}
*/

		pMesh->currentFace++;
	}
	else
	{
		Error( "Unknown token '%s' while parsing MESH_FACE_LIST", token );
	}
}

static void ASE_KeyTFACE_LIST( const char *token )
{
	aseMesh_t *pMesh = ASE_GetCurrentMesh();

	if ( !strcmp( token, "*MESH_TFACE" ) )
	{
		int a, b, c;

		ASE_GetToken( qfalse );

		ASE_GetToken( qfalse );
		a = atoi( s_token );
		ASE_GetToken( qfalse );
		c = atoi( s_token );
		ASE_GetToken( qfalse );
		b = atoi( s_token );

		pMesh->tfaces[pMesh->currentFace][0] = a;
		pMesh->tfaces[pMesh->currentFace][1] = b;
		pMesh->tfaces[pMesh->currentFace][2] = c;

		pMesh->currentFace++;
	}
	else
	{
		Error( "Unknown token '%s' in MESH_TFACE", token );
	}
}

static void ASE_KeyMESH_TVERTLIST( const char *token )
{
	aseMesh_t *pMesh = ASE_GetCurrentMesh();

	if ( !strcmp( token, "*MESH_TVERT" ) )
	{
		char u[80], v[80], w[80];

		ASE_GetToken( qfalse );

		ASE_GetToken( qfalse );
		strcpy( u, s_token );

		ASE_GetToken( qfalse );
		strcpy( v, s_token );

		ASE_GetToken( qfalse );
		strcpy( w, s_token );

		pMesh->tvertexes[pMesh->currentVertex].s = atof( u );
		pMesh->tvertexes[pMesh->currentVertex].t = 1.0f - atof( v );

		pMesh->currentVertex++;

		if ( pMesh->currentVertex > pMesh->numTVertexes )
		{
			Error( "pMesh->currentVertex > pMesh->numTVertexes" );
		}
	}
	else
	{
		Error( "Unknown token '%s' while parsing MESH_TVERTLIST" );
	}
}

static void ASE_KeyMESH( const char *token )
{
	aseMesh_t *pMesh = ASE_GetCurrentMesh();

	if ( !strcmp( token, "*TIMEVALUE" ) )
	{
		ASE_GetToken( qfalse );

		pMesh->timeValue = atoi( s_token );
		VERBOSE( ( ".....timevalue: %d\n", pMesh->timeValue ) );
	}
	else if ( !strcmp( token, "*MESH_NUMVERTEX" ) )
	{
		ASE_GetToken( qfalse );

		pMesh->numVertexes = atoi( s_token );
		VERBOSE( ( ".....TIMEVALUE: %d\n", pMesh->timeValue ) );
		VERBOSE( ( ".....num vertexes: %d\n", pMesh->numVertexes ) );
	}
	else if ( !strcmp( token, "*MESH_NUMFACES" ) )
	{
		ASE_GetToken( qfalse );

		pMesh->numFaces = atoi( s_token );
		VERBOSE( ( ".....num faces: %d\n", pMesh->numFaces ) );
	}
	else if ( !strcmp( token, "*MESH_NUMTVFACES" ) )
	{
		ASE_GetToken( qfalse );

		if ( atoi( s_token ) != pMesh->numFaces )
		{
			Error( "MESH_NUMTVFACES != MESH_NUMFACES" );
		}
	}
	else if ( !strcmp( token, "*MESH_NUMTVERTEX" ) )
	{
		ASE_GetToken( qfalse );

		pMesh->numTVertexes = atoi( s_token );
		VERBOSE( ( ".....num tvertexes: %d\n", pMesh->numTVertexes ) );
	}
	else if ( !strcmp( token, "*MESH_VERTEX_LIST" ) )
	{
		pMesh->vertexes = calloc( sizeof( aseVertex_t ) * pMesh->numVertexes, 1 );
		pMesh->currentVertex = 0;
		VERBOSE( ( ".....parsing MESH_VERTEX_LIST\n" ) );
		ASE_ParseBracedBlock( ASE_KeyMESH_VERTEX_LIST );
	}
	else if ( !strcmp( token, "*MESH_TVERTLIST" ) )
	{
		pMesh->currentVertex = 0;
		pMesh->tvertexes = calloc( sizeof( aseTVertex_t ) * pMesh->numTVertexes, 1 );
		VERBOSE( ( ".....parsing MESH_TVERTLIST\n" ) );
		ASE_ParseBracedBlock( ASE_KeyMESH_TVERTLIST );
	}
	else if ( !strcmp( token, "*MESH_FACE_LIST" ) )
	{
		pMesh->faces = calloc( sizeof( aseFace_t ) * pMesh->numFaces, 1 );
		pMesh->currentFace = 0;
		VERBOSE( ( ".....parsing MESH_FACE_LIST\n" ) );
		ASE_ParseBracedBlock( ASE_KeyMESH_FACE_LIST );
	}
	else if ( !strcmp( token, "*MESH_TFACELIST" ) )
	{
		pMesh->tfaces = calloc( sizeof( aseFace_t ) * pMesh->numFaces, 1 );
		pMesh->currentFace = 0;
		VERBOSE( ( ".....parsing MESH_TFACE_LIST\n" ) );
		ASE_ParseBracedBlock( ASE_KeyTFACE_LIST );
	}
	else if ( !strcmp( token, "*MESH_NORMALS" ) )
	{
		ASE_ParseBracedBlock( 0 );
	}
}

static void ASE_KeyMESH_ANIMATION( const char *token )
{
	aseMesh_t *pMesh = ASE_GetCurrentMesh();

	// loads a single animation frame
	if ( !strcmp( token, "*MESH" ) )
	{
		VERBOSE( ( "...found MESH\n" ) );
		assert( pMesh->faces == 0 );
		assert( pMesh->vertexes == 0 );
		assert( pMesh->tvertexes == 0 );
		memset( pMesh, 0, sizeof( *pMesh ) );

		ASE_ParseBracedBlock( ASE_KeyMESH );

		if ( ++ase.objects[ase.currentObject].anim.currentFrame == MAX_ASE_ANIMATION_FRAMES )
		{
			Error( "Too many animation frames" );
		}
	}
	else
	{
		Error( "Unknown token '%s' while parsing MESH_ANIMATION", token );
	}
}

static void ASE_KeyGEOMOBJECT( const char *token )
{
	if ( !strcmp( token, "*NODE_NAME" ) )
	{
		char *name = ase.objects[ase.currentObject].name;

		ASE_GetToken( qtrue );
		VERBOSE( ( " %s\n", s_token ) );
		strcpy( ase.objects[ase.currentObject].name, s_token + 1 );
		if ( strchr( ase.objects[ase.currentObject].name, '"' ) )
			*strchr( ase.objects[ase.currentObject].name, '"' ) = 0;

		if ( strstr( name, "tag" ) == name )
		{
			while ( strchr( name, '_' ) != strrchr( name, '_' ) )
			{
				*strrchr( name, '_' ) = 0;
			}
			while ( strrchr( name, ' ' ) )
			{
				*strrchr( name, ' ' ) = 0;
			}
		}
	}
	else if ( !strcmp( token, "*NODE_PARENT" ) )
	{
		ASE_SkipRestOfLine();
	}
	// ignore unused data blocks
	else if ( !strcmp( token, "*NODE_TM" ) ||
		      !strcmp( token, "*TM_ANIMATION" ) )
	{
		ASE_ParseBracedBlock( 0 );
	}
	// ignore regular meshes that aren't part of animation
	else if ( !strcmp( token, "*MESH" ) && !ase.grabAnims )
	{
/*
		if ( strstr( ase.objects[ase.currentObject].name, "tag_" ) == ase.objects[ase.currentObject].name ) 
		{
			s_forceStaticMesh = qtrue;
			ASE_ParseBracedBlock( ASE_KeyMESH );
			s_forceStaticMesh = qfalse;
		}
*/
		ASE_ParseBracedBlock( ASE_KeyMESH );
		if ( ++ase.objects[ase.currentObject].anim.currentFrame == MAX_ASE_ANIMATION_FRAMES )
		{
			Error( "Too many animation frames" );
		}
		ase.objects[ase.currentObject].anim.numFrames = ase.objects[ase.currentObject].anim.currentFrame;
		ase.objects[ase.currentObject].numAnimations++;
/*
		// ignore meshes that aren't part of animations if this object isn't a 
		// a tag
		else
		{
			ASE_ParseBracedBlock( 0 );
		}
*/
	}
	// according to spec these are obsolete
	else if ( !strcmp( token, "*MATERIAL_REF" ) )
	{
		ASE_GetToken( qfalse );

		ase.objects[ase.currentObject].materialRef = atoi( s_token );
	}
	// loads a sequence of animation frames
	else if ( !strcmp( token, "*MESH_ANIMATION" ) )
	{
		if ( ase.grabAnims )
		{
			VERBOSE( ( "..found MESH_ANIMATION\n" ) );

			if ( ase.objects[ase.currentObject].numAnimations )
			{
				Error( "Multiple MESH_ANIMATIONS within a single GEOM_OBJECT" );
			}
			ASE_ParseBracedBlock( ASE_KeyMESH_ANIMATION );
			ase.objects[ase.currentObject].anim.numFrames = ase.objects[ase.currentObject].anim.currentFrame;
			ase.objects[ase.currentObject].numAnimations++;
		}
		else
		{
			ASE_SkipEnclosingBraces();
		}
	}
	// skip unused info
	else if ( !strcmp( token, "*PROP_MOTIONBLUR" ) ||
		      !strcmp( token, "*PROP_CASTSHADOW" ) ||
			  !strcmp( token, "*PROP_RECVSHADOW" ) )
	{
		ASE_SkipRestOfLine();
	}
}

#if 0
static void ConcatenateObjects( aseGeomObject_t *pObjA, aseGeomObject_t *pObjB )
{
//	int numVertexes;

}

static void CollapseObjects( void )
{
	int i;
	int numObjects = ase.currentObject;

	for ( i = 0; i < numObjects; i++ )
	{
		int j;

		// skip tags
		if ( strstr( ase.objects[i].name, "tag" ) == ase.objects[i].name )
		{
			continue;
		}

		if ( !ase.objects[i].numAnimations )
		{
			continue;
		}

		for ( j = i + 1; j < numObjects; j++ )
		{
			if ( strstr( ase.objects[j].name, "tag" ) == ase.objects[j].name )
			{
				continue;
			}
			if ( ase.objects[i].materialRef == ase.objects[j].materialRef )
			{
				if ( ase.objects[j].numAnimations )
				{
					ConcatenateObjects( &ase.objects[i], &ase.objects[j] );
				}
			}
		}
	}
}
#endif
/*
** ASE_Process
*/
static void ASE_Process( int type )
{
	while ( ASE_GetToken( qfalse ) )
	{
		if ( !strcmp( s_token, "*3DSMAX_ASCIIEXPORT" ) ||
			 !strcmp( s_token, "*COMMENT" ) )
		{
			ASE_SkipRestOfLine();
		}
		else if ( !strcmp( s_token, "*SCENE" ) )
		{
			ASE_SkipEnclosingBraces();
		}
		else if ( !strcmp( s_token, "*HELPEROBJECT" ) )
		{
			ASE_SkipEnclosingBraces();
		}
		else if ( !strcmp( s_token, "*MATERIAL_LIST" ) )
		{
			VERBOSE( ("MATERIAL_LIST\n") );

			ASE_ParseBracedBlock( ASE_KeyMATERIAL_LIST );
		}
		else if ( !strcmp( s_token, "*GEOMOBJECT" ) )
		{
			VERBOSE( ("GEOMOBJECT" ) );

			ASE_ParseBracedBlock( ASE_KeyGEOMOBJECT );

			if (!ase.objects[ase.currentObject].anim.frames[0].numFaces)	//we didn't get any faces of animation!
			{
				Error( "WARNING: ASE_Process no triangles grabbed for GEOMOBJECT \"%s\"!\n", ase.objects[ase.currentObject].name);
			}
			_strlwr(ase.objects[ase.currentObject].name);
			if ( strstr( ase.objects[ase.currentObject].name, "Bip" ) ||
				 strstr( ase.objects[ase.currentObject].name, "ignore_" ) )
			{
				VERBOSE( ( "(discarding BIP/ignore object)\n" ) );
				ASE_FreeGeomObject( ase.currentObject );
			}
			else if ( (type /*== TYPE_PLAYER*/) && ( strstr( ase.objects[ase.currentObject].name, "h_" ) != ase.objects[ase.currentObject].name ) &&
				      ( strstr( ase.objects[ase.currentObject].name, "l_" ) != ase.objects[ase.currentObject].name ) &&
					  ( strstr( ase.objects[ase.currentObject].name, "u_" ) != ase.objects[ase.currentObject].name ) &&
					  ( strstr( ase.objects[ase.currentObject].name, "tag" ) != ase.objects[ase.currentObject].name ) &&
					  ase.grabAnims )
			{
				VERBOSE( ( "(ignoring improperly labeled object '%s')\n", ase.objects[ase.currentObject].name ) );
				ASE_FreeGeomObject( ase.currentObject );
			}
			else
			{
				if ( ++ase.currentObject == MAX_ASE_OBJECTS )
				{
					Error( "Too many GEOMOBJECTs" );
				}
			}
		}
		else if ( s_token[0] )
		{
			printf( "Unknown token '%s'\n", s_token );
		}
	}

	if ( !ase.currentObject )
		Error( "No animation data!" );

//	CollapseObjects();
}

/*
** ASE_InitForGrabbing
**
** any sort of initialization required to use aseGrab
*/
void ASE_InitForGrabbing(void)
{
	memset(&aseGrab, sizeof(ase_t), 0);
}

/*
** ASE_CopyFrames
**
** helper for copying frames from one animation to another. 
** do your bounds checking
*/

void ASE_CopyFrames(aseMeshAnimation_t *destAnim , 
					aseMeshAnimation_t *sourceAnim, int nDest,
					int nSource, int nNum, framecopy_type fillType)
{
	int i = 0, s = nSource, d = 0;

	if (FRAMECOPY_NONE == fillType)
	{
		// what the heck are you tryin' to do?
		return;
	}

	for (i = 0; i < nNum; i++)
	{
		d = nDest + i;
		if (FRAMECOPY_1TO1 == fillType)
		{
			s = nSource + i;
		}
		if (destAnim->frames[d].numVertexes)
		{
			free(destAnim->frames[d].vertexes);
		}
		if (destAnim->frames[d].numTVertexes)
		{
			free(destAnim->frames[d].tvertexes);
		}
		if (destAnim->frames[d].numFaces)
		{
			free(destAnim->frames[d].faces);
			free(destAnim->frames[d].tfaces);
		}
		memcpy(	&destAnim->frames[d],
				&sourceAnim->frames[s],
				sizeof(aseMesh_t));
		destAnim->frames[d].vertexes = calloc( sizeof( aseVertex_t ) *
			sourceAnim->frames[s].numVertexes, 1 );
		memcpy(destAnim->frames[d].vertexes, sourceAnim->frames[s].vertexes, 
			sourceAnim->frames[s].numVertexes * sizeof(aseVertex_t));
		destAnim->frames[d].tvertexes = calloc( sizeof( aseTVertex_t ) *
			sourceAnim->frames[s].numTVertexes, 1 );
		memcpy(destAnim->frames[d].tvertexes, sourceAnim->frames[s].tvertexes, 
			sourceAnim->frames[s].numTVertexes * sizeof(aseTVertex_t));
		destAnim->frames[d].faces = calloc( sizeof( aseFace_t ) *
			sourceAnim->frames[s].numFaces, 1 );
		memcpy(destAnim->frames[d].faces, sourceAnim->frames[s].faces, 
			sourceAnim->frames[s].numFaces * sizeof(aseFace_t));
		destAnim->frames[d].tfaces = calloc( sizeof( aseFace_t ) *
			sourceAnim->frames[s].numFaces, 1 );
		memcpy(destAnim->frames[d].tfaces, sourceAnim->frames[s].tfaces,
			sourceAnim->frames[s].numFaces * sizeof(aseFace_t));
	}
}

/*
** ASE_CopyGrabToConcatBuffer
**
** just copying information from ase into aseGrab...have to do some allocation 'n' stuff,
** so we're assuming aseGrab is zeroed out
*/
void ASE_CopyGrabToConcatBuffer(const int nFillFrame)
{
	int i = 0;
	aseGeomObject_t* sourceObj = NULL, *destObj = NULL;
	aseMeshAnimation_t* sourceAnim = NULL, *destAnim = NULL;

	// don't care about copying ase's buffer or curpos information cuz we're
	//never going to do any reading with aseGrab
	aseGrab.currentObject = ase.currentObject;
	aseGrab.grabAnims = ase.grabAnims;
	aseGrab.len = ase.len;
	aseGrab.numMaterials = ase.numMaterials;
	aseGrab.verbose = ase.verbose;

	for (i = 0; i < ase.numMaterials; i++)
	{
		strcpy(aseGrab.materials[i].name, ase.materials[i].name);
	}

	for (i = 0; i < ase.currentObject; i++)
	{
		sourceObj = &ase.objects[i];
		destObj = &aseGrab.objects[i];

		destObj->materialRef = sourceObj->materialRef;
		strcpy(destObj->name, sourceObj->name);
		destObj->numAnimations = sourceObj->numAnimations;
		destObj->anim.numFrames = sourceObj->anim.numFrames;
		destObj->anim.currentFrame = sourceObj->anim.currentFrame;
		sourceAnim = &sourceObj->anim;
		destAnim = &destObj->anim;

		ASE_CopyFrames(destAnim, sourceAnim, 0,	0, sourceAnim->numFrames, FRAMECOPY_1TO1);
		if (nFillFrame > destObj->anim.numFrames)
		{
			// fill the rest of the dest frames with the last source frame
			ASE_CopyFrames(destAnim, sourceAnim, destObj->anim.numFrames,
				sourceObj->anim.numFrames-1, nFillFrame - destAnim->numFrames,
				FRAMECOPY_FILL);
			destAnim->numFrames = nFillFrame;
			destAnim->currentFrame = nFillFrame;
		}
	}
}

/*
** ASE_CopyFromConcatBuffer
**
** just copying information from aseGrab into ase
*/
void ASE_CopyFromConcatBuffer(void)
{
	int i = 0;
	aseGeomObject_t* sourceObj = NULL, *destObj = NULL;
	aseMeshAnimation_t* sourceAnim = NULL, *destAnim = NULL;

	// don't care about copying aseGrab's buffer or curpos information cuz this
	//function _should_ only get called as part of the output sequence for a file
	ase.currentObject = aseGrab.currentObject;
	ase.grabAnims = aseGrab.grabAnims;
	ase.len = aseGrab.len;
	ase.numMaterials = aseGrab.numMaterials;
	ase.verbose = aseGrab.verbose;

	for (i = 0; i < aseGrab.numMaterials; i++)
	{
		strcpy(ase.materials[i].name, aseGrab.materials[i].name);
	}

	for (i = 0; i < aseGrab.currentObject; i++)
	{
		sourceObj = &aseGrab.objects[i];
		destObj = &ase.objects[i];

		destObj->materialRef = sourceObj->materialRef;
		strcpy(destObj->name, sourceObj->name);
		destObj->numAnimations = sourceObj->numAnimations;
		destObj->anim.numFrames = sourceObj->anim.numFrames;
		destObj->anim.currentFrame = sourceObj->anim.currentFrame;
		sourceAnim = &sourceObj->anim;
		destAnim = &destObj->anim;

		ASE_CopyFrames(destAnim, sourceAnim, 0, 0,
			sourceObj->anim.numFrames, FRAMECOPY_1TO1);
	}
}

/*
** ASE_CatGrabbedFrames
**
** precondition: ase has been loaded with a file
**
** concatenate the animation frames from the recently loaded ase struct with
**the existing model and frames already in aseGrab struct
*/
qboolean ASE_CatGrabbedFrames(const int nSourceFrame, int nDestFrame, int nNumFrames, const int nFillFrame)
{
	qboolean bFailed = qfalse, bFirstGrab = (0 == aseGrab.currentObject);
	int i, j, lastValidFrame, nFillPos, nMidFillPos,framesAvail;
	aseMeshAnimation_t* sourceAnim = NULL, *destAnim = NULL;

	// gotta do a little special setup stuff if this is the first file we've grabbed
	if (bFirstGrab)
	{
		//filling here means out to the last frame
		ASE_CopyGrabToConcatBuffer(nFillFrame);
		printf("\tFirst anim starts on 0, ends on %d (%d frames)\n", ase.objects[0].anim.numFrames-1, ase.objects[0].anim.numFrames);
		return qfalse;
	}

	// if no value was given for number of frames to grab, get 'em all
	if (nNumFrames == -1)
	{
		nNumFrames = ase.objects[0].anim.numFrames;
	}

	// if no value was given for dest frame, assume last one grabbed
	if (nDestFrame== -1)
	{
		nDestFrame = aseGrab.objects[0].anim.numFrames;
	}
	
	nFillPos = nDestFrame + nNumFrames;
	lastValidFrame = ase.objects[0].anim.numFrames - 1;
	framesAvail = ase.objects[0].anim.numFrames - nSourceFrame;
	nMidFillPos = nDestFrame + framesAvail;

	// filling after the first grab, means only fill to the number of frames specified if the source doesn't have enough

	// nMidFillPos	position in dest animation to start filling in using the last frame of our source animation
	// nFillPos		position after dest animation to start filling out to end
	//
	// lastValidFrame	frame of the source animation to use as a fill frame in the dest animation
	//

	// since we've done at least one prior file grab, now all we want from ase is 
	//frame info. go through every object in the new file (ase) and add its frames to
	//the corresponding object in aseGrab.
	printf("\t\t anim starts on %d, ends on %d (%d frames)\n", nDestFrame, nDestFrame+nNumFrames-1, nNumFrames);
	for (i = 0; i < aseGrab.currentObject; i++)
	{
		destAnim = &aseGrab.objects[i].anim;
		for (j = 0; j < ase.currentObject; j++)
		{
			if (strcmp(aseGrab.objects[i].name, ase.objects[j].name) == 0)
			{
				sourceAnim = &ase.objects[j].anim;
				if (nDestFrame + nNumFrames > MAX_ASE_ANIMATION_FRAMES)
				{
					Error("can't put that many frames where you want 'em!");
					bFailed = qtrue;
				}
				else
				{
					// can't just copy the desired frames cuz they involve
					//frame-specific allocations. first, free the original (dest) frame, 
					//then allocate new space for the incoming (source) frame. then copy.
					if (framesAvail >= nNumFrames) {
						ASE_CopyFrames(destAnim, sourceAnim, nDestFrame, nSourceFrame,
							nNumFrames, FRAMECOPY_1TO1);
					}
					else {	//there is a shortage
						ASE_CopyFrames(destAnim, sourceAnim, nDestFrame, nSourceFrame,
						framesAvail, FRAMECOPY_1TO1);
						// fill the rest of the dest frames with the last source frame
						ASE_CopyFrames(destAnim, sourceAnim, nMidFillPos, lastValidFrame,
							nNumFrames - framesAvail, FRAMECOPY_FILL);
					}
					//mark the highest frame filled - better hope there aren't any holes in the seq
					if (destAnim->numFrames < nFillPos)
					{
						destAnim->numFrames = nFillPos;
					}

					if (nFillFrame)	//now this is to fill out to maxFrame
					{
						// fill the rest of the dest frames with the last source frame
						ASE_CopyFrames(destAnim, sourceAnim, nFillPos, lastValidFrame,
							nFillFrame - nFillPos, FRAMECOPY_FILL);
						if (destAnim->numFrames < nFillFrame)
							destAnim->numFrames = nFillFrame;
					}
					destAnim->currentFrame = destAnim->numFrames;
				}
				break;
			}
		}
	}
	return bFailed;
}

void ASE_FreeGrab(void)
{
	int i = 0, j = 0;
	aseMesh_t* mesh = NULL;

	for (i = 0; i < aseGrab.currentObject; i++)
	{
		for (j = 0; j < aseGrab.objects[i].anim.numFrames; j++)
		{
			mesh = &aseGrab.objects[i].anim.frames[j];
			if (mesh->numVertexes)
			{
				free(mesh->vertexes);
			}
			if (mesh->numTVertexes)
			{
				free(mesh->tvertexes);
			}
			if (mesh->numFaces)
			{
				free(mesh->faces);
				free(mesh->tfaces);
			}
		}
	}
}
