#include "GameObject.h"
#include "ComponentEmitter.h"
#include "GameManager.h"

GameObject::GameObject(glm::vec3 position, glm::quat rotation, glm::vec3 scale, const char* name)
{
	transform = new Transform(this, position, rotation, scale);

	if (name != nullptr)
		this->name = name;
	else this->name = "Game Object";

	transform->type == ComponentType_TRANSFORM;

	components.push_back(transform);
}

GameObject::~GameObject()
{
	transform = nullptr;
}

Transform* GameObject::GetComponentTransform() const
{
	return transform;
}

ComponentEmitter* GameObject::GetComponentEmitter()
{
	ComponentEmitter* ret = nullptr;
	for (std::vector<Component*>::iterator iterator = components.begin(); iterator != components.end(); ++iterator)
	{
		if((*iterator)->type == ComponentType_EMITTER)
			ret = (ComponentEmitter*)(*iterator);
	}
	return ret;
}

ComponentEmitter* GameObject::AddComponentEmitter(ControllerParticles* controller, int maxParticles)
{

	ComponentEmitter* newEmitter = new ComponentEmitter(this,/* controller->particleManager,*/ controller, maxParticles);
	components.push_back(newEmitter);

	newEmitter->type = ComponentType_EMITTER;
	return newEmitter;
}

bool GameObject::HasEmitter()
{
	bool ret = false;
	for (std::vector<Component*>::iterator iterator = components.begin(); iterator != components.end(); ++iterator)
	{
		if ((*iterator)->type == ComponentType_EMITTER)
			ret = true;
	}
	return ret;
}

void GameObject::RemoveComponents()
{
	for (std::vector<Component*>::iterator iterator = components.begin(); iterator != components.end(); ++iterator)
	{
		delete (*iterator);
	}
	components.clear();
}
