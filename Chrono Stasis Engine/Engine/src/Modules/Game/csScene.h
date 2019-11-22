#pragma once

#include "csModule.h"
#include "csGlobals.h"
#include "csOctree.h"

#include <map>

class GameObject;
class BoundingBox;
class ComponentCamera; 

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

	bool Init(JSON_Object* node) override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt);
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	void DrawScene();
	void DebugDrawScene(); 

	void DrawGrid();
	void DrawOriginAxis(); 
	void DrawOctree(); 

	// --------- Game Objects ---------
	void UpdateAllGameObjects(GameObject* parent, float dt); 
	void DrawAllGameObjects(GameObject* parent); 
	void DebugDrawAllGameObjects(GameObject* parent); 


	// Mouse picking
	void CheckRayAgainstAABBS(GameObject* parent, const LineSegment& ray, std::multimap<float, GameObject*>& objectsIntersected, int& tests);
	GameObject* CheckRayAgainstTris(const LineSegment& ray, const std::multimap<float, GameObject*>& intersected); 

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

	void SetRoot(GameObject* go);
	void DeleteRoot();
	// ------------ Camera -----------
	void SetMainCamera(ComponentCamera* camera);
	void ClearCamera(); 
	ComponentCamera* GetMainCamera() const; 

	// ------------ Octree ------------
	bool isOctreeActive() const; 

public: 
	ComponentCamera* mainCamera = nullptr; 
	Octree* octree = nullptr;

	bool drawOctree = true; 
	bool activeOctree = true;

private:
	//vars
	GameObject* root = nullptr; 
	GameObject* selected = nullptr; 
};
