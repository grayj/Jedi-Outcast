data
export remapCount
align 4
LABELV remapCount
byte 4 0
export AddRemap
code
proc AddRemap 8 8
file "../g_utils.c"
line 18
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:// g_utils.c -- misc utility functions for game module
;4:
;5:#include "g_local.h"
;6:
;7:typedef struct {
;8:  char oldShader[MAX_QPATH];
;9:  char newShader[MAX_QPATH];
;10:  float timeOffset;
;11:} shaderRemap_t;
;12:
;13:#define MAX_SHADER_REMAPS 128
;14:
;15:int remapCount = 0;
;16:shaderRemap_t remappedShaders[MAX_SHADER_REMAPS];
;17:
;18:void AddRemap(const char *oldShader, const char *newShader, float timeOffset) {
line 21
;19:	int i;
;20:
;21:	for (i = 0; i < remapCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $83
JUMPV
LABELV $80
line 22
;22:		if (Q_stricmp(oldShader, remappedShaders[i].oldShader) == 0) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 132
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 remappedShaders
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $84
line 24
;23:			// found it, just update this one
;24:			strcpy(remappedShaders[i].newShader,newShader);
CNSTI4 132
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 remappedShaders+64
ADDP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 25
;25:			remappedShaders[i].timeOffset = timeOffset;
CNSTI4 132
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 remappedShaders+128
ADDP4
ADDRFP4 8
INDIRF4
ASGNF4
line 26
;26:			return;
ADDRGP4 $79
JUMPV
LABELV $84
line 28
;27:		}
;28:	}
LABELV $81
line 21
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $83
ADDRLP4 0
INDIRI4
ADDRGP4 remapCount
INDIRI4
LTI4 $80
line 29
;29:	if (remapCount < MAX_SHADER_REMAPS) {
ADDRGP4 remapCount
INDIRI4
CNSTI4 128
GEI4 $88
line 30
;30:		strcpy(remappedShaders[remapCount].newShader,newShader);
CNSTI4 132
ADDRGP4 remapCount
INDIRI4
MULI4
ADDRGP4 remappedShaders+64
ADDP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 31
;31:		strcpy(remappedShaders[remapCount].oldShader,oldShader);
CNSTI4 132
ADDRGP4 remapCount
INDIRI4
MULI4
ADDRGP4 remappedShaders
ADDP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 32
;32:		remappedShaders[remapCount].timeOffset = timeOffset;
CNSTI4 132
ADDRGP4 remapCount
INDIRI4
MULI4
ADDRGP4 remappedShaders+128
ADDP4
ADDRFP4 8
INDIRF4
ASGNF4
line 33
;33:		remapCount++;
ADDRLP4 4
ADDRGP4 remapCount
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 34
;34:	}
LABELV $88
line 35
;35:}
LABELV $79
endproc AddRemap 8 8
bss
align 1
LABELV $93
skip 4096
export BuildShaderStateConfig
code
proc BuildShaderStateConfig 144 24
line 37
;36:
;37:const char *BuildShaderStateConfig(void) {
line 42
;38:	static char	buff[MAX_STRING_CHARS*4];
;39:	char out[(MAX_QPATH * 2) + 5];
;40:	int i;
;41:  
;42:	memset(buff, 0, MAX_STRING_CHARS);
ADDRGP4 $93
ARGP4
CNSTI4 0
ARGI4
CNSTI4 1024
ARGI4
ADDRGP4 memset
CALLP4
pop
line 43
;43:	for (i = 0; i < remapCount; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $97
JUMPV
LABELV $94
line 44
;44:		Com_sprintf(out, (MAX_QPATH * 2) + 5, "%s=%s:%5.2f@", remappedShaders[i].oldShader, remappedShaders[i].newShader, remappedShaders[i].timeOffset);
ADDRLP4 4
ARGP4
CNSTI4 133
ARGI4
ADDRGP4 $98
ARGP4
ADDRLP4 140
CNSTI4 132
ADDRLP4 0
INDIRI4
MULI4
ASGNI4
ADDRLP4 140
INDIRI4
ADDRGP4 remappedShaders
ADDP4
ARGP4
ADDRLP4 140
INDIRI4
ADDRGP4 remappedShaders+64
ADDP4
ARGP4
ADDRLP4 140
INDIRI4
ADDRGP4 remappedShaders+128
ADDP4
INDIRF4
ARGF4
ADDRGP4 Com_sprintf
CALLV
pop
line 45
;45:		Q_strcat( buff, sizeof( buff ), out);
ADDRGP4 $93
ARGP4
CNSTI4 4096
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
line 46
;46:	}
LABELV $95
line 43
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $97
ADDRLP4 0
INDIRI4
ADDRGP4 remapCount
INDIRI4
LTI4 $94
line 47
;47:	return buff;
ADDRGP4 $93
RETP4
LABELV $92
endproc BuildShaderStateConfig 144 24
export G_FindConfigstringIndex
proc G_FindConfigstringIndex 1036 12
line 64
;48:}
;49:
;50:/*
;51:=========================================================================
;52:
;53:model / sound configstring indexes
;54:
;55:=========================================================================
;56:*/
;57:
;58:/*
;59:================
;60:G_FindConfigstringIndex
;61:
;62:================
;63:*/
;64:int G_FindConfigstringIndex( char *name, int start, int max, qboolean create ) {
line 68
;65:	int		i;
;66:	char	s[MAX_STRING_CHARS];
;67:
;68:	if ( !name || !name[0] ) {
ADDRLP4 1028
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1028
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $104
ADDRLP4 1028
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $102
LABELV $104
line 69
;69:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $101
JUMPV
LABELV $102
line 72
;70:	}
;71:
;72:	for ( i=1 ; i<max ; i++ ) {
ADDRLP4 1024
CNSTI4 1
ASGNI4
ADDRGP4 $108
JUMPV
LABELV $105
line 73
;73:		trap_GetConfigstring( start + i, s, sizeof( s ) );
ADDRFP4 4
INDIRI4
ADDRLP4 1024
INDIRI4
ADDI4
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_GetConfigstring
CALLV
pop
line 74
;74:		if ( !s[0] ) {
ADDRLP4 0
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $109
line 75
;75:			break;
ADDRGP4 $107
JUMPV
LABELV $109
line 77
;76:		}
;77:		if ( !strcmp( s, name ) ) {
ADDRLP4 0
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1032
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 1032
INDIRI4
CNSTI4 0
NEI4 $111
line 78
;78:			return i;
ADDRLP4 1024
INDIRI4
RETI4
ADDRGP4 $101
JUMPV
LABELV $111
line 80
;79:		}
;80:	}
LABELV $106
line 72
ADDRLP4 1024
ADDRLP4 1024
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $108
ADDRLP4 1024
INDIRI4
ADDRFP4 8
INDIRI4
LTI4 $105
LABELV $107
line 82
;81:
;82:	if ( !create ) {
ADDRFP4 12
INDIRI4
CNSTI4 0
NEI4 $113
line 83
;83:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $101
JUMPV
LABELV $113
line 86
;84:	}
;85:
;86:	if ( i == max ) {
ADDRLP4 1024
INDIRI4
ADDRFP4 8
INDIRI4
NEI4 $115
line 87
;87:		G_Error( "G_FindConfigstringIndex: overflow" );
ADDRGP4 $117
ARGP4
ADDRGP4 G_Error
CALLV
pop
line 88
;88:	}
LABELV $115
line 90
;89:
;90:	trap_SetConfigstring( start + i, name );
ADDRFP4 4
INDIRI4
ADDRLP4 1024
INDIRI4
ADDI4
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 92
;91:
;92:	return i;
ADDRLP4 1024
INDIRI4
RETI4
LABELV $101
endproc G_FindConfigstringIndex 1036 12
export G_SkinIndex
proc G_SkinIndex 4 16
line 99
;93:}
;94:
;95:/*
;96:Ghoul2 Insert Start
;97:*/
;98:
;99:int G_SkinIndex( char *name ) {
line 100
;100:	return G_FindConfigstringIndex (name, CS_CHARSKINS, MAX_CHARSKINS, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 576
ARGI4
CNSTI4 64
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 0
ADDRGP4 G_FindConfigstringIndex
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
RETI4
LABELV $118
endproc G_SkinIndex 4 16
export G_ModelIndex
proc G_ModelIndex 4 16
line 106
;101:}
;102:/*
;103:Ghoul2 Insert End
;104:*/
;105:
;106:int G_ModelIndex( char *name ) {
line 107
;107:	return G_FindConfigstringIndex (name, CS_MODELS, MAX_MODELS, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 32
ARGI4
CNSTI4 256
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 0
ADDRGP4 G_FindConfigstringIndex
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
RETI4
LABELV $119
endproc G_ModelIndex 4 16
export G_SoundIndex
proc G_SoundIndex 4 16
line 110
;108:}
;109:
;110:int G_SoundIndex( char *name ) {
line 111
;111:	return G_FindConfigstringIndex (name, CS_SOUNDS, MAX_SOUNDS, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 288
ARGI4
CNSTI4 256
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 0
ADDRGP4 G_FindConfigstringIndex
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
RETI4
LABELV $120
endproc G_SoundIndex 4 16
export G_EffectIndex
proc G_EffectIndex 4 16
line 115
;112:}
;113:
;114:int G_EffectIndex( char *name )
;115:{
line 116
;116:	return G_FindConfigstringIndex (name, CS_EFFECTS, MAX_FX, qtrue);
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 768
ARGI4
CNSTI4 64
ARGI4
CNSTI4 1
ARGI4
ADDRLP4 0
ADDRGP4 G_FindConfigstringIndex
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
RETI4
LABELV $121
endproc G_EffectIndex 4 16
export G_TeamCommand
proc G_TeamCommand 8 8
line 129
;117:}
;118:
;119://=====================================================================
;120:
;121:
;122:/*
;123:================
;124:G_TeamCommand
;125:
;126:Broadcasts a command to only a specific team
;127:================
;128:*/
;129:void G_TeamCommand( team_t team, char *cmd ) {
line 132
;130:	int		i;
;131:
;132:	for ( i = 0 ; i < level.maxclients ; i++ ) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $126
JUMPV
LABELV $123
line 133
;133:		if ( level.clients[i].pers.connected == CON_CONNECTED ) {
CNSTI4 1756
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 1368
ADDP4
INDIRI4
CNSTI4 2
NEI4 $128
line 134
;134:			if ( level.clients[i].sess.sessionTeam == team ) {
CNSTI4 1756
ADDRLP4 0
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
CNSTI4 1520
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $130
line 135
;135:				trap_SendServerCommand( i, va("%s", cmd ));
ADDRGP4 $132
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 136
;136:			}
LABELV $130
line 137
;137:		}
LABELV $128
line 138
;138:	}
LABELV $124
line 132
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $126
ADDRLP4 0
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $123
line 139
;139:}
LABELV $122
endproc G_TeamCommand 8 8
export G_Find
proc G_Find 8 8
line 155
;140:
;141:
;142:/*
;143:=============
;144:G_Find
;145:
;146:Searches all active entities for the next one that holds
;147:the matching string at fieldofs (use the FOFS() macro) in the structure.
;148:
;149:Searches beginning at the entity after from, or the beginning if NULL
;150:NULL will be returned if the end of the list is reached.
;151:
;152:=============
;153:*/
;154:gentity_t *G_Find (gentity_t *from, int fieldofs, const char *match)
;155:{
line 158
;156:	char	*s;
;157:
;158:	if (!from)
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $134
line 159
;159:		from = g_entities;
ADDRFP4 0
ADDRGP4 g_entities
ASGNP4
ADDRGP4 $139
JUMPV
LABELV $134
line 161
;160:	else
;161:		from++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 828
ADDP4
ASGNP4
line 163
;162:
;163:	for ( ; from < &g_entities[level.num_entities] ; from++)
ADDRGP4 $139
JUMPV
LABELV $136
line 164
;164:	{
line 165
;165:		if (!from->inuse)
ADDRFP4 0
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
NEI4 $141
line 166
;166:			continue;
ADDRGP4 $137
JUMPV
LABELV $141
line 167
;167:		s = *(char **) ((byte *)from + fieldofs);
ADDRLP4 0
ADDRFP4 4
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRP4
ASGNP4
line 168
;168:		if (!s)
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $143
line 169
;169:			continue;
ADDRGP4 $137
JUMPV
LABELV $143
line 170
;170:		if (!Q_stricmp (s, match))
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $145
line 171
;171:			return from;
ADDRFP4 0
INDIRP4
RETP4
ADDRGP4 $133
JUMPV
LABELV $145
line 172
;172:	}
LABELV $137
line 163
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 828
ADDP4
ASGNP4
LABELV $139
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTI4 828
ADDRGP4 level+12
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
CVPU4 4
LTU4 $136
line 174
;173:
;174:	return NULL;
CNSTP4 0
RETP4
LABELV $133
endproc G_Find 8 8
export G_RadiusList
proc G_RadiusList 4176 16
line 185
;175:}
;176:
;177:
;178:
;179:/*
;180:============
;181:G_RadiusList - given an origin and a radius, return all entities that are in use that are within the list
;182:============
;183:*/
;184:int G_RadiusList ( vec3_t origin, float radius,	gentity_t *ignore, qboolean takeDamage, gentity_t *ent_list[MAX_GENTITIES])					  
;185:{
line 193
;186:	float		dist;
;187:	gentity_t	*ent;
;188:	int			entityList[MAX_GENTITIES];
;189:	int			numListedEntities;
;190:	vec3_t		mins, maxs;
;191:	vec3_t		v;
;192:	int			i, e;
;193:	int			ent_count = 0;
ADDRLP4 24
CNSTI4 0
ASGNI4
line 195
;194:
;195:	if ( radius < 1 ) 
ADDRFP4 4
INDIRF4
CNSTF4 1065353216
GEF4 $148
line 196
;196:	{
line 197
;197:		radius = 1;
ADDRFP4 4
CNSTF4 1065353216
ASGNF4
line 198
;198:	}
LABELV $148
line 200
;199:
;200:	for ( i = 0 ; i < 3 ; i++ ) 
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $150
line 201
;201:	{
line 202
;202:		mins[i] = origin[i] - radius;
ADDRLP4 4156
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4156
INDIRI4
ADDRLP4 4132
ADDP4
ADDRLP4 4156
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRFP4 4
INDIRF4
SUBF4
ASGNF4
line 203
;203:		maxs[i] = origin[i] + radius;
ADDRLP4 4160
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4160
INDIRI4
ADDRLP4 4144
ADDP4
ADDRLP4 4160
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRFP4 4
INDIRF4
ADDF4
ASGNF4
line 204
;204:	}
LABELV $151
line 200
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $150
line 206
;205:
;206:	numListedEntities = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );
ADDRLP4 4132
ARGP4
ADDRLP4 4144
ARGP4
ADDRLP4 32
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 4156
ADDRGP4 trap_EntitiesInBox
CALLI4
ASGNI4
ADDRLP4 4128
ADDRLP4 4156
INDIRI4
ASGNI4
line 208
;207:
;208:	for ( e = 0 ; e < numListedEntities ; e++ ) 
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRGP4 $157
JUMPV
LABELV $154
line 209
;209:	{
line 210
;210:		ent = &g_entities[entityList[ e ]];
ADDRLP4 4
CNSTI4 828
ADDRLP4 20
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 32
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 212
;211:
;212:		if ((ent == ignore) || !(ent->inuse) || ent->takedamage != takeDamage)
ADDRLP4 4
INDIRP4
CVPU4 4
ADDRFP4 8
INDIRP4
CVPU4 4
EQU4 $161
ADDRLP4 4
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $161
ADDRLP4 4
INDIRP4
CNSTI4 680
ADDP4
INDIRI4
ADDRFP4 12
INDIRI4
EQI4 $158
LABELV $161
line 213
;213:			continue;
ADDRGP4 $155
JUMPV
LABELV $158
line 216
;214:
;215:		// find the distance from the edge of the bounding box
;216:		for ( i = 0 ; i < 3 ; i++ ) 
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $162
line 217
;217:		{
line 218
;218:			if ( origin[i] < ent->r.absmin[i] ) 
ADDRLP4 4164
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4164
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRLP4 4164
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 344
ADDP4
ADDP4
INDIRF4
GEF4 $166
line 219
;219:			{
line 220
;220:				v[i] = ent->r.absmin[i] - origin[i];
ADDRLP4 4168
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4168
INDIRI4
ADDRLP4 8
ADDP4
ADDRLP4 4168
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 344
ADDP4
ADDP4
INDIRF4
ADDRLP4 4168
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 221
;221:			} else if ( origin[i] > ent->r.absmax[i] ) 
ADDRGP4 $167
JUMPV
LABELV $166
ADDRLP4 4168
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4168
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRLP4 4168
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 356
ADDP4
ADDP4
INDIRF4
LEF4 $168
line 222
;222:			{
line 223
;223:				v[i] = origin[i] - ent->r.absmax[i];
ADDRLP4 4172
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4172
INDIRI4
ADDRLP4 8
ADDP4
ADDRLP4 4172
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRLP4 4172
INDIRI4
ADDRLP4 4
INDIRP4
CNSTI4 356
ADDP4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 224
;224:			} else 
ADDRGP4 $169
JUMPV
LABELV $168
line 225
;225:			{
line 226
;226:				v[i] = 0;
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
CNSTF4 0
ASGNF4
line 227
;227:			}
LABELV $169
LABELV $167
line 228
;228:		}
LABELV $163
line 216
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $162
line 230
;229:
;230:		dist = VectorLength( v );
ADDRLP4 8
ARGP4
ADDRLP4 4164
ADDRGP4 VectorLength
CALLF4
ASGNF4
ADDRLP4 28
ADDRLP4 4164
INDIRF4
ASGNF4
line 231
;231:		if ( dist >= radius ) 
ADDRLP4 28
INDIRF4
ADDRFP4 4
INDIRF4
LTF4 $170
line 232
;232:		{
line 233
;233:			continue;
ADDRGP4 $155
JUMPV
LABELV $170
line 237
;234:		}
;235:		
;236:		// ok, we are within the radius, add us to the incoming list
;237:		ent_list[ent_count] = ent;
ADDRLP4 24
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 16
INDIRP4
ADDP4
ADDRLP4 4
INDIRP4
ASGNP4
line 238
;238:		ent_count++;
ADDRLP4 24
ADDRLP4 24
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 240
;239:
;240:	}
LABELV $155
line 208
ADDRLP4 20
ADDRLP4 20
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $157
ADDRLP4 20
INDIRI4
ADDRLP4 4128
INDIRI4
LTI4 $154
line 242
;241:	// we are done, return how many we found
;242:	return(ent_count);
ADDRLP4 24
INDIRI4
RETI4
LABELV $147
endproc G_RadiusList 4176 16
export G_PickTarget
proc G_PickTarget 144 12
line 257
;243:}
;244:
;245:
;246:
;247:/*
;248:=============
;249:G_PickTarget
;250:
;251:Selects a random entity from among the targets
;252:=============
;253:*/
;254:#define MAXCHOICES	32
;255:
;256:gentity_t *G_PickTarget (char *targetname)
;257:{
line 258
;258:	gentity_t	*ent = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
line 259
;259:	int		num_choices = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 262
;260:	gentity_t	*choice[MAXCHOICES];
;261:
;262:	if (!targetname)
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $177
line 263
;263:	{
line 264
;264:		G_Printf("G_PickTarget called with NULL targetname\n");
ADDRGP4 $175
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 265
;265:		return NULL;
CNSTP4 0
RETP4
ADDRGP4 $172
JUMPV
LABELV $176
line 269
;266:	}
;267:
;268:	while(1)
;269:	{
line 270
;270:		ent = G_Find (ent, FOFS(targetname), targetname);
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 588
ARGI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 136
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 136
INDIRP4
ASGNP4
line 271
;271:		if (!ent)
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $179
line 272
;272:			break;
ADDRGP4 $178
JUMPV
LABELV $179
line 273
;273:		choice[num_choices++] = ent;
ADDRLP4 140
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 4
ADDRLP4 140
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 140
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
ADDRLP4 0
INDIRP4
ASGNP4
line 274
;274:		if (num_choices == MAXCHOICES)
ADDRLP4 4
INDIRI4
CNSTI4 32
NEI4 $181
line 275
;275:			break;
ADDRGP4 $178
JUMPV
LABELV $181
line 276
;276:	}
LABELV $177
line 268
ADDRGP4 $176
JUMPV
LABELV $178
line 278
;277:
;278:	if (!num_choices)
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $183
line 279
;279:	{
line 280
;280:		G_Printf("G_PickTarget: target %s not found\n", targetname);
ADDRGP4 $185
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 281
;281:		return NULL;
CNSTP4 0
RETP4
ADDRGP4 $172
JUMPV
LABELV $183
line 284
;282:	}
;283:
;284:	return choice[rand() % num_choices];
ADDRLP4 136
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 136
INDIRI4
ADDRLP4 4
INDIRI4
MODI4
CNSTI4 2
LSHI4
ADDRLP4 8
ADDP4
INDIRP4
RETP4
LABELV $172
endproc G_PickTarget 144 12
export G_UseTargets
proc G_UseTargets 24 12
line 299
;285:}
;286:
;287:
;288:/*
;289:==============================
;290:G_UseTargets
;291:
;292:"activator" should be set to the entity that initiated the firing.
;293:
;294:Search for (string)targetname in all entities that
;295:match (string)self.target and call their .use function
;296:
;297:==============================
;298:*/
;299:void G_UseTargets( gentity_t *ent, gentity_t *activator ) {
line 302
;300:	gentity_t		*t;
;301:	
;302:	if ( !ent ) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $187
line 303
;303:		return;
ADDRGP4 $186
JUMPV
LABELV $187
line 306
;304:	}
;305:
;306:	if (ent->targetShaderName && ent->targetShaderNewName) {
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
CNSTU4 0
ASGNU4
ADDRLP4 4
INDIRP4
CNSTI4 596
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 8
INDIRU4
EQU4 $189
ADDRLP4 4
INDIRP4
CNSTI4 600
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 8
INDIRU4
EQU4 $189
line 307
;307:		float f = level.time * 0.001;
ADDRLP4 12
CNSTF4 981668463
ADDRGP4 level+32
INDIRI4
CVIF4 4
MULF4
ASGNF4
line 308
;308:		AddRemap(ent->targetShaderName, ent->targetShaderNewName, f);
ADDRLP4 16
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 596
ADDP4
INDIRP4
ARGP4
ADDRLP4 16
INDIRP4
CNSTI4 600
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
INDIRF4
ARGF4
ADDRGP4 AddRemap
CALLV
pop
line 309
;309:		trap_SetConfigstring(CS_SHADERSTATE, BuildShaderStateConfig());
ADDRLP4 20
ADDRGP4 BuildShaderStateConfig
CALLP4
ASGNP4
CNSTI4 24
ARGI4
ADDRLP4 20
INDIRP4
ARGP4
ADDRGP4 trap_SetConfigstring
CALLV
pop
line 310
;310:	}
LABELV $189
line 312
;311:
;312:	if ( !ent->target ) {
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $192
line 313
;313:		return;
ADDRGP4 $186
JUMPV
LABELV $192
line 316
;314:	}
;315:
;316:	t = NULL;
ADDRLP4 0
CNSTP4 0
ASGNP4
ADDRGP4 $195
JUMPV
LABELV $194
line 317
;317:	while ( (t = G_Find (t, FOFS(targetname), ent->target)) != NULL ) {
line 318
;318:		if ( t == ent ) {
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
NEU4 $197
line 319
;319:			G_Printf ("WARNING: Entity used itself.\n");
ADDRGP4 $199
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 320
;320:		} else {
ADDRGP4 $198
JUMPV
LABELV $197
line 321
;321:			if ( t->use ) {
ADDRLP4 0
INDIRP4
CNSTI4 652
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $200
line 322
;322:				t->use (t, ent, activator);
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 652
ADDP4
INDIRP4
CALLV
pop
line 323
;323:			}
LABELV $200
line 324
;324:		}
LABELV $198
line 325
;325:		if ( !ent->inuse ) {
ADDRFP4 0
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
NEI4 $202
line 326
;326:			G_Printf("entity was removed while using targets\n");
ADDRGP4 $204
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 327
;327:			return;
ADDRGP4 $186
JUMPV
LABELV $202
line 329
;328:		}
;329:	}
LABELV $195
line 317
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 588
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 584
ADDP4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 G_Find
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ASGNP4
ADDRLP4 12
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $194
line 330
;330:}
LABELV $186
endproc G_UseTargets 24 12
bss
align 4
LABELV $206
skip 4
align 4
LABELV $207
skip 96
export tv
code
proc tv 8 0
line 341
;331:
;332:
;333:/*
;334:=============
;335:TempVector
;336:
;337:This is just a convenience function
;338:for making temporary vectors for function calls
;339:=============
;340:*/
;341:float	*tv( float x, float y, float z ) {
line 348
;342:	static	int		index;
;343:	static	vec3_t	vecs[8];
;344:	float	*v;
;345:
;346:	// use an array so that multiple tempvectors won't collide
;347:	// for a while
;348:	v = vecs[index];
ADDRLP4 0
CNSTI4 12
ADDRGP4 $206
INDIRI4
MULI4
ADDRGP4 $207
ADDP4
ASGNP4
line 349
;349:	index = (index + 1)&7;
ADDRLP4 4
ADDRGP4 $206
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
CNSTI4 7
BANDI4
ASGNI4
line 351
;350:
;351:	v[0] = x;
ADDRLP4 0
INDIRP4
ADDRFP4 0
INDIRF4
ASGNF4
line 352
;352:	v[1] = y;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRFP4 4
INDIRF4
ASGNF4
line 353
;353:	v[2] = z;
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
ADDRFP4 8
INDIRF4
ASGNF4
line 355
;354:
;355:	return v;
ADDRLP4 0
INDIRP4
RETP4
LABELV $205
endproc tv 8 0
bss
align 4
LABELV $209
skip 4
align 1
LABELV $210
skip 256
export vtos
code
proc vtos 12 24
line 367
;356:}
;357:
;358:
;359:/*
;360:=============
;361:VectorToString
;362:
;363:This is just a convenience function
;364:for printing vectors
;365:=============
;366:*/
;367:char	*vtos( const vec3_t v ) {
line 373
;368:	static	int		index;
;369:	static	char	str[8][32];
;370:	char	*s;
;371:
;372:	// use an array so that multiple vtos won't collide
;373:	s = str[index];
ADDRLP4 0
ADDRGP4 $209
INDIRI4
CNSTI4 5
LSHI4
ADDRGP4 $210
ADDP4
ASGNP4
line 374
;374:	index = (index + 1)&7;
ADDRLP4 4
ADDRGP4 $209
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
CNSTI4 7
BANDI4
ASGNI4
line 376
;375:
;376:	Com_sprintf (s, 32, "(%i %i %i)", (int)v[0], (int)v[1], (int)v[2]);
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRGP4 $211
ARGP4
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
INDIRP4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
CVFI4 4
ARGI4
ADDRLP4 8
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
CVFI4 4
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 378
;377:
;378:	return s;
ADDRLP4 0
INDIRP4
RETP4
LABELV $208
endproc vtos 12 24
data
align 4
LABELV $213
byte 4 0
byte 4 3212836864
byte 4 0
align 4
LABELV $214
byte 4 0
byte 4 0
byte 4 1065353216
align 4
LABELV $215
byte 4 0
byte 4 3221225472
byte 4 0
align 4
LABELV $216
byte 4 0
byte 4 0
byte 4 3212836864
export G_SetMovedir
code
proc G_SetMovedir 16 16
line 392
;379:}
;380:
;381:
;382:/*
;383:===============
;384:G_SetMovedir
;385:
;386:The editor only specifies a single value for angles (yaw),
;387:but we have special constants to generate an up or down direction.
;388:Angles will be cleared, because it is being used to represent a direction
;389:instead of an orientation.
;390:===============
;391:*/
;392:void G_SetMovedir( vec3_t angles, vec3_t movedir ) {
line 398
;393:	static vec3_t VEC_UP		= {0, -1, 0};
;394:	static vec3_t MOVEDIR_UP	= {0, 0, 1};
;395:	static vec3_t VEC_DOWN		= {0, -2, 0};
;396:	static vec3_t MOVEDIR_DOWN	= {0, 0, -1};
;397:
;398:	if ( VectorCompare (angles, VEC_UP) ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $213
ARGP4
ADDRLP4 0
ADDRGP4 VectorCompare
CALLI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $217
line 399
;399:		VectorCopy (MOVEDIR_UP, movedir);
ADDRFP4 4
INDIRP4
ADDRGP4 $214
INDIRB
ASGNB 12
line 400
;400:	} else if ( VectorCompare (angles, VEC_DOWN) ) {
ADDRGP4 $218
JUMPV
LABELV $217
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $215
ARGP4
ADDRLP4 4
ADDRGP4 VectorCompare
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $219
line 401
;401:		VectorCopy (MOVEDIR_DOWN, movedir);
ADDRFP4 4
INDIRP4
ADDRGP4 $216
INDIRB
ASGNB 12
line 402
;402:	} else {
ADDRGP4 $220
JUMPV
LABELV $219
line 403
;403:		AngleVectors (angles, movedir, NULL, NULL);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 8
CNSTP4 0
ASGNP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRLP4 8
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 404
;404:	}
LABELV $220
LABELV $218
line 405
;405:	VectorClear( angles );
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 12
CNSTF4 0
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 8
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
ADDRLP4 8
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
ADDRLP4 8
INDIRP4
ADDRLP4 12
INDIRF4
ASGNF4
line 406
;406:}
LABELV $212
endproc G_SetMovedir 16 16
export G_InitGentity
proc G_InitGentity 4 0
line 408
;407:
;408:void G_InitGentity( gentity_t *e ) {
line 409
;409:	e->inuse = qtrue;
ADDRFP4 0
INDIRP4
CNSTI4 412
ADDP4
CNSTI4 1
ASGNI4
line 410
;410:	e->classname = "noclass";
ADDRFP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $222
ASGNP4
line 411
;411:	e->s.number = e - g_entities;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRGP4 g_entities
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 828
DIVI4
ASGNI4
line 412
;412:	e->r.ownerNum = ENTITYNUM_NONE;
ADDRFP4 0
INDIRP4
CNSTI4 396
ADDP4
CNSTI4 1023
ASGNI4
line 413
;413:	e->s.modelGhoul2 = 0; //assume not
ADDRFP4 0
INDIRP4
CNSTI4 204
ADDP4
CNSTI4 0
ASGNI4
line 414
;414:}
LABELV $221
endproc G_InitGentity 4 0
export G_Spawn
proc G_Spawn 16 20
line 431
;415:
;416:/*
;417:=================
;418:G_Spawn
;419:
;420:Either finds a free entity, or allocates a new one.
;421:
;422:  The slots from 0 to MAX_CLIENTS-1 are always reserved for clients, and will
;423:never be used by anything else.
;424:
;425:Try to avoid reusing an entity that was recently freed, because it
;426:can cause the client to think the entity morphed into something else
;427:instead of being removed and recreated, which can cause interpolated
;428:angles and bad trails.
;429:=================
;430:*/
;431:gentity_t *G_Spawn( void ) {
line 435
;432:	int			i, force;
;433:	gentity_t	*e;
;434:
;435:	e = NULL;	// shut up warning
ADDRLP4 0
CNSTP4 0
ASGNP4
line 436
;436:	i = 0;		// shut up warning
ADDRLP4 4
CNSTI4 0
ASGNI4
line 437
;437:	for ( force = 0 ; force < 2 ; force++ ) {
ADDRLP4 8
CNSTI4 0
ASGNI4
LABELV $224
line 440
;438:		// if we go through all entities and can't find one to free,
;439:		// override the normal minimum times before use
;440:		e = &g_entities[MAX_CLIENTS];
ADDRLP4 0
ADDRGP4 g_entities+26496
ASGNP4
line 441
;441:		for ( i = MAX_CLIENTS ; i<level.num_entities ; i++, e++) {
ADDRLP4 4
CNSTI4 32
ASGNI4
ADDRGP4 $232
JUMPV
LABELV $229
line 442
;442:			if ( e->inuse ) {
ADDRLP4 0
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $234
line 443
;443:				continue;
ADDRGP4 $230
JUMPV
LABELV $234
line 448
;444:			}
;445:
;446:			// the first couple seconds of server time can involve a lot of
;447:			// freeing and allocating, so relax the replacement policy
;448:			if ( !force && e->freetime > level.startTime + 2000 && level.time - e->freetime < 1000 ) {
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $236
ADDRLP4 12
ADDRLP4 0
INDIRP4
CNSTI4 484
ADDP4
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
ADDRGP4 level+40
INDIRI4
CNSTI4 2000
ADDI4
LEI4 $236
ADDRGP4 level+32
INDIRI4
ADDRLP4 12
INDIRI4
SUBI4
CNSTI4 1000
GEI4 $236
line 449
;449:				continue;
ADDRGP4 $230
JUMPV
LABELV $236
line 453
;450:			}
;451:
;452:			// reuse this slot
;453:			G_InitGentity( e );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_InitGentity
CALLV
pop
line 454
;454:			return e;
ADDRLP4 0
INDIRP4
RETP4
ADDRGP4 $223
JUMPV
LABELV $230
line 441
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 828
ADDP4
ASGNP4
LABELV $232
ADDRLP4 4
INDIRI4
ADDRGP4 level+12
INDIRI4
LTI4 $229
line 456
;455:		}
;456:		if ( i != MAX_GENTITIES ) {
ADDRLP4 4
INDIRI4
CNSTI4 1024
EQI4 $240
line 457
;457:			break;
ADDRGP4 $226
JUMPV
LABELV $240
line 459
;458:		}
;459:	}
LABELV $225
line 437
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 2
LTI4 $224
LABELV $226
line 460
;460:	if ( i == ENTITYNUM_MAX_NORMAL ) {
ADDRLP4 4
INDIRI4
CNSTI4 1022
NEI4 $242
line 461
;461:		for (i = 0; i < MAX_GENTITIES; i++) {
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $244
line 462
;462:			G_Printf("%4i: %s\n", i, g_entities[i].classname);
ADDRGP4 $248
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
CNSTI4 828
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_entities+416
ADDP4
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 463
;463:		}
LABELV $245
line 461
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 1024
LTI4 $244
line 464
;464:		G_Error( "G_Spawn: no free entities" );
ADDRGP4 $250
ARGP4
ADDRGP4 G_Error
CALLV
pop
line 465
;465:	}
LABELV $242
line 468
;466:	
;467:	// open up a new slot
;468:	level.num_entities++;
ADDRLP4 12
ADDRGP4 level+12
ASGNP4
ADDRLP4 12
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 471
;469:
;470:	// let the server system know that there are more entities
;471:	trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ), 
ADDRGP4 level+4
INDIRP4
ARGP4
ADDRGP4 level+12
INDIRI4
ARGI4
CNSTI4 828
ARGI4
ADDRGP4 level
INDIRP4
ARGP4
CNSTI4 1756
ARGI4
ADDRGP4 trap_LocateGameData
CALLV
pop
line 474
;472:		&level.clients[0].ps, sizeof( level.clients[0] ) );
;473:
;474:	G_InitGentity( e );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 G_InitGentity
CALLV
pop
line 475
;475:	return e;
ADDRLP4 0
INDIRP4
RETP4
LABELV $223
endproc G_Spawn 16 20
export G_EntitiesFree
proc G_EntitiesFree 8 0
line 483
;476:}
;477:
;478:/*
;479:=================
;480:G_EntitiesFree
;481:=================
;482:*/
;483:qboolean G_EntitiesFree( void ) {
line 487
;484:	int			i;
;485:	gentity_t	*e;
;486:
;487:	e = &g_entities[MAX_CLIENTS];
ADDRLP4 4
ADDRGP4 g_entities+26496
ASGNP4
line 488
;488:	for ( i = MAX_CLIENTS; i < level.num_entities; i++, e++) {
ADDRLP4 0
CNSTI4 32
ASGNI4
ADDRGP4 $259
JUMPV
LABELV $256
line 489
;489:		if ( e->inuse ) {
ADDRLP4 4
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $261
line 490
;490:			continue;
ADDRGP4 $257
JUMPV
LABELV $261
line 493
;491:		}
;492:		// slot available
;493:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $254
JUMPV
LABELV $257
line 488
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 4
ADDRLP4 4
INDIRP4
CNSTI4 828
ADDP4
ASGNP4
LABELV $259
ADDRLP4 0
INDIRI4
ADDRGP4 level+12
INDIRI4
LTI4 $256
line 495
;494:	}
;495:	return qfalse;
CNSTI4 0
RETI4
LABELV $254
endproc G_EntitiesFree 8 0
data
export gG2KillNum
align 4
LABELV gG2KillNum
byte 4 0
export G_SendG2KillQueue
code
proc G_SendG2KillQueue 1032 12
line 504
;496:}
;497:
;498:#define MAX_G2_KILL_QUEUE 64
;499:
;500:int gG2KillIndex[MAX_G2_KILL_QUEUE];
;501:int gG2KillNum = 0;
;502:
;503:void G_SendG2KillQueue(void)
;504:{
line 506
;505:	char g2KillString[1024];
;506:	int i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 508
;507:	
;508:	if (!gG2KillNum)
ADDRGP4 gG2KillNum
INDIRI4
CNSTI4 0
NEI4 $264
line 509
;509:	{
line 510
;510:		return;
ADDRGP4 $263
JUMPV
LABELV $264
line 513
;511:	}
;512:
;513:	Com_sprintf(g2KillString, 1024, "kg2");
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $266
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
ADDRGP4 $268
JUMPV
LABELV $267
line 516
;514:
;515:	while (i < gG2KillNum)
;516:	{
line 517
;517:		Q_strcat(g2KillString, 1024, va(" %i", gG2KillIndex[i]));
ADDRGP4 $270
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 gG2KillIndex
ADDP4
INDIRI4
ARGI4
ADDRLP4 1028
ADDRGP4 va
CALLP4
ASGNP4
ADDRLP4 4
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 1028
INDIRP4
ARGP4
ADDRGP4 Q_strcat
CALLV
pop
line 518
;518:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 519
;519:	}
LABELV $268
line 515
ADDRLP4 0
INDIRI4
ADDRGP4 gG2KillNum
INDIRI4
LTI4 $267
line 521
;520:
;521:	trap_SendServerCommand(-1, g2KillString);
CNSTI4 -1
ARGI4
ADDRLP4 4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 524
;522:
;523:	//Clear the count because we just sent off the whole queue
;524:	gG2KillNum = 0;
ADDRGP4 gG2KillNum
CNSTI4 0
ASGNI4
line 525
;525:}
LABELV $263
endproc G_SendG2KillQueue 1032 12
export G_KillG2Queue
proc G_KillG2Queue 4 8
line 528
;526:
;527:void G_KillG2Queue(int entNum)
;528:{
line 529
;529:	if (gG2KillNum >= MAX_G2_KILL_QUEUE)
ADDRGP4 gG2KillNum
INDIRI4
CNSTI4 64
LTI4 $272
line 530
;530:	{ //This would be considered a Bad Thing.
line 535
;531:#ifdef _DEBUG
;532:		Com_Printf("WARNING: Exceeded the MAX_G2_KILL_QUEUE count for this frame!\n");
;533:#endif
;534:		//Since we're out of queue slots, just send it now as a seperate command (eats more bandwidth, but we have no choice)
;535:		trap_SendServerCommand(-1, va("kg2 %i", entNum));
ADDRGP4 $274
ARGP4
ADDRFP4 0
INDIRI4
ARGI4
ADDRLP4 0
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 536
;536:		return;
ADDRGP4 $271
JUMPV
LABELV $272
line 539
;537:	}
;538:
;539:	gG2KillIndex[gG2KillNum] = entNum;
ADDRGP4 gG2KillNum
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 gG2KillIndex
ADDP4
ADDRFP4 0
INDIRI4
ASGNI4
line 540
;540:	gG2KillNum++;
ADDRLP4 0
ADDRGP4 gG2KillNum
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 0
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 541
;541:}
LABELV $271
endproc G_KillG2Queue 4 8
export G_FreeEntity
proc G_FreeEntity 20 12
line 550
;542:
;543:/*
;544:=================
;545:G_FreeEntity
;546:
;547:Marks the entity as free
;548:=================
;549:*/
;550:void G_FreeEntity( gentity_t *ed ) {
line 553
;551:	//gentity_t *te;
;552:
;553:	if (ed->isSaberEntity)
ADDRFP4 0
INDIRP4
CNSTI4 812
ADDP4
INDIRI4
CNSTI4 0
EQI4 $276
line 554
;554:	{
line 558
;555:#ifdef _DEBUG
;556:		Com_Printf("Tried to remove JM saber!\n");
;557:#endif
;558:		return;
ADDRGP4 $275
JUMPV
LABELV $276
line 561
;559:	}
;560:
;561:	trap_UnlinkEntity (ed);		// unlink from world
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 trap_UnlinkEntity
CALLV
pop
line 563
;562:
;563:	if ( ed->neverFree ) {
ADDRFP4 0
INDIRP4
CNSTI4 468
ADDP4
INDIRI4
CNSTI4 0
EQI4 $278
line 564
;564:		return;
ADDRGP4 $275
JUMPV
LABELV $278
line 571
;565:	}
;566:
;567:	//rww - this may seem a bit hackish, but unfortunately we have no access
;568:	//to anything ghoul2-related on the server and thus must send a message
;569:	//to let the client know he needs to clean up all the g2 stuff for this
;570:	//now-removed entity
;571:	if (ed->s.modelGhoul2)
ADDRFP4 0
INDIRP4
CNSTI4 204
ADDP4
INDIRI4
CNSTI4 0
EQI4 $280
line 572
;572:	{ //force all clients to accept an event to destroy this instance, right now
line 579
;573:		/*
;574:		te = G_TempEntity( vec3_origin, EV_DESTROY_GHOUL2_INSTANCE );
;575:		te->r.svFlags |= SVF_BROADCAST;
;576:		te->s.eventParm = ed->s.number;
;577:		*/
;578:		//Or not. Events can be dropped, so that would be a bad thing.
;579:		G_KillG2Queue(ed->s.number);
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
ADDRGP4 G_KillG2Queue
CALLV
pop
line 580
;580:	}
LABELV $280
line 582
;581:
;582:	if (ed->s.eFlags & EF_SOUNDTRACKER)
ADDRFP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 8388608
BANDI4
CNSTI4 0
EQI4 $282
line 583
;583:	{
line 584
;584:		int i = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $285
JUMPV
LABELV $284
line 588
;585:		gentity_t *ent;
;586:
;587:		while (i < MAX_CLIENTS)
;588:		{
line 589
;589:			ent = &g_entities[i];
ADDRLP4 0
CNSTI4 828
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 591
;590:
;591:			if (ent && ent->inuse && ent->client)
ADDRLP4 12
CNSTU4 0
ASGNU4
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRLP4 12
INDIRU4
EQU4 $287
ADDRLP4 0
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $287
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 12
INDIRU4
EQU4 $287
line 592
;592:			{
line 593
;593:				int ch = TRACK_CHANNEL_NONE-50;
ADDRLP4 16
CNSTI4 0
ASGNI4
ADDRGP4 $290
JUMPV
LABELV $289
line 596
;594:
;595:				while (ch < NUM_TRACK_CHANNELS-50)
;596:				{
line 597
;597:					if (ent->client->ps.fd.killSoundEntIndex[ch] == ed->s.number)
ADDRLP4 16
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1188
ADDP4
ADDP4
INDIRI4
ADDRFP4 0
INDIRP4
INDIRI4
NEI4 $292
line 598
;598:					{
line 599
;599:						ent->client->ps.fd.killSoundEntIndex[ch] = 0;
ADDRLP4 16
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1188
ADDP4
ADDP4
CNSTI4 0
ASGNI4
line 600
;600:					}
LABELV $292
line 602
;601:
;602:					ch++;
ADDRLP4 16
ADDRLP4 16
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 603
;603:				}
LABELV $290
line 595
ADDRLP4 16
INDIRI4
CNSTI4 6
LTI4 $289
line 604
;604:			}
LABELV $287
line 606
;605:
;606:			i++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 607
;607:		}
LABELV $285
line 587
ADDRLP4 4
INDIRI4
CNSTI4 32
LTI4 $284
line 608
;608:	}
LABELV $282
line 610
;609:
;610:	memset (ed, 0, sizeof(*ed));
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 0
ARGI4
CNSTI4 828
ARGI4
ADDRGP4 memset
CALLP4
pop
line 611
;611:	ed->classname = "freed";
ADDRFP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $294
ASGNP4
line 612
;612:	ed->freetime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 484
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 613
;613:	ed->inuse = qfalse;
ADDRFP4 0
INDIRP4
CNSTI4 412
ADDP4
CNSTI4 0
ASGNI4
line 614
;614:}
LABELV $275
endproc G_FreeEntity 20 12
export G_TempEntity
proc G_TempEntity 20 8
line 625
;615:
;616:/*
;617:=================
;618:G_TempEntity
;619:
;620:Spawns an event entity that will be auto-removed
;621:The origin will be snapped to save net bandwidth, so care
;622:must be taken if the origin is right on a surface (snap towards start vector first)
;623:=================
;624:*/
;625:gentity_t *G_TempEntity( vec3_t origin, int event ) {
line 629
;626:	gentity_t		*e;
;627:	vec3_t		snapped;
;628:
;629:	e = G_Spawn();
ADDRLP4 16
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 16
INDIRP4
ASGNP4
line 630
;630:	e->s.eType = ET_EVENTS + event;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRFP4 4
INDIRI4
CNSTI4 16
ADDI4
ASGNI4
line 632
;631:
;632:	e->classname = "tempEntity";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $297
ASGNP4
line 633
;633:	e->eventTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 488
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 634
;634:	e->freeAfterEvent = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 492
ADDP4
CNSTI4 1
ASGNI4
line 636
;635:
;636:	VectorCopy( origin, snapped );
ADDRLP4 4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 637
;637:	SnapVector( snapped );		// save network bandwidth
ADDRLP4 4
ADDRLP4 4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 4+4
ADDRLP4 4+4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 4+8
ADDRLP4 4+8
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
line 638
;638:	G_SetOrigin( e, snapped );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 641
;639:
;640:	// find cluster for PVS
;641:	trap_LinkEntity( e );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 643
;642:
;643:	return e;
ADDRLP4 0
INDIRP4
RETP4
LABELV $296
endproc G_TempEntity 20 8
export GetTrackerEnt
proc GetTrackerEnt 12 0
line 649
;644:}
;645:
;646:
;647:
;648:gentity_t *GetTrackerEnt(int channel)
;649:{
line 650
;650:	int i = MAX_CLIENTS;
ADDRLP4 4
CNSTI4 32
ASGNI4
ADDRGP4 $305
JUMPV
LABELV $304
line 654
;651:	gentity_t *ent;
;652:
;653:	while (i < MAX_GENTITIES)
;654:	{
line 655
;655:		ent = &g_entities[i];
ADDRLP4 0
CNSTI4 828
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 657
;656:
;657:		if (ent && (ent->s.eFlags & EF_SOUNDTRACKER) && ent->s.saberEntityNum == channel)
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $307
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 8388608
BANDI4
CNSTI4 0
EQI4 $307
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
INDIRI4
ADDRFP4 0
INDIRI4
NEI4 $307
line 658
;658:		{
line 659
;659:			return ent;
ADDRLP4 0
INDIRP4
RETP4
ADDRGP4 $303
JUMPV
LABELV $307
line 662
;660:		}
;661:
;662:		i++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 663
;663:	}
LABELV $305
line 653
ADDRLP4 4
INDIRI4
CNSTI4 1024
LTI4 $304
line 665
;664:
;665:	return NULL;
CNSTP4 0
RETP4
LABELV $303
endproc GetTrackerEnt 12 0
export G_SoundTempEntity
proc G_SoundTempEntity 20 8
line 675
;666:}
;667:
;668:/*
;669:=================
;670:G_SoundTempEntity
;671:
;672:Special event entity that keeps sound trackers in mind
;673:=================
;674:*/
;675:gentity_t *G_SoundTempEntity( vec3_t origin, int event, int channel ) {
line 679
;676:	gentity_t		*e;
;677:	vec3_t		snapped;
;678:
;679:	e = G_Spawn();
ADDRLP4 16
ADDRGP4 G_Spawn
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 16
INDIRP4
ASGNP4
line 681
;680:
;681:	e->s.eType = ET_EVENTS + event;
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
ADDRFP4 4
INDIRI4
CNSTI4 16
ADDI4
ASGNI4
line 682
;682:	e->inuse = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 412
ADDP4
CNSTI4 1
ASGNI4
line 684
;683:
;684:	e->classname = "tempEntity";
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
ADDRGP4 $297
ASGNP4
line 685
;685:	e->eventTime = level.time;
ADDRLP4 0
INDIRP4
CNSTI4 488
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 686
;686:	e->freeAfterEvent = qtrue;
ADDRLP4 0
INDIRP4
CNSTI4 492
ADDP4
CNSTI4 1
ASGNI4
line 688
;687:
;688:	VectorCopy( origin, snapped );
ADDRLP4 4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 689
;689:	SnapVector( snapped );		// save network bandwidth
ADDRLP4 4
ADDRLP4 4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 4+4
ADDRLP4 4+4
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
ADDRLP4 4+8
ADDRLP4 4+8
INDIRF4
CVFI4 4
CVIF4 4
ASGNF4
line 690
;690:	G_SetOrigin( e, snapped );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 G_SetOrigin
CALLV
pop
line 693
;691:
;692:	// find cluster for PVS
;693:	trap_LinkEntity( e );
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 trap_LinkEntity
CALLV
pop
line 695
;694:
;695:	return e;
ADDRLP4 0
INDIRP4
RETP4
LABELV $309
endproc G_SoundTempEntity 20 8
export G_KillBox
proc G_KillBox 4168 32
line 716
;696:}
;697:
;698:
;699:
;700:/*
;701:==============================================================================
;702:
;703:Kill box
;704:
;705:==============================================================================
;706:*/
;707:
;708:/*
;709:=================
;710:G_KillBox
;711:
;712:Kills all entities that would touch the proposed new positioning
;713:of ent.  Ent should be unlinked before calling this!
;714:=================
;715:*/
;716:void G_KillBox (gentity_t *ent) {
line 722
;717:	int			i, num;
;718:	int			touch[MAX_GENTITIES];
;719:	gentity_t	*hit;
;720:	vec3_t		mins, maxs;
;721:
;722:	VectorAdd( ent->client->ps.origin, ent->r.mins, mins );
ADDRLP4 4132
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4136
ADDRLP4 4132
INDIRP4
CNSTI4 408
ADDP4
ASGNP4
ADDRLP4 4108
ADDRLP4 4136
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 4132
INDIRP4
CNSTI4 316
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4108+4
ADDRLP4 4136
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 4132
INDIRP4
CNSTI4 320
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4140
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4108+8
ADDRLP4 4140
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 4140
INDIRP4
CNSTI4 324
ADDP4
INDIRF4
ADDF4
ASGNF4
line 723
;723:	VectorAdd( ent->client->ps.origin, ent->r.maxs, maxs );
ADDRLP4 4144
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4148
ADDRLP4 4144
INDIRP4
CNSTI4 408
ADDP4
ASGNP4
ADDRLP4 4120
ADDRLP4 4148
INDIRP4
INDIRP4
CNSTI4 20
ADDP4
INDIRF4
ADDRLP4 4144
INDIRP4
CNSTI4 328
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4120+4
ADDRLP4 4148
INDIRP4
INDIRP4
CNSTI4 24
ADDP4
INDIRF4
ADDRLP4 4144
INDIRP4
CNSTI4 332
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 4152
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4120+8
ADDRLP4 4152
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 28
ADDP4
INDIRF4
ADDRLP4 4152
INDIRP4
CNSTI4 336
ADDP4
INDIRF4
ADDF4
ASGNF4
line 724
;724:	num = trap_EntitiesInBox( mins, maxs, touch, MAX_GENTITIES );
ADDRLP4 4108
ARGP4
ADDRLP4 4120
ARGP4
ADDRLP4 12
ARGP4
CNSTI4 1024
ARGI4
ADDRLP4 4156
ADDRGP4 trap_EntitiesInBox
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 4156
INDIRI4
ASGNI4
line 726
;725:
;726:	for (i=0 ; i<num ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $323
JUMPV
LABELV $320
line 727
;727:		hit = &g_entities[touch[i]];
ADDRLP4 4
CNSTI4 828
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRLP4 12
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 728
;728:		if ( !hit->client ) {
ADDRLP4 4
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $324
line 729
;729:			continue;
ADDRGP4 $321
JUMPV
LABELV $324
line 733
;730:		}
;731:
;732:		// nail it
;733:		G_Damage ( hit, ent, ent, NULL, NULL,
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 4160
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4160
INDIRP4
ARGP4
ADDRLP4 4160
INDIRP4
ARGP4
ADDRLP4 4164
CNSTP4 0
ASGNP4
ADDRLP4 4164
INDIRP4
ARGP4
ADDRLP4 4164
INDIRP4
ARGP4
CNSTI4 100000
ARGI4
CNSTI4 8
ARGI4
CNSTI4 33
ARGI4
ADDRGP4 G_Damage
CALLV
pop
line 735
;734:			100000, DAMAGE_NO_PROTECTION, MOD_TELEFRAG);
;735:	}
LABELV $321
line 726
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $323
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
LTI4 $320
line 737
;736:
;737:}
LABELV $315
endproc G_KillBox 4168 32
export G_AddPredictableEvent
proc G_AddPredictableEvent 0 12
line 750
;738:
;739://==============================================================================
;740:
;741:/*
;742:===============
;743:G_AddPredictableEvent
;744:
;745:Use for non-pmove events that would also be predicted on the
;746:client side: jumppads and item pickups
;747:Adds an event+parm and twiddles the event counter
;748:===============
;749:*/
;750:void G_AddPredictableEvent( gentity_t *ent, int event, int eventParm ) {
line 751
;751:	if ( !ent->client ) {
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $327
line 752
;752:		return;
ADDRGP4 $326
JUMPV
LABELV $327
line 754
;753:	}
;754:	BG_AddPredictableEventToPlayerstate( event, eventParm, &ent->client->ps );
ADDRFP4 4
INDIRI4
ARGI4
ADDRFP4 8
INDIRI4
ARGI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
ARGP4
ADDRGP4 BG_AddPredictableEventToPlayerstate
CALLV
pop
line 755
;755:}
LABELV $326
endproc G_AddPredictableEvent 0 12
export G_AddEvent
proc G_AddEvent 4 8
line 765
;756:
;757:
;758:/*
;759:===============
;760:G_AddEvent
;761:
;762:Adds an event+parm and twiddles the event counter
;763:===============
;764:*/
;765:void G_AddEvent( gentity_t *ent, int event, int eventParm ) {
line 768
;766:	int		bits;
;767:
;768:	if ( !event ) {
ADDRFP4 4
INDIRI4
CNSTI4 0
NEI4 $330
line 769
;769:		G_Printf( "G_AddEvent: zero event added for entity %i\n", ent->s.number );
ADDRGP4 $332
ARGP4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
ADDRGP4 G_Printf
CALLV
pop
line 770
;770:		return;
ADDRGP4 $329
JUMPV
LABELV $330
line 774
;771:	}
;772:
;773:	// clients need to add the event in playerState_t instead of entityState_t
;774:	if ( ent->client ) {
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $333
line 775
;775:		bits = ent->client->ps.externalEvent & EV_EVENT_BITS;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 132
ADDP4
INDIRI4
CNSTI4 768
BANDI4
ASGNI4
line 776
;776:		bits = ( bits + EV_EVENT_BIT1 ) & EV_EVENT_BITS;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 256
ADDI4
CNSTI4 768
BANDI4
ASGNI4
line 777
;777:		ent->client->ps.externalEvent = event | bits;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 132
ADDP4
ADDRFP4 4
INDIRI4
ADDRLP4 0
INDIRI4
BORI4
ASGNI4
line 778
;778:		ent->client->ps.externalEventParm = eventParm;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 136
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 779
;779:		ent->client->ps.externalEventTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 140
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 780
;780:	} else {
ADDRGP4 $334
JUMPV
LABELV $333
line 781
;781:		bits = ent->s.event & EV_EVENT_BITS;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 252
ADDP4
INDIRI4
CNSTI4 768
BANDI4
ASGNI4
line 782
;782:		bits = ( bits + EV_EVENT_BIT1 ) & EV_EVENT_BITS;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 256
ADDI4
CNSTI4 768
BANDI4
ASGNI4
line 783
;783:		ent->s.event = event | bits;
ADDRFP4 0
INDIRP4
CNSTI4 252
ADDP4
ADDRFP4 4
INDIRI4
ADDRLP4 0
INDIRI4
BORI4
ASGNI4
line 784
;784:		ent->s.eventParm = eventParm;
ADDRFP4 0
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 785
;785:	}
LABELV $334
line 786
;786:	ent->eventTime = level.time;
ADDRFP4 0
INDIRP4
CNSTI4 488
ADDP4
ADDRGP4 level+32
INDIRI4
ASGNI4
line 787
;787:}
LABELV $329
endproc G_AddEvent 4 8
export G_PlayEffect
proc G_PlayEffect 8 8
line 795
;788:
;789:/*
;790:=============
;791:G_PlayEffect
;792:=============
;793:*/
;794:gentity_t *G_PlayEffect(int fxID, vec3_t org, vec3_t ang)
;795:{
line 798
;796:	gentity_t	*te;
;797:
;798:	te = G_TempEntity( org, EV_PLAY_EFFECT );
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 63
ARGI4
ADDRLP4 4
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 799
;799:	VectorCopy(ang, te->s.angles);
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ADDRFP4 8
INDIRP4
INDIRB
ASGNB 12
line 800
;800:	VectorCopy(org, te->s.origin);
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 801
;801:	te->s.eventParm = fxID;
ADDRLP4 0
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 0
INDIRI4
ASGNI4
line 803
;802:
;803:	return te;
ADDRLP4 0
INDIRP4
RETP4
LABELV $337
endproc G_PlayEffect 8 8
export G_ScreenShake
proc G_ScreenShake 12 8
line 812
;804:}
;805:
;806:/*
;807:=============
;808:G_ScreenShake
;809:=============
;810:*/
;811:gentity_t *G_ScreenShake(vec3_t org, gentity_t *target, float intensity, int duration, qboolean global)
;812:{
line 815
;813:	gentity_t	*te;
;814:
;815:	te = G_TempEntity( org, EV_SCREENSHAKE );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 38
ARGI4
ADDRLP4 4
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 816
;816:	VectorCopy(org, te->s.origin);
ADDRLP4 0
INDIRP4
CNSTI4 92
ADDP4
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 817
;817:	te->s.angles[0] = intensity;
ADDRLP4 0
INDIRP4
CNSTI4 116
ADDP4
ADDRFP4 8
INDIRF4
ASGNF4
line 818
;818:	te->s.time = duration;
ADDRLP4 0
INDIRP4
CNSTI4 84
ADDP4
ADDRFP4 12
INDIRI4
ASGNI4
line 820
;819:
;820:	if (target)
ADDRFP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $339
line 821
;821:	{
line 822
;822:		te->s.modelindex = target->s.number+1;
ADDRLP4 0
INDIRP4
CNSTI4 212
ADDP4
ADDRFP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 823
;823:	}
ADDRGP4 $340
JUMPV
LABELV $339
line 825
;824:	else
;825:	{
line 826
;826:		te->s.modelindex = 0;
ADDRLP4 0
INDIRP4
CNSTI4 212
ADDP4
CNSTI4 0
ASGNI4
line 827
;827:	}
LABELV $340
line 829
;828:
;829:	if (global)
ADDRFP4 16
INDIRI4
CNSTI4 0
EQI4 $341
line 830
;830:	{
line 831
;831:		te->r.svFlags |= SVF_BROADCAST;
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 304
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 32
BORI4
ASGNI4
line 832
;832:	}
LABELV $341
line 834
;833:
;834:	return te;
ADDRLP4 0
INDIRP4
RETP4
LABELV $338
endproc G_ScreenShake 12 8
export G_MuteSound
proc G_MuteSound 16 8
line 843
;835:}
;836:
;837:/*
;838:=============
;839:G_MuteSound
;840:=============
;841:*/
;842:void G_MuteSound( int entnum, int channel )
;843:{
line 846
;844:	gentity_t	*te, *e;
;845:
;846:	te = G_TempEntity( vec3_origin, EV_MUTE_SOUND );
ADDRGP4 vec3_origin
ARGP4
CNSTI4 65
ARGI4
ADDRLP4 8
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 847
;847:	te->r.svFlags = SVF_BROADCAST;
ADDRLP4 0
INDIRP4
CNSTI4 304
ADDP4
CNSTI4 32
ASGNI4
line 848
;848:	te->s.trickedentindex2 = entnum;
ADDRLP4 0
INDIRP4
CNSTI4 152
ADDP4
ADDRFP4 0
INDIRI4
ASGNI4
line 849
;849:	te->s.trickedentindex = channel;
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 851
;850:
;851:	e = &g_entities[entnum];
ADDRLP4 4
CNSTI4 828
ADDRFP4 0
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 853
;852:
;853:	if (e && (e->s.eFlags & EF_SOUNDTRACKER))
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $344
ADDRLP4 4
INDIRP4
CNSTI4 8
ADDP4
INDIRI4
CNSTI4 8388608
BANDI4
CNSTI4 0
EQI4 $344
line 854
;854:	{
line 855
;855:		G_FreeEntity(e);
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 856
;856:		e->s.eFlags = 0;
ADDRLP4 4
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 0
ASGNI4
line 857
;857:	}
LABELV $344
line 858
;858:}
LABELV $343
endproc G_MuteSound 16 8
export G_Sound
proc G_Sound 24 12
line 865
;859:
;860:/*
;861:=============
;862:G_Sound
;863:=============
;864:*/
;865:void G_Sound( gentity_t *ent, int channel, int soundIndex ) {
line 868
;866:	gentity_t	*te;
;867:
;868:	te = G_SoundTempEntity( ent->r.currentOrigin, EV_GENERAL_SOUND, channel );
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
CNSTI4 66
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRLP4 4
ADDRGP4 G_SoundTempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 869
;869:	te->s.eventParm = soundIndex;
ADDRLP4 0
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 871
;870:
;871:	if (ent && ent->client && channel > TRACK_CHANNEL_NONE)
ADDRLP4 8
ADDRFP4 0
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
EQU4 $347
ADDRLP4 8
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CVPU4 4
ADDRLP4 12
INDIRU4
EQU4 $347
ADDRFP4 4
INDIRI4
CNSTI4 50
LEI4 $347
line 872
;872:	{ //let the client remember the index of the player entity so he can kill the most recent sound on request
line 873
;873:		te->s.saberEntityNum = channel;
ADDRLP4 0
INDIRP4
CNSTI4 232
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 875
;874:
;875:		if (g_entities[ent->client->ps.fd.killSoundEntIndex[channel-50]].inuse &&
ADDRLP4 16
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 200
SUBI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1188
ADDP4
ADDP4
INDIRI4
ASGNI4
CNSTI4 828
ADDRLP4 16
INDIRI4
MULI4
ADDRGP4 g_entities+412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $349
ADDRLP4 16
INDIRI4
CNSTI4 32
LEI4 $349
line 877
;876:			ent->client->ps.fd.killSoundEntIndex[channel-50] > MAX_CLIENTS)
;877:		{
line 878
;878:			G_MuteSound(ent->client->ps.fd.killSoundEntIndex[channel-50], CHAN_VOICE);
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 200
SUBI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1188
ADDP4
ADDP4
INDIRI4
ARGI4
CNSTI4 3
ARGI4
ADDRGP4 G_MuteSound
CALLV
pop
line 879
;879:			if (ent->client->ps.fd.killSoundEntIndex[channel-50] > MAX_CLIENTS && g_entities[ent->client->ps.fd.killSoundEntIndex[channel-50]].inuse)
ADDRLP4 20
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 200
SUBI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1188
ADDP4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 32
LEI4 $352
CNSTI4 828
ADDRLP4 20
INDIRI4
MULI4
ADDRGP4 g_entities+412
ADDP4
INDIRI4
CNSTI4 0
EQI4 $352
line 880
;880:			{
line 881
;881:				G_FreeEntity(&g_entities[ent->client->ps.fd.killSoundEntIndex[channel-50]]);
CNSTI4 828
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 200
SUBI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1188
ADDP4
ADDP4
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRGP4 G_FreeEntity
CALLV
pop
line 882
;882:			}
LABELV $352
line 883
;883:			ent->client->ps.fd.killSoundEntIndex[channel-50] = 0;
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 200
SUBI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1188
ADDP4
ADDP4
CNSTI4 0
ASGNI4
line 884
;884:		}
LABELV $349
line 886
;885:
;886:		ent->client->ps.fd.killSoundEntIndex[channel-50] = te->s.number;
ADDRFP4 4
INDIRI4
CNSTI4 2
LSHI4
CNSTI4 200
SUBI4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 1188
ADDP4
ADDP4
ADDRLP4 0
INDIRP4
INDIRI4
ASGNI4
line 887
;887:		te->s.trickedentindex = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 888
;888:		te->s.eFlags = EF_SOUNDTRACKER;
ADDRLP4 0
INDIRP4
CNSTI4 8
ADDP4
CNSTI4 8388608
ASGNI4
line 890
;889:		//te->freeAfterEvent = qfalse;
;890:	}
LABELV $347
line 891
;891:}
LABELV $346
endproc G_Sound 24 12
export G_SoundAtLoc
proc G_SoundAtLoc 8 8
line 898
;892:
;893:/*
;894:=============
;895:G_SoundAtLoc
;896:=============
;897:*/
;898:void G_SoundAtLoc( vec3_t loc, int channel, int soundIndex ) {
line 901
;899:	gentity_t	*te;
;900:
;901:	te = G_TempEntity( loc, EV_GENERAL_SOUND );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 66
ARGI4
ADDRLP4 4
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 902
;902:	te->s.eventParm = soundIndex;
ADDRLP4 0
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 903
;903:}
LABELV $355
endproc G_SoundAtLoc 8 8
export G_EntitySound
proc G_EntitySound 8 8
line 910
;904:
;905:/*
;906:=============
;907:G_EntitySound
;908:=============
;909:*/
;910:void G_EntitySound( gentity_t *ent, int channel, int soundIndex ) {
line 913
;911:	gentity_t	*te;
;912:
;913:	te = G_TempEntity( ent->r.currentOrigin, EV_ENTITY_SOUND );
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ARGP4
CNSTI4 69
ARGI4
ADDRLP4 4
ADDRGP4 G_TempEntity
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 914
;914:	te->s.eventParm = soundIndex;
ADDRLP4 0
INDIRP4
CNSTI4 256
ADDP4
ADDRFP4 8
INDIRI4
ASGNI4
line 915
;915:	te->s.weapon = ent->s.number;
ADDRLP4 0
INDIRP4
CNSTI4 276
ADDP4
ADDRFP4 0
INDIRP4
INDIRI4
ASGNI4
line 916
;916:	te->s.trickedentindex = channel;
ADDRLP4 0
INDIRP4
CNSTI4 148
ADDP4
ADDRFP4 4
INDIRI4
ASGNI4
line 917
;917:}
LABELV $356
endproc G_EntitySound 8 8
export ValidUseTarget
proc ValidUseTarget 0 0
line 930
;918:
;919:
;920://==============================================================================
;921:
;922:/*
;923:==============
;924:ValidUseTarget
;925:
;926:Returns whether or not the targeted entity is useable
;927:==============
;928:*/
;929:qboolean ValidUseTarget( gentity_t *ent )
;930:{
line 931
;931:	if ( !ent->use )
ADDRFP4 0
INDIRP4
CNSTI4 652
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $358
line 932
;932:	{
line 933
;933:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $357
JUMPV
LABELV $358
line 936
;934:	}
;935:
;936:	if ( !(ent->r.svFlags & SVF_PLAYER_USABLE) )
ADDRFP4 0
INDIRP4
CNSTI4 304
ADDP4
INDIRI4
CNSTI4 16
BANDI4
CNSTI4 0
NEI4 $360
line 937
;937:	{//Check for flag that denotes BUTTON_USE useability
line 938
;938:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $357
JUMPV
LABELV $360
line 941
;939:	}
;940:
;941:	return qtrue;
CNSTI4 1
RETI4
LABELV $357
endproc ValidUseTarget 0 0
export TryUse
proc TryUse 1156 28
line 955
;942:}
;943:
;944:/*
;945:==============
;946:TryUse
;947:
;948:Try and use an entity in the world, directly ahead of us
;949:==============
;950:*/
;951:
;952:#define USE_DISTANCE	64.0f
;953:
;954:void TryUse( gentity_t *ent )
;955:{
line 961
;956:	gentity_t	*target;
;957:	trace_t		trace;
;958:	vec3_t		src, dest, vf;
;959:	vec3_t		viewspot;
;960:
;961:	VectorCopy(ent->client->ps.origin, viewspot);
ADDRLP4 1120
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 20
ADDP4
INDIRB
ASGNB 12
line 962
;962:	viewspot[2] += ent->client->ps.viewheight;
ADDRLP4 1120+8
ADDRLP4 1120+8
INDIRF4
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 168
ADDP4
INDIRI4
CVIF4 4
ADDF4
ASGNF4
line 964
;963:
;964:	VectorCopy( viewspot, src );
ADDRLP4 0
ADDRLP4 1120
INDIRB
ASGNB 12
line 965
;965:	AngleVectors( ent->client->ps.viewangles, vf, NULL, NULL );
ADDRFP4 0
INDIRP4
CNSTI4 408
ADDP4
INDIRP4
CNSTI4 156
ADDP4
ARGP4
ADDRLP4 1104
ARGP4
ADDRLP4 1132
CNSTP4 0
ASGNP4
ADDRLP4 1132
INDIRP4
ARGP4
ADDRLP4 1132
INDIRP4
ARGP4
ADDRGP4 AngleVectors
CALLV
pop
line 967
;966:
;967:	VectorMA( src, USE_DISTANCE, vf, dest );
ADDRLP4 1136
CNSTF4 1115684864
ASGNF4
ADDRLP4 1092
ADDRLP4 0
INDIRF4
ADDRLP4 1136
INDIRF4
ADDRLP4 1104
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 1092+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 1136
INDIRF4
ADDRLP4 1104+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 1092+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1115684864
ADDRLP4 1104+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 970
;968:
;969:	//Trace ahead to find a valid target
;970:	trap_Trace( &trace, src, vec3_origin, vec3_origin, dest, ent->s.number, MASK_OPAQUE|CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_ITEM|CONTENTS_CORPSE );
ADDRLP4 12
ARGP4
ADDRLP4 0
ARGP4
ADDRLP4 1140
ADDRGP4 vec3_origin
ASGNP4
ADDRLP4 1140
INDIRP4
ARGP4
ADDRLP4 1140
INDIRP4
ARGP4
ADDRLP4 1092
ARGP4
ADDRFP4 0
INDIRP4
INDIRI4
ARGI4
CNSTI4 1180419
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 972
;971:	
;972:	if ( trace.fraction == 1.0f || trace.entityNum < 1 )
ADDRLP4 12+8
INDIRF4
CNSTF4 1065353216
EQF4 $374
ADDRLP4 12+52
INDIRI4
CNSTI4 1
GEI4 $370
LABELV $374
line 973
;973:	{
line 974
;974:		return;
ADDRGP4 $362
JUMPV
LABELV $370
line 977
;975:	}
;976:
;977:	target = &g_entities[trace.entityNum];
ADDRLP4 1116
CNSTI4 828
ADDRLP4 12+52
INDIRI4
MULI4
ADDRGP4 g_entities
ADDP4
ASGNP4
line 980
;978:
;979:	//Check for a use command
;980:	if ( ValidUseTarget( target ) )
ADDRLP4 1116
INDIRP4
ARGP4
ADDRLP4 1144
ADDRGP4 ValidUseTarget
CALLI4
ASGNI4
ADDRLP4 1144
INDIRI4
CNSTI4 0
EQI4 $376
line 981
;981:	{
line 990
;982:		/*
;983:		NPC_SetAnim( ent, SETANIM_TORSO, BOTH_FORCEPUSH, SETANIM_FLAG_OVERRIDE|SETANIM_FLAG_HOLD );
;984:		if ( !VectorLengthSquared( ent->client->ps.velocity ) )
;985:		{
;986:			NPC_SetAnim( ent, SETANIM_LEGS, BOTH_FORCEPUSH, SETANIM_FLAG_NORMAL|SETANIM_FLAG_HOLD );
;987:		}
;988:		*/
;989:
;990:		target->use(target, ent, ent);
ADDRLP4 1116
INDIRP4
ARGP4
ADDRLP4 1152
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 1152
INDIRP4
ARGP4
ADDRLP4 1152
INDIRP4
ARGP4
ADDRLP4 1116
INDIRP4
CNSTI4 652
ADDP4
INDIRP4
CALLV
pop
line 991
;991:		return;
LABELV $376
line 993
;992:	}
;993:}
LABELV $362
endproc TryUse 1156 28
export G_PointInBounds
proc G_PointInBounds 12 0
line 996
;994:
;995:qboolean G_PointInBounds( vec3_t point, vec3_t mins, vec3_t maxs )
;996:{
line 999
;997:	int i;
;998:
;999:	for(i = 0; i < 3; i++ )
ADDRLP4 0
CNSTI4 0
ASGNI4
LABELV $379
line 1000
;1000:	{
line 1001
;1001:		if ( point[i] < mins[i] )
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 4
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRLP4 4
INDIRI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRF4
GEF4 $383
line 1002
;1002:		{
line 1003
;1003:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $378
JUMPV
LABELV $383
line 1005
;1004:		}
;1005:		if ( point[i] > maxs[i] )
ADDRLP4 8
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ASGNI4
ADDRLP4 8
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRF4
ADDRLP4 8
INDIRI4
ADDRFP4 8
INDIRP4
ADDP4
INDIRF4
LEF4 $385
line 1006
;1006:		{
line 1007
;1007:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $378
JUMPV
LABELV $385
line 1009
;1008:		}
;1009:	}
LABELV $380
line 999
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 3
LTI4 $379
line 1011
;1010:
;1011:	return qtrue;
CNSTI4 1
RETI4
LABELV $378
endproc G_PointInBounds 12 0
export G_BoxInBounds
proc G_BoxInBounds 56 0
line 1015
;1012:}
;1013:
;1014:qboolean G_BoxInBounds( vec3_t point, vec3_t mins, vec3_t maxs, vec3_t boundsMins, vec3_t boundsMaxs )
;1015:{
line 1019
;1016:	vec3_t boxMins;
;1017:	vec3_t boxMaxs;
;1018:
;1019:	VectorAdd( point, mins, boxMins );
ADDRLP4 24
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 28
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 24
INDIRP4
INDIRF4
ADDRLP4 28
INDIRP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 32
CNSTI4 4
ASGNI4
ADDRLP4 0+4
ADDRLP4 24
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
ADDRLP4 28
INDIRP4
ADDRLP4 32
INDIRI4
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 36
CNSTI4 8
ASGNI4
ADDRLP4 0+8
ADDRFP4 0
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRF4
ADDRFP4 4
INDIRP4
ADDRLP4 36
INDIRI4
ADDP4
INDIRF4
ADDF4
ASGNF4
line 1020
;1020:	VectorAdd( point, maxs, boxMaxs );
ADDRLP4 40
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 44
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 12
ADDRLP4 40
INDIRP4
INDIRF4
ADDRLP4 44
INDIRP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 48
CNSTI4 4
ASGNI4
ADDRLP4 12+4
ADDRLP4 40
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
ADDRLP4 44
INDIRP4
ADDRLP4 48
INDIRI4
ADDP4
INDIRF4
ADDF4
ASGNF4
ADDRLP4 52
CNSTI4 8
ASGNI4
ADDRLP4 12+8
ADDRFP4 0
INDIRP4
ADDRLP4 52
INDIRI4
ADDP4
INDIRF4
ADDRFP4 8
INDIRP4
ADDRLP4 52
INDIRI4
ADDP4
INDIRF4
ADDF4
ASGNF4
line 1022
;1021:
;1022:	if(boxMaxs[0]>boundsMaxs[0])
ADDRLP4 12
INDIRF4
ADDRFP4 16
INDIRP4
INDIRF4
LEF4 $392
line 1023
;1023:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $387
JUMPV
LABELV $392
line 1025
;1024:
;1025:	if(boxMaxs[1]>boundsMaxs[1])
ADDRLP4 12+4
INDIRF4
ADDRFP4 16
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
LEF4 $394
line 1026
;1026:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $387
JUMPV
LABELV $394
line 1028
;1027:
;1028:	if(boxMaxs[2]>boundsMaxs[2])
ADDRLP4 12+8
INDIRF4
ADDRFP4 16
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
LEF4 $397
line 1029
;1029:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $387
JUMPV
LABELV $397
line 1031
;1030:
;1031:	if(boxMins[0]<boundsMins[0])
ADDRLP4 0
INDIRF4
ADDRFP4 12
INDIRP4
INDIRF4
GEF4 $400
line 1032
;1032:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $387
JUMPV
LABELV $400
line 1034
;1033:
;1034:	if(boxMins[1]<boundsMins[1])
ADDRLP4 0+4
INDIRF4
ADDRFP4 12
INDIRP4
CNSTI4 4
ADDP4
INDIRF4
GEF4 $402
line 1035
;1035:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $387
JUMPV
LABELV $402
line 1037
;1036:
;1037:	if(boxMins[2]<boundsMins[2])
ADDRLP4 0+8
INDIRF4
ADDRFP4 12
INDIRP4
CNSTI4 8
ADDP4
INDIRF4
GEF4 $405
line 1038
;1038:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $387
JUMPV
LABELV $405
line 1041
;1039:
;1040:	//box is completely contained within bounds
;1041:	return qtrue;
CNSTI4 1
RETI4
LABELV $387
endproc G_BoxInBounds 56 0
export G_SetAngles
proc G_SetAngles 0 0
line 1046
;1042:}
;1043:
;1044:
;1045:void G_SetAngles( gentity_t *ent, vec3_t angles )
;1046:{
line 1047
;1047:	VectorCopy( angles, ent->r.currentAngles );
ADDRFP4 0
INDIRP4
CNSTI4 380
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 1048
;1048:	VectorCopy( angles, ent->s.angles );
ADDRFP4 0
INDIRP4
CNSTI4 116
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 1049
;1049:	VectorCopy( angles, ent->s.apos.trBase );
ADDRFP4 0
INDIRP4
CNSTI4 60
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 1050
;1050:}
LABELV $408
endproc G_SetAngles 0 0
bss
align 4
LABELV $410
skip 1080
export G_ClearTrace
code
proc G_ClearTrace 4 28
line 1053
;1051:
;1052:qboolean G_ClearTrace( vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int ignore, int clipmask )
;1053:{
line 1056
;1054:	static	trace_t	tr;
;1055:
;1056:	trap_Trace( &tr, start, mins, maxs, end, ignore, clipmask );
ADDRGP4 $410
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 12
INDIRP4
ARGP4
ADDRFP4 16
INDIRI4
ARGI4
ADDRFP4 20
INDIRI4
ARGI4
ADDRGP4 trap_Trace
CALLV
pop
line 1058
;1057:
;1058:	if ( tr.allsolid || tr.startsolid || tr.fraction < 1.0 )
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $410
INDIRI4
ADDRLP4 0
INDIRI4
NEI4 $416
ADDRGP4 $410+4
INDIRI4
ADDRLP4 0
INDIRI4
NEI4 $416
ADDRGP4 $410+8
INDIRF4
CNSTF4 1065353216
GEF4 $411
LABELV $416
line 1059
;1059:	{
line 1060
;1060:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $409
JUMPV
LABELV $411
line 1063
;1061:	}
;1062:
;1063:	return qtrue;
CNSTI4 1
RETI4
LABELV $409
endproc G_ClearTrace 4 28
export G_SetOrigin
proc G_SetOrigin 8 0
line 1074
;1064:}
;1065:
;1066:
;1067:/*
;1068:================
;1069:G_SetOrigin
;1070:
;1071:Sets the pos trajectory for a fixed position
;1072:================
;1073:*/
;1074:void G_SetOrigin( gentity_t *ent, vec3_t origin ) {
line 1075
;1075:	VectorCopy( origin, ent->s.pos.trBase );
ADDRFP4 0
INDIRP4
CNSTI4 24
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 1076
;1076:	ent->s.pos.trType = TR_STATIONARY;
ADDRFP4 0
INDIRP4
CNSTI4 12
ADDP4
CNSTI4 0
ASGNI4
line 1077
;1077:	ent->s.pos.trTime = 0;
ADDRFP4 0
INDIRP4
CNSTI4 16
ADDP4
CNSTI4 0
ASGNI4
line 1078
;1078:	ent->s.pos.trDuration = 0;
ADDRFP4 0
INDIRP4
CNSTI4 20
ADDP4
CNSTI4 0
ASGNI4
line 1079
;1079:	VectorClear( ent->s.pos.trDelta );
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 4
CNSTF4 0
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 44
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 40
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
ADDRLP4 0
INDIRP4
CNSTI4 36
ADDP4
ADDRLP4 4
INDIRF4
ASGNF4
line 1081
;1080:
;1081:	VectorCopy( origin, ent->r.currentOrigin );
ADDRFP4 0
INDIRP4
CNSTI4 368
ADDP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 1082
;1082:}
LABELV $417
endproc G_SetOrigin 8 0
lit
align 4
LABELV $419
byte 4 0
byte 4 0
byte 4 1065353216
export DebugLine
code
proc DebugLine 116 12
line 1092
;1083:
;1084:/*
;1085:================
;1086:DebugLine
;1087:
;1088:  debug polygons only work when running a local game
;1089:  with r_debugSurface set to 2
;1090:================
;1091:*/
;1092:int DebugLine(vec3_t start, vec3_t end, int color) {
line 1093
;1093:	vec3_t points[4], dir, cross, up = {0, 0, 1};
ADDRLP4 72
ADDRGP4 $419
INDIRB
ASGNB 12
line 1096
;1094:	float dot;
;1095:
;1096:	VectorCopy(start, points[0]);
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 1097
;1097:	VectorCopy(start, points[1]);
ADDRLP4 0+12
ADDRFP4 0
INDIRP4
INDIRB
ASGNB 12
line 1099
;1098:	//points[1][2] -= 2;
;1099:	VectorCopy(end, points[2]);
ADDRLP4 0+24
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 1101
;1100:	//points[2][2] -= 2;
;1101:	VectorCopy(end, points[3]);
ADDRLP4 0+36
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 12
line 1104
;1102:
;1103:
;1104:	VectorSubtract(end, start, dir);
ADDRLP4 88
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 92
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 60
ADDRLP4 88
INDIRP4
INDIRF4
ADDRLP4 92
INDIRP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 96
CNSTI4 4
ASGNI4
ADDRLP4 60+4
ADDRLP4 88
INDIRP4
ADDRLP4 96
INDIRI4
ADDP4
INDIRF4
ADDRLP4 92
INDIRP4
ADDRLP4 96
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
ADDRLP4 100
CNSTI4 8
ASGNI4
ADDRLP4 60+8
ADDRFP4 4
INDIRP4
ADDRLP4 100
INDIRI4
ADDP4
INDIRF4
ADDRFP4 0
INDIRP4
ADDRLP4 100
INDIRI4
ADDP4
INDIRF4
SUBF4
ASGNF4
line 1105
;1105:	VectorNormalize(dir);
ADDRLP4 60
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 1106
;1106:	dot = DotProduct(dir, up);
ADDRLP4 84
ADDRLP4 60
INDIRF4
ADDRLP4 72
INDIRF4
MULF4
ADDRLP4 60+4
INDIRF4
ADDRLP4 72+4
INDIRF4
MULF4
ADDF4
ADDRLP4 60+8
INDIRF4
ADDRLP4 72+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 1107
;1107:	if (dot > 0.99 || dot < -0.99) VectorSet(cross, 1, 0, 0);
ADDRLP4 84
INDIRF4
CNSTF4 1065185444
GTF4 $431
ADDRLP4 84
INDIRF4
CNSTF4 3212669092
GEF4 $429
LABELV $431
ADDRLP4 48
CNSTF4 1065353216
ASGNF4
ADDRLP4 48+4
CNSTF4 0
ASGNF4
ADDRLP4 48+8
CNSTF4 0
ASGNF4
ADDRGP4 $430
JUMPV
LABELV $429
line 1108
;1108:	else CrossProduct(dir, up, cross);
ADDRLP4 60
ARGP4
ADDRLP4 72
ARGP4
ADDRLP4 48
ARGP4
ADDRGP4 CrossProduct
CALLV
pop
LABELV $430
line 1110
;1109:
;1110:	VectorNormalize(cross);
ADDRLP4 48
ARGP4
ADDRGP4 VectorNormalize
CALLF4
pop
line 1112
;1111:
;1112:	VectorMA(points[0], 2, cross, points[0]);
ADDRLP4 108
CNSTF4 1073741824
ASGNF4
ADDRLP4 0
ADDRLP4 0
INDIRF4
ADDRLP4 108
INDIRF4
ADDRLP4 48
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+4
ADDRLP4 0+4
INDIRF4
ADDRLP4 108
INDIRF4
ADDRLP4 48+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+8
ADDRLP4 0+8
INDIRF4
CNSTF4 1073741824
ADDRLP4 48+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 1113
;1113:	VectorMA(points[1], -2, cross, points[1]);
ADDRLP4 0+12
ADDRLP4 0+12
INDIRF4
CNSTF4 3221225472
ADDRLP4 48
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+12+4
ADDRLP4 0+12+4
INDIRF4
CNSTF4 3221225472
ADDRLP4 48+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+12+8
ADDRLP4 0+12+8
INDIRF4
CNSTF4 3221225472
ADDRLP4 48+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 1114
;1114:	VectorMA(points[2], -2, cross, points[2]);
ADDRLP4 0+24
ADDRLP4 0+24
INDIRF4
CNSTF4 3221225472
ADDRLP4 48
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+24+4
ADDRLP4 0+24+4
INDIRF4
CNSTF4 3221225472
ADDRLP4 48+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+24+8
ADDRLP4 0+24+8
INDIRF4
CNSTF4 3221225472
ADDRLP4 48+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 1115
;1115:	VectorMA(points[3], 2, cross, points[3]);
ADDRLP4 0+36
ADDRLP4 0+36
INDIRF4
CNSTF4 1073741824
ADDRLP4 48
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+36+4
ADDRLP4 0+36+4
INDIRF4
CNSTF4 1073741824
ADDRLP4 48+4
INDIRF4
MULF4
ADDF4
ASGNF4
ADDRLP4 0+36+8
ADDRLP4 0+36+8
INDIRF4
CNSTF4 1073741824
ADDRLP4 48+8
INDIRF4
MULF4
ADDF4
ASGNF4
line 1117
;1116:
;1117:	return trap_DebugPolygonCreate(color, 4, points);
ADDRFP4 8
INDIRI4
ARGI4
CNSTI4 4
ARGI4
ADDRLP4 0
ARGP4
ADDRLP4 112
ADDRGP4 trap_DebugPolygonCreate
CALLI4
ASGNI4
ADDRLP4 112
INDIRI4
RETI4
LABELV $418
endproc DebugLine 116 12
export G_ROFF_NotetrackCallback
proc G_ROFF_NotetrackCallback 296 12
line 1121
;1118:}
;1119:
;1120:void G_ROFF_NotetrackCallback( gentity_t *cent, const char *notetrack)
;1121:{
line 1123
;1122:	char type[256];
;1123:	int i = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
line 1124
;1124:	int addlArg = 0;
ADDRLP4 260
CNSTI4 0
ASGNI4
line 1126
;1125:
;1126:	if (!cent || !notetrack)
ADDRLP4 264
CNSTU4 0
ASGNU4
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRLP4 264
INDIRU4
EQU4 $479
ADDRFP4 4
INDIRP4
CVPU4 4
ADDRLP4 264
INDIRU4
NEU4 $481
LABELV $479
line 1127
;1127:	{
line 1128
;1128:		return;
ADDRGP4 $476
JUMPV
LABELV $480
line 1132
;1129:	}
;1130:
;1131:	while (notetrack[i] && notetrack[i] != ' ')
;1132:	{
line 1133
;1133:		type[i] = notetrack[i];
ADDRLP4 0
INDIRI4
ADDRLP4 4
ADDP4
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRI1
ASGNI1
line 1134
;1134:		i++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 1135
;1135:	}
LABELV $481
line 1131
ADDRLP4 268
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 268
INDIRI4
CNSTI4 0
EQI4 $483
ADDRLP4 268
INDIRI4
CNSTI4 32
NEI4 $480
LABELV $483
line 1137
;1136:
;1137:	type[i] = '\0';
ADDRLP4 0
INDIRI4
ADDRLP4 4
ADDP4
CNSTI1 0
ASGNI1
line 1139
;1138:
;1139:	if (!i || !type[0])
ADDRLP4 272
CNSTI4 0
ASGNI4
ADDRLP4 0
INDIRI4
ADDRLP4 272
INDIRI4
EQI4 $486
ADDRLP4 4
INDIRI1
CVII4 1
ADDRLP4 272
INDIRI4
NEI4 $484
LABELV $486
line 1140
;1140:	{
line 1141
;1141:		return;
ADDRGP4 $476
JUMPV
LABELV $484
line 1144
;1142:	}
;1143:
;1144:	if (notetrack[i] == ' ')
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRP4
ADDP4
INDIRI1
CVII4 1
CNSTI4 32
NEI4 $487
line 1145
;1145:	{
line 1146
;1146:		addlArg = 1;
ADDRLP4 260
CNSTI4 1
ASGNI4
line 1147
;1147:	}
LABELV $487
line 1149
;1148:
;1149:	if (strcmp(type, "loop") == 0)
ADDRLP4 4
ARGP4
ADDRGP4 $491
ARGP4
ADDRLP4 276
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 276
INDIRI4
CNSTI4 0
NEI4 $489
line 1150
;1150:	{
line 1151
;1151:		if (addlArg) //including an additional argument means reset to original position before loop
ADDRLP4 260
INDIRI4
CNSTI4 0
EQI4 $492
line 1152
;1152:		{
line 1153
;1153:			VectorCopy(cent->s.origin2, cent->s.pos.trBase);
ADDRLP4 280
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 280
INDIRP4
CNSTI4 24
ADDP4
ADDRLP4 280
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 1154
;1154:			VectorCopy(cent->s.origin2, cent->r.currentOrigin);
ADDRLP4 284
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 284
INDIRP4
CNSTI4 368
ADDP4
ADDRLP4 284
INDIRP4
CNSTI4 104
ADDP4
INDIRB
ASGNB 12
line 1155
;1155:			VectorCopy(cent->s.angles2, cent->s.apos.trBase);
ADDRLP4 288
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 288
INDIRP4
CNSTI4 60
ADDP4
ADDRLP4 288
INDIRP4
CNSTI4 128
ADDP4
INDIRB
ASGNB 12
line 1156
;1156:			VectorCopy(cent->s.angles2, cent->r.currentAngles);
ADDRLP4 292
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 292
INDIRP4
CNSTI4 380
ADDP4
ADDRLP4 292
INDIRP4
CNSTI4 128
ADDP4
INDIRB
ASGNB 12
line 1157
;1157:		}
LABELV $492
line 1159
;1158:
;1159:		trap_ROFF_Play(cent->s.number, cent->roffid, qfalse);
ADDRLP4 280
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 280
INDIRP4
INDIRI4
ARGI4
ADDRLP4 280
INDIRP4
CNSTI4 464
ADDP4
INDIRI4
ARGI4
CNSTI4 0
ARGI4
ADDRGP4 trap_ROFF_Play
CALLI4
pop
line 1160
;1160:	}
LABELV $489
line 1161
;1161:}
LABELV $476
endproc G_ROFF_NotetrackCallback 296 12
bss
export gG2KillIndex
align 4
LABELV gG2KillIndex
skip 256
export remappedShaders
align 4
LABELV remappedShaders
skip 16896
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
import G_RunObject
import G_TouchSolids
import G_TouchTriggers
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
LABELV $491
char 1 108
char 1 111
char 1 111
char 1 112
char 1 0
align 1
LABELV $332
char 1 71
char 1 95
char 1 65
char 1 100
char 1 100
char 1 69
char 1 118
char 1 101
char 1 110
char 1 116
char 1 58
char 1 32
char 1 122
char 1 101
char 1 114
char 1 111
char 1 32
char 1 101
char 1 118
char 1 101
char 1 110
char 1 116
char 1 32
char 1 97
char 1 100
char 1 100
char 1 101
char 1 100
char 1 32
char 1 102
char 1 111
char 1 114
char 1 32
char 1 101
char 1 110
char 1 116
char 1 105
char 1 116
char 1 121
char 1 32
char 1 37
char 1 105
char 1 10
char 1 0
align 1
LABELV $297
char 1 116
char 1 101
char 1 109
char 1 112
char 1 69
char 1 110
char 1 116
char 1 105
char 1 116
char 1 121
char 1 0
align 1
LABELV $294
char 1 102
char 1 114
char 1 101
char 1 101
char 1 100
char 1 0
align 1
LABELV $274
char 1 107
char 1 103
char 1 50
char 1 32
char 1 37
char 1 105
char 1 0
align 1
LABELV $270
char 1 32
char 1 37
char 1 105
char 1 0
align 1
LABELV $266
char 1 107
char 1 103
char 1 50
char 1 0
align 1
LABELV $250
char 1 71
char 1 95
char 1 83
char 1 112
char 1 97
char 1 119
char 1 110
char 1 58
char 1 32
char 1 110
char 1 111
char 1 32
char 1 102
char 1 114
char 1 101
char 1 101
char 1 32
char 1 101
char 1 110
char 1 116
char 1 105
char 1 116
char 1 105
char 1 101
char 1 115
char 1 0
align 1
LABELV $248
char 1 37
char 1 52
char 1 105
char 1 58
char 1 32
char 1 37
char 1 115
char 1 10
char 1 0
align 1
LABELV $222
char 1 110
char 1 111
char 1 99
char 1 108
char 1 97
char 1 115
char 1 115
char 1 0
align 1
LABELV $211
char 1 40
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 32
char 1 37
char 1 105
char 1 41
char 1 0
align 1
LABELV $204
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
LABELV $199
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
LABELV $185
char 1 71
char 1 95
char 1 80
char 1 105
char 1 99
char 1 107
char 1 84
char 1 97
char 1 114
char 1 103
char 1 101
char 1 116
char 1 58
char 1 32
char 1 116
char 1 97
char 1 114
char 1 103
char 1 101
char 1 116
char 1 32
char 1 37
char 1 115
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
char 1 10
char 1 0
align 1
LABELV $175
char 1 71
char 1 95
char 1 80
char 1 105
char 1 99
char 1 107
char 1 84
char 1 97
char 1 114
char 1 103
char 1 101
char 1 116
char 1 32
char 1 99
char 1 97
char 1 108
char 1 108
char 1 101
char 1 100
char 1 32
char 1 119
char 1 105
char 1 116
char 1 104
char 1 32
char 1 78
char 1 85
char 1 76
char 1 76
char 1 32
char 1 116
char 1 97
char 1 114
char 1 103
char 1 101
char 1 116
char 1 110
char 1 97
char 1 109
char 1 101
char 1 10
char 1 0
align 1
LABELV $132
char 1 37
char 1 115
char 1 0
align 1
LABELV $117
char 1 71
char 1 95
char 1 70
char 1 105
char 1 110
char 1 100
char 1 67
char 1 111
char 1 110
char 1 102
char 1 105
char 1 103
char 1 115
char 1 116
char 1 114
char 1 105
char 1 110
char 1 103
char 1 73
char 1 110
char 1 100
char 1 101
char 1 120
char 1 58
char 1 32
char 1 111
char 1 118
char 1 101
char 1 114
char 1 102
char 1 108
char 1 111
char 1 119
char 1 0
align 1
LABELV $98
char 1 37
char 1 115
char 1 61
char 1 37
char 1 115
char 1 58
char 1 37
char 1 53
char 1 46
char 1 50
char 1 102
char 1 64
char 1 0
