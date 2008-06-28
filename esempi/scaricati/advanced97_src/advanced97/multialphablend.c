/*
 *  multialphablend.c
 *  Celeste Fowler, 1997
 *
 *  An example of using alpha-blending to render multiple transparent
 *  objects (with sorting).
 *
 */

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#ifdef _WIN32
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif

GLUquadricObj *cone, *base, *qsphere;

#ifdef _WIN32
#define trunc(x) ((double)((int)(x)))

int
gettimeofday(struct timeval* tp)
{
  struct timeb tb;

  ftime(&tb);
  tp->tv_sec = tb.time;
  tp->tv_usec = tb.millitm * 1000;

  /* 0 indicates that the call succeeded. */
  return 0;
}
#endif

void init(void)
{
  static GLfloat lightpos[] = {.5, .75, 1.5, 1};

  glEnable(GL_DEPTH_TEST); 
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

  cone = gluNewQuadric();
  base = gluNewQuadric();
  qsphere = gluNewQuadric();

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void reshape(GLsizei w, GLsizei h) 
{
  glViewport(0, 0, w, h);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, 1, .01, 10);
  gluLookAt(0, 0, 2.577, 0, 0, -5, 0, 1, 0);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void draw_room(void)
{
  /* material for the walls, floor, ceiling */
  static GLfloat wall_mat[] = {1.f, 1.f, 1.f, 1.f};

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wall_mat);

  glBegin(GL_QUADS);
  
  /* floor */
  glNormal3f(0, 1, 0);
  glVertex3f(-1, -1, 1);
  glVertex3f(1, -1, 1);
  glVertex3f(1, -1, -1);
  glVertex3f(-1, -1, -1);

  /* ceiling */
  glNormal3f(0, -1, 0);
  glVertex3f(-1, 1, -1);
  glVertex3f(1, 1, -1);
  glVertex3f(1, 1, 1);
  glVertex3f(-1, 1, 1);  

  /* left wall */
  glNormal3f(1, 0, 0);
  glVertex3f(-1, -1, -1);
  glVertex3f(-1, -1, 1);
  glVertex3f(-1, 1, 1);
  glVertex3f(-1, 1, -1);

  /* right wall */
  glNormal3f(-1, 0, 0);
  glVertex3f(1, 1, -1);
  glVertex3f(1, 1, 1);
  glVertex3f(1, -1, 1);
  glVertex3f(1, -1, -1);

  /* far wall */
  glNormal3f(0, 0, 1);
  glVertex3f(-1, -1, -1);
  glVertex3f(1, -1, -1);
  glVertex3f(1, 1, -1);
  glVertex3f(-1, 1, -1);

  glEnd();
}

void draw_cone(void)
{
  static GLfloat cone_mat[] = {0.f, .5f, 1.f, .5f};

  glPushMatrix();
  glTranslatef(0, -1, 0);
  glRotatef(-90, 1, 0, 0);

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cone_mat);

  /* base is coplanar with floor, so turn off depth testing */
  glDisable(GL_DEPTH_TEST);
  gluDisk(base, 0., .3, 20, 1); 
  glEnable(GL_DEPTH_TEST);

  gluCylinder(cone, .3, 0, 1.25, 20, 1);

  glPopMatrix();
}

void draw_sphere(GLdouble angle)
{
  static GLfloat sphere_mat[] = {1.f, .5f, 0.f, .5f};

  glPushMatrix();
  glTranslatef(0, -.3, 0);
  glRotatef(angle, 0, 1, 0);
  glTranslatef(.6, 0, 0);

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sphere_mat);
  gluSphere(qsphere, .3, 20, 20);

  glPopMatrix();
}

GLdouble get_secs(void)
{
  struct timeval t;
  gettimeofday(&t);
  return (t.tv_sec % 60) + t.tv_usec / 1000000.;
}

void draw(void)
{
    GLenum err;
    GLdouble secs, degrees;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* one revolution every 10 seconds... */
    secs = get_secs();
    secs = secs - 10.*trunc(secs / 10.);
    degrees = (secs/10.) * (360.);

    draw_room();

    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    if (degrees < 180) {
      /* sphere behind cone */
      glCullFace(GL_FRONT);
      draw_sphere(degrees);
      draw_cone();
      glCullFace(GL_BACK);
      draw_sphere(degrees);
      draw_cone();
    } else {
      /* cone behind sphere */
      glCullFace(GL_FRONT);
      draw_cone();
      draw_sphere(degrees);
      glCullFace(GL_BACK);
      draw_cone();
      draw_sphere(degrees);
    }
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    err = glGetError();
    if (err != GL_NO_ERROR) printf("Error:  %s\n", gluErrorString(err));

    glutSwapBuffers();
}

void key(unsigned char key, int x, int y)
{
  static int idle = 1;
  if (key == 27) exit(0);
  idle = (idle == 0);
  if (idle) {
    glutIdleFunc(draw);
  } else {
    glutIdleFunc(0);
  }

}

void visible(int state)
{
    if (state == GLUT_VISIBLE)
	glutIdleFunc(draw);
    else
	glutIdleFunc(NULL);
}

int main(int argc, char *argv[])
{
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(512, 512);
    glutInit(&argc, argv);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(draw);
    glutIdleFunc(draw);
    glutKeyboardFunc(key);
    glutReshapeFunc(reshape);
    glutVisibilityFunc(visible);

    init();
    glutMainLoop();
    return 0;
}
