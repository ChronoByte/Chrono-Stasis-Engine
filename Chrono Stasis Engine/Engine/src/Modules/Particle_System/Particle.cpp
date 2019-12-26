#include "Particle.h"
#include "csParticleSystem.h"
#include "ComponentBillboard.h"
#include "csApp.h"
#include "csCamera3D.h"
#include "GL/gl.h"

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
	//speed += currentForce * dt; 

	// Move
	position += speed * dt; 
}

void Particle::PostUpdate(float dt)
{
	// Interpolate ?
}

void Particle::Draw()
{
	glColor4f(currentColor.x, currentColor.y, currentColor.z, currentColor.w);

	glPushMatrix();
	glMultMatrixf((GLfloat*) & (float4x4::FromTRS(position, rotation, float3(1.f,1.f,1.f)).Transposed()));

	glBegin(GL_TRIANGLES);

	glVertex3f(-0.5f * currentSize, -0.5f * currentSize, 0.f);
	glVertex3f(0.5f * currentSize, 0.5f * currentSize, 0.f);
	glVertex3f(-0.5f * currentSize, 0.5f * currentSize, 0.f);

	glVertex3f(-0.5f * currentSize, -0.5f * currentSize, 0.f);
	glVertex3f(0.5f * currentSize, -0.5f * currentSize, 0.f);
	glVertex3f(0.5f * currentSize, 0.5f * currentSize, 0.f);

	glEnd(); 

	glPopMatrix(); 
	glColor4f(1.f, 1.f, 1.f, 1.f);

}

void Particle::Orientate(ComponentCamera * camera)
{
	switch (owner->emmitter.GetBillboardType())
	{
	case BillboardType::SCREEN:
		rotation = Billboard::AlignToScreen(camera);
		break;

	case BillboardType::WORLD:
		rotation = Billboard::AlignToWorld(camera, position);
		break;

	case BillboardType::AXIS:
		rotation = Billboard::AlignToAxis(camera, position);

		break;

	case BillboardType::NONE:

		break;

	default:
		break;
	}
}


float3 Particle::GetPosition() const
{
	return position;
}
