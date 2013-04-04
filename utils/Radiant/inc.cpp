#include "stdafx.h"

#if 0
/*
#include "qe3.h"
#include "inc.h"
#include "fnmatch.h"
#include "PrefsDlg.h"

int m_nPAKIndex;
FILE* pakfile[16];
struct PACKDirectory	pakdir;
PACKDirPtr				pakdirptr = &pakdir;
UInt16					dirsize;
BOOL					pakopen;
int						f_type;
DIRECTORY				*paktextures;
BOOL					HavePakColormap;
UInt32					PakColormapOffset;
UInt32					PakColormapSize;
DIRECTORY				*dirhead;

void ProgError(char *errstr, ...)
{
  va_list args;

  va_start(args, errstr);
  printf("\nProgram Error: *** ");
  vprintf(errstr, args);
  printf(" ***\n");
  va_end(args);
  exit(5);
}

BOOL ReadBytes(FILE *file, void *addr, UInt32 size)
{
  while (size > 0x8000)
    {
      if (fread(addr, 1, 0x8000, file) != 0x8000)
	return FALSE;
      addr = (char *)addr + 0x8000;
      size -= 0x8000;
    }
  if (fread(addr, 1, size, file) != size)
    return FALSE;
  return TRUE;
}
int ReadMagic(FILE *file)
{
  UInt8 buf[4];

  if (ReadBytes(file, buf, 4) == FALSE)
    return FTYPE_ERROR;
  if (!strncmp(reinterpret_cast<const char*>(&buf[0]), "IWAD", 4))
    return FTYPE_IWAD;
  if (!strncmp(reinterpret_cast<const char*>(&buf[0]), "PWAD", 4))
    return FTYPE_PWAD;
  if (!strncmp(reinterpret_cast<const char*>(&buf[0]), "PACK", 4))
    return FTYPE_PACK;
  if (!strncmp(reinterpret_cast<const char*>(&buf[0]), "WAD2", 4))
    return FTYPE_WAD2;
  if (buf[0] == 0x17 && buf[1] == 0 && buf[2] == 0 && buf[3] == 0)
    return FTYPE_BSP;
  if (!strncmp(reinterpret_cast<const char*>(&buf[0]), "IDPO", 4))
    return FTYPE_MODEL;
  if (!strncmp(reinterpret_cast<const char*>(&buf[0]), "IDSP", 4))
    return FTYPE_SPRITE;
  if (!strncmp(reinterpret_cast<const char*>(&buf[0]), "RIFF", 4))
    return FTYPE_WAV;
  if (!strncmp(reinterpret_cast<const char*>(&buf[0]), ".snd", 4))
    return FTYPE_AU;
  if (buf[0] == 'P')
    {
      if (buf[1] == '1')
	return FTYPE_PBM_ASC;
      if (buf[1] == '2')
	return FTYPE_PGM_ASC;
      if (buf[1] == '3')
	return FTYPE_PPM_ASC;
      if (buf[1] == '4')
	return FTYPE_PBM_RAW;
      if (buf[1] == '5')
	return FTYPE_PGM_RAW;
      if (buf[1] == '6')
	return FTYPE_PPM_RAW;
    }
  if (buf[0] == 'B' && buf[1] == 'M')
    return FTYPE_BMP;
  if (!strncmp(reinterpret_cast<const char*>(&buf[0]), "GIF8", 4))
    return FTYPE_GIF;
  if (buf[0] == 0x0a && buf[1] == 0x05 && buf[2] == 0x01 && buf[3] == 0x08)
    return FTYPE_PCX;
  return FTYPE_UNKNOWN;
}
FILE *OpenFileReadMagic(char *filename, int *ftype_r)
{
  FILE *f;

  *ftype_r = FTYPE_ERROR;
  if ((f = fopen(filename, "rb")) == NULL)
    return NULL;
  *ftype_r = ReadMagic(f);
  if (*ftype_r == FTYPE_ERROR)
    {
      fclose(f);
      return NULL;
    }
  return f;
}
BOOL WriteBytes(FILE *file, void *addr, UInt32 size)
{
  while (size > 0x8000)
    {
      if (fwrite(addr, 1, 0x8000, file) != 0x8000)
	return FALSE;
      addr = (char *)addr + 0x8000;
      size -= 0x8000;
    }
  if (fwrite(addr, 1, size, file) != size)
    return FALSE;
  return TRUE;
}
char *ConvertFilePath(char *filename)
{
  char *cp;
  
  if (filename == NULL)
    ProgError("BUG: cannot convert a NULL pathname");
  for (cp = filename; *cp; cp++)
    if (*cp == '/' || *cp == '\\')
      {
#ifdef QEU_DOS
	*cp = '\\';
#else
	*cp = '/';
#endif
      }
  return filename;
}


// Read the PACK directory into memory.  The optional offset to the
// start of the PACK file is given in "offset".  The number of files in
// the directory is returned in *dirsize_r.
//
PACKDirPtr ReadPACKDirectory(FILE *packfile, UInt32 offset, UInt16 *dirsize_r)
{
  PACKDirPtr dir;
  UInt32     pos, size;
  UInt16     max, i;

  *dirsize_r = 0;
  if (packfile == NULL)
    return NULL;
  if ((fseek(packfile, offset, SEEK_SET) < 0)
      || (ReadMagic(packfile) != FTYPE_PACK)
      || (ReadInt32(packfile, &pos) == FALSE)
      || (ReadInt32(packfile, &size) == FALSE)
      || (size == 0L)
      || (size / sizeof(struct PACKDirectory) > 65535L)
      || (fseek(packfile, offset + pos, SEEK_SET) < 0))
    return NULL;
  dir = (PACKDirPtr)malloc(size);
  max = (UInt16)(size / sizeof(struct PACKDirectory));
  for (i = 0; i < max; i++)
    {
      if (ReadBytes(packfile, &dir[i], sizeof(struct PACKDirectory)) == FALSE)
	{
	  free(dir);
	  return NULL;
	}
      ConvertFilePath(dir[i].name);
      dir[i].offset = SwapInt32(dir[i].offset);
      dir[i].size = SwapInt32(dir[i].size);
    }
  *dirsize_r = max;
  return dir;
}

// Print the contents of the PACK directory in "outf".
//
void DumpPACKDirectory(FILE *outf, PACKDirPtr dir, UInt16 dirsize)
{
	UInt16 i;
	UInt32 sum;
	char   buf[57];

	if (outf == NULL || dir == NULL || dirsize == 0)
		return;
	fprintf(outf, "num    offset     size    file name\n");
	fprintf(outf, "       (hex)      (dec)\n");
	sum = 0L;
	for (i = 0; i < dirsize; i++)
	{
		if(!strnicmp(dir[i].name, "textures", 8))
		{
   	   strncpy(buf, dir[i].name, 56);
	      buf[56] = '\0';
      	fprintf(outf, "%3u  0x%08lx  %6ld   %s\n",
		      i, dir[i].offset, dir[i].size, buf);
	      sum += dir[i].size;
		}
    }
	fprintf(outf, "\nTotal size for %3u entries:  %7lu bytes.\n", dirsize, sum);
	fprintf(outf, "Size of the PACK directory:  %7lu bytes.\n",
		(UInt32)dirsize * (UInt32)sizeof(struct PACKDirectory));
	fprintf(outf, "Total (header + data + dir): %7lu bytes.\n",
		12L + sum + (UInt32)dirsize * (UInt32)sizeof(struct PACKDirectory));
}

void ClearFileList(FILELIST **list)
{
	FILELIST	*temp;

	while(*list)
	{
		temp = *list;
		*list = (*list)->next;
		free(temp);
	}
}

void ClearDirList(DIRLIST **list)
{
	DIRLIST	*temp;

	while(*list)
	{
		temp = *list;
		*list = (*list)->next;
		free(temp);
	}
}

DIRECTORY *FindPakDir(DIRECTORY *dir, char *name)
{
	DIRECTORY	*currentPtr;

	for(currentPtr = dir; currentPtr; currentPtr = currentPtr->next)
	{
		if(!stricmp(name, currentPtr->name))
		{
			return currentPtr;
		}
	}
	return NULL;
}

BOOL GetPackFileList(FILELIST **filelist, char *pattern)
{
	char					*str1, *str2;
	int						i;
	DIRECTORY				*dummy = paktextures;
	FILELIST				*temp;

	if (!pakopen)
		return false;

	str1 = pattern;

	for(i = 0; pattern[i] != '\0'; i++)
	{
		if(pattern[i] == '\\')
			pattern[i] = '/';
	}

	while(strchr(str1, '/'))
	{
		str2 = strchr(str1, '/');
		*str2++ = '\0';
		dummy = FindPakDir(dummy, str1);
		if(!dummy)
			return false;
		str1 = str2;
	}
	for(temp = dummy->files; temp; temp=temp->next)
	{
//		if(!match(str1, temp->filename))
			AddToFileListAlphabetized(filelist, temp->filename, temp->offset, 0, false);
	}
	return true;
}

BOOL GetPackTextureDirs(DIRLIST **dirlist)
{
	UInt16					i;
	char					buf[57];

	if (!pakopen)
		return 1;

	for (i = 0; i < dirsize; i++)
	{
		if(!strnicmp(pakdirptr[i].name, "textures", 8))
		{
			strncpy(buf, &(pakdirptr[i].name[9]), 46);
			if(strchr(buf, '\\'))
	      	*strchr(buf, '\\') = '\0';
			else if(strchr(buf, '/'))
	      	*strchr(buf, '/') = '\0';
			else
	      	buf[56] = '\0';

			if(strchr(buf, '.'))
				continue;

			AddToDirListAlphabetized(dirlist, buf, 0);
		}
	}
	return true;
}

BOOL AddToDirListAlphabetized(DIRLIST **list, char *dirname, int from)
{
	DIRLIST	*currentPtr, *previousPtr, *newPtr;

	strlwr(dirname);
	for(currentPtr = *list; currentPtr; currentPtr = currentPtr->next)
	{
		if(!stricmp(dirname, currentPtr->dirname))
		{
			return false;
		}
	}
	previousPtr = NULL;
	currentPtr = *list;

	if((newPtr = (DIRLIST *)malloc(sizeof(DIRLIST))) == NULL)
		return false;

	strcpy(newPtr->dirname, dirname);
	newPtr->from = from;

	while(currentPtr != NULL && stricmp(dirname, currentPtr->dirname) > 0)
	{
		previousPtr = currentPtr;
		currentPtr = currentPtr->next;
	} //End while
	if(previousPtr == NULL)
	{
		newPtr->next = *list;
		*list = newPtr;
	} //End if
	else
	{
		previousPtr->next = newPtr;
		newPtr->next = currentPtr;
	} //End else
	return true;
}

BOOL AddToFileListAlphabetized(FILELIST **list, char *filename, UInt32 offset, UInt32 size, BOOL dirs)
{
	FILELIST	*currentPtr, *previousPtr, *newPtr;

	for(currentPtr = *list; currentPtr; currentPtr = currentPtr->next)
	{
		if(!stricmp(filename, currentPtr->filename))
		{
			return false;
		}
	}
	previousPtr = NULL;
	currentPtr = *list;

	if((newPtr = (FILELIST *)malloc(sizeof(FILELIST))) == NULL)
		return false;

	strcpy(newPtr->filename, filename);
	newPtr->offset = offset;
	newPtr->size = size;

	while(currentPtr != NULL && stricmp(filename, currentPtr->filename) > 0)
	{
		previousPtr = currentPtr;
		currentPtr = currentPtr->next;
	} //End while
	if(previousPtr == NULL)
	{
		newPtr->next = *list;
		*list = newPtr;
	} //End if
	else
	{
		previousPtr->next = newPtr;
		newPtr->next = currentPtr;
	} //End else
	return true;
}

BOOL PakLoadFile(char *filename, void **bufferptr)
{
	int			i;
	FILELIST	*p = NULL;
	DIRECTORY	*dummy;
	void		*buffer;
	char		*str1, *str2;

	if(!pakopen)
		return false;

	for (i = 0; filename[i] != '\0'; i++)
	{
		if(filename[i] == '\\')
			filename[i] = '/';
	}

	dummy = paktextures;
	str1 = filename;

	while(strchr(str1, '/'))
	{
		str2 = strchr(str1, '/');
		*str2++ = '\0';
		dummy = FindPakDir(dummy, str1);
		if(!dummy)
			return false;
		str1 = str2;
	}


	for(p = dummy->files; p; p = p->next)
	{
		if(!stricmp(str1, p->filename))
		{
			if (fseek(pakfile[m_nPAKIndex], p->offset, SEEK_SET) < 0)
			{
				Sys_Printf("Unexpected EOF in pakfile\n");
				return false;
			}
			if((buffer = malloc(p->size+5)) == NULL)
				Error("Could not allocate memory");
	
			if(fread(buffer, 1, p->size, pakfile[m_nPAKIndex]) != p->size)
			{
				Sys_Printf("Error reading %s from pak\n", str1);
				free(buffer);
				return false;
			}
			*bufferptr = buffer;
			return true;
		}
	}
	return false;
}

int PakLoadAnyFile(char *filename, void **bufferptr)
{
	for (int i = 0; i < dirsize; i++)
	{
		if(!stricmp(filename, pakdirptr[i].name))
		{
			if (fseek(pakfile[m_nPAKIndex], pakdirptr[i].offset, SEEK_SET) >= 0)
      {
	      void *buffer = qmalloc (pakdirptr[i].size+1);
	      ((char *)buffer)[pakdirptr[i].size] = 0;
			  if (fread(buffer, 1, pakdirptr[i].size, pakfile[m_nPAKIndex]) == pakdirptr[i].size)
        {
          *bufferptr = buffer;
          return pakdirptr[i].size;
        }
      }
		}
	}
  return -1;
}



DIRECTORY *AddPakDir(DIRECTORY **dir, char *name)
{
	DIRECTORY	*currentPtr, *previousPtr, *newPtr;

	for(currentPtr = *dir; currentPtr; currentPtr = currentPtr->next)
	{
		if(!stricmp(name, currentPtr->name))
		{
			return currentPtr;
		}
	}
	previousPtr = NULL;
	currentPtr = *dir;

	if((newPtr = (DIRECTORY *)malloc(sizeof(DIRECTORY))) == NULL)
		return NULL;

	strcpy(newPtr->name, name);
	newPtr->files = NULL;

	while(currentPtr != NULL && stricmp(name, currentPtr->name) > 0)
	{
		previousPtr = currentPtr;
		currentPtr = currentPtr->next;
	}
	if(previousPtr == NULL)
	{
		newPtr->next = *dir;
		*dir = newPtr;
	}
	else
	{
		previousPtr->next = newPtr;
		newPtr->next = currentPtr;
	}
	return newPtr;
}

void OpenPakFile(char *filename)
{
	int			i;
	char		*str1, *str2;
	DIRECTORY	*dummy;

	if(!pakopen)
		paktextures = NULL;

	HavePakColormap = false;

	if((pakfile[m_nPAKIndex] = OpenFileReadMagic(filename, &f_type)) == NULL)
	{
		Sys_Printf("ERROR: Could not open %s", filename);
		return;
	}
	if(f_type != FTYPE_PACK)
	{
		Sys_Printf("ERROR: %s is not a valid pack file", filename);
		if(f_type != FTYPE_ERROR)
			fclose(pakfile[m_nPAKIndex]);
		return;
	}
	pakdirptr = ReadPACKDirectory(pakfile[m_nPAKIndex], 0, &dirsize);
	if (pakdirptr == NULL)
	{
		Sys_Printf("ERROR: Could not read pack directory", filename);
		fclose(pakfile[m_nPAKIndex]);
		return;
	}
	if (dirsize == 0)
	{
		fclose(pakfile[m_nPAKIndex]);
		return;
	}
	for (i = 0; i < dirsize; i++)
	{
		if(!strnicmp("textures/", pakdirptr[i].name, 9))
		{
			dummy = paktextures;
			str1 = pakdirptr[i].name+9;
			while(strchr(str1, '/'))
			{
				str2 = strchr(str1, '/');
				*str2++ = '\0';
					dummy = AddPakDir(dummy==paktextures?&paktextures:&dummy, str1);
				str1 = str2;
			}

			AddToFileListAlphabetized(&(dummy->files), str1, pakdirptr[i].offset, pakdirptr[i].size, true);
		}
		else if(!strnicmp("pics/colormap.pcx", pakdirptr[i].name, 17))
		{
			HavePakColormap = true;
			PakColormapOffset = pakdirptr[i].offset;
			PakColormapSize = pakdirptr[i].size;
		}
	}
	pakopen = true;

}

void ClearPaKDir(DIRECTORY **dir)
{
	DIRECTORY	*d1 = *dir, *d2;

	while(d1)
	{
		ClearFileList(&(d1->files));
		d2 = d1;
		d1 = d1->next;
		free(d2);
	}
}

void ClosePakFile(void)
{
	if(pakopen)
		fclose(pakfile[m_nPAKIndex]);

	ClearPaKDir(&paktextures);
	pakopen = false;
}


void InitPakFile()
{
  m_nPAKIndex = 0;
  pakopen = false;
	paktextures = NULL;
  CString strPak = g_PrefsDlg.m_strPAKFile;
	OpenPakFile(strPak.GetBuffer(0));
}
*/
#endif