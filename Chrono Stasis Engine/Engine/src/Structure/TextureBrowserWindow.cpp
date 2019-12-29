#include "TextureBrowserWindow.h"
#include "csApp.h"
#include "csResources.h"
#include "ResourceTexture.h"
#include "ComponentParticleSystem.h"

TextureBrowserWindow::TextureBrowserWindow(bool startOpened) : Window(startOpened)
{
}

TextureBrowserWindow::~TextureBrowserWindow()
{
}

void TextureBrowserWindow::Draw()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse;

	resources = App->resources->GetResourcesFromType(Resource::R_TEXTURE);

	if (ImGui::Begin("Textures", &active))
	{
		ImGui::Text("Total Materials: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", resources.size());
		ImGui::Separator();
		for (std::vector<Resource*>::iterator texs = resources.begin(); texs != resources.end(); ++texs)
		{
			ResourceTexture* texResource = (ResourceTexture*)(*texs);

			if (lastNumResources != resources.size())
			{
				if (texResource->gpu_id == 0) {
					//callback->LoadResource(texResource);
					texResource->LoadToMemory();
				}
				
			}

			if(ImGui::ImageButton((ImTextureID)texResource->gpu_id, ImVec2(PREVIEW_SIZE, PREVIEW_SIZE), { 0,1 }, { 1,0 }))
			{
				callback->AssignResource(texResource->GetUID());
				active = false;
			}
			
			ImGui::SameLine();
			ImGui::Text("Texture Name: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", texResource->GetName());
			
			ImGui::Text("Buffer ID: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", texResource->gpu_id);

			ImGui::Text("Times Used: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", texResource->CountReferences()-1);
	
			ImGui::Separator();
		}
		lastNumResources = resources.size();
	}

	ImGui::End();

}


