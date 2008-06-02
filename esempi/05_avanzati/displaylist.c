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


/* ---------------------------------------------------------- */
void getPointOnTorus(GLfloat point[3],GLfloat normal[3],
							GLfloat param1,GLfloat param2)
{
	const GLfloat radius =1.0f ;
	const GLfloat section=0.2f;

	GLfloat u=(2.0f*M_PI)*param1;
	GLfloat v=(2.0f*M_PI)*param2;

	point[0]=(GLfloat)((radius+section*cos(v))*cos(u));
	point[1]=(GLfloat)((radius+section*cos(v))*sin(u));
	point[2]=(GLfloat)((section*sin(v)));

	normal[0]=(GLfloat)((section*cos(v))*cos(u));
	normal[1]=(GLfloat)((section*cos(v))*sin(u));
	normal[2]=(GLfloat)((section*sin(v)));
}


/* ---------------------------------------------------------- */
void subdivide(GLfloat v1[2], GLfloat v2[2],GLfloat v3[2], int depth)
{ 
	int i;
	GLfloat v12[2], v23[2], v31[2]; 
	
	if (!depth) 
	{
		GLfloat P0[3],P1[3],P2[3],N0[3],N1[3],N2[3];
		getPointOnTorus(P0,N0,v1[0],v1[1]);glNormal3fv(N0);glVertex3fv(P0);
		getPointOnTorus(P1,N1,v2[0],v2[1]);glNormal3fv(N1);glVertex3fv(P1);
		getPointOnTorus(P2,N2,v3[0],v3[1]);glNormal3fv(N2);glVertex3fv(P2);
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

int DL=-1;

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

		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, colorgray );glEnable (GL_LIGHT0);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, colorwhite);glEnable (GL_LIGHT1);
		glEnable(GL_LIGHTING);
		

		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

		if (DL==-1)
		{
			DL=glGenLists(1);
			glNewList( DL , GL_COMPILE );
			glBegin(GL_TRIANGLES);
			subdivide(params[0],params[1],params[2],5);
			subdivide(params[2],params[1],params[3],5);
			glEnd();
			glEndList();
		}

		{
			int i;
			
#define Rnd (rand()/(float)RAND_MAX)
			
			srand(0);
			for (i=0;i<100;i++)
			{
				GLfloat color[4]={Rnd,Rnd,Rnd,1};
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
				glPushMatrix();
				glRotatef(100*M_PI*Rnd,Rnd,Rnd,Rnd);
				glScalef(0.1,0.1,0.1);
				glTranslatef(6*Rnd,6*Rnd,6*Rnd);
				glCallList(DL);
				glPopMatrix();
			}

			
		}

	glPopMatrix();
	glutSwapBuffers();
}



