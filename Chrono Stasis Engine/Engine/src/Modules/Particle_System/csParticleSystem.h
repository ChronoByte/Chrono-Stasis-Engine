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

	void CreateParticle(ParticleInfo info, ParticleMutableInfo startInfo, ParticleMutableInfo endInfo);
	void DrawParticles();
	void DrawPointsForParticles();

	void DrawEmmitter(); 

	void ResetSystem(); 

public: 

	ParticleEmmitter emmitter; 

	ParticleInfo particleInfo;
	ParticleMutableInfo startInfo;
	ParticleMutableInfo endInfo;

private: 

	std::vector<Particle*> particles; 
	uint totalParticles = 0u;

};