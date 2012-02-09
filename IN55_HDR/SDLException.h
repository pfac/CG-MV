#ifndef __SDL_EXCEPTION_H__
#define __SDL_EXCEPTION_H__

#include <exception>
#include <string>
#include <SDL.h>

class SDLException : public std::exception
{
public:
	SDLException(const std::string &funcName) throw()
	{
		msg = "Error in \"" + funcName + "\" : " + std::string(SDL_GetError());
	}

	~SDLException() throw() {}

	const char *what() const throw()
	{
		return msg.c_str();
	}

private:
	std::string msg;
};

#endif // __SDL_EXCEPTION_H__
