/* 
   nii.c
   Nate Robins, 1997

   Network Integration Incorporated logo.

*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined (__APPLE__) || defined (MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


GLfloat n_vertices[] = {
    -17, 17, 6,             /* front */
    -17, -17, 6,
    -5, -17, 6,
    -5, -7, 6,
    5, -17, 6,
    17, -17, 6,
    17, 9, 6,
    5, 9, 6,
    5, 7, 6,
    -5, 17, 6,
    -17, 17, -6,            /* back */
    -17, -17, -6,
    -5, -17, -6,
    -5, -7, -6,
    5, -17, -6,
    17, -17, -6,
    17, 9, -6,
    5, 9, -6,
    5, 7, -6,
    -5, 17, -6,
    6, 8, 6,
    6, -16, 6,
    16, -16, 6,
    16, 8, 6,
    6, 8, -6,
    6, -16, -6,
    16, -16, -6,
    16, 8, -6,
};

GLuint n_vindices[] = {
    0, 1, 2,                /* front */
    0, 2, 9,
    9, 3, 8,
    8, 3, 4, 
/*   7, 4, 6, */
/*   6, 4, 5, */
    10, 12, 11,             /* back */
    10, 19, 12,
    19, 18, 13,
    18, 14, 13,
/*   17, 16, 14, */
/*   16, 15, 14, */
    0, 9, 19,               /* top left */
    0, 19, 10,
    19, 9, 8,               /* top slant */
    8, 18, 19,
    8, 7, 17,               /* small step */
    17, 18, 8,
    17, 7, 6,               /* below cube */
    17, 6, 16,
    6, 5, 15,               /* top right */
    16, 6, 15, 
    5, 14, 15,              /* bottom right */
    5, 4, 14,
    4, 3, 13,               /* bottom slant */
    4, 13, 14,
    2, 12, 13,              /* step */
    2, 13, 3,
    2, 11, 12,              /* bottom left */
    2, 1, 11,
    1, 0, 10,               /* left */
    1, 10, 11,
    7, 4, 20,               /* hollow front */
    20, 4, 21,
    21, 4, 5,
    21, 5, 22,
    22, 5, 6,
    6, 23, 22,
    7, 23, 6,
    7, 20, 23,
    17, 24, 14,             /* hollow back */
    24, 25, 14,
    25, 15, 14,
    25, 26, 15,
    26, 16, 15,
    16, 26, 27,
    17, 16, 27,
    17, 27, 24,
    20, 24, 23,             /* hollow top */
    24, 27, 23,
    23, 27, 26,             /* hollow right */
    23, 26, 22,
    20, 21, 25,             /* hollow left */
    20, 25, 24, 
    25, 21, 22,             /* hollow bottom */
    25, 22, 26,
};

GLfloat n_normals[] = {
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
/*   0, 0, 1, */
/*   0, 0, 1, */
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
/*   0, 0, -1, */
/*   0, 0, -1, */
    0, 1, 0,
    0, 1, 0,
    0.7071f, 0.7071f, 0,
    0.7071f, 0.7071f, 0,
    -1, 0, 0,
    -1, 0, 0,
    0, 1, 0,
    0, 1, 0,
    1, 0, 0,
    1, 0, 0,
    0, -1, 0,
    0, -1, 0,
    -0.7071f, -0.7071f, 0,
    -0.7071f, -0.7071f, 0,
    1, 0, 0,
    1, 0, 0,
    0, -1, 0,
    0, -1, 0,
    -1, 0, 0,
    -1, 0, 0,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, -1, 0,
    0, -1, 0,
    -1, 0, 0,
    -1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    0, 1, 0,
    0, 1, 0,
};

GLint n_numtriangles = 52;

GLfloat c_vertices[] = {
    -6, 3, 6,
    -6, -3, 6,
    6, -3, 6,
    6, 3, 6,
    -6, 3, -6,
    -6, -3, -6,
    6, -3, -6,
    6, 3, -6,
};

GLuint c_vindices[] = {
    0, 1, 2,
    0, 2, 3,
    0, 3, 7,
    0, 7, 4,
    3, 2, 6,
    3, 6, 7,
    7, 6, 5,
    7, 5, 4,
    6, 2, 1,
    6, 1, 5,
    4, 5, 1,
    4, 1, 0,
};

GLfloat c_normals[] = {
    0, 0, 1,
    0, 0, 1,
    0, 1, 0,
    0, 1, 0,
    1, 0, 0,
    1, 0, 0,
    0, 0, -1,
    0, 0, -1,
    0, -1, 0,
    0, -1, 0,
    -1, 0, 0,
    -1, 0, 0,
};

GLint c_numtriangles = 12;

GLfloat spin_x = -30;
GLfloat spin_y = 25;
GLfloat spin_c = 0;

int grab_frames = 0;


void
reshape(int width, int height)
{
    GLfloat light_pos[] = { 0, 1, 1, 0 };

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)width/height, 1, 1200); 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 60, 0, 0, 0, 0, 1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glClearColor(1, 1, 1, 1);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
}

void
display(void)
{
    int i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glRotatef(spin_y, 1, 0, 0);
    glRotatef(spin_x, 0, 1, 0);

    glColor3ub(165, 163, 160);
    glBegin(GL_TRIANGLES);
    for (i = 0; i < n_numtriangles; i++) {
        glNormal3fv(&n_normals[3*i]);
        glVertex3fv(&n_vertices[3*n_vindices[3*i+0]]);
        glVertex3fv(&n_vertices[3*n_vindices[3*i+1]]);
        glVertex3fv(&n_vertices[3*n_vindices[3*i+2]]);
    }
    glEnd();

    glColor3ub(0, 90, 107);
    glTranslatef(11, 14, 0);
    glRotatef(spin_c, 0, 1, 0);
    glBegin(GL_TRIANGLES);
    for (i = 0; i < c_numtriangles; i++) {
        glNormal3fv(&c_normals[3*i]);
        glVertex3fv(&c_vertices[3*c_vindices[3*i+0]]);
        glVertex3fv(&c_vertices[3*c_vindices[3*i+1]]);
        glVertex3fv(&c_vertices[3*c_vindices[3*i+2]]);
    }
    glEnd();

    glPopMatrix();

    glutSwapBuffers();

    if (grab_frames) {
        static int i = 0;
        char s[256];
        sprintf(s, "scrsave image%02d.sgi %d %d %d %d", i++, 
            glutGet(GLUT_WINDOW_X), 
            glutGet(GLUT_WINDOW_X)+glutGet(GLUT_WINDOW_WIDTH)-1,
            glutGet(GLUT_SCREEN_HEIGHT)-glutGet(GLUT_WINDOW_Y)-1,
            glutGet(GLUT_SCREEN_HEIGHT)-
            (glutGet(GLUT_WINDOW_Y)+glutGet(GLUT_WINDOW_HEIGHT)));
        printf("image%02d.sgi\n", i-1);
        system(s);
    }
}

void
idle(void)
{
    if (grab_frames) {
        spin_c += 10;
        if (spin_c > 360/4)
            exit(0);
    } else {
        spin_c += 1;
        if (spin_c > 360)
            spin_c -= 360;
    }
    glutPostRedisplay();
}

void
keyboard(unsigned char key, int x, int y)
{
    static int toggle = 0;

    switch (key) {
    case 27:
        exit(0);
    case ' ':
        spin_c = 0;
        toggle = !toggle;
        if (toggle)
            glutIdleFunc(NULL);
        else
            glutIdleFunc(idle);
        break;
    case 'g':
        spin_c = 0;
        grab_frames = 1;
        break;
    }

    glutPostRedisplay();
}

int old_x, old_y;

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

void
visibility(int state)
{
    if (state == GLUT_VISIBLE)
        glutIdleFunc(idle);
    else
        glutIdleFunc(NULL);
}

int
main(int argc, char** argv)
{
    glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(640, 480);
    glutInit(&argc, argv);

    glutCreateWindow("Network Integration");
    glutVisibilityFunc(visibility);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
