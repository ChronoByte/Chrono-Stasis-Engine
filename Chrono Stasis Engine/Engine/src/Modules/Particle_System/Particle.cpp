#include "Particle.h"
#include "csParticleSystem.h"
#include "ComponentBillboard.h"
#include "csApp.h"
#include "csCamera3D.h"
#include "GL/gl.h"

Particle::Particle(ParticleSystem * owner, ParticleInfo info, ParticleMutableInfo startInfo, ParticleMutableInfo endInfo) : owner(owner), particleInfo(info), startInfo(startInfo), endInfo(endInfo)
{
}

Particle::~Particle()
{
}

void Particle::PreUpdate(float dt)
{
	currentLifeTime += dt;
	if (currentLifeTime > particleInfo.maxLifeTime)
		to_delete = true;

}

void Particle::Update(float dt)
{
	// Apply forces
	particleInfo.speed += particleInfo.force * dt;

	// Move
	particleInfo.position += particleInfo.speed * dt;
}

void Particle::PostUpdate(float dt)
{
	// Interpolate ?
}

void Particle::Draw()
{
	glColor4f(particleInfo.color.x, particleInfo.color.y, particleInfo.color.z, particleInfo.color.w);

	glPushMatrix();
	glMultMatrixf((GLfloat*) & (float4x4::FromTRS(particleInfo.position, particleInfo.rotation, float3(1.f,1.f,1.f)).Transposed()));

	glBegin(GL_TRIANGLES);

	glVertex3f(-0.5f * particleInfo.size, -0.5f * particleInfo.size, 0.f);
	glVertex3f(0.5f * particleInfo.size, 0.5f * particleInfo.size, 0.f);
	glVertex3f(-0.5f * particleInfo.size, 0.5f * particleInfo.size, 0.f);

	glVertex3f(-0.5f * particleInfo.size, -0.5f * particleInfo.size, 0.f);
	glVertex3f(0.5f * particleInfo.size, -0.5f * particleInfo.size, 0.f);
	glVertex3f(0.5f * particleInfo.size, 0.5f * particleInfo.size, 0.f);

	glEnd(); 

	glPopMatrix(); 
	glColor4f(1.f, 1.f, 1.f, 1.f);

}

void Particle::Orientate(ComponentCamera * camera)
{
	switch (owner->emmitter.GetBillboardType())
	{
	case BillboardType::SCREEN:
		particleInfo.rotation = Billboard::AlignToScreen(camera);
		break;

	case BillboardType::WORLD:
		particleInfo.rotation = Billboard::AlignToWorld(camera, particleInfo.position);
		break;

	case BillboardType::AXIS:
		particleInfo.rotation = Billboard::AlignToAxis(camera, particleInfo.position);

		break;

	case BillboardType::NONE:

		break;

	default:
		break;
	}
}


float3 Particle::GetPosition() const
{
	return particleInfo.position;
}
