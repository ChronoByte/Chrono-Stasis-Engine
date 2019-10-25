#include "csApp.h"
#include "csFBXLoader.h"
#include "csFileSystem.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#include "src/Structure/HierarchyWindow.h"

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

	if (App->input->dropped)
	{
		std::string extension;
		std::string file = App->input->file;
		App->fs->GetExtensionFile(file.c_str(), extension);

		type = FileType::UNKNOWN;
		
		if (!extension.compare(".fbx") || !extension.compare(".FBX"))
		{
			type = FileType::MODEL;
		}

		if (!extension.compare(".png") || !extension.compare(".PNG") || !extension.compare(".tga") || 
			!extension.compare(".TGA") || !extension.compare(".dds") || 
			!extension.compare(".jpg") || !extension.compare(".JPG"))
		{
			type = FileType::TEXTURE;
		}
			
	}

	return UPDATE_CONTINUE;
}

update_status ModuleFBXLoader::PostUpdate(float dt)
{

	if (App->input->dropped) 
	{

		switch (type)
		{
		case MODEL:
		{
			//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "File dropped on window", App->input->file, App->window->window);

			App->fbx->LoadModel(App->input->file);
			break;
		}
		case TEXTURE:
		{
			//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "File dropped on window", App->input->file, App->window->window);
			GameObject* go = App->editor->hierarchy->GetSelected();

			if (go != nullptr && go != App->scene->GetRoot())
			{
				if (go->HasComponent(ComponentType::C_MATERIAL))
				{
					ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(go->FindComponent(ComponentType::C_MATERIAL));
					mat->SetTexture(App->texture->LoadTexture(App->input->file));
				}
				else
				{
					ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(go->CreateComponent(ComponentType::C_MATERIAL));
					if(mat!=nullptr)
						mat->SetTexture(App->texture->LoadTexture(App->input->file));
				}
			}
			else LOG("Error applying texture: There was not a valid Game Object selected."); 

			break;
		}
		case UNKNOWN:
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Unknown file dropped on window", App->input->file, App->window->window);
			break;
		}
		case NONE:
		{
			break;
		}

		default:
			break;
		}

		type = FileType::NONE;
		App->input->dropped = false;
		SDL_free(App->input->file);
	}

	return UPDATE_CONTINUE;
}

bool ModuleFBXLoader::CleanUp()
{
	

	aiDetachAllLogStreams();
	
	return true;
}

void ModuleFBXLoader::FBXModelImport(const char* fbx_name)
{
	GameObject* newGameObject = LoadModel(fbx_name);

}

GameObject* ModuleFBXLoader::LoadModel(const char* path)
{

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		std::string name;
		App->fs->GetNameFile(path, name);
		newGo = App->scene->CreateGameObject(nullptr, name.c_str());

		LOG("-----------------------------")
		LOG("Loading FBX Model with path: %s", path);
		SetBoundingBox(scene);
		
		filePath = App->fs->GetDirectoryPath(path); 
		NodePath(scene->mRootNode, scene);


		aiQuaternion quat_rotation;
		aiVector3D position;
		aiVector3D scale;

		scene->mRootNode->mTransformation.Decompose(scale, quat_rotation, position);
		Quat rot(quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);

		float3 euler_rotation = rot.ToEulerXYZ();

		/*model->transform[0].Set(position.x, position.y, position.z);
		model->transform[1].Set(euler_rotation.x, euler_rotation.y, euler_rotation.z);
		model->transform[2].Set(scale.x, scale.y, scale.z);*/
		bounding_box.box = AABB(bounding_box.min, bounding_box.max);

		newGo->GetTransform()->SetupTransform(math::float3(position.x, position.y, position.z), math::float3(scale.x, scale.y, scale.z), rot);
		newGo->GetTransform()->SetBoundingBox(bounding_box.box);

		//-----------------------------------
		
		
		LOG("-----------------------------");

		aiReleaseImport(scene);

		return newGo;
	}
	else
	{
		LOG("Error: FBX format not valid.");
		return nullptr;
	}
}

void ModuleFBXLoader::NodePath(aiNode* node, const aiScene* scene)
{

	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		// --------------- A GameObject son for each mesh
		GameObject* go = App->scene->CreateGameObject(newGo, node->mName.C_Str()); 

		// Create and assign Component Mesh 
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		go->AssignComponent(LoadMesh(mesh, scene));
		

		// --------------- Set Up Texture - Component Material
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiString fileName;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &fileName);

		if (strcmp(fileName.C_Str(), "") != 0) 
		{
			std::string newPath = filePath + fileName.C_Str(); 
			ComponentMaterial* myMaterial = dynamic_cast<ComponentMaterial*>(go->CreateComponent(ComponentType::C_MATERIAL));
			myMaterial->SetTexture(App->texture->LoadTexture(newPath.c_str()));	
		}
		else LOG("FBX does not have a embedded Texture");
		// TODO: Get the path correctly. 
		// TODO: Save created textures, so no need to load multiple times same texture
		// TODO: Ugly Code 
		// TODO: Safety

		// --------------- Set Up the Game Object Transform
		aiQuaternion quat_rotation;
		aiVector3D position;
		aiVector3D scale;
		
		node->mTransformation.Decompose(scale, quat_rotation, position);
		Quat rot(quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);

		go->GetTransform()->SetupTransform(math::float3(position.x, position.y, position.z), math::float3(scale.x, scale.y, scale.z), rot);		
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
	

	UpdateBoundingBox(mesh, &bounding_box.min, &bounding_box.max);

	m->CreateMeshBuffers();

	return m;
}


GameObject* ModuleFBXLoader::LoadFBXData(const char* fbx_name)
{
	bool ret = false;
	const aiScene* scene = aiImportFile(fbx_name, aiProcessPreset_TargetRealtime_MaxQuality); //container of meshes
	
	aiMesh* new_mesh = nullptr; //pointer interator of meshes
	ComponentMesh* m = nullptr;
	GameObject* parent = nullptr; 


	if (scene != nullptr && scene->HasMeshes())
	{

		parent = App->scene->CreateGameObject(nullptr, fbx_name);

		for (uint i = 0; i < scene->mNumMeshes; i++) // Use scene->mNumMeshes to iterate on scene->mMeshes array
		{
			GameObject* go = App->scene->CreateGameObject(parent, "Meshx"); 

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
	return parent;
}

void ModuleFBXLoader::SetBoundingBox(const aiScene* scene)
{
	if (scene->mMeshes[0]->mNumVertices > 0)
	{
		bounding_box.min = float3(scene->mMeshes[0]->mVertices[0].x, scene->mMeshes[0]->mVertices[0].y, scene->mMeshes[0]->mVertices[0].z);
		bounding_box.max = float3(scene->mMeshes[0]->mVertices[0].x, scene->mMeshes[0]->mVertices[0].y, scene->mMeshes[0]->mVertices[0].z);
	}
}

void ModuleFBXLoader::UpdateBoundingBox(aiMesh* mesh, float3* min, float3* max)
{
	for (uint i = 0; i < mesh->mNumVertices; i++)
	{
		float3 vec(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		*min = min->Min(vec);
		*max = max->Max(vec);
	}

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
