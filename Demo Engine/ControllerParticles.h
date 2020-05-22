#ifndef __CONTROLLER_PARTICLES_H__
#define __CONTROLLER_PARTICLES_H__

#include "Controller.h"
#include <list>

#include "ComponentEmitter.h"

class ControllerParticles: public Controller
{
public:

	ControllerParticles(GameManager* mng);
	~ControllerParticles();

	bool Start();
	bool Update(float dt);

public:
};

#endif