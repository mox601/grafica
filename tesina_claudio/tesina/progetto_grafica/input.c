#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     
#include <GL/glut.h>  
#include "player.h"

#define EXIT 27
#define FORWARD 119
#define BACKWARD 115
#define LEFT 97
#define RIGHT 100
#define CROUCH 99
#define JUMP 32

#ifndef M_PI
#define M_PI 3.14159265
#endif

int startx, starty;
int time, deltatime;
int going_forward, going_backward, going_left, going_right, moving_head; 

/* prototype */
void mouse(int button, int state, int x, int y);
void keyPressed(unsigned char c, int x, int y);
void keyReleased(unsigned char c, int x, int y);
void motion(int x, int y);
void motion2(int x, int y);



/* ---------------------------------------------------------- */
void keyPressed(unsigned char c, int x, int y) {
	
	switch(c) {
		case EXIT: 
			exit(0);
			break;
		case FORWARD:
			walkForward(PLAYER_MOVING);
			break;
		case BACKWARD:
			walkBackward(PLAYER_MOVING);
			break;
		case LEFT:
			walkLeft(PLAYER_MOVING);
			break;
		case RIGHT:
			walkRight(PLAYER_MOVING);
			break;
		case CROUCH:
			crouch(PLAYER_MOVING);
			break;
		case JUMP:
			jump(PLAYER_MOVING);
			break;
	}

	setPlayerTime(glutGet(GLUT_ELAPSED_TIME));

	glutPostRedisplay();

}

/* ---------------------------------------------------------- */
void keyReleased(unsigned char c, int x, int y) {
	
	switch(c) {
		case FORWARD:
			walkForward(PLAYER_IDLE);
			break;
		case BACKWARD:
			walkBackward(PLAYER_IDLE);
			break;
		case LEFT:
			walkLeft(PLAYER_IDLE);
			break;
		case RIGHT:
			walkRight(PLAYER_IDLE);
			break;
		case CROUCH:
			crouch(PLAYER_IDLE);
			break;
		case JUMP:
			jump(PLAYER_IDLE);
			break;
	}

	glutPostRedisplay();
}


/* ---------------------------------------------------------- */
void mouse(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			moveHead(PLAYER_MOVING);
			//printf("player moving head");
			startx = x;
			starty = y;
		}
		else
		      moveHead(PLAYER_IDLE);
	}

	glutPostRedisplay();

}


/* ---------------------------------------------------------- */
void motion2(int x, int y) {

	turnHead(startx-x,starty-y);
	startx = x;
	starty = y;
	//glutPostRedisplay();

}

void motion(int x, int y) {
	startx = x;
	starty = y;
	glutPassiveMotionFunc(motion2);
}
