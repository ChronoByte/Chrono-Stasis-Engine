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
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float size = 1.f; 
	float4 lightColor; 
	float3 force = float3(0.f, -10.f, 0.f);  // float3::zero;
};

struct ParticleInfo
{
	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 velocity = float3(0.f, 5.f, 0.f); // float3::zero;
	float3 force = float3(0.f, 0.0f, 0.f); // float3::zero;
	float speed = 1.0f; 

	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f); 
	float size = 1.f;
	float4 lightColor = float4::zero;
	
	float maxLifeTime = 5.f;
};

class Particle
{

public: 

	Particle(ParticleSystem* owner, ParticleInfo info, ParticleMutableInfo startInfo, ParticleMutableInfo endInfo);
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
	ParticleSystem* owner = nullptr; 

	ParticleInfo particleInfo; 
	ParticleMutableInfo startInfo;
	ParticleMutableInfo endInfo; 

	float currentLifeTime = 0.f; 
};