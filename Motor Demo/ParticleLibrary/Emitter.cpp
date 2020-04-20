#include "Emitter.h"
#include "ParticleManager.h"
#include "Timer.h"

Emitter::Emitter(ParticleManager* parent): parent(parent) 
{
	ParticleColor startColor;
	startColor.name = "Start Color";
	startValues.colorList.push_back(startColor);
}

Emitter::~Emitter()
{

}

void Emitter::Update(float dt)
{
	if (particlesEmition > 0 && runningTime)
	{
		float time = emitterTimer.GetTimeMilisec() / 1000.0f;
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
			parent->particleArray[particleId].CreateParticle(globalPosition, startValues, particleAnimation, this);
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

ShapeEmitter Emitter::GetShapeEmitter() const
{
	return shapeEmitter;
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
		randomPos.x = parent->GetRandomNum(-boxShapeSize.x / 2, boxShapeSize.x / 2);
		randomPos.y = parent->GetRandomNum(-boxShapeSize.y / 2, boxShapeSize.y / 2);
		randomPos.z = parent->GetRandomNum(-boxShapeSize.z / 2, boxShapeSize.z / 2);

		startValues.particleDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case SphereShape:
	case SphereShapeCenter:
	case SphereShapeBorder:
		//Sphere rad
		randomPos.x = parent->GetRandomNum(-1.0f, 1.0f);
		randomPos.y = parent->GetRandomNum(-1.0f, 1.0f);
		randomPos.z = parent->GetRandomNum(-1.0f, 1.0f);

		randomPos = glm::normalize(randomPos);
		startValues.particleDirection = randomPos;

		if (shapeEmitter == SphereShape)
			randomPos *= parent->GetRandomNum(0.0f, sphereShapeRad);
		else if (shapeEmitter == SphereShapeCenter)
			randomPos = glm::vec3(0.0f);
		else if (shapeEmitter == SphereShapeBorder)
			randomPos = startValues.particleDirection * sphereShapeRad;
		break;
	case ConeShape:
		//The position is always 0. We only change the direction
	{
		glm::vec3 destination;
		destination.x = parent->GetRandomNum(-coneShapeRad, coneShapeRad);
		destination.y = coneShapeHeight;
		destination.z = parent->GetRandomNum(-coneShapeRad, coneShapeRad);

		startValues.particleDirection = glm::normalize(destination);
	}
	break;
	default:
		break;
	}


	return randomPos;
}

