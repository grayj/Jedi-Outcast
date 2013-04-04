#include "stdafx.h"
#include <stdlib.h>

#define Reg1 register
int match(char *mask, char *name)
{
	Reg1	unsigned char	*m = (unsigned char *)mask, *n = (unsigned char *)name;
	char	*ma = mask, *na = name;
	int	wild = 0, q = 0;

	while (1)
	    {
		if (*m == '*')
		   {
			while (*m == '*')
				m++;
			wild = 1;
			ma = (char *)m;
			na = (char *)n;
		    }

		if (!*m)
		    {
	  		if (!*n)
				return 0;
	  		for (m--; (m > (unsigned char *)mask) && (*m == '?'); m--)
				;
			if ((*m == '*') && (m > (unsigned char *)mask) &&
			    (m[-1] != '\\'))
				return 0;
			if (!wild) 
				return 1;
			m = (unsigned char *)ma;
			n = (unsigned char *)++na;
		    }
		else if (!*n)
		    {
			while(*m == '*')
				m++;
			return (*m != 0);
		    }
		if ((*m == '\\') && ((m[1] == '*') || (m[1] == '?')))
		    {
			m++;
			q = 1;
		    }
		else
			q = 0;

		if ((tolower(*m) != tolower(*n)) && ((*m != '?') || q))
		    {
			if (!wild)
				return 1;
			m = (unsigned char *)ma;
			n = (unsigned char *)++na;
		    }
		else
		    {
			if (*m)
				m++;
			if (*n)
				n++;
		    }
	    }
}

