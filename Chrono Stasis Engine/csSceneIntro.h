#pragma once
#include "csModule.h"
#include "csGlobals.h"
#include "Primitive.h"

#include "imgui.h"


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

	bool show_demo_window = true;
	
};
