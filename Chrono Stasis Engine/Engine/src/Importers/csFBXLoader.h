#pragma once
#include "csModule.h"
#include "csGlobals.h"
#include "csMesh.h"

class ModuleFBXLoader : public Module
{
public:
	 ModuleFBXLoader(bool start_enabled = true);
	~ModuleFBXLoader();

	 bool Init(JSON_Object* node);
	 bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	
	bool CleanUp();
	
public:
	Mesh* LoadFBXData(const char* fbx_name);
	bool SaveMeshData(const char* fbx_name, Mesh* mesh_data);
	bool LoadMeshData();

};