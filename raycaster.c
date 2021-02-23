#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define format     "P2"
#define inf        1.0/0.0
#define MAX_VAL    255
#define BACKGROUND_COLOUR      255
#define BLACK                    1

typedef struct point {
	float x, y, z;
} point;

typedef struct sphere {
	point centre; 
	int radius;
	unsigned char colour;
} sphere;

typedef struct light {
	unsigned char type;
	float intensity;
	point position;
} light;

typedef struct scene {
	sphere *spheres;
	light  *lights;
} scene;

const float viewportWidth = 1.0, viewportHeight  = 1.0;
const int canvasWidth   = 1080, canvasHeight = 1080; 
const int projection_plane_d = 1;
const point camera = {0.0F, 0.0F, 0.0F};
scene *environment;

void insertHeader(FILE *fptr, const char *form, int w, int h)
{
	char buf[20] = {0};
	sprintf(buf, "%s\n%d %d\n%d\n", form, w, h, MAX_VAL);
	fprintf(fptr, "%s", buf);
}

void putPixel(unsigned char canvas[][canvasHeight], int x, int y, unsigned char colour)
{
	canvas[(canvasWidth / 2) + x][(canvasHeight / 2) - y] = colour;	
}

point *canvasToViewport(int x, int y)
{
	point *viewport_coord = malloc(sizeof(point));
	viewport_coord->x = (x * viewportWidth) / canvasWidth;
	viewport_coord->y = (y * viewportHeight) / canvasHeight;
	viewport_coord->z = projection_plane_d;
	return viewport_coord;
}

point *subtractPoints(point a, point b)
{
	point *c = malloc(sizeof(point));
	c->x = (a.x) - (b.x);
	c->y = (a.y) - (b.y);
	c->z = (a.z) - (b.z);
	return c;
}

float dot(point *a, point *b)
{
	float c = 0;
	c += (a->x) * (b->x);
	c += (a->y) * (b->y);
	c += (a->z) * (b->z);
	return c;
}

float *intersectRaySphere(point camera, point *D, sphere s)
{
	float *t = malloc(2 * sizeof(float));
	int r = s.radius;
	point *C0 = subtractPoints(camera, s.centre);

	float a = dot(D, D);
	float b = 2 * dot(C0, D);
	float c = dot(C0, C0) - r*r;

	float discriminant = b*b - 4*a*c;
	free(C0);
	if(discriminant < 0) {
		t[0] = inf;
		t[1] = inf;
		return t;
	}
	t[0] = (-b + sqrt(discriminant)) / (2*a);
	t[1] = (-b - sqrt(discriminant)) / (2*a);
	return t;
}

unsigned char traceRay(point camera, point *D, float t_min, float t_max)
{
	float closest_t = inf;
	sphere *closest_sphere = NULL;

	for(int i = 0; i < 3; i++) {
		float *t = intersectRaySphere(camera, D, *(environment->spheres + i));
		if(t[0] > t_min && t[0] < t_max && t[0] < closest_t) {
			closest_t = t[0];
			closest_sphere = (environment->spheres + i);
		}
		if(t[1] > t_min && t[1] < t_max && t[1] < closest_t) {
			closest_t = t[1];
			closest_sphere = (environment->spheres + i);
		}
		free(t);
	}
	if(closest_sphere == NULL) {
		return BACKGROUND_COLOUR;
	}
	return closest_sphere->colour;
}
/*
float computeLighting(point P, point N)
{
	float i = 0.0;
	
	for(int i = 0; i < 3; i++) {
		if(environment->(lights + i)->type == 0) {
			i += environment->(lights + i)->intensity;
		} else {
			point *L;
			if(environment->(lights + i)->type == 1) {
				L = subtractPoints(environment->(lights+i)->position, P);
			} else {
				L = (environment->(lights+i)->position);
			}

			n_dot_l = dot(N, L);
			if(n_dot_l > 0) {
				i += (environment->(lights+i)->intensity) * n_dot_l / length(N) * length(L);
	}
}
*/
void drawCanvas(FILE *fptr, unsigned char canvas[][canvasHeight])
{
	for(int y = 0; y < canvasHeight; y++) {
		for(int x = 0; x < canvasWidth; x++) {
			fprintf(fptr, "%u ", canvas[x][y]);
		}
		fprintf(fptr, "\n");
	}
}

int main()
{
	unsigned char canvas[canvasWidth][canvasHeight];
	FILE *fptr;
	
	fptr = fopen("/home/grim0ne/Programming/graphics/test.pgm", "w");

	if(fptr == NULL)
   	{
      		printf("Error!");
      		exit(1);
   	}
	insertHeader(fptr, format, canvasWidth, canvasHeight);
	// PGM body
	environment = malloc(sizeof(scene));
	environment->spheres = malloc(3* sizeof(sphere));

	sphere red = {{0.0F, -1.0F, 4.0F}, 1, 100};
	sphere blue = {{2.0F,  0.0F, 4.0F}, 1, 50};
	sphere green = {{-2.0F, 0.0F, 4.0F}, 1, 125}; 
	environment->spheres[0] = red;
	environment->spheres[1] = blue;
	environment->spheres[2] = green;

	for(int x = -canvasWidth / 2; x < canvasWidth / 2; x++) {
		for(int y = -canvasHeight / 2; y <= canvasHeight / 2; y++) {
			point *D = canvasToViewport(x, y);
			unsigned char colour = traceRay(camera, D, 1, inf);
			free(D);
			putPixel(canvas, x, y, colour);
		}
	}
	drawCanvas(fptr, canvas);
	fclose(fptr);
}
