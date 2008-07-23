#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     

//#include <GL/glut.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "vectors.h"
//#include "model.h"
#include "lights.h"
#include "simpleviewer.h"
#include "camera.h"

/* prototype */
void redraw(void);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void key(unsigned char c, int x, int y);
void controlMenu(int value);

int startx,starty;

int going_forward   = 0;
int moving_on_plane = 0;


GLfloat spostamentoX;
GLfloat spostamentoY;
GLfloat spostamentoZ = 0.8f;
GLfloat quotaMinimaZ = 0.8f;


Point3d  position ={-14, 11, 0.800000};
Point3d  target   ={-13.431612, 85, 17};
Vector3d vup      ={0,0,1};

/* dettaglio dei triangoli disegnati */
/* influisce sulla performance */
GLfloat dettaglio = 1.0f;
GLfloat dettaglioMax = 0.08f;
GLfloat dettaglioMin = 15.0f;
GLfloat stepDetail = 0.5f;

enum {M_NONE,M_LOCAL_LIGHT,M_DIRECTIONAL_LIGHT,M_WIREFRAME};

/* materiali */
GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat no_shininess[] = { 0.0 };
GLfloat low_shininess[] = { 5.0 };
GLfloat high_shininess[] = { 100.0 };
GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};


/* angolo per animazione luce */
float lightAngle = 0.0;

int enable_light_directional = 1;
/* faretto */
int enable_light_local = 1;
int draw_wireframe = 0;


/* ---------------------------------------------------------- */

/* inizializza parametri di opengl */

void init(void) {

	/* da sol esame */
	glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glClearColor(0.0f,0.0f,0.0f,1.0f);

	/* spostato nel file lights.c */
	setupLights();

	/* smooth shading */
	glShadeModel(GL_SMOOTH);
	/* antialiasing sulle linee */
	glEnable(GL_LINE_SMOOTH);

	/* blending per i vetri */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	//glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
	
	//fog?
	
	
	xPosition = 0.0f;
	yPosition = 0.0f;	
	zPosition = 0.0f;
	



}

void reshape(int W,int H)
{
	glViewport(0, 0, W, H);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 
		60.0,  /* field of view in degree */ 
		W/H,   /* aspect ratio */ 
		1.0,   /* Z near */ 
		200.0  /* Z far */ 
	);
	
	
	
	glMatrixMode(GL_MODELVIEW);		
	glLoadIdentity();
	gluLookAt(
		position.x, position.y ,  position.z,		   
		target.x  , target.y   ,  target.z,       
		vup.x     , vup.y      ,  vup.z);


	glutPostRedisplay();
}


void idle(void)
{
/* movimento luce */
	lightAngle += 0.01f;
	if (lightAngle > 360.f) {
		lightAngle = 0.0f;
		}
//	printf("angolo luce: %f\n", lightAngle);
	glutPostRedisplay();
}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);
	glutInitWindowSize(800,800);
	glutCreateWindow("Palazzo");

	init();
	
	/* Register GLUT callbacks. */
	glutReshapeFunc (reshape);
	glutDisplayFunc(redraw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	/* creazione del menu per il tasto DX */
	glutCreateMenu(controlMenu);
	
	glutAddMenuEntry("-----------------------", M_NONE);

	glutAddMenuEntry("Local light"      , M_LOCAL_LIGHT);
	glutAddMenuEntry("Directional light", M_DIRECTIONAL_LIGHT);
	glutAddMenuEntry("Draw wireframe",    M_WIREFRAME);
	glutAttachMenu(GLUT_RIGHT_BUTTON);



	/* caricamento dei .ply */
	loadPlyModels();





	glMatrixMode(GL_PROJECTION);
	gluPerspective( 
		60.0,  /* field of view in degree */ 
		1.0,   /* aspect ratio */ 
		1.0,   /* Z near */ 
		200.0  /* Z far */ 
	);

	glMatrixMode(GL_MODELVIEW);
	glutMainLoop();
	return 0; 
}

/* ---------------------------------------------------------- */

/* ---------------------------------------------------------- */


void aumentaDettaglio() {
	if (dettaglio > (dettaglioMax + stepDetail)) { 
		dettaglio = dettaglio - stepDetail;
	} else {
		dettaglio = dettaglioMax;
		}
}
void diminuisciDettaglio() {
	if (dettaglio < (dettaglioMin - stepDetail)) { 
		dettaglio = dettaglio + stepDetail;
	} else {
		dettaglio = dettaglioMin;
		}
}




/* --------------------gestione della tastiera------------------------------ */
void key(unsigned char c, int x, int y)
{
 	
	Vector3d direction_forward,direction_right,direction_up, direction_backward, direction_left, direction_down;

	vector_diff(&direction_forward,&target,&position);
	vector_diff(&direction_backward,&position,&target);

	vector_cross_product(&direction_left,&vup,&direction_forward);
	vector_cross_product(&direction_right,&vup,&direction_backward);

	direction_up=vup;
	direction_down=vup;
	vector_scale(&direction_down, -1);


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
		vector_scale(&direction_left, 0.50f);
		point_translate(&target, &direction_left);
		glutPostRedisplay();
		break;

	case 'd': 
		vector_scale(&direction_right, 0.50f);
		point_translate(&target, &direction_right);
		glutPostRedisplay();	
		break; 

/* reset della componente z del target */
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
		
	


/* volo */
	case '1':
		vector_scale(&direction_up,spostamentoZ);
		point_translate(&position, &direction_up);
		point_translate(&target, &direction_up);
		glutPostRedisplay();
		break;
	
	case '2':
	
	
		
		if((position.z - spostamentoZ) < quotaMinimaZ) {
			position.z = quotaMinimaZ;
		}
		
		else {
			vector_scale(&direction_down,spostamentoZ);
			point_translate(&position, &direction_down);
			point_translate(&target, &direction_down);
		}
		
		glutPostRedisplay();
		break;



/* aumento o diminuisco il dettaglio dei triangoli */


	case '9': 
		/*aumento il dettaglio*/
		aumentaDettaglio();
		break; 

	case '0': 
		/*diminuisco il dettaglio*/
		diminuisciDettaglio();
		break; 
		
		
		
		
		/* controlli per il piazzamento dei solidi */
		
	case '7': 
		/*abbassa z*/
		zPosition -= 0.1;
		printf("posizione attuale: %f %f %f\n", xPosition, yPosition, zPosition);
		glutPostRedisplay();
		break; 

	case '8': 
		/* alza z */
		zPosition += 0.1;
		printf("posizione attuale: %f %f %f\n", xPosition, yPosition, zPosition);
		glutPostRedisplay();
		break; 
		
	case 'k': 
		/*abbassa x*/
		xPosition -= 0.1;
		printf("posizione attuale: %f %f %f\n", xPosition, yPosition, zPosition);		
		break; 

	case 'i': 
		/* alza x */
		xPosition += 0.1;
		printf("posizione attuale: %f %f %f\n", xPosition, yPosition, zPosition);		
		glutPostRedisplay();
		break; 
	
	case 'j': 
		/*abbassa y*/
		yPosition -= 0.1;
		printf("posizione attuale: %f %f %f\n", xPosition, yPosition, zPosition);
		glutPostRedisplay();
		break; 

	case 'l': 
		/* alza y */
		yPosition += 0.1;
		printf("posizione attuale: %f %f %f\n", xPosition, yPosition, zPosition);		
		glutPostRedisplay();
		break;	
		
	case 'b': // switch the blending.
		printf("B/b pressed; blending is: %d\n", blend);
		blend = blend ? 0 : 1;
		printf("Blend is now: %d\n", blend);
		glutPostRedisplay();
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
		printf("luce locale = %d\n", enable_light_local);
	break;

	case M_DIRECTIONAL_LIGHT:
		enable_light_directional=1-enable_light_directional;
		printf("switch luce direzionale\n");
		printf("luce direzionale = %d\n", enable_light_directional);
	break;

	case M_WIREFRAME:draw_wireframe=1-draw_wireframe;
		printf("switch wireframe\n");
	break;

	}



	glutPostRedisplay();
}






/* ---------------------------------------------------------- */
void redraw(void)
{

	/* clear the screen and the depth buffer */
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glLoadIdentity();

	gluLookAt(
		position.x, position.y ,  position.z,		   
		target.x  , target.y   ,  target.z,       
		vup.x     , vup.y      ,  vup.z);

	/* movimento luce 0 local */
	light_position_local[0]= -30.0f + sin(lightAngle)*5;
	light_position_local[1]= -1.0f + cos(lightAngle)*5;
//	light_position_local[2]= 1.0f + cos(lightAngle)*5;

	/* per fare update della posizione e stato della luce 0 */
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
			glutSolidSphere(0.3, 10, 10);
		else
			glutWireSphere(0.3, 10, 10);

	glPopMatrix();



	/* luce 1, una luce direzionale */


if (enable_light_directional)
	{
		glLightfv(GL_LIGHT1, GL_POSITION, light_position_directional);
		glEnable(GL_LIGHT1);
	}
	else
	{
		glDisable(GL_LIGHT1);
	}


	/* disegna sfera dove si trova la luce direzionale */
	glPushMatrix();
	glTranslatef(light_position_directional[0],light_position_directional[1],light_position_directional[2]);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, coloryellow);

		if (!draw_wireframe)
			glutSolidSphere(0.3, 10, 10);
		else
			glutWireSphere(0.3, 10, 10);

	glPopMatrix();
	





	
	/* disegna sfera dove si trova la luce LIGHT_2 */
	
	//glLightfv(GL_LIGHT2, GL_POSITION, light_position_lampadario);

	
	glPushMatrix();
	glTranslatef(light_position_lampadario[0], light_position_lampadario[1], light_position_lampadario[2]);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, coloryellow);

		if (!draw_wireframe)
			glutSolidSphere(0.3, 10, 10);
		else
			glutWireSphere(0.3, 10, 10);

	glPopMatrix();
	
	
	
	
	


	


	glPushMatrix();
	
	/* assi di riferimento */
/*
	glLineWidth(4);
	glBegin(GL_LINES);
	glColor3f(1,0,0);glVertex3f(0,0,0);glVertex3f(1,0,0);
	glColor3f(0,1,0);glVertex3f(0,0,0);glVertex3f(0,1,0);
	glColor3f(0,0,1);glVertex3f(0,0,0);glVertex3f(0,0,1);
	glEnd();
*/

	glLineWidth(1);

	/* disegna la struttura esterna */
	drawEsterni();


	/* disegna struttura interna */
	drawInterni();
	
	
	
	
	  
	

	
	
	
	
	
	

	/* disegna muro frontale */

//	drawFrontWall(lunghezza, dettaglio, segno_inclinazione, altezza);


	/* drawWall che disegna un muro a partire da una certa altezza, 
	senza partire dalla base */

//	drawWallSizeHigh(lunghezza, dettaglio, segno_inclinazione, altezza);

	
	//drawWall(&a, &b, &c, &d, spessore, sp_laterale, incl_frontale, dettaglio);

	//glTranslatef(-20.0f, 0.0f, 0.0f);

	//drawWall(&a, &b, &c, &d, spessore, sp_laterale, -incl_frontale, dettaglio);

	glPopMatrix();



	setGlassMaterial();
	
	Point3d a1 = {0, 0, 0}; 
	Point3d b1 = {0, 0, 10}; 
	Point3d c1 = {-10, 0, 10}; 
	Point3d d1 = {-10, 0, 0}; 
	
	
	//drawGlass(&a1, &b1, &c1, &d1);
	
	/* dopo aver disegnato il vetro, riabilita depth test e spegni il blending */
	glDisable(GL_BLEND);              // Turn Blending Off
	glEnable(GL_DEPTH_TEST);      










/* per conoscere la posizione */
/*
	printf("position: %f %f %f\n", position.x, position.y, position.z); 
	printf("lookat:  %f %f %f\n", target.x, target.y, target.z); 
*/



	
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
		vector_scale(&direction_right, 0.03f*deltax);
		vector_scale(&direction_up, 0.05f*deltay);
		
		point_translate(&target, &direction_up);
		point_translate(&target, &direction_right);
		
		startx = x;starty = y;
		glutPostRedisplay();
	}
}



