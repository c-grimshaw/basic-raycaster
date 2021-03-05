#define MAX_OBJ               8 
#define HEADER_LENGTH        18
#define INF             1.0/0.0
#define MAX_VAL             255
#define BACKGROUND_COLOUR   255
#define BLACK                 1

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

/* Globals declaration */
Scene* scene;
extern const size_t canvas_width, canvas_height;

/* Setup functions */
void insert_header(FILE* fptr, const char* form, int width, int height);

/* Point functions */
Point add(Point* a, Point* b);
Point sub(Point* a, Point* b);
Point s_mul(float t, Point* a);
float length(Point* a);
float dot(Point* a, Point* b);

/* Scene functions */
Scene* scene_init(void);
void destroy_scene(Scene* scene);
void scene_add_sphere(Scene* scene, Point centre, int radius, unsigned char colour);
void scene_add_am_light(Scene* scene, float intensity);
void scene_add_pt_light(Scene* scene, float intensity, Point position);
void scene_add_di_light(Scene* scene, float intensity, Point direction);

/* Rendering functions */
Point canvas_to_viewport(int x, int y);
unsigned char trace_ray(Point camera, Point* D, float t_min, float t_max);
