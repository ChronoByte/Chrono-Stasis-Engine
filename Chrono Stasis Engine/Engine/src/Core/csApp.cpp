#include "csApp.h"
#include "Windows.h"
#include "Psapi.h"
#include "mmgr/mmgr.h"
#include "JSON/parson.h"
#include "PseudoRandom.h"

Application::Application()
{
	
	// TODO: Load with JSON
	//engine_title = TITLE;
	//organization_name = ORGANIZATION;
	//version = "v0.0.1";
	//framerate_cap = 30;
	//frame_ms_cap = 1000 / framerate_cap;

	window		= new ModuleWindow();
	input		= new ModuleInput();
	editor		= new ModuleEditor();
	renderer3D	= new ModuleRenderer3D();
	camera		= new ModuleCamera3D();
	gui			= new ModuleUI();
	fbx			= new ModuleFBXLoader();
	texture		= new ModuleTextureLoader();
	scene		= new ModuleScene();
	fs			= new ModuleFileSystem();

	json		= new Serializer();
	serialization = new ModuleSceneSerializer();

	resources	= new ModuleResources();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	
	// Managers
	AddModule(editor);
	AddModule(fbx);
	AddModule(texture);
	AddModule(fs);
	AddModule(serialization);
	AddModule(resources);

	// Game Scenes 
	AddModule(scene);
	
	// Renderer GUI
	AddModule(gui);
	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{

	std::list<Module*>::const_reverse_iterator item = list_modules.rbegin();

	while (item != list_modules.rend())
	{
		delete (*item);
		item++;
	}

	window		= nullptr;
	input		= nullptr;
	editor		= nullptr;
	renderer3D	= nullptr;
	camera		= nullptr;
	gui			= nullptr;
	fbx			= nullptr;
	texture     = nullptr;
	scene		= nullptr;
	fs			= nullptr;
	serialization = nullptr;
	json = nullptr;
	resources = nullptr;
	
}

bool Application::Init()
{
	bool ret = false;

	InitSeedEntropy();

	JSON_Value* config_file;
	JSON_Object* config;
	JSON_Object* config_node;

	config_file = json_parse_file("Settings/config.json");

	if (config_file)
	{
		ret = true;

		config = json_value_get_object(config_file);
		config_node = json_object_dotget_object(config, "Application");
		engine_title = json_object_get_string(config_node, "Engine Name");
		organization_name = json_object_get_string(config_node, "Organization Name");
		version = json_object_get_string(config_node, "Version");
		framerate_cap = json_object_get_number(config_node, "Max FPS");
		vsync = json_object_get_boolean(config_node, "VSYNC");

		// Call Init() in all modules
		std::list<Module*>::const_iterator item = list_modules.begin();

		LOG("-------------- Application Init --------------");

		while (item != list_modules.end() && ret == UPDATE_CONTINUE)
		{
			if ((*item)->IsEnabled()) 
			{
				config_node = json_object_get_object(config, (*item)->name.c_str());
				ret = (*item)->Init(config_node);
			}
			item++;
		}

		// After all Init calls we call Start() in all modules
		LOG("-------------- Application Start --------------");
		item = list_modules.begin();

		while (item != list_modules.end() && ret == UPDATE_CONTINUE)
		{
			if ((*item)->IsEnabled())
				ret = (*item)->Start();

			item++;
		}

		startup_time.Start();
	}
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_ms_cap = 1000 / framerate_cap;

	frame_count++;
	last_sec_frame_count++;
	frame_time.Start(); 

	dt = (1 / (float)framerate_cap); // Differential Time

	// Set Game DT
	if (dtMultiplier <= 0.f || dtMultiplier > MAX_DT_MULTIPLIER)
		dtMultiplier = 1.f; 

	gameDt = dt * dtMultiplier; 

}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (want_to_load == true) 
	{
		LoadNow();
		want_to_load = false;
	}

	if (want_to_save == true) 
	{
		SaveNow();
		want_to_save = false;
	}


	// Framerate calculations --
	if (last_sec_frame_time.Read() > 1000) // When every sec is reached, frame counter is reset
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;

		chart_frames.push_back(prev_last_sec_frame_count); //Saving frame to plot as a chartflow 
		if (chart_frames.size() > MAX_FRAMES_LOGGED)
			chart_frames.erase(chart_frames.begin());
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec(); // Current Framerate expresed in FPS (Frames x Sec)
	float seconds_since_startup = startup_time.ReadSec(); // Seconds since game is up (Init Module)
	seconds = seconds_since_startup;
	uint32 last_frame_ms = frame_time.Read(); // Amount of ms from last frame
	uint32 frames_on_last_sec = prev_last_sec_frame_count; // Amount of frames from last second

	chart_ms.push_back((float)last_frame_ms); // Saving ms to plot as a chartflow
	if (chart_ms.size() > MAX_FRAMES_LOGGED)
		chart_ms.erase(chart_ms.begin());

	// Framerate CAP --
	if (frame_ms_cap > 0 && (last_frame_ms < frame_ms_cap)) {
		time_to_wait = frame_ms_cap - last_frame_ms;
		SDL_Delay(time_to_wait); // Time to wait until monitor scan the panel due to its refresh rate
	}

	// ms Capped 
	chart_ms_capped.push_back((float)frame_ms_cap);
	if (chart_ms_capped.size() > MAX_MEMORY_LOGGED)
	{
		chart_ms_capped.erase(chart_ms_capped.begin());
	}

	// RAM USAGE --
	PROCESS_MEMORY_COUNTERS counters;
	GetProcessMemoryInfo(GetCurrentProcess(), &counters, sizeof(counters));

	float ramInUse = counters.WorkingSetSize;
	ramInUse /= 1024;
	ramInUse /= 1024;

	chart_ram.push_back(ramInUse);
	if (chart_ram.size() > MAX_RAM_LOGGED)
	{
		chart_ram.erase(chart_ram.begin());
	}

	// MEMORY USAGE --
	chart_memory.push_back(m_getMemoryStatistics().totalActualMemory);
	if (chart_memory.size() > MAX_RAM_LOGGED)
		chart_memory.erase(chart_memory.begin());

	// ---------------------
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	std::list<Module*>::const_iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsEnabled())
			ret = (*item)->PreUpdate(dt);
		
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsEnabled())
			ret = (*item)->Update(dt);
	
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsEnabled())
			ret = (*item)->PostUpdate(dt);
		
		item++;
	}

	if (to_close_app)
		ret = UPDATE_STOP;

	// Set to pause after step
	if (toDoStep)
	{
		toDoStep = false; 
		SetGameState(GameState::ONPAUSE);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	std::list<Module*>::const_reverse_iterator item = list_modules.rbegin();

	while (item != list_modules.rend() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->CleanUp();
		item++;
	}

	return ret;
}

// ---------------------------------------

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

// ---------------------------------------

void Application::SetTitle(const char* name)
{
	this->engine_title = name;
	window->SetTitle(engine_title.c_str());
}

// ---------------------------------------

const char* Application::GetTitle() const
{
	return engine_title.data();
}

// ---------------------------------------

void Application::SetOrganization(const char* name)
{
	this->organization_name = name;
}

// ---------------------------------------

const char* Application::GetOrganization() const
{
	return this->organization_name.data();
}

// ---------------------------------------

void Application::SetVersion(const char* name)
{
	this->version = name;
}

// ---------------------------------------

const char* Application::GetVersion() const
{
	return this->version.data();
}


uint Application::GetFPS() const
{
	return (uint)(framerate_cap);
}
	

void Application::SetFPS(uint fps)
{
	if (fps > 0)
	{
		framerate_cap = fps;
		frame_ms_cap = 1000 / fps;
	}
	
}

std::vector<float> Application::GetFrames() const
{
	return chart_frames;
}

std::vector<float> Application::GetMS() const
{
	return chart_ms;
}

std::vector<float> Application::GetMSCapped() const
{
	return chart_ms_capped;
}

std::vector<float> Application::GetRAM() const
{
	return chart_ram;
}

std::vector<float> Application::GetMemory() const
{
	return chart_memory;
}


uint32 Application::GetCappedMS() const
{
	return frame_ms_cap;
}


float Application::GetDT() const
{
	return dt;
}

uint64 Application::GetFrameCounter() const
{
	return frame_count;
}

float Application::GetSeconds() const
{
	return seconds;
}

bool Application::GetVSYNC() const
{
	return vsync;
}


// ---------------------------------------

void Application::SendToLink(const char * link) const
{
	ShellExecuteA(NULL, "open", link, NULL, NULL, SW_SHOWNORMAL);
}

// ---------------------------------------

void Application::SetGameState(GameState state)
{
	if (state == gameState)
		return; 

	switch (state)
	{

	case GameState::ONPLAY:
		
		LOG("Starting Game Mode");
		// TODO Safe Scene

		gameTimer.Start();
		gameState = GameState::ONPLAY;
		break;

	case GameState::ONPAUSE:
		LOG("Pausing Game Mode");

		gameTimer.Pause();
		gameState = GameState::ONPAUSE;
		break;

	case GameState::ONSTOP:

		LOG("Exiting Game Mode");

		// TODO Load Scene Back

		gameTimer.Stop();
		gameState = GameState::ONSTOP;
		break; 

	}
}

// ---------------------------------------

void Application::Save()
{
	want_to_save = true;
}

void Application::Load()
{
	want_to_load = true;
}

bool Application::LoadNow()
{
	LOG("LOADING CONFIG -----------------------")

	bool ret = false;

	JSON_Value* config_file = nullptr;
	config_file = json_parse_file("config.json");

	if (config_file != nullptr) {
		LOG("config.json loaded successfully...");
		bool ret = true;

		for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret == true; it++)
		{
			JSON_Object* config_node = json_object_get_object(json_value_get_object(config_file), (*it)->name.c_str());
			ret = (*it)->Load(config_node);

		}
		json_value_free(config_file);
	}
	else {
		LOG("Error loading config.json file");
		
	}



	return ret;
}

bool Application::SaveNow()
{

	bool ret = false;

	LOG("SAVING CONFIG -----------------------")

	JSON_Value* config_file;
	JSON_Object* config;
	JSON_Object* config_node;

	config_file = json_parse_file("config.json");

	if (config_file != nullptr)
	{
		ret = true;

		config = json_value_get_object(config_file);
		config_node = json_object_get_object(config, "Application");

		// Application settings
		
		json_object_set_string(config_node, "Engine Name", App->GetTitle());
		json_object_set_string(config_node, "Organization Name", App->GetOrganization());
		json_object_set_string(config_node, "Version", App->GetVersion());
		json_object_set_number(config_node, "Max FPS", App->GetFPS());
		json_object_set_boolean(config_node, "VSYNC", App->GetVSYNC());
		
		LOG("config.json saved successfully...");
		for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end() && ret == true; it++)
		{
			config_node = json_object_get_object(config, (*it)->name.c_str());
			ret = (*it)->Save(config_node);
	
		}
		json_serialize_to_file(config_file, "config.json");
	}

	return ret;
}
