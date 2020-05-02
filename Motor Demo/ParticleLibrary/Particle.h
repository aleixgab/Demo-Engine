#ifndef __Particle_H__
#define __Particle_H__

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

#define PI 3.14159265359
#define MAX_UINT32 4294967295

typedef unsigned int uint;

class ParticleStartValues;
class Emitter;
struct ParticleColor;
struct ParticleAnimation;
struct DrawInfo;

//Transform of the particle where there are Translation - Rotation - Scale
struct PartTransform
{
	//global position
	glm::vec3 position = glm::vec3(0.0f);
	//Plane rotation
	float angle = 0.0f;
	//global scale
	float scale = 1.0f;
};

class Particle
{
public:

	//Set all the particle values and start the particle
	void SetParticleValues(glm::vec3 pos, ParticleStartValues values, ParticleAnimation animation, Emitter* owner);
	bool Update(float dt);
	//We safe the distance between the camera and the particle to order by distance after this
	void SaveCameraDistance(glm::vec3 cameraPosition);
	glm::vec4 GetTextureCoords() const;
	glm::vec4 GetColor() const;
	void GetTransform(glm::vec3& pos, float& angle, float& scale) const;

	bool operator<(const Particle& particle) const
	{
		return cameraDist > particle.cameraDist;
	}
private:

	//Create random float betweet 2 edges
	float CreateRandomNum(glm::vec2 edges);

public:
	//Bool to know if the particle is alive and operating
	bool isActive = false;
	//Pointer to its own parent
	Emitter* owner = nullptr;
	//Float to know the distance between camera and this particle
	float cameraDist = 0.0f;

private:
	//Time in seconds that the particle will be alive
	float currLife, initialLife = 0.0f;

	//Velocity of the particle
	float speed = 0.0f;
	//Acceleration of the particle
	float acceleration = 0.0f;

	//Gravity of the world that affect the particle
	glm::vec3 gravity = glm::vec3(0.0f, 0.0f, 0.0f);
	//Direction of the particle in 3D
	glm::vec3 direction = glm::vec3(0.0f, 1.0f, 0.0f);

	//Velocity of the rotation
	float angularVelocity = 0.0f;
	//Acceleration of the rotation
	float angularAcceleration = 0.0f;

	//Increase(or decrease if it's negative) of the size of the particle
	float sizeOverTime = 0.0f;

	std::vector<ParticleColor> color;
	bool isMulticolor = false;
	uint index = 0u;

	glm::vec4 finalColor = glm::vec4(1.0f);
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
	glm::vec2 currMinUVCoord = glm::vec2(0.0f);
};

#endif