#ifndef __MATH_H__
#define __MATH_H__

#include "std.h"

class Math
{
public:
	// Trigonometric functions
	static float cos(float rad);
	static float sin(float rad);
	static float tan(float rad);
	static float acos(float rad);
	static float asin(float rad);
	static float atan(float rad);
	static float rad2deg(float rad);
	static float deg2rad(float deg);

	// Misc functions
	static float exp(float t);
	static float abs(float t);
	static float sqrt(float t);
	static int floor(float t);
	static int ceil(float t);
	static int clamp(int val, int min, int max);
	static float clamp(float val, float min, float max);

	// Constants
	static const float PI;
	static const float PI_DIV_2;
	static const float PI_DIV_4;
};


inline float Math::cos(float rad)
{
	return cosf(rad);
}

inline float Math::sin(float rad)
{
	return sinf(rad);
}

inline float Math::tan(float rad)
{
	return tanf(rad);
}

inline float Math::acos(float rad)
{
	return acosf(rad);
}

inline float Math::asin(float rad)
{
	return asinf(rad);
}

inline float Math::atan(float rad)
{
	return atanf(rad);
}

inline float Math::rad2deg(float rad)
{
	return rad * 180.0f / PI;
}

inline float Math::deg2rad(float deg)
{
	return deg * PI / 180.0f;
}

inline float Math::exp(float t)
{
	return ::exp(t);
}

inline float Math::abs(float t)
{
	return fabsf(t);
}

inline float Math::sqrt(float t)
{
	return sqrtf(t);
}

inline int Math::floor(float t)
{
	return (int)::floorf(t);
}

inline int Math::ceil(float t)
{
	return (int)::ceilf(t);
}

inline int Math::clamp(int val, int min, int max)
{
	assert(min <= max);

	if (val > max)
		val = max;

	if (val < min)
		val = min;

	return val;
}

inline float Math::clamp(float val, float min, float max)
{
	assert(min <= max);

	if (val > max)
		val = max;

	if (val < min)
		val = min;

	return val;
}

#endif // __MATH_H__
