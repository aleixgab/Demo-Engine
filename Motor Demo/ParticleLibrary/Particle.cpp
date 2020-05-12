#include "Particle.h"
#include "Emitter.h"
#include "ParticleManager.h"
#include "PlaneImporter.h"

#include <glad/glad.h>
#include <random>

#include <Brofiler/Brofiler.h>

void Particle::SetParticleValues(PartVec3 pos, ParticleStartValues values, Emitter* owner)
{
	runningTime = &owner->runningTime;

	//Save all the initial values 
	initialLife = CreateRandomNum(values.life, owner);
	currLife = 0.0f;
	speed = CreateRandomNum(values.speed, owner);
	gravity = values.gravity;
	acceleration = CreateRandomNum(values.acceleration, owner);
	direction = values.particleDirection;
	angularVelocity = CreateRandomNum(values.angularVelocity, owner);
	angularAcceleration = CreateRandomNum(values.angularAcceleration, owner);
	sizeOverTime = CreateRandomNum(values.sizeOverTime, owner);

	angle = CreateRandomNum(values.rotation, owner);
	initialPos = pos;
	scale = CreateRandomNum(values.size, owner);

	isActive = true;
}

bool Particle::Update(float dt)
{
	if (!(*runningTime))
		dt = 0.0f;

	if (currLife < initialLife)
		currLife += dt;
	
	else
	{
		//Deactivate the particle
		isActive = false;
	}
	return isActive;
}

PartVec2 Particle::GetCurrLife() const
{
	return PartVec2(currLife,initialLife);
}

void Particle::GetTransform(PartVec3& initialPos, PartVec3& direction, float& speed, float& acceleration, float& angle, float& angleVel, float& angleAccel, float& scale, float& scaleTime) const
{
	direction = this->direction;
	speed = this->speed;
	acceleration = this->acceleration;
	angleVel = angularVelocity;
	angleAccel = angularAcceleration;
	scaleTime = sizeOverTime;

	initialPos = this->initialPos;
	angle = this->angle;
	scale = this->scale;
}

float Particle::CreateRandomNum(PartVec2 edges, Emitter* owner)//.x = minPoint & .y = maxPoint
{
	float num = edges.x;
	if (edges.x < edges.y)
	{
		num = owner->parent->GetRandomNum(edges.x, edges.y);
	}
	return num;
}
