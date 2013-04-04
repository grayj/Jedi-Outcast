
// window system independent camera view code

typedef enum
{
	cd_wire,
	cd_solid,
	cd_texture,
  cd_light,
	cd_blend
} camera_draw_mode;

typedef struct
{
	int		width, height;

	qboolean	timing;

	vec3_t	origin;
	vec3_t	angles;

	camera_draw_mode	draw_mode;

	vec3_t	color;			// background 

	vec3_t	forward, right, up;	// move matrix

	vec3_t	vup, vpn, vright;	// view matrix
} camera_t;

