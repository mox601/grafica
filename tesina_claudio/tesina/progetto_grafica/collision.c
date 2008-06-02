#include <stdio.h>
#include <math.h>

#define COLLISION_UNIT 4
#define COLLISION_RADIUSX 100
#define COLLISION_RADIUSZ 200
#define COLLISION_DIMX (2*COLLISION_RADIUSX/COLLISION_UNIT)
#define COLLISION_DIMZ (2*COLLISION_RADIUSZ/COLLISION_UNIT)

static char collisionMap[COLLISION_DIMX][COLLISION_DIMZ];
static float step;

static int dbg = 0;
static int collisionMapInitialized = 0;

static const float xOffset = -89.0; 
static const float zOffset = 186.5;

void initCollision() {

	int i,j;

	for(i=0; i<COLLISION_DIMX; i++) 
		for(j=0; j<COLLISION_DIMZ; j++) 
			collisionMap[i][j] = '.';

	step = (float)COLLISION_UNIT * sqrt(2.0) - 0.01;

}



static void collisionWithSameX(float constX, float minZ, float maxZ) {

	int minRow, maxRow;
	int minCol, maxCol;
	int j;

	if (dbg) printf("collisionWithSameX ( constX=%f, minZ=%f, maxZ=%f )\n",constX,minZ,maxZ);

	minRow = (int)( constX + COLLISION_RADIUSX ) / COLLISION_UNIT ;
	maxRow = minRow + 1;

	if (dbg) printf("minRow = %d, maxRow = %d\n",minRow,maxRow);

	minCol = (int)( minZ + COLLISION_RADIUSZ ) / COLLISION_UNIT ;
	maxCol = (int)( maxZ + COLLISION_RADIUSZ ) / COLLISION_UNIT ;

	if (dbg) printf("minCol = %d, maxCol = %d\n",minCol,maxCol);

	for(j=minCol; j<=maxCol; j++) {

		collisionMap[minRow][j] = 'X';
		collisionMap[maxRow][j] = 'X';

	}
	
}

static void collisionWithSameZ(float constZ, float minX, float maxX) {


	int minRow, maxRow;
	int minCol, maxCol;
	int i;

	if (dbg) printf("collisionWithSameZ ( constZ=%f, minX=%f, maxX=%f )\n",constZ,minX,maxX);

	minRow = (int)( minX + COLLISION_RADIUSX ) / COLLISION_UNIT ;
	maxRow = (int)( maxX + COLLISION_RADIUSX ) / COLLISION_UNIT ;

	if (dbg) printf("minRow = %d, maxRow = %d\n",minRow,maxRow);

	minCol = (int)( constZ + COLLISION_RADIUSZ ) / COLLISION_UNIT ;
	maxCol = minCol + 1;

	if (dbg) printf("minCol = %d, maxCol = %d\n",minCol,maxCol);

	for(i=minRow; i<=maxRow; i++) {

		collisionMap[i][minCol] = 'X';
		collisionMap[i][maxCol] = 'X';

	}

}

void addCollisionLine(float coll_x1, float coll_z1, float coll_x2, float coll_z2) {

	//aggiungo OFFSET!!
	coll_x1 += xOffset;
	coll_x2 += xOffset;
	coll_z1 += zOffset;
	coll_z2 += zOffset;

	if(!collisionMapInitialized) {
	
		initCollision();
		collisionMapInitialized = 1;

	}


	float minCoord = 0.0, maxCoord = 0.0;

	if(dbg) printf("\n\nadd collision line...\n");

	//stessa x

	if( coll_x1 == coll_x2 ) {

		if(dbg) printf("x1 = x2 = %f\n",coll_x1);

		if( coll_z1 < coll_z2 ) {
			minCoord = coll_z1;	maxCoord = coll_z2;
		}
		else {
			minCoord = coll_z2;	maxCoord = coll_z1;
		}

		if(dbg) printf("minCoord = %f, maxCoord = %f\n",minCoord,maxCoord);

		collisionWithSameX(coll_x1,minCoord,maxCoord);

	}

	//stessa z

	else if( coll_z1 == coll_z2 ) {

		if(dbg) printf("z1 = z2 = %f\n",coll_z1);

		if( coll_x1 < coll_x2 ) {
			minCoord = coll_x1;	maxCoord = coll_x2;
		}
		else {
			minCoord = coll_x2;	maxCoord = coll_x1;
		}

		if(dbg) printf("minCoord = %f, maxCoord = %f\n",minCoord,maxCoord);

		collisionWithSameZ(coll_z1,minCoord,maxCoord);

	}

}

static int isInCollisionMap(float xOnCollisionMap, float zOnCollisionMap) {

	return 
		xOnCollisionMap >= 0 && xOnCollisionMap <= (COLLISION_DIMX - 1) &&
		zOnCollisionMap >= 0 && zOnCollisionMap <= (COLLISION_DIMZ - 1) ;

}


float distance2D(float x1, float z1, float x2, float z2) {

	float result = sqrt( (x1-x2)*(x1-x2) + (z1-z2)*(z1-z2) );
	return result;

}


void testCollision(float position[3], float moveX, float moveZ) {

	if(!collisionMapInitialized) {
	
		initCollision();
		collisionMapInitialized = 1;

	}

	float startX = ( position[0] + COLLISION_RADIUSX ) / COLLISION_UNIT;
	float startZ = ( position[2] + COLLISION_RADIUSZ ) / COLLISION_UNIT;
	float spostX = ( moveX ) / COLLISION_UNIT;
	float spostZ = ( moveZ ) / COLLISION_UNIT;
	float targetX = ( position[0] + moveX + COLLISION_RADIUSX ) / COLLISION_UNIT;
	float targetZ = ( position[2] + moveZ + COLLISION_RADIUSZ ) / COLLISION_UNIT;

	int nearestCollisionX, nearestCollisionZ;

	int firstIntX, lastIntX;
	int firstIntZ, lastIntZ;
	int currentIntX, currentIntZ;
	int deltaX, deltaZ;
	int segnoX, segnoZ;
	float zForCurrentIntX, xForCurrentIntZ;
	float distanceFromCurrentIntX, distanceFromCurrentIntZ;
	float reachedX, reachedZ;
	int previousBound, nextBound;


	if(dbg) {
		printf("posX = %f, posZ = %f\n",position[0],position[2]);
		printf("moveX = %f, moveZ = %f\n",moveX,moveZ);
		printf("startX = %f, startZ = %f (coordinate matrice)\n",startX,startZ);
		printf("spostX = %f, spostZ = %f (coordinate matrice)\n",spostX,spostZ);
		printf("targetX = %f, targetZ = %f (coordinate matrice)\n",targetX,targetZ);
	}

	//se vado fuori dall'area soggetta a collisioni
	if(!isInCollisionMap(targetX,targetZ)) {

		if(dbg) printf("sono fuori dall'area soggetta a collisioni...\n");
		position[0] += moveX;
		position[2] += moveZ;
		return;

	}

	//se mi muovo dentro l'area soggetta a collisioni
	else {

		nearestCollisionX = (int)(targetX + 0.5);
		nearestCollisionZ = (int)(targetZ + 0.5);

		//se il punto dove voglio andare è occupato
		if(collisionMap[nearestCollisionX][nearestCollisionZ] == 'X') {

			//se posso "scorrere" lungo la componente x
			nearestCollisionX = (int)(targetX + 0.5);
			nearestCollisionZ = (int)(startZ + 0.5);
			if(collisionMap[nearestCollisionX][nearestCollisionZ] == '.') {

				position[0] += moveX;

			}

			//se posso "scorrere" lungo la componente z
			nearestCollisionX = (int)(startX + 0.5);
			nearestCollisionZ = (int)(targetZ + 0.5);
			if(collisionMap[nearestCollisionX][nearestCollisionZ] == '.') {

				position[2] += moveZ;

			}

			return;
		}
		else {
			position[0] += moveX;
			position[2] += moveZ;
			return;
		}

	}

}


void printCollisionMap() {

	int i,j;

	printf("\ncollision map:\n");
	for(i=0; i<COLLISION_DIMX; i++) {
		for(j=0; j<COLLISION_DIMZ; j++) 
			printf("%c",collisionMap[i][j]);
		printf("\n");
	}
	printf("\n");

}
