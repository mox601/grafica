/* 
   qix.c
   Nate Robins, 1997

   An example of a 'qix'-like line demo, but without the traditional
   erase lines, and with anti-aliased lines.

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined (__APPLE__) || defined (MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


/* #define SCREEN_SAVER_MODE */


typedef struct _point {
    float x, y;
    struct _point* next;
} point;


point* first;
point* second;
int    num_points = 73;
float  first_vx = 0.223487f;
float  first_vy = 0.532978f;
float  second_vx = 0.63257f;
float  second_vy = 0.325897f;
float  vstep = 27.232743f;

#define COLORS 48
GLubyte colors[COLORS][3] = {
    { 255, 0, 0 },
    { 255, 32, 0 },
    { 255, 64, 0 },
    { 255, 96, 0 },
    {  255, 128, 0 },
    {  255, 160, 0 },
    {  255, 192, 0 },
    {  255, 224, 0 },
    {  255, 255, 0 },
    {  224, 255, 0 },
    {  192, 255, 0 },
    {  160, 255, 0 },
    {  128, 255, 0 },
    {  96, 255, 0 },
    {  64, 255, 0 },
    {  32, 255, 0 },
    {  0, 255, 0 },
    {  0, 255, 32 },
    {  0, 255, 64 },
    {  0, 255, 96 },
    {  0, 255, 128 },
    {  0, 255, 160 },
    {  0, 255, 192 },
    {  0, 255, 224 },
    {  0, 255, 255 },
    {  0, 224, 255 },
    {  0, 196, 255 },
    {  0, 160, 255 },
    {  0, 128, 255 },
    {  0, 96, 255 },
    {  0, 64, 255 },
    {  0, 32, 255 },
    {  0, 0, 255 },
    {  32, 0, 255 },
    {  64, 0, 255 },
    {  96, 0, 255 },
    {  128, 0, 255 },
    {  160, 0, 255 },
    {  196, 0, 255 },
    {  224, 0, 255 },
    {  255, 0, 255 },
    {  255, 0, 224 },
    {  255, 0, 196 },
    {  255, 0, 160 },
    {  255, 0, 128 },
    {  255, 0, 96 },
    {  255, 0, 64 },
    {  255, 0, 32 },
};


void
reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void
display(void)
{
    int i;
    static int color = 0;

    glClear(GL_COLOR_BUFFER_BIT);

    first->next->x = first->x + first_vx;
    first->next->y = first->y + first_vy;
    first = first->next;
    if (first->x < 0) {
        first->x = 0;
        first_vx = -first_vx;
    } else if (first->x > glutGet(GLUT_WINDOW_WIDTH)) {
        first->x = glutGet(GLUT_WINDOW_WIDTH);
        first_vx = -first_vx;
    }
    if (first->y < 0) {
        first->y = 0;
        first_vy = -first_vy;
    } else if (first->y > glutGet(GLUT_WINDOW_HEIGHT)) {
        first->y = glutGet(GLUT_WINDOW_HEIGHT);
        first_vy = -first_vy;
    }
    
    second->next->x = second->x + second_vx;
    second->next->y = second->y + second_vy;
    second = second->next;
    if (second->x < 0) {
        second->x = 0;
        second_vx = -second_vx;
    } else if (second->x > glutGet(GLUT_WINDOW_WIDTH)) {
        second->x = glutGet(GLUT_WINDOW_WIDTH);
        second_vx = -second_vx;
    }
    if (second->y < 0) {
        second->y = 0;
        second_vy = -second_vy;
    } else if (second->y > glutGet(GLUT_WINDOW_HEIGHT)) {
        second->y = glutGet(GLUT_WINDOW_HEIGHT);
        second_vy = -second_vy;
    }
    
    glBegin(GL_LINES);
    for (i = 0; i < num_points; i++) {
        glVertex2i(first->x, first->y);
        glVertex2i(second->x, second->y);
        glColor3ubv(colors[(color+i) % COLORS]);
        first = first->next;
        second = second->next;
    }
    glEnd();

    color++;
    if (color >= COLORS)
        color = 0;

    glutSwapBuffers();
}

void
idle(void)
{
    glutPostRedisplay();
}

void
bail(int code)
{
    int i;
    point* nuke;

    for (i = 0; i < num_points; i++) {
        nuke = first;
        first = first->next;
        free(nuke);
    }
    for (i = 0; i < num_points; i++) {
        nuke = second;
        second = second->next;
        free(nuke);
    }
    exit(code);
}

#ifdef SCREEN_SAVER_MODE
void
ss_keyboard(char key, int x, int y)
{
    bail(0);
}

void
ss_mouse(int button, int state, int x, int y)
{
    bail(0);
}

void
ss_passive(int x, int y)
{
    static int been_here = 0;

    /* for some reason, GLUT sends an initial passive motion callback
       when a window is initialized, so this would immediately
       terminate the program.  to get around this, see if we've been
       here before. (actually if we've been here twice.) */

    if (been_here > 1)
        bail(0);
    been_here++;
}

#else

void
keyboard(unsigned char key, int x, int y)
{
    static int old_x = 50;
    static int old_y = 50;
    static int old_width = 320;
    static int old_height = 320;

    if (key == 27) {
        bail(0);
    } else if (key == 'w') {
        glutPositionWindow(old_x, old_y);
        glutReshapeWindow(old_width, old_height);
    } else if (key == 'f') {
        if (glutGet(GLUT_WINDOW_WIDTH) != glutGet(GLUT_SCREEN_WIDTH)) {
            old_x = glutGet(GLUT_WINDOW_X);
            old_y = glutGet(GLUT_WINDOW_Y);
            old_width = glutGet(GLUT_WINDOW_WIDTH);
            old_height = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();
        }
    }
}
#endif

int
main(int argc, char** argv)
{
    int i;
    point* newborn;

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(320, 320);
    glutInit(&argc, argv);

    glutCreateWindow("Qix");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
#ifdef SCREEN_SAVER_MODE
    glutPassiveMotionFunc(ss_passive);
    glutKeyboardFunc(ss_keyboard);
    glutMouseFunc(ss_mouse);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutFullScreen(); 
#else
    glutKeyboardFunc(keyboard);
#endif

    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0) {
            fprintf(stderr, "%s [lines] [step]\n", argv[0]);
            exit(0);
        }
        sscanf(argv[1], "%d", &num_points);
        if (argc > 2)
            sscanf(argv[2], "%f", &vstep);
    }      

    first_vx *= vstep;
    first_vy *= vstep;
    second_vx *= vstep;
    second_vy *= vstep;

    for (i = 0; i < num_points; i++) {
        newborn = (point*)malloc(sizeof(point));
        newborn->next = first;
        newborn->x = -1.0;
        newborn->y = -1.0;
        first = newborn;
    }

    newborn = first;
    while(newborn->next)
        newborn = newborn->next;
    newborn->next = first;

    first->x = rand() % glutGet(GLUT_WINDOW_WIDTH);
    first->y = rand() % glutGet(GLUT_WINDOW_HEIGHT);

    for (i = 0; i < num_points; i++) {
        newborn = (point*)malloc(sizeof(point));
        newborn->next = second;
        newborn->x = -1.0;
        newborn->y = -1.0;
        second = newborn;
    }

    newborn = second;
    while(newborn->next)
        newborn = newborn->next;
    newborn->next = second;

    second->x = rand() % glutGet(GLUT_WINDOW_WIDTH);
    second->y = rand() % glutGet(GLUT_WINDOW_HEIGHT);

    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
