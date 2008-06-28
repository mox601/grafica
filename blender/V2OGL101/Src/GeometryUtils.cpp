/********************************************************************************************
** GeometryUtils.cpp - Source Code Module for Vrml2OGL.exe V1.0 -> V1.01
** Copyright 1999 by Alexander Rohra. All rights reserved.
**
** Program Name:	Vrml2OGL.exe V1.0 -> V1.01
** Author:			Alexander Rohra
** Date:			May 1999
** Compiler:		MSVC++ V5.0	& others
** App Type:		Command Line Utility (Win32 Console Application)
** Revisions:
** Code Desc.:		This source code module contains math related
**					functions used by VRML2OGL. 
**
** General Info:	Please see Vrml2OGL.cpp for general information about the
**					program.
********************************************************************************************/

#include <stdio.h>
#include <math.h>

#include "GeometryUtils.h"	// needed for typedefs from "SharedDefs.h"
#include "Vrml2OGL.h"		// needed for debugMode extern var definition

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: getBoxCenter()
// 
// Description:
//	This function calculates the center of a box defined by 8 vertecis. It returns the
//	resulting center vertex.
/////////////////////////////////////////////////////////////////////////////////////////////
VERTEX getBoxCenter(VERTEX *boundingBox)
{
	static VERTEX center;
	static double width, height, depth, widthDiv2, heightDiv2, depthDiv2;

	width=boundingBox[1].x-boundingBox[0].x;		// maxX-minX
	height=boundingBox[0].y-boundingBox[2].y;		// maxY-minY
	depth=boundingBox[4].z-boundingBox[0].z;		// maxZ-minZ	

	widthDiv2=width<0 ? -width/2 : width/2;
	heightDiv2=height<0 ? -height/2 : height/2;
	depthDiv2=depth<0 ? -depth/2 : depth/2;	

	center.x=boundingBox[0].x+widthDiv2;
	center.y=boundingBox[2].y+heightDiv2;
	center.z=boundingBox[0].z+depthDiv2;

	return(center);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Function Name: calculateNormal()
// 
// Description:
//	This function calculates the normalized normal vector of a polygon by using 3 of the
//	polygon's vertecis. It returns the resulting normal vector.
/////////////////////////////////////////////////////////////////////////////////////////////
VECTOR calculateNormal(VERTEX *vA, VERTEX *vB, VERTEX *vC)
{
	static VECTOR normal;
	static VERTEX tmpA, tmpB;
	double length;	
	double tmpVal;	
	
	// calculate normal vector
	tmpA.x=vB->x - vA->x;
	tmpA.y=vB->y - vA->y;	// = A
	tmpA.z=vB->z - vA->z;

	tmpB.x=vC->x - vA->x;
	tmpB.y=vC->y - vA->y;	// = B
	tmpB.z=vC->z - vA->z;
	  
	// calculate cross product of vectors A*B
	tmpVal=(tmpA.y*tmpB.z)-(tmpB.y*tmpA.z);
	normal.x=tmpVal;
	tmpVal=(tmpA.z*tmpB.x)-(tmpB.z*tmpA.x);
	normal.y=tmpVal;
	tmpVal=(tmpA.x*tmpB.y)-(tmpB.x*tmpA.y);
	normal.z=tmpVal;

	// normalize normal vector
	length=sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);	

	if(length)
	{
		normal.x/=length;
		normal.y/=length;
		normal.z/=length;
	}

	return(normal);
}
