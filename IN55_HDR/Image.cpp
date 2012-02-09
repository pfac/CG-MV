#include "Image.h"
#include "rgbe.h"
#include "AllocOverLoaderOn.h"

Image::Image()
{
	data = 0;
	sizeX = sizeY = 0;
}

Image::~Image()
{
	if (data)
		delete [] data;
}

void Image::loadFromBmp(const std::string &filename)
{
	// Open the file
	FILE* f = fopen(filename.c_str(), "rb");
	if (f == 0)
		throw Exception("File missing : " + filename);
    
    // Read width and height
    fseek(f, 18, SEEK_CUR);
    fread(&sizeX, 4, 1, f);
    fread(&sizeY, 4, 1, f);
    
    // Image size in BGR and RGBA
    uint32 size3 = sizeX * sizeY * 3;
	uint32 size4 = sizeX * sizeY * 4;

    // Check planes
	uint16 planes;
	fread(&planes, 2, 1, f);
    if (planes != 1)
		throw Exception("Incorrect bitmap file : " +filename);

    // Check bits per pixel
	uint16 bpp;
	fread(&bpp, 2, 1, f);
    if (bpp != 24)
		throw Exception("Incorrect bitmap file : " + filename);

    // Allocate memory
	data = new uchar[size4];
	uchar* tempData = new uchar[size3];

	// Read BGR image
	fseek(f, 24, SEEK_CUR);
	fread(tempData, size3, 1, f);

	// Convert BGR to RGBA
	unsigned long c1, c2;
    for (c1=0, c2=0 ; c1<size4 ; c1+=4, c2+=3) 
	{ 
		data[c1+0]	= tempData[c2+2];
		data[c1+1]	= tempData[c2+1];
		data[c1+2]	= tempData[c2];
		data[c1+3]	= 255;
    }

	// Free temporary memory
	delete [] tempData;

	// Set format and type
	format = GL_RGBA;
	type = GL_UNSIGNED_BYTE;
}

void Image::loadFromHdr(const std::string &filename)
{
	// Try to open the file
	FILE* f = fopen(filename.c_str(), "rb");
	if (f == 0)
		throw Exception("File missing : " + filename);
	
	// Read image header
	int res = RGBE_ReadHeader(f, &sizeX, &sizeY, 0);
	if (res < 0)
		throw Exception("Incorrect .hdr file : " + filename);

	// Read image data
	data = new uchar[sizeof(float)*3*sizeX*sizeY];
	res = RGBE_ReadPixels_RLE(f, reinterpret_cast<float*>(data), sizeX, sizeY);

	// Set format and type
	format = GL_RGB;
	type = GL_FLOAT;

	// Close the file
	fclose(f);
}

void Image::flipHorizontal()
{
	const int stride = getPixelSize();
	const int w = sizeX;
	const int h = sizeY;
	const int m = h / 2;
	uchar *temp = new uchar[stride];

	for (int y=0 ; y<m ; ++y)
	{
		for (int x=0 ; x<w ; ++x)
		{
			int offset = (y*w+x)*stride;
			int offsetOpp = ((h-y-1)*w+x)*stride;

			memcpy(temp, data+offset, stride);
			memcpy(data+offset, data+offsetOpp, stride);
			memcpy(data+offsetOpp, temp, stride);
		}
	}
	delete [] temp;
}

void Image::flipVertical()
{
	const int stride = getPixelSize();
	const int w = sizeX;
	const int h = sizeY;
	const int m = w / 2;
	uchar *temp = new uchar[stride];

	for (int y=0 ; y<h ; ++y)
	{
		for (int x=0 ; x<m ; ++x)
		{
			int offset = (y*w+x)*stride;
			int offsetOpp = (y*w+(w-x-1))*stride;

			memcpy(temp, data+offset, stride);
			memcpy(data+offset, data+offsetOpp, stride);
			memcpy(data+offsetOpp, temp, stride);
		}
	}
	delete [] temp;
}

int Image::getPixelSize() const
{
	int nbComponent = 0;
	switch (format)
	{
	case GL_RGBA:
		nbComponent = 4;
		break;
	case GL_RGB:
		nbComponent = 3;
		break;
	default:
		throw Exception("getPixelSize() : bad image format");
	}

	int componentSize = 0;
	switch (type)
	{
	case GL_UNSIGNED_BYTE:
		componentSize = sizeof(uchar);
		break;
	case GL_FLOAT:
		componentSize = sizeof(float);
		break;
	default:
		throw Exception("getPixelSize() : bad image type");
	}

	return nbComponent * componentSize;
}

