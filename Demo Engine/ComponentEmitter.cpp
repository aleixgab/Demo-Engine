#include "ComponentEmitter.h"
#include <imgui\imgui.h>
#include "GameManager.h"
#include "Controller.h"
#include "ControllerRender.h"
#include "TextureImporter.h"

#include "GameObject.h"
#include <ParticleManager.h>
#include <string>

ComponentEmitter::ComponentEmitter(GameObject* gameObject, ControllerParticles* controller, int maxParticles) : Component(gameObject, ComponentType_EMITTER)
{
	emitter = Part::CreateEmitter(&gameObject->transform->position.x, maxParticles);
	this->controller = controller;
	this->maxParticles = maxParticles;
}

ComponentEmitter::~ComponentEmitter()
{
	Part::RemoveEmitter(emitter);
}

void ComponentEmitter::Inspector()
{
	ImGui::Separator();
	ImGui::Text("Particle Emitter");
	ImGui::Separator();
	ImGui::InputInt("Max Particles", &maxParticles);
	if (ImGui::Button("Change", ImVec2(125, 25)))
		emitter->ChangeMaxParticles(maxParticles);
StartValuesInsp();
	
	BurstInsp();

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
		ShowFloatValue(&emitter->particleValues.speed.x, checkSpeed, "Speed", 0.25f, 0.25f, 20.0f);

		ImGui::Checkbox("##Acceleration", &checkAcceleration);
		ShowFloatValue(&emitter->particleValues.acceleration.x, checkAcceleration, "Acceleration", 0.25f, -5.0f, 5.0f);

		ImGui::PushItemWidth(180.0f);
		ImGui::DragFloat3("Acceleration 3D", &emitter->particleValues.gravity.x);
		ImGui::PopItemWidth();

		ImGui::Checkbox("##Rotation", &checkRotation);
		ShowFloatValue(&emitter->particleValues.rotation.x, checkRotation, "Initial Rotation", 0.25f, -360.0f, 360.0f);

		ImGui::Checkbox("##AngularVelocity", &checkAngularVelocity);
		ShowFloatValue(&emitter->particleValues.angularVelocity.x, checkAngularVelocity, "Angular Velocity", 0.25f, -45.0f, 45.0f);

		ImGui::Checkbox("##AngularAcceleration", &checkAngularAcceleration);
		ShowFloatValue(&emitter->particleValues.angularAcceleration.x, checkAngularAcceleration, "Angular Acceleration", 0.25f, -45.0f, 45.0f);

		ImGui::Checkbox("##Lifetime", &checkLife);
		ShowFloatValue(&emitter->particleValues.life.x, checkLife, "Lifetime", 0.5f, 1.0f, 20.0f);

		ImGui::Checkbox("##Size", &checkSize);
		ShowFloatValue(&emitter->particleValues.size.x, checkSize, "Size", 0.1f, 0.1f, 5.0f);

		ImGui::Checkbox("##SizeOverTime", &checkSizeOverTime);
		ShowFloatValue(&emitter->particleValues.sizeOverTime.x, checkSizeOverTime, "SizeOverTime", 0.25f, -1.0f, 1.0f);

		ImGui::PushItemWidth(200.0f);
		ImGui::DragInt("Emition", &emitter->particlesEmition, 1.0f, 0.0f, 300.0f, "%.2f");
		ImGui::PopItemWidth();

	}
}

void ComponentEmitter::ShowFloatValue(float* value, bool checkBox, const char* name, float v_speed, float v_min, float v_max)
{
	ImGui::SameLine();
	if (checkBox)
	{
		ImGui::PushItemWidth(70.0f);
		std::string str = "##";
		str.append(name);
		str.append("min");
		if (ImGui::DragFloat(str.data(), &value[0], v_speed, v_min, v_max, "%.2f"))
			CheckMinMax(value);
		ImGui::SameLine();
		if (ImGui::DragFloat(name, &value[1], v_speed, v_min, v_max, "%.2f"))
			CheckMinMax(value);
	}
	else
	{
		ImGui::PushItemWidth(148.0f);
		if (ImGui::DragFloat(name, &value[0], v_speed, v_min, v_max, "%.2f"))
			value[1] = value[0];
	}
	ImGui::PopItemWidth();
}

void ComponentEmitter::CheckMinMax(float* value)
{
	if (value[0] > value[1])
		value[1] = value[0];
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
				emitter->shapeEmitter == ShapeEmitter::BoxShape;
			else if (ImGui::MenuItem("Sphere"))
				emitter->shapeEmitter == ShapeEmitter::SphereShape;
			else if (ImGui::MenuItem("Cone"))
				emitter->shapeEmitter == ShapeEmitter::ConeShape;
				ImGui::End();
		}

		switch (emitter->shapeEmitter)
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

			if (ImGui::RadioButton("Random", emitter->shapeEmitter == SphereShape))
				emitter->shapeEmitter = ShapeEmitter::SphereShape;
			ImGui::SameLine();
			if (ImGui::RadioButton("Center", emitter->shapeEmitter == SphereShapeCenter))
				emitter->shapeEmitter = ShapeEmitter::SphereShapeCenter;
			ImGui::SameLine();
			if (ImGui::RadioButton("Border", emitter->shapeEmitter == SphereShapeBorder))
				emitter->shapeEmitter = ShapeEmitter::SphereShapeBorder;

			ImGui::DragFloat("Sphere Size", &emitter->sphereShapeRad, 0.25f, 1.0f, 20.0f, "%.2f");

			break;
		case ConeShape:
			ImGui::Text("Cone");
			ImGui::DragFloat("Cone Circle Rad", &emitter->coneShapeRad, 0.25f, 0.25f, 20.0f, "%.2f");
			ImGui::DragFloat("Height Cone", &emitter->coneShapeHeight, 0.25f, 0.25f, 20.0f, "%.2f");
			break;
		default:
			break;
		}
	}
}
#pragma endregion

#pragma region Burst
void ComponentEmitter::BurstInsp()
{
	if (ImGui::CollapsingHeader("Particle Burst"))
	{
		ImGui::Checkbox("Burst", &emitter->isBurst);
		if (ImGui::BeginMenu("Shape"))
		{
			if (ImGui::MenuItem("Box"))
				emitter->burstShapeEmitter = ShapeEmitter::BoxShape;
			else if (ImGui::MenuItem("Sphere"))
				emitter->burstShapeEmitter = ShapeEmitter::SphereShape;
			else if (ImGui::MenuItem("Sphere From center"))
				emitter->burstShapeEmitter = ShapeEmitter::SphereShapeCenter;
			else if (ImGui::MenuItem("Sphere From Surface"))
				emitter->burstShapeEmitter = ShapeEmitter::SphereShapeBorder;
			else if (ImGui::MenuItem("Cone"))
				emitter->burstShapeEmitter = ShapeEmitter::ConeShape;
			ImGui::End();
		}
		ImGui::DragInt("Min particles", &emitter->minBurst, 1.0f, 0, 100);
		if (emitter->minBurst > emitter->maxBurst)
			emitter->maxBurst = emitter->minBurst;
		ImGui::DragInt("Max Particles", &emitter->maxBurst, 1.0f, 0, 100);
		if (emitter->maxBurst < emitter->minBurst)
			emitter->minBurst = emitter->maxBurst;

		ImGui::DragFloat("Seconds between Burst", &emitter->burstSeconds, 0.5f, 0.0f, 0.0f, "%.1f");

		ImGui::Separator();
	}
}
#pragma endregion

#pragma region ColorValues
void ComponentEmitter::ColorValuesInsp()
{
	if (ImGui::CollapsingHeader("Particle Color"))
	{
		ImGui::Text("Particle Color");
		ImGui::Separator();
		std::list<float> positions;
		emitter->GetAllPositions(positions);

		for (std::list<float>::iterator iter = positions.begin(); iter != positions.end(); ++iter)
		{
			glm::vec4 color;
			emitter->GetColor(&color.x, *iter);
			if (!EditColor(&color.x, *iter))
				emitter->EraseColor(*iter);
		}

		ImGui::Separator();
		ImGui::Checkbox("Color time", &emitter->activeMulticolor);
		if (emitter->activeMulticolor)
		{

			ImGui::DragInt("Position", &nextPos, 1.0f, 1, 100);
			ImGui::ColorPicker4("", &nextColor.x, ImGuiColorEditFlags_AlphaBar);
			if (ImGui::Button("Add Color", ImVec2(125, 25)))
			{
				emitter->AddColor(&nextColor.x,(nextPos/100.0f));
			}
		}
	}
}

bool ComponentEmitter::EditColor(float* color, float position)
{
	bool ret = true;
	ImVec4 ImColor;
	ImColor.x = color[0];
	ImColor.y = color[1];
	ImColor.z = color[2];
	ImColor.w = color[3];

	if (ImGui::ColorEdit4(std::to_string(position).c_str(), color, ImGuiColorEditFlags_AlphaBar))
		emitter->EditColor(color, position);

	if (position > 0)
	{
		std::string colorStr = "Remove Color ";
		colorStr.append(std::to_string(position));
		if (ImGui::Button(colorStr.data(), ImVec2(125, 25)))
			ret = false;
	}

	return ret;
}
#pragma endregion

#pragma region TextureValues
void ComponentEmitter::TextureValuesInsp()
{
	if (ImGui::CollapsingHeader("Particle Texture", ImGuiTreeNodeFlags_FramePadding))
	{
		ImGui::Checkbox("Using texture", &emitter->useTexture);

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
					emitter->textureRows = 1;
					emitter->textureColumns = 1;
					emitter->dieOnFinishAnim = false;
				}
			}
			if (emitter->isParticleAnimated)
			{
				ImGui::DragFloat("Animation Speed", &emitter->animationSpeed, 0.001f, 0.0f, 5.0f);

				ImGui::DragInt("Rows", &emitter->textureRows, 1, 1, 10);
				ImGui::DragInt("Columns", &emitter->textureColumns, 1, 1, 10);

				ImGui::Checkbox("Kill particle with animation", &emitter->dieOnFinishAnim);
				if (emitter->dieOnFinishAnim)
				{
					checkLife = false;
					emitter->particleValues.life.x = emitter->animationSpeed * (emitter->textureColumns * emitter->textureRows - 1);
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