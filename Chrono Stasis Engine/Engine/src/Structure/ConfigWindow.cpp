#include "ConfigWindow.h"
#include "csApp.h"
#include "csWindow.h"

#include "SDL/include/SDL_cpuinfo.h"
#include "SDL/include/SDL_version.h"
#define TEXT_COLOR { 239, 201, 0, 255 }

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
		InputConfiguration();
		RendererConfiguration();
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

		//------- DEPENDENCIES --------//
		ImGui::Text("ImGui version: %s", ImGui::GetVersion());
		ImGui::Text("GLEW version: %i.%i.%i", GLEW_VERSION_MAJOR, GLEW_VERSION_MINOR, GLEW_VERSION_MICRO);
		ImGui::Text("MathGeoLib version: %s", MATHGEOLIB_VERSION);
		ImGui::Text("PCG version: %s", PCG_VERSION);

		ImGui::Separator();
		//------- CPU --------//
		ImGui::Text("CPU:"); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%i (Cache: %ikb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());

		//------- RAM --------//
		ImGui::Text("Memory Ram:"); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%iMb", (float)SDL_GetSystemRAM()/1024.0);
		
		
		//------- GPU --------//
		ImGui::Text("GPU:"); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%s", glGetString(GL_RENDERER));
		ImGui::Text("Brand:"); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%s", glGetString(GL_VENDOR));
		
		//-------------VRAM-------------

		ImGui::Separator();

		glGetIntegerv(GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total_memory);
		glGetIntegerv(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &available_memory);
		glGetIntegerv(GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &dedicated_memory);
		memory_usage = total_memory - available_memory;

		ImGui::Text("VRAM Budget: "); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%.4f MB", (total_memory * 0.001));

		ImGui::Text("VRAM Usage: "); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%.4f MB", (memory_usage * 0.001));

		ImGui::Text("VRAM Available: "); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%.4f MB", (available_memory * 0.001));

		ImGui::Text("VRAM Reserved: "); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%.4f MB", (dedicated_memory * 0.001));

		ImGui::Separator();

		//------- CAPACITIES --------//
		ImGui::Text("Capacities:"); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%s%s%s%s%s%s%s%s%s%s%s", (SDL_HasAVX()) ? "AVX " : "", (SDL_HasAVX2()) ? "AVX2 " : "", (SDL_HasAltiVec()) ? "AltiVec " : "",
			(SDL_Has3DNow()) ? "3DNow " : "", (SDL_HasMMX()) ? "MMX " : "", (SDL_HasRDTSC()) ? "RDTSC " : "", (SDL_HasSSE()) ? "SEE " : "",
			(SDL_HasSSE2()) ? "SSE2 " : "", (SDL_HasSSE3()) ? "SSE3 " : "", (SDL_HasSSE41()) ? "SSE41 " : "",
			(SDL_HasSSE42()) ? "SSE42 " : "");

		ImGui::Separator();
	
	}
	

}

void ConfigWindow::InputConfiguration()
{
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Mouse Position:");
		ImGui::SameLine(); 
		ImGui::TextColored(TEXT_COLOR, "%i, %i", App->input->GetMouseX(), App->input->GetMouseY());

		ImGui::Text("Mouse Motion:");
		ImGui::SameLine(); 
		ImGui::TextColored(TEXT_COLOR, "%i, %i", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());

		ImGui::Text("Mouse Wheel:");
		ImGui::SameLine(); 
		ImGui::TextColored(TEXT_COLOR, "%i", App->input->GetMouseZ());
	}

}

void ConfigWindow::RendererConfiguration()
{
	if (ImGui::CollapsingHeader("Renderer"))
	{
		static bool depth_test = glIsEnabled(GL_DEPTH_TEST);
		static bool cull_face = glIsEnabled(GL_CULL_FACE);
		static bool lighting = glIsEnabled(GL_LIGHTING);
		static bool line_smooth = glIsEnabled(GL_LINE_SMOOTH);
		static bool polygon_smooth = glIsEnabled(GL_POLYGON_SMOOTH);
		static bool color_material = glIsEnabled(GL_COLOR_MATERIAL);
		static bool texture_2D = glIsEnabled(GL_TEXTURE_2D);
		static bool wire_mode = false;

		if (ImGui::Checkbox("Depth Test", &depth_test))
		{
			if (depth_test)
				glEnable(GL_DEPTH_TEST);

			else
				glDisable(GL_DEPTH_TEST);
		}

		if (ImGui::Checkbox("Cull Face", &cull_face))
		{
			if (cull_face)
				glEnable(GL_CULL_FACE);

			else
				glDisable(GL_CULL_FACE);
		}


		if (ImGui::Checkbox("Lighting", &lighting))
		{
			if (lighting)
				glEnable(GL_LIGHTING);

			else
				glDisable(GL_LIGHTING);

		}

		if (ImGui::Checkbox("Smooth Lines", &line_smooth))
		{
			if (line_smooth)
				glEnable(GL_LINE_SMOOTH);

			else
				glDisable(GL_LINE_SMOOTH);

		}

		if (ImGui::Checkbox("Smooth Polygons", &polygon_smooth))
		{
			if (polygon_smooth)
				glEnable(GL_POLYGON_SMOOTH);

			else
				glDisable(GL_POLYGON_SMOOTH);

		}

		if (ImGui::Checkbox("Color Material", &color_material))
		{
			if (color_material)
				glEnable(GL_MATRIX_MODE);

			else
				glDisable(GL_MATRIX_MODE);

		}

		if (ImGui::Checkbox("Texture 2D", &texture_2D))
		{
			if (texture_2D)
				glEnable(GL_TEXTURE_2D);

			else
				glDisable(GL_TEXTURE_2D);

		}


		if (ImGui::Checkbox("Wire Mode", &wire_mode))
		{
			if (wire_mode)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		}
	}

}