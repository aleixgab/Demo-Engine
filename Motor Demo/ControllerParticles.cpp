#include "GameManager.h"
#include "ControllerParticles.h"
#include "ParticleLibrary/ParticleManager.h"

ControllerParticles::ControllerParticles(GameManager* mng) : Controller(mng)
{

}

ControllerParticles::~ControllerParticles()
{
	if(particleManager)
		delete particleManager;
}

bool ControllerParticles::Start()
{
	if(particleManager == nullptr)
		particleManager = new ParticleManager();
	particleManager->SetCameraValues(&Mng->scene->camera->Up, &Mng->scene->camera->Front, &Mng->scene->camera->Position);
	return true;
}

bool ControllerParticles::Update(float dt)
{
	bool ret = false;

	if (particleManager)
		ret = particleManager->Update(dt);

	return ret;
}

