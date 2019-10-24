#include "InspectorWindow.h"
#include "csApp.h"
#include "HierarchyWindow.h"
#include "csGameObject.h"

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
		GameObject* go_selected = App->editor->hierarchy->GetSelected();
		//GameObject* go_selected = App->scene->CreateGameObject(nullptr, "");

		if (go_selected != nullptr) {

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Add Component"))
				{
					if (ImGui::MenuItem("Add Material")) {}

					if (ImGui::MenuItem("Add Geometry")) {}

					if (ImGui::MenuItem("Add Camera")) {}


					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			go_selected->DrawInspectorComponents();  
		   // ShowInspector();
		}
		
	}
	ImGui::End();

}

void InspectorWindow::ShowInspector()
{
	//JUST HOW TO NOT CODE THIS
	//TODO1 : obtain a gameobject or array of them and loop each one to generate collapsingheaders
	//TODO2 : create a itemMenu to create on inspector components using GameObject methods
	//TODO3 : loop a component draw method to put info
	
		ImGui::Text("Options");
		bool enabled = true;

		ImGui::Checkbox("", &enabled);
		ImGui::SameLine();
		char name[25];
		sprintf_s(name, 25, "Cube");
		ImGui::InputText("", name, 25, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		//Set gameobject name

	
		if (ImGui::CollapsingHeader("Local Transformation" , ImGuiTreeNodeFlags_DefaultOpen))
		{
			
			float pos[3] = { 0.0f, 0.0f, 0.0f };
			ImGui::InputFloat3("Position", pos, 2);
			
			float rot[3] = { 0.0f, 0.0f, 0.0f };
			ImGui::InputFloat3("Rotation", rot, 2);
			
			float scale[3] = { 1.0f,1.0f, 1.0f };
			ImGui::InputFloat3("Scale", scale, 2);
		}
		if (ImGui::CollapsingHeader("Geometry", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//TODO: loop all data of each mesh incrment the value to know the total of each stuff
			ImGui::Text("Total Vertices:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", 1);
			ImGui::Text("Total Meshes:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", 1);
			ImGui::Text("Total Faces:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", 1);
			ImGui::Text("Total UV:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", 1);
		}
		if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Texture:");
			ImGui::Image((ImTextureID*)2, ImVec2(150, 150));

			float3 color = float3(20.0f, 0.0f, 40.0f);

			if (ImGui::TreeNodeEx("Color", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::ColorEdit3("", (float*)&color);
				ImGui::TreePop();
			}
			
		}
	
}
