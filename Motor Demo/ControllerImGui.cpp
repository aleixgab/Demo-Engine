#include "ControllerImGui.h"
#include "ControllerWindow.h"
#include "GameManager.h" 
#include "GameObject.h"

#include "imgui/imgui.h"
#include "imgui/imgui_dock.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

ControllerImGui::ControllerImGui(GameManager* mng) : Controller(mng)
{
}

ControllerImGui::~ControllerImGui()
{
}

bool ControllerImGui::Start()
{
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(Mng->window->window, true);

	ImGui::InitDock();

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
	if (ImGui::Begin("Scene", &alwaysOpen, ImGuiWindowFlags_NoMove))


		//Options menu
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("New Game Object"))
			{
				std::string str = "Random Object ";
				str.append(std::to_string(rand() % 100));
				Mng->gameObject->CreateGameObject(glm::vec3(0.0f), glm::quat(), glm::vec3(2.0f), str.data());
			}

			ImGui::MenuItem("Close");
			ImGui::EndMenu();
		}
	ImGui::Separator();

	////Iterate all GO in sceene
	//if (App->gameObject->rootGameObject)
	//{
	//	CreateGOTreeNode(App->gameObject->rootGameObject);
	//	if (objectDrag)
	//		objectDrag = false; //put false for next frame
	//}
	////Call only one time OpenPopup
	//if (popHierarchy)
	//{
	//	ImGui::OpenPopup("GameObjectsPop");
	//	popHierarchy = false;
	//}
	////Opened every frame if popup its opened
	//if (ImGui::BeginPopup("GameObjectsPop"))
	//{
	//	std::string str = "New Game Object in ";
	//	str.append(objectSelected->name);
	//	if (ImGui::MenuItem(str.data()))
	//	{
	//		str = "Random Object ";
	//		str.append(std::to_string(rand() % 100));
	//		App->gameObject->CreateGameObject(float3::zero, Quat::identity, float3::one, objectSelected, str.data());
	//	}
	//	if (ImGui::MenuItem("Delete"))
	//		objectSelected->Delete();
	//	ImGui::EndPopup();
	//}
	//else if (objectSelected)
	//{
	//	objectSelected = nullptr;
	//}

	ImGui::SetWindowPos({ 0.0f,0.0f });
	ImGui::SetWindowSize({ 50.0f, 500.0f});
	ImGui::End();
}


bool ControllerImGui::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return true;
}
