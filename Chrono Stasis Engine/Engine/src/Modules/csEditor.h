#pragma once
#include "csModule.h"
#include "csGlobals.h"

#include "imgui/imgui.h"
#include "pcg/pcg C/include/pcg_variants.h"
#include "pcg/pcg C/extras/entropy.h"

#include "glew/include/GL/glew.h"

#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLibFwd.h"

#define MAX_SNAKE 2

struct PhysBody3D;
class ConfigWindow;
class GeometryWindow; 
class AboutWindow;

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	//void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:

	ConfigWindow*	config = nullptr;
	GeometryWindow* geometryWin = nullptr; 
	AboutWindow*	about = nullptr;
	PhysBody3D*		sensor = nullptr;

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
};
