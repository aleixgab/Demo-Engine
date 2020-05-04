#include "PlaneImporter.h"
#include <glad/glad.h>
#include "PartMath.h"
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
	glGenBuffers(1, &VBO[0]);

	glBindVertexArray(VAO);

	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
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
	glGenBuffers(1, &VBO[0]); //Static VBO
	glGenBuffers(1, &VBO[1]); //TextureCoords
	glGenBuffers(1, &VBO[2]); //Color
	glGenBuffers(1, &VBO[3]); //Position
	glGenBuffers(1, &VBO[4]); //Rotation
	glGenBuffers(1, &VBO[5]); //Size

	glBindVertexArray(VAO);

	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);	
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PartVec4) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PartVec4) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PartVec3) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * maxParticles, NULL, GL_DYNAMIC_DRAW);
}

PlaneImporter::~PlaneImporter()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO[0]);
	glDeleteBuffers(1, &VBO[1]);
	glDeleteBuffers(1, &VBO[2]);
	glDeleteBuffers(1, &VBO[3]);
	glDeleteBuffers(1, &VBO[4]);
	glDeleteBuffers(1, &VBO[5]);
}

