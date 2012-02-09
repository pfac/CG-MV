#include "File.h"
#include "AllocOverLoaderOn.h"

File::File(const std::string &n) : name(n)
{
	std::replace(name.begin(), name.end(), '/', '\\');
}

File::File(const char *n) : name(n)
{
	std::replace(name.begin(), name.end(), '/', '\\');
}

bool File::isOpen() const
{
	std::ifstream file(name.c_str());
	return file.is_open();
}

std::string File::getFullName() const
{
    return name;
}

std::string File::getFileName() const
{
	std::string::size_type pos = name.find_last_of("\\");

    if (pos != std::string::npos)
        return name.substr(pos + 1, std::string::npos);
    else
        return name;
}

std::string File::getShortFileName() const
{
	return getFileName().substr(0, getFileName().find_last_of("."));
}

std::string File::getExtension() const
{
    std::string::size_type pos = name.find_last_of(".");

    if (pos != std::string::npos)
        return name.substr(pos + 1, std::string::npos);
    else
        return "";
}
