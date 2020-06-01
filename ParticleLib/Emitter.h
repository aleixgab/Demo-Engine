#ifndef __Emitter_H__
#define __Emitter_H__

#include <map>
#include <vector>
#include <list>

#include <PartMath.h>
#include <Timer.h>
#include <random>

#include <ParticleLib.h>

#ifdef __cplusplus
extern "C" {
#endif

class PlaneImporter;

struct Particle
{
	PartVec3 position;
	PartVec3 direction;
	float speed;
	float acceleration;
	PartVec3 gravity;
	float angleRot;
	float angleVel;
	float angleAccel;
	float size;
	float sizeTime;
};

PARTICLELIB_API enum ShapeEmitter {
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
PARTICLELIB_API struct ParticleValues
{
	//------------------------------------------------BASIC VALUES--------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------------
	//The seconds that the particle will be alive
	PartVec2 life;
	//The initial velocity that will get the particle
	PartVec2 speed;
	//The acceleration in world coordinates that will afect the particle
	PartVec3 gravity;
	//The acceleration that will afect the particle velocity without changing the direction
	PartVec2 acceleration;
	//The initial plane size
	PartVec2 size;
	//The acceleration plane size
	PartVec2 sizeOverTime;
	//The initial rotation plane
	PartVec2 rotation;
	//The angular acceleration of the plane 
	PartVec2 angularAcceleration;
	//The angular velocity of the plane 
	PartVec2 angularVelocity;
	//_________________________________________________________________________________________________________________________________

	//------------------------------------------------TEXTURE--------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------------
	//Texture id that draw function need it
	unsigned int textureID = 0u;

	//It says if this texture have animation or not
	bool isParticleAnimated = false;

	//Number of rows in the texture. If we have no animation rows = 1
	int  textureRows = 1;
	//Number of colums in the texture. If we have no animation colums = 1
	int  textureColumns = 1;
	//Seconds to change the animation between texture cells
	float animationSpeed = 0.1f;
	//It says if particles anim dies after finishing the animation
	bool dieOnFinishAnim = false;
	//Knowing if we have more than one color during the time
	bool useTexture = false;
	//_________________________________________________________________________________________________________________________________


	//------------------------------------------------COLOR----------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------------
	bool activeMulticolor = false;
	//_________________________________________________________________________________________________________________________________

	//The particles we want to create per second.
	int particlesEmition = 1.0f;
};

PARTICLELIB_API struct EmitterValues
{
	//------------------------------------------------SHAPE----------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------------
	//Shape that the current emitter will have
	ShapeEmitter shapeEmitter = BoxShape;
	//Shape that the burst emitter will have
	ShapeEmitter burstShapeEmitter = BoxShape;

	//The dimensions of the box shape that will spawn the particles (width and height)
	PartVec3 boxShapeSize;
	//The radiant of the sphere shape that will spawn the particles
	float sphereShapeRad = 1.0f;
	//The height is the distance between the tip of the cone to the base
	float coneShapeHeight = 1.0f;
	//The rad is witch radiant will have the cone base
	float coneShapeRad = 1.0f;
	//_________________________________________________________________________________________________________________________________

	//------------------------------------------------BURST----------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------------------------------------------------
		//You active Burst option.
	bool isBurst = false;
	/*Seconds between bursts
	If the busrt seconds are 0 will do the burst only onces*/
	float burstSeconds = 1.0;

	//Min number of particles in each Burst. Random between this and maxBurst
	int minBurst = 1;
	//Max number of particles in each Burst. Random between this and minBurst
	int maxBurst = 10;
	//_________________________________________________________________________________________________________________________________
};

PARTICLELIB_API class ParticleEmitter
{
public:
	friend class ParticleManager;
	ParticleEmitter(float* emitterPos, int maxParticles);
	~ParticleEmitter();

	//Set All Particle Vaules 
	PARTICLELIB_API void SetParticleValues(ParticleValues values);
	//Get the current data of Particle Values
	PARTICLELIB_API ParticleValues GetParticleValues() const;

	//Set All Emitter Vaules 
	PARTICLELIB_API void SetEmitterValues(EmitterValues values);
	//Get the current data of Emitter Values
	PARTICLELIB_API EmitterValues GetEmitterValues() const;

	/*Add New Color with RGBA & the percentatge position between 0 & 1 
	The colors will change during the time depends on the position
	position 0 = begining of the particle
	position 1 = end of the particle 
	DO NOT CREATE 2 COLORS IN THE SAME POSITION*/
	PARTICLELIB_API void AddColor(float* colorRGBA, const float position);
	//Edit the specific  color sending the own position
	PARTICLELIB_API bool EditColor(float* colorRGBA, const float position);
	//Get the specific color sending the own position
	PARTICLELIB_API bool GetColor(float* colorRGBA, const float position);
	//Get all the positions
	PARTICLELIB_API void GetAllPositions(std::list<float>& positions);
	//Erase Color sending the own position
	PARTICLELIB_API bool EraseColor(const float position);

private:
	void Update(float dt);
	bool SaveCameraDistance(float* cameraPos);
	//Draw emitter by emitter for diferents textures
	void Draw(unsigned int shaderUuid);
	//Start to emit particles
	void StartEmitter();
	//Stop to emit particles
	void StopEmitter();
	//Stop to emit particles
	void PauseEmitter();

	/*Create Particle with the start values
	The number of particles that we want to create this frame(you may need to create more than one particle each frame)
	And if we are in normal or burst mode */
	void CreateParticles(int numParticles, ShapeEmitter emitter);

	void SetParticleValues(PartVec3 pos);

	void SetNewBuffers();

	/*Get the position according to the differents shapes we have, and set the particle direction 
	BoxShape -> the particles will spawn in vec3 boxShapeSize
	SphereShape -> the particles will spawn in a sphere with this radius float sphereShapeRad
	ConeShape -> the particles will spawn in the center of the emitter and will go to the circle with this height (float coneShapeHeight) in this radius (float coneShapeRad)

	IMPORTANT TO SET THE GLOBAL POS BEFORE CREATE THE PARTCLES
	*/
	PartVec3 GetRandomPos(ShapeEmitter emitter);
	float GetRandomNum(float min, float max);
private:
	float cameraDist = 0.0f;

	ParticleValues particleValues;
	EmitterValues emitterValues;

	/*Map of all colors will be in the particle with RGBA. The colors will change during the time
	float is the number between 0 and 1. Means the position in percentage 
	number 0 = begining of the particle
	number 1 = end of the particle*/
	std::map<float, PartVec4> colorMap;

	//Set true if time is running for this emitter, false to pause it
	TimerState runningTime = TimerState::StateStopped;

	//Burst Timer
	Timer burstTimer;

	//The initial direction that will take the particle
	PartVec3 initialParticleDirection = PartVec3(0.0f, 1.0f, 0.0f);

	bool changeBuffers = false;
	int particleActive = 0;

	//Global position of the object of this emitter;
	PartVec3 globalObjPos = PartVec3(0.0f, 0.0f, 0.0f);

	Timer emitterTimer;

	//Calculation of the seconds to create one Particle depending on the "particlesEmittion"
	float secParticleCreation = 0.0f;

	PlaneImporter* plane = nullptr;

	std::vector<Particle> particles;
	std::vector<PartVec2> particleLife;
	//std::vector<PartVec3> particlePosition;
	//std::vector<PartVec3> particleDirection;
	//std::vector<PartVec3> particleGravity;
	//std::vector<float> particleSpeed;
	//std::vector<float> particleAcceleration;
	//std::vector<float> particleAngleRot;
	//std::vector<float> particleAngleVel;
	//std::vector<float> particleAngleAccel;
	//std::vector<float> particleSize;
	//std::vector<float> particleSizeTime;
	std::vector<bool> particleActiveBool;

	bool onceBurst = false;

	std::mt19937 rng;
};
#ifdef __cplusplus
}
#endif

#endif