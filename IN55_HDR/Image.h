#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "std.h"
#include "OpenGL.h"

class Image
{
public:
	Image();
	~Image();

	// Get image data as uchar pointer
	uchar* getData() const { return data; }

	// Get image data as float pointer
	float* getDataFloat() const { return reinterpret_cast<float*>(data); }

	// Get width of the image
	int getSizeX() const { return sizeX; }

	// Get height of the image
	int getSizeY() const { return sizeY; }

	// Read image from file
	void loadFromBmp(const std::string &filename);
	void loadFromHdr(const std::string &filename);

	// Get OpenGL associated format and type
	GLenum getFormat() const { return format; }
	GLenum getType() const { return type; }

	// Flip image
	void flipHorizontal();
	void flipVertical();

	// Get the number of bytes of one pixel
	int getPixelSize() const;

private:
	// Image data
	uchar *data;

	// Image size
	int sizeX, sizeY;

	// OpenGL format and type
	GLenum format;
	GLenum type;

	// ImageManager need access to private data
	friend class ImageManager;
};

#endif // __IMAGE_H__
