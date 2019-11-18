#pragma once

#include "Window.h"
#include "imgui/imgui.h"

class Folder;

class ProjectWindow : public Window
{

public:
	ProjectWindow(bool startOpened = false);
	~ProjectWindow();

	void Draw();
	
	void TestUnityProject();
	void TestChronoStasisProject();

	void DrawAssetsTree(Folder* root);

	
};
