#pragma once
#include "csModule.h"
#include "csGlobals.h"


struct Mesh
{
	uint id_indices = 0; // index in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_vertices = 0; // unique vertex in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;

	~Mesh()
	{
		delete[] indices;
		delete[] vertices;
	}

	
};



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
	bool LoadFBXData(const char* fbx_name);

private:
	Mesh m;
	std::vector<Mesh> meshes;
};