// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_players.c -- handle the media and animation for player entities
#include "cg_local.h"
#include "..\ghoul2\g2.h"

extern stringID_table_t animTable [MAX_ANIMATIONS+1];

char	*cg_customSoundNames[MAX_CUSTOM_SOUNDS] = {
	"*death1.wav",
	"*death2.wav",
	"*death3.wav",
	"*jump1.wav",
	"*pain25.wav",
	"*pain50.wav",
	"*pain75.wav",
	"*pain100.wav",
	"*falling1.wav",
	"*choke1.wav",
	"*choke2.wav",
	"*choke3.wav",
	"*gasp.wav",
	"*land1.wav",
	"*falling1.wav",
	"*taunt.wav"
};


/*
================
CG_CustomSound

================
*/
sfxHandle_t	CG_CustomSound( int clientNum, const char *soundName ) {
	clientInfo_t *ci;
	int			i;

	if ( soundName[0] != '*' ) {
		return trap_S_RegisterSound( soundName );
	}

	if ( clientNum < 0 || clientNum >= MAX_CLIENTS ) {
		clientNum = 0;
	}
	ci = &cgs.clientinfo[ clientNum ];

	for ( i = 0 ; i < MAX_CUSTOM_SOUNDS && cg_customSoundNames[i] ; i++ ) {
		if ( !strcmp( soundName, cg_customSoundNames[i] ) ) {
			return ci->sounds[i];
		}
	}

	CG_Error( "Unknown custom sound: %s", soundName );
	return 0;
}



/*
=============================================================================

CLIENT INFO

=============================================================================
*/

/*
==========================
CG_NeedAnimSequence

Called to check if models are missing required animation sequences
We do not list all used animations here but this check should eliminate
a broad range of unsupported models. At least the ones that are included,
which is all we really care about.
==========================
*/
qboolean CG_NeedAnimSequence(int anim)
{
	if (anim >= BOTH_DEATH1 &&
		anim <= BOTH_DEATH19)
	{
		return qtrue;
	}

	if (anim >= BOTH_DISMEMBER_HEAD1 &&
		anim <= BOTH_DISMEMBER_LARM)
	{
		return qtrue;
	}

	if (anim >= BOTH_A1_T__B_ &&
		anim <= BOTH_H1_S1_BR)
	{
		return qtrue;
	}

	return qfalse;
}

/*
==========================
CG_RegisterClientModelname
==========================
*/
static qboolean CG_RegisterClientModelname( clientInfo_t *ci, const char *modelName, const char *skinName, const char *teamName, int clientNum ) {
	int handle;
	char		afilename[MAX_QPATH];
	char		/**GLAName,*/ *slash;
	char		GLAName[MAX_QPATH];
	vec3_t	tempVec = {0,0,0};
	qboolean badModel = qfalse;
	qboolean retriedAlready = qfalse;
retryModel:
	if (ci->ATST && clientNum == -1)
	{
		Com_sprintf(ci->teamName, sizeof(ci->teamName), teamName);
		return qtrue;
	}

	if (badModel)
	{
		modelName = "kyle";
		skinName = "default";
		Com_Printf("WARNING: Attempted to load an unsupported multiplayer model! (bad or missing bone, or missing animation sequence)\n");

		badModel = qfalse;
		retriedAlready = qtrue;
	}

	// First things first.  If this is a ghoul2 model, then let's make sure we demolish this first.
	if (ci->ghoul2Model && trap_G2_HaveWeGhoul2Models(ci->ghoul2Model))
	{
		trap_G2API_CleanGhoul2Models(&(ci->ghoul2Model));
	}

	if (cgs.gametype >= GT_TEAM)
	{
		if (ci->team == TEAM_RED)
		{
			Com_sprintf(ci->skinName, sizeof(ci->skinName), "red");
			skinName = "red";
		}
		else if (ci->team == TEAM_BLUE)
		{
			Com_sprintf(ci->skinName, sizeof(ci->skinName), "blue");
			skinName = "blue";
		}
	}

	if (clientNum != -1 && cg_entities[clientNum].currentState.teamowner && !cg_entities[clientNum].isATST)
	{
		handle = trap_G2API_InitGhoul2Model(&ci->ghoul2Model, "models/players/atst/model.glm", 0, 0, 0, 0, 0);
	}
	else
	{
		Com_sprintf( afilename, sizeof( afilename ), "models/players/%s/model.glm", modelName );
		handle = trap_G2API_InitGhoul2Model(&ci->ghoul2Model, afilename, 0, trap_R_RegisterSkin(va("models/players/%s/model_%s.skin", modelName, skinName)), 0, 0, 0);
	}
	if (handle<0)
	{
		return qfalse;
	}

	if (clientNum != -1 && cg_entities[clientNum].currentState.teamowner && !cg_entities[clientNum].isATST)
	{
		ci->torsoSkin = 0;
		ci->ATST = qtrue;
	}
	else
	{
		ci->torsoSkin = trap_R_RegisterSkin(va("models/players/%s/model_%s.skin", modelName, skinName));
		ci->ATST = qfalse;
	}

	// The model is now loaded.

	GLAName[0] = 0;

	if (!BGPAFtextLoaded)
	{
		trap_G2API_GetGLAName( ci->ghoul2Model, 0, GLAName);
		if (GLAName[0] == 0/*GLAName == NULL*/)
		{
			if (!BG_ParseAnimationFile("models/players/_humanoid/animation.cfg"))
			{
				Com_Printf( "Failed to load animation file %s\n", afilename );
				return qfalse;
			}
			return qtrue;
		}
		Q_strncpyz( afilename, GLAName, sizeof( afilename ));
		slash = Q_strrchr( afilename, '/' );
		if ( slash )
		{
			strcpy(slash, "/animation.cfg");
		}	// Now afilename holds just the path to the animation.cfg
		else 
		{	// Didn't find any slashes, this is a raw filename right in base (whish isn't a good thing)
			return qfalse;
		}

		/*
		// Try to load the animation.cfg for this model then.
		if ( !BG_ParseAnimationFile( afilename, ci->animations ) )
		{	// The GLA's animations failed
			if (!BG_ParseAnimationFile("models/players/_humanoid/animation.cfg", ci->animations))
			{
				Com_Printf( "Failed to load animation file %s\n", afilename );
				return qfalse;
			}
		}
		*/
		//rww - For now, we'll just ignore what animation file it wants. In theory all multiplayer-supported models
		//should want _humanoid/animation.cfg, so if it doesn't want that then throw it away
		if (Q_stricmp(afilename, "models/players/_humanoid/animation.cfg"))
		{
			Com_Printf( "Model does not use supported animation config.\n");
			return qfalse;
		}
		else if (!BG_ParseAnimationFile("models/players/_humanoid/animation.cfg"))
		{
			Com_Printf( "Failed to load animation file models/players/_humanoid/animation.cfg\n" );
			return qfalse;
		}
		else if (!retriedAlready)
		{
			int i;

			for(i = 0; i < MAX_ANIMATIONS; i++)
			{
				if (!bgGlobalAnimations[i].firstFrame && !bgGlobalAnimations[i].numFrames && CG_NeedAnimSequence(i))
				{ //using default for this animation so it obviously never got filled in.
					//if it's a sequence that we need, this model must be an unsupported one.
					badModel = qtrue;
					goto retryModel;
				}
			}
		}
	}

	if (clientNum != -1 && cg_entities[clientNum].currentState.teamowner && !cg_entities[clientNum].isATST)
	{
		ci->torsoSkin = 0;
		ci->bolt_rhand = trap_G2API_AddBolt(ci->ghoul2Model, 0, "*flash1");
		trap_G2API_SetBoneAnim(ci->ghoul2Model, 0, "Model_root", 0, 12, BONE_ANIM_OVERRIDE_LOOP, 1.0f, cg.time, -1, -1);
		trap_G2API_SetBoneAngles(ci->ghoul2Model, 0, "l_clavical", tempVec, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, NULL, 0, cg.time);
		trap_G2API_SetBoneAngles(ci->ghoul2Model, 0, "r_clavical", tempVec, BONE_ANGLES_POSTMULT, POSITIVE_Z, NEGATIVE_Y, POSITIVE_X, NULL, 0, cg.time);

		ci->bolt_lhand = trap_G2API_AddBolt(ci->ghoul2Model, 0, "*flash2");
		ci->bolt_head = trap_G2API_AddBolt(ci->ghoul2Model, 0, "pelvis");
	}
	else
	{
		ci->bolt_rhand = trap_G2API_AddBolt(ci->ghoul2Model, 0, "*r_hand");
		
		if (!trap_G2API_SetBoneAnim(ci->ghoul2Model, 0, "model_root", 0, 12, BONE_ANIM_OVERRIDE_LOOP, 1.0f, cg.time, -1, -1))
		{
			badModel = qtrue;
		}
		
		if (!trap_G2API_SetBoneAngles(ci->ghoul2Model, 0, "upper_lumbar", tempVec, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, NULL, 0, cg.time))
		{
			badModel = qtrue;
		}

		if (!trap_G2API_SetBoneAngles(ci->ghoul2Model, 0, "cranium", tempVec, BONE_ANGLES_POSTMULT, POSITIVE_Z, NEGATIVE_Y, POSITIVE_X, NULL, 0, cg.time))
		{
			badModel = qtrue;
		}

		ci->bolt_lhand = trap_G2API_AddBolt(ci->ghoul2Model, 0, "*l_hand");
		ci->bolt_head = trap_G2API_AddBolt(ci->ghoul2Model, 0, "*head_top");

		ci->bolt_motion = trap_G2API_AddBolt(ci->ghoul2Model, 0, "Motion");

		//We need a lower lumbar bolt for footsteps
		ci->bolt_llumbar = trap_G2API_AddBolt(ci->ghoul2Model, 0, "lower_lumbar");

		if (ci->bolt_rhand == -1 || ci->bolt_lhand == -1 || ci->bolt_head == -1 || ci->bolt_motion == -1 || ci->bolt_llumbar == -1)
		{
			badModel = qtrue;
		}

		if (badModel)
		{
			goto retryModel;
		}
	}

//	ent->s.radius = 90;

	if (clientNum != -1)
	{
		if (cg_entities[clientNum].isATST)
		{
			animation_t *anim;

			anim = &bgGlobalAnimations[ (cg_entities[clientNum].currentState.legsAnim & ~ANIM_TOGGLEBIT) ];

			if (anim)
			{
				int flags = BONE_ANIM_OVERRIDE_FREEZE;
				int firstFrame = anim->firstFrame + anim->numFrames-1;

				if (anim->loopFrames != -1)
				{
					flags = BONE_ANIM_OVERRIDE_LOOP;
					firstFrame = anim->firstFrame;
				}

				//rww - Set the animation again because it just got reset due to the model change
				trap_G2API_SetBoneAnim(ci->ghoul2Model, 0, "model_root", firstFrame, anim->firstFrame + anim->numFrames, flags, 1.0f, cg.time, -1, 150);

				cg_entities[clientNum].currentState.legsAnim = 0;
			}

			anim = &bgGlobalAnimations[ (cg_entities[clientNum].currentState.torsoAnim & ~ANIM_TOGGLEBIT) ];

			if (anim)
			{
				int flags = BONE_ANIM_OVERRIDE_FREEZE;
				int firstFrame = anim->firstFrame + anim->numFrames-1;

				if (anim->loopFrames != -1)
				{
					flags = BONE_ANIM_OVERRIDE_LOOP;
					firstFrame = anim->firstFrame;
				}

				//rww - Set the animation again because it just got reset due to the model change
				trap_G2API_SetBoneAnim(ci->ghoul2Model, 0, "upper_lumbar", anim->firstFrame + anim->numFrames-1, anim->firstFrame + anim->numFrames, flags, 1.0f, cg.time, -1, 150);

				cg_entities[clientNum].currentState.torsoAnim = 0;
			}
		}

		/*
		if (cg_entities[clientNum].ghoul2 && trap_G2_HaveWeGhoul2Models(cg_entities[clientNum].ghoul2))
		{
			trap_G2API_CleanGhoul2Models(&(cg_entities[clientNum].ghoul2));
		}
		trap_G2API_DuplicateGhoul2Instance(ci->ghoul2Model, &cg_entities[clientNum].ghoul2);	
		*/

		cg_entities[clientNum].ghoul2weapon = NULL;
	}

	Com_sprintf(ci->teamName, sizeof(ci->teamName), teamName);

	// Model icon for drawing the portrait on screen
	ci->modelIcon = trap_R_RegisterShaderNoMip ( va ( "models/players/%s/icon_%s", modelName, skinName ) );

	return qtrue;
}

/*
====================
CG_ColorFromString
====================
*/
static void CG_ColorFromString( const char *v, vec3_t color ) {
	int val;

	VectorClear( color );

	val = atoi( v );

	if ( val < 1 || val > 7 ) {
		VectorSet( color, 1, 1, 1 );
		return;
	}

	if ( val & 1 ) {
		color[2] = 1.0f;
	}
	if ( val & 2 ) {
		color[1] = 1.0f;
	}
	if ( val & 4 ) {
		color[0] = 1.0f;
	}
}

#define DEFAULT_FEMALE_SOUNDPATH "chars/mp_generic_female/misc"//"chars/tavion/misc"
#define DEFAULT_MALE_SOUNDPATH "chars/mp_generic_male/misc"//"chars/kyle/misc"
/*
===================
CG_LoadClientInfo

Load it now, taking the disk hits.
This will usually be deferred to a safe time
===================
*/
void CG_LoadClientInfo( clientInfo_t *ci ) {
	const char	*dir, *fallback;
	int			i, modelloaded;
	const char	*s;
	int			clientNum;
	char		teamname[MAX_QPATH];
	int			fLen = 0;
	char		soundpath[MAX_QPATH];
	char		soundName[1024];
	qboolean	isFemale = qfalse;
	fileHandle_t f;

	clientNum = ci - cgs.clientinfo;

	if (clientNum < 0 || clientNum >= MAX_CLIENTS)
	{
		clientNum = -1;
	}

	ci->deferred = qfalse;

	/*
	if (ci->team == TEAM_SPECTATOR)
	{
		// reset any existing players and bodies, because they might be in bad
		// frames for this new model
		clientNum = ci - cgs.clientinfo;
		for ( i = 0 ; i < MAX_GENTITIES ; i++ ) {
			if ( cg_entities[i].currentState.clientNum == clientNum
				&& cg_entities[i].currentState.eType == ET_PLAYER ) {
				CG_ResetPlayerEntity( &cg_entities[i] );
			}
		}

		if (ci->ghoul2Model && trap_G2_HaveWeGhoul2Models(ci->ghoul2Model))
		{
			trap_G2API_CleanGhoul2Models(&ci->ghoul2Model);
		}

		return;
	}
	*/

	teamname[0] = 0;
	if( cgs.gametype >= GT_TEAM) {
		if( ci->team == TEAM_BLUE ) {
			Q_strncpyz(teamname, cg_blueTeamName.string, sizeof(teamname) );
		} else {
			Q_strncpyz(teamname, cg_redTeamName.string, sizeof(teamname) );
		}
	}
	if( teamname[0] ) {
		strcat( teamname, "/" );
	}
	modelloaded = qtrue;
	if ( !CG_RegisterClientModelname( ci, ci->modelName, ci->skinName, teamname, clientNum ) ) {
		//CG_Error( "CG_RegisterClientModelname( %s, %s, %s, %s %s ) failed", ci->modelName, ci->skinName, ci->headModelName, ci->headSkinName, teamname );
		//rww - DO NOT error out here! Someone could just type in a nonsense model name and crash everyone's client.
		//Give it a chance to load default model for this client instead.

		// fall back to default team name
		if( cgs.gametype >= GT_TEAM) {
			// keep skin name
			if( ci->team == TEAM_BLUE ) {
				Q_strncpyz(teamname, DEFAULT_BLUETEAM_NAME, sizeof(teamname) );
			} else {
				Q_strncpyz(teamname, DEFAULT_REDTEAM_NAME, sizeof(teamname) );
			}
			if ( !CG_RegisterClientModelname( ci, DEFAULT_TEAM_MODEL, ci->skinName, teamname, -1 ) ) {
				CG_Error( "DEFAULT_TEAM_MODEL / skin (%s/%s) failed to register", DEFAULT_TEAM_MODEL, ci->skinName );
			}
		} else {
			if ( !CG_RegisterClientModelname( ci, DEFAULT_MODEL, "default", teamname, -1 ) ) {
				CG_Error( "DEFAULT_MODEL (%s) failed to register", DEFAULT_MODEL );
			}
		}
		modelloaded = qfalse;
	}

	if (clientNum != -1 && ci->ghoul2Model && trap_G2_HaveWeGhoul2Models(ci->ghoul2Model))
	{
		if (cg_entities[clientNum].ghoul2 && trap_G2_HaveWeGhoul2Models(cg_entities[clientNum].ghoul2))
		{
			trap_G2API_CleanGhoul2Models(&cg_entities[clientNum].ghoul2);
		}
		trap_G2API_DuplicateGhoul2Instance(ci->ghoul2Model, &cg_entities[clientNum].ghoul2);
	}

	ci->newAnims = qfalse;
	if ( ci->torsoModel ) {
		orientation_t tag;
		// if the torso model has the "tag_flag"
		if ( trap_R_LerpTag( &tag, ci->torsoModel, 0, 0, 1, "tag_flag" ) ) {
			ci->newAnims = qtrue;
		}
	}

	// sounds
	dir = ci->modelName;
	fallback = DEFAULT_MALE_SOUNDPATH; //(cgs.gametype >= GT_TEAM) ? DEFAULT_TEAM_MODEL : DEFAULT_MODEL;

	fLen = trap_FS_FOpenFile(va("models/players/%s/sounds.cfg", dir), &f, FS_READ);

	soundpath[0] = 0;

	if (f)
	{
		i = 0;

		trap_FS_Read(soundpath, fLen, f);

		i = fLen;

		while (i >= 0 && soundpath[i] != '\n')
		{
			if (soundpath[i] == 'f')
			{
				isFemale = qtrue;
			}

			i--;
		}

		soundpath[i-1] = '\0';

		trap_FS_FCloseFile(f);
	}

	if (isFemale)
	{
		ci->gender = GENDER_FEMALE;
	}
	else
	{
		ci->gender = GENDER_MALE;
	}

	for ( i = 0 ; i < MAX_CUSTOM_SOUNDS ; i++ ) {
		s = cg_customSoundNames[i];
		if ( !s ) {
			break;
		}

		Com_sprintf(soundName, sizeof(soundName), "%s", s+1);
		COM_StripExtension(soundName, soundName);
		//strip the extension because we might want .mp3's

		//ci->sounds[i] = 0;
		// if the model didn't load use the sounds of the default model
		if (soundpath[0])
		{
			ci->sounds[i] = trap_S_RegisterSound( va("sound/%s/%s", soundpath, soundName) );

			if (!ci->sounds[i])
			{
				if (isFemale)
				{
					ci->sounds[i] = trap_S_RegisterSound( va("sound/%s/%s", DEFAULT_FEMALE_SOUNDPATH, soundName) );
				}
				else
				{
					ci->sounds[i] = trap_S_RegisterSound( va("sound/%s/%s", DEFAULT_MALE_SOUNDPATH, soundName) );
				}
			}
		}
		else
		{
			if (modelloaded)
			{
				ci->sounds[i] = trap_S_RegisterSound( va("sound/chars/%s/misc/%s", dir, soundName) );
			}

			if ( !ci->sounds[i] )
			{
				ci->sounds[i] = trap_S_RegisterSound( va("sound/%s/%s", fallback, soundName) );
			}
		}
	}

	ci->deferred = qfalse;

	// reset any existing players and bodies, because they might be in bad
	// frames for this new model
	clientNum = ci - cgs.clientinfo;
	for ( i = 0 ; i < MAX_GENTITIES ; i++ ) {
		if ( cg_entities[i].currentState.clientNum == clientNum
			&& cg_entities[i].currentState.eType == ET_PLAYER ) {
			CG_ResetPlayerEntity( &cg_entities[i] );
		}
	}
}




/*
======================
CG_CopyClientInfoModel
======================
*/
static void CG_CopyClientInfoModel( clientInfo_t *from, clientInfo_t *to ) {
	VectorCopy( from->headOffset, to->headOffset );
	to->footsteps = from->footsteps;
	to->gender = from->gender;

	to->legsModel = from->legsModel;
	to->legsSkin = from->legsSkin;
	to->torsoModel = from->torsoModel;
	to->torsoSkin = from->torsoSkin;
	//to->headModel = from->headModel;
	//to->headSkin = from->headSkin;
	to->modelIcon = from->modelIcon;

	to->newAnims = from->newAnims;

	//to->ghoul2Model = from->ghoul2Model;
	//rww - Trying to use the same ghoul2 pointer for two seperate clients == DISASTER
	if (to->ghoul2Model && trap_G2_HaveWeGhoul2Models(to->ghoul2Model))
	{
		trap_G2API_CleanGhoul2Models(&to->ghoul2Model);
	}
	if (from->ghoul2Model && trap_G2_HaveWeGhoul2Models(from->ghoul2Model))
	{
		trap_G2API_DuplicateGhoul2Instance(from->ghoul2Model, &to->ghoul2Model);
	}

	to->bolt_head = from->bolt_head;
	to->bolt_lhand = from->bolt_lhand;
	to->bolt_rhand = from->bolt_rhand;
	to->bolt_motion = from->bolt_motion;
	to->bolt_llumbar = from->bolt_llumbar;

//	to->ATST = from->ATST;

	memcpy( to->sounds, from->sounds, sizeof( to->sounds ) );
}

/*
======================
CG_ScanForExistingClientInfo
======================
*/
static qboolean CG_ScanForExistingClientInfo( clientInfo_t *ci ) {
	int		i;
	clientInfo_t	*match;

	for ( i = 0 ; i < cgs.maxclients ; i++ ) {
		match = &cgs.clientinfo[ i ];
		if ( !match->infoValid ) {
			continue;
		}
		if ( match->deferred ) {
			continue;
		}
		if ( !Q_stricmp( ci->modelName, match->modelName )
			&& !Q_stricmp( ci->skinName, match->skinName )
//			&& !Q_stricmp( ci->headModelName, match->headModelName )
//			&& !Q_stricmp( ci->headSkinName, match->headSkinName ) 
			&& !Q_stricmp( ci->blueTeam, match->blueTeam ) 
			&& !Q_stricmp( ci->redTeam, match->redTeam )
			&& (cgs.gametype < GT_TEAM || ci->team == match->team) 
			&& match->ghoul2Model
			&& match->bolt_head) //if the bolts haven't been initialized, this "match" is useless to us
		{
			// this clientinfo is identical, so use it's handles

			ci->deferred = qfalse;

			CG_CopyClientInfoModel( match, ci );

			return qtrue;
		}
	}

	// nothing matches, so defer the load
	return qfalse;
}

/*
======================
CG_SetDeferredClientInfo

We aren't going to load it now, so grab some other
client's info to use until we have some spare time.
======================
*/
static void CG_SetDeferredClientInfo( clientInfo_t *ci ) {
	int		i;
	clientInfo_t	*match;

	// if someone else is already the same models and skins we
	// can just load the client info
	for ( i = 0 ; i < cgs.maxclients ; i++ ) {
		match = &cgs.clientinfo[ i ];
		if ( !match->infoValid || match->deferred ) {
			continue;
		}
		if ( Q_stricmp( ci->skinName, match->skinName ) ||
			 Q_stricmp( ci->modelName, match->modelName ) ||
//			 Q_stricmp( ci->headModelName, match->headModelName ) ||
//			 Q_stricmp( ci->headSkinName, match->headSkinName ) ||
			 (cgs.gametype >= GT_TEAM && ci->team != match->team) ) {
			continue;
		}
		// just load the real info cause it uses the same models and skins
		CG_LoadClientInfo( ci );
		return;
	}

	// if we are in teamplay, only grab a model if the skin is correct
	if ( cgs.gametype >= GT_TEAM ) {
		for ( i = 0 ; i < cgs.maxclients ; i++ ) {
			match = &cgs.clientinfo[ i ];
			if ( !match->infoValid || match->deferred ) {
				continue;
			}
			if ( Q_stricmp( ci->skinName, match->skinName ) ||
				(cgs.gametype >= GT_TEAM && ci->team != match->team) ) {
				continue;
			}
			ci->deferred = qtrue;
			CG_CopyClientInfoModel( match, ci );
			return;
		}
		// load the full model, because we don't ever want to show
		// an improper team skin.  This will cause a hitch for the first
		// player, when the second enters.  Combat shouldn't be going on
		// yet, so it shouldn't matter
		CG_LoadClientInfo( ci );
		return;
	}

	// find the first valid clientinfo and grab its stuff
	for ( i = 0 ; i < cgs.maxclients ; i++ ) {
		match = &cgs.clientinfo[ i ];
		if ( !match->infoValid ) {
			continue;
		}

		ci->deferred = qtrue;
		CG_CopyClientInfoModel( match, ci );
		return;
	}

	// we should never get here...
	CG_Printf( "CG_SetDeferredClientInfo: no valid clients!\n" );

	CG_LoadClientInfo( ci );
}

/*
======================
CG_NewClientInfo
======================
*/
void CG_NewClientInfo( int clientNum, qboolean entitiesInitialized ) {
	clientInfo_t *ci;
	clientInfo_t newInfo;
	const char	*configstring;
	const char	*v;
	char		*slash;
	void *oldGhoul2;
	int i = 0;
	qboolean wasATST = qfalse;

	ci = &cgs.clientinfo[clientNum];

	oldGhoul2 = ci->ghoul2Model;

	configstring = CG_ConfigString( clientNum + CS_PLAYERS );
	if ( !configstring[0] ) {
		memset( ci, 0, sizeof( *ci ) );
		return;		// player just left
	}

	if (ci)
	{
		wasATST = ci->ATST;
	}

	// build into a temp buffer so the defer checks can use
	// the old value
	memset( &newInfo, 0, sizeof( newInfo ) );

	// isolate the player's name
	v = Info_ValueForKey(configstring, "n");
	Q_strncpyz( newInfo.name, v, sizeof( newInfo.name ) );

	// colors
	v = Info_ValueForKey( configstring, "c1" );
	CG_ColorFromString( v, newInfo.color1 );

	newInfo.icolor1 = atoi(v);

	v = Info_ValueForKey( configstring, "c2" );
	CG_ColorFromString( v, newInfo.color2 );

	// bot skill
	v = Info_ValueForKey( configstring, "skill" );
	newInfo.botSkill = atoi( v );

	// handicap
	v = Info_ValueForKey( configstring, "hc" );
	newInfo.handicap = atoi( v );

	// wins
	v = Info_ValueForKey( configstring, "w" );
	newInfo.wins = atoi( v );

	// losses
	v = Info_ValueForKey( configstring, "l" );
	newInfo.losses = atoi( v );

	// team
	v = Info_ValueForKey( configstring, "t" );
	newInfo.team = atoi( v );

	// team task
	v = Info_ValueForKey( configstring, "tt" );
	newInfo.teamTask = atoi(v);

	// team leader
	v = Info_ValueForKey( configstring, "tl" );
	newInfo.teamLeader = atoi(v);

	v = Info_ValueForKey( configstring, "g_redteam" );
	Q_strncpyz(newInfo.redTeam, v, MAX_TEAMNAME);

	v = Info_ValueForKey( configstring, "g_blueteam" );
	Q_strncpyz(newInfo.blueTeam, v, MAX_TEAMNAME);

	// model
	v = Info_ValueForKey( configstring, "model" );
	if ( cg_forceModel.integer ) {
		// forcemodel makes everyone use a single model
		// to prevent load hitches
		char modelStr[MAX_QPATH];
		char *skin;

		if( cgs.gametype >= GT_TEAM ) {
			Q_strncpyz( newInfo.modelName, DEFAULT_TEAM_MODEL, sizeof( newInfo.modelName ) );
			Q_strncpyz( newInfo.skinName, "default", sizeof( newInfo.skinName ) );
		} else {
			trap_Cvar_VariableStringBuffer( "model", modelStr, sizeof( modelStr ) );
			if ( ( skin = strchr( modelStr, '/' ) ) == NULL) {
				skin = "default";
			} else {
				*skin++ = 0;
			}

			Q_strncpyz( newInfo.skinName, skin, sizeof( newInfo.skinName ) );
			Q_strncpyz( newInfo.modelName, modelStr, sizeof( newInfo.modelName ) );
		}

		if ( cgs.gametype >= GT_TEAM ) {
			// keep skin name
			slash = strchr( v, '/' );
			if ( slash ) {
				Q_strncpyz( newInfo.skinName, slash + 1, sizeof( newInfo.skinName ) );
			}
		}
	} else {
		Q_strncpyz( newInfo.modelName, v, sizeof( newInfo.modelName ) );

		slash = strchr( newInfo.modelName, '/' );
		if ( !slash ) {
			// modelName didn not include a skin name
			Q_strncpyz( newInfo.skinName, "default", sizeof( newInfo.skinName ) );
		} else {
			Q_strncpyz( newInfo.skinName, slash + 1, sizeof( newInfo.skinName ) );
			// truncate modelName
			*slash = 0;
		}
	}

	// head model
/*
	v = Info_ValueForKey( configstring, "hmodel" );
	if ( cg_forceModel.integer ) {
		// forcemodel makes everyone use a single model
		// to prevent load hitches
		char modelStr[MAX_QPATH];
		char *skin;

		if( cgs.gametype >= GT_TEAM ) {
			Q_strncpyz( newInfo.headModelName, DEFAULT_TEAM_MODEL, sizeof( newInfo.headModelName ) );
			Q_strncpyz( newInfo.headSkinName, "default", sizeof( newInfo.headSkinName ) );
		} else {
			trap_Cvar_VariableStringBuffer( "headmodel", modelStr, sizeof( modelStr ) );
			if ( ( skin = strchr( modelStr, '/' ) ) == NULL) {
				skin = "default";
			} else {
				*skin++ = 0;
			}

			Q_strncpyz( newInfo.headSkinName, skin, sizeof( newInfo.headSkinName ) );
			Q_strncpyz( newInfo.headModelName, modelStr, sizeof( newInfo.headModelName ) );
		}

		if ( cgs.gametype >= GT_TEAM ) {
			// keep skin name
			slash = strchr( v, '/' );
			if ( slash ) {
				Q_strncpyz( newInfo.headSkinName, slash + 1, sizeof( newInfo.headSkinName ) );
			}
		}
	} else {
		Q_strncpyz( newInfo.headModelName, v, sizeof( newInfo.headModelName ) );

		slash = strchr( newInfo.headModelName, '/' );
		if ( !slash ) {
			// modelName didn not include a skin name
			Q_strncpyz( newInfo.headSkinName, "default", sizeof( newInfo.headSkinName ) );
		} else {
			Q_strncpyz( newInfo.headSkinName, slash + 1, sizeof( newInfo.headSkinName ) );
			// truncate modelName
			*slash = 0;
		}
	}
*/
	// force powers
	v = Info_ValueForKey( configstring, "forcepowers" );
	Q_strncpyz( newInfo.forcePowers, v, sizeof( newInfo.forcePowers ) );

	newInfo.ATST = wasATST;

	if (cgs.gametype >= GT_TEAM)
	{
		if (newInfo.team == TEAM_RED)
		{
			strcpy(newInfo.skinName, "red");
//			strcpy(newInfo.headSkinName, "red");
		}
		if (newInfo.team == TEAM_BLUE)
		{
			strcpy(newInfo.skinName, "blue");
//			strcpy(newInfo.headSkinName, "blue");
		}
	}

	// scan for an existing clientinfo that matches this modelname
	// so we can avoid loading checks if possible
	if ( !CG_ScanForExistingClientInfo( &newInfo ) ) {
		qboolean	forceDefer;

		forceDefer = trap_MemoryRemaining() < 4000000;

		// if we are defering loads, just have it pick the first valid
		if (cg.snap && cg.snap->ps.clientNum == clientNum && !forceDefer)
		{ //rww - don't defer your own client info ever, unless really low on memory
			CG_LoadClientInfo( &newInfo );
		}
		else if ( forceDefer || ( cg_deferPlayers.integer && !cg_buildScript.integer && !cg.loading ) ) {
			// keep whatever they had if it won't violate team skins
			CG_SetDeferredClientInfo( &newInfo );
			// if we are low on memory, leave them with this model
			if ( forceDefer ) {
				CG_Printf( "Memory is low.  Using deferred model.\n" );
				newInfo.deferred = qfalse;
			}
		} else {
			CG_LoadClientInfo( &newInfo );
		}
	}

	// replace whatever was there with the new one
	newInfo.infoValid = qtrue;
	*ci = newInfo;

	//force a weapon change anyway, for all clients being rendered to the current client
	while (i < MAX_CLIENTS)
	{
		cg_entities[i].ghoul2weapon = NULL;
		i++;
	}

	// Check if the ghoul2 model changed in any way.  This is safer than assuming we have a legal cent shile loading info.
	if (entitiesInitialized && ci->ghoul2Model && (oldGhoul2 != ci->ghoul2Model))
	{	// Copy the new ghoul2 model to the centity.
		animation_t *anim;
		// First check if we have a ghoul2 model on the client entity.
		
		anim = &bgGlobalAnimations[ (cg_entities[clientNum].currentState.legsAnim & ~ANIM_TOGGLEBIT) ];

		if (anim)
		{
			int flags = BONE_ANIM_OVERRIDE_FREEZE;
			int firstFrame = anim->firstFrame + anim->numFrames-1;

			if (anim->loopFrames != -1)
			{
				flags = BONE_ANIM_OVERRIDE_LOOP;
				firstFrame = anim->firstFrame;
			}

			//rww - Set the animation again because it just got reset due to the model change
			trap_G2API_SetBoneAnim(ci->ghoul2Model, 0, "model_root", firstFrame, anim->firstFrame + anim->numFrames, flags, 1.0f, cg.time, -1, 150);

			cg_entities[clientNum].currentState.legsAnim = 0;
		}

		anim = &bgGlobalAnimations[ (cg_entities[clientNum].currentState.torsoAnim & ~ANIM_TOGGLEBIT) ];

		if (anim)
		{
			int flags = BONE_ANIM_OVERRIDE_FREEZE;
			int firstFrame = anim->firstFrame + anim->numFrames-1;

			if (anim->loopFrames != -1)
			{
				flags = BONE_ANIM_OVERRIDE_LOOP;
				firstFrame = anim->firstFrame;
			}

			//rww - Set the animation again because it just got reset due to the model change
			trap_G2API_SetBoneAnim(ci->ghoul2Model, 0, "upper_lumbar", anim->firstFrame + anim->numFrames-1, anim->firstFrame + anim->numFrames, flags, 1.0f, cg.time, -1, 150);

			cg_entities[clientNum].currentState.torsoAnim = 0;
		}

		if (cg_entities[clientNum].ghoul2 && trap_G2_HaveWeGhoul2Models(cg_entities[clientNum].ghoul2))
		{
			trap_G2API_CleanGhoul2Models(&cg_entities[clientNum].ghoul2);
		}
		trap_G2API_DuplicateGhoul2Instance(ci->ghoul2Model, &cg_entities[clientNum].ghoul2);

		/*
		if (cg_entities[clientNum].currentState.weapon > WP_NONE)
		{
			CG_CopyG2WeaponInstance(cg_entities[clientNum].currentState.weapon, cg_entities[clientNum].ghoul2);
		}
		*/
		//It should catch this next update anyway. We just set all ghoul2weapon's to NULL above.
	}
	/*
	else if (ci->team == TEAM_SPECTATOR && cg_entities[clientNum].ghoul2 && trap_G2_HaveWeGhoul2Models(cg_entities[clientNum].ghoul2))
	{ //this shouldn't actually happen now because we are not trying to register models for spectators. But just in case.
		trap_G2API_CleanGhoul2Models(&cg_entities[clientNum].ghoul2);
		if (ci->ghoul2Model && trap_G2_HaveWeGhoul2Models(ci->ghoul2Model))
		{
			trap_G2API_DuplicateGhoul2Instance(ci->ghoul2Model, &cg_entities[clientNum].ghoul2);
		}
	}
	*/

}


qboolean cgQueueLoad = qfalse;
/*
======================
CG_ActualLoadDeferredPlayers

Called at the beginning of CG_Player if cgQueueLoad is set.
======================
*/
void CG_ActualLoadDeferredPlayers( void )
{
	int		i;
	clientInfo_t	*ci;

	// scan for a deferred player to load
	for ( i = 0, ci = cgs.clientinfo ; i < cgs.maxclients ; i++, ci++ ) {
		if ( ci->infoValid && ci->deferred ) {
			// if we are low on memory, leave it deferred
			if ( trap_MemoryRemaining() < 4000000 ) {
				CG_Printf( "Memory is low.  Using deferred model.\n" );
				ci->deferred = qfalse;
				continue;
			}
			CG_LoadClientInfo( ci );
//			break;
		}
	}
}

/*
======================
CG_LoadDeferredPlayers

Called each frame when a player is dead
and the scoreboard is up
so deferred players can be loaded
======================
*/
void CG_LoadDeferredPlayers( void ) {
	cgQueueLoad = qtrue;
}

/*
=============================================================================

PLAYER ANIMATION

=============================================================================
*/

static qboolean CG_FirstAnimFrame(clientInfo_t *ci, lerpFrame_t *lf, qboolean torsoOnly, float speedScale);

qboolean CG_InRoll( centity_t *cent )
{
	switch ( (cent->currentState.legsAnim&~ANIM_TOGGLEBIT) )
	{
	case BOTH_ROLL_F:
	case BOTH_ROLL_B:
	case BOTH_ROLL_R:
	case BOTH_ROLL_L:
		if ( cent->pe.legs.animationTime > cg.time )
		{
			return qtrue;
		}
		break;
	}
	return qfalse;
}

qboolean CG_InRollAnim( centity_t *cent )
{
	switch ( (cent->currentState.legsAnim&~ANIM_TOGGLEBIT) )
	{
	case BOTH_ROLL_F:
	case BOTH_ROLL_B:
	case BOTH_ROLL_R:
	case BOTH_ROLL_L:
		return qtrue;
	}
	return qfalse;
}

/*
===============
CG_SetLerpFrameAnimation

may include ANIM_TOGGLEBIT
===============
*/
static void CG_SetLerpFrameAnimation( centity_t *cent, clientInfo_t *ci, lerpFrame_t *lf, int newAnimation, float animSpeedMult, qboolean torsoOnly) {
	animation_t	*anim;
	float animSpeed;
	int	  flags=BONE_ANIM_OVERRIDE_FREEZE;
	int oldAnim = -1;
	int blendTime = 150;

	if (cent->currentState.number < MAX_CLIENTS &&
		cent->currentState.teamowner &&
		!cent->isATST)
	{
		return;
	}

	if (cent->isATST)
	{
		vec3_t testVel;
		float fVel;

		if (lf->animationNumber == BOTH_RUN1 ||
			lf->animationNumber == BOTH_WALK1 ||
			lf->animationNumber == BOTH_WALKBACK1 ||
			lf->animationNumber == BOTH_TURN_LEFT1 ||
			lf->animationNumber == BOTH_TURN_RIGHT1)
		{
			if (cent->atstFootClang < cg.time)
			{
				if (rand() & 1)
				{
					trap_S_StartSound(NULL, cent->currentState.number, CHAN_AUTO, trap_S_RegisterSound("sound/chars/atst/ATSTstep1.wav"));
				}
				else
				{
					trap_S_StartSound(NULL, cent->currentState.number, CHAN_AUTO, trap_S_RegisterSound("sound/chars/atst/ATSTstep2.wav"));
				}

				cent->atstFootClang = cg.time + 1300;
			}
		}

		VectorCopy(cent->currentState.pos.trDelta, testVel);

		fVel = VectorNormalize(testVel);

		if (cent->currentState.eFlags & EF_DEAD)
		{
			if (lf->animationNumber != BOTH_DEATH1 &&
				lf->animationNumber != BOTH_DEAD1)
			{
				trap_S_StartSound(NULL, cent->currentState.number, CHAN_BODY, trap_S_RegisterSound("sound/chars/atst/ATSTcrash.wav"));
			}
			newAnimation = BOTH_DEATH1;
		}
		else if (fVel > 0)
		{
			qboolean doNotSet = qfalse;

			if (fVel > 250)
			{
				animSpeedMult = 1.2;
			}
			else if (fVel > 200)
			{
				animSpeedMult = 1;
			}
			else if (fVel > 100)
			{
				animSpeedMult = 0.9;
			}
			else if (fVel > 0)
			{
				animSpeedMult = 0.8;
			}
			else
			{
				doNotSet = qtrue;
			}

			if (!doNotSet)
			{
				if ((cent->currentState.torsoAnim & ~ANIM_TOGGLEBIT) == BOTH_RUN1 ||
					(cent->currentState.torsoAnim & ~ANIM_TOGGLEBIT) == BOTH_WALKBACK1 ||
					(cent->currentState.torsoAnim & ~ANIM_TOGGLEBIT) == BOTH_WALK1)
				{
					newAnimation = (cent->currentState.torsoAnim & ~ANIM_TOGGLEBIT);
				}
				else
				{
					newAnimation = BOTH_RUN1;
				}
			}
		}
		else if ((cent->currentState.torsoAnim & ~ANIM_TOGGLEBIT) == BOTH_RUN1START)
		{
			if (lf->animationNumber != BOTH_RUN1START)
			{
				trap_S_StartSound(NULL, cent->currentState.number, CHAN_BODY, trap_S_RegisterSound("sound/chars/atst/ATSTstart.wav"));
				cent->atstFootClang = cg.time + 650;
			}
			newAnimation = BOTH_RUN1START;
		}
		else if (cent->pe.legs.yawing &&
			(cent->pe.legs.yawSwingDif < -20 || cent->pe.legs.yawSwingDif > 20))
		{
			if (cent->pe.legs.yawSwingDif > 0)
			{
				newAnimation = BOTH_TURN_LEFT1;//BOTH_RUN1;
				if (lf->animationNumber != BOTH_TURN_LEFT1)
				{
					cent->atstFootClang = cg.time + 500;
				}
			}
			else
			{
				newAnimation = BOTH_TURN_RIGHT1;//BOTH_RUN1;
				if (lf->animationNumber != BOTH_TURN_RIGHT1)
				{
					cent->atstFootClang = cg.time + 500;
				}
			}
			animSpeedMult = 0.7;
		}
		else
		{
			newAnimation = BOTH_STAND1;
		}

		if (newAnimation != BOTH_STAND1 && newAnimation != BOTH_RUN1 &&
			newAnimation != BOTH_RUN1START && newAnimation != BOTH_WALK1 &&
			newAnimation != BOTH_WALKBACK1 && newAnimation != BOTH_DEATH1 &&
			newAnimation != BOTH_DEAD1 && newAnimation != BOTH_TURN_RIGHT1 &&
			newAnimation != BOTH_TURN_LEFT1 && newAnimation != BOTH_PAIN1 &&
			newAnimation != BOTH_PAIN2)
		{ //not a valid anim for the ATST..
			newAnimation = BOTH_RUN1;
		}

		if (lf->animationNumber == newAnimation)
		{
			return;
		}

//		if (lf->animationNumber != BOTH_STAND1 &&
//			((lf->frame-lf->animation->firstFrame) < (lf->animation->numFrames/2)-8 ||
//			(lf->frame-lf->animation->firstFrame) > (lf->animation->numFrames/2)+8))
//		{
//			return;
//		}
	//	if (lf->animationNumber != BOTH_STAND1 &&
	//		!CG_FirstAnimFrame(ci, lf, torsoOnly, animSpeedMult))
	//	{
	//		return;
	//	}
	}

	oldAnim = lf->animationNumber;

	lf->animationNumber = newAnimation;
	newAnimation &= ~ANIM_TOGGLEBIT;

	if ( newAnimation < 0 || newAnimation >= MAX_TOTALANIMATIONS ) {
		CG_Error( "Bad animation number: %i", newAnimation );
	}

	anim = &bgGlobalAnimations[ newAnimation ];

	lf->animation = anim;
	lf->animationTime = lf->frameTime + anim->initialLerp;

	if ( cg_debugAnim.integer ) {
		CG_Printf( "Anim: %i, '%s'\n", newAnimation, GetStringForID(animTable, newAnimation));
	}

	if (cent->ghoul2)
	{
		animSpeed = 50.0f / anim->frameLerp;
		if (lf->animation->loopFrames != -1)
		{
			flags = BONE_ANIM_OVERRIDE_LOOP;
		}

		if (cent->isATST)
		{
			if (animSpeed < 0.3)
			{
				animSpeed = 0.3;
			}

			if (newAnimation == BOTH_WALKBACK1)
			{
				animSpeed = 0.8;
			}

			if (newAnimation != BOTH_DEATH1)
			{
				flags = BONE_ANIM_OVERRIDE_LOOP;
			}
		}

		if (cg_animBlend.integer)
		{
			flags |= BONE_ANIM_BLEND;
		}

		if (!cent->isATST)
		{
			if (/*BG_FlippingAnim(newAnimation) ||*/ BG_InDeathAnim(newAnimation))
			{
				flags &= ~BONE_ANIM_BLEND;
			}
			else if ( oldAnim != -1 &&
				(/*BG_FlippingAnim(oldAnim) ||*/ BG_InDeathAnim(oldAnim)) )
			{
				flags &= ~BONE_ANIM_BLEND;
			}

			if (flags & BONE_ANIM_BLEND)
			{
				if (BG_FlippingAnim(newAnimation))
				{
					blendTime = 200;
				}
				else if ( oldAnim != -1 &&
					(BG_FlippingAnim(oldAnim)) )
				{
					blendTime = 200;
				}
			}
		}

		animSpeed *= animSpeedMult;

		if (torsoOnly)
		{
			lf->animationTorsoSpeed = animSpeedMult;
		}
		else
		{
			lf->animationSpeed = animSpeedMult;
		}

		if (cent->isATST)
		{
			int atstBlend = 400;

			if (torsoOnly)
			{
				trap_G2API_SetBoneAnim(cent->ghoul2, 0, "pelvis", anim->firstFrame, anim->firstFrame + anim->numFrames, flags, animSpeed, cg.time, -1, atstBlend);
			}
			else
			{
				trap_G2API_SetBoneAnim(cent->ghoul2, 0, "model_root", anim->firstFrame, anim->firstFrame + anim->numFrames, flags, animSpeed, cg.time, -1, atstBlend);
			}
		}
		else
		{
			if (torsoOnly)
			{
				trap_G2API_SetBoneAnim(cent->ghoul2, 0, "upper_lumbar", anim->firstFrame, anim->firstFrame + anim->numFrames, flags, animSpeed,cg.time, -1, blendTime);
				cgs.clientinfo[cent->currentState.number].torsoAnim = newAnimation;
			}
			else
			{
				trap_G2API_SetBoneAnim(cent->ghoul2, 0, "model_root", anim->firstFrame, anim->firstFrame + anim->numFrames, flags, animSpeed, cg.time, -1, blendTime);
				cgs.clientinfo[cent->currentState.number].torsoAnim = newAnimation;
				cgs.clientinfo[cent->currentState.number].legsAnim = newAnimation;
			}

			/*
			if ((cent->currentState.torsoAnim&~ANIM_TOGGLEBIT) == newAnimation &&
				!BG_FlippingAnim( cent->currentState.legsAnim ) &&
				!BG_SpinningSaberAnim( cent->currentState.legsAnim ) &&
				!BG_SpinningSaberAnim( cent->currentState.torsoAnim ) &&
				!BG_InSpecialJump( cent->currentState.legsAnim ) &&
				!BG_InSpecialJump( cent->currentState.torsoAnim ) &&
				!BG_InDeathAnim(cent->currentState.legsAnim) &&
				!BG_InDeathAnim(cent->currentState.torsoAnim) &&
				!CG_InRoll(cent) &&
				!BG_SaberInSpecial(cent->currentState.saberMove) &&
				!BG_SaberInSpecialAttack(cent->currentState.torsoAnim) &&
				!BG_SaberInSpecialAttack(cent->currentState.legsAnim) )
				*/
			if (cg.snap && cg.snap->ps.clientNum == cent->currentState.number)
			{ //go ahead and use the predicted state if you can.
				if ((cg.predictedPlayerState.torsoAnim&~ANIM_TOGGLEBIT) == newAnimation)
				{
					trap_G2API_SetBoneAnim(cent->ghoul2, 0, "Motion", anim->firstFrame, anim->firstFrame + anim->numFrames, flags, animSpeed, cg.time, -1, blendTime);
				}
			}
			else
			{
				if ((cent->currentState.torsoAnim&~ANIM_TOGGLEBIT) == newAnimation)
				{
					trap_G2API_SetBoneAnim(cent->ghoul2, 0, "Motion", anim->firstFrame, anim->firstFrame + anim->numFrames, flags, animSpeed, cg.time, -1, blendTime);
				}
			}
		}
	}
}


/*
===============
CG_FirstAnimFrame

Returns true if the lerpframe is on its first frame of animation.
Otherwise false.

This is used to scale an animation into higher-speed without restarting
the animation before it completes at normal speed, in the case of a looping
animation (such as the leg running anim).
===============
*/
static qboolean CG_FirstAnimFrame(clientInfo_t *ci, lerpFrame_t *lf, qboolean torsoOnly, float speedScale)
{
	if (torsoOnly)
	{
		if (lf->animationTorsoSpeed == speedScale)
		{
			return qfalse;
		}
	}
	else
	{
		if (lf->animationSpeed == speedScale)
		{
			return qfalse;
		}
	}

	if (lf->animation->numFrames < 2)
	{
		return qtrue;
	}

	if (lf->animation->firstFrame == lf->frame)
	{
		return qtrue;
	}

	return qfalse;
}

/*
===============
CG_InWalkingAnim

Returns last frame to step on if the animation on the frame is desired for playing footstep sounds,
otherwise 0
===============
*/
int CG_InWalkingAnim(int animNum)
{
	int anim = animNum;
	
	anim &= ~ANIM_TOGGLEBIT;

	if (anim == BOTH_WALL_RUN_RIGHT ||
		anim == BOTH_WALL_RUN_LEFT)
	{
		return 8;
	}

	if (anim >= BOTH_WALK1 &&
		anim <= BOTH_RUNAWAY1)
	{
		if (anim == BOTH_RUN1)
		{
			return 18;//12;
		}
		else
		{
			//return 9;
			return 18;
		}
	}

	if (anim >= BOTH_WALKBACK1 &&
		anim <= BOTH_RUNBACK2)
	{
		if (anim == BOTH_WALKBACK1)
		{
			return 18;
		}
		else
		{
			return 9;
		}
	}

	if (anim >= LEGS_WALKBACK1 &&
		anim <= LEGS_RUNBACK2)
	{
		if (anim == LEGS_WALKBACK1)
		{
			return 18;
		}
		else
		{
			return 9;
		}
	}

	return qfalse;
}

#define FOOTSTEP_GENERIC					1
#define FOOTSTEP_METAL						2

static int CG_FootstepForSurface( centity_t *cent, int skip )
{
	trace_t tr;
	vec3_t org, dOrg, legDir, bAngles;
	vec3_t playerMins = {-15, -15, DEFAULT_MINS_2};
	vec3_t playerMaxs = {15, 15, DEFAULT_MAXS_2};
	mdxaBone_t boltMatrix;

//	VectorCopy(ent->lerpOrigin, org);
//	VectorCopy(org, dOrg);
//	dOrg[2] -= 64; //should never have to go further than this because we're already onground

	//We are now tracing off of a bolt point for footsteps, so that if we happen to be walking sideways on a wall we will
	//still play step sounds properly.

	VectorCopy(cent->lerpAngles, bAngles);
	bAngles[PITCH] = 0;

	trap_G2API_GetBoltMatrix_NoReconstruct( cent->ghoul2, 0, cgs.clientinfo[cent->currentState.number].bolt_llumbar, &boltMatrix, bAngles, cent->lerpOrigin, cg.time, /*cgs.gameModels*/0, cent->modelScale);
	trap_G2API_GiveMeVectorFromMatrix( &boltMatrix, ORIGIN, org );
	trap_G2API_GiveMeVectorFromMatrix( &boltMatrix, NEGATIVE_Y, legDir );

	dOrg[0] = org[0] + legDir[0]*64;
	dOrg[1] = org[1] + legDir[1]*64;
	dOrg[2] = org[2] + legDir[2]*64;

	//CG_TestLine(org, dOrg, 100, 0x0000ff, 3);

	CG_Trace(&tr, org, playerMins, playerMaxs, dOrg, skip, MASK_PLAYERSOLID);

	if (tr.fraction == 1)
	{
		return 0;
	}

	if ( tr.surfaceFlags & SURF_NOSTEPS )
	{
		return 0;
	}

	if ( tr.surfaceFlags & SURF_METALSTEPS )
	{
		return FOOTSTEP_METAL;
	}

	return FOOTSTEP_GENERIC;
}

void CG_FootStep(centity_t *cent, clientInfo_t *ci, int anim)
{
	int groundType;

	if ((anim & ~ANIM_TOGGLEBIT) == BOTH_WALL_RUN_RIGHT ||
		(anim & ~ANIM_TOGGLEBIT) == BOTH_WALL_RUN_LEFT)
	{
		groundType = FOOTSTEP_GENERIC;
		goto skipCheck;
	}

	if (cent->currentState.groundEntityNum == ENTITYNUM_NONE)
	{
		return;
	}

skipCheck:
	groundType = CG_FootstepForSurface(cent, cent->currentState.number);

//skipCheck:

	if (!groundType)
	{
		return;
	}

	switch (groundType)
	{
	case FOOTSTEP_GENERIC:
		trap_S_StartSound (NULL, cent->currentState.number, CHAN_BODY, 
			cgs.media.footsteps[ ci->footsteps ][rand()&3] );
		break;
	case FOOTSTEP_METAL:
		trap_S_StartSound (NULL, cent->currentState.number, CHAN_BODY, 
			cgs.media.footsteps[ FOOTSTEP_METAL ][rand()&3] );
		break;
	default:
		break;
	}
}
/*
===============
CG_RunLerpFrame

Sets cg.snap, cg.oldFrame, and cg.backlerp
cg.time should be between oldFrameTime and frameTime after exit
===============
*/
static void CG_RunLerpFrame( centity_t *cent, clientInfo_t *ci, lerpFrame_t *lf, int newAnimation, float speedScale, qboolean torsoOnly) 
{
	int			f, numFrames;
	animation_t	*anim;

	// debugging tool to get no animations
	if ( cg_animSpeed.integer == 0 ) {
		lf->oldFrame = lf->frame = lf->backlerp = 0;
		return;
	}

	// see if the animation sequence is switching
	if (cent->currentState.forceFrame)
	{
		int flags = BONE_ANIM_OVERRIDE_FREEZE; //|BONE_ANIM_BLEND;
		float animSpeed = 1.0f;
		trap_G2API_SetBoneAnim(cent->ghoul2, 0, "upper_lumbar", cent->currentState.forceFrame, cent->currentState.forceFrame+1, flags, animSpeed, cg.time, -1, 150);
		trap_G2API_SetBoneAnim(cent->ghoul2, 0, "model_root", cent->currentState.forceFrame, cent->currentState.forceFrame+1, flags, animSpeed, cg.time, -1, 150);
		trap_G2API_SetBoneAnim(cent->ghoul2, 0, "Motion", cent->currentState.forceFrame, cent->currentState.forceFrame+1, flags, animSpeed, cg.time, -1, 150);

		lf->animationNumber = 0;
	}
	else if ( (newAnimation != lf->animationNumber || !lf->animation) || (CG_FirstAnimFrame(ci, lf, torsoOnly, speedScale)) ) 
	{
		CG_SetLerpFrameAnimation( cent, ci, lf, newAnimation, speedScale, torsoOnly);
	}
	else if (cent->isATST)
	{
		if (cent->pe.legs.yawing != !lf->torsoYawing)
		{
			CG_SetLerpFrameAnimation( cent, ci, lf, newAnimation, speedScale, torsoOnly);
			lf->torsoYawing = cent->pe.legs.yawing;
		}
	}

	// if we have passed the current frame, move it to
	// oldFrame and calculate a new frame
	if ( cg.time >= lf->frameTime ) {
		if (lf->oldFrame != lf->frame &&
			lf == &(cent->pe.legs))
		{
			int addFinalFrame = CG_InWalkingAnim(lf->animationNumber); //9;

			if (!cent->isATST &&
				((lf->animationNumber&~ANIM_TOGGLEBIT) == BOTH_WALL_RUN_RIGHT || (lf->animationNumber&~ANIM_TOGGLEBIT) == BOTH_WALL_RUN_LEFT) &&
				addFinalFrame)
			{
				if ( lf->frame >= (lf->animation->firstFrame+2) &&
					lf->oldFrame < (lf->animation->firstFrame+2))
				{
					CG_FootStep(cent, ci, lf->animationNumber);
				}
				else if ( lf->frame >= (lf->animation->firstFrame+addFinalFrame) &&
					lf->oldFrame < (lf->animation->firstFrame+addFinalFrame))
				{
					CG_FootStep(cent, ci, lf->animationNumber);
				}
				else if ( lf->frame >= (lf->animation->firstFrame+12) &&
					lf->oldFrame < (lf->animation->firstFrame+12))
				{
					CG_FootStep(cent, ci, lf->animationNumber);
				}
				else if ( lf->frame >= (lf->animation->firstFrame+16) &&
					lf->oldFrame < (lf->animation->firstFrame+16))
				{
					CG_FootStep(cent, ci, lf->animationNumber);
				}
				else if (lf->oldFrame > lf->frame && lf->frame > (lf->animation->firstFrame+1))
				{ //missed one
					CG_FootStep(cent, ci, lf->animationNumber);
				}
			}
			else if (addFinalFrame && !cent->isATST)
			{
				if ( lf->frame >= (lf->animation->firstFrame+3) &&
					lf->oldFrame < (lf->animation->firstFrame+3))
				{
					CG_FootStep(cent, ci, lf->animationNumber);
				}
				else if ( lf->frame >= (lf->animation->firstFrame+addFinalFrame) &&
					lf->oldFrame < (lf->animation->firstFrame+addFinalFrame))
				{
					CG_FootStep(cent, ci, lf->animationNumber);
				}
				else if (lf->oldFrame > lf->frame && lf->frame > (lf->animation->firstFrame+1))
				{ //missed one
					CG_FootStep(cent, ci, lf->animationNumber);
				}
			}
		}

		lf->oldFrame = lf->frame;
		lf->oldFrameTime = lf->frameTime;

		// get the next frame based on the animation
		anim = lf->animation;
		if ( !anim->frameLerp ) {
			return;		// shouldn't happen
		}

		if ( cg.time < lf->animationTime ) {
			lf->frameTime = lf->animationTime;		// initial lerp
		} else {
			lf->frameTime = lf->oldFrameTime + anim->frameLerp;
		}
		f = ( lf->frameTime - lf->animationTime ) / anim->frameLerp;
		f *= speedScale;		// adjust for haste, etc

		numFrames = anim->numFrames;
		if (anim->flipflop) {
			numFrames *= 2;
		}
		if ( f >= numFrames ) {
			f -= numFrames;
			if ( anim->loopFrames != -1 ) //Before 0 meant no loop
			{
				if(anim->numFrames - anim->loopFrames == 0)
				{
					f %= anim->numFrames;
				}
				else
				{
					f %= (anim->numFrames - anim->loopFrames);
				}
				f += anim->loopFrames;
			} 
			else 
			{
				f = numFrames - 1;
				// the animation is stuck at the end, so it
				// can immediately transition to another sequence
				lf->frameTime = cg.time;
			}
		}
		if ( anim->reversed ) {
			lf->frame = anim->firstFrame + anim->numFrames - 1 - f;
		}
		else if (anim->flipflop && f>=anim->numFrames) {
			lf->frame = anim->firstFrame + anim->numFrames - 1 - (f%anim->numFrames);
		}
		else {
			lf->frame = anim->firstFrame + f;
		}
		if ( cg.time > lf->frameTime ) {
			lf->frameTime = cg.time;
			if ( cg_debugAnim.integer ) {
				CG_Printf( "Clamp lf->frameTime\n");
			}
		}
	}

	if ( lf->frameTime > cg.time + 200 ) {
		lf->frameTime = cg.time;
	}

	if ( lf->oldFrameTime > cg.time ) {
		lf->oldFrameTime = cg.time;
	}
	// calculate current lerp value
	if ( lf->frameTime == lf->oldFrameTime ) {
		lf->backlerp = 0;
	} else {
		lf->backlerp = 1.0 - (float)( cg.time - lf->oldFrameTime ) / ( lf->frameTime - lf->oldFrameTime );
	}
}


/*
===============
CG_ClearLerpFrame
===============
*/
static void CG_ClearLerpFrame( centity_t *cent, clientInfo_t *ci, lerpFrame_t *lf, int animationNumber, qboolean torsoOnly) {
	lf->frameTime = lf->oldFrameTime = cg.time;
	CG_SetLerpFrameAnimation( cent, ci, lf, animationNumber, 1, torsoOnly );
	lf->oldFrame = lf->frame = lf->animation->firstFrame;
}


/*
===============
CG_PlayerAnimation
===============
*/
static void CG_PlayerAnimation( centity_t *cent, int *legsOld, int *legs, float *legsBackLerp,
						int *torsoOld, int *torso, float *torsoBackLerp ) {
	clientInfo_t	*ci;
	int				clientNum;
	float			speedScale;

	clientNum = cent->currentState.clientNum;

	if ( cg_noPlayerAnims.integer ) {
		*legsOld = *legs = *torsoOld = *torso = 0;
		return;
	}

	if (cent->currentState.forcePowersActive & (1 << FP_RAGE))
	{
		speedScale = 1.3;
	}
	else if (cent->currentState.forcePowersActive & (1 << FP_SPEED))
	{
		speedScale = 1.7;
	}
	else
	{
		speedScale = 1;
	}

	ci = &cgs.clientinfo[ clientNum ];

	// do the shuffle turn frames locally
	if(0){//	if ( cent->pe.legs.yawing && ( cent->currentState.legsAnim & ~ANIM_TOGGLEBIT ) == TORSO_WEAPONREADY3) {
		CG_RunLerpFrame( cent, ci, &cent->pe.legs, LEGS_TURN1, speedScale, qfalse);
	} else {
		CG_RunLerpFrame( cent, ci, &cent->pe.legs, cent->currentState.legsAnim, speedScale, qfalse);
	}

	if (!(cent->currentState.forcePowersActive & (1 << FP_RAGE)))
	{ //don't affect torso anim speed unless raged
		speedScale = 1;
	}
	else
	{
		speedScale = 1.7;
	}

	*legsOld = cent->pe.legs.oldFrame;
	*legs = cent->pe.legs.frame;
	*legsBackLerp = cent->pe.legs.backlerp;

	CG_RunLerpFrame( cent, ci, &cent->pe.torso, cent->currentState.torsoAnim, speedScale, qtrue );

	*torsoOld = cent->pe.torso.oldFrame;
	*torso = cent->pe.torso.frame;
	*torsoBackLerp = cent->pe.torso.backlerp;
}




/*
=============================================================================

PLAYER ANGLES

=============================================================================
*/

/*
==================
CG_SwingAngles
==================
*/
static float CG_SwingAngles( float destination, float swingTolerance, float clampTolerance,
					float speed, float *angle, qboolean *swinging ) {
	float	swing;
	float	move;
	float	scale;

	if ( !*swinging ) {
		// see if a swing should be started
		swing = AngleSubtract( *angle, destination );
		if ( swing > swingTolerance || swing < -swingTolerance ) {
			*swinging = qtrue;
		}
	}

	if ( !*swinging ) {
		return 0;
	}
	
	// modify the speed depending on the delta
	// so it doesn't seem so linear
	swing = AngleSubtract( destination, *angle );
	scale = fabs( swing );
	if ( scale < swingTolerance * 0.5 ) {
		scale = 0.5;
	} else if ( scale < swingTolerance ) {
		scale = 1.0;
	} else {
		scale = 2.0;
	}

	// swing towards the destination angle
	if ( swing >= 0 ) {
		move = cg.frametime * scale * speed;
		if ( move >= swing ) {
			move = swing;
			*swinging = qfalse;
		}
		*angle = AngleMod( *angle + move );
	} else if ( swing < 0 ) {
		move = cg.frametime * scale * -speed;
		if ( move <= swing ) {
			move = swing;
			*swinging = qfalse;
		}
		*angle = AngleMod( *angle + move );
	}

	// clamp to no more than tolerance
	swing = AngleSubtract( destination, *angle );
	if ( swing > clampTolerance ) {
		*angle = AngleMod( destination - (clampTolerance - 1) );
	} else if ( swing < -clampTolerance ) {
		*angle = AngleMod( destination + (clampTolerance - 1) );
	}

	return swing;
}

/*
==================
CG_SwingAngles
==================
*/
static float CG_SwingAnglesATST( centity_t *cent, float destination, float swingTolerance, float clampTolerance,
					float speed, float *angle, qboolean *swinging ) {
	float	swing;
	float	move;
	float	scale;

	if ( !*swinging ) {
		// see if a swing should be started
		swing = AngleSubtract( *angle, destination );
		if ( swing > swingTolerance || swing < -swingTolerance ) {
			*swinging = qtrue;
		}
	}

	if ( !*swinging ) {
		return 0;
	}
	
	// modify the speed depending on the delta
	// so it doesn't seem so linear
	swing = AngleSubtract( destination, *angle );
	scale = fabs( swing );
	if ( scale < swingTolerance * 0.5 ) {
		scale = 0.5;
	} else if ( scale < swingTolerance ) {
		scale = 1.0;
	} else {
		scale = 2.0;
	}

	if (swing < 10 && swing > -10)
	{
		cent->atstSwinging = 0;
	}
	if (swing < 60 && swing > -60 && !cent->atstSwinging)
	{
		return 0;
	}

	cent->atstSwinging = 1;

	// swing towards the destination angle
	if ( swing >= 0 ) {
		move = cg.frametime * scale * speed;
		if ( move >= swing ) {
			move = swing;
			*swinging = qfalse;
		}
		*angle = AngleMod( *angle + move );
	} else if ( swing < 0 ) {
		move = cg.frametime * scale * -speed;
		if ( move <= swing ) {
			move = swing;
			*swinging = qfalse;
		}
		*angle = AngleMod( *angle + move );
	}

	// clamp to no more than tolerance
	swing = AngleSubtract( destination, *angle );
	if ( swing > clampTolerance ) {
		*angle = AngleMod( destination - (clampTolerance - 1) );
	} else if ( swing < -clampTolerance ) {
		*angle = AngleMod( destination + (clampTolerance - 1) );
	}

	return swing;
}

/*
=================
CG_AddPainTwitch
=================
*/
static void CG_AddPainTwitch( centity_t *cent, vec3_t torsoAngles ) {
	int		t;
	float	f;

	t = cg.time - cent->pe.painTime;
	if ( t >= PAIN_TWITCH_TIME ) {
		return;
	}

	f = 1.0 - (float)t / PAIN_TWITCH_TIME;

	if ( cent->pe.painDirection ) {
		torsoAngles[ROLL] += 20 * f;
	} else {
		torsoAngles[ROLL] -= 20 * f;
	}
}


typedef struct boneAngleParms_s {
	void *ghoul2;
	int modelIndex;
	char *boneName;
	vec3_t angles;
	int flags;
	int up;
	int right;
	int forward;
	qhandle_t *modelList;
	int blendTime;
	int currentTime;

	qboolean refreshSet;
} boneAngleParms_t;

boneAngleParms_t cgBoneAnglePostSet;

void CG_G2SetBoneAngles(void *ghoul2, int modelIndex, const char *boneName, const vec3_t angles, const int flags,
								const int up, const int right, const int forward, qhandle_t *modelList,
								int blendTime , int currentTime )
{ //we want to hold off on setting the bone angles until the end of the frame, because every time we set
  //them the entire skeleton has to be reconstructed.
#if 0
	//This function should ONLY be called from CG_Player() or a function that is called only within CG_Player().
	//At the end of the frame we will check to use this information to call SetBoneAngles
	memset(&cgBoneAnglePostSet, 0, sizeof(cgBoneAnglePostSet));
	cgBoneAnglePostSet.ghoul2 = ghoul2;
	cgBoneAnglePostSet.modelIndex = modelIndex;
	cgBoneAnglePostSet.boneName = (char *)boneName;

	cgBoneAnglePostSet.angles[0] = angles[0];
	cgBoneAnglePostSet.angles[1] = angles[1];
	cgBoneAnglePostSet.angles[2] = angles[2];

	cgBoneAnglePostSet.flags = flags;
	cgBoneAnglePostSet.up = up;
	cgBoneAnglePostSet.right = right;
	cgBoneAnglePostSet.forward = forward;
	cgBoneAnglePostSet.modelList = modelList;
	cgBoneAnglePostSet.blendTime = blendTime;
	cgBoneAnglePostSet.currentTime = currentTime;

	cgBoneAnglePostSet.refreshSet = qtrue;
#endif
	//Unfortunately the above doesn't really work.
	trap_G2API_SetBoneAngles(ghoul2, modelIndex, boneName, angles, flags, up, right, forward, modelList,
		blendTime, currentTime);
}

void CG_G2ClientSpineAngles( centity_t *cent, vec3_t viewAngles, const vec3_t angles, vec3_t thoracicAngles, vec3_t ulAngles, vec3_t llAngles )
{
	int ang = 0;

	if (cent->isATST || cent->currentState.teamowner)
	{
		return;
	}

	VectorClear(ulAngles);
	VectorClear(llAngles);

	//cent->pe.torso.pitchAngle = viewAngles[PITCH];
	viewAngles[YAW] = AngleDelta( cent->lerpAngles[YAW], angles[YAW] );
	//cent->pe.torso.yawAngle = viewAngles[YAW];

	if ( !BG_FlippingAnim( cent->currentState.legsAnim ) &&
		!BG_SpinningSaberAnim( cent->currentState.legsAnim ) &&
		!BG_SpinningSaberAnim( cent->currentState.torsoAnim ) &&
		!BG_InSpecialJump( cent->currentState.legsAnim ) &&
		!BG_InSpecialJump( cent->currentState.torsoAnim ) &&
		!BG_InDeathAnim(cent->currentState.legsAnim) &&
		!BG_InDeathAnim(cent->currentState.torsoAnim) &&
		!CG_InRoll(cent) &&
		!CG_InRollAnim(cent) &&
		!BG_SaberInSpecial(cent->currentState.saberMove) &&
		!BG_SaberInSpecialAttack(cent->currentState.torsoAnim) &&
		!BG_SaberInSpecialAttack(cent->currentState.legsAnim) &&

		!BG_FlippingAnim( cgs.clientinfo[cent->currentState.number].legsAnim ) &&
		!BG_SpinningSaberAnim( cgs.clientinfo[cent->currentState.number].legsAnim ) &&
		!BG_SpinningSaberAnim( cgs.clientinfo[cent->currentState.number].torsoAnim ) &&
		!BG_InSpecialJump( cgs.clientinfo[cent->currentState.number].legsAnim ) &&
		!BG_InSpecialJump( cgs.clientinfo[cent->currentState.number].torsoAnim ) &&
		!BG_InDeathAnim(cgs.clientinfo[cent->currentState.number].legsAnim) &&
		!BG_InDeathAnim(cgs.clientinfo[cent->currentState.number].torsoAnim) &&
		!BG_SaberInSpecialAttack(cgs.clientinfo[cent->currentState.number].torsoAnim) &&
		!BG_SaberInSpecialAttack(cgs.clientinfo[cent->currentState.number].legsAnim) &&

		/*
		!BG_FlippingAnim( cent->rootBone ) &&
		!BG_SpinningSaberAnim( cent->rootBone ) &&
		!BG_InSpecialJump( cent->rootBone ) &&
		!BG_InDeathAnim(cent->rootBone) &&
		!BG_SaberInSpecialAttack(cent->rootBone) &&
		*/

		!(cent->currentState.eFlags & EF_DEAD) )
	{
		//adjust for motion offset
		mdxaBone_t	boltMatrix;
		vec3_t		motionFwd, motionAngles;

		//trap_G2API_GetBoltMatrix( cent->ghoul2, 0, cgs.clientinfo[cent->currentState.number].bolt_motion, &boltMatrix, vec3_origin, cent->lerpOrigin, cg.time, /*cgs.gameModels*/0, cent->modelScale);
		trap_G2API_GetBoltMatrix_NoReconstruct( cent->ghoul2, 0, cgs.clientinfo[cent->currentState.number].bolt_motion, &boltMatrix, vec3_origin, cent->lerpOrigin, cg.time, /*cgs.gameModels*/0, cent->modelScale);
	//	trap_G2API_GiveMeVectorFromMatrix( &boltMatrix, POSITIVE_X, motionFwd );
		//trap_G2API_GiveMeVectorFromMatrix( &boltMatrix, POSITIVE_Y, motionFwd );
		trap_G2API_GiveMeVectorFromMatrix( &boltMatrix, NEGATIVE_Y, motionFwd );

		vectoangles( motionFwd, motionAngles );
		for ( ang = 0; ang < 3; ang++ )
		{
			viewAngles[ang] = AngleNormalize180( viewAngles[ang] - AngleNormalize180( motionAngles[ang] ) );
		}

		//Using NEGATIVE_Y and subtractinging 90 seems to magically fix our horrible contortion issues.
		//SP actually just uses NEGATIVE_Y without this. Unfortunately we have some sort of worthless
		//chunk of code in our GBM function that rotates the entire matrix 90 degrees before returning
		//a "proper" direction. SP does not have this. And I am not even going to consider changing it at
		//this point to match.
		//Com_Printf("Comp: %f %f %f\n", viewAngles[0], viewAngles[1], viewAngles[2]);

		if (viewAngles[YAW] < -90)
		{
			viewAngles[YAW] += 360;
		}

		viewAngles[YAW] -= 90;
	}
	//distribute the angles differently up the spine
	//NOTE: each of these distributions must add up to 1.0f
	thoracicAngles[PITCH] = 0;//viewAngles[PITCH]*0.20f;
	llAngles[PITCH] = 0;//viewAngles[PITCH]*0.40f;
	ulAngles[PITCH] = 0;//viewAngles[PITCH]*0.40f;

	thoracicAngles[YAW] = viewAngles[YAW]*0.20f - (viewAngles[PITCH]*(viewAngles[YAW]*.020f));
	ulAngles[YAW] = viewAngles[YAW]*0.25f - (viewAngles[PITCH]*(viewAngles[YAW]*.0005f));
	llAngles[YAW] = viewAngles[YAW]*0.25f - (viewAngles[PITCH]*(viewAngles[YAW]*.0005f));

	if (thoracicAngles[YAW] > 20)
	{
		thoracicAngles[YAW] = 20;
	}
	if (ulAngles[YAW] > 20)
	{
		ulAngles[YAW] = 20;
	}
	if (llAngles[YAW] > 20)
	{
		llAngles[YAW] = 20;
	}

	thoracicAngles[ROLL] = viewAngles[ROLL]*0.20f;
	ulAngles[ROLL] = viewAngles[ROLL]*0.35f;
	llAngles[ROLL] = viewAngles[ROLL]*0.45f;
	
	for ( ang = 0; ang < 3; ang++ )
	{
		if (ulAngles[ang] < 0)
		{
			ulAngles[ang] += 360;
		}
	}

	//thoracic is added modified again by neckAngle calculations, so don't set it until then
//	BG_G2SetBoneAngles( cent, cent->gent, cent->gent->upperLumbarBone, ulAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.model_draw); 
//	BG_G2SetBoneAngles( cent, cent->gent, cent->gent->lowerLumbarBone, llAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.model_draw); 

//	trap_G2API_SetBoneAngles(cent->ghoul2, 0, "upper_lumbar", ulAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 
//	trap_G2API_SetBoneAngles(cent->ghoul2, 0, "lower_lumbar", llAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 
//	trap_G2API_SetBoneAngles(cent->ghoul2, 0, "thoracic", thoracicAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 
}

static void CG_G2PlayerAngles( centity_t *cent, vec3_t legs[3], vec3_t legsAngles){
	vec3_t		torsoAngles, headAngles;
	float		dest;
	static	int	movementOffsets[8] = { 0, 22, 45, -22, 0, 22, -45, -22 };
	vec3_t		velocity;
	float		speed; //, speed_dif, speed_desired;
	int			dir;
	vec3_t		velPos, velAng;
	int			adddir = 0;
	float		dif;
	float		degrees_negative = 0;
	float		degrees_positive = 0;
	vec3_t		ulAngles, llAngles, viewAngles, angles, thoracicAngles = {0,0,0};

	VectorCopy( cent->lerpAngles, headAngles );
	headAngles[YAW] = AngleMod( headAngles[YAW] );
	VectorClear( legsAngles );
	VectorClear( torsoAngles );

	// --------- yaw -------------

	// allow yaw to drift a bit
	if ((( cent->currentState.legsAnim & ~ANIM_TOGGLEBIT ) != BOTH_STAND1) || 
			( cent->currentState.torsoAnim & ~ANIM_TOGGLEBIT ) != WeaponReadyAnim[cent->currentState.weapon]  ) 
	{
		// if not standing still, always point all in the same direction
		cent->pe.torso.yawing = qtrue;	// always center
		cent->pe.torso.pitching = qtrue;	// always center
		cent->pe.legs.yawing = qtrue;	// always center
	}

	// adjust legs for movement dir
	if ( cent->currentState.eFlags & EF_DEAD ) {
		// don't let dead bodies twitch
		dir = 0;
	} else {
		dir = cent->currentState.angles2[YAW];
		if ( dir < 0 || dir > 7 ) {
			CG_Error( "Bad player movement angle" );
		}
	}
	if (!cent->isATST)
	{
		torsoAngles[YAW] = headAngles[YAW] + 0.25 * movementOffsets[ dir ];
	}
	else
	{
		torsoAngles[YAW] = headAngles[YAW];
	}

	// torso
//	CG_SwingAngles( torsoAngles[YAW], 25, 90, /*cg_swingSpeed.value*/ 0.3, &cent->pe.torso.yawAngle, &cent->pe.torso.yawing );
//	torsoAngles[YAW] = cent->pe.torso.yawAngle;

	//for now, turn torso instantly and let the legs swing to follow
	cent->pe.torso.yawAngle = torsoAngles[YAW];

	// --------- pitch -------------

	VectorCopy( cent->currentState.pos.trDelta, velocity );
	speed = VectorNormalize( velocity );

	if (!speed)
	{
		torsoAngles[YAW] = headAngles[YAW];
	}

	// only show a fraction of the pitch angle in the torso
	if ( headAngles[PITCH] > 180 ) {
		dest = (-360 + headAngles[PITCH]) * 0.75;
	} else {
		dest = headAngles[PITCH] * 0.75;
	}
	CG_SwingAngles( dest, 15, 30, 0.1, &cent->pe.torso.pitchAngle, &cent->pe.torso.pitching );
	torsoAngles[PITCH] = cent->pe.torso.pitchAngle;

	// --------- roll -------------


	// lean towards the direction of travel

	/*
	speed_desired = cent->currentState.speed/4;

	if (!speed)
	{
		speed_dif = 0;
	}
	else
	{
		speed_dif = (speed/speed_desired);
	}

	if (speed_dif > 1)
	{
		speed_dif = 1;
	}
	else if (speed_dif < 0)
	{
		speed_dif = 0;
	}
	*/
	if ( speed ) {
		vec3_t	axis[3];
		float	side;

		speed *= 0.05;

		AnglesToAxis( legsAngles, axis );
		side = speed * DotProduct( velocity, axis[1] );
		legsAngles[ROLL] -= side;

		side = speed * DotProduct( velocity, axis[0] );
		legsAngles[PITCH] += side;
	}

	//legsAngles[YAW] = headAngles[YAW] + (movementOffsets[ dir ]*speed_dif);

	//rww - crazy velocity-based leg angle calculation
	legsAngles[YAW] = headAngles[YAW];
	velPos[0] = cent->lerpOrigin[0] + velocity[0];
	velPos[1] = cent->lerpOrigin[1] + velocity[1];
	velPos[2] = cent->lerpOrigin[2];// + velocity[2];

	if (cent->currentState.groundEntityNum == ENTITYNUM_NONE)
	{ //off the ground, no direction-based leg angles
		VectorCopy(cent->lerpOrigin, velPos);
	}

	VectorSubtract(cent->lerpOrigin, velPos, velAng);

	if (!VectorCompare(velAng, vec3_origin))
	{
		vectoangles(velAng, velAng);

		if (velAng[YAW] <= legsAngles[YAW])
		{
			degrees_negative = (legsAngles[YAW] - velAng[YAW]);
			degrees_positive = (360 - legsAngles[YAW]) + velAng[YAW];
		}
		else
		{
			degrees_negative = legsAngles[YAW] + (360 - velAng[YAW]);
			degrees_positive = (velAng[YAW] - legsAngles[YAW]);
		}

		if (degrees_negative < degrees_positive)
		{
			dif = degrees_negative;
			adddir = 0;
		}
		else
		{
			dif = degrees_positive;
			adddir = 1;
		}

		if (dif > 90)
		{
			dif = (180 - dif);
		}

		if (cent->isATST)
		{
			if (dif > 360)
			{
				dif = 360;
			}
		}
		else
		{
			if (dif > 60)
			{
				dif = 60;
			}
		}

		//Slight hack for when playing is running backward
		if (dir == 3 || dir == 5)
		{
			dif = -dif;
		}

		if (adddir)
		{
			legsAngles[YAW] -= dif;
		}
		else
		{
			legsAngles[YAW] += dif;
		}
	}

	if (cent->isATST)
	{
		cent->pe.legs.yawSwingDif = CG_SwingAnglesATST( cent, legsAngles[YAW], 40, 360, /*cg_swingSpeed.value*/ 0.05, &cent->pe.legs.yawAngle, &cent->pe.legs.yawing );
	}
	else
	{
		if (!cg_swingAngles.integer)
		{
			cent->pe.legs.yawAngle = legsAngles[YAW];
		}
		else
		{
			CG_SwingAngles( legsAngles[YAW], 40, 90, /*cg_swingSpeed.value*/ 0.3, &cent->pe.legs.yawAngle, &cent->pe.legs.yawing );
		}
	}
	legsAngles[YAW] = cent->pe.legs.yawAngle;

	// pain twitch
	CG_AddPainTwitch( cent, torsoAngles );

	legsAngles[ROLL] = 0;
	torsoAngles[ROLL] = 0;

	VectorCopy(legsAngles, cent->turAngles);

	if (cent->isATST)
	{
		legsAngles[ROLL] = 0;
		legsAngles[PITCH] = 0;
	}

	// pull the angles back out of the hierarchial chain
	AnglesSubtract( headAngles, torsoAngles, headAngles );
	AnglesSubtract( torsoAngles, legsAngles, torsoAngles );
	AnglesToAxis( legsAngles, legs );
	// we assume that model 0 is the player model.

	if (cent->isATST)
	{
		vec3_t flatYaw;

		flatYaw[YAW] = 0;//cent->lerpAngles[YAW];
		flatYaw[ROLL] = 0;
		flatYaw[PITCH] = 0;
		CG_G2SetBoneAngles(cent->ghoul2, 0, "pelvis", flatYaw, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 

		CG_G2SetBoneAngles(cent->ghoul2, 0, "thoracic", torsoAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 

		return;
	}

//	trap_G2API_SetBoneAngles(cent->ghoul2, 0, "upper_lumbar", torsoAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 

	VectorCopy( cent->lerpAngles, viewAngles );
	viewAngles[YAW] = viewAngles[ROLL] = 0;
	viewAngles[PITCH] *= 0.5;

	VectorCopy( cent->lerpAngles, angles );
	angles[PITCH] = 0;

	CG_G2ClientSpineAngles(cent, viewAngles, angles, thoracicAngles, ulAngles, llAngles);

	ulAngles[YAW] += torsoAngles[YAW]*0.3;
	llAngles[YAW] += torsoAngles[YAW]*0.3;
	thoracicAngles[YAW] += torsoAngles[YAW]*0.4;

	ulAngles[PITCH] = torsoAngles[PITCH]*0.3;
	llAngles[PITCH] = torsoAngles[PITCH]*0.3;
	thoracicAngles[PITCH] = torsoAngles[PITCH]*0.4;

	ulAngles[ROLL] += torsoAngles[ROLL]*0.3;
	llAngles[ROLL] += torsoAngles[ROLL]*0.3;
	thoracicAngles[ROLL] += torsoAngles[ROLL]*0.4;

	if ( cent->currentState.otherEntityNum2 && !(cent->currentState.eFlags & EF_DEAD) )
	{ //using an emplaced gun
		centity_t *empEnt = &cg_entities[cent->currentState.otherEntityNum2];
		vec3_t headAng;

		VectorClear(headAng);

		if (empEnt)
		{
			vec3_t dif, empOrg, originalAngle;

			VectorCopy(cent->lerpAngles, originalAngle);

			VectorCopy(empEnt->lerpOrigin, empOrg);

			empOrg[2] += 32;

			VectorSubtract(empOrg, cent->lerpOrigin, dif);
			VectorNormalize(dif);
			vectoangles(dif, dif);
			
			VectorCopy(dif, cent->lerpAngles);

			VectorCopy(cent->lerpAngles, legsAngles);

			VectorSubtract(originalAngle, cent->lerpAngles, headAng);

			headAng[PITCH] /= 3; //scale it down so that you can't bend your neck all the way back
		}
		else
		{
			VectorClear(cent->lerpAngles);
			VectorClear(legsAngles);
		}

		AnglesToAxis( legsAngles, legs );

		CG_G2SetBoneAngles(cent->ghoul2, 0, "upper_lumbar", vec3_origin, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 
		CG_G2SetBoneAngles(cent->ghoul2, 0, "lower_lumbar", vec3_origin, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 
		CG_G2SetBoneAngles(cent->ghoul2, 0, "thoracic", vec3_origin, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 

		CG_G2SetBoneAngles(cent->ghoul2, 0, "cervical", headAng, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time);
	}
	else
	{
		CG_G2SetBoneAngles(cent->ghoul2, 0, "upper_lumbar", ulAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 
		CG_G2SetBoneAngles(cent->ghoul2, 0, "lower_lumbar", llAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 
		CG_G2SetBoneAngles(cent->ghoul2, 0, "thoracic", thoracicAngles, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time); 
		CG_G2SetBoneAngles(cent->ghoul2, 0, "cervical", vec3_origin, BONE_ANGLES_POSTMULT, POSITIVE_X, NEGATIVE_Y, NEGATIVE_Z, cgs.gameModels, 0, cg.time);
	}

	//trap_G2API_SetBoneAngles(cent->ghoul2, 0, "cranium", headAngles, BONE_ANGLES_POSTMULT, POSITIVE_Z, NEGATIVE_Y, POSITIVE_X, cgs.gameModels, 0, cg.time); 
}
//==========================================================================

/*
===============
CG_HasteTrail
===============
*/
static void CG_HasteTrail( centity_t *cent ) {
	localEntity_t	*smoke;
	vec3_t			origin;
	int				anim;

	if ( cent->trailTime > cg.time ) {
		return;
	}
	anim = cent->pe.legs.animationNumber & ~ANIM_TOGGLEBIT;
	if ( anim != BOTH_RUN1 && anim != BOTH_RUNBACK1 ) {
		return;
	}

	cent->trailTime += 100;
	if ( cent->trailTime < cg.time ) {
		cent->trailTime = cg.time;
	}

	VectorCopy( cent->lerpOrigin, origin );
	origin[2] -= 16;

	smoke = CG_SmokePuff( origin, vec3_origin, 
				  8, 
				  1, 1, 1, 1,
				  500, 
				  cg.time,
				  0,
				  0,
				  cgs.media.hastePuffShader );

	// use the optimized local entity add
	smoke->leType = LE_SCALE_FADE;
}

/*
===============
CG_DustTrail
===============
*/
/*
static void CG_DustTrail( centity_t *cent ) {
	int				anim;
	localEntity_t	*dust;
	vec3_t end, vel;
	trace_t tr;

	if (!cg_enableDust.integer)
		return;

	if ( cent->dustTrailTime > cg.time ) {
		return;
	}

	anim = cent->pe.legs.animationNumber & ~ANIM_TOGGLEBIT;
	if ( anim != LEGS_LANDB && anim != LEGS_LAND ) {
		return;
	}

	cent->dustTrailTime += 40;
	if ( cent->dustTrailTime < cg.time ) {
		cent->dustTrailTime = cg.time;
	}

	VectorCopy(cent->currentState.pos.trBase, end);
	end[2] -= 64;
	CG_Trace( &tr, cent->currentState.pos.trBase, NULL, NULL, end, cent->currentState.number, MASK_PLAYERSOLID );

	if ( !(tr.surfaceFlags & SURF_DUST) )
		return;

	VectorCopy( cent->currentState.pos.trBase, end );
	end[2] -= 16;

	VectorSet(vel, 0, 0, -30);
	dust = CG_SmokePuff( end, vel,
				  24,
				  .8f, .8f, 0.7f, 0.33f,
				  500,
				  cg.time,
				  0,
				  0,
				  cgs.media.dustPuffShader );
}
*/

/*
===============
CG_TrailItem
===============
*/
#if 0
static void CG_TrailItem( centity_t *cent, qhandle_t hModel ) {
	refEntity_t		ent;
	vec3_t			angles;
	vec3_t			axis[3];

	VectorCopy( cent->lerpAngles, angles );
	angles[PITCH] = 0;
	angles[ROLL] = 0;
	AnglesToAxis( angles, axis );

	memset( &ent, 0, sizeof( ent ) );
	VectorMA( cent->lerpOrigin, -16, axis[0], ent.origin );
	ent.origin[2] += 16;
	angles[YAW] += 90;
	AnglesToAxis( angles, ent.axis );

	ent.hModel = hModel;
	trap_R_AddRefEntityToScene( &ent );
}
#endif


/*
===============
CG_PlayerFlag
===============
*/
static void CG_PlayerFlag( centity_t *cent, qhandle_t hModel ) {
	refEntity_t		ent;
	vec3_t			angles;
	vec3_t			axis[3];
	vec3_t			boltOrg, tAng, getAng, right;
	mdxaBone_t		boltMatrix;

	if (cent->currentState.number == cg.snap->ps.clientNum &&
		!cg.renderingThirdPerson)
	{
		return;
	}

	if (!cent->ghoul2)
	{
		return;
	}

	VectorSet( tAng, cent->turAngles[PITCH], cent->turAngles[YAW], cent->turAngles[ROLL] );

	trap_G2API_GetBoltMatrix(cent->ghoul2, 0, cgs.clientinfo[cent->currentState.number].bolt_llumbar, &boltMatrix, tAng, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale);
	trap_G2API_GiveMeVectorFromMatrix(&boltMatrix, ORIGIN, boltOrg);

	trap_G2API_GiveMeVectorFromMatrix(&boltMatrix, POSITIVE_X, tAng);
	vectoangles(tAng, tAng);

	VectorCopy(cent->lerpAngles, angles);

	boltOrg[2] -= 12;
	VectorSet(getAng, 0, cent->lerpAngles[1], 0);
	AngleVectors(getAng, 0, right, 0);
	boltOrg[0] += right[0]*8;
	boltOrg[1] += right[1]*8;
	boltOrg[2] += right[2]*8;

	angles[PITCH] = -cent->lerpAngles[PITCH]/2-30;
	angles[YAW] = tAng[YAW]+270;

	AnglesToAxis(angles, axis);

	memset( &ent, 0, sizeof( ent ) );
	VectorMA( boltOrg, 24, axis[0], ent.origin );

	angles[ROLL] += 20;
	AnglesToAxis( angles, ent.axis );

	ent.hModel = hModel;

	ent.modelScale[0] = 0.5;
	ent.modelScale[1] = 0.5;
	ent.modelScale[2] = 0.5;
	ScaleModelAxis(&ent);

	/*
	if (cent->currentState.number == cg.snap->ps.clientNum)
	{ //If we're the current client (in third person), render the flag on our back transparently
		ent.renderfx |= RF_FORCE_ENT_ALPHA;
		ent.shaderRGBA[3] = 100;
	}
	*/
	//FIXME: Not doing this at the moment because sorting totally messes up

	trap_R_AddRefEntityToScene( &ent );
}


/*
===============
CG_PlayerPowerups
===============
*/
static void CG_PlayerPowerups( centity_t *cent, refEntity_t *torso ) {
	int		powerups;
	clientInfo_t	*ci;

	powerups = cent->currentState.powerups;
	if ( !powerups ) {
		return;
	}

	// quad gives a dlight
	if ( powerups & ( 1 << PW_QUAD ) ) {
		trap_R_AddLightToScene( cent->lerpOrigin, 200 + (rand()&31), 0.2f, 0.2f, 1 );
	}

	ci = &cgs.clientinfo[ cent->currentState.clientNum ];
	// redflag
	if ( powerups & ( 1 << PW_REDFLAG ) ) {
		CG_PlayerFlag( cent, cgs.media.redFlagModel );
		trap_R_AddLightToScene( cent->lerpOrigin, 200 + (rand()&31), 1.0, 0.2f, 0.2f );
	}

	// blueflag
	if ( powerups & ( 1 << PW_BLUEFLAG ) ) {
		CG_PlayerFlag( cent, cgs.media.blueFlagModel );
		trap_R_AddLightToScene( cent->lerpOrigin, 200 + (rand()&31), 0.2f, 0.2f, 1.0 );
	}

	// neutralflag
	if ( powerups & ( 1 << PW_NEUTRALFLAG ) ) {
		trap_R_AddLightToScene( cent->lerpOrigin, 200 + (rand()&31), 1.0, 1.0, 1.0 );
	}

	// haste leaves smoke trails
	if ( powerups & ( 1 << PW_HASTE ) ) {
		CG_HasteTrail( cent );
	}
}


/*
===============
CG_PlayerFloatSprite

Float a sprite over the player's head
===============
*/
static void CG_PlayerFloatSprite( centity_t *cent, qhandle_t shader ) {
	int				rf;
	refEntity_t		ent;

	if ( cent->currentState.number == cg.snap->ps.clientNum && !cg.renderingThirdPerson ) {
		rf = RF_THIRD_PERSON;		// only show in mirrors
	} else {
		rf = 0;
	}

	memset( &ent, 0, sizeof( ent ) );
	VectorCopy( cent->lerpOrigin, ent.origin );
	ent.origin[2] += 48;
	ent.reType = RT_SPRITE;
	ent.customShader = shader;
	ent.radius = 10;
	ent.renderfx = rf;
	ent.shaderRGBA[0] = 255;
	ent.shaderRGBA[1] = 255;
	ent.shaderRGBA[2] = 255;
	ent.shaderRGBA[3] = 255;
	trap_R_AddRefEntityToScene( &ent );
}



/*
===============
CG_PlayerFloatSprite

Same as above but allows custom RGBA values
===============
*/
#if 0
static void CG_PlayerFloatSpriteRGBA( centity_t *cent, qhandle_t shader, vec4_t rgba ) {
	int				rf;
	refEntity_t		ent;

	if ( cent->currentState.number == cg.snap->ps.clientNum && !cg.renderingThirdPerson ) {
		rf = RF_THIRD_PERSON;		// only show in mirrors
	} else {
		rf = 0;
	}

	memset( &ent, 0, sizeof( ent ) );
	VectorCopy( cent->lerpOrigin, ent.origin );
	ent.origin[2] += 48;
	ent.reType = RT_SPRITE;
	ent.customShader = shader;
	ent.radius = 10;
	ent.renderfx = rf;
	ent.shaderRGBA[0] = rgba[0];
	ent.shaderRGBA[1] = rgba[1];
	ent.shaderRGBA[2] = rgba[2];
	ent.shaderRGBA[3] = rgba[3];
	trap_R_AddRefEntityToScene( &ent );
}
#endif


/*
===============
CG_PlayerShadow

Returns the Z component of the surface being shadowed

  should it return a full plane instead of a Z?
===============
*/
#define	SHADOW_DISTANCE		128
static qboolean CG_PlayerShadow( centity_t *cent, float *shadowPlane ) {
	vec3_t		end, mins = {-15, -15, 0}, maxs = {15, 15, 2};
	trace_t		trace;
	float		alpha;

	*shadowPlane = 0;

	if ( cg_shadows.integer == 0 ) {
		return qfalse;
	}

	if (cent->currentState.eFlags & EF_DEAD)
	{
		return qfalse;
	}

	if (CG_IsMindTricked(cent->currentState.trickedentindex,
		cent->currentState.trickedentindex2,
		cent->currentState.trickedentindex3,
		cent->currentState.trickedentindex4,
		cg.snap->ps.clientNum))
	{
		return qfalse; //this entity is mind-tricking the current client, so don't render it
	}

	// send a trace down from the player to the ground
	VectorCopy( cent->lerpOrigin, end );
	end[2] -= SHADOW_DISTANCE;

	trap_CM_BoxTrace( &trace, cent->lerpOrigin, end, mins, maxs, 0, MASK_PLAYERSOLID );

	// no shadow if too high
	if ( trace.fraction == 1.0 || trace.startsolid || trace.allsolid ) {
		return qfalse;
	}

	*shadowPlane = trace.endpos[2] + 1;

	if ( cg_shadows.integer != 1 ) {	// no mark for stencil or projection shadows
		return qtrue;
	}

	// fade the shadow out with height
	alpha = 1.0 - trace.fraction;

	// bk0101022 - hack / FPE - bogus planes?
	//assert( DotProduct( trace.plane.normal, trace.plane.normal ) != 0.0f ) 

	// add the mark as a temporary, so it goes directly to the renderer
	// without taking a spot in the cg_marks array
	CG_ImpactMark( cgs.media.shadowMarkShader, trace.endpos, trace.plane.normal, 
		cent->pe.legs.yawAngle, alpha,alpha,alpha,1, qfalse, 24, qtrue );

	return qtrue;
}


/*
===============
CG_PlayerSplash

Draw a mark at the water surface
===============
*/
static void CG_PlayerSplash( centity_t *cent ) {
	vec3_t		start, end;
	trace_t		trace;
	int			contents;
	polyVert_t	verts[4];

	if ( !cg_shadows.integer ) {
		return;
	}

	VectorCopy( cent->lerpOrigin, end );
	end[2] -= 24;

	// if the feet aren't in liquid, don't make a mark
	// this won't handle moving water brushes, but they wouldn't draw right anyway...
	contents = trap_CM_PointContents( end, 0 );
	if ( !( contents & ( CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA ) ) ) {
		return;
	}

	VectorCopy( cent->lerpOrigin, start );
	start[2] += 32;

	// if the head isn't out of liquid, don't make a mark
	contents = trap_CM_PointContents( start, 0 );
	if ( contents & ( CONTENTS_SOLID | CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA ) ) {
		return;
	}

	// trace down to find the surface
	trap_CM_BoxTrace( &trace, start, end, NULL, NULL, 0, ( CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA ) );

	if ( trace.fraction == 1.0 ) {
		return;
	}

	// create a mark polygon
	VectorCopy( trace.endpos, verts[0].xyz );
	verts[0].xyz[0] -= 32;
	verts[0].xyz[1] -= 32;
	verts[0].st[0] = 0;
	verts[0].st[1] = 0;
	verts[0].modulate[0] = 255;
	verts[0].modulate[1] = 255;
	verts[0].modulate[2] = 255;
	verts[0].modulate[3] = 255;

	VectorCopy( trace.endpos, verts[1].xyz );
	verts[1].xyz[0] -= 32;
	verts[1].xyz[1] += 32;
	verts[1].st[0] = 0;
	verts[1].st[1] = 1;
	verts[1].modulate[0] = 255;
	verts[1].modulate[1] = 255;
	verts[1].modulate[2] = 255;
	verts[1].modulate[3] = 255;

	VectorCopy( trace.endpos, verts[2].xyz );
	verts[2].xyz[0] += 32;
	verts[2].xyz[1] += 32;
	verts[2].st[0] = 1;
	verts[2].st[1] = 1;
	verts[2].modulate[0] = 255;
	verts[2].modulate[1] = 255;
	verts[2].modulate[2] = 255;
	verts[2].modulate[3] = 255;

	VectorCopy( trace.endpos, verts[3].xyz );
	verts[3].xyz[0] += 32;
	verts[3].xyz[1] -= 32;
	verts[3].st[0] = 1;
	verts[3].st[1] = 0;
	verts[3].modulate[0] = 255;
	verts[3].modulate[1] = 255;
	verts[3].modulate[2] = 255;
	verts[3].modulate[3] = 255;

	trap_R_AddPolyToScene( cgs.media.wakeMarkShader, 4, verts );
}

void CG_ForcePushBlur( vec3_t org )
{
	localEntity_t	*ex;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_PUFF;
	ex->refEntity.reType = RT_SPRITE;
	ex->radius = 2.0f;
	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 120;
	VectorCopy( org, ex->pos.trBase );
	ex->pos.trTime = cg.time;
	ex->pos.trType = TR_LINEAR;
	VectorScale( cg.refdef.viewaxis[1], 55, ex->pos.trDelta );
		
	ex->color[0] = 24;
	ex->color[1] = 32;
	ex->color[2] = 40;
	ex->refEntity.customShader = trap_R_RegisterShader( "gfx/effects/forcePush" );

	ex = CG_AllocLocalEntity();
	ex->leType = LE_PUFF;
	ex->refEntity.reType = RT_SPRITE;
	ex->refEntity.rotation = 180.0f;
	ex->radius = 2.0f;
	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 120;
	VectorCopy( org, ex->pos.trBase );
	ex->pos.trTime = cg.time;
	ex->pos.trType = TR_LINEAR;
	VectorScale( cg.refdef.viewaxis[1], -55, ex->pos.trDelta );
		
	ex->color[0] = 24;
	ex->color[1] = 32;
	ex->color[2] = 40;
	ex->refEntity.customShader = trap_R_RegisterShader( "gfx/effects/forcePush" );
}

void CG_ForceGripEffect( vec3_t org )
{
	localEntity_t	*ex;
	float wv = sin( cg.time * 0.004f ) * 0.08f + 0.1f;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_PUFF;
	ex->refEntity.reType = RT_SPRITE;
	ex->radius = 2.0f;
	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 120;
	VectorCopy( org, ex->pos.trBase );
	ex->pos.trTime = cg.time;
	ex->pos.trType = TR_LINEAR;
	VectorScale( cg.refdef.viewaxis[1], 55, ex->pos.trDelta );
		
	ex->color[0] = 200+((wv*255));
	if (ex->color[0] > 255)
	{
		ex->color[0] = 255;
	}
	ex->color[1] = 0;
	ex->color[2] = 0;
	ex->refEntity.customShader = trap_R_RegisterShader( "gfx/effects/forcePush" );

	ex = CG_AllocLocalEntity();
	ex->leType = LE_PUFF;
	ex->refEntity.reType = RT_SPRITE;
	ex->refEntity.rotation = 180.0f;
	ex->radius = 2.0f;
	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 120;
	VectorCopy( org, ex->pos.trBase );
	ex->pos.trTime = cg.time;
	ex->pos.trType = TR_LINEAR;
	VectorScale( cg.refdef.viewaxis[1], -55, ex->pos.trDelta );

	/*
	ex->color[0] = 200+((wv*255));
	if (ex->color[0] > 255)
	{
		ex->color[0] = 255;
	}
	*/
	ex->color[0] = 255;
	ex->color[1] = 255;
	ex->color[2] = 255;
	ex->refEntity.customShader = cgs.media.redSaberGlowShader;//trap_R_RegisterShader( "gfx/effects/forcePush" );
}


/*
===============
CG_AddRefEntityWithPowerups

Adds a piece with modifications or duplications for powerups
Also called by CG_Missile for quad rockets, but nobody can tell...
===============
*/
void CG_AddRefEntityWithPowerups( refEntity_t *ent, entityState_t *state, int team ) {

	if (CG_IsMindTricked(state->trickedentindex,
		state->trickedentindex2,
		state->trickedentindex3,
		state->trickedentindex4,
		cg.snap->ps.clientNum))
	{
		return; //this entity is mind-tricking the current client, so don't render it
	}

	trap_R_AddRefEntityToScene( ent );

	if ( state->powerups & ( 1 << PW_QUAD ) )
	{
		if (team == TEAM_RED)
			ent->customShader = cgs.media.redQuadShader;
		else
			ent->customShader = cgs.media.quadShader;
		trap_R_AddRefEntityToScene( ent );
	}
	if ( state->powerups & ( 1 << PW_BATTLESUIT ) ) {
		ent->customShader = cgs.media.battleSuitShader;
		trap_R_AddRefEntityToScene( ent );
	}
}

#define MAX_SHIELD_TIME	2000.0
#define MIN_SHIELD_TIME	2000.0


void CG_PlayerShieldHit(int entitynum, vec3_t dir, int amount)
{
	centity_t *cent;
	int	time;

	if (entitynum<0 || entitynum >= MAX_CLIENTS)
	{
		return;
	}

	cent = &cg_entities[entitynum];

	if (amount > 100)
	{
		time = cg.time + MAX_SHIELD_TIME;		// 2 sec.
	}
	else
	{
		time = cg.time + 500 + amount*15;
	}

	if (time > cent->damageTime)
	{
		cent->damageTime = time;
		VectorScale(dir, -1, dir);
		vectoangles(dir, cent->damageAngles);
	}
}


void CG_DrawPlayerShield(centity_t *cent, vec3_t origin)
{
	refEntity_t ent;
	int			alpha;
	float		scale;
	
	// Don't draw the shield when the player is dead.
	if (cent->currentState.eFlags & EF_DEAD)
	{
		return;
	}

	memset( &ent, 0, sizeof( ent ) );

	VectorCopy( origin, ent.origin );
	ent.origin[2] += 10.0;
	AnglesToAxis( cent->damageAngles, ent.axis );

	alpha = 255.0 * ((cent->damageTime - cg.time) / MIN_SHIELD_TIME) + random()*16;
	if (alpha>255)
		alpha=255;

	// Make it bigger, but tighter if more solid
	scale = 1.4 - ((float)alpha*(0.4/255.0));		// Range from 1.0 to 1.4
	VectorScale( ent.axis[0], scale, ent.axis[0] );
	VectorScale( ent.axis[1], scale, ent.axis[1] );
	VectorScale( ent.axis[2], scale, ent.axis[2] );

	ent.hModel = cgs.media.halfShieldModel;
	ent.customShader = cgs.media.halfShieldShader;
	ent.shaderRGBA[0] = alpha;
	ent.shaderRGBA[1] = alpha;
	ent.shaderRGBA[2] = alpha;
	ent.shaderRGBA[3] = 255;
	trap_R_AddRefEntityToScene( &ent );
}


void CG_PlayerHitFX(centity_t *cent)
{
	centity_t *curent;

	// only do the below fx if the cent in question is...uh...me, and it's first person.
	if (cent->currentState.clientNum != cg.predictedPlayerState.clientNum || cg.renderingThirdPerson)
	{
		// Get the NON-PREDICTED player entity, because the predicted one doesn't have the damage info on it.
		curent = &cg_entities[cent->currentState.number];

		if (curent->damageTime > cg.time)
		{
			CG_DrawPlayerShield(curent, cent->lerpOrigin);
		}

		return;
	}
}



/*
=================
CG_LightVerts
=================
*/
int CG_LightVerts( vec3_t normal, int numVerts, polyVert_t *verts )
{
	int				i, j;
	float			incoming;
	vec3_t			ambientLight;
	vec3_t			lightDir;
	vec3_t			directedLight;

	trap_R_LightForPoint( verts[0].xyz, ambientLight, directedLight, lightDir );

	for (i = 0; i < numVerts; i++) {
		incoming = DotProduct (normal, lightDir);
		if ( incoming <= 0 ) {
			verts[i].modulate[0] = ambientLight[0];
			verts[i].modulate[1] = ambientLight[1];
			verts[i].modulate[2] = ambientLight[2];
			verts[i].modulate[3] = 255;
			continue;
		} 
		j = ( ambientLight[0] + incoming * directedLight[0] );
		if ( j > 255 ) {
			j = 255;
		}
		verts[i].modulate[0] = j;

		j = ( ambientLight[1] + incoming * directedLight[1] );
		if ( j > 255 ) {
			j = 255;
		}
		verts[i].modulate[1] = j;

		j = ( ambientLight[2] + incoming * directedLight[2] );
		if ( j > 255 ) {
			j = 255;
		}
		verts[i].modulate[2] = j;

		verts[i].modulate[3] = 255;
	}
	return qtrue;
}

void CG_DoSaber( vec3_t origin, vec3_t dir, float length, saber_colors_t color, int rfx )
{
	vec3_t		mid, rgb={1,1,1};
	qhandle_t	blade = 0, glow = 0;
	refEntity_t saber;
	float radiusmult;

	if ( length < 0.5f )
	{
		// if the thing is so short, just forget even adding me.
		return;
	}

	// Find the midpoint of the saber for lighting purposes
	VectorMA( origin, length * 0.5f, dir, mid );

	switch( color )
	{
		case SABER_RED:
			glow = cgs.media.redSaberGlowShader;
			blade = cgs.media.redSaberCoreShader;
			VectorSet( rgb, 1.0f, 0.2f, 0.2f );
			break;
		case SABER_ORANGE:
			glow = cgs.media.orangeSaberGlowShader;
			blade = cgs.media.orangeSaberCoreShader;
			VectorSet( rgb, 1.0f, 0.5f, 0.1f );
			break;
		case SABER_YELLOW:
			glow = cgs.media.yellowSaberGlowShader;
			blade = cgs.media.yellowSaberCoreShader;
			VectorSet( rgb, 1.0f, 1.0f, 0.2f );
			break;
		case SABER_GREEN:
			glow = cgs.media.greenSaberGlowShader;
			blade = cgs.media.greenSaberCoreShader;
			VectorSet( rgb, 0.2f, 1.0f, 0.2f );
			break;
		case SABER_BLUE:
			glow = cgs.media.blueSaberGlowShader;
			blade = cgs.media.blueSaberCoreShader;
			VectorSet( rgb, 0.2f, 0.4f, 1.0f );
			break;
		case SABER_PURPLE:
			glow = cgs.media.purpleSaberGlowShader;
			blade = cgs.media.purpleSaberCoreShader;
			VectorSet( rgb, 0.9f, 0.2f, 1.0f );
			break;
		default:
			glow = cgs.media.blueSaberGlowShader;
			blade = cgs.media.blueSaberCoreShader;
			VectorSet( rgb, 0.2f, 0.4f, 1.0f );
			break;
	}

	// always add a light because sabers cast a nice glow before they slice you in half!!  or something...
	trap_R_AddLightToScene( mid, (length*2.0f) + (random()*8.0f), rgb[0], rgb[1], rgb[2] );

	memset( &saber, 0, sizeof( refEntity_t ));

	// Saber glow is it's own ref type because it uses a ton of sprites, otherwise it would eat up too many
	//	refEnts to do each glow blob individually
	saber.saberLength = length;

	// Jeff, I did this because I foolishly wished to have a bright halo as the saber is unleashed.  
	// It's not quite what I'd hoped tho.  If you have any ideas, go for it!  --Pat
	if (length < SABER_LENGTH_MAX)
	{
		radiusmult = 1.0 + (2.0 / length);		// Note this creates a curve, and length cannot be < 0.5.
	}
	else
	{
		radiusmult = 1.0;
	}


	saber.radius = (2.8 + crandom() * 0.2f)*radiusmult;


	VectorCopy( origin, saber.origin );
	VectorCopy( dir, saber.axis[0] );
	saber.reType = RT_SABER_GLOW;
	saber.customShader = glow;
	saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;
	saber.renderfx = rfx;

	trap_R_AddRefEntityToScene( &saber );

	// Do the hot core
	VectorMA( origin, length, dir, saber.origin );
	VectorMA( origin, -1, dir, saber.oldorigin );


//	CG_TestLine(saber.origin, saber.oldorigin, 50, 0x000000ff, 3);
	saber.customShader = blade;
	saber.reType = RT_LINE;
	saber.radius = (1.0 + crandom() * 0.2f)*radiusmult;

	saber.shaderTexCoord[0] = saber.shaderTexCoord[1] = 1.0f;
	saber.shaderRGBA[0] = saber.shaderRGBA[1] = saber.shaderRGBA[2] = saber.shaderRGBA[3] = 0xff;

	trap_R_AddRefEntityToScene( &saber );
}

//--------------------------------------------------------------
// CG_GetTagWorldPosition
//
// Can pass in NULL for the axis
//--------------------------------------------------------------
void CG_GetTagWorldPosition( refEntity_t *model, char *tag, vec3_t pos, vec3_t axis[3] )
{
	orientation_t	orientation;
	int i = 0;

	// Get the requested tag
	trap_R_LerpTag( &orientation, model->hModel, model->oldframe, model->frame,
		1.0f - model->backlerp, tag );

	VectorCopy( model->origin, pos );
	for ( i = 0 ; i < 3 ; i++ ) 
	{
		VectorMA( pos, orientation.origin[i], model->axis[i], pos );
	}

	if ( axis )
	{
		MatrixMultiply( orientation.axis, model->axis, axis );
	}
}

#define	MAX_MARK_FRAGMENTS	128
#define	MAX_MARK_POINTS		384
extern markPoly_t *CG_AllocMark();

void CG_CreateSaberMarks( vec3_t start, vec3_t end, vec3_t normal )
{
//	byte			colors[4];
	int				i, j;
	int				numFragments;
	vec3_t			axis[3], originalPoints[4], mid;
	vec3_t			markPoints[MAX_MARK_POINTS], projection;
	polyVert_t		*v, verts[MAX_VERTS_ON_POLY];
	markPoly_t		*mark;
	markFragment_t	markFragments[MAX_MARK_FRAGMENTS], *mf;

	float	radius = 0.65f;

	if ( !cg_addMarks.integer ) 
	{
		return;
	}

	VectorSubtract( end, start, axis[1] );
	VectorNormalize( axis[1] );

	// create the texture axis
	VectorCopy( normal, axis[0] );
	CrossProduct( axis[1], axis[0], axis[2] );

	// create the full polygon that we'll project
	for ( i = 0 ; i < 3 ; i++ ) 
	{	// stretch a bit more in the direction that we are traveling in...  debateable as to whether this makes things better or worse
		originalPoints[0][i] = start[i] - radius * axis[1][i] - radius * axis[2][i];
		originalPoints[1][i] = end[i] + radius * axis[1][i] - radius * axis[2][i];
		originalPoints[2][i] = end[i] + radius * axis[1][i] + radius * axis[2][i];
		originalPoints[3][i] = start[i] - radius * axis[1][i] + radius * axis[2][i];
	}

	VectorScale( normal, -1, projection );

	// get the fragments
	numFragments = trap_CM_MarkFragments( 4, (const float (*)[3])originalPoints,
					projection, MAX_MARK_POINTS, markPoints[0], MAX_MARK_FRAGMENTS, markFragments );


	for ( i = 0, mf = markFragments ; i < numFragments ; i++, mf++ ) 
	{
		// we have an upper limit on the complexity of polygons that we store persistantly
		if ( mf->numPoints > MAX_VERTS_ON_POLY ) 
		{
			mf->numPoints = MAX_VERTS_ON_POLY;
		}

		for ( j = 0, v = verts ; j < mf->numPoints ; j++, v++ ) 
		{
			vec3_t delta;

			// Set up our texture coords, this may need some work 
			VectorCopy( markPoints[mf->firstPoint + j], v->xyz );
			VectorAdd( end, start, mid );
			VectorScale( mid, 0.5f, mid );
			VectorSubtract( v->xyz, mid, delta );

			v->st[0] = 0.5 + DotProduct( delta, axis[1] ) * (0.05f + random() * 0.03f); 
			v->st[1] = 0.5 + DotProduct( delta, axis[2] ) * (0.15f + random() * 0.05f);	
		}

		// save it persistantly, do burn first
		mark = CG_AllocMark();
		mark->time = cg.time;
		mark->alphaFade = qtrue;
		mark->markShader = cgs.media.rivetMarkShader;
		mark->poly.numVerts = mf->numPoints;
		mark->color[0] = mark->color[1] = mark->color[2] = mark->color[3] = 255;
		memcpy( mark->verts, verts, mf->numPoints * sizeof( verts[0] ) );

		// And now do a glow pass
		// by moving the start time back, we can hack it to fade out way before the burn does
		mark = CG_AllocMark();
		mark->time = cg.time - 8500;
		mark->alphaFade = qfalse;
		mark->markShader = trap_R_RegisterShader("gfx/effects/saberDamageGlow" );
		mark->poly.numVerts = mf->numPoints;
		mark->color[0] = 215 + random() * 40.0f;
		mark->color[1] = 96 + random() * 32.0f;
		mark->color[2] = mark->color[3] = random()*15.0f;
		memcpy( mark->verts, verts, mf->numPoints * sizeof( verts[0] ) );
	}
}

#define SABER_TRAIL_TIME	40.0f
#define FX_USE_ALPHA		0x08000000

void CG_AddSaberBlade( centity_t *cent, centity_t *scent, refEntity_t *saber, int renderfx, int modelIndex, vec3_t origin, vec3_t angles, qboolean fromSaber)
{
	vec3_t	org_, end, v,
			axis_[3] = {0,0,0, 0,0,0, 0,0,0};	// shut the compiler up
	trace_t	trace;
	int i = 0;
	float saberLen, dualSaberLen;
	float diff;
	clientInfo_t *client;
	centity_t *saberEnt;
	saberTrail_t *saberTrail;
	mdxaBone_t	boltMatrix;
	vec3_t futureAngles;
	effectTrailArgStruct_t fx;
	int scolor = 0;
	vec3_t otherPos, otherDir, otherEnd;
	float dualLen = 0.7;

	saberEnt = &cg_entities[cent->currentState.saberEntityNum];

	if (/*cg.snap->ps.clientNum == cent->currentState.number && */
		cgs.clientinfo[ cent->currentState.clientNum ].team != TEAM_SPECTATOR &&
		!(cg.snap->ps.pm_flags & PMF_FOLLOW))
	{
		if (cent->saberLength < 1)
		{
			cent->saberLength = 1;
			cent->saberExtendTime = cg.time;
		}

		if (cent->saberLength < SABER_LENGTH_MAX)
		{
			cent->saberLength += (cg.time - cent->saberExtendTime)*0.05;
		}

		if (cent->saberLength > SABER_LENGTH_MAX)
		{
			cent->saberLength = SABER_LENGTH_MAX;
		}

		cent->saberExtendTime = cg.time;
		saberLen = cent->saberLength;
	}
	else
	{
		saberLen = SABER_LENGTH_MAX;
	}

/*
Ghoul2 Insert Start
*/

	dualSaberLen = saberLen;

	if ((cent->currentState.eFlags & EF_DEAD) && !fromSaber)
	{ //trying to draw a saber on a corpse? That's bad.
		return;
	}

	futureAngles[YAW] = angles[YAW];
	futureAngles[PITCH] = angles[PITCH];
	futureAngles[ROLL] = angles[ROLL];

	// figure out where the actual model muzzle is
	if (fromSaber)
	{
		trap_G2API_GetBoltMatrix(scent->ghoul2, 0, 0, &boltMatrix, futureAngles, origin, cg.time, cgs.gameModels, scent->modelScale);
	}
	else
	{
		trap_G2API_GetBoltMatrix(scent->ghoul2, 1, 0, &boltMatrix, futureAngles, origin, cg.time, cgs.gameModels, scent->modelScale);
	}
	// work the matrix axis stuff into the original axis and origins used.
	trap_G2API_GiveMeVectorFromMatrix(&boltMatrix, ORIGIN, org_);
	trap_G2API_GiveMeVectorFromMatrix(&boltMatrix, NEGATIVE_Y, axis_[0]);

	if (!fromSaber && saberEnt)
	{
		VectorCopy(org_, saberEnt->currentState.pos.trBase);

		VectorCopy(axis_[0], saberEnt->currentState.apos.trBase);
	}

	client = &cgs.clientinfo[cent->currentState.number];

	if (!client)
	{ //something horrible has apparently happened
		return;
	}

	if (cent->currentState.bolt2)
	{
		VectorMA( org_, saberLen*dualLen, axis_[0], end );
	}
	else
	{
		VectorMA( org_, saberLen, axis_[0], end );
	}
	
	VectorAdd( end, axis_[0], end );

	if (cent->currentState.bolt2)
	{
		otherPos[0] = org_[0] - axis_[0][0]*12;
		otherPos[1] = org_[1] - axis_[0][1]*12;
		otherPos[2] = org_[2] - axis_[0][2]*12;

		otherDir[0] = -axis_[0][0];
		otherDir[1] = -axis_[0][1];
		otherDir[2] = -axis_[0][2];

		VectorMA( otherPos, dualSaberLen*dualLen, otherDir, otherEnd );
		VectorAdd( otherEnd, otherDir, otherEnd );
	}

	scolor = cgs.clientinfo[cent->currentState.number].icolor1;

	if (cgs.gametype >= GT_TEAM)
	{
		if (cgs.clientinfo[cent->currentState.number].team == TEAM_RED)
		{
			scolor = SABER_RED;
		}
		else if (cgs.clientinfo[cent->currentState.number].team == TEAM_BLUE)
		{
			scolor = SABER_BLUE;
		}
	}

	if (!cg_saberContact.integer)
	{ //if we don't have saber contact enabled, just add the blade and don't care what it's touching
		goto CheckTrail;
	}

	for ( i = 0; i < 1; i++ )//was 2 because it would go through architecture and leave saber trails on either side of the brush - but still looks bad if we hit a corner, blade is still 8 longer than hit
	{
		if ( i )
		{//tracing from end to base
			CG_Trace( &trace, end, NULL, NULL, org_, ENTITYNUM_NONE, MASK_SOLID );
		}
		else
		{//tracing from base to end
			CG_Trace( &trace, org_, NULL, NULL, end, ENTITYNUM_NONE, MASK_SOLID );
		}
		
		if ( trace.fraction < 1.0f )
		{
			vec3_t trDir;
			VectorCopy(trace.plane.normal, trDir);
			if (!trDir[0] && !trDir[1] && !trDir[2])
			{
				trDir[1] = 1;
			}
			trap_FX_PlayEffectID( trap_FX_RegisterEffect("saber/spark.efx"), trace.endpos, trDir );

			//Stop saber? (it wouldn't look right if it was stuck through a thin wall and unable to hurt players on the other side)
			VectorSubtract(org_, trace.endpos, v);
			saberLen = VectorLength(v);

			VectorCopy(trace.endpos, end);

			if (cent->currentState.bolt2)
			{
				break;
			}
			// All I need is a bool to mark whether I have a previous point to work with.
			//....come up with something better..
			if ( client->saberTrail.haveOldPos[i] )
			{
				if ( trace.entityNum == ENTITYNUM_WORLD )
				{//only put marks on architecture
					// Let's do some cool burn/glowing mark bits!!!
					CG_CreateSaberMarks( client->saberTrail.oldPos[i], trace.endpos, trace.plane.normal );
				
					//make a sound
					if ( cg.time - client->saberHitWallSoundDebounceTime >= 100 )
					{//ugh, need to have a real sound debouncer... or do this game-side
						client->saberHitWallSoundDebounceTime = cg.time;
						trap_S_StartSound ( trace.endpos, -1, CHAN_WEAPON, trap_S_RegisterSound( va("sound/weapons/saber/saberhitwall%i", Q_irand(1, 3)) ) );
					}
				}
			}
			else
			{
				// if we impact next frame, we'll mark a slash mark
				client->saberTrail.haveOldPos[i] = qtrue;
//				CG_ImpactMark( cgs.media.rivetMarkShader, client->saberTrail.oldPos[i], client->saberTrail.oldNormal[i],
//						0.0f, 1.0f, 1.0f, 1.0f, 1.0f, qfalse, 1.1f, qfalse );
			}

			// stash point so we can connect-the-dots later
			VectorCopy( trace.endpos, client->saberTrail.oldPos[i] );
			VectorCopy( trace.plane.normal, client->saberTrail.oldNormal[i] );
		}
		else
		{
			if (cent->currentState.bolt2)
			{
				break;
			}

			if ( client->saberTrail.haveOldPos[i] )
			{
				// Hmmm, no impact this frame, but we have an old point
				// Let's put the mark there, we should use an endcap mark to close the line, but we 
				//	can probably just get away with a round mark
//					CG_ImpactMark( cgs.media.rivetMarkShader, client->saberTrail.oldPos[i], client->saberTrail.oldNormal[i],
//							0.0f, 1.0f, 1.0f, 1.0f, 1.0f, qfalse, 1.1f, qfalse );
			}

			// we aren't impacting, so turn off our mark tracking mechanism
			client->saberTrail.haveOldPos[i] = qfalse;
		}
	}

	if (cent->currentState.bolt2)
	{
		for ( i = 0; i < 1; i++ )//was 2 because it would go through architecture and leave saber trails on either side of the brush - but still looks bad if we hit a corner, blade is still 8 longer than hit
		{
			CG_Trace( &trace, otherPos, NULL, NULL, otherEnd, ENTITYNUM_NONE, MASK_SOLID );
		
			if ( trace.fraction < 1.0f )
			{
				vec3_t trDir;
				VectorCopy(trace.plane.normal, trDir);
				if (!trDir[0] && !trDir[1] && !trDir[2])
				{
					trDir[1] = 1;
				}

				trap_FX_PlayEffectID( trap_FX_RegisterEffect("saber/spark.efx"), trace.endpos, trDir );

				//Stop saber? (it wouldn't look right if it was stuck through a thin wall and unable to hurt players on the other side)
				VectorSubtract(otherPos, trace.endpos, v);
				dualSaberLen = VectorLength(v);

				VectorCopy(trace.endpos, end);
			}
		}
	}
CheckTrail:

	if (!cg_saberTrail.integer)
	{ //don't do the trail in this case
		goto JustDoIt;
	}

	saberTrail = &client->saberTrail;

	// if we happen to be timescaled or running in a high framerate situation, we don't want to flood
	//	the system with very small trail slices...but perhaps doing it by distance would yield better results?
	if ( cg.time > saberTrail->lastTime + 2 ) // 2ms
	{
		if ( (saberMoveData[cent->currentState.saberMove].trailLength > 0 || ((cent->currentState.powerups & (1 << PW_SPEED) && cg_speedTrail.integer)) || cent->currentState.saberInFlight) && cg.time < saberTrail->lastTime + 2000 ) // if we have a stale segment, don't draw until we have a fresh one
		{
			vec3_t	rgb1={255.0f,255.0f,255.0f};

			switch( scolor )
			{
				case SABER_RED:
					VectorSet( rgb1, 255.0f, 0.0f, 0.0f );
					break;
				case SABER_ORANGE:
					VectorSet( rgb1, 255.0f, 64.0f, 0.0f );
					break;
				case SABER_YELLOW:
					VectorSet( rgb1, 255.0f, 255.0f, 0.0f );
					break;
				case SABER_GREEN:
					VectorSet( rgb1, 0.0f, 255.0f, 0.0f );
					break;
				case SABER_BLUE:
					VectorSet( rgb1, 0.0f, 64.0f, 255.0f );
					break;
				case SABER_PURPLE:
					VectorSet( rgb1, 220.0f, 0.0f, 255.0f );
					break;
				default:
					VectorSet( rgb1, 0.0f, 64.0f, 255.0f );
					break;
			}

			//Here we will use the happy process of filling a struct in with arguments and passing it to a trap function
			//so that we can take the struct and fill in an actual CTrail type using the data within it once we get it
			//into the effects area

			// Go from new muzzle to new end...then to old end...back down to old muzzle...finally
			//	connect back to the new muzzle...this is our trail quad
			VectorCopy( org_, fx.mVerts[0].origin );
			VectorMA( end, 3.0f, axis_[0], fx.mVerts[1].origin );

			VectorCopy( saberTrail->tip, fx.mVerts[2].origin );
			VectorCopy( saberTrail->base, fx.mVerts[3].origin );

			diff = cg.time - saberTrail->lastTime;

			// I'm not sure that clipping this is really the best idea
			//This prevents the trail from showing at all in low framerate situations.
			//if ( diff <= SABER_TRAIL_TIME * 2 )
			{
				float oldAlpha = 1.0f - ( diff / SABER_TRAIL_TIME );

				// New muzzle
				VectorCopy( rgb1, fx.mVerts[0].rgb );
				fx.mVerts[0].alpha = 255.0f;

				fx.mVerts[0].ST[0] = 0.0f;
				fx.mVerts[0].ST[1] = 1.0f;
				fx.mVerts[0].destST[0] = 1.0f;
				fx.mVerts[0].destST[1] = 1.0f;

				// new tip
				VectorCopy( rgb1, fx.mVerts[1].rgb );
				fx.mVerts[1].alpha = 255.0f;
				
				fx.mVerts[1].ST[0] = 0.0f;
				fx.mVerts[1].ST[1] = 0.0f;
				fx.mVerts[1].destST[0] = 1.0f;
				fx.mVerts[1].destST[1] = 0.0f;

				// old tip
				VectorCopy( rgb1, fx.mVerts[2].rgb );
				fx.mVerts[2].alpha = 255.0f;

				fx.mVerts[2].ST[0] = 1.0f - oldAlpha; // NOTE: this just happens to contain the value I want
				fx.mVerts[2].ST[1] = 0.0f;
				fx.mVerts[2].destST[0] = 1.0f + fx.mVerts[2].ST[0];
				fx.mVerts[2].destST[1] = 0.0f;

				// old muzzle
				VectorCopy( rgb1, fx.mVerts[3].rgb );
				fx.mVerts[3].alpha = 255.0f;

				fx.mVerts[3].ST[0] = 1.0f - oldAlpha; // NOTE: this just happens to contain the value I want
				fx.mVerts[3].ST[1] = 1.0f;
				fx.mVerts[3].destST[0] = 1.0f + fx.mVerts[2].ST[0];
				fx.mVerts[3].destST[1] = 1.0f;
		
				fx.mShader = cgs.media.saberBlurShader;
				fx.mSetFlags = FX_USE_ALPHA;
				fx.mKillTime = SABER_TRAIL_TIME;

				trap_FX_AddPrimitive(&fx);
			}

			if (cent->currentState.bolt2)
			{
				float oldAlpha = 1.0f - ( diff / SABER_TRAIL_TIME );

				VectorCopy( otherPos, fx.mVerts[0].origin );
				VectorMA( otherEnd, 3.0f, otherDir, fx.mVerts[1].origin );

				VectorCopy( saberTrail->dualtip, fx.mVerts[2].origin );
				VectorCopy( saberTrail->dualbase, fx.mVerts[3].origin );

				// New muzzle
				VectorCopy( rgb1, fx.mVerts[0].rgb );
				fx.mVerts[0].alpha = 255.0f;

				fx.mVerts[0].ST[0] = 0.0f;
				fx.mVerts[0].ST[1] = 1.0f;
				fx.mVerts[0].destST[0] = 1.0f;
				fx.mVerts[0].destST[1] = 1.0f;

				// new tip
				VectorCopy( rgb1, fx.mVerts[1].rgb );
				fx.mVerts[1].alpha = 255.0f;
				
				fx.mVerts[1].ST[0] = 0.0f;
				fx.mVerts[1].ST[1] = 0.0f;
				fx.mVerts[1].destST[0] = 1.0f;
				fx.mVerts[1].destST[1] = 0.0f;

				// old tip
				VectorCopy( rgb1, fx.mVerts[2].rgb );
				fx.mVerts[2].alpha = 255.0f;

				fx.mVerts[2].ST[0] = 1.0f - oldAlpha; // NOTE: this just happens to contain the value I want
				fx.mVerts[2].ST[1] = 0.0f;
				fx.mVerts[2].destST[0] = 1.0f + fx.mVerts[2].ST[0];
				fx.mVerts[2].destST[1] = 0.0f;

				// old muzzle
				VectorCopy( rgb1, fx.mVerts[3].rgb );
				fx.mVerts[3].alpha = 255.0f;

				fx.mVerts[3].ST[0] = 1.0f - oldAlpha; // NOTE: this just happens to contain the value I want
				fx.mVerts[3].ST[1] = 1.0f;
				fx.mVerts[3].destST[0] = 1.0f + fx.mVerts[2].ST[0];
				fx.mVerts[3].destST[1] = 1.0f;
		
				fx.mShader = cgs.media.saberBlurShader;
				fx.mSetFlags = FX_USE_ALPHA;
				fx.mKillTime = SABER_TRAIL_TIME;

				trap_FX_AddPrimitive(&fx);
			}
		}

		// we must always do this, even if we aren't active..otherwise we won't know where to pick up from
		VectorCopy( org_, saberTrail->base );
		VectorMA( end, 3.0f, axis_[0], saberTrail->tip );
		saberTrail->lastTime = cg.time;

		if (cent->currentState.bolt2)
		{
			VectorCopy( otherPos, saberTrail->dualbase );
			VectorMA( otherEnd, 3.0f, otherDir, saberTrail->dualtip );
		}
	}

JustDoIt:

	if (client && cent->currentState.bolt2)
	{
		float sideOneLen = saberLen*dualLen;
		float sideTwoLen = dualSaberLen*dualLen;

		if (sideOneLen < 1)
		{
			sideOneLen = 1;
		}
		
		CG_DoSaber( org_, axis_[0], sideOneLen, scolor, renderfx );

		CG_DoSaber( otherPos, otherDir, sideTwoLen, scolor, renderfx );
	}
	else
	{
		// Pass in the renderfx flags attached to the saber weapon model...this is done so that saber glows
		//	will get rendered properly in a mirror...not sure if this is necessary??
		CG_DoSaber( org_, axis_[0], saberLen, scolor, renderfx );
	}
}

int CG_IsMindTricked(int trickIndex1, int trickIndex2, int trickIndex3, int trickIndex4, int client)
{
	int checkIn;
	int sub = 0;

	if (cg_entities[client].currentState.forcePowersActive & (1 << FP_SEE))
	{
		return 0;
	}

	if (client > 47)
	{
		checkIn = trickIndex4;
		sub = 48;
	}
	else if (client > 31)
	{
		checkIn = trickIndex3;
		sub = 32;
	}
	else if (client > 15)
	{
		checkIn = trickIndex2;
		sub = 16;
	}
	else
	{
		checkIn = trickIndex1;
	}

	if (checkIn & (1 << (client-sub)))
	{
		return 1;
	}
	
	return 0;
}

#define SPEED_TRAIL_DISTANCE 6

void CG_DrawPlayerSphere(centity_t *cent, vec3_t origin, float scale, int shader)
{
	refEntity_t ent;
	
	// Don't draw the shield when the player is dead.
	if (cent->currentState.eFlags & EF_DEAD)
	{
		return;
	}

	memset( &ent, 0, sizeof( ent ) );

	VectorCopy( origin, ent.origin );
	ent.origin[2] += 9.0;

	VectorSubtract(cg.refdef.vieworg, ent.origin, ent.axis[0]);
	if (VectorNormalize(ent.axis[0]) <= 0.1f)
	{	// Entity is right on vieworg.  quit.
		return;
	}

	VectorCopy(cg.refdef.viewaxis[2], ent.axis[2]);
	CrossProduct(ent.axis[0], ent.axis[2], ent.axis[1]);

	VectorScale(ent.axis[0], scale, ent.axis[0]);
	VectorScale(ent.axis[1], scale, ent.axis[1]);
	VectorScale(ent.axis[2], -scale, ent.axis[2]);

	ent.hModel = cgs.media.halfShieldModel;
	ent.customShader = shader;	

	trap_R_AddRefEntityToScene( &ent );
}

void CG_AddLightningBeam(vec3_t start, vec3_t end)
{
	vec3_t	dir, chaos,
			c1, c2,
			v1, v2;
	float	len,
			s1, s2, s3;

	addbezierArgStruct_t b;

	VectorCopy(start, b.start);
	VectorCopy(end, b.end);

	VectorSubtract( b.end, b.start, dir );
	len = VectorNormalize( dir );

	// Get the base control points, we'll work from there
	VectorMA( b.start, 0.3333f * len, dir, c1 );
	VectorMA( b.start, 0.6666f * len, dir, c2 );

	// get some chaos values that really aren't very chaotic :)
	s1 = sin( cg.time * 0.005f ) * 2 + crandom() * 0.2f;
	s2 = sin( cg.time * 0.001f );
	s3 = sin( cg.time * 0.011f );

	VectorSet( chaos, len * 0.01f * s1,
						len * 0.02f * s2,
						len * 0.04f * (s1 + s2 + s3));

	VectorAdd( c1, chaos, c1 );
	VectorScale( chaos, 4.0f, v1 );

	VectorSet( chaos, -len * 0.02f * s3,
						len * 0.01f * (s1 * s2),
						-len * 0.02f * (s1 + s2 * s3));

	VectorAdd( c2, chaos, c2 );
	VectorScale( chaos, 2.0f, v2 );

	VectorSet( chaos, 1.0f, 1.0f, 1.0f );

	VectorCopy(c1, b.control1);
	VectorCopy(vec3_origin, b.control1Vel);
	VectorCopy(c2, b.control2);
	VectorCopy(vec3_origin, b.control2Vel);

	b.size1 = 6.0f;
	b.size2 = 6.0f;
	b.sizeParm = 0.0f;
	b.alpha1 = 0.0f;
	b.alpha2 = 0.2f;
	b.alphaParm = 0.5f;
	
	/*
	VectorCopy(WHITE, b.sRGB);
	VectorCopy(WHITE, b.eRGB);
	*/

	b.sRGB[0] = 255;
	b.sRGB[1] = 255;
	b.sRGB[2] = 255;
	VectorCopy(b.sRGB, b.eRGB);

	b.rgbParm = 0.0f;
	b.killTime = 50;
	b.shader = trap_R_RegisterShader( "gfx/misc/electric2" );
	b.flags = 0x00000001; //FX_ALPHA_LINEAR

	trap_FX_AddBezier(&b);
}

void CG_AddRandomLightning(vec3_t start, vec3_t end)
{
	vec3_t inOrg, outOrg;

	VectorCopy(start, inOrg);
	VectorCopy(end, outOrg);

	if ( rand() & 1 )
	{
		outOrg[0] += Q_irand(0, 24);
		inOrg[0] += Q_irand(0, 8);
	}
	else
	{
		outOrg[0] -= Q_irand(0, 24);
		inOrg[0] -= Q_irand(0, 8);
	}

	if ( rand() & 1 )
	{
		outOrg[1] += Q_irand(0, 24);
		inOrg[1] += Q_irand(0, 8);
	}
	else
	{
		outOrg[1] -= Q_irand(0, 24);
		inOrg[1] -= Q_irand(0, 8);
	}

	if ( rand() & 1 )
	{
		outOrg[2] += Q_irand(0, 50);
		inOrg[2] += Q_irand(0, 40);
	}
	else
	{
		outOrg[2] -= Q_irand(0, 64);
		inOrg[2] -= Q_irand(0, 40);
	}

	CG_AddLightningBeam(inOrg, outOrg);
}

extern char *forceHolocronModels[];

qboolean CG_ThereIsAMaster(void)
{
	int i = 0;
	centity_t *cent;

	while (i < MAX_CLIENTS)
	{
		cent = &cg_entities[i];

		if (cent && cent->currentState.isJediMaster)
		{
			return qtrue;
		}

		i++;
	}

	return qfalse;
}

/*
===============
CG_Player
===============
*/
void CG_Player( centity_t *cent ) {
	clientInfo_t	*ci;
	refEntity_t		legs;
	refEntity_t		torso;
	int				clientNum;
	int				renderfx;
	qboolean		shadow = qfalse;
	float			shadowPlane = 0;
	qboolean		dead = qfalse;
	vec3_t			rootAngles;
	refEntity_t		seeker;
	float			angle;
	vec3_t			angles, dir, elevated, enang, seekorg;
	int				iwantout = 0, successchange = 0;
	int				team;
	float			prefig = 0;
	centity_t		*enent;
	mdxaBone_t 		boltMatrix, lHandMatrix;
	vec3_t			efOrg;
	vec3_t			tDir;
	int				distVelBase;
	int				doAlpha = 0;
	int				effectTimeLayer = 0;
	qboolean		gotLHandMatrix = qfalse;
	qboolean		g2HasWeapon = qfalse;

	if (cgQueueLoad)
	{
		CG_ActualLoadDeferredPlayers();
		cgQueueLoad = qfalse;
	}

	// the client number is stored in clientNum.  It can't be derived
	// from the entity number, because a single client may have
	// multiple corpses on the level using the same clientinfo
	clientNum = cent->currentState.clientNum;
	if ( clientNum < 0 || clientNum >= MAX_CLIENTS ) {
		CG_Error( "Bad clientNum on player entity");
	}
	ci = &cgs.clientinfo[ clientNum ];

	// it is possible to see corpses from disconnected players that may
	// not have valid clientinfo
	if ( !ci->infoValid ) {
		return;
	}

	cent->ghoul2 = cg_entities[cent->currentState.number].ghoul2;

	if (!cent->ghoul2)
	{ //not ready yet?
#ifdef _DEBUG
		Com_Printf("WARNING: Client %i has a null ghoul2 instance\n", cent->currentState.number);
#endif
		if (cgs.clientinfo[cent->currentState.number].ghoul2Model &&
			trap_G2_HaveWeGhoul2Models(cgs.clientinfo[cent->currentState.number].ghoul2Model))
		{
#ifdef _DEBUG
			Com_Printf("Clientinfo instance was valid, duplicating for cent\n");
#endif
			trap_G2API_DuplicateGhoul2Instance(cgs.clientinfo[cent->currentState.number].ghoul2Model, &cent->ghoul2);
			cg_entities[cent->currentState.number].ghoul2 = cent->ghoul2;
		}
		return;
	}

	g2HasWeapon = trap_G2API_HasGhoul2ModelOnIndex(&(cent->ghoul2), 1);

	if (!g2HasWeapon)
	{ //force a redup of the weapon instance onto the client instance
		cent->ghoul2weapon = NULL;
	}

	if (cent->torsoBolt && !(cent->currentState.eFlags & EF_DEAD))
	{ //he's alive and has a limb missing still, reattach it and reset the weapon
		CG_ReattachLimb(cent);
	}

	if (cent->currentState.teamowner && !cent->isATST)
	{
		cg_entities[cent->currentState.number].currentState.teamowner = cent->currentState.teamowner;
		cg_entities[cent->currentState.number].isATST = cent->isATST;

		if (CG_RegisterClientModelname(&cgs.clientinfo[cent->currentState.number], cgs.clientinfo[cent->currentState.number].modelName, cgs.clientinfo[cent->currentState.number].skinName,
			cgs.clientinfo[cent->currentState.number].teamName, cent->currentState.number))
		{
			cent->isATST = 1;
			cg_entities[cent->currentState.number].isATST = cent->isATST;
			return;
		}
	}
	else if (!cent->currentState.teamowner && cent->isATST)
	{
		cg_entities[cent->currentState.number].currentState.teamowner = cent->currentState.teamowner;
		cg_entities[cent->currentState.number].isATST = cent->isATST;

		if (CG_RegisterClientModelname(&cgs.clientinfo[cent->currentState.number], cgs.clientinfo[cent->currentState.number].modelName, cgs.clientinfo[cent->currentState.number].skinName,
			cgs.clientinfo[cent->currentState.number].teamName, cent->currentState.number))
		{
			cent->isATST = 0;
			cg_entities[cent->currentState.number].isATST = cent->isATST;
			return;
		}
	}

	if (cent->currentState.number < MAX_CLIENTS &&
		cent->currentState.teamowner &&
		!cent->isATST)
	{
		return;
	}

	if (!cent->trickAlphaTime || (cg.time - cent->trickAlphaTime) > 1000)
	{ //things got out of sync, perhaps a new client is trying to fill in this slot
		cent->trickAlpha = 255;
		cent->trickAlphaTime = cg.time;
	}

	//If this client has tricked you.
	if (CG_IsMindTricked(cent->currentState.trickedentindex,
		cent->currentState.trickedentindex2,
		cent->currentState.trickedentindex3,
		cent->currentState.trickedentindex4,
		cg.snap->ps.clientNum))
	{
		if (cent->trickAlpha > 1)
		{
			cent->trickAlpha -= (cg.time - cent->trickAlphaTime)*0.5;
			cent->trickAlphaTime = cg.time;

			if (cent->trickAlpha < 0)
			{
				cent->trickAlpha = 0;
			}

			doAlpha = 1;
		}
		else
		{
			doAlpha = 1;
			cent->trickAlpha = 1;
			cent->trickAlphaTime = cg.time;
			iwantout = 1;
		}
	}
	else
	{
		if (cent->trickAlpha < 255)
		{
			cent->trickAlpha += (cg.time - cent->trickAlphaTime);
			cent->trickAlphaTime = cg.time;

			if (cent->trickAlpha > 255)
			{
				cent->trickAlpha = 255;
			}

			doAlpha = 1;
		}
		else
		{
			cent->trickAlpha = 255;
			cent->trickAlphaTime = cg.time;
		}
	}

	// get the player model information
	renderfx = 0;
	if ( cent->currentState.number == cg.snap->ps.clientNum) {
		if (!cg.renderingThirdPerson) {
			renderfx = RF_THIRD_PERSON;			// only draw in mirrors
		} else {
			if (cg_cameraMode.integer) {
				iwantout = 1;

				
				// goto minimal_add;
				
				// NOTENOTE Temporary
				return;
			}
		}
	}

	// Update the player's client entity information regarding weapons.
	// Explanation:  The entitystate has a weapond defined on it.  The cliententity does as well.
	// The cliententity's weapon tells us what the ghoul2 instance on the cliententity has bolted to it.
	// If the entitystate and cliententity weapons differ, then the state's needs to be copied to the client.
	// Save the old weapon, to verify that it is or is not the same as the new weapon.
	// rww - Make sure weapons don't get set BEFORE cent->ghoul2 is initialized or else we'll have no
	// weapon bolted on
	if (cent->currentState.saberInFlight)
	{
		cent->ghoul2weapon = g2WeaponInstances[WP_SABER];
	}

	if (cent->ghoul2 && 
		cent->ghoul2weapon != g2WeaponInstances[cent->currentState.weapon] &&
		!(cent->currentState.eFlags & EF_DEAD) && !cent->torsoBolt && !cent->isATST)
	{
		CG_CopyG2WeaponInstance(cent->currentState.weapon, cent->ghoul2);

		if (!(cg.snap->ps.pm_flags & PMF_FOLLOW))
		{
			if (cent->weapon == WP_SABER && cent->weapon != cent->currentState.weapon && !cent->currentState.shouldtarget)
			{ //switching away from the saber
				trap_S_StartSound(cent->lerpOrigin, cent->currentState.number, CHAN_AUTO, trap_S_RegisterSound( "sound/weapons/saber/saberoffquick.wav" ));
			}
			else if (cent->currentState.weapon == WP_SABER && cent->weapon != cent->currentState.weapon)
			{ //switching to the saber
				trap_S_StartSound(cent->lerpOrigin, cent->currentState.number, CHAN_AUTO, trap_S_RegisterSound( "sound/weapons/saber/saberon.wav" ));
			}
		}

		cent->weapon = cent->currentState.weapon;
		cent->ghoul2weapon = g2WeaponInstances[cent->currentState.weapon];
	}
	else if ((cent->currentState.eFlags & EF_DEAD) || cent->torsoBolt)
	{
		cent->ghoul2weapon = NULL; //be sure to update after respawning/getting limb regrown
	}

	
	memset (&legs, 0, sizeof(legs));

	CG_SetGhoul2Info(&legs, cent);

	VectorSet(legs.modelScale, 1,1,1);
	legs.radius = 64;
	VectorClear(legs.angles);

	if (cent->isATST)
	{
		legs.radius = 400;
	}

// minimal_add:

	team = cgs.clientinfo[ cent->currentState.clientNum ].team;

	if (cgs.gametype >= GT_TEAM && cg_drawFriend.integer &&
		cent->currentState.number != cg.snap->ps.clientNum)			// Don't show a sprite above a player's own head in 3rd person.
	{	// If the view is either a spectator or on the same team as this character, show a symbol above their head.
		if ((cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR || cg.snap->ps.persistant[PERS_TEAM] == team) &&
			!(cent->currentState.eFlags & EF_DEAD))
		{
			if (team == TEAM_RED)
			{
				CG_PlayerFloatSprite( cent, cgs.media.teamRedShader);
			}
			else	// if (team == TEAM_BLUE)
			{
				CG_PlayerFloatSprite( cent, cgs.media.teamBlueShader);
			}
		}
	}

	if (cgs.gametype == GT_JEDIMASTER && cg_drawFriend.integer &&
		cent->currentState.number != cg.snap->ps.clientNum)			// Don't show a sprite above a player's own head in 3rd person.
	{	// If the view is either a spectator or on the same team as this character, show a symbol above their head.
		if ((cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR || cg.snap->ps.persistant[PERS_TEAM] == team) &&
			!(cent->currentState.eFlags & EF_DEAD))
		{
			if (CG_ThereIsAMaster())
			{
				if (!cg.snap->ps.isJediMaster)
				{
					if (!cent->currentState.isJediMaster)
					{
						CG_PlayerFloatSprite( cent, cgs.media.teamRedShader);
					}
				}
			}
		}
	}

	if (cent->isATST)
	{
		goto doEssentialOne;
	}

	// add the shadow
	shadow = CG_PlayerShadow( cent, &shadowPlane );

	if ( (cent->currentState.eFlags & EF_SEEKERDRONE) || cent->currentState.genericenemyindex != -1 )
	{
		memset( &seeker, 0, sizeof(seeker) );

		VectorCopy(cent->lerpOrigin, elevated);
		elevated[2] += 40;

		VectorCopy( elevated, seeker.lightingOrigin );
		seeker.shadowPlane = shadowPlane;
		seeker.renderfx = 0; //renderfx;
							 //don't show in first person?

		angle = ((cg.time / 12) & 255) * (M_PI * 2) / 255;
		dir[0] = cos(angle) * 20;
		dir[1] = sin(angle) * 20;
		dir[2] = cos(angle) * 5;
		VectorAdd(elevated, dir, seeker.origin);

		VectorCopy(seeker.origin, seekorg);

		if (cent->currentState.genericenemyindex > 1024)
		{
			prefig = (cent->currentState.genericenemyindex-cg.time)/80;

			if (prefig > 55)
			{
				prefig = 55;
			}
			else if (prefig < 1)
			{
				prefig = 1;
			}

			elevated[2] -= 55-prefig;

			angle = ((cg.time / 12) & 255) * (M_PI * 2) / 255;
			dir[0] = cos(angle) * 20;
			dir[1] = sin(angle) * 20;
			dir[2] = cos(angle) * 5;
			VectorAdd(elevated, dir, seeker.origin);
		}
		else if (cent->currentState.genericenemyindex != ENTITYNUM_NONE && cent->currentState.genericenemyindex != -1)
		{
			enent = &cg_entities[cent->currentState.genericenemyindex];

			if (enent)
			{
				VectorSubtract(enent->lerpOrigin, seekorg, enang);
				VectorNormalize(enang);
				vectoangles(enang, angles);
				successchange = 1;
			}
		}

		if (!successchange)
		{
			angles[0] = sin(angle) * 30;
			angles[1] = (angle * 180 / M_PI) + 90;
			if (angles[1] > 360)
				angles[1] -= 360;
			angles[2] = 0;
		}

		AnglesToAxis( angles, seeker.axis );

		seeker.hModel = trap_R_RegisterModel("models/items/remote.md3");
		trap_R_AddRefEntityToScene( &seeker );
	}

doEssentialOne:
	// add a water splash if partially in and out of water
	CG_PlayerSplash( cent );

	if ( cg_shadows.integer == 3 && shadow ) {
		renderfx |= RF_SHADOW_PLANE;
	}
	renderfx |= RF_LIGHTING_ORIGIN;			// use the same origin for all

	// if we've been hit, display proper fullscreen fx
	CG_PlayerHitFX(cent);

	VectorCopy( cent->lerpOrigin, legs.origin );

	VectorCopy( cent->lerpOrigin, legs.lightingOrigin );
	legs.shadowPlane = shadowPlane;
	legs.renderfx = renderfx;
	VectorCopy (legs.origin, legs.oldorigin);	// don't positionally lerp at all

	CG_G2PlayerAngles( cent, legs.axis, rootAngles );

	//This call is mainly just to reconstruct the skeleton. But we'll get the left hand matrix while we're at it.
	//If we don't reconstruct the skeleton after setting the bone angles, we will get bad bolt points on the model
	//(e.g. the weapon model bolt will look "lagged") if there's no other GetBoltMatrix call for the rest of the
	//frame. Yes, this is stupid and needs to be fixed properly.
	//The current solution is to force it not to reconstruct the skeleton for the first GBM call in G2PlayerAngles.
	//It works and we end up only reconstructing it once, but it doesn't seem like the best solution.
	trap_G2API_GetBoltMatrix(cent->ghoul2, 0, cgs.clientinfo[cent->currentState.number].bolt_lhand, &lHandMatrix, cent->turAngles, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale);
	gotLHandMatrix = qtrue;

	if (cent->currentState.eFlags & EF_DEAD)
	{
		dead = qtrue;
		//rww - since our angles are fixed when we're dead this shouldn't be an issue anyway
		//we need to render the dying/dead player because we are now spawning the body on respawn instead of death
		//return;
	}

	ScaleModelAxis(&legs);

	memset( &torso, 0, sizeof(torso) );

	if (cent->isATST)
	{
		goto doEssentialTwo;
	}

	//rww - force speed "trail" effect
	if (!(cent->currentState.powerups & (1 << PW_SPEED)) || doAlpha || !cg_speedTrail.integer)
	{
		cent->frame_minus1_refreshed = 0;
		cent->frame_minus2_refreshed = 0;
	}

	if (cent->frame_minus1.ghoul2 != cent->ghoul2)
	{
		cent->frame_minus1_refreshed = 0;
	}
	if (cent->frame_minus2.ghoul2 != cent->ghoul2)
	{
		cent->frame_minus2_refreshed = 0;
	}

	VectorCopy(cent->currentState.pos.trDelta, tDir);

	distVelBase = SPEED_TRAIL_DISTANCE*(VectorNormalize(tDir)*0.004);

	if (cent->frame_minus1.ghoul2 && cent->frame_minus1_refreshed)
	{
		cent->frame_minus1.renderfx |= RF_FORCE_ENT_ALPHA;
		cent->frame_minus1.shaderRGBA[3] = 100;

		//rww - if the client gets a bad framerate we will only receive frame positions
		//once per frame anyway, so we might end up with speed trails very spread out.
		//in order to avoid that, we'll get the direction of the last trail from the player
		//and place the trail refent a set distance from the player location this frame
		VectorSubtract(cent->frame_minus1.origin, legs.origin, tDir);
		VectorNormalize(tDir);

		cent->frame_minus1.origin[0] = legs.origin[0]+tDir[0]*distVelBase;
		cent->frame_minus1.origin[1] = legs.origin[1]+tDir[1]*distVelBase;
		cent->frame_minus1.origin[2] = legs.origin[2]+tDir[2]*distVelBase;

		trap_R_AddRefEntityToScene(&cent->frame_minus1);
	}

	if (cent->frame_minus2.ghoul2 && cent->frame_minus2_refreshed)
	{
		cent->frame_minus2.renderfx |= RF_FORCE_ENT_ALPHA;
		cent->frame_minus2.shaderRGBA[3] = 50;

		//Same as above but do it between trail points instead of the player and first trail entry
		VectorSubtract(cent->frame_minus2.origin, cent->frame_minus1.origin, tDir);
		VectorNormalize(tDir);

		cent->frame_minus2.origin[0] = cent->frame_minus1.origin[0]+tDir[0]*distVelBase;
		cent->frame_minus2.origin[1] = cent->frame_minus1.origin[1]+tDir[1]*distVelBase;
		cent->frame_minus2.origin[2] = cent->frame_minus1.origin[2]+tDir[2]*distVelBase;

		trap_R_AddRefEntityToScene(&cent->frame_minus2);
	}

doEssentialTwo:
	VectorCopy(cent->turAngles, cg_entities[cent->currentState.number].turAngles);
	VectorCopy(legs.origin, cg_entities[cent->currentState.number].lerpOrigin);

	// get the animation state (after rotation, to allow feet shuffle)
	CG_PlayerAnimation( cent, &legs.oldframe, &legs.frame, &legs.backlerp,
		 &torso.oldframe, &torso.frame, &torso.backlerp );

	if (cent->currentState.eFlags & EF_DEAD)
	{ //keep track of death anim frame for when we copy off the bodyqueue
		cgs.clientinfo[cent->currentState.number].frame = cent->pe.torso.frame;
	}

/*	if ( cent->gent->s.number == 0 && cg_thirdPersonAlpha.value < 1.0f )
	{
		ent.renderfx |= RF_ALPHA_FADE;
		ent.shaderRGBA[3] = (unsigned char)(cg_thirdPersonAlpha.value * 255.0f);
	}
*/

	if (cent->isATST)
	{
		goto doEssentialThree;
	}

	//rww - render effects multiple times to compensate for low framerate? This won't do much because
	//the effect still gets rendered in this frame and expires, possibly before the next frame. So
	//it is disabled for now (setting effectTimeLayer to 0 after one play)
	if (cent->trailTime < cg.time)
	{
		cent->trailTime = cg.time;
	}

	//compensate for up to 300ms
	effectTimeLayer = (300 - (cent->trailTime - cg.time));

	if (effectTimeLayer < 50)
	{ //play at least once
		effectTimeLayer = 50;
	}

	cent->trailTime = cg.time + 300;

	if (cent->currentState.activeForcePass > FORCE_LEVEL_3)
	{
		int effectTimeLayerL = effectTimeLayer;

		vec3_t axis[3];
		vec3_t tAng, fAng, fxDir;
		int realForceLev = (cent->currentState.activeForcePass - FORCE_LEVEL_3);

		VectorSet( tAng, cent->turAngles[PITCH], cent->turAngles[YAW], cent->turAngles[ROLL] );

		VectorSet( fAng, cent->pe.torso.pitchAngle, cent->pe.torso.yawAngle, 0 );

		AngleVectors( fAng, fxDir, NULL, NULL );

		//trap_G2API_GetBoltMatrix(cent->ghoul2, 0, cgs.clientinfo[cent->currentState.number].bolt_lhand, &boltMatrix, tAng, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale);
		if (!gotLHandMatrix)
		{
			trap_G2API_GetBoltMatrix(cent->ghoul2, 0, cgs.clientinfo[cent->currentState.number].bolt_lhand, &lHandMatrix, cent->turAngles, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale);
			gotLHandMatrix = qtrue;
		}

		efOrg[0] = lHandMatrix.matrix[0][3];
		efOrg[1] = lHandMatrix.matrix[1][3];
		efOrg[2] = lHandMatrix.matrix[2][3];

		AnglesToAxis( fAng, axis );
	
		while (effectTimeLayerL > 0)
		{
			if ( realForceLev > FORCE_LEVEL_2 )
			{//arc
				//trap_FX_PlayEffectID( cgs.effects.forceLightningWide, efOrg, fxDir );
				trap_FX_PlayEntityEffectID(cgs.effects.forceDrainWide, efOrg, axis, cent->boltInfo, cent->currentState.number);
			}
			else
			{//line
				//trap_FX_PlayEffectID( cgs.effects.forceLightning, efOrg, fxDir );
				trap_FX_PlayEntityEffectID(cgs.effects.forceDrain, efOrg, axis, cent->boltInfo, cent->currentState.number);
			}

			effectTimeLayerL = 0;//-= 50;
		}

		/*
		if (cent->bolt4 < cg.time)
		{
			cent->bolt4 = cg.time + 100;
			trap_S_StartSound(NULL, cent->currentState.number, CHAN_AUTO, trap_S_RegisterSound("sound/weapons/force/drain.wav") );
		}
		*/
	}
	else if ( cent->currentState.activeForcePass )
	{//doing the electrocuting
		int effectTimeLayerL = effectTimeLayer;

		vec3_t axis[3];
		vec3_t tAng, fAng, fxDir;
		VectorSet( tAng, cent->turAngles[PITCH], cent->turAngles[YAW], cent->turAngles[ROLL] );

		VectorSet( fAng, cent->pe.torso.pitchAngle, cent->pe.torso.yawAngle, 0 );

		AngleVectors( fAng, fxDir, NULL, NULL );

		//trap_G2API_GetBoltMatrix(cent->ghoul2, 0, cgs.clientinfo[cent->currentState.number].bolt_lhand, &boltMatrix, tAng, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale);
		if (!gotLHandMatrix)
		{
			trap_G2API_GetBoltMatrix(cent->ghoul2, 0, cgs.clientinfo[cent->currentState.number].bolt_lhand, &lHandMatrix, cent->turAngles, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale);
			gotLHandMatrix = qtrue;
		}

		efOrg[0] = lHandMatrix.matrix[0][3];
		efOrg[1] = lHandMatrix.matrix[1][3];
		efOrg[2] = lHandMatrix.matrix[2][3];

		AnglesToAxis( fAng, axis );
	
		while (effectTimeLayerL > 0)
		{
			if ( cent->currentState.activeForcePass > FORCE_LEVEL_2 )
			{//arc
				//trap_FX_PlayEffectID( cgs.effects.forceLightningWide, efOrg, fxDir );
				trap_FX_PlayEntityEffectID(cgs.effects.forceLightningWide, efOrg, axis, cent->boltInfo, cent->currentState.number);
			}
			else
			{//line
				//trap_FX_PlayEffectID( cgs.effects.forceLightning, efOrg, fxDir );
				trap_FX_PlayEntityEffectID(cgs.effects.forceLightning, efOrg, axis, cent->boltInfo, cent->currentState.number);
			}

			effectTimeLayerL = 0;//-= 50;
		}

		/*
		if (cent->bolt4 < cg.time)
		{
			cent->bolt4 = cg.time + 100;
			trap_S_StartSound(NULL, cent->currentState.number, CHAN_AUTO, trap_S_RegisterSound("sound/weapons/force/lightning.wav") );
		}
		*/
	}

	if ( cent->currentState.powerups & (1 << PW_DISINT_4) )
	{
		vec3_t tAng;
		//VectorSet( tAng, 0, cent->pe.torso.yawAngle, 0 );
		VectorSet( tAng, cent->turAngles[PITCH], cent->turAngles[YAW], cent->turAngles[ROLL] );

		//trap_G2API_GetBoltMatrix(cent->ghoul2, 0, cgs.clientinfo[cent->currentState.number].bolt_lhand, &boltMatrix, tAng, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale);
		if (!gotLHandMatrix)
		{
			trap_G2API_GetBoltMatrix(cent->ghoul2, 0, cgs.clientinfo[cent->currentState.number].bolt_lhand, &lHandMatrix, cent->turAngles, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale);
			gotLHandMatrix = qtrue;
		}

		efOrg[0] = lHandMatrix.matrix[0][3];
		efOrg[1] = lHandMatrix.matrix[1][3];
		efOrg[2] = lHandMatrix.matrix[2][3];

		if ( (cent->currentState.forcePowersActive & (1 << FP_GRIP)) &&
			(cg.renderingThirdPerson || cent->currentState.number != cg.snap->ps.clientNum) )
		{
			vec3_t boltDir;
			vec3_t origBolt;
			VectorCopy(efOrg, origBolt);
			trap_G2API_GiveMeVectorFromMatrix( &lHandMatrix, NEGATIVE_Y, boltDir );

			CG_ForceGripEffect( efOrg );
			CG_ForceGripEffect( efOrg );

			//Render a scaled version of the model's hand with a n337 looking shader
			{
				const char *rotateBone;
				char *limbName;
				char *limbCapName;
				vec3_t armAng;
				float wv = sin( cg.time * 0.003f ) * 0.08f + 0.1f;

				rotateBone = "lradius";
				limbName = "l_arm";
				limbCapName = "l_arm_cap_torso_off";

				if (cent->grip_arm.ghoul2 && trap_G2_HaveWeGhoul2Models(cent->grip_arm.ghoul2))
				{
					trap_G2API_CleanGhoul2Models(&(cent->grip_arm.ghoul2));
				}

				memset( &cent->grip_arm, 0, sizeof(cent->grip_arm) );

				VectorCopy(origBolt, efOrg);
				trap_G2API_GiveMeVectorFromMatrix( &lHandMatrix, NEGATIVE_Y, boltDir );
				efOrg[0] += boltDir[0]*8;
				efOrg[1] += boltDir[1]*8;
				efOrg[2] += boltDir[2]*8;
				trap_G2API_GiveMeVectorFromMatrix( &lHandMatrix, NEGATIVE_X, boltDir );
				efOrg[0] -= boltDir[0]*4;
				efOrg[1] -= boltDir[1]*4;
				efOrg[2] -= boltDir[2]*4;

				efOrg[2] += 8;

				VectorCopy(efOrg, cent->grip_arm.origin);
				VectorCopy(cent->grip_arm.origin, cent->grip_arm.lightingOrigin);

				VectorCopy(cent->lerpAngles, armAng);
				armAng[ROLL] = -90;
				AnglesToAxis(armAng, cent->grip_arm.axis);
				
				trap_G2API_DuplicateGhoul2Instance(cent->ghoul2, &cent->grip_arm.ghoul2);

				trap_G2API_SetRootSurface(cent->grip_arm.ghoul2, 0, limbName);
				trap_G2API_SetNewOrigin(cent->grip_arm.ghoul2, trap_G2API_AddBolt(cent->grip_arm.ghoul2, 0, rotateBone));
				trap_G2API_SetSurfaceOnOff(cent->grip_arm.ghoul2, limbCapName, 0);

				cent->grip_arm.modelScale[0] = 1;//+(wv*6);
				cent->grip_arm.modelScale[1] = 1;//+(wv*6);
				cent->grip_arm.modelScale[2] = 1;//+(wv*6);
				ScaleModelAxis(&cent->grip_arm);

				cent->grip_arm.radius = 64;

				cent->grip_arm.customShader = trap_R_RegisterShader( "gfx/misc/red_portashield" );
				
				cent->grip_arm.renderfx |= RF_RGB_TINT;
				cent->grip_arm.shaderRGBA[0] = 255 - (wv*900);
				if (cent->grip_arm.shaderRGBA[0] < 30)
				{
					cent->grip_arm.shaderRGBA[0] = 30;
				}
				if (cent->grip_arm.shaderRGBA[0] > 255)
				{
					cent->grip_arm.shaderRGBA[0] = 255;
				}
				cent->grip_arm.shaderRGBA[1] = cent->grip_arm.shaderRGBA[2] = cent->grip_arm.shaderRGBA[0];
				
				trap_R_AddRefEntityToScene( &cent->grip_arm );
			}
		}
		else if (!(cent->currentState.forcePowersActive & (1 << FP_GRIP)))
		{
			CG_ForcePushBlur( efOrg );
		}
	}

	if (cent->currentState.weapon == WP_STUN_BATON && cent->currentState.number == cg.snap->ps.clientNum)
	{
		trap_S_AddLoopingSound( cent->currentState.number, cg.refdef.vieworg, vec3_origin, 
			trap_S_RegisterSound( "sound/weapons/baton/idle.wav" ) );
	}

	//NOTE: All effects that should be visible during mindtrick should go above here

	if (iwantout)
	{
		goto stillDoSaber;
		//return;
	}
	else if (doAlpha)
	{
		legs.renderfx |= RF_FORCE_ENT_ALPHA;
		legs.shaderRGBA[3] = cent->trickAlpha;

		if (legs.shaderRGBA[3] < 1)
		{ //don't cancel it out even if it's < 1
			legs.shaderRGBA[3] = 1;
		}
	}

	if (cent->teamPowerEffectTime > cg.time)
	{
		vec4_t preCol;
		int preRFX;

		preRFX = legs.renderfx;

		legs.renderfx |= RF_RGB_TINT;
		legs.renderfx |= RF_FORCE_ENT_ALPHA;

		preCol[0] = legs.shaderRGBA[0];
		preCol[1] = legs.shaderRGBA[1];
		preCol[2] = legs.shaderRGBA[2];
		preCol[3] = legs.shaderRGBA[3];

		if (cent->teamPowerType == 1)
		{ //heal
			legs.shaderRGBA[0] = 0;
			legs.shaderRGBA[1] = 255;
			legs.shaderRGBA[2] = 0;
		}
		else if (cent->teamPowerType == 0)
		{ //regen
			legs.shaderRGBA[0] = 0;
			legs.shaderRGBA[1] = 0;
			legs.shaderRGBA[2] = 255;
		}
		else
		{ //drain
			legs.shaderRGBA[0] = 255;
			legs.shaderRGBA[1] = 0;
			legs.shaderRGBA[2] = 0;
		}

		legs.shaderRGBA[3] = ((cent->teamPowerEffectTime - cg.time)/8);

		legs.customShader = trap_R_RegisterShader( "powerups/ysalimarishell" );
		trap_R_AddRefEntityToScene(&legs);

		legs.customShader = 0;
		legs.renderfx = preRFX;
		legs.shaderRGBA[0] = preCol[0];
		legs.shaderRGBA[1] = preCol[1];
		legs.shaderRGBA[2] = preCol[2];
		legs.shaderRGBA[3] = preCol[3];
	}

	//If you've tricked this client.
	if (CG_IsMindTricked(cg.snap->ps.fd.forceMindtrickTargetIndex,
		cg.snap->ps.fd.forceMindtrickTargetIndex2,
		cg.snap->ps.fd.forceMindtrickTargetIndex3,
		cg.snap->ps.fd.forceMindtrickTargetIndex4,
		cent->currentState.number))
	{
		if (cent->ghoul2)
		{
			vec3_t efOrg;
			vec3_t tAng, fxAng;
			vec3_t axis[3];
			int effectTimeLayerC = effectTimeLayer;
			sharedBoltInterface_t fxObj;

			//VectorSet( tAng, 0, cent->pe.torso.yawAngle, 0 );
			VectorSet( tAng, cent->turAngles[PITCH], cent->turAngles[YAW], cent->turAngles[ROLL] );

			trap_G2API_GetBoltMatrix(cent->ghoul2, 0, cgs.clientinfo[cent->currentState.number].bolt_head, &boltMatrix, tAng, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale);

			trap_G2API_GiveMeVectorFromMatrix(&boltMatrix, ORIGIN, efOrg);
			trap_G2API_GiveMeVectorFromMatrix(&boltMatrix, NEGATIVE_Y, fxAng);

 			axis[0][0] = boltMatrix.matrix[0][0];
 			axis[0][1] = boltMatrix.matrix[1][0];
		 	axis[0][2] = boltMatrix.matrix[2][0];

 			axis[1][0] = boltMatrix.matrix[0][1];
 			axis[1][1] = boltMatrix.matrix[1][1];
		 	axis[1][2] = boltMatrix.matrix[2][1];

 			axis[2][0] = boltMatrix.matrix[0][2];
 			axis[2][1] = boltMatrix.matrix[1][2];
		 	axis[2][2] = boltMatrix.matrix[2][2];

			VectorCopy(/*efOrg*/cent->lerpOrigin, fxObj.origin);
			VectorCopy(/*fxAng*/tAng, fxObj.angles);
			VectorCopy(cent->modelScale, fxObj.scale);
			fxObj.ghoul2 = cent->ghoul2;
			fxObj.isValid = 1;
			fxObj.modelNum = 0;
			fxObj.boltNum = cgs.clientinfo[cent->currentState.number].bolt_head;
			fxObj.entNum = cent->currentState.number;
	
			while (effectTimeLayerC > 0)
			{
				trap_FX_PlayEntityEffectID(trap_FX_RegisterEffect("force/confusion.efx"), efOrg, axis, cent->boltInfo, cent->currentState.number);

				//FIXME: Due to the horrible inefficiency involved in the current effect bolt process an effect with as many particles as this won't
				//work too happily. It also doesn't look a lot better due to the lag between origin updates with the effect bolt. If those issues
				//are ever resolved it should be switched over to BoltedEffect.
				//trap_FX_PlayBoltedEffectID(trap_FX_RegisterEffect("force/confusion.efx"), &fxObj);
				effectTimeLayerC = 0;//-= 50;
			}
		}
	}

	if (cgs.gametype == GT_HOLOCRON && cent->currentState.time2 && (cg.renderingThirdPerson || cg.snap->ps.clientNum != cent->currentState.number))
	{
		int i = 0;
		int renderedHolos = 0;
		refEntity_t		holoRef;

		while (i < NUM_FORCE_POWERS && renderedHolos < 3)
		{
			if (cent->currentState.time2 & (1 << i))
			{
				memset( &holoRef, 0, sizeof(holoRef) );

				VectorCopy(cent->lerpOrigin, elevated);
				elevated[2] += 8;

				VectorCopy( elevated, holoRef.lightingOrigin );
				holoRef.shadowPlane = shadowPlane;
				holoRef.renderfx = 0;//RF_THIRD_PERSON;

				if (renderedHolos == 0)
				{
					angle = ((cg.time / 8) & 255) * (M_PI * 2) / 255;
					dir[0] = cos(angle) * 20;
					dir[1] = sin(angle) * 20;
					dir[2] = cos(angle) * 20;
					VectorAdd(elevated, dir, holoRef.origin);

					angles[0] = sin(angle) * 30;
					angles[1] = (angle * 180 / M_PI) + 90;
					if (angles[1] > 360)
						angles[1] -= 360;
					angles[2] = 0;
					AnglesToAxis( angles, holoRef.axis );
				}
				else if (renderedHolos == 1)
				{
					angle = ((cg.time / 8) & 255) * (M_PI * 2) / 255 + M_PI;
					if (angle > M_PI * 2)
						angle -= (float)M_PI * 2;
					dir[0] = sin(angle) * 20;
					dir[1] = cos(angle) * 20;
					dir[2] = cos(angle) * 20;
					VectorAdd(elevated, dir, holoRef.origin);

					angles[0] = cos(angle - 0.5 * M_PI) * 30;
					angles[1] = 360 - (angle * 180 / M_PI);
					if (angles[1] > 360)
						angles[1] -= 360;
					angles[2] = 0;
					AnglesToAxis( angles, holoRef.axis );
				}
				else
				{
					angle = ((cg.time / 6) & 255) * (M_PI * 2) / 255 + 0.5 * M_PI;
					if (angle > M_PI * 2)
						angle -= (float)M_PI * 2;
					dir[0] = sin(angle) * 20;
					dir[1] = cos(angle) * 20;
					dir[2] = 0;
					VectorAdd(elevated, dir, holoRef.origin);
			
					VectorCopy(dir, holoRef.axis[1]);
					VectorNormalize(holoRef.axis[1]);
					VectorSet(holoRef.axis[2], 0, 0, 1);
					CrossProduct(holoRef.axis[1], holoRef.axis[2], holoRef.axis[0]);
				}

				holoRef.modelScale[0] = 0.5;
				holoRef.modelScale[1] = 0.5;
				holoRef.modelScale[2] = 0.5;
				ScaleModelAxis(&holoRef);

				{
					float wv;
					addspriteArgStruct_t fxSArgs;
					vec3_t holoCenter;

					holoCenter[0] = holoRef.origin[0] + holoRef.axis[2][0]*18;
					holoCenter[1] = holoRef.origin[1] + holoRef.axis[2][1]*18;
					holoCenter[2] = holoRef.origin[2] + holoRef.axis[2][2]*18;

					wv = sin( cg.time * 0.004f ) * 0.08f + 0.1f;

					VectorCopy(holoCenter, fxSArgs.origin);
					VectorClear(fxSArgs.vel);
					VectorClear(fxSArgs.accel);
					fxSArgs.scale = wv*60;
					fxSArgs.dscale = wv*60;
					fxSArgs.sAlpha = wv*12;
					fxSArgs.eAlpha = wv*12;
					fxSArgs.rotation = 0.0f;
					fxSArgs.bounce = 0.0f;
					fxSArgs.life = 1.0f;

					fxSArgs.flags = 0x08000000|0x00000001;

					if (forcePowerDarkLight[i] == FORCE_DARKSIDE)
					{ //dark
						fxSArgs.sAlpha *= 3;
						fxSArgs.eAlpha *= 3;
						fxSArgs.shader = cgs.media.redSaberGlowShader;
						trap_FX_AddSprite(&fxSArgs);
					}
					else if (forcePowerDarkLight[i] == FORCE_LIGHTSIDE)
					{ //light
						fxSArgs.sAlpha *= 1.5;
						fxSArgs.eAlpha *= 1.5;
						fxSArgs.shader = cgs.media.redSaberGlowShader;
						trap_FX_AddSprite(&fxSArgs);
						fxSArgs.shader = cgs.media.greenSaberGlowShader;
						trap_FX_AddSprite(&fxSArgs);
						fxSArgs.shader = cgs.media.blueSaberGlowShader;
						trap_FX_AddSprite(&fxSArgs);
					}
					else
					{ //neutral
						if (i == FP_SABERATTACK ||
							i == FP_SABERDEFEND ||
							i == FP_SABERTHROW)
						{ //saber power
							fxSArgs.sAlpha *= 1.5;
							fxSArgs.eAlpha *= 1.5;
							fxSArgs.shader = cgs.media.greenSaberGlowShader;
							trap_FX_AddSprite(&fxSArgs);
						}
						else
						{
							fxSArgs.sAlpha *= 0.5;
							fxSArgs.eAlpha *= 0.5;
							fxSArgs.shader = cgs.media.greenSaberGlowShader;
							trap_FX_AddSprite(&fxSArgs);
							fxSArgs.shader = cgs.media.blueSaberGlowShader;
							trap_FX_AddSprite(&fxSArgs);
						}
					}
				}

				holoRef.hModel = trap_R_RegisterModel(forceHolocronModels[i]);
				trap_R_AddRefEntityToScene( &holoRef );

				renderedHolos++;
			}
			i++;
		}
	}

	if ((cent->currentState.powerups & (1 << PW_YSALAMIRI)) ||
		(cgs.gametype == GT_CTY && ((cent->currentState.powerups & (1 << PW_REDFLAG)) || (cent->currentState.powerups & (1 << PW_BLUEFLAG)))) )
	{
		if (cgs.gametype == GT_CTY && (cent->currentState.powerups & (1 << PW_REDFLAG)))
		{
			CG_DrawPlayerSphere(cent, cent->lerpOrigin, 1.4, cgs.media.ysaliredShader );
		}
		else if (cgs.gametype == GT_CTY && (cent->currentState.powerups & (1 << PW_BLUEFLAG)))
		{
			CG_DrawPlayerSphere(cent, cent->lerpOrigin, 1.4, cgs.media.ysaliblueShader );
		}
		else
		{
			CG_DrawPlayerSphere(cent, cent->lerpOrigin, 1.4, cgs.media.ysalimariShader );
		}
	}
	
	if (cent->currentState.powerups & (1 << PW_FORCE_BOON))
	{
		CG_DrawPlayerSphere(cent, cent->lerpOrigin, 2.0, cgs.media.boonShader );
	}

	if (cent->currentState.powerups & (1 << PW_FORCE_ENLIGHTENED_DARK))
	{
		CG_DrawPlayerSphere(cent, cent->lerpOrigin, 2.0, cgs.media.endarkenmentShader );
	}
	else if (cent->currentState.powerups & (1 << PW_FORCE_ENLIGHTENED_LIGHT))
	{
		CG_DrawPlayerSphere(cent, cent->lerpOrigin, 2.0, cgs.media.enlightenmentShader );
	}

	if (cent->currentState.eFlags & EF_INVULNERABLE)
	{
		CG_DrawPlayerSphere(cent, cent->lerpOrigin, 1.4, cgs.media.invulnerabilityShader );
	}
stillDoSaber:
	if (cent->currentState.weapon == WP_SABER && !cent->currentState.shouldtarget)
	{
		if (!cent->currentState.saberInFlight && !(cent->currentState.eFlags & EF_DEAD))
		{
			if (cg.snap->ps.clientNum == cent->currentState.number)
			{
				trap_S_AddLoopingSound( cent->currentState.number, cg.refdef.vieworg, vec3_origin, 
					trap_S_RegisterSound( "sound/weapons/saber/saberhum1.wav" ) );
			}
			else
			{
				trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, 
					trap_S_RegisterSound( "sound/weapons/saber/saberhum1.wav" ) );
			}
		}

		if (iwantout && !cent->currentState.saberInFlight)
		{
			if (cent->currentState.eFlags & EF_DEAD)
			{
				if (cent->ghoul2 && cent->currentState.saberInFlight && g2HasWeapon)
				{ //special case, kill the saber on a freshly dead player if another source says to.
					trap_G2API_RemoveGhoul2Model(&(cent->ghoul2), 1);
					g2HasWeapon = qfalse;
				}
			}
			//return;
			goto endOfCall;
		}

		if (cent->currentState.saberInFlight && cent->currentState.saberEntityNum)
		{
			centity_t *saberEnt;

			saberEnt = &cg_entities[cent->currentState.saberEntityNum];

			if (/*!cent->bolt4 &&*/ g2HasWeapon)
			{ //saber is in flight, do not have it as a standard weapon model
				trap_G2API_RemoveGhoul2Model(&(cent->ghoul2), 1);
				g2HasWeapon = qfalse;

				//cent->bolt4 = 1;

				saberEnt->currentState.pos.trTime = cg.time;
				saberEnt->currentState.apos.trTime = cg.time;

				VectorCopy(saberEnt->currentState.pos.trBase, saberEnt->lerpOrigin);
				VectorCopy(saberEnt->currentState.apos.trBase, saberEnt->lerpAngles);

				cent->bolt3 = saberEnt->currentState.apos.trBase[0];
				cent->bolt2 = 0;

				saberEnt->currentState.bolt2 = 123;

				if (saberEnt->ghoul2)
				{
					// now set up the gun bolt on it
					trap_G2API_AddBolt(saberEnt->ghoul2, 0, "*flash");
				}
				else
				{
					trap_G2API_InitGhoul2Model(&saberEnt->ghoul2, "models/weapons2/saber/saber_w.glm", 0, 0, 0, 0, 0);

					if (saberEnt->ghoul2)
					{
						trap_G2API_AddBolt(saberEnt->ghoul2, 0, "*flash");
						//cent->bolt4 = 2;
						
						VectorCopy(saberEnt->currentState.pos.trBase, saberEnt->lerpOrigin);
						VectorCopy(saberEnt->currentState.apos.trBase, saberEnt->lerpAngles);
						saberEnt->currentState.pos.trTime = cg.time;
						saberEnt->currentState.apos.trTime = cg.time;
					}
				}
			}
			/*else if (cent->bolt4 != 2)
			{
				if (saberEnt->ghoul2)
				{
					trap_G2API_AddBolt(saberEnt->ghoul2, 0, "*flash");
					cent->bolt4 = 2;
				}
			}*/

			if (saberEnt && saberEnt->ghoul2 /*&& cent->bolt4 == 2*/)
			{
				vec3_t bladeAngles;

				if (!cent->bolt2)
				{
					cent->bolt2 = cg.time;
				}

				if (cent->bolt3 != 90)
				{
					if (cent->bolt3 < 90)
					{
						cent->bolt3 += (cg.time - cent->bolt2)*0.5;

						if (cent->bolt3 > 90)
						{
							cent->bolt3 = 90;
						}
					}
					else if (cent->bolt3 > 90)
					{
						cent->bolt3 -= (cg.time - cent->bolt2)*0.5;

						if (cent->bolt3 < 90)
						{
							cent->bolt3 = 90;
						}
					}
				}

				cent->bolt2 = cg.time;

				saberEnt->currentState.apos.trBase[0] = cent->bolt3;
				saberEnt->lerpAngles[0] = cent->bolt3;

				if (!saberEnt->currentState.saberInFlight && saberEnt->currentState.bolt2 != 123)
				{ //owner is pulling is back
					vec3_t owndir;

					VectorSubtract(saberEnt->lerpOrigin, cent->lerpOrigin, owndir);
					VectorNormalize(owndir);

					vectoangles(owndir, owndir);

					owndir[0] += 90;

					VectorCopy(owndir, saberEnt->currentState.apos.trBase);
					VectorCopy(owndir, saberEnt->lerpAngles);
					VectorClear(saberEnt->currentState.apos.trDelta);
				}

				//We don't actually want to rely entirely on server updates to render the position of the saber, because we actually know generally where
				//it's going to be before the first position update even gets here, and it needs to start getting rendered the instant the saber model is
				//removed from the player hand. So we'll just render it manually and let normal rendering for the entity be ignored.
				if (!saberEnt->currentState.saberInFlight && saberEnt->currentState.bolt2 != 123)
				{ //tell it that we're a saber and to render the glow around our handle because we're being pulled back
					saberEnt->bolt3 = 999;
				}

				saberEnt->currentState.modelGhoul2 = 1;
				CG_ManualEntityRender(saberEnt);
				saberEnt->bolt3 = 0;
				saberEnt->currentState.modelGhoul2 = 127;

				VectorCopy(saberEnt->lerpAngles, bladeAngles);
				bladeAngles[ROLL] = 0;
				CG_AddSaberBlade(cent, saberEnt, NULL, 0, 0, saberEnt->lerpOrigin, bladeAngles, qtrue);

				//Make the player's hand glow while guiding the saber
				{
					vec3_t tAng;
					float wv;
					addspriteArgStruct_t fxSArgs;

					VectorSet( tAng, cent->turAngles[PITCH], cent->turAngles[YAW], cent->turAngles[ROLL] );

					trap_G2API_GetBoltMatrix(cent->ghoul2, 0, cgs.clientinfo[cent->currentState.number].bolt_rhand, &boltMatrix, tAng, cent->lerpOrigin, cg.time, cgs.gameModels, cent->modelScale);

					efOrg[0] = boltMatrix.matrix[0][3];
					efOrg[1] = boltMatrix.matrix[1][3];
					efOrg[2] = boltMatrix.matrix[2][3];

					wv = sin( cg.time * 0.003f ) * 0.08f + 0.1f;

					//trap_FX_AddSprite( NULL, efOrg, NULL, NULL, 8.0f, 8.0f, wv, wv, 0.0f, 0.0f, 1.0f, cgs.media.yellowSaberGlowShader, 0x08000000 );
					VectorCopy(efOrg, fxSArgs.origin);
					VectorClear(fxSArgs.vel);
					VectorClear(fxSArgs.accel);
					fxSArgs.scale = 8.0f;
					fxSArgs.dscale = 8.0f;
					fxSArgs.sAlpha = wv;
					fxSArgs.eAlpha = wv;
					fxSArgs.rotation = 0.0f;
					fxSArgs.bounce = 0.0f;
					fxSArgs.life = 1.0f;
					fxSArgs.shader = cgs.media.yellowDroppedSaberShader;
					fxSArgs.flags = 0x08000000;
					trap_FX_AddSprite(&fxSArgs);
				}
			}
		}
		else
		{
			centity_t *saberEnt;

			saberEnt = &cg_entities[cent->currentState.saberEntityNum];

			if (/*cent->bolt4 && */!g2HasWeapon)
			{
				trap_G2API_CopySpecificGhoul2Model(g2WeaponInstances[WP_SABER], 0, cent->ghoul2, 1);

				if (saberEnt && saberEnt->ghoul2)
				{
					trap_G2API_CleanGhoul2Models(&(saberEnt->ghoul2));
				}

				saberEnt->currentState.modelindex = 0;
				saberEnt->ghoul2 = NULL;
				VectorClear(saberEnt->currentState.pos.trBase);
			}
			CG_AddSaberBlade( cent, cent, NULL, 0, 0, legs.origin, rootAngles, qfalse);
			cent->bolt3 = 0;
			cent->bolt2 = 0;

			//cent->bolt4 = 0;
		}
	}
	else
	{
		cent->saberLength = 0;
	}

	if (cent->currentState.eFlags & EF_DEAD)
	{
		if (cent->ghoul2 && cent->currentState.saberInFlight && g2HasWeapon)
		{ //special case, kill the saber on a freshly dead player if another source says to.
			trap_G2API_RemoveGhoul2Model(&(cent->ghoul2), 1);
			g2HasWeapon = qfalse;
		}
	}

	if (iwantout)
	{
		//return;
		goto endOfCall;
	}

	if ((cg.snap->ps.fd.forcePowersActive & (1 << FP_SEE)) && cg.snap->ps.clientNum != cent->currentState.number)
	{
		legs.shaderRGBA[0] = 255;
		legs.shaderRGBA[1] = 255;
		legs.shaderRGBA[2] = 0;
		legs.renderfx |= RF_MINLIGHT;
	}

	if (cg.snap->ps.duelInProgress /*&& cent->currentState.number != cg.snap->ps.clientNum*/)
	{ //I guess go ahead and glow your own client too in a duel
		if (cent->currentState.number != cg.snap->ps.duelIndex &&
			cent->currentState.number != cg.snap->ps.clientNum)
		{ //everyone not involved in the duel is drawn very dark
			legs.shaderRGBA[0] = 50;
			legs.shaderRGBA[1] = 50;
			legs.shaderRGBA[2] = 50;
			legs.renderfx |= RF_RGB_TINT;
		}
		else
		{ //adjust the glow by how far away you are from your dueling partner
			centity_t *duelEnt;

			duelEnt = &cg_entities[cg.snap->ps.duelIndex];
			
			if (duelEnt)
			{
				vec3_t vecSub;
				float subLen = 0;

				VectorSubtract(duelEnt->lerpOrigin, cg.snap->ps.origin, vecSub);
				subLen = VectorLength(vecSub);

				if (subLen < 1)
				{
					subLen = 1;
				}

				if (subLen > 1020)
				{
					subLen = 1020;
				}

				legs.shaderRGBA[0] = 255 - subLen/4;
				legs.shaderRGBA[1] = 255 - subLen/4;
				legs.shaderRGBA[2] = 255 - subLen/4;

				if (legs.shaderRGBA[2] < 1) legs.shaderRGBA[2] = 1;

				legs.renderfx &= ~RF_RGB_TINT;
				legs.renderfx &= ~RF_FORCE_ENT_ALPHA;
				legs.customShader = cgs.media.forceShell;
		
				trap_R_AddRefEntityToScene( &legs );

				legs.customShader = 0;

				legs.shaderRGBA[0] = 255 - subLen/8;
				legs.shaderRGBA[1] = 255 - subLen/8;
				legs.shaderRGBA[2] = 255 - subLen/8;

				if (legs.shaderRGBA[2] < 1)
				{
					legs.shaderRGBA[2] = 1;
				}
				if (legs.shaderRGBA[2] > 255)
				{
					legs.shaderRGBA[2] = 255;
				}

				if (subLen <= 1024)
				{
					legs.renderfx |= RF_RGB_TINT;
				}
			}
		}
	}
	else
	{
		if (cent->currentState.bolt1 && !(cent->currentState.eFlags & EF_DEAD) && cent->currentState.number != cg.snap->ps.clientNum && (!cg.snap->ps.duelInProgress || cg.snap->ps.duelIndex != cent->currentState.number))
		{
			legs.shaderRGBA[0] = 50;
			legs.shaderRGBA[1] = 50;
			legs.shaderRGBA[2] = 50;
			legs.renderfx |= RF_RGB_TINT;
		}
	}
doEssentialThree:
	if (cent->currentState.eFlags & EF_DISINTEGRATION)
	{
		vec3_t tempAng, hitLoc;
		float tempLength;

		if (!cent->dustTrailTime)
		{
			cent->dustTrailTime = cg.time;
			cent->miscTime = legs.frame;
		}

		if ((cg.time - cent->dustTrailTime) > 1500)
		{ //avoid rendering the entity after disintegration has finished anyway
			goto endOfCall;
		}

		trap_G2API_SetBoneAnim(legs.ghoul2, 0, "model_root", cent->miscTime, cent->miscTime, BONE_ANIM_OVERRIDE_FREEZE, 1.0f, cg.time, cent->miscTime, -1);
		trap_G2API_SetBoneAnim(legs.ghoul2, 0, "upper_lumbar", cent->miscTime, cent->miscTime, BONE_ANIM_OVERRIDE_FREEZE, 1.0f, cg.time, cent->miscTime, -1);
		trap_G2API_SetBoneAnim(legs.ghoul2, 0, "Motion", cent->miscTime, cent->miscTime, BONE_ANIM_OVERRIDE_FREEZE, 1.0f, cg.time, cent->miscTime, -1);

		VectorCopy(cent->currentState.origin2, hitLoc);

		VectorSubtract( hitLoc, legs.origin, legs.oldorigin );
		
		tempLength = VectorNormalize( legs.oldorigin );
		vectoangles( legs.oldorigin, tempAng );
		tempAng[YAW] -= cent->lerpAngles[YAW];
		AngleVectors( tempAng, legs.oldorigin, NULL, NULL );
		VectorScale( legs.oldorigin, tempLength, legs.oldorigin );
		legs.endTime = cent->dustTrailTime;

		//FIXME: Sorting is all wrong here, it can't decide if it should render this part or the outer body first
		/*
		legs.renderfx |= RF_DISINTEGRATE2;
		legs.customShader = cgs.media.disruptorShader;
		trap_R_AddRefEntityToScene( &legs );
		*/

		legs.renderfx &= ~(RF_DISINTEGRATE2);
		legs.renderfx |= (RF_DISINTEGRATE1);

		legs.customShader = 0;
		trap_R_AddRefEntityToScene( &legs );

		goto endOfCall;
	}
	else
	{
		cent->dustTrailTime = 0;
		cent->miscTime = 0;
	}

	trap_R_AddRefEntityToScene(&legs);

	if (cent->isATST)
	{
		//return;
		goto endOfCall;
	}

	cent->frame_minus2 = cent->frame_minus1;
	if (cent->frame_minus1_refreshed)
	{
		cent->frame_minus2_refreshed = 1;
	}
	cent->frame_minus1 = legs;
	cent->frame_minus1_refreshed = 1;

	if (!cent->frame_hold_refreshed && (cent->currentState.powerups & (1 << PW_SPEEDBURST)))
	{
		cent->frame_hold_time = cg.time + 254;
	}

	if (cent->frame_hold_time >= cg.time)
	{
		if (!cent->frame_hold_refreshed)
		{ //We're taking the ghoul2 instance from the original refent and duplicating it onto our refent alias so that we can then freeze the frame and fade it for the effect
			if (cent->frame_hold.ghoul2 && trap_G2_HaveWeGhoul2Models(cent->frame_hold.ghoul2) &&
				cent->frame_hold.ghoul2 != cent->ghoul2)
			{
				trap_G2API_CleanGhoul2Models(&(cent->frame_hold.ghoul2));
			}
			cent->frame_hold = legs;
			cent->frame_hold_refreshed = 1;
			cent->frame_hold.ghoul2 = NULL;
	
			trap_G2API_DuplicateGhoul2Instance(cent->ghoul2, &cent->frame_hold.ghoul2);

			//Set the animation to the current frame and freeze on end
			//trap_G2API_SetBoneAnim(cent->frame_hold.ghoul2, 0, "model_root", cent->frame_hold.frame, cent->frame_hold.frame, BONE_ANIM_OVERRIDE_FREEZE, 1.0f, cg.time, cent->frame_hold.frame, -1);
			trap_G2API_SetBoneAnim(cent->frame_hold.ghoul2, 0, "model_root", cent->frame_hold.frame, cent->frame_hold.frame, 0, 1.0f, cg.time, cent->frame_hold.frame, -1);
		}

		cent->frame_hold.renderfx |= RF_FORCE_ENT_ALPHA;
		cent->frame_hold.shaderRGBA[3] = (cent->frame_hold_time - cg.time);
		if (cent->frame_hold.shaderRGBA[3] > 254)
		{
			cent->frame_hold.shaderRGBA[3] = 254;
		}
		if (cent->frame_hold.shaderRGBA[3] < 1)
		{
			cent->frame_hold.shaderRGBA[3] = 1;
		}

		trap_R_AddRefEntityToScene(&cent->frame_hold);
	}
	else
	{
		cent->frame_hold_refreshed = 0;
	}

	//
	// add the gun / barrel / flash
	//
	if (cent->currentState.weapon != WP_EMPLACED_GUN)
	{
		CG_AddPlayerWeapon( &legs, NULL, cent, ci->team, rootAngles, qtrue );
	}
	// add powerups floating behind the player
	CG_PlayerPowerups( cent, &legs );

	if ((cent->currentState.forcePowersActive & (1 << FP_RAGE)) &&
		(cg.renderingThirdPerson || cent->currentState.number != cg.snap->ps.clientNum))
	{
		//legs.customShader = cgs.media.rageShader;
		legs.renderfx &= ~RF_FORCE_ENT_ALPHA;
		legs.renderfx &= ~RF_MINLIGHT;

		legs.renderfx |= RF_RGB_TINT;
		legs.shaderRGBA[0] = 255;
		legs.shaderRGBA[1] = legs.shaderRGBA[2] = 0;
		legs.shaderRGBA[3] = 255;

		if ( rand() & 1 )
		{
			legs.customShader = cgs.media.electricBodyShader;	
		}
		else
		{
			legs.customShader = cgs.media.electricBody2Shader;
		}

		trap_R_AddRefEntityToScene(&legs);
	}

	if (!cg.snap->ps.duelInProgress && cent->currentState.bolt1 && !(cent->currentState.eFlags & EF_DEAD) && cent->currentState.number != cg.snap->ps.clientNum && (!cg.snap->ps.duelInProgress || cg.snap->ps.duelIndex != cent->currentState.number))
	{
		legs.shaderRGBA[0] = 50;
		legs.shaderRGBA[1] = 50;
		legs.shaderRGBA[2] = 255;

		legs.renderfx &= ~RF_RGB_TINT;
		legs.renderfx &= ~RF_FORCE_ENT_ALPHA;
		legs.customShader = cgs.media.forceSightBubble;
		
		trap_R_AddRefEntityToScene( &legs );
	}

	//For now, these two are using the old shield shader. This is just so that you
	//can tell it apart from the JM/duel shaders, but it's still very obvious.
	if (cent->currentState.forcePowersActive & (1 << FP_PROTECT))
	{ //aborb is represented by green..
		legs.shaderRGBA[0] = 0;
		legs.shaderRGBA[1] = 255;
		legs.shaderRGBA[2] = 0;
		legs.shaderRGBA[3] = 254;

		legs.renderfx &= ~RF_RGB_TINT;
		legs.renderfx &= ~RF_FORCE_ENT_ALPHA;
		legs.customShader = cgs.media.playerShieldDamage;
		
		trap_R_AddRefEntityToScene( &legs );
	}
	if (cent->currentState.forcePowersActive & (1 << FP_ABSORB))
	{ //aborb is represented by blue..
		legs.shaderRGBA[0] = 0;
		legs.shaderRGBA[1] = 0;
		legs.shaderRGBA[2] = 255;
		legs.shaderRGBA[3] = 254;

		legs.renderfx &= ~RF_RGB_TINT;
		legs.renderfx &= ~RF_FORCE_ENT_ALPHA;
		legs.customShader = cgs.media.playerShieldDamage;
		
		trap_R_AddRefEntityToScene( &legs );
	}

	if (cent->currentState.isJediMaster && cg.snap->ps.clientNum != cent->currentState.number)
	{
		legs.shaderRGBA[0] = 100;
		legs.shaderRGBA[1] = 100;
		legs.shaderRGBA[2] = 255;

		legs.renderfx &= ~RF_RGB_TINT;
		legs.renderfx &= ~RF_FORCE_ENT_ALPHA;
		legs.renderfx |= RF_NODEPTH;
		legs.customShader = cgs.media.forceShell;
		
		trap_R_AddRefEntityToScene( &legs );

		legs.renderfx &= ~RF_NODEPTH;
	}

	if ((cg.snap->ps.fd.forcePowersActive & (1 << FP_SEE)) && cg.snap->ps.clientNum != cent->currentState.number && cg_auraShell.integer)
	{
		if (cgs.gametype >= GT_TEAM)
		{	// A team game
			switch(cgs.clientinfo[ cent->currentState.clientNum ].team)
			{
			case TEAM_RED:
				legs.shaderRGBA[0] = 255;
				legs.shaderRGBA[1] = 50;
				legs.shaderRGBA[2] = 50;
				break;
			case TEAM_BLUE:
				legs.shaderRGBA[0] = 75;
				legs.shaderRGBA[1] = 75;
				legs.shaderRGBA[2] = 255;
				break;

			default:
				legs.shaderRGBA[0] = 255;
				legs.shaderRGBA[1] = 255;
				legs.shaderRGBA[2] = 0;
				break;
			}
		}
		else
		{	// Not a team game
			legs.shaderRGBA[0] = 255;
			legs.shaderRGBA[1] = 255;
			legs.shaderRGBA[2] = 0;
		}

/*		if (cg.snap->ps.fd.forcePowerLevel[FP_SEE] <= FORCE_LEVEL_1)
		{
			legs.renderfx |= RF_MINLIGHT;
		}
		else
*/		{	// See through walls.
			legs.renderfx |= RF_MINLIGHT | RF_NODEPTH;

			if (cg.snap->ps.fd.forcePowerLevel[FP_SEE] < FORCE_LEVEL_2)
			{ //only level 2+ can see players through walls
				legs.renderfx &= ~RF_NODEPTH;
			}
		}

		legs.renderfx &= ~RF_RGB_TINT;
		legs.renderfx &= ~RF_FORCE_ENT_ALPHA;
		legs.customShader = cgs.media.sightShell;
		
		trap_R_AddRefEntityToScene( &legs );
	}

	// Electricity
	//------------------------------------------------
	if ( cent->currentState.emplacedOwner > cg.time ) 
	{
		int	dif = cent->currentState.emplacedOwner - cg.time;

		if ( dif > 0 && random() > 0.4f )
		{
			// fade out over the last 500 ms
			int brightness = 255;
			
			if ( dif < 500 )
			{
				brightness = floor((dif - 500.0f) / 500.0f * 255.0f );
			}

			legs.renderfx &= ~RF_FORCE_ENT_ALPHA;
			legs.renderfx &= ~RF_MINLIGHT;

			legs.renderfx |= RF_RGB_TINT;
			legs.shaderRGBA[0] = legs.shaderRGBA[1] = legs.shaderRGBA[2] = brightness;
			legs.shaderRGBA[3] = 255;

			if ( rand() & 1 )
			{
				legs.customShader = cgs.media.electricBodyShader;	
			}
			else
			{
				legs.customShader = cgs.media.electricBody2Shader;
			}

			trap_R_AddRefEntityToScene( &legs );

			if ( random() > 0.9f )
				trap_S_StartSound ( NULL, cent->currentState.number, CHAN_AUTO, cgs.media.crackleSound );
		}
	} 

	if (cent->currentState.powerups & (1 << PW_SHIELDHIT))
	{
		/*
		legs.shaderRGBA[0] = legs.shaderRGBA[1] = legs.shaderRGBA[2] = 255.0f * 0.5f;//t;
		legs.shaderRGBA[3] = 255;
		legs.renderfx &= ~RF_ALPHA_FADE;
		legs.renderfx |= RF_RGB_TINT;
		*/

		legs.shaderRGBA[0] = legs.shaderRGBA[1] = legs.shaderRGBA[2] = Q_irand(1, 255);

		legs.renderfx &= ~RF_FORCE_ENT_ALPHA;
		legs.renderfx &= ~RF_MINLIGHT;
		legs.renderfx &= ~RF_RGB_TINT;
		legs.customShader = cgs.media.playerShieldDamage;
		
		trap_R_AddRefEntityToScene( &legs );
	}
endOfCall:
	
	if (cgBoneAnglePostSet.refreshSet)
	{
		trap_G2API_SetBoneAngles(cgBoneAnglePostSet.ghoul2, cgBoneAnglePostSet.modelIndex, cgBoneAnglePostSet.boneName,
			cgBoneAnglePostSet.angles, cgBoneAnglePostSet.flags, cgBoneAnglePostSet.up, cgBoneAnglePostSet.right,
			cgBoneAnglePostSet.forward, cgBoneAnglePostSet.modelList, cgBoneAnglePostSet.blendTime, cgBoneAnglePostSet.currentTime);

		cgBoneAnglePostSet.refreshSet = qfalse;
	}
}


//=====================================================================

/*
===============
CG_ResetPlayerEntity

A player just came into view or teleported, so reset all animation info
===============
*/
void CG_ResetPlayerEntity( centity_t *cent ) 
{
	cent->errorTime = -99999;		// guarantee no error decay added
	cent->extrapolated = qfalse;	

	CG_ClearLerpFrame( cent, &cgs.clientinfo[ cent->currentState.clientNum ], &cent->pe.legs, cent->currentState.legsAnim, qfalse);
	CG_ClearLerpFrame( cent, &cgs.clientinfo[ cent->currentState.clientNum ], &cent->pe.torso, cent->currentState.torsoAnim, qtrue);

	BG_EvaluateTrajectory( &cent->currentState.pos, cg.time, cent->lerpOrigin );
	BG_EvaluateTrajectory( &cent->currentState.apos, cg.time, cent->lerpAngles );

	VectorCopy( cent->lerpOrigin, cent->rawOrigin );
	VectorCopy( cent->lerpAngles, cent->rawAngles );

	memset( &cent->pe.legs, 0, sizeof( cent->pe.legs ) );
	cent->pe.legs.yawAngle = cent->rawAngles[YAW];
	cent->pe.legs.yawing = qfalse;
	cent->pe.legs.pitchAngle = 0;
	cent->pe.legs.pitching = qfalse;

	memset( &cent->pe.torso, 0, sizeof( cent->pe.legs ) );
	cent->pe.torso.yawAngle = cent->rawAngles[YAW];
	cent->pe.torso.yawing = qfalse;
	cent->pe.torso.pitchAngle = cent->rawAngles[PITCH];
	cent->pe.torso.pitching = qfalse;

	if ((cent->ghoul2 == NULL) && trap_G2_HaveWeGhoul2Models(cgs.clientinfo[cent->currentState.clientNum].ghoul2Model))
	{
		trap_G2API_DuplicateGhoul2Instance(cgs.clientinfo[cent->currentState.clientNum].ghoul2Model, &cent->ghoul2);
		CG_CopyG2WeaponInstance(cent->currentState.weapon, cgs.clientinfo[cent->currentState.clientNum].ghoul2Model);
		cent->weapon = cent->currentState.weapon;
	}

	if ( cg_debugPosition.integer ) {
		CG_Printf("%i ResetPlayerEntity yaw=%i\n", cent->currentState.number, cent->pe.torso.yawAngle );
	}
}

