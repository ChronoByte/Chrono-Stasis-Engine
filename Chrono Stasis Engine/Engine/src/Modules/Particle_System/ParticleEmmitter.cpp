#include "ParticleEmmitter.h"
#include "csParticleSystem.h"

#include "GL/gl.h"
ParticleEmmitter::ParticleEmmitter() : particleSystem()
{
	spawnTimer.Start();

	// Seed
	entropy_getbytes((void*)seeds, sizeof(seeds));
	pcg32_srandom_r(&rng, seeds[0], seeds[1]);
}

ParticleEmmitter::~ParticleEmmitter()
{
}

bool ParticleEmmitter::Update(float dt)
{
	lifeTime += dt;
	currentSpawnTime += dt;

	if (loop && lifeTime >= maxLifeTime)
		Reset();

	if (isActive() && currentSpawnTime >= spawnRate)
	{
		LOG("Spawning particle");
		spawnTimer.Start();
		currentSpawnTime = 0.f;
		return true;
	}

	return false;
}

void ParticleEmmitter::DebugDrawEmmitter()
{
	switch (shape)
	{
	case Emmitter_Shape::Sphere:
		DrawSphere(radius, 10, 10);
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

		switch (zone)
		{
		case Emmitter_Zone::Base:
			position = GetWorldPosition();
			velocity = float3::RandomDir(lcg, 1.0f) * speed;
			break;

		case Emmitter_Zone::Volume:
			// Gets a random position inside the sphere
			position = GetWorldPosition() + float3::RandomDir(lcg, 1.0f) * pcg32_boundedrand_r(&rng_bounded, (radius - 0) + 1); 
			// Calculates the direction respect the center of the sphere
			velocity = (position - GetWorldPosition()).Normalized() * speed;
			break; 
		}

		break;

	case Emmitter_Shape::Hemisphere:

		break;

	case Emmitter_Shape::Cube:
		position = GetWorldPosition();

		break;

	case Emmitter_Shape::Cone:
	{
		math::Circle baseCircle = math::Circle(GetWorldPosition(), rotation.WorldZ(), radius);
		math::Circle outerCircle = math::Circle(GetWorldPosition() + rotation.WorldZ() * distance, rotation.WorldZ(), outRadius);

		// (baseCircle.RandomPointInside(lcg) -> Gets a random point in the surface of the circle 
		//position = (baseCircle.RandomPointInside(lcg) - GetWorldPosition()).Normalized() * pcg32_boundedrand_r(&rng_bounded, (radius - 0) + 1);

		// For the moment lets emmit from the center point of the emmitter
		position = GetWorldPosition();

		float3 outerCircleRandomPoint = (outerCircle.RandomPointInside(lcg) - outerCircle.CenterPoint()).Normalized() * pcg32_boundedrand_r(&rng_bounded, (outRadius - 0) + 1);
		velocity = (position - outerCircleRandomPoint).Normalized() * speed;
	}
		break;

	case Emmitter_Shape::Plane:
		position = GetWorldPosition();

		break;

	default:
		break;
	}


}

bool ParticleEmmitter::hasToBurst() const
{
	return burst.active && !burst.hasBursted && lifeTime >= burst.timeToBurst;
}

void ParticleEmmitter::DrawSphere(double r, int lats, int longs)
{
	for (int i = 0; i <= lats; i++) {
		double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = M_PI * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		glPushMatrix();
		glMultMatrixf((GLfloat*) & (float4x4::FromTRS(position, rotation, float3(1.f, 1.f, 1.f)).Transposed()));
		glColor3f(0.f, 1.f, 1.f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= longs; j++) {
			double lng = 2 * M_PI * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(r * x * zr0, r * y * zr0, r * z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(r * x * zr1, r * y * zr1, r * z1);
		}
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(1.f, 1.f, 1.f);
		glPopMatrix();
	}	
}

void ParticleEmmitter::Reset()
{
	lifeTime = 0.f; 
	burst.hasBursted = false; 
}

bool ParticleEmmitter::isActive() const
{
	return lifeTime < maxLifeTime;
}


// ----------------- Modify Emmitter -----------------

void ParticleEmmitter::SetShape(Emmitter_Shape shape)
{
	this->shape = shape;
}

void ParticleEmmitter::SetRadius(float radius)
{
	this->radius = radius;
}

void ParticleEmmitter::SetOutRadius(float radius)
{
	this->outRadius = radius; 
}

void ParticleEmmitter::SetMaxLife(float maxLife)
{
	this->maxLifeTime = maxLife;

	if (burst.timeToBurst > maxLifeTime)
		burst.timeToBurst = maxLifeTime;
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

float ParticleEmmitter::GetRadius() const
{
	return radius;
}

float ParticleEmmitter::GetOutRadius() const
{
	return outRadius;
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

