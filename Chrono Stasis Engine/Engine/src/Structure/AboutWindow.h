#pragma once

#include "csGlobals.h"
#include "imgui/imgui.h"
#include "Window.h"

class Application;

class AboutWindow : public Window
{

public:

	AboutWindow(bool startOpened = false);
	~AboutWindow();

	void Draw() override;

	void Introduction(); 
	void MadeBy(); 
	void SoftwareUsed();
	void License(); 
};
