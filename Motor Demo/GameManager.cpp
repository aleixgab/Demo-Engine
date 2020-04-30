#include "GameManager.h"
#include <GLFW/glfw3.h>

GameManager::GameManager()
{
	window = new ControllerWindow(this);
	scene = new ControllerScene(this);
	gameObject = new ControllerGameObject(this);
	gui = new ControllerImGui(this);
	particle = new ControllerParticles(this);
	render = new ControllerRender(this);

	controllersList.push_back(window);
	controllersList.push_back(scene);
	controllersList.push_back(gameObject);
	controllersList.push_back(gui);

	controllersList.push_back(particle);
	controllersList.push_back(render);
}

GameManager::~GameManager()
{
	std::list<Controller*>::const_iterator iterator = controllersList.begin();

	while (iterator != controllersList.end())
	{
		delete (*iterator);
		controllersList.erase(iterator);
		iterator = controllersList.begin();
	}
}

bool GameManager::Start()
{
	std::list<Controller*>::const_iterator iterator = controllersList.begin();
	bool canContinue = true;
	while (iterator != controllersList.end() && canContinue)
	{
		canContinue = (*iterator)->Start();
		++iterator;
	}
	return canContinue;
}

bool GameManager::Update()
{
	// per-frame time logic
	// --------------------

	float currentFrame = glfwGetTime();
	dt = currentFrame - lastFrame;
	lastFrame = currentFrame;

	std::list<Controller*>::const_iterator iterator = controllersList.begin();
	bool canContinue = true;
	while (iterator != controllersList.end() && canContinue)
	{
		canContinue = (*iterator)->Update(dt);
		++iterator;
	}
	return canContinue;
}

bool GameManager::CleanUp()
{
	std::list<Controller*>::const_iterator iterator = controllersList.begin();
	bool canContinue = true;
	while (iterator != controllersList.end() && canContinue)
	{
		canContinue = (*iterator)->CleanUp();
		++iterator;
	}
	return canContinue;
}
