#include "csApp.h"
#include "csFBXLoader.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Engine/Dependencies/Assimp/libx86/assimp.lib")

ModuleFBXLoader::ModuleFBXLoader(bool start_enabled) : Module(start_enabled)
{
	name = "FBXLoader";
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

	return UPDATE_CONTINUE;
}

update_status ModuleFBXLoader::PostUpdate(float dt)
{
	if (App->input->imported)
	{
		LoadFBXData(App->input->dropped_filedir);
		App->input->imported = false;
	}
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
			
			LOG("Mesh num: %u", i);

			m.LoadMeshVertices(new_mesh);

			if (new_mesh->HasFaces())
				m.LoadMeshIndices(new_mesh);
			
			if (new_mesh->HasNormals())
				m.LoadMeshNormals(new_mesh);

			if (new_mesh->GetNumColorChannels() > 0) 
			{
				for (int i = 0; i < new_mesh->GetNumColorChannels(); ++i)
				{
					if (new_mesh->HasVertexColors(i))
						m.LoadMeshColors(new_mesh, i);
				}
			}
			else LOG("No Color Channel detected");

			if (new_mesh->GetNumUVChannels() > 0) 
			{
				for (int i = 0; i < new_mesh->GetNumUVChannels(); ++i) 
				{
					if (new_mesh->HasTextureCoords(i))
						m.LoadMeshTextureCoords(new_mesh, i);
				}
			}
			else LOG("No UV Channel detected");

			//m.LoadMeshFromFBX(new_mesh);
			m.CreateMeshBuffers(); 

			App->renderer3D->PushMeshToRender(&m);  // Push mesh into container of meshes
		}

		
		aiReleaseImport(scene); // a tomar por culo el fbx 
		scene = nullptr;
	}
	else
		LOG("Error loading scene %s", fbx_name);
	return ret;
}
