#include "Vector3.h"
#include "Matrix4.h"

const Vector3 Vector3::ZERO	  = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::UNIT_X = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UNIT_Y = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UNIT_Z = Vector3(0.0f, 0.0f, 1.0f);


float vec3Dot(const Vector3 &v1, const Vector3 &v2)
{
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}


Vector3 vec3Cross(const Vector3 &v1, const Vector3 &v2)
{
	Vector3 vCross;

	vCross.x = v1.y * v2.z - v1.z * v2.y;
	vCross.y = v1.z * v2.x - v1.x * v2.z;
	vCross.z = v1.x * v2.y - v1.y * v2.x;

	return vCross;
}


Vector3	vec3Transform(const Vector3 &v, const Matrix4 &m)
{
	Vector3 r;

	r.x = m(0,0)*v.x + m(0,1)*v.y + m(0,2)*v.z + m(0,3);
	r.y = m(1,0)*v.x + m(1,1)*v.y + m(1,2)*v.z + m(1,3);
	r.z = m(2,0)*v.x + m(2,1)*v.y + m(2,2)*v.z + m(2,3);

	return r;
}
