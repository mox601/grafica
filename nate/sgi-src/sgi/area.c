/* 
   area.c
   Nate Robins, 1997

   A simple program to compute the area of a rasterized triangle.

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#if defined (__APPLE__) || defined (MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


void
reshape(int width, int height)
{
    printf("width = %d, height = %d, area/2 = %d\n",
        width, height, width*height/2);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 2, 0, 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void
draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3ub(255, 255, 255);
    glBegin(GL_TRIANGLES);
    glNormal3f(0, 0, 1);
    glColor3ub(255, 0, 0);
    glVertex2i(1, 2);
    glColor3ub(0, 255, 0);
    glVertex2i(0, 0);
    glColor3ub(0, 0, 255);
    glVertex2i(2, 0);
    glEnd();

    glutSwapBuffers();
}

int
area(void)
{
    GLfloat* buffer;
    int i, n, size;
    GLfloat a = 0;

    /* allocate memory - 8 values in feedback buffer per triangle */
    buffer = (GLfloat*)malloc(sizeof(GLfloat)*1*8);
    glFeedbackBuffer(1*8, GL_2D, buffer);
    glRenderMode(GL_FEEDBACK);
    draw();
    size = glRenderMode(GL_RENDER);

    i = 0;
    while (i < size) {
        if (buffer[i] == GL_POLYGON_TOKEN) {
            i++; n = buffer[i]; i++;

            /* z component of cross product = twice triangle area, if
               area is negative, the triangle is backfacing */
            a = ((buffer[i+2]-buffer[i+0])*(buffer[i+5]-buffer[i+1]) - 
                (buffer[i+4]-buffer[i+0])*(buffer[i+3]-buffer[i+1])) / 2;

            i += n*2;
        } else {
            printf("unknown token 0x%x parsed at %d\n", (short)buffer[i], i);
            i++;
        }
    }

    free(buffer);
    
    return (int)a;
}

void
display(void)
{
    draw();
    printf("feedback triangle area = %d pixels\n", area());
}

void
keyboard(unsigned char key, int x, int y)
{
    if (key == 27) {
        exit(0);
    }
}

int
main(int argc, char** argv)
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(320, 320);
    glutInit(&argc, argv);

    glutCreateWindow("Area");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
