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
	
	//float lastX = 800 / 2.0f;
	//float lastY = 600 / 2.0f;
	//bool firstMouse = true;
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

	if (glfwWindowShouldClose(Mng->window->window))//Close windows and finish
		ret = false;

	return ret;
}

bool ControllerScene::CleanUp()
{
	return false;
}
