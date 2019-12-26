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


// ----------------- Modify Emmitter -----------------

void ParticleEmmitter::SetShape(Emmitter_Shape shape)
{
	this->shape = shape;
}

void ParticleEmmitter::SetMaxLife(float maxLife)
{
	this->maxLifeTime = maxLife;
}

void ParticleEmmitter::SetSpawnRate(float spawnRate)
{
	this->spawnRate = spawnRate;
}

void ParticleEmmitter::SetDelay(float delay)
{
	this->delay = delay;
}

void ParticleEmmitter::SetLoop(bool isLoop)
{
	loop = isLoop;
}

void ParticleEmmitter::SetPosition(float3 position)
{
	this->position = position;
}

void ParticleEmmitter::SetRotation(Quat rotation)
{
	this->rotation = rotation;
}

void ParticleEmmitter::SetRotation(float3 rotation)
{
	this->rotation = Quat::FromEulerXYZ(rotation.x * DEGTORAD, rotation.y * DEGTORAD, rotation.z * DEGTORAD);
}

void ParticleEmmitter::SetScale(float3 scale)
{
	this->scale = scale;
}

// --------------------------------------------------


Emmitter_Shape ParticleEmmitter::GetShape() const
{
	return shape;
}

float ParticleEmmitter::GetMaxLife() const
{
	return maxLifeTime;
}

float ParticleEmmitter::GetSpawnRate() const
{
	return spawnRate;
}

float ParticleEmmitter::GetDelay() const
{
	return delay;
}

bool ParticleEmmitter::GetLoop() const
{
	return loop;
}

float3 ParticleEmmitter::GetPosition() const
{
	return position;
}

float3 ParticleEmmitter::GetRotation() const
{
	return rotation.ToEulerXYZ() * RADTODEG;
}

float3 ParticleEmmitter::GetScale() const
{
	return scale;
}

