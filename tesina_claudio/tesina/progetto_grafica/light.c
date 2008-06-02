#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <GL/glut.h>
#include "utils.h"

GLfloat ambient1[4] = {0.2,0.2,0.2,1.0};
GLfloat fogcolor1[4] = {0.1,0.0,0.1,1.0};
GLfloat skycolor1[4] = {0.0,0.0,0.2,1.0};
GLfloat diffuse1[4] = {1.0,1.0,1.0,1.0};
GLfloat specular1[4] = {1.0,1.0,1.0,1.0};
GLfloat emission1[4] = {0.0,0.0,0.0,1.0};
GLfloat lightmoon1[4] = {0.5,0.5,0.5,1.0};

GLfloat black1[4] = {0.0,0.0,0.0,1.0};

void resetMaterial();
void setColor(float red, float green, float blue);

void resetMaterial() {

	glMaterialfv(GL_FRONT,GL_AMBIENT,ambient1);
	glMaterialfv(GL_BACK,GL_AMBIENT,black1);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse1);
	glMaterialfv(GL_BACK,GL_DIFFUSE,black1);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular1);
	glMaterialfv(GL_BACK,GL_SPECULAR,black1);
	glMaterialfv(GL_FRONT,GL_EMISSION,black1);
	glMaterialfv(GL_BACK,GL_EMISSION,black1);

}

void updateFiammaInterna(float position[4], float light[4], float maxRandomRadius) {

	float intensity = random01();

	position[0] += (2.0 * random01() - 1.0) * maxRandomRadius;
	position[1] += (2.0 * random01() - 1.0) * maxRandomRadius;
	position[2] += (2.0 * random01() - 1.0) * maxRandomRadius;
	position[3] = 1.0;

	light[0] = 0.3 + 0.3 * intensity;
	light[1] = 0.3 + 0.3 * intensity;
	light[2] = (0.1 + (0.2 * random01())) * intensity;
	light[3] = 1.0;

	//printf("light2 = %f\n",light[2]);

}

void updateFiammaEsterna(float position[4], float light[4], float maxRandomRadius) {

	float intensity = random01();

	position[0] += (2.0 * random01() - 1.0) * maxRandomRadius;
	position[1] += (2.0 * random01() - 1.0) * maxRandomRadius;
	position[2] += (2.0 * random01() - 1.0) * maxRandomRadius;
	position[3] = 1.0;

	light[0] = 0.5 + 0.5 * intensity;
	light[1] = 0.5 + 0.5 * intensity;
	light[2] = (0.5 + (0.5 * random01())) * intensity;
	light[3] = 1.0;

	//printf("light2 = %f\n",light[2]);

}


static void settaColori(float red, float green, float blue, float alpha) {

	float material_Ka[4]={0.21*red,0.21*green,0.21*blue,alpha};
	float material_Kd[4]={red,green,blue,alpha};
	float material_Ks[4]={0.33*red,0.33*green,0.33*blue,alpha};
	float material_Ke[4]={0,0,0,0};
	float material_Se=10;

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT ,material_Ka);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE ,material_Kd);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,material_Ks);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,material_Ke);
	glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,material_Se);

}

void setColor(float red, float green, float blue) {

	settaColori(red, green, blue, 1.0);
}

void setTransparentColor(float red, float green, float blue) {

	settaColori(red, green, blue, 0.8);
}

void settaFiammellaColori(float red, float green, float blue) {

	float material_Ka[4]={red,green,blue,0.5};
	float material_Kd[4]={red,green,blue,0.5};
	float material_Ks[4]={red,green,blue,0.5};
	float material_Ke[4]={red,green,blue,0.5};
	float material_Se=10;

	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT ,material_Ka);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE ,material_Kd);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,material_Ks);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,material_Ke);
	glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,material_Se);

}

