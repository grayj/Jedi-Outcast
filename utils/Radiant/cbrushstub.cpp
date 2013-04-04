#include "stdafx.h"
#include "qe3.h"

void Curve_Invert (void) 
{
}

void Curve_MakeCurvedBrush (qboolean negative, qboolean top, qboolean bottom, 
					qboolean s1, qboolean s2, qboolean s3, qboolean s4) 
{
}


void Curve_CameraDraw (brush_t *b) 
{
}

void Curve_XYDraw (brush_t *b) 
{
}

void Curve_WriteFile (char *name) 
{
}

void Curve_StripFakePlanes( brush_t *b )
{
}

void Curve_AddFakePlanes( brush_t *b ) 
{
}


void Patch_BrushToMesh(){};
void Patch_GenericMesh(int nWidth, int nHeight, int nOrientation){};
void Patch_WriteFile (char *name){}; 
void Patch_Move(int n, const vec3_t vMove){};
void Patch_EditPatch(int n){};
void Patch_Scale(int n, const vec3_t vOrigin, const vec3_t vAmt){};
bool g_bShowPatchBounds;

/*
bool g_bPatchWireFrame;
void Patch_ApplyMatrix(int n, const vec3_t vOrigin, const vec3_t vMatrix[3]){};
void Patch_Deselect(){};
void Patch_Select(int n){};
void Patch_Cleanup(){};
void Patch_Delete(int n){};
void Patch_BuildPoints (brush_t *b){};
void Curve_BuildPoints (brush_t *b) 
{
}
void Patch_ReadFile (char *name){};
*/