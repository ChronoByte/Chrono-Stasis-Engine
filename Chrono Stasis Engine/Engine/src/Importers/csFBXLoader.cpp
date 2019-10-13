#include "csApp.h"
#include "csFBXLoader.h"
#include "csFileSystem.h"
#include "ComponentMesh.h"

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

	return UPDATE_CONTINUE;
}

bool ModuleFBXLoader::CleanUp()
{
	

	aiDetachAllLogStreams();
	
	return true;
}

void ModuleFBXLoader::FBXModelImport(const char* fbx_name)
{
	FBXModel* new_model = LoadModel(fbx_name);

}

FBXModel* ModuleFBXLoader::LoadModel(const char* path)
{
	//bool ret = false;
	//FBXModel* new_model = nullptr;

	model = new FBXModel();

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		
		NodePath(scene->mRootNode, scene);


		aiQuaternion quat_rotation;
		aiVector3D position;
		aiVector3D scale;

		scene->mRootNode->mTransformation.Decompose(scale, quat_rotation, position);
		Quat rot(quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);

		float3 euler_rotation = rot.ToEulerXYZ();

		model->transform[0].Set(position.x, position.y, position.z);
		model->transform[1].Set(euler_rotation.x, euler_rotation.y, euler_rotation.z);
		model->transform[2].Set(scale.x, scale.y, scale.z);

		//-----------------------------------

		aiReleaseImport(scene);

		return model;
	}

}

void ModuleFBXLoader::NodePath(aiNode* node, const aiScene* scene)
{
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		model->meshes.push_back(LoadMesh(mesh, scene));
	}

	for (uint i = 0; i < node->mNumChildren; i++)
	{
		NodePath(node->mChildren[i], scene);
	}
}

ComponentMesh* ModuleFBXLoader::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	ComponentMesh* m = new ComponentMesh(nullptr);

	m->LoadMeshVertices(mesh);

	if (mesh->HasFaces())
		m->LoadMeshIndices(mesh);

	if (mesh->HasNormals())
		m->LoadMeshNormals(mesh);

	if (mesh->GetNumColorChannels() > 0)
	{
			if (mesh->HasVertexColors(0))
				m->LoadMeshColors(mesh, 0);
	}
	else LOG("No Color Channel detected");

	if (mesh->GetNumUVChannels() > 0)
	{
			if (mesh->HasTextureCoords(0))
			m->LoadMeshTextureCoords(mesh, 0);
	}
	else LOG("No UV Channel detected");


	return m;
}


GameObject* ModuleFBXLoader::LoadFBXData(const char* fbx_name)
{
	bool ret = false;
	const aiScene* scene = aiImportFile(fbx_name, aiProcessPreset_TargetRealtime_MaxQuality); //container of meshes
	
	aiMesh* new_mesh = nullptr; //pointer interator of meshes
	ComponentMesh* m = nullptr;
	GameObject* go = nullptr; 

	if (scene != nullptr && scene->HasMeshes())
	{
		
		for (uint i = 0; i < scene->mNumMeshes; i++) // Use scene->mNumMeshes to iterate on scene->mMeshes array
		{
			go = App->scene->CreateGameObject(go, "FBX"); 

			new_mesh = scene->mMeshes[i];
			m = dynamic_cast<ComponentMesh*>(go->CreateComponent(ComponentType::C_MESH)); 

			LOG("Mesh num: %u", i);

			m->LoadMeshVertices(new_mesh);

			if (new_mesh->HasFaces())
				m->LoadMeshIndices(new_mesh);
			
			if (new_mesh->HasNormals())
				m->LoadMeshNormals(new_mesh);

			if (new_mesh->GetNumColorChannels() > 0) 
			{
				for (int i = 0; i < new_mesh->GetNumColorChannels(); ++i)
				{
					if (new_mesh->HasVertexColors(i))
						m->LoadMeshColors(new_mesh, i);
				}
			}
			else LOG("No Color Channel detected");

			if (new_mesh->GetNumUVChannels() > 0) 
			{
				for (int i = 0; i < new_mesh->GetNumUVChannels(); ++i) 
				{
					if (new_mesh->HasTextureCoords(i))
						m->LoadMeshTextureCoords(new_mesh, i);
				}
			}
			else LOG("No UV Channel detected");

			m->CreateMeshBuffers(); 
			
			//App->renderer3D->PushMeshToRender(m);  // Push mesh into container of meshes
			//SaveMeshData(fbx_name, m);
		}

		
		aiReleaseImport(scene); // a tomar por culo el fbx 
		scene = nullptr;
	}
	else
		LOG("Error loading scene %s", fbx_name);
	return go;
}

bool ModuleFBXLoader::SaveMeshData(const char* fbx_name, ComponentMesh* mesh_data)
{
	std::string output_file;
	App->fs->GetNameFile(fbx_name, output_file);

	std::vector<uint> bytes;
	bytes.push_back(sizeof(float) * mesh_data->vertex.capacity * 3);
	bytes.push_back(sizeof(uint) * mesh_data->index.capacity);
	bytes.push_back(sizeof(float) * mesh_data->normals.capacity);

	uint mesh_bytes = bytes.at(0) + bytes.at(1) + bytes.at(2);

	char* buffer = new char[mesh_bytes];
	char* ptr = buffer;

	memset(ptr, *mesh_data->vertex.buffer, bytes.at(0));
	ptr += bytes.at(0);
	memset(ptr, *mesh_data->index.buffer, bytes.at(1));
	ptr += bytes.at(1);
	memset(ptr, *mesh_data->normals.buffer, bytes.at(2));


	std::string new_path(MESH_DIR);
	new_path.append("/"); new_path.append(output_file.data()); new_path.append(".meta");

	App->fs->WriteFile(new_path.c_str(), buffer, mesh_bytes);


	if (buffer != nullptr) {
		delete[] buffer;
		buffer = nullptr;
	}

	return false;
}
