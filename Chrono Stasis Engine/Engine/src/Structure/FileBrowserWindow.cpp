#include "csApp.h"
#include "FileBrowserWindow.h"
#include "csSceneSerializer.h"
#include "csFileSystem.h"
#include "csScene.h"
#include "csEditor.h"
#include "csTextureLoader.h"

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
	static std::string tmp_text;
	ImGui::BeginMenuBar();
	if (ImGui::ArrowButton("Back", ImGuiDir_Left))
	{
		if (current_path.compare(ASSETS_FOLDER) != 0) 
			current_path.pop_back(); //delete "/"
		
		current_path = current_path.substr(0, current_path.find_last_of("/")+1);
		ClearStorage();
		App->fs->GetStorageResources(current_path.c_str(),storage, extension.c_str(), META_EXTENSION);
	}
	ImGui::SameLine();
	ImGui::Text(current_path.c_str());
	ImGui::EndMenuBar();

	for (auto unit = storage.begin(); unit != storage.end(); unit++) 
	{
		switch ((*unit)->type)
		{
		case StorageUnit::FOLDER:
			
			ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f,1.0f,1.0f,0.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f,0.37f,0.0f,0.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.95f,0.5f,0.0f,0.0f });
			ImGui::ImageButton((ImTextureID)(App->editor->iconFolder->id), { 24, 24 });
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::SetCursorPos({ ImGui::GetCursorPosX() - 2, ImGui::GetCursorPosY() + 8 });
			ImGui::Selectable((*unit)->name.c_str(), false, 0, {ImGui::GetWindowWidth(),24});
			break;
		case StorageUnit::FILE:
			ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f,1.0f,1.0f,0.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f,0.37f,0.0f,0.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.95f,0.5f,0.0f,0.0f });
			ImGui::ImageButton((ImTextureID)(App->editor->iconFile->id), { 24, 24 });
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::SetCursorPos({ ImGui::GetCursorPosX() - 2, ImGui::GetCursorPosY() + 8 });
			
			ImGui::Selectable((*unit)->name.c_str(), false, 0, { ImGui::GetWindowWidth(),24 });
			
			break;
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
		{
			if ((*unit)->type == StorageUnit::FOLDER)
			{
				current_path += (*unit)->name + "/";
				ClearStorage();
				App->fs->GetStorageResources(current_path.c_str(), storage, extension.c_str(), META_EXTENSION);
				break;
			}
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
		{
			if ((*unit)->type == StorageUnit::FILE)
			{
				App->serialization->current_scene = (*unit)->name; // TO LOAD
				inputText = App->serialization->current_scene.c_str();
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
	if (ImGui::InputText("", (char*)inputText.c_str(), 120, ImGuiInputTextFlags_AutoSelectAll)) 
	{
		App->serialization->current_scene = inputText.c_str(); // TO SAVE AS NEW
		//scene += SCENES_EXTENSION;
	}

	ImGui::PopItemWidth();
	ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 35);
	ImGui::SetCursorPosX(170);
	if (ImGui::Button(name.c_str(), { 100, 25 }))
	{
		serialization = true;
	}


	ImGui::SameLine();

	if (ImGui::Button("Cancel", { 100, 25 }))
		this->SwitchActive();
	
	if (serialization)
	{

		if (name == "Save")
		{
			App->serialization->scene_to_serialize = current_path + App->serialization->current_scene;
			App->serialization->SaveScene(App->serialization->scene_to_serialize.c_str());
			LOG("Scene &s Saved successfully", App->serialization->current_scene.c_str());
		}
	
		else if (name == "Load") 
		{
			App->scene->ClearScene();
			App->serialization->scene_to_serialize = current_path + App->serialization->current_scene;
			App->serialization->LoadScene(App->serialization->scene_to_serialize.c_str());
			LOG("Scene %s Loaded successfully", App->serialization->current_scene.c_str());
		}

		serialization = false;
		this->SwitchActive();

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
	this->name = std::string("Save");
	this->current_path = path;
	this->extension = extension;
	this->inputText = "";
	ClearStorage();
	App->fs->GetStorageResources(path, storage, extension, META_EXTENSION);
}

void FileBrowserWindow::LoadScene(const char* path, const char* extension)
{
	this->name = std::string("Load");
	this->current_path = path;
	this->extension = extension;
	this->inputText = "";
	ClearStorage();
	App->fs->GetStorageResources(path, storage, extension, META_EXTENSION);
}

void FileBrowserWindow::NewScene()
{
	App->scene->ClearScene();
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
