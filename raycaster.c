#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "scene.h"

#define FORMAT     "P2"
#define INF        1.0/0.0
#define MAX_VAL    255
#define BACKGROUND_COLOUR      255
#define BLACK                    1

const int canvasWidth   = 1080, canvasHeight = 1080; 
Point camera = {0.0F, 0.0F, 0.0F};
Scene* scene;

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

Point canvasToViewport(int x, int y)
{
	const int projection_plane_d = 1;
	const float viewportWidth = 1.0, viewportHeight  = 1.0;
	Point viewport_coord;

	viewport_coord.x = (x * viewportWidth) / canvasWidth;
	viewport_coord.y = (y * viewportHeight) / canvasHeight;
	viewport_coord.z = projection_plane_d;
	return viewport_coord;
}

float* intersectRaySphere(Point camera, Point* D, Sphere* s)
{
	float *t = malloc(2 * sizeof(float));
	int r = s->radius;
	Point C0 = sub(&camera, &(s->centre));

	float a = dot(D, D);
	float b = 2 * dot(&C0, D);
	float c = dot(&C0, &C0) - r*r;

	float discriminant = b*b - 4*a*c;

	if(discriminant < 0) {
		t[0] = INF;
		t[1] = INF;
		return t;
	}
	t[0] = (-b + sqrt(discriminant)) / (2*a);
	t[1] = (-b - sqrt(discriminant)) / (2*a);
	return t;
}

float computeLighting(Point* P, Point* N)
{
	float n_dot_l = 0.0, intensity = 0.0;
	Point L;

	for(int i = 0; i < scene->sc_ambient_lights_sz; i++) {
		intensity += scene->sc_ambient_lights[i]->intensity;
	}
	
	for(int i = 0; i < scene->sc_point_lights_sz; i++) {
		L = sub(&(scene->sc_point_lights[i]->position), P);
		n_dot_l = dot(N, &L);
		if (n_dot_l > 0) {
			intensity += scene->sc_point_lights[i]->intensity\
			* n_dot_l/(length(N) * length(&L));
		}
	}
	
	for(int i = 0; i < scene->sc_directional_lights_sz; i++) {
		L = scene->sc_directional_lights[i]->direction;
		n_dot_l = dot(N, &L);
		if (n_dot_l > 0) {
			intensity += scene->sc_directional_lights[i]->intensity\
			* n_dot_l/(length(N) * length(&L));
		}
	}

	return intensity;
}

unsigned char traceRay(Point camera, Point* D, float t_min, float t_max)
{
	float closest_t = INF;
	Point P, N;
	Sphere *closest_sphere = 0;

	for(int i = 0; i < scene->sc_sphere_list_sz; i++) {
		float *t = intersectRaySphere(camera, D, scene->sc_sphere_list[i]);
		if(t[0] > t_min && t[0] < t_max && t[0] < closest_t) {
			closest_t = t[0];
			closest_sphere = scene->sc_sphere_list[i];
		}
		if(t[1] > t_min && t[1] < t_max && t[1] < closest_t) {
			closest_t = t[1];
			closest_sphere = scene->sc_sphere_list[i];
		}
		free(t);
	}
	if(closest_sphere == 0) {
		return BACKGROUND_COLOUR;
	}
	P = s_mul(closest_t, D);
	P = add(&camera, &P);
	N = sub(&P, &(closest_sphere->centre));
	N = s_mul(1/length(&N), &N);
	return closest_sphere->colour * computeLighting(&P, &N);
}

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
	unsigned char canvas[canvasWidth][canvasHeight], colour;
	FILE* fptr;
	Point D;

	fptr = fopen("./test.pgm", "w");
	if(fptr == 0)
   	{
      		printf("Error!");
      		exit(1);
   	}

	insertHeader(fptr, FORMAT, canvasWidth, canvasHeight);
	// PGM body
	scene = scene_init();
	scene_add_sphere(scene, (Point){0.0F, -1.0F, 4.0F}, 1, 150);
	scene_add_sphere(scene, (Point){2.0F, 0.0F, 4.0F}, 1, 150);
	scene_add_sphere(scene, (Point){-2.0F, 0.0F, 4.0F}, 1, 150);
	scene_add_sphere(scene, (Point){0, -5001, 0}, 5000, 10);
	scene_add_am_light(scene, 0.2);
	scene_add_pt_light(scene, 0.6, (Point){2, 1, 0});
	scene_add_di_light(scene, 0.2, (Point){1, 4, 4});

	for(int x = -canvasWidth / 2; x < canvasWidth / 2; x++) {
		for(int y = canvasHeight / 2; y > -canvasHeight / 2; y--) {
			D = canvasToViewport(x, y);
			colour = traceRay(camera, &D, 1, INF);
			putPixel(canvas, x, y, colour);
		}
	}
	drawCanvas(fptr, canvas);
	fclose(fptr);
}
