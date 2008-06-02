#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     
#include <GL/glut.h>  

#ifndef M_PI
#define M_PI 3.14159265
#endif

#ifdef WIN32
#pragma warning(disable:4305 4244)
#endif

#define min2(a,b) ((a)<=(b)?(a):(b))
#define max2(a,b) ((a)>=(b)?(a):(b))

/* prototype */
static void redraw(void);
static void key(unsigned char c, int x, int y);
static void controlMenu(int value);
static void refreshPos(void);

int mtextures=256;
int ntextures;
GLuint textures[256];
int curtexture;

GLfloat angley=0.0f;
GLfloat distance=1;

/* ---------------------------------------------------------- */
GLuint loadTexture(char* filename)
{
	GLuint textureid=(GLuint)-1;

#define LINE_WIDTH 1024

	int i,val;
	char temp[LINE_WIDTH];
	char img_type;					// Temporary Storage
	int texwidth, texheight;	// Texture Dimensions
	unsigned char *texdata;		// Texture data
	FILE* f=fopen(filename,"rb");
	
	do fgets(temp,LINE_WIDTH,f);while (temp[0]=='#'); 
	img_type = temp[1]; // Store the type
	
	do fgets(temp,LINE_WIDTH,f);while (temp[0]=='#'); 
	sscanf(temp,"%d %d",&texwidth,&texheight); // Store Dimensions
	
	do fgets(temp,LINE_WIDTH,f);while (temp[0]=='#'); 
	
	texdata=(unsigned char*)malloc(sizeof(unsigned char)*texwidth*texheight*3);
	
	if (img_type == '6') // what kind?
	{
		int imagesize=3*texwidth*texheight;
		int remaining=imagesize;
		int cont=0;

		while (remaining)
		{
			int nread=fread(texdata+cont,1,remaining,f);
			remaining-=nread;
			cont     +=nread;
		}

		if (cont!=imagesize)
		{
			printf("error reading file\n");
			exit(-1);
		}
	}
	else if(img_type == '3')
	{
		for(i=0;i<texwidth*texheight*3;i++)
		{ 
			fscanf(f,"%d",&val);
			texdata[i] = val;
		}
	}
	else 
	{
		printf("error reading PPM file\n");
		exit(0); // cant read other PNMs
	}
	
	fclose(f); // Close File
	
	glGenTextures(1,&textureid);
	glBindTexture(GL_TEXTURE_2D, textureid);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texwidth, texheight, GL_RGB, GL_UNSIGNED_BYTE, texdata);
	free(texdata);

	return textureid;
}


/* ---------------------------------------------------------- */
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(800,800);
	glutCreateWindow("OpenGl application");
	
	glutDisplayFunc(redraw);
	glutKeyboardFunc(key);
	glutIdleFunc (refreshPos);

	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	
	ntextures=0;
	curtexture=0;
	textures[ntextures++]=loadTexture("earthmap.ppm");
	textures[ntextures++]=loadTexture("earthmap2.ppm");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 
		/* field of view in degree */ 40.0,
		/* aspect ratio */ 1.0,
		/* Z near */ 0.1, /* Z far */ 100.0);
	

	glutMainLoop();
	return 0; 
}


/* ---------------------------------------------------------- */
static void key(unsigned char c, int x, int y)
{
  if (c == 27) exit(0);
  
	if (c=='+')       distance-=0.01;
	if (c=='-')       distance+=0.01;

	if (c=='t')       curtexture=(curtexture+1)%ntextures;

	if (distance<0.1) distance=0.1;

  glutPostRedisplay();
}

/* ---------------------------------------------------------- */
void getPointOnSphere(float dest[3],float radius,float u,float v)
{
	u*=2*M_PI;
	v=-1*(v-0.5)*M_PI;
	dest[0]=radius*cos(u)*cos(v);
	dest[1]=radius*sin(u)*cos(v);
	dest[2]=radius*sin(v);
}


/* ---------------------------------------------------------- */
void drawSphere(float radius,int nx,int ny)
{
	int i,j;
	GLfloat p0[3],p1[3],p2[3],p3[3];
	GLfloat u0,u1,v0,v1;
	GLfloat stepx=1/(float)nx;
	GLfloat stepy=1/(float)ny;

	glBegin(GL_TRIANGLES);
	
	for (j=0;j<ny;j++)
	{
		for (i=0;i<nx;i++)
		{
			u0=i*stepx;u1=(i+1)*stepx;
			v0=j*stepy;v1=(j+1)*stepy;
			
			getPointOnSphere(p0,radius,u0,v0);
			getPointOnSphere(p1,radius,u1,v0);
			getPointOnSphere(p2,radius,u1,v1);
			getPointOnSphere(p3,radius,u0,v1);
			
			glTexCoord2f(u0,v0);glNormal3fv(p0);glVertex3fv(p0);
			glTexCoord2f(u1,v0);glNormal3fv(p1);glVertex3fv(p1);
			glTexCoord2f(u1,v1);glNormal3fv(p2);glVertex3fv(p2);
			
			glTexCoord2f(u0,v0);glNormal3fv(p0);glVertex3fv(p0);
			glTexCoord2f(u1,v1);glNormal3fv(p2);glVertex3fv(p2);
			glTexCoord2f(u0,v1);glNormal3fv(p3);glVertex3fv(p3);
		}
	}
		
	glEnd();
}


/* ---------------------------------------------------------- */
void drawWireSphere(float radius,int nx,int ny)
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	drawSphere(radius,nx,ny);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}


/* ---------------------------------------------------------- */
void drawSolidSphere(float radius,int nx,int ny)
{
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	drawSphere(radius,nx,ny);
}


/* ---------------------------------------------------------- */
static void redraw(void)
{
	GLfloat colorwhite []  = {1.0f, 1.0f, 1.0f, 1.0f}; 
	GLfloat light_position[4]={cos(M_PI/4)*(distance+1),0.0,sin(M_PI/4)*(distance+1),1};

	glClearColor(0,0.4f,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		cos(M_PI/4)*(distance+1),0.0,sin(M_PI/4)*(distance+1), 
		0.0,0.0,0.9,  
		0.0,0.0,1.0); 

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, colorwhite );
	glEnable (GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glPushMatrix();

		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		glColor3f(1,0,0);glVertex3f(0,0,0);glVertex3f(2,0,0);
		glColor3f(0,1,0);glVertex3f(0,0,0);glVertex3f(0,2,0);
		glColor3f(0,0,1);glVertex3f(0,0,0);glVertex3f(0,0,2);
		glEnd();

		glRotatef(angley,0,1,0);
		
		glBindTexture(GL_TEXTURE_2D,textures[curtexture]);
		glEnable(GL_TEXTURE_2D);
		glColor3f(1,1,1);
		glEnable(GL_LIGHTING);
		glEnable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset(1,1);
		drawSolidSphere(1,64,64);
		glDisable(GL_TEXTURE_2D);
		glDisable( GL_POLYGON_OFFSET_FILL );

	glPopMatrix();
	glutSwapBuffers();
}

/* ---------------------------------------------------------- */
static void refreshPos(void)
{
	angley+=0.5;
	glutPostRedisplay();
}
