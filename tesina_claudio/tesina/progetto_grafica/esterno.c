#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     
#include <GL/glut.h>  
#include "utils.h"
#include "light.h"
#include "oggetti.h"
#include "texture.h"

#ifndef M_PI
#define M_PI 3.14159265
#endif

/* privati */
void drawPaliAvanti();
void drawPaliDietroIlluminati();
void drawPaliDietroNonIlluminati();
void drawRosone(float r , int s);
void drawPorta(float length, float height, float width);

/* prototipi */
void drawPareteDestraAlta();
void drawPareteDestraBassa();

void drawPareteSinistraAlta();
void drawPareteSinistraBassa();

void drawRetroIlluminato();
void drawRetroNonIlluminato();

void drawTettoIlluminato();
void drawTettoNonIlluminato();

void drawFacciata();
void drawBasamento(float altezza, float sporgenza, int gradini);

void costruisciLampioniFronte();
void costruisciLampioniSinistra();
void costruisciLampioniDestra();

void costruisciRosone();


/*__________________________________________________________________________________________*/
/*-------------------------------BASE PIRAMIDALE DELLA CHIESA-------------------------------*/

void drawBasamento(float altezza, float sporgenza, int gradini){

	float dimx = 178.0;
	float dimz = 373.0;
	int i;

	glEnable(GL_TEXTURE_2D);

		setTexture(TEXTURE_MARMO);
		setTextureScaling(30);

	glPushMatrix();

	glTranslatef(0,-altezza/2,0);

	glBegin(GL_QUADS);

		for(i=0;i<gradini;i++){
		
			poliTex(
				"basen",
				0-(i*sporgenza), (altezza/2) - i*(altezza/gradini), 3+(i*sporgenza),
				dimx+(i*sporgenza), (altezza/2) - i*(altezza/gradini), 3+(i*sporgenza),
				dimx+(i*sporgenza), (altezza/2)- i*(altezza/gradini), -dimz-(i*sporgenza),	
				0-(i*sporgenza), (altezza/2) - i*(altezza/gradini), -dimz-(sporgenza*i),	
				
				0-(i*sporgenza), (altezza/2) - (i+1)*(altezza/gradini), 3+(i*sporgenza),
				dimx+(i*sporgenza), (altezza/2) - (i+1)*(altezza/gradini), 3+(i*sporgenza),
				dimx+(i*sporgenza), (altezza/2) - (i+1)*(altezza/gradini), -dimz-(i*sporgenza),	
				0-(i*sporgenza), (altezza/2) - (i+1)*(altezza/gradini), -dimz-(sporgenza*i)
			);

		}

	glEnd();

	glPopMatrix();

}

/*------------------------FACCIATA CON STIPITI E ROSONE-------------------------------------*/
/*------------------------------------------------------------------------------------------*/

void drawFacciata(){

		glEnable(GL_TEXTURE_2D);

		setTexture(TEXTURE_MARMO_BIANCO);
		setTextureScaling(30);

		glPushMatrix();

			glTranslatef(69.0,120.0,0.0);
			drawQuadratoBucato(40.0,8);

		glPopMatrix();		

		glBegin(GL_QUADS);

			//frontcenter1
			verTex(
				"frontcenter1",
				39,0,0,
				69,0,0,
				69,50,0,
				39,50,0
			);

			//frontcenter1.1
			verTex(
				"frontcenter1.1",
				39,50,0,
				69,50,0,
				69,120,0,
				39,120,0
			);

			//frontcenter1.1.1
			verTex(
				"frontcenter1.1",
				39,120,0,
				69,120,0,
				69,170,0,
				39,170,0
			);

			//frontcenter2
			verTex(
				"frontcenter2",
				69,50,0,
				109,50,0,
				109,120,0,
				69,120,0
			);

			//soprabuco
			verTex(
				"soprabuco",
				69,160,0,
				109,160,0,
				109,170,0,
				69,170,0);

			//frontcenter3
			verTex(
				"frontcenter3",
				109,0,0,
				139,0,0,
				139,50,0,
				109,50,0
			);

			//frontcenter3.3
			verTex(
				"frontcenter3.3",
				109,50,0,
				139,50,0,
				139,120,0,
				109,120,0
			);

			//frontcenter3.3.3
			verTex(
				"frontcenter3.3",
				109,120,0,
				139,120,0,
				139,170,0,
				109,170,0
			);


			//frontcenter4
			verTex(
				"frontcenter4",
				39,170,0,
				139,170,0,
				89,245,0,
				39,170,0
			);

			//frontsx1
			verTex(
				"frontsx1",
				5,0,0,
				12.5,0,0,
				12.5,30,0,
				5,30,0
			);

			//frontsx1.1
			verTex(
				"frontsx1.1",
				5,30,0,
				12.5,30,0,
				12.5,66.25,0,
				5,55,0
			);

			//frontsx2
			verTex(
				"frontsx2",
				12.5,30,0,
				27.5,30,0,
				27.5,88.75,0,
				12.5,66.25,0
			);

			//frontsx3
			verTex(
				"frontsx3",
				27.5,0,0,
				35,0,0,
				35,100,0,
				27.5,88.75,0
			);

			//frontdx3
			verTex(
				"frontdx3",
				165.5,0,0,
				173,0,0,
				173,30,0,
				165.5,30,0
			);

			//frontdx3.1
			verTex(
				"frontdx3.1",
				165.5,30,0,
				173,30,0,
				173,55,0,
				165.5,66.25,0
			);

			//frontdx2
			verTex(
				"frontdx2",
				150.5,30,0,
				165.5,30,0,
				165.5,66.25,0,
				150.5,88.75,0
			);

			//frontdx1
			verTex(
				"frontdx1",
				143,0,0,
				150.5,0,0,
				150.5,30,0,
				143,30,0
			);

			//frontdx1.1
			verTex(
				"frontdx1.1",
				143,30,0,
				150.5,30,0,
				150.5,88.75,0,
				143,100,0
			);

	
		glEnd();

		
		setTexture(TEXTURE_LEGNO_MASSICCIO);


		glBegin(GL_QUADS);

		//DISEGNO GLI STIPITI
			
			//stipitesx
			poliTex(
				"stipitesx1",
				11.6,0,1,
				12.6,0,1,
				12.6,29.9,1,
				11.6,29.9,1,

				11.6,0,-4,
				12.6,0,-4,
				12.6,29.9,-4,
				11.6,29.9,-4

			);
		
			poliTex(
				"stipitesx2",
				27.4,0,1,
				28.4,0,1,
				28.4,29.9,1,
				27.4,29.9,1,

				27.4,0,-4,
				28.4,0,-4,
				28.4,29.9,-4,
				27.4,29.9,-4

			);

			poliTex(
				"stipitesx3",
				11.6,29.9,1,
				28.4,29.9,1,
				28.4,30.9,1,
				11.6,30.9,1,

				11.6,29.9,-4,
				28.4,29.9,-4,
				28.4,30.9,-4,
				11.6,30.9,-4

			);
		
	
			//stipitedx
			poliTex(
				"stipitedx1",
				138+11.6,0,1,
				138+12.6,0,1,
				138+12.6,29.9,1,
				138+11.6,29.9,1,

				138+11.6,0,-4,
				138+12.6,0,-4,
				138+12.6,29.9,-4,
				138+11.6,29.9,-4

			);
		
			poliTex(
				"stipitedx2",
				138+27.4,0,1,
				138+28.4,0,1,
				138+28.4,29.9,1,
				138+27.4,29.9,1,

				138+27.4,0,-4,
				138+28.4,0,-4,
				138+28.4,29.9,-4,
				138+27.4,29.9,-4

			);

			poliTex(
				"stipitedx3",
				138+11.6,29.9,1,
				138+28.4,29.9,1,
				138+28.4,30.9,1,
				138+11.6,30.9,1,

				138+11.6,29.9,-4,
				138+28.4,29.9,-4,
				138+28.4,30.9,-4,
				138+11.6,30.9,-4

			);
			
			//stipitecentro
			poliTex(
				"stipitecentro1",
				68.1,0,1,
				69.1,0,1,
				69.1,49.9,1,
				68.1,49.9,1,

				68.1,0,-4,
				69.1,0,-4,
				69.1,49.9,-4,
				68.1,49.9,-4

			);
		
			poliTex(
				"stipitecentro2",
				108.9,0,1,
				109.9,0,1,
				109.9,49.9,1,
				108.9,49.9,1,

				108.9,0,-4,
				109.9,0,-4,
				109.9,49.9,-4,
				108.9,49.9,-4

			);

			poliTex(
				"stipitedx3",
				68.1,49.9,1,
				109.9,49.9,1,
				109.9,50.9,1,
				68.1,50.9,1,

				68.1,49.9,-4,
				109.9,49.9,-4,
				109.9,50.9,-4,
				68.1,50.9,-4

			);
			


		glEnd();

		drawPaliAvanti();

		/* DISEGNO LE PORTE */

		glPushMatrix();
	
			glTranslatef(0,0,-0.5);

			glPushMatrix();
				glTranslatef(12.6,0,0);
				
				glRotatef(85,0,1,0);
				drawPorta(14.9,30.0,1.0);
			glPopMatrix();


			glPushMatrix();
				glTranslatef(69.1,0,-1);
			
				glRotatef(20,0,1,0);
				drawPorta(19.9,50.0,2.0);
			glPopMatrix();

			glDisable(GL_CULL_FACE);
		
			glPushMatrix();
				glTranslatef(143+7.4+15,0,0);
				
				glRotatef(-60,0,1,0);
				glScalef(-1,1,1);
				drawPorta(14.9,30.0,1.0);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(109,0,-1);
				
				glRotatef(-90,0,1,0);
				glScalef(-1,1,1);
				drawPorta(19.9,50.0,2.0);
			glPopMatrix();

			glEnable(GL_CULL_FACE);

		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
			
		costruisciLampioniFronte();

		glEnable(GL_TEXTURE_2D);

}

void costruisciRosone(){

	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

		glPushMatrix();

			glTranslatef(89,140,-1.5);
			drawRosone(20.0,12);

		glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

}

/*--------------------------------------PALI E SEPARAZIONI PARETI-----------------------------------*/
/*--------------------------------------------------------------------------------------------------*/

void drawPaliAvanti(){

	glEnable(GL_TEXTURE_2D);
	setTexture(TEXTURE_MARMO);
	setTextureScaling(30);

	glBegin(GL_QUADS);

			//separazionesx1
			verTex(
				"separazionesx1",
				35,0,3,
				39,0,3,
				39,170,3,
				35,170,3
			);

			//separazionesx2
			verTex(
				"separazionesx2",
				39,0,3,
				39,0,0,
				39,170,0,
				39,170,3
			);

			//separazionesx3
			verTex(
				"separazionesx3",
				35,0,0,
				35,0,3,
				35,170,3,
				35,170,0
			);
	//separazionesxdietro
			poliTex(
				"separazionesxdietro",
				35,170,-323,
				35,170,-320,
				39,170,-320,
				39,170,-323,

				35,0,-323,
				35,0,-320,
				39,0,-320,
				39,0,-323

			);
			//separazionedx1
			verTex(
				"separazionedx1",
				139,0,3,
				143,0,3,
				143,170,3,
				139,170,3
			);

			//separazionedx2
			verTex(
				"separazionedx2",
				143,0,3,
				143,0,0,
				143,170,0,
				143,170,3
			);

			//separazionedx3
			verTex(
				"separazionedx3",
				139,0,0,
				139,0,3,
				139,170,3,
				139,170,0
			);

			//bordosx1
			verTex(
				"bordosx1",
				0,0,3,
				5,0,3,
				5,55,3,
				0,55,3
			);

			//bordosx2
			verTex(
				"bordosx2",
				0,0,0,
				0,0,3,
				0,55,3,
				0,55,0
			);

			//bordosx3
			verTex(
				"bordosx3",
				5,0,3,
				5,0,0,
				5,55,0,
				5,55,3
			);

			//bordosx4
			verTex(
				"bordosx4",
				0,0,0,
				0,55,0,
				5,55,0,
				5,0,0

			);

			//bordodx1
			verTex(
				"bordodx1",
				173,0,3,
				178,0,3,
				178,55,3,
				173,55,3
			);

			//bordodx2
			verTex(
				"bordodx2",
				173,0,0,
				173,0,3,
				173,55,3,
				173,55,0
			);

			//bordodx3
			verTex(
				"bordodx3",
				178,0,3,
				178,0,0,
				178,55,0,
				178,55,3
			);

			//bordodx4
			verTex(
				"bordodx4",
				173,0,0,
				173,55,0,
				178,55,0,
				178,0,0

			);

	glEnd();
}

void drawPaliDietroNonIlluminati(){

	glEnable(GL_TEXTURE_2D);
	setTexture(TEXTURE_MARMO);
	setTextureScaling(30);

	glBegin(GL_QUADS);
		
		//bordosxdietro
			poliTex(
				"bordosxdietro",
				0,55,-323,
				0,55,-320,
				5,55,-320,
				5,55,-323,
		
				0,0,-323,
				0,0,-320,
				5,0,-320,
				5,0,-323
			);

		//separazionesxdietro
			poliTex(
				"separazionesxdietro",
				35,170,-323,
				35,170,-320,
				39,170,-320,
				39,170,-323,

				35,0,-323,
				35,0,-320,
				39,0,-320,
				39,0,-323

			);
	glEnd();

}

void drawPaliDietroIlluminati(){

	glEnable(GL_TEXTURE_2D);
	setTexture(TEXTURE_MARMO);
	setTextureScaling(30);
	
	glBegin(GL_QUADS);

		//bordodxdietro
			poliTex(
				"bordodxdietro",
				173,55,-323,
				173,55,-320,
				178,55,-320,
				178,55,-323,

				173,0,-323,
				173,0,-320,
				178,0,-320,
				178,0,-323
			);

		//separazionedxdietro
			poliTex(
				"separazionedxdietro",
				139,170,-323,
				139,170,-320,
				143,170,-320,
				143,170,-323,

				139,0,-323,
				139,0,-320,
				143,0,-320,
				143,0,-323

			);

		glEnd();


}

/*-------------------------------TETTO DIVISO PER L'ILLUMINAZIONE----------------------------------*/
/*------------------------------------------------------------------------------------------------------------*/

void drawTettoNonIlluminato(){

	glEnable(GL_TEXTURE_2D);
	setTexture(TEXTURE_MARMO);
	setTextureScaling(50);

	glBegin(GL_QUADS);

		//tettosxbasso
			poliTex(
				"tettosxbasso",
				35,100,3,
				39,100,3,
				39,100,-323,
				35,100,-323,
				
				0,55,3,
				5,55,3,
				5,55,-323,
				0,55,-323			
			);

		
		//tettosxaltro
			poliTex(
				"tettosxalto",
				85,245,3,
				89,245,3,
				89,245,-320,
				85,245,-320,
				
				35,170,3,
				39,170,3,
				39,170,-320,
				35,170,-320			
			);



	glEnd();

}


void drawTettoIlluminato(){

	glEnable(GL_TEXTURE_2D);
	setTexture(TEXTURE_MARMO);

	setTextureScaling(50);

	glBegin(GL_QUADS);


			//tettodxalto
			poliTex(
				"tettodxalto",
				89,245,3,
				93,245,3,
				93,245,-320,
				89,245,-320,
				
				139,170,3,
				143,170,3,
				143,170,-320,
				139,170,-320			
			);

			//tettodxbasso
			poliTex(
				"tettodxbasso",
				139,100,3,
				143,100,3,
				143,100,-323,
				139,100,-323,
				
				173,55,3,
				178,55,3,
				178,55,-323,
				173,55,-323			
			);

			//punta
			poliTex(
				"punta",
				85,245,3,
				93,245,3,
				89,251,3,
				89,251,3,
				
				85,245,-320,
				93,245,-320,
				89,251,-320,
				89,251,-320			
			);	
			
			
	glEnd();

	//tettodietro
			
	glPushMatrix();

		glTranslatef(89,170,-320);
		glRotatef(180,0,1,0);

		drawMezzoCono(54,81,4,32);		

	glPopMatrix();


}

/*---------------------------PARETI DIVISE PER L'ILLUMINAZIONE--------------------------------*/
/*--------------------------------------------------------------------------------------------*/

void drawPareteSinistraAlta(){

	glEnable(GL_TEXTURE_2D);
	setTexture(TEXTURE_MARMO_BIANCO);
	setTextureScaling(40);

	glBegin(GL_QUADS);
				
			//sxalta
			verTex(
				"sxalta",
				39,100,0,
				39,170,0,
				39,170,-320,
				39,100,-320
			);
		
	glEnd();

}


void drawPareteSinistraBassa(){

	glEnable(GL_TEXTURE_2D);
	setTexture(TEXTURE_MARMO_BIANCO);
	setTextureScaling(40);

	glBegin(GL_QUADS);

			//sxbassa1.1
			verTex(
				"sxbassa1.1",
				3,0,0,
				3,25,0,
				3,25,-64,
				3,0,-64
			);

			//sxbassa1.2
			verTex(
				"sxbassa1.2",
				3,25,0,
				3,55,0,
				3,55,-64,
				3,25,-64
			);

			//sxbassa2.1
			verTex(
				"sxbassa2.1",
				3,0,-64,
				3,25,-64,
				3,25,-128,
				3,0,-128
			);

			//sxbassa2.2
			verTex(
				"sxbassa2.2",
				3,25,-64,
				3,55,-64,
				3,55,-128,
				3,25,-128
			);

			//sxbassa3.1
			verTex(
				"sxbassa3.1",
				3,0,-128,
				3,25,-128,
				3,25,-192,
				3,0,-192
			);


			//sxbassa3.2
			verTex(
				"sxbassa3.2",
				3,25,-128,
				3,55,-128,
				3,55,-192,
				3,25,-192
			);

			//sxbassa4.1
			verTex(
				"sxbassa4.1",
				3,0,-192,
				3,25,-192,
				3,25,-256,
				3,0,-256
			);

			//sxbassa4.2
			verTex(
				"sxbassa4.2",
				3,25,-192,
				3,55,-192,
				3,55,-256,
				3,25,-256
			);

			//sxbassa5.1
			verTex(
				"sxbassa5.1",
				3,0,-256,
				3,25,-256,
				3,25,-320,
				3,0,-320
			);

			//sxbassa5.1
			verTex(
				"sxbassa5.1",
				3,25,-256,
				3,55,-256,
				3,55,-320,
				3,25,-320
			);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	costruisciLampioniSinistra();

	glEnable(GL_TEXTURE_2D);
}


void drawPareteDestraAlta(){

		glEnable(GL_TEXTURE_2D);
		setTexture(TEXTURE_MARMO_BIANCO);
		setTextureScaling(40);

		glBegin(GL_QUADS);
				
			//dxalta
			verTex(
				"dxalta",
				139,100,0,
				139,100,-320,
				139,170,-320,
				139,170,0
			);

		glEnd();


	
}

void drawPareteDestraBassa(){

	glEnable(GL_TEXTURE_2D);
	setTexture(TEXTURE_MARMO_BIANCO);
	setTextureScaling(40);

	glBegin(GL_QUADS);

			//dxbassa1.1
			verTex(
				"dxbassa1.1",
				175,0,0,
				175,0,-64,
				175,25,-64,
				175,25,0
			);

			//dxbassa1.2
			verTex(
				"dxbassa1.2",
				175,25,0,
				175,25,-64,
				175,55,-64,
				175,55,0
			);

			//dxbassa2.1
			verTex(
				"dxbassa2.1",
				175,0,-64,
				175,0,-128,
				175,25,-128,
				175,25,-64
			);

			//dxbassa2.2
			verTex(
				"dxbassa2.2",
				175,25,-64,
				175,25,-128,
				175,55,-128,
				175,55,-64
			);
		
			//dxbassa3.1
			verTex(
				"dxbassa3.1",
				175,0,-128,
				175,0,-192,
				175,25,-192,
				175,25,-128
			);

			//dxbassa3.2
			verTex(
				"dxbassa3.2",
				175,25,-128,
				175,25,-192,
				175,55,-192,
				175,55,-128
			);


			//dxbassa4.1
			verTex(
				"dxbassa4.1",
				175,0,-192,
				175,0,-256,
				175,25,-256,
				175,25,-192
			);

			//dxbassa4.2
			verTex(
				"dxbassa4.2",
				175,25,-192,
				175,25,-256,
				175,55,-256,
				175,55,-192
			);

			//dxbassa5.1
			verTex(
				"dxbassa5.1",
				175,0,-256,
				175,0,-320,
				175,25,-320,
				175,25,-256
			);

			//dxbassa5.2
			verTex(
				"dxbassa5.2",
				175,25,-256,
				175,25,-320,
				175,55,-320,
				175,55,-256
			);
				
	glEnd();
	
	glDisable(GL_TEXTURE_2D);

	costruisciLampioniDestra();

	glEnable(GL_TEXTURE_2D);
}

/*________________________________RETRO DIVISO PER ILLUMINAZIONE DELLA LUNA__________________________*/
/*---------------------------------------------------------------------------------------------------*/

void drawRetroNonIlluminato(){

	glEnable(GL_TEXTURE_2D);

	drawPaliDietroNonIlluminati();

	setTexture(TEXTURE_MARMO_BIANCO);
	setTextureScaling(40);

//è il retro sx non illuminato dalla luna
	glPushMatrix();
				
		glTranslatef(20,0.0,-320.0);	

		glRotatef(180.0,0.0,1.0,0.0);

		drawSemiCilindroOutside(15.0,40.0,32);
	
		//disegno il quarto di sfera sopra
		glPushMatrix();
			glTranslatef(0,40,0);
			drawSphereQuarterOutside(15,32,32);
	
		glPopMatrix();
	glPopMatrix();

		
	glPushMatrix();

		glTranslatef(35,40,-320);
		glRotatef(180,0,1,0);
		drawSuperficieArcata(15,16);
	
	glPopMatrix();
	


		glBegin(GL_QUADS);

			//triangoletto dietrosx
			verTex(
				"dietrosx",
				35,55,-320,
				5,55,-320,
				35,100,-320,
				35,55,-320
			);

			//bordosxdietro
			poliTex(
				"bordosxdietro",
				0,55,-323,
				0,55,-320,
				5,55,-320,
				5,55,-323,
		
				0,0,-323,
				0,0,-320,
				5,0,-320,
				5,0,-323
			);

			//separazionesxdietro
			poliTex(
				"separazionesxdietro",
				35,170,-323,
				35,170,-320,
				39,170,-320,
				39,170,-323,

				35,0,-323,
				35,0,-320,
				39,0,-320,
				39,0,-323

			);

		glEnd();


}


void drawRetroIlluminato(){

	glEnable(GL_TEXTURE_2D);

	drawPaliDietroIlluminati();

	setTexture(TEXTURE_MARMO_BIANCO);
	setTextureScaling(40);

	glPushMatrix();
		
		glTranslatef(89.0,0.0,-320.0);		

		glRotatef(180.0,0.0,1.0,0.0);

		drawSemiCilindroOutside(50.0,170.0,36);

	glPopMatrix();

	glPushMatrix();

		glTranslatef(158.0,0,-320);

		glRotatef(180.0,0.0,1.0,0.0);

		drawSemiCilindroOutside(15.0,40.0,32);

		//disegno il quarto di sfera sopra

		glPushMatrix();
			glTranslatef(0,40,0);
			drawSphereQuarterOutside(15,32,32);
		glPopMatrix();

	glPopMatrix();

	//dx
	glPushMatrix();

		glTranslatef(35,40,-320);
		glTranslatef(138,0,0);
	
		glRotatef(180,0,1,0);
		drawSuperficieArcata(15,16);
	
	glPopMatrix();



		glBegin(GL_QUADS);

		
			//triangoletto dietrodx
			verTex(
				"dietrodx",
				143,55,-320,
				143,100,-320,
				173,55,-320,
				143,55,-320
			);

			//bordodxdietro
			poliTex(
				"bordodxdietro",
				173,55,-323,
				173,55,-320,
				178,55,-320,
				178,55,-323,

				173,0,-323,
				173,0,-320,
				178,0,-320,
				178,0,-323
			);
			//separazionedxdietro
			poliTex(
				"separazionedxdietro",
				139,170,-323,
				139,170,-320,
				143,170,-320,
				143,170,-323,

				139,0,-323,
				139,0,-320,
				143,0,-320,
				143,0,-323

			);


		glEnd();

}


void drawPorta(float length, float height, float width) {

	glEnable(GL_TEXTURE_2D);
	setTexture(TEXTURE_LEGNO_MASSICCIO);
	setTextureScaling(30);

	glBegin(GL_QUADS);

		//porta
		poliTex(
			"porta",
			0.0,0.0,width,
			length,0.0,width,
			length,height,width,
			0.0,height,width,
			0.0,0.0,0.0,
			length,0.0,0.0,
			length,height,0.0,
			0.0,height,0.0
		);

	glEnd();

		glPushMatrix();

			setTexture(TEXTURE_GOLD);
			setTextureScaling(20);
		
			//maniglia
			glTranslatef(4.0*length/5.0,2.0*height/5.0,width);
			glRotatef(90.0,1.0,0.0,0.0);
			glRotatef(90.0,0.0,1.0,0.0);
			drawSemiTorus(height/20.0,length/40.0,32,32);

		glPopMatrix();

}

void drawRosone(float radius, int steps) {

	int i,j;
	int stepsubdivs = 5;
	float step = 2*M_PI/steps;
	float substep = step / stepsubdivs;
	float angle0, angle1, anglem;
	float p0[3], p1[3], p2[3], p3[3];
	p0[0] = 0.0; 	p0[1] = 0.0; 	p0[2] = 0.0; 
	float color[4];

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	glPushMatrix();

	glNormal3f(0.0,0.0,1.0);

//	printf("step=%f, substep=%f\n",step,substep);

	for(i=0; i<steps; i++) {

		angle0 = i*step;
		angle1 = (i+1)*step;
		anglem = (i+0.5)*step;

		p1[0] = radius*cos(angle0); 	p1[1] = radius*sin(angle0); 	p1[2] = 0.0; 
		p2[0] = radius*0.7*cos(anglem);	p2[1] = radius*0.7*sin(anglem);	p2[2] = 0.0; 
		p3[0] = radius*cos(angle1); 	p3[1] = radius*sin(angle1); 	p3[2] = 0.0; 

		glBegin(GL_TRIANGLES);

			randomColor(color);
			glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,color);
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,color);

			glVertex3fv(p0);	
			glVertex3fv(p1);	
			glVertex3fv(p2);

			randomColor(color);
			glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,color);
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,color);

			glVertex3fv(p0);	
			glVertex3fv(p2);	
			glVertex3fv(p3);

		glEnd();

		glBegin(GL_TRIANGLE_FAN);

			randomColor(color);
			glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,color);
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,color);			

			glVertex3fv(p2);	
			glVertex3fv(p1);

			for(j=1; j<=stepsubdivs; j++) {

				glVertex3f(radius*cos(angle0+j*substep),radius*sin(angle0+j*substep),0.0);

			}	

		glEnd();
		
	}

	glPopMatrix();

	glEnable(GL_CULL_FACE);
	
}

void costruisciLampioniFronte() {


	glPushMatrix();
	glTranslatef(54.0, 50.0+3.5, 0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawLampione(3.0, 6.0, 7.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(124.0, 50.0+3.5, 0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawLampione(3.0, 6.0, 7.0);
	glPopMatrix();
	
}

void costruisciLampioniSinistra(){


	glPushMatrix();
	glTranslatef(3.0, 35.0, -320.0/5.0);
	drawLampione(3.0, 6.0, 7.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.0, 35.0, (-320.0*3.0)/5.0);
	drawLampione(3.0, 6.0, 7.0);
	glPopMatrix();


}

void costruisciLampioniDestra() {

	glPushMatrix();
	glTranslatef(175.0, 35.0, -320.0/5.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	drawLampione(3.0, 6.0, 7.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(175.0, 35.0, (-320.0*3.0)/5.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	drawLampione(3.0, 6.0, 7.0);
	glPopMatrix();

}

void drawFiammelleEsterne() {


/************************************************** LAMPIONI A SINISTRA ***********************************************/

		glPushMatrix();				
			glTranslatef(-1.5, 26.2, -(320.0*3)/5.0);
			drawFiammellaLampadario();
		glPopMatrix();

		glPushMatrix();				
			glTranslatef(-1.5, 26.2, -320.0/5.0);
			drawFiammellaLampadario();
		glPopMatrix();


/************************************************** LAMPIONI DAVANTI ***********************************************/

		glPushMatrix();
			
			glTranslatef(54.0, 44.7, 4.5);
			drawFiammellaLampadario();

		glPopMatrix();

		glPushMatrix();
			
			glTranslatef(124.0, 44.7, 4.5);
			drawFiammellaLampadario();

		glPopMatrix();


/************************************************** LAMPIONI A DESTRA ***********************************************/

		glPushMatrix();
			
			glTranslatef(179.5, 26.2, -320.0/5.0);
			drawFiammellaLampadario();

		glPopMatrix();

		glPushMatrix();
			
			glTranslatef(179.5, 26.2, -(320.0*3)/5.0);
			drawFiammellaLampadario();

		glPopMatrix();

		resetMaterial();

}

