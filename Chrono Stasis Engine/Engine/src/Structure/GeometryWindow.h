#pragma once

#include "csGlobals.h"
#include "imgui/imgui.h"
#include "Window.h"

#include "MathGeoLib/include/MathGeoLib.h"

class Application;

class GeometryWindow : public Window
{

public:

	GeometryWindow(bool startOpened = false);
	~GeometryWindow();

	void Draw() override;
};
