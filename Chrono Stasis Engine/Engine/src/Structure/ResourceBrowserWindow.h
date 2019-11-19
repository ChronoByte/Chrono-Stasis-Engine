#pragma once

#include "csGlobals.h"
#include "imgui/imgui.h"
#include "Window.h"
#include "Resource.h"
#include <vector>

class Component;

class ResourceBrowserWindow : public Window
{

public:

	ResourceBrowserWindow(bool startOpened = false);
	~ResourceBrowserWindow();

	void Draw() override;

	void OpenExplorer(Resource::Type type, float x, float y, Component* callback);
public:
	Resource::Type currentType = Resource::R_UNKNOWN;
	std::vector<Resource*> resources;
	Component* callbackComponent = nullptr;
};
