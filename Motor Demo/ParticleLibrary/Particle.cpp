#include "Particle.h"
#include "Emitter.h"
#include "ParticleManager.h"
#include "PlaneImporter.h"

#include <glad/glad.h>
#include <random>

#include <Brofiler/Brofiler.h>

#include <iostream>

void Particle::SetParticleValues(PartVec3 pos, ParticleStartValues values, ParticleAnimation animation, Emitter* owner)
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

 	transform.angle = CreateRandomNum(values.rotation) * (PI / 180.0f);
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

void Particle::Update(float dt)
{
	if (!owner->runningTime)
		dt = 0.0f;

	if (currLife > 0.0f)
	{
		
		//Tranlate
		speed += acceleration * dt;
		transform.position += (direction * speed * dt);

		//Scale
		transform.scale += sizeOverTime * dt;

		//Rotation
		angularVelocity += angularAcceleration *  dt;
		transform.angle += angularVelocity * dt ;

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
				finalColor = color[index].color.PartLerp(color[index + 1].color, timeNormalized);
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
}

void Particle::SetVertexs(Vertex* particleVertexs, PartVec3 cameraForward, PartVec3 cameraUp)
{
	//Billboard
	PartVec3 xAxis = (cameraUp.CrossProduct(cameraForward)).Normalize();

	PartMat4 aModel = PartMat4(xAxis.x, cameraUp.x, cameraForward.x, transform.position.x,
								 xAxis.y, cameraUp.y, cameraForward.y, transform.position.y,
								 xAxis.z, cameraUp.z, cameraForward.z, transform.position.z,
								 0.0f,0.0f,0.0f, 1.0f);
	//Plane Rotation
	aModel.Multiply(PartMat4(cos(transform.angle), -sin(transform.angle), 0.0, 0.0,
							 sin(transform.angle), cos(transform.angle), 0.0, 0.0,
							 0.0, 0.0, 1.0, 0.0,
							 0.0, 0.0, 0.0, 1.0));

	float maxTextCoordX = currMinUVCoord.x + textureColumnsNorm;
	float maxTextCoordY = currMinUVCoord.y + textureRowsNorm;

	float size = 0.5f * transform.scale;
	// top right	
	particleVertexs[0].position = aModel.Multiply(PartVec4(size, size, 0.0f, 1.0f));
	particleVertexs[0].texCoords = PartVec2(maxTextCoordX, maxTextCoordY);
	particleVertexs[0].color = finalColor;
	
	// bottom right	
	particleVertexs[1].position = aModel.Multiply(PartVec4(size, -size, 0.0f, 1.0f));
	particleVertexs[1].texCoords = PartVec2(maxTextCoordX, currMinUVCoord.y);
	particleVertexs[1].color = finalColor;

	// top left		
	particleVertexs[2].position = aModel.Multiply(PartVec4(-size, size, 0.0f, 1.0f));
	particleVertexs[2].texCoords = PartVec2(currMinUVCoord.x, maxTextCoordY);
	particleVertexs[2].color = finalColor;

	// bottom right	
	particleVertexs[3].position = particleVertexs[1].position;
	particleVertexs[3].texCoords = particleVertexs[1].texCoords;
	particleVertexs[3].color = particleVertexs[1].color;

	// bottom left	
	particleVertexs[4].position = aModel.Multiply(PartVec4(-size, -size, 0.0f, 1.0f));
	particleVertexs[4].texCoords = PartVec2(currMinUVCoord.x, currMinUVCoord.y);
	particleVertexs[4].color = finalColor;

	// top left		
	particleVertexs[5].position = particleVertexs[2].position;
	particleVertexs[5].texCoords = particleVertexs[2].texCoords;
	particleVertexs[5].color = particleVertexs[2].color;
}

float Particle::CreateRandomNum(PartVec2 edges)//.x = minPoint & .y = maxPoint
{
	float num = edges.x;
	if (edges.x < edges.y)
	{
		num = owner->parent->GetRandomNum(edges.x, edges.y);
	}
	return num;
}