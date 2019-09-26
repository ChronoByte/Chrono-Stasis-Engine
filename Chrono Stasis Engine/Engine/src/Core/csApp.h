#pragma once

#include "csGlobals.h"
#include "Timer.h"
#include "csModule.h"
#include "csWindow.h"
#include "csInput.h"

#include "csEditor.h"
#include "csRenderer3D.h"
#include "csCamera3D.h"
#include "csRendererGUI.h"



#include <list>

class Application
{
public:
	ModuleWindow*		window = nullptr;
	ModuleInput*		input = nullptr;
	ModuleEditor*		editor = nullptr;
	ModuleRenderer3D*	renderer3D = nullptr;
	ModuleCamera3D*		camera = nullptr;
	ModuleUI*			gui = nullptr;
	
private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	const char* GetTitle() const;
public:
	
	std::string engine_title; //Remminder for me: std::string var
	bool to_close_app = false;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

};