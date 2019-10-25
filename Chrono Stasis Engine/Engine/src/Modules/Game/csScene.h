#pragma once

#include "csModule.h"
#include "csGlobals.h"

class GameObject;
class BoundingBox;

enum class PrimitiveType
{
	CUBE = 0,
	SPHERE,
	PLANE,
	CYLINDER,
	CONE,
	TORUS,
	KLEIN_BOTTLE,
	TREFOIL_KNOT,
	HEMISPHERE,
	ROCK,
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
	update_status PreUpdate();
	update_status Update(float dt);
	update_status PostUpdate();
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
