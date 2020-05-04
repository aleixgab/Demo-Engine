#include "ComponentEmitter.h"
#include <imgui\imgui.h>
#include "GameManager.h"
#include "Controller.h"
#include "ControllerRender.h"
#include "TextureImporter.h"

#include "GameObject.h"
#include "ParticleLibrary/ParticleManager.h"
#include <string>

ComponentEmitter::ComponentEmitter(GameObject* gameObject, ParticleManager* manager, ControllerParticles* controller) : Component(gameObject, ComponentType_EMITTER)
{
	emitter = manager->CreateEmitter(&gameObject->transform->position.x);
	this->controller = controller;
}

ComponentEmitter::~ComponentEmitter()
{
	emitter->parent->RemoveEmitter(emitter);
}

void ComponentEmitter::Inspector()
{
	ImGui::Separator();
	ImGui::Text("Particle Emitter");
	ImGui::Separator();
	StartValuesInsp();
	
	ShapeValuesInsp();

	ColorValuesInsp();

	TextureValuesInsp();
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

void ComponentEmitter::ShowFloatValue(PartVec2& value, bool checkBox, const char* name, float v_speed, float v_min, float v_max)
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

void ComponentEmitter::CheckMinMax(PartVec2& value)
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
		ImGui::SliderFloat("Color Percentage", &emitter->colorPercent, 0.0f, 1.0f, "%.2f");
		ImGui::Checkbox("Color time", &emitter->startValues.isMulticolor);
		if (emitter->startValues.isMulticolor)
		{

			ImGui::DragInt("Position", &nextPos, 1.0f, 1, 100);
			ImGui::ColorPicker4("", &nextColor.x, ImGuiColorEditFlags_AlphaBar);
			if (ImGui::Button("Add Color", ImVec2(125, 25)))
			{
				ParticleColor colorTime;
				colorTime.color.x = nextColor.x;
				colorTime.color.y = nextColor.y;
				colorTime.color.z = nextColor.z;
				colorTime.color.w = nextColor.w;
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

#pragma region TextureValues
void ComponentEmitter::TextureValuesInsp()
{
	if (ImGui::CollapsingHeader("Particle Texture", ImGuiTreeNodeFlags_FramePadding))
	{
		if (emitter->textureID > 0u)
		{
			ImGui::Text("Texture num '%i'", emitter->textureID);

			ImGui::Image((void*)emitter->textureID, ImVec2(256.0f, 256.0f), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

			if (ImGui::BeginMenu("Change Texture"))
			{
				float sz = ImGui::GetTextLineHeight();
				for (std::list<TextureImporter*>::iterator iter = controller->Mng->render->textures.begin(); iter != controller->Mng->render->textures.end(); ++iter)
				{
					ImVec2 p = ImGui::GetCursorScreenPos();
					ImGui::Dummy(ImVec2(sz, sz));
					ImGui::SameLine();
					if (ImGui::MenuItem((*iter)->texture.name.c_str()))
						emitter->textureID = (*iter)->texture.id;
				}
				ImGui::EndMenu();
			}

			if (ImGui::Checkbox("Animated sprite", &emitter->isParticleAnimated))
			{
				if (!emitter->isParticleAnimated)
				{
					emitter->particleAnimation.textureRows = 1;
					emitter->particleAnimation.textureColumns = 1;
					emitter->dieOnFinishAnim = false;
				}
			}
			if (emitter->isParticleAnimated)
			{
				if (ImGui::Checkbox("##AnimationSpeed", &checkAnimationSpeed))
					emitter->startValues.acceleration.y = emitter->startValues.acceleration.x;
				ShowFloatValue(emitter->particleAnimation.animationSpeed, checkAnimationSpeed, "Animation Speed", 0.001, 0.0f, 5.0f);

				if (ImGui::DragInt("Rows", &emitter->particleAnimation.textureRows, 1, 1, 10))
					emitter->particleAnimation.textureRowsNorm = 1.0f / emitter->particleAnimation.textureRows;
				if (ImGui::DragInt("Columns", &emitter->particleAnimation.textureColumns, 1, 1, 10))
					emitter->particleAnimation.textureColumnsNorm = 1.0f / emitter->particleAnimation.textureColumns;

				ImGui::Checkbox("Kill particle with animation", &emitter->dieOnFinishAnim);
				ImGui::Checkbox("Random Starting Frame", &emitter->particleAnimation.isAnimRand);
				if (emitter->dieOnFinishAnim)
				{
					checkLife = false;
					emitter->startValues.life.x = emitter->particleAnimation.animationSpeed.x * (emitter->particleAnimation.textureColumns * emitter->particleAnimation.textureRows - 1);
				}
			}
			
			if (ImGui::Button("Remove Texture", ImVec2(125, 25)))
			{
				emitter->textureID = 0u;
			}
		}
		else
		{
			if (ImGui::BeginMenu("Add New Texture"))
			{
				float sz = ImGui::GetTextLineHeight();
				for (std::list<TextureImporter*>::iterator iter = controller->Mng->render->textures.begin() ; iter != controller->Mng->render->textures.end(); ++iter)
				{
					ImVec2 p = ImGui::GetCursorScreenPos();
					ImGui::Dummy(ImVec2(sz, sz));
					ImGui::SameLine();
					if (ImGui::MenuItem((*iter)->texture.name.c_str()))
						emitter->textureID = (*iter)->texture.id;
				}
				ImGui::EndMenu();
			}
		}
		ImGui::Separator();
	}
}

#pragma endregion