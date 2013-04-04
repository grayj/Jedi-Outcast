#include "stdafx.h"

#include "qe3.h"


#define	MAX_POINTFILE	8192
static vec3_t	s_pointvecs[MAX_POINTFILE];
static int		s_num_points, s_check_point;

// returns NULL if all ok, else error message...
//
char *Pointfile_Delete(void)
{
	char   *psReturn = NULL;
	char	name[1024];

	strcpy (name, currentmap);
	StripExtension (name);
	strcat (name, ".lin");

	if (remove(name))
	{
		static char sTemp[1024];
		sprintf(sTemp,"Error deleting file \"%s\"",name);
		if (errno == EACCES)	// path specifies a read-only file
		{
			strcat(sTemp," because it's read-only.");
			psReturn = &sTemp[0];
		}
		else
		if (errno != ENOENT)	// not found, or path is a dir
		{
			// so anything else is probably...
			//
			strcat(sTemp," because it's probably still open or something?");
			psReturn = &sTemp[0];
		}
		else
		{
			// (not found) harmless, so ignore
		}

		if (psReturn)
		{
			strcat(sTemp,"\nYou should fix this, otherwise you'll keep getting erroneous leak reports!\n");			
		}
	}	

	return psReturn;
}

// advance camera to next point
void Pointfile_Next (void)
{
	vec3_t	dir;

	if (s_check_point >= s_num_points-2)
	{
		Sys_Status ("End of pointfile", 0);
		return;
	}
	s_check_point++;
	VectorCopy (s_pointvecs[s_check_point], g_pParentWnd->GetCamera()->Camera().origin);
	VectorCopy (s_pointvecs[s_check_point], g_pParentWnd->GetXYWnd()->GetOrigin());
	VectorSubtract (s_pointvecs[s_check_point+1], g_pParentWnd->GetCamera()->Camera().origin, dir);
	VectorNormalize (dir);
	g_pParentWnd->GetCamera()->Camera().angles[1] = atan2 (dir[1], dir[0])*180/3.14159;
	g_pParentWnd->GetCamera()->Camera().angles[0] = asin (dir[2])*180/3.14159;

	Sys_UpdateWindows (W_ALL);
}

// advance camera to previous point
void Pointfile_Prev (void)
{
	vec3_t	dir;

	if ( s_check_point == 0)
	{
		Sys_Status ("Start of pointfile", 0);
		return;
	}
	s_check_point--;
	VectorCopy (s_pointvecs[s_check_point], g_pParentWnd->GetCamera()->Camera().origin);
	VectorCopy (s_pointvecs[s_check_point], g_pParentWnd->GetXYWnd()->GetOrigin());
	VectorSubtract (s_pointvecs[s_check_point+1], g_pParentWnd->GetCamera()->Camera().origin, dir);
	VectorNormalize (dir);
	g_pParentWnd->GetCamera()->Camera().angles[1] = atan2 (dir[1], dir[0])*180/3.14159;
	g_pParentWnd->GetCamera()->Camera().angles[0] = asin (dir[2])*180/3.14159;

	Sys_UpdateWindows (W_ALL);
}

void Pointfile_Check (void)
{
	char	name[1024];
	FILE	*f;
	vec3_t	v;

	strcpy (name, currentmap);
	StripExtension (name);
	strcat (name, ".lin");

	f = fopen (name, "r");
	if (!f)
		return;

	Sys_Printf ("Reading pointfile %s\n", name);

	if (!g_qeglobals.d_pointfile_display_list)
		g_qeglobals.d_pointfile_display_list = qglGenLists(1);

	s_num_points = 0;
  qglNewList (g_qeglobals.d_pointfile_display_list,  GL_COMPILE);
	qglColor3f (1, 0, 0);
	qglDisable(GL_TEXTURE_2D);
	qglDisable(GL_TEXTURE_1D);
	qglLineWidth (4);
	qglBegin(GL_LINE_STRIP);
	do
	{
		if (fscanf (f, "%f %f %f\n", &v[0], &v[1], &v[2]) != 3)
			break;
		if (s_num_points < MAX_POINTFILE)
		{
			VectorCopy (v, s_pointvecs[s_num_points]);
			s_num_points++;
		}
		qglVertex3fv (v);
	} while (1);
	qglEnd();
	qglLineWidth (1);
	qglEndList ();

	s_check_point = 0;
	fclose (f);

	// don't jump the camera to the leak line, the designers hated this...
#if 0
	Pointfile_Next ();
#else
	Sys_UpdateWindows (W_ALL);
#endif
}

void Pointfile_Draw( void )
{
	int i;

	qglColor3f( 1.0F, 0.0F, 0.0F );
	qglDisable(GL_TEXTURE_2D);
	qglDisable(GL_TEXTURE_1D);
	qglLineWidth (4);
	qglBegin(GL_LINE_STRIP);
	for ( i = 0; i < s_num_points; i++ )
	{
		qglVertex3fv( s_pointvecs[i] );
	}
	qglEnd();
	qglLineWidth( 1 );
}

void Pointfile_Clear (void)
{
	if (!g_qeglobals.d_pointfile_display_list)
		return;

	qglDeleteLists (g_qeglobals.d_pointfile_display_list, 1);
	g_qeglobals.d_pointfile_display_list = 0;
	Sys_UpdateWindows (W_ALL);
}

