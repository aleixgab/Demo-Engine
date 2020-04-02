#ifndef __Transform_H__
#define __Transform_H__

#include "Component.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

class GameObject;

class Transform : public Component
{
public:

	Transform(GameObject* gameObject, glm::vec3 position, glm::quat rotation, glm::vec3 scale);
	Transform(GameObject* gameObject);

	~Transform() {}

	void SetTransform(glm::mat4 trans);
	void SetIdentity();
	void SetPos(glm::vec3 position);
	void SetRotation(glm::quat rotation);
	void SetScale(glm::vec3 scale);

	glm::vec3 GetPos() const;
	glm::quat GetRotation() const;
	glm::vec3 GetScale() const;
	glm::mat4 GetTransform() const;

	void Inspector();

public:
	bool dragTransform = true;

	glm::vec3 position = glm::vec3(0.0f);
	glm::quat rotation = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec3 scale = glm::vec3(1.0f);
};
#endif

