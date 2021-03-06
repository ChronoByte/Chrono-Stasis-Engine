#include "csApp.h"
#include "csFileSystem.h"
#include <fstream> 
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
	LOG(" --- Creating File System Module ---");

	LOG("Initializing PhysFS library"); 
	char* app_path = SDL_GetBasePath();
	
	if (PHYSFS_init(NormalizeSlashSymbol(app_path).c_str()) != 0)
	{
		ret = true;
		LOG("PhysFS successfully initialized.");
		LOG("PhysFS version: v%i.%i.%i", PHYSFS_VER_MAJOR, PHYSFS_VER_MINOR, PHYSFS_VER_PATCH);
	}
	else
		LOG("FILESYSTEM: error while initaliazing PHYSFS: %s", PHYSFS_getLastError());

	SDL_free(app_path);

	if (PHYSFS_setWriteDir(".") == 0)
		LOG("FILESYSTEM: error while creating write dir: %s\n", PHYSFS_getLastError());

	
	AddPath(".");
	//AddPath("../Game");
	
	LOG("Generate directories");
	for (uint i = 0; i < directories.size(); ++i)
		GenerateDirectory(directories.at(i));
	
	LOG("Mounting Pathes");
	AddPath(ASSETS_FOLDER);
	AddPath(LIBRARY_FOLDER);
	AddPath(SETTINGS_FOLDER);
	


	return ret;
}

bool ModuleFileSystem::Start()
{
	refresh_timer.Start();

	assets = new Folder();
	assets->name = ASSETS_FOLDER;

	LOG("FILESYSTEM: Scanning Assets");
	PushFilesRecursively(assets->name.c_str());
	LogAssetsInfo(assets);

	LOG("FILESYSTEM: Importing Assets");
	ImportFilesRecursively(assets);


	/*GenerateDirectory(LIBRARY_FOLDER);
	App->fs->GenerateDirectory(MESHES_FOLDER);
	App->fs->GenerateDirectory(TEXTURES_FOLDER);*/


	////WRITING FILE TEST
	//char* text = "Testing PhysFS";
	//WriteFile("TestFile.txt", text, strlen(text));

	////EXIST FILE TEST
	//bool test1 = false;
	//test1 = FileExist("TestFile.txt");
	//LOG("FILESYSTEM: txt: %d", test1);

	////READING FILE TEST
	//char* text2 = new char[20];
	//ReadFile("TestFile.txt", text2);
	//LOG("%s", text2);

	return true;
}

update_status ModuleFileSystem::Update(float dt)
{

	if (refresh_timer.ReadSec() >= refresh_delay)
	{
		LOG("FILESYSTEM: Refresh Files");
		RefreshFiles();
		//LogAssetsInfo(assets);
		refresh_timer.Start();
	}


	if (App->input->dropped && App->input->file != nullptr)
	{
		bool file_found = CheckDroppedFile(App->input->file);

		if (!file_found) 
		{
			bool file_copied = CopyToAssets(App->input->file);
			if (file_copied)
			{
				LOG("SUCCESS: File Copied to Assets Directory successfully");
			}
			else
				LOG("ERROR: File could not be copied to Assets");
				
		}
		else
		{
			LOG("Copying a file that already exists in assets directory");
		}
		
		App->input->dropped = false;
		SDL_free(App->input->file);
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		LOG("FILESYSTEM: Refresh Files");
		RefreshFiles();
		LogAssetsInfo(assets);
	}

	//TODO: Check if new file was added or moified in Assets Folder

	//Delete .META files just for swagg
	/*if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) 
	{
		LOG("FILESYSTEM: Deleting .META files");
		DeleteMetaFiles(assets);
	}*/

	////Delete Own Format files & Resources just for swagg
	//if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	//{
	//	LOG("FILESYSTEM: Deleting Own Format files & Resources");
	//	DeleteOwnFormatFiles(assets);
	//}

	/*if (App->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
	{
		LOG("FILESYSTEM: Checking files to Update");
		ImportFilesRecursively(assets);
	}*/


	return UPDATE_CONTINUE;
}

bool ModuleFileSystem::CleanUp()
{
	bool ret = false;

	delete assets;
	assets = nullptr;
	LOG("FILESYSTEM: Deinitializing PHYSFS...");

	for (uint i = 0; i < directories.size(); ++i)
		DeleteDirectory(directories.at(i));
    
   /* DeleteDirectory(TEXTURES_FOLDER);
	DeleteDirectory(MESHES_FOLDER);
	DeleteDirectory(LIBRARY_FOLDER);*/

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

uint ModuleFileSystem::ReadFile(const char* file_name, char** buffer)
{
	
	PHYSFS_File* file = OpenFileRead(file_name);
	uint size = 0u;

	if (file != nullptr)
	{
		size = PHYSFS_fileLength(file);

		*buffer = new char[size];

		if (PHYSFS_readBytes(file, *buffer, size) == -1)
		{
			size = 0u;
			LOG("FILESYSTEM: Could not read file [%s]: %s", file_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		}
		else
			LOG("FILESYSTEM: Success on read file [%s]", file_name);
	}

	CloseFile(file, file_name);

	return size;
}


bool ModuleFileSystem::GenerateDirectory(const char* dir_name)
{
	bool ret = false;

	if (PHYSFS_mkdir(dir_name) != 0)
	{
		ret = true;
		LOG("FILESYSTEM: Directory [%s] created successfully!", dir_name);
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

void ModuleFileSystem::GetExtensionFile(const char* file, std::string& extension)
{
	std::string ref = file;
	//extension = file;
	
	uint position = ref.find_last_of(".");

	if (position != std::string::npos) 
	{
		extension = file;
		extension = extension.substr(position);
	}


}

void ModuleFileSystem::GetJSONExtensionFile(const char* file, std::string& extension)
{
	std::string ref = file;
	//extension = file;

	uint position = ref.find_first_of(".");

	if (position != std::string::npos)
	{
		extension = file;
		extension = extension.substr(position);
	}
	
}



void ModuleFileSystem::GetNameFile(const char* file, std::string& name)
{
	name = file;

	uint slash_pos = name.find_last_of("\\/");  // TODO: Handle with this file nav stuff 

	if (slash_pos != std::string::npos)
		name = name.substr(slash_pos + 1);

	uint ext_pos = name.find_last_of(".");

	if (ext_pos != std::string::npos)
		name = name.erase(ext_pos);
}

std::string ModuleFileSystem::GetDirectoryPath(const char * file)	// Gets directory of a certain path
{
	std::string path = file; 

	uint ext_pos = path.find_last_of("\\/"); // TODO: Handle with this file nav stuff too plsxd

	if (ext_pos != std::string::npos)
		path = path.erase(ext_pos + 1);

	return path;
}

std::string ModuleFileSystem::NormalizeSlashSymbol(const char* path)
{
	std::string str = path;

	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '\\')
			str[i] = '/';
	}

	return str.c_str();
}


void ModuleFileSystem::GetStorageResources(const char* path, std::list<StorageUnit*>& storage, const char* desiredExtension, const char* metaExtension, ExtensionType type)
{

	char** scannedFiles = PHYSFS_enumerateFiles(path);
	
	for (char** i = scannedFiles; *i != NULL; i++) {

		std::string currentPath = std::string(path) + *i; //TODO: check if it has "/"
		std::string fileName;
		std::string fileExtension;
		std::string jsonExtension;

		GetNameFile(currentPath.c_str(), fileName);
		GetExtensionFile(currentPath.c_str(), fileExtension);
		GetJSONExtensionFile(currentPath.c_str(), jsonExtension);
		if (fileExtension.length() > 0)
			fileName += fileExtension;

		
		// READ .SCENE FILES
		if (type == ExtensionType::SCENE_EXTENSION)
		{
			if (jsonExtension != ".scene.json")
			{
				if ((".meta" + fileExtension == metaExtension) && (metaExtension != nullptr)) { continue; }
			}
			else
			{
				StorageUnit* newResource = new StorageUnit();
				newResource->name = fileName;

				if (PHYSFS_isDirectory(currentPath.c_str()))
					newResource->type = StorageUnit::FOLDER;
				else
					newResource->type = StorageUnit::FILE;

				storage.push_back(newResource);
				continue;
			}

		}
		else if (type == ExtensionType::PARTICLE_EXTENSION)
		{
			// READ .PARTICLE FILES
		 

			if (jsonExtension != ".particle.json")
			{
				if ((".meta" + fileExtension == metaExtension) && (metaExtension != nullptr)) { continue; }
			}

			else
			{
				StorageUnit* newResource = new StorageUnit();
				newResource->name = fileName;

				if (PHYSFS_isDirectory(currentPath.c_str()))
					newResource->type = StorageUnit::FOLDER;
				else
					newResource->type = StorageUnit::FILE;

				storage.push_back(newResource);
				continue;
			}

		}
		
		
		//READ .META FILES
		if (!PHYSFS_isDirectory(currentPath.c_str()) && (fileExtension == desiredExtension) || PHYSFS_isDirectory(currentPath.c_str()) || !PHYSFS_isDirectory(currentPath.c_str()) && desiredExtension == "all")
		{
			StorageUnit* newResource = new StorageUnit();
			newResource->name = fileName;

			if (PHYSFS_isDirectory(currentPath.c_str()))
				newResource->type = StorageUnit::FOLDER;
			else
				newResource->type = StorageUnit::FILE;

			storage.push_back(newResource);
		}
	}


}

bool ModuleFileSystem::CopyToAssets(const char* src_file_path)
{
	bool ret = false;

	// Opening file for reading
	std::ifstream output_file(src_file_path, std::ifstream::binary);

	if (output_file)
	{
		ret = true;

		output_file.seekg(0, output_file.end);
		int length = output_file.tellg();
		output_file.seekg(0, output_file.beg);

		// Reading file
		char* buffer = new char[length];
		output_file.read(buffer, length);

		if (buffer != nullptr)
		{
			output_file.close();

			// Get Name File
			std::string file_name;
			GetNameFile(src_file_path, file_name);
			// Get Extension File
			std::string extension;
			GetExtensionFile(src_file_path, extension);

			// Get File
			std::string full_file = file_name + extension;


			if (!extension.compare(".fbx") || !extension.compare(".FBX"))
			{
				WriteFile((A_MODELS_FOLDER + full_file).c_str(), buffer, length);
			}

			if (!extension.compare(".png") || !extension.compare(".PNG") || !extension.compare(".tga") ||
				!extension.compare(".TGA") || !extension.compare(".dds") ||
				!extension.compare(".jpg") || !extension.compare(".JPG"))
			{
				WriteFile((A_TEXTURES_FOLDER + full_file).c_str(), buffer, length);
			}
		}
		else
		{
			output_file.close();
			ret = false;
		}
		RELEASE_ARRAY(buffer);
	}
	return ret;

}

void ModuleFileSystem::PushFilesRecursively(const char* folder_name)
{
	std::string tmp_folder(folder_name);

	char** scannedFiles = PHYSFS_enumerateFiles(tmp_folder.c_str());

	if (scannedFiles)
	{
		for (char** i = scannedFiles; *i != nullptr; i++)
		{
			if(PHYSFS_isDirectory((tmp_folder + *i).c_str()))
			{
				std::string folder_path(tmp_folder);
				folder_path += *i; // ex: Assets/Models

				Folder new_folder;
				new_folder.name = *i; // ex: Models
				new_folder.path = folder_path;

				assets->AddFolder(new_folder);
				//assets->folders.push_back(new_folder);
				PushFilesRecursively((folder_path + "/").c_str());
			}
			else
			{
				File new_file;
				new_file.name = *i;
				new_file.lastModTime = PHYSFS_getLastModTime((tmp_folder + *i).c_str());
				new_file.path = tmp_folder; //not file path

				if (!tmp_folder.compare(ASSETS_FOLDER))
					assets->files.push_back(new_file);
				else
					assets->folders.at(assets->index-1).files.push_back(new_file);
					
			}
		}
	}
	PHYSFS_freeList(scannedFiles);

}

void ModuleFileSystem::LogAssetsInfo(Folder* root)
{
	for (uint i = 0u; i < root->files.size(); i++)
		LOG("FILE: [%s] exists in folder [%s]", root->files[i].name.c_str(), root->name.c_str());

	for (uint j = 0u; j < root->folders.size(); j++) {
		LOG("FOLDER: [%s] exists", root->folders[j].name.c_str());
		LogAssetsInfo(&root->folders[j]);
	}

}

void ModuleFileSystem::ImportFilesRecursively(Folder* root, bool start)
{

	SearchLibraryFolders();

	for (uint i = 0u; i < root->files.size(); i++) {
		
		std::string file_name = root->files[i].name;
		std::string file_path = root->files[i].path;

		//Check if is meta file
		std::string tmp_ext = "";
		GetExtensionFile(file_name.c_str(), tmp_ext);
		
		if (tmp_ext == ".json")
			continue;

		//If isn't check for its meta file
		file_path += file_name;

		std::string meta_file = file_path + META_EXTENSION;
		
		if (!PHYSFS_exists(meta_file.c_str()))
		{
			std::string extension;
			GetExtensionFile(file_name.c_str(), extension);

			if (!extension.compare(".fbx") || !extension.compare(".FBX"))
			{
				UID ret;
				ret = App->resources->ImportFile(file_path.c_str(), Resource::R_MESH);
			}

			else if (!extension.compare(".png") || !extension.compare(".PNG") || !extension.compare(".tga") ||
					 !extension.compare(".TGA") || !extension.compare(".dds") ||
					 !extension.compare(".jpg") || !extension.compare(".JPG"))
			{
				UID ret;
				ret = App->resources->ImportFile(file_path.c_str(), Resource::R_TEXTURE);
			}

			else
			{
				//LOG("Unsupported file format: [%s]", extension);
			}

		}
		else 
		{
			std::string extension;
			GetExtensionFile(file_name.c_str(), extension);

			if (!extension.compare(".fbx") || !extension.compare(".FBX"))
			{
				//UID ret;
				//ret = App->resources->ImportFile(file_path.c_str(), Resource::R_MESH);
			}

			else if (!extension.compare(".png") || !extension.compare(".PNG") || !extension.compare(".tga") ||
				!extension.compare(".TGA") || !extension.compare(".dds") ||
				!extension.compare(".jpg") || !extension.compare(".JPG"))
			{
				
				ResourceTexture* resMat = (ResourceTexture*)App->resources->GetResourceFromFile(file_path.c_str());

				if(resMat == nullptr)
				{
					JSON_Value * json_file = nullptr;
					JSON_Object* root_obj = nullptr;

					json_file = json_parse_file(meta_file.c_str());
					root_obj = json_value_get_object(json_file);

					UID exported_uuid = json_object_get_number(root_obj, "UUID");
					//std::string exported_path = json_object_get_string(root_obj, "UUID_path");

					UID ret;
					ret = App->resources->ImportFile(file_path.c_str(), Resource::R_TEXTURE, exported_uuid);
				}


			}

			/*JSON_Value* config_file;
			JSON_Object* config;
			JSON_Object* config_node;

			config_file = json_parse_file(meta_file.c_str());
			if (config_file != nullptr)
			{
				config = json_value_get_object(config_file);
				config_node = json_object_get_object(config, "Model");
				std::string exported_path = json_object_dotget_string(config_node, "GameObject1.Components.Component1.Resource Mesh Path");

				if (PHYSFS_exists(exported_path.c_str()))
				{

				}
			}*/
			//LOG("File was imported previously");
			//std::string extension;
			//GetExtensionFile(file_name.c_str(), extension);

			//
			//JSON_Value* json_file = nullptr;
			//JSON_Object* root_obj = nullptr;


			//json_file = json_parse_file(meta_file.c_str());
			//root_obj = json_value_get_object(json_file);

			//UID exported_uuid = json_object_get_number(root_obj, "UUID");
			//std::string exported_path = json_object_get_string(root_obj, "UUID_path");

			//	if (!PHYSFS_exists(exported_path.c_str()))
			//	{
			//		std::string extension;
			//		GetExtensionFile(file_name.c_str(), extension);

			//		if (!extension.compare(".fbx") || !extension.compare(".FBX"))
			//		{
			//			UID ret;
			//			//ret = App->resources->ImportFile(file_path.c_str(), Resource::R_MESH, exported_uuid);
			//		}

			//		else if (!extension.compare(".png") || !extension.compare(".PNG") || !extension.compare(".tga") ||
			//			!extension.compare(".TGA") || !extension.compare(".dds") ||
			//			!extension.compare(".jpg") || !extension.compare(".JPG"))
			//		{
			//			UID ret;
			//			ret = App->resources->ImportFile(file_path.c_str(), Resource::R_TEXTURE, exported_uuid);
			//		}

			//		else
			//		{
			//			LOG("Unsupported file format");
			//		}
			//	}
			//	else
			//	{
			//		std::string extension;
			//		GetExtensionFile(file_name.c_str(), extension);

			//		if (!extension.compare(".fbx") || !extension.compare(".FBX"))
			//		{
			//			UID ret;
			//			//ret = App->resources->ImportFile(file_path.c_str(), Resource::R_MESH, exported_uuid);
			//		}

			//		else if (!extension.compare(".png") || !extension.compare(".PNG") || !extension.compare(".tga") ||
			//			!extension.compare(".TGA") || !extension.compare(".dds") ||
			//			!extension.compare(".jpg") || !extension.compare(".JPG"))
			//		{
			//			UID ret;
			//			Resource* res = App->resources->CreateNewResource(Resource::R_TEXTURE, exported_uuid, file_name.c_str(), file_path.c_str(), exported_path.c_str(), true);
			//			if (res)
			//				ret = res->GetUID();
			//			ret = App->resources->ImportFile(file_path.c_str(), Resource::R_TEXTURE, exported_uuid);
			//		}

			//		else
			//		{
			//			LOG("Unsupported file format");
			//		}
			//	}
			
			
			//TODO: Check modifications (remove, add, rename,...)
			// Re-Import new meta
		}

	}

	for (uint i = 0u; i < root->folders.size(); i++) 
		ImportFilesRecursively(&root->folders[i]);
	
}

void ModuleFileSystem::DeleteMetaFiles(Folder* root)
{
	for (uint i = 0u; i < root->files.size(); i++) 
	{

		std::string file_name = root->files[i].name;
		std::string file_path = root->files[i].path;
		file_path += file_name;

		std::string meta_file = file_path + META_EXTENSION;

		if (PHYSFS_exists(meta_file.c_str()))
			DeleteFiles(meta_file.c_str());
		
	}

	for (uint i = 0u; i < root->folders.size(); i++)
		DeleteMetaFiles(&root->folders[i]);

}

bool ModuleFileSystem::DeleteFiles(const char* path)
{
	bool ret = true;

	if (PHYSFS_delete(path) != 0) 
	{
		LOG("FILESYSTEM: success on deleting %s\n", path);
	}
	else
	{
		ret = false;
		LOG("FILESYSTEM: could not delete %s: %s\n", path, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}

	return ret;
}

void ModuleFileSystem::DeleteOwnFormatFiles(Folder* root)
{
	for (uint i = 0u; i < root->files.size(); i++)
	{

		std::string file_name = root->files[i].name;
		std::string file_path = root->files[i].path;
		file_path += file_name;

		std::string meta_file = file_path + META_EXTENSION;

		if (PHYSFS_exists(meta_file.c_str())) 
		{
			JSON_Value* json_file = nullptr;
			JSON_Object* root_obj = nullptr;

			
			json_file = json_parse_file(meta_file.c_str());
			root_obj = json_value_get_object(json_file);

			UID uuid = json_object_get_number(root_obj, "UUID");

			Resource* tmp_res = App->resources->GetResource(uuid);
		    
			if (tmp_res)
			{
				DeleteFiles(tmp_res->GetExportedFile());
			}

			App->resources->DeleteResourceFromUID(uuid);
		}
			

	}

	for (uint i = 0u; i < root->folders.size(); i++)
		DeleteOwnFormatFiles(&root->folders[i]);


}

void ModuleFileSystem::SearchLibraryFolders()
{
	for(int i = 0; i < library_directories.size(); ++i)
	{
		if (!FileExist(library_directories.at(i)))
			GenerateDirectory(library_directories.at(i));
	}

}

Folder* ModuleFileSystem::GetAssetsFolder() const
{
	return assets;
}

bool ModuleFileSystem::CheckDroppedFile(const char* dropped_file)
{
	bool file_found = false;

	std::string file_name;
	GetNameFile(dropped_file, file_name);

	std::string extension;
	GetExtensionFile(dropped_file, extension);

	std::string full_file = file_name + extension;

	std::vector<File> files;
	assets->GetFilesRecursively(files);

	for (uint i = 0; i < files.size(); i++)
	{
		if (!files[i].name.compare(full_file))
		{
			file_found = true;
			//LOG("Copying a file that already exists in assets directory");
			break;
		}

	}

	return file_found;
}

void ModuleFileSystem::RefreshFiles()
{
	CleanAssets(assets);
	assets->ClearFiles();

	PushFilesRecursively(assets->name.c_str());
	ImportFilesRecursively(assets, false);

}

void ModuleFileSystem::CleanAssets(Folder* root)
{
	root->files.clear();

	for (uint i = 0u; i < root->folders.size(); i++) {
		CleanAssets(&root->folders[i]);
		root->folders.clear();
	}

}


int ModuleFileSystem::GetLastModTime(const char* path) const
{
	PHYSFS_Stat stat;
	if (PHYSFS_stat(path, &stat) != 0)
	{
		return stat.modtime;
	}

	return 0;
}

JSON_Status ModuleFileSystem::json_array_dotset_float3(JSON_Object* object, const char* name, float3 transform)
{
	JSON_Value* value = json_value_init_array();
	if (value == NULL) {
		return JSONFailure;
	}
	JSON_Array* arry = json_value_get_array(value);
	if (json_object_dotset_value(object, name, value) == JSONFailure)
	{
		json_value_free(value);
		return JSONFailure;
	}
	json_array_append_number(arry, transform.x);
	json_array_append_number(arry, transform.y);
	json_array_append_number(arry, transform.z);
	return JSONSuccess;
}

JSON_Status ModuleFileSystem::json_array_dotset_float2(JSON_Object* object, const char* name, float2 transform)
{
	JSON_Value* value = json_value_init_array();
	if (value == NULL) {
		return JSONFailure;
	}
	JSON_Array* arry = json_value_get_array(value);
	if (json_object_dotset_value(object, name, value) == JSONFailure)
	{
		json_value_free(value);
		return JSONFailure;
	}
	json_array_append_number(arry, transform.x);
	json_array_append_number(arry, transform.y);
	return JSONSuccess;
}

JSON_Status ModuleFileSystem::json_array_dotset_float4(JSON_Object* object, const char* name, float4 transform)
{
	JSON_Value* value = json_value_init_array();
	if (value == NULL) {
		return JSONFailure;
	}
	JSON_Array* arry = json_value_get_array(value);
	if (json_object_dotset_value(object, name, value) == JSONFailure)
	{
		json_value_free(value);
		return JSONFailure;
	}
	json_array_append_number(arry, transform.x);
	json_array_append_number(arry, transform.y);
	json_array_append_number(arry, transform.z);
	json_array_append_number(arry, transform.w);
	return JSONSuccess;
}

float3 ModuleFileSystem::json_array_dotget_float3_string(const JSON_Object* object, const char* name)
{
	JSON_Array* array = json_object_dotget_array(object, name);
	float3 transform;
	transform.x = (float)json_value_get_number(json_array_get_value(array, 0));
	transform.y = (float)json_value_get_number(json_array_get_value(array, 1));
	transform.z = (float)json_value_get_number(json_array_get_value(array, 2));

	return transform;
}
float2 ModuleFileSystem::json_array_dotget_float2_string(const JSON_Object* object, const char* name)
{
	JSON_Array* array = json_object_dotget_array(object, name);
	float2 transform;
	transform.x = (float)json_value_get_number(json_array_get_value(array, 0));
	transform.y = (float)json_value_get_number(json_array_get_value(array, 1));

	return transform;
}
float4 ModuleFileSystem::json_array_dotget_float4_string(const JSON_Object* object, const char* name)
{
	JSON_Array* array = json_object_dotget_array(object, name);
	float4 transform;
	transform.x = (float)json_value_get_number(json_array_get_value(array, 0));
	transform.y = (float)json_value_get_number(json_array_get_value(array, 1));
	transform.z = (float)json_value_get_number(json_array_get_value(array, 2));
	transform.w = (float)json_value_get_number(json_array_get_value(array, 3));

	return transform;
}


