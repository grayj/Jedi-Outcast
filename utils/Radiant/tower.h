// tower.h
//  routines for tower.c

#define TOWER_EPSILON 0.001f


	void DoTower(void);
	face_t* MakePlaneList(face_t* top, face_t* bottom);
	void WrapFaces( face_t* top, face_t* bottom);
	void CopyFace( face_t* in, face_t* out);
