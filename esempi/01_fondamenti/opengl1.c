#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     
#include <GL/glut.h>  

#ifndef M_PI
#define M_PI 3.14159265
#endif

#define MENU_COMMAND_NONE 0

/* prototype */
void redraw(void);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void idle(void);
void visible(int vis);
void key(unsigned char c, int x, int y);
void controlMenu(int value);

/* moving parameters */
int moving, startx, starty;

/* if animating or not */
int DoAnimation = 1;

/* parameters for animation */
float jump = 0.0;
GLfloat angley = 0;   /* in degrees */
GLfloat anglex = 0;   /* in degrees */


/* ---------------------------------------------------------- */
int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize(800,800);
  glutCreateWindow("OpenGl application");

  /* Register GLUT callbacks. */
  glutDisplayFunc(redraw);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutVisibilityFunc(visible);
  glutKeyboardFunc(key);

  /* create menus */
  glutCreateMenu(controlMenu);

  glutAddMenuEntry("-----------------------", MENU_COMMAND_NONE);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glEnable(GL_DEPTH_TEST);
  glLineWidth(3.0);

  glMatrixMode(GL_PROJECTION);
  gluPerspective
  ( 
		40.0,/* field of view in degree */ 
		 1.0,/* aspect ratio *//* Z near */
		 20.0, 
		100.0/* Z far */ 
  );

  glMatrixMode(GL_MODELVIEW);
  gluLookAt(
		0.0, 8.0, 60.0,  /* eye  */
		0.0, 4.0, 0.0,      /* center  */
		0.0, 1.0, 0.);      /* up is in positive Y direction */

  glutMainLoop();

  return 0; /* ANSI C requires main to return int. */
}


/* ---------------------------------------------------------- */
void key(unsigned char c, int x, int y)
{
  if (c == 27) {
    exit(0); 
  }
  glutPostRedisplay();
}


/* ---------------------------------------------------------- */
void visible(int vis)
{
	if (vis == GLUT_VISIBLE) 
	{
		if (DoAnimation)
			glutIdleFunc(idle);
	} 
	else 
	{
		if (!DoAnimation)
			glutIdleFunc(NULL);
	}
}

/* ---------------------------------------------------------- */
void controlMenu(int value)
{
	switch (value) 
	{
	case MENU_COMMAND_NONE:
		return;
		
	}
	glutPostRedisplay();
}


/* ---------------------------------------------------------- */
void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON) 
  {
    if (state == GLUT_DOWN) 
	 {
      moving = 1;
      startx = x;
      starty = y;
    }

    if (state == GLUT_UP) 
	 {
      moving = 0;
    }
  }
}

/* ---------------------------------------------------------- */
void motion(int x, int y)
{
  if (moving) 
  {
	 /* since y goes up... */
    angley = angley + (x - startx);
    anglex = anglex + (y - starty);
    startx = x;
    starty = y;
    glutPostRedisplay();
  }
}

/* ---------------------------------------------------------- */
void idle(void)
{
	static float time = 0.0;
	
	if (!moving)
	{
		time = glutGet(GLUT_ELAPSED_TIME) / 500.0;
		jump = 4.0 * fabs(sin(time)*0.5);
		glutPostRedisplay();
	}
}


/* ---------------------------------------------------------- */
void redraw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	
		glRotatef(anglex, 1.0, 0.0, 0.0);
		glRotatef(angley, 0.0, 1.0, 0.0);

		glBegin(GL_LINES);
			glColor3f(1,0,0);glVertex3f(0,0,0);glVertex3f(1,0,0);
			glColor3f(0,1,0);glVertex3f(0,0,0);glVertex3f(0,1,0);
			glColor3f(0,0,1);glVertex3f(0,0,0);glVertex3f(0,0,1);
		glEnd();

		
		glColor3f(0.2f,0.2f,0.2f); /* gray */
		glBegin(GL_QUADS );
			glVertex3f(-10,-5,-10);
			glVertex3f(+10,-5,-10);
			glVertex3f(+10,-5,+10);
			glVertex3f(-10,-5,+10);
		glEnd();
		
		glColor3f(1.0f,1.0f,1.0f); /* gray */
		glPushMatrix();
			glTranslatef(0.0, jump, 0.0);
			glutWireCube(10);
		glPopMatrix();
	
	glPopMatrix();
	
	glutSwapBuffers();
}



