#include "GameManager.h"
#include "ControllerParticles.h"
#include <ParticleManager.h>

ControllerParticles::ControllerParticles(GameManager* mng) : Controller(mng)
{

}

ControllerParticles::~ControllerParticles()
{
	//PART_CleanUpLibrary();
}

bool ControllerParticles::Start()
{
	bool ret = false;	
	Part::StartLibrary();
	ret = Part::SetCameraPos(&Mng->scene->camera->Position.x);

	return ret;
}

bool ControllerParticles::Update(float dt)
{
	bool ret = false;

	ret = Part::Update(dt);

	return true;
}

