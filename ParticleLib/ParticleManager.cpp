#include "ParticleManager.h"
#include "Emitter.h"

#include <glad/glad.h>

PARTICLELIB_API bool Part::Update(float dt)
{
	bool ret = true;
	if (!Part::emittersList.empty())
	{
		for (std::list<ParticleEmitter*>::iterator it = Part::emittersList.begin(); it != Part::emittersList.end(); ++it)
		{
			(*it)->Update(dt);
			ret = (*it)->SaveCameraDistance(Part::cameraPos);
		}
	}
	return ret;
}

PARTICLELIB_API void Part::StartLibrary()
{
	gladLoadGL();
	Part::cameraPos = new float[3];
}

PARTICLELIB_API void Part::CleanUpLibrary()
{
	delete Part::cameraPos;
}

//Call this function from the renderer to draw all the particles 
PARTICLELIB_API void Part::Draw(unsigned int shaderProgramUuid, float* viewMatrix, float* projMatrix, std::list<ParticleEmitter*> emittersToDraw)
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

PARTICLELIB_API void Part::GetEmitters(std::list<ParticleEmitter*>& emitters)
{
	emitters = Part::emittersList;
}

//Create new emitter
PARTICLELIB_API ParticleEmitter* Part::CreateEmitter(float* emitterPos, int maxParticles)
{
	ParticleEmitter* newEmitter = new ParticleEmitter(emitterPos, maxParticles);
	Part::emittersList.push_back(newEmitter);

	return newEmitter;
}

//Remove the wanted emitter
PARTICLELIB_API bool Part::RemoveEmitter(ParticleEmitter* emitter)
{
	bool ret = false;
	if (emitter != nullptr)
	{
		emitter->StopEmitter();
		Part::emittersList.remove(emitter);
		delete emitter;
		ret = true;
	}

	return ret;
}

PARTICLELIB_API void Part::StartAllEmitters()
{
	if (!Part::emittersList.empty())
	{
		for (std::list<ParticleEmitter*>::iterator it = Part::emittersList.begin(); it != Part::emittersList.end(); ++it)
		{
			(*it)->StartEmitter();
		}
	}
}
PARTICLELIB_API void Part::StartEmmitter(ParticleEmitter* emitter)
{
	if (emitter)
		emitter->StartEmitter();
}

PARTICLELIB_API void Part::PauseAllEmitters()
{
	if (!Part::emittersList.empty())
	{
		for (std::list<ParticleEmitter*>::iterator it = Part::emittersList.begin(); it != Part::emittersList.end(); ++it)
		{
			(*it)->PauseEmitter();
		}
	}
}

PARTICLELIB_API void Part::PauseEmmitter(ParticleEmitter* emitter)
{
	if (emitter)
		emitter->PauseEmitter();
}

PARTICLELIB_API void Part::StopAllEmitters()
{
	if (!Part::emittersList.empty())
	{
		for (std::list<ParticleEmitter*>::iterator it = Part::emittersList.begin(); it != Part::emittersList.end(); ++it)
		{
			(*it)->StopEmitter();
		}
	}
}

PARTICLELIB_API void Part::StopEmitter(ParticleEmitter* emitter)
{
	if (emitter)
		emitter->StopEmitter();
}


PARTICLELIB_API bool Part::SetCameraPos(float* newCameraPos)
{
	bool ret = false;
	if (Part::cameraPos)
	{
		Part::cameraPos[0] = newCameraPos[0];
		Part::cameraPos[1] = newCameraPos[1];
		Part::cameraPos[2] = newCameraPos[2];

		ret = true;
	}
	return ret;
}

