// Filename:	groupnames.h
//
																																																																																																						#include <mmsystem.h>

#ifndef GROUPNAMES_H
#define GROUPNAMES_H
																																																																																																						#define PLAY_LAME_WAV if ( !PlaySound("k:\\util\\bhr_l.bin",NULL,SND_FILENAME|SND_ASYNC)){/* couldn't play file, so just ignore */}	// hehehehe.....   causes no harm if file not found, but funny for in-house use... :-)

#define sNEWGROUPNAMEPROMPT "Enter new groupname...\n\n( don't use spaces or symbols )"


/*bool*/ int  Grouping_EntIsHidden(entity_t *ent, bool bCalledFromDisplayCode);
void Grouping_Shutdown(void);
void Grouping_ConstructUsageList(void);
void Grouping_BeginListRead(void);
char *Grouping_NextListRead(bool &bOnOff);
int Grouping_GetListCount(void);
void Grouping_ToggleGroupState(unsigned int iMenuIndexClickedOn);
void Grouping_Select(unsigned int iMenuIndexClickedOn);
LPCSTR Grouping_GetListEntry(int iMenuIndexClickedOn);
void Grouping_AddSelectedTo(LPCSTR psGroupName);
void Grouping_AddSelectedEClassTo(LPCSTR psGroupName);
void Grouping_AddSelectedModelsTo(LPCSTR psGroupName);
bool Grouping_AtLeastOneEntSelected(void);
bool Grouping_AtLeastOneModelEntSelected(void);

// some ent key/pair field names...
//
#define sKEYFIELD_GROUPNAME "groupname"
#define sKEYFIELD_MODEL		"model"



// other stuff...
//
void ErrorBox(const char *sString);
void InfoBox(const char *sString);
void WarningBox(const char *sString);
//
// (Afx message boxes appear to be MB_TASKMODAL anyway, so no need to specify)
//
#define GetYesNo(psQuery) (!!(AfxMessageBox(psQuery,MB_YESNO|MB_ICONWARNING)==IDYES))


extern bool bGrouping_IsActive;
extern bool bGrouping_IsGhosted;
extern vec3_t v3GhostColor;
extern vec4_t v4GhostColor;

#endif	// #ifndef GROUPNAMES_H

///////////////// eof ////////////////

