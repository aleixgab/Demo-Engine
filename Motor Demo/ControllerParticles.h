#ifndef __CONTROLLER_PARTICLES_H__
#define __CONTROLLER_PARTICLES_H__

#include "Controller.h"
#include <list>

#include "ParticleLibrary/Particle.h"
#include "ComponentEmitter.h"

//Max num of particles that the engine can support at the same time. You can change this number depends on your engine
#define MAX_PARTICLES 20000

class PlaneImporter;

class ControllerParticles: public Controller
{
public:

	ControllerParticles(GameManager* mng);
	~ControllerParticles();

	bool Start();
	bool Update(float dt);

	void Draw();

	bool CleanUp();

public:
	//You have to add in this list all the emitters that you will have in your scene.
	std::list<Emitter*> emittersList;
	//Particle pool where are all the particles (active and inactive)
	Particle particleArray[MAX_PARTICLES];
	//Counter from all the active particles. This will let us ignore the other particles.
	int numActivePart = 0;

	PlaneImporter* plane = nullptr;

};

#endif