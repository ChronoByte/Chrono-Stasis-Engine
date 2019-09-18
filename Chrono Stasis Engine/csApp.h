#pragma once

#include "p2List.h"
#include "csGlobals.h"
#include "Timer.h"
#include "csModule.h"
#include "csWindow.h"
#include "csInput.h"
#include "csAudio.h"
#include "csSceneIntro.h"
#include "csRenderer3D.h"
#include "csCamera3D.h"
#include "csPhysics3D.h"
//#include "csPlayer.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	//ModulePlayer* player;

private:

	Timer	ms_timer;
	float	dt;
	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};