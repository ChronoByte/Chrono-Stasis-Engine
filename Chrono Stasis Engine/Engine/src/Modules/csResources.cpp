#include "csApp.h"
#include "csResources.h"

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
