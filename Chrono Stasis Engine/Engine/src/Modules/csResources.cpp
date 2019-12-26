#include "csApp.h"
#include "csFileSystem.h"
#include "csTextureLoader.h"
#include "csFBXLoader.h"

#include "csResources.h"
#include "ResourceTexture.h"
#include "ResourceMesh.h"

ModuleResources::ModuleResources(bool start_enabled)
{
}

ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Start()
{
	return true;
}

update_status ModuleResources::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
	return true;
}

uint ModuleResources::ImportFile(const char* assets_file, Resource::Type type, UID uuid_to_force)
{
	uint ret = 0;
	bool import_ok = false; 
	std::string written_file; // Own format file

	std::string asset_name;
	App->fs->GetNameFile(assets_file, asset_name);
	//UID uuid = 0; 
	switch (type) {
		case Resource::R_TEXTURE: import_ok = App->texture->Import(assets_file, written_file, uuid_to_force); break; // Create Own format file
		case Resource::R_MESH: import_ok = App->fbx->Import(assets_file, written_file, uuid_to_force); break;
		case Resource::R_SCENE: import_ok = false; break;
	}

	// Create Resource
	if (import_ok == true && type == Resource::R_TEXTURE) { 
		Resource* res = CreateNewResource(type, uuid_to_force);
		res->uid = uuid_to_force;
		res->name = asset_name;
		res->file = assets_file;
		res->exported_file = written_file;
		res->type = type;
		ret = res->uid;
		LOG("Resource file created successfully from: [%s]", assets_file);

		//Create .meta file
		CreateNewMeta(res, assets_file); 
	}


	return ret;
}

Resource* ModuleResources::CreateNewResource(Resource::Type type, UID force_uid, const char* name, const char* file, const char* exported_file, bool load)
{
	Resource* ret = nullptr;

	if(force_uid == 0)
		force_uid = GenerateUUID();
	else {
		if (FindUID(force_uid) != nullptr)
			return FindUID(force_uid);
	}

	switch (type) {
		case Resource::R_TEXTURE: ret = (Resource*) new ResourceTexture(force_uid,type); break;
		case Resource::R_MESH: ret = (Resource*) new ResourceMesh(force_uid, type); break;
		case Resource::R_SCENE: ret = false; break;
		
	}

	if (load)
	{
		ret->uid = force_uid;
		ret->name = name;
		ret->file = file;
		ret->exported_file = exported_file;
		ret->type = type;
	}

	if (ret != nullptr)
		resources[force_uid] = ret;
	
	return ret;
}

void ModuleResources::CreateNewMeta(Resource* resource, const char* file)
{
	JSON_Value* json_file = nullptr;
	JSON_Object* root_obj = nullptr;

	//Generate value
	json_file = json_value_init_object();

	//Create file
	root_obj = json_value_get_object(json_file);

	// Setting main meta properties
	json_object_set_number(root_obj, "UUID", resource->uid);
	json_object_set_string(root_obj, "UUID_path", resource->exported_file.c_str());
	json_object_set_number(root_obj, "LastChange", App->fs->GetLastModTime(file));
	
	// TODO: Add import settings


	// Serialization to meta file
	char* serialized_string = NULL;
	serialized_string = json_serialize_to_string_pretty(json_file);
	puts(serialized_string);

	std::string meta_file = resource->file;
	meta_file.append(META_EXTENSION);
	

	json_serialize_to_file(json_file, meta_file.c_str());

	json_free_serialized_string(serialized_string);
	json_value_free(json_file);

	LOG(".META File exported successfully at [%s]", meta_file);


}

Resource* ModuleResources::GetResource(UID uid)
{
	std::map <UID, Resource* >::iterator it = resources.find(uid);

	if (resources.find(uid) != resources.end())
	{
		return resources[uid];
	}

	return nullptr;
}

Resource* ModuleResources::FindUID(UID uid)
{
	std::map<UID, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
		return it->second;
	return nullptr;
}

bool ModuleResources::DeleteResourceFromUID(UID uid)
{
	if (resources.find(uid) != resources.end())
	{
		Resource* res = resources[uid];
		resources.erase(uid);
		RELEASE(res);

		return true;
	}
	return false;
}

std::vector<Resource*> ModuleResources::GetResourcesFromType(Resource::Type type)
{
	std::vector<Resource*> ret;
	auto i = resources.begin();

	for (auto res = resources.begin(); res != resources.end(); res++)
	{
		if ((*res).second->GetType() == type)
			ret.push_back((*res).second);
	}

	return ret;
}

Resource* ModuleResources::GetResourceFromName(const char* texture)
{
	//std::map<uint, Resource*>::iterator it = resources.begin();

	auto it = resources.begin();
	for (int i = 0; i < resources.size(); i++)
	{
		if (strcmp(it->second->name.c_str(), texture) == 0)
		{
			return it->second;
		}
		it++;
	}
	return nullptr;
}

Resource* ModuleResources::GetResourceFromFile(const char* file)
{
	//std::map<uint, Resource*>::iterator it = resources.begin();

	auto it = resources.begin();
	for (int i = 0; i < resources.size(); i++)
	{
		if (strcmp(it->second->file.c_str(), file) == 0)
		{
			return it->second;
		}
		it++;
	}
	return nullptr;
}
