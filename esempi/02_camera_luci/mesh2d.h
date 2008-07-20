/*
 *  mesh2d.h
 *  
 *
 *  Created by Matteo on 19/07/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

//#include <GL/glut.h>


#ifndef __MESH2d_H_
#define __MESH2d_H_

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif



#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "ply.h"


/* definition of a triangle as 3 indeces to a vector of float containing vertices infos */
struct triangle_t
{
	int i0,i1,i2;
	struct triangle_t* next;
};

struct vertex_t
{
	float x,y,z;		/* world coordinates */
	float nx,ny,nz;	/* normal */
};


//static struct
struct mesh
{
	/* number of vertices */
	int   numvertices; 

	/* pointer to vertices */
	struct vertex_t* vertices;

	/* pointer to first triangle */
	struct triangle_t* triangles; 
};
//ho invertito nome/definizione


typedef struct mesh meshType;

#define swap_int(a,b) {int _tmp=(a);(a)=(b);(b)=_tmp;}
#define min2(a,b) (((a)<=(b))?(a):(b))
#define max2(a,b) (((a)>=(b))?(a):(b))



/*======================================================= */
/*======================================================= */
void set_normals(meshType mesh);

/*======================================================= */
/*======================================================= */

meshType open_ply(const char* filename);


/*======================================================= */
/*======================================================= */

//devo levare static?

void draw_triangles(meshType mesh);

/*======================================================= */
/*======================================================= */

void display_ply(meshType mesh, int viewmode); /* 0==filled face 1==filled+wireframe 2==wireframe */


#endif
