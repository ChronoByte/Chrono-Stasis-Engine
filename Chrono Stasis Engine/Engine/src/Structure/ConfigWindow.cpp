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

	App->window->GetWindowSize(window_width, window_height);
	refresh_rate = App->window->GetRefreshRate();
	window_size = App->window->GetSize();
	brightness = App->window->GetBrightness();
	fullscreen = App->window->GetFullscreenWindow();
	resizable = App->window->GetResizableWindow();
	borderless = App->window->GetBorderlessWindow();
	fulldekstop = App->window->GetFullDesktopWindow();
	maximized = App->window->GetMaximized();

	return ret;

}

update_status ConfigWindow::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	

	return ret;
}

void ConfigWindow::Draw()
{
	if (ImGui::Begin("Configuration", &active))
	{
		AppConfiguration();
		WindowConfiguration();
	}
	ImGui::End();

}

bool ConfigWindow::CleanUp()
{
	bool ret = true;

	return ret;
}

void ConfigWindow::AppConfiguration()
{
	
		if (ImGui::CollapsingHeader("Application"))
		{		
			strcpy_s(engine_name, 120, App->GetTitle());
	
			if (ImGui::InputText("Engine Name", engine_name, 120, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
				App->SetTitle(engine_name);

			strcpy_s(org_name, 120, App->GetOrganization());

			if (ImGui::InputText("Organization Name", org_name, 120, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
				App->SetOrganization(org_name);

			strcpy_s(version_name, 120, App->GetVersion());

			if (ImGui::InputText("Version", version_name, 254))
				App->SetVersion(version_name);
			
			int max_fps = App->GetFPS();
			if (ImGui::InputInt("Max FPS", &max_fps))
				App->SetFPS(max_fps);

			ImGui::Text("Framerate:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%i", App->GetFPS());

			flowFrames = App->GetFrames();

			if (!flowFrames.empty())
			{
				char title[25];
				sprintf_s(title, 25, "%.1f", flowFrames[flowFrames.size() - 1]);
				ImGui::PlotHistogram("Framerate ChartFlow", &flowFrames[0], flowFrames.size(), 0, title, 70.0f, 0.0f, ImVec2(0, 100));
			}

			flowMS = App->GetMS();

			if (!flowMS.empty())
			{
				char title[25];
				sprintf_s(title, 25, "%0.1f", flowMS[flowMS.size() - 1]);
				ImGui::PlotHistogram("ms ChartFlow", &flowMS[0], flowMS.size(), 0, title, 70.0f, 0.0f, ImVec2(0, 100));
			}
		//TODO: Memory consumption
		}
}


void ConfigWindow::WindowConfiguration()
{

	if (ImGui::CollapsingHeader("Window"))
	{
		if (ImGui::InputInt("Width", &window_width, 100))
			App->window->SetWindowSize(window_width, window_height);

		
		if (ImGui::InputInt("Height", &window_height, 100))
			App->window->SetWindowSize(window_width, window_height);

		if (ImGui::InputInt("Size", &window_size, 1))
			App->window->SetSize(window_size, window_width, window_height);
	
		if (ImGui::Checkbox("Fullscreen", &fullscreen))
			App->window->SetFullscreen(fullscreen);

		ImGui::SameLine();

		if (ImGui::Checkbox("Resizable", &resizable))
			App->window->SetResizable(resizable);

		
		if (ImGui::Checkbox("Borderless", &borderless))
			App->window->SetBorderless(borderless);

		ImGui::SameLine();

		if (ImGui::Checkbox("Full Dekstop", &fulldekstop))
			App->window->SetFullScreenDesktop(fulldekstop);

		
		if (ImGui::Checkbox("Maximized", &maximized))
			App->window->SetMaximized(maximized);

		
		if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 2.0f))
			App->window->SetBrightness(brightness);
		
		ImGui::Text("Refresh rate:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "%u", refresh_rate);

		if (ImGui::Checkbox("Delete App :) ", &deletion))
			delete App;
		
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Pulsa la checkbox para que reviente el programa :) ");
	}
}