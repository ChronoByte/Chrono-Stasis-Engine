#pragma once

#include "csModule.h"
#include "csGlobals.h"

class GameObject;

enum class PrimitiveType
{
	CUBE = 0,
	SPHERE, 
	CYLINDER,
	CONE, 
	TORUS,
	PLANE,
	// ---- 

	MAX
};


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

	GameObject* CreateObject3D(PrimitiveType type, GameObject* parent); 
	void DirectDrawing(const uint &imageId);

private:
	//vars
	GameObject* root = nullptr; 
};
