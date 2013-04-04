
#if !defined(FX_PRIMITIVES_H_INC)
	#include "FxPrimitives.h"
#endif

#ifndef FX_UTIL_H_INC
#define FX_UTIL_H_INC


bool	FX_Free( void );	// ditches all active effects;
int		FX_Init( void );	// called in CG_Init to purge the fx system.
void	FX_Add( void );		// called every cgame frame to add all fx into the scene.
void	FX_Stop( void );	// ditches all active effects without touching the templates.

bool	FX_ActiveFx(void);	// returns whether there are any active or scheduled effects

CCloud	*FX_AddCloud(void);


CParticle *FX_AddParticle( CCloud *effectCloud, vec3_t org, vec3_t vel, vec3_t accel, 
							float size1, float size2, float sizeParm, 
							float alpha1, float alpha2, float alphaParm, 
							vec3_t rgb1, vec3_t rgb2, float rgbParm,
							float rotation, float rotationDelta,
							vec3_t min, vec3_t max, float elasticity, 
							int deathID, int impactID,
							int killTime, qhandle_t shader, int flags );

#ifndef EFFECTSED
#ifndef CHC // Sof2 only
CParticle *FX_AddParticle( CCloud *effectCloud, CFxBoltInterface *obj, vec3_t vel, vec3_t accel, float size1, float size2, float sizeParm,
							float alpha1, float alpha2, float alphaParm,
							vec3_t rgb1, vec3_t rgb2, float rgbParm,
							float rotation, float rotationDelta,
							int killTime, qhandle_t shader, int flags, bool objCopy );
#endif
#endif

CLine *FX_AddLine( CCloud *effectCloud, vec3_t start, vec3_t end, 
							float size1, float size2, float sizeParm,
							float alpha1, float alpha2, float alphaParm,
							vec3_t rgb1, vec3_t rgb2, float rgbParm,
							int killTime, qhandle_t shader, int flags );

CElectricity *FX_AddElectricity( CCloud *effectCloud, vec3_t start, vec3_t end, float size1, float size2, float sizeParm,
							float alpha1, float alpha2, float alphaParm,
							vec3_t sRGB, vec3_t eRGB, float rgbParm,
							float chaos, int killTime, qhandle_t shader, int flags );

CTail *FX_AddTail( CCloud *effectCloud, vec3_t org, vec3_t vel, vec3_t accel, 
							float size1, float size2, float sizeParm, 
							float length1, float length2, float lengthParm,
							float alpha1, float alpha2, float alphaParm,
							vec3_t rgb1, vec3_t rgb2, float rgbParm,
							vec3_t min, vec3_t max, float elasticity, 
							int deathID, int impactID,
							int killTime, qhandle_t shader, int flags );

CCylinder *FX_AddCylinder( CCloud *effectCloud, vec3_t start, vec3_t normal, 
							float size1s, float size1e, float size1Parm,
							float size2s, float size2e, float size2Parm,
							float length1, float length2, float lengthParm,
							float alpha1, float alpha2, float alphaParm,
							vec3_t rgb1, vec3_t rgb2, float rgbParm,
							int killTime, qhandle_t shader, int flags );

CEmitter *FX_AddEmitter( CCloud *effectCloud, vec3_t org, vec3_t vel, vec3_t accel,
							float size1, float size2, float sizeParm,
							float alpha1, float alpha2, float alphaParm,
							vec3_t rgb1, vec3_t rgb2, float rgbParm,
							vec3_t angs, vec3_t deltaAngs,
							vec3_t min, vec3_t max, float elasticity, 
							int deathID, int impactID, int emitterID,
							float density, float variance,
							int killTime, qhandle_t model, int flags );

CLight *FX_AddLight( CCloud *effectCloud, vec3_t org, float size1, float size2, float sizeParm,
							vec3_t rgb1, vec3_t rgb2, float rgbParm,
							int killTime, int flags );

COrientedParticle *FX_AddOrientedParticle(CCloud *effectCloud, vec3_t org, vec3_t norm, vec3_t vel, vec3_t accel,
							float size1, float size2, float sizeParm,
							float alpha1, float alpha2, float alphaParm,
							vec3_t rgb1, vec3_t rgb2, float rgbParm,
							float rotation, float rotationDelta,
							vec3_t min, vec3_t max, float bounce,
							int deathID, int impactID,
							int killTime, qhandle_t shader, int flags );

CPoly *FX_AddPoly( CCloud *effectCloud, vec3_t *verts, vec2_t *st, int numVerts,
							vec3_t vel, vec3_t accel,
							float alpha1, float alpha2, float alphaParm,
							vec3_t rgb1, vec3_t rgb2, float rgbParm,
							vec3_t rotationDelta, float bounce, int motionDelay,
							int killTime, qhandle_t shader, int flags );

CBezier *FX_AddBezier( vec3_t start, vec3_t end, 
								vec3_t control1, vec3_t control1Vel,
								vec3_t control2, vec3_t control2Vel,
								float size1, float size2, float sizeParm,
								float alpha1, float alpha2, float alphaParm,
								vec3_t sRGB, vec3_t eRGB, float rgbParm,
								int killTime, qhandle_t shader, int flags );

CFlash *FX_AddFlash( CCloud *effectCloud, vec3_t origin, vec3_t sRGB, vec3_t eRGB, float rgbParm,
						int killTime, qhandle_t shader, int flags );


#ifdef CHC
// Included for backwards compatibility with CHC and for doing quick programmatic effects.
void FX_AddSprite( CCloud *effectCloud, vec3_t origin, vec3_t vel, vec3_t accel, 
							float scale, float dscale, 
							float sAlpha, float eAlpha, 
							float rotation, float bounce, 
							int life, qhandle_t shader, int flags = 0 );

void FX_AddSprite( CCloud *effectCloud, vec3_t origin, vec3_t vel, vec3_t accel, 
							float scale, float dscale, 
							float sAlpha, float eAlpha, 
							vec3_t sRGB, vec3_t eRGB, 
							float rotation, float bounce, 
							int life, qhandle_t shader, int flags = 0 );

void FX_AddLine( CCloud *effectCloud, vec3_t start, vec3_t end, float stScale, 
							float width, float dwidth, 
							float sAlpha, float eAlpha, 
							int life, qhandle_t shader, int flags = 0 );

void FX_AddLine( CCloud *effectCloud, vec3_t start, vec3_t end, float stScale, 
							float width, float dwidth, 
							float sAlpha, float eAlpha, 
							vec3_t sRGB, vec3_t eRGB, 
							int life, qhandle_t shader, int flags = 0 );
#endif // CHC

#endif //FX_UTIL_H_INC