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
	bool SaveScene(const char* scene_path, std::string dir);
	bool LoadScene(const char* scene_path, std::string dir);

private:
	//vars
	std::string directory;
public:
	std::string current_scene;
	std::string scene_to_serialize;
};