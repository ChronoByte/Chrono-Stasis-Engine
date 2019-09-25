#pragma once

#include "csGlobals.h"
#include "imgui/imgui.h"
#include "csModule.h"

class Application;

class ConfigWindow 
{

public:

	ConfigWindow();
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
	Application* App; //temporal
};
