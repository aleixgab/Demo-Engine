#include "Emitter.h"
#include "ParticleManager.h"
#include "Timer.h"

Emitter::Emitter(ParticleManager* parent): parent(parent) {}

Emitter::~Emitter()
{

}

void Emitter::Update(float dt)
{
	if (particlesEmition > 0 && runningTime)
	{
		float time = emitterTimer.GetTimeSec();
		if (time > secParticleCreation)
		{
			int particlesToCreate = (time / (1.0f / particlesEmition));
			CreateParticles(particlesToCreate, globalObjPos);

			secParticleCreation = (1.0f / particlesEmition);

			emitterTimer.Play();
		}
	}
}

void Emitter::CreateParticles(int numParticles, glm::vec3 globalPosition)
{
	for (int i = 0; i < numParticles; ++i)
	{
		int particleId = 0;
		if (parent->GetNextParticleSlot(particleId))
		{
			globalPosition += GetRandomPos();
			//Create the particle in the correctly slot in the pool
			parent->particleArray[particleId].CreateParticle(globalPosition, startValues, this);
			//Save the particle in emitter list to know wich particles have this emitter
			particles.push_back(&parent->particleArray[particleId]);
			//Add one count in the active particles from ParticleManager
			parent->numActivePart++;
		}
		else
			break;
	}
}

void Emitter::SetShapeEmitter(ShapeEmitter shape)
{
	shapeEmitter = shape;
}

void Emitter::SetGlobalPos(glm::vec3 globalPos)
{
	globalObjPos = globalPos;
}

void Emitter::StartEmitter()
{
	runningTime = true;
	emitterTimer.Play();
}

void Emitter::StopEmitter()
{
	for (std::list<Particle*>::iterator iterator = particles.begin(); iterator != particles.end(); ++iterator)
	{
		(*iterator)->isActive = false;
		(*iterator)->owner = nullptr;
	}

	parent->numActivePart -= particles.size();
	runningTime = false;
	secParticleCreation = 0.0f;

	particles.clear();
	emitterTimer.Stop();
}

void Emitter::PauseEmitter()
{
	runningTime = false;
}

glm::vec3 Emitter::GetRandomPos()
{
	glm::vec3 randomPos = glm::vec3(0.0f, 0.0f, 0.0f);

	switch (shapeEmitter)
	{
	case BoxShape:
		//Box Size
		randomPos.x = (float)rand() / (float)RAND_MAX * boxShapeSize.x - boxShapeSize.x/2;
		randomPos.y = (float)rand() / (float)RAND_MAX * boxShapeSize.y - boxShapeSize.y/2;
		randomPos.z = (float)rand() / (float)RAND_MAX * boxShapeSize.z - boxShapeSize.z/2;

		startValues.particleDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case SphereShape:
		//Sphere rad
		randomPos.x = (float)rand() / (float)RAND_MAX * 2.0f - 1.0f;
		randomPos.y = (float)rand() / (float)RAND_MAX * 2.0f - 1.0f;
		randomPos.z = (float)rand() / (float)RAND_MAX * 2.0f - 1.0f;

		randomPos = glm::normalize(randomPos);

		startValues.particleDirection = randomPos;

		randomPos *= sphereShapeRad;

		break;
	case ConeShape:
		//The position is always 0. We only change the direction
	{
		glm::vec3 destination;
		destination.x = (float)rand() / (float)RAND_MAX * 2.0f - 1.0f;
		destination.y = 0.0f;
		destination.z = (float)rand() / (float)RAND_MAX * 2.0f - 1.0f;

		destination = glm::normalize(destination);

		destination *= coneShapeRad;
		destination.y = coneShapeHeight;

		startValues.particleDirection = glm::normalize(destination);
	}
		break;
	default:
		break;
	}


	return randomPos;
}

