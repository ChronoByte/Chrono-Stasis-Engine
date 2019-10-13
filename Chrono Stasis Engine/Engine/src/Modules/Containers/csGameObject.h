#pragma once

#include <string>
#include <list>

class Component; 
enum class ComponentType; 

class GameObject
{
public:

	GameObject();
	~GameObject(); 

	void Update(float dt); 

	void RemoveChild(GameObject* child); 

	GameObject* GetParent() const; 
	void SetParent(GameObject* parent); 
	void SetName(const char* name); 

	Component* CreateComponent(ComponentType type);
	void AddComponent(Component* component);

public:
	bool isActive() const;
	void Enable();
	void Disable();

private: 

	bool active = false; 
	std::string name; 
	std::list<Component*> components; 

	GameObject* parent = nullptr; 
	std::list<GameObject*> childs; 
};