#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include "std.h"
#include "Math.h"
#include "Vector3.h"

class Matrix4
{
public:
	float m[16];

public:
	// Constructors
	Matrix4();
	Matrix4(const float *matrix);
	Matrix4(const Matrix4 &matrix);
	Matrix4(float m00, float m01, float m02, float m03, 
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);

	// Conversion operators
	operator float* ();
	operator const float* () const;

	// Access operators
	float& operator () (int row, int col);
	const float operator () (int row, int col) const;

	// Index operators
	float& operator [] (int i);
	const float operator [] (int i) const;

	// Assignation operators
	const Matrix4& operator += (const Matrix4 &matrix);
	const Matrix4& operator -= (const Matrix4 &matrix);
	const Matrix4& operator *= (const Matrix4 &matrix);

	// Opérateurs binaires
	Matrix4 operator + (const Matrix4 &matrix) const;
	Matrix4 operator - (const Matrix4 &matrix) const;
	Matrix4 operator * (const Matrix4 &matrix) const;

	// Opérateurs de comparaison
	bool operator == (const Matrix4 &matrix) const;
	bool operator != (const Matrix4 &matrix) const;

	// Translation
	void setTranslation(const Vector3 &v);
	Vector3	getTranslation() const;

	// mat4 functions
	friend Matrix4 mat4RotationAxis(const Vector3 &axis, float rad);

	// Construit une matrice de rotation centrée en (0, 0, 0)
	friend Matrix4 mat4RotationX(float rad);
	friend Matrix4 mat4RotationY(float rad);
	friend Matrix4 mat4RotationZ(float rad);

	// Construit une matrice de rotation en spécifiant le centre
	friend Matrix4 mat4RotationX(float rad, const Vector3& center);
    friend Matrix4 mat4RotationY(float rad, const Vector3& center);
    friend Matrix4 mat4RotationZ(float rad, const Vector3& center);

	friend Matrix4 mat4Translation(const Vector3 &v);

	// create a frustum matrix
	friend Matrix4 mat4Frustum(float left, float right, float bottom, float top, float near, float far);
	friend Matrix4 mat4FrustumInf(float left, float right, float bottom, float top, float near);

	// create a lookAt matrix (same as gluLookAt)
	friend Matrix4 mat4LookAt(const Vector3 &eye, const Vector3 &center, const Vector3 &up);

	// Constantes
	static const Matrix4 ZERO;
	static const Matrix4 IDENTITY;

	// Debug
	void print()
	{
		printf("%.2f\t%.2f\t%.2f\t%.2f\n", m[0], m[4], m[ 8], m[12]);
		printf("%.2f\t%.2f\t%.2f\t%.2f\n", m[1], m[5], m[ 9], m[13]);
		printf("%.2f\t%.2f\t%.2f\t%.2f\n", m[2], m[6], m[10], m[14]);
		printf("%.2f\t%.2f\t%.2f\t%.2f\n", m[3], m[7], m[11], m[15]);
	}

};


inline Matrix4::Matrix4()
{

}

inline Matrix4::Matrix4(const Matrix4& mat)
{
	for (int i=0 ; i<16 ; i++)
		m[i] = mat.m[i];
}

inline Matrix4::Matrix4(const float* mat)
{
	for (int i=0 ; i<16 ; i++)
		m[i] = mat[i];
}

inline Matrix4::Matrix4(float m00, float m01, float m02, float m03, 
						float m10, float m11, float m12, float m13,
						float m20, float m21, float m22, float m23,
						float m30, float m31, float m32, float m33)
{
	m[ 0] = m00;
	m[ 1] = m10;
	m[ 2] = m20;
	m[ 3] = m30;

	m[ 4] = m01;
	m[ 5] = m11;
	m[ 6] = m21;
	m[ 7] = m31;

	m[ 8] = m02;
	m[ 9] = m12;
	m[10] = m22;
	m[11] = m32;

	m[12] = m03;
	m[13] = m13;
	m[14] = m23;
	m[15] = m33;
}

inline Matrix4::operator float* ()
{
	return m;
}

inline Matrix4::operator const float* () const
{
	return m;
}

inline float& Matrix4::operator () (int row, int col)
{
	assert(row>=0 && row<4);
	assert(col>=0 && col<4);
	return m[col*4+row];
}

inline const float Matrix4::operator () (int row, int col) const
{
	assert(row>=0 && row<4);
	assert(col>=0 && col<4);
	return m[col*4+row];
}

inline float& Matrix4::operator [] (int i)
{
	assert(i>=0 && i<16);
	return m[i];
}

inline const float Matrix4::operator [] (int i) const
{
	assert(i>=0 && i<16);
	return m[i];
}

inline const Matrix4& Matrix4::operator += (const Matrix4 &matrix)
{
	m[ 0] += matrix.m[ 0]; m[ 4] += matrix.m[ 4]; m[ 8] += matrix.m[ 8]; m[12] += matrix.m[12]; 
	m[ 1] += matrix.m[ 1]; m[ 5] += matrix.m[ 5]; m[ 9] += matrix.m[ 9]; m[13] += matrix.m[13];
	m[ 2] += matrix.m[ 2]; m[ 6] += matrix.m[ 6]; m[10] += matrix.m[10]; m[14] += matrix.m[14];
	m[ 3] += matrix.m[ 3]; m[ 7] += matrix.m[ 7]; m[11] += matrix.m[11]; m[15] += matrix.m[15];

    return *this;
}

inline const Matrix4& Matrix4::operator -= (const Matrix4 &matrix)
{
	m[ 0] -= matrix.m[ 0]; m[ 4] -= matrix.m[ 4]; m[ 8] -= matrix.m[ 8]; m[12] -= matrix.m[12]; 
	m[ 1] -= matrix.m[ 1]; m[ 5] -= matrix.m[ 5]; m[ 9] -= matrix.m[ 9]; m[13] -= matrix.m[13];
	m[ 2] -= matrix.m[ 2]; m[ 6] -= matrix.m[ 6]; m[10] -= matrix.m[10]; m[14] -= matrix.m[14];
	m[ 3] -= matrix.m[ 3]; m[ 7] -= matrix.m[ 7]; m[11] -= matrix.m[11]; m[15] -= matrix.m[15];

    return *this;
}

inline const Matrix4& Matrix4::operator *= (const Matrix4 &matrix)
{
	*this = *this * matrix;
    return *this;
}

inline Matrix4 Matrix4::operator + (const Matrix4& mat) const
{
	Matrix4 result;

	for (int i=0 ; i<16 ; i++)
		result.m[i] = m[i] + mat.m[i];

	return result;
}


inline Matrix4 Matrix4::operator - (const Matrix4& mat) const
{
	Matrix4 result;

	for (int i=0 ; i<16 ; i++)
		result.m[i] = m[i] - mat.m[i];

	return result;
}


inline Matrix4 Matrix4::operator * (const Matrix4& m2) const
{
	Matrix4 r;

	r.m[ 0] = m[0]*m2.m[ 0] + m[4]*m2.m[ 1] + m[ 8]*m2.m[ 2] + m[12]*m2.m[ 3];
	r.m[ 1] = m[1]*m2.m[ 0] + m[5]*m2.m[ 1] + m[ 9]*m2.m[ 2] + m[13]*m2.m[ 3];
	r.m[ 2] = m[2]*m2.m[ 0] + m[6]*m2.m[ 1] + m[10]*m2.m[ 2] + m[14]*m2.m[ 3];
	r.m[ 3] = m[3]*m2.m[ 0] + m[7]*m2.m[ 1] + m[11]*m2.m[ 2] + m[15]*m2.m[ 3];

	r.m[ 4] = m[0]*m2.m[ 4] + m[4]*m2.m[ 5] + m[ 8]*m2.m[ 6] + m[12]*m2.m[ 7];
	r.m[ 5] = m[1]*m2.m[ 4] + m[5]*m2.m[ 5] + m[ 9]*m2.m[ 6] + m[13]*m2.m[ 7];
	r.m[ 6] = m[2]*m2.m[ 4] + m[6]*m2.m[ 5] + m[10]*m2.m[ 6] + m[14]*m2.m[ 7];
	r.m[ 7] = m[3]*m2.m[ 4] + m[7]*m2.m[ 5] + m[11]*m2.m[ 6] + m[15]*m2.m[ 7];

	r.m[ 8] = m[0]*m2.m[ 8] + m[4]*m2.m[ 9] + m[ 8]*m2.m[10] + m[12]*m2.m[11];
	r.m[ 9] = m[1]*m2.m[ 8] + m[5]*m2.m[ 9] + m[ 9]*m2.m[10] + m[13]*m2.m[11];
	r.m[10] = m[2]*m2.m[ 8] + m[6]*m2.m[ 9] + m[10]*m2.m[10] + m[14]*m2.m[11];
	r.m[11] = m[3]*m2.m[ 8] + m[7]*m2.m[ 9] + m[11]*m2.m[10] + m[15]*m2.m[11];

	r.m[12] = m[0]*m2.m[12] + m[4]*m2.m[13] + m[ 8]*m2.m[14] + m[12]*m2.m[15];
	r.m[13] = m[1]*m2.m[12] + m[5]*m2.m[13] + m[ 9]*m2.m[14] + m[13]*m2.m[15];
	r.m[14] = m[2]*m2.m[12] + m[6]*m2.m[13] + m[10]*m2.m[14] + m[14]*m2.m[15];
	r.m[15] = m[3]*m2.m[12] + m[7]*m2.m[13] + m[11]*m2.m[14] + m[15]*m2.m[15];

	return r;
}

inline bool Matrix4::operator == (const Matrix4& m2) const
{
	if( m[0]  != m2.m[0]  || m[1]  != m2.m[1]  || m[2]  != m2.m[2]  || m[3]  != m2.m[3]  ||
		m[4]  != m2.m[4]  || m[5]  != m2.m[5]  || m[6]  != m2.m[6]  || m[7]  != m2.m[7]  ||
		m[8]  != m2.m[8]  || m[9]  != m2.m[9]  || m[10] != m2.m[10] || m[11] != m2.m[11] ||
		m[12] != m2.m[12] || m[13] != m2.m[13] || m[14] != m2.m[14] || m[15] != m2.m[15]  )
		return false;
	return true;
}

inline bool Matrix4::operator != (const Matrix4& m2) const
{
	if( m[0]  != m2.m[0]  || m[1]  != m2.m[1]  || m[2]  != m2.m[2]  || m[3]  != m2.m[3]  ||
		m[4]  != m2.m[4]  || m[5]  != m2.m[5]  || m[6]  != m2.m[6]  || m[7]  != m2.m[7]  ||
		m[8]  != m2.m[8]  || m[9]  != m2.m[9]  || m[10] != m2.m[10] || m[11] != m2.m[11] ||
		m[12] != m2.m[12] || m[13] != m2.m[13] || m[14] != m2.m[14] || m[15] != m2.m[15]  )
		return true;
	return false;
}

inline void Matrix4::setTranslation(const Vector3 &v)
{
	m[12] = v.x;
	m[13] = v.y;
	m[14] = v.z;
}

inline Vector3 Matrix4::getTranslation() const
{
	return Vector3(m[12], m[13], m[14]);
}

#endif	// __MATRIX4_H__
