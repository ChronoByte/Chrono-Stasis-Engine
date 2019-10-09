#pragma once

#include "csModule.h"
#include "csGlobals.h"

class ModuleTextureLoader : public Module
{
public:

	ModuleTextureLoader(bool start_enabled = true);
	~ModuleTextureLoader();

	
	bool Init(JSON_Object* node);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	bool CleanUp();

private:

};
