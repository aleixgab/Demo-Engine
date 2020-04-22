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
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	//Plane rotation = Quaternion identity
	glm::quat rotation = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
	//global scale
	glm::vec3 scale = glm::vec3(1.0f,1.0f,1.0f);

	//Function to join position, rotation and scale in one matrix 4x4
	glm::mat4 GetMatrix() const;
};

class Particle
{
public:

	//Set all the particle values and start the particle
	void CreateParticle(glm::vec3 pos, ParticleStartValues values, ParticleAnimation animation, Emitter* owner);
	bool Update(float dt);
	//Draw function with shaderUuid, and camera view and projection
	void Draw(uint uuid, glm::mat4 viewMatrix, glm::mat4 projMatrix);
	//We safe the distance between the camera and the particle to order by distance after this
	void SaveCameraDistance(glm::vec3 cameraPosition);
	DrawInfo GetDrawInfo() const;
	glm::vec4 GetTexture() const;
	glm::vec4 GetColor() const;
	glm::mat4 GetTransform() const;
private:
	/*We rotate the plane to force it to be always in parallel of camera view
	  This function needs the vector UP(y axis) and FRONT(z axis) of the camera that we save it previously
	  Return false if the user doesn't save the camera pointers
	*/
	bool LookAtCamera();
	//Create random float betweet 2 edges
	float CreateRandomNum(glm::vec2 edges);

public:
	//Bool to know if the particle is alive and operating
	bool isActive = false;
	//Pointer to its own parent
	Emitter* owner = nullptr;
	//Float to know the distance between camera and this particle
	float cameraDist = 0.0f;

	//Say if the texture is animated or not
	bool isParticleAnimated = false;
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

	//Inclination of the particle
	float angle = 0.0f;
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