// entity.h

#define DlgXBorder 5
#define DlgYBorder 5


enum
{
	EntList = 0,
	EntComment,
	EntCheck1,
	EntCheck2,
	EntCheck3,
	EntCheck4,
	EntCheck5,
	EntCheck6,
	EntCheck7,
	EntCheck8,
	EntCheck9,
	EntCheck10,
	EntCheck11,
	EntCheck12,
#ifdef SOF
	EntCheck13,
	EntCheck14,
	EntCheck15,
	EntCheck16,
	EntCheck17,
	EntCheck18,
	EntCheck19,
	EntCheck20,
	EntCheck21,
#endif
	EntProps,
	EntDir0,
	EntDir45,
	EntDir90,
	EntDir135,
	EntDir180,
	EntDir225,
	EntDir270,
	EntDir315,
	EntDirUp,
	EntDirDown,
	EntDelProp,
	EntKeyLabel,
	EntKeyField,
	EntValueLabel,
	EntValueField,
	EntColor,
	EntAssignSounds,
	EntAssignModels,

	EntLast,
};

extern HWND hwndEnt[EntLast];

extern int rgIds[EntLast];

