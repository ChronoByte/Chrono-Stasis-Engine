#include "csApp.h"
#include "csResources.h"
#include "ResourceTexture.h"

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

uint ModuleResources::ImportFile(const char* assets_file, Resource::Type type)
{
	uint ret = 0;
	bool import_ok = false; 
	std::string written_file; // Own format file
	UID uuid = 0; 
	switch (type) {
		case Resource::R_TEXTURE: import_ok = App->texture->Import(assets_file, written_file, uuid); break; // Create Own format file
		case Resource::R_MESH: import_ok = false; break;
		case Resource::R_SCENE: import_ok = false; break;
	}

	// Create Resource
	if (import_ok == true) { 
		Resource* res = CreateNewResource(type);
		res->file = assets_file;
		res->exported_file = written_file;
		ret = res->uid;
		
		//Create .meta file
		CreateNewMeta(res, assets_file); 
	}


	return ret;
}

Resource* ModuleResources::CreateNewResource(Resource::Type type, uint force_uid)
{
	Resource* ret = nullptr;
	uint uid = GenerateUUID(); //Temporal

	switch (type) {
		case Resource::R_TEXTURE: ret = (Resource*) new ResourceTexture(uid,type); break;
		case Resource::R_MESH: ret = false; break;
		case Resource::R_SCENE: ret = false; break;
		
	}

	if (ret != nullptr)
		resources[uid] = ret;

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

	std::string meta_file = resource->imported_file;
	meta_file.append(META_EXTENSION);

	json_serialize_to_file(json_file, meta_file.c_str());

	json_free_serialized_string(serialized_string);
	json_value_free(json_file);



}
