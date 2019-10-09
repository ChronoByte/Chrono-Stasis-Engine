#include "csApp.h"
#include "csTextureLoader.h"

#include "DevIL\include\ilut.h"
#include "DevIL\include\il.h"
#include "DevIL\include\ilu.h"

#pragma comment (lib, "Engine/Dependencies/DevIL/libx86/DevIL.lib") 
#pragma comment (lib, "Engine/Dependencies/DevIL/libx86/ILU.lib")
#pragma comment (lib, "Engine/Dependencies/DevIL/libx86/ILUT.lib")

ModuleTextureLoader::ModuleTextureLoader(bool start_enabled)
{
}

ModuleTextureLoader::~ModuleTextureLoader()
{
}

bool ModuleTextureLoader::Init(JSON_Object* node)
{
	return true;
}

bool ModuleTextureLoader::Start()
{
	return true;
}

update_status ModuleTextureLoader::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleTextureLoader::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleTextureLoader::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleTextureLoader::CleanUp()
{
	return true;
}
