#ifndef __ParticleManager_H__
#define __ParticleManager_H__

//Max num of particles that the engine can support at the same time. You can change this number depends on your engine
#define MAX_PARTICLES 20000

#include <list>
#include <vector>

#include "Particle.h"
#include "Emitter.h"

#include <random>

typedef unsigned int uint;

class PlaneImporter;
class Shader;

class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	//Set camera pointers. If returns false it's mean some pointer is null
	bool SetCameraValues(glm::vec3 * pointerCameraUp, glm::vec3* pointerCameraForward, glm::vec3* cameraPos);
	/*Send delta time that will affect the particles
	Returns false if user doesn't set camera pointers correctly
	*/
	bool Update(float dt);
	//Draw function with shaderUuid, and camera view and projection
	void Draw(uint shaderProgramUuid, glm::mat4 viewMatrix, glm::mat4 projMatrix);
	//You get the next slot in the array for the new particle
	bool GetNextParticleSlot(int& id);

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
	//Ger random num with a min and max parameter
	float GetRandomNum(float min, float max);

	//Create new emitter
	Emitter* CreateEmitter();
	//Remove the wanted emitter
	void RemoveEmitter(Emitter* emitter);

public:
	//You have to add in this list all the emitters that you will have in your scene.
	std::list<Emitter*> emittersList; 
	//Particle pool where are all the particles (active and inactive)
	Particle particleArray[MAX_PARTICLES];
	//Counter from all the active particles. This will let us ignore the other particles.
	int numActivePart = 0;

	glm::vec3* cameraUp = nullptr;
	glm::vec3* cameraForward = nullptr;
	glm::vec3* cameraPos = nullptr;

	PlaneImporter* plane = nullptr;
private:
	//Save only the active particle for a future Draw 
	std::vector<Particle*> activePartVec;
	//Counter to know which part of the array pool we are. This will allow us to continue the pool consecutively.	
	int lastUsedParticle = 0;

	std::mt19937 rng;
};

#endif
