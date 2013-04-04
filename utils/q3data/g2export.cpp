// Filename:-	g2export.cpp
//

#include "q3data.h"
#include <io.h>
/*
#include "system.h"		// stuff needed for caller program, standar headers etc
#include "oddbits.h"
//
*/
#include "matrix4.h"
#include "mdx_format.h"
#include "matcomp.h"
//
#include "g2export_interface.h"
#include "g2export.h"

#define BASEDIRNAME "base" 


static LPCSTR Filename_StripQuakeBase(LPCSTR psFullPathedName)
{
	static char sTemp[1024];		
	
	int iQuakeLen = strlen(gamedir);
	strcpy(sTemp,&psFullPathedName[iQuakeLen]);

	return &sTemp[0];
}

static void ForwardSlash(char *psLocalName)
{
	while (*psLocalName)
	{
		if (*psLocalName == '\\')
			*psLocalName =  '/';

		psLocalName++;
	}
}

// saves clogging the param stack...
//
static char *ExportGhoul2FromMD3_Main(FILE *fhGLM, /*FILE *fhGLA,*/ LPCSTR psFullPathedNameGLM)
{
	char *psErrMess = NULL;

	const int iMallocSize = 1024*1024*20;	// 20MB should be enough for one model... :-)
	byte *pbBuffer = (byte *) malloc(iMallocSize);
	if (pbBuffer)
	{
		// filename used in both files...
		//
//		char sLocalNameGLA[MAX_QPATH];
//		strcpy(	sLocalNameGLA,Filename_WithoutExt(Filename_StripQuakeBase(psFullPathedNameGLM)));		
//		ForwardSlash(sLocalNameGLA);		

		// start writing...
		//
		byte *at = pbBuffer;

		// write GLM Header...
		//
		mdxmHeader_t *pMDXMHeader = (mdxmHeader_t *) at;
		at += sizeof(mdxmHeader_t);

		{// GLM brace-match for skipping...

					pMDXMHeader->ident			=	MDXM_IDENT;
					pMDXMHeader->version			=	MDXM_VERSION;					
			strcpy(	pMDXMHeader->name,Filename_StripQuakeBase(psFullPathedNameGLM));
			strcpy(	pMDXMHeader->animName,sDEFAULT_GLA_NAME);//sLocalNameGLA);
					pMDXMHeader->animIndex		=	0;	// ingame use only
					pMDXMHeader->numBones		=	1;	// ... and a fake one at that.
					pMDXMHeader->numLODs			=	giNumLODs;
	//				pMDXMHeader->ofsLODs			=	?????????????????????????????????????????????
					pMDXMHeader->numSurfaces		=	giNumSurfaces + giNumTags;
	//				pMDXMHeader->ofsSurfHierarchy= 	?????????????????????????????????????????????
	//				pMDXMHeader->ofsEnd			=	?????????????????????????????????????????????



			// for hierarchiy purposes, I'm going to just write out the first surface as the parent, 
			//	then make every other surface a child of that one...
			//
			// G2 surfaces come from MD3 meshes first, then the MD3 tags (since G2 uses tag surfaces)
			//
			mdxmHierarchyOffsets_t *pHierarchyOffsets = (mdxmHierarchyOffsets_t *) at;
			at += sizeof(mdxmHierarchyOffsets_t) * pMDXMHeader->numSurfaces;

			pMDXMHeader->ofsSurfHierarchy = at - (byte *) pMDXMHeader;
			
			for (int iSurfaceIndex = 0; iSurfaceIndex < pMDXMHeader->numSurfaces; iSurfaceIndex++)
			{
				// Note:  bool bSurfaceIsTag == (iSurfaceIndex < giNumSurfaces)
				//
				mdxmSurfHierarchy_t *pSurfHierarchy = (mdxmSurfHierarchy_t *) at;
				//
				// store this offset...
				//
				pHierarchyOffsets->offsets[iSurfaceIndex] = (byte *)pSurfHierarchy - (byte *)pHierarchyOffsets;

				// fill in surf hierarchy struct...
				//
				strcpy(	pSurfHierarchy->name,	G2Exporter_Surface_GetName(iSurfaceIndex));

						pSurfHierarchy->flags		= 0;

				if ( iSurfaceIndex >= giNumSurfaces)
				{
						pSurfHierarchy->flags		|= G2SURFACEFLAG_ISBOLT;
				}
				if (!strnicmp(&pSurfHierarchy->name[strlen(pSurfHierarchy->name)-4],"_off",4))
				{
						pSurfHierarchy->flags		|= G2SURFACEFLAG_OFF;
				}


				strcpy(	pSurfHierarchy->shader,	G2Exporter_Surface_GetShaderName(iSurfaceIndex));
						pSurfHierarchy->shaderIndex	= 0;	// ingame use only
						pSurfHierarchy->parentIndex = iSurfaceIndex?0:-1;
						pSurfHierarchy->numChildren = iSurfaceIndex?0:pMDXMHeader->numSurfaces-1;
				if (!iSurfaceIndex)
				{
					for (int i=0; i<pSurfHierarchy->numChildren; i++)
					{
						pSurfHierarchy->childIndexes[i] = i+1;
					}
				}

				int iThisHierarchySize = (int)( &((mdxmSurfHierarchy_t *)0)->childIndexes[ pSurfHierarchy->numChildren ] );

				at += iThisHierarchySize;
			}



			// write out LODs...
			//
			pMDXMHeader->ofsLODs = at - (byte *) pMDXMHeader;
			for (int iLODIndex = 0; iLODIndex < pMDXMHeader->numLODs; iLODIndex++)
			{
				mdxmLOD_t *pLOD = (mdxmLOD_t *) at;
				at += sizeof(mdxmLOD_t);

				mdxmLODSurfOffset_t *pLODSurfOffsets = (mdxmLODSurfOffset_t *) at;
				at += sizeof(mdxmLODSurfOffset_t) * pMDXMHeader->numSurfaces;

				for (int iSurfaceIndex = 0; iSurfaceIndex < pMDXMHeader->numSurfaces; iSurfaceIndex++)
				{
					mdxmSurface_t *pSurface = (mdxmSurface_t *) at;
					at += sizeof(mdxmSurface_t);
					//
					// store this offset...
					//
					pLODSurfOffsets->offsets[iSurfaceIndex] = (byte *)pSurface - (byte *) pLODSurfOffsets;
					//
					// fill in this surface struct...
					//
					pSurface->ident				=	0;	// ingame-use only, defaulted to 0 here
					pSurface->thisSurfaceIndex	=	iSurfaceIndex;
					pSurface->ofsHeader			= 	(byte *)pMDXMHeader - (byte *)pSurface;	// offset back to main header
					pSurface->numVerts			=	G2Exporter_Surface_GetNumVerts(iSurfaceIndex, iLODIndex);
					pSurface->numTriangles		=	G2Exporter_Surface_GetNumTris (iSurfaceIndex, iLODIndex);
					pSurface->maxVertBoneWeights=	1;	// :-)
					
					//
					// write out triangles...
					//
					pSurface->ofsTriangles		=	at - (byte *)pSurface;
					for (int iTriangleIndex = 0; iTriangleIndex < pSurface->numTriangles; iTriangleIndex++)
					{
						mdxmTriangle_t *pTriangle = (mdxmTriangle_t *) at;
						at += sizeof(mdxmTriangle_t);

						for (int i=0; i<3; i++)
						{
							pTriangle->indexes[i] = G2Exporter_Surface_GetTriIndex(iSurfaceIndex,iTriangleIndex,i, iLODIndex);
						}
					}

					//
					// write out verts...(when exporting from MD3 these are all weighted to only 1 bone)
					//
					pSurface->ofsVerts			=	at - (byte *)pSurface;
					for (int iVertIndex = 0; iVertIndex < pSurface->numVerts; iVertIndex++)
					{
						mdxmVertex_t *pVert = (mdxmVertex_t *) at;

						memcpy(pVert->normal,	 G2Exporter_Surface_GetVertNormal(iSurfaceIndex,iVertIndex,iLODIndex),sizeof(vec3_t));
						memcpy(pVert->vertCoords,G2Exporter_Surface_GetVertCoords(iSurfaceIndex,iVertIndex,iLODIndex),sizeof(vec3_t));
						memcpy(pVert->texCoords, G2Exporter_Surface_GetTexCoords (iSurfaceIndex,iVertIndex,iLODIndex),sizeof(vec2_t));
						pVert->numWeights	=	1;	// force-weight every vert...

						for (int iWeight=0; iWeight<pVert->numWeights; iWeight++)
						{
							pVert->weights[iWeight].boneIndex	= 0;
							pVert->weights[iWeight].boneWeight	= 1.0f;
						}

						at = (byte *) &pVert->weights[pVert->numWeights];
					}

					// remaining surface struct fields...
					//
					pSurface->numBoneReferences =	1;				
					pSurface->ofsBoneReferences =	at - (byte *) pSurface;
					int *piBonesUsed = (int *) at;				
					at += pSurface->numBoneReferences * sizeof(int);
					piBonesUsed[0] = 0;		// the one and only bone ref

					pSurface->ofsEnd			= at - (byte *) pSurface;
				}
			
				pLOD->ofsEnd = at - (byte *) pLOD;
			}

			pMDXMHeader->ofsEnd = at - (byte *) pMDXMHeader;
		}


/*
		// now create GLA file...
		//
		mdxaHeader_t *pMDXAHeader = (mdxaHeader_t *) at;
		at += sizeof(mdxaHeader_t);
		{// for brace-skipping...			

					pMDXAHeader->ident			=	MDXA_IDENT;
					pMDXAHeader->version		=	MDXA_VERSION;
			strncpy(pMDXAHeader->name,sLocalNameGLA,sizeof(pMDXAHeader->name));
					pMDXAHeader->name[sizeof(pMDXAHeader->name)-1]='\0';
					pMDXAHeader->fScale = 1.0f;
					pMDXAHeader->numFrames		=	1;	// inherently, when doing MD3 to G2 files
//					pMDXAHeader->ofsFrames		=	??????????????????
					pMDXAHeader->numBones		=	1;	// inherently, when doing MD3 to G2 files
//					pMDXAHeader->ofsCompBonePool=	??????????????????
//					pMDXAHeader->ofsSkel		=	??????????????????
//					pMDXAHeader->ofsEnd			=	??????????????????

			// write out bone hierarchy...
			//
			mdxaSkelOffsets_t * pSkelOffsets = (mdxaSkelOffsets_t *) at;
			at += (int)( &((mdxaSkelOffsets_t *)0)->offsets[ pMDXAHeader->numBones ] );
			
			pMDXAHeader->ofsSkel	= at - (byte *) pMDXAHeader; 
			for (int iSkelIndex = 0; iSkelIndex < pMDXAHeader->numBones; iSkelIndex++)
			{
				mdxaSkel_t *pSkel = (mdxaSkel_t *) at;

				pSkelOffsets->offsets[iSkelIndex] = (byte *) pSkel - (byte *) pSkelOffsets;

				// setup flags...
				//
						pSkel->flags = 0;	
				strcpy(	pSkel->name, "Generated by Q3Data");	// doesn't matter what this is called I believe.
						pSkel->parent= -1;	// index of bone that is parent to this one, -1 = NULL/root

				Matrix4 BasePose;
						BasePose.Identity();
						BasePose.To3x4(pSkel->BasePoseMat.matrix);				

				Matrix4 BasePoseInverse;
						BasePoseInverse.Inverse(BasePose);
						BasePoseInverse.To3x4(pSkel->BasePoseMatInv.matrix);

						pSkel->numChildren	=	0;	// inherently, when doing MD3 to G2 files

				int iThisSkelSize = (int)( &((mdxaSkel_t *)0)->children[ pSkel->numChildren ] );

				at += iThisSkelSize;
			}


			// write out frames...
			//			
			pMDXAHeader->ofsFrames = at - (byte *) pMDXAHeader;			
			int iFrameSize = (int)( &((mdxaFrame_t *)0)->boneIndexes[ pMDXAHeader->numBones ] );
			for (int i=0; i<pMDXAHeader->numFrames; i++)
			{
				mdxaFrame_t *pFrame = (mdxaFrame_t *) at;
				at += iFrameSize;	// variable sized struct

				pFrame->boneIndexes[0] = 0;	// inherently, when doing MD3 to G2 files
			}

			// now write out compressed bone pool...
			//
			pMDXAHeader->ofsCompBonePool = at - (byte *) pMDXAHeader;
			for (int iCompBoneIndex = 0; iCompBoneIndex < 1 //CompressedBones.size()
																						; iCompBoneIndex++)
			{
				mdxaCompBone_t *pCompBone = (mdxaCompBone_t *) at;
				at += sizeof(mdxaCompBone_t);

				float matThis[3][4];

				Matrix4 Bone;
						Bone.Identity();
						Bone.To3x4(matThis);
				
				byte Comp[sizeof(mdxaCompBone_t)];
				MC_Compress(matThis,Comp);

				memcpy(pCompBone->Comp,Comp,sizeof(Comp));
			}
			
//			int iPoolBytesUsed = (CompressedBones.size()*MC_COMP_BYTES) + (iStats_NumBoneRefs*sizeof(int));
//			printf("( Compressed bone bytes: %d, using pool = %d. Saving = %d bytes )\n",iOldBoneSize,iPoolBytesUsed, iOldBoneSize - iPoolBytesUsed);

			// done...
			//
			pMDXAHeader->ofsEnd = at - (byte *) pMDXAHeader;		
		}
*/

		// write 'em out to disk...
		//
		int iGLMSize = fwrite(pMDXMHeader,1,/*(byte *)pMDXAHeader - (byte *)pMDXMHeader*/ pMDXMHeader->ofsEnd, fhGLM);
		assert(iGLMSize == pMDXMHeader->ofsEnd);

//		int iGLASize = fwrite(pMDXAHeader,1,at - (byte *)pMDXAHeader, fhGLA);
//		assert(iGLASize == pMDXAHeader->ofsEnd);

		// and finally...
		//
		free(pbBuffer);
	}
	else
	{
		psErrMess = va("Error! Unable to allocate %d bytes for workspace!\n",iMallocSize);
	}

	return psErrMess;
}


GLM_ImportModel_t ImportedModel;


// return = NULL for no error, else string of error for caller to display...
//
char *ExportGhoul2FromMD3_ImportExisting(LPCSTR psFullPathedFilename)
{		
	char *psErrorMess = NULL;
	void *pvData = NULL;

	ImportedModel.ImportedLODs.clear();
	ImportedModel.SurfaceIndexRemaps.clear();

	FILE *fhHandle = fopen( psFullPathedFilename, "rb" );
	if   (fhHandle)
	{
		long lLen = filelength( fileno( fhHandle ) );

		pvData = malloc( lLen );
		if (pvData)
		{
			long lBytesRead = fread( pvData, 1, lLen, fhHandle );

			if (lBytesRead == lLen)
			{
				mdxmHeader_t *pMDXMHeader = (mdxmHeader_t *) pvData;

				if (pMDXMHeader->ident == MDXM_IDENT)
				{
					if (pMDXMHeader->version == MDXM_VERSION)
					{
						if (!strcmp(pMDXMHeader->animName, sDEFAULT_GLA_NAME))
						{
							if (pMDXMHeader->numBones == 1)	// actually we can skip this because of the sDEFAULT_GLA_NAME check, but wtf?
							{
								if (pMDXMHeader->numSurfaces == giNumSurfaces + giNumTags)
								{
									// now for the important stuff, let's read some data...
									//
									// setup remap tables so export code can query imported model transparently with same surface indexes as 
									//	q3data version (they should be the same, but it's possible to not be and still be legal)...
									//
									mdxmHierarchyOffsets_t *pHierarchyOffsets = (mdxmHierarchyOffsets_t *) ((byte*)pMDXMHeader + sizeof(*pMDXMHeader));
									//
									// scan imported surface names...
									//
									for (int iSurface = 0; iSurface < pMDXMHeader->numSurfaces; iSurface++)
									{
										mdxmSurfHierarchy_t *pSurfHierarchy = (mdxmSurfHierarchy_t *) ((byte*)pHierarchyOffsets + pHierarchyOffsets->offsets[iSurface]);
										LPCSTR psSurfaceName = pSurfHierarchy->name;
										//
										// ... to match current qdata surface names...
										//
										for (int iQ3Surface = 0; iQ3Surface < pMDXMHeader->numSurfaces/*the same at this point*/; iQ3Surface++)
										{
											LPCSTR psQ3SurfaceName = G2Exporter_Surface_GetName(iQ3Surface);

											if (!stricmp(psSurfaceName,psQ3SurfaceName))
											{
												ImportedModel.SurfaceIndexRemaps[iQ3Surface] = iSurface;	// new remap table entry
												break;
											}
										}
										if (iQ3Surface == pMDXMHeader->numSurfaces)
										{
											psErrorMess = va("Unable to find imported surface \"%s\" in current Q3data model\n",psSurfaceName);
											break;
										}
									}

									if (psErrorMess == NULL)	// worth carrying on?
									{
										// now read the actual imported data...
										//
										ImportedModel.ImportedLODs.resize(pMDXMHeader->numLODs);

										mdxmLOD_t *pLOD = (mdxmLOD_t *) ((byte*) pMDXMHeader + pMDXMHeader->ofsLODs);
										for (int iLOD = 0; iLOD < pMDXMHeader->numLODs; iLOD++)
										{												
											mdxmLODSurfOffset_t *pLODSurfOffset = (mdxmLODSurfOffset_t *) &pLOD[1];

											for (iSurface = 0; iSurface < pMDXMHeader->numSurfaces; iSurface++)
											{
												GLM_ImportSurface_t GLM_ImportSurface;

												mdxmSurface_t *pSurface = (mdxmSurface_t *) ((byte *) pLODSurfOffset +  pLODSurfOffset->offsets[iSurface]);

												// triangles...
												//
												mdxmTriangle_t *pTriangle = (mdxmTriangle_t *) ((byte*) pSurface + pSurface->ofsTriangles);
												for (int iTriangle = 0; iTriangle < pSurface->numTriangles; iTriangle++, pTriangle++)
												{
													GLM_ImportSurface.ImportedTris.push_back(*pTriangle);
												}

												// verts...
												//
												mdxmVertex_t *pVert = (mdxmVertex_t *) ((byte*) pSurface + pSurface->ofsVerts);
												for (int iVert = 0; iVert < pSurface->numVerts; iVert++)
												{
													GLM_ImportVertex_t GLM_ImportVertex;

													memcpy(GLM_ImportVertex.normal,		pVert->normal,		sizeof(GLM_ImportVertex.normal));
													memcpy(GLM_ImportVertex.vertCoords,	pVert->vertCoords,	sizeof(GLM_ImportVertex.vertCoords));
													memcpy(GLM_ImportVertex.texCoords,	pVert->texCoords,	sizeof(GLM_ImportVertex.texCoords));

													GLM_ImportSurface.ImportedVerts.push_back(GLM_ImportVertex);

													pVert = (mdxmVertex_t *)&pVert->weights[/*pVert->numWeights*/pSurface->maxVertBoneWeights];
												}

												// done, so add this imported surface to the imported model...
												//
												ImportedModel.ImportedLODs[iLOD].ImportedSurfaces.push_back(GLM_ImportSurface);
											}

											pLOD = (mdxmLOD_t *)((byte *)pLOD + pLOD->ofsEnd);
										}

										// sanity...
										// 
										assert(ImportedModel.SurfaceIndexRemaps.size() == pMDXMHeader->numSurfaces);
										assert(ImportedModel.ImportedLODs.size() == pMDXMHeader->numLODs);
										assert(ImportedModel.ImportedLODs[0].ImportedSurfaces.size() == pMDXMHeader->numSurfaces);

										// and finally, tell the export q3data-query code how many LODS there now are,
										//	so it knows when to query this data as opposed to the current q3data model...
										//
										giNumLODs += pMDXMHeader->numLODs;
									}
								}
								else
								{
									psErrorMess = va("ExportGhoul2FromMD3_ImportExisting(): found %d surfaces instead of %d\nFile:\n\"%s\"\n",pMDXMHeader->numSurfaces, giNumSurfaces + giNumTags, psFullPathedFilename);
								}
							}
							else
							{
								psErrorMess = va("ExportGhoul2FromMD3_ImportExisting(): found %d bones instead of %d\nFile: \"%s\"\n",pMDXMHeader->numBones,1, psFullPathedFilename);
							}
						}
						else
						{
							psErrorMess = va("ExportGhoul2FromMD3_ImportExisting(): File did not have \"%s\" as animName.\nFile:\"%s\"\n",sDEFAULT_GLA_NAME, psFullPathedFilename);
						}
					}
					else
					{
						psErrorMess = va("ExportGhoul2FromMD3_ImportExisting(): Bad version (%d, expecting %d) on file:\n\"%s\"\n",pMDXMHeader->version, MDXM_VERSION, psFullPathedFilename);
					}
				}
				else
				{
					psErrorMess = va("ExportGhoul2FromMD3_ImportExisting(): Bad ident on file \"%s\"\n",psFullPathedFilename);
				}
			}
			else
			{
				psErrorMess = va("ExportGhoul2FromMD3_ImportExisting(): Short read (%d bytes instead of %d)\nFile = \"%s\"\n",lBytesRead,lLen,psFullPathedFilename);
			}
		}
		else
		{
			psErrorMess = va("ExportGhoul2FromMD3_ImportExisting(): Unable to allocate %d bytes\n",lLen);
		}
	}
	else
	{
		psErrorMess = va("ExportGhoul2FromMD3_ImportExisting():\nUnable to open file \"%s\" for append!\n",psFullPathedFilename);
	}

	if (fhHandle)
	{
		fclose(fhHandle);
//			   fhHandle = NULL;
	}

	if (pvData)
	{
		free(pvData);
			 pvData = NULL;
	}

	return psErrorMess;
}



// assumes 'psFullPathedFilename' is full-pathed filename with ".glm" on the end,
//	ppsFullPathedNameGLA is optional feedback param if you want to know what the GLA name was.
//
// return = NULL for no error, else string of error for caller to display...
//
extern "C" void	CreatePath (const char *path);
char *ExportGhoul2FromMD3(LPCSTR psFullPathedFilename, int iNumLODs, int iNumSurfaces, int iNumTags
//						  ,LPCSTR *ppsFullPathedNameGLA /* = NULL */
						  )
{
	char *psErrorMess = NULL;

	giNumLODs		= iNumLODs;
	giNumSurfaces	= iNumSurfaces;
	giNumTags		= iNumTags;

	CreatePath(psFullPathedFilename);	// this has already been done earlier in q3data, but wtf?

	// new, we need to work out if this file is to be an output, or a LOD append (ie are we "<modelname>_1.glm") ?
	//
	bool bAppending = false;
	char sTemp[2048];
	strcpy(sTemp, Filename_WithoutExt( psFullPathedFilename ));
	char *psSuffixPos = &sTemp[strlen(sTemp)-2];
	if (psSuffixPos[0] == '_' && isdigit(psSuffixPos[1]))
	{
		bAppending = true;		

		// since we're appending, we first of all need to get the data from the file already out there...
		//
		*psSuffixPos='\0';	// chop off trailing "_1" etc
		strcat(sTemp, ".glm");
		psErrorMess = ExportGhoul2FromMD3_ImportExisting( sTemp );
		if (psErrorMess)
			return psErrorMess;	// which will cause an app-exit
	}

	FILE *fhGLM = fopen(bAppending?sTemp:psFullPathedFilename,"wb");
	if (  fhGLM)
	{
		static char sNameGLA[MAX_QPATH];

//		strcpy(sNameGLA, Filename_WithoutExt(psFullPathedFilename) );
//		strcat(sNameGLA, ".gla");
		strcpy(sNameGLA, sDEFAULT_GLA_NAME );

//		CreatePath(sNameGLA);
//		FILE *fhGLA = fopen(sNameGLA,"wb");
//		if (  fhGLA)
		{
//			if ( ppsFullPathedNameGLA)
//			{
//				*ppsFullPathedNameGLA = &sNameGLA[0];
//			}

			psErrorMess = ExportGhoul2FromMD3_Main(fhGLM, /*fhGLA,*/ psFullPathedFilename);

//			fclose(fhGLA);
		}
//		else
//		{
//			psErrorMess = va("Error: Unable to open file '%s'!\n");
//		}

		fclose(fhGLM);
	}
	else
	{
		psErrorMess = va("Error: Unable to open file '%s'!\n");
	}

	if (bAppending && !psErrorMess)
	{
		printf("\n( Appended LOD to \"%s\" )\n\n",sTemp);
	}
	return psErrorMess;
}



/////////////////// eof ////////////////////

