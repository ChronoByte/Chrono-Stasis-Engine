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
			ImGui::Selectable((*unit)->name.c_str(), false, 0, { ImGui::GetWindowWidth(),24 });

			break;
		case StorageUnit::FILE:
			
			ImGui::Selectable((*unit)->name.c_str());
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
		}
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


