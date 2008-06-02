#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     
#include <GL/glut.h>  


#ifndef M_PI
#define M_PI 3.14159265
#endif


/* prototype */
static void redraw(void);
static void mouse(int button, int state, int x, int y);
static void motion(int x, int y);
static void key(unsigned char c, int x, int y);
static void controlMenu(int value);


void makeFloorTexture(void);

enum {
  M_NONE
};


int moving, startx, starty;
GLfloat anglex = 0;   
GLfloat angley = 0;   


static char *circles[] = 
{
  "....xxxx........",
  "..xxxxxxxx......",
  ".xxxxxxxxxx.....",
  ".xxx....xxx.....",
  "xxx......xxx....",
  "xxx......xxx....",
  "xxx......xxx....",
  "xxx......xxx....",
  ".xxx....xxx.....",
  ".xxxxxxxxxx.....",
  "..xxxxxxxx......",
  "....xxxx........",
  "................",
  "................",
  "................",
  "................",
};


/* ---------------------------------------------------------- */
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(800,800);
	glutCreateWindow("OpenGl application");
	
	glutDisplayFunc(redraw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(key);
	
	glutCreateMenu(controlMenu);
	
	glutAddMenuEntry("-----------------------", M_NONE);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	glEnable(GL_DEPTH_TEST);
	glLineWidth(3.0);
	
	glMatrixMode(GL_PROJECTION);
	gluPerspective( 
		/* field of view in degree */ 40.0,
		/* aspect ratio */ 1.0,
		/* Z near */ 1.0, /* Z far */ 100.0);
		
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(
		0.0, 0.0, 10.0,  /* eye  */
		0.0, 0.0, 0.0,  /* center  */
		1.0, 0.0, 0.0); /* up is in positive Y direction */
	
	makeFloorTexture();

	glutMainLoop();
	return 0; /* ANSI C requires main to return int. */
}


void makeFloorTexture(void)
{
	GLubyte floorTexture[16][16][3];
	GLubyte *loc;
	int s, t;
	
	/* Setup RGB image for the texture. */
	loc = (GLubyte*) floorTexture;
	for (t = 0; t < 16; t++) 
	{
		for (s = 0; s < 16; s++) 
		{
			if (circles[t][s] == 'x') 
			{
				/* Nice green. */
				loc[0] = 0x1f;
				loc[1] = 0x8f;
				loc[2] = 0x1f;
			} else {
				/* Light gray. */
				loc[0] = 0xaa;
				loc[1] = 0xaa;
				loc[2] = 0xaa;
			}
			loc += 3;
		}
	}
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 16, 16, 0,GL_RGB, GL_UNSIGNED_BYTE, floorTexture);
}


/* ---------------------------------------------------------- */
static void key(unsigned char c, int x, int y)
{
  if (c == 27) exit(0); 
  glutPostRedisplay();
}


/* ---------------------------------------------------------- */
static void controlMenu(int value)
{
	switch (value) 
	{
		case M_NONE:return;	
	}
	glutPostRedisplay();
}


/* ---------------------------------------------------------- */
static void redraw(void)
{
	glClearColor(0,0.4,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	
		glRotatef(anglex, 1.0, 0.0, 0.0);
		glRotatef(angley, 0.0, 1.0, 0.0);

		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glTexCoord2f( 0.0,  0.0);glVertex3f(-1,-1,0);
			glTexCoord2f( 0.0, 16.0);glVertex3f(+1,-1,0);
			glTexCoord2f(16.0, 16.0);glVertex3f(+1,+1,0);
			glTexCoord2f(16.0,  0.0);glVertex3f(-1,+1,0);
		glEnd();
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	
	glutSwapBuffers();
}

/* ---------------------------------------------------------- */
static void mouse(int button, int state, int x, int y)
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
static void motion(int x, int y)
{
  if (moving) 
  {
    anglex = anglex  + (x - startx);
    angley = angley + (y - starty);
    startx = x;starty = y;
    glutPostRedisplay();
  }
}



