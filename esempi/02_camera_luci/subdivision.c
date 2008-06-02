#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     
#include <GL/glut.h>  


/* prototype */
void redraw(void);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void key(unsigned char c, int x, int y);
void controlMenu(int value);

/* moving parameters */
int moving, startx, starty;

/* parameters for animation */
GLfloat angley = 0;   /* in degrees */
GLfloat anglex = 0;   /* in degrees */


#ifndef M_PI
#define M_PI 3.14159265f
#endif

#define MENU_COMMAND_NONE 0
#define MENU_SPHERE 1
#define MENU_CYLINDER 2
#define MENU_ELICOID 3

int ndepth=5;

enum {FN_SPHERE,FN_CYLINDER,FN_ELICOID};

int fn=FN_SPHERE;

/* ---------------------------------------------------------- */
void getPoint(GLfloat dest[3],GLfloat param1,GLfloat param2)
{
	switch(fn)
	{
	case FN_SPHERE:
		param1=(2.0f*M_PI)*param1;
		param2=(M_PI)*(param2-0.5f);
		dest[0]=(GLfloat)(cos(param1)*cos(param2));
		dest[1]=(GLfloat)(sin(param1)*cos(param2));
		dest[2]=(GLfloat)(sin(param2));
		break;

	case FN_CYLINDER:
		param1=(2.0f*M_PI)*param1;
		dest[0]=(GLfloat)(cos(param1));
		dest[1]=(GLfloat)(sin(param1));
		dest[2]=(GLfloat)(param2);
		break;

	case FN_ELICOID:
		param1=(param1-0.5f)*M_PI;
		param2=(param2*2)-1;
		dest[0]=(GLfloat)(param2*cos(4*param1));
		dest[1]=(GLfloat)(param2*sin(4*param1));
		dest[2]=(GLfloat)(param1);
		break;
	}
}




/* ---------------------------------------------------------- */
void subdivide(GLfloat v1[2], GLfloat v2[2],GLfloat v3[2], int depth)
{ 
	int i;
	GLfloat v12[2], v23[2], v31[2]; 
	
	if (!depth) 
	{
		GLfloat P0[3],P1[3],P2[3];
		getPoint(P0,v1[0],v1[1]);glVertex3fv(P0);
		getPoint(P1,v2[0],v2[1]);glVertex3fv(P1);
		getPoint(P2,v3[0],v3[1]);glVertex3fv(P2);
	}
	else 
  	{
		for (i = 0; i < 2; i++) 
		{
			v12[i] = (v1[i]+v2[i])/2.0f;
			v23[i] = (v2[i]+v3[i])/2.0f;
			v31[i] = (v3[i]+v1[i])/2.0f;		
		}

		subdivide(v1 ,v12,v31 ,depth-1);
		subdivide(v31,v12,v23 ,depth-1);
		subdivide(v23,v12,v2  ,depth-1);
		subdivide(v31,v23,v3  ,depth-1); 
	}
}


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
  glutKeyboardFunc(key);

  /* create menus */
  glutCreateMenu(controlMenu);

  glutAddMenuEntry("-----------------------", MENU_COMMAND_NONE);
  glutAddMenuEntry("Sphere", MENU_SPHERE);
  glutAddMenuEntry("Cylinder", MENU_CYLINDER);
  glutAddMenuEntry("Elicoid", MENU_ELICOID);

  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  gluPerspective
  ( 
		40.0,/* field of view in degree */ 
		 1.0,/* aspect ratio *//* Z near */
		 1.0, 
		100.0/* Z far */ 
  );

  glMatrixMode(GL_MODELVIEW);
  gluLookAt(
		0.0, 0.0, 5.0,  /* eye  */
		0.0, 0.0, 0.0,  /* center  */
		1.0, 0.0, 0.0); /* up is in positive Y direction */

  glutMainLoop();

  return 0; /* ANSI C requires main to return int. */
}


/* ---------------------------------------------------------- */
void key(unsigned char c, int x, int y)
{
  if (c == 27) {
    exit(0); 
  }
  if (c=='+')
	  ndepth++;
  else if (c=='-')
	{if (ndepth>2) ndepth--;}

  glutPostRedisplay();
}



/* ---------------------------------------------------------- */
void controlMenu(int value)
{
	switch (value) 
	{
	case MENU_COMMAND_NONE:return;
	case MENU_SPHERE  :fn=FN_SPHERE;break;
	case MENU_CYLINDER:fn=FN_CYLINDER;break;
	case MENU_ELICOID :fn=FN_ELICOID;break;
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
    angley = angley - (y - starty);
    anglex = anglex + (x - startx);
    startx = x;
    starty = y;
    glutPostRedisplay();
  }
}

/* ---------------------------------------------------------- */
void redraw(void)
{
	GLfloat params[4][2]={ {0,0}, {1,0}, {0,1},{1,1} };

	glClearColor(0,0.5,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	
		glRotatef(anglex, 1.0, 0.0, 0.0);
		glRotatef(angley, 0.0, 1.0, 0.0);

		glBegin(GL_LINES);
			glColor3f(1,0,0);glVertex3f(0,0,0);glVertex3f(1,0,0);
			glColor3f(0,1,0);glVertex3f(0,0,0);glVertex3f(0,1,0);
			glColor3f(0,0,1);glVertex3f(0,0,0);glVertex3f(0,0,1);
		glEnd();

		glColor3f(0,0,0);

		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glBegin(GL_TRIANGLES);

		subdivide(params[0],params[1],params[2],ndepth);
		subdivide(params[2],params[1],params[3],ndepth);

		glEnd();
	
	glPopMatrix();
	
	glutSwapBuffers();
}



