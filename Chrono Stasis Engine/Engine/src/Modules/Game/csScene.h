#pragma once

#include "csModule.h"
#include "csGlobals.h"
#include <map>

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

class ComponentCamera; 

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

	void DrawScene();
	void DrawGrid();
	void DrawOriginAxis(); 

	// --------- Game Objects ---------
	void UpdateAllGameObjects(GameObject* parent, float dt); 
	void DrawAllGameObjects(GameObject* parent); 
	
	// Mouse picking
	void CheckRayAgainstAABBS(GameObject* parent, const LineSegment& ray, std::multimap<float, GameObject*>& objectsIntersected);
	GameObject* CheckRayAgainstTris(const LineSegment& ray, std::multimap<float, GameObject*>& intersected); 

	// Creation
	GameObject* CreateGameObject(GameObject* parent, const char* name); 
	GameObject* CreateRoot(); 
	GameObject* GetRoot() const; 

	GameObject* CreateObject3D(PrimitiveType type, GameObject* parent); 
	GameObject* CreateCamera(GameObject* parent, const char* name); 

	// Selection
	GameObject* GetSelected() const; 
	void SetSelected(GameObject* go); 
	void CleanSelected(); 

	// ------------ Camera -----------
	void SetMainCamera(ComponentCamera* camera);
	void ClearCamera(); 
	ComponentCamera* GetMainCamera() const; 


public: 
	ComponentCamera* mainCamera = nullptr; 

private:
	//vars
	GameObject* root = nullptr; 
	GameObject* selected = nullptr; 
};
