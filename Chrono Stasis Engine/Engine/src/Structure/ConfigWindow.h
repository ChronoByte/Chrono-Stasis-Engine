#pragma once

#include "csGlobals.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "Window.h"
#include <vector>

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
};
