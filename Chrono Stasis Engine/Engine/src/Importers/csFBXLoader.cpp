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
		
		delete newGo; 
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
			std::string extension; 
			std::string name; 
			App->fs->GetNameFile(fileName.C_Str(), name); 
			App->fs->GetExtensionFile(fileName.C_Str(), extension); 
			filePath = A_TEXTURES_FOLDER;
			std::string newPath = filePath + name + extension; 
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
	uint ranges[6] = { res->GetIndicesSize(), res->GetVerticesSize(), res->GetNormalsSize(), res->GetTextureCoordsSize(), res->GetFaceNormalsSize(), res->GetVertexNormalsSize() };

	uint size_indices = sizeof(uint) * res->GetIndicesSize();
	uint size_vertices = sizeof(float) * (res->GetVerticesSize());
	uint size_normals = sizeof(float) * (res->GetNormalsSize());
	uint size_texCoords = sizeof(float) * (res->GetTextureCoordsSize());
	uint size_faceN = sizeof(float) * (res->GetFaceNormalsSize());
	uint size_vertexN = sizeof(float) * (res->GetVertexNormalsSize());


	//uint size = sizeof(ranges) + sizeof(float) * res->GetVertices() + sizeof(uint) * res->GetIndices() + sizeof(float) * res->GetNormals() + sizeof(float) * res->GetTextureCoords();
	uint size = sizeof(ranges) + size_indices + size_vertices + size_normals + size_texCoords + size_faceN + size_vertexN;

	// Allocating all data 
	char* data = new char[size];
	char* cursor = data;

	// Storing Ranges
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);

	// Storing Indices
	cursor += bytes;
	bytes = size_indices;
	memcpy(cursor, res->index.buffer, bytes);

	// Storing Vertices
	cursor += bytes;
	bytes = size_vertices;
	memcpy(cursor, res->vertex.buffer, bytes);
	LOG("Saving %i space for vertices", bytes); 
	// Storing Normals
	cursor += bytes;
	bytes = size_normals;
	memcpy(cursor, res->normals.buffer, bytes);

	// Storing Tex Coords
	cursor += bytes;
	bytes = size_texCoords; 
	memcpy(cursor, res->textureCoords.buffer, bytes);

	// Storing Face Normals
	cursor += bytes;
	bytes = size_faceN;
	memcpy(cursor, res->faceNormals.buffer, bytes);

	// Storing Vertex Normals
	cursor += bytes;
	bytes = size_vertexN;
	memcpy(cursor, res->vertexNormals.buffer, bytes);

	
	// Release all pointers
	RELEASE_ARRAY(res->vertex.buffer);
	RELEASE_ARRAY(res->index.buffer);
	RELEASE_ARRAY(res->normals.buffer);
	RELEASE_ARRAY(res->colors.buffer);
	RELEASE_ARRAY(res->textureCoords.buffer);
	RELEASE_ARRAY(res->faceNormals.buffer);
	RELEASE_ARRAY(res->vertexNormals.buffer);

	//Create Own format mesh file 
	std::string output_file(L_MESHES_FOLDER + std::to_string(uuid) + MESH_EXTENSION);
	App->fs->WriteFile(output_file.c_str(), (char*)data, size);
	library_path = output_file;
	LOG("Own format Mesh file exported successfully at [%s]", library_path);

	RELEASE_ARRAY(data);
}
