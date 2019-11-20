#include "ComponentMaterial.h"
#include "csApp.h"
#include "ResourceTexture.h"
ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(parent)
{
	type = ComponentType::C_MATERIAL;
	checkersTex = App->texture->testTexture;
	name = "Material"; 
}

ComponentMaterial::~ComponentMaterial()
{
	ResourceTexture* texReference = (ResourceTexture*)currentResource;
	if (texReference != nullptr)
		texReference->UnloadFromMemory();

	LOG("Deleting component material from %s", owner->GetName());
}

void ComponentMaterial::Update(float dt)
{
	ResourceTexture* texReference = (ResourceTexture*)currentResource;

	if (texReference != nullptr)
		glBindTexture(GL_TEXTURE_2D, texReference->gpu_id);
	else
		glColor3f(1, 1, 1);
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
	
	ResourceTexture* texReference = (ResourceTexture*)currentResource;

	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active Material", &active); // Can't repeat checkbox name (!!)

		ImGui::Text("Texture:");
		if (texture != nullptr)
		{
			DroppableSpace((texReference == nullptr) ? "No Texture" : texReference->GetName(), texReference == nullptr);
			ImGui::SameLine();
			ResourceExplorerButton(Resource::R_TEXTURE);


			ImGui::Image((ImTextureID*)texture->id, ImVec2(150, 150));
			ImGui::Text("Size: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i x", texture->width);
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", texture->height);
			ImGui::Text("Path: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
			ImGui::TextWrapped(texture->path.c_str());
			ImGui::PopStyleColor(); 
			ImGui::Text("Reference Counting: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", (texReference == nullptr) ? 0 : texReference->CountReferences());
			//ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", texture->path.c_str());
		}

		ImGui::Checkbox("Checkers Texture", &checkers);

		if (ImGui::TreeNodeEx("Color", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::ColorEdit3("", (float*)&color);
			ImGui::TreePop();
		}
	}
	//TODO: Set more info
}

void ComponentMaterial::Save(RJSON_Value* component) const
{
	RJSON_Value* texture = component->CreateValue(rapidjson::kObjectType);

	texture->SetUint("UUID", UUID);
	component->AddValue("Material", *texture);

	//TODO: MAKE A TEXTURE RESOURCE
		//texture->SetString("texture", file);
}

void ComponentMaterial::Load(RJSON_Value* component)
{
}
