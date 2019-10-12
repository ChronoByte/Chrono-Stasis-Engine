#include "InspectorWindow.h"
#include "csApp.h"


InspectorWindow::InspectorWindow(bool startOpened)
{
}

InspectorWindow::~InspectorWindow()
{
}

void InspectorWindow::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(App->window->width - 300.0f, 20.0f), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300.0f, 700.0f), ImGuiCond_Once);
	
	if (ImGui::Begin("Panel"))
	{
		ImGui::BeginDockspace();
		ShowInspector();
		ImGui::EndDockspace();
	}
	ImGui::End();
	
		
}

void InspectorWindow::ShowInspector()
{
	static int width;
	static int height;

	
	if (!ImGui::BeginDock("INSPECTOR", &active, ImGuiWindowFlags_HorizontalScrollbar))
	{
		ImGui::EndDock();
		return;
	}

	ImGui::Text("MODEL INFO");
	if (ImGui::CollapsingHeader("Local Transformation"))
	{
		ImGui::Text("Position:");
		ImGui::Text("Rotation:");
		ImGui::Text("Scale");
	}
	if (ImGui::CollapsingHeader("Geometry"))
	{
		
	}
	if (ImGui::CollapsingHeader("Texture"))
	{
		
	}

	ImGui::EndDock();
}
