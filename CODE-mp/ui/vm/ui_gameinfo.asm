export UI_ParseInfos
code
proc UI_ParseInfos 2084 12
file "../ui_gameinfo.c"
line 26
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3://
;4:// gameinfo.c
;5://
;6:
;7:#include "ui_local.h"
;8:
;9:
;10://
;11:// arena and bot info
;12://
;13:
;14:
;15:int				ui_numBots;
;16:static char		*ui_botInfos[MAX_BOTS];
;17:
;18:static int		ui_numArenas;
;19:static char		*ui_arenaInfos[MAX_ARENAS];
;20:
;21:/*
;22:===============
;23:UI_ParseInfos
;24:===============
;25:*/
;26:int UI_ParseInfos( char *buf, int max, char *infos[] ) {
line 32
;27:	char	*token;
;28:	int		count;
;29:	char	key[MAX_TOKEN_CHARS];
;30:	char	info[MAX_INFO_STRING];
;31:
;32:	count = 0;
ADDRLP4 2052
CNSTI4 0
ASGNI4
ADDRGP4 $119
JUMPV
LABELV $118
line 34
;33:
;34:	while ( 1 ) {
line 35
;35:		token = COM_Parse( (const char **)&buf );
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
line 36
;36:		if ( !token[0] ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $121
line 37
;37:			break;
ADDRGP4 $120
JUMPV
LABELV $121
line 39
;38:		}
;39:		if ( strcmp( token, "{" ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $125
ARGP4
ADDRLP4 2060
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 2060
INDIRI4
CNSTI4 0
EQI4 $123
line 40
;40:			Com_Printf( "Missing { in info file\n" );
ADDRGP4 $126
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 41
;41:			break;
ADDRGP4 $120
JUMPV
LABELV $123
line 44
;42:		}
;43:
;44:		if ( count == max ) {
ADDRLP4 2052
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $127
line 45
;45:			Com_Printf( "Max infos exceeded\n" );
ADDRGP4 $129
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 46
;46:			break;
ADDRGP4 $120
JUMPV
LABELV $127
line 49
;47:		}
;48:
;49:		info[0] = '\0';
ADDRLP4 1028
CNSTI1 0
ASGNI1
ADDRGP4 $131
JUMPV
LABELV $130
line 50
;50:		while ( 1 ) {
line 51
;51:			token = COM_ParseExt( (const char **)&buf, qtrue );
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
line 52
;52:			if ( !token[0] ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $133
line 53
;53:				Com_Printf( "Unexpected end of info file\n" );
ADDRGP4 $135
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 54
;54:				break;
ADDRGP4 $132
JUMPV
LABELV $133
line 56
;55:			}
;56:			if ( !strcmp( token, "}" ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $138
ARGP4
ADDRLP4 2068
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 2068
INDIRI4
CNSTI4 0
NEI4 $136
line 57
;57:				break;
ADDRGP4 $132
JUMPV
LABELV $136
line 59
;58:			}
;59:			Q_strncpyz( key, token, sizeof( key ) );
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
line 61
;60:
;61:			token = COM_ParseExt( (const char **)&buf, qfalse );
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
line 62
;62:			if ( !token[0] ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $139
line 63
;63:				strcpy( token, "<NULL>" );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $141
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 64
;64:			}
LABELV $139
line 65
;65:			Info_SetValueForKey( info, key, token );
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
line 66
;66:		}
LABELV $131
line 50
ADDRGP4 $130
JUMPV
LABELV $132
line 68
;67:		//NOTE: extra space for arena number
;68:		infos[count] = UI_Alloc(strlen(info) + strlen("\\num\\") + strlen(va("%d", MAX_ARENAS)) + 1);
ADDRLP4 1028
ARGP4
ADDRLP4 2064
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRGP4 $142
ARGP4
ADDRLP4 2068
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRGP4 $143
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
ADDRGP4 UI_Alloc
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
line 69
;69:		if (infos[count]) {
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
EQU4 $144
line 70
;70:			strcpy(infos[count], info);
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
line 71
;71:			count++;
ADDRLP4 2052
ADDRLP4 2052
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 72
;72:		}
LABELV $144
line 73
;73:	}
LABELV $119
line 34
ADDRGP4 $118
JUMPV
LABELV $120
line 74
;74:	return count;
ADDRLP4 2052
INDIRI4
RETI4
LABELV $117
endproc UI_ParseInfos 2084 12
proc UI_LoadArenasFromFile 8216 16
line 82
;75:}
;76:
;77:/*
;78:===============
;79:UI_LoadArenasFromFile
;80:===============
;81:*/
;82:static void UI_LoadArenasFromFile( char *filename ) {
line 87
;83:	int				len;
;84:	fileHandle_t	f;
;85:	char			buf[MAX_ARENAS_TEXT];
;86:
;87:	len = trap_FS_FOpenFile( filename, &f, FS_READ );
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
line 88
;88:	if ( !f ) {
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $147
line 89
;89:		trap_Print( va( S_COLOR_RED "file not found: %s\n", filename ) );
ADDRGP4 $149
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
ADDRGP4 trap_Print
CALLV
pop
line 90
;90:		return;
ADDRGP4 $146
JUMPV
LABELV $147
line 92
;91:	}
;92:	if ( len >= MAX_ARENAS_TEXT ) {
ADDRLP4 0
INDIRI4
CNSTI4 8192
LTI4 $150
line 93
;93:		trap_Print( va( S_COLOR_RED "file too large: %s is %i, max allowed is %i", filename, len, MAX_ARENAS_TEXT ) );
ADDRGP4 $152
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
ADDRGP4 trap_Print
CALLV
pop
line 94
;94:		trap_FS_FCloseFile( f );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 95
;95:		return;
ADDRGP4 $146
JUMPV
LABELV $150
line 98
;96:	}
;97:
;98:	trap_FS_Read( buf, len, f );
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
line 99
;99:	buf[len] = 0;
ADDRLP4 0
INDIRI4
ADDRLP4 8
ADDP4
CNSTI1 0
ASGNI1
line 100
;100:	trap_FS_FCloseFile( f );
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 102
;101:
;102:	ui_numArenas += UI_ParseInfos( buf, MAX_ARENAS - ui_numArenas, &ui_arenaInfos[ui_numArenas] );
ADDRLP4 8
ARGP4
ADDRLP4 8204
ADDRGP4 ui_numArenas
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
ADDRGP4 ui_arenaInfos
ADDP4
ARGP4
ADDRLP4 8212
ADDRGP4 UI_ParseInfos
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
line 103
;103:}
LABELV $146
endproc UI_LoadArenasFromFile 8216 16
export UI_LoadArenas
proc UI_LoadArenas 1528 16
line 110
;104:
;105:/*
;106:===============
;107:UI_LoadArenas
;108:===============
;109:*/
;110:void UI_LoadArenas( void ) {
line 120
;111:	int			numdirs;
;112:	vmCvar_t	arenasFile;
;113:	char		filename[128];
;114:	char		dirlist[1024];
;115:	char*		dirptr;
;116:	int			i, n;
;117:	int			dirlen;
;118:	char		*type;
;119:
;120:	ui_numArenas = 0;
ADDRGP4 ui_numArenas
CNSTI4 0
ASGNI4
line 121
;121:	uiInfo.mapCount = 0;
ADDRGP4 uiInfo+20208
CNSTI4 0
ASGNI4
line 123
;122:
;123:	trap_Cvar_Register( &arenasFile, "g_arenasFile", "", CVAR_INIT|CVAR_ROM );
ADDRLP4 152
ARGP4
ADDRGP4 $155
ARGP4
ADDRGP4 $156
ARGP4
CNSTI4 80
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 124
;124:	if( *arenasFile.string ) {
ADDRLP4 152+16
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $157
line 125
;125:		UI_LoadArenasFromFile(arenasFile.string);
ADDRLP4 152+16
ARGP4
ADDRGP4 UI_LoadArenasFromFile
CALLV
pop
line 126
;126:	}
ADDRGP4 $158
JUMPV
LABELV $157
line 127
;127:	else {
line 128
;128:		UI_LoadArenasFromFile("scripts/arenas.txt");
ADDRGP4 $161
ARGP4
ADDRGP4 UI_LoadArenasFromFile
CALLV
pop
line 129
;129:	}
LABELV $158
line 132
;130:
;131:	// get all arenas from .arena files
;132:	numdirs = trap_FS_GetFileList("scripts", ".arena", dirlist, 1024 );
ADDRGP4 $162
ARGP4
ADDRGP4 $163
ARGP4
ADDRLP4 424
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 1448
ADDRGP4 trap_FS_GetFileList
CALLI4
ASGNI4
ADDRLP4 148
ADDRLP4 1448
INDIRI4
ASGNI4
line 133
;133:	dirptr  = dirlist;
ADDRLP4 8
ADDRLP4 424
ASGNP4
line 134
;134:	for (i = 0; i < numdirs; i++, dirptr += dirlen+1) {
ADDRLP4 140
CNSTI4 0
ASGNI4
ADDRGP4 $167
JUMPV
LABELV $164
line 135
;135:		dirlen = strlen(dirptr);
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 1452
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 144
ADDRLP4 1452
INDIRI4
ASGNI4
line 136
;136:		strcpy(filename, "scripts/");
ADDRLP4 12
ARGP4
ADDRGP4 $168
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 137
;137:		strcat(filename, dirptr);
ADDRLP4 12
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 138
;138:		UI_LoadArenasFromFile(filename);
ADDRLP4 12
ARGP4
ADDRGP4 UI_LoadArenasFromFile
CALLV
pop
line 139
;139:	}
LABELV $165
line 134
ADDRLP4 1452
CNSTI4 1
ASGNI4
ADDRLP4 140
ADDRLP4 140
INDIRI4
ADDRLP4 1452
INDIRI4
ADDI4
ASGNI4
ADDRLP4 8
ADDRLP4 144
INDIRI4
ADDRLP4 1452
INDIRI4
ADDI4
ADDRLP4 8
INDIRP4
ADDP4
ASGNP4
LABELV $167
ADDRLP4 140
INDIRI4
ADDRLP4 148
INDIRI4
LTI4 $164
line 140
;140:	trap_Print( va( "%i arenas parsed\n", ui_numArenas ) );
ADDRGP4 $169
ARGP4
ADDRGP4 ui_numArenas
INDIRI4
ARGI4
ADDRLP4 1456
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1456
INDIRP4
ARGP4
ADDRGP4 trap_Print
CALLV
pop
line 141
;141:	if (UI_OutOfMemory()) {
ADDRLP4 1460
ADDRGP4 UI_OutOfMemory
CALLI4
ASGNI4
ADDRLP4 1460
INDIRI4
CNSTI4 0
EQI4 $170
line 142
;142:		trap_Print(S_COLOR_YELLOW"WARNING: not anough memory in pool to load all arenas\n");
ADDRGP4 $172
ARGP4
ADDRGP4 trap_Print
CALLV
pop
line 143
;143:	}
LABELV $170
line 145
;144:
;145:	for( n = 0; n < ui_numArenas; n++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $176
JUMPV
LABELV $173
line 148
;146:		// determine type
;147:
;148:		uiInfo.mapList[uiInfo.mapCount].cinematic = -1;
CNSTI4 100
ADDRGP4 uiInfo+20208
INDIRI4
MULI4
ADDRGP4 uiInfo+20212+24
ADDP4
CNSTI4 -1
ASGNI4
line 149
;149:		uiInfo.mapList[uiInfo.mapCount].mapLoadName = String_Alloc(Info_ValueForKey(ui_arenaInfos[n], "map"));
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ui_arenaInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $183
ARGP4
ADDRLP4 1464
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1464
INDIRP4
ARGP4
ADDRLP4 1468
ADDRGP4 String_Alloc
CALLP4
ASGNP4
CNSTI4 100
ADDRGP4 uiInfo+20208
INDIRI4
MULI4
ADDRGP4 uiInfo+20212+4
ADDP4
ADDRLP4 1468
INDIRP4
ASGNP4
line 150
;150:		uiInfo.mapList[uiInfo.mapCount].mapName = String_Alloc(Info_ValueForKey(ui_arenaInfos[n], "longname"));
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ui_arenaInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $186
ARGP4
ADDRLP4 1472
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 1472
INDIRP4
ARGP4
ADDRLP4 1476
ADDRGP4 String_Alloc
CALLP4
ASGNP4
CNSTI4 100
ADDRGP4 uiInfo+20208
INDIRI4
MULI4
ADDRGP4 uiInfo+20212
ADDP4
ADDRLP4 1476
INDIRP4
ASGNP4
line 151
;151:		uiInfo.mapList[uiInfo.mapCount].levelShot = -1;
CNSTI4 100
ADDRGP4 uiInfo+20208
INDIRI4
MULI4
ADDRGP4 uiInfo+20212+92
ADDP4
CNSTI4 -1
ASGNI4
line 152
;152:		uiInfo.mapList[uiInfo.mapCount].imageName = String_Alloc(va("levelshots/%s", uiInfo.mapList[uiInfo.mapCount].mapLoadName));
ADDRGP4 $193
ARGP4
ADDRLP4 1480
CNSTI4 100
ASGNI4
ADDRLP4 1480
INDIRI4
ADDRGP4 uiInfo+20208
INDIRI4
MULI4
ADDRGP4 uiInfo+20212+4
ADDP4
INDIRP4
ARGP4
ADDRLP4 1484
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1484
INDIRP4
ARGP4
ADDRLP4 1488
ADDRGP4 String_Alloc
CALLP4
ASGNP4
ADDRLP4 1480
INDIRI4
ADDRGP4 uiInfo+20208
INDIRI4
MULI4
ADDRGP4 uiInfo+20212+8
ADDP4
ADDRLP4 1488
INDIRP4
ASGNP4
line 153
;153:		uiInfo.mapList[uiInfo.mapCount].typeBits = 0;
CNSTI4 100
ADDRGP4 uiInfo+20208
INDIRI4
MULI4
ADDRGP4 uiInfo+20212+20
ADDP4
CNSTI4 0
ASGNI4
line 155
;154:
;155:		type = Info_ValueForKey( ui_arenaInfos[n], "type" );
ADDRLP4 4
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ui_arenaInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $200
ARGP4
ADDRLP4 1492
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 1492
INDIRP4
ASGNP4
line 157
;156:		// if no type specified, it will be treated as "ffa"
;157:		if( *type ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $201
line 158
;158:			if( strstr( type, "ffa" ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $205
ARGP4
ADDRLP4 1496
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 1496
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $203
line 159
;159:				uiInfo.mapList[uiInfo.mapCount].typeBits |= (1 << GT_FFA);
ADDRLP4 1500
CNSTI4 100
ADDRGP4 uiInfo+20208
INDIRI4
MULI4
ADDRGP4 uiInfo+20212+20
ADDP4
ASGNP4
ADDRLP4 1500
INDIRP4
ADDRLP4 1500
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 160
;160:			}
LABELV $203
line 161
;161:			if( strstr( type, "holocron" ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $211
ARGP4
ADDRLP4 1500
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 1500
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $209
line 162
;162:				uiInfo.mapList[uiInfo.mapCount].typeBits |= (1 << GT_HOLOCRON);
ADDRLP4 1504
CNSTI4 100
ADDRGP4 uiInfo+20208
INDIRI4
MULI4
ADDRGP4 uiInfo+20212+20
ADDP4
ASGNP4
ADDRLP4 1504
INDIRP4
ADDRLP4 1504
INDIRP4
INDIRI4
CNSTI4 2
BORI4
ASGNI4
line 163
;163:			}
LABELV $209
line 164
;164:			if( strstr( type, "jedimaster" ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $217
ARGP4
ADDRLP4 1504
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 1504
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $215
line 165
;165:				uiInfo.mapList[uiInfo.mapCount].typeBits |= (1 << GT_JEDIMASTER);
ADDRLP4 1508
CNSTI4 100
ADDRGP4 uiInfo+20208
INDIRI4
MULI4
ADDRGP4 uiInfo+20212+20
ADDP4
ASGNP4
ADDRLP4 1508
INDIRP4
ADDRLP4 1508
INDIRP4
INDIRI4
CNSTI4 4
BORI4
ASGNI4
line 166
;166:			}
LABELV $215
line 167
;167:			if( strstr( type, "duel" ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $223
ARGP4
ADDRLP4 1508
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 1508
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $221
line 168
;168:				uiInfo.mapList[uiInfo.mapCount].typeBits |= (1 << GT_TOURNAMENT);
ADDRLP4 1512
CNSTI4 100
ADDRGP4 uiInfo+20208
INDIRI4
MULI4
ADDRGP4 uiInfo+20212+20
ADDP4
ASGNP4
ADDRLP4 1512
INDIRP4
ADDRLP4 1512
INDIRP4
INDIRI4
CNSTI4 8
BORI4
ASGNI4
line 169
;169:			}
LABELV $221
line 170
;170:			if( strstr( type, "saga" ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $229
ARGP4
ADDRLP4 1512
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 1512
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $227
line 171
;171:				uiInfo.mapList[uiInfo.mapCount].typeBits |= (1 << GT_SAGA);
ADDRLP4 1516
CNSTI4 100
ADDRGP4 uiInfo+20208
INDIRI4
MULI4
ADDRGP4 uiInfo+20212+20
ADDP4
ASGNP4
ADDRLP4 1516
INDIRP4
ADDRLP4 1516
INDIRP4
INDIRI4
CNSTI4 64
BORI4
ASGNI4
line 172
;172:			}
LABELV $227
line 173
;173:			if( strstr( type, "ctf" ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $235
ARGP4
ADDRLP4 1516
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 1516
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $233
line 174
;174:				uiInfo.mapList[uiInfo.mapCount].typeBits |= (1 << GT_CTF);
ADDRLP4 1520
CNSTI4 100
ADDRGP4 uiInfo+20208
INDIRI4
MULI4
ADDRGP4 uiInfo+20212+20
ADDP4
ASGNP4
ADDRLP4 1520
INDIRP4
ADDRLP4 1520
INDIRP4
INDIRI4
CNSTI4 128
BORI4
ASGNI4
line 175
;175:			}
LABELV $233
line 176
;176:			if( strstr( type, "cty" ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $241
ARGP4
ADDRLP4 1520
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 1520
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $202
line 177
;177:				uiInfo.mapList[uiInfo.mapCount].typeBits |= (1 << GT_CTY);
ADDRLP4 1524
CNSTI4 100
ADDRGP4 uiInfo+20208
INDIRI4
MULI4
ADDRGP4 uiInfo+20212+20
ADDP4
ASGNP4
ADDRLP4 1524
INDIRP4
ADDRLP4 1524
INDIRP4
INDIRI4
CNSTI4 256
BORI4
ASGNI4
line 178
;178:			}
line 179
;179:		} else {
ADDRGP4 $202
JUMPV
LABELV $201
line 180
;180:			uiInfo.mapList[uiInfo.mapCount].typeBits |= (1 << GT_FFA);
ADDRLP4 1496
CNSTI4 100
ADDRGP4 uiInfo+20208
INDIRI4
MULI4
ADDRGP4 uiInfo+20212+20
ADDP4
ASGNP4
ADDRLP4 1496
INDIRP4
ADDRLP4 1496
INDIRP4
INDIRI4
CNSTI4 1
BORI4
ASGNI4
line 181
;181:		}
LABELV $202
line 183
;182:
;183:		uiInfo.mapCount++;
ADDRLP4 1496
ADDRGP4 uiInfo+20208
ASGNP4
ADDRLP4 1496
INDIRP4
ADDRLP4 1496
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 184
;184:		if (uiInfo.mapCount >= MAX_MAPS) {
ADDRGP4 uiInfo+20208
INDIRI4
CNSTI4 128
LTI4 $249
line 185
;185:			break;
ADDRGP4 $175
JUMPV
LABELV $249
line 187
;186:		}
;187:	}
LABELV $174
line 145
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $176
ADDRLP4 4
INDIRI4
ADDRGP4 ui_numArenas
INDIRI4
LTI4 $173
LABELV $175
line 188
;188:}
LABELV $153
endproc UI_LoadArenas 1528 16
proc UI_LoadBotsFromFile 8224 16
line 196
;189:
;190:
;191:/*
;192:===============
;193:UI_LoadBotsFromFile
;194:===============
;195:*/
;196:static void UI_LoadBotsFromFile( char *filename ) {
line 202
;197:	int				len;
;198:	fileHandle_t	f;
;199:	char			buf[MAX_BOTS_TEXT];
;200:	char			*stopMark;
;201:
;202:	len = trap_FS_FOpenFile( filename, &f, FS_READ );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8196
ARGP4
CNSTI4 0
ARGI4
ADDRLP4 8204
ADDRGP4 trap_FS_FOpenFile
CALLI4
ASGNI4
ADDRLP4 8192
ADDRLP4 8204
INDIRI4
ASGNI4
line 203
;203:	if ( !f ) {
ADDRLP4 8196
INDIRI4
CNSTI4 0
NEI4 $253
line 204
;204:		trap_Print( va( S_COLOR_RED "file not found: %s\n", filename ) );
ADDRGP4 $149
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8208
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8208
INDIRP4
ARGP4
ADDRGP4 trap_Print
CALLV
pop
line 205
;205:		return;
ADDRGP4 $252
JUMPV
LABELV $253
line 207
;206:	}
;207:	if ( len >= MAX_BOTS_TEXT ) {
ADDRLP4 8192
INDIRI4
CNSTI4 8192
LTI4 $255
line 208
;208:		trap_Print( va( S_COLOR_RED "file too large: %s is %i, max allowed is %i", filename, len, MAX_BOTS_TEXT ) );
ADDRGP4 $152
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8192
INDIRI4
ARGI4
CNSTI4 8192
ARGI4
ADDRLP4 8208
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 8208
INDIRP4
ARGP4
ADDRGP4 trap_Print
CALLV
pop
line 209
;209:		trap_FS_FCloseFile( f );
ADDRLP4 8196
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 210
;210:		return;
ADDRGP4 $252
JUMPV
LABELV $255
line 213
;211:	}
;212:
;213:	trap_FS_Read( buf, len, f );
ADDRLP4 0
ARGP4
ADDRLP4 8192
INDIRI4
ARGI4
ADDRLP4 8196
INDIRI4
ARGI4
ADDRGP4 trap_FS_Read
CALLV
pop
line 214
;214:	buf[len] = 0;
ADDRLP4 8192
INDIRI4
ADDRLP4 0
ADDP4
CNSTI1 0
ASGNI1
line 216
;215:
;216:	stopMark = strstr(buf, "@STOPHERE");
ADDRLP4 0
ARGP4
ADDRGP4 $257
ARGP4
ADDRLP4 8208
ADDRGP4 strstr
CALLP4
ASGNP4
ADDRLP4 8200
ADDRLP4 8208
INDIRP4
ASGNP4
line 222
;217:
;218:	//This bot is in place as a mark for modview's bot viewer.
;219:	//If we hit it just stop and trace back to the beginning of the bot define and cut the string off.
;220:	//This is only done in the UI and not the game so that "test" bots can be added manually and still
;221:	//not show up in the menu.
;222:	if (stopMark)
ADDRLP4 8200
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $258
line 223
;223:	{
line 224
;224:		int startPoint = stopMark - buf;
ADDRLP4 8212
ADDRLP4 8200
INDIRP4
CVPU4 4
ADDRLP4 0
CVPU4 4
SUBU4
CVUI4 4
ASGNI4
ADDRGP4 $261
JUMPV
LABELV $260
line 227
;225:
;226:		while (buf[startPoint] != '{')
;227:		{
line 228
;228:			startPoint--;
ADDRLP4 8212
ADDRLP4 8212
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 229
;229:		}
LABELV $261
line 226
ADDRLP4 8212
INDIRI4
ADDRLP4 0
ADDP4
INDIRI1
CVII4 1
CNSTI4 123
NEI4 $260
line 231
;230:
;231:		buf[startPoint] = 0;
ADDRLP4 8212
INDIRI4
ADDRLP4 0
ADDP4
CNSTI1 0
ASGNI1
line 232
;232:	}
LABELV $258
line 234
;233:
;234:	trap_FS_FCloseFile( f );
ADDRLP4 8196
INDIRI4
ARGI4
ADDRGP4 trap_FS_FCloseFile
CALLV
pop
line 236
;235:
;236:	COM_Compress(buf);
ADDRLP4 0
ARGP4
ADDRGP4 COM_Compress
CALLI4
pop
line 238
;237:
;238:	ui_numBots += UI_ParseInfos( buf, MAX_BOTS - ui_numBots, &ui_botInfos[ui_numBots] );
ADDRLP4 0
ARGP4
ADDRLP4 8212
ADDRGP4 ui_numBots
ASGNP4
ADDRLP4 8216
ADDRLP4 8212
INDIRP4
INDIRI4
ASGNI4
CNSTI4 1024
ADDRLP4 8216
INDIRI4
SUBI4
ARGI4
ADDRLP4 8216
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ui_botInfos
ADDP4
ARGP4
ADDRLP4 8220
ADDRGP4 UI_ParseInfos
CALLI4
ASGNI4
ADDRLP4 8212
INDIRP4
ADDRLP4 8216
INDIRI4
ADDRLP4 8220
INDIRI4
ADDI4
ASGNI4
line 239
;239:}
LABELV $252
endproc UI_LoadBotsFromFile 8224 16
export UI_LoadBots
proc UI_LoadBots 1452 16
line 246
;240:
;241:/*
;242:===============
;243:UI_LoadBots
;244:===============
;245:*/
;246:void UI_LoadBots( void ) {
line 255
;247:	vmCvar_t	botsFile;
;248:	int			numdirs;
;249:	char		filename[128];
;250:	char		dirlist[1024];
;251:	char*		dirptr;
;252:	int			i;
;253:	int			dirlen;
;254:
;255:	ui_numBots = 0;
ADDRGP4 ui_numBots
CNSTI4 0
ASGNI4
line 257
;256:
;257:	trap_Cvar_Register( &botsFile, "g_botsFile", "", CVAR_INIT|CVAR_ROM );
ADDRLP4 144
ARGP4
ADDRGP4 $264
ARGP4
ADDRGP4 $156
ARGP4
CNSTI4 80
ARGI4
ADDRGP4 trap_Cvar_Register
CALLV
pop
line 258
;258:	if( *botsFile.string ) {
ADDRLP4 144+16
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $265
line 259
;259:		UI_LoadBotsFromFile(botsFile.string);
ADDRLP4 144+16
ARGP4
ADDRGP4 UI_LoadBotsFromFile
CALLV
pop
line 260
;260:	}
ADDRGP4 $266
JUMPV
LABELV $265
line 261
;261:	else {
line 262
;262:		UI_LoadBotsFromFile("botfiles/bots.txt");
ADDRGP4 $269
ARGP4
ADDRGP4 UI_LoadBotsFromFile
CALLV
pop
line 263
;263:	}
LABELV $266
line 266
;264:
;265:	// get all bots from .bot files
;266:	numdirs = trap_FS_GetFileList("scripts", ".bot", dirlist, 1024 );
ADDRGP4 $162
ARGP4
ADDRGP4 $270
ARGP4
ADDRLP4 416
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 1440
ADDRGP4 trap_FS_GetFileList
CALLI4
ASGNI4
ADDRLP4 140
ADDRLP4 1440
INDIRI4
ASGNI4
line 267
;267:	dirptr  = dirlist;
ADDRLP4 0
ADDRLP4 416
ASGNP4
line 268
;268:	for (i = 0; i < numdirs; i++, dirptr += dirlen+1) {
ADDRLP4 132
CNSTI4 0
ASGNI4
ADDRGP4 $274
JUMPV
LABELV $271
line 269
;269:		dirlen = strlen(dirptr);
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 1444
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 136
ADDRLP4 1444
INDIRI4
ASGNI4
line 270
;270:		strcpy(filename, "scripts/");
ADDRLP4 4
ARGP4
ADDRGP4 $168
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 271
;271:		strcat(filename, dirptr);
ADDRLP4 4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 272
;272:		UI_LoadBotsFromFile(filename);
ADDRLP4 4
ARGP4
ADDRGP4 UI_LoadBotsFromFile
CALLV
pop
line 273
;273:	}
LABELV $272
line 268
ADDRLP4 1444
CNSTI4 1
ASGNI4
ADDRLP4 132
ADDRLP4 132
INDIRI4
ADDRLP4 1444
INDIRI4
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 136
INDIRI4
ADDRLP4 1444
INDIRI4
ADDI4
ADDRLP4 0
INDIRP4
ADDP4
ASGNP4
LABELV $274
ADDRLP4 132
INDIRI4
ADDRLP4 140
INDIRI4
LTI4 $271
line 274
;274:	trap_Print( va( "%i bots parsed\n", ui_numBots ) );
ADDRGP4 $275
ARGP4
ADDRGP4 ui_numBots
INDIRI4
ARGI4
ADDRLP4 1448
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 1448
INDIRP4
ARGP4
ADDRGP4 trap_Print
CALLV
pop
line 275
;275:}
LABELV $263
endproc UI_LoadBots 1452 16
export UI_GetBotInfoByNumber
proc UI_GetBotInfoByNumber 8 8
line 283
;276:
;277:
;278:/*
;279:===============
;280:UI_GetBotInfoByNumber
;281:===============
;282:*/
;283:char *UI_GetBotInfoByNumber( int num ) {
line 284
;284:	if( num < 0 || num >= ui_numBots ) {
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
LTI4 $279
ADDRLP4 0
INDIRI4
ADDRGP4 ui_numBots
INDIRI4
LTI4 $277
LABELV $279
line 285
;285:		trap_Print( va( S_COLOR_RED "Invalid bot number: %i\n", num ) );
ADDRGP4 $280
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
ADDRGP4 trap_Print
CALLV
pop
line 286
;286:		return NULL;
CNSTP4 0
RETP4
ADDRGP4 $276
JUMPV
LABELV $277
line 288
;287:	}
;288:	return ui_botInfos[num];
ADDRFP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ui_botInfos
ADDP4
INDIRP4
RETP4
LABELV $276
endproc UI_GetBotInfoByNumber 8 8
export UI_GetBotInfoByName
proc UI_GetBotInfoByName 16 8
line 297
;289:}
;290:
;291:
;292:/*
;293:===============
;294:UI_GetBotInfoByName
;295:===============
;296:*/
;297:char *UI_GetBotInfoByName( const char *name ) {
line 301
;298:	int		n;
;299:	char	*value;
;300:
;301:	for ( n = 0; n < ui_numBots ; n++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $285
JUMPV
LABELV $282
line 302
;302:		value = Info_ValueForKey( ui_botInfos[n], "name" );
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ui_botInfos
ADDP4
INDIRP4
ARGP4
ADDRGP4 $286
ARGP4
ADDRLP4 8
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
ASGNP4
line 303
;303:		if ( !Q_stricmp( value, name ) ) {
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
NEI4 $287
line 304
;304:			return ui_botInfos[n];
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 ui_botInfos
ADDP4
INDIRP4
RETP4
ADDRGP4 $281
JUMPV
LABELV $287
line 306
;305:		}
;306:	}
LABELV $283
line 301
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $285
ADDRLP4 0
INDIRI4
ADDRGP4 ui_numBots
INDIRI4
LTI4 $282
line 308
;307:
;308:	return NULL;
CNSTP4 0
RETP4
LABELV $281
endproc UI_GetBotInfoByName 16 8
export UI_GetNumBots
proc UI_GetNumBots 0 0
line 311
;309:}
;310:
;311:int UI_GetNumBots() {
line 312
;312:	return ui_numBots;
ADDRGP4 ui_numBots
INDIRI4
RETI4
LABELV $289
endproc UI_GetNumBots 0 0
export UI_GetBotNameByNumber
proc UI_GetBotNameByNumber 12 8
line 316
;313:}
;314:
;315:
;316:char *UI_GetBotNameByNumber( int num ) {
line 317
;317:	char *info = UI_GetBotInfoByNumber(num);
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 UI_GetBotInfoByNumber
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 318
;318:	if (info) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $291
line 319
;319:		return Info_ValueForKey( info, "name" );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 $286
ARGP4
ADDRLP4 8
ADDRGP4 Info_ValueForKey
CALLP4
ASGNP4
ADDRLP4 8
INDIRP4
RETP4
ADDRGP4 $290
JUMPV
LABELV $291
line 321
;320:	}
;321:	return "Kyle";
ADDRGP4 $293
RETP4
LABELV $290
endproc UI_GetBotNameByNumber 12 8
bss
align 4
LABELV ui_arenaInfos
skip 4096
align 4
LABELV ui_numArenas
skip 4
align 4
LABELV ui_botInfos
skip 4096
export ui_numBots
align 4
LABELV ui_numBots
skip 4
import UI_RankStatusMenu
import RankStatus_Cache
import UI_SignupMenu
import Signup_Cache
import UI_LoginMenu
import Login_Cache
import UI_InitGameinfo
import UI_SPUnlockMedals_f
import UI_SPUnlock_f
import UI_GetAwardLevel
import UI_LogAwardData
import UI_NewGame
import UI_GetCurrentGame
import UI_CanShowTierVideo
import UI_ShowTierVideo
import UI_TierCompleted
import UI_SetBestScore
import UI_GetBestScore
import UI_GetNumSPTiers
import UI_GetNumSPArenas
import UI_GetNumArenas
import UI_GetSpecialArenaInfo
import UI_GetArenaInfoByMap
import UI_GetArenaInfoByNumber
import UI_NetworkOptionsMenu
import UI_NetworkOptionsMenu_Cache
import UI_SoundOptionsMenu
import UI_SoundOptionsMenu_Cache
import UI_DisplayOptionsMenu
import UI_DisplayOptionsMenu_Cache
import UI_SaveConfigMenu
import UI_SaveConfigMenu_Cache
import UI_LoadConfigMenu
import UI_LoadConfig_Cache
import UI_TeamOrdersMenu_Cache
import UI_TeamOrdersMenu_f
import UI_TeamOrdersMenu
import UI_RemoveBotsMenu
import UI_RemoveBots_Cache
import UI_AddBotsMenu
import UI_AddBots_Cache
import trap_G2API_SetBoneAngles
import trap_R_RemapShader
import trap_RealTime
import trap_CIN_SetExtents
import trap_CIN_DrawCinematic
import trap_CIN_RunCinematic
import trap_CIN_StopCinematic
import trap_CIN_PlayCinematic
import trap_S_StartBackgroundTrack
import trap_S_StopBackgroundTrack
import trap_AnyLanguage_ReadCharFromString
import trap_R_Font_DrawString
import trap_R_Font_HeightPixels
import trap_R_Font_StrLenChars
import trap_R_Font_StrLenPixels
import trap_R_RegisterFont
import trap_MemoryRemaining
import trap_LAN_CompareServers
import trap_LAN_ServerStatus
import trap_LAN_ResetPings
import trap_LAN_RemoveServer
import trap_LAN_AddServer
import trap_LAN_UpdateVisiblePings
import trap_LAN_ServerIsVisible
import trap_LAN_MarkServerVisible
import trap_LAN_SaveCachedServers
import trap_LAN_LoadCachedServers
import trap_LAN_GetPingInfo
import trap_LAN_GetPing
import trap_LAN_ClearPing
import trap_LAN_GetPingQueueCount
import trap_LAN_GetServerPing
import trap_LAN_GetServerInfo
import trap_LAN_GetServerAddressString
import trap_LAN_GetServerCount
import trap_GetConfigString
import trap_GetGlconfig
import trap_GetClientState
import trap_GetClipboardData
import trap_Key_SetCatcher
import trap_Key_GetCatcher
import trap_Key_ClearStates
import trap_Key_SetOverstrikeMode
import trap_Key_GetOverstrikeMode
import trap_Key_IsDown
import trap_Key_SetBinding
import trap_Key_GetBindingBuf
import trap_Key_KeynumToStringBuf
import trap_S_RegisterSound
import trap_S_StartLocalSound
import trap_CM_LerpTag
import trap_UpdateScreen
import trap_R_ModelBounds
import trap_R_DrawStretchPic
import trap_R_SetColor
import trap_R_RenderScene
import trap_R_AddLightToScene
import trap_R_AddPolyToScene
import trap_R_AddRefEntityToScene
import trap_R_ClearScene
import trap_R_RegisterShaderNoMip
import trap_R_RegisterSkin
import trap_R_RegisterModel
import trap_FS_GetFileList
import trap_FS_FCloseFile
import trap_FS_Write
import trap_FS_Read
import trap_FS_FOpenFile
import trap_Cmd_ExecuteText
import trap_Argv
import trap_Argc
import trap_Cvar_InfoStringBuffer
import trap_Cvar_Create
import trap_Cvar_Reset
import trap_Cvar_SetValue
import trap_Cvar_VariableStringBuffer
import trap_Cvar_VariableValue
import trap_Cvar_Set
import trap_Cvar_Update
import trap_Cvar_Register
import trap_Milliseconds
import trap_Error
import trap_Print
import UI_SPSkillMenu_Cache
import UI_SPSkillMenu
import UI_SPPostgameMenu_f
import UI_SPPostgameMenu_Cache
import UI_SPArena_Start
import UI_SPLevelMenu_ReInit
import UI_SPLevelMenu_f
import UI_SPLevelMenu
import UI_SPLevelMenu_Cache
import uis
import UI_LoadBestScores
import m_entersound
import UI_StartDemoLoop
import UI_Cvar_VariableString
import UI_Argv
import UI_ForceMenuOff
import UI_PopMenu
import UI_PushMenu
import UI_SetActiveMenu
import UI_IsFullscreen
import UI_DrawTextBox
import UI_CursorInRect
import UI_DrawChar
import UI_DrawString
import UI_ProportionalStringWidth
import UI_DrawProportionalString
import UI_ProportionalSizeScale
import UI_DrawBannerString
import UI_LerpColor
import UI_SetColor
import UI_UpdateScreen
import UI_DrawSides
import UI_DrawTopBottom
import UI_DrawRect
import UI_FillRect
import UI_DrawHandlePic
import UI_DrawNamedPic
import UI_ClampCvar
import UI_ConsoleCommand
import UI_Refresh
import UI_MouseEvent
import UI_KeyEvent
import UI_Shutdown
import UI_Init
import uiInfo
import DriverInfo_Cache
import GraphicsOptions_Cache
import UI_GraphicsOptionsMenu
import ServerInfo_Cache
import UI_ServerInfoMenu
import UI_BotSelectMenu_Cache
import UI_BotSelectMenu
import ServerOptions_Cache
import StartServer_Cache
import UI_StartServerMenu
import ArenaServers_Cache
import UI_ArenaServersMenu
import SpecifyServer_Cache
import UI_SpecifyServerMenu
import SpecifyLeague_Cache
import UI_SpecifyLeagueMenu
import Preferences_Cache
import UI_PreferencesMenu
import PlayerSettings_Cache
import UI_PlayerSettingsMenu
import PlayerModel_Cache
import UI_PlayerModelMenu
import UI_CDKeyMenu_f
import UI_CDKeyMenu_Cache
import UI_CDKeyMenu
import UI_ModsMenu_Cache
import UI_ModsMenu
import UI_CinematicsMenu_Cache
import UI_CinematicsMenu_f
import UI_CinematicsMenu
import Demos_Cache
import UI_DemosMenu
import Controls_Cache
import UI_ControlsMenu
import UI_DrawConnectScreen
import TeamMain_Cache
import UI_TeamMainMenu
import UI_SetupMenu
import UI_SetupMenu_Cache
import UI_ConfirmMenu
import ConfirmMenu_Cache
import UI_InGameMenu
import InGame_Cache
import UI_CreditMenu
import UI_UpdateCvars
import UI_RegisterCvars
import UI_MainMenu
import MainMenu_Cache
import UI_LoadForceConfig_List
import UI_ClearScores
import UI_ShowPostGame
import UI_AdjustTimeByGame
import _UI_SetActiveMenu
import UI_LoadMenus
import UI_Load
import UI_Report
import UI_FeederSelection
import MenuField_Key
import MenuField_Draw
import MenuField_Init
import MField_Draw
import MField_CharEvent
import MField_KeyDownEvent
import MField_Clear
import ui_medalSounds
import ui_medalPicNames
import ui_medalNames
import text_color_highlight
import text_color_normal
import text_color_disabled
import listbar_color
import list_color
import name_color
import color_dim
import color_red
import color_orange
import color_blue
import color_yellow
import color_white
import color_black
import menu_dim_color
import menu_black_color
import menu_red_color
import menu_highlight_color
import menu_dark_color
import menu_grayed_color
import menu_text_color
import weaponChangeSound
import menu_null_sound
import menu_buzz_sound
import menu_out_sound
import menu_move_sound
import menu_in_sound
import ScrollList_Key
import ScrollList_Draw
import Bitmap_Draw
import Bitmap_Init
import Menu_DefaultKey
import Menu_SetCursorToItem
import Menu_SetCursor
import Menu_ActivateItem
import Menu_ItemAtCursor
import Menu_Draw
import Menu_AdjustCursor
import Menu_AddItem
import Menu_Focus
import Menu_Cache
import ui_serverStatusTimeOut
import ui_bigFont
import ui_smallFont
import ui_scoreTime
import ui_scoreShutoutBonus
import ui_scoreSkillBonus
import ui_scoreTimeBonus
import ui_scoreBase
import ui_scoreTeam
import ui_scorePerfect
import ui_scoreScore
import ui_scoreGauntlets
import ui_scoreAssists
import ui_scoreDefends
import ui_scoreExcellents
import ui_scoreImpressives
import ui_scoreAccuracy
import ui_singlePlayerActive
import ui_lastServerRefresh_3
import ui_lastServerRefresh_2
import ui_lastServerRefresh_1
import ui_lastServerRefresh_0
import ui_selectedPlayerName
import ui_selectedPlayer
import ui_currentOpponent
import ui_mapIndex
import ui_currentNetMap
import ui_currentMap
import ui_currentTier
import ui_menuFiles
import ui_opponentName
import ui_dedicated
import ui_serverFilterType
import ui_netSource
import ui_joinGameType
import ui_actualNetGameType
import ui_netGameType
import ui_gameType
import ui_fragLimit
import ui_captureLimit
import ui_cdkeychecked
import ui_cdkey
import ui_server16
import ui_server15
import ui_server14
import ui_server13
import ui_server12
import ui_server11
import ui_server10
import ui_server9
import ui_server8
import ui_server7
import ui_server6
import ui_server5
import ui_server4
import ui_server3
import ui_server2
import ui_server1
import ui_marks
import ui_drawCrosshairNames
import ui_drawCrosshair
import ui_browserShowEmpty
import ui_browserShowFull
import ui_browserSortKey
import ui_browserGameType
import ui_browserMaster
import ui_spSelection
import ui_spSkill
import ui_spVideos
import ui_spAwards
import ui_spScores5
import ui_spScores4
import ui_spScores3
import ui_spScores2
import ui_spScores1
import ui_botsFile
import ui_arenasFile
import ui_ctf_friendly
import ui_ctf_timelimit
import ui_ctf_capturelimit
import ui_team_friendly
import ui_team_timelimit
import ui_team_fraglimit
import ui_selectedModelIndex
import ui_tourney_timelimit
import ui_tourney_fraglimit
import ui_ffa_timelimit
import ui_ffa_fraglimit
import trap_SP_GetStringTextString
import trap_SP_Register
import trap_SP_RegisterServer
import trap_PC_RemoveAllGlobalDefines
import trap_PC_LoadGlobalDefines
import trap_PC_SourceFileAndLine
import trap_PC_ReadToken
import trap_PC_FreeSource
import trap_PC_LoadSource
import trap_PC_AddGlobalDefine
import Controls_SetDefaults
import Controls_SetConfig
import Controls_GetConfig
import UI_OutOfMemory
import UI_InitMemory
import UI_Alloc
import Display_CacheAll
import Menu_SetFeederSelection
import Menu_Paint
import Menus_CloseAll
import LerpColor
import Display_HandleKey
import Menus_CloseByName
import Menus_ShowByName
import Menus_FindByName
import Menus_OpenByName
import Display_KeyBindPending
import Display_CursorType
import Display_MouseMove
import Display_CaptureItem
import Display_GetContext
import Menus_Activate
import Menus_AnyFullScreenVisible
import Menu_Reset
import Menus_ActivateByName
import Menu_PaintAll
import Menu_New
import Menu_Count
import PC_Script_Parse
import PC_String_Parse
import PC_Rect_Parse
import PC_Int_Parse
import PC_Color_Parse
import PC_Float_Parse
import Script_Parse
import String_Parse
import Rect_Parse
import Int_Parse
import Color_Parse
import Float_Parse
import Menu_ScrollFeeder
import Menu_HandleMouseMove
import Menu_HandleKey
import Menu_GetFocused
import Menu_PostParse
import Item_Init
import Menu_Init
import Display_ExpandMacros
import Init_Display
import String_Report
import String_Init
import String_Alloc
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
LABELV $293
char 1 75
char 1 121
char 1 108
char 1 101
char 1 0
align 1
LABELV $286
char 1 110
char 1 97
char 1 109
char 1 101
char 1 0
align 1
LABELV $280
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
LABELV $275
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
LABELV $270
char 1 46
char 1 98
char 1 111
char 1 116
char 1 0
align 1
LABELV $269
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
LABELV $264
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
LABELV $257
char 1 64
char 1 83
char 1 84
char 1 79
char 1 80
char 1 72
char 1 69
char 1 82
char 1 69
char 1 0
align 1
LABELV $241
char 1 99
char 1 116
char 1 121
char 1 0
align 1
LABELV $235
char 1 99
char 1 116
char 1 102
char 1 0
align 1
LABELV $229
char 1 115
char 1 97
char 1 103
char 1 97
char 1 0
align 1
LABELV $223
char 1 100
char 1 117
char 1 101
char 1 108
char 1 0
align 1
LABELV $217
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
LABELV $211
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
LABELV $205
char 1 102
char 1 102
char 1 97
char 1 0
align 1
LABELV $200
char 1 116
char 1 121
char 1 112
char 1 101
char 1 0
align 1
LABELV $193
char 1 108
char 1 101
char 1 118
char 1 101
char 1 108
char 1 115
char 1 104
char 1 111
char 1 116
char 1 115
char 1 47
char 1 37
char 1 115
char 1 0
align 1
LABELV $186
char 1 108
char 1 111
char 1 110
char 1 103
char 1 110
char 1 97
char 1 109
char 1 101
char 1 0
align 1
LABELV $183
char 1 109
char 1 97
char 1 112
char 1 0
align 1
LABELV $172
char 1 94
char 1 51
char 1 87
char 1 65
char 1 82
char 1 78
char 1 73
char 1 78
char 1 71
char 1 58
char 1 32
char 1 110
char 1 111
char 1 116
char 1 32
char 1 97
char 1 110
char 1 111
char 1 117
char 1 103
char 1 104
char 1 32
char 1 109
char 1 101
char 1 109
char 1 111
char 1 114
char 1 121
char 1 32
char 1 105
char 1 110
char 1 32
char 1 112
char 1 111
char 1 111
char 1 108
char 1 32
char 1 116
char 1 111
char 1 32
char 1 108
char 1 111
char 1 97
char 1 100
char 1 32
char 1 97
char 1 108
char 1 108
char 1 32
char 1 97
char 1 114
char 1 101
char 1 110
char 1 97
char 1 115
char 1 10
char 1 0
align 1
LABELV $169
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
LABELV $168
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
LABELV $163
char 1 46
char 1 97
char 1 114
char 1 101
char 1 110
char 1 97
char 1 0
align 1
LABELV $162
char 1 115
char 1 99
char 1 114
char 1 105
char 1 112
char 1 116
char 1 115
char 1 0
align 1
LABELV $161
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
LABELV $156
char 1 0
align 1
LABELV $155
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
LABELV $152
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
LABELV $149
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
LABELV $143
char 1 37
char 1 100
char 1 0
align 1
LABELV $142
char 1 92
char 1 110
char 1 117
char 1 109
char 1 92
char 1 0
align 1
LABELV $141
char 1 60
char 1 78
char 1 85
char 1 76
char 1 76
char 1 62
char 1 0
align 1
LABELV $138
char 1 125
char 1 0
align 1
LABELV $135
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
LABELV $129
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
LABELV $126
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
LABELV $125
char 1 123
char 1 0
