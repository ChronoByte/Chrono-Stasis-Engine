#pragma once
#include "csGlobals.h"
#include "Light.h"

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/quat.h"

class ParticleSystem; 

class Particle
{

public: 

	Particle(ParticleSystem* owner, float3 position, float3 speed);
	~Particle();

	void PreUpdate(float dt);
	void Update(float dt);
	void PostUpdate(float dt);

	float3 GetPosition() const; 

public:

	bool to_delete = false;
	Light light;

private: 

	ParticleSystem* owner = nullptr; 

	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 scale = float3::zero; 

	float3 speed = float3::zero;
	float3 force = float3::zero; 

	float maxLifeTime = 0.f;
	float currentLifeTime = 0.f; 

};