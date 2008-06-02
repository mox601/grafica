#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     
#include <GL/glut.h>  
#include "utils.h"
#include "oggetti.h"
#include "esterno.h"
#include "texture.h"
#include "input.h"
#include "player.h"
#include "ambiente.h"
#include "interno.h"
#include "light.h"
#include "collision.h"

#ifndef M_PI
#define M_PI 3.14159265
#endif


/* prototype */
static void redraw(void);
void visible(int vis);
void idle(void);
void initDisplayList();

enum {
  M_NONE
};


GLfloat ambient[4] = {0.2,0.2,0.2,0.2};
GLfloat fogcolor[4] = {0.1,0.0,0.1,1.0};
GLfloat skycolor[4] = {0.0,0.0,0.2,1.0};
GLfloat diffuse[4] = {1.0,1.0,1.0,1.0};
GLfloat specular[4] = {1.0,1.0,1.0,1.0};
GLfloat emission[4] = {0.0,0.0,0.0,1.0};
GLfloat lightmoon[4] = {0.5,0.5,0.5,1.0};

GLfloat black[4] = {0.0,0.0,0.0,1.0};

int skyList = -1;
int sandList = -1;
int interniList = -1;
int facciataList = -1;
int retroIlluminatoList = -1;
int retroNonIlluminatoList = -1;
int altaIlluminataList = -1;
int altaNonIlluminataList = -1;
int bassaIlluminataList = -1;
int bassaNonIlluminataList = -1;
int basamentoList = -1;

float altezzaBaseChiesa = 20.0;

int updateLight = 0;



/* ---------------------------------------------------------- */
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(800,800);
	glutCreateWindow("chiesa");
	
	glutDisplayFunc(redraw);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(motion);
	glutVisibilityFunc(visible);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyReleased);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glLineWidth(3.0);
	
	glMatrixMode(GL_PROJECTION);
	gluPerspective( 
		/* field of view in degree */ 80.0,
		/* aspect ratio */ 1.0,
		/* Z near */ 1.0, /* Z far */ 3000.0);
		
	glMatrixMode(GL_MODELVIEW);

	loadTextures();

	//inizializzo tutte le diplayList
	initDisplayList();

	initLookAt(
		28.19, 18, 1483.92,  /* eye  */
		28.47, 18, 1483.09, /* center  */
		0.0, 1.0, 0.0); /* up is in positive Y direction */
	

	//initCollision();

	//parete sinistra chiesa
	addCollisionLine(0.0,3.0,0.0,-320.0);
	addCollisionLine(5.0,3.0,5.0,-320.0);

	//parete destra chiesa
	addCollisionLine(178.0,3.0,178.0,-320.0);
	addCollisionLine(173.0,3.0,173.0,-320.0);

	//facciata chiesa
	addCollisionLine(0.0,0.0,13.0,0.0);
	addCollisionLine(0.0,-3.0,13.0,-3.0);
	addCollisionLine(26.0,0.0,70.0,0.0);
	addCollisionLine(26.0,-3.0,70.0,-3.0);
	addCollisionLine(108.0,0.0,151.0,0.0);
	addCollisionLine(108.0,-3.0,151.0,-3.0);
	addCollisionLine(165.0,0.0,178.0,0.0);
	addCollisionLine(165.0,-3.0,178.0,-3.0);

	//parete dietro chiesa
	addCollisionLine(0.0,-320.0,178.0,-320.0);
	addCollisionLine(0.0,-315.0,178.0,-315.0);

	addCollisionLine(38.0,-320.0,38.0,-300.0);
	addCollisionLine(140.0,-320.0,140.0,-300.0);

	addCollisionLine(143.0,-310.0,153.0,-310.0);
	addCollisionLine(153.0,-310.0,153.0,-320.0);
	addCollisionLine(163.0,-310.0,163.0,-320.0);
	addCollisionLine(163.0,-310.0,173.0,-310.0);

	addCollisionLine(5.0,-310.0,15.0,-310.0);
	addCollisionLine(15.0,-310.0,15.0,-320.0);
	addCollisionLine(25.0,-310.0,25.0,-320.0);
	addCollisionLine(25.0,-310.0,35.0,-310.0);

	//retro chiesa
	addCollisionLine(5.0,-310.0,35.0,-310.0);
	addCollisionLine(5.0,-320.0,5.0,-330.0);
	addCollisionLine(35.0,-320.0,35.0,-330.0);

	addCollisionLine(10.0,-320.0,10.0,-335.0);
	addCollisionLine(30.0,-320.0,30.0,-335.0);
	addCollisionLine(10.0,-335.0,30.0,-335.0);

	//absidi
	addCollisionLine(143.0,-310.0,173.0,-310.0);
	addCollisionLine(143.0,-320.0,143.0,-330.0);
	addCollisionLine(173.0,-320.0,173.0,-330.0);

	addCollisionLine(148.0,-320.0,148.0,-335.0);
	addCollisionLine(168.0,-320.0,168.0,-335.0);
	addCollisionLine(148.0,-335.0,168.0,-335.0);
	
	//grande
	addCollisionLine(39.0,-320.0,139.0,-320.0);
	addCollisionLine(39.0,-320.0,39.0,-350.0);
	addCollisionLine(139.0,-320.0,139.0,-320.0);
	addCollisionLine(39.0,-350.0,139.0,-350.0);	

	addCollisionLine(49.0,-360.0,129.0,-360.0);
	addCollisionLine(49.0,-320.0,49.0,-360.0);
	addCollisionLine(129.0,-320.0,129.0,-360.0);	

	addCollisionLine(59.0,-320.0,59.0,-370.0);
	addCollisionLine(119.0,-320.0,119.0,-370.0);
	addCollisionLine(59.0,-370.0,119.0,-370.0);	

	addCollisionLine(10.0,-330.0,165.0,-330.0);


	//panchine sx
	addCollisionLine(60.0,-60.0,80.0,-60.0);
	//addCollisionLine(60.0,-64.0,80.0,-64.0);
	addCollisionLine(64.0,-60.0,64.0,-232.0);
	addCollisionLine(60.0,-232.0,80.0,-232.0);
	addCollisionLine(80.0,-60.0,80.0,-232.0);

	//panchine dx
	addCollisionLine(100.0,-60.0,120.0,-60.0);
	addCollisionLine(100.0,-60.0,100.0,-232.0);
	addCollisionLine(100.0,-232.0,120.0,-232.0);
	addCollisionLine(120.0,-60.0,120.0,-232.0);

	//altare
	addCollisionLine(60.0,-270.0,60.0,-320.0);
	addCollisionLine(60.0,-320.0,120.0,-320.0);
	addCollisionLine(120.0,-320.0,120.0,-270.0);
	addCollisionLine(120.0,-270.0,60.0,-270.0);

	//tavolini ceri
	addCollisionLine(5.0,-60.0,13.0,-60.0);
	addCollisionLine(167.0,-60.0,175.0,-60.0);
	addCollisionLine(5.0,-191.0,13.0,-191.0);
	addCollisionLine(167.0,-191.0,175.0,-191.0);

	//colonnato sx
	addCollisionLine(35.0,-39.0,41.0,-39.0);
	addCollisionLine(41.0,-39.0,41.0,-45.0);
	addCollisionLine(41.0,-45.0,35.0,-45.0);
	addCollisionLine(35.0,-45.0,35.0,-39.0);

	addCollisionLine(35.0,-82.0,41.0,-82.0);
	addCollisionLine(41.0,-82.0,41.0,-88.0);
	addCollisionLine(41.0,-88.0,35.0,-88.0);
	addCollisionLine(35.0,-88.0,35.0,-82.0);

	addCollisionLine(35.0,-125.0,41.0,-125.0);
	addCollisionLine(41.0,-125.0,41.0,-131.0);
	addCollisionLine(41.0,-131.0,35.0,-131.0);
	addCollisionLine(35.0,-131.0,35.0,-125.0);

	addCollisionLine(35.0,-168.0,41.0,-168.0);
	addCollisionLine(41.0,-168.0,41.0,-174.0);
	addCollisionLine(41.0,-174.0,35.0,-174.0);
	addCollisionLine(35.0,-174.0,35.0,-168.0);

	addCollisionLine(35.0,-211.0,41.0,-211.0);
	addCollisionLine(41.0,-211.0,41.0,-217.0);
	addCollisionLine(41.0,-217.0,35.0,-217.0);
	addCollisionLine(35.0,-217.0,35.0,-211.0);

	addCollisionLine(35.0,-254.0,41.0,-254.0);
	addCollisionLine(41.0,-254.0,41.0,-260.0);
	addCollisionLine(41.0,-260.0,35.0,-260.0);
	addCollisionLine(35.0,-260.0,35.0,-254.0);

	//colonnato dx
	addCollisionLine(137.0,-39.0,143.0,-39.0);
	addCollisionLine(143.0,-39.0,143.0,-45.0);
	addCollisionLine(143.0,-45.0,137.0,-45.0);
	addCollisionLine(137.0,-45.0,137.0,-39.0);

	addCollisionLine(137.0,-82.0,143.0,-82.0);
	addCollisionLine(143.0,-82.0,143.0,-88.0);
	addCollisionLine(143.0,-88.0,137.0,-88.0);
	addCollisionLine(137.0,-88.0,137.0,-82.0);

	addCollisionLine(137.0,-125.0,143.0,-125.0);
	addCollisionLine(143.0,-125.0,143.0,-131.0);
	addCollisionLine(143.0,-131.0,137.0,-131.0);
	addCollisionLine(137.0,-131.0,137.0,-125.0);

	addCollisionLine(137.0,-168.0,143.0,-168.0);
	addCollisionLine(143.0,-168.0,143.0,-174.0);
	addCollisionLine(143.0,-174.0,137.0,-174.0);
	addCollisionLine(137.0,-174.0,137.0,-168.0);

	addCollisionLine(137.0,-211.0,143.0,-211.0);
	addCollisionLine(143.0,-211.0,143.0,-217.0);
	addCollisionLine(143.0,-217.0,137.0,-217.0);
	addCollisionLine(137.0,-217.0,137.0,-211.0);

	addCollisionLine(137.0,-254.0,143.0,-254.0);
	addCollisionLine(143.0,-254.0,143.0,-260.0);
	addCollisionLine(143.0,-260.0,137.0,-260.0);
	addCollisionLine(137.0,-260.0,137.0,-254.0);



	printCollisionMap();


	glShadeModel(GL_SMOOTH);
	
	glutIgnoreKeyRepeat(1);

	glutMainLoop();
	//glDeleteTextures(TEXTURE_COUNT, textures);

	return 0; 
}


/* ---------------------------------------------------------- */
void visible(int vis)
{
	glutIdleFunc((vis==GLUT_VISIBLE)?idle:NULL);
}


/* ------------------------------------------------------------------------------ */
void initDisplayList(){


	if (skyList == -1)
		{
			skyList=glGenLists(1);
			glNewList( skyList , GL_COMPILE );
				drawStelle(1600.0,60);
			glEndList();
		}


	if (sandList == -1)
		{
			sandList=glGenLists(1);
			glNewList( sandList , GL_COMPILE );
				setTexture(TEXTURE_SAND);
				drawTerreno(1600.0,6);
			glEndList();
		}

	if(interniList == -1)
		{
			interniList=glGenLists(1);
			glNewList(interniList, GL_COMPILE);
				drawPavimentoConRiflesso();
				
				drawInterni();
				costruisciRosone();
			glEndList();
		}

	if(basamentoList == -1)

		{
			basamentoList=glGenLists(1);
			glNewList(basamentoList, GL_COMPILE);
				drawBasamento(2.0*altezzaBaseChiesa, 3.0, 15);
			glEndList();
		}

	

	if(facciataList == -1)

		{
			facciataList=glGenLists(1);
			glNewList(facciataList, GL_COMPILE);
				drawFacciata();
			glEndList();
		}


	
	if(retroIlluminatoList == -1)

		{
			retroIlluminatoList=glGenLists(1);
			glNewList(retroIlluminatoList, GL_COMPILE);

				drawRetroIlluminato();
			glEndList();
		}


	
	if(altaIlluminataList == -1)

		{
			altaIlluminataList=glGenLists(1);
			glNewList(altaIlluminataList, GL_COMPILE);

				drawTettoIlluminato();
				drawPareteDestraAlta();

			glEndList();
		}


	if(bassaIlluminataList == -1)

		{
			bassaIlluminataList=glGenLists(1);
			glNewList(bassaIlluminataList, GL_COMPILE);

				drawPareteDestraBassa();

			glEndList();
		}

	if(retroNonIlluminatoList == -1)

		{
			retroNonIlluminatoList=glGenLists(1);
			glNewList(retroNonIlluminatoList, GL_COMPILE);

				drawRetroNonIlluminato();

			glEndList();
		}


	if(altaNonIlluminataList == -1)

		{
			altaNonIlluminataList=glGenLists(1);
			glNewList(altaNonIlluminataList, GL_COMPILE);

				drawTettoNonIlluminato();
				drawPareteSinistraAlta();

			glEndList();
		}

	
	if(bassaNonIlluminataList == -1)

		{
			bassaNonIlluminataList=glGenLists(1);
			glNewList(bassaNonIlluminataList, GL_COMPILE);

						
				drawPareteSinistraBassa();

			glEndList();
		}

}






/*--------------------------------------------------------------------------------*/

static void redraw(void)
{


	GLfloat lightpos[4]={0.0,20.0,0.0,1.0};
	GLfloat fiammaPos[4] = {10.0,10.0,5.0,1.0};
	GLfloat lightdiffuse[4]={1.0,1.0,1.0,1.0};
	GLfloat fiammaLight[4];

	GLfloat lightLampadarioPos[4]={89.0, 135.0, -175.0,1.0};
	GLfloat lightCandelabro1Pos[4]={80.0, 30.1, -285.0,1.0};
	GLfloat lightCandelabro2Pos[4]={100.0, 30.1, -285.0,1.0};
	GLfloat lightCeri1Pos[4]={9.5, 15.0, -64.0,1.0};
	GLfloat lightCeri2Pos[4]={9.5, 15.0, -195.0,1.0};
	GLfloat lightCeri3Pos[4]={168.5, 15.0, -64.0,1.0};
	GLfloat lightCeri4Pos[4]={168.5, 15.0, -195.0,1.0};

	GLfloat lightLampione1Pos[4]={-5.0, 35.0, -(320.0*3)/5.0,1};
	GLfloat lightLampione2Pos[4]={-5.0, 35.0, -320.0/5.0,1};
	GLfloat lightLampione3Pos[4]={54.0, 50.0+3.5, 20,1};
	GLfloat lightLampione4Pos[4]={124.0, 50.0+3.5, 20,1};
	GLfloat lightLampione5Pos[4]={185.0, 35.0, -320.0/5.0,1};
	GLfloat lightLampione6Pos[4]={185.0, 35.0, -(320.0*3)/5.0,1};


	glClearColor(0.1,0,0.1,1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);





	//disegno il cielo
	glCallList(skyList);

	glEnable(GL_LIGHTING);

	resetMaterial();
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	updateLookAt();

	
	getMoonCoords(lightpos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightmoon);
	glLightfv(GL_LIGHT1, GL_EMISSION, lightmoon);
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos);

	
	glEnable(GL_LIGHT1);

	glDisable(GL_LIGHT0);
	
	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR, fogcolor);
	glFogf(GL_FOG_START, 80.0f);
	glFogf(GL_FOG_END, 1200.0f);
	glFogi(GL_FOG_MODE, GL_LINEAR);

	glEnable(GL_TEXTURE_2D);





	//TRASLO LA CHIESA AL CENTRO!!!!
	glTranslatef(-89.0,altezzaBaseChiesa,373.0/2.0);


	

/******************** DISEGNO IL PAVIEMTO RIFLETTENTE E GLI INTERNI ACCOMPAGNATI DALLE LUCI **********************/
/*****************************************************************************************************************	
	GLfloat lightLampadarioPos[4]={89.0, 120.0, -175.0,1.0};
	GLfloat lightCandelabro1Pos[4]={80.0, 20.1, -285.0,1.0};
	GLfloat lightCandelabro2Pos[4]={100.0, 20.1, -285.0,1.0};
	GLfloat lightCeri1Pos[4]={7.5, 10.0, -64.0,1.0};
	GLfloat lightCeri2Pos[4]={7.5, 10.0, -195.0,1.0};
	GLfloat lightCeri3Pos[4]={170.5, 10.0, -64.0,1.0};
	GLfloat lightCeri4Pos[4]={170.5, 10.0, -195.0,1.0}; */

	
		glPushMatrix();
			updateFiammaEsterna(lightLampadarioPos,fiammaLight,0.0005);
			glLightfv(GL_LIGHT3, GL_DIFFUSE, fiammaLight);
			glLightfv(GL_LIGHT3, GL_POSITION, lightLampadarioPos);
			glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.05f);
			glEnable(GL_LIGHT3);
		glPopMatrix();

		glPushMatrix();
			updateFiammaInterna(lightCandelabro1Pos,fiammaLight,0.0005);
			glLightfv(GL_LIGHT4, GL_DIFFUSE, fiammaLight);
			glLightfv(GL_LIGHT4, GL_POSITION, lightCandelabro1Pos);
			glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.05f);
			glEnable(GL_LIGHT4);
		glPopMatrix();

		glPushMatrix();
			updateFiammaInterna(lightCandelabro2Pos,fiammaLight,0.0005);
			glLightfv(GL_LIGHT5, GL_DIFFUSE, fiammaLight);
			glLightfv(GL_LIGHT5, GL_POSITION, lightCandelabro2Pos);
			glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.05f);
			glEnable(GL_LIGHT5);
		glPopMatrix();

		glPushMatrix();
			updateFiammaInterna(lightCeri1Pos,fiammaLight,0.0005);
			glLightfv(GL_LIGHT6, GL_DIFFUSE, fiammaLight);
			glLightfv(GL_LIGHT6, GL_POSITION, lightCeri1Pos);
			glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, 0.05f);
			glEnable(GL_LIGHT6);
		glPopMatrix();

		glPushMatrix();
			updateFiammaInterna(lightCeri2Pos,fiammaLight,0.0005);
			glLightfv(GL_LIGHT7, GL_DIFFUSE, fiammaLight);
			glLightfv(GL_LIGHT7, GL_POSITION, lightCeri2Pos);
			glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, 0.05f);
			glEnable(GL_LIGHT7);
		glPopMatrix();

		glPushMatrix();
			updateFiammaInterna(lightCeri3Pos,fiammaLight,0.0005);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, fiammaLight);
			glLightfv(GL_LIGHT0, GL_POSITION, lightCeri3Pos);
			glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
			glEnable(GL_LIGHT0);
		glPopMatrix();

		glPushMatrix();
			updateFiammaInterna(lightCeri4Pos,fiammaLight,0.0005);
			glLightfv(GL_LIGHT2, GL_DIFFUSE, fiammaLight);
			glLightfv(GL_LIGHT2, GL_POSITION, lightCeri1Pos);
			glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.05f);
			glEnable(GL_LIGHT2);
		glPopMatrix();

		glDisable(GL_LIGHT1);

		//PRIMA DISEGNO IL PAVIMENTO COL RIFLESSO E POI GLI INTERNI
		glCallList(interniList);





		//PIPISTRELLIIIIIIIII
		glPushMatrix();
			glTranslatef(89.0,60.0,-186.0);
			glRotatef(30.0,0.0,0.0,1.0);
			drawPipistrelloInVolo(30.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(89.0,70.0,-186.0);
			glRotatef(-40.0,0.0,1.0,0.0);
			glTranslatef(5.0,0.0,0.0);
			glRotatef(-40.0,0.0,0.0,1.0);
			drawPipistrelloInVolo(20.0);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(80.0,50.0,-196.0);
			glRotatef(90.0,0.0,1.0,0.0);
			glTranslatef(9.0,0.0,-10.0);
			glRotatef(20.0,0.0,0.0,1.0);
			drawPipistrelloInVolo(30.0);
		glPopMatrix();


		glDisable(GL_LIGHT2);		
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
		glDisable(GL_LIGHT6);
		glDisable(GL_LIGHT7);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);

		drawFiammelleEsterne();
		drawFiammelleInterne();





/*********************** DISEGNO GLI ESTERNI E LE LUCI SUDDIVISI PER INTERAZIONE *****************************/
/*************************************************************************************************************
	
	lightLampione1Pos={3.0, 35.0, -(320.0*3)/5.0};
	lightLampione2Pos={3.0, 35.0, -320.0/5.0};
	lightLampione3Pos={54.0, 50.0+3.5, 0};
	lightLampione4Pos={124.0, 50.0+3.5, 0};
	lightLampione5Pos={175.0, 35.0, -320.0/5.0};
	lightLampione6Pos={175.0, 35.0, -(320.0*3)/5.0}; */

		glEnable(GL_LIGHT1);

		glPushMatrix();
			updateFiammaEsterna(lightLampione1Pos,fiammaLight,0.0005);
			glLightfv(GL_LIGHT2, GL_DIFFUSE, fiammaLight);
			glLightfv(GL_LIGHT2, GL_POSITION, lightLampione1Pos);
			glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.05f);
			
			glTranslatef(lightLampione1Pos[0],lightLampione1Pos[1],lightLampione1Pos[2]);
			//glutWireCube(20.0);
		glPopMatrix();

		glPushMatrix();
			updateFiammaEsterna(lightLampione2Pos,fiammaLight,0.0005);
			glLightfv(GL_LIGHT3, GL_DIFFUSE, fiammaLight);
			glLightfv(GL_LIGHT3, GL_POSITION, lightLampione2Pos);
			glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.05f);
			
			glTranslatef(lightLampione2Pos[0],lightLampione2Pos[1],lightLampione2Pos[2]);
			//glutWireCube(20.0);
		glPopMatrix();

		glPushMatrix();
			updateFiammaEsterna(lightLampione3Pos,fiammaLight,0.0005);
			glLightfv(GL_LIGHT4, GL_DIFFUSE, fiammaLight);
			glLightfv(GL_LIGHT4, GL_POSITION, lightLampione3Pos);
			glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.05f);
			
			glTranslatef(lightLampione3Pos[0],lightLampione3Pos[1],lightLampione3Pos[2]);
			//glutWireCube(20.0);
		glPopMatrix();

		glPushMatrix();
			updateFiammaEsterna(lightLampione4Pos,fiammaLight,0.0005);
			glLightfv(GL_LIGHT5, GL_DIFFUSE, fiammaLight);
			glLightfv(GL_LIGHT5, GL_POSITION, lightLampione4Pos);
			glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.05f);
			
			glTranslatef(lightLampione4Pos[0],lightLampione4Pos[1],lightLampione4Pos[2]);
			//glutWireCube(20.0);
		glPopMatrix();

		glPushMatrix();
			updateFiammaEsterna(lightLampione5Pos,fiammaLight,0.0005);
			glLightfv(GL_LIGHT6, GL_DIFFUSE, fiammaLight);
			glLightfv(GL_LIGHT6, GL_POSITION, lightLampione5Pos);
			glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, 0.05f);
			
			glTranslatef(lightLampione5Pos[0],lightLampione5Pos[1],lightLampione5Pos[2]);
			//glutWireCube(20.0);
		glPopMatrix();

		glPushMatrix();
			updateFiammaEsterna(lightLampione6Pos,fiammaLight,0.0005);
			glLightfv(GL_LIGHT7, GL_DIFFUSE, fiammaLight);
			glLightfv(GL_LIGHT7, GL_POSITION, lightLampione6Pos);
			glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, 0.05f);

			glTranslatef(lightLampione6Pos[0],lightLampione6Pos[1],lightLampione6Pos[2]);
			//glutWireCube(20.0);
		glPopMatrix();
	
					
		//TUTTO ACCESO
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		glEnable(GL_LIGHT3);
		glEnable(GL_LIGHT4);
		glEnable(GL_LIGHT5);
		glEnable(GL_LIGHT6);
		glEnable(GL_LIGHT7);


		//disegno la base della chiesa
		glCallList(basamentoList);


		//LASCIO ABILITATE SOLO LE FRONTALI
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT6);
		glDisable(GL_LIGHT7);
	

		//disegno la facciata della chiesa
		glCallList(facciataList);

		resetMaterial();

		//RIMANE SOLO LA LUCE DELLA LUNA
		glDisable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
		

		//disegno il retro della chiesa influenzato dalla luce della luna
		glCallList(retroIlluminatoList);


		//disegna la parte alta illuminata dalla luna
		glCallList(altaIlluminataList);

		
		//ILLUMINO IL LATO DX CON LE LANTERNE A DX
		glEnable(GL_LIGHT6);
		glEnable(GL_LIGHT7);


		//parte bassa illuminata
		glCallList(bassaIlluminataList);
		

		//SPENGO TUTTO X LE PARTI NON ILLUMINATE
		glDisable(GL_LIGHT6);
		glDisable(GL_LIGHT7);
		glDisable(GL_LIGHT1);
		

		//disegna il retro non illuminato della chiesa
		glCallList(retroNonIlluminatoList);

		
		//disegna la parte alta non illuminata
		glCallList(altaNonIlluminataList);


		//ILLUMINO IL LATO SX CON LE LANTERNE DI SX
		glEnable(GL_LIGHT2);
		glEnable(GL_LIGHT3);

	
		//disegno parte bassa non illuminata
		glCallList(bassaNonIlluminataList);

		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);	

		glEnable(GL_LIGHT1);

			glPushMatrix();
				glTranslatef(89.0,-altezzaBaseChiesa,-373.0/2.0);
				//disegno il terreno sabbioso
				glCallList(sandList);
			glPopMatrix();

		glDisable(GL_LIGHT1);


		glDisable(GL_TEXTURE_2D);

		resetMaterial();

		glDisable(GL_FOG);
		glMaterialfv(GL_FRONT,GL_EMISSION,specular);
		glMaterialfv(GL_BACK,GL_EMISSION,specular);
		drawMoon();

	glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	
	glutSwapBuffers();
}




/* ---------------------------------------------------------- */
void idle(void)
{
	/*
	if (isMoving()) 
		glutPostRedisplay();

	updateLight = (updateLight+1) % 80000;

	
	
	if(updateLight == 0) {
		glutPostRedisplay();
		//printf("%d\n",updateLight);
	}
*/

	glutPostRedisplay();

}
