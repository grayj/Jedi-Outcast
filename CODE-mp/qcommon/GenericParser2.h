#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
#if !defined(GENERICPARSER2_H_INC)
#define GENERICPARSER2_H_INC

#ifdef DEBUG_LINKING
	#pragma message("...including GenericParser2.h")
#endif

#include "disablewarnings.h"

class CTextPool;
class CGPObject;

class CTextPool
{
private:
	char		*mPool;
	CTextPool	*mNext;
	int			mSize, mUsed;

public:
	CTextPool(int initSize = 10240);
	~CTextPool(void);

	CTextPool	*GetNext(void) { return mNext; }
	void		SetNext(CTextPool *which) { mNext = which; }
	char		*GetPool(void) { return mPool; }
	int			GetUsed(void) { return mUsed; }

	char		*AllocText(char *text, bool addNULL = true, CTextPool **poolPtr = 0);
};

class CGPObject
{
protected:
	const char	*mName;
	CGPObject	*mNext, *mInOrderNext, *mInOrderPrevious;

public:
	CGPObject(const char *initName);

	const char	*GetName(void) { return mName; }

	CGPObject	*GetNext(void) { return mNext; }
	void		SetNext(CGPObject *which) { mNext = which; }
	CGPObject	*GetInOrderNext(void) { return mInOrderNext; }
	void		SetInOrderNext(CGPObject *which) { mInOrderNext = which; }
	CGPObject	*GetInOrderPrevious(void) { return mInOrderPrevious; }
	void		SetInOrderPrevious(CGPObject *which) { mInOrderPrevious = which; }

	bool		WriteText(CTextPool **textPool, const char *text);
};



class CGPValue : public CGPObject
{
private:
	CGPObject	*mList;

public:
	CGPValue(const char *initName, const char *initValue = 0);
	~CGPValue(void);

	bool			IsList(void);
	const char		*GetTopValue(void);
	CGPObject		*GetList(void) { return mList; }
	void			AddValue(const char *newValue);

	void		Parse(char **dataPtr, CTextPool **textPool);

	bool		Write(CTextPool **textPool, int depth);
};



class CGPGroup : public CGPObject
{
private:
	CGPValue			*mPairs, *mInOrderPairs;
	CGPValue			*mCurrentPair;
	CGPGroup			*mSubGroups, *mInOrderSubGroups;
	CGPGroup			*mCurrentSubGroup;
	CGPGroup			*mParent;
	bool				mWriteable;

	void	SortObject(CGPObject *object, CGPObject **unsortedList, CGPObject **sortedList, 
					   CGPObject **lastObject);

public:
	CGPGroup(const char *initName = "Top Level", CGPGroup *initParent = 0);
	~CGPGroup(void);

	void	Clean(void); 

	void		SetWriteable(const bool writeable) { mWriteable = writeable; }
	CGPValue	*GetPairs(void) { return mPairs; }
	CGPValue	*GetInOrderPairs(void) { return mInOrderPairs; }
	CGPGroup	*GetSubGroups(void) { return mSubGroups; }
	CGPGroup	*GetInOrderSubGroups(void) { return mInOrderSubGroups; }

	CGPValue	*AddPair(const char *name, const char *value);
	CGPGroup	*AddGroup(const char *name);
	CGPGroup	*FindSubGroup(const char *name);
	void		Parse(char **dataPtr, CTextPool **textPool);
	bool		Write(CTextPool **textPool, int depth);

	const char	*FindPairValue(const char *key, const char *defaultVal = 0);
};

class CGenericParser2
{
private:
	CGPGroup		mTopLevel;
	CTextPool		*mTextPool;
	bool			mWriteable;

public:
	CGenericParser2(void);
	~CGenericParser2(void);

	void		SetWriteable(const bool writeable) { mWriteable = writeable; }
	CGPGroup	*GetBaseParseGroup(void) { return &mTopLevel; }

	void	Parse(char **dataPtr, bool cleanFirst = true, bool writeable = false);
	void	Parse(char *dataPtr, bool cleanFirst = true, bool writeable = false)
	{
		Parse(&dataPtr, cleanFirst, writeable);
	}
	void	Clean(void);

	bool	Write(CTextPool *textPool);
};

#endif // GENERICPARSER2_H_INC
