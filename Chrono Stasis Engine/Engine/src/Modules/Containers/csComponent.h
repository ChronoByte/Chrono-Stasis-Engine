#pragma once

#include <string>

class GameObject; 


enum class ComponentType
{
	C_NONE = 0,

	C_TRANSFORM,
	C_MESH,
	C_MATERIAL,
	C_LIGHT,

	C_MAX
};

class Component
{

public:
	Component() {}
	Component(GameObject* parent) : myGameObject(parent) {}

	~Component(){}

	virtual void Update(float dt){}
	
	virtual void Enable() { active = true; }
	virtual void Disable() { active = false; }
	bool isActive() const { return active; }

	void SetParent(const GameObject* goParent) { myGameObject = goParent; }
	void SetType(ComponentType mytype) { type = mytype; }
	ComponentType GetType() const { return type;  }

protected:

	ComponentType type = ComponentType::C_NONE; 
	const GameObject* myGameObject = nullptr; 
	bool active = true; 
	std::string name; 

};