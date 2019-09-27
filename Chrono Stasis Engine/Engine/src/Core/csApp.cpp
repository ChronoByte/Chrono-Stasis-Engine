#include "csApp.h"

Application::Application()
{
	// TODO: Load with JSON
	engine_title = TITLE;
	organization_name = ORGANIZATION;

	frames = 0;
	last_frame_ms = -1;
	last_fps = -1;
	capped_ms = 1000 / 60;
	fps_counter = 0;

	window		= new ModuleWindow(this);
	input		= new ModuleInput(this);
	editor		= new ModuleEditor(this);
	renderer3D	= new ModuleRenderer3D(this);
	camera		= new ModuleCamera3D(this);
	gui			= new ModuleUI(this);

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
		ret = (*item)->Init();
		item++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Start();
		item++;
	}

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	// Framerate calculations --

	++frames;
	++fps_counter;

	if (fps_timer.Read() >= 1000)
	{
		last_fps = fps_counter;
		fps_counter = 0;
		fps_timer.Start();
	}

	last_frame_ms = ms_timer.Read();

	// cap fps
	if (capped_ms > 0 && (last_frame_ms < capped_ms))
		SDL_Delay(capped_ms - last_frame_ms);



}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	std::list<Module*>::const_iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
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


uint Application::GetFPS() const
{
	if (capped_ms > 0)
		return (uint)((1.0f / (float)capped_ms) * 1000.0f);
	else
		return 0;
}

void Application::SetFPS(uint max_fps)
{
	if (max_fps > 0)
		capped_ms = 1000 / max_fps;
	else
		capped_ms = 0;
}

// ---------------------------------------


void Application::SendToLink(const char * link) const
{
	ShellExecuteA(NULL, "open", link, NULL, NULL, SW_SHOWNORMAL);
}
