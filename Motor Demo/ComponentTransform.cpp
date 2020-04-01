#include "ComponentTransform.h"


Transform::Transform(GameObject* gameObject, glm::vec3 position, glm::quat rotation, glm::vec3 scale) : Component(gameObject, ComponentType_TRANSFORM)
{
}

Transform::Transform(GameObject* gameObject) : Component(gameObject, ComponentType_TRANSFORM)
{
}

void Transform::SetTransform(glm::mat4 trans)
{
}

void Transform::SetIdentity()
{
}

glm::mat4 Transform::GetMatrixOGL() const
{
	return glm::mat4();
}

glm::mat4 Transform::GetMatrix() const
{
	return glm::mat4();
}

void Transform::Inspector()
{
}
