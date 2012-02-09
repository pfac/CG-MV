#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include "std.h"
#include "Math.h"

class Vector3
{
public:
	float x, y, z;

public:
	// Constructors
	Vector3();
	Vector3(const Vector3 &v);
	Vector3(const float *v);
	Vector3(float vx, float vy, float vz);

	// Conversion operators
	operator float* ();
	operator const float* () const;

	// Access operators
	float  operator [] (int i) const;
	float& operator [] (int i);

	// Assignation operators
	Vector3& operator += (const Vector3 &v);
	Vector3& operator -= (const Vector3 &v);
	Vector3& operator *= (float t);
	Vector3& operator /= (float t);

	// Unary operators
	Vector3 operator + () const;
	Vector3 operator - () const;

	// Binary operators
	Vector3 operator + (const Vector3 &v) const;
	Vector3 operator - (const Vector3 &v) const;
	Vector3 operator * (float t) const;
	Vector3 operator / (float t) const;
	friend Vector3 operator * (const float t, const Vector3 &v);

	// Comparison operators
	bool operator == (const Vector3 &v) const;
	bool operator != (const Vector3 &v) const;
	bool operator < (const Vector3 &v) const;
	bool operator > (const Vector3 &v) const;

	// Normalization
	float norm() const;
	void normalize();

	// vec3 functions
	friend float	vec3Dot(const Vector3 &v1, const Vector3 &v2);
	friend Vector3	vec3Cross(const Vector3 &v1, const Vector3 &v2);
	friend Vector3	vec3Transform(const Vector3 &v, const Matrix4 &m);

	// Constants
	static const Vector3 ZERO;
	static const Vector3 UNIT_X;
	static const Vector3 UNIT_Y;
	static const Vector3 UNIT_Z;

	// Debug
	void print()
	{
		printf("X: %.2f \tY: %.2f \tZ: %.2f \tNorm: %.2f \n", x, y, z, norm());
	}
};

inline Vector3::Vector3()
{

}


inline Vector3::Vector3(const Vector3 &v) : x(v.x), y(v.y), z(v.z)
{

}


inline Vector3::Vector3(const float* v) : x(v[0]), y(v[1]), z(v[2])
{

}


inline Vector3::Vector3(float vx, float vy, float vz) : x(vx), y(vy), z(vz)
{

}


inline Vector3::operator float* ()
{
	return &x;
}


inline Vector3::operator const float* () const
{
	return &x;
}


inline float Vector3::operator [] (int i) const
{
	assert(i>=0 && i<3);
	return *(&x+i);
}


inline float& Vector3::operator [] (int i)
{
	assert(i>=0 && i<3);
	return *(&x+i);
}


inline Vector3& Vector3::operator += (const Vector3 &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}


inline Vector3& Vector3::operator -= (const Vector3 &v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}


inline Vector3& Vector3::operator *= (float t)
{
	x *= t;
	y *= t;
	z *= t;
	return *this;
}


inline Vector3& Vector3::operator /= (float t)
{
	assert(t != 0.0f);
	float inv = 1.0f / t;
	x *= inv;
	y *= inv;
	z *= inv;
	return *this;
}


inline Vector3 Vector3::operator + () const
{
	return *this;
}


inline Vector3 Vector3::operator - () const
{
	return Vector3(-x, -y, -z);
}


inline Vector3 Vector3::operator + (const Vector3 &v) const
{
	return Vector3(x+v.x, y+v.y, z+v.z);
}


inline Vector3 Vector3::operator - (const Vector3 &v) const
{
	return Vector3(x-v.x, y-v.y, z-v.z);
}


inline Vector3 Vector3::operator * (float t) const
{
	return Vector3(x*t, y*t, z*t);
}


inline Vector3 Vector3::operator / (float t) const
{
	assert(t != 0.0f);
	float inv = 1.0f / t;
	return Vector3(x*inv, y*inv, z*inv);
}


inline Vector3 operator * (const float t, const Vector3 &v)
{
	return Vector3(v.x*t, v.y*t, v.z*t);
}


inline bool Vector3::operator == (const Vector3 &v) const
{
	return ((x == v.x) && (y == v.y) && (z == v.z));
}


inline bool Vector3::operator != (const Vector3 &v) const
{
	return ((x != v.x) || (y != v.y) || (z != v.z));

}


inline bool Vector3::operator < (const Vector3 &v) const
{
	/*
	if(x<v.x && y<v.y && z<v.z)
		return true;
	return false;
	*/

	if (x < v.x)
		return true;
	if (x == v.x && y < v.y)
		return true;
	if (x == v.x && y == v.y && z < v.z)
		return true;

	return false;
}

inline bool Vector3::operator > (const Vector3 &v) const
{
	if(x>v.x && y>v.y && z>v.z)
		return true;
	return false;
}


inline float Vector3::norm() const
{
	return Math::sqrt(x*x + y*y + z*z);
}


inline void Vector3::normalize()
{
	float length = norm();
	if (length != 0.0f)
	{
		float inv = 1.0f / length;
		x *= inv;
		y *= inv;
		z *= inv;
	}
}


#endif // __VECTOR3_H__
