#include "csParticleSystem.h"

ParticleSystem::ParticleSystem()
{
	// Create an emmitter
	emmitter = new ParticleEmmitter(this); 
}

ParticleSystem::~ParticleSystem()
{
	// Delete emmmitter
	if (emmitter != nullptr)
	{
		delete emmitter;
		emmitter = nullptr; 
	}

	// Delete all particles
	for (std::vector<Particle*>::iterator iter = particles.begin(); iter != particles.end();)
	{
		delete (*iter);
		(*iter) = nullptr; 
	}

	particles.clear(); 
}

bool ParticleSystem::PreUpdate(float dt)
{
	// Spawn option from particle system 

	for (uint i = 0; i < particles.size(); ++i)
	{
		particles[i]->PreUpdate(dt);
	}

	return true;
}

bool ParticleSystem::Update(float dt)
{
	for (uint i = 0; i < particles.size(); ++i)
	{
		particles[i]->Update(dt); 
	}

	return true;
}

bool ParticleSystem::PostUpdate(float dt)
{
	for (std::vector<Particle*>::iterator iter = particles.begin(); iter != particles.end();)
	{
		(*iter)->PostUpdate(dt);

		// Delete the particles that are needed
		if ((*iter)->to_delete)
		{
			delete (*iter);
			(*iter) = nullptr;
			iter = particles.erase(iter);
		}
		else
			iter++; 
	}

	return true;
}

void ParticleSystem::DrawParticles()
{
	// And Render Lights !
}
