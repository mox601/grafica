#ifndef __CAMERA_H
#define __CAMERA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     
#include <GL/glut.h>
#include "vectors.h"
#include "model.h"

int startx,starty;

GLfloat dettaglio;
GLfloat dettaglioMax;
GLfloat dettaglioMin;
GLfloat stepDetail;
int draw_wireframe;

void redraw(void);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void key(unsigned char c, int x, int y);
void controlMenu(int value);
void aumentaDettaglio();
void diminuisciDettaglio();
void reshape(int W,int H);

#endif
