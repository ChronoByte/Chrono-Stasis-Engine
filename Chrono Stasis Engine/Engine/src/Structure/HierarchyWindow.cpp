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

	const GameObject* root = App->scene->GetRoot(); 

	if (ImGui::Begin("Hierarchy", &active, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse))
	{
		CreateRecursiveTreeNodes(root); 
	}
	ImGui::End();

}

void HierarchyWindow::CreateRecursiveTreeNodes(const GameObject * parent)
{
	
	if (ImGui::TreeNode(parent, "%s", parent->GetName()))
	{
		std::list<GameObject*>::const_iterator it = parent->childs.begin();

		for (it; it != parent->childs.end(); ++it)
		{
			CreateRecursiveTreeNodes((*it)); 
		}
		ImGui::TreePop();
	}
}
