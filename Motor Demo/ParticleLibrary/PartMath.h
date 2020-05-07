#ifndef __Math_H__
#define __Math_H__

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

public:
	//X component.
	float x = 0.0f;
	//Y component.
	float y = 0.0f;
	//Z component.
	float z = 0.0f;
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

class PartMat4
{
public:
	PartMat4() {}
	PartMat4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33);

	void Multiply(const PartMat4& m);
	PartVec3 Multiply(const PartVec4& vec);

	PartMat4& operator=(const PartMat4& m);
public:

	//Save the data in a matrix 4x4
	float mat[4][4];
};
#endif