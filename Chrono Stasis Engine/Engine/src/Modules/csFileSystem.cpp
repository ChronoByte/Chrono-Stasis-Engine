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
	assets = new Folder();
	assets->name = ASSETS_FOLDER;

	LOG("FILESYSTEM: Scanning Assets");
	PushFilesRecursively(assets->name.c_str());
	LogAssetsInfo(assets);
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
	//TODO: Check if new file was added or moified in Assets Folder
	return UPDATE_CONTINUE;
}

bool ModuleFileSystem::CleanUp()
{
	bool ret = false;

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

std::string ModuleFileSystem::GetFullPath(const char* path, const char* folder, const char* extension)
{
	std::string full_path = path;
	std::string directory;

	
	if (folder != nullptr || extension != nullptr)
		SplitPath(path, &directory, &full_path, nullptr);

	if (folder != nullptr)
		full_path = folder + full_path;
	else
		full_path = directory + full_path;
	if (extension != nullptr)
		full_path += extension;

	return full_path.c_str();
}

void ModuleFileSystem::SplitPath(const char* full_path, std::string* path, std::string* filename, std::string* extension)
{
	std::string str = NormalizeSlashSymbol(full_path);
	uint pos_slash = str.find_last_of('/');
	uint pos_dot = str.find_last_of('.');

	if (path != nullptr)
	{
		if (pos_slash < str.length())
			*path = str.substr(0, pos_slash + 1);
		else
			path->clear();
	}

	if (filename != nullptr)
	{
		if (pos_slash < str.length())
			*filename = str.substr(pos_slash + 1, pos_dot - pos_slash - 1);
		else
			*filename = str.substr(0, pos_dot);
	}

	if (extension != nullptr)
	{
		if (pos_dot < str.length())
			*extension = str.substr(pos_dot + 1);
		else
			extension->clear();
	}
}

void ModuleFileSystem::GetStorageResources(const char* path, std::list<StorageUnit*>& storage, const char* desiredExtension)
{

	char** scannedFiles = PHYSFS_enumerateFiles(path);
	
	for (char** i = scannedFiles; *i != NULL; i++) {

		std::string currentPath = std::string(path) + *i; //TODO: check if it has "/"
		std::string fileName;
		std::string fileExtension;

		GetNameFile(currentPath.c_str(), fileName);
		GetExtensionFile(currentPath.c_str(), fileExtension);

		if (fileExtension.length() > 0) //if is not a folder, add dot and extension 
			fileName += fileExtension;

		if (!PHYSFS_isDirectory(currentPath.c_str()) && (fileExtension == desiredExtension) || PHYSFS_isDirectory(currentPath.c_str()))
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

			// Getting file name
			std::string file_name;
			GetNameFile(src_file_path, file_name);
			

			// Getting file extension
			std::string extension;
			GetExtensionFile(src_file_path, extension);

			//Getting full name
			std::string full_file_name = file_name + extension;
		

			// Checking if file already exists in assets
			std::vector<File> files;
			assets->GetFilesRecursively(files);

			bool file_exists = false;
			bool new_file = false;
			for (uint i = 0; i < files.size(); i++)
			{
				if (!files[i].name.compare(full_file_name))
				{
					file_exists = true;
					LOG("Copying a file that already exists in assets directory");
					break;
				}
			
			}

			if (!file_exists)
			{
				bool accepted_file = false;
				std::string new_path;

				if (!extension.compare(".fbx") || !extension.compare(".FBX"))
				{
					accepted_file = true;
					new_path = A_MODELS_FOLDER;
				}
				else if(!extension.compare(".png") || !extension.compare(".PNG") || !extension.compare(".tga") ||
					    !extension.compare(".TGA") || !extension.compare(".dds") ||
					    !extension.compare(".jpg") || !extension.compare(".JPG"))
				{
					accepted_file = true;
					new_path = A_TEXTURES_FOLDER;
				}

				if (accepted_file)
				{
					WriteFile((new_path + full_file_name).c_str(), buffer, length);
				
				}
			}

		}
		else
		{
		output_file.close();
		ret = false;
		}
	delete[] buffer;
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





int ModuleFileSystem::GetLastModTime(const char* path) const
{
	PHYSFS_Stat stat;
	if (PHYSFS_stat(path, &stat) != 0)
	{
		return stat.modtime;
	}

	return 0;
}




