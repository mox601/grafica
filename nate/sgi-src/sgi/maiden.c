/* 
   maiden.c
   Nate Robins, 1997

   A killer "Iron Maiden rocks out with OpenGL" demo (according to
   Mark Kilgard).

*/


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined (__APPLE__) || defined (MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#if !defined(GL_VERSION_1_1)
#define glBindTexture glBindTextureEXT
#define glAreTexturesResident glAreTexturesResidentEXT
#else
#define GL_REPLACE_EXT GL_REPLACE
#endif


#define Ri  4				/* inner radius of torus */
#define Ro  8				/* outer radius of torus */

#define COLORS 12
#define color(c)  glColor3ubv(colors[COLORS/num_spheres*c])
GLubyte colors[COLORS][3] = {
    { 255, 0, 0 },
    { 255, 128, 0 },
    { 255, 255, 0 },
    { 128, 255, 0 },
    { 0, 255, 0 },
    { 0, 255, 128 },
    { 0, 255, 255 },
    { 0, 128, 255 },
    { 0, 0, 255 },
    { 128, 0, 255 },
    { 255, 0, 255 },
    { 255, 0, 128 },
};

typedef struct _star {
    float x, y;
    float vx, vy;
} star;

#define num_stars 500
star stars[num_stars];

GLubyte* background;

int     lod = 24;			/* level of detail */
GLfloat spin_y = 0;
GLfloat spin_x = 0;
GLfloat spin_z = 0;

int   num_spheres = 12;
GLint num_textures = 4;
GLenum mode = GL_MODULATE;		/* modulate, decal */
GLenum filter = GL_LINEAR;		/* texture filtering mode */

GLboolean drawbackground = GL_TRUE;	/* draw background image? */
GLboolean drawstars = GL_TRUE;		/* draw stars? */
GLboolean texturing = GL_TRUE;		/* texturing? */
GLboolean perftiming = GL_FALSE;		/* performance timing? */
GLboolean frozen = GL_FALSE;		/* animation frozen? */

GLuint width, height;
GLint bgtexture = 1;

char texnames[4][64] = {
    "ppm/deadone-256x256.ppm",
    "ppm/virus-256x256.ppm",
    "ppm/ace-256x256.ppm",
    "ppm/space-256x256.ppm",
};

/* ppmRead: read a PPM raw (type P6) file.  The PPM file has a header
   that should look something like:

   P6
   # comment
   width height max_value
   rgbrgbrgb...

   where "P6" is the magic cookie which identifies the file type and
   should be the only characters on the first line followed by a
   carriage return.  Any line starting with a # mark will be treated
   as a comment and discarded.  After the magic cookie, three integer
   values are expected: width, height of the image and the maximum
   value for a pixel (max_value must be < 256 for PPM raw files).  The
   data section consists of width*height rgb triplets (one byte each)
   in binary format (i.e., such as that written with fwrite() or
   equivalent).

   The rgb data is returned as an array of unsigned chars (packed
   rgb).  The malloc()'d memory should be free()'d by the caller.  If
   an error occurs, an error message is sent to stderr and NULL is
   returned.

*/
unsigned char* 
ppmRead(char* filename, int* width, int* height)
{
    FILE* fp;
    int i, w, h, d;
    unsigned char* image;
    char head[70];			/* max line <= 70 in PPM (per spec). */

    fp = fopen(filename, "rb");
    if (!fp) {
        perror(filename);
        return NULL;
    }

    /* grab first two chars of the file and make sure that it has the
       correct magic cookie for a raw PPM file. */
    fgets(head, 70, fp);
    if (strncmp(head, "P6", 2)) {
        fprintf(stderr, "%s: Not a raw PPM file\n", filename);
        return NULL;
    }

    /* grab the three elements in the header (width, height, maxval). */
    i = 0;
    while(i < 3) {
        fgets(head, 70, fp);
        if (head[0] == '#')		/* skip comments. */
            continue;
        if (i == 0)
            i += sscanf(head, "%d %d %d", &w, &h, &d);
        else if (i == 1)
            i += sscanf(head, "%d %d", &h, &d);
        else if (i == 2)
            i += sscanf(head, "%d", &d);
    }

    /* grab all the image data in one fell swoop. */
    image = (unsigned char*)malloc(sizeof(unsigned char)*w*h*3);
    fread(image, sizeof(unsigned char), w*h*3, fp);
    fclose(fp);

    *width = w;
    *height = h;
    return image;
}

void
sphere(GLuint texture)
{
    if (texturing) {
        glBindTexture(GL_TEXTURE_2D, texture);
        glutSolidSphere(Ri, lod, lod);
    } else {
        glutSolidSphere(Ri, lod, lod);
    }
}

void
textures(void)
{
    int w, h, i;
    GLubyte* texture;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /* XXX - RE bug - must enable texture before bind. */
    glEnable(GL_TEXTURE_2D);

    for (i = 0; i < 4; i++) {
        glBindTexture(GL_TEXTURE_2D, i+1);
        texture = (GLubyte*)ppmRead(texnames[i], &w, &h);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, w, h,
            GL_RGB, GL_UNSIGNED_BYTE, texture);
        free(texture);
    }

    /* XXX - RE bug - must enable texture before bind. */
    glDisable(GL_TEXTURE_2D);
}

void
init(void)
{
    GLfloat Ka[] = { 0.2f, 0.2f, 0.2f, 1 };
    GLfloat Ks[] = { 1, 1, 1, 1 };

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glMaterialf(GL_FRONT, GL_SHININESS, 64);
    glMaterialfv(GL_FRONT, GL_SPECULAR, Ks);
    glMaterialfv(GL_FRONT, GL_AMBIENT, Ka);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
}

void
reshape(int w, int h)
{
    int i;

    width = w; height = h;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(120, (GLfloat)width/height, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);

    for (i = 0; i < num_stars; i++) {
        stars[i].x = rand() % width;
        stars[i].y = rand() % height;
        stars[i].vx = rand() / (float)RAND_MAX * 5 + 2;
        stars[i].vy = 0;
    }
}

void
display(void)
{
    static int i, start, last, end, step;
    static char s[16], *p;

    if (perftiming)
        start = glutGet(GLUT_ELAPSED_TIME);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (drawbackground || drawstars || perftiming) {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, width, 0, height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);

        if (drawbackground) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, bgtexture);
            glColor3ub(255, 255, 255);
            glBegin(GL_QUADS);
            glTexCoord2i(0, 0);
            glVertex2i(0, 0);
            glTexCoord2i(1, 0);
            glVertex2i(width, 0);
            glTexCoord2i(1, 1);
            glVertex2i(width, height);
            glTexCoord2i(0, 1);
            glVertex2i(0, height);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }

        if (drawstars) {
            glEnable(GL_BLEND);
            glBegin(GL_LINES);
            for (i = 0; i < num_stars; i++) {
                stars[i].x += stars[i].vx;
                if (stars[i].x < width) {
                    glColor4ub(0, 0, 0, 0);
                    glVertex2i(stars[i].x-stars[i].vx*3, stars[i].y);
                    glColor4ub(255, 255, 255, 255);
                    glVertex2i(stars[i].x, stars[i].y);
                } else {
                    stars[i].x = 0;
                }
            }
            glEnd();
            glDisable(GL_BLEND);
        }

        if (perftiming) {
            glColor3ub(255, 255, 255);
            sprintf(s, "%.1f", 1.0/((float)(end-last)/1000.0));
            glRasterPos2i(5, 5);
            for (p = s; *p; p++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
            last = start;
        }

        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }

    glPushMatrix();

    if (texturing) {
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glEnable(GL_TEXTURE_2D);
    }

    glRotatef(spin_y, 0, 1, 0);
    glColor3ub(196, 196, 196);
    glutSolidTorus(Ri, Ro, lod, lod);

    step = 360.0/num_spheres;
    for (i = 0; i < num_spheres; i++) {
        glPushMatrix();
        glRotatef(step*i+spin_z, 0, 0, 1);
        glTranslatef(0, Ro, 0);
        glRotatef(step*i+spin_x, 1, 0, 0);
        glTranslatef(0, Ri+Ri, 0);
        color(i);
        sphere(i%num_textures+1);
        glPopMatrix();
    }

    if (texturing) {
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();

    glutSwapBuffers();

    if (perftiming) {
        end = glutGet(GLUT_ELAPSED_TIME);
    }
}

int
pixels(void)
{
    int i, j, n, values;
    GLfloat* buffer;
    GLfloat ax, ay, bx, by, area, acc = 0;

    /*
      calculate the size of the feedback buffer:
      lod*lod*2 = number of triangles in the torus
      lod*2 = number of triangles in each cap of a sphere
      lod*(lod-2)*2 = number of triangles in latitudinal strips of a sphere
      *8 = 3 vertices (2 values each) + polygon token and a vertex count
      num_stars*5 = line token + 2 vertices (2 values each)
      5*3 = (possibly 5) bitmap tokens + 1 vertex each (2 values each)
    */
    buffer = (GLfloat*)malloc(sizeof(GLfloat)*((lod*lod*2+
                (lod*2+lod*(lod-2)*2)*num_spheres)*8+
            num_stars*5+5*3));
    glFeedbackBuffer((lod*lod*2+(lod*2+lod*(lod-2)*2)*num_spheres)*8+
        num_stars*5+5*3, GL_2D, buffer);
    glRenderMode(GL_FEEDBACK);
    display();
    values = glRenderMode(GL_RENDER);
    i = 0;
    while (i < values) {
        if (buffer[i] == GL_POLYGON_TOKEN) {
            i++; n = buffer[i]; i++;
            for (j = 0; j < n-2; j++) {
                ax = buffer[i+2+2*j]-buffer[i+0]; 
                ay = buffer[i+3+2*j]-buffer[i+1];
                bx = buffer[i+4+2*j]-buffer[i+0]; 
                by = buffer[i+5+2*j]-buffer[i+1];
                area = ax*by-bx*ay;
                acc += area < 0 ? -area : area; /* -area= backfacing polygon */
                i += n*2;
            }
        } else if (buffer[i] == GL_LINE_RESET_TOKEN) {
            i++;
            /* XXX assume left-to-right horizontal lines */
            acc += buffer[i+2]-buffer[i+0];
            i += 4;
        } else if (buffer[i] == GL_BITMAP_TOKEN) {
            i++;
            /* XXX skip past bitmap tokens */
            i += 2;
        } else {
            printf("unknown token found 0x%x at %d!\n", (int)buffer[i], i);
            i++;
        }
    }
    free(buffer);
    
    acc /= 2.0;

    return (int)acc;
}

void
idle(void)
{
    if (!frozen) {
        spin_y += 0.5;
        if (spin_y > 360) spin_y -= 360;
        spin_x += 10;
        if (spin_x > 360) spin_x -= 360;
        spin_z += 1;
        if (spin_z > 360) spin_z -= 360;
    }

    glutPostRedisplay();
}

void
keyboard(unsigned char key, int x, int y)
{
    int i;

    switch (key) {
    case 27:
        free(background);
        exit(0);
        break;
    case 'p':
        perftiming = !perftiming;
        break;
    case 't':
        texturing = !texturing;
        break;
    case 'm':
        if (mode == GL_REPLACE_EXT)
            mode = GL_MODULATE;
        else if (mode == GL_MODULATE)
            mode = GL_REPLACE_EXT;
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
        printf("%s mode.\n", mode == GL_MODULATE ? "GL_MODULATE":"GL_REPLACE");
        break;
    case 'f':
        if (filter == GL_NEAREST)
            filter = GL_LINEAR;
        else 
            filter = GL_NEAREST;
        for (i = 0; i < num_textures; i++) {
            glBindTexture(GL_TEXTURE_2D, i+1);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        }
        printf("%s filtering.\n", filter==GL_LINEAR?"GL_LINEAR":"GL_NEAREST");
        break;
    case '>':
        num_textures++;
        if (num_textures > 4) num_textures = 4;
        break;
    case '<':
        num_textures--;
        if (num_textures < 1) num_textures = 1;
        break;
    case 'b':
        drawbackground = !drawbackground;
        break;
    case '*':
        drawstars = !drawstars;
        break;
    case 'r':
        printf("%d triangles, %d pixels.\n",
            lod*lod*2 + (lod*2 + lod*(lod-2)*2) * num_spheres, pixels());
        /* ^-- torus    ^-- caps    ^-- body */
        break;
    case 'c':
        if (glIsEnabled(GL_CULL_FACE))
            glDisable(GL_CULL_FACE);
        else
            glEnable(GL_CULL_FACE);
        printf("Culling %s.\n",glIsEnabled(GL_CULL_FACE)?"enabled":"disabled");
        break;
    case ' ':
        frozen = !frozen;
        break;
    case '1':
        bgtexture = 1;
        break;
    case '2':
        bgtexture = 2;
        break;
    case '3':
        bgtexture = 3;
        break;
    case '4':
        bgtexture = 4;
        break;
    }
}

void
special(int value, int x, int y)
{
    switch (value) {
    case GLUT_KEY_UP:
        lod++;
        if (lod > 32) lod = 32;
        break;
    case GLUT_KEY_DOWN:
        lod--;
        if (lod < 3) lod = 3;
        break;
    case GLUT_KEY_RIGHT:
        num_spheres++;
        if (num_spheres > COLORS) num_spheres = COLORS;
        break;
    case GLUT_KEY_LEFT:
        num_spheres--;
        if (num_spheres < 1) num_spheres = 1;
        break;
    }
}

void
menu(int value)
{
    keyboard((unsigned char)value, 0, 0);
}

int
main(int argc, char** argv)
{
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGBA|GLUT_MULTISAMPLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(800, 600);
    glutInit(&argc, argv);

    glutCreateWindow("Maiden");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutCreateMenu(menu);
    glutAddMenuEntry("Toggle texture mapping", 't');
    glutAddMenuEntry("Toggle texture mode", 'm');
    glutAddMenuEntry("Toggle filter mode", 'f');
    glutAddMenuEntry("Toggle performance", 'p');
    glutAddMenuEntry("Toggle background", 'b');
    glutAddMenuEntry("Toggle animation", ' ');
    glutAddMenuEntry("Toggle culling", 'c');
    glutAddMenuEntry("Toggle stars", '*');
    glutAddMenuEntry("Time full frame (no swap)", 'n');
    glutAddMenuEntry("Print pixels/frame", 'r');
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("> and < keys change # of textures", 0);
    glutAddMenuEntry("Arrows up/down change level of detail", 0);
    glutAddMenuEntry("Arrows right/left change # of spheres", 0);
    glutAddMenuEntry("1-4 keys change background image", 0);
    glutAddMenuEntry("", 0);
    glutAddMenuEntry("Quit", 'r');
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    textures();

    init();
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
