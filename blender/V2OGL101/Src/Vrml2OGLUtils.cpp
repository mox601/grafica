/********************************************************************************************
** Vrml2OGLUtils.cpp - Source Code Module for Vrml2OGL.exe V1.0 -> V1.01
** Copyright 1999 by Alexander Rohra. All rights reserved.
**
** Program Name:	Vrml2OGL.exe V1.0 -> V1.01
** Author:			Alexander Rohra
** Date:			May 1999
** Compiler:		MSVC++ V5.0 & others
** App Type:		Command Line Utility (Win32 Console Application)
** Revisions:		August 1999 - Alexander Rohra, as suggested by Scott Nelson -> V1.01:
**					1)	Replaced the fixed #include statement, that is inserted into the
**						output header file, by a platform independent #define
**						(GLHDRINCSTATMENT)
** Code Desc.:		This source code module contains miscellaneous utility
**					functions used by Vrml2OGL. They range from input
**					retrieval and output functions to memory allocation/
**					deallocation functions. For strictly math oriented
**					functions please see GeometryUtils.cpp.
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
#include <ctype.h>
#include <stdlib.h>

#include "Vrml2OGLUtils.h"			// needed for typedefs from "SharedDefs.h"
#include "FindNodes.h"				// needed for shared define statements
#include "GeometryUtils.h"			// needed for geometry function prototypes
#include "Vrml2OGL.h"				// needed for debugMode extern var definition

// *** global variables - private to this source code module***
static char begNumChars[]="01234567890.+-";		// chars that mark the beginning of a number
//static char midNumChars[]="01234567890DdEe.";	// chars that are contained in a number
static char totNumChars[]="01234567890DdEe.+-";	// all chars making up a number
static char fieldDelimiters[]={' ', '\t', '\n', ',', ';', 13};
static char *faceType[]={"NULL", "GL_POINTS", "GL_LINES", "GL_TRIANGLES", "GL_QUADS",
						 "GL_POLYGON"};

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: checkVersion()
// 
// Description:
//	This function reads the first line of the .WRL file and checks if it contains the string
//	"VRML V1.0 ascii". Depending on the outcome of this check it then returns the appropriate
//	status value.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE checkVersion(FILE *vFile)
{
	char line[MAXLINE];

	// read the very first line of the .WRL file
	if(fgets(line, MAXLINE, vFile)==NULL)
		return(GENINPREADERR);
	
	// check if it contains the correct VRML version string
	if(strstr(line, "VRML V1.0 ascii")==NULL)
	{
		if(strstr(line, "#VRML V")!=NULL)
			fprintf(stderr, "VRML versions other than V1.0 are not supported\n");
		else
			fprintf(stderr, "Could not find or recognize VRML version string in first line\n");

		return(VERSIONERR);
	}

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: findBoundingBox()
// 
// Description:
//	This function reads through the entire .WRL file to determine the minimum and maximum
//	x, y and z values of all the vertecis contained in all Coordinate3 arrays to determine the
//	bounding box of the object.
//	If no coordinates were found in the input file, an error message is ouput and SYNTAXERR
//	is returned. An error status will also be returned if a read error occurs or a nested
//	LOD (an LOD within an LOD) is found. If however none of the above occurs NOERR is returned.
// Note: If the file contains LOD nodes, only the 1st level of detail of each LOD node is
//		 scaned for Coordinate3 arrays, all other levels of detail are skipped (see also
//		 handleSubIndent()).
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE findBoundingBox(FILE *vFile, VERTEX *boundingBox)
{
	BYTE ret, coordinatesFound=V_FALSE;
	char line[MAXLINE], *linePtr, *tmpPtr;
	double minX, minY, minZ, maxX, maxY, maxZ;
	VERTEX vertex;
	int braceCount=0, currLOD=0;
	unsigned int i;
	BYTE inLOD=V_FALSE, continueLineScan=V_FALSE;
	BYTE firstCoordinate3Array=V_TRUE;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("Finding Bounding Box\n");
#endif _V_DEBUGMODE_

	// read and scan each line in the .WRL file
	while(continueLineScan || spFgets(line, MAXLINE, vFile)!=NULL)
	{
		continueLineScan=V_FALSE;

		// check if this line contains an LOD node, if so start counting all open and
		// close braces in order to be able to identify each level of detail
		if((linePtr=strstr(line, "LOD"))!=NULL)
		{
			if(inLOD)
			{
				fprintf(stderr, "Nested LOD nodes are not supported\n");
				return(NESTEDLODERR);
			}
			else
			{
				inLOD=V_TRUE;
				braceCount=0;
			}
		}
		else
			linePtr=line;

		// check if this line containes a Coordinate3 node
		tmpPtr=strstr(line, "Coordinate3");

		// if we are in an LOD check each line for open and close braces
		if(inLOD)
		{
			for(i=0; i<strlen(linePtr); i++)
				if(linePtr[i]==OPENBRACE)
				{
					// brace #1 belongs to the LOD node specifier, so each time we return
					// to LOD #1 it means that a new level of detail starts
					if(braceCount==1)
					{
						currLOD++;
#ifdef _V_DEBUGMODE_
						if(debugMode)
							printf("\tFound LOD #%d\n", currLOD);
#endif _V_DEBUGMODE_
					}

					braceCount++;
				}
				else if(linePtr[i]==CLOSEBRACE)
				{
					if(!--braceCount)
						inLOD=V_FALSE;
				}
		}

		// if the current line contained a Coordinate3 node specifier and we are not in
		// an LOD or are in the correct level of detail, scan all vertecis to determine
		// the object's bounding box
		if(tmpPtr!=NULL && (!inLOD || (inLOD && currLOD==ACTIVELOD)))
		{
			// erase everything until after the node name to make sure the '3' in the
			// specifier is not considered the first array element
			memset(line, ' ', tmpPtr+strlen("Coordinate3")-line);

			// advance file pointer to the first line containing numerical entries
			if((ret=readToBegOfNum(line, vFile))!=SUCCESS)
			{
				if(ret==NOERR)
				{
					fprintf(stderr, "Expected array of numbers after Coordinate3 specifier\n");
					return(SYNTAXERR);
				}
				else
					return(ret);
			}

#ifdef _V_DEBUGMODE_
			if(debugMode)
				printf("\tFound the beginning of array\n");
#endif _V_DEBUGMODE_

			// get very first vertex and consider it the max and min vertex
			if(firstCoordinate3Array)
			{
				firstCoordinate3Array=V_FALSE;
				if((ret=retrieveGEN3DBLUNIT(line, &linePtr, (GEN3DBLUNIT*) &vertex,
					vFile))==SUCCESS)
				{
					coordinatesFound=V_TRUE;	// set flag that some coordinates were found
					minX=vertex.x; minY=vertex.y; minZ=vertex.z;
					maxX=vertex.x; maxY=vertex.y; maxZ=vertex.z;
				}
				else if(ret==NOERR)
				{
					fprintf(stderr, "Expected at least one complete 3D vertex after" \
						" Coordinate3 specifier\n");
					return(SYNTAXERR);
				}
				else
					return(ret);
			}
			
			// read all vertecis in this array and update the min and max x, y and z values
			while((ret=retrieveGEN3DBLUNIT(line, &linePtr, (GEN3DBLUNIT*) &vertex, vFile))
				==SUCCESS)
			{
				if(vertex.x<minX)
					minX=vertex.x;
				else if(vertex.x>maxX)
					maxX=vertex.x;

				if(vertex.y<minY)
					minY=vertex.y;
				else if(vertex.y>maxY)
					maxY=vertex.y;

				if(vertex.z<minZ)
					minZ=vertex.z;
				else if(vertex.z>maxZ)
					maxZ=vertex.z;
			}

			if(ret!=NOERR)
				return(ret);

			// the file pointer is now positioned in the last line of the Coordinate3 node
			// continue to scan this line to make sure all braces are found
			if(inLOD)
				continueLineScan=V_TRUE;
		}
	}

	// check for an un-closed LOD
	if(inLOD)
	{
		if(braceCount==1)	// singular
		{
			fprintf(stderr, "1 close brace was not found - corrupt input file?\n");
			return(SYNTAXERR);
		}
		else if(braceCount)	// plural
		{
			fprintf(stderr, "%d close braces were not found - corrupt input file?\n",
				braceCount);
			return(SYNTAXERR);
		}
	}

	if(!coordinatesFound)
	{
		fprintf(stderr, "The input file does not contain any coordinates. Aborted.\n");
		return(SYNTAXERR);
	}


	// generate bounding box from min and max x, y and z values
	boundingBox[0].x=minX; boundingBox[0].y=maxY; boundingBox[0].z=minZ;	// P0	 /P0----P1
	boundingBox[1].x=maxX; boundingBox[1].y=maxY; boundingBox[1].z=minZ;	// P1	P4--+-P5/|
	boundingBox[2].x=maxX; boundingBox[2].y=minY; boundingBox[2].z=minZ;	// P2	|  |  |  |
	boundingBox[3].x=minX; boundingBox[3].y=minY; boundingBox[3].z=minZ;	// P3	| P3--+-P2
	boundingBox[4].x=minX; boundingBox[4].y=maxY; boundingBox[4].z=maxZ;	// P4	P7/---P6/
	boundingBox[5].x=maxX; boundingBox[5].y=maxY; boundingBox[5].z=maxZ;	// P5
	boundingBox[6].x=maxX; boundingBox[6].y=minY; boundingBox[6].z=maxZ;	// P6
	boundingBox[7].x=minX; boundingBox[7].y=minY; boundingBox[7].z=maxZ;	// P7

#ifdef _V_DEBUGMODE_
	if(debugMode)
	{
		printf("Bounding Box:\n");
		for(int i=0; i<8; i++)
			printf("\t(%5.4f, %5.4f, %5.4f)\n",
				boundingBox[i].x, boundingBox[i].y, boundingBox[i].z);
	}
#endif _V_DEBUGMODE_

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: spFgets()
// 
// Description:
//	This function (spFgets=special fgets) adds on to the functionality of fgets by calling
//	fgets() and scanning a read in line for VRML # comments and excluding them if found
//	prior to returning control to the calling function.
/////////////////////////////////////////////////////////////////////////////////////////////
char *spFgets(char *string, int n, FILE *stream)
{
	static int i;
	static char *retPtr;

	if((retPtr=fgets(string, n, stream ))==NULL)	
		return(NULL);
	else
	{
		// can scan line for a # sign, replace it by '\0' if found
		n=strlen(string);
		for(i=0; i<n; i++)
		{
			if(string[i]==COMMENTCHAR)
			{
				string[i]='\0';
				break;
			}
		}

		return(retPtr);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: getDEF()
// 
// Description:
//	This function scans a passed in line for a DEF specifier and if found retrieves and
//	passes back a name tag. Otherwise NULL is returned.
// Note: The line passed to this function is expected to have been terminated prior to
//		 a recognized node specifier. Therefore anything after a DEF specifier is considered
//		 a name tag (since it is surounded by a DEF and a (excluded) node specifier).
/////////////////////////////////////////////////////////////////////////////////////////////
char *getDEF(char *line)
{
	static char DEF[MAXLINE], *foundAddr;
	static int i, j;

	j=0;

	// make sure the line is at least more than 4 chars long otherwise it can impossibly
	// have a DEF specifier and tag in it ("DEF a"==5 chars minimum)
	if(strlen(line)>4)
	{
		// check if this line contains a DEF and if so make sure the line extends
		// enough beyond the DEF specifier to contain at least a one character name
		// also make sure that if the 3 letter sequence "DEF" is found it is not part of some
		// tag or specifier
		if((foundAddr=strstr(line, "DEF"))!=NULL && strlen(foundAddr+3)>1 &&
			((foundAddr==line && foundAddr[3]<=32) ||
			 (foundAddr!=line && foundAddr[-1]<=32 && foundAddr[3]<=32)))
		{
			// copy the tag
			for(i=4; foundAddr[i]!='\0'; i++)
				if(isascii(foundAddr[i]) && foundAddr[i]!=' ')
					DEF[j++]=foundAddr[i];
		}

		DEF[j]='\0';
	}

	if(j)
		return(DEF);

	return(NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: addDefToList()
// 
// Description:
//	This function allocates memory for a passed in DEF name tag, adds it on top of the
//	current indentation's DEFList and records the current indentation's openedSubIndents
//	count in order to be able to identify where this DEF ends.
// Note: If the current indentations DEFList is exhausted a warning is output and the DEF
//		 is not added to the list. This DEF will therefore be excluded from the output file.
//		 The function will still return NOERR; GENALLOCERR is only returned if an allocation
//		 error occurs.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE addDefToList(INDENTATION *iArrayElement, int iIdx, char *DEF)
{
	// make sure the expected maximum amount of DEFs has not already been reached
	if(iArrayElement->DEFIdx==MAXDEFSPERINDENT)
	{
		fprintf(stderr, "Warning: Maximum number of DEFs per indent reached ignoring: %s\n",
			DEF);
		return(NOERR);
	}
	
	// record current subIndentNo to be able to identify where this DEF ends
	iArrayElement->DEFList[iArrayElement->DEFIdx].subIndentNo=iArrayElement->openedSubIndents;

	// allocate memory for string and attach it to the DEFList of the current indentation
	if((iArrayElement->DEFList[iArrayElement->DEFIdx].DEF=new char[strlen(DEF)+1])==NULL)
		return(GENALLOCERR);
	
	// copy the DEF name tag into the allocated space and increase the current DEF list index
	strcpy(iArrayElement->DEFList[iArrayElement->DEFIdx].DEF, DEF);
	iArrayElement->DEFIdx++;

#ifdef _V_DEBUGMODE_
		if(debugMode)
			printf("\tAdded DEF[%d][%3.3d]!\n", iIdx, iArrayElement->DEFIdx-1);
#endif _V_DEBUGMODE_

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: skipToCloseParentBrace()
// 
// Description:
//	This function searches for open and close braces which increase and decrease a given
//	braceCount until it returns to zero. This is used to skip an entire node - ignoring	its
//	contents and advancing the file pointer to the line containing its final close brace.
//	Usually the initialBraceCount argument is zero in which case the function considers
//	the first open brace found the starting brace of this node. In some cases a previous
//	routine might have already found one or more open braces and just needs to advance
//	the file pointer to the position in the input file where the close brace is found that
//	returns the braceCount to zero.
//	An error is returned if the file ends prior to finding the close brace or if an IO error
//	occurs.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE skipToCloseParentBrace(char *line, FILE *vFile, long initalBraceCount)
{
	static int i, len;
	static long braceCount;
	static BYTE foundMinimum;

	braceCount=initalBraceCount;	// account for previously found open braces

	// make sure we find at least one open brace before returning to the loop ending
	// brace count of zero - this can be overridden by passing in an initialBraceCount>0
	foundMinimum=initalBraceCount? V_TRUE : V_FALSE;

	do
	{
		// increase braceCount for each OPENBRACE and decrease it for each CLOSEBRACE
		len=strlen(line);
		for(i=0; i<len && (braceCount || !foundMinimum); i++)
		{
			if(line[i]==OPENBRACE)
			{
				braceCount++;
				foundMinimum=V_TRUE;
			}
			else if(line[i]==CLOSEBRACE)
				braceCount--;
		}
	}
	while(braceCount && spFgets(line, MAXLINE, vFile)!=NULL);

#ifdef _V_DEBUGMODE_
	if(debugMode && !braceCount)
		printf("\tClose Parent Brace found!\n");
#endif _V_DEBUGMODE_

	if(feof(vFile)  && braceCount)	// expected closing brace
	{
		fprintf(stderr, "Unexpected end of file found (%d generic parent close braces" \
			" are missing)\n", braceCount);
		return(SYNTAXERR);
	}

	if(ferror(vFile))	
		return(GENINPREADERR);
	
	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: skipToCloseBrace()
// 
// Description:
//	This function advances the input file pointer to the line that contains the next close
//	brace.
//	It will return an error if the close brace found does not terminate the line it is in
//	(eg. lines containing "}}" or "}   Material", etc. will cause an error).
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE skipToCloseBrace(char *line, FILE *vFile)
{
	static char *ptr;
	static int i;

	do	
	{
		if((ptr=strchr(line, CLOSEBRACE))!=NULL)
		{
			// make sure close brace was found at the end of the line
			for(i=1; ptr[i]!=0 && ptr[i]<=32; i++);
			if(ptr[i]!=0)
			{				
				fprintf(stderr, "Expected closing brace ('%c') at the end of the line:\n",
				  CLOSEBRACE);
				fprintf(stderr, "%s (%d)", line, ptr[i]);
				return(SYNTAXERR);
			}
#ifdef _V_DEBUGMODE_
			else if(debugMode)
				printf("Close Brace found!\n");
#endif _V_DEBUGMODE_

			break;
		}
	}
	while(spFgets(line, MAXLINE, vFile)!=NULL);

	if(feof(vFile))	// expected closing brace
	{
		fprintf(stderr, "Expected closing brace ('%c') prior to end of file\n", CLOSEBRACE);
		return(SYNTAXERR);
	}
	else if(ferror(vFile))	
		return(GENINPREADERR);
	
	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: findBegOfNum()
// 
// Description:
//	This function searches the passed in line for characters that could resemble the
//	beginning of a number. If a prospective character found is not a digit (such as a '-'
//	or '.' symbol) it makes sure that it is followed by a digit before qualifying the
//	current position in the line as the beginning of a number.
//	If a number was found the pointer to the beginning of the number is returned otherwise
//	NULL is returned.
//	Note: It is not ensured that a digit found is not part of some string (eg. a line
//		  containing the string "Coordinate3" would qualify as a line containing a number).
/////////////////////////////////////////////////////////////////////////////////////////////
char *findBegOfNum(char *line)
{
	static char *ptr;
	static int i, j, k;

	ptr=NULL;

	for(i=0; line[i]!=0 && ptr==NULL; i++)
	{
		// check each char of the line against chars that could resemble the beginning of a
		// number
		for(j=0; j<=BEGNUMCHARCOUNT && ptr==NULL; j++)
		{
			// found some kind of character to could resemble the beginning of a number
			if(line[i]==begNumChars[j])
			{
				// found a digit between 0-9
				if(j<BEGNUMCHARCOUNT-SYMBCHARCOUNT)
				{
					ptr=line+i;
					break;
				}
				else	// found a symbol (. or -)
				{
					// check for a following digit (0-9)
					for(k=0; k<BEGNUMCHARCOUNT-SYMBCHARCOUNT; k++)
						if(line[i+1]==begNumChars[k])
						{
							ptr=line+i;
							break;
						}
				}
			}
		}
	}

	return(ptr);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: readToBegOfNum()
// 
// Description:
//	This function advances the file pointer to the first line containing a number.
//	If such a line was found SUCCESS is returned, if it could not be found before
//	the ending of the input file NOERR is returned; otherwise in case of an IO error
//	GENINPERROR is returned.
//	Note: See note for findBegOfNum().
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE readToBegOfNum(char *line, FILE *vFile)
{
	do
	{
		if(findBegOfNum(line)!=NULL)
			return(SUCCESS);
		//else if(strchr(line, CLOSEBRACE)!=NULL)
		//	break;
	} while(spFgets(line, MAXLINE, vFile)!=NULL);

	if(ferror(vFile))	
		return(GENINPREADERR);
	
	return(NOERR);		// we didn't find any number beginning and there was no error
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: getNumLength()
// 
// Description:
//	This function traverses a passed in string until a character not qualifying as part
//	of a number is found. It then returns the amount of positions that contained number
//	qualifying characters prior to a non number qualifying character.
//	Note: This function is used to indentify the string length of a number string contained
//		  in a line. The str pointer passed in is expected to be positioned at the beginning
//		  of a number.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE getNumLength(char *str)
{
	static BYTE i;

	// check each position of the string against a list qualifying number components
	for(i=0; strchr(totNumChars, str[i])!=NULL; i++);

	return(i);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: writeFileHeaders()
// 
// Description:
//	This function writes generic file headers to all three output files (OGL, Hdr & Dat).
// Note: Some of the information written is expected to be "wraped up" by
//	     writeFileFooters().
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE writeFileHeaders(FILE *outFiles[], HEADERFOOTERINFO *hfInfo)
{
	char tmpStr1[MAXHDRTMPSTR];
	char tmpStr2[MAXHDRTMPSTR];

	// choose text to be appear in the header of the OpenGL file
	if(hfInfo->cmdLSwitches!=NULL)
		sprintf(tmpStr1, "**\tThe following command line switches were applied:\n**\t%s\n",
			hfInfo->cmdLSwitches);
	else
		sprintf(tmpStr1, "**\tNo command line switches were applied.\n");

	if(hfInfo->center!=NULL)
		sprintf(tmpStr2, "**\tThe following object center was subtracted:\n**\t%s\n",
			hfInfo->center);
	else
		tmpStr2[0]='\0';

	// write comment and function header to OGL file
	if(fprintf(oglFile,
		"/*****************************************************************************\n" \
		"** %s - OpenGL C Code, File 1 of 3\n" \
		"**\tGenerated on %s from \"%s\" by\n" \
		"**\t%s Copyright 1999 by Alexander Rohra.\n" \
		"%s%s" \
		"*****************************************************************************/\n\n" \
		GLHDRINCSTATMENT \
		"\n" \
		"#include \"%s\"\n" \
		"\nvoid %s(void)\n" \
		"{\n" \
		"%sglPushAttrib(GL_ALL_ATTRIB_BITS);\n" \
		"%s/*** The following lines have been inserted by %s ***/\n" \
		"%sglEnable(GL_NORMALIZE);\n" \
		"%sglCullFace(_CULL_FACE);\n" \
		"%sglEnable(GL_CULL_FACE);\n" \
		"%sglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);\n" \
		"%sglEnable(GL_BLEND);\n" \
		"%s/*** End of insert **********************************************/\n",
		hfInfo->oglFName, hfInfo->currDate, hfInfo->inFName, hfInfo->prgName, tmpStr1, tmpStr2,
		hfInfo->hdrFName, hfInfo->funcName, filler[0], filler[0], hfInfo->prgName, filler[0],
		filler[0], filler[0], filler[0], filler[0], filler[0])<0)
		return(GENOUTPWRITEERR);

	// write comment header and a few definitions to Hdr file
	if(fprintf(hdrFile, 
		"/*****************************************************************************\n" \
		"** %s - OpenGL C Code, File 2 of 3\n" \
		"**\tGenerated on %s from \"%s\" by\n" \
		"**\t%s Copyright 1999 by Alexander Rohra.\n" \
		"**\tSee %s for more details.\n" \
		"*****************************************************************************/\n\n" \
		"#ifndef %s\n" \
		"#define %s\n" \
		"\n"
		GLHDRINCSTATMENT \
		"\n" \
		"\n#define _CULL_FACE\tGL_BACK\n" \
		"#define _MAT_FACE\tGL_FRONT_AND_BACK\n" \
		"\nextern void %s(void);\n\n",
		hfInfo->hdrFName, hfInfo->currDate, hfInfo->inFName, hfInfo->prgName, hfInfo->oglFName,
		hfInfo->hdrDefine, hfInfo->hdrDefine, hfInfo->funcName)<0)
		return(GENHDRWRITEERR);

	// write comment header and includes to Dat file
	if(fprintf(datFile,
		"/*****************************************************************************\n" \
		"** %s - OpenGL C Code, File 3 of 3\n" \
		"**\tGenerated on %s from \"%s\" by\n" \
		"**\t%s Copyright 1999 by Alexander Rohra.\n" \
		"**\tSee %s for more details.\n" \
		"*****************************************************************************/\n\n" \
		GLHDRINCSTATMENT \
		"\n" \
		"#include \"%s\"\n\n",
		hfInfo->datFName, hfInfo->currDate, hfInfo->inFName, hfInfo->prgName, hfInfo->oglFName,
		hfInfo->hdrFName)<0)
		return(GENDATWRITEERR);

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: writeFileFooters()
// 
// Description:
//	This function writes generic file footers to all three output files "wrapping up" some
//	of the information written by writeFileHeaders().
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE writeFileFooters(FILE *outFiles[], HEADERFOOTERINFO *hfInfo)
{
	// write function footer to OGL file
	if(fprintf(oglFile,
		"%s/*** The following lines have been inserted by %s ***/\n" \
		"%sglDisable(GL_BLEND);\n" \
		"%sglDisable(GL_CULL_FACE);\n" \
		"%sglDisable(GL_NORMALIZE);\n" \
		"%s/*** End of insert **********************************************/\n" \
		"%sglPopAttrib();\n" \
		"}\n",
		filler[0], hfInfo->prgName, filler[0], filler[0], filler[0], filler[0], filler[0])<0)
		return(GENOUTPWRITEERR);

	// write closing #endif to Hdr file
	if(fprintf(hdrFile, "\n#endif /* %s */\n", hfInfo->hdrDefine)<0)
		return(GENHDRWRITEERR);

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: retrieveDoubleArray()
// 
// Description:
//	This function reads an array of doubles from the input file into the passed in work array.
//	numCount will contain the number of doubles found.
// Note: This function will abort with an error status if the data retrieved from the input
//		 file is about to exceede the capacity of the work array.
//		 The file pointer is expected to be positioned so that the first number found in
//		 the current or following lines represents the beginning of the array to retrieve.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE retrieveDoubleArray(char *line, long *numCount, double *wArray, long maxWArray,
						 FILE *vFile)
{
	static BYTE ret;
	static SBYTE tmpVal;
	static char chr;

	*numCount=0;

	// advance file pointer to the first line containing numbers
	if((ret=readToBegOfNum(line, vFile))!=SUCCESS)
	{
		if(ret==NOERR)
		{
			fprintf(stderr, "Expected array of numbers after specifier\n");
			return(SYNTAXERR);
		}
		else
			return(ret);
	}

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("\tFound beginning of array\n");
#endif _V_DEBUGMODE_

	// read each line until the end of the number array
	do
	{
		// make sure the internal work array will not overflow
		if(*numCount+MAXEXPNUMSPLINE>=maxWArray)
		{
			fprintf(stderr, "Too many numbers for internal work array (so far: %ld)\n",
				*numCount);
			return(OUTOFWARRAYERR);
		}
		// retrieve all doubles from this line and add them to the work array
		else if((tmpVal=retrieveDoubles(line, wArray+(*numCount)))<=0)
		{
			tmpVal*=-1;
			*numCount+=tmpVal;
			break;
		}
		else
			*numCount+=tmpVal;
	} while(spFgets(line, MAXLINE, vFile)!=NULL &&
		(chr=getFirstNonBlankChar(line))!='a' && chr!='d' && chr!='s' && chr!='t' && chr!='e');

	if(ferror(vFile))
		return(GENINPREADERR);

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: retrieveDoubles()
// 
// Description:
//	This function retrieves all doubles (floating point numbers) contained in the passed in
//	line and stores them in the supplied double array.
//	The return value is the amount of doubles retrieved from this line - this value
//	is multiplied by (-1) if the line contained a character different from a standard field
//	delimiter in which case it is assumed to represent the end of the input double array.
//	The calling function can therefore interpret a return value of <=0 as: ret*(-1) doubles
//	were found and the input array is now exhausted. 
/////////////////////////////////////////////////////////////////////////////////////////////
SBYTE retrieveDoubles(char *line, double *array)
{
	static SBYTE count;
	static int i, len;
	static char *ptr, *linePtr;
	static char numStr[MAXLINE];

	count=0;
	linePtr=line;

	// traverse through the current line, retrieve all the doubles it contains and add them
	// to the passed in array
	while((ptr=findBegOfNum(linePtr))!=NULL)
	{
		len=getNumLength(ptr);
		strncpy(numStr, ptr, len);
		numStr[len]=0;
		array[count++]=atof(numStr);
		linePtr=ptr+len;
	}
	
	// check if the delimiter at the end of this line is different than the standard
	// field delimiter in which case it is assumed to denote the end of the array (eg. ']')
	for(i=0; linePtr[i]!=0; i++)
	{
		if(strchr(fieldDelimiters, linePtr[i])==NULL)
		{
			count*=-1;
			break;
		}
	}	

	return(count);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: centerCopyWArrayToOwnGEN3DBLUNITArray()
// 
// Description:
//	This function allocates memory space for the contents of the supplied work array,
//	subtracts a 3 double center value from each set of 3 doubles (usually representing a
//	vertex) from the work array and	copies the result to one generic 3 double unit into
//	the allocated array.
//	If a memory allocation error occurs NULL is returned; otherwise the address to the
//	allocated (and populated) array is returned.
/////////////////////////////////////////////////////////////////////////////////////////////
GEN3DBLUNIT *centerCopyWArrayToOwnGEN3DBLUNITArray(double *wArray, long count, VERTEX center)
{
	static GEN3DBLUNIT *oArray;
	static long i, j;

	if((oArray=new GEN3DBLUNIT[count/3])!=NULL)
	{
		for(i=j=0; i+2<count; i+=3)
		{
			oArray[j].item1=wArray[i+0]-center.x;
			oArray[j].item2=wArray[i+1]-center.y;
			oArray[j].item3=wArray[i+2]-center.z;
			j++;
		}
	}

	return(oArray);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: copyWArrayToOwnGEN3DBLUNITArray()
// 
// Description:
//	This function allocates memory space for the contents of the supplied work array
//	and copies each 3 doubles from the work array (usually representing a vertex) into one
//	generic 3 double unit in the allocated array.
//	If a memory allocation error occurs NULL is returned; otherwise the address to the
//	allocated (and populated) array is returned.
/////////////////////////////////////////////////////////////////////////////////////////////
GEN3DBLUNIT *copyWArrayToOwnGEN3DBLUNITArray(double *wArray, long count)
{
	static GEN3DBLUNIT *oArray;
	static long i, j;

	if((oArray=new GEN3DBLUNIT[count/3])!=NULL)
	{
		for(i=j=0; i+2<count; i+=3)
		{
			oArray[j].item1=wArray[i+0];
			oArray[j].item2=wArray[i+1];
			oArray[j].item3=wArray[i+2];
			j++;
		}
	}

	return(oArray);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: outputMatArrays()
// 
// Description:
//	This function outputs the passed in data for a specific material as a C data array to the
//	Dat file. If matCount equals zero default values for this material are output; otherwise
//	the values in the array pointed to by matArrayPtr are output. Data from matArrayPtr is
//	zero padded up to maxMatCount values (provided matCount<maxMatCount) while data from
//	defaultMat is repeated up to maxMatCount elements.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE outputMatArrays(COLOR *matArrayPtr, long matCount, long maxMatCount,
					 double *transpArrayPtr, COLOR *defaultMat, char *varName,
					 FILE *outFiles[])
{
	static int i, j;
	static int twoColorLineCount, oneColorLineCount;

	if(fprintf(hdrFile, "extern GLfloat %s[];\n", varName)<0)
		return(GENHDRWRITEERR);

	if(fprintf(datFile, "GLfloat %s[]=\n{\n", varName)<0)
		return(GENDATWRITEERR);
	
	if(!matCount) // no data was found for this material, use its default
	{
		oneColorLineCount=maxMatCount%2;
		twoColorLineCount=(maxMatCount-oneColorLineCount)/2;
		if(oneColorLineCount)
		{
			for(i=0; i<twoColorLineCount; i++)
			{
				if(fprintf(datFile,
					"\t%5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f,\n",
					defaultMat->r, defaultMat->g, defaultMat->b, OPAQUEVAL-transpArrayPtr[i+0],
					defaultMat->r, defaultMat->g, defaultMat->b, OPAQUEVAL-transpArrayPtr[i+1])
					<0)
					return(GENDATWRITEERR);
			}

			if(fprintf(datFile,
				"\t%5.4f, %5.4f, %5.4f, %5.4f\n",
				defaultMat->r, defaultMat->g, defaultMat->b, OPAQUEVAL-transpArrayPtr[i+0])<0)
				return(GENDATWRITEERR);
		}
		else // since there is no one liner - leave no ',' at the end of last two liner
		{
			for(i=0; i<twoColorLineCount-1; i++)
			{
				if(fprintf(datFile,
					"\t%5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f,\n",
					defaultMat->r, defaultMat->g, defaultMat->b, OPAQUEVAL-transpArrayPtr[i+0],
					defaultMat->r, defaultMat->g, defaultMat->b, OPAQUEVAL-transpArrayPtr[i+1])
					<0)
					return(GENDATWRITEERR);
			}

			if(fprintf(datFile,
				"\t%5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f\n",
				defaultMat->r, defaultMat->g, defaultMat->b, OPAQUEVAL-transpArrayPtr[i+0],
				defaultMat->r, defaultMat->g, defaultMat->b, OPAQUEVAL-transpArrayPtr[i+1])<0)
				return(GENDATWRITEERR);
		}
	}
	else // print the data found for this material
	{
		oneColorLineCount=matCount%2;
		twoColorLineCount=(matCount-oneColorLineCount)/2;
		if(matCount==maxMatCount)
		{
			// no padding needs to be done if we get here
			if(oneColorLineCount)
			{
				for(i=j=0; i<twoColorLineCount; i++, j+=2)
				{
					if(fprintf(datFile,
						"\t%5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f,\n",
						matArrayPtr[j+0].r, matArrayPtr[j+0].g,matArrayPtr[j+0].b,
						OPAQUEVAL-transpArrayPtr[j+0],
						matArrayPtr[j+1].r, matArrayPtr[j+1].g,	matArrayPtr[j+1].b,
						OPAQUEVAL-transpArrayPtr[j+1])<0)
						return(GENDATWRITEERR);
				}

				if(fprintf(datFile,	"\t%5.4f, %5.4f, %5.4f, %5.4f\n",
					matArrayPtr[j+0].r, matArrayPtr[j+0].g, matArrayPtr[j+0].b,
					OPAQUEVAL-transpArrayPtr[j+0])<0)
					return(GENDATWRITEERR);
			}
			else  // since there is no one liner - leave no ',' at the end of last two liner
			{
				for(i=j=0; i<twoColorLineCount-1; i++, j+=2)
				{
					if(fprintf(datFile,
						"\t%5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f,\n",
						matArrayPtr[j+0].r, matArrayPtr[j+0].g,matArrayPtr[j+0].b,
						OPAQUEVAL-transpArrayPtr[j+0],
						matArrayPtr[j+1].r, matArrayPtr[j+1].g,matArrayPtr[j+1].b,
						OPAQUEVAL-transpArrayPtr[j+1])<0)
						return(GENDATWRITEERR);
				}

				if(fprintf(datFile,
					"\t%5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f\n",
					matArrayPtr[j+0].r, matArrayPtr[j+0].g, matArrayPtr[j+0].b,
					OPAQUEVAL-transpArrayPtr[j+0],
					matArrayPtr[j+1].r, matArrayPtr[j+1].g, matArrayPtr[j+1].b,
					OPAQUEVAL-transpArrayPtr[j+1])
					<0)
					return(GENDATWRITEERR);
			}
		}
		else  // print data found for this material and pad the rest with zeros
		{
			for(i=j=0; i<twoColorLineCount; i++, j+=2)
			{
				if(fprintf(datFile,
					"\t%5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f, %5.4f,\n",
					matArrayPtr[j+0].r, matArrayPtr[j+0].g,matArrayPtr[j+0].b,
					OPAQUEVAL-transpArrayPtr[j+0],
					matArrayPtr[j+1].r, matArrayPtr[j+1].g,matArrayPtr[j+1].b,
					OPAQUEVAL-transpArrayPtr[j+1])<0)
					return(GENDATWRITEERR);
			}

			if(oneColorLineCount)
			{
				// there is at least one field to pad if we get here so it can be hardcoded
				maxMatCount--;
				if(matCount<maxMatCount)
				{
					if(fprintf(datFile,	"\t%5.4f, %5.4f, %5.4f, %5.4f, " \
						"0.0000, 0.0000, 0.0000, %5.4f,\n",
						matArrayPtr[j+0].r, matArrayPtr[j+0].g, matArrayPtr[j+0].b,
						OPAQUEVAL-transpArrayPtr[j+0], OPAQUEVAL-transpArrayPtr[j+1])<0)
						return(GENDATWRITEERR);
				}
				else
				{
					if(fprintf(datFile,	"\t%5.4f, %5.4f, %5.4f, %5.4f, " \
						"0.0000, 0.0000, 0.0000, %5.4f\n",
						matArrayPtr[j+0].r, matArrayPtr[j+0].g, matArrayPtr[j+0].b,
						OPAQUEVAL-transpArrayPtr[j+0], OPAQUEVAL-transpArrayPtr[j+1])<0)
						return(GENDATWRITEERR);
				}
			}

			if(matCount<maxMatCount)
			{
				oneColorLineCount=(maxMatCount-matCount)%2;
				twoColorLineCount=(maxMatCount-matCount-oneColorLineCount)/2;
				if(oneColorLineCount)
				{
					for(i=0; i<twoColorLineCount; i++, j+=2)
					{
						if(fprintf(datFile,
							"\t0.0000, 0.0000, 0.0000, %%5.4f," \
							" 0.0000, 0.0000, 0.0000, %5.4f,\n",
							OPAQUEVAL-transpArrayPtr[j+0], OPAQUEVAL-transpArrayPtr[j+1])<0)
							return(GENDATWRITEERR);
					}
			
					if(fprintf(datFile, "\t0.0000, 0.0000, 0.0000, %5.4f\n",
						OPAQUEVAL-transpArrayPtr[j+0])<0)
						return(GENDATWRITEERR);
				}
				else // since there is no one liner - leave no ',' at the end of last two liner
				{		
					for(i=0; i<twoColorLineCount-1; i++, j+=2)
					{
						if(fprintf(datFile,
							"\t0.0000, 0.0000, 0.0000, %5.4f," \
							" 0.0000, 0.0000, 0.0000, %5.4f,\n",
							OPAQUEVAL-transpArrayPtr[j+0], OPAQUEVAL-transpArrayPtr[j+1])<0)
							return(GENDATWRITEERR);
					}

					if(fprintf(datFile,
							"\t0.0000, 0.0000, 0.0000, %5.4f," \
							" 0.0000, 0.0000, 0.0000, %5.4f\n",
							OPAQUEVAL-transpArrayPtr[j+0], OPAQUEVAL-transpArrayPtr[j+1])<0)
							return(GENDATWRITEERR);
				}
			}
		}
	}

	if(fprintf(datFile, "};\n")<0)
		return(GENDATWRITEERR);

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: getFirstNonBlankChar()
// 
// Description:
//	This function searches a passed in string for the first character not resembling a space
//	or TAB and returns this character.
/////////////////////////////////////////////////////////////////////////////////////////////
char getFirstNonBlankChar(char *line)
{
	static int i;

	for(i=0; line[i]!='\0'; i++)
		if(line[i]!='\t' && line[i]!=' ')
			break;

	return(line[i]);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: retrieveGEN3DBLUNITArray()
// 
// Description:
//	This function reads an array of doubles from the input file into memory. Initially
//	the array of doubles is read into a work array of pre-determined size. The data is then
//	reformated (each 3 doubles = 1 generic 3 double unit) and copied to a newly allocated
//	array of appropriate size.
//	If no memory allocation error occurs arrayPtr will point to the allocated and populated
//	array; otherwise it will equal NULL. The actual return value will reflect the outcome
//	status accordingly.
// Note: This function will abort with an error status if the data retrieved from the input
//		 file is about to exceede the capacity of the work array.
//		 The file pointer is expected to be positioned so that the first number found in
//		 the current or following lines represents the beginning of the array to retrieve.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE retrieveGEN3DBLUNITArray(char *line, long *numCount, double *wArray, long maxWArray,
							  GEN3DBLUNIT **arrayPtr, FILE *vFile)
{
	static BYTE ret;
	static SBYTE tmpVal;
	static char chr;

	*numCount=0;

	// advance file pointer to the first line containing numbers
	if((ret=readToBegOfNum(line, vFile))!=SUCCESS)
	{
		if(ret==NOERR)
		{
			fprintf(stderr, "Expected array of numbers after specifier\n");
			return(SYNTAXERR);
		}
		else
			return(ret);
	}

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("\tFound the beginning of array\n");
#endif _V_DEBUGMODE_

	// read each line until the end of the number array
	do
	{
		// make sure the internal work array will not overflow
		if(*numCount+MAXEXPNUMSPLINE>=maxWArray)
		{
			fprintf(stderr, "Too many numbers for internal work array (so far: %ld)\n",
				*numCount);
			return(OUTOFWARRAYERR);
		}
		// retrieve all doubles from this line and add them to the work array
		else if((tmpVal=retrieveDoubles(line, wArray+(*numCount)))<=0)
		{
			tmpVal*=-1;
			*numCount+=tmpVal;
			break;
		}
		else
			*numCount+=tmpVal;
	} while(spFgets(line, MAXLINE, vFile)!=NULL &&
		(chr=getFirstNonBlankChar(line))!='a' && chr!='d' && chr!='s' && chr!='t' && chr!='e');

	if(ferror(vFile))
		return(GENINPREADERR);

	// copy the read in data in doubles from the work array to a newly allocated
	// generic 3 double unit array
	if((*arrayPtr=copyWArrayToOwnGEN3DBLUNITArray(wArray, *numCount))
		==NULL)
		return(GENALLOCERR);

	// calculate the amount of generic 3 double units retrieved
	*numCount/=3;
	
	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: retrieveGEN3DBLUNIT()
// 
// Description:
//	This function retrieves the first 3 doubles from a passed in line and stores them as
//	a GEN3DBLUNIT in threeDoubleUnit.
//	If this line conains absolutely no doubles NOERR is returned; if it does contain at least
//	three doubles SUCCESS is returned otherwise if there is more than 0 and less than 3
//	doubles SYNTAXERR is returned.
// Note: linePtr is keeping track of where the next GEN3DBLUNIT can be found in the current
//		 line (if there is a next one). Upon first entry of this function it should either
//		 point to the beginning of line or if the beginning of line is desired to be skipped
//		 it should point to line+x. After initialization and during repeated calling of this
//		 function (eg. in a loop) linePtr should not be modified outside of this function.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE retrieveGEN3DBLUNIT(char *line, char **linePtr, GEN3DBLUNIT *threeDoubleUnit, FILE *vFile)
{
	static BYTE ret;
	static BYTE nothingFound;

	nothingFound=V_TRUE;
		
	if((ret=getDouble(line, linePtr, vFile, &(threeDoubleUnit->item1)))==SUCCESS)
	{
		nothingFound=V_FALSE;
		if((ret=getDouble(line, linePtr, vFile, &(threeDoubleUnit->item2)))==SUCCESS)
			ret=getDouble(line, linePtr, vFile, &(threeDoubleUnit->item3));
	}

	if(nothingFound==V_FALSE && ret==NOERR)
	{
		fprintf(stderr, "Missing at least one component of 3 double unit\n");
		return(SYNTAXERR);
	}

	return(ret);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: retrieveGEN4DBLUNIT()
// 
// Description:
//	This function retrieves the first 4 doubles from a passed in line and stores them as
//	a GEN4DBLUNIT in fourDoubleUnit (usually representing a COLOR of R,G,B and A components).
//	If this line conains absolutely no doubles NOERR is returned; if it does contain at least
//	three doubles SUCCESS is returned otherwise if there is more than 0 and less than 4
//	doubles SYNTAXERR is returned.
// Note: linePtr is keeping track of where the next GEN4DBLUNIT can be found in the current
//		 line (if there is a next one). Upon first entry of this function it should either
//		 point to the beginning of line or if the beginning of line is desired to be skipped
//		 it should point to line+x. After initialization and during repeated calling of this
//		 function (eg. in a loop) linePtr should not be modified outside of this function.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE retrieveGEN4DBLUNIT(char *line, char **linePtr, GEN4DBLUNIT *fourDoubleUnit, FILE *vFile)
{
	static BYTE ret;

	if((ret=getDouble(line, linePtr, vFile, &(fourDoubleUnit->item1)))==SUCCESS)
		if((ret=getDouble(line, linePtr, vFile, &(fourDoubleUnit->item2)))==SUCCESS)
			if((ret=getDouble(line, linePtr, vFile, &(fourDoubleUnit->item3)))==SUCCESS)
				ret=getDouble(line, linePtr, vFile, &(fourDoubleUnit->item4));

	if(ret==NOERR)
	{
		fprintf(stderr, "Missing at least one component of 4 double unit\n");
		return(SYNTAXERR);
	}

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: retrieveCoordArray()
// 
// Description:
//	This function is used either to retrieve a Coordinate3 or a Normal array. It traverses
//	through all lines (from the current file pointer position) starting with the first line
//	that contains a number to the line containing a non delimiter character (which is assumed
//	to denote the end of the array) retrieving all the doubles each line contains and storing
//	them in a temporary work array. When all doubles have been read the array is copied
//	to a newly allocated GEN3DBLUNIT array (which can be casted to a *VERTEX) of the required
//	size.
// Note: This function will abort with an error status if the data retrieved from the input
//		 file is about to exceede the capacity of the work array.
//		 The file pointer is expected to be positioned so that the first number found in
//		 the current or following lines represents the beginning of the array to retrieve.
//		 cntrMode should be set to V_FALSE if subtracting a center is not desired (eg. for
//		 retrieval of Normal arrays)
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE retrieveCoordArray(char *line, long *numCount, double *wArray, long maxWArray,
				   VERTEX **arrayPtr, FILE *vFile, BYTE cntrMode)
{
	static BYTE ret;
	static SBYTE tmpVal;

	*numCount=0;

	// advance file pointer to the first line containing a number
	if((ret=readToBegOfNum(line, vFile))!=SUCCESS)
	{
		if(ret==NOERR)
		{
			fprintf(stderr, "Expected array of numbers after Coordinate3/Normal specifier\n");
			return(SYNTAXERR);
		}
		else
			return(ret);
	}

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("\tFound the beginning of array\n");
#endif _V_DEBUGMODE_

	// read each line in the coordinate array
	do
	{
		// make sure internal work array is not about to overflow
		if(*numCount+MAXEXPNUMSPLINE>=maxWArray)
		{
			fprintf(stderr, "Too many coordinates for internal work array (so far: %ld)\n",
				*numCount);
			return(OUTOFWARRAYERR);
		} // retrieve all doubles from the current line directly into the work array
		else if((tmpVal=retrieveDoubles(line, wArray+(*numCount)))<=0)
		{
			// a negative return value means that (return value*(-1)) doubles were found
			// and that the end of the array was found so terminate this loop
			tmpVal*=-1;
			*numCount+=tmpVal;	// account for the amount of doubles added to the work array
			break;
		}
		else
			*numCount+=tmpVal;	// account for the amount of doubles added to the work array
	} while(spFgets(line, MAXLINE, vFile)!=NULL);

	if(ferror(vFile))
		return(GENINPREADERR);
	
	// copy the data from the work array to a memory allocated array of exact size
	if(cntrMode)	// cntrMode should be V_FALSE if this function was called for normals
		*arrayPtr=(VERTEX*)centerCopyWArrayToOwnGEN3DBLUNITArray(wArray, *numCount, objCenter);
	else
		*arrayPtr=(VERTEX*)copyWArrayToOwnGEN3DBLUNITArray(wArray, *numCount);

	if(*arrayPtr==NULL)
		return(GENALLOCERR);
	
	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: getDouble()
// 
// Description:
//	This function searches from the current line to the next one in the input file that
//	contains a number. It then retrieves this number and converts it to a double.
//	If a close bracket is found prior to finding a number the function is aborted and returns
//	NOERR. If the file ends prior to finding a number or in case of a read error an error
//	value is returned. If however none of the above occurs and a double	was found SUCCESS is
//	returned.
// Note: linePtr is keeping track of where the next double can be found in the current
//		 line (if there is a next one). Upon first entry of this function it should either
//		 point to the beginning of line or if the beginning of line is desired to be skipped
//		 it should point to line+x. After initialization and during repeated calling of this
//		 function (eg. in a loop) linePtr should not be modified outside of this function.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE getDouble(char *line, char **linePtr, FILE *vFile, double *number)
{		
	static int len;
	static char *ptr, chr;
	
	// retrieve the next number from this line or read the next line if the current line
	// does not conain any numbers
	while((ptr=findBegOfNum(*linePtr))==NULL)
	{
		// could not find a number in this line, check if it contains a close bracket ( ] )
		if(strchr(line, CLOSEBRACKET)!=NULL)
			return(NOERR);
		else if(spFgets(line, MAXLINE, vFile)==NULL)
		{
			if(ferror(vFile))
				return(GENINPREADERR);
			else if(feof(vFile))
				return(GENUNEXPEOFERR);
		}

		*linePtr=line;
	}

	// retrieve the first number found and convert it to a double
	len=getNumLength(ptr);
	chr=ptr[len];
	ptr[len]='\0';
	*number=atof(ptr);
	ptr[len]=chr;
	*linePtr=ptr+len;
	
	return(SUCCESS);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: getLong()
// 
// Description:
//	This function searches from the current line to the next one in the input file that
//	contains a number. It then retrieves this number and converts it to a long.
//	If a close bracket is found prior to finding a number the function is aborted and returns
//	NOERR. If the file ends prior to finding a number or in case of a read error an error
//	value is returned. If however none of the above occurs and a long was found SUCCESS is
//	returned.
// Note: linePtr is keeping track of where the next long can be found in the current
//		 line (if there is a next one). Upon first entry of this function it should either
//		 point to the beginning of line or if the beginning of line is desired to be skipped
//		 it should point to line+x. After initialization and during repeated calling of this
//		 function (eg. in a loop) linePtr should not be modified outside of this function.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE getLong(char *line, char **linePtr, FILE *vFile, long *number)
{		
	static int len;
	static char *ptr, chr;
	
	// retrieve the next number from this line or read the next line if the current line
	// does not conain any numbers
	while((ptr=findBegOfNum(*linePtr))==NULL)
	{
		// could not find a number in this line, check if it contains a close bracket ( ] )
		if(strchr(line, CLOSEBRACKET)!=NULL)
			return(NOERR);
		else if(spFgets(line, MAXLINE, vFile)==NULL)
		{
			if(ferror(vFile))
				return(GENINPREADERR);
			else if(feof(vFile))
				return(GENUNEXPEOFERR);
		}

		*linePtr=line;
	}

	// retrieve the next number found and convert it to a long
	len=getNumLength(ptr);
	chr=ptr[len];
	ptr[len]='\0';
	*number=atol(ptr);
	ptr[len]=chr;
	*linePtr=ptr+len;
	
	return(SUCCESS);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: getFaceIndecis()
// 
// Description:
//	This function retrieves a set of index values representing the indecis of a polygon from
//	an index array (coordIndex, normalIndex or materialIndex). Each set of indecis for a
//	polygon in the array is expected to be terminated by a -1.
//	When a -1 is found the function ensures that the amount of indecis found prior to the
//	terminator is of a certain minimum. Otherwise the function skips this face and continues
//	reading the indecis for the next face until a set of indecis has been found that conains
//	the minimum amount of entries. If a close bracket is found prior to reading all the
//	indecis of a polygon the function is aborted and returns NOERR. If the file ends prior to
//	finding a complete set of polygon indecis or a read error occurs or if a larger number
//	of indecids were found than acceptable (>=MAXIDXARRAY) an error value is returned.
//	If however none of the above occurs and the complete set of indecis describing a polygon
//	was found SUCCESS is returned.
// Note: linePtr is keeping track of where the next index value can be found in the current
//		 line (if there is a next one). Upon first entry of this function it should either
//		 point to the beginning of line or if the beginning of line is desired to be skipped
//		 it should point to line+x. After initialization and during repeated calling of this
//		 function (eg. in a loop) linePtr should not be modified outside of this function.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE getFaceIndecis(char *line, char **linePtr, FILE *vFile, long *idxArray, int *idxCount,
					BYTE setType)
{		
	static int len;
	static char *ptr, chr;

	*idxCount=0;

	do
	{
		// retrieve the next number from this line or read the next line if the current line
		// is exhausted or does not conain any numbers
		while((ptr=findBegOfNum(*linePtr))==NULL)
		{
			// could not find anymore numbers in this line, check if it contains a
			// close bracket ( ] )
			if(strchr(line, CLOSEBRACKET)!=NULL)
				return(NOERR);
			else if(spFgets(line, MAXLINE, vFile)==NULL)
			{
				if(ferror(vFile))
					return(GENINPREADERR);
				else if(feof(vFile))
					return(GENUNEXPEOFERR);
			}

			*linePtr=line;
		}

		// retieve the next number from this line
		len=getNumLength(ptr);
		chr=ptr[len];
		ptr[len]='\0';
		idxArray[*idxCount]=atol(ptr);
		ptr[len]=chr;
		*linePtr=ptr+len;

		// if the number retrieved is a -1 make sure that depending on the set type the
		// correct minimum amount of numbers has been found otherwise skip the indecis for
		// this incomplete face and continue reading the next set of numbers up the next -1
		if(idxArray[*idxCount]==-1)
		{
			if(setType==LINESET)
			{
				if(*idxCount>=2)
					return(SUCCESS);
			}
			else
				if(*idxCount>=3)
					return(SUCCESS);

			// if this face consists of less than 3 (or 2) vertecis ignore it and continue
			// to the next one
			*idxCount=-1;
		}		
	} while(++(*idxCount)<MAXIDXARRAY);

	fprintf(stderr, "Face vertex count exceeds %d\n", MAXIDXARRAY);
	return(SYNTAXERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: processCoordIndecis()
// 
// Description:
//	This function is called when only a coordIndex array within an IndexedFaceSet node was
//	found. It reads the indecis for each polygon, uses those indecis to dereference the
//	correct coordinates in the previously found Coordinate3 array, calculates the normal
//	vector for each face and outputs the appropriate OpenGL commands for drawing the polygons.
// Note: Prior to dereferencing each coordinate it is ensured that the index does not refer
//		 to a non existant coordinate. If it does an error message is output and the
//		 function aborts with an error status. The function also aborts with an error status
//		 if a read or write error occurs. Otherwise if the entire index array was read
//		 gracefully NOERR is returned.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE processCoordIndecis(char *line, INDENTATION *iArray, int iIdx, FILE *vFile,
						 FILE *oglOutFile, BYTE setType)
{
	static BYTE ret;
	static char *linePtr;
	static int i, idxCount, curMatArrayNum;
	static long idxArray[MAXIDXARRAY];
	static VERTEX normal;
	static int mostRecentFace, curFace;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("\tProcessing coordindate indecis\n");
#endif _V_DEBUGMODE_

	mostRecentFace=-1;
	curMatArrayNum=iArray->matArrayNum;
	linePtr=line;

	// output OpenGL material commands determining the material for all of the following
	// polygons
	if(iArray->matCount!=0 && fprintf(oglOutFile,
		"%sglMaterialf(_MAT_FACE, GL_SHININESS, %5.4f);\n" \
		"%sglMaterialfv(_MAT_FACE, GL_AMBIENT, ambMat%s%4.4d);\n" \
		"%sglMaterialfv(_MAT_FACE, GL_DIFFUSE, diffMat%s%4.4d);\n" \
		"%sglMaterialfv(_MAT_FACE, GL_SPECULAR, specMat%s%4.4d);\n" \
		"%sglMaterialfv(_MAT_FACE, GL_EMISSION, emissMat%s%4.4d);\n",
		filler[iIdx+0], iArray->shininess,
		filler[iIdx+0], baseName, curMatArrayNum,
		filler[iIdx+0], baseName, curMatArrayNum,
		filler[iIdx+0], baseName, curMatArrayNum,
		filler[iIdx+0], baseName, curMatArrayNum)<0)
		return(GENOUTPWRITEERR);

	// process each face defined by the coordIndex array
	do
	{
		if((ret=getFaceIndecis(line, &linePtr, vFile, idxArray, &idxCount, setType))!=SUCCESS)
		{
			if(ret==NOERR)
				break;

			return(ret);
		}
		else
		{
			if(setType==LINESET)
			{
				if(mostRecentFace!=-1 && fprintf(oglOutFile, "%sglEnd();\n", filler[iIdx+0])<0)
					return(GENOUTPWRITEERR);
				if(fprintf(oglOutFile, "%sglBegin(GL_LINE_STRIP);\n", filler[iIdx+0])<0)
					return(GENOUTPWRITEERR);
				// since there is no normal array output a default normal
				if(fprintf(oglOutFile, "%sglNormal3f(0.0, 0.0, 0.0);\n", filler[iIdx+1])<0)
					return(GENOUTPWRITEERR);
				mostRecentFace=-2;	// just set it to any other value but -1
			}
			else
			{
				// determine the face type (..., 3 indecis = traingle, 4 indecis = quads,
				// index count>MAXFACE = polygon)
				curFace=idxCount>MAXFACE? MAXFACE : idxCount;

				// see if the amount of vertecis for this polygon has changed in which case
				// it might represent a different type of polygon (eg. triangle)
				// if so output the appropriate glBegin() command
				// if this is the first polygon output a glBegin in any case
				if(mostRecentFace!=curFace)
				{
					// if this is the first polygon do not output a glEnd() since there is
					// no previous glBegin() to terminate
					if(mostRecentFace==-1)
					{
						if(fprintf(oglOutFile, "%sglBegin(%s);\n",
							filler[iIdx+0], faceType[curFace])<0)
							return(GENOUTPWRITEERR);
					}
					else
					{
						if(fprintf(oglOutFile, "%sglEnd();\n%sglBegin(%s);\n",
							filler[iIdx+0], filler[iIdx+0], faceType[curFace])<0)
							return(GENOUTPWRITEERR);
					}
					mostRecentFace=curFace==MAXFACE? -2 : curFace;
				}

				// since there is no normal array calculate and output normal per face
				normal=calculateNormal(iArray->coordArray+idxArray[0],
					iArray->coordArray+idxArray[1], iArray->coordArray+idxArray[2]);
				if((iArray->vertexOrder==CCWORDER && !reverseNormals) ||
					(iArray->vertexOrder==CWORDER && reverseNormals))
				{
					if(fprintf(oglOutFile, "%sglNormal3f(%5.4f, %5.4f, %5.4f);\n",
						filler[iIdx+1], normal.x, normal.y, normal.z)<0)
						return(GENOUTPWRITEERR);
				}
				else
				{
					if(fprintf(oglOutFile, "%sglNormal3f(%5.4f, %5.4f, %5.4f);\n",
						filler[iIdx+1], -normal.x, -normal.y, -normal.z)<0)
						return(GENOUTPWRITEERR);
				}
			}

			// output all vertecis for this polygon
			for(i=0; i<idxCount; i++)
			{
				// make sure each index references an existing coordinate
				if(idxArray[i]>=iArray->coordCount)
				{
					fprintf(stderr, "Vertex index references non-existant vertex (%d>=%d)\n",
						idxArray[i], iArray->coordCount);
					return(SYNTAXERR);
				}
				else if(fprintf(oglOutFile, "%sglVertex3f(%5.4f, %5.4f, %5.4f);\n",
					filler[iIdx+1],
					iArray->coordArray[idxArray[i]].x, iArray->coordArray[idxArray[i]].y,
					iArray->coordArray[idxArray[i]].z)<0)
					return(GENOUTPWRITEERR);
			}
		}
	} while(V_TRUE);

	// output a final glEnd() for the previous glBegin()
	if(fprintf(oglOutFile, "%sglEnd();\n", filler[iIdx+0])<0)
		return(GENOUTPWRITEERR);

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: processCoordColIndecis()
// 
// Description:
//	This function is called when a coordIndex and materialIndex array within an
//	IndexedFaceSet node were found. It reads the indecis for each polygon, uses those indecis
//	to dereference the correct coordinates/material values in the previously found Coordinate3/
//	Material arrays, calculates the normal vector for each face and outputs the appropriate
//	OpenGL commands for drawing the polygons.
// Note: Prior to dereferencing each coordinate/material it is ensured that the index does not
//		 refer to a non existant coordinate/material. If it does an error message is output
//		 and the function aborts with an error status. The function also aborts with an error
//		 status if a read or write error occurs. Otherwise if the entire index array was read
//		 gracefully NOERR is returned.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE processCoordColIndecis(char *line, char *line1, INDENTATION *iArray, int iIdx,
							FILE *vFile[], FILE *oglOutFile, BYTE setType)
{
	static BYTE ret;
	static char *linePtr[2];
	static int i, idxCount[2], curMatArrayNum;
	static long idxArray[3][MAXIDXARRAY], matIndex;
	static VERTEX normal;
	static int mostRecentFace, curFace;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("\tProcessing coordinate and material indecis\n");
#endif _V_DEBUGMODE_

	mostRecentFace=-1;
	curMatArrayNum=iArray->matArrayNum;
	linePtr[0]=line;
	linePtr[1]=line1;

	if(fprintf(oglOutFile, "%sglMaterialf(_MAT_FACE, GL_SHININESS, %5.4f);\n",
		filler[iIdx+0], iArray->shininess)<0)
		return(GENOUTPWRITEERR);

	// process each face defined by the index arrays
	do
	{
		if((ret=getFaceIndecis(line, linePtr+0, vFile[0], idxArray[0], idxCount+0, setType))
			!=SUCCESS)
		{			
			if(ret==NOERR)
				break;

			return(ret);
		}
		else if((ret=getLong(line1, linePtr+1, vFile[1], &matIndex))!=SUCCESS)
		{
			if(ret==NOERR)
				break;

			return(ret);
		}
		else
		{
			if(setType==LINESET)
			{
				if(mostRecentFace!=-1 && fprintf(oglOutFile, "%sglEnd();\n", filler[iIdx+0])<0)
					return(GENOUTPWRITEERR);
				if(fprintf(oglOutFile, "%sglBegin(GL_LINE_STRIP);\n", filler[iIdx+0])<0)
					return(GENOUTPWRITEERR);
				// since there is no normal array output a default normal
				if(fprintf(oglOutFile, "%sglNormal3f(0.0, 0.0, 0.0);\n", filler[iIdx+1])<0)
					return(GENOUTPWRITEERR);
				mostRecentFace=-2;	// just set it to any other value but -1
			}
			else
			{
				// determine the face type (..., 3 indecis=traingle, 4 indecis=quads,
				// index count>MAXFACE = polygon)
				curFace=idxCount[0]>MAXFACE? MAXFACE : idxCount[0];

				// see if the amount of vertecis for this polygon has changed in which case
				// it might represent a different type of polygon (eg. triangle)
				// if so output the appropriate glBegin() command
				// if this is the first polygon output a glBegin in any case
				if(mostRecentFace!=curFace)
				{
					// if this is the first polygon do not output a glEnd() since there is
					// no previous glBegin() to terminate
					if(mostRecentFace==-1)
					{
						if(fprintf(oglOutFile, "%sglBegin(%s);\n",
							filler[iIdx+0], faceType[curFace])<0)
							return(GENOUTPWRITEERR);
					}
					else
					{
						if(fprintf(oglOutFile, "%sglEnd();\n%sglBegin(%s);\n",
							filler[iIdx+0], filler[iIdx+0], faceType[curFace])<0)
							return(GENOUTPWRITEERR);
					}
					mostRecentFace=curFace==MAXFACE? -2 : curFace;
				}
				
				// since there is no normal array calculate and output normal per face
				normal=calculateNormal(iArray->coordArray+idxArray[0][0],
					iArray->coordArray+idxArray[0][1], iArray->coordArray+idxArray[0][2]);
				if((iArray->vertexOrder==CCWORDER && !reverseNormals) ||
					(iArray->vertexOrder==CWORDER && reverseNormals))
				{
					if(fprintf(oglOutFile, "%sglNormal3f(%5.4f, %5.4f, %5.4f);\n",
						filler[iIdx+1], normal.x, normal.y, normal.z)<0)
						return(GENOUTPWRITEERR);
				}
				else
				{
					if(fprintf(oglOutFile, "%sglNormal3f(%5.4f, %5.4f, %5.4f);\n",
						filler[iIdx+1], -normal.x, -normal.y, -normal.z)<0)
						return(GENOUTPWRITEERR);
				}
			}

			// output all vertecis and their materials for this polygon
			for(i=0; i<idxCount[0]; i++)
			{
				// make sure each index references an existing coordinate/material
				if(idxArray[0][i]>=iArray->coordCount)
				{
					fprintf(stderr, "Vertex index references non-existant vertex (%d>=%d)\n",
						idxArray[0][i], iArray->coordCount);
					return(SYNTAXERR);
				}
				else if(matIndex>=iArray->matCount)
				{
					fprintf(stderr, "Material index references non-existant material" \
						" (%d>=%d)\n", matIndex, iArray->matCount);
					return(SYNTAXERR);
				}
				else if(fprintf(oglOutFile,
					"%sglMaterialfv(_MAT_FACE, GL_AMBIENT, ambMat%s%4.4d+%ld);\n" \
					"%sglMaterialfv(_MAT_FACE, GL_DIFFUSE, diffMat%s%4.4d+%ld);\n" \
					"%sglMaterialfv(_MAT_FACE, GL_SPECULAR, specMat%s%4.4d+%ld);\n" \
					"%sglMaterialfv(_MAT_FACE, GL_EMISSION, emissMat%s%4.4d+%ld);\n",
					filler[iIdx+1], baseName, curMatArrayNum, matIndex*MATCOLCOMPCOUNT,
					filler[iIdx+1], baseName, curMatArrayNum, matIndex*MATCOLCOMPCOUNT,
					filler[iIdx+1], baseName, curMatArrayNum, matIndex*MATCOLCOMPCOUNT,
					filler[iIdx+1], baseName, curMatArrayNum, matIndex*MATCOLCOMPCOUNT)<0)
					return(GENOUTPWRITEERR);
				else if(fprintf(oglOutFile, "%sglVertex3f(%5.4f, %5.4f, %5.4f);\n",
					filler[iIdx+1],
					iArray->coordArray[idxArray[0][i]].x,
					iArray->coordArray[idxArray[0][i]].y,
					iArray->coordArray[idxArray[0][i]].z)<0)
					return(GENOUTPWRITEERR);
			}
		}
	} while(V_TRUE);

	// output a final glEnd() for the previous glBegin()
	if(fprintf(oglOutFile, "%sglEnd();\n", filler[iIdx+0])<0)
		return(GENOUTPWRITEERR);

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: processCoordNormIndecis()
// 
// Description:
//	This function is called when a coordIndex and normalIndex array within an
//	IndexedFaceSet node were found. It reads the indecis for each polygon, uses those indecis
//	to dereference the correct coordinates/normals in the previously found Coordinate3/
//	Normal arrays and outputs the appropriate OpenGL commands for drawing the polygons.
// Note: Prior to dereferencing each coordinate/normal it is ensured that the index does not
//		 refer to a non existant coordinate/normal. If it does an error message is output
//		 and the function aborts with an error status. The function also aborts with an error
//		 status if a read or write error occurs. Otherwise if the entire index array was read
//		 gracefully NOERR is returned.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE processCoordNormIndecis(char *line, char *line2, INDENTATION *iArray, int iIdx,
							 FILE *vFile[], FILE *oglOutFile, BYTE setType)
{
	static BYTE ret;
	static char *linePtr[3];
	static int i, idxCount[3], curMatArrayNum;
	static long idxArray[3][MAXIDXARRAY];
	static int mostRecentFace, curFace;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("\tProcessing coordinate and normal indecis\n");
#endif _V_DEBUGMODE_

	mostRecentFace=-1;
	curMatArrayNum=iArray->matArrayNum;
	linePtr[0]=line;	
	linePtr[2]=line2;

	if(iArray->matCount!=0 && fprintf(oglOutFile,
		"%sglMaterialf(_MAT_FACE, GL_SHININESS, %5.4f);\n" \
		"%sglMaterialfv(_MAT_FACE, GL_AMBIENT, ambMat%s%4.4d);\n" \
		"%sglMaterialfv(_MAT_FACE, GL_DIFFUSE, diffMat%s%4.4d);\n" \
		"%sglMaterialfv(_MAT_FACE, GL_SPECULAR, specMat%s%4.4d);\n" \
		"%sglMaterialfv(_MAT_FACE, GL_EMISSION, emissMat%s%4.4d);\n",
		filler[iIdx+0], iArray->shininess,
		filler[iIdx+0], baseName, curMatArrayNum,
		filler[iIdx+0], baseName, curMatArrayNum,
		filler[iIdx+0], baseName, curMatArrayNum,
		filler[iIdx+0], baseName, curMatArrayNum)<0)
		return(GENOUTPWRITEERR);

	do
	{
		if((ret=getFaceIndecis(line, linePtr+0, vFile[0], idxArray[0], idxCount+0, setType))
			!=SUCCESS)
		{			
			if(ret==NOERR)
				break;

			return(ret);
		}		
		else if((ret=getFaceIndecis(line2, linePtr+2, vFile[2], idxArray[2], idxCount+2,
			setType))!=SUCCESS)
		{		
			if(ret==NOERR)
				break;

			return(ret);
		}		
		else if(idxCount[0]!=idxCount[2])
		{
			fprintf(stderr, "The amount of vertecis for this face does not match the amount" \
				" of normals\n");
			return(SYNTAXERR);
		}
		else
		{
			if(setType==LINESET)
			{
				// if this is the first polygon do not output a glEnd() since there is
					// no previous glBegin() to terminate
				if(mostRecentFace!=-1 && fprintf(oglOutFile, "%sglEnd();\n", filler[iIdx+0])<0)
					return(GENOUTPWRITEERR);
				if(fprintf(oglOutFile, "%sglBegin(GL_LINE_STRIP);\n", filler[iIdx+0])<0)
					return(GENOUTPWRITEERR);
				mostRecentFace=-2;	// just set it to any other value but -1
			}
			else
			{
				// determine the face type (..., 3 indecis=traingle, 4 indecis=quads,
				// index count>MAXFACE = polygon)
				curFace=idxCount[0]>MAXFACE? MAXFACE : idxCount[0];

				// see if the amount of vertecis for this polygon has changed in which case
				// it might represent a different type of polygon (eg. triangle)
				// if so output the appropriate glBegin() command
				// if this is the first polygon output a glBegin in any case
				if(mostRecentFace!=curFace)
				{
					if(mostRecentFace==-1)
					{
						if(fprintf(oglOutFile, "%sglBegin(%s);\n",
							filler[iIdx+0], faceType[curFace])<0)
							return(GENOUTPWRITEERR);
					}
					else
					{
						if(fprintf(oglOutFile, "%sglEnd();\n%sglBegin(%s);\n",
							filler[iIdx+0], filler[iIdx+0], faceType[curFace])<0)
							return(GENOUTPWRITEERR);
					}
					mostRecentFace=curFace==MAXFACE? -2 : curFace;
				}
			}

			// output all vertecis and their normals for this polygon
			for(i=0; i<idxCount[0]; i++)
			{
				// make sure each index references an existing coordinate/normal
				if(idxArray[0][i]>=iArray->coordCount)
				{
					fprintf(stderr, "Vertex index references non-existant vertex (%d>=%d)\n",
						idxArray[0][i], iArray->coordCount);
					return(SYNTAXERR);
				}
				else if(idxArray[2][i]>=iArray->normCount)
				{
					fprintf(stderr, "Normal index references non-existant normal (%d>=%d)\n",
						idxArray[2][i], iArray->normCount);
					return(SYNTAXERR);
				}
				else if(reverseNormals)
				{
					if(fprintf(oglOutFile, "%sglNormal3f(%5.4f, %5.4f, %5.4f);\n" \
						"%sglVertex3f(%5.4f, %5.4f, %5.4f);\n",
						filler[iIdx+1], -iArray->normArray[idxArray[2][i]].x,
						-iArray->normArray[idxArray[2][i]].y,
						-iArray->normArray[idxArray[2][i]].z,
						filler[iIdx+1],	iArray->coordArray[idxArray[0][i]].x,
						iArray->coordArray[idxArray[0][i]].y,
						iArray->coordArray[idxArray[0][i]].z)<0)
					return(GENOUTPWRITEERR);
				}
				else if(fprintf(oglOutFile, "%sglNormal3f(%5.4f, %5.4f, %5.4f);\n" \
					"%sglVertex3f(%5.4f, %5.4f, %5.4f);\n",
					filler[iIdx+1],
					iArray->normArray[idxArray[2][i]].x, iArray->normArray[idxArray[2][i]].y,
					iArray->normArray[idxArray[2][i]].z,
					filler[iIdx+1],
					iArray->coordArray[idxArray[0][i]].x, iArray->coordArray[idxArray[0][i]].y,
					iArray->coordArray[idxArray[0][i]].z)<0)
					return(GENOUTPWRITEERR);
			}
		}
	} while(V_TRUE);

	// output a final glEnd() for the previous glBegin()
	if(fprintf(oglOutFile, "%sglEnd();\n", filler[iIdx+0])<0)
		return(GENOUTPWRITEERR);

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: processCoordColNormIndecis()
// 
// Description:
//	This function is called when a coordIndex, normalIndex and materialIndex array within
//	an IndexedFaceSet node were found. It reads the indecis for each polygon, uses those
//	indecis to dereference the correct coordinates/normals/materials in the previously found
//	Coordinate3/Normal/Material arrays and outputs the appropriate OpenGL commands for drawing
//	the polygons.
// Note: Prior to dereferencing each coordinate/normal/material it is ensured that the index
//		 does not refer to a non existant coordinate/normal/material. If it does an error
//		 message is output and the function aborts with an error status. The function also
//		 aborts with an error status if a read or write error occurs. Otherwise if the entire
//		 index array was read gracefully NOERR is returned.
/////////////////////////////////////////////////////////////////////////////////////////////
BYTE processCoordColNormIndecis(char *line, char *line1, char *line2, INDENTATION *iArray,
								int iIdx, FILE *vFile[], FILE *oglOutFile, BYTE setType)
{
	static BYTE ret;
	static char *linePtr[3];
	static int i, idxCount[3], curMatArrayNum;
	static long idxArray[3][MAXIDXARRAY], matIndex;
	static int mostRecentFace, curFace;

#ifdef _V_DEBUGMODE_
	if(debugMode)
		printf("\tProcessing coordinate, material and normal indecis\n");
#endif _V_DEBUGMODE_

	mostRecentFace=-1;
	curMatArrayNum=iArray->matArrayNum;
	linePtr[0]=line;
	linePtr[1]=line1;
	linePtr[2]=line2;

	if(fprintf(oglOutFile, "%sglMaterialf(_MAT_FACE, GL_SHININESS, %5.4f);\n",
		filler[iIdx+0], iArray->shininess)<0)
		return(GENOUTPWRITEERR);

	do
	{
		if((ret=getFaceIndecis(line, linePtr+0, vFile[0], idxArray[0], idxCount+0, setType))
			!=SUCCESS)
		{			
			if(ret==NOERR)
				break;

			return(ret);
		}
		else if((ret=getLong(line1, linePtr+1, vFile[1], &matIndex))!=SUCCESS)
		{
			if(ret==NOERR)
				break;

			return(ret);
		}
		else if((ret=getFaceIndecis(line2, linePtr+2, vFile[2], idxArray[2], idxCount+2,
			setType))!=SUCCESS)
		{		
			if(ret==NOERR)
				break;

			return(ret);
		}		
		else if(idxCount[0]!=idxCount[2])
		{
			fprintf(stderr, "The amount of vertecis for this face does not match the amount" \
				" of normals\n");
			return(SYNTAXERR);
		}
		else
		{
			if(setType==LINESET)
			{
				if(mostRecentFace!=-1 && fprintf(oglOutFile, "%sglEnd();\n", filler[iIdx+0])<0)
					return(GENOUTPWRITEERR);
				if(fprintf(oglOutFile, "%sglBegin(GL_LINE_STRIP);\n", filler[iIdx+0])<0)
					return(GENOUTPWRITEERR);
				mostRecentFace=-2;	// just set it to any other value but -1
			}
			else
			{
				// determine the face type (..., 3 indecis=traingle, 4 indecis=quads,
				// index count>MAXFACE = polygon)
				curFace=idxCount[0]>MAXFACE? MAXFACE : idxCount[0];

				// see if the amount of vertecis for this polygon has changed in which case
				// it might represent a different type of polygon (eg. triangle)
				// if so output the appropriate glBegin() command
				// if this is the first polygon output a glBegin in any case
				if(mostRecentFace!=curFace)
				{
					// if this is the first polygon do not output a glEnd() since there is
					// no previous glBegin() to terminate
					if(mostRecentFace==-1)
					{
						if(fprintf(oglOutFile, "%sglBegin(%s);\n",
							filler[iIdx+0], faceType[curFace])<0)
							return(GENOUTPWRITEERR);
					}
					else
					{
						if(fprintf(oglOutFile, "%sglEnd();\n%sglBegin(%s);\n",
							filler[iIdx+0], filler[iIdx+0], faceType[curFace])<0)
							return(GENOUTPWRITEERR);
					}
					mostRecentFace=curFace==MAXFACE? -2 : curFace;
				}
			}

			// output all vertecis and their materials and normals for this polygon
			for(i=0; i<idxCount[0]; i++)
			{
				// make sure each index references an existing coordinate/material/normal
				if(idxArray[0][i]>=iArray->coordCount)
				{
					fprintf(stderr, "Vertex index references non-existant vertex (%d>=%d)\n",
						idxArray[0][i], iArray->coordCount);
					return(SYNTAXERR);
				}
				else if(idxArray[2][i]>=iArray->normCount)
				{
					fprintf(stderr, "Normal index references non-existant normal (%d>=%d)\n",
						idxArray[2][i], iArray->normCount);
					return(SYNTAXERR);
				}
				else if(matIndex>=iArray->matCount)
				{
					fprintf(stderr, "Material index references non-existant material" \
						" (%d>=%d)\n", matIndex, iArray->matCount);
					return(SYNTAXERR);
				}
				else if(fprintf(oglOutFile,
					"%sglMaterialfv(_MAT_FACE, GL_AMBIENT, ambMat%s%4.4d+%ld);\n" \
					"%sglMaterialfv(_MAT_FACE, GL_DIFFUSE, diffMat%s%4.4d+%ld);\n" \
					"%sglMaterialfv(_MAT_FACE, GL_SPECULAR, specMat%s%4.4d+%ld);\n" \
					"%sglMaterialfv(_MAT_FACE, GL_EMISSION, emissMat%s%4.4d+%ld);\n",
					filler[iIdx+1], baseName, curMatArrayNum, matIndex*MATCOLCOMPCOUNT,
					filler[iIdx+1], baseName, curMatArrayNum, matIndex*MATCOLCOMPCOUNT,
					filler[iIdx+1], baseName, curMatArrayNum, matIndex*MATCOLCOMPCOUNT,
					filler[iIdx+1], baseName, curMatArrayNum, matIndex*MATCOLCOMPCOUNT)<0)
					return(GENOUTPWRITEERR);
				else if(reverseNormals)
				{
					if(fprintf(oglOutFile, "%sglNormal3f(%5.4f, %5.4f, %5.4f);\n" \
						"%sglVertex3f(%5.4f, %5.4f, %5.4f);\n",
						filler[iIdx+1], -iArray->normArray[idxArray[2][i]].x,
						-iArray->normArray[idxArray[2][i]].y,
						-iArray->normArray[idxArray[2][i]].z,
						filler[iIdx+1], iArray->coordArray[idxArray[0][i]].x,
						iArray->coordArray[idxArray[0][i]].y,
						iArray->coordArray[idxArray[0][i]].z)<0)
					return(GENOUTPWRITEERR);
				}
				else if(fprintf(oglOutFile, "%sglNormal3f(%5.4f, %5.4f, %5.4f);\n" \
					"%sglVertex3f(%5.4f, %5.4f, %5.4f);\n",
					filler[iIdx+1],
					iArray->normArray[idxArray[2][i]].x, iArray->normArray[idxArray[2][i]].y,
					iArray->normArray[idxArray[2][i]].z,
					filler[iIdx+1],
					iArray->coordArray[idxArray[0][i]].x, iArray->coordArray[idxArray[0][i]].y,
					iArray->coordArray[idxArray[0][i]].z)<0)
					return(GENOUTPWRITEERR);
			}
		}
	} while(V_TRUE);

	// output a final glEnd() for the previous glBegin()
	if(fprintf(oglOutFile, "%sglEnd();\n", filler[iIdx+0])<0)
		return(GENOUTPWRITEERR);

	return(NOERR);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: deleteIndentArrays()
// 
// Description:
//	This function deletes (deallocates) all arrays (DEFList, coordArray & normArray if present)
//	attached to the indentation with the index *indentIdx. coordArray and normArray will
//	not be deleted if they have either not been found or have been inherited from a parent
//	indentation.
//	The function is called either when an indentation close is found (in which case all of its
//	definitions go out of scope) or when the program is aborted due to an error.
/////////////////////////////////////////////////////////////////////////////////////////////
void deleteIndentArrays(INDENTATION *indentArray, int indentIdx, char *debugMsg)
{
	static int i;

	// delete all DEFs for this indentation
	for(i=0; i<indentArray[indentIdx].DEFIdx; i++)
	{
		delete [] indentArray[indentIdx].DEFList[i].DEF;
#ifdef _V_DEBUGMODE_
		if(debugMode)
			printf("%s DEF[%d][%3.3d]!\n", debugMsg, indentIdx, i);
#endif _V_DEBUGMODE_
	}

	deleteCoordArray(indentArray, indentIdx, debugMsg);
	deleteNormArray(indentArray, indentIdx, debugMsg);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: deleteCoordArray()
// 
// Description:
//	This function deletes (deallocates) a Coordinate3 array from memory for the indicated
//	indentation. Nothing will be deleted if the array was inherited from the current
//	indentation's parent node or if no Coordinate3 array was attached to this indentation.
/////////////////////////////////////////////////////////////////////////////////////////////
void deleteCoordArray(INDENTATION *indentArray, int indentIdx, char *debugMsg)
{
	if(indentArray[indentIdx].coordArray!=indentArray[indentIdx-1].coordArray &&
		indentArray[indentIdx].coordArray!=NULL)
	{
		delete [] indentArray[indentIdx].coordArray;
		indentArray[indentIdx].coordCount=0;

#ifdef _V_DEBUGMODE_
		if(debugMode)
			printf("%s Coordinate3 array!\n", debugMsg);
#endif _V_DEBUGMODE_
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: deleteNormArray()
// 
// Description:
//	This function deletes (deallocates) a Normal array from memory for the indicated
//	indentation. Nothing will be deleted if the array was inherited from the current
//	indentation's parent node or if no Normal array was attached to this indentation.
/////////////////////////////////////////////////////////////////////////////////////////////
void deleteNormArray(INDENTATION *indentArray, int indentIdx, char *debugMsg)
{
	if(indentArray[indentIdx].normArray!=indentArray[indentIdx-1].normArray &&
		indentArray[indentIdx].normArray!=NULL)
	{
		delete [] indentArray[indentIdx].normArray;
		indentArray[indentIdx].normCount=0;

#ifdef _V_DEBUGMODE_
		if(debugMode)
			printf("%s Normal array!\n", debugMsg);
#endif _V_DEBUGMODE_
	}
}
