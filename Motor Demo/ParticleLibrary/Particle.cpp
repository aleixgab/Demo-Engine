#include "Particle.h"
#include "ParticleManager.h"
#include "PlaneImporter.h"

#include <glad/glad.h>
#include <glm/gtx/compatibility.hpp>
#include <random>

void Particle::CreateParticle(glm::vec3 pos, ParticleStartValues values, ParticleAnimation animation, Emitter* owner)
{
	this->owner = owner;

	//Save all the initial values 
	initialLife = currLife = CreateRandomNum(values.life);
	speed = CreateRandomNum(values.speed);
	gravity = values.gravity;
	acceleration = CreateRandomNum(values.acceleration);
	direction = values.particleDirection;
	angle =/* glm::radians(*/CreateRandomNum(values.rotation)/*)*/;
	angularVelocity = CreateRandomNum(values.angularVelocity) * (PI / 180.0f);
	angularAcceleration = CreateRandomNum(values.angularAcceleration) * (PI / 180.0f);
	sizeOverTime = CreateRandomNum(values.sizeOverTime);
	for (std::list<ParticleColor>::iterator iter = values.colorList.begin(); iter != values.colorList.end(); ++iter)
		color.push_back(*iter);

	isMulticolor = values.isMulticolor;
	colorPercentage = values.colorPercent;
	index = 0u;

	transform.position = pos;

	countAnimTime = 0.0f;

	isParticleAnimated = animation.isParticleAnimated;
	textureRows = animation.textureRows;
	textureColumns = animation.textureColumns;
	textureRowsNorm = animation.textureRowsNorm;
	textureColumnsNorm = animation.textureColumnsNorm;
	animTime = CreateRandomNum(animation.animationSpeed);

	if (animation.isAnimRand)
		currentFrame = owner->parent->GetRandomNum(0, textureColumns * textureRows);
	else
		currentFrame = 0u;

	contFrame = 0u;
	currMinUVCoord.x = (currentFrame % textureColumns) * textureColumnsNorm;
	currMinUVCoord.y = ((textureRows - 1) - (currentFrame / textureColumns)) * textureRowsNorm;

	isActive = true;
}

bool Particle::Update(float dt)
{
	bool ret = true;
	
	if (!owner->runningTime)
		dt = 0.0f;

	if (currLife > 0.0f)
	{
		//Tranlate
		speed + acceleration * dt;
		transform.position += direction * (speed * dt);

		//Scale
		transform.scale.x += sizeOverTime * dt;
		transform.scale.y += sizeOverTime * dt;
		transform.scale.z += sizeOverTime * dt;

		//Rotation
		ret = LookAtCamera();
		angularVelocity += angularAcceleration * dt;
		angle += angularVelocity * dt;

		transform.rotation *= glm::angleAxis(angle, glm::vec3(0.0f, 0.0f, 1.0f));
		//transform.rotation = glm::quat(0.0f,0.0f,1.0f,angle);

		//COLOR
		if (color.size() == 1 || !isMulticolor)
			finalColor = color.front().color;

		else if (index + 1 < color.size())
		{
			float lifeNormalized = 1.0f - currLife / initialLife;
			if (color[index + 1].position > lifeNormalized)
			{
				float timeNormalized = (lifeNormalized - color[index].position) / (color[index + 1].position - color[index].position);
				if (color[index + 1].position == 0)
					timeNormalized = 0;
				//LOG("%i", index);
				finalColor = glm::lerp(color[index].color, color[index + 1].color, timeNormalized);
				//LERP Color
			}
			else
				index++;
		}
		else
			finalColor = color[index].color;

		//ANIMATION
		if (isParticleAnimated && (textureRows > 1 || textureColumns > 1))
		{
			countAnimTime += dt;
			if (countAnimTime > animTime)
			{
				if ((textureColumns * textureRows) > contFrame + 1)
				{
					if ((textureColumns * textureRows) > currentFrame + 1)
					{
						currentFrame++;
						contFrame++;
					}
					else
						currentFrame = 0;

					currMinUVCoord.x = (currentFrame % textureColumns) * textureColumnsNorm;
					currMinUVCoord.y = ((textureRows - 1) - (currentFrame / textureColumns)) * textureRowsNorm;
					countAnimTime = 0.0f;
				}
				else if (!owner->dieOnFinishAnim)
					contFrame = 0u;

				else
					currLife = 0.0f;
			}
		}




		//gpuParticles[index].v1 = v1;
		//gpuParticles[index].v2 = v1;
		//gpuParticles[index].v3 = v1;
		//gpuParticles[index].v4 = v1;
		//gpuParticles[index].v5 = v1;
		//gpuParticles[index].v6 = v1;

		//glBufferSubData(GL_BUFFER.., 0, sizeOfGPUParticlesInBytes, &gpuParticles[0]);


		//glDrawArrays(GL_TRIANGLES, 0, gpuParticles.size() * 6);

		currLife -= dt;
	}
	else
	{
		//Deactivate the particle
		isActive = false;
		owner->particles.remove(this);
		owner->parent->numActivePart--;
		color.clear();
	}
	
	return ret;
}

void Particle::Draw(uint uuid, glm::mat4 viewMatrix, glm::mat4 projMatrix)
{
	if (isActive)
	{
		bool blend = glIsEnabled(GL_BLEND);
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		glUseProgram(uuid);
		glUniformMatrix4fv(glGetUniformLocation(uuid, "projection"), 1, GL_FALSE, &projMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(uuid, "view"), 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(uuid, "model"), 1, GL_FALSE, &transform.GetMatrix()[0][0]);
		glUniform4fv(glGetUniformLocation(uuid, "color"), 1, &finalColor.r);
		glUniform1f(glGetUniformLocation(uuid, "colorPercent"), colorPercentage);

		glUniform2fv(glGetUniformLocation(uuid, "currMinCoord"), 1, &currMinUVCoord.x);
		glUniform1f(glGetUniformLocation(uuid, "rowUVNorm"), textureRowsNorm);
		glUniform1f(glGetUniformLocation(uuid, "columUVNorm"), textureColumnsNorm);
		glUniform1i(glGetUniformLocation(uuid, "isAnimated"), isParticleAnimated);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, owner->textureID);
		glBindVertexArray(owner->parent->plane->VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDepthMask(GL_TRUE);
 		glEnable(blend);
	}
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
	{
		num = owner->parent->GetRandomNum(edges.x, edges.y);
	}
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


		transform.rotation = glm::toQuat(glm::mat3(xAxis, yAxis, zAxis));
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
