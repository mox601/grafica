#ifndef __VECTORS_H
#define __VECTORS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>  


typedef struct {GLfloat x,y,z;} Point3d;
typedef struct {GLfloat x,y,z;} Vector3d;


void point_translate(Point3d* point,const Vector3d* v);

void vector_normalize(Vector3d* v);

GLfloat vector_modulo(Vector3d* v);

void vector_scale(Vector3d* v,GLfloat coeff);

void init_point(Point3d* source, Point3d* dest);

void point_scale(Point3d* v,GLfloat coeff);

void vector_diff(Vector3d* dest,const Point3d* u,const Point3d* v);

void vector_cross_product(Vector3d* dest,const Vector3d* u,const Vector3d* v);

GLfloat minDistance(Point3d* point1, Point3d* point2, Point3d* point3);

GLfloat getMinimo(GLfloat a, GLfloat b, GLfloat c);

void midPoint(Point3d* dest, Point3d* a, Point3d* b);

void convexCombination(Point3d* destinazione, Point3d* a, Point3d* b, GLfloat lambda);

#endif

