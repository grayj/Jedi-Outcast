// scriplib.h

#ifndef __CMDLIB__
#include "cmdlib.h"
#endif
#ifndef __MATHLIB__
#include "mathlib.h"
#endif

#define	MAXTOKEN	1024

extern	char	token[MAXTOKEN];
extern	char	*scriptbuffer,*script_p,*scriptend_p;
extern	int		grabbed;
extern	int		scriptline;
extern	qboolean	endofscript;


void LoadScriptFile( const char *filename );
void ParseFromMemory (char *buffer, int size);

qboolean GetToken (qboolean crossline);
void UnGetToken (void);
qboolean TokenAvailable (void);

void MatchToken( char *match );

void Parse1DMatrix (int x, vec_t *m);
void Parse2DMatrix (int y, int x, vec_t *m);
void Parse3DMatrix (int z, int y, int x, vec_t *m);

void Write1DMatrix (FILE *f, int x, vec_t *m);
void Write2DMatrix (FILE *f, int y, int x, vec_t *m);
void Write3DMatrix (FILE *f, int z, int y, int x, vec_t *m);
