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
	std::string written_file;

	switch (type) {
		case Resource::R_TEXTURE: import_ok = App->texture->Import(assets_file, written_file); break;
		case Resource::R_MESH: import_ok = false; break;
		case Resource::R_SCENE: import_ok = false; break;
	}

	if (import_ok == true) { // If import was successful, create a new resource
		Resource* res = CreateNewResource(type);
		res->file = assets_file;
		res->exported_file = written_file;
		ret = res->uid;
		
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
