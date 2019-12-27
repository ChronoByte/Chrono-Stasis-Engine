#include "TextureBrowserWindow.h"
#include "csApp.h"
#include "csResources.h"
#include "ResourceTexture.h"

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

	if (ImGui::Begin("Textures", &active, flags))
	{
		ImGui::Text("%i", resources.size());
		for (std::vector<Resource*>::iterator texs = resources.begin(); texs != resources.end(); ++texs)
		{
			ResourceTexture* texResource = (ResourceTexture*)(*texs);
			if(ImGui::ImageButton((ImTextureID)texResource->gpu_id, ImVec2(PREVIEW_SIZE, PREVIEW_SIZE), { 0,1 }, { 1,0 }))
			{
				callback->AssignResource(texResource->GetUID());
			}

			ImGui::Text("%i", texResource->gpu_id);
		}
	}

	ImGui::End();

}


