export GetIDForString
code
proc GetIDForString 8 8
file "../q_shared.c"
line 14
;1:// Copyright (C) 1999-2000 Id Software, Inc.
;2://
;3:// q_shared.c -- stateless support routines that are included in each code dll
;4:#include "q_shared.h"
;5:
;6:/*
;7:-------------------------
;8:GetIDForString 
;9:-------------------------
;10:*/
;11:
;12:
;13:int GetIDForString ( stringID_table_t *table, const char *string )
;14:{
line 15
;15:	int	index = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $38
JUMPV
LABELV $37
line 19
;16:
;17:	while ( ( table[index].name != NULL ) &&
;18:			( table[index].name[0] != 0 ) )
;19:	{
line 20
;20:		if ( !Q_stricmp( table[index].name, string ) )
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $40
line 21
;21:			return table[index].id;
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRFP4 0
INDIRP4
ADDP4
CNSTI4 4
ADDP4
INDIRI4
RETI4
ADDRGP4 $36
JUMPV
LABELV $40
line 23
;22:
;23:		index++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 24
;24:	}
LABELV $38
line 17
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $42
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $37
LABELV $42
line 26
;25:
;26:	return -1;
CNSTI4 -1
RETI4
LABELV $36
endproc GetIDForString 8 8
export GetStringForID
proc GetStringForID 8 0
line 36
;27:}
;28:
;29:/*
;30:-------------------------
;31:GetStringForID
;32:-------------------------
;33:*/
;34:
;35:const char *GetStringForID( stringID_table_t *table, int id )
;36:{
line 37
;37:	int	index = 0;
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $45
JUMPV
LABELV $44
line 41
;38:
;39:	while ( ( table[index].name != NULL ) &&
;40:			( table[index].name[0] != 0 ) )
;41:	{
line 42
;42:		if ( table[index].id == id )
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRFP4 0
INDIRP4
ADDP4
CNSTI4 4
ADDP4
INDIRI4
ADDRFP4 4
INDIRI4
NEI4 $47
line 43
;43:			return table[index].name;
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRP4
RETP4
ADDRGP4 $43
JUMPV
LABELV $47
line 45
;44:
;45:		index++;
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 46
;46:	}
LABELV $45
line 39
ADDRLP4 4
ADDRLP4 0
INDIRI4
CNSTI4 3
LSHI4
ADDRFP4 0
INDIRP4
ADDP4
INDIRP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $49
ADDRLP4 4
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $44
LABELV $49
line 48
;47:
;48:	return NULL;
CNSTP4 0
RETP4
LABELV $43
endproc GetStringForID 8 0
export Com_Clamp
proc Com_Clamp 0 0
line 52
;49:}
;50:
;51:
;52:float Com_Clamp( float min, float max, float value ) {
line 53
;53:	if ( value < min ) {
ADDRFP4 8
INDIRF4
ADDRFP4 0
INDIRF4
GEF4 $51
line 54
;54:		return min;
ADDRFP4 0
INDIRF4
RETF4
ADDRGP4 $50
JUMPV
LABELV $51
line 56
;55:	}
;56:	if ( value > max ) {
ADDRFP4 8
INDIRF4
ADDRFP4 4
INDIRF4
LEF4 $53
line 57
;57:		return max;
ADDRFP4 4
INDIRF4
RETF4
ADDRGP4 $50
JUMPV
LABELV $53
line 59
;58:	}
;59:	return value;
ADDRFP4 8
INDIRF4
RETF4
LABELV $50
endproc Com_Clamp 0 0
export COM_SkipPath
proc COM_SkipPath 4 0
line 69
;60:}
;61:
;62:
;63:/*
;64:============
;65:COM_SkipPath
;66:============
;67:*/
;68:char *COM_SkipPath (char *pathname)
;69:{
line 72
;70:	char	*last;
;71:	
;72:	last = pathname;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $57
JUMPV
LABELV $56
line 74
;73:	while (*pathname)
;74:	{
line 75
;75:		if (*pathname=='/')
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 47
NEI4 $59
line 76
;76:			last = pathname+1;
ADDRLP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
LABELV $59
line 77
;77:		pathname++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 78
;78:	}
LABELV $57
line 73
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $56
line 79
;79:	return last;
ADDRLP4 0
INDIRP4
RETP4
LABELV $55
endproc COM_SkipPath 4 0
export COM_StripExtension
proc COM_StripExtension 12 0
line 87
;80:}
;81:
;82:/*
;83:============
;84:COM_StripExtension
;85:============
;86:*/
;87:void COM_StripExtension( const char *in, char *out ) {
ADDRGP4 $63
JUMPV
LABELV $62
line 88
;88:	while ( *in && *in != '.' ) {
line 89
;89:		*out++ = *in++;
ADDRLP4 0
ADDRFP4 4
INDIRP4
ASGNP4
ADDRLP4 8
CNSTI4 1
ASGNI4
ADDRFP4 4
ADDRLP4 0
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
ASGNP4
ADDRLP4 4
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 4
INDIRP4
ADDRLP4 8
INDIRI4
ADDP4
ASGNP4
ADDRLP4 0
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI1
ASGNI1
line 90
;90:	}
LABELV $63
line 88
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 0
EQI4 $65
ADDRLP4 0
INDIRI4
CNSTI4 46
NEI4 $62
LABELV $65
line 91
;91:	*out = 0;
ADDRFP4 4
INDIRP4
CNSTI1 0
ASGNI1
line 92
;92:}
LABELV $61
endproc COM_StripExtension 12 0
export COM_DefaultExtension
proc COM_DefaultExtension 76 20
line 100
;93:
;94:
;95:/*
;96:==================
;97:COM_DefaultExtension
;98:==================
;99:*/
;100:void COM_DefaultExtension (char *path, int maxSize, const char *extension ) {
line 108
;101:	char	oldPath[MAX_QPATH];
;102:	char    *src;
;103:
;104://
;105:// if path doesn't have a .EXT, append extension
;106:// (extension should include the .)
;107://
;108:	src = path + strlen(path) - 1;
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 68
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 68
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
CNSTI4 -1
ADDP4
ASGNP4
ADDRGP4 $68
JUMPV
LABELV $67
line 110
;109:
;110:	while (*src != '/' && src != path) {
line 111
;111:		if ( *src == '.' ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 46
NEI4 $70
line 112
;112:			return;                 // it has an extension
ADDRGP4 $66
JUMPV
LABELV $70
line 114
;113:		}
;114:		src--;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 -1
ADDP4
ASGNP4
line 115
;115:	}
LABELV $68
line 110
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 47
EQI4 $72
ADDRLP4 0
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
NEU4 $67
LABELV $72
line 117
;116:
;117:	Q_strncpyz( oldPath, path, sizeof( oldPath ) );
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
line 118
;118:	Com_sprintf( path, maxSize, "%s%s", oldPath, extension );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 $73
ARGP4
ADDRLP4 4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 119
;119:}
LABELV $66
endproc COM_DefaultExtension 76 20
export ShortSwap
proc ShortSwap 2 0
ADDRFP4 0
ADDRFP4 0
INDIRI4
CVII2 4
ASGNI2
line 151
;120:
;121:/*
;122:============================================================================
;123:
;124:					BYTE ORDER FUNCTIONS
;125:
;126:============================================================================
;127:*/
;128:/*
;129:// can't just use function pointers, or dll linkage can
;130:// mess up when qcommon is included in multiple places
;131:static short	(*_BigShort) (short l);
;132:static short	(*_LittleShort) (short l);
;133:static int		(*_BigLong) (int l);
;134:static int		(*_LittleLong) (int l);
;135:static qint64	(*_BigLong64) (qint64 l);
;136:static qint64	(*_LittleLong64) (qint64 l);
;137:static float	(*_BigFloat) (const float *l);
;138:static float	(*_LittleFloat) (const float *l);
;139:
;140:short	BigShort(short l){return _BigShort(l);}
;141:short	LittleShort(short l) {return _LittleShort(l);}
;142:int		BigLong (int l) {return _BigLong(l);}
;143:int		LittleLong (int l) {return _LittleLong(l);}
;144:qint64 	BigLong64 (qint64 l) {return _BigLong64(l);}
;145:qint64 	LittleLong64 (qint64 l) {return _LittleLong64(l);}
;146:float	BigFloat (const float *l) {return _BigFloat(l);}
;147:float	LittleFloat (const float *l) {return _LittleFloat(l);}
;148:*/
;149:
;150:short   ShortSwap (short l)
;151:{
line 154
;152:	byte    b1,b2;
;153:
;154:	b1 = l&255;
ADDRLP4 0
ADDRFP4 0
INDIRI2
CVII4 2
CNSTI4 255
BANDI4
CVIU4 4
CVUU1 4
ASGNU1
line 155
;155:	b2 = (l>>8)&255;
ADDRLP4 1
ADDRFP4 0
INDIRI2
CVII4 2
CNSTI4 8
RSHI4
CNSTI4 255
BANDI4
CVIU4 4
CVUU1 4
ASGNU1
line 157
;156:
;157:	return (b1<<8) + b2;
ADDRLP4 0
INDIRU1
CVUI4 1
CNSTI4 8
LSHI4
ADDRLP4 1
INDIRU1
CVUI4 1
ADDI4
CVII2 4
CVII4 2
RETI4
LABELV $74
endproc ShortSwap 2 0
export ShortNoSwap
proc ShortNoSwap 0 0
ADDRFP4 0
ADDRFP4 0
INDIRI4
CVII2 4
ASGNI2
line 161
;158:}
;159:
;160:short	ShortNoSwap (short l)
;161:{
line 162
;162:	return l;
ADDRFP4 0
INDIRI2
CVII4 2
RETI4
LABELV $75
endproc ShortNoSwap 0 0
export LongSwap
proc LongSwap 4 0
line 166
;163:}
;164:
;165:int    LongSwap (int l)
;166:{
line 169
;167:	byte    b1,b2,b3,b4;
;168:
;169:	b1 = l&255;
ADDRLP4 0
ADDRFP4 0
INDIRI4
CNSTI4 255
BANDI4
CVIU4 4
CVUU1 4
ASGNU1
line 170
;170:	b2 = (l>>8)&255;
ADDRLP4 1
ADDRFP4 0
INDIRI4
CNSTI4 8
RSHI4
CNSTI4 255
BANDI4
CVIU4 4
CVUU1 4
ASGNU1
line 171
;171:	b3 = (l>>16)&255;
ADDRLP4 2
ADDRFP4 0
INDIRI4
CNSTI4 16
RSHI4
CNSTI4 255
BANDI4
CVIU4 4
CVUU1 4
ASGNU1
line 172
;172:	b4 = (l>>24)&255;
ADDRLP4 3
ADDRFP4 0
INDIRI4
CNSTI4 24
RSHI4
CNSTI4 255
BANDI4
CVIU4 4
CVUU1 4
ASGNU1
line 174
;173:
;174:	return ((int)b1<<24) + ((int)b2<<16) + ((int)b3<<8) + b4;
ADDRLP4 0
INDIRU1
CVUI4 1
CNSTI4 24
LSHI4
ADDRLP4 1
INDIRU1
CVUI4 1
CNSTI4 16
LSHI4
ADDI4
ADDRLP4 2
INDIRU1
CVUI4 1
CNSTI4 8
LSHI4
ADDI4
ADDRLP4 3
INDIRU1
CVUI4 1
ADDI4
RETI4
LABELV $76
endproc LongSwap 4 0
export LongNoSwap
proc LongNoSwap 0 0
line 178
;175:}
;176:
;177:int	LongNoSwap (int l)
;178:{
line 179
;179:	return l;
ADDRFP4 0
INDIRI4
RETI4
LABELV $77
endproc LongNoSwap 0 0
export Long64Swap
proc Long64Swap 8 0
line 183
;180:}
;181:
;182:qint64 Long64Swap (qint64 ll)
;183:{
line 186
;184:	qint64	result;
;185:
;186:	result.b0 = ll.b7;
ADDRLP4 0
ADDRFP4 4
INDIRP4
CNSTI4 7
ADDP4
INDIRU1
ASGNU1
line 187
;187:	result.b1 = ll.b6;
ADDRLP4 0+1
ADDRFP4 4
INDIRP4
CNSTI4 6
ADDP4
INDIRU1
ASGNU1
line 188
;188:	result.b2 = ll.b5;
ADDRLP4 0+2
ADDRFP4 4
INDIRP4
CNSTI4 5
ADDP4
INDIRU1
ASGNU1
line 189
;189:	result.b3 = ll.b4;
ADDRLP4 0+3
ADDRFP4 4
INDIRP4
CNSTI4 4
ADDP4
INDIRU1
ASGNU1
line 190
;190:	result.b4 = ll.b3;
ADDRLP4 0+4
ADDRFP4 4
INDIRP4
CNSTI4 3
ADDP4
INDIRU1
ASGNU1
line 191
;191:	result.b5 = ll.b2;
ADDRLP4 0+5
ADDRFP4 4
INDIRP4
CNSTI4 2
ADDP4
INDIRU1
ASGNU1
line 192
;192:	result.b6 = ll.b1;
ADDRLP4 0+6
ADDRFP4 4
INDIRP4
CNSTI4 1
ADDP4
INDIRU1
ASGNU1
line 193
;193:	result.b7 = ll.b0;
ADDRLP4 0+7
ADDRFP4 4
INDIRP4
INDIRU1
ASGNU1
line 195
;194:
;195:	return result;
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRB
ASGNB 8
LABELV $78
endproc Long64Swap 8 0
export Long64NoSwap
proc Long64NoSwap 0 0
line 199
;196:}
;197:
;198:qint64 Long64NoSwap (qint64 ll)
;199:{
line 200
;200:	return ll;
ADDRFP4 0
INDIRP4
ADDRFP4 4
INDIRP4
INDIRB
ASGNB 8
LABELV $87
endproc Long64NoSwap 0 0
export FloatSwap
proc FloatSwap 12 4
line 208
;201:}
;202:
;203:typedef union {
;204:    float	f;
;205:    unsigned int i;
;206:} _FloatByteUnion;
;207:
;208:float FloatSwap (const float *f) {
line 212
;209:	const _FloatByteUnion *in;
;210:	_FloatByteUnion out;
;211:
;212:	in = (_FloatByteUnion *)f;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 213
;213:	out.i = LongSwap(in->i);
ADDRLP4 0
INDIRP4
INDIRU4
CVUI4 4
ARGI4
ADDRLP4 8
ADDRGP4 LongSwap
CALLI4
ASGNI4
ADDRLP4 4
ADDRLP4 8
INDIRI4
CVIU4 4
ASGNU4
line 215
;214:
;215:	return out.f;
ADDRLP4 4
INDIRF4
RETF4
LABELV $90
endproc FloatSwap 12 4
export FloatNoSwap
proc FloatNoSwap 0 0
line 219
;216:}
;217:
;218:float FloatNoSwap (const float *f)
;219:{
line 220
;220:	return *f;
ADDRFP4 0
INDIRP4
INDIRF4
RETF4
LABELV $91
endproc FloatNoSwap 0 0
export COM_BeginParseSession
proc COM_BeginParseSession 0 16
line 273
;221:}
;222:
;223:/*
;224:================
;225:Swap_Init
;226:================
;227:*/
;228:/*
;229:void Swap_Init (void)
;230:{
;231:	byte	swaptest[2] = {1,0};
;232:
;233:// set the byte swapping variables in a portable manner	
;234:	if ( *(short *)swaptest == 1)
;235:	{
;236:		_BigShort = ShortSwap;
;237:		_LittleShort = ShortNoSwap;
;238:		_BigLong = LongSwap;
;239:		_LittleLong = LongNoSwap;
;240:		_BigLong64 = Long64Swap;
;241:		_LittleLong64 = Long64NoSwap;
;242:		_BigFloat = FloatSwap;
;243:		_LittleFloat = FloatNoSwap;
;244:	}
;245:	else
;246:	{
;247:		_BigShort = ShortNoSwap;
;248:		_LittleShort = ShortSwap;
;249:		_BigLong = LongNoSwap;
;250:		_LittleLong = LongSwap;
;251:		_BigLong64 = Long64NoSwap;
;252:		_LittleLong64 = Long64Swap;
;253:		_BigFloat = FloatNoSwap;
;254:		_LittleFloat = FloatSwap;
;255:	}
;256:
;257:}
;258:*/
;259:
;260:/*
;261:============================================================================
;262:
;263:PARSING
;264:
;265:============================================================================
;266:*/
;267:
;268:static	char	com_token[MAX_TOKEN_CHARS];
;269:static	char	com_parsename[MAX_TOKEN_CHARS];
;270:static	int		com_lines;
;271:
;272:void COM_BeginParseSession( const char *name )
;273:{
line 274
;274:	com_lines = 0;
ADDRGP4 com_lines
CNSTI4 0
ASGNI4
line 275
;275:	Com_sprintf(com_parsename, sizeof(com_parsename), "%s", name);
ADDRGP4 com_parsename
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $93
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 276
;276:}
LABELV $92
endproc COM_BeginParseSession 0 16
export COM_GetCurrentParseLine
proc COM_GetCurrentParseLine 0 0
line 279
;277:
;278:int COM_GetCurrentParseLine( void )
;279:{
line 280
;280:	return com_lines;
ADDRGP4 com_lines
INDIRI4
RETI4
LABELV $94
endproc COM_GetCurrentParseLine 0 0
export COM_Parse
proc COM_Parse 4 8
line 284
;281:}
;282:
;283:char *COM_Parse( const char **data_p )
;284:{
line 285
;285:	return COM_ParseExt( data_p, qtrue );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 0
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
RETP4
LABELV $95
endproc COM_Parse 4 8
bss
align 1
LABELV $97
skip 4096
export COM_ParseError
code
proc COM_ParseError 4 16
line 289
;286:}
;287:
;288:void COM_ParseError( char *format, ... )
;289:{
line 293
;290:	va_list argptr;
;291:	static char string[4096];
;292:
;293:	va_start (argptr, format);
ADDRLP4 0
ADDRFP4 0+4
ASGNP4
line 294
;294:	vsprintf (string, format, argptr);
ADDRGP4 $97
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 vsprintf
CALLI4
pop
line 295
;295:	va_end (argptr);
ADDRLP4 0
CNSTP4 0
ASGNP4
line 297
;296:
;297:	Com_Printf("ERROR: %s, line %d: %s\n", com_parsename, com_lines, string);
ADDRGP4 $99
ARGP4
ADDRGP4 com_parsename
ARGP4
ADDRGP4 com_lines
INDIRI4
ARGI4
ADDRGP4 $97
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 298
;298:}
LABELV $96
endproc COM_ParseError 4 16
bss
align 1
LABELV $101
skip 4096
export COM_ParseWarning
code
proc COM_ParseWarning 4 16
line 301
;299:
;300:void COM_ParseWarning( char *format, ... )
;301:{
line 305
;302:	va_list argptr;
;303:	static char string[4096];
;304:
;305:	va_start (argptr, format);
ADDRLP4 0
ADDRFP4 0+4
ASGNP4
line 306
;306:	vsprintf (string, format, argptr);
ADDRGP4 $101
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 vsprintf
CALLI4
pop
line 307
;307:	va_end (argptr);
ADDRLP4 0
CNSTP4 0
ASGNP4
line 309
;308:
;309:	Com_Printf("WARNING: %s, line %d: %s\n", com_parsename, com_lines, string);
ADDRGP4 $103
ARGP4
ADDRGP4 com_parsename
ARGP4
ADDRGP4 com_lines
INDIRI4
ARGI4
ADDRGP4 $101
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 310
;310:}
LABELV $100
endproc COM_ParseWarning 4 16
export SkipWhitespace
proc SkipWhitespace 8 0
line 324
;311:
;312:/*
;313:==============
;314:COM_Parse
;315:
;316:Parse a token out of a string
;317:Will never return NULL, just empty strings
;318:
;319:If "allowLineBreaks" is qtrue then an empty
;320:string will be returned if the next token is
;321:a newline.
;322:==============
;323:*/
;324:const char *SkipWhitespace( const char *data, qboolean *hasNewLines ) {
ADDRGP4 $106
JUMPV
LABELV $105
line 327
;325:	int c;
;326:
;327:	while( (c = *data) <= ' ') {
line 328
;328:		if( !c ) {
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $108
line 329
;329:			return NULL;
CNSTP4 0
RETP4
ADDRGP4 $104
JUMPV
LABELV $108
line 331
;330:		}
;331:		if( c == '\n' ) {
ADDRLP4 0
INDIRI4
CNSTI4 10
NEI4 $110
line 332
;332:			com_lines++;
ADDRLP4 4
ADDRGP4 com_lines
ASGNP4
ADDRLP4 4
INDIRP4
ADDRLP4 4
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 333
;333:			*hasNewLines = qtrue;
ADDRFP4 4
INDIRP4
CNSTI4 1
ASGNI4
line 334
;334:		}
LABELV $110
line 335
;335:		data++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 336
;336:	}
LABELV $106
line 327
ADDRLP4 4
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 32
LEI4 $105
line 338
;337:
;338:	return data;
ADDRFP4 0
INDIRP4
RETP4
LABELV $104
endproc SkipWhitespace 8 0
export COM_Compress
proc COM_Compress 48 0
line 341
;339:}
;340:
;341:int COM_Compress( char *data_p ) {
line 344
;342:	char *in, *out;
;343:	int c;
;344:	qboolean newline = qfalse, whitespace = qfalse;
ADDRLP4 12
CNSTI4 0
ASGNI4
ADDRLP4 16
CNSTI4 0
ASGNI4
line 346
;345:	
;346:	in = out = data_p;
ADDRLP4 20
ADDRFP4 0
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 20
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 20
INDIRP4
ASGNP4
line 347
;347:	if (in) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $113
ADDRGP4 $116
JUMPV
LABELV $115
line 348
;348:		while ((c = *in) != 0) {
line 350
;349:			// skip double slash comments
;350:			if ( c == '/' && in[1] == '/' ) {
ADDRLP4 24
CNSTI4 47
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 24
INDIRI4
NEI4 $118
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
ADDRLP4 24
INDIRI4
NEI4 $118
ADDRGP4 $121
JUMPV
LABELV $120
line 351
;351:				while (*in && *in != '\n') {
line 352
;352:					in++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 353
;353:				}
LABELV $121
line 351
ADDRLP4 28
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $123
ADDRLP4 28
INDIRI4
CNSTI4 10
NEI4 $120
LABELV $123
line 355
;354:				// skip /* */ comments
;355:			} else if ( c == '/' && in[1] == '*' ) {
ADDRGP4 $119
JUMPV
LABELV $118
ADDRLP4 4
INDIRI4
CNSTI4 47
NEI4 $124
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
CNSTI4 42
NEI4 $124
ADDRGP4 $127
JUMPV
LABELV $126
line 357
;356:				while ( *in && ( *in != '*' || in[1] != '/' ) ) 
;357:					in++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
LABELV $127
line 356
ADDRLP4 32
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
EQI4 $129
ADDRLP4 32
INDIRI4
CNSTI4 42
NEI4 $126
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
CNSTI4 47
NEI4 $126
LABELV $129
line 358
;358:				if ( *in ) 
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $125
line 359
;359:					in += 2;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
line 361
;360:				// record when we hit a newline
;361:			} else if ( c == '\n' || c == '\r' ) {
ADDRGP4 $125
JUMPV
LABELV $124
ADDRLP4 4
INDIRI4
CNSTI4 10
EQI4 $134
ADDRLP4 4
INDIRI4
CNSTI4 13
NEI4 $132
LABELV $134
line 362
;362:				newline = qtrue;
ADDRLP4 12
CNSTI4 1
ASGNI4
line 363
;363:				in++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 365
;364:				// record when we hit whitespace
;365:			} else if ( c == ' ' || c == '\t') {
ADDRGP4 $133
JUMPV
LABELV $132
ADDRLP4 4
INDIRI4
CNSTI4 32
EQI4 $137
ADDRLP4 4
INDIRI4
CNSTI4 9
NEI4 $135
LABELV $137
line 366
;366:				whitespace = qtrue;
ADDRLP4 16
CNSTI4 1
ASGNI4
line 367
;367:				in++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 369
;368:				// an actual token
;369:			} else {
ADDRGP4 $136
JUMPV
LABELV $135
line 371
;370:				// if we have a pending newline, emit it (and it counts as whitespace)
;371:				if (newline) {
ADDRLP4 12
INDIRI4
CNSTI4 0
EQI4 $138
line 372
;372:					*out++ = '\n';
ADDRLP4 36
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 36
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
CNSTI1 10
ASGNI1
line 373
;373:					newline = qfalse;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 374
;374:					whitespace = qfalse;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 375
;375:				} if (whitespace) {
LABELV $138
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $140
line 376
;376:					*out++ = ' ';
ADDRLP4 36
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 36
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
CNSTI1 32
ASGNI1
line 377
;377:					whitespace = qfalse;
ADDRLP4 16
CNSTI4 0
ASGNI4
line 378
;378:				}
LABELV $140
line 381
;379:				
;380:				// copy quoted strings unmolested
;381:				if (c == '"') {
ADDRLP4 4
INDIRI4
CNSTI4 34
NEI4 $142
line 382
;382:					*out++ = c;
ADDRLP4 36
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 36
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 36
INDIRP4
ADDRLP4 4
INDIRI4
CVII1 4
ASGNI1
line 383
;383:					in++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRGP4 $145
JUMPV
LABELV $144
line 384
;384:					while (1) {
line 385
;385:						c = *in;
ADDRLP4 4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 386
;386:						if (c && c != '"') {
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $146
ADDRLP4 4
INDIRI4
CNSTI4 34
EQI4 $146
line 387
;387:							*out++ = c;
ADDRLP4 44
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 44
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 44
INDIRP4
ADDRLP4 4
INDIRI4
CVII1 4
ASGNI1
line 388
;388:							in++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 389
;389:						} else {
line 390
;390:							break;
LABELV $148
line 392
;391:						}
;392:					}
LABELV $145
line 384
ADDRGP4 $144
JUMPV
LABELV $146
line 393
;393:					if (c == '"') {
ADDRLP4 4
INDIRI4
CNSTI4 34
NEI4 $143
line 394
;394:						*out++ = c;
ADDRLP4 40
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 40
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 40
INDIRP4
ADDRLP4 4
INDIRI4
CVII1 4
ASGNI1
line 395
;395:						in++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 396
;396:					}
line 397
;397:				} else {
ADDRGP4 $143
JUMPV
LABELV $142
line 398
;398:					*out = c;
ADDRLP4 8
INDIRP4
ADDRLP4 4
INDIRI4
CVII1 4
ASGNI1
line 399
;399:					out++;
ADDRLP4 8
ADDRLP4 8
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 400
;400:					in++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 401
;401:				}
LABELV $143
line 402
;402:			}
LABELV $136
LABELV $133
LABELV $125
LABELV $119
line 403
;403:		}
LABELV $116
line 348
ADDRLP4 24
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 4
ADDRLP4 24
INDIRI4
ASGNI4
ADDRLP4 24
INDIRI4
CNSTI4 0
NEI4 $115
line 404
;404:	}
LABELV $113
line 405
;405:	*out = 0;
ADDRLP4 8
INDIRP4
CNSTI1 0
ASGNI1
line 406
;406:	return out - data_p;
ADDRLP4 8
INDIRP4
CVPU4 4
ADDRFP4 0
INDIRP4
CVPU4 4
SUBU4
CVUI4 4
RETI4
LABELV $112
endproc COM_Compress 48 0
export COM_ParseExt
proc COM_ParseExt 36 8
line 410
;407:}
;408:
;409:char *COM_ParseExt( const char **data_p, qboolean allowLineBreaks )
;410:{
line 411
;411:	int c = 0, len;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 412
;412:	qboolean hasNewLines = qfalse;
ADDRLP4 12
CNSTI4 0
ASGNI4
line 415
;413:	const char *data;
;414:
;415:	data = *data_p;
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRP4
ASGNP4
line 416
;416:	len = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 417
;417:	com_token[0] = 0;
ADDRGP4 com_token
CNSTI1 0
ASGNI1
line 420
;418:
;419:	// make sure incoming data is valid
;420:	if ( !data )
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $155
line 421
;421:	{
line 422
;422:		*data_p = NULL;
ADDRFP4 0
INDIRP4
CNSTP4 0
ASGNP4
line 423
;423:		return com_token;
ADDRGP4 com_token
RETP4
ADDRGP4 $151
JUMPV
LABELV $154
line 427
;424:	}
;425:
;426:	while ( 1 )
;427:	{
line 429
;428:		// skip whitespace
;429:		data = SkipWhitespace( data, &hasNewLines );
ADDRLP4 0
INDIRP4
ARGP4
ADDRLP4 12
ARGP4
ADDRLP4 16
ADDRGP4 SkipWhitespace
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 16
INDIRP4
ASGNP4
line 430
;430:		if ( !data )
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $157
line 431
;431:		{
line 432
;432:			*data_p = NULL;
ADDRFP4 0
INDIRP4
CNSTP4 0
ASGNP4
line 433
;433:			return com_token;
ADDRGP4 com_token
RETP4
ADDRGP4 $151
JUMPV
LABELV $157
line 435
;434:		}
;435:		if ( hasNewLines && !allowLineBreaks )
ADDRLP4 20
CNSTI4 0
ASGNI4
ADDRLP4 12
INDIRI4
ADDRLP4 20
INDIRI4
EQI4 $159
ADDRFP4 4
INDIRI4
ADDRLP4 20
INDIRI4
NEI4 $159
line 436
;436:		{
line 437
;437:			*data_p = data;
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRP4
ASGNP4
line 438
;438:			return com_token;
ADDRGP4 com_token
RETP4
ADDRGP4 $151
JUMPV
LABELV $159
line 441
;439:		}
;440:
;441:		c = *data;
ADDRLP4 4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 444
;442:
;443:		// skip double slash comments
;444:		if ( c == '/' && data[1] == '/' )
ADDRLP4 24
CNSTI4 47
ASGNI4
ADDRLP4 4
INDIRI4
ADDRLP4 24
INDIRI4
NEI4 $161
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
ADDRLP4 24
INDIRI4
NEI4 $161
line 445
;445:		{
line 446
;446:			data += 2;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
ADDRGP4 $164
JUMPV
LABELV $163
line 447
;447:			while (*data && *data != '\n') {
line 448
;448:				data++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 449
;449:			}
LABELV $164
line 447
ADDRLP4 28
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 28
INDIRI4
CNSTI4 0
EQI4 $166
ADDRLP4 28
INDIRI4
CNSTI4 10
NEI4 $163
LABELV $166
line 450
;450:		}
ADDRGP4 $162
JUMPV
LABELV $161
line 452
;451:		// skip /* */ comments
;452:		else if ( c=='/' && data[1] == '*' ) 
ADDRLP4 4
INDIRI4
CNSTI4 47
NEI4 $156
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
CNSTI4 42
NEI4 $156
line 453
;453:		{
line 454
;454:			data += 2;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
ADDRGP4 $170
JUMPV
LABELV $169
line 456
;455:			while ( *data && ( *data != '*' || data[1] != '/' ) ) 
;456:			{
line 457
;457:				data++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 458
;458:			}
LABELV $170
line 455
ADDRLP4 32
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 32
INDIRI4
CNSTI4 0
EQI4 $172
ADDRLP4 32
INDIRI4
CNSTI4 42
NEI4 $169
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
CNSTI4 47
NEI4 $169
LABELV $172
line 459
;459:			if ( *data ) 
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
EQI4 $168
line 460
;460:			{
line 461
;461:				data += 2;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
line 462
;462:			}
line 463
;463:		}
line 465
;464:		else
;465:		{
line 466
;466:			break;
LABELV $168
LABELV $162
line 468
;467:		}
;468:	}
LABELV $155
line 426
ADDRGP4 $154
JUMPV
LABELV $156
line 471
;469:
;470:	// handle quoted strings
;471:	if (c == '\"')
ADDRLP4 4
INDIRI4
CNSTI4 34
NEI4 $175
line 472
;472:	{
line 473
;473:		data++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRGP4 $178
JUMPV
LABELV $177
line 475
;474:		while (1)
;475:		{
line 476
;476:			c = *data++;
ADDRLP4 16
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 16
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 4
ADDRLP4 16
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 477
;477:			if (c=='\"' || !c)
ADDRLP4 4
INDIRI4
CNSTI4 34
EQI4 $182
ADDRLP4 4
INDIRI4
CNSTI4 0
NEI4 $180
LABELV $182
line 478
;478:			{
line 479
;479:				com_token[len] = 0;
ADDRLP4 8
INDIRI4
ADDRGP4 com_token
ADDP4
CNSTI1 0
ASGNI1
line 480
;480:				*data_p = ( char * ) data;
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRP4
ASGNP4
line 481
;481:				return com_token;
ADDRGP4 com_token
RETP4
ADDRGP4 $151
JUMPV
LABELV $180
line 483
;482:			}
;483:			if (len < MAX_TOKEN_CHARS)
ADDRLP4 8
INDIRI4
CNSTI4 1024
GEI4 $183
line 484
;484:			{
line 485
;485:				com_token[len] = c;
ADDRLP4 8
INDIRI4
ADDRGP4 com_token
ADDP4
ADDRLP4 4
INDIRI4
CVII1 4
ASGNI1
line 486
;486:				len++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 487
;487:			}
LABELV $183
line 488
;488:		}
LABELV $178
line 474
ADDRGP4 $177
JUMPV
line 489
;489:	}
LABELV $175
LABELV $185
line 493
;490:
;491:	// parse a regular word
;492:	do
;493:	{
line 494
;494:		if (len < MAX_TOKEN_CHARS)
ADDRLP4 8
INDIRI4
CNSTI4 1024
GEI4 $188
line 495
;495:		{
line 496
;496:			com_token[len] = c;
ADDRLP4 8
INDIRI4
ADDRGP4 com_token
ADDP4
ADDRLP4 4
INDIRI4
CVII1 4
ASGNI1
line 497
;497:			len++;
ADDRLP4 8
ADDRLP4 8
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 498
;498:		}
LABELV $188
line 499
;499:		data++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 500
;500:		c = *data;
ADDRLP4 4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 501
;501:		if ( c == '\n' )
ADDRLP4 4
INDIRI4
CNSTI4 10
NEI4 $190
line 502
;502:			com_lines++;
ADDRLP4 16
ADDRGP4 com_lines
ASGNP4
ADDRLP4 16
INDIRP4
ADDRLP4 16
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $190
line 503
;503:	} while (c>32);
LABELV $186
ADDRLP4 4
INDIRI4
CNSTI4 32
GTI4 $185
line 505
;504:
;505:	if (len == MAX_TOKEN_CHARS)
ADDRLP4 8
INDIRI4
CNSTI4 1024
NEI4 $192
line 506
;506:	{
line 508
;507://		Com_Printf ("Token exceeded %i chars, discarded.\n", MAX_TOKEN_CHARS);
;508:		len = 0;
ADDRLP4 8
CNSTI4 0
ASGNI4
line 509
;509:	}
LABELV $192
line 510
;510:	com_token[len] = 0;
ADDRLP4 8
INDIRI4
ADDRGP4 com_token
ADDP4
CNSTI1 0
ASGNI1
line 512
;511:
;512:	*data_p = ( char * ) data;
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRP4
ASGNP4
line 513
;513:	return com_token;
ADDRGP4 com_token
RETP4
LABELV $151
endproc COM_ParseExt 36 8
export COM_MatchToken
proc COM_MatchToken 12 16
line 577
;514:}
;515:
;516:
;517:#if 0
;518:// no longer used
;519:/*
;520:===============
;521:COM_ParseInfos
;522:===============
;523:*/
;524:int COM_ParseInfos( char *buf, int max, char infos[][MAX_INFO_STRING] ) {
;525:	char	*token;
;526:	int		count;
;527:	char	key[MAX_TOKEN_CHARS];
;528:
;529:	count = 0;
;530:
;531:	while ( 1 ) {
;532:		token = COM_Parse( &buf );
;533:		if ( !token[0] ) {
;534:			break;
;535:		}
;536:		if ( strcmp( token, "{" ) ) {
;537:			Com_Printf( "Missing { in info file\n" );
;538:			break;
;539:		}
;540:
;541:		if ( count == max ) {
;542:			Com_Printf( "Max infos exceeded\n" );
;543:			break;
;544:		}
;545:
;546:		infos[count][0] = 0;
;547:		while ( 1 ) {
;548:			token = COM_ParseExt( &buf, qtrue );
;549:			if ( !token[0] ) {
;550:				Com_Printf( "Unexpected end of info file\n" );
;551:				break;
;552:			}
;553:			if ( !strcmp( token, "}" ) ) {
;554:				break;
;555:			}
;556:			Q_strncpyz( key, token, sizeof( key ) );
;557:
;558:			token = COM_ParseExt( &buf, qfalse );
;559:			if ( !token[0] ) {
;560:				strcpy( token, "<NULL>" );
;561:			}
;562:			Info_SetValueForKey( infos[count], key, token );
;563:		}
;564:		count++;
;565:	}
;566:
;567:	return count;
;568:}
;569:#endif
;570:
;571:
;572:/*
;573:==================
;574:COM_MatchToken
;575:==================
;576:*/
;577:void COM_MatchToken( const char **buf_p, char *match ) {
line 580
;578:	char	*token;
;579:
;580:	token = COM_Parse( buf_p );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 COM_Parse
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 4
INDIRP4
ASGNP4
line 581
;581:	if ( strcmp( token, match ) ) {
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 0
EQI4 $195
line 582
;582:		Com_Error( ERR_DROP, "MatchToken: %s != %s", token, match );
CNSTI4 1
ARGI4
ADDRGP4 $197
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 583
;583:	}
LABELV $195
line 584
;584:}
LABELV $194
endproc COM_MatchToken 12 16
export SkipBracedSection
proc SkipBracedSection 12 8
line 596
;585:
;586:
;587:/*
;588:=================
;589:SkipBracedSection
;590:
;591:The next token should be an open brace.
;592:Skips until a matching close brace is found.
;593:Internal brace depths are properly skipped.
;594:=================
;595:*/
;596:void SkipBracedSection (const char **program) {
line 600
;597:	char			*token;
;598:	int				depth;
;599:
;600:	depth = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
LABELV $199
line 601
;601:	do {
line 602
;602:		token = COM_ParseExt( program, qtrue );
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 1
ARGI4
ADDRLP4 8
ADDRGP4 COM_ParseExt
CALLP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
ASGNP4
line 603
;603:		if( token[1] == 0 ) {
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $202
line 604
;604:			if( token[0] == '{' ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 123
NEI4 $204
line 605
;605:				depth++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 606
;606:			}
ADDRGP4 $205
JUMPV
LABELV $204
line 607
;607:			else if( token[0] == '}' ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 125
NEI4 $206
line 608
;608:				depth--;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
line 609
;609:			}
LABELV $206
LABELV $205
line 610
;610:		}
LABELV $202
line 611
;611:	} while( depth && *program );
LABELV $200
ADDRLP4 4
INDIRI4
CNSTI4 0
EQI4 $208
ADDRFP4 0
INDIRP4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $199
LABELV $208
line 612
;612:}
LABELV $198
endproc SkipBracedSection 12 8
export SkipRestOfLine
proc SkipRestOfLine 16 0
line 619
;613:
;614:/*
;615:=================
;616:SkipRestOfLine
;617:=================
;618:*/
;619:void SkipRestOfLine ( const char **data ) {
line 623
;620:	const char	*p;
;621:	int		c;
;622:
;623:	p = *data;
ADDRLP4 4
ADDRFP4 0
INDIRP4
INDIRP4
ASGNP4
ADDRGP4 $211
JUMPV
LABELV $210
line 624
;624:	while ( (c = *p++) != 0 ) {
line 625
;625:		if ( c == '\n' ) {
ADDRLP4 0
INDIRI4
CNSTI4 10
NEI4 $213
line 626
;626:			com_lines++;
ADDRLP4 8
ADDRGP4 com_lines
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 627
;627:			break;
ADDRGP4 $212
JUMPV
LABELV $213
line 629
;628:		}
;629:	}
LABELV $211
line 624
ADDRLP4 8
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 12
ADDRLP4 8
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 0
ADDRLP4 12
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $210
LABELV $212
line 631
;630:
;631:	*data = p;
ADDRFP4 0
INDIRP4
ADDRLP4 4
INDIRP4
ASGNP4
line 632
;632:}
LABELV $209
endproc SkipRestOfLine 16 0
export Parse1DMatrix
proc Parse1DMatrix 16 8
line 635
;633:
;634:
;635:void Parse1DMatrix (const char **buf_p, int x, float *m) {
line 639
;636:	char	*token;
;637:	int		i;
;638:
;639:	COM_MatchToken( buf_p, "(" );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $216
ARGP4
ADDRGP4 COM_MatchToken
CALLV
pop
line 641
;640:
;641:	for (i = 0 ; i < x ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $220
JUMPV
LABELV $217
line 642
;642:		token = COM_Parse(buf_p);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ADDRGP4 COM_Parse
CALLP4
ASGNP4
ADDRLP4 4
ADDRLP4 8
INDIRP4
ASGNP4
line 643
;643:		m[i] = atof(token);
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 12
ADDRGP4 atof
CALLF4
ASGNF4
ADDRLP4 0
INDIRI4
CNSTI4 2
LSHI4
ADDRFP4 8
INDIRP4
ADDP4
ADDRLP4 12
INDIRF4
ASGNF4
line 644
;644:	}
LABELV $218
line 641
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $220
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRI4
LTI4 $217
line 646
;645:
;646:	COM_MatchToken( buf_p, ")" );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $221
ARGP4
ADDRGP4 COM_MatchToken
CALLV
pop
line 647
;647:}
LABELV $215
endproc Parse1DMatrix 16 8
export Parse2DMatrix
proc Parse2DMatrix 8 12
line 649
;648:
;649:void Parse2DMatrix (const char **buf_p, int y, int x, float *m) {
line 652
;650:	int		i;
;651:
;652:	COM_MatchToken( buf_p, "(" );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $216
ARGP4
ADDRGP4 COM_MatchToken
CALLV
pop
line 654
;653:
;654:	for (i = 0 ; i < y ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $226
JUMPV
LABELV $223
line 655
;655:		Parse1DMatrix (buf_p, x, m + i * x);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
MULI4
CNSTI4 2
LSHI4
ADDRFP4 12
INDIRP4
ADDP4
ARGP4
ADDRGP4 Parse1DMatrix
CALLV
pop
line 656
;656:	}
LABELV $224
line 654
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $226
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRI4
LTI4 $223
line 658
;657:
;658:	COM_MatchToken( buf_p, ")" );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $221
ARGP4
ADDRGP4 COM_MatchToken
CALLV
pop
line 659
;659:}
LABELV $222
endproc Parse2DMatrix 8 12
export Parse3DMatrix
proc Parse3DMatrix 12 16
line 661
;660:
;661:void Parse3DMatrix (const char **buf_p, int z, int y, int x, float *m) {
line 664
;662:	int		i;
;663:
;664:	COM_MatchToken( buf_p, "(" );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $216
ARGP4
ADDRGP4 COM_MatchToken
CALLV
pop
line 666
;665:
;666:	for (i = 0 ; i < z ; i++) {
ADDRLP4 0
CNSTI4 0
ASGNI4
ADDRGP4 $231
JUMPV
LABELV $228
line 667
;667:		Parse2DMatrix (buf_p, y, x, m + i * x*y);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRFP4 8
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
ARGI4
ADDRLP4 8
ADDRFP4 12
INDIRI4
ASGNI4
ADDRLP4 8
INDIRI4
ARGI4
ADDRLP4 0
INDIRI4
ADDRLP4 8
INDIRI4
MULI4
ADDRLP4 4
INDIRI4
MULI4
CNSTI4 2
LSHI4
ADDRFP4 16
INDIRP4
ADDP4
ARGP4
ADDRGP4 Parse2DMatrix
CALLV
pop
line 668
;668:	}
LABELV $229
line 666
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
LABELV $231
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRI4
LTI4 $228
line 670
;669:
;670:	COM_MatchToken( buf_p, ")" );
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 $221
ARGP4
ADDRGP4 COM_MatchToken
CALLV
pop
line 671
;671:}
LABELV $227
endproc Parse3DMatrix 12 16
export Q_isprint
proc Q_isprint 4 0
line 683
;672:
;673:
;674:/*
;675:============================================================================
;676:
;677:					LIBRARY REPLACEMENT FUNCTIONS
;678:
;679:============================================================================
;680:*/
;681:
;682:int Q_isprint( int c )
;683:{
line 684
;684:	if ( c >= 0x20 && c <= 0x7E )
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 32
LTI4 $233
ADDRLP4 0
INDIRI4
CNSTI4 126
GTI4 $233
line 685
;685:		return ( 1 );
CNSTI4 1
RETI4
ADDRGP4 $232
JUMPV
LABELV $233
line 686
;686:	return ( 0 );
CNSTI4 0
RETI4
LABELV $232
endproc Q_isprint 4 0
export Q_islower
proc Q_islower 4 0
line 690
;687:}
;688:
;689:int Q_islower( int c )
;690:{
line 691
;691:	if (c >= 'a' && c <= 'z')
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 97
LTI4 $236
ADDRLP4 0
INDIRI4
CNSTI4 122
GTI4 $236
line 692
;692:		return ( 1 );
CNSTI4 1
RETI4
ADDRGP4 $235
JUMPV
LABELV $236
line 693
;693:	return ( 0 );
CNSTI4 0
RETI4
LABELV $235
endproc Q_islower 4 0
export Q_isupper
proc Q_isupper 4 0
line 697
;694:}
;695:
;696:int Q_isupper( int c )
;697:{
line 698
;698:	if (c >= 'A' && c <= 'Z')
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 65
LTI4 $239
ADDRLP4 0
INDIRI4
CNSTI4 90
GTI4 $239
line 699
;699:		return ( 1 );
CNSTI4 1
RETI4
ADDRGP4 $238
JUMPV
LABELV $239
line 700
;700:	return ( 0 );
CNSTI4 0
RETI4
LABELV $238
endproc Q_isupper 4 0
export Q_isalpha
proc Q_isalpha 8 0
line 704
;701:}
;702:
;703:int Q_isalpha( int c )
;704:{
line 705
;705:	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
ADDRLP4 0
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 0
INDIRI4
CNSTI4 97
LTI4 $245
ADDRLP4 0
INDIRI4
CNSTI4 122
LEI4 $244
LABELV $245
ADDRLP4 4
ADDRFP4 0
INDIRI4
ASGNI4
ADDRLP4 4
INDIRI4
CNSTI4 65
LTI4 $242
ADDRLP4 4
INDIRI4
CNSTI4 90
GTI4 $242
LABELV $244
line 706
;706:		return ( 1 );
CNSTI4 1
RETI4
ADDRGP4 $241
JUMPV
LABELV $242
line 707
;707:	return ( 0 );
CNSTI4 0
RETI4
LABELV $241
endproc Q_isalpha 8 0
export Q_strrchr
proc Q_strrchr 12 0
line 711
;708:}
;709:
;710:char* Q_strrchr( const char* string, int c )
;711:{
line 712
;712:	char cc = c;
ADDRLP4 4
ADDRFP4 4
INDIRI4
CVII1 4
ASGNI1
line 714
;713:	char *s;
;714:	char *sp=(char *)0;
ADDRLP4 8
CNSTP4 0
ASGNP4
line 716
;715:
;716:	s = (char*)string;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $248
JUMPV
LABELV $247
line 719
;717:
;718:	while (*s)
;719:	{
line 720
;720:		if (*s == cc)
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 4
INDIRI1
CVII4 1
NEI4 $250
line 721
;721:			sp = s;
ADDRLP4 8
ADDRLP4 0
INDIRP4
ASGNP4
LABELV $250
line 722
;722:		s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 723
;723:	}
LABELV $248
line 718
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $247
line 724
;724:	if (cc == 0)
ADDRLP4 4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $252
line 725
;725:		sp = s;
ADDRLP4 8
ADDRLP4 0
INDIRP4
ASGNP4
LABELV $252
line 727
;726:
;727:	return sp;
ADDRLP4 8
INDIRP4
RETP4
LABELV $246
endproc Q_strrchr 12 0
export Q_strncpyz
proc Q_strncpyz 0 12
line 737
;728:}
;729:
;730:/*
;731:=============
;732:Q_strncpyz
;733: 
;734:Safe strncpy that ensures a trailing zero
;735:=============
;736:*/
;737:void Q_strncpyz( char *dest, const char *src, int destsize ) {
line 739
;738:  // bk001129 - also NULL dest
;739:  if ( !dest ) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $255
line 740
;740:    Com_Error( ERR_FATAL, "Q_strncpyz: NULL dest" );
CNSTI4 0
ARGI4
ADDRGP4 $257
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 741
;741:  }
LABELV $255
line 742
;742:	if ( !src ) {
ADDRFP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $258
line 743
;743:		Com_Error( ERR_FATAL, "Q_strncpyz: NULL src" );
CNSTI4 0
ARGI4
ADDRGP4 $260
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 744
;744:	}
LABELV $258
line 745
;745:	if ( destsize < 1 ) {
ADDRFP4 8
INDIRI4
CNSTI4 1
GEI4 $261
line 746
;746:		Com_Error(ERR_FATAL,"Q_strncpyz: destsize < 1" ); 
CNSTI4 0
ARGI4
ADDRGP4 $263
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 747
;747:	}
LABELV $261
line 749
;748:
;749:	strncpy( dest, src, destsize-1 );
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRI4
CNSTI4 1
SUBI4
ARGI4
ADDRGP4 strncpy
CALLP4
pop
line 750
;750:  dest[destsize-1] = 0;
ADDRFP4 8
INDIRI4
CNSTI4 1
SUBI4
ADDRFP4 0
INDIRP4
ADDP4
CNSTI1 0
ASGNI1
line 751
;751:}
LABELV $254
endproc Q_strncpyz 0 12
export Q_stricmpn
proc Q_stricmpn 32 0
line 753
;752:                 
;753:int Q_stricmpn (const char *s1, const char *s2, int n) {
line 757
;754:	int		c1, c2;
;755:
;756:	// bk001129 - moved in 1.17 fix not in id codebase
;757:        if ( s1 == NULL ) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $265
line 758
;758:           if ( s2 == NULL )
ADDRFP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $267
line 759
;759:             return 0;
CNSTI4 0
RETI4
ADDRGP4 $264
JUMPV
LABELV $267
line 761
;760:           else
;761:             return -1;
CNSTI4 -1
RETI4
ADDRGP4 $264
JUMPV
LABELV $265
line 763
;762:        }
;763:        else if ( s2==NULL )
ADDRFP4 4
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $269
line 764
;764:          return 1;
CNSTI4 1
RETI4
ADDRGP4 $264
JUMPV
LABELV $269
LABELV $271
line 768
;765:
;766:
;767:	
;768:	do {
line 769
;769:		c1 = *s1++;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 8
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 770
;770:		c2 = *s2++;
ADDRLP4 12
ADDRFP4 4
INDIRP4
ASGNP4
ADDRFP4 4
ADDRLP4 12
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 4
ADDRLP4 12
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 772
;771:
;772:		if (!n--) {
ADDRLP4 16
ADDRFP4 8
INDIRI4
ASGNI4
ADDRFP4 8
ADDRLP4 16
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $274
line 773
;773:			return 0;		// strings are equal until end point
CNSTI4 0
RETI4
ADDRGP4 $264
JUMPV
LABELV $274
line 776
;774:		}
;775:		
;776:		if (c1 != c2) {
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $276
line 777
;777:			if (c1 >= 'a' && c1 <= 'z') {
ADDRLP4 0
INDIRI4
CNSTI4 97
LTI4 $278
ADDRLP4 0
INDIRI4
CNSTI4 122
GTI4 $278
line 778
;778:				c1 -= ('a' - 'A');
ADDRLP4 0
ADDRLP4 0
INDIRI4
CNSTI4 32
SUBI4
ASGNI4
line 779
;779:			}
LABELV $278
line 780
;780:			if (c2 >= 'a' && c2 <= 'z') {
ADDRLP4 4
INDIRI4
CNSTI4 97
LTI4 $280
ADDRLP4 4
INDIRI4
CNSTI4 122
GTI4 $280
line 781
;781:				c2 -= ('a' - 'A');
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 32
SUBI4
ASGNI4
line 782
;782:			}
LABELV $280
line 783
;783:			if (c1 != c2) {
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $282
line 784
;784:				return c1 < c2 ? -1 : 1;
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
GEI4 $285
ADDRLP4 28
CNSTI4 -1
ASGNI4
ADDRGP4 $286
JUMPV
LABELV $285
ADDRLP4 28
CNSTI4 1
ASGNI4
LABELV $286
ADDRLP4 28
INDIRI4
RETI4
ADDRGP4 $264
JUMPV
LABELV $282
line 786
;785:			}
;786:		}
LABELV $276
line 787
;787:	} while (c1);
LABELV $272
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $271
line 789
;788:	
;789:	return 0;		// strings are equal
CNSTI4 0
RETI4
LABELV $264
endproc Q_stricmpn 32 0
export Q_strncmp
proc Q_strncmp 24 0
line 792
;790:}
;791:
;792:int Q_strncmp (const char *s1, const char *s2, int n) {
LABELV $288
line 795
;793:	int		c1, c2;
;794:	
;795:	do {
line 796
;796:		c1 = *s1++;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 8
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 0
ADDRLP4 8
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 797
;797:		c2 = *s2++;
ADDRLP4 12
ADDRFP4 4
INDIRP4
ASGNP4
ADDRFP4 4
ADDRLP4 12
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 4
ADDRLP4 12
INDIRP4
INDIRI1
CVII4 1
ASGNI4
line 799
;798:
;799:		if (!n--) {
ADDRLP4 16
ADDRFP4 8
INDIRI4
ASGNI4
ADDRFP4 8
ADDRLP4 16
INDIRI4
CNSTI4 1
SUBI4
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
NEI4 $291
line 800
;800:			return 0;		// strings are equal until end point
CNSTI4 0
RETI4
ADDRGP4 $287
JUMPV
LABELV $291
line 803
;801:		}
;802:		
;803:		if (c1 != c2) {
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
EQI4 $293
line 804
;804:			return c1 < c2 ? -1 : 1;
ADDRLP4 0
INDIRI4
ADDRLP4 4
INDIRI4
GEI4 $296
ADDRLP4 20
CNSTI4 -1
ASGNI4
ADDRGP4 $297
JUMPV
LABELV $296
ADDRLP4 20
CNSTI4 1
ASGNI4
LABELV $297
ADDRLP4 20
INDIRI4
RETI4
ADDRGP4 $287
JUMPV
LABELV $293
line 806
;805:		}
;806:	} while (c1);
LABELV $289
ADDRLP4 0
INDIRI4
CNSTI4 0
NEI4 $288
line 808
;807:	
;808:	return 0;		// strings are equal
CNSTI4 0
RETI4
LABELV $287
endproc Q_strncmp 24 0
export Q_stricmp
proc Q_stricmp 12 12
line 811
;809:}
;810:
;811:int Q_stricmp (const char *s1, const char *s2) {
line 812
;812:	return (s1 && s2) ? Q_stricmpn (s1, s2, 99999) : -1;
ADDRLP4 4
CNSTU4 0
ASGNU4
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
EQU4 $300
ADDRFP4 4
INDIRP4
CVPU4 4
ADDRLP4 4
INDIRU4
EQU4 $300
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 99999
ARGI4
ADDRLP4 8
ADDRGP4 Q_stricmpn
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 8
INDIRI4
ASGNI4
ADDRGP4 $301
JUMPV
LABELV $300
ADDRLP4 0
CNSTI4 -1
ASGNI4
LABELV $301
ADDRLP4 0
INDIRI4
RETI4
LABELV $298
endproc Q_stricmp 12 12
export Q_strlwr
proc Q_strlwr 12 4
line 816
;813:}
;814:
;815:
;816:char *Q_strlwr( char *s1 ) {
line 819
;817:    char	*s;
;818:
;819:    s = s1;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $304
JUMPV
LABELV $303
line 820
;820:	while ( *s ) {
line 821
;821:		*s = tolower(*s);
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ARGI4
ADDRLP4 8
ADDRGP4 tolower
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 8
INDIRI4
CVII1 4
ASGNI1
line 822
;822:		s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 823
;823:	}
LABELV $304
line 820
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $303
line 824
;824:    return s1;
ADDRFP4 0
INDIRP4
RETP4
LABELV $302
endproc Q_strlwr 12 4
export Q_strupr
proc Q_strupr 12 4
line 827
;825:}
;826:
;827:char *Q_strupr( char *s1 ) {
line 830
;828:    char	*s;
;829:
;830:    s = s1;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $308
JUMPV
LABELV $307
line 831
;831:	while ( *s ) {
line 832
;832:		*s = toupper(*s);
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ARGI4
ADDRLP4 8
ADDRGP4 toupper
CALLI4
ASGNI4
ADDRLP4 0
INDIRP4
ADDRLP4 8
INDIRI4
CVII1 4
ASGNI1
line 833
;833:		s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 834
;834:	}
LABELV $308
line 831
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $307
line 835
;835:    return s1;
ADDRFP4 0
INDIRP4
RETP4
LABELV $306
endproc Q_strupr 12 4
export Q_strcat
proc Q_strcat 12 12
line 840
;836:}
;837:
;838:
;839:// never goes past bounds or leaves without a terminating 0
;840:void Q_strcat( char *dest, int size, const char *src ) {
line 843
;841:	int		l1;
;842:
;843:	l1 = strlen( dest );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 4
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ASGNI4
line 844
;844:	if ( l1 >= size ) {
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRI4
LTI4 $311
line 845
;845:		Com_Error( ERR_FATAL, "Q_strcat: already overflowed" );
CNSTI4 0
ARGI4
ADDRGP4 $313
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 846
;846:	}
LABELV $311
line 847
;847:	Q_strncpyz( dest + l1, src, size - l1 );
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRP4
ADDP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRFP4 4
INDIRI4
ADDRLP4 0
INDIRI4
SUBI4
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 848
;848:}
LABELV $310
endproc Q_strcat 12 12
export Q_PrintStrlen
proc Q_PrintStrlen 20 0
line 851
;849:
;850:
;851:int Q_PrintStrlen( const char *string ) {
line 855
;852:	int			len;
;853:	const char	*p;
;854:
;855:	if( !string ) {
ADDRFP4 0
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $315
line 856
;856:		return 0;
CNSTI4 0
RETI4
ADDRGP4 $314
JUMPV
LABELV $315
line 859
;857:	}
;858:
;859:	len = 0;
ADDRLP4 4
CNSTI4 0
ASGNI4
line 860
;860:	p = string;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $318
JUMPV
LABELV $317
line 861
;861:	while( *p ) {
line 862
;862:		if( Q_IsColorString( p ) ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $320
ADDRLP4 12
CNSTI4 94
ASGNI4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 12
INDIRI4
NEI4 $320
ADDRLP4 16
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 16
INDIRI4
CNSTI4 0
EQI4 $320
ADDRLP4 16
INDIRI4
ADDRLP4 12
INDIRI4
EQI4 $320
line 863
;863:			p += 2;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 2
ADDP4
ASGNP4
line 864
;864:			continue;
ADDRGP4 $318
JUMPV
LABELV $320
line 866
;865:		}
;866:		p++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 867
;867:		len++;
ADDRLP4 4
ADDRLP4 4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 868
;868:	}
LABELV $318
line 861
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $317
line 870
;869:
;870:	return len;
ADDRLP4 4
INDIRI4
RETI4
LABELV $314
endproc Q_PrintStrlen 20 0
export Q_CleanStr
proc Q_CleanStr 32 0
line 874
;871:}
;872:
;873:
;874:char *Q_CleanStr( char *string ) {
line 879
;875:	char*	d;
;876:	char*	s;
;877:	int		c;
;878:
;879:	s = string;
ADDRLP4 0
ADDRFP4 0
INDIRP4
ASGNP4
line 880
;880:	d = string;
ADDRLP4 8
ADDRFP4 0
INDIRP4
ASGNP4
ADDRGP4 $324
JUMPV
LABELV $323
line 881
;881:	while ((c = *s) != 0 ) {
line 882
;882:		if ( Q_IsColorString( s ) ) {
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $326
ADDRLP4 16
CNSTI4 94
ASGNI4
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ADDRLP4 16
INDIRI4
NEI4 $326
ADDRLP4 20
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 20
INDIRI4
CNSTI4 0
EQI4 $326
ADDRLP4 20
INDIRI4
ADDRLP4 16
INDIRI4
EQI4 $326
line 883
;883:			s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 884
;884:		}		
ADDRGP4 $327
JUMPV
LABELV $326
line 885
;885:		else if ( c >= 0x20 && c <= 0x7E ) {
ADDRLP4 4
INDIRI4
CNSTI4 32
LTI4 $328
ADDRLP4 4
INDIRI4
CNSTI4 126
GTI4 $328
line 886
;886:			*d++ = c;
ADDRLP4 28
ADDRLP4 8
INDIRP4
ASGNP4
ADDRLP4 8
ADDRLP4 28
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRLP4 28
INDIRP4
ADDRLP4 4
INDIRI4
CVII1 4
ASGNI1
line 887
;887:		}
LABELV $328
LABELV $327
line 888
;888:		s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 889
;889:	}
LABELV $324
line 881
ADDRLP4 12
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 4
ADDRLP4 12
INDIRI4
ASGNI4
ADDRLP4 12
INDIRI4
CNSTI4 0
NEI4 $323
line 890
;890:	*d = '\0';
ADDRLP4 8
INDIRP4
CNSTI1 0
ASGNI1
line 892
;891:
;892:	return string;
ADDRFP4 0
INDIRP4
RETP4
LABELV $322
endproc Q_CleanStr 32 0
export Com_sprintf
proc Com_sprintf 32012 12
line 896
;893:}
;894:
;895:
;896:void QDECL Com_sprintf( char *dest, int size, const char *fmt, ...) {
line 901
;897:	int		len;
;898:	va_list		argptr;
;899:	char	bigbuffer[32000];	// big, but small enough to fit in PPC stack
;900:
;901:	va_start (argptr,fmt);
ADDRLP4 4
ADDRFP4 8+4
ASGNP4
line 902
;902:	len = vsprintf (bigbuffer,fmt,argptr);
ADDRLP4 8
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRLP4 4
INDIRP4
ARGP4
ADDRLP4 32008
ADDRGP4 vsprintf
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 32008
INDIRI4
ASGNI4
line 903
;903:	va_end (argptr);
ADDRLP4 4
CNSTP4 0
ASGNP4
line 904
;904:	if ( len >= sizeof( bigbuffer ) ) {
ADDRLP4 0
INDIRI4
CVIU4 4
CNSTU4 32000
LTU4 $332
line 905
;905:		Com_Error( ERR_FATAL, "Com_sprintf: overflowed bigbuffer" );
CNSTI4 0
ARGI4
ADDRGP4 $334
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 906
;906:	}
LABELV $332
line 907
;907:	if (len >= size) {
ADDRLP4 0
INDIRI4
ADDRFP4 4
INDIRI4
LTI4 $335
line 908
;908:		Com_Printf ("Com_sprintf: overflow of %i in %i\n", len, size);
ADDRGP4 $337
ARGP4
ADDRLP4 0
INDIRI4
ARGI4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 Com_Printf
CALLV
pop
line 914
;909:#ifdef	_DEBUG
;910:		__asm {
;911:			int 3;
;912:		}
;913:#endif
;914:	}
LABELV $335
line 915
;915:	Q_strncpyz (dest, bigbuffer, size );
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8
ARGP4
ADDRFP4 4
INDIRI4
ARGI4
ADDRGP4 Q_strncpyz
CALLV
pop
line 916
;916:}
LABELV $330
endproc Com_sprintf 32012 12
bss
align 1
LABELV $339
skip 64000
data
align 4
LABELV $340
byte 4 0
export va
code
proc va 12 12
line 928
;917:
;918:
;919:/*
;920:============
;921:va
;922:
;923:does a varargs printf into a temp buffer, so I don't need to have
;924:varargs versions of all text functions.
;925:FIXME: make this buffer size safe someday
;926:============
;927:*/
;928:char	* QDECL va( const char *format, ... ) {
line 934
;929:	va_list		argptr;
;930:	static char		string[2][32000];	// in case va is called by nested functions
;931:	static int		index = 0;
;932:	char	*buf;
;933:
;934:	buf = string[index & 1];
ADDRLP4 4
CNSTI4 32000
ADDRGP4 $340
INDIRI4
CNSTI4 1
BANDI4
MULI4
ADDRGP4 $339
ADDP4
ASGNP4
line 935
;935:	index++;
ADDRLP4 8
ADDRGP4 $340
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 8
INDIRP4
INDIRI4
CNSTI4 1
ADDI4
ASGNI4
line 937
;936:
;937:	va_start (argptr, format);
ADDRLP4 0
ADDRFP4 0+4
ASGNP4
line 938
;938:	vsprintf (buf, format,argptr);
ADDRLP4 4
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
INDIRP4
ARGP4
ADDRGP4 vsprintf
CALLI4
pop
line 939
;939:	va_end (argptr);
ADDRLP4 0
CNSTP4 0
ASGNP4
line 941
;940:
;941:	return buf;
ADDRLP4 4
INDIRP4
RETP4
LABELV $338
endproc va 12 12
bss
align 1
LABELV $343
skip 16384
data
align 4
LABELV $344
byte 4 0
export Info_ValueForKey
code
proc Info_ValueForKey 8220 8
line 962
;942:}
;943:
;944:
;945:/*
;946:=====================================================================
;947:
;948:  INFO STRINGS
;949:
;950:=====================================================================
;951:*/
;952:
;953:/*
;954:===============
;955:Info_ValueForKey
;956:
;957:Searches the string for the given
;958:key and returns the associated value, or an empty string.
;959:FIXME: overflow check?
;960:===============
;961:*/
;962:char *Info_ValueForKey( const char *s, const char *key ) {
line 969
;963:	char	pkey[BIG_INFO_KEY];
;964:	static	char value[2][BIG_INFO_VALUE];	// use two buffers so compares
;965:											// work without stomping on each other
;966:	static	int	valueindex = 0;
;967:	char	*o;
;968:	
;969:	if ( !s || !key ) {
ADDRLP4 8196
CNSTU4 0
ASGNU4
ADDRFP4 0
INDIRP4
CVPU4 4
ADDRLP4 8196
INDIRU4
EQU4 $347
ADDRFP4 4
INDIRP4
CVPU4 4
ADDRLP4 8196
INDIRU4
NEU4 $345
LABELV $347
line 970
;970:		return "";
ADDRGP4 $348
RETP4
ADDRGP4 $342
JUMPV
LABELV $345
line 973
;971:	}
;972:
;973:	if ( strlen( s ) >= BIG_INFO_STRING ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8200
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8200
INDIRI4
CNSTI4 8192
LTI4 $349
line 974
;974:		Com_Error( ERR_DROP, "Info_ValueForKey: oversize infostring" );
CNSTI4 1
ARGI4
ADDRGP4 $351
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 975
;975:	}
LABELV $349
line 977
;976:
;977:	valueindex ^= 1;
ADDRLP4 8204
ADDRGP4 $344
ASGNP4
ADDRLP4 8204
INDIRP4
ADDRLP4 8204
INDIRP4
INDIRI4
CNSTI4 1
BXORI4
ASGNI4
line 978
;978:	if (*s == '\\')
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $355
line 979
;979:		s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
ADDRGP4 $355
JUMPV
LABELV $354
line 981
;980:	while (1)
;981:	{
line 982
;982:		o = pkey;
ADDRLP4 0
ADDRLP4 4
ASGNP4
ADDRGP4 $358
JUMPV
LABELV $357
line 984
;983:		while (*s != '\\')
;984:		{
line 985
;985:			if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $360
line 986
;986:				return "";
ADDRGP4 $348
RETP4
ADDRGP4 $342
JUMPV
LABELV $360
line 987
;987:			*o++ = *s++;
ADDRLP4 8208
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 8216
CNSTI4 1
ASGNI4
ADDRLP4 0
ADDRLP4 8208
INDIRP4
ADDRLP4 8216
INDIRI4
ADDP4
ASGNP4
ADDRLP4 8212
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 8212
INDIRP4
ADDRLP4 8216
INDIRI4
ADDP4
ASGNP4
ADDRLP4 8208
INDIRP4
ADDRLP4 8212
INDIRP4
INDIRI1
ASGNI1
line 988
;988:		}
LABELV $358
line 983
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $357
line 989
;989:		*o = 0;
ADDRLP4 0
INDIRP4
CNSTI1 0
ASGNI1
line 990
;990:		s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 992
;991:
;992:		o = value[valueindex];
ADDRLP4 0
ADDRGP4 $344
INDIRI4
CNSTI4 13
LSHI4
ADDRGP4 $343
ADDP4
ASGNP4
ADDRGP4 $363
JUMPV
LABELV $362
line 995
;993:
;994:		while (*s != '\\' && *s)
;995:		{
line 996
;996:			*o++ = *s++;
ADDRLP4 8208
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 8216
CNSTI4 1
ASGNI4
ADDRLP4 0
ADDRLP4 8208
INDIRP4
ADDRLP4 8216
INDIRI4
ADDP4
ASGNP4
ADDRLP4 8212
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 8212
INDIRP4
ADDRLP4 8216
INDIRI4
ADDP4
ASGNP4
ADDRLP4 8208
INDIRP4
ADDRLP4 8212
INDIRP4
INDIRI1
ASGNI1
line 997
;997:		}
LABELV $363
line 994
ADDRLP4 8208
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 8208
INDIRI4
CNSTI4 92
EQI4 $365
ADDRLP4 8208
INDIRI4
CNSTI4 0
NEI4 $362
LABELV $365
line 998
;998:		*o = 0;
ADDRLP4 0
INDIRP4
CNSTI1 0
ASGNI1
line 1000
;999:
;1000:		if (!Q_stricmp (key, pkey) )
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 8212
ADDRGP4 Q_stricmp
CALLI4
ASGNI4
ADDRLP4 8212
INDIRI4
CNSTI4 0
NEI4 $366
line 1001
;1001:			return value[valueindex];
ADDRGP4 $344
INDIRI4
CNSTI4 13
LSHI4
ADDRGP4 $343
ADDP4
RETP4
ADDRGP4 $342
JUMPV
LABELV $366
line 1003
;1002:
;1003:		if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $368
line 1004
;1004:			break;
ADDRGP4 $356
JUMPV
LABELV $368
line 1005
;1005:		s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 1006
;1006:	}
LABELV $355
line 980
ADDRGP4 $354
JUMPV
LABELV $356
line 1008
;1007:
;1008:	return "";
ADDRGP4 $348
RETP4
LABELV $342
endproc Info_ValueForKey 8220 8
export Info_NextPair
proc Info_NextPair 20 0
line 1019
;1009:}
;1010:
;1011:
;1012:/*
;1013:===================
;1014:Info_NextPair
;1015:
;1016:Used to itterate through all the key/value pairs in an info string
;1017:===================
;1018:*/
;1019:void Info_NextPair( const char **head, char *key, char *value ) {
line 1023
;1020:	char	*o;
;1021:	const char	*s;
;1022:
;1023:	s = *head;
ADDRLP4 0
ADDRFP4 0
INDIRP4
INDIRP4
ASGNP4
line 1025
;1024:
;1025:	if ( *s == '\\' ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $371
line 1026
;1026:		s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 1027
;1027:	}
LABELV $371
line 1028
;1028:	key[0] = 0;
ADDRFP4 4
INDIRP4
CNSTI1 0
ASGNI1
line 1029
;1029:	value[0] = 0;
ADDRFP4 8
INDIRP4
CNSTI1 0
ASGNI1
line 1031
;1030:
;1031:	o = key;
ADDRLP4 4
ADDRFP4 4
INDIRP4
ASGNP4
ADDRGP4 $374
JUMPV
LABELV $373
line 1032
;1032:	while ( *s != '\\' ) {
line 1033
;1033:		if ( !*s ) {
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $376
line 1034
;1034:			*o = 0;
ADDRLP4 4
INDIRP4
CNSTI1 0
ASGNI1
line 1035
;1035:			*head = s;
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRP4
ASGNP4
line 1036
;1036:			return;
ADDRGP4 $370
JUMPV
LABELV $376
line 1038
;1037:		}
;1038:		*o++ = *s++;
ADDRLP4 8
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 16
CNSTI4 1
ASGNI4
ADDRLP4 4
ADDRLP4 8
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
ASGNP4
ADDRLP4 12
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI1
ASGNI1
line 1039
;1039:	}
LABELV $374
line 1032
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $373
line 1040
;1040:	*o = 0;
ADDRLP4 4
INDIRP4
CNSTI1 0
ASGNI1
line 1041
;1041:	s++;
ADDRLP4 0
ADDRLP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 1043
;1042:
;1043:	o = value;
ADDRLP4 4
ADDRFP4 8
INDIRP4
ASGNP4
ADDRGP4 $379
JUMPV
LABELV $378
line 1044
;1044:	while ( *s != '\\' && *s ) {
line 1045
;1045:		*o++ = *s++;
ADDRLP4 8
ADDRLP4 4
INDIRP4
ASGNP4
ADDRLP4 16
CNSTI4 1
ASGNI4
ADDRLP4 4
ADDRLP4 8
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
ASGNP4
ADDRLP4 12
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 0
ADDRLP4 12
INDIRP4
ADDRLP4 16
INDIRI4
ADDP4
ASGNP4
ADDRLP4 8
INDIRP4
ADDRLP4 12
INDIRP4
INDIRI1
ASGNI1
line 1046
;1046:	}
LABELV $379
line 1044
ADDRLP4 8
ADDRLP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 8
INDIRI4
CNSTI4 92
EQI4 $381
ADDRLP4 8
INDIRI4
CNSTI4 0
NEI4 $378
LABELV $381
line 1047
;1047:	*o = 0;
ADDRLP4 4
INDIRP4
CNSTI1 0
ASGNI1
line 1049
;1048:
;1049:	*head = s;
ADDRFP4 0
INDIRP4
ADDRLP4 0
INDIRP4
ASGNP4
line 1050
;1050:}
LABELV $370
endproc Info_NextPair 20 0
export Info_RemoveKey
proc Info_RemoveKey 2076 8
line 1058
;1051:
;1052:
;1053:/*
;1054:===================
;1055:Info_RemoveKey
;1056:===================
;1057:*/
;1058:void Info_RemoveKey( char *s, const char *key ) {
line 1064
;1059:	char	*start;
;1060:	char	pkey[MAX_INFO_KEY];
;1061:	char	value[MAX_INFO_VALUE];
;1062:	char	*o;
;1063:
;1064:	if ( strlen( s ) >= MAX_INFO_STRING ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 2056
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 2056
INDIRI4
CNSTI4 1024
LTI4 $383
line 1065
;1065:		Com_Error( ERR_DROP, "Info_RemoveKey: oversize infostring" );
CNSTI4 1
ARGI4
ADDRGP4 $385
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 1066
;1066:	}
LABELV $383
line 1068
;1067:
;1068:	if (strchr (key, '\\')) {
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 92
ARGI4
ADDRLP4 2060
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 2060
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $389
line 1069
;1069:		return;
ADDRGP4 $382
JUMPV
LABELV $388
line 1073
;1070:	}
;1071:
;1072:	while (1)
;1073:	{
line 1074
;1074:		start = s;
ADDRLP4 1028
ADDRFP4 0
INDIRP4
ASGNP4
line 1075
;1075:		if (*s == '\\')
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $391
line 1076
;1076:			s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
LABELV $391
line 1077
;1077:		o = pkey;
ADDRLP4 0
ADDRLP4 4
ASGNP4
ADDRGP4 $394
JUMPV
LABELV $393
line 1079
;1078:		while (*s != '\\')
;1079:		{
line 1080
;1080:			if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $396
line 1081
;1081:				return;
ADDRGP4 $382
JUMPV
LABELV $396
line 1082
;1082:			*o++ = *s++;
ADDRLP4 2064
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 2072
CNSTI4 1
ASGNI4
ADDRLP4 0
ADDRLP4 2064
INDIRP4
ADDRLP4 2072
INDIRI4
ADDP4
ASGNP4
ADDRLP4 2068
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 2068
INDIRP4
ADDRLP4 2072
INDIRI4
ADDP4
ASGNP4
ADDRLP4 2064
INDIRP4
ADDRLP4 2068
INDIRP4
INDIRI1
ASGNI1
line 1083
;1083:		}
LABELV $394
line 1078
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $393
line 1084
;1084:		*o = 0;
ADDRLP4 0
INDIRP4
CNSTI1 0
ASGNI1
line 1085
;1085:		s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 1087
;1086:
;1087:		o = value;
ADDRLP4 0
ADDRLP4 1032
ASGNP4
ADDRGP4 $399
JUMPV
LABELV $398
line 1089
;1088:		while (*s != '\\' && *s)
;1089:		{
line 1090
;1090:			if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $401
line 1091
;1091:				return;
ADDRGP4 $382
JUMPV
LABELV $401
line 1092
;1092:			*o++ = *s++;
ADDRLP4 2064
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 2072
CNSTI4 1
ASGNI4
ADDRLP4 0
ADDRLP4 2064
INDIRP4
ADDRLP4 2072
INDIRI4
ADDP4
ASGNP4
ADDRLP4 2068
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 2068
INDIRP4
ADDRLP4 2072
INDIRI4
ADDP4
ASGNP4
ADDRLP4 2064
INDIRP4
ADDRLP4 2068
INDIRP4
INDIRI1
ASGNI1
line 1093
;1093:		}
LABELV $399
line 1088
ADDRLP4 2064
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 2064
INDIRI4
CNSTI4 92
EQI4 $403
ADDRLP4 2064
INDIRI4
CNSTI4 0
NEI4 $398
LABELV $403
line 1094
;1094:		*o = 0;
ADDRLP4 0
INDIRP4
CNSTI1 0
ASGNI1
line 1096
;1095:
;1096:		if (!strcmp (key, pkey) )
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 2068
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 2068
INDIRI4
CNSTI4 0
NEI4 $404
line 1097
;1097:		{
line 1098
;1098:			strcpy (start, s);	// remove this part
ADDRLP4 1028
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1099
;1099:			return;
ADDRGP4 $382
JUMPV
LABELV $404
line 1102
;1100:		}
;1101:
;1102:		if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $406
line 1103
;1103:			return;
ADDRGP4 $382
JUMPV
LABELV $406
line 1104
;1104:	}
LABELV $389
line 1072
ADDRGP4 $388
JUMPV
line 1106
;1105:
;1106:}
LABELV $382
endproc Info_RemoveKey 2076 8
export Info_RemoveKey_Big
proc Info_RemoveKey_Big 16412 8
line 1113
;1107:
;1108:/*
;1109:===================
;1110:Info_RemoveKey_Big
;1111:===================
;1112:*/
;1113:void Info_RemoveKey_Big( char *s, const char *key ) {
line 1119
;1114:	char	*start;
;1115:	char	pkey[BIG_INFO_KEY];
;1116:	char	value[BIG_INFO_VALUE];
;1117:	char	*o;
;1118:
;1119:	if ( strlen( s ) >= BIG_INFO_STRING ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 16392
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 16392
INDIRI4
CNSTI4 8192
LTI4 $409
line 1120
;1120:		Com_Error( ERR_DROP, "Info_RemoveKey_Big: oversize infostring" );
CNSTI4 1
ARGI4
ADDRGP4 $411
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 1121
;1121:	}
LABELV $409
line 1123
;1122:
;1123:	if (strchr (key, '\\')) {
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 92
ARGI4
ADDRLP4 16396
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 16396
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $415
line 1124
;1124:		return;
ADDRGP4 $408
JUMPV
LABELV $414
line 1128
;1125:	}
;1126:
;1127:	while (1)
;1128:	{
line 1129
;1129:		start = s;
ADDRLP4 8196
ADDRFP4 0
INDIRP4
ASGNP4
line 1130
;1130:		if (*s == '\\')
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $417
line 1131
;1131:			s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
LABELV $417
line 1132
;1132:		o = pkey;
ADDRLP4 0
ADDRLP4 4
ASGNP4
ADDRGP4 $420
JUMPV
LABELV $419
line 1134
;1133:		while (*s != '\\')
;1134:		{
line 1135
;1135:			if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $422
line 1136
;1136:				return;
ADDRGP4 $408
JUMPV
LABELV $422
line 1137
;1137:			*o++ = *s++;
ADDRLP4 16400
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 16408
CNSTI4 1
ASGNI4
ADDRLP4 0
ADDRLP4 16400
INDIRP4
ADDRLP4 16408
INDIRI4
ADDP4
ASGNP4
ADDRLP4 16404
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 16404
INDIRP4
ADDRLP4 16408
INDIRI4
ADDP4
ASGNP4
ADDRLP4 16400
INDIRP4
ADDRLP4 16404
INDIRP4
INDIRI1
ASGNI1
line 1138
;1138:		}
LABELV $420
line 1133
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 92
NEI4 $419
line 1139
;1139:		*o = 0;
ADDRLP4 0
INDIRP4
CNSTI1 0
ASGNI1
line 1140
;1140:		s++;
ADDRFP4 0
ADDRFP4 0
INDIRP4
CNSTI4 1
ADDP4
ASGNP4
line 1142
;1141:
;1142:		o = value;
ADDRLP4 0
ADDRLP4 8200
ASGNP4
ADDRGP4 $425
JUMPV
LABELV $424
line 1144
;1143:		while (*s != '\\' && *s)
;1144:		{
line 1145
;1145:			if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $427
line 1146
;1146:				return;
ADDRGP4 $408
JUMPV
LABELV $427
line 1147
;1147:			*o++ = *s++;
ADDRLP4 16400
ADDRLP4 0
INDIRP4
ASGNP4
ADDRLP4 16408
CNSTI4 1
ASGNI4
ADDRLP4 0
ADDRLP4 16400
INDIRP4
ADDRLP4 16408
INDIRI4
ADDP4
ASGNP4
ADDRLP4 16404
ADDRFP4 0
INDIRP4
ASGNP4
ADDRFP4 0
ADDRLP4 16404
INDIRP4
ADDRLP4 16408
INDIRI4
ADDP4
ASGNP4
ADDRLP4 16400
INDIRP4
ADDRLP4 16404
INDIRP4
INDIRI1
ASGNI1
line 1148
;1148:		}
LABELV $425
line 1143
ADDRLP4 16400
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
ASGNI4
ADDRLP4 16400
INDIRI4
CNSTI4 92
EQI4 $429
ADDRLP4 16400
INDIRI4
CNSTI4 0
NEI4 $424
LABELV $429
line 1149
;1149:		*o = 0;
ADDRLP4 0
INDIRP4
CNSTI1 0
ASGNI1
line 1151
;1150:
;1151:		if (!strcmp (key, pkey) )
ADDRFP4 4
INDIRP4
ARGP4
ADDRLP4 4
ARGP4
ADDRLP4 16404
ADDRGP4 strcmp
CALLI4
ASGNI4
ADDRLP4 16404
INDIRI4
CNSTI4 0
NEI4 $430
line 1152
;1152:		{
line 1153
;1153:			strcpy (start, s);	// remove this part
ADDRLP4 8196
INDIRP4
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1154
;1154:			return;
ADDRGP4 $408
JUMPV
LABELV $430
line 1157
;1155:		}
;1156:
;1157:		if (!*s)
ADDRFP4 0
INDIRP4
INDIRI1
CVII4 1
CNSTI4 0
NEI4 $432
line 1158
;1158:			return;
ADDRGP4 $408
JUMPV
LABELV $432
line 1159
;1159:	}
LABELV $415
line 1127
ADDRGP4 $414
JUMPV
line 1161
;1160:
;1161:}
LABELV $408
endproc Info_RemoveKey_Big 16412 8
export Info_Validate
proc Info_Validate 8 8
line 1174
;1162:
;1163:
;1164:
;1165:
;1166:/*
;1167:==================
;1168:Info_Validate
;1169:
;1170:Some characters are illegal in info strings because they
;1171:can mess up the server's parsing
;1172:==================
;1173:*/
;1174:qboolean Info_Validate( const char *s ) {
line 1175
;1175:	if ( strchr( s, '\"' ) ) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 34
ARGI4
ADDRLP4 0
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 0
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $435
line 1176
;1176:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
LABELV $435
line 1178
;1177:	}
;1178:	if ( strchr( s, ';' ) ) {
ADDRFP4 0
INDIRP4
ARGP4
CNSTI4 59
ARGI4
ADDRLP4 4
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 4
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $437
line 1179
;1179:		return qfalse;
CNSTI4 0
RETI4
ADDRGP4 $434
JUMPV
LABELV $437
line 1181
;1180:	}
;1181:	return qtrue;
CNSTI4 1
RETI4
LABELV $434
endproc Info_Validate 8 8
export Info_SetValueForKey
proc Info_SetValueForKey 1068 20
line 1191
;1182:}
;1183:
;1184:/*
;1185:==================
;1186:Info_SetValueForKey
;1187:
;1188:Changes or adds a key/value pair
;1189:==================
;1190:*/
;1191:void Info_SetValueForKey( char *s, const char *key, const char *value ) {
line 1194
;1192:	char	newi[MAX_INFO_STRING];
;1193:
;1194:	if ( strlen( s ) >= MAX_INFO_STRING ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1024
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1024
INDIRI4
CNSTI4 1024
LTI4 $440
line 1195
;1195:		Com_Error( ERR_DROP, "Info_SetValueForKey: oversize infostring" );
CNSTI4 1
ARGI4
ADDRGP4 $442
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 1196
;1196:	}
LABELV $440
line 1198
;1197:
;1198:	if (strchr (key, '\\') || strchr (value, '\\'))
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 92
ARGI4
ADDRLP4 1028
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 1028
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $445
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 92
ARGI4
ADDRLP4 1032
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 1032
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $443
LABELV $445
line 1199
;1199:	{
line 1200
;1200:		Com_Printf ("Can't use keys or values with a \\\n");
ADDRGP4 $446
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1201
;1201:		return;
ADDRGP4 $439
JUMPV
LABELV $443
line 1204
;1202:	}
;1203:
;1204:	if (strchr (key, ';') || strchr (value, ';'))
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 59
ARGI4
ADDRLP4 1036
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 1036
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $449
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 59
ARGI4
ADDRLP4 1040
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 1040
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $447
LABELV $449
line 1205
;1205:	{
line 1206
;1206:		Com_Printf ("Can't use keys or values with a semicolon\n");
ADDRGP4 $450
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1207
;1207:		return;
ADDRGP4 $439
JUMPV
LABELV $447
line 1210
;1208:	}
;1209:
;1210:	if (strchr (key, '\"') || strchr (value, '\"'))
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 34
ARGI4
ADDRLP4 1044
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 1044
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $453
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 34
ARGI4
ADDRLP4 1048
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 1048
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $451
LABELV $453
line 1211
;1211:	{
line 1212
;1212:		Com_Printf ("Can't use keys or values with a \"\n");
ADDRGP4 $454
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1213
;1213:		return;
ADDRGP4 $439
JUMPV
LABELV $451
line 1216
;1214:	}
;1215:
;1216:	Info_RemoveKey (s, key);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 Info_RemoveKey
CALLV
pop
line 1217
;1217:	if (!value || !strlen(value))
ADDRLP4 1052
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 1052
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $457
ADDRLP4 1052
INDIRP4
ARGP4
ADDRLP4 1056
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1056
INDIRI4
CNSTI4 0
NEI4 $455
LABELV $457
line 1218
;1218:		return;
ADDRGP4 $439
JUMPV
LABELV $455
line 1220
;1219:
;1220:	Com_sprintf (newi, sizeof(newi), "\\%s\\%s", key, value);
ADDRLP4 0
ARGP4
CNSTI4 1024
ARGI4
ADDRGP4 $458
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 1222
;1221:
;1222:	if (strlen(newi) + strlen(s) > MAX_INFO_STRING)
ADDRLP4 0
ARGP4
ADDRLP4 1060
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 1064
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 1060
INDIRI4
ADDRLP4 1064
INDIRI4
ADDI4
CNSTI4 1024
LEI4 $459
line 1223
;1223:	{
line 1224
;1224:		Com_Printf ("Info string length exceeded\n");
ADDRGP4 $461
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1225
;1225:		return;
ADDRGP4 $439
JUMPV
LABELV $459
line 1228
;1226:	}
;1227:
;1228:	strcat (newi, s);
ADDRLP4 0
ARGP4
ADDRFP4 0
INDIRP4
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 1229
;1229:	strcpy (s, newi);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 strcpy
CALLP4
pop
line 1230
;1230:}
LABELV $439
endproc Info_SetValueForKey 1068 20
export Info_SetValueForKey_Big
proc Info_SetValueForKey_Big 8236 20
line 1239
;1231:
;1232:/*
;1233:==================
;1234:Info_SetValueForKey_Big
;1235:
;1236:Changes or adds a key/value pair
;1237:==================
;1238:*/
;1239:void Info_SetValueForKey_Big( char *s, const char *key, const char *value ) {
line 1242
;1240:	char	newi[BIG_INFO_STRING];
;1241:
;1242:	if ( strlen( s ) >= BIG_INFO_STRING ) {
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8192
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8192
INDIRI4
CNSTI4 8192
LTI4 $463
line 1243
;1243:		Com_Error( ERR_DROP, "Info_SetValueForKey: oversize infostring" );
CNSTI4 1
ARGI4
ADDRGP4 $442
ARGP4
ADDRGP4 Com_Error
CALLV
pop
line 1244
;1244:	}
LABELV $463
line 1246
;1245:
;1246:	if (strchr (key, '\\') || strchr (value, '\\'))
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 92
ARGI4
ADDRLP4 8196
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 8196
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $467
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 92
ARGI4
ADDRLP4 8200
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 8200
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $465
LABELV $467
line 1247
;1247:	{
line 1248
;1248:		Com_Printf ("Can't use keys or values with a \\\n");
ADDRGP4 $446
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1249
;1249:		return;
ADDRGP4 $462
JUMPV
LABELV $465
line 1252
;1250:	}
;1251:
;1252:	if (strchr (key, ';') || strchr (value, ';'))
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 59
ARGI4
ADDRLP4 8204
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 8204
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $470
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 59
ARGI4
ADDRLP4 8208
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 8208
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $468
LABELV $470
line 1253
;1253:	{
line 1254
;1254:		Com_Printf ("Can't use keys or values with a semicolon\n");
ADDRGP4 $450
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1255
;1255:		return;
ADDRGP4 $462
JUMPV
LABELV $468
line 1258
;1256:	}
;1257:
;1258:	if (strchr (key, '\"') || strchr (value, '\"'))
ADDRFP4 4
INDIRP4
ARGP4
CNSTI4 34
ARGI4
ADDRLP4 8212
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 8212
INDIRP4
CVPU4 4
CNSTU4 0
NEU4 $473
ADDRFP4 8
INDIRP4
ARGP4
CNSTI4 34
ARGI4
ADDRLP4 8216
ADDRGP4 strchr
CALLP4
ASGNP4
ADDRLP4 8216
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $471
LABELV $473
line 1259
;1259:	{
line 1260
;1260:		Com_Printf ("Can't use keys or values with a \"\n");
ADDRGP4 $454
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1261
;1261:		return;
ADDRGP4 $462
JUMPV
LABELV $471
line 1264
;1262:	}
;1263:
;1264:	Info_RemoveKey_Big (s, key);
ADDRFP4 0
INDIRP4
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRGP4 Info_RemoveKey_Big
CALLV
pop
line 1265
;1265:	if (!value || !strlen(value))
ADDRLP4 8220
ADDRFP4 8
INDIRP4
ASGNP4
ADDRLP4 8220
INDIRP4
CVPU4 4
CNSTU4 0
EQU4 $476
ADDRLP4 8220
INDIRP4
ARGP4
ADDRLP4 8224
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8224
INDIRI4
CNSTI4 0
NEI4 $474
LABELV $476
line 1266
;1266:		return;
ADDRGP4 $462
JUMPV
LABELV $474
line 1268
;1267:
;1268:	Com_sprintf (newi, sizeof(newi), "\\%s\\%s", key, value);
ADDRLP4 0
ARGP4
CNSTI4 8192
ARGI4
ADDRGP4 $458
ARGP4
ADDRFP4 4
INDIRP4
ARGP4
ADDRFP4 8
INDIRP4
ARGP4
ADDRGP4 Com_sprintf
CALLV
pop
line 1270
;1269:
;1270:	if (strlen(newi) + strlen(s) > BIG_INFO_STRING)
ADDRLP4 0
ARGP4
ADDRLP4 8228
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 8232
ADDRGP4 strlen
CALLI4
ASGNI4
ADDRLP4 8228
INDIRI4
ADDRLP4 8232
INDIRI4
ADDI4
CNSTI4 8192
LEI4 $477
line 1271
;1271:	{
line 1272
;1272:		Com_Printf ("BIG Info string length exceeded\n");
ADDRGP4 $479
ARGP4
ADDRGP4 Com_Printf
CALLV
pop
line 1273
;1273:		return;
ADDRGP4 $462
JUMPV
LABELV $477
line 1276
;1274:	}
;1275:
;1276:	strcat (s, newi);
ADDRFP4 0
INDIRP4
ARGP4
ADDRLP4 0
ARGP4
ADDRGP4 strcat
CALLP4
pop
line 1277
;1277:}
LABELV $462
endproc Info_SetValueForKey_Big 8236 20
export Q_irand
proc Q_irand 8 0
line 1282
;1278:
;1279:
;1280://rww - convience function..
;1281:int Q_irand(int value1, int value2)
;1282:{
line 1285
;1283:	int r;
;1284:
;1285:	r = rand()%value2;
ADDRLP4 4
ADDRGP4 rand
CALLI4
ASGNI4
ADDRLP4 0
ADDRLP4 4
INDIRI4
ADDRFP4 4
INDIRI4
MODI4
ASGNI4
line 1286
;1286:	r += value1;
ADDRLP4 0
ADDRLP4 0
INDIRI4
ADDRFP4 0
INDIRI4
ADDI4
ASGNI4
line 1288
;1287:	
;1288:	return r;
ADDRLP4 0
INDIRI4
RETI4
LABELV $480
endproc Q_irand 8 0
bss
align 4
LABELV com_lines
skip 4
align 1
LABELV com_parsename
skip 1024
align 1
LABELV com_token
skip 1024
import irand
import flrand
import Rand_Init
import Com_Printf
import Com_Error
import Info_RemoveKey_big
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
LABELV $479
char 1 66
char 1 73
char 1 71
char 1 32
char 1 73
char 1 110
char 1 102
char 1 111
char 1 32
char 1 115
char 1 116
char 1 114
char 1 105
char 1 110
char 1 103
char 1 32
char 1 108
char 1 101
char 1 110
char 1 103
char 1 116
char 1 104
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
LABELV $461
char 1 73
char 1 110
char 1 102
char 1 111
char 1 32
char 1 115
char 1 116
char 1 114
char 1 105
char 1 110
char 1 103
char 1 32
char 1 108
char 1 101
char 1 110
char 1 103
char 1 116
char 1 104
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
LABELV $458
char 1 92
char 1 37
char 1 115
char 1 92
char 1 37
char 1 115
char 1 0
align 1
LABELV $454
char 1 67
char 1 97
char 1 110
char 1 39
char 1 116
char 1 32
char 1 117
char 1 115
char 1 101
char 1 32
char 1 107
char 1 101
char 1 121
char 1 115
char 1 32
char 1 111
char 1 114
char 1 32
char 1 118
char 1 97
char 1 108
char 1 117
char 1 101
char 1 115
char 1 32
char 1 119
char 1 105
char 1 116
char 1 104
char 1 32
char 1 97
char 1 32
char 1 34
char 1 10
char 1 0
align 1
LABELV $450
char 1 67
char 1 97
char 1 110
char 1 39
char 1 116
char 1 32
char 1 117
char 1 115
char 1 101
char 1 32
char 1 107
char 1 101
char 1 121
char 1 115
char 1 32
char 1 111
char 1 114
char 1 32
char 1 118
char 1 97
char 1 108
char 1 117
char 1 101
char 1 115
char 1 32
char 1 119
char 1 105
char 1 116
char 1 104
char 1 32
char 1 97
char 1 32
char 1 115
char 1 101
char 1 109
char 1 105
char 1 99
char 1 111
char 1 108
char 1 111
char 1 110
char 1 10
char 1 0
align 1
LABELV $446
char 1 67
char 1 97
char 1 110
char 1 39
char 1 116
char 1 32
char 1 117
char 1 115
char 1 101
char 1 32
char 1 107
char 1 101
char 1 121
char 1 115
char 1 32
char 1 111
char 1 114
char 1 32
char 1 118
char 1 97
char 1 108
char 1 117
char 1 101
char 1 115
char 1 32
char 1 119
char 1 105
char 1 116
char 1 104
char 1 32
char 1 97
char 1 32
char 1 92
char 1 10
char 1 0
align 1
LABELV $442
char 1 73
char 1 110
char 1 102
char 1 111
char 1 95
char 1 83
char 1 101
char 1 116
char 1 86
char 1 97
char 1 108
char 1 117
char 1 101
char 1 70
char 1 111
char 1 114
char 1 75
char 1 101
char 1 121
char 1 58
char 1 32
char 1 111
char 1 118
char 1 101
char 1 114
char 1 115
char 1 105
char 1 122
char 1 101
char 1 32
char 1 105
char 1 110
char 1 102
char 1 111
char 1 115
char 1 116
char 1 114
char 1 105
char 1 110
char 1 103
char 1 0
align 1
LABELV $411
char 1 73
char 1 110
char 1 102
char 1 111
char 1 95
char 1 82
char 1 101
char 1 109
char 1 111
char 1 118
char 1 101
char 1 75
char 1 101
char 1 121
char 1 95
char 1 66
char 1 105
char 1 103
char 1 58
char 1 32
char 1 111
char 1 118
char 1 101
char 1 114
char 1 115
char 1 105
char 1 122
char 1 101
char 1 32
char 1 105
char 1 110
char 1 102
char 1 111
char 1 115
char 1 116
char 1 114
char 1 105
char 1 110
char 1 103
char 1 0
align 1
LABELV $385
char 1 73
char 1 110
char 1 102
char 1 111
char 1 95
char 1 82
char 1 101
char 1 109
char 1 111
char 1 118
char 1 101
char 1 75
char 1 101
char 1 121
char 1 58
char 1 32
char 1 111
char 1 118
char 1 101
char 1 114
char 1 115
char 1 105
char 1 122
char 1 101
char 1 32
char 1 105
char 1 110
char 1 102
char 1 111
char 1 115
char 1 116
char 1 114
char 1 105
char 1 110
char 1 103
char 1 0
align 1
LABELV $351
char 1 73
char 1 110
char 1 102
char 1 111
char 1 95
char 1 86
char 1 97
char 1 108
char 1 117
char 1 101
char 1 70
char 1 111
char 1 114
char 1 75
char 1 101
char 1 121
char 1 58
char 1 32
char 1 111
char 1 118
char 1 101
char 1 114
char 1 115
char 1 105
char 1 122
char 1 101
char 1 32
char 1 105
char 1 110
char 1 102
char 1 111
char 1 115
char 1 116
char 1 114
char 1 105
char 1 110
char 1 103
char 1 0
align 1
LABELV $348
char 1 0
align 1
LABELV $337
char 1 67
char 1 111
char 1 109
char 1 95
char 1 115
char 1 112
char 1 114
char 1 105
char 1 110
char 1 116
char 1 102
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
char 1 32
char 1 111
char 1 102
char 1 32
char 1 37
char 1 105
char 1 32
char 1 105
char 1 110
char 1 32
char 1 37
char 1 105
char 1 10
char 1 0
align 1
LABELV $334
char 1 67
char 1 111
char 1 109
char 1 95
char 1 115
char 1 112
char 1 114
char 1 105
char 1 110
char 1 116
char 1 102
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
char 1 101
char 1 100
char 1 32
char 1 98
char 1 105
char 1 103
char 1 98
char 1 117
char 1 102
char 1 102
char 1 101
char 1 114
char 1 0
align 1
LABELV $313
char 1 81
char 1 95
char 1 115
char 1 116
char 1 114
char 1 99
char 1 97
char 1 116
char 1 58
char 1 32
char 1 97
char 1 108
char 1 114
char 1 101
char 1 97
char 1 100
char 1 121
char 1 32
char 1 111
char 1 118
char 1 101
char 1 114
char 1 102
char 1 108
char 1 111
char 1 119
char 1 101
char 1 100
char 1 0
align 1
LABELV $263
char 1 81
char 1 95
char 1 115
char 1 116
char 1 114
char 1 110
char 1 99
char 1 112
char 1 121
char 1 122
char 1 58
char 1 32
char 1 100
char 1 101
char 1 115
char 1 116
char 1 115
char 1 105
char 1 122
char 1 101
char 1 32
char 1 60
char 1 32
char 1 49
char 1 0
align 1
LABELV $260
char 1 81
char 1 95
char 1 115
char 1 116
char 1 114
char 1 110
char 1 99
char 1 112
char 1 121
char 1 122
char 1 58
char 1 32
char 1 78
char 1 85
char 1 76
char 1 76
char 1 32
char 1 115
char 1 114
char 1 99
char 1 0
align 1
LABELV $257
char 1 81
char 1 95
char 1 115
char 1 116
char 1 114
char 1 110
char 1 99
char 1 112
char 1 121
char 1 122
char 1 58
char 1 32
char 1 78
char 1 85
char 1 76
char 1 76
char 1 32
char 1 100
char 1 101
char 1 115
char 1 116
char 1 0
align 1
LABELV $221
char 1 41
char 1 0
align 1
LABELV $216
char 1 40
char 1 0
align 1
LABELV $197
char 1 77
char 1 97
char 1 116
char 1 99
char 1 104
char 1 84
char 1 111
char 1 107
char 1 101
char 1 110
char 1 58
char 1 32
char 1 37
char 1 115
char 1 32
char 1 33
char 1 61
char 1 32
char 1 37
char 1 115
char 1 0
align 1
LABELV $103
char 1 87
char 1 65
char 1 82
char 1 78
char 1 73
char 1 78
char 1 71
char 1 58
char 1 32
char 1 37
char 1 115
char 1 44
char 1 32
char 1 108
char 1 105
char 1 110
char 1 101
char 1 32
char 1 37
char 1 100
char 1 58
char 1 32
char 1 37
char 1 115
char 1 10
char 1 0
align 1
LABELV $99
char 1 69
char 1 82
char 1 82
char 1 79
char 1 82
char 1 58
char 1 32
char 1 37
char 1 115
char 1 44
char 1 32
char 1 108
char 1 105
char 1 110
char 1 101
char 1 32
char 1 37
char 1 100
char 1 58
char 1 32
char 1 37
char 1 115
char 1 10
char 1 0
align 1
LABELV $93
char 1 37
char 1 115
char 1 0
align 1
LABELV $73
char 1 37
char 1 115
char 1 37
char 1 115
char 1 0
