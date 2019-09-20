#pragma once
#include "csModule.h"
#include "csGlobals.h"
#include "Primitive.h"

#include "imgui.h"
#include "pcg/pcg C/include/pcg_variants.h"

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
	Cube s;
	PhysBody3D* sensor;

	double rand1;
	uint32_t rand2;
	//Struct with state + inc
	pcg32_random_t rng; //typedef struct 64-b
	pcg_state_setseq_64 rng_bounded; // struct 64-b

	bool show_demo_window = false;
	bool random_panel = false;
};
