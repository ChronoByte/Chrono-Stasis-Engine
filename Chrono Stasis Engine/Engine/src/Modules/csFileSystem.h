#pragma once

#include "csGlobals.h"
#include "csModule.h"
#include <array>
//#define LIBRARY_DIR "Library"
//#define MESH_DIR "/Library/Meshes"
//#define MATERIAL_DIR "/Library/Materials"

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

	void GetExtensionFile(const char* file, std::string& extension);
	void GetNameFile(const char* file, std::string& name);
	std::string GetDirectoryPath(const char* file); 
	std::string NormalizeSlashSymbol(const char* path);
	std::string GetFullPath(const char* path, const char* folder = nullptr, const char* extension = nullptr);
	void SplitPath(const char* full_path, std::string* path, std::string* filename, std::string* extension);
private:
	std::array<const char*, PATHS_AMOUNT> directories = {ASSETS_FOLDER,LIBRARY_FOLDER,SETTINGS_FOLDER,MESHES_FOLDER,TEXTURES_FOLDER,SCENES_FOLDER};

};