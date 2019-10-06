#include "csApp.h"
#include "csFileSystem.h"

#include "PhysFS\include\physfs.h"
#pragma comment( lib, "Engine/Dependencies/PhysFS/libx86/physfs.lib" )

ModuleFileSystem::ModuleFileSystem()
{
	name = "FileSystem";
}

ModuleFileSystem::~ModuleFileSystem()
{
}

bool ModuleFileSystem::Init(JSON_Object* node)
{
	LOG("Initializing PhysFS library...");

	char* app_path = SDL_GetBasePath();
	
	PHYSFS_init(app_path);
	SDL_free(app_path);

	LOG("Game Folder Directory pointed...");
	PHYSFS_setWriteDir(".");
	return true;
}

bool ModuleFileSystem::Start()
{
	return true;
}

bool ModuleFileSystem::CleanUp()
{
	return true;
}