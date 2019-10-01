#pragma once

#include "csGlobals.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "Window.h"
#include <vector>

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

	bool deletion = false;

	int total_memory = 0;
	int memory_usage = 0;
	int dedicated_memory = 0;
	int available_memory = 0;
};
