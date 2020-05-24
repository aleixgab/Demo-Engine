#include "GameManager.h"
#include "ControllerParticles.h"
#include <ParticleSystem.h>
#include <Brofiler/Brofiler.h>

ControllerParticles::ControllerParticles(GameManager* mng) : Controller(mng)
{

}

ControllerParticles::~ControllerParticles()
{
	Part::CleanUpLibrary();
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
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::PapayaWhip);
	ret = Part::Update(dt);

	return true;
}

