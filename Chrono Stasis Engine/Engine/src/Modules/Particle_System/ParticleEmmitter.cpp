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
		//LOG("Spawning particle");
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
		DrawCone();
		break;

	case Emmitter_Shape::Plane:

		break;

	default:
		break;
	}
}

void ParticleEmmitter::GetInitialValues(float3 & position, float3 & velocity, float speed,  bool globalTransform)
{
	math::LCG lcg; 	// Maybe here we could use some pcg ol' tricks of yours

	// Set the spawn position
	// If its not on local space (global space) instantiate it on the emitter position
	// If its in local space, then instantiate it in pos 0,0,0
	float3 spawnPosition = float3::zero;
	if (globalTransform)
		 spawnPosition = GetWorldPosition();

	switch (shape)
	{
	case Emmitter_Shape::Sphere:

		switch (zone)
		{
		case Emmitter_Zone::Base:
			position = spawnPosition;
			velocity = float3::RandomDir(lcg, 1.0f) * speed;
			break;

		case Emmitter_Zone::Volume:
			// Gets a random position inside the sphere
			position = spawnPosition + float3::RandomDir(lcg, 1.0f) * GetRandomBetween(0, radius);
			// Calculates the direction respect the center of the sphere
			velocity = (position - spawnPosition).Normalized() * speed;
			break; 
		}

		break;

	case Emmitter_Shape::Hemisphere:

		break;

	case Emmitter_Shape::Cube:
		position = spawnPosition;

		break;

	case Emmitter_Shape::Cone:
	{
		math::Circle baseCircle = math::Circle(spawnPosition, rotation.WorldZ(), radius);
		math::Circle outerCircle = math::Circle(spawnPosition, rotation.WorldZ(), outRadius);
		// (baseCircle.RandomPointInside(lcg) -> Gets a random point in the surface of the circle 
		//position = (baseCircle.RandomPointInside(lcg) - GetWorldPosition()).Normalized() * pcg32_boundedrand_r(&rng_bounded, (radius - 0) + 1);
		
		
		// For the moment lets emmit from the center point of the emmitter
		position = spawnPosition;

		// Get direction respect the second circle of the cone
		float3 circleRandomPoint = (outerCircle.RandomPointInside(lcg) - spawnPosition).Normalized() * GetRandomBetween(0, outRadius);
		circleRandomPoint += spawnPosition; // Move it to the emmitter location, otherwise its on the Origin.
		circleRandomPoint += rotation.WorldZ() * distance; // Move the point forward
		velocity = (circleRandomPoint - position).Normalized() * speed;  // Finally make the initial velocity vector
	}
		break;

	case Emmitter_Shape::Plane:
		position = spawnPosition;

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
		glMultMatrixf((GLfloat*) & (float4x4::FromTRS(GetWorldPosition(), rotation, float3(1.f, 1.f, 1.f)).Transposed()));
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

void ParticleEmmitter::DrawCone()
{
	int n = 30; 

	//float innerRadius = radius; 
	float innerRadius = 0; // For the moment, the inner circle is always radius = 0

	glPushMatrix();
	glMultMatrixf((GLfloat*) & (float4x4::FromTRS(position, rotation, float3(1.f, 1.f, 1.f)).Transposed()));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_POLYGON);

	//// Cylinder Bottom
	for (int i = 360; i >= 0; i -= (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(innerRadius * cos(a), innerRadius * sin(a), 0);	
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for (int i = 0; i <= 360; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians
		glVertex3f(outRadius * cos(a), outRadius * sin(a), distance);
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < 480; i += (360 / n))
	{
		float a = i * M_PI / 180; // degrees to radians

		glVertex3f(outRadius * cos(a), outRadius * sin(a), distance);
		glVertex3f(innerRadius * cos(a), innerRadius * sin(a), 0.0f); 
	}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix(); 

	// Draw Spawn Points 
	/*glColor3f(0.f, 1.f, 1.f);
	glBegin(GL_POINTS);

	for (int i = 0; i < debug.size(); ++i)
		glVertex3f(debug[i].x, debug[i].y, debug[i].z);

	glEnd();
	glColor3f(1.f, 1.f, 1.f);*/

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

void ParticleEmmitter::SetZone(Emmitter_Zone zone)
{
	this->zone = zone;
}

void ParticleEmmitter::SetRadius(float radius)
{
	this->radius = radius;
}

void ParticleEmmitter::SetOutRadius(float radius)
{
	this->outRadius = radius; 
}

void ParticleEmmitter::SetDistance(float distance)
{
	this->distance = distance; 
}

void ParticleEmmitter::SetMaxLife(float maxLife)
{
	this->maxLifeTime = maxLife;

	if (burst.timeToBurst > maxLifeTime)
		burst.timeToBurst = maxLifeTime;
}

void ParticleEmmitter::SetCurrentLife(float currentLife)
{
	this->lifeTime = currentLife;
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

Emmitter_Zone ParticleEmmitter::GetZone() const
{
	return zone;
}

float ParticleEmmitter::GetRadius() const
{
	return radius;
}

float ParticleEmmitter::GetOutRadius() const
{
	return outRadius;
}

float ParticleEmmitter::GetDistance() const
{
	return distance;
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

float4x4 ParticleEmmitter::GetGlobalTransform() const
{
	return float4x4::FromTRS(position, rotation, float3::one);
}

