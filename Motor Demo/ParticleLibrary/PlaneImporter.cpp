#include "PlaneImporter.h"
#include <glad/glad.h>

PlaneImporter::PlaneImporter()
{
	//float vertices[] = {
	//-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	// 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	// 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	// 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	//-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	//-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	//};
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindVertexArray(VAO);

	//// position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//// normal attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//

	GLfloat particle_quad[] = {
		//POSITION				TEXT COORD
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
					  
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f
	};
	glGenVertexArrays(1, &partVAO);
	glGenBuffers(1, &partVBO);

	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, partVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	glBindVertexArray(partVAO);

	// Set mesh attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// textCoords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

PlaneImporter::~PlaneImporter()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

