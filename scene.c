#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "scene.h"

Point add(Point* a, Point* b)
{
	return (Point){a->x + b->x, a->y + b->y, a->z + b->z};
}

Point sub(Point* a, Point* b)
{
	return (Point){a->x - b->x, a->y - b->y, a->z - b->z};
}


Point s_mul(float t, Point* a)
{
	return (Point){t * a->x, t * a->y, t * a->z};
}

float length(Point* a)
{
	return sqrt((a->x * a->x)+(a->y * a->y)+(a->z * a->z));
}

float dot(Point* a, Point* b)
{
	return (float)((a->x * b->x)+(a->y * b->y)+(a->z * b->z));
}

Scene* scene_init(void) {
    Scene* s = malloc(sizeof(Scene));
    s->sc_sphere_list_sz = 0;
    s->sc_ambient_lights_sz = 0;
    s->sc_point_lights_sz = 0;
    s->sc_directional_lights_sz = 0;
    return s;
}

void scene_add_sphere(Scene* scene, Point p, int r, unsigned char c) {
    Sphere* s = malloc(sizeof(Sphere));
    s->centre = p;
    s->radius = r;
    s->colour = c;
    scene->sc_sphere_list[scene->sc_sphere_list_sz++] = s;
}

void scene_add_am_light(Scene* scene, float intensity)
{
	am_Light* a = malloc(sizeof(am_Light));
	a->intensity = intensity;
	scene->sc_ambient_lights[scene->sc_ambient_lights_sz++] = a;
}

void scene_add_pt_light(Scene* scene, float intensity, Point position)
{
	pt_Light* a = malloc(sizeof(pt_Light));
	a->intensity = intensity;
	a->position = position;
	scene->sc_point_lights[scene->sc_point_lights_sz++] = a;
}

void scene_add_di_light(Scene* scene, float intensity, Point direction)
{
	di_Light* a = malloc(sizeof(di_Light));
	a->intensity = intensity;
	a->direction = direction;
	scene->sc_directional_lights[scene->sc_directional_lights_sz++] = a;
}
