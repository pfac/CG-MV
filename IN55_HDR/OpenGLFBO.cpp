#include "OpenGLFBO.h"

OpenGLFBO::OpenGLFBO()
{
	fb = depth_rb = color_tex = 0;
	internalformat = 0;
	target = GL_TEXTURE_2D;
	textureW = textureH = 0;
}

OpenGLFBO::~OpenGLFBO()
{
	release();
}

void OpenGLFBO::bind()
{
	glEnable(target);
	glBindTexture(target, color_tex);
}

void OpenGLFBO::init(int w, int h, int format)
{
	// Size
	textureW = w;
	textureH = h;
	internalformat = format;

	// initalize FBO
	glGenFramebuffersEXT(1, &fb);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);

	// initialize color texture
	glGenTextures(1, &color_tex);
	glBindTexture(target, color_tex);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(target, 0, internalformat, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, target, color_tex, 0);

	// initialize depth renderbuffer
	glGenRenderbuffersEXT(1, &depth_rb);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);

	// check for errors
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			break;
		default:
			throw Exception("FBO creation error");
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void OpenGLFBO::release()
{
	if (depth_rb != 0)
	{
		glDeleteRenderbuffersEXT(1, &depth_rb);
		depth_rb = 0;
	}

	if (color_tex != 0)
	{
		glDeleteTextures(1, &color_tex);
		color_tex = 0;
	}

	if (fb != 0)
	{
		glDeleteFramebuffersEXT(1, &fb);
		fb = 0;
	}
}

void OpenGLFBO::beginCapture()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
}

void OpenGLFBO::endCapture()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
