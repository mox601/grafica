/*
 *  texture.c
 *  
 *
 *  Created by Matteo on 28/09/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "texture.h"
#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>  

//#include <GL/glut.h>


#define TEXTURE_COUNT 1

// Texture objects

GLuint  textures[TEXTURE_COUNT];
const char *szTextureFiles[TEXTURE_COUNT] = {
	"textures/moon2.ppm"
		};
	
	

void loadTextures();

void setTexture(int texturename);


void setTexture(int texturename) 
{
	
	glBindTexture(GL_TEXTURE_2D, textures[texturename]);

}


void loadTextures() {

	
	int loop;
	int i,val;
	char temp[100];
	char img_type; // Temporary Storage
	int texwidth, texheight; // Texture Dimensions
	unsigned char *texdata; // Texture data
	FILE* f;

	
	//glEnable(GL_TEXTURE_2D);

	// Load textures
    glGenTextures(TEXTURE_COUNT, textures);

	
    printf("\n");
	
	
    for(loop=0; loop<TEXTURE_COUNT; loop++) {

		// Bind to next texture object
    
        glBindTexture(GL_TEXTURE_2D, textures[loop]);
		
		
        f=fopen(szTextureFiles[loop],"rt");
	
		do fgets(temp,100,f);while (temp[0]=='#'); 
   		img_type = temp[1]; // Store the type
	
		do fgets(temp,100,f);while (temp[0]=='#'); 
		sscanf(temp,"%d %d",&texwidth,&texheight); // Store Dimensions
		
		printf("loading texture n.%d (%dpx*%dpx)...\n",loop+1,texwidth,texheight);
	
		do fgets(temp,100,f);while (temp[0]=='#'); 
		texdata=(unsigned char*)malloc(sizeof(char)*texwidth*texheight*3);
	
   		if (img_type == '6') { // what kind?
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

	printf("\n");

	glDisable(GL_TEXTURE_2D);
}

