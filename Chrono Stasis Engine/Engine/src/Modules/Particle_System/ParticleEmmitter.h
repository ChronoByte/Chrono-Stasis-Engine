#pragma once
#include "csGlobals.h"
#include "Timer.h"

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/quat.h"
#include "MathGeoLib/include/Geometry/Circle.h"

#include "pcg/pcg C/include/pcg_variants.h"
#include "pcg/pcg C/extras/entropy.h"

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

enum class Emmitter_Zone
{
	Base,
	Volume,
	Surface,

	None
};

struct Burst
{
	Burst() {}
	float timeToBurst = 0.f; 
	int partsToInstantiate = 60; 
	bool hasBursted = false;

	void Reset() { hasBursted = false; }
};

class ParticleSystem; 

class ParticleEmmitter {

public :  

	ParticleEmmitter();
	~ParticleEmmitter();
	
	// Updates emmitter lifetime 
	void Update(float dt);

	// Returns how many particles should be instantiated this frame
	int GetParticlesToInstantiate();
	int GetParticlesToBurst();

	void Reset();
	void ResetBursts(); 

	bool isActive() const; 

	void DebugDrawEmmitter();
	// Get an initial position and an initial velocity given the emmiter type
	void GetInitialValues(float3& position, float3& velocity, float speed, bool localTransform);
	
	// -------- Bursts -----------

	bool HasBurstsActive() const; 
	void RemoveBurst(int index);

	// ------- Debug Draw --------

	void DrawSphere(double r, int lats, int longs);
	void DrawCone();
	
	// ----- Modify Emmitter -----

	void SetShape(Emmitter_Shape shape);
	void SetZone(Emmitter_Zone zone);
	void SetRadius(float radius);
	void SetOutRadius(float radius);
	void SetDistance(float distance); 

	void SetMaxLife(float maxLife); 
	void SetCurrentLife(float currentLife);
	void SetSpawnRate(float spawnRate);
	void SetDelay(float delay);
	void SetLoop(bool isLoop);

	void SetPosition(float3 position);
	void SetRelativePosition(float3 position);
	
	void SetRotation(Quat rotation);
	void SetRotation(float3 rotation);
	void SetRelativeRotation(float3 rotation);
	void SetScale(float3 scale);

	// ---------------------------

	// ----- Get Emmitter -----

	Emmitter_Shape GetShape() const;
	Emmitter_Zone GetZone() const;
	float GetRadius() const;
	float GetOutRadius() const;
	float GetDistance() const; 

	float GetMaxLife() const;
	float GetCurrentLife() const; 
	float GetSpawnRate() const;
	float GetDelay() const;
	bool GetLoop() const;

	float3 GetPosition() const; 
	float3 GetRelativePosition() const;
	float3 GetWorldPosition() const; 
	float3 GetRotation() const; 
	float3 GetRelativeRotation() const; 
	Quat GetWorldRotation() const; 
	float3 GetScale() const; 
	float4x4 GetGlobalTransform() const;

	// ---------------------------

public: 

	ParticleSystem* particleSystem = nullptr; 
	std::vector<Burst> bursts;

private: 


	Emmitter_Shape shape = Emmitter_Shape::Sphere; 
	Emmitter_Zone zone = Emmitter_Zone::Base;

	float radius = 5.f; 
	float outRadius = 10.f; 
	float distance = 20.f;

	float currentSpawnTime = 0.f; 
	float spawnRate = 0.05f; 

	float maxLifeTime = 10.f; 
	float lifeTime = 0.f;  
	float delay = 0.f; 

	bool loop = true; 

	// ----- Position Respect Game Object ------

	float3 position = float3::zero; 
	float3 relativePosition = float3::zero;

	Quat rotation = Quat::identity;
	Quat relativeRotation = Quat::identity;

	float3 scale = float3::zero;

	// -------- Random stuff ----------------

	uint64_t seeds[2];
	//Struct with state + inc
	pcg32_random_t rng; //typedef struct 64-b
	pcg_state_setseq_64 rng_bounded; // struct 64-b

	// more stuff
};