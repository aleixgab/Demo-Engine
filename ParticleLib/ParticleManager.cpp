#include "ParticleManager.h"
#include "Emitter.h"

#include <glad/glad.h>

PARTICLELIB_API bool Part::Update(float dt)
{
	return manager->Update(dt);
}

PARTICLELIB_API void Part::StartLibrary()
{
	gladLoadGL();
	manager = new ParticleManager();
}

PARTICLELIB_API void Part::CleanUpLibrary()
{
	delete manager->cameraPos;
}

//Call this function from the renderer to draw all the particles 
PARTICLELIB_API void Part::Draw(unsigned int shaderProgramUuid, float* viewMatrix, float* projMatrix, std::list<ParticleEmitter*> emittersToDraw)
{
	manager->Draw(shaderProgramUuid, viewMatrix, projMatrix, emittersToDraw);
}

PARTICLELIB_API void Part::GetEmitters(std::list<ParticleEmitter*>& emitters)
{
	emitters = manager->emittersList;
}

//Create new emitter
PARTICLELIB_API ParticleEmitter* Part::CreateEmitter(float* emitterPos, int maxParticles)
{
	ParticleEmitter* newEmitter = new ParticleEmitter(emitterPos, maxParticles);
	manager->emittersList.push_back(newEmitter);

	return newEmitter;
}

//Remove the wanted emitter
PARTICLELIB_API bool Part::RemoveEmitter(ParticleEmitter* emitter)
{
	bool ret = false;
	if (emitter != nullptr)
	{
		manager->emittersList.remove(emitter);
		delete emitter;
		ret = true;
	}

	return ret;
}

PARTICLELIB_API void Part::StartAllEmitters()
{
	manager->StartAllEmitters();
}
PARTICLELIB_API void Part::StartEmmitter(ParticleEmitter* emitter)
{
	manager->StartEmmitter(emitter);
}

PARTICLELIB_API void Part::PauseAllEmitters()
{
	manager->PauseAllEmitters();
}

PARTICLELIB_API void Part::PauseEmmitter(ParticleEmitter* emitter)
{
	manager->PauseEmmitter(emitter);
}

PARTICLELIB_API void Part::StopAllEmitters()
{
	manager->StopAllEmitters();
}

PARTICLELIB_API void Part::StopEmitter(ParticleEmitter* emitter)
{
	manager->StopEmitter(emitter);
}

PARTICLELIB_API bool Part::SetCameraPos(float* newCameraPos)
{
	bool ret = false;
	if (manager->cameraPos)
	{
		manager->cameraPos[0] = newCameraPos[0];
		manager->cameraPos[1] = newCameraPos[1];
		manager->cameraPos[2] = newCameraPos[2];

		ret = true;
	}
	return ret;
}


bool ParticleManager::Update(float dt)
{
	bool ret = true;
	if (!emittersList.empty())
	{
		for (std::list<ParticleEmitter*>::iterator it = emittersList.begin(); it != emittersList.end(); ++it)
		{
			(*it)->Update(dt);
			ret = (*it)->SaveCameraDistance(cameraPos);
		}
	}
	return ret;
}

//Call this function from the renderer to draw all the particles 
void ParticleManager::Draw(unsigned int shaderProgramUuid, float* viewMatrix, float* projMatrix, std::list<ParticleEmitter*> emittersToDraw)
{

	if (emittersToDraw.size() > 0)
	{
		//Sort back to front
		emittersToDraw.sort([](const ParticleEmitter* emitter1, const ParticleEmitter* emitter2)
			{
				return emitter1->cameraDist > emitter2->cameraDist;
			});

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		bool blend = glIsEnabled(GL_BLEND);
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);

		glUseProgram(shaderProgramUuid);
		// Constant Uniforms
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramUuid, "projection"), 1, GL_FALSE, projMatrix);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramUuid, "view"), 1, GL_FALSE, viewMatrix);

		for (std::list<ParticleEmitter*>::iterator iter = emittersToDraw.begin(); iter != emittersToDraw.end(); ++iter)
		{
			(*iter)->Draw(shaderProgramUuid);
		}

		glDepthMask(GL_TRUE);
		glEnable(blend);
	}
}

ParticleManager::ParticleManager()
{
	cameraPos = new float[3];
}
ParticleManager::~ParticleManager()
{
	delete cameraPos;
}

void ParticleManager::StartAllEmitters()
{
	if (!emittersList.empty())
	{
		for (std::list<ParticleEmitter*>::iterator it = emittersList.begin(); it != emittersList.end(); ++it)
		{
			(*it)->StartEmitter();
		}
	}
}
void ParticleManager::StartEmmitter(ParticleEmitter* emitter)
{
	if (emitter)
		emitter->StartEmitter();
}

void ParticleManager::PauseAllEmitters()
{
	if (!emittersList.empty())
	{
		for (std::list<ParticleEmitter*>::iterator it = emittersList.begin(); it != emittersList.end(); ++it)
		{
			(*it)->PauseEmitter();
		}
	}
}

void ParticleManager::PauseEmmitter(ParticleEmitter* emitter)
{
	if (emitter)
		emitter->PauseEmitter();
}

void ParticleManager::StopAllEmitters()
{
	if (!emittersList.empty())
	{
		for (std::list<ParticleEmitter*>::iterator it = emittersList.begin(); it != emittersList.end(); ++it)
		{
			(*it)->StopEmitter();
		}
	}
}

void ParticleManager::StopEmitter(ParticleEmitter* emitter)
{
	if (emitter)
		emitter->StopEmitter();
}

