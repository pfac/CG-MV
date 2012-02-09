#include "OpenGLTexture2D.h"
#include "ImageManager.h"

OpenGLTexture2D::OpenGLTexture2D()
{
	id = 0;
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	assert(id == 0);
}

void OpenGLTexture2D::release()
{
	if(id > 0)
	{
		glDeleteTextures(1, &id);
		id = 0;
	}
}

void OpenGLTexture2D::load(const std::string &filename, GLint internalformat)
{
	// Generate id
	assert(id == 0);
	glGenTextures(1, &id);

	// Bind texture and activate bilinear filtering
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Get image
	Image *im = ImageManager::get()->getImage(filename);

	// Get image informations
	GLsizei w = im->getSizeX();
	GLsizei h = im->getSizeY();
	GLenum format = im->getFormat();
	GLenum type = im->getType();

	// Upload texture
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, w, h, 0, format, type, im->getData());
}

void OpenGLTexture2D::bind() const
{
	glBindTexture(GL_TEXTURE_2D, id);
}

