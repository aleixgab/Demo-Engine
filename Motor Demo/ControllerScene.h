#ifndef __CONTROLLER_SCEENE_H__
#define __CONTROLLER_SCEENE_H__

#include "Camera.h"

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
};

#endif