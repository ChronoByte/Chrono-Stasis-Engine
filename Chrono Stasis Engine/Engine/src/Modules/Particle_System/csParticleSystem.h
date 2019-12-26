#pragma once
#include "csGlobals.h"
#include <vector>

#include "Particle.h"
#include "ParticleEmmitter.h"

class ParticleSystem
{
	friend class ParticleEmmitter;

public: 

	ParticleSystem();
	~ParticleSystem();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void DrawParticles(); 

private: 

	std::vector<Particle*> particles; 
	ParticleEmmitter* emmitter = nullptr; 
	uint totalParticles = 0u;

};