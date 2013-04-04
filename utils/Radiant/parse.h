// parse.h -- text file parsing routines

#define	MAXTOKEN	1024

extern	char	token[MAXTOKEN];
extern	int		scriptline;

void	StartTokenParsing (char *data);
qboolean GetToken (qboolean crossline);
void UngetToken (void);
qboolean TokenAvailable (void);

