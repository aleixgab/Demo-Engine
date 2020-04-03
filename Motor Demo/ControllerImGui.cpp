#include "ControllerImGui.h"
#include "ControllerWindow.h"
#include "GameManager.h" 
#include "GameObject.h"

#include "imgui/imgui.h"
#include "3rdPart/includes/imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

ControllerImGui::ControllerImGui(GameManager* mng) : Controller(mng)
{
}

ControllerImGui::~ControllerImGui()
{
}

bool ControllerImGui::Start()
{
	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(Mng->window->window, true);
	ImGui_ImplOpenGL3_Init("#version 130");



	return true;
}

bool ControllerImGui::Update(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
	ObjHierarchy();

	return true;
}

void ControllerImGui::ObjHierarchy()
{
	bool alwaysOpen = true;
	bool show_another_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (ImGui::Begin("Scene", &alwaysOpen))
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("New Game Object"))
			{
				std::string str = "Random Object ";
				str.append(std::to_string(rand() % 100));
				Mng->gameObject->CreateGameObject(glm::vec3(0.0f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(1.0f), str.data());
			}

			ImGui::MenuItem("Close");
			ImGui::EndMenu();
		}
		ImGui::Separator();
		ImGui::End();
	}
}

void ControllerImGui::Draw()
{
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool ControllerImGui::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return true;
}
