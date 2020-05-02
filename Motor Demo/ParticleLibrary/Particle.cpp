#include "Particle.h"
#include "ParticleManager.h"
#include "PlaneImporter.h"

#include <glad/glad.h>
#include <glm/gtx/compatibility.hpp>
#include <random>

#include <Brofiler/Brofiler.h>

void Particle::SetParticleValues(glm::vec3 pos, ParticleStartValues values, ParticleAnimation animation, Emitter* owner)
{
	this->owner = owner;

	//Save all the initial values 
	initialLife = currLife = CreateRandomNum(values.life);
	speed = CreateRandomNum(values.speed);
	gravity = values.gravity;
	acceleration = CreateRandomNum(values.acceleration);
	direction = values.particleDirection;
	angularVelocity = CreateRandomNum(values.angularVelocity) * (PI / 180.0f);
	angularAcceleration = CreateRandomNum(values.angularAcceleration) * (PI / 180.0f);
	sizeOverTime = CreateRandomNum(values.sizeOverTime);
	for (std::list<ParticleColor>::iterator iter = values.colorList.begin(); iter != values.colorList.end(); ++iter)
		color.push_back(*iter);

	isMulticolor = values.isMulticolor;
	index = 0u;

	transform.angle = glm::radians(CreateRandomNum(values.rotation));
	transform.position = pos;
	transform.scale = CreateRandomNum(values.size);
	countAnimTime = 0.0f;

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
		speed += acceleration * dt;
		transform.position += direction * (speed * dt);

		//Scale
		transform.scale += sizeOverTime * dt;

		//Rotation
		angularVelocity += angularAcceleration * dt;
		transform.angle += angularVelocity * dt;

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
		if (owner->isParticleAnimated && (textureRows > 1 || textureColumns > 1))
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

void Particle::SaveCameraDistance(glm::vec3 cameraPosition)
{
	float x = cameraPosition.x - transform.position.x;
	float y = cameraPosition.y - transform.position.y;
	float z = cameraPosition.z - transform.position.z;

	cameraDist = x * x + y * y + z * z;
}

glm::vec4 Particle::GetTextureCoords() const
{
	return glm::vec4(currMinUVCoord, textureColumnsNorm, textureRowsNorm);
}

glm::vec4 Particle::GetColor() const
{
	return finalColor;
}

void Particle::GetTransform(glm::vec3& pos, float& angle, float& scale ) const
{
	pos = transform.position;
	angle = transform.angle;
	scale = transform.scale;
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
