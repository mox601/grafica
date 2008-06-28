/********************************************************************************************
** FindNodes.cpp - Source Code Module for Vrml2OGL.exe V1.0 -> V1.01
**
** Program Name:	Vrml2OGL.exe V1.0 -> V1.01
** Author:			Alexander Rohra
** Date:			May 1999
** Compiler:		MSVC++ V5.0	& others
** App Type:		Command Line Utility (Win32 Console Application)
** Revisions:		August 1999 - Alexander Rohra -> V1.01:
**					1)	Allowed recognition of node types even if the node name is
**						directly (without a blank in between, eg. "Coordinate3{")
**						preceded by a '}' or followed by a '{'.
** Code Desc.:		The code in this module contains the program's core function
**					(findNodes()) and all node handling functions.
**					The core function is responsible for reading through the VRML
**					file, recognizing the different node types and calling the
**					appropriate handling functions. Each handling function then
**					retrieves the necessary information from each node and
**					contributes to generating the desired output files containing
**					OpenGL C code.
**					Each function that returns a value (other than void)
**					returns either NOERR or one of the error stati enumerated
**					in sharedDefs.h unless where denoted otherwise in
**					function descriptions.
**
** General Info:	Please see Vrml2OGL.cpp for general information about the
**					program.
********************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "FindNodes.h"				// needed for shared define statements (inc. the ones from
									// "SharedDefs.h")
#include "Vrml2OGLUtils.h"			// needed for utility function prototypes
#include "Vrml2OGL.h"				// needed for debugMode extern var definition
#include "GeometryUtils.h"			// needed for radianToDegree macro

COLOR defaultAmbMat=DEFAULTAMBMAT;
COLOR defaultDiffMat=DEFAULTDIFFMAT;
COLOR defaultSpecMat=DEFAULTSPECMAT;
COLOR defaultEmissMat=DEFAULTEMISSMAT;
char *vertexOrderType[]={"GL_CW", "GL_CCW", "GL_CW"};  // order has to correspond to CWORDER
													   // and CCWORDER defines in FindNodes.cpp
double workArray[MAXWORKARRAY];
int matArrayNum, matrixNum;
int currLOD;

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: findNodes()
// 
// Description:
//	This function finds all nodes contained in the WRL file and invokes the appropriate
//	handling functions as defined in nodeTypes[]. Upon return of the handling function the
//	read file pointer will have been advanced to the next line after the end of the node
//	from where this function continues scanning for the next node.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE findNodes(FILE *vrmlFile[], FILE *outFiles[], NODE *nodeTypes)
{
	int i, lastNodeType;
	int lineLen, nodeLen;	
	BYTE ret=NOERR;
	char line[MAXLINE], chr;
	char *foundAddr, *endAddr, *endOfLine, *curLineAddr, *DEF;
	INDENTATION indentArray[MAXINDENTS];
	int indentIdx=0;
	
	// find the index of the last element in nodeTypes[]
	for(lastNodeType=0; nodeTypes[lastNodeType].name!=NULL; lastNodeType++);

	// setup inital defaults (!)
	// this 1st array element is reserved and should not be modified
	indentArray[0].coordArray=NULL;		
	indentArray[0].coordCount=0;
	indentArray[0].normArray=NULL;
	indentArray[0].normCount=0;	
	indentArray[0].matCount=0;
	indentArray[0].vertexOrder=DEFAULTORDER;
	indentArray[0].openedSubIndents=0;
	indentArray[0].shininess=DEFAULTSHININESS;
	indentArray[0].LODHead=NOTHEAD;

	matrixNum=matArrayNum=indentArray[0].matArrayNum=0;

	currLOD=0;

	// read each line in the WRL file in the search for nodes
	while(spFgets(line, MAXLINE, vrmlFile[0])!=NULL && ret==NOERR)
	{
		lineLen=strlen(line);
		endOfLine=line+lineLen;
		// check each node name in nodeTypes[] against the contents of this line
		for(i=0; nodeTypes[i].name!=NULL && ret==NOERR; i++)
		{							
			nodeLen=nodeTypes[i].len;		// no nT.len-1, EOL considered at 0ah
			curLineAddr=line;

			// check the current line for the current node name until the line is
			// depleted (the while loop is used here since there could be something like
			// "DEF NormalArray Normal {" in which case we would find the node name Normal
			// twice just that the 1st one is invalid since it is part of a string and
			// the second one is valid)
			while((foundAddr=strstr(curLineAddr, nodeTypes[i].name))!=NULL)
			{
				endAddr=foundAddr+nodeLen;
				// check that the node name found at foundAddr is not part of a string
				if(nodeTypes[i].len==1 ||
				   ((foundAddr==line &&
				    (endAddr==endOfLine || endAddr[0]<=32 || endAddr[0]=='{')) ||
				   ((foundAddr[-1]<=32 || foundAddr[-1]=='}') &&
					(endAddr==endOfLine || endAddr[0]<=32 || endAddr[0]=='{'))))
				{
					curLineAddr=endAddr;

					// retrieve DEF name if there is any
					chr=*foundAddr;
					*foundAddr='\0';
					DEF=getDEF(line);
					*foundAddr=chr;

					// erase everything in this line until after the node name to prevent
					// it from being interpreted by the handling functions
					memset(line, ' ', endAddr-line);

					// if we are currently in an LOD and we have found one of the lower
					// detail levels (>ACTIVELOD) ignore it by handling all nodes genericly
					// (except for indent type nodes since we still need to keep track of
					//  indentations)
					if(currLOD>ACTIVELOD && indentArray[indentIdx].inLOD &&
						nodeTypes[i].type<INDENTTYPESTART)
					{
						ret=handleGenericNode(indentArray, &indentIdx,
							nodeTypes[i].type, line, DEF, vrmlFile, outFiles);
					}
					else // otherwise handle the current node as normal
					{
						ret=nodeTypes[i].handlingFunc(indentArray, &indentIdx,
							nodeTypes[i].type, line, DEF, vrmlFile, outFiles);
					}
					i=lastNodeType-1;	// invoke scaning of next line
					break;
				}
				else
					// if we didn't find a real node name in this line yet continue scaning it
					curLineAddr=endAddr;
			}
		}
	}

	if(ret==NOERR)
		if(ferror(vrmlFile[0]))
			ret=GENINPREADERR;

	// delete all allocated arrays if there are any (eg. due to an error)
	for(i=1; i<=indentIdx; i++)
		deleteIndentArrays(indentArray, i, "Safety Net: deallocated");

	if(ret==NOERR)
	{
		if(indentIdx==1)
		{
			fprintf(stderr, "%d close brace was not found - corrupt input file?\n",
				indentIdx);
			return(SYNTAXERR);
		}
		else if(indentIdx)
		{
			fprintf(stderr, "%d close braces were not found - corrupt input file?\n",
				indentIdx);
			return(SYNTAXERR);
		}
	}

	return(ret);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: handleSubIndent()
// 
// Description:
//	This function handles sub-indentations such as Group, Switch and LOD nodes since they can
//	serve as parent nodes to regular nodes. These nodes are different from Separator nodes
//	since definitions made in them are still valid even beyond their parents' scope. Eg. even
//	if a Group node is closed a Coordinate3 array that was contained in this Group node is
//	still valid until the closure of its underlying Separator node.
// Note: The level of detail is determined by the count of Separator nodes that are direct
//		 children to an LOD node (see handleIndent()).
//		 See also findBoundingBox().
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE handleSubIndent(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
					 FILE *vFile[], FILE *outFiles[])
{
	static BYTE ret;

	// if this function was called for an LOD node, not only record another opened brace but
	// also the sequence number of the open brace so that we know where the LOD ends since
	// the different detail levels receive different treatment (see findNodes())
	if(iType==LOD)
	{
		if(iArray[*iIdx].inLOD==V_TRUE)
		{
			fprintf(stderr, "Nested LOD nodes are not supported\n");
			return(NESTEDLODERR);
		}
		else
		{
#ifdef _V_DEBUGMODE_
			if(debugMode)
				printf("LOD Sub Indentation Found!\n");
#endif _V_DEBUGMODE_

			// record the sequence number of the open brace for this LOD node
			iArray[*iIdx].LODHead=++iArray[*iIdx].openedSubIndents;
			iArray[*iIdx].inLOD=V_TRUE;
		}
	}
	else
	{
#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("Sub Indentation Found!\n");
#endif _V_DEBUGMODE_

		// for any other sub indentation type just increase the opened sub indents count
		iArray[*iIdx].openedSubIndents++;
	}

	// show DEF as a comment only if we are in an active level of detail or none
	// (none means currLOD==0)
	if(currLOD<=ACTIVELOD && DEF!=NULL)
	{
		if(fprintf(oglFile, "%s/*** Start of DEF: %s  { ***/\n",
			filler[*iIdx+0], DEF)<0)
			return(GENOUTPWRITEERR);

		if((ret=addDefToList(iArray+(*iIdx), *iIdx, DEF))!=NOERR)
			return(ret);
	}

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: handleIndent()
// 
// Description:
//	This function is invoked for Separator nodes and creates a stack entry for each of them
//	Definitions made in parent Separator nodes are valid and available to child Separator
//	nodes. However once a Separator node is closed definitions made in them go out of scope
//	and are invalid for subsequent none child nodes.
// Note: The first Separator after an LOD node is considered the first level of detail.
//	     Once this Separator (and all its possible child Separators) is closed the next
//		 level of detail begins until the LOD node is closed.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE handleIndent(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
				  FILE *vFile[], FILE *outFiles[])
{
	static BYTE ret;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("Indentation Found!\n");
#endif _V_DEBUGMODE_

	// if this Separator is a direct child to an LOD node it marks the beginning to a new
	// level of detail
	if(iArray[*iIdx].LODHead!=NOTHEAD)
	{
		// increase the level of detail counter
		currLOD++;

		if(currLOD==ACTIVELOD)
		{
			if(fprintf(oglFile, "%s/*** Start of ACTIVE Level of Detail (%2.2d) { ***/\n",
				filler[*iIdx+0], currLOD)<0)
				return(GENOUTPWRITEERR);
		}
		else if(currLOD>ACTIVELOD)
		{
			if(fprintf(oglFile, "%s/*** INACTIVE Level of Detail (%2.2d) (IGNORED) ***/\n",
				filler[*iIdx+0], currLOD)<0)
				return(GENOUTPWRITEERR);
		}
	}

	// create another stack entry
	(*iIdx)++;
	iArray[*iIdx]=iArray[(*iIdx)-1];
	iArray[*iIdx].type=iType;

	// setup inital defaults (!) for this indentation
	iArray[*iIdx].openedSubIndents=0;
	iArray[*iIdx].LODHead=NOTHEAD;
	iArray[*iIdx].DEFIdx=0;

	// show DEF as a comment only if we are in an active level of detail or none
	// (none means currLOD==0)
	if(currLOD<=ACTIVELOD)
	{
		if(DEF!=NULL)
		{
			if(fprintf(oglFile, "%s/*** Start of DEF: %s { ***/\n",
				filler[*iIdx+0], DEF)<0)
				return(GENOUTPWRITEERR);

			if((ret=addDefToList(iArray+(*iIdx), *iIdx, DEF))!=NOERR)
				return(ret);
		}

		if(fprintf(oglFile, "%sglPushAttrib(GL_ALL_ATTRIB_BITS);\n%sglPushMatrix();\n",
			filler[*iIdx+0], filler[*iIdx+0])<0)
			return(GENOUTPWRITEERR);
	}

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: handleIndentClose()
// 
// Description:
//	This function is called for close braces ( { ) for Separator nodes as well as for
//	all sub-indent nodes (such as Group, Switch and LOD nodes).
//	If the openedSubIndents count for this stack entry is greater than zero this close
//	brace must refer to a sub-indentation. Once the openedSubIndents count is zero this
//  close brace must refer to a Separator node which causes the current stack entry to be
//	deleted and thereby all its definitions are deallocated and go out of scope.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE handleIndentClose(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
					   FILE *vFile[], FILE *outFiles[])
{
	// check if this close brace refers to a sub-indentation
	if(iArray[*iIdx].openedSubIndents)
	{
		// output the end of a DEF if there is one that refers to this now closed
		// sub-indentation and deallocate it
		if(currLOD<=ACTIVELOD && iArray[*iIdx].DEFIdx &&
			iArray[*iIdx].DEFList[iArray[*iIdx].DEFIdx-1].subIndentNo==
			iArray[*iIdx].openedSubIndents)
		{
			if(fprintf(oglFile, "%s/*** End of DEF: %s } ***/\n",
				filler[*iIdx+0], iArray[*iIdx].DEFList[iArray[*iIdx].DEFIdx-1].DEF)<0)
				return(GENOUTPWRITEERR);

			delete [] iArray[*iIdx].DEFList[iArray[*iIdx].DEFIdx-1].DEF;
#ifdef _V_DEBUGMODE_
			if(debugMode)
				printf("\tDeallocated DEF[%d][%3.3d]!\n", *iIdx, iArray[*iIdx].DEFIdx-1);
#endif _V_DEBUGMODE_
			iArray[*iIdx].DEFIdx--;
		}

		// mark the ending of an LOD if this close brace refers to one
		// (identified by the sequence index of this sub-indentation that was recorded by
		//	handleSubIndent() in LODhead)
		if(iArray[*iIdx].LODHead==iArray[*iIdx].openedSubIndents)
		{
#ifdef _V_DEBUGMODE_
			if(debugMode)
				printf("LOD Sub Indentation Close Found!\n");
#endif _V_DEBUGMODE_

			iArray[*iIdx].inLOD=V_FALSE;
			iArray[*iIdx].LODHead=NOTHEAD;
			currLOD=0;
		}
		else
		{
#ifdef _V_DEBUGMODE_
			if(debugMode)
				printf("Sub Indentation Close Found!\n");
#endif _V_DEBUGMODE_
		}

		// decrease the current openedSubIndents count
		--iArray[*iIdx].openedSubIndents;
	}
	else	// if this close brace does not refer to a sub-indentation it refers to a Separator
	{
#ifdef _V_DEBUGMODE_
		if(debugMode)
			printf("Indentation Close Found!\n");
#endif _V_DEBUGMODE_

		// if we are in an active level of detail or in none (none means currLOD==0) output
		// glPop* statements since all defintions made within this Separator go out of scope
		if(currLOD<=ACTIVELOD)
		{
			if(fprintf(oglFile, "%sglPopMatrix();\n%sglPopAttrib();\n",
				filler[*iIdx+0], filler[*iIdx+0])<0)
				return(GENOUTPWRITEERR);

			// output the end of a DEF if there is one that refers to this now closed
			// Separator  (no need to deallocate it here, it is deallocated automatically
			// in deleteIndentArrays() below)
			if(iArray[*iIdx].DEFIdx && fprintf(oglFile, "%s/*** End of DEF: %s } ***/\n",
				filler[*iIdx+0], iArray[*iIdx].DEFList[iArray[*iIdx].DEFIdx-1].DEF)<0)
				return(GENOUTPWRITEERR);
		}
	
		if(*iIdx==0)
		{
			fprintf(stderr, "Found more close braces than open braces (syntax error?)\n");
			return(SYNTAXERR);
		}
		else
		{
			deleteIndentArrays(iArray, *iIdx, "\tDeallocated");

			(*iIdx)--;
		}

		// if this Separator was a direct child to an LOD node this close brace marks the end
		// of a level of detail; ouput a message if this is the end of the active level of
		// detail
		if(iArray[*iIdx].LODHead!=NOTHEAD && currLOD==ACTIVELOD &&
			fprintf(oglFile, "%s/*** End of ACTIVE Level of Detail (%2.2d) } ***/\n",
			filler[*iIdx+0], currLOD)<0)
			return(GENOUTPWRITEERR);
	}
	
	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: handleNormal()
// 
// Description:
//	This function is called for Normal nodes and reads all following coordinates into
//	an array in memory until the end of each Normal node.
// Note: Since only one array of normals is expected per Separator a previously read in
//		 array would simply be replaced by the new one but the old one would remain in
//		 memory and produce a memory leak.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE handleNormal(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
				  FILE *vFile[], FILE *outFiles[])
{
	static BYTE ret;
	static long entryCount;	
	static VERTEX *arrayPtr;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("Normal Found!\n");
#endif _V_DEBUGMODE_

	if(DEF!=NULL && fprintf(oglFile, "%s/*** DEF: %s (Normal) ***/\n", filler[*iIdx], DEF)<0)
		return(GENOUTPWRITEERR);

	// delete previously allocated Coordinate3 array first (if one was found and if there it
	// was not inherited from a parent SEPARATOR)
	deleteNormArray(iArray, *iIdx, "\tFor Replacement Deallocated");

	// pass V_FALSE as centerMode to prevent retrieveCoordArray() from centering normals
	if((ret=retrieveCoordArray(line, &entryCount, workArray, MAXWORKARRAY, &arrayPtr,
		vFile[0], V_FALSE))==NOERR)
	{
		// attach the array to the current stack entry (indentation/Separator) so that when
		// the current stack entry goes out of scope, so does this array
		iArray[*iIdx].normArray=arrayPtr;
		iArray[*iIdx].normCount=entryCount/3;

#ifdef _V_DEBUGMODE_
		if(debugMode)
			printf("\tFound: %ld numeric entries (= %ld vertices) \n",
				entryCount, entryCount/3);
#endif _V_DEBUGMODE_
				
		ret=skipToCloseBrace(line, vFile[0]);
	}	
		
	return(ret);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: handleCoordinate3()
// 
// Description:
//	This function is called for Coordinate3 nodes and reads all following coordinates into
//	an array in memory until the end of each Coordinate3 node.
// Note: Since only one array of coordinates is expected per Separator a previously read in
//		 array would simply be replaced by the new one but the old one would remain in
//		 memory and produce a memory leak.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE handleCoordinate3(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
					   FILE *vFile[], FILE *outFiles[])
{
	static BYTE ret;
	static long entryCount;	
	static VERTEX *arrayPtr;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("Coordinate3 Found!\n");
#endif _V_DEBUGMODE_

	// delete previously allocated Coordinate3 array first (if one was found and if there it
	// was not inherited from a parent SEPARATOR)
	deleteCoordArray(iArray, *iIdx, "\tFor Replacement Deallocated");

	if(DEF!=NULL && fprintf(oglFile, "%s/*** DEF: %s (Coordinate3) ***/\n",
		filler[*iIdx], DEF)<0)
		return(GENOUTPWRITEERR);

	if((ret=retrieveCoordArray(line, &entryCount, workArray, MAXWORKARRAY, &arrayPtr,
		vFile[0], centerMode))==NOERR)
	{
		iArray[*iIdx].coordArray=arrayPtr;
		iArray[*iIdx].coordCount=entryCount/3;

#ifdef _V_DEBUGMODE_
		if(debugMode)
			printf("\tFound: %ld numeric entries (= %ld vertices) \n",
				entryCount, entryCount/3);
#endif _V_DEBUGMODE_
				
		ret=skipToCloseBrace(line, vFile[0]);
	}	
		
	return(ret);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: handleShapeHints()
// 
// Description:
//	This function only handles vertex order info contained in ShapeHints nodes. All other
//	information contained in these nodes is discarded.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE handleShapeHints(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
					  FILE *vFile[], FILE *outFiles[])
{
	static BYTE newVertexOrder;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("ShapeHints Found!\n");
#endif _V_DEBUGMODE_

	if(DEF!=NULL && fprintf(oglFile, "%s/*** DEF: %s (ShapeHints) ***/\n",
		filler[*iIdx], DEF)<0)
		return(GENOUTPWRITEERR);

	newVertexOrder=iArray[*iIdx].vertexOrder;

	do
	{		
		// check for vertex order info
		if(strstr(line, "COUNTERCLOCKWISE")!=NULL)		
			newVertexOrder=CCWORDER;		
		else if(strstr(line, "CLOCKWISE")!=NULL)		
			newVertexOrder=CWORDER;		
		else if(strstr(line, "UNKNOWN_ORDERING")!=NULL)		
			newVertexOrder=DEFAULTORDER;		
	} while(strchr(line, CLOSEBRACE)==NULL && spFgets(line, MAXLINE, vFile[0])!=NULL);

	if(newVertexOrder!=iArray[*iIdx].vertexOrder)
		iArray[*iIdx].vertexOrder=newVertexOrder;

	if(ferror(vFile[0]))
		return(GENINPREADERR);

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: handleIndexedFLSet()
// 
// Description:
//	This function handles IndexedFaceSet as well as IndexedLineSet nodes. coordIndex,
//	normalIndex and materialIndex arrays will be applied on/reference previously defined
//	Coordinate3, Normal and/or Material arrays respectively. textureCoordIndex arrays are
//	skipped since textures are unsupported by this program.
// Note: Each IndexedFaceSet and IndexedLineSet node is expected to contain a coordIndex array;
//		 if none could be found the program is aborted.
//		 Since these index arrays (if there is more than one) are to be used simultaneously
//		 (eg. if there is a coordIndex, normalIndex and materialIndex array each vertex output
//		  is preceeded by material properties and a normal vector), a separate file pointer
//		 into the input file (contained in vFile[]) is used for each index array type.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE handleIndexedFLSet(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
						  FILE *vFile[], FILE *outFiles[])
{
	static BYTE ret, coordIndexFound, materialIndexFound, normalIndexFound;	
	static char *tmpPtr, line1[MAXLINE], line2[MAXLINE];
	static long filePos;

#ifdef _V_DEBUGMODE_
	if(debugMode)
	{
		if(iType==LINESET)
			printf("IndexedLineSet Found!\n");
		else
			printf("IndexedFaceSet Found!\n");
	}
#endif _V_DEBUGMODE_

	if(DEF!=NULL)
	{
		if(iType==LINESET)
		{
			if(fprintf(oglFile, "%s/*** Start of DEF: %s (IndexedLineSet) { ***/\n",
				filler[*iIdx], DEF)<0)
				return(GENOUTPWRITEERR);
		}
		else if(fprintf(oglFile, "%s/*** Start of DEF: %s (IndexedFaceSet) { ***/\n",
			filler[*iIdx], DEF)<0)
			return(GENOUTPWRITEERR);
	}

	tmpPtr=line;
	coordIndexFound=materialIndexFound=normalIndexFound=V_FALSE;

	// prepare additional file pointers pointing into vrmlFile
	if((filePos=ftell(vFile[0]))==-1L)		
		return(GENFTELLERR);	

	if(fseek(vFile[1], filePos, SEEK_SET) || fseek(vFile[2], filePos, SEEK_SET))			
		return(GENFSEEKERR);

	strcpy(line1, line);
	strcpy(line2, line);

	// position file pointers right at the first line containing indecis for each index array
	// but first try to find each type of array
	// search for coordIndex array
	do
	{	
		if(strstr(line, "coordIndex")!=NULL)
		{
#ifdef _V_DEBUGMODE_
			if(debugMode)
				printf("coordIndex Found!\n");
#endif _V_DEBUGMODE_

			// position coordIndex file pointer to the first line that contains the indecis	
			if((ret=readToBegOfNum(line, vFile[0]))!=SUCCESS)
			{
				if(ret==NOERR)
				{
					fprintf(stderr,
						"Expected array of numbers after coordIndex specifier\n");
					return(SYNTAXERR);
				}
				else
					return(ret);
			}

			coordIndexFound=V_TRUE;
			break;		
		}
	} while(strchr(line, CLOSEBRACE)==NULL && (tmpPtr=spFgets(line, MAXLINE, vFile[0]))!=NULL);
	
	if(tmpPtr==NULL)
	{
		if(feof(vFile[0]))
			return(GENUNEXPEOFERR);
		else		
			return(GENINPREADERR);
	}	

	// abort if there is no Coordinate3 array
	if(coordIndexFound==V_FALSE)
	{
		if(iType==LINESET)		
			fprintf(stderr, "Could not find coordIndex label within IndexedLineSet Node\n");
		else
			fprintf(stderr, "Could not find coordIndex label within IndexedFaceSet Node\n");
		return(SYNTAXERR);
	}
	
	// search for materialIndex array
	do
	{	
		if(strstr(line1, "materialIndex")!=NULL)
		{
#ifdef _V_DEBUGMODE_
			if(debugMode)
				printf("materialIndex Found!\n");
#endif _V_DEBUGMODE_

			// position materialIndex file pointer to the first line that contains the indecis
			if((ret=readToBegOfNum(line1, vFile[1]))!=SUCCESS)
			{
				if(ret==NOERR)
				{
					fprintf(stderr,
						"Expected array of numbers after materialIndex specifier\n");
					return(SYNTAXERR);
				}
				else
					return(ret);
			}
			
			materialIndexFound=V_TRUE;
			break;
		}
	} while(strchr(line1, CLOSEBRACE)==NULL &&
		(tmpPtr=spFgets(line1, MAXLINE, vFile[1]))!=NULL);

	if(tmpPtr==NULL)
	{
		if(feof(vFile[1]))
			return(GENUNEXPEOFERR);
		else
			return(GENINPREADERR);
	}

	// search for normalIndex array
	do
	{	
		if(strstr(line2, "normalIndex")!=NULL)
		{
#ifdef _V_DEBUGMODE_
			if(debugMode)
				printf("normalIndex Found!\n");
#endif _V_DEBUGMODE_

			// position normalIndex file pointer to the first line that contains the indecis	
			if((ret=readToBegOfNum(line2, vFile[2]))!=SUCCESS)
			{
				if(ret==NOERR)
				{
					fprintf(stderr,
						"Expected array of numbers after normalIndex specifier\n");
					return(SYNTAXERR);
				}
				else
					return(ret);
			}

			normalIndexFound=V_TRUE;
			break;
		}
	} while(strchr(line2, CLOSEBRACE)==NULL &&
		(tmpPtr=spFgets(line2, MAXLINE, vFile[2]))!=NULL);

	if(tmpPtr==NULL)
	{
		if(feof(vFile[2]))
			return(GENUNEXPEOFERR);
		else
			return(GENINPREADERR);
	}

	// output vertex order
	if(reverseFaces)
	{
		if(fprintf(oglFile, "%sglFrontFace(%s);\n",
			filler[*iIdx+0], vertexOrderType[iArray[*iIdx].vertexOrder+1])<0)
			return(GENOUTPWRITEERR);
	}
	else if(fprintf(oglFile, "%sglFrontFace(%s);\n",
		filler[*iIdx+0], vertexOrderType[iArray[*iIdx].vertexOrder])<0)
		return(GENOUTPWRITEERR);

	// depending on the arrays found call the appropriate processing function that will
	// read each entry in the arrays simultaneously and output it
	if(materialIndexFound!=V_TRUE && normalIndexFound!=V_TRUE)
		ret=processCoordIndecis(line, iArray+(*iIdx), *iIdx, vFile[0], oglFile, iType);
	else if(materialIndexFound==V_TRUE && normalIndexFound==V_TRUE)
		ret=processCoordColNormIndecis(line, line1, line2, iArray+(*iIdx), *iIdx, vFile,
			oglFile, iType);
	else if(materialIndexFound==V_TRUE && normalIndexFound!=V_TRUE)
		ret=processCoordColIndecis(line, line1, iArray+(*iIdx), *iIdx, vFile, oglFile, iType);
	else if(materialIndexFound!=V_TRUE && normalIndexFound==V_TRUE)
		ret=processCoordNormIndecis(line, line2, iArray+(*iIdx), *iIdx, vFile, oglFile, iType);

	if(ret==NOERR)	
		ret=skipToCloseBrace(line, vFile[0]);

	if(ret==NOERR && DEF!=NULL)
	{
		if(iType==LINESET)
		{
			if(fprintf(oglFile, "%s/*** End of DEF: %s (IndexedLineSet) } ***/\n",
				filler[*iIdx], DEF)<0)
				return(GENOUTPWRITEERR);
		}
		else if(fprintf(oglFile, "%s/*** End of DEF: %s (IndexedFaceSet) } ***/\n",
			filler[*iIdx], DEF)<0)
			return(GENOUTPWRITEERR);
	}
		
	return(ret);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: handleTransform()
// 
// Description:
//	This function handles translations, rotations, scaleFactors and centers within Transform
//	nodes. scaleOrientations are ignored. These transformations are first accumulated
//	(if found) and then output in the following order: translations, +centers, scaleFactors,
//	rotations and -centers. Multiple transformations of one kind are output in the order they
//	were found (eg. translation1, rotation1, translation2 results in translation1,
//	translation2, rotation1).
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE handleTransform(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
					 FILE *vFile[], FILE *outFiles[])
{
	static BYTE ret;
	static VECTOR translArray[MAXTRANSLARRAY], centerArray[MAXCENTERARRAY];
	static VECTOR scaleFactArray[MAXSCALEFACTARRAY];
	static ROTATION rotArray[MAXROTARRAY];
	static BYTE translIdx, rotIdx, centerIdx, scaleFactIdx;
	static char *tmpPtr, *linePtr;
	static int i;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("Transform Found!\n");
#endif	_V_DEBUGMODE_

	if(DEF!=NULL && fprintf(oglFile, "%s/*** Start of DEF: %s (Transform) { ***/\n",
		filler[*iIdx], DEF)<0)
		return(GENOUTPWRITEERR);

	translIdx=rotIdx=centerIdx=scaleFactIdx=0;
	tmpPtr=line;

	// read each line of the Transform node
	do
	{
		// check for a translation command
		if(strstr(line, "translation")!=NULL)
		{
			// retireve the translation data
			linePtr=line;
			if((ret=retrieveGEN3DBLUNIT(line, &linePtr, (GEN3DBLUNIT*)translArray+translIdx,
				vFile[0]))!=SUCCESS)
			{
				if(ret==NOERR)
				{
					fprintf(stderr,
						"Could not find translation vector after translation specifier\n");
					return(SYNTAXERR);
				}
				
				return(ret);
			}

			translIdx++;
		}

		// check for a scaleFactor command
		if(strstr(line, "scaleFactor")!=NULL)
		{
			// retrieve the scaleFactor data
			linePtr=line;
			if((ret=retrieveGEN3DBLUNIT(line, &linePtr,
				(GEN3DBLUNIT*)scaleFactArray+scaleFactIdx, vFile[0]))!=SUCCESS)
			{
				if(ret==NOERR)
				{
					fprintf(stderr,
						"Could not find scale factor after scale specifier\n");
					return(SYNTAXERR);
				}
				
				return(ret);
			}

			scaleFactIdx++;
		}

		// check for a center command
		if(strstr(line, "center")!=NULL)
		{
			// retrieve the center data
			linePtr=line;
			if((ret=retrieveGEN3DBLUNIT(line, &linePtr, (GEN3DBLUNIT*)centerArray+centerIdx,
				vFile[0]))!=SUCCESS)				
			{
				if(ret==NOERR)
				{
					fprintf(stderr,
						"Could not find center vector after center specifier\n");
					return(SYNTAXERR);
				}
				
				return(ret);
			}

			centerIdx++;
		}

		// check for a rotation command
		if(strstr(line, "rotation")!=NULL)
		{
			// retrieve the rotation data
			linePtr=line;
			if((ret=retrieveGEN4DBLUNIT(line, &linePtr, (GEN4DBLUNIT*)rotArray+rotIdx,
				vFile[0]))!=NOERR)
				return(ret);			

			// convert VRML's radians to degrees
			rotArray[rotIdx].angle=radiansToDegree(rotArray[rotIdx].angle);
			rotIdx++;
		}
	} while(strchr(line, CLOSEBRACE)==NULL &&
		(tmpPtr=spFgets(line, MAXLINE, vFile[0]))!=NULL);

	if(tmpPtr==NULL)
	{
		if(feof(vFile[0]))
			return(GENUNEXPEOFERR);
		else
			return(GENINPREADERR);
	}

	if(ret!=NOERR)
	{
		if(ret==SUCCESS)
			ret=NOERR;
		else
			return(ret);
	}

	// output all transformations in pre-determined order
	for(i=0; i<translIdx; i++)
		if(fprintf(oglFile, "%sglTranslatef(%5.4f, %5.4f, %5.4f);\n",
			filler[*iIdx+0], translArray[i].x, translArray[i].y, translArray[i].z)<0)
			return(GENOUTPWRITEERR);
	for(i=0; i<centerIdx; i++)
		if(fprintf(oglFile, "%sglTranslatef(%5.4f, %5.4f, %5.4f);\n",
			filler[*iIdx+0], centerArray[i].x, centerArray[i].y, centerArray[i].z)<0)
			return(GENOUTPWRITEERR);
	for(i=0; i<rotIdx; i++)
		if(fprintf(oglFile, "%sglRotatef(%5.4f, %5.4f, %5.4f, %5.4f);\n",
			filler[*iIdx+0], rotArray[i].angle, rotArray[i].x, rotArray[i].y, rotArray[i].z)<0)
			return(GENOUTPWRITEERR);
	for(i=0; i<scaleFactIdx; i++)
		if(fprintf(oglFile, "%sglScalef(%5.4f, %5.4f, %5.4f);\n",
			filler[*iIdx+0], scaleFactArray[i].x, scaleFactArray[i].y, scaleFactArray[i].z)<0)
			return(GENOUTPWRITEERR);
	for(i=centerIdx-1; i>=0; i--)
		if(fprintf(oglFile, "%sglTranslatef(%5.4f, %5.4f, %5.4f);\n",
			filler[*iIdx+0], -centerArray[i].x, -centerArray[i].y, -centerArray[i].z)<0)
			return(GENOUTPWRITEERR);

	if(DEF!=NULL && fprintf(oglFile, "%s/*** End of DEF: %s (Transform) } ***/\n",
		filler[*iIdx], DEF)<0)
		return(GENOUTPWRITEERR);

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: handleTranslation()
// 
// Description:
//	This function handles Translation nodes. Multiple translations within one Translation
//	node are permitted and will be output in the same order as they were found.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE handleTranslation(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
					   FILE *vFile[], FILE *outFiles[])
{
	static BYTE ret;
	static VECTOR translation;	
	static char *tmpPtr, *linePtr;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("Translation Found!\n");
#endif	_V_DEBUGMODE_

	if(DEF!=NULL && fprintf(oglFile, "%s/*** Start of DEF: %s (Translation) { ***/\n",
		filler[*iIdx], DEF)<0)
		return(GENOUTPWRITEERR);

	// read each line of the Translation node
	tmpPtr=line;
	do
	{
		// find translation command
		if(strstr(line, "translation")!=NULL)
		{
			// retrieve translation data
			linePtr=line;
			if((ret=retrieveGEN3DBLUNIT(line, &linePtr, (GEN3DBLUNIT*)&translation,
				vFile[0]))==SUCCESS)
			{
				if(fprintf(oglFile, "%sglTranslatef(%5.4f, %5.4f, %5.4f);\n",
					filler[*iIdx+0], translation.x, translation.y, translation.z)<0)
					return(GENOUTPWRITEERR);
			}
			else
			{
				if(ret==NOERR)
				{
					fprintf(stderr,
						"Could not find translation vector after translation specifier\n");
					return(SYNTAXERR);
				}
				
				return(ret);
			}
		}
	} while(strchr(line, CLOSEBRACE)==NULL &&
		(tmpPtr=spFgets(line, MAXLINE, vFile[0]))!=NULL);	

	if(tmpPtr==NULL)
	{
		if(feof(vFile[0]))
			return(GENUNEXPEOFERR);
		else
			return(GENINPREADERR);
	}

	if(DEF!=NULL && fprintf(oglFile, "%s/*** End of DEF: %s (Translation) } ***/\n",
		filler[*iIdx], DEF)<0)
		return(GENOUTPWRITEERR);

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: handleInfo()
// 
// Description:
//	This function reads in strings contained in Info nodes and outputs them as comments.
//	Strings are permitted to span across multiple lines.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE handleInfo(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
				FILE *vFile[], FILE *outFiles[])
{
	static int len;
	static char *startPtr, *endPtr, *tmpPtr;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("Info Found!\n");
#endif	_V_DEBUGMODE_

	if(DEF!=NULL && fprintf(oglFile, "%s/*** Start of DEF: %s (Info) { ***/\n",
		filler[*iIdx], DEF)<0)
		return(GENOUTPWRITEERR);
	
	// output as many strings as this info node has (search for starting string quotes ( " ))
	tmpPtr=line;
	do
	{
		// search for starting string quote
		if((startPtr=strchr(line, '\"'))!=NULL)
		{
			startPtr++;
			do
			{
				// search for ending string quote which may or may not be in the same line
				// as the starting quote is
				if((endPtr=strchr(startPtr, '\"'))!=NULL)
				{
					// ending string quote found, output the rest of the string as a commment
					if((len=endPtr-startPtr)>0)
					{
						if(fprintf(oglFile, "%s/* %*.*s */\n",
							filler[*iIdx], len, len, startPtr)<0)
							return(GENOUTPWRITEERR);
					}

					break;
				}
				else
				{
					// no ending string quote found, output this line which is part of the
					// string as a comment
					if((len=strlen(startPtr))>0)
					{
						// exclude \r\n from comment
						if(startPtr[len-2]==13)
							startPtr[len-2]='\0';
						else
							startPtr[len-1]='\0';

						if(fprintf(oglFile, "%s/* %s */\n", filler[*iIdx], startPtr)<0)
							return(GENOUTPWRITEERR);
					}
				}
				
				startPtr=line;
			} while((tmpPtr=spFgets(line, MAXLINE, vFile[0]))!=NULL);

			if(tmpPtr==NULL)
			{
				if(feof(vFile[0]))
					return(GENUNEXPEOFERR);
				else
					return(GENINPREADERR);
			}
		}
	} while(strchr(line, CLOSEBRACE)==NULL &&
		(tmpPtr=spFgets(line, MAXLINE, vFile[0]))!=NULL);

	if(tmpPtr==NULL)
	{
		if(feof(vFile[0]))
			return(GENUNEXPEOFERR);
		else
			return(GENINPREADERR);
	}

	if(DEF!=NULL && fprintf(oglFile, "%s/*** End of DEF: %s (Info) } ***/\n",
		filler[*iIdx], DEF)<0)
		return(GENOUTPWRITEERR);

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: handleScale()
// 
// Description:
//	This function handles Scale nodes. Multiple scaleFactors within one Scale node are
//	permitted and will be output in the same order as they were found.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE handleScale(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
				 FILE *vFile[], FILE *outFiles[])
{
	static BYTE ret;
	static VECTOR scaleFactor;	
	static char *tmpPtr, *linePtr;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("Scale Found!\n");
#endif	_V_DEBUGMODE_

	if(DEF!=NULL && fprintf(oglFile, "%s/*** Start of DEF: %s (Scale) { ***/\n",
		filler[*iIdx], DEF)<0)
		return(GENOUTPWRITEERR);

	// read all lines within the Scale node
	tmpPtr=line;
	do
	{
		if(strstr(line, "scaleFactor")!=NULL)
		{
			// retrieve scaleFactor data
			linePtr=line;
			if((ret=retrieveGEN3DBLUNIT(line, &linePtr, (GEN3DBLUNIT*)&scaleFactor,
				vFile[0]))==SUCCESS)
			{
				if(fprintf(oglFile, "%sglScalef(%5.4f, %5.4f, %5.4f);\n",
					filler[*iIdx+0], scaleFactor.x, scaleFactor.y, scaleFactor.z)<0)
					return(GENOUTPWRITEERR);
			}
			else
			{
				if(ret==NOERR)
				{
					fprintf(stderr,
						"Could not find scale factor after scale specifier\n");
					return(SYNTAXERR);
				}
				
				return(ret);
			}
		}
	} while(strchr(line, CLOSEBRACE)==NULL &&
		(tmpPtr=spFgets(line, MAXLINE, vFile[0]))!=NULL);	

	if(tmpPtr==NULL)
	{
		if(feof(vFile[0]))
			return(GENUNEXPEOFERR);
		else
			return(GENINPREADERR);
	}

	if(DEF!=NULL && fprintf(oglFile, "%s/*** End of DEF: %s (Scale) } ***/\n",
		filler[*iIdx], DEF)<0)
		return(GENOUTPWRITEERR);

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: handleMatrixTransform()
// 
// Description:
//	This function handles MatrixTransform nodes. If there are multiple matrices within this
//	node they will be output in the order found. The data for each 4x4 matrix will be written
//	to Dat file while commands referencing these matrix arrays will be output to the OGL file.
//	In addition for each matrix array an external variable definition is written to the Hdr
//	file.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE handleMatrixTransform(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
						   FILE *vFile[], FILE *outFiles[])
{
	static BYTE ret;
	static MATRIXROW row[4];
	static char *tmpPtr, *linePtr;
	static int i;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("MatrixTransform Found!\n");
#endif	_V_DEBUGMODE_

	if(DEF!=NULL && fprintf(oglFile, "%s/*** Start of DEF: %s (MatrixTransform) { ***/\n",
		filler[*iIdx], DEF)<0)
		return(GENOUTPWRITEERR);

	// read all lines contained in this node
	tmpPtr=line;
	do
	{
		if(strstr(line, "matrix")!=NULL)
		{
			// read in all four rows with four columns each
			linePtr=line;
			for(i=0; i<4; i++)
			{
				if((ret=retrieveGEN4DBLUNIT(line, &linePtr, (GEN4DBLUNIT*)row+i, vFile[0]))
					!=NOERR)
				{
					if(ret==SYNTAXERR)
						fprintf(stderr,	"Expected a 4x4 matrix after matrix specifier\n");
				
					return(ret);
				}
			}

			// output external variable definition for the current matrix
			if(fprintf(hdrFile, "extern GLfloat matrix%s%2.2d[];\n",
				baseName, matrixNum)<0)
				return(GENHDRWRITEERR);

			// output 4x4 matrix data as an array
			if(fprintf(datFile, "GLfloat matrix%s%2.2d[]=\n{\n" \
				"\t%5.4f, %5.4f, %5.4f, %5.4f,\n" \
				"\t%5.4f, %5.4f, %5.4f, %5.4f,\n" \
				"\t%5.4f, %5.4f, %5.4f, %5.4f,\n" \
				"\t%5.4f, %5.4f, %5.4f, %5.4f\n"  \
				"};\n", baseName, matrixNum,
				row[0].m0, row[0].m1, row[0].m2, row[0].m3,
				row[1].m0, row[1].m1, row[1].m2, row[1].m3,
				row[2].m0, row[2].m1, row[2].m2, row[2].m3,
				row[3].m0, row[3].m1, row[3].m2, row[3].m3)<0)
				return(GENDATWRITEERR);

			// output the OpenGL command referencing the current matrix
			if(fprintf(oglFile, "%sglMultMatrixf(matrix%s%2.2d);\n",
				filler[*iIdx+0], baseName, matrixNum++)<0)
				return(GENOUTPWRITEERR);
		}
	} while(strchr(line, CLOSEBRACE)==NULL &&
		(tmpPtr=spFgets(line, MAXLINE, vFile[0]))!=NULL);	

	if(tmpPtr==NULL)
	{
		if(feof(vFile[0]))
			return(GENUNEXPEOFERR);
		else
			return(GENINPREADERR);
	}

	if(DEF!=NULL && fprintf(oglFile, "%s/*** End of DEF: %s (MatrixTransform) } ***/\n",
		filler[*iIdx], DEF)<0)
		return(GENOUTPWRITEERR);

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: handleRotation()
// 
// Description:
//	This function handles Rotation nodes. Multiple rotations within one Rotation node are
//	permitted and will be output in the same order as they were found.
// Note: VRML specifies angles in radians which are converted to degrees for use by OpenGL.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE handleRotation(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
					FILE *vFile[], FILE *outFiles[])
{
	static BYTE ret;
	static ROTATION rotation;
	static char *tmpPtr, *linePtr;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("Rotation Found!\n");
#endif	_V_DEBUGMODE_

	if(DEF!=NULL && fprintf(oglFile, "%s/*** Start of DEF: %s (Rotation) { ***/\n",
		filler[*iIdx], DEF)<0)
		return(GENOUTPWRITEERR);

	// read each line of the Rotation node
	tmpPtr=line;
	do
	{
		if(strstr(line, "rotation")!=NULL)
		{
			linePtr=line;
			if((ret=retrieveGEN4DBLUNIT(line, &linePtr, (GEN4DBLUNIT*)&rotation, vFile[0]))
				==NOERR)
			{
				if(fprintf(oglFile, "%sglRotatef(%5.4f, %5.4f, %5.4f, %5.4f);\n",
					filler[*iIdx+0],
					radiansToDegree(rotation.angle), rotation.x, rotation.y, rotation.z)<0)
					return(GENOUTPWRITEERR);
			}
			else
			{
				if(ret==SYNTAXERR)
					fprintf(stderr,	"Could not find rotation data after rotation specifier\n");
				
				return(ret);
			}
		}
	} while(strchr(line, CLOSEBRACE)==NULL &&
		(tmpPtr=spFgets(line, MAXLINE, vFile[0]))!=NULL);	

	if(tmpPtr==NULL)
	{
		if(feof(vFile[0]))
			return(GENUNEXPEOFERR);
		else
			return(GENINPREADERR);
	}

	if(DEF!=NULL && fprintf(oglFile, "%s/*** End of DEF: %s (Rotation) } ***/\n",
		filler[*iIdx], DEF)<0)
		return(GENOUTPWRITEERR);

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: handleMaterial()
// 
// Description:
//	This function handles ambientColor, diffuseColor, specularColor, emissiveColor, shininess
//	and transparency material properties contained within each Material node. The data for
//	each of these properties (excluding shininess values which are simply stored in memory
//	for the	current indentation) is temporarily read into memory and then written to the Dat
//	file as	arrays (each fourth array element originates from the transparency array). The
//	appropriate external variable definitions for each array are written to the Hdr file.
//	Each material data array is padded (with zeros) up to the length of the longest material
//	data array found within this node.
// Note: These material properties will be referenced by the appropriate OpenGL statements
//		 once a subsequent IndexedFaceSet node is found (see handleIndexedFLSet()).
//		 Each material property for which (absolutely) no data was found, receives
//		 pre-determined default values.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE handleMaterial(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
					FILE *vFile[], FILE *outFiles[])
{	
	static BYTE ret;
	static long ambCount, diffCount, specCount, maxMatCount, emissCount, transpCount;
	static double shininess, *transpArrayPtr, *tmpTranspArrayPtr, *wArray;
	static COLOR *ambArrayPtr, *diffArrayPtr, *specArrayPtr, *emissArrayPtr;
	static char varName[MAXVARNAME];
	static char *linePtr;
	static int i;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("Material Found!\n");
#endif	_V_DEBUGMODE_

	if(DEF!=NULL && fprintf(oglFile, "%s/*** DEF: %s (Material *Mat%s%4.4d) ***/\n",
		filler[*iIdx], DEF, baseName, matArrayNum)<0)
		return(GENOUTPWRITEERR);

	ret=NOERR;
	ambArrayPtr=diffArrayPtr=specArrayPtr=emissArrayPtr=NULL;
	transpArrayPtr=tmpTranspArrayPtr=NULL;
	ambCount=diffCount=specCount=emissCount=transpCount=0;
	wArray=workArray;

	// set shininess and transparency to defaults
	iArray[*iIdx].shininess=DEFAULTSHININESS;

	do
	{
		// check for the different material properties and if found, read their data into
		// memory
		if(strstr(line, "ambientColor")!=NULL)
		{
			if((ret=retrieveGEN3DBLUNITArray(line, &ambCount, wArray, MAXWORKARRAY,
				(GEN3DBLUNIT**)&ambArrayPtr, vFile[0]))!=NOERR)			
				break;
		}

		if(strstr(line, "diffuseColor")!=NULL)
		{
			if((ret=retrieveGEN3DBLUNITArray(line, &diffCount, wArray, MAXWORKARRAY,
				(GEN3DBLUNIT**)&diffArrayPtr, vFile[0]))!=NOERR)
				break;
		}

		if(strstr(line, "specularColor")!=NULL)
		{
			if((ret=retrieveGEN3DBLUNITArray(line, &specCount, wArray, MAXWORKARRAY,
				(GEN3DBLUNIT**)&specArrayPtr, vFile[0]))!=NOERR)
				break;
		}

		if(strstr(line, "emissiveColor")!=NULL)
		{
			if((ret=retrieveGEN3DBLUNITArray(line, &emissCount, wArray, MAXWORKARRAY,
				(GEN3DBLUNIT**)&emissArrayPtr, vFile[0]))!=NOERR)
				break;
		}

		if(strstr(line, "shininess")!=NULL)
		{
			linePtr=line;
			if((ret=getDouble(line, &linePtr, vFile[0], &shininess))!=SUCCESS)
			{
				if(ret==NOERR)
				{
					fprintf(stderr,
						"Could not find shininess value after shininess specifier\n");
					return(SYNTAXERR);
				}
			}
			
			// mutiply VRML's shininess value by 128 for use by OpenGL
			iArray[*iIdx].shininess=shininess*SHININESSCONVMULTIP;
			ret=NOERR;
		}

		if(strstr(line, "transparency")!=NULL)
		{
			if((ret=retrieveDoubleArray(line, &transpCount, wArray, MAXWORKARRAY,vFile[0]))
				!=NOERR)
				break;
			else
			{
				transpArrayPtr=wArray;
				wArray+=transpCount;		// keep transparency data in workArray
			}
		}
	}
	while(strchr(line, CLOSEBRACE)==NULL && spFgets(line, MAXLINE, vFile[0])!=NULL);

	if(ret==NOERR)
	{

		if(ferror(vFile[0]))
			return(GENINPREADERR);

		// find out which material color array is the longest (for padding)
		maxMatCount=ambCount;
		if(diffCount>maxMatCount)
			maxMatCount=diffCount;
		if(specCount>maxMatCount)
			maxMatCount=specCount;
		if(emissCount>maxMatCount)
			maxMatCount=emissCount;
		if(transpCount>maxMatCount)
			maxMatCount=transpCount;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("\tmaterialElementCount=%d\n", maxMatCount);
#endif	_V_DEBUGMODE_

		// output each of the different material color arrays
		if((iArray[*iIdx].matCount=maxMatCount)!=0)
		{
			if(transpCount!=maxMatCount)
			{
				if((tmpTranspArrayPtr=new double[maxMatCount])==NULL)
				  ret=GENALLOCERR;
				else
				{
					for(i=0; i<transpCount; i++)
						tmpTranspArrayPtr[i]=transpArrayPtr[i];

					for(;i<maxMatCount; i++)
						tmpTranspArrayPtr[i]=DFLTVRMLTRANSP;

					transpArrayPtr=tmpTranspArrayPtr;
				}
			}

			if(ret==NOERR)
			{
				sprintf(varName, "ambMat%s%4.4d", baseName, matArrayNum);
				if((ret=outputMatArrays(ambArrayPtr, ambCount, maxMatCount,
					transpArrayPtr, &defaultAmbMat, varName, outFiles))==NOERR)
				{
					sprintf(varName, "diffMat%s%4.4d", baseName, matArrayNum);
					if((ret=outputMatArrays(diffArrayPtr, diffCount, maxMatCount,
						transpArrayPtr, &defaultDiffMat, varName, outFiles))==NOERR)
					{
						sprintf(varName, "specMat%s%4.4d", baseName, matArrayNum);
						if((ret=outputMatArrays(specArrayPtr, specCount, maxMatCount,
							transpArrayPtr, &defaultSpecMat,varName, outFiles))==NOERR)
						{
							sprintf(varName, "emissMat%s%4.4d", baseName, matArrayNum);
							ret=outputMatArrays(emissArrayPtr, emissCount, maxMatCount,
								transpArrayPtr, &defaultEmissMat, varName, outFiles);
						}
					}
				}

			iArray[*iIdx].matArrayNum=matArrayNum++;
			}
		}
	}

	// deallocate the color material arrays found from memory
	if(ambArrayPtr!=NULL)
		delete [] ambArrayPtr;
	if(diffArrayPtr!=NULL)
		delete [] diffArrayPtr;
	if(specArrayPtr!=NULL)
		delete [] specArrayPtr;
	if(emissArrayPtr!=NULL)
		delete [] emissArrayPtr;
	if(tmpTranspArrayPtr!=NULL)
		delete [] tmpTranspArrayPtr;

	return(ret);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: handleGenericNode()
// 
// Description:
//	This function generically handles any type of node. It ignores the actual node contents
//	and advances the file pointer up to the line right after this node.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE handleGenericNode(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
					   FILE *vFile[], FILE *outFiles[])
{
#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("Generic Node Found!\n");
#endif _V_DEBUGMODE_

	if(DEF!=NULL && fprintf(oglFile, "%s/*** DEF: %s (IGNORED!) ***/\n", filler[*iIdx], DEF)<0)
		return(GENOUTPWRITEERR);

	return(skipToCloseParentBrace(line, vFile[0], iType==OPENBRACE));

	//return(skipToCloseBrace(line, vFile[0]));
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: handleUnimplementedNode()
// 
// Description:
//	This function causes the program to abort for the kind of node that invoked it.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE handleUnimplementedNode(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
							 FILE *vFile[], FILE *outFiles[])
{	
	fprintf(stderr, "Node not implemented!\n");
	return(SYNTAXERR);
}
