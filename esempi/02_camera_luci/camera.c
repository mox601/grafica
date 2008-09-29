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
#include "texture.h"

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

Point3d  position = {26.788454, 115.961273, 31.399990};
Point3d  target   = {-281.613159, -521.421997, -12.699849};
Vector3d vup      = {0, 0, 1};
GLfloat zFar = 1000.0f;

/* dettaglio dei triangoli disegnati */
/* influisce sulla performance */
GLfloat dettaglio = 1.0f;
GLfloat dettaglioMax = 0.08f;
GLfloat dettaglioMin = 15.0f;
GLfloat stepDetail = 0.5f;

enum {M_NONE, M_LOCAL_LIGHTONE, M_LOCAL_LIGHTTWO, M_LOCAL_LIGHTTHREE, M_DIRECTIONAL_LIGHT, M_WIREFRAME, VIEW_LIGHTS};

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

// sole
int enable_light_directional = 1;
int enable_light_localONE = 0;
int enable_light_localTWO = 0;
int enable_light_localTHREE = 0;
int enable_lunar_light = 0;
int draw_wireframe = 0;
int enable_light_spheres = 0;

// sfera
GLUquadricObj *quadratic;	// Storage For Our Quadratic Objects

GLuint	filter;			// Which Filter To Use (nearest/linear/mipmapped)
GLuint	texture[3];		// Storage for 3 textures.

/* Image type  */
/* e' in camera.h
struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};
typedef struct Image Image;
*/




/* carica le texture openGL */

GLvoid LoadGLTextures(GLvoid) {

	// Stores the texture
	Image *image1;

	// Allocate space for texture
	image1 = (Image *) malloc(sizeof(Image));
	if (image1 == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}

	if (!ImageLoad("./textures/cielo.bmp", image1)) {
		exit(1);
	}

	// create Texture
    glGenTextures(3, &texture[0]);

    // texture 1 (poor quality scaling)
    glBindTexture(GL_TEXTURE_2D, texture[0]);   // 2d texture (x and y size)

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // cheap scaling when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // cheap scaling when image smalled than texture

    // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
    // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);

    // texture 2 (linear scaling)
    glBindTexture(GL_TEXTURE_2D, texture[1]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);

    // texture 3 (mipmapped scaling)
    glBindTexture(GL_TEXTURE_2D, texture[2]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); // scale linearly + mipmap when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);

    // 2d texture, 3 colors, width, height, RGB in that order, byte data, and the data.
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image1->sizeX, image1->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image1->data); 

}

/* ---------------------------------------------------------- */

/* inizializza parametri di opengl */

void init(void) {

	LoadGLTextures();					// Load the textures
	loadTextures();

	
	//glEnable(GL_TEXTURE_2D);			// Enable texture mapping

    //glBindTexture(GL_TEXTURE_2D, texture[filter]);   // choose the texture to use.

	glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glClearColor(0.0f,0.0f,0.0f,1.0f);

	/* smooth shading */
	glShadeModel(GL_SMOOTH);
	/* antialiasing sulle linee */
	glEnable(GL_LINE_SMOOTH);


	/* spostato nel file lights.c */
	setupLights();

	
	/* blending per i vetri */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	//glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
	
	//fog
	//glClearColor(0.5f, 0.5f, 0.5f, 1);
	//glEnable(GL_FOG);
	
	xPosition = 0.0f;
	yPosition = 0.0f;	
	zPosition = 0.0f;
	
}





/* carica le immagini da file .BMP */


// Load an image from .BMP file
int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1) 
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // used to convert bgr to rgb color.

	// Make sure the file exists
	if ((file = fopen(filename, "rb"))==NULL)
	{
		printf("File Not Found : %s\n",filename);
		return 0;
	}

	// Skip to bmp header
	fseek(file,18, SEEK_CUR);

	// read width
	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
		printf("Error reading width from %s.\n", filename);
		return 0;
	}
	printf("Width of %s: %lu\n",filename, image->sizeX);

	//read the height
	if ((i = fread(&image->sizeY,4,1,file)) != 1) {
		printf("Error reading height from %s.\n", filename);
		return 0;
	}
	printf("Height of %s: %lu\n", filename, image->sizeY);

	// calculate the size (assuming 24 bpp)
	size = image->sizeX * image->sizeY * 3;

	// read the planes
	if ((fread(&planes, 2, 1, file)) != 1) {
		printf("Error reading planes from %s. \n", filename);
		return 0;
	}

	if (planes != 1) {
		printf("Planes from %s is not 1: %u\n", filename, planes);
		return 0;
	}
	printf("Planes from %s is : %u\n", filename, planes);


	// read the bpp
	if ((i = fread(&bpp, 2, 1, file)) != 1) {
		printf("Error reading bpp from %s. \n", filename);
		return 0;
	}

	if (bpp != 24) {
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		return 0;
	}

	// seek past the rest of the bitmap header
	fseek(file, 24, SEEK_CUR);

	// Read the data
	image->data = (char *) malloc(size);
	if (image->data == NULL) {
		printf("Error allocating memory for colour-corrected image data");
		return 0;
	}

	if ((i = fread(image->data,size,1,file)) != 1) {
		printf("Error reading image data from %s.\n", filename);
		return 0;
	}

	// reverse all of the colours bgr => rgb)
	for (i=0;i<size;i+=3) {
		temp = image->data[i];
		image->data[i] = image->data[i+2];
		image->data[i+2] = temp;
	}

	// Thats all folks
	return 1;

}






void reshape(int W,int H)
{
	glViewport(0, 0, W, H);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 
		60.0,  /* field of view in degree */ 
		W/H,   /* aspect ratio */ 
		0.8,   /* Z near */ 
		zFar  /* Z far molto grande per far vedere lo sfondo */ 
	);
	

	glMatrixMode(GL_MODELVIEW);		
	glLoadIdentity();
	gluLookAt(
		position.x, position.y ,  position.z,		   
		target.x  , target.y   ,  target.z,       
		vup.x     , vup.y      ,  vup.z);


	glutPostRedisplay();
}


GLfloat motionTimer = 0.0f;
GLfloat motionStep = 0.05f;
GLfloat motionUpperBound = 1.0f;
GLfloat motionLowerBound = -1.0f;
GLint motionSign = 1;


void incrementMotionTimer() {

	if ((motionTimer + motionStep) > motionUpperBound) {
		motionTimer = motionUpperBound;
		motionSign = -1;
		//printf("upper bound hit\n");
	} else { 
		motionTimer += motionStep;
	}

}

void decrementMotionTimer() {

	if ((motionTimer - motionStep) < motionLowerBound) {
		motionTimer = motionLowerBound;
		motionSign = 1;
		//printf("lower bound hit\n");
	} else { 
		motionTimer -= motionStep;
	}

}


void idle(void)
{
/* movimento luce */
	
	
	lightAngle += 0.01f;
	if (lightAngle > 360.f) {
		lightAngle = 0.0f;
	
	}
	
	
	//printf("lightAngle: %f\n", lightAngle);
	
	if (motionSign == 1) {
		//incrementMotionTimer();
	} else {
		//	decrementMotionTimer();
		}
		
	//printf("motionTimer: %f, motionSign: %d\n", motionTimer, motionSign);


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
	
	//glutAddMenuEntry("-----------------------", M_NONE);

	glutAddMenuEntry("Local light - luce rossa al piano terra"      , M_LOCAL_LIGHTONE);
	glutAddMenuEntry("Local light - luce gialla al secondo piano"      , M_LOCAL_LIGHTTWO);
	glutAddMenuEntry("Local light - luce variabile al terzo piano"      , M_LOCAL_LIGHTTHREE);
	glutAddMenuEntry("Giorno / Notte", M_DIRECTIONAL_LIGHT);
//	glutAddMenuEntry("Local light - Notte", M_LUNAR_LIGHT);
	glutAddMenuEntry("visualizza posizione delle luci", VIEW_LIGHTS);
	glutAddMenuEntry("Draw wireframe",    M_WIREFRAME);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	/* caricamento dei .ply */
	loadPlyModels();


	glMatrixMode(GL_PROJECTION);
	gluPerspective( 
		60.0,  /* field of view in degree */ 
		1.0,   /* aspect ratio */ 
		0.8,   /* Z near */ 
		zFar  /* Z far per far rientrare lo sfondo s */ 
	);

	glMatrixMode(GL_MODELVIEW);
	glutMainLoop();
	return 0; 
}

/* ---------------------------------------------------------- */

/* ---------------------------------------------------------- */

// alcuni valori di dettaglio fanno segmentation fault

void aumentaDettaglio() {
	if (dettaglio > (dettaglioMax + stepDetail)) { 
		dettaglio = dettaglio - stepDetail;
	} else {
		dettaglio = dettaglioMax;
		}		
		printf("dettaglio attuale: %f\n", dettaglio);
}
void diminuisciDettaglio() {
	if (dettaglio < (dettaglioMin - stepDetail)) { 
		dettaglio = dettaglio + stepDetail;
	} else {
		dettaglio = dettaglioMin;
		}
		printf("dettaglio attuale: %f\n", dettaglio);
}




/* --------------------gestione della tastiera------------------------------ */
void key(unsigned char c, int x, int y)
{
 	
			
	Vector3d direction_forward, 
	direction_right, 
	direction_up, 
	direction_backward, 
	direction_left, 
	direction_down;

	vector_diff(&direction_forward,&target,&position);
	vector_diff(&direction_backward,&position,&target);
	
	// normalizzo le direzioni forward e backward
	vector_normalize(&direction_forward);
	vector_normalize(&direction_backward);
	
	//??
	//vector_normalize(&target);

	vector_cross_product(&direction_left,&vup,&direction_forward);
	vector_cross_product(&direction_right,&vup,&direction_backward);
	
	// normalizzo le direzioni left e right
	//vector_normalize(&direction_left);
	//vector_normalize(&direction_right);

	direction_up=vup;
	direction_down=vup;
	vector_scale(&direction_down, -1);

	// lunghezza dei passi avanti e indietro
	GLfloat avanzamento = 0.9f;

switch(c) 
	{

/* w: avanti; s: indietro */
	case 'w':
		vector_scale(&direction_forward, avanzamento);
		Vector3d direction_forward_x = direction_forward; 
		direction_forward_x.z = 0.0f; 
		point_translate(&position, &direction_forward_x);
		point_translate(&target, &direction_forward_x);
		glutPostRedisplay();
		break;
	
	case 's':
		vector_scale(&direction_backward,avanzamento);
		Vector3d direction_backward_x = direction_backward; 
		direction_backward_x.z = 0.0f; 
		point_translate(&position, &direction_backward_x);
		point_translate(&target, &direction_backward_x);
		glutPostRedisplay();	
		break;


	
		
		GLfloat rotation = 100.0f;
		
	
/* girati a dx o sx */
	case 'a':
		//printf("target pre: %f %f %f\n", target.x, target.y, target.z);
		//printf("direction_left pre: %f %f %f\n", direction_left.x, &direction_left.y, &direction_left.z);
		//vector_scale(&direction_left, rotation);
	
		vector_scale(&direction_left, rotation);
//		vector_scale(&direction_up, 0.1f*rotation);

		//printf("coeff deltax: %f\n", 0.1f * deltax);
		//printf("coeff deltay: %f\n", 0.1f * deltay);
				
		//point_translate(&target, &direction_up);
		point_translate(&target, &direction_left);
			
			
			
			
			
							
		//printf("direction_left dop: %f %f %f\n", direction_left.x, &direction_left.y, &direction_left.z);
		//point_translate(&target, &direction_left);
		//printf("target dop: %f %f %f\n", target.x, target.y, target.z);
		glutPostRedisplay();
		break;

	case 'd': 
		//printf("direction_right pre: %f %f %f\n", direction_right.x, &direction_right.y, &direction_right.z);
		vector_scale(&direction_right, rotation);
		//printf("direction_right dop: %f %f %f\n", direction_right.x, &direction_right.y, &direction_right.z);
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
		
		
		
		
		
	case 'F':
    case 'f': // switch the filter.
		printf("F/f pressed; filter is: %d\n", filter);
		filter+=1;
		if (filter>2) 
			filter=0;		
		printf("Filter is now: %d\n", filter);
	break;
		
		
		
		GLfloat spostamento = 0.001f;
		
		
		/* controlli per il piazzamento dei solidi */
		
	case '7': 
		/*abbassa z*/
		zPosition -= spostamento;
		printf("posizione attuale: %f %f %f\n", xPosition, yPosition, zPosition);
		glutPostRedisplay();
		break; 

	case '8': 
		/* alza z */
		zPosition += spostamento;
		printf("posizione attuale: %f %f %f\n", xPosition, yPosition, zPosition);
		glutPostRedisplay();
		break; 
		
	case 'k': 
		/*abbassa x*/
		xPosition -= spostamento;
		printf("posizione attuale: %f %f %f\n", xPosition, yPosition, zPosition);		
		break; 

	case 'i': 
		/* alza x */
		xPosition += spostamento;
		printf("posizione attuale: %f %f %f\n", xPosition, yPosition, zPosition);		
		glutPostRedisplay();
		break; 
	
	case 'j': 
		/*abbassa y*/
		yPosition -= spostamento;
		printf("posizione attuale: %f %f %f\n", xPosition, yPosition, zPosition);
		glutPostRedisplay();
		break; 

	case 'l': 
		/* alza y */
		yPosition += spostamento;
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


	/* per conoscere la posizione */
/*
	printf("position: %f %f %f\n", position.x, position.y, position.z); 
	printf("lookat:  %f %f %f\n", target.x, target.y, target.z); 
*/






	}
	
	
	
	

  glutPostRedisplay();
}


/* -------------gestione del menu con tasto destro---------------- */

void controlMenu(int value)
{
	switch (value) 
	{
	case M_NONE:return;

	case M_LOCAL_LIGHTONE:
		enable_light_localONE=1-enable_light_localONE;
		printf("switch luce locale rossa al piano terra\n");
		printf("luce locale = %d\n", enable_light_localONE);
	break;

	case M_LOCAL_LIGHTTWO:
		enable_light_localTWO=1-enable_light_localTWO;
		printf("switch luce locale gialla al primo piano\n");
		printf("luce locale = %d\n", enable_light_localTWO);
	break;
	
	case M_LOCAL_LIGHTTHREE:
		enable_light_localTHREE=1-enable_light_localTHREE;
		printf("switch luce locale verde al secondo piano\n");
		printf("luce locale = %d\n", enable_light_localTHREE);
	break;
	
	case M_DIRECTIONAL_LIGHT:
		enable_light_directional=1-enable_light_directional;
		enable_lunar_light = 1-enable_lunar_light;	
		printf("switch giorno/notte\n");
		printf("luce direzionale del sole = %d\n", enable_light_directional);
		printf("luce direzionale della luna = %d\n", enable_lunar_light);

	break;
	
	case VIEW_LIGHTS:
		enable_light_spheres=1-enable_light_spheres;
		printf("switch luci visibili = %d\n", enable_light_spheres);
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
	
	
	//disabilita le texture, le attivano solo gli oggetti con texture
	glDisable(GL_TEXTURE_2D);

	
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glLoadIdentity();


	gluLookAt(
		position.x, position.y ,  position.z,		   
		target.x  , target.y   ,  target.z,       
		vup.x     , vup.y      ,  vup.z);

	/* movimento luce 1 locale */
//	light_position_local[0]= -30.0f + sin(lightAngle)*5;
//	light_position_local[1]= -1.0f + cos(lightAngle)*5;
//	light_position_local[2]= 1.0f + cos(lightAngle)*5;

	
	

/* luce 0, una luce del sole direzionale */

	if (enable_light_directional)
	{
		glLightfv(GL_LIGHT0, GL_POSITION, light_position_directional);
		glEnable(GL_LIGHT0);
	}
	else
	{
		glDisable(GL_LIGHT0);
	}

	
		
	
if (enable_light_spheres) {
	/* disegna sfera gialla dove si trova la luce solare (o da dove viene) */
	glPushMatrix();
	glTranslatef(light_position_directional[0],light_position_directional[1],light_position_directional[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color_yellow_light);
		if (!draw_wireframe)
			glutSolidSphere(0.2, 5, 5);
		else
			glutWireSphere(0.2, 5, 5);
	glPopMatrix();
}




	/* aggiorno la posizione della luce rossa */

	//y e z 23.659597 1.644782
	// si deve muovere sull'asse x a partire da valore x = -11.38 fino a x=-0.759130

	GLfloat ampiezzaMovimento = (-11.38 + 0.759130);
	GLfloat spostamentoLocale = ampiezzaMovimento /2;

	light_position_localONE[0] = ((spostamentoLocale * cos(1.5f*lightAngle)) + spostamentoLocale - 0.5) * GLSCALAMENTO;
	light_position_localONE[1] = 23.659597 * GLSCALAMENTO;
	light_position_localONE[2] = 1.644782 * GLSCALAMENTO;


	/* per fare update della posizione e stato della luce 1 */
	if (enable_light_localONE)
	{
		glLightfv(GL_LIGHT1, GL_POSITION, light_position_localONE);
		glEnable(GL_LIGHT1);
	}
	else
	{
		glDisable(GL_LIGHT1);
	}


if (enable_light_spheres) {
	/* disegna sfera dove si trova la luce direzionale rossa 1 */
	glPushMatrix();
	glTranslatef(light_position_localONE[0],light_position_localONE[1],light_position_localONE[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color_red);

		if (!draw_wireframe)
			glutSolidSphere(0.3, 10, 10);
		else
			glutWireSphere(0.3, 10, 10);

	glPopMatrix();
}







/* aggiorno la posizione della luce gialla, primo piano*/

	//y e z 23.659597 1.644782
	// si deve muovere sull'asse x a partire da valore x = -11.38 fino a x=-0.759130

	GLfloat deltaYellow = 180.0f;

	light_position_localTWO[0] = ((spostamentoLocale * cos(0.5f*(lightAngle + deltaYellow))) + spostamentoLocale - 0.5) * GLSCALAMENTO;
	light_position_localTWO[1] = 23.659597 * GLSCALAMENTO;
	//primo piano!!!
	light_position_localTWO[2] = (1.644782 + 2.024347) * GLSCALAMENTO;


	/* per fare update della posizione e stato della luce 2 */
	if (enable_light_localTWO)
	{
		glLightfv(GL_LIGHT2, GL_POSITION, light_position_localTWO);
		glEnable(GL_LIGHT2);
	}
	else
	{
		glDisable(GL_LIGHT2);
	}

if (enable_light_spheres) {
	/* disegna sfera dove si trova la luce direzionale gialla 2 */
	glPushMatrix();
	glTranslatef(light_position_localTWO[0],light_position_localTWO[1],light_position_localTWO[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, coloryellow);

		if (!draw_wireframe)
			glutSolidSphere(0.3, 10, 10);
		else
			glutWireSphere(0.3, 10, 10);

	glPopMatrix();
}

/* aggiorno la posizione della luce verde, secondo piano*/

// parte da verde e poi cambia i coefficienti sinusoidalmente
	
	GLfloat deltaGreen = 90.0f;

	light_position_localTHREE[0] = ((spostamentoLocale * cos(2.0f*(lightAngle+ deltaGreen))) + spostamentoLocale - 0.5) * GLSCALAMENTO;
	light_position_localTHREE[1] = 23.659597 * GLSCALAMENTO;
	//secondo piano!!!
//	light_position_localTHREE[2] = (1.644782f + (2.024347) + 3.289563) * GLSCALAMENTO;
	light_position_localTHREE[2] = (6.958692f) * GLSCALAMENTO;
		

	// cambio i parametri del colore
	
	variableLight[0] = sin(2.0f*lightAngle);
	variableLight[1] = sin(0.8 * lightAngle + 45.0f);
	variableLight[2] = sin(lightAngle + 180.0f);
			
	// update del colore della luce
	//glLightfv(GL_LIGHT3, GL_AMBIENT  , variableLight);
	glLightfv(GL_LIGHT3, GL_DIFFUSE  , variableLight);
	glLightfv(GL_LIGHT3, GL_SPECULAR , variableLight);

	/* per fare update della posizione e stato della luce 2 */
	if (enable_light_localTHREE)
	{
		glLightfv(GL_LIGHT3, GL_POSITION, light_position_localTHREE);
		glEnable(GL_LIGHT3);
	}
	else
	{
		glDisable(GL_LIGHT3);
	}

if (enable_light_spheres) {
	/* disegna sfera dove si trova la luce direzionale verde 3 */
	glPushMatrix();
	glTranslatef(light_position_localTHREE[0],light_position_localTHREE[1],light_position_localTHREE[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, variableLight);

		if (!draw_wireframe)
			glutSolidSphere(0.3, 10, 10);
		else
			glutWireSphere(0.3, 10, 10);

	glPopMatrix();
}


	//enable_lunar_light



/* per fare update della posizione e stato della luce della luna e del faro */






/* posizione faro */

	//faro_spotlight[0] = xPosition/GLSCALAMENTO; 
	//faro_spotlight[1] = yPosition/GLSCALAMENTO; 
	//faro_spotlight[2] = zPosition/GLSCALAMENTO; 
	
/* movimento del faro */

	GLfloat maxHeight = 3.0;
	
	direzioneFaro[0] = 0.0f + xPosition;
	direzioneFaro[1] = 0.0f + yPosition;
//	direzioneFaro[2] = (maxHeight * cos(lightAngle * 1.2f)) + maxHeight;
	direzioneFaro[2] = 0.0f;
	
	
	/*
	printf("direzione faro x: %f\n", direzioneFaro[0]);
	printf("direzione faro y: %f\n", direzioneFaro[1]);
	printf("direzione faro z: %f\n", direzioneFaro[2]);
	*/

	if (enable_lunar_light)
	{
		glLightfv(GL_LIGHT4, GL_POSITION, light_position_lunar);
		glLightfv(GL_LIGHT5, GL_POSITION, faro_spotlight);
		glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, direzioneFaro);
		glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 6.073038f);

		glEnable(GL_LIGHT4);
		glEnable(GL_LIGHT5);
	}
	else
	{
		glDisable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
	}


	if(enable_lunar_light) {
	
	
	
	/* metto la texture */
	glEnable(GL_TEXTURE_2D);
	//setTextureScaling(30);
	setTexture(TEXTURE_MOON);
	
	
	
	/* disegna sfera dove si trova la luna */
	glPushMatrix();
	glTranslatef(light_position_lunar[0],light_position_lunar[1],light_position_lunar[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color_white);

		glDisable(GL_LIGHTING);
		
		if (!draw_wireframe) {
			drawSphere(15.0, 10, 10);
			}
		else
			glutWireSphere(15.0, 10, 10);
		glEnable(GL_LIGHTING);
		
	glPopMatrix();
	
	//spengo le texture, dopo aver fatto la luna
	glDisable(GL_TEXTURE_2D);
	
	}


if (enable_light_spheres) {
/* blocchetto dove si trova il faro */
	glPushMatrix();
	glTranslatef(faro_spotlight[0], faro_spotlight[1], faro_spotlight[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color_black);

	GLUquadric* faro = gluNewQuadric();

	GLdouble raggioBase = 0.1f;
	GLdouble raggioTop = 0.1f;
	GLdouble altezza = 0.5f;
	
		//glDisable(GL_LIGHTING);
		if (!draw_wireframe) {
		//gluCylinder( faro, raggioBase, raggioTop, altezza, 5, 5 );
		}
		else {
		//gluCylinder( faro, raggioBase, raggioTop, altezza, 5, 5 );
		//glEnable(GL_LIGHTING);
		}
	glPopMatrix();
}






																														
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
	/* attutisco la luce del sole e della luna, e dopo la rimetto come era per il vetro */

	/* luce numero 0 direzionale, il sole */
	glLightfv(GL_LIGHT0, GL_AMBIENT  , sun_color_interni);
	glLightfv(GL_LIGHT0, GL_DIFFUSE  , sun_color_interni);
	glLightfv(GL_LIGHT0, GL_SPECULAR , sun_color_interni);
	/* luna */
	glLightfv(GL_LIGHT4, GL_AMBIENT  , moon_color_interni);
	glLightfv(GL_LIGHT4, GL_DIFFUSE  , moon_color_interni);
	glLightfv(GL_LIGHT4, GL_SPECULAR , moon_color_interni);
	
	
	
		drawInterni();
	
	
	
	// rimetto il colore normale al sole 
	glLightfv(GL_LIGHT0, GL_AMBIENT  , color_yellow_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE  , color_yellow_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR , color_yellow_light);
	/* luna */
	//glLightfv(GL_LIGHT4, GL_AMBIENT  , moon_color_esterni);
	glLightfv(GL_LIGHT4, GL_DIFFUSE  , moon_color_esterni);
	glLightfv(GL_LIGHT4, GL_SPECULAR , moon_color_esterni);
	
	
		
	/* disegna muro frontale */

//	drawFrontWall(lunghezza, dettaglio, segno_inclinazione, altezza);


	/* drawWall che disegna un muro a partire da una certa altezza, 
	senza partire dalla base */

//	drawWallSizeHigh(lunghezza, dettaglio, segno_inclinazione, altezza);

	
	//drawWall(&a, &b, &c, &d, spessore, sp_laterale, incl_frontale, dettaglio);

	//glTranslatef(-20.0f, 0.0f, 0.0f);

	//drawWall(&a, &b, &c, &d, spessore, sp_laterale, -incl_frontale, dettaglio);

	setGlassMaterial();
	
	//draw glass!!!
	
	glPushMatrix();
	
	
	glTranslatef(-23.912642, 32.516140, 0.0f);
	
	// posso anche spegnere l'illuminazione?
	// no, meglio curare il colore a posteriori
	
		//glDisable(GL_LIGHT0);

			drawGlassRight();

		//glEnable(GL_LIGHT0);

	glPopMatrix();





	glPopMatrix();
// esterni ed interni 
	
	/* dopo aver disegnato il vetro, riabilita depth test e spegni il blending */
	glDisable(GL_BLEND);              // Turn Blending Off
	glEnable(GL_DEPTH_TEST);      





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
		vector_scale(&direction_right, 0.9f*deltax);
		vector_scale(&direction_up, 0.9f*deltay);

		//printf("coeff deltax: %f\n", 0.1f * deltax);
		//printf("coeff deltay: %f\n", 0.1f * deltay);
				
		point_translate(&target, &direction_up);
		point_translate(&target, &direction_right);
		
		startx = x;starty = y;
		glutPostRedisplay();
	}
}



