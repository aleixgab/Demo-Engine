#include "ComponentEmitter.h"
#include <imgui\imgui.h>
#include "GameManager.h"
#include "ControllerRender.h"
#include "TextureImporter.h"

#include "GameObject.h"
#include <string>

ComponentEmitter::ComponentEmitter(GameObject* gameObject, ControllerParticles* controller, int maxParticles) : Component(gameObject, ComponentType_EMITTER)
{
	emitter = Part::CreateEmitter(&gameObject->transform->position.x, maxParticles);
	this->controller = controller;
	if (maxParticles <= 0)
		maxParticles = 1;
	this->maxParticles = maxParticles;
	particleValues = emitter->GetParticleValues();
	emitterValues = emitter->GetEmitterValues();

	SetMap();
}

void ComponentEmitter::SetMap()
{
	std::list<float> pos;
	std::list<float*> color;
	emitter->GetAllPositions(pos);
	emitter->GetAllColors(color);

	std::list<float*>::iterator iterColor = color.begin();
	for (std::list<float>::iterator iterPos = pos.begin(); iterPos != pos.end(); ++iterPos, ++iterColor)
	{
		colorMap.insert(std::pair<float, glm::vec4>(*iterPos, glm::vec4((*iterColor)[0], (*iterColor)[1], (*iterColor)[2], (*iterColor)[3])));
	}
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

	if (particleChange)
	{
		emitter->SetParticleValues(particleValues);
		particleChange = false;
	}
	if (emitterChange)
	{
		emitter->SetEmitterValues(emitterValues);
		emitterChange = false;	
	}
}

#pragma region StartValues
void ComponentEmitter::StartValuesInsp()
{
	if (ImGui::CollapsingHeader("Particle Values", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("##Speed", &checkSpeed);
		ShowFloatValue(&particleValues.speed.x, checkSpeed, "Speed", 0.25f, 0.25f, 20.0f);

		ImGui::Checkbox("##Acceleration", &checkAcceleration);
		ShowFloatValue(&particleValues.acceleration.x, checkAcceleration, "Acceleration", 0.25f, -5.0f, 5.0f);

		ImGui::PushItemWidth(180.0f);
		if (ImGui::DragFloat3("Acceleration 3D", &particleValues.gravity.x))
			particleChange = true;
		ImGui::PopItemWidth();

		ImGui::Checkbox("##Rotation", &checkRotation);
		ShowFloatValue(&particleValues.rotation.x, checkRotation, "Initial Rotation", 0.25f, -360.0f, 360.0f);

		ImGui::Checkbox("##AngularVelocity", &checkAngularVelocity);
		ShowFloatValue(&particleValues.angularVelocity.x, checkAngularVelocity, "Angular Velocity", 0.25f, -45.0f, 45.0f);

		ImGui::Checkbox("##AngularAcceleration", &checkAngularAcceleration);
		ShowFloatValue(&particleValues.angularAcceleration.x, checkAngularAcceleration, "Angular Acceleration", 0.25f, -45.0f, 45.0f);

		ImGui::Checkbox("##Lifetime", &checkLife);
		ShowFloatValue(&particleValues.life.x, checkLife, "Lifetime", 0.5f, 1.0f, 20.0f);

		ImGui::Checkbox("##Size", &checkSize);
		ShowFloatValue(&particleValues.size.x, checkSize, "Size", 0.1f, 0.1f, 5.0f);

		ImGui::Checkbox("##SizeOverTime", &checkSizeOverTime);
		ShowFloatValue(&particleValues.sizeOverTime.x, checkSizeOverTime, "SizeOverTime", 0.25f, -1.0f, 1.0f);

		ImGui::PushItemWidth(200.0f);
		if(ImGui::DragInt("Emition", &particleValues.particlesEmition, 1.0f, 0.0f, 300.0f, "%.2f"))
			particleChange = true;
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
		{
			value[1] = value[0];
			particleChange = true;
		}
	}
	ImGui::PopItemWidth();
}

void ComponentEmitter::CheckMinMax(float* value)
{
	if (value[0] > value[1])
		value[1] = value[0];
	particleChange = true;
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
			{
				emitterValues.shapeEmitter = ShapeEmitter::BoxShape;
				emitterChange = true;
			}
			else if (ImGui::MenuItem("Sphere"))
			{
				emitterValues.shapeEmitter = ShapeEmitter::SphereShape;
				emitterChange = true;
			}
			else if (ImGui::MenuItem("Cone"))
			{
				emitterValues.shapeEmitter = ShapeEmitter::ConeShape;
				emitterChange = true;
			}
				ImGui::End();
		}

		switch (emitterValues.shapeEmitter)
		{
		case BoxShape:
			ImGui::Text("Box");
			if(ImGui::DragFloat3("Box Size", &emitterValues.boxShapeSize.x, 0.1f, 0.1f, 20.0f, "%.2f"))
				emitterChange = true;

			break;
		case SphereShape:
		case SphereShapeCenter:
		case SphereShapeBorder:
			ImGui::Text("Sphere");
			ImGui::Text("Particle emision from:");

			if (ImGui::RadioButton("Random", emitterValues.shapeEmitter == SphereShape))
			{
				emitterValues.shapeEmitter = ShapeEmitter::SphereShape;
				emitterChange = true;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Center", emitterValues.shapeEmitter == SphereShapeCenter))
			{
				emitterValues.shapeEmitter = ShapeEmitter::SphereShapeCenter;
				emitterChange = true;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Border", emitterValues.shapeEmitter == SphereShapeBorder))
			{
				emitterValues.shapeEmitter = ShapeEmitter::SphereShapeBorder;
				emitterChange = true;
			}

			if(ImGui::DragFloat("Sphere Size", &emitterValues.sphereShapeRad, 0.25f, 1.0f, 20.0f, "%.2f"))
				emitterChange = true;

			break;
		case ConeShape:
			ImGui::Text("Cone");
			if(ImGui::DragFloat("Cone Circle Rad", &emitterValues.coneShapeRad, 0.25f, 0.25f, 20.0f, "%.2f"))
				emitterChange = true;
			if(ImGui::DragFloat("Height Cone", &emitterValues.coneShapeHeight, 0.25f, 0.25f, 20.0f, "%.2f"))
				emitterChange = true;
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
		if(ImGui::Checkbox("Burst", &emitterValues.isBurst))
			emitterChange = true;

		if (ImGui::BeginMenu("Shape"))
		{
			if (ImGui::MenuItem("Box"))
			{
				emitterValues.burstShapeEmitter = ShapeEmitter::BoxShape;
				emitterChange = true;
			}
			else if (ImGui::MenuItem("Sphere"))
			{
				emitterValues.burstShapeEmitter = ShapeEmitter::SphereShape;
				emitterChange = true;
			}
			else if (ImGui::MenuItem("Sphere From center"))
			{
				emitterValues.burstShapeEmitter = ShapeEmitter::SphereShapeCenter;
				emitterChange = true;
			}
			else if (ImGui::MenuItem("Sphere From Surface"))
			{
				emitterValues.burstShapeEmitter = ShapeEmitter::SphereShapeBorder;
				emitterChange = true;
			}
			else if (ImGui::MenuItem("Cone"))
			{
				emitterValues.burstShapeEmitter = ShapeEmitter::ConeShape;
				emitterChange = true;
			}
			ImGui::End();
		}
		if(ImGui::DragInt("Min particles", &emitterValues.minBurst, 1.0f, 0, 100))
			emitterChange = true;
		if (emitterValues.minBurst > emitterValues.maxBurst)
		{
			emitterValues.maxBurst = emitterValues.minBurst;
			emitterChange = true;
		}
		if(ImGui::DragInt("Max Particles", &emitterValues.maxBurst, 1.0f, 0, 100))
			emitterChange = true;
		if (emitterValues.maxBurst < emitterValues.minBurst)
			emitterValues.minBurst = emitterValues.maxBurst;

		if(ImGui::DragFloat("Seconds between Burst", &emitterValues.burstSeconds, 0.5f, 0.0f, 0.0f, "%.1f"))
			emitterChange = true;

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

		for (std::map<float, glm::vec4>::iterator iter = colorMap.begin(); iter != colorMap.end(); ++iter)
		{
			if (!EditColor(&(*iter).second.x, (*iter).first))
				emitter->EraseColor((*iter).first);
		}

		ImGui::Separator();
		if (ImGui::Checkbox("Color time", &particleValues.activeMulticolor))
			particleChange = true;
		if (particleValues.activeMulticolor)
		{

			ImGui::DragInt("Position", &nextPos, 1.0f, 1, 100);
			ImGui::ColorPicker4("", &nextColor.x, ImGuiColorEditFlags_AlphaBar);
			if (ImGui::Button("Add Color", ImVec2(125, 25)))
			{
				emitter->AddColor(&nextColor.x,(nextPos/100.0f));
				colorMap.insert(std::pair<float, glm::vec4>((nextPos / 100.0f), nextColor));
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
		colorStr.append(std::to_string(int(position * 100))+"%");
		if (ImGui::Button(colorStr.data(), ImVec2(150, 25)))
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
		if (ImGui::Checkbox("Using texture", &particleValues.useTexture))
			particleChange = true;

		if (particleValues.textureID > 0u)
		{
			ImGui::Text("Texture num '%i'", particleValues.textureID);

			ImGui::Image((void*)particleValues.textureID, ImVec2(256.0f, 256.0f), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

			if (ImGui::BeginMenu("Change Texture"))
			{
				float sz = ImGui::GetTextLineHeight();
				for (std::list<TextureImporter*>::iterator iter = controller->Mng->render->textures.begin(); iter != controller->Mng->render->textures.end(); ++iter)
				{
					ImVec2 p = ImGui::GetCursorScreenPos();
					ImGui::Dummy(ImVec2(sz, sz));
					ImGui::SameLine();
					if (ImGui::MenuItem((*iter)->texture.name.c_str()))
					{
						particleValues.textureID = (*iter)->texture.id;
						particleChange = true;
					}
				}
				if (ImGui::MenuItem("Add New Texture"))
					popParticle = true;

				ImGui::EndMenu();
			}

			if (ImGui::Checkbox("Animated sprite", &particleValues.isParticleAnimated))
			{
				if (!particleValues.isParticleAnimated)
				{
					particleValues.textureRows = 1;
					particleValues.textureColumns = 1;
					particleValues.dieOnFinishAnim = false;
				}
				particleChange = true;
			}
			if (particleValues.isParticleAnimated)
			{
				if(ImGui::DragFloat("Animation Speed", &particleValues.animationSpeed, 0.001f, 0.0f, 5.0f))
					particleChange = true;

				if(ImGui::DragInt("Rows", &particleValues.textureRows, 1, 1, 10))
					particleChange = true;
				if(ImGui::DragInt("Columns", &particleValues.textureColumns, 1, 1, 10))
					particleChange = true;

				if(ImGui::Checkbox("Kill particle with animation", &particleValues.dieOnFinishAnim))
					particleChange = true;
				if (particleValues.dieOnFinishAnim)
				{
					checkLife = false;
					particleValues.life.x = particleValues.animationSpeed * (particleValues.textureColumns * particleValues.textureRows - 1);
				}
			}
			
			if (ImGui::Button("Remove Texture", ImVec2(125, 25)))
			{
				particleValues.textureID = 0u;
				particleChange = true;
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
					{
						particleValues.textureID = (*iter)->texture.id;
						particleChange = true;
					}
				}
				if (ImGui::MenuItem("Import New Texture"))
					popParticle = true;

				ImGui::EndMenu();
			}
		}

		if (popParticle)
		{
			ImGui::OpenPopup("Import new texture path");


			if (ImGui::BeginPopupModal("Import new texture path", (bool*)0, ImGuiWindowFlags_NoResize))
			{

				ImGui::Text("Import new texture. Write the correct path");
				ImGui::InputText("##MaxPart", path, 64);
				if (ImGui::Button("Import", ImVec2(100.0f, 25.0f)))
				{
					TextureImporter* newTexture = new TextureImporter(path);
					controller->Mng->render->textures.push_back(newTexture);

					popParticle = false;
				}
				ImGui::EndPopup();
			}
		}
		ImGui::Separator();
	}
}

#pragma endregion