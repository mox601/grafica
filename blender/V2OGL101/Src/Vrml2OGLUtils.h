/********************************************************************************************
** Vrml2OGLUtils.h - Header file for Vrml2OGLUtils.cpp
** Copyright 1999 by Alexander Rohra. All rights reserved.
**
** Program Name:	Vrml2OGL.exe V1.0 -> V1.01
** Author:			Alexander Rohra
** Date:			May 1999
** Compiler:		MSVC++ V5.0	& others
** App Type:		Command Line Utility (Win32 Console Application)
** Revisions:		August 1999 - Alexander Rohra -> V1.01:
**					1)	Added the GLHDRINCSTATMENT #define containing the appropriate include
**						statement (that will be inserted in the output header file) for the
**						platform Vrml2OGL is being compiled on
** Code Desc.:		The definitions in this header file are primarily used by
**					code in Vrml2OGLUtils.cpp but might be used by other source
**					code modules including this file.
********************************************************************************************/

#ifndef _VRML2OGLUTILS_H_
#define _VRML2OGLUTILS_H_

#include <stdio.h>
#include "SharedDefs.h"				// needed for typedefs

#define MAXIDXARRAY			200
#define MAXHDRTMPSTR		256
#define MAXEXPNUMSPLINE		20
#define MAXFACE				5		// see faceType[]
#define MATCOLCOMPCOUNT		4
#define OPAQUEVAL			(double)1.0
#define BEGNUMCHARCOUNT		(sizeof(begNumChars)/sizeof(char))-1
#define NUMCHARCOUNT		(sizeof(numChars)/sizeof(char))-1
#define SYMBCHARCOUNT		3		// number of characters at the end of begNumChars that
									// cannot stand alone to represent a number

#ifdef _WIN32
	#define GLHDRINCSTATMENT "#include <gl\\gl.h>"
#else // UNIX
	#define GLHDRINCSTATMENT "#include <GL/gl.h>"
#endif

// *** function prototypes for global/public functions *** 
extern void deleteCoordArray(INDENTATION *indentArray, int indentIdx, char *debugMsg);
extern void deleteNormArray(INDENTATION *indentArray, int indentIdx, char *debugMsg);
extern void deleteIndentArrays(INDENTATION *indentArray, int indentIdx, char *debugMsg);
extern BYTE checkVersion(FILE *vFile);
extern BYTE findBoundingBox(FILE *vFile, VERTEX *boundingBox);
extern char *spFgets(char *string, int n, FILE *stream);
extern BYTE skipToCloseParentBrace(char *line, FILE *vFile, long initalBraceCount);
extern BYTE skipToCloseBrace(char *line, FILE *vFile);
extern char *findBegOfNum(char *line);
extern BYTE readToBegOfNum(char *line, FILE *vFile);
extern char getFirstNonBlankChar(char *line);
extern BYTE getNumLength(char *str);
extern BYTE getLong(char *line, char **linePtr, FILE *vFile, long *number);
extern BYTE getDouble(char *line, char **linePtr, FILE *vFile, double *number);
extern SBYTE retrieveDoubles(char *line, double *array);
extern GEN3DBLUNIT *copyWArrayToOwnGEN3DBLUNITArray(double *wArray, long count);
extern GEN3DBLUNIT *centerCopyWArrayToOwnGEN3DBLUNITArray(double *wArray, long count,
					VERTEX center);
extern BYTE retrieveDoubleArray(char *line, long *numCount, double *wArray, long maxWArray,
			FILE *vFile);
extern BYTE retrieveCoordArray(char *line, long *numCount, double *wArray, long maxWArray,
			VERTEX **arrayPtr, FILE *vFile, BYTE cntrMode);
extern BYTE retrieveGEN3DBLUNITArray(char *line, long *numCount, double *wArray,
			long maxWArray,	GEN3DBLUNIT **arrayPtr, FILE *vFile);
extern BYTE retrieveGEN3DBLUNIT(char *line, char **linePtr, GEN3DBLUNIT *threeDoubleUnit,
			FILE *vFile);
extern BYTE retrieveGEN4DBLUNIT(char *line, char **linePtr, GEN4DBLUNIT *fourDoubleUnit,
			FILE *vFile);
extern BYTE processCoordIndecis(char *line, INDENTATION *iArray, int iIdx, FILE *vFile,
			FILE *oglOutFile, BYTE setType);
extern BYTE processCoordColIndecis(char *line, char *line1, INDENTATION *iArray, int iIdx,
			FILE *vFile[], FILE *oglOutFile, BYTE setType);
extern BYTE processCoordNormIndecis(char *line, char *line2, INDENTATION *iArray, int iIdx,
			FILE *vFile[], FILE *oglOutFile, BYTE setType);
extern BYTE processCoordColNormIndecis(char *line, char *line1, char *line2,
			INDENTATION *iArray, int iIdx, FILE *vFile[], FILE *oglOutFile, BYTE setType);
extern BYTE outputMatArrays(COLOR *matArrayPtr, long matCount, long maxMatCount,
			double *transpArrayPtr, COLOR *defaultMat, char *varName, FILE *outFiles[]);
extern BYTE writeFileHeaders(FILE *outFiles[], HEADERFOOTERINFO *hfInfo);
extern BYTE writeFileFooters(FILE *outFiles[], HEADERFOOTERINFO *hfInfo);
char *getDEF(char *line);
extern BYTE addDefToList(INDENTATION *iArrayElement, int iIdx, char *DEF);

#endif	// _VRML2OGLUTILS_H_

