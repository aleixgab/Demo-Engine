#ifndef __CONTROLLER_WINDOW_H__
#define __CONTROLLER_WINDOW_H__

#include <glfw/glfw3.h>

class GameManager;

class ControllerWindow : public Controller
{
public:

	ControllerWindow(GameManager* mng);

	~ControllerWindow();

	bool Start();
	bool CleanUp();

public:
	GLFWwindow* window;
	int SCR_WIDTH = 1280.0f;
	int SCR_HEIGHT = 861.0f;
};

#endif