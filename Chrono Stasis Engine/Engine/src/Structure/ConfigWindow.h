#pragma once

#include "csGlobals.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "Window.h"
#include <vector>
#include <string>


#define PCG_VERSION "0.98"
#define MATHGEOLIB_VERSION "1.5"
#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX			0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX		0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX    0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX				0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX				0x904B


class Application;

class ConfigWindow : public Window
{

public:

	ConfigWindow(bool startOpened = false);
	~ConfigWindow();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Draw() override;
	void DockDraw();
	void AppConfiguration();
	void WindowConfiguration();
	void HardwareConfiguration();
	void InputConfiguration();
	void RendererConfiguration();
	void GUIConfiguration();
private:

	char engine_name[120];
	char org_name[120];
	char version_name[120];

	std::vector<float> flowFrames;
	std::vector<float> flowMS;
	std::vector<float> flowRAM;
	std::vector<float> flowMSCap;
	std::vector<float> flowMemory;

	int  window_width = 0;
	int  window_height = 0;
	int window_size = 1;
	uint refresh_rate = 0;

	bool fullscreen = false;
	bool resizable = false;
	bool borderless = false;
	bool fulldekstop = false;
	bool maximized = false;

	float brightness = 0.0f;

	int total_memory = 0;
	int memory_usage = 0;
	int dedicated_memory = 0;
	int available_memory = 0;

	bool depth_test = false;
	bool cull_face = false;
	bool lighting = false;
	bool line_smooth = false;
	bool polygon_smooth = false;
	bool color_material = false;
	bool texture_2D = false;
	bool wire_mode = false;
	bool ambient_model_light = false;
	bool ambient_light = false;
	bool diffuse_light = false;
	bool specular_light = false;
	bool fog = false;

	uint vendor_id;
	uint device_id;
	uint64 mp_budget;
	uint64 mb_usage;
	uint64 mb_available;
	uint64 vmb_reserved;
	std::wstring brand;

public:
	float ambient_color[4] = { 0.25f, 0.25f, 0.25f, 1.0f };
	float diffuse_color[4] = { 0.25f, 0.25f, 0.25f, 1.0f };
	float specular_color[4] = { 0.25f, 0.25f, 0.25f, 1.0f };

	float ambient_model_color[4] = { 0.25f, 0.25f, 0.25f, 1.0f };
	float default_color[4] = { 0.25f, 0.25f, 0.25f, 1.0f };
	
};
