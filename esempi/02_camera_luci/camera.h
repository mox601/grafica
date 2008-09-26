#ifndef __CAMERA_H
#define __CAMERA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     
//#include <GL/glut.h>
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

#include "vectors.h"
#include "model.h"

int startx,starty;

GLfloat dettaglio;
GLfloat dettaglioMax;
GLfloat dettaglioMin;
GLfloat stepDetail;
int draw_wireframe;


int enable_light_directional;
int enable_light_localONE;
int enable_light_localTWO;
int enable_light_localTHREE;


GLfloat xPosition;
GLfloat yPosition;
GLfloat zPosition;

GLUquadricObj *quadratic;

GLuint	filter;			// Which Filter To Use (nearest/linear/mipmapped)
GLuint	texture[3];		// Storage for 3 textures.

/* Image type  */
struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};
typedef struct Image Image;





void redraw(void);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void key(unsigned char c, int x, int y);
void controlMenu(int value);
void aumentaDettaglio();
void diminuisciDettaglio();
void reshape(int W,int H);


GLvoid LoadGLTextures(GLvoid);
int ImageLoad(char *filename, Image *image);








#endif
