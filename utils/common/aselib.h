#include "cmdlib.h"
#include "mathlib.h"
#include "polyset.h"

typedef enum
{
	FRAMECOPY_NONE = 0,
	FRAMECOPY_1TO1,
	FRAMECOPY_FILL
} framecopy_type;


void		ASE_Load( const char *filename, qboolean verbose, qboolean meshanims, int type );
int			ASE_GetNumSurfaces( void );
polyset_t	*ASE_GetSurfaceAnimation( int ndx, int *numFrames, int skipFrameStart, int skipFrameEnd, int maxFrames );
const char  *ASE_GetSurfaceName( int ndx );
void		ASE_Free( void );

void		ASE_InitForGrabbing(void);
qboolean	ASE_CatGrabbedFrames(const int nSourceFrame, int nDestFrame, int nNumFrames, const int nFillFrame);
void		ASE_FreeGrab(void);
void		ASE_CopyFromConcatBuffer(void);
