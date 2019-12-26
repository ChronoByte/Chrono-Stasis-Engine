#pragma once
#include "csGlobals.h"
#include <vector>

#include "Particle.h"
#include "ParticleEmmitter.h"

#define MAX_PARTICLES 10000

class ParticleSystem
{

public: 

	ParticleSystem();
	~ParticleSystem();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void CreateParticle(float3 position, float3 speed);
	void DrawParticles(); 
	void DrawEmmitter(); 

	void ResetSystem(); 

public: 

	ParticleEmmitter emmitter; 

private: 

	std::vector<Particle*> particles; 
	uint totalParticles = 0u;

};