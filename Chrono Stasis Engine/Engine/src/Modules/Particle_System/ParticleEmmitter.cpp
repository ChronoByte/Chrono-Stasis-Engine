#include "ParticleEmmitter.h"
#include "csParticleSystem.h"

ParticleEmmitter::ParticleEmmitter() : particleSystem()
{
	spawnTimer.Start();
}

ParticleEmmitter::~ParticleEmmitter()
{
}

void ParticleEmmitter::DebugDrawEmmitter()
{

}

void ParticleEmmitter::ChangeShape(Emmitter_Shape shape)
{
}

bool ParticleEmmitter::Update(float dt)
{
	lifeTime += dt; 

	if (spawnTimer.Read() >= spawnRate)
	{
		// Spawn ?
		LOG("Spawning particle");
		particleSystem->CreateParticle(position, float3(0, 1, 0));
		spawnTimer.Start();
	}

	return true;
}

void ParticleEmmitter::Reset()
{
	lifeTime = 0.f; 
}

bool ParticleEmmitter::isActive() const
{
	return lifeTime < maxLifeTime || loop;
}
