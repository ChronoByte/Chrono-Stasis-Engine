#pragma once

#include <string>

#include "csGameObject.h"
#include "imgui/imgui.h"
#include "PseudoRandom.h"

struct RJSON_Value;

enum class ComponentType
{
	C_NONE = 0,

	C_TRANSFORM,
	C_MESH,
	C_MATERIAL,
	C_CAMERA,
	C_LIGHT,

	C_MAX
};

class Component
{

public:
	Component() {}
	Component(GameObject* parent) : owner(parent) { UUID = (uint)GenerateUUID; }

	virtual ~Component(){}

	virtual void Update(float dt){}
	virtual void OnDraw() {}

	virtual void Enable() { active = true; }
	virtual void Disable() { active = false; }
	
	virtual void InspectorInfo() {} // Update Inspector Info

	void SetOwner(GameObject* parent) { owner = parent; } // Only to be used in GameObject->AddComponent()

	// Gets
	bool isActive() const { return active; }
	ComponentType GetType() const { return type;  }
	const char* GetName() const { return name.c_str(); }
	GameObject* GetOwner() const { return owner; }

	// Parser
	virtual void Save(RJSON_Value* component) const {}
	virtual void Load(RJSON_Value* component) {}

protected:

	ComponentType type = ComponentType::C_NONE; 
	GameObject* owner = nullptr; 
	bool active = true; 
	std::string name; 
	uint UUID = 0;
};