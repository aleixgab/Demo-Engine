#include "GameObject.h"
#include <glad/glad.h>

GameObject::GameObject(glm::vec3 position, glm::quat rotation, glm::vec3 scale, const char* name)
{
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	transform = new Transform(this, position, rotation, scale);

	if (name != nullptr)
		this->name = name;
	else this->name = "Game Object";

	components.push_back(transform);
}

GameObject::~GameObject()
{
	transform = nullptr;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

Transform* GameObject::GetComponentTransform() const
{
	return transform;
}

void GameObject::RemoveComponents()
{
	for (std::vector<Component*>::iterator iterator = components.begin(); iterator != components.end(); ++iterator)
	{
		delete (*iterator);
	}
	components.clear();
}
