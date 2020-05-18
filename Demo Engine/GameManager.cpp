#include "GameManager.h"
#include <GLFW/glfw3.h>

GameManager::GameManager() : dtVector(50), fpsVector(50)
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

	float BeginTime = glfwGetTime();
//	dt = BeginTime - lastFrame;
//	lastFrame = BeginTime;

	std::list<Controller*>::const_iterator iterator = controllersList.begin();
	bool canContinue = true;
	while (iterator != controllersList.end() && canContinue)
	{
		canContinue = (*iterator)->Update(dt);
		++iterator;
	}
	//Get dt and save it in a Vector
	float lastUpdatedFrame = (glfwGetTime() - BeginTime) * 1000;
	for (uint i = dtVector.size() - 1; i > 0; --i)
		dtVector[i] = dtVector[i - 1];
	dtVector[0] = lastUpdatedFrame;

	//Get fps and save it in a Vector
	float fps = 1000.0f / lastUpdatedFrame;
	for (uint i = fpsVector.size() - 1; i > 0; --i)
		fpsVector[i] = fpsVector[i - 1];
	fpsVector[0] = fps;


	dt = 1.0f / fps;

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

std::vector<float> GameManager::GetFps() const
{
	return fpsVector;
}

std::vector<float> GameManager::GetMsec() const
{
	return dtVector;
}
