#include "ControllerGameObject.h"

ControllerGameObject::ControllerGameObject(GameManager* mng) : Controller(mng)
{
}

ControllerGameObject::~ControllerGameObject()
{
}

bool ControllerGameObject::Start()
{
	return true;
}

bool ControllerGameObject::Update(float dt)
{
	std::list<GameObject*>::iterator iter = gameObjectVec.begin();
	while (iter != gameObjectVec.end())
	{
		if ((*iter)->toDelete)
		{
			gameObjectVec.erase(iter++);  // alternatively, i = items.erase(i);
		}
		else
		{
			++iter;
		}
	}

	for (std::list<GameObject*>::iterator iterator = gameObjectVec.begin(); iterator != gameObjectVec.end(); ++iterator)
	{
		if ((*iterator)->toDelete)
		{


		}
	}
	return true;
}

bool ControllerGameObject::CleanUp()
{
	return true;
}

GameObject* ControllerGameObject::CreateGameObject(glm::vec3 position, glm::quat rotation, glm::vec3 scale, const char* name)
{
	GameObject* obj = new GameObject(position, rotation, scale, name);
	gameObjectVec.push_back(obj);

	return obj;
}

void ControllerGameObject::GetGameObjects(std::list<GameObject*>& gos) const
{
	gos = gameObjectVec;
}
