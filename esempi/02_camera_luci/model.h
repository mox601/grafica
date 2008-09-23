#ifndef __MODEL_H
#define __MODEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

//#include <GL/glut.h>


#include "vectors.h"
#include "camera.h"


/* blending acceso/spento */
GLuint  blend;


GLfloat lunghezza;
GLfloat altezza;
GLfloat profondita_lunghezza_ratio;



void drawTriangle(Point3d* point1, Point3d* point2, Point3d* point3, GLfloat dettaglio);

void drawWall(Point3d* point1, Point3d* point2, Point3d* point3, Point3d* point4, GLfloat spessore, GLfloat spigolo_laterale, GLfloat incl_frontale, GLfloat dettaglio);

void drawWallSize(GLfloat lunghezza, GLfloat dettaglio, GLfloat inclinazione); 

void drawWallSizeHigh(GLfloat lunghezza, GLfloat altezza, GLfloat inclinazione,  GLfloat spessore, GLfloat dettaglio, GLfloat lambda, GLfloat sp_laterale_front, GLfloat sp_laterale_rear, GLfloat differenza); 

void drawWallOblique(Point3d* point1, Point3d* point2, Point3d* point3, Point3d* point4, GLfloat spessore, GLfloat spigolo_laterale_front, GLfloat spigolo_laterale_rear, GLfloat incl_frontale, GLfloat dettaglio, GLfloat differenza); 

void drawFrontWall(GLfloat lunghezza, GLfloat dettaglio, GLfloat segno_inclinazione, GLfloat altezza);

void drawGriglia(GLfloat dim_larghezza, GLfloat dim_altezza, GLfloat altezzaMax, GLfloat dim_spessore, int n_strisce, GLfloat inclinazione);

void drawStriscia(Point3d punto_start, Point3d punto_end, GLfloat spessore, GLfloat inclinazione);

void drawRearWall(GLfloat lunghezza, GLfloat dettaglio, GLfloat segno_inclinazione, GLfloat altezza);

void drawGlass(Point3d* a, Point3d* b, Point3d* c, Point3d* d);

void drawWallHighNew(Point3d* point1, Point3d* point2, Point3d* point3, Point3d* point4, GLfloat spessore, GLfloat spigolo_laterale_front, GLfloat spigolo_laterale_rear, GLfloat incl_frontale, GLfloat dettaglio, GLfloat differenza, GLfloat altezza);

GLdouble* drawCircle(GLfloat radius, GLint dettaglio, GLfloat Start_angolo, GLfloat End_angolo);
void setMaterial(GLfloat R,GLfloat G,GLfloat B);
void setMaterialType(GLfloat R,GLfloat G,GLfloat B, unsigned char tipo);

void drawShell(GLdouble* circle, GLint dettaglio, GLfloat profondita);

void drawShellInvertedNormals(GLdouble* circle, GLint numPoints, GLfloat profondita);

void drawSingleColumn(GLfloat raggio, GLfloat altezzaColonne);

void drawColumns(GLfloat raggio, GLfloat altezzaColonne);



#endif

