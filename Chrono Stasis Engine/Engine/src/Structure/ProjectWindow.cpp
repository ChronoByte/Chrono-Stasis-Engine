#include "csApp.h"
#include "ProjectWindow.h"

ProjectWindow::ProjectWindow(bool startOpened) : Window(startOpened)
{
}

ProjectWindow::~ProjectWindow()
{
}

void ProjectWindow::Draw()
{
	TestUnityProject();
}

void ProjectWindow::TestUnityProject()
{
	ImGui::Begin("Project", &active);
	
	static bool useChild = true;
	ImGui::Checkbox("use child", &useChild);

	static bool useChildBorder = true;
	ImGui::Checkbox("use child border", &useChildBorder);

	static float childHeight = 225.f;
	//ImGui::SliderFloat("child height", &childHeight, 0.0f, 500.0f);

	ImGui::Columns(2);
	ImGui::SetColumnOffset(1, 250);
	if (useChild) ImGui::BeginChild("col 1", ImVec2(0.0f, childHeight), useChildBorder);

	std::string root_tree = ASSETS_FOLDER;
	root_tree.pop_back();

	if (ImGui::TreeNodeEx(root_tree.c_str()))
	{
		DrawAssetsTree(App->fs->GetAssetsFolder());
		ImGui::TreePop();
	}

	if (useChild) ImGui::EndChild();

	ImGui::NextColumn();

	if (useChild) ImGui::BeginChild("col 2", ImVec2(0.0f, childHeight), useChildBorder);

	ImGui::Text("LOLAS QUE HACES MIRANDO NUESTRO CODIGO DEL ENGINE?");
	ImGui::Text("PILLLIIIIIIIIIIIIIIIIIIIIIIN");

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
		if (ImGui::TreeNodeEx(root->folders[i].name.c_str()))
		{
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


