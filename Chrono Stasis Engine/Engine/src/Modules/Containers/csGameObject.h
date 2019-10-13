#pragma once

#include <string>
#include <list>

class Component; 
enum class ComponentType; 

class GameObject
{
public:

	GameObject();
	GameObject(GameObject* parent); 
	~GameObject(); 

	// Logic
	void Update(float dt); 

	void Enable();
	void Disable();

	// Parenting
	void RemoveChild(GameObject* child); 
	void SetParent(GameObject* parent); 
	void SetName(const char* name); 

	// Components 
	Component* CreateComponent(ComponentType type);
	bool AssignComponent(Component* component);
	bool FindComponent(ComponentType type); 
	void RemoveComponent(Component* component);

	// Gets
	bool isActive() const;
	GameObject* GetParent() const;
	const char* GetName() const;
	std::list<GameObject*> GetChilds() const; 

	// Inspector Panel
	void DrawInspectorComponents();


private: 

	bool active = true; 
	GameObject* parent = nullptr; 

public:

	std::string name; 
	std::list<Component*> components; 
	std::list<GameObject*> childs; 
};