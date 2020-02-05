#pragma once
#include "csGlobals.h"
#include "Light.h"

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4.h"
#include "MathGeoLib/include/Math/quat.h"
#include "MathGeoLib/include/Math/float4x4.h"

#include "ResourceTexture.h"

class ParticleSystem; 
class ComponentCamera; 

struct ParticleInfo
{
	float3 position = float3::zero;
	Quat rotation = Quat::identity;

	float3 initForce = float3::zero; 
	float3 finalForce = float3::zero;

	float4 initColor = float4::one;
	float4 finalColor = float4::one;

	float initSize = 1.f;
	float finalSize = 1.f; 

	float3 velocity = float3::zero;
	float speed = 1.0f; 

	float4 initLightColor = float4::zero;
	float4 finalLightColor = float4::zero; 

	float maxLifeTime = 5.f;

	bool globalTransform = true;
	bool changeOverLifeTime = false; 

	void EqualizeFinalValues()
	{
		finalForce = initForce;
		finalColor = initColor; 
		finalSize = initSize; 
		finalLightColor = initLightColor; 
	}

};

class Particle
{

public: 

	Particle(ParticleSystem* owner, ParticleInfo info);
	~Particle();

	void PreUpdate(float dt);
	void Update(float dt);
	void PostUpdate(float dt);

	void Draw();
	void Orientate(ComponentCamera* camera); 

	void InterpolateValues(float dt);

	float3 GetPosition() const; 

	float Lerp(float v0, float v1, float t);

public:

	bool to_delete = false;
	Light light;

private: 
	//ResourceTexture* resMat = nullptr;

	ParticleSystem* owner = nullptr; 

	ParticleInfo particleInfo; 

	float currentSize = 0.f;
	float3 currentForce = float3::zero;
	float4 currentColor = float4::one;
	float4 currentLightColor = float4::one; 

	float currentLifeTime = 0.f; 

	// -------- Lerping -------------

	float rateToLerp = 0.f; 
	float percentage = 0.0f; 
	
};