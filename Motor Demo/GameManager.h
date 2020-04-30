#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <list>
#include "Controller.h"
#include "ControllerWindow.h"
#include "ControllerRender.h"
#include "ControllerScene.h"
#include "ControllerGameObject.h"
#include "ControllerImGui.h"
#include "ControllerParticles.h"
class GameManager
{
public:

	GameManager();
	~GameManager();

	bool Start();
	bool Update();
	bool CleanUp();

public:
	ControllerWindow* window;
	ControllerRender* render;
	ControllerScene* scene;
	ControllerGameObject* gameObject;
	ControllerImGui* gui;
	ControllerParticles* particle;

private:
	std::list<Controller*> controllersList;
	float dt = 0.0f;
	float lastFrame = 0.0f;
};

#endif