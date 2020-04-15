#include "ControllerParticles.h"
#include "ParticleLibrary/PlaneImporter.h"

ControllerParticles::ControllerParticles(GameManager* mng) : Controller(mng)
{

}

ControllerParticles::~ControllerParticles()
{

}

bool ControllerParticles::Start()
{
	return true;
}

bool ControllerParticles::Update(float dt)
{
	return true;
}

void ControllerParticles::Draw()
{
}

bool ControllerParticles::CleanUp()
{
	return true;
}
