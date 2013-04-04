/*==================*
 * TYPE DEFINITIONS *
 *==================*/

#ifndef _gdefs_H_INCLUDED
#define _gdefs_H_INCLUDED

typedef unsigned char byte;
typedef unsigned short word;

#ifndef YES
typedef char			BOOL;
#define YES             (BOOL)1
#define NO              (BOOL)0
#endif //YES

#ifndef WIN32
#define strdup NXCopyStringBuffer
#endif //strdup

#ifdef WIN32
#define strncasecmp strnicmp
#define strcasecmp stricmp
#define random rand
#endif

#define	dabs(a) (((a)<0) ? -(a) : (a))
#define CLAMP(v,l,h) ((v)<(l) ? (l) : (v)>(h) ? (h) : v)
#define	swap(a,b) { a^=b; b^=a; a^=b; }
#define lum(a) ( 0.2990*(a>>16) + 0.5870*((a>>8)&0xff) + 0.1140*(a&0xff) )
#define gsign(a)  	((a) < 0 ? -1 : 1)
#define mnint(a)	((a) < 0 ? (int)(a - 0.5) : (int)(a + 0.5))
#define mmax(a, b)  	((a) > (b) ? (a) : (b))
#define mmin(a, b)  	((a) < (b) ? (a) : (b))
#define glimit(x) ( (x)<(0) ? (0) : (x)>(255) ? (255) : x)
#define RGBDIST( src0, src1 ) ( ((src0[0]-src1[0])*(src0[0]-src1[0])) + \
								((src0[1]-src1[1])*(src0[1]-src1[1])) + \
								((src0[2]-src1[2])*(src0[2]-src1[2])) )

#define RGBADIST( src0, src1 ) ( ((src0[0]-src1[0])*(src0[0]-src1[0])) + \
								 ((src0[1]-src1[1])*(src0[1]-src1[1])) + \
								 ((src0[2]-src1[2])*(src0[2]-src1[2])) + \
								 ((src0[3]-src1[3])*(src0[3]-src1[3])) )

#define Y_FROM_RGB(src) (RMULT*src[0] + GMULT*src[1] + BMULT*src[2])
#define U_FROM_RGB(src) (RIEMULT*src[0] + GIEMULT*src[1] + BIEMULT*src[2])
#define V_FROM_RGB(src) (RQEMULT*src[0] + GQEMULT*src[1] + BQEMULT*src[2])
#define YUVDIST(a,b) ( ((Y_FROM_RGB(a) - Y_FROM_RGB(b)) * (Y_FROM_RGB(a) - Y_FROM_RGB(b))) + \
					   ((U_FROM_RGB(a) - U_FROM_RGB(b)) * (U_FROM_RGB(a) - U_FROM_RGB(b))) + \
					   ((V_FROM_RGB(a) - V_FROM_RGB(b)) * (V_FROM_RGB(a) - V_FROM_RGB(b))) + \
					   ((a[3] - b[3]) * (a[3] - b[3])) )
#define RMULT 0.2990				// use these for televisions
#define GMULT 0.5870				
#define BMULT 0.1140
#define RIEMULT -0.16874
#define RQEMULT  0.50000
#define GIEMULT -0.33126
#define GQEMULT -0.41869
#define BIEMULT  0.50000
#define BQEMULT -0.08131

#endif //_gdefs_H_INCLUDED