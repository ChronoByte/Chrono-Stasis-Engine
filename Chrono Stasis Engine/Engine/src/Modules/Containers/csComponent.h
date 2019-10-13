#pragma once

#include <string>

#include "csGameObject.h"

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
	Component(GameObject* parent) : owner(parent) {}

	~Component(){}

	virtual void Update(float dt){}
	
	virtual void Enable() { active = true; }
	virtual void Disable() { active = false; }
	

	// Gets
	bool isActive() const { return active; }
	ComponentType GetType() const { return type;  }
	const char* GetName() const { return name.c_str(); }

protected:

	ComponentType type = ComponentType::C_NONE; 
	GameObject* owner = nullptr; 
	bool active = true; 
	std::string name; 

};