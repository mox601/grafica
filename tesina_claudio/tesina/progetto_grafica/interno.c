#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     
#include <GL/glut.h>  
#include "utils.h"
#include "oggetti.h"
#include "texture.h"
#include "light.h"

#ifndef M_PI
#define M_PI 3.14159265
#endif

/* prototipi */

void drawStrutturaInterna();
void drawColonnato(float length, float height, float width, int columns, float columnHeight, float columnWidth);
void drawSoffittoCassettonato(int cassettix, int cassettiz, float spessorey, int texturesoffitto);
void drawPavimento();
void drawPavimentoConRiflesso();

/*____________________________________________________________*/

void drawPavimento(){

	setTextureScaling(30);
	setTexture(TEXTURE_MARMO_FEASIBLE);

	setTransparentColor(1.0,1.0,1.0);

	glBegin(GL_QUADS);

	//colonnasx
		poliTex(
			"pavimentoquadrato",
			5,0.1,-3,
			55,0.1,-3,
			55,0.1,-60,
			5,0.1,-60,
			
			5,0.0,-3,
			55,0.0,-3,
			55,0.0,-60,
			5,0.0,-60);

		poliTex(
			"pavimentoquadrato",
			5,0.1,-60,
			55,0.1,-60,
			55,0.1,-120,
			5,0.1,-120,
			
			5,0.0,-60,
			55,0.0,-60,
			55,0.0,-120,
			5,0.0,-120);

		poliTex(
			"pavimentoquadrato",
			5,0.1,-120,
			55,0.1,-120,
			55,0.1,-180,
			5,0.1,-180,
			
			5,0.0,-120,
			55,0.0,-120,
			55,0.0,-180,
			5,0.0,-180);

		poliTex(
			"pavimentoquadrato",
			5,0.1,-180,
			55,0.1,-180,
			55,0.1,-240,
			5,0.1,-240,
			
			5,0.0,-180,
			55,0.0,-180,
			55,0.0,-240,
			5,0.0,-240);

		poliTex(
			"pavimentoquadrato",
			5,0.1,-240,
			55,0.1,-240,
			55,0.1,-300,
			5,0.1,-300,
			
			5,0.0,-240,
			55,0.0,-240,
			55,0.0,-300,
			5,0.0,-300);

	//colonnadx
		poliTex(
			"pavimentoquadrato",
			123,0.1,-3,
			173,0.1,-3,
			173,0.1,-60,
			123,0.1,-60,
			
			123,0.0,-3,
			173,0.0,-3,
			173,0.0,-60,
			123,0.0,-60);

		poliTex(
			"pavimentoquadrato",
			123,0.1,-60,
			173,0.1,-60,
			173,0.1,-120,
			123,0.1,-120,
			
			123,0.0,-60,
			173,0.0,-60,
			173,0.0,-120,
			123,0.0,-120);

		poliTex(
			"pavimentoquadrato",
			123,0.1,-120,
			173,0.1,-120,
			173,0.1,-180,
			123,0.1,-180,
			
			123,0.0,-120,
			173,0.0,-120,
			173,0.0,-180,
			123,0.0,-180);

		poliTex(
			"pavimentoquadrato",
			123,0.1,-180,
			173,0.1,-180,
			173,0.1,-240,
			123,0.1,-240,
			
			123,0.0,-180,
			173,0.0,-180,
			173,0.0,-240,
			123,0.0,-240);

		poliTex(
			"pavimentoquadrato",
			123,0.1,-240,
			173,0.1,-240,
			173,0.1,-300,
			123,0.1,-300,
			
			123,0.0,-240,
			173,0.0,-240,
			173,0.0,-300,
			123,0.0,-300);

		//colonna centrale
		poliTex(
			"pavimentoquadrato",
			55,0.1,-3,
			123,0.1,-3,
			123,0.1,-60,
			55,0.1,-60,
			
			55,0.0,-3,
			123,0.0,-3,
			123,0.0,-60,
			55,0.0,-60);

		poliTex(
			"pavimentoquadrato",
			55,0.1,-60,
			123,0.1,-60,
			123,0.1,-120,
			55,0.1,-120,
			
			55,0.0,-60,
			123,0.0,-60,
			123,0.0,-120,
			55,0.0,-120);

		poliTex(
			"pavimentoquadrato",
			55,0.1,-120,
			123,0.1,-120,
			123,0.1,-180,
			55,0.1,-180,
			
			55,0.0,-120,
			123,0.0,-120,
			123,0.0,-180,
			55,0.0,-180);

		poliTex(
			"pavimentoquadrato",
			55,0.1,-180,
			123,0.1,-180,
			123,0.1,-240,
			55,0.1,-240,
			
			55,0.0,-180,
			123,0.0,-180,
			123,0.0,-240,
			55,0.0,-240);

	
		poliTex(
			"pavimentoquadrato",
			55,0.1,-240,
			123,0.1,-240,
			123,0.1,-300,
			55,0.1,-300,
			
			55,0.0,-240,
			123,0.0,-240,
			123,0.0,-300,
			55,0.0,-300);

	glEnd();

	float step = 32;
	float raggioalcova = 15;
	float raggioabside = 48;

	glPushMatrix();

		glTranslatef(20,0.1,-300);
		
		glPushMatrix();
		
			glRotatef(180,0,1,0);
			drawSemisfera(raggioalcova,step);

		glPopMatrix();

		glPushMatrix();

			glTranslatef(69,0,0);
			glRotatef(180,0,1,0);
			drawSemisfera(raggioabside,step);
		glPopMatrix();


		glPushMatrix();

			glTranslatef(69+69,0,0);
			glRotatef(180,0,1,0);
			drawSemisfera(raggioalcova,step);
		glPopMatrix();

	glPopMatrix();	

	resetMaterial();

}

/*----------------------------------------------------------------------------------------------*/

void drawSoffittoCassettonato(int cassettix, int cassettiz, float spessorey, int texturesoffitto){

	/*le cose fisse sono 5: il soffitto piatto a y=170; il rapporto tra separatore e cassetto (1/3);
		le dimensioni del soffitto [X,Z] = (100*297); il contorno della griglia (che dipende dallo spessorey
		passato come paramentro); il delta di differenza tra file parallele a z e quelle parallele a x
	*/

	float material_emission[4]={0.1,0.1,0.1,1.0};
	float dimy = 170 ;
	float dimx = 96 ;
	float dimz = 297 ;
	float delta = 0.5;
	int i;
	float sizebordox,sizebordoz,sizex,sizez; 

	//disegno la parte di griglia parallela a x

	sizebordoz = dimz/((4*cassettiz) + 1);
	sizez = 4*sizebordoz;

	//printf("dimz = %f \n sizebordoz= %f \n sizez = %f \n\n ",dimz,sizebordoz,sizez);
	setTexture(TEXTURE_LEGNO_MASSICCIO);
	
	for(i=0;i<=cassettiz;i++){

		glPushMatrix();		

			glTranslatef(0,0,i*sizez);	

			glBegin(GL_QUADS);
	
			poliTex(
				"paralellazxn",
				0,dimy,sizebordoz,
				0,dimy-delta-spessorey,sizebordoz,
				dimx,dimy-delta-spessorey,sizebordoz,
				dimx,dimy,sizebordoz,
		
				0,dimy,0,
				0,dimy-delta-spessorey,0,
				dimx,dimy-delta-spessorey,0,
				dimx,dimy,0			
		
			);	
		
			glEnd();
	
		glPopMatrix();

	}

	//disegno la parte di griglia parallela a z

	sizebordox = dimx/( (4*cassettix) + 1);
	sizex = 4*sizebordox;

	//printf("dimx = %f \n sizebordox= %f \n sizex = %f \n\n ",dimx,sizebordox,sizex);
	
	for(i=0;i<=cassettix;i++){

		glPushMatrix();		

			glTranslatef(i*sizex,0,0);	

			glBegin(GL_QUADS);
	
			poliTex(
				"paralellazxn",
				0,dimy,dimz,
				0,dimy-delta,dimz,
				sizebordox,dimy-delta,dimz,
				sizebordox,dimy,dimz,
		
				0,dimy,0,
				0,dimy-delta,0,
				sizebordox,dimy-delta,0,
				sizebordox,dimy,0			
		
			);	
		
			glEnd();
	
		glPopMatrix();

	}

	setTexture(texturesoffitto);
		
	//glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,material_emission);	

	glPushMatrix();

	glTranslatef(sizebordox,0,sizebordoz);

	int j;

	glNormal3f(0.0,-1.0,0.0);

	for(i=0;i<cassettiz;i++){

		glPushMatrix();

		glTranslatef(0,0,i*sizez);

		for(j=0;j<cassettix;j++){

			glPushMatrix();
	
				glTranslatef(j*sizex,0,0);
	
				glBegin(GL_QUADS);

					glTexCoord2f(0,0);	glVertex3f(0,dimy,0);
					glTexCoord2f(1,0);	glVertex3f(3*sizebordox,dimy,0);	
					glTexCoord2f(1,1);	glVertex3f(3*sizebordox,dimy,3*sizebordoz);
					glTexCoord2f(0,1);	glVertex3f(0,dimy,3*sizebordoz);
				
			
				glEnd();

			glPopMatrix();

		}
		
		glPopMatrix();

	}

	glPopMatrix();

	resetMaterial();
}



/*------------------------------------------------------------*/

void drawStrutturaInterna(){

	setTexture(TEXTURE_MARMO_BIANCO);

	glPushMatrix();
		
		glTranslatef(89.0,0.0,-300.0);		

		glRotatef(180.0,0.0,1.0,0.0);
		//abside centrale
		drawSemiCilindroInside(48.0,100.0,36);
		
		glPushMatrix();
			glTranslatef(0,100,0);
			drawSphereQuarterInside(48,32,32);
		glPopMatrix();

	glPopMatrix();

	glPushMatrix();

		glTranslatef(20,0.0,-300.0);		

		glPushMatrix();

			glRotatef(180.0,0.0,1.0,0.0);
			//alcova sx
			drawSemiCilindroInside(15.0,40.0,32);
	
			//disegno il quarto di sfera sopra
			glPushMatrix();
				glTranslatef(0,40,0);
				drawSphereQuarterInside(15,32,32);
			glPopMatrix();
	
		glPopMatrix();

		glPushMatrix();

			glTranslatef(138.0,0,0);

			glRotatef(180.0,0.0,1.0,0.0);
			//alcova dx
			drawSemiCilindroInside(15.0,40.0,32);

			//disegno il quarto di sfera sopra
			glPushMatrix();
				glTranslatef(0,40,0);
				drawSphereQuarterInside(15,32,32);
			glPopMatrix();
	
		glPopMatrix();


	glPopMatrix();


	//le 3 superfici arcate

glPushMatrix();

	//centrale
	glPushMatrix();
		glTranslatef(41,100,-300);
		drawSuperficieArcata(48,16);
	glPopMatrix();

	//sx
	glTranslatef(5,40,-300);
	
	drawSuperficieArcata(15,16);
	
	//dx
	glPushMatrix();

		glTranslatef(138,0,0);
	
		drawSuperficieArcata(15,16);
	
	glPopMatrix();

glPopMatrix();


//completo il fondo nella navata centrale
glPushMatrix();

	glBegin(GL_QUADS);

	verTex(
		"connessioneabsidetriangolone",
		41,148,-300,
		137,148,-300,
		137,170,-300,
		41,170,-300
		);

	verTex(
		"triangolosx",
		5,55,-300,
		35,55,-300,
		35,100,-300,
		35,100,-300

	);

	verTex(
		"triangolodx",
		143,55,-300,
		173,55,-300,
		143,100,-300,
		143,100,-300

	);
	
	//facciainternasx
	verTex(
		"internabassasx",
		12.5,0,-3,
		5,0,-3,
		5,30,-3,
		12.5,30,-3

	);

	verTex(
		"internabassadx",
		35,0,-3,
		27.5,0,-3,
		27.5,30,-3,
		35,30,-3

	);

	verTex(
		"triangolettosx",
		5,30,-3,
		5,55,-3,
		35,100,-3,
		35,30,-3

		);

	//facciainternadx
	verTex(
		"internabassadx",
		173,0,-3,
		165.5,0,-3,
		165.5,30,-3,
		173,30,-3

	);

	verTex(
		"internabassasx",
		150.5,0,-3,
		143,0,-3,
		143,30,-3,
		150.5,30,-3

	);

	verTex(
		"triangolettodx",
		143,30,-3,
		143,100,-3,
		173,55,-3,
		173,30,-3

		);

	//facciainternacentraledx1.1
	verTex(
		"internacentraledx1.1",
		139,0,-3,
		109,0,-3,
		109,50,-3,
		139,50,-3

	);


	//facciainternacentraledx1.2
	verTex(
		"internacentraledx1.2",
		139,50,-3,
		109,50,-3,
		109,120,-3,
		139,120,-3

	);
	
	//facciainternacentraledx1.3
	verTex(
		"internacentraledx1.3",
		139,120,-3,
		109,120,-3,
		109,170,-3,
		139,170,-3

	);

	//facciainternacentralesx1.1
	verTex(
		"internacentralesx1.1",
		69,0,-3,
		39,0,-3,
		39,50,-3,
		69,50,-3

	);

	//facciainternacentralesx1.2
	verTex(
		"internacentralesx1.1",
		69,50,-3,
		39,50,-3,
		39,120,-3,
		69,120,-3

	);

	//facciainternacentralesx1.3
	verTex(
		"internacentralesx1.3",
		69,120,-3,
		39,120,-3,
		39,170,-3,
		69,170,-3

	);

	verTex(
		"riempimento",
		69,50,-3,
		69,120,-3,
		109,120,-3,
		109,50,-3

		);

	verTex(
		"soprabucointerno",
		69,160,-3,
		69,170,-3,
		109,170,-3,
		109,160,-3

		);


	//dxbassa interna
		verTex(
			"dxbassainterna",
			173,0,0,
			173,55,0,
			173,55,-60,
			173,0,-60
			
			
		);

	//dxbassa interna1
		verTex(
			"dxbassainterna1",
			173,0,-60,
			173,55,-60,
			173,55,-120,
			173,0,-120
			
			
		);

	//dxbassa interna2
		verTex(
			"dxbassainterna2",
			173,0,-120,
			173,55,-120,
			173,55,-180,
			173,0,-180
			
			
		);

	//dxbassa interna3
		verTex(
			"dxbassainterna3",
			173,0,-180,
			173,55,-180,
			173,55,-240,
			173,0,-240
			
			
		);

	//dxbassa interna4
		verTex(
			"dxbassainterna4",
			173,0,-240,
			173,55,-240,
			173,55,-300,
			173,0,-300
			
			
		);


	//sxbassa interna
		verTex(
			"sxbassainterna",
			5,0,0,
			5,0,-60,
			5,55,-60,
			5,55,0
				
		);

	//sxbassa interna1
		verTex(
			"sxbassainterna1",
			5,0,-60,
			5,0,-120,
			5,55,-120,
			5,55,-60
				
		);

	//sxbassa interna2
		verTex(
			"sxbassainterna2",
			5,0,-120,
			5,0,-180,
			5,55,-180,
			5,55,-120
				
		);


	//sxbassa interna3
		verTex(
			"sxbassainterna3",
			5,0,-180,
			5,0,-240,
			5,55,-240,
			5,55,-180
				
		);

	//sxbassa interna4
		verTex(
			"sxbassainterna4",
			5,0,-240,
			5,0,-300,
			5,55,-300,
			5,55,-240
				
		);


	//tetto interno sx

		verTex(
			"tettosxinterno",
			5,55,0,
			5,55,-320,
			35,90,-320,
			35,90,0
				
		);

	//tetto interno dx

		verTex(
			"tettodxinterno",
			173,55,-320,
			173,55,0,
			143,90,0,
			143,90,-320
				
		);


	//pareta alta sx

		verTex(
			"pareteinternaaltasx",
			41,90,0,
			41,90,-80,
			41,170,-80,
			41,170,0
				
		);

		verTex(
			"pareteinternaaltasx",
			41,90,-80,
			41,90,-160,
			41,170,-160,
			41,170,-80
				
		);

		verTex(
			"pareteinternaaltasx",
			41,90,-160,
			41,90,-240,
			41,170,-240,
			41,170,-160
				
		);

		verTex(
			"pareteinternaaltasx",
			41,90,-240,
			41,90,-320,
			41,170,-320,
			41,170,-240
				
		);

	//parete alta dx

		verTex(
			"paretealtainternadx",
			137,90,-80,
			137,90,0,
			137,170,0,
			137,170,-80
				
		);

		verTex(
			"paretealtainternadx",
			137,90,-160,
			137,90,-80,
			137,170,-80,
			137,170,-160
				
		);

		verTex(
			"paretealtainternadx",
			137,90,-240,
			137,90,-160,
			137,170,-160,
			137,170,-240
				
		);

		verTex(
			"paretealtainternadx",
			137,90,-320,
			137,90,-240,
			137,170,-240,
			137,170,-320
				
		);

	//retro separazione sx

		verTex(
			"retroseparazionesx",
			35,0,-3,	
			35,95,-3,
			41,95,-3,
			41,0,-3);

	//retro separazione dx

		verTex(
			"retroseparazionedx",
			137,0,-3,
			137,95,-3,
			143,95,-3,
			143,0,-3);

	//retro separazione sx fondo

		verTex(
			"retroseparazionefondosx",
			35,95,-300,			
			35,0,-300,	
			41,0,-300,
			41,95,-300);

	//retro separazione dx fondo

		verTex(
			"retroseparazionefondodx",
			137,95,-300,
			137,0,-300,
			143,0,-300,
			143,95,-300);


	glEnd();

	glPushMatrix();

			glTranslatef(109.0,120.0,-3);

			glRotatef(180,0,1,0);
			drawQuadratoBucato(40.0,8);

	glPopMatrix();		

	glPushMatrix();

			glTranslatef(89,140,-3);
			glRotatef(90,1,0,0);
			drawCilindroInside(20,3,32);

	glPopMatrix();

glPopMatrix();

}


void drawColonnato(float length, float height, float width, int columns, float columnHeight, float columnWidth) {

	//bisogna disegnare un arco in più rispetto al numero di colonne: divido lo spazio rimasto per il numero di archi
	//e il risultato ottenuto per 2, ottenendo il raggio di ogni arco da disegnare
	float raggioArco = ( (length - width*columns) / (columns+1) ) / 2.0;
	//printf("raggioArco = \n( (%f - %f*%d) / (%d+1) ) / 2.0 = %f\n",length,width,columns,columns,raggioArco);

	int i;
	
	//se le dimensioni passate come parametro sono sbagliate, interrompi!
	if(columnHeight+raggioArco > height)
		return;

	//parti di muro che sovrastano il colonnato
	glBegin(GL_QUADS);

		verTex(
			"parte superiore muro davanti",
			0.0, columnHeight+raggioArco, 0.0,
			length, columnHeight+raggioArco, 0.0,
			length, height, 0.0,
			0.0, height, 0.0
		);

		verTex(
			"parte superiore muro dietro",
			length, columnHeight+raggioArco, -width,
			0.0, columnHeight+raggioArco, -width,
			0.0, height, -width,
			length, height, -width
		);

	glEnd();

	//parte sotto (colonne, archi, raccordi)
	glPushMatrix();

		for(i=0; i<columns; i++) {

			glPushMatrix();

				glTranslatef(0.0,columnHeight,0.0);

				drawArco(raggioArco,width,16);

				glTranslatef(2*raggioArco,0.0,0.0);

				glBegin(GL_QUADS);
					verTex(
						"raccordo muro davanti",
						0.0, 0.0, 0.0,
						width, 0.0, 0.0,
						width, raggioArco, 0.0,
						0.0, raggioArco, 0.0
					);
					verTex(
						"raccordo muro dietro",
						width, 0.0, -width,
						0.0, 0.0, -width,
						0.0, raggioArco, -width,
						width, raggioArco, -width
					);
				glEnd();

			glPopMatrix();

			glTranslatef(2*raggioArco+width/2,0.0,-width/2);

			drawColonna(columnWidth, columnHeight, (columnWidth * 0.8), 0, 0);

			glTranslatef(width/2,0.0,width/2);

		}

		glPushMatrix();
			glTranslatef(0.0,columnHeight,0.0);
			drawArco(raggioArco,width,16);
		glPopMatrix();

	glPopMatrix();
}

void costruisciAltareConCroce() {
	
		setTexture(TEXTURE_MARMO);
		setTextureScaling(30);
		glPushMatrix();
		glTranslatef(59.0, 0.0, -270.0);
		drawBaseAltare(60.0, 4.0, 50.0);
		glPopMatrix();

		setTexture(TEXTURE_LEGNO_MASSICCIO);
		setTextureScaling(40);

		glPushMatrix();
		glTranslatef(85.0, 8.0, -285.0);
		drawAltare();
		glPopMatrix();

		setTexture(TEXTURE_LEGNO_MASSICCIO);
		setTextureScaling(30);

		glPushMatrix();
		glTranslatef(86.0, 0.0, -330.0);
		drawCroce(8.0, 80.0);
		glPopMatrix();

		setTexture(TEXTURE_TOVAGLIA);
		setTextureScaling(30);

		glPushMatrix();
		glTranslatef(85.0, 8.0, -285.0);
		drawTovaglia();	
		glPopMatrix();	

		setTexture(TEXTURE_GOLD);			
		setTextureScaling(30);

		glPushMatrix();
		glTranslatef(80.0, 20.1, -285.0);
		drawCandelabro(10.0, 0.2);	
		glPopMatrix();	

		glPushMatrix();
		glTranslatef(100.0, 20.1, -285.0);
		drawCandelabro(10.0, 0.2);	
		glPopMatrix();	


}

void costruisciPanchinato() {

		setTexture(TEXTURE_LEGNO_MASSICCIO);	
		setTextureScaling(30);
		
		glPushMatrix();
		glTranslatef(41.5, 0.0, -55.0);
		drawSchieraPanchine(15);
		glPopMatrix();	
	
		glPushMatrix();
		glTranslatef(81.5, 0.0, -55.0);
		drawSchieraPanchine(15);		
		glPopMatrix();

}

void costruisciQuadriConCeri() {
	
	glPushMatrix();
	glTranslatef(7.5, 0.0, -64.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawTavoloCeri(8.0,10.0,5.0,3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.5, 15, -39.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawQuadro(50.0, 30.0, 1.5, TEXTURE_CENA);	
	glPopMatrix();

	//	

	glPushMatrix();
	glTranslatef(7.5, 0.0, -195.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawTavoloCeri(8.0,10.0,5.0,2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.5, 15, -170.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawQuadro(50.0, 30.0, 1.5, TEXTURE_CREAZIONE);	
	glPopMatrix();

	//

			
	glPushMatrix();
	glTranslatef(170.5, 0.0, -64.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	drawTavoloCeri(8.0,10.0,5.0,2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(172.5, 15, -91);
	glRotatef(-90, 0.0, 1.0, 0.0);
	drawQuadro(50.0, 30.0, 1.5, TEXTURE_CARAVAGGIO);	
	glPopMatrix();

	//

	glPushMatrix();
	glTranslatef(170.5, 0.0, -195.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	drawTavoloCeri(8.0,10.0,5.0,1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(172.5, 15, -222);
	glRotatef(-90, 0.0, 1.0, 0.0);
	drawQuadro(50.0, 30.0, 1.5, TEXTURE_VENERE);	
	glPopMatrix();

}

void costruisciLampadario() {

	setTexture(TEXTURE_GOLD);
	setTextureScaling(30);

	glPushMatrix();
		glTranslatef(89.0, 120.0, -175.0);
		drawLampadario();
	glPopMatrix();	

}

void costruisciAcquesantiere() {

	setTexture(TEXTURE_MARMO);
	setTextureScaling(30);
	glPushMatrix();
		glTranslatef(49.0, 12.0, -3.5);
		glRotatef(180, 0.0, 1.0, 0.0);
		drawAcquasantiera(5.0);
	glPopMatrix();	

	glPushMatrix();
		glTranslatef(82.5+49.0, 12.0, -3.5);
		glRotatef(180, 0.0, 1.0, 0.0);
		drawAcquasantiera(5.0);
	glPopMatrix();	
	
}

void costruisciSoffittoCassettonato() {

	setTexture(TEXTURE_LEGNO_MASSICCIO);
	setTextureScaling(30);
	
	glPushMatrix();
		glTranslatef(41,0,-300);
		drawSoffittoCassettonato(5,10,4,TEXTURE_CASSETTONE);
	glPopMatrix();

}

void costruisciColonnato() {

	setTexture(TEXTURE_MARMO_BIANCO);

	glPushMatrix();
		glTranslatef(41,0,0);			
		glRotatef(90.0,0.0,1.0,0.0);
	drawColonnato(300, 90, 6, 6, 70, 10);
	glPopMatrix();
		
	glPushMatrix();
		glTranslatef(143,0,0);
		glRotatef(90.0,0.0,1.0,0.0);
		drawColonnato(300, 90, 6, 6, 70, 10);
	glPopMatrix();

}

void drawInterni() {

	glEnable(GL_TEXTURE_2D);

	setTextureScaling(30);

	drawStrutturaInterna();
	
	costruisciColonnato();

	costruisciAltareConCroce();
	costruisciPanchinato();
	costruisciQuadriConCeri();
	costruisciSoffittoCassettonato();
	costruisciLampadario();
	costruisciAcquesantiere();

}

void drawPavimentoConRiflesso() {

	glDisable(GL_CULL_FACE);
	glPushMatrix();
		//glScalef(1.0,-1.0,1.0);
		glTranslatef(178.0,0.0,0.0);
		glRotatef(180.0,0.0,0.0,1.0);
		drawInterni();
	glPopMatrix();

	//glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		
	drawPavimento();


	//glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);

}


void drawFiammelleInterne() {

	int i;


/************************************************** LAMPADARIO ***********************************************/

			glPushMatrix();
				glTranslatef(89.0, 125.1, -175.0);		
		
			for (i=0;i<8;i++)
			{
			glPushMatrix();				
				glRotatef((360.0/8.0)*i, 0.0, 1.0, 0.0);
				glTranslatef(0.0, 0.0, 20.8);
				drawFiammellaLampadario();
			glPopMatrix();

			}
			glPopMatrix();


/************************************************** CANDELABRO 1 ***********************************************/

		glPushMatrix();
			
			glTranslatef(77.0, 30.1, -285.0);
			drawFiammella();

		glPopMatrix();

		glPushMatrix();
			
			glTranslatef(80.0, 30.1, -285.0);
			drawFiammella();

		glPopMatrix();

		glPushMatrix();
			
			glTranslatef(83.0, 30.1, -285.0);
			drawFiammella();

		glPopMatrix();


/************************************************** CANDELABRO 2 ***********************************************/

		glPushMatrix();
			
			glTranslatef(97.0, 30.1, -285.0);
			drawFiammella();

		glPopMatrix();

		glPushMatrix();
			
			glTranslatef(100.0, 30.1, -285.0);
			drawFiammella();

		glPopMatrix();

		glPushMatrix();
			
			glTranslatef(103.0, 30.1, -285.0);
			drawFiammella();

		glPopMatrix();


/************************************************** CERI 1 ***********************************************/

		glPushMatrix();
			
			glTranslatef(8.5, 11.0, -64.0);
			drawFiammella();

		glPopMatrix();

		glPushMatrix();
			
			glTranslatef(10.53, 11.0, -66.1525);
			drawFiammella();

		glPopMatrix();

		glPushMatrix();
			
			glTranslatef(10.52, 11.0, -61.855);
			drawFiammella();

		glPopMatrix();


/************************************************** CERI 2 ***********************************************/
	
		glPushMatrix();
			
			glTranslatef(8.53, 11.0, -197.145);
			drawFiammella();

		glPopMatrix();

		glPushMatrix();
			
			glTranslatef(10.52, 11.0, -192.855);
			drawFiammella();

		glPopMatrix();

/************************************************** CERI 3 ***********************************************/


		glPushMatrix();
			
			glTranslatef(167.5, 11.0, -66.1525);
			drawFiammella();

		glPopMatrix();

		glPushMatrix();
			
			glTranslatef(169.5, 11.0, -61.855);
			drawFiammella();

		glPopMatrix();

/************************************************** CERI 4 ***********************************************/
	
		glPushMatrix();
			
			glTranslatef(168, 11.0, -195.0);
			drawFiammella();

		glPopMatrix();

		resetMaterial();


}



















