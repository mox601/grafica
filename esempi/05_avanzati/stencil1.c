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


/* ---------------------------------------------------------- */
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(800,800);
	glutCreateWindow("OpenGl application");
	
	/* Register GLUT callbacks. */
	glutDisplayFunc(redraw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(key);
	
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
	
	{
		GLfloat color[4]={1,0,0,1};
		GLfloat colorwhite []  = {1.0f, 1.0f, 1.0f, 1.0f}; 
		GLfloat colorgray  []  = {0.2f, 0.2f, 0.2f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, colorgray );glEnable (GL_LIGHT0);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, colorwhite);glEnable (GL_LIGHT1);
		glEnable(GL_LIGHTING);
	}

	glutMainLoop();
	
	return 0; /* ANSI C requires main to return int. */
}


/* ---------------------------------------------------------- */
void key(unsigned char c, int x, int y)
{
  if (c == 27)
    exit(0); 

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
      startx = x;starty = y;
    }

    if (state == GLUT_UP) 
      moving = 0;
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
	glEnable( GL_LIGHTING );
	glEnable(GL_DEPTH_TEST);

	glClearColor(0,0.5,1,1);
	glClearStencil(0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	
	glPushMatrix();
	
		glRotatef(anglex, 1.0, 0.0, 0.0);
		glRotatef(angley, 0.0, 1.0, 0.0);

		glEnable( GL_STENCIL_TEST );
		// Set the stencil buffer to write a 1 in every time a pixel is written to the screen
		glStencilFunc( GL_ALWAYS, 1, 0xFFFF );
		glStencilOp  ( GL_KEEP, GL_KEEP, GL_REPLACE );

		glColor3f( 0.0f, 0.0f, 0.0f );
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glutSolidSphere(1,16,16);
		
		// Set the stencil buffer to only allow writing to the screen when the value of the stencil buffer is not 1
		glStencilFunc( GL_NOTEQUAL, 1, 0xFFFF );
		glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );

		glDisable( GL_LIGHTING );
		glLineWidth( 10.0f );
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glColor3f( 1,1,0 );
		glutSolidSphere(1,16,16);


	glPopMatrix();

	glutSwapBuffers();
}



