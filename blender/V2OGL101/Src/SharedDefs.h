/********************************************************************************************
** SharedDefs.h - Independent header file
** Copyright 1999 by Alexander Rohra. All rights reserved.
**
** Program Name:	Vrml2OGL.exe V1.0 -> V1.01
** Author:			Alexander Rohra
** Date:			May 1999
** Compiler:		MSVC++ V5.0	& others
** App Type:		Command Line Utility (Win32 Console Application)
** Revisions:		August 1999 - Alexander Rohra, as suggested by Scott Nelson:
**					1)	Added #define for _MAX_FNAME for platforms that don't define it in
**						stdlib.h
** Code Desc.:		The code in this header file is not specific to any
**					source code module. It rather represents an accumulation of
**					defintions used by at least two source code modules but cannot
**					directly be associated with any of them.
********************************************************************************************/

#ifndef _SHAREDDEFS_H_
#define _SHAREDDEFS_H_

#include <stdlib.h>			// for _MAX_FNAME (see below)

#define V_TRUE				1
#define V_FALSE				0

#define oglFile				outFiles[0]
#define hdrFile				outFiles[1]
#define datFile				outFiles[2]

#define oglFName			outFNames[0]
#define hdrFName			outFNames[1]
#define datFName			outFNames[2]

#define READMODE			"rb"			// avoid carriage return/linefeed translation
#define WRITEMODE			"w"

#define NOTHEAD				-1

#define OPENBRACE			'{'
#define CLOSEBRACE			'}'
#define CLOSEBRACKET		']'
#define COMMENTCHAR			'#'
#define ACTIVELOD			1
#define MAXLINE				256
#define MAXINDENTS			100
#define MAXDEFSPERINDENT	50
#define MAXOUTFILES			3

#ifdef _MAX_FNAME
#define MAXBASENAME			_MAX_FNAME		// _MAX_FNAME is defined in stdlib.h (WIN32 only)
#else
#define MAXBASENAME			128				// UNIX
#endif	// _MAX_FNAME

#define MAXNAME				256

enum {NOERR=0, SUCCESS, OPENERR, SYNTAXERR, VERSIONERR, NESTEDLODERR, OUTOFWARRAYERR,
		GENINPREADERR, GENOUTPWRITEERR, GENDATWRITEERR,	GENHDRWRITEERR, GENALLOCERR,
		GENUNEXPEOFERR, GENFTELLERR, GENFSEEKERR};
enum {END=0, ANYTYPE, GENERIC, GENXFRM, FACESET, LINESET,
		// Note: the following enums all have to refer to some kind of indentation node type
		//		 enums refering to any other node type have to be <INDENTTYPESTART
		INDENTTYPESTART,
			SEPARATOR, GROUP, SWITCH, LOD, TRANSFORMSEPARATOR, INDENTCLOSE};

typedef unsigned char BYTE;
typedef char SBYTE;
typedef struct
{
	double item1;
	double item2;
	double item3;
} GEN3DBLUNIT;
typedef struct
{
	double x;
	double y;
	double z;
} VERTEX, VECTOR;
typedef struct
{
	double item1;
	double item2;
	double item3;
	double item4;
} GEN4DBLUNIT;
typedef struct
{
	double x;
	double y;
	double z;
	double angle;
} ROTATION;
typedef struct
{
	double m0;
	double m1;
	double m2;
	double m3;
} MATRIXROW;
typedef struct
{
	double r;
	double g;
	double b;
} COLOR;
typedef struct
{
	int subIndentNo;
	char *DEF;
} DEFINFO;
typedef struct
{
	BYTE type;
	int openedSubIndents;
	int LODHead;
	BYTE inLOD;
	int DEFIdx;
	DEFINFO DEFList[MAXDEFSPERINDENT];
	VERTEX *coordArray;
	VERTEX *normArray;	
	long coordCount;
	long normCount;
	long matCount;
	int matArrayNum;
	double shininess;
	BYTE vertexOrder;
} INDENTATION;
typedef struct
{
	char *name;
	BYTE len;
	BYTE (*handlingFunc)(INDENTATION *indentArray, int *indentIdx, BYTE indentType,
						 char *line, char *DEF, FILE *vFile[], FILE *outFiles[]);
	BYTE type;
} NODE;
typedef struct
{
	char *prgName;
	char *inFName;
	char **outFNames;	// use oglFName, hdrFName and datFName #defines
	char *currDate;
	char *cmdLSwitches;
	char *center;
	char *hdrDefine;
	char *funcName;
} HEADERFOOTERINFO;


#endif	// _SHAREDDEFS_H_
