#include "ComponentEmitter.h"
#include <imgui\imgui.h>

#include "GameObject.h"
#include "ParticleLibrary/ParticleManager.h"
#include <string>

ComponentEmitter::ComponentEmitter(GameObject* gameObject, ParticleManager* manager) : Component(gameObject, ComponentType_EMITTER)
{
	emitter = manager->CreateEmitter();
}

ComponentEmitter::~ComponentEmitter()
{
	emitter->parent->RemoveEmitter(emitter);
}

void ComponentEmitter::Inspector()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::CollapsingHeader("Particle Values", ImGuiTreeNodeFlags_DefaultOpen))
		{

			ImGui::Checkbox("##Speed", &checkSpeed);
			ShowFloatValue(emitter->startValues.speed, checkSpeed, "Speed", 0.25f, 0.25f, 20.0f);

			ImGui::Checkbox("##Acceleration", &checkAcceleration);
			ShowFloatValue(emitter->startValues.acceleration, checkAcceleration, "Acceleration", 0.25f, -5.0f, 5.0f);

			ImGui::Checkbox("##Rotation", &checkRotation);
			ShowFloatValue(emitter->startValues.rotation, checkRotation, "Initial Rotation", 0.25f, -360.0f, 360.0f);

			ImGui::Checkbox("##AngularVelocity", &checkAngularVelocity);
			ShowFloatValue(emitter->startValues.angularVelocity, checkAngularVelocity, "Angular Velocity", 0.25f, -45.0f, 45.0f);

			ImGui::Checkbox("##AngularAcceleration", &checkAngularAcceleration);
			ShowFloatValue(emitter->startValues.angularAcceleration, checkAngularAcceleration, "Angular Acceleration", 0.25f, -45.0f, 45.0f);

			ImGui::Checkbox("##Lifetime", &checkLife);
			ShowFloatValue(emitter->startValues.life, checkLife, "Lifetime", 0.5f, 1.0f, 20.0f);

			ImGui::Checkbox("##Size", &checkSize);
			ShowFloatValue(emitter->startValues.size, checkSize, "Size", 0.1f, 0.1f, 5.0f);

			ImGui::Checkbox("##SizeOverTime", &checkSizeOverTime);
			ShowFloatValue(emitter->startValues.sizeOverTime, checkSizeOverTime, "SizeOverTime", 0.25f, -1.0f, 1.0f);

			ImGui::DragInt("Emition", &emitter->particlesEmition, 1.0f, 0.0f, 300.0f, "%.2f");
			/*
			ImGui::Separator();
			if (ImGui::Checkbox("Loop", &loop))
				loopTimer.Start();
			ImGui::DragFloat("Duration", &duration, 0.5f, 0.5f, 20.0f, "%.2f");*/
		}
	}
}

void ComponentEmitter::ShowFloatValue(glm::vec2& value, bool checkBox, const char* name, float v_speed, float v_min, float v_max)
{
	ImGui::SameLine();
	if (checkBox)
	{
		ImGui::PushItemWidth(70.0f);
		std::string str = "##";
		str.append(name);
		str.append("min");
		if (ImGui::DragFloat(str.data(), &value.x, v_speed, v_min, v_max, "%.2f"))
			CheckMinMax(value);
		ImGui::SameLine();
		if (ImGui::DragFloat(name, &value.y, v_speed, v_min, v_max, "%.2f"))
			CheckMinMax(value);
	}
	else
	{
		ImGui::PushItemWidth(148.0f);
		if (ImGui::DragFloat(name, &value.x, v_speed, v_min, v_max, "%.2f"))
			value.y = value.x;
	}
	ImGui::PopItemWidth();
}

void ComponentEmitter::CheckMinMax(glm::vec2& value)
{
	if (value.x > value.y)
		value.y = value.x;
}