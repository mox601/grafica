#ifndef __LIGHTS_H
#define __LIGHTS_H

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


#define GLSCALAMENTO 2.0


//#include <GL/glut.h>  

/* posizioni */
GLfloat light_position_local[4];
GLfloat light_position_localONE[4];
GLfloat light_position_localTWO[4];
GLfloat light_position_localTHREE[4];
GLfloat light_position_directional[4];
GLfloat light_position_lunar[4];
GLfloat direzioneFaro[3];
GLfloat faro_spotlight[4];




/* colori */
GLfloat color_black     [4]; 
GLfloat color_white     [4]; 
GLfloat color_red       [4];
GLfloat color_green     [4];
GLfloat color_blue      [4];
GLfloat coloryellow	[4];
GLfloat color_yellow_light[4];
GLfloat sun_color_interni[4];
GLfloat moon_color_interni[4];
GLfloat moon_color_esterni[4];
GLfloat sun_color_interni_internalshell[4];
GLfloat variableLight[4];





void setupLights();


#endif
