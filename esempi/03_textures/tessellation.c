#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     
#include <GL/glut.h>  

#ifdef WIN32
#pragma warning(disable:4244)
#endif


static void redraw(void);
static void mouse(int button, int state, int x, int y);
static void motion(int x, int y);
static void key(unsigned char c, int x, int y);

GLUtriangulatorObj *tobj=NULL;

static int moving, startx, starty;

GLfloat anglex = -150;   /* in degrees */
GLfloat angley = 30;   /* in degrees */


static GLdouble bodyWidth = 3.0;

static GLfloat skinColor[]  = {0.1f, 1.0f, 0.1f, 1.0f};
static GLfloat lightColor[] = {0.8f, 1.0f, 0.8f, 1.0f}; 

static GLfloat body[][2] = 
{ 
	{0, 3}, {1, 1}, {5, 1}, {8, 4}, {10, 4}, {11, 5},
	{11, 11.5}, {13, 12}, {13, 13}, {10, 13.5}, {13, 14}, {13, 15}, {11, 16},
	{8, 16}, {7, 15}, {7, 13}, {8, 12}, {7, 11}, {6, 6}, {4, 3}, {3, 2},{1, 2} 
};

static GLfloat arm[][2] = 
{ 
	{8, 10}, {9, 9}, {10, 9}, {13, 8}, {14, 9}, {16, 9},
	{15, 9.5}, {16, 10}, {15, 10}, {15.5, 11}, {14.5, 10}, {14, 11}, {14, 10},
	{13, 9}, {11, 11}, {9, 11} 
};

static GLfloat leg[][2] = 
{ 
	{8, 6}, {8, 4}, {9, 3}, {9, 2}, {8, 1}, {8, 0.5}, {9, 0},
	{12, 0}, {10, 1}, {10, 2}, {12, 4}, {11, 6}, {10, 7}, {9, 7} 
};

static GLfloat eye[][2] = 
{ 
	{8.75f, 15}, {9, 14.7f}, {9.6f, 14.7f}, {10.1f, 15},{9.6f, 15.25f}, {9, 15.25f} 
};


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
	
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	gluPerspective( 
		/* field of view in degree */ 40.0,
		/* aspect ratio */ 1.0,
		/* Z near */ 20.0, /* Z far */ 100.0);
		
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, 0, 60.0,  /* eye  */
		0.0, 0.0, 0.0,      /* center  */
		0.0, 1.0, 0.);      /* up is in positive Y direction */
	
	tobj = gluNewTess();  /* create and initialize a GLU polygon * * tesselation object */
	gluTessCallback(tobj, GLU_BEGIN  , glBegin);
	gluTessCallback(tobj, GLU_VERTEX , glVertex2fv);  /* semi-tricky */
	gluTessCallback(tobj, GLU_END    , glEnd);

	glShadeModel(GL_FLAT);
	
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, skinColor);

	glutMainLoop();
	return 0; 
}

/* ---------------------------------------------------------- */

void extrude(GLfloat data[][2], unsigned int dataSize,GLfloat thickness)
{
	GLdouble vertex[3], dx, dy, len;
	int i;
	int numpoints = dataSize / (2 * sizeof(GLfloat));
	
	/* bordi che congiungono la faccia base e quella estrusa */
	glFrontFace(GL_CW);

	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= numpoints; i++) 
	{
		glVertex3f(data[i % numpoints][0], data[i % numpoints][1], 0.0      );
		glVertex3f(data[i % numpoints][0], data[i % numpoints][1], thickness);

		dx = data[(i + 1) % numpoints][1] - data[i % numpoints][1];
		dy = data[i % numpoints][0] - data[(i + 1) % numpoints][0];
		len = sqrt(dx * dx + dy * dy);
		glNormal3f(dx / len, dy / len, 0.0);
	}
	glEnd();

	
	/* faccia base non estrusa  */
	glNormal3f(0.0, 0.0, -1.0);  
	{
		gluBeginPolygon(tobj);
		
		for (i = 0; i < numpoints; i++) 
		{
			vertex[0] = data[i][0];
			vertex[1] = data[i][1];
			vertex[2] = 0;
			gluTessVertex(tobj, vertex, data[i]);
		}
		gluEndPolygon(tobj);
	}

	/* faccia base estrusa di tickness */
	glPushMatrix();

		glTranslatef(0.0, 0.0, thickness);
		glFrontFace(GL_CCW);
		glNormal3f(0.0, 0.0, 1.0);  /* opposite normal for other side */
		 
		gluBeginPolygon(tobj);
		
		for (i = 0; i < numpoints; i++) 
		{
			vertex[0] = data[i][0];
			vertex[1] = data[i][1];
			vertex[2] = 0;
			gluTessVertex(tobj, vertex, data[i]);
		}
		gluEndPolygon(tobj);
		
	glPopMatrix();
}


/* ---------------------------------------------------------- */
static void key(unsigned char c, int x, int y)
{
  if (c == 27) {
    exit(0); 
  }
  glutPostRedisplay();
}

/* ---------------------------------------------------------- */
static void redraw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	
		glRotatef(anglex, 1.0, 0.0, 0.0);
		glRotatef(angley, 0.0, 1.0, 0.0);
		
		glColor3f(1.0f,1.0f,1.0f);
		glPushMatrix();

			 glPushMatrix();

				glTranslatef(-6, -5, +bodyWidth / 2-3);
				extrude(body, sizeof(body) , bodyWidth);

				glTranslatef(0.0, 0.0, bodyWidth);
				extrude(arm , sizeof(arm)  , bodyWidth / 4);
				extrude(leg , sizeof(leg)  , bodyWidth / 2);

				glTranslatef(0.0, 0.0, -bodyWidth - bodyWidth / 4);
				extrude(arm , sizeof(arm)  , bodyWidth / 4);

				glTranslatef(0.0, 0.0, -bodyWidth / 4);
				extrude(leg , sizeof(leg)  , bodyWidth / 2);

				glTranslatef(0.0, 0.0, bodyWidth / 2 - 0.1);
				extrude(eye , sizeof(eye)  , bodyWidth + 0.2);

			glPopMatrix();
		
		glPopMatrix();

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
    angley = angley + (x - startx);
    anglex = anglex + (y - starty);
    startx = x;starty = y;
    glutPostRedisplay();
  }
}




