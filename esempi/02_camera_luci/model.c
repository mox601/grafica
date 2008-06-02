#include "camera.h"
#include "model.h"

/*continuare con le funzioni del modello: parete curva, etc */



//non contando la porta
GLfloat lunghezza = 30.0f;
GLfloat altezza = 7.5f;
GLfloat segno_inclinazione = 1.0f;


/* riceve i punti con cui costruire un triangolo */
/* riceve i punti in senso ANTIORARIO */
void drawTriangle(Point3d* point1, Point3d* point2,Point3d* point3, GLfloat dettaglio) {

//	printf("x punto: %f, %f, %f\n", point1->x, point1->y, point1->z);
//	printf("y punto: %f, %f, %f\n", point2->x, point2->y, point2->z);
//	printf("z punto: %f, %f, %f\n", point3->x, point3->y, point3->z);


	/* devo impostare la normale del triangolo che sto costruendo: */

	Vector3d destNormal; 
	Vector3d diff1_2;
	Vector3d diff2_3;
	vector_diff(&diff1_2, point1, point2); 
	vector_diff(&diff2_3, point2, point3); 
	/* giusto? */
	vector_cross_product(&destNormal, &diff2_3, &diff1_2); 
	/* errato? */
//	vector_cross_product(&destNormal, &diff1_2, &diff2_3); 

	/* verifica!! */

	glNormal3f(destNormal.x,destNormal.y,destNormal.z);

//	printf("normale: %f %f %f\n", destNormal.x,destNormal.y,destNormal.z); 



/* se la minore distanza tra due punti del triangolo è minore di dettaglio: */
/* disegno il triangolo */

	GLfloat distanza_minima = minDistance(point1, point2, point3); 

//	printf("distanza minima = %f\n", distanza_minima);
//	glColor3f(0,1,0);
	if (distanza_minima <= dettaglio) {
		/* disegno triangoli oppure line loops */
		glBegin(draw_wireframe?GL_LINE_LOOP:GL_TRIANGLES);
			glVertex3f(point1->x, point1->y, point1->z);
			glVertex3f(point2->x, point2->y, point2->z);
			glVertex3f(point3->x, point3->y, point3->z);
		glEnd();	
	} else {
		/* richiamo la funzione per costruire 4 triangoli più 
		piccoli, costruiti usando i punti medi del triangolo grande */


		Point3d point_a;
		midPoint(&point_a, point1, point2);
//		printf("punto_medio_a: %f, %f, %f\n", point_a.x, point_a.y, point_a.z);

		Point3d point_b;
		midPoint(&point_b, point2, point3);
//		printf("punto_medio_b: %f, %f, %f\n", point_b.x, point_b.y, point_b.z);

		Point3d point_c;
		midPoint(&point_c, point3, point1);
//		printf("punto_medio_c: %f, %f, %f\n", point_c.x, point_c.y, point_c.z);


	/* ho i punti medi, disegno 4 triangoli rispettando il senso ANTIORARIO */
	/* il dettaglio resta lo stesso */

		glColor3f(1,0,0);
		/* triangolo A: 1-a-c */
		drawTriangle(point1, &point_a, &point_c, dettaglio);
		glColor3f(0,1,0);
		/* triangolo B: a-b-c */
		drawTriangle(&point_a, &point_b, &point_c, dettaglio);
		glColor3f(0,0,1);
		/* triangolo C: a-2-b */
		drawTriangle(&point_a, point2, &point_b, dettaglio);
		glColor3f(1,1,1);
		/* triangolo D: c-b-3 */
		drawTriangle(&point_c, &point_b, point3, dettaglio);
	}
}



/* ricevo i punti in senso ANTIORARIO  
a partire da in basso a SX, poi alto SX, poi alto DX (spigolo) e basso SX */


void drawWall(Point3d* point1, Point3d* point2, Point3d* point3, Point3d* point4, GLfloat spessore, GLfloat spigolo_laterale, GLfloat incl_frontale, GLfloat dettaglio) {



/* disegna primo quadrilatero (2 triangoli) sui punti in input usando il drawTriangle */
	/* inclinazione frontale del muro, su punto 3 e punto 4 */
	Vector3d traslazione_inclinazione = {incl_frontale, 0.0f, 0.0f}; 

	point_translate(point3,&traslazione_inclinazione);
	point_translate(point4,&traslazione_inclinazione);


	/* utilizzo lo spigolo laterale per il point3 */

	Vector3d traslazione_spigolo = {0.0f, spigolo_laterale, 0.0f}; 
	point_translate(point3,&traslazione_spigolo);

	drawTriangle(point3, point4, point1, dettaglio);


	GLfloat dettaglio_inclinato = dettaglio; 
	if (spigolo_laterale > 0.0f) {
		dettaglio_inclinato = dettaglio_inclinato + 0.05f;
	}

	/* è il triangolo inclinato, devo ridurre il dettaglio se c'é */
	drawTriangle(point1, point2, point3, dettaglio_inclinato);

	/* secondo quadrilatero, a distanza -spessore dal precedente */

	Point3d point1t;
	Point3d point2t; 
	Point3d point3t; 
	Point3d point4t;  

	init_point(point1, &point1t);
	init_point(point2, &point2t);
	init_point(point3, &point3t);
	init_point(point4, &point4t);

	Vector3d traslazione_spessore = {-spessore, 0.0f, 0.0f}; 

	point_translate(&point1t,&traslazione_spessore);
	point_translate(&point2t,&traslazione_spessore);
	point_translate(&point3t,&traslazione_spessore);
	point_translate(&point4t,&traslazione_spessore);


	drawTriangle(&point1t, &point3t, &point2t, dettaglio);
	drawTriangle(&point1t, &point4t, &point3t, dettaglio);


	/* facce laterali, DX e SX, con dettaglio più basso? basta aumentare il parametro */
	drawTriangle(point2, &point2t, &point3t, dettaglio);
	drawTriangle(&point3t, point3, point2, dettaglio);

	drawTriangle(point1, point4, &point4t, dettaglio);
	drawTriangle(point1, &point4t, &point1t, dettaglio);

	/* facce superiore e inferiore */
	
	drawTriangle(point3, &point3t, point4, dettaglio);
	drawTriangle(&point3t, &point4t, point4, dettaglio);
	
	drawTriangle(point2, point1, &point2t, dettaglio);
	drawTriangle(&point1t, &point2t, point1, dettaglio);

	/* collisioni? ho bisogno di sapere le coordinate mondo del muro */

}

/* costruisce un muro prendendo la lunghezza come parametro */
void drawWallSize(GLfloat lunghezza, GLfloat dettaglio, GLfloat inclinazione) {


	GLfloat altezza = lunghezza * 0.30f;
	Point3d  a = {0,0,0};
	Point3d  b = {0,lunghezza,0};
	Point3d  c = {0,lunghezza,altezza};
	Point3d  d = {0,0,altezza};


	GLfloat spessore = lunghezza * 0.02f;
	GLfloat sp_laterale_front = lunghezza * 0.2f;
	GLfloat sp_laterale_back = lunghezza * -0.1f;
	GLfloat incl_frontale = lunghezza * 0.03f * inclinazione;
	/* differenza di altezza tra point3 e point4*/
	GLfloat differenza = 0.3; 

	drawWallOblique(&a, &b, &c, &d, spessore, sp_laterale_front, sp_laterale_back, incl_frontale, dettaglio, differenza);

}


/* drawWall che disegna un muro a partire da una certa altezza, senza partire dalla base */
void drawWallSizeHigh(GLfloat lunghezza, GLfloat altezzaMax, GLfloat inclinazione,  GLfloat spessore, GLfloat dettaglio, GLfloat lambda) {


//	GLfloat altezza = lunghezza * 0.30f;
/* le coordinate dei punti alla base (a, b, a', b')sono calcolati in base alla combinazione convessa pesata tra (a, d) e (b, c) */

	Point3d  a = {0,0,0};
	Point3d  b = {0,lunghezza,0};
	Point3d  c = {0, lunghezza, altezzaMax};
	Point3d  d = {0, 0, altezzaMax};

//	printf("c wallhigh: %f %f %f\n", c.x, c.y, c.z); 
//	printf("d wallhigh: %f %f %f\n", d.x, d.y, d.z); 

	Point3d a1;
	Point3d b1;
	Point3d c1 = {inclinazione, lunghezza, altezzaMax};
	Point3d d1 = {inclinazione, 0, altezzaMax};

	convexCombination(&a1, &a, &d1, lambda);
	convexCombination(&b1, &b, &c1, lambda); 

	GLfloat sp_laterale = 0.0f;

//	drawWall(&a1, &b1, &c, &d, spessore, sp_laterale, inclinazione, dettaglio);
	/* provo ad usare oblique */
//	drawWallOblique(&a1, &b1, &c, &d, spessore, sp_laterale, inclinazione, dettaglio);

}



/* disegna il muro con la faccia superiore inclinata */

void drawWallOblique(Point3d* point1, Point3d* point2, Point3d* point3, Point3d* point4, GLfloat spessore, GLfloat spigolo_laterale_front, GLfloat spigolo_laterale_rear, GLfloat incl_frontale, GLfloat dettaglio, GLfloat differenza) {

/* disegna primo quadrilatero (2 triangoli) sui punti in input usando il drawTriangle */

	
	/* inclinazione frontale del muro, su punto 3 e punto 4 */
	Vector3d traslazione_inclinazione = {incl_frontale, 0.0f, 0.0f}; 

	point_translate(point3,&traslazione_inclinazione);
	point_translate(point4,&traslazione_inclinazione);

	/* modifico il point4 utilizzo la combinazione convessa tra 1_4 */

//	printf("point4 prima: %f %f %f\n", point4->x, point4->y, point4->z);

	/* si decide la differenza di altezza tra point3 e point4 */
	convexCombination(point4, point1, point4, differenza); 

	//printf("point1: %f %f %f\n", point1->x, point1->y, point1->z);
	//printf("point4: %f %f %f\n", point4->x, point4->y, point4->z);
//	printf("point4 dopo: %f %f %f\n", point4->x, point4->y, point4->z);
	
	/* utilizzo lo spigolo laterale_front per il point3 */

	Vector3d traslazione_spigolo_front = {0.0f, spigolo_laterale_front, 0.0f}; 
	point_translate(point3,&traslazione_spigolo_front);

	/* utilizzo lo spigolo laterale_rear per il point4 */

	Vector3d traslazione_spigolo_rear = {0.0f, spigolo_laterale_rear, 0.0f}; 
	point_translate(point4,&traslazione_spigolo_rear);

	drawTriangle(point3, point4, point1, dettaglio);

	GLfloat dettaglio_inclinato = dettaglio; 
	if (spigolo_laterale_front > 0.0f) {
		dettaglio_inclinato = dettaglio_inclinato + 0.05f;
	}

	/* è il triangolo inclinato, devo ridurre il dettaglio se c'é */
	drawTriangle(point1, point2, point3, dettaglio_inclinato);



	/* secondo quadrilatero, a distanza -spessore dal precedente */

	Point3d point1t;
	Point3d point2t; 
	Point3d point3t; 
	Point3d point4t;  

	init_point(point1, &point1t);
	init_point(point2, &point2t);
	init_point(point3, &point3t);
	init_point(point4, &point4t);

	Vector3d traslazione_spessore = {-spessore, 0.0f, 0.0f}; 

	point_translate(&point1t,&traslazione_spessore);
	point_translate(&point2t,&traslazione_spessore);
	point_translate(&point3t,&traslazione_spessore);
	point_translate(&point4t,&traslazione_spessore);


	drawTriangle(&point1t, &point3t, &point2t, dettaglio);
	drawTriangle(&point1t, &point4t, &point3t, dettaglio);


	/* facce laterali, DX e SX, con dettaglio più basso? basta aumentare il parametro */
	drawTriangle(point2, &point2t, &point3t, dettaglio);
	drawTriangle(&point3t, point3, point2, dettaglio);

	drawTriangle(point1, point4, &point4t, dettaglio);
	drawTriangle(point1, &point4t, &point1t, dettaglio);

	/* facce superiore e inferiore */
	
	drawTriangle(point3, &point3t, point4, dettaglio);
	drawTriangle(&point3t, &point4t, point4, dettaglio);
	
	drawTriangle(point2, point1, &point2t, dettaglio);
	drawTriangle(&point1t, &point2t, point1, dettaglio);

	/* collisioni? ho bisogno di sapere le coordinate mondo del muro */



	/* disegno i contorni del muro con delle linee blu */

	/* faccia frontale */

	glColor3f(0.0f, 0.0f, 1.0f);

	glBegin(GL_LINE_LOOP);
		glVertex3f(point1->x, point1->y, point1->z);
		glVertex3f(point2->x, point2->y, point2->z);
		glVertex3f(point3->x, point3->y, point3->z);
		glVertex3f(point4->x, point4->y, point4->z);
	glEnd();

	/* faccia posteriore */	

	glBegin(GL_LINE_LOOP);
		glVertex3f(point1t.x, point1t.y, point1t.z);
		glVertex3f(point2t.x, point2t.y, point2t.z);
		glVertex3f(point3t.x, point3t.y, point3t.z);
		glVertex3f(point4t.x, point4t.y, point4t.z);
	glEnd();

	/* spigoli */

	glBegin(GL_LINES);
		glVertex3f(point1->x, point1->y, point1->z);
		glVertex3f(point1t.x, point1t.y, point1t.z);
		glVertex3f(point2->x, point2->y, point2->z);
		glVertex3f(point2t.x, point2t.y, point2t.z);
		glVertex3f(point3->x, point3->y, point3->z);
		glVertex3f(point3t.x, point3t.y, point3t.z);
		glVertex3f(point4->x, point4->y, point4->z);
		glVertex3f(point4t.x, point4t.y, point4t.z);
	glEnd();




}


void drawFrontWall(GLfloat lunghezza, GLfloat dettaglio, GLfloat segno_inclinazione, GLfloat altezza) {



	GLfloat lunghezza_w1 = lunghezza * 0.5f; 
	GLfloat lunghezza_w2 = lunghezza * 0.5f; // sarà minore
	
	GLfloat altezza_w2 = lunghezza * 0.30f;

	GLfloat larghezza_porta = lunghezza * 0.085f; 

	/* da migliorare la differenza di altezza tra le due parti di muro */

	/* differenza di altezza tra point3 e point4 di w1 */
	GLfloat differenza_w1 = lunghezza * 0.01; 
	/* differenza di altezza tra point3 e point4 di w2 */
	GLfloat differenza_w2 = lunghezza * 0.01; 
	/* l'altezza del muro w1 è funzione dell'altezza di w2 * differenza altezza */
	GLfloat altezza_w1 = altezza_w2 - (altezza_w2 * differenza_w2);

	/* punti per w1 */
	Point3d  a1 = {0,0,0};
	Point3d  b1 = {0,lunghezza_w1,0};
	Point3d  c1 = {0,lunghezza_w1, altezza_w1};
	Point3d  d1 = {0,0,altezza_w1};

	/* punti per w2, altrimenti vengono modificati dalla drawWall */
	Point3d  a2 = {0,0,0};
	Point3d  b2 = {0,lunghezza_w2,0};
	Point3d  c2 = {0,lunghezza_w2, altezza_w2};
	Point3d  d2 = {0,0,altezza_w2};


	GLfloat spessore = lunghezza * 0.02f;
	GLfloat spigolo_laterale_front_w1 = 0.0f;
	GLfloat spigolo_laterale_back_w1 = lunghezza * -0.05f;
	
	GLfloat spigolo_laterale_front_w2 = lunghezza * 0.05f;
	GLfloat spigolo_laterale_back_w2 = 0.0f;


	GLfloat incl_frontale = lunghezza * 0.03f * segno_inclinazione;

	
//	printf("a1: %f, %f, %f\n", a1.x, a1.y, a1.z);


//	printf("b2: %f, %f, %f\n", b2.x, b2.y, b2.z);


/* disegno la facciata in due parti per lasciare spazio alla porta */

	drawWallOblique(&a1, &b1, &c1, &d1, spessore, spigolo_laterale_front_w1, spigolo_laterale_back_w1, incl_frontale, dettaglio, differenza_w1);

	
/* spazio per la porta e la griglia sovrastante */
/* mi sposto per disegnare il wallHigh */
	glPushMatrix();
	glTranslatef(0.0f, lunghezza_w1, 0.0f);

	/* disegno della griglia sovrastante */

	GLfloat altezza_porta = lunghezza * 0.1f; 
	GLfloat altezza_Max = altezza_w1; 
	GLfloat dim_x = larghezza_porta; 
	GLfloat dim_y = altezza_w1 - altezza_porta; 

	drawGriglia(larghezza_porta, altezza_porta, altezza_Max, spessore, 6, incl_frontale);

	/* test per funzione highwall */
	drawWallSizeHigh(larghezza_porta, altezza_w1, incl_frontale, spessore, dettaglio, 0.55f);

	glTranslatef(0.0f, larghezza_porta, 0.0f);

//	printf("c terzomuro : %f %f %f\n", c2.x, c2.y, c2.z);
//	printf("d terzomuro : %f %f %f\n", d2.x, d2.y, d2.z);

	drawWallOblique(&a2, &b2, &c2, &d2, spessore, spigolo_laterale_front_w2, spigolo_laterale_back_w2, incl_frontale, dettaglio, differenza_w2);

	
	glPopMatrix();
	/* ho disegnato il wallHigh */

//	glPushMatrix();


//	drawWallOblique(&a2, &b2, &c2, &d2, spessore, spigolo_laterale_front_w2, spigolo_laterale_back_w2, incl_frontale, dettaglio, differenza_w2);

//	glPopMatrix();

}

void drawGriglia(GLfloat dim_larghezza, GLfloat dim_altezza, GLfloat altezzaMax, GLfloat dim_spessore, int n_strisce, GLfloat inclinazione) {


	/* la griglia parte da punti con altezza dim_altezza, calcolati sulla 
	retta inclinata */
	int i; 
	GLfloat spessore = dim_spessore; 

	GLfloat distanza = dim_larghezza / n_strisce;

	Point3d punto_start_altezza_griglia;
	
	
	glPushMatrix();
	for (i = 0; i < n_strisce; i++) {
		//drawStriscia();
		glTranslatef(0.0f, distanza, 0.0f);
	}
	glPopMatrix();


}


void drawStriscia(Point3d punto_start, Point3d punto_end, GLfloat spessore, GLfloat inclinazione){

	glBegin(draw_wireframe?GL_LINE_LOOP:GL_QUADS);
	glColor3f(0.5f, 0.0f, 0.5f); 
	glVertex3f(punto_start.x, punto_start.y, punto_start.z);
	glVertex3f(punto_start.x - spessore, punto_start.y, punto_start.z);
	glVertex3f(punto_end.x - spessore + inclinazione, punto_end.y, punto_end.z);
	glVertex3f(punto_end.x + inclinazione ,punto_end.y, punto_end.z);

	glEnd(); 

}
	
	


void drawEsterni(){


	drawFrontWall(lunghezza, dettaglio, segno_inclinazione, altezza);

	//drawGriglia(1.5f, 5.0f, 0.5, 6);

}




