#pragma once

#include "csModule.h"
#include "csGlobals.h"

class ModuleScene : public Module
{
public:
	ModuleScene(bool start_enabled = true);
	virtual ~ModuleScene();

	bool Init(JSON_Object* node);
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	

private:
	//vars

};
