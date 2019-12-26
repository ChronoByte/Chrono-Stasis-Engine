#pragma once
#include "csGlobals.h"
#include "Timer.h"

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/quat.h"

enum class Emmitter_Shape{

	Sphere, 
	Hemisphere,
	Cube, 
	Cone, 
	Plane,

	// etc

	// --- 
	None
};

class ParticleSystem; 

class ParticleEmmitter {

public :  

	ParticleEmmitter(ParticleSystem* owner);
	~ParticleEmmitter();

	void DebugDrawEmmitter(); 
	void ChangeShape(Emmitter_Shape shape);

	// Discuss if the emmiter does logic, or the particle system itself does it for him (taking into account its data)
	bool Update(float dt);

private: 

	ParticleSystem* particleSystem = nullptr; 

	Emmitter_Shape shape; 

	Timer spawnTimer; 
	float spawnRate = 0.f; 

	float maxLifeTime = 0.f; 
	float lifeTime = 0.f;  
	float delay = 0.f; 

	bool loop = true; 

	// ----- Position Respect Game Object ------

	float3 position = float3::zero; 
	Quat rotation = Quat::identity;
	float3 scale = float3::zero; 

	// -----------

	// more stuff
};