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
	switch (shape)
	{
	case Emmitter_Shape::Sphere:

		break;

	case Emmitter_Shape::Hemisphere:

		break;

	case Emmitter_Shape::Cube:

		break;

	case Emmitter_Shape::Cone:

		break;

	case Emmitter_Shape::Plane:

		break;

	default:
		break;
	}
}

void ParticleEmmitter::GetInitialValues(float3 & position, float3 & velocity, float speed)
{
	math::LCG lcg; 	// Maybe here we could use some pcg ol' tricks of yours

	switch (shape)
	{
	case Emmitter_Shape::Sphere:
		position = GetWorldPosition();
		velocity = float3::RandomDir(lcg, 1.0f) * speed;
		break;

	case Emmitter_Shape::Hemisphere:

		break;

	case Emmitter_Shape::Cube:
		position = GetWorldPosition();

		break;

	case Emmitter_Shape::Cone:
		position = GetWorldPosition();

		break;

	case Emmitter_Shape::Plane:
		position = GetWorldPosition();

		break;

	default:
		break;
	}


}

bool ParticleEmmitter::Update(float dt)
{
	lifeTime += dt; 
	currentSpawnTime += dt; 

	if (isActive() && currentSpawnTime >= spawnRate)
	{
		LOG("Spawning particle");
		spawnTimer.Start();
		currentSpawnTime = 0.f;
		return true; 
	}

	return false;
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
	this->spawnRate = 1.f / spawnRate;
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

void ParticleEmmitter::SetRelativePosition(float3 position)
{
	relativePosition = position; 
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

float ParticleEmmitter::GetCurrentLife() const
{
	return lifeTime;
}

float ParticleEmmitter::GetSpawnRate() const
{
	return 1.f / spawnRate;
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

float3 ParticleEmmitter::GetRelativePosition() const
{
	return relativePosition;
}

float3 ParticleEmmitter::GetWorldPosition() const
{
	return position + relativePosition;
}

float3 ParticleEmmitter::GetRotation() const
{
	return rotation.ToEulerXYZ() * RADTODEG;
}

float3 ParticleEmmitter::GetScale() const
{
	return scale;
}

