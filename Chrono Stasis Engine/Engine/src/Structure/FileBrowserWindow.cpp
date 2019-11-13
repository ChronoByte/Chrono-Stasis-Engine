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
	ImGui::Begin((std::string("Scene Browser:") + this->name).c_str(), &active, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);
	static char text[120];

	ImGui::BeginMenuBar();
	if (ImGui::ArrowButton("Back", ImGuiDir_Left))
	{
		if (current_path.compare(ASSETS_FOLDER) != 0) 
			current_path.pop_back(); //delete "/"
		
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
			//ImGui::Text((*unit)->name.c_str());
			ImGui::Selectable((*unit)->name.c_str());
			break;
		case StorageUnit::FILE:
			//ImGui::Text((*unit)->name.c_str());
			ImGui::Selectable((*unit)->name.c_str());
			break;
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
		{
			if ((*unit)->type == StorageUnit::FOLDER)
			{
				current_path += (*unit)->name + "/";
				ClearStorage();
				App->fs->GetStorageResources(current_path.c_str(), storage, extension.c_str());
				break;
			}
			if ((*unit)->type == StorageUnit::FILE)
			{

			}
		}

	}
	ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 75);
	ImGui::Separator();
	ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 65);
	ImGui::SetCursorPosX(20);
	ImGui::PushItemWidth(ImGui::GetWindowWidth() - 90);
	ImGui::Text("Name: ");
	ImGui::SameLine();
	if (ImGui::InputText("", text, 120, ImGuiInputTextFlags_AutoSelectAll)) 
	{
		scene = text;
		scene += SCENES_EXTENSION;
	}

	ImGui::PopItemWidth();
	ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 35);
	ImGui::SetCursorPosX(170);
	if (ImGui::Button(name.c_str(), { 100, 25 }))
	{
		if (name == "Save")
		{
			LOG("Scene &s Saved successfully", scene.c_str());
		}
	
		else if (name == "Load") 
		{
			LOG("Scene %s Loaded successfully", scene.c_str());
		}
	}


	ImGui::SameLine();

	if (ImGui::Button("Cancel", { 100, 25 }))
		this->SwitchActive();
	
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
	this->name = std::string("Save");
	this->current_path = path;
	this->extension = extension;

	ClearStorage();
	App->fs->GetStorageResources(path, storage, extension);
}

void FileBrowserWindow::LoadScene(const char* path, const char* extension)
{
	this->name = std::string("Load");
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
