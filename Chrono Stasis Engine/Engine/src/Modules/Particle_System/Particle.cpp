#include "Particle.h"

Particle::Particle()
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
	// Move
}

void Particle::PostUpdate(float dt)
{
	// Interpolate ?
}