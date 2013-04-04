export InitSagaMode
code
proc InitSagaMode 9072 16
file "../g_saga.c"
line 23
;1:#include "g_local.h"
;2:#include "bg_saga.h"
;3:
;4:char		saga_info[MAX_SAGA_INFO_SIZE];
;5:int			saga_valid;
;6:int			saga_round_over;
;7:
;8:int			imperial_goals_required;
;9:int			imperial_goals_completed;
;10:int			rebel_goals_required;
;11:int			rebel_goals_completed;
;12:
;13:int			rebel_attackers;
;14:int			imperial_attackers;
;15:
;16:void LogExit( const char *string );
;17:
;18:int GetValueGroup(char *buf, char *group, char *outbuf);
;19:int GetPairedValue(char *buf, char *key, char *outbuf);
;20://Using the bot parsing functions on saga ini's, at least for now.
;21:
;22:void InitSagaMode(void)
;23:{
line 28
;24:	vmCvar_t		mapname;
;25:	char			levelname[512];
;26:	char			goalreq[64];
;27:	char			objectives[MAX_SAGA_INFO_SIZE];
;28:	int				len = 0;
ADDRLP4 8704
CNSTI4 0
ASGNI4
line 31
;29:	fileHandle_t	f;
;30:
;31:	if (g_gametype.integer != GT_SAGA)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 6
EQI4 $79
line 32
;32:	{
line 33
;33:		goto failure;
ADDRGP4 $82
JUMPV
LABELV $79
line 36
;34:	}
;35:
;36:	imperial_goals_completed = 0;
ADDRGP4 imperial_goals_completed
CNSTI4 0
ASGNI4
line 37
;37:	rebel_goals_completed = 0;
ADDRGP4 rebel_goals_completed
CNSTI4 0
ASGNI4
line 39
;38:
;39:	saga_round_over = 0;
ADDRGP4 saga_round_over
CNSTI4 0
ASGNI4
line 41
;40:
;41:	trap_Cvar_Register( &mapname, "mapname", "", CVAR_SERVERINFO | CVAR_ROM );
ADDRLP4 8776
ARGP4
ADDRGP4 $83
ARGP4
ADDRGP4 $84
ARGP4
CNSTI4 68
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 43
;42:
;43:	Com_sprintf(levelname, sizeof(levelname), "maps/%s.saga\0", mapname.string);
ADDRLP4 0
ARGP4
CNSTI4 512
ARGI4
ADDRGP4 $85
ARGP4
ADDRLP4 8776+16
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 45
;44:
;45:	if (!levelname || !levelname[0])
ADDRLP4 0
CVPU4 4
CNSTU4 0
EQU4 $89
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $87
LABELV $89
line 46
;46:	{
line 47
;47:		goto failure;
ADDRGP4 $82
JUMPV
LABELV $87
line 50
;48:	}
;49:
;50:	len = trap_FS_FOpenFile(levelname, &f, FS_READ);
ADDRLP4 0
ARGP4
ADDRLP4 8772
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 9048
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 8704
ADDRLP4 9048
INDIRI4
ASGNI4
line 52
;51:
;52:	if (!f || len >= MAX_SAGA_INFO_SIZE)
ADDRLP4 8772
INDIRI4
CNSTI4 0
EQI4 $92
ADDRLP4 8704
INDIRI4
CNSTI4 8192
LTI4 $90
LABELV $92
line 53
;53:	{
line 54
;54:		goto failure;
ADDRGP4 $82
JUMPV
LABELV $90
line 57
;55:	}
;56:
;57:	trap_FS_Read(saga_info, len, f);
ADDRGP4 saga_info
ARGP4
ADDRLP4 8704
INDIRI4
ARGI4
ADDRLP4 8772
INDIRI4
ARGI4
ADDRGP4 trap_FS_Read
CALLV
pop
line 59
;58:
;59:	saga_valid = 1;
ADDRGP4 saga_valid
CNSTI4 1
ASGNI4
line 61
;60:
;61:	if (GetValueGroup(saga_info, "Rebel", objectives))
ADDRGP4 saga_info
ARGP4
ADDRGP4 $95
ARGP4
ADDRLP4 512
ARGP4
ADDRLP4 9052
ADDRGP4 GetValueGroup
CALLI4
ASGNI4
ADDRLP4 9052
INDIRI4
CNSTI4 0
EQI4 $93
line 62
;62:	{
line 63
;63:		if (GetPairedValue(objectives, "RequiredObjectives", goalreq))
ADDRLP4 512
ARGP4
ADDRGP4 $98
ARGP4
ADDRLP4 8708
ARGP4
ADDRLP4 9056
ADDRGP4 GetPairedValue
CALLI4
ASGNI4
ADDRLP4 9056
INDIRI4
CNSTI4 0
EQI4 $96
line 64
;64:		{
line 65
;65:			rebel_goals_required = atoi(goalreq);
ADDRLP4 8708
ARGP4
ADDRLP4 9060
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRGP4 rebel_goals_required
ADDRLP4 9060
INDIRI4
ASGNI4
line 66
;66:		}
LABELV $96
line 67
;67:		if (GetPairedValue(objectives, "attackers", goalreq))
ADDRLP4 512
ARGP4
ADDRGP4 $101
ARGP4
ADDRLP4 8708
ARGP4
ADDRLP4 9060
ADDRGP4 GetPairedValue
CALLI4
ASGNI4
ADDRLP4 9060
INDIRI4
CNSTI4 0
EQI4 $99
line 68
;68:		{
line 69
;69:			rebel_attackers = atoi(goalreq);
ADDRLP4 8708
ARGP4
ADDRLP4 9064
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRGP4 rebel_attackers
ADDRLP4 9064
INDIRI4
ASGNI4
line 70
;70:		}
LABELV $99
line 71
;71:	}
LABELV $93
line 72
;72:	if (GetValueGroup(saga_info, "Imperial", objectives))
ADDRGP4 saga_info
ARGP4
ADDRGP4 $104
ARGP4
ADDRLP4 512
ARGP4
ADDRLP4 9056
ADDRGP4 GetValueGroup
CALLI4
ASGNI4
ADDRLP4 9056
INDIRI4
CNSTI4 0
EQI4 $78
line 73
;73:	{
line 74
;74:		if (GetPairedValue(objectives, "RequiredObjectives", goalreq))
ADDRLP4 512
ARGP4
ADDRGP4 $98
ARGP4
ADDRLP4 8708
ARGP4
ADDRLP4 9060
ADDRGP4 GetPairedValue
CALLI4
ASGNI4
ADDRLP4 9060
INDIRI4
CNSTI4 0
EQI4 $105
line 75
;75:		{
line 76
;76:			imperial_goals_required = atoi(goalreq);
ADDRLP4 8708
ARGP4
ADDRLP4 9064
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRGP4 imperial_goals_required
ADDRLP4 9064
INDIRI4
ASGNI4
line 77
;77:		}
LABELV $105
line 78
;78:		if (GetPairedValue(objectives, "attackers", goalreq))
ADDRLP4 512
ARGP4
ADDRGP4 $101
ARGP4
ADDRLP4 8708
ARGP4
ADDRLP4 9064
ADDRGP4 GetPairedValue
CALLI4
ASGNI4
ADDRLP4 9064
INDIRI4
CNSTI4 0
EQI4 $78
line 79
;79:		{
line 80
;80:			imperial_attackers = atoi(goalreq);
ADDRLP4 8708
ARGP4
ADDRLP4 9068
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRGP4 imperial_attackers
ADDRLP4 9068
INDIRI4
ASGNI4
line 81
;81:		}
line 82
;82:	}
line 84
;83:
;84:	return;
ADDRGP4 $78
JUMPV
LABELV $82
line 87
;85:
;86:failure:
;87:	saga_valid = 0;
ADDRGP4 saga_valid
CNSTI4 0
ASGNI4
line 88
;88:}
LABELV $78
endproc InitSagaMode 9072 16
export UseSagaTarget
proc UseSagaTarget 24 12
line 91
;89:
;90:void UseSagaTarget(gentity_t *other, gentity_t *en, char *target)
;91:{ //actually use the player which triggered the object which triggered the saga objective to trigger the target
line 95
;92:	gentity_t		*t;
;93:	gentity_t		*ent;
;94:
;95:	if ( !en || !en->client )
ADDRLP4 8
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 12
CNSTU4 0
ASGNU4
ADDRLP4 8
INDIRP4
CVPU4 4
ADDRLP4 12
INDIRU4
EQU4 $112
ADDRLP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 12
INDIRU4
NEU4 $110
LABELV $112
line 96
;96:	{ //looks like we don't have access to a player, so just use the activating entity
line 97
;97:		ent = other;
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
line 98
;98:	}
ADDRGP4 $111
JUMPV
LABELV $110
line 100
;99:	else
;100:	{
line 101
;101:		ent = en;
ADDRLP4 4
ADDRFP4 4
INDIRP4
ASGNP4
line 102
;102:	}
LABELV $111
line 104
;103:
;104:	if (!en)
ADDRFP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $113
line 105
;105:	{
line 106
;106:		return;
ADDRGP4 $109
JUMPV
LABELV $113
line 109
;107:	}
;108:
;109:	if ( !target )
ADDRFP4 8
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $115
line 110
;110:	{
line 111
;111:		return;
ADDRGP4 $109
JUMPV
LABELV $115
line 114
;112:	}
;113:
;114:	t = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
ADDRGP4 $118
JUMPV
LABELV $117
line 116
;115:	while ( (t = G_Find (t, FOFS(targetname), target)) != NULL )
;116:	{
line 117
;117:		if ( t == ent )
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRP4
CVPU4 4
NEU4 $120
line 118
;118:		{
line 119
;119:			G_Printf ("WARNING: Entity used itself.\n");
ADDRGP4 $122
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 120
;120:		}
ADDRGP4 $121
JUMPV
LABELV $120
line 122
;121:		else
;122:		{
line 123
;123:			if ( t->use )
ADDRLP4 0
INDIRP4
CNSTI4 652
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $123
line 124
;124:			{
line 125
;125:				t->use (t, ent, ent);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 652
ADDP4
INDIRP4
CALLV
pop
line 126
;126:			}
LABELV $123
line 127
;127:		}
LABELV $121
line 128
;128:		if ( !ent->inuse )
ADDRLP4 4
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
NEI4 $125
line 129
;129:		{
line 130
;130:			G_Printf("entity was removed while using targets\n");
ADDRGP4 $127
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 131
;131:			return;
ADDRGP4 $109
JUMPV
LABELV $125
line 133
;132:		}
;133:	}
LABELV $118
line 115
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 588
ARGI4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 16
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $117
line 134
;134:}
LABELV $109
endproc UseSagaTarget 24 12
export SagaBroadcast_OBJECTIVECOMPLETE
proc SagaBroadcast_OBJECTIVECOMPLETE 28 8
line 137
;135:
;136:void SagaBroadcast_OBJECTIVECOMPLETE(int team, int client, int objective)
;137:{
line 141
;138:	gentity_t *te;
;139:	vec3_t nomatter;
;140:
;141:	VectorClear(nomatter);
ADDRLP4 16
CNSTF4 0
ASGNF4
ADDRLP4 4+8
ADDRLP4 16
INDIRF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 16
INDIRF4
ASGNF4
ADDRLP4 4
ADDRLP4 16
INDIRF4
ASGNF4
line 143
;142:
;143:	te = G_TempEntity( nomatter, EV_SAGA_OBJECTIVECOMPLETE );
ADDRLP4 4
ARGP4
CNSTI4 89
ARGI4
ADDRLP4 20
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
ASGNP4
line 144
;144:	te->r.svFlags |= SVF_BROADCAST;
ADDRLP4 24
ADDRLP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 145
;145:	te->s.eventParm = team;
ADDRLP4 0
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 0
INDIRI4
ASGNI4
line 146
;146:	te->s.weapon = client;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 147
;147:	te->s.trickedentindex = objective;
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 148
;148:}
LABELV $128
endproc SagaBroadcast_OBJECTIVECOMPLETE 28 8
export SagaBroadcast_ROUNDOVER
proc SagaBroadcast_ROUNDOVER 28 8
line 151
;149:
;150:void SagaBroadcast_ROUNDOVER(int winningteam, int winningclient)
;151:{
line 155
;152:	gentity_t *te;
;153:	vec3_t nomatter;
;154:
;155:	VectorClear(nomatter);
ADDRLP4 16
CNSTF4 0
ASGNF4
ADDRLP4 4+8
ADDRLP4 16
INDIRF4
ASGNF4
ADDRLP4 4+4
ADDRLP4 16
INDIRF4
ASGNF4
ADDRLP4 4
ADDRLP4 16
INDIRF4
ASGNF4
line 157
;156:
;157:	te = G_TempEntity( nomatter, EV_SAGA_ROUNDOVER );
ADDRLP4 4
ARGP4
CNSTI4 88
ARGI4
ADDRLP4 20
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
ASGNP4
line 158
;158:	te->r.svFlags |= SVF_BROADCAST;
ADDRLP4 24
ADDRLP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 24
INDIRP4
ADDRLP4 24
INDIRP4
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 159
;159:	te->s.eventParm = winningteam;
ADDRLP4 0
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 0
INDIRI4
ASGNI4
line 160
;160:	te->s.weapon = winningclient;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 161
;161:}
LABELV $131
endproc SagaBroadcast_ROUNDOVER 28 8
export BroadcastObjectiveCompletion
proc BroadcastObjectiveCompletion 12 12
line 164
;162:
;163:void BroadcastObjectiveCompletion(int team, int objective, int final, int client)
;164:{
line 165
;165:	if (client != ENTITYNUM_NONE && g_entities[client].client && g_entities[client].client->sess.sessionTeam == team)
ADDRLP4 0
ADDRFP4 12
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 1023
EQI4 $135
ADDRLP4 4
CNSTI4 828
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 4
INDIRI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $135
ADDRLP4 4
INDIRI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $135
line 166
;166:	{ //guy who completed this objective gets points, providing he's on the opposing team
line 167
;167:		AddScore(&g_entities[client], g_entities[client].client->ps.origin, SAGA_POINTS_OBJECTIVECOMPLETED);
ADDRLP4 8
CNSTI4 828
ADDRFP4 12
INDIRI4
MULI4
ASGNI4
ADDRLP4 8
INDIRI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRLP4 8
INDIRI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
CNSTI4 3
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 168
;168:	}
LABELV $135
line 170
;169:	
;170:	SagaBroadcast_OBJECTIVECOMPLETE(team, client, objective);
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 SagaBroadcast_OBJECTIVECOMPLETE
CALLV
pop
line 172
;171:	//G_Printf("Broadcast goal completion team %i objective %i final %i\n", team, objective, final);
;172:}
LABELV $134
endproc BroadcastObjectiveCompletion 12 12
export AddSagaWinningTeamPoints
proc AddSagaWinningTeamPoints 24 12
line 175
;173:
;174:void AddSagaWinningTeamPoints(int team, int winner)
;175:{
line 176
;176:	int i = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $142
JUMPV
LABELV $141
line 180
;177:	gentity_t *ent;
;178:
;179:	while (i < MAX_CLIENTS)
;180:	{
line 181
;181:		ent = &g_entities[i];
ADDRLP4 0
CNSTI4 828
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 183
;182:
;183:		if (ent && ent->client && ent->client->sess.sessionTeam == team)
ADDRLP4 12
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 12
INDIRU4
EQU4 $144
ADDRLP4 16
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CVPU4 4
ADDRLP4 12
INDIRU4
EQU4 $144
ADDRLP4 16
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $144
line 184
;184:		{
line 185
;185:			if (i == winner)
ADDRLP4 4
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $146
line 186
;186:			{
line 187
;187:				AddScore(ent, ent->client->ps.origin, SAGA_POINTS_TEAMWONROUND+SAGA_POINTS_FINALOBJECTIVECOMPLETED);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
CNSTI4 10
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 188
;188:			}
ADDRGP4 $147
JUMPV
LABELV $146
line 190
;189:			else
;190:			{
line 191
;191:				AddScore(ent, ent->client->ps.origin, SAGA_POINTS_TEAMWONROUND);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
ARGP4
CNSTI4 5
ARGI4
ADDRGP4 AddScore
CALLV
pop
line 192
;192:			}
LABELV $147
line 193
;193:		}
LABELV $144
line 195
;194:
;195:		i++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 196
;196:	}
LABELV $142
line 179
ADDRLP4 4
INDIRI4
CNSTI4 32
LTI4 $141
line 197
;197:}
LABELV $140
endproc AddSagaWinningTeamPoints 24 12
export SagaRoundComplete
proc SagaRoundComplete 24 8
line 200
;198:
;199:void SagaRoundComplete(int winningteam, int winningclient)
;200:{
line 203
;201:	vec3_t nomatter;
;202:
;203:	saga_round_over = 1;
ADDRGP4 saga_round_over
CNSTI4 1
ASGNI4
line 207
;204:
;205:	//G_Printf("Team %i won\n", winningteam);
;206:
;207:	if (winningclient != ENTITYNUM_NONE && g_entities[winningclient].client &&
ADDRLP4 12
ADDRFP4 4
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 1023
EQI4 $149
ADDRLP4 16
CNSTI4 828
ADDRLP4 12
INDIRI4
MULI4
ASGNI4
ADDRLP4 16
INDIRI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $149
ADDRLP4 16
INDIRI4
ADDRGP4 g_entities+408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
EQI4 $149
line 209
;208:		g_entities[winningclient].client->sess.sessionTeam != winningteam)
;209:	{ //this person just won the round for the other team..
line 210
;210:		winningclient = ENTITYNUM_NONE;
ADDRFP4 4
CNSTI4 1023
ASGNI4
line 211
;211:	}
LABELV $149
line 213
;212:
;213:	VectorClear(nomatter);
ADDRLP4 20
CNSTF4 0
ASGNF4
ADDRLP4 0+8
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 20
INDIRF4
ASGNF4
ADDRLP4 0
ADDRLP4 20
INDIRF4
ASGNF4
line 215
;214:
;215:	SagaBroadcast_ROUNDOVER(winningteam, winningclient);
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 SagaBroadcast_ROUNDOVER
CALLV
pop
line 217
;216:
;217:	AddSagaWinningTeamPoints(winningteam, winningclient);
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 AddSagaWinningTeamPoints
CALLV
pop
line 219
;218:
;219:	if (winningteam == SAGATEAM_IMPERIAL)
ADDRFP4 0
INDIRI4
CNSTI4 1
NEI4 $155
line 220
;220:	{
line 221
;221:		LogExit( "The Imperials completed their final objective." );
ADDRGP4 $157
ARGP4
ADDRGP4 LogExit
CALLV
pop
line 222
;222:	}
ADDRGP4 $156
JUMPV
LABELV $155
line 224
;223:	else
;224:	{
line 225
;225:		LogExit( "The Rebels completed their final objective." );
ADDRGP4 $158
ARGP4
ADDRGP4 LogExit
CALLV
pop
line 226
;226:	}
LABELV $156
line 227
;227:}
LABELV $148
endproc SagaRoundComplete 24 8
export SagaObjectiveCompleted
proc SagaObjectiveCompleted 12 16
line 230
;228:
;229:void SagaObjectiveCompleted(int team, int objective, int final, int client)
;230:{
line 233
;231:	int goals_completed, goals_required;
;232:
;233:	if (saga_round_over)
ADDRGP4 saga_round_over
INDIRI4
CNSTI4 0
EQI4 $160
line 234
;234:	{
line 235
;235:		return;
ADDRGP4 $159
JUMPV
LABELV $160
line 238
;236:	}
;237:
;238:	if (final != -1)
ADDRFP4 8
INDIRI4
CNSTI4 -1
EQI4 $162
line 239
;239:	{
line 240
;240:		if (team == SAGATEAM_IMPERIAL)
ADDRFP4 0
INDIRI4
CNSTI4 1
NEI4 $164
line 241
;241:		{
line 242
;242:			imperial_goals_completed++;
ADDRLP4 8
ADDRGP4 imperial_goals_completed
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 243
;243:		}
ADDRGP4 $165
JUMPV
LABELV $164
line 245
;244:		else
;245:		{
line 246
;246:			rebel_goals_completed++;
ADDRLP4 8
ADDRGP4 rebel_goals_completed
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 247
;247:		}
LABELV $165
line 248
;248:	}
LABELV $162
line 250
;249:
;250:	if (team == SAGATEAM_IMPERIAL)
ADDRFP4 0
INDIRI4
CNSTI4 1
NEI4 $166
line 251
;251:	{
line 252
;252:		goals_completed = imperial_goals_completed;
ADDRLP4 0
ADDRGP4 imperial_goals_completed
INDIRI4
ASGNI4
line 253
;253:		goals_required = imperial_goals_required;
ADDRLP4 4
ADDRGP4 imperial_goals_required
INDIRI4
ASGNI4
line 254
;254:	}
ADDRGP4 $167
JUMPV
LABELV $166
line 256
;255:	else
;256:	{
line 257
;257:		goals_completed = rebel_goals_completed;
ADDRLP4 0
ADDRGP4 rebel_goals_completed
INDIRI4
ASGNI4
line 258
;258:		goals_required = rebel_goals_required;
ADDRLP4 4
ADDRGP4 rebel_goals_required
INDIRI4
ASGNI4
line 259
;259:	}
LABELV $167
line 261
;260:
;261:	if (final == 1 || goals_completed >= goals_required)
ADDRFP4 8
INDIRI4
CNSTI4 1
EQI4 $170
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
LTI4 $168
LABELV $170
line 262
;262:	{
line 263
;263:		SagaRoundComplete(team, client);
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 SagaRoundComplete
CALLV
pop
line 264
;264:	}
ADDRGP4 $169
JUMPV
LABELV $168
line 266
;265:	else
;266:	{
line 267
;267:		BroadcastObjectiveCompletion(team, objective, final, client);
ADDRFP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 BroadcastObjectiveCompletion
CALLV
pop
line 268
;268:	}
LABELV $169
line 269
;269:}
LABELV $159
endproc SagaObjectiveCompleted 12 16
export StripTabs
proc StripTabs 12 0
line 272
;270:
;271:void StripTabs(char *buf)
;272:{
line 273
;273:	int i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 274
;274:	int i_r = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $173
JUMPV
LABELV $172
line 277
;275:
;276:	while (buf[i])
;277:	{
line 278
;278:		if (buf[i] != 9)
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 9
EQI4 $175
line 279
;279:		{
line 280
;280:			buf[i_r] = buf[i];
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
INDIRI4
ADDRLP4 8
INDIRP4
ADDP4
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRP4
ADDP4
INDIRI1
ASGNI1
line 281
;281:			i_r++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 282
;282:		}
LABELV $175
line 284
;283:
;284:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 285
;285:	}
LABELV $173
line 276
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $172
line 286
;286:	buf[i_r] = '\0';
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
CNSTI1 0
ASGNI1
line 287
;287:}
LABELV $171
endproc StripTabs 12 0
export sagaTriggerUse
proc sagaTriggerUse 16556 16
line 290
;288:
;289:void sagaTriggerUse(gentity_t *ent, gentity_t *other, gentity_t *activator)
;290:{
line 295
;291:	char			teamstr[64];
;292:	char			objectivestr[64];
;293:	char			objectives[MAX_SAGA_INFO_SIZE];
;294:	char			desiredobjective[MAX_SAGA_INFO_SIZE];
;295:	int				clUser = ENTITYNUM_NONE;
ADDRLP4 8260
CNSTI4 1023
ASGNI4
line 296
;296:	int				final = 0;
ADDRLP4 8328
CNSTI4 0
ASGNI4
line 297
;297:	int				i = 0;
ADDRLP4 64
CNSTI4 0
ASGNI4
line 299
;298:
;299:	if (!saga_valid)
ADDRGP4 saga_valid
INDIRI4
CNSTI4 0
NEI4 $178
line 300
;300:	{
line 301
;301:		return;
ADDRGP4 $177
JUMPV
LABELV $178
line 304
;302:	}
;303:
;304:	if (activator && activator->client)
ADDRLP4 16524
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 16528
CNSTU4 0
ASGNU4
ADDRLP4 16524
INDIRP4
CVPU4 4
ADDRLP4 16528
INDIRU4
EQU4 $180
ADDRLP4 16524
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 16528
INDIRU4
EQU4 $180
line 305
;305:	{ //activator will hopefully be the person who triggered this event
line 306
;306:		clUser = activator->s.number;
ADDRLP4 8260
ADDRFP4 8
INDIRP4
INDIRI4
ASGNI4
line 307
;307:	}
LABELV $180
line 309
;308:
;309:	if (ent->side == SAGATEAM_IMPERIAL)
ADDRFP4 0
INDIRP4
CNSTI4 440
ADDP4
INDIRI4
CNSTI4 1
NEI4 $182
line 310
;310:	{
line 311
;311:		Com_sprintf(teamstr, sizeof(teamstr), "Imperial");
ADDRLP4 0
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $104
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 312
;312:	}
ADDRGP4 $183
JUMPV
LABELV $182
line 314
;313:	else
;314:	{
line 315
;315:		Com_sprintf(teamstr, sizeof(teamstr), "Rebel");
ADDRLP4 0
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $95
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 316
;316:	}
LABELV $183
line 318
;317:
;318:	if (GetValueGroup(saga_info, teamstr, objectives))
ADDRGP4 saga_info
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 68
ARGP4
ADDRLP4 16532
ADDRGP4 GetValueGroup
CALLI4
ASGNI4
ADDRLP4 16532
INDIRI4
CNSTI4 0
EQI4 $184
line 319
;319:	{
line 320
;320:		Com_sprintf(objectivestr, sizeof(objectivestr), "Objective%i", ent->objective);
ADDRLP4 8264
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 $186
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 436
ADDP4
INDIRI4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 322
;321:
;322:		StripTabs(objectives); //tabs confuse the parsing function
ADDRLP4 68
ARGP4
ADDRGP4 StripTabs
CALLV
pop
line 324
;323:
;324:		if (GetValueGroup(objectives, objectivestr, desiredobjective))
ADDRLP4 68
ARGP4
ADDRLP4 8264
ARGP4
ADDRLP4 8332
ARGP4
ADDRLP4 16536
ADDRGP4 GetValueGroup
CALLI4
ASGNI4
ADDRLP4 16536
INDIRI4
CNSTI4 0
EQI4 $187
line 325
;325:		{
line 326
;326:			if (GetPairedValue(desiredobjective, "final", teamstr))
ADDRLP4 8332
ARGP4
ADDRGP4 $191
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 16540
ADDRGP4 GetPairedValue
CALLI4
ASGNI4
ADDRLP4 16540
INDIRI4
CNSTI4 0
EQI4 $189
line 327
;327:			{
line 328
;328:				final = atoi(teamstr);
ADDRLP4 0
ARGP4
ADDRLP4 16544
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 8328
ADDRLP4 16544
INDIRI4
ASGNI4
line 329
;329:			}
LABELV $189
line 331
;330:
;331:			if (GetPairedValue(desiredobjective, "target", teamstr))
ADDRLP4 8332
ARGP4
ADDRGP4 $194
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 16544
ADDRGP4 GetPairedValue
CALLI4
ASGNI4
ADDRLP4 16544
INDIRI4
CNSTI4 0
EQI4 $192
line 332
;332:			{
ADDRGP4 $196
JUMPV
LABELV $195
line 334
;333:				while (teamstr[i])
;334:				{
line 335
;335:					if (teamstr[i] == '\r' ||
ADDRLP4 16548
ADDRLP4 64
INDIRI4
ADDRLP4 0
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 16548
INDIRI4
CNSTI4 13
EQI4 $200
ADDRLP4 16548
INDIRI4
CNSTI4 10
NEI4 $198
LABELV $200
line 337
;336:						teamstr[i] == '\n')
;337:					{
line 338
;338:						teamstr[i] = '\0';
ADDRLP4 64
INDIRI4
ADDRLP4 0
ADDP4
CNSTI1 0
ASGNI1
line 339
;339:					}
LABELV $198
line 341
;340:
;341:					i++;
ADDRLP4 64
ADDRLP4 64
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 342
;342:				}
LABELV $196
line 333
ADDRLP4 64
INDIRI4
ADDRLP4 0
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $195
line 343
;343:				UseSagaTarget(other, activator, teamstr);
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 UseSagaTarget
CALLV
pop
line 344
;344:			}
LABELV $192
line 346
;345:
;346:			if (ent->target && ent->target[0])
ADDRLP4 16548
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
ASGNP4
ADDRLP4 16548
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $201
ADDRLP4 16548
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $201
line 347
;347:			{ //use this too
line 348
;348:				UseSagaTarget(other, activator, ent->target);
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
ARGP4
ADDRGP4 UseSagaTarget
CALLV
pop
line 349
;349:			}
LABELV $201
line 351
;350:
;351:			SagaObjectiveCompleted(ent->side, ent->objective, final, clUser);
ADDRLP4 16552
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16552
INDIRP4
CNSTI4 440
ADDP4
INDIRI4
ARGI4
ADDRLP4 16552
INDIRP4
CNSTI4 436
ADDP4
INDIRI4
ARGI4
ADDRLP4 8328
INDIRI4
ARGI4
ADDRLP4 8260
INDIRI4
ARGI4
ADDRGP4 SagaObjectiveCompleted
CALLV
pop
line 352
;352:		}
LABELV $187
line 353
;353:	}
LABELV $184
line 354
;354:}
LABELV $177
endproc sagaTriggerUse 16556 16
export SP_info_saga_objective
proc SP_info_saga_objective 8 12
line 362
;355:
;356:
;357:/*QUAKED info_saga_objective (1 0 1) (-16 -16 -24) (16 16 32)
;358:"objective" - specifies the objective to complete upon activation
;359:"side" - set to 1 to specify an imperial goal, 2 to specify rebels
;360:*/
;361:void SP_info_saga_objective (gentity_t *ent)
;362:{
line 363
;363:	if (!saga_valid || g_gametype.integer != GT_SAGA)
ADDRGP4 saga_valid
INDIRI4
CNSTI4 0
EQI4 $207
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 6
EQI4 $204
LABELV $207
line 364
;364:	{
line 365
;365:		G_FreeEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 366
;366:		return;
ADDRGP4 $203
JUMPV
LABELV $204
line 369
;367:	}
;368:
;369:	ent->use = sagaTriggerUse;
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
ADDRGP4 sagaTriggerUse
ASGNP4
line 370
;370:	G_SpawnInt( "objective", "0", &ent->objective);
ADDRGP4 $208
ARGP4
ADDRGP4 $209
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 436
ADDP4
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 371
;371:	G_SpawnInt( "side", "0", &ent->side);
ADDRGP4 $210
ARGP4
ADDRGP4 $209
ARGP4
ADDRFP4 0
INDIRP4
CNSTI4 440
ADDP4
ARGP4
ADDRGP4 G_SpawnInt
CALLI4
pop
line 373
;372:
;373:	if (!ent->objective || !ent->side)
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRP4
CNSTI4 436
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $213
ADDRLP4 0
INDIRP4
CNSTI4 440
ADDP4
INDIRI4
ADDRLP4 4
INDIRI4
NEI4 $211
LABELV $213
line 374
;374:	{ //j00 fux0red something up
line 375
;375:		G_FreeEntity(ent);
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 376
;376:		G_Printf("ERROR: info_saga_objective without an objective or side value\n");
ADDRGP4 $214
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 377
;377:		return;
LABELV $211
line 379
;378:	}
;379:}
LABELV $203
endproc SP_info_saga_objective 8 12
import GetPairedValue
import GetValueGroup
import LogExit
bss
export imperial_attackers
align 4
LABELV imperial_attackers
skip 4
export rebel_attackers
align 4
LABELV rebel_attackers
skip 4
export rebel_goals_completed
align 4
LABELV rebel_goals_completed
skip 4
export rebel_goals_required
align 4
LABELV rebel_goals_required
skip 4
export imperial_goals_completed
align 4
LABELV imperial_goals_completed
skip 4
export imperial_goals_required
align 4
LABELV imperial_goals_required
skip 4
export saga_round_over
align 4
LABELV saga_round_over
skip 4
export saga_valid
align 4
LABELV saga_valid
skip 4
export saga_info
align 1
LABELV saga_info
skip 8192
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
import UpdateTournamentInfo
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
LABELV $214
char 1 69
char 1 82
char 1 82
char 1 79
char 1 82
char 1 58
char 1 32
char 1 105
char 1 110
char 1 102
char 1 111
char 1 95
char 1 115
char 1 97
char 1 103
char 1 97
char 1 95
char 1 111
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 105
char 1 118
char 1 101
char 1 32
char 1 119
char 1 105
char 1 116
char 1 104
char 1 111
char 1 117
char 1 116
char 1 32
char 1 97
char 1 110
char 1 32
char 1 111
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 105
char 1 118
char 1 101
char 1 32
char 1 111
char 1 114
char 1 32
char 1 115
char 1 105
char 1 100
char 1 101
char 1 32
char 1 118
char 1 97
char 1 108
char 1 117
char 1 101
char 1 10
char 1 0
align 1
LABELV $210
char 1 115
char 1 105
char 1 100
char 1 101
char 1 0
align 1
LABELV $209
char 1 48
char 1 0
align 1
LABELV $208
char 1 111
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 105
char 1 118
char 1 101
char 1 0
align 1
LABELV $194
char 1 116
char 1 97
char 1 114
char 1 103
char 1 101
char 1 116
char 1 0
align 1
LABELV $191
char 1 102
char 1 105
char 1 110
char 1 97
char 1 108
char 1 0
align 1
LABELV $186
char 1 79
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 105
char 1 118
char 1 101
char 1 37
char 1 105
char 1 0
align 1
LABELV $158
char 1 84
char 1 104
char 1 101
char 1 32
char 1 82
char 1 101
char 1 98
char 1 101
char 1 108
char 1 115
char 1 32
char 1 99
char 1 111
char 1 109
char 1 112
char 1 108
char 1 101
char 1 116
char 1 101
char 1 100
char 1 32
char 1 116
char 1 104
char 1 101
char 1 105
char 1 114
char 1 32
char 1 102
char 1 105
char 1 110
char 1 97
char 1 108
char 1 32
char 1 111
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 105
char 1 118
char 1 101
char 1 46
char 1 0
align 1
LABELV $157
char 1 84
char 1 104
char 1 101
char 1 32
char 1 73
char 1 109
char 1 112
char 1 101
char 1 114
char 1 105
char 1 97
char 1 108
char 1 115
char 1 32
char 1 99
char 1 111
char 1 109
char 1 112
char 1 108
char 1 101
char 1 116
char 1 101
char 1 100
char 1 32
char 1 116
char 1 104
char 1 101
char 1 105
char 1 114
char 1 32
char 1 102
char 1 105
char 1 110
char 1 97
char 1 108
char 1 32
char 1 111
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 105
char 1 118
char 1 101
char 1 46
char 1 0
align 1
LABELV $127
char 1 101
char 1 110
char 1 116
char 1 105
char 1 116
char 1 121
char 1 32
char 1 119
char 1 97
char 1 115
char 1 32
char 1 114
char 1 101
char 1 109
char 1 111
char 1 118
char 1 101
char 1 100
char 1 32
char 1 119
char 1 104
char 1 105
char 1 108
char 1 101
char 1 32
char 1 117
char 1 115
char 1 105
char 1 110
char 1 103
char 1 32
char 1 116
char 1 97
char 1 114
char 1 103
char 1 101
char 1 116
char 1 115
char 1 10
char 1 0
align 1
LABELV $122
char 1 87
char 1 65
char 1 82
char 1 78
char 1 73
char 1 78
char 1 71
char 1 58
char 1 32
char 1 69
char 1 110
char 1 116
char 1 105
char 1 116
char 1 121
char 1 32
char 1 117
char 1 115
char 1 101
char 1 100
char 1 32
char 1 105
char 1 116
char 1 115
char 1 101
char 1 108
char 1 102
char 1 46
char 1 10
char 1 0
align 1
LABELV $104
char 1 73
char 1 109
char 1 112
char 1 101
char 1 114
char 1 105
char 1 97
char 1 108
char 1 0
align 1
LABELV $101
char 1 97
char 1 116
char 1 116
char 1 97
char 1 99
char 1 107
char 1 101
char 1 114
char 1 115
char 1 0
align 1
LABELV $98
char 1 82
char 1 101
char 1 113
char 1 117
char 1 105
char 1 114
char 1 101
char 1 100
char 1 79
char 1 98
char 1 106
char 1 101
char 1 99
char 1 116
char 1 105
char 1 118
char 1 101
char 1 115
char 1 0
align 1
LABELV $95
char 1 82
char 1 101
char 1 98
char 1 101
char 1 108
char 1 0
align 1
LABELV $85
char 1 109
char 1 97
char 1 112
char 1 115
char 1 47
char 1 37
char 1 115
char 1 46
char 1 115
char 1 97
char 1 103
char 1 97
char 1 0
char 1 0
align 1
LABELV $84
char 1 0
align 1
LABELV $83
char 1 109
char 1 97
char 1 112
char 1 110
char 1 97
char 1 109
char 1 101
char 1 0
