#include "ParticleManager.h"
#include "Emitter.h"

#include <glad/glad.h>
#include <Brofiler/Brofiler.h>


ParticleManager::ParticleManager()
{
	rng = std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count());
	cameraPos = new PartVec3(0.0f);
}

ParticleManager::~ParticleManager()
{
	delete cameraPos;
}

bool ParticleManager::Update(float dt)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::PapayaWhip);
	bool ret = true;
	{
		for (std::list<Emitter*>::iterator it = emittersList.begin(); it != emittersList.end(); ++it)
		{
			(*it)->Update(dt);
			ret = (*it)->SaveCameraDistance();
		}
	}

	return ret;
}

//Call this function from the renderer to draw all the particles 
void ParticleManager::Draw(uint shaderProgramUuid, float* viewMatrix, float* projMatrix, std::list<Emitter*> emittersToDraw)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::PapayaWhip);

	if (canDraw)
	{
	
		//Sort back to front
		emittersToDraw.sort([](const Emitter* emitter1, const Emitter* emitter2)
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

		for (std::list<Emitter*>::iterator iter = emittersToDraw.begin(); iter != emittersToDraw.end(); ++iter)
		{
			(*iter)->Draw(shaderProgramUuid);
		}

		glDepthMask(GL_TRUE);
		glEnable(blend);
	}
	
}

void ParticleManager::GetEmitters(std::list<Emitter*>& emitters) const
{
	emitters = emittersList;
}

//Create new emitter
Emitter* ParticleManager::CreateEmitter(float* emitterPos, int maxParticles)
{
	Emitter* newEmitter = new Emitter(this, emitterPos, maxParticles);
	emittersList.push_back(newEmitter);

	return newEmitter;
}

//Remove the wanted emitter
void ParticleManager::RemoveEmitter(Emitter* emitter)
{
	emittersList.remove(emitter);
	delete emitter;
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
}

uint ParticleManager::GetRandomNum()
{
	return rng();
}

float ParticleManager::GetRandomNum(float min, float max)
{
	if (min < max)
		return (max - min) * (float)rng() / (float)rng.max() + min;
	else
		return min;
}

bool ParticleManager::SetCameraPos(float* cameraPos)
{
	bool ret = false;
	if (cameraPos)
	{
		this->cameraPos->x = cameraPos[0];
		this->cameraPos->y = cameraPos[1];
		this->cameraPos->z = cameraPos[2];

		ret = true;
	}
	return ret;
}
