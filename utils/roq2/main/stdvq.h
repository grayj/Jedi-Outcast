/******************************************************************************

 *

 * NAME

 *    stdvq.h

 *    J. R. Goldschneider 5/93

 *

 * DESCRIPTION

 *    Declare global variables here.

 *

 *****************************************************************************/



extern FILE     *trainingfile;

extern char     trainingname[NAME_MAXIMUM];

extern char     codebookname[NAME_MAXIMUM];

extern int      dimension; 

extern int     codebooksize;

extern float    threshold;

extern float    offset_add;

extern float    offset_mul;

extern BOOLEAN  write_all_codebooks;

extern BOOLEAN  display_info;

extern char     programname[NAME_MAXIMUM];

