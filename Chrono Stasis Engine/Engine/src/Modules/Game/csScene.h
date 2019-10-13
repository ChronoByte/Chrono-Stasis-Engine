#pragma once

#include "csModule.h"
#include "csGlobals.h"

class GameObject;

class ModuleScene : public Module
{
public:
	ModuleScene(bool start_enabled = true);
	virtual ~ModuleScene();

	bool Init(JSON_Object* node);
	bool Start();
	update_status Update(float dt);
	void DirectDrawing(const uint &imageId);
	bool CleanUp();


	GameObject* CreateGameObject(const char* name); 
	

private:
	//vars
	GameObject* root = nullptr; 
};
