#include "csApp.h"
#include "csSceneSerializer.h"
#include "csGameObject.h"
#include "ComponentParticleSystem.h"
#include "Particle_System/csParticleSystem.h"
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


void ModuleSceneSerializer::SaveScene(const char* scene_path)
{
	LOG("SAVING SCENE -----");

	std::string extension = SCENES_EXTENSION;
	//extension.append(".json"); // to view in tree json
	std::string scene_file = scene_path + extension;

	uint count = 0;
	uint countResources = 0;

	JSON_Value* config_file;
	JSON_Object* config;
	JSON_Object* config_node;

	config_file = json_value_init_object();

	if (config_file != nullptr)
	{
		config = json_value_get_object(config_file);
		json_object_clear(config);
		json_object_dotset_number(config, "Scene.Info.Number of GameObjects", App->scene->GetRoot()->childs.size());
		config_node = json_object_get_object(config, "Scene");

		// Update GameObjects
		for (auto go : App->scene->GetRoot()->childs)
		{
			std::string name = "GameObject" + std::to_string(count++);
			std::string tmp_go;
			name += ".";

			// UUID--------
			tmp_go = name + "UUID";
			json_object_dotset_number(config_node, tmp_go.c_str(), go->GetUUID());
			// Parent UUID------------
			tmp_go = name + "ParentUUID";
			json_object_dotset_number(config_node, tmp_go.c_str(), -1);
			// Name --------
			tmp_go = name + "Name";
			json_object_dotset_string(config_node, tmp_go.c_str(), go->GetName());
			// Active ---------
			tmp_go = name + "Active";
			json_object_dotset_boolean(config_node, tmp_go.c_str(), go->isActive());
			// Static ---------
			tmp_go = name + "Static";
			json_object_dotset_boolean(config_node, tmp_go.c_str(), go->isStatic());

			// Components  ------------
			std::string components = name;
			std::string tmp_comp = components + "Number of Components";
			json_object_dotset_number(config_node, tmp_comp.c_str(), go->components.size());

			if (go->components.size() > 0)
			{
				components += "Components.";
				go->SaveComponents(config_node, components, true, countResources);
			}

			// Children GO -------------
			if (go->childs.size() > 0)
			{
				for (auto child : go->childs)
				{
					SaveGameObjects(config_node, *child, count, countResources);
				}
			}
		}
		
		json_object_dotset_number(config_node, "Info.Number of GameObjects", count);
		json_serialize_to_file(config_file, scene_file.c_str());
	}

	json_value_free(config_file);

}

void ModuleSceneSerializer::LoadScene(const char* scene_path)
{
	LOG("LOADING SCENE -----");
	JSON_Value* config_file;
	JSON_Object* config;
	JSON_Object* config_node;

	config_file = json_parse_file(scene_path);

	if (config_file != nullptr)
	{
		config = json_value_get_object(config_file);
		config_node = json_object_get_object(config, "Scene");
		int NUmberGameObjects = json_object_dotget_number(config_node, "Info.Number of GameObjects");
		if (NUmberGameObjects > 0)
		{
			
			GameObject* go = nullptr;
			GameObject* lastParent = nullptr;
			for (int i = 0; i < NUmberGameObjects; i++)
			{

				std::string name = "GameObject" + std::to_string(i);
				name += ".";
				std::string tmp_go;

				// Create Game Object + Set Name & UUID ------
				tmp_go = name + "Name";
				std::string nameGO = json_object_dotget_string(config_node, tmp_go.c_str());
				tmp_go = name + "UUID";
				UID uid = json_object_dotget_number(config_node, tmp_go.c_str());
				tmp_go = name + "ParentUUID";
				long long  parentUUID = json_object_dotget_number(config_node, tmp_go.c_str());

				if (parentUUID == -1)
				{
					 go = App->scene->LoadGameObject(nullptr, nameGO.c_str(), uid);

				}
				else 
				{
					 go = App->scene->LoadGameObject(lastParent, nameGO.c_str(), uid);
				}

				// Set Static -------
				tmp_go = name + "Static";
				bool staticGO = json_object_dotget_boolean(config_node, tmp_go.c_str());
				go->SetStatic(staticGO);

				// Set Active -------
				tmp_go = name + "Active";
				bool activeGO = json_object_dotget_boolean(config_node, tmp_go.c_str());
				go->SetActive(activeGO);

				//Load Components -----
				std::string tmp_comp;
				tmp_comp = name + "Number of Components";
				int CompNums = json_object_dotget_number(config_node, tmp_comp.c_str());

				if (CompNums > 0)
				{
					go->LoadComponents(config_node, name + "Components.", CompNums);
				}

				
				//Add GameObject
				if (parentUUID == -1)
				{
					lastParent = go;
				}
				else
				{
					GameObject* parent = App->scene->GetRoot();
					for (auto& child : parent->childs)
					{
						LoadModelChildren(*child, *go, parentUUID);

					}
				}
					
				
			}
		}
	}
	json_value_free(config_file);
}

void ModuleSceneSerializer::SaveGameObjects(JSON_Object* config_node, const GameObject& GOchild, uint& count, uint& countResources)
{
	// Update GameObjects
	std::string name = "GameObject" + std::to_string(count++);
	std::string tmp_go_child;
	name += ".";
	// UUID--------
	tmp_go_child = name + "UUID";
	json_object_dotset_number(config_node, tmp_go_child.c_str(), GOchild.GetUUID());

	// Parent UUID------------
	UID ParentUUID = -1;
	if (GOchild.GetParent() != nullptr)
	{
		ParentUUID = GOchild.GetParent()->GetUUID();
	}
	tmp_go_child = name + "ParentUUID";
	json_object_dotset_number(config_node, tmp_go_child.c_str(), ParentUUID);

	// Name- --------
	tmp_go_child = name + "Name";
	json_object_dotset_string(config_node, tmp_go_child.c_str(), GOchild.GetName());

	// Active ---------
	tmp_go_child = name + "Active";
	json_object_dotset_boolean(config_node, tmp_go_child.c_str(), GOchild.isActive());

	// Static ---------
	tmp_go_child = name + "Static";
	json_object_dotset_boolean(config_node, tmp_go_child.c_str(), GOchild.isStatic());

	// Components  ------------
	std::string components = name;
	std::string tmp_comp = components + "Number of Components";
	json_object_dotset_number(config_node, tmp_comp.c_str(), GOchild.components.size());
	
	if (GOchild.components.size() > 0)
	{
		components += "Components.";
		GOchild.SaveComponents(config_node, components, true, countResources);
	}

	// Children GO -------------
	if (GOchild.childs.size() > 0)
	{
		for (auto child : GOchild.childs)
		{
			SaveGameObjects(config_node, child, count, countResources);
		}
	}
}

void ModuleSceneSerializer::LoadGameObjects(GameObject& parent, GameObject& child, int uuidParent)
{
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
	nameJson += ".fbx.meta.json";
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
	UID uuidParent = -1;
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
	model_to_serialize = model;

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
				//GameObject* go = new GameObject(GOname, uid);
				GameObject* go = App->scene->LoadGameObject(parent, GOname.c_str(), uid);

				//Load Components
				tmp_go = name + "Number of Components";
				int CompNums = json_object_dotget_number(config_node, tmp_go.c_str());

				if (CompNums > 0)
				{
					go->LoadComponents(config_node, name + "Components.", CompNums);
				}

				tmp_go = name + "Parent";
				UID uuid_parent = json_object_dotget_number(config_node, tmp_go.c_str());

				//Add GameObject
				if (uuid_parent == -1)
					parent = go;
				else
					LoadModelChildren(*parent, *go, uuid_parent);

			}

		}
	}
	json_value_free(config_file);
}

void ModuleSceneSerializer::LoadModelChildren(GameObject& GOparent, GameObject& GOchild, UID uuidParent)
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

void ModuleSceneSerializer::SaveParticleSystem(const char* particle_path)
{
	LOG("SAVING PARTICLE SYSTEM -----");

	std::string extension = PARTICLE_SYSTEM_EXTENSION;
	std::string particle_file = particle_path + extension;

	
	uint countResources = 0;

	JSON_Value* config_file;
	JSON_Object* config;
	JSON_Object* config_node;

	config_file = json_value_init_object();

	if (config_file != nullptr)
	{
		config = json_value_get_object(config_file);

		std::string temp = name + "ParticleSystem.";
		particleCallback->Save(config, temp, true, countResources);

		json_serialize_to_file(config_file, particle_file.c_str());
	}

	json_value_free(config_file);
}

void ModuleSceneSerializer::LoadParticleSystem(const char* particle_path)
{

	LOG("LOADING PARTICLE SYSTEM -----");

	JSON_Value* config_file;
	JSON_Object* config;
	JSON_Object* config_node;

	config_file = json_parse_file(particle_path);

	if (config_file != nullptr)
	{
		config = json_value_get_object(config_file);
		//config_node = json_object_get_object(config, "ParticleSystem");

		std::string temp = name + "ParticleSystem.";
		ComponentParticleSystem* particleComp = (ComponentParticleSystem*)particleCallback;
		ParticleSystem* system = particleComp->GetSystem();
		system->ResetSystem();

		ResourceTexture* resMat = (ResourceTexture*)particleComp->GetCurrentResource();


		particleCallback->Load(config, temp);

		if (resMat != nullptr)
			resMat->UnloadFromMemory();
	}
	json_value_free(config_file);


}
