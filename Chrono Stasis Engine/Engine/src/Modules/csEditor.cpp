#include "csGlobals.h"
#include "csApp.h"
#include "csEditor.h"
#include <string>
#include "time.h"
#include "csInput.h"
#include "imgui/imgui.h"
#include "imgui/imgui_dock.h"

#include "csMesh.h"

// Including windows
#include "src/Structure/ConfigWindow.h"
#include "src/Structure/GeometryWindow.h"
#include "src/Structure/AboutWindow.h"
#include "src/Structure/ConsoleWindow.h"
#include "src/Structure/RandomWindow.h"
#include "src/Structure/InspectorWindow.h"
ModuleEditor::ModuleEditor(bool start_enabled) : Module(start_enabled)
{
	name = "Editor";

	config = new ConfigWindow();
	geometryWin = new GeometryWindow();
	about = new AboutWindow();	
	console = new ConsoleWindow(true);
	randomWin = new RandomWindow(); 

	windowManager.push_back(new InspectorWindow());

}

ModuleEditor::~ModuleEditor()
{
}

// Load assets
bool ModuleEditor::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	
	config->Start();

	windowManager[INSPECTOR]->Activate();

	//START ALL WINDOWS
	std::vector<Window*>::iterator item = windowManager.begin();
	for (int i = 0; i < windowManager.size(); i++)
		item[i]->Start();
	
	
	return ret;
}

// Load assets
bool ModuleEditor::CleanUp()
{
	LOG("Unloading Intro scene");

	delete config; 
	config = nullptr; 

	delete geometryWin;
	geometryWin = nullptr; 

	delete about;
	about = nullptr; 

	delete console; 
	console = nullptr; 
	
	return true;
}

void ModuleEditor::Log(char * log) const
{
	if (console != nullptr)
		console->PushLog(log);
}

// Update
update_status ModuleEditor::Update(float dt)
{
	/*static bool show_global = true;

	static int width;
	static int height;
	SDL_GetWindowSize(App->window->window, &width, &height);
	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(width, height - 20));


	ImGui::Begin("MasterWindow", &show_global, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);

	ImGui::BeginDockspace();
	inspector->Draw();
	ImGui::EndDockspace();

	ImGui::End();*/

	
	//ImGui::SetNextWindowPos(ImVec2(App->window->width - 500.0f, App->window->height - 205.0f), ImGuiCond_Once); //Imaginary numbers are from size params!
	//ImGui::SetNextWindowSize(ImVec2(500.0f, 205.0f), ImGuiCond_Once);
	//if (ImGui::Begin("Dock Demo"))
	//{
	//	// dock layout by hard-coded or .ini file
	//	ImGui::BeginDockspace();

	//	if (ImGui::BeginDock("Dock 1")) {
	//		ImGui::Text("Sebas");
	//	}
	//	ImGui::EndDock();

	//	if (ImGui::BeginDock("Dock 2")) {
	//		ImGui::Text("Carlos");
	//	}
	//	ImGui::EndDock();

	//	if (ImGui::BeginDock("Dock 3")) {
	//		ImGui::Text("E.T.");
	//	}
	//	ImGui::EndDock();
	//	if (ImGui::BeginDock("Configuration Window")) {
	//		config->DockDraw();
	//	}
	//	ImGui::EndDock();

	//	ImGui::EndDockspace();
	//}
	//ImGui::End();

	
	update_status ret = UPDATE_CONTINUE; 

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Close", "Ctrl+W")) { App->to_close_app = true; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Demo Window", "",  &show_demo_window))	{}
			if (ImGui::MenuItem("Console", "", console->GetBool()))	{}
			if (ImGui::MenuItem("Inspector", "", windowManager[INSPECTOR]->GetBool())) {}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Configuration"))
		{
			config->Activate();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Collision Tester", "", geometryWin->GetBool())) {}
			if (ImGui::MenuItem("Random Tester", "", randomWin->GetBool())) {}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			SetHelpMenu();
			ImGui::EndMenu(); 
		}

		ImGui::EndMainMenuBar();
	}

	if (config->GetActive())
		config->Draw();

	if (geometryWin->GetActive())
		geometryWin->Draw();

	if (about->GetActive())
		about->Draw(); 

	if (console->GetActive())
		console->Draw(); 

	if (randomWin->GetActive())
		randomWin->Draw();

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	std::vector<Window*>::iterator item = windowManager.begin();
	for (int i = 0; i < windowManager.size(); i++)
	{
		if (item[i]->GetBool())
			item[i]->Draw();
	}

	return ret;
}

void ModuleEditor::SetHelpMenu()
{
	if (ImGui::MenuItem("Documentation"))
		App->SendToLink("https://github.com/ChronoByte/Chrono-Stasis-Engine/wiki");

	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Hand);

	if (ImGui::MenuItem("Download Latest Version"))
		App->SendToLink("https://github.com/ChronoByte/Chrono-Stasis-Engine/releases");

	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Hand);

	if (ImGui::MenuItem("Report a bug"))
		App->SendToLink("https://github.com/ChronoByte/Chrono-Stasis-Engine/issues");

	if (ImGui::IsItemHovered())
		ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_Hand);

	if (ImGui::MenuItem("About"))
		about->SwitchActive();
}

void ModuleEditor::CreateCube(const vec3& position, const uint & length, const uint & width, const uint & height)
{
	par_shapes_mesh* mesh = nullptr;
	mesh = par_shapes_create_cube();
	par_shapes_translate(mesh, position.x, position.y, position.z);
	par_shapes_scale(mesh, length, height, width);

	LOG("Created a Cube in position: \nx = %.2f, y = %.2f, z = %.2f", position.x, position.y, position.y);

	Mesh* shape = new Mesh(mesh);

	App->renderer3D->PushMeshToRender(shape);
}

//void ModuleEditor::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
//{
//	LOG("Hit!");
//}

