#ifndef __LIGHTS_H
#define __LIGHTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>  

/* posizioni */
GLfloat light_position_local[4];
GLfloat light_position_directional[4];

/* colori */
GLfloat color_black     [4]; 
GLfloat color_white     [4]; 
GLfloat color_red       [4];
GLfloat color_green     [4];
GLfloat color_blue      [4];
GLfloat coloryellow	[4];




void setupLights();


#endif
