#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <vector>
#include <string>

#include "Component.h"
#include "ComponentTransform.h"

class ComponentEmitter;
class ParticleManager;

enum ComponentType;

class GameObject
{
public:
	GameObject(glm::vec3 position, glm::quat rotation = glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3 scale = glm::vec3(1.0f), const char* name = nullptr);

	~GameObject();

	Transform* GetComponentTransform() const;
	ComponentEmitter* GetComponentEmitter();

	ComponentEmitter* AddComponentEmitter(ParticleManager* manager);

	bool HasEmitter();

	void RemoveComponents();
	inline void Delete() { toDelete = true; }

public:
	Transform* transform = nullptr;

	std::vector<Component*> components;

	std::string name;

	bool toDelete = false;

	bool isActive = true;
};
#endif // !__GameObject_H__