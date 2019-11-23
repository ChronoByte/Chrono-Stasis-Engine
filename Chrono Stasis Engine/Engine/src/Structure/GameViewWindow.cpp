#include "GameViewWindow.h"
#include "csApp.h"
#include "csViewport.h"
#include "ComponentCamera.h"

GameViewWindow::GameViewWindow(bool startOpened) : Window(startOpened)
{

}

GameViewWindow::~GameViewWindow()
{
}

void GameViewWindow::Draw()
{
	ImGui::Begin("Game", &active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	//ImGui::SetCursorPos({ -(App->window->width - size.x) / 2,-(App->window->height - size.y) / 2 });

	ImVec2 size = ImGui::GetWindowSize();

	width = size.x;
	height = size.y;
	
	if (regionX != ImGui::GetContentRegionAvail().x || regionY != ImGui::GetContentRegionAvail().y)
	{
		regionX = ImGui::GetContentRegionAvail().x;
		regionY = ImGui::GetContentRegionAvail().y;

		App->renderer3D->gameViewport->SetSize(width, height);
	}

	if (App->scene->GetMainCamera() != nullptr)
	{
		if(App->renderer3D->displayZBuffer || App->scene->GetMainCamera()->showZBuffer)
			ImGui::Image((ImTextureID)App->renderer3D->gameViewport->zBufferTexture, { width, height }, { 0,1 }, { 1,0 });
		
		else 
			ImGui::Image((ImTextureID)App->renderer3D->gameViewport->renderTexture, { width, height }, { 0,1 }, { 1,0 });
	}

	ImGui::End();

}





