
// window system independent camera view code

typedef struct
{
	int		width, height;

	qboolean	timing;

	vec3_t	origin;			// at center of window
	float	scale;

	float	topclip, bottomclip;

	qboolean d_dirty;
} xy_t;


/*BOOL*/ int FilterBrush(brush_t *pb, BOOL bCalledFromDisplayCode = false);

