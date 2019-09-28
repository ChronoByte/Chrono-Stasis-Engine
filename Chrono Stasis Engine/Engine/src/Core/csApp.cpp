#include "csApp.h"

Application::Application()
{
	// TODO: Load with JSON
	engine_title = TITLE;
	organization_name = ORGANIZATION;
	version = "v0.0.1";
	framerate_cap = 30;
	frame_ms_cap = 1000 / framerate_cap;

	window		= new ModuleWindow();
	input		= new ModuleInput();
	editor		= new ModuleEditor();
	renderer3D	= new ModuleRenderer3D();
	camera		= new ModuleCamera3D();
	gui			= new ModuleUI();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	// Scenes
	AddModule(editor);

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
}

bool Application::Init()
{
	bool ret = true;


	// Call Init() in all modules
	std::list<Module*>::const_iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if((*item)->IsEnabled())
			ret = (*item)->Init();

		item++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsEnabled())
			ret = (*item)->Start();

		item++;
	}
	
	startup_time.Start();
	
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;
	frame_time.Start(); 

	dt = (1 / (float)framerate_cap); // Differential Time
}

// ---------------------------------------------
void Application::FinishUpdate()
{
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


// ---------------------------------------


void Application::SendToLink(const char * link) const
{
	ShellExecuteA(NULL, "open", link, NULL, NULL, SW_SHOWNORMAL);
}
