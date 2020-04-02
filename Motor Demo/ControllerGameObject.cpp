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
	for (std::vector<GameObject*>::iterator iterator = gameObjectVec.begin(); iterator != gameObjectVec.end(); ++iterator)
	{
		
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

void ControllerGameObject::RemoveGameObject(GameObject* obj)
{
	std::vector<GameObject*>::iterator iter = std::find(gameObjectVec.begin(), gameObjectVec.end(), obj);
	if (iter != gameObjectVec.end())
	{
		
		//gameObjectVec.erase(obj);
	}
}

void ControllerGameObject::GetGameObjects(std::vector<GameObject*>& gos) const
{
	gos = gameObjectVec;
}
