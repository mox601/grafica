#include "lights.h"

/* posizioni luci */

// luce direzionale, sole
//  GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 }; 
/* posizione LIGHT 0 direzionale - a 45 gradi, diretta verso l'origine */
GLfloat light_position_directional[4]={
		-1.0f,
		-1.0f,
		-1.0f,
		0.0f
};


/* posizione LIGHT 1 */
GLfloat light_position_local[4]={
		-10.0f,
		-25.0f,
		1.0f,
		1.0f
	};


/* LIGHT2: luce del lampadario */
GLfloat light_position_lampadario[4]={
		-13.100014, 14.100018, 10.000002, 1
};

GLfloat light_direction_lampadario[4]={
		1.0f,
		-13.,
		0.0f
};


/* colori */

GLfloat color_black     [4]  = {0.0f, 0.0f, 0.0f, 1.0f}; 
GLfloat color_white     [4]  = {1.0f, 1.0f, 1.0f, 1.0f}; 
GLfloat color_red       [4]  = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat color_green     [4]  = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat color_blue      [4]  = {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat coloryellow		[4]  = {1.0f, 1.0f, 0.0f, 1.0f};
GLfloat color_yellow_light[4]  = {1.0f, 0.65f, 0.06f, 1.0f};





void setupLights() {

	printf("setup lights parameters. runs once\n");
	/* migliora l'illuminazione */
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);

	/*
	da fare: 
	luce0 - luce ambientale, il sole. viene dall'alto. da abbinare ad un'altra luce
	luce1 - accoppiata alla luce ambientale?? 
	luce2 - luce di un lampione?
	
	*/

	/* luce numero 0 direzionale, il sole  - giallo? */
	glLightfv(GL_LIGHT0, GL_AMBIENT  , color_yellow_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE  , color_yellow_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR , color_yellow_light);
	glLightfv(GL_LIGHT0, GL_POSITION , light_position_directional);

	/* luce numero 1 locale */
	glLightfv(GL_LIGHT1, GL_AMBIENT  , color_white);
	glLightfv(GL_LIGHT1, GL_DIFFUSE  , color_white);
	glLightfv(GL_LIGHT1, GL_SPECULAR , color_white);
	glLightfv(GL_LIGHT1, GL_POSITION , light_position_local);



	/* LIGHT2: lampadario */	
	glLightfv(GL_LIGHT2, GL_AMBIENT  , color_red);
	glLightfv(GL_LIGHT2, GL_DIFFUSE  , color_red);
	glLightfv(GL_LIGHT2, GL_SPECULAR , color_white);
	glLightfv(GL_LIGHT2, GL_POSITION , light_position_lampadario);
	//glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 77);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_direction_lampadario);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.05f);
	glLighti(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.1f);
	

	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);

	glEnable(GL_LIGHTING);
}

