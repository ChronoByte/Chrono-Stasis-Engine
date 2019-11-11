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
#include "csFBXLoader.h"
#include "csTextureLoader.h"
#include "csScene.h"
#include "csFileSystem.h"

#include "Serializer.h"
#include <list>
#include <vector>

#define MAX_FRAMES_LOGGED 100
#define MAX_MEMORY_LOGGED 100
#define MAX_RAM_LOGGED 100

class Application
{
public:
	ModuleWindow*			window = nullptr;
	ModuleInput*			input = nullptr;
	ModuleEditor*			editor = nullptr;
	ModuleRenderer3D*		renderer3D = nullptr;
	ModuleCamera3D*			camera = nullptr;
	ModuleUI*				gui = nullptr;
	ModuleFBXLoader*		fbx = nullptr;
	ModuleTextureLoader*	texture = nullptr;
	ModuleScene*			scene = nullptr;
	ModuleFileSystem*		fs = nullptr;

	Serializer*            json = nullptr;
private:

	std::list<Module*> list_modules;
	
	
	float				dt = 0.0f;

	Timer				startup_time;
	Timer				frame_time;

	uint64				frame_count = 0;
	float				seconds = 0.0f;
	Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;

	uint32				framerate_cap = 0;
	uint32				frame_ms_cap = 0;

	uint32				time_to_wait = 0;
	
	bool cap_frames = false;
	bool vsync = true;

	std::vector<float> chart_frames;
	std::vector<float> chart_ms;
	std::vector<float> chart_memory;
	std::vector<float> chart_ram;
	std::vector<float> chart_ms_capped;

	bool				want_to_save = false;
	bool				want_to_load = false;
	
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();


public:
	const char* GetTitle() const;
	const char* GetOrganization() const;
	const char* GetVersion() const;

	void SetTitle(const char*);
	void SetOrganization(const char*);
	void SetVersion(const char*);

	uint GetFPS() const;
	void SetFPS(uint max_fps);
	uint32 GetCappedMS() const;
	float GetDT() const;
	uint64 GetFrameCounter() const;
	float GetSeconds() const;
	bool GetVSYNC() const;

	std::vector<float> GetFrames() const;
	std::vector<float> GetMS() const;
	std::vector<float> GetRAM() const;
	std::vector<float> GetMSCapped() const;
	std::vector<float> GetMemory() const;


	void SendToLink(const char* link) const;

	void Save();
	void Load();

public:

	std::string engine_title; //Remminder for me: std::string var
	std::string organization_name;
	std::string version;

	bool to_close_app = false;
	

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	bool LoadNow();
	bool SaveNow();
};

extern Application* App; 