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
	bool AddComponent(Component* component);
	bool FindComponent(ComponentType type); 
	void RemoveComponent(Component* component);

	// Gets
	bool isActive() const;
	GameObject* GetParent() const;
	const char* GetName() const;

	// Inspector Panel
	void DrawInspectorComponents();

public:


private: 

	bool active = false; 
	std::string name; 
	std::list<Component*> components; 

	GameObject* parent = nullptr; 
	std::list<GameObject*> childs; 
};