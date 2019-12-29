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

		if (emmitter.isActive() && emmitter.hasToBurst())
			BurstParticles();
		
	}

	// ------------------------------------------


	// --------------- Update Particles -----------------
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
			totalParticles--;
		}
		else
			iter++; 
	}

	// ------------------ ORDER PARTICLES NEAR TO FAR ------------------------

	std::sort(particles.begin(), particles.end(), compareParticles);

	/*for (uint i = 0; i < particles.size(); ++i)
	{
		float distance = mainCamera->GetPos().DistanceSq(particles[i]->GetPosition());
		sortedParticles[distance] = particles[i];
	}*/

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
	// Debugging drawing points in particles Position
	//DrawPointsForParticles();

	ComponentCamera* mainCamera = App->scene->GetMainCameraSafe();
	//-------------------------- DRAW PARTICLES FAR TO NEAR ------------------

	for (std::vector<Particle*>::reverse_iterator iter = particles.rbegin(); iter != particles.rend(); ++iter)
	{
		(*iter)->Orientate(mainCamera); 
		(*iter)->Draw();
	}
	//-------------------------- DRAW PARTICLES FAR TO NEAR ------------------

	/*for (std::map<float, Particle*>::reverse_iterator iter = sortedParticles.rbegin(); iter != sortedParticles.rend(); ++iter)
	{
		iter->second->Orientate(mainCamera);
		iter->second->Draw();	
	}*/

	//sortedParticles.clear();
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

bool ParticleSystem::isSystemActive() const
{
	return !particles.empty() || emmitter.isActive();
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

bool compareParticles(Particle * a, Particle * b)
{
	return a->GetPosition().DistanceSq(App->scene->GetMainCameraSafe()->GetPos()) < b->GetPosition().DistanceSq(App->scene->GetMainCameraSafe()->GetPos());
}
