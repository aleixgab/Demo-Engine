#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <list>
#include <string>

#include "Component.h"
#include "ComponentTransform.h"

enum ComponentType;

class GameObject
{
public:
	GameObject(const char* name = nullptr);
	GameObject(glm::vec3 position, glm::quat rotation = glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3 scale = glm::vec3(1.0f), const char* name = nullptr);

	~GameObject();

	void RemoveComponents();

	void Update();
	void PostUpdate();

	inline void Delete() { toDelete = true; }

	//Components
	//----------------------------------------------------------------
		// Add
	Component* AddComponent(ComponentType type);
	// Remove
	void RemoveComponent(Component* component);
	// Get
	Component* GetComponent(ComponentType type);
	std::list<Component*> GetAllComponent(ComponentType type);

private:
	void CreateGameObject(glm::vec3 position, glm::quat rotation, glm::vec3 scale);

	void RealDelete();

public:
	Transform* transform = nullptr;

	std::list<Component*> components;

	std::string name;

	bool toDelete = false;
	bool canDraw = false;

	bool isActive = true;
};
#endif // !__GameObject_H__