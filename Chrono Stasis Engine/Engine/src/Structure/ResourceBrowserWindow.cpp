#include "csApp.h"
#include "ResourceBrowserWindow.h"
#include "csResources.h"

ResourceBrowserWindow::ResourceBrowserWindow(bool startOpened) : Window(startOpened)
{
}

ResourceBrowserWindow::~ResourceBrowserWindow()
{
}

void ResourceBrowserWindow::Draw()
{
	ImGui::Begin("Resource Explorer", &active, ImGuiWindowFlags_MenuBar);

	ImGui::BeginMenuBar();
	switch (currentType)
	{
	case Resource::R_MESH:
		ImGui::Text("Meshes");
		break;
	case Resource::R_TEXTURE:
		ImGui::Text("Textures");
		break;
	case Resource::R_SCENE:
		ImGui::Text("Scenes");
		break;
	}
	ImGui::EndMenuBar();

	if (ImGui::Selectable("No Texture"))
	{
		if (callbackComponent != nullptr)
		{
			callbackComponent->AssignResource(0);
		}
		active = false;
	}

	ImGui::Separator();

	for (int i = 0; i < resources.size(); i++)
	{
		if (ImGui::Selectable(resources[i]->GetName()))
		{
			if (callbackComponent != nullptr)
			{
				callbackComponent->AssignResource(resources[i]->GetUID());
			}
			active = false;
		}
	}
	if (ImGui::IsMouseClicked(0) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
		active = false;

	ImGui::End();
}

void ResourceBrowserWindow::OpenExplorer(Resource::Type type, float x, float y, Component* callback)
{
	currentType = type;
	resources = App->resources->GetResourcesFromType(type);
	callbackComponent = callback;
	ImGui::SetWindowPos("Hola", { (float)x, (float)y });
	active = true;
}
