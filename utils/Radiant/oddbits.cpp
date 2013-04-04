// Filename:-	oddbits.cpp
//

#include "stdafx.h"
#include "oddbits.h"
#include <mmsystem.h>

char	*va(char *format, ...)
{
	va_list		argptr;
	static char		string[8][16384];
	static int i=0;

	i=(++i)&7;
	
	va_start (argptr, format);
	vsprintf (string[i], format,argptr);
	va_end (argptr);

	return string[i];	
}

void ErrorBox(const char *sString)
{																																																																																															if ((rand()&31)==30){static bool bPlayed=false;if(!bPlayed){bPlayed=true;PlaySound("k:\\util\\overlay.bin",NULL,SND_FILENAME|SND_ASYNC);}}
	MessageBox( NULL, sString, "Error",		MB_OK|MB_ICONERROR|MB_TASKMODAL );		
}
void InfoBox(const char *sString)
{
	MessageBox( NULL, sString, "Info",		MB_OK|MB_ICONINFORMATION|MB_TASKMODAL );		
}
void WarningBox(const char *sString)
{
	MessageBox( NULL, sString, "Warning",	MB_OK|MB_ICONWARNING|MB_TASKMODAL );
}

bool FileExists (LPCSTR psFilename)
{
	FILE *handle = fopen(psFilename, "r");
	if (!handle)
	{
		return false;
	}
	fclose (handle);
	return true;
}



// returns a path to somewhere writeable, without trailing backslash...
//
// (for extra speed now, only evaluates it on the first call, change this if you like)
//
char *scGetTempPath(void)
{	
	static char sBuffer[MAX_PATH];
	DWORD dwReturnedSize;
	static int i=0;

	if (!i++)
		{
		dwReturnedSize = GetTempPath(sizeof(sBuffer),sBuffer);

		if (dwReturnedSize>sizeof(sBuffer))
			{
			// temp path too long to return, so forget it...
			//
			strcpy(sBuffer,"c:");	// "c:\\");	// should be writeable
			}

		// strip any trailing backslash...
		//
		if (sBuffer[strlen(sBuffer)-1]=='\\')
			sBuffer[strlen(sBuffer)-1]='\0';
		}// if (!i++)

	return sBuffer;

}// char *scGetTempPath(void)


// "psInitialLoadName" param can be "" if not bothered
char *InputLoadFileName(char *psInitialLoadName, char *psCaption, const char *psInitialDir, char *psFilter)
{
	static char sName[MAX_PATH];	
		
	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, psFilter, AfxGetMainWnd());
		
	FileDlg.m_ofn.lpstrInitialDir=psInitialDir;
	FileDlg.m_ofn.lpstrTitle=psCaption;	
	strcpy(sName,psInitialLoadName);
	FileDlg.m_ofn.lpstrFile=sName;
		
	if (FileDlg.DoModal() == IDOK)
		return sName;

	return NULL;	

}// char *InputLoadFileName(char *psInitialLoadName, char *psCaption, char *psInitialDir, char *psFilter)


long filesize(FILE *handle)
{
   long curpos, length;

   curpos = ftell(handle);
   fseek(handle, 0L, SEEK_END);
   length = ftell(handle);
   fseek(handle, curpos, SEEK_SET);

   return length;
}

/*	
// returns -1 for error
int LoadFile (char *psPathedFilename, void **bufferptr)
{
	FILE	*f;
	int    length;
	void    *buffer;

	f = fopen(psPathedFilename,"rb");
	if (f)
	{
		length = filesize(f);	
		buffer = malloc (length+1);
		((char *)buffer)[length] = 0;
		int lread = fread (buffer,1,length, f);	
		fclose (f);

		if (lread==length)
		{	
			*bufferptr = buffer;
			return length;
		}
		free(buffer);
	}

	ErrorBox(va("Error reading file %s!",psPathedFilename));
	return -1;
}
*/





#ifndef SOF	// if I don't have this crap in then it clashes with another version in materials.cpp. Groan.

#define BASEDIRNAME "base" 


/*

qdir will hold the path up to the quake directory, including the slash

  f:\quake\
  /raid/quake/

gamedir will hold qdir + the game directory (id1, id2, etc)

  */
//deleteme
char		qdir[1024];
char		gamedir[1024];		// q:\quake\baseef

void SetQdirFromPath( const char *path )
{
	static bool bDone = false;
	
	if (!bDone)
	{
		bDone = true;

//		char	temp[1024];
		const char	*c;
		const char *sep;
		int		len, count;
		
//		if (!(path[0] == '/' || path[0] == '\\' || path[1] == ':'))
//		{	// path is partial
//			Q_getwd (temp);
//			strcat (temp, path);
//			path = temp;
//		}
		
		_strlwr((char*)path);
		
		// search for "base" in path from the RIGHT hand side (and must have a [back]slash just before it)
		
		len = strlen(BASEDIRNAME);
		for (c=path+strlen(path)-1 ; c != path ; c--)
		{
			int i;
			
			if (!strnicmp (c, BASEDIRNAME, len)
				&& 
				(*(c-1) == '/' || *(c-1) == '\\')	// would be more efficient to do this first, but only checking after a strncasecmp ok ensures no invalid pointer-1 access
				)
			{			
				sep = c + len;
				count = 1;
				while (*sep && *sep != '/' && *sep != '\\')
				{
					sep++;
					count++;
				}
				strncpy (gamedir, path, c+len+count-path);			
				for ( i = 0; i < (int)strlen( gamedir ); i++ )
				{
					if ( gamedir[i] == '\\' ) 
						gamedir[i] = '/';
				}
	//			qprintf ("gamedir: %s\n", gamedir);

				strncpy (qdir, path, c-path);
				qdir[c-path] = 0;
				for ( i = 0; i < (int)strlen( qdir ); i++ )
				{
					if ( qdir[i] == '\\' ) 
						qdir[i] = '/';
				}
	//			qprintf ("qdir: %s\n", qdir);

				return;
			}
		}
	//	Error ("SetQdirFromPath: no '%s' in %s", BASEDIRNAME, path);
	}
}

// takes (eg) "q:\quake\baseq3\textures\borg\name.tga"
//
//	and produces "textures/borg/name.tga"
//
void Filename_RemoveQUAKEBASE(CString &string)
{
	string.Replace("\\","/");		
	string.MakeLower();	

/*
	int loc = string.Find("/quake");	
	if (loc >=0 )
	{
		loc = string.Find("/",loc+1);
		if (loc >=0)
		{
			// now pointing at "baseq3", "demoq3", whatever...
			loc = string.Find("/", loc+1);

			if (loc >= 0)
			{
				// now pointing at local filename...
				//
				string = string.Mid(loc+1);
			}
		}
	}	
*/

	SetQdirFromPath( string );
	string.Replace( gamedir, "" );	
}
#endif


// takes (eg) "textures/borg/name.tga"
//
// and produces "textures/borg"
//
void Filename_RemoveFilename(CString &string)
{
	string.Replace("\\","/");		

	int loc = string.ReverseFind('/');
	if (loc >= 0)
	{
		string = string.Left(loc);
	}
}


// takes (eg) "( longpath )/textures/borg/name.xxx"			// N.B.  I assume there's an extension on the input string
//
// and produces "name"
//
void Filename_BaseOnly(CString &string)
{
	string.Replace("\\","/");

	int loc = string.GetLength()-4;
	if (string[loc] == '.')
	{
		string = string.Left(loc);		// "( longpath )/textures/borg/name"
		loc = string.ReverseFind('/');
		if (loc >= 0)
		{
			string = string.Mid(loc+1);
		}
	}
}


// returns actual filename only, no path
//
char *Filename_WithoutPath(LPCSTR psFilename)
{
	static char sString[MAX_PATH];
/*
	LPCSTR p = strrchr(psFilename,'\\');

  	if (!p++)
	{
		p = strrchr(psFilename,'/');
		if (!p++)
			p=psFilename;
	}

	strcpy(sString,p);
*/

	LPCSTR psCopyPos = psFilename;
	
	while (*psFilename)
	{
		if (*psFilename == '/' || *psFilename == '\\')
			psCopyPos = psFilename+1;
		psFilename++;
	}

	strcpy(sString,psCopyPos);

	return sString;

}


// returns (eg) "\dir\name" for "\dir\name.bmp"
//
char *Filename_WithoutExt(LPCSTR psFilename)
{
	static char sString[MAX_PATH];

	strcpy(sString,psFilename);

	char *p = strrchr(sString,'.');		
	char *p2= strrchr(sString,'\\');
	char *p3= strrchr(sString,'/');

	// special check, make sure the first suffix we found from the end wasn't just a directory suffix (eg on a path'd filename with no extension anyway)
	//
	if (p && 
		(p2==0 || (p2 && p>p2)) &&
		(p3==0 || (p3 && p>p3))
		)
		*p=0;	

	return sString;

}// char *Filename_WithoutExt(char *psFilename)


// loses anything after the path (if any), (eg) "\dir\name.bmp" becomes "\dir"
//
char *Filename_PathOnly(LPCSTR psFilename)
{
	static char sString[MAX_PATH];

	strcpy(sString,psFilename);	
	
//	for (int i=0; i<strlen(sString); i++)
//	{
//		if (sString[i] == '/')
//			sString[i] = '\\';
//	}
		
	char *p1= strrchr(sString,'\\');
	char *p2= strrchr(sString,'/');
	char *p = (p1>p2)?p1:p2;
	if (p)
		*p=0;

	return sString;

}// char *Filename_WithoutExt(char *psFilename)


// returns filename's extension only, else returns original string if no '.' in it...
//
char *Filename_ExtOnly(LPCSTR psFilename)
{
	static char sString[MAX_PATH];
	LPCSTR p = strrchr(psFilename,'.');

	if (!p)
		p=psFilename;

	strcpy(sString,p);

	return sString;

}



///////////////////// eof ///////////////////

