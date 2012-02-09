#include "OpenGLTextureCube.h"
#include "ImageManager.h"
#include "AllocOverLoaderOn.h"

OpenGLTextureCube::OpenGLTextureCube()
{
	id = 0;
}

OpenGLTextureCube::~OpenGLTextureCube()
{
	assert(id == 0);
}

void OpenGLTextureCube::release()
{
	if(id > 0)
	{
		glDeleteTextures(1, &id);
		id = 0;
	}
}

void OpenGLTextureCube::load(const std::string &filename, GLint internalformat)
{
	// Generate id
	assert(id == 0);
	glGenTextures(1, &id);

	// Bind texture and active bilinear filtering
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Load images
	ImageManager::get()->extractCubeMap(filename);
	Image *imPosX = ImageManager::get()->getCubeFace(filename, ImageManager::CUBE_FACE_POSITIVE_X);
	Image *imNegX = ImageManager::get()->getCubeFace(filename, ImageManager::CUBE_FACE_NEGATIVE_X);
	Image *imPosY = ImageManager::get()->getCubeFace(filename, ImageManager::CUBE_FACE_POSITIVE_Y);
	Image *imNegY = ImageManager::get()->getCubeFace(filename, ImageManager::CUBE_FACE_NEGATIVE_Y);
	Image *imPosZ = ImageManager::get()->getCubeFace(filename, ImageManager::CUBE_FACE_POSITIVE_Z);
	Image *imNegZ = ImageManager::get()->getCubeFace(filename, ImageManager::CUBE_FACE_NEGATIVE_Z);

	// Upload textures
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalformat, imPosX->getSizeX(), imPosX->getSizeY(), 0, imPosX->getFormat(), imPosX->getType(), imPosX->getData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalformat, imNegX->getSizeX(), imNegX->getSizeY(), 0, imNegX->getFormat(), imNegX->getType(), imNegX->getData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalformat, imPosY->getSizeX(), imPosY->getSizeY(), 0, imPosY->getFormat(), imPosY->getType(), imPosY->getData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalformat, imNegY->getSizeX(), imNegY->getSizeY(), 0, imNegY->getFormat(), imNegY->getType(), imNegY->getData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalformat, imPosZ->getSizeX(), imPosZ->getSizeY(), 0, imPosZ->getFormat(), imPosZ->getType(), imPosZ->getData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalformat, imNegZ->getSizeX(), imNegZ->getSizeY(), 0, imNegZ->getFormat(), imNegZ->getType(), imNegZ->getData());
}

void OpenGLTextureCube::bind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}
