export trap_Cvar_VariableValue
code
proc trap_Cvar_VariableValue 132 12
file "../g_bot.c"
line 35
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:// g_bot.c
;4:
;5:#include "g_local.h"
;6:
;7:
;8:static int		g_numBots;
;9:static char		*g_botInfos[MAX_BOTS];
;10:
;11:
;12:int				g_numArenas;
;13:static char		*g_arenaInfos[MAX_ARENAS];
;14:
;15:
;16:#define BOT_BEGIN_DELAY_BASE		2000
;17:#define BOT_BEGIN_DELAY_INCREMENT	1500
;18:
;19:#define BOT_SPAWN_QUEUE_DEPTH	16
;20:
;21:typedef struct {
;22:	int		clientNum;
;23:	int		spawnTime;
;24:} botSpawnQueue_t;
;25:
;26://static int			botBeginDelay = 0;  // bk001206 - unused, init
;27:static botSpawnQueue_t	botSpawnQueue[BOT_SPAWN_QUEUE_DEPTH];
;28:
;29:vmCvar_t bot_minplayers;
;30:
;31:extern gentity_t	*podium1;
;32:extern gentity_t	*podium2;
;33:extern gentity_t	*podium3;
;34:
;35:float trap_Cvar_VariableValue( const char *var_name ) {
line 38
;36:	char buf[128];
;37:
;38:	trap_Cvar_VariableStringBuffer(var_name, buf, sizeof(buf));
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
CNSTI4 128
ARGI4
ADDRGP4 trap_Cvar_VariableStringBuffer
CALLV
pop
line 39
;39:	return atof(buf);
ADDRLP4 0
ARGP4
ADDRLP4 128
ADDRGP4 atof
CALLF4
ASGNF4
ADDRLP4 128
INDIRF4
RETF4
LABELV $79
endproc trap_Cvar_VariableValue 132 12
export G_ParseInfos
proc G_ParseInfos 2084 12
line 49
;40:}
;41:
;42:
;43:
;44:/*
;45:===============
;46:G_ParseInfos
;47:===============
;48:*/
;49:int G_ParseInfos( char *buf, int max, char *infos[] ) {
line 55
;50:	char	*token;
;51:	int		count;
;52:	char	key[MAX_TOKEN_CHARS];
;53:	char	info[MAX_INFO_STRING];
;54:
;55:	count = 0;
ADDRLP4 2052
CNSTI4 0
ASGNI4
ADDRGP4 $82
JUMPV
LABELV $81
line 57
;56:
;57:	while ( 1 ) {
line 58
;58:		token = COM_Parse( (const char **)(&buf) );
ADDRFP4 0
ARGP4
ADDRLP4 2056
ADDRGP4 COM_Parse
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 2056
INDIRP4
ASGNP4
line 59
;59:		if ( !token[0] ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $84
line 60
;60:			break;
ADDRGP4 $83
JUMPV
LABELV $84
line 62
;61:		}
;62:		if ( strcmp( token, "{" ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $88
ARGP4
ADDRLP4 2060
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 2060
INDIRI4
CNSTI4 0
EQI4 $86
line 63
;63:			Com_Printf( "Missing { in info file\n" );
ADDRGP4 $89
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 64
;64:			break;
ADDRGP4 $83
JUMPV
LABELV $86
line 67
;65:		}
;66:
;67:		if ( count == max ) {
ADDRLP4 2052
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $90
line 68
;68:			Com_Printf( "Max infos exceeded\n" );
ADDRGP4 $92
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 69
;69:			break;
ADDRGP4 $83
JUMPV
LABELV $90
line 72
;70:		}
;71:
;72:		info[0] = '\0';
ADDRLP4 1028
CNSTI1 0
ASGNI1
ADDRGP4 $94
JUMPV
LABELV $93
line 73
;73:		while ( 1 ) {
line 74
;74:			token = COM_ParseExt( (const char **)(&buf), qtrue );
ADDRFP4 0
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 2064
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 2064
INDIRP4
ASGNP4
line 75
;75:			if ( !token[0] ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $96
line 76
;76:				Com_Printf( "Unexpected end of info file\n" );
ADDRGP4 $98
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 77
;77:				break;
ADDRGP4 $95
JUMPV
LABELV $96
line 79
;78:			}
;79:			if ( !strcmp( token, "}" ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $101
ARGP4
ADDRLP4 2068
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 2068
INDIRI4
CNSTI4 0
NEI4 $99
line 80
;80:				break;
ADDRGP4 $95
JUMPV
LABELV $99
line 82
;81:			}
;82:			Q_strncpyz( key, token, sizeof( key ) );
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 84
;83:
;84:			token = COM_ParseExt( (const char **)(&buf), qfalse );
ADDRFP4 0
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 2072
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 2072
INDIRP4
ASGNP4
line 85
;85:			if ( !token[0] ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $102
line 86
;86:				strcpy( token, "<NULL>" );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $104
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 87
;87:			}
LABELV $102
line 88
;88:			Info_SetValueForKey( info, key, token );
ADDRLP4 1028
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 89
;89:		}
LABELV $94
line 73
ADDRGP4 $93
JUMPV
LABELV $95
line 91
;90:		//NOTE: extra space for arena number
;91:		infos[count] = G_Alloc(strlen(info) + strlen("\\num\\") + strlen(va("%d", MAX_ARENAS)) + 1);
ADDRLP4 1028
ARGP4
ADDRLP4 2064
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRGP4 $105
ARGP4
ADDRLP4 2068
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRGP4 $106
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 2072
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 2072
INDIRP4
ARGP4
ADDRLP4 2076
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 2064
INDIRI4
ADDRLP4 2068
INDIRI4
ADDI4
ADDRLP4 2076
INDIRI4
ADDI4
CNSTI4 1
ADDI4
ARGI4
ADDRLP4 2080
ADDRGP4 G_Alloc
CALLP4
ASGNP4
ADDRLP4 2052
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
ADDRLP4 2080
INDIRP4
ASGNP4
line 92
;92:		if (infos[count]) {
ADDRLP4 2052
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $107
line 93
;93:			strcpy(infos[count], info);
ADDRLP4 2052
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
INDIRP4
ARGP4
ADDRLP4 1028
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 94
;94:			count++;
ADDRLP4 2052
ADDRLP4 2052
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 95
;95:		}
LABELV $107
line 96
;96:	}
LABELV $82
line 57
ADDRGP4 $81
JUMPV
LABELV $83
line 97
;97:	return count;
ADDRLP4 2052
INDIRI4
RETI4
LABELV $80
endproc G_ParseInfos 2084 12
proc G_LoadArenasFromFile 8216 16
line 105
;98:}
;99:
;100:/*
;101:===============
;102:G_LoadArenasFromFile
;103:===============
;104:*/
;105:static void G_LoadArenasFromFile( char *filename ) {
line 110
;106:	int				len;
;107:	fileHandle_t	f;
;108:	char			buf[MAX_ARENAS_TEXT];
;109:
;110:	len = trap_FS_FOpenFile( filename, &f, FS_READ );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 8200
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8200
INDIRI4
ASGNI4
line 111
;111:	if ( !f ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $110
line 112
;112:		trap_Printf( va( S_COLOR_RED "file not found: %s\n", filename ) );
ADDRGP4 $112
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8204
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8204
INDIRP4
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 113
;113:		return;
ADDRGP4 $109
JUMPV
LABELV $110
line 115
;114:	}
;115:	if ( len >= MAX_ARENAS_TEXT ) {
ADDRLP4 0
INDIRI4
CNSTI4 8192
LTI4 $113
line 116
;116:		trap_Printf( va( S_COLOR_RED "file too large: %s is %i, max allowed is %i", filename, len, MAX_ARENAS_TEXT ) );
ADDRGP4 $115
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 8192
ARGI4
ADDRLP4 8204
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8204
INDIRP4
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 117
;117:		trap_FS_FCloseFile( f );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 118
;118:		return;
ADDRGP4 $109
JUMPV
LABELV $113
line 121
;119:	}
;120:
;121:	trap_FS_Read( buf, len, f );
ADDRLP4 8
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_Read
CALLV
pop
line 122
;122:	buf[len] = 0;
ADDRLP4 0
INDIRI4
ADDRLP4 8
ADDP4
CNSTI1 0
ASGNI1
line 123
;123:	trap_FS_FCloseFile( f );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 125
;124:
;125:	g_numArenas += G_ParseInfos( buf, MAX_ARENAS - g_numArenas, &g_arenaInfos[g_numArenas] );
ADDRLP4 8
ARGP4
ADDRLP4 8204
ADDRGP4 g_numArenas
ASGNP4
ADDRLP4 8208
ADDRLP4 8204
INDIRP4
INDIRI4
ASGNI4
CNSTI4 1024
ADDRLP4 8208
INDIRI4
SUBI4
ARGI4
ADDRLP4 8208
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaInfos
ADDP4
ARGP4
ADDRLP4 8212
ADDRGP4 G_ParseInfos
CALLI4
ASGNI4
ADDRLP4 8204
INDIRP4
ADDRLP4 8208
INDIRI4
ADDRLP4 8212
INDIRI4
ADDI4
ASGNI4
line 126
;126:}
LABELV $109
endproc G_LoadArenasFromFile 8216 16
export G_GetMapTypeBits
proc G_GetMapTypeBits 32 8
line 129
;127:
;128:int G_GetMapTypeBits(char *type)
;129:{
line 130
;130:	int typeBits = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 132
;131:
;132:	if( *type ) {
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $117
line 133
;133:		if( strstr( type, "ffa" ) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $121
ARGP4
ADDRLP4 4
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $119
line 134
;134:			typeBits |= (1 << GT_FFA);
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 135
;135:			typeBits |= (1 << GT_TEAM);
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 136
;136:		}
LABELV $119
line 137
;137:		if( strstr( type, "holocron" ) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $124
ARGP4
ADDRLP4 8
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 8
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $122
line 138
;138:			typeBits |= (1 << GT_HOLOCRON);
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 139
;139:		}
LABELV $122
line 140
;140:		if( strstr( type, "jedimaster" ) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $127
ARGP4
ADDRLP4 12
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $125
line 141
;141:			typeBits |= (1 << GT_JEDIMASTER);
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 142
;142:		}
LABELV $125
line 143
;143:		if( strstr( type, "duel" ) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $130
ARGP4
ADDRLP4 16
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 16
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $128
line 144
;144:			typeBits |= (1 << GT_TOURNAMENT);
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 145
;145:		}
LABELV $128
line 146
;146:		if( strstr( type, "saga" ) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $133
ARGP4
ADDRLP4 20
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 20
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $131
line 147
;147:			typeBits |= (1 << GT_SAGA);
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 148
;148:		}
LABELV $131
line 149
;149:		if( strstr( type, "ctf" ) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $136
ARGP4
ADDRLP4 24
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 24
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $134
line 150
;150:			typeBits |= (1 << GT_CTF);
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 128
BORI4
ASGNI4
line 151
;151:		}
LABELV $134
line 152
;152:		if( strstr( type, "cty" ) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $139
ARGP4
ADDRLP4 28
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 28
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $118
line 153
;153:			typeBits |= (1 << GT_CTY);
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 256
BORI4
ASGNI4
line 154
;154:		}
line 155
;155:	} else {
ADDRGP4 $118
JUMPV
LABELV $117
line 156
;156:		typeBits |= (1 << GT_FFA);
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 157
;157:	}
LABELV $118
line 159
;158:
;159:	return typeBits;
ADDRLP4 0
INDIRI4
RETI4
LABELV $116
endproc G_GetMapTypeBits 32 8
export G_DoesMapSupportGametype
proc G_DoesMapSupportGametype 28 8
line 163
;160:}
;161:
;162:qboolean G_DoesMapSupportGametype(const char *mapname, int gametype)
;163:{
line 164
;164:	int			typeBits = 0;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 165
;165:	int			thisLevel = -1;
ADDRLP4 8
CNSTI4 -1
ASGNI4
line 166
;166:	int			n = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 167
;167:	char		*type = NULL;
ADDRLP4 4
CNSTP4 0
ASGNP4
line 169
;168:
;169:	if (!g_arenaInfos[0])
ADDRGP4 g_arenaInfos
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $141
line 170
;170:	{
line 171
;171:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $140
JUMPV
LABELV $141
line 174
;172:	}
;173:
;174:	if (!mapname || !mapname[0])
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $145
ADDRLP4 16
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $143
LABELV $145
line 175
;175:	{
line 176
;176:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $140
JUMPV
LABELV $143
line 179
;177:	}
;178:
;179:	for( n = 0; n < g_numArenas; n++ )
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $149
JUMPV
LABELV $146
line 180
;180:	{
line 181
;181:		type = Info_ValueForKey( g_arenaInfos[n], "map" );
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $150
ARGP4
ADDRLP4 20
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 20
INDIRP4
ASGNP4
line 183
;182:
;183:		if (Q_stricmp(mapname, type) == 0)
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $151
line 184
;184:		{
line 185
;185:			thisLevel = n;
ADDRLP4 8
ADDRLP4 0
INDIRI4
ASGNI4
line 186
;186:			break;
ADDRGP4 $148
JUMPV
LABELV $151
line 188
;187:		}
;188:	}
LABELV $147
line 179
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $149
ADDRLP4 0
INDIRI4
ADDRGP4 g_numArenas
INDIRI4
LTI4 $146
LABELV $148
line 190
;189:
;190:	if (thisLevel == -1)
ADDRLP4 8
INDIRI4
CNSTI4 -1
NEI4 $153
line 191
;191:	{
line 192
;192:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $140
JUMPV
LABELV $153
line 195
;193:	}
;194:
;195:	type = Info_ValueForKey(g_arenaInfos[thisLevel], "type");
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $155
ARGP4
ADDRLP4 20
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 20
INDIRP4
ASGNP4
line 197
;196:
;197:	typeBits = G_GetMapTypeBits(type);
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 24
ADDRGP4 G_GetMapTypeBits
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 24
INDIRI4
ASGNI4
line 198
;198:	if (typeBits & (1 << gametype))
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDRFP4 4
INDIRI4
LSHI4
BANDI4
CNSTI4 0
EQI4 $156
line 199
;199:	{ //the map in question supports the gametype in question, so..
line 200
;200:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $140
JUMPV
LABELV $156
line 203
;201:	}
;202:
;203:	return qfalse;
CNSTI4 0
RETI4
LABELV $140
endproc G_DoesMapSupportGametype 28 8
export G_RefreshNextMap
proc G_RefreshNextMap 312 16
line 208
;204:}
;205:
;206://rww - auto-obtain nextmap. I could've sworn Q3 had something like this, but I guess not.
;207:const char *G_RefreshNextMap(int gametype, qboolean forced)
;208:{
line 209
;209:	int			typeBits = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 210
;210:	int			thisLevel = 0;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 211
;211:	int			desiredMap = 0;
ADDRLP4 292
CNSTI4 0
ASGNI4
line 212
;212:	int			n = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 213
;213:	char		*type = NULL;
ADDRLP4 4
CNSTP4 0
ASGNP4
line 214
;214:	qboolean	loopingUp = qfalse;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 217
;215:	vmCvar_t	mapname;
;216:
;217:	if (!g_autoMapCycle.integer && !forced)
ADDRLP4 296
CNSTI4 0
ASGNI4
ADDRGP4 g_autoMapCycle+12
INDIRI4
ADDRLP4 296
INDIRI4
NEI4 $159
ADDRFP4 4
INDIRI4
ADDRLP4 296
INDIRI4
NEI4 $159
line 218
;218:	{
line 219
;219:		return NULL;
CNSTP4 0
RETP4
ADDRGP4 $158
JUMPV
LABELV $159
line 222
;220:	}
;221:
;222:	if (!g_arenaInfos[0])
ADDRGP4 g_arenaInfos
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $162
line 223
;223:	{
line 224
;224:		return NULL;
CNSTP4 0
RETP4
ADDRGP4 $158
JUMPV
LABELV $162
line 227
;225:	}
;226:
;227:	trap_Cvar_Register( &mapname, "mapname", "", CVAR_SERVERINFO | CVAR_ROM );
ADDRLP4 20
ARGP4
ADDRGP4 $164
ARGP4
ADDRGP4 $165
ARGP4
CNSTI4 68
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 228
;228:	for( n = 0; n < g_numArenas; n++ )
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $169
JUMPV
LABELV $166
line 229
;229:	{
line 230
;230:		type = Info_ValueForKey( g_arenaInfos[n], "map" );
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $150
ARGP4
ADDRLP4 300
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 300
INDIRP4
ASGNP4
line 232
;231:
;232:		if (Q_stricmp(mapname.string, type) == 0)
ADDRLP4 20+16
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 304
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 304
INDIRI4
CNSTI4 0
NEI4 $170
line 233
;233:		{
line 234
;234:			thisLevel = n;
ADDRLP4 12
ADDRLP4 0
INDIRI4
ASGNI4
line 235
;235:			break;
ADDRGP4 $168
JUMPV
LABELV $170
line 237
;236:		}
;237:	}
LABELV $167
line 228
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $169
ADDRLP4 0
INDIRI4
ADDRGP4 g_numArenas
INDIRI4
LTI4 $166
LABELV $168
line 239
;238:
;239:	desiredMap = thisLevel;
ADDRLP4 292
ADDRLP4 12
INDIRI4
ASGNI4
line 241
;240:
;241:	n = thisLevel+1;
ADDRLP4 0
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRGP4 $174
JUMPV
LABELV $173
line 243
;242:	while (n != thisLevel)
;243:	{ //now cycle through the arena list and find the next map that matches the gametype we're in
line 244
;244:		if (!g_arenaInfos[n] || n >= g_numArenas)
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaInfos
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $178
ADDRLP4 0
INDIRI4
ADDRGP4 g_numArenas
INDIRI4
LTI4 $176
LABELV $178
line 245
;245:		{
line 246
;246:			if (loopingUp)
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $179
line 247
;247:			{ //this shouldn't happen, but if it does we have a null entry break in the arena file
line 249
;248:			  //if this is the case just break out of the loop instead of sticking in an infinite loop
;249:				break;
ADDRGP4 $175
JUMPV
LABELV $179
line 251
;250:			}
;251:			n = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 252
;252:			loopingUp = qtrue;
ADDRLP4 16
CNSTI4 1
ASGNI4
line 253
;253:		}
LABELV $176
line 255
;254:
;255:		type = Info_ValueForKey(g_arenaInfos[n], "type");
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $155
ARGP4
ADDRLP4 304
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 304
INDIRP4
ASGNP4
line 257
;256:		
;257:		typeBits = G_GetMapTypeBits(type);
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 308
ADDRGP4 G_GetMapTypeBits
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 308
INDIRI4
ASGNI4
line 258
;258:		if (typeBits & (1 << gametype))
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDRFP4 0
INDIRI4
LSHI4
BANDI4
CNSTI4 0
EQI4 $181
line 259
;259:		{
line 260
;260:			desiredMap = n;
ADDRLP4 292
ADDRLP4 0
INDIRI4
ASGNI4
line 261
;261:			break;
ADDRGP4 $175
JUMPV
LABELV $181
line 264
;262:		}
;263:
;264:		n++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 265
;265:	}
LABELV $174
line 242
ADDRLP4 0
INDIRI4
ADDRLP4 12
INDIRI4
NEI4 $173
LABELV $175
line 267
;266:
;267:	if (desiredMap == thisLevel)
ADDRLP4 292
INDIRI4
ADDRLP4 12
INDIRI4
NEI4 $183
line 268
;268:	{ //If this is the only level for this game mode or we just can't find a map for this game mode, then nextmap
line 270
;269:	  //will always restart.
;270:		trap_Cvar_Set( "nextmap", "map_restart 0");
ADDRGP4 $185
ARGP4
ADDRGP4 $186
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 271
;271:	}
ADDRGP4 $184
JUMPV
LABELV $183
line 273
;272:	else
;273:	{ //otherwise we have a valid nextmap to cycle to, so use it.
line 274
;274:		type = Info_ValueForKey( g_arenaInfos[desiredMap], "map" );
ADDRLP4 292
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $150
ARGP4
ADDRLP4 300
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 300
INDIRP4
ASGNP4
line 275
;275:		trap_Cvar_Set( "nextmap", va("map %s", type));
ADDRGP4 $187
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 304
ADDRGP4 va
CALLP4
ASGNP4
ADDRGP4 $185
ARGP4
ADDRLP4 304
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 276
;276:	}
LABELV $184
line 278
;277:
;278:	return Info_ValueForKey( g_arenaInfos[desiredMap], "map" );
ADDRLP4 292
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $150
ARGP4
ADDRLP4 300
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 300
INDIRP4
RETP4
LABELV $158
endproc G_RefreshNextMap 312 16
proc G_LoadArenas 1460 16
line 286
;279:}
;280:
;281:/*
;282:===============
;283:G_LoadArenas
;284:===============
;285:*/
;286:static void G_LoadArenas( void ) {
line 295
;287:	int			numdirs;
;288:	vmCvar_t	arenasFile;
;289:	char		filename[128];
;290:	char		dirlist[1024];
;291:	char*		dirptr;
;292:	int			i, n;
;293:	int			dirlen;
;294:
;295:	g_numArenas = 0;
ADDRGP4 g_numArenas
CNSTI4 0
ASGNI4
line 297
;296:
;297:	trap_Cvar_Register( &arenasFile, "g_arenasFile", "", CVAR_INIT|CVAR_ROM );
ADDRLP4 148
ARGP4
ADDRGP4 $189
ARGP4
ADDRGP4 $165
ARGP4
CNSTI4 80
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 298
;298:	if( *arenasFile.string ) {
ADDRLP4 148+16
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $190
line 299
;299:		G_LoadArenasFromFile(arenasFile.string);
ADDRLP4 148+16
ARGP4
ADDRGP4 G_LoadArenasFromFile
CALLV
pop
line 300
;300:	}
ADDRGP4 $191
JUMPV
LABELV $190
line 301
;301:	else {
line 302
;302:		G_LoadArenasFromFile("scripts/arenas.txt");
ADDRGP4 $194
ARGP4
ADDRGP4 G_LoadArenasFromFile
CALLV
pop
line 303
;303:	}
LABELV $191
line 306
;304:
;305:	// get all arenas from .arena files
;306:	numdirs = trap_FS_GetFileList("scripts", ".arena", dirlist, 1024 );
ADDRGP4 $195
ARGP4
ADDRGP4 $196
ARGP4
ADDRLP4 420
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 1444
ADDRGP4 trap_FS_GetFileList
CALLI4
ASGNI4
ADDRLP4 144
ADDRLP4 1444
INDIRI4
ASGNI4
line 307
;307:	dirptr  = dirlist;
ADDRLP4 4
ADDRLP4 420
ASGNP4
line 308
;308:	for (i = 0; i < numdirs; i++, dirptr += dirlen+1) {
ADDRLP4 136
CNSTI4 0
ASGNI4
ADDRGP4 $200
JUMPV
LABELV $197
line 309
;309:		dirlen = strlen(dirptr);
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 1448
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 140
ADDRLP4 1448
INDIRI4
ASGNI4
line 310
;310:		strcpy(filename, "scripts/");
ADDRLP4 8
ARGP4
ADDRGP4 $201
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 311
;311:		strcat(filename, dirptr);
ADDRLP4 8
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 312
;312:		G_LoadArenasFromFile(filename);
ADDRLP4 8
ARGP4
ADDRGP4 G_LoadArenasFromFile
CALLV
pop
line 313
;313:	}
LABELV $198
line 308
ADDRLP4 1448
CNSTI4 1
ASGNI4
ADDRLP4 136
ADDRLP4 136
INDIRI4
ADDRLP4 1448
INDIRI4
ADDI4
ASGNI4
ADDRLP4 4
ADDRLP4 140
INDIRI4
ADDRLP4 1448
INDIRI4
ADDI4
ADDRLP4 4
INDIRP4
ADDP4
ASGNP4
LABELV $200
ADDRLP4 136
INDIRI4
ADDRLP4 144
INDIRI4
LTI4 $197
line 314
;314:	trap_Printf( va( "%i arenas parsed\n", g_numArenas ) );
ADDRGP4 $202
ARGP4
ADDRGP4 g_numArenas
INDIRI4
ARGI4
ADDRLP4 1452
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1452
INDIRP4
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 316
;315:	
;316:	for( n = 0; n < g_numArenas; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $206
JUMPV
LABELV $203
line 317
;317:		Info_SetValueForKey( g_arenaInfos[n], "num", va( "%i", n ) );
ADDRGP4 $208
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 1456
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $207
ARGP4
ADDRLP4 1456
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 318
;318:	}
LABELV $204
line 316
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $206
ADDRLP4 0
INDIRI4
ADDRGP4 g_numArenas
INDIRI4
LTI4 $203
line 320
;319:
;320:	G_RefreshNextMap(g_gametype.integer, qfalse);
ADDRGP4 g_gametype+12
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 G_RefreshNextMap
CALLP4
pop
line 321
;321:}
LABELV $188
endproc G_LoadArenas 1460 16
export G_GetArenaInfoByMap
proc G_GetArenaInfoByMap 12 8
line 329
;322:
;323:
;324:/*
;325:===============
;326:G_GetArenaInfoByNumber
;327:===============
;328:*/
;329:const char *G_GetArenaInfoByMap( const char *map ) {
line 332
;330:	int			n;
;331:
;332:	for( n = 0; n < g_numArenas; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $214
JUMPV
LABELV $211
line 333
;333:		if( Q_stricmp( Info_ValueForKey( g_arenaInfos[n], "map" ), map ) == 0 ) {
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $150
ARGP4
ADDRLP4 4
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $215
line 334
;334:			return g_arenaInfos[n];
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_arenaInfos
ADDP4
INDIRP4
RETP4
ADDRGP4 $210
JUMPV
LABELV $215
line 336
;335:		}
;336:	}
LABELV $212
line 332
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $214
ADDRLP4 0
INDIRI4
ADDRGP4 g_numArenas
INDIRI4
LTI4 $211
line 338
;337:
;338:	return NULL;
CNSTP4 0
RETP4
LABELV $210
endproc G_GetArenaInfoByMap 12 8
proc PlayerIntroSound 80 12
line 347
;339:}
;340:
;341:
;342:/*
;343:=================
;344:PlayerIntroSound
;345:=================
;346:*/
;347:static void PlayerIntroSound( const char *modelAndSkin ) {
line 351
;348:	char	model[MAX_QPATH];
;349:	char	*skin;
;350:
;351:	Q_strncpyz( model, modelAndSkin, sizeof(model) );
ADDRLP4 4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 352
;352:	skin = Q_strrchr( model, '/' );
ADDRLP4 4
ARGP4
CNSTI4 47
ARGI4
ADDRLP4 68
ADDRGP4 Q_strrchr
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 68
INDIRP4
ASGNP4
line 353
;353:	if ( skin ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $218
line 354
;354:		*skin++ = '\0';
ADDRLP4 72
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 72
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 72
INDIRP4
CNSTI1 0
ASGNI1
line 355
;355:	}
ADDRGP4 $219
JUMPV
LABELV $218
line 356
;356:	else {
line 357
;357:		skin = model;
ADDRLP4 0
ADDRLP4 4
ASGNP4
line 358
;358:	}
LABELV $219
line 360
;359:
;360:	if( Q_stricmp( skin, "default" ) == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $222
ARGP4
ADDRLP4 72
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 0
NEI4 $220
line 361
;361:		skin = model;
ADDRLP4 0
ADDRLP4 4
ASGNP4
line 362
;362:	}
LABELV $220
line 364
;363:
;364:	trap_SendConsoleCommand( EXEC_APPEND, va( "play sound/player/announce/%s.wav\n", skin ) );
ADDRGP4 $223
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 76
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 2
ARGI4
ADDRLP4 76
INDIRP4
ARGP4
ADDRGP4 trap_SendConsoleCommand
CALLV
pop
line 365
;365:}
LABELV $217
endproc PlayerIntroSound 80 12
export G_AddRandomBot
proc G_AddRandomBot 80 20
line 372
;366:
;367:/*
;368:===============
;369:G_AddRandomBot
;370:===============
;371:*/
;372:void G_AddRandomBot( int team ) {
line 378
;373:	int		i, n, num;
;374:	float	skill;
;375:	char	*value, netname[36], *teamstr;
;376:	gclient_t	*cl;
;377:
;378:	num = 0;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 379
;379:	for ( n = 0; n < g_numBots ; n++ ) {
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRGP4 $228
JUMPV
LABELV $225
line 380
;380:		value = Info_ValueForKey( g_botInfos[n], "name" );
ADDRLP4 12
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $229
ARGP4
ADDRLP4 64
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 64
INDIRP4
ASGNP4
line 382
;381:		//
;382:		for ( i=0 ; i< g_maxclients.integer ; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $233
JUMPV
LABELV $230
line 383
;383:			cl = level.clients + i;
ADDRLP4 0
CNSTI4 1756
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 384
;384:			if ( cl->pers.connected != CON_CONNECTED ) {
ADDRLP4 0
INDIRP4
CNSTI4 1368
ADDP4
INDIRI4
CNSTI4 2
EQI4 $235
line 385
;385:				continue;
ADDRGP4 $231
JUMPV
LABELV $235
line 387
;386:			}
;387:			if ( !(g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT) ) {
CNSTI4 828
ADDRLP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+296+8
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
NEI4 $237
line 388
;388:				continue;
ADDRGP4 $231
JUMPV
LABELV $237
line 390
;389:			}
;390:			if ( team >= 0 && cl->sess.sessionTeam != team ) {
ADDRLP4 68
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 68
INDIRI4
CNSTI4 0
LTI4 $241
ADDRLP4 0
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ADDRLP4 68
INDIRI4
EQI4 $241
line 391
;391:				continue;
ADDRGP4 $231
JUMPV
LABELV $241
line 393
;392:			}
;393:			if ( !Q_stricmp( value, cl->pers.netname ) ) {
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 1416
ADDP4
ARGP4
ADDRLP4 72
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 0
NEI4 $243
line 394
;394:				break;
ADDRGP4 $232
JUMPV
LABELV $243
line 396
;395:			}
;396:		}
LABELV $231
line 382
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $233
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $230
LABELV $232
line 397
;397:		if (i >= g_maxclients.integer) {
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $245
line 398
;398:			num++;
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 399
;399:		}
LABELV $245
line 400
;400:	}
LABELV $226
line 379
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $228
ADDRLP4 12
INDIRI4
ADDRGP4 g_numBots
INDIRI4
LTI4 $225
line 401
;401:	num = random() * num;
ADDRLP4 64
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 16
ADDRLP4 64
INDIRI4
CNSTI4 32767
BANDI4
CVIF4 4
CNSTF4 1191181824
DIVF4
ADDRLP4 16
INDIRI4
CVIF4 4
MULF4
CVFI4 4
ASGNI4
line 402
;402:	for ( n = 0; n < g_numBots ; n++ ) {
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRGP4 $251
JUMPV
LABELV $248
line 403
;403:		value = Info_ValueForKey( g_botInfos[n], "name" );
ADDRLP4 12
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $229
ARGP4
ADDRLP4 68
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 8
ADDRLP4 68
INDIRP4
ASGNP4
line 405
;404:		//
;405:		for ( i=0 ; i< g_maxclients.integer ; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $255
JUMPV
LABELV $252
line 406
;406:			cl = level.clients + i;
ADDRLP4 0
CNSTI4 1756
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 407
;407:			if ( cl->pers.connected != CON_CONNECTED ) {
ADDRLP4 0
INDIRP4
CNSTI4 1368
ADDP4
INDIRI4
CNSTI4 2
EQI4 $257
line 408
;408:				continue;
ADDRGP4 $253
JUMPV
LABELV $257
line 410
;409:			}
;410:			if ( !(g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT) ) {
CNSTI4 828
ADDRLP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+296+8
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
NEI4 $259
line 411
;411:				continue;
ADDRGP4 $253
JUMPV
LABELV $259
line 413
;412:			}
;413:			if ( team >= 0 && cl->sess.sessionTeam != team ) {
ADDRLP4 72
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 72
INDIRI4
CNSTI4 0
LTI4 $263
ADDRLP4 0
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ADDRLP4 72
INDIRI4
EQI4 $263
line 414
;414:				continue;
ADDRGP4 $253
JUMPV
LABELV $263
line 416
;415:			}
;416:			if ( !Q_stricmp( value, cl->pers.netname ) ) {
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 1416
ADDP4
ARGP4
ADDRLP4 76
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 76
INDIRI4
CNSTI4 0
NEI4 $265
line 417
;417:				break;
ADDRGP4 $254
JUMPV
LABELV $265
line 419
;418:			}
;419:		}
LABELV $253
line 405
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $255
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $252
LABELV $254
line 420
;420:		if (i >= g_maxclients.integer) {
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $267
line 421
;421:			num--;
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 422
;422:			if (num <= 0) {
ADDRLP4 16
INDIRI4
CNSTI4 0
GTI4 $270
line 423
;423:				skill = trap_Cvar_VariableValue( "g_spSkill" );
ADDRGP4 $272
ARGP4
ADDRLP4 72
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 56
ADDRLP4 72
INDIRF4
ASGNF4
line 424
;424:				if (team == TEAM_RED) teamstr = "red";
ADDRFP4 0
INDIRI4
CNSTI4 1
NEI4 $273
ADDRLP4 60
ADDRGP4 $275
ASGNP4
ADDRGP4 $274
JUMPV
LABELV $273
line 425
;425:				else if (team == TEAM_BLUE) teamstr = "blue";
ADDRFP4 0
INDIRI4
CNSTI4 2
NEI4 $276
ADDRLP4 60
ADDRGP4 $278
ASGNP4
ADDRGP4 $277
JUMPV
LABELV $276
line 426
;426:				else teamstr = "";
ADDRLP4 60
ADDRGP4 $165
ASGNP4
LABELV $277
LABELV $274
line 427
;427:				strncpy(netname, value, sizeof(netname)-1);
ADDRLP4 20
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
CNSTI4 35
ARGI4
ADDRGP4 strncpy
CALLP4
pop
line 428
;428:				netname[sizeof(netname)-1] = '\0';
ADDRLP4 20+35
CNSTI1 0
ASGNI1
line 429
;429:				Q_CleanStr(netname);
ADDRLP4 20
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 430
;430:				trap_SendConsoleCommand( EXEC_INSERT, va("addbot %s %f %s %i\n", netname, skill, teamstr, 0) );
ADDRGP4 $280
ARGP4
ADDRLP4 20
ARGP4
ADDRLP4 56
INDIRF4
ARGF4
ADDRLP4 60
INDIRP4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 76
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 1
ARGI4
ADDRLP4 76
INDIRP4
ARGP4
ADDRGP4 trap_SendConsoleCommand
CALLV
pop
line 431
;431:				return;
ADDRGP4 $224
JUMPV
LABELV $270
line 433
;432:			}
;433:		}
LABELV $267
line 434
;434:	}
LABELV $249
line 402
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $251
ADDRLP4 12
INDIRI4
ADDRGP4 g_numBots
INDIRI4
LTI4 $248
line 435
;435:}
LABELV $224
endproc G_AddRandomBot 80 20
export G_RemoveRandomBot
proc G_RemoveRandomBot 52 8
line 442
;436:
;437:/*
;438:===============
;439:G_RemoveRandomBot
;440:===============
;441:*/
;442:int G_RemoveRandomBot( int team ) {
line 447
;443:	int i;
;444:	char netname[36];
;445:	gclient_t	*cl;
;446:
;447:	for ( i=0 ; i< g_maxclients.integer ; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $285
JUMPV
LABELV $282
line 448
;448:		cl = level.clients + i;
ADDRLP4 0
CNSTI4 1756
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 449
;449:		if ( cl->pers.connected != CON_CONNECTED ) {
ADDRLP4 0
INDIRP4
CNSTI4 1368
ADDP4
INDIRI4
CNSTI4 2
EQI4 $287
line 450
;450:			continue;
ADDRGP4 $283
JUMPV
LABELV $287
line 452
;451:		}
;452:		if ( !(g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT) ) {
CNSTI4 828
ADDRLP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+296+8
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
NEI4 $289
line 453
;453:			continue;
ADDRGP4 $283
JUMPV
LABELV $289
line 455
;454:		}
;455:		if ( team >= 0 && cl->sess.sessionTeam != team ) {
ADDRLP4 44
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 44
INDIRI4
CNSTI4 0
LTI4 $293
ADDRLP4 0
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ADDRLP4 44
INDIRI4
EQI4 $293
line 456
;456:			continue;
ADDRGP4 $283
JUMPV
LABELV $293
line 458
;457:		}
;458:		strcpy(netname, cl->pers.netname);
ADDRLP4 8
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 1416
ADDP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 459
;459:		Q_CleanStr(netname);
ADDRLP4 8
ARGP4
ADDRGP4 Q_CleanStr
CALLP4
pop
line 460
;460:		trap_SendConsoleCommand( EXEC_INSERT, va("kick %s\n", netname) );
ADDRGP4 $295
ARGP4
ADDRLP4 8
ARGP4
ADDRLP4 48
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 1
ARGI4
ADDRLP4 48
INDIRP4
ARGP4
ADDRGP4 trap_SendConsoleCommand
CALLV
pop
line 461
;461:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $281
JUMPV
LABELV $283
line 447
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $285
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $282
line 463
;462:	}
;463:	return qfalse;
CNSTI4 0
RETI4
LABELV $281
endproc G_RemoveRandomBot 52 8
export G_CountHumanPlayers
proc G_CountHumanPlayers 16 0
line 471
;464:}
;465:
;466:/*
;467:===============
;468:G_CountHumanPlayers
;469:===============
;470:*/
;471:int G_CountHumanPlayers( int team ) {
line 475
;472:	int i, num;
;473:	gclient_t	*cl;
;474:
;475:	num = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 476
;476:	for ( i=0 ; i< g_maxclients.integer ; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $300
JUMPV
LABELV $297
line 477
;477:		cl = level.clients + i;
ADDRLP4 0
CNSTI4 1756
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 478
;478:		if ( cl->pers.connected != CON_CONNECTED ) {
ADDRLP4 0
INDIRP4
CNSTI4 1368
ADDP4
INDIRI4
CNSTI4 2
EQI4 $302
line 479
;479:			continue;
ADDRGP4 $298
JUMPV
LABELV $302
line 481
;480:		}
;481:		if ( g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT ) {
CNSTI4 828
ADDRLP4 0
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+296+8
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
EQI4 $304
line 482
;482:			continue;
ADDRGP4 $298
JUMPV
LABELV $304
line 484
;483:		}
;484:		if ( team >= 0 && cl->sess.sessionTeam != team ) {
ADDRLP4 12
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
LTI4 $308
ADDRLP4 0
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ADDRLP4 12
INDIRI4
EQI4 $308
line 485
;485:			continue;
ADDRGP4 $298
JUMPV
LABELV $308
line 487
;486:		}
;487:		num++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 488
;488:	}
LABELV $298
line 476
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $300
ADDRLP4 4
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $297
line 489
;489:	return num;
ADDRLP4 8
INDIRI4
RETI4
LABELV $296
endproc G_CountHumanPlayers 16 0
export G_CountBotPlayers
proc G_CountBotPlayers 20 0
line 497
;490:}
;491:
;492:/*
;493:===============
;494:G_CountBotPlayers
;495:===============
;496:*/
;497:int G_CountBotPlayers( int team ) {
line 501
;498:	int i, n, num;
;499:	gclient_t	*cl;
;500:
;501:	num = 0;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 502
;502:	for ( i=0 ; i< g_maxclients.integer ; i++ ) {
ADDRLP4 8
CNSTI4 0
ASGNI4
ADDRGP4 $314
JUMPV
LABELV $311
line 503
;503:		cl = level.clients + i;
ADDRLP4 4
CNSTI4 1756
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 504
;504:		if ( cl->pers.connected != CON_CONNECTED ) {
ADDRLP4 4
INDIRP4
CNSTI4 1368
ADDP4
INDIRI4
CNSTI4 2
EQI4 $316
line 505
;505:			continue;
ADDRGP4 $312
JUMPV
LABELV $316
line 507
;506:		}
;507:		if ( !(g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT) ) {
CNSTI4 828
ADDRLP4 4
INDIRP4
CNSTI4 144
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities+296+8
ADDP4
INDIRI4
CNSTI4 8
BANDI4
CNSTI4 0
NEI4 $318
line 508
;508:			continue;
ADDRGP4 $312
JUMPV
LABELV $318
line 510
;509:		}
;510:		if ( team >= 0 && cl->sess.sessionTeam != team ) {
ADDRLP4 16
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
LTI4 $322
ADDRLP4 4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $322
line 511
;511:			continue;
ADDRGP4 $312
JUMPV
LABELV $322
line 513
;512:		}
;513:		num++;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 514
;514:	}
LABELV $312
line 502
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $314
ADDRLP4 8
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $311
line 515
;515:	for( n = 0; n < BOT_SPAWN_QUEUE_DEPTH; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $324
line 516
;516:		if( !botSpawnQueue[n].spawnTime ) {
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue+4
ADDP4
INDIRI4
CNSTI4 0
NEI4 $328
line 517
;517:			continue;
ADDRGP4 $325
JUMPV
LABELV $328
line 519
;518:		}
;519:		if ( botSpawnQueue[n].spawnTime > level.time ) {
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue+4
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
LEI4 $331
line 520
;520:			continue;
ADDRGP4 $325
JUMPV
LABELV $331
line 522
;521:		}
;522:		num++;
ADDRLP4 12
ADDRLP4 12
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 523
;523:	}
LABELV $325
line 515
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $324
line 524
;524:	return num;
ADDRLP4 12
INDIRI4
RETI4
LABELV $310
endproc G_CountBotPlayers 20 0
bss
align 4
LABELV $336
skip 4
export G_CheckMinimumPlayers
code
proc G_CheckMinimumPlayers 36 4
line 532
;525:}
;526:
;527:/*
;528:===============
;529:G_CheckMinimumPlayers
;530:===============
;531:*/
;532:void G_CheckMinimumPlayers( void ) {
line 537
;533:	int minplayers;
;534:	int humanplayers, botplayers;
;535:	static int checkminimumplayers_time;
;536:
;537:	if (level.intermissiontime) return;
ADDRGP4 level+9008
INDIRI4
CNSTI4 0
EQI4 $337
ADDRGP4 $335
JUMPV
LABELV $337
line 539
;538:	//only check once each 10 seconds
;539:	if (checkminimumplayers_time > level.time - 10000) {
ADDRGP4 $336
INDIRI4
ADDRGP4 level+32
INDIRI4
CNSTI4 10000
SUBI4
LEI4 $340
line 540
;540:		return;
ADDRGP4 $335
JUMPV
LABELV $340
line 542
;541:	}
;542:	checkminimumplayers_time = level.time;
ADDRGP4 $336
ADDRGP4 level+32
INDIRI4
ASGNI4
line 543
;543:	trap_Cvar_Update(&bot_minplayers);
ADDRGP4 bot_minplayers
ARGP4
ADDRGP4 trap_Cvar_Update
CALLV
pop
line 544
;544:	minplayers = bot_minplayers.integer;
ADDRLP4 0
ADDRGP4 bot_minplayers+12
INDIRI4
ASGNI4
line 545
;545:	if (minplayers <= 0) return;
ADDRLP4 0
INDIRI4
CNSTI4 0
GTI4 $345
ADDRGP4 $335
JUMPV
LABELV $345
line 547
;546:
;547:	if (g_gametype.integer >= GT_TEAM) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
LTI4 $347
line 548
;548:		if (minplayers >= g_maxclients.integer / 2) {
ADDRLP4 0
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
CNSTI4 2
DIVI4
LTI4 $350
line 549
;549:			minplayers = (g_maxclients.integer / 2) -1;
ADDRLP4 0
ADDRGP4 g_maxclients+12
INDIRI4
CNSTI4 2
DIVI4
CNSTI4 1
SUBI4
ASGNI4
line 550
;550:		}
LABELV $350
line 552
;551:
;552:		humanplayers = G_CountHumanPlayers( TEAM_RED );
CNSTI4 1
ARGI4
ADDRLP4 12
ADDRGP4 G_CountHumanPlayers
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 12
INDIRI4
ASGNI4
line 553
;553:		botplayers = G_CountBotPlayers(	TEAM_RED );
CNSTI4 1
ARGI4
ADDRLP4 16
ADDRGP4 G_CountBotPlayers
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRI4
ASGNI4
line 555
;554:		//
;555:		if (humanplayers + botplayers < minplayers) {
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
GEI4 $354
line 556
;556:			G_AddRandomBot( TEAM_RED );
CNSTI4 1
ARGI4
ADDRGP4 G_AddRandomBot
CALLV
pop
line 557
;557:		} else if (humanplayers + botplayers > minplayers && botplayers) {
ADDRGP4 $355
JUMPV
LABELV $354
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
LEI4 $356
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $356
line 558
;558:			G_RemoveRandomBot( TEAM_RED );
CNSTI4 1
ARGI4
ADDRGP4 G_RemoveRandomBot
CALLI4
pop
line 559
;559:		}
LABELV $356
LABELV $355
line 561
;560:		//
;561:		humanplayers = G_CountHumanPlayers( TEAM_BLUE );
CNSTI4 2
ARGI4
ADDRLP4 24
ADDRGP4 G_CountHumanPlayers
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 24
INDIRI4
ASGNI4
line 562
;562:		botplayers = G_CountBotPlayers( TEAM_BLUE );
CNSTI4 2
ARGI4
ADDRLP4 28
ADDRGP4 G_CountBotPlayers
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 28
INDIRI4
ASGNI4
line 564
;563:		//
;564:		if (humanplayers + botplayers < minplayers) {
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
GEI4 $358
line 565
;565:			G_AddRandomBot( TEAM_BLUE );
CNSTI4 2
ARGI4
ADDRGP4 G_AddRandomBot
CALLV
pop
line 566
;566:		} else if (humanplayers + botplayers > minplayers && botplayers) {
ADDRGP4 $348
JUMPV
LABELV $358
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
LEI4 $348
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $348
line 567
;567:			G_RemoveRandomBot( TEAM_BLUE );
CNSTI4 2
ARGI4
ADDRGP4 G_RemoveRandomBot
CALLI4
pop
line 568
;568:		}
line 569
;569:	}
ADDRGP4 $348
JUMPV
LABELV $347
line 570
;570:	else if (g_gametype.integer == GT_TOURNAMENT ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 3
NEI4 $362
line 571
;571:		if (minplayers >= g_maxclients.integer) {
ADDRLP4 0
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $365
line 572
;572:			minplayers = g_maxclients.integer-1;
ADDRLP4 0
ADDRGP4 g_maxclients+12
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 573
;573:		}
LABELV $365
line 574
;574:		humanplayers = G_CountHumanPlayers( -1 );
CNSTI4 -1
ARGI4
ADDRLP4 12
ADDRGP4 G_CountHumanPlayers
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 12
INDIRI4
ASGNI4
line 575
;575:		botplayers = G_CountBotPlayers( -1 );
CNSTI4 -1
ARGI4
ADDRLP4 16
ADDRGP4 G_CountBotPlayers
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRI4
ASGNI4
line 577
;576:		//
;577:		if (humanplayers + botplayers < minplayers) {
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
GEI4 $369
line 578
;578:			G_AddRandomBot( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRGP4 G_AddRandomBot
CALLV
pop
line 579
;579:		} else if (humanplayers + botplayers > minplayers && botplayers) {
ADDRGP4 $363
JUMPV
LABELV $369
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
LEI4 $363
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $363
line 581
;580:			// try to remove spectators first
;581:			if (!G_RemoveRandomBot( TEAM_SPECTATOR )) {
CNSTI4 3
ARGI4
ADDRLP4 24
ADDRGP4 G_RemoveRandomBot
CALLI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $363
line 583
;582:				// just remove the bot that is playing
;583:				G_RemoveRandomBot( -1 );
CNSTI4 -1
ARGI4
ADDRGP4 G_RemoveRandomBot
CALLI4
pop
line 584
;584:			}
line 585
;585:		}
line 586
;586:	}
ADDRGP4 $363
JUMPV
LABELV $362
line 587
;587:	else if (g_gametype.integer == GT_FFA) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 0
NEI4 $375
line 588
;588:		if (minplayers >= g_maxclients.integer) {
ADDRLP4 0
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $378
line 589
;589:			minplayers = g_maxclients.integer-1;
ADDRLP4 0
ADDRGP4 g_maxclients+12
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 590
;590:		}
LABELV $378
line 591
;591:		humanplayers = G_CountHumanPlayers( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRLP4 12
ADDRGP4 G_CountHumanPlayers
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 12
INDIRI4
ASGNI4
line 592
;592:		botplayers = G_CountBotPlayers( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRLP4 16
ADDRGP4 G_CountBotPlayers
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRI4
ASGNI4
line 594
;593:		//
;594:		if (humanplayers + botplayers < minplayers) {
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
GEI4 $382
line 595
;595:			G_AddRandomBot( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRGP4 G_AddRandomBot
CALLV
pop
line 596
;596:		} else if (humanplayers + botplayers > minplayers && botplayers) {
ADDRGP4 $376
JUMPV
LABELV $382
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
LEI4 $376
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $376
line 597
;597:			G_RemoveRandomBot( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRGP4 G_RemoveRandomBot
CALLI4
pop
line 598
;598:		}
line 599
;599:	}
ADDRGP4 $376
JUMPV
LABELV $375
line 600
;600:	else if (g_gametype.integer == GT_HOLOCRON || g_gametype.integer == GT_JEDIMASTER) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 1
EQI4 $390
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 2
NEI4 $386
LABELV $390
line 601
;601:		if (minplayers >= g_maxclients.integer) {
ADDRLP4 0
INDIRI4
ADDRGP4 g_maxclients+12
INDIRI4
LTI4 $391
line 602
;602:			minplayers = g_maxclients.integer-1;
ADDRLP4 0
ADDRGP4 g_maxclients+12
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 603
;603:		}
LABELV $391
line 604
;604:		humanplayers = G_CountHumanPlayers( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRLP4 12
ADDRGP4 G_CountHumanPlayers
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 12
INDIRI4
ASGNI4
line 605
;605:		botplayers = G_CountBotPlayers( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRLP4 16
ADDRGP4 G_CountBotPlayers
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 16
INDIRI4
ASGNI4
line 607
;606:		//
;607:		if (humanplayers + botplayers < minplayers) {
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
GEI4 $395
line 608
;608:			G_AddRandomBot( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRGP4 G_AddRandomBot
CALLV
pop
line 609
;609:		} else if (humanplayers + botplayers > minplayers && botplayers) {
ADDRGP4 $396
JUMPV
LABELV $395
ADDRLP4 8
INDIRI4
ADDRLP4 4
INDIRI4
ADDI4
ADDRLP4 0
INDIRI4
LEI4 $397
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $397
line 610
;610:			G_RemoveRandomBot( TEAM_FREE );
CNSTI4 0
ARGI4
ADDRGP4 G_RemoveRandomBot
CALLI4
pop
line 611
;611:		}
LABELV $397
LABELV $396
line 612
;612:	}
LABELV $386
LABELV $376
LABELV $363
LABELV $348
line 613
;613:}
LABELV $335
endproc G_CheckMinimumPlayers 36 4
export G_CheckBotSpawn
proc G_CheckBotSpawn 1032 12
line 620
;614:
;615:/*
;616:===============
;617:G_CheckBotSpawn
;618:===============
;619:*/
;620:void G_CheckBotSpawn( void ) {
line 624
;621:	int		n;
;622:	char	userinfo[MAX_INFO_VALUE];
;623:
;624:	G_CheckMinimumPlayers();
ADDRGP4 G_CheckMinimumPlayers
CALLV
pop
line 626
;625:
;626:	for( n = 0; n < BOT_SPAWN_QUEUE_DEPTH; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $400
line 627
;627:		if( !botSpawnQueue[n].spawnTime ) {
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue+4
ADDP4
INDIRI4
CNSTI4 0
NEI4 $404
line 628
;628:			continue;
ADDRGP4 $401
JUMPV
LABELV $404
line 630
;629:		}
;630:		if ( botSpawnQueue[n].spawnTime > level.time ) {
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue+4
ADDP4
INDIRI4
ADDRGP4 level+32
INDIRI4
LEI4 $407
line 631
;631:			continue;
ADDRGP4 $401
JUMPV
LABELV $407
line 633
;632:		}
;633:		ClientBegin( botSpawnQueue[n].clientNum, qfalse );
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue
ADDP4
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 ClientBegin
CALLV
pop
line 634
;634:		botSpawnQueue[n].spawnTime = 0;
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue+4
ADDP4
CNSTI4 0
ASGNI4
line 636
;635:
;636:		if( g_gametype.integer == GT_SINGLE_PLAYER ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 4
NEI4 $412
line 637
;637:			trap_GetUserinfo( botSpawnQueue[n].clientNum, userinfo, sizeof(userinfo) );
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue
ADDP4
INDIRI4
ARGI4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 638
;638:			PlayerIntroSound( Info_ValueForKey (userinfo, "model") );
ADDRLP4 4
ARGP4
ADDRGP4 $415
ARGP4
ADDRLP4 1028
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 PlayerIntroSound
CALLV
pop
line 639
;639:		}
LABELV $412
line 640
;640:	}
LABELV $401
line 626
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $400
line 641
;641:}
LABELV $399
endproc G_CheckBotSpawn 1032 12
proc AddBotToSpawnQueue 4 8
line 649
;642:
;643:
;644:/*
;645:===============
;646:AddBotToSpawnQueue
;647:===============
;648:*/
;649:static void AddBotToSpawnQueue( int clientNum, int delay ) {
line 652
;650:	int		n;
;651:
;652:	for( n = 0; n < BOT_SPAWN_QUEUE_DEPTH; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $417
line 653
;653:		if( !botSpawnQueue[n].spawnTime ) {
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue+4
ADDP4
INDIRI4
CNSTI4 0
NEI4 $421
line 654
;654:			botSpawnQueue[n].spawnTime = level.time + delay;
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue+4
ADDP4
ADDRGP4 level+32
INDIRI4
ADDRFP4 4
INDIRI4
ADDI4
ASGNI4
line 655
;655:			botSpawnQueue[n].clientNum = clientNum;
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue
ADDP4
ADDRFP4 0
INDIRI4
ASGNI4
line 656
;656:			return;
ADDRGP4 $416
JUMPV
LABELV $421
line 658
;657:		}
;658:	}
LABELV $418
line 652
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $417
line 660
;659:
;660:	G_Printf( S_COLOR_YELLOW "Unable to delay spawn\n" );
ADDRGP4 $426
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 661
;661:	ClientBegin( clientNum, qfalse );
ADDRFP4 0
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 ClientBegin
CALLV
pop
line 662
;662:}
LABELV $416
endproc AddBotToSpawnQueue 4 8
export G_RemoveQueuedBotBegin
proc G_RemoveQueuedBotBegin 4 0
line 673
;663:
;664:
;665:/*
;666:===============
;667:G_RemoveQueuedBotBegin
;668:
;669:Called on client disconnect to make sure the delayed spawn
;670:doesn't happen on a freed index
;671:===============
;672:*/
;673:void G_RemoveQueuedBotBegin( int clientNum ) {
line 676
;674:	int		n;
;675:
;676:	for( n = 0; n < BOT_SPAWN_QUEUE_DEPTH; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $428
line 677
;677:		if( botSpawnQueue[n].clientNum == clientNum ) {
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $432
line 678
;678:			botSpawnQueue[n].spawnTime = 0;
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 botSpawnQueue+4
ADDP4
CNSTI4 0
ASGNI4
line 679
;679:			return;
ADDRGP4 $427
JUMPV
LABELV $432
line 681
;680:		}
;681:	}
LABELV $429
line 676
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 16
LTI4 $428
line 682
;682:}
LABELV $427
endproc G_RemoveQueuedBotBegin 4 0
export G_BotConnect
proc G_BotConnect 1336 12
line 690
;683:
;684:
;685:/*
;686:===============
;687:G_BotConnect
;688:===============
;689:*/
;690:qboolean G_BotConnect( int clientNum, qboolean restart ) {
line 694
;691:	bot_settings_t	settings;
;692:	char			userinfo[MAX_INFO_STRING];
;693:
;694:	trap_GetUserinfo( clientNum, userinfo, sizeof(userinfo) );
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 292
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 696
;695:
;696:	Q_strncpyz( settings.personalityfile, Info_ValueForKey( userinfo, "personality" ), sizeof(settings.personalityfile) );
ADDRLP4 292
ARGP4
ADDRGP4 $436
ARGP4
ADDRLP4 1316
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
ADDRLP4 1316
INDIRP4
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 697
;697:	settings.skill = atof( Info_ValueForKey( userinfo, "skill" ) );
ADDRLP4 292
ARGP4
ADDRGP4 $438
ARGP4
ADDRLP4 1320
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1320
INDIRP4
ARGP4
ADDRLP4 1324
ADDRGP4 atof
CALLF4
ASGNF4
ADDRLP4 0+144
ADDRLP4 1324
INDIRF4
ASGNF4
line 698
;698:	Q_strncpyz( settings.team, Info_ValueForKey( userinfo, "team" ), sizeof(settings.team) );
ADDRLP4 292
ARGP4
ADDRGP4 $440
ARGP4
ADDRLP4 1328
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0+148
ARGP4
ADDRLP4 1328
INDIRP4
ARGP4
CNSTI4 144
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 700
;699:
;700:	if (!BotAISetupClient( clientNum, &settings, restart )) {
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 1332
ADDRGP4 BotAISetupClient
CALLI4
ASGNI4
ADDRLP4 1332
INDIRI4
CNSTI4 0
NEI4 $442
line 701
;701:		trap_DropClient( clientNum, "BotAISetupClient failed" );
ADDRFP4 0
INDIRI4
ARGI4
ADDRGP4 $444
ARGP4
ADDRGP4 trap_DropClient
CALLV
pop
line 702
;702:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $435
JUMPV
LABELV $442
line 705
;703:	}
;704:
;705:	return qtrue;
CNSTI4 1
RETI4
LABELV $435
endproc G_BotConnect 1336 12
proc G_AddBot 1140 12
line 714
;706:}
;707:
;708:
;709:/*
;710:===============
;711:G_AddBot
;712:===============
;713:*/
;714:static void G_AddBot( const char *name, float skill, const char *team, int delay, char *altname) {
line 724
;715:	int				clientNum;
;716:	char			*botinfo;
;717:	gentity_t		*bot;
;718:	char			*key;
;719:	char			*s;
;720:	char			*botname;
;721:	char			*model;
;722://	char			*headmodel;
;723:	char			userinfo[MAX_INFO_STRING];
;724:	int				preTeam = 0;
ADDRLP4 1052
CNSTI4 0
ASGNI4
line 727
;725:
;726:	// get the botinfo from bots.txt
;727:	botinfo = G_GetBotInfoByName( name );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1056
ADDRGP4 G_GetBotInfoByName
CALLP4
ASGNP4
ADDRLP4 1032
ADDRLP4 1056
INDIRP4
ASGNP4
line 728
;728:	if ( !botinfo ) {
ADDRLP4 1032
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $446
line 729
;729:		G_Printf( S_COLOR_RED "Error: Bot '%s' not defined\n", name );
ADDRGP4 $448
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 730
;730:		return;
ADDRGP4 $445
JUMPV
LABELV $446
line 734
;731:	}
;732:
;733:	// create the bot's userinfo
;734:	userinfo[0] = '\0';
ADDRLP4 0
CNSTI1 0
ASGNI1
line 736
;735:
;736:	botname = Info_ValueForKey( botinfo, "funname" );
ADDRLP4 1032
INDIRP4
ARGP4
ADDRGP4 $449
ARGP4
ADDRLP4 1060
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1048
ADDRLP4 1060
INDIRP4
ASGNP4
line 737
;737:	if( !botname[0] ) {
ADDRLP4 1048
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $450
line 738
;738:		botname = Info_ValueForKey( botinfo, "name" );
ADDRLP4 1032
INDIRP4
ARGP4
ADDRGP4 $229
ARGP4
ADDRLP4 1064
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1048
ADDRLP4 1064
INDIRP4
ASGNP4
line 739
;739:	}
LABELV $450
line 741
;740:	// check for an alternative name
;741:	if (altname && altname[0]) {
ADDRLP4 1064
ADDRFP4 16
INDIRP4
ASGNP4
ADDRLP4 1064
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $452
ADDRLP4 1064
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $452
line 742
;742:		botname = altname;
ADDRLP4 1048
ADDRFP4 16
INDIRP4
ASGNP4
line 743
;743:	}
LABELV $452
line 744
;744:	Info_SetValueForKey( userinfo, "name", botname );
ADDRLP4 0
ARGP4
ADDRGP4 $229
ARGP4
ADDRLP4 1048
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 745
;745:	Info_SetValueForKey( userinfo, "rate", "25000" );
ADDRLP4 0
ARGP4
ADDRGP4 $454
ARGP4
ADDRGP4 $455
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 746
;746:	Info_SetValueForKey( userinfo, "snaps", "20" );
ADDRLP4 0
ARGP4
ADDRGP4 $456
ARGP4
ADDRGP4 $457
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 747
;747:	Info_SetValueForKey( userinfo, "skill", va("%1.2f", skill) );
ADDRGP4 $458
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRLP4 1068
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
ADDRGP4 $438
ARGP4
ADDRLP4 1068
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 749
;748:
;749:	if ( skill >= 1 && skill < 2 ) {
ADDRLP4 1072
ADDRFP4 4
INDIRF4
ASGNF4
ADDRLP4 1072
INDIRF4
CNSTF4 1065353216
LTF4 $459
ADDRLP4 1072
INDIRF4
CNSTF4 1073741824
GEF4 $459
line 750
;750:		Info_SetValueForKey( userinfo, "handicap", "50" );
ADDRLP4 0
ARGP4
ADDRGP4 $461
ARGP4
ADDRGP4 $462
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 751
;751:	}
ADDRGP4 $460
JUMPV
LABELV $459
line 752
;752:	else if ( skill >= 2 && skill < 3 ) {
ADDRLP4 1076
ADDRFP4 4
INDIRF4
ASGNF4
ADDRLP4 1076
INDIRF4
CNSTF4 1073741824
LTF4 $463
ADDRLP4 1076
INDIRF4
CNSTF4 1077936128
GEF4 $463
line 753
;753:		Info_SetValueForKey( userinfo, "handicap", "70" );
ADDRLP4 0
ARGP4
ADDRGP4 $461
ARGP4
ADDRGP4 $465
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 754
;754:	}
ADDRGP4 $464
JUMPV
LABELV $463
line 755
;755:	else if ( skill >= 3 && skill < 4 ) {
ADDRLP4 1080
ADDRFP4 4
INDIRF4
ASGNF4
ADDRLP4 1080
INDIRF4
CNSTF4 1077936128
LTF4 $466
ADDRLP4 1080
INDIRF4
CNSTF4 1082130432
GEF4 $466
line 756
;756:		Info_SetValueForKey( userinfo, "handicap", "90" );
ADDRLP4 0
ARGP4
ADDRGP4 $461
ARGP4
ADDRGP4 $468
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 757
;757:	}
LABELV $466
LABELV $464
LABELV $460
line 759
;758:
;759:	key = "model";
ADDRLP4 1024
ADDRGP4 $415
ASGNP4
line 760
;760:	model = Info_ValueForKey( botinfo, key );
ADDRLP4 1032
INDIRP4
ARGP4
ADDRLP4 1024
INDIRP4
ARGP4
ADDRLP4 1084
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1044
ADDRLP4 1084
INDIRP4
ASGNP4
line 761
;761:	if ( !*model ) {
ADDRLP4 1044
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $469
line 762
;762:		model = "visor/default";
ADDRLP4 1044
ADDRGP4 $471
ASGNP4
line 763
;763:	}
LABELV $469
line 764
;764:	Info_SetValueForKey( userinfo, key, model );
ADDRLP4 0
ARGP4
ADDRLP4 1024
INDIRP4
ARGP4
ADDRLP4 1044
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 765
;765:	key = "team_model";
ADDRLP4 1024
ADDRGP4 $472
ASGNP4
line 766
;766:	Info_SetValueForKey( userinfo, key, model );
ADDRLP4 0
ARGP4
ADDRLP4 1024
INDIRP4
ARGP4
ADDRLP4 1044
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 777
;767:
;768:/*	key = "headmodel";
;769:	headmodel = Info_ValueForKey( botinfo, key );
;770:	if ( !*headmodel ) {
;771:		headmodel = model;
;772:	}
;773:	Info_SetValueForKey( userinfo, key, headmodel );
;774:	key = "team_headmodel";
;775:	Info_SetValueForKey( userinfo, key, headmodel );
;776:*/
;777:	key = "gender";
ADDRLP4 1024
ADDRGP4 $473
ASGNP4
line 778
;778:	s = Info_ValueForKey( botinfo, key );
ADDRLP4 1032
INDIRP4
ARGP4
ADDRLP4 1024
INDIRP4
ARGP4
ADDRLP4 1088
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 1088
INDIRP4
ASGNP4
line 779
;779:	if ( !*s ) {
ADDRLP4 1028
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $474
line 780
;780:		s = "male";
ADDRLP4 1028
ADDRGP4 $476
ASGNP4
line 781
;781:	}
LABELV $474
line 782
;782:	Info_SetValueForKey( userinfo, "sex", s );
ADDRLP4 0
ARGP4
ADDRGP4 $477
ARGP4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 784
;783:
;784:	key = "color1";
ADDRLP4 1024
ADDRGP4 $478
ASGNP4
line 785
;785:	s = Info_ValueForKey( botinfo, key );
ADDRLP4 1032
INDIRP4
ARGP4
ADDRLP4 1024
INDIRP4
ARGP4
ADDRLP4 1092
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 1092
INDIRP4
ASGNP4
line 786
;786:	if ( !*s ) {
ADDRLP4 1028
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $479
line 787
;787:		s = "4";
ADDRLP4 1028
ADDRGP4 $481
ASGNP4
line 788
;788:	}
LABELV $479
line 789
;789:	Info_SetValueForKey( userinfo, key, s );
ADDRLP4 0
ARGP4
ADDRLP4 1024
INDIRP4
ARGP4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 791
;790:
;791:	key = "color2";
ADDRLP4 1024
ADDRGP4 $482
ASGNP4
line 792
;792:	s = Info_ValueForKey( botinfo, key );
ADDRLP4 1032
INDIRP4
ARGP4
ADDRLP4 1024
INDIRP4
ARGP4
ADDRLP4 1096
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 1096
INDIRP4
ASGNP4
line 793
;793:	if ( !*s ) {
ADDRLP4 1028
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $483
line 794
;794:		s = "5";
ADDRLP4 1028
ADDRGP4 $485
ASGNP4
line 795
;795:	}
LABELV $483
line 796
;796:	Info_SetValueForKey( userinfo, key, s );
ADDRLP4 0
ARGP4
ADDRLP4 1024
INDIRP4
ARGP4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 798
;797:
;798:	s = Info_ValueForKey(botinfo, "personality");
ADDRLP4 1032
INDIRP4
ARGP4
ADDRGP4 $436
ARGP4
ADDRLP4 1100
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ADDRLP4 1100
INDIRP4
ASGNP4
line 799
;799:	if (!*s )
ADDRLP4 1028
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $486
line 800
;800:	{
line 801
;801:		Info_SetValueForKey( userinfo, "personality", "botfiles/default.jkb" );
ADDRLP4 0
ARGP4
ADDRGP4 $436
ARGP4
ADDRGP4 $488
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 802
;802:	}
ADDRGP4 $487
JUMPV
LABELV $486
line 804
;803:	else
;804:	{
line 805
;805:		Info_SetValueForKey( userinfo, "personality", s );
ADDRLP4 0
ARGP4
ADDRGP4 $436
ARGP4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 806
;806:	}
LABELV $487
line 809
;807:
;808:	// have the server allocate a client slot
;809:	clientNum = trap_BotAllocateClient();
ADDRLP4 1104
ADDRGP4 trap_BotAllocateClient
CALLI4
ASGNI4
ADDRLP4 1036
ADDRLP4 1104
INDIRI4
ASGNI4
line 810
;810:	if ( clientNum == -1 ) {
ADDRLP4 1036
INDIRI4
CNSTI4 -1
NEI4 $489
line 813
;811://		G_Printf( S_COLOR_RED "Unable to add bot.  All player slots are in use.\n" );
;812://		G_Printf( S_COLOR_RED "Start server with more 'open' slots.\n" );
;813:		trap_SendServerCommand( -1, va("print \"%s\n\"", G_GetStripEdString("SVINGAME", "UNABLE_TO_ADD_BOT")));
ADDRGP4 $492
ARGP4
ADDRGP4 $493
ARGP4
ADDRLP4 1108
ADDRGP4 G_GetStripEdString
CALLP4
ASGNP4
ADDRGP4 $491
ARGP4
ADDRLP4 1108
INDIRP4
ARGP4
ADDRLP4 1112
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 1112
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 814
;814:		return;
ADDRGP4 $445
JUMPV
LABELV $489
line 818
;815:	}
;816:
;817:	// initialize the bot settings
;818:	if( !team || !*team ) {
ADDRLP4 1108
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 1108
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $496
ADDRLP4 1108
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $494
LABELV $496
line 819
;819:		if( g_gametype.integer >= GT_TEAM ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
LTI4 $497
line 820
;820:			if( PickTeam(clientNum) == TEAM_RED) {
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 1112
ADDRGP4 PickTeam
CALLI4
ASGNI4
ADDRLP4 1112
INDIRI4
CNSTI4 1
NEI4 $500
line 821
;821:				team = "red";
ADDRFP4 8
ADDRGP4 $275
ASGNP4
line 822
;822:			}
ADDRGP4 $498
JUMPV
LABELV $500
line 823
;823:			else {
line 824
;824:				team = "blue";
ADDRFP4 8
ADDRGP4 $278
ASGNP4
line 825
;825:			}
line 826
;826:		}
ADDRGP4 $498
JUMPV
LABELV $497
line 827
;827:		else {
line 828
;828:			team = "red";
ADDRFP4 8
ADDRGP4 $275
ASGNP4
line 829
;829:		}
LABELV $498
line 830
;830:	}
LABELV $494
line 832
;831://	Info_SetValueForKey( userinfo, "characterfile", Info_ValueForKey( botinfo, "aifile" ) );
;832:	Info_SetValueForKey( userinfo, "skill", va( "%5.2f", skill ) );
ADDRGP4 $502
ARGP4
ADDRFP4 4
INDIRF4
ARGF4
ADDRLP4 1112
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
ARGP4
ADDRGP4 $438
ARGP4
ADDRLP4 1112
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 833
;833:	Info_SetValueForKey( userinfo, "team", team );
ADDRLP4 0
ARGP4
ADDRGP4 $440
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 835
;834:
;835:	bot = &g_entities[ clientNum ];
ADDRLP4 1040
CNSTI4 828
ADDRLP4 1036
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 836
;836:	bot->r.svFlags |= SVF_BOT;
ADDRLP4 1116
ADDRLP4 1040
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 1116
INDIRP4
ADDRLP4 1116
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 837
;837:	bot->inuse = qtrue;
ADDRLP4 1040
INDIRP4
CNSTI4 412
ADDP4
CNSTI4 1
ASGNI4
line 840
;838:
;839:	// register the userinfo
;840:	trap_SetUserinfo( clientNum, userinfo );
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 trap_SetUserinfo
CALLV
pop
line 842
;841:
;842:	if (g_gametype.integer >= GT_TEAM)
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 5
LTI4 $503
line 843
;843:	{
line 844
;844:		if (team && Q_stricmp(team, "red") == 0)
ADDRLP4 1120
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 1120
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $506
ADDRLP4 1120
INDIRP4
ARGP4
ADDRGP4 $275
ARGP4
ADDRLP4 1124
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1124
INDIRI4
CNSTI4 0
NEI4 $506
line 845
;845:		{
line 846
;846:			bot->client->sess.sessionTeam = TEAM_RED;
ADDRLP4 1040
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
CNSTI4 1
ASGNI4
line 847
;847:		}
ADDRGP4 $507
JUMPV
LABELV $506
line 848
;848:		else if (team && Q_stricmp(team, "blue") == 0)
ADDRLP4 1128
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 1128
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $508
ADDRLP4 1128
INDIRP4
ARGP4
ADDRGP4 $278
ARGP4
ADDRLP4 1132
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1132
INDIRI4
CNSTI4 0
NEI4 $508
line 849
;849:		{
line 850
;850:			bot->client->sess.sessionTeam = TEAM_BLUE;
ADDRLP4 1040
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
CNSTI4 2
ASGNI4
line 851
;851:		}
ADDRGP4 $509
JUMPV
LABELV $508
line 853
;852:		else
;853:		{
line 854
;854:			bot->client->sess.sessionTeam = PickTeam( -1 );
CNSTI4 -1
ARGI4
ADDRLP4 1136
ADDRGP4 PickTeam
CALLI4
ASGNI4
ADDRLP4 1040
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
ADDRLP4 1136
INDIRI4
ASGNI4
line 855
;855:		}
LABELV $509
LABELV $507
line 856
;856:	}
LABELV $503
line 858
;857:
;858:	preTeam = bot->client->sess.sessionTeam;
ADDRLP4 1052
ADDRLP4 1040
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ASGNI4
line 861
;859:
;860:	// have it connect to the game as a normal client
;861:	if ( ClientConnect( clientNum, qtrue, qtrue ) ) {
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 1120
CNSTI4 1
ASGNI4
ADDRLP4 1120
INDIRI4
ARGI4
ADDRLP4 1120
INDIRI4
ARGI4
ADDRLP4 1124
ADDRGP4 ClientConnect
CALLP4
ASGNP4
ADDRLP4 1124
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $510
line 862
;862:		return;
ADDRGP4 $445
JUMPV
LABELV $510
line 865
;863:	}
;864:
;865:	if (bot->client->sess.sessionTeam != preTeam)
ADDRLP4 1040
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ADDRLP4 1052
INDIRI4
EQI4 $512
line 866
;866:	{
line 867
;867:		trap_GetUserinfo(clientNum, userinfo, MAX_INFO_STRING);
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetUserinfo
CALLV
pop
line 869
;868:
;869:		if (bot->client->sess.sessionTeam == TEAM_SPECTATOR)
ADDRLP4 1040
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 3
NEI4 $514
line 870
;870:		{
line 871
;871:			bot->client->sess.sessionTeam = preTeam;
ADDRLP4 1040
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
ADDRLP4 1052
INDIRI4
ASGNI4
line 872
;872:		}
LABELV $514
line 874
;873:
;874:		if (bot->client->sess.sessionTeam == TEAM_RED)
ADDRLP4 1040
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
CNSTI4 1
NEI4 $516
line 875
;875:		{
line 876
;876:			team = "Red";
ADDRFP4 8
ADDRGP4 $518
ASGNP4
line 877
;877:		}
ADDRGP4 $517
JUMPV
LABELV $516
line 879
;878:		else
;879:		{
line 880
;880:			team = "Blue";
ADDRFP4 8
ADDRGP4 $519
ASGNP4
line 881
;881:		}
LABELV $517
line 883
;882:
;883:		Info_SetValueForKey( userinfo, "team", team );
ADDRLP4 0
ARGP4
ADDRGP4 $440
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Info_SetValueForKey
CALLV
pop
line 885
;884:
;885:		trap_SetUserinfo( clientNum, userinfo );
ADDRLP4 1036
INDIRI4
ARGI4
ADDRLP4 0
ARGP4
ADDRGP4 trap_SetUserinfo
CALLV
pop
line 887
;886:
;887:		bot->client->ps.persistant[ PERS_TEAM ] = bot->client->sess.sessionTeam;
ADDRLP4 1128
ADDRLP4 1040
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ASGNP4
ADDRLP4 1128
INDIRP4
CNSTI4 292
ADDP4
ADDRLP4 1128
INDIRP4
CNSTI4 1520
ADDP4
INDIRI4
ASGNI4
line 889
;888:
;889:		G_ReadSessionData( bot->client );
ADDRLP4 1040
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ARGP4
ADDRGP4 G_ReadSessionData
CALLV
pop
line 890
;890:		ClientUserinfoChanged( clientNum );
ADDRLP4 1036
INDIRI4
ARGI4
ADDRGP4 ClientUserinfoChanged
CALLV
pop
line 891
;891:	}
LABELV $512
line 893
;892:
;893:	if( delay == 0 ) {
ADDRFP4 12
INDIRI4
CNSTI4 0
NEI4 $520
line 894
;894:		ClientBegin( clientNum, qfalse );
ADDRLP4 1036
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 ClientBegin
CALLV
pop
line 895
;895:		return;
ADDRGP4 $445
JUMPV
LABELV $520
line 898
;896:	}
;897:
;898:	AddBotToSpawnQueue( clientNum, delay );
ADDRLP4 1036
INDIRI4
ARGI4
ADDRFP4 12
INDIRI4
ARGI4
ADDRGP4 AddBotToSpawnQueue
CALLV
pop
line 899
;899:}
LABELV $445
endproc G_AddBot 1140 12
export Svcmd_AddBot_f
proc Svcmd_AddBot_f 4112 20
line 907
;900:
;901:
;902:/*
;903:===============
;904:Svcmd_AddBot_f
;905:===============
;906:*/
;907:void Svcmd_AddBot_f( void ) {
line 916
;908:	float			skill;
;909:	int				delay;
;910:	char			name[MAX_TOKEN_CHARS];
;911:	char			altname[MAX_TOKEN_CHARS];
;912:	char			string[MAX_TOKEN_CHARS];
;913:	char			team[MAX_TOKEN_CHARS];
;914:
;915:	// are bots enabled?
;916:	if ( !trap_Cvar_VariableIntegerValue( "bot_enable" ) ) {
ADDRGP4 $525
ARGP4
ADDRLP4 4104
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRLP4 4104
INDIRI4
CNSTI4 0
NEI4 $523
line 917
;917:		return;
ADDRGP4 $522
JUMPV
LABELV $523
line 921
;918:	}
;919:
;920:	// name
;921:	trap_Argv( 1, name, sizeof( name ) );
CNSTI4 1
ARGI4
ADDRLP4 1024
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 922
;922:	if ( !name[0] ) {
ADDRLP4 1024
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $526
line 923
;923:		trap_Printf( "Usage: Addbot <botname> [skill 1-5] [team] [msec delay] [altname]\n" );
ADDRGP4 $528
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 924
;924:		return;
ADDRGP4 $522
JUMPV
LABELV $526
line 928
;925:	}
;926:
;927:	// skill
;928:	trap_Argv( 2, string, sizeof( string ) );
CNSTI4 2
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 929
;929:	if ( !string[0] ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $529
line 930
;930:		skill = 4;
ADDRLP4 4096
CNSTF4 1082130432
ASGNF4
line 931
;931:	}
ADDRGP4 $530
JUMPV
LABELV $529
line 932
;932:	else {
line 933
;933:		skill = atof( string );
ADDRLP4 0
ARGP4
ADDRLP4 4108
ADDRGP4 atof
CALLF4
ASGNF4
ADDRLP4 4096
ADDRLP4 4108
INDIRF4
ASGNF4
line 934
;934:	}
LABELV $530
line 937
;935:
;936:	// team
;937:	trap_Argv( 3, team, sizeof( team ) );
CNSTI4 3
ARGI4
ADDRLP4 3072
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 940
;938:
;939:	// delay
;940:	trap_Argv( 4, string, sizeof( string ) );
CNSTI4 4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 941
;941:	if ( !string[0] ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $531
line 942
;942:		delay = 0;
ADDRLP4 4100
CNSTI4 0
ASGNI4
line 943
;943:	}
ADDRGP4 $532
JUMPV
LABELV $531
line 944
;944:	else {
line 945
;945:		delay = atoi( string );
ADDRLP4 0
ARGP4
ADDRLP4 4108
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 4100
ADDRLP4 4108
INDIRI4
ASGNI4
line 946
;946:	}
LABELV $532
line 949
;947:
;948:	// alternative name
;949:	trap_Argv( 5, altname, sizeof( altname ) );
CNSTI4 5
ARGI4
ADDRLP4 2048
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 951
;950:
;951:	G_AddBot( name, skill, team, delay, altname );
ADDRLP4 1024
ARGP4
ADDRLP4 4096
INDIRF4
ARGF4
ADDRLP4 3072
ARGP4
ADDRLP4 4100
INDIRI4
ARGI4
ADDRLP4 2048
ARGP4
ADDRGP4 G_AddBot
CALLV
pop
line 955
;952:
;953:	// if this was issued during gameplay and we are playing locally,
;954:	// go ahead and load the bot's media immediately
;955:	if ( level.time - level.startTime > 1000 &&
ADDRGP4 level+32
INDIRI4
ADDRGP4 level+40
INDIRI4
SUBI4
CNSTI4 1000
LEI4 $533
ADDRGP4 $537
ARGP4
ADDRLP4 4108
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRLP4 4108
INDIRI4
CNSTI4 0
EQI4 $533
line 956
;956:		trap_Cvar_VariableIntegerValue( "cl_running" ) ) {
line 957
;957:		trap_SendServerCommand( -1, "loaddefered\n" );	// FIXME: spelled wrong, but not changing for demo
CNSTI4 -1
ARGI4
ADDRGP4 $538
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 958
;958:	}
LABELV $533
line 959
;959:}
LABELV $522
endproc Svcmd_AddBot_f 4112 20
export Svcmd_BotList_f
proc Svcmd_BotList_f 4120 20
line 966
;960:
;961:/*
;962:===============
;963:Svcmd_BotList_f
;964:===============
;965:*/
;966:void Svcmd_BotList_f( void ) {
line 973
;967:	int i;
;968:	char name[MAX_TOKEN_CHARS];
;969:	char funname[MAX_TOKEN_CHARS];
;970:	char model[MAX_TOKEN_CHARS];
;971:	char personality[MAX_TOKEN_CHARS];
;972:
;973:	trap_Printf("^1name             model            personality              funname\n");
ADDRGP4 $540
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 974
;974:	for (i = 0; i < g_numBots; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $544
JUMPV
LABELV $541
line 975
;975:		strcpy(name, Info_ValueForKey( g_botInfos[i], "name" ));
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $229
ARGP4
ADDRLP4 4100
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ARGP4
ADDRLP4 4100
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 976
;976:		if ( !*name ) {
ADDRLP4 4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $545
line 977
;977:			strcpy(name, "Padawan");
ADDRLP4 4
ARGP4
ADDRGP4 $547
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 978
;978:		}
LABELV $545
line 979
;979:		strcpy(funname, Info_ValueForKey( g_botInfos[i], "funname" ));
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $449
ARGP4
ADDRLP4 4104
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1028
ARGP4
ADDRLP4 4104
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 980
;980:		if ( !*funname ) {
ADDRLP4 1028
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $548
line 981
;981:			strcpy(funname, "");
ADDRLP4 1028
ARGP4
ADDRGP4 $165
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 982
;982:		}
LABELV $548
line 983
;983:		strcpy(model, Info_ValueForKey( g_botInfos[i], "model" ));
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $415
ARGP4
ADDRLP4 4108
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 2052
ARGP4
ADDRLP4 4108
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 984
;984:		if ( !*model ) {
ADDRLP4 2052
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $550
line 985
;985:			strcpy(model, "visor/default");
ADDRLP4 2052
ARGP4
ADDRGP4 $471
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 986
;986:		}
LABELV $550
line 987
;987:		strcpy(personality, Info_ValueForKey( g_botInfos[i], "personality"));
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $436
ARGP4
ADDRLP4 4112
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 3076
ARGP4
ADDRLP4 4112
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 988
;988:		if (!*personality ) {
ADDRLP4 3076
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $552
line 989
;989:			strcpy(personality, "botfiles/default.jkb");
ADDRLP4 3076
ARGP4
ADDRGP4 $488
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 990
;990:		}
LABELV $552
line 991
;991:		trap_Printf(va("%-16s %-16s %-20s %-20s\n", name, model, personality, funname));
ADDRGP4 $554
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 2052
ARGP4
ADDRLP4 3076
ARGP4
ADDRLP4 1028
ARGP4
ADDRLP4 4116
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4116
INDIRP4
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 992
;992:	}
LABELV $542
line 974
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $544
ADDRLP4 0
INDIRI4
ADDRGP4 g_numBots
INDIRI4
LTI4 $541
line 993
;993:}
LABELV $539
endproc Svcmd_BotList_f 4120 20
proc G_SpawnBots 1056 16
line 1001
;994:
;995:
;996:/*
;997:===============
;998:G_SpawnBots
;999:===============
;1000:*/
;1001:static void G_SpawnBots( char *botList, int baseDelay ) {
line 1008
;1002:	char		*bot;
;1003:	char		*p;
;1004:	float		skill;
;1005:	int			delay;
;1006:	char		bots[MAX_INFO_VALUE];
;1007:
;1008:	podium1 = NULL;
ADDRGP4 podium1
CNSTP4 0
ASGNP4
line 1009
;1009:	podium2 = NULL;
ADDRGP4 podium2
CNSTP4 0
ASGNP4
line 1010
;1010:	podium3 = NULL;
ADDRGP4 podium3
CNSTP4 0
ASGNP4
line 1012
;1011:
;1012:	skill = trap_Cvar_VariableValue( "g_spSkill" );
ADDRGP4 $272
ARGP4
ADDRLP4 1040
ADDRGP4 trap_Cvar_VariableValue
CALLF4
ASGNF4
ADDRLP4 12
ADDRLP4 1040
INDIRF4
ASGNF4
line 1013
;1013:	if( skill < 1 ) {
ADDRLP4 12
INDIRF4
CNSTF4 1065353216
GEF4 $556
line 1014
;1014:		trap_Cvar_Set( "g_spSkill", "1" );
ADDRGP4 $272
ARGP4
ADDRGP4 $558
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1015
;1015:		skill = 1;
ADDRLP4 12
CNSTF4 1065353216
ASGNF4
line 1016
;1016:	}
ADDRGP4 $557
JUMPV
LABELV $556
line 1017
;1017:	else if ( skill > 5 ) {
ADDRLP4 12
INDIRF4
CNSTF4 1084227584
LEF4 $559
line 1018
;1018:		trap_Cvar_Set( "g_spSkill", "5" );
ADDRGP4 $272
ARGP4
ADDRGP4 $485
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1019
;1019:		skill = 5;
ADDRLP4 12
CNSTF4 1084227584
ASGNF4
line 1020
;1020:	}
LABELV $559
LABELV $557
line 1022
;1021:
;1022:	Q_strncpyz( bots, botList, sizeof(bots) );
ADDRLP4 16
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1023
;1023:	p = &bots[0];
ADDRLP4 0
ADDRLP4 16
ASGNP4
line 1024
;1024:	delay = baseDelay;
ADDRLP4 4
ADDRFP4 4
INDIRI4
ASGNI4
ADDRGP4 $562
JUMPV
line 1025
;1025:	while( *p ) {
LABELV $564
line 1027
;1026:		//skip spaces
;1027:		while( *p && *p == ' ' ) {
line 1028
;1028:			p++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 1029
;1029:		}
LABELV $565
line 1027
ADDRLP4 1044
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
EQI4 $567
ADDRLP4 1044
INDIRI4
CNSTI4 32
EQI4 $564
LABELV $567
line 1030
;1030:		if( !p ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $568
line 1031
;1031:			break;
ADDRGP4 $563
JUMPV
LABELV $568
line 1035
;1032:		}
;1033:
;1034:		// mark start of bot name
;1035:		bot = p;
ADDRLP4 8
ADDRLP4 0
INDIRP4
ASGNP4
ADDRGP4 $571
JUMPV
LABELV $570
line 1038
;1036:
;1037:		// skip until space of null
;1038:		while( *p && *p != ' ' ) {
line 1039
;1039:			p++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 1040
;1040:		}
LABELV $571
line 1038
ADDRLP4 1048
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
EQI4 $573
ADDRLP4 1048
INDIRI4
CNSTI4 32
NEI4 $570
LABELV $573
line 1041
;1041:		if( *p ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $574
line 1042
;1042:			*p++ = 0;
ADDRLP4 1052
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 1052
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 1052
INDIRP4
CNSTI1 0
ASGNI1
line 1043
;1043:		}
LABELV $574
line 1047
;1044:
;1045:		// we must add the bot this way, calling G_AddBot directly at this stage
;1046:		// does "Bad Things"
;1047:		trap_SendConsoleCommand( EXEC_INSERT, va("addbot %s %f free %i\n", bot, skill, delay) );
ADDRGP4 $576
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 12
INDIRF4
ARGF4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 1052
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 1
ARGI4
ADDRLP4 1052
INDIRP4
ARGP4
ADDRGP4 trap_SendConsoleCommand
CALLV
pop
line 1049
;1048:
;1049:		delay += BOT_BEGIN_DELAY_INCREMENT;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1500
ADDI4
ASGNI4
line 1050
;1050:	}
LABELV $562
line 1025
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $565
LABELV $563
line 1051
;1051:}
LABELV $555
endproc G_SpawnBots 1056 16
proc G_LoadBotsFromFile 8216 16
line 1059
;1052:
;1053:
;1054:/*
;1055:===============
;1056:G_LoadBotsFromFile
;1057:===============
;1058:*/
;1059:static void G_LoadBotsFromFile( char *filename ) {
line 1064
;1060:	int				len;
;1061:	fileHandle_t	f;
;1062:	char			buf[MAX_BOTS_TEXT];
;1063:
;1064:	len = trap_FS_FOpenFile( filename, &f, FS_READ );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 8200
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8200
INDIRI4
ASGNI4
line 1065
;1065:	if ( !f ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $578
line 1066
;1066:		trap_Printf( va( S_COLOR_RED "file not found: %s\n", filename ) );
ADDRGP4 $112
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8204
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8204
INDIRP4
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 1067
;1067:		return;
ADDRGP4 $577
JUMPV
LABELV $578
line 1069
;1068:	}
;1069:	if ( len >= MAX_BOTS_TEXT ) {
ADDRLP4 0
INDIRI4
CNSTI4 8192
LTI4 $580
line 1070
;1070:		trap_Printf( va( S_COLOR_RED "file too large: %s is %i, max allowed is %i", filename, len, MAX_BOTS_TEXT ) );
ADDRGP4 $115
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
CNSTI4 8192
ARGI4
ADDRLP4 8204
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8204
INDIRP4
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 1071
;1071:		trap_FS_FCloseFile( f );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 1072
;1072:		return;
ADDRGP4 $577
JUMPV
LABELV $580
line 1075
;1073:	}
;1074:
;1075:	trap_FS_Read( buf, len, f );
ADDRLP4 8
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_Read
CALLV
pop
line 1076
;1076:	buf[len] = 0;
ADDRLP4 0
INDIRI4
ADDRLP4 8
ADDP4
CNSTI1 0
ASGNI1
line 1077
;1077:	trap_FS_FCloseFile( f );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 1079
;1078:
;1079:	g_numBots += G_ParseInfos( buf, MAX_BOTS - g_numBots, &g_botInfos[g_numBots] );
ADDRLP4 8
ARGP4
ADDRLP4 8204
ADDRGP4 g_numBots
ASGNP4
ADDRLP4 8208
ADDRLP4 8204
INDIRP4
INDIRI4
ASGNI4
CNSTI4 1024
ADDRLP4 8208
INDIRI4
SUBI4
ARGI4
ADDRLP4 8208
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
ARGP4
ADDRLP4 8212
ADDRGP4 G_ParseInfos
CALLI4
ASGNI4
ADDRLP4 8204
INDIRP4
ADDRLP4 8208
INDIRI4
ADDRLP4 8212
INDIRI4
ADDI4
ASGNI4
line 1080
;1080:}
LABELV $577
endproc G_LoadBotsFromFile 8216 16
proc G_LoadBots 1456 16
line 1087
;1081:
;1082:/*
;1083:===============
;1084:G_LoadBots
;1085:===============
;1086:*/
;1087:static void G_LoadBots( void ) {
line 1096
;1088:	vmCvar_t	botsFile;
;1089:	int			numdirs;
;1090:	char		filename[128];
;1091:	char		dirlist[1024];
;1092:	char*		dirptr;
;1093:	int			i;
;1094:	int			dirlen;
;1095:
;1096:	if ( !trap_Cvar_VariableIntegerValue( "bot_enable" ) ) {
ADDRGP4 $525
ARGP4
ADDRLP4 1440
ADDRGP4 trap_Cvar_VariableIntegerValue
CALLI4
ASGNI4
ADDRLP4 1440
INDIRI4
CNSTI4 0
NEI4 $583
line 1097
;1097:		return;
ADDRGP4 $582
JUMPV
LABELV $583
line 1100
;1098:	}
;1099:
;1100:	g_numBots = 0;
ADDRGP4 g_numBots
CNSTI4 0
ASGNI4
line 1102
;1101:
;1102:	trap_Cvar_Register( &botsFile, "g_botsFile", "", CVAR_INIT|CVAR_ROM );
ADDRLP4 144
ARGP4
ADDRGP4 $585
ARGP4
ADDRGP4 $165
ARGP4
CNSTI4 80
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 1103
;1103:	if( *botsFile.string ) {
ADDRLP4 144+16
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $586
line 1104
;1104:		G_LoadBotsFromFile(botsFile.string);
ADDRLP4 144+16
ARGP4
ADDRGP4 G_LoadBotsFromFile
CALLV
pop
line 1105
;1105:	}
ADDRGP4 $587
JUMPV
LABELV $586
line 1106
;1106:	else {
line 1108
;1107:		//G_LoadBotsFromFile("scripts/bots.txt");
;1108:		G_LoadBotsFromFile("botfiles/bots.txt");
ADDRGP4 $590
ARGP4
ADDRGP4 G_LoadBotsFromFile
CALLV
pop
line 1109
;1109:	}
LABELV $587
line 1112
;1110:
;1111:	// get all bots from .bot files
;1112:	numdirs = trap_FS_GetFileList("scripts", ".bot", dirlist, 1024 );
ADDRGP4 $195
ARGP4
ADDRGP4 $591
ARGP4
ADDRLP4 416
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 1444
ADDRGP4 trap_FS_GetFileList
CALLI4
ASGNI4
ADDRLP4 140
ADDRLP4 1444
INDIRI4
ASGNI4
line 1113
;1113:	dirptr  = dirlist;
ADDRLP4 0
ADDRLP4 416
ASGNP4
line 1114
;1114:	for (i = 0; i < numdirs; i++, dirptr += dirlen+1) {
ADDRLP4 132
CNSTI4 0
ASGNI4
ADDRGP4 $595
JUMPV
LABELV $592
line 1115
;1115:		dirlen = strlen(dirptr);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1448
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 136
ADDRLP4 1448
INDIRI4
ASGNI4
line 1116
;1116:		strcpy(filename, "scripts/");
ADDRLP4 4
ARGP4
ADDRGP4 $201
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1117
;1117:		strcat(filename, dirptr);
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 1118
;1118:		G_LoadBotsFromFile(filename);
ADDRLP4 4
ARGP4
ADDRGP4 G_LoadBotsFromFile
CALLV
pop
line 1119
;1119:	}
LABELV $593
line 1114
ADDRLP4 1448
CNSTI4 1
ASGNI4
ADDRLP4 132
ADDRLP4 132
INDIRI4
ADDRLP4 1448
INDIRI4
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 136
INDIRI4
ADDRLP4 1448
INDIRI4
ADDI4
ADDRLP4 0
INDIRP4
ADDP4
ASGNP4
LABELV $595
ADDRLP4 132
INDIRI4
ADDRLP4 140
INDIRI4
LTI4 $592
line 1120
;1120:	trap_Printf( va( "%i bots parsed\n", g_numBots ) );
ADDRGP4 $596
ARGP4
ADDRGP4 g_numBots
INDIRI4
ARGI4
ADDRLP4 1452
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1452
INDIRP4
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 1121
;1121:}
LABELV $582
endproc G_LoadBots 1456 16
export G_GetBotInfoByNumber
proc G_GetBotInfoByNumber 8 8
line 1130
;1122:
;1123:
;1124:
;1125:/*
;1126:===============
;1127:G_GetBotInfoByNumber
;1128:===============
;1129:*/
;1130:char *G_GetBotInfoByNumber( int num ) {
line 1131
;1131:	if( num < 0 || num >= g_numBots ) {
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $600
ADDRLP4 0
INDIRI4
ADDRGP4 g_numBots
INDIRI4
LTI4 $598
LABELV $600
line 1132
;1132:		trap_Printf( va( S_COLOR_RED "Invalid bot number: %i\n", num ) );
ADDRGP4 $601
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_Printf
CALLV
pop
line 1133
;1133:		return NULL;
CNSTP4 0
RETP4
ADDRGP4 $597
JUMPV
LABELV $598
line 1135
;1134:	}
;1135:	return g_botInfos[num];
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
RETP4
LABELV $597
endproc G_GetBotInfoByNumber 8 8
export G_GetBotInfoByName
proc G_GetBotInfoByName 16 8
line 1144
;1136:}
;1137:
;1138:
;1139:/*
;1140:===============
;1141:G_GetBotInfoByName
;1142:===============
;1143:*/
;1144:char *G_GetBotInfoByName( const char *name ) {
line 1148
;1145:	int		n;
;1146:	char	*value;
;1147:
;1148:	for ( n = 0; n < g_numBots ; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $606
JUMPV
LABELV $603
line 1149
;1149:		value = Info_ValueForKey( g_botInfos[n], "name" );
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $229
ARGP4
ADDRLP4 8
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
ASGNP4
line 1150
;1150:		if ( !Q_stricmp( value, name ) ) {
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $607
line 1151
;1151:			return g_botInfos[n];
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 g_botInfos
ADDP4
INDIRP4
RETP4
ADDRGP4 $602
JUMPV
LABELV $607
line 1153
;1152:		}
;1153:	}
LABELV $604
line 1148
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $606
ADDRLP4 0
INDIRI4
ADDRGP4 g_numBots
INDIRI4
LTI4 $603
line 1155
;1154:
;1155:	return NULL;
CNSTP4 0
RETP4
LABELV $602
endproc G_GetBotInfoByName 16 8
export G_InitBots
proc G_InitBots 1148 16
line 1167
;1156:}
;1157:
;1158://rww - pd
;1159:void LoadPath_ThisLevel(void);
;1160://end rww
;1161:
;1162:/*
;1163:===============
;1164:G_InitBots
;1165:===============
;1166:*/
;1167:void G_InitBots( qboolean restart ) {
line 1176
;1168:	int			fragLimit;
;1169:	int			timeLimit;
;1170:	const char	*arenainfo;
;1171:	char		*strValue;
;1172:	int			basedelay;
;1173:	char		map[MAX_QPATH];
;1174:	char		serverinfo[MAX_INFO_STRING];
;1175:
;1176:	G_LoadBots();
ADDRGP4 G_LoadBots
CALLV
pop
line 1177
;1177:	G_LoadArenas();
ADDRGP4 G_LoadArenas
CALLV
pop
line 1179
;1178:
;1179:	trap_Cvar_Register( &bot_minplayers, "bot_minplayers", "0", CVAR_SERVERINFO );
ADDRGP4 bot_minplayers
ARGP4
ADDRGP4 $610
ARGP4
ADDRGP4 $611
ARGP4
CNSTI4 4
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 1182
;1180:
;1181:	//rww - new bot route stuff
;1182:	LoadPath_ThisLevel();
ADDRGP4 LoadPath_ThisLevel
CALLV
pop
line 1185
;1183:	//end rww
;1184:
;1185:	if( g_gametype.integer == GT_SINGLE_PLAYER ) {
ADDRGP4 g_gametype+12
INDIRI4
CNSTI4 4
NEI4 $612
line 1186
;1186:		trap_GetServerinfo( serverinfo, sizeof(serverinfo) );
ADDRLP4 80
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetServerinfo
CALLV
pop
line 1187
;1187:		Q_strncpyz( map, Info_ValueForKey( serverinfo, "mapname" ), sizeof(map) );
ADDRLP4 80
ARGP4
ADDRGP4 $164
ARGP4
ADDRLP4 1108
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 16
ARGP4
ADDRLP4 1108
INDIRP4
ARGP4
CNSTI4 64
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 1188
;1188:		arenainfo = G_GetArenaInfoByMap( map );
ADDRLP4 16
ARGP4
ADDRLP4 1112
ADDRGP4 G_GetArenaInfoByMap
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 1112
INDIRP4
ASGNP4
line 1189
;1189:		if ( !arenainfo ) {
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $615
line 1190
;1190:			return;
ADDRGP4 $609
JUMPV
LABELV $615
line 1193
;1191:		}
;1192:
;1193:		strValue = Info_ValueForKey( arenainfo, "fraglimit" );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $617
ARGP4
ADDRLP4 1116
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 1116
INDIRP4
ASGNP4
line 1194
;1194:		fragLimit = atoi( strValue );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1120
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 1120
INDIRI4
ASGNI4
line 1195
;1195:		if ( fragLimit ) {
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $618
line 1196
;1196:			trap_Cvar_Set( "fraglimit", strValue );
ADDRGP4 $617
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1197
;1197:		}
ADDRGP4 $619
JUMPV
LABELV $618
line 1198
;1198:		else {
line 1199
;1199:			trap_Cvar_Set( "fraglimit", "0" );
ADDRGP4 $617
ARGP4
ADDRGP4 $611
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1200
;1200:		}
LABELV $619
line 1202
;1201:
;1202:		strValue = Info_ValueForKey( arenainfo, "timelimit" );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $620
ARGP4
ADDRLP4 1124
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 1124
INDIRP4
ASGNP4
line 1203
;1203:		timeLimit = atoi( strValue );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1128
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 12
ADDRLP4 1128
INDIRI4
ASGNI4
line 1204
;1204:		if ( timeLimit ) {
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $621
line 1205
;1205:			trap_Cvar_Set( "timelimit", strValue );
ADDRGP4 $620
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1206
;1206:		}
ADDRGP4 $622
JUMPV
LABELV $621
line 1207
;1207:		else {
line 1208
;1208:			trap_Cvar_Set( "timelimit", "0" );
ADDRGP4 $620
ARGP4
ADDRGP4 $611
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1209
;1209:		}
LABELV $622
line 1211
;1210:
;1211:		if ( !fragLimit && !timeLimit ) {
ADDRLP4 1132
CNSTI4 0
ASGNI4
ADDRLP4 8
INDIRI4
ADDRLP4 1132
INDIRI4
NEI4 $623
ADDRLP4 12
INDIRI4
ADDRLP4 1132
INDIRI4
NEI4 $623
line 1212
;1212:			trap_Cvar_Set( "fraglimit", "10" );
ADDRGP4 $617
ARGP4
ADDRGP4 $625
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1213
;1213:			trap_Cvar_Set( "timelimit", "0" );
ADDRGP4 $620
ARGP4
ADDRGP4 $611
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 1214
;1214:		}
LABELV $623
line 1216
;1215:
;1216:		basedelay = BOT_BEGIN_DELAY_BASE;
ADDRLP4 1104
CNSTI4 2000
ASGNI4
line 1217
;1217:		strValue = Info_ValueForKey( arenainfo, "special" );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $626
ARGP4
ADDRLP4 1136
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 1136
INDIRP4
ASGNP4
line 1218
;1218:		if( Q_stricmp( strValue, "training" ) == 0 ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $629
ARGP4
ADDRLP4 1140
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1140
INDIRI4
CNSTI4 0
NEI4 $627
line 1219
;1219:			basedelay += 10000;
ADDRLP4 1104
ADDRLP4 1104
INDIRI4
CNSTI4 10000
ADDI4
ASGNI4
line 1220
;1220:		}
LABELV $627
line 1222
;1221:
;1222:		if( !restart ) {
ADDRFP4 0
INDIRI4
CNSTI4 0
NEI4 $630
line 1223
;1223:			G_SpawnBots( Info_ValueForKey( arenainfo, "bots" ), basedelay );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 $632
ARGP4
ADDRLP4 1144
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1144
INDIRP4
ARGP4
ADDRLP4 1104
INDIRI4
ARGI4
ADDRGP4 G_SpawnBots
CALLV
pop
line 1224
;1224:		}
LABELV $630
line 1225
;1225:	}
LABELV $612
line 1226
;1226:}
LABELV $609
endproc G_InitBots 1148 16
import LoadPath_ThisLevel
import podium3
import podium2
import podium1
bss
export bot_minplayers
align 4
LABELV bot_minplayers
skip 272
align 4
LABELV botSpawnQueue
skip 128
align 4
LABELV g_arenaInfos
skip 4096
export g_numArenas
align 4
LABELV g_numArenas
skip 4
align 4
LABELV g_botInfos
skip 4096
align 4
LABELV g_numBots
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
import BotInterbreedEndMatch
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
LABELV $632
char 1 98
char 1 111
char 1 116
char 1 115
char 1 0
align 1
LABELV $629
char 1 116
char 1 114
char 1 97
char 1 105
char 1 110
char 1 105
char 1 110
char 1 103
char 1 0
align 1
LABELV $626
char 1 115
char 1 112
char 1 101
char 1 99
char 1 105
char 1 97
char 1 108
char 1 0
align 1
LABELV $625
char 1 49
char 1 48
char 1 0
align 1
LABELV $620
char 1 116
char 1 105
char 1 109
char 1 101
char 1 108
char 1 105
char 1 109
char 1 105
char 1 116
char 1 0
align 1
LABELV $617
char 1 102
char 1 114
char 1 97
char 1 103
char 1 108
char 1 105
char 1 109
char 1 105
char 1 116
char 1 0
align 1
LABELV $611
char 1 48
char 1 0
align 1
LABELV $610
char 1 98
char 1 111
char 1 116
char 1 95
char 1 109
char 1 105
char 1 110
char 1 112
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 115
char 1 0
align 1
LABELV $601
char 1 94
char 1 49
char 1 73
char 1 110
char 1 118
char 1 97
char 1 108
char 1 105
char 1 100
char 1 32
char 1 98
char 1 111
char 1 116
char 1 32
char 1 110
char 1 117
char 1 109
char 1 98
char 1 101
char 1 114
char 1 58
char 1 32
char 1 37
char 1 105
char 1 10
char 1 0
align 1
LABELV $596
char 1 37
char 1 105
char 1 32
char 1 98
char 1 111
char 1 116
char 1 115
char 1 32
char 1 112
char 1 97
char 1 114
char 1 115
char 1 101
char 1 100
char 1 10
char 1 0
align 1
LABELV $591
char 1 46
char 1 98
char 1 111
char 1 116
char 1 0
align 1
LABELV $590
char 1 98
char 1 111
char 1 116
char 1 102
char 1 105
char 1 108
char 1 101
char 1 115
char 1 47
char 1 98
char 1 111
char 1 116
char 1 115
char 1 46
char 1 116
char 1 120
char 1 116
char 1 0
align 1
LABELV $585
char 1 103
char 1 95
char 1 98
char 1 111
char 1 116
char 1 115
char 1 70
char 1 105
char 1 108
char 1 101
char 1 0
align 1
LABELV $576
char 1 97
char 1 100
char 1 100
char 1 98
char 1 111
char 1 116
char 1 32
char 1 37
char 1 115
char 1 32
char 1 37
char 1 102
char 1 32
char 1 102
char 1 114
char 1 101
char 1 101
char 1 32
char 1 37
char 1 105
char 1 10
char 1 0
align 1
LABELV $558
char 1 49
char 1 0
align 1
LABELV $554
char 1 37
char 1 45
char 1 49
char 1 54
char 1 115
char 1 32
char 1 37
char 1 45
char 1 49
char 1 54
char 1 115
char 1 32
char 1 37
char 1 45
char 1 50
char 1 48
char 1 115
char 1 32
char 1 37
char 1 45
char 1 50
char 1 48
char 1 115
char 1 10
char 1 0
align 1
LABELV $547
char 1 80
char 1 97
char 1 100
char 1 97
char 1 119
char 1 97
char 1 110
char 1 0
align 1
LABELV $540
char 1 94
char 1 49
char 1 110
char 1 97
char 1 109
char 1 101
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 112
char 1 101
char 1 114
char 1 115
char 1 111
char 1 110
char 1 97
char 1 108
char 1 105
char 1 116
char 1 121
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 102
char 1 117
char 1 110
char 1 110
char 1 97
char 1 109
char 1 101
char 1 10
char 1 0
align 1
LABELV $538
char 1 108
char 1 111
char 1 97
char 1 100
char 1 100
char 1 101
char 1 102
char 1 101
char 1 114
char 1 101
char 1 100
char 1 10
char 1 0
align 1
LABELV $537
char 1 99
char 1 108
char 1 95
char 1 114
char 1 117
char 1 110
char 1 110
char 1 105
char 1 110
char 1 103
char 1 0
align 1
LABELV $528
char 1 85
char 1 115
char 1 97
char 1 103
char 1 101
char 1 58
char 1 32
char 1 65
char 1 100
char 1 100
char 1 98
char 1 111
char 1 116
char 1 32
char 1 60
char 1 98
char 1 111
char 1 116
char 1 110
char 1 97
char 1 109
char 1 101
char 1 62
char 1 32
char 1 91
char 1 115
char 1 107
char 1 105
char 1 108
char 1 108
char 1 32
char 1 49
char 1 45
char 1 53
char 1 93
char 1 32
char 1 91
char 1 116
char 1 101
char 1 97
char 1 109
char 1 93
char 1 32
char 1 91
char 1 109
char 1 115
char 1 101
char 1 99
char 1 32
char 1 100
char 1 101
char 1 108
char 1 97
char 1 121
char 1 93
char 1 32
char 1 91
char 1 97
char 1 108
char 1 116
char 1 110
char 1 97
char 1 109
char 1 101
char 1 93
char 1 10
char 1 0
align 1
LABELV $525
char 1 98
char 1 111
char 1 116
char 1 95
char 1 101
char 1 110
char 1 97
char 1 98
char 1 108
char 1 101
char 1 0
align 1
LABELV $519
char 1 66
char 1 108
char 1 117
char 1 101
char 1 0
align 1
LABELV $518
char 1 82
char 1 101
char 1 100
char 1 0
align 1
LABELV $502
char 1 37
char 1 53
char 1 46
char 1 50
char 1 102
char 1 0
align 1
LABELV $493
char 1 85
char 1 78
char 1 65
char 1 66
char 1 76
char 1 69
char 1 95
char 1 84
char 1 79
char 1 95
char 1 65
char 1 68
char 1 68
char 1 95
char 1 66
char 1 79
char 1 84
char 1 0
align 1
LABELV $492
char 1 83
char 1 86
char 1 73
char 1 78
char 1 71
char 1 65
char 1 77
char 1 69
char 1 0
align 1
LABELV $491
char 1 112
char 1 114
char 1 105
char 1 110
char 1 116
char 1 32
char 1 34
char 1 37
char 1 115
char 1 10
char 1 34
char 1 0
align 1
LABELV $488
char 1 98
char 1 111
char 1 116
char 1 102
char 1 105
char 1 108
char 1 101
char 1 115
char 1 47
char 1 100
char 1 101
char 1 102
char 1 97
char 1 117
char 1 108
char 1 116
char 1 46
char 1 106
char 1 107
char 1 98
char 1 0
align 1
LABELV $485
char 1 53
char 1 0
align 1
LABELV $482
char 1 99
char 1 111
char 1 108
char 1 111
char 1 114
char 1 50
char 1 0
align 1
LABELV $481
char 1 52
char 1 0
align 1
LABELV $478
char 1 99
char 1 111
char 1 108
char 1 111
char 1 114
char 1 49
char 1 0
align 1
LABELV $477
char 1 115
char 1 101
char 1 120
char 1 0
align 1
LABELV $476
char 1 109
char 1 97
char 1 108
char 1 101
char 1 0
align 1
LABELV $473
char 1 103
char 1 101
char 1 110
char 1 100
char 1 101
char 1 114
char 1 0
align 1
LABELV $472
char 1 116
char 1 101
char 1 97
char 1 109
char 1 95
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 0
align 1
LABELV $471
char 1 118
char 1 105
char 1 115
char 1 111
char 1 114
char 1 47
char 1 100
char 1 101
char 1 102
char 1 97
char 1 117
char 1 108
char 1 116
char 1 0
align 1
LABELV $468
char 1 57
char 1 48
char 1 0
align 1
LABELV $465
char 1 55
char 1 48
char 1 0
align 1
LABELV $462
char 1 53
char 1 48
char 1 0
align 1
LABELV $461
char 1 104
char 1 97
char 1 110
char 1 100
char 1 105
char 1 99
char 1 97
char 1 112
char 1 0
align 1
LABELV $458
char 1 37
char 1 49
char 1 46
char 1 50
char 1 102
char 1 0
align 1
LABELV $457
char 1 50
char 1 48
char 1 0
align 1
LABELV $456
char 1 115
char 1 110
char 1 97
char 1 112
char 1 115
char 1 0
align 1
LABELV $455
char 1 50
char 1 53
char 1 48
char 1 48
char 1 48
char 1 0
align 1
LABELV $454
char 1 114
char 1 97
char 1 116
char 1 101
char 1 0
align 1
LABELV $449
char 1 102
char 1 117
char 1 110
char 1 110
char 1 97
char 1 109
char 1 101
char 1 0
align 1
LABELV $448
char 1 94
char 1 49
char 1 69
char 1 114
char 1 114
char 1 111
char 1 114
char 1 58
char 1 32
char 1 66
char 1 111
char 1 116
char 1 32
char 1 39
char 1 37
char 1 115
char 1 39
char 1 32
char 1 110
char 1 111
char 1 116
char 1 32
char 1 100
char 1 101
char 1 102
char 1 105
char 1 110
char 1 101
char 1 100
char 1 10
char 1 0
align 1
LABELV $444
char 1 66
char 1 111
char 1 116
char 1 65
char 1 73
char 1 83
char 1 101
char 1 116
char 1 117
char 1 112
char 1 67
char 1 108
char 1 105
char 1 101
char 1 110
char 1 116
char 1 32
char 1 102
char 1 97
char 1 105
char 1 108
char 1 101
char 1 100
char 1 0
align 1
LABELV $440
char 1 116
char 1 101
char 1 97
char 1 109
char 1 0
align 1
LABELV $438
char 1 115
char 1 107
char 1 105
char 1 108
char 1 108
char 1 0
align 1
LABELV $436
char 1 112
char 1 101
char 1 114
char 1 115
char 1 111
char 1 110
char 1 97
char 1 108
char 1 105
char 1 116
char 1 121
char 1 0
align 1
LABELV $426
char 1 94
char 1 51
char 1 85
char 1 110
char 1 97
char 1 98
char 1 108
char 1 101
char 1 32
char 1 116
char 1 111
char 1 32
char 1 100
char 1 101
char 1 108
char 1 97
char 1 121
char 1 32
char 1 115
char 1 112
char 1 97
char 1 119
char 1 110
char 1 10
char 1 0
align 1
LABELV $415
char 1 109
char 1 111
char 1 100
char 1 101
char 1 108
char 1 0
align 1
LABELV $295
char 1 107
char 1 105
char 1 99
char 1 107
char 1 32
char 1 37
char 1 115
char 1 10
char 1 0
align 1
LABELV $280
char 1 97
char 1 100
char 1 100
char 1 98
char 1 111
char 1 116
char 1 32
char 1 37
char 1 115
char 1 32
char 1 37
char 1 102
char 1 32
char 1 37
char 1 115
char 1 32
char 1 37
char 1 105
char 1 10
char 1 0
align 1
LABELV $278
char 1 98
char 1 108
char 1 117
char 1 101
char 1 0
align 1
LABELV $275
char 1 114
char 1 101
char 1 100
char 1 0
align 1
LABELV $272
char 1 103
char 1 95
char 1 115
char 1 112
char 1 83
char 1 107
char 1 105
char 1 108
char 1 108
char 1 0
align 1
LABELV $229
char 1 110
char 1 97
char 1 109
char 1 101
char 1 0
align 1
LABELV $223
char 1 112
char 1 108
char 1 97
char 1 121
char 1 32
char 1 115
char 1 111
char 1 117
char 1 110
char 1 100
char 1 47
char 1 112
char 1 108
char 1 97
char 1 121
char 1 101
char 1 114
char 1 47
char 1 97
char 1 110
char 1 110
char 1 111
char 1 117
char 1 110
char 1 99
char 1 101
char 1 47
char 1 37
char 1 115
char 1 46
char 1 119
char 1 97
char 1 118
char 1 10
char 1 0
align 1
LABELV $222
char 1 100
char 1 101
char 1 102
char 1 97
char 1 117
char 1 108
char 1 116
char 1 0
align 1
LABELV $208
char 1 37
char 1 105
char 1 0
align 1
LABELV $207
char 1 110
char 1 117
char 1 109
char 1 0
align 1
LABELV $202
char 1 37
char 1 105
char 1 32
char 1 97
char 1 114
char 1 101
char 1 110
char 1 97
char 1 115
char 1 32
char 1 112
char 1 97
char 1 114
char 1 115
char 1 101
char 1 100
char 1 10
char 1 0
align 1
LABELV $201
char 1 115
char 1 99
char 1 114
char 1 105
char 1 112
char 1 116
char 1 115
char 1 47
char 1 0
align 1
LABELV $196
char 1 46
char 1 97
char 1 114
char 1 101
char 1 110
char 1 97
char 1 0
align 1
LABELV $195
char 1 115
char 1 99
char 1 114
char 1 105
char 1 112
char 1 116
char 1 115
char 1 0
align 1
LABELV $194
char 1 115
char 1 99
char 1 114
char 1 105
char 1 112
char 1 116
char 1 115
char 1 47
char 1 97
char 1 114
char 1 101
char 1 110
char 1 97
char 1 115
char 1 46
char 1 116
char 1 120
char 1 116
char 1 0
align 1
LABELV $189
char 1 103
char 1 95
char 1 97
char 1 114
char 1 101
char 1 110
char 1 97
char 1 115
char 1 70
char 1 105
char 1 108
char 1 101
char 1 0
align 1
LABELV $187
char 1 109
char 1 97
char 1 112
char 1 32
char 1 37
char 1 115
char 1 0
align 1
LABELV $186
char 1 109
char 1 97
char 1 112
char 1 95
char 1 114
char 1 101
char 1 115
char 1 116
char 1 97
char 1 114
char 1 116
char 1 32
char 1 48
char 1 0
align 1
LABELV $185
char 1 110
char 1 101
char 1 120
char 1 116
char 1 109
char 1 97
char 1 112
char 1 0
align 1
LABELV $165
char 1 0
align 1
LABELV $164
char 1 109
char 1 97
char 1 112
char 1 110
char 1 97
char 1 109
char 1 101
char 1 0
align 1
LABELV $155
char 1 116
char 1 121
char 1 112
char 1 101
char 1 0
align 1
LABELV $150
char 1 109
char 1 97
char 1 112
char 1 0
align 1
LABELV $139
char 1 99
char 1 116
char 1 121
char 1 0
align 1
LABELV $136
char 1 99
char 1 116
char 1 102
char 1 0
align 1
LABELV $133
char 1 115
char 1 97
char 1 103
char 1 97
char 1 0
align 1
LABELV $130
char 1 100
char 1 117
char 1 101
char 1 108
char 1 0
align 1
LABELV $127
char 1 106
char 1 101
char 1 100
char 1 105
char 1 109
char 1 97
char 1 115
char 1 116
char 1 101
char 1 114
char 1 0
align 1
LABELV $124
char 1 104
char 1 111
char 1 108
char 1 111
char 1 99
char 1 114
char 1 111
char 1 110
char 1 0
align 1
LABELV $121
char 1 102
char 1 102
char 1 97
char 1 0
align 1
LABELV $115
char 1 94
char 1 49
char 1 102
char 1 105
char 1 108
char 1 101
char 1 32
char 1 116
char 1 111
char 1 111
char 1 32
char 1 108
char 1 97
char 1 114
char 1 103
char 1 101
char 1 58
char 1 32
char 1 37
char 1 115
char 1 32
char 1 105
char 1 115
char 1 32
char 1 37
char 1 105
char 1 44
char 1 32
char 1 109
char 1 97
char 1 120
char 1 32
char 1 97
char 1 108
char 1 108
char 1 111
char 1 119
char 1 101
char 1 100
char 1 32
char 1 105
char 1 115
char 1 32
char 1 37
char 1 105
char 1 0
align 1
LABELV $112
char 1 94
char 1 49
char 1 102
char 1 105
char 1 108
char 1 101
char 1 32
char 1 110
char 1 111
char 1 116
char 1 32
char 1 102
char 1 111
char 1 117
char 1 110
char 1 100
char 1 58
char 1 32
char 1 37
char 1 115
char 1 10
char 1 0
align 1
LABELV $106
char 1 37
char 1 100
char 1 0
align 1
LABELV $105
char 1 92
char 1 110
char 1 117
char 1 109
char 1 92
char 1 0
align 1
LABELV $104
char 1 60
char 1 78
char 1 85
char 1 76
char 1 76
char 1 62
char 1 0
align 1
LABELV $101
char 1 125
char 1 0
align 1
LABELV $98
char 1 85
char 1 110
char 1 101
char 1 120
char 1 112
char 1 101
char 1 99
char 1 116
char 1 101
char 1 100
char 1 32
char 1 101
char 1 110
char 1 100
char 1 32
char 1 111
char 1 102
char 1 32
char 1 105
char 1 110
char 1 102
char 1 111
char 1 32
char 1 102
char 1 105
char 1 108
char 1 101
char 1 10
char 1 0
align 1
LABELV $92
char 1 77
char 1 97
char 1 120
char 1 32
char 1 105
char 1 110
char 1 102
char 1 111
char 1 115
char 1 32
char 1 101
char 1 120
char 1 99
char 1 101
char 1 101
char 1 100
char 1 101
char 1 100
char 1 10
char 1 0
align 1
LABELV $89
char 1 77
char 1 105
char 1 115
char 1 115
char 1 105
char 1 110
char 1 103
char 1 32
char 1 123
char 1 32
char 1 105
char 1 110
char 1 32
char 1 105
char 1 110
char 1 102
char 1 111
char 1 32
char 1 102
char 1 105
char 1 108
char 1 101
char 1 10
char 1 0
align 1
LABELV $88
char 1 123
char 1 0
