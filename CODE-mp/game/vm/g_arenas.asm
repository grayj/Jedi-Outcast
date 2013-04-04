export UpdateTournamentInfo
code
proc UpdateTournamentInfo 1116 68
file "../g_arenas.c"
line 20
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3://
;4:// g_arenas.c
;5://
;6:
;7:#include "g_local.h"
;8:
;9:
;10:gentity_t	*podium1;
;11:gentity_t	*podium2;
;12:gentity_t	*podium3;
;13:
;14:
;15:/*
;16:==================
;17:UpdateTournamentInfo
;18:==================
;19:*/
;20:void UpdateTournamentInfo( void ) {
line 32
;21:	int			i;
;22:	gentity_t	*player;
;23:	int			playerClientNum;
;24:	int			n, accuracy, perfect,	msglen;
;25:	int			buflen;
;26:	int			score1, score2;
;27:	qboolean	won;
;28:	char		buf[32];
;29:	char		msg[MAX_STRING_CHARS];
;30:
;31:	// find the real player
;32:	player = NULL;
ADDRLP4 40
CNSTP4 0
ASGNP4
line 33
;33:	for (i = 0; i < level.maxclients; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $82
JUMPV
LABELV $79
line 34
;34:		player = &g_entities[i];
ADDRLP4 40
CNSTI4 828
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 35
;35:		if ( !player->inuse ) {
ADDRLP4 40
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
NEI4 $84
line 36
;36:			continue;
ADDRGP4 $80
JUMPV
LABELV $84
line 38
;37:		}
;38:		if ( !( player->r.svFlags & SVF_BOT ) ) {
ADDRLP4 40
INDIRP4
CNSTI4 304
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
NEI4 $86
line 39
;39:			break;
ADDRGP4 $81
JUMPV
LABELV $86
line 41
;40:		}
;41:	}
LABELV $80
line 33
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $82
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $79
LABELV $81
line 43
;42:	// this should never happen!
;43:	if ( !player || i == level.maxclients ) {
ADDRLP4 40
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $91
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
NEI4 $88
LABELV $91
line 44
;44:		return;
ADDRGP4 $78
JUMPV
LABELV $88
line 46
;45:	}
;46:	playerClientNum = i;
ADDRLP4 1076
ADDRLP4 0
INDIRI4
ASGNI4
line 48
;47:
;48:	CalculateRanks();
ADDRGP4 CalculateRanks
CALLV
pop
line 50
;49:
;50:	if ( level.clients[playerClientNum].sess.sessionTeam == TEAM_SPECTATOR ) {
CNSTI4 1756
ADDRLP4 1076
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
NEI4 $92
line 51
;51:		Com_sprintf( msg, sizeof(msg), "postgame %i %i 0 0 0 0 0 0 0 0 0 0 0", level.numNonSpectatorClients, playerClientNum );
ADDRLP4 44
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $94
ARGP4
ADDRGP4 level+76
INDIRI4
ARGI4
ADDRLP4 1076
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 52
;52:	}
ADDRGP4 $93
JUMPV
LABELV $92
line 53
;53:	else {
line 54
;54:		if( player->client->accuracy_shots ) {
ADDRLP4 40
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1652
ADDP4
INDIRI4
CNSTI4 0
EQI4 $96
line 55
;55:			accuracy = player->client->accuracy_hits * 100 / player->client->accuracy_shots;
ADDRLP4 1100
ADDRLP4 40
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1084
CNSTI4 100
ADDRLP4 1100
INDIRP4
CNSTI4 1656
ADDP4
INDIRI4
MULI4
ADDRLP4 1100
INDIRP4
CNSTI4 1652
ADDP4
INDIRI4
DIVI4
ASGNI4
line 56
;56:		}
ADDRGP4 $97
JUMPV
LABELV $96
line 57
;57:		else {
line 58
;58:			accuracy = 0;
ADDRLP4 1084
CNSTI4 0
ASGNI4
line 59
;59:		}
LABELV $97
line 60
;60:		won = qfalse;
ADDRLP4 1080
CNSTI4 0
ASGNI4
line 61
;61:		if (g_gametype.integer >= GT_CTF) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 7
LTI4 $98
line 62
;62:			score1 = level.teamScores[TEAM_RED];
ADDRLP4 1092
ADDRGP4 level+44+4
INDIRI4
ASGNI4
line 63
;63:			score2 = level.teamScores[TEAM_BLUE];
ADDRLP4 1096
ADDRGP4 level+44+8
INDIRI4
ASGNI4
line 64
;64:			if (level.clients[playerClientNum].sess.sessionTeam	== TEAM_RED) {
CNSTI4 1756
ADDRLP4 1076
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 1
NEI4 $105
line 65
;65:				won = (level.teamScores[TEAM_RED] > level.teamScores[TEAM_BLUE]);
ADDRGP4 level+44+4
INDIRI4
ADDRGP4 level+44+8
INDIRI4
LEI4 $112
ADDRLP4 1100
CNSTI4 1
ASGNI4
ADDRGP4 $113
JUMPV
LABELV $112
ADDRLP4 1100
CNSTI4 0
ASGNI4
LABELV $113
ADDRLP4 1080
ADDRLP4 1100
INDIRI4
ASGNI4
line 66
;66:			} else {
ADDRGP4 $99
JUMPV
LABELV $105
line 67
;67:				won = (level.teamScores[TEAM_BLUE] > level.teamScores[TEAM_RED]);
ADDRGP4 level+44+8
INDIRI4
ADDRGP4 level+44+4
INDIRI4
LEI4 $119
ADDRLP4 1100
CNSTI4 1
ASGNI4
ADDRGP4 $120
JUMPV
LABELV $119
ADDRLP4 1100
CNSTI4 0
ASGNI4
LABELV $120
ADDRLP4 1080
ADDRLP4 1100
INDIRI4
ASGNI4
line 68
;68:			}
line 69
;69:		} else {
ADDRGP4 $99
JUMPV
LABELV $98
line 70
;70:			if (&level.clients[playerClientNum] == &level.clients[ level.sortedClients[0] ]) {
ADDRLP4 1100
CNSTI4 1756
ASGNI4
ADDRLP4 1104
ADDRGP4 level
INDIRP4
ASGNP4
ADDRLP4 1100
INDIRI4
ADDRLP4 1076
INDIRI4
MULI4
ADDRLP4 1104
INDIRP4
ADDP4
CVPU4 4
ADDRLP4 1100
INDIRI4
ADDRGP4 level+84
INDIRI4
MULI4
ADDRLP4 1104
INDIRP4
ADDP4
CVPU4 4
NEU4 $121
line 71
;71:				won = qtrue;
ADDRLP4 1080
CNSTI4 1
ASGNI4
line 72
;72:				score1 = level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE];
ADDRLP4 1092
CNSTI4 1756
ADDRGP4 level+84
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 280
ADDP4
INDIRI4
ASGNI4
line 73
;73:				score2 = level.clients[ level.sortedClients[1] ].ps.persistant[PERS_SCORE];
ADDRLP4 1096
CNSTI4 1756
ADDRGP4 level+84+4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 280
ADDP4
INDIRI4
ASGNI4
line 74
;74:			} else {
ADDRGP4 $122
JUMPV
LABELV $121
line 75
;75:				score2 = level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE];
ADDRLP4 1096
CNSTI4 1756
ADDRGP4 level+84
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 280
ADDP4
INDIRI4
ASGNI4
line 76
;76:				score1 = level.clients[ level.sortedClients[1] ].ps.persistant[PERS_SCORE];
ADDRLP4 1092
CNSTI4 1756
ADDRGP4 level+84+4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 280
ADDP4
INDIRI4
ASGNI4
line 77
;77:			}
LABELV $122
line 78
;78:		}
LABELV $99
line 79
;79:		if (won && player->client->ps.persistant[PERS_KILLED] == 0) {
ADDRLP4 1100
CNSTI4 0
ASGNI4
ADDRLP4 1080
INDIRI4
ADDRLP4 1100
INDIRI4
EQI4 $130
ADDRLP4 40
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 312
ADDP4
INDIRI4
ADDRLP4 1100
INDIRI4
NEI4 $130
line 80
;80:			perfect = 1;
ADDRLP4 1088
CNSTI4 1
ASGNI4
line 81
;81:		} else {
ADDRGP4 $131
JUMPV
LABELV $130
line 82
;82:			perfect = 0;
ADDRLP4 1088
CNSTI4 0
ASGNI4
line 83
;83:		}
LABELV $131
line 84
;84:		Com_sprintf( msg, sizeof(msg), "postgame %i %i %i %i %i %i %i %i %i %i %i %i %i %i", level.numNonSpectatorClients, playerClientNum, accuracy,
ADDRLP4 44
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $132
ARGP4
ADDRGP4 level+76
INDIRI4
ARGI4
ADDRLP4 1076
INDIRI4
ARGI4
ADDRLP4 1084
INDIRI4
ARGI4
ADDRLP4 1104
ADDRLP4 40
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1104
INDIRP4
CNSTI4 316
ADDP4
INDIRI4
ARGI4
ADDRLP4 1104
INDIRP4
CNSTI4 320
ADDP4
INDIRI4
ARGI4
ADDRLP4 1104
INDIRP4
CNSTI4 324
ADDP4
INDIRI4
ARGI4
ADDRLP4 1104
INDIRP4
CNSTI4 328
ADDP4
INDIRI4
ARGI4
ADDRLP4 1104
INDIRP4
CNSTI4 332
ADDP4
INDIRI4
ARGI4
ADDRLP4 1104
INDIRP4
CNSTI4 280
ADDP4
INDIRI4
ARGI4
ADDRLP4 1088
INDIRI4
ARGI4
ADDRLP4 1092
INDIRI4
ARGI4
ADDRLP4 1096
INDIRI4
ARGI4
ADDRGP4 level+32
INDIRI4
ARGI4
ADDRLP4 1104
INDIRP4
CNSTI4 336
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 88
;85:			player->client->ps.persistant[PERS_IMPRESSIVE_COUNT], player->client->ps.persistant[PERS_EXCELLENT_COUNT],player->client->ps.persistant[PERS_DEFEND_COUNT],
;86:			player->client->ps.persistant[PERS_ASSIST_COUNT], player->client->ps.persistant[PERS_GAUNTLET_FRAG_COUNT], player->client->ps.persistant[PERS_SCORE],
;87:			perfect, score1, score2, level.time, player->client->ps.persistant[PERS_CAPTURES] );
;88:	}
LABELV $93
line 90
;89:
;90:	msglen = strlen( msg );
ADDRLP4 44
ARGP4
ADDRLP4 1100
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1072
ADDRLP4 1100
INDIRI4
ASGNI4
line 91
;91:	for( i = 0; i < level.numNonSpectatorClients; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $138
JUMPV
LABELV $135
line 92
;92:		n = level.sortedClients[i];
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 level+84
ADDP4
INDIRI4
ASGNI4
line 93
;93:		Com_sprintf( buf, sizeof(buf), " %i %i %i", n, level.clients[n].ps.persistant[PERS_RANK], level.clients[n].ps.persistant[PERS_SCORE] );
ADDRLP4 8
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 $141
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 1108
CNSTI4 1756
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
ADDRLP4 1108
INDIRP4
CNSTI4 288
ADDP4
INDIRI4
ARGI4
ADDRLP4 1108
INDIRP4
CNSTI4 280
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 94
;94:		buflen = strlen( buf );
ADDRLP4 8
ARGP4
ADDRLP4 1112
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1068
ADDRLP4 1112
INDIRI4
ASGNI4
line 95
;95:		if( msglen + buflen + 1 >= sizeof(msg) ) {
ADDRLP4 1072
INDIRI4
ADDRLP4 1068
INDIRI4
ADDI4
CNSTI4 1
ADDI4
CVIU4 4
CNSTU4 1024
LTU4 $142
line 96
;96:			break;
ADDRGP4 $137
JUMPV
LABELV $142
line 98
;97:		}
;98:		strcat( msg, buf );
ADDRLP4 44
ARGP4
ADDRLP4 8
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 99
;99:	}
LABELV $136
line 91
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $138
ADDRLP4 0
INDIRI4
ADDRGP4 level+76
INDIRI4
LTI4 $135
LABELV $137
line 100
;100:	trap_SendConsoleCommand( EXEC_APPEND, msg );
CNSTI4 2
ARGI4
ADDRLP4 44
ARGP4
ADDRGP4 trap_SendConsoleCommand
CALLV
pop
line 101
;101:}
LABELV $78
endproc UpdateTournamentInfo 1116 68
bss
export podium3
align 4
LABELV podium3
skip 4
export podium2
align 4
LABELV podium2
skip 4
export podium1
align 4
LABELV podium1
skip 4
import trap_ROFF_Purge_Ent
import trap_ROFF_Play
import trap_ROFF_Cache
import trap_ROFF_UpdateEntities
import trap_ROFF_Clean
import trap_SP_GetStringTextString
import trap_SP_Register
import trap_SP_RegisterServer
import trap_SnapVector
import trap_GeneticParentsAndChildSelection
import trap_BotResetWeaponState
import trap_BotFreeWeaponState
import trap_BotAllocWeaponState
import trap_BotLoadWeaponWeights
import trap_BotGetWeaponInfo
import trap_BotChooseBestFightWeapon
import trap_BotAddAvoidSpot
import trap_BotInitMoveState
import trap_BotFreeMoveState
import trap_BotAllocMoveState
import trap_BotPredictVisiblePosition
import trap_BotMovementViewTarget
import trap_BotReachabilityArea
import trap_BotResetLastAvoidReach
import trap_BotResetAvoidReach
import trap_BotMoveInDirection
import trap_BotMoveToGoal
import trap_BotResetMoveState
import trap_BotFreeGoalState
import trap_BotAllocGoalState
import trap_BotMutateGoalFuzzyLogic
import trap_BotSaveGoalFuzzyLogic
import trap_BotInterbreedGoalFuzzyLogic
import trap_BotFreeItemWeights
import trap_BotLoadItemWeights
import trap_BotUpdateEntityItems
import trap_BotInitLevelItems
import trap_BotSetAvoidGoalTime
import trap_BotAvoidGoalTime
import trap_BotGetLevelItemGoal
import trap_BotGetMapLocationGoal
import trap_BotGetNextCampSpotGoal
import trap_BotItemGoalInVisButNotVisible
import trap_BotTouchingGoal
import trap_BotChooseNBGItem
import trap_BotChooseLTGItem
import trap_BotGetSecondGoal
import trap_BotGetTopGoal
import trap_BotGoalName
import trap_BotDumpGoalStack
import trap_BotDumpAvoidGoals
import trap_BotEmptyGoalStack
import trap_BotPopGoal
import trap_BotPushGoal
import trap_BotResetAvoidGoals
import trap_BotRemoveFromAvoidGoals
import trap_BotResetGoalState
import trap_BotSetChatName
import trap_BotSetChatGender
import trap_BotLoadChatFile
import trap_BotReplaceSynonyms
import trap_UnifyWhiteSpaces
import trap_BotMatchVariable
import trap_BotFindMatch
import trap_StringContains
import trap_BotGetChatMessage
import trap_BotEnterChat
import trap_BotChatLength
import trap_BotReplyChat
import trap_BotNumInitialChats
import trap_BotInitialChat
import trap_BotNumConsoleMessages
import trap_BotNextConsoleMessage
import trap_BotRemoveConsoleMessage
import trap_BotQueueConsoleMessage
import trap_BotFreeChatState
import trap_BotAllocChatState
import trap_Characteristic_String
import trap_Characteristic_BInteger
import trap_Characteristic_Integer
import trap_Characteristic_BFloat
import trap_Characteristic_Float
import trap_BotFreeCharacter
import trap_BotLoadCharacter
import trap_EA_ResetInput
import trap_EA_GetInput
import trap_EA_EndRegular
import trap_EA_ForcePower
import trap_EA_Alt_Attack
import trap_EA_View
import trap_EA_Move
import trap_EA_DelayedJump
import trap_EA_Jump
import trap_EA_SelectWeapon
import trap_EA_MoveRight
import trap_EA_MoveLeft
import trap_EA_MoveBack
import trap_EA_MoveForward
import trap_EA_MoveDown
import trap_EA_MoveUp
import trap_EA_Crouch
import trap_EA_Respawn
import trap_EA_Use
import trap_EA_Attack
import trap_EA_Talk
import trap_EA_Gesture
import trap_EA_Action
import trap_EA_Command
import trap_EA_SayTeam
import trap_EA_Say
import trap_AAS_PredictClientMovement
import trap_AAS_Swimming
import trap_AAS_AlternativeRouteGoals
import trap_AAS_PredictRoute
import trap_AAS_EnableRoutingArea
import trap_AAS_AreaTravelTimeToGoalArea
import trap_AAS_AreaReachability
import trap_AAS_IntForBSPEpairKey
import trap_AAS_FloatForBSPEpairKey
import trap_AAS_VectorForBSPEpairKey
import trap_AAS_ValueForBSPEpairKey
import trap_AAS_NextBSPEntity
import trap_AAS_PointContents
import trap_AAS_TraceAreas
import trap_AAS_PointReachabilityAreaIndex
import trap_AAS_PointAreaNum
import trap_AAS_Time
import trap_AAS_PresenceTypeBoundingBox
import trap_AAS_Initialized
import trap_AAS_EntityInfo
import trap_AAS_AreaInfo
import trap_AAS_BBoxAreas
import trap_BotUserCommand
import trap_BotGetServerCommand
import trap_BotGetSnapshotEntity
import trap_BotLibTest
import trap_BotLibUpdateEntity
import trap_BotLibLoadMap
import trap_BotLibStartFrame
import trap_BotLibDefine
import trap_BotLibVarGet
import trap_BotLibVarSet
import trap_BotLibShutdown
import trap_BotLibSetup
import trap_DebugPolygonDelete
import trap_DebugPolygonCreate
import trap_GetEntityToken
import trap_GetUsercmd
import trap_BotFreeClient
import trap_BotAllocateClient
import trap_EntityContact
import trap_EntitiesInBox
import trap_UnlinkEntity
import trap_LinkEntity
import trap_AreasConnected
import trap_AdjustAreaPortalState
import trap_InPVSIgnorePortals
import trap_InPVS
import trap_PointContents
import trap_Trace
import trap_SetBrushModel
import trap_GetServerinfo
import trap_SetUserinfo
import trap_GetUserinfo
import trap_GetConfigstring
import trap_SetConfigstring
import trap_SendServerCommand
import trap_DropClient
import trap_LocateGameData
import trap_Cvar_VariableStringBuffer
import trap_Cvar_VariableValue
import trap_Cvar_VariableIntegerValue
import trap_Cvar_Set
import trap_Cvar_Update
import trap_Cvar_Register
import trap_SendConsoleCommand
import trap_FS_GetFileList
import trap_FS_FCloseFile
import trap_FS_Write
import trap_FS_Read
import trap_FS_FOpenFile
import trap_Args
import trap_Argv
import trap_Argc
import trap_Milliseconds
import trap_Error
import trap_Printf
import g_timeouttospec
import g_forceDodge
import g_dismember
import g_singlePlayer
import g_enableBreath
import g_enableDust
import g_rankings
import pmove_msec
import pmove_fixed
import g_smoothClients
import g_blueteam
import g_redteam
import g_debugUp
import g_debugRight
import g_debugForward
import g_filterBan
import g_banIPs
import g_teamForceBalance
import g_teamAutoJoin
import g_allowVote
import g_blood
import g_doWarmup
import g_warmup
import g_motd
import g_synchronousClients
import g_adaptRespawn
import g_weaponTeamRespawn
import g_weaponRespawn
import g_debugDamage
import g_debugAlloc
import g_debugMove
import g_inactivity
import g_forcerespawn
import g_quadfactor
import g_knockback
import g_speed
import g_gravity
import g_needpass
import g_password
import g_friendlySaber
import g_friendlyFire
import g_saberInterpolate
import g_capturelimit
import g_timelimit
import g_duel_fraglimit
import g_fraglimit
import g_duelWeaponDisable
import g_weaponDisable
import g_forcePowerDisable
import g_spawnInvulnerability
import g_forceRegenTime
import g_saberLocking
import g_privateDuel
import g_forceBasedTeams
import g_maxForceRank
import g_dmflags
import g_autoMapCycle
import g_restarted
import g_maxGameClients
import g_maxclients
import g_cheats
import g_dedicated
import g_gametype
import g_entities
import level
import Pickup_Team
import CheckTeamStatus
import TeamplayInfoMessage
import Team_GetLocationMsg
import Team_GetLocation
import SelectSagaSpawnPoint
import SelectCTFSpawnPoint
import Team_FreeEntity
import Team_ReturnFlag
import Team_InitGame
import Team_CheckHurtCarrier
import Team_FragBonuses
import Team_DroppedFlagThink
import AddTeamScore
import TeamColorString
import OtherTeamName
import TeamName
import OtherTeam
import BotAIStartFrame
import BotAIShutdownClient
import BotAISetupClient
import BotAILoadMap
import BotAIShutdown
import BotAISetup
import B_CleanupAlloc
import B_InitAlloc
import InFieldOfVision
import BotOrder
import OrgVisible
import InitSagaMode
import G_ClearClientLog
import G_LogExit
import G_LogWeaponOutput
import G_LogWeaponInit
import G_LogWeaponItem
import G_LogWeaponPowerup
import G_LogWeaponFrag
import G_LogWeaponDeath
import G_LogWeaponKill
import G_LogWeaponDamage
import G_LogWeaponFire
import G_LogWeaponPickup
import Jedi_DodgeEvasion
import ForceTelepathy
import ForceThrow
import ForceSeeing
import ForceTeamForceReplenish
import ForceTeamHeal
import ForceAbsorb
import ForceProtect
import ForceGrip
import ForceRage
import ForceSpeed
import ForceHeal
import ForcePowerUsableOn
import WP_ForcePowersUpdate
import WP_SpawnInitForcePowers
import WP_InitForcePowers
import WP_SaberInitBladeData
import WP_SaberCanBlock
import WP_SaberPositionUpdate
import WP_ForcePowerStop
import HasSetSaberOnly
import G_PreDefSound
import G_RefreshNextMap
import G_DoesMapSupportGametype
import BotInterbreedEndMatch
import Svcmd_BotList_f
import Svcmd_AddBot_f
import G_BotConnect
import G_RemoveQueuedBotBegin
import G_CheckBotSpawn
import G_GetBotInfoByName
import G_GetBotInfoByNumber
import G_InitBots
import G_WriteSessionData
import G_InitWorldSession
import G_InitSessionData
import G_ReadSessionData
import Svcmd_GameMem_f
import G_InitMemory
import G_Alloc
import Team_CheckDroppedItem
import OnSameTeam
import G_RunClient
import ClientEndFrame
import ClientThink
import G_CheckClientTimeouts
import ClientCommand
import ClientBegin
import ClientDisconnect
import ClientUserinfoChanged
import ClientConnect
import G_GetStripEdString
import G_Error
import G_Printf
import SendScoreboardMessageToAllClients
import G_LogPrintf
import G_RunThink
import CheckTeamLeader
import SetLeader
import FindIntermissionPoint
import g_ff_objectives
import DeathmatchScoreboardMessage
import G_SetStats
import MoveClientToIntermission
import BlowDetpacks
import FireWeapon
import G_FilterPacket
import G_ProcessIPBans
import ConsoleCommand
import gJMSaberEnt
import SpotWouldTelefrag
import CalculateRanks
import AddScore
import player_die
import ClientSpawn
import InitBodyQue
import BeginIntermission
import respawn
import CopyToBodyQue
import SelectSpawnPoint
import SetClientViewAngle
import PickTeam
import TeamLeader
import TeamCount
import CheckGauntletAttack
import SnapVectorTowards
import CalcMuzzlePoint
import LogAccuracyHit
import WP_FireGenericBlasterMissile
import WP_FireTurretMissile
import G_PlayerBecomeATST
import ATST_ManageDamageBoxes
import TeleportPlayer
import trigger_teleporter_touch
import Touch_DoorTrigger
import G_RunMover
import WP_FireBlasterMissile
import G_ExplodeMissile
import G_BounceProjectile
import CreateMissile
import G_RunMissile
import G_ReflectMissile
import ExplodeDeath
import TossClientCubes
import TossClientItems
import TossClientWeapon
import body_die
import G_RadiusDamage
import G_Damage
import CanDamage
import trap_G2API_SetBoneAnim
import trap_G2API_GetGLAName
import trap_G2API_SetBoneAngles
import trap_G2API_CleanGhoul2Models
import trap_G2API_RemoveGhoul2Model
import trap_G2API_HasGhoul2ModelOnIndex
import trap_G2API_DuplicateGhoul2Instance
import trap_G2API_CopySpecificGhoul2Model
import trap_G2API_CopyGhoul2Instance
import trap_G2API_SetBoltInfo
import trap_G2API_AddBolt
import trap_G2API_InitGhoul2Model
import trap_G2API_GetBoltMatrix_NoReconstruct
import trap_G2API_GetBoltMatrix
import trap_G2_HaveWeGhoul2Models
import trap_G2_SetGhoul2ModelIndexes
import trap_G2_ListModelBones
import trap_G2_ListModelSurfaces
import G_SkinIndex
import BuildShaderStateConfig
import AddRemap
import G_SetOrigin
import G_AddEvent
import G_AddPredictableEvent
import vtos
import tv
import G_RunObject
import G_TouchSolids
import G_TouchTriggers
import G_EntitiesFree
import G_FreeEntity
import G_KillG2Queue
import G_SendG2KillQueue
import TryUse
import G_EntitySound
import G_SoundAtLoc
import G_Sound
import G_MuteSound
import G_ScreenShake
import G_PlayEffect
import G_TempEntity
import G_Spawn
import G_InitGentity
import G_SetAngles
import G_SetMovedir
import G_UseTargets
import G_PickTarget
import G_RadiusList
import G_Find
import G_KillBox
import G_TeamCommand
import G_EffectIndex
import G_SoundIndex
import G_ModelIndex
import SaveRegisteredItems
import RegisterItem
import ClearRegisteredItems
import Touch_Item
import Add_Ammo
import ArmorIndex
import Think_Weapon
import FinishSpawningItem
import G_SpawnItem
import SetRespawn
import LaunchItem
import Drop_Item
import PrecacheItem
import UseHoldableItem
import RespawnItem
import G_RunItem
import G_CheckTeamItems
import ItemUse_MedPack
import ItemUse_Seeker
import ItemUse_Sentry
import ItemUse_Shield
import ItemUse_Binoculars
import G_GetDuelWinner
import Cmd_EngageDuel_f
import Cmd_ToggleSaber_f
import G_ItemUsable
import Cmd_SaberAttackCycle_f
import Cmd_FollowCycle_f
import SetTeam
import BroadcastTeamChange
import StopFollowing
import Cmd_Score_f
import G_NewString
import G_SpawnEntitiesFromString
import G_SpawnVector
import G_SpawnInt
import G_SpawnFloat
import G_SpawnString
import g2SaberInstance
import precachedKyle
import forcePowerDarkLight
import WeaponAttackAnim
import WeaponReadyAnim
import BG_OutOfMemory
import BG_StringAlloc
import BG_TempFree
import BG_TempAlloc
import BG_AllocUnaligned
import BG_Alloc
import BG_CanUseFPNow
import BG_HasYsalamiri
import BG_GetItemIndexByTag
import BG_ParseAnimationFile
import BG_PlayerTouchesItem
import BG_G2PlayerAngles
import BG_PlayerStateToEntityStateExtraPolate
import BG_PlayerStateToEntityState
import BG_TouchJumpPad
import BG_AddPredictableEventToPlayerstate
import BG_EvaluateTrajectoryDelta
import BG_EvaluateTrajectory
import BG_ForcePowerDrain
import BG_SaberStartTransAnim
import BG_InDeathAnim
import BG_InRoll
import BG_SaberInSpecialAttack
import BG_SpinningSaberAnim
import BG_FlippingAnim
import BG_SaberInIdle
import BG_SaberInSpecial
import BG_SaberInAttack
import BG_DirectFlippingAnim
import BG_InSaberStandAnim
import BG_InSpecialJump
import BG_LegalizedForcePowers
import saberMoveData
import BG_CanItemBeGrabbed
import BG_FindItemForHoldable
import BG_FindItemForPowerup
import BG_FindItemForWeapon
import BG_FindItem
import vectoyaw
import bg_numItems
import bg_itemlist
import Pmove
import PM_UpdateViewAngles
import pm
import bgForcePowerCost
import forceMasteryPoints
import forceMasteryLevels
import bgGlobalAnimations
import BGPAFtextLoaded
import forcePowerSorted
import WP_MuzzlePoint
import ammoData
import weaponData
import GetStringForID
import GetIDForString
import Q_irand
import irand
import flrand
import Rand_Init
import Com_Printf
import Com_Error
import Info_NextPair
import Info_Validate
import Info_SetValueForKey_Big
import Info_SetValueForKey
import Info_RemoveKey_big
import Info_RemoveKey
import Info_ValueForKey
import va
import Q_CleanStr
import Q_PrintStrlen
import Q_strcat
import Q_strncpyz
import Q_strrchr
import Q_strupr
import Q_strlwr
import Q_stricmpn
import Q_strncmp
import Q_stricmp
import Q_isalpha
import Q_isupper
import Q_islower
import Q_isprint
import Com_sprintf
import Parse3DMatrix
import Parse2DMatrix
import Parse1DMatrix
import SkipRestOfLine
import SkipBracedSection
import COM_MatchToken
import COM_ParseWarning
import COM_ParseError
import COM_Compress
import COM_ParseExt
import COM_Parse
import SkipWhitespace
import COM_GetCurrentParseLine
import COM_BeginParseSession
import COM_DefaultExtension
import COM_StripExtension
import COM_SkipPath
import Com_Clamp
import PerpendicularVector
import AngleVectors
import MatrixMultiply
import MakeNormalVectors
import RotateAroundDirection
import RotatePointAroundVector
import ProjectPointOnPlane
import PlaneFromPoints
import AngleDelta
import AngleNormalize180
import AngleNormalize360
import AnglesSubtract
import AngleSubtract
import LerpAngle
import AngleMod
import BoxOnPlaneSide
import SetPlaneSignbits
import AxisCopy
import AxisClear
import AnglesToAxis
import vectoangles
import Q_crandom
import Q_random
import Q_rand
import Q_acos
import Q_log2
import VectorRotate
import Vector4Scale
import VectorNormalize2
import VectorNormalize
import CrossProduct
import VectorInverse
import VectorNormalizeFast
import DistanceSquared
import Distance
import VectorLengthSquared
import VectorLength
import VectorCompare
import AddPointToBounds
import ClearBounds
import RadiusFromBounds
import NormalizeColor
import ColorBytes4
import ColorBytes3
import _VectorMA
import _VectorScale
import _VectorCopy
import _VectorAdd
import _VectorSubtract
import _DotProduct
import ByteToDir
import DirToByte
import powf
import ClampShort
import ClampChar
import Q_rsqrt
import Q_fabs
import axisDefault
import vec3_origin
import g_color_table
import colorDkBlue
import colorLtBlue
import colorDkGrey
import colorMdGrey
import colorLtGrey
import colorWhite
import colorCyan
import colorMagenta
import colorYellow
import colorBlue
import colorGreen
import colorRed
import colorBlack
import colorTable
import bytedirs
import Com_Memcpy
import Com_Memset
import Hunk_Alloc
import forceSpeedLevels
import FloatSwap
import LongSwap
import ShortSwap
import acos
import fabs
import abs
import tan
import atan2
import cos
import sin
import sqrt
import floor
import ceil
import memcpy
import memset
import memmove
import sscanf
import vsprintf
import _atoi
import atoi
import _atof
import atof
import toupper
import tolower
import strncpy
import strstr
import strchr
import strcmp
import strcpy
import strcat
import strlen
import rand
import srand
import qsort
lit
align 1
LABELV $141
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 0
align 1
LABELV $132
char 1 112
char 1 111
char 1 115
char 1 116
char 1 103
char 1 97
char 1 109
char 1 101
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 0
align 1
LABELV $94
char 1 112
char 1 111
char 1 115
char 1 116
char 1 103
char 1 97
char 1 109
char 1 101
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 32
char 1 48
char 1 32
char 1 48
char 1 32
char 1 48
char 1 32
char 1 48
char 1 32
char 1 48
char 1 32
char 1 48
char 1 32
char 1 48
char 1 32
char 1 48
char 1 32
char 1 48
char 1 32
char 1 48
char 1 32
char 1 48
char 1 0
