#include "lights.h"

/* posizioni luci */

/* posizione luce 0 */
GLfloat light_position_local[4]={
		-10.0f,
		1.0f,
		1.0f,
		1.0f
	};

/* posizione luce 1 direzionale */
GLfloat light_position_directional[4]={
		10.0f,
		1.0f,
		1.0f,
		1.0f
};



/* colori */

GLfloat color_black     [4]  = {0.0f, 0.0f, 0.0f, 1.0f}; 
GLfloat color_white     [4]  = {1.0f, 1.0f, 1.0f, 1.0f}; 
GLfloat color_red       [4]  = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat color_green     [4]  = {0.0f, 1.0f, 0.0f, 1.0f};
GLfloat color_blue      [4]  = {0.0f, 0.0f, 1.0f, 1.0f};
GLfloat coloryellow[4]  = {1.0f, 1.0f, 0.0f, 1.0f};





void setupLights() {

	printf("setup lights parameters. runs once\n");
	/* migliora l'illuminazione */
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);

	/*
	da fare: 
	luce0 - luce ambientale, il sole. viene dall'alto. da abbinare ad un'altra luce
	luce1 - accoppiata alla luce ambientale. 
	luce2 - luce di un lampione
	
	*/


	/* luce numero 0 locale */
	glLightfv(GL_LIGHT0, GL_AMBIENT  , color_white);
	glLightfv(GL_LIGHT0, GL_DIFFUSE  , color_white);
	glLightfv(GL_LIGHT0, GL_SPECULAR , color_white);
	glLightfv(GL_LIGHT0, GL_POSITION , light_position_local);

	/* luce numero 1 direzionale */
	glLightfv(GL_LIGHT1, GL_AMBIENT  , color_white);
	glLightfv(GL_LIGHT1, GL_DIFFUSE  , color_white);
	glLightfv(GL_LIGHT1, GL_SPECULAR , color_white);
	glLightfv(GL_LIGHT1, GL_POSITION , light_position_directional);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
}

