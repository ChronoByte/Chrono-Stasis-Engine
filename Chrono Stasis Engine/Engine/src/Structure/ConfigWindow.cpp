#include "ConfigWindow.h"
#include "csApp.h"
#include "csWindow.h"

#include "SDL/include/SDL_cpuinfo.h"
#include "SDL/include/SDL_version.h"
#define TEXT_COLOR { 239, 201, 0, 255 }

#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX			0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX		0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX    0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX				0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX				0x904B

ConfigWindow::ConfigWindow(bool startOpened) : Window(startOpened)
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
		HardwareConfiguration();
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

			ImGui::Separator();

			ImGui::Text("Framerate:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%i FPS", App->GetFPS());
			
			ImGui::SameLine(200.0f);
			
			ImGui::Text("Frame Counter:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%i frames", App->GetFrameCounter());

			ImGui::NewLine();

			ImGui::Text("Capped ms:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%i ms", App->GetCappedMS());
			
			ImGui::SameLine(200.0f);
			
			ImGui::Text("Seconds since App Init:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%.2f s", App->GetSeconds());
		
			ImGui::NewLine();

			ImGui::Text("Differential Time:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%.3f s", App->GetDT());

			ImGui::Separator();
			



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

void ConfigWindow::HardwareConfiguration()
{


	if (ImGui::CollapsingHeader("Hardware"))
	{

		//------- SDL & OpenGL --------//
		SDL_version version;
		SDL_GetVersion(&version);

		ImGui::Text("OpenGL version: %s", glGetString(GL_VERSION));
		ImGui::Text("SDL version: %i.%i.%i", version.major, version.minor, version.patch);

		ImGui::Separator();

		//------- CPU --------//
		ImGui::Text("CPU:"); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%i (Cache: %ikb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

		//------- RAM --------//
		ImGui::Text("Memory Ram:"); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%iMb", SDL_GetSystemRAM());
		
		
		//------- GPU --------//
		ImGui::Text("GPU:"); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%s", glGetString(GL_RENDERER));
		ImGui::Text("Brand:"); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%s", glGetString(GL_VENDOR));
		
		//------- CAPACITIES --------//
		ImGui::Text("Capacities:"); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%s%s%s%s%s%s%s%s%s%s%s", (SDL_HasAVX()) ? "AVX " : "", (SDL_HasAVX2()) ? "AVX2 " : "", (SDL_HasAltiVec()) ? "AltiVec " : "",
			(SDL_Has3DNow()) ? "3DNow " : "", (SDL_HasMMX()) ? "MMX " : "", (SDL_HasRDTSC()) ? "RDTSC " : "", (SDL_HasSSE()) ? "SEE " : "",
			(SDL_HasSSE2()) ? "SSE2 " : "", (SDL_HasSSE3()) ? "SSE3 " : "", (SDL_HasSSE41()) ? "SSE41 " : "",
			(SDL_HasSSE42()) ? "SSE42 " : "");

		ImGui::Separator();
	
	}
	
}