#include "csParticleSystem.h"
#include "csApp.h"
#include "ComponentCamera.h"
#include "csCamera3D.h"

#include "GL/gl.h"

ParticleSystem::ParticleSystem()
{
	particles.reserve(MAX_PARTICLES);
	emmitter.particleSystem = this; 
}

ParticleSystem::~ParticleSystem()
{
	// Delete all particles
	for (std::vector<Particle*>::iterator iter = particles.begin(); iter != particles.end(); ++iter)
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
	// --------------- Emmitter -----------------

	if (emmitter.isActive()) // If its active, update it, and check if it has to spawn
	{
		if (emmitter.Update(dt)) // if its time to spawn a particle
		{
			emmitter.GetInitialValues(particleInfo.position, particleInfo.velocity, particleInfo.speed);
			CreateParticle(particleInfo, startInfo, endInfo);
		}

		if (emmitter.hasToBurst())
			BurstParticles();
		
	}

	// ------------------------------------------


	// --------------- Update Particles -----------------
	for (uint i = 0; i < particles.size(); ++i)
	{
		particles[i]->Update(dt); 
	}

	// ------------------ FAR TO NEAR ORDER PARTICLES WITH BLEND ------------------------

	for (uint i = 0; i < particles.size(); ++i)
	{
		float distance = App->camera->fakeCamera->GetPos().DistanceSq(particles[i]->GetPosition());
		sortedParticles[distance] = particles[i];
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
			totalParticles--;
		}
		else
			iter++; 
	}

	return true;
}

void ParticleSystem::CreateParticle(ParticleInfo info, ParticleMutableInfo startInfo, ParticleMutableInfo endInfo)
{
	if (totalParticles > MAX_PARTICLES)
		return; 

	particles.push_back(new Particle(this, info, startInfo, endInfo));
	totalParticles++;
}

void ParticleSystem::BurstParticles()
{
	for (uint i = 0; i < emmitter.burst.partsToInstantiate; ++i)
	{
		emmitter.GetInitialValues(particleInfo.position, particleInfo.velocity, particleInfo.speed);
		CreateParticle(particleInfo, startInfo, endInfo);
	}
	emmitter.burst.hasBursted = true;
}

void ParticleSystem::DrawParticles()
{
	DrawPointsForParticles();

	for (std::vector<Particle*>::iterator iter = particles.begin(); iter != particles.end(); ++iter)
	{
		(*iter)->Orientate(App->camera->fakeCamera); // For the moment, we use the editor camera
		(*iter)->Draw();
	}

	//-------------------------- DRAW PARTICLES FAR TO NEAR ------------------

	/*
	for (std::map<float, Particle*>::reverse_iterator iter = sortedParticles.rbegin(); iter != sortedParticles.rend(); ++iter)
	{
		iter->second->Orientate(App->camera->fakeCamera);
		iter->second->Draw();
		
	}
	*/
}

void ParticleSystem::DrawPointsForParticles()
{
	glBegin(GL_POINTS);
	glColor3f(0.f, 0.f, 1.f);

	LOG("Drawing Particles: %i", totalParticles);
	for (std::vector<Particle*>::iterator iter = particles.begin(); iter != particles.end(); ++iter)
	{

		// Draw Particle
		float3 particlePos = (*iter)->GetPosition();
		glVertex3f(particlePos.x, particlePos.y, particlePos.z);

		// Render light if any
		(*iter)->light.Render(); // Don't know how we'll handle this in mesh mode 

	}

	glColor3f(1.f, 1.f, 1.f);
	glEnd();
}

void ParticleSystem::DrawEmmitter()
{
	emmitter.DebugDrawEmmitter(); 
}

void ParticleSystem::SetBillboardType(BillboardType type)
{
	bbType = type; 
}

BillboardType ParticleSystem::GetBillboardType() const
{
	return bbType;
}

void ParticleSystem::ResetSystem()
{
	// Reset Emmitter
	emmitter.Reset(); 

	// Delete all particles
	for (std::vector<Particle*>::iterator iter = particles.begin(); iter != particles.end(); ++iter)
	{
		delete (*iter);
		(*iter) = nullptr;
	}

	particles.clear();
}
