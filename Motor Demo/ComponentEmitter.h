#ifndef __COMPONENT_EMITTER_H__
#define __COMPONENT_EMITTER_H__

#include "Component.h"

class ComponentEmitter : public Component
{
public:
	ComponentEmitter(GameObject* gameObject);

	~ComponentEmitter() {}
};

#endif