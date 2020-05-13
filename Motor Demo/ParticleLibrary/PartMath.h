#ifndef __Particle_Math_H__
#define __Particle_Math_H__

class PartVec2
{
public:
	PartVec2() {}

	PartVec2(float x, float y);

public:
	//X component.
	float x = 0.0f;
	//Y component.
	float y = 0.0f;
};

class PartVec3
{
public:
	PartVec3() {}

	PartVec3(float x, float y, float z);
	PartVec3(float x);

	PartVec3 Normalize();
	PartVec3 CrossProduct(const PartVec3& vec);

	PartVec3& operator =(const PartVec3& vec);
	PartVec3& operator -();
	PartVec3& operator +=(const PartVec3& vec);
	PartVec3& operator /=(const PartVec3& vec);
	PartVec3& operator /=(const float num);
	PartVec3& operator *=(const float num);
	PartVec3 operator*(const float num);
	PartVec3 operator*(const PartVec3& vec);
#pragma region XYZ Components
public:
	//X component.
	float x = 0.0f;
	//Y component.
	float y = 0.0f;
	//Z component.
	float z = 0.0f;
#pragma endregion
};

class PartVec4
{
public:
	PartVec4() {}
	PartVec4(float x, float y, float z, float w);
	PartVec4(PartVec2 xy, float z, float w);
	PartVec4(float x);

	//Lerp between this and a final value
	PartVec4 PartLerp(PartVec4 final, float t);
public:
	//X component.
	float x = 0.0f;
	//Y component.
	float y = 0.0f;
	//Z component.
	float z = 0.0f;
	//W component.
	float w = 0.0f;
};
#endif
