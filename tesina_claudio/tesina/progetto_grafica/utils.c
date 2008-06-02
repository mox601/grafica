#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <GL/glut.h>
#include "light.h"
#include "texture.h"

#ifndef DRAW_AND_NORMAL_DEFINED
#define DRAW_AND_NORMAL DEFINED 0
#define DRAW_CILINDRO 2
#define DRAW_SEMICILINDRO 1
#define NORMAL_OUTSIDE 2
#define NORMAL_INSIDE 1
#define DRAW_SEMITORO 2
#define DRAW_QUARTODITORO 1
#endif

char* STRING_UP = "(up)";
char* STRING_DOWN = "(down)";
char* STRING_12 = "(12)";
char* STRING_23 = "(23)";
char* STRING_34 = "(34)";
char* STRING_41 = "(41)";



static int dbg = 0;
static float textureScaling = 1.0;

/* prototipi */
static void drawCilindroOrSemiCilindro(float radius, float h, int steps, int mode, int normalMode);
static void getPointOnQuarterSphere(float dest[3],float radius,float u,float v);
static void getPointOnSemiTorus(GLfloat point[3], GLfloat radius, GLfloat section, GLfloat param1, GLfloat param2);
static void getPointOnQuarterTorus(GLfloat point[3], GLfloat radius, GLfloat section, GLfloat param1, GLfloat param2);
static void drawSphereQuarter(float radius,int nx,int ny, int normalMode);
static void drawSemiTorusOrQuarter(float radius,float section,int nx,int ny, int mode);
void randomColor(float color[4]);
void crossProd(GLfloat out[3], GLfloat u[3], GLfloat v[3]);
void normalize(GLfloat dest[3]);
void verTex(
	char* name,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3,
	float x4, float y4, float z4
	);
void poliTex(
	char* name,
	float xup1, float yup1, float zup1,
	float xup2, float yup2, float zup2,
	float xup3, float yup3, float zup3,
	float xup4, float yup4, float zup4,
	float xdown1, float ydown1, float zdown1,
	float xdown2, float ydown2, float zdown2,
	float xdown3, float ydown3, float zdown3,
	float xdown4, float ydown4, float zdown4
	);
void drawSuperficieArcata(float radius, int semicircleSteps);
void drawQuadratoBucato(float base, int quarterCircleSteps);
void drawCilindro(float radius, float h, int steps);
void drawSemiCilindroOutside(float radius, float h, int steps);
void drawSemiCilindroInside(float radius, float h, int steps);
void setTextureScaling(float ratio);
void drawMezzoCono(GLfloat raggio, GLfloat altezza, GLfloat spessore, GLint slices);
void getPointOnSemiSphere(float dest[3],float radius,float u,float v);
float pointDistance(float p1[3], float p2[3]);
void drawSemiTorus(float radius,float section,int nx,int ny);
void drawTorusQuarter(float radius,float section,int nx,int ny);
void drawCilindroChiuso(float radius, float h, int steps);
void drawSphereQuarterOutside(float radius,int nx,int ny);
void drawSphereQuarterInside(float radius,int nx,int ny);
void drawCilindroInside(float radius, float h, int steps);
void drawRosone(float radius, int steps);
float random01();
void drawArco(float radius, float width, int semicircleSteps);
void drawSemisfera(float radius, int steps);





/*----------------------------------------------------*/
/*----------------------------------------------------*/





/*
Disegna un cilindro o un semicilindro (a seconda del valore dell'argomento mode),
il cui raggio è pari a radius, la cui altezza pari ad h, e la precisione dettata da steps.
estensione cilindro (X,Y,Z): 		[-radius,radius] * [0,h] * [-radius,radius]
estensione semicilindro (X,Y,Z): 	[-radius,radius] * [0,h] * [0,radius]
*/
static void drawCilindroOrSemiCilindro(float radius, float h, int steps, int mode, int normalMode) {
	
	int repeats = 0;
	float tempScaling;
	float step;
	float chord;
	int i;
	float angle0, angle1, anglem;
	float normal[3];
	float vect1[3],vect2[3];
	float perimetro;

	//FONDAMENTALE:
	// se mode=DRAW_CILINDRO=2, lo step si calcola su 360 gradi
	// se mode=DRAW_SEMICILIDRO=1, solo su 180 gradi!!
	step=(float)M_PI*mode/(float)steps;

	chord = sin(step/2.0) * 2.0*radius;

	//printf("chord = %f\n",chord);

	tempScaling = textureScaling;
	perimetro = chord * (float)steps;
	//printf("perimetro = %f\n",perimetro);
	//approssimo il texture scaling al valore intero più appropriato
	repeats = (int)(perimetro / textureScaling);
	//printf("repeats = %d\n",repeats);
	if(repeats != 0)
		tempScaling = (float)perimetro / (int)repeats;
	//printf("tempScaling = %f\n",tempScaling);

	//glPolygonMode(GL_FRONT, GL_FILL);
	
	//disegno il cilindro o semicilindro

	for(i=0;i<steps;i++) {
		angle0=(float)i*step;
		angle1=(float)(i+1)*step;
		anglem=(angle0+angle1)/2;

		if(normalMode == NORMAL_OUTSIDE) {

			glBegin(GL_QUADS);

			vect1[0] = radius*cos(angle0) - radius*cos(angle1);
			vect1[1] = 0.0;
			vect1[2] = radius*sin(angle0) - radius*sin(angle1);
			vect2[0] = radius*cos(angle0) - radius*cos(angle1);
			vect2[1] = h;
			vect2[2] = radius*sin(angle0) - radius*sin(angle1);
			crossProd(normal,vect1,vect2);
			glNormal3fv(normal);

			glTexCoord2f(((float)i+1.0)*chord / tempScaling, 0); 
			glVertex3f(radius*cos(angle1), 0, radius*sin(angle1));

			//printf("%f %f %f %f\n",(float)i+1.0,((float)i+1.0),((float)i+1.0)*chord,((float)i+1.0)*chord / tempScaling);
			//printf("texcoord1 = (%f,%f)\n",((float)i+1.0)*chord / tempScaling, 0);

			glTexCoord2f((float)i*chord / tempScaling, 0); 
			glVertex3f(radius*cos(angle0), 0, radius*sin(angle0));

			//printf("texcoord2 = (%f,%f)\n",(float)i*chord / tempScaling, 0);

			glTexCoord2f((float)i*chord / tempScaling, h / tempScaling); 
			glVertex3f(radius*cos(angle0), h, radius*sin(angle0));

			//printf("texcoord3 = (%f,%f)\n",(float)i*chord / tempScaling, h / tempScaling);

			glTexCoord2f(((float)i+1.0)*chord / tempScaling, h / tempScaling); 	
			glVertex3f(radius*cos(angle1), h, radius*sin(angle1));

			//printf("texcoord4 = (%f,%f)\n",((float)i+1.0)*chord / tempScaling, h / tempScaling);
		
			glEnd();

		}


		else if(normalMode == NORMAL_INSIDE) {

			glBegin(GL_QUADS);

			vect1[0] = radius*cos(angle1) - radius*cos(angle0);
			vect1[1] = 0.0;
			vect1[2] = radius*sin(angle1) - radius*sin(angle0);
			vect2[0] = radius*cos(angle1) - radius*cos(angle0);
			vect2[1] = h;
			vect2[2] = radius*sin(angle1) - radius*sin(angle0);
			crossProd(normal,vect1,vect2);
			glNormal3fv(normal);

			glTexCoord2f((float)i*chord / tempScaling, 0); 
			glVertex3f(radius*cos(angle0), 0, radius*sin(angle0));

			glTexCoord2f(((float)i+1.0)*chord / tempScaling, 0); 
			glVertex3f(radius*cos(angle1), 0, radius*sin(angle1));

			glTexCoord2f(((float)i+1.0)*chord / tempScaling, h / tempScaling); 	
			glVertex3f(radius*cos(angle1), h, radius*sin(angle1));

			glTexCoord2f((float)i*chord / tempScaling, h / tempScaling); 
			glVertex3f(radius*cos(angle0), h, radius*sin(angle0));

			glEnd();

		}
	

/*
		if(normalMode == NORMAL_OUTSIDE) {
			normal[0] = cos(anglem);	normal[1] = 0.0;	normal[2] = sin(anglem);
			glNormal3f(cos(anglem),0,sin(anglem));
		}
		else if(normalMode == NORMAL_INSIDE) {
			normal[0] = -cos(anglem);	normal[1] = 0.0;	normal[2] = -sin(anglem);
			glNormal3f(-cos(anglem),0,-sin(anglem));
		}

		//printf("anglem = %f\n",anglem*360.0/(2*M_PI));

		//glNormal3fv(normal);
		glTexCoord2f(i*chord / tempScaling, 0); 			
		glVertex3f(radius*cos(angle0), 0, radius*sin(angle0));

		//glNormal3fv(normal);
		glTexCoord2f((i+1)*chord / tempScaling, 0); 			
		glVertex3f(radius*cos(angle1), 0, radius*sin(angle1));

		//glNormal3fv(normal);
		glTexCoord2f((i+1)*chord / tempScaling, h / tempScaling); 	
		glVertex3f(radius*cos(angle1), h, radius*sin(angle1));

		//glNormal3fv(normal);
		glTexCoord2f(i*chord / tempScaling, h / tempScaling); 		
		glVertex3f(radius*cos(angle0), h, radius*sin(angle0));
*/

	}


}



/*----------------------------------------------------*/



/*
Determina il punto sulla superficie di un quarto di sfera determinato dai due
valori di u e v (a condizione che siano compresi tra 0 e 1!) e il raggio radius,
memorizzando il risultato in dest.
*/
static void getPointOnQuarterSphere(float dest[3],float radius,float u,float v)
{
	u*=M_PI;
	v*=(M_PI / 2);
	dest[0]=radius*cos(u)*cos(v);
	dest[1]=radius*sin(u)*cos(v);
	dest[2]=radius*sin(v);
}



/*----------------------------------------------------*/



/*
Determina il punto sulla superficie di una semisfera determinata dai due
valori di u e v (a condizione che siano compresi tra 0 e 1!) e il raggio radius,
memorizzando il risultato in dest.
*/
void getPointOnSemiSphere(float dest[3],float radius,float u,float v)
{
	u*=M_PI;
	v*=(M_PI);
	v-=M_PI/2;
	dest[0]=radius*cos(u)*cos(v);
	dest[1]=radius*sin(u)*cos(v);
	dest[2]=radius*sin(v);
}



/*----------------------------------------------------*/



/*
Determina il punto sulla superficie di una sfera determinata dai due
valori di u e v (a condizione che siano compresi tra 0 e 1!) e il raggio radius,
memorizzando il risultato in dest.
*/
void getPointOnSphere(float dest[3],float radius,float u,float v)
{
	u*=2*M_PI;
	v=-1*(v-0.5)*M_PI;
	dest[0]=radius*cos(u)*cos(v);
	dest[1]=radius*sin(u)*cos(v);
	dest[2]=radius*sin(v);
}

/*----------------------------------------------------*/
/*----------------------------------------------------*/





/*
Calcola il prodotto vettoriale tra u e v, memorizzandolo in out.
*/
void crossProd(GLfloat out[3], GLfloat u[3], GLfloat v[3]) {

	int i;
   
	out[0] = u[1]*v[2] - u[2]*v[1];
	out[1] = u[2]*v[0] - u[0]*v[2];
	out[2] = u[0]*v[1] - u[1]*v[0];

	for(i=0; i<3; i++)
		if(out[i]==-0.0) out[i] = 0.0;

	normalize(out);

}



/*----------------------------------------------------*/



/*
Normalizza il vettore passato come parametro portandone il modulo a 1.
*/
void normalize(GLfloat dest[3]) {
	
	/* scaling factor (original vector length) */
	GLfloat l = sqrt(dest[0]*dest[0] + dest[1]*dest[1] + dest[2]*dest[2]);
	dest[0] /= l;
	dest[1] /= l;
	dest[2] /= l;
	
}



/*----------------------------------------------------*/



/*
Disegna un quadrilatero specificando le texCoords, dati i suoi 4 vertici;
è limitato ad alcune tipologie semplici di quadrilateri, e segue l'ordine
dei vertici in input per la generazione del poligono.
Funziona anche con i triangoli (così sembra!), specificando due vertici uguali.
*/
void verTex(
	char* name,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3,
	float x4, float y4, float z4
	) {

	float tx1, ty1, tx2, ty2, tx3, ty3, tx4, ty4;
	GLfloat normal[3], vect1[3], vect2[3];
	double tempSqrt;

	if(dbg)	printf("%s\n", name);

	//x non varia, si usa la z per la x della texture
	if(x1 == x2 && x2 == x3 && x3 == x4) {

		if(dbg) printf("x non varia.\n");

		tx1 = z1 / textureScaling;
		ty1 = y1 / textureScaling;
		tx2 = z2 / textureScaling;
		ty2 = y2 / textureScaling;
		tx3 = z3 / textureScaling;
		ty3 = y3 / textureScaling;
		tx4 = z4 / textureScaling;
		ty4 = y4 / textureScaling;
	}

	//y non varia, si usa la z per la y della texture
	else if(y1 == y2 && y2 == y3 && y3 == y4) {

		if(dbg) printf("y non varia.\n");

		tx1 = x1 / textureScaling;
		ty1 = z1 / textureScaling;
		tx2 = x2 / textureScaling;
		ty2 = z2 / textureScaling;
		tx3 = x3 / textureScaling;
		ty3 = z3 / textureScaling;
		tx4 = x4 / textureScaling;
		ty4 = z4 / textureScaling;
	}

	//z non varia
	else if(z1 == z2 && z2 == z3 && z3 == z4) {

		if(dbg) printf("z non varia.\n");

		tx1 = x1 / textureScaling;
		ty1 = y1 / textureScaling;
		tx2 = x2 / textureScaling;
		ty2 = y2 / textureScaling;
		tx3 = x3 / textureScaling;
		ty3 = y3 / textureScaling;
		tx4 = x4 / textureScaling;
		ty4 = y4 / textureScaling;
	}

	//piano inclinato solo rispetto all'asse x (posizionato male)
	else if(y2==y3 && z2==z3 && y4==y1 && z4==z1) {

		if(dbg) printf("piano inclinato risp. asse x (male)...\n");

		verTex(
			name,
			x2, y2, z2,
			x3, y3, z3,
			x4, y4, z4,
			x1, y1, z1
		);

		return;

	}

	//piano inclinato solo rispetto all'asse x
	else if(y1==y2 && z1==z2 && y3==y4 && z3==z4) {

		if(dbg) printf("piano inclinato risp. asse x.\n");

		tempSqrt = sqrt((y1-y3)*(y1-y3)+(z1-z3)*(z1-z3));

		tx1 = x1 / textureScaling;
		ty1 = y1 / textureScaling;
		tx2 = x2 / textureScaling;
		ty2 = y2 / textureScaling;
		tx3 = x3 / textureScaling;
		ty3 = (y1 + tempSqrt) / textureScaling;
		tx4 = x4 / textureScaling;
		ty4 = (y2 + tempSqrt) / textureScaling;
	}

	//piano inclinato solo rispetto all'asse y (posizionato male)
	else if(x2==x3 && z2==z3 && x4==x1 && z4==z1) {

		if(dbg) printf("piano inclinato risp. asse y (male)...\n");

		verTex(
			name,
			x2, y2, z2,
			x3, y3, z3,
			x4, y4, z4,
			x1, y1, z1
		);

		return;

	}

	//piano inclinato solo rispetto all'asse y
	else if(x1==x2 && z1==z2 && x3==x4 && z3==z4) {

		if(dbg) printf("piano inclinato risp. asse y.\n");

		tempSqrt = sqrt((x1-x3)*(x1-x3)+(z1-z3)*(z1-z3));

		tx1 = y1 / textureScaling;
		ty1 = x1 / textureScaling;
		tx2 = y2 / textureScaling;
		ty2 = x2 / textureScaling;
		tx3 = y3 / textureScaling;
		ty3 = (x1 + tempSqrt) / textureScaling;
		tx4 = y4 / textureScaling;
		ty4 = (x2 + tempSqrt) / textureScaling;

	}

	//piano inclinato solo rispetto all'asse z (posizionato male)
	else if(x2==x3 && y2==y3 && x4==x1 && y4==y1) {

		if(dbg) printf("piano inclinato risp. asse z (male)...\n");

		verTex(
			name,
			x2, y2, z2,
			x3, y3, z3,
			x4, y4, z4,
			x1, y1, z1
		);

		return;

	}

	//piano inclinato solo rispetto all'asse z
	else if(x1==x2 && y1==y2 && x3==x4 && y3==y4) {

		if(dbg) printf("piano inclinato risp. asse z.\n");

		tempSqrt = sqrt((x1-x3)*(x1-x3)+(y1-y3)*(y1-y3));

		tx1 = z1 / textureScaling;
		ty1 = y1 / textureScaling;
		tx2 = z2 / textureScaling;
		ty2 = y2 / textureScaling;
		tx3 = z3 / textureScaling;
		ty3 = (y1 + tempSqrt) / textureScaling;
		tx4 = z4 / textureScaling;
		ty4 = (y2 + tempSqrt) / textureScaling;

	}



	//calcola normale

	//se i primi due punti coincidono
	if(x1==x2 && y1==y2 && z1==z2) {
		vect1[0] = x3-x2;
		vect1[1] = y3-y2;
		vect1[2] = z3-z2;
		vect2[0] = x4-x2;
		vect2[1] = y4-y2;
		vect2[2] = z4-z2;
	}

	//altrimenti se coincidono secondo e terzo punto
	else if(x1==x2 && y1==y2 && z1==z2) {
		vect1[0] = x3-x1;
		vect1[1] = y3-y1;
		vect1[2] = z3-z1;
		vect2[0] = x4-x1;
		vect2[1] = y4-y1;
		vect2[2] = z4-z1;
	}

	else {
		vect1[0] = x2-x1;
		vect1[1] = y2-y1;
		vect1[2] = z2-z1;
		vect2[0] = x3-x1;
		vect2[1] = y3-y1;
		vect2[2] = z3-z1;
	}

	crossProd(normal,vect1,vect2);

	if(dbg) printf("normale:\n(%f,%f,%f)\n",
		normal[0],normal[1],normal[2]
	);

	if(dbg) printf("vertici:\n(%f,%f,%f)\n(%f,%f,%f)\n(%f,%f,%f)\n(%f,%f,%f)\n",
		x1, y1, z1,
		x2, y2, z2,
		x3, y3, z3,
		x4, y4, z4
	);

	if(dbg) printf("texcoord:\n(%f,%f)\n(%f,%f)\n(%f,%f)\n(%f,%f)\n",
		tx1, ty1, tx2, ty2, tx3, ty3, tx4, ty4
	);

	if(dbg) printf("\n\n");

	//specifica normale, texture, vertici
	glNormal3f(normal[0],normal[1],normal[2]);
	glTexCoord2f(tx1, ty1); glVertex3f(x1,y1,z1);
	glTexCoord2f(tx2, ty2); glVertex3f(x2,y2,z2);
	glTexCoord2f(tx3, ty3); glVertex3f(x3,y3,z3);
	glTexCoord2f(tx4, ty4); glVertex3f(x4,y4,z4);

	return;

}



/*----------------------------------------------------*/



/*
Disegna un poliedro specificando le texCoords, dati i vertici delle facce superiore e inferiore
(che devono essere necessariamente rettangoli! oppure triangoli, facendo coincidere due vertici);
gli spigoli disegnati saranno (up1,down1) (up2,down2) (up3,down3) (up4,down4), quindi bisogna 
specificare la faccia inferiore con orientamento OPPOSTO (la funzione correggerà automaticamente).
Valgono le stesse semplificazioni della funzione verTex.
*/
void poliTex(
	char* name,
	float xup1, float yup1, float zup1,
	float xup2, float yup2, float zup2,
	float xup3, float yup3, float zup3,
	float xup4, float yup4, float zup4,
	float xdown1, float ydown1, float zdown1,
	float xdown2, float ydown2, float zdown2,
	float xdown3, float ydown3, float zdown3,
	float xdown4, float ydown4, float zdown4
	) {

	int isDrawable = 0;

	//faccia superiore e faccia inferiore su piani perpendicolari all'asse x
	if(xup1 == xup2 && xup2 == xup3 && xup3 == xup4 &&
	   xdown1 == xdown2 && xdown2 == xdown3 && xdown3 == xdown4 )

		//rettangoli "semplici"
		if(	(yup1==yup2 && yup3==yup4 && ydown1==ydown2 && ydown3==ydown4) ||
			(zup1==zup2 && zup3==zup4 && zdown1==zdown2 && zdown3==zdown4) ) 

			isDrawable = 1;

	//faccia superiore e faccia inferiore su piani perpendicolari all'asse y
	if(yup1 == yup2 && yup2 == yup3 && yup3 == yup4 &&
	   ydown1 == ydown2 && ydown2 == ydown3 && ydown3 == ydown4 )


		//rettangoli "semplici"
		if(	(xup1==xup2 && xup3==xup4 && xdown1==xdown2 && xdown3==xdown4) ||
			(zup1==zup2 && zup3==zup4 && zdown1==zdown2 && zdown3==zdown4) ) 

			isDrawable = 1;


	//faccia superiore e faccia inferiore su piani perpendicolari all'asse z
	if(zup1 == zup2 && zup2 == zup3 && zup3 == zup4 &&
	   zdown1 == zdown2 && zdown2 == zdown3 && zdown3 == zdown4 ) 

		//rettangoli "semplici"
		if(	(xup1==xup2 && xup3==xup4 && xdown1==xdown2 && xdown3==xdown4) ||
			(yup1==yup2 && yup3==yup4 && ydown1==ydown2 && ydown3==ydown4) ) 

			isDrawable = 1;




	if(isDrawable) {

		//face up
		verTex(
			STRING_UP,
			xup1, yup1, zup1,
			xup2, yup2, zup2,
			xup3, yup3, zup3,
			xup4, yup4, zup4
		);

		//face down
		verTex(
			STRING_DOWN,
			xdown2, ydown2, zdown2,
			xdown1, ydown1, zdown1,
			xdown4, ydown4, zdown4,
			xdown3, ydown3, zdown3
		);

		//face (down1,down2,up2,up1)
		verTex(
			STRING_12,
			xdown1, ydown1, zdown1,
			xdown2, ydown2, zdown2,
			xup2, yup2, zup2,
			xup1, yup1, zup1
		);

		//face (down2,down3,up3,up2)
		verTex(
			STRING_23,
			xdown2, ydown2, zdown2,
			xdown3, ydown3, zdown3,
			xup3, yup3, zup3,
			xup2, yup2, zup2
		);

		//face (down3,down4,up4,up3)
		verTex(
			STRING_34,
			xdown3, ydown3, zdown3,
			xdown4, ydown4, zdown4,
			xup4, yup4, zup4,
			xup3, yup3, zup3
		);

		//face (down4,down1,up1,up4)
		verTex(
			STRING_41,
			xdown4, ydown4, zdown4,
			xdown1, ydown1, zdown1,
			xup1, yup1, zup1,
			xup4, yup4, zup4
		);

	}

	return;

}



/*----------------------------------------------------*/



/*
Disegna la superficie laterale di un'arcata.
estensione arcata (X,Y): 	[0,2*radius] * [0,radius]
*/
void drawSuperficieArcata(float radius, int quarterCircleSteps) {

	float alpha;
	float step = (M_PI/2) / quarterCircleSteps;
	int cycle;

	//normale
	glNormal3f(0.0,0.0,1.0);

	//triangle fan in alto a sinistra
	glBegin(GL_TRIANGLE_FAN);

		glTexCoord2f(0, radius / textureScaling); glVertex3f(0,radius,0);

		for(cycle=0; cycle<=quarterCircleSteps; cycle++) {
			alpha = M_PI - cycle*step;
			glTexCoord2f(radius*(cos(alpha)+1) / textureScaling, radius*sin(alpha) / textureScaling); 
			glVertex3f(radius*(cos(alpha)+1),radius*sin(alpha),0);
		}

	glEnd();

	//triangle fan in alto a destra
	glBegin(GL_TRIANGLE_FAN);

		glTexCoord2f(2*radius / textureScaling, radius / textureScaling); glVertex3f(2*radius,radius,0);

		for(cycle=0; cycle<=quarterCircleSteps; cycle++) {
			alpha = (M_PI/2) - cycle*step;
			glTexCoord2f(radius*(cos(alpha)+1) / textureScaling, radius*sin(alpha) / textureScaling); 
			glVertex3f(radius*(cos(alpha)+1),radius*sin(alpha),0);
		}

	glEnd();

	return;
	
}



/*----------------------------------------------------*/



/*
Disegna la differenza tra una superficie quadrata e una circonferenza.
estensione quadrato bucato (X,Y): 	[0,base]*[0,base]
*/
void drawQuadratoBucato(float base, int quarterCircleSteps) {

	glDisable(GL_CULL_FACE);

	glPushMatrix();
		glTranslatef(0.0,base/2,0.0);
		drawSuperficieArcata(base/2,quarterCircleSteps);
		glScalef(1.0,-1.0,1.0);
		drawSuperficieArcata(base/2,quarterCircleSteps);
	glPopMatrix();

	glEnable(GL_CULL_FACE);


}



/*----------------------------------------------------*/



/*
Disegna un cilindro il cui raggio è pari a radius, la cui altezza pari ad h, e la precisione dettata da steps.
estensione cilindro (X,Y,Z): 		[-radius,radius] * [0,h] * [-radius,radius]
*/
void drawCilindro(float radius, float h, int steps) { 
		
	drawCilindroOrSemiCilindro(radius,h,steps,DRAW_CILINDRO,NORMAL_OUTSIDE);
}




/*----------------------------------------------------*/




/*
Disegna un cilindro il cui raggio è pari a radius, la cui altezza pari ad h, e la precisione dettata da steps.
Il cilindro è illuminato all'interno!
estensione cilindro (X,Y,Z): 		[-radius,radius] * [0,h] * [-radius,radius]
*/
void drawCilindroInside(float radius, float h, int steps) { 
		
	drawCilindroOrSemiCilindro(radius,h,steps,DRAW_CILINDRO,NORMAL_INSIDE);
}
/*----------------------------------------------------*/



/*
Disegna un semicilindro il cui raggio è pari a radius, la cui altezza pari ad h, e la precisione dettata da steps.
il semicindro ha le normali rivolte verso l'esterno.
estensione semicilindro (X,Y,Z): 	[-radius,radius] * [0,h] * [0,radius]
*/
void drawSemiCilindroOutside(float radius, float h, int steps) { 
		
	drawCilindroOrSemiCilindro(radius,h,steps,DRAW_SEMICILINDRO,NORMAL_OUTSIDE);
}




/*----------------------------------------------------*/



/*
Disegna un semicilindro il cui raggio è pari a radius, la cui altezza pari ad h, e la precisione dettata da steps.
il semicindro ha le normali rivolte verso l'interno.
estensione semicilindro (X,Y,Z): 	[-radius,radius] * [0,h] * [0,radius]
*/
void drawSemiCilindroInside(float radius, float h, int steps) { 
		
	drawCilindroOrSemiCilindro(radius,h,steps,DRAW_SEMICILINDRO,NORMAL_INSIDE);
}


/*----------------------------------------------------*/




/*
Disegna una sfera di raggio pari a radius e precisione dettata da nx e ny.
estensione sfera (X,Y,Z):		[-radius,+radius] * [-radius,+radius] * [-radius,+radius]
*/
void drawSphere(float radius,int nx,int ny) {

	int i,j;
	GLfloat p0[3],p1[3],p2[3],p3[3];
	GLfloat u0,u1,v0,v1;
	GLfloat stepx=1/(float)nx;
	GLfloat stepy=1/(float)ny;

	glDisable(GL_CULL_FACE);
	glBegin(GL_TRIANGLES);
	
	for (j=0;j<ny;j++)
	{
		for (i=0;i<nx;i++)
		{
			u0=i*stepx;u1=(i+1)*stepx;
			v0=j*stepy;v1=(j+1)*stepy;
			
			getPointOnSphere(p0,radius,u0,v0);
			getPointOnSphere(p1,radius,u1,v0);
			getPointOnSphere(p2,radius,u1,v1);
			getPointOnSphere(p3,radius,u0,v1);
			
			glTexCoord2f(u0,v0);glNormal3fv(p0);glVertex3fv(p0);
			glTexCoord2f(u1,v0);glNormal3fv(p1);glVertex3fv(p1);
			glTexCoord2f(u1,v1);glNormal3fv(p2);glVertex3fv(p2);
			
			glTexCoord2f(u0,v0);glNormal3fv(p0);glVertex3fv(p0);
			glTexCoord2f(u1,v1);glNormal3fv(p2);glVertex3fv(p2);
			glTexCoord2f(u0,v1);glNormal3fv(p3);glVertex3fv(p3);
		}
	}
		
	glEnd();
	glEnable(GL_CULL_FACE);
}


/*
Disegna un quarto di sfera di raggio pari a radius e precisione dettata da nx e ny.
Il quarto di sfera ha le normali di faccia rivolte verso l'esterno.
estensione quarto di sfera (X,Y,Z):		[-radius,+radius] * [0,+radius] * [0,+radius]
*/
void drawSphereQuarterOutside(float radius,int nx,int ny) {
	drawSphereQuarter(radius, nx, ny, NORMAL_OUTSIDE);
}


/*
Disegna un quarto di sfera di raggio pari a radius e precisione dettata da nx e ny.
Il quarto di sfera ha le normali di faccia rivolte verso l'interno.
estensione quarto di sfera (X,Y,Z):		[-radius,+radius] * [0,+radius] * [0,+radius]
*/
void drawSphereQuarterInside(float radius,int nx,int ny) {
	drawSphereQuarter(radius, nx, ny, NORMAL_INSIDE);
}


/*
Disegna un quarto di sfera di raggio pari a radius e precisione dettata da nx e ny.
estensione quarto di sfera (X,Y,Z):		[-radius,+radius] * [0,+radius] * [0,+radius]
*/
static void drawSphereQuarter(float radius,int nx,int ny, int normalMode)
{
	int i,j;
	GLfloat p0[3],p1[3],p2[3],p3[3];
	GLfloat u0,u1,v0,v1;
	GLfloat currentNormal[3], vect1[3], vect2[3];
	GLfloat stepx=1/(float)nx;
	GLfloat stepy=1/(float)ny;

	glDisable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_TRIANGLES);
	
	for (j=0;j<ny;j++)
	{


		for (i=0;i<nx;i++)
		{
			u0=i*stepx;u1=(i+1)*stepx;
			v0=j*stepy;v1=(j+1)*stepy;
			
			getPointOnQuarterSphere(p0,radius,u0,v0);
			getPointOnQuarterSphere(p1,radius,u1,v0);
			getPointOnQuarterSphere(p2,radius,u1,v1);
			getPointOnQuarterSphere(p3,radius,u0,v1);

	
			//u0 /= textureScaling;
			//v0 /= textureScaling;

			//SPHERE MAPPING EXPERIMENT!!!

			u0 -= 0.5;
			u0 /= ((cos((1-v0)*M_PI/2)+1.5)/2.5);
			//if(u0 < (-0.5)) u0 = -0.5;
			//if(u0 > (0.5))  u0 =  0.5;
			u0 += 0.5;
			u1 -= 0.5;
			u1 /= ((cos((1-v1)*M_PI/2)+1.5)/2.5);
			//if(u1 < (-0.5)) u1 = -0.5;
			//if(u1 > (0.5))  u1 =  0.5;
			u1 += 0.5;


			vect1[0] = p1[0] - p0[0];	vect1[1] = p1[1] - p0[1];	vect1[2] = p1[2] - p0[2];
			vect2[0] = p2[0] - p0[0];	vect2[1] = p2[1] - p0[1];	vect2[2] = p2[2] - p0[2];
			if(normalMode == NORMAL_OUTSIDE)
				crossProd(currentNormal,vect1,vect2);
			else if(normalMode == NORMAL_INSIDE)
				crossProd(currentNormal,vect2,vect1);
			glNormal3fv(currentNormal);

			glTexCoord2f(u0,v0);glVertex3fv(p0);
			glTexCoord2f(u1,v0);glVertex3fv(p1);
			glTexCoord2f(u1,v1);glVertex3fv(p2);
	
			//printf("vect1:(%f,%f,%f), vect2:(%f,%f,%f)\n",vect1[0],vect1[1],vect1[2],vect2[0],vect2[1],vect2[2]);
			//printf("normal 1:(%f,%f,%f)\n",currentNormal[0],currentNormal[1],currentNormal[2]);
			

			vect1[0] = p2[0] - p0[0];	vect1[1] = p2[1] - p0[1];	vect1[2] = p2[2] - p0[2];
			vect2[0] = p3[0] - p0[0];	vect2[1] = p3[1] - p0[1];	vect2[2] = p3[2] - p0[2];
			if(normalMode == NORMAL_OUTSIDE)
				crossProd(currentNormal,vect1,vect2);
			else if(normalMode == NORMAL_INSIDE)
				crossProd(currentNormal,vect2,vect1);
			glNormal3fv(currentNormal);

			glTexCoord2f(u0,v0);glVertex3fv(p0);
			glTexCoord2f(u1,v1);glVertex3fv(p2);
			glTexCoord2f(u0,v1);glVertex3fv(p3);

			//printf("vect1:(%f,%f,%f), vect2:(%f,%f,%f)\n",vect1[0],vect1[1],vect1[2],vect2[0],vect2[1],vect2[2]);
			//printf("normal 2:(%f,%f,%f)\n\n",currentNormal[0],currentNormal[1],currentNormal[2]);
		
		}
	}
		
	glEnd();
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}



/*----------------------------------------------------*/



/*
Modifica il rapporto per determinare il texture mapping.
*/
void setTextureScaling(float ratio) {
	if(ratio!=0)
		textureScaling = ratio;
	return;
}



/*----------------------------------------------------*/



//i parametri dicono tutto.. la base è centrata nell'origine
/*
Disegna un mezzo cono di raggio pari a raggio, altezza pari ad altezza (ma va'??) e precisione dettata da slices.
estensione (X,Y,Z):		[-raggio,+raggio] * [0,+altezza] * [0,+raggio]
*/
void drawMezzoCono(GLfloat raggio, GLfloat altezza, GLfloat spessore, GLint slices) {

	
	int i;
	GLfloat angle1,angle2,anglem,angle3;
	GLfloat step = M_PI / slices;
	glPolygonMode(GL_FRONT,GL_FILL);

	angle3 = asin( raggio/( sqrt( (raggio*raggio) + (altezza*altezza) )));

	for(i=0;i<slices;i++) 
	{
		angle1= i* step;
		angle2=(i+1) * step;
		anglem= (angle1 + angle2) / 2.0; //angolo al centro della faccia
		
		glBegin(GL_TRIANGLES);

			verTex(
				"tri",
				raggio*cos(angle1) , 0 , raggio*sin(angle1),
				0,altezza,0,
				raggio*cos(angle2) , 0 , raggio*sin(angle2),
				 
				raggio*cos(angle2) , 0 , raggio*sin(angle2));

		/*glNormal3f( 
			cos(anglem)*cos(angle3),
			sin(angle3),
			sin(anglem)*cos(angle3)
		); 
		
		glVertex3f( raggio*cos(angle1) , 0 , raggio*sin(angle1) );
		glVertex3f( 0,altezza,0 );
		glVertex3f( raggio*cos(angle2) , 0 , raggio*sin(angle2) );
		*/
		glEnd();		

		glBegin(GL_QUADS);

			verTex(
				"quad",
				(raggio-spessore)*cos(angle1) , 0 , (raggio-spessore)*sin(angle1),
				raggio*cos(angle1) , 0 , raggio*sin(angle1),
				raggio*cos(angle2) , 0 , raggio*sin(angle2),
				(raggio-spessore)*cos(angle2) , 0 , (raggio-spessore)*sin(angle2)
			);
			
		glEnd();		
		
	}
}





float pointDistance(float p1[3], float p2[3]) {

	return sqrt(
		(p1[0]-p2[0])*(p1[0]-p2[0]) +
		(p1[1]-p2[1])*(p1[1]-p2[1]) +
		(p1[2]-p2[2])*(p1[2]-p2[2]) 
	);
	
}



/*
Determina il punto sulla superficie di un semi toro determinato dai due
valori di u e v (a condizione che siano compresi tra 0 e 1!), il raggio radius e la sezione section,
memorizzando il risultato in dest.
*/
static void getPointOnSemiTorus(GLfloat point[3], GLfloat radius, GLfloat section, GLfloat param1, GLfloat param2) {

	//const GLfloat radius =1.0f ;
	//const GLfloat section=0.2f;

	GLfloat u=(M_PI)*param1;
	GLfloat v=(2.0f*M_PI)*param2;

	point[0]=(GLfloat)((radius+section*cos(v))*cos(u));
	point[1]=(GLfloat)((radius+section*cos(v))*sin(u));
	point[2]=(GLfloat)((section*sin(v)));


}


/*
Determina il punto sulla superficie di un quarto di toro determinato dai due
valori di u e v (a condizione che siano compresi tra 0 e 1!), il raggio radius e la sezione section,
memorizzando il risultato in dest.
*/
static void getPointOnQuarterTorus(GLfloat point[3], GLfloat radius, GLfloat section, GLfloat param1, GLfloat param2) {

	//const GLfloat radius =1.0f ;
	//const GLfloat section=0.2f;

	GLfloat u=(M_PI/2)*(param1+1);
	GLfloat v=(2.0f*M_PI)*param2;

	point[0]=(GLfloat)((radius+section*cos(v))*cos(u));
	point[1]=(GLfloat)((radius+section*cos(v))*sin(u));
	point[2]=(GLfloat)((section*sin(v)));


}

/*
Disegna un quarto di toro di raggio pari a radius, raggio della sezione pari a section e precisione dettata dai params.
(in pratica il pezzo superiore a sinistra!)
estensione (X,Y,Z):		[-radius-section,0] * [0,+altezza+section] * [-section,+section]
*/
void drawTorusQuarter(float radius,float section,int nx,int ny) {
	drawSemiTorusOrQuarter(radius, section, nx, ny, DRAW_QUARTODITORO);
}

/*
Disegna un mezzo toro di raggio pari a radius, raggio della sezione pari a section e precisione dettata dai params.
(in pratica il semicerchio superiore!)
estensione (X,Y,Z):		[-radius-section,+raggio+section] * [0,+altezza+section] * [-section,+section]
*/
void drawSemiTorus(float radius,float section,int nx,int ny) {
	drawSemiTorusOrQuarter(radius, section, nx, ny, DRAW_SEMITORO);
}


static void drawSemiTorusOrQuarter(float radius,float section,int nx,int ny, int mode)
{
	int i,j;
	GLfloat p0[3],p1[3],p2[3],p3[3];
	GLfloat u0,u1,v0,v1;
	GLfloat currentNormal[3], vect1[3], vect2[3];
	GLfloat stepx=1/(float)nx;
	GLfloat stepy=1/(float)ny;
	//printf("radius=%f, section=%f\n",radius,section);

	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_TRIANGLES);
	
	for (j=0;j<ny;j++)
	{


		for (i=0;i<nx;i++)
		{
			u0=i*stepx;u1=(i+1)*stepx;
			v0=j*stepy;v1=(j+1)*stepy;
			
			if(mode == DRAW_SEMITORO) {
				getPointOnSemiTorus(p0,radius,section,u0,v0);
				getPointOnSemiTorus(p1,radius,section,u1,v0);
				getPointOnSemiTorus(p2,radius,section,u1,v1);
				getPointOnSemiTorus(p3,radius,section,u0,v1);
			}

			else if(mode == DRAW_QUARTODITORO) {
				getPointOnQuarterTorus(p0,radius,section,u0,v0);
				getPointOnQuarterTorus(p1,radius,section,u1,v0);
				getPointOnQuarterTorus(p2,radius,section,u1,v1);
				getPointOnQuarterTorus(p3,radius,section,u0,v1);
			}

//			printf("points: \n(%f,%f,%f)\n(%f,%f,%f)\n(%f,%f,%f)\n(%f,%f,%f)\n",
//				p0[0], p0[1], p0[2],
//				p1[0], p1[1], p1[2],
//				p2[0], p2[1], p2[2],
//				p3[0], p3[1], p3[2]
//			);

			vect1[0] = p1[0] - p0[0];	vect1[1] = p1[1] - p0[1];	vect1[2] = p1[2] - p0[2];
			vect2[0] = p2[0] - p0[0];	vect2[1] = p2[1] - p0[1];	vect2[2] = p2[2] - p0[2];
			crossProd(currentNormal,vect1,vect2);
			glNormal3fv(currentNormal);
			glTexCoord2f(u0,v0);glVertex3fv(p0);
			glTexCoord2f(u1,v0);glVertex3fv(p1);
			glTexCoord2f(u1,v1);glVertex3fv(p2);
	
//			printf("vect1:(%f,%f,%f), vect2:(%f,%f,%f)\n",vect1[0],vect1[1],vect1[2],vect2[0],vect2[1],vect2[2]);
//			printf("normal 1:(%f,%f,%f)\n",currentNormal[0],currentNormal[1],currentNormal[2]);
			

			vect1[0] = p2[0] - p0[0];	vect1[1] = p2[1] - p0[1];	vect1[2] = p2[2] - p0[2];
			vect2[0] = p3[0] - p0[0];	vect2[1] = p3[1] - p0[1];	vect2[2] = p3[2] - p0[2];
			crossProd(currentNormal,vect1,vect2);
			glNormal3fv(currentNormal);
			glTexCoord2f(u0,v0);glVertex3fv(p0);
			glTexCoord2f(u1,v1);glVertex3fv(p2);
			glTexCoord2f(u0,v1);glVertex3fv(p3);

//			printf("vect1:(%f,%f,%f), vect2:(%f,%f,%f)\n",vect1[0],vect1[1],vect1[2],vect2[0],vect2[1],vect2[2]);
//			printf("normal 2:(%f,%f,%f)\n\n",currentNormal[0],currentNormal[1],currentNormal[2]);
		
		}
	}
		
	glEnd();
	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}



void drawCilindroChiuso(float radius, float h, int steps) {
	
	int i;
	float angle0;
	float step = 2*M_PI/steps;

	drawSemiCilindroOutside(radius,h,steps/2);
	glPushMatrix();
		glRotatef(180.0,0.0,1.0,0.0);
		drawSemiCilindroOutside(radius,h,steps/2);
	glPopMatrix();


	//printf("per dio!");

	//glPushMatrix();
	glBegin(GL_POLYGON);
	for(i=0;i<steps;i++) {
		angle0=(i)*step;
		glNormal3f(0.0, -1.0, 0.0);
		glTexCoord2f(radius*cos(angle0) / textureScaling, radius*sin(angle0) / textureScaling);
		glVertex3f(radius*cos(angle0),0.0,radius*sin(angle0));
	}
	glEnd();
	//glPopMatrix();

	//glPushMatrix();
	glBegin(GL_POLYGON);
	for(i=steps;i>0;i--) {
		angle0=(i)*step;
		glNormal3f(0.0, +1.0, 0.0);
		glTexCoord2f(radius*cos(angle0) / textureScaling, radius*sin(angle0) / textureScaling);
		glVertex3f(radius*cos(angle0),h,radius*sin(angle0));
	}
	glEnd();
	//glPopMatrix();

}



float random01() {
	return (float)rand() / ((float)(RAND_MAX)+(float)(1));
}




void randomColor(float color[4]) {
	color[0] = random01()*0.4+0.6;
	color[1] = random01()*0.4+0.6;
	color[2] = random01()*0.4+0.6;
	color[3] = 0.8;
}


void drawArco(float radius, float width, int semicircleSteps) {

	setTexture(TEXTURE_MARMO_BIANCO);

	drawSuperficieArcata(radius, semicircleSteps);

	glPushMatrix();
		glTranslatef(2*radius,0.0,-width);
		glRotatef(180.0,0.0,1.0,0.0);
		drawSuperficieArcata(radius, semicircleSteps);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(radius,0.0,0.0);
		glRotatef(-90.0,1.0,0.0,0.0);
		drawSemiCilindroInside(radius, width, semicircleSteps*2);
	glPopMatrix();

}



/*
Disegna una semisfera di raggio radius e precisione steps.
estensione su X,Y,Z: [-radius,+radius]*[0,0]*[0,+radius]
L'origine coincide con il centro degli assi
*/
void drawSemisfera(float radius, int steps) {
	
	float alpha = M_PI / (float)steps;
	int i;

	glNormal3f(0.0,1.0,0.0);

	glBegin(GL_TRIANGLE_FAN);
	
		glTexCoord2f(radius / textureScaling, 0.0);	glVertex3f(0.0,0.0,0.0);
		//printf("vertice semisfera: (%f,%f,%f)\n",0.0,0.0,0.0);

		for(i=steps; i>=0; i--) {
			glTexCoord2f(radius*(1+cos(i*alpha)) / textureScaling, radius*sin(i*alpha) / textureScaling);	
			glVertex3f(radius*cos(i*alpha),0.0,radius*sin(i*alpha));
			//printf("vertice semisfera: (%f,%f,%f)\n",radius*cos(i*alpha),0.0,radius*sin(i*alpha));
		}

	glEnd();

}


void drawFiammella() {

	int i;
	float altezza = 0.5;
	float larghezza = 0.3;
	float p1[3], p2[3], p3[3], p4[3], p5[3], p6[3], p7[3], p8[3];
	float color[4];

	glDisable(GL_TEXTURE_2D);

	glDisable(GL_CULL_FACE);

	glNormal3f(0.0,0.0,1.0);

//	printf("step=%f, substep=%f\n",step,substep);

	glPushMatrix();
		glRotatef((random01()*90.0), 0.0, 1.0, 0.0);

		altezza = (random01()*altezza)+0.5;
		
		p1[0] = 0.0; 	p1[1] = 0.0; 	p1[2] = 0.0; 
		p2[0] = larghezza/2;	p2[1] = altezza/2;	p2[2] = 0.0; 
		p3[0] = 0.0; 	p3[1] = altezza; 	p3[2] = 0.0; 
		p4[0] = -larghezza/2; 	p4[1] = altezza/2; 	p4[2] = 0.0; 
			
		p5[0] = 0.0; 	p5[1] = 0.0; 	p5[2] = 0.0; 
		p6[0] = 0.0;	p6[1] = altezza/2;	p6[2] = -larghezza/2; 
		p7[0] = 0.0; 	p7[1] = altezza; 	p7[2] = 0.0; 
		p8[0] = 0.0; 	p8[1] = altezza/2; 	p8[2] = larghezza/2; 

		glBegin(GL_QUADS);


		settaFiammellaColori(1.0, random01(), 0.0);				
		glVertex3fv(p1);	
		settaFiammellaColori(1.0, random01(), 0.0);				
		glVertex3fv(p2);	
		settaFiammellaColori(1.0, random01(), 0.0);				
		glVertex3fv(p3);
		settaFiammellaColori(1.0, random01(), 0.0);				
		glVertex3fv(p4);

		settaFiammellaColori(1.0, random01(), 0.0);				
		glVertex3fv(p5);	
		settaFiammellaColori(1.0, random01(), 0.0);				
		glVertex3fv(p6);	
		settaFiammellaColori(1.0, random01(), 0.0);				
		glVertex3fv(p7);
		settaFiammellaColori(1.0, random01(), 0.0);				
		glVertex3fv(p8);

	glEnd();
	
	glPopMatrix();	
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

}

void drawFiammellaLampadario() {

	int i;
	float altezza = 0.8;
	float larghezza = 0.5;
	float p1[3], p2[3], p3[3], p4[3], p5[3], p6[3], p7[3], p8[3];
	float color[4];

	glDisable(GL_TEXTURE_2D);

	glDisable(GL_CULL_FACE);

	glNormal3f(0.0,0.0,1.0);

//	printf("step=%f, substep=%f\n",step,substep);

	glPushMatrix();
		glRotatef((random01()*90.0), 0.0, 1.0, 0.0);

		altezza = (random01()*altezza)+0.8;
		
		p1[0] = 0.0; 	p1[1] = 0.0; 	p1[2] = 0.0; 
		p2[0] = larghezza/2;	p2[1] = altezza/2;	p2[2] = 0.0; 
		p3[0] = 0.0; 	p3[1] = altezza; 	p3[2] = 0.0; 
		p4[0] = -larghezza/2; 	p4[1] = altezza/2; 	p4[2] = 0.0; 
			
		p5[0] = 0.0; 	p5[1] = 0.0; 	p5[2] = 0.0; 
		p6[0] = 0.0;	p6[1] = altezza/2;	p6[2] = -larghezza/2; 
		p7[0] = 0.0; 	p7[1] = altezza; 	p7[2] = 0.0; 
		p8[0] = 0.0; 	p8[1] = altezza/2; 	p8[2] = larghezza/2; 

		glBegin(GL_QUADS);


		settaFiammellaColori(1.0, random01(), 0.0);				
		glVertex3fv(p1);	
		settaFiammellaColori(1.0, random01(), 0.0);				
		glVertex3fv(p2);	
		settaFiammellaColori(1.0, random01(), 0.0);				
		glVertex3fv(p3);
		settaFiammellaColori(1.0, random01(), 0.0);				
		glVertex3fv(p4);

		settaFiammellaColori(1.0, random01(), 0.0);				
		glVertex3fv(p5);	
		settaFiammellaColori(1.0, random01(), 0.0);				
		glVertex3fv(p6);	
		settaFiammellaColori(1.0, random01(), 0.0);				
		glVertex3fv(p7);
		settaFiammellaColori(1.0, random01(), 0.0);				
		glVertex3fv(p8);

	glEnd();
	
	glPopMatrix();	
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

}



/*----------------------------------------------------*/
/*----------------------------------------------------*/

