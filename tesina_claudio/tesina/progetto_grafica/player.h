#ifndef PLAYER_MODES
#define PLAYER_MOVING 1
#define PLAYER_IDLE   0
#endif

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
