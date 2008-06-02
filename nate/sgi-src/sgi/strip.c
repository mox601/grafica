/* 
   strip.c
   Nate Robins, 1997

   A simple program to show how to do a 'swaptmesh' to generate a
   longer triangle strip in OpenGL.

*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#if defined (__APPLE__) || defined (MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifdef _WIN32
#define M_PI 3.1415926
#endif

#define MAX_MESH 10
GLfloat mesh[MAX_MESH][MAX_MESH];

GLfloat old_x, old_y, spin_x = 0, spin_y = 0;


void
reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)width/height, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_BACK, GL_LINE);
}

void
display(void)
{
    int i, k, swap = 0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(-MAX_MESH/2, 0, -MAX_MESH*2);
    glRotatef(spin_x, 0, 1, 0);
    glRotatef(spin_y, 1, 0, 0);

    glColor3ub(255, 255, 255);
    glBegin(GL_TRIANGLE_STRIP);
    for (k = 0; k < MAX_MESH-1; k++) {
        if (swap) {
            for (i = MAX_MESH-1; i >= 0; i--) {
                glColor3ub(255, 0, 0);
                glVertex3f(i, mesh[k][i], k);
                glColor3ub(0, 0, 255);
                glVertex3f(i, mesh[k+1][i], k+1);
                if (i == 0)
                    glVertex3f(i, mesh[k+1][i], k+1); /* degenerate */
            }
        } else {
            for (i = 0; i < MAX_MESH; i++) {
                glColor3ub(255, 0, 0);
                glVertex3f(i, mesh[k][i], k);
                glColor3ub(0, 0, 255);
                glVertex3f(i, mesh[k+1][i], k+1);
                if (i == MAX_MESH-1)
                    glVertex3f(i, mesh[k+1][i], k+1); /* degenerate */
            }
        }
        swap ^= 1;
    }
    glEnd();

    glPopMatrix();

    glutSwapBuffers();
}

void
keyboard(unsigned char key, int x, int y)
{
    static int wireframe = 0;

    if (key == 27) {
        exit(0);
    } else if (key == 'w') {
        wireframe ^= 1;
        if (wireframe)
            glPolygonMode(GL_FRONT, GL_LINE);
        else
            glPolygonMode(GL_FRONT, GL_FILL);
    } else if (key == 'c') {
        if (glIsEnabled(GL_CULL_FACE))
            glDisable(GL_CULL_FACE);
        else
            glEnable(GL_CULL_FACE); 
    }

    glutPostRedisplay();
}

void
mouse(int button, int state, int x, int y)
{
    old_x = x;
    old_y = y;

    glutPostRedisplay();
}

void
motion(int x, int y)
{
    spin_x = x - old_x;
    spin_y = y - old_y;

    glutPostRedisplay();
}

int
main(int argc, char** argv)
{
    int i, k;

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(320, 320);
    glutInit(&argc, argv);

    for (k = 0; k < MAX_MESH; k++) {
        for (i = 0; i < MAX_MESH; i++) {
            mesh[k][i] = sin((float)(i+k)/MAX_MESH*M_PI)*3;
        }
    }

    glutCreateWindow("strip");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}
