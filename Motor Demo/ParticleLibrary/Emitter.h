#ifndef __Emitter_H__
#define __Emitter_H__

#include <list>
#include <string>

#include "Timer.h"
#include <glm/gtc/matrix_transform.hpp>

class Particle;
class ParticleManager;

struct ParticleColor
{
	glm::vec4 color = glm::vec4(1.0f);
	//position in percentage to paint correctly the colors during the time
	float position = 0.0f;
	//Name just to differentiate diferents colors
	std::string name = " ";
	//open window for change particle color
	bool changingColor = false;
	//Order the colors depending on the position
	bool operator<(const ParticleColor& color) const
	{
		return position < color.position;
	}
};

enum ShapeEmitter {
	BoxShape = 0,
	SphereShape,
	ConeShape
};

//Start values. All the values with a float2 are a possible random. They are just a float, this random will be calculated in the particle creation.
struct ParticleStartValues
{

	//The seconds that the particle will be alive
	glm::vec2 life = glm::vec2(5.0f, 5.0f);
	//The initial velocity that will get the particle
	glm::vec2 speed = glm::vec2(3.0f, 3.0f);
	//The acceleration in world coordinates that will afect the particle
	glm::vec3 gravity = glm::vec3(0.0f, 0.0f, 0.0f);
	//The acceleration that will afect the particle velocity without changing the direction
	glm::vec2 acceleration = glm::vec2(0.0f, 0.0f);
	//The initial plane size
	glm::vec2 size = glm::vec2(1.0f, 1.0f);
	//The acceleration plane size
	glm::vec2 sizeOverTime = glm::vec2(0.0f, 0.0f);
	//The initial rotation plane
	glm::vec2 rotation = glm::vec2(0.0f, 0.0f);
	//The angular acceleration of the plane 
	glm::vec2 angularAcceleration = glm::vec2(0.0f, 0.0f);
	//The angular velocity of the plane 
	glm::vec2 angularVelocity = glm::vec2(0.0f, 0.0f);

	//Vector of all colors will be in the particle with RGBA. The colors will change during de time
	std::list<ParticleColor> colorList;
	//Knowing if we have more than one color during the time
	bool isMulticolor = false;
	//The initial direction that will take the particle
	glm::vec3 particleDirection = glm::vec3(0.0f, 1.0f, 0.0f);
};

class Emitter
{
public:
	Emitter(ParticleManager* parent);
	~Emitter();

	void Update(float dt);

	/*Create Particle with the start values
	The number of particles that we want to create this frame(you may need to create more than one particle each frame)
	And the global position in the world*/
	void CreateParticles(int numParticles, glm::vec3 globalPosition);

	//Set emision type to know witch direction will take the particles and witch shape
	void SetShapeEmitter(ShapeEmitter shape);

	void SetGlobalPos(glm::vec3 globalPos);

	//Start to emit particles
	void StartEmitter();
	//Stop to emit particles
	void StopEmitter();
	//Stop to emit particles
	void PauseEmitter();
private:
	//Get the position according to the differents shapes we have, and set the particle direction 
	glm::vec3 GetRandomPos();
public:
	//Public varible to have the particle start values acces out of this class
	ParticleStartValues startValues;

	// Particles list of this emitter
	std::list<Particle*> particles;
	//The particle manager pointer to acces in the own functions
	ParticleManager* parent;
	//The dimensions of the box shape that will spawn the particles (width and height)
	glm::vec3 boxShapeSize;
	//The radiant of the sphere shape that will spawn the particles
	float sphereShapeRad = 0.0f;
	//The height is the distance between the tip of the cone to the base, and the rad is witch radiant will have this base
	float coneShapeHeight = 0.0f;
	float coneShapeRad = 0.0f;

	//The particles we want to create per second.
	int particlesEmition = 0;
	//Set true if time is running for this emitter, false to pause it
	bool runningTime = false;
private:
	//Shape that the current emitter will have
	ShapeEmitter shapeEmitter = BoxShape;
	//Global position of the object of this emitter;
	glm::vec3 globalObjPos = glm::vec3(0.0f, 0.0f, 0.0f);

	Timer emitterTimer;

	//Calculation of the seconds to create one Particle depending on the "particlesEmittion"
	float secParticleCreation = 0.0f;
};

#endif