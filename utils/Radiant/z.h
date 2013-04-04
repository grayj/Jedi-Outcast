
// window system independent camera view code

typedef struct
{
	int		width, height;

	qboolean	timing;

	vec3_t	origin;			// at center of window
	float	scale;
} z_t;

extern z_t z;

void Z_Init (void);
void Z_MouseDown (int x, int y, int buttons);
void Z_MouseUp (int x, int y, int buttons);
void Z_MouseMoved (int x, int y, int buttons);
void Z_Draw (void);

