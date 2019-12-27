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
enum class BillboardType; 

class ParticleEmmitter {

public :  

	ParticleEmmitter();
	~ParticleEmmitter();
	
	// Updates emmitter lifetime and returns true if its time to spawn a particle
	bool Update(float dt);

	void Reset(); 
	bool isActive() const; 

	void DebugDrawEmmitter();
	// Get an initial position and an initial velocity given the emmiter type
	void GetInitialValues(float3& position, float3& velocity, float speed);

	// ----- Modify Emmitter -----

	void SetShape(Emmitter_Shape shape);
	void SetBillboardType(BillboardType type);
	void SetMaxLife(float maxLife); 
	void SetSpawnRate(float spawnRate);
	void SetDelay(float delay);
	void SetLoop(bool isLoop);

	void SetPosition(float3 position);
	void SetRotation(Quat rotation);
	void SetRotation(float3 rotation);
	void SetScale(float3 scale);

	// ---------------------------

	// ----- Get Emmitter -----

	Emmitter_Shape GetShape() const;
	BillboardType GetBillboardType() const; 
	float GetMaxLife() const;
	float GetCurrentLife() const; 
	float GetSpawnRate() const;
	float GetDelay() const;
	bool GetLoop() const;

	float3 GetPosition() const; 
	float3 GetRotation() const; 
	float3 GetScale() const; 

	// ---------------------------

public: 

	ParticleSystem* particleSystem = nullptr; 

private: 


	Emmitter_Shape shape = Emmitter_Shape::Sphere; 
	BillboardType bbtype; 

	Timer spawnTimer; 
	float currentSpawnTime = 0.f; 
	float spawnRate = 0.05f; 

	float maxLifeTime = 10.f; 
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