#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     
#include <GL/glut.h>
#include "vectors.h"
#include "model.h"

/* prototype */
void redraw(void);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void key(unsigned char c, int x, int y);
void controlMenu(int value);

int startx,starty;

int going_forward   = 0;
int moving_on_plane = 0;



Point3d  position ={5,0,0};
Point3d  target   ={0,0,0};
Vector3d vup      ={0,0,1};

/* dettaglio dei triangoli disegnati */
/* influisce sulla performance */
GLfloat dettaglio = 1.0f;
GLfloat dettaglioMax = 0.08f;
GLfloat dettaglioMin = 15.0f;
GLfloat stepDetail = 0.5f;


enum {M_NONE,M_LOCAL_LIGHT,M_DIRECTIONAL_LIGHT,M_WIREFRAME};

GLfloat coloryellow[]  = {1.0f, 1.0f, 0.0f, 1.0f};

int enable_light_directional=0;
/* faretto */
int enable_light_local      =1;
int draw_wireframe          =0;


/* posizione luce */
float lightAngle = 0.0;

GLfloat l_position[] = { 1.0, 1.0, 1.0, 0.0 };

/* ---------------------------------------------------------- */

/* inizializza parametri di opengl */

void init(void) {

 	/* init parametri per la luce */
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	
	GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat local_view[] = { 0.0 };

//	glClearColor(0.0, 0.1, 0.1, 0.0);
	
	/* smooth shading */
	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);

}




int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE );
	glutInitWindowSize(800,800);
	glutCreateWindow("OpenGl application");

	init();
	
	/* Register GLUT callbacks. */
	glutDisplayFunc(redraw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(key);

	/* creazione del menu per il tasto DX */
	glutCreateMenu(controlMenu);
	
	glutAddMenuEntry("-----------------------", M_NONE);

	glutAddMenuEntry("Local light"      , M_LOCAL_LIGHT);
	glutAddMenuEntry("Directional light", M_DIRECTIONAL_LIGHT);
	glutAddMenuEntry("Draw wireframe",    M_WIREFRAME);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	/* occlusione facce? */
	glEnable(GL_DEPTH_TEST);

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

/* --------------------gestione della tastiera------------------------------ */
void key(unsigned char c, int x, int y)
{
 	
	Vector3d direction_forward,direction_right,direction_up, direction_backward, direction_left;

	vector_diff(&direction_forward,&target,&position);
	vector_diff(&direction_backward,&position,&target);

	vector_cross_product(&direction_left,&vup,&direction_forward);
	vector_cross_product(&direction_right,&vup,&direction_backward);
	direction_up=vup;

switch(c) 
	{

/* w: avanti; s: indietro */
	case 'w':
		vector_scale(&direction_forward,0.05f);
		Vector3d direction_forward_x = direction_forward; 
		direction_forward_x.z = 0.0f; 
		point_translate(&position, &direction_forward_x);
		point_translate(&target, &direction_forward_x);
		glutPostRedisplay();
		break;
	
	case 's':
		vector_scale(&direction_backward,0.05f);
		Vector3d direction_backward_x = direction_backward; 
		direction_backward_x.z = 0.0f; 
		point_translate(&position, &direction_backward_x);
		point_translate(&target, &direction_backward_x);
		glutPostRedisplay();	
		break;

/* girati a dx o sx */
	case 'a':
		vector_scale(&direction_left, 0.100f);
		point_translate(&target, &direction_left);
		glutPostRedisplay();
		break;

	case 'd': 
		vector_scale(&direction_right, 0.100f);
		point_translate(&target, &direction_right);
		glutPostRedisplay();	
		break; 

/* reset della componente z del target*/
	case 'r': 
		target.z = 0.0f;
		glutPostRedisplay();	
		break; 


/* faccio spostare la luce */
/*
	case '+':
		l_position[1] = l_position[1] + 1.0f;
		printf("lpos: %f, %f, %f\n", l_position[0], l_position[1], l_position[2]);
		glutPostRedisplay();
		break; 
	case '-':
		l_position[1] = l_position[1] - 1.0f;
		printf("lpos: %f, %f, %f\n", l_position[0], l_position[1], l_position[2]);
		glutPostRedisplay();
		break; 
*/

/* spostamento di lato, SX e DX */
	case 'z':
		vector_scale(&direction_forward,0.01f);
		Vector3d direction_left_y; 
		vector_cross_product(&direction_left_y, &vup, &direction_forward);
		point_translate(&position, &direction_left_y);
		point_translate(&target, &direction_left_y);
		glutPostRedisplay();
		break;
	
	case 'x':
		vector_scale(&direction_forward,0.01f);
		Vector3d direction_right_y;
		vector_cross_product(&direction_right_y, &direction_forward, &vup);
		point_translate(&position, &direction_right_y);
		point_translate(&target, &direction_right_y);
		glutPostRedisplay();
		break;


/* aumento o diminuisco il dettaglio dei triangoli */


	case '9': 
		/*aumento il dettaglio*/
		aumentaDettaglio();
//		printf("9: dettaglio attuale %f\n", dettaglio);
		break; 

	case '0': 
		/*diminuisco il dettaglio*/
		diminuisciDettaglio();
//		printf("0: dettaglio attuale %f\n", dettaglio);
		break; 




	case 27:
		exit(0);
		break;
	}

  glutPostRedisplay();
}


/* -------------gestione del menu con tasto destro---------------- */

void controlMenu(int value)
{
	switch (value) 
	{
	case M_NONE:return;

	case M_LOCAL_LIGHT:
		enable_light_local=1-enable_light_local;
		printf("switch luce locale\n");
	break;

	case M_DIRECTIONAL_LIGHT:
		enable_light_directional=1-enable_light_directional;
		printf("switch luce direzionale\n");
	break;

	case M_WIREFRAME:draw_wireframe=1-draw_wireframe;
		printf("switch wireframe\n");
	break;

	}
	glutPostRedisplay();
}







/* ---------------------------------------------------------- */


aumentaDettaglio() {
	if (dettaglio > (dettaglioMax + stepDetail)) { 
		dettaglio = dettaglio - stepDetail;
	} else {
		dettaglio = dettaglioMax;
		}
}
diminuisciDettaglio() {
	if (dettaglio < (dettaglioMin - stepDetail)) { 
		dettaglio = dettaglio + stepDetail;
	} else {
		dettaglio = dettaglioMin;
		}
}



/* ---------------------------------------------------------- */
void redraw(void)
{

	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
	GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat no_shininess[] = { 0.0 };
	GLfloat low_shininess[] = { 5.0 };
	GLfloat high_shininess[] = { 100.0 };
	GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};




	GLfloat light_position_local[4]={
		1.0f,
		1.0f,
		1.0f,
		1.0f /* local light! */
	};




	glClear(GL_COLOR_BUFFER_BIT);


	glClear(GL_DEPTH_BUFFER_BIT);

	glClearColor(0.0, 0.0, 0.0, 0.0);



	/* per fare update della posizione e stato della luce */
	if (enable_light_local)
	{
		glLightfv(GL_LIGHT0, GL_POSITION, light_position_local);
		glEnable(GL_LIGHT0);
	}
	else
	{
		glDisable(GL_LIGHT0);
	}

	/* disegna sfera dove si trova la luce */
	glPushMatrix();
	glTranslatef(light_position_local[0],light_position_local[1],light_position_local[2]);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, coloryellow);

		if (!draw_wireframe)
			glutSolidSphere(0.3, 30, 30);
		else
			glutWireSphere(0.3, 30, 30);

	glPopMatrix();


	glPushMatrix();
	
	gluLookAt(
		position.x, position.y ,  position.z,		   
		target.x  , target.y   ,  target.z,       
		vup.x     , vup.y      ,  vup.z);


	/* assi di riferimento */

	glLineWidth(8);
	glBegin(GL_LINES);
	glColor3f(1,0,0);glVertex3f(0,0,0);glVertex3f(1,0,0);
	glColor3f(0,1,0);glVertex3f(0,0,0);glVertex3f(0,1,0);
	glColor3f(0,0,1);glVertex3f(0,0,0);glVertex3f(0,0,1);
	glEnd();
	glLineWidth(1);


	/* sfera */
	/*
	glColor3f(0.3f,0.3f,0.3f);
	if (!draw_wireframe)
		glutSolidSphere(1.0, 30, 30);
	else
		glutWireSphere(1.0, 30, 30);
	*/

	/* drawWall */

	
	Point3d  a = {0,0,0};
	Point3d  b = {0,40,0};
	Point3d  c = {0,40,20};
	Point3d  d = {0,0,20};


	GLfloat spessore = 0.4f;
	GLfloat sp_laterale = 8.0f;
	GLfloat incl_frontale = 3.0f;



	/* drawWall parametrica, prende la dimensione del lato lungo e 
	il senso dell'inclinazione (+1 o -1)*/
	 
//	drawWallSize(lunghezza, dettaglio, inclinazione);

	drawEsterni();

	/* disegna muro frontale */

//	drawFrontWall(lunghezza, dettaglio, segno_inclinazione, altezza);


	/* drawWall che disegna un muro a partire da una certa altezza, 
	senza partire dalla base */

	glTranslatef(-10.0f, 0.0f, 0.0f);

//	drawWallSizeHigh(lunghezza, dettaglio, segno_inclinazione, altezza);

	
	//drawWall(&a, &b, &c, &d, spessore, sp_laterale, incl_frontale, dettaglio);

	//glTranslatef(-20.0f, 0.0f, 0.0f);

	//drawWall(&a, &b, &c, &d, spessore, sp_laterale, -incl_frontale, dettaglio);

	
	/* triangolo */
		
//	GLfloat dettaglio = 0.05;


	Point3d  x = {2,0,0};
	Point3d  y = {0,2,0};
	Point3d  z = {0,0,2};
	
	/* do i punti in senso antiorario */
//	drawTriangle(&z, &y, &x, dettaglio);
	
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
//		going_forward = (state == GLUT_DOWN) ? 1 : 0;
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
		vector_scale(&direction_right, 0.003f*deltax);
		vector_scale(&direction_up, 0.005f*deltay);
		
		point_translate(&target, &direction_up);
		point_translate(&target, &direction_right);
		
		startx = x;starty = y;
		glutPostRedisplay();
	}
}





