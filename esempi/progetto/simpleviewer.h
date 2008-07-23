//#include <GL/glut.h>

#ifndef __SIMPLEVIEWER_H
#define __SIMPLEVIEWER_H

#include "mesh2d.h"

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

#define TOTALMESHES 20

/* dati per il caricamento delle mesh */
//int totalMeshes = TOTALMESHES;

meshType meshes[TOTALMESHES];
//int enable_texture=0;

#endif