#pragma once

#include <string>
#include <vector>

class Component; 

class GameObject
{
public:

	GameObject();
	~GameObject(); 

	void Update(float dt); 

public:
	bool isActive() const;
	void Enable();
	void Disable();

private: 

	bool active = false; 
	std::string name; 
	std::vector<Component*> components; 

	const GameObject* parent = nullptr; 
	std::vector<GameObject*> childs; 
};