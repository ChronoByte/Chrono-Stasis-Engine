#pragma once
#include "csGlobals.h"
#include "Light.h"

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Math/quat.h"
#include "MathGeoLib/include/Math/float4x4.h"

class ParticleSystem; 
class ComponentCamera; 

struct ParticleMutableInfo
{
	float4 color; 
	float size = 1.f; 
	float4 lightColor; 
	float3 force;
};


class Particle
{

public: 

	Particle(ParticleSystem* owner, float3 position, float3 speed);
	~Particle();

	void PreUpdate(float dt);
	void Update(float dt);
	void PostUpdate(float dt);

	void Draw();
	void Orientate(ComponentCamera* camera); 

	float3 GetPosition() const; 


public:

	bool to_delete = false;
	Light light;

private: 

	ParticleMutableInfo startInfo;
	ParticleMutableInfo endInfo; 

	ParticleSystem* owner = nullptr; 

	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 speed = float3::zero;
	
	float4 currentColor;
	float currentSize = 1.f;
	float4 currentLightColor;
	float3 currentForce;

	float maxLifeTime = 0.f;
	float currentLifeTime = 0.f; 

};