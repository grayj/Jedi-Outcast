#ifndef FX_EXPORT_H_INC
#define FX_EXPORT_H_INC

int	FX_RegisterEffect(const char *file);

void FX_PlaySimpleEffect( const char *file, vec3_t org );					// uses a default up axis
void FX_PlayEffect( const char *file, vec3_t org, vec3_t fwd );		// builds arbitrary perp. right vector, does a cross product to define up
void FX_PlayEntityEffect( const char *file, vec3_t org, 
						vec3_t axis[3], const int boltInfo, const int entNum );

void FX_PlaySimpleEffectID( int id, vec3_t org );					// uses a default up axis
void FX_PlayEffectID( int id, vec3_t org, vec3_t fwd );		// builds arbitrary perp. right vector, does a cross product to define up
void FX_PlayEntityEffectID( int id, vec3_t org, 
						vec3_t axis[3], const int boltInfo, const int entNum );
void FX_PlayBoltedEffectID( int id, sharedBoltInterface_t *fxObj );

void FX_AddScheduledEffects( void );

int			FX_InitSystem( void );	// called in CG_Init to purge the fx system.
qboolean	FX_FreeSystem( void );	// ditches all active effects;
void		FX_AdjustTime_Pos( int time, vec3_t refdef_vieworg, vec3_t refdef_viewaxis[3] );


#endif // FX_EXPORT_H_INC
