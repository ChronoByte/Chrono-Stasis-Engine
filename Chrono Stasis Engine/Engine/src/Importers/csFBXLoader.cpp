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

	LOG(" --- Creating FBX Importerer --- ");
	LOG("Initializing Assimp Library.");
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	LOG("Assimp Library Loaded Successfully.");
	LOG("Assimp Version: v%i.%i.%i", 3, 1, 1);
	return true;
}


bool ModuleFBXLoader::Start()
{
	return true;
}

update_status ModuleFBXLoader::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleFBXLoader::Update(float dt)
{

	/*if (App->input->dropped)
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
			
	}*/

	return UPDATE_CONTINUE;
}

update_status ModuleFBXLoader::PostUpdate(float dt)
{

	//if (App->input->dropped) 
	//{

	//	switch (type)
	//	{
	//	case MODEL:
	//	{
	//		//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "File dropped on window", App->input->file, App->window->window);

	//		App->fbx->LoadModel(App->input->file);
	//		break;
	//	}
	//	case TEXTURE:
	//	{
	//		//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "File dropped on window", App->input->file, App->window->window);
	//		GameObject* go = App->scene->GetSelected();

	//		if (go != nullptr && go != App->scene->GetRoot())
	//		{
	//			if (go->HasComponent(ComponentType::C_MATERIAL))
	//			{
	//				ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(go->FindComponent(ComponentType::C_MATERIAL));
	//				mat->SetTexture(App->texture->LoadTexture(App->input->file));
	//			}
	//			else
	//			{
	//				ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(go->CreateComponent(ComponentType::C_MATERIAL));
	//				if(mat!=nullptr)
	//					mat->SetTexture(App->texture->LoadTexture(App->input->file));
	//			}
	//		}
	//		else LOG("Error applying texture: There was not a valid Game Object selected."); 

	//		break;
	//	}
	//	case UNKNOWN:
	//	{
	//		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Unknown file dropped on window", App->input->file, App->window->window);
	//		break;
	//	}
	//	case NONE:
	//	{
	//		break;
	//	}

	//	default:
	//		break;
	//	}

	//	type = FileType::NONE;
	//	App->input->dropped = false;
	//	SDL_free(App->input->file);
	//}

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

		LOG("----------- Loading FBX Model: %s -----------", name.c_str());
		LOG("Path: %s", path);

		//SetBoundingBox(scene);
		
		filePath = App->fs->GetDirectoryPath(path); 
		NodePath(scene->mRootNode, scene);
		
		aiQuaternion quat_rotation;
		aiVector3D position;
		aiVector3D scale;

		scene->mRootNode->mTransformation.Decompose(scale, quat_rotation, position);
		Quat rot(quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);

		float3 euler_rotation = rot.ToEulerXYZ();

		// --------------------Transforms and BB 

		//bounding_box.box = AABB(bounding_box.min, bounding_box.max);

		newGo->GetTransform()->SetupTransform(math::float3(position.x, position.y, position.z), math::float3(scale.x, scale.y, scale.z), rot);
		//-----------------------------------
		
		
		LOG("----------- Ended Loading FBX Model: %s -----------", name.c_str());
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
		//textures[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]
		if (strcmp(fileName.C_Str(), "") != 0) 
		{
			std::string newPath = filePath + fileName.C_Str(); 
			ComponentMaterial* myMaterial = dynamic_cast<ComponentMaterial*>(go->CreateComponent(ComponentType::C_MATERIAL));

			// Try to find the texture in our vector of loaded textures so its not loaded twice 
			TextureInfo* texture = App->texture->FindLoadedTextureWithPath(newPath.c_str());
			if (texture != nullptr)
			{
				myMaterial->SetTexture(texture);
				LOG("Assigning texture already loaded in memory: %s", newPath.c_str());
			}
			else
			{
				myMaterial->SetTexture(App->texture->LoadTexture(newPath.c_str()));
			}

		}
		else LOG("FBX does not have a embedded Texture");

		// --------------- Set Up the Game Object Transform
		aiQuaternion quat_rotation;
		aiVector3D position;
		aiVector3D scale;
		
		node->mTransformation.Decompose(scale, quat_rotation, position);
		Quat rot(quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);

		// TODO: Check scale and rotation in loading fbx
		go->GetTransform()->SetupTransform(math::float3(position.x, position.y, position.z), math::float3(1, 1, 1), rot);		
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
	/*if (scene->mMeshes[0]->mNumVertices > 0)
	{
		boundingBox.min = float3(scene->mMeshes[0]->mVertices[0].x, scene->mMeshes[0]->mVertices[0].y, scene->mMeshes[0]->mVertices[0].z);
		boundingBox.max = float3(scene->mMeshes[0]->mVertices[0].x, scene->mMeshes[0]->mVertices[0].y, scene->mMeshes[0]->mVertices[0].z);
	}*/
}

void ModuleFBXLoader::GrowBoundingBox(aiMesh* mesh, float3* min, float3* max)
{
	for (uint i = 0; i < mesh->mNumVertices; i++)
	{
		float3 vec(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		*min = min->Min(vec);
		*max = max->Max(vec);
	}

}

//bool ModuleFBXLoader::SaveMeshData(const char* fbx_name, ComponentMesh* mesh_data)
//{
//	std::string output_file;
//	App->fs->GetNameFile(fbx_name, output_file);
//
//	std::vector<uint> bytes;
//	bytes.push_back(sizeof(float) * mesh_data->vertex.capacity * 3);
//	bytes.push_back(sizeof(uint) * mesh_data->index.capacity);
//	bytes.push_back(sizeof(float) * mesh_data->normals.capacity);
//
//	uint mesh_bytes = bytes.at(0) + bytes.at(1) + bytes.at(2);
//
//	char* buffer = new char[mesh_bytes];
//	char* ptr = buffer;
//
//	memset(ptr, *mesh_data->vertex.buffer, bytes.at(0));
//	ptr += bytes.at(0);
//	memset(ptr, *mesh_data->index.buffer, bytes.at(1));
//	ptr += bytes.at(1);
//	memset(ptr, *mesh_data->normals.buffer, bytes.at(2));
//
//
//	std::string new_path(L_MESHES_FOLDER);
//	new_path.append("/"); new_path.append(output_file.data()); new_path.append(".meta");
//
//	App->fs->WriteFile(new_path.c_str(), buffer, mesh_bytes);
//
//
//	if (buffer != nullptr) {
//		delete[] buffer;
//		buffer = nullptr;
//	}
//
//	return false;
//}

bool ModuleFBXLoader::Import(const char* assets_path, std::string& library_path, UID uid)
{
	const aiScene* scene = aiImportFile(assets_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		std::string name;
		App->fs->GetNameFile(assets_path, name);
		newGo = App->scene->CreateGameObject(nullptr, name.c_str(), true);

		LOG("----------- Importing FBX Model: %s -----------", name.c_str());
		LOG("Path: %s", assets_path);

		//SetBoundingBox(scene);

		filePath = App->fs->GetDirectoryPath(assets_path);
		NodePath(scene->mRootNode, scene, library_path, assets_path, uid);


		aiQuaternion quat_rotation;
		aiVector3D position;
		aiVector3D scale;

		scene->mRootNode->mTransformation.Decompose(scale, quat_rotation, position);
		Quat rot(quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);

		float3 euler_rotation = rot.ToEulerXYZ();

		// --------------------Transforms and BB 

		//bounding_box.box = AABB(bounding_box.min, bounding_box.max);

		newGo->GetTransform()->SetupTransform(math::float3(position.x, position.y, position.z), math::float3(scale.x, scale.y, scale.z), rot);
		//-----------------------------------
		
		App->serialization->SaveModel(*newGo, A_MODELS_FOLDER, assets_path);
		
		//TODO: GO needs to be deleted after being serialized into .meta fbx
		//TODO: NewGo->DeleteGO();
		
		LOG("----------- Ended Importing FBX Model: %s -----------", name.c_str());
		aiReleaseImport(scene);

		return true;
	}
	else
	{
		LOG("Error: FBX format not valid.");
		return false;
	};
}

void ModuleFBXLoader::NodePath(aiNode* node, const aiScene* scene, std::string& library_path, std::string assets_path, UID uid)
{
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		// --------------- A GameObject son for each mesh
		GameObject* go = App->scene->CreateGameObject(newGo, node->mName.C_Str());

		// Create and assign Component Mesh 
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		go->AssignComponent(ImportMesh(mesh, scene, library_path, uid, node->mName.C_Str(), assets_path.c_str()));

		// --------------- Set Up Texture - Component Material
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiString fileName;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &fileName);
		//textures[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]
		if (strcmp(fileName.C_Str(), "") != 0)
		{
			filePath = A_TEXTURES_FOLDER;
			std::string newPath = filePath + fileName.C_Str();
			ComponentMaterial* myMaterial = dynamic_cast<ComponentMaterial*>(go->CreateComponent(ComponentType::C_MATERIAL));

			/*std::string name;
			App->fs->GetNameFile(fileName.C_Str(), name);*/
			
			ResourceTexture* resMat = (ResourceTexture*)App->resources->GetResourceFromFile(newPath.c_str());
			//App->resources->GetResourcesFromType();
			if (resMat)
			{
				UID uuid = 0u;
				uuid = resMat->GetUID();
				myMaterial->AssignResource(uuid);
				LOG("Assigning texture already imported: %s", newPath.c_str());
			}
			else 
			{
				UID uuid = 0u;
				uuid = App->resources->ImportFile(newPath.c_str(), Resource::R_TEXTURE, uuid);
				myMaterial->AssignResource(uuid);
				LOG("Assigning texture embbeded: %s", newPath.c_str());
			}
			
		}
		else LOG("FBX does not have a embedded Texture");

		// --------------- Set Up the Game Object Transform
		aiQuaternion quat_rotation;
		aiVector3D position;
		aiVector3D scale;

		node->mTransformation.Decompose(scale, quat_rotation, position);
		Quat rot(quat_rotation.x, quat_rotation.y, quat_rotation.z, quat_rotation.w);

		// TODO: Check scale and rotation in loading fbx
		go->GetTransform()->SetupTransform(math::float3(position.x, position.y, position.z), math::float3(1, 1, 1), rot);
	}

	for (uint i = 0; i < node->mNumChildren; i++)
	{
		NodePath(node->mChildren[i], scene, library_path, assets_path, uid);
	}

}

ComponentMesh* ModuleFBXLoader::ImportMesh(aiMesh* mesh, const aiScene* scene,std::string& library_path, UID uuid, const char* name, const char* assets_path)
{

	//GENERATE UUID ---------------------------
	if (uuid == 0u)
		uuid = GenerateUUID();

	//RESOURCE MESH ---------------------------
	ResourceMesh* res = (ResourceMesh*)App->resources->CreateNewResource(Resource::Type::R_MESH, uuid);

	res->LoadMeshVertices(mesh);

	if (mesh->HasFaces())
		res->LoadMeshIndices(mesh);

	if (mesh->HasNormals())
		res->LoadMeshNormals(mesh);

	if (mesh->GetNumColorChannels() > 0)
	{
		if (mesh->HasVertexColors(0))
			res->LoadMeshColors(mesh, 0);
	}
	else LOG("No Color Channel detected");

	if (mesh->GetNumUVChannels() > 0)
	{
		if (mesh->HasTextureCoords(0))
			res->LoadMeshTextureCoords(mesh, 0);
	}
	else LOG("No UV Channel detected");


	// OWN FORMAT FILE ------------------------
	std::string own_file;
	SaveMesh(res, uuid, own_file);

	// RESOURCE MESH ------------------------
	res->SetName(name);
	res->SetFile(assets_path);
	res->SetExportedFile(own_file.c_str());
	
	//COMPONENT MESH ---------------------------
	ComponentMesh* m = new ComponentMesh(nullptr);
	m->AssignResource(uuid);

	return m;
}

void ModuleFBXLoader::SaveMesh(ResourceMesh* res, UID uuid, std::string& library_path)
{
	uint ranges[3] = { res->GetVertices(), res->GetIndices(), res->GetNormals() };
	uint size = sizeof(ranges) + sizeof(float) * res->GetVertices() + sizeof(uint) * res->GetIndices() + sizeof(float) * res->GetNormals() + sizeof(float) * res->GetVertices();

	// Allocating all data 
	char* data = new char[size];
	char* cursor = data;

	// Storing Ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);

	// Storing Vertices
	cursor += bytes;
	bytes = sizeof(float) * res->GetVertices();
	memcpy(cursor, res->vertex.buffer, bytes);

	// Storing Indices
	cursor += bytes;
	bytes = sizeof(uint) * res->GetIndices();
	memcpy(cursor, res->index.buffer, bytes);

	// Storing Normals
	cursor += bytes;
	bytes = sizeof(float) * res->GetNormals();
	memcpy(cursor, res->normals.buffer, bytes);

	// Storing Tex Coords
	cursor += bytes;
	bytes = sizeof(float) * res->GetVertices(); //num_tex_coords;
	memcpy(cursor, res->textureCoords.buffer, bytes);

	// Release all pointers
	RELEASE_ARRAY(res->vertex.buffer);
	RELEASE_ARRAY(res->index.buffer);
	RELEASE_ARRAY(res->normals.buffer);
	RELEASE_ARRAY(res->textureCoords.buffer);

	//Create Own format mesh file 
	std::string output_file(L_MESHES_FOLDER + std::to_string(uuid) + MESH_EXTENSION);
	App->fs->WriteFile(output_file.c_str(), (char*)data, size);
	library_path = output_file;
	LOG("Own format Mesh file exported successfully at [%s]", library_path);

	RELEASE_ARRAY(data);
}
