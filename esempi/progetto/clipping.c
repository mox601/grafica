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

int startx,starty;

int going_forward   = 0;
int moving_on_plane = 0;

typedef struct {GLfloat x,y,z;} Point3d;
typedef struct {GLfloat x,y,z;} Vector3d;


void point_translate(Point3d* point,const Vector3d* v)
{
	point->x+=v->x;
	point->y+=v->y;
	point->z+=v->z;
}

void vector_normalize(Vector3d* v)
{
	GLfloat m=(GLfloat)sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
	v->x/=m;
	v->y/=m;
	v->z/=m;
}

void vector_scale(Vector3d* v,GLfloat coeff)
{
	v->x *= coeff;
	v->y *= coeff;
	v->z *= coeff;
}

void vector_diff(Vector3d* dest,const Point3d* u,const Point3d* v)
{
	dest->x=u->x-v->x;
	dest->y=u->y-v->y;
	dest->z=u->z-v->z;
}

void vector_cross_product(Vector3d* dest,const Vector3d* u,const Vector3d* v)
{
	dest->x=(u->y*v->z) - (u->z*v->y);
	dest->y=(u->z*v->x) - (u->x*v->z);
	dest->z=(u->x*v->y) - (u->y*v->x);
	vector_normalize(dest);
}


Point3d  position ={5,0,0};
Point3d  target   ={0,0,0};
Vector3d vup      ={0,0,1};

/* ---------------------------------------------------------- */
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE );
	glutInitWindowSize(800,800);
	glutCreateWindow("OpenGl application");
	
	/* Register GLUT callbacks. */
	glutDisplayFunc(redraw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(key);
	
	glMatrixMode(GL_PROJECTION);
	gluPerspective( 
		40.0,  /* field of view in degree */ 
		1.0,   /* aspect ratio */ 
		1.0,   /* Z near */ 
		100.0  /* Z far */ 
	);

	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();
	return 0; 
}

/* ---------------------------------------------------------- */
void key(unsigned char c, int x, int y)
{
  if (c == 27) exit(0); 
  glutPostRedisplay();
}


/* ---------------------------------------------------------- */
void redraw(void)
{
	GLdouble eqn1[4] = {0.0, 1.0, 0.0, 0.0};
	GLdouble eqn2[4] = {1.0, 0.0, 0.0, 0.0}; 

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT );

	glPushMatrix();
	
	gluLookAt(
		position.x, position.y ,  position.z,		   
		target.x  , target.y   ,  target.z,       
		vup.x     , vup.y      ,  vup.z);



	glLineWidth(4);
	glBegin(GL_LINES);
	glColor3f(1,0,0);glVertex3f(0,0,0);glVertex3f(1,0,0);
	glColor3f(0,1,0);glVertex3f(0,0,0);glVertex3f(0,1,0);
	glColor3f(0,0,1);glVertex3f(0,0,0);glVertex3f(0,0,1);
	glEnd();
	glLineWidth(1);

	/* clip lower half -- y < 0 */
	glClipPlane (GL_CLIP_PLANE0, eqn1);
	glEnable (GL_CLIP_PLANE0);

	/* clip left half -- x < 0 */
	glClipPlane (GL_CLIP_PLANE1, eqn2);
	glEnable (GL_CLIP_PLANE1);

	glColor3f(0.3f,0.3f,0.3f);
	glutWireSphere(1.0, 30, 30);

	glDisable (GL_CLIP_PLANE0);
	glDisable (GL_CLIP_PLANE1);

	glPopMatrix();
	glutSwapBuffers();
}

/* ---------------------------------------------------------- */
void mouse(int button, int state, int x, int y)
{
	startx = x;
	starty = y;

	if (button == GLUT_RIGHT_BUTTON) 
	{
		going_forward = (state == GLUT_DOWN) ? 1 : 0;
	}
	else if (button == GLUT_LEFT_BUTTON) 
	{
		moving_on_plane = (state == GLUT_DOWN) ? 1 : 0;
	}
}

/* ---------------------------------------------------------- */
void motion(int x, int y)
{
	int deltax = (x - startx);
	int deltay = (y - starty);
	
	Vector3d direction_forward,direction_right,direction_up;

	vector_diff(&direction_forward,&target,&position);
	vector_cross_product(&direction_right,&vup,&direction_forward);
	direction_up=vup;
	
	if (going_forward) 
	{
		vector_scale(&direction_forward,deltay*0.01f);
		
		point_translate(&position,&direction_forward);
		point_translate(&target  ,&direction_forward);
		
		startx = x;starty = y;
		glutPostRedisplay();
	}
	else if (moving_on_plane)
	{		
		vector_scale(&direction_right,0.001f*deltax);
		vector_scale(&direction_up   ,0.005f*deltay);

		point_translate(&position,&direction_up);
		point_translate(&target  ,&direction_up);
		
		point_translate(&position,&direction_right);
		point_translate(&target  ,&direction_right);
		
		startx = x;starty = y;
		glutPostRedisplay();
	}
}





