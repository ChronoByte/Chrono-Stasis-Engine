#pragma once

#include "csGlobals.h"
#include "Window.h"
#include "imgui/imgui.h"
#include "Resource.h"
#include <vector>
#define PREVIEW_SIZE 75

class TextureBrowserWindow : public Window
{

public:

	TextureBrowserWindow(bool startOpened = false);
	~TextureBrowserWindow();

	void Draw() override;
	

private:
	std::vector<Resource*> resources;
};


