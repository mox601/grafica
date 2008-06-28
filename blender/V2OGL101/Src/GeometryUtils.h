/********************************************************************************************
** GeometryUtils.h - Header file for GeometryUtils.cpp
** Copyright 1999 by Alexander Rohra. All rights reserved.
**
** Program Name:	Vrml2OGL.exe V1.0 -> V1.01
** Author:			Alexander Rohra
** Date:			May 1999
** Compiler:		MSVC++ V5.0	& others
** App Type:		Command Line Utility (Win32 Console Application)
** Revisions:
** Code Desc.:		The definitions in this header file are primarily used by
**					code in GeometryUtils.cpp but might be used by other source
**					code modules including this file.
********************************************************************************************/

#ifndef _GEOMETRYUTILS_H_
#define _GEOMETRYUTILS_H_

#include "SharedDefs.h"		// needed for typedefs

#define DOUBLEPI			(double)3.1415926535898
#define radiansToDegree(r)	(r*(double)180/DOUBLEPI)

// *** function prototypes for global/public functions ***
VERTEX getBoxCenter(VERTEX *boundingBox);
VECTOR calculateNormal(VERTEX *vA, VERTEX *vB, VERTEX *vC);

#endif	// _GEOMETRYUTILS_H_
