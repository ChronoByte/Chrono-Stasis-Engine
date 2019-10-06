#pragma once

#include "csGlobals.h"
#include "csModule.h"

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
	bool FileExist(char* file_name) const;
	bool OpenFileWrite(char* file_name) const;
	bool OpenFileRead(char* file_name) const;
private:

};