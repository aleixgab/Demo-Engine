#include "PlaneImporter.h"
#include <glad/glad.h>

PlaneImporter::PlaneImporter()
{
	float particle_quad[] = {

		// positions	        // texture coords			-- Save mat4Transform + vec4Color + vec4 TextureCoordsModified
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f, // top right	-- Save mat4Transform + vec4Color + vec4 TextureCoordsModified
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, // bottom right	-- Save mat4Transform + vec4Color + vec4 TextureCoordsModified
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, // top left		-- Save mat4Transform + vec4Color + vec4 TextureCoordsModified
		
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, // bottom right	-- Save mat4Transform + vec4Color + vec4 TextureCoordsModified
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, // bottom left	-- Save mat4Transform + vec4Color + vec4 TextureCoordsModified
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, // top left		-- Save mat4Transform + vec4Color + vec4 TextureCoordsModified
	};

	//unsigned int indices[] = {
	//0, 1, 3, // first triangle
	//1, 2, 3  // second triangle
	//};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Set mesh attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// textCoords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}

PlaneImporter::~PlaneImporter()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

