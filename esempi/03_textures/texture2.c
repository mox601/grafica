#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>     
#include <GL/glut.h>  


#ifndef M_PI
#define M_PI 3.14159265
#endif


/* prototype */
static void redraw(void);
static void mouse(int button, int state, int x, int y);
static void motion(int x, int y);
static void key(unsigned char c, int x, int y);
static void controlMenu(int value);


void makeFloorTexture(void);

enum {
  M_NONE
};


int moving, startx, starty;
GLfloat anglex = 0;   
GLfloat angley = 0;   


void loadTexture(char* filename);

/* ---------------------------------------------------------- */
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize(800,800);
	glutCreateWindow("OpenGl application");
	
	glutDisplayFunc(redraw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(key);
	
	glutCreateMenu(controlMenu);
	
	glutAddMenuEntry("-----------------------", M_NONE);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	
	glEnable(GL_DEPTH_TEST);
	glLineWidth(3.0);
	
	glMatrixMode(GL_PROJECTION);
	gluPerspective( 
		/* field of view in degree */ 40.0,
		/* aspect ratio */ 1.0,
		/* Z near */ 1.0, /* Z far */ 100.0);
		
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(
		0.0, 0.0, 10.0,  /* eye  */
		0.0, 0.0, 0.0,  /* center  */
		1.0, 0.0, 0.0); /* up is in positive Y direction */
	
	loadTexture("esempio.ppm");

	glutMainLoop();
	return 0; /* ANSI C requires main to return int. */
}


void loadTexture(char* filename)
{
	int i,val;
	char temp[100];
	char img_type; // Temporary Storage
	int texwidth, texheight; // Texture Dimensions
	unsigned char *texdata; // Texture data
	FILE* f=fopen(filename,"rt");

	do fgets(temp,100,f);while (temp[0]=='#'); 
   img_type = temp[1]; // Store the type

	do fgets(temp,100,f);while (temp[0]=='#'); 
	sscanf(temp,"%d %d",&texwidth,&texheight); // Store Dimensions

	do fgets(temp,100,f);while (temp[0]=='#'); 

   texdata=(char*)malloc(sizeof(char)*texwidth*texheight*3);

   if (img_type == '6') // what kind?
	{
		fread(texdata,sizeof(char)*3,texwidth*texheight,f);
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
		exit(0); // cant read other PNMs

	fclose(f); // Close File
	
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texwidth, texheight, GL_RGB, GL_UNSIGNED_BYTE, texdata);

	free(texdata);
}


/* ---------------------------------------------------------- */
static void key(unsigned char c, int x, int y)
{
  if (c == 27) exit(0); 
  glutPostRedisplay();
}


/* ---------------------------------------------------------- */
static void controlMenu(int value)
{
	switch (value) 
	{
		case M_NONE:return;	
	}
	glutPostRedisplay();
}


/* ---------------------------------------------------------- */
static void redraw(void)
{
	glClearColor(0,0.4f,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	
		glRotatef(anglex, 1.0, 0.0, 0.0);
		glRotatef(angley, 0.0, 1.0, 0.0);

		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);glVertex3f(-1,-1,0);
			glTexCoord2f(0.0, 1.0);glVertex3f(+1,-1,0);
			glTexCoord2f(1.0, 1.0);glVertex3f(+1,+1,0);
			glTexCoord2f(1.0, 0.0);glVertex3f(-1,+1,0);
		glEnd();
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	
	glutSwapBuffers();
}

/* ---------------------------------------------------------- */
static void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON) 
  {
    if (state == GLUT_DOWN) 
	 {
      moving = 1;
      startx = x;starty = y;
    }

    if (state == GLUT_UP) 
      moving = 0;
  }
}

/* ---------------------------------------------------------- */
static void motion(int x, int y)
{
  if (moving) 
  {
    anglex = anglex  + (x - startx);
    angley = angley + (y - starty);
    startx = x;starty = y;
    glutPostRedisplay();
  }
}



