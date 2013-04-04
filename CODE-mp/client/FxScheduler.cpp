#include "client.h"

#if !defined(FX_SCHEDULER_H_INC)
	#include "FxScheduler.h"
#endif

#ifdef EFFECTSED

	#include "IEffectsSystem.h"
	#include "CEffectSegment.h"
	#include "../PseudoEngineBackdoor.h"

#else	// !EFFECTSED

#if !defined(GHOUL2_SHARED_H_INC)
//	#include "..\game\ghoul2_shared.h"	//for CGhoul2Info_v
#endif

#if !defined(G2_H_INC)
	#include "../ghoul2/G2_local.h"
#endif

#if !defined(__Q_SHARED_H)
	#include "../game/q_shared.h"
#endif

#include <set>
extern set<CCloud *> OutstandClouds;


#endif	// EFFECTSED

CFxScheduler	theFxScheduler;

//-----------------------------------------------------------
void CMediaHandles::operator=(const CMediaHandles &that )
{
	mMediaList.clear();

	for ( int i = 0; i < that.mMediaList.size(); i++ )
	{
		mMediaList.push_back( that.mMediaList[i] );
	}
}

//------------------------------------------------------
CFxScheduler::CFxScheduler()
{ 
#ifdef EFFECTSED
	mbStopScheduled = false;
#endif

	memset( &mEffectTemplates, 0, sizeof( mEffectTemplates ));
}

//-----------------------------------------------------------
void SEffectTemplate::operator=(const SEffectTemplate &that)
{
	mCopy = true;

	strcpy( mEffectName, that.mEffectName );

	mPrimitiveCount = that.mPrimitiveCount;

	for( int i = 0; i < mPrimitiveCount; i++ )
	{
		mPrimitives[i] = new CPrimitiveTemplate;
		*(mPrimitives[i]) = *(that.mPrimitives[i]);
		// Mark use as a copy so that we know that we should be chucked when used up
		mPrimitives[i]->mCopy = true;
	}
}

//------------------------------------------------------
// Clean
//	Free up any memory we've allocated so we aren't leaking memory
//
// Input:
//	Whether to clean everything or just stop the playing (active) effects
//
// Return:
//	None
//
//------------------------------------------------------
void CFxScheduler::Clean(bool bRemoveTemplates /*= true*/, int idToPreserve /*= 0*/)
{
	int								i, j;
	TScheduledEffect::iterator		itr, next;

#ifdef EFFECTSED
	mbStopScheduled = false;
#endif

	// Ditch any scheduled effects
	itr = mFxSchedule.begin();

	while ( itr != mFxSchedule.end() )
	{
		next = itr;
		next++;

		if ((*itr)->mParent&&OutstandClouds.find((*itr)->mParent)!=OutstandClouds.end())
		{
			(*itr)->mParent->DecreasePending();
		}
		delete *itr;
		mFxSchedule.erase(itr);

		itr = next;
	}

	if (bRemoveTemplates)
	{
		// Ditch any effect templates
		for ( i = 1; i < FX_MAX_EFFECTS; i++ )
		{
			if ( i == idToPreserve)
			{
				continue;
			}

			if ( mEffectTemplates[i].mInUse )
			{
				// Ditch the primitives
				for (j = 0; j < mEffectTemplates[i].mPrimitiveCount; j++)
				{
					delete mEffectTemplates[i].mPrimitives[j];
				}
			} 

			mEffectTemplates[i].mInUse = false;
		}

		if (idToPreserve == 0)
		{
			mEffectIDs.clear();
		}
		else
		{
			// Clear the effect names, but first get the name of the effect to preserve,
			// and restore it after clearing.
			string str;
			TEffectID::iterator iter;

			for (iter = mEffectIDs.begin(); iter != mEffectIDs.end(); ++iter)
			{
				if ((*iter).second == idToPreserve)
				{
					str = (*iter).first;
					break;
				}
			}

			mEffectIDs.clear();

			mEffectIDs[str] = idToPreserve;
		}
	}
}

//------------------------------------------------------
// RegisterEffect
//	Attempt to open the specified effect file, if 
//	file read succeeds, parse the file.
//
// Input:
//	path or filename to open
//
// Return:
//	int handle to the effect
//------------------------------------------------------
int CFxScheduler::RegisterEffect( const char *file, bool bHasCorrectPath /*= false*/ )
{
	// Dealing with file names:
	// File names can come from two places - the editor, in which case we should use the given
	// path as is, and the effect file, in which case we should add the correct path and extension.
	// In either case we create a stripped file name to use for naming effects.
	//

	char sfile[MAX_QPATH];

	// kef -- frankly, I don't give a rat's butt whether I have the correct path or not. filenames that
	//come in here should start right after the base folder. they can have an extension or not...I don't
	//care because I'm going to strip it off automatically.

	// Get an extension stripped version of the file
	/*
	if (bHasCorrectPath)
	{
		const char *last = file, *p = file;

		while (*p != '\0')
		{
			if ((*p == '/') || (*p == '\\'))
			{
				last = p + 1;
			}

			p++;
		}

		COM_StripExtension( last, sfile );
	}
	else
	*/
	{
		COM_StripExtension( file, sfile );
		strlwr(sfile);
	}

	// see if the specified file is already registered.  If it is, just return the id of that file
	TEffectID::iterator itr;
	
	itr = mEffectIDs.find( sfile );

	if ( itr != mEffectIDs.end() )
	{
		return (*itr).second;
	}

	CGenericParser2	parser;
	int				len = 0;
	fileHandle_t	fh;
	char			data[65536];
	char			*bufParse = 0;

	// if our file doesn't have an extension, add one
	string finalFilename = file;
	string effectsSubstr = finalFilename.substr(0, 7);

	if (finalFilename.find('.') == string::npos)
	{
		// didn't find an extension so add one
		finalFilename += ".efx";
	}

	// kef - grr. this angers me. every filename everywhere should start from the base dir
	if (effectsSubstr.compare("effects") != 0)
	{
		//theFxHelper.Print("Hey!!! '%s' should be pathed from the base directory!!!\n", finalFilename.c_str());
		string strTemp = finalFilename;
		finalFilename = "effects/";
		finalFilename += strTemp;
	}

	len = theFxHelper.OpenFile( finalFilename.c_str(), &fh, FS_READ );

	/*
	if (bHasCorrectPath)
	{
		pfile = file;
	}
	else
	{
		// Add on our extension and prepend the file with the default path
		sprintf( temp, "%s/%s.efx", FX_FILE_PATH, sfile );
		pfile = temp;
	}
	len = theFxHelper.OpenFile( pfile, &fh, FS_READ );
	*/


	if ( len < 0 )
	{
		theFxHelper.Print( "Effect file load failed: %s\n", finalFilename.c_str() );
		return 0;
	}

	if (len == 0)
	{
		theFxHelper.Print( "INVALID Effect file: %s\n", finalFilename.c_str() );
		theFxHelper.CloseFile( fh );
		return 0;
	}

	// If we'll overflow our buffer, bail out--not a particularly elegant solution
	if (len >= sizeof(data) - 1 )
	{
		theFxHelper.CloseFile( fh );
		return 0;
	}

	// Get the goods and ensure Null termination
	theFxHelper.ReadFile( data, len, fh );
	data[len] = '\0';
	bufParse = data;

	// Let the generic parser process the whole file
	parser.Parse( &bufParse );

	theFxHelper.CloseFile( fh );

	// Lets convert the effect file into something that we can work with
	return ParseEffect( sfile, parser.GetBaseParseGroup() );
}


#ifdef EFFECTSED
//------------------------------------------------------
// GetEffectName
//	Lookup the name of the specified effect.
//
// Input:
//	int handle to the effect
//
// Return:
//	simplified (extension stripped) filename of the effect
//------------------------------------------------------
const char *CFxScheduler::GetEffectName(int id)
{
	for (TEffectID::iterator iter = mEffectIDs.begin(); iter != mEffectIDs.end(); ++iter)
	{
		if ((*iter).second == id)
		{
			return (*iter).first.c_str();
		}
	}

	return "(unknown effect)";
}

//------------------------------------------------------
// NewEffect
//	Create a new effect template containing no primitives.
//
// Input:
//	None
//
// Return:
//	int handle to the effect
//------------------------------------------------------
int CFxScheduler::NewEffect()
{
	static int	s_iEffectNum = 0;
	char		name[FX_MAX_PRIM_NAME];
	int			id;

	SEffectTemplate *effect;

	sprintf( name, "untitled editor effect %d", s_iEffectNum++ );

	effect = GetNewEffectTemplate( &id, name );

	if ( effect && id)
	{
		strcpy( effect->mEffectName, name );
	}

	return id;
}


//------------------------------------------------------
// RegisterEffectWithEditor
//	Informs the editing system of each primitive in the effect
//
// Input:
//	int handle to the effect
//
// Return:
//	None
//------------------------------------------------------
void CFxScheduler::RegisterEffectWithEditor(int id)
{
	SEffectTemplate				*fx;
	CPrimitiveTemplate			*prim;
	int							i;

	if ( id < 1 || id >= FX_MAX_EFFECTS || !mEffectTemplates[id].mInUse )
	{
		return;
	}

	// Get the effect.
	fx = &mEffectTemplates[id];

	// Loop through the primitives and register each bit
	for ( i = 0; i < fx->mPrimitiveCount; i++ )
	{
		prim = fx->mPrimitives[i];

		// Add the primitive to the editor
		CEffectSegment::RegisterPrimitiveTemplate(prim);
	}
}


//------------------------------------------------------
// NewPrimitive
//	Creates a new primitive template and adds it to the effect and
//	to the editor.  The new primitive will have the given type and
//	default parameters.
//
// Input:
//	int handle to the effect
//	type of the new primitive template
//
// Return:
//	None
//------------------------------------------------------
void CFxScheduler::NewPrimitive(int id, EPrimType type)
{
	CPrimitiveTemplate *prim;

	if ( id < 1 || id >= FX_MAX_EFFECTS || !mEffectTemplates[id].mInUse )
	{
		return;
	}

	prim = new CPrimitiveTemplate;

	prim->mType = type;

	// TODONOW - fill in default values for the template based on the type

	// Add our primitive template to the effect list
	AddPrimitiveToEffect( &mEffectTemplates[id], prim );

	// Add the primitive to the editor
	CEffectSegment::RegisterPrimitiveTemplate(prim);
}

//------------------------------------------------------
// DeletePrimitive
//	Removes the primitive from the template.  This function must be careful
//	when deleting templates that are referenced from scheduled effects
//
// Input:
//	int handle to the effect
//	pointer to the primitive template to delete
//
// Return:
//	None
//------------------------------------------------------
void CFxScheduler::DeletePrimitive(int id, CPrimitiveTemplate *prim)
{
	SEffectTemplate				*fx;
	int							i;

	if ( id < 1 || id >= FX_MAX_EFFECTS || !mEffectTemplates[id].mInUse )
	{
		return;
	}

	// Get the effect.
	fx = &mEffectTemplates[id];

	// Loop through the primitives looking for this one
	for ( i = 0; i < fx->mPrimitiveCount; i++ )
	{
		if (prim == fx->mPrimitives[i])
		{
			fx->mPrimitiveCount--;

			for ( ; i < fx->mPrimitiveCount; i++)
			{
				fx->mPrimitives[i] = fx->mPrimitives[i + 1];
			}

			break;
		}
	}

	// Look for any scheduled effects that reference this template, and delete them.

	TScheduledEffect::iterator	itr, next;

	itr = mFxSchedule.begin();

	while ( itr != mFxSchedule.end() )
	{
		next = itr;
		next++;

		if ( (*itr)->mpTemplate == prim )
		{
			// Get 'em out of there.
			if ((*itr)->mParent)
			{
				(*itr)->mParent->DecreasePending();
			}
			delete *itr;
			mFxSchedule.erase(itr);
		}

		itr = next;
	}

	delete prim;
}

void CFxScheduler::PrepareReloadAssets(int idToPreserve)
{
	// The effects system has been stopped, and all the assets are about to be unloaded.
	// For each CMediaHandles member in each of the main effect's (idToPreserve) templates,
	// memorize the names of the assets so we can reload them later.  Then throw away all
	// the effects and templates except the main effect.

	SEffectTemplate				*fx;
	CPrimitiveTemplate			*prim;
	int							i;

	if ( idToPreserve < 1 || idToPreserve >= FX_MAX_EFFECTS ||
		!mEffectTemplates[idToPreserve].mInUse )
	{
		Clean();
		return;
	}

	// Get the effect.
	fx = &mEffectTemplates[idToPreserve];

	// Loop through the primitives and memorize asset names
	for ( i = 0; i < fx->mPrimitiveCount; i++ )
	{
		prim = fx->mPrimitives[i];

		EMediaTypes mediaType = MEDIA_SHADERS;

		if (prim->mType == Emitter)
		{
			mediaType = MEDIA_MODELS;
		}
		else if (prim->mType == Sound)
		{
			mediaType = MEDIA_SOUNDS;
		}

		prim->mMediaHandles.PrepareReloadAssets(mediaType);
		prim->mImpactFxHandles.PrepareReloadAssets(MEDIA_EFFECTS);
		prim->mDeathFxHandles.PrepareReloadAssets(MEDIA_EFFECTS);
		prim->mEmitterFxHandles.PrepareReloadAssets(MEDIA_EFFECTS);
		prim->mPlayFxHandles.PrepareReloadAssets(MEDIA_EFFECTS);
	}

	// Now throw away everything except the main effect
	Clean(true, idToPreserve);
}

void CFxScheduler::ReloadAssets(int idToPreserve)
{
	// Reload all the assets whose names we memorized earlier.

	SEffectTemplate				*fx;
	CPrimitiveTemplate			*prim;
	int							i;

	if ( idToPreserve < 1 || idToPreserve >= FX_MAX_EFFECTS ||
		!mEffectTemplates[idToPreserve].mInUse )
	{
		return;
	}

	// Get the effect.
	fx = &mEffectTemplates[idToPreserve];

	// Loop through the primitives and reload assets
	for ( i = 0; i < fx->mPrimitiveCount; i++ )
	{
		prim = fx->mPrimitives[i];

		EMediaTypes mediaType = MEDIA_SHADERS;

		if (prim->mType == Emitter)
		{
			mediaType = MEDIA_MODELS;
		}
		else if (prim->mType == Sound)
		{
			mediaType = MEDIA_SOUNDS;
		}

		prim->mMediaHandles.ReloadAssets(mediaType);
		prim->mImpactFxHandles.ReloadAssets(MEDIA_EFFECTS);
		prim->mDeathFxHandles.ReloadAssets(MEDIA_EFFECTS);
		prim->mEmitterFxHandles.ReloadAssets(MEDIA_EFFECTS);
		prim->mPlayFxHandles.ReloadAssets(MEDIA_EFFECTS);
	}
}

void CMediaHandles::PrepareReloadAssets(enum EMediaTypes mediaType)
{
	mMediaNames.clear();

	for (int i = 0; i < mMediaList.size(); i++)
	{
		if (mediaType == MEDIA_EFFECTS)
		{
			mMediaNames.push_back(theFxScheduler.GetEffectName(mMediaList[i]));
		}
		else
		{
			mMediaNames.push_back(NameFromMediaHandle(mMediaList[i]));
		}
	}
}

void CMediaHandles::ReloadAssets(enum EMediaTypes mediaType)
{
	mMediaList.clear();

	for (int i = 0; i < mMediaNames.size(); i++)
	{
		int handle;

		if (mediaType == MEDIA_EFFECTS)
		{
			handle = theFxScheduler.RegisterEffect(mMediaNames[i].c_str());
		}
		else
		{
			handle = RegisterMediaItem(mMediaNames[i].c_str(), mediaType);
		}

		if (handle != 0)
		{
			mMediaList.push_back(handle);
		}
	}
}

#endif	// EFFECTSED


//------------------------------------------------------
// ParseEffect
//	Starts at ground zero, using each group header to 
//	determine which kind of effect we are working with.
//	Then we call the appropriate function to parse the
//	specified effect group.
//
// Input:
//	base group, essentially the whole files contents
//
// Return:
//	int handle of the effect
//------------------------------------------------------
int CFxScheduler::ParseEffect( const char *file, CGPGroup *base )
{
	CGPGroup			*primitiveGroup;
	CPrimitiveTemplate	*prim;
	const char			*grpName;
	SEffectTemplate		*effect = 0;
	EPrimType			type;
	int					handle;

	effect = GetNewEffectTemplate( &handle, file );

	if ( !handle || !effect )
	{
		// failure
		return 0;
	}

	primitiveGroup = base->GetSubGroups();

	while ( primitiveGroup )
	{
		grpName = primitiveGroup->GetName();

		// Huge stricmp lists suxor
		if ( !stricmp( grpName, "particle" ))
		{
			type = Particle;
		}
		else if ( !stricmp( grpName, "line" ))
		{
			type = Line;
		}
		else if ( !stricmp( grpName, "tail" ))
		{
			type = Tail;
		}
		else if ( !stricmp( grpName, "sound" ))
		{
			type = Sound;
		}
		else if ( !stricmp( grpName, "cylinder" ))
		{
			type = Cylinder;
		}
		else if ( !stricmp( grpName, "electricity" ))
		{
			type = Electricity;
		}
		else if ( !stricmp( grpName, "emitter" ))
		{
			type = Emitter;
		}
		else if ( !stricmp( grpName, "decal" ))
		{
			type = Decal;
		}
		else if ( !stricmp( grpName, "orientedparticle" ))
		{
			type = OrientedParticle;
		}
		else if ( !stricmp( grpName, "fxrunner" ))
		{
			type = FxRunner;
		}
		else if ( !stricmp( grpName, "light" ))
		{
			type = Light;
		}
		else if ( !stricmp( grpName, "cameraShake" ))
		{
			type = CameraShake;
		}
/*
		// NOTE:  Pat requested that flashes be disabled in MP.  Since fx files are shared with SP, this is the easiest way to accomplish that....
		//	 code will fall through and become type NONE....and therefore not parsed and added to the effect definition.
		else if ( !stricmp( grpName, "flash" ))
		{
			type = ScreenFlash;
		}
*/
		else 
		{
			type = None;
		}
		
		if ( type != None )
		{
			prim = new CPrimitiveTemplate;

			prim->mType = type;
			prim->ParsePrimitive( primitiveGroup );

			// Add our primitive template to the effect list
			AddPrimitiveToEffect( effect, prim );
		}

		primitiveGroup = (CGPGroup *)primitiveGroup->GetNext();
	}

	return handle;
}


//------------------------------------------------------
// AddPrimitiveToEffect
//	Takes a primitive and attaches it to the effect.
//	
// Input:
//	Effect template that we tack the primitive on to
//	Primitive to add to the effect template
//
// Return:
//	None
//------------------------------------------------------
void CFxScheduler::AddPrimitiveToEffect( SEffectTemplate *fx, CPrimitiveTemplate *prim )
{
	int ct = fx->mPrimitiveCount;

	if ( ct >= FX_MAX_EFFECT_COMPONENTS )
	{
		theFxHelper.Print( "FxScheduler:  Error--too many primitives in an effect\n" );
	}
	else
	{
		fx->mPrimitives[ct] = prim;
		fx->mPrimitiveCount++;
	}
}

//------------------------------------------------------
// GetNewEffectTemplate
//	Finds an unused effect template and returns it to the
//	caller.
//	
// Input:
//	pointer to an id that will be filled in, 
//	file name-- should be NULL when requesting a copy
//
// Return:
//	the id of the added effect template
//------------------------------------------------------
SEffectTemplate *CFxScheduler::GetNewEffectTemplate( int *id, const char *file )
{
	SEffectTemplate *effect;

	// wanted zero to be a bogus effect ID, so we just skip it.
	for ( int i = 1; i < FX_MAX_EFFECTS; i++ )
	{
		effect = &mEffectTemplates[i];

		if ( !effect->mInUse )
		{
			*id = i;
			memset( effect, 0, sizeof( SEffectTemplate ));

			// If we are a copy, we really won't have a name that we care about saving for later
			if ( file )
			{
				mEffectIDs[file] = i;
				strcpy( effect->mEffectName, file );
			}

			effect->mInUse = true;
			return effect;
		}
	}

	theFxHelper.Print( "FxScheduler:  Error--reached max effects\n" );
	*id = 0;
	return 0;
}

//------------------------------------------------------
// GetEffectCopy
//	Returns a copy of the desired effect so that it can
//	easily be modified run-time.
//	
// Input:
//	file-- the name of the effect file that you want a copy of
//	newHandle-- will actually be the returned handle to the new effect
//				you have to hold onto this if you intend to call it again
//
// Return:
//	the pointer to the copy
//------------------------------------------------------
SEffectTemplate *CFxScheduler::GetEffectCopy( const char *file, int *newHandle )
{
	return ( GetEffectCopy( mEffectIDs[file], newHandle ) );
}

//------------------------------------------------------
// GetEffectCopy
//	Returns a copy of the desired effect so that it can
//	easily be modified run-time.
//	
// Input:
//	fxHandle-- the handle to the effect that you want a copy of
//	newHandle-- will actually be the returned handle to the new effect
//				you have to hold onto this if you intend to call it again
//
// Return:
//	the pointer to the copy
//------------------------------------------------------
SEffectTemplate *CFxScheduler::GetEffectCopy( int fxHandle, int *newHandle )
{
	if ( fxHandle < 1 || fxHandle >= FX_MAX_EFFECTS || !mEffectTemplates[fxHandle].mInUse )
	{
		// Didn't even request a valid effect to copy!!!
		theFxHelper.Print( "FxScheduler: Bad effect file copy request\n" );

		*newHandle = 0;
		return 0;
	}

	// Copies shouldn't have names, otherwise they could trash our stl map used for getting ID from name
	SEffectTemplate *copy = GetNewEffectTemplate( newHandle, NULL );

	if ( copy && *newHandle )
	{
		// do the effect copy and mark us as what we are
		*copy = mEffectTemplates[fxHandle];
		copy->mCopy = true;

		// the user had better hold onto this handle if they ever hope to call this effect.
		return copy;
	}

	// No space left to return an effect
	*newHandle = 0;
	return 0;
}

//------------------------------------------------------
// GetPrimitiveCopy
//	Helper function that returns a copy of the desired primitive
//	
// Input:
//	fxHandle - the pointer to the effect copy you want to override
//	componentName - name of the component to find
//
// Return:
//	the pointer to the desired primitive
//------------------------------------------------------
CPrimitiveTemplate *CFxScheduler::GetPrimitiveCopy( SEffectTemplate *effectCopy, const char *componentName )
{
	if ( !effectCopy || !effectCopy->mInUse )
	{
		return NULL;
	}

	for ( int i = 0; i < effectCopy->mPrimitiveCount; i++ )
	{
		if ( !stricmp( effectCopy->mPrimitives[i]->mName, componentName ))
		{
			// we found a match, so return it
			return effectCopy->mPrimitives[i];
		}
	}

	// bah, no good.
	return NULL;
}

//------------------------------------------------------
static void ReportPlayEffectError(int id)
{
#ifdef EFFECTSED
	// report the error.  After we generate three of these messages in rapid succession,
	// kill the effect.
	static DWORD s_dwLastErrorTicks = 0;
	static int s_iErrors = 0;

	if (theFxScheduler.IsStopScheduled())
	{
		return;
	}
	else if ((GetTickCount() - s_dwLastErrorTicks) < 50)
	{
		if (++s_iErrors >= 3)
		{
			theFxScheduler.ScheduleStop();
			s_iErrors = 0;
			return;
		}
	}
	else
	{
		s_iErrors = 0;
	}
#endif

	theFxHelper.Print( "CFxScheduler::PlayEffect called with invalid effect ID: %i\n", id );

#ifdef EFFECTSED
	s_dwLastErrorTicks = GetTickCount();
#endif
}


//------------------------------------------------------
// PlayEffect
//	Handles scheduling an effect so all the components
//	happen at the specified time.  Applies a default up
//	axis.
//	
// Input:
//	Effect file id and the origin
//
// Return:
//	none
//------------------------------------------------------
void CFxScheduler::PlayEffect( int id, vec3_t origin )
{
	vec3_t	axis[3];

	VectorSet( axis[0], 0, 0, 1 );
	VectorSet( axis[1], 1, 0, 0 );
	VectorSet( axis[2], 0, 1, 0 );

	PlayEffect( id, origin, axis );
}

//------------------------------------------------------
// PlayEffect
//	Handles scheduling an effect so all the components
//	happen at the specified time.  Takes a fwd vector
//	and builds a right and up vector
//	
// Input:
//	Effect file id, the origin, and a fwd vector
//
// Return:
//	none
//------------------------------------------------------
void CFxScheduler::PlayEffect( int id, vec3_t origin, vec3_t forward )
{
	vec3_t	axis[3];

	// Cannot have a empty forward vector
	if ( !forward[0] && !forward[1] && !forward[2] )
	{		
		assert ( qfalse );
		return;
	}

	// Take the forward vector and create two arbitrary but perpendicular vectors
	VectorCopy( forward, axis[0] );
	MakeNormalVectors( forward, axis[1], axis[2] );

	PlayEffect( id, origin, axis );
}

#ifndef EFFECTSED
//------------------------------------------------------
// PlayEffect
//	Handles scheduling an effect so all the components
//	happen at the specified time.  Uses the specified axis
//	
// Input:
//	Effect file name, the origin, and axis.
//	Optional boltInfo (defaults to -1)
//  Optional entity number to be used by a cheap entity origin bolt (defaults to -1)
//
// Return:
//	none
//------------------------------------------------------
void CFxScheduler::PlayEffect( const char *file, vec3_t origin, vec3_t axis[3], const int boltInfo, const int entNum )
{
	char	sfile[MAX_QPATH];

	// Get an extenstion stripped version of the file
	COM_StripExtension( file, sfile );

	// This is a horribly dumb thing to have to do, but QuakeIII might not have calc'd the lerpOrigin
	//	for the entity we may be trying to bolt onto.  We like having the correct origin, so we are
	//	forced to call this function....
	if ( entNum != -1 )
	{
		VM_Call( cgvm, CG_CALC_LERP_POSITIONS, entNum);
	}

	PlayEffect( mEffectIDs[sfile], origin, axis, boltInfo, entNum );
}

int	totalPrimitives = 0;
int	totalEffects = 0;

//------------------------------------------------------
// PlayEffect
//	Handles scheduling an effect so all the components
//	happen at the specified time.  Uses the specified axis
//	
// Input:
//	Effect file name, the origin, and axis.
//	Optional boltInfo (defaults to -1)
//  Optional entity number to be used by a cheap entity origin bolt (defaults to -1)
//
// Return:
//	none
//------------------------------------------------------
extern cvar_t	*cl_autolodscale;
extern int gCLTotalClientNum;
void CFxScheduler::PlayEffect( int id, CFxBoltInterface *obj )
{
	SEffectTemplate			*fx;
	CPrimitiveTemplate		*prim;
	int						i = 0;
	int						count = 0, delay = 0;
	SScheduledEffect		*sfx;
	float					factor = 0.0f;
	bool					forceScheduling = false;
	CCloud					*effectCloud = 0;
	vec3_t store;

	vec3_t origin, forward;
	vec3_t axis[3];

	obj->GetForward(forward);

	VectorCopy( forward, axis[0] );
	MakeNormalVectors( forward, axis[1], axis[2] );

	if ( id < 1 || id >= FX_MAX_EFFECTS || !mEffectTemplates[id].mInUse )
	{
		// Now you've done it!
		ReportPlayEffectError(id);
		return;
	}

	// Don't bother scheduling the effect if the system is currently frozen
	if ( fx_freeze.integer )
	{
		return;
	}

#ifndef EFFECTSED
//	int						modelNum = 0, boltNum = 0;
//	int						entityNum = entNum;

/*rjr	if ( boltInfo > 0 )
	{
		// extract the wraith ID from the bolt info
		modelNum	= ( boltInfo >> MODEL_SHIFT )	& MODEL_AND;
		boltNum		= ( boltInfo >> BOLT_SHIFT )	& BOLT_AND;
		entityNum	= ( boltInfo >> ENTITY_SHIFT )	& ENTITY_AND;

		// We always force ghoul bolted objects to be scheduled so that they don't play right away.
		forceScheduling = true;
	} */
#endif

	// Get the effect.
	fx = &mEffectTemplates[id];

	effectCloud = FX_AddCloud();

	float cullRange, effectDistSq = DistanceSquared( origin, theFxHelper.refdef.vieworg );

	// Loop through the primitives and schedule each bit
	for ( i = 0; i < fx->mPrimitiveCount; i++ )
	{
		totalPrimitives++;
		prim = fx->mPrimitives[i];

#ifdef EFFECTSED
		if (!prim->mEnabled)
		{
			continue;
		}
#endif

		if ( prim->mCullRange )
		{
			cullRange = prim->mCullRange;
			if ( cl_autolodscale->integer )
			{
				if ( gCLTotalClientNum >= 8 )
				{
					cullRange *= 8.0f/gCLTotalClientNum;
				}
			}
			if ( effectDistSq > cullRange )  // cull range has already been squared
			{
				// is too far away, so don't add this primitive group
				continue;
			}
		}

		count = prim->mSpawnCount.GetRoundedVal();

		if ( prim->mCopy )
		{
			// If we are a copy, we need to store a "how many references count" so that we
			//	can keep the primitive template around for the correct amount of time.
			prim->mRefCount = count;
		}

		if ( prim->mSpawnFlags & FX_EVEN_DISTRIBUTION )
		{
			factor = abs(prim->mSpawnDelay.GetMax() - prim->mSpawnDelay.GetMin()) / (float)count;
		}

		// Schedule the random number of bits
		for ( int t = 0; t < count; t++ )
		{
			totalEffects++;
			if ( prim->mSpawnFlags & FX_EVEN_DISTRIBUTION )
			{
				delay = t * factor;
			}
			else
			{
				delay = prim->mSpawnDelay.GetVal();
			}

			// if the delay is so small, we may as well just create this bit right now
			if ( delay < 1 && !forceScheduling )
			{
#if 0//#ifndef EFFECTSED
				if ( boltInfo == -1 && entNum != -1 )
				{
					// Find out where the entity currently is
					vec3_t	lerpOrigin;

					VM_Call( cgvm, CG_GET_LERP_ORIGIN, entNum, lerpOrigin);
					CreateEffect( prim, lerpOrigin, axis, -delay, effectCloud );
				}
				else
#endif
				{
					if (effectCloud)
					{
						CFxBoltInterface boltobj;

						boltobj.SetEntNum(obj->GetEntNum());
						boltobj.SetBoltNum(obj->GetBoltNum());
						boltobj.SetModelNum(obj->GetModelNum());
						boltobj.SetG2Handle(obj->GetG2Handle());

						obj->GetForward(store);
						boltobj.SetForward(store);
						obj->GetOrigin(store);
						boltobj.SetOrigin(store);
						obj->GetScale(store);
						boltobj.SetScale(store);
						obj->GetInitialOffset(store);
						boltobj.SetInitialOffset(store);
						obj->GetInitialPartOrg(store);
						boltobj.SetInitialPartOrg(store);

						effectCloud->SetBoltInterface(&boltobj);
					}
					CreateEffect( prim, origin, axis, -delay, effectCloud );
				}
			}
			else
			{
				// We have to create a new scheduled effect so that we can create it at a later point
				//	you should avoid this because it's much more expensive
				sfx = new SScheduledEffect;

				if (effectCloud)
				{
					CFxBoltInterface boltobj;

					boltobj.SetEntNum(obj->GetEntNum());
					boltobj.SetBoltNum(obj->GetBoltNum());
					boltobj.SetModelNum(obj->GetModelNum());
					boltobj.SetG2Handle(obj->GetG2Handle());

					obj->GetForward(store);
					boltobj.SetForward(store);
					obj->GetOrigin(store);
					boltobj.SetOrigin(store);
					obj->GetScale(store);
					boltobj.SetScale(store);
					obj->GetInitialOffset(store);
					boltobj.SetInitialOffset(store);
					obj->GetInitialPartOrg(store);
					boltobj.SetInitialPartOrg(store);

					effectCloud->SetBoltInterface(&boltobj);
				}

				sfx->mParent = effectCloud;
				sfx->mStartTime = theFxHelper.mTime + delay;
				sfx->mpTemplate = prim;
				sfx->mObj = 0;

#if 0//#ifndef EFFECTSED#ifndef EFFECTSED
				if ( boltInfo == -1 )
				{
					if ( entNum == -1 )
					{
						// we aren't bolting, so make sure the spawn system knows this by putting -1's in these fields
						sfx->mBoltNum = -1;
						sfx->mEntNum = -1;
						sfx->mModelNum = 0;
#else
						sfx->mBoltNum = -1;
						sfx->mEntNum = -1;
						sfx->mModelNum = 0;
#endif
						if ( origin )
						{
							VectorCopy( origin, sfx->mOrigin );
						}
						else
						{
							VectorClear( sfx->mOrigin );
						}

						AxisCopy( axis, sfx->mAxis );
#if 0//#ifndef EFFECTSED#ifndef EFFECTSED
					}
					else
					{
						// we are doing bolting onto the origin of the entity, so use a cheaper method
						sfx->mBoltNum = -1;
						sfx->mEntNum = entityNum;
						sfx->mModelNum = 0;

						AxisCopy( axis, sfx->mAxis );
					}
				}
				else
				{
					// we are bolting, so store the extra info
					sfx->mBoltNum = boltNum;
					sfx->mEntNum = entityNum;
					sfx->mModelNum = modelNum;

					// Also, the ghoul bolt may not be around yet, so delay the creation one frame
					sfx->mStartTime++;
				}
#endif

				if (effectCloud)
				{
					effectCloud->IncreasePending();
				}
				mFxSchedule.push_front( sfx );
			}
		}
	}

	// We track effect templates and primitive templates separately.
	if ( fx->mCopy )
	{
		// We don't use dynamic memory allocation, so just mark us as dead
		fx->mInUse = false;
	}
}

//------------------------------------------------------
// CreateEffect
//	Creates the specified fx taking into account the
//	multitude of different ways it could be spawned.
//	
// Input:
//	template used to build the effect, desired effect origin, 
//	desired orientation and how late the effect is so that
//	it can be moved to the correct spot
//
// Return:
//	none
//------------------------------------------------------
void CFxScheduler::CreateEffect( CPrimitiveTemplate *fx, CFxBoltInterface *obj, int delay, CCloud *effectCloud)
{
	vec3_t	org, temp, vel,
				accel, sRGB,
				eRGB, ang,
				ax[3], axis[3],
				origin, objOrg,
				objAng, objScale;

	CGhoul2Info_v *g2Handle;
	mdxaBone_t boltMatrix;

	int lateTime = 0;

	g2Handle = obj->GetG2Handle();

	if (!g2Handle || !G2API_HaveWeGhoul2Models(*((CGhoul2Info_v *)g2Handle)))
	{
		return;
	}

	obj->GetOrigin(objOrg);
	obj->GetForward(objAng);
	obj->GetScale(objScale);

	G2API_GetBoltMatrix(*((CGhoul2Info_v *)g2Handle), obj->GetModelNum(), obj->GetBoltNum(), &boltMatrix, objAng, objOrg, theFxHelper.mTime, /*MODELLIST*/NULL, objScale);

	G2API_GiveMeVectorFromMatrix(&boltMatrix, ORIGIN, origin);
	G2API_GiveMeVectorFromMatrix(&boltMatrix, NEGATIVE_Y, ang);

	axis[0][0] = boltMatrix.matrix[0][0];
	axis[0][1] = boltMatrix.matrix[1][0];
	axis[0][2] = boltMatrix.matrix[2][0];

	axis[1][0] = boltMatrix.matrix[0][1];
	axis[1][1] = boltMatrix.matrix[1][1];
	axis[1][2] = boltMatrix.matrix[2][1];

	axis[2][0] = boltMatrix.matrix[0][2];
	axis[2][1] = boltMatrix.matrix[1][2];
	axis[2][2] = boltMatrix.matrix[2][2];

	// We may modify the axis, so make a work copy
	AxisCopy( axis, ax );

	if( fx->mSpawnFlags & FX_RAND_ROT_AROUND_FWD )
	{
		RotatePointAroundVector( ax[1], ax[0], axis[1], random()*360.0f );
		CrossProduct( ax[0], ax[1], ax[2] );
	}

	// Origin calculations
	//-------------------------------------
	if ( fx->mSpawnFlags & FX_CHEAP_ORG_CALC )
	{ // let's take the easy way out
		VectorSet( org, fx->mOrigin1X.GetVal(), fx->mOrigin1Y.GetVal(), fx->mOrigin1Z.GetVal() );
	}
	else
	{ // time for some extra work
		VectorScale( ax[0], fx->mOrigin1X.GetVal(), org );
		VectorMA( org, fx->mOrigin1Y.GetVal(), ax[1], org );
		VectorMA( org, fx->mOrigin1Z.GetVal(), ax[2], org );
	//	VectorCopy(origin, org);
	}

	// We always add our calculated offset to the passed in origin...
	VectorAdd( org, origin, org );

	// Now, we may need to calc a point on a sphere/ellipsoid/cylinder/disk and add that to it
	//----------------------------------------------------------------
	if ( fx->mSpawnFlags & FX_ORG_ON_SPHERE )
	{
		float x, y;
		float width, height;
		
		x = DEG2RAD( random() * 360.0f );
		y = DEG2RAD( random() * 180.0f );

		width = fx->mRadius.GetVal();
		height = fx->mHeight.GetVal();

		// calculate point on ellipse
		VectorSet( temp, sin(x) * width * sin(y), cos(x) * width * sin(y), cos(y) * height ); // sinx * siny, cosx * siny, cosy
		VectorAdd( org, temp, org );

		if ( fx->mSpawnFlags & FX_AXIS_FROM_SPHERE )
		{ 
			// well, we will now override the axis at the users request
			VectorNormalize2( temp, ax[0] );
			MakeNormalVectors( ax[0], ax[1], ax[2] );
		}
	}
	else if ( fx->mSpawnFlags & FX_ORG_ON_CYLINDER )
	{
		vec3_t	pt;

		// set up our point, then rotate around the current direction to.  Make unrotated cylinder centered around 0,0,0
		VectorScale( ax[1], fx->mRadius.GetVal(), pt );
		VectorMA( pt, crandom() * 0.5f * fx->mHeight.GetVal(), ax[0], pt );
		RotatePointAroundVector( temp, ax[0], pt, random() * 360.0f );

		VectorAdd( org, temp, org );

		if ( fx->mSpawnFlags & FX_AXIS_FROM_SPHERE )
		{ 
			vec3_t up={0,0,1};

			// well, we will now override the axis at the users request
			VectorNormalize2( temp, ax[0] );

			if ( ax[0][2] == 1.0f )
			{
				// readjust up
				VectorSet( up, 0, 1, 0 );
			}

			CrossProduct( up, ax[0], ax[1] );
			CrossProduct( ax[0], ax[1], ax[2] );
		}
	}

	// There are only a few types that really use velocity and acceleration, so do extra work for those types
	//--------------------------------------------------------------------------------------------------------
	if ( fx->mType == Particle || fx->mType == OrientedParticle || fx->mType == Tail || fx->mType == Emitter )
	{
		// Velocity calculations
		//-------------------------------------
		if ( fx->mSpawnFlags & FX_VEL_IS_ABSOLUTE )
		{  
			VectorSet( vel, fx->mVelX.GetVal(), fx->mVelY.GetVal(), fx->mVelZ.GetVal() );
		}
		else
		{ // bah, do some extra work to coerce it
			VectorScale( ax[0], fx->mVelX.GetVal(), vel );
			VectorMA( vel, fx->mVelY.GetVal(), ax[1], vel );
			VectorMA( vel, fx->mVelZ.GetVal(), ax[2], vel );
		}

		//-------------------------------------
		if ( fx->mSpawnFlags & FX_AFFECTED_BY_WIND )
		{
			// wind is affecting us, so modify our initial velocity.  ideally, we would update throughout our lives, but this is easier
			VectorMA( vel, fx->mWindModifier.GetVal() * 0.01f, cl_windVec, vel );
		}

		// Acceleration calculations
		//-------------------------------------
		if ( fx->mSpawnFlags & FX_ACCEL_IS_ABSOLUTE )
		{ 
			VectorSet( accel, fx->mAccelX.GetVal(), fx->mAccelY.GetVal(), fx->mAccelZ.GetVal() );
		}
		else
		{
			VectorScale( ax[0], fx->mAccelX.GetVal(), accel );
			VectorMA( accel, fx->mAccelY.GetVal(), ax[1], accel );
			VectorMA( accel, fx->mAccelZ.GetVal(), ax[2], accel );
		}

		// Gravity is completely decoupled from acceleration since it is __always__ absolute
		// NOTE: I only effect Z ( up/down in the Quake world )
		accel[2] += fx->mGravity.GetVal();
	
		// There may be a lag between when the effect should be created and when it actually gets created.  
		//	Since we know what the discrepancy is, we can attempt to compensate...
		if ( lateTime > 0 )
		{
			// Calc the time differences
			float ftime = lateTime * 0.001f;
			float time2 = ftime * ftime * 0.5f;

			VectorMA( vel, ftime, accel, vel );

			// Predict the new position
			for ( int i = 0 ; i < 3 ; i++ ) 
			{
				org[i] = org[i] + ftime * vel[i] + time2 * vel[i];
			}
		}
	} // end moving types

	// handle RGB color, but only for types that will use it
	//---------------------------------------------------------------------------
	if ( fx->mType != Sound && fx->mType != FxRunner && fx->mType != CameraShake )
	{
		if ( fx->mSpawnFlags & FX_RGB_COMPONENT_INTERP )
		{
			float perc = random();

			VectorSet( sRGB, fx->mRedStart.GetVal( perc ), fx->mGreenStart.GetVal( perc ), fx->mBlueStart.GetVal( perc ) );
			VectorSet( eRGB, fx->mRedEnd.GetVal( perc ), fx->mGreenEnd.GetVal( perc ), fx->mBlueEnd.GetVal( perc ) );
		}
		else
		{
			VectorSet( sRGB, fx->mRedStart.GetVal(), fx->mGreenStart.GetVal(), fx->mBlueStart.GetVal() );
			VectorSet( eRGB, fx->mRedEnd.GetVal(), fx->mGreenEnd.GetVal(), fx->mBlueEnd.GetVal() );
		}
	}

	// We only support particles for now
	//------------------------
	switch( fx->mType )
	{
	//---------
	case Particle:
	//---------
		{
			vec3_t offset;

			VectorSubtract(org, origin, offset);

			obj->SetInitialOffset(offset);

			obj->SetInitialPartOrg(org);

			FX_AddParticle( effectCloud, obj, vel, accel, fx->mSizeStart.GetVal(), fx->mSizeEnd.GetVal(), fx->mSizeParm.GetVal(),
					fx->mAlphaStart.GetVal(), fx->mAlphaEnd.GetVal(), fx->mAlphaParm.GetVal(),
					sRGB, eRGB, fx->mRGBParm.GetVal(),
					fx->mRotation.GetVal(), fx->mRotationDelta.GetVal(),
					fx->mLife.GetVal(), fx->mMediaHandles.GetHandle(), fx->mFlags | FX_RELATIVE, true );
		break;
		}
	default:
		break;
	}

	// Track when we need to clean ourselves up if we are a copy
	if ( fx->mCopy )
	{
		fx->mRefCount--;

		if ( fx->mRefCount <= 0 )
		{
			delete fx;
		}
	}
}

#endif

//------------------------------------------------------
// PlayEffect
//	Handles scheduling an effect so all the components
//	happen at the specified time.  Uses the specified axis
//	
// Input:
//	Effect id, the origin, and axis.
//	Optional boltInfo (defaults to -1)
//  Optional entity number to be used by a cheap entity origin bolt (defaults to -1)
//
// Return:
//	none
//------------------------------------------------------
void CFxScheduler::PlayEffect( int id, vec3_t origin, vec3_t axis[3], const int boltInfo, const int entNum )
{
	SEffectTemplate			*fx;
	CPrimitiveTemplate		*prim;
	int						i = 0;
	int						count = 0, delay = 0;
	SScheduledEffect		*sfx;
	float					factor = 0.0f;
	bool					forceScheduling = false;
	CCloud					*effectCloud = 0;

	if ( id < 1 || id >= FX_MAX_EFFECTS || !mEffectTemplates[id].mInUse )
	{
		// Now you've done it!
		ReportPlayEffectError(id);
		return;
	}

	// Don't bother scheduling the effect if the system is currently frozen
	if ( fx_freeze.integer )
	{
		return;
	}

#ifndef EFFECTSED
	int						modelNum = 0, boltNum = 0;
	int						entityNum = entNum;

/*rjr	if ( boltInfo > 0 )
	{
		// extract the wraith ID from the bolt info
		modelNum	= ( boltInfo >> MODEL_SHIFT )	& MODEL_AND;
		boltNum		= ( boltInfo >> BOLT_SHIFT )	& BOLT_AND;
		entityNum	= ( boltInfo >> ENTITY_SHIFT )	& ENTITY_AND;

		// We always force ghoul bolted objects to be scheduled so that they don't play right away.
		forceScheduling = true;
	} */
#endif

	// Get the effect.
	fx = &mEffectTemplates[id];

	effectCloud = FX_AddCloud();

	float cullRange, effectDistSq = DistanceSquared( origin, theFxHelper.refdef.vieworg );

	// Loop through the primitives and schedule each bit
	for ( i = 0; i < fx->mPrimitiveCount; i++ )
	{
		totalPrimitives++;
		prim = fx->mPrimitives[i];

#ifdef EFFECTSED
		if (!prim->mEnabled)
		{
			continue;
		}
#endif

		if ( prim->mCullRange )
		{
			cullRange = prim->mCullRange;
			if ( cl_autolodscale->integer )
			{
				if ( gCLTotalClientNum >= 8 )
				{
					cullRange *= 8.0f/gCLTotalClientNum;
				}
			}
			if ( effectDistSq > cullRange )  // cull range has already been squared
			{
				// is too far away, so don't add this primitive group
				continue;
			}
		}

		count = prim->mSpawnCount.GetRoundedVal();

		if ( prim->mCopy )
		{
			// If we are a copy, we need to store a "how many references count" so that we
			//	can keep the primitive template around for the correct amount of time.
			prim->mRefCount = count;
		}

		if ( prim->mSpawnFlags & FX_EVEN_DISTRIBUTION )
		{
			factor = abs(prim->mSpawnDelay.GetMax() - prim->mSpawnDelay.GetMin()) / (float)count;
		}

		// Schedule the random number of bits
		for ( int t = 0; t < count; t++ )
		{
			totalEffects++;
			if ( prim->mSpawnFlags & FX_EVEN_DISTRIBUTION )
			{
				delay = t * factor;
			}
			else
			{
				delay = prim->mSpawnDelay.GetVal();
			}

			// if the delay is so small, we may as well just create this bit right now
			if ( delay < 1 && !forceScheduling )
			{
#ifndef EFFECTSED
				if ( boltInfo == -1 && entNum != -1 )
				{
					// Find out where the entity currently is
					vec3_t	lerpOrigin;

//					VM_Call( cgvm, CG_GET_LERP_ORIGIN, entNum, lerpOrigin);
					TCGVectorData	*data = (TCGVectorData*)cl.mSharedMemory;
					data->mEntityNum = entNum;
					VM_Call( cgvm, CG_GET_LERP_ORIGIN );
					VectorCopy(data->mPoint, lerpOrigin);

					CreateEffect( prim, lerpOrigin, axis, -delay, effectCloud );
				}
				else
#endif
				{
					CreateEffect( prim, origin, axis, -delay, effectCloud );
				}
			}
			else
			{
				// We have to create a new scheduled effect so that we can create it at a later point
				//	you should avoid this because it's much more expensive
				sfx = new SScheduledEffect;
				sfx->mParent = effectCloud;
				sfx->mStartTime = theFxHelper.mTime + delay;
				sfx->mpTemplate = prim;
				sfx->mObj = 0;

#ifndef EFFECTSED
				if ( boltInfo == -1 )
				{
					if ( entNum == -1 )
					{
						// we aren't bolting, so make sure the spawn system knows this by putting -1's in these fields
						sfx->mBoltNum = -1;
						sfx->mEntNum = -1;
						sfx->mModelNum = 0;
#endif

						if ( origin )
						{
							VectorCopy( origin, sfx->mOrigin );
						}
						else
						{
							VectorClear( sfx->mOrigin );
						}

						AxisCopy( axis, sfx->mAxis );
#ifndef EFFECTSED
					}
					else
					{
						// we are doing bolting onto the origin of the entity, so use a cheaper method
						sfx->mBoltNum = -1;
						sfx->mEntNum = entityNum;
						sfx->mModelNum = 0;

						AxisCopy( axis, sfx->mAxis );
					}
				}
				else
				{
					// we are bolting, so store the extra info
					sfx->mBoltNum = boltNum;
					sfx->mEntNum = entityNum;
					sfx->mModelNum = modelNum;

					// Also, the ghoul bolt may not be around yet, so delay the creation one frame
					sfx->mStartTime++;
				}
#endif

				if (effectCloud)
				{
					effectCloud->IncreasePending();
				}
				mFxSchedule.push_front( sfx );
			}
		}
	}

	// We track effect templates and primitive templates separately.
	if ( fx->mCopy )
	{
		// We don't use dynamic memory allocation, so just mark us as dead
		fx->mInUse = false;
	}
}

//------------------------------------------------------
// PlayEffect
//	Handles scheduling an effect so all the components
//	happen at the specified time.  Applies a default up
//	axis.
//	
// Input:
//	Effect file name and the origin
//
// Return:
//	none
//------------------------------------------------------
void CFxScheduler::PlayEffect( const char *file, vec3_t origin )
{
	char	sfile[MAX_QPATH];

	// Get an extenstion stripped version of the file
	COM_StripExtension( file, sfile );

	PlayEffect( mEffectIDs[sfile], origin );
}

//------------------------------------------------------
// PlayEffect
//	Handles scheduling an effect so all the components
//	happen at the specified time.  Takes a forward vector
//	and uses this to complete the axis field.
//	
// Input:
//	Effect file name, the origin, and a forward vector
//
// Return:
//	none
//------------------------------------------------------
void CFxScheduler::PlayEffect( const char *file, vec3_t origin, vec3_t forward )
{
	char	sfile[MAX_QPATH];

	// Get an extenstion stripped version of the file
	COM_StripExtension( file, sfile );

	PlayEffect( mEffectIDs[sfile], origin, forward );
}

//------------------------------------------------------
// AddScheduledEffects
//	Handles determining if a scheduled effect should
//	be created or not.  If it should it handles converting
//	the template effect into a real one.
//	
// Input:
//	none
//
// Return:
//	none
//------------------------------------------------------

#if _JK2
#define CHC

void CFxScheduler::AddScheduledEffects( void )
{
	TScheduledEffect::iterator	itr, next;
	SScheduledEffect			*schedEffect = 0;

	itr = mFxSchedule.begin();

	while ( itr != mFxSchedule.end() )
	{
		next = itr;
		next++;
		schedEffect = (*itr);
		if ( *(*itr) <= theFxHelper.mTime )
		{
			if ((*itr)->mParent && OutstandClouds.find((*itr)->mParent)!=OutstandClouds.end())
			{
				// ok, are we spawning a bolt on effect or a normal one?
				if ( (*itr)->mEntNum != -1 )
				{
					// Find out where the entity currently is
					vec3_t	lerpOrigin;

//					VM_Call( cgvm, CG_GET_LERP_ORIGIN, (*itr)->mEntNum, lerpOrigin);
					TCGVectorData	*data = (TCGVectorData*)cl.mSharedMemory;
					data->mEntityNum = (*itr)->mEntNum;
					VM_Call( cgvm, CG_GET_LERP_ORIGIN );
					VectorCopy(data->mPoint, lerpOrigin);

					CreateEffect( (*itr)->mpTemplate, 
								lerpOrigin, (*itr)->mAxis, 
								theFxHelper.mTime - (*itr)->mStartTime, (*itr)->mParent );
				}
				else
				{
					CreateEffect( (*itr)->mpTemplate, 
								(*itr)->mOrigin, (*itr)->mAxis, 
								theFxHelper.mTime - (*itr)->mStartTime, (*itr)->mParent );
				}
				// Get 'em out of there.
				if ((*itr)->mParent&&OutstandClouds.find((*itr)->mParent)!=OutstandClouds.end())
				{
					(*itr)->mParent->DecreasePending();
				}
			}
			delete *itr;
			mFxSchedule.erase(itr); 
		}

		itr = next;
	}
	// Add all active effects into the scene
	FX_Add();
}

#else

void CFxScheduler::AddScheduledEffects( void )
{

	TScheduledEffect::iterator	itr, next;
	SScheduledEffect			*schedEffect = 0;
#ifndef EFFECTSED
	vec3_t						origin;
	vec3_t						axis[3];
	int							oldEntNum = -1, oldBoltIndex = -1, oldModelNum = -1;
	qboolean					doesBoltExist  = qfalse;
#endif

	itr = mFxSchedule.begin();

	while ( itr != mFxSchedule.end() )
	{
		next = itr;
		next++;
		schedEffect = (*itr);
		if ( *(*itr) <= theFxHelper.mTime )
		{
#ifndef EFFECTSED
#ifndef CHC
//			if ( (*itr)->mpTemplate->mFlags & FX_RELATIVE )
			if ( (*itr)->mObj && (*itr)->mObj->IsValid() == true &&
				((*itr)->mpTemplate->mFlags & FX_RELATIVE))
			{
				CreateEffect( (*itr)->mpTemplate, (*itr)->mObj, 
								theFxHelper.mTime - (*itr)->mStartTime, (*itr)->mParent );
			}
			else
#endif // CHC
			/*if ((*itr)->mBoltNum == -1)*/
			if (1)
			{// ok, are we spawning a bolt on effect or a normal one?
				if ( (*itr)->mEntNum != -1 )
				{
					// Find out where the entity currently is
					vec3_t	lerpOrigin;

//					VM_Call( cgvm, CG_GET_LERP_ORIGIN, (*itr)->mEntNum, lerpOrigin);
					TCGVectorData	*data = (TCGVectorData*)cl.mSharedMemory;
					data->mEntityNum = (*itr)->mEntNum;
					VM_Call( cgvm, CG_GET_LERP_ORIGIN );
					VectorCopy(data->mPoint, lerpOrigin);

					CreateEffect( (*itr)->mpTemplate, 
								lerpOrigin, (*itr)->mAxis, 
								theFxHelper.mTime - (*itr)->mStartTime, (*itr)->mParent );
				}
				else
				{
#endif // EFFECTSED
					CreateEffect( (*itr)->mpTemplate, 
								(*itr)->mOrigin, (*itr)->mAxis, 
								theFxHelper.mTime - (*itr)->mStartTime, (*itr)->mParent );
#ifndef EFFECTSED
				}
			}
			else
			{
				// do we need to go and re-get the bolt matrix again? Since it takes time lets try and do it only once
				if (((*itr)->mModelNum != oldModelNum) || 
					((*itr)->mEntNum != oldEntNum) || 
					((*itr)->mBoltNum != oldBoltIndex))
				{
					mdxaBone_t 		boltMatrix;

					oldModelNum = (*itr)->mModelNum;
					oldEntNum = (*itr)->mEntNum;
					oldBoltIndex = (*itr)->mBoltNum;
					vec3_t	lerpOrigin, lerpAngles, modelScale;

//					VM_Call( cgvm, CG_GET_LERP_ORIGIN, (*itr)->mEntNum, lerpOrigin);
//					VM_Call( cgvm, CG_GET_LERP_ANGLES, (*itr)->mEntNum, lerpAngles);
//					VM_Call( cgvm, CG_GET_MODEL_SCALE, (*itr)->mEntNum, modelScale);

					TCGVectorData	*data = (TCGVectorData*)cl.mSharedMemory;
					data->mEntityNum = (*itr)->mEntNum;

					VM_Call( cgvm, CG_GET_LERP_ORIGIN );
					VectorCopy(data->mPoint, lerpOrigin);
					VM_Call( cgvm, CG_GET_LERP_ANGLES );
					VectorCopy(data->mPoint, lerpAngles);
					VM_Call( cgvm, CG_GET_MODEL_SCALE );
					VectorCopy(data->mPoint, modelScale);

					// go away and get me the bolt position for this frame please
					doesBoltExist = G2API_GetBoltMatrix(*((CGhoul2Info_v *)VM_Call( cgvm, CG_GET_GHOUL2, (*itr)->mEntNum)), (*itr)->mModelNum, (*itr)->mBoltNum, &boltMatrix, lerpAngles, lerpOrigin, cls.realtime, (int *)VM_Call( cgvm, CG_GET_MODEL_LIST, (*itr)->mEntNum), modelScale);

					if (doesBoltExist)
					{	// set up the axis and origin we need for the actual effect spawning
	   					origin[0] = boltMatrix.matrix[0][3];
						origin[1] = boltMatrix.matrix[1][3];
						origin[2] = boltMatrix.matrix[2][3];

						axis[0][0] = boltMatrix.matrix[0][0];
						axis[0][1] = boltMatrix.matrix[1][0];
						axis[0][2] = boltMatrix.matrix[2][0];

						axis[1][0] = boltMatrix.matrix[0][1];
						axis[1][1] = boltMatrix.matrix[1][1];
						axis[1][2] = boltMatrix.matrix[2][1];

						axis[2][0] = boltMatrix.matrix[0][2];
						axis[2][1] = boltMatrix.matrix[1][2];
						axis[2][2] = boltMatrix.matrix[2][2];
					}
				}

				// only do this if we found the bolt
				if (doesBoltExist)
				{
					CreateEffect( (*itr)->mpTemplate, 
									origin, axis, 
									theFxHelper.mTime - (*itr)->mStartTime, (*itr)->mParent );
				} 
			}
#endif

			// Get 'em out of there.
			if ((*itr)->mParent)
			{
				(*itr)->mParent->DecreasePending();
			}
			delete *itr;
			mFxSchedule.erase(itr); 
		}

		itr = next;
	}

#ifdef EFFECTSED
	// To dissuade designers from overloading the effects system, code in FxUtil.cpp
	// will schedule a stop of all the active and scheduled effects if we run out of
	// effects slots too rapidly.  If this occurs, call Stop.
	if (IsStopScheduled())
	{
		EffectsSystem().Stop();
	}
#endif

	// Add all active effects into the scene
	FX_Add();
}

#endif // #if1

//------------------------------------------------------
// CreateEffect
//	Creates the specified fx taking into account the
//	multitude of different ways it could be spawned.
//	
// Input:
//	template used to build the effect, desired effect origin, 
//	desired orientation and how late the effect is so that
//	it can be moved to the correct spot
//
// Return:
//	none
//------------------------------------------------------
void CFxScheduler::CreateEffect( CPrimitiveTemplate *fx, vec3_t origin, vec3_t axis[3], int lateTime, CCloud *effectCloud )
{
	vec3_t	org, org2, temp,
				vel, accel,
				sRGB, eRGB,
				ang, angDelta,
				ax[3], origin_certain;
	trace_t	tr;
	int		emitterModel;
	CFxBoltInterface *fxBoltInterface = NULL;
	VectorCopy(origin, origin_certain);

	if (effectCloud->IsBoltInterfaceValid())
	{ //we were created by the special effect-bolting function, which means we also have bolt data
		fxBoltInterface = effectCloud->GetBIPointer();

		if (fxBoltInterface)
		{
			fxBoltInterface->GetOrigin(origin_certain);
		}
	}

	// We may modify the axis, so make a work copy
	AxisCopy( axis, ax );

	if( fx->mSpawnFlags & FX_RAND_ROT_AROUND_FWD )
	{
		RotatePointAroundVector( ax[1], ax[0], axis[1], random()*360.0f );
		CrossProduct( ax[0], ax[1], ax[2] );
	}

	// Origin calculations
	//-------------------------------------
	if ( fx->mSpawnFlags & FX_CHEAP_ORG_CALC )
	{ // let's take the easy way out
		VectorSet( org, fx->mOrigin1X.GetVal(), fx->mOrigin1Y.GetVal(), fx->mOrigin1Z.GetVal() );
	}
	else
	{ // time for some extra work
		VectorScale( ax[0], fx->mOrigin1X.GetVal(), org );
		VectorMA( org, fx->mOrigin1Y.GetVal(), ax[1], org );
		VectorMA( org, fx->mOrigin1Z.GetVal(), ax[2], org );
	}

	// We always add our calculated offset to the passed in origin...
	VectorAdd( org, origin_certain, org );

	// Now, we may need to calc a point on a sphere/ellipsoid/cylinder/disk and add that to it
	//----------------------------------------------------------------
	if ( fx->mSpawnFlags & FX_ORG_ON_SPHERE )
	{
		float x, y;
		float width, height;
		
		x = DEG2RAD( random() * 360.0f );
		y = DEG2RAD( random() * 180.0f );

		width = fx->mRadius.GetVal();
		height = fx->mHeight.GetVal();

		// calculate point on ellipse
		VectorSet( temp, sin(x) * width * sin(y), cos(x) * width * sin(y), cos(y) * height ); // sinx * siny, cosx * siny, cosy
		VectorAdd( org, temp, org );

		if ( fx->mSpawnFlags & FX_AXIS_FROM_SPHERE )
		{ 
			// well, we will now override the axis at the users request
			VectorNormalize2( temp, ax[0] );
			MakeNormalVectors( ax[0], ax[1], ax[2] );
		}
	}
	else if ( fx->mSpawnFlags & FX_ORG_ON_CYLINDER )
	{
		vec3_t	pt;

		// set up our point, then rotate around the current direction to.  Make unrotated cylinder centered around 0,0,0
		VectorScale( ax[1], fx->mRadius.GetVal(), pt );
		VectorMA( pt, crandom() * 0.5f * fx->mHeight.GetVal(), ax[0], pt );
		RotatePointAroundVector( temp, ax[0], pt, random() * 360.0f );

		VectorAdd( org, temp, org );

		if ( fx->mSpawnFlags & FX_AXIS_FROM_SPHERE )
		{ 
			vec3_t up={0,0,1};

			// well, we will now override the axis at the users request
			VectorNormalize2( temp, ax[0] );

			if ( ax[0][2] == 1.0f )
			{
				// readjust up
				VectorSet( up, 0, 1, 0 );
			}

			CrossProduct( up, ax[0], ax[1] );
			CrossProduct( ax[0], ax[1], ax[2] );
		}
	}

	// There are only a few types that really use velocity and acceleration, so do extra work for those types
	//--------------------------------------------------------------------------------------------------------
	if ( fx->mType == Particle || fx->mType == OrientedParticle || fx->mType == Tail || fx->mType == Emitter )
	{
		// Velocity calculations
		//-------------------------------------
		if ( fx->mSpawnFlags & FX_VEL_IS_ABSOLUTE )
		{  
			VectorSet( vel, fx->mVelX.GetVal(), fx->mVelY.GetVal(), fx->mVelZ.GetVal() );
		}
		else
		{ // bah, do some extra work to coerce it
			VectorScale( ax[0], fx->mVelX.GetVal(), vel );
			VectorMA( vel, fx->mVelY.GetVal(), ax[1], vel );
			VectorMA( vel, fx->mVelZ.GetVal(), ax[2], vel );
		}

		//-------------------------------------
		if ( fx->mSpawnFlags & FX_AFFECTED_BY_WIND )
		{
			// wind is affecting us, so modify our initial velocity.  ideally, we would update throughout our lives, but this is easier
			VectorMA( vel, fx->mWindModifier.GetVal() * 0.01f, cl_windVec, vel );
		}

		// Acceleration calculations
		//-------------------------------------
		if ( fx->mSpawnFlags & FX_ACCEL_IS_ABSOLUTE )
		{ 
			VectorSet( accel, fx->mAccelX.GetVal(), fx->mAccelY.GetVal(), fx->mAccelZ.GetVal() );
		}
		else
		{
			VectorScale( ax[0], fx->mAccelX.GetVal(), accel );
			VectorMA( accel, fx->mAccelY.GetVal(), ax[1], accel );
			VectorMA( accel, fx->mAccelZ.GetVal(), ax[2], accel );
		}

		// Gravity is completely decoupled from acceleration since it is __always__ absolute
		// NOTE: I only effect Z ( up/down in the Quake world )
		accel[2] += fx->mGravity.GetVal();
	
		// There may be a lag between when the effect should be created and when it actually gets created.  
		//	Since we know what the discrepancy is, we can attempt to compensate...
		if ( lateTime > 0 )
		{
			// Calc the time differences
			float ftime = lateTime * 0.001f;
			float time2 = ftime * ftime * 0.5f;

			VectorMA( vel, ftime, accel, vel );

			// Predict the new position
			for ( int i = 0 ; i < 3 ; i++ ) 
			{
				org[i] = org[i] + ftime * vel[i] + time2 * vel[i];
			}
		}
	} // end moving types

	// Line type primitives work with an origin2, so do the extra work for them
	//--------------------------------------------------------------------------
	if ( fx->mType == Line || fx->mType == Electricity )
	{
		// We may have to do a trace to find our endpoint
		if ( fx->mSpawnFlags & FX_ORG2_FROM_TRACE )
		{
			VectorMA( org, FX_MAX_TRACE_DIST, ax[0], temp );

			if ( fx->mSpawnFlags & FX_ORG2_IS_OFFSET )
			{ // add a random flair to the endpoint...note: org2 will have to be pretty large to affect this much
				// we also do this pre-trace as opposed to post trace since we may have to render an impact effect
				//	and we will want the normal at the exact endpos...
				if ( fx->mSpawnFlags & FX_CHEAP_ORG2_CALC )
				{ 
					VectorSet( org2, fx->mOrigin2X.GetVal(), fx->mOrigin2Y.GetVal(), fx->mOrigin2Z.GetVal() );
					VectorAdd( org2, temp, temp );
				}
				else
				{ // I can only imagine a few cases where you might want to do this...
					VectorMA( temp, fx->mOrigin2X.GetVal(), ax[0], temp );
					VectorMA( temp, fx->mOrigin2Y.GetVal(), ax[1], temp );
					VectorMA( temp, fx->mOrigin2Z.GetVal(), ax[2], temp );
				}
			}

			theFxHelper.Trace( tr, org, NULL, NULL, temp, -1, CONTENTS_SOLID );

			VectorCopy( tr.endpos, org2 );

			if ( fx->mSpawnFlags & FX_TRACE_IMPACT_FX )
			{
				PlayEffect( fx->mImpactFxHandles.GetHandle(), org2, tr.plane.normal );
			}
		}
		else
		{
			if ( fx->mSpawnFlags & FX_CHEAP_ORG2_CALC )
			{
				VectorSet( org2, fx->mOrigin2X.GetVal(), fx->mOrigin2Y.GetVal(), fx->mOrigin2Z.GetVal() );
			}
			else
			{
				VectorScale( ax[0], fx->mOrigin2X.GetVal(), org2 );
				VectorMA( org2, fx->mOrigin2Y.GetVal(), ax[1], org2 );
				VectorMA( org2, fx->mOrigin2Z.GetVal(), ax[2], org2 );
			}

			VectorAdd( org2, origin_certain, org2 );
		}
	} // end special org2 types

	// handle RGB color, but only for types that will use it
	//---------------------------------------------------------------------------
	if ( fx->mType != Sound && fx->mType != FxRunner && fx->mType != CameraShake )
	{
		if ( fx->mSpawnFlags & FX_RGB_COMPONENT_INTERP )
		{
			float perc = random();

			VectorSet( sRGB, fx->mRedStart.GetVal( perc ), fx->mGreenStart.GetVal( perc ), fx->mBlueStart.GetVal( perc ) );
			VectorSet( eRGB, fx->mRedEnd.GetVal( perc ), fx->mGreenEnd.GetVal( perc ), fx->mBlueEnd.GetVal( perc ) );
		}
		else
		{
			VectorSet( sRGB, fx->mRedStart.GetVal(), fx->mGreenStart.GetVal(), fx->mBlueStart.GetVal() );
			VectorSet( eRGB, fx->mRedEnd.GetVal(), fx->mGreenEnd.GetVal(), fx->mBlueEnd.GetVal() );
		}
	}
	// Now create the appropriate effect entity
	//------------------------
	switch( fx->mType )
	{
	//---------
	case Particle:
	//---------
		FX_AddParticle( effectCloud, org, vel, accel, 
						fx->mSizeStart.GetVal(), fx->mSizeEnd.GetVal(), fx->mSizeParm.GetVal(),
						fx->mAlphaStart.GetVal(), fx->mAlphaEnd.GetVal(), fx->mAlphaParm.GetVal(),
						sRGB, eRGB, fx->mRGBParm.GetVal(),
						fx->mRotation.GetVal(), fx->mRotationDelta.GetVal(),
						fx->mMin, fx->mMax, fx->mElasticity.GetVal(), 
						fx->mDeathFxHandles.GetHandle(), fx->mImpactFxHandles.GetHandle(),
						fx->mLife.GetVal(), fx->mMediaHandles.GetHandle(), fx->mFlags );
		break;

	//---------
	case Line:
	//---------
		FX_AddLine( effectCloud, org, org2, 
						fx->mSizeStart.GetVal(), fx->mSizeEnd.GetVal(), fx->mSizeParm.GetVal(),
						fx->mAlphaStart.GetVal(), fx->mAlphaEnd.GetVal(), fx->mAlphaParm.GetVal(),
						sRGB, eRGB, fx->mRGBParm.GetVal(),
						fx->mLife.GetVal(), fx->mMediaHandles.GetHandle(), fx->mFlags );
		break;

	//---------
	case Tail:
	//---------
		FX_AddTail( effectCloud, org, vel, accel,
						fx->mSizeStart.GetVal(), fx->mSizeEnd.GetVal(), fx->mSizeParm.GetVal(),
						fx->mLengthStart.GetVal(), fx->mLengthEnd.GetVal(), fx->mLengthParm.GetVal(),
						fx->mAlphaStart.GetVal(), fx->mAlphaEnd.GetVal(), fx->mAlphaParm.GetVal(),
						sRGB, eRGB, fx->mRGBParm.GetVal(),
						fx->mMin, fx->mMax, fx->mElasticity.GetVal(), 
						fx->mDeathFxHandles.GetHandle(), fx->mImpactFxHandles.GetHandle(),
						fx->mLife.GetVal(), fx->mMediaHandles.GetHandle(), fx->mFlags );
		break;

	//----------------
	case Electricity:
	//----------------
		FX_AddElectricity( effectCloud, org, org2, 
						fx->mSizeStart.GetVal(), fx->mSizeEnd.GetVal(), fx->mSizeParm.GetVal(),
						fx->mAlphaStart.GetVal(), fx->mAlphaEnd.GetVal(), fx->mAlphaParm.GetVal(),
						sRGB, eRGB, fx->mRGBParm.GetVal(),
						fx->mElasticity.GetVal(), fx->mLife.GetVal(), fx->mMediaHandles.GetHandle(), fx->mFlags );
		break;

	//---------
	case Cylinder:
	//---------
		FX_AddCylinder( effectCloud, org, ax[0],
						fx->mSizeStart.GetVal(), fx->mSizeEnd.GetVal(), fx->mSizeParm.GetVal(),
						fx->mSize2Start.GetVal(), fx->mSize2End.GetVal(), fx->mSize2Parm.GetVal(),
						fx->mLengthStart.GetVal(), fx->mLengthEnd.GetVal(), fx->mLengthParm.GetVal(),
						fx->mAlphaStart.GetVal(), fx->mAlphaEnd.GetVal(), fx->mAlphaParm.GetVal(),
						sRGB, eRGB, fx->mRGBParm.GetVal(),
						fx->mLife.GetVal(), fx->mMediaHandles.GetHandle(), fx->mFlags );
		break;

	//---------
	case Emitter:
	//---------
		// for chunk angles, you don't really need much control over the end result...you just want variation..
		VectorSet( ang, 
					fx->mAngle1.GetVal(), 
					fx->mAngle2.GetVal(), 
					fx->mAngle3.GetVal() );

		vectoangles( ax[0], temp );
		VectorAdd( ang, temp, ang );

		VectorSet( angDelta, 
					fx->mAngle1Delta.GetVal(), 
					fx->mAngle2Delta.GetVal(), 
					fx->mAngle3Delta.GetVal() );

		emitterModel = fx->mMediaHandles.GetHandle();

		FX_AddEmitter( effectCloud, org, vel, accel,
						fx->mSizeStart.GetVal(), fx->mSizeEnd.GetVal(), fx->mSizeParm.GetVal(),
						fx->mAlphaStart.GetVal(), fx->mAlphaEnd.GetVal(), fx->mAlphaParm.GetVal(),
						sRGB, eRGB, fx->mRGBParm.GetVal(),
						ang, angDelta,
						fx->mMin, fx->mMax, fx->mElasticity.GetVal(),
						fx->mDeathFxHandles.GetHandle(), fx->mImpactFxHandles.GetHandle(),
						fx->mEmitterFxHandles.GetHandle(),
						fx->mDensity.GetVal(), fx->mVariance.GetVal(),
						fx->mLife.GetVal(), emitterModel, fx->mFlags );
		break;

	//---------
	case Decal:
	//---------
		{
		// I'm calling this function ( at least for now ) because it handles projecting
		//	the decal mark onto the surfaces properly.  This is especially important for large marks.
		// The downside is that it's much less flexible....
		/*CG_ImpactMark( fx->mMediaHandles.GetHandle(), org, ax[0], fx->mRotation.GetVal(), 
			sRGB[0], sRGB[1], sRGB[2], fx->mAlphaStart.GetVal(), 
			qtrue, fx->mSizeStart.GetVal(), qfalse );*/
		//VM_Call(cgvm, CG_IMPACT_MARK, fx->mMediaHandles.GetHandle(), org, ax[0], (int)fx->mRotation.GetVal(), 
		//	(int)sRGB[0], (int)sRGB[1], (int)sRGB[2], (int)fx->mAlphaStart.GetVal(), (int)fx->mSizeStart.GetVal());
		TCGImpactMark	*data = (TCGImpactMark *)cl.mSharedMemory;

		data->mHandle = fx->mMediaHandles.GetHandle();
		VectorCopy(org, data->mPoint);
		VectorCopy(ax[0], data->mAngle);
		data->mRotation = fx->mRotation.GetVal();
		data->mRed = sRGB[0];
		data->mGreen = sRGB[1];
		data->mBlue = sRGB[2];
		data->mAlphaStart = fx->mAlphaStart.GetVal();
		data->mSizeStart = fx->mSizeStart.GetVal();

		VM_Call(cgvm, CG_IMPACT_MARK);
		}
		break;
	//-------------------
	case OrientedParticle:
	//-------------------
		FX_AddOrientedParticle( effectCloud, org, ax[0], vel, accel,
					fx->mSizeStart.GetVal(), fx->mSizeEnd.GetVal(), fx->mSizeParm.GetVal(),
					fx->mAlphaStart.GetVal(), fx->mAlphaEnd.GetVal(), fx->mAlphaParm.GetVal(),
					sRGB, eRGB, fx->mRGBParm.GetVal(),
					fx->mRotation.GetVal(), fx->mRotationDelta.GetVal(),
					fx->mMin, fx->mMax, fx->mElasticity.GetVal(),
					fx->mDeathFxHandles.GetHandle(), fx->mImpactFxHandles.GetHandle(),
					fx->mLife.GetVal(), fx->mMediaHandles.GetHandle(), fx->mFlags );
		break;

	//---------
	case Sound:
	//---------
		theFxHelper.PlaySound( org, ENTITYNUM_NONE, CHAN_AUTO, fx->mMediaHandles.GetHandle() );
		break;

	//---------
	case FxRunner:
	//---------
		PlayEffect( fx->mPlayFxHandles.GetHandle(), org, ax );
		break;

	//---------
	case Light:
	//---------
		FX_AddLight( effectCloud, org, fx->mSizeStart.GetVal(), fx->mSizeEnd.GetVal(), fx->mSizeParm.GetVal(),
						sRGB, eRGB, fx->mRGBParm.GetVal(),
						fx->mLife.GetVal(), fx->mFlags );
		break;

	//---------
	case CameraShake:
	//---------
		// It calculates how intense the shake should be based on how close you are to the origin you pass in here
		//	elasticity is actually the intensity...radius is the distance in which the shake will have some effect
		//	life is how long the effect lasts.
		theFxHelper.CameraShake( org, fx->mElasticity.GetVal(), fx->mRadius.GetVal(), fx->mLife.GetVal() );
		break;

	//--------------
	case ScreenFlash:
	//--------------
		FX_AddFlash( effectCloud, org, 
					sRGB, eRGB, fx->mRGBParm.GetVal(),
					fx->mLife.GetVal(), fx->mMediaHandles.GetHandle(), fx->mFlags );
		break;

	default:
		break;
	}

	// Track when we need to clean ourselves up if we are a copy
	if ( fx->mCopy )
	{
		fx->mRefCount--;

		if ( fx->mRefCount <= 0 )
		{
			delete fx;
		}
	}
}