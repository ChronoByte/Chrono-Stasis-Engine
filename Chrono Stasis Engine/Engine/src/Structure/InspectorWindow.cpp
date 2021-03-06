#include "InspectorWindow.h"
#include "csApp.h"
#include "HierarchyWindow.h"
#include "csGameObject.h"
#include "csComponent.h"


InspectorWindow::InspectorWindow(bool startOpened) : Window(startOpened)
{
}

InspectorWindow::~InspectorWindow()
{
}

void InspectorWindow::Draw()
{
	//ImGui::SetNextWindowPos(ImVec2(App->window->width - 300.0f, 20.0f), ImGuiCond_Once);
	//ImGui::SetNextWindowSize(ImVec2(300.0f, 700.0f), ImGuiCond_Once);

	if (ImGui::Begin("Inspector", &active, ImGuiWindowFlags_MenuBar))
	{
		GameObject* go_selected = App->scene->GetSelected();
		//GameObject* go_selected = App->scene->CreateGameObject(nullptr, "");

		if (go_selected != nullptr) {

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Add Component"))
				{
					if (ImGui::MenuItem("Add Material")) { go_selected->CreateComponent(ComponentType::C_MATERIAL); }

					if (ImGui::MenuItem("Add Mesh")) { go_selected->CreateComponent(ComponentType::C_MESH); }

					if (ImGui::MenuItem("Add Camera")) { go_selected->CreateComponent(ComponentType::C_CAMERA); }

					if (ImGui::MenuItem("Add Billboard")) { go_selected->CreateComponent(ComponentType::C_BILLBOARD); }

					if (ImGui::MenuItem("Add Particle System")) { go_selected->CreateComponent(ComponentType::C_PARTICLE_SYSTEM); }

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			go_selected->DrawInspectorComponents();  
		   
		}
		
	}
	ImGui::End();

}

