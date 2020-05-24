#include <glad/glad.h>
#include "GameManager.h"

#include "ControllerRender.h"
#include "ControllerParticles.h"
#include <ParticleSystem.h>
#include "GroundImporter.h"
#include "ControllerWindow.h"
#include "TextureImporter.h"
#include <Brofiler/Brofiler.h>

ControllerRender::ControllerRender(GameManager* mng) : Controller(mng)
{
}

ControllerRender::~ControllerRender()
{
}

bool ControllerRender::Start()
{

	basicShader.SetShader("Shaders/VertexShader.txt", "Shaders/FragmentShader.txt");
	particleShader.SetShader("Shaders/Particle_VShader.txt", "Shaders/Particle_FShader.txt");

	TextureImporter* newTexture = new TextureImporter("Assets/texture.jpg");
	TextureImporter* newTexture2 = new TextureImporter("Assets/texture2.jpg");
	TextureImporter* newTexture3 = new TextureImporter("Assets/awesomeface.png");
	TextureImporter* newTexture4 = new TextureImporter("Assets/Smoke30Frames_0.png");
	TextureImporter* newTexture5 = new TextureImporter("Assets/smokeTest.jpg");
	TextureImporter* newTexture6 = new TextureImporter("Assets/FlameRoundParticleSheet.png");
	TextureImporter* newTexture7 = new TextureImporter("Assets/Fire_sprites_v1.png");
	textures.push_back(newTexture);
	textures.push_back(newTexture2);
	textures.push_back(newTexture3);
	textures.push_back(newTexture4);
	textures.push_back(newTexture5);
	textures.push_back(newTexture6);
	textures.push_back(newTexture7);

	ground = new GroundImporter();
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

	if ((*toDraw.begin())->isActive)
	{
			basicShader.UseProgram();
			basicShader.SetMat4("view", view);
			basicShader.SetMat4("projection", projection);
			basicShader.SetMat4("model", (*toDraw.begin())->GetComponentTransform()->GetTransform());
			basicShader.SetVec3("uColor", 1.0f, 1.0f, 1.0f);

		// render Plane
			glBindVertexArray(ground->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	std::list<ParticleEmitter*> emitterList;
	Part::GetEmitters(emitterList);
	{
		BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::PapayaWhip);
		Part::Draw(particleShader.uid, &view[0][0], &projection[0][0], emitterList);
	}
	Mng->gui->Draw();
	int display_w, display_h;
	glfwGetFramebufferSize(Mng->window->window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

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
	
	delete ground;
	return true;
}
