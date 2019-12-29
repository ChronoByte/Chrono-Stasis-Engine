#pragma once
#include "csGlobals.h"
#include <vector>
#include "Billboard.h"

#include "Particle.h"
#include "ParticleEmmitter.h"
#include <map>

#define MAX_PARTICLES 10000
#define MAX_PARTICLES_TO_BURST 500

enum class EquationBlendType
{
	FUNC_ADD,
	FUNC_SUBTRACT,
	FUNC_REVERSE_SUBTRACT
};

enum class FunctionBlendType
{
	ZERO,
	ONE,
	SRC_COLOR,
	ONE_MINUS_SRC_COLOR,
	DST_COLOR,
	ONE_MINUS_DST_COLOR,
	SRC_ALPHA,
	ONE_MINUS_SRC_ALPHA,
	DST_ALPHA,
	ONE_MINUS_DST_ALPHA,
	CONSTANT_COLOR,
	ONE_MINUS_CONSTANT_COLOR,
	CONSTANT_ALPHA,
	ONE_MINUS_CONSTANT_ALPHA
	
};

bool compareParticles(Particle* a, Particle* b);

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
	uint GetTotalParticles() const;

	bool isSystemActive() const; 
	void ResetSystem(); 

public: 

	ParticleEmmitter emmitter; 
	BillboardType bbType = BillboardType::SCREEN;

	ParticleInfo particleInfo;
	ParticleMutableInfo startInfo;
	ParticleMutableInfo endInfo;

	bool isPlaying = true; 

private: 

	//std::map<float, Particle*> sortedParticles;
	std::vector<Particle*> particles; 
	uint totalParticles = 0u;

public:
	ResourceTexture* resMat = nullptr;
	EquationBlendType eqBlend = EquationBlendType::FUNC_ADD;
	FunctionBlendType funcBlendSource = FunctionBlendType::SRC_ALPHA;
	FunctionBlendType funcBlendDest = FunctionBlendType::ONE_MINUS_SRC_ALPHA;

	int sourceFactor;
	int destinationFactor;

};