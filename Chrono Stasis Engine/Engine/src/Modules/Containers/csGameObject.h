#pragma once

#include <string>
#include <list>
#include <vector>

#include "csGlobals.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4.h"

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct Logic
{
	bool doLogic = false; 

	float3 velocity = float3::zero; 
	float maxLifeTime = 0.f;
	float currentLifeTime = 0.f; 
	float4 color = float4::zero; 

	bool readyToDie = false; 
	bool invisible = false;
};

class Component; 
class ComponentTransform; 
enum class ComponentType; 
struct RJSON_Value;

class GameObject
{
public:

	GameObject();
	GameObject(GameObject* parent); 
	GameObject(GameObject* parent, UID uid);
	~GameObject(); 

	// Logic
	void Update(float dt); 
	void DoLogic(float dt);

	void OnPlay();
	void OnPause(); 
	void OnStop(); 
	void OnGameUpdate(float dt);
	void OnDraw(); 
	void OnDebugDraw(); 

	void Enable();
	void Disable();

	void SetStatic(bool stat); 
	void SetActive(bool active);

	// Parenting
	void RemoveChild(GameObject* child); 
	void SetParent(GameObject* parent); 
	void SetName(const char* name); 
	GameObject* FindGreaterParent();

	// Components 
	Component* CreateComponent(ComponentType type);
	bool AssignComponent(Component* component);
	bool HasComponent(ComponentType type); 
	Component* FindComponent(ComponentType type); 
	void RemoveComponent(Component* component);
	void FindComponentsInAllChilds(ComponentType type, std::vector<Component*>& compsfound);

	// Gets
	UID GetUUID() const;
	int GetComponentsNum() const;
	int GetChildrenNum() const;
	bool isActive() const;
	bool isStatic() const;
	GameObject* GetParent() const;
	const char* GetName() const;
	std::list<GameObject*> GetChilds() const; 
	ComponentTransform* GetTransform() const; 
	// Inspector Panel 
	void DrawInspectorComponents();

	//Parser
	void LoadGameObjectChild(GameObject* child);
	void SaveComponents(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const;
	void LoadComponents(const JSON_Object* object, std::string name, uint numComponents);
private: 

	UID UUID = 0;
	bool active = true; 
	bool staticGO = false;
	GameObject* parent = nullptr; 
	ComponentTransform* transform = nullptr; 

public:
	Logic logic;

	bool to_delete = false; 
	std::string name; 
	std::list<Component*> components; 
	std::list<GameObject*> childs; 
};