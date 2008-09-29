#include "lights.h"
#include "camera.h"

/* posizioni luci */

#define GLSCALAMENTO 2.0






/* posizione LIGHT 0 il sole */
/* posizione LIGHT 0 direzionale - a 45 gradi, diretta verso l'origine */
/* meglio locale ?*/
GLfloat light_position_directional[4]={
		10.0f * GLSCALAMENTO,
		10.0f * GLSCALAMENTO,
		10.0f * GLSCALAMENTO,
		1.0f
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


/* posizione light 4, luce della luna, dalla parte opposta rispetto al sole */
GLfloat light_position_lunar[4] = {
		-100.0f * GLSCALAMENTO,
		-100.0f * GLSCALAMENTO,
		100.0f * GLSCALAMENTO,
		0.0f
};

 
 
/* faro esterno */
GLfloat faro_spotlight[4] = {
		13.790876 ,
		-16.574362,
		0.0f,
		1.0f
};




/* colori */

GLfloat color_black     [4]  = {0.0f, 0.0f, 0.0f, 1.0f}; 
GLfloat color_white     [4]  = {1.0f, 1.0f, 1.0f, 1.0f}; 
GLfloat color_red       [4]  = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat color_green     [4]  = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat color_blue      [4]  = {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat coloryellow		[4]  = {1.0f, 1.0f, 0.0f, 1.0f};
GLfloat color_yellow_light[4]  = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat sun_color_interni[4]  = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat moon_color_esterni[4]  = {0.05f, 0.05f, 0.05f, 1.0f};
GLfloat moon_color_interni[4]  = {0.005f, 0.005f, 0.005f, 1.0f};
GLfloat sun_color_interni_internalshell[4]  = {0.24f, 0.24f, 0.24f, 1.0f};
GLfloat variableLight[4]  = {0.0f, 1.0f, 0.0f, 1.0f};
/* direzioni */
GLfloat direzioneLuceRossa[3]  = {0, -1, 0};
GLfloat direzioneFaro[3]  = {0, 0, 15};





void setupLights() {

	printf("Setup lights parameters. runs once\n");
	
	/* migliora l'illuminazione */
	//glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
	

	/* luce numero 0 direzionale, il sole  - giallo? */
	glLightfv(GL_LIGHT0, GL_AMBIENT  , color_yellow_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE  , color_yellow_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR , color_yellow_light);
	glLightfv(GL_LIGHT0, GL_POSITION , light_position_directional);



	/* luce rossa numero 1 locale al piano terra, Ž una spotlight */
	//glLightfv(GL_LIGHT1, GL_AMBIENT  , color_red);
	glLightfv(GL_LIGHT1, GL_DIFFUSE  , color_red);
	glLightfv(GL_LIGHT1, GL_SPECULAR , color_red);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0f);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direzioneLuceRossa);
	glLightfv(GL_LIGHT1, GL_POSITION , light_position_localONE);
	
	// attenuazione con la distanza
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION,  1.0f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0035f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.002f);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 0.0f);






	/* luce gialla numero 2 locale al primo piano */
	//glLightfv(GL_LIGHT2, GL_AMBIENT  , coloryellow);
	glLightfv(GL_LIGHT2, GL_DIFFUSE  , coloryellow);
	glLightfv(GL_LIGHT2, GL_SPECULAR , coloryellow);
	glLightfv(GL_LIGHT2, GL_POSITION , light_position_localTWO);
	
	// attenuazione con la distanza
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION,  1.0f);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.045f);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.002f);
	
	
	
	/* luce verde numero 3 locale al secondo piano */
	//glLightfv(GL_LIGHT3, GL_AMBIENT  , color_green);
	glLightfv(GL_LIGHT3, GL_DIFFUSE  , color_green);
	glLightfv(GL_LIGHT3, GL_SPECULAR , color_green);
	glLightfv(GL_LIGHT3, GL_POSITION , light_position_localTHREE);


	// attenuazione con la distanza
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION,  1.0f);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.045f);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.002f);





	/* luce bianca numero 4 locale luna */
	glLightfv(GL_LIGHT4, GL_AMBIENT  , color_black);
	glLightfv(GL_LIGHT4, GL_DIFFUSE  , moon_color_esterni);
	glLightfv(GL_LIGHT4, GL_SPECULAR , moon_color_esterni);
	glLightfv(GL_LIGHT4, GL_POSITION , light_position_lunar);

	// attenuazione con la distanza
	glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION,  0.8f);
	glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.005f);
	glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.002f);



	/* luce bianca numero 5 locale faro */
	glLightfv(GL_LIGHT5, GL_AMBIENT  , color_black);
	glLightfv(GL_LIGHT5, GL_DIFFUSE  , color_white);
	glLightfv(GL_LIGHT5, GL_SPECULAR , color_white);
	glLightfv(GL_LIGHT5, GL_POSITION , faro_spotlight);

	// attenuazione con la distanza
	glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION,  0.8f);
	glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.005f);
	glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 0.02f);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 15.0f);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, direzioneFaro);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 0.0f);






	//glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 77);
	//glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_direction_lampadario);
	//glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.05f);
	//glLighti(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.1f);
	

	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);

	glEnable(GL_LIGHTING);
}

