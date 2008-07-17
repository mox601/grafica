//#include <GL/glut.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif



#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "ply.h"


/* definition of a triangle as 3 indeces to a vector of float containing vertices infos */
struct triangle_t
{
	int i0,i1,i2;
	struct triangle_t* next;
};

struct vertex_t
{
	float x,y,z;		/* world coordinates */
	float nx,ny,nz;	/* normal */
};

static struct 
{
	/* number of vertices */
	int   numvertices; 

	/* pointer to vertices */
	struct vertex_t* vertices;

	/* pointer to first triangle */
	struct triangle_t* triangles; 
} 
mesh;


#define swap_int(a,b) {int _tmp=(a);(a)=(b);(b)=_tmp;}
#define min2(a,b) (((a)<=(b))?(a):(b))
#define max2(a,b) (((a)>=(b))?(a):(b))



/*======================================================= */
/*======================================================= */
void set_normals()
{
	int   k;
	float x,y,z;
	float x0,y0,z0;
	float x1,y1,z1;
	float len;

	struct triangle_t* cursor=mesh.triangles;

	for (k=0;k<mesh.numvertices;k++)
	{
		struct vertex_t* v=mesh.vertices+k;
		v->nx=0;
		v->ny=0;
		v->nz=0;
	}

	while (cursor)
	{
		struct vertex_t* v0=mesh.vertices+cursor->i0;
		struct vertex_t* v1=mesh.vertices+cursor->i1;
		struct vertex_t* v2=mesh.vertices+cursor->i2;

		/* cross product */
		x0 = v2->x - v0->x;y0 = v2->y - v0->y;z0 = v2->z - v0->z;
		x1 = v1->x - v0->x;y1 = v1->y - v0->y;z1 = v1->z - v0->z;

		x = y0 * z1 - z0 * y1;
		y = z0 * x1 - x0 * z1;
		z = x0 * y1 - y0 * x1;

		len = (float)sqrt(x*x + y*y + z*z);
		x/=len;
		y/=len;
		z/=len;

		v0->nx+=x;v1->nx+=x;v2->nx+=x;
		v0->ny+=y;v1->ny+=y;v2->ny+=y;
		v0->nz+=z;v1->nz+=z;v2->nz+=z;

		cursor=cursor->next;
	}

	for (k=0;k<mesh.numvertices;k++)
	{
		struct vertex_t* v=mesh.vertices+k;
		len=(float)sqrt(v->nx*v->nx+v->ny*v->ny+v->nz*v->nz);
		v->nx /= len;
		v->ny /= len;
		v->nz /= len;

	}
}

/*======================================================= */
/*======================================================= */

int open_ply(const char* filename)
{
	/* bounding box */
	int I;
	struct triangle_t* t,**cursor;
	float x1=+1e18f,x2=-1e18f;
	float y1=+1e18f,y2=-1e18f;
	float z1=+1e18f,z2=-1e18f;

	PlyProperty vert_prop[3]={
		{    "x", Float32, Float32,  0, 0, 0, 0, 0},
		{    "y", Float32, Float32,  4, 0, 0, 0, 0},
		{    "z", Float32, Float32,  8, 0, 0, 0, 0}
	};

	PlyProperty   face_prop	={"vertex_indices", Int32, Int32, 4,PLY_LIST, Int32, Int32, 0};

	PlyProperty** props=NULL;
	int numvertices,numproperties,numtriangles,numstrips,numfaces;
	int clockwise;
	struct {int nverts,*verts;} face;
	int h,k;
	float maxdim;
	PlyFile* ply=0;

	FILE* file = fopen( filename, "rb" );

	if (!file)
		return 0;

	ply = read_ply(file);

	if (!ply)
	{
		fclose(file);
		return 0;
	}

	cursor=&(mesh.triangles);

	props = get_element_description_ply(ply, "vertex", &numvertices, &numproperties);
	
	mesh.vertices=(struct vertex_t*)malloc(sizeof(struct vertex_t)*numvertices);
	mesh.numvertices=numvertices;

	get_element_setup_ply(ply, "vertex", 3,vert_prop);
	
	for( k=0;k<numvertices; ++k) 
	{
		struct vertex_t* v=mesh.vertices+k;
		get_element_ply( ply, (void*)v);
		
		x1=min2(x1,v->x);x2=max2(x2,v->x);
		y1=min2(y1,v->y);y2=max2(y2,v->y);
		z1=min2(z1,v->z);z2=max2(z2,v->z);
	}

	/* normalize to unit box to [-1,+1],[-1,+1],[-1,+1] mantaining proportions */
	maxdim=max2(x2-x1,y2-y1);
	maxdim=max2(maxdim,z2-z1);

	for( k=0;k<numvertices; ++k) 
	{
		struct vertex_t* v=mesh.vertices+k;
		v->x=2*((v->x-x1)/maxdim-0.5f);
		v->y=2*((v->y-y1)/maxdim-0.5f);
		v->z=2*((v->z-z1)/maxdim-0.5f);
	}

	numtriangles=0;

	if (props = get_element_description_ply( ply, "face", &numfaces, &numproperties))
	{
		get_element_setup_ply( ply, "face", 1, &face_prop);
		
		for(h=0; h<numfaces; h++ ) 
		{
			get_element_ply( ply, (void*)&face);
						
			for(k=2; k<face.nverts;++k) 
			{
				t=(struct triangle_t*)malloc(sizeof(struct triangle_t));
				t->i0=face.verts[0  ];
				t->i1=face.verts[k-1];
				t->i2=face.verts[k  ];

				t->next=0;
				(*cursor)=t;
				cursor=&(t->next);
			}
			
			free(face.verts);
		}
	}
	else
	{
		props = get_element_description_ply( ply, "tristrips", &numstrips, &numproperties);
		
		get_element_setup_ply(ply,"tristrips",1,&face_prop);

		for(k=0; k<numstrips;++k ) 
		{
			get_element_ply( ply, (void*)&face);
			clockwise=1;

			for (I=2;I< face.nverts; I++) 
			{
				if (face.verts[I] == -1) 
					{
						I += 2;
						clockwise = 1;
				}
				else 
				{			
					t=(struct triangle_t*)malloc(sizeof(struct triangle_t));
					
					t->i0=face.verts[I-2];
					t->i1=face.verts[I-1];
					t->i2=face.verts[I  ];

					if (!clockwise) swap_int(t->i1,t->i2);

					t->next=0;
					(*cursor)=t;
					cursor=&(t->next);
					clockwise = 1-clockwise;
				}
			}
			
			free(face.verts);
		}
	}
	
	//close_ply( ply );
	fclose(file);

	set_normals();
	return 1; /* ok */
}



/*======================================================= */
/*======================================================= */

static void draw_triangles()
{
	struct triangle_t* cursor=mesh.triangles;

	glBegin(GL_TRIANGLES);

	while (cursor)
	{
		struct vertex_t* v0=mesh.vertices+cursor->i0;
		struct vertex_t* v1=mesh.vertices+cursor->i1;
		struct vertex_t* v2=mesh.vertices+cursor->i2;

		glNormal3f(-v0->nx,-v0->ny,-v0->nz);glTexCoord1f(0.5f*(v0->y+1));glVertex3f(v0->x,v0->y,v0->z);
		glNormal3f(-v1->nx,-v1->ny,-v1->nz);glTexCoord1f(0.5f*(v1->y+1));glVertex3f(v1->x,v1->y,v1->z);
		glNormal3f(-v2->nx,-v2->ny,-v2->nz);glTexCoord1f(0.5f*(v2->y+1));glVertex3f(v2->x,v2->y,v2->z);

		cursor=cursor->next;
	}
	glEnd();
}

/*======================================================= */
/*======================================================= */

void display_ply(int viewmode) /* 0==filled face 1==filled+wireframe 2==wireframe */
{

	glDisable(GL_POLYGON_OFFSET_FILL);

	if (viewmode==1)
	{
		glPolygonOffset(1,1);
		glEnable(GL_POLYGON_OFFSET_FILL);
	}

	if (viewmode==0 || viewmode==1)
	{
		draw_triangles();
	}

	if (viewmode==1 || viewmode==2)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glColor3f(0,0,0);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		draw_triangles();
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glEnable(GL_LIGHTING);
	}
}




