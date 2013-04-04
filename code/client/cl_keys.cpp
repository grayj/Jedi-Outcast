// leave this as first line for PCH reasons...
//
#include "../server/exe_headers.h"


#include "client.h"
#include "client_ui.h"

/*

key up events are sent even if in console mode

*/

field_t	historyEditLines[COMMAND_HISTORY];

int			nextHistoryLine;		// the last line in the history buffer, not masked
int			historyLine;	// the line being displayed from history buffer
							// will be <= nextHistoryLine

field_t		g_consoleField;
field_t		chatField;

qboolean	key_overstrikeMode;
qboolean	key_wastab;		// Hit tab once already? 
char		keymatch_part[256];
char		keymatch_last[256];

qboolean	anykeydown;
qkey_t		keys[MAX_KEYS];


typedef struct {
	char	*name;
	int		keynum;
} keyname_t;

// names not in this list can either be lowercase ascii, or '0xnn' hex sequences
keyname_t keynames[] =
{
	{"TAB", K_TAB},
	{"ENTER", K_ENTER},
	{"ESCAPE", K_ESCAPE},
	{"SPACE", K_SPACE},
	{"BACKSPACE", K_BACKSPACE},
	{"UPARROW", K_UPARROW},
	{"DOWNARROW", K_DOWNARROW},
	{"LEFTARROW", K_LEFTARROW},
	{"RIGHTARROW", K_RIGHTARROW},

	{"ALT", K_ALT},
	{"CTRL", K_CTRL},
	{"SHIFT", K_SHIFT},

	{"CAPSLOCK", K_CAPSLOCK},
	
	{"F1", K_F1},
	{"F2", K_F2},
	{"F3", K_F3},
	{"F4", K_F4},
	{"F5", K_F5},
	{"F6", K_F6},
	{"F7", K_F7},
	{"F8", K_F8},
	{"F9", K_F9},
	{"F10", K_F10},
	{"F11", K_F11},
	{"F12", K_F12},

	{"INS", K_INS},
	{"DEL", K_DEL},
	{"PGDN", K_PGDN},
	{"PGUP", K_PGUP},
	{"HOME", K_HOME},
	{"END", K_END},

	{"MOUSE1", K_MOUSE1},
	{"MOUSE2", K_MOUSE2},
	{"MOUSE3", K_MOUSE3},
	{"MOUSE4", K_MOUSE4},
	{"MOUSE5", K_MOUSE5},

	{"MWHEELUP",	K_MWHEELUP },
	{"MWHEELDOWN",	K_MWHEELDOWN },

	{"JOY1", K_JOY1},
	{"JOY2", K_JOY2},
	{"JOY3", K_JOY3},
	{"JOY4", K_JOY4},
	{"JOY5", K_JOY5},
	{"JOY6", K_JOY6},
	{"JOY7", K_JOY7},
	{"JOY8", K_JOY8},
	{"JOY9", K_JOY9},
	{"JOY10", K_JOY10},
	{"JOY11", K_JOY11},
	{"JOY12", K_JOY12},
	{"JOY13", K_JOY13},
	{"JOY14", K_JOY14},
	{"JOY15", K_JOY15},
	{"JOY16", K_JOY16},
	{"JOY17", K_JOY17},
	{"JOY18", K_JOY18},
	{"JOY19", K_JOY19},
	{"JOY20", K_JOY20},
	{"JOY21", K_JOY21},
	{"JOY22", K_JOY22},
	{"JOY23", K_JOY23},
	{"JOY24", K_JOY24},
	{"JOY25", K_JOY25},
	{"JOY26", K_JOY26},
	{"JOY27", K_JOY27},
	{"JOY28", K_JOY28},
	{"JOY29", K_JOY29},
	{"JOY30", K_JOY30},
	{"JOY31", K_JOY31},
	{"JOY32", K_JOY32},

	{"AUX1", K_AUX1},
	{"AUX2", K_AUX2},
	{"AUX3", K_AUX3},
	{"AUX4", K_AUX4},
	{"AUX5", K_AUX5},
	{"AUX6", K_AUX6},
	{"AUX7", K_AUX7},
	{"AUX8", K_AUX8},
	{"AUX9", K_AUX9},
	{"AUX10", K_AUX10},
	{"AUX11", K_AUX11},
	{"AUX12", K_AUX12},
	{"AUX13", K_AUX13},
	{"AUX14", K_AUX14},
	{"AUX15", K_AUX15},
	{"AUX16", K_AUX16},

	{"KP_HOME",			K_KP_HOME },
	{"KP_UPARROW",		K_KP_UPARROW },
	{"KP_PGUP",			K_KP_PGUP },
	{"KP_LEFTARROW",	K_KP_LEFTARROW },
	{"KP_5",			K_KP_5 },
	{"KP_RIGHTARROW",	K_KP_RIGHTARROW },
	{"KP_END",			K_KP_END },
	{"KP_DOWNARROW",	K_KP_DOWNARROW },
	{"KP_PGDN",			K_KP_PGDN },
	{"KP_ENTER",		K_KP_ENTER },
	{"KP_INS",			K_KP_INS },
	{"KP_DEL",			K_KP_DEL },
	{"KP_SLASH",		K_KP_SLASH },
	{"KP_MINUS",		K_KP_MINUS },
	{"KP_PLUS",			K_KP_PLUS },
	{"KP_NUMLOCK",		K_KP_NUMLOCK },
	{"KP_STAR",			K_KP_STAR },
	{"KP_EQUALS",		K_KP_EQUALS },

	{"PAUSE", K_PAUSE},
	
	{"SEMICOLON", ';'},	// because a raw semicolon seperates commands

	{"COMMAND", K_COMMAND},	//mac

	{NULL,0}
};

//english printed keynames, for when we want what's printed to be different
//from the "technical" bind label
keyname_t keynames_e[] =
{
	{"TAB", K_TAB},
	{"ENTER", K_ENTER},
	{"ESCAPE", K_ESCAPE},
	{"SPACE", K_SPACE},
	{"BACKSPACE", K_BACKSPACE},
	{"UP", K_UPARROW},
	{"DOWN", K_DOWNARROW},
	{"LEFT", K_LEFTARROW},
	{"RIGHT", K_RIGHTARROW},

	{"ALT", K_ALT},
	{"CTRL", K_CTRL},
	{"SHIFT", K_SHIFT},

	{"COMMAND", K_COMMAND},

	{"CAPSLOCK", K_CAPSLOCK},

	
	{"F1", K_F1},
	{"F2", K_F2},
	{"F3", K_F3},
	{"F4", K_F4},
	{"F5", K_F5},
	{"F6", K_F6},
	{"F7", K_F7},
	{"F8", K_F8},
	{"F9", K_F9},
	{"F10", K_F10},
	{"F11", K_F11},
	{"F12", K_F12},

	{"INS", K_INS},
	{"DEL", K_DEL},
	{"PGDN", K_PGDN},
	{"PGUP", K_PGUP},
	{"HOME", K_HOME},
	{"END", K_END},

	{"MOUSE1", K_MOUSE1},
	{"MOUSE2", K_MOUSE2},
	{"MOUSE3", K_MOUSE3},
	{"MOUSE4", K_MOUSE4},
	{"MOUSE5", K_MOUSE5},

	{"MWHEELUP",	K_MWHEELUP },
	{"MWHEELDOWN",	K_MWHEELDOWN },

	{"JOY1", K_JOY1},
	{"JOY2", K_JOY2},
	{"JOY3", K_JOY3},
	{"JOY4", K_JOY4},
	{"JOY5", K_JOY5},
	{"JOY6", K_JOY6},
	{"JOY7", K_JOY7},
	{"JOY8", K_JOY8},
	{"JOY9", K_JOY9},
	{"JOY10", K_JOY10},
	{"JOY11", K_JOY11},
	{"JOY12", K_JOY12},
	{"JOY13", K_JOY13},
	{"JOY14", K_JOY14},
	{"JOY15", K_JOY15},
	{"JOY16", K_JOY16},
	{"JOY17", K_JOY17},
	{"JOY18", K_JOY18},
	{"JOY19", K_JOY19},
	{"JOY20", K_JOY20},
	{"JOY21", K_JOY21},
	{"JOY22", K_JOY22},
	{"JOY23", K_JOY23},
	{"JOY24", K_JOY24},
	{"JOY25", K_JOY25},
	{"JOY26", K_JOY26},
	{"JOY27", K_JOY27},
	{"JOY28", K_JOY28},
	{"JOY29", K_JOY29},
	{"JOY30", K_JOY30},
	{"JOY31", K_JOY31},
	{"JOY32", K_JOY32},

	{"AUX1", K_AUX1},
	{"AUX2", K_AUX2},
	{"AUX3", K_AUX3},
	{"AUX4", K_AUX4},
	{"AUX5", K_AUX5},
	{"AUX6", K_AUX6},
	{"AUX7", K_AUX7},
	{"AUX8", K_AUX8},
	{"AUX9", K_AUX9},
	{"AUX10", K_AUX10},
	{"AUX11", K_AUX11},
	{"AUX12", K_AUX12},
	{"AUX13", K_AUX13},
	{"AUX14", K_AUX14},
	{"AUX15", K_AUX15},
	{"AUX16", K_AUX16},

	{"KP_HOME",			K_KP_HOME },
	{"KP_UP",		K_KP_UPARROW },
	{"KP_PGUP",			K_KP_PGUP },
	{"KP_LEFT",	K_KP_LEFTARROW },
	{"KP_5",			K_KP_5 },
	{"KP_RIGHT",	K_KP_RIGHTARROW },
	{"KP_END",			K_KP_END },
	{"KP_DOWN",	K_KP_DOWNARROW },
	{"KP_PGDN",			K_KP_PGDN },
	{"KP_ENTER",		K_KP_ENTER },
	{"KP_INS",			K_KP_INS },
	{"KP_DEL",			K_KP_DEL },
	{"KP_SLASH",		K_KP_SLASH },
	{"KP_MINUS",		K_KP_MINUS },
	{"KP_PLUS",			K_KP_PLUS },
	{"KP_NUMLOCK",		K_KP_NUMLOCK },
	{"KP_STAR",			K_KP_STAR },
	{"KP_EQUALS",		K_KP_EQUALS },

	{"PAUSE", K_PAUSE},
	
	{"SEMICOLON", ';'},	// because a raw semicolon seperates commands

	{NULL,0}
};

keyname_t keynames_d[] =	//deutsch
{
	{"TAB", K_TAB},
	{"EINGABETASTE", K_ENTER},
	{"ESC", K_ESCAPE},
	{"LEERTASTE", K_SPACE},
	{"RÜCKTASTE", K_BACKSPACE},
	{"PFEILT.AUF", K_UPARROW},
	{"PFEILT.UNTEN", K_DOWNARROW},
	{"PFEILT.LINKS", K_LEFTARROW},
	{"PFEILT.RECHTS", K_RIGHTARROW},

	{"ALT", K_ALT},
	{"STRG", K_CTRL},
	{"UMSCHALT", K_SHIFT},

	{"FESTSTELLT", K_CAPSLOCK},

	{"F1", K_F1},
	{"F2", K_F2},
	{"F3", K_F3},
	{"F4", K_F4},
	{"F5", K_F5},
	{"F6", K_F6},
	{"F7", K_F7},
	{"F8", K_F8},
	{"F9", K_F9},
	{"F10", K_F10},
	{"F11", K_F11},
	{"F12", K_F12},

	{"EINFG", K_INS},
	{"ENTF", K_DEL},
	{"BILD-AB", K_PGDN},
	{"BILD-AUF", K_PGUP},
	{"POS1", K_HOME},
	{"ENDE", K_END},

	{"MAUS1", K_MOUSE1},
	{"MAUS2", K_MOUSE2},
	{"MAUS3", K_MOUSE3},
	{"MAUS4", K_MOUSE4},
	{"MAUS5", K_MOUSE5},

	{"MRADOBEN",	K_MWHEELUP },
	{"MRADUNTEN",	K_MWHEELDOWN },

	{"JOY1", K_JOY1},
	{"JOY2", K_JOY2},
	{"JOY3", K_JOY3},
	{"JOY4", K_JOY4},
	{"JOY5", K_JOY5},
	{"JOY6", K_JOY6},
	{"JOY7", K_JOY7},
	{"JOY8", K_JOY8},
	{"JOY9", K_JOY9},
	{"JOY10", K_JOY10},
	{"JOY11", K_JOY11},
	{"JOY12", K_JOY12},
	{"JOY13", K_JOY13},
	{"JOY14", K_JOY14},
	{"JOY15", K_JOY15},
	{"JOY16", K_JOY16},
	{"JOY17", K_JOY17},
	{"JOY18", K_JOY18},
	{"JOY19", K_JOY19},
	{"JOY20", K_JOY20},
	{"JOY21", K_JOY21},
	{"JOY22", K_JOY22},
	{"JOY23", K_JOY23},
	{"JOY24", K_JOY24},
	{"JOY25", K_JOY25},
	{"JOY26", K_JOY26},
	{"JOY27", K_JOY27},
	{"JOY28", K_JOY28},
	{"JOY29", K_JOY29},
	{"JOY30", K_JOY30},
	{"JOY31", K_JOY31},
	{"JOY32", K_JOY32},

	{"AUX1", K_AUX1},
	{"AUX2", K_AUX2},
	{"AUX3", K_AUX3},
	{"AUX4", K_AUX4},
	{"AUX5", K_AUX5},
	{"AUX6", K_AUX6},
	{"AUX7", K_AUX7},
	{"AUX8", K_AUX8},
	{"AUX9", K_AUX9},
	{"AUX10", K_AUX10},
	{"AUX11", K_AUX11},
	{"AUX12", K_AUX12},
	{"AUX13", K_AUX13},
	{"AUX14", K_AUX14},
	{"AUX15", K_AUX15},
	{"AUX16", K_AUX16},

	{"ZB_POS1",			K_KP_HOME },
	{"ZB_PFEILT.AUF",	K_KP_UPARROW },
	{"ZB_BILD-AUF",		K_KP_PGUP },
	{"ZB_PFEILT.LINKS",	K_KP_LEFTARROW },
	{"ZB_5",			K_KP_5 },
	{"ZB_PFEILT.RECHTS",K_KP_RIGHTARROW },
	{"ZB_ENDE",			K_KP_END },
	{"ZB_PFEILT.UNTEN",	K_KP_DOWNARROW },
	{"ZB_BILD-AB",		K_KP_PGDN },
	{"ZB_ENTER",		K_KP_ENTER },
	{"ZB_EINFG",		K_KP_INS },
	{"ZB_ENTF",			K_KP_DEL },
	{"ZB_SLASH",		K_KP_SLASH },
	{"ZB_MINUS",		K_KP_MINUS },
	{"ZB_PLUS",			K_KP_PLUS },
	{"ZB_NUM",			K_KP_NUMLOCK },
	{"ZB_*",			K_KP_STAR },
	{"ZB_EQUALS",		K_KP_EQUALS },

	{"PAUSE", K_PAUSE},
	
	{"COMMAND", K_COMMAND},	//mac
	{NULL,0}
};	//end german

keyname_t keynames_f[] =	//french
{
	{"TAB", K_TAB},
	{"ENTREE",	K_ENTER},
	{"ECHAP",	K_ESCAPE},
	{"ESPACE",	K_SPACE},
	{"RETOUR",	K_BACKSPACE},
	{"HAUT",	K_UPARROW},
	{"BAS",		K_DOWNARROW},
	{"GAUCHE",	K_LEFTARROW},
	{"DROITE",	K_RIGHTARROW},

	{"ALT",		K_ALT},
	{"CTRL",	K_CTRL},
	{"MAJ",		K_SHIFT},

	{"VERRMAJ", K_CAPSLOCK},
	
	{"F1", K_F1},
	{"F2", K_F2},
	{"F3", K_F3},
	{"F4", K_F4},
	{"F5", K_F5},
	{"F6", K_F6},
	{"F7", K_F7},
	{"F8", K_F8},
	{"F9", K_F9},
	{"F10", K_F10},
	{"F11", K_F11},
	{"F12", K_F12},

	{"INSER", K_INS},
	{"SUPPR", K_DEL},
	{"PGBAS", K_PGDN},
	{"PGHAUT", K_PGUP},
	{"ORIGINE", K_HOME},
	{"FIN", K_END},

	{"SOURIS1", K_MOUSE1},
	{"SOURIS2", K_MOUSE2},
	{"SOURIS3", K_MOUSE3},
	{"SOURIS4", K_MOUSE4},
	{"SOURIS5", K_MOUSE5},

	{"MOLETTEHT.",	K_MWHEELUP },
	{"MOLETTEBAS",	K_MWHEELDOWN },

	{"JOY1", K_JOY1},
	{"JOY2", K_JOY2},
	{"JOY3", K_JOY3},
	{"JOY4", K_JOY4},
	{"JOY5", K_JOY5},
	{"JOY6", K_JOY6},
	{"JOY7", K_JOY7},
	{"JOY8", K_JOY8},
	{"JOY9", K_JOY9},
	{"JOY10", K_JOY10},
	{"JOY11", K_JOY11},
	{"JOY12", K_JOY12},
	{"JOY13", K_JOY13},
	{"JOY14", K_JOY14},
	{"JOY15", K_JOY15},
	{"JOY16", K_JOY16},
	{"JOY17", K_JOY17},
	{"JOY18", K_JOY18},
	{"JOY19", K_JOY19},
	{"JOY20", K_JOY20},
	{"JOY21", K_JOY21},
	{"JOY22", K_JOY22},
	{"JOY23", K_JOY23},
	{"JOY24", K_JOY24},
	{"JOY25", K_JOY25},
	{"JOY26", K_JOY26},
	{"JOY27", K_JOY27},
	{"JOY28", K_JOY28},
	{"JOY29", K_JOY29},
	{"JOY30", K_JOY30},
	{"JOY31", K_JOY31},
	{"JOY32", K_JOY32},

	{"AUX1", K_AUX1},
	{"AUX2", K_AUX2},
	{"AUX3", K_AUX3},
	{"AUX4", K_AUX4},
	{"AUX5", K_AUX5},
	{"AUX6", K_AUX6},
	{"AUX7", K_AUX7},
	{"AUX8", K_AUX8},
	{"AUX9", K_AUX9},
	{"AUX10", K_AUX10},
	{"AUX11", K_AUX11},
	{"AUX12", K_AUX12},
	{"AUX13", K_AUX13},
	{"AUX14", K_AUX14},
	{"AUX15", K_AUX15},
	{"AUX16", K_AUX16},

	{"PN_ORIGINE",		K_KP_HOME },
	{"PN_HAUT",			K_KP_UPARROW },
	{"PN_PGBAS",		K_KP_PGUP },
	{"PN_GAUCHE",		K_KP_LEFTARROW },
	{"PN_5",			K_KP_5 },
	{"PN_DROITE",		K_KP_RIGHTARROW },
	{"PN_FIN",			K_KP_END },
	{"PN_BAS",			K_KP_DOWNARROW },
	{"PN_PGBAS",		K_KP_PGDN },
	{"PN_ENTR",			K_KP_ENTER },
	{"PN_INSER",		K_KP_INS },
	{"PN_SUPPR",		K_KP_DEL },
	{"PN_SLASH",		K_KP_SLASH },
	{"PN_MOINS",		K_KP_MINUS },
	{"PN_PLUS",			K_KP_PLUS },
	{"PN_VERRNUM",		K_KP_NUMLOCK },
	{"PN_*",			K_KP_STAR },
	{"PN_EQUALS",		K_KP_EQUALS },

	{"PAUSE", K_PAUSE},
	
	{"COMMAND", K_COMMAND},	//mac

	{"POINT-VIRGULE",	';' },	// because a raw semicolon seperates commands

	{NULL,0}
};	//end french

/*
=============================================================================

EDIT FIELDS

=============================================================================
*/


/*
===================
Field_Draw

Handles horizontal scrolling and cursor blinking
x, y, amd width are in pixels
===================
*/
void Field_VariableSizeDraw( field_t *edit, int x, int y, int width, int size, qboolean showCursor ) {
	int		len;
	int		drawLen;
	int		prestep;
	int		cursorChar;
	char	str[MAX_STRING_CHARS];
	int		i;

	drawLen = edit->widthInChars;
	len = strlen( edit->buffer ) + 1;

	// guarantee that cursor will be visible
	if ( len <= drawLen ) {
		prestep = 0;
	} else {
		if ( edit->scroll + drawLen > len ) {
			edit->scroll = len - drawLen;
			if ( edit->scroll < 0 ) {
				edit->scroll = 0;
			}
		}
		prestep = edit->scroll;
/*
		if ( edit->cursor < len - drawLen ) {
			prestep = edit->cursor;	// cursor at start
		} else {
			prestep = len - drawLen;
		}
*/
	}

	if ( prestep + drawLen > len ) {
		drawLen = len - prestep;
	}


	// extract <drawLen> characters from the field at <prestep>
	if ( drawLen >= MAX_STRING_CHARS ) {
		Com_Error( ERR_DROP, "drawLen >= MAX_STRING_CHARS" );
	}
	memcpy( str, edit->buffer + prestep, drawLen );
	str[ drawLen ] = 0;

	// draw it
	if ( size == SMALLCHAR_WIDTH ) {
		for ( i = 0 ; i < drawLen-1 ; i++ ) {
			SCR_DrawSmallChar( x + i * SMALLCHAR_WIDTH, y, str[i] );
		}
	} else {
		// draw big string with drop shadow
		SCR_DrawBigString( x, y, str, 1.0 );
	}

	// draw the cursor
	if ( !showCursor ) {
		return;
	}

	if ( (int)( cls.realtime >> 8 ) & 1 ) {
		return;		// off blink
	}

	if ( key_overstrikeMode ) {
		cursorChar = 11;
	} else {
		cursorChar = 10;
	}
	if ( size == SMALLCHAR_WIDTH ) {
		SCR_DrawSmallChar( x + ( edit->cursor - prestep ) * size, y, cursorChar );
	} else {
		str[0] = cursorChar;
		str[1] = 0;
		SCR_DrawBigString( x + ( edit->cursor - prestep ) * size, y, str, 1.0 );
	}
}

void Field_Draw( field_t *edit, int x, int y, int width, qboolean showCursor ) 
{
	Field_VariableSizeDraw( edit, x, y, width, SMALLCHAR_WIDTH, showCursor );
}

void Field_BigDraw( field_t *edit, int x, int y, int width, qboolean showCursor ) 
{
	Field_VariableSizeDraw( edit, x, y, width, BIGCHAR_WIDTH, showCursor );
}

/*
================
Field_Paste
================
*/
void Field_Paste( field_t *edit ) {
	char	*cbd;
	int		pasteLen, i;

	cbd = Sys_GetClipboardData();

	if ( !cbd ) {
		return;
	}

	// send as if typed, so insert / overstrike works properly
	pasteLen = strlen( cbd );
	for ( i = 0 ; i < pasteLen ; i++ ) {
		Field_CharEvent( edit, cbd[i] );
	}

	Z_Free( cbd );
}

/*
=================
Field_KeyDownEvent

Performs the basic line editing functions for the console,
in-game talk, and menu fields

Key events are used for non-printable characters, others are gotten from char events.
=================
*/
void Field_KeyDownEvent( field_t *edit, int key ) {
	int		len;

	// shift-insert is paste
	if ( ( ( key == K_INS ) || ( key == K_KP_INS ) ) && keys[K_SHIFT].down ) {
		Field_Paste( edit );
		return;
	}

	len = strlen( edit->buffer );

	if ( key == K_DEL ) {
		if ( edit->cursor < len ) {
			memmove( edit->buffer + edit->cursor, 
				edit->buffer + edit->cursor + 1, len - edit->cursor );
		}
		return;
	}

	if ( key == K_RIGHTARROW ) 
	{
		if ( edit->cursor < len ) {
			edit->cursor++;
		}
		if ( edit->cursor >= edit->scroll + edit->widthInChars && edit->cursor <= len )
		{
			edit->scroll++;
		}
		return;
	}

	if ( key == K_LEFTARROW ) 
	{
		if ( edit->cursor > 0 ) {
			edit->cursor--;
		}
		if ( edit->cursor < edit->scroll )
		{
			edit->scroll--;
		}
		return;
	}

	if ( key == K_HOME || ( tolower(key) == 'a' && keys[K_CTRL].down ) ) {
		edit->cursor = 0;
		return;
	}

	if ( key == K_END || ( tolower(key) == 'e' && keys[K_CTRL].down ) ) {
		edit->cursor = len;
		return;
	}

	if ( key == K_INS ) {
		key_overstrikeMode = !key_overstrikeMode;
		return;
	}
}

/*
==================
Field_CharEvent
==================
*/
void Field_CharEvent( field_t *edit, int ch ) {
	int		len;

	if ( ch == 'v' - 'a' + 1 ) {	// ctrl-v is paste
		Field_Paste( edit );
		return;
	}

	if ( ch == 'c' - 'a' + 1 ) {	// ctrl-c clears the field
		Field_Clear( edit );
		return;
	}

	len = strlen( edit->buffer );

	if ( ch == 'h' - 'a' + 1 )	{	// ctrl-h is backspace
		if ( edit->cursor > 0 ) {
			memmove( edit->buffer + edit->cursor - 1, 
				edit->buffer + edit->cursor, len + 1 - edit->cursor );
			edit->cursor--;
			if ( edit->cursor < edit->scroll )
			{
				edit->scroll--;
			}
		}
		return;
	}

	if ( ch == 'a' - 'a' + 1 ) {	// ctrl-a is home
		edit->cursor = 0;
		edit->scroll = 0;
		return;
	}

	if ( ch == 'e' - 'a' + 1 ) {	// ctrl-e is end
		edit->cursor = len;
		edit->scroll = edit->cursor - edit->widthInChars;
		return;
	}

	//
	// ignore any other non printable chars
	//
	if ( ch < 32 ) {
		return;
	}

	if ( key_overstrikeMode ) {	
		if ( edit->cursor == MAX_EDIT_LINE - 1 )
			return;
		edit->buffer[edit->cursor] = ch;
		edit->cursor++;
	} else {	// insert mode
		if ( len == MAX_EDIT_LINE - 1 ) {
			return; // all full
		}
		memmove( edit->buffer + edit->cursor + 1, 
			edit->buffer + edit->cursor, len + 1 - edit->cursor );
		edit->buffer[edit->cursor] = ch;
		edit->cursor++;
	}

	if ( edit->cursor >= edit->widthInChars )
	{
		edit->scroll++;
	}

	if ( edit->cursor == len + 1) {
		edit->buffer[edit->cursor] = 0;
	}
}

/*
==================
Field_Clear
==================
*/
void Field_Clear( field_t *edit ) {
	edit->buffer[0] = 0;
	edit->cursor = 0;
	edit->scroll = 0;
}

/*
=============================================================================

CONSOLE LINE EDITING

==============================================================================
*/
char *Cmd_CompleteCommandNext (char *partial, char *last);
char *Cvar_CompleteVariableNext (char *partial, char *last);

/*
===============
CompleteCommand

Tab expansion
===============
*/
void CompleteCommand( void ) 
{
	char		*cmd;
	field_t	*edit;
	static qboolean checking_cmd;

	edit = &g_consoleField;

	if (key_wastab)
	{
		if (checking_cmd)			
		{
			cmd = Cmd_CompleteCommandNext(keymatch_part, keymatch_last);
			if(!cmd)
			{
				checking_cmd = qfalse;  //go to checking variables
				cmd = Cvar_CompleteVariableNext(keymatch_part, NULL);
				if(!cmd)
				{
					checking_cmd = qtrue;  //go back to checking cmd
					cmd = Cmd_CompleteCommandNext(keymatch_part, NULL);
				}	
			}	
		}
		else
		{
			cmd = Cvar_CompleteVariableNext(keymatch_part, keymatch_last);
			if(!cmd)
			{
				checking_cmd = qtrue;  //go back to checking cmd
				cmd = Cmd_CompleteCommandNext(keymatch_part, NULL);
				if(!cmd)
				{
					checking_cmd = qfalse;  //go to checking variables
					cmd = Cvar_CompleteVariableNext(keymatch_part, NULL);
				}	
			}	
		}
		if (cmd)
		{
			strcpy(keymatch_last, cmd);
			key_wastab= qtrue;
			Com_sprintf( edit->buffer, sizeof( edit->buffer ), "%s ", cmd );
			edit->cursor = strlen( edit->buffer );
			return;
		}
	}
	else
	{
		strcpy(keymatch_part, edit->buffer);
		checking_cmd = qtrue;
		cmd = Cmd_CompleteCommand(edit->buffer);
		if (!cmd)
		{	//means no cmds, so check cvars only
			checking_cmd= qfalse;
			cmd = Cvar_CompleteVariable (edit->buffer);
		}
		if (cmd)
		{
			Com_sprintf( edit->buffer, sizeof( edit->buffer ), "%s ", cmd );
			edit->cursor = strlen( edit->buffer );
			strcpy(keymatch_last, cmd);
			key_wastab= qtrue;
			return;
		}

	}

}


/*
====================
Console_Key

Handles history and console scrollback
====================
*/
void Console_Key (int key) {
	// ctrl-L clears screen
	if ( key == 'l' && keys[K_CTRL].down ) {
		Cbuf_AddText ("clear\n");
		key_wastab = qfalse;	// For double tabbing on a cvar
		return;
	}

/*	extern qboolean SwallowBadNumLockedKPKey( int iKey );
	if (SwallowBadNumLockedKPKey(key)){
		return;
	}
*/
	// enter finishes the line
	if ( key == K_ENTER || key == K_KP_ENTER ) {
		Cbuf_AddText( g_consoleField.buffer );	// valid command
		Cbuf_AddText ("\n");
		Com_Printf ( "]%s\n", g_consoleField.buffer );

		// copy line to history buffer
		historyEditLines[nextHistoryLine % COMMAND_HISTORY] = g_consoleField;
		nextHistoryLine++;
		historyLine = nextHistoryLine;

		Field_Clear( &g_consoleField );
		g_consoleField.widthInChars = g_console_field_width;
		if ( cls.state == CA_DISCONNECTED ) {
			SCR_UpdateScreen ();	// force an update, because the command
		}							// may take some time
		key_wastab = qfalse;	// For double tabbing on a cvar
		return;
	}

	// command completion

	if (key == K_TAB) {
		CompleteCommand();
		return;
	}

	key_wastab = qfalse;	// For double tabbing on a cvar

	// command history (ctrl-p ctrl-n for unix style)

	if ( ( key == K_UPARROW ) || ( key == K_KP_UPARROW ) ||
		 ( ( tolower(key) == 'p' ) && keys[K_CTRL].down ) )
	{
		if ( nextHistoryLine - historyLine < COMMAND_HISTORY 
			&& historyLine > 0 ) {
			historyLine--;
		}
		g_consoleField = historyEditLines[ historyLine % COMMAND_HISTORY ];
		return;
	}

	if ( ( key == K_DOWNARROW ) || ( key == K_KP_DOWNARROW ) ||
		 ( ( tolower(key) == 'n' ) && keys[K_CTRL].down ) )
	{
		if (historyLine == nextHistoryLine)
			return;
		historyLine++;
		g_consoleField = historyEditLines[ historyLine % COMMAND_HISTORY ];
		return;
	}

	// console scrolling
	if ( key == K_PGUP ) {
		Con_PageUp();
		return;
	}

	if ( key == K_PGDN ) {
		Con_PageDown();
		return;
	}

	// ctrl-home = top of console
	if ( key == K_HOME && keys[K_CTRL].down ) {
		Con_Top();
		return;
	}

	// ctrl-end = bottom of console
	if ( key == K_END && keys[K_CTRL].down ) {
		Con_Bottom();
		return;
	}

	// pass to the normal editline routine
	Field_KeyDownEvent( &g_consoleField, key );
}

//============================================================================


/*
================
Message_Key

In game talk message
================
*/
void Message_Key( int key ) {
	char	buffer[MAX_STRING_CHARS];

	if (key == K_ESCAPE) {
		cls.keyCatchers &= ~KEYCATCH_MESSAGE;
		Field_Clear( &chatField );
		return;
	}

	if ( key == K_ENTER || key == K_KP_ENTER )
	{
		if ( chatField.buffer[0] && cls.state == CA_ACTIVE ) {
			Com_sprintf( buffer, sizeof( buffer ), "say \"%s\"\n", chatField.buffer );

			CL_AddReliableCommand( buffer );
		}
		cls.keyCatchers &= ~KEYCATCH_MESSAGE;
		Field_Clear( &chatField );
		return;
	}

	Field_KeyDownEvent( &chatField, key );
}

//============================================================================


qboolean Key_GetOverstrikeMode( void ) {
	return key_overstrikeMode;
}


void Key_SetOverstrikeMode( qboolean state ) {
	key_overstrikeMode = state;
}


/*
===================
Key_IsDown
===================
*/
qboolean Key_IsDown( int keynum ) {
	if ( keynum == -1 ) {
		return qfalse;
	}

	return keys[keynum].down;
}


/*
===================
Key_StringToKeynum

Returns a key number to be used to index keys[] by looking at
the given string.  Single ascii characters return themselves, while
the K_* names are matched up.

0x11 will be interpreted as raw hex, which will allow new controlers
to be configured even if they don't have defined names.
===================
*/
int Key_StringToKeynum( char *str ) {
	keyname_t	*kn;
	
	if ( !str || !str[0] ) {
		return -1;
	}
	if ( !str[1] ) {
		return str[0];
	}

	// check for hex code
	if ( str[0] == '0' && str[1] == 'x' && strlen( str ) == 4) {
		int		n1, n2;
		
		n1 = str[2];
		if ( n1 >= '0' && n1 <= '9' ) {
			n1 -= '0';
		} else if ( n1 >= 'a' && n1 <= 'f' ) {
			n1 = n1 - 'a' + 10;
		} else {
			n1 = 0;
		}

		n2 = str[3];
		if ( n2 >= '0' && n2 <= '9' ) {
			n2 -= '0';
		} else if ( n2 >= 'a' && n2 <= 'f' ) {
			n2 = n2 - 'a' + 10;
		} else {
			n2 = 0;
		}

		return n1 * 16 + n2;
	}

	// scan for a text match
	for ( kn=keynames ; kn->name ; kn++ ) {
		if ( !Q_stricmp( str,kn->name ) )
			return kn->keynum;
	}

	return -1;
}

/*
===================
Key_KeynumToString

Returns a string (either a single ascii char, a K_* name, or a 0x11 hex string) for the
given keynum.
===================
*/
char *Key_KeynumToString( int keynum, qboolean bTranslate ) {	//note: translate is only called for menu display not configs
	keyname_t	*kn;	
	static	char	tinystr[5];
	int			i, j;

	if ( keynum == -1 ) {
		return "<KEY NOT FOUND>";
	}

	if ( keynum < 0 || keynum > 255 ) {
		return "<OUT OF RANGE>";
	}

	// check for printable ascii (don't use quote)
	if ( keynum > 32 && keynum < 127 && keynum != '"' ) {
		tinystr[0] = keynum;
		tinystr[1] = 0;
		if (keynum == ';' && !bTranslate) {
			//fall through and use keyname table
		} else {
			return tinystr;
		}
	}


	kn=keynames;	//init to english
	if (bTranslate) {
		if ( sp_language->integer == SP_LANGUAGE_GERMAN ) {
			kn=keynames_d;	//use german
		} else if ( sp_language->integer == SP_LANGUAGE_FRENCH ) {
			kn=keynames_f;	//use french
		}
		else //rww - this is actually English and doesn't need to be "translated".
		{    //however, certain key names are too long to display right, this does the trick.
			kn=keynames_e;
		}
	}

	// check for a key string
	for ( ; kn->name ; kn++ ) {
		if (keynum == kn->keynum) {
			return kn->name;
		}
	}

	// make a hex string
	i = keynum >> 4;
	j = keynum & 15;

	tinystr[0] = '0';
	tinystr[1] = 'x';
	tinystr[2] = i > 9 ? i - 10 + 'a' : i + '0';
	tinystr[3] = j > 9 ? j - 10 + 'a' : j + '0';
	tinystr[4] = 0;

	return tinystr;
}


/*
===================
Key_SetBinding
===================
*/
void Key_SetBinding( int keynum, const char *binding ) {
	if ( keynum == -1 ) {
		return;
	}

	// free old bindings
	if ( keys[ keynum ].binding ) {
		Z_Free( keys[ keynum ].binding );
	}
			
	// allocate memory for new binding
	keys[keynum].binding = CopyString( binding );

	// consider this like modifying an archived cvar, so the
	// file write will be triggered at the next oportunity
	cvar_modifiedFlags |= CVAR_ARCHIVE;
}


/*
===================
Key_GetBinding
===================
*/
char *Key_GetBinding( int keynum ) {
	if ( keynum == -1 ) {
		return "";
	}

	assert (keynum < (sizeof(keys)/sizeof(keys[0])));
	return keys[ keynum ].binding;
}


/*
===================
Key_Unbind_f
===================
*/
void Key_Unbind_f (void)
{
	int		b;

	if (Cmd_Argc() != 2)
	{
		Com_Printf ("unbind <key> : remove commands from a key\n");
		return;
	}
	
	b = Key_StringToKeynum (Cmd_Argv(1));
	if (b==-1)
	{
		Com_Printf ("\"%s\" isn't a valid key\n", Cmd_Argv(1));
		return;
	}

	Key_SetBinding (b, "");
}

/*
===================
Key_Unbindall_f
===================
*/
void Key_Unbindall_f (void)
{
	int		i;
	
	for (i=0 ; i<256 ; i++)
		if (keys[i].binding)
			Key_SetBinding (i, "");
}


/*
===================
Key_Bind_f
===================
*/
void Key_Bind_f (void)
{
	int			i, c, b;
	char		cmd[1024];
	
	c = Cmd_Argc();

	if (c < 2)
	{
		Com_Printf ("bind <key> [command] : attach a command to a key\n");
		return;
	}
	b = Key_StringToKeynum (Cmd_Argv(1));
	if (b==-1)
	{
		Com_Printf ("\"%s\" isn't a valid key\n", Cmd_Argv(1));
		return;
	}

	if (c == 2)
	{
		if (keys[b].binding)
			Com_Printf ("\"%s\" = \"%s\"\n", Cmd_Argv(1), keys[b].binding );
		else
			Com_Printf ("\"%s\" is not bound\n", Cmd_Argv(1) );
		return;
	}
	
// copy the rest of the command line
	cmd[0] = 0;		// start out with a null string
	for (i=2 ; i< c ; i++)
	{
		strcat (cmd, Cmd_Argv(i));
		if (i != (c-1))
			strcat (cmd, " ");
	}

	Key_SetBinding (b, cmd);
}

/*
============
Key_WriteBindings

Writes lines containing "bind key value"
============
*/
void Key_WriteBindings( fileHandle_t f ) {
	int		i;

	FS_Printf (f, "unbindall\n" );
	for (i=0 ; i<256 ; i++) {
		if (keys[i].binding && keys[i].binding[0] ) {
			FS_Printf (f, "bind %s \"%s\"\n", Key_KeynumToString(i, qfalse), keys[i].binding);
		}
	}
}


/*
============
Key_Bindlist_f

============
*/
void Key_Bindlist_f( void ) {
	int		i;

	for ( i = 0 ; i < 256 ; i++ ) {
		if ( keys[i].binding && keys[i].binding[0] ) {
			Com_Printf( "%s \"%s\"\n", Key_KeynumToString(i, qfalse), keys[i].binding );
		}
	}
}


/*
===================
CL_InitKeyCommands
===================
*/
void CL_InitKeyCommands( void ) {
	// register our functions
	Cmd_AddCommand ("bind",Key_Bind_f);
	Cmd_AddCommand ("unbind",Key_Unbind_f);
	Cmd_AddCommand ("unbindall",Key_Unbindall_f);
	Cmd_AddCommand ("bindlist",Key_Bindlist_f);
}

/*
===================
CL_KeyEvent

Called by the system for both key up and key down events
===================
*/
void CL_KeyEvent (int key, qboolean down, unsigned time) {
	char	*kb;
	char	cmd[1024];

	// update auto-repeat status and BUTTON_ANY status
	keys[key].down = down;

	if (down) {
		keys[key].repeats++;
		if ( keys[key].repeats == 1) {
			anykeydown++;
		}
	} else {
		keys[key].repeats = 0;
		anykeydown--;
		if (anykeydown < 0) {
			anykeydown = 0;
		}
	}

	// console key is hardcoded, so the user can never unbind it
	if (key == '`' || key == '~') {
		if (!down ) {
			return;

		}
#ifdef FINAL_BUILD
		if (!(cls.keyCatchers & KEYCATCH_CONSOLE) && !keys[K_SHIFT].down )	//we're not in the console
		{//so we require the control keys to get in
			return;
		}
#endif
		Con_ToggleConsole_f ();
		return;
	}

	// most keys during demo playback will bring up the menu, but non-ascii

	// keys can still be used for bound actions
	if ( down && (key < 128 || key == K_MOUSE1) 
//		&& ( cls.state == CA_CINEMATIC ) && !cls.keyCatchers ) 
		&& ( cls.state == CA_CINEMATIC || CL_IsRunningInGameCinematic()) && !cls.keyCatchers ) 
	{
		SCR_StopCinematic(true);
		return;
//		Cvar_Set ("nextdemo","");
//		key = K_ESCAPE;
	}

	// escape is always handled special
	if ( key == K_ESCAPE && down ) 
	{
		if ( cls.keyCatchers & KEYCATCH_MESSAGE ) 
		{
			// clear message mode
			Message_Key( key );
			return;
		}

		if ( !( cls.keyCatchers & KEYCATCH_UI ) ) 
		{
			if ( cls.state == CA_ACTIVE ) 
			{
				UI_SetActiveMenu( "ingame",NULL );
			}
			else 
			{
				CL_Disconnect_f();
				UI_SetActiveMenu( "mainMenu",NULL );
			}
			return;
		}

		_UI_KeyEvent( key,down);
		return;
	}

	//
	// key up events only perform actions if the game key binding is
	// a button command (leading + sign).  These will be processed even in
	// console mode and menu mode, to keep the character from continuing 
	// an action started before a mode switch.
	//
	if (!down) {
		kb = keys[key].binding;
		if (kb && kb[0] == '+') {
			// button commands add keynum and time as parms so that multiple
			// sources can be discriminated and subframe corrected
			Com_sprintf (cmd, sizeof(cmd), "-%s %i %i\n", kb+1, key, time);
			Cbuf_AddText (cmd);
		}
		return;
	}


	// distribute the key down event to the apropriate handler
	if ( cls.keyCatchers & KEYCATCH_CONSOLE ) {
		Console_Key( key );
	} else if ( cls.keyCatchers & KEYCATCH_UI ) {
		_UI_KeyEvent( key,down );
	} else if ( cls.keyCatchers & KEYCATCH_MESSAGE ) {
		Message_Key( key );
	} else if ( cls.state == CA_DISCONNECTED ) {

		Console_Key( key );

	} else {
		// send the bound action
		kb = keys[key].binding;
		if ( !kb ) {
			if (key >= 200) {
				Com_Printf ("%s is unbound, use controls menu to set.\n"
					, Key_KeynumToString( key, qfalse ) );
			}
		} else if (kb[0] == '+') {	
			// button commands add keynum and time as parms so that multiple
			// sources can be discriminated and subframe corrected
			Com_sprintf (cmd, sizeof(cmd), "%s %i %i\n", kb, key, time);
			Cbuf_AddText (cmd);
		} else {
			// down-only command
			Cbuf_AddText (kb);
			Cbuf_AddText ("\n");
		}
	}
}


/*
===================
CL_CharEvent

Normal keyboard characters, already shifted / capslocked / etc
===================
*/
void CL_CharEvent( int key ) {
	// the console key should never be used as a char
	if ( key == '`' || key == '~' ) {
		return;
	}

	// distribute the key down event to the apropriate handler
	if ( cls.keyCatchers & KEYCATCH_CONSOLE )
	{
		Field_CharEvent( &g_consoleField, key );
	}
	else if ( cls.keyCatchers & KEYCATCH_UI )
	{
		_UI_KeyEvent( key | K_CHAR_FLAG, qtrue );
	}
	else if ( cls.keyCatchers & KEYCATCH_MESSAGE ) 
	{
		Field_CharEvent( &chatField, key );
	}
	else if ( cls.state == CA_DISCONNECTED )
	{
		Field_CharEvent( &g_consoleField, key );
	}
}


/*
===================
Key_ClearStates
===================
*/
void Key_ClearStates (void)
{
	int		i;

	anykeydown = qfalse;

	for ( i=0 ; i < MAX_KEYS ; i++ ) {
		if ( keys[i].down ) {
			CL_KeyEvent( i, qfalse, 0 );
		}
		keys[i].down = 0;
		keys[i].repeats = 0;
	}
}

