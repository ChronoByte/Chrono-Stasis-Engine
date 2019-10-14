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
	bool CleanUp();

	void RecursiveUpdate(GameObject* parent, float dt); 

	GameObject* CreateGameObject(GameObject* parent, const char* name); 
	GameObject* CreateRoot(); 
	GameObject* GetRoot() const; 
	void DirectDrawing(const uint &imageId);

private:
	//vars
	GameObject* root = nullptr; 
};
