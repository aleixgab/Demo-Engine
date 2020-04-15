#include "ParticleManager.h"
#include <algorithm>
#include "PlaneImporter.h"

bool ParticleManager::Start()
{
	plane = new PlaneImporter();

	return true;
}

bool ParticleManager::CleanUp()
{
	delete plane;
	return true;
}

bool ParticleManager::SetCameraValues(glm::vec3* cameraUp, glm::vec3* cameraForward, glm::vec3* cameraPos)
{
	bool ret = false;
	if (cameraUp && cameraForward && cameraPos)
	{
		this->cameraUp = cameraUp;
		this->cameraForward = cameraForward;
		this->cameraPos = cameraPos;

		ret = true;
	}
	return ret;
}


bool ParticleManager::Update(float dt)
{
	bool ret = true;

	for (std::list<Emitter*>::iterator it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		(*it)->Update(dt);
	}

	//Resize the vector to get only the active particles
	activePartVec.resize(numActivePart);

	int j = 0;
	for (int i = 0; i < MAX_PARTICLES; ++i)
	{
		if (particleArray[i].isActive)
		{
			//We do the update only to the active particles. Then we add the particles in the vector to draw it after this
			particleArray[i].Update(dt);
			particleArray[i].SaveCameraDistance(*cameraPos);
			activePartVec[j++] = &particleArray[i];
		}
		else
		{
			particleArray[i].cameraDist = -1;
		}
	}


	return ret;
}

//Call this function from the renderer to draw all the particles 
void ParticleManager::Draw(uint shaderProgramUuid, glm::mat4 viewProjMatrix, PlaneInfoOGL plane)
{
	//Sort the particles from end to beginning depend on the camera distance
	std::sort(activePartVec.begin(), activePartVec.end(), [](const Particle* particle1, const Particle* particle2)
	{
		return particle1->cameraDist > particle2->cameraDist;
	});

	for (int i = 0; i < activePartVec.size(); ++i)
	{
		//Draw each active particle
		activePartVec[i]->Draw(shaderProgramUuid, viewProjMatrix, plane);
	}
}

//Create new emitter
Emitter* ParticleManager::CreateEmitter()
{
	Emitter* newEmitter = new Emitter(this);
	emittersList.push_back(newEmitter);

	return newEmitter;
}

//Remove the wanted emitter
void ParticleManager::RemoveEmitter(Emitter* emitter)
{
	emittersList.remove(emitter);
	delete emitter;
}

//You get the next slot in the array for the new particle
bool ParticleManager::GetNextParticleSlot(int& id)
{
	for (int i = lastUsedParticle; i < MAX_PARTICLES; ++i)
	{
		if (!particleArray[i].isActive)
		{
			lastUsedParticle = i;
			id = i;
			return true;
		}
	}

	for (int i = 0; i < lastUsedParticle; ++i)
	{
		if (!particleArray[i].isActive)
		{
			lastUsedParticle = i;
			id = i;
			return true;
		}
	}
	return false;
}

void ParticleManager::StartAllEmitters()
{
	for (std::list<Emitter*>::iterator it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		(*it)->StartEmitter();
	}
}

void ParticleManager::StartEmmitter(Emitter* emitter)
{
	if (emitter)
		emitter->StartEmitter();
}

void ParticleManager::StopAllEmitters()
{
	for (std::list<Emitter*>::iterator it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		(*it)->StopEmitter();
	}
}

void ParticleManager::StopEmmitter(Emitter* emitter)
{
	if (emitter)
		emitter->StopEmitter();
}
