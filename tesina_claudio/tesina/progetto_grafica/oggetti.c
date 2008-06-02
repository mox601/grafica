#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <GL/glut.h>
#include "utils.h"
#include "light.h"
#include "texture.h"


/* prototipi */
void drawPanchina();
void drawAltare();
void drawTovaglia();


void drawCandela(float larghezza, float altezza, int precisione) {
	
	glDisable(GL_TEXTURE_2D);
	
	setColor(1.0, 1.0, 1.0);
	drawCilindroChiuso(larghezza, altezza, precisione);

	setColor(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, altezza, 0.0);
	glVertex3f(0.0, altezza+0.5, 0.0);
	glEnd();
	
	resetMaterial();
	glEnable(GL_TEXTURE_2D);
	
}


/* La panchina viene disegnata avendo il suo vertice in basso a sinistra nell'origine. Si estenderà perciò in larghezza(lungo X), in altezza(lungo Y) e in profondità lungo i valori negativi di Z 
La panchina è profonda 7.5
La panchina è alta 11
La panchina è larga 25
*/

void drawPanchina() {

		glBegin(GL_QUADS);

			//sedile faccia sopra
			verTex("Sedile",
				0.5,5,0,
				24.5,5,0,
				24.5,5,-4,
				0.5,5,-4
			);

			//sedile faccia sotto
			verTex("Sedile",
				0.5,4.5,0,
				0.5,4.5,-4,					
				24.5,4.5,-4,
				24.5,4.5,0
			);

			//sedile faccia dietro
			verTex("Sedile",
				0.5,4.5,-4,
				0.5,5,-4,
				24.5,5,-4,
				24.5,4.5,-4
			);

			//sedile faccia davanti
			verTex("Sedile",
				0.5,4.5,0,
				24.5,4.5,0,
				24.5,5,0,
				0.5,5,0					
			);

			//sedile faccia destra
			verTex("Sedile",
				24.5,4.5,0,
				24.5,4.5,-4,
				24.5,5,-4,
				24.5,5,0
			);

			//sedile faccia sinistra
			verTex("Sedile",
				0.5,4.5,-4,
				0.5,4.5,0,
				0.5,5,0,
				0.5,5,-4					
			);


			//lato destro esterno
			verTex( "lato dx basso",
				25,0,0.5,
				25,0,-5.5,
				25,5,-5.5,
				25,5,-0.5
			);

			//lato destro interno
			verTex( "lato dx basso",
				24.5,0,-5.5,
				24.5,0,0.5,
				24.5,5,-0.5,
				24.5,5,-5.5
			);

			//lato destro sopra
			verTex( "lato dx basso",
				25,5,-0.5,
				25,5,-5.5,
				24.5,5,-5.5,
				24.5,5,-0.5
			);

			//lato destro sotto
			verTex( "lato dx basso",
				24.5,0,0,
				24.5,0,-5.5,
				25,0,-5.5,
				25,0,0
			);

			//lato destro davanti
			verTex( "lato dx basso",
				24.5,0,0.5,
				25,0,0.5,
				25,5,-0.5,
				24.5,5,-0.5
			);

			//lato destro dietro
			verTex( "lato dx basso",
				24.5,0,-5.5,
				24.5,5,-5.5,
				25,5,-5.5,
				25,0,-5.5
			);

			//lato sinistro esterno
			verTex( "lato sx basso",
				0,0,0.5,
				0,5,-0.5,
				0,5,-5.5,
				0,0,-5.5
			);

			//lato sinistro interno
			verTex( "lato sx basso",
				0.5,0,0.5,
				0.5,0,-5.5,
				0.5,5,-5.5,
				0.5,5,0
			);

			//lato sinistro sopra
			verTex( "lato sx basso",
				0,5,-0.5,
				0.5,5,-0.5,
				0.5,5,-5.5,
				0,5,-5.5
			);

			//lato sinistro sotto
			verTex( "lato sx basso",
				0,0,0.5,
				0,0,-5.5,
				0.5,0,-5.5,
				0.5,0,0.5
			);

			//lato sinistro davanti
			verTex( "lato sx basso",
				0,0,0.5,
				0.5,0,0.5,
				0.5,5,-0.5,
				0,5,-0.5
			);

			//lato sinistro dietro
			verTex( "lato sx basso",
				0,0,-5.5,
				0,5,-5.5,
				0.5,5,-5.5,
				0.5,0,-5.5
			);

			//schienale
			verTex( "lato davanti",
				0.5,8.5,-4.2,
				24.5,8.5,-4.2,
				24.5,11,-4.5,
				0.5,11,-4.5
			);
			//schienale
			verTex( "lato dietro",
				0.5,8.5,-5.5,
				0.5,11,-5.5,
				24.5,11,-5.5,
				24.5,8.5,-5.5
			);
			//schienale
			verTex( "lato sopra",
				0.5,11,-4.5,
				24.5,11,-4.5,
				24.5,11,-5.5,
				0.5,11,-5.5
			);
			//schienale
			verTex( "lato sotto",
				0.5,8.5,-4.2,
				0.5,8.5,-5.5,
				24.5,8.5,-5.5,
				24.5,8.5,-4.2
			);
			//schienale
			verTex( "lato destro",
				24.5,8.5,-4.2,
				24.5,8.5,-5.5,
				24.5,11,-5.5,
				24.5,11,-4.5
			);
			//schienale
			verTex( "lato sinistro",
				0.5,8.5,-4.2,
				0.5,11,-4.5,
				0.5,11,-5.5,
				0.5,8.5,-5.5
			);

			//a lato dello schienale
			verTex( "lato destro esterno",
				25,11,-4.5,
				25,5,-4,
				25,5,-5.5,
				25,11,-5.5
			);
			//a lato dello schienale
			verTex( "lato destro interno",
				24.5,5,-5.5,
				24.5,5,-4,
				24.5,11,-4.5,
				24.5,11,-5.5
			);
			//a lato dello schienale
			verTex( "lato destro davanti",
				24.5,5,-4,
				25,5,-4,
				25,11,-4.5,
				24.5,11,-4.5
			);
			//a lato dello schienale
			verTex( "lato destro dietro",
				24.5,5,-5.5,
				24.5,11,-5.5,
				25,11,-5.5,
				25,5,-5.5
			);
			//a lato dello schienale
			verTex( "lato destro sopra",
				24.5,11,-4.5,
				25,11,-4.5,
				25,11,-5.5,
				24.5,11,-5.5
			);

			//a lato dello schienale
			verTex( "lato sinistro esterno",
				0,5,-4,
				0,11,-4.5,
				0,11,-5.5,
				0,5,-5.5
			);
			//a lato dello schienale
			verTex( "lato sinistro interno",
				0.5,5,-4,
				0.5,5,-5.5,
				0.5,11,-5.5,
				0.5,11,-4.5
			);
			//a lato dello schienale
			verTex( "lato sinistro davanti",
				0,5,-4,
				0.5,5,-4,
				0.5,11,-4.5,
				0,11,-4.5
			);
			//a lato dello schienale
			verTex( "lato sinistro dietro",
				0,5,-5.5,
				0,11,-5.5,
				0.5,11,-5.5,
				0.5,5,-5.5
			);
			//a lato dello schienale
			verTex( "lato sinistro sopra",
				0,11,-4.5,
				0.5,11,-4.5,
				0.5,11,-5.5,
				0,11,-5.5
			);
			
			//poggia piedi(rustico)
			poliTex( "base del poggia piedi",
				0,2,-5.5,
				25,2,-5.5,
				25,2,-6.3,
				0,2,-6.3,
				0,0,-5.5,
				25,0,-5.5,
				25,0,-6.7,
				0,0,-6.7
			);		

			//asta dove poggiano i piedi
			poliTex( "base del poggia piedi",
				-0.2,2.2,-5.5,
				25.2,2.2,-5.5,
				25.2,2.2,-7,
				-0.2,2.2,-7,
				-0.2,2,-5.5,
				25.2,2,-5.5,
				25.2,2,-7,
				-0.2,2,-7
			);	

		glEnd();

}


/* Disegna l'altare.
Pongo il vertice in basso a sinistra della base nell'origine.
Dimensioni base: 30x; 12y; -18z
*/

void drawAltare() {

	glBegin(GL_QUADS);

		//base, parte inferiore, dell'altare
		poliTex("base altare",
			0,3,0,
			10,3,0,
			10,3,-8,
			0,3,-8,
			0,0,0,
			10,0,0,
			10,0,-8,
			0,0,-8
		);
	
		//base, parte superiore, dell'altare
		poliTex("base altare",
			-5,10,0,
			15,10,0,
			15,10,-8,
			-5,10,-8,			
			0,3,0,
			10,3,0,
			10,3,-8,
			0,3,-8
		);
	
		//tavolo
		poliTex("tavolo altare",
			-10,12,5,
			20,12,5,
			20,12,-13,
			-10,12,-13,			
			-10,10,5,
			20,10,5,
			20,10,-13,
			-10,10,-13
		);

	glEnd();

}

//tovaglia divisa in 7 poligoni
void drawTovaglia() {
	
	glBegin(GL_QUADS);

		//1° pezzo	
		poliTex("tovaglia del tavolo dell'altare",
			-10,12.1,-7,
			20,12.1,-7,
			14,12.1,-13,
			-4,12.1,-13,			
			-10,12,-7,
			20,12,-7,
			14,12,-13,
			-4,12,-13
		);
	
		//2° pezzo
		poliTex("tovaglia del tavolo dell'altare",
			-10,12.1,-1,
			20,12.1,-1,
			20,12.1,-7,
			-10,12.1,-7,			
			-10,12,-1,
			20,12,-1,
			20,12,-7,
			-10,12,-7
		);
	
		//3° pezzo
		poliTex("tovaglia del tavolo dell'altare",
			-4,12.1,5,
			14,12.1,5,
			20,12.1,-1,
			-10,12.1,-1,			
			-4,12,5,
			14,12,5,
			20,12,-1,
			-10,12,-1
		);
	
		//4° pezzo, cade ai piedi dell'altare dietro	
		poliTex("tovaglia del tavolo dell'altare",
			-4,12.1,-13.1,
			14,12.1,-13.1,
			5,3.1,-13.1,
			5,3.1,-13.1,			
			-4,12.1,-13,
			14,12.1,-13,
			5,3.1,-13,
			5,3.1,-13
		);
	
		//5° pezzo, cade ai piedi dell'altare davanti	
		poliTex("tovaglia del tavolo dell'altare",
			14,12.1,5.1,			
			-4,12.1,5.1,
			5,3.1,5.1,
			5,3.1,5.1,
			14,12.1,5,
			-4,12.1,5,
			5,3.1,5,
			5,3.1,5
		);
	
		//6° pezzo, cade a destra dell'altare	
		poliTex("tovaglia del tavolo dell'altare",
			20.1,12.1,-7,			
			20.1,12.1,-1,
			20.1,9.1,-4,
			20.1,9.1,-4,
			20,12.1,-7,
			20,12.1,-1,
			20,9.1,-4,
			20,9.1,-4
		);
	
		//7° pezzo, cade a sinistra dell'altare	
		poliTex("tovaglia del tavolo dell'altare",
			-10.1,12.1,-1,			
			-10.1,12.1,-7,
			-10.1,9.1,-4,
			-10.1,9.1,-4,
			-10,12.1,-1,
			-10,12.1,-7,
			-10,9.1,-4,
			-10,9.1,-4
		);

	glEnd();
	
}


/* Base altare pone la base maggiore nel punto 0, 0, 0 corrispondente al suo vertice basso a sinistra */

void drawBaseAltare(float x, float y, float z) {

	glBegin(GL_QUADS);

	//Disegno la base più grande sotto 
	poliTex("base maggiore dell'altare",
		0,y,0,
		x,y,0,
		x,y,-z,
		0,y,-z,
		0,0,0,
		x,0,0,
		x,0,-z,
		0,0,-z	
	);


	//Disegno la base più piccola sopra 
	poliTex("base minore dell'altare",
		5,2*y,-5,
		x-5,2*y,-5,
		x-5,2*y,-z+5,
		5,2*y,-z+5,
		5,y,-5,
		x-5,y,-5,
		x-5,y,-z+5,
		5,y,-z+5
	);

	glEnd();
}

/*Disegna la candela centrata nell'origine degli assi e intorno il contenitore: tutto parametrico*/
void drawCero(float radius, float altezza, int steps) {

	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);

	setTransparentColor(1.0, 0.0, 0.0);
	drawCilindro(radius, altezza, steps);
	glPushMatrix();
		glTranslatef(0.0,0.01,0.0);
		drawCandela(radius-radius*0.3, altezza-altezza*0.4, steps);
	glPopMatrix();

	glEnable(GL_CULL_FACE);

}

/*Disegna la croce con il vertice in basso a sinistra della base nell'origine*/
void drawCroce(float xperp, float yperp) {

	glBegin(GL_QUADS);
	poliTex("palo perpendicolare a terra della croce",
		0, yperp, 0,
		xperp, yperp, 0,
		xperp, yperp, -xperp,
		0, yperp, -xperp,
		0, 0, 0,
		xperp, 0, 0,
		xperp, 0, -xperp,
		0, 0, -xperp
	);

	poliTex("palo parallelo a terra della croce",
		-yperp/4-xperp/2, 3*(yperp/4), 0+0.5,
		yperp/4+3*(xperp/2), 3*(yperp/4), 0+0.5,
		yperp/4+3*(xperp/2), 3*(yperp/4), -xperp-0.5,
		-yperp/4-xperp/2, 3*(yperp/4), -xperp-0.5,
		-yperp/4-xperp/2, 3*(yperp/4)-xperp, 0+0.5,
		yperp/4+3*(xperp/2), 3*(yperp/4)-xperp, 0+0.5,
		yperp/4+3*(xperp/2), 3*(yperp/4)-xperp, -xperp-0.5,
		-yperp/4-xperp/2, 3*(yperp/4)-xperp, -xperp-0.5
	);

	glEnd();

}

/*Il quadro viene disegnato con coordinata del vertice della cornice in basso a sinistra nell'origine
	Ogni quadro ha una cornice con dimensione predefinita */
void drawQuadro(float dimx, float dimy, float sporg_corn, int ref_texture) {

	float cornice = 3.0;		
	float material_emission[4]={0.1,0.1,0.1,1.0};

	setTexture(TEXTURE_LEGNO_MASSICCIO);
	
	glBegin(GL_QUADS);	

	/*---Disegno la cornice divisa in 4 parti---*/

	//Primo listello
	verTex("listello in basso parte sporgente",
		0.0, 0.0, sporg_corn,
		2*cornice+dimx, 0.0, sporg_corn,
		cornice+dimx, cornice, 0.0,
		cornice, cornice, 0.0
	);

	//Primo listello, base
	verTex("base del listello in basso parte sporgente",
		0.0, 0.0, 0.0,
		2*cornice+dimx, 0.0, 0.0,
		2*cornice+dimx, 0.0, sporg_corn,
		0.0, 0.0, sporg_corn
	);

	//Secondo listello
	verTex("listello a destra parte sporgente",
		2*cornice+dimx, 0.0, sporg_corn,
		2*cornice+dimx, 2*cornice+dimy, sporg_corn,
		cornice+dimx, cornice+dimy, 0.0,
		cornice+dimx, cornice, 0.0
	);

	//Secondo listello, base
	verTex("base del listello a destra parte sporgente",
		2*cornice+dimx, 0.0, 0.0,
		2*cornice+dimx, 2*cornice+dimy, 0.0,
		2*cornice+dimx, 2*cornice+dimy, sporg_corn,
		2*cornice+dimx, 0.0, sporg_corn
	);

	//Terzo listello
	verTex("listello in alto parte sporgente",
		2*cornice+dimx, 2*cornice+dimy, sporg_corn,
		0.0, 2*cornice+dimy, sporg_corn,
		cornice, cornice+dimy, 0.0,
		cornice+dimx, cornice+dimy, 0.0
	);

	//Terzo listello, base
	verTex("base del listello in alto parte sporgente",
		0.0, 2*cornice+dimy, sporg_corn,
		2*cornice+dimx, 2*cornice+dimy, sporg_corn,
		2*cornice+dimx, 2*cornice+dimy, 0.0,
		0.0, 2*cornice+dimy, 0.0
	);

	//Quarto listello
	verTex("listello a sinistra parte sporgente",
		0.0, 0.0, sporg_corn,
		cornice, cornice, 0.0,
		cornice, cornice+dimy, 0.0,
		0.0, 2*cornice+dimy, sporg_corn
	);
	
	//Quarto listello, base
	verTex("base del listello a sinistra parte sporgente",
		0.0, 0.0, sporg_corn,
		0.0, 2*cornice+dimy, sporg_corn,
		0.0, 2*cornice+dimy, 0.0,
		0.0, 0.0, 0.0
	);

	glEnd();
	
	setTexture(ref_texture);

	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,material_emission);	
	
	glBegin(GL_TRIANGLE_FAN);

		glTexCoord2f(0.5,0.5); glVertex3f(cornice+(dimx/2.0), cornice+(dimy/2.0), 0.0);
		glTexCoord2f(0,1); glVertex3f(cornice, cornice, 0.0);
		glTexCoord2f(1,1); glVertex3f(cornice+dimx, cornice, 0.0);
		glTexCoord2f(1,0); glVertex3f(cornice+dimx, cornice+dimy, 0.0);
		glTexCoord2f(0,0); glVertex3f(cornice, cornice+dimy, 0.0);
		glTexCoord2f(0,1); glVertex3f(cornice, cornice, 0.0);

	glEnd();

	resetMaterial();	
	
}

/* Il lampione deve essere appeso fuori, sulle pareti della chiesa
	Le sue dimensioni si estenderanno di: 
				dimx_base_sup sull'asse x
				dimy+0.2+1/4dimy sull'asse y
				-dimx_base_sup sull'asse z

	La base inferiore del lampione è centrata nell'origine
*/


void drawLampione(float dimx_base_inf, float dimx_base_sup, float dimy) {

	glDisable(GL_TEXTURE_2D);
	
	setColor(0.0, 0.0, 0.0);
	
	glPushMatrix();
	glTranslatef(0.0, (-dimx_base_sup/2)-(dimx_base_sup/4), 0.0);
	drawTorusQuarter((dimx_base_sup/2)+(dimx_base_sup/4), 0.4, 20.0, 20.0);


	glTranslatef((-dimx_base_sup/2)-(dimx_base_sup/4), -dimy-(dimy/4), 0.0);	

	glBegin(GL_QUADS);

	poliTex("base inferiore del lampione",
		-dimx_base_inf/2, 0.1, dimx_base_inf/2,
		dimx_base_inf/2, 0.1, dimx_base_inf/2,
		dimx_base_inf/2, 0.1, -dimx_base_inf/2,
		-dimx_base_inf/2, 0.1, -dimx_base_inf/2,
		-dimx_base_inf/2, 0.0, dimx_base_inf/2,
		dimx_base_inf/2, 0.0, dimx_base_inf/2,
		dimx_base_inf/2, 0.0, -dimx_base_inf/2,
		-dimx_base_inf/2, 0.0, -dimx_base_inf/2
	);

	//Disegnamo ora i 4 paletti che daranno la forma del contenitore della luce
	
	poliTex("primo listello: a sinistra davanti",
		-dimx_base_sup/2+0.2, dimy, dimx_base_sup/2-0.2,
		-dimx_base_sup/2+0.5, dimy, dimx_base_sup/2-0.2,
		-dimx_base_sup/2+0.5, dimy, dimx_base_sup/2-0.5,
		-dimx_base_sup/2+0.2, dimy, dimx_base_sup/2-0.5,
		-dimx_base_inf/2+0.2, 0.1, dimx_base_inf/2-0.2,
		-dimx_base_inf/2+0.5, 0.1, dimx_base_inf/2-0.2,
		-dimx_base_inf/2+0.5, 0.1, dimx_base_inf/2-0.5,
		-dimx_base_inf/2+0.2, 0.1, dimx_base_inf/2-0.5
	);

	poliTex("secondo listello: a destra davanti",
		dimx_base_sup/2-0.5, dimy, dimx_base_sup/2-0.2,
		dimx_base_sup/2-0.2, dimy, dimx_base_sup/2-0.2,
		dimx_base_sup/2-0.2, dimy, dimx_base_sup/2-0.5,
		dimx_base_sup/2-0.5, dimy, dimx_base_sup/2-0.5,
		dimx_base_inf/2-0.5, 0.1, dimx_base_inf/2-0.2,
		dimx_base_inf/2-0.2, 0.1, dimx_base_inf/2-0.2,
		dimx_base_inf/2-0.2, 0.1, dimx_base_inf/2-0.5,
		dimx_base_inf/2-0.5, 0.1, dimx_base_inf/2-0.5
	);

	poliTex("terzo listello: a destra dietro",
		dimx_base_sup/2-0.5, dimy, -dimx_base_sup/2+0.5,
		dimx_base_sup/2-0.2, dimy, -dimx_base_sup/2+0.5,
		dimx_base_sup/2-0.2, dimy, -dimx_base_sup/2+0.2,
		dimx_base_sup/2-0.5, dimy, -dimx_base_sup/2+0.2,
		dimx_base_inf/2-0.5, 0.1, -dimx_base_inf/2+0.5,
		dimx_base_inf/2-0.2, 0.1, -dimx_base_inf/2+0.5,
		dimx_base_inf/2-0.2, 0.1, -dimx_base_inf/2+0.2,
		dimx_base_inf/2-0.5, 0.1, -dimx_base_inf/2+0.2
	);

	poliTex("quarto listello: a sinistra dietro",
		-dimx_base_sup/2+0.2, dimy, -dimx_base_sup/2+0.5,
		-dimx_base_sup/2+0.5, dimy, -dimx_base_sup/2+0.5,
		-dimx_base_sup/2+0.5, dimy, -dimx_base_sup/2+0.2,
		-dimx_base_sup/2+0.2, dimy, -dimx_base_sup/2+0.2,
		-dimx_base_inf/2+0.2, 0.1, -dimx_base_inf/2+0.5,
		-dimx_base_inf/2+0.5, 0.1, -dimx_base_inf/2+0.5,
		-dimx_base_inf/2+0.5, 0.1, -dimx_base_inf/2+0.2,
		-dimx_base_inf/2+0.2, 0.1, -dimx_base_inf/2+0.2
	);

	//Disegnamo ora la base superiore che chiude il contenitore della luce

	poliTex("base superiore del lampione",
		-dimx_base_sup/2, dimy+0.1, dimx_base_sup/2,
		dimx_base_sup/2, dimy+0.1, dimx_base_sup/2,
		dimx_base_sup/2, dimy+0.1, -dimx_base_sup/2,
		-dimx_base_sup/2, dimy+0.1, -dimx_base_sup/2,
		-dimx_base_sup/2, dimy, dimx_base_sup/2,
		dimx_base_sup/2, dimy, dimx_base_sup/2,
		dimx_base_sup/2, dimy, -dimx_base_sup/2,
		-dimx_base_sup/2, dimy, -dimx_base_sup/2
	);

	//Disegnamo ora il tronco di piramide sopra la base superiore del lampione
	poliTex("tronco di piramide sopra al lampione",
		-dimx_base_sup/2+dimx_base_sup/4, dimy+(dimy/4), dimx_base_sup/2-dimx_base_sup/4,
		dimx_base_sup/2-dimx_base_sup/4, dimy+(dimy/4), dimx_base_sup/2-dimx_base_sup/4,
		dimx_base_sup/2-dimx_base_sup/4, dimy+(dimy/4), -dimx_base_sup/2+dimx_base_sup/4,
		-dimx_base_sup/2+dimx_base_sup/4, dimy+(dimy/4), -dimx_base_sup/2+dimx_base_sup/4,
		-dimx_base_sup/2+0.2, dimy+0.1, dimx_base_sup/2-0.2,
		dimx_base_sup/2-0.2, dimy+0.1, dimx_base_sup/2-0.2,
		dimx_base_sup/2-0.2, dimy+0.1, -dimx_base_sup/2+0.2,
		-dimx_base_sup/2+0.2, dimy+0.1, -dimx_base_sup/2+0.2
	);

	glEnd();

	glPushMatrix();
	glTranslatef(0.0, 0.1, 0.0);
	drawCandela(dimx_base_inf/4.0, (3*dimy)/5, 30);	
	glPopMatrix();

	glPopMatrix();	


	resetMaterial();

	glEnable(GL_TEXTURE_2D);
					
}


void drawCandelabro(float altezza, float larghezza) {

	//Disegno la base
	drawCilindroChiuso(larghezza*4, 0.05*altezza, 30);
	

	glPushMatrix();
	glTranslatef(0.0, 0.05*altezza, 0.0);
	//Disegno il corpo cilindrico
	drawCilindroChiuso(larghezza, 0.6*altezza, 30);
	glPopMatrix();

	//Disegno il corpo che tiene le candele laterali
	glPushMatrix();
	glTranslatef(0.0, 0.65*altezza, 0.0);
	glRotatef(180.0, 1.0, 0.0, 0.0);
	drawSemiTorus((0.6*altezza)/2,larghezza,30,30);
	glPopMatrix();

	//Disegno il 1° dischetto portacandele
	glPushMatrix();
	glTranslatef(0.0, 0.65*altezza, 0.0);
	drawCilindroChiuso(larghezza*2, 0.01*altezza, 30);
	glTranslatef(0.0, 0.01*altezza, 0.0);
	drawCandela(larghezza/2, 0.34*altezza, 30);
	glPopMatrix();

	//Disegno il 2° dischetto portacandele
	glPushMatrix();
	glTranslatef((0.6*altezza)/2, 0.65*altezza, 0.0);
	drawCilindroChiuso(larghezza*2, 0.01*altezza, 30);
	glTranslatef(0.0, 0.01*altezza, 0.0);
	drawCandela(larghezza/2, 0.34*altezza, 30);
	glPopMatrix();

	//Disegno il 3° dischetto portacandele
	glPushMatrix();
	glTranslatef(-(0.6*altezza)/2, 0.65*altezza, 0.0);
	drawCilindroChiuso(larghezza*2, 0.01*altezza, 30);
	glTranslatef(0.0, 0.01*altezza, 0.0);
	drawCandela(larghezza/2, 0.34*altezza, 30);	
	glPopMatrix();

}


/*La colonna è centrata nell'origine
	La dimensione della parte superiore si determina automaticamente a seconda delle dimensioni passate come parametro ed è:
	dimx*dimx(lato*lato) ovvero un quadrato pari alla dimensione della base della colonna
*/
void drawColonna(float dimx, float altezza, float radius, int texture_colonna, int texture_basi) {

	setTexture(TEXTURE_MARMO);

	glBegin(GL_QUADS);
	
	poliTex("base della colonna 1a parte",
		-dimx/2, 0.025*altezza, dimx/2,
		dimx/2, 0.025*altezza, dimx/2,
		dimx/2, 0.025*altezza, -dimx/2,
		-dimx/2, 0.025*altezza, -dimx/2,
		-dimx/2, 0.0, dimx/2,
		dimx/2, 0.0, dimx/2,
		dimx/2, 0.0, -dimx/2,
		-dimx/2, 0.0, -dimx/2
	);

	poliTex("base della colonna, 2a parte",
		-radius/2, 0.05*altezza, radius/2,
		radius/2, 0.05*altezza, radius/2,
		radius/2, 0.05*altezza, -radius/2,
		-radius/2, 0.05*altezza, -radius/2,
		-radius/2, 0.025*altezza, radius/2,
		radius/2, 0.025*altezza, radius/2,
		radius/2, 0.025*altezza, -radius/2,
		-radius/2, 0.025*altezza, -radius/2
	);

	//Disegnamo ora il cilindro della colonna 

	glEnd();

	setTexture(TEXTURE_MARMO_MACCHIATO);

	glPushMatrix();
	
	glTranslatef(0.0, 0.05*altezza, 0.0);
	drawCilindro(radius/2, 0.8*altezza, 20);	
	
	glPopMatrix();

	setTexture(TEXTURE_MARMO);

	glBegin(GL_QUADS);

	//Disegnamo ora la parte superiore della colonna
	poliTex("parte superiore della colonna, 1a parte",
		-radius/2, 0.875*altezza, radius/2,
		radius/2, 0.875*altezza, radius/2,
		radius/2, 0.875*altezza, -radius/2,
		-radius/2, 0.875*altezza, -radius/2,
		-radius/2, 0.85*altezza, radius/2,
		radius/2, 0.85*altezza, radius/2,
		radius/2, 0.85*altezza, -radius/2,
		-radius/2, 0.85*altezza, -radius/2
	);
	
	poliTex("parte superiore della colonna, 2a parte",
		-radius/2-((dimx-radius)/2), 0.975*altezza, radius/2+((dimx-radius)/2),
		radius/2+((dimx-radius)/2), 0.975*altezza, radius/2+((dimx-radius)/2),
		radius/2+((dimx-radius)/2), 0.975*altezza, -radius/2-((dimx-radius)/2),
		-radius/2-((dimx-radius)/2), 0.975*altezza, -radius/2-((dimx-radius)/2),
		-radius/2, 0.875*altezza, radius/2,
		radius/2, 0.875*altezza, radius/2,
		radius/2, 0.875*altezza, -radius/2,
		-radius/2, 0.875*altezza, -radius/2
	);

	poliTex("parte superiore della colonna, 3a parte",
		-radius/2-((dimx-radius)/2), altezza, radius/2+((dimx-radius)/2),
		radius/2+((dimx-radius)/2), altezza, radius/2+((dimx-radius)/2),
		radius/2+((dimx-radius)/2), altezza, -radius/2-((dimx-radius)/2),
		-radius/2-((dimx-radius)/2), altezza, -radius/2-((dimx-radius)/2),
		-radius/2-((dimx-radius)/2), 0.975*altezza, radius/2+((dimx-radius)/2),
		radius/2+((dimx-radius)/2), 0.975*altezza, radius/2+((dimx-radius)/2),
		radius/2+((dimx-radius)/2), 0.975*altezza, -radius/2-((dimx-radius)/2),
		-radius/2-((dimx-radius)/2), 0.975*altezza, -radius/2-((dimx-radius)/2)
	);
	
	glEnd();	

}

static void drawBraccioLampadarioSopra() {

	glPushMatrix();
		drawSemiTorus(5.0,0.8,20,20);
		glTranslatef(8.5, 0.0, 0.0);
	
	glPushMatrix();
		glRotatef(180, 1.0, 0.0, 0.0);
		drawSemiTorus(3.5,0.8,20,20);
	glPopMatrix();

		glTranslatef(3.5, 0.0, 0.0);
		drawCilindroChiuso(2.0, 0.5, 20);
		drawCandela(0.8, 4.0, 30);

	glPopMatrix();
	
}



/* Lampadario con dimensione fissa, si estende secondo queste dimensioni:
		-  44x(ovvero 4 metri e 4decimetri)
		-  10y (più lunghezza della corda che può essere scelta)
		-  44z(ovvero 4 metri e 4decimetri)
   Il lampadario è centrato nell'origine
*/	
void drawLampadario() {

	int i;
		
	
	drawCilindroChiuso(5.0, 1.0, 20);

	glPushMatrix();
	glTranslatef(0.0, 1.0, 0.0);
	
	glBegin(GL_QUADS);
	poliTex("filo che tiene il lampadario",
		-0.5,50.0,0.5,
		0.5,50.0,0.5,
		0.5,50.0,-0.5,
		-0.5,50.0,-0.5,
		-0.5,0.0,0.5,
		0.5,0.0,0.5,
		0.5,0.0,-0.5,
		-0.5,0.0,-0.5
	);
	glEnd();

	drawCilindroChiuso(2.5, 0.5, 20);
	glPopMatrix();

	for(i=0;i<8;i++) {
		glPushMatrix();
		glRotatef((360/8)*i, 0.0, 1.0, 0.0);
		glTranslatef(8.8, 1.0, 0.0);
		drawBraccioLampadarioSopra();
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(0.0, -3.5, 0.0);
	drawSphere(5.0, 20, 20);
	glPopMatrix();


}


/* L'acquasantiera è centrata nell'origine e ha raggio radius e si estende sui tre assi così:
	-  da -1.04*radius a 1.04*radius sull'asse x
	-  da -1.04*radius a 0.04*radius sull'asse y
	-  da -1.04*radius a 0.04*radius sull'asse z
*/
void drawAcquasantiera(float radius) {

	setTexture(TEXTURE_MARMO);	
	glPushMatrix();

		glScalef(1.0,-1.0,1.0);
		drawSphereQuarterOutside(radius,32,32);
		drawSphereQuarterInside(radius*0.96,32,32);

	glPopMatrix();

	setTexture(TEXTURE_GOLD);
	glPushMatrix();

		glRotatef(90.0,1.0,0.0,0.0);
		drawSemiTorus(radius*0.98,radius*0.04,32,32);

	glPopMatrix();

}

void drawSchieraPanchine(int n) {
	
	float lunghTot = 215.0;
	int i;

	if( lunghTot/n >= 7.5 ) 
	{
	
		glPushMatrix();
		glTranslatef(0.0, 0.0, 7.5);
		glRotatef(180.0, 0.0, 1.0, 0.0);
		glTranslatef(-40.0, 0.0, 20.0);		
		for(i=0; i<n; i++) {
			glPushMatrix();
			glTranslatef(0.0, 0.0, i*12.0);
			drawPanchina();
			glPopMatrix();			
		}
		
		glPopMatrix();
	}
}

static void metti1CeriSulTavolo(float larghezza, float altezza, float profondita) {

		glPushMatrix();
		glTranslatef(0.0,altezza,0.0+profondita/2);
		drawCero(0.7, 1.5, 30);
		glPopMatrix();

}

static void metti2CeriSulTavolo(float larghezza, float altezza, float profondita) {

		glPushMatrix();
		glTranslatef(((-1*larghezza)/3)*0.8,altezza,profondita*0.6);
		drawCero(0.7, 1.5, 30);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(((1*larghezza)/3)*0.8,altezza,0.0+profondita*0.2);
		drawCero(0.7, 1.5, 30);
		glPopMatrix();

}

static void metti3CeriSulTavolo(float larghezza, float altezza, float profondita) {

		glPushMatrix();
		glTranslatef(((-1*larghezza)/3)*0.8,altezza,profondita*0.6);
		drawCero(0.7, 1.5, 30);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(((1*larghezza)/3)*0.8,altezza,profondita*0.6);
		drawCero(0.7, 1.5, 30);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0,altezza,0.0+profondita*0.2);
		drawCero(0.7, 1.5, 30);
		glPopMatrix();

}


/* Il tavolo dei ceri poggia ad altezza 0.0 sull'asse y ma è centrato nell'origine.
	Si estende da -larghezza/2 a + larghezza/2 su x 
	Si estende da -profondita/2 a + profondita/2 su z
	Si estende di altezza su y
*/
void drawTavoloCeri(float larghezza, float altezza, float profondita, int ceri) {	

	setTexture(TEXTURE_LEGNO_MASSICCIO);	

	glBegin(GL_QUADS);
		
		poliTex("base del tavolo",
			-larghezza/2, 0.05*altezza, profondita/2,
			larghezza/2, 0.05*altezza, profondita/2,		
			larghezza/2, 0.05*altezza, -profondita/2,		
			-larghezza/2, 0.05*altezza, -profondita/2,		
			-larghezza/2, 0.0, profondita/2,
			larghezza/2, 0.0, profondita/2,		
			larghezza/2, 0.0, -profondita/2,		
			-larghezza/2, 0.0, -profondita/2
		);

		poliTex("schiena del tavolo",
			-larghezza/2, 0.95*altezza, -profondita/2+0.2*profondita,
			larghezza/2, 0.95*altezza, -profondita/2+0.2*profondita,		
			larghezza/2, 0.95*altezza, -profondita/2,		
			-larghezza/2, 0.95*altezza, -profondita/2,		
			-larghezza/2, 0.05*altezza, -profondita/2+0.2*profondita,
			larghezza/2, 0.05*altezza, -profondita/2+0.2*profondita,		
			larghezza/2, 0.05*altezza, -profondita/2,		
			-larghezza/2, 0.05*altezza, -profondita/2
		);

		poliTex("testa del tavolo",
			-larghezza/2, altezza, profondita,
			larghezza/2, altezza, profondita,		
			larghezza/2, altezza, -profondita/2,		
			-larghezza/2, altezza, -profondita/2,		
			-larghezza/2, 0.95*altezza, profondita,
			larghezza/2, 0.95*altezza, profondita,		
			larghezza/2, 0.95*altezza, -profondita/2,		
			-larghezza/2, 0.95*altezza, -profondita/2
		);

	glEnd();

	switch (ceri)
	{
		case 3:	metti3CeriSulTavolo(larghezza, altezza, profondita);break;

		case 2:	metti2CeriSulTavolo(larghezza, altezza, profondita);break;

		case 1:	metti1CeriSulTavolo(larghezza, altezza, profondita);break;
	}
}










