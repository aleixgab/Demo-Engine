#ifndef __COMPONENT_EMITTER_H__
#define __COMPONENT_EMITTER_H__

#include "Component.h"
#include "ParticleLibrary/Emitter.h"

class ComponentEmitter : public Component
{
public:
	ComponentEmitter(GameObject* gameObject, ParticleManager* manager);

	~ComponentEmitter();

	void Inspector();

	void ShowFloatValue(glm::vec2& value, bool checkBox, const char* name, float v_speed, float v_min, float v_max);

	void CheckMinMax(glm::vec2& value);

	bool EditColor(ParticleColor& colorTime, unsigned int pos = 0u);

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

	// Loop the particle (if true the particle emitter will never stop)
	bool loop = true;

	Emitter* emitter;

	int nextPos = 100;
	glm::vec4 nextColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
};

#endif