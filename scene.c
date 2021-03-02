#include <stdlib.h>
#include <stdio.h>
#include "scene.h"

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
