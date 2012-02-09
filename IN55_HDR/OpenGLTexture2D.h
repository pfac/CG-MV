#ifndef __OPENGL_TEXTURE_2D_H__
#define __OPENGL_TEXTURE_2D_H__

#include "OpenGL.h"
#include "std.h"

class OpenGLTexture2D
{
public:
	OpenGLTexture2D();
	~OpenGLTexture2D();
	
	// Load the texture from a file and convert it to the given format
	void load(const std::string &filename, GLint internalformat);

	// Free OpenGL memory
	void release();

	// Bind the texture to the current texture unit
	void bind() const;

private:
	// OpenGL texture id
	GLuint id;
};

#endif // __OPENGL_TEXTURE_2D_H__
