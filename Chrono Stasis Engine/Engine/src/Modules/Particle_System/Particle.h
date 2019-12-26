#pragma once
#include "csGlobals.h"

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/quat.h"

class ParticleSystem; 

class Particle
{

public: 

	Particle();
	~Particle();

	void PreUpdate(float dt);
	void Update(float dt);
	void PostUpdate(float dt);

public:

	bool to_delete = false;

private: 

	ParticleSystem* owner = nullptr; 

	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 scale = float3::zero; 

	float3 force; 

	float maxLifeTime = 0.f;
	float currentLifeTime = 0.f; 


};