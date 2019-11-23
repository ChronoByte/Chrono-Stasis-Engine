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
	float width = 0.f;
	float height = 0.f;

	float regionX = 0.f;
	float regionY = 0.f;

};


