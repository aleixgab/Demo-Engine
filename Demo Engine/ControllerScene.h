#ifndef __CONTROLLER_SCEENE_H__
#define __CONTROLLER_SCEENE_H__

#include "Camera.h"

class GameObject;
class ControllerScene : public Controller
{
public:

	ControllerScene(GameManager* mng);
	~ControllerScene();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

public:
	Camera* camera = nullptr;
	GameObject* currGO = nullptr;

private:
	bool firstClick = true;
};

#endif