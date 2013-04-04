// map.h -- the state of the current world that all views are displaying

extern	char		currentmap[1024];

// head/tail of doubly linked lists
extern	brush_t	active_brushes;	// brushes currently being displayed
extern	brush_t	selected_brushes;	// highlighted
extern	face_t	*selected_face;
extern	brush_t	*selected_face_brush;
extern	brush_t	filtered_brushes;	// brushes that have been filtered or regioned

extern	entity_t	entities;
extern	entity_t	*world_entity;	// the world entity is NOT included in
									// the entities chain

extern	qboolean	modified;		// for quit confirmations

extern	vec3_t	region_mins, region_maxs;
extern	qboolean	region_active;

void 	Map_LoadFile (const char *filename);
bool	Map_SaveFile (const char *filename, qboolean use_region);
void	Map_New (void);
void	Map_BuildBrushData(void);

void	Map_RegionOff (void);
void	Map_RegionXY (void);
void	Map_RegionTallBrush (void);
void	Map_RegionBrush (void);
void	Map_RegionSelectedBrushes (void);
qboolean Map_IsBrushFiltered (brush_t *b);

void Map_SaveSelected(CMemFile* pMemFile, CMemFile* pPatchFile = NULL);
void Map_ImportBuffer (char* buf);
