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
	bool Save(const char* scene_path);
	bool Load(const char* scene_path);

private:
	//vars
};