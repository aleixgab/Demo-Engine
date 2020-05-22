#include <PlaneImporter.h>
#include <glad/glad.h>
#include <cstddef>
#include <PartMath.h>
#include <Emitter.h>
PlaneImporter::PlaneImporter()
{
	float particle_quad[] = {

		// positions	       /// texture coords		
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, // bottom right
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, // top left	
							   
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, // top left	
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_static);  //Static VBO
	glGenBuffers(1, &VBO_dynamic[0]);  //Dynamic Life VBO
	glGenBuffers(1, &VBO_dynamic[1]);  //Dynamic VBO

	glBindVertexArray(VAO);

	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO_static);	
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
}

void PlaneImporter::SetDynamicValues(unsigned int maxParticles)
{
	//Particle Life
	glBindBuffer(GL_ARRAY_BUFFER, VBO_dynamic[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PartVec2) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	//Particle Dynamic Values
	glBindBuffer(GL_ARRAY_BUFFER, VBO_dynamic[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * maxParticles, NULL, GL_DYNAMIC_DRAW);
}


PlaneImporter::~PlaneImporter()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO_static);
	glDeleteBuffers(1, &VBO_dynamic[0]);
	glDeleteBuffers(1, &VBO_dynamic[1]);

}

