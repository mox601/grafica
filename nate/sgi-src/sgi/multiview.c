/*
  multiview.c
  Nate Robins, 1997
	
  Program that shows how to use multiple viewports in a single
  context (using scissoring).
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#if defined (__APPLE__) || defined (MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


GLuint torus_list = 0;
GLfloat spin_x = 0.0;
GLfloat spin_y = 0.0;


void
text(char* string)
{
    char* p;

    for (p = string; *p; p++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);
}

void
lists(void)
{
    GLfloat gold_Ka[4] = { 0.24725f, 0.1995f, 0.0745f, 1.0f };
    GLfloat gold_Kd[4] = { 0.75164f, 0.60648f, 0.22648f, 1.0f };
    GLfloat gold_Ks[4] = { 0.628281f, 0.555802f, 0.366065f, 1.0f };
    GLfloat gold_Ke    = 41.2f;
    GLfloat silver_Ka[4]  = { 0.05f, 0.05f, 0.05f, 1.0f };
    GLfloat silver_Kd[4]  = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat silver_Ks[4]  = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat silver_Ke     = 12.0f;
    
    torus_list = glGenLists(1);
    glNewList(torus_list, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT,  gold_Ka);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  gold_Kd);
    glMaterialfv(GL_FRONT, GL_SPECULAR, gold_Ks);
    glMaterialf(GL_FRONT, GL_SHININESS, gold_Ke);
    glMaterialfv(GL_BACK, GL_AMBIENT,  silver_Ka);
    glMaterialfv(GL_BACK, GL_DIFFUSE,  silver_Kd);
    glMaterialfv(GL_BACK, GL_SPECULAR, silver_Ks);
    glMaterialf(GL_BACK, GL_SHININESS, silver_Ke);
    glutWireTorus(0.3, 0.5, 16, 32);
    glEndList();
}

void
init(void)
{
    GLfloat light_pos[4] = { 1.0, 1.0, 1.0, 0.0 };

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void
reshape(int width, int height)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void
projection(int width, int height, int perspective)
{
    float ratio = (float)width/height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (perspective)
        gluPerspective(60, ratio, 1, 256);
    else 
        glOrtho(-ratio, ratio, -ratio, ratio, 1, 256);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

#define bottom_left                             \
    glViewport(0, 0, width, height);            \
    glScissor(0, 0, width, height)

#define bottom_right                            \
    glViewport(width, 0, width, height);        \
    glScissor(width, 0, width, height)

#define top_left                                \
    glViewport(0, height, width, height);       \
    glScissor(0, height, width, height)

#define top_right                               \
    glViewport(width, height, width, height);   \
    glScissor(width, height, width, height)

#define front                                   \
    projection(width, height, 0);               \
    glRotatef(spin_y, 1.0, 0.0, 0.0);           \
    glRotatef(spin_x, 0.0, 1.0, 0.0)

#define back                                    \
    projection(width, height, 0);               \
    glRotatef(180.0, 0.0, 1.0, 0.0);            \
    glRotatef(spin_y, 1.0, 0.0, 0.0);           \
    glRotatef(spin_x, 0.0, 1.0, 0.0)

#define right                                   \
    projection(width, height, 0);               \
    glRotatef(90.0, 0.0, 1.0, 0.0);             \
    glRotatef(spin_y, 1.0, 0.0, 0.0);           \
    glRotatef(spin_x, 0.0, 1.0, 0.0)

#define left                                    \
    projection(width, height, 0);               \
    glRotatef(-90.0, 0.0, 1.0, 0.0);            \
    glRotatef(spin_y, 1.0, 0.0, 0.0);           \
    glRotatef(spin_x, 0.0, 1.0, 0.0)

#define top                                     \
    projection(width, height, 0);               \
    glRotatef(90.0, 1.0, 0.0, 0.0);             \
    glRotatef(spin_y, 1.0, 0.0, 0.0);           \
    glRotatef(spin_x, 0.0, 1.0, 0.0)

#define bottom                                  \
    projection(width, height, 0);               \
    glRotatef(-90.0, 1.0, 0.0, 0.0);            \
    glRotatef(spin_y, 1.0, 0.0, 0.0);           \
    glRotatef(spin_x, 0.0, 1.0, 0.0)

#define perspective                             \
    projection(width, height, 1);               \
    glRotatef(30.0, 0.0, 1.0, 0.0);             \
    glRotatef(20.0, 1.0, 0.0, 0.0);             \
    glRotatef(spin_y, 1.0, 0.0, 0.0);           \
    glRotatef(spin_x, 0.0, 1.0, 0.0)

void
display(void)
{
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_LIGHTING);
    glColor3ub(255, 255, 255);
    glBegin(GL_LINES);
    glVertex2i(width/2,0);
    glVertex2i(width/2,height);
    glVertex2i(0,height/2);
    glVertex2i(width,height/2);
    glEnd();

    glRasterPos2i(5, 5);
    text("Front");
    glRasterPos2i(width/2+5, 5);
    text("Right");
    glRasterPos2i(5, height/2+5);
    text("Top");
    glRasterPos2i(width/2+5, height/2+5);
    text("Perspective");

    glEnable(GL_LIGHTING);

    width = (width+1)/2;
    height = (height+1)/2;

    glEnable(GL_SCISSOR_TEST);

    bottom_left;
    front;
    glCallList(torus_list);
    
    bottom_right;
    right;
    glCallList(torus_list);

    top_left;
    top;
    glCallList(torus_list);

    top_right;
    perspective;
    glCallList(torus_list);

    glDisable(GL_SCISSOR_TEST);

    glutSwapBuffers();
}

void
keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
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

int
main(int argc, char** argv)
{
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(512, 512);
    glutInit(&argc, argv);
    glutCreateWindow("Multiple Viewports");
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
  
    init();
    lists();

    glutMainLoop();
    return 0;
}
