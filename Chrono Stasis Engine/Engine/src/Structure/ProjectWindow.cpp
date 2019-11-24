#include "csApp.h"
#include "ProjectWindow.h"

ProjectWindow::ProjectWindow(bool startOpened) : Window(startOpened)
{
	
}

ProjectWindow::~ProjectWindow()
{
}

bool ProjectWindow::Start()
{
	current_path = ASSETS_FOLDER;
	extension = META_EXTENSION;
	//extension += ".json";
	ClearStorage();
	App->fs->GetStorageResources(current_path.c_str(), storage, nullptr, extension.c_str());
	return true;
}

void ProjectWindow::Draw()
{
	TestUnityProject();
}

void ProjectWindow::TestUnityProject()
{
	ImGui::Begin("Project", &active, ImGuiWindowFlags_MenuBar);

	ImGui::BeginMenuBar();
	if (ImGui::ArrowButton("Back", ImGuiDir_Left))
	{
		if (current_path.compare(ASSETS_FOLDER) != 0)
			current_path.pop_back(); //delete "/"

		current_path = current_path.substr(0, current_path.find_last_of("/") + 1);
		ClearStorage();
		App->fs->GetStorageResources(current_path.c_str(), storage,"all", extension.c_str());
	}

	ImGui::SameLine();
	ImGui::Text(current_path.c_str());

	//std::string folder;
	//App->fs->GetNameFile(current_path.c_str(), folder);
	ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 60);
	
	if (ImGui::BeginMenu("Create"))
	{
		if (ImGui::MenuItem("New Folder"))
		{ 
			std::string path = current_path;
			path += "Unnamed";
			App->fs->GenerateDirectory(path.c_str());

			StorageUnit* newFolder = new StorageUnit();
			newFolder->type = StorageUnit::FOLDER;
			newFolder->name = "Unnamed";
			newFolder->rename = true;
			storage.push_back(newFolder);

		}

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();

	//---------------------------------------------------------------------------------------

	static bool useChild = true;
	ImGui::Checkbox("use child", &useChild);

	static bool useChildBorder = true;
	ImGui::Checkbox("use child border", &useChildBorder);

	static float childHeight = 225.f;
	
	ImGui::Columns(2);
	ImGui::SetColumnOffset(1, 250);
	if (useChild) ImGui::BeginChild("col 1", ImVec2(0.0f, childHeight), useChildBorder);


	std::string root_tree = ASSETS_FOLDER;
	root_tree.pop_back();

	if (ImGui::TreeNodeEx(root_tree.c_str(), ImGuiTreeNodeFlags_OpenOnDoubleClick))
	{
		if (ImGui::IsItemClicked(0) && current_path != root_tree + "/")
		{
			current_path = root_tree + "/";
			ClearStorage();
			App->fs->GetStorageResources(current_path.c_str(), storage, "all", extension.c_str());

		}

		DrawAssetsTree(App->fs->GetAssetsFolder());
		ImGui::TreePop();
	}

	if (useChild) ImGui::EndChild();

	ImGui::NextColumn();
	if (useChild) ImGui::BeginChild("col 2", ImVec2(0.0f, childHeight), useChildBorder);

	ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f,1.0f,1.0f,0.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f,0.37f,0.0f,0.7f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.95f,0.5f,0.0f,0.7f });


	int counter = 1;
	int i = 0;
	int text_counter = 1;
	float YLine = ImGui::GetCursorPosY();
	float offsetH = 70;
	for (auto unit = storage.begin(); unit != storage.end(); unit++)
	{

		switch ((*unit)->type)
		{
		case StorageUnit::FOLDER:
			
			if ((*unit)->rename)
			{
				ImGui::PushStyleColor(ImGuiCol_FrameBg, { 1.0f,1.0f,1.0f,0.9f });
				ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f,0.0f,0.0f,1.0f });
				ImGui::PushItemWidth(60);
				char text[120];
				strcpy_s(text, 120, (*unit)->name.c_str());
				ImGui::PushID(-i);
				ImGui::SetCursorPos({ 5 + ImGui::GetCursorPosX() + (64 + 30) * (counter - 1), 70 });
				if (ImGui::InputText("", text, 120, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue) || (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)))
				{
					if ((*unit)->name != text) {
						std::string path = current_path;
						path += (*unit)->name;

						if ((*unit)->name.size() == 0)
							(*unit)->name = "Unnamed";

						(*unit)->name = text;

						App->fs->GenerateDirectory((current_path + text).c_str());
						App->fs->DeleteDirectory(path.c_str());
					}
					(*unit)->rename = false;
				}
				if (!ImGui::IsItemFocused())
					ImGui::SetKeyboardFocusHere(0);
				ImGui::PopID();
				ImGui::PopItemWidth();
				ImGui::PopStyleColor(2);
			}
			else {
				ImGui::SetCursorPos({ 5 + ImGui::GetCursorPosX() + (64 + 30) * (counter - 1), 70 });
				ImGui::Text((*unit)->name.c_str());
			}

			ImGui::SetCursorPos({ ImGui::GetCursorPosX() + (64 + 30) * (counter - 1), 0 });
			ImGui::PushID(i);
			ImGui::ImageButton((ImTextureID)(App->editor->iconFolder->id), { 64, 64 });
			ImGui::PopID();
	

			break;
		case StorageUnit::FILE:
			ImGui::SetCursorPos({ 5 + ImGui::GetCursorPosX() + (64 + 30) * (counter - 1), 70 });
			ImGui::Text((*unit)->name.c_str());
			//ImGui::Selectable((*unit)->name.c_str());
			ImGui::SetCursorPos({ ImGui::GetCursorPosX() + (64 + 30) * (counter - 1), 0 });
			ImGui::ImageButton((ImTextureID)(App->editor->iconFile->id), { 64, 64 });
			
			break;
		}


		
	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
	{
		if ((*unit)->type == StorageUnit::FOLDER)
		{
			current_path += (*unit)->name + "/";
			ClearStorage();
			App->fs->GetStorageResources(current_path.c_str(), storage, "all", extension.c_str());

			break;
		}
		if ((*unit)->type == StorageUnit::FILE)
		{
		  std::string extension;
		  App->fs->GetExtensionFile((*unit)->name.c_str(), extension );

		  if(!extension.compare(".scene"))
			  ImGui::OpenPopup("WARNING!");

		  if (!extension.compare(".fbx") || !extension.compare(".FBX"))
			  App->serialization->LoadModel((current_path + (*unit)->name + META_EXTENSION).c_str());
		}
	}
	if (ImGui::BeginPopupContextItem(((*unit)->name + "rightClick").c_str(), 1))
	{
		if (ImGui::Button("Delete"))
		{
			std::string path = current_path;
			path += (*unit)->name;
			App->fs->DeleteDirectory(path.c_str());

			StorageUnit* deleteUnit = *unit;
			unit--; 
			storage.remove(deleteUnit);
			RELEASE(deleteUnit);
		}
		if (ImGui::Button("Rename"))
		{
			(*unit)->rename = true;
		}

		ImGui::EndPopup();
	}



		counter++;
		i++;
	}
	ImGui::PopStyleColor(3);

	ImGui::SetNextWindowSize(ImVec2(800.0f, 130.0f));
	if (ImGui::BeginPopupModal("WARNING!", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		ImGui::Separator();
		ImGui::Separator();
		ImVec2 new_txt_pos(ImGui::GetWindowPos().x + 330.0f, ImGui::GetCursorScreenPos().y);
		ImGui::SetCursorScreenPos(new_txt_pos);
		ImGui::Text("ALTO AHI PAJILLERO!");
		ImVec2 new_txt_pos2(ImGui::GetWindowPos().x + 175.0f, ImGui::GetCursorScreenPos().y);
		ImGui::SetCursorScreenPos(new_txt_pos2);
		ImGui::Text("Estas accediendo a zonas no programadas todavia, ten paciencia!");
		//ImGui::NewLine();
		ImGui::Separator();
		ImGui::Separator();
		ImGui::Spacing();
		ImVec2 new_btn_pos(ImGui::GetWindowPos().x + (800.0f / 2.0f - 350.0f / 2.0f), ImGui::GetCursorScreenPos().y);
		ImGui::SetCursorScreenPos(new_btn_pos);
		if (ImGui::Button("TU PUTA MADRE", ImVec2(120, 40)))
		{
			//LOAD SCENE
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(450);
		if (ImGui::Button("CANCEL", ImVec2(120, 40)))
		{
			ImGui::CloseCurrentPopup();
		}
		
		ImGui::EndPopup();
	}
	if (useChild) ImGui::EndChild();
	ImGui::End();
}

void ProjectWindow::TestChronoStasisProject()
{
	ImGui::Begin("Project", &active);

	ImGui::Text("LOLAS QUE HACES MIRANDO NUESTRO CODIGO DEL ENGINE?");
	ImGui::Text("PILLLIIIIIIIIIIIIIIIIIIIIIIN");
	ImGui::End();
}

void ProjectWindow::DrawAssetsTree(Folder* root)
{
	for (uint i = 0; i < root->folders.size(); i++)
	{
		if (ImGui::TreeNodeEx(root->folders[i].name.c_str(), ImGuiTreeNodeFlags_OpenOnDoubleClick))
		{
			/*std::string folder_path = current_path;
			folder_path.pop_back();
			App->fs->GetNameFile(folder_path.c_str(), folder_path);
			folder_path.append("/");*/

			if (ImGui::IsItemClicked(0) &&  current_path != root->folders[i].path + "/")
			{
				current_path = root->folders[i].path  + "/";
				ClearStorage();
				App->fs->GetStorageResources(current_path.c_str(), storage, "all", extension.c_str());
				
			}

			DrawAssetsTree(&root->folders[i]);
			ImGui::TreePop();
		}
	}

	for (uint i = 0; i < root->files.size(); i++)
	{
		std::string extension;
		App->fs->GetExtensionFile(root->files[i].name.c_str(), extension);

		if (extension.compare(".meta") != 0)
		{
			if (ImGui::TreeNodeEx(root->files[i].name.c_str(), ImGuiTreeNodeFlags_Leaf))
			{
				
				ImGui::TreePop();
			}
		}
	}

}

void ProjectWindow::ClearStorage()
{
	std::list<StorageUnit*>::iterator unit = storage.begin();
	while (unit != storage.end())
	{
		RELEASE(*unit);
		unit++;
	}
	storage.clear();
}


