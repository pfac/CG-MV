#ifndef __COLOR_H__
#define __COLOR_H__

class Color
{
public:
	float r, g, b, a;

public:
	// Constructors
	Color();
	Color(const Color &c);
	Color(float red, float green, float blue, float alpha = 0.0f);
	
	// Cast operators
	operator float* ();
	operator const float* () const;

	// Constants
	static const Color BLACK;
	static const Color WHITE;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
};

inline Color::Color()
{

}

inline Color::Color(const Color &c) : r(c.r), g(c.g), b(c.b), a(c.a)
{

}

inline Color::Color(float red, float green, float blue, float alpha) : r(red), g(green), b(blue), a(alpha)
{

}

inline Color::operator float* ()
{
	return &r;
}

inline Color::operator const float* () const
{
	return &r;
}

#endif // __COLOR_H__
