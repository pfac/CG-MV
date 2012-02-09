#ifndef __IMAGE_MANAGER_H__
#define __IMAGE_MANAGER_H__

#include "std.h"
#include "Image.h"
#include "Singleton.h"

class ImageManager : public Singleton<ImageManager>
{
public:
	// Directory containing images
	void setDir(const std::string &path);

	// Get the image loaded from 'filename'
	Image* getImage(const std::string &filename);

	// Cube map management
	enum CUBE_FACE
	{
		CUBE_FACE_POSITIVE_X,
		CUBE_FACE_NEGATIVE_X,
		CUBE_FACE_POSITIVE_Y,
		CUBE_FACE_NEGATIVE_Y,
		CUBE_FACE_POSITIVE_Z,
		CUBE_FACE_NEGATIVE_Z,
	};

	// Load and extract the 6 cube map faces
	void extractCubeMap(const std::string &filename);

	// Get a cube map face
	Image* getCubeFace(const std::string &filename, CUBE_FACE face);

	// Get a short name for the face
	std::string getFaceName(CUBE_FACE face);

private:
	ImageManager();
	~ImageManager();

	std::map<std::string, Image*> images;
	std::map<std::string, Image*>::iterator it;
	std::string dir;

	friend class Singleton<ImageManager>;
};

#endif // __IMAGE_MANAGER_H__
