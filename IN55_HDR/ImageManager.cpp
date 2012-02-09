#include "ImageManager.h"
#include "AllocOverLoaderOn.h"

ImageManager::ImageManager()
{
	setDir("media/");
}

ImageManager::~ImageManager()
{
	// Destroy all images
	for (it=images.begin() ; it!=images.end() ; ++it)
		delete it->second;
	images.clear();
}

void ImageManager::setDir(const std::string &path)
{
	dir = path;
}

Image* ImageManager::getImage(const std::string &filename)
{
	// Check if image is already loaded
	it = images.find(filename);
	if (it != images.end())
		return it->second;

	// Get file extension
	size_t pos = filename.find('.');
	if (pos == std::string::npos)
		throw Exception("Bad filename : " + filename);
	std::string ext(filename, pos+1);

	// Create a new image
	Image *im = new Image();
	if (ext == "bmp")
		im->loadFromBmp((dir+filename).c_str());
	else if (ext == "hdr")
		im->loadFromHdr((dir+filename).c_str());
	else
		throw Exception("File type not supported : " + std::string(filename));

	// Add it to the map
	images[filename] = im;
	return im;
}

void ImageManager::extractCubeMap(const std::string &filename)
{
	// Check if cube map is already extracted
	it = images.find(filename);
	if (it != images.end())
		return;

	// Get informations from cross image
	Image *cubeMap = getImage(filename);
	int crossW = cubeMap->getSizeX();
	int crossH = cubeMap->getSizeY();

	// Calculate faces width and height
	int w = crossW / 3;
	int h = crossH / 4;

	// Check that the input image is really a cross cube map
	if (w != h)
		throw Exception("This image is not a valide cube map : " + filename);

	// Creates 6 images (faces of the cube map)
	Image *cubeFaces[6];
	for (int i=0 ; i<6 ; i++)
	{
		// Create face and allocate memory
		cubeFaces[i] = new Image();
		cubeFaces[i]->data = new uint8[sizeof(float)*3*w*h];
		cubeFaces[i]->format = GL_RGB;
		cubeFaces[i]->type = GL_FLOAT;
		cubeFaces[i]->sizeX = w;
		cubeFaces[i]->sizeY = h;

		// Add the face to the images map : prefix it with face name
		images[getFaceName(CUBE_FACE(i))+filename] = cubeFaces[i];
	}

	// Get float pointer from source image (the cross cube map)
	float *srcData = cubeMap->getDataFloat();

	// Extract each face from original cross image
	for (int y=0 ; y<crossH ; ++y)
	{
		for (int x=0 ; x<3 ; ++x)
		{
			// Calculate stride, source offset and face offset
			int stride = w * 3 * sizeof(float);
			int srcOffset = 3*(y*crossW+x*w);
			int offset = 3*((y%h)*w);

			// CUBE_FACE_POSITIVE_X
			if (y>=h && y<2*h && x==2)
				memcpy(cubeFaces[CUBE_FACE_POSITIVE_X]->getDataFloat()+offset, srcData+srcOffset, stride);

			// CUBE_FACE_NEGATIVE_X
			if (y>=h && y<2*h && x==0)
				memcpy(cubeFaces[CUBE_FACE_NEGATIVE_X]->getDataFloat()+offset, srcData+srcOffset, stride);

			// CUBE_FACE_POSITIVE_Y
			if (y>=0 && y<h && x==1)
				memcpy(cubeFaces[CUBE_FACE_POSITIVE_Y]->getDataFloat()+offset, srcData+srcOffset, stride);

			// CUBE_FACE_NEGATIVE_Y
			if (y>=2*h && y<3*h && x==1)
				memcpy(cubeFaces[CUBE_FACE_NEGATIVE_Y]->getDataFloat()+offset, srcData+srcOffset, stride);

			// CUBE_FACE_POSITIVE_Z
			if (y>=h && y<2*h && x==1)
				memcpy(cubeFaces[CUBE_FACE_POSITIVE_Z]->getDataFloat()+offset, srcData+srcOffset, stride);

			// CUBE_FACE_NEGATIVE_Z
			if (y>=3*h && y<4*h && x==1)
				memcpy(cubeFaces[CUBE_FACE_NEGATIVE_Z]->getDataFloat()+offset, srcData+srcOffset, stride);
		}
	}

	// CUBE_FACE_NEGATIVE_Z need to be flipped
	cubeFaces[CUBE_FACE_NEGATIVE_Z]->flipHorizontal();
	cubeFaces[CUBE_FACE_NEGATIVE_Z]->flipVertical();
}

Image* ImageManager::getCubeFace(const std::string &filename, CUBE_FACE face)
{
	// Check if face was generated
	it = images.find(getFaceName(face)+filename);
	if (it != images.end())
		return it->second;

	throw Exception("You must first extract the cube map : " + std::string(filename));
	return 0;
}

std::string ImageManager::getFaceName(CUBE_FACE face)
{
	std::string name;
	switch (face)
	{
	case CUBE_FACE_POSITIVE_X:
		return "PosX_";

	case CUBE_FACE_NEGATIVE_X:
		return "NegX_";

	case CUBE_FACE_POSITIVE_Y:
		return "PosY_";

	case CUBE_FACE_NEGATIVE_Y:
		return "NegY_";

	case CUBE_FACE_POSITIVE_Z:
		return "PosZ_";

	case CUBE_FACE_NEGATIVE_Z:
		return "NegZ_";

	default:
		throw Exception("Bad cube face");
		return "";
	}
}
