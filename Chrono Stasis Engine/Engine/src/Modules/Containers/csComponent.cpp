#include "csApp.h"
#include "csComponent.h"

void Component::DroppableSpace(const char* string, bool empty, float x, float y)
{
	float button_alpha = 0.7f;
	if (!empty)
		button_alpha = 0.4f;
	ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f,1.0f,1.0f,button_alpha });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 1.0f,1.0f,1.0f,button_alpha });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1.0f,1.0f,1.0f,button_alpha });
	if (empty)
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.4f);

	ImGui::Button(string, { x, y });

	if (empty)
		ImGui::PopStyleVar();
	ImGui::PopStyleColor(3);

}

void Component::ResourceExplorerButton(Resource::Type type)
{
	ImGui::PushID("pick" + type);
	if (ImGui::RadioButton("", false))
	{
		ImVec2 pos = ImGui::GetWindowPos();
		if (pos.x < App->window->width / 2)
			pos.x += ImGui::GetWindowWidth();
		else
			pos.x -= 190;
		App->editor->OpenResourceExplorer(type, pos.x, pos.y, this);
	}
	ImGui::PopID();

}

void Component::AssignResource(UID uid)
{
	if (currentResource != nullptr)
		currentResource->UnloadFromMemory();

	currentResource = App->resources->GetResource(uid);

	if (currentResource)
		currentResource->LoadToMemory();
	
	
}
