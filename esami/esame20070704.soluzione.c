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

#ifdef WIN32
#pragma warning(disable: 4305 4244)
#endif

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

/* I'm on x axes looking towards the origin, the VUP is (0,0,1) */
GLfloat viewer[3]={5,0,0};
GLfloat target[3]={0,0,0};
GLfloat vup   [3]={0,0,1};


/* ------------------------------------------ */

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




/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void setColor(GLfloat R,GLfloat G,GLfloat B)
{
	GLfloat material_Ka[]={0.21,0.21,0.21,1};
	GLfloat material_Kd[]={R,G,B,1};
	GLfloat material_Ks[]={0.33,0.33,0.52,1};
	GLfloat material_Ke[]={0,0,0,0};
	GLfloat material_Se=10;

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT ,material_Ka);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE ,material_Kd);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,material_Ks);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,material_Ke);
	glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,material_Se);
}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void setLight()
{
	GLfloat light_pos[]={4,4,4,1};

	GLfloat light_Ka[]={ 0.2,0.2,0.2,1};
	GLfloat light_Kd[]={ 1,1,1,1};
	GLfloat light_Ks[]={ 1,1,1,1};
	glLightfv(GL_LIGHT0,GL_POSITION,light_pos);
	glLightfv(GL_LIGHT0,GL_AMBIENT ,light_Ka);
	glLightfv(GL_LIGHT0,GL_DIFFUSE ,light_Kd);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light_Ks);

	glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.02f);
	glEnable (GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
}



/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

float dimx=1;
float dimy=2;
float dimz=1;



#if USE_SOLUTION

void disegnaBox(float x,float y,float z,
					 float dimx,float dimy,float dimz,
					 float angley,
					 float R,float G,float B)
{
	setColor(R,G,B);
	glPushMatrix();
		glTranslatef(x,y,z);
		glRotatef(angley,0,1,0);
		glScalef(dimx,dimy,dimz);
		glTranslatef(0.5f,0.5f,0.5f);
		glutSolidCube(1);
	glPopMatrix();	
}

#endif


void disegnaModello()
{
	/* inserire qui la soluzione che vede essere parametrica */

#if USE_SOLUTION

	int t;
	int ntasti=52;

	disegnaBox(0.0f,0.0f,0.0f,  dimx/2,dimy,dimz,       0              ,1,0,0);
	disegnaBox(dimx/2,0,dimz/2-dimz/10,      dimx/2,dimy,dimz/10,  0   ,0,1,0);

	disegnaBox(dimx-dimx/10   ,0           ,0,      dimx/10,dimy/20,dimz/2-dimz/10,  0  ,0,0,1);
	disegnaBox(dimx-dimx/10   ,dimy-dimy/20,0,      dimx/10,dimy/20,dimz/2-dimz/10,  0  ,0,0,1);

	disegnaBox(dimx/2,dimy/2          ,dimz/20,  dimx/5,dimy/20,dimz/20,  10*(rand() % 2), 1,1,0);
	disegnaBox(dimx/2,dimy/2-2*dimy/20,dimz/20,  dimx/5,dimy/20,dimz/20,  10*(rand() % 2), 1,1,0);
	disegnaBox(dimx/2,dimy/2+2*dimy/20,dimz/20,  dimx/5,dimy/20,dimz/20,  10*(rand() % 2), 1,1,0);

	for (t=0;t<ntasti;t++)
	{
		float dimtasto=dimy/ntasti;
		int angle1=3*(rand() % 2);
		int angle2=10*(rand() % 2);

		glPushMatrix();

		disegnaBox(dimx/2,t*dimtasto,dimz/2,
					  dimx/2,dimtasto-dimtasto/5,dimz/20,
					  angle1,
					  1,1,1);

		glPopMatrix();

		if (t!=(ntasti-1))
		{
			disegnaBox(dimx/2,t*dimtasto+dimtasto/2,dimz/2+dimz/20,
					  dimx/4,dimtasto-dimtasto/5  ,dimz/20,
					  angle2,
					  0.4,0.4,0.4);
		}
	}

	glutPostRedisplay();

#endif


}




/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void display(void)
{
	/* clear the screen and the depth buffer */
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(viewer[0],viewer[1],viewer[2],target[0],target[1],target[2],vup[0],vup[1],vup[2]);

	/* rotation of the matrix */
	glRotatef(angley, 0.0, 1.0, 0.0);
	glRotatef(anglez, 0.0, 0.0, 1.0);

	/* zoom in zoom out */
	glScalef(scalefactor,scalefactor,scalefactor);

	/* cartesian axes */
	glLineWidth(3);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glColor3f(1,0,0);glVertex3f(0,0,0);glVertex3f(1,0,0);
		glColor3f(0,1,0);glVertex3f(0,0,0);glVertex3f(0,1,0);
		glColor3f(0,0,1);glVertex3f(0,0,0);glVertex3f(0,0,1);
	glEnd();

	glEnable(GL_LIGHTING);
	glLineWidth(1);
	setLight();

	disegnaModello();

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
void keydown(unsigned char key, GLint x, GLint y) 
{
	switch(key) 
	{
		/* esc -> exit */
		case 27: 
			exit(0);
			return;

		case 'x':dimx-=0.1;break;
		case 'X':dimx+=0.1;break;
		case 'y':dimy-=0.1;break;
		case 'Y':dimy+=0.1;break;
		case 'z':dimz-=0.1;break;
		case 'Z':dimz+=0.1;break;
	}

	glutPostRedisplay();
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

	/* register GLUT callbacks */
	glutReshapeFunc (reshape);
	glutKeyboardFunc(keydown);
	glutDisplayFunc (display);
	glutMouseFunc   (mouse);
	glutMotionFunc  (motion);

	setLight();

	/* enter in the main loop */
	glutMainLoop();
	return 0;
}




