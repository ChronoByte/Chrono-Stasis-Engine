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
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		zBuffer = !zBuffer;

	ImGui::Begin("Scene", &active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImVec2 size = ImGui::GetWindowSize();
	width = size.x;
	height = size.y;
	ImGui::SetCursorPos({ -(App->window->width - size.x) / 2,-(App->window->height - size.y) / 2 });

	//LOG("Scene view width: %f height: %f", width, height);

	mouseX = App->input->GetMouseX();;
	mouseY = App->input->GetMouseY();
	//LOG("Mouse X: %f Mouse Y: %f", mouseX, mouseY);
	
	ImVec2 winPos = ImGui::GetWindowPos();
	mouseX = mouseX - winPos.x; 
	mouseY = mouseY - winPos.y;

	//LOG("Corrected Mouse X: %f Mouse Y: %f", mouseX, mouseY);

	ImVec2 current_viewport_size = ImGui::GetContentRegionAvail();

	if(zBuffer)
		ImGui::Image((ImTextureID)App->renderer3D->zBufferTexture, { (float)App->window->width, (float)App->window->height }, { 0,1 }, { 1,0 });
	else
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

float SceneViewWindow::GetWindowWidth() const
{
	return width;
}

float SceneViewWindow::GetWindowHeight() const
{
	return height;
}

float SceneViewWindow::GetMouseXInWindow() const
{
	return mouseX;
}

float SceneViewWindow::GetMouseYInWindow() const
{
	return mouseY;
}





