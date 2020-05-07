#ifndef __Particle_H__
#define __Particle_H__

#include <vector>
#include "PartMath.h"

//Number PI
#define PI 3.14159265359

typedef unsigned int uint;

class Emitter;
class ParticleStartValues;
struct ParticleColor;
struct ParticleAnimation;

//Transform of the particle where there are Translation - Rotation - Scale
struct PartTransform
{
	//global position
	PartVec3 position = PartVec3(0.0f);
	//Plane rotation
	float angle = 0.0f;
	//global scale
	float scale = 1.0f;
};

class Particle
{
public:

	//Set all the particle values and start the particle
	void SetParticleValues(PartVec3 pos, ParticleStartValues values, Emitter* owner);
	void Update(float dt);
	PartVec2 GetCurrLife() const;
	void GetTransform(PartVec3& pos, float& angle, float& scale) const;

private:

	//Create random float betweet 2 edges
	float CreateRandomNum(PartVec2 edges);

public:
	//Bool to know if the particle is alive and operating
	bool isActive = false;
	//Pointer to its own parent
	Emitter* owner = nullptr;

private:
	//Time in seconds that the particle will be alive
	float currLife, initialLife = 0.0f;

	//Velocity of the particle
	float speed = 0.0f;
	//Acceleration of the particle
	float acceleration = 0.0f;

	//Gravity of the world that affect the particle
	PartVec3 gravity = PartVec3(0.0f);
	//Direction of the particle in 3D
	PartVec3 direction = PartVec3(0.0f, 1.0f, 0.0f);

	//Velocity of the rotation
	float angularVelocity = 0.0f;
	//Acceleration of the rotation
	float angularAcceleration = 0.0f;

	//Increase(or decrease if it's negative) of the size of the particle
	float sizeOverTime = 0.0f;

	PartVec4 finalColor = PartVec4(1.0f);
	PartTransform transform;
};

#endif