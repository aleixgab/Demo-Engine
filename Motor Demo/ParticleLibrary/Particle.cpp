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
	initialLife = CreateRandomNum(values.life);
	currLife = 0.0f;
	speed = CreateRandomNum(values.speed);
	gravity = values.gravity;
	acceleration = CreateRandomNum(values.acceleration);
	direction = values.particleDirection;
	angularVelocity = CreateRandomNum(values.angularVelocity);
	angularAcceleration = CreateRandomNum(values.angularAcceleration);
	sizeOverTime = CreateRandomNum(values.sizeOverTime);

	transform.angle = CreateRandomNum(values.rotation);
	transform.position = pos;
	transform.scale = CreateRandomNum(values.size);

	isActive = true;
}

void Particle::Update(float dt)
{
	if (!owner->runningTime)
		dt = 0.0f;

	if (currLife < initialLife)
		currLife += dt;
	
	else
	{
		//Deactivate the particle
		isActive = false;
		owner->particles.remove(this);
	}
}

PartVec2 Particle::GetCurrLife() const
{
	return PartVec2(currLife,initialLife);
}

void Particle::GetTransform(PartVec3& initialPos, PartVec3& direction, float& speed, float& acceleration, float& angle, float& angleVel, float& angleAccel, float& scale, float& scaleTime) const
{
	initialPos = transform.position;
	direction = this->direction;
	speed = this->speed;
	acceleration = this->acceleration;
	angle = transform.angle;
	angleVel = angularVelocity;
	angleAccel = angularAcceleration;
	scale = transform.scale;
	scaleTime = sizeOverTime;
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
