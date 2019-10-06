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
	//EXIST FILE TEST
	bool test1 = false;
	test1 = FileExist("test.txt");
	LOG("FILESYSTEM: txt: %d", test1);

	//WRITING FILE TEST
	char* text = "Tonto quien lo lea (te avise...)";
	WriteFile("No lo leas.txt", text, strlen(text));

	//READING FILE TEST
	char* text2 = new char[20];
	ReadFile("No lo leas.txt", text2);
	LOG("%s", text2);
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

bool ModuleFileSystem::FileExist(const char* file_name) const
{
	bool ret = false;

	if (PHYSFS_exists(file_name) != 0)
		ret = true;

	return ret;
}

PHYSFS_File* ModuleFileSystem::OpenFileWrite(const char* file_name) const
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

	
	return file;
} 	

PHYSFS_File* ModuleFileSystem::OpenFileRead(const char* file_name) const
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

	
	return file;

}

void ModuleFileSystem::CloseFile(PHYSFS_File* file, const char* file_name) const
{
	if (PHYSFS_close(file) != 0)
	{
		LOG("FILESYSTEM: success on closing file %s", file_name);
	}
	else
		LOG("FILESYSTEM: could not close file %s: %s", file_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
}

bool ModuleFileSystem::WriteFile(const char* file_name, char* buffer, uint32 size) const
{
	bool ret = false;

	PHYSFS_File* file = OpenFileWrite(file_name);

	if (file && PHYSFS_writeBytes(file, (const void*)buffer, size) >= size) 
	{
		ret = true;
		LOG("FILESYSTEM: success on writting in file [ %s ]", file_name);
	}
	else
		LOG("FILESYSTEM: failure on writting in file [ %s ]: %s", file_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	

	CloseFile(file, file_name);

	return ret;
}

bool ModuleFileSystem::ReadFile(const char* file_name, char* buffer)
{
	bool ret = false;

	PHYSFS_File* file = OpenFileRead(file_name);

	if (file && PHYSFS_readBytes(file, buffer, PHYSFS_fileLength(file)) != -1)
	{
		ret = true;
		LOG("FILESYSTEM: Success on read file [%s]", file_name);
	}
		
	else
		LOG("FILESYSTEM: Could not read file [%s]: %s", file_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	

	CloseFile(file, file_name);

	return ret;
}


bool ModuleFileSystem::GenerateDirectory(const char* dir_name)
{
	bool ret = false;

	if (PHYSFS_mkdir(dir_name) != 0)
	{
		ret = true;
		LOG("FILESYSTEM: Directory [%s] created!", dir_name);
	}
	else
		LOG("FILESYSTEM: could not create directory [%s]: %s", dir_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	

	return ret;
}

bool ModuleFileSystem::DeleteDirectory(const char* file_dir_name)
{
	bool ret = false;

	if (PHYSFS_delete(file_dir_name) != 0) {
		ret = true;
		LOG("FILESYSTEM: Directory [%s] deleted!", file_dir_name);
	}
	else
		LOG("FILESYSTEM: could not delete [%s] directory: %s", file_dir_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	

	return ret;
}