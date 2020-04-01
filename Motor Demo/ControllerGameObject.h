#ifndef __CONTROLLER_GAMEOBJECT_H__
#define __CONTROLLER_GAMEOBJECT_H__

#include "Controller.h"
#include <list>

class ControllerGameObject : public Controller
{
public:

	ControllerGameObject(GameManager* mng);
	~ControllerGameObject();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

public:
	std::list<int> gameObjectList;
};

#endif