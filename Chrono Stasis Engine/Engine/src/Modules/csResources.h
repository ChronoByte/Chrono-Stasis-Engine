#pragma once

#include "csModule.h"
#include "csGlobals.h"
#include <map>

#include "Resource.h"
#include "PseudoRandom.h"
//class Resource;

class ModuleResources : public Module
{
	

public:
	ModuleResources(bool start_enabled = true);
	~ModuleResources();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	uint ImportFile(const char* file, Resource::Type type, UID uuid_to_force = 0u);
	Resource* CreateNewResource(Resource::Type type, UID force_uid = 0u, const char* name = nullptr, const char* file = nullptr, const char* exported_file = nullptr, bool load = false);
	void CreateNewMeta(Resource* resource, const char* file);
	Resource* GetResource(UID uid);
	Resource* FindUID(UID uid);
	bool DeleteResourceFromUID(UID uid);
	std::vector<Resource*> GetResourcesFromType(Resource::Type type);
	Resource* GetResourceFromName(const char* texture);
	//uint Find(const char* file) const;
	
	

private:
	std::map <UID, Resource* > resources;
};