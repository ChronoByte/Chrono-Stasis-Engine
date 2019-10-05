#include "ConfigWindow.h"
#include "csApp.h"
#include "csWindow.h"


#include "SDL/include/SDL_cpuinfo.h"
#include "SDL/include/SDL_version.h"
#include "gpudetect/DeviceId.h"
#include "mmgr/mmgr.h"

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

	depth_test = App->renderer3D->GetDepthTest();
	cull_face = App->renderer3D->GetCullFace();
	lighting = App->renderer3D->GetLighting();
	line_smooth = App->renderer3D->GetSmoothLine();
	polygon_smooth = App->renderer3D->GetSmoothPolygon();
	color_material = App->renderer3D->GetColorMaterial();
	texture_2D = App->renderer3D->GetTexture2D();
	wire_mode = App->renderer3D->GetWireframe();

	ambient_model_light = App->renderer3D->GetLightModelAmbient();
	ambient_light = App->renderer3D->GetAmbientLight();
	diffuse_light = App->renderer3D->GetDiffuseLight();
	specular_light = App->renderer3D->GetSpecularLight();
	fog = App->renderer3D->GetFog();

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
		GUIConfiguration();
		HardwareConfiguration();
		InputConfiguration();
		RendererConfiguration();
	}
	ImGui::End();

}

void ConfigWindow::DockDraw() {

		AppConfiguration();
		WindowConfiguration();
		GUIConfiguration();
		HardwareConfiguration();
		InputConfiguration();
		RendererConfiguration();
	
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
				sprintf_s(title, 25, "Framerate: %.1f", flowFrames[flowFrames.size() - 1]);
				ImGui::PlotHistogram("##Framerate:", &flowFrames[0], flowFrames.size(), 0, title, 70.0f, 0.0f, ImVec2(0, 100));
			}

			flowMSCap = App->GetMSCapped();

			if (!flowMSCap.empty())
			{
				char title[25];
				sprintf_s(title, 25, "Milliseconds Cap: %0.1f", flowMSCap[flowMSCap.size() - 1]);
				ImGui::PlotHistogram("##Milliseconds Cap:", &flowMSCap[0], flowMSCap.size(), 0, title, 70.0f, 0.0f, ImVec2(0, 100));
			}

			flowMS = App->GetMS();

			if (!flowMS.empty())
			{
				char title[25];
				sprintf_s(title, 25, "Milliseconds: %0.1f", flowMS[flowMS.size() - 1]);
				ImGui::PlotHistogram("##Milliseconds:", &flowMS[0], flowMS.size(), 0, title, 70.0f, 0.0f, ImVec2(0, 100));
			}
			
			flowRAM = App->GetRAM();

			if (!flowRAM.empty()) {
				char title[25];
				sprintf_s(title, 25, "RAM Usage: %.1f", flowRAM[flowRAM.size() - 1]);
				ImGui::PlotHistogram("##RAM", &flowRAM[0], flowRAM.size(), 0, title, 0.0f, 125.0f, ImVec2(0, 100));
			}

			ImGui::Separator();

			sMStats memStats = m_getMemoryStatistics();

			flowMemory = App->GetMemory();
			
			if (!flowMemory.empty()) {
				char title[50];
				sprintf_s(title, 50, "Memory Consumption: %.1f",flowMemory[flowMemory.size()-1]);
				ImGui::PlotHistogram("##Memory", &flowMemory[0], flowMemory.size(), 0, title, 0.0f, 15000.0f, ImVec2(0, 100));
			}

			ImGui::Text("Total Reported Mem:"); ImGui::SameLine();
			ImGui::TextColored(TEXT_COLOR, " %u", memStats.totalReportedMemory);
			ImGui::Text("Total Actual Mem:"); ImGui::SameLine();
			ImGui::TextColored(TEXT_COLOR, "%u", memStats.totalActualMemory);
			ImGui::Text("Peak Reported Mem:"); ImGui::SameLine();
			ImGui::TextColored(TEXT_COLOR, "%u", memStats.peakReportedMemory);
			ImGui::Text("Peak Actual Mem:"); ImGui::SameLine();
			ImGui::TextColored(TEXT_COLOR, "%u", memStats.peakActualMemory);
			ImGui::Text("Accumulated Reported Mem:"); ImGui::SameLine();
			ImGui::TextColored(TEXT_COLOR, "%u", memStats.accumulatedReportedMemory);
			ImGui::Text("Accumulated Actual Mem:"); ImGui::SameLine();
			ImGui::TextColored(TEXT_COLOR, "%u", memStats.accumulatedActualMemory);
			ImGui::Text("Acumulated Alloc Unit Count:"); ImGui::SameLine();
			ImGui::TextColored(TEXT_COLOR, "%u", memStats.accumulatedAllocUnitCount);
			ImGui::Text("Total Alloc Unit Count:"); ImGui::SameLine();
			ImGui::TextColored(TEXT_COLOR, "%u", memStats.totalAllocUnitCount);
			ImGui::Text("Peak Alloc Unit Count:"); ImGui::SameLine();
			ImGui::TextColored(TEXT_COLOR, "%u", memStats.peakAllocUnitCount);

			ImGui::Separator();
		
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
		ImGui::TextColored(TEXT_COLOR, "%.2fGB", (float)SDL_GetSystemRAM() / 1024);
		
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&memInfo);
		DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
		float totalRamInUse = physMemUsed / 1024;
		totalRamInUse /= 1024;
		totalRamInUse /= 1024;

		ImGui::Text("Total RAM in use:"); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%.2f GB", totalRamInUse);
		
		ImGui::Separator();

		//------- GPU --------//
		ImGui::Text("GPU:"); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%s", glGetString(GL_RENDERER));
		ImGui::Text("Brand:"); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%s", glGetString(GL_VENDOR));

		//------- VRAM --------// (detect gpu!)
		getGraphicsDeviceInfo(&vendor_id, &device_id, &brand, &mp_budget, &mb_usage, &mb_available, &vmb_reserved);

		////MB
		mp_budget /= (1024 * 1024);
		mb_usage /= (1024 * 1024);
		mb_available /= (1024 * 1024);
		vmb_reserved /= (1024 * 1024);

		ImGui::Text("Vendor ID: %d", vendor_id); 
		ImGui::Text("Device ID: %d", device_id); 

		ImGui::NewLine();

		static char c_brand[300];
		sprintf_s(c_brand, 300, "%S", brand.c_str());
		ImGui::Text("GPU Brand: %s", c_brand); 
		ImGui::NewLine();

		ImGui::Text("Video Memory:"); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR,"%d MB", mp_budget); 
		ImGui::Text("Video Memory On Use:"); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR,"%d MB", mb_usage);
		ImGui::Text("Video Memory Available: "); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR,"%d MB", mb_available);
		ImGui::Text("Video Memory Reserved:"); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR,"%d MB", vmb_reserved);
		
		
		//------- VRAM --------// (doesnt detect gpu)

		ImGui::Separator();
		
		glGetIntegerv(GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total_memory);
		glGetIntegerv(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &available_memory);
		glGetIntegerv(GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &dedicated_memory);
		memory_usage = total_memory - available_memory;

		ImGui::Text("VRAM Budget: "); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%.2f MB", (float)total_memory / 1024);

		ImGui::Text("VRAM available: "); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%.2f MB", (float)available_memory / 1024);

		ImGui::Text("VRAM in use: "); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%.2f MB", ((float)total_memory / 1024) - (float)available_memory / 1024);

		ImGui::Text("VRAM Reserved: "); ImGui::SameLine();
		ImGui::TextColored(TEXT_COLOR, "%.4f MB", ((float)dedicated_memory / 1024)); 

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

		ImGui::Text("Mouse pressed: %s", App->input->GetIsMousePressed() == true ? "Yes" : "No");
		ImGui::Text("Key pressed: %s", App->input->GetIsKeyPressed() == true ? "Yes" : "No");
	}

}

void ConfigWindow::RendererConfiguration()
{
	if (ImGui::CollapsingHeader("Renderer"))
	{
		
		if (ImGui::Checkbox("Depth Test", &depth_test))
			App->renderer3D->SetDepthTest(depth_test); ImGui::SameLine(200.0f);

		if (ImGui::Checkbox("Cull Face", &cull_face))
			App->renderer3D->SetCullFace(cull_face); ImGui::SameLine(400.0f);
		
		if (ImGui::Checkbox("Ambient Model Light", &ambient_model_light)) {}

		if (ImGui::Checkbox("Lighting", &lighting)) 
			App->renderer3D->SetLighting(lighting); ImGui::SameLine(200.0f);

		if (ImGui::Checkbox("Smooth Lines", &line_smooth)) 
			App->renderer3D->SetSmoothLine(line_smooth); ImGui::SameLine(400.0f);
		
		if (ImGui::Checkbox("Diffuse Light", &diffuse_light)) {}

		if (ImGui::Checkbox("Smooth Polygons", &polygon_smooth))
			App->renderer3D->SetSmoothPolygon(polygon_smooth); ImGui::SameLine(200.0f);

		if (ImGui::Checkbox("Color Material", &color_material))
			App->renderer3D->SetColorMaterial(color_material); ImGui::SameLine(400.0f);

		if (ImGui::Checkbox("Specular Light", &specular_light)) {}

		if (ImGui::Checkbox("Texture 2D", &texture_2D))
			App->renderer3D->SetColorMaterial(texture_2D); ImGui::SameLine(200.0f);

		if (ImGui::Checkbox("Wire Mode", &wire_mode))
			App->renderer3D->SetWireframe(wire_mode); ImGui::SameLine(400.0f);

		if (ImGui::Checkbox("Fog", &fog))
			App->renderer3D->SetFog(fog);


		ImGui::Separator();

		if (ambient_model_light)
		{
			if (ImGui::TreeNode("Ambient Model Light Color"))
			{
				ImGui::ColorPicker4("Ambient Model Color Picker", ambient_color,
					ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_AlphaPreview);

				ImGui::TreePop();
			}
				App->renderer3D->SetLightModelAmbient(ambient_model_light, ambient_color);
		}
		else {
			App->renderer3D->SetLightModelAmbient(ambient_model_light, default_color);
		}

		if (diffuse_light)
		{
			if (ImGui::TreeNode("Diffuse Light Color"))
			{
				ImGui::ColorPicker4("Diffuse Color Picker", diffuse_color,
					ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_AlphaPreview);

				ImGui::TreePop();
			}
				App->renderer3D->SetDiffuseLight(ambient_model_light, diffuse_color);
		}
		else {
			App->renderer3D->SetDiffuseLight(ambient_model_light, default_color);
		}

		if (specular_light)
		{
			if (ImGui::TreeNode("Specular Light Color"))
			{
				ImGui::ColorPicker4("Specular Color Picker", specular_color,
					ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_AlphaPreview);

				ImGui::TreePop();
			}
				App->renderer3D->SetSpecularLight(ambient_model_light, specular_color);
		}
		else {
			App->renderer3D->SetSpecularLight(ambient_model_light, default_color);
		}

	}

}

void ConfigWindow::GUIConfiguration()
{

	if (ImGui::CollapsingHeader("GUI"))
	{
		ImGuiStyle& style = ImGui::GetStyle();

		ImGui::ShowStyleSelector("ImGui Style");
		
		static int style_idx = -1;
		if (ImGui::Combo("Custom Style", &style_idx, "Orange\0Black\0Light\0"))
		{
			switch (style_idx)
			{
			case 0: App->gui->StyleLoader("ORANGE"); break;
			case 1: App->gui->StyleLoader("BLACK"); break;
			case 2: ImGui::StyleColorsLight(); break;
			}
			
		}

		if (ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f))
		{
			style.GrabRounding = style.FrameRounding;
		}

		bool window_border = style.WindowBorderSize;
		if (ImGui::Checkbox("WindowBorder", &window_border))
			style.WindowBorderSize = window_border;
		
		ImGui::SameLine();

		bool frame_border = style.FrameBorderSize;
		if (ImGui::Checkbox("FrameBorder", &frame_border))
			style.FrameBorderSize = frame_border;

	}



}