#include "Particle.h"
#include "ParticleManager.h"
#include "PlaneImporter.h"

#include <glad/glad.h>

//#pragma comment (lib, "glu32.lib")    
//#pragma comment (lib, "opengl32.lib") 

void Particle::CreateParticle(glm::vec3 pos, ParticleStartValues values, Emitter* owner)
{
	//Save all the initial values 
	lifeTime = CreateRandomNum(values.life);
	speed = CreateRandomNum(values.speed);
	gravity = values.gravity;
	acceleration = CreateRandomNum(values.acceleration);
	direction = values.particleDirection;
	angle = CreateRandomNum(values.rotation) * (PI / 180.0f);
	angularVelocity = CreateRandomNum(values.angularVelocity) * (PI / 180.0f);
	angularAcceleration = CreateRandomNum(values.angularAcceleration) * (PI / 180.0f);
	sizeOverTime = CreateRandomNum(values.sizeOverTime);
	color = glm::vec4(1.0f);

	isActive = true;

	this->owner = owner;
}

bool Particle::Update(float dt)
{
	bool ret = true;
	
	if (!owner->runningTime)
		dt = 0.0f;

	if (lifeTime > 0.0f)
	{
		//Tranlate
		speed + acceleration * dt;
		transform.position += direction * (speed * dt);

		//Scale
		transform.scale.x += sizeOverTime * dt;
		transform.scale.y += sizeOverTime * dt;
		transform.scale.z += sizeOverTime * dt;

		//Rotation
		angularVelocity += angularAcceleration * dt;
		angle += angularVelocity * dt;
		transform.rotation *= glm::quat(0.0f,0.0f,1.0f,angle);

		//gpuParticles[index].v1 = v1;
		//gpuParticles[index].v2 = v1;
		//gpuParticles[index].v3 = v1;
		//gpuParticles[index].v4 = v1;
		//gpuParticles[index].v5 = v1;
		//gpuParticles[index].v6 = v1;

		//glBufferSubData(GL_BUFFER.., 0, sizeOfGPUParticlesInBytes, &gpuParticles[0]);


		//glDrawArrays(GL_TRIANGLES, 0, gpuParticles.size() * 6);

		ret = LookAtCamera();
		lifeTime -= dt;
	}
	else
	{
		//Deactivate the particle
		isActive = false;
		owner->particles.remove(this);
		owner->parent->numActivePart--;
	}
	
	return ret;
}

void Particle::Draw(uint uuid, glm::mat4 viewMatrix, glm::mat4 projMatrix)
{
	if (isActive)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glUseProgram(uuid);
		glUniformMatrix4fv(glGetUniformLocation(uuid, "projection"), 1, GL_FALSE, &projMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(uuid, "view"), 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(uuid, "model"), 1, GL_FALSE, &transform.GetMatrix()[0][0]);
		glUniform4fv(glGetUniformLocation(uuid, "color"), 1, &color.r);

		//this->texture.Bind();
		glBindVertexArray(owner->parent->plane->partVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Particle::SaveCameraDistance(glm::vec3 cameraPosition)
{
	float x = cameraPosition.x - transform.position.x;
	float y = cameraPosition.y - transform.position.y;
	float z = cameraPosition.z - transform.position.z;

	cameraDist = x * x + y * y + z * z;
}

float Particle::CreateRandomNum(glm::vec2 edges)//.x = minPoint & .y = maxPoint
{
	float num = edges.x;
	if (edges.x < edges.y)
		num = edges.x + (((float)rand()) / (float)RAND_MAX) * (edges.y - edges.x);

	return num;
}

//Shader
bool Particle::LookAtCamera()
{
	bool ret = false;
	if (owner->parent->cameraForward && owner->parent->cameraUp)
	{
		glm::vec3 zAxis = -(*owner->parent->cameraForward);
		glm::vec3 yAxis = *owner->parent->cameraUp;
		glm::vec3 xAxis = glm::normalize(glm::cross(yAxis, zAxis));


		glm::toQuat(glm::mat3(xAxis, yAxis, zAxis));
		ret = true;
	}
	return ret;
}

glm::mat4 PartTransform::GetMatrix() const
{
	glm::mat4 mat = glm::mat4(1.0f);
	mat = translate(mat, position);
	mat = glm::rotate(mat, glm::angle(rotation), glm::axis(rotation));
	mat = glm::scale(mat, scale);
	return mat;
}
