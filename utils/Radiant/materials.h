// Filename:-	materials.h
//
#ifndef MATERIALS_H
#define MATERIALS_H
#ifdef SOF


typedef struct
{
	char	*name;
	int		value;
} materialtype_t;

enum
{
	MATERIAL_GRAVEL,
	MATERIAL_METAL,
	MATERIAL_STONE,
	MATERIAL_WOOD,
};

extern materialtype_t	*materialtypes;


void QFile_ReadMaterialTypes(char* filename);
char *ExpandArg (char *path);


#endif	// #ifdef SOF
#endif	// #ifndef MATERIALS_H

////////////////// eof /////////////////////

