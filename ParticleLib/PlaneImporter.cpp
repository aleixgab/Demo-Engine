#include "PlaneImporter.h"
#include <glad/glad.h>
#include "PartMath.h"
#include <cstddef>

PlaneImporter::PlaneImporter(unsigned int maxParticles)
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
	glGenBuffers(1, &VBO[0]);  //Static VBO
	glGenBuffers(1, &VBO[1]);  //Particle Life
	glGenBuffers(1, &VBO[2]);  //Position
	glGenBuffers(1, &VBO[3]);  //Direction
	glGenBuffers(1, &VBO[4]);  //Speed
	glGenBuffers(1, &VBO[5]);  //Acceleration
	glGenBuffers(1, &VBO[6]);  //Gravity - Acceleration 3D
	glGenBuffers(1, &VBO[7]);  //Rotation
	glGenBuffers(1, &VBO[8]);  //Rotation Vel
	glGenBuffers(1, &VBO[9]);  //Rotation Accel 
	glGenBuffers(1, &VBO[10]); //Size
	glGenBuffers(1, &VBO[11]); //Size over time

	glBindVertexArray(VAO);

	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);	
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
}

void PlaneImporter::SetDynamicValues(unsigned int maxParticles)
{
	//Particle Life
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PartVec2) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	//Position
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PartVec3) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	//Direction
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PartVec3) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	//Speed
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	//Acceleration
	glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	//Gravity - Acceleration 3D
	glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PartVec3) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	//Rotation
	glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	//Rotation Vel
	glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	//Rotation Accel 
	glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	//Size
	glBindBuffer(GL_ARRAY_BUFFER, VBO[10]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * maxParticles, NULL, GL_DYNAMIC_DRAW);

	//Size over time
	glBindBuffer(GL_ARRAY_BUFFER, VBO[11]);
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
	glDeleteBuffers(1, &VBO[6]);
	glDeleteBuffers(1, &VBO[7]);
	glDeleteBuffers(1, &VBO[8]);
	glDeleteBuffers(1, &VBO[9]); 
	glDeleteBuffers(1, &VBO[10]);
	glDeleteBuffers(1, &VBO[11]);

}

