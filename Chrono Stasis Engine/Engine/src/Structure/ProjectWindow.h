#pragma once

#include "Window.h"
#include "imgui/imgui.h"

class ProjectWindow : public Window
{

public:
	ProjectWindow(bool startOpened = false);
	~ProjectWindow();

	void Draw();
	



};
