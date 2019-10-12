#pragma once

#include "csGlobals.h"
#include "Window.h"
#include "imgui/imgui.h"
#include "imgui/imgui_dock.h"

class Application;

class InspectorWindow : public Window
{

public:

	InspectorWindow(bool startOpened = false);
	~InspectorWindow();

	void Draw() override;

	void ShowInspector();
private:
	bool activate = true;
};

