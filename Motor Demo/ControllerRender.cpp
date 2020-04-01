#include "ControllerRender.h"
#include <glad/glad.h>
#include "GameManager.h"

#include "ControllerWindow.h"

ControllerRender::ControllerRender(GameManager* mng) : Controller(mng)
{
}

ControllerRender::~ControllerRender()
{
}

bool ControllerRender::Start()
{
	// lighting
	lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	lightingShader.SetShader("VShader_BasicLight.txt", "FShader_BasicLight.txt");
	lampShader.SetShader("VShader_Lamp.txt", "FShader_Lamp.txt");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	};
	// first, configure the cube's VAO (and VBO)
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return true;
}

bool ControllerRender::Update(float dt)
{
	// render
	// ------
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// be sure to activate shader when setting uniforms/drawing objects
	lightingShader.UseProgram();
	lightingShader.SetVec3("objectColor", 1.0f, 0.5f, 0.31f);
	lightingShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
	lightingShader.SetVec3("lightPos", lightPos);
	lightingShader.SetVec3("viewPos", Mng->scene->camera->Position);

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(Mng->scene->camera->Zoom), (float)Mng->window->SCR_WIDTH / (float)Mng->window->SCR_HEIGHT, 0.1f, 100.0f);

	glm::mat4 view = Mng->scene->camera->GetViewMatrix();
	lightingShader.SetMat4("projection", projection);
	lightingShader.SetMat4("view", view);

	// world transformation
	glm::mat4 model = glm::mat4(1.0f);
	lightingShader.SetMat4("model", model);

	// render the cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	// also draw the lamp object
	lampShader.UseProgram();
	lampShader.SetMat4("projection", projection);
	lampShader.SetMat4("view", view);
	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
	lampShader.SetMat4("model", model);

	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(Mng->window->window);
	glfwPollEvents();

	return true;
}

bool ControllerRender::CleanUp()
{
	// optional: de-allocate all resources once they've outlived their purpose:
// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	return true;
}
