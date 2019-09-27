#include "ConfigWindow.h"
#include "csApp.h"
#include "csWindow.h"

ConfigWindow::ConfigWindow(Application* app) : Window(app)
{	
}


ConfigWindow::~ConfigWindow() {}


bool ConfigWindow::Start() 
{
	bool ret = true;

	return ret;

}

update_status ConfigWindow::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	

	return ret;
}

void ConfigWindow::Draw()
{
	Configuration();
}

bool ConfigWindow::CleanUp()
{
	bool ret = true;

	return ret;
}

void ConfigWindow::Configuration()
{
	ImGui::Begin("Configuration", &active);

	if (ImGui::CollapsingHeader("Application"))
	{		
		strcpy_s(engine_name, 120, App->GetTitle());
	
		if (ImGui::InputText("Engine Name", engine_name, 120, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->SetTitle(engine_name);

		strcpy_s(org_name, 120, App->GetOrganization());

		if (ImGui::InputText("Organization Name", org_name, 120, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->SetOrganization(org_name);

		
		int max_fps = App->GetFPS();
		if (ImGui::SliderInt("Max FPS", &max_fps, 0, 120))
			App->SetFPS(max_fps);

		ImGui::Text("Framerate:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%i", App->GetFPS());

	}
	ImGui::End();

}