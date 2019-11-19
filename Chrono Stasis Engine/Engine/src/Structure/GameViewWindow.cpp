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

	ImVec2 size = ImGui::GetWindowSize();
	ImGui::SetCursorPos({ -(App->window->width - size.x) / 2,-(App->window->height - size.y) / 2 });
	
	if (App->scene->GetMainCamera() != nullptr)
	{
		if(App->renderer3D->displayZBuffer || App->scene->GetMainCamera()->showZBuffer)
			ImGui::Image((ImTextureID)App->renderer3D->gameViewport->zBufferTexture, { (float)App->renderer3D->gameViewport->width, (float)App->renderer3D->gameViewport->height }, { 0,1 }, { 1,0 });
		
		else 
			ImGui::Image((ImTextureID)App->renderer3D->gameViewport->renderTexture, { (float)App->renderer3D->gameViewport->width, (float)App->renderer3D->gameViewport->height }, { 0,1 }, { 1,0 });
	}

	ImGui::End();

}





