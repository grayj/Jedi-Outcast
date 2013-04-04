
// entity.h


//--#define	MAX_FLAGS	8
//--
//--
//--typedef struct trimodel_t
//--{
//--  vec3_t v[3];
//--  float  st[3][2];
//--} trimodel;
//--
//--typedef struct entitymodel_t
//--{
//--  entitymodel_t *pNext;
//--  int nTriCount;
//--  trimodel *pTriList;
//--  int nTextureBind;
//--  int nSkinWidth;
//--  int nSkinHeight;
//--  int	nModelPosition;
//--} entitymodel;
//--
//--
//--typedef struct eclass_s
//--{
//--	struct eclass_s *next;
//--	char	*name;
//--	qboolean	fixedsize;
//--	qboolean	unknown;		// wasn't found in source
//--	vec3_t	mins, maxs;
//--	vec3_t	color;
//--	texdef_t	texdef;
//--	char	*comments;
//--	char	flagnames[MAX_FLAGS][32];
//--
//--/*
//--  int nTriCount;
//--  trimodel *pTriList;
//--  int nTextureBind;
//--  int nSkinWidth, nSkinHeight;
//--*/
//--  entitymodel *model;
//--  char	*modelpath;
//--  char	*skinpath;
//--  int   nFrame;
//--} eclass_t;
//--
//--extern	eclass_t	*eclass;

void Eclass_InitForSourceDirectory (char *path);
eclass_t *Eclass_ForName (char *name, qboolean has_brushes);

//===================================================


typedef struct epair_s
{
	struct epair_s	*next;
	char	*key;
	char	*value;
} epair_t;

typedef struct entity_s
{
	struct	entity_s	*prev, *next;
	brush_t		brushes;	// head/tail of list
	vec3_t		origin;
	eclass_t	*eclass;
	epair_t		*epairs;
  eclass_t  *md3Class;
} entity_t;

int		GetNumKeys(entity_t *ent);
char	*GetKeyString(entity_t *ent, int iIndex);
char 	*ValueForKey (entity_t *ent, LPCSTR key);
void 	SetKeyValue (entity_t *ent, LPCSTR key, LPCSTR value, bool bDoTracking = true);
void 	DeleteKey (entity_t *ent, LPCSTR key);
float	FloatForKey (entity_t *ent, LPCSTR key);
int		IntForKey (entity_t *ent, LPCSTR key);
bool	GetVectorForKey (entity_t *ent, LPCSTR key, vec3_t vec);

void		Entity_Free (entity_t *e);
entity_t	*Entity_Parse (qboolean onlypairs, brush_t* pList = NULL);
void Entity_Write (entity_t *e, FILE *f, qboolean use_region);
void Entity_WriteSelected(entity_t *e, FILE *f);
void Entity_WriteSelected(entity_t *e, CMemFile*);
entity_t	*Entity_Create (eclass_t *c);
entity_t	*Entity_Clone (entity_t *e);

void		Entity_LinkBrush (entity_t *e, brush_t *b);
void		Entity_UnlinkBrush (brush_t *b);
entity_t *FindEntity(const char *pszKey, const char *pszValue);
entity_t *FindEntityInt(const char *pszKey, int iValue);

int GetUniqueTargetId(int iHint);
int Eclass_hasModel(eclass_t *e, vec3_t &vMin, vec3_t &vMax);
eclass_t* GetCachedModel(entity_t *pEntity, const char *pName, vec3_t &vMin, vec3_t &vMax);

extern entity_t *gEntityToSetBoundsOf;

