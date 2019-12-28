#pragma once

#include <string>

#include "csGameObject.h"
#include "imgui/imgui.h"
#include "PseudoRandom.h"
#include "Resource.h"
struct RJSON_Value;
struct json_object_t;
typedef struct json_object_t JSON_Object;

enum class ComponentType
{
	C_NONE = 0,

	C_TRANSFORM,
	C_MESH,
	C_MATERIAL,
	C_CAMERA,
	C_LIGHT,
	C_BILLBOARD,
	C_PARTICLE_SYSTEM,

	C_MAX
};

class Component
{

public:
	Component() {}
	Component(GameObject* parent) : owner(parent) {}

	virtual ~Component(){}

	virtual void Update(float dt){}
	virtual void OnGameUpdate(float dt) {}
	virtual void OnDraw() {}
	virtual void OnDebugDraw() {}
	virtual void Enable() { active = true; }
	virtual void Disable() { active = false; }
	
	virtual void InspectorInfo() {} // Update Inspector Info

	void SetOwner(GameObject* parent) { owner = parent; } // Only to be used in GameObject->AddComponent()

	// Gets
	bool isActive() const { return active; }
	ComponentType GetType() const { return type;  }
	const char* GetName() const { return name.c_str(); }
	GameObject* GetOwner() const { return owner; }
	Resource* GetCurrentResource() const { return currentResource; }

	// Parser
	virtual void Save(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const {};
	virtual void Load(const JSON_Object* object, std::string name) {};

	void AssignResource(UID uid);
	void AssignParticleResource(UID uid);

protected:
	Resource* currentResource = nullptr;
	ComponentType type = ComponentType::C_NONE; 
	GameObject* owner = nullptr; 
	bool active = true; 
	std::string name; 
	UID UUID = 0;
};