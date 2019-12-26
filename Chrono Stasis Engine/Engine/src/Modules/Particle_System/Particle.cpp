#include "Particle.h"

Particle::Particle(ParticleSystem * owner, float3 position, float3 speed) : owner(owner), position(position), speed(speed)
{
}

Particle::~Particle()
{
}

void Particle::PreUpdate(float dt)
{
	if (currentLifeTime > maxLifeTime)
		to_delete = true;

}

void Particle::Update(float dt)
{
	// Apply forces
	//speed += force * dt; 

	// Move
	position += speed * dt; 
}

void Particle::PostUpdate(float dt)
{
	// Interpolate ?
}

float3 Particle::GetPosition() const
{
	return position;
}
