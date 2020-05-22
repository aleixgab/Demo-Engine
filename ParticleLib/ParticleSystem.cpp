#include <ParticleSystem.h>
#include <ParticleManager.h>

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


