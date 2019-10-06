#include "csFileSystem.h"

#include "PhysFS\include\physfs.h"
#pragma comment( lib, "Engine/Dependencies/PhysFS/libx86/physfs.lib" )

ModuleFileSystem::ModuleFileSystem()
{
}

ModuleFileSystem::~ModuleFileSystem()
{
}

bool ModuleFileSystem::Start()
{
	return true;
}

bool ModuleFileSystem::CleanUp()
{
	return true;
}