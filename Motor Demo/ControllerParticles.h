#ifndef __CONTROLLER_PARTICLES_H__
#define __CONTROLLER_PARTICLES_H__

#include "Controller.h"
#include <list>

#include "ParticleLibrary/Particle.h"
#include "ComponentEmitter.h"

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