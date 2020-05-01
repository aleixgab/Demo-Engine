#include "PlaneImporter.h"
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

PlaneImporter::PlaneImporter()
{
	float particle_quad[] = {

		// positions	        // texture coords			
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f, // top right	
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, // bottom right	
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, // top left		

		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, // bottom right	
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, // bottom left	
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, // top left		
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	// Set mesh attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// textCoords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

PlaneImporter::PlaneImporter(unsigned int maxParticles)
{
	float particle_quad[] = {

		// positions	       /// texture coords				-- Save mat4Transform + vec4Color + vec4 TextureCoordsModified
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f, // top right		-- Save mat4Transform + vec4Color + vec4 TextureCoordsModified
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, // bottom right		-- Save mat4Transform + vec4Color + vec4 TextureCoordsModified
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, // top left			-- Save mat4Transform + vec4Color + vec4 TextureCoordsModified
							   
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, // bottom right		-- Save mat4Transform + vec4Color + vec4 TextureCoordsModified
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, // bottom left		-- Save mat4Transform + vec4Color + vec4 TextureCoordsModified
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, // top left			-- Save mat4Transform + vec4Color + vec4 TextureCoordsModified
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO_Texture);
	glGenBuffers(1, &VBO_Color);
	glGenBuffers(1, &VBO_Position);
	glGenBuffers(1, &VBO_Rotation);
	glGenBuffers(1, &VBO_Size);

	glBindVertexArray(VAO);

	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Texture);	
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Color);	
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Rotation);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Size);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * maxParticles, NULL, GL_DYNAMIC_DRAW);
}

PlaneImporter::~PlaneImporter()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO_Color);
	glDeleteBuffers(1, &VBO_Texture);
	glDeleteBuffers(1, &VBO_Position);
	glDeleteBuffers(1, &VBO_Rotation);
	glDeleteBuffers(1, &VBO_Size);
}

