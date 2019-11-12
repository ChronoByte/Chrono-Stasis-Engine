#pragma once

#include "csGlobals.h"
#include "imgui/imgui.h"
#include "Window.h"
#include <string>

enum BrowserState
{
	NONE_SCENE = 0,

	SAVE_SCENE,
	SAVE_SCENE_AS,
	LOAD_SCENE,
	NEW_SCENE
};

class FileBrowserWindow : public Window
{

public:

	FileBrowserWindow(bool startOpened = false);
	~FileBrowserWindow();

	void Draw() override;
	void OpenBrowser(const BrowserState& state);

	void SaveScene(const char* path, const char* extension);
	void LoadScene(const char* path, const char* extension);
	void NewScene();
	void SaveSceneAs();

private:
	std::string name="";
	std::string current_path = "";
};
