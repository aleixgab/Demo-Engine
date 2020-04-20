#include <glad/glad.h>
#include "GameManager.h"

#include "ControllerRender.h"
#include "ControllerParticles.h"
#include "ParticleLibrary/ParticleManager.h"
#include "ParticleLibrary/PlaneImporter.h"
#include "ControllerWindow.h"
#include "TextureImporter.h"

ControllerRender::ControllerRender(GameManager* mng) : Controller(mng)
{
}

ControllerRender::~ControllerRender()
{
}

bool ControllerRender::Start()
{
	lightingShader.SetShader("VShader_BasicLight.txt", "FShader_BasicLight.txt");
	lampShader.SetShader("VShader_Lamp.txt", "FShader_Lamp.txt");
	particleShader.SetShader("ParticleLibrary/Shaders/Particle_VShader.txt", "ParticleLibrary/Shaders/Particle_FShader.txt");

	TextureImporter* newTexture = new TextureImporter("Assets/texture.jpg");
	TextureImporter* newTexture2 = new TextureImporter("Assets/texture2.jpg");
	TextureImporter* newTexture3 = new TextureImporter("Assets/awesomeface.png");
	textures.push_back(newTexture);
	textures.push_back(newTexture2);
	textures.push_back(newTexture3);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);



	return true;
}

bool ControllerRender::Update(float dt)
{
	// render
	// ------
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::list<GameObject*> toDraw;
	Mng->gameObject->GetGameObjects(toDraw);

	// view/projection transformations
	glm::mat4 projection = glm::perspective(Mng->scene->camera->zoom, (float)Mng->window->SCR_WIDTH / (float)Mng->window->SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = Mng->scene->camera->GetViewMatrix();

	for (std::list<GameObject*>::iterator iterator = toDraw.begin(); iterator != toDraw.end(); ++iterator)
	{
		if ((*iterator)->isActive && iterator == toDraw.begin())
		{
			lightingShader.UseProgram();
			lightingShader.SetMat4("projection", projection);
			lightingShader.SetMat4("view", view);
			lightingShader.SetMat4("model", (*iterator)->GetComponentTransform()->GetTransform());

			// be sure to activate shader when setting uniforms/drawing objects
			lightingShader.SetVec3("objectColor", 1.0f, 0.5f, 0.31f);
			lightingShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
			lightingShader.SetVec3("lightPos", 0.0f, 0.0f, 0.0f);
			lightingShader.SetVec3("viewPos", Mng->scene->camera->Position);


			// render the cube
			glBindVertexArray(Mng->particle->particleManager->plane->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}

	Mng->particle->particleManager->Draw(particleShader.uid, view, projection);
	Mng->gui->Draw();
	int display_w, display_h;
	glfwGetFramebufferSize(Mng->window->window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(Mng->window->window);
	glfwPollEvents();

	return true;
}

bool ControllerRender::CleanUp()
{
	for (std::list<TextureImporter*>::iterator iterator = textures.begin(); iterator != textures.end(); ++iterator)
	{
		delete (*iterator);
	}
	textures.clear();
	return true;
}
