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

	void OnPlay() override; 
	void OnGameUpdate(float dt) override; 
	void OnDraw() override; 
	void OnDebugDraw() override; 

	void InspectorInfo() override;

	ParticleSystem* GetSystem() const;

	// Parser
	void Save(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const;
	void Load(const JSON_Object* object, std::string name);

private: 

	
	ParticleSystem* particleSystem = nullptr; 
	bool drawEmmitter = false; 
	int shapeSelected = 0;
	int bbTypeSelected = 0;
	int matTypeSelected = 0;
	int eqTypeSelected = 0;
	int funcTypeSource = 6;
	int funcTypeDest = 7;
	int transformSelected = 0; // 0 Global - 1 Local

	// Animated Texture 
	bool enableAnim = false;
	float animSpeed = 0.1f;
	int texRows = 1;
	int texColumns = 1;
};