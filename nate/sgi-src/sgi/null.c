/* 
   null.c
   Nate Robins, 1997

   An example of using null bitmaps to place the rasterpos at
   positions that may be clipped.

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined (__APPLE__) || defined (MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


unsigned char image[256*256*3];
unsigned char bitmap[1] = { 0 };
int raster_x = 32;
int raster_y = 32;
int old_raster_x;
int old_raster_y;

void
reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void
display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glRasterPos2i(0, 0);
    glBitmap(0, 0, 0, 0, raster_x, raster_y, NULL);
    glDrawPixels(256, 256, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)image);

    glutSwapBuffers();
}

void
mouse(int button, int state, int x, int y)
{
    y = glutGet(GLUT_WINDOW_HEIGHT) - y;

    old_raster_x = x - raster_x;
    old_raster_y = y - raster_y;
}

void
motion(int x, int y)
{
    y = glutGet(GLUT_WINDOW_HEIGHT) - y;

    raster_x = x - old_raster_x;
    raster_y = y - old_raster_y;

    glutPostRedisplay();
}

void
keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);
}

int
main(int argc, char** argv)
{
    int i, j;

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(320, 320);
    glutInit(&argc, argv);

    glutCreateWindow("null");
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);

    /* create a pretty color ramp */
    for (j = 0; j < 256; j++) {
        for (i = 0; i < 256; i++) {
            image[(256 * j + i) * 3 + 0] = 255-i*j/255;
            image[(256 * j + i) * 3 + 1] = i;
            image[(256 * j + i) * 3 + 2] = j;
        }
    }

    glutMainLoop();
    return 0;
}
