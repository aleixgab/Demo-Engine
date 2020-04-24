#include "ParticleManager.h"
#include <algorithm>
#include "PlaneImporter.h"
#include <iostream>
#include <vector>
#include <glad/glad.h>


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

		glVertexAttribDivisor(0, 0);												
		glVertexAttribDivisor(1, 0);												
		glVertexAttribDivisor(2, 1); //Textures modified			
		glVertexAttribDivisor(3, 1); //Color
		glVertexAttribDivisor(4, 1); //Matrix 1st row	
		glVertexAttribDivisor(5, 1); //Matrix 2nd row	
		glVertexAttribDivisor(6, 1); //Matrix 3rd row	
		glVertexAttribDivisor(7, 1); //Matrix 4th row	
										

		glBindBuffer(GL_ARRAY_BUFFER, plane->VBO);
		// Set mesh attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
		// textCoords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(3 * sizeof(float)));


		glBindBuffer(GL_ARRAY_BUFFER, plane->VBO_Texture);
		glEnableVertexAttribArray(2);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * activePartVec.size(), &particleTexture[0], GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, activePartVec.size() * sizeof(glm::vec4), &particleTexture[0]);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);


		glBindBuffer(GL_ARRAY_BUFFER, plane->VBO_Color);
		glEnableVertexAttribArray(3);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * activePartVec.size(), &particleColor[0], GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, activePartVec.size() * sizeof(glm::vec4), &particleColor[0]);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);


		glBindBuffer(GL_ARRAY_BUFFER, plane->VBO_TransformX);
		glEnableVertexAttribArray(4);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * activePartVec.size(), &particleTransforms[0][0], GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, activePartVec.size() * sizeof(glm::vec4), &particleTransformsX[0]);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, plane->VBO_TransformY);
		glEnableVertexAttribArray(5);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * activePartVec.size(), &particleTransforms[0][0], GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, activePartVec.size() * sizeof(glm::vec4), &particleTransformsY[0]);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, plane->VBO_TransformW);
		glEnableVertexAttribArray(6);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * activePartVec.size(), &particleTransforms[0][0], GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, activePartVec.size() * sizeof(glm::vec4), &particleTransformsZ[0]);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, plane->VBO_TransformZ);
		glEnableVertexAttribArray(4);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * activePartVec.size(), &particleTransforms[0][0], GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, activePartVec.size() * sizeof(glm::vec4), &particleTransformsW[0]);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);


		//for (unsigned int i = 0; i < 4; i++) {
		//	glEnableVertexAttribArray(4 + i);
		//	glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
		//	glVertexAttribDivisor(4 + i, 1);
		//}
		





		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, activePartVec.size());

		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(5);
		glDisableVertexAttribArray(6);
		glDisableVertexAttribArray(7);

		glBindVertexArray(0);

		glDepthMask(GL_TRUE);
		glEnable(blend);
	}
	
}

void ParticleManager::GetParticleValues()
{
	for (int i = 0; i < activePartVec.size(); ++i)
	{
		//Draw each active particle
		//activePartVec[i]->Draw(shaderProgramUuid, viewMatrix, projMatrix);
		glm::mat4 mat = activePartVec[i]->GetTransform();
		particleTransformsX[i] = glm::vec4(mat[0][0], mat[1][0], mat[2][0], mat[3][0]);
		particleTransformsY[i] = glm::vec4(mat[0][1], mat[1][1], mat[2][1], mat[3][1]);
		particleTransformsZ[i] = glm::vec4(mat[0][2], mat[1][2], mat[2][2], mat[3][2]);
		particleTransformsW[i] = glm::vec4(mat[0][3], mat[1][3], mat[2][3], mat[3][3]);

		particleColor[i] = activePartVec[i]->GetColor();
		particleTexture[i] = activePartVec[i]->GetTexture();
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