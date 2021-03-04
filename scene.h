#define MAX_OBJ 8 

typedef struct Point {
	float x, y, z;
} Point;

typedef struct Sphere {
	Point centre; 
	int radius;
	unsigned char colour;
} Sphere;

typedef struct am_Light {
	float intensity;
} am_Light;

typedef struct pt_Light {
	float intensity;
	Point position;
} pt_Light;

typedef struct di_Light {
	float intensity;
	Point direction;
} di_Light;

typedef struct Scene {
	Sphere*   sc_sphere_list[MAX_OBJ];
	am_Light* sc_ambient_lights[MAX_OBJ];
	pt_Light* sc_point_lights[MAX_OBJ];
	di_Light* sc_directional_lights[MAX_OBJ];
	size_t    sc_sphere_list_sz;
	size_t    sc_ambient_lights_sz;
	size_t    sc_point_lights_sz;
	size_t    sc_directional_lights_sz;
} Scene;

/* Point functions */
Point* add(Point a, Point b);
Point* sub(Point a, Point b);
float length(Point* a);
void scalar_multiply(float t, Point* a);
float dot(Point* a, Point* b);

/* Scene functions */
Scene* scene_init(void);
void scene_add_sphere(Scene* scene, Point centre, int radius, unsigned char colour);
void scene_add_am_light(Scene* scene, float intensity);
void scene_add_pt_light(Scene* scene, float intensity, Point position);
void scene_add_di_light(Scene* scene, float intensity, Point direction);
void scene_print_spheres(Scene* scene);
