#define MAX_OBJ 64

typedef struct Point {
	float x, y, z;
} Point;

typedef struct Sphere {
	Point centre; 
	int radius;
	unsigned char colour;
} Sphere;

typedef struct Scene {
	Sphere* sc_sphere_list[MAX_OBJ];
	size_t  sc_sphere_list_sz;
} Scene;

Scene* scene_init(void);
void scene_add_sphere(Scene* scene, Point centre, int radius, unsigned char colour);
void scene_print_spheres(Scene* scene);
