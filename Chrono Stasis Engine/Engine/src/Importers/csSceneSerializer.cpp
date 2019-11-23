#include "csApp.h"
#include "csSceneSerializer.h"
#include "csGameObject.h"

ModuleSceneSerializer::ModuleSceneSerializer(bool start_enabled)
{
}

ModuleSceneSerializer::~ModuleSceneSerializer()
{
}

bool ModuleSceneSerializer::Init(JSON_Object* node)
{
	return true;
}

bool ModuleSceneSerializer::Start()
{
	return true;
}

update_status ModuleSceneSerializer::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneSerializer::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneSerializer::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleSceneSerializer::CleanUp()
{
	return true;
}

bool ModuleSceneSerializer::SaveScene(const char* scene_path, std::string dir)
{

	std::string extension = SCENES_EXTENSION;
	std::string directory = A_SCENES_FOLDER;
	std::string path;

	if(!dir.compare("ASSET"))
		path = scene_path + extension;

	if (!dir.compare("LIBRARY")) 
	{
		App->fs->GetNameFile(scene_path, path);
		path = directory + path + extension;
	}

	RJSON_File* scene = App->json->JSONWrite(path.c_str());

	if (scene != nullptr)
	{
		RJSON_Value* gameObjects = scene->CreateValue(rapidjson::kArrayType);
		GameObject* rootGO = App->scene->GetRoot();
		
		for (auto& child : rootGO->childs)
			child->Save(gameObjects);

		scene->AddValue("GameObjects", *gameObjects);
		scene->WriteFile();

	}

	App->json->JSONClose(scene);
	LOG("Scene &s Serialized successfully", App->serialization->current_scene.c_str());
	return true;
}

bool ModuleSceneSerializer::LoadScene(const char* scene_path, std::string dir)
{
	return true;
}

void ModuleSceneSerializer::SaveModel(const GameObject& go, const char* dir, const char* file_path)
{

	LOG("SAVING MODEL %s -----", go.GetName());

	JSON_Value* config_file;
	JSON_Object* config;
	JSON_Object* config_node;

	std::string nameJson = dir;
	nameJson += "/";
	nameJson += go.GetName();
	nameJson += ".fbx.meta";
	config_file = json_value_init_object();

	uint count = 0;
	uint countResources = 0;
	if (config_file != nullptr)
	{
		config = json_value_get_object(config_file);
		json_object_clear(config);
		json_object_dotset_number(config, "Model.Info.Number of GameObjects", count);
		json_object_dotset_string(config, "Model.Info.Directory Model", file_path);
		json_object_dotset_number(config, "Model.Info.Resources.Number of Resources", countResources);
		config_node = json_object_get_object(config, "Model");
		std::string name = "GameObject" + std::to_string(count++);
		name += ".";

		// UUID--------
		std::string tmp_name;
		tmp_name = name + "UUID";
		json_object_dotset_number(config_node, tmp_name.c_str(), go.GetUUID());
		// Parent UUID------------
		tmp_name = name + "Parent";
		json_object_dotset_number(config_node, tmp_name.c_str(), -1);
		// Name- --------
		tmp_name = name + "Name";
		json_object_dotset_string(config_node, tmp_name.c_str(), go.GetName());

		// Components  ------------
		std::string components = name;
		std::string tmp_comp = components + "Number of Components";
		json_object_dotset_number(config_node, tmp_comp.c_str(), go.GetComponentsNum());
		if (go.components.size() > 0)
		{
			components += "Components.";
			go.SaveComponents(config_node, components, false, countResources);
		}
		// Childs --------------
		if (go.childs.size() > 0)
		{
			for (auto& child : go.childs)
			{
				SaveModelChildren(config_node, *child, count, countResources);
			}

		}
		json_object_dotset_number(config_node, "Info.Number of GameObjects", count);
		json_object_dotset_number(config, "Model.Info.Resources.Number of Resources", countResources);
		json_serialize_to_file(config_file, nameJson.c_str());
	}
	json_value_free(config_file);

}

void ModuleSceneSerializer::SaveModelChildren(JSON_Object* config_node, const GameObject& go, uint& count, uint& countResources)
{
	std::string tmp_child;

	// Update GameObjects
	std::string name = "GameObject" + std::to_string(count++);
	name += ".";
	// UUID--------
	tmp_child = name + "UUID";
	json_object_dotset_number(config_node, tmp_child.c_str(), go.GetUUID());
	// Parent UUID------------
	int uuidParent = -1;
	if (go.GetParent() != nullptr)
		uuidParent = go.GetParent()->GetUUID();
	
	tmp_child = name + "Parent";
	json_object_dotset_number(config_node, tmp_child.c_str(), uuidParent);
	
	// Name- --------
	tmp_child = name + "Name";
	json_object_dotset_string(config_node, tmp_child.c_str(), go.GetName());

	// Components ------------
	std::string components = name;
	std::string tmp_comp = components + "Number of Components";
	json_object_dotset_number(config_node, tmp_comp.c_str(), go.GetComponentsNum());
	if (go.components.size() > 0)
	{
		components += "Components.";
		go.SaveComponents(config_node, components, false, countResources);
	}
	// Childs --------------
	if (go.childs.size() > 0)
	{
		for (auto& child : go.childs)
		{
			SaveModelChildren(config_node, *child, count, countResources);
		}

	}
}

void ModuleSceneSerializer::LoadModel(const char* model)
{
	LOG("LOADING MODEL %s -----", model);

	JSON_Value* config_file;
	JSON_Object* config;
	JSON_Object* config_node;

	config_file = json_parse_file(model);
	if (config_file != nullptr)
	{
		config = json_value_get_object(config_file);
		config_node = json_object_get_object(config, "Model");
		int GOnums = json_object_dotget_number(config_node, "Info.Number of GameObjects");

		if (GOnums > 0)
		{
		
			GameObject* parent = nullptr;
			for (int i = 0; i < GOnums; i++)
			{
				std::string name = "GameObject" + std::to_string(i) + ".";//name += ".";
				std::string tmp_go;

				//Get Basic GO data
				tmp_go = name + "Name";
				std::string GOname = json_object_dotget_string(config_node, tmp_go.c_str());
				tmp_go = name + "UUID";
				UID uid = json_object_dotget_number(config_node, tmp_go.c_str());

				//Create GameObject
				GameObject* go = new GameObject(GOname, uid);

				//Load Components
				tmp_go = name + "Number of Components";
				int CompNums = json_object_dotget_number(config_node, tmp_go.c_str());

				if (CompNums > 0)
				{
					go->LoadComponents(config_node, name + "Components.", CompNums);
				}

				tmp_go = name + "Parent";
				int uuid_parent = json_object_dotget_number(config_node, tmp_go.c_str());

				//Add GameObject
				if (uuid_parent == -1)
					parent = go;
				else
					LoadModelChildren(*parent, *go, uuid_parent);
				
			}
			
			// Add GameObject
			App->scene->SetRoot(parent);
		}
	}
	json_value_free(config_file);
}

void ModuleSceneSerializer::LoadModelChildren(GameObject& GOparent, GameObject& GOchild, int uuidParent)
{
	
	if (GOparent.childs.size() > 0)
	{
		for (auto& child : GOparent.childs)
		{
			
			if (GOparent.GetUUID() == uuidParent)
			{
				GOparent.LoadGameObjectChild(&GOchild);
				return;
			}
			else
			{
				LoadModelChildren(*child, GOchild, uuidParent);
			}
		}
	}
	else
	{
		if (GOparent.GetUUID() == uuidParent)
		{
			GOparent.LoadGameObjectChild(&GOchild);
			return;
		}
	}
}
