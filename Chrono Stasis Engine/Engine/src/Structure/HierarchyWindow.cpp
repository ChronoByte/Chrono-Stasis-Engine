#include "HierarchyWindow.h"
#include "csApp.h"
#include "csEditor.h"
#include "csGameObject.h"

HierarchyWindow::HierarchyWindow(bool startOpened) : Window(startOpened)
{
}

HierarchyWindow::~HierarchyWindow()
{
}

void HierarchyWindow::Draw()
{
	//ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Once);
	//ImGui::SetNextWindowSize(ImVec2(300.0f, App->window->GetHeight()), ImGuiCond_Once);

	GameObject* root = App->scene->GetRoot(); 

	if (ImGui::Begin("Hierarchy", &active,  ImGuiWindowFlags_MenuBar))
	{
		ImGui::BeginMenuBar(); 
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("Create Empty", "")) { App->scene->CreateGameObject(nullptr, "GameObject"); }
			if (ImGui::BeginMenu("3D Object"))
			{
				if (ImGui::MenuItem("Cube")) { App->scene->CreateObject3D(PrimitiveType::CUBE, nullptr); }
				if (ImGui::MenuItem("Sphere")) { App->scene->CreateObject3D(PrimitiveType::SPHERE, nullptr); }
				if (ImGui::MenuItem("Plane")) { App->scene->CreateObject3D(PrimitiveType::PLANE, nullptr); }
				if (ImGui::MenuItem("Cylinder")) { App->scene->CreateObject3D(PrimitiveType::CYLINDER, nullptr); }
				if (ImGui::MenuItem("Cone")) { App->scene->CreateObject3D(PrimitiveType::CONE, nullptr); }
				if (ImGui::MenuItem("Torus")) { App->scene->CreateObject3D(PrimitiveType::TORUS, nullptr); }
				if (ImGui::MenuItem("Klein Bottle")) { App->scene->CreateObject3D(PrimitiveType::KLEIN_BOTTLE, nullptr); }
				if (ImGui::MenuItem("Trefoil Knot")) { App->scene->CreateObject3D(PrimitiveType::TREFOIL_KNOT, nullptr); }
				if (ImGui::MenuItem("Hemisphere")) { App->scene->CreateObject3D(PrimitiveType::HEMISPHERE, nullptr); }
				if (ImGui::MenuItem("Rock")) { App->scene->CreateObject3D(PrimitiveType::ROCK, nullptr); }
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}		
		ImGui::EndMenuBar();

		CreateRecursiveTreeNodes(root); 
	}
	ImGui::End();		
}

void HierarchyWindow::CreateRecursiveTreeNodes(GameObject * parent)
{
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	ImGuiTreeNodeFlags node_flags = base_flags;

	if(parent == selectedGo)
		node_flags |= ImGuiTreeNodeFlags_Selected;

	if (ImGui::TreeNodeEx(parent, node_flags, parent->GetName()))
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete"))
			{
				// TODO deleting
				ImGui::CloseCurrentPopup();
			}

			ImGui::Separator(); 

			if (ImGui::MenuItem("Create Child"))
			{
				App->scene->CreateGameObject(parent, "Game Object");
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::BeginMenu("3D Object"))
			{
				if (ImGui::MenuItem("Cube")) { App->scene->CreateObject3D(PrimitiveType::CUBE, nullptr); ImGui::CloseCurrentPopup(); }
				if (ImGui::MenuItem("Sphere")) { App->scene->CreateObject3D(PrimitiveType::SPHERE, nullptr); ImGui::CloseCurrentPopup(); }
				if (ImGui::MenuItem("Plane")) { App->scene->CreateObject3D(PrimitiveType::PLANE, nullptr); ImGui::CloseCurrentPopup(); }
				if (ImGui::MenuItem("Cylinder")) { App->scene->CreateObject3D(PrimitiveType::CYLINDER, nullptr); ImGui::CloseCurrentPopup(); }
				if (ImGui::MenuItem("Cone")) { App->scene->CreateObject3D(PrimitiveType::CONE, nullptr); ImGui::CloseCurrentPopup(); }
				if (ImGui::MenuItem("Torus")) { App->scene->CreateObject3D(PrimitiveType::TORUS, nullptr); ImGui::CloseCurrentPopup(); }
				if (ImGui::MenuItem("Klein Bottle")) { App->scene->CreateObject3D(PrimitiveType::KLEIN_BOTTLE, nullptr); ImGui::CloseCurrentPopup();}
				if (ImGui::MenuItem("Trefoil Knot")) { App->scene->CreateObject3D(PrimitiveType::TREFOIL_KNOT, nullptr); ImGui::CloseCurrentPopup(); }
				if (ImGui::MenuItem("Hemisphere")) { App->scene->CreateObject3D(PrimitiveType::HEMISPHERE, nullptr); ImGui::CloseCurrentPopup(); }
				if (ImGui::MenuItem("Rock")) { App->scene->CreateObject3D(PrimitiveType::ROCK, nullptr); ImGui::CloseCurrentPopup(); }
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}

		if (ImGui::IsItemClicked())
		{
			selectedGo = parent;		// TODO: Handle the situation where a GameObject is deleted and selected at the same time
			LOG("Selected game object: %s", selectedGo->GetName());
		}

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


