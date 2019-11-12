#pragma once
#include "csModule.h"
#include "csGlobals.h"
#include <array>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

class ComponentMesh; 
class GameObject; 

enum FileType
{
	NONE = -1,
	UNKNOWN,
	MODEL,
	TEXTURE
};

struct FBXModel
{
	// Meshes of Model
	std::vector<ComponentMesh*> meshes;
	
	//Transform of Model
	float3 position;
	float3 rotation;
	float3 scale;

	std::array<float3, 3> transform; //just for test transform properties on compile time

	float3 GetPosition() const { return transform.at(0); };
	float3 GetRotation() const { return transform.at(1); };
	float3 GetScale() const { return transform.at(2); };;
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
	void FBXModelImport(const char* path);
	GameObject* LoadModel(const char* path);

	ComponentMesh* LoadMesh(aiMesh* mesh,const aiScene* scene);
	void NodePath(aiNode* node, const aiScene* scene);

	GameObject* LoadFBXData(const char* fbx_name);
	void GrowBoundingBox(aiMesh* mesh, float3* min, float3* max);
	void SetBoundingBox(const aiScene* scene);
	bool SaveMeshData(const char* fbx_name, ComponentMesh* mesh_data);
	bool LoadMeshData();

private:
	FBXModel* model = nullptr;
	GameObject* newGo = nullptr; 
	FileType type = FileType::NONE;

	std::string filePath;
};