// Filename:-	groupnames.cpp
//
#include "stdafx.h"
#include "Radiant.h"
#include "qe3.h"
#include "groupnames.h"
#include <assert.h>

// I fucking hate this pragma shit

#pragma warning(disable : 4786)     // shut the fuck up about debug symbol name length
#include <set>
#include <string>
using namespace std;

// all of the groupname equates in resource.h for this type have 100 entries, or 100+1 for the ones that have a menu
//	item such as <new group>. Unfortunately resource.h isn't the world's greatest file to put comments in, since VC
//	rewrites it when it feels like it, so fingers crossed no-one fucks with my code.
//
#define MAX_GROUPNAMES ((ID_SCRIPTPOPOP_GROUPNAMES_ID_END-ID_SCRIPTPOPOP_GROUPNAMES_ID_START)+1)


bool bGrouping_IsActive = true;		// activates / deactivates all grouping at once
bool bGrouping_IsGhosted= false;	// controls whether hidden groups are still displayed, albeit as ghosts, for placement

vec3_t v3GhostColor = {0.8f,0.8f,0.8f};
vec4_t v4GhostColor = {0.8f,0.8f,0.8f,0.4f};

// to avoid adding STL to this project I'll use key/pairs on fake ents to maintain simple string lists... :-)
//
// (note, that comment was from the EF1 codebase, but WTF, it works...)
//
entity_t fakeEnt1={0};
entity_t fakeEnt2={0};

entity_t *fakeEnts[2]={&fakeEnt1,&fakeEnt2};
int ifakeEntIndex=0;	// this should only be toggled in one place!!


// returns:
// 0 for not-hidden
// 1 for hidden
// 2 for hidden, but ghosted
//	
/*bool*/ int Grouping_EntIsHidden(entity_t *ent, bool bCalledFromDisplayCode)
{
	if (bGrouping_IsActive)
	{
		// does this ent have a group name?...
		//
		char *psGroupName = ValueForKey(ent, sKEYFIELD_GROUPNAME);	
		if (strlen(psGroupName))
		{
			// is this groupname a hidden one?
			//
			char *psBoolValue = ValueForKey(fakeEnts[ifakeEntIndex], psGroupName);

			if (strlen(psBoolValue))
			{
				bool bHidden = !atoi(psBoolValue);

				if (!bCalledFromDisplayCode || !bHidden)
					return bHidden;

				return bGrouping_IsGhosted?2:1;
			}
		}
	}

	return 0;
}


static void Entity_DeleteKeys(entity_t *ent)	// I'd have thought there'd be one of these already, but...
{
	epair_t* pEPair = ent->epairs;
	
	while (pEPair)
	{
		epair_t* pNextEPair = pEPair->next;
		free (pEPair->key);
		free (pEPair->value);
		free (pEPair);
		pEPair = pNextEPair;
	}
	
	ent->epairs = NULL;
}

void Grouping_Shutdown(void)
{		
	for (int i=0; i<sizeof(fakeEnts) / sizeof(fakeEnts[0]); i++)
	{
		Entity_DeleteKeys(fakeEnts[i]);
	}
}


// for filling in menus, do the following (only):
//
// Grouping_ConstructUsageList();
// Grouping_BeginListRead();
// 
// char *psString;
// bool bTicked;
//
// while ((psString = Grouping_NextListRead(&bTicked))
//      Add-menu-item ( psString, bTicked );
//


// make up a list of all groupnames in use, and discard any that no longer exist. 
//	This is done by keeping two lists, building into one and reading from the other for any prev values, then deleting
//	the other one. This keeps usage lists to in-use only, while preserving settings.
//
void Grouping_ConstructUsageList()
{
	entity_t *prevEnt = fakeEnts[ifakeEntIndex];

	ifakeEntIndex = (++ifakeEntIndex)&1;	// this should be the only place the index is changed!

	entity_t *currEnt = fakeEnts[ifakeEntIndex];

	// empty this list...
	//
	Entity_DeleteKeys(currEnt);

	// scan all ents, build up new list...
	//
	for (entity_t* pEnt=entities.next ; pEnt != &entities ; pEnt=pEnt->next)
	{
		char *psGroupName = ValueForKey(pEnt, sKEYFIELD_GROUPNAME);

		if (strlen(psGroupName))
		{
			// make sure this groupname is added to the list...
			//
			// first, did this groupname key exist in the previous list?
			//
			char *psBoolValue = ValueForKey(prevEnt, psGroupName);

			if (!strlen(psBoolValue))
			{
				// no prev bool found, so...
				//
				psBoolValue = "1";	// new groups defaulted to ON (of course). If not, an ent would vanish as soon as you entered a new groupname <g>
			}

			SetKeyValue(currEnt, psGroupName, psBoolValue);
		}
	}
}



// I hadn't really wanted to do this, but...
//
int Grouping_GetListCount(void)
{
	Grouping_BeginListRead();

	int iCount = 0;
	bool bScrap;
	
	while (Grouping_NextListRead(bScrap)) iCount++;

	return (iCount<MAX_GROUPNAMES)?iCount:MAX_GROUPNAMES;	// limit how many are offered to the user via menu
}


// returns NULL or valid group name...
//
LPCSTR Grouping_GetListEntry(int iMenuIndexClickedOn)
{
	Grouping_BeginListRead();

	while (1)
	{
		bool bOnOff;
		char *psGroupName = Grouping_NextListRead(bOnOff);

		if (!psGroupName)
		{
			assert(0);
			Sys_Printf("Grouping_GetListEntry() Error: unable to locate groupname index %d!, tell me! (Ste)\n",iMenuIndexClickedOn);
			return NULL;	// can't find it ?!?!?!?!, err....
		}

		if (!iMenuIndexClickedOn--)
		{
			return psGroupName;
		}
	}
}


int iReadIndex;
void Grouping_BeginListRead(void)
{
	iReadIndex = 0;
}

char *Grouping_NextListRead(bool &bOnOff)
{
	int iIndex = iReadIndex++;

	epair_t* pEPair = fakeEnts[ifakeEntIndex]->epairs;
	
	while (pEPair && iIndex)
	{
		pEPair = pEPair->next;

		iIndex--;
	}

	if (!iIndex && pEPair)
	{
		bOnOff = !!atoi(pEPair->value);
		return pEPair->key;
	}

	return NULL;
}



void Grouping_ToggleGroupState(unsigned int iMenuIndexClickedOn)
{
	Grouping_BeginListRead();

	while (1)
	{
		bool bOnOff;
		char *psGroupName = Grouping_NextListRead(bOnOff);

		if (!psGroupName)
		{
			Sys_Printf("Error: unable to locate groupname index %d!         tell me! (Ste)\n",iMenuIndexClickedOn);
			break;	// can't find it, err....
		}

		if (!iMenuIndexClickedOn--)
		{
			// this is the one, so toggle it in the current grouping list...
			//
			SetKeyValue(fakeEnts[ifakeEntIndex], psGroupName, !bOnOff?"1":"0");
			break;
		}
	}

	Sys_UpdateWindows(W_ALL);
}


// select all ents belonging to the specified group in the menu we've just clicked on...
//
void Grouping_Select(unsigned int iMenuIndexClickedOn)
{
	Grouping_BeginListRead();

	while (1)
	{
		bool junk;
		char *psGroupName = Grouping_NextListRead(junk);

		if (!psGroupName)
		{
			Sys_Printf("Error: unable to locate groupname index %d!         tell me! (Ste)\n",iMenuIndexClickedOn);
			break;	// can't find it, err....
		}

		if (!iMenuIndexClickedOn--)
		{
			// this is the one, so select all brushes who use this group...
			//
			brush_t* next;
			brush_t* b;

			g_bScreenUpdates = false;	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!
			{
				for (b=active_brushes.next ; b != &active_brushes ; b=next)
				{
					next = b->next;				// important to do this here, in case brush gets linked to selected list
					entity_t* ent = b->owner;

					if (ent)	// needed!
					{
	 					if (FilterBrush (b))
	 						continue;

						// does this ent belong to the group we're trying to switch on?
						//
						char *psEntGroupName = ValueForKey(ent, sKEYFIELD_GROUPNAME);

						if (strcmp(psGroupName, psEntGroupName)==0)
						{					
							// yes, so select it...
							//
							Select_Brush(b, false);
						}
					}
				}
			}
			g_bScreenUpdates = true;	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!
			break;
		}
	}

	Sys_UpdateWindows(W_ALL);
}


void Grouping_AddSelectedTo(LPCSTR psGroupName)
{	
	bool bUpdated = false;

	for (brush_t *pBrush = selected_brushes.next ; pBrush != &selected_brushes ; pBrush = pBrush->next)
	{
		if (pBrush->owner && pBrush->owner != world_entity)
		{
			SetKeyValue(pBrush->owner, sKEYFIELD_GROUPNAME, psGroupName);
			bUpdated = true;
		}
	}


	if (bUpdated )
	{
		Sys_MarkMapModified();
	}
}



void Grouping_AddSelectedEClassTo(LPCSTR psGroupName)
{
	bool bUpdated = false;

	typedef set <string>	EClasses_t;
							EClasses_t EClasses;

	// first, build a list of all the eclasses of the selected (non-world) brush ents...
	//
	for (brush_t *pBrush = selected_brushes.next ; pBrush != &selected_brushes ; pBrush = pBrush->next)
	{
		if (pBrush->owner && pBrush->owner != world_entity)
		{
			EClasses.insert(EClasses.end(), pBrush->owner->eclass->name);			
			//
			// and set the groupname as well while we're here...
			//
			SetKeyValue(pBrush->owner, sKEYFIELD_GROUPNAME, psGroupName);
			bUpdated = true;
		}
	}


	// now go through the eclass list we've just built, and search through all non-selected (and non-world) ent-brushes 
	//	for ones with the same ent class as the list, and set them to the supplied groupname...
	//
	for (EClasses_t::iterator it = EClasses.begin(); it != EClasses.end(); ++it)
	{
		string strEClassName = (*it);

		for (pBrush = active_brushes.next ; pBrush != &active_brushes ; pBrush = pBrush->next)
		{
			if (pBrush->owner && pBrush->owner != world_entity)
			{			
	 			if (FilterBrush (pBrush))
	 				continue;

				// does this ent's eclass match the one we're checking for?...
				//
				if (stricmp(pBrush->owner->eclass->name, strEClassName.c_str()) == 0)
				{
					SetKeyValue(pBrush->owner, sKEYFIELD_GROUPNAME, psGroupName);
					bUpdated = true;
				}
			}
		}
	}


	if (bUpdated )
	{
		Sys_MarkMapModified();
	}
}


void Grouping_AddSelectedModelsTo(LPCSTR psGroupName)
{
	bool bUpdated = false;

	typedef set <string>	ModelsUsed_t;
							ModelsUsed_t ModelsUsed;

	// first, build a list of all the eclasses of the selected (non-world) brush ents...
	//
	for (brush_t *pBrush = selected_brushes.next ; pBrush != &selected_brushes ; pBrush = pBrush->next)
	{
		if (pBrush->owner && pBrush->owner != world_entity)
		{
			char *psModelName = ValueForKey (pBrush->owner, sKEYFIELD_MODEL);
			if (strlen(psModelName))	// non-blank entry?
			{
				ModelsUsed.insert(ModelsUsed.end(), psModelName);
				//
				// and set the groupname as well while we're here...
				//
			}
			SetKeyValue(pBrush->owner, sKEYFIELD_GROUPNAME, psGroupName);	// deliberately outside the IF
			bUpdated = true;
		}
	}


	// now go through the models-used list we've just built, and search through all non-selected (and non-world) ent-brushes 
	//	which are also using this model and set them to the supplied groupname...
	//
	for (ModelsUsed_t::iterator it = ModelsUsed.begin(); it != ModelsUsed.end(); ++it)
	{
		string strModelUsedName = (*it);

		for (pBrush = active_brushes.next ; pBrush != &active_brushes ; pBrush = pBrush->next)
		{
			if (pBrush->owner && pBrush->owner != world_entity)
			{			
	 			if (FilterBrush (pBrush))
	 				continue;

				// does this ent's model match the one we're checking for?...
				//
				char *psModelName = ValueForKey (pBrush->owner, sKEYFIELD_MODEL);
				if (strlen(psModelName))	// non-blank entry?
				{
					if (stricmp(psModelName, strModelUsedName.c_str()) == 0)
					{
						SetKeyValue(pBrush->owner, sKEYFIELD_GROUPNAME, psGroupName);
						bUpdated = true;
					}
				}
			}
		}
	}


	if (bUpdated )
	{
		Sys_MarkMapModified();
	}
}

		 

// couldn't find an existing function that did this, so...
//
// (used for smarter menu greying)
//
bool Grouping_AtLeastOneEntSelected(void)
{
	for (brush_t *pBrush = selected_brushes.next ; pBrush != &selected_brushes ; pBrush = pBrush->next)
	{
		if (pBrush->owner && pBrush->owner != world_entity)
		{
			return true;
		}
	}

	return false;
}


// query whether of the selected brushes there's at least one with a "model" key/value pair...
//
// (used for smarter menu greying)
//
bool Grouping_AtLeastOneModelEntSelected(void)
{
	for (brush_t *pBrush = selected_brushes.next ; pBrush != &selected_brushes ; pBrush = pBrush->next)
	{
		if (pBrush->owner && pBrush->owner != world_entity)
		{
			char *psModelName = ValueForKey (pBrush->owner, sKEYFIELD_MODEL);
			if (strlen(psModelName))	// non-blank entry?
			{
				return true;
			}
		}
	}

	return false;
}


#if 0
// some other stuff I wanted but couldn't be bothered making another file for...
//
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
#endif

////////////////// eof ////////////////


