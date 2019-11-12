#include "FileBrowserWindow.h"

FileBrowserWindow::FileBrowserWindow(bool startOpened) : Window(startOpened)
{
}

FileBrowserWindow::~FileBrowserWindow()
{
}

void FileBrowserWindow::Draw()
{
	ImGui::SetNextWindowSize(ImVec2(400,500));
	ImGui::Begin(this->name.c_str(), &active, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);

	ImGui::BeginMenuBar();
	if (ImGui::ArrowButton("Back", ImGuiDir_Left))
	{
	}
	ImGui::EndMenuBar();
	ImGui::End();
}

void FileBrowserWindow::OpenBrowser(const BrowserState& state)
{

	switch (state) {
	case BrowserState::SAVE_SCENE:
		this->SwitchActive();
		SaveScene(ASSETS_FOLDER, SCENES_EXTENSION);
		break;
	case BrowserState::SAVE_SCENE_AS:
		SaveSceneAs(); //TODO
		break;
	case BrowserState::LOAD_SCENE:
		this->SwitchActive();
		LoadScene(ASSETS_FOLDER, SCENES_EXTENSION);
		break;
	case BrowserState::NEW_SCENE:
		NewScene(); //TODO
		break;
	}
}

void FileBrowserWindow::SaveScene(const char* path, const char* extension)
{
	this->name = std::string("Scene Browser: Save");
	this->current_path = path;
}

void FileBrowserWindow::LoadScene(const char* path, const char* extension)
{
	this->name = std::string("Scene Browser: Load");
	this->current_path = path;
}

void FileBrowserWindow::NewScene()
{
}

void FileBrowserWindow::SaveSceneAs()
{
}
