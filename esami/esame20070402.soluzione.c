/* include headers */
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>


#define USE_SOLUTION 1

/* define PIGRECO */
#define PIGRECO (3.14159265f)

/* actual operation  */
int scaling=0;
int moving =0;
int panning=0;

/* starting mouse position */
int beginx=0;
int beginy=0;

/* rotations */
GLfloat angley = 0;   
GLfloat anglez = 0;   

/* ortho for panning */
float ortho_left,ortho_right,ortho_bottom,ortho_top;

/* current scale factor (zoom in/out) */
float scalefactor=1;

/* define some colors */
GLfloat color_black     []  = {0.0f, 0.0f, 0.0f, 1.0f}; 
GLfloat color_white     []  = {1.0f, 1.0f, 1.0f, 1.0f}; 
GLfloat color_red       []  = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat color_green     []  = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat color_blue      []  = {0.0f, 0.0f, 1.0f, 1.0f};

/* I'm on x axes looking towards the origin, the VUP is (0,0,1) */
GLfloat viewer[3]={5,0,0};
GLfloat target[3]={0,0,0};
GLfloat vup   [3]={0,0,1};

/*  spot light coming from the viewer (a little bit above the viewer) */
GLfloat lightpos[4]={5,0,2,1};

/* since glut does not have a glutSolidCylinder I define it! */
void cylinder(GLint slices)
{
	GLint i;

	GLUquadric* myQuad=gluNewQuadric();
	gluCylinder( myQuad, 1, 1, 1.0, slices, 1 );
	gluDeleteQuadric(myQuad);

	/* draw top face */
	glNormal3f(0,0,1);
	glBegin(GL_POLYGON);
	for (i=0;i<slices;i++)
	{
		GLfloat angle=i*2*PIGRECO/slices;
		GLfloat X=(GLfloat)cos(angle);
		GLfloat Y=(GLfloat)sin(angle);
		glVertex3f(X,Y,1.0);
	}
	glEnd();

	/* draw bottom face */
	glNormal3f(0,0,-1);
	glBegin(GL_POLYGON);
	for (i=0;i<slices;i++)
	{
		GLfloat angle=i*2*PIGRECO/slices;
		GLfloat X=(GLfloat)cos(angle);
		GLfloat Y=(GLfloat)sin(angle);
		glVertex3f(X,Y,0.0);
	}
	glEnd();
}


#if USE_SOLUTION
/* --------------------------------------------------------------- */

int npetals=8;

void setMaterial(GLfloat R,GLfloat G,GLfloat B)
{
	GLfloat material_ambient      []  = {0.1f*R, 0.1f*G, 0.1f*B, 1.0f}; 
	GLfloat material_diffuse      []  = {0.3f*R, 0.3f*G, 0.3f*B, 1.0f}; 
	GLfloat material_specular     []  = {0.5f*R, 0.5f*G, 0.5f*B, 1.0f}; 
	GLfloat material_emission     []  = {0.0f*R, 0.0f*G, 0.0f*B, 1.0f}; 
	GLfloat material_shininess    []  = {30};
	
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT  , material_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE  , material_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR , material_specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION , material_emission);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS, material_shininess);
}


void disegnaPetali(int npetals)
{
	GLint i;
	for (i=0;i<npetals;i++)
	{
		glPushMatrix();
			glRotatef(360*(i/(GLfloat)npetals),0,0,1);
			glTranslatef(2,0,0);
			glScalef(1,0.5,0.2f);
			glTranslatef(0,0,-0.5);
			cylinder(16);	
		glPopMatrix();
	}
}

void disegnaFusto()
{
	glPushMatrix();
		glTranslatef(0,0,-6);
		glScalef(0.1f,0.1f,5);
		cylinder(16);
	glPopMatrix();
}

GLfloat rotazione_petali=0;

void idle(void)
{
	rotazione_petali+=0.1f;
	glutPostRedisplay();
}

#endif


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void keydown(unsigned char key, GLint x, GLint y) 
{
	switch(key) 
	{
		/* esc -> exit */
		case 27: 
			exit(0);
			return;

#if USE_SOLUTION
		case '+':
			npetals++;
			break;
		case '-':
			npetals--;
			if (npetals<1) npetals=0;
			break;
#endif
	}

	glutPostRedisplay();
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void display(void)
{
	/* clear the screen and the depth buffer */
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	/* reset model-view transformation */
	glLoadIdentity();
	gluLookAt(viewer[0],viewer[1],viewer[2],target[0],target[1],target[2],vup[0],vup[1],vup[2]);

	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	/* rotation of the matrix */
	glRotatef(angley, 0.0, 1.0, 0.0);
	glRotatef(anglez, 0.0, 0.0, 1.0);

	/* zoom in zoom out */
	glScalef(scalefactor,scalefactor,scalefactor);

#if USE_SOLUTION
	/* draw a sphere */
		setMaterial(1,1,0);
		glutSolidSphere(1,32,32);

	/* draw petali */
		glPushMatrix();
		glRotatef(rotazione_petali,0,0,1);
		setMaterial(1,1,1);
		disegnaPetali(npetals);
		glPopMatrix();

	/* draw fusto */
		setMaterial(0,1,0);
		disegnaFusto();
#endif

	/* cartesian axes */
	glLineWidth(3);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glColor3f(1,0,0);glVertex3f(0,0,0);glVertex3f(2,0,0);
		glColor3f(0,1,0);glVertex3f(0,0,0);glVertex3f(0,2,0);
		glColor3f(0,0,1);glVertex3f(0,0,0);glVertex3f(0,0,2);
	glEnd();
	glEnable(GL_LIGHTING);
	glLineWidth(1);

	/* since we are using double buffering */
	glutSwapBuffers();
}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void reshape(int W,int H)
{
	/* set the new viewport */
	glViewport(0,0,W,H);

	/* set the projection matrix mantaining proportions */
	ortho_left  =-5;
	ortho_right =+5;
	ortho_bottom=-(ortho_right-ortho_left)*((float)H/W)/2.0f;
	ortho_top   =-1*ortho_bottom;

	/* load the new orthographic projection in the projection matrix */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(ortho_left, ortho_right, ortho_bottom, ortho_top, -100, +100);
	glMatrixMode(GL_MODELVIEW);
	
	/* force a redisplay */
	glutPostRedisplay();
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void mouse(GLint button, GLint state, GLint x, GLint y)
{
	if (state==GLUT_UP)
	{
		/* mouse up, no motion at all */
		moving=scaling=panning=0;
		glutPostRedisplay();
		return;
	}

	switch(button)
	{
		case GLUT_RIGHT_BUTTON : scaling=1;break; /* zooming in/out with right button */
		case GLUT_LEFT_BUTTON  : moving =1;break; /* rotating the scene with left button */
		case GLUT_MIDDLE_BUTTON: panning=1;break; /* panning with middle button */
	}
	
	/* store the starting mouse position */
	beginx = x;beginy = y;
	glutPostRedisplay();
}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void motion(GLint x, GLint y)
{
	int W=glutGet(GLUT_WINDOW_WIDTH );
	int H=glutGet(GLUT_WINDOW_HEIGHT);

	float dx=(beginx-x)/(float)W;
	float dy=(y-beginy)/(float)H;

	/* if panning change the projection matrix in order to refresh the view volume */
	if (panning)
	{
		ortho_left  +=dx;
		ortho_right +=dx;
		ortho_bottom+=dy;
		ortho_top   +=dy;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(ortho_left, ortho_right, ortho_bottom, ortho_top, -10, +10);
		glMatrixMode(GL_MODELVIEW);

		beginx = x;
		beginy = y;
		glutPostRedisplay();
		return;
	}
	/* if scaling set the new scaling factor */
	else if (scaling) 
	{
		scalefactor *= (1.0f+dx);
		beginx = x;
		beginy = y;
		glutPostRedisplay();
		return;
	}
	/* if moving refresh the angles to rotate the scene */
	else if (moving) 
	{
		angley = angley + (y - beginy);
		anglez = anglez + (x - beginx);

		beginx = x;
		beginy = y;
		glutPostRedisplay();
		return;
	}
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
int main(int narg,char** args)
{
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE );
	glutInitWindowSize(800,800);
	glutCreateWindow("EsameGL");

	/* usually stuff */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glDepthFunc(GL_LESS);
	glClearColor(0.3f,0.3f,1.0f,1.0f);

	/* I have one light on the scene */
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT  , color_white);
	glLightfv(GL_LIGHT0, GL_DIFFUSE  , color_white);
	glLightfv(GL_LIGHT0, GL_SPECULAR , color_white);

	/* register GLUT callbacks */
	glutReshapeFunc (reshape);
	glutKeyboardFunc(keydown);
	glutDisplayFunc (display);
	glutMouseFunc   (mouse);
	glutMotionFunc  (motion);

#if USE_SOLUTION
	glutIdleFunc(idle);
#endif

	/* enter in the main loop */
	glutMainLoop();
	return 0;
}




