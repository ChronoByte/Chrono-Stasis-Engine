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

}
