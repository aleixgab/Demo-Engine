#ifndef __Emitter_H__
#define __Emitter_H__

//Max num of particles that the engine can support at the same time. You can change this number depends on your engine
#define MAX_PARTICLES 20000

#include <list>
#include <string>

#include "Timer.h"
#include "PartMath.h"

class Particle;
class ParticleManager;
class PlaneImporter;

struct ParticleColor
{
	PartVec4 color = PartVec4(1.0f);
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

struct ParticleAnimation
{
	int  textureRows = 1;
	int  textureColumns = 1;
	float textureRowsNorm = 1.0f;
	float textureColumnsNorm = 1.0f;
	PartVec2 animationSpeed = PartVec2(0.1f, 0.1f);
	bool isAnimRand = false;
};

enum ShapeEmitter {
	//Spawn particles from the box and takes object Up direction
	BoxShape = 0,
	//Spawn particles from all the sphere size
	SphereShape,
	//Spawn particles from the border of the sphere
	SphereShapeBorder,
	//Spawn particles form the center
	SphereShapeCenter,
	//Spawn particles from the tip and goes to the circle base
	ConeShape
};

//Start values. All the values with a float2 are a possible random. They are just a float, this random will be calculated in the particle creation.
struct ParticleStartValues
{

	//The seconds that the particle will be alive
	PartVec2 life = PartVec2(5.0f, 5.0f);
	//The initial velocity that will get the particle
	PartVec2 speed = PartVec2(3.0f, 3.0f);
	//The acceleration in world coordinates that will afect the particle
	PartVec3 gravity = PartVec3(0.0f, 0.0f, 0.0f);
	//The acceleration that will afect the particle velocity without changing the direction
	PartVec2 acceleration = PartVec2(0.0f, 0.0f);
	//The initial plane size
	PartVec2 size = PartVec2(1.0f, 1.0f);
	//The acceleration plane size
	PartVec2 sizeOverTime = PartVec2(0.0f, 0.0f);
	//The initial rotation plane
	PartVec2 rotation = PartVec2(0.0f, 0.0f);
	//The angular acceleration of the plane 
	PartVec2 angularAcceleration = PartVec2(0.0f, 0.0f);
	//The angular velocity of the plane 
	PartVec2 angularVelocity = PartVec2(0.0f, 0.0f);

	//Vector of all colors will be in the particle with RGBA. The colors will change during de time
	std::list<ParticleColor> colorList;
	//Knowing if we have more than one color during the time
	bool isMulticolor = false;
	//The initial direction that will take the particle
	PartVec3 particleDirection = PartVec3(0.0f, 1.0f, 0.0f);
};

class Emitter
{
public:
	Emitter(ParticleManager* parent, float* emitterPos);
	~Emitter();

	void Update(float dt);

	/*Create Particle with the start values
	The number of particles that we want to create this frame(you may need to create more than one particle each frame)
	And the global position in the world*/
	void CreateParticles(int numParticles, PartVec3 globalPosition);

	//Set emision type to know witch direction will take the particles and witch shape
	void SetShapeEmitter(ShapeEmitter shape);
	ShapeEmitter GetShapeEmitter() const;
	//Set the position of the emitter in the world coordinates
	void SetGlobalPos(float* globalPos);

	bool SaveCameraDistance();

	//Draw emitter by emitter for diferents textures
	void Draw(unsigned int shaderUuid);
	void GetParticleValues();
	//Start to emit particles
	void StartEmitter();
	//Stop to emit particles
	void StopEmitter();
	//Stop to emit particles
	void PauseEmitter();

private:
	/*Get the position according to the differents shapes we have, and set the particle direction 
	BoxShape -> the particles will spawn in vec3 boxShapeSize
	SphereShape -> the particles will spawn in a sphere with this radius float sphereShapeRad
	ConeShape -> the particles will spawn in the center of the emitter and will go to the circle with this height (float coneShapeHeight) in this radius (float coneShapeRad)

	IMPORTANT TO SET THE GLOBAL POS BEFORE CREATE THE PARTCLES
	*/
	PartVec3 GetRandomPos();
public:
	//Public varible to have the particle start values acces out of this class
	ParticleStartValues startValues;

	// Particles list of this emitter
	std::list<Particle*> particles;
	//The particle manager pointer to acces in the own functions
	ParticleManager* parent;
	//The dimensions of the box shape that will spawn the particles (width and height)
	PartVec3 boxShapeSize = PartVec3(1.0f);
	//The radiant of the sphere shape that will spawn the particles
	float sphereShapeRad = 1.0f;
	//The height is the distance between the tip of the cone to the base, and the rad is witch radiant will have this base
	float coneShapeHeight = 1.0f;
	float coneShapeRad = 1.0f;

	//The particles we want to create per second.
	int particlesEmition = 1.0f;
	//Set true if time is running for this emitter, false to pause it
	bool runningTime = false;
	//Texture id that draw function need it
	unsigned int textureID = 0u;

	ParticleAnimation particleAnimation;
	bool dieOnFinishAnim = false;
	bool isParticleAnimated = false;

	/*Percentage of the color in the particle between 0 and 1
	0.0f it will draw the texture color
	1.0f it will draw the colors that you chose
	between it will mix both colors depending on the percentage.
	*/
	float colorPercent = 0.5f;
	float cameraDist = 0.0f;
private:
	//Shape that the current emitter will have
	ShapeEmitter shapeEmitter = BoxShape;
	//Global position of the object of this emitter;
	PartVec3 globalObjPos = PartVec3(0.0f, 0.0f, 0.0f);

	Timer emitterTimer;

	//Calculation of the seconds to create one Particle depending on the "particlesEmittion"
	float secParticleCreation = 0.0f;

	PlaneImporter* plane = nullptr;

	PartVec3 particlePosition[MAX_PARTICLES];
	float particleAngleRot[MAX_PARTICLES];
	float particleSize[MAX_PARTICLES];
	PartVec4 particleColor[MAX_PARTICLES];
	PartVec4 particleTexture[MAX_PARTICLES];
};

#endif