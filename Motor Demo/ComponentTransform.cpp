#include "ComponentTransform.h"
#include "glm/gtx/matrix_decompose.hpp"
#include <glm/gtx/euler_angles.hpp>
#include "imgui/imgui.h"

Transform::Transform(GameObject* gameObject, glm::vec3 position, glm::quat rotation, glm::vec3 scale) : Component(gameObject, ComponentType_TRANSFORM)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}

Transform::Transform(GameObject* gameObject) : Component(gameObject, ComponentType_TRANSFORM)
{
}

void Transform::SetTransform(glm::mat4 trans)
{
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(trans, scale, rotation, position, skew, perspective);
}

void Transform::SetIdentity()
{
	SetPos(glm::vec3(0.0f));
	SetRotation(glm::quat(0.0f, 0.0f, 0.0f, 1.0f));
	SetScale(glm::vec3(1.0f));
}

void Transform::SetPos(glm::vec3 position)
{
	this->position = position;
}

void Transform::SetRotation(glm::quat rotation)
{
	this->rotation = rotation;
}

void Transform::SetScale(glm::vec3 scale)
{
	this->scale = scale;
}

glm::vec3 Transform::GetPos() const
{
	return position;
}

glm::quat Transform::GetRotation() const
{
	return rotation;
}

glm::vec3 Transform::GetScale() const
{
	return scale;
}

glm::mat4 Transform::GetTransform() const
{
	glm::mat4 mat = glm::mat4(1.0);
	mat = translate(mat, position);
	mat = glm::rotate(mat, glm::angle(rotation), glm::axis(rotation));
	mat = glm::scale(mat, scale);
	return mat;
}


void Transform::Inspector()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{

		glm::vec3 angles = glm::eulerAngles(rotation);

		angles[0] = glm::degrees(angles.x);
		angles[1] = glm::degrees(angles.y);
		angles[2] = glm::degrees(angles.z);

		if (ImGui::DragFloat3("Position", &position[0], 0.1f, 0.0f, 0.0f, "%.2f")) 
			SetPos(position);

		if (ImGui::DragFloat3("Scale", &scale[0], 0.01f, 0.0f, 0.0f, "%.2f"))
			SetScale(scale);

		if (ImGui::DragFloat3("Rotation", &angles[0], 1, 0.0f, 0.0f, "%.2f"))
		{

			SetRotation(glm::eulerAngleXYZ(angles.x, angles.y, angles.z));

			glm::quat QuatAroundX = glm::angleAxis(angles.x, glm::vec3(1.0, 0.0, 0.0));
			glm::quat QuatAroundY = glm::angleAxis(angles.y, glm::vec3(0.0, 1.0, 0.0));
			glm::quat QuatAroundZ = glm::angleAxis(angles.z, glm::vec3(0.0, 0.0, 1.0));
			SetRotation(glm::normalize(QuatAroundX * QuatAroundY * QuatAroundZ));
		}

		if (ImGui::Button("Reset", ImVec2(100, 20)))
			SetIdentity();

	}
}

