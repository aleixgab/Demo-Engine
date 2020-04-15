#ifndef __Particle_H__
#define __Particle_H__

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#define PI 3.14159265359
typedef unsigned int uint;

class ParticleStartValues;
class Emitter;

struct PlaneInfoOGL
{
	uint vao = 0u;
	uint ibo = 0u;
	uint indicesCount = 0u;
};

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
	bool CreateParticle(glm::vec3 pos, ParticleStartValues values);
	bool Update(float dt);
	//Draw function
	void Draw(uint shaderProgramUuid, glm::mat4 viewProjMatrix, PlaneInfoOGL plane);
	//We safe the distance between the camera and the particle to order by distance after this
	void SaveCameraDistance(glm::vec3 cameraPosition);
private:
	/*We rotate the plane to force it to be always in parallel of camera view
	  This function needs the vector UP(y axis) and FRONT(z axis) of the camera that we save it previously
	*/
	void LookAtCamera();
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
	float lifeTime = 0.0f;

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

	glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	PartTransform transform;
};

#endif