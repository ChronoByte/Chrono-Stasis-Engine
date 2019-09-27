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

	std::list<Module*> list_modules;
	
	
	float				dt;

	Timer				startup_time;
	Timer				frame_time;

	uint64				frame_count = 0;
	Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;

	uint32				framerate_cap = 0;
	uint32				frame_ms_cap = 0;

	uint32				time_to_wait = 0;
	

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
