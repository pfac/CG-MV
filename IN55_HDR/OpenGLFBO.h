#ifndef __OPENGL_FBO_H__
#define __OPENGL_FBO_H__

#include "OpenGL.h"
#include "std.h"

class OpenGLFBO
{
public:
	OpenGLFBO();
	~OpenGLFBO();

	// Create a render texture
	void init(int w, int h, int format);

	// Free OpenGL memory
	void release();

	// Use it as a texture
	void bind();

	// Render to this texture
	void beginCapture();
	static void endCapture();

	// Size of the texture
	int getW() const { return textureW; }
	int getH() const { return textureH; }

private:
	GLuint fb;
	GLuint depth_rb;
	GLuint color_tex;
	GLint internalformat;
	GLenum target;

	int textureW, textureH;
};

#endif // __OPENGL_FBO_H__
