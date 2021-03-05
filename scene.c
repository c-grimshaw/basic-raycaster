#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "scene.h"

static float compute_lighting(Point* P, Point* N);
static float pt_compute_lighting(pt_Light* a, Point* P, Point* N);
static float di_compute_lighting(di_Light* a, Point* P, Point* N);
static float* intersect_ray_sphere(Point camera, Point* D, Sphere* s);

void insert_header(FILE *fptr, const char *form, int width, int height)
{
	char buf[HEADER_LENGTH] = {0};
	snprintf(buf,HEADER_LENGTH,"%s\n%d %d\n%d\n",form,width,height,MAX_VAL);
	fprintf(fptr,"%s",buf);
}

Point add(Point* a, Point* b) {
	return (Point){a->x + b->x, a->y + b->y, a->z + b->z};
}

Point sub(Point* a, Point* b) {
	return (Point){a->x - b->x, a->y - b->y, a->z - b->z};
}

Point s_mul(float t, Point* a) {
	return (Point){t * a->x, t * a->y, t * a->z};
}

float length(Point* a) {
	return sqrt((a->x * a->x)+(a->y * a->y)+(a->z * a->z));
}

float dot(Point* a, Point* b) {
	return (float)((a->x * b->x)+(a->y * b->y)+(a->z * b->z));
}

Scene* scene_init(void)
{
    Scene* s = malloc(sizeof(Scene));
    s->sc_sphere_list_sz = 0;
    s->sc_ambient_lights_sz = 0;
    s->sc_point_lights_sz = 0;
    s->sc_directional_lights_sz = 0;
    return s;
}

void destroy_scene(Scene* scene)
{
	for (int i = 0; i < scene->sc_sphere_list_sz; i++)
		free(scene->sc_sphere_list[i]);
	for (int i = 0; i < scene->sc_ambient_lights_sz; i++)
		free(scene->sc_ambient_lights[i]);
	for (int i = 0; i < scene->sc_point_lights_sz; i++)
		free(scene->sc_point_lights[i]);
	for (int i = 0; i < scene->sc_directional_lights_sz; i++)
		free(scene->sc_directional_lights[i]);
	free(scene);
}

void scene_add_sphere(Scene* scene, Point p, int r, unsigned char c)
{
	if (scene->sc_sphere_list_sz < MAX_OBJ) {
		Sphere* s = malloc(sizeof(Sphere));
		s->centre = p;
		s->radius = r;
		s->colour = c;
		scene->sc_sphere_list[scene->sc_sphere_list_sz++] = s;
	}
}

void scene_add_am_light(Scene* scene, float intensity)
{
	if (scene->sc_ambient_lights_sz < MAX_OBJ) {
		am_Light* a = malloc(sizeof(am_Light));
		a->intensity = intensity;
		scene->sc_ambient_lights[scene->sc_ambient_lights_sz++] = a;
	}
}

void scene_add_pt_light(Scene* scene, float intensity, Point position)
{
	if (scene->sc_point_lights_sz < MAX_OBJ) {
		pt_Light* a = malloc(sizeof(pt_Light));
		a->intensity = intensity;
		a->position = position;
		scene->sc_point_lights[scene->sc_point_lights_sz++] = a;
	}
}

void scene_add_di_light(Scene* scene, float intensity, Point direction)
{
	if (scene->sc_directional_lights_sz < MAX_OBJ) {
		di_Light* a = malloc(sizeof(di_Light));
		a->intensity = intensity;
		a->direction = direction;
		scene->sc_directional_lights[scene->sc_directional_lights_sz++] = a;
	}
}

Point canvas_to_viewport(int x, int y) 
{
	const int projection_plane_d = 1;
	const float viewport_width = 1.0, viewport_height  = 1.0;
	Point viewport_coord;

	viewport_coord.x = (x * viewport_width) / canvas_width;
	viewport_coord.y = (y * viewport_height) / canvas_height;
	viewport_coord.z = projection_plane_d;
	return viewport_coord;
}

unsigned char trace_ray(Point camera, Point* D, float t_min, float t_max)
{
	float closest_t = INF;
	Point P, N;
	Sphere *closest_sphere = 0;

	for (int i = 0; i < scene->sc_sphere_list_sz; i++) {
		float *t = intersect_ray_sphere(camera, D, scene->sc_sphere_list[i]);
		if (t[0] > t_min && t[0] < t_max && t[0] < closest_t) {
			closest_t = t[0];
			closest_sphere = scene->sc_sphere_list[i];
		}
		if (t[1] > t_min && t[1] < t_max && t[1] < closest_t) {
			closest_t = t[1];
			closest_sphere = scene->sc_sphere_list[i];
		}
		free(t);
	}

	if (closest_sphere == 0)
		return BACKGROUND_COLOUR;
	P = s_mul(closest_t, D);
	P = add(&camera, &P);
	N = sub(&P, &(closest_sphere->centre));
	N = s_mul(1/length(&N), &N);
	return closest_sphere->colour * compute_lighting(&P, &N);
}

static float* intersect_ray_sphere(Point camera, Point* D, Sphere* s)
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

static float compute_lighting(Point* P, Point* N)
{
	float intensity = 0.0;

	for (int i = 0; i < scene->sc_ambient_lights_sz; i++) 
		intensity += scene->sc_ambient_lights[i]->intensity;

	for (int i = 0; i < scene->sc_point_lights_sz; i++) 
		intensity += pt_compute_lighting(scene->sc_point_lights[i], P, N);

	for (int i = 0; i < scene->sc_directional_lights_sz; i++) 
		intensity += di_compute_lighting(scene->sc_directional_lights[i], P, N);

	return intensity;
}

static float pt_compute_lighting(pt_Light* a, Point* P, Point* N)
{
	float n_dot_l = 0.0, intensity = 0.0;
	Point L;

	L = sub(&(a->position), P);
	n_dot_l = dot(N, &L);
	if (n_dot_l > 0) 
		intensity += a->intensity * n_dot_l/(length(N) * length(&L));
	return intensity;
}

static float di_compute_lighting(di_Light* a, Point* P, Point* N)
{
	float n_dot_l = 0.0, intensity = 0.0;
	Point L;

	L = a->direction;
	n_dot_l = dot(N, &L);
	if (n_dot_l > 0) 
		intensity += a->intensity * n_dot_l/(length(N) * length(&L));
	return intensity;
}
