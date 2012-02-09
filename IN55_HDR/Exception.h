#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <exception>
#include <string>

class Exception : public std::exception
{
public:
	Exception(const std::string &error) throw() : msg(error) {}
	~Exception() throw() {}
	const char *what() const throw() { return msg.c_str(); }

private:
	std::string msg;
};

#endif // __EXCEPTION_H__
