#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     
#include <GL/glut.h>  

/* prototype */
void redraw(void);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void idle(void);
void visible(int vis);
void key(unsigned char c, int x, int y);
void controlMenu(int value);

enum {M_NONE,M_LOCAL_LIGHT,M_DIRECTIONAL_LIGHT,M_WIREFRAME};

int moving, startx, starty;
int animation = 1;
GLfloat angley = 0;   
GLfloat anglex = 0;   

float lightAngle = 0.0;

GLfloat colorwhite []  = {1.0f, 1.0f, 1.0f, 1.0f}; 
GLfloat colorgray  []  = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat colorgreen []  = {0.1f, 1.0f, 0.1f, 1.0f};
GLfloat coloryellow[]  = {1.0f, 1.0f, 0.0f, 1.0f};
GLfloat colorblue[]    = {0.0f, 0.0f, 1.0f, 1.0f};

int enable_light_directional=0;
int enable_light_local      =0;
int draw_wireframe          =0;


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
	
	glutCreateMenu(controlMenu);
	
	glutAddMenuEntry("-----------------------", M_NONE);

	glutAddMenuEntry("Local light"      , M_LOCAL_LIGHT);
	glutAddMenuEntry("Directional light", M_DIRECTIONAL_LIGHT);
	glutAddMenuEntry("Draw wireframe",    M_WIREFRAME);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	gluPerspective( 
		40.0,  /* field of view in degree */ 
		1.0,   /* aspect ratio */ 
		1.0,   /* Z near */ 
		100.0  /* Z far */ 
	);
		
	/* sto in (0,0,5)  e guardo verso l'origine. il vettore x corrisponde a VUP */
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(
		0.0, 0.0, 5.0,		   
		0.0, 0.0, 0.0,       
		1.0, 0.0, 0.0);      
			
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, colorgray );
	glLightfv(GL_LIGHT1, GL_DIFFUSE, colorwhite);

	glEnable(GL_LIGHTING);

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
void visible(int vis)
{
	glutIdleFunc((vis==GLUT_VISIBLE && animation)?idle:NULL);
}

/* ---------------------------------------------------------- */
void controlMenu(int value)
{
	switch (value) 
	{
	case M_NONE              :return;
	case M_LOCAL_LIGHT       :enable_light_local      =1-enable_light_local      ;break;
	case M_DIRECTIONAL_LIGHT :enable_light_directional=1-enable_light_directional;break;
	case M_WIREFRAME         :draw_wireframe          =1-draw_wireframe          ;break;
	}
	glutPostRedisplay();
}


/* ---------------------------------------------------------- */
void redraw(void)
{
	GLfloat ligh_position_directional[4]={
		0,0,10,
		0 /* directional light! */
	};

	GLfloat ligh_position_local[4]={
		(GLfloat)cos(lightAngle),
		(GLfloat)sin(lightAngle),
		0.0f,
		1.0f /* local light! */
	};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	/* directional light */
	if (enable_light_directional)
	{
		glLightfv(GL_LIGHT0, GL_POSITION, ligh_position_directional);
		glEnable (GL_LIGHT0);
	}
	else
	{
		glDisable (GL_LIGHT0);
	}
	
	glRotatef(anglex, 1.0, 0.0, 0.0);
	glRotatef(angley, 0.0, 1.0, 0.0);

	if (enable_light_local)
	{
		glLightfv(GL_LIGHT1, GL_POSITION, ligh_position_local);
		glEnable(GL_LIGHT1);
	}
	else
	{
		glDisable(GL_LIGHT1);
	}

	/* base quad with normal=z vector */
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorblue);
	glNormal3f(0,0,1);

	glBegin(draw_wireframe?GL_LINE_LOOP:GL_QUADS);
	glVertex3f(-1,-1,-1);
	glVertex3f(+1,-1,-1);
	glVertex3f(+1,+1,-1);
	glVertex3f(-1,+1,-1);
	glEnd();

	/* draw the yellow local light (which is rotating around main ball) */
	glPushMatrix();
		glTranslatef(ligh_position_local[0],ligh_position_local[1],ligh_position_local[2]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, coloryellow);

		if (!draw_wireframe)
			glutSolidSphere(0.05, 30, 30);
		else
			glutWireSphere(0.05, 30, 30);

	glPopMatrix();

	/* draw a green-main-ball */
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colorgreen);

	if (!draw_wireframe)
		glutSolidSphere(0.2, 30, 30);
	else
		glutWireSphere(0.2, 30, 30);
		
	glPopMatrix();
	
	glutSwapBuffers();
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
    anglex += (x - startx);
    angley -= (y - starty);
    startx = x;
    starty = y;
    glutPostRedisplay();
  }
}

/* ---------------------------------------------------------- */
void idle(void)
{
	float time = 0.0;
	
	if (!moving)
	{
		glutPostRedisplay();
		lightAngle += 0.03f;
	}
}




