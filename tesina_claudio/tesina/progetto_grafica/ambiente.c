#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <GL/glut.h>
#include "utils.h"
#include "sistemilineari.h"
#include "texture.h"
#include "light.h"


#define MATDIM 17

static float maxDistance = 7.0;
static float normalizer;
static GLfloat punto1[3], punto2[3], punto3[3], punto4[3];
static GLfloat terrainCentre[3];
static float skyRadius;
static float terrainRadius;
static int firstRecursiveCall = 1;
GLfloat moonCoords[3];
static float terrainHeight[MATDIM][MATDIM];
static int terrainHeightFillArray = 3;
static int dbg = 0;
static int dbgpolline = 0;
static int dbgpipistrello = 0;
static GLfloat minLimitX = -89.0;
static GLfloat maxLimitX = 89.0;
static GLfloat minLimitZ = -186.5;
static GLfloat maxLimitZ = 186.5;

static float coordPolline[200][3];
static int pollineInitialized = 0;

static float angleRotazionePipistrello = 0.0;
static float angleAltezzaPipistrello = 0.0;

static float getHeightScalinata(float x, float z);
static void drawPolline();


static void drawPipistrello(float alpha) {

	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	//setColor(0.1,0.1,0.1);
	glColor3f(0.0,0.0,0.0);

	//corpo
	glBegin(GL_QUADS);

		glVertex3f(0.0,0.0,1.0);
		glVertex3f(0.5,0.0,0.0);
		glVertex3f(0.0,0.0,-1.0);
		glVertex3f(-0.5,0.0,0.0);

	glEnd();

	//ala sx
	glPushMatrix();

		//glTranslatef(0.5,0.0,0.0);
		glRotatef(alpha,0.0,0.0,1.0);

		glBegin(GL_TRIANGLES);

			glVertex3f(0.0,0.0,0.0);
			glVertex3f(1.5,0.0,1.5);
			glVertex3f(3.0,0.0,0.0);

		glEnd();

	glPopMatrix();

	//ala dx
	glPushMatrix();

		//glTranslatef(-0.5,0.0,0.0);
		glRotatef(-alpha,0.0,0.0,1.0);

		glBegin(GL_TRIANGLES);

			glVertex3f(0.0,0.0,0.0);
			glVertex3f(-1.5,0.0,1.5);
			glVertex3f(-3.0,0.0,0.0);

		glEnd();

	glPopMatrix();

	resetMaterial();

	glClearColor(0.1,0,0.1,1);

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

}

void drawPipistrelloInVolo(float radius) {

	glPushMatrix();
	
		glRotatef(-angleRotazionePipistrello,0.0,1.0,0.0);	
		glTranslatef(radius,5.0*sin(angleAltezzaPipistrello),0.0);

		if(dbgpipistrello) printf("pipistrello: rotate(%f,%f,%f,%f) + traslate(%f,%f,%f)\n",
			-angleRotazionePipistrello,0.0,1.0,0.0,radius,(radius / 8.0)*sin(angleAltezzaPipistrello),0.0);

		drawPipistrello(45.0*sin(angleAltezzaPipistrello));

	glPopMatrix();

	angleRotazionePipistrello += 3.0;
	angleAltezzaPipistrello ++;

	if(angleRotazionePipistrello > 360.0)
		angleRotazionePipistrello -= 360.0;

	if(angleAltezzaPipistrello > 360.0)
		angleAltezzaPipistrello -= 360.0;


}


static void drawPolline() {

	float dist = 5.0;
	
	glBegin(GL_QUADS);

		glVertex3f(0.0,-dist,0.0);
		glVertex3f(0.0,0.0,dist);
		glVertex3f(0.0,dist,0.0);
		glVertex3f(0.0,0.0,-dist);

		glVertex3f(0.0,-dist,0.0);
		glVertex3f(dist,0.0,0.0);
		glVertex3f(0.0,dist,0.0);
		glVertex3f(-dist,0.0,0.0);

	glEnd();

}

static void initPolline() {

	int i;

	for(i=0; i<200; i++) {

		coordPolline[i][0] = ( random01() * 3200.0 ) - 1600.0;
		coordPolline[i][1] = ( random01() * 200.0 ) + 20.0;
		coordPolline[i][2] = ( random01() * 3200.0 ) - 1600.0;

	}

}


void updatePolline() {

	float deltaX = 1.0;
	float deltaZ = 2.0;
	int i=0;
	int fuoriDallaScena = 0;

	if(!pollineInitialized) {
		if(dbgpolline) printf("inizializzo polline...\n\n");
		initPolline();
		pollineInitialized = 1;
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);

	setColor(1.0,1.0,1.0);

	for(i=0; i<200; i++) {

		glPushMatrix();

			glTranslatef(coordPolline[i][0],coordPolline[i][1],coordPolline[i][2]);
			glRotatef(15.0,random01(),random01(),random01());

			if(dbgpolline && i==0) printf("translate n. %d: (%f,%f,%f)\n",i,coordPolline[i][0],coordPolline[i][1],coordPolline[i][2]);

			drawPolline();

		glPopMatrix();

		coordPolline[i][0] += deltaX + random01();
		coordPolline[i][1] += ( random01() - 0.5 ) * 3.0;
		coordPolline[i][2] += deltaZ + random01();

		if(coordPolline[i][0] > 1600.0) {
			coordPolline[i][0] = 1600.0;
			fuoriDallaScena = 1;
		}

		if(coordPolline[i][2] > 1600.0) {
			coordPolline[i][2] = 1600.0;
			fuoriDallaScena = 1;
		}

		if(fuoriDallaScena) {
			coordPolline[i][0] = 1600.0 - coordPolline[i][0];
			coordPolline[i][2] = 1600.0 - coordPolline[i][2];
		}

	}

	//printf("\n");

	resetMaterial();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);

}


void getPointOnSky(float dest[3],float radius,float u,float v)
{

	u*=M_PI;
	v*=(M_PI);
	v-=M_PI/2;

	//mi sposto un po' dall'orizzonte!!!
	u*=0.8;
	u+=0.1*u;
	v*=0.8;

	dest[0]=radius*cos(u)*cos(v);
	dest[1]=radius*sin(u)*cos(v);
	dest[2]=radius*sin(v);
}


static int isInsideLimit(float point[3]) {

	return ( point[0]>=minLimitX && point[0]<=maxLimitX && point[2]>=minLimitZ && point[2]<=maxLimitZ );

} 

static int isNearLimit(float point[3]) {

	float delta = 5.0;
	int result = 0;



	if( 
		(fabs(point[0]-minLimitX) <= delta || fabs(point[0]-maxLimitX) <= delta) &&
		(fabs(point[2]-minLimitZ) <= delta || fabs(point[2]-maxLimitZ) <= delta)	 ) {

	//printf("p1 = %f, p2 = %f, abs = %f.\n",point[0],minLimitX, fabs(point[0]-minLimitX));
		result = 1;

	}


	return result;

} 


static void nearestCentre(float dest[3], float point[3]) {

	float x1, x2, x3, z1, z2, z3;
	float minX, minZ;
	

	x1 = abs(point[0]-minLimitX);	x2 = abs(point[0]);	x3 = abs(point[0]-maxLimitX);
	z1 = abs(point[2]-minLimitZ);	z2 = abs(point[2]);	z3 = abs(point[2]-maxLimitZ);

	minX = x1;
	if(x2<x1) minX = x2;
	if(x3<x2) minX = x3;

	minZ = z1;
	if(z2<z1) minZ = z2;
	if(z3<z2) minZ = z3;	

	dest[0] = minX;	dest[1] = 0.0;	dest[2] = minZ;

}


static void subdivide(int precision, int fillArray, GLfloat p1[3], GLfloat p2[3], GLfloat p3[3], GLfloat p4[3]) {
	
	GLfloat p12[3], p23[3], p34[3], p41[3], centro[3];
	GLfloat vect1[3], vect2[3], normal[3];
	GLfloat d1, d2, d3, d4, dcentro;
	int row, column;
	int i;
	float tempCentre[3];

	d1 = pointDistance(p1,terrainCentre);
	d2 = pointDistance(p2,terrainCentre);
	d3 = pointDistance(p3,terrainCentre);
	d4 = pointDistance(p4,terrainCentre);

	//se almeno un vertice del quadrato è dentro la sfera ...
	if(d1<=terrainRadius || d2<=terrainRadius || d3<=terrainRadius || d4<=terrainRadius || firstRecursiveCall) {

		firstRecursiveCall = 0;

		if(dbg) printf("random=%f, pointDistance=%f, exp=%f\n",random01(),pointDistance(centro,terrainCentre),exp(normalizer-precision));

		centro[1] = 0.0;
		centro[0] = (p1[0]+p2[0]+p3[0]+p4[0])/4; 
		centro[2] = (p1[2]+p2[2]+p3[2]+p4[2])/4; 
		//if(!isInsideLimit(centro)) {
			//nearestCentre(tempCentre,centro);
			centro[1] = (p1[1]+p2[1]+p3[1]+p4[1])/4 + 
				0.6*(random01()-0.5)*pointDistance(centro,terrainCentre) / exp(normalizer-precision) ;
		//}
		if(isNearLimit(centro) && dbg) 
			printf("centro: (%f,%f,%f)\n",centro[0],centro[1],centro[2]);


		if(fillArray==0) {

			row 	= (float)(MATDIM-1) * ( p1[0] + terrainRadius ) / ( 2 * terrainRadius );
			column 	= (float)(MATDIM-1) * ( p1[2] + terrainRadius ) / ( 2 * terrainRadius );
			terrainHeight[row][column] = p1[1];
			if(dbg) printf("p1 (%3.2f,%3.2f,%3.2f) \t--> height[%d][%d]=%3.2f\n",p1[0],p1[1],p1[2],row,column,p1[1]);


			row 	= (float)(MATDIM-1) * ( p2[0] + terrainRadius ) / ( 2 * terrainRadius );
			column 	= (float)(MATDIM-1) * ( p2[2] + terrainRadius ) / ( 2 * terrainRadius );
			terrainHeight[row][column] = p2[1];
			if(dbg) printf("p2 (%3.2f,%3.2f,%3.2f) \t--> height[%d][%d]=%3.2f\n",p2[0],p2[1],p2[2],row,column,p2[1]);


			row 	= (float)(MATDIM-1) * ( p3[0] + terrainRadius ) / ( 2 * terrainRadius );
			column 	= (float)(MATDIM-1) * ( p3[2] + terrainRadius ) / ( 2 * terrainRadius );
			terrainHeight[row][column] = p3[1];
			if(dbg) printf("p3 (%3.2f,%3.2f,%3.2f) \t--> height[%d][%d]=%3.2f\n",p3[0],p3[1],p3[2],row,column,p3[1]);


			row 	= (float)(MATDIM-1) * ( p4[0] + terrainRadius ) / ( 2 * terrainRadius );
			column 	= (float)(MATDIM-1) * ( p4[2] + terrainRadius ) / ( 2 * terrainRadius );
			terrainHeight[row][column] = p4[1];
			if(dbg) printf("p4 (%3.2f,%3.2f,%3.2f) \t--> height[%d][%d]=%3.2f\n",p4[0],p4[1],p4[2],row,column,p4[1]);


			row 	= (float)(MATDIM-1) * ( centro[0] + terrainRadius ) / ( 2 * terrainRadius );
			column 	= (float)(MATDIM-1) * ( centro[2] + terrainRadius ) / ( 2 * terrainRadius );
			terrainHeight[row][column] = centro[1];
			if(dbg) printf("centro (%3.2f,%3.2f,%3.2f) \t--> height[%d][%d]=%3.2f\n\n",centro[0],centro[1],centro[2],row,column,centro[1]);

		}

	
		if(precision==0) {
	
			glBegin(GL_TRIANGLES);

				if (dbg) printf("triangle fan:\n");
	
				//centro,1,2
				for(i=0; i<3; i++) {
					vect1[i] = p1[i]-centro[i];
					vect2[i] = p2[i]-centro[i];
				}
				crossProd(normal,vect1,vect2);
				glNormal3fv(normal);
				glTexCoord2f(centro[0],centro[2]);	glVertex3fv(centro);
				glTexCoord2f(p1[0],p1[2]);		glVertex3fv(p1);
				glTexCoord2f(p2[0],p2[2]);		glVertex3fv(p2);
				if (dbg) {
					printf("centro-1-2: (%f,%f,%f), (%f,%f,%f), (%f,%f,%f)\n",
						centro[0], centro[1], centro[2],
						p1[0], p1[1], p1[2],
						p2[0], p2[1], p2[2]
					);
				}
	
				//centro,2,3
				for(i=0; i<3; i++) {
					vect1[i] = p2[i]-centro[i];
					vect2[i] = p3[i]-centro[i];
				}
				crossProd(normal,vect1,vect2);
				glNormal3fv(normal);
				glTexCoord2f(centro[0],centro[2]);	glVertex3fv(centro);
				glTexCoord2f(p2[0],p2[2]);		glVertex3fv(p2);
				glTexCoord2f(p3[0],p3[2]);		glVertex3fv(p3);
				if (dbg) {
					printf("centro-2-3: (%f,%f,%f), (%f,%f,%f), (%f,%f,%f)\n",
						centro[0], centro[1], centro[2],
						p2[0], p2[1], p2[2],
						p3[0], p3[1], p3[2]
					);
				}
	
				//centro,3,4
				for(i=0; i<3; i++) {
					vect1[i] = p3[i]-centro[i];
					vect2[i] = p4[i]-centro[i];
				}
				crossProd(normal,vect1,vect2);
				glNormal3fv(normal);
				glTexCoord2f(centro[0],centro[2]);	glVertex3fv(centro);
				glTexCoord2f(p3[0],p3[2]);		glVertex3fv(p3);
				glTexCoord2f(p4[0],p4[2]);		glVertex3fv(p4);
				if (dbg) {
					printf("centro-3-4: (%f,%f,%f), (%f,%f,%f), (%f,%f,%f)\n",
						centro[0], centro[1], centro[2],
						p3[0], p3[1], p3[2],
						p4[0], p4[1], p4[2]
					);
				}
	
				//centro,4,1
				for(i=0; i<3; i++) {
					vect1[i] = p4[i]-centro[i];
					vect2[i] = p1[i]-centro[i];
				}
				crossProd(normal,vect1,vect2);
				glNormal3fv(normal);
				glTexCoord2f(centro[0],centro[2]);	glVertex3fv(centro);
				glTexCoord2f(p4[0],p4[2]);		glVertex3fv(p4);
				glTexCoord2f(p1[0],p1[2]);		glVertex3fv(p1);
				if (dbg) {
					printf("centro-4-1: (%f,%f,%f), (%f,%f,%f), (%f,%f,%f)\n\n",
						centro[0], centro[1], centro[2],
						p4[0], p4[1], p4[2],
						p1[0], p1[1], p1[2]
					);
				}
	
			glEnd();
	
		}
	
		else {
	
			//12
			p12[1] = 0.0; 
			p12[0] = (p1[0]+p2[0])/2; 
			p12[2] = (p1[2]+p2[2])/2; 
			//if(!isInsideLimit(p12))
				//nearestCentre(tempCentre,centro);
				p12[1] = (p1[1]+p2[1])/2; 
			if(isNearLimit(p12) && dbg) 
				printf("p12: (%f,%f,%f)\n",p12[0],p12[1],p12[2]);
	
			//23
			p23[1] = 0.0; 
			p23[0] = (p2[0]+p3[0])/2; 
			p23[2] = (p2[2]+p3[2])/2; 
			//if(!isInsideLimit(p23))
				p23[1] = (p2[1]+p3[1])/2; 
			if(isNearLimit(p23) && dbg) 
				printf("p23: (%f,%f,%f)\n",p23[0],p23[1],p23[2]);
	
			//34
			p34[1] = 0.0; 
			p34[0] = (p3[0]+p4[0])/2; 
			p34[2] = (p3[2]+p4[2])/2; 
			//if(!isInsideLimit(p34))
				p34[1] = (p3[1]+p4[1])/2; 
			if(isNearLimit(p34) && dbg) 
				printf("p34: (%f,%f,%f)\n",p34[0],p34[1],p34[2]);
	
			//41
			p41[1] = 0.0; 
			p41[0] = (p4[0]+p1[0])/2; 
			p41[2] = (p4[2]+p1[2])/2; 
			//if(!isInsideLimit(p34))
				p41[1] = (p4[1]+p1[1])/2; 
			if(isNearLimit(p41) && dbg) 
				printf("p41: (%f,%f,%f)\n",p41[0],p41[1],p41[2]);

			//fillArray--;
	
			subdivide(precision-1,fillArray-1,p1,p12,centro,p41);
			subdivide(precision-1,fillArray-1,p2,p23,centro,p12);
			subdivide(precision-1,fillArray-1,p3,p34,centro,p23);
			subdivide(precision-1,fillArray-1,p4,p41,centro,p34);
	
		}

	}

}


float getHeight(float x, float z) {

	float xOnMatrix, zOnMatrix;
	float A, B, C;
	int rowCentre, colCentre;
	int rowP1, colP1;
	int rowP2, colP2;
	float coefficienti[3][3];
	float termininoti[3] = {1.0,1.0,1.0};
	float risultato[3];

	float result;

	xOnMatrix = (float)(MATDIM-1) * ( x + terrainRadius ) / ( 2 * terrainRadius );
	zOnMatrix = (float)(MATDIM-1) * ( z + terrainRadius ) / ( 2 * terrainRadius );

	//riga del centro più vicino
	rowCentre = ( (int)( xOnMatrix / 2.0 ) ) * 2  + 1;
	if(rowCentre == MATDIM)	rowCentre = MATDIM - 2;
	
	//colonna del centro più vicino
	colCentre = ( (int)( zOnMatrix / 2.0 ) ) * 2  + 1;
	if(colCentre == MATDIM)	colCentre = MATDIM - 2;

	if(xOnMatrix+zOnMatrix > rowCentre+colCentre) {
		
		//triangolo a destra
		if(xOnMatrix-zOnMatrix > rowCentre-colCentre) {
			rowP1 = rowCentre + 1;
			colP1 = colCentre + 1;
			rowP2 = rowCentre + 1;
			colP2 = colCentre - 1;

		}
		
		//triangolo in basso
		else {
			rowP1 = rowCentre - 1;
			colP1 = colCentre + 1;
			rowP2 = rowCentre + 1;
			colP2 = colCentre + 1;
		}

	}

	else {

		//triangolo in alto
		if(xOnMatrix-zOnMatrix > rowCentre-colCentre) {
			rowP1 = rowCentre + 1;
			colP1 = colCentre - 1;
			rowP2 = rowCentre - 1;
			colP2 = colCentre - 1;

		}
		
		//triangolo a sinistra
		else {
			rowP1 = rowCentre - 1;
			colP1 = colCentre - 1;
			rowP2 = rowCentre - 1;
			colP2 = colCentre + 1;
		}

	}

	if(dbg) {
		printf("x = %f, z = %f;\n",x,z);
		printf("xOnMatrix = %f, zOnMatrix = %f;\n",xOnMatrix, zOnMatrix);
		printf("rowP1 = %d, colP1 = %d.\n",rowP1, colP1);
		printf("rowP2 = %d, colP2 = %d.\n",rowP2, colP2);
		printf("rowCentre = %d, colCentre = %d.\n\n",rowCentre, colCentre);
	}

	coefficienti[0][0] = rowP1;	coefficienti[0][1] = terrainHeight[rowP1][colP1];	coefficienti[0][2] = colP1;	
	coefficienti[1][0] = rowP2;	coefficienti[1][1] = terrainHeight[rowP2][colP2];	coefficienti[1][2] = colP2;	
	coefficienti[2][0] = rowCentre;	coefficienti[2][1] = terrainHeight[rowCentre][colCentre];	coefficienti[2][2] = colCentre;	

	gauss(coefficienti,termininoti,risultato);
	result = ( 1.0 - risultato[0]*xOnMatrix - risultato[2]*zOnMatrix ) / risultato[1] ;

	if(dbg) {
		printf("piano individuato: %fx + %fy + %fz = 1.0\n\n",risultato[0],risultato[1],risultato[2]);
		printf("altezza = %f!\n\n",result);
	}

	//AGGIUNTA SCALINATA!!
	if(getHeightScalinata(x,z) > result)
		result = getHeightScalinata(x,z);

	return result;

}

static float getHeightScalinata(float x, float z) {

	float minLowX = - 89.0 - 42.0;
	float maxLowX = + 89.0 + 42.0;
	float minLowZ = - 186.5 - 42.0;
	float maxLowZ = + 186.5 + 42.0;

	float minHiX = - 89.0;
	float maxHiX = + 89.0;
	float minHiZ = - 186.5;
	float maxHiZ = + 186.5;

	float result;

	float largGradino = 3.0;
	float altGradino = 1.5;

	float tempHeightX = 0.0;
	float tempHeightZ = 0.0;

	//se sto in cima
	if(x>=minHiX && x<=maxHiX && z>=minHiZ && z<=maxHiZ)
		result = 20.0;

	else if( (x<minLowX || x>maxLowX) && (z<minLowZ || z>maxLowZ) )
		result = -999999999;

	else {

		if(x<0.0)
			x = -x;
		if(z<0.0)
			z = -z;

		tempHeightX = 20.0 - ( ( (int)(x-maxHiX) / largGradino ) * altGradino ) ;
		tempHeightZ = 20.0 - ( ( (int)(z-maxHiZ) / largGradino ) * altGradino ) ;

		if(tempHeightX < tempHeightZ)
			result = tempHeightX;
		else
			result = tempHeightZ;

	}


	return result;
		

}


void drawTerreno(float radius, int precision) {

	int i,j;

	punto1[0] = -radius; 	punto1[1] = 0.0; 	punto1[2] = radius;
	punto2[0] = radius; 	punto2[1] = 0.0; 	punto2[2] = radius;
	punto3[0] = radius; 	punto3[1] = 0.0; 	punto3[2] = -radius;
	punto4[0] = -radius; 	punto4[1] = 0.0; 	punto4[2] = -radius;



	terrainCentre[0] = 0.0;	terrainCentre[1] = 0.0;	terrainCentre[2] = 0.0;	

	terrainRadius = radius;

	normalizer = precision-1;


	subdivide(precision,terrainHeightFillArray,punto1,punto2,punto3,punto4);



	if(dbg) {
	
		printf("terrainHeight:\n");

		for(i=0; i<MATDIM; i++) {

			for(j=0; j<MATDIM; j++) {
				printf("%3.1f\t",terrainHeight[i][j]);
			}
			printf("\n");
	
		}

		printf("\n");
	
	}


}

void drawStelle(float radius, int nStelle) {
	
	int i;
	GLfloat coords[3];

	skyRadius = radius;
	moonCoords[0] = skyRadius*sin(M_PI/4);
	moonCoords[1] = skyRadius*sin(M_PI/4);
	moonCoords[2] = 0.0;

/*
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
*/
	glColor4f(1.0,1.0,1.0,1.0);	

	float brightness;

	glPointSize( 3.0 );

	for(i=0; i<nStelle/4; i++) {

		brightness = 0.5*(random01() + 1.0);
		
		getPointOnSky(coords,radius,random01(),random01());
		
		glBegin(GL_POINTS);
			glVertex3fv(coords);
		glEnd();

	}

	glPointSize( 1.0 );

	for(i=0; i<3*nStelle/4; i++) {
		
		brightness = 0.5*(random01() + 1.0);

		getPointOnSky(coords,radius,random01(),random01());
		
		glBegin(GL_POINTS);
			glVertex3fv(coords);
		glEnd();

	}


	glPointSize( 1.0 );

}

void drawMoon() {

	glEnable(GL_TEXTURE_2D);
	setTextureScaling(30);
	setTexture(TEXTURE_MOON);

	glPushMatrix();
		glTranslatef(moonCoords[0],moonCoords[1],moonCoords[2]);
		//glTranslatef(40.0,30.0,30.0);
		//glScalef(10.0,10.0,10.0);
		drawSphere(120.0,32,32);
	glPopMatrix();

}

void getMoonCoords(float dest[3]) {
	dest[0] = moonCoords[0];
	dest[1] = moonCoords[1];
	dest[2] = moonCoords[2];
}
