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

	void Draw();
	void Configuration();
	void Active();
	bool GetActive();
private:

	bool activeConfig = false;
	char engine_name[120];
	
};