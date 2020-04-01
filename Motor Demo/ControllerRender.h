#ifndef __CONTROLLER_RENDER_H__
#define __CONTROLLER_RENDER_H__

#include "Controller.h"
#include "Shader.h"

class ControllerRender : public Controller
{
public:

	ControllerRender(GameManager* mng);
	~ControllerRender();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

private:
	unsigned int VBO = 0u;
	unsigned int cubeVAO = 0u;
	unsigned int lightVAO = 0u;

	Shader lightingShader;
	Shader lampShader;

	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
};

#endif