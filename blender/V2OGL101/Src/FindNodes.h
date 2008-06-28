/********************************************************************************************
** FindNodes.h - Header file for FindNodes.cpp
** Copyright 1999 by Alexander Rohra. All rights reserved.
**
** Program Name:	Vrml2OGL.exe V1.0 -> V1.01
** Author:			Alexander Rohra
** Date:			May 1999
** Compiler:		MSVC++ V5.0	& others
** App Type:		Command Line Utility (Win32 Console Application)
** Revisions:		August 1999 - Alexander Rohra, as suggested by Scott Nelson -> V1.01:
**					1)	Added conditional definition of SEEK_SET for Sun Solaris 1.x
**						compatibility.
** Code Desc.:		The definitions in this header file are primarily used by
**					code in FindNodes.cpp but might be used by other source
**					code modules including this file.
********************************************************************************************/

#ifndef _FINDNODES_H_
#define _FINDNODES_H_

#include <stdio.h>
#include "SharedDefs.h"		// needed for typedefs

#ifndef SEEK_SET			// for Sun Solaris 1.x, which defines this in unistd.h while on
#define SEEK_SET	0		// all other platforms, this is defined in stdio.h
#endif	// SEEK_SET

#define CWORDER				0			// corresponds to GL_CW, see vertexOrderType[]
#define CCWORDER			1			// corresponds to GL_CCW, see vertexOrderType[]
#define DEFAULTORDER		CCWORDER
#define DEFAULTAMBMAT		{0.2, 0.2, 0.2}
#define DEFAULTDIFFMAT		{0.8, 0.8, 0.8}
#define DEFAULTSPECMAT		{0.0, 0.0, 0.0}
#define DEFAULTEMISSMAT		{0.0, 0.0, 0.0}
#define SHININESSCONVMULTIP	128
#define DEFAULTSHININESS	0.2*SHININESSCONVMULTIP
#define DFLTVRMLTRANSP		0.0			// default VRML transparancy value (opaque)

#define MAXVARNAME			100
#define MAXWORKARRAY		100000	// size of temporary work array in # of items
#define MAXTRANSLARRAY		15
#define MAXROTARRAY			15
#define MAXCENTERARRAY		15
#define MAXSCALEFACTARRAY	15

// *** function prototypes for global/public functions ***
extern BYTE findNodes(FILE *vrmlFile[], FILE *outFiles[], NODE *nodeTypes);
extern BYTE handleSubIndent(INDENTATION *itArray, int *iIdx, BYTE iType, char *line, char *DEF,
			FILE *vFile[], FILE *outFiles[]);
extern BYTE handleIndent(INDENTATION *itArray, int *iIdx, BYTE iType, char *line, char *DEF,
			FILE *vFile[], FILE *outFiles[]);
extern BYTE handleIndentClose(INDENTATION *itArray, int *iIdx, BYTE iType, char *line,
			char *DEF, FILE *vFile[], FILE *outFiles[]);
extern BYTE handleCoordinate3(INDENTATION *itArray, int *iIdx, BYTE iType, char *line,
			char *DEF, FILE *vFile[], FILE *outFiles[]);
extern BYTE handleNormal(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
			FILE *vFile[], FILE *outFiles[]);
extern BYTE handleIndexedFLSet(INDENTATION *itArray, int *iIdx, BYTE iType, char *line,
			char *DEF, FILE *vFile[], FILE *outFiles[]);
extern BYTE handleShapeHints(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
			FILE *vFile[], FILE *outFiles[]);
extern BYTE handleMaterial(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
			FILE *vFile[], FILE *outFiles[]);
extern BYTE handleTransform(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
			FILE *vFile[], FILE *outFiles[]);
extern BYTE handleTranslation(INDENTATION *iArray, int *iIdx, BYTE iType, char *line,
			char *DEF, FILE *vFile[], FILE *outFiles[]);
extern BYTE handleMatrixTransform(INDENTATION *iArray, int *iIdx, BYTE iType, char *line,
			char *DEF, FILE *vFile[], FILE *outFiles[]);
extern BYTE handleRotation(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
			FILE *vFile[], FILE *outFiles[]);
extern BYTE handleScale(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
			FILE *vFile[], FILE *outFiles[]);
extern BYTE handleGenericNode(INDENTATION *itArray, int *iIdx, BYTE iType, char *line,
			char *DEF, FILE *vFile[], FILE *outFiles[]);
extern BYTE handleUnimplementedNode(INDENTATION *itArray, int *iIdx, BYTE iType, char *line,
			char *DEF, FILE *vFile[], FILE *outFiles[]);
extern BYTE handleInfo(INDENTATION *iArray, int *iIdx, BYTE iType, char *line, char *DEF,
			FILE *vFile[], FILE *outFiles[]);

#endif // _FINDNODES_H_
