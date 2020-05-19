#ifndef __ParticleManager_H__
#define __ParticleManager_H__

#include <list>
#include <random>
#include "PartMath.h"

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
	/*Draw function with shaderUuid, camera view and projection
	You have to send emitter list. If you don't do camera culling send all the emitters,
	otherwise send only the emitters that camera sees*/
	void Draw(uint shaderProgramUuid, float* viewMatrix, float* projMatrix, std::list<Emitter*> emittersToDraw);

	//Get All the emitters. You may need it for drawing with or without Camera Culling.
	void GetEmitters(std::list<Emitter*>& emitters) const;

	//Create new emitter
	Emitter* CreateEmitter(float* emitterPos, int maxParticles);
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

	PartVec3* cameraPos = nullptr;

private:
	std::mt19937 rng;
};

#endif
