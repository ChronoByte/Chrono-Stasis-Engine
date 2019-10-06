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
	bool ModuleFileSystem::AddPath(const char* path);
	bool ModuleFileSystem::FileExist(char* file_name) const;

private:

};