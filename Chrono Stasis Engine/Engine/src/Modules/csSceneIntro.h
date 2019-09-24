#pragma once
#include "csModule.h"
#include "csGlobals.h"
//#include "Primitive.h"

#include "imgui/imgui.h"
#include "pcg/pcg C/include/pcg_variants.h"
#include "pcg/pcg C/extras/entropy.h"

//#include "SDL\include\SDL.h"
//#include "SDL\include\SDL_opengl.h"
#include "glew/include/GL/glew.h"

#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLibFwd.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:

	PhysBody3D* sensor;

	double rand1;
	uint32_t rand2;
	uint32_t rand3;

	uint32_t nums[6];
	uint64_t seeds[2];

	int min = 0;
	int	max = 0;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	bool math_test = false;

	//Struct with state + inc
	pcg32_random_t rng; //typedef struct 64-b
	pcg_state_setseq_64 rng_bounded; // struct 64-b
	pcg_state_setseq_64 rng_bounded2;
	bool show_demo_window = false;
	bool random_panel = false;

	Sphere sphere_1;
	Sphere sphere_2;

	Capsule capsule_1;
	Capsule capsule_2;
	
	Triangle triangle_1;
	Triangle triangle_2;

	Plane plane_1;
	Plane plane_2;

	AABB aabb_1;
	AABB aabb_2;

	Ray ray_1;
	
	OBB obb_1;
	OBB obb_2;
};
