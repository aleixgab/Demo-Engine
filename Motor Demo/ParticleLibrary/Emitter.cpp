#include "Emitter.h"
#include "ParticleManager.h"
#include "Timer.h"
#include <glad/glad.h>
#include "PlaneImporter.h"
#include <Brofiler/Brofiler.h>

#include <iostream>
Emitter::Emitter(ParticleManager* parent, float* emitterPos): parent(parent), globalObjPos(emitterPos[0])
{
	ParticleColor startColor;
	startColor.name = "Start Color";
	startValues.colorList.push_back(startColor);
	plane = new PlaneImporter(MAX_PARTICLES);
}

Emitter::~Emitter()
{
	StopEmitter();
	delete plane;
}

void Emitter::Update(float dt)
{
	if (runningTime)
	{
		if (particlesEmition > 0)
		{
			float time = emitterTimer.GetTime() / 1000.0f;
			if (time > secParticleCreation)
			{
				int particlesToCreate = (time / (1.0f / particlesEmition));
				CreateParticles(particlesToCreate, globalObjPos, shapeEmitter);

				secParticleCreation = (1.0f / particlesEmition);

				emitterTimer.Play();
			}
		}
		if (isBurst)
		{
			float time = burstTimer.GetTime() / 1000.0f;
			if (time > burstSeconds && !onceBurst)
			{
				CreateParticles(parent->GetRandomNum(minBurst, maxBurst), globalObjPos, burstShapeEmitter);

				if (burstSeconds == 0)
				{
					onceBurst = true;
					burstTimer.Stop();
				}
				else
					burstTimer.Play();
			}
		}
	}
}

void Emitter::CreateParticles(int numParticles, PartVec3 globalPosition, ShapeEmitter emitter)
{
	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::PapayaWhip);

	for (int i = 0; i < numParticles; ++i)
	{
		int particleId = 0;
		if (parent->GetNextParticleSlot(particleId))
		{
			globalPosition += GetRandomPos(emitter);
			//Create the particle in the correctly slot in the pool
			parent->particleArray[particleId].SetParticleValues(globalPosition, startValues, particleAnimation, this);
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

void Emitter::SetBurstShapeEmitter(ShapeEmitter shape)
{
	burstShapeEmitter = shape;
}

ShapeEmitter Emitter::GetBurstShapeEmitter() const
{
	return burstShapeEmitter;
}

void Emitter::SetGlobalPos(float* globalPos)
{
	globalObjPos.x = globalPos[0];
	globalObjPos.y = globalPos[1];
	globalObjPos.z = globalPos[2];
}


void Emitter::Draw(unsigned int shaderUuid, float* viewMatrix)
{

	GetParticleValues(viewMatrix);

	glUniform1f(glGetUniformLocation(shaderUuid, "colorPercent"), colorPercent);
	glUniform1i(glGetUniformLocation(shaderUuid, "isAnimated"), isParticleAnimated);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);


	//Bind VAO
	glBindVertexArray(plane->VAO);

	// Set mesh attributes
	glBindBuffer(GL_ARRAY_BUFFER, plane->VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * sizeof(Vertex) * 6, &particlesVertex[0]);

	//Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//TexturesCoords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(PartVec3));

	//TexturesCoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(PartVec3) + sizeof(PartVec2)));

	glDrawArrays(GL_TRIANGLES, 0, 6 * particles.size());

	glBindVertexArray(0);
}

void Emitter::GetParticleValues(float* viewMatrix)
{
	PartVec3 cameraForward = PartVec3(viewMatrix[2], viewMatrix[6], viewMatrix[10]); //Camera Forward
	PartVec3 cameraUp = PartVec3(viewMatrix[1], viewMatrix[5], viewMatrix[9]); //Camera Up

	uint cont = 0u;
	for (std::list<Particle*>::iterator iter = particles.begin(); iter != particles.end(); ++iter, ++cont)
	{
		(*iter)->SetVertexs(&particlesVertex[cont * 6], cameraForward, cameraUp);
	}
}

bool Emitter::SaveCameraDistance()
{
	bool ret = false;
	if (parent->cameraPos)
	{
		float x = (*parent->cameraPos).x - globalObjPos.x;
		float y = (*parent->cameraPos).y - globalObjPos.y;
		float z = (*parent->cameraPos).z - globalObjPos.z;

		cameraDist = x * x + y * y + z * z;
		ret = true;
	}
	return ret;
}

void Emitter::StartEmitter()
{
	runningTime = true;
	onceBurst = false;
	emitterTimer.Play();
	burstTimer.Play();
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
	burstTimer.Stop();
}

void Emitter::PauseEmitter()
{
	runningTime = false;
}

PartVec3 Emitter::GetRandomPos(ShapeEmitter emitter)
{
	PartVec3 randomPos = PartVec3(0.0f);

	switch (emitter)
	{
	case BoxShape:
		//Box Size
		randomPos.x = parent->GetRandomNum(-boxShapeSize.x / 2, boxShapeSize.x / 2);
		randomPos.y = parent->GetRandomNum(-boxShapeSize.y / 2, boxShapeSize.y / 2);
		randomPos.z = parent->GetRandomNum(-boxShapeSize.z / 2, boxShapeSize.z / 2);

		startValues.particleDirection = PartVec3(0.0f, 1.0f, 0.0f);
		break;
	case SphereShape:
	case SphereShapeCenter:
	case SphereShapeBorder:
		//Sphere rad
		randomPos.x = parent->GetRandomNum(-1.0f, 1.0f);
		randomPos.y = parent->GetRandomNum(-1.0f, 1.0f);
		randomPos.z = parent->GetRandomNum(-1.0f, 1.0f);

		randomPos = randomPos.Normalize();
		startValues.particleDirection = randomPos;

		if (shapeEmitter == SphereShape)
			randomPos *= parent->GetRandomNum(0.0f, sphereShapeRad);
		else if (shapeEmitter == SphereShapeCenter)
			randomPos = PartVec3(0.0f);
		else if (shapeEmitter == SphereShapeBorder)
			randomPos = startValues.particleDirection * sphereShapeRad;
		break;
	case ConeShape:
		//The position is always 0. We only change the direction
	{
		PartVec3 destination;
		destination.x = parent->GetRandomNum(-coneShapeRad, coneShapeRad);
		destination.y = coneShapeHeight;
		destination.z = parent->GetRandomNum(-coneShapeRad, coneShapeRad);

		startValues.particleDirection = destination.Normalize();

	}
	break;
	default:
		break;
	}


	return randomPos;
}

