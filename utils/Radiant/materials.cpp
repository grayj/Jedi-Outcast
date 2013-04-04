// Filename:-	materials.cpp
//
// (File contains stuff imported from SoFED to fill in the materials dialogue box, only applies to SOF)
//
#include "stdafx.h"		// including these for what would otherwise be blank under Q3 keeps .PCHs working better.
#include "Radiant.h"



// Note: this file contains some stuff that's duplicated in other files (eg parse.cpp), but the code here has so
//	many fiddly differences (and is only loaded to fill in one dialogue in the SOF compile-version of this editor
//	that I've just kepy it all self-contained here, so it can be ripped out and disposed off later when the project 
//	is done.


#ifdef SOF	// may as well hide entire file except for SOF mode
#include "materials.h"
#include "direct.h"

//#include "cmdlib.h"
//#include "scriplib.h"


#define	BASEDIRNAME	"base"
#define PATHSEPERATOR   '/'


/*
=============================================================================

						PARSING STUFF

=============================================================================
*/

typedef struct
{
	char	filename[1024];
	char    *buffer,*script_p,*end_p;
	int     line;
} script_t;

#define	MAX_INCLUDES	8
script_t	scriptstack[MAX_INCLUDES];
script_t	*script;
int			sof_scriptline;

char    sof_token[MAXTOKEN];
qboolean endofscript;
qboolean tokenready;                     // only true if UnGetScriptToken was just called




// protos...
//
void AddScriptToStack (char *filename);
void LoadScriptFile (char *filename);
void ParseFromMemory (char *buffer, int size);
void UnGetScriptToken (void);
qboolean EndOfScript (qboolean crossline);
qboolean GetScriptToken (qboolean crossline);
qboolean ScriptTokenAvailable (void);
char *ExpandPath (char *path);




extern	char	sof_token[MAXTOKEN];
extern	int		sof_scriptline;

void	StartTokenParsing (char *data);
qboolean GetToken (qboolean crossline);
void UngetToken (void);
qboolean TokenAvailable (void);





void Q_getwd (char *out)
{
#ifdef WIN32
   _getcwd (out, 256);
   strcat (out, "\\");
#else
   getwd (out);
   strcat (out, "/");
#endif
}





/*

qdir will hold the path up to the quake directory, including the slash

  f:\quake\
  /raid/quake/

gamedir will hold qdir + the game directory (id1, id2, etc)

  */

char		qdir[1024];
char		gamedir[1024];

void SetQdirFromPath (char *path)
{
	char	temp[1024];
	char	*c;
	int		len;

	if (!(path[0] == '/' || path[0] == '\\' || path[1] == ':'))
	{	// path is partial
		Q_getwd (temp);
		strcat (temp, path);
		path = temp;
	}

	// search for "quake2" in path
#if 1
	len = strlen(BASEDIRNAME);

	for (c=path+strlen(path)-1 ; c != path ; c--)
	{
		if (!Q_strncasecmp (c, BASEDIRNAME, len))
		{
			strncpy (qdir, path, c-path);
			Sys_Printf ("qdir: %s\n", qdir);

			while (*c)
			{
				if (*c == '/' || *c == '\\')
				{
					strncpy (gamedir, path, c+1-path);
					Sys_Printf ("gamedir: %s\n", gamedir);
					return;
				}

				c++;
			}

//			assert(0);
			Error ("Gamedir at root in %s", path);

			return;
		}
	}

//	assert(0);
	Error ("SetQdirFromPath: no '%s' in %s", BASEDIRNAME, path);
#else
	len = strlen(BASEDIRNAME);
	for (c=path+strlen(path)-1 ; c != path ; c--)
		if (!Q_strncasecmp (c, BASEDIRNAME, len))
		{
			strncpy (qdir, path, c+len+1-path);
			qprintf ("qdir: %s\n", qdir);
			c += len+1;
			while (*c)
			{
				if (*c == '/' || *c == '\\')
				{
					strncpy (gamedir, path, c+1-path);
					qprintf ("gamedir: %s\n", gamedir);
					return;
				}
				c++;
			}
			Error ("No gamedir in %s", path);
			return;
		}
	Error ("SetQdirFromPath: no '%s' in %s", BASEDIRNAME, path);
#endif
}



materialtype_t	defaultmaterialtypes[] =
{
	{"gravel",	MATERIAL_GRAVEL},
	{"metal",	MATERIAL_METAL},
	{"stone",	MATERIAL_STONE},
	{"wood",	MATERIAL_WOOD},
	{NULL,		0}
};

materialtype_t	*materialtypes = NULL;// defaultmaterialtypes;

void QFile_ReadMaterialTypes(char* filename)
{
	int		i;
	FILE	*f;

	if (materialtypes != defaultmaterialtypes)
	{
		if (materialtypes)
			free(materialtypes);
		materialtypes = (materialtype_t*)malloc(256 * sizeof(materialtype_t));
	}
	
	f = fopen (filename, "rb");
	if (!f)
	{
		materialtypes = defaultmaterialtypes;
		return;
	}
	fclose (f);

	LoadScriptFile(filename);
	i = 0;
	
	do	
	{
		GetScriptToken (true);
		if (endofscript)
		{
			break;
		}
		if (strcmp(sof_token, "material") != 0)
		{
			while (ScriptTokenAvailable())
			{
				GetScriptToken(false);
			}
		}
		else
		{
			GetScriptToken(false);
			materialtypes[i].name = (char*)malloc(strlen(sof_token) + 1);
			strcpy(materialtypes[i].name, sof_token);
			GetScriptToken (false);
			materialtypes[i].value = atoi(sof_token);
		}
	}
	while (i++ < 255);

	materialtypes[i].name = NULL;
	materialtypes[i].value = 0;
}



char *ExpandPath (char *path)
{
	static char full[1024];
	if (!qdir)
		Error ("ExpandPath called without qdir set");
	if (path[0] == '/' || path[0] == '\\' || path[1] == ':')
		return path;
	sprintf (full, "%s%s", qdir, path);
	return full;
}



/*
==============
AddScriptToStack
==============
*/
void AddScriptToStack (char *filename)
{
	int            size;

	script++;
	if (script == &scriptstack[MAX_INCLUDES])
		Error ("script file exceeded MAX_INCLUDES");
	strcpy (script->filename, ExpandPath (filename) );

	size = LoadFile (script->filename, (void **)&script->buffer);

	printf ("entering %s\n", script->filename);

	script->line = 1;

	script->script_p = script->buffer;
	script->end_p = script->buffer + size;
}


/*
==============
LoadScriptFile
==============
*/
void LoadScriptFile (char *filename)
{
	script = scriptstack;
	AddScriptToStack (filename);

	endofscript = false;
	tokenready = false;
}


/*
==============
ParseFromMemory
==============
*/
void ParseFromMemory (char *buffer, int size)
{
	script = scriptstack;
	script++;
	if (script == &scriptstack[MAX_INCLUDES])
		Error ("script file exceeded MAX_INCLUDES");
	strcpy (script->filename, "memory buffer" );

	script->buffer = buffer;
	script->line = 1;
	script->script_p = script->buffer;
	script->end_p = script->buffer + size;

	endofscript = false;
	tokenready = false;
}


/*
==============
UnGetScriptToken

Signals that the current sof_token was not used, and should be reported
for the next GetScriptToken.  Note that

GetScriptToken (true);
UnGetScriptToken ();
GetScriptToken (false);

could cross a line boundary.
==============
*/
void UnGetScriptToken (void)
{
	tokenready = true;
}


qboolean EndOfScript (qboolean crossline)
{
	if (!crossline)
		Error ("Line %i is incomplete\n",sof_scriptline);

	if (!strcmp (script->filename, "memory buffer"))
	{
		endofscript = true;
		return false;
	}

	free (script->buffer);
	if (script == scriptstack+1)
	{
		endofscript = true;
		return false;
	}
	script--;
	sof_scriptline = script->line;
	printf ("returning to %s\n", script->filename);
	return GetScriptToken (crossline);
}

/*
==============
GetScriptToken
==============
*/
qboolean GetScriptToken (qboolean crossline)
{
	char    *token_p;

	if (tokenready)                         // is a sof_token allready waiting?
	{
		tokenready = false;
		return true;
	}

	if (script->script_p >= script->end_p)
		return EndOfScript (crossline);

//
// skip space
//
skipspace:
	while (*script->script_p <= 32)
	{
		if (script->script_p >= script->end_p)
			return EndOfScript (crossline);
		if (*script->script_p++ == '\n')
		{
			if (!crossline)
				Error ("Line %i is incomplete\n",sof_scriptline);
			sof_scriptline = script->line++;
		}
	}

	if (script->script_p >= script->end_p)
		return EndOfScript (crossline);

	// ; # // comments
	if (*script->script_p == ';' || *script->script_p == '#'
		|| ( script->script_p[0] == '/' && script->script_p[1] == '/') )
	{
		if (!crossline)
			Error ("Line %i is incomplete\n",sof_scriptline);
		while (*script->script_p++ != '\n')
			if (script->script_p >= script->end_p)
				return EndOfScript (crossline);
		goto skipspace;
	}

	// /* */ comments
	if (script->script_p[0] == '/' && script->script_p[1] == '*')
	{
		if (!crossline)
			Error ("Line %i is incomplete\n",sof_scriptline);
		script->script_p+=2;
		while (script->script_p[0] != '*' && script->script_p[1] != '/')
		{
			script->script_p++;
			if (script->script_p >= script->end_p)
				return EndOfScript (crossline);
		}
		script->script_p += 2;
		goto skipspace;
	}

//
// copy sof_token
//
	token_p = sof_token;

	if (*script->script_p == '"')
	{
		// quoted sof_token
		script->script_p++;
		while (*script->script_p != '"')
		{
			*token_p++ = *script->script_p++;
			if (script->script_p == script->end_p)
				break;
			if (token_p == &sof_token[MAXTOKEN])
				Error ("Token too large on line %i\n",sof_scriptline);
		}
		script->script_p++;
	}
	else	// regular sof_token
	while ( *script->script_p > 32 && *script->script_p != ';')
	{
		*token_p++ = *script->script_p++;
		if (script->script_p == script->end_p)
			break;
		if (token_p == &sof_token[MAXTOKEN])
			Error ("Token too large on line %i\n",sof_scriptline);
	}

	*token_p = 0;

	if (!strcmp (sof_token, "$include"))
	{
		GetScriptToken (false);
		AddScriptToStack (sof_token);
		return GetScriptToken (crossline);
	}

	return true;
}


/*
==============
ScriptTokenAvailable

Returns true if there is another sof_token on the line
==============
*/
qboolean ScriptTokenAvailable (void)
{
	char    *search_p;

	search_p = script->script_p;

	if (search_p >= script->end_p)
		return false;

	while ( *search_p <= 32)
	{
		if (*search_p == '\n')
			return false;
		search_p++;
		if (search_p == script->end_p)
			return false;

	}

	if (*search_p == ';')
		return false;

	return true;
}





char *ExpandArg (char *path)
{
	static char full[1024];

	if (path[0] != '/' && path[0] != '\\' && path[1] != ':')
	{
		Q_getwd (full);
		strcat (full, path);
	}
	else
		strcpy (full, path);
	return full;
}







#endif	// #ifdef SOF

///////////////////// eof //////////////////////////



