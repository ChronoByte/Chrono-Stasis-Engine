#pragma once

#include "csGlobals.h"
#include "csModule.h"

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem();
	~ModuleFileSystem();

	bool Start();
	bool CleanUp();

private:

};