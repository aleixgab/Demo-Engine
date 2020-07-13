# DemoEngine

This project is a particle library in C++. Easily to implement in any engine in order 
to create a particle system for a video game.

## How to use it.

Include `ParticleSystem.h`, `particle.lib`, `particle.dll` and the 3 particle shaders in your project

First, initialize the library calling:

```C++
Part::StartLibrary()
```

Next load the shaders added previously and save the int value. you will need this UID integer to draw:
```C++
int Part::SetShader(char* vertexPath, char* geometryPath, char* fragmentPath)
```

Every time you change the camera you need to set the position pointer.
```C++
Part::SetCameraPos(float* cameraPos)
```

You have to provide a function to be called every time.
```C++
Part::Update(float dt)
```

You can get all the Emitters list to discard all the emitters out of the camera with your Camera culling
```C++
Part::GetEmitters(std::list<ParticleEmitter*>& emitters)
```

Then you have to draw all the emitter particles you want after the scene render:
```C++
Part::Draw(unsigned int shaderProgramUuid, float* view matrix, float*projMatrix, std::list<ParticleEmitter*> emittersToDraw)
```
### Particle Emitters.

Each emitter system has to be created with his position and the max particles that the emitter will draw at the same time and save the pointer.
```C++
ParticleEmitter* Part::CreateEmitter(float* emitterPos, int maxParticles)
```
You can change the max particles
```C++
ParticleEmitter::ChangeMaxParticle(int maxParticles)
```
After creating the emitter you can get and set all the particle and emitter values
```C++
ParticleEmitter::GetEmitterValues();
ParticleEmitter::SetEmitterValues(EmitterValues values);

ParticleEmitter::SetParticlesValues();
ParticleEmitter::GetParticleValues(ParticleValues values);
```

If you change the emitter position after creating it you have to set it
```C++
ParticleEmitter::SetGlobalPos(float* position)
```

To start, stop or pause the emitters, it has similar functions changing just the beginning
```C++
Part::StartAllEmitters();
//Specific emitter
Part::StartEmmitter(ParticleEmitter* emitter);
```

Remove an existing emitter. If the emitter doesn't exist the function returns false.
```C++
Part:: RemoveEmitter(ParticleEmitter* emitter)
```

Close the library always with
```C++
Part::CleanUpLibrary();
```
