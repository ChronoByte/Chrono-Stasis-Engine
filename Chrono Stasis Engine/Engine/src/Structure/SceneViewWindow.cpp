#include "SceneViewWindow.h"
#include "csApp.h"
#include "csViewport.h"
#include "ImGuizmo/ImGuizmo.h"

#include "ComponentTransform.h"
#include "ComponentCamera.h"

SceneViewWindow::SceneViewWindow(bool startOpened) : Window(startOpened)
{

}

SceneViewWindow::~SceneViewWindow()
{
}

void SceneViewWindow::Draw()
{
	if (ImGui::Begin("Scene", &active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
	{
		//ImGui::SetCursorPos({ -(App->window->width - size.x) / 2,-(App->window->height - size.y) / 2 });

		ImVec2 size = ImGui::GetWindowSize();

		width = size.x;
		height = size.y;

		// Handle Resizing
		if (regionX != ImGui::GetContentRegionAvail().x || regionY != ImGui::GetContentRegionAvail().y)
		{
			regionX = ImGui::GetContentRegionAvail().x;
			regionY = ImGui::GetContentRegionAvail().y; 

			App->renderer3D->editorViewport->SetSize(width, height);
		}

		ImVec2 cursorPos = ImGui::GetCursorScreenPos();

		mouseX = ImGui::GetMousePos().x - cursorPos.x;
		mouseY = ImGui::GetMousePos().y - cursorPos.y;

		windowPosX = ImGui::GetWindowPos().x;
		windowPosY = ImGui::GetWindowPos().y;

		//LOG("Corrected Mouse X: %f Mouse Y: %f", ImGui::GetMousePos().x, ImGui::GetMousePos().y);


		if (!App->renderer3D->displayZBuffer)
			ImGui::Image((ImTextureID)App->renderer3D->editorViewport->renderTexture, { width, height }, { 0,1 }, { 1,0 });
		//ImGui::Image((ImTextureID)App->renderer3D->editorViewport->renderTexture, { (float)App->renderer3D->editorViewport->width, (float)App->renderer3D->editorViewport->height }, { 0,1 }, { 1,0 });
		else
			ImGui::Image((ImTextureID)App->renderer3D->editorViewport->zBufferTexture, { (float)App->renderer3D->editorViewport->width, (float)App->renderer3D->editorViewport->height }, { 0,1 }, { 1,0 });


		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(cursorPos.x, cursorPos.y, width, height);

		if (App->scene->GetSelected() != nullptr)
		{
			ImGuizmo::Enable(true);
			UpdateGuizmo();
		}
		else ImGuizmo::Enable(false);



		/*int new_width, new_height;
		App->window->GetWindowSize(new_width, new_height);
		if (App->window->width != new_width || App->window->height != new_height)
		{

			App->window->width = new_width;
			App->window->height = new_height;
			App->renderer3D->OnResize(new_width, new_height);
		}*/


		if (ImGui::IsWindowHovered())
			App->camera->cameraControls = true;
		else
			App->camera->cameraControls = false;
	}

	ImGui::End();

}

void SceneViewWindow::UpdateGuizmo()
{
	GameObject* selected = App->scene->GetSelected();

	static ImGuizmo::OPERATION operation = ImGuizmo::OPERATION::TRANSLATE;
	static ImGuizmo::MODE mode = ImGuizmo::MODE::WORLD;
	float snap = 0.f;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		operation = ImGuizmo::OPERATION::TRANSLATE;
		mode = ImGuizmo::MODE::LOCAL;
	}

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		operation = ImGuizmo::OPERATION::ROTATE;
		mode = ImGuizmo::MODE::LOCAL;
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		operation = ImGuizmo::OPERATION::SCALE;
		mode = ImGuizmo::MODE::WORLD;
	}

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		snap = 10.f;
	}

	float4x4 workingMatrix = selected->GetTransform()->GetGlobalTransform().Transposed();

	ImGuizmo::Manipulate(App->camera->fakeCamera->GetViewMatrix().ptr(),
		App->camera->fakeCamera->GetProjectionMatrix().ptr(),
		operation, mode,
		workingMatrix.ptr(), nullptr, &snap);

	if (ImGuizmo::IsUsing())
	{
		float3 pos = float3::zero;
		Quat rot = Quat::identity;
		float3 scale = float3::zero;

		workingMatrix.Transpose();
		workingMatrix.Decompose(pos, rot, scale);

		selected->GetTransform()->SetPosition(pos);
		selected->GetTransform()->SetRotationQuat(rot);
		selected->GetTransform()->SetScale(scale);

		selected->GetTransform()->toRecalculateTransform = true;
	}
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





