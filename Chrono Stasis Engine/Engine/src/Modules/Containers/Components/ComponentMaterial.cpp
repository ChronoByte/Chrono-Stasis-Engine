#include "ComponentMaterial.h"
#include "csApp.h"

ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(parent)
{
	type = ComponentType::C_MATERIAL;
	checkersTex = App->texture->testTexture;
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::SetColor(float r, float g, float b, float a)
{
	color = Color(r,g,b,a);
}

Color ComponentMaterial::GetColor() const
{
	return color;
}

const TextureInfo* ComponentMaterial::GetTexture() const
{
	return checkers ? checkersTex : texture;
}

void ComponentMaterial::SetTexture(TextureInfo* texture)
{
	this->texture = texture;
}

void ComponentMaterial::SetMaterial(TextureInfo* texture, float r, float g, float b, float a)
{
	SetTexture(texture);
	SetColor(r, g, b, a);
}

void ComponentMaterial::InspectorInfo()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Mat", &active); // Can't repeat checkbox name (!!)
		
		ImGui::Text("Texture:");
		if (texture != nullptr)
			ImGui::Image((ImTextureID*)texture->id, ImVec2(150, 150));

		ImGui::Checkbox("Checkers Texture", &checkers);

		if (ImGui::TreeNodeEx("Color", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::ColorEdit3("", (float*)&color);
			ImGui::TreePop();
		}
	}
	//TODO: Set more info
}
