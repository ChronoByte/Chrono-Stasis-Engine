#include "GameViewWindow.h"
#include "csApp.h"
GameViewWindow::GameViewWindow(bool startOpened) : Window(startOpened)
{

}

GameViewWindow::~GameViewWindow()
{
}

void GameViewWindow::Draw()
{
	ImGui::Begin("Game", &active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImVec2 size = ImGui::GetWindowSize();
	ImGui::SetCursorPos({ -(App->window->width - size.x) / 2,-(App->window->height - size.y) / 2 });
	
	ImVec2 current_viewport_size = ImGui::GetContentRegionAvail();
	ImGui::Image((ImTextureID)App->renderer3D->gameTexture, { (float)App->window->width, (float)App->window->height }, { 0,1 }, { 1,0 });
	
	int new_width, new_height;
	App->window->GetWindowSize(new_width, new_height);

	if (App->window->width != new_width || App->window->height != new_height)
	{
		App->window->width = new_width;
		App->window->height = new_height;
		App->renderer3D->OnResize(new_width, new_height);
	}	

	ImGui::End();
	
}





