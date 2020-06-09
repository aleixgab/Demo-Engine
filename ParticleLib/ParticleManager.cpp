#include <ParticleManager.h>
#include <Emitter.h>
#include <PlaneImporter.h>

#include <glad/glad.h>

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


