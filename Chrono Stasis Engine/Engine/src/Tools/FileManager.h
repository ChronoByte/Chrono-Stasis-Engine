#pragma once

#include <string>
#include <vector>

class File
{
public:
	File() {};
	File(char* name, int64_t last_modified)
	{
		this->name = name;
		this->lastModTime = last_modified;
	}
	void Serialize(char*& cursor) const{}
	void DeSerialize(char*& cursor){}

public:
	std::string name;
	std::string path;
	int64_t lastModTime = 0.0f;
};

class Folder
{
public:

	Folder() { }
	Folder(char* name) { this->name = name; }

	void AddFolder(Folder dir)
	{
		this->folders.push_back(dir);
		index++;
	}
	void AddFile(File file)
	{
		this->files.push_back(file);
		
	}

	void GetFilesRecursively(std::vector<File>& data)
	{
		for (uint i = 0u; i < files.size(); ++i)
			data.push_back(files[i]);

		for (uint i = 0u; i < folders.size(); ++i)
			folders[i].GetFilesRecursively(data);
	}

	
	void ClearFiles() 
	{
		index = 0u;
		files.clear();
		folders.clear();
	}

	void Serialize(char*& cursor) const {}
	void DeSerialize(char*& cursor) {}

	~Folder() 
	{ 
		name.clear();
		files.clear();
		folders.clear();
	}

	std::vector<Folder> GetFolders() const 
	{
		return folders;
	}

public:
	std::string name;
	std::vector<Folder> folders;
	std::vector<File> files;
	uint index = 0u;

};