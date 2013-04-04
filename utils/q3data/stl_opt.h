// Filename:-	stl_opt.h
//


#ifndef STL_OPT_H
#define STL_OPT_H


#ifdef __cplusplus
extern "C"
{
#endif



void TriangleAbutList_Gen(int mesh[][3], int iNumTris, int iMaxVerts);
const int* TriangleAbutList_Query(int iTriangle, int *piCount);




#ifdef __cplusplus
}
#endif


#endif	// #ifndef STL_OPT_H

//////////////// eof ////////////////
