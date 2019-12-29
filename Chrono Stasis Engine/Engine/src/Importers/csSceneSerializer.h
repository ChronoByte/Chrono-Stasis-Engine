#pragma once

#include "csModule.h"
#include "csGlobals.h"



class ModuleSceneSerializer : public Module
{
public:
	ModuleSceneSerializer(bool start_enabled = true);
	~ModuleSceneSerializer();

	bool Init(JSON_Object* node);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	// Parser
	void SaveScene(const char* scene_path);
	void LoadScene(const char* scene_path);
	void SaveGameObjects(JSON_Object* config_node, const GameObject& gameObject, uint& count, uint& countResources);
	void LoadGameObjects(GameObject& parent, GameObject& child, int uuidParent);

	void SaveModel(const GameObject& go, const char* dir, const char* file_path);
	void LoadModel(const char* prefab);
	void SaveModelChildren(JSON_Object* config_node, const GameObject& go, uint& count, uint& countResources);
	void LoadModelChildren(GameObject& parent, GameObject& child, UID uuidParent);

	void SaveParticleSystem(const char* particle_path);
	void LoadParticleSystem(const char* particle_path);
private:
	//vars
	std::string directory;
public:
	std::string current_scene;
	std::string scene_to_serialize;
	std::string model_to_serialize;
	std::string particle_to_serialize;
	std::string particle_template;
	Component* particleCallback = nullptr;
};