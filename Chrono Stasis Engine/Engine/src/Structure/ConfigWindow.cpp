#include "ConfigWindow.h"
#include "csApp.h"
#include "csWindow.h"

ConfigWindow::ConfigWindow() {}


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
	//std::string name = TITLE;
	if (ImGui::CollapsingHeader("Application"))
	{

		static char str0[128] = "Hello, world!";
		
		ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));

		/*std::string name(TITLE);
		strcpy_s(engine_name, 120, name.c_str());
		ImGui::InputText("Engine Name", engine_name,120);*/
		//if (ImGui::InputText("Engine Name", engine_name, 120, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			//name = engine_name;
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