#include "camera.h"
#include "model.h"
#include "simpleviewer.h"
#include "mesh2d.h"

//non contando la porta
GLfloat lunghezza = 30.0f;
GLfloat altezza = 7.5f;
GLfloat segno_inclinazione = 1.0f;
GLfloat profondita_lunghezza_ratio = 0.4f;


/* rapporti */
GLfloat lunghezza_larghezzaporta_ratio = 0.085f;
GLfloat lunghezza_altezza_ratio = 0.30f;
GLfloat lunghezza_spessore_ratio = 0.02f;
GLfloat lunghezza_sp_laterale_front_ratio = 0.2f;
GLfloat lunghezza_sp_laterale_back_ratio = -0.1f;
GLfloat lunghezza_inclinazione_frontale_ratio = 0.03f;
GLfloat differenza_ratio = 0.3f;

/* blending acceso/spento */
GLuint  blend;

/* riceve i punti con cui costruire un triangolo */
/* riceve i punti in senso ANTIORARIO */
void drawTriangle(Point3d* point1, Point3d* point2,Point3d* point3, GLfloat dettaglio) {

//	printf("x punto: %f, %f, %f\n", point1->x, point1->y, point1->z);
//	printf("y punto: %f, %f, %f\n", point2->x, point2->y, point2->z);
//	printf("z punto: %f, %f, %f\n", point3->x, point3->y, point3->z);

	Vector3d destNormal; 
	Vector3d diff1_2;
	Vector3d diff2_3;
	vector_diff(&diff1_2, point1, point2); 
	vector_diff(&diff2_3, point2, point3); 
	/* sbagliato!! */
//	vector_cross_product(&destNormal, &diff2_3, &diff1_2); 
	/* giusto!! */

	vector_cross_product(&destNormal, &diff1_2, &diff2_3); 


	/* devo impostare la normale del triangolo che sto costruendo: */
	/* verifica!! */

	/* la normale la imposto qui ? */
//	glNormal3f(destNormal.x,destNormal.y,destNormal.z);

//	printf("normale: %f %f %f\n", destNormal.x,destNormal.y,destNormal.z); 



/* se la minore distanza tra due punti del triangolo è minore di dettaglio: */
/* disegno il triangolo */

	GLfloat distanza_minima = minDistance(point1, point2, point3); 

//	printf("distanza minima = %f\n", distanza_minima);
//	glColor3f(0,1,0);
	if (distanza_minima <= dettaglio) {
		/* disegno triangoli oppure line loops */
		glBegin(draw_wireframe?GL_LINE_LOOP:GL_TRIANGLES);
			/* imposto qui la normale? */
			glNormal3f(destNormal.x,destNormal.y,destNormal.z);
		
			glVertex3f(point1->x, point1->y, point1->z);
			glVertex3f(point2->x, point2->y, point2->z);
			glVertex3f(point3->x, point3->y, point3->z);
		glEnd();	
		
		
/* disegno la normale, un segmento che collega due punti ed esce dalla superficie */
/*
		glBegin(GL_LINES);
			//glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(destNormal.x,destNormal.y,destNormal.z);
		glEnd();
*/		
		
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

//		glColor3f(1,0,0);
		/* triangolo A: 1-a-c */
		drawTriangle(point1, &point_a, &point_c, dettaglio);
//		glColor3f(0,1,0);
		/* triangolo B: a-b-c */
		drawTriangle(&point_a, &point_b, &point_c, dettaglio);
//		glColor3f(0,0,1);
		/* triangolo C: a-2-b */
		drawTriangle(&point_a, point2, &point_b, dettaglio);
//		glColor3f(1,1,1);
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


	GLfloat altezza = lunghezza * lunghezza_altezza_ratio;
	Point3d  a = {0,0,0};
	Point3d  b = {0,lunghezza,0};
	Point3d  c = {0,lunghezza,altezza};
	Point3d  d = {0,0,altezza};

	GLfloat spessore = lunghezza * lunghezza_spessore_ratio;
	GLfloat sp_laterale_front = lunghezza * lunghezza_sp_laterale_front_ratio;
	GLfloat sp_laterale_back = lunghezza * lunghezza_sp_laterale_back_ratio;
	GLfloat incl_frontale = lunghezza * lunghezza_inclinazione_frontale_ratio * inclinazione;
	/* differenza di altezza tra point3 e point4 */
	GLfloat differenza = differenza_ratio; 

	drawWallOblique(&a, &b, &c, &d, spessore, sp_laterale_front, sp_laterale_back, incl_frontale, dettaglio, differenza);

}


/* drawWall che disegna un muro a partire da una certa altezza, senza partire dalla base */
/* serve un'altra funzione che lo fa obliquo */
void drawWallSizeHigh(GLfloat lunghezza, GLfloat altezzaMax, GLfloat inclinazione,  GLfloat spessore, GLfloat dettaglio, GLfloat lambda, GLfloat sp_laterale_front, GLfloat sp_laterale_rear, GLfloat differenza) {


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

//	drawWall(&a1, &b1, &c, &d, spessore, sp_laterale, inclinazione, dettaglio);

	/* provo ad usare oblique */
	drawWallOblique(&a1, &b1, &c, &d, spessore, sp_laterale_front, sp_laterale_rear, inclinazione, dettaglio, differenza);

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

	//glColor3f(0.0f, 0.0f, 1.0f);


/*
	glBegin(GL_LINE_LOOP);
		glVertex3f(point1->x, point1->y, point1->z);
		glVertex3f(point2->x, point2->y, point2->z);
		glVertex3f(point3->x, point3->y, point3->z);
		glVertex3f(point4->x, point4->y, point4->z);
	glEnd();
*/
	/* faccia posteriore */	
/*
	glBegin(GL_LINE_LOOP);
		glVertex3f(point1t.x, point1t.y, point1t.z);
		glVertex3f(point2t.x, point2t.y, point2t.z);
		glVertex3f(point3t.x, point3t.y, point3t.z);
		glVertex3f(point4t.x, point4t.y, point4t.z);
	glEnd();
*/
	/* spigoli */
/*
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
*/



}




/* disegna un muro partendo da una certa altezza */

void drawWallHighNew(Point3d* point1, Point3d* point2, Point3d* point3, Point3d* point4, GLfloat spessore, GLfloat spigolo_laterale_front, GLfloat spigolo_laterale_rear, GLfloat incl_frontale, GLfloat dettaglio, GLfloat differenza, GLfloat altezza) {

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


	/* usando altezza devo calcolare la combinazione convessa tra 1_4 e tra 
	2_3 per alzare 1 e 2 */



	convexCombination(point1, point1, point4, altezza); 
	convexCombination(point2, point2, point3, altezza); 


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

//	glColor3f(0.0f, 0.0f, 1.0f);
/*
	glBegin(GL_LINE_LOOP);
		glVertex3f(point1->x, point1->y, point1->z);
		glVertex3f(point2->x, point2->y, point2->z);
		glVertex3f(point3->x, point3->y, point3->z);
		glVertex3f(point4->x, point4->y, point4->z);
	glEnd();
*/
	/* faccia posteriore */	
/*
	glBegin(GL_LINE_LOOP);
		glVertex3f(point1t.x, point1t.y, point1t.z);
		glVertex3f(point2t.x, point2t.y, point2t.z);
		glVertex3f(point3t.x, point3t.y, point3t.z);
		glVertex3f(point4t.x, point4t.y, point4t.z);
	glEnd();
*/
	/* spigoli */
/*
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
*/



}



void drawFrontWall(GLfloat lunghezza, GLfloat dettaglio, GLfloat segno_inclinazione, GLfloat altezza) {



	GLfloat lunghezza_w1 = lunghezza * 0.5f; 
	GLfloat lunghezza_w2 = lunghezza * 0.5f; // sarà minore
	
	GLfloat altezza_w2 = lunghezza * 0.30f;

	GLfloat larghezza_porta = lunghezza * lunghezza_larghezzaporta_ratio; 

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

/* prima parte */
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

	
	GLfloat sp_laterale_front = 0.0f;
	GLfloat sp_laterale_rear = 0.0f;
	GLfloat differenza = 0.0f;

	/* test per funzione highwall */
	drawWallSizeHigh(larghezza_porta, altezza_w1, incl_frontale, spessore, dettaglio, 0.55f, sp_laterale_front, sp_laterale_rear, differenza);



	glTranslatef(0.0f, larghezza_porta, 0.0f);

//	printf("c terzomuro : %f %f %f\n", c2.x, c2.y, c2.z);
//	printf("d terzomuro : %f %f %f\n", d2.x, d2.y, d2.z);

	/* todo */
	GLfloat compensazione_inclinazione = 0.38f;

	drawWallOblique(&a2, &b2, &c2, &d2, spessore, spigolo_laterale_front_w2, spigolo_laterale_back_w2, incl_frontale + compensazione_inclinazione, dettaglio, differenza_w2);

	
	glPopMatrix();
	/* ho disegnato il wallHigh */

//	glPushMatrix();



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

/* disegna un cerchio in senso orario approssimandolo con tanti lati quanti indicati in dettaglio */
GLdouble* drawCircle(GLfloat radius, GLint dettaglio, GLfloat Start_angolo, GLfloat End_angolo) 
{

//dettaglio è il numero di lati, per i punti mi serve un punto in più 

GLdouble* coordinate = malloc(2 * (dettaglio + 1) * sizeof(GLdouble));

glPushMatrix();

	/* angolo tra end e start - start deve essere precedente a end */
	/* in senso orario */

	/* la coordinata y non cambia, è un cerchio verticale */
	int i; 


	GLfloat misura_angolo = abs(End_angolo - Start_angolo);

//	printf("start %f  end %f misura %f\n", Start_angolo, End_angolo, misura_angolo); 


	GLfloat incremento = misura_angolo / dettaglio; 
	GLfloat incremento_rad = incremento * M_PI * 2 / 360 ; 

	/* sin e cos lavorano in radianti!!! */

	GLdouble valore_angolo = Start_angolo;
	GLdouble valore_angolo_rad = valore_angolo * M_PI * 2 / 360; 

	glBegin(GL_LINE_STRIP);

	for (i = 0; i <= dettaglio; i++) {
		GLdouble coseno = cos(valore_angolo_rad);
		GLdouble seno = sin(valore_angolo_rad);

		coordinate[2 * i] = radius * coseno;
		coordinate[2 * i + 1] = radius * seno;

		//valore_angolo = valore_angolo - incremento;

		valore_angolo_rad = valore_angolo_rad - incremento_rad;
	}

	glEnd();

	glPopMatrix();

	return coordinate;

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


	
void setGlassMaterial() {
	
	
	if (!blend) {
		glDisable(GL_BLEND);              // Turn Blending Off
		glEnable(GL_DEPTH_TEST);          // Turn Depth Testing On
		//glDisable(GL_LIGHTING);
	} else {
		glEnable(GL_BLEND);		    // Turn Blending On
		//glDisable(GL_DEPTH_TEST);         // Turn Depth Testing Off
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);		// Blending Function For Translucency Based On Source Alpha Value ( NEW )
	}
	
	GLfloat alpha = 0.8f;



	GLfloat material_ambient      []  = {0.1f, 0.2f, 0.4f, alpha}; 
	GLfloat material_diffuse      []  = {0.3f, 0.3f, 0.6f, alpha}; 
	GLfloat material_specular     []  = {0.7f, 0.7f, 0.7f, alpha}; 
	GLfloat material_emission     []  = {0.0f, 0.0f, 0.0f, alpha}; 
	GLfloat material_shininess    []  = {66};

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT  , material_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE  , material_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR , material_specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION , material_emission);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS, material_shininess);
	
	
}
	
	
	
	
	
	
	
void drawRearWall(GLfloat lunghezza, GLfloat dettaglio, GLfloat segno_inclinazione, GLfloat altezza) {


		glPushMatrix(); 

		GLfloat larghezza = profondita_lunghezza_ratio * lunghezza;

		glTranslatef(-larghezza, 0.0f, 0.0f);

		GLfloat lunghezza_w1 = lunghezza * 0.5f; 

		GLfloat lunghezza_w2_high = lunghezza * (0.5f + lunghezza_larghezzaporta_ratio);

		GLfloat lunghezza_w3_high = lunghezza_w2_high * 0.75f;


		GLfloat altezza_w2_high = lunghezza * lunghezza_altezza_ratio;

		GLfloat altezza_w3_high = altezza_w2_high * 0.999f; 


	/* differenza di altezza tra point3 e point4 di w1 */
		GLfloat differenza_w1 = lunghezza * 0.01; 

	/* differenza di altezza tra point3 e point4 di w2 */
		GLfloat differenza_w2_high = lunghezza * 0.01; 

	/* l'altezza del muro w1 è funzione dell'altezza di w2 * differenza altezza */
		GLfloat altezza_w1 = altezza_w2_high - (altezza_w2_high * differenza_w2_high);

	/* punti per w1 */
	Point3d  a1 = {0,0,0};
	Point3d  b1 = {0,lunghezza_w1,0};
	Point3d  c1 = {0,lunghezza_w1, altezza_w1};
	Point3d  d1 = {0,0,altezza_w1};

	/* punti per w2, altrimenti vengono modificati dalla drawWall */
	Point3d  a2 = {0,0,0};
	Point3d  b2 = {0,lunghezza_w2_high,0};
	Point3d  c2 = {0,lunghezza_w2_high, altezza_w2_high};
	Point3d  d2 = {0,0,altezza_w2_high};


	GLfloat spessore = lunghezza * 0.02f;
	GLfloat spigolo_laterale_front_w1 = 0.0f;
	GLfloat spigolo_laterale_back_w1 = lunghezza * -0.05f;
	GLfloat incl_frontale = lunghezza * 0.03f * segno_inclinazione;
	GLfloat incl_frontale_w2 = incl_frontale * 1.35f;
	
	/* prima parte */
	drawWallOblique(&a1, &b1, &c1, &d1, spessore, spigolo_laterale_front_w1, spigolo_laterale_back_w1, incl_frontale, dettaglio, differenza_w1);


	/* parte alta */
	glPushMatrix();
	glTranslatef(0.0f, lunghezza_w1, 0.0f);

	/* modifica i parametri */
	GLfloat spigolo_laterale_front_w2_high = lunghezza * 0.05f;
	GLfloat spigolo_laterale_back_w2_high = 0.0f;

	GLfloat differenza = 0.6f;

	/*altezza da cui parte il muro */
	GLfloat altezzaL = 0.4f;

//	drawWallSizeHigh(lunghezza_w2_high, altezza_w2_high, incl_frontale, spessore, dettaglio, 0.5f, spigolo_laterale_front_w2_high, spigolo_laterale_back_w2_high, differenza);


	/* riscrivo la draw wall high */
	drawWallHighNew(&a2, &b2, &c2, &d2, spessore, spigolo_laterale_front_w2_high, spigolo_laterale_back_w2_high, incl_frontale_w2, dettaglio, differenza_w2_high, altezzaL);

/* parte alta laterale */

GLfloat profondita = lunghezza * profondita_lunghezza_ratio + spessore + 0.03f; 



/* punti per muro laterale, altrimenti vengono modificati dalla drawWall */
	Point3d  a3 = {0,0,0};
	Point3d  b3 = {0,profondita,0};
	Point3d  c3 = {0,profondita, altezza_w3_high};
	Point3d  d3 = {0,0,altezza_w3_high};

/* punti per muro laterale basso, altrimenti vengono modificati dalla drawWall */
	Point3d  a4 = {spessore/2,0,0};
	Point3d  b4 = {spessore/2,profondita/2,0};
	Point3d  c4 = {0,profondita/2, altezza_w3_high * (1-altezzaL)};
	Point3d  d4 = {0,0,altezza_w3_high * (1-altezzaL)};


	glPushMatrix();
	glTranslatef(-spessore, lunghezza_w2_high, 0.0f); 
	glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);


/* levo tutte le chiamate ai materiali, le metto esterne */
//	setMaterial(1, 0, 0);

	GLfloat spigolo_laterale_back_w3_high = -1.21f;
	GLfloat spigolo_laterale_front_w3_high = 1.23f;
	GLfloat differenza_w3_high = 0.0f;

	drawWallHighNew(&a3, &b3, &c3, &d3, spessore, spigolo_laterale_front_w3_high, spigolo_laterale_back_w3_high, incl_frontale_w2, dettaglio, differenza_w3_high, altezzaL);

	glTranslatef(0.0f , profondita/2, 0.0f); 
	
	
//	setMaterial(0, 1, 0.5);
	//glutSolidSphere(0.5f, 10.0f, 10.0f);
	/* parte bassa */
	drawWallOblique(&a4, &b4, &c4, &d4, spessore, 0.74f, 0.0, incl_frontale+0.17f, dettaglio, differenza_w3_high);


	glPopMatrix();




	/* end parte alta laterale */

	glPopMatrix(); /* parte alta del muro */


	/* disegno il quadrilatero che sarà il vetro */
	GLfloat rapporto = altezzaL; 







/* coordinate del vetro */


/* il vetro si disegna alla fine, dopo tutti gli interni */
//	drawGlassRight();

/* ** refactorized **

	Point3d  uno = {0, 15, 0};
	Point3d  due = {0, 32.5f, 0};
	Point3d  tre = {5.7f, 32.5f, 0};
	
	Point3d  unoSu = {-0.6f +0.077f, 15, 3.8f };
	Point3d  dueSu = {-0.800000, 33.400034f, 5.4f };
	Point3d  treSu = {5.7f + 1.265217, 32.5f + 1.138695, 0 + 5.440430};
	
	
	glPushMatrix();
	
	
	setGlassMaterial();
	
	
	glBegin(GL_QUADS);
		glVertex3f(uno.x, uno.y, uno.z);
		glVertex3f(due.x, due.y, due.z);
		glVertex3f(dueSu.x, dueSu.y, dueSu.z);
		glVertex3f(unoSu.x, unoSu.y, unoSu.z);
		
		
		glVertex3f(tre.x, tre.y, tre.z);
		glVertex3f(due.x, due.y, due.z);
		glVertex3f(dueSu.x, dueSu.y, dueSu.z);
		glVertex3f(treSu.x, treSu.y, treSu.z);
		
	glEnd();
	
	
	glDisable(GL_BLEND);		

	glPopMatrix();

*/



	
	
	
	glPushMatrix();
		//primo punto: 0, 15, 0 
		glTranslatef(0.0f, 15.0f, 0.000000);
	//	glutSolidSphere(0.05f, 5.0f, 5.0f);
		//secondo punto: 0, 15+17.5, 0
		//glTranslatef(0.0f, 17.5, 0.000000);
	//	glutSolidSphere(0.05f, 5.0f, 5.0f);
		//terzo punto: 5.7f, 15+17.5, 0
		//glTranslatef(5.7f, 0.0f, 0.000000);
//		glutSolidSphere(0.05f, 5.0f, 5.0f);
		
		
		
		
		//punti in alto
		//terzo punto: 5,7, 15+1.5+1.000000, 5.4
		//glTranslatef(0.000000, 1.000000, 5.4);
//		glutSolidSphere(0.05f, 5.0f, 5.0f);

				
		//primo punto in alto: -0.6, 15, 3,8
		//glTranslatef(-0.600000, 0.000000, 3.799999);
//		glutSolidSphere(0.05f, 5.0f, 5.0f);
		
		//secondo punto in alto: -0.800000 15+18.400034 3.8 + 1.600000 
		//glTranslatef(xPosition, yPosition, zPosition);
//		glutSolidSphere(0.05f, 5.0f, 5.0f);

		
	glPopMatrix();
	
	
	glPopMatrix(); /* profondità palazzo */

}




void drawGlassRight() {

/* coordinate del vetro */

//disabilito illuminazione?
// xPosition, yPosition, zPosition

	Point3d  uno = {-0.126522, -2.530433, 0.000000};
	Point3d  due = {-0.126522, 32.626522, 0.000000};
	Point3d  tre = {11.393473, 32.500000, 0.000000};

	Point3d  unoSu = {-1.265217, -2.5, 7.595649};
	Point3d  dueSu = {-1.559130, 34.412209, 10.840429};

	Point3d  treSu = {11.519996, 34.524345, 10.880859};

	//printf("point3Su coords: %f %f %f\n", treSu.x, treSu.y, treSu.z);
		
	//1.265217 1.138695 5.440430
	
	glPushMatrix();
	
	
	setGlassMaterial();
	
	
	glBegin(GL_QUADS);
		glVertex3f(uno.x, uno.y, uno.z);
		glVertex3f(due.x, due.y, due.z);
		glVertex3f(dueSu.x, dueSu.y, dueSu.z);
		glVertex3f(unoSu.x, unoSu.y, unoSu.z);
		
		
		glVertex3f(tre.x, tre.y, tre.z);
		glVertex3f(due.x, due.y, due.z);
		glVertex3f(dueSu.x, dueSu.y, dueSu.z);
		glVertex3f(treSu.x, treSu.y, treSu.z);
		
	glEnd();
	
	
	glDisable(GL_BLEND);		    // Turn Blending Off

	glPopMatrix();
	
}









void drawGlass(Point3d* a, Point3d* b, Point3d* c, Point3d* d) {

/* punti locali */
Point3d v1; 
Point3d v2; 
Point3d v3; 
Point3d v4; 

init_point(a, &v1);
init_point(b, &v2);
init_point(c, &v3);
init_point(d, &v4);

Vector3d trasl_v2 = {0.0f, 2.5f, 0.0f};
Vector3d trasl_v3 = {0.0, 3.5f, -0.8f};
Vector3d trasl_v4 = {0.0f, 1.5f, -0.65f};

point_translate(&v2, &trasl_v2);
point_translate(&v3, &trasl_v3);
point_translate(&v4, &trasl_v4);

//printf("v1: %f %f %f\n", v1.x, v1.y, v1.z);
//printf("v2: %f %f %f\n", v2.x, v2.y, v2.z);
//printf("v3: %f %f %f\n", v3.x, v3.y, v3.z);
//printf("v4: %f %f %f\n", v4.x, v4.y, v4.z);


/* fino alla fine del muro */

	glPushMatrix();

	glTranslatef(b->x, b->y, b->z);

	Vector3d normale; 
	Vector3d v1_2;
	Vector3d v2_3;

	vector_diff(&v1_2, &v1, &v2);
	vector_diff(&v2_3, &v2, &v3);
	
//	printf("v1_2: %f %f %f\n", v1_2.x, v1_2.y, v1_2.z);
//	printf("v2_3: %f %f %f\n", v2_3.x, v2_3.y, v2_3.z);

	vector_cross_product(&normale, &v1_2, &v2_3);

//	printf("normale: %f %f %f\n", normale.x, normale.y, normale.z);

	glNormal3f(normale.x, normale.y, normale.z);

	// è disegnato in senso antiorario. giusto?
	// si illumina ugualmente sia che la luce sia davanti, che dietro. 
	/*
	glBegin(GL_QUADS);
		glVertex3f(v4.x, v4.y, v4.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v1.x, v1.y, v1.z);		
	glEnd();
	*/
	
	glBegin(GL_QUADS);
		glVertex3f(a->x, a->y, a->z);
		glVertex3f(b->x, b->y, b->z);
		glVertex3f(c->x, c->y, c->z);
		glVertex3f(d->x, d->y, d->z);
	glEnd();
	
	
	
	

/* altro vetro, fino a metà del muro laterale */

/* 2,3,5,6 */

//GLfloat lunghezza_glass = profondita/2;

//Point3d v5 = {lunghezza_glass, v3.y, v3.z}; 
//Point3d v6 = {lunghezza_glass, v2.y, 0.0f}; 

/*
	glBegin(GL_QUADS);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v5.x, v5.y, v5.z);
		glVertex3f(v6.x, v6.y, v6.z);
	glEnd();
*/

	/* disegno lo spigolo */

//	setMaterial(1,1,1); 

/*
	glBegin(GL_LINES);
		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);
	glEnd();
*/
	glPopMatrix();



}

/* piano continuo */
void plane(Point3d *coordinate, GLfloat riduzioneDettaglio) {

//	int i;			
//	printf("stampa coordinate: \n"); 
//		setMaterial(1, 0, 0);
		drawTriangle(&coordinate[0], &coordinate[1], &coordinate[2], dettaglio * riduzioneDettaglio);
		drawTriangle(&coordinate[0], &coordinate[2], &coordinate[3], dettaglio * riduzioneDettaglio);
/*
	for (i = 0; i<=3; i++) {
		printf("punto %d: %f %f %f\n", 
		i, coordinate[i].x, coordinate[i].y, coordinate[i].z);
	} 
*/


} 




/* tetto e muro curvo */
void drawBackCurveAndRoof() {


	GLfloat inclinazione = 9.0f; 

	GLint numPoints = 7 / dettaglio;


//	setMaterial(0.1,1,1); 

	glPushMatrix();
	glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, 4.5f);
	glTranslatef(0.0f, -profondita_lunghezza_ratio * lunghezza - 1.10f, 0.0f);

	GLint i = 0; 
	GLfloat profondita = 10.35f - (0.02 * lunghezza);

	GLdouble* circle = drawCircle(3.0f, numPoints, 90.0f, 210.0f);

	// problema: si illumina sia dentro che fuori. 
	
	// soluzione: un involucro che si illumina in un modo e uno che si illumina in un altro.
	
	
	GLfloat R = 1.0f;
	GLfloat G = 1.0f;
	GLfloat B = 1.0f;
	
	GLfloat Ka = 1.0f;
	GLfloat Kd = 0.8f;
	GLfloat Ks = 0.8f;
	GLfloat Ke = 0.0f;
	GLfloat shininess = 1.0f;
	
	
	GLfloat material_ambient      []  = {Ka*R, Ka*G, Ka*B, 1.0f}; 
	GLfloat material_diffuse      []  = {Kd*R, Kd*G, Kd*B, 1.0f}; 
	GLfloat material_specular     []  = {Ks*R, Ks*G, Ks*B, 1.0f}; 
	GLfloat material_emission     []  = {Ke*R, Ke*G, Ke*B, 1.0f}; 
	GLfloat material_shininess    []  = {shininess};

	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE  , material_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR , material_specular);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION , material_emission);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS, material_shininess);
	
	
	drawShell(circle, numPoints, profondita+4);
	
	
	// disegno un'altra shell, per farla illuminare diversamente
	
	glPushMatrix();
		glScalef(1.0, 0.99f, 0.9f);
		glTranslatef(0.0f, yPosition, zPosition);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE  , material_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR , material_specular);
		glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION , material_emission);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS, material_shininess);
		
		drawShellInvertedNormals(circle, numPoints, profondita+4);
			
	glPopMatrix();	
	
	glPopMatrix();


}


void drawShellInvertedNormals(GLdouble* circle, GLint numPoints, GLfloat profondita) {

	Point3d a; 
	Point3d b; 
	Point3d c; 
	Point3d d; 
	
	int i = 0;

	GLfloat deltaInclinazione = 0.8f;

	GLfloat deltaY = deltaInclinazione/numPoints;

	/* posso inclinare i punti mano mano che li disegno */

	for (i = 0; i < numPoints; i++) {
	
		/* primo */
		a.x = circle[2 * i];
		if (i == 0 ) { 
			a.y = 0.0f;
			c.y = profondita;
		}
			else {
				a.y = (deltaY * (i-1)); 
				c.y = profondita - (deltaY * (i-1));
			}
		a.z = circle[(2 * i)  + 1];
		/* secondo */
		b.x = circle[(2 * i) + 2];
		b.y = 0.0f + (deltaY * i); 
		b.z = circle[(2 * i) + 3];
	
		/* punti distanti a profondita */
		/* terzo */
		c.x = circle[2 * i];
	// c.y	 
		c.z = circle[(2 * i)  + 1];
		/* quarto */
		d.x = circle[(2 * i) + 2];
		d.y = profondita - (deltaY * i); 
		d.z = circle[(2 * i) + 3];
	
	
		/* come mi organizzo per le normali? */
	
		/* il dettaglio è un pò cambiato */


//oriento in maniera diversa i triangle
		drawTriangle(&a, &c, &b, dettaglio * 0.7f);
		drawTriangle(&b, &c, &d, dettaglio * 0.7f);

/* oriento in modo diverso le facce della parte curva */
	//	drawTriangle(&a, &b, &c, dettaglio * 0.7f);
	//	drawTriangle(&b, &d, &c, dettaglio * 0.7f);


/* ma le disegno anche nell' altro senso, per fare l'illuminazione anche all'esterno */
	/*
		glPushMatrix();
			drawTriangle(&a, &c, &b, dettaglio * 0.7f);
			drawTriangle(&b, &c, &d, dettaglio * 0.7f);
		glPopMatrix();
*/
	}//for


	Point3d e;
	Point3d f;
	GLfloat altezza = 4.5f; 

	/* punto d, in alto a dx */
	/* punto b, in alto a sx */

	/* punto e, in basso a sx */
	e.x = circle[0]; 
	e.y = 0.0f + (deltaY * numPoints + 0.3f);
	e.z = - altezza; 

	/* punto f, in basso a dx */
	f.x = circle[0]; 
	f.y = profondita - (deltaY * numPoints + 0.3f);
	f.z = - altezza;
	
	/* normali ? */
	/* muro obliquo retrostante */
	
	/*
	drawTriangle(&d, &b, &e, dettaglio);
	drawTriangle(&e, &f, &d, dettaglio);
	*/
	
	drawTriangle(&d, &e, &b, dettaglio);
	drawTriangle(&e, &d, &f, dettaglio);
	


	/* da e ed f traccio dei triangoli con ogni punto del cerchio */


	/* centro faccia profondita */

	Point3d h; 
	h.x = 0.0f;
	h.y = profondita - (deltaY * numPoints / 2); 
	h.z = 0.0f; 
	
	drawTriangle(&h, &f, &d, dettaglio);

	Point3d temp1; 
	Point3d temp2; 
	Point3d temp3; 
	Point3d temp4;

	/* centro faccia 0.0f */
	Point3d j; 
	j.x = 0.0f;
	j.y = 0.0f + (deltaY * numPoints / 2); 
	j.z = 0.0f; 
	
	drawTriangle(&j, &b, &e, dettaglio);

	/* disegno i triangolini laterali */



	for (i = 0; i < numPoints; i++) {
	
	if (i == 0 ) { 
				temp1.y = profondita;
		}
			else {
				temp1.y = profondita - (deltaY * (i-1));
			}

		temp1.x = circle[2 * i]; 
		temp1.z = circle[(2 * i) + 1]; 
		temp2.x = circle[(2 * i) + 2]; 
		temp2.y = profondita - (deltaY * (i));
		temp2.z = circle[(2 * i) + 3]; 

		


		drawTriangle(&h, &temp2, &temp1, dettaglio);

		/* e poi li disegno sull'altra faccia */
		/*
		temp3.x = circle[2 * i]; 
		temp3.y = profondita - (deltaY * (i-1));
		temp3.z = circle[(2 * i) + 1]; 

		temp4.x = circle[(2 * i) + 2]; 
		temp4.y = profondita - (deltaY * (i));
		temp4.z = circle[(2 * i) + 3]; 
		*/
		
		if ( i == 0 ) {
			temp1.y = 0.0f;
		}
		else {
			temp1.y = 0.0f + (deltaY * (i-1));
		}

		temp2.y = 0.0f + (deltaY * (i));		

		drawTriangle(&j, &temp1, &temp2, dettaglio);

	}



	Point3d start1; 
	start1.x = circle[0];
	start1.y = profondita; 
	start1.z = circle[1];

	Point3d start2; 
	start2.x = circle[0];
	start2.y = 0.0f; 
	start2.z = circle[1];

	GLfloat adjust = 3.8f; 

	Point3d front1 = {circle[0] - lunghezza - adjust, profondita, circle[1]};
	Point3d front2 = {circle[0] - lunghezza - adjust, 0.0f, circle[1]};


	/* normali!! 
	é il piano superiore */
	/*
	drawTriangle(&front1, &front2, &start1, dettaglio);
	drawTriangle(&start1, &front2, &start2, dettaglio);

	
	drawTriangle(&front1, &start1, &h, dettaglio);
	drawTriangle(&front2, &j, &start2, dettaglio);
	*/
	
	
	drawTriangle(&front1, &start1, &front2, dettaglio);
	drawTriangle(&start1, &start2, &front2, dettaglio);

	
	drawTriangle(&front1, &h, &start1, dettaglio);
	drawTriangle(&front2, &start2, &j, dettaglio);

}



void drawShell(GLdouble* circle, GLint numPoints, GLfloat profondita) {

	Point3d a; 
	Point3d b; 
	Point3d c; 
	Point3d d; 
	
	int i = 0;

	GLfloat deltaInclinazione = 0.8f;

	GLfloat deltaY = deltaInclinazione/numPoints;

	/* posso inclinare i punti mano mano che li disegno */

	for (i = 0; i < numPoints; i++) {
	
		/* primo */
		a.x = circle[2 * i];
		if (i == 0 ) { 
			a.y = 0.0f;
			c.y = profondita;
		}
			else {
				a.y = (deltaY * (i-1)); 
				c.y = profondita - (deltaY * (i-1));
			}
		a.z = circle[(2 * i)  + 1];
		/* secondo */
		b.x = circle[(2 * i) + 2];
		b.y = 0.0f + (deltaY * i); 
		b.z = circle[(2 * i) + 3];
	
		/* punti distanti a profondita */
		/* terzo */
		c.x = circle[2 * i];
	// c.y	 
		c.z = circle[(2 * i)  + 1];
		/* quarto */
		d.x = circle[(2 * i) + 2];
		d.y = profondita - (deltaY * i); 
		d.z = circle[(2 * i) + 3];
	
	
		/* come mi organizzo per le normali? */
	
		/* il dettaglio è un pò cambiato */

/*
		drawTriangle(&a, &c, &b, dettaglio * 0.7f);
		drawTriangle(&b, &c, &d, dettaglio * 0.7f);
*/
/* oriento in modo diverso le facce della parte curva */
		drawTriangle(&a, &b, &c, dettaglio * 0.7f);
		drawTriangle(&b, &d, &c, dettaglio * 0.7f);


/* ma le disegno anche nell' altro senso, per fare l'illuminazione anche all'esterno */
	/*
		glPushMatrix();
			drawTriangle(&a, &c, &b, dettaglio * 0.7f);
			drawTriangle(&b, &c, &d, dettaglio * 0.7f);
		glPopMatrix();
*/
	}//for


	Point3d e;
	Point3d f;
	GLfloat altezza = 4.5f; 

	/* punto d, in alto a dx */
	/* punto b, in alto a sx */

	/* punto e, in basso a sx */
	e.x = circle[0]; 
	e.y = 0.0f + (deltaY * numPoints + 0.3f);
	e.z = - altezza; 

	/* punto f, in basso a dx */
	f.x = circle[0]; 
	f.y = profondita - (deltaY * numPoints + 0.3f);
	f.z = - altezza;
	
	/* normali ? */
	/* muro obliquo retrostante */
	drawTriangle(&d, &b, &e, dettaglio);
	drawTriangle(&e, &f, &d, dettaglio);


	/* da e ed f traccio dei triangoli con ogni punto del cerchio */


	/* centro faccia profondita */

	Point3d h; 
	h.x = 0.0f;
	h.y = profondita - (deltaY * numPoints / 2); 
	h.z = 0.0f; 
	drawTriangle(&h, &d, &f, dettaglio);

	Point3d temp1; 
	Point3d temp2; 
	Point3d temp3; 
	Point3d temp4;

	/* centro faccia 0.0f */
	Point3d j; 
	j.x = 0.0f;
	j.y = 0.0f + (deltaY * numPoints / 2); 
	j.z = 0.0f; 
	drawTriangle(&j, &e, &b, dettaglio);

	/* disegno i triangolini laterali */



	for (i = 0; i < numPoints; i++) {
	
	if (i == 0 ) { 
				temp1.y = profondita;
		}
			else {
				temp1.y = profondita - (deltaY * (i-1));
			}

		temp1.x = circle[2 * i]; 

		temp1.z = circle[(2 * i) + 1]; 

		temp2.x = circle[(2 * i) + 2]; 
		temp2.y = profondita - (deltaY * (i));
		temp2.z = circle[(2 * i) + 3]; 

		


		drawTriangle(&h, &temp1, &temp2, dettaglio);

		/* e poi li disegno sull'altra faccia */
		/*
		temp3.x = circle[2 * i]; 
		temp3.y = profondita - (deltaY * (i-1));
		temp3.z = circle[(2 * i) + 1]; 

		temp4.x = circle[(2 * i) + 2]; 
		temp4.y = profondita - (deltaY * (i));
		temp4.z = circle[(2 * i) + 3]; 
		*/
		
		if ( i == 0 ) {
			temp1.y = 0.0f;
		}
		else {
			temp1.y = 0.0f + (deltaY * (i-1));
		}

		temp2.y = 0.0f + (deltaY * (i));		

		drawTriangle(&j, &temp2, &temp1, dettaglio);

	}



	Point3d start1; 
	start1.x = circle[0];
	start1.y = profondita; 
	start1.z = circle[1];

	Point3d start2; 
	start2.x = circle[0];
	start2.y = 0.0f; 
	start2.z = circle[1];

	GLfloat adjust = 3.8f; 

	Point3d front1 = {circle[0] - lunghezza - adjust, profondita, circle[1]};
	Point3d front2 = {circle[0] - lunghezza - adjust, 0.0f, circle[1]};


	/* normali!! 
	é il piano superiore */
	drawTriangle(&front1, &front2, &start1, dettaglio);
	drawTriangle(&start1, &front2, &start2, dettaglio);

	
	drawTriangle(&front1, &start1, &h, dettaglio);
	drawTriangle(&front2, &j, &start2, dettaglio);

}




/* funzione per impostare i materiali */
void setMaterial(GLfloat R,GLfloat G,GLfloat B)

{

	GLfloat material_ambient      []  = {0.1f*R, 0.1f*G, 0.1f*B, 1.0f}; 

	GLfloat material_diffuse      []  = {0.3f*R, 0.3f*G, 0.3f*B, 1.0f}; 

	GLfloat material_specular     []  = {0.5f*R, 0.5f*G, 0.5f*B, 1.0f}; 

	GLfloat material_emission     []  = {0.0f*R, 0.0f*G, 0.0f*B, 1.0f}; 

	GLfloat material_shininess    []  = {30};

	

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT  , material_ambient);

	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE  , material_diffuse);

	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR , material_specular);

	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION , material_emission);

	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS, material_shininess);

}




/* funzione per impostare i materiali */
void setMaterialType(GLfloat R,GLfloat G,GLfloat B, unsigned char tipo)

{

	GLfloat Ka; 
	GLfloat Kd; 
	GLfloat Ks;
	GLfloat Ke;
	GLfloat shininess; 
	
switch(tipo) {
	case 'e':
		//printf("materiale muro\n");
/* set delle variabili con glMaterial... */
		Ka = 0.05f;
		Kd = 0.66f;
		Ks = 0.01f;
		Ke = 0.0f;
		shininess = 22;
		break; 
		
	case 'o':
		//printf("materiale opaco tipo gomma\n");		
/* set delle variabili con glMaterial... */
		Ka = 0.1f;
		Kd = 0.1f;
		Ks = 0.01;
		Ke = 0.0f;
		shininess = 5;
		break; 
		
	case 'm':
		//printf("materiale metallico\n");
		Ka = 0.8;
		Kd = 0.5;
		Ks = 0.8f;
		Ke = 0.0f;
		shininess = 77;
		break;
	
}

//	printf("comincia set material\n");		


	GLfloat material_ambient      []  = {Ka*R, Ka*G, Ka*B, 1.0f}; 

	GLfloat material_diffuse      []  = {Kd*R, Kd*G, Kd*B, 1.0f}; 

	GLfloat material_specular     []  = {Ks*R, Ks*G, Ks*B, 1.0f}; 

	GLfloat material_emission     []  = {Ke*R, Ke*G, Ke*B, 1.0f}; 

	GLfloat material_shininess    []  = {shininess};

//	printf("ambient: %f, %f, %f\n", material_ambient[0], material_ambient[1], material_ambient[2]);	

	glMaterialfv(GL_FRONT,GL_AMBIENT  , material_ambient);

	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE  , material_diffuse);

	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR , material_specular);

	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION , material_emission);

	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS, material_shininess);

}



void drawCuboid() {

//	glutSolidSphere(0.2f, 5.0f, 5.0f);
	
	GLfloat altezza = 0.2f; 
	GLfloat lunghezza = 24.0f; 
	GLfloat profondita = 24.0f; 
	
	
	
/* base di sotto */
	Point3d a = {0, 0, 0};
	Point3d b = {0, profondita, 0};
	Point3d c = {lunghezza, profondita, 0};
	Point3d d = {lunghezza, 0, 0};
	
	
	
	Point3d a1 = {0, 0, altezza};
	Point3d b1 = {0, profondita, altezza};
	Point3d c1 = {lunghezza, profondita, altezza};
	Point3d d1 = {lunghezza, 0, altezza};
	
	
	
	
	/* faccia di sotto */
	
	drawTriangle(&a, &b, &c, dettaglio);
	drawTriangle(&a, &c, &d, dettaglio);	

	
	/* faccia di sopra */
	
	drawTriangle(&a1, &c1, &b1, dettaglio);
	drawTriangle(&a1, &d1, &c1, dettaglio);	
	
	/* facce laterali */
		
	drawTriangle(&b, &a1, &b1, dettaglio);
	drawTriangle(&b, &a, &a1, dettaglio);
	
	drawTriangle(&a, &d1, &a1, dettaglio);
	drawTriangle(&a, &d, &d1, dettaglio);
	
	drawTriangle(&d, &c1, &d1, dettaglio);
	drawTriangle(&d, &c, &c1, dettaglio);
	
	drawTriangle(&c, &b1, &c1, dettaglio);
	drawTriangle(&c, &b, &b1, dettaglio);
	
}	


void drawPlane() {

//	glutSolidSphere(0.2f, 5.0f, 5.0f);
	
	GLfloat altezza = 0.2f; 
	GLfloat lunghezza = 23.0f; 
	GLfloat profondita = 23.0f; 
	
	
	GLfloat distanzaY = 11.0f;
	GLfloat distanzaX = 9.0f;
	GLfloat larghezzaBuco = 2.0f;
	GLfloat lunghezzaBuco = 6.0f;
	
/* base di sotto */
	Point3d a = {0, 0, 0};
	Point3d b = {0, profondita, 0};
	Point3d c = {lunghezza, profondita, 0};
	Point3d d = {lunghezza, 0, 0};
	Point3d e = {distanzaX, distanzaY, 0};
	Point3d f = {distanzaX, distanzaY + larghezzaBuco, 0};
	Point3d g = {distanzaX + lunghezzaBuco, distanzaY + larghezzaBuco, 0};
	Point3d h = {distanzaX + lunghezzaBuco, distanzaY, 0};
	Point3d i = {distanzaX, profondita, 0};
	Point3d j = {distanzaX + lunghezzaBuco, profondita, 0};
	Point3d k = {distanzaX + lunghezzaBuco, 0, 0};
	Point3d l = {distanzaX, 0, 0};
	
	
	
	Point3d a1 = {0, 0, altezza};
	Point3d b1 = {0, profondita, altezza};
	Point3d c1 = {lunghezza, profondita, altezza};
	Point3d d1 = {lunghezza, 0, altezza};
	Point3d e1 = {distanzaX, distanzaY, altezza};
	Point3d f1 = {distanzaX, distanzaY + larghezzaBuco, altezza};
	Point3d g1 = {distanzaX + lunghezzaBuco, distanzaY + larghezzaBuco, altezza};
	Point3d h1 = {distanzaX + lunghezzaBuco, distanzaY, altezza};
	Point3d i1 = {distanzaX, profondita, altezza};
	Point3d j1 = {distanzaX + lunghezzaBuco, profondita, altezza};
	Point3d k1 = {distanzaX + lunghezzaBuco, 0, altezza};
	Point3d l1 = {distanzaX, 0, altezza};
	
	
	
	/* faccia di sotto con buco */
	
	drawTriangle(&a, &b, &i, dettaglio);
	drawTriangle(&a, &i, &l, dettaglio);	
	drawTriangle(&k, &j, &c, dettaglio);
	drawTriangle(&k, &c, &d, dettaglio);

	drawTriangle(&f, &i, &j, dettaglio);
	drawTriangle(&f, &j, &g, dettaglio);
	
	drawTriangle(&l, &e, &h, dettaglio);	
	drawTriangle(&l, &h, &k, dettaglio);


	
	
	/* faccia di sopra con buco */
	
	drawTriangle(&a1, &i1, &b1, dettaglio);
	drawTriangle(&a1, &l1, &i1, dettaglio);	
	drawTriangle(&k1, &c1, &j1, dettaglio);
	drawTriangle(&k1, &d1, &c1, dettaglio);

	drawTriangle(&f1, &j1, &i1, dettaglio);
	drawTriangle(&f1, &g1, &j1, dettaglio);
	
	drawTriangle(&l1, &h1, &e1, dettaglio);	
	drawTriangle(&l1, &k1, &h1, dettaglio);
	
	/* facce laterali */
	
	
	drawTriangle(&b, &a1, &b1, dettaglio);
	drawTriangle(&b, &a, &a1, dettaglio);
	
	drawTriangle(&a, &d1, &a1, dettaglio);
	drawTriangle(&a, &d, &d1, dettaglio);
	
	drawTriangle(&d, &c1, &d1, dettaglio);
	drawTriangle(&d, &c, &c1, dettaglio);
	
	drawTriangle(&c, &b1, &c1, dettaglio);
	drawTriangle(&c, &b, &b1, dettaglio);
	
	/* facce interne */
	
	drawTriangle(&f, &g1, &f1, dettaglio);
	drawTriangle(&f, &g, &g1, dettaglio);
	
	drawTriangle(&g, &h1, &g1, dettaglio);
	drawTriangle(&g, &h, &h1, dettaglio);
	
	drawTriangle(&h, &e1, &h1, dettaglio);
	drawTriangle(&h, &e, &e1, dettaglio);
	
	drawTriangle(&e, &f1, &e1, dettaglio);
	drawTriangle(&e, &f, &f1, dettaglio);

}	


void drawPlaneEsterni(Point3d *coordinate, GLfloat estensione) {

	Point3d e;
	Point3d f;
	Point3d g; 
	Point3d h; 
	Point3d i; 
	Point3d j;
	Point3d k; 
	Point3d l;
	
	GLfloat dettaglioLocale = dettaglio * 2.0f;
	
	init_point(&coordinate[0], &e);
	init_point(&coordinate[1], &f);
	init_point(&coordinate[2], &g);
	init_point(&coordinate[3], &h);
	e.y = e.y - estensione;
	f.y = f.y - estensione;
	g.y = g.y + estensione; 
	h.y = h.y + estensione;
	
	init_point(&e, &i);
	init_point(&f, &j);
	init_point(&h, &l);
	init_point(&g, &k);
	i.x = i.x - estensione;
	j.x = j.x + estensione;
	l.x = l.x - estensione;
	k.x = k.x + estensione;
	
	
	drawTriangle(&e, &l, &i, dettaglioLocale);
	drawTriangle(&e, &h, &l, dettaglioLocale);
	
	drawTriangle(&f, &coordinate[0], &e, dettaglioLocale);
	drawTriangle(&f, &coordinate[1], &coordinate[0], dettaglioLocale);

	drawTriangle(&coordinate[2], &h, &coordinate[3], dettaglioLocale);	
	drawTriangle(&coordinate[2], &g, &h, dettaglioLocale);	
	
	drawTriangle(&j, &g, &f, dettaglioLocale);	
	drawTriangle(&j, &k, &g, dettaglioLocale);	

}	








void drawFloor() {

	GLfloat length = lunghezza + (lunghezza * lunghezza_larghezzaporta_ratio);
	GLfloat height = profondita_lunghezza_ratio * lunghezza;

	Point3d a = {0, 0, 0};
	Point3d b = {length, 0, 0};
	Point3d c = {length, height, 0};
	Point3d d = {0, height, 0};
	
	drawTriangle(&a, &b, &c, dettaglio);
	drawTriangle(&a, &c, &d, dettaglio);	

}

void drawPlanes(GLint npiani) {
	glPushMatrix();
	glTranslatef(-24.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 40.0f, 4.0f);
	int i; 
	for(i = 1; i<=npiani; i++) {
	
		drawPlane();
		glTranslatef(0.0f, 0.0f, 4.0f);
		
	}

	glPopMatrix();

}




void drawEsterni(){
	
	
	glPushMatrix();

	/* ingrandisce la dimensione della struttura */
	glScalef(2.0f, 2.0f, 2.0f); 
	
	
	
	/* inserisco qui la roba delle luci, o raddoppio le coordinate ?*/
	
	
	
	
	
	
	
	
	
	
	
	
	/* pavimento all'esterno */
	Point3d *coordinate4= malloc(4 * 3 * sizeof(GLfloat));
	
	GLfloat lunghezza1 = 32.50f;
	GLfloat profondita1 = 12.0f;
	GLfloat aggiuntaExt = 150.0f;
	GLfloat estensione = 40.0f + aggiuntaExt;
	/* piano semplice */
	Point3d a = {0.0f, 0.0f, 0.0f};
	Point3d b = {profondita1, 0.0f, 0.0f};
	Point3d c = {profondita1, lunghezza1, 0.0f};
	Point3d d = {0.0f, lunghezza1, 0.0f};
	
	
	coordinate4[0] = a;
	coordinate4[1] = b;
	coordinate4[2] = c;
	coordinate4[3] = d;
	
	
	glPushMatrix();

	glTranslatef(-12.0f, 0.0f, 0.0f);	
	//materiale pavimento esterno
	setMaterialType(0.0f, 1.0f, 0.0f, 'o');
	drawPlaneEsterni(coordinate4, estensione);

	glPopMatrix();
	
	glPushMatrix();
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		setMaterial(1.0, 0.0, 0.0);
		//pavimento interno
		drawFloor();
	glPopMatrix();


/* ri-setta il materiale per gli esterni del palazzo*/

	setMaterialType(1, 1, 1, 'e');



	//setMaterial(1,1,0);
	/* chiamate per settare i materiali */
//	setMaterialType(1,1,0, 'p');
	drawFrontWall(lunghezza, dettaglio, segno_inclinazione, altezza);

	//setMaterial(1,1,1);
	drawRearWall(lunghezza, dettaglio, -segno_inclinazione, altezza);

	//drawGriglia(1.5f, 5.0f, 0.5, 6);

	/* copertura tetto e muro laterale */

	setMaterialType(1,1,1, 'e');

	drawBackCurveAndRoof(); 

	glPopMatrix();
	
	
	
	
	
	
	/* disegno una sfera esterna con texture per fare lo sfondo */
	
	glEnable(GL_TEXTURE_2D);			// Enable texture mapping



	// sfera
	quadratic = gluNewQuadric();                  // Create A Pointer To The Quadric Object ( NEW )
	
	// e' rivolto verso l'interno, per le texture
	gluQuadricOrientation(quadratic, GLU_INSIDE);
	

    gluQuadricNormals(quadratic, GLU_SMOOTH);   // Create Smooth Normals
	gluQuadricTexture(quadratic, GL_TRUE);      // Create Texture Coords ( NEW )
	
	
	gluQuadricDrawStyle(quadratic, draw_wireframe?GLU_LINE:GLU_FILL);

	
	glBindTexture(GL_TEXTURE_2D, texture[filter]);   // choose the texture to use.
	setMaterialType(1,1,1, 'e');


	gluSphere(quadratic,410,10,10);                // Draw A Sphere


	glDisable(GL_TEXTURE_2D);			// disable texture mapping

		
	
}



void poligono(int n, GLfloat raggio){

	GLint lati = 20;
	GLfloat alpha = 2.0 * M_PI / lati;
	int i;
	
	glBegin(draw_wireframe?GL_LINE_LOOP:GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f); //normale
	for (i = 0; i < 20; i++) {
		glTexCoord2f(raggio * cos(alpha * i), raggio *sin(alpha * i));
		glVertex3f(raggio * cos(alpha * i), raggio * sin(alpha * i), 0.0f);
	}
	glEnd();


}

void cilindro(int n, GLfloat raggio) {

//a partire dalla base

	GLint lati = 20;
	GLfloat alpha = 2.0 * M_PI / lati;
	GLfloat angle0, angle1, anglem;
	int i;

	glPolygonMode(GL_FRONT,GL_FILL);
	
	glBegin(draw_wireframe?GL_LINE_LOOP:GL_QUADS);
	
	for (i = 0; i < lati; i++) {
		angle0 = i * alpha;
		angle1 = (i+1) * alpha;
		anglem = (angle1 + angle0) / 2;
		
		glNormal3f(raggio * cos(anglem), raggio * sin(anglem), 0);
		glTexCoord2f(0,0);
		glVertex3f(raggio * cos(angle0), raggio * sin(angle0), 0);
		glTexCoord2f(0,1);
		glVertex3f(raggio * cos(angle0), raggio * sin(angle0), 1);
		glTexCoord2f(1,1);
		glVertex3f(raggio * cos(angle1), raggio * sin(angle1), 1);
		glTexCoord2f(1,0);
		glVertex3f(raggio * cos(angle1), raggio * sin(angle1), 0);
	}
	glEnd();

}



void drawSingleColumn(GLfloat raggio, GLfloat altezzaColonne) {


	GLint lati_poligono;

	glPushMatrix();
	
	glScalef(1.0f, 1.0f, altezzaColonne);

	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);	
	poligono(lati_poligono, raggio);	
	glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);	
	cilindro(lati_poligono, raggio);	
	glTranslatef(0.0f, 0.0f, 1.0f);	
	poligono(lati_poligono, raggio);
	
	glPopMatrix();

}


void drawLampadaSecca() {



//	lampada appesa in alto, bianca (o acciaio)
		setMaterialType(0.5, 0.5, 0.5, 'e');	




/* disegno la lampada secca */
	glPushMatrix();
		//glLoadIdentity();
		
		glScalef(1.0, 1.0, 1.0);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		//glTranslatef(0.0f, 0.0f, 4.0f);
		glTranslatef(48.099838, 2.200000, 4.199998);
		
		displayPly(meshes[10]);
		
		GLfloat lunghezzaLampada = 2.0f;
		
		int j; 
		int numLampade = 10;
		for (j = 0; j<numLampade; j++) {
			glTranslatef(0.0f, lunghezzaLampada, 0.0f);
			displayPly(meshes[10]);
		}
	
	glPopMatrix();


}


void drawColumns(GLfloat raggio, GLfloat altezzaColonne) {
	
	glPushMatrix();
	
	glTranslatef(-21.900047, 59.799660, 0.000000);
	//glTranslatef(xPosition, yPosition, zPosition); 
	
	
	GLfloat lunghezzaPiano, larghezzaPiano;


	lunghezzaPiano = 19.0f; 
	larghezzaPiano = 18.0f;
	
	drawSingleColumn(raggio, altezzaColonne);
	
	glTranslatef(lunghezzaPiano, 0, 0); 
	
	drawSingleColumn(raggio, altezzaColonne);
	
	glTranslatef(0, -larghezzaPiano, 0);
	
	drawSingleColumn(raggio, altezzaColonne);
	
	glTranslatef(-lunghezzaPiano, 0, 0); 	
	
	drawSingleColumn(raggio, altezzaColonne);
	glPopMatrix();
}


void drawCassa() {

			glPushMatrix();
			glScalef(0.6, 0.6, 0.6);
			displayPly(meshes[0]);
			glTranslatef(0.0f, 0.0f, 2.0);
			displayPly(meshes[1]);
			glPopMatrix();


}

void drawPianoTerra() {

		glPushMatrix();
		
		glTranslatef(-3.899998, 57.399696, 0.300000);
		
		/* contenitore per le mele */
		glPushMatrix();
			glScalef(0.5f, 0.5f, 0.8f);
			glTranslatef(0.0f, 0.0f, 1.38f);
			
			// viola
			setMaterialType(0, 0, 1, 'e');
			
			displayPly(meshes[3]);
			
			/* mele */
			glScalef(0.3, 0.3, 0.3);
			glTranslatef(1.200000, 0.700000, -0.60);
			
			// rossa
			setMaterialType(1, 0, 0, 'e');
			
			displayPly(meshes[4]);
			glRotatef(44.0, 0.0, 0.0, 1.0);
			glTranslatef(-2.100000, -1.100000f, 0.0f);
			
			//verde
			setMaterialType(0, 1, 0, 'e');
			
			displayPly(meshes[4]);			
			glRotatef(77.0, 0.0, 0.0, 1.0);
			glTranslatef(3.099999, 0.0, 0.0); 
			
			//arancione
			setMaterialType(0.95, 0.5, 0.15, 'e');
			
			displayPly(meshes[4]);			

		glPopMatrix();
		
		glScalef(0.3f, 0.3f, 0.3f);
		
		
		
		setMaterialType(1,1,1, 'e');
		
		displayPly(meshes[8]);
		// finito il primo espositore
		
	
		
		glTranslatef(0.0f, -16.0f, 0.0f);
		glPushMatrix();
		glTranslatef(-0.00000, 0.500000, 1.9000);
		
			// giallo
			setMaterialType(0.93, 0.74, 0.11, 'e');
			
			displayPly(meshes[5]);
		glPopMatrix();
		

		setMaterialType(1, 1, 1, 'e');		
		displayPly(meshes[8]);
		// banana sul secondo espositore


		//glTranslatef(xPosition, yPosition, zPosition);
		glTranslatef(-21.3f, -33.5f, 0.0f);
		glPushMatrix();
		glTranslatef(0.000000, 0.100000, 1.800000);


			//terracotta
			setMaterialType(0.56, 0.26, 0.21, 'e');		

			displayPly(meshes[11]);
		glPopMatrix();

		setMaterialType(1, 1, 1, 'e');		

		displayPly(meshes[8]);
		
		//vaso sul terzo espositore
		
		glPopMatrix();
		
		
		glPushMatrix(); 
			glTranslatef(-15.600023, 57.799690, 0.55);
			//glTranslatef(-15.600023, 57.799690, 0.00);
			
			//nero
			setMaterialType(0.14, 0.17, 0.19, 'e');		

			drawCassa();
			glTranslatef(8.0f, 0.0f, 0.0f);
			drawCassa();
		glPopMatrix();


	

		drawLampadaSecca();
		
}




void drawLampada() {

	glPushMatrix();
	
		glTranslatef(-0.800000, -0.700000, 1.40000);
		
		//base, ottone molto speculare
		setMaterialType(0.41, 0.25, 0.06, 'e');		

		displayPly(meshes[15]);
		glTranslatef(-0.200000 + 0.126522, 0.200000 -0.379565, 1.900000);
		

		// bianca, che emette luce gialla
		setMaterialType(1, 1, 1, 'e');		
		displayPly(meshes[16]);

	glPopMatrix();


}




void drawPrimoPiano(){

	glPushMatrix();


		glTranslatef(-3.0f, 57.399696, 0.300000);
		glTranslatef(-0.900000, 3.199999, 4.499998);
		glTranslatef(0.0f, 0.0f, -0.25);
		//glTranslatef(xPosition, yPosition, zPosition);
		
		
		glPushMatrix();
			glTranslatef(0.100000, 0.100000, 0.8);
			glScalef(0.4f, 0.4f, 0.5f);
			
			//rosso, metallo
			setMaterialType(1, 0, 0, 'e');		

			displayPly(meshes[14]);
		glPopMatrix();

		
		glScalef(0.3f, 0.3f, 0.3f);
		
		//bianco, espositore
		setMaterialType(1, 1, 1, 'e');		

		displayPly(meshes[9]);
		//idrante sul primo espositore strano
		
		
		glTranslatef(-53.399757, -0.900000, 0.000000);
		
		
		
		glPushMatrix();
			glTranslatef(0.100000, 0.100000, 0.8);
			glScalef(0.4f, 0.4f, 0.5f);
			
			//materiali all'interno ->> 
			drawLampada();

		glPopMatrix();
		
		
		setMaterialType(1, 1, 1, 'e');		

		displayPly(meshes[9]);
		//lampada sul secondo espositore strano
		
	
		//divano
		glPushMatrix(); 
			GLfloat scalaDivano = 5.0f * 0.8f;
			glScalef(scalaDivano, scalaDivano, scalaDivano);
			glTranslatef(-0.800000, -1.500000 + -0.632608, 0.700000);
						
			// pelle bordeaux, speculare
			setMaterialType(0.12, 0.01, 0.01, 'e');		
			
			displayPly(meshes[7]); 
		glPopMatrix();
		
		 
		//poltrona cuscino
		glPushMatrix();
			glScalef(2.0f, 2.0f, 2.0f);
			glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(-20.700043, -27.100067, 0.45);
			
			
			//poltrona, bianca
			setMaterialType(1, 1, 1, 'e');		
			
			displayPly(meshes[17]); 
			
			glRotatef(180, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.100000, 5.999997, 1.500000);
			
			//nero, speculare
			setMaterialType(0.1, 0.1, 0.1, 'e');		

			displayPly(meshes[18]);
			//apple
			
		glPopMatrix(); 
		
	glPopMatrix();

}
	
	// trova le dimensioni della scala e del piano
	// 4.0 di altezza del piano + 0.2 di spessore del piano
	// larghezza 2.0 e lunghezza 6.0 del buco per le scale
	
	
	GLfloat lunghezzaScala = 6.0f;
	//il numero di scalini non si cambia
	GLint nScalini = 20;
	GLfloat altezzaPiano = 4.2f;
	GLfloat lunghezzaScalino;
	GLfloat altezzaScalino;
	GLfloat profonditaScalino = 2.0f;
	
//	lunghezzaScalino = lunghezzaScala / nScalini;
//	altezzaScalino = altezzaPiano / nScalini;

	
void drawScalino() {
	
	//setMaterial(0,0,1);

	lunghezzaScalino = lunghezzaScala / nScalini;
	altezzaScalino = (altezzaPiano / nScalini);
	//printf("altezzaScalino: %f; lunghezzaScalino: %f\n", altezzaScalino, lunghezzaScalino);

	// draw scalino parte da quale punto? 
	
	//glutSolidSphere(0.04f, 10.0f, 10.0f);
	
	Point3d point1 = {0,0,0};
	Point3d point2 = {0,lunghezzaScalino,0};
	Point3d point3 = {0,lunghezzaScalino,altezzaScalino};
	Point3d point4 = {0,0,altezzaScalino};
	drawWall(&point1, &point2, &point3, &point4, profonditaScalino, 0.0f, 0.0f, dettaglio);	

}


void drawScala() {


//  materiale delle scale

		setMaterialType(0.2, 0.2, 0.2, 'e');	

	
	glPushMatrix();
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef(0, 0, 7);
		glTranslatef(-50.999794, -9.299999, -3.099999);
		//glTranslatef(xPosition, yPosition, zPosition);
	//glTranslatef(0, -lunghezzaScalino, -altezzaScalino);	
	//drawScalino();
	
	GLint i = 1;
	
	for(i = 1; i <= nScalini; i++) {
		drawScalino();
		glTranslatef(0, -lunghezzaScalino, -altezzaScalino);
	}

	glPopMatrix();
	
}


void drawSecondoPiano(){

		
	drawScala();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 4.099998);
	
	
	
	drawLampadaSecca();
	glPopMatrix();
}
	
void drawTerzoPiano(){

	glPushMatrix();
		glTranslatef(0,0,4.0f);
		//
		altezzaPiano = 4.0f;
		drawScala();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 10.700005);
	drawLampadaSecca();
	glPopMatrix();

}



void drawInterni()
{
	//setMaterial(1,1,1);
	
	
	
	// materiale dei piani 
	setMaterialType(1, 1, 1, 'e');

	drawPlanes(2);
	
	GLfloat raggio = 0.5f;
	GLfloat altezzaColonne = 15.0f;
	
	
	// materiale delle colonne
	setMaterialType(1, 1, 1, 'e');
	drawColumns(raggio, altezzaColonne);
	

	/* disegno le mesh al piano */
	
	drawPianoTerra();
	drawPrimoPiano(); 
	drawSecondoPiano();
	drawTerzoPiano();
	
	
}

