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
	if(activate)
		ShowInspector();
}

void InspectorWindow::ShowInspector()
{
	static int width;
	static int height;

	
	if (!ImGui::BeginDock("INSPECTOR", &activate, ImGuiWindowFlags_HorizontalScrollbar))
	{
		ImGui::EndDock();
		return;
	}

	ImGui::Text("MODEL INFO");
	if (ImGui::CollapsingHeader("Transformation"))
	{
		ImGui::Text("Position:");
		ImGui::Text("Rotation:");
		ImGui::Text("Scale");
	}

	ImGui::EndDock();
}
