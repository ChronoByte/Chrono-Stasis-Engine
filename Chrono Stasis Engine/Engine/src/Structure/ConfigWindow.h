#pragma once

#include "csGlobals.h"
#include "imgui/imgui.h"
#include "Window.h"

class Application;

class ConfigWindow : public Window
{

public:

	ConfigWindow(Application* app);
	~ConfigWindow();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Draw() override;
	void Configuration();

private:

	char engine_name[120];
	char org_name[120];
	
};
