#include "ParticleManager.h"
#include <algorithm>
#include "PlaneImporter.h"
#include <iostream>
#include <vector>
#include <glad/glad.h>


ParticleManager::ParticleManager()
{
	plane = new PlaneImporter();

	rng = std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count());
}

ParticleManager::~ParticleManager()
{
	delete plane;
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
			ret = particleArray[i].Update(dt);
			if (cameraPos)
				particleArray[i].SaveCameraDistance(*cameraPos);
			else
				ret = false;
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
void ParticleManager::Draw(uint shaderProgramUuid, glm::mat4 viewMatrix, glm::mat4 projMatrix)
{
	//Sort the particles from end to beginning depend on the camera distance
	std::sort(activePartVec.begin(), activePartVec.end(), [](const Particle* particle1, const Particle* particle2)
		{
			return particle1->cameraDist > particle2->cameraDist;
		});

	if (activePartVec.size() > 0)
	{
		for (int i = 0; i < activePartVec.size(); ++i)
		{
			//Draw each active particle
			//activePartVec[i]->Draw(shaderProgramUuid, viewMatrix, projMatrix);
			particleTransforms[i] = activePartVec[i]->GetTransform();
			particleColor[i] = activePartVec[i]->GetColor();
			particleTexture[i] = activePartVec[i]->GetTexture();
		}


		bool blend = glIsEnabled(GL_BLEND);
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);

		glUseProgram(shaderProgramUuid);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramUuid, "projection"), 1, GL_FALSE, &projMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramUuid, "view"), 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramUuid, "model"), activePartVec.size(), GL_FALSE, &particleTransforms[0][0][0]);

		glUniform4fv(glGetUniformLocation(shaderProgramUuid, "color"), activePartVec.size(), &particleColor[0].r);
		glUniform4fv(glGetUniformLocation(shaderProgramUuid, "modidiedCoords"), activePartVec.size(), &particleTexture[0].x);

		glUniform1f(glGetUniformLocation(shaderProgramUuid, "colorPercent"), activePartVec[0]->owner->colorPercent);
		glUniform1i(glGetUniformLocation(shaderProgramUuid, "isAnimated"), activePartVec[0]->isParticleAnimated);
	
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, activePartVec[0]->owner->textureID);
		
		glBindVertexArray(plane->VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawElementsInstanced(GL_TRIANGLES, 6 * activePartVec.size(), GL_UNSIGNED_INT, 0, activePartVec.size());
		glBindVertexArray(0);

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
}

void ParticleManager::StartEmmitter(Emitter* emitter)
{
	if (emitter)
		emitter->StartEmitter();
	std::cout << "START EMITTER" << std::endl;
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
	return (max - min) * (float)rng() / (float)rng.max() + min;
}

int ParticleManager::GetRandomNum(int min, int max)
{
	return (max - min) * rng() / rng.max() + min;
}