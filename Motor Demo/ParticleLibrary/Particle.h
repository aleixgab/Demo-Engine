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
struct Vertex;

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
	void SetParticleValues(PartVec3 pos, ParticleStartValues values, ParticleAnimation animation, Emitter* owner);
	void Update(float dt);
	void SetVertexs(Vertex* particleVertexs, PartVec3 cameraForward, PartVec3 cameraUp);

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

	std::vector<ParticleColor> color;
	bool isMulticolor = false;
	uint index = 0u;

	PartVec4 finalColor = PartVec4(1.0f);
	PartTransform transform;

	//Number of rows and columns of the texture
	int textureRows = 1;
	int textureColumns = 1;
	//Number of rows and columns of the texture between 0 and 1
	float textureRowsNorm = 1.0f;
	float textureColumnsNorm = 1.0f;

	//Counter of the time to know when we should change the animation
	float countAnimTime = 0.0f;
	//Time value to say how seconds we will stay in each sprite
	float animTime = 0.0f;

	//It Says wich cell are to know the row and column
	uint currentFrame = 0u;
	/*Counter of the cells to know when it's finish
	Diferent than currentFrmae because there are random cells and it might not goes in order
	*/
	uint contFrame = 0u;
	PartVec2 currMinUVCoord = PartVec2(0.0f, 0.0f);
};

#endif