#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>

#include "simpleviewer.h"
#include "trackball.h"


/* ply functions */
int  open_ply   (const char* filename);
void display_ply(int viewmode);

/* prototypes */
void keydown(unsigned char key, GLint x, GLint y) ;
void display(void);
void mouse(GLint button, GLint state, GLint x, GLint y);
void motion(GLint x, GLint y);
void reshape(GLint W,GLint H);

/* ------------- */
/* viewer status */
/* ------------- */

/* actual operation  */
static int scaling;
static int moving;
static int panning;

/* starting "moving" coordinates */
static int beginx,beginy;

/* current transformation matrix */
static float curmat[4][4];

/* ortho */
float ortho_left,ortho_right,ortho_bottom,ortho_top;

/* current scale factor */
static float scalefactor;

/* viewmode 0==filled face 1==filled+wireframe 2=wireframe */
int viewmode=0;

/* trackball data */
static float curquat[4],lastquat[4];


int enable_texture=0;
GLuint  mytexture1d=(GLuint )0;
GLubyte palette[3*256];


void setPalette()
{
	int i,buf[256];

	buf[0] = 0x7e0000; buf[1] = 0x830000; buf[2] = 0x870000; buf[3] = 0x8b0000; 
		buf[4] = 0x8f0000; buf[5] = 0x930000; buf[6] = 0x970000; buf[7] = 0x9b0000; 
		buf[8] = 0x9f0000; buf[9] = 0xa30000; buf[10] = 0xa70000; buf[11] = 0xab0000; 
		buf[12] = 0xaf0000; buf[13] = 0xb30000; buf[14] = 0xb70000; buf[15] = 0xbb0000; 
		buf[16] = 0xbf0000; buf[17] = 0xc30000; buf[18] = 0xc70000; buf[19] = 0xcb0000; 
		buf[20] = 0xcf0000; buf[21] = 0xd30000; buf[22] = 0xd70000; buf[23] = 0xdb0000; 
		buf[24] = 0xdf0000; buf[25] = 0xe30000; buf[26] = 0xe70000; buf[27] = 0xeb0000; 
		buf[28] = 0xef0000; buf[29] = 0xf30000; buf[30] = 0xf70100; buf[31] = 0xfb0200; 
		buf[32] = 0xff0300; buf[33] = 0xff0500; buf[34] = 0xff0700; buf[35] = 0xff0a00; 
		buf[36] = 0xff0d00; buf[37] = 0xff1000; buf[38] = 0xff1300; buf[39] = 0xff1700; 
		buf[40] = 0xff1b00; buf[41] = 0xff1f00; buf[42] = 0xff2300; buf[43] = 0xff2700; 
		buf[44] = 0xff2b00; buf[45] = 0xff2f00; buf[46] = 0xff3300; buf[47] = 0xff3700; 
		buf[48] = 0xff3b00; buf[49] = 0xff3f00; buf[50] = 0xff4300; buf[51] = 0xff4700; 
		buf[52] = 0xff4b00; buf[53] = 0xff4f00; buf[54] = 0xff5300; buf[55] = 0xff5700; 
		buf[56] = 0xff5b00; buf[57] = 0xff5f00; buf[58] = 0xff6300; buf[59] = 0xff6700; 
		buf[60] = 0xff6b00; buf[61] = 0xff6f00; buf[62] = 0xff7300; buf[63] = 0xff7700; 
		buf[64] = 0xff7b00; buf[65] = 0xff7f00; buf[66] = 0xff8300; buf[67] = 0xff8600; 
		buf[68] = 0xff8a00; buf[69] = 0xff8e00; buf[70] = 0xff9200; buf[71] = 0xff9600; 
		buf[72] = 0xff9a00; buf[73] = 0xff9e00; buf[74] = 0xffa200; buf[75] = 0xffa600; 
		buf[76] = 0xffaa00; buf[77] = 0xffae00; buf[78] = 0xffb200; buf[79] = 0xffb600; 
		buf[80] = 0xffba00; buf[81] = 0xffbe00; buf[82] = 0xffc200; buf[83] = 0xffc600; 
		buf[84] = 0xffca00; buf[85] = 0xffce00; buf[86] = 0xffd200; buf[87] = 0xffd600; 
		buf[88] = 0xffda00; buf[89] = 0xffde00; buf[90] = 0xffe200; buf[91] = 0xffe600; 
		buf[92] = 0xffea00; buf[93] = 0xffee00; buf[94] = 0xfff200; buf[95] = 0xfff600; 
		buf[96] = 0xfff900; buf[97] = 0xfffd00; buf[98] = 0xfcff00; buf[99] = 0xf8ff02; 
		buf[100] = 0xf5ff07; buf[101] = 0xf1ff0a; buf[102] = 0xedff0f; buf[103] = 0xe9ff12; 
		buf[104] = 0xe5ff17; buf[105] = 0xe1ff1a; buf[106] = 0xddff1f; buf[107] = 0xd9ff22; 
		buf[108] = 0xd5ff27; buf[109] = 0xd1ff2b; buf[110] = 0xcdff2f; buf[111] = 0xc9ff32; 
		buf[112] = 0xc5ff37; buf[113] = 0xc1ff3b; buf[114] = 0xbdff3f; buf[115] = 0xb9ff42; 
		buf[116] = 0xb5ff47; buf[117] = 0xb1ff4b; buf[118] = 0xadff4f; buf[119] = 0xa9ff52; 
		buf[120] = 0xa4ff57; buf[121] = 0xa0ff5b; buf[122] = 0x9cff5f; buf[123] = 0x98ff62; 
		buf[124] = 0x94ff67; buf[125] = 0x90ff6b; buf[126] = 0x8cff6f; buf[127] = 0x88ff72; 
		buf[128] = 0x85ff77; buf[129] = 0x81ff7b; buf[130] = 0x7fff7f; buf[131] = 0x7bff81; 
		buf[132] = 0x77ff85; buf[133] = 0x72ff88; buf[134] = 0x6fff8c; buf[135] = 0x6bff90; 
		buf[136] = 0x67ff94; buf[137] = 0x62ff98; buf[138] = 0x5fff9c; buf[139] = 0x5bffa0; 
		buf[140] = 0x57ffa4; buf[141] = 0x52ffa9; buf[142] = 0x4fffad; buf[143] = 0x4bffb1; 
		buf[144] = 0x47ffb5; buf[145] = 0x42ffb9; buf[146] = 0x3fffbd; buf[147] = 0x3bffc1; 
		buf[148] = 0x37ffc5; buf[149] = 0x32ffc9; buf[150] = 0x2fffcd; buf[151] = 0x2bffd1; 
		buf[152] = 0x27ffd5; buf[153] = 0x22ffd9; buf[154] = 0x1fffdd; buf[155] = 0x1affe1; 
		buf[156] = 0x17ffe5; buf[157] = 0x12ffe9; buf[158] = 0xfffee; buf[159] = 0xafff2; 
		buf[160] = 0x7fff6; buf[161] = 0x2fffa; buf[162] = 0xfffe; buf[163] = 0xfbff; 
		buf[164] = 0xf7ff; buf[165] = 0xf3ff; buf[166] = 0xefff; buf[167] = 0xebff; 
		buf[168] = 0xe7ff; buf[169] = 0xe3ff; buf[170] = 0xdfff; buf[171] = 0xdbff; 
		buf[172] = 0xd7ff; buf[173] = 0xd3ff; buf[174] = 0xcfff; buf[175] = 0xcbff; 
		buf[176] = 0xc7ff; buf[177] = 0xc3ff; buf[178] = 0xbfff; buf[179] = 0xbbff; 
		buf[180] = 0xb7ff; buf[181] = 0xb3ff; buf[182] = 0xafff; buf[183] = 0xabff; 
		buf[184] = 0xa7ff; buf[185] = 0xa3ff; buf[186] = 0x9fff; buf[187] = 0x9bff; 
		buf[188] = 0x96ff; buf[189] = 0x93ff; buf[190] = 0x8fff; buf[191] = 0x8bff; 
		buf[192] = 0x87ff; buf[193] = 0x83ff; buf[194] = 0x7eff; buf[195] = 0x7aff; 
		buf[196] = 0x77ff; buf[197] = 0x72ff; buf[198] = 0x6fff; buf[199] = 0x6bff; 
		buf[200] = 0x67ff; buf[201] = 0x62ff; buf[202] = 0x5fff; buf[203] = 0x5bff; 
		buf[204] = 0x57ff; buf[205] = 0x53ff; buf[206] = 0x4fff; buf[207] = 0x4bff; 
		buf[208] = 0x47ff; buf[209] = 0x43ff; buf[210] = 0x3fff; buf[211] = 0x3bff; 
		buf[212] = 0x37ff; buf[213] = 0x33ff; buf[214] = 0x2fff; buf[215] = 0x2bff; 
		buf[216] = 0x27ff; buf[217] = 0x23ff; buf[218] = 0x1fff; buf[219] = 0x1bff; 
		buf[220] = 0x17ff; buf[221] = 0x13ff; buf[222] = 0x10ff; buf[223] = 0xdff; 
		buf[224] = 0xaff; buf[225] = 0x7ff; buf[226] = 0x5ff; buf[227] = 0x3fa; 
		buf[228] = 0x2f6; buf[229] = 0x1f1; buf[230] = 0xed; buf[231] = 0xe9; 
		buf[232] = 0xe4; buf[233] = 0xe0; buf[234] = 0xdb; buf[235] = 0xd7; 
		buf[236] = 0xd3; buf[237] = 0xce; buf[238] = 0xca; buf[239] = 0xc5; 
		buf[240] = 0xc1; buf[241] = 0xbd; buf[242] = 0xb8; buf[243] = 0xb4; 
		buf[244] = 0xaf; buf[245] = 0xab; buf[246] = 0xa7; buf[247] = 0xa2; 
		buf[248] = 0x9e; buf[249] = 0x99; buf[250] = 0x95; buf[251] = 0x91; 
		buf[252] = 0x8c; buf[253] = 0x88; buf[254] = 0x83; buf[255] = 0x7d; 


	for ( i=0;i<256;i++)
	{
		palette[i*3+0]=(buf[i]    ) & 0xff;
		palette[i*3+1]=(buf[i]>>8 ) & 0xff;
		palette[i*3+2]=(buf[i]>>16) & 0xff;
	}
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
int main(int narg,char** args)
{
	GLfloat light_ka[]={0,0,0,1};
	GLfloat light_kd[]={1,1,1,1};
	GLfloat light_ks[]={1,1,1,1};

	GLfloat material_ka[]={0.16f,0.12f,0.11f,1.00f};
	GLfloat material_kd[]={0.61f,0.57f,0.36f,1.00f};
	GLfloat material_ks[]={0.56f,0.55f,0.44f,1.00f};
	GLfloat material_ke[]={0.00f,0.00f,0.00f,0.00f};
	GLfloat material_se[]={16};

	if (narg!=2)
	{
		fprintf(stderr,"Syntax error. Use %s filename.ply\n",args[0]);
		return -1;
	}
	
	
	glutInit(&narg, args); 
	glutInitDisplayMode(GLUT_DEPTH|GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize(800,800);
	glutCreateWindow("Simple OpenGL viewer");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glDepthFunc(GL_LESS);
	glClearColor(0.4f,0.4f,1.0f,1.0f);

	glutReshapeFunc (reshape);
	glutKeyboardFunc(keydown);
	glutDisplayFunc (display);
	glutMouseFunc   (mouse);
	glutMotionFunc  (motion);

	glLightfv(GL_LIGHT0,GL_AMBIENT,light_ka);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light_kd);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light_ks);
	glEnable(GL_LIGHT0);

	glMaterialfv(GL_FRONT_AND_BACK ,GL_AMBIENT  ,material_ka);
	glMaterialfv(GL_FRONT_AND_BACK ,GL_DIFFUSE  ,material_kd);
	glMaterialfv(GL_FRONT_AND_BACK ,GL_SPECULAR ,material_ks);
	glMaterialfv(GL_FRONT_AND_BACK ,GL_EMISSION ,material_ke);
	glMaterialfv(GL_FRONT_AND_BACK ,GL_SHININESS,material_se);

	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,0);

	glEnable(GL_LIGHTING);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	scaling  = 0;
	moving   = 0;
	panning  = 0;
	beginx   = 0;
	beginy   = 0;

	matident(curmat);
	scalefactor=1;

	trackball(curquat , 0.0f, 0.0f, 0.0f, 0.0f);
	trackball(lastquat, 0.0f, 0.0f, 0.0f, 0.0f);	
	add_quats(lastquat, curquat, curquat);
	build_rotmatrix(curmat, curquat);

	if (!open_ply(args[1]))
	{
		fprintf(stderr,"Error opening %s file\n",args[1]);
		return -1;
	}


	glGenTextures(1,&mytexture1d);
	setPalette();
	glBindTexture(GL_TEXTURE_1D, mytexture1d);
	glTexImage1D(GL_TEXTURE_1D,0,3,256,0,GL_RGB,GL_UNSIGNED_BYTE,palette);

	glutMainLoop();
	return 0;
}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void keydown(unsigned char key, GLint x, GLint y) 
{
	switch(key) 
	{
	case 'l':case 'L':
		viewmode++;
		break;
	case 27:
		exit(0);
		return;

	case 't':case 'T':
		enable_texture=1-enable_texture;
		break;

	case 'F':
		glutFullScreen();
		break;

	case 's':case 'S': //reset matrix
		matident(curmat);
		break;
	}

	glutPostRedisplay();
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(ortho_left, ortho_right, ortho_bottom, ortho_top, -10, +10);

	glMatrixMode (GL_MODELVIEW);
	glPushMatrix();
	{
		glLoadIdentity();
		glMultMatrixf(&(curmat[0][0]));
		glScalef(scalefactor,scalefactor,scalefactor);

		/* your display function here! */
		if (enable_texture)
		{
			glBindTexture(GL_TEXTURE_1D, mytexture1d);
			glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP );
			glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP );
			glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glEnable(GL_TEXTURE_1D);
		}

		display_ply(viewmode % 3);
		
		if (enable_texture)
			glDisable(GL_TEXTURE_1D);
	}
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);

	glutSwapBuffers();
}



/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void reshape(int W,int H)
{
	ortho_left  =-1;
	ortho_right =+1;

	/* mantain factor to prevent distortion */
	ortho_bottom=-(ortho_right-ortho_left)*((float)H/W)/2.0f;
	ortho_top   =-1*ortho_bottom;

	glViewport(0,0,W,H);
	glutPostRedisplay();
}



/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void mouse(GLint button, GLint state, GLint x, GLint y)
{
	if (state==GLUT_UP)
	{
		moving=scaling=panning=0;
		glutPostRedisplay();
		return;
	}

	switch(button)
	{
		case GLUT_RIGHT_BUTTON : scaling=1                                ;break;
		case GLUT_LEFT_BUTTON  : moving=1 ;trackball(lastquat, 0, 0, 0, 0);break;
		case GLUT_MIDDLE_BUTTON: panning=1                                ;break;
	}
	
	beginx = x;beginy = y;
	glutPostRedisplay();
}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void motion(GLint x, GLint y)
{
	int W=glutGet(GLUT_WINDOW_WIDTH );
	int H=glutGet(GLUT_WINDOW_HEIGHT);

	float dx=(beginx-x)/(float)W;
	float dy=(y-beginy)/(float)H;

	if (panning)
	{
		ortho_left  +=dx;
		ortho_right +=dx;
		ortho_bottom+=dy;
		ortho_top   +=dy;

		beginx = x;beginy = y;
		glutPostRedisplay();
		return;
	}
	else if (scaling) 
	{
		scalefactor *= (1.0f+dx);

		beginx = x;beginy = y;
		glutPostRedisplay();
		return;
	}
	else if (moving) 
	{
		trackball(lastquat,
			(2.0f * beginx - W) / W,
			(H - 2.0f * beginy) / H,
			(2.0f * x - W) / W,
			(H - 2.0f * y) / H
		);

		add_quats(lastquat, curquat, curquat);
		build_rotmatrix(curmat, curquat);

		beginx = x;beginy = y;
		glutPostRedisplay();
		return;
	}
}






