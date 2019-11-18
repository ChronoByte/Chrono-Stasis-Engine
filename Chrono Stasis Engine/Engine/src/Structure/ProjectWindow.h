#pragma once

#include "Window.h"
#include "imgui/imgui.h"

class Folder;
struct StorageUnit;

class ProjectWindow : public Window
{

public:
	ProjectWindow(bool startOpened = false);
	~ProjectWindow();

	bool Start();

	void Draw();
	
	void TestUnityProject();
	void TestChronoStasisProject();

	void DrawAssetsTree(Folder* root);
	void ClearStorage();
private:
	std::string current_path = "";
	std::string extension = "";
	std::list<StorageUnit*> storage;
	
};
