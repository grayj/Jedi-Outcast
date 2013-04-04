#ifdef _SOF2
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#if !defined(G2_GORE_H_INC)
#define G2_GORE_H_INC

#define MAX_LODS (8)
struct GoreTextureCoordinates
{
	float *tex[MAX_LODS];

	GoreTextureCoordinates()
	{
		int i;
		for (i=0;i<MAX_LODS;i++)
		{
			tex[i]=0;
		}
	}
	~GoreTextureCoordinates()
	{
		int i;
		for (i=0;i<MAX_LODS;i++)
		{
			if(tex[i])
			{
				Z_Free(tex[i]);
			}
		}
	}
};

int AllocGoreRecord();
GoreTextureCoordinates *FindGoreRecord(int tag);
void DeleteGoreRecord(int tag);

struct SGoreSurface
{
	int			mGoreShaderEnum;
	int			mGoreTag;
};

class CGoreSet
{
public:
	int mMyGoreSetTag;
	multimap<int,SGoreSurface> mGoreRecords; // a map from surface index
	CGoreSet(int tag) : mMyGoreSetTag(tag) {}
	~CGoreSet();
};

CGoreSet	*FindGoreSet(int goreSetTag);
CGoreSet	*NewGoreSet();
void		DeleteGoreSet(int goreSetTag);

#endif // G2_GORE_H_INC
#endif // _SOF2