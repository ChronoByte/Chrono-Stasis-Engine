#pragma once

#include "csGlobals.h"
#include "csModule.h"

#define LIBRARY_DIR "Library"
#define MESH_DIR "/Library/Meshes"
#define MATERIAL_DIR "/Library/Materials"

struct PHYSFS_File;


class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem();
	~ModuleFileSystem();
	bool ModuleFileSystem::Init(JSON_Object* node);
	bool Start();
	bool CleanUp();

public:
	bool AddPath(const char* path);
	bool FileExist(const char* file_name) const;

	PHYSFS_File* OpenFileWrite(const char* file_name) const;
	PHYSFS_File* OpenFileRead(const char* file_name) const;

	bool WriteFile(const char* file_name, char* buffer, uint32 size) const;
	bool ReadFile(const char* file_name, char* buffer);

	void CloseFile(PHYSFS_File* file, const char* file_name) const;
	bool GenerateDirectory(const char* dir_name);
	bool DeleteDirectory(const char* file_dir_name);
private:

};