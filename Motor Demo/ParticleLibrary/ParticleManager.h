#ifndef __ParticleManager_H__
#define __ParticleManager_H__

#include <list>
#include <random>
#include "Particle.h"

//Max num of particles that the engine can support at the same time. You can change this number depends on your engine
#define MAX_PARTICLES 10000000

typedef unsigned int uint;

class Emitter;

class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	/*Send delta time that will affect the particles
	Returns false if user doesn't set camera pointers correctly*/
	bool Update(float dt);
	//Draw function with shaderUuid, and camera view and projection
	void Draw(uint shaderProgramUuid, float* viewMatrix, float* projMatrix);
	//You get the next slot in the pool array for the new particle
	bool GetNextParticleSlot(int& id);

	//Create new emitter
	Emitter* CreateEmitter(float* emitterPos);
	//Remove the wanted emitter
	void RemoveEmitter(Emitter* emitter);

	//Start Playing all the emitters
	void StartAllEmitters();
	//Start specific emitter
	void StartEmmitter(Emitter *emitter);

	void PauseAllEmitters();
	void PauseEmmitter(Emitter* emitter);

	//Stop Playing all the emitters
	void StopAllEmitters();
	//Stop specific emitter
	void StopEmitter(Emitter* emitter);

	//Get random num between 0 and max uint32 value
	uint GetRandomNum();
	//Ger random float num with a min and max parameter
	float GetRandomNum(float min, float max);

	//Set camera pointers. If returns false it's mean the pointer it is null
	bool SetCameraPos(float* cameraPos);

public:
	//You have to add in this list all the emitters that you will have in your scene.
	std::list<Emitter*> emittersList; 
	//Particle pool where are all the particles (active and inactive)
	Particle particleArray[MAX_PARTICLES];
	//Counter from all the active particles. This will let us ignore the other particles.
	int numActivePart = 0;

	PartVec3* cameraPos = nullptr;

private:
	//Counter to know which part of the array pool we are. This will allow us to continue the pool consecutively.	
	int lastUsedParticle = 0;

	std::mt19937 rng;

	bool canDraw = false;
};

#endif
