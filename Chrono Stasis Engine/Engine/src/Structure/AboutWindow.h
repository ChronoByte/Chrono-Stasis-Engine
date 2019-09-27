#pragma once

#include "csGlobals.h"
#include "imgui/imgui.h"
#include "Window.h"

class Application;

class AboutWindow : public Window
{

public:

	AboutWindow(Application* app);
	~AboutWindow();

	void Draw() override;

	void About();
	void Introduction(); 
	void MadeBy(); 
	void SoftwareUsed();
	void License(); 
};
