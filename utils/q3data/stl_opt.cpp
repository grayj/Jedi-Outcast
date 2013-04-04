// Filename:-	stl_opt.cpp
//
#pragma warning( disable : 4786 )  // identifier was truncated 

#include <windows.h>	// all this shit just to get OutputdebugString() proto!!!!!
#include <assert.h>


//#include "smodel.h"
#include "stl_opt.h"

#include <vector>
#include <map>
#include <string>
#include <set>
using namespace std;



typedef set 	<int> VertUsage_t;									// stores user-tri numbers...
typedef vector		 <VertUsage_t>	VertUsageList_t;
									VertUsageList_t VertUsageList;	// ... for all verts

typedef vector <int> AbuttingTris_t;
typedef vector		<AbuttingTris_t>	AbuttingTrisList_t;
										AbuttingTrisList_t AbuttingTrisList;

void TriangleAbutList_Gen(int mesh[][3], int iNumTris, int iMaxVerts)
{
	// not 100% sure how efficient this [re/]initialising is, but it works...
	//
	VertUsageList.clear();
	VertUsageList.resize(iMaxVerts);
	//
	AbuttingTrisList.clear();
	AbuttingTrisList.resize(iNumTris);

	// generate tris-using list per vert...
	//
	for (int iTriangle = 0; iTriangle < iNumTris; iTriangle++)
	{
		VertUsageList[ mesh[iTriangle][0] ].insert( VertUsageList[ mesh[iTriangle][0] ].begin(),iTriangle );
		VertUsageList[ mesh[iTriangle][1] ].insert( VertUsageList[ mesh[iTriangle][1] ].begin(),iTriangle );
		VertUsageList[ mesh[iTriangle][2] ].insert( VertUsageList[ mesh[iTriangle][2] ].begin(),iTriangle );
	}

/*
	for (int iVert=0; iVert<iMaxVerts; iVert++)
	{
		char sTemp[1000];

		sprintf(sTemp,"Vert %d/%d is used by tris: ",iVert,iMaxVerts);
		for (VertUsage_t::iterator it = VertUsageList[iVert].begin(); it != VertUsageList[iVert].end(); ++it)
		{
			strcat(sTemp,va("%d,",(int)*it));
		}
		strcat(sTemp,"\n");

		OutputDebugString(sTemp);
	}
*/
	// generate abuttment list per tri...
	//
	for (iTriangle = 0; iTriangle < iNumTris; iTriangle++)
	{
		typedef map <int,int>	NodesSharedPerTri_t;	// key=tri, value = # nodes shared
								NodesSharedPerTri_t NodesSharedPerTri;

		for (int iVert=0; iVert<3; iVert++)
		{
			int iThisVert = mesh[iTriangle][iVert];

			for (VertUsage_t::iterator it = VertUsageList[iThisVert].begin(); it != VertUsageList[iThisVert].end(); ++it)
			{
//				if (NodesSharedPerTri.find(*it) != NodesSharedPerTri.end())
//				{
//					OutputDebugString(va("Tri %d shares %d verts\n",*it,NodesSharedPerTri[*it]));
//				}

				NodesSharedPerTri[*it]++;	// amazingly enough this actually works, STL rules...

//				if (NodesSharedPerTri.find(*it) != NodesSharedPerTri.end())
//				{
//					OutputDebugString(va("Tri %d shares %d verts\n",*it,NodesSharedPerTri[*it]));
//				}
//				else
//				{
//					assert(0);
//				}
			}
		}

		for (NodesSharedPerTri_t::iterator it = NodesSharedPerTri.begin(); it != NodesSharedPerTri.end(); ++it)
		{
			int iOtherTriangle	= (*it).first;
			int iSharedVertTot	= (*it).second;

			if ( iOtherTriangle != iTriangle && iSharedVertTot >= 2 )	// this tri shares 2 or more verts with us?
			{
				AbuttingTrisList[ iTriangle ].push_back( iOtherTriangle );
			}
		}
	}

/*
	int iLastTri = 0;
	int iHighestAbutCount=0;	
	for (iTriangle = 0; iTriangle< iNumTris; iTriangle++)
	{
		if (AbuttingTrisList[ iTriangle ].size())
		{
			if (AbuttingTrisList[ iTriangle ].size() > iHighestAbutCount)
				iHighestAbutCount = AbuttingTrisList[ iTriangle ].size();

			iLastTri = iTriangle;
			char sTemp[1000];
			
			sprintf(sTemp,"Tri %d/%d is abutted by triangles: ",iTriangle,iNumTris);

			for (int i=0; i<AbuttingTrisList[ iTriangle ].size(); i++)
			{
				strcat( sTemp, va("%d, ",AbuttingTrisList[ iTriangle ][i]));
			}

			strcat(sTemp,"\n");
			OutputDebugString(sTemp);
		}
	}
*/
}


const int* TriangleAbutList_Query(int iTriangle, int *piCount)
{
	*piCount = AbuttingTrisList[iTriangle].size();

	return &AbuttingTrisList[iTriangle][0];
}


///////////// eof //////////////

