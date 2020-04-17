#include <iostream>
#include "GameManager.h"

#pragma comment(lib, "3rdPart\\lib\\glfw3.lib")

GameManager* Mng = nullptr;
int main(int argc, char** argv)
{
	bool canContinue = true;
	Mng = new GameManager();
	canContinue = Mng->Start();
	if (!canContinue)
		std::cout << "ERROR STARTING THE ENGINE" << std::endl;


	while (canContinue)
	{
		canContinue = Mng->Update();
		if (!canContinue)
			std::cout << "STOP UPDATE" << std::endl;
	}


	Mng->CleanUp();
	return 0;
}

//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	// make sure the viewport matches the new window dimensions; note that width and 
//	// height will be significantly larger than specified on retina displays.
//	glViewport(0, 0, width, height);
//}


//// glfw: whenever the mouse moves, this callback is called
//// -------------------------------------------------------
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	if (firstMouse)
//	{
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//
//	float xoffset = xpos - lastX;
//	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//	lastX = xpos;
//	lastY = ypos;
//
//	camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
//// glfw: whenever the mouse scroll wheel scrolls, this callback is called
//// ----------------------------------------------------------------------
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	camera.ProcessMouseScroll(yoffset);
//}