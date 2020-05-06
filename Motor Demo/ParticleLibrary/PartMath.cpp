#include "PartMath.h"
#include <math.h>

//_________________________________________________________________________________________________________________________
//-------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------VECTOR2------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------

PartVec2::PartVec2(float x, float y) : x(x), y(y) {}

//_________________________________________________________________________________________________________________________
//-------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------VECTOR3------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------

PartVec3::PartVec3(float x, float y, float z) : x(x), y(y), z(z) {}
PartVec3::PartVec3(float x) : x(x), y(x), z(x) {}

PartVec3 PartVec3::Normalize()
{
	PartVec3 copy = PartVec3(x, y, z);

	float divider = sqrt(x * x + y * y + z * z);
	copy /= divider;

	return copy;
}

PartVec3 PartVec3::CrossProduct(const PartVec3& vec)
{
	return PartVec3(y * vec.z - z * vec.y,
		z * vec.x - x * vec.z,
		x * vec.y - y * vec.x);
}

PartVec3& PartVec3::operator =(const PartVec3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}
PartVec3& PartVec3::operator-()
{
	x = -x;
	y = -y;
	z = -z;
	return *this;
}
PartVec3& PartVec3::operator +=(const PartVec3& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}
PartVec3& PartVec3::operator/=(const PartVec3& vec)
{
	x /= vec.x;
	y /= vec.y;
	z /= vec.z;
	return *this;
}
PartVec3& PartVec3::operator /=(const float num)
{
	x /= num;
	y /= num;
	z /= num;
	return *this;
}

PartVec3& PartVec3::operator*=(const float num)
{
	x *= num;
	y *= num;
	z *= num;
	return *this;
}

PartVec3 PartVec3::operator*(const float num)
{
	PartVec3 res;

	res.x = x * num;
	res.y = y * num;
	res.z = z * num;
	return res;
}

//_________________________________________________________________________________________________________________________
//-------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------VECTOR4------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------

PartVec4::PartVec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

PartVec4::PartVec4(PartVec2 xy, float z, float w) : x(xy.x), y(xy.y), z(z), w(w) {}

PartVec4::PartVec4(float x) : x(x), y(x), z(x), w(x) {}

PartVec4 PartVec4::PartLerp(PartVec4 final, float t)
{
	PartVec4 ret = PartVec4(0.0f);

	ret.x = (final.x - x) * t + x;
	ret.y = (final.y - y) * t + y;
	ret.z = (final.z - z) * t + z;
	ret.w = (final.w - w) * t + w;

	return ret;
}

//_________________________________________________________________________________________________________________________
//-------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------MATRIX4------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------

PartMat4::PartMat4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
{

	mat[0][0] = m00;	mat[0][1] = m01;	mat[0][2] = m02;	mat[0][3] = m03;
	mat[1][0] = m10;	mat[1][1] = m11;	mat[1][2] = m12;	mat[1][3] = m13;
	mat[2][0] = m20;	mat[2][1] = m21;	mat[2][2] = m22;	mat[2][3] = m23;
	mat[3][0] = m30;	mat[3][1] = m31;	mat[3][2] = m32;	mat[3][3] = m33;

}

void PartMat4::Multiply(const PartMat4& m)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mat[i][j] = mat[i][0] * m.mat[0][j] + mat[i][1] * m.mat[1][j] + mat[i][2] * m.mat[2][j] + mat[i][3] * m.mat[3][j];
		}
	}
}

PartVec4 PartMat4::Multiply(const PartVec4& vec)
{
	return PartVec4(mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z + mat[0][3] * vec.w,
					mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z + mat[1][3] * vec.w,
					mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z + mat[2][3] * vec.w,
					mat[3][0] * vec.x + mat[3][1] * vec.y + mat[3][2] * vec.z + mat[3][3] * vec.w);
}


PartMat4& PartMat4::operator =(const PartMat4& m)
{
	mat[0][0] = m.mat[0][0];
	mat[0][1] = m.mat[0][1];
	mat[0][2] = m.mat[0][2];
	mat[0][3] = m.mat[0][3];

	mat[1][0] = m.mat[1][0];
	mat[1][1] = m.mat[1][1];
	mat[1][2] = m.mat[1][2];

	mat[2][0] = m.mat[2][0];
	mat[2][1] = m.mat[2][1];
	mat[2][2] = m.mat[2][2];
	mat[2][3] = m.mat[2][3];

	mat[3][0] = m.mat[3][0];
	mat[3][1] = m.mat[3][1];
	mat[3][2] = m.mat[3][2];
	mat[3][3] = m.mat[3][3];
	return *this;
}