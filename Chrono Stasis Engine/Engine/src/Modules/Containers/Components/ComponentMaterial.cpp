#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(parent)
{
	type = ComponentType::C_MATERIAL;
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
	return texture;
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
		//if (ImGui::Checkbox("Active Component", &active)) {

			ImGui::Text("Texture:");
			ImGui::Image((ImTextureID*)2, ImVec2(150, 150));

			if (ImGui::TreeNodeEx("Color", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::ColorEdit3("", (float*)&color);
				ImGui::TreePop();
			}
		//}
	}
	//TODO: Set more info
	
}
