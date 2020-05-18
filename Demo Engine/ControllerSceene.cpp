#include "GameManager.h"

#include "ControllerScene.h"
#include "ControllerWindow.h"

#include "Camera.h"

ControllerScene::ControllerScene(GameManager* mng) : Controller(mng)
{
}

ControllerScene::~ControllerScene()
{
}

bool ControllerScene::Start()
{
	// camera
	camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	
	//GameObjects
	Mng->gameObject->CreateGameObject(glm::vec3(0.0f), glm::quat(0.7071f, 0.7071f, 0.0f, 0.0f), glm::vec3(10.0f), "Ground");
	//Mng->gameObject->CreateGameObject(glm::vec3(1.0f, 0.0f, 0.0f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.5f, 1.0f, 1.0f), "Simple Object2");
	//Mng->gameObject->CreateGameObject(glm::vec3(0.0f, 1.0f, 0.0f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.5f, 1.0f), "Simple Object3");
	//Mng->gameObject->CreateGameObject(glm::vec3(-1.0f, 0.0f, 0.0f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.5f), "Simple Object4");
	//Mng->gameObject->CreateGameObject(glm::vec3(1.2f, 1.0f, 2.0f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.2f), "Light");



	return true;
}

bool ControllerScene::Update(float dt)
{
	bool ret = true;
	if (glfwGetKey(Mng->window->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(Mng->window->window, true);
	if (glfwGetKey(Mng->window->window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, dt);
	if (glfwGetKey(Mng->window->window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, dt);
	if (glfwGetKey(Mng->window->window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, dt);
	if (glfwGetKey(Mng->window->window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, dt);
	if (glfwGetKey(Mng->window->window, GLFW_KEY_E) == GLFW_PRESS)
		camera->ProcessKeyboard(UP, dt);
	if (glfwGetKey(Mng->window->window, GLFW_KEY_Q) == GLFW_PRESS)
		camera->ProcessKeyboard(DOWN, dt);

	if (glfwGetKey(Mng->window->window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(Mng->window->window, &xpos, &ypos);
		camera->MouseCallback(xpos, ypos, firstClick);
		if (firstClick)
			firstClick = false;
	}
	if (glfwGetKey(Mng->window->window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
			firstClick = true;

	if (glfwWindowShouldClose(Mng->window->window))//Close windows and finish
		ret = false;

	return ret;
}

bool ControllerScene::CleanUp()
{
	return false;
}
