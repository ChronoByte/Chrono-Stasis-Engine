#include "SceneViewWindow.h"
#include "csApp.h"
SceneViewWindow::SceneViewWindow(bool startOpened) : Window(startOpened)
{

}

SceneViewWindow::~SceneViewWindow()
{
}

void SceneViewWindow::Draw()
{
	ImGui::Begin("Scene", &active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImVec2 size = ImGui::GetWindowSize();
	ImGui::SetCursorPos({ -(App->window->width - size.x) / 2,-(App->window->height - size.y) / 2 });
	
	ImVec2 current_viewport_size = ImGui::GetContentRegionAvail();
	ImGui::Image((ImTextureID)App->renderer3D->textureBuffer, { (float)App->window->width, (float)App->window->height }, { 0,1 }, { 1,0 });
	//ImGui::Image((ImTextureID)App->renderer3D->textureBuffer, ImVec2(current_viewport_size.x, current_viewport_size.y), { 0,1 }, { 1,0 });
	
	int new_width, new_height;
	App->window->GetWindowSize(new_width, new_height);

	if (App->window->width != new_width || App->window->height != new_height)
	{
		App->window->width = new_width;
		App->window->height = new_height;
		App->renderer3D->OnResize(new_width, new_height);
	}



	if (ImGui::IsWindowHovered())
		App->camera->cameraControls = true;
	else 
		App->camera->cameraControls = false;
	

	ImGui::End();
	
}





