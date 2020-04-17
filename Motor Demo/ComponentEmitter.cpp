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
	StartValuesInsp();
	
	ShapeValuesInsp();

	ColorValuesInsp();
}

#pragma region StartValues
void ComponentEmitter::StartValuesInsp()
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
#pragma endregion

#pragma region ShpapeValues 
void ComponentEmitter::ShapeValuesInsp()
{
	if (ImGui::CollapsingHeader("Particle Shape"))
	{
		ImGui::Separator();
		if (ImGui::BeginMenu("Change Shape"))
		{
			if (ImGui::MenuItem("Box"))
				emitter->SetShapeEmitter(ShapeEmitter::BoxShape);
			else if (ImGui::MenuItem("Sphere"))
				emitter->SetShapeEmitter(ShapeEmitter::SphereShape);
			else if (ImGui::MenuItem("Cone"))
				emitter->SetShapeEmitter(ShapeEmitter::ConeShape);
			ImGui::End();
		}

		ShapeEmitter tempEmitter = emitter->GetShapeEmitter();
		switch (tempEmitter)
		{
		case BoxShape:
			ImGui::Text("Box");
			ImGui::DragFloat3("Box Size", &emitter->boxShapeSize.x, 0.1f, 0.1f, 20.0f, "%.2f");

			break;
		case SphereShape:
		case SphereShapeCenter:
		case SphereShapeBorder:
			ImGui::Text("Sphere");

			ImGui::Text("Particle emision from:");

			if (ImGui::RadioButton("Random", tempEmitter == SphereShape))
				emitter->SetShapeEmitter(ShapeEmitter::SphereShape);
			ImGui::SameLine();
			if (ImGui::RadioButton("Center", tempEmitter == SphereShapeCenter))
				emitter->SetShapeEmitter(ShapeEmitter::SphereShapeCenter);
			ImGui::SameLine();
			if (ImGui::RadioButton("Border", tempEmitter == SphereShapeBorder))
				emitter->SetShapeEmitter(ShapeEmitter::SphereShapeBorder);

			ImGui::DragFloat("Sphere Size", &emitter->sphereShapeRad, 0.25f, 1.0f, 20.0f, "%.2f");

			break;
		case ConeShape:
			ImGui::Text("Cone");
			ImGui::DragFloat("Sphere Size", &emitter->coneShapeRad, 0.25f, 0.25f, 20.0f, "%.2f");

			break;
		default:
			break;
		}
	}
}
#pragma endregion

#pragma region ColorValues
void ComponentEmitter::ColorValuesInsp()
{
	if (ImGui::CollapsingHeader("Particle Color"))
	{
		ImGui::Text("Particle Color");
		ImGui::SameLine();
		std::list<ParticleColor>::iterator iter = emitter->startValues.colorList.begin();
		uint posList = 0u;
		while (iter != emitter->startValues.colorList.end())
		{
			//TODO: they must be able to change position
			if ((iter) == emitter->startValues.colorList.begin())
			{//Cant delete 1st color

				if (!EditColor(*iter))
					break;
				iter++;
			}
			else
			{
				if (!EditColor(*iter, posList))
					emitter->startValues.colorList.erase(iter++);
				else
					iter++;
			}
			++posList;
		}
		ImGui::Separator();
		ImGui::Checkbox("Color time", &emitter->startValues.isMulticolor);
		if (emitter->startValues.isMulticolor)
		{

			ImGui::DragInt("Position", &nextPos, 1.0f, 1, 100);
			ImGui::ColorPicker4("", &nextColor.x, ImGuiColorEditFlags_AlphaBar);
			if (ImGui::Button("Add Color", ImVec2(125, 25)))
			{
				ParticleColor colorTime;
				colorTime.color = nextColor;
				colorTime.position = (float)nextPos / 100;
				colorTime.name = std::to_string((int)nextPos) + "%";
				emitter->startValues.colorList.push_back(colorTime);
				emitter->startValues.colorList.sort();
			}
		}
	}
}

bool ComponentEmitter::EditColor(ParticleColor& colorTime, uint pos)
{
	bool ret = true;
	ImVec4 color;
	color.x = colorTime.color.x;
	color.y = colorTime.color.y;
	color.z = colorTime.color.z;
	color.w = colorTime.color.w;

	if (ImGui::ColorButton(colorTime.name.data(), color, ImGuiColorEditFlags_None, ImVec2(100, 20)))
		colorTime.changingColor = !colorTime.changingColor;

	if (!colorTime.changingColor)
	{
		ImGui::SameLine();
		ImGui::TextUnformatted(colorTime.name.data());
		if (pos > 0)
		{
			std::string colorStr = "Remove Color ";
			colorStr.append(std::to_string(pos));
			ImGui::SameLine();
			if (ImGui::Button(colorStr.data(), ImVec2(125, 25)))
				ret = false;
		}
		else if (!emitter->startValues.isMulticolor)
			ret = false;
	}
	else
		ImGui::ColorEdit4(colorTime.name.data(), &colorTime.color.x, ImGuiColorEditFlags_AlphaBar);

	return ret;
}
#pragma endregion