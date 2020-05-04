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

