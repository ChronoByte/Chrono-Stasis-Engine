#pragma once

#include "csModule.h"
#include "csGlobals.h"
#include "csOctree.h"

#include <map>

#define OCTREE_SIZE 100.f
class GameObject;
class BoundingBox;
class ComponentCamera; 
enum class EventType; 

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
	update_status Update(float dt) override;
	void OnGameUpdate(float dt); 
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	void DrawScene();
	void DrawCullingAllObjects();
	void DrawCullingObjectsWithOctree();
	void DebugDrawScene(); 

	void DrawGrid();
	void DrawOriginAxis(); 
	void DrawOctree(); 
	 
	void HandleEvent(EventType eventType) override; 

	// --------- Game Objects ---------
	void UpdateAllGameObjects(GameObject* parent, float dt); 
	void DrawAllGameObjects(GameObject* parent); 
	void DebugDrawAllGameObjects(GameObject* parent); 


	// Mouse picking
	void CheckRayAgainstAABBSRecursive(GameObject* parent, const LineSegment& ray, std::multimap<float, GameObject*>& objectsIntersected, int& tests);
	GameObject* CheckRayAgainstTris(const LineSegment& ray, const std::multimap<float, GameObject*>& intersected); 

	// Creation
	GameObject* CreateGameObject(GameObject* parent, const char* name, bool import = false); 
	GameObject* CreateRoot(); 
	GameObject* GetRoot() const; 

	GameObject* CreateObject3D(PrimitiveType type, GameObject* parent); 
	GameObject* CreateCamera(GameObject* parent, const char* name); 
	GameObject* CreateParticleSystem(GameObject* parent, const char* name); 
	GameObject* LoadGameObject(GameObject* parent, const char* name, UID uid);
	// Selection
	GameObject* GetSelected() const; 
	void SetSelected(GameObject* go); 
	void CleanSelected(); 

	void DeleteRoot();
	void ClearScene(); 
	void SetRoot(GameObject* go);
	// ------------ Camera -----------
	void SetMainCamera(ComponentCamera* camera);
	void ClearCamera(); 
	ComponentCamera* GetMainCamera() const; 

	// ------------ Octree ------------
	void CreateOctree(const float& size);

	void InsertInOctree(GameObject* go); 
	void RemoveFromOctree(GameObject* go); 

	void ResetOctree(); 
	void RecreateOctree(); 

	bool isOctreeActive() const; 

	// ------------- DynVector -----------
	void PushToDynamic(GameObject* go); 
	void RemoveFromDynamic(GameObject* go);
	std::list<GameObject*> GetDynamicObjects() const;	
	
	// ------------- StaticVector -----------
	void PushToStatic(GameObject* go); 
	void RemoveFromStatic(GameObject* go);
	std::list<GameObject*> GetStaticObjects() const;


public: 
	ComponentCamera* mainCamera = nullptr; 

	bool drawOctree = true; 
	bool activeOctree = false;
	bool toRecreateOctree = false; 
	Octree* octree = nullptr;


private:
	std::list<GameObject*> dynamicGameObjects; 
	std::list<GameObject*> staticGameObjects; 
	//vars
	GameObject* root = nullptr; 
	GameObject* selected = nullptr; 
};
