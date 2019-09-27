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

	Timer	fps_timer;
	Uint32	frames;

	int		last_frame_ms;
	int		last_fps;
	int		capped_ms;
	int		fps_counter;


public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();


public:
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void SetTitle(const char*);
	void SetOrganization(const char*);


	uint GetFPS() const;
	void SetFPS(uint max_fps);


	void SendToLink(const char* link) const;

public:

	std::string engine_title; //Remminder for me: std::string var
	std::string organization_name;
	bool to_close_app = false;
	bool cap_frames = false;


private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

};
