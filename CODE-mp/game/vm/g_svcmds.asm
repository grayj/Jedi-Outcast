code
proc StringToFilter 160 8
file "../g_svcmds.c"
line 58
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:
;4:// this file holds commands that can be executed by the server console, but not remote clients
;5:
;6:#include "g_local.h"
;7:
;8:
;9:/*
;10:==============================================================================
;11:
;12:PACKET FILTERING
;13: 
;14:
;15:You can add or remove addresses from the filter list with:
;16:
;17:addip <ip>
;18:removeip <ip>
;19:
;20:The ip address is specified in dot format, and any unspecified digits will match any value, so you can specify an entire class C network with "addip 192.246.40".
;21:
;22:Removeip will only remove an address specified exactly the same way.  You cannot addip a subnet, then removeip a single host.
;23:
;24:listip
;25:Prints the current list of filters.
;26:
;27:g_filterban <0 or 1>
;28:
;29:If 1 (the default), then ip addresses matching the current list will be prohibited from entering the game.  This is the default setting.
;30:
;31:If 0, then only addresses matching the list will be allowed.  This lets you easily set up a private game, or a game that only allows players from your local network.
;32:
;33:
;34:==============================================================================
;35:*/
;36:
;37:// extern	vmCvar_t	g_banIPs;
;38:// extern	vmCvar_t	g_filterBan;
;39:
;40:
;41:typedef struct ipFilter_s
;42:{
;43:	unsigned	mask;
;44:	unsigned	compare;
;45:} ipFilter_t;
;46:
;47:#define	MAX_IPFILTERS	1024
;48:
;49:static ipFilter_t	ipFilters[MAX_IPFILTERS];
;50:static int			numIPFilters;
;51:
;52:/*
;53:=================
;54:StringToFilter
;55:=================
;56:*/
;57:static qboolean StringToFilter (char *s, ipFilter_t *f)
;58:{
line 64
;59:	char	num[128];
;60:	int		i, j;
;61:	byte	b[4];
;62:	byte	m[4];
;63:	
;64:	for (i=0 ; i<4 ; i++)
ADDRLP4 132
CNSTI4 0
ASGNI4
LABELV $79
line 65
;65:	{
line 66
;66:		b[i] = 0;
ADDRLP4 132
INDIRI4
ADDRLP4 136
ADDP4
CNSTU1 0
ASGNU1
line 67
;67:		m[i] = 0;
ADDRLP4 132
INDIRI4
ADDRLP4 140
ADDP4
CNSTU1 0
ASGNU1
line 68
;68:	}
LABELV $80
line 64
ADDRLP4 132
ADDRLP4 132
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 132
INDIRI4
CNSTI4 4
LTI4 $79
line 70
;69:	
;70:	for (i=0 ; i<4 ; i++)
ADDRLP4 132
CNSTI4 0
ASGNI4
LABELV $83
line 71
;71:	{
line 72
;72:		if (*s < '0' || *s > '9')
ADDRLP4 144
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 144
INDIRI4
CNSTI4 48
LTI4 $89
ADDRLP4 144
INDIRI4
CNSTI4 57
LEI4 $87
LABELV $89
line 73
;73:		{
line 74
;74:			G_Printf( "Bad filter address: %s\n", s );
ADDRGP4 $90
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 75
;75:			return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $78
JUMPV
LABELV $87
line 78
;76:		}
;77:		
;78:		j = 0;
ADDRLP4 128
CNSTI4 0
ASGNI4
ADDRGP4 $92
JUMPV
LABELV $91
line 80
;79:		while (*s >= '0' && *s <= '9')
;80:		{
line 81
;81:			num[j++] = *s++;
ADDRLP4 148
ADDRLP4 128
INDIRI4
ASGNI4
ADDRLP4 156
CNSTI4 1
ASGNI4
ADDRLP4 128
ADDRLP4 148
INDIRI4
ADDRLP4 156
INDIRI4
ADDI4
ASGNI4
ADDRLP4 152
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 152
INDIRP4
ADDRLP4 156
INDIRI4
ADDP4
ASGNP4
ADDRLP4 148
INDIRI4
ADDRLP4 0
ADDP4
ADDRLP4 152
INDIRP4
INDIRI1
ASGNI1
line 82
;82:		}
LABELV $92
line 79
ADDRLP4 148
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 148
INDIRI4
CNSTI4 48
LTI4 $94
ADDRLP4 148
INDIRI4
CNSTI4 57
LEI4 $91
LABELV $94
line 83
;83:		num[j] = 0;
ADDRLP4 128
INDIRI4
ADDRLP4 0
ADDP4
CNSTI1 0
ASGNI1
line 84
;84:		b[i] = atoi(num);
ADDRLP4 0
ARGP4
ADDRLP4 152
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 132
INDIRI4
ADDRLP4 136
ADDP4
ADDRLP4 152
INDIRI4
CVIU4 4
CVUU1 4
ASGNU1
line 85
;85:		if (b[i] != 0)
ADDRLP4 132
INDIRI4
ADDRLP4 136
ADDP4
INDIRU1
CVUI4 1
CNSTI4 0
EQI4 $95
line 86
;86:			m[i] = 255;
ADDRLP4 132
INDIRI4
ADDRLP4 140
ADDP4
CNSTU1 255
ASGNU1
LABELV $95
line 88
;87:
;88:		if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $97
line 89
;89:			break;
ADDRGP4 $85
JUMPV
LABELV $97
line 90
;90:		s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 91
;91:	}
LABELV $84
line 70
ADDRLP4 132
ADDRLP4 132
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
ADDRLP4 132
INDIRI4
CNSTI4 4
LTI4 $83
LABELV $85
line 93
;92:	
;93:	f->mask = *(unsigned *)m;
ADDRFP4 4
INDIRP4
ADDRLP4 140
INDIRU4
ASGNU4
line 94
;94:	f->compare = *(unsigned *)b;
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
ADDRLP4 136
INDIRU4
ASGNU4
line 96
;95:	
;96:	return qtrue;
CNSTI4 1
RETI4
LABELV $78
endproc StringToFilter 160 8
proc UpdateIPBans 1040 28
line 105
;97:}
;98:
;99:/*
;100:=================
;101:UpdateIPBans
;102:=================
;103:*/
;104:static void UpdateIPBans (void)
;105:{
line 110
;106:	byte	b[4];
;107:	int		i;
;108:	char	iplist[MAX_INFO_STRING];
;109:
;110:	*iplist = 0;
ADDRLP4 4
CNSTI1 0
ASGNI1
line 111
;111:	for (i = 0 ; i < numIPFilters ; i++)
ADDRLP4 1028
CNSTI4 0
ASGNI4
ADDRGP4 $103
JUMPV
LABELV $100
line 112
;112:	{
line 113
;113:		if (ipFilters[i].compare == 0xffffffff)
ADDRLP4 1028
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 ipFilters+4
ADDP4
INDIRU4
CNSTU4 4294967295
NEU4 $104
line 114
;114:			continue;
ADDRGP4 $101
JUMPV
LABELV $104
line 116
;115:
;116:		*(unsigned *)b = ipFilters[i].compare;
ADDRLP4 0
ADDRLP4 1028
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 ipFilters+4
ADDP4
INDIRU4
ASGNU4
line 117
;117:		Com_sprintf( iplist + strlen(iplist), sizeof(iplist) - strlen(iplist), 
ADDRLP4 4
ARGP4
ADDRLP4 1032
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 4
ARGP4
ADDRLP4 1036
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1032
INDIRI4
ADDRLP4 4
ADDP4
ARGP4
CNSTU4 1024
ADDRLP4 1036
INDIRI4
CVIU4 4
SUBU4
CVUI4 4
ARGI4
ADDRGP4 $108
ARGP4
ADDRLP4 0
INDIRU1
CVUI4 1
ARGI4
ADDRLP4 0+1
INDIRU1
CVUI4 1
ARGI4
ADDRLP4 0+2
INDIRU1
CVUI4 1
ARGI4
ADDRLP4 0+3
INDIRU1
CVUI4 1
ARGI4
ADDRGP4 Com_sprintf
CALLV
pop
line 119
;118:			"%i.%i.%i.%i ", b[0], b[1], b[2], b[3]);
;119:	}
LABELV $101
line 111
ADDRLP4 1028
ADDRLP4 1028
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $103
ADDRLP4 1028
INDIRI4
ADDRGP4 numIPFilters
INDIRI4
LTI4 $100
line 121
;120:
;121:	trap_Cvar_Set( "g_banIPs", iplist );
ADDRGP4 $112
ARGP4
ADDRLP4 4
ARGP4
ADDRGP4 trap_Cvar_Set
CALLV
pop
line 122
;122:}
LABELV $99
endproc UpdateIPBans 1040 28
lit
align 1
LABELV $114
char 1 0
char 1 0
char 1 0
char 1 0
export G_FilterPacket
code
proc G_FilterPacket 28 0
line 130
;123:
;124:/*
;125:=================
;126:G_FilterPacket
;127:=================
;128:*/
;129:qboolean G_FilterPacket (char *from)
;130:{
line 133
;131:	int		i;
;132:	unsigned	in;
;133:	byte m[4] = {'\0','\0','\0','\0'};
ADDRLP4 8
ADDRGP4 $114
INDIRB
ASGNB 4
line 136
;134:	char *p;
;135:
;136:	i = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 137
;137:	p = from;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $116
JUMPV
LABELV $115
line 138
;138:	while (*p && i < 4) {
line 139
;139:		m[i] = 0;
ADDRLP4 4
INDIRI4
ADDRLP4 8
ADDP4
CNSTU1 0
ASGNU1
ADDRGP4 $119
JUMPV
LABELV $118
line 140
;140:		while (*p >= '0' && *p <= '9') {
line 141
;141:			m[i] = m[i]*10 + (*p - '0');
ADDRLP4 16
ADDRLP4 4
INDIRI4
ADDRLP4 8
ADDP4
ASGNP4
ADDRLP4 16
INDIRP4
CNSTI4 10
ADDRLP4 16
INDIRP4
INDIRU1
CVUI4 1
MULI4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 48
SUBI4
ADDI4
CVIU4 4
CVUU1 4
ASGNU1
line 142
;142:			p++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 143
;143:		}
LABELV $119
line 140
ADDRLP4 16
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 48
LTI4 $121
ADDRLP4 16
INDIRI4
CNSTI4 57
LEI4 $118
LABELV $121
line 144
;144:		if (!*p || *p == ':')
ADDRLP4 20
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $124
ADDRLP4 20
INDIRI4
CNSTI4 58
NEI4 $122
LABELV $124
line 145
;145:			break;
ADDRGP4 $117
JUMPV
LABELV $122
line 146
;146:		i++, p++;
ADDRLP4 24
CNSTI4 1
ASGNI4
ADDRLP4 4
ADDRLP4 4
INDIRI4
ADDRLP4 24
INDIRI4
ADDI4
ASGNI4
ADDRLP4 0
ADDRLP4 0
INDIRP4
ADDRLP4 24
INDIRI4
ADDP4
ASGNP4
line 147
;147:	}
LABELV $116
line 138
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $125
ADDRLP4 4
INDIRI4
CNSTI4 4
LTI4 $115
LABELV $125
LABELV $117
line 149
;148:	
;149:	in = *(unsigned *)m;
ADDRLP4 12
ADDRLP4 8
INDIRU4
ASGNU4
line 151
;150:
;151:	for (i=0 ; i<numIPFilters ; i++)
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $129
JUMPV
LABELV $126
line 152
;152:		if ( (in & ipFilters[i].mask) == ipFilters[i].compare)
ADDRLP4 16
ADDRLP4 4
INDIRI4
CNSTI4 3
LSHI4
ASGNI4
ADDRLP4 12
INDIRU4
ADDRLP4 16
INDIRI4
ADDRGP4 ipFilters
ADDP4
INDIRU4
BANDU4
ADDRLP4 16
INDIRI4
ADDRGP4 ipFilters+4
ADDP4
INDIRU4
NEU4 $130
line 153
;153:			return g_filterBan.integer != 0;
ADDRGP4 g_filterBan+12
INDIRI4
CNSTI4 0
EQI4 $135
ADDRLP4 20
CNSTI4 1
ASGNI4
ADDRGP4 $136
JUMPV
LABELV $135
ADDRLP4 20
CNSTI4 0
ASGNI4
LABELV $136
ADDRLP4 20
INDIRI4
RETI4
ADDRGP4 $113
JUMPV
LABELV $130
LABELV $127
line 151
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $129
ADDRLP4 4
INDIRI4
ADDRGP4 numIPFilters
INDIRI4
LTI4 $126
line 155
;154:
;155:	return g_filterBan.integer == 0;
ADDRGP4 g_filterBan+12
INDIRI4
CNSTI4 0
NEI4 $139
ADDRLP4 24
CNSTI4 1
ASGNI4
ADDRGP4 $140
JUMPV
LABELV $139
ADDRLP4 24
CNSTI4 0
ASGNI4
LABELV $140
ADDRLP4 24
INDIRI4
RETI4
LABELV $113
endproc G_FilterPacket 28 0
proc AddIP 8 8
line 164
;156:}
;157:
;158:/*
;159:=================
;160:AddIP
;161:=================
;162:*/
;163:static void AddIP( char *str )
;164:{
line 167
;165:	int		i;
;166:
;167:	for (i = 0 ; i < numIPFilters ; i++)
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $145
JUMPV
LABELV $142
line 168
;168:		if (ipFilters[i].compare == 0xffffffff)
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 ipFilters+4
ADDP4
INDIRU4
CNSTU4 4294967295
NEU4 $146
line 169
;169:			break;		// free spot
ADDRGP4 $144
JUMPV
LABELV $146
LABELV $143
line 167
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $145
ADDRLP4 0
INDIRI4
ADDRGP4 numIPFilters
INDIRI4
LTI4 $142
LABELV $144
line 170
;170:	if (i == numIPFilters)
ADDRLP4 0
INDIRI4
ADDRGP4 numIPFilters
INDIRI4
NEI4 $149
line 171
;171:	{
line 172
;172:		if (numIPFilters == MAX_IPFILTERS)
ADDRGP4 numIPFilters
INDIRI4
CNSTI4 1024
NEI4 $151
line 173
;173:		{
line 174
;174:			G_Printf ("IP filter list is full\n");
ADDRGP4 $153
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 175
;175:			return;
ADDRGP4 $141
JUMPV
LABELV $151
line 177
;176:		}
;177:		numIPFilters++;
ADDRLP4 4
ADDRGP4 numIPFilters
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 178
;178:	}
LABELV $149
line 180
;179:	
;180:	if (!StringToFilter (str, &ipFilters[i]))
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 ipFilters
ADDP4
ARGP4
ADDRLP4 4
ADDRGP4 StringToFilter
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $154
line 181
;181:		ipFilters[i].compare = 0xffffffffu;
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 ipFilters+4
ADDP4
CNSTU4 4294967295
ASGNU4
LABELV $154
line 183
;182:
;183:	UpdateIPBans();
ADDRGP4 UpdateIPBans
CALLV
pop
line 184
;184:}
LABELV $141
endproc AddIP 8 8
export G_ProcessIPBans
proc G_ProcessIPBans 1044 12
line 192
;185:
;186:/*
;187:=================
;188:G_ProcessIPBans
;189:=================
;190:*/
;191:void G_ProcessIPBans(void) 
;192:{
line 196
;193:	char *s, *t;
;194:	char		str[MAX_TOKEN_CHARS];
;195:
;196:	Q_strncpyz( str, g_banIPs.string, sizeof(str) );
ADDRLP4 8
ARGP4
ADDRGP4 g_banIPs+16
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 198
;197:
;198:	for (t = s = g_banIPs.string; *t; /* */ ) {
ADDRLP4 1032
ADDRGP4 g_banIPs+16
ASGNP4
ADDRLP4 0
ADDRLP4 1032
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 1032
INDIRP4
ASGNP4
ADDRGP4 $162
JUMPV
LABELV $159
line 199
;199:		s = strchr(s, ' ');
ADDRLP4 0
INDIRP4
ARGP4
CNSTI4 32
ARGI4
ADDRLP4 1036
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 1036
INDIRP4
ASGNP4
line 200
;200:		if (!s)
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $167
line 201
;201:			break;
ADDRGP4 $161
JUMPV
LABELV $166
line 203
;202:		while (*s == ' ')
;203:			*s++ = 0;
ADDRLP4 1040
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 1040
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 1040
INDIRP4
CNSTI1 0
ASGNI1
LABELV $167
line 202
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 32
EQI4 $166
line 204
;204:		if (*t)
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $169
line 205
;205:			AddIP( t );
ADDRLP4 4
INDIRP4
ARGP4
ADDRGP4 AddIP
CALLV
pop
LABELV $169
line 206
;206:		t = s;
ADDRLP4 4
ADDRLP4 0
INDIRP4
ASGNP4
line 207
;207:	}
LABELV $160
line 198
LABELV $162
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $159
LABELV $161
line 208
;208:}
LABELV $157
endproc G_ProcessIPBans 1044 12
export Svcmd_AddIP_f
proc Svcmd_AddIP_f 1028 12
line 217
;209:
;210:
;211:/*
;212:=================
;213:Svcmd_AddIP_f
;214:=================
;215:*/
;216:void Svcmd_AddIP_f (void)
;217:{
line 220
;218:	char		str[MAX_TOKEN_CHARS];
;219:
;220:	if ( trap_Argc() < 2 ) {
ADDRLP4 1024
ADDRGP4 trap_Argc
CALLI4
ASGNI4
ADDRLP4 1024
INDIRI4
CNSTI4 2
GEI4 $172
line 221
;221:		G_Printf("Usage:  addip <ip-mask>\n");
ADDRGP4 $174
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 222
;222:		return;
ADDRGP4 $171
JUMPV
LABELV $172
line 225
;223:	}
;224:
;225:	trap_Argv( 1, str, sizeof( str ) );
CNSTI4 1
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 227
;226:
;227:	AddIP( str );
ADDRLP4 0
ARGP4
ADDRGP4 AddIP
CALLV
pop
line 229
;228:
;229:}
LABELV $171
endproc Svcmd_AddIP_f 1028 12
export Svcmd_RemoveIP_f
proc Svcmd_RemoveIP_f 1048 12
line 237
;230:
;231:/*
;232:=================
;233:Svcmd_RemoveIP_f
;234:=================
;235:*/
;236:void Svcmd_RemoveIP_f (void)
;237:{
line 242
;238:	ipFilter_t	f;
;239:	int			i;
;240:	char		str[MAX_TOKEN_CHARS];
;241:
;242:	if ( trap_Argc() < 2 ) {
ADDRLP4 1036
ADDRGP4 trap_Argc
CALLI4
ASGNI4
ADDRLP4 1036
INDIRI4
CNSTI4 2
GEI4 $176
line 243
;243:		G_Printf("Usage:  sv removeip <ip-mask>\n");
ADDRGP4 $178
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 244
;244:		return;
ADDRGP4 $175
JUMPV
LABELV $176
line 247
;245:	}
;246:
;247:	trap_Argv( 1, str, sizeof( str ) );
CNSTI4 1
ARGI4
ADDRLP4 12
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 249
;248:
;249:	if (!StringToFilter (str, &f))
ADDRLP4 12
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 1040
ADDRGP4 StringToFilter
CALLI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $179
line 250
;250:		return;
ADDRGP4 $175
JUMPV
LABELV $179
line 252
;251:
;252:	for (i=0 ; i<numIPFilters ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $184
JUMPV
LABELV $181
line 253
;253:		if (ipFilters[i].mask == f.mask	&&
ADDRLP4 1044
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ASGNI4
ADDRLP4 1044
INDIRI4
ADDRGP4 ipFilters
ADDP4
INDIRU4
ADDRLP4 4
INDIRU4
NEU4 $185
ADDRLP4 1044
INDIRI4
ADDRGP4 ipFilters+4
ADDP4
INDIRU4
ADDRLP4 4+4
INDIRU4
NEU4 $185
line 254
;254:			ipFilters[i].compare == f.compare) {
line 255
;255:			ipFilters[i].compare = 0xffffffffu;
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRGP4 ipFilters+4
ADDP4
CNSTU4 4294967295
ASGNU4
line 256
;256:			G_Printf ("Removed.\n");
ADDRGP4 $190
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 258
;257:
;258:			UpdateIPBans();
ADDRGP4 UpdateIPBans
CALLV
pop
line 259
;259:			return;
ADDRGP4 $175
JUMPV
LABELV $185
line 261
;260:		}
;261:	}
LABELV $182
line 252
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $184
ADDRLP4 0
INDIRI4
ADDRGP4 numIPFilters
INDIRI4
LTI4 $181
line 263
;262:
;263:	G_Printf ( "Didn't find %s.\n", str );
ADDRGP4 $191
ARGP4
ADDRLP4 12
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 264
;264:}
LABELV $175
endproc Svcmd_RemoveIP_f 1048 12
export Svcmd_EntityList_f
proc Svcmd_EntityList_f 16 8
line 271
;265:
;266:/*
;267:===================
;268:Svcmd_EntityList_f
;269:===================
;270:*/
;271:void	Svcmd_EntityList_f (void) {
line 275
;272:	int			e;
;273:	gentity_t		*check;
;274:
;275:	check = g_entities+1;
ADDRLP4 0
ADDRGP4 g_entities+828
ASGNP4
line 276
;276:	for (e = 1; e < level.num_entities ; e++, check++) {
ADDRLP4 4
CNSTI4 1
ASGNI4
ADDRGP4 $197
JUMPV
LABELV $194
line 277
;277:		if ( !check->inuse ) {
ADDRLP4 0
INDIRP4
CNSTI4 412
ADDP4
INDIRI4
CNSTI4 0
NEI4 $199
line 278
;278:			continue;
ADDRGP4 $195
JUMPV
LABELV $199
line 280
;279:		}
;280:		G_Printf("%3i:", e);
ADDRGP4 $201
ARGP4
ADDRLP4 4
INDIRI4
ARGI4
ADDRGP4 G_Printf
CALLV
pop
line 281
;281:		switch ( check->s.eType ) {
ADDRLP4 8
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
LTI4 $202
ADDRLP4 8
INDIRI4
CNSTI4 13
GTI4 $202
ADDRLP4 8
INDIRI4
CNSTI4 2
LSHI4
ADDRGP4 $230
ADDP4
INDIRP4
JUMPV
lit
align 4
LABELV $230
address $205
address $207
address $209
address $211
address $202
address $202
address $213
address $215
address $217
address $219
address $221
address $223
address $225
address $227
code
LABELV $205
line 283
;282:		case ET_GENERAL:
;283:			G_Printf("ET_GENERAL          ");
ADDRGP4 $206
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 284
;284:			break;
ADDRGP4 $203
JUMPV
LABELV $207
line 286
;285:		case ET_PLAYER:
;286:			G_Printf("ET_PLAYER           ");
ADDRGP4 $208
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 287
;287:			break;
ADDRGP4 $203
JUMPV
LABELV $209
line 289
;288:		case ET_ITEM:
;289:			G_Printf("ET_ITEM             ");
ADDRGP4 $210
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 290
;290:			break;
ADDRGP4 $203
JUMPV
LABELV $211
line 292
;291:		case ET_MISSILE:
;292:			G_Printf("ET_MISSILE          ");
ADDRGP4 $212
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 293
;293:			break;
ADDRGP4 $203
JUMPV
LABELV $213
line 295
;294:		case ET_MOVER:
;295:			G_Printf("ET_MOVER            ");
ADDRGP4 $214
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 296
;296:			break;
ADDRGP4 $203
JUMPV
LABELV $215
line 298
;297:		case ET_BEAM:
;298:			G_Printf("ET_BEAM             ");
ADDRGP4 $216
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 299
;299:			break;
ADDRGP4 $203
JUMPV
LABELV $217
line 301
;300:		case ET_PORTAL:
;301:			G_Printf("ET_PORTAL           ");
ADDRGP4 $218
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 302
;302:			break;
ADDRGP4 $203
JUMPV
LABELV $219
line 304
;303:		case ET_SPEAKER:
;304:			G_Printf("ET_SPEAKER          ");
ADDRGP4 $220
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 305
;305:			break;
ADDRGP4 $203
JUMPV
LABELV $221
line 307
;306:		case ET_PUSH_TRIGGER:
;307:			G_Printf("ET_PUSH_TRIGGER     ");
ADDRGP4 $222
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 308
;308:			break;
ADDRGP4 $203
JUMPV
LABELV $223
line 310
;309:		case ET_TELEPORT_TRIGGER:
;310:			G_Printf("ET_TELEPORT_TRIGGER ");
ADDRGP4 $224
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 311
;311:			break;
ADDRGP4 $203
JUMPV
LABELV $225
line 313
;312:		case ET_INVISIBLE:
;313:			G_Printf("ET_INVISIBLE        ");
ADDRGP4 $226
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 314
;314:			break;
ADDRGP4 $203
JUMPV
LABELV $227
line 316
;315:		case ET_GRAPPLE:
;316:			G_Printf("ET_GRAPPLE          ");
ADDRGP4 $228
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 317
;317:			break;
ADDRGP4 $203
JUMPV
LABELV $202
line 319
;318:		default:
;319:			G_Printf("%3i                 ", check->s.eType);
ADDRGP4 $229
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 4
ADDP4
INDIRI4
ARGI4
ADDRGP4 G_Printf
CALLV
pop
line 320
;320:			break;
LABELV $203
line 323
;321:		}
;322:
;323:		if ( check->classname ) {
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $231
line 324
;324:			G_Printf("%s", check->classname);
ADDRGP4 $233
ARGP4
ADDRLP4 0
INDIRP4
CNSTI4 416
ADDP4
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 325
;325:		}
LABELV $231
line 326
;326:		G_Printf("\n");
ADDRGP4 $234
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 327
;327:	}
LABELV $195
line 276
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
LABELV $197
ADDRLP4 4
INDIRI4
ADDRGP4 level+12
INDIRI4
LTI4 $194
line 328
;328:}
LABELV $192
endproc Svcmd_EntityList_f 16 8
export ClientForString
proc ClientForString 24 8
line 330
;329:
;330:gclient_t	*ClientForString( const char *s ) {
line 336
;331:	gclient_t	*cl;
;332:	int			i;
;333:	int			idnum;
;334:
;335:	// numeric values are just slot numbers
;336:	if ( s[0] >= '0' && s[0] <= '9' ) {
ADDRLP4 12
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 48
LTI4 $236
ADDRLP4 12
INDIRI4
CNSTI4 57
GTI4 $236
line 337
;337:		idnum = atoi( s );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 atoi
CALLI4
ASGNI4
ADDRLP4 8
ADDRLP4 16
INDIRI4
ASGNI4
line 338
;338:		if ( idnum < 0 || idnum >= level.maxclients ) {
ADDRLP4 20
ADDRLP4 8
INDIRI4
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
LTI4 $241
ADDRLP4 20
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $238
LABELV $241
line 339
;339:			Com_Printf( "Bad client slot: %i\n", idnum );
ADDRGP4 $242
ARGP4
ADDRLP4 8
INDIRI4
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 340
;340:			return NULL;
CNSTP4 0
RETP4
ADDRGP4 $235
JUMPV
LABELV $238
line 343
;341:		}
;342:
;343:		cl = &level.clients[idnum];
ADDRLP4 0
CNSTI4 1756
ADDRLP4 8
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 344
;344:		if ( cl->pers.connected == CON_DISCONNECTED ) {
ADDRLP4 0
INDIRP4
CNSTI4 1368
ADDP4
INDIRI4
CNSTI4 0
NEI4 $243
line 345
;345:			G_Printf( "Client %i is not connected\n", idnum );
ADDRGP4 $245
ARGP4
ADDRLP4 8
INDIRI4
ARGI4
ADDRGP4 G_Printf
CALLV
pop
line 346
;346:			return NULL;
CNSTP4 0
RETP4
ADDRGP4 $235
JUMPV
LABELV $243
line 348
;347:		}
;348:		return cl;
ADDRLP4 0
INDIRP4
RETP4
ADDRGP4 $235
JUMPV
LABELV $236
line 352
;349:	}
;350:
;351:	// check for a name match
;352:	for ( i=0 ; i < level.maxclients ; i++ ) {
ADDRLP4 4
CNSTI4 0
ASGNI4
ADDRGP4 $249
JUMPV
LABELV $246
line 353
;353:		cl = &level.clients[i];
ADDRLP4 0
CNSTI4 1756
ADDRLP4 4
INDIRI4
MULI4
ADDRGP4 level
INDIRP4
ADDP4
ASGNP4
line 354
;354:		if ( cl->pers.connected == CON_DISCONNECTED ) {
ADDRLP4 0
INDIRP4
CNSTI4 1368
ADDP4
INDIRI4
CNSTI4 0
NEI4 $251
line 355
;355:			continue;
ADDRGP4 $247
JUMPV
LABELV $251
line 357
;356:		}
;357:		if ( !Q_stricmp( cl->pers.netname, s ) ) {
ADDRLP4 0
INDIRP4
CNSTI4 1416
ADDP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 16
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $253
line 358
;358:			return cl;
ADDRLP4 0
INDIRP4
RETP4
ADDRGP4 $235
JUMPV
LABELV $253
line 360
;359:		}
;360:	}
LABELV $247
line 352
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $249
ADDRLP4 4
INDIRI4
ADDRGP4 level+24
INDIRI4
LTI4 $246
line 362
;361:
;362:	G_Printf( "User %s is not on the server\n", s );
ADDRGP4 $255
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 G_Printf
CALLV
pop
line 364
;363:
;364:	return NULL;
CNSTP4 0
RETP4
LABELV $235
endproc ClientForString 24 8
export Svcmd_ForceTeam_f
proc Svcmd_ForceTeam_f 1032 12
line 374
;365:}
;366:
;367:/*
;368:===================
;369:Svcmd_ForceTeam_f
;370:
;371:forceteam <player> <team>
;372:===================
;373:*/
;374:void	Svcmd_ForceTeam_f( void ) {
line 379
;375:	gclient_t	*cl;
;376:	char		str[MAX_TOKEN_CHARS];
;377:
;378:	// find the player
;379:	trap_Argv( 1, str, sizeof( str ) );
CNSTI4 1
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 380
;380:	cl = ClientForString( str );
ADDRLP4 0
ARGP4
ADDRLP4 1028
ADDRGP4 ClientForString
CALLP4
ASGNP4
ADDRLP4 1024
ADDRLP4 1028
INDIRP4
ASGNP4
line 381
;381:	if ( !cl ) {
ADDRLP4 1024
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $257
line 382
;382:		return;
ADDRGP4 $256
JUMPV
LABELV $257
line 386
;383:	}
;384:
;385:	// set the team
;386:	trap_Argv( 2, str, sizeof( str ) );
CNSTI4 2
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 387
;387:	SetTeam( &g_entities[cl - level.clients], str );
CNSTI4 828
ADDRLP4 1024
INDIRP4
CVPU4 4
ADDRGP4 level
INDIRP4
CVPU4 4
SUBU4
CVUI4 4
CNSTI4 1756
DIVI4
MULI4
ADDRGP4 g_entities
ADDP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 SetTeam
CALLV
pop
line 388
;388:}
LABELV $256
endproc Svcmd_ForceTeam_f 1032 12
export ConsoleCommand
proc ConsoleCommand 1068 12
line 398
;389:
;390:char	*ConcatArgs( int start );
;391:
;392:/*
;393:=================
;394:ConsoleCommand
;395:
;396:=================
;397:*/
;398:qboolean	ConsoleCommand( void ) {
line 401
;399:	char	cmd[MAX_TOKEN_CHARS];
;400:
;401:	trap_Argv( 0, cmd, sizeof( cmd ) );
CNSTI4 0
ARGI4
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 trap_Argv
CALLV
pop
line 403
;402:
;403:	if ( Q_stricmp (cmd, "entitylist") == 0 ) {
ADDRLP4 0
ARGP4
ADDRGP4 $262
ARGP4
ADDRLP4 1024
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1024
INDIRI4
CNSTI4 0
NEI4 $260
line 404
;404:		Svcmd_EntityList_f();
ADDRGP4 Svcmd_EntityList_f
CALLV
pop
line 405
;405:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $259
JUMPV
LABELV $260
line 408
;406:	}
;407:
;408:	if ( Q_stricmp (cmd, "forceteam") == 0 ) {
ADDRLP4 0
ARGP4
ADDRGP4 $265
ARGP4
ADDRLP4 1028
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1028
INDIRI4
CNSTI4 0
NEI4 $263
line 409
;409:		Svcmd_ForceTeam_f();
ADDRGP4 Svcmd_ForceTeam_f
CALLV
pop
line 410
;410:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $259
JUMPV
LABELV $263
line 413
;411:	}
;412:
;413:	if (Q_stricmp (cmd, "game_memory") == 0) {
ADDRLP4 0
ARGP4
ADDRGP4 $268
ARGP4
ADDRLP4 1032
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1032
INDIRI4
CNSTI4 0
NEI4 $266
line 414
;414:		Svcmd_GameMem_f();
ADDRGP4 Svcmd_GameMem_f
CALLV
pop
line 415
;415:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $259
JUMPV
LABELV $266
line 418
;416:	}
;417:
;418:	if (Q_stricmp (cmd, "addbot") == 0) {
ADDRLP4 0
ARGP4
ADDRGP4 $271
ARGP4
ADDRLP4 1036
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1036
INDIRI4
CNSTI4 0
NEI4 $269
line 419
;419:		Svcmd_AddBot_f();
ADDRGP4 Svcmd_AddBot_f
CALLV
pop
line 420
;420:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $259
JUMPV
LABELV $269
line 423
;421:	}
;422:
;423:	if (Q_stricmp (cmd, "botlist") == 0) {
ADDRLP4 0
ARGP4
ADDRGP4 $274
ARGP4
ADDRLP4 1040
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1040
INDIRI4
CNSTI4 0
NEI4 $272
line 424
;424:		Svcmd_BotList_f();
ADDRGP4 Svcmd_BotList_f
CALLV
pop
line 425
;425:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $259
JUMPV
LABELV $272
line 433
;426:	}
;427:
;428:/*	if (Q_stricmp (cmd, "abort_podium") == 0) {
;429:		Svcmd_AbortPodium_f();
;430:		return qtrue;
;431:	}
;432:*/
;433:	if (Q_stricmp (cmd, "addip") == 0) {
ADDRLP4 0
ARGP4
ADDRGP4 $277
ARGP4
ADDRLP4 1044
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1044
INDIRI4
CNSTI4 0
NEI4 $275
line 434
;434:		Svcmd_AddIP_f();
ADDRGP4 Svcmd_AddIP_f
CALLV
pop
line 435
;435:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $259
JUMPV
LABELV $275
line 438
;436:	}
;437:
;438:	if (Q_stricmp (cmd, "removeip") == 0) {
ADDRLP4 0
ARGP4
ADDRGP4 $280
ARGP4
ADDRLP4 1048
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1048
INDIRI4
CNSTI4 0
NEI4 $278
line 439
;439:		Svcmd_RemoveIP_f();
ADDRGP4 Svcmd_RemoveIP_f
CALLV
pop
line 440
;440:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $259
JUMPV
LABELV $278
line 443
;441:	}
;442:
;443:	if (Q_stricmp (cmd, "listip") == 0) {
ADDRLP4 0
ARGP4
ADDRGP4 $283
ARGP4
ADDRLP4 1052
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1052
INDIRI4
CNSTI4 0
NEI4 $281
line 444
;444:		trap_SendConsoleCommand( EXEC_NOW, "g_banIPs\n" );
CNSTI4 0
ARGI4
ADDRGP4 $284
ARGP4
ADDRGP4 trap_SendConsoleCommand
CALLV
pop
line 445
;445:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $259
JUMPV
LABELV $281
line 448
;446:	}
;447:
;448:	if (g_dedicated.integer) {
ADDRGP4 g_dedicated+12
INDIRI4
CNSTI4 0
EQI4 $285
line 449
;449:		if (Q_stricmp (cmd, "say") == 0) {
ADDRLP4 0
ARGP4
ADDRGP4 $290
ARGP4
ADDRLP4 1056
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
NEI4 $288
line 450
;450:			trap_SendServerCommand( -1, va("print \"server: %s\"", ConcatArgs(1) ) );
CNSTI4 1
ARGI4
ADDRLP4 1060
ADDRGP4 ConcatArgs
CALLP4
ASGNP4
ADDRGP4 $291
ARGP4
ADDRLP4 1060
INDIRP4
ARGP4
ADDRLP4 1064
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 1064
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 451
;451:			return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $259
JUMPV
LABELV $288
line 454
;452:		}
;453:		// everything else will also be printed as a say command
;454:		trap_SendServerCommand( -1, va("print \"server: %s\"", ConcatArgs(0) ) );
CNSTI4 0
ARGI4
ADDRLP4 1060
ADDRGP4 ConcatArgs
CALLP4
ASGNP4
ADDRGP4 $291
ARGP4
ADDRLP4 1060
INDIRP4
ARGP4
ADDRLP4 1064
ADDRGP4 va
CALLP4
ASGNP4
CNSTI4 -1
ARGI4
ADDRLP4 1064
INDIRP4
ARGP4
ADDRGP4 trap_SendServerCommand
CALLV
pop
line 455
;455:		return qtrue;
CNSTI4 1
RETI4
ADDRGP4 $259
JUMPV
LABELV $285
line 458
;456:	}
;457:
;458:	return qfalse;
CNSTI4 0
RETI4
LABELV $259
endproc ConsoleCommand 1068 12
import ConcatArgs
bss
align 4
LABELV numIPFilters
skip 4
align 4
LABELV ipFilters
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
LABELV $291
char 1 112
char 1 114
char 1 105
char 1 110
char 1 116
char 1 32
char 1 34
char 1 115
char 1 101
char 1 114
char 1 118
char 1 101
char 1 114
char 1 58
char 1 32
char 1 37
char 1 115
char 1 34
char 1 0
align 1
LABELV $290
char 1 115
char 1 97
char 1 121
char 1 0
align 1
LABELV $284
char 1 103
char 1 95
char 1 98
char 1 97
char 1 110
char 1 73
char 1 80
char 1 115
char 1 10
char 1 0
align 1
LABELV $283
char 1 108
char 1 105
char 1 115
char 1 116
char 1 105
char 1 112
char 1 0
align 1
LABELV $280
char 1 114
char 1 101
char 1 109
char 1 111
char 1 118
char 1 101
char 1 105
char 1 112
char 1 0
align 1
LABELV $277
char 1 97
char 1 100
char 1 100
char 1 105
char 1 112
char 1 0
align 1
LABELV $274
char 1 98
char 1 111
char 1 116
char 1 108
char 1 105
char 1 115
char 1 116
char 1 0
align 1
LABELV $271
char 1 97
char 1 100
char 1 100
char 1 98
char 1 111
char 1 116
char 1 0
align 1
LABELV $268
char 1 103
char 1 97
char 1 109
char 1 101
char 1 95
char 1 109
char 1 101
char 1 109
char 1 111
char 1 114
char 1 121
char 1 0
align 1
LABELV $265
char 1 102
char 1 111
char 1 114
char 1 99
char 1 101
char 1 116
char 1 101
char 1 97
char 1 109
char 1 0
align 1
LABELV $262
char 1 101
char 1 110
char 1 116
char 1 105
char 1 116
char 1 121
char 1 108
char 1 105
char 1 115
char 1 116
char 1 0
align 1
LABELV $255
char 1 85
char 1 115
char 1 101
char 1 114
char 1 32
char 1 37
char 1 115
char 1 32
char 1 105
char 1 115
char 1 32
char 1 110
char 1 111
char 1 116
char 1 32
char 1 111
char 1 110
char 1 32
char 1 116
char 1 104
char 1 101
char 1 32
char 1 115
char 1 101
char 1 114
char 1 118
char 1 101
char 1 114
char 1 10
char 1 0
align 1
LABELV $245
char 1 67
char 1 108
char 1 105
char 1 101
char 1 110
char 1 116
char 1 32
char 1 37
char 1 105
char 1 32
char 1 105
char 1 115
char 1 32
char 1 110
char 1 111
char 1 116
char 1 32
char 1 99
char 1 111
char 1 110
char 1 110
char 1 101
char 1 99
char 1 116
char 1 101
char 1 100
char 1 10
char 1 0
align 1
LABELV $242
char 1 66
char 1 97
char 1 100
char 1 32
char 1 99
char 1 108
char 1 105
char 1 101
char 1 110
char 1 116
char 1 32
char 1 115
char 1 108
char 1 111
char 1 116
char 1 58
char 1 32
char 1 37
char 1 105
char 1 10
char 1 0
align 1
LABELV $234
char 1 10
char 1 0
align 1
LABELV $233
char 1 37
char 1 115
char 1 0
align 1
LABELV $229
char 1 37
char 1 51
char 1 105
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
char 1 32
char 1 32
char 1 32
char 1 0
align 1
LABELV $228
char 1 69
char 1 84
char 1 95
char 1 71
char 1 82
char 1 65
char 1 80
char 1 80
char 1 76
char 1 69
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
char 1 0
align 1
LABELV $226
char 1 69
char 1 84
char 1 95
char 1 73
char 1 78
char 1 86
char 1 73
char 1 83
char 1 73
char 1 66
char 1 76
char 1 69
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 0
align 1
LABELV $224
char 1 69
char 1 84
char 1 95
char 1 84
char 1 69
char 1 76
char 1 69
char 1 80
char 1 79
char 1 82
char 1 84
char 1 95
char 1 84
char 1 82
char 1 73
char 1 71
char 1 71
char 1 69
char 1 82
char 1 32
char 1 0
align 1
LABELV $222
char 1 69
char 1 84
char 1 95
char 1 80
char 1 85
char 1 83
char 1 72
char 1 95
char 1 84
char 1 82
char 1 73
char 1 71
char 1 71
char 1 69
char 1 82
char 1 32
char 1 32
char 1 32
char 1 32
char 1 32
char 1 0
align 1
LABELV $220
char 1 69
char 1 84
char 1 95
char 1 83
char 1 80
char 1 69
char 1 65
char 1 75
char 1 69
char 1 82
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
char 1 0
align 1
LABELV $218
char 1 69
char 1 84
char 1 95
char 1 80
char 1 79
char 1 82
char 1 84
char 1 65
char 1 76
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
char 1 0
align 1
LABELV $216
char 1 69
char 1 84
char 1 95
char 1 66
char 1 69
char 1 65
char 1 77
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
char 1 0
align 1
LABELV $214
char 1 69
char 1 84
char 1 95
char 1 77
char 1 79
char 1 86
char 1 69
char 1 82
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
char 1 0
align 1
LABELV $212
char 1 69
char 1 84
char 1 95
char 1 77
char 1 73
char 1 83
char 1 83
char 1 73
char 1 76
char 1 69
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
char 1 0
align 1
LABELV $210
char 1 69
char 1 84
char 1 95
char 1 73
char 1 84
char 1 69
char 1 77
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
char 1 0
align 1
LABELV $208
char 1 69
char 1 84
char 1 95
char 1 80
char 1 76
char 1 65
char 1 89
char 1 69
char 1 82
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
char 1 0
align 1
LABELV $206
char 1 69
char 1 84
char 1 95
char 1 71
char 1 69
char 1 78
char 1 69
char 1 82
char 1 65
char 1 76
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
char 1 0
align 1
LABELV $201
char 1 37
char 1 51
char 1 105
char 1 58
char 1 0
align 1
LABELV $191
char 1 68
char 1 105
char 1 100
char 1 110
char 1 39
char 1 116
char 1 32
char 1 102
char 1 105
char 1 110
char 1 100
char 1 32
char 1 37
char 1 115
char 1 46
char 1 10
char 1 0
align 1
LABELV $190
char 1 82
char 1 101
char 1 109
char 1 111
char 1 118
char 1 101
char 1 100
char 1 46
char 1 10
char 1 0
align 1
LABELV $178
char 1 85
char 1 115
char 1 97
char 1 103
char 1 101
char 1 58
char 1 32
char 1 32
char 1 115
char 1 118
char 1 32
char 1 114
char 1 101
char 1 109
char 1 111
char 1 118
char 1 101
char 1 105
char 1 112
char 1 32
char 1 60
char 1 105
char 1 112
char 1 45
char 1 109
char 1 97
char 1 115
char 1 107
char 1 62
char 1 10
char 1 0
align 1
LABELV $174
char 1 85
char 1 115
char 1 97
char 1 103
char 1 101
char 1 58
char 1 32
char 1 32
char 1 97
char 1 100
char 1 100
char 1 105
char 1 112
char 1 32
char 1 60
char 1 105
char 1 112
char 1 45
char 1 109
char 1 97
char 1 115
char 1 107
char 1 62
char 1 10
char 1 0
align 1
LABELV $153
char 1 73
char 1 80
char 1 32
char 1 102
char 1 105
char 1 108
char 1 116
char 1 101
char 1 114
char 1 32
char 1 108
char 1 105
char 1 115
char 1 116
char 1 32
char 1 105
char 1 115
char 1 32
char 1 102
char 1 117
char 1 108
char 1 108
char 1 10
char 1 0
align 1
LABELV $112
char 1 103
char 1 95
char 1 98
char 1 97
char 1 110
char 1 73
char 1 80
char 1 115
char 1 0
align 1
LABELV $108
char 1 37
char 1 105
char 1 46
char 1 37
char 1 105
char 1 46
char 1 37
char 1 105
char 1 46
char 1 37
char 1 105
char 1 32
char 1 0
align 1
LABELV $90
char 1 66
char 1 97
char 1 100
char 1 32
char 1 102
char 1 105
char 1 108
char 1 116
char 1 101
char 1 114
char 1 32
char 1 97
char 1 100
char 1 100
char 1 114
char 1 101
char 1 115
char 1 115
char 1 58
char 1 32
char 1 37
char 1 115
char 1 10
char 1 0
