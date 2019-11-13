#include "csApp.h"
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
		current_path = current_path.substr(0, current_path.find_last_of("/")+1);
		ClearStorage();
		App->fs->GetStorageResources(current_path.c_str(),storage, extension.c_str());
	}
	ImGui::SameLine();
	ImGui::Text(current_path.c_str());
	ImGui::EndMenuBar();

	for (auto unit = storage.begin(); unit != storage.end(); unit++) 
	{
		switch ((*unit)->type)
		{
		case StorageUnit::FOLDER:
			ImGui::Text((*unit)->name.c_str());
			break;
		case StorageUnit::FILE:
			ImGui::Text((*unit)->name.c_str());
			break;
		}

	}
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
	this->extension = extension;

	ClearStorage();
	App->fs->GetStorageResources(path, storage, extension);
}

void FileBrowserWindow::LoadScene(const char* path, const char* extension)
{
	this->name = std::string("Scene Browser: Load");
	this->current_path = path;
	this->extension = extension;

	ClearStorage();
	App->fs->GetStorageResources(path, storage, extension);
}

void FileBrowserWindow::NewScene()
{
}

void FileBrowserWindow::SaveSceneAs()
{
}

void FileBrowserWindow::ClearStorage()
{
	std::list<StorageUnit*>::iterator unit = storage.begin();
	while (unit != storage.end())
	{
		RELEASE(*unit);
		unit++;
	}
	storage.clear();

	//auto unit2 = storage.begin();

	
}
