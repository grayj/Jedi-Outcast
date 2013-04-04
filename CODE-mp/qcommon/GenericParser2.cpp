// this include must remain at the top of every CPP file

#include "../game/q_shared.h"
#include "qcommon.h"

#if !defined(GENERICPARSER2_H_INC)
	#include "GenericParser2.h"
#endif

#define _EXE

#define MAX_TOKEN_SIZE	1024
static char	token[MAX_TOKEN_SIZE];

static char *GetToken(char **text, bool allowLineBreaks, bool readUntilEOL = false)
{
	char	*pointer = *text;
	int		length = 0;
	int		c = 0;
	bool	foundLineBreak;

	token[0] = 0;
	if (!pointer)
	{
		return token;
	}

	while(1)
	{
		foundLineBreak = false;
		while(1)
		{
			c = *pointer;
			if (c > ' ')
			{
				break;
			}
			if (!c)
			{
				*text = 0;
				return token;
			}
			if (c == '\n')
			{
				foundLineBreak = true;
			}
			pointer++;
		}
		if (foundLineBreak && !allowLineBreaks)
		{
			*text = pointer;
			return token;
		}

		c = *pointer;

		// skip single line comment
		if (c == '/' && pointer[1] == '/')
		{
			pointer += 2;
			while (*pointer && *pointer != '\n') 
			{
				pointer++;
			}
		}
		// skip multi line comments
		else if (c == '/' && pointer[1] == '*') 
		{
			pointer += 2;
			while (*pointer && (*pointer != '*' || pointer[1] != '/')) 
			{
				pointer++;
			}
			if (*pointer) 
			{
				pointer += 2;
			}
		}
		else
		{	// found the start of a token
			break;
		}
	}

	if (c == '\"' && !readUntilEOL)
	{	// handle a string
		pointer++;
		while (1)
		{
			c = *pointer++;
			if (c == '\"')
			{
//				token[length++] = c;
				break;
			}
			else if (!c)
			{
				break;
			}
			else if (length < MAX_TOKEN_SIZE)
			{
				token[length++] = c;
			}
		}
	}
	else if (readUntilEOL)
	{
		// absorb all characters until EOL
		while(c != '\n' && c != '\r')
		{
			if (length < MAX_TOKEN_SIZE)
			{
				token[length++] = c;
			}
			pointer++;
			c = *pointer;
		}
		// remove trailing white space
		while(length && token[length-1] < ' ')
		{
			length--;
		}
	}
	else
	{
		while(c > ' ')
		{
			if (length < MAX_TOKEN_SIZE)
			{
				token[length++] = c;
			}
			pointer++;
			c = *pointer;
		}
	}

	if (token[0] == '\"')
	{	// remove start quote
		length--;
		memmove(token, token+1, length);

		if (length && token[length-1] == '\"')
		{	// remove end quote
			length--;
		}
	}

	if (length >= MAX_TOKEN_SIZE)
	{
		length = 0;
	}
	token[length] = 0;
	*text = (char *)pointer;

	return token;
}




CTextPool::CTextPool(int initSize) :
	mNext(0),
	mSize(initSize),
	mUsed(0)
{
#ifdef _EXE
	mPool = (char *)Z_Malloc(mSize, TAG_TEXTPOOL, qtrue);
#else
	mPool = (char *)trap_Z_Malloc(mSize, TAG_GP2);
#endif
}

CTextPool::~CTextPool(void)
{
#ifdef _EXE
	Z_Free(mPool);
#else
	trap_Z_Free(mPool);
#endif
}

char *CTextPool::AllocText(char *text, bool addNULL, CTextPool **poolPtr)
{
	int	length = strlen(text) + (addNULL ? 1 : 0);

	if (mUsed + length + 1> mSize)
	{	// extra 1 to put a null on the end
		if (poolPtr)
		{
			(*poolPtr)->SetNext(new CTextPool(mSize));
			*poolPtr = (*poolPtr)->GetNext();

			return (*poolPtr)->AllocText(text, addNULL);
		}

		return 0;
	}

	strcpy(mPool + mUsed, text);
	mUsed += length;
	mPool[mUsed] = 0;

	return mPool + mUsed - length;
}







CGPObject::CGPObject(const char *initName) :
	mName(initName),
	mNext(0),
	mInOrderNext(0),
	mInOrderPrevious(0)
{
}

bool CGPObject::WriteText(CTextPool **textPool, const char *text)
{
   if (strchr(text, ' '))
   {
	   (*textPool)->AllocText("\"", false, textPool);
	   (*textPool)->AllocText((char *)text, false, textPool);
	   (*textPool)->AllocText("\"", false, textPool);
   }
   else
   {
	   (*textPool)->AllocText((char *)text, false, textPool);
   }

   return true;
}


	
	
	






	
	

CGPValue::CGPValue(const char *initName, const char *initValue) :
	CGPObject(initName),
	mList(0)
{
	if (initValue)
	{
		AddValue(initValue);
	}
}

CGPValue::~CGPValue(void)
{
	CGPObject	*next;

	while(mList)
	{
		next = mList->GetNext();
		delete mList;
		mList = next;
	}
}

bool CGPValue::IsList(void)
{
	if (!mList || !mList->GetNext())
	{
		return false;
	}

	return true;
}

const char *CGPValue::GetTopValue(void) 
{ 
	if (mList)
	{
		return mList->GetName();
	}

	return 0;
}

void CGPValue::AddValue(const char *newValue)
{
	if (mList == 0)
	{
		mList = new CGPObject(newValue);
		mList->SetInOrderNext(mList);
	}
	else
	{
		mList->GetInOrderNext()->SetNext(new CGPObject(newValue));
		mList->SetInOrderNext(mList->GetInOrderNext()->GetNext());
	}
}

void CGPValue::Parse(char **dataPtr, CTextPool **textPool)
{
	char		*token;
	char		*value;

	while(1)
	{
		token = GetToken(dataPtr, true, true);

		if (!token[0])
		{	// end of data - error!
			break;
		}
		else if (strcmpi(token, "]") == 0)
		{	// ending brace for this list
			break;
		}

		value = (*textPool)->AllocText(token, true, textPool);
		AddValue(value);
	}
}

bool CGPValue::Write(CTextPool **textPool, int depth)
{
	int				i;
	CGPObject	*next;

	if (!mList)
	{
		return true;
	}

	for(i=0;i<depth;i++)
	{
		(*textPool)->AllocText("\t", false, textPool);
	}

	WriteText(textPool, mName);

	if (!mList->GetNext())
	{
		(*textPool)->AllocText("\t\t", false, textPool);
		(*textPool)->AllocText((char *)mList->GetName(), false, textPool);
		(*textPool)->AllocText("\r\n", false, textPool);
	}
	else
	{
		(*textPool)->AllocText("\r\n", false, textPool);

		for(i=0;i<depth;i++)
		{
			(*textPool)->AllocText("\t", false, textPool);
		}
		(*textPool)->AllocText("[\r\n", false, textPool);

		next = mList;
		while(next)
		{
			for(i=0;i<depth+1;i++)
			{
				(*textPool)->AllocText("\t", false, textPool);
			}
			(*textPool)->AllocText((char *)next->GetName(), false, textPool);
			(*textPool)->AllocText("\r\n", false, textPool);

			next = next->GetNext();
		}

		for(i=0;i<depth;i++)
		{
			(*textPool)->AllocText("\t", false, textPool);
		}
		(*textPool)->AllocText("]\r\n", false, textPool);
	}

	return true;
}
















CGPGroup::CGPGroup(const char *initName, CGPGroup *initParent) :
	CGPObject(initName),
	mPairs(0),
	mInOrderPairs(0),
	mCurrentPair(0),
	mSubGroups(0),
	mInOrderSubGroups(0),
	mCurrentSubGroup(0),
	mParent(initParent),
	mWriteable(false)
{
}

CGPGroup::~CGPGroup(void)
{
	Clean();
}

void CGPGroup::Clean(void)
{
	while(mPairs)
	{
		mCurrentPair = (CGPValue *)mPairs->GetNext();
		delete mPairs;
		mPairs = mCurrentPair;
	}

	while(mSubGroups)
	{
		mCurrentSubGroup = (CGPGroup *)mSubGroups->GetNext();
		delete mSubGroups;
		mSubGroups = mCurrentSubGroup;
	}

	mPairs = mInOrderPairs = mCurrentPair = 0;
	mSubGroups = mInOrderSubGroups = mCurrentSubGroup = 0;
	mParent = 0;
	mWriteable = false;
}

void CGPGroup::SortObject(CGPObject *object, CGPObject **unsortedList, CGPObject **sortedList,
							 CGPObject **lastObject)
{
	CGPObject	*test, *last;

	if (!*unsortedList)
	{
		*unsortedList = *sortedList = object;
	}
	else
	{
		(*lastObject)->SetNext(object);

		test = *sortedList;
		last = 0;
		while(test)
		{
			if (strcmpi(object->GetName(), test->GetName()) < 0)
			{
				break;
			}

			last = test;
			test = test->GetInOrderNext();
		}

		if (test)
		{
			test->SetInOrderPrevious(object);
			object->SetInOrderNext(test);
		}
		if (last)
		{
			last->SetInOrderNext(object);
			object->SetInOrderPrevious(last);
		}
		else
		{
			*sortedList = object;
		}
	}

	*lastObject = object;
}

CGPValue *CGPGroup::AddPair(const char *name, const char *value)
{
	CGPValue	*newPair;

	newPair = new CGPValue(name, value);

	SortObject(newPair, (CGPObject **)&mPairs, (CGPObject **)&mInOrderPairs, 
		(CGPObject **)&mCurrentPair);

	return newPair;
}

CGPGroup *CGPGroup::AddGroup(const char *name)
{
	CGPGroup	*newGroup;

	newGroup = new CGPGroup(name);

	SortObject(newGroup, (CGPObject **)&mSubGroups, (CGPObject **)&mInOrderSubGroups, 
		(CGPObject **)&mCurrentSubGroup);

	return newGroup;
}
	
CGPGroup *CGPGroup::FindSubGroup(const char *name)
{
	CGPGroup	*group;

	group = mSubGroups;
	while(group)
	{
		if(!stricmp(name, group->GetName()))
		{
			return(group);
		}
		group = (CGPGroup *)group->GetNext();
	}
	return(NULL);
}

void CGPGroup::Parse(char **dataPtr, CTextPool **textPool)
{
	char		*token;
	char		lastToken[MAX_TOKEN_SIZE];
	CGPGroup	*newSubGroup;
	CGPValue	*newPair;
	char		*name, *value;

	while(1)
	{
		token = GetToken(dataPtr, true);

		if (!token[0])
		{	// end of data - error!
			break;
		}
		else if (strcmpi(token, "}") == 0)
		{	// ending brace for this group
			break;
		}

		strcpy(lastToken, token);

		// read ahead to see what we are doing
		token = GetToken(dataPtr, true, true);
		if (strcmpi(token, "{") == 0)
		{	// new sub group
			name = (*textPool)->AllocText(lastToken, true, textPool);
			newSubGroup = AddGroup(name);
			newSubGroup->SetWriteable(mWriteable);
			newSubGroup->Parse(dataPtr, textPool);
		}
		else if (strcmpi(token, "[") == 0)
		{	// new pair list
			name = (*textPool)->AllocText(lastToken, true, textPool);
			newPair = AddPair(name, 0);
			newPair->Parse(dataPtr, textPool);
		}
		else
		{	// new pair
			name = (*textPool)->AllocText(lastToken, true, textPool);
			value = (*textPool)->AllocText(token, true, textPool);
			AddPair(name, value);
		}
	}
}

bool CGPGroup::Write(CTextPool **textPool, int depth)
{
	int				i;
	CGPValue		*mPair = mPairs;
	CGPGroup		*mSubGroup = mSubGroups;

	if (depth >= 0)
	{
		for(i=0;i<depth;i++)
		{
			(*textPool)->AllocText("\t", false, textPool);
		}
		WriteText(textPool, mName);
		(*textPool)->AllocText("\r\n", false, textPool);
		
		for(i=0;i<depth;i++)
		{
			(*textPool)->AllocText("\t", false, textPool);
		}
		(*textPool)->AllocText("{\r\n", false, textPool);
	}

	while(mPair)
	{
		mPair->Write(textPool, depth+1);
		mPair = (CGPValue *)mPair->GetNext();
	}

	while(mSubGroup)
	{
		mSubGroup->Write(textPool, depth+1);
		mSubGroup = (CGPGroup *)mSubGroup->GetNext();
	}

	if (depth >= 0)
	{
		for(i=0;i<depth;i++)
		{
			(*textPool)->AllocText("\t", false, textPool);
		}
		(*textPool)->AllocText("}\r\n", false, textPool);
	}

	return true;
}

const char *CGPGroup::FindPairValue(const char *key, const char *defaultVal)
{
	CGPValue		*mPair = mPairs;

	while(mPair)
	{
		if (strcmpi(mPair->GetName(), key) == 0)
		{
			return mPair->GetTopValue();
		}

		mPair = (CGPValue *)mPair->GetNext();
	}

	return defaultVal;
}















CGenericParser2::CGenericParser2(void) :
	mTextPool(0),
	mWriteable(false)
{
}

CGenericParser2::~CGenericParser2(void)
{
	Clean();
}

void CGenericParser2::Parse(char **dataPtr, bool cleanFirst, bool writeable)
{
	CTextPool	*topPool;

	if (cleanFirst)
	{
		Clean();

		mTextPool = new CTextPool;
	}

	SetWriteable(writeable);
	mTopLevel.SetWriteable(writeable);
	topPool = mTextPool;
	mTopLevel.Parse(dataPtr, &topPool);
}

void CGenericParser2::Clean(void)
{
	CTextPool	*nextPool;

	mTopLevel.Clean();

	while(mTextPool)
	{
		nextPool = mTextPool->GetNext();
		delete mTextPool;
		mTextPool = nextPool;
	}
}

bool CGenericParser2::Write(CTextPool *textPool)
{
	return mTopLevel.Write(&textPool, -1);
}


