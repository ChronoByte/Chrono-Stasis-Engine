#pragma once

#include "csGlobals.h"
#include "csModule.h"
#include <array>
#include "../Structure/FileBrowserWindow.h"

#include "FileManager.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "MathGeoLib/include/Math/float2.h"
#include "MathGeoLib/include/Math/float4.h"

#include "JSON/parson.h"
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
	update_status Update(float dt);
	bool CleanUp();

public:
	bool AddPath(const char* path);
	bool FileExist(const char* file_name) const;

	PHYSFS_File* OpenFileWrite(const char* file_name) const;
	PHYSFS_File* OpenFileRead(const char* file_name) const;

	bool WriteFile(const char* file_name, char* buffer, uint32 size) const;
	uint ReadFile(const char* file_name, char** buffer);

	void CloseFile(PHYSFS_File* file, const char* file_name) const;
	bool GenerateDirectory(const char* dir_name);
	bool DeleteDirectory(const char* file_dir_name);

	void GetExtensionFile(const char* file, std::string& extension);
	void GetJSONExtensionFile(const char* file, std::string& extension);
	void GetNameFile(const char* file, std::string& name);
	std::string GetDirectoryPath(const char* file); 
	std::string NormalizeSlashSymbol(const char* path);
	
	JSON_Status json_array_dotset_float3(JSON_Object* object, const char* name, float3 transform);
	JSON_Status json_array_dotset_float2(JSON_Object* object, const char* name, float2 transform);
	JSON_Status json_array_dotset_float4(JSON_Object* object, const char* name, float4 transform);
	float3 json_array_dotget_float3_string(const JSON_Object* object, const char* name);
	float2 json_array_dotget_float2_string(const JSON_Object* object, const char* name);
	float4 json_array_dotget_float4_string(const JSON_Object* object, const char* name);
	
	void GetStorageResources(const char* path, std::list<StorageUnit*>& storage, const char* desiredExtension, const char* metaExtension = nullptr, ExtensionType type = ExtensionType::NONE_EXTENSION);
	bool CopyToAssets(const char* src_file_path);
	void PushFilesRecursively(const char* folder_name);
	int GetLastModTime(const char* path) const;
	void LogAssetsInfo(Folder* root);
	void ImportFilesRecursively(Folder* root, bool start = false);

	void DeleteMetaFiles(Folder* root);
	bool DeleteFiles(const char* path);
	void DeleteOwnFormatFiles(Folder* root);

	void SearchLibraryFolders();
	Folder* GetAssetsFolder() const;

	bool CheckDroppedFile(const char* dropped_file);
	void RefreshFiles();
	void CleanAssets(Folder* root);

private:


	std::array<const char*, PATHS_AMOUNT> directories = {ASSETS_FOLDER,LIBRARY_FOLDER,SETTINGS_FOLDER,L_MESHES_FOLDER,L_TEXTURES_FOLDER,L_SCENES_FOLDER};
	std::array<const char*, 4> library_directories = { LIBRARY_FOLDER, L_MESHES_FOLDER,L_TEXTURES_FOLDER,L_SCENES_FOLDER };
	Folder* assets = nullptr;
	Timer refresh_timer;
	float refresh_delay = 1.0f; //Seconds
public:

};