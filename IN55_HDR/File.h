/**
 * Não é relevante, diria eu
 * naps62
 */

#ifndef __FILE_H__
#define __FILE_H__

#include "std.h"

class File
{
private:
	// Complete file path
    std::string name;

public:
	// Constructor starting with a std::string
    File(const std::string &n = "unknown");

    // Constructor starting with a const char*
    File(const char* n);

    // Is the file open?
    bool isOpen() const;

    // Get fullname (with the path)
    std::string getFullName() const;

    // Get filename (without the path)
    std::string getFileName() const;

	// Get shortfilename (without the path and without the extension)
    std::string getShortFileName() const;

    // Get file extension
    std::string getExtension() const;
};

#endif // __FILE_H__
