#include "lights.h"
#include "camera.h"

/* posizioni luci */

#define GLSCALAMENTO 2.0






/* posizione LIGHT 0 il sole */
/* posizione LIGHT 0 direzionale - a 45 gradi, diretta verso l'origine */
GLfloat light_position_directional[4]={
		10.0f * GLSCALAMENTO,
		10.0f * GLSCALAMENTO,
		10.0f * GLSCALAMENTO,
		0.0f
};


/* posizione LIGHT 1, luce mobile rossa al primo piano */
GLfloat light_position_localONE[4]={
		5.0f * GLSCALAMENTO,
		5.0f * GLSCALAMENTO,
		5.0f * GLSCALAMENTO,
		1.0f
	};

/* posizione LIGHT 2, luce mobile gialla al secondo piano */
GLfloat light_position_localTWO[4]={
		-10.0f * GLSCALAMENTO,
		-25.0f * GLSCALAMENTO,
		1.0f * GLSCALAMENTO,
		1.0f
	};


/* posizione LIGHT 3, luce mobile verde al secondo piano */
GLfloat light_position_localTHREE[4]={
		-10.0f * GLSCALAMENTO,
		-25.0f * GLSCALAMENTO,
		1.0f * GLSCALAMENTO,
		1.0f
	};







/* LIGHT2: luce del lampadario */
GLfloat light_position_lampadario[4]={
		-13.100014 * GLSCALAMENTO, 
		14.100018 * GLSCALAMENTO, 
		10.000002 * GLSCALAMENTO, 
		1
};

GLfloat light_direction_lampadario[4]={
		1.0f * GLSCALAMENTO,
		-13.0 * GLSCALAMENTO,
		-1.0f * GLSCALAMENTO,
		0.0f
};


/* colori */

GLfloat color_black     [4]  = {0.0f, 0.0f, 0.0f, 1.0f}; 
GLfloat color_white     [4]  = {1.0f, 1.0f, 1.0f, 1.0f}; 
GLfloat color_red       [4]  = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat color_green     [4]  = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat color_blue      [4]  = {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat coloryellow		[4]  = {1.0f, 1.0f, 0.0f, 1.0f};
GLfloat color_yellow_light[4]  = {0.87f, 0.47f, 0.09f, 1.0f};
GLfloat sun_color_interni[4]  = {0.5f, 0.5f, 0.5f, 1.0f};





void setupLights() {

	printf("Setup lights parameters. runs once\n");
	
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



	/* luce rossa numero 1 locale al piano terra */
	glLightfv(GL_LIGHT1, GL_AMBIENT  , color_red);
	glLightfv(GL_LIGHT1, GL_DIFFUSE  , color_red);
	glLightfv(GL_LIGHT1, GL_SPECULAR , color_red);
	glLightfv(GL_LIGHT1, GL_POSITION , light_position_localONE);


	/* luce gialla numero 2 locale al primo piano */
	glLightfv(GL_LIGHT2, GL_AMBIENT  , coloryellow);
	glLightfv(GL_LIGHT2, GL_DIFFUSE  , coloryellow);
	glLightfv(GL_LIGHT2, GL_SPECULAR , coloryellow);
	glLightfv(GL_LIGHT2, GL_POSITION , light_position_localTWO);
	
	
	/* luce verde numero 3 locale al primo piano */
	glLightfv(GL_LIGHT3, GL_AMBIENT  , color_green);
	glLightfv(GL_LIGHT3, GL_DIFFUSE  , color_green);
	glLightfv(GL_LIGHT3, GL_SPECULAR , color_green);
	glLightfv(GL_LIGHT3, GL_POSITION , light_position_localTHREE);





	//glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 77);
	//glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_direction_lampadario);
	//glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.05f);
	//glLighti(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.1f);
	

	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);

	glEnable(GL_LIGHTING);
}

