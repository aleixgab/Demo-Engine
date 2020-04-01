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
	return true;
}

bool ControllerGameObject::CleanUp()
{
	return true;
}
