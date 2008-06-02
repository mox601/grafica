#ifndef __MODEL_H
#define __MODEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include "vectors.h"

GLfloat lunghezza;
GLfloat altezza;

void drawTriangle(Point3d* point1, Point3d* point2, Point3d* point3, GLfloat dettaglio);
void drawWall(Point3d* point1, Point3d* point2, Point3d* point3, Point3d* point4, GLfloat spessore, GLfloat spigolo_laterale, GLfloat incl_frontale, GLfloat dettaglio);
void drawWallSize(GLfloat lunghezza, GLfloat dettaglio, GLfloat inclinazione); 
void drawWallSizeHigh(GLfloat lunghezza, GLfloat altezza, GLfloat inclinazione,  GLfloat spessore, GLfloat dettaglio, GLfloat lambda); 

void drawWallOblique(Point3d* point1, Point3d* point2, Point3d* point3, Point3d* point4, GLfloat spessore, GLfloat spigolo_laterale_front, GLfloat spigolo_laterale_rear, GLfloat incl_frontale, GLfloat dettaglio, GLfloat differenza); 

void drawFrontWall(GLfloat lunghezza, GLfloat dettaglio, GLfloat segno_inclinazione, GLfloat altezza);
void drawGriglia(GLfloat dim_larghezza, GLfloat dim_altezza, GLfloat altezzaMax, GLfloat dim_spessore, int n_strisce, GLfloat inclinazione);
void drawStriscia(Point3d punto_start, Point3d punto_end, GLfloat spessore, GLfloat inclinazione);


#endif

