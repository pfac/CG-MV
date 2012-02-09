#ifndef __STD_H__
#define __STD_H__

// STL headers
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <list>
#include <map>
#include <set>

// C headers
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cassert>


// boost headers
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

// Exception
#include "Exception.h"

// Class forward declarations
class Camera;
class Color;
class DownSampler;
class Exception;
class Image;
class ImageManager;
class OpenGLFBO;
class OpenGLFont;
class OpenGLRenderer;
class OpenGLShader;
class OpenGLTexture2D;
class OpenGLTextureCube;
class Math;
class Matrix4;
class SDLException;
class TriMesh;
class TriMeshManager;
class Vector3;

typedef unsigned char	uchar;
typedef unsigned char	uint8;
typedef signed char		sint8;
typedef unsigned short	uint16;
typedef short			sint16;
typedef unsigned int	uint32;
typedef int				sint32;

#endif // __STD_H__
