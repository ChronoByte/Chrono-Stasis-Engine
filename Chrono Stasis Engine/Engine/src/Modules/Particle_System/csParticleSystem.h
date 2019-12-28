#pragma once
#include "csGlobals.h"
#include <vector>
#include "Billboard.h"

#include "Particle.h"
#include "ParticleEmmitter.h"

#define MAX_PARTICLES 10000
#define MAX_PARTICLES_TO_BURST 500

class ParticleSystem
{

public: 

	ParticleSystem();
	~ParticleSystem();

	bool PreUpdate(float dt);
	bool Update(float dt);
	bool PostUpdate(float dt);

	void CreateParticle(ParticleInfo info, ParticleMutableInfo startInfo, ParticleMutableInfo endInfo);
	void BurstParticles();
	void DrawParticles();
	void DrawPointsForParticles();

	void DrawEmmitter(); 

	void SetBillboardType(BillboardType type);
	BillboardType GetBillboardType() const;

	void ResetSystem(); 

public: 

	ParticleEmmitter emmitter; 
	BillboardType bbType = BillboardType::SCREEN;

	ParticleInfo particleInfo;
	ParticleMutableInfo startInfo;
	ParticleMutableInfo endInfo;

	uint textureId = 8u; // Hardcoded for the moment

private: 

	std::vector<Particle*> particles; 
	uint totalParticles = 0u;
public:
	ResourceTexture* resMat = nullptr;
};