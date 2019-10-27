#pragma once

#include "csGlobals.h"
#include "Window.h"
#include "imgui/imgui.h"


class Application;

class InspectorWindow : public Window
{

public:

	InspectorWindow(bool startOpened = false);
	~InspectorWindow();

	void Draw() override;
	
private:
	 
	
};

