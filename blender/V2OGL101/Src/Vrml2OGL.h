/********************************************************************************************
** Vrml2OGL.h - Header file for Vrml2OGL.cpp
** Copyright 1999 by Alexander Rohra. All rights reserved.
**
** Program Name:	Vrml2OGL.exe V1.0 -> V1.01
** Author:			Alexander Rohra
** Date:			May 1999
** Compiler:		MSVC++ V5.0	& others
** App Type:		Command Line Utility (Win32 Console Application)
** Revisions:		August 1999 - Alexander Rohra -> V1.01:
**					1)	Added multi platform compatible #defines to support the changes
**						made to Vrml2OGL.cpp
** Code Desc.:		The definitions in this header file are primarily used by
**					code in Vrml2OGL.cpp but might be used by other source code
**					modules including this file.
********************************************************************************************/

#ifndef _VRML2OGL_H_
#define _VRML2OGL_H_

#include "SharedDefs.h"	// for node type defines (eg. SEPERATOR)

#define _V_DEBUGMODE_	// comment this out to _slightly_ improve program performance
//#define NOMODELID		// uncomment this to keep Vrml2OGL from generating variable names,
						// #defines and default output file names that include the input
						// file name's base name (MODELID); see main() for more detail
#define MAXFNAME		100
#define MAXDATESTRING	40
#define MINARGS			2
#define MAXVRMLFPOINTER	3
#define MINFILLERSIZE	0
#define MAXFILLERSIZE	7
#define MAXCENTERSTR	256
#define MAXCMDLSWSTR	256
#define USETAB			-1
#define DEFLTFILLERSIZE USETAB
#define BLANKS			"                                                                    "
#define EXTCHAR			'.'
#ifdef _WIN32
	#define PATHCHAR	'\\'	// for windows platforms
#else
	#define	PATHCHAR	'/'		// for UNIX platforms
#endif // _WIN32

// Note: The handling functions for the following nodes (recognized by type GENXFRM) are by
//		 default changed to handleGenericNode() by code in main() (see includeTransformations)
//		 unless the -it argument was specified on the command line:
//		 Transform, Translation, Rotation, Scale & MatrixTransform
#define NODETYPES		{{"Separator", 0, handleIndent, SEPARATOR}, \
						 {"Group", 0, handleSubIndent, GROUP}, \
						 {"Switch", 0, handleSubIndent, SWITCH}, \
						 {"LOD", 0, handleSubIndent, LOD}, \
						 {"TransformSeparator", 0, handleSubIndent, TRANSFORMSEPARATOR}, \
						 {"Coordinate3", 0, handleCoordinate3, ANYTYPE}, \
						 {"ShapeHints", 0, handleShapeHints, ANYTYPE}, \
						 {"IndexedFaceSet", 0, handleIndexedFLSet, FACESET}, \
						 {"IndexedLineSet", 0, handleIndexedFLSet, LINESET}, \
						 {"Normal", 0, handleNormal, ANYTYPE}, \
						 {"Material", 0, handleMaterial, ANYTYPE}, \
						 {"Transform", 0, handleTransform, GENXFRM}, \
						 {"Translation", 0, handleTranslation, GENXFRM}, \
						 {"Rotation", 0, handleRotation, GENXFRM}, \
						 {"Scale", 0, handleScale, GENXFRM}, \
						 {"MatrixTransform", 0, handleMatrixTransform, GENXFRM}, \
						 {"Info", 0, handleInfo, ANYTYPE}, \
/* keep this at bottom */{"{", 0, handleGenericNode, OPENBRACE}, \
/* keep this at bottom */{"}", 0, handleIndentClose, INDENTCLOSE}, \
						 {NULL, 0, NULL, END}};

/*** The following nodes are all handled genericly (through the "{" node entry above) ***
						 {"NormalBinding", 0, handleGenericNode, GENERIC}
						 {"MaterialBinding", 0, handleGenericNode, GENERIC}
						 {"Cone", 0, handleGenericNode, GENERIC}
						 {"Cube", 0, handleGenericNode, GENERIC}
						 {"Cylinder", 0, handleGenericNode, GENERIC}
						 {"Sphere", 0, handleGenericNode, GENERIC}
						 {"PointSet", 0, handleGenericNode, GENERIC}
						 {"Texture2", 0, handleGenericNode, GENERIC}
						 {"TextureCoordinate2", 0, handleGenericNode, GENERIC}
						 {"Texture2Transform", 0, handleGenericNode, GENERIC}
						 {"DirectionalLight", 0, handleGenericNode, GENERIC}
						 {"SpotLight", 0, handleGenericNode, GENERIC}
						 {"PointLight", 0, handleGenericNode, GENERIC}
						 {"OrhtographicCamera", 0, handleGenericNode, GENERIC}
						 {"PerspectiveCamera", 0, handleGenericNode, GENERIC}
						 {"FontStyle", 0, handleGenericNode, GENERIC}
						 {"AsciiText", 0, handleGenericNode, GENERIC}
						 {"WWWInline", 0, handleUnimplementedNode, GENERIC}
						 {"WWWAnchor", 0, handleUnimplementedNode, GENERIC}
****************************************************************************************/

enum {OFF=0, ON, AUTOCENTER};

#define PRGNAME			"Vrml2OGL V1.01"
#define PRGHEADER		PRGNAME \
						" Copyright 1999 by Alexander Rohra. All Rights Reserved.\n"

#ifdef _V_DEBUGMODE_
	#define USAGEOPTIONAL	" [-d]"
#else
	#define USAGEOPTIONAL	""
#endif

#define USAGE			PRGHEADER \
						"Usage: Vrml2OGL [-?] | [[-it] [-c | [-cxVAL -cyVAL -czVAL]] [-rf]" \
							" [-rn]\n" \
						"                [-ft | -f0<=VAL<8] [-bt | -b0<=VAL<8]" \
							USAGEOPTIONAL \
						"\n" \
						"                vrmlFile [oglFile hdrFile datFile]]\n"

#define HELPPART1		"This utility converts a VRML V1.0 file to OpenGL C code.\n\n" \
						"Command Line Options:\n" \
						"-it      : include transformations such as translations, rotations," \
							" etc.\n" \
						"         : if present in the input file (by DEFAULT xfrms are not" \
							" included)\n" \
						"-c       : compute the object center and subtract it from all" \
							" obj. coordinates\n" \
						"-cx/y/zV : subract a value from each x,y and/or z coordinate, where" \
							" V is any\n" \
						"         : real number\n" \
						"-rf/n    : reverse all faces (vertex order) and/or normals\n" \
						"-fV      : for each logical indentation use V amount of spaces," \
							" where 0<=V<8\n" \
						"-ft      : for each logical indentation use one TAB (DEFAULT)\n" \
						"-bV      : precede each line with an indentation base of V amount" \
							" of spaces,\n" \
						"         : where 0<=V<8\n" \
						"-bt      : precede each line with an indentation base of one TAB" \
							" (DEFAULT)\n"

#ifdef _V_DEBUGMODE_
	#define HELPOPTIONAL	"-d       : prints some progress info to stdout (helpful for" \
								" debugging)\n\n"
#else
	#define HELPOPTIONAL	"\n"
#endif

#define HELPPART2		"File Names:\n" \
						"vrmlFile               : input VRML V1.0 file (*.wrl) to be" \
							" converted\n" \
						"oglFile/hdrFile/datFile: name of the main OpenGL C code/header/data" \
							" files to be\n" \
						"                         created\n" \
						"\nPlease view Readme.txt for more info. incl. disclaimer &" \
						" distribution notice"

#define HELPSCREEN		PRGHEADER \
						HELPPART1 \
						HELPOPTIONAL \
						HELPPART2

// global variables
#ifdef _V_DEBUGMODE_
	extern BYTE debugMode;
#endif _V_DEBUGMODE_

extern BYTE centerMode;
extern VERTEX objCenter;
extern BYTE reverseNormals;
extern BYTE reverseFaces;
extern char *baseName;
extern char *filler[];

#endif // _VRML2OGL_H_

