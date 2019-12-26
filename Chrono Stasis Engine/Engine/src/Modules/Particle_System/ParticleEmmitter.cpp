#include "ParticleEmmitter.h"

ParticleEmmitter::ParticleEmmitter(ParticleSystem * owner)
{
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
	if (spawnTimer.Read() > spawnRate)
	{
		// Spawn ?
		spawnTimer.Start();
	}

	return true;
}
