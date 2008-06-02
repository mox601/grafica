#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     
#include <GL/glut.h>  
#include "utils.h"
#include "ambiente.h"
#include "collision.h"

#ifndef M_PI
#define M_PI 3.14159265
#endif

GLfloat position [3];
GLfloat lookingAt [3];
GLfloat direction [3];
GLfloat vUp [3];

static float initialY;
static float crouchY;
static float standUpY;
static int maxJumpTime = 600;
static int jumpTime;
static float maxSpeed = 0.07;
static float slowSpeed = 0.03;
static float speed;
static int time, deltatime;
static int going_forward, going_backward, going_left, going_right, moving_head, jumping, flying, crouching;
GLfloat angley = 0.0;   
GLfloat anglex = 0.0; 

void initLookAt(GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat xLookAt, GLfloat yLookAt, GLfloat zLookAt, GLfloat xUp, GLfloat yUp, GLfloat zUp);
void updateLookAt();
void walkLeft(int mode);
void walkRight(int mode);
void walkForward(int mode);
void walkBackward(int mode);
void moveHead(int mode);
void turnHead(float deltax, float deltay);
void jump(int mode);
void crouch(int mode);
int isMoving();
void setPlayerTime(int millisec);

/* ---------------------------------------------------------- */
void initLookAt (GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat xLookAt, GLfloat yLookAt, GLfloat zLookAt, GLfloat xUp, GLfloat yUp, GLfloat zUp) {


	/* initial time value */
	time = glutGet(GLUT_ELAPSED_TIME);

	/* initial Y position */
	initialY = yPos;
	crouchY = initialY/2;
	standUpY = initialY;

	/* initial values for direction vector (not normalized) */
	GLfloat deltax = xLookAt - xPos;
	GLfloat deltay = yLookAt - yPos;
	GLfloat deltaz = zLookAt - zPos;
	
	/* normalizing deltax and deltaz (to set anglex) */
	direction[0] = deltax;
	direction[1] = 0.0f;
	direction[2] = deltaz;
	normalize(direction);

	/* setting anglex starting value */
	if(direction[0]<0.0f)
		anglex = -360.0f*acos(direction[2])/(2*M_PI);
	else
		anglex = +360.0f*acos(direction[2])/(2*M_PI);

	/* setting starting direction */
	direction[0] = deltax;
	direction[1] = deltay;
	direction[2] = deltaz;
	normalize(direction);

	/* setting angley starting value */
	angley = 360.0f*asin(direction[1])/(2*M_PI);

	/* setting starting position */
	position[0] = xPos;
	position[1] = yPos;
	position[2] = zPos;

	/* setting starting lookAt */
	lookingAt[0] = position[0] + direction[0];
	lookingAt[1] = position[1] + direction[1];
	lookingAt[2] = position[2] + direction[2];

	/* setting starting vUp */
	vUp[0] = xUp;
	vUp[1] = yUp;
	vUp[2] = zUp;
	normalize(vUp);

	/* altezza terreno! */
	position[1] += getHeight(position[0],position[2]);


	/* calling gluLookAt(...) */
 	glLoadIdentity();
	gluLookAt(
		position[0], 	position[1], 	position[2],		   
		lookingAt[0], 	lookingAt[1], 	lookingAt[2],
		vUp[0], 	vUp[1], 	vUp[2]); 

	//printf("anglex = %f, angle y = %f;\n",anglex,angley);
	printf("initial gluLookAt = (\n\t%f,%f,%f,\n\t%f,%f,%f,\n\t%f,%f,%f\n)\n",
		position[0], position[1], position[2],		   
		lookingAt[0], lookingAt[1], lookingAt[2],
		vUp[0], vUp[1], vUp[2]);

	return;

}



static float handleJump(int deltatime) {
	
	float result = 0;

	//printf("handle jump\n");

	//if(!jumping && !flying)
	//	result = 0;

	if(jumping && !flying) {
		//printf("start jump, deltatime=%d\n",deltatime);
		flying = 1;
		jumpTime = deltatime;
		result += 20 * sin(M_PI*((float)jumpTime/(float)maxJumpTime));
		//printf("first result = %f\n",result);

		glutPostRedisplay();
	}

	if(flying) {
		jumpTime += deltatime;
		if(jumpTime>=maxJumpTime) {
			result = 0;
			flying = 0;
			//printf("end of jump\n");
		}
		else {
			result += 20 * sin(M_PI*((float)jumpTime/(float)maxJumpTime));
		}

		glutPostRedisplay();

	}

	return result;

}



/* ---------------------------------------------------------- */
void updateLookAt() {

	double moveX = 10.0;
	double moveZ = 10.0;
	int i;

	/* calculating time elapsed since last update */
	deltatime = glutGet(GLUT_ELAPSED_TIME) - time;
	time += deltatime;

	/* calculating new direction vector */
	direction[0] = sin(2.0*M_PI*anglex/360.0);
	direction[2] = cos(2.0*M_PI*anglex/360.0);
	direction[1] = sin(2.0*M_PI*angley/360.0);

	for(i=0; i<3; i++)
		if(direction[i]==-0.0) direction[i] = 0.0;

	/* normalizing direction vector */
	normalize(direction);

	/* updating position (if moving) */
	if(crouching) {
		position[1] = crouchY;
		speed = slowSpeed;
	}
	else {
		position[1] = standUpY;
		speed = maxSpeed;
	}

	//jump!
	position[1] += handleJump(deltatime);

	if(isMoving()) {

		if(going_forward) {

			moveX += direction[0]*deltatime*speed;
			moveZ += direction[2]*deltatime*speed;
	
		}

		if(going_backward) {

			moveX -= direction[0]*deltatime*speed;
			moveZ -= direction[2]*deltatime*speed;

		}
	
		if(going_right) {

			moveX -= direction[2]*deltatime*speed;
			moveZ += direction[0]*deltatime*speed;

		}

		if(going_left) {

			moveX += direction[2]*deltatime*speed;
			moveZ -= direction[0]*deltatime*speed;

		}	

		moveX -= 10.0;
		moveZ -= 10.0;

		/* test collision! */
		testCollision(position,moveX,moveZ);

	}

	/* altezza terreno! */
	position[1] += getHeight(position[0],position[2]);

	/* updating lookingAt */
	lookingAt[0] = position[0] + direction[0];
	lookingAt[1] = position[1] + direction[1];
	lookingAt[2] = position[2] + direction[2];

	/* calling gluLookAt(...) */
 	glLoadIdentity();
	gluLookAt(
		position[0], position[1], position[2],		   
		lookingAt[0], lookingAt[1], lookingAt[2],
		vUp[0], vUp[1], vUp[2]); 

	/*printf("position=(%f,%f,%f);\nlookingAt=(%f,%f,%f).\n",
		position[0], position[1], position[2],		   
		lookingAt[0], lookingAt[1], lookingAt[2]);*/

}


void walkLeft(int mode) {
	going_left = mode;
}

void walkRight(int mode) {
	going_right = mode;
}

void walkForward(int mode) {
	going_forward = mode;
}

void walkBackward(int mode) {
	going_backward = mode;
}

void moveHead(int mode) {
	moving_head = mode;
}

void turnHead(float deltax, float deltay) {

	//if(moving_head) {
		anglex += deltax;
		angley += deltay;
		if(angley>89.0f) 	angley = 89.0f;
		if(angley<-89.0f) 	angley = -89.0f;
		if(anglex>360.0f) 	anglex -= 360.0f;
		if(anglex<-360.0f) 	anglex += 360.0f;
		glutPostRedisplay();
	//}
	
}

void jump(int mode) {
	jumping = mode;
}

void crouch(int mode) {
	crouching = mode;
}

int isMoving() {
	if(going_left || going_right || going_forward || going_backward || flying)
		return 1;
	return 0;
}

void setPlayerTime(int millisec) {
	time = millisec;
}
