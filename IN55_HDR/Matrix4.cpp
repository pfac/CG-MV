#include "Matrix4.h"

const Matrix4 Matrix4::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f );

const Matrix4 Matrix4::ZERO(	0.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 0.0f );



Matrix4 mat4Translation(const Vector3 &v)
{
	Matrix4 m(Matrix4::IDENTITY);
	m[12] = v.x;
	m[13] = v.y;
	m[14] = v.z;
	return m;
}

Matrix4 mat4RotationX(float rad)
{
	float c = Math::cos(rad);
	float s = Math::sin(rad);

	Matrix4 m(Matrix4::IDENTITY);
	m[5]  = c;
	m[6]  = s;
	m[9]  = -s;
	m[10] = c;
	return m;
}

Matrix4 mat4RotationY(float rad)
{
	float c = Math::cos(rad);
	float s = Math::sin(rad);

	Matrix4 m(Matrix4::IDENTITY);
	m[0]  = c;
	m[2]  = -s;
	m[8]  = s;
	m[10] = c;
	return m;
}

Matrix4 mat4RotationZ(float rad)
{
	float c = Math::cos(rad);
	float s = Math::sin(rad);

	Matrix4 m(Matrix4::IDENTITY);
	m[0] = c;
	m[1] = s;
	m[4] = -s;
	m[5] = c;
	return m;
}

Matrix4 mat4RotationAxis(const Vector3 &axis, float rad)
{
	Matrix4 m(Matrix4::IDENTITY);

	float fCos = Math::cos(rad);
	float fSin = Math::sin(rad);
	float fOneMinusCos = 1.0f - fCos;
	float fX2 = axis.x*axis.x;
	float fY2 = axis.y*axis.y;
	float fZ2 = axis.z*axis.z;
	float fXYM = axis.x*axis.y*fOneMinusCos;
	float fXZM = axis.x*axis.z*fOneMinusCos;
	float fYZM = axis.y*axis.z*fOneMinusCos;
	float fXSin = axis.x*fSin;
	float fYSin = axis.y*fSin;
	float fZSin = axis.z*fSin;

	m[0]  = fX2*fOneMinusCos + fCos;
	m[4]  = fXYM-fZSin;
	m[8]  = fXZM+fYSin;

	m[1]  = fXYM+fZSin;
	m[5]  = fY2*fOneMinusCos + fCos;
	m[9]  = fYZM-fXSin;

	m[2]  = fXZM-fYSin;
	m[6]  = fYZM+fXSin;
	m[10] = fZ2*fOneMinusCos + fCos;

	return m;
}

Matrix4 mat4RotationX(float rad, const Vector3& center)
{
	return mat4Translation(center) * mat4RotationX(rad) * mat4Translation(-center);
}

Matrix4 mat4RotationY(float rad, const Vector3& center)
{
	return mat4Translation(center) * mat4RotationY(rad) * mat4Translation(-center);
}

Matrix4 mat4RotationZ(float rad, const Vector3& center)
{
    return mat4Translation(center) * mat4RotationZ(rad) * mat4Translation(-center);
}

Matrix4 mat4Frustum(float left, float right, float bottom, float top, float near, float far)
{
	if (right == left)
		return Matrix4();

	if (top == bottom)
		return Matrix4();

	if (near == far)
		return Matrix4();

	Matrix4 m = Matrix4::ZERO;
	m(0,0) = 2.0f * near / (right - left);
	m(1,1) = 2.0f * near / (top - bottom);
	m(0,2) = (right + left) / (right - left);
	m(1,2) = (top + bottom) / (top - bottom);
	m(2,2) = - (far + near) / (far - near);
	m(3,2) = -1.0f;
	m(2,3) = - (2.0f * far * near) / (far - near);

	return m;
}

Matrix4 mat4FrustumInf(float left, float right, float bottom, float top, float near)
{
	assert(right != left);
	assert(top != bottom);
	assert(near > 0.0f);

	Matrix4 m = Matrix4::ZERO;
	m(0,0) = 2.0f * near / (right - left);
	m(1,1) = 2.0f * near / (top - bottom);
	m(0,2) = (right + left) / (right - left);
	m(1,2) = (top + bottom) / (top - bottom);
	m(2,2) = -1.0f;
	m(3,2) = -1.0f;
	m(2,3) = - 2.0f * near;

	return m;
}

Matrix4 mat4LookAt(const Vector3 &eye, const Vector3 &center, const Vector3 &up)
{
	// Forward vector
	Vector3 f = eye - center;
	f.normalize();

	// Right vector
	Vector3 r = vec3Cross(up, f);
	r.normalize();

	// Up vector
	Vector3 u = vec3Cross(f, r);
	u.normalize();

	// Create matrix
	Matrix4 mat(r.x,  r.y,  r.z,  0.0f,
				u.x,  u.y,  u.z,  0.0f,
				f.x,  f.y,  f.z,  0.0f,
				0.0f, 0.0f, 0.0f, 1.0f );
	return mat * mat4Translation(-eye);
}
