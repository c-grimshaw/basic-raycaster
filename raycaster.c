#include <stdio.h>
#include <stdlib.h>
#include "scene.h"

const size_t canvas_width = 1080, canvas_height = 1080;
const char* format = "P5";

void prepare_scene(void) 
{
	scene = scene_init();
	scene_add_sphere(scene, (Point){0.0F, -1.0F, 4.0F}, 1, 150);
	scene_add_sphere(scene, (Point){2.0F, 0.0F, 4.0F}, 1, 150);
	scene_add_sphere(scene, (Point){-2.0F, 0.0F, 4.0F}, 1, 150);
	scene_add_sphere(scene, (Point){0, -5001, 0}, 5000, 10);
	scene_add_am_light(scene, 0.2);
	scene_add_pt_light(scene, 0.8, (Point){2, 1, 0});
	scene_add_di_light(scene, 0.2, (Point){1, 4, 4});

}

int main()
{
	unsigned char colour;
	FILE* fptr;
	Point D, camera = (Point){0,0,0};

	fptr = fopen("./spheres.pgm", "wb");
	insert_header(fptr, format, canvas_width, canvas_height);
	prepare_scene();

	for (int y = 0; y < canvas_height - 1; y++) {
		for (int x = 0; x < canvas_width - 1; x++) {
			D = canvas_to_viewport(x - canvas_width/2, -y + canvas_height/2);
			colour = trace_ray(camera, &D, 1, INF);
			fputc(colour, fptr); 
		}
		fprintf(fptr, "\n");
	}
	destroy_scene(scene);
	fclose(fptr);
}
