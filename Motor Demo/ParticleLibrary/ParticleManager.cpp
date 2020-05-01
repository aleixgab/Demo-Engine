#include "ParticleManager.h"
#include <algorithm>
#include "PlaneImporter.h"
#include <iostream>
#include <vector>
#include <glad/glad.h>

#include <Brofiler/Brofiler.h>

ParticleManager::ParticleManager()
{
	plane = new PlaneImporter(MAX_PARTICLES);

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
	{
		for (std::list<Emitter*>::iterator it = emittersList.begin(); it != emittersList.end(); ++it)
		{
			(*it)->Update(dt);
		}
	}
	//Resize the vector to get only the active particles
	activePartVec.resize(numActivePart);
	{
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
	}


	return ret;
}

//Call this function from the renderer to draw all the particles 
void ParticleManager::Draw(uint shaderProgramUuid, glm::mat4 viewMatrix, glm::mat4 projMatrix)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::PapayaWhip);

	ParticleSort();

	if (activePartVec.size() > 0)
	{
		GetParticleValues();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		bool blend = glIsEnabled(GL_BLEND);
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);

		// Constant Uniforms
		glUseProgram(shaderProgramUuid);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramUuid, "projection"), 1, GL_FALSE, &projMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramUuid, "view"), 1, GL_FALSE, &viewMatrix[0][0]);

		glUniform1f(glGetUniformLocation(shaderProgramUuid, "colorPercent"), activePartVec[0]->owner->colorPercent);
		glUniform1i(glGetUniformLocation(shaderProgramUuid, "isAnimated"), activePartVec[0]->owner->isParticleAnimated);
	
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, activePartVec[0]->owner->textureID);
	
		//Bind VAO
		glBindVertexArray(plane->VAO);
								
		// Set mesh attributes
		glBindBuffer(GL_ARRAY_BUFFER, plane->VBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

		// textCoords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));

		glBindBuffer(GL_ARRAY_BUFFER, plane->VBO_Texture);
		//Textures modified	
		glVertexAttribDivisor(2, 1);		
		glEnableVertexAttribArray(2);
		glBufferSubData(GL_ARRAY_BUFFER, 0, activePartVec.size() * sizeof(glm::vec4), &particleTexture[0]);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, plane->VBO_Color);
		//Color
		glVertexAttribDivisor(3, 1); 									
		glEnableVertexAttribArray(3);
		glBufferSubData(GL_ARRAY_BUFFER, 0, activePartVec.size() * sizeof(glm::vec4), &particleColor[0]);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);


		glBindBuffer(GL_ARRAY_BUFFER, plane->VBO_Position);
		//Position
		glVertexAttribDivisor(4, 1);
		glEnableVertexAttribArray(4);
		glBufferSubData(GL_ARRAY_BUFFER, 0, activePartVec.size() * sizeof(glm::vec3), &particleTransforms1[0]);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, plane->VBO_Rotation);
		//Rotation
		glVertexAttribDivisor(5, 1);
		glEnableVertexAttribArray(5);
		glBufferSubData(GL_ARRAY_BUFFER, 0, activePartVec.size() * sizeof(glm::vec3), &particleTransforms2[0]);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, plane->VBO_Size);
		//Scale
		glVertexAttribDivisor(6, 1);
		glEnableVertexAttribArray(6);
		glBufferSubData(GL_ARRAY_BUFFER, 0, activePartVec.size() * sizeof(float), &particleTransforms3[0]);
		glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
		

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, activePartVec.size());

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(5);
		glDisableVertexAttribArray(6);

		glBindVertexArray(0);

		glDepthMask(GL_TRUE);
		glEnable(blend);
	}
	
}

void ParticleManager::GetParticleValues()
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::PapayaWhip);
	//BROFILER_CATEGORY("Transform", Profiler::Color::AliceBlue);
	for (int i = 0; i < activePartVec.size(); ++i)
	{
		activePartVec[i]->GetTransform(particleTransforms1[i], particleTransforms2[i], particleTransforms3[i]);
		particleColor[i] = activePartVec[i]->GetColor();
		particleTexture[i] = activePartVec[i]->GetTextureCoords();
	}
}
void ParticleManager::ParticleSort()
{
	//Sort the particles from end to beginning depend on the camera distance
	std::sort(activePartVec.begin(), activePartVec.end(), [](const Particle* particle1, const Particle* particle2)
		{
			return particle1->cameraDist > particle2->cameraDist;
		});
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
	//std::cout << "START EMITTER" << std::endl;
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