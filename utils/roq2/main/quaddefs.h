

#include "im.h"



#ifndef _quadDefs_H_INCLUDED

#define _quadDefs_H_INCLUDED





#define DEP 0

#define FCC 1

#define CCC 2

#define SLD 3

#define PAT 4

#define MOT 5

#define DEAD 6



#define COLA 0

#define COLB 1

#define COLC 2

#define COLS 3

#define COLPATA 4

#define COLPATB 5

#define COLPATS 6

#define GENERATION 7



#define CCCBITMAP 0

#define FCCDOMAIN 1

#define PATNUMBER 2

#define PATNUMBE2 3

#define PATNUMBE3 4

#define PATNUMBE4 5

#define PATNUMBE5 6



#define MAXSIZE		16

#define MINSIZE		4



#define RoQ_ID 			0x1084

#define	RoQ_QUAD		0x1000

#define	RoQ_PUZZLE_QUAD	0x1003

#define RoQ_QUAD_HANG	0x1013

#define	RoQ_QUAD_SMALL	0x1010

#define	RoQ_QUAD_INFO	0x1001

#define RoQ_QUAD_VQ		0x1011

#define RoQ_QUAD_JPEG	0x1012

#define RoQ_QUAD_CODEBOOK		0x1002



typedef struct {

	byte	size;				//	32, 16, 8, or 4

	word	xat;				// where is it at on the screen

	word	yat;				// 

} shortQuadCel;



typedef struct {

	byte	size;				//	32, 16, 8, or 4

	word	xat;				// where is it at on the screen

	word	yat;				// 



	float	cccsnr;				// ccc bitmap snr to actual image

	float	fccsnr;				// fcc bitmap snr to actual image

	float	motsnr;				// delta snr to previous image

	float	sldsnr;				// solid color snr

	float	patsnr;

	float	dctsnr;

	float	rsnr;				// what's the current snr



	unsigned int	cola;			// color a for ccc

	unsigned int	colb;			// color b for ccc

	unsigned int	colc;			// color b for ccc

	unsigned int	sldcol;			// sold color

	unsigned int	colpata;

	unsigned int	colpatb;

	unsigned int	colpats;

	unsigned int	bitmap;				// ccc bitmap

	

	word	domain;				// where to copy from for fcc

	word	patten[5];			// which pattern



	int		status;

	BOOL	mark;

	float			snr[DEAD];				// snrssss

} quadcel;



typedef struct {

	float			snr[DEAD];				// snrssss

	unsigned int	cols[8];

	unsigned int	bitmaps[7];				// ccc bitmap

} dataQuadCel;



typedef struct {

	float				normal;

	unsigned short int	index;

} norm;



typedef struct {

	unsigned char dtlMap[256];

	int	r[4];

	int g[4];

	int b[4];

	int a[4];

	float ymean;

} dtlCel;



typedef struct {

	byte	r,g,b,a;

} pPixel;



#endif //_quadDefs_H_INCLUDED