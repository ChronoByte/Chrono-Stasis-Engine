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

void Particle::Orientate()
{
	switch (owner->emmitter.GetBillboardType())
	{
		case BillboardType::SCREEN:
		{
			float4x4 viewMatrix = App->camera->GetViewMatrix();

			float3x3 rot = float3x3(viewMatrix.WorldX(), viewMatrix.WorldY(), viewMatrix.WorldZ());
			rotation = rot.ToQuat();
		}
			break;

		case BillboardType::WORLD:

			break;

		case BillboardType::AXIS:

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
