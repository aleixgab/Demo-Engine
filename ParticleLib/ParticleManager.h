#ifndef __ParticleManager_H__
#define __ParticleManager_H__

#include <list>
class ParticleEmitter;

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
	void Draw(unsigned int shaderProgramUuid, float* viewMatrix, float* projMatrix, std::list<ParticleEmitter*> emittersToDraw);

	//Start Playing all the emitters
	void StartAllEmitters();
	//Start specific emitter
	void StartEmmitter(ParticleEmitter* emitter);

	void PauseAllEmitters();
	void PauseEmmitter(ParticleEmitter* emitter);

	//Stop Playing all the emitters
	void StopAllEmitters();
	//Stop specific emitter
	void StopEmitter(ParticleEmitter* emitter);

	//Change the max particles you will have at the same time on this emitter
	bool ChangeMaxParticles(int maxParticles, ParticleEmitter* emitter);

	//Set the position of the emitter in the world coordinates
	bool SetGlobalPos(float* globalPos, ParticleEmitter* emitter);


public:
	//You have to add in this list all the emitters that you will have in your scene.
	std::list<ParticleEmitter*> emittersList;

	float* cameraPos = nullptr;
};
#endif