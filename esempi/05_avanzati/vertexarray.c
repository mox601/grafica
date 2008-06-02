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

GLfloat colorwhite []  = {1.0f, 1.0f, 1.0f, 1.0f}; 
GLfloat colorgray  []  = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat colorgreen []  = {0.1f, 1.0f, 0.1f, 1.0f};
GLfloat coloryellow[]  = {1.0f, 1.0f, 0.0f, 1.0f};
GLfloat colorblue[]    = {0.0f, 0.0f, 1.0f, 1.0f};


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
	GLfloat params[4][2]={ {0,0}, {1,0}, {0,1},{1,1} };

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
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

		glDisable(GL_LIGHTING);
		

		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

		{
			static GLint vertices[]  = {
				0,0,0,
				1,0,0,
				1,1,0,
				0,1,0,
				0,0,1,
				1,0,1,
				1,1,1,
				0,1,1
			};

			static GLfloat colors[]  = {
				0,0,0,
				1,0,0,
				1,1,0,
				0,1,0,
				0,0,1,
				1,0,1,
				1,1,1,
				0,1,1
			};
			
			static GLubyte faces[6][4]=
			{
				{0,1,2,3},
				{4,5,6,7},
				{0,1,5,4},
				{1,2,6,5},
				{2,3,7,6},
				{3,0,4,7}
			};

			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);


			glColorPointer (3, GL_FLOAT, (GLsizei )0, (void*)colors);
			glVertexPointer(3, GL_INT  , (GLsizei )0, (void*)vertices);

			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, faces[0]);
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, faces[1] );
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, faces[2] );
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, faces[3] );
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, faces[4] );
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, faces[5] );

			/* oppure */
			{
				static GLubyte faces[]=
				{
					0,1,2,3,
					4,5,6,7,
					0,1,5,4,
					1,2,6,5,
					2,3,7,6,
					3,0,4,7
				};

				glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, faces);
			}

		}

	glPopMatrix();
	glutSwapBuffers();
}



