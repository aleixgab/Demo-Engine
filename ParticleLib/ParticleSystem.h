#ifndef __ParticleSistem_H__
#define __ParticleSistem_H__

#include <list>
#include <ParticleLib.h>
#include <Emitter.h>

class ParticleManager;

#ifdef __cplusplus
extern "C" {
#endif
namespace Part {

	PARTICLELIB_API void StartLibrary();
	PARTICLELIB_API void CleanUpLibrary();
	//Create new emitter
	PARTICLELIB_API ParticleEmitter* CreateEmitter(float* emitterPos, int maxParticles);
	//Remove the wanted emitter
	PARTICLELIB_API bool RemoveEmitter(ParticleEmitter* emitter);

	//Start Playing all the emitters
	PARTICLELIB_API void StartAllEmitters();
	//Start specific emitter
	PARTICLELIB_API void StartEmmitter(ParticleEmitter* emitter);

	PARTICLELIB_API void PauseAllEmitters();
	PARTICLELIB_API void PauseEmmitter(ParticleEmitter* emitter);

	//Stop Playing all the emitters
	PARTICLELIB_API void StopAllEmitters();
	//Stop specific emitter
	PARTICLELIB_API void StopEmitter(ParticleEmitter* emitter);

	//Set camera pointers. If returns false it's mean the pointer it is null
	PARTICLELIB_API bool SetCameraPos(float* cameraPos);

	//Set Particle shader sending the pathslocation of vertex, geometry and fragment. It returns de Uid that the drawer needs
	PARTICLELIB_API int SetShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath);

	/*Draw function with shaderUuid, camera view and projection
	You have to send emitter list. If you don't do camera culling send all the emitters,
	otherwise send only the emitters that camera sees*/
	PARTICLELIB_API void Draw(unsigned int shaderProgramUuid, float* viewMatrix, float* projMatrix, std::list<ParticleEmitter*> emittersToDraw);

	//Get All the emitters. You may need it for drawing with or without Camera Culling.
	PARTICLELIB_API void GetEmitters(std::list<ParticleEmitter*>& emitters);

	/*Send delta time that will affect the particles
	Returns false if user doesn't set camera pointers correctly*/
	PARTICLELIB_API bool Update(float dt);

	//Change the max particles you will have at the same time on this emitter
	PARTICLELIB_API bool ChangeMaxParticles(int maxParticles, ParticleEmitter* emitter);

	//Set the position of the emitter in the world coordinates
	PARTICLELIB_API bool SetGlobalPos(float* globalPos, ParticleEmitter* emitter);

	//You have to add in this list all the emitters that you will have in your scene.
	PARTICLELIB_API ParticleManager* manager;
}


#ifdef __cplusplus
}
#endif

#endif
