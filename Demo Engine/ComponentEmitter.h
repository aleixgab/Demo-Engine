#ifndef __COMPONENT_EMITTER_H__
#define __COMPONENT_EMITTER_H__

#include "Component.h"
#include <glm/vec4.hpp>
#include <ParticleManager.h>

class ControllerParticles;

class ComponentEmitter : public Component
{
public:
	ComponentEmitter(GameObject* gameObject, ControllerParticles* controller, int maxParticles);

	~ComponentEmitter();

	void Inspector();

	void BurstInsp();
	void ColorValuesInsp();
	void StartValuesInsp();
	void ShapeValuesInsp();
	void TextureValuesInsp();

	//Float value = PartVec2
	void ShowFloatValue(float* value, bool checkBox, const char* name, float v_speed, float v_min, float v_max);

	void CheckMinMax(float* value);

	bool EditColor(float* color, float position);

public:
	//Check box Randomize values
	bool checkLife = false;
	bool checkSpeed = false;
	bool checkAcceleration = false;
	bool checkSizeOverTime = false;
	bool checkSize = false;
	bool checkRotation = false;
	bool checkAngularAcceleration = false;
	bool checkAngularVelocity = false;
	bool checkAnimationSpeed = false;
	bool checkBurst = false;

	// Loop the particle (if true the particle emitter will never stop)
	bool loop = true;

	ParticleEmitter* emitter;

	ParticleValues particleValues;
	EmitterValues emitterValues;

	bool particleChange = false;
	bool emitterChange = false;

	int maxParticles = 0;
	int nextPos = 100;
	glm::vec4 nextColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

private:
	ControllerParticles* controller = nullptr;
};

#endif