#include "TextureBrowserWindow.h"
#include "csApp.h"
#include "csTextureLoader.h"

TextureBrowserWindow::TextureBrowserWindow(bool startOpened) : Window(startOpened)
{
}

TextureBrowserWindow::~TextureBrowserWindow()
{
}

void TextureBrowserWindow::Draw()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse;

	std::vector<TextureInfo*> textures = App->texture->textures;

	if (ImGui::Begin("Textures", &active, flags))
	{
		ImGui::Text("%i", textures.size());
		for (std::vector<TextureInfo*>::iterator texs = textures.begin(); texs != textures.end(); ++texs)
		{
			ImGui::Image((ImTextureID)(*texs)->id, ImVec2(PREVIEW_SIZE, PREVIEW_SIZE), { 0,1 }, { 1,0 });
		}
	}

	ImGui::End();

}
