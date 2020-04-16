#ifndef __CONTROLLER_PARTICLES_H__
#define __CONTROLLER_PARTICLES_H__

#include "Controller.h"
#include <list>

#include "ParticleLibrary/Particle.h"
#include "ComponentEmitter.h"

//Max num of particles that the engine can support at the same time. You can change this number depends on your engine
#define MAX_PARTICLES 20000

class ParticleManager;

class ControllerParticles: public Controller
{
public:

	ControllerParticles(GameManager* mng);
	~ControllerParticles();

	bool Start();
	bool Update(float dt);

public:

	ParticleManager* particleManager;

};

#endif