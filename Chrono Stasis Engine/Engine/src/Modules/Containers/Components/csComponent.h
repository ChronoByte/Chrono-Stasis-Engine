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
	Component(){}
	~Component(){}

	virtual void Update(float dt){}
	virtual void Enable() { active = true; }
	virtual void Disable() { active = false; }

	void SetParent(const GameObject* goParent)
	{
		myGo = goParent; 
	}
	void SetType(ComponentType mytype)
	{
		type = mytype; 
	}

private:

	ComponentType type = ComponentType::C_NONE; 
	const GameObject* myGo = nullptr; 
	bool active = true; 
	std::string name; 

};