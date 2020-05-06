#include "PlaneImporter.h"
#include "PartMath.h"
#include "Emitter.h"
#include <glad/glad.h>
#include <cstddef>

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
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * maxParticles, NULL, GL_DYNAMIC_DRAW);
}

PlaneImporter::~PlaneImporter()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

