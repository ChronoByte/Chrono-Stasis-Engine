#pragma once

#include "csGlobals.h"
#include "Window.h"
#include "imgui/imgui.h"


class Application;

class GameViewWindow : public Window
{

public:

	GameViewWindow(bool startOpened = false);
	~GameViewWindow();

	void Draw() override;
	
private:
	

};


