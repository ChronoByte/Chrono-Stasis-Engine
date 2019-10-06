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
	bool ret = false;
	LOG("FILESYSTEM: Initializing PhysFS library...");

	char* app_path = SDL_GetBasePath();
	
	if (PHYSFS_init(app_path) != 0)
	{
		ret = true;
		LOG("FILESYSTEM: PhysFS successfully initialized...");
	}
	else
		LOG("FILESYSTEM: error while initaliazing PHYSFS: %s", PHYSFS_getLastError());

	SDL_free(app_path);

	LOG("FILESYSTEM: Setting Up directories");
	AddPath(".");
	AddPath("../Game");

	PHYSFS_setWriteDir(".");

	return ret;
}

bool ModuleFileSystem::Start()
{
	bool test1 = false;
	test1 = FileExist("test.txt");
	LOG("FILESYSTEM: txt: %d", test1);
	return true;
}

bool ModuleFileSystem::CleanUp()
{
	bool ret = false;

	LOG("FILESYSTEM: Deinitializing PHYSFS...");

	if (PHYSFS_deinit() != 0)
	{
		ret = true;
		LOG("FILESYSTEM: PhysFS successfully deinitialized...");
	}
	else
		LOG("FILESYSTEM: error while deinitaliazing PHYSFS: %s", PHYSFS_getLastError());

	return ret;
}

bool ModuleFileSystem::AddPath(const char* path)
{
	bool ret = false;

	if (PHYSFS_mount(path, nullptr, 1) != 0)
	{
		ret = true;
		LOG("FILESYSTEM: Directory ( %s ) successfully added to the search path!", path);
	}
	else
		LOG("FILESYSTEM: could not add Directory ( %s ) to the search path: %s",path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

	return ret;
}

bool ModuleFileSystem::FileExist(char* file_name) const
{
	bool ret = false;

	if (PHYSFS_exists(file_name) != 0)
		ret = true;

	return ret;
}

bool ModuleFileSystem::OpenFileWrite(char* file_name) const
{
	bool ret = false;
	PHYSFS_File* file = PHYSFS_openWrite(file_name);

	if (file != nullptr)
	{
		ret = true;
		LOG("FILESYSTEM: file %s successfully opened for writing.", file_name);
	}
	else
		LOG("FILESYSTEM: could not open file %s for writting: %s", file_name, PHYSFS_getLastErrorCode());

	PHYSFS_close(file);
	return ret;		
} 	

bool ModuleFileSystem::OpenFileRead(char* file_name) const
{
	bool ret = false;
	PHYSFS_File* file = PHYSFS_openRead(file_name);

	if (file != nullptr)
	{
		ret = true;
		LOG("FILESYSTEM: file %s successfully opened for reading.", file_name);
	}
	else
		LOG("FILESYSTEM: could not open file %s for reading: %s", file_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

	PHYSFS_close(file);
	return ret;

}