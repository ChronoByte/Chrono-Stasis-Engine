#include "csApp.h"
#include "csFBXLoader.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Engine/Dependencies/Assimp/libx86/assimp.lib")

ModuleFBXLoader::ModuleFBXLoader(bool start_enabled) : Module(start_enabled)
{
	//name = "FBXLoader";
}

ModuleFBXLoader::~ModuleFBXLoader()
{

}

bool ModuleFBXLoader::Init(JSON_Object* node)
{
	return true;
}


bool ModuleFBXLoader::Start()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	return true;
}

update_status ModuleFBXLoader::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleFBXLoader::Update(float dt)
{

	if(App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN){
	
		if (!meshes.empty()) {
			for (uint i = 0; i < meshes.size(); i++)
			{
				LOG("Mesh num: %d", i);
				LOG("Num of Vertices: %d", meshes[i].num_vertices);
				LOG("Num of Indices: %d", meshes[i].num_indices);
			}
		}
	
	}
		
	
	return UPDATE_CONTINUE;
}

update_status ModuleFBXLoader::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleFBXLoader::CleanUp()
{
	aiDetachAllLogStreams();
	
	return true;
}

bool ModuleFBXLoader::LoadFBXData(const char* fbx_name)
{
	bool ret = false;
	const aiScene* scene = aiImportFile(fbx_name, aiProcessPreset_TargetRealtime_MaxQuality); //container of meshes
	
	aiMesh* new_mesh = nullptr; //pointer interator of meshes
	
	if (scene != nullptr && scene->HasMeshes())
	{
		
		for (uint i = 0; i < scene->mNumMeshes; i++) // Use scene->mNumMeshes to iterate on scene->mMeshes array
		{
		
			new_mesh = scene->mMeshes[i]; 

			// copy vertices
			m.num_vertices = new_mesh->mNumVertices;
			m.vertices = new float[m.num_vertices * 3];
			memcpy(m.vertices, new_mesh->mVertices, sizeof(float) * m.num_vertices * 3);


			// copy faces
			if (new_mesh->HasFaces())
			{
				m.num_indices = new_mesh->mNumFaces * 3;
				m.indices = new uint[m.num_indices]; // assume each face is a triangle
			
				for (uint i = 0; i < new_mesh->mNumFaces; ++i)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");
					}
				
					else memcpy(&m.indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}
			}

			LOG("Mesh Loaded Correctly!");
			meshes.push_back(m); // Push mesh into container of meshes
		}

		
		aiReleaseImport(scene); // a tomar por culo el fbx 
		scene = nullptr;
	}
	else
		LOG("Error loading scene %s", fbx_name);
	return ret;
}
