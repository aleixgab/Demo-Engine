#include "Particle.h"
#include "Emitter.h"
#include "ParticleManager.h"
#include "PlaneImporter.h"

#include <glad/glad.h>
#include <random>

#include <Brofiler/Brofiler.h>

void Particle::SetParticleValues(PartVec3 pos, ParticleStartValues values, Emitter* owner)
{
	this->owner = owner;

	//Save all the initial values 
	initialLife = currLife = CreateRandomNum(values.life);
	speed = CreateRandomNum(values.speed);
	gravity = values.gravity;
	acceleration = CreateRandomNum(values.acceleration);
	direction = values.particleDirection;
	angularVelocity = CreateRandomNum(values.angularVelocity) * (PI / 180.0f);
	angularAcceleration = CreateRandomNum(values.angularAcceleration) * (PI / 180.0f);
	sizeOverTime = CreateRandomNum(values.sizeOverTime);

	transform.angle = CreateRandomNum(values.rotation) * (PI / 180.0f);
	transform.position = pos;
	transform.scale = CreateRandomNum(values.size);

	isActive = true;
}

void Particle::Update(float dt)
{
	if (!owner->runningTime)
		dt = 0.0f;

	if (currLife > 0.0f)
	{
		
		//Tranlate
		speed += acceleration * dt;
		transform.position += (direction * speed * dt);

		//Scale
		transform.scale += sizeOverTime * dt;

		//Rotation
		angularVelocity += angularAcceleration * dt;
		transform.angle += angularVelocity * dt;

		currLife -= dt;
	}
	else
	{
		//Deactivate the particle
		isActive = false;
		owner->particles.remove(this);
		owner->parent->numActivePart--;
	}
}

PartVec2 Particle::GetCurrLife() const
{
	return PartVec2(currLife,initialLife);
}

void Particle::GetTransform(PartVec3& pos, float& angle, float& scale ) const
{
	pos = transform.position;
	angle = transform.angle;
	scale = transform.scale;
}

float Particle::CreateRandomNum(PartVec2 edges)//.x = minPoint & .y = maxPoint
{
	float num = edges.x;
	if (edges.x < edges.y)
	{
		num = owner->parent->GetRandomNum(edges.x, edges.y);
	}
	return num;
}
