#include "csGlobals.h"
#include "csApp.h"
#include "csModuleUI.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleUI::~ModuleUI()
{}

// Called before render is available
bool ModuleUI::Init()
{
	LOG("Creating Module UI context");
	bool ret = true;

	//Create context
	IMGUI_CHECKVERSION();
	if(ImGui::CreateContext() != NULL)
	{
		ImGuiIO& io = ImGui::GetIO();
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
		ImGui_ImplOpenGL2_Init();

		// Setup style
		ImGui::StyleColorsDark();
	}
	else
	{
		ret = false; 
		LOG("There was an error creating ImGUI context. SDL_Error: %s", SDL_GetError());
	}
 
	return true; 
}

// Called every draw update
update_status ModuleUI::PreUpdate(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate(float dt)
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleUI::CleanUp()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}