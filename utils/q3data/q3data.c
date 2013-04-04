#include <io.h>
#include "q3data.h"
#include "md3lib.h"

qboolean	g_verbose;
qboolean	g_stripify = qtrue;
qboolean	g_release;			// don't grab, copy output data to new tree
char		g_releasedir[1024];	// c:\quake2\baseq2, etc
qboolean	g_archive;			// don't grab, copy source data to new tree
char		g_only[256];		// if set, only grab this cd
qboolean	g_skipmodel;		// set true when a cd is not g_only
qboolean	gbKeyPress = qfalse;
/*
=======================================================

  PAK FILES

=======================================================
*/

unsigned Com_BlockChecksum (void *buffer, int length);

typedef struct
{
	char	name[56];
	int		filepos, filelen;
} packfile_t;

typedef struct
{
	char	id[4];
	int		dirofs;
	int		dirlen;
} packheader_t;

packfile_t		pfiles[16384];
FILE			*pakfile;
packfile_t		*pf;
packheader_t	pakheader;

/*
==============
BeginPak
==============
*/
#if 0
void BeginPak (char *outname)
{
	if (!g_pak)
		return;

	pakfile = SafeOpenWrite (outname);

	// leave space for header
	SafeWrite (pakfile, &pakheader, sizeof(pakheader));
	
	pf = pfiles;
}
#endif


/*
==============
ReleaseFile

Filename should be gamedir reletive.
Either copies the file to the release dir, or adds it to
the pak file.
==============
*/
void ReleaseFile (char *filename)
{
	char	source[1024];
	char	dest[1024];

	if (!g_release)
		return;

	sprintf (source, "%s%s", gamedir, filename);

//	if (!g_pak)
	{	// copy it
		sprintf (dest, "%s/%s", g_releasedir, filename);
		printf ("copying to %s\n", dest);
		QCopyFile (source, dest);
		return;
	}
#if 0
	else
	{
		int		len;
		byte	*buf;

		// pak it
		printf ("paking %s\n", filename);
		if (strlen(filename) >= sizeof(pf->name))
			Error ("Filename too long for pak: %s", filename);

		len = LoadFile (source, (void **)&buf);

	/*
		if (g_compress_pak && len < 4096*1024 )
		{
			cblock_t	in, out;
			cblock_t Huffman (cblock_t in);

			in.count = len;
			in.data = buf;

			out = Huffman (in);

			if (out.count < in.count)
			{
				printf ("   compressed from %i to %i\n", in.count, out.count);
				free (in.data);
				buf = out.data;
				len = out.count;
			}
			else
				free (out.data);
		}
	*/

		strcpy (pf->name, filename);
		pf->filepos = LittleLong(ftell(pakfile));
		pf->filelen = LittleLong(len);
		pf++;

		SafeWrite (pakfile, buf, len);

		free (buf);
	}
#endif
}

typedef struct
{
	// shader
	// opaque
	// opaque 2
	// blend 
	// blend 2
	char names[5][1024];
	int	 num;
} ShaderFiles_t;

ShaderFiles_t s_shaderFiles;

void FindShaderFiles( char *filename )
{
	char buffer[1024];
	char stripped[1024];
	char linebuffer[1024];
	int len, i;
	char *buf;
	char *diffuseExtensions[] =
	{
		".TGA",
		".WAL",
		".PCX",
		0
	};
	char *otherExtensions[] =
	{
		".specular.TGA",
		".blend.TGA",
		".alpha.TGA",
		0
	};

	s_shaderFiles.num = 0;

	strcpy( stripped, filename );
	if ( strrchr( stripped, '.' ) )
		*strrchr( stripped, '.' ) = 0;
	strcat( stripped, ".shader" );

	if ( FileExists( stripped ) )
	{
		char *p;
		char mapa[512], mapb[512];

		strcpy( s_shaderFiles.names[s_shaderFiles.num], stripped );
		s_shaderFiles.num++;

		// load and parse
		len = LoadFile( stripped, (void **)&buf);

		p = buf;

		while ( p - buf < len )
		{
			i = 0;

			// skip spaces
			while ( *p == ' ' || *p == '\n' || *p == '\t' )
				p++;

			// grab rest of the line
			while ( *p != 0 && *p != '\n' )
			{
				linebuffer[i] = *p;
				i++;
				p++;
			}
			if ( *p == '\n' )
				p++;
			linebuffer[i] = 0;

			strlwr( linebuffer );

			// see if the line specifies an opaque map or blendmap
			if ( strstr( linebuffer, "opaquemap" ) == linebuffer ||
				 strstr( linebuffer, "blendmap" ) == linebuffer )
			{
				int j;

				i = 0;

				mapa[0] = mapb[0] = 0;

				// skip past the keyword
				while ( linebuffer[i] != ' ' && linebuffer[i] != '\t' && linebuffer[i] )
					i++;
				// skip past spaces
				while ( ( linebuffer[i] == ' ' || linebuffer[i] == '\t' ) && linebuffer[i] )
					i++;

				// grab first map name
				j = 0;
				while ( linebuffer[i] != ' ' && linebuffer[i] != '\t' && linebuffer[i] )
				{
					mapa[j] = linebuffer[i];
					j++;
					i++;
				}
				mapa[j] = 0;

				// skip past spaces
				while ( ( linebuffer[i] == ' ' || linebuffer[i] == '\t' ) && linebuffer[i] )
					i++;

				// grab second map name
				j = 0;
				while ( linebuffer[i] != ' ' && linebuffer[i] != '\t' && linebuffer[i] )
				{
					mapb[j] = linebuffer[i];
					j++;
					i++;
				}
				mapb[j] = 0;

				// store map names
				if ( mapa[0] != 0 && mapa[0] != '-' )
				{
					sprintf( s_shaderFiles.names[s_shaderFiles.num], "%s%s", gamedir, mapa );
					s_shaderFiles.num++;
				}
				if ( mapb[0] != 0 && mapb[0] != '-' && mapb[0] != '^' && mapb[0] != '*' )
				{
					sprintf( s_shaderFiles.names[s_shaderFiles.num], "%s%s", gamedir, mapb );
					s_shaderFiles.num++;
				}
			}
		}
	}
	else
	{
		if ( strrchr( stripped, '.' ) )
			*strrchr( stripped, '.' ) = 0;

		// look for diffuse maps
		for ( i = 0; i < 3; i++ )
		{
			strcpy( buffer, stripped );
			strcat( buffer, diffuseExtensions[i] );
			if ( FileExists( buffer ) )
			{
				strcpy( s_shaderFiles.names[s_shaderFiles.num], buffer );
				s_shaderFiles.num++;
				break;
			}
		}
		for ( i = 0; i < 3; i++ )
		{
			strcpy( buffer, stripped );
			strcat( buffer, otherExtensions[i] );
			if ( FileExists( buffer ) )
			{
				strcpy( s_shaderFiles.names[s_shaderFiles.num], buffer );
				s_shaderFiles.num++;
			}
		}
	}
}

/*
==============
ReleaseShader

Copies all needed files for a shader to the release directory
==============
*/
void ReleaseShader( char *filename ) 
{
	char fullpath[1024];
	char dest[1024];
	char stripped[1024];
	int i;

	sprintf( fullpath, "%s%s", gamedir, filename );

	FindShaderFiles( fullpath );

//	if ( !g_pak )
	{
		for ( i = 0; i < s_shaderFiles.num; i++ )
		{
			strcpy( stripped, s_shaderFiles.names[i] );
			if ( strstr( stripped, gamedir ) )
			{
				memmove( stripped, stripped+ strlen( gamedir ), strlen( stripped ) );
			}
			sprintf( dest, "%s/%s", g_releasedir, stripped );
			printf ("copying to %s\n", dest );
			QCopyFile( s_shaderFiles.names[i], dest );
		}
	}
}

/*
==============
FinishPak
==============
*/
#if 0
void FinishPak (void)
{
	int		dirlen;
	int		d;
	int		i;
	unsigned	checksum;

	if (!g_pak)
		return;

	pakheader.id[0] = 'P';
	pakheader.id[1] = 'A';
	pakheader.id[2] = 'C';
	pakheader.id[3] = 'K';
	dirlen = (byte *)pf - (byte *)pfiles;
	pakheader.dirofs = LittleLong(ftell(pakfile));
	pakheader.dirlen = LittleLong(dirlen);
	
	checksum = Com_BlockChecksum ( (void *)pfiles, dirlen );

	SafeWrite (pakfile, pfiles, dirlen);

	i = ftell (pakfile);
	 
	fseek (pakfile, 0, SEEK_SET);
	SafeWrite (pakfile, &pakheader, sizeof(pakheader));
	fclose (pakfile);	
	
	d = pf - pfiles;
	printf ("%i files packed in %i bytes\n",d, i);
	printf ("checksum: 0x%x\n", checksum);
}
#endif


/*
===============
Cmd_File

This is only used to cause a file to be copied during a release
build (default.cfg, maps, etc)
===============
*/
void Cmd_File (void)
{
	GetToken (qfalse);
	ReleaseFile (token);
}

/*
===============
PackDirectory_r

===============
*/
#ifdef _WIN32
#include "io.h"
void PackDirectory_r (char *dir)
{
	struct _finddata_t fileinfo;
	int		handle;
	char	dirstring[1024];
	char	filename[1024];

	sprintf (dirstring, "%s%s/*.*", gamedir, dir);

	handle = _findfirst (dirstring, &fileinfo);
	if (handle == -1)
		return;

	do
	{
		sprintf (filename, "%s/%s", dir, fileinfo.name);
		if (fileinfo.attrib & _A_SUBDIR)
		{	// directory
			if (fileinfo.name[0] != '.')	// don't pak . and ..
				PackDirectory_r (filename);
			continue;
		}
		// copy or pack the file
		ReleaseFile (filename);		
	} while (_findnext( handle, &fileinfo ) != -1);

	_findclose (handle);
}
#else

#include <sys/types.h>
#ifdef NeXT
#include <sys/dir.h>
#else
#include <sys/dirent.h>
#endif

void PackDirectory_r (char *dir)
{
#ifdef NeXT
	struct direct **namelist, *ent;
#else
	struct dirent **namelist, *ent;
#endif
	int		count;
	struct stat st;
	int			i;
	int			len;
	char		fullname[1024];
	char		dirstring[1024];
	char		*name;
	
	sprintf (dirstring, "%s%s", gamedir, dir);
	count = scandir(dirstring, &namelist, NULL, NULL);
	
	for (i=0 ; i<count ; i++)
	{
		ent = namelist[i];	
		name = ent->d_name;

		if (name[0] == '.')
			continue;
	
		sprintf (fullname, "%s/%s", dir, name);
		sprintf (dirstring, "%s%s/%s", gamedir, dir, name);
		
		if (stat (dirstring, &st) == -1)
			Error ("fstating %s", pf->name);
		if (st.st_mode & S_IFDIR)
		{	// directory
			PackDirectory_r (fullname);
			continue;
		}

		// copy or pack the file
		ReleaseFile (fullname);		
	}
}
#endif


/*
===============
Cmd_Dir

This is only used to cause a directory to be copied during a
release build (sounds, etc)
===============
*/
void Cmd_Dir (void)
{
	GetToken (qfalse);
	PackDirectory_r (token);	
}

//========================================================================

#define	MAX_RTEX	16384
int		numrtex;
char	rtex[MAX_RTEX][64];

void ReleaseTexture (char *name)
{
	int		i;
	char	path[1024];

	for (i=0 ; i<numrtex ; i++)
		if (!Q_stricmp(name, rtex[i]))
			return;

	if (numrtex == MAX_RTEX)
		Error ("numrtex == MAX_RTEX");

	strcpy (rtex[i], name);
	numrtex++;

	sprintf (path, "textures/%s.wal", name);
	ReleaseFile (path);
}

/*
===============
Cmd_Maps

Only relevent for release and pak files.
Releases the .bsp files for the maps, and scans all of the files to
build a list of all textures used, which are then released.
===============
*/
void Cmd_Maps (void)
{
	char	map[1024];
//	int		i;

	while (TokenAvailable ())
	{
		GetToken (qfalse);
		sprintf (map, "maps/%s.bsp", token);
		ReleaseFile (map);

		if (!g_release)
			continue;

		// get all the texture references
		sprintf (map, "%smaps/%s.bsp", gamedir, token);
		LoadBSPFile( map );
#if 0		// FIXME: Q3 surface parsing
		for (i=0 ; i<numtexinfo ; i++)
			ReleaseTexture (texinfo[i].texture);
#endif
	}
}


//==============================================================

/*
===============
ParseScript
===============
*/
void ParseScript (void)
{
	while (1)
	{
		do
		{	// look for a line starting with a $ command
			GetToken (qtrue);
			if (endofscript)
				return;
			if (token[0] == '$')
				break;				
			while (TokenAvailable())
				GetToken (qfalse);
		} while (1);
	
		//
		// model commands
		//
		if (!strcmp (token, "$modelname"))
			Cmd_Modelname ();
		else if (!strcmp (token, "$base"))
			Cmd_Base ();
		else if ( !strcmp( token, "$exit" ) )
			break;
		else if ( !strcmp( token, "$3dsconvert" ) )
			Cmd_3DSConvert();
		else if (!strcmp (token, "$spritebase"))
			Cmd_SpriteBase ();
		else if (!strcmp (token, "$cd"))
			Cmd_Cd ();
		else if (!strcmp (token, "$origin"))
			Cmd_Origin ();
		else if (!strcmp (token, "$scale"))
			Cmd_ScaleUp ();
		else if (!strcmp (token, "$frame"))
			Cmd_Frame ();
		else if (!strcmp (token, "$skin" ))
			Cmd_Skin();
		else if (!strcmp (token, "$spriteshader"))
			Cmd_SpriteShader();
		else if (!strcmp( token, "$aseconvert" ))
			Cmd_ASEConvert( qfalse, qfalse );
		else if (!strcmp( token, "$aseconvertg2" ))
			Cmd_ASEConvert( qfalse, qtrue );
		else if (!strcmp( token, "$aseanimconvert" ) )
			Cmd_ASEConvert( qtrue, qfalse );
		else if (!strcmp( token, "$aseanimgrabinit" ) )
			Cmd_ASEInitAnimGrab();
		else if (!strcmp( token, "$aseanimgrabfinalize" ) )
			Cmd_ASEFinalizeAnimGrab();
		else if (!strcmp( token, "$aseanimgrab" ) )
			Cmd_ASEAnimGrab();

		//
		// image commands
		//
		else if (!strcmp (token, "$grab"))
			Cmd_Grab ();
		else if (!strcmp (token, "$raw"))
			Cmd_Raw ();
		else if (!strcmp (token, "$colormap"))
			Cmd_Colormap ();
		else if (!strcmp (token, "$environment"))
			Cmd_Environment ();

		//
		// video
		//
		else if (!strcmp (token, "$video"))
			Cmd_Video ();
		//
		// misc
		//
		else if (!strcmp (token, "$file"))
			Cmd_File ();
		else if (!strcmp (token, "$dir"))
			Cmd_Dir ();
		else if (!strcmp (token, "$maps"))
			Cmd_Maps ();
		else
			Error ("bad command %s\n", token);
	}
}

//=======================================================


// convenient to breakpoint on to catch exit() calls...
//
void myexit(void)
{
}

int main (int argc, char **argv)
{
	static	int		i;		// VC4.2 compiler bug if auto...
	char	path[1024];

	atexit(myexit);

	printf ("Q3Data v2.1 (c) 1999 Id Software Inc. - Enhanced by Raven Software (c) 2000\n");

	ExpandWildcards (&argc, &argv);

	for (i=1 ; i<argc ; i++)
	{
		if (!strcmp(argv[i], "-archive"))
		{
			// -archive f:/quake2/release/dump_11_30
			archive = qtrue;
			strcpy (archivedir, argv[i+1]);
			printf ("Archiving source to: %s\n", archivedir);
			i++;
		}
		else if (!strcmp(argv[i], "-release"))
		{
			g_release = qtrue;
			strcpy (g_releasedir, argv[i+1]);
			printf ("Copy output to: %s\n", g_releasedir);
			i++;
		}
		else if ( !strcmp( argv[i], "-nostrips" ) )
		{
			g_stripify = qfalse;
			printf( "Not optimizing for strips\n" );
		}
		else if ( !strcmp( argv[i], "-writedir" ) )
		{
			strcpy( writedir, argv[i+1] );
			printf( "Write output to: %s\n", writedir );
			i++;
		}
		else if ( !strcmp( argv[i], "-keypress") )
		{
			gbKeyPress = qtrue;
		}
		else if ( !strcmp( argv[i], "-verbose" ) )
		{
			g_verbose = qtrue;
		}
		else if ( !strcmp( argv[i], "-dump" ) )
		{
			printf( "Dumping contents of: '%s'\n", argv[i+1] );
			if ( strstr( argv[i+1], ".md3" ) )
			{
				MD3_Dump( argv[i+1] );
			}
			else
			{
				Error( "Do not know how to dump the contents of '%s'\n", argv[i+1] );
			}
			exit (0);
		}
		else if ( !strcmp( argv[i], "-3dsconvert" ) )
		{
			printf( "Converting %s.3DS to %s.MD3\n", argv[i+1], argv[i+1] );
			SetQdirFromPath( argv[i+1] );
			Convert3DStoMD3( argv[i+1] );
			i++;
		}
#if 0
		else if (!strcmp(argv[i], "-pak"))
		{
			g_release = qtrue;
			g_pak = qtrue;
			printf ("Building pakfile: %s\n", argv[i+1]);
			BeginPak (argv[i+1]);
			i++;
		}
#endif
		else if (!strcmp(argv[i], "-only"))
		{
			strcpy (g_only, argv[i+1]);
			printf ("Only grabbing %s\n", g_only);
			i++;
		}
		else if (argv[i][0] == '-')
			Error ("Unknown option \"%s\"", argv[i]);
		else
			break;
	}

	if (i == argc)
		Error ("Usage:\nq3data [-archive <dir>] [-dump <file>] [-release <dir>] [-only <model>] [-3ds] [-verbose] [-keypress] [file.qdt]");

	for ( ; i<argc ; i++)
	{
		printf ("--------------- %s ---------------\n", argv[i]);
		// load the script
		strcpy (path, argv[i]);
		DefaultExtension (path, ".qdt");
		SetQdirFromPath (path);
		LoadScriptFile (ExpandArg(path));
		
		//
		// parse it
		//
		ParseScript ();

		// write out the last model
		FinishModel ( TYPE_UNKNOWN );
	}

	// clean up any allocations we performed doing $aseanimgrab's
	CleanUpAfterGrabbing();

#if 0
	if (g_pak)
		FinishPak ();
#endif
	return 0;
}

