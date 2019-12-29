#pragma once

#include "csGlobals.h"
#include "imgui/imgui.h"
#include "Window.h"
#include <string>
#include <list>
enum BrowserState
{
	NONE_SCENE = 0,

	SAVE_SCENE,
	SAVE_SCENE_AS,
	LOAD_SCENE,
	NEW_SCENE,
	SAVE_PARTICLE_SYSTEM,
	LOAD_PARTICLE_SYSTEM
};

enum ExtensionType
{
	SCENE_EXTENSION,
	PARTICLE_EXTENSION,
	NONE_EXTENSION,
};
struct StorageUnit
{
	enum storageType
	{
		NONE = 0,
		FOLDER,
		FILE
	};
	std::string name = "";
	storageType type;
	bool selected = false;
	bool rename = false;
};

class FileBrowserWindow : public Window
{

public:

	FileBrowserWindow(bool startOpened = false);
	~FileBrowserWindow();

	void Draw() override;
	void OpenBrowser(const BrowserState& state);

	void SaveScene(const char* path, const char* extension, ExtensionType type);
	void LoadScene(const char* path, const char* extension, ExtensionType type);
	void NewScene();
	void SaveSceneAs();

	void ClearStorage();

private:
	std::string name="";
	std::string current_path = "";
	std::string extension = "";
	ExtensionType type = ExtensionType::NONE_EXTENSION;
	std::string scene = "";
	std::string inputText;
	bool serialization = false;
	std::list<StorageUnit*> storage;
};
