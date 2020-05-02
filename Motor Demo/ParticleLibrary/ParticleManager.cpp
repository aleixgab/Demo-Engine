#include "ParticleManager.h"
#include <algorithm>
#include <iostream>
#include <glad/glad.h>

#include <Brofiler/Brofiler.h>

ParticleManager::ParticleManager()
{
	rng = std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count());
}

ParticleManager::~ParticleManager()
{
}

bool ParticleManager::SetCameraPos(glm::vec3* cameraPos)
{
	bool ret = false;
	if (cameraPos)
	{
		this->cameraPos = cameraPos;

		ret = true;
	}
	return ret;
}


bool ParticleManager::Update(float dt)
{
	bool ret = true;
	{
		BROFILER_CATEGORY("Emitter Update", Profiler::Color::PapayaWhip);
		for (std::list<Emitter*>::iterator it = emittersList.begin(); it != emittersList.end(); ++it)
		{
			(*it)->Update(dt);
			ret = (*it)->SaveCameraDistance();
		}
	}

	if (ret)
	{
		for (int i = 0; i < MAX_PARTICLES; ++i)
		{
			if (particleArray[i].isActive)
			{
				//We do the update only to the active particles. Then we add the particles in the vector to draw it after this
				ret = particleArray[i].Update(dt);
				if (cameraPos)
					particleArray[i].SaveCameraDistance(*cameraPos);
				else
					ret = false;
			}
			else
			{
				particleArray[i].cameraDist = -1;
			}
		}
	}
	return ret;
}

//Call this function from the renderer to draw all the particles 
void ParticleManager::Draw(uint shaderProgramUuid, glm::mat4 viewMatrix, glm::mat4 projMatrix)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::PapayaWhip);

	if (canDraw)
	{
		//Sort back to front
		emittersList.sort();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		bool blend = glIsEnabled(GL_BLEND);
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);

		glUseProgram(shaderProgramUuid);
		// Constant Uniforms
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramUuid, "projection"), 1, GL_FALSE, &projMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramUuid, "view"), 1, GL_FALSE, &viewMatrix[0][0]);

		for (std::list<Emitter*>::iterator iter = emittersList.begin(); iter != emittersList.end(); ++iter)
		{
			(*iter)->Draw(shaderProgramUuid);
		}

		glDepthMask(GL_TRUE);
		glEnable(blend);
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
	canDraw = true;
}

void ParticleManager::StartEmmitter(Emitter* emitter)
{
	if (emitter)
		emitter->StartEmitter();
	canDraw = true;
}

void ParticleManager::PauseAllEmitters()
{
	for (std::list<Emitter*>::iterator it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		(*it)->PauseEmitter();
	}
}

void ParticleManager::PauseEmmitter(Emitter* emitter)
{
	if (emitter)
		emitter->PauseEmitter();
}

void ParticleManager::StopAllEmitters()
{
	for (std::list<Emitter*>::iterator it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		(*it)->StopEmitter();
	}
	canDraw = false;
}

void ParticleManager::StopEmitter(Emitter* emitter)
{
	if (emitter)
		emitter->StopEmitter();
	canDraw = false;
}

uint ParticleManager::GetRandomNum()
{
	return rng();
}

float ParticleManager::GetRandomNum(float min, float max)
{
	return (max - min) * (float)rng() / (float)rng.max() + min;
}

int ParticleManager::GetRandomNum(int min, int max)
{
	return (max - min) * rng() / rng.max() + min;
}