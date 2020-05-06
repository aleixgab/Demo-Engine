#include "Emitter.h"
#include "ParticleManager.h"
#include "Timer.h"
#include <glad/glad.h>
#include "PlaneImporter.h"
#include <Brofiler/Brofiler.h>


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


void Emitter::Draw(unsigned int shaderUuid)
{

	GetParticleValues();

	glUniform1f(glGetUniformLocation(shaderUuid, "colorPercent"), colorPercent);
	glUniform1i(glGetUniformLocation(shaderUuid, "isAnimated"), isParticleAnimated);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Bind VAO
	glBindVertexArray(plane->VAO);

	// Set mesh attributes
	glBindBuffer(GL_ARRAY_BUFFER, plane->VBO[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	// textCoords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));

	glBindBuffer(GL_ARRAY_BUFFER, plane->VBO[1]);
	//Textures modified	
	glVertexAttribDivisor(2, 1);
	glEnableVertexAttribArray(2);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * sizeof(PartVec4), &particleTexture[0]);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, plane->VBO[2]);
	//Color
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(3);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * sizeof(PartVec4), &particleColor[0]);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glBindBuffer(GL_ARRAY_BUFFER, plane->VBO[3]);
	//Position
	glVertexAttribDivisor(4, 1);
	glEnableVertexAttribArray(4);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * sizeof(PartVec3), &particlePosition[0]);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, plane->VBO[4]);
	//Rotation
	glVertexAttribDivisor(5, 1);
	glEnableVertexAttribArray(5);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * sizeof(float), &particleAngleRot[0]);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, plane->VBO[5]);
	//Scale
	glVertexAttribDivisor(6, 1);
	glEnableVertexAttribArray(6);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * sizeof(float), &particleSize[0]);
	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particles.size());

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);

	glBindVertexArray(0);
}

void Emitter::GetParticleValues()
{
	uint cont = 0u;
	for (std::list<Particle*>::iterator iter = particles.begin(); iter != particles.end(); ++iter, ++cont)
	{
		(*iter)->GetTransform(particlePosition[cont], particleAngleRot[cont], particleSize[cont]);
		particleColor[cont] = (*iter)->GetColor();
		particleTexture[cont] = (*iter)->GetTextureCoords();
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

