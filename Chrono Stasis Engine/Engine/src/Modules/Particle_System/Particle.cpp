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
	particleInfo.velocity += particleInfo.force * dt;

	// Move
	particleInfo.position += particleInfo.velocity * dt;
}

void Particle::PostUpdate(float dt)
{
	// Interpolate ?
}

void Particle::Draw()
{
	glColor4f(particleInfo.color.x, particleInfo.color.y, particleInfo.color.z, particleInfo.color.w);
	//glColor4f(1.f, 1.f, 1.f, 1.f);

	glPushMatrix();
	glMultMatrixf((GLfloat*) & (float4x4::FromTRS(particleInfo.position, particleInfo.rotation, float3(1.f, 1.f, 1.f)).Transposed()));


	if (owner->resMat != nullptr)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(1.0f, 1.0f, 1.0f, owner->resMat->transparency);
		glEnable(GL_ALPHA_TEST);
		//glAlphaFunc(GL_GREATER, owner->resMat->transparency);
		glBindTexture(GL_TEXTURE_2D, owner->resMat->gpu_id);
	}
	else
		glBindTexture(GL_TEXTURE_2D, 2);
	glBegin(GL_TRIANGLES);

	// -------------------

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.5f * particleInfo.size, -0.5f * particleInfo.size, 0.f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f * particleInfo.size, 0.5f * particleInfo.size, 0.f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-0.5f * particleInfo.size, 0.5f * particleInfo.size, 0.f);


	// -------------------

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-0.5f * particleInfo.size, -0.5f * particleInfo.size, 0.f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(0.5f * particleInfo.size, -0.5f * particleInfo.size, 0.f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0.5f * particleInfo.size, 0.5f * particleInfo.size, 0.f);

	// -------------------

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	glColor4f(1.f, 1.f, 1.f, 1.f);

}

void Particle::Orientate(ComponentCamera * camera)
{
	switch (owner->GetBillboardType())
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
