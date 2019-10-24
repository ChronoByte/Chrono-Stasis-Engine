#pragma once

#include "csGlobals.h"
#include "Window.h"
#include "imgui/imgui.h"


class Application;

class SceneViewWindow : public Window
{

public:

	SceneViewWindow(bool startOpened = false);
	~SceneViewWindow();

	void Draw() override;

private:
	

};


