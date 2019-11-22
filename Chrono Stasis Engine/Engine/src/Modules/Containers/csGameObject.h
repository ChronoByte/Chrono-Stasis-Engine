#pragma once

#include <string>
#include <list>
#include <vector>

#include "csGlobals.h"

struct json_object_t;
typedef struct json_object_t JSON_Object;

class Component; 
class ComponentTransform; 
enum class ComponentType; 
struct RJSON_Value;

class GameObject
{
public:

	GameObject();
	GameObject(GameObject* parent); 
	~GameObject(); 

	// Logic
	void Update(float dt); 
	void OnDraw(); 
	void OnDebugDraw(); 

	void Enable();
	void Disable();

	void SetStatic(bool stat); 

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
	uint GetUUID() const;
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
	void Save(RJSON_Value* go);
	void SaveComponents(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const;

private: 

	uint UUID = 0;
	bool active = true; 
	bool staticGO = false;
	GameObject* parent = nullptr; 
	ComponentTransform* transform = nullptr; 
public:

	bool to_delete = false; 
	std::string name; 
	std::list<Component*> components; 
	std::list<GameObject*> childs; 
};