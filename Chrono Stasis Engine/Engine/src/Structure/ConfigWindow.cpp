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
	ImGui::Begin("Configuration", &activeConfig);

	if (ImGui::CollapsingHeader("Application"))
	{

		
		strcpy_s(engine_name, 120, App->GetTitle());
	
		if (ImGui::InputText("Engine Name", engine_name, 120, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->window->SetTitle(engine_name);
	}
	ImGui::End();
}

void ConfigWindow::Active() 
{
	activeConfig = true;
	
}

bool ConfigWindow::GetActive()
{
	return activeConfig;
}