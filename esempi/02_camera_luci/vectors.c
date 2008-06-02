#include "vectors.h"

void point_translate(Point3d* point, const Vector3d* v)
{
	point->x+=v->x;
	point->y+=v->y;
	point->z+=v->z;
}


void point_sum(Point3d* dest, Point3d* point1, Point3d* point2)
{
	dest->x = point1->x + point2->x;
	dest->y = point1->y + point2->y;
	dest->z = point1->z + point2->z;
}

void init_point(Point3d* source, Point3d* dest) 
{
	dest->x=source->x;
	dest->y=source->y;
	dest->z=source->z;
}


void vector_normalize(Vector3d* v)
{
	GLfloat m=(GLfloat)sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
	v->x/=m;
	v->y/=m;
	v->z/=m;
}

GLfloat vector_modulo(Vector3d* v) 
{
	GLfloat m=(GLfloat)sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
	return m; 
}


void vector_scale(Vector3d* v,GLfloat coeff)
{
	v->x *= coeff;
	v->y *= coeff;
	v->z *= coeff;
}


void point_scale(Point3d* v,GLfloat coeff)
{
	v->x *= coeff;
	v->y *= coeff;
	v->z *= coeff;
}

/* vettore risultato di punto u - punto v */
void vector_diff(Vector3d* dest,const Point3d* u,const Point3d* v)
{
	dest->x=u->x-v->x;
	dest->y=u->y-v->y;
	dest->z=u->z-v->z;
}

void vector_cross_product(Vector3d* dest,const Vector3d* u,const Vector3d* v)
{
	dest->x=(u->y*v->z) - (u->z*v->y);
	dest->y=(u->z*v->x) - (u->x*v->z);
	dest->z=(u->x*v->y) - (u->y*v->x);
	vector_normalize(dest);
}


GLfloat getMinimo(GLfloat a, GLfloat b, GLfloat c) 
{	
	if(a <= b) 
	{
		if(a<=c) 
		{
			return a;
		} else {
			return c;
		}
	}
	else {
		if(b<=c) 
		{
			return b;
		} else {
			return c;
		}
	}
}


void convexCombination(Point3d* destinazione, Point3d* a, Point3d* b, GLfloat lambda) 
{
	Point3d x; 
	init_point(a, &x);
	point_scale(&x, lambda); 
//	printf("comb_scalato: %f %f %f\n", x.x, x.y, x.z); 

	Point3d y; 
	init_point(b, &y);
	lambda = 1.0f - lambda;
	point_scale(&y, lambda); 
//	printf("y_scalato: %f %f %f\n", y.x, y.y, y.z);

	point_sum(destinazione, &x, &y);
}



void midPoint(Point3d* dest, Point3d* a, Point3d* b) 
{
	/* richiamo la combinazione convessa con 0.5, la metà */
	convexCombination(dest, a, b, 0.5f);
}


GLfloat minDistance(Point3d* point1, Point3d* point2, Point3d* point3) 
{
	Vector3d distance1_2; 
	vector_diff(&distance1_2, point1, point2); 
//	printf("distance1_2: %f %f %f\n", distance1_2.x, distance1_2.y, distance1_2.z);

	Vector3d distance2_3; 
	vector_diff(&distance2_3, point2, point3); 
//	printf("distance2_3: %f %f %f\n", distance2_3.x, distance2_3.y, distance2_3.z);

	Vector3d distance3_1; 
	vector_diff(&distance3_1, point3, point1); 
//	printf("distance3_1: %f %f %f\n", distance3_1.x, distance3_1.y, distance3_1.z);

//	printf("moduli: \n");

	GLfloat mod1_2 = vector_modulo(&distance1_2);
//	printf("modulo_dist_1_2: %f\n", mod1_2);
	GLfloat mod2_3 = vector_modulo(&distance2_3);
//	printf("modulo_dist_2_3: %f\n", mod2_3);
	GLfloat mod3_1 = vector_modulo(&distance3_1);
//	printf("modulo_dist_3_1: %f\n", mod3_1);

	GLfloat minimo;
	minimo = getMinimo(mod1_2, mod2_3, mod3_1);
//	printf("modulo minimo: %f\n", minimo); 

	return minimo;
}




