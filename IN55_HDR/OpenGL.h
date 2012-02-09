#ifndef __OPENGL_H__
#define __OPENGL_H__

#ifdef WIN32
	#include <windows.h>
	#pragma comment(lib, "SDL.lib")
	#pragma comment(lib, "SDLmain.lib")
	#pragma comment(lib, "opengl32")
#endif

#include <SDL.h>
#include "GLee.h"

#endif // __OPENGL_H__
