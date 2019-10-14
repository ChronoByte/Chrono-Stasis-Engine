#include "HierarchyWindow.h"
#include "csApp.h"

#include "csGameObject.h"

HierarchyWindow::HierarchyWindow(bool startOpened) : Window(startOpened)
{
}

HierarchyWindow::~HierarchyWindow()
{
}

void HierarchyWindow::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300.0f, App->window->GetHeight()), ImGuiCond_Once);

	GameObject* root = App->scene->GetRoot(); 

	if (ImGui::Begin("Hierarchy", &active, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse))
	{
		CreateRecursiveTreeNodes(root); 
	}
	ImGui::End();

	if(selectedGo != nullptr)
		LOG("Selected game object: %s", selectedGo->GetName());
}

void HierarchyWindow::CreateRecursiveTreeNodes(GameObject * parent)
{
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	ImGuiTreeNodeFlags node_flags = base_flags;

	if(parent == selectedGo)
		node_flags |= ImGuiTreeNodeFlags_Selected;

	if (ImGui::TreeNodeEx(parent, node_flags, parent->GetName()))
	{
		if (ImGui::IsItemClicked())	// TODO: Handle the situation where a GameObject is deleted and selected at the same time
			selectedGo = parent;

		std::list<GameObject*>::const_iterator it = parent->childs.begin();

		for (it; it != parent->childs.end(); ++it)
		{
			CreateRecursiveTreeNodes((*it)); 
		}
		ImGui::TreePop();
	}
}

GameObject* HierarchyWindow::GetSelected() const
{
	return selectedGo;
}


