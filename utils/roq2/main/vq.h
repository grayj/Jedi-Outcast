/******************************************************************************
 *
 * NAME
 *    vq.h
 *    J. R. Goldschneider 5/93
 *
 * DESCRIPTION
 *    Macros used in all of the programs block.c unblock.c stdvq.c stdvqe.c
 *    are defined here.
 *
 *****************************************************************************/
#ifndef _vq_H_INCLUDED
#define _vq_H_INCLUDED

#define VQDATA           double
//double
#define DATA             double /* unsigned char*/
#define BOOLEAN          int

#ifndef TRUE
#define TRUE             1
#endif  //TRUE

#ifndef FALSE
#define FALSE            0
#endif //FALSE

#define NAME_MAXIMUM     100
#define DEF_INNAME       "hello"
#define DEF_OUTNAME      "hello"
#define DEF_APPEND_CB    ".CB"
#define DEF_APPEND_VQ    ".VQ"
#define DEF_APPEND_TS    ".TS"
#define DEF_APPEND_RS    ".RS"
#define NAME_MAX         NAME_MAXIMUM - 8

/* defaults for the blocking programs */
#define DEF_ROWS           512
#define DEF_COLS           512
#define DEF_BLOCKWIDTH     4*3
#define DEF_BLOCKHEIGHT    4

/* defaults for the VQ programs */
#define DEF_DIMENSION    16
#define DEF_CODEBOOKSIZE 256
#define DEF_THRESHOLD    0.01
#define DEF_OFFSET_ADD   0.01
#define DEF_OFFSET_MUL   0.01
#define DEF_SPEEDUP      1

#define USAGE            "Usage:"
#define HOWTOUSE_BLOCK   "-i f1 -o f2 -r rows -l columns -h blockheight -w blockwidth"
#define HOWTOUSE_STDVQ   "-i f1 -c f2 -d dimension -f codebooksize \n     -h threshold -a addoffset -m multoffset -s speedup -W"
#define HOWTOUSE_STDVQE  "-i f1 -c f2 -o f3 -d dimension -f codebooksize -s speedup -D"
#define NOTFOUND         "not found"
#define NOTOPTION        "unknown option"
#define NOMEMORY         "insufficient memory"
#define NOREAD           "error reading data"
#define NOWRITE          "error writing data"
#define AND              "and" 
#define ARESAME          "are identical"
#define ABORT_BLOCK      "image not blocked"
#define ABORT_UNBLOCK    "image not unblocked"
#define ABORT_STDVQ      "codebook not generated"
#define ABORT_STDVQE     "data not encoded"
#define DISTORTION       "Codebook distortion of size"
#define NOFILL           "Unable to fill"
#define TRYFILL          "Attempting to fill"
#define EMPTYCELLS       "empty cells of codebook"
#define EMPTYCELL        "empty cell of codebook"
#define STOP             "stopping with codebook"
#define OVERFLOWED       "distortion overflow"

#endif _vq_H_INCLUDED
