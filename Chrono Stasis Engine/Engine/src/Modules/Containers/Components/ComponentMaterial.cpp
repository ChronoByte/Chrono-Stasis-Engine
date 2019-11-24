#include "ComponentMaterial.h"
#include "csApp.h"
#include "ResourceTexture.h"
ComponentMaterial::ComponentMaterial(GameObject* parent) : Component(parent)
{
	UUID = GenerateUUID();
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
	
	ResourceTexture* matRes = (ResourceTexture*)currentResource;

	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Active Material", &active); // Can't repeat checkbox name (!!)

		ImGui::Text("Texture:");
		if (matRes != nullptr)
		{
			//DroppableSpace((matRes == nullptr) ? "No Texture" : matRes->GetName(), matRes == nullptr);
			//ImGui::SameLine();
			//ResourceExplorerButton(Resource::R_TEXTURE);


			ImGui::Image((ImTextureID*)((matRes == nullptr) ? 0 : matRes->gpu_id), ImVec2(150, 150));
			ImGui::Text("Size: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i x", (matRes == nullptr) ? 0 : matRes->width);
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", (matRes == nullptr) ? 0 :  matRes->height);
			ImGui::Text("Path: ");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
			ImGui::TextWrapped((matRes == nullptr) ? 0 : matRes->path.c_str());
			ImGui::PopStyleColor(); 
			ImGui::Text("Reference Counting: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", (matRes == nullptr) ? 0 : matRes->CountReferences());
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


void ComponentMaterial::Save(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const
{
	std::string tmp_mat;

	tmp_mat = name + "Type";
	json_object_dotset_number(object, tmp_mat.c_str(), (double)type );
	float4 tempColor = { color.r, color.g, color.b, color.a };
	
	tmp_mat = name + "UUID";
	json_object_dotset_number(object, tmp_mat.c_str(), UUID);

	if (currentResource != nullptr)
	{
		/*if (saveScene == false)
		{
			std::string tmp_res;
			std::string temp = std::to_string(countResources++);

			tmp_res = "Info.Resources.Resource " + temp + ".UUID Resource";
			json_object_dotset_number(object, tmp_res.c_str(), currentResource->GetUID());
			tmp_res = "Info.Resources.Resource " + temp + ".Name";
			json_object_dotset_string(object, tmp_res.c_str(), currentResource->GetName());
		}*/

		tmp_mat = name + "Resource Material Name";
		json_object_dotset_string(object, tmp_mat.c_str(), currentResource->GetName());

		tmp_mat = name + "Resource Material UUID";
		json_object_dotset_number(object, tmp_mat.c_str(), currentResource->GetUID());

		tmp_mat = name + "Resource Material File";
		json_object_dotset_string(object, tmp_mat.c_str(), currentResource->GetFile());

		tmp_mat = name + "Resource Material Path";
		json_object_dotset_string(object, tmp_mat.c_str(), currentResource->GetExportedFile());
	}
	else
	{
		tmp_mat = name + "Resource Material UUID";
		json_object_dotset_number(object, tmp_mat.c_str(), 0);
		tmp_mat = name + "Resource Material Path";
		json_object_dotset_string(object, tmp_mat.c_str(), "");
	}
}

void ComponentMaterial::Load(const JSON_Object* object, std::string name)
{
	std::string tmp_mat;

	tmp_mat = name + "UUID";
	UUID = json_object_dotget_number(object, tmp_mat.c_str());

	tmp_mat = name + "Resource Material UUID";
	UID resUUID = json_object_dotget_number(object, tmp_mat.c_str());

	tmp_mat = name + "Resource Material Path";
	std::string exported_file = json_object_dotget_string(object, tmp_mat.c_str());

	tmp_mat = name + "Resource Material File";
	std::string file = json_object_dotget_string(object, tmp_mat.c_str());

	tmp_mat = name + "Resource Material Name";
	std::string name_file = json_object_dotget_string(object, tmp_mat.c_str());

	if (resUUID > 0)
	{
		ResourceTexture* resMat = (ResourceTexture*)App->resources->GetResource(resUUID);
		if (resMat != nullptr)
		{
			this->AssignResource(resUUID);
			resMat->LoadToMemory();
		}
		else 
		{
			resMat = (ResourceTexture*)App->resources->CreateNewResource(Resource::R_TEXTURE, resUUID, name_file.c_str(), file.c_str(), exported_file.c_str(), true);
			this->AssignResource(resUUID);
			resMat->LoadToMemory();


			/*JSON_Value* config_file;
			JSON_Object* config;
			JSON_Object* config_node;

			config_file = json_parse_file(App->serialization->model_to_serialize.c_str());

			config = json_value_get_object(config_file);
			config_node = json_object_get_object(config, "Model");
			std::string file = json_object_dotget_string(config_node, "Info.Directory Model");

			int ResNum = json_object_dotget_number(config_node, "Info.Resources.Number of Resources");
			std::string name_file;

			for (int i = 0; i < ResNum; i++)
			{
				std::string tmp_res = "Info.Resources.Resource " + std::to_string(i) + ".UUID Resource";
					UID tmp_uid = json_object_dotget_number(config_node, tmp_res.c_str());

				if (tmp_uid == resUUID)
				{
				tmp_res = "Info.Resources.Resource " + std::to_string(i) + ".Name";
				name_file = json_object_dotget_string(config_node, tmp_res.c_str());
				break;
				}

			}*/


		}
	}
}
