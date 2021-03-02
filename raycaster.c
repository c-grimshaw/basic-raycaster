#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "scene.h"

#define format     "P2"
#define inf        1.0/0.0
#define MAX_VAL    255
#define BACKGROUND_COLOUR      255
#define BLACK                    1

const float viewportWidth = 1.0, viewportHeight  = 1.0;
const int canvasWidth   = 1080, canvasHeight = 1080; 
const int projection_plane_d = 1;
const Point camera = {0.0F, 0.0F, 0.0F};
Scene *environment;

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

Point* canvasToViewport(int x, int y)
{
	Point *viewport_coord = malloc(sizeof(Point));
	viewport_coord->x = (x * viewportWidth) / canvasWidth;
	viewport_coord->y = (y * viewportHeight) / canvasHeight;
	viewport_coord->z = projection_plane_d;
	return viewport_coord;
}

Point* subtractPoints(Point a, Point b)
{
	Point *c = malloc(sizeof(Point));
	c->x = (a.x) - (b.x);
	c->y = (a.y) - (b.y);
	c->z = (a.z) - (b.z);
	return c;
}

float dot(Point *a, Point *b)
{
	float c = 0;
	c += (a->x) * (b->x);
	c += (a->y) * (b->y);
	c += (a->z) * (b->z);
	return c;
}

float *intersectRaySphere(Point camera, Point *D, Sphere* s)
{
	float *t = malloc(2 * sizeof(float));
	int r = s->radius;
	Point *C0 = subtractPoints(camera, s->centre);

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

unsigned char traceRay(Point camera, Point *D, float t_min, float t_max)
{
	float closest_t = inf;
	Sphere *closest_sphere = NULL;

	for(int i = 0; i < environment->sc_sphere_list_sz; i++) {
		float *t = intersectRaySphere(camera, D, environment->sc_sphere_list[i]);
		if(t[0] > t_min && t[0] < t_max && t[0] < closest_t) {
			closest_t = t[0];
			closest_sphere = environment->sc_sphere_list[i];
		}
		if(t[1] > t_min && t[1] < t_max && t[1] < closest_t) {
			closest_t = t[1];
			closest_sphere = environment->sc_sphere_list[i];
		}
		free(t);
	}
	if(closest_sphere == NULL) {
		return BACKGROUND_COLOUR;
	}
	return closest_sphere->colour;
}

/*
float computeLighting(Point P, Point N)
{
	float i = 0.0;
	
	for(int i = 0; i < 3; i++) {
		if(environment->(lights + i)->type == 0) {
			i += environment->(lights + i)->intensity;
		} else {
			Point *L;
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
	environment = scene_init();
	scene_add_sphere(environment, (Point){0.0F, -1.0F, 4.0F}, 1, 100);
	scene_add_sphere(environment, (Point){2.0F, 0.0F, 4.0F}, 1, 50);
	scene_add_sphere(environment, (Point){-2.0F, 0.0F, 4.0F}, 1, 125);
	scene_add_sphere(environment, (Point){-2.0F, 0.0F, 12.0F}, 1, 10);

	for(int x = -canvasWidth / 2; x < canvasWidth / 2; x++) {
		for(int y = -canvasHeight / 2; y <= canvasHeight / 2; y++) {
			Point *D = canvasToViewport(x, y);
			unsigned char colour = traceRay(camera, D, 1, inf);
			free(D);
			putPixel(canvas, x, y, colour);
		}
	}
	drawCanvas(fptr, canvas);
	fclose(fptr);
}
