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

bool ModuleSceneSerializer::Save(const char* scene_path)
{
	std::string dir = SCENES_FOLDER;
	std::string extension = SCENES_EXTENSION;
	std::string path = dir + scene_path + extension;

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
	return true;
}

bool ModuleSceneSerializer::Load(const char* scene_path)
{
	return true;
}
