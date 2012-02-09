#ifndef __OPENGL_SHADER_H__
#define __OPENGL_SHADER_H__

#include "std.h"
#include "OpenGL.h"

class OpenGLShader
{
public:
	 OpenGLShader();
	~OpenGLShader();

	// Read the shader from file : vertex and fragment programs
	void load(const std::string &vert, const std::string &frag);

	// Release OpenGL memory
	void release();

	// Start using this shader
	void begin() const;

	// Stop using a shader
	static void end();

	// Send uniform variables to shader
	void setUniform(const char* str, int i) const;
	void setUniform(const char* str, float f) const;
	void setUniform(const char* str, const Color &c) const;
	void setUniform(const char* str, const Vector3 &v) const;

	// Directory containg the shaders
	static void setDir(const std::string &path);

private:
	GLhandleARB progObj;
	GLhandleARB vertObj;
	GLhandleARB fragObj;

	static std::string dir;

	void loadShaderFromFile(GLhandleARB obj, const std::string &filename);
};

#endif	// __OPENGL_SHADER_H__
