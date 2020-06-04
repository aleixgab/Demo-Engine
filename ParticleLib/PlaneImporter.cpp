#include <PlaneImporter.h>
#include <glad/glad.h>
#include <cstddef>
#include <PartMath.h>
#include <Emitter.h>
PlaneImporter::PlaneImporter()
{

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_dynamic[0]);  //Dynamic Life VBO
	glGenBuffers(1, &VBO_dynamic[1]);  //Dynamic VBO

	glBindVertexArray(VAO);
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
	glDeleteBuffers(1, &VBO_dynamic[0]);
	glDeleteBuffers(1, &VBO_dynamic[1]);

}

