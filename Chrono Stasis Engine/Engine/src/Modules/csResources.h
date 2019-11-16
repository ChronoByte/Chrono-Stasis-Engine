#pragma once

#include "csModule.h"
#include "csGlobals.h"
#include <map>

#include "Resource.h"
//class Resource;

class ModuleResources : public Module
{
public:
	ModuleResources(bool start_enabled = true);
	~ModuleResources();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	/*uint Find(const char* file) const;
	uint ImportFile(const char* file, Resource::Type type);
	uint GenerateNewUID();
	Resource* GetResource(uint uid);
	Resource* AddResource(Resource::Type type, uint forced_uid = 0);*/

private:
	std::map <uint, Resource* > resources;
};