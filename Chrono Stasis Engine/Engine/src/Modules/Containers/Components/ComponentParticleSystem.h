#pragma once
#include "csComponent.h"
#include "csGlobals.h"

class ParticleSystem;

class ComponentParticleSystem : public Component
{

public:

	ComponentParticleSystem(GameObject* parent);
	~ComponentParticleSystem();

	void Update(float dt) override;

	void OnGameUpdate(float dt) override; 
	void OnDraw() override; 
	void OnDebugDraw() override; 

	void InspectorInfo() override;

	// Parser
	void Save(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const;
	void Load(const JSON_Object* object, std::string name);

private: 

	
	ParticleSystem* particleSystem = nullptr; 
	bool drawEmmitter = true; 
	int shapeSelected = 0;
	int bbTypeSelected = 0;
	int matTypeSelected = 0;

};