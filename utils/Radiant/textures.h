
//--typedef struct texdef_s
//--{
//--	char	name[32];
//--	float	shift[2];
//--	float	rotate;
//--	float	scale[2];
//--	int		contents;
//--	int		flags;
//--	int		value;
//--} texdef_t;
//--
//--typedef struct
//--{
//--	int			width, height;
//--	int			originy;
//--	texdef_t	texdef;
//--  int m_nTotalHeight;
//--} texturewin_t;
//--
//--#define QER_TRANS     0x00000001
//--#define QER_NOCARVE   0x00000002
//--
//--typedef struct qtexture_s
//--{
//--	struct	qtexture_s *next;
//--	char	name[64];		// includes partial directory and extension
//--  int		width,  height;
//--	int		contents;
//--	int		flags;
//--	int		value;
//--	int		texture_number;	// gl bind number
//--  
//--  char  shadername[1024]; // old shader stuff
//--  qboolean bFromShader;   // created from a shader
//--  float fTrans;           // amount of transparency
//--  int   nShaderFlags;     // qer_ shader flags
//--	vec3_t	color;			    // for flat shade mode
//--	qboolean	inuse;		    // true = is present on the level
//--} qtexture_t;
//--

// a texturename of the form (0 0 0) will
// create a solid color texture

void	Texture_Init (bool bHardInit = true);
void	Texture_Flush (bool bReload = false);
void	Texture_ClearInuse (void);
void	Texture_ShowInuse (void);
void	Texture_ShowDirectory (int menunum, bool bLinked = false);

typedef enum
{
	eReplace_NO,
	eReplace_YES,
	eReplace_TIMESTAMP

} eReplace_t;

qtexture_t *Texture_ForName (const char *name, bool bForArchitecture = true, const char *psBasePath = NULL, eReplace_t eReplace = eReplace_NO, bool bPlaceHolderLoad = false);

void	Texture_Init (void);
void	Texture_SetTexture (texdef_t *texdef, bool bFitScale = false, bool bNoSystemTextureOverwrite = false);

void	Texture_SetMode(int iMenu);	// GL_TEXTURE_NEAREST, etc..
void Texture_ResetPosition();

void FreeShaders();
void LoadShaders();
void ReloadShaders();
int Texture_LoadSkin(char *pName, int *pnWidth, int *pnHeight, const char *psBasePath);
void Texture_LoadFromPlugIn(LPVOID vp);
void Texture_StartPos (void);
qtexture_t *Texture_NextPos (int *x, int *y);
